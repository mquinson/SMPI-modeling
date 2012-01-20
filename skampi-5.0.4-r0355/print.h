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

void print_output_variable_value(struct variable *v);
void fprint_variable_value(FILE *f, struct variable *v);
char *var_type_to_string(enum var_type type);
void fprint_var_type(FILE *f, enum var_type type);

void fprint_variable(FILE *f, struct variable *v);
void fprint_op(FILE *f, enum op_type op);
void fprint_term(FILE *f, struct term *t);
void fprint_statement(FILE *f, struct statement *s);
void fprint_variable_value(FILE *f, struct variable *v);



