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
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>

#include "mpiversiontest.h"

#include "misc.h"
#include "private_misc.h"
#include "debug.h"
#include "synchronize.h"
#include "accounting.h"

double *ping_pong_min_time; /* ping_pong_min_time[i] is the minimum time of one ping_pong
			       between the current node and node i, negative value means 
			       time not yet determined */
double *tds;                /* tds[i] is the time difference between the 
			       current node and global node i */


enum {
  Number_ping_pongs = 100,
  Minimum_ping_pongs = 8
};

bool mpi_wtime_is_global;

extern int wait_till(double time_stamp, double *last_time_stamp);
extern double should_wait_till(int counter, double interval, double offset);


void init_synchronization_module(void)
{
  int i, flag;
  int *mpi_wtime_is_global_ptr;

  tds = skampi_malloc_doubles(get_global_size());
  for(i = 0; i < get_global_size(); i++) tds[i] = 0.0;
  ping_pong_min_time = skampi_malloc_doubles(get_global_size());
  for( i = 0; i < get_global_size(); i++) ping_pong_min_time[i] = -1.0;

#if MPI_VERSION < 2
  MPI_Attr_get(MPI_COMM_WORLD, MPI_WTIME_IS_GLOBAL, &mpi_wtime_is_global_ptr, &flag);
#else
  MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_WTIME_IS_GLOBAL, &mpi_wtime_is_global_ptr, &flag);
#endif
  if( flag == 0 ) 
    mpi_wtime_is_global = False;
  else
    mpi_wtime_is_global = *mpi_wtime_is_global_ptr;

  rdebug(DBG_TIMEDIFF, "mpi_wtime_is_global = %d\n", mpi_wtime_is_global);
}

void print_global_time_differences(void)
{
  int i, p, name_len, pid;
  char my_name[MPI_MAX_PROCESSOR_NAME];


  double *all_tds = NULL;
  char *names = NULL;
  int *pids = NULL;

  if( grootproc() ) {
    all_tds = skampi_malloc_doubles(get_global_size()*get_global_size());
    names = skampi_malloc_chars(get_global_size()*MPI_MAX_PROCESSOR_NAME);
    pids = skampi_malloc_ints(get_global_size());
  }

  MPI_Get_processor_name(my_name, &name_len);
  my_name[name_len] = '\0';
  pid = getpid();

  MPI_Gather(tds, get_global_size(), MPI_DOUBLE, 
	     all_tds, get_global_size(), MPI_DOUBLE, 
	     0, MPI_COMM_WORLD);
  MPI_Gather(my_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR,
	     names, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);
  MPI_Gather(&pid, 1, MPI_INT, pids, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if( grootproc() ) {
    for( i = 0; i < get_global_size(); i++)
      printf("name[%d] = \"%s\" pid=%d\n", i, &(names[i*MPI_MAX_PROCESSOR_NAME]), pids[i]);

    for( p = 0; p < get_global_size(); p++) {
      printf("tds[%d -> ..] = [%9.6f", p, all_tds[p*get_global_size() + 0]);
      for( i = 1; i < get_global_size(); i++) 
	printf(", %9.6f", all_tds[p*get_global_size() + i]);
      printf("]\n");
    }

    free(all_tds);
    free(names);
    free(pids);
  }
}

/*----------------------------------------------------------------------------*/

static void ping_pong(int p1, int p2)
{
  int i, other_global_id;
  double my_time, my_last_time, other_time;
  double td_min, td_max;
  double invalid_time = -1.0;
  MPI_Status status;
  int pp_tag = 43;


  /* I had to unroll the main loop because I didn't find a portable way
     to define the initial td_min and td_max with INFINITY and NINFINITY */
  if( get_measurement_rank() == p1 ) {
    other_global_id = get_global_rank(p2);
    my_last_time = wtime();
    MPI_Send(&my_last_time, 1, MPI_DOUBLE, p2, pp_tag, get_measurement_comm());
    MPI_Recv(&other_time, 1, MPI_DOUBLE, p2, pp_tag, get_measurement_comm(), &status);
    my_time = wtime();
    td_min = other_time - my_time;
    td_max = other_time - my_last_time;
    MPI_Send(&my_time, 1, MPI_DOUBLE, p2, pp_tag, get_measurement_comm());
  } else {
    other_global_id = get_global_rank(p1);
    MPI_Recv(&other_time, 1, MPI_DOUBLE, p1, pp_tag, get_measurement_comm(), &status);
    my_last_time = wtime();
    td_min = other_time - my_last_time;
    MPI_Send(&my_last_time, 1, MPI_DOUBLE, p1, pp_tag, get_measurement_comm());
    MPI_Recv(&other_time, 1, MPI_DOUBLE, p1, pp_tag, get_measurement_comm(), &status);
    my_time = wtime();
    td_min = fmax2(td_min, other_time - my_time);
    td_max = other_time - my_last_time;
  }

  if( get_measurement_rank() == p1 ) {
    i = 1;
    while( 1 ) {
      MPI_Recv(&other_time, 1, MPI_DOUBLE, p2, pp_tag, get_measurement_comm(), &status);
      if( other_time < 0.0 ) break; 
      my_last_time = my_time; 
      my_time = wtime();
      td_min = fmax2(td_min, other_time - my_time);
      td_max = fmin2(td_max, other_time - my_last_time);
      if( ping_pong_min_time[other_global_id] >= 0.0  && 
	  i >= Minimum_ping_pongs && 
	  my_time - my_last_time < ping_pong_min_time[other_global_id]*1.10 ) {
	MPI_Send(&invalid_time, 1, MPI_DOUBLE, p2, pp_tag, get_measurement_comm());
	break;
      }
      i++;
      if( i == Number_ping_pongs ) {
	MPI_Send(&invalid_time, 1, MPI_DOUBLE, p2, pp_tag, get_measurement_comm());
	break;
      }
      MPI_Send(&my_time, 1, MPI_DOUBLE, p2, pp_tag, get_measurement_comm());

    }
  } else {
    i = 1;
    while( 1 ) {
      MPI_Send(&my_time, 1, MPI_DOUBLE, p1, pp_tag, get_measurement_comm());
      MPI_Recv(&other_time, 1, MPI_DOUBLE, p1, pp_tag, get_measurement_comm(), &status);
      if( other_time < 0.0 ) break; 
      my_last_time = my_time;
      my_time = wtime();
      td_min = fmax2(td_min, other_time - my_time);
      td_max = fmin2(td_max, other_time - my_last_time);
      if( ping_pong_min_time[other_global_id] >= 0.0 && 
	  i >= Minimum_ping_pongs &&
	  my_time - my_last_time < ping_pong_min_time[other_global_id]*1.10 ) {
	MPI_Send(&invalid_time, 1, MPI_DOUBLE, p1, pp_tag, get_measurement_comm());
	break;
      }
      i++;
    }
  }
  
  if( ping_pong_min_time[other_global_id] < 0.0) 
    ping_pong_min_time[other_global_id] = td_max-td_min;
  else 
    ping_pong_min_time[other_global_id] = 
      fmin2(ping_pong_min_time[other_global_id], td_max-td_min);

  tds[other_global_id] = (td_min+td_max)/2.0;
}

void determine_time_differences(void)
{
  int i;
  double *tmp_tds;
  double start_time, finish_time;

  if( lrootproc() ) 
    start_time = acc_start_sync();
  else
    start_time = MPI_Wtime();
  logging(DBG_SYNC, "determine_time_differences\n");
  for( i = 1; i < get_measurement_size(); i++) {
    MPI_Barrier(get_measurement_comm());
    if( get_measurement_rank() == 0 || get_measurement_rank() == i ) 
      ping_pong(0,i);
  }
  tmp_tds = skampi_malloc_doubles(get_measurement_size());
  if( lrootproc() ) {
    for( i = 1; i < get_measurement_size(); i++) 
            tmp_tds[i] = tds[get_global_rank(i)];
  }
  
  assert(get_measurement_size() - 1 >= 0 );
  MPI_Bcast(&(tmp_tds[1]), get_measurement_size()-1, MPI_DOUBLE, 0, get_measurement_comm());

  if( get_measurement_rank() != 0 ) {
    logging(DBG_TIMEDIFF, "tds[%d] = %9.1f us\n", 
	    get_global_rank(0), tds[get_global_rank(0)]*1.0e6);
    for( i = 1; i < get_measurement_size(); i++) {
      tds[get_global_rank(i)] = tmp_tds[i] + tds[get_global_rank(0)];
      logging(DBG_TIMEDIFF, "tds[%d] = %9.1f us\n", 
	      get_global_rank(i), tds[get_global_rank(i)]*1.0e6);
    }
  } 
  free(tmp_tds);
  MPI_Barrier(get_measurement_comm());
  if( lrootproc() )
    finish_time = acc_stop_sync();
  else
    finish_time = MPI_Wtime();
  rdebug(DBG_TIMEDIFF, "determine_time_differences() on %d processes: %f sec.\n", 
	 get_measurement_size(), finish_time - start_time);
}
