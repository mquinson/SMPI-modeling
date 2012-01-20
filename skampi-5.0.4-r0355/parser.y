/* SKaMPI   MPI-Benchmark

Copyright 2003-2005 Werner Augustin
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

%{

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#include "misc.h"
#include "debug.h"
#include "output.h"
#include "main.h"
#include "private_misc.h"
#include "exec.h"
#include "print.h"
#include "symboltable.h"
#include "compile.h"

struct loop_variable {
  struct variable *variable;
  bool refine;
};


extern int yylex(void); /* @@ actually one should include some header file */
                        /*    created by flex */


int current_line_number;
char *current_filename;
char *input_filename;
char *input_path;
int char_counter;

char *measurement_name;

void print_source(int from, int to)
{
  int i;
  int newline;
  char c;
  
  /* move to first non-whitespace line */
  int line_start = from;
  i = from;
  while (i < to) {
    c = source_buf.srcbuf[i];
    if (c == '\n') line_start = i+1;
    if (!(c == '\n' || c == '\t' || c == ' '))
      break;
    i++;
  }
  from = line_start;
  
  newline = 1;
  for (i=from; i<to; i++) {
    if (newline) {
      print_output("#> ");
      newline = 0;
    }
    print_output("%c", source_buf.srcbuf[i]);
    if (source_buf.srcbuf[i] == '\n') {
      newline = 1;
    }
  }
  print_output("\n\n");
}

void store_measurement_pos()
{
  if (source_buf.max_measurements <= source_buf.num_measurements) {
    source_buf.measurement_pos = realloc(source_buf.measurement_pos, source_buf.max_measurements*2);
    source_buf.max_measurements *= 2;
  }
  source_buf.measurement_pos[source_buf.num_measurements] = char_counter;
  source_buf.num_measurements++;
}

void yyerror(char *s)
{
  if( grootproc() ) fprintf(stderr, "%s in line %d\n", s, current_line_number); 
  parser_result = False;
}

%}

%union {
  char *identifier;
  int intv;
  double doublev;
  char *stringv;
  struct symboltable_entry *symboltable_entry;
  struct variable *variable;
  struct loop_variable *loop_variable;
  struct term *term;
  struct statement *statement;
}

%error-verbose

%token <identifier> IDENTIFIER
%token <stringv>  STRING
%token <intv> INTEGER
%token <doublev> DOUBLE
%token BEGIN_TOKEN END MEASUREMENT DIAGRAM RESULT TO STEP METHOD PRINT PRINTALL FLUSH
%token FOR DO OD IN MEASURE IF THEN ELSE FI EQ NEQ LE GE AND OR NOT NOTLIMITED
%token MULTIPLEOF AUTOREFINE ILLEGALCHAR

%type <statement> meas_statement meas_statement_list meas_statement_list0
%type <statement> assignment simple_statement procedure_call
%type <term> term value term_list term_list1 function_call for_end_term
%type <variable> for_in_head
%type <loop_variable> for_head
%type <symboltable_entry> iterator

%left OR
%left AND
%left '|'
%left '&'
%left EQ NEQ
%left LE GE '<' '>'
%left '+' '-'
%left '*' '/' '%'
%right NOT '~'

%start measurement_list

%%

/*---------------------------------------------------------------------------*/

meas_statement_list: meas_statement
{ 
  $$ = $1;
} 
| meas_statement_list meas_statement 
{ 
  struct statement *s;
  iodebug(DBG_PARSER2, "meas_statement_list: meas_statement_list meas_statement\n");
  for( s = $1; s->next != NULL ; s = s->next ) ;
  s->next = $2;
  $$ = $1;
}

meas_statement_list0:
{ $$ = NULL; }
| meas_statement_list
{ $$ = $1; }

simple_statement: assignment { $$ = $1; } | procedure_call { $$ = $1; }

assignment: IDENTIFIER '=' term
{ 
  iodebug(DBG_PARSER2, "assignment: IDENTIFIER = term\n");
  $$ = create_assignment(search_lvalue($1), $3);  
}

procedure_call: function_call
{
  iodebug(DBG_PARSER2, "procedure_call: '%s'\n", search_fname($1->call_fp, $1->iterator_fp)); 
  $$ = create_procedure_call($1);
}


meas_statement: 
simple_statement { $$ = $1; }
| PRINT term_list1
{
  iodebug(DBG_PARSER2, "PRINT term\n");
  $$ = create_print($2, 0);
}
| PRINTALL term_list1
{
  iodebug(DBG_PARSER2, "PRINTALL term\n");
  $$ = create_print($2, 1);
}
| FLUSH
{
  iodebug(DBG_PARSER2, "FLUSH\n");
  $$ = create_flush();
}

| MEASURE term ':' term
{
  iodebug(DBG_PARSER2, "MEASURE term\n");
  $$ = create_measure($2, $4);
}

| for_head term TO for_end_term DO meas_statement_list0 OD
{
  $$ = create_for_loop($1->variable, $1->refine, $2, $4, $6);
  free($1);
}

| for_head term TO for_end_term STEP term DO meas_statement_list0 OD
{
  $$ = create_for_step_loop($1->variable, $1->refine, $2, $4, OP_ADD, $6, $8);
  free($1);
}

| for_head term TO for_end_term STEP '*' term DO meas_statement_list0 OD
{
  $$ = create_for_step_loop($1->variable, $1->refine, $2, $4, OP_MULT,  $7, $9);
  free($1);
}

| for_head term TO for_end_term STEP '*' term MULTIPLEOF term DO meas_statement_list0 OD
{
  $$ = create_for_step_multipleof_loop($1->variable, $1->refine, $2, $4, OP_MULT,  $7, $9, $11);
  free($1);
}
| for_in_head '[' term_list ']' DO meas_statement_list0 OD
{
  $$ = create_for_list_loop($1, $3, $6);
}
| for_in_head iterator '(' term_list ')' DO meas_statement_list0 OD
{
  $$ = create_for_iterator_loop($1, $2, $4, $7);
}
| IF term THEN meas_statement_list0 FI
{
  $$ = create_if_then_else($2, $4, NULL);
}
| IF term THEN meas_statement_list0 ELSE meas_statement_list0 FI
{
  $$ = create_if_then_else($2, $4, $6);
}

for_head: FOR IDENTIFIER '='
{
  $$ = malloc(sizeof(struct loop_variable));
  $$->variable = search_lvalue($2);
  $$->refine = 0;
}
| AUTOREFINE IDENTIFIER '='
{
  $$ = malloc(sizeof(struct loop_variable));
  $$->variable = search_lvalue($2);
  $$->refine = 1;
}

for_in_head: FOR IDENTIFIER IN
{
  $$ = search_lvalue($2);
}

term_list1: term
{
  $$ = $1;
}
|
term_list ',' term
{ 
  struct term *t;
  for( t = $1; t->next != NULL ; t = t->next ) ;
  t->next = $3;
  $$ = $1;
}

term_list: 
{ $$ = NULL; }
| term
{
  $$ = $1;
}
| term_list ',' term
{
  struct term *t;
  for( t = $1; t->next != NULL ; t = t->next ) ;
  t->next = $3;
  $$ = $1;
}

function_call: IDENTIFIER '(' term_list ')' 
{
  int n, interface_length;
  struct term *t;
  struct symboltable_entry *se;

  iodebug(DBG_PARSER2, "function_call: '%s' (", $1);

  se = search_function($1);

  if( se == NULL ) 
    error_with_abort(5, "unknown function name '%s' in line %d\n", 
		     $1, current_line_number);

  for( n = 0, t = $3; t != NULL; n++, t = t->next ) ; 
  
  interface_length = strlen(se->interface);
  if( n < interface_length ) 
    error_with_abort(7, "too few parameters for function '%s' in line %d, expecting %d instead of %d\n",
		     $1, current_line_number, interface_length, n);
  else if( n > interface_length ) 
    error_with_abort(6, "too many parameters for function '%s' in line %d, expecting %d instead of %d\n", 
		     $1, current_line_number, interface_length, n);
  $$ = create_function_call(se, $3);
}

for_end_term: term
{
  $$ = $1;
}
| NOTLIMITED
{
  $$ = NULL;
}

term: value 
{ 
  iodebug(DBG_PARSER2, "term: value\n");
  $$ = $1;
}
| IDENTIFIER
{
  struct variable *v;


  v = search_rvalue($1);
  if( v == NULL ) 
    error_with_abort(9, "unknown identifier '%s' in line %d\n", $1, current_line_number);
  else
    iodebug(DBG_PARSER2, "term: identifier '%s {%s}'\n", $1, var_type_to_string(v->type));

  $$ = create_identifier_term(v);
}
| '{' term_list '}' { $$ = create_array_term($2); }
| term '+' term {  $$ = create_binary_term(OP_ADD, $1, $3); }
| term '-' term {  $$ = create_binary_term(OP_SUB, $1, $3); }
| term '*' term {  $$ = create_binary_term(OP_MULT, $1, $3); }
| term '/' term {  $$ = create_binary_term(OP_DIV, $1, $3); }
| term '%' term {  $$ = create_binary_term(OP_MOD, $1, $3); }
| '(' term ')'  {  $$ = $2; } 
| '-' term { $$ = create_unary_term(OP_SUB, $2); }
| '+' term { $$ = $2; }
| function_call { $$ = $1; }
| term '<' term { $$ = create_binary_term(OP_LT, $1, $3); }
| term '>' term { $$ = create_binary_term(OP_GT, $1, $3); }
| term EQ term  { $$ = create_binary_term(OP_EQ, $1, $3); }
| term NEQ term { $$ = create_binary_term(OP_NEQ, $1, $3); }
| term LE term  { $$ = create_binary_term(OP_LE, $1, $3); }
| term GE term  { $$ = create_binary_term(OP_GE, $1, $3); }
| term AND term { $$ = create_binary_term(OP_AND, $1, $3); }
| term '&' term { $$ = create_binary_term(OP_BAND, $1, $3); }
| term OR term  { $$ = create_binary_term(OP_OR, $1, $3); }
| term '|' term  { $$ = create_binary_term(OP_BOR, $1, $3); }
| NOT term      { $$ = create_unary_term(OP_NOT, $2); }
| '~' term      { $$ = create_unary_term(OP_BNOT, $2); }

value: INTEGER 
{
  iodebug(DBG_PARSER2, "value: INTEGER %d\n", $1);
  $$ = create_int_term($1);
}
| DOUBLE 
{
  iodebug(DBG_PARSER2, "value: DOUBLE %f\n", $1);
  $$ = create_double_term($1);
}
| STRING 
{
  iodebug(DBG_PARSER2, "value: STRING '%s'\n", $1);
  $$ = create_string_term($1);
}

measurement: BEGIN_TOKEN MEASUREMENT STRING 
{ 
  inside_measurement_block = True; 
  measurement_num++;
  just_entered_measurement_block = True;
  if( get_my_global_rank() == get_output_rank() && !syntax_check_only) {
    if (log_source) 
      print_source(source_buf.measurement_pos[measurement_num-1], source_buf.measurement_pos[measurement_num]);
    print_output("# begin result \"%s\"\n", $3);
    measurement_name = $3;
  }
}
meas_statement_list0 END MEASUREMENT 
{ 
  double start_time = 0.0;

  inside_measurement_block = False; 
  if( ! syntax_check_only ) {
    if( get_my_global_rank() == get_output_rank() ) start_time = MPI_Wtime(); 
    execute_statement($5);
    flush_output();
    clear_locals(); 
    if( get_my_global_rank() == get_output_rank() ) {
      print_output("# end result \"%s\"\n", $3);
      print_output("# duration = %4.2f sec\n\n", MPI_Wtime() - start_time);
    }
  } else {
    store_measurement_pos();	
  }
  
}

measurement_list: measurement_list measurement 
| measurement
| measurement_list simple_statement 
{ if( !syntax_check_only ) execute_statement($2); }
| simple_statement 
{ if( !syntax_check_only ) execute_statement($1); }


iterator: IDENTIFIER
{
  struct symboltable_entry *se;
  
  se = search_iterator($1);
  if( se == NULL ) 
    error_with_abort(10, "unknown iterator '%s' in line %d\n", 
		     $1, current_line_number);
  $$ = se;
}


%%




