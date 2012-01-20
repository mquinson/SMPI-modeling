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

void init_user_accounting(void);
void print_user_acc_info(void);
void print_global_acc_info(void);

double acc_start_sync(void);
double acc_stop_sync(void);
double acc_start_total(void);
double acc_stop_total(void);

void add_to_comm_time(double t);
void add_to_late_time(double t);
void add_to_over_time(double t);
void add_to_wait_time(double t);




double acc_start_unec(void);
double acc_stop_unec(void);
