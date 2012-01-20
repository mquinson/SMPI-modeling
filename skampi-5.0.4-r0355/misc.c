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

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mpiversiontest.h"
#include "misc.h"
#include "private_misc.h"

#include "output.h"
#include "string.h"
#include "errno.h"
/*---------------------------------------------------------------------------*/

static int measurement_rank;
static int measurement_size;
static MPI_Comm measurement_comm;

static int global_rank;
static int global_size;
static int *global_ranks;

void update_measurement_comm(MPI_Comm comm) 
{
  measurement_comm = comm;
  MPI_Comm_rank(measurement_comm, &measurement_rank);
  MPI_Comm_size(measurement_comm, &measurement_size);
  
  MPI_Allgather(&global_rank, 1, MPI_INT, 
		global_ranks, 1, MPI_INT, measurement_comm);

}

int get_measurement_rank(void)
{
  return measurement_rank;
}

int get_measurement_size(void)
{
  return measurement_size;
}

MPI_Comm get_measurement_comm(void)
{
  return measurement_comm;
}

/*---------------------------------------------*/

void init_globals(void)
{
  MPI_Comm_rank(MPI_COMM_WORLD, &global_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &global_size);
  global_ranks = skampi_malloc_ints(global_size);
}

int get_my_global_rank(void)
{
  return global_rank;
}

int get_global_rank(int m_rank)
{
  return global_ranks[m_rank];
}

int get_global_size(void)
{
  return global_size;
}

/*---------------------------------------------------------------------------*/

static char *send_buffer;
static char *recv_buffer;

static MPI_Aint reported_message_size;

void set_send_buffer(char *buffer)
{
  send_buffer = buffer;
}

char *get_send_buffer(void)
{
  return send_buffer;
}

void set_recv_buffer(char *buffer)
{
  recv_buffer = buffer;
}

char *get_recv_buffer(void)
{
  return recv_buffer;
}

void set_reported_message_size(MPI_Aint n)
{
  reported_message_size = n;
}

MPI_Aint get_reported_message_size(void)
{
  return reported_message_size;
}

/*---------------------------------------------------------------------------*/

static char *progname;

void set_progname(char *s)
{
  /* @@@ remove path to executable? */
  progname = s;
}

char *get_progname(void)
{
  return progname;
}

/*---------------------------------------------------------------------------*/

int imax2(int a, int b)
{
  if( a > b) return a;
  else       return b;
}

int imin2(int a, int b)
{
  if( a < b) return a;
  else       return b;
}

int imax3(int a, int b, int c)
{
  if( a > b ) {
    if( c > a ) return c;
    else        return a;
  } else {
    if( c > b) return c;
    else       return b;
  }
}   

double fmin2(double a, double b)
{
  if( a < b) return a;
  else       return b;
}

double fmax2(double a, double b)
{
  if( a > b) return a;
  else       return b;
}
  
int isqr(int a)
{
  return a*a;
}

double fsqr(double a)
{
  return a*a;
}

int gcd(int a, int b)
{
  if( b == 0 ) return a;
  return gcd(b, a % b);
}

int lcm(int a, int b)
{
  return a*b / gcd(a,b);
}

int round_int_to_multipleof(int i, int m)
{
  int d;

  d = i / m;
  if( i >= 0 ) {
    if( i - d*m <= (d+1)*m - i ) return d*m;
    else                         return (d+1)*m;
  } else {
    if( i - (d-1)*m < d*m - i ) return (d-1)*m;
    else                        return  d*m;
  }
}

int round_double_to_multipleof(double f, int m)
{
  int d;

  d = (int)(f / m);

  if( f >= 0 ) {
    if( f - d*m <= (d+1)*m - f ) return d*m;
    else                         return (d+1)*m;
  } else {
    if( f - (d-1)*m < d*m - f ) return (d-1)*m;
    else                        return d*m;
  }
}

/*---------------------------------------------------------------------------*/

bool grootproc(void) { return get_my_global_rank() == 0; }
bool lrootproc(void) { return get_measurement_rank() == 0; }

double wtime(void)
{
  /*         return MPI_Wtime() + get_my_global_rank()*1.0; */
           return MPI_Wtime();
}


/*---------------------------------------------------------------------------*/

static int use_mpi_memory_allocation = False;

void set_mpi_alloc_mem_use(int flag) {
  use_mpi_memory_allocation = flag;
}

int get_mpi_alloc_mem_use(void) {
  return use_mpi_memory_allocation;
}


#ifdef USE_MPI_INFO
static MPI_Info mpi_malloc_info = MPI_INFO_NULL;

void set_mpi_alloc_mem_info(MPI_Info info)
{
  mpi_malloc_info = info;
}

MPI_Info get_mpi_alloc_mem_info(void)
{
  return mpi_malloc_info;
}
#endif

void *_skampi_malloc(int size, const char *_file, unsigned int _line)
{
  void *baseptr = NULL;

  if ( size < 0 ) {   
    error_with_abort( MPI_ERR_ARG, 
		      "\nskampi_malloc: Invalid value for size, must be non-negative but is %d"
		      "\nskampi_malloc: Caller in file %s at line %u\n", 
		      (int) size, _file, _line );
  }
  baseptr = malloc(size);
  if (baseptr == NULL) {
    error_with_abort( MPI_ERR_ARG,
                      "\nskampi_malloc: malloc(size=%d) failed."
                      "\nskampi_malloc: %s (%d)"
                      "\nskampi_malloc: Caller in file %s at line %u\n",  
                      (size_t) size, strerror(errno), errno, _file, _line );
  }

  return baseptr;
}

void *_mpi_malloc(MPI_Aint size, const char *_file, unsigned int _line)
{
  void *baseptr = NULL;

#ifdef USE_MPI_ALLOC_MEM
  int  errcode;
  int  errclass;
  char errstring[MPI_MAX_ERROR_STRING];
  int  resultlen;
#endif

  if ( size < 0 ) {   
    error_with_abort( MPI_ERR_ARG, 
		      "\nmpi_malloc: Invalid value for size, must be non-negative but is %d"
		      "\nmpi_malloc: Caller in file %s at line %u\n", 
		      (int) size, _file, _line );
  }

#ifdef USE_MPI_ALLOC_MEM
  if (use_mpi_memory_allocation) {
    errcode = MPI_Alloc_mem( size, get_mpi_alloc_mem_info(), &baseptr );
    
    /* MPI 1 (p. 195): "After an error is detected, the state of MPI is 
       undefined!". Therefore abort, if the MPI implementation attempts 
       to continue. */
    if ( errcode != MPI_SUCCESS ) {
      MPI_Error_string( errcode, errstring, &resultlen );
      MPI_Error_class( errcode, &errclass );    
      error_with_abort( errcode,
			"\nmpi_malloc: MPI_Alloc_mem(size=%d, ..) failed."
			"\nmpi_malloc: %s (%d)"
			"\nmpi_malloc: Caller in file %s at line %u\n",  
			size, errstring, errclass, _file, _line );
    }
  } else {
#endif
    baseptr = malloc(size);
    if (baseptr == NULL) {
      error_with_abort( MPI_ERR_ARG,
			"\nmpi_malloc: malloc(size=%d) failed."
			"\nmpi_malloc: %s (%d)"
			"\nmpi_malloc: Caller in file %s at line %u\n",  
			(size_t) size, strerror(errno), errno, _file, _line );
    }
#ifdef USE_MPI_ALLOC_MEM
  }
#endif

  return baseptr;
}


double *_skampi_malloc_doubles(int n, const char *_file, unsigned int _line)
{
  return (double*)_skampi_malloc(n*sizeof(double), _file, _line);
}

double *_mpi_malloc_doubles(int n, const char *_file, unsigned int _line)
{
  return (double*)_mpi_malloc(n*sizeof(double), _file, _line);
}

int *_skampi_malloc_ints(int n, const char *_file, unsigned int _line)
{
  return (int*)_skampi_malloc(n*sizeof(int), _file, _line);
}

int *_mpi_malloc_ints(int n, const char *_file, unsigned int _line)
{
  return (int*)_mpi_malloc(n*sizeof(int), _file, _line);
}

char *_skampi_malloc_chars(int n, const char *_file, unsigned int _line)
{
  return (char*)_skampi_malloc(n*sizeof(char), _file, _line);
}

char *_mpi_malloc_chars(int n, const char *_file, unsigned int _line)
{
  return (char*)_mpi_malloc(n*sizeof(char), _file, _line);
}

char **_skampi_malloc_charps(int n, const char *_file, unsigned int _line)
{
  return (char**)_skampi_malloc(n*sizeof(char*), _file, _line);
}

char **_mpi_malloc_charps(int n, const char *_file, unsigned int _line)
{
  return (char**)_mpi_malloc(n*sizeof(char*), _file, _line);
}

MPI_Request *_skampi_malloc_reqs(int n, const char *_file, unsigned int _line)
{
  return (MPI_Request*)_skampi_malloc(n*sizeof(MPI_Request), _file, _line);
}

MPI_Request *_mpi_malloc_reqs(int n, const char *_file, unsigned int _line)
{
  return (MPI_Request*)_mpi_malloc(n*sizeof(MPI_Request), _file, _line);
}

MPI_Offset *_skampi_malloc_offsets(int n, const char *_file, unsigned int _line)
{
  return (MPI_Offset*)_skampi_malloc(n*sizeof(MPI_Offset), _file, _line);
}

MPI_Offset *_mpi_malloc_offsets(int n, const char *_file, unsigned int _line)
{
  return (MPI_Offset*)_mpi_malloc(n*sizeof(MPI_Offset), _file, _line);
}

MPI_Status *_skampi_malloc_statuses(int n, const char *_file, unsigned int _line)
{
  return (MPI_Status*)_skampi_malloc(n*sizeof(MPI_Status), _file, _line);
}

MPI_Status *_mpi_malloc_statuses(int n, const char *_file, unsigned int _line)
{
  return (MPI_Status*)_mpi_malloc(n*sizeof(MPI_Status), _file, _line);
}

void mpi_free(void *base)
{
#ifdef USE_MPI_ALLOC_MEM
  MPI_Free_mem(base);
#else
  free(base);
#endif
}
