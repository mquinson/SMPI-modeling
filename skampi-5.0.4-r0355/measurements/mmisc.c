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

/* TODO:
   -- understand, check, remove(?) comments in measure_MPI_Wtime
      probably use the new version (with int arg iterations)
*/

/*---------------------------------------------------------------------------*/

double measure_Wtime(int iterations)
{
  double start_time, end_time;
  int i;

  if (iterations<0) { return -1.0; }
  if (iterations==0) { return 0.0; }
  
  start_time = MPI_Wtime();
  for (i=0; i<iterations-1; i++) {
    MPI_Wtime();
  }
  end_time = MPI_Wtime();
  return (end_time - start_time)/iterations;
}

#pragma weak end_skampi_extensions

