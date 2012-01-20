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
#include <assert.h>
#include <mpi.h>
#include <math.h>

#include "misc.h"
#include "private_misc.h"
#include "debug.h"
#include "output.h"
#include "main.h"
#include "exec.h"
#include "print.h"
#include "mem.h"
#include "measurement.h"
#include "symboltable.h"
#include "synchronize.h"
#include "accounting.h"
#include "autorefine.h"

static struct variable meas_params[MAX_NUMBER_PARAMS];



static double max_relative_standard_error = 0.10;
static int min_repetitions = 8;
static int max_repetitions = 33;
static int max_nr_node_times = 16;

#define First_interval 0.00000 /* rather to small, than to large */

enum {
  First_max_counter = 4,

  INVALID_STARTED_LATE = 600, /* id should increase like accounting priority */
  INVALID_TOOK_TOO_LONG = 601
};

static void call_set_max_relative_standard_error(struct variable *result, struct variable par[])
{ 
  max_relative_standard_error = par[0].u.doublev; 
}

static void call_set_min_repetitions(struct variable *result, struct variable par[])
{
  min_repetitions = par[0].u.intv;
}

static void call_set_max_repetitions(struct variable *result, struct variable par[])
{
  max_repetitions = par[0].u.intv;
}

static void call_set_max_nr_node_times(struct variable *result, struct variable par[])
{
  max_nr_node_times = par[0].u.intv;
}

void init_builtin_measurement_functions(void)
{
  insert_function(NULL, &call_set_max_relative_standard_error, NULL, 
		  "set_max_relative_standard_error", "f", TYPE_VOID); 
  insert_function(NULL, &call_set_min_repetitions, NULL, 
		  "set_min_repetitions", "i", TYPE_VOID);
  insert_function(NULL, &call_set_max_repetitions, NULL,
		  "set_max_repetitions", "i", TYPE_VOID);
  insert_function(NULL, &call_set_max_nr_node_times, NULL,
		  "set_max_nr_node_times", "i", TYPE_VOID);
}

/*---------------------------------------------------------------------------*/

static double *all_results;    /* all_results[get_measurement_size()][max_rep_hard_limit] only on lrootproc */
static double *max_results;
static double *final_results;

static int max_rep_hard_limit;

static int *invalid;             /* invalid[max_rep_hard_limit] */
static int *global_invalid;      /* invalid[max_rep_hard_limit*get_measurement_size()] */

static double mean_value, std_error;
static double sum_of_results;
static double sum_of_squares;

static double final_result;

static void update_std_error(int a, int n)
{
  int i, p;

  for( i = a; i < n; i++) {
    max_results[i] = 0.0;
    for( p = 0; p < get_measurement_size(); p++ ) 
      max_results[i] = fmax2( max_results[i], all_results[p * max_rep_hard_limit + i]);

    sum_of_results += max_results[i];
    sum_of_squares += fsqr( max_results[i] );
  }

  mean_value = sum_of_results / n;
  std_error = sqrt( fabs( sum_of_squares - (fsqr(sum_of_results)/n)) /
		    (n*(n-1))  );
  logging(DBG_MEAS, "new std_error = %9.1f\n", std_error*1.0e6);
}

#if 0
/* currently the following is  not used everywhere */
static int double_cmp (const void *d1, const void *d2)
{
  if (*(double *)d1 < *(double *)d2)
    return (-1);
  else
    return (*(double *)d1 > *(double *)d2);
}
#endif

static int max_result_cmp(const void *i1, const void *i2)
{
  if( max_results[*(int*)i1] < max_results[*(int*)i2] )
    return -1;
  else
    return max_results[*(int*)i1] > max_results[*(int*)i2];
}

static void sort_max_results(int n, int k, int *indices)
{
  int i;
  for( i = 0; i < n; i++) indices[i] = i;

  qsort(indices, n, sizeof(int), &max_result_cmp);
}

static void extract_final_results(int n)
{
  int *indices;
  int i,k,p;
  double v;

  k = n - n/4 - n/4;

  indices = skampi_malloc_ints(n);

  sort_max_results(n, n/4, indices);

  for( p = 0; p < get_measurement_size(); p++) {
    final_results[p] = 0.0;
    for( i = n/4; i < n - n/4; i++)
      final_results[p] += all_results[p*max_rep_hard_limit + indices[i] ];
    final_results[p] /= k;
  }

  sum_of_results = 0.0;
  sum_of_squares = 0.0;
  for( i = n/4; i < n - n/4; i++) {
    v = max_results[indices[i]];
    sum_of_results += v;
    sum_of_squares += fsqr(v);
  }
  final_result = sum_of_results / k;
  std_error = sqrt( fabs( sum_of_squares - (fsqr(sum_of_results)/k)) /
		    (k*(k-1))  );

  free(indices);
}

static int final_result_cmp(const void *i1, const void *i2)
{
  if( *((double*) i1) < *((double*) i2) )
    return -1;
  else
    return *((double*) i1) > *((double*) i2);
}

static double* extract_quantiles()
{
  int p;
  int ms;
  double *quantiles;
   
  ms = get_measurement_size();
  quantiles = skampi_malloc(max_nr_node_times * sizeof(double));
  
  qsort(final_results, ms, sizeof(double), &final_result_cmp);

  if ( max_nr_node_times > 1 ) {  
    for ( p=0; p < (max_nr_node_times - 1); p++ ) {
      quantiles[p] = final_results[ms * p / (max_nr_node_times - 1)];
    }
    quantiles[max_nr_node_times - 1] = final_results[ms - 1];
  } else {
    quantiles[0] = final_results[ms / 2];
  }

  return quantiles;
}  

/*---------------------------------------------------------------------------*/

static double start_batch, stop_batch, global_stop_batch, start_sync, stop_sync;

static enum sync_type current_synchronization, form_of_synchronization = SYNC_REAL;
static bool first_measurement_run;

static int counter, max_counter;

static double interval;


int wait_till(double time_stamp, double *last_time_stamp) 
{
  if( (*last_time_stamp = wtime()) > time_stamp ) 
    return 0;
  else {
    while( (*last_time_stamp = wtime()) < time_stamp ) ;
    return 1;
  }
}

double should_wait_till(int counter, double interval, double offset) 
{
  return (counter+1)*interval + start_batch + offset; 
}

static double normalize_time(double t)
{
  return (t - start_batch + tds[get_global_rank(0)])*1.0e6;
}

void set_synchronization(enum sync_type s)
{
  form_of_synchronization = s;
}

void init_synchronization(void)
{
  current_synchronization = form_of_synchronization;
  max_counter = First_max_counter;
  interval = First_interval;
  first_measurement_run = True;

  logging(DBG_SYNC, "starting with max_counter = %d interval = %9.1f\n", 
	 max_counter, interval*1.0e6);

  if( current_synchronization == SYNC_REAL) {
    if( ! mpi_wtime_is_global ) determine_time_differences();
    if( lrootproc() ) start_batch = wtime();
    logging(DBG_SYNC, "---- new start_batch ----------------\n");
    MPI_Bcast(&start_batch, 1, MPI_DOUBLE, 0, get_measurement_comm());
  }
}

double start_synchronization(void)
{
  switch( current_synchronization ) {
  case SYNC_BARRIER:
    MPI_Barrier(get_measurement_comm());
    start_sync = wtime();
    break;
  case SYNC_REAL:
    logging(DBG_SYNC, "start = %9.1f ", normalize_time(wtime()));
    logging(DBG_SYNC, " -> %9.1f ", (counter+1)*interval*1.0e6 );

    if( !wait_till(should_wait_till(counter, interval, -tds[get_global_rank(0)]), 
		   &start_sync) )
      invalid[counter] = INVALID_STARTED_LATE;
    else
      invalid[counter] = False;
    logging(DBG_SYNC, "start_sync = %9.1f ", normalize_time(start_sync));
    break;
  case SYNC_NOSYNC:
    start_sync = wtime();
    break;
  default:
    error_without_abort("out of cases in measurement.c__start_synchronization() current_synchronization = %d\n", current_synchronization);
  }

  return start_sync;
}

double stop_synchronization(void)
{
  stop_batch = stop_sync = wtime();

  if( current_synchronization == SYNC_REAL ) {

    if( stop_sync - start_sync > interval )
      invalid[counter] = INVALID_TOOK_TOO_LONG;
  
    logging(DBG_SYNC, "stop_sync = %9.1f ", normalize_time(stop_sync));
    switch( invalid[counter] ) {
    case INVALID_TOOK_TOO_LONG: logging(DBG_SYNC, "invalid_too_long\n"); break;
    case INVALID_STARTED_LATE:  logging(DBG_SYNC, "invalid_started_late\n"); break;
    default:                    logging(DBG_SYNC, "\n"); 
    }
  }

  return stop_sync;
}

static int get_longest_run(void)
{
  int i, p, longest_run, run_counter;

  longest_run = 0;
  for( p = 0; p < get_measurement_size(); p++ ) {
    run_counter = 0;
    for( i = 0; i < max_counter; i++ ) {
      if( !global_invalid[p*max_counter+i] ) {
	longest_run = imax2(longest_run, run_counter);
	run_counter = 0;
      } else if( global_invalid[p*max_counter+i] == INVALID_STARTED_LATE )
	  run_counter++;
    }
    longest_run = imax2(longest_run, run_counter);
  }
  return longest_run;
}

static int get_number_flawed_measurements(void)
{
  int i, p, flawed_measurements;
  bool flawed;

  flawed_measurements = 0;
  for( i = 0; i < max_counter; i++ ) {
    flawed = False;
    for( p = 0; p < get_measurement_size(); p++)
      flawed = imax2(flawed, global_invalid[p*max_counter+i]);
    if( flawed ) {
      global_invalid[i] = flawed;
      flawed_measurements++;
    }
  }
  return flawed_measurements;
}

static void update_batch_params(void)
{
  int longest_run, flawed_measurements;

  if( current_synchronization == SYNC_REAL ) {
    
    longest_run = get_longest_run();
    flawed_measurements = get_number_flawed_measurements();

    if( flawed_measurements > max_counter/4 ) {
      interval = fmax2(2.0*interval,
		       (global_stop_batch-start_batch)/(max_counter+1)*1.5);
    }
    if( first_measurement_run ) {
      first_measurement_run = False;
      max_counter = min_repetitions;
    } else if( longest_run > max_counter/2 ) { 
      /* more than half of the measurements are late in a row */
      max_counter = imax2(max_counter/2, First_max_counter);
    }
    logging(DBG_SYNC, "flawed_m = %d longest_run = %d max_counter = %d interval = %9.1f\n", 
	    flawed_measurements, longest_run, max_counter, interval*1.0e6);

  } else {
    max_counter = imax2(4, min_repetitions); 
    /* @@ how can we predict how many measurements we'll need to get a
       specific standard error ? */
  }
}


/*---------------------------------------------------------------------------*/

static void measurement_loop(struct term *t)
{
  struct variable spent_time;
  int i, p;
  int result_index, start_not_yet_gathered_results;
  int  source, dest;
  double *local_results;     /* local_results[max_rep_hard_limit] */
  double previous_interval;
  int previous_max_counter;
  bool stop;
  double *quantiles;
  
  int *recv_counts = NULL;
  int *recv_displs = NULL;

  logging(DBG_MEAS, "starting measurement with min_repetitions = %d\n", 
	  min_repetitions);
  logging(DBG_MEAS, "                          max_repetitions = %d\n", 
	  max_repetitions);
  logging(DBG_MEAS, "                          max_relative_standard_error = %f\n",
	  max_relative_standard_error);

  assert( min_repetitions <= max_repetitions );

  max_rep_hard_limit = imax2(First_max_counter,
			     max_repetitions*1.5+0.5); /* @@ pretty arbitrary */ 

  local_results = skampi_malloc_doubles(max_rep_hard_limit);
  invalid = skampi_malloc_ints(max_rep_hard_limit);
  if( lrootproc() ) {
    all_results = skampi_malloc_doubles(get_measurement_size()*max_rep_hard_limit);
    global_invalid = skampi_malloc_ints(get_measurement_size()*max_rep_hard_limit);

    recv_counts = skampi_malloc_ints(get_measurement_size());
    recv_displs = skampi_malloc_ints(get_measurement_size());

    max_results = skampi_malloc_doubles(max_rep_hard_limit); /* @@ max_repetitions? */
    final_results = skampi_malloc_doubles(get_measurement_size());
  }
  sum_of_results = 0.0;
  sum_of_squares = 0.0;
  result_index = 0;
  start_not_yet_gathered_results = 0;

  init_buffer();
  init_struct_variable(&spent_time, TYPE_VOID, NULL, NULL);
  stop = False; 
  do { /* measurement loop */
    
    
    logging(DBG_MEAS, "max_counter = %d\n", max_counter);
    /*    assert( result_index + max_counter <= max_rep_hard_limit); */
    max_counter = imin2(max_rep_hard_limit - result_index, max_counter);
    for( counter = 0; counter < max_counter; counter++ ) {  /* doing the actual measurements */
      check_buffer();
      t->call_fp(&spent_time, meas_params);
      assert( spent_time.type == TYPE_DOUBLE );
      logging(DBG_MEAS, "spent_time = %9.1f\n", spent_time.u.doublev*1.0e6);
      local_results[result_index] = spent_time.u.doublev;
      result_index++;
    }
    logging(DBG_MEAS, "result_index = %d\n", result_index);

    DEBUG(DBG_MEAS, 
    {
      for( i = start_not_yet_gathered_results; i < result_index; i++) 
	logging(DBG_MEAS, "local_results[%d] = %9.1f\n", i, local_results[i]*1.0e6);
    });  

    if( lrootproc() ) {
      for( p = 0; p < get_measurement_size(); p++) {
	recv_counts[p] = result_index - start_not_yet_gathered_results;
	recv_displs[p] = p*max_rep_hard_limit + start_not_yet_gathered_results;
      }
    }

    MPI_Gatherv(&(local_results[start_not_yet_gathered_results]), 
		result_index - start_not_yet_gathered_results, MPI_DOUBLE,
		all_results, recv_counts, recv_displs, MPI_DOUBLE, 0, get_measurement_comm());

    if( current_synchronization == SYNC_REAL ) {
      MPI_Gather(invalid, max_counter, MPI_INT, 
		 global_invalid, max_counter, MPI_INT, 0, get_measurement_comm());
      
      stop_batch += tds[get_global_rank(0)];
      
      MPI_Reduce(&stop_batch, &global_stop_batch, 1, MPI_DOUBLE, MPI_MAX, 0, get_measurement_comm());
    }

    if( lrootproc() ) { /* overwrite invalid results */ 

      for( i = start_not_yet_gathered_results; i < result_index; i++ ) {
	logging(DBG_MEAS, "all_results[%d] = [%9.1f", i, all_results[0*max_rep_hard_limit + i]*1.0e6);
	for( p = 1; p < get_measurement_size(); p++)
	  logging(DBG_MEAS, " %9.1f", all_results[p*max_rep_hard_limit + i]*1.0e6);
	logging(DBG_MEAS, "]\n"); 
      }

      previous_interval = interval;
      previous_max_counter = max_counter;
      update_batch_params(); /* adapt max_counter for next batch of measurements */

      logging(DBG_MEAS, "start overwrite invalid results\n");
      logging(DBG_MEAS, "start_not_yet_gathered_results = %d  result_index = %d\n", 
	      start_not_yet_gathered_results, result_index);

      /* move up valid results in all_results[] and global_invalid[], overwriting invalid ones */
      if( current_synchronization == SYNC_REAL ) {
	source = start_not_yet_gathered_results;
	dest = start_not_yet_gathered_results;
	while( source < result_index ) {
	  while( source < result_index && 
		 global_invalid[source-start_not_yet_gathered_results] ) { 
	    if( global_invalid[source-start_not_yet_gathered_results] == INVALID_TOOK_TOO_LONG ) {
	      /*	      double m = 0.0; */
	      /* for(p = 0; p < get_measurement_size(); p++ )  */
		/* m = fmax2(m, all_results[p * max_rep_hard_limit + source]); */
		/* add_to_over_time(m); */
		add_to_over_time(previous_interval);
	    }
	    source++;	
	  }
	  if( source >= result_index ) break;
	  {
	    double m = 0.0;
	      for(p = 0; p < get_measurement_size(); p++ ) 
		m = fmax2(m, all_results[p * max_rep_hard_limit + source]);
	      add_to_comm_time(m);  /* @@ actually that's wrong, we should add stop_sync - start_sync  */
	      add_to_wait_time(previous_interval - m);
	  }

	  if( source != dest ) {
	    for( p = 0; p < get_measurement_size(); p++ ) 
	      all_results[p*max_rep_hard_limit + dest] = 
		all_results[p*max_rep_hard_limit + source];
	  }
	  source++;
	  dest++;
        }
	logging(DBG_MEAS, "%d results thrown away\n", source - dest);
	
	result_index = dest;
	logging(DBG_MEAS, "result_index = %d start_not_yet_gathered_results = %d\n",
		result_index, start_not_yet_gathered_results);

	add_to_over_time(global_stop_batch - start_batch - previous_max_counter*previous_interval);
      }
      DEBUG(DBG_MEAS,
      {
        for( i = start_not_yet_gathered_results; i < result_index; i++ ) {
          logging(DBG_MEAS, "all_results[%d] = [%9.1f", i, all_results[0*max_rep_hard_limit +i]*1.0e6);
          for( p = 1; p < get_measurement_size(); p++)
            logging(DBG_MEAS, " %9.1f", all_results[p*max_rep_hard_limit + i]*1.0e6);
          logging(DBG_MEAS, "]\n");
        }
      });


      update_std_error(start_not_yet_gathered_results, result_index);
      /* eigentlich update_std_error(start_not_yet_gathered_results, 
	 min(result_index, max_results)); ???!!! @@@@ */

      stop = result_index >= max_repetitions ||
	     (result_index >= min_repetitions &&
	      std_error <= max_relative_standard_error*mean_value);
    } 
    MPI_Bcast(&result_index, 1, MPI_INT, 0, get_measurement_comm());
    start_not_yet_gathered_results = result_index;
    MPI_Bcast(&stop, 1, MPI_INT, 0, get_measurement_comm());      /* @@@@ optimize */
    MPI_Bcast(&max_counter, 1, MPI_INT, 0, get_measurement_comm()); 
    MPI_Bcast(&interval, 1, MPI_DOUBLE, 0, get_measurement_comm());
    
    if( current_synchronization == SYNC_REAL ) {
      if( lrootproc() ) start_batch = wtime();
      /* @@ actually it should be 

         if( lrootproc() )
           start_batch = wtime() + timeof(Bcast))+overhead 
     
           and transform "counter + 1" to "counter" and "max_counter +
           1" to "max_counter" respectively in should_wait_till,
           start_synchronization, update_batch_params */
      MPI_Bcast(&start_batch, 1, MPI_DOUBLE, 0, get_measurement_comm());
    }

  } while( !stop );


  if( lrootproc() ) {
    /* output results */
    extract_final_results(result_index);
    for( i = 0; i < loopvar_stack_sp; i++) {  /* print set of parameters */
      print_output("%s= ", loopvar_stack[i]->name);
      print_output_variable_value(loopvar_stack[i]);
      print_output(" ");
    }
    print_output("%8d %9.1f %9.1f %8d", get_reported_message_size(), 
		 final_result*1.0e6, std_error*1.0e6, result_index);
    
    if ( max_nr_node_times > 0 ) {
      if ( get_measurement_size() <= max_nr_node_times ) {   
        for( i = 0; i < get_measurement_size(); i++) {
          if( final_results[i] >= 0.0 ) 
            print_output(" %9.1f", final_results[i]*1.0e6);
          else
            print_output("    ---   ");
        }
      } else {
        quantiles = extract_quantiles();
        for ( i = 0; i < max_nr_node_times; i++) {
          if ( quantiles[i] >= 0.0 ) 
            print_output(" %9.1f", quantiles[i]*1.0e6);
          else
            print_output("    ---   ");
        }
        free(quantiles);
      }
    }
    print_output("\n");
    register_measurement_result(final_result);

    free(all_results);
    free(global_invalid);
    free(recv_counts);
    free(recv_displs);
    free(max_results);
    free(final_results);
  }
  free(invalid);
  free(local_results);
}


int execute_measurement(struct statement *s)
{
  struct variable comm;
  /*   struct variable init_error_code; */
  int meas_buffer_too_small, global_meas_buffer_too_small;

  global_meas_buffer_too_small = False;
  init_struct_variable(&comm, TYPE_COMM, NULL, NULL);
  evaluate_term(&comm, s->u.measure.comm);
  assert( comm.type == TYPE_COMM );
  if( comm.u.commv != MPI_COMM_NULL ) { 

    if( lrootproc() ) acc_start_total();

    update_measurement_comm(comm.u.commv);

    set_send_buffer_usage(0);
    set_recv_buffer_usage(0);
    set_send_buffer_slice_usage(0);
    set_recv_buffer_slice_usage(0);
    set_reported_message_size(0);

    current_synchronization = SYNC_NOSYNC;
    max_counter = min_repetitions;

    evaluate_params(s->u.measure.call->interface,
		    s->u.measure.call->params, meas_params);
    if( s->u.measure.call->init_fp != NULL )
      s->u.measure.call->init_fp(NULL, meas_params);
    /*     assert( init_error_code.type == TYPE_INTEGER ); 
	   if( init_error_code.u.intv != MEAS_OK ) return; */

    meas_buffer_too_small = is_buffer_too_small();

    MPI_Allreduce(&meas_buffer_too_small, &global_meas_buffer_too_small, 1, 
		  MPI_INT, MPI_MAX, get_measurement_comm());

    if (lrootproc() && just_entered_measurement_block && global_meas_buffer_too_small) {
      error_without_abort("Warning !?\n"
                          "   No measurements executed in this block.\n"
                          "   Current skampi_buffer_size is %d.\n"
                          "   This value is too small to do even a single measurement.\n"
                          "   Use set_skampi_buffer( <nr_of_bytes> ) in the .ski file to increase it.\n"
                          , skampi_buffer_size);
    }
    else {
      just_entered_measurement_block = False;
    }

    if( !global_meas_buffer_too_small )
      measurement_loop(s->u.measure.call);

    if( s->u.measure.call->finalize_fp != NULL ) 
      s->u.measure.call->finalize_fp(NULL, meas_params);
  
    if( lrootproc() ) acc_stop_total();
  }
  flush_output();
  decrease_reference(&comm);
  if( global_meas_buffer_too_small ) return MEAS_BUFFER_TOO_SMALL;

  return MEAS_OK;
}
