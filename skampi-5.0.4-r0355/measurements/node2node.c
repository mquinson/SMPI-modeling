/* SKaMPI   MPI-Benchmark

Copyright 2006-2008 Thomas Worsch
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
#include <stdio.h>
#include "../output.h"


#pragma weak begin_skampi_extensions

/*---------------------------------------------------------------------------*/

void init_Par_Pingpong_Send_Recv(int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);

  if (get_measurement_size() < 2*nodesize) {
    error_without_abort("ERROR in node2node.c/init_Par_Pingpong_Send_Recv: \n"
                        "   measurement communicator is too small\n"
                        "   got only %d procs, need %d (nodesize=%d)", 
                        get_measurement_size(), 2*nodesize, nodesize);
  }
}

double measure_Par_Pingpong_Send_Recv(int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize)
{
  double start_time, end_time;
  MPI_Status status;
  int i, m_rank;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  m_rank = get_measurement_rank();

  if( m_rank < procs ) {
    start_time = MPI_Wtime();
    for (i=0; i<iterations; i++) {
      MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
      MPI_Recv(get_recv_buffer(), count, datatype, 1, tag, get_measurement_comm(), &status);
    }
    end_time = MPI_Wtime();
    MPI_Send(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
  } else if (nodesize+0 <= m_rank  &&  m_rank < nodesize+procs) {
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


#pragma weak end_skampi_extensions
