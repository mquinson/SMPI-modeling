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

#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <assert.h>

#include <unistd.h>

#include "../misc.h"
#include "../synchronize.h"

#pragma weak begin_skampi_extensions

static MPI_Request *req_array;
static MPI_Status *status_array;
static MPI_Aint send_dt_extent, recv_dt_extent;

static int *recv_counts;
static int *send_counts;
static int *recv_displs;
static int *send_displs;

static int *initialize_counts(int count)
{
  int i;
  int *counts;

  counts = (int*)malloc(get_measurement_size()*sizeof(int));
  assert( counts != NULL );

  for( i = 0; i < get_measurement_size(); i++)
    counts[i] = count / get_measurement_size();

  for( i = 0; i < count % get_measurement_size(); i++) 
    counts[i*get_measurement_size() / (count % get_measurement_size())]++;

  return counts;
}

static int *initialize_displs(int *counts)
{
  int i;
  int *displs;

  displs = (int*)malloc(get_measurement_size()*sizeof(int));
  assert( displs != NULL );

  displs[0] = 0;
  for( i = 1; i < get_measurement_size(); i++ )
    displs[i] = displs[i-1] + counts[i-1];

  return displs;
}

static void initialize_req_array(int n)
{
  req_array = (MPI_Request*)malloc(n*sizeof(MPI_Request));
  assert( req_array != NULL );
}

static void initialize_status_array(int n)
{
  status_array = (MPI_Status*)malloc(n*sizeof(MPI_Status));
  assert( status_array != NULL );
}

/*---------------------------------------------------------------------------*/

void init_Bcast(int count, MPI_Datatype dt, int root)
{
  set_send_buffer_usage(get_extent(count, dt));
  set_reported_message_size(get_extent(count, dt));
  set_recv_buffer_usage(0);
  init_synchronization();
}

double measure_Bcast(int count, MPI_Datatype dt, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Bcast(get_send_buffer(), count, dt, root, get_measurement_comm());
  end_time = stop_synchronization();
  return  end_time - start_time;
}

/*---------------------------------------------------------------------------*/

static MPI_Comm mycomm;
static int myrank;

void init_Bcast_using_Send_Recv(int count, MPI_Datatype dt, int root)
{
  set_send_buffer_usage(get_extent(count, dt));
  set_reported_message_size(get_extent(count, dt));
  set_recv_buffer_usage(0);
  MPI_Comm_split(get_measurement_comm(), 0, (get_measurement_rank()+get_measurement_size()-root) % get_measurement_size(), 
		 &mycomm);
  MPI_Comm_rank(mycomm, &myrank);
  init_synchronization();
}

double measure_Bcast_using_Send_Recv(int count, MPI_Datatype dt, int root)
{
  MPI_Status status;
  double start_time, end_time;

  start_time = start_synchronization();
  if( myrank != 0 ) 
    MPI_Recv(get_send_buffer(), count, dt, (myrank-1)/2, 0, mycomm, &status);

  if( 2*myrank+1 < get_measurement_size() ) 
    MPI_Send(get_send_buffer(), count, dt, 2*myrank+1, 0, mycomm);
  if( 2*myrank+2 < get_measurement_size() )
    MPI_Send(get_send_buffer(), count, dt, 2*myrank+2, 0, mycomm);
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Bcast_using_Send_Recv(int count, MPI_Datatype dt, int root) 
{
  MPI_Comm_free(&mycomm);
}

/*---------------------------------------------------------------------------*/

void init_Barrier(void)
{
  init_synchronization();
}

double measure_Barrier(void)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Barrier(get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Reduce(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  MPI_Aint extent;

  extent = get_extent(count, dt);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
  init_synchronization();
}

double measure_Reduce(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Reduce(get_send_buffer(), get_recv_buffer(), count, dt, op, root, 
	     get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Allreduce(int count, MPI_Datatype dt, MPI_Op op)
{
  MPI_Aint extent;

  extent = get_extent(count, dt);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
  init_synchronization();
}

double measure_Allreduce(int count, MPI_Datatype dt, MPI_Op op)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Allreduce(get_send_buffer(), get_recv_buffer(), count, dt, op,
		get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Allreduce_using_Reduce_Bcast(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  MPI_Aint extent;

  extent = get_extent(count, dt);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
  init_synchronization();
}

double measure_Allreduce_using_Reduce_Bcast(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Reduce(get_send_buffer(), get_recv_buffer(), count, dt, op, root, 
	     get_measurement_comm());
  MPI_Bcast(get_recv_buffer(), count, dt, root, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Reduce_scatter(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  recv_counts = initialize_counts(count);
  set_send_buffer_usage(get_extent(count, dt));
  set_reported_message_size(get_extent(count, dt));
  set_recv_buffer_usage(get_extent(recv_counts[get_measurement_rank()], dt));
  init_synchronization();
}

double measure_Reduce_scatter(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Reduce_scatter(get_send_buffer(), get_recv_buffer(), recv_counts, 
		     dt, op, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Reduce_scatter(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  free(recv_counts);
}

/*---------------------------------------------------------------------------*/

void init_Alltoall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  set_send_buffer_usage(get_measurement_size()*get_extent(send_count, send_dt));
  set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  init_synchronization();
}

double measure_Alltoall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Alltoall(get_send_buffer(), send_count, send_dt,
	       get_recv_buffer(), recv_count, recv_dt, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Alltoall_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  initialize_req_array(2*get_measurement_size());
  initialize_status_array(2*get_measurement_size());

  send_dt_extent = get_extent(1, send_dt);
  recv_dt_extent = get_extent(1, recv_dt);

  set_send_buffer_usage(get_measurement_size()*get_extent(send_count, send_dt));
  set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  init_synchronization();
}

double measure_Alltoall_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  double start_time, end_time;
  int i;

  start_time = start_synchronization();
  for( i = 0; i < get_measurement_size(); i++)
    MPI_Irecv((char*)get_recv_buffer() + i*recv_dt_extent, recv_count, recv_dt, i, 
	      0, get_measurement_comm(), &(req_array[i]));

  for( i = 0; i < get_measurement_size(); i++)
    MPI_Isend((char*)get_send_buffer() + i*send_dt_extent, send_count, send_dt, i, 
	      0, get_measurement_comm(), &(req_array[i+get_measurement_size()]));

  MPI_Waitall(2*get_measurement_size(), req_array, status_array);
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Alltoall_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  free(req_array);
  free(status_array);
}

/*---------------------------------------------------------------------------*/

void init_Gather(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  set_send_buffer_usage(get_extent(send_count, send_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  if( get_measurement_rank() == root )
    set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  else
    set_recv_buffer_usage(0);
  init_synchronization();
}

double measure_Gather(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Gather(get_send_buffer(), send_count, send_dt, 
	     get_recv_buffer(), recv_count, recv_dt,
	     root, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Gather_using_Send_Recv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  set_send_buffer_usage(get_extent(send_count, send_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  if( get_measurement_rank() == root )
    set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  else
    set_recv_buffer_usage(0);
  recv_dt_extent = get_extent(1, recv_dt);
  init_synchronization();
}

double measure_Gather_using_Send_Recv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  MPI_Status status;
  double start_time, end_time;
  int i;

  if( get_measurement_rank() == root ) {
    start_time = start_synchronization();
    for( i = 0; i < get_measurement_size(); i++)
      if( i != root )
	MPI_Recv((char*)get_recv_buffer() + i*recv_count*recv_dt_extent, recv_count,
		 recv_dt, i, 0, get_measurement_comm(), &status);
    end_time = stop_synchronization();
    return end_time - start_time;
  } else {
    start_time = start_synchronization();
    MPI_Send(get_send_buffer(), send_count, send_dt, root, 0, get_measurement_comm());
    end_time = stop_synchronization();
    return end_time - start_time;
  }
}

/*---------------------------------------------------------------------------*/

void init_Gather_using_Isend_Irecv_Waitall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  set_send_buffer_usage(get_extent(send_count, send_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  if( get_measurement_rank() == root ) {
    set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
    initialize_req_array(get_measurement_size());
    initialize_status_array(get_measurement_size());
  } else
    set_recv_buffer_usage(0);
  recv_dt_extent = get_extent(1, recv_dt);
  init_synchronization();
}

double measure_Gather_using_Isend_Irecv_Waitall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  MPI_Request req;
  MPI_Status status;
  double start_time, end_time;
  int i;

  if( get_measurement_rank() == root ) {
    start_time = start_synchronization();
    for( i = 0; i < get_measurement_size(); i++)
      if( i != root )
	MPI_Irecv((char*)get_recv_buffer() + i*recv_count*recv_dt_extent, recv_count,
		 recv_dt, i, 0, get_measurement_comm(), &req_array[i]);
    req_array[root] = MPI_REQUEST_NULL;
    MPI_Waitall(get_measurement_size(), req_array, status_array);
    end_time = stop_synchronization();
  } else {
    start_time = start_synchronization();
    MPI_Isend(get_send_buffer(), send_count, send_dt, root, 0, get_measurement_comm(), 
	      &req);
    end_time = stop_synchronization();
    MPI_Wait(&req, &status);
  }
  return end_time - start_time;
}

void finalize_Gather_using_Isend_Irecv_Waitall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  if( get_measurement_rank() == root ) {
    free(req_array);
    free(status_array);
  }
}
  
/*---------------------------------------------------------------------------*/

void init_Allgather(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  set_send_buffer_usage(get_extent(send_count, send_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  init_synchronization();
}

double measure_Allgather(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Allgather(get_send_buffer(), send_count, send_dt, 
		get_recv_buffer(), recv_count, recv_dt, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Scatter(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  if( get_measurement_rank() == root )
    set_send_buffer_usage(get_measurement_size()*get_extent(send_count, send_dt));
  else
    set_send_buffer_usage(0);
  set_recv_buffer_usage(get_extent(recv_count, recv_dt));
  set_reported_message_size(get_extent(recv_count, recv_dt));
  init_synchronization();
}

double measure_Scatter(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Scatter(get_send_buffer(), send_count, send_dt, 
	     get_recv_buffer(), recv_count, recv_dt,
	     root, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Reduce_Scatterv(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  MPI_Aint extent;

  send_counts = initialize_counts(count);
  send_displs = initialize_displs(send_counts);

  extent = get_extent(count, dt);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  if( get_measurement_rank() == root ) 
    set_recv_buffer_usage(extent);
  else
    set_recv_buffer_usage(0);
  init_synchronization();
}

double measure_Reduce_Scatterv(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Reduce(get_send_buffer(), get_recv_buffer(), count, dt, op, root, 
	     get_measurement_comm());
  MPI_Scatterv(get_recv_buffer(), send_counts, send_displs, dt, 
	       get_send_buffer(), send_counts[get_measurement_rank()], dt, 
	       root, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Reduce_Scatterv(int count, MPI_Datatype dt, MPI_Op op, int root)
{
  free(send_counts);
  free(send_displs);
}

/*---------------------------------------------------------------------------*/

void init_Alltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  send_counts = initialize_counts(send_count*get_measurement_size());
  send_displs = initialize_displs(send_counts);
  recv_counts = initialize_counts(recv_count*get_measurement_size());
  recv_displs = initialize_displs(recv_counts);
  
  set_send_buffer_usage(get_measurement_size()*get_extent(send_count, send_dt));
  set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  init_synchronization();
}

double measure_Alltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Alltoallv(get_send_buffer(), send_counts, send_displs, send_dt,
		get_recv_buffer(), recv_counts, recv_displs, recv_dt, 
		get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Alltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  free(send_counts);
  free(send_displs);
  free(recv_counts);
  free(recv_displs);
}

/*---------------------------------------------------------------------------*/

void init_Alltoallv_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  send_counts = initialize_counts(send_count*get_measurement_size());
  send_displs = initialize_displs(send_counts);
  recv_counts = initialize_counts(recv_count*get_measurement_size());
  recv_displs = initialize_displs(recv_counts);

  initialize_req_array(2*get_measurement_size());
  initialize_status_array(2*get_measurement_size());
  
  send_dt_extent = get_extent(1, send_dt);
  recv_dt_extent = get_extent(1, recv_dt);

  set_send_buffer_usage(get_measurement_size()*get_extent(send_count, send_dt));
  set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  init_synchronization();
}

double measure_Alltoallv_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  double start_time, end_time;
  int i;

  start_time = start_synchronization();
  for( i = 0; i < get_measurement_size(); i++)
    MPI_Irecv((char*)get_recv_buffer() + recv_displs[i]*recv_dt_extent, recv_count, 
	      recv_dt, i, 0, get_measurement_comm(), &(req_array[i]));

  for( i = 0; i < get_measurement_size(); i++)
    MPI_Isend((char*)get_send_buffer() + send_displs[i]*send_dt_extent, send_count, 
	      send_dt, i, 0, get_measurement_comm(), &(req_array[i+get_measurement_size()]));

  MPI_Waitall(2*get_measurement_size(), req_array, status_array);
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Alltoallv_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  free(send_counts);
  free(send_displs);
  free(recv_counts);
  free(recv_displs);
  free(req_array);
  free(status_array);
}

/*---------------------------------------------------------------------------*/

void init_Gatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  recv_counts = initialize_counts(recv_count*get_measurement_size());
  recv_displs = initialize_displs(recv_counts);

  set_send_buffer_usage(get_extent(send_count, send_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  if( get_measurement_rank() == root )
    set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  else
    set_recv_buffer_usage(0);
  init_synchronization();
}

double measure_Gatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Gatherv(get_send_buffer(), send_count, send_dt, 
	      get_recv_buffer(), recv_counts, recv_displs, recv_dt,
	      root, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Gatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  free(recv_counts);
  free(recv_displs);
}

/*---------------------------------------------------------------------------*/

void init_Allgatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  recv_counts = initialize_counts(recv_count*get_measurement_size());
  recv_displs = initialize_displs(recv_counts);

  set_send_buffer_usage(get_extent(send_count, send_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  init_synchronization();
}

double measure_Allgatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Allgatherv(get_send_buffer(), send_count, send_dt, 
		 get_recv_buffer(), recv_counts, recv_displs, recv_dt, 
		 get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Allgatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt)
{
  free(recv_counts);
  free(recv_displs);
}

/*---------------------------------------------------------------------------*/

void init_Scatterv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  send_counts = initialize_counts(send_count*get_measurement_size());
  send_displs = initialize_displs(send_counts);

  if( get_measurement_rank() == root )
    set_send_buffer_usage(get_measurement_size()*get_extent(send_count, send_dt));
  else
    set_send_buffer_usage(0);
  set_recv_buffer_usage(get_extent(recv_count, recv_dt));
  set_reported_message_size(get_extent(recv_count, recv_dt));
  init_synchronization();
}

double measure_Scatterv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Scatterv(get_send_buffer(), send_counts, send_displs, send_dt, 
	      get_recv_buffer(), recv_count, recv_dt, root, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_Scatterv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root)
{
  free(send_counts);
  free(send_displs);
}

/*---------------------------------------------------------------------------*/

void init_Scan(int count, MPI_Datatype dt, MPI_Op op)
{
  MPI_Aint extent;

  extent = get_extent(count, dt);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
  init_synchronization();
}

double measure_Scan(int count, MPI_Datatype dt, MPI_Op op)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Scan(get_send_buffer(), get_recv_buffer(), count, dt, op, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Comm_split(void)
{
  init_synchronization();
}

double measure_Comm_split(void)
{
  double start_time, end_time;
  MPI_Comm new_comm;

  start_time = start_synchronization();
  MPI_Comm_split(get_measurement_comm(), get_measurement_rank() % 2, 0, &new_comm);
  end_time = stop_synchronization();
  MPI_Comm_free(&new_comm);
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Comm_dup(void)
{
  init_synchronization();
}

double measure_Comm_dup(void)
{
  double start_time, end_time;
  MPI_Comm new_comm;

  start_time = start_synchronization();
  MPI_Comm_dup(get_measurement_comm(), &new_comm);
  end_time = stop_synchronization();
  MPI_Comm_free(&new_comm);
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/


int *init_send_counts(int count, int charged_rank, double ratio)
{
  int i;
  int *counts;

  counts = (int*)malloc(get_measurement_size()*sizeof(int));
  assert( counts != NULL );

  for( i = 0; i < get_measurement_size(); i++){
    if( i == charged_rank ){
      counts[i] = (int)(count * ratio) / get_measurement_size();
    }
    else{
      counts[i] = (int)(count / ratio) / get_measurement_size();
    }
  }

  for( i = 0; i < count % get_measurement_size(); i++) 
    counts[i*get_measurement_size() / (count % get_measurement_size())]++;

  return counts;
}

int *init_recv_counts(int count, int charged_rank, double ratio)
{
  int i;
  int *counts;

  counts = (int*)malloc(get_measurement_size()*sizeof(int));
  assert( counts != NULL );

  if( get_measurement_rank() == charged_rank ){
    for( i = 0; i < get_measurement_size(); i++){
      counts[i] = (int)(count * ratio) / get_measurement_size();
    }
  }
  else{
    for( i = 0; i < get_measurement_size(); i++){
      counts[i] = (int)(count / ratio) / get_measurement_size();
    }
  }

  if( count % get_measurement_size() && 
      (get_measurement_rank() % (get_measurement_size() / (count % get_measurement_size())) == 0) ){
    for( i = 0; i < get_measurement_size(); i++){
      counts[i]++;
    }
  }
  return counts;
}

void init_myalltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio)
{
  int p;
  int send_buffer_usage, recv_buffer_usage;


  send_counts = init_send_counts(send_count*get_measurement_size(), charged_rank, ratio);
  send_displs = initialize_displs(send_counts);
  recv_counts = init_recv_counts(recv_count*get_measurement_size(), charged_rank, ratio);
  recv_displs = initialize_displs(recv_counts);

  send_buffer_usage = 0;
  recv_buffer_usage = 0;
  for(p = 0; p < get_measurement_size(); p++) {
    send_buffer_usage += get_extent(send_counts[p], send_dt);
    recv_buffer_usage += get_extent(recv_counts[p], recv_dt);
  }
  set_send_buffer_usage(send_buffer_usage);
  set_recv_buffer_usage(recv_buffer_usage);
  set_reported_message_size(send_buffer_usage);

  init_synchronization();
}

double measure_myalltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio)
{
  double start_time, end_time;

  start_time = start_synchronization();
  MPI_Alltoallv(get_send_buffer(), send_counts, send_displs, send_dt,
                get_recv_buffer(), recv_counts, recv_displs, recv_dt, 
                get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

void finalize_myalltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio)
{
  free(send_counts);
  free(send_displs);
  free(recv_counts);
  free(recv_displs);
}

#pragma weak end_skampi_extensions

