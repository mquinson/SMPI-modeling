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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <assert.h>

#include "../misc.h"
#include "../exec.h"


#pragma begin_skampi_extensions

void init_bug(int iterations)
{
  MPI_Aint extent;

  set_send_buffer_usage(42);
  set_reported_message_size(42);
  set_recv_buffer_usage(42);
}

double measure_bug(int i)
{
  double start_time, end_time;
  MPI_Status status;

#if 0  
  int a, b=0, c, d=0;

  if (i<0) { return -1.0; }
  if (i==0) { return 0.0; }
  
  start_time = MPI_Wtime();

  MPI_Reduce(&a, &b, 1, MPI_INT, MPI_SUM, 0, get_measurement_comm());
  fprintf(stderr, "a=%d  b=%d\n"); fflush(stderr);

  for (i=0; i<1; i++) {
    MPI_Allreduce(&a, &b, 1, MPI_INT, MPI_SUM, get_measurement_comm());
  }

  MPI_Reduce(&a, &b, 1, MPI_INT, MPI_SUM, 0, get_measurement_comm());
  fprintf(stderr, "a=%d  b=%d\n"); fflush(stderr);


  end_time = MPI_Wtime();
  return (end_time - start_time)/i;
#endif

  return 0.0;

}

