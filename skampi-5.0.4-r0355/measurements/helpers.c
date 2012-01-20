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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "../mpiversiontest.h"
#include "../misc.h"
#include "../private_misc.h"
#include "../exec.h"
#include "../mem.h"
#include "../synchronize.h"
#include "../accounting.h"
#include "../autorefine.h"

#ifndef  USE_MPI_ALLOC_MEM
#include "../output.h"
#endif

#pragma weak begin_skampi_extensions

void func_set_skampi_buffer(int size) {
  set_skampi_buffer(size);
}

void func_switch_buffer_cycling_on(void) {
  switch_buffer_cycling_on();
}

void func_switch_buffer_cycling_off(void) {
  switch_buffer_cycling_off();
}

void func_set_send_buffer_alignment(int a) {
  set_send_buffer_alignment(a);
}

void func_set_recv_buffer_alignment(int a) {
  set_recv_buffer_alignment(a);
}

/*---------------------------------------------------------------------------*/

void func_choose_no_synchronization(void) {
  set_synchronization(SYNC_NOSYNC);
}

void func_choose_barrier_synchronization(void) {
  set_synchronization(SYNC_BARRIER);
}

void func_choose_real_synchronization(void) {
  set_synchronization(SYNC_REAL);
}

/*---------------------------------------------------------------------------*/

void func_set_cache_size(int size)
{
  skampi_cache_size = size;
}

void func_set_autorefine_factor(double f)
{
  set_autorefine_factor(f);
}

void func_init_time_accounting(void) {
  init_user_accounting();
}

void func_print_time_accounting_info(void) {
  print_user_acc_info();
}

/*---------------------------------------------------------------------------*/

int func_get_np(void) /* deprecated */
{
  return get_global_size();
}

int func_get_comm_size(MPI_Comm comm)
{
  int size;

  MPI_Comm_size(comm, &size);
  return size;
}

int func_get_comm_rank(MPI_Comm comm)
{
  int rank;
  MPI_Comm_rank(comm, &rank);
  return rank;
}

int func_last_proc(void) /* deprecated */
{
  return get_global_size() - 1;
}

/*---------------------------------------------------------------------------*/

double func_sqrt(double x)
{
  return sqrt(x);
}

double func_cbrt(double x)
{
  return cbrt(x);
}

double func_sqr(double x)
{
  return fsqr(x);
}

int func_floor(double x)
{
  return (int)floor(x);
}

int func_round(double x)
{
  return (int)floor(x+0.5);
}

int func_ceil(double x)
{
  return (int)ceil(x);
}

int func_atoi(char* s)
{
  return atoi(s);
}

int func_modulo(int a, int b)
{
  return a % b;
}

double func_power(double a, double b)
{
  return pow(a,b);
}

double func_log(double a)
{
  return log(a);
}

double func_log2(double a)
{
  return (log(a) / log(2));
}

double func_log10(double a)
{
  return (log(a) / log(10));
}

int func_min_int(int a, int b)
{
  return (a > b) ? b : a;
}

int func_max_int(int a, int b)
{
  return (a > b) ? a : b;
}

/*---------------------------------------------------------------------------*/

MPI_Comm func_comm(int i)
{
  MPI_Comm newcomm;

  MPI_Comm_split(MPI_COMM_WORLD, get_my_global_rank() < i ? 0 : MPI_UNDEFINED, 
                 get_my_global_rank(), &newcomm); 
  return newcomm;
}

MPI_Comm func_comm2(int a, int b)
{
  MPI_Comm newcomm;

  MPI_Comm_split(MPI_COMM_WORLD, 
     get_my_global_rank() == a || get_my_global_rank() == b  ? 0 : MPI_UNDEFINED, 
     get_my_global_rank() == a ? 0 : 1, &newcomm); 
  return newcomm;
}

MPI_Comm func_comm_from_to(int first, int last)
{
  MPI_Comm newcomm;

  MPI_Comm_split(MPI_COMM_WORLD, 
     get_my_global_rank() >= first && get_my_global_rank() < last  ? 0 : MPI_UNDEFINED, 
     get_my_global_rank(), &newcomm); 
  return newcomm;
}

MPI_Comm func_comm4(int a, int b, int c, int d)
{
  MPI_Comm newcomm;
  int color, key;

  if( get_my_global_rank() == a ) {
    color = 0; key = 0;
  } else if( get_my_global_rank() == b ) {
    color = 0; key = 1;
  } else if( get_my_global_rank() == c ) {
    color = 0; key = 2;
  } else if( get_my_global_rank() == d ) {
    color = 0; key = 3;
  } else {
    color = MPI_UNDEFINED; key = 0;
  }

  MPI_Comm_split(MPI_COMM_WORLD, color, key, &newcomm); 
  return newcomm;
}

MPI_Comm func_copy_comm(MPI_Comm c)
{
  MPI_Comm newcomm;
  
  MPI_Comm_dup(c, &newcomm);
  return newcomm;
}

MPI_Comm func_comm_first_half(MPI_Comm c)
{
  MPI_Comm newcomm;
  int rank, size;

  MPI_Comm_size(c, &size);
  MPI_Comm_rank(c, &rank);
  MPI_Comm_split(c, rank < size/2, 0, &newcomm);
  return newcomm;
}

MPI_Comm func_comm_second_half(MPI_Comm c)
{
  MPI_Comm newcomm;
  int rank, size;

  MPI_Comm_size(c, &size);
  MPI_Comm_rank(c, &rank);
  MPI_Comm_split(c, rank >= size/2, 0, &newcomm);
  return newcomm;
}

MPI_Comm func_comm2_max_latency_with_root(void)
{
  MPI_Comm newcomm;
  double *all_ping_pong_min_times = 0;
  double max_time, t;
  int i, j, max_procs[2];

  update_measurement_comm(MPI_COMM_WORLD);
  determine_time_differences();

  if( grootproc() ) {
    all_ping_pong_min_times = skampi_malloc_doubles(get_global_size()*get_global_size());
  }

  MPI_Gather(ping_pong_min_time, get_global_size(), MPI_DOUBLE, 
	     all_ping_pong_min_times, get_global_size(), MPI_DOUBLE, 
	     0, MPI_COMM_WORLD);

  max_time = 0.0;
  max_procs[0] = 0; max_procs[1] = 1;
  if( grootproc() ) {
    for( i = 0; i < get_global_size(); i++)
      for( j = i+1; j < get_global_size(); j++) {
	t = all_ping_pong_min_times[i*get_global_size()+j] +
	    all_ping_pong_min_times[j*get_global_size()+i];
	if( t > max_time ) {
	  max_time = t;
	  max_procs[0] = i;
	  max_procs[1] = j;
	}
      }
    free(all_ping_pong_min_times);
  }
  MPI_Bcast(max_procs, 2, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Comm_split(MPI_COMM_WORLD, 
		 get_my_global_rank() == max_procs[0] || 
		 get_my_global_rank() == max_procs[1] ? 0 : MPI_UNDEFINED,
		 get_my_global_rank() == max_procs[0] ? 0 : 1, &newcomm);

  return newcomm;
}

/*---------------------------------------------------------------------------*/

struct item {
  int k;
  int v;
};

int cmp_k(const void *p1, const void *p2)
{
  int a, b; 
  a = ((struct item*)p1)->k;
  b = ((struct item*)p2)->k;
  
  return (a - b);
}

int cmp_v(const void *p1, const void *p2)
{
  int a, b; 
  a = ((struct item*)p1)->v;
  b = ((struct item*)p2)->v;
  
  return (a - b);
}

MPI_Comm func_comm_rand(int n)
{
  int i;
  double time;
  int size;
  MPI_Comm newcomm;
  struct item *pos;
  int key[2];

  if (get_my_global_rank() == 0) {

    time = MPI_Wtime();
    time = time - (int) time;
    srand( ((unsigned int) (time*100000.0)) % UINT_MAX );

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    pos = mpi_malloc( size * sizeof(struct item) );

    for (i = 0; i < size; ++i) {
      pos[i].k = (int) rand(); /* between 0 and RAND_MAX */
      pos[i].v = i;
    }
    qsort(pos, size, sizeof(struct item), cmp_k);
    for (i = n; i < size; ++i) {
      pos[i].k = -1;
    }
    qsort(pos, size, sizeof(struct item), cmp_v);

    MPI_Scatter(pos, 2, MPI_INT, &key, 2, MPI_INT, 0, MPI_COMM_WORLD);
    mpi_free(pos);
 
  } else {
    MPI_Scatter(NULL, 2, MPI_INT, &key, 2, MPI_INT, 0, MPI_COMM_WORLD);
  }

  assert(key[1] == get_my_global_rank());

  MPI_Comm_split(MPI_COMM_WORLD, (key[0] != -1) ? 0 : MPI_UNDEFINED, 
		 key[0], &newcomm); 
  return newcomm;
}

/*---------------------------------------------------------------------------*/

int func_is_mpi_comm_null(MPI_Comm comm)
{
  return (comm == MPI_COMM_NULL);
}

/*---------------------------------------------------------------------------*/

char * func_mpi_processor_names(void)
{
  char name[MPI_MAX_PROCESSOR_NAME];
  int rank, namelen;
  char * result_str;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(name, &namelen);

  result_str = skampi_malloc_chars(2+4+1+6+2+namelen+1);
  sprintf(result_str, "# rank %6d: ", rank);
  strncpy(result_str+2+4+1+6+2, name, namelen);
  result_str[2+4+1+6+2+namelen] = '\0';
  return result_str;
}

/*---------------------------------------------------------------------------*/

MPI_Comm func_topology_cartesian(MPI_Comm c, IArray dims, IArray periodic, int reorder)
{
  MPI_Comm newcomm;
  
  assert(dims.n == periodic.n);
  MPI_Cart_create(c, dims.n, dims.v, periodic.v, reorder, &newcomm);
  return newcomm;
}

IArray func_dims_create(int nnodes, IArray dims)
{
  IArray res;

  res.n = dims.n;
  res.v = skampi_malloc_ints(res.n);
  memcpy(res.v, dims.v, res.n * sizeof(int));
  MPI_Dims_create(nnodes, res.n, res.v);
  
  return res;
}

MPI_Comm func_topology_graph(MPI_Comm c, IArray index, IArray edges, int reorder)
{
  MPI_Comm newcomm;
  
  MPI_Graph_create(c, index.n, index.v, edges.v, reorder, &newcomm);
  return newcomm;
}

MPI_Comm func_topology_graph2(MPI_Comm c, IArray graph, int reorder)
{
  MPI_Comm newcomm;
  int nnodes;
  int* edges;
  int* index;
  int i, j, e, size;
  
  assert(graph.n % 2 == 0);
  
  nnodes = 0;
  for (i=0; i < graph.n; i++)
    if (graph.v[i] > nnodes)
      nnodes = graph.v[i];
  nnodes++; /* number of nodes = largest node index + 1 */
  MPI_Comm_size(c, &size);
  assert(nnodes <= size);
  
  index = skampi_malloc_ints(nnodes);
  edges = skampi_malloc_ints(graph.n);
  
  e = 0;
  for (i=0; i<nnodes; i++) {
    for (j=0; j < graph.n; j += 2) {
      if (graph.v[j] == i) {
        edges[e++] = graph.v[j+1];
      } else if (graph.v[j+1] == i) {
        edges[e++] = graph.v[j];
      }
    }
    index[i] = e;
  }
  
  MPI_Graph_create(c, nnodes, index, edges, reorder, &newcomm);
    
  return newcomm;
}

/*---------------------------------------------------------------------------*/

IArray func_alloc_int_array(int size)
{
  IArray a;
  
  a.n = size;
  a.v = calloc(size, sizeof(int));
    
  return a;
}

void func_int_array_set(IArray iarr, int i, int v)
{
  iarr.v[i] = v;
}


int func_int_array_get(IArray iarr, int i)
{
  return iarr.v[i];
}


DArray func_alloc_datatype_array(int size)
{
  DArray a;
  
  a.n = size;
  a.v = calloc(size, sizeof(MPI_Datatype));
    
  return a;
}

void func_datatype_array_set(DArray darr, int i, MPI_Datatype v)
{
  darr.v[i] = v;
}

MPI_Datatype func_datatype_array_get(DArray darr, int i)

{
  return darr.v[i];
}

/*---------------------------------------------------------------------------*/

#ifdef USE_MPI_INFO

MPI_Info func_info_create(void)
{
  MPI_Info newinfo;
  MPI_Info_create(&newinfo); 

  return newinfo;
}

void func_info_free(MPI_Info info)
{
  MPI_Info_free(&info);
}

void func_info_set(MPI_Info info, char* key, char* value)
{
  int len, flag;
  MPI_Info_set(info, key, value);
  MPI_Info_get_valuelen(info, key, &len, &flag);
}

#endif

/*---------------------------------------------------------------------------*/

void func_use_mpi_alloc_mem(int flag) {
#ifndef USE_MPI_ALLOC_MEM
  if (flag) {
    print_output("# Note: It is not possible to use MPI_ALLOC_MEM. Fallback to malloc().\n\n");
  } 
#endif

  set_mpi_alloc_mem_use(flag);
}

#ifdef USE_MPI_INFO

void func_set_mpi_alloc_mem_info(MPI_Info info)
{
  set_mpi_alloc_mem_info(info);
}

MPI_Info func_get_mpi_alloc_mem_info(void)
{
  return get_mpi_alloc_mem_info();
}

#endif

/*---------------------------------------------------------------------------*/

int func_get_random_int(int min_value, int max_value)
{
  double time;
  
  assert(max_value >= min_value);

  time = MPI_Wtime();
  time = time - (int) time;
  
  srand( ((unsigned int) (time*(100000.0 + get_my_global_rank()*237.0))) % 
	 UINT_MAX );
  
  return ((int) (min_value + (max_value - min_value+1) * 
		 ((double) rand() / ((double) RAND_MAX + 1.0))));  
}

/*---------------------------------------------------------------------------*/


char *func_get_env(char *name)
{
  int len;
  char *env;

  len = strlen(getenv(name));
  env = mpi_malloc_chars(len+1);

  sprintf(env, "%s/", getenv(name));

  return env;
}

/*---------------------------------------------------------------------------*/

#pragma weak end_skampi_extensions
