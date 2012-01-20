/* SKaMPI   MPI-Benchmark

Copyright 2003-2008 Werner Augustin
Lehrstuhl Informatik fuer Naturwissenschaftler und Ingenieure 
Fakultaet fuer Informatik
University of Karlsruhe

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <assert.h>

#include "misc.h"
#include "private_misc.h"
#include "debug.h"
#include "output.h"
#include "exec.h"

static struct variable *loopvar;

static bool running_autorefine = False;
static bool logarithmic;
static bool using_int_params;
static int multipleof;
static int registered_measurements;
static int regular_measurements;
static double iteration_multiplier = 2.0;
static bool new_iteration;
static bool autorefining;

struct value_list {
  int i;       /* i and f are alternatives excluding each other,  */
  double f;    /* but use of a union is too bureaucratic          */

  bool is_candidate;

  double result;
  double weight;

  struct value_list *next; /* next/prev sorted after value of varying */
  struct value_list *prev; /* argument (i/f) */

  struct value_list *next_prior; /* next/prev sorted after priority for */
  struct value_list *prev_prior; /* further measurement                 */
};

static struct value_list *results;  /* list of results, linked by next/prev */
static struct value_list *last_result;

static struct value_list *candidates; /* list of candidates, linked by  */
                                      /* next_prior / prev_prior        */


void set_autorefine_factor(double f)
{
  iteration_multiplier = f;
}


static struct value_list *malloc_struct_value_list(void)
{
  struct value_list *vl;
  vl = (struct value_list*)malloc(sizeof(struct value_list));
  assert( vl != NULL );
  vl->is_candidate = False;

  vl->i = -1; /* only to have initialized values for tools like valgrind */
  vl->f = -1.0;

  vl->next = NULL;
  vl->prev = NULL;
  vl->next_prior = NULL;
  vl->prev_prior = NULL;
  return vl;
}

/* function val2string is upto 5 times re-entrant, because it uses a constant
   buffer */

static char *val2string(struct value_list *v)
{
  static char string_buffer[5][20];
  static int buffer_idx = 0;

  buffer_idx = (buffer_idx+1) % 5;

  if( using_int_params ) 
    snprintf(string_buffer[buffer_idx], 20, "%d", v->i);
  else
    snprintf(string_buffer[buffer_idx], 20, "%f", v->f);
  return string_buffer[buffer_idx];
}

static void print_candidates_list(void)
{
  struct value_list *cand;
  int i;

  debug(DBG_AUTOREFINE, "print_candidates_list()\n");
  for( cand = candidates, i = 0; cand != NULL && i < 50; cand = cand->next_prior, i++ )
    debug(DBG_AUTOREFINE, "cand at %s with weight %f\n", val2string(cand), cand->weight);
}

static void print_measurement_result_list(void)
{
  struct value_list *vl;

  debug(DBG_AUTOREFINE, "print_measurement_result_list()\n");
  if( results != NULL ) {
    for( vl = results; vl != NULL; vl = vl->next ) 
      if( vl->is_candidate ) 
	debug(DBG_AUTOREFINE, "candidate[%s] with weight %f\n", val2string(vl), vl->weight);
      else 
	debug(DBG_AUTOREFINE, "result[%s] = %f\n", val2string(vl), vl->result);
  }
}

static void check_list(void)
{
  struct value_list *vl;

  if( results == NULL ) 
    debug(DBG_AUTOREFINE, "check_list: results == NULL, nothing to check\n");

  for( vl = results; vl != NULL; vl = vl->next ) {
    if( vl != results ) {
      if( vl->prev == NULL ) 
	debug(DBG_AUTOREFINE, "check_list: vl[%s] vl->prev == NULL\n", val2string(vl));
      else if( vl->prev->next == NULL )
	debug(DBG_AUTOREFINE, "check_list: vl[%s] vl->prev->next == NULL\n", val2string(vl));
      else if( vl->prev->next != vl )
	debug(DBG_AUTOREFINE, "check_list: vl[%s] vl->prev->next != vl\n", val2string(vl));
    }
    if( vl->next != NULL && vl->next->prev != vl )
      debug(DBG_AUTOREFINE, "check_list: vl[%s] vl->next->prev != vl\n", val2string(vl));
  }

  if( candidates == NULL ) 
    debug(DBG_AUTOREFINE, "check_list: candidates == NULL, nothing to check\n");

  for( vl = candidates; vl != NULL; vl = vl->next_prior ) {
    if( vl != candidates ) {
      if( vl->prev_prior == NULL ) 
	debug(DBG_AUTOREFINE, "check_list: vl[%s] vl->prev_prior == NULL\n", val2string(vl));
      else if( vl->prev_prior->next_prior == NULL )
	debug(DBG_AUTOREFINE, "check_list: vl[%s] vl->prev_prior->next_prior == NULL\n", val2string(vl));
      else if( vl->prev_prior->next_prior != vl )
	debug(DBG_AUTOREFINE, "check_list: vl[%s] vl->prev_prior->next_prior != vl\n", val2string(vl));
    }
    if( vl->next_prior != NULL && vl->next_prior->prev_prior != vl )
      debug(DBG_AUTOREFINE, "check_list: vl[%s] vl->next_prior->prev_prior != vl\n", val2string(vl));
  }
}

static void free_value_list(struct value_list *vl)
{
  struct value_list *next;

  debug(DBG_AUTOREFINE, "free_value_list()\n");

  DEBUG(DBG_CHECK, check_list(); );

  while( vl != NULL ) {
    next = vl->next;
    debug(DBG_AUTOREFINE, "free_value_list(vl[%s])\n", val2string(vl));
    free(vl);
    vl = next;
  }
}


void init_autorefine(struct statement *s, int m) 
{
  if( !lrootproc() ) return; /* at the moment there is no need for 
				init/finalize on non-root procs */

  assert( s->type == ST_ARITHM_VAR );

  debug(DBG_AUTOREFINE, "init_autorefine(variable='%s', multipleof=%d)\n",
	s->u.arithm_var.loopvar->name, m);

  if( running_autorefine )
    error_with_abort(15, "autorefine statements cannot be nested\n");

  running_autorefine = True;
  last_result = NULL;
  loopvar = s->u.arithm_var.loopvar;
  if( s->u.arithm_var.op == OP_ADD ) logarithmic = False;
  else                               logarithmic = True;

  if( loopvar->type == TYPE_INTEGER ) { using_int_params = True; loopvar->u.doublev = -1.0; }
  else                                { using_int_params = False; loopvar->u.intv = -1; }
  
  multipleof = m;
  registered_measurements = 0;
  new_iteration = True;
  autorefining = False;
  results = NULL;
  candidates = NULL; 
  
  debug(DBG_AUTOREFINE, "init_autorefine: logarithmic=%d using_int_params=%d multipleof=%d\n", 
	logarithmic, using_int_params, multipleof );

}

static int less_than(int a, int b, double f, double g)
{
  if( using_int_params ) 
    return a < b;
  else 
    return f < g;
}

static int equal(int a, int b, double f, double g)
{
  if( using_int_params ) 
    return a == b;
  else 
    return f == g;
}

static int greater_than(int a, int b, double f, double g)
{
  if( using_int_params ) 
    return a > b;
  else 
    return f > g;
}


static struct value_list *insert_element(int i, double f)
{
  struct value_list *el, *vl;

  if( using_int_params ) 
    debug(DBG_AUTOREFINE, "insert_element(%d)\n", i);
  else
    debug(DBG_AUTOREFINE, "insert_element(%f)\n", f);
    
  if( results == NULL ) {       
    el = malloc_struct_value_list();
    results = el;
    last_result = el;
    
  } else if( less_than(last_result->i, i, last_result->f, f) ) {
    for( vl = last_result; 
	 vl->next != NULL && less_than(vl->next->i, i, vl->next->f, f); 
	 vl = vl->next) ;
    if( vl->next == NULL ) {
      el = malloc_struct_value_list();
      vl->next = el;
      el->prev = vl;
    } else if( equal(vl->next->i, i, vl->next->f, f) ) {
      el = vl;
    } else {
      el = malloc_struct_value_list();
      el->prev = vl;
      el->next = vl->next;
      el->next->prev = el;
      el->prev->next = el;
    }

  } else if( equal(last_result->i, i, last_result->f, f) ) {
    el = last_result;

  } else {
    for( vl = last_result; 
	 vl->prev != NULL && greater_than(vl->prev->i, i, vl->prev->f, f); 
	 vl = vl->prev) ;
    if( vl->prev == NULL ) {
      el = malloc_struct_value_list();
      vl->prev = el;
      el->next = vl;
      results = el;
    } else if( equal(vl->prev->i, i, vl->prev->f, f) ) {
      el = vl;
    } else {
      el = malloc_struct_value_list();
      el->prev = vl->prev;
      el->next = vl;
      el->next->prev = el;
      el->prev->next = el;
    }
  }
  if( using_int_params )
    el->i = i;
  else 
    el->f = f;
  return el;
}

/* interpolates a double at value arg_inter using (arg1,res1) and (arg2,res2) */

static double linear_interpolate (double arg_inter, double arg1, double arg2, 
				  double res1, double res2)
{
  if (arg1 == arg2)
    return ((res1 + res2) / 2);

  return (res1 + (arg_inter - arg1) * 
	  (res2 - res1) / (arg2 - arg1));
}

/* at the moment both the integer and the double calculate key use the
   same base function, but one could think of a cleverer solution for
   the integer case? */

static double calculate_key(double x, 
			    double a, double ar, double b, double br,
			    double c, double cr, double d, double dr)
{
  double diff_left, diff_right;

  debug(DBG_AUTOREFINE, "calculate_key: a = (%f, %f), b = (%f, %f)\n", a, ar, b, br);
  debug(DBG_AUTOREFINE, "calculate_key: c = (%f, %f), d = (%f, %f)\n", c, cr, d, dr);

  diff_left  = fabs((linear_interpolate(b, c, d, cr, dr)-br)*(d-a));
  diff_right = fabs((linear_interpolate(c, a, b, ar, br)-cr)*(d-a));


  debug(DBG_AUTOREFINE, "diff_left  = %f\n", diff_left);
  debug(DBG_AUTOREFINE, "diff_right = %f\n", diff_right);

  return fmax2(diff_left, diff_right);
}

static double calculate_key_int(int x, 
				int a, double ar, int b, double br,
				int c, double cr, int d, double dr)
{
  return calculate_key(x,a,ar,b,br,c,cr,d,dr);
}


static double calculate_key_double(double x, 
				   double a, double ar, double b, double br,
				   double c, double cr, double d, double dr)
{
  return calculate_key(x,a,ar,b,br,c,cr,d,dr);
}


static struct value_list *prev_value(struct value_list *el)
{
  el = el->prev;
  while( el->is_candidate ) el = el->prev;
  return el;
}

static struct value_list *next_value(struct value_list *el)
{
  el = el->next;
  while( el->is_candidate ) el = el->next;
  return el;
}


static void remove_candidate_from_prior_queue(struct value_list *cand)
{
  debug(DBG_AUTOREFINE, "remove_candidate_from_prior_queue(%s)\n", val2string(cand));

  if( cand->prev_prior != NULL )
    cand->prev_prior->next_prior = cand->next_prior;
  else
    candidates = cand->next_prior;
  if( cand->next_prior != NULL )
    cand->next_prior->prev_prior = cand->prev_prior;

  cand->next_prior = NULL;
  cand->prev_prior = NULL;

  DEBUG(DBG_CHECK, check_list(); );
}

static void insert_candidate_in_priority_queue(struct value_list *cand)
{
  struct value_list *cl;
  double w;

  w = cand->weight;
  if( candidates == NULL ) {
    debug(DBG_AUTOREFINE, "insert_candidate_in_priority_queue: new candidate queue\n");
    candidates = cand;
    return;

  } else if( candidates->weight < w ) {
    debug(DBG_AUTOREFINE, "insert_candidate_in_priority_queue: inserting at top of candidate queue\n");
    cand->next_prior = candidates;
    candidates->prev_prior = cand;
    candidates = cand;

  } else if(candidates->next_prior == NULL && candidates->weight >= w ) {
    debug(DBG_AUTOREFINE, "insert_candidate_in_priority_queue: inserting after single element %s\n", 
	  val2string(candidates));
    candidates->next_prior = cand;
    cand->prev_prior = candidates;

  } else {
    for( cl = candidates; cl->next_prior != NULL && cl->weight >= w; cl = cl->next_prior ) ; 
    debug(DBG_AUTOREFINE, "insert_candidate_in_priority_queue: inserting after %s and before %s\n", 
	  val2string(cl->prev_prior), val2string(cl));
    cl->prev_prior->next_prior = cand;
    cand->prev_prior = cl->prev_prior;
    cand->next_prior = cl;
    cl->prev_prior = cand;
  }
  DEBUG(DBG_CHECK, check_list(); );
}

static void reposition_candidate_priority_queue(struct value_list *cand)
{
  /* @@ should be optimized */
  remove_candidate_from_prior_queue(cand);
  insert_candidate_in_priority_queue(cand);
}

static void calculate_weight(struct value_list *cand)
{
  struct value_list *left, *right;
  struct value_list *left_prev, *right_next;
  double w;

  debug(DBG_AUTOREFINE, "calculate_weight(cand=%s)\n", val2string(cand));
  
  left = cand->prev;
  right = cand->next;
  
  debug(DBG_AUTOREFINE, "calculate_weigth: left=%s, right=%s\n", val2string(left), val2string(right));

  left_prev = prev_value(left);
  right_next = next_value(right);

  if( using_int_params ) {
    cand->i = round_int_to_multipleof((left->i+right->i)/2, multipleof);
    /* @@ check for overflow and rounding in the wrong direction */
    w = calculate_key_int(cand->i, 
			  left_prev->i, left_prev->result,
			  left->i, left->result,
			  right->i, right->result,
			  right_next->i, right_next->result);
  } else {
    cand->f = (left->f + right->f)/2.0;
    w = calculate_key_double(cand->f, 
			  left_prev->f, left_prev->result,
			  left->f, left->result,
			  right->f, right->result,
			  right_next->f, right_next->result);
  }			     
  cand->weight = w;
  debug(DBG_AUTOREFINE, "calculate_weight: new weight is %f\n", w);
}

static void update_weight(struct value_list *cand)
{
  if( cand == NULL ) return;
  if( !cand->is_candidate ) return;

  debug(DBG_AUTOREFINE, "update_weight(cand=%s)\n", val2string(cand));
  calculate_weight(cand);
  reposition_candidate_priority_queue(cand);
}

static void insert_candidate(struct value_list *left, struct value_list *right)
{
  struct value_list *cand;
  int new_int_par = -1;          /* dummy init */
  double new_double_par = -1.0;  /* dummy init */

  if( using_int_params ) {
    new_int_par = round_int_to_multipleof((left->i+right->i)/2, multipleof);
    if( new_int_par == left->i || new_int_par == right->i ) {
      debug(DBG_AUTOREFINE, "no new candidate to insert\n"); 
      return;
    }
  } else 
    new_double_par = round_double_to_multipleof((left->f+right->f)/2, multipleof);

  debug(DBG_AUTOREFINE, "insert_candidate(left=%s, right=%s)\n", val2string(left), val2string(right));
  cand = malloc_struct_value_list();
  cand->is_candidate = True;
  cand->i = new_int_par;
  cand->f = new_double_par;

  /* insert between left and right */
  left->next = cand;
  right->prev = cand;
  cand->prev = left;
  cand->next = right;

  calculate_weight(cand);
  insert_candidate_in_priority_queue(cand);
}

static void init_candidates(void)
{
  struct value_list *guard, *vl;

  debug(DBG_AUTOREFINE, "starting init_candidates()\n");

  assert( results != NULL ); /* @@ possibly error message? */

  guard = malloc_struct_value_list(); /* insert guard element as first element */
  guard->result = results->result;    /* @@ linear interpolate guard->result? */
  if( using_int_params )  guard->i = results->i;
  else                    guard->f = results->f;
  guard->next = results;
  results->prev = guard;

  for( vl = results->next; vl->next != NULL; vl = vl->next ) {
    insert_candidate(vl->prev, vl);
    /* print_candidates_list(); */
  }

  debug(DBG_AUTOREFINE, "inserting guard after %s\n", val2string(vl));

  guard = malloc_struct_value_list(); /* insert guard element as last element */
  guard->result = vl->result;         /* @@ linear interpolate guard->result? */
  if( using_int_params )  guard->i = vl->i;
  else                    guard->f = vl->f;
  vl->next = guard;
  guard->prev = vl;

  insert_candidate(vl->prev, vl);

  DEBUG(DBG_AUTOREFINE, print_candidates_list(); );
  DEBUG(DBG_AUTOREFINE, print_measurement_result_list(); );

  DEBUG(DBG_CHECK, check_list(); );
}


void register_measurement_result(double result)
{
  struct value_list *el;
  
  if( ! running_autorefine ) return;

  if( using_int_params ) 
    debug(DBG_AUTOREFINE, "register_measurement_result(%s=%d, result=%f)\n", 
	  loopvar->name, loopvar->u.intv, result);
  else
    debug(DBG_AUTOREFINE, "register_measurement_result(%s=%f, result=%f)\n", 
	  loopvar->name, loopvar->u.doublev, result);

  if( autorefining ) {
    DEBUG(DBG_CHECK, check_list(); );
    el = candidates;
    /* candidates = el->next_prior; */
    remove_candidate_from_prior_queue(el);
    el->is_candidate = False;
    el->result = result;
    
    debug(DBG_AUTOREFINE, "inside register_measurement_result\n");
    /* print_measurement_result_list(); */
    DEBUG(DBG_CHECK, check_list(); );

    update_weight(el->prev->prev);
    update_weight(el->next->next);
    insert_candidate(el->prev, el);
    insert_candidate(el, el->next);

  } else {
    el = insert_element(loopvar->u.intv, loopvar->u.doublev);
    el->result = result;
  }
  registered_measurements++;
  DEBUG(DBG_CHECK, check_list(); );
  debug(DBG_AUTOREFINE, "register_measurement_results: measurement_list\n");
  DEBUG(DBG_AUTOREFINE, print_measurement_result_list(); );
  debug(DBG_AUTOREFINE, "\n");

  debug(DBG_AUTOREFINE, "register_measurement_results: candidate_list\n");
  DEBUG(DBG_AUTOREFINE, print_candidates_list(); );
  debug(DBG_AUTOREFINE, "\n");
}


bool set_new_loop_values(void)
{
  int continue_loop;

  if( lrootproc() ) {
    if( !autorefining ) {
      regular_measurements = registered_measurements;
      debug(DBG_AUTOREFINE, "set_new_loop_values: starting autorefining with regular_measurements=%d\n",
	    regular_measurements);
      init_candidates();
      autorefining = True;
    }

    continue_loop = registered_measurements < regular_measurements*iteration_multiplier;

    if( candidates != NULL ) {
      debug(DBG_AUTOREFINE, "set_new_loop_values: top candidate is %s with weight %f\n", 
	      val2string(candidates), candidates->weight);
      if( using_int_params ) loopvar->u.intv    = candidates->i;
      else     	             loopvar->u.doublev = candidates->f;

    } else
      continue_loop = False;
  }
  
  MPI_Bcast(&continue_loop, 1, MPI_INT, 0, get_measurement_comm()); /* @@ optimize! */
  if( continue_loop ) {
    if( using_int_params )
      MPI_Bcast(&loopvar->u.intv, 1, MPI_INT, 0, get_measurement_comm());
    else
      MPI_Bcast(&loopvar->u.doublev, 1, MPI_DOUBLE, 0, get_measurement_comm());
  }

  return continue_loop;
}


void starting_new_iteration(void) 
{
  new_iteration = True;
}


void finalize_autorefine(void) 
{
  if( !lrootproc() ) return; /* at the moment there is no need for 
				init/finalize on non-root procs */

  debug(DBG_AUTOREFINE, "finalize_autorefine()\n");
  DEBUG(DBG_AUTOREFINE, print_measurement_result_list(); );
  running_autorefine = False;
  free_value_list(results);
}
