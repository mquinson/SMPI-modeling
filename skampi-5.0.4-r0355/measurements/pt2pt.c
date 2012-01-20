/* SKaMPI   MPI-Benchmark

Copyright 2003-2008 Werner Augustin, Thomas Worsch
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

#include "../misc.h"


#pragma weak begin_skampi_extensions

/* TODO:
   -- initial (kind of) synchronizations
   -- What if some measurement function is started on more or less
      than 2 processes; check in the init_ functions??
*/

/*---------------------------------------------------------------------------*/

void init_Pingpong_Send_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Send_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  int i;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
      MPI_Recv(get_recv_buffer(), count, datatype, 1, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
    MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
  } else {
      MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
      start_time = MPI_Wtime();
      for (i=0; i<iterations; i++) {
        MPI_Send(get_send_buffer(), count, datatype, 0, tag, get_measurement_comm());
        MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
      }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

/*---------------------------------------------------------------------------*/

void init_Pingpong_Send_Iprobe_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Send_Iprobe_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  int i, flag;
  
  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
      do {
        MPI_Iprobe(1, tag, get_measurement_comm(), &flag, &status);
      } while( !flag);
      MPI_Recv(get_recv_buffer(), count, datatype, 1, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
    MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
    return MPI_Wtime() - start_time;
  } else {
    MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Send(get_send_buffer(), count, datatype, 0, tag, get_measurement_comm());
      do { /* ??? Change! add iprobe for rank 1 to make time comparable to that of rank 0 ! */
        MPI_Iprobe(0, tag, get_measurement_comm(), &flag, &status);
      } while( !flag);
      MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}
/*---------------------------------------------------------------------------*/

void init_Pingpong_Send_Irecv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Send_Irecv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  MPI_Request req;
  int i;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
      MPI_Irecv(get_recv_buffer(), count, datatype, 1, tag, get_measurement_comm(), &req);
      MPI_Wait(&req, &status);
    }
    end_time =  MPI_Wtime();
    MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
  } else {
    MPI_Irecv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &req);
    MPI_Wait(&req, &status);
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Send(get_send_buffer(), count, datatype, 0, tag, get_measurement_comm());
      MPI_Irecv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &req);
      MPI_Wait(&req, &status);
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

/*---------------------------------------------------------------------------*/

void init_Pingpong_Send_Recv_AT(int count, MPI_Datatype datatype, int tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Send_Recv_AT(int count, MPI_Datatype datatype, int tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  int i;
  
  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
      MPI_Recv(get_recv_buffer(), count, datatype, 1, MPI_ANY_TAG, get_measurement_comm(),
               &status);
    }
    end_time = MPI_Wtime();
    MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
  } else {
    MPI_Recv(get_recv_buffer(), count, datatype, 0, MPI_ANY_TAG, get_measurement_comm(),
             &status);
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Send(get_send_buffer(), count, datatype, 0, tag, get_measurement_comm());
      MPI_Recv(get_recv_buffer(), count, datatype, 0, MPI_ANY_TAG, get_measurement_comm(),
               &status);
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

/*---------------------------------------------------------------------------*/

void init_Pingpong_Ssend_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Ssend_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  int i;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Ssend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
      MPI_Recv(get_recv_buffer(), count, datatype, 1, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
    MPI_Ssend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
  } else {
    MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Ssend(get_send_buffer(), count, datatype, 0, tag, get_measurement_comm());
      MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

/*---------------------------------------------------------------------------*/

void init_Pingpong_Isend_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Isend_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  MPI_Request req;
  int i;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Isend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm(), &req);
      MPI_Wait(&req, &status);
      MPI_Recv(get_recv_buffer(), count, datatype, 1, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
    MPI_Isend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm(), &req);
    MPI_Wait(&req, &status);
  } else {
    MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Isend(get_send_buffer(), count, datatype, 0, tag, get_measurement_comm(), &req);
      MPI_Wait(&req, &status);
      MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

/*---------------------------------------------------------------------------*/

void init_Pingpong_Issend_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Issend_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  MPI_Request req;
  int i;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Issend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm(), &req);
      MPI_Wait(&req, &status);
      MPI_Recv(get_recv_buffer(), count, datatype, 1, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
    MPI_Issend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm(), &req);
    MPI_Wait(&req, &status);
  } else {
    MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Issend(get_send_buffer(), count, datatype, 0, tag, get_measurement_comm(), &req);
      MPI_Wait(&req, &status);
      MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

/*---------------------------------------------------------------------------*/

void init_Pingpong_Bsend_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Bsend_Recv(int count, MPI_Datatype datatype, int tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  int i;
  
  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Bsend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
      MPI_Recv(get_recv_buffer(), count, datatype, 1, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
    MPI_Bsend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
  } else {
    MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Bsend(get_send_buffer(), count, datatype, 0, tag, get_measurement_comm());
      MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

/*---------------------------------------------------------------------------*/
void init_Pingpong_Sendrecv(int count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, send_dt);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Pingpong_Sendrecv(int send_count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  int i;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Sendrecv(get_send_buffer(), send_count, send_dt, 1, send_tag, 
                   get_recv_buffer(), recv_count, recv_dt, 1, recv_tag, 
                   get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
  } else {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Sendrecv(get_send_buffer(), recv_count, recv_dt, 0, recv_tag, 
                   get_recv_buffer(), send_count, send_dt, 0, send_tag, 
                   get_measurement_comm(), &status);
    /* @@ did I understand it correctly ? count, dt and tags should be 
       swapped ?!  */
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

/*---------------------------------------------------------------------------*/

void init_Pingpong_Sendrecv_replace(int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(0);
}

double measure_Pingpong_Sendrecv_replace(int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations)
{
  double start_time, end_time;
  MPI_Status status;
  int i;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Sendrecv_replace(get_send_buffer(), count, datatype, 1, send_tag, 
                           1, recv_tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
  } else {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Sendrecv_replace(get_send_buffer(), count, datatype, 0, recv_tag, 
                           0, send_tag, get_measurement_comm(), &status);
      /* @@ did I understand it correctly ? tags should be swapped ?!  */
    }
    end_time = MPI_Wtime();
  }
  return (end_time - start_time)/iterations;
}

#pragma weak end_skampi_extensions
