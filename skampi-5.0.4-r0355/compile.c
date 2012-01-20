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
#include <assert.h>
#include <math.h>
#include <mpi.h>

#include "misc.h"
#include "private_misc.h"
#include "debug.h"
#include "exec.h"
#include "symboltable.h"
#include "print.h"
#include "compile.h"

#if 0
/* currently the following is  not used everywhere
   see also get_result_type2 in exec.c */
static enum var_type get_result_type(enum op_type op, enum var_type left, 
			      enum var_type right)
{
  if( left == TYPE_INTEGER && right == TYPE_INTEGER ) return TYPE_INTEGER;
  else return TYPE_DOUBLE;
 /* not complete @@ */
}
#endif

int types_compatible(enum var_type dest, enum var_type source)
{
  if( dest == source ) return 1;
  if( dest == TYPE_INTEGER && source == TYPE_DOUBLE ) return 1;
  if( dest == TYPE_DOUBLE && source == TYPE_INTEGER ) return 1;
  return 0;
} 


struct term *malloc_struct_term(void)
{
  struct term *t;
  t = (struct term*)malloc(sizeof(struct term));
  assert( t != NULL );
  return t;
}

struct statement *malloc_struct_statement(void)
{
  struct statement *s;
  s = (struct statement*)malloc(sizeof(struct statement));
  assert( s != NULL );
  return s;
}

struct term *create_int_term(int i)
{
  struct variable *v;
  struct term *t;

  v = create_struct_variable(TYPE_INTEGER, NULL, NULL);
  v->u.intv = i;

  t = malloc_struct_term();
  t->type = TERM_VALUE;
  t->var = v;
  t->next = NULL;

  return t;
}

struct term *create_array_term(struct term* arrt)
{
  struct term *t;

  t = malloc_struct_term();
  t->type = TERM_ARRAY;
  t->left = arrt;
  t->next = NULL;
  return t;
}

struct term *create_double_term(double f)
{
  struct variable *v;
  struct term *t;

  v = create_struct_variable(TYPE_DOUBLE, NULL, NULL);
  v->u.doublev = f;

  t = malloc_struct_term();
  t->type = TERM_VALUE;
  t->var = v;
  t->next = NULL;

  return t;
}

struct term *create_string_term(char *s)
{
  struct variable *v;
  struct term *t;

  v = create_struct_variable(TYPE_STRING, NULL, NULL);
  v->u.stringv = s;

  t = malloc_struct_term();
  t->type = TERM_VALUE;
  t->var = v;
  t->next = NULL;

  return t;
}

struct term *create_identifier_term(struct variable *v)
{
  struct term *t;

  assert( v != NULL );

  t = malloc_struct_term();
  t->type = TERM_VALUE;
  t->var = v;
  t->next = NULL;
  
  return t;
}

struct term *create_unary_term(enum op_type op, struct term *operand)
{
  struct term *t;

  assert( operand != NULL );
  t = malloc_struct_term();
  t->type = TERM_UNARY;
  t->op = op;
  t->left = operand;
  t->next = NULL;
  DEBUG(DBG_PARSER2,
	fprintf(stderr, "#proc %d: create_unary_term(", get_my_global_rank());
	fprint_op(stderr, op);
	fprintf(stderr, ", ");
	fprint_term(stderr, operand);
	fprintf(stderr, " ) -> ");
	fprint_term(stderr, t);
	fprintf(stderr, "\n");
	);
  return t;
}

struct term *create_binary_term(enum op_type op, 
				struct term *left, struct term *right)
{
  struct term *t;
  
  assert( left != NULL && right != NULL );
  t = malloc_struct_term();
  t->type = TERM_BINARY;
  t->op = op;
  t->left = left;
  t->right = right;
  t->next = NULL;

  DEBUG(DBG_PARSER2,
	fprintf(stderr, "#proc %d: create_binary_term(", 
		get_my_global_rank());
	fprint_op(stderr, op);
	fprintf(stderr, ", ");
	fprint_term(stderr, left);
	fprintf(stderr, ", ");
	fprint_term(stderr, right);
	fprintf(stderr, " ) ->");
	fprint_term(stderr, t);
	fprintf(stderr, "\n");
	);
  return t;
}

struct term *create_function_call(struct symboltable_entry *se, 
				  struct term *params)
{
  struct term *t;

  t = malloc_struct_term();
  t->type = TERM_FUNC;
  t->init_fp = se->init_fp;
  t->call_fp = se->call_fp;
  t->finalize_fp = se->finalize_fp;
  t->iterator_fp = se->iterator_fp;
  t->interface = se->interface;
  t->params = params;
  t->next = NULL;
  return t;
}

struct statement *create_assignment(struct variable *v, struct term *t)
{
  struct statement *s;

  assert( v != NULL && t != NULL );

  s = malloc_struct_statement();
  s->type = ST_ASSIGNMENT;
  s->next = NULL;
  s->u.assignment.lval = v;
  s->u.assignment.rval = t;

  DEBUG(DBG_PARSER2,
	fprintf(stderr, "#proc %d: create_assignment(%s, ",
		get_my_global_rank(), v->name );
	fprint_term(stderr, t);
	fprintf(stderr, ") -> ");
	fprint_statement(stderr, s);
	fprintf(stderr, "\n");
	);

  return s;
}

struct statement *create_procedure_call(struct term *t)
{
  struct statement *s;

  assert( t != NULL );

  s = malloc_struct_statement();
  s->type = ST_ASSIGNMENT;
  s->next = NULL;
  s->u.assignment.lval = NULL;
  s->u.assignment.rval = t;

  DEBUG(DBG_PARSER2,
	fprintf(stderr, "#proc %d: create_procedure_call(", 
		get_my_global_rank());
	fprint_term(stderr, t);
	fprintf(stderr, ") -> ");
	fprint_statement(stderr, s);
	fprintf(stderr, "\n");
	);

  return s;
}

struct statement *create_print(struct term *t, bool flush)
{
  struct statement *s;

  assert( t != NULL);
  s = malloc_struct_statement();
  s->type = ST_PRINT;
  s->next = NULL;
  s->u.print.val = t;
  s->u.print.flush = flush;

  DEBUG(DBG_PARSER2,
	fprintf(stderr, "#proc %d: create_print(", get_my_global_rank());
	fprint_term(stderr, t);
	fprintf(stderr, ") -> ");
	fprint_statement(stderr, s);
	fprintf(stderr, "\n");
	);

  return s;
}

struct statement *create_flush()
{
  struct statement *s;

  s = malloc_struct_statement();
  s->type = ST_FLUSH;
  s->next = NULL;

  DEBUG(DBG_PARSER2, 
	fprintf(stderr, "#proc %d: create_flush()\n", get_my_global_rank());
	);

  return s;
}

struct statement *create_measure(struct term *comm, struct term *call)
{
  struct statement *s;
  
  s = malloc_struct_statement();
  s->type = ST_MEASURE;
  s->next = NULL;
  s->u.measure.comm = comm;
  s->u.measure.call = call;

  DEBUG(DBG_PARSER2,
	fprintf(stderr, "#proc %d: create_measure(", get_my_global_rank());
	fprint_term(stderr, comm);
	fprintf(stderr, " ");
	fprint_term(stderr, call);
	fprintf(stderr, " -> ");
	fprint_statement(stderr, s);
	fprintf(stderr, "\n");
	);

  return s;
}

struct statement *create_for_loop(struct variable *v, bool refine, 
				  struct term *first, struct term *last, 
				  struct statement *body)
{
  struct statement *s;
  
  assert( v != NULL );

  s = malloc_struct_statement();
  s->type = ST_ARITHM_VAR;
  s->next = NULL;
  s->u.arithm_var.loopvar = v;
  s->u.arithm_var.refine = refine;
  s->u.arithm_var.first = first;
  s->u.arithm_var.last = last;
  s->u.arithm_var.step = create_int_term(1);
  s->u.arithm_var.op = OP_ADD;
  s->u.arithm_var.multipleof = NULL;
  s->u.arithm_var.body = body;

  return s;
}

struct statement *create_for_step_loop(struct variable *v, bool refine,
    struct term *first, struct term *last, enum op_type op, 
    struct term *step, struct statement *body)
{
  struct statement *s;
  
  s = create_for_loop(v, refine, first, last, body);
  s->u.arithm_var.op = op;
  s->u.arithm_var.step = step;
  return s;
}

struct statement *create_for_step_multipleof_loop(struct variable *v, 
    bool refine, struct term *first, struct term *last, enum op_type op, 
    struct term *step, struct term *multipleof, struct statement *body)
{
  struct statement *s;
  
  s = create_for_loop(v, refine, first, last, body);
  s->u.arithm_var.op = op;
  s->u.arithm_var.step = step;
  s->u.arithm_var.multipleof = multipleof;
  return s;
}

struct statement *create_for_list_loop(struct variable *v,
				       struct term *term_list,
				       struct statement *body)
{
  struct statement *s;

  assert( v != NULL );
  s = malloc_struct_statement();
  s->type = ST_LIST_VAR;
  s->next = NULL;
  s->u.list_var.loopvar = v;
  s->u.list_var.term_list = term_list;
  s->u.list_var.body = body;
  return s;
}

struct statement *create_for_iterator_loop(struct variable *v,
					   struct symboltable_entry *se,
					   struct term *params,
					   struct statement *body)
{
  struct statement *s;

  assert( v != NULL );
  assert( se != NULL );

  s = malloc_struct_statement();
  s->type = ST_ITERATOR_VAR;
  s->next = NULL;
  s->u.iterator_var.loopvar = v;
  s->u.iterator_var.iterator_fp = se->iterator_fp;
  s->u.iterator_var.par_list = params;
  s->u.iterator_var.interface = se->interface;
  s->u.iterator_var.body = body;
  return s;
}

struct statement *create_if_then_else(struct term *condition, 
				      struct statement *then_body,
				      struct statement *else_body)
{
  struct statement *s;

  s = malloc_struct_statement();
  s->type = ST_IF_THEN;
  s->next = NULL;
  s->u.if_then.condition = condition;
  s->u.if_then.then_body = then_body;
  s->u.if_then.else_body = else_body;
  return s;
}
