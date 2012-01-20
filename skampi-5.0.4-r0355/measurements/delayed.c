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

#include <mpi.h>
#include <assert.h>


#include "../misc.h"
#include "../synchronize.h"
#include "../private_misc.h"

#pragma weak begin_skampi_extensions

void init_delayed_Alltoall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node)
{
  set_send_buffer_usage(get_measurement_size()*get_extent(send_count, send_dt));
  set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  init_synchronization();
}

double measure_delayed_Alltoall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node)
{
  double start_time, end_time;

  start_time = start_synchronization();
  if( get_measurement_rank() == node )  while( wtime() < start_time + delay ) ;
  MPI_Alltoall(get_send_buffer(), send_count, send_dt,
	       get_recv_buffer(), recv_count, recv_dt, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_delayed_Gather(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node)
{
  set_send_buffer_usage(get_extent(send_count, send_dt));
  set_reported_message_size(get_extent(send_count, send_dt));
  if( get_measurement_rank() == root )
    set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt));
  else
    set_recv_buffer_usage(0);
  init_synchronization();
}

double measure_delayed_Gather(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node)
{
  double start_time, end_time;

  start_time = start_synchronization();
  if( get_measurement_rank() == node )  while( wtime() < start_time + delay ) ;
  MPI_Gather(get_send_buffer(), send_count, send_dt, 
	     get_recv_buffer(), recv_count, recv_dt,
	     root, get_measurement_comm());
  end_time = stop_synchronization();
  return end_time - start_time;
}

#pragma weak end_skampi_extensions
