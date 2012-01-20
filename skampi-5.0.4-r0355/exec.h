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

#include "mpiversiontest.h"

enum {
  MAX_NUMBER_PARAMS = 10
};

enum var_type {
  TYPE_INTEGER  = 'i',
  TYPE_DOUBLE   = 'f',
  TYPE_STRING   = 's',
  TYPE_DATATYPE = 'd',
  TYPE_COMM     = 'c',
  TYPE_OP       = 'o',
  TYPE_INFO     = 'h',
  TYPE_VOID     = '0',
  TYPE_IARRAY   = 'a',
  TYPE_DARRAY   = 'e'
};

typedef struct {
  int* v;
  int n;
} IArray;

typedef struct {
  MPI_Datatype *v;
  int n;
} DArray;

struct variable {
  enum var_type type;
  char *name;
  char *string_repr;
  int *ref_counter;
  /*int **ref_counters;*/  /* reference counters of array elements */
  union {
    int intv;
    double doublev;
    char *stringv;
    MPI_Datatype datatypev;
    MPI_Comm commv;
    MPI_Op opv;
    IArray iarrv;
    DArray darrv;
#ifdef USE_MPI_INFO
    MPI_Info infov;
#endif
  } u;
};



typedef void (*type_normal_fp)(struct variable *result, struct variable par[]);
typedef void (*type_iterator_fp)(struct variable *result, 
				 struct variable par[], void **self);



enum term_type {
  TERM_VALUE = 300,
  TERM_UNARY,
  TERM_BINARY,
  TERM_FUNC,
  TERM_ARRAY
};

enum op_type {
  OP_ADD = 400,
  OP_SUB,
  OP_MULT,
  OP_DIV,
  OP_MOD,
  OP_LT,
  OP_LE,
  OP_GT,
  OP_GE,
  OP_EQ,
  OP_NEQ,
  OP_AND,
  OP_BAND,
  OP_OR,
  OP_BOR,
  OP_NOT,
  OP_BNOT
};
  
enum meas_error { /* bitfield */
  MEAS_OK = 0,
  MEAS_OUT_OF_BOUNDS = 1,
  MEAS_BUFFER_TOO_SMALL = 2
};

struct term {
  enum term_type type;
  /*   enum var_type result_type; */
  struct term *next;

  enum op_type op;
  struct term *left;
  struct term *right;

  struct variable *var;
  
  type_normal_fp init_fp, call_fp, finalize_fp;
  type_iterator_fp iterator_fp;
  char *interface;
  
  struct term *params;
};


enum statement_type {
  ST_ASSIGNMENT = 500,
  ST_PRINT,
  ST_FLUSH,
  ST_MEASURE,
  ST_ARITHM_VAR,
  ST_LIST_VAR,
  ST_ITERATOR_VAR,
  ST_IF_THEN
};

struct statement {
  enum statement_type type;
  struct statement *next;

  union {
    struct {
      struct variable *lval;
      struct term *rval;
    } assignment;

    struct {
      bool flush;
      struct term *val;
    } print;

    struct {
      struct term *comm;
      struct term *call;
    } measure;

    struct {
      struct variable *loopvar;
      bool refine;
      struct term *first;
      struct term *last;
      struct term *step;
      enum op_type op;
      struct term *multipleof;
      struct statement *body;
    } arithm_var;

    struct {
      struct variable *loopvar;
      struct term *term_list;
      struct statement *body;
    } list_var;

    struct {
      struct variable *loopvar;
      type_iterator_fp iterator_fp;
      struct term *par_list;
      char *interface;
      void *self;
      struct statement *body;
    } iterator_var;

    struct {
      struct term *condition;
      struct statement *then_body;
      struct statement *else_body;
    } if_then;

  } u;
};


extern struct statement *statement_root;

void increase_reference(struct variable *v); 
void decrease_reference(struct variable *v);
void init_struct_variable(struct variable *v, 
			  enum var_type type, char *name, char *string_repr);
struct variable *create_struct_variable(enum var_type type, char *name, 
					char *string_repr);
void free_variable(struct variable *v);


void free_term(struct term *t);


void evaluate_params(char *interface, struct term *source_params,
		     struct variable *dest_params);
void evaluate_term(struct variable *result, struct term *t);

void execute_statement(struct statement *s);


void store_int(struct variable *result, int i);
void store_double(struct variable *result, double f);
void store_string(struct variable *result, char *s);
void store_datatype(struct variable *result, MPI_Datatype datatype);
void store_comm(struct variable *result, MPI_Comm comm);
void store_operator(struct variable *result, MPI_Op op);
void store_iarray(struct variable* result, IArray iarr);
void store_darray(struct variable* result, DArray darr);
#ifdef USE_MPI_INFO
void store_info(struct variable *result, MPI_Info info);
#endif


extern struct variable* loopvar_stack[10]; /* @@@@ */
extern int loopvar_stack_sp;
