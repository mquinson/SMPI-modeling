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
#include <stdlib.h>

#include "../misc.h"
#include "../private_misc.h"
#include "../synchronize.h"

MPI_Request *req_array;

#pragma weak begin_skampi_extensions

/* ********************************************************************** */
/* two simple measurements for collective waiting operations              */
/* ********************************************************************** */

void init_waitpattern_down(double scale)
{
  init_synchronization();
}

double measure_waitpattern_down(double scale)
{
  double start_time, end_time;

  start_time = start_synchronization();
  while( (end_time = wtime()) < (start_time + (get_measurement_size() - get_measurement_rank()) * scale) ) 
    {/* active waiting */}
  end_time = stop_synchronization();
  return end_time - start_time;
}

void init_waitpattern_up(double scale)
{
  init_synchronization();
}

double measure_waitpattern_up(double scale)
{
  double start_time, end_time;

  start_time = start_synchronization();
  while( (end_time = wtime()) < (start_time + (get_measurement_rank() + 1) * scale) )
    {/* active waiting */}
  end_time = stop_synchronization();
  return end_time - start_time;
}

/* ********************************************************************** */
/* two pretty useless iterators                                           */
/* ********************************************************************** */

/* ---------------------------------------------------------------------
 * The first iterator is not re-entrant, i.e. you cannot nest two loops
 * both of which use it.
 * a re-entrant version should use something like *self = malloc(...) 
 */

/* This iterator could be used in a SKaMPI input file like this:
 *
 *   for i = range(2,7) do
 *      <loop body>
 *   od
 *
 * and the loop body would be executed for each integer from 2 to 6 as 
 * the value of variable i
 */

int int_state;

int iterator_range(void **self, int start, int end)
{
  if( *self == NULL ) {
    *self = &int_state;
    int_state = start+1;
    return start;
  };

  if( int_state >= end ) {
    *self = NULL;
    return 42;
  }
  
  return int_state++;
}

/* ---------------------------------------------------------------------
 * For comparison the following iterator IS re-entrant, i.e. you CAN use
 * two (or more) nested loops which use it; 
 */

/* This iterator could be used in a SKaMPI input file like this:
 *
 *   for i = squares(2,7) do
 *      for j = squares(1,4) do
 *         <loop body>
 *      od
 *   od
 *
 * and the loop body would be executed for each pair (i*i, j*j) with 
 * i from the interval 2..7 and j from the interval 1..4, i.e. 
 * for (4,1), (4,4), (4,9), (9,1), (9,4), (9,9), (16,1), .... (36,9).
 */

int iterator_squares(void **self, int start, int end)
{
  int *counter;

  if( *self == NULL ) {
    if( start > end ) return 42;
    counter = skampi_malloc_ints(1);
    *self = counter;
    *counter = start;
    return start*start;
  };

  counter = *self;

  if( *counter >= end ) {
    *self = NULL;
    return 42; 
  }

  (*counter)++;

  return isqr(*counter);
}

#pragma weak end_skampi_extensions
