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
#include <string.h>
#include <assert.h>
#include <mpi.h>

#include "mpiversiontest.h"

#include "misc.h"
#include "output.h"
#include "private_misc.h"
#include "debug.h"
#include "mem.h"

char *skampi_buffer = NULL;                /* aligned buffer */
static char *real_skampi_buffer = NULL;           /* start of allocated (possibly not aligned) buffer */
static int  skampi_buffer_allocation_method = 0;

static MPI_Aint send_buffer_usage;
static MPI_Aint recv_buffer_usage;

int send_buffer_alignment = 32;
int recv_buffer_alignment = 32;
static int skampi_buffer_alignment = 32; /* = lcm(send_buffer_alignment, 
				                  recv_buffer_alignment) */

static char *fresh_ptr = NULL;

int skampi_buffer_size = 0;       /* in bytes */
static int skampi_real_buffer_size = 0;

int skampi_cache_size = 0;        /* in bytes */

int  buffer_cycling = True; 


static char *send_buffer_slice = NULL;
static char *recv_buffer_slice = NULL;

static MPI_Aint send_buffer_slice_usage = 0; /* in bytes */
static MPI_Aint recv_buffer_slice_usage = 0; /* in bytes */

/* -------------------------------------------------------------------------- */

#if 0
/* currently the following is  not used everywhere */
static int increase_to_multiple_of(MPI_Aint x, int a) {
  if( x % a == 0 ) return x;
  else             return x + a - (x % a);
}
#endif

static int alignment_fix(char *p, int a)
{
  MPI_Aint absolute_address;
  unsigned long address;
  int fix;

#if MPI_VERSION < 2
  MPI_Address(p, &absolute_address);
#else
  MPI_Get_address(p, &absolute_address);
#endif
  /* actually, the MPI standard doesn't guarantee to return an absolute 
     address, but an address relative to MPI_BOTTOM, but we hope that that
     is useful enough for alignment purposes, portable suggestions for 
     improvement are welcome */

  /* for the following arithmetic operations the value of the `absolut_address' 
     variable must be converted to the value of a variable of type `unsigned long'. 
     The reason is that in some MPI implementations the type `MPI_Aint' is in 
     fact of type `signed int'. If the "absolut" address returned by MPI is to big 
     (e.g. 0xa7b8540c) the arithmetic operations below are working with negative
     values (e.g. -1481092084), that triggers a faulty result. */

  address = (unsigned long) absolute_address;

  if (a<=0) {
    fix = 0;
  } else {
    /* if( address % a == 0 ) fix = 0; else fix = a - (address % a); */
    fix = (a - (address % a)) % a;

    /* alignment should be for a not for 2*a */ 
    if( (address+fix) % (2*a) == 0 )
      fix = fix+a;
  }

  debug(DBG_BUFFER, "alignment_fix(p=0x%x=%d, a=0x%x=%d) = 0x%x=%d\n", (int) p, (int) p, a, a, fix, fix);
  return fix;
}

static char *align_ptr(char *p, int a)
{
  return p + alignment_fix(p, a);
}

MPI_Aint get_extent(int count, MPI_Datatype datatype)
{
  MPI_Aint extent;
#if MPI_VERSION >= 2
  MPI_Aint lb;

  MPI_Type_get_extent(datatype, &lb, &extent);
#else
  MPI_Type_extent(datatype, &extent); /* deprecated */
#endif
  return count*extent;   /* calculation is probably too simple?! */
                         /* what about holes, offsets?! @@@ */
}

/* if (MPI_VERSION >= 1) && (MPI_SUBVERSION >= 2)  */
#ifdef USE_MPI_IO
MPI_Aint get_true_extent(int count, MPI_Datatype datatype)
{
  MPI_Aint true_extent;
  MPI_Aint true_lb;

  MPI_Type_get_true_extent(datatype, &true_lb, &true_extent);
  
  return count*true_extent;
}
#endif

static void discard_cache(void)
{
  debug(DBG_BUFFER, "discard_cache\n");
  /*  memset(skampi_buffer, 0, imin2(skampi_buffer_size, skampi_cache_size*2)); */
  /* @@@ works only for sequential usage of the buffer i.e.  memory at
     a lower adress will be accessed before memory with higher one */
}

void init_send_buffer_slice()
{
  debug(DBG_BUFFER, "init_send_buffer_slice()\n");
  send_buffer_slice = NULL;
}

void init_recv_buffer_slice()
{
  debug(DBG_BUFFER, "init_recv_buffer_slice()\n");
  recv_buffer_slice = NULL;
}

void init_buffer(void)
{
  fresh_ptr = skampi_buffer;
  debug(DBG_BUFFER, "init_buffer: fresh_ptr = 0x%x = %d\n", fresh_ptr, fresh_ptr);
  discard_cache();
}

void check_buffer(void)
{
  char *tmp_ptr;

  debug(DBG_BUFFER, "check_buffer()\n");

  if( fresh_ptr - skampi_buffer > skampi_real_buffer_size - send_buffer_alignment ) {
    debug(DBG_BUFFER, "cycle_buffer overflow, starting again\n");
    init_buffer();
  } else {
    tmp_ptr = align_ptr(fresh_ptr, send_buffer_alignment);
    if( tmp_ptr - skampi_buffer > skampi_real_buffer_size - get_send_buffer_usage() - recv_buffer_alignment) {
      debug(DBG_BUFFER, "cycle_buffer overflow, starting again\n");
      init_buffer();
    } else {
      tmp_ptr = align_ptr(tmp_ptr+get_send_buffer_usage(), recv_buffer_alignment );
      if( tmp_ptr - skampi_buffer > skampi_real_buffer_size - get_recv_buffer_usage() ) {
	debug(DBG_BUFFER, "cycle_buffer overflow, starting again\n");
	init_buffer();
      }
    }
  }
  tmp_ptr = align_ptr(fresh_ptr, send_buffer_alignment);
  set_send_buffer(tmp_ptr);
  init_send_buffer_slice();
  debug(DBG_BUFFER, "send_buffer = 0x%x = %d\n", get_send_buffer(), get_send_buffer());
  set_recv_buffer(align_ptr(tmp_ptr+get_send_buffer_usage(), recv_buffer_alignment)); 
  init_recv_buffer_slice();
  debug(DBG_BUFFER, "recv_buffer = 0x%x = %d\n", get_recv_buffer(), get_recv_buffer());
  if( buffer_cycling ) 
    fresh_ptr = (char*)get_recv_buffer() + get_recv_buffer_usage();
  debug(DBG_BUFFER, "fresh_ptr = 0x%x = %d\n", fresh_ptr, fresh_ptr);
}


void set_skampi_buffer(int size)
{
  int alignments;

  if( skampi_buffer != NULL ) {
    if (skampi_buffer_allocation_method == 0)
      free(real_skampi_buffer);
#ifdef USE_ONESIDED
    else if (skampi_buffer_allocation_method == 1)
      MPI_Free_mem(real_skampi_buffer);
#endif
  }

  alignments = 8*skampi_buffer_alignment;
  /* factor 2  for send and recv  
     factor 2  see function definition alignment_fix 
     factor 2  when using buffer cycling, there should be enough buffer memory 
               for two different copies */
  debug(DBG_BUFFER, "set_skampi_buffer(size = 0x%x = %d)\n", size, size);
  debug(DBG_BUFFER, "  skampi_buffer_alignment = 0x%x = %d\n", skampi_buffer_alignment, skampi_buffer_alignment);
  debug(DBG_BUFFER, "  send_buffer_alignment = 0x%x = %d\n", send_buffer_alignment, send_buffer_alignment);
  debug(DBG_BUFFER, "  recv_buffer_alignment = 0x%x = %d\n", recv_buffer_alignment, recv_buffer_alignment);
  debug(DBG_BUFFER, "  size + alignments = 0x%x = %d\n", size + alignments, size + alignments);
  real_skampi_buffer = (char*) malloc(size + alignments);
  assert( real_skampi_buffer != NULL );
  skampi_buffer_allocation_method = 0; /* use free() */
  skampi_buffer_size = size;
  skampi_real_buffer_size = size + alignments;
  skampi_buffer = align_ptr(real_skampi_buffer, skampi_buffer_alignment);
  debug(DBG_BUFFER, "  skampi_buffer = 0x%x = %d\n", skampi_buffer, skampi_buffer);
  memset(real_skampi_buffer, 0x55, skampi_real_buffer_size); 
  /* touch buffer so that we can be sure to actually 'have' it in memory */
  init_buffer();
}

#ifdef USE_ONESIDED
void set_skampi_buffer_mpi_alloc_mem(int size, MPI_Info info)
{
  int errcode, alignments;

  if( skampi_buffer != NULL ) {
    if (skampi_buffer_allocation_method == 0)
      free(skampi_buffer);
    else if (skampi_buffer_allocation_method == 1)
      MPI_Free_mem(skampi_buffer);
  }

  alignments = 8*skampi_buffer_alignment;
  errcode = MPI_Alloc_mem(size + alignments, info, &real_skampi_buffer);
  assert( real_skampi_buffer != NULL );
  assert( errcode == MPI_SUCCESS );
  skampi_buffer_allocation_method = 1; /* dont use free() */
  skampi_buffer_size = size;
  skampi_real_buffer_size = size + alignments;
  skampi_buffer = align_ptr(real_skampi_buffer, skampi_buffer_alignment);
  memset(real_skampi_buffer, 0x55, skampi_real_buffer_size); 
  /* touch buffer so that we can be sure to actually 'have' it in memory */
  init_buffer();
}
#endif

void switch_buffer_cycling_on(void)
{
  buffer_cycling = True;
}

void switch_buffer_cycling_off(void)
{
  buffer_cycling = False;
}

void set_send_buffer_alignment(int a)
{
  int new_alignment;

  if( a < 1 ) a = 1;
  send_buffer_alignment = a;
  new_alignment = lcm(send_buffer_alignment, recv_buffer_alignment);
  if(  new_alignment != skampi_buffer_alignment ) {
    skampi_buffer_alignment = new_alignment;
    set_skampi_buffer(skampi_buffer_size);
  }
}

void set_recv_buffer_alignment(int a)
{
  int new_alignment;

  if( a < 1 ) a = 1;
  recv_buffer_alignment = a;
  new_alignment = lcm(send_buffer_alignment, recv_buffer_alignment);
  if(  new_alignment != skampi_buffer_alignment ) {
    skampi_buffer_alignment = new_alignment;
    set_skampi_buffer(skampi_buffer_size);
  }
}

MPI_Aint get_send_buffer_usage(void)
{
  return send_buffer_usage;
}

void set_send_buffer_usage(MPI_Aint n)
{
  debug(DBG_BUFFER, "set_send_buffer_usage(0x%x = %d)\n", n, n);
  send_buffer_usage = n;
}

MPI_Aint get_recv_buffer_usage(void)
{
  return recv_buffer_usage;
}

void set_recv_buffer_usage(MPI_Aint n)
{
  debug(DBG_BUFFER, "set_recv_buffer_usage(0x%x = %d)\n", n, n);
  recv_buffer_usage = n;
}

bool is_buffer_too_small()
{
  if( buffer_cycling ) 
    return skampi_real_buffer_size < 
      2*(send_buffer_usage + 2*send_buffer_alignment + 
	 recv_buffer_usage + 2*recv_buffer_alignment);
  else
    return skampi_real_buffer_size <
      send_buffer_usage + 2*send_buffer_alignment + 
      recv_buffer_usage + 2*recv_buffer_alignment;
}

/* -------------------------------------------------------------------------- */

void set_send_buffer_slice_usage(MPI_Aint n) /* in bytes */
{
  debug(DBG_BUFFER, "set_send_buffer_slice_usage(0x%x = %d)\n", n, n);
  assert(n >= 0);  
  send_buffer_slice_usage = n;
}

void set_recv_buffer_slice_usage(MPI_Aint n) /* in bytes */
{
  debug(DBG_BUFFER, "set_recv_buffer_slice_usage(0x%x = %d)\n", n, n);
  assert(n >= 0);  
  recv_buffer_slice_usage = n;
}

MPI_Aint get_send_buffer_slice_usage(void)
{
  return send_buffer_slice_usage;
}

MPI_Aint get_recv_buffer_slice_usage(void)
{
  return recv_buffer_slice_usage;
}

/*
 * get_send_buffer_slice
 * 
 * Returns everytime the function is called a new slice of the previous 
 * reserved send buffer. If an allocation of a new slice fails, it returns 
 * NULL and resets to the first slice, that will be returned in the next call.
 * 
 * Alignments are considered.
 *
 * @return
 * char*            pointer to allocated slice, or
 *                  NULL, if allocation fails 
 */
char *get_send_buffer_slice(void)
{
  char *tmp_ptr; 

  if (send_buffer_slice_usage <= 0) {
    error_with_abort(4711, "illegal invocation of get_send_buffer_slice(), send_buffer_slice_usage is %d.", send_buffer_slice_usage);
  }

  debug(DBG_BUFFER, "get_send_buffer_slice()\n");
  if (send_buffer_slice == NULL) {
    if (send_buffer_slice_usage <= get_send_buffer_usage()) {
      send_buffer_slice = get_send_buffer(); /* already aligned */
    }
  } else {
    tmp_ptr = align_ptr(send_buffer_slice + send_buffer_slice_usage, send_buffer_alignment);
    if (tmp_ptr - get_send_buffer() <= get_send_buffer_usage() - send_buffer_slice_usage) {
      send_buffer_slice = tmp_ptr;
    } else {
      send_buffer_slice = NULL;
    }
  }
  debug(DBG_BUFFER, "  send_buffer_slice = 0x%x = %d\n", send_buffer_slice, send_buffer_slice);
  return send_buffer_slice;
}

/*
 * get_recv_buffer_slice
 * 
 * Returns everytime the function is called a new slice of the previous 
 * reserved receive buffer. If an allocation of a new slice fails, it returns 
 * NULL and resets to the first slice, that will be returned in the next call.
 * 
 * Alignments are considered.
 * 
 * @return
 * char*            pointer to allocated slice, or
 *                  NULL, if allocation fails 
 */
char *get_recv_buffer_slice(void)
{
  char *tmp_ptr; 

  if (recv_buffer_slice_usage <= 0) {
    error_with_abort(4711, "illegal invocation of get_recv_buffer_slice(), recv_buffer_slice_usage is %d.", recv_buffer_slice_usage);
  }

  debug(DBG_BUFFER, "get_recv_buffer_slice()\n");
  if (recv_buffer_slice == NULL) {
    if (recv_buffer_slice_usage <= get_recv_buffer_usage()) {
      recv_buffer_slice = get_recv_buffer(); /* already aligned */
    }
  } else {
    tmp_ptr = align_ptr(recv_buffer_slice + recv_buffer_slice_usage, recv_buffer_alignment);
    if (tmp_ptr - get_recv_buffer() <= get_recv_buffer_usage() - recv_buffer_slice_usage) {
      recv_buffer_slice = tmp_ptr;
    } else {
      recv_buffer_slice = NULL;
    }
  }
  debug(DBG_BUFFER, "  recv_buffer_slice = 0x%x = %d\n", recv_buffer_slice, recv_buffer_slice);
  return recv_buffer_slice;
}

/*
 * get_send_buffer_min_size
 * 
 * Determines the minimum size of the send buffer
 * needed for `n' slices of `slice_size'.
 *
 * @parameter
 * slice_size       size of slice (in bytes)
 * n                number of slices
 *
 * @return
 * MPI_Aint         minimun size of send buffer
 */
MPI_Aint get_send_buffer_min_size(MPI_Aint slice_size, int n)
{
  int fix;
  int a;

  assert(n >= 0);
  a = send_buffer_alignment;

  /* see function alignment_fix */
  fix = (a - (slice_size % a)) % a;
  if ( ( a+slice_size+fix ) % (2*a) == 0 ) {
    fix += a;
  }

  debug(DBG_BUFFER, "send_buffer_minimum_size = 0x%x = %d\n", (slice_size+fix)*n, (slice_size+fix)*n);
  debug(DBG_BUFFER, "  (slice_size+fix)*n = (%d+%d)*%d\n", slice_size, fix, n);
  return ((MPI_Aint) ((slice_size+fix)*n));
}

/*
 * get_recv_buffer_min_size
 * 
 * Determines the minimum size of the receive buffer
 * needed for `n' slices of `slice_size'.
 *
 * @parameter
 * slice_size       size of slice (in bytes)
 * n                number of slices
 *
 * @return
 * MPI_Aint         minimun size of receive buffer
 */
MPI_Aint get_recv_buffer_min_size(MPI_Aint slice_size, int n)
{
  int fix;
  int a;

  assert(n >= 0);
  a = recv_buffer_alignment;

  /* see function alignment_fix */
  fix = (a - (slice_size % a)) % a;
  if ( ( a+slice_size+fix ) % (2*a) == 0 ) {
    fix += a;
  }

  debug(DBG_BUFFER, "recv_buffer_minimum_size = 0x%x = %d\n", (slice_size+fix)*n, (slice_size+fix)*n);
  debug(DBG_BUFFER, "  (slice_size+fix)*n = (%d+%d)*%d\n", slice_size, fix, n);
  return ((MPI_Aint) ((slice_size+fix)*n));
}


