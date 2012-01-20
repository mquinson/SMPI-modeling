/* SKaMPI   MPI-Benchmark

Copyright 2003-2008 Werner Augustin, Lars Diesselberg
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <mpi.h>

#include "mpiversiontest.h"

#include "misc.h"
#include "private_misc.h"
#include "debug.h"
#include "output.h"
#include "exec.h"
#include "output_sort.h"
#include "print.h"
#include "measurement.h"
#include "autorefine.h"


/* @@@ function shouldn't be needed !!! @@@@ */
enum var_type get_result_type2(enum op_type op, enum var_type left, 
			       enum var_type right)
{
  if( left == TYPE_INTEGER && right == TYPE_INTEGER ) return TYPE_INTEGER;
  else return TYPE_DOUBLE;
 /* not complete @@ */
}


static bool needs_reference_counter(struct variable *v)
{
  if( v == NULL ) return False;
  switch( v->type ) {
  case TYPE_INTEGER :
  case TYPE_DOUBLE  :
  case TYPE_STRING  :      /* @@@ */
  case TYPE_VOID    : return False; 
  case TYPE_DATATYPE:
  case TYPE_COMM    :
  case TYPE_OP      :
  case TYPE_IARRAY  :
  case TYPE_DARRAY  :
  case TYPE_INFO    : return True; 
  default:
    error_without_abort("out of cases in exec.c__needs_reference_counter() v->type = %d\n", v->type);
    return False;
  }
}

void increase_reference(struct variable *v) 
{
  /*int i;*/
  
  assert( v != NULL );

  if( needs_reference_counter(v) ) {

    if( v->name == NULL ) {
      if( v->ref_counter == NULL )
	debug(DBG_EXEC, "increase_reference(unnamed variable) NULL -> 1\n");
      else 
	debug(DBG_EXEC, "increase_reference(unnamed variable) %d -> %d\n",
	      *(v->ref_counter), (*(v->ref_counter))+1);
    } else {
      if( v->ref_counter == NULL )
	debug(DBG_EXEC, "increase_reference(%s) NULL -> 1\n", v->name);
      else 
	debug(DBG_EXEC, "increase_reference(%s) %d -> %d\n", v->name,
	      *(v->ref_counter), (*(v->ref_counter))+1);
    }

    if( v->ref_counter == NULL ) {
      v->ref_counter = skampi_malloc_ints(1);
      *(v->ref_counter) = 1;
    } else {
      (*(v->ref_counter))++;
    }
    
    /*if (v->type == TYPE_DARRAY) {
      if( v->ref_counters == NULL ) {
        v->ref_counters = malloc(v->u.darrv.n * sizeof(int*));
        for (i=0; i < v->u.darrv.n; i++) {
          v->ref_counters[i] = malloc_ints(1);
          *(v->ref_counters[i]) = 1;
        }
      } else {
        for (i=0; i < v->u.darrv.n; i++)
          (*(v->ref_counters[i]))++;
      }
    }*/
    
  }
}

void decrease_reference(struct variable *v) 
{
  /*int i;*/
  
  assert( v != NULL );
  /* 
     An error is thrown for MPI_DATATYPE_NULL, MPI_OP_NULL and
     MPI_INFO_NULL because there seams to be no constructor function
     wich produces these objects. MPI_COMM_NULL can be produced with 
     MPI_Comm_split and color=MPI_UNDEFINED 
  */
  if( needs_reference_counter(v) ) {
    if( v->name == NULL ) {
      if( v->ref_counter == NULL )
	debug(DBG_EXEC, "decrease_reference(unnamed variable) NULL -> NULL\n");
      else 
	debug(DBG_EXEC, "decrease_reference(unnamed variable) %d -> %d\n",
	      *(v->ref_counter), (*(v->ref_counter))-1);
    } else {
      if( v->ref_counter == NULL )
	debug(DBG_EXEC, "decrease_reference(%s) NULL -> NULL\n", v->name);
      else 
	debug(DBG_EXEC, "decrease_reference(%s) %d -> %d\n", v->name,
	      *(v->ref_counter), (*(v->ref_counter))-1);
    }

    if( v->ref_counter == NULL ) return;

    (*(v->ref_counter))--;
    if( *(v->ref_counter) == 0 ) {
      if( v->name == NULL )
	debug(DBG_EXEC, "decrease_reference: freeing unnamed variable\n");
      else
	debug(DBG_EXEC, "decrease_reference: freeing '%s'\n", v->name);
      switch( v->type ) {
      case TYPE_DATATYPE: 
	if( v->u.datatypev == MPI_DATATYPE_NULL )
	  error_without_abort("trying to free an MPI_Datatype object with value MPI_DATATYPE_NULL\n");
	else
	  MPI_Type_free(&(v->u.datatypev)); 
	break;
      case TYPE_COMM    : 
	if( v->u.commv != MPI_COMM_NULL ) MPI_Comm_free(&(v->u.commv)); 
	break;
      case TYPE_IARRAY  : 
	if( v->u.iarrv.v != NULL) free(v->u.iarrv.v);
        v->u.iarrv.v = NULL;
        v->u.iarrv.n = 0;
	break;
      case TYPE_DARRAY  :
        /* decrease reference counters of array elements */
	/* for (i = 0; i < v->u.darrv.n; i++) {
          if (v->ref_counters[i] == NULL) continue;
          (*(v->ref_counters[i]))--;
          if (*(v->ref_counters[i]) == 0) {
            if (v->u.darrv.v[i] == MPI_DATATYPE_NULL)
              error_without_abort("trying to free an MPI_Datatype object with value MPI_DATATYPE_NULL\n");
            else
              MPI_Type_free(&(v->u.darrv.v[i])); 
            free(v->ref_counters[i]);
          }
        }*/
	if (v->u.darrv.v != NULL) free(v->u.darrv.v);
        v->u.darrv.v = NULL;
        v->u.darrv.n = 0;
        /*if (v->ref_counters != NULL) free(v->ref_counters);
        v->ref_counters = NULL;*/
        break;
      case TYPE_OP      : 
	if( v->u.opv == MPI_OP_NULL )
	  error_without_abort("trying to free an MPI_Op object with value MPI_OP_NULL\n");
	else
	  MPI_Op_free(&(v->u.opv)); 
	break;
#ifdef USE_MPI_INFO
      case TYPE_INFO    : 
	if( v->u.infov == MPI_INFO_NULL )
	  error_without_abort("trying to free an MPI_Info object with value MPI_INFO_NULL\n");
	else
	  MPI_Info_free(&(v->u.infov)); 
	break;
#endif
      default:
	error_without_abort("out of cases in exec.c__decrease_reference() v->type = %d\n", v->type);
      }
      free(v->ref_counter);
      v->ref_counter = NULL;
    }
    v->ref_counter = NULL;
  }
}

void init_struct_variable(struct variable *v, 
			  enum var_type type, char *name, char *string_repr)
{
  v->type = type;
  v->name = name;
  v->string_repr = string_repr;
  v->ref_counter = NULL;
  /*v->ref_counters = NULL;*/
}

struct variable *create_struct_variable(enum var_type type, char *name, 
					char *string_repr)
{
  struct variable *v;
  v = (struct variable*)malloc(sizeof(struct variable));
  assert( v != NULL);
  init_struct_variable(v, type, name, string_repr);
  return v;
}

void free_variable(struct variable *v)
{
  assert( v != NULL);
  if( v->name == NULL ) 
    debug(DBG_EXEC || DBG_SYMBOLT, "free_variable(unnamed variable)\n");
  else
    debug(DBG_EXEC || DBG_SYMBOLT, "free_variable('%s')\n", v->name);

  /*  if( v->name != NULL ) free(v->name); */        
  /* @@ reasonable ?! no!! name can be used more than once! */
  if( v->string_repr != NULL ) free(v->string_repr); /* @@ reasonable ?! */

  decrease_reference(v);
}

void free_term(struct term *t) 

{
  struct term *tmp;
  
  assert( t != NULL );
  for( ; t != NULL; t = t->next ) {
    switch( t->type ) {
    case TERM_VALUE:
      free_variable(t->var);
      free(t);
      break;
    case TERM_UNARY:
      free_term(t->left);
      free(t);
      break;
    case TERM_BINARY:
      free_term(t->right);
      free_term(t->left);
      free(t);
      break;
    case TERM_ARRAY:
      tmp = t->left;
      free(t);
      t = tmp;
      while (t != NULL) {
        tmp = t->next;
        free_term(t);
        t = tmp;
      }
      break;
    default:
      error_without_abort("out of cases in exec.c__free_term()\n");
    }
  }
}



double epsilon = 0.000001; /* @@@@@ */

static int cmp_var(struct variable *v1, struct variable *v2)
{
  int i1, i2;
  double f1, f2;

  if( v1->type == TYPE_INTEGER && v2->type == TYPE_INTEGER ) {
    i1 = v1->u.intv;
    i2 = v2->u.intv;
    if( i1 == i2 ) return 0;
    if( i1 < i2)   return -1;
    else           return 1;
  } else {
    if( v1->type == TYPE_DOUBLE ) f1 = v1->u.doublev;
    else                          f1 = (double)(v1->u.intv);
    if( v2->type == TYPE_DOUBLE ) f2 = v2->u.doublev;
    else                          f2 = (double)(v2->u.intv);
    if( fabs(f1-f2) < epsilon) return 0; /* @@@ */
    if( f1 < f2 )              return -1;
    else                       return 1;
  }
}

static int sign_var(struct variable *v)
{
  if( v->type == TYPE_INTEGER ) {
    if( v->u.intv > 0 )       return 1;
    else if( v->u.intv == 0 ) return 0;
    else                      return -1;
  } else {
    if( v->u.doublev > 0 )       return 1;
    else if( v->u.doublev == 0 ) return 0;
    else                         return -1;
  }
}

void store_int(struct variable *result, int i)
{
  result->type = TYPE_INTEGER;
  result->u.intv = i;
}

void store_double(struct variable *result, double f)
{
  result->type = TYPE_DOUBLE;
  result->u.doublev = f;
}

void store_string(struct variable *result, char *s)
{
  result->type = TYPE_STRING;
  result->u.stringv = s;
}

void store_datatype(struct variable *result, MPI_Datatype datatype)
{
  result->type = TYPE_DATATYPE;
  result->u.datatypev = datatype;
}

void store_comm(struct variable *result, MPI_Comm comm)
{
  result->type = TYPE_COMM;
  result->u.commv = comm;
}

void store_operator(struct variable *result, MPI_Op op)
{
  result->type = TYPE_OP;
  result->u.opv = op;
}

void store_iarray(struct variable* result, IArray iarr)
{
  result->type = TYPE_IARRAY;
  result->u.iarrv = iarr;
}

void store_darray(struct variable* result, DArray darr)
{
  result->type = TYPE_DARRAY;
  result->u.darrv = darr;
}

#ifdef USE_MPI_INFO
void store_info(struct variable *result, MPI_Info info)
{
  result->type = TYPE_INFO;
  result->u.infov = info;
}
#endif

static int greater_than_one(struct variable *v)
{
  if( v->type == TYPE_INTEGER) return 1;
  if( v->u.doublev > 1.0 ) return 1;
  return 0;
}

static int equal_zero(struct variable *v)
{
  assert( v->type == TYPE_INTEGER );
  if( v->u.intv == 0 ) return 1;
  else                 return 0;
}

static void negate_variable(struct variable *v) 
{
  if( v-> type == TYPE_INTEGER )
    v->u.intv = - v->u.intv;
  else
    v->u.doublev = - v->u.doublev;
}

static void cast_variable(enum var_type type, struct variable *v)
{
  double temp_double;
  int    temp_int;

  if( type == v->type ) return;

  DEBUG(DBG_EXEC,
	fprintf(stderr, "#proc %d: cast_variable(", get_my_global_rank());
	fprint_var_type(stderr, type);
	fprintf(stderr, ", ");
	fprint_variable_value(stderr, v);
	fprintf(stderr, ") -> ");
	);
	
  if( type == TYPE_DOUBLE && v->type == TYPE_INTEGER) { /* not complete @@@ */
    temp_double = (double)(v->u.intv);
    v->type = TYPE_DOUBLE;
    v->u.doublev = temp_double;
    DEBUG(DBG_EXEC,
	  fprint_variable_value(stderr, v);
	  fprintf(stderr, "\n");
	  );
    return;
  }
  if( type == TYPE_INTEGER && v->type == TYPE_DOUBLE) {
    temp_int = (int)(v->u.doublev+0.5); /* round ?! @@@ */
    v->type = TYPE_INTEGER;
    v->u.intv = temp_int;
    DEBUG(DBG_EXEC,
	  fprint_variable_value(stderr, v);
	  fprintf(stderr, "\n");
	  );
    return;
  }
  error_with_abort(13, "can't cast variable of type %s to parameter of type %s\n", var_type_to_string(v->type), var_type_to_string(type)); 
}

void cast_variable_to_multipleof(struct variable *multipleof, struct variable *v)
{
  assert( multipleof->type == TYPE_INTEGER );
  assert( v->type == TYPE_INTEGER || v->type == TYPE_DOUBLE);
  
  if( v->type == TYPE_INTEGER ) 
    v->u.intv = round_int_to_multipleof(v->u.intv, multipleof->u.intv);
  else {
    v->type = TYPE_INTEGER;
    v->u.intv = round_double_to_multipleof(v->u.doublev, multipleof->u.intv);
  }
}

void evaluate_params(char *interface, struct term *source_params,
		     struct variable *dest_params)
{
  int i;
  struct term *param_term;

  for( i = 0, param_term = source_params; param_term != NULL; 
       param_term = param_term->next, i++ ) {
    assert( i < MAX_NUMBER_PARAMS );
    init_struct_variable(&(dest_params[i]), TYPE_VOID, NULL, NULL);
    evaluate_term(&(dest_params[i]), param_term);
    cast_variable((enum var_type)(interface[i]), &(dest_params[i]));
  }
}

static void free_params(struct term *source_params, struct variable *dest_params)
{
  int i;
  struct term *param_term;

  for( i = 0, param_term = source_params; param_term != NULL; 
       param_term = param_term->next, i++ )
    decrease_reference(&(dest_params[i]));
}

void evaluate_term(struct variable *result, struct term *t)
{
  int i, n;
  struct variable left, right;
  struct variable params[MAX_NUMBER_PARAMS];
  struct term *tmp;
  char *new_str_rep;

  init_struct_variable(&left, TYPE_VOID, NULL, NULL);
  init_struct_variable(&right, TYPE_VOID, NULL, NULL);

  assert( t != NULL );
  DEBUG(DBG_EXEC,
	fprintf(stderr, "#proc %d: evaluate_term(..., ", 
		get_my_global_rank());
	fprint_term(stderr, t);
	fprintf(stderr, ")\n");
	);

  switch( t->type ) {
  case TERM_VALUE:
    decrease_reference(result);
    if( result->string_repr != NULL ) free(result->string_repr);
    if( t->var->string_repr != NULL ) 
      new_str_rep = strdup(t->var->string_repr);
    else
      new_str_rep = NULL;
    init_struct_variable(result, t->var->type, result->name, new_str_rep);
    result->u    = t->var->u;
    result->ref_counter = t->var->ref_counter;
    increase_reference(result);
    break;
  case TERM_UNARY:
    evaluate_term(&left, t->left);
    result->type = left.type;
    if( t->op == OP_SUB ) negate_variable(&left);
    if( t->op == OP_NOT ) {
      assert( left.type == TYPE_INTEGER );
      left.u.intv = ! left.u.intv;
    }
    if( t->op == OP_BNOT ) {
      assert( left.type == TYPE_INTEGER );
      left.u.intv = ~ left.u.intv;
    }
    result->string_repr = NULL;
    result->u    = left.u;
    break;
  case TERM_BINARY:
    result->string_repr = NULL;
    evaluate_term(&left, t->left);
    evaluate_term(&right, t->right);
    result->type = get_result_type2(t->op, left.type, right.type);
    cast_variable(result->type, &left);
    cast_variable(result->type, &right);
    switch( result->type ) {
    case TYPE_INTEGER:
      switch( t->op ) {
      case OP_ADD:  result->u.intv = left.u.intv + right.u.intv; break;
      case OP_SUB:  result->u.intv = left.u.intv - right.u.intv; break;
      case OP_MULT: result->u.intv = left.u.intv * right.u.intv; break;
      case OP_DIV:  result->u.intv = left.u.intv / right.u.intv; break; /* @@@ */
      case OP_MOD:  result->u.intv = left.u.intv % right.u.intv; break; /* @@@ */
      case OP_LT : result->u.intv = left.u.intv <  right.u.intv; break;     
      case OP_LE : result->u.intv = left.u.intv <= right.u.intv; break;
      case OP_GT : result->u.intv = left.u.intv >  right.u.intv; break;
      case OP_GE : result->u.intv = left.u.intv >= right.u.intv; break;
      case OP_EQ : result->u.intv = left.u.intv == right.u.intv; break;
      case OP_NEQ: result->u.intv = left.u.intv != right.u.intv; break;
      case OP_AND: result->u.intv = left.u.intv && right.u.intv; break;
      case OP_BAND: result->u.intv = left.u.intv & right.u.intv; break;
      case OP_OR : result->u.intv = left.u.intv || right.u.intv; break;
      case OP_BOR: result->u.intv = left.u.intv | right.u.intv; break;
      default: 
	error_without_abort("out of cases in exec.c__evaluate_term()_2\n");
      }
      break;
    case TYPE_DOUBLE:
      switch( t->op ) {
      case OP_ADD:  result->u.doublev = left.u.doublev+right.u.doublev; break;
      case OP_SUB:  result->u.doublev = left.u.doublev-right.u.doublev; break;
      case OP_MULT: result->u.doublev = left.u.doublev*right.u.doublev; break;
      case OP_DIV:  result->u.doublev = left.u.doublev/right.u.doublev; break; 
	                                                /* check for NAN ?! */
      default:  
	error_without_abort("out of cases in exec.c__evaluate_term()_3\n");
      }
      break;
    default:
	error_without_abort("out of cases in exec.c__evaluate_term()_4\n");
    }
    break;
  case TERM_FUNC: 
    evaluate_params(t->interface, t->params, params);
    if( result != NULL ) {
      decrease_reference(result);
      init_struct_variable(result, TYPE_VOID, result->name, NULL);
    }
    t->call_fp(result, params);
    free_params(t->params, params);
    if( result != NULL ) increase_reference(result);
    break;
  case TERM_ARRAY:
    result->string_repr = NULL;
    if (t->left == NULL) { /* no array elements, default to integer type */
      result->type = TYPE_IARRAY;
      result->u.iarrv.v = malloc(0);
      result->u.iarrv.n = 0;
      break;
    }
    
    /* count terms in array */
    n = 0;
    tmp = t->left;
    while (tmp != NULL) {
      n++;
      tmp = tmp->next;
    }
    
    /* evaluate first term to determine array type */
    tmp = t->left;
    evaluate_term(&left, tmp);
    switch (left.type) {
    case TYPE_INTEGER:
      result->type = TYPE_IARRAY;
      result->u.iarrv.n = n;
      result->u.iarrv.v = malloc(sizeof(int) * n);
      result->u.iarrv.v[0] = left.u.intv;
      tmp = tmp->next;
      for (i = 1; i < n; i++) {
        evaluate_term(&left, tmp);
        if (left.type != TYPE_INTEGER)
          error_without_abort("exec.c__evaluate_term(): non-integer found in integer array\n");
        else
          result->u.iarrv.v[i] = left.u.intv;
        tmp = tmp->next;
      }
      break;
    case TYPE_DATATYPE:
      result->type = TYPE_DARRAY;
      result->u.darrv.n = n;
      result->u.darrv.v = malloc(sizeof(MPI_Datatype) * n);
      /*result->ref_counters = malloc(n * sizeof(int*));*/
      result->u.darrv.v[0] = left.u.datatypev;
      /*result->ref_counters[0] = left.ref_counter;*/
      increase_reference(&left);
      tmp = tmp->next;
      for (i = 1; i < n; i++) {
        evaluate_term(&left, tmp);
        if (left.type != TYPE_DATATYPE)
          error_without_abort("exec.c__evaluate_term(): non-datatype found in datatype array\n");
        else {
          result->u.darrv.v[i] = left.u.datatypev;
          /*result->ref_counters[i] = left.ref_counter;*/
          increase_reference(&left);
        }
        tmp = tmp->next;
      }
      break;
    default:
      error_without_abort("out of cases in exec.c__evaluate_term() t->type=%d\n", 
			t->type);
    }
    break;
    
  default:
    error_without_abort("out of cases in exec.c__evaluate_term() t->type=%d\n", 
			t->type);
  }
  DEBUG(DBG_EXEC, 
	fprintf(stderr, "#proc %d: evaluate_term(..., ", 
		get_my_global_rank());
	fprint_term(stderr, t);
	if( result != NULL ) {
	  fprintf(stderr, ") -> ");
	  fprint_variable_value(stderr, result);
	  fprintf(stderr, "\n");
	} else
 	  fprintf(stderr, ")\n");
	);

}


struct variable* loopvar_stack[10]; /* @@@@ */
int loopvar_stack_sp = 0;


static void push_variable(struct variable *v)
{
  loopvar_stack[loopvar_stack_sp++] = v;
}

static void pop_variable(void)
{
  loopvar_stack_sp--;
}

static int abort_inner_loop;

void execute_statement(struct statement *s)
{
  struct variable tmp;
  struct term *t;
  int error_code, local_abort_inner_loop;

  while( s != NULL ) {
    DEBUG(DBG_EXEC,
	  fprintf(stderr, "#proc %d: execute_statement(",
		  get_my_global_rank());
	  fprint_statement(stderr, s);
	  fprintf(stderr, ")\n");
	  );
    
    switch( s->type ) {
    case ST_ASSIGNMENT: {
      evaluate_term(s->u.assignment.lval, s->u.assignment.rval);
    } break;
    case ST_PRINT:
      init_struct_variable(&tmp, TYPE_VOID, NULL, NULL);
      print_output("#P[%d] ", get_my_global_rank());
      t = s->u.print.val;
      while (t != NULL) {
        evaluate_term(&tmp, t);
        print_output_variable_value(&tmp);
        t = t->next;
      }
      print_output("\n");
      if (s->u.print.flush) /* collective flush */
        flush_output();
      decrease_reference(&tmp);
      break;
    case ST_FLUSH:
       flush_output();
       break;
    case ST_MEASURE: 
      error_code = execute_measurement(s);
      local_abort_inner_loop = error_code & (MEAS_BUFFER_TOO_SMALL | MEAS_OUT_OF_BOUNDS);
      MPI_Allreduce(&local_abort_inner_loop, &abort_inner_loop, 
		    1, MPI_INT, MPI_BOR, MPI_COMM_WORLD);
      break;
    case ST_ARITHM_VAR: {
      struct variable first, last, step, multipleof;
      struct term loopterm;
      struct term loopvarterm;
      struct variable *loopvar;         /* user visible loop variable,
                                           keeps type of first initialisation */
      struct variable last_loopvar;     /* last value of user visible loop variable,
					   skip loop body if this value has not 
					   changed */
      struct variable internal_loopvar; /* internal (exact) loop variable */
      int increasing;
      
      init_struct_variable(&last_loopvar, TYPE_VOID, NULL, NULL);
      init_struct_variable(&first, TYPE_VOID, NULL, NULL);
      evaluate_term(&first, s->u.arithm_var.first);
      init_struct_variable(&last, TYPE_VOID, NULL, NULL);
      if( s->u.arithm_var.last ) 
	evaluate_term(&last, s->u.arithm_var.last);
      init_struct_variable(&step, TYPE_VOID, NULL, NULL);
      evaluate_term(&step, s->u.arithm_var.step);
      init_struct_variable(&multipleof, TYPE_VOID, NULL, NULL);
      if( s->u.arithm_var.multipleof ) {
	evaluate_term(&multipleof, s->u.arithm_var.multipleof);
	if( multipleof.type != TYPE_INTEGER || multipleof.u.intv < 0)
	  error_with_abort(14, "paramater of keyword multipleof has to be a positive integer\n");
      }

      loopvar = s->u.arithm_var.loopvar;
      push_variable(loopvar);
      evaluate_term(loopvar, s->u.arithm_var.first); /* evaluate a second time ??!! @@@@ */

      init_struct_variable(&internal_loopvar, first.type, NULL, NULL);
      internal_loopvar.u = loopvar->u;
      loopvarterm.type = TERM_VALUE;
      loopvarterm.var = &internal_loopvar;

      loopterm.type = TERM_BINARY;
      loopterm.op = s->u.arithm_var.op;
      loopterm.left = &loopvarterm;
      loopterm.right = s->u.arithm_var.step;

      if( loopterm.op == OP_ADD ) {
	if( sign_var(&step) > 0 ) increasing = 1;
	else                      increasing = 0;
      } else {
	if( ((sign_var(&first) > 0 ) && greater_than_one(&step)) ||
	    ((sign_var(&first) < 0 ) && !greater_than_one(&step)) )   
	  increasing = 1;
	else                                                          
	  increasing = 0;
      }

      if( s->u.arithm_var.multipleof )
	cast_variable_to_multipleof(&multipleof, loopvar);

      if( s->u.arithm_var.refine ) {
	register_output_for_sorting(s);
	if( s->u.arithm_var.multipleof )
	  init_autorefine(s, multipleof.u.intv);
	else
	  init_autorefine(s, 1);
      }
      
      if( s->u.arithm_var.multipleof )
	cast_variable_to_multipleof(&multipleof, loopvar);

      abort_inner_loop = False;
      if( increasing ) {
	while( !abort_inner_loop && 
	       (last.type == TYPE_VOID || cmp_var(loopvar, &last) <= 0 )) {
	  if( last_loopvar.type == TYPE_VOID ||
	      cmp_var(&last_loopvar, loopvar) != 0 ) {
	    starting_new_iteration();
	    execute_statement(s->u.arithm_var.body);
	  }
	  last_loopvar = *loopvar;
	  evaluate_term(&internal_loopvar, &loopterm);
	  evaluate_term(loopvar, &loopvarterm);
	  if( s->u.arithm_var.multipleof )
	    cast_variable_to_multipleof(&multipleof, loopvar);
	  else
	    cast_variable(first.type, loopvar);
	}
      } else {	
	while( !abort_inner_loop && 
	       (last.type == TYPE_VOID || cmp_var(loopvar, &last) >= 0 )) {
	  if( last_loopvar.type == TYPE_VOID ||
	      cmp_var(&last_loopvar, loopvar) != 0 ) {
	    starting_new_iteration();
	    execute_statement(s->u.arithm_var.body);
	  }
	  last_loopvar = *loopvar;
	  evaluate_term(&internal_loopvar, &loopterm);
	  evaluate_term(loopvar, &loopvarterm);
	  if( s->u.arithm_var.multipleof )
	    cast_variable_to_multipleof(&multipleof, loopvar);
	  else
	    cast_variable(first.type, loopvar);
	}
      }
      if( s->u.arithm_var.refine ) {
	while( set_new_loop_values() ) {
	  starting_new_iteration();
	  execute_statement(s->u.arithm_var.body);
	}
	finalize_autorefine();
      }
      sort_and_print_output();

      decrease_reference(&first);
      decrease_reference(&last);
      decrease_reference(&internal_loopvar);
      abort_inner_loop = False;
      pop_variable();
    } break;
    case ST_LIST_VAR: {
      struct term *loopterm;
      struct variable *loopvar;
      
      loopvar = s->u.list_var.loopvar;
      push_variable(loopvar);
      abort_inner_loop = False;
      for( loopterm = s->u.list_var.term_list; !abort_inner_loop && loopterm != NULL; 
	   loopterm = loopterm->next ) {
	evaluate_term(loopvar, loopterm);
	execute_statement(s->u.list_var.body);
      }
      abort_inner_loop = False;
      pop_variable();
    } break;
    case ST_ITERATOR_VAR: {
      struct variable *loopvar;
      struct variable params[MAX_NUMBER_PARAMS];

      loopvar = s->u.iterator_var.loopvar;
      push_variable(loopvar);


      s->u.iterator_var.self = NULL;
      abort_inner_loop = False;
      do {
	evaluate_params(s->u.iterator_var.interface,
			s->u.iterator_var.par_list, params);
	s->u.iterator_var.iterator_fp(loopvar, params, 
				      &(s->u.iterator_var.self));
	free_params(s->u.iterator_var.par_list, params);
	if( s->u.iterator_var.self == NULL ) break;
	increase_reference(loopvar);
	execute_statement(s->u.iterator_var.body);
      }  while( !abort_inner_loop);
      abort_inner_loop = False;
      pop_variable();
    } break;
    case ST_IF_THEN: {
      struct variable condition;
      
      init_struct_variable(&condition, TYPE_VOID, NULL, NULL);
      evaluate_term(&condition, s->u.if_then.condition);
      if( !equal_zero(&condition) ) {
	decrease_reference(&condition);
	execute_statement(s->u.if_then.then_body);
      } else if( s->u.if_then.else_body != NULL ) {
	decrease_reference(&condition);
	execute_statement(s->u.if_then.else_body);
      }
    } break;
    default:
      error_without_abort("out of cases in exec.__execute_statement()\n");
    }
    
    s = s->next;
  }
}

