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
#include <mpi.h>
#include <assert.h>

#include "misc.h"
#include "debug.h"
#include "private_misc.h"
#include "output.h"
#include "accounting.h"

/*

  tc_synchronization = 0.0;

  Bestimmung der Zeitdifferenzen
  
  Lauf von determine_time_differences





  tc_communication = 0.0;

  falls Messung gültig, Maximum der Dauer auf den verschiedenen Knoten

  tc_waiting = 0.0;

  falls Messung gültig, Differenz aus Maximum und interval

  tc_late = 0.0;

    falls Messung verspätet angefangen hat, wird um interval erhöht

  tc_too_long = 0.0;

    falls Messung nicht ins vorgegebenen Intervall gepasst hat, wird um
    interval erhoeht




  tc_unnecessary_comm = 0.0;

  falls Messung gültig, aber nicht nötig




  tc_whole_measurement = 0.0;



  tc_root_comm = 0.0;




  tc_whole_col_time = 0.0;

  tc_initialize_data = 0.0;




*/

enum {
  Number_acc_times = 7
};

static double acc_sync_time = 0.0;
static double acc_comm_time = 0.0;
static double acc_late_time = 0.0;
static double acc_over_time = 0.0;
static double acc_unec_time = 0.0;
static double acc_wait_time = 0.0;
static double acc_total_time = 0.0;

static double user_acc_sync_time = -1.0;
static double user_acc_comm_time = -1.0;
static double user_acc_late_time = -1.0;
static double user_acc_over_time = -1.0;
static double user_acc_unec_time = -1.0;
static double user_acc_wait_time = -1.0;
static double user_acc_total_time = -1.0;



#if 0
/* currently the following is  not used everywhere */
static void init_global_accounting(void)
{
}
#endif

void init_user_accounting(void)
{
  user_acc_sync_time = 0.0;
  user_acc_comm_time = 0.0;
  user_acc_late_time = 0.0;
  user_acc_over_time = 0.0;
  user_acc_unec_time = 0.0;
  user_acc_wait_time = 0.0;
  user_acc_total_time = 0.0;
}

void print_global_acc_info(void)
{
  double single_times[Number_acc_times], sums[Number_acc_times];
  double total;
  int i;

  single_times[0] = acc_total_time;
  single_times[1] = acc_comm_time;
  single_times[2] = acc_late_time;
  single_times[3] = acc_over_time;
  single_times[4] = acc_unec_time;
  single_times[5] = acc_wait_time;
  single_times[6] = acc_sync_time;

  MPI_Reduce(single_times, sums, Number_acc_times, MPI_DOUBLE, MPI_SUM, 
	     0, MPI_COMM_WORLD);

  for( i = 1, total = 0.0 ; i < Number_acc_times; i++) total += sums[i];

  if( grootproc() ) {
    print_output("# global accounting info\n");
    print_output("# ----------------------\n");
    print_output("# acc_sync_time   = %7.3f  %4.1f%%\n", sums[6], sums[6]/sums[0]*100.0);
    print_output("# acc_comm_time   = %7.3f  %4.1f%%\n", sums[1], sums[1]/sums[0]*100.0);
    print_output("# acc_wait_time   = %7.3f  %4.1f%%\n", sums[5], sums[5]/sums[0]*100.0);
    /*    print_output("# acc_late_time   = %7.3f  %4.1f%%\n", sums[2], sums[2]/sums[0]*100.0); */
    print_output("# acc_over_time   = %7.3f  %4.1f%%\n", sums[3], sums[3]/sums[6]*100.0);
    /*    print_output("# acc_unec_time   = %7.3f  %4.1f%%\n", sums[4], sums[4]/sums[0]*100.0); */
    print_output("# summed up total = %7.3f  %4.1f%%\n", total  , total  /sums[0]*100.0);
    print_output("# --------------------------------\n");
    print_output("# total time      = %7.3f 100.0%%\n\n", sums[0]);
    print_output("#\n");
  }
}

void print_user_acc_info(void)
{
  double single_times[Number_acc_times], sums[Number_acc_times];
  double total;
  int i;

  single_times[0] = user_acc_total_time;
  single_times[1] = user_acc_comm_time;
  single_times[2] = user_acc_late_time;
  single_times[3] = user_acc_over_time;
  single_times[4] = user_acc_unec_time;
  single_times[5] = user_acc_wait_time;
  single_times[6] = user_acc_sync_time;

  MPI_Reduce(single_times, sums, Number_acc_times, MPI_DOUBLE, MPI_SUM, 
	     0, MPI_COMM_WORLD);

  for( i = 1, total = 0.0 ; i < Number_acc_times; i++) total += sums[i];

  if( grootproc() ) {
    print_output("# user accounting info\n");
    print_output("# --------------------\n");
    print_output("# user_acc_sync_time = %7.3f  %4.1f%%\n", sums[6], sums[6]/sums[0]*100.0);
    print_output("# user_acc_comm_time = %7.3f  %4.1f%%\n", sums[1], sums[1]/sums[0]*100.0);
    print_output("# user_acc_wait_time = %7.3f  %4.1f%%\n", sums[5], sums[5]/sums[0]*100.0);
    /*    print_output("# user_acc_late_time = %7.3f  %4.1f%%\n", sums[2], sums[2]/sums[0]*100.0); */
    print_output("# user_acc_over_time = %7.3f  %4.1f%%\n", sums[3], sums[3]/sums[0]*100.0);
    /*    print_output("# user_acc_unec_time = %7.3f  %4.1f%%\n", sums[4], sums[4]/sums[0]*100.0); */
    print_output("# summed up total    = %7.3f  %4.1f%%\n", total  , total  /sums[0]*100.0);
    print_output("# ------------------------------------\n");
    print_output("# total time         = %7.3f 100.0%%\n", sums[0]);
    print_output("#\n");
  }
}

/*----------------------------------------------------------------------------*/

static double sync_time;

double acc_start_sync(void)
{
  sync_time = MPI_Wtime();
  return sync_time;
}

double acc_stop_sync(void) 
{
  double now;

  now = MPI_Wtime();
  if( acc_sync_time >= 0.0 ) acc_sync_time += now - sync_time;
  if( user_acc_sync_time >= 0.0 ) user_acc_sync_time += now - sync_time;
  return now - sync_time;
}

/*----------------------------------------------------------------------------*/

static double total_time;

double acc_start_total(void)
{
  total_time = MPI_Wtime();
  return total_time;
}

double acc_stop_total(void) 
{
  double now;

  now = MPI_Wtime();
  if( acc_total_time >= 0.0 ) acc_total_time += now - total_time;
  if( user_acc_total_time >= 0.0 ) user_acc_total_time += now - total_time;
  return now - total_time;
}
/*----------------------------------------------------------------------------*/

void add_to_comm_time(double t)
{
  /*   assert( t >= 0.0 ); */
  if( acc_comm_time >= 0.0 ) acc_comm_time += t;
  if( user_acc_comm_time >= 0.0 ) user_acc_comm_time += t;
}  

void add_to_late_time(double t)
{
  /*  assert( t >= 0.0 ); */
  if( acc_late_time >= 0.0 ) acc_late_time += t;
  if( user_acc_late_time >= 0.0 ) user_acc_late_time += t;
}

void add_to_over_time(double t)
{
  /*  assert( t >= 0.0 ); */
  if( acc_over_time >= 0.0 ) acc_over_time += t;
  if( user_acc_over_time >= 0.0 ) user_acc_over_time += t;
}

void add_to_wait_time(double t)
{
  /*  assert( t >= 0.0 ); */
  if( acc_wait_time >= 0.0 ) acc_wait_time += t;
  if( user_acc_wait_time >= 0.0 ) user_acc_wait_time += t;
}

/*----------------------------------------------------------------------------*/

static double unec_time;

double acc_start_unec(void)
{
  unec_time = MPI_Wtime();
  return unec_time;
}

double acc_stop_unec(void) 
{
  double now;

  now = MPI_Wtime();
  if( acc_unec_time >= 0.0 ) acc_unec_time += now - unec_time;
  if( user_acc_unec_time >= 0.0 ) user_acc_unec_time += now - unec_time;
  return now - unec_time;
}

