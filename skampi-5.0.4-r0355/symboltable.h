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

struct symboltable_entry {
  char *name;
  struct symboltable_entry *next;

  struct variable *globalv;
  struct variable *localv;

  type_normal_fp init_fp, call_fp, finalize_fp;
  type_iterator_fp iterator_fp;

  char *interface;
  enum var_type result_type;
  
};

void init_symboltable(void);


struct variable *search_lvalue(char *name);
struct variable *search_rvalue(char *name);
struct symboltable_entry *search_function(char *name);
struct symboltable_entry *search_iterator(char *name);

void insert_function(type_normal_fp init_fp, type_normal_fp call_fp,
		     type_normal_fp finalize_fp, 
		     char *name, char *interface, 
		     enum var_type result_type);

void insert_iterator(type_iterator_fp iterator_fp,
		     char *name, char *interface, 
		     enum var_type result_type);


char *search_fname(type_normal_fp call_fp, type_iterator_fp iterator_fp);

void clear_locals(void);

void print_symboltable(void);

