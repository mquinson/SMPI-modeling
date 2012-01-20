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

#include "mpiversiontest.h"

extern int buffer_cycling;

void init_buffer(void);
void check_buffer(void);
void cycle_buffer(void);
void set_skampi_buffer(int size);
#ifdef USE_ONESIDED
void set_skampi_buffer_mpi_alloc_mem(int size, MPI_Info info);
#endif
void switch_buffer_cycling_on(void);
void switch_buffer_cycling_off(void);
void set_send_buffer_alignment(int a);
void set_recv_buffer_alignment(int a);
bool is_buffer_too_small(void);

void set_send_buffer_slice_usage(MPI_Aint n);
void set_recv_buffer_slice_usage(MPI_Aint n);
MPI_Aint get_send_buffer_slice_usage(void);
MPI_Aint get_recv_buffer_slice_usage(void);

char *get_send_buffer_slice(void);
char *get_recv_buffer_slice(void);
MPI_Aint get_send_buffer_min_size(MPI_Aint slice_size, int n);
MPI_Aint get_recv_buffer_min_size(MPI_Aint slice_size, int n);
