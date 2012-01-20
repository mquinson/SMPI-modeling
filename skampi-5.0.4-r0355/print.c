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
#include "output.h"
#include "exec.h"
#include "symboltable.h"
#include "print.h"

static void fprintf_comm(FILE *f, MPI_Comm comm);
static void print_output_comm(MPI_Comm comm);
static void fprint_iarray(FILE *f, IArray a);
static void print_output_iarray(IArray a);
static char* datatype_string(MPI_Datatype t);
static void fprint_darray(FILE *f, DArray a);
static void print_output_darray(DArray a);

void fprint_variable_value(FILE *f, struct variable *v)
{
  assert( v != NULL);
  if( v->string_repr != NULL ) {
    fprintf(f, "%s", v->string_repr);
    return;
  }
  switch( v->type ) {
  case TYPE_INTEGER: fprintf(f, "%d", v->u.intv); break;
  case TYPE_DOUBLE: fprintf(f, "%f", v->u.doublev); break;
  case TYPE_STRING: fprintf(f, "\"%s\"", v->u.stringv); break;
  case TYPE_DATATYPE: fprintf(f, "<datatype>"); break;
  case TYPE_DARRAY: fprint_darray(f, v->u.darrv); break;
  case TYPE_IARRAY: fprint_iarray(f, v->u.iarrv); break;
  case TYPE_COMM: fprintf_comm(f, v->u.commv); break;
  case TYPE_OP: fprintf(f, "<operator>"); break;
  case TYPE_INFO: fprintf(f, "<info>"); break;
  default:
    error_without_abort("out of cases in exec.c__fprint_variable_value() v->type = %d\n", v->type);
  }
}

void print_output_variable_value(struct variable *v)
{
  assert( v != NULL);
  if( v->string_repr != NULL ) {
    print_output( "%s", v->string_repr);
    return;
  }
  switch( v->type ) {
  case TYPE_INTEGER: print_output("%d", v->u.intv); break;
  case TYPE_DOUBLE: print_output("%f", v->u.doublev); break;
  case TYPE_STRING: print_output("%s", v->u.stringv); break;
  case TYPE_DATATYPE: print_output("<datatype>"); break;
  case TYPE_DARRAY: print_output_darray(v->u.darrv); break;
  case TYPE_IARRAY: print_output_iarray(v->u.iarrv); break;
  case TYPE_COMM: print_output_comm(v->u.commv); break;
  case TYPE_OP: print_output("<operator>"); break;
  case TYPE_INFO: print_output("<info>"); break;
  default:
    error_without_abort("out of cases in exec.c__print_output_variable_value() v->type = %d\n", v->type);
  }
}

char *var_type_to_string(enum var_type type)
{
  switch( type ) {
  case TYPE_INTEGER : return "int";
  case TYPE_DOUBLE  : return "double";
  case TYPE_STRING  : return "string";
  case TYPE_DATATYPE: return "datatype";
  case TYPE_COMM    : return "comm";
  case TYPE_OP      : return "operator"; 
  case TYPE_INFO    : return "info";
  case TYPE_VOID    : return "void";
  case TYPE_IARRAY  : return "iarray";
  case TYPE_DARRAY  : return "darray";
  default:
    error_without_abort("out of cases in exec.c__var_type_to_string() -> %d\n",
			type);
    return ""; 
  }
}

void fprint_var_type(FILE *f, enum var_type type)
{
  fprintf(f, var_type_to_string(type));
}

void fprint_variable(FILE *f, struct variable *v)
{
  if( v->name != NULL ) {
    fprintf(f, "%s{", v->name);
    fprint_var_type(f, v->type);
    if( v->ref_counter )
      fprintf(f, ",%d}", *(v->ref_counter));
    else
      fprintf(f, "}"); 
  }  else
    fprint_variable_value(f, v);
}


void fprint_op(FILE *f, enum op_type op) 
{
  switch( op ) {
  case OP_ADD:  fprintf(f, "+"); break;
  case OP_SUB:  fprintf(f, "-"); break;
  case OP_MULT: fprintf(f, "*"); break;
  case OP_DIV:  fprintf(f, "/"); break;
  case OP_MOD:  fprintf(f, "%%"); break;
  case OP_LT:  fprintf(f, "<"); break;
  case OP_LE:  fprintf(f, "<="); break;
  case OP_GT:  fprintf(f, ">"); break;
  case OP_GE:  fprintf(f, ">="); break;
  case OP_EQ:  fprintf(f, "=="); break;
  case OP_NEQ: fprintf(f, "!="); break;
  case OP_AND: fprintf(f, "and"); break;
  case OP_OR:  fprintf(f, "or"); break;
  case OP_NOT: fprintf(f, "not"); break;                
  default:
    error_without_abort("out of cases in exec.c__fprint_op()\n");
  }
}

void fprint_term(FILE *f, struct term *t)
{
  assert( t != NULL);
  switch( t->type ) {
  case TERM_VALUE:
    fprint_variable(f, t->var);
    break;
  case TERM_UNARY:
    fprint_op(f, t->op);
    fprintf(f, "(");
    fprint_term(f, t->left);
    fprintf(f, ")");
    break;
  case TERM_BINARY:
    fprintf(f, "(");
    fprint_term(f, t->left);
    fprint_op(f, t->op);
    fprint_term(f, t->right);
    fprintf(f, ")");
    break;
  case TERM_FUNC:
    fprintf(f, "%s(...)", search_fname(t->call_fp, t->iterator_fp));
    /*
    for( params = t->params; params->next != NULL; params = params->next) {
      print_term(params);
      printf(", ");
    }
    print_term(params);
    printf(")");
    */
    break;
  default:
    error_without_abort("out of cases in exec.c__fprint_term()\n");
  }
}

void fprint_statement(FILE *f, struct statement *s)
{
  struct term *t;
  assert( s != NULL);
  switch( s->type ) {
  case ST_ASSIGNMENT:
    if( s->u.assignment.lval != NULL && s->u.assignment.lval->name != NULL ) 
      fprintf(f, "%s=", s->u.assignment.lval->name);
    fprint_term(f, s->u.assignment.rval);
    break;
  case ST_PRINT:
    fprintf(f, "print ");
    fprint_term(f, s->u.print.val);
    break;
  case ST_MEASURE:
    fprintf(f, "measure ");
    fprint_term(f, s->u.measure.comm);
    fprintf(f, " ");
    fprint_term(f, s->u.measure.call);
    break;
  case ST_ARITHM_VAR:
    fprintf(f, "for ");
    fprint_variable(f, s->u.arithm_var.loopvar);
    fprintf(f, " = ");
    fprint_term(f, s->u.arithm_var.first);
    fprintf(f, " to ");
    if( s->u.arithm_var.last ) 
      fprint_term(f, s->u.arithm_var.last);
    else
      fprintf(f, "...");
    fprintf(f, " step ");
    fprint_op(f, s->u.arithm_var.op);
    fprint_term(f, s->u.arithm_var.step);
    fprintf(f, " do...");
    break;
  case ST_LIST_VAR:
    fprintf(f, "for ");
    fprint_variable(f, s->u.list_var.loopvar);
    fprintf(f, " in [");
    if( s->u.list_var.term_list != NULL ) {
      for( t = s->u.list_var.term_list; t->next != NULL; t = t->next) {
	fprint_term(f, t);
	fprintf(f, ", ");
      }
      fprint_term(f, t);
    }
    fprintf(f, " ] do...");
    break;
  case ST_ITERATOR_VAR:
    fprintf(f, "for ");
    fprint_variable(f, s->u.iterator_var.loopvar);
    fprintf(f, " in ");
    fprintf(f, "i%s(...) do...", 
	   search_fname(NULL, s->u.iterator_var.iterator_fp));
    break;
  case ST_IF_THEN:
    fprintf(f, "if ");
    fprint_term(f, s->u.if_then.condition);
    fprintf(f, " then ...");
    if( s->u.if_then.else_body != NULL )
      fprintf(f, " else ... fi");
    else
      fprintf(f, " fi");
    break;
  default:
    error_without_abort("out of cases in exec.c__fprint_statement() s->type=%d\n",
			s->type);
  }
}

void fprintf_comm(FILE *f, MPI_Comm comm)
{
  int i, size, rank, *ranks = NULL, global_rank;
 
  if( comm != MPI_COMM_NULL ) {
    
    MPI_Comm_rank(comm, &rank);
    global_rank = get_my_global_rank();
    if( rank == 0 ) {
      MPI_Comm_size(comm, &size);
      ranks = skampi_malloc_ints(size);
    }

    MPI_Gather(&global_rank, 1, MPI_INT, ranks, 1, MPI_INT, 0, comm);

    if( rank == 0 ) {
      fprintf(f, "[%d", ranks[0]);
      for( i = 1; i < size; i++)
	fprintf(f, ", %d", ranks[i]);
      fprintf(f, "]");
      free(ranks);
    }
  }
}

void print_output_comm(MPI_Comm comm)
{
  int i, size, rank, *ranks = NULL, global_rank;
 
  if( comm != MPI_COMM_NULL ) {
    
    MPI_Comm_rank(comm, &rank);
    global_rank = get_my_global_rank();
    if( rank == 0 ) {
      MPI_Comm_size(comm, &size);
      ranks = skampi_malloc_ints(size);
    }

    MPI_Gather(&global_rank, 1, MPI_INT, ranks, 1, MPI_INT, 0, comm);

    if( rank == 0 ) {
      print_output("[%d", ranks[0]);
      for( i = 1; i < size; i++)
	print_output(", %d", ranks[i]);
      print_output("]\n");
      free(ranks);
    }
  }
}

void fprint_iarray(FILE *f, IArray a)
{
  int i;
  fprintf(f, "[");
  if (a.n > 0)
    fprintf(f, "%d", a.v[0]);
  for (i=1; i<a.n; i++) {
    fprintf(f, ", %d", a.v[i]);
  }
  fprintf(f, "]");
}

void print_output_iarray(IArray a)
{
  int i;
  print_output("[");
  if (a.n > 0)
    print_output("%d", a.v[0]);
  for (i=1; i<a.n; i++) {
    print_output(", %d", a.v[i]);
  }
  print_output("]");
}

char* datatype_string(MPI_Datatype t)
{
  if (t == MPI_CHAR) return "MPI_CHAR";
  else if (t == MPI_BYTE) return "MPI_BYTE";        
  else if (t == MPI_SHORT) return "MPI_SHORT";
  else if (t == MPI_INT) return "MPI_INT";  
  else if (t == MPI_LONG) return "MPI_LONG";    
  else if (t == MPI_FLOAT) return "MPI_FLOAT";   
  else if (t == MPI_DOUBLE) return "MPI_DOUBLE";  
  else if (t == MPI_UNSIGNED_CHAR) return "MPI_UNSIGNED_CHAR";
  else if (t == MPI_UNSIGNED_SHORT) return "MPI_UNSIGNED_SHORT";
  else if (t == MPI_UNSIGNED) return "MPI_UNSIGNED";
  else if (t == MPI_UNSIGNED_LONG) return "MPI_UNSIGNED_LONG";
  else if (t == MPI_LONG_DOUBLE) return "MPI_LONG_DOUBLE";
  else if (t == MPI_FLOAT_INT) return "MPI_FLOAT_INT";
  else if (t == MPI_LONG_INT) return "MPI_LONG_INT";
  else if (t == MPI_DOUBLE_INT) return "MPI_DOUBLE_INT";
  else if (t == MPI_SHORT_INT) return "MPI_SHORT_INT";
  else if (t == MPI_2INT) return "MPI_2INT";
  else if (t == MPI_LONG_DOUBLE_INT) return "MPI_LONG_DOUBLE_INT";
  else if (t == MPI_LONG_LONG_INT) return "MPI_LONG_LONG_INT";
  else if (t == MPI_LB) return "MPI_LB";
  else if (t == MPI_UB) return "MPI_UB";     
  else return "<datatype>";
}

void fprint_darray(FILE *f, DArray a)
{
  int i;
  fprintf(f, "[");
  if (a.n > 0)
    fprintf(f, datatype_string(a.v[0]));
  for (i=1; i<a.n; i++) {
    fprintf(f, ", %s", datatype_string(a.v[i]));
  }
  fprintf(f, "]");
}

void print_output_darray(DArray a)
{
  int i;
  print_output("[");
  if (a.n > 0)
    print_output(datatype_string(a.v[0]));
  for (i=1; i<a.n; i++) {
    print_output(", %s", datatype_string(a.v[i]));
  }
  print_output("]");
}
