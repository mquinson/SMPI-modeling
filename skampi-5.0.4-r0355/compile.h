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

struct term *create_int_term(int i);
struct term *create_array_term(struct term *s);
struct term *create_double_term(double f);
struct term *create_string_term(char *s);

struct term *create_identifier_term(struct variable *v);
struct term *create_unary_term(enum op_type op, struct term *operand);
struct term *create_binary_term(enum op_type op, 
				struct term *left, struct term *right);

int check_types_for_function_call(char *interface, struct term *params); 
struct term *create_function_call(struct symboltable_entry *se, struct term *params);


struct statement *create_assignment(struct variable *v, struct term *t);
struct statement *create_procedure_call(struct term *t);
struct statement *create_print(struct term *t, bool flush);
struct statement *create_flush();
struct statement *create_measure(struct term *comm, struct term *call);
struct statement *create_for_loop(struct variable *v, bool refine, 
				  struct term *first, struct term *last, 
				  struct statement *body);
struct statement *create_for_step_loop(struct variable *v, bool refine, 
    struct term *first, struct term *last, enum op_type op, 
    struct term *step, struct statement *body);
struct statement *create_for_step_multipleof_loop(struct variable *v, 
    bool refine, struct term *first, struct term *last, enum op_type op, 
    struct term *step, struct term *multipleof, struct statement *body);
struct statement *create_for_list_loop(struct variable *v,
				       struct term *term_list,
				       struct statement *body);
struct statement *create_for_iterator_loop(struct variable *v,
					   struct symboltable_entry *se, 
					   struct term *params,
					   struct statement *body);
struct statement *create_if_then_else(struct term *condition, 
				      struct statement *then_body,
				      struct statement *else_body);
