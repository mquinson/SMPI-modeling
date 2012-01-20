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


enum {
  True = 1,
  False = 0
};

typedef int bool;


int get_my_global_rank(void);
int get_global_rank(int m_rank);
int get_global_size(void);

int get_measurement_rank(void);
int get_measurement_size(void);
MPI_Comm get_measurement_comm(void);


char *get_send_buffer(void);
char *get_recv_buffer(void);
void set_send_buffer_usage(MPI_Aint n);
MPI_Aint get_send_buffer_usage(void);
void set_recv_buffer_usage(MPI_Aint n);
MPI_Aint get_recv_buffer_usage(void);
void set_reported_message_size(MPI_Aint n);
MPI_Aint get_reported_message_size(void);

int imax2(int a, int b);
int imin2(int a, int b);
int imax3(int a, int b, int c);
double fmin2(double a, double b);
double fmax2(double a, double b);
int isqr(int a);
double fsqr(double a);
int gcd(int a, int b);
int lcm(int a, int b);
int round_int_to_multipleof(int i, int m);
int round_double_to_multipleof(double f, int m);

/*---------------------------------------------------------------------------*/

extern int skampi_buffer_size; /* in bytes */
extern int skampi_cache_size;  /* in bytes */

extern MPI_Aint get_extent(int count, MPI_Datatype datatype);
extern MPI_Aint get_true_extent(int count, MPI_Datatype datatype);

/*---------------------------------------------------------------------------*/

void set_mpi_alloc_mem_use(int flag);
int  get_mpi_alloc_mem_use(void);

#ifdef USE_MPI_ALLOC_MEM
void     set_mpi_alloc_mem_info(MPI_Info info);
MPI_Info get_mpi_alloc_mem_info(void);
#endif

#define mpi_malloc(size)        _mpi_malloc((size), __FILE__, __LINE__)
#define mpi_malloc_doubles(n)   _mpi_malloc_doubles((n), __FILE__, __LINE__)
#define mpi_malloc_ints(n)      _mpi_malloc_ints((n), __FILE__, __LINE__)
#define mpi_malloc_chars(n)     _mpi_malloc_chars((n), __FILE__, __LINE__)
#define mpi_malloc_charps(n)    _mpi_malloc_charps((n), __FILE__, __LINE__)
#define mpi_malloc_reqs(n)      _mpi_malloc_reqs((n), __FILE__, __LINE__)
#define mpi_malloc_offsets(n)   _mpi_malloc_offsets((n), __FILE__, __LINE__)
#define mpi_malloc_statuses(n)  _mpi_malloc_statuses((n), __FILE__, __LINE__)

void        *_mpi_malloc(MPI_Aint size , const char *_file, unsigned int _line);
double      *_mpi_malloc_doubles(int n, const char *_file, unsigned int _line);
int         *_mpi_malloc_ints(int n, const char *_file, unsigned int _line);
char        *_mpi_malloc_chars(int n, const char *_file, unsigned int _line);
char       **_mpi_malloc_charps(int n, const char *_file, unsigned int _line);
MPI_Request *_mpi_malloc_reqs(int n, const char *_file, unsigned int _line);
MPI_Offset  *_mpi_malloc_offsets(int n, const char *_file, unsigned int _line);
MPI_Status  *_mpi_malloc_statuses(int n, const char *_file, unsigned int _line);

void mpi_free(void *);

#define skampi_malloc(size)        _skampi_malloc((size), __FILE__, __LINE__)
#define skampi_malloc_doubles(n)   _skampi_malloc_doubles((n), __FILE__, __LINE__)
#define skampi_malloc_ints(n)      _skampi_malloc_ints((n), __FILE__, __LINE__)
#define skampi_malloc_chars(n)     _skampi_malloc_chars((n), __FILE__, __LINE__)
#define skampi_malloc_charps(n)    _skampi_malloc_charps((n), __FILE__, __LINE__)
#define skampi_malloc_reqs(n)      _skampi_malloc_reqs((n), __FILE__, __LINE__)
#define skampi_malloc_offsets(n)   _skampi_malloc_offsets((n), __FILE__, __LINE__)
#define skampi_malloc_statuses(n)  _skampi_malloc_statuses((n), __FILE__, __LINE__)

void        *_skampi_malloc(int size , const char *_file, unsigned int _line);
double      *_skampi_malloc_doubles(int n, const char *_file, unsigned int _line);
int         *_skampi_malloc_ints(int n, const char *_file, unsigned int _line);
char        *_skampi_malloc_chars(int n, const char *_file, unsigned int _line);
char       **_skampi_malloc_charps(int n, const char *_file, unsigned int _line);
MPI_Request *_skampi_malloc_reqs(int n, const char *_file, unsigned int _line);
MPI_Offset  *_skampi_malloc_offsets(int n, const char *_file, unsigned int _line);
MPI_Status  *_skampi_malloc_statuses(int n, const char *_file, unsigned int _line);
