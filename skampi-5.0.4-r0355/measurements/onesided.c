/* SKaMPI MPI-Benchmark

Copyright 2003-2008 Marc-Oliver Straub
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
#include "../mpiversiontest.h"

#ifdef USE_ONESIDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../misc.h"
#include "../mem.h"
#include "../synchronize.h"

#pragma weak begin_skampi_extensions

#define TIMINGDETAILS 0

/*---------------------------------------------------------------------------*/
/* Globals for onesided measurements                                         */
/*---------------------------------------------------------------------------*/
static MPI_Win      onesided_win;
static int          onesided_winsize;
static MPI_Group    onesided_group;
static MPI_Datatype onesided_datatype;

/*---------------------------------------------------------------------------*/
/* Utility routines                                                          */
/*---------------------------------------------------------------------------*/

void func_set_skampi_buffer_mpi_alloc_mem(int size, MPI_Info info) {
  set_skampi_buffer_mpi_alloc_mem(size, info);
}

int func_round_to_fourbytes(int size) {
  
  if (size % 4 == 0) 
    return size;
  
  return (size + 4) - (size % 4);
}

int func_round_to_fivebytes(int size) {
  
  if (size % 4 == 0) 
    return size+1;
  
  return (size + 4) - (size % 4) +1;
}

int wait_for(int us)
{
  int its = 0;
  double seconds, starttime, endtime;

  if (us == 0) 
    return 0;
  its = 0;
  seconds = ((double)us) / 1.0e6;
  starttime = MPI_Wtime();
  endtime = starttime + seconds;
  while(MPI_Wtime() < endtime)
    its++;
  return its;
}

/* Selection of allowed constants for onesided MPI Calls */
int extract_onesided_assertions(int assertions, char* function_call)
{
  int mpiassert = 0;

  /* mask out unspecified assertions */
  if (function_call != NULL) {
    if (strstr(function_call, "MPI_Win_start") != NULL)
      mpiassert &= (MPI_MODE_NOCHECK);
    if (strstr(function_call, "MPI_Win_post") != NULL)
      mpiassert &= (MPI_MODE_NOCHECK | MPI_MODE_NOSTORE | MPI_MODE_NOPUT);
    if (strstr(function_call, "MPI_Win_fence") != NULL)
      mpiassert &= (MPI_MODE_NOPRECEDE | MPI_MODE_NOSUCCEED | MPI_MODE_NOSTORE | MPI_MODE_NOPUT);
    if (strstr(function_call, "MPI_Win_lock") != NULL)
      mpiassert &= (MPI_MODE_NOCHECK);
  }

  return mpiassert;
}

MPI_Group exclude_rank_from_group(int rank, MPI_Group group)
{
  MPI_Group newgroup;
  MPI_Group_excl(group, 1, &rank, &newgroup);

  return newgroup;
}

MPI_Group exclude_all_ranks_except_from_group(int rank, MPI_Group group)
{
  MPI_Group newgroup;
  MPI_Group_incl(group, 1, &rank, &newgroup);

  return newgroup;
}

/*---------------------------------------------------------------------------*/
/* Primitive measurements                                                    */
/*---------------------------------------------------------------------------*/

void init_onesided_dummy(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);
  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_onesided_dummy(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time, end_time;
  
  start_time = start_synchronization();
  end_time = stop_synchronization();

  if (get_measurement_rank()==0)
    return end_time - start_time;
  else
    return -1;
}

/*---------------------------------------------------------------------------*/

void init_Win_fence_open(int count, MPI_Datatype datatype, MPI_Info info, int assertion)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);
  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_fence_open(int count, MPI_Datatype datatype, MPI_Info info, int assertion)
{
  double start_time, end_time;
  
  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOPRECEDE | assertion, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_fence_close(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_fence_close(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  double start_time, end_time;
  
  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE | assertion, onesided_win);
  if (do_a_put)
    MPI_Put(get_send_buffer(), 1, datatype, get_measurement_rank(),
	    0, 1, datatype, onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

void init_Win_fence_close_collective(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  onesided_winsize = get_extent(count, datatype);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(0);
  else
    set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  if (get_measurement_rank() == 0)
    onesided_winsize = get_extent(get_measurement_size()*count, datatype);
  else
    onesided_winsize = 0;
  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_fence_close_collective(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  double start_time, end_time;
  
  MPI_Win_create(get_recv_buffer(), onesided_winsize, get_extent(count, datatype), info, get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE | assertion, onesided_win);
  if (get_measurement_rank() != 0 && do_a_put)
    MPI_Put(get_send_buffer(), count, datatype, 0,
	    get_measurement_rank(), count, datatype, onesided_win);
  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_fence_close_delayed(int count, MPI_Datatype datatype, MPI_Info info, int msg_count, int delay_us)
{
  onesided_winsize = get_extent(count*msg_count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_fence_close_delayed(int count, MPI_Datatype datatype, MPI_Info info, int msg_count, int delay_us)
{
  double start_time, end_time;
  int i;
  int extent, offset;
  
  extent = get_extent(count, datatype);
  MPI_Win_create(get_recv_buffer(), onesided_winsize, extent,
		 info, get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);

  for(i=0; i < msg_count; i++)
    {
      offset = i*extent;
      MPI_Put((char*)get_send_buffer()+offset, count, datatype,
	      (get_measurement_rank() + 1)%get_measurement_size(),
	      i, count, datatype, onesided_win);
    }

  wait_for(delay_us);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_fence_openclose(int count, MPI_Datatype datatype, MPI_Info info, int do_a_put)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_fence_openclose(int count, MPI_Datatype datatype, MPI_Info info, int do_a_put)
{
  double start_time, end_time;
  
  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (do_a_put)
    MPI_Put(get_send_buffer(), 1, datatype, get_measurement_rank(),
	    0, 1, datatype, onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(0, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_start(int count, MPI_Datatype datatype, MPI_Info info, int assertion)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_start(int count, MPI_Datatype datatype, MPI_Info info, int assertion)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);

  mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
  MPI_Win_post(reduced_group, mpiassert, onesided_win);

  if (get_measurement_rank() == 0) {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    start_time = start_synchronization();
    MPI_Win_start(onesided_group, mpiassert, onesided_win);
    end_time = MPI_Wtime();
  }
  else {
    start_synchronization();
  }
  stop_synchronization();

  if (get_measurement_rank() == 0) {
    MPI_Win_complete(onesided_win);
  }

  MPI_Group_free(&reduced_group);
  MPI_Win_wait(onesided_win);

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&onesided_group);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Win_start_delayed_post(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int delay_us, int delay_rank)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_start_delayed_post(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int delay_us, int delay_rank)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);

  mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");

  if (get_measurement_rank() == 0) {
    MPI_Win_post(reduced_group, mpiassert, onesided_win);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    start_time = start_synchronization();
    MPI_Win_start(onesided_group, mpiassert, onesided_win);
    end_time = MPI_Wtime();
  }
  else {
    if (get_measurement_rank() == delay_rank || delay_rank == -1) {
      start_synchronization();
      wait_for(delay_us);
      MPI_Win_post(reduced_group, mpiassert, onesided_win);
    }
    else {
      MPI_Win_post(reduced_group, mpiassert, onesided_win);
      start_synchronization();
    }
  }
  stop_synchronization();

  if (get_measurement_rank() == 0) {
    MPI_Win_complete(onesided_win);
  }

  MPI_Group_free(&reduced_group);
  MPI_Win_wait(onesided_win);

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&onesided_group);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Win_post(int count, MPI_Datatype datatype, MPI_Info info, int assertion)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_post(int count, MPI_Datatype datatype, MPI_Info info, int assertion)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
  /* Assertion MPI_MODE_NOCHECK can be used here, because all MPI_Win_post
     calls will be finished before MPI_Win_start is being called. */
  start_time = start_synchronization();
  MPI_Win_post(onesided_group, mpiassert, onesided_win);
  end_time = stop_synchronization();

  mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
  MPI_Win_start(onesided_group, mpiassert, onesided_win);
  MPI_Win_complete(onesided_win);

  MPI_Win_wait(onesided_win);
  MPI_Win_free(&onesided_win);
  MPI_Group_free(&onesided_group);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_complete(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  onesided_winsize = get_extent(count, datatype);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(onesided_winsize);
  else
    set_send_buffer_usage(0);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_complete(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert, i;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);
  
  if (get_measurement_rank() == 0) {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    reduced_group = exclude_rank_from_group(0, onesided_group);
    MPI_Win_start(reduced_group, mpiassert, onesided_win);
    if (do_a_put)
      for(i = 1; i < get_measurement_size(); i++)
	MPI_Put(get_send_buffer(), count, datatype, i,
		0, count, datatype, onesided_win);
    start_time = start_synchronization();
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    MPI_Win_post(reduced_group, mpiassert, onesided_win);

    start_synchronization();
    MPI_Win_wait(onesided_win);
    stop_synchronization();
  }
  
  MPI_Win_free(&onesided_win);
  MPI_Group_free(&onesided_group);
  MPI_Group_free(&reduced_group);

  return end_time - start_time;
}

void init_Win_startcomplete(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  onesided_winsize = get_extent(count, datatype);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(onesided_winsize);
  else
    set_send_buffer_usage(0);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_startcomplete(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert, i;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);
  
  if (get_measurement_rank() == 0) {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    reduced_group = exclude_rank_from_group(0, onesided_group);
    start_time = start_synchronization();
    MPI_Win_start(reduced_group, mpiassert, onesided_win);
    if (do_a_put)
      for(i = 1; i < get_measurement_size(); i++)
	MPI_Put(get_send_buffer(), count, datatype, i,
		0, count, datatype, onesided_win);
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    start_synchronization();
    MPI_Win_post(reduced_group, mpiassert, onesided_win);
    MPI_Win_wait(onesided_win);
    stop_synchronization();
  }
  
  MPI_Win_free(&onesided_win);
  MPI_Group_free(&onesided_group);
  MPI_Group_free(&reduced_group);

  return end_time - start_time;
}


void init_Win_complete_n(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int msg_count, int delay_us)
{
  onesided_winsize = get_extent(count, datatype);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(onesided_winsize);
  else
    set_send_buffer_usage(0);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(msg_count * onesided_winsize);

  init_synchronization();
}

double measure_Win_complete_n(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int msg_count, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert, i;
  int msize = get_extent(count, datatype);

  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize * msg_count, msize, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);
  
  if (get_measurement_rank() == 0) {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    reduced_group = exclude_rank_from_group(0, onesided_group);
    MPI_Win_start(reduced_group, mpiassert, onesided_win);
    while(--msg_count >= 0)
      for(i = 1; i < get_measurement_size(); i++)
	MPI_Put(get_send_buffer(), count, datatype, i,
		msg_count, count, datatype, onesided_win);
    wait_for(delay_us);
    start_time = start_synchronization();
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    MPI_Win_post(reduced_group, mpiassert, onesided_win);

    start_synchronization();
    MPI_Win_wait(onesided_win);
    stop_synchronization();
  }
  
  MPI_Win_free(&onesided_win);
  MPI_Group_free(&onesided_group);
  MPI_Group_free(&reduced_group);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_complete_delayed_wait(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us)
{
  onesided_winsize = get_extent(count, datatype);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(onesided_winsize);
  else
    set_send_buffer_usage(0);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_complete_delayed_wait(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert, i;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    reduced_group = exclude_rank_from_group(0, onesided_group);
    MPI_Win_start(reduced_group, mpiassert, onesided_win);

    if (do_a_put)
      for(i = 1; i < get_measurement_size(); i++)
	MPI_Put(get_send_buffer(), count, datatype, i,
		0, count, datatype, onesided_win);

    start_time = start_synchronization();
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    MPI_Win_post(reduced_group, mpiassert, onesided_win);

    start_synchronization();
    if (delay_us)
      wait_for(delay_us);
    MPI_Win_wait(onesided_win);
    stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&onesided_group);
  MPI_Group_free(&reduced_group);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_wait_early_complete(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us)
{
  set_reported_message_size(get_extent(count, datatype));

  if (get_measurement_rank() == 0)
    onesided_winsize = get_extent(get_measurement_size()*count, datatype);
  else
    onesided_winsize = 0;

  set_recv_buffer_usage(onesided_winsize);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(0);
  else
    set_send_buffer_usage(get_extent(count, datatype));

  init_synchronization();
}

double measure_Win_wait_early_complete(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, get_extent(count,datatype), info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_rank_from_group(0, onesided_group);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
    MPI_Win_post(reduced_group, mpiassert, onesided_win);

    start_time = start_synchronization();
    if (delay_us > 0)
      wait_for(delay_us);
    start_time = MPI_Wtime();
    MPI_Win_wait(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    MPI_Win_start(reduced_group, mpiassert, onesided_win);
    if (do_a_put)
      MPI_Put(get_send_buffer(), count, datatype, 0, get_measurement_rank(),
	      count, datatype, onesided_win);
    start_synchronization();
    MPI_Win_complete(onesided_win);
    stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1;
}

/*---------------------------------------------------------------------------*/

void init_Win_wait_delayed_complete(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us)
{
  set_reported_message_size(get_extent(count, datatype));

  if (get_measurement_rank() == 0)
    onesided_winsize = get_extent(get_measurement_size()*count, datatype);
  else
    onesided_winsize = 0;

  set_recv_buffer_usage(onesided_winsize);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(0);
  else
    set_send_buffer_usage(get_extent(count, datatype));

  init_synchronization();
}

double measure_Win_wait_delayed_complete(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, get_extent(count,datatype), info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_rank_from_group(0, onesided_group);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
    MPI_Win_post(reduced_group, mpiassert, onesided_win);
    start_time = start_synchronization();
    MPI_Win_wait(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    MPI_Win_start(reduced_group, mpiassert, onesided_win);
    if (do_a_put)
      MPI_Put(get_send_buffer(), count, datatype, 0, get_measurement_rank(),
	      count, datatype, onesided_win);
    
    start_synchronization();
    if (delay_us)
      wait_for(delay_us);
    MPI_Win_complete(onesided_win);
    stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_test(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  set_reported_message_size(get_extent(count, datatype));

  if (get_measurement_rank() == 0)
    onesided_winsize = get_extent(get_measurement_size()*count, datatype);
  else
    onesided_winsize = 0;

  set_recv_buffer_usage(onesided_winsize);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(0);
  else
    set_send_buffer_usage(get_extent(count, datatype));

  init_synchronization();
}

double measure_Win_test(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert, flag;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, get_extent(count,datatype), info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_rank_from_group(0, onesided_group);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
    MPI_Win_post(reduced_group, mpiassert, onesided_win);

    start_time = start_synchronization();
    MPI_Win_test(onesided_win, &flag);
    end_time = stop_synchronization();
    if (flag == 0)
      MPI_Win_wait(onesided_win);
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    MPI_Win_start(reduced_group, mpiassert, onesided_win);
    if (do_a_put)
      MPI_Put(get_send_buffer(), count, datatype, 0, get_measurement_rank(),
	      count, datatype, onesided_win);
    MPI_Win_complete(onesided_win);
    start_synchronization();
    stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_test_delayed(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us)
{
  set_reported_message_size(get_extent(count, datatype));

  if (get_measurement_rank() == 0)
    onesided_winsize = get_extent(get_measurement_size()*count, datatype);
  else
    onesided_winsize = 0;

  set_recv_buffer_usage(onesided_winsize);

  if (get_measurement_rank() == 0)
    set_send_buffer_usage(0);
  else
    set_send_buffer_usage(get_extent(count, datatype));

  init_synchronization();
}

double measure_Win_test_delayed(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert, flag;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, get_extent(count,datatype), info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_rank_from_group(0, onesided_group);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_post");
    MPI_Win_post(reduced_group, mpiassert, onesided_win);
    start_time = start_synchronization();
    MPI_Win_test(onesided_win, &flag);
    end_time = stop_synchronization();
    if (flag == 0)
      MPI_Win_wait(onesided_win);
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    mpiassert = extract_onesided_assertions(assertion, "MPI_Win_start");
    MPI_Win_start(reduced_group, mpiassert, onesided_win);
    if (do_a_put)
      MPI_Put(get_send_buffer(), count, datatype, 0, get_measurement_rank(),
	      count, datatype, onesided_win);
    
    start_synchronization();
    if (delay_us)
      wait_for(delay_us);
    MPI_Win_complete(onesided_win);
    stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_create(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_create(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  start_time = start_synchronization();
  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  end_time = stop_synchronization();
  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_free(int count, MPI_Datatype datatype, MPI_Info info, int do_a_put)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_free(int count, MPI_Datatype datatype, MPI_Info info, int do_a_put)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (do_a_put)
    MPI_Put(get_send_buffer(), 1, datatype, get_measurement_rank(),
	    0, 1, datatype, onesided_win);

  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  start_time = start_synchronization();
  MPI_Win_free(&onesided_win);
  end_time = stop_synchronization();

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Win_lock(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_lock(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert = extract_onesided_assertions(assertion, "MPI_Win_lock");

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  if (get_measurement_rank() == 0)
    MPI_Win_lock(lock_type, dest_rank, mpiassert, onesided_win);
  end_time = stop_synchronization();

  if (get_measurement_rank() == 0)
    MPI_Win_unlock(dest_rank, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1;
}

/*---------------------------------------------------------------------------*/

void init_Win_lock_desync(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int delay_us)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_lock_desync(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert = extract_onesided_assertions(assertion, "MPI_Win_lock");

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_synchronization();
  wait_for(delay_us);
  start_time = MPI_Wtime();
  if (get_measurement_rank() == 0)
    MPI_Win_lock(lock_type, dest_rank, mpiassert, onesided_win);
  end_time = stop_synchronization();

  if (get_measurement_rank() == 0)
    MPI_Win_unlock(dest_rank, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1;
}

/*---------------------------------------------------------------------------*/

void init_Win_unlock(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int do_a_put, int delay_us)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Win_unlock(int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int do_a_put, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int mpiassert = extract_onesided_assertions(assertion, "MPI_Win_unlock");

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  if (get_measurement_rank() == 0) {
    MPI_Win_lock(lock_type, dest_rank, mpiassert, onesided_win);

    if (do_a_put)
      MPI_Put(get_send_buffer(), count, datatype, dest_rank,
	      0, count, datatype, onesided_win);
  }

  wait_for(delay_us);
  start_time = start_synchronization();
  if (get_measurement_rank() == 0)
    MPI_Win_unlock(dest_rank, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Pingpong_Put_Put(int count, MPI_Datatype datatype, MPI_Info info, int iterations)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Pingpong_Put_Put(int count, MPI_Datatype datatype, MPI_Info info, int iterations)
{
  double start_time = 1.0, end_time = 0.0;
  int i;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  if (iterations<0) {
    return -1.0;   /* indicate that this definitely is an error */
  }
  if (iterations==0) {
    return 0.0;    /* avoid division by zero at the end */
  }

  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  start_time = start_synchronization();
  for (i=0; i<iterations; i++) {
    if (get_measurement_rank() == 0)
      MPI_Put(get_send_buffer(), count, datatype, 1,
              0, count, datatype, onesided_win);
    MPI_Win_fence(0, onesided_win);
    if (get_measurement_rank() == 1)
      MPI_Put(get_recv_buffer(), count, datatype, 0,
              0, count, datatype, onesided_win);
    MPI_Win_fence(0, onesided_win);
  }
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return (end_time - start_time)/iterations;
}

void init_Put_fence_bidirectional(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_fence_bidirectional(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);


  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  start_time = start_synchronization();
  MPI_Put(get_send_buffer(), count, datatype, 1-get_measurement_rank(),
	  0, count, datatype, onesided_win);
  MPI_Win_fence(0, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Put(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_recv_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  init_synchronization();
}

double measure_Put(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);

  start_time = start_synchronization();
  if (get_measurement_rank() == 0)
    MPI_Put(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void init_Isend_callduration(int count, MPI_Datatype datatype)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Isend_callduration(int count, MPI_Datatype datatype)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Request r;
  MPI_Status s;

  if (get_measurement_rank() == 1)
    MPI_Irecv(get_recv_buffer(), count, datatype, 0, 0,
	      get_measurement_comm(),&r);

  start_time = start_synchronization();
  if (get_measurement_rank() == 0)
    MPI_Isend(get_send_buffer(), count, datatype, 1, 0,
	      get_measurement_comm(),&r);
  end_time = stop_synchronization();
  
  MPI_Wait(&r, &s);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Put_dedicated(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_dedicated(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_all_ranks_except_from_group(1, onesided_group);
    MPI_Win_start(reduced_group, 0, onesided_win);

    start_time = start_synchronization();
    MPI_Put(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    MPI_Win_post(reduced_group, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Win_wait(onesided_win);
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  return end_time - start_time;
}
/*---------------------------------------------------------------------------*/

void init_Put_passive(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_passive(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  if (get_measurement_rank() == 0) {
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 1, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Put(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
    MPI_Win_unlock(1, onesided_win);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Put_passive_concurrent(int count, MPI_Datatype datatype, MPI_Info info, int lock_type)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(get_measurement_size() * onesided_winsize);

  init_synchronization();
}

double measure_Put_passive_concurrent(int count, MPI_Datatype datatype, MPI_Info info, int lock_type)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize * get_measurement_size(),
		 onesided_winsize, info, get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  MPI_Win_lock(lock_type, 0, 0, onesided_win);
  MPI_Put(get_send_buffer(), count, datatype, 0,
	  get_measurement_rank(), count, datatype, onesided_win);
  MPI_Win_unlock(0, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Put_fence(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_fence(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank() == 0) {
    start_time = start_synchronization();
    MPI_Put(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
    MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Put_activewait_get(int count, MPI_Info info)
{
  onesided_winsize = get_extent(count, MPI_INT);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_activewait_get(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  unsigned int complete = 0;

  ((int*)get_send_buffer())[count-1]=0xDEADBEEF;
  ((int*)get_recv_buffer())[count-1]=0x0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank() == 0) {
    start_time = start_synchronization();
    MPI_Put(get_send_buffer(), count, MPI_INT, 1,
	    0, count, MPI_INT, onesided_win);
    while(complete != 0xDEADBEEF)
      MPI_Get(&complete, 1, MPI_INT, 1,
	      get_extent(count-1,MPI_INT), 1, MPI_INT, onesided_win);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    end_time = stop_synchronization();
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Put_activewait_twosided(int count, MPI_Info info)
{
  onesided_winsize = get_extent(count, MPI_INT);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_activewait_twosided(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  int complete = 0;
  volatile int* lastitem = &(((int*)get_recv_buffer())[count-1]);
  MPI_Status status;

  ((int*)get_send_buffer())[count-1]=0xDEADBEEF;
  ((int*)get_recv_buffer())[count-1]=0x0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank() == 0) {
    start_time = start_synchronization();
    MPI_Put(get_send_buffer(), count, MPI_INT, 1,
	    0, count, MPI_INT, onesided_win);
    MPI_Recv(&complete,1,MPI_INT,1,0,get_measurement_comm(),&status);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    while(*lastitem != 0xDEADBEEF)
      ;
    MPI_Send((void*)lastitem,1,MPI_INT,0,0,get_measurement_comm());
    end_time = stop_synchronization();
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void init_Put_activewait_twosided_sleep(int count, MPI_Info info, int delay_us)
{
  onesided_winsize = get_extent(count, MPI_INT);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_activewait_twosided_sleep(int count, MPI_Info info, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int complete = 0;
  volatile int* lastitem = &(((int*)get_recv_buffer())[count-1]);
  MPI_Status status;

  ((int*)get_send_buffer())[count-1]=0xDEADBEEF;
  ((int*)get_recv_buffer())[count-1]=0x0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank() == 0) {
    start_time = start_synchronization();
    MPI_Put(get_send_buffer(), count, MPI_INT, 1,
	    0, count, MPI_INT, onesided_win);
    MPI_Recv(&complete,1,MPI_INT,1,0,get_measurement_comm(),&status);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    while(*lastitem != 0xDEADBEEF)
      wait_for(delay_us);
    MPI_Send((void*)lastitem,1,MPI_INT,0,0,get_measurement_comm());
    end_time = stop_synchronization();
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Get_Pingpong(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Get_Pingpong(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  start_time = start_synchronization();
  if (get_measurement_rank() == 0)
    MPI_Get(get_recv_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
  MPI_Win_fence(0, onesided_win);
  if (get_measurement_rank() == 1)
    MPI_Get(get_send_buffer(), count, datatype, 0,
	    0, count, datatype, onesided_win);
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Get_callduration(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Get_callduration(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);

  start_time = start_synchronization();
  if (get_measurement_rank() == 0)
    MPI_Get(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Get_dedicated(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Get_dedicated(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_all_ranks_except_from_group(1, onesided_group);
    MPI_Win_start(reduced_group, 0, onesided_win);

    start_time = start_synchronization();
    MPI_Get(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    MPI_Win_post(reduced_group, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Win_wait(onesided_win);
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Get_passive(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Get_passive(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  if (get_measurement_rank() == 0) {
    MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 1, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Get(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
    MPI_Win_unlock(1, onesided_win);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Get_fence(int count, MPI_Datatype datatype, MPI_Info info)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Get_fence(int count, MPI_Datatype datatype, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank() == 0) {
    start_time = start_synchronization();
    MPI_Get(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
    MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Get_activewait(int count, MPI_Info info)
{
  onesided_winsize = get_extent(count, MPI_INT);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Get_activewait(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  volatile int* lastitem = &(((int*)get_recv_buffer())[count-1]);

  ((int*)get_send_buffer())[count-1]=0xDEADBEEF;
  ((int*)get_recv_buffer())[count-1]=0x0;

  MPI_Win_create(get_send_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank() == 0) {
    start_time = start_synchronization();
    MPI_Get(get_recv_buffer(), count, MPI_INT, 1,
	    0, count, MPI_INT, onesided_win);
    while(*lastitem != 0xDEADBEEF)
      ;
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    end_time = stop_synchronization();
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void init_Get_activewait_sleep(int count, MPI_Info info, int delay_us)
{
  onesided_winsize = get_extent(count, MPI_INT);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Get_activewait_sleep(int count, MPI_Info info, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  volatile int* lastitem = &(((int*)get_recv_buffer())[count-1]);

  ((int*)get_send_buffer())[count-1]=0xDEADBEEF;
  ((int*)get_recv_buffer())[count-1]=0x0;

  MPI_Win_create(get_send_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank() == 0) {
    start_time = start_synchronization();
    MPI_Get(get_recv_buffer(), count, MPI_INT, 1,
	    0, count, MPI_INT, onesided_win);
    while(*lastitem != 0xDEADBEEF)
      wait_for(delay_us);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    end_time = stop_synchronization();
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Accumulate(int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Accumulate(int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_all_ranks_except_from_group(1, onesided_group);
    MPI_Win_start(reduced_group, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Accumulate(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, operation, onesided_win);
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    MPI_Win_post(reduced_group, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Win_wait(onesided_win);
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  return end_time - start_time;
}

void init_Accumulate_activewait_twosided(int count, MPI_Info info)
{
  onesided_winsize = get_extent(count, MPI_INT);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Accumulate_activewait_twosided(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  int complete = 0;
  volatile int* lastitem = &(((int*)get_recv_buffer())[count-1]);
  MPI_Status status;

  ((int*)get_send_buffer())[count-1]=0xDEADBEEF;
  ((int*)get_recv_buffer())[count-1]=0x0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank() == 0) {
    start_time = start_synchronization();
    MPI_Accumulate(get_send_buffer(), count, MPI_INT, 1,
	    0, count, MPI_INT, MPI_SUM, onesided_win);
    MPI_Recv(&complete,1,MPI_INT,1,0,get_measurement_comm(),&status);
    end_time = stop_synchronization();
  }
  else {
    start_time = start_synchronization();
    while(*lastitem != 0xDEADBEEF)
      ;
    MPI_Send((void*)lastitem,1,MPI_INT,0,0,get_measurement_comm());
    end_time = stop_synchronization();
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);

  MPI_Win_free(&onesided_win);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Accumulate_concurrent(int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation, int disjoint)
{
  onesided_winsize = get_extent(count, datatype);
  set_reported_message_size(onesided_winsize);

  if (onesided_winsize == 0)
    onesided_winsize = 1;


  set_send_buffer_usage(onesided_winsize);
  if (get_measurement_rank() == 0)
    set_send_buffer_usage(0);

  set_recv_buffer_usage(onesided_winsize);

  if (get_measurement_rank() == 0) {
    if (disjoint)
      set_recv_buffer_usage((get_measurement_size() - 1) * onesided_winsize);
    else
      set_recv_buffer_usage(onesided_winsize);
  }

  init_synchronization();
}

double measure_Accumulate_concurrent(int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation, int disjoint)
{
  double start_time = 1.0, end_time = 0.0;
  int offset = 0, size = onesided_winsize;
  MPI_Group reduced_group;

  if (disjoint) {
    offset = get_measurement_rank() - 1;
    size *= get_measurement_size() - 1;
  }

  if (get_measurement_rank() == 0)
    MPI_Win_create(get_recv_buffer(), size, onesided_winsize, info,
		   get_measurement_comm(), &onesided_win);
  else
    MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		   get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_rank_from_group(0, onesided_group);
    MPI_Win_post(reduced_group, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Win_wait(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    MPI_Win_start(reduced_group, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Accumulate(get_send_buffer(), count, datatype, 0,
		   offset, count, datatype, operation,
		   onesided_win);
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Caching(int count, MPI_Datatype datatype, MPI_Info info, int delay_us)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Caching(int count, MPI_Datatype datatype, MPI_Info info, int delay_us)
{
  double start_time = 1.0, end_time = 0.0;
  int rank;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  start_synchronization();
  if (get_measurement_rank() == 0) {
    for(rank = 1; rank < get_measurement_size(); rank++) {
      MPI_Put(get_send_buffer(), count, datatype, rank,
	      0, count, datatype, onesided_win);
    }
    wait_for(delay_us);
  }

  start_time = MPI_Wtime();
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();
  
  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_Combining_Put_fence(int count, MPI_Datatype datatype, MPI_Info info, int msg_count)
{
  onesided_winsize = get_extent(msg_count*count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Combining_Put_fence(int count, MPI_Datatype datatype, MPI_Info info, int msg_count)
{
  double start_time = 1.0, end_time = 0.0;
  int i, extent;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  extent = get_extent(count, datatype);

  start_synchronization();
  if (get_measurement_rank() == 0) {
    for(i = 0; i < msg_count; i++) {
      MPI_Put((char*)get_send_buffer()+i*extent, count, datatype, 1,
	      i*extent, count, datatype, onesided_win);
    }
  }

  start_time = MPI_Wtime();
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();
  
  MPI_Win_free(&onesided_win);
  
  return end_time - start_time;
}

void init_Combining_Put_dedicated(int count, MPI_Datatype datatype, MPI_Info info, int msg_count)
{
  onesided_winsize = get_extent(msg_count*count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Combining_Put_dedicated(int count, MPI_Datatype datatype, MPI_Info info, int msg_count)
{
  double start_time = 1.0, end_time = 0.0;
  int i, extent;

  MPI_Group reduced_group;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_get_group(onesided_win, &onesided_group);

  extent = get_extent(count, datatype);

  if (get_measurement_rank() == 0) {
    reduced_group = exclude_all_ranks_except_from_group(1, onesided_group);
    MPI_Win_start(reduced_group, 0, onesided_win);

    start_time = start_synchronization();
    for(i = 0; i < msg_count; i++) {
      MPI_Put((char*)get_send_buffer()+i*extent, count, datatype, 1,
	      i*extent, count, datatype, onesided_win);
    }
    MPI_Win_complete(onesided_win);
    end_time = stop_synchronization();
  }
  else {
    reduced_group = exclude_all_ranks_except_from_group(0, onesided_group);
    MPI_Win_post(reduced_group, 0, onesided_win);
    start_time = start_synchronization();
    MPI_Win_wait(onesided_win);
    end_time = stop_synchronization();
  }

  MPI_Win_free(&onesided_win);
  MPI_Group_free(&reduced_group);
  MPI_Group_free(&onesided_group);

  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

MPI_Datatype create_complex_datatype(void)
{
#define COMPLEX_DT_ELEM_COUNT 600
  int blocklens[COMPLEX_DT_ELEM_COUNT];
  MPI_Aint displ[COMPLEX_DT_ELEM_COUNT];
  MPI_Datatype types[COMPLEX_DT_ELEM_COUNT];
  int i;
  MPI_Datatype datatype;

  for(i=0;i<COMPLEX_DT_ELEM_COUNT;i++) {
    blocklens[i]=2;
    displ[i]=i*sizeof(int)*2;
    types[i]=MPI_INT;
  }

  MPI_Type_struct(COMPLEX_DT_ELEM_COUNT,blocklens,displ,types,&datatype);

  return datatype;
}

MPI_Datatype create_complex_mixed_datatype(void)
{
#define COMPLEX_DT_MIXED_ELEM_COUNT 200
  int blocklens[2*COMPLEX_DT_MIXED_ELEM_COUNT];
  MPI_Aint displ[2*COMPLEX_DT_MIXED_ELEM_COUNT];
  MPI_Datatype types[2*COMPLEX_DT_MIXED_ELEM_COUNT];
  int i;
  int disp = 0;
  MPI_Datatype datatype;

  for(i=0;i<COMPLEX_DT_MIXED_ELEM_COUNT;i++) {
    blocklens[2*i]=2;
    displ[2*i]=disp;
    disp += 2*sizeof(int);
    types[2*i]=MPI_INT;
    blocklens[2*i+1]=2;
    displ[2*i+1]=disp;
    disp += 2*sizeof(double);
    types[2*i+1]=MPI_DOUBLE;
  }

  MPI_Type_struct(COMPLEX_DT_MIXED_ELEM_COUNT*2,blocklens,displ,types,&datatype);

  return datatype;
}

void init_Datatype_complex_everytime(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    MPI_Datatype datatype;
    datatype = create_complex_datatype();
    onesided_winsize = get_extent(count, datatype);
    MPI_Type_free(&datatype);
  }

  MPI_Bcast(&onesided_winsize, 1, MPI_INT, 0, get_measurement_comm());

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Datatype_complex_everytime(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Datatype datatype;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  if (get_measurement_rank() == 0) {
    datatype = create_complex_datatype();
    MPI_Type_commit(&datatype);
  }

  start_time = start_synchronization();
  if (get_measurement_rank() == 0) {
    MPI_Put(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  if (get_measurement_rank() == 0) {
    MPI_Type_free(&datatype);
  }
  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void init_Datatype_complex_once(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    onesided_datatype = create_complex_datatype();
    MPI_Type_commit(&onesided_datatype);
    onesided_winsize = get_extent(count, onesided_datatype);
  }

  MPI_Bcast(&onesided_winsize, 1, MPI_INT, 0, get_measurement_comm());

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Datatype_complex_once(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  start_time = start_synchronization();
  if (get_measurement_rank() == 0) {
    MPI_Put(get_send_buffer(), count, onesided_datatype, 1,
	    0, count, onesided_datatype, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void finalize_Datatype_complex_once(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    MPI_Type_free(&onesided_datatype);
  }
}

void init_Datatype_complex_mixed_everytime(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    MPI_Datatype datatype;
    datatype = create_complex_mixed_datatype();
    onesided_winsize = get_extent(count, datatype);
    MPI_Type_free(&datatype);
  }

  MPI_Bcast(&onesided_winsize, 1, MPI_INT, 0, get_measurement_comm());

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Datatype_complex_mixed_everytime(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Datatype datatype;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  if (get_measurement_rank() == 0) {
    datatype = create_complex_mixed_datatype();
    MPI_Type_commit(&datatype);
  }

  start_time = start_synchronization();
  if (get_measurement_rank() == 0) {
    MPI_Put(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  if (get_measurement_rank() == 0) {
    MPI_Type_free(&datatype);
  }
  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void init_Datatype_complex_mixed_once(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    onesided_datatype = create_complex_mixed_datatype();
    MPI_Type_commit(&onesided_datatype);
    onesided_winsize = get_extent(count, onesided_datatype);
  }

  MPI_Bcast(&onesided_winsize, 1, MPI_INT, 0, get_measurement_comm());

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Datatype_complex_mixed_once(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  start_time = start_synchronization();
  if (get_measurement_rank() == 0) {
    MPI_Put(get_send_buffer(), count, onesided_datatype, 1,
	    0, count, onesided_datatype, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void finalize_Datatype_complex_mixed_once(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    MPI_Type_free(&onesided_datatype);
  }
}

void init_Datatype_complex_Get(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    MPI_Datatype datatype;
    datatype = create_complex_datatype();
    onesided_winsize = get_extent(count, datatype);
    MPI_Type_free(&datatype);
  }

  MPI_Bcast(&onesided_winsize, 1, MPI_INT, 0, get_measurement_comm());

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Datatype_complex_Get(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Datatype datatype;

  MPI_Win_create(get_send_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  if (get_measurement_rank() == 0) {
    datatype = create_complex_datatype();
    MPI_Type_commit(&datatype);
  }

  start_time = start_synchronization();
  if (get_measurement_rank() == 0) {
    MPI_Get(get_recv_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  if (get_measurement_rank() == 0) {
    MPI_Type_free(&datatype);
  }
  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void init_Datatype_int(int count, MPI_Info info)
{
  onesided_winsize = get_extent(count*2*COMPLEX_DT_ELEM_COUNT, MPI_INT);
    
  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Datatype_int(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  start_time = start_synchronization();
  if (get_measurement_rank() == 0) {
    MPI_Put(get_send_buffer(), count*2*COMPLEX_DT_ELEM_COUNT, MPI_INT, 1,
	    0, count*2*COMPLEX_DT_ELEM_COUNT, MPI_INT, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

MPI_Datatype create_simple_datatype(void)
{
#define SIMPLE_DT_ELEM_COUNT COMPLEX_DT_ELEM_COUNT*2
  MPI_Datatype datatype;

  MPI_Type_vector(1, SIMPLE_DT_ELEM_COUNT,0,MPI_INT,&datatype);

  return datatype;
}

void init_Datatype_simple(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    MPI_Datatype datatype;
    datatype = create_simple_datatype();
    onesided_winsize = get_extent(count, datatype);
    MPI_Type_free(&datatype);
  }

  MPI_Bcast(&onesided_winsize, 1, MPI_INT, 0, get_measurement_comm());

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Datatype_simple(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Datatype datatype;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  if (get_measurement_rank() == 0) {
    datatype = create_simple_datatype();
    MPI_Type_commit(&datatype);
  }

  start_time = start_synchronization();
  if (get_measurement_rank() == 0) {
    MPI_Put(get_send_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  if (get_measurement_rank() == 0) {
    MPI_Type_free(&datatype);
  }
  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

void init_Datatype_simple_Get(int count, MPI_Info info)
{
  if (get_measurement_rank() == 0) {
    MPI_Datatype datatype;
    datatype = create_simple_datatype();
    onesided_winsize = get_extent(count, datatype);
    MPI_Type_free(&datatype);
  }

  MPI_Bcast(&onesided_winsize, 1, MPI_INT, 0, get_measurement_comm());

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Datatype_simple_Get(int count, MPI_Info info)
{
  double start_time = 1.0, end_time = 0.0;
  MPI_Datatype datatype;

  MPI_Win_create(get_send_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);
  MPI_Win_fence(0, onesided_win);

  if (get_measurement_rank() == 0) {
    datatype = create_simple_datatype();
    MPI_Type_commit(&datatype);
  }

  start_time = start_synchronization();
  if (get_measurement_rank() == 0) {
    MPI_Get(get_recv_buffer(), count, datatype, 1,
	    0, count, datatype, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  if (get_measurement_rank() == 0) {
    MPI_Type_free(&datatype);
  }
  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/
/* Additional measurements                                                   */
/*---------------------------------------------------------------------------*/

void init_Put_Shift(int count, MPI_Datatype datatype, MPI_Info info, int distance)
{
  onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_Shift(int count, MPI_Datatype datatype, MPI_Info info, int distance)
{
  double start_time = 1.0, end_time = 0.0;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int dstrank = (rank+size+distance)%size;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  MPI_Put(get_send_buffer(), count, datatype, dstrank,
	  0, count, datatype, onesided_win);
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Exchange(int count, MPI_Datatype datatype, int msg_count)
{
  onesided_winsize = get_extent(count*msg_count, datatype);

  set_send_buffer_usage(get_extent(count, datatype));
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Exchange(int count, MPI_Datatype datatype, int msg_count)
{
  double start_time = 1.0, end_time = 0.0;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int dstrank = rank, srcrank = rank;
  int extent = get_extent(count, datatype);
  int displacement, msg;
  MPI_Request *rreq, *sreq;

  rreq = skampi_malloc_reqs(msg_count);
  sreq = skampi_malloc_reqs(msg_count);
  start_time = start_synchronization();
  for(msg=0; msg < msg_count; msg++)
    {
      displacement = msg*extent;
      srcrank = (srcrank-1+size)%size;
      MPI_Irecv((char*)get_recv_buffer()+displacement, count, datatype,
		srcrank, 0, get_measurement_comm(), &rreq[msg]);
      dstrank = (dstrank+1)%size;
      MPI_Isend(get_send_buffer(), count, datatype,
		dstrank, 0, get_measurement_comm(), &sreq[msg]);
    }
  MPI_Waitall(msg_count, rreq, MPI_STATUSES_IGNORE);
  MPI_Waitall(msg_count, sreq, MPI_STATUSES_IGNORE);
  end_time = stop_synchronization();

  free(rreq);
  free(sreq);
  
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Put_Exchange(int count, MPI_Datatype datatype, MPI_Info info, int msg_count)
{
  onesided_winsize = get_extent(count*msg_count, datatype);

  set_send_buffer_usage(get_extent(count, datatype));
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_Exchange(int count, MPI_Datatype datatype, MPI_Info info, int msg_count)
{
  double start_time = 1.0, end_time = 0.0;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int dstrank = rank;
  int extent = get_extent(count, datatype);
  int displacement, msg;

  MPI_Win_create(get_send_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  for(msg=0; msg < msg_count; msg++)
    {
      displacement = msg*extent;
      dstrank = (dstrank+1)%size;
      MPI_Put(get_send_buffer(), count, datatype, dstrank,
	      displacement, count, datatype, onesided_win);
    }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Put_Exchange_passive(int count, MPI_Datatype datatype, MPI_Info info, int msg_count)
{
  onesided_winsize = get_extent(count*msg_count, datatype);

  set_send_buffer_usage(get_extent(count, datatype));
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Put_Exchange_passive(int count, MPI_Datatype datatype, MPI_Info info, int msg_count)
{
  double start_time = 1.0, end_time = 0.0;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int dstrank = rank;
  int extent = get_extent(count, datatype);
  int displacement, msg;

  MPI_Win_create(get_send_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  for(msg=0; msg < msg_count; msg++)
    {
      displacement = msg*extent;
      dstrank = (dstrank+1)%size;
      MPI_Win_lock(MPI_LOCK_SHARED, dstrank, 0, onesided_win);
      MPI_Put(get_send_buffer(), count, datatype, dstrank,
	      displacement, count, datatype, onesided_win);
      MPI_Win_unlock(dstrank, onesided_win);
    }
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_Accumulate_concurrent_multi(int count, MPI_Datatype datatype, MPI_Info info, int msg_count, MPI_Op operator, int disjoint)
{
  if(disjoint)
    onesided_winsize = get_extent(count*msg_count, datatype);
  else
    onesided_winsize = get_extent(count, datatype);

  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(get_extent(count*msg_count, datatype));

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_Accumulate_concurrent_multi(int count, MPI_Datatype datatype, MPI_Info info, int msg_count, MPI_Op operator, int disjoint)
{
  double start_time = 1.0, end_time = 0.0;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int dstrank = rank;
  int extent = get_extent(count, datatype);
  int displacement, msg;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  for(msg=0; msg < msg_count; msg++)
    {
      if (disjoint)
	displacement = msg*extent;
      else
	displacement = 0;
      dstrank = (dstrank+1)%size;
      MPI_Accumulate(get_send_buffer(), count, datatype, dstrank,
	      displacement, count, datatype, operator, onesided_win);
    }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_onesided_bcast(int count, MPI_Datatype datatype, MPI_Info info, int bcast_count)
{
  onesided_winsize = get_extent(bcast_count * count, datatype);

  set_send_buffer_usage(get_extent(count, datatype));
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_onesided_bcast(int count, MPI_Datatype datatype, MPI_Info info, int bcast_count)
{
  double start_time = 1.0, end_time = 0.0;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int i,dstrank=rank;
  int j;
  MPI_Win_create(get_recv_buffer(), onesided_winsize,
		 get_extent(count, datatype), info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  if (get_measurement_rank()==0)
    {
      for(j=0; j < bcast_count; j++)
	for(i=0; i <= size; i++)
	  {
	    dstrank = (dstrank+1)%size;
	    MPI_Put(get_send_buffer(), count, datatype, dstrank,
		    j, count, datatype, onesided_win);
	  }
    }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  if (get_measurement_rank() == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/*---------------------------------------------------------------------------*/

void init_onesided_alltoall(int count, MPI_Datatype datatype, MPI_Info info, int alltoall_count)
{
  int size = get_measurement_size();
  onesided_winsize = get_extent(count*size, datatype);

  set_send_buffer_usage(onesided_winsize);

  onesided_winsize = get_extent(count*size*alltoall_count, datatype);
  set_reported_message_size(onesided_winsize);

  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_onesided_alltoall(int count, MPI_Datatype datatype, MPI_Info info, int alltoall_count)
{
  double start_time = 1.0, end_time = 0.0;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int i,j,offset,dstrank=rank;
  int msgsize = get_extent(count, datatype);

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  for(j=0; j < alltoall_count; j++) {
    offset = j * size * msgsize;
    for(i=0; i <= size; i++)
      {
	dstrank = (dstrank+1)%size;
	MPI_Put((char*)get_send_buffer()+(msgsize*dstrank), count, datatype,
		dstrank, offset+rank*msgsize, count, datatype, onesided_win);
      }
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  return end_time - start_time;
}

void init_multi_Alltoall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int alltoall_count)
{
  set_send_buffer_usage(get_measurement_size()*get_extent(send_count, send_dt));
  set_recv_buffer_usage(get_measurement_size()*get_extent(recv_count, recv_dt)*alltoall_count);
  set_reported_message_size(get_extent(send_count, send_dt)*alltoall_count);
  init_synchronization();
}

double measure_multi_Alltoall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int alltoall_count)
{
  double start_time, end_time;
  int j, offset;

  start_time = start_synchronization();
  for(j = 0 ; j < alltoall_count; j++) {
    offset = j * get_measurement_size()*get_extent(recv_count, recv_dt);
    MPI_Alltoall(get_send_buffer(), send_count, send_dt,
		 (char*)get_recv_buffer()+offset, recv_count, recv_dt, get_measurement_comm());
  }
  end_time = stop_synchronization();
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

void init_onesided_reduce(int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op)
{
  onesided_winsize = get_extent(count, datatype);
  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);
  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_onesided_reduce(int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op)
{
  double start_time = 1.0, end_time = 0.0;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  MPI_Accumulate(get_send_buffer(), count, datatype, 0,
		0, count, datatype, op, onesided_win);
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  return end_time - start_time;
}

void init_onesided_allreduce(int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op)
{
  onesided_winsize = get_extent(count, datatype);
  set_send_buffer_usage(onesided_winsize);
  set_reported_message_size(onesided_winsize);
  set_recv_buffer_usage(onesided_winsize);

  init_synchronization();
}

double measure_onesided_allreduce(int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op)
{
  double start_time = 1.0, end_time = 0.0;
  int i, dest;

  MPI_Win_create(get_recv_buffer(), onesided_winsize, 1, info,
		 get_measurement_comm(), &onesided_win);

  start_time = start_synchronization();
  MPI_Win_fence(MPI_MODE_NOPRECEDE, onesided_win);
  for(i = 1; i <= get_measurement_size(); i++) {
    dest = (get_measurement_rank() + i) % get_measurement_size();
    MPI_Accumulate(get_send_buffer(), count, datatype, dest,
		   0, count, datatype, op, onesided_win);
  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, onesided_win);
  end_time = stop_synchronization();

  MPI_Win_free(&onesided_win);
  
  return end_time - start_time;
}

/*---------------------------------------------------------------------------*/

static MPI_Win to_win, from_win;
static double *to_buffer, *from_buffer;
static int border_rows, border_cols;
static double t_wait, t_work, t_start;
#if TIMINGDETAILS
static FILE* timingFile;
#endif

void init_onesided_borderexchange(int elemcount, MPI_Info info, int iteration_count, int compute)
{
  int bufsize;
  MPI_Group group;
  int size,rank, ranks[2];
  int rankcount;
#if TIMINGDETAILS
  char timingfname[32];
#endif

  border_rows = sqrt(elemcount/2);
  border_cols = 2*border_rows;
  bufsize = (2+border_rows)*border_cols*sizeof(double);

  /* 2 buffers: old state, new state */
  onesided_winsize = bufsize;
  set_recv_buffer_usage(onesided_winsize);
  set_send_buffer_usage(onesided_winsize);

  /* number of bytes each proc sends */
  set_reported_message_size(2*border_cols*sizeof(double));

  rank = get_measurement_rank();
  size = get_measurement_size();
  ranks[0]=(rank+1)%size;
  ranks[1]=(rank+size-1)%size;
  rankcount = 2;
  if (ranks[0]==ranks[1])
    rankcount=1;
  MPI_Comm_group(get_measurement_comm(), &group);
  MPI_Group_incl(group,rankcount,ranks,&onesided_group);
  MPI_Group_free(&group);

#if TIMINGDETAILS
  Snprintf(timingfname, 32, "border_onesided_rank%d", get_measurement_rank());
  timingFile = fopen(timingfname,"a");
  fprintf(timingFile, "onesided elemcount:%d\n", elemcount);
#endif
  init_synchronization();
}


void onesided_exchange_border_cells(void)
{
  double start;
  /* send cells to upper and lower neighbours */
  double* first_row = &to_buffer[                0 * border_cols];
  double* last_row =  &to_buffer[(border_rows + 1) * border_cols];
  int first_row_displacement =          1 * border_cols;
  int last_row_displacement = border_rows * border_cols;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int next_rank=(rank+1)%size;
  int prev_rank=(rank+size-1)%size;

  start = MPI_Wtime();
  MPI_Win_post(onesided_group, MPI_MODE_NOPUT, to_win);
  MPI_Win_start(onesided_group, 0, to_win);

  MPI_Get(first_row, border_cols, MPI_DOUBLE,
          prev_rank, last_row_displacement,
          border_cols, MPI_DOUBLE, to_win);
  MPI_Get(last_row, border_cols, MPI_DOUBLE,
          next_rank, first_row_displacement,
          border_cols, MPI_DOUBLE, to_win);

  t_start += MPI_Wtime() - start;
}


void onesided_wait_for_border_cell_arrival(void)
{
  double start = MPI_Wtime();
  /* wait until data has arrived */
  MPI_Win_complete(from_win);
  MPI_Win_wait(from_win);
  t_wait += MPI_Wtime() - start;
}

void onesided_exchange_buffers(void)
{
  MPI_Win temp_win;
  double* temp_buffer;

  temp_buffer = to_buffer;
  to_buffer = from_buffer;
  from_buffer = temp_buffer;

  temp_win = to_win;
  to_win = from_win;
  from_win = temp_win;
}

void simulate_outer_cells(int compute)
{
  double start = MPI_Wtime();
  /* simulate buffer lines 1 and N */
  if (compute) {
    int j;
    for(j=0; j < border_cols; j++) {
      to_buffer[1*border_cols+j]=(
        from_buffer[0*border_cols+j]+
        from_buffer[1*border_cols+j]+
        from_buffer[2*border_cols+j])/3;
      to_buffer[border_rows*border_cols+j]=(
        from_buffer[(border_rows-1)*border_cols+j]+
        from_buffer[(border_rows  )*border_cols+j]+
        from_buffer[(border_rows+1)*border_cols+j])/3;
    } 
  }
  t_work += MPI_Wtime() - start;
}

void simulate_inner_cells(int compute)
{
  double start = MPI_Wtime();
  /* simulate buffer lines 2 to N-1 */
  if (compute) {
    int i,j;

    for(i=2; i < border_rows; i++)
      for(j=0; j < border_cols; j++)
        to_buffer[i*border_cols+j]=(
  	from_buffer[(i-1)*border_cols+j]+
  	from_buffer[(i  )*border_cols+j]+
  	from_buffer[(i+1)*border_cols+j])/3;
  }
  t_work += MPI_Wtime() - start;
}


double measure_onesided_borderexchange(int count, MPI_Info info, int iteration_count, int compute)
{
  double start_time = 1.0, end_time = 0.0;
  int iteration;

  from_buffer = (double*) get_send_buffer();
  to_buffer = (double*) get_recv_buffer();
  MPI_Win_create(from_buffer, onesided_winsize, sizeof(double), info,
		 get_measurement_comm(), &from_win);
  MPI_Win_create(to_buffer, onesided_winsize, sizeof(double), info,
		 get_measurement_comm(), &to_win);

  t_start = t_wait = t_work = 0;
  start_time = start_synchronization();

  if (border_rows >= 3 && border_cols > 1)
    {
      onesided_exchange_buffers();
      onesided_exchange_border_cells();
      onesided_exchange_buffers();
      for(iteration = 0; iteration < iteration_count; iteration++)
	{
	  onesided_wait_for_border_cell_arrival();
	  simulate_outer_cells(compute);
	  onesided_exchange_border_cells();
	  simulate_inner_cells(compute);
	  onesided_exchange_buffers();
	}
      onesided_wait_for_border_cell_arrival();
      simulate_outer_cells(compute);
      simulate_inner_cells(compute);
    }

  end_time = stop_synchronization();

#if TIMINGDETAILS
  fprintf(timingFile, "total: %7f start: %7f work: %7f wait: %7f\n", end_time-start_time, t_start, t_work, t_wait);
  fflush(timingFile);
#endif
  MPI_Win_free(&to_win);
  MPI_Win_free(&from_win);
  
  return end_time - start_time;
}


void finalize_onesided_borderexchange(int elemcount, MPI_Info info, int iteration_count, int compute)
{
#if TIMINGDETAILS
  fclose(timingFile);
#endif
  MPI_Group_free(&onesided_group);
}

/*---------------------------------------------------------------------------*/

static MPI_Request twosided_requests[4];
char* sendbuf1,*sendbuf2;

void init_twosided_borderexchange(int elemcount, int iteration_count, int compute)
{
  int bufsize;
#if TIMINGDETAILS
  char timingfname[32];
#endif  
  border_rows = sqrt(elemcount/2);
  border_cols = 2*border_rows;
  bufsize = (4+border_rows)*border_cols*sizeof(double);

  /* 2 buffers: old state, new state */
  onesided_winsize = bufsize;
  set_recv_buffer_usage(onesided_winsize);
  set_send_buffer_usage(onesided_winsize);

  /* number of bytes each proc sends */
  set_reported_message_size(2*border_cols*sizeof(double));

#if TIMINGDETAILS
  snprintf(timingfname, 32, "border_twosided_rank%d", get_measurement_rank());
  timingFile = fopen(timingfname,"a");
  fprintf(timingFile, "onesided elemcount:%d\n", elemcount);
#endif

  init_synchronization();
}

void twosided_exchange_border_cells(void)
{
  double start;

  /* send cells to upper and lower neighbours */
  double* first_row = &to_buffer[                0 * border_cols];
  double* last_row =  &to_buffer[(border_rows + 1) * border_cols];
  double* first_row_displacement = &to_buffer[1 * border_cols];
  double* last_row_displacement = &to_buffer[border_rows * border_cols];
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int next_rank=(rank+1)%size;
  int prev_rank=(rank+size-1)%size;

  start = MPI_Wtime();
  memcpy(sendbuf1, first_row_displacement, border_cols*sizeof(double));
  memcpy(sendbuf2, last_row_displacement, border_cols*sizeof(double));
  MPI_Irecv(first_row, border_cols, MPI_DOUBLE,
	    prev_rank, 0, get_measurement_comm(),
	    &twosided_requests[0]);
  MPI_Irecv(last_row, border_cols, MPI_DOUBLE,
	    next_rank, 1, get_measurement_comm(),
	    &twosided_requests[1]);
  MPI_Isend(sendbuf1, border_cols, MPI_DOUBLE,
	    prev_rank, 1, get_measurement_comm(), 
	    &twosided_requests[2]);
  MPI_Isend(sendbuf2, border_cols, MPI_DOUBLE,
	    next_rank, 0, get_measurement_comm(),
	    &twosided_requests[3]);
  t_start += MPI_Wtime() - start;
}

void twosided_wait_for_border_cell_arrival(void)
{
  double start = MPI_Wtime();
  /* wait until data has arrived */
  MPI_Waitall(4, twosided_requests, MPI_STATUSES_IGNORE);
  t_wait += MPI_Wtime() - start;
}

void twosided_exchange_buffers(void)
{
  double* temp_buffer;

  temp_buffer = to_buffer;
  to_buffer = from_buffer;
  from_buffer = temp_buffer;
}

double measure_twosided_borderexchange(int count, int iteration_count, int compute)
{
  double start_time = 1.0, end_time = 0.0;
  int iteration;

  from_buffer = (double*) get_send_buffer();
  to_buffer = (double*) get_recv_buffer();
  sendbuf1 = (char*) get_send_buffer() + (2+border_rows)*border_cols*sizeof(double);
  sendbuf2 = (char*) get_send_buffer() + (3+border_rows)*border_cols*sizeof(double);

  t_start = t_wait = t_work = 0;
  start_time = start_synchronization();
  
  if (border_rows >= 3 && border_cols > 1)
    {
      twosided_exchange_buffers();
      twosided_exchange_border_cells();
      twosided_exchange_buffers();
      for(iteration = 0; iteration < iteration_count; iteration++)
	{
	  twosided_wait_for_border_cell_arrival();
	  simulate_outer_cells(compute);
	  twosided_exchange_border_cells();
	  simulate_inner_cells(compute);
	  twosided_exchange_buffers();
	}
      twosided_wait_for_border_cell_arrival();
      simulate_outer_cells(compute);
      simulate_inner_cells(compute);
    }

  end_time = stop_synchronization();

#if TIMINGDETAILS
  fprintf(timingFile, "total: %7f start: %7f work: %7f wait: %7f\n", end_time-start_time, t_start, t_work, t_wait);
  fflush(timingFile);
#endif  
  return end_time - start_time;
}

void finalize_twosided_borderexchange(int count, int iteration_count, int compute)
{
#if TIMINGDETAILS
  fclose(timingFile);
#endif
}

/*---------------------------------------------------------------------------*/

void init_onesided_borderexchange_fence(int elemcount, MPI_Info info, int iteration_count, int compute)
{
  int bufsize;

  border_rows = sqrt(elemcount/2);
  border_cols = 2*border_rows;
  bufsize = (2+border_rows)*border_cols*sizeof(double);

  /* 2 buffers: old state, new state */
  onesided_winsize = bufsize;
  set_recv_buffer_usage(onesided_winsize);
  set_send_buffer_usage(onesided_winsize);

  /* number of bytes each proc sends */
  set_reported_message_size(2*border_cols*sizeof(double));

  init_synchronization();
}

void onesided_exchange_border_cells_fence(void)
{
  /* send cells to upper and lower neighbours */
  double* first_row = &to_buffer[                0 * border_cols];
  double* last_row =  &to_buffer[(border_rows + 1) * border_cols];
  int first_row_displacement =          1 * border_cols;
  int last_row_displacement = border_rows * border_cols;
  int rank = get_measurement_rank();
  int size = get_measurement_size();
  int next_rank=(rank+1)%size;
  int prev_rank=(rank+size-1)%size;

  MPI_Win_fence(0, to_win);

  MPI_Get(first_row, border_cols, MPI_DOUBLE,
          prev_rank, last_row_displacement,
          border_cols, MPI_DOUBLE, to_win);
  MPI_Get(last_row, border_cols, MPI_DOUBLE,
          next_rank, first_row_displacement,
          border_cols, MPI_DOUBLE, to_win);
}

void onesided_wait_for_border_cell_arrival_fence(void)
{
  /* wait until data has arrived */
  MPI_Win_fence(0, from_win);
}

double measure_onesided_borderexchange_fence(int count, MPI_Info info, int iteration_count, int compute)
{
  double start_time = 1.0, end_time = 0.0;
  int iteration;

  from_buffer = (double*) get_send_buffer();
  to_buffer = (double*) get_recv_buffer();
  MPI_Win_create(from_buffer, onesided_winsize, sizeof(double), info,
		 get_measurement_comm(), &from_win);
  MPI_Win_create(to_buffer, onesided_winsize, sizeof(double), info,
		 get_measurement_comm(), &to_win);

  start_time = start_synchronization();

  if (border_rows >= 3 && border_cols > 1)
    {
      onesided_exchange_buffers();
      onesided_exchange_border_cells_fence();
      onesided_exchange_buffers();
      for(iteration = 0; iteration < iteration_count; iteration++)
	{
	  onesided_wait_for_border_cell_arrival_fence();
	  simulate_outer_cells(compute);
	  onesided_exchange_border_cells_fence();
	  simulate_inner_cells(compute);
	  onesided_exchange_buffers();
	}
      onesided_wait_for_border_cell_arrival_fence();
      simulate_outer_cells(compute);
      simulate_inner_cells(compute);
    }

  end_time = stop_synchronization();
  MPI_Win_free(&to_win);
  MPI_Win_free(&from_win);
  
  return end_time - start_time;
}

void init_Send(int count, MPI_Datatype datatype, int tag)
{
  MPI_Aint extent;

  extent = get_extent(count, datatype);
  set_send_buffer_usage(extent);
  set_reported_message_size(extent);
  set_recv_buffer_usage(extent);
}

double measure_Send(int count, MPI_Datatype datatype, int tag)
{
  double start_time;
  MPI_Status status;
  
  if( get_measurement_rank() == 0 ) {
    start_time = MPI_Wtime();
    MPI_Ssend(get_send_buffer(), count, datatype, 1, tag, get_measurement_comm());
    return MPI_Wtime() - start_time;
  } else {
    MPI_Recv(get_recv_buffer(), count, datatype, 0, tag, get_measurement_comm(), &status);
    return -1.0;
  }
}

#pragma weak end_skampi_extensions
#endif /* USE_ONESIDED */
