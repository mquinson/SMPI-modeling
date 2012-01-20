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
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

#include "mpiversiontest.h"

#include "misc.h"
#include "debug.h"
#include "main.h"
#include "private_misc.h"
#include "exec.h"
#include "print.h"
#include "symboltable.h"
#include "measurement.h"

static struct symboltable_entry *symboltable_root;

extern void init_functions(void); /* generated, located in m.c */

static void init_builtin_datatypes(void);
static void init_builtin_operators(void);
static void init_builtin_comms(void);
static void init_builtin_info(void);
static void init_builtin_constants(void);

void init_symboltable(void) 
{
  symboltable_root = NULL;
  init_functions();
  init_builtin_datatypes();
  init_builtin_operators();
  init_builtin_comms();
  init_builtin_info();
  init_builtin_constants();
  init_builtin_measurement_functions();
}


static struct symboltable_entry *search_name(char *name)
{
  struct symboltable_entry *p;

  for( p = symboltable_root; p != NULL; p = p->next ) {
    /* debug(DBG_SYMBOLT, "checking '%s'\n", p->name); */
    if( !strcmp(name, p->name) ) return p;
  }
  return NULL;
}


struct variable *search_lvalue(char *name)
{
  struct symboltable_entry *p;
  struct variable *v;

  debug(DBG_SYMBOLT, "search_lvalue(%s) -> ", name);

  p = search_name(name);
  if( p == NULL) {
    p = (struct symboltable_entry*)malloc(sizeof(struct symboltable_entry));
    assert( p != NULL );
    p->name = name;
    p->globalv = NULL;
    p->localv = NULL;
    p->next = symboltable_root;
    symboltable_root = p;
  }

  if( inside_measurement_block ) {
    if( p->localv ) {
      debug(DBG_SYMBOLT, "found localv\n");
      return p->localv;
    } else {
      v = create_struct_variable(TYPE_VOID, name, NULL);
      p->localv = v;
      debug(DBG_SYMBOLT, "new localv\n"); 
      return v;
    }
  } else {
    if( p->globalv ) {
      debug(DBG_SYMBOLT, "found globalv\n"); 
      return p->globalv;
    } else {
      v = create_struct_variable(TYPE_VOID, name, NULL);
      p->globalv = v;
      debug(DBG_SYMBOLT, "new globalv\n"); 
      return v;
    }
  }
}

struct variable *search_rvalue(char *name)
{
  struct symboltable_entry *p;

  debug(DBG_SYMBOLT, "search_rvalue(%s) ->", name);
  p = search_name(name);
  if( p == NULL ) return NULL;
  /*    debug(ERROR, "search_rvalue(\"%s\") was not succesfull\n", name);*/
  /* assert( p != NULL ); */
  if( p->localv ) { debug(DBG_SYMBOLT, "found localv\n"); return p->localv; }
  else            { debug(DBG_SYMBOLT, "found globalv\n"); return p->globalv; }
}

struct symboltable_entry *search_function(char *name)
{
  struct symboltable_entry *p;

  debug(DBG_SYMBOLT, "search_function(%s)\n", name);
  p = search_name(name);
 if( p != NULL && p->call_fp == NULL ) return NULL;
 return p;
}

struct symboltable_entry *search_iterator(char *name)
{
  struct symboltable_entry *p;

  debug(DBG_SYMBOLT, "search_iterator(%s)\n", name);
  p = search_name(name);
 if( p != NULL && p->iterator_fp == NULL ) return NULL;
 return p;
}

void insert_function(type_normal_fp init_fp, type_normal_fp call_fp,
		     type_normal_fp finalize_fp, 
		     char *name, char *interface, 
		     enum var_type result_type)
{
  struct symboltable_entry *p;

  DEBUG(DBG_SYMBOLT, 
  { 
    fprintf(stderr, "#proc %d: insert_function(..., \"%s\", \"%s\", ", 
	    get_my_global_rank(), name, interface);
    fprint_var_type(stderr, result_type); 
    fprintf(stderr, ")\n");
  };);

  assert( search_name(name) == NULL );
  p = (struct symboltable_entry*)malloc(sizeof(struct symboltable_entry));
  assert( p != NULL );
  p->name = name;
  p->localv = NULL;
  p->globalv = NULL;

  p->init_fp = init_fp;
  p->call_fp = call_fp;
  p->finalize_fp = finalize_fp;
  p->iterator_fp = NULL;
  p->interface = interface;
  p->result_type = result_type;
  p->next = symboltable_root;
  symboltable_root = p;
}

void insert_iterator(type_iterator_fp iterator_fp,
		     char *name, char *interface, 
		     enum var_type result_type)
{
  struct symboltable_entry *p;

  DEBUG(DBG_SYMBOLT, 
  { 
    fprintf(stderr, "#proc %d: insert_iterator(..., \"%s\", \"%s\", ", 
	   get_my_global_rank(), name, interface);
    fprint_var_type(stderr, result_type); 
    fprintf(stderr, ")\n");
  };);

  assert( search_name(name) == NULL );
  p = (struct symboltable_entry*)malloc(sizeof(struct symboltable_entry));
  assert( p != NULL );
  p->name = name;
  p->localv = NULL;
  p->globalv = NULL;

  p->init_fp = NULL;
  p->call_fp = NULL;
  p->finalize_fp = NULL;
  p->iterator_fp = iterator_fp;
  p->interface = interface;
  p->result_type = result_type;
  p->next = symboltable_root;
  symboltable_root = p;
}

char *search_fname(type_normal_fp call_fp, type_iterator_fp iterator_fp)
{
  struct symboltable_entry *p;

  for( p = symboltable_root; p != NULL; p = p->next )
    if( p->call_fp == call_fp && p->iterator_fp == iterator_fp )
      /* if( p->call_fp != NULL || p->iterator_fp != NULL ) */
      return p->name;
  assert( 0 ); /* shouldn't reach this line */
  return NULL; 
}

void clear_locals(void)
{
  struct symboltable_entry *p;

  debug(DBG_SYMBOLT, "clear_locals()\n");
  for( p = symboltable_root; p != NULL ; p = p->next )
    if( p->localv != NULL ) {
      free_variable(p->localv);
      p->localv = NULL;
    }
}

void print_symboltable(void)
{
  struct symboltable_entry *s;
  char *name;
  
  for( s = symboltable_root; s != NULL ; s = s->next ) {
    if( s->name != NULL ) name = s->name;
    else                  name = "noname";
    if( s->init_fp != NULL || s->call_fp != NULL || s->finalize_fp != NULL ||
	s->iterator_fp)
      printf("function %s\n", name);
    else {
      if( s->globalv != NULL ) {
	printf("global %s=", name);
	fprint_variable(stdout, s->globalv);
	printf("\n");
      } else 
	printf("global %s not defined\n", name);
      
      if( s->localv != NULL ) {
	printf("local %s=", name);
	fprint_variable(stdout, s->localv);
	printf("\n");
      } else 
	printf("local %s not defined\n", name);
    }
  }
}

static void insert_datatype(char *name, MPI_Datatype datatype)
{
  struct variable *v;

  v = search_lvalue(name);
  init_struct_variable(v, TYPE_DATATYPE, name, name);
  increase_reference(v);
  v->u.datatypev = datatype;
  debug(DBG_SYMBOLT, "inserting %s\n", name);
}

static void insert_communicator(char *name, MPI_Comm comm)
{
  struct variable *v;

  v = search_lvalue(name);
  init_struct_variable(v, TYPE_COMM, name, name);
  increase_reference(v);
  v->u.commv = comm;
  debug(DBG_SYMBOLT, "inserting %s\n", name);
}

void init_builtin_datatypes(void)
{
  insert_datatype("MPI_CHAR"           , MPI_CHAR            );      
  insert_datatype("MPI_BYTE"	       , MPI_BYTE            );
  insert_datatype("MPI_SHORT"	       , MPI_SHORT           );
  insert_datatype("MPI_INT"	       , MPI_INT             );
  insert_datatype("MPI_LONG"	       , MPI_LONG            );
  insert_datatype("MPI_FLOAT"	       , MPI_FLOAT           );
  insert_datatype("MPI_DOUBLE"	       , MPI_DOUBLE          );
  insert_datatype("MPI_UNSIGNED_CHAR"  , MPI_UNSIGNED_CHAR   );
  insert_datatype("MPI_UNSIGNED_SHORT" , MPI_UNSIGNED_SHORT  );
  insert_datatype("MPI_UNSIGNED"       , MPI_UNSIGNED        );
  insert_datatype("MPI_UNSIGNED_LONG"  , MPI_UNSIGNED_LONG   );
  insert_datatype("MPI_LONG_DOUBLE"    , MPI_LONG_DOUBLE     );
  insert_datatype("MPI_FLOAT_INT"      , MPI_FLOAT_INT       );
  insert_datatype("MPI_LONG_INT"       , MPI_LONG_INT        );
  insert_datatype("MPI_DOUBLE_INT"     , MPI_DOUBLE_INT      );
  insert_datatype("MPI_SHORT_INT"      , MPI_SHORT_INT       );
  insert_datatype("MPI_2INT"	       , MPI_2INT            );
  insert_datatype("MPI_LONG_DOUBLE_INT", MPI_LONG_DOUBLE_INT );
  insert_datatype("MPI_LONG_LONG_INT"  , MPI_LONG_LONG_INT   );

  insert_datatype("MPI_LB"             , MPI_LB              );
  insert_datatype("MPI_UB"             , MPI_UB              );
  /* @@@ check if these are necessary ?? 
       MPI_PACKED
       MPI_UB - For MPI_Type_struct ; an upper-bound indicator
       MPI_LB - For MPI_Type_struct ; a lower-bound indicator
  */
}

static void insert_operator(char *name, MPI_Op op)
{
  struct variable *v;

  v = search_lvalue(name);
  init_struct_variable(v, TYPE_OP, name, name);
  increase_reference(v);
  v->u.opv = op;
  debug(DBG_SYMBOLT, "inserting %s\n", name);
}

void init_builtin_operators(void)
{
  insert_operator("MPI_MAX"    , MPI_MAX     );
  insert_operator("MPI_MIN"    , MPI_MIN     );	   
  insert_operator("MPI_SUM"    , MPI_SUM     );	   
  insert_operator("MPI_PROD"   , MPI_PROD    );
  insert_operator("MPI_LAND"   , MPI_LAND    );
  insert_operator("MPI_BAND"   , MPI_BAND    );
  insert_operator("MPI_LOR"    , MPI_LOR     );	   
  insert_operator("MPI_BOR"    , MPI_BOR     );	   
  insert_operator("MPI_LXOR"   , MPI_LXOR    );
  insert_operator("MPI_BXOR"   , MPI_BXOR    );
  insert_operator("MPI_MINLOC" , MPI_MINLOC  );
  insert_operator("MPI_MAXLOC" , MPI_MAXLOC  );
#ifdef USE_ONESIDED
  insert_operator("MPI_REPLACE", MPI_REPLACE );
#endif
}

void init_builtin_comms(void)
{
  insert_communicator("MPI_COMM_WORLD", MPI_COMM_WORLD);
  insert_communicator("MPI_COMM_SELF" , MPI_COMM_SELF );
  insert_communicator("MPI_COMM_NULL" , MPI_COMM_NULL );
}

#ifdef USE_MPI_INFO
void insert_info(char *name, MPI_Info info)
{
  struct variable *v;

  v = search_lvalue(name);
  init_struct_variable(v, TYPE_INFO, name, name);
  increase_reference(v);
  v->u.infov = info;
  debug(DBG_SYMBOLT, "inserting %s\n", name);
}
#endif

void init_builtin_info(void)
{
#ifdef USE_MPI_INFO
  insert_info("MPI_INFO_NULL", MPI_INFO_NULL);
#endif
}

#if defined(USE_ONESIDED) || defined(USE_MPI_IO)
static void insert_constant(char *name, int constant)
{
  struct variable *v;

  v = search_lvalue(name);
  init_struct_variable(v, TYPE_INTEGER, name, name);
  increase_reference(v);
  v->u.intv = constant;
  debug(DBG_SYMBOLT, "inserting %s\n", name);
}
#endif

/* fixme: it shouldn't be allowed to overwrite these constants
   in the config file */

void init_builtin_constants(void)
{
#ifdef USE_ONESIDED
  insert_constant("MPI_LOCK_EXCLUSIVE"      , MPI_LOCK_EXCLUSIVE);
  insert_constant("MPI_LOCK_SHARED"         , MPI_LOCK_SHARED);
  insert_constant("MPI_MODE_NOCHECK"        , MPI_MODE_NOCHECK);
  insert_constant("MPI_MODE_NOPRECEDE"      , MPI_MODE_NOPRECEDE);
  insert_constant("MPI_MODE_NOPUT"          , MPI_MODE_NOPUT);
  insert_constant("MPI_MODE_NOSTORE"        , MPI_MODE_NOSTORE);
  insert_constant("MPI_MODE_NOSUCCEED"      , MPI_MODE_NOSUCCEED);
#endif
#ifdef USE_MPI_IO
  insert_constant("MPI_MODE_APPEND"         , MPI_MODE_APPEND);
  insert_constant("MPI_MODE_CREATE"         , MPI_MODE_CREATE);
  insert_constant("MPI_MODE_DELETE_ON_CLOSE", MPI_MODE_DELETE_ON_CLOSE);
  insert_constant("MPI_MODE_EXCL"           , MPI_MODE_EXCL);
  insert_constant("MPI_MODE_RDONLY"         , MPI_MODE_RDONLY);
  insert_constant("MPI_MODE_RDWR"           , MPI_MODE_RDWR);
  insert_constant("MPI_MODE_SEQUENTIAL"     , MPI_MODE_SEQUENTIAL);
  insert_constant("MPI_MODE_UNIQUE_OPEN"    , MPI_MODE_UNIQUE_OPEN);
  insert_constant("MPI_MODE_WRONLY"         , MPI_MODE_WRONLY);

  insert_constant("MPI_DISTRIBUTE_BLOCK"    , MPI_DISTRIBUTE_BLOCK);
  insert_constant("MPI_DISTRIBUTE_CYCLIC"   , MPI_DISTRIBUTE_CYCLIC);
  insert_constant("MPI_DISTRIBUTE_NONE"     , MPI_DISTRIBUTE_NONE);
  insert_constant("MPI_DISTRIBUTE_DFLT_DARG", MPI_DISTRIBUTE_DFLT_DARG);
#endif
}
