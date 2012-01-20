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

enum sync_type {
  SYNC_NOSYNC  = 600, /* no synchronization for measuring collectives */
  SYNC_BARRIER = 601, /* synchronization with MPI_Barrier */
  SYNC_REAL    = 602  /* measurement with real synchronization using 
                         local clocks and considering their differences
                         if necessary */
};

void print_global_time_differences(void);

void init_synchronization_module(void);

void determine_time_differences(void);

extern double *tds;
extern double *ping_pong_min_time; /* ping_pong_min_time[i] is the minimum time
				      of one ping_pong between the current node 
				      and node i, negative value means time not 
				      yet determined */
extern bool mpi_wtime_is_global;

void init_synchronization(void);
double start_synchronization(void);
double stop_synchronization(void);
void set_synchronization(enum sync_type);

