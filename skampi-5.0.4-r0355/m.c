/* This file was generated with extract.pl from the source files
 *    datatypes.c bug.c coll.c delayed.c demo.c helpers.c io.c mmisc.c node2node.c onesided.c pt2pt.c
 * in directory 
 *    measurements
 * Do not change this file manually. All changes will be lost, 
 *    next time extract.pl is run to generate this file.
 */
        
#include <stdlib.h>
#include <mpi.h>

#include "mpiversiontest.h"
        
#include "misc.h"
#include "exec.h"
#include "symboltable.h"
        

#if !defined(USE_ONESIDED) && !defined(USE_MPI_IO)

extern double measure_Allgather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Allgather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Allgatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Allgatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Allgatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Allreduce( int count, MPI_Datatype dt, MPI_Op op );
extern void init_Allreduce( int count, MPI_Datatype dt, MPI_Op op );
extern double measure_Allreduce_using_Reduce_Bcast( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Allreduce_using_Reduce_Bcast( int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoall_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoall_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoall_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoallv_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoallv_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoallv_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Barrier( void );
extern void init_Barrier( void );
extern double measure_Bcast( int count, MPI_Datatype dt, int root );
extern void init_Bcast( int count, MPI_Datatype dt, int root );
extern double measure_Bcast_using_Send_Recv( int count, MPI_Datatype dt, int root );
extern void init_Bcast_using_Send_Recv( int count, MPI_Datatype dt, int root );
extern void finalize_Bcast_using_Send_Recv(int count, MPI_Datatype dt, int root );
extern double measure_Comm_dup( void );
extern void init_Comm_dup( void );
extern double measure_Comm_split( void );
extern void init_Comm_split( void );
extern double measure_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gather_using_Isend_Irecv_Waitall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather_using_Isend_Irecv_Waitall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Gather_using_Isend_Irecv_Waitall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gather_using_Send_Recv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather_using_Send_Recv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Gatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Par_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize );
extern void init_Par_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize );
extern double measure_Pingpong_Bsend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Bsend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Isend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Isend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Issend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Issend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Iprobe_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Iprobe_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Irecv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Irecv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Recv_AT( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Recv_AT( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Sendrecv( int send_count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations );
extern void init_Pingpong_Sendrecv( int count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations );
extern double measure_Pingpong_Sendrecv_replace( int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations );
extern void init_Pingpong_Sendrecv_replace( int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations );
extern double measure_Pingpong_Ssend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Ssend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Reduce( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce( int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Reduce_Scatterv( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce_Scatterv( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void finalize_Reduce_Scatterv(int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Reduce_scatter( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce_scatter( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void finalize_Reduce_scatter(int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Scan( int count, MPI_Datatype dt, MPI_Op op );
extern void init_Scan( int count, MPI_Datatype dt, MPI_Op op );
extern double measure_Scatter( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Scatter( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Scatterv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Scatterv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Scatterv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Wtime( int iterations );
extern DArray func_alloc_datatype_array( int size );
extern IArray func_alloc_int_array( int size );
extern int func_atoi( char* s );
extern double func_cbrt( double x );
extern int func_ceil( double x );
extern void func_choose_barrier_synchronization( void );
extern void func_choose_no_synchronization( void );
extern void func_choose_real_synchronization( void );
extern MPI_Comm func_comm( int i );
extern MPI_Comm func_comm2( int a, int b );
extern MPI_Comm func_comm2_max_latency_with_root( void );
extern MPI_Comm func_comm4( int a, int b, int c, int d );
extern MPI_Comm func_comm_first_half( MPI_Comm c );
extern MPI_Comm func_comm_from_to( int first, int last );
extern MPI_Comm func_comm_rand( int n );
extern MPI_Comm func_comm_second_half( MPI_Comm c );
extern MPI_Comm func_copy_comm( MPI_Comm c );
extern MPI_Datatype func_datatype_array_get( DArray darr, int i );
extern void func_datatype_array_set( DArray darr, int i, MPI_Datatype v );
extern double measure_delayed_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node );
extern void init_delayed_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node );
extern double measure_delayed_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node );
extern void init_delayed_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node );
extern IArray func_dims_create( int nnodes, IArray dims );
extern int func_floor( double x );
extern int func_get_comm_rank( MPI_Comm comm );
extern int func_get_comm_size( MPI_Comm comm );
extern char* func_get_env( char*name );
extern int func_get_np( void );
extern int func_get_random_int( int min_value, int max_value );
extern void func_init_time_accounting( void );
extern int func_int_array_get( IArray iarr, int i );
extern void func_int_array_set( IArray iarr, int i, int v );
extern int func_is_mpi_comm_null( MPI_Comm comm );
extern int func_last_proc( void );
extern double func_log( double a );
extern double func_log10( double a );
extern double func_log2( double a );
extern int func_max_int( int a, int b );
extern int func_min_int( int a, int b );
extern int func_modulo( int a, int b );
extern char* func_mpi_processor_names( void );
extern MPI_Datatype func_mpi_type_contiguous( int count, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_hindexed( IArray blocklengths, IArray displacements, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_hvector( int count, int length, int stride, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_indexed( IArray blocklengths, IArray displacements, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_struct( IArray blocklengths, IArray displacements, DArray dtypes );
extern MPI_Datatype func_mpi_type_vector( int count, int length, int stride, MPI_Datatype oldtype );
extern double measure_myalltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern void init_myalltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern void finalize_myalltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern double func_power( double a, double b );
extern void func_print_time_accounting_info( void );
extern int iterator_range( void** self, int start, int end );
extern int func_round( double x );
extern void func_set_autorefine_factor( double f );
extern void func_set_cache_size( int size );
extern void func_set_recv_buffer_alignment( int a );
extern void func_set_send_buffer_alignment( int a );
extern void func_set_skampi_buffer( int size );
extern double func_sqr( double x );
extern double func_sqrt( double x );
extern int iterator_squares( void** self, int start, int end );
extern void func_switch_buffer_cycling_off( void );
extern void func_switch_buffer_cycling_on( void );
extern MPI_Comm func_topology_cartesian( MPI_Comm c, IArray dims, IArray periodic, int reorder );
extern MPI_Comm func_topology_graph( MPI_Comm c, IArray index, IArray edges, int reorder );
extern MPI_Comm func_topology_graph2( MPI_Comm c, IArray graph, int reorder );
extern void func_use_mpi_alloc_mem( int flag );
extern double measure_waitpattern_down( double scale );
extern void init_waitpattern_down( double scale );
extern double measure_waitpattern_up( double scale );
extern void init_waitpattern_up( double scale );

static void call_measure_Allgather( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allgather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Allgather( struct variable *result, struct variable par[] )
{ init_Allgather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Allgatherv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Allgatherv( struct variable *result, struct variable par[] )
{ init_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Allgatherv( struct variable *result, struct variable par[] )
{ finalize_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Allreduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv )); }

static void call_init_Allreduce( struct variable *result, struct variable par[] )
{ init_Allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv ); }

static void call_measure_Allreduce_using_Reduce_Bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allreduce_using_Reduce_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Allreduce_using_Reduce_Bcast( struct variable *result, struct variable par[] )
{ init_Allreduce_using_Reduce_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoall( struct variable *result, struct variable par[] )
{ init_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ init_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ finalize_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoallv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoallv( struct variable *result, struct variable par[] )
{ init_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoallv( struct variable *result, struct variable par[] )
{ finalize_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ init_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ finalize_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Barrier( struct variable *result, struct variable par[] )
{ store_double( result, measure_Barrier(  )); }

static void call_init_Barrier( struct variable *result, struct variable par[] )
{ init_Barrier(  ); }

static void call_measure_Bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Bcast( struct variable *result, struct variable par[] )
{ init_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ init_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_finalize_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ finalize_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Comm_dup( struct variable *result, struct variable par[] )
{ store_double( result, measure_Comm_dup(  )); }

static void call_init_Comm_dup( struct variable *result, struct variable par[] )
{ init_Comm_dup(  ); }

static void call_measure_Comm_split( struct variable *result, struct variable par[] )
{ store_double( result, measure_Comm_split(  )); }

static void call_init_Comm_split( struct variable *result, struct variable par[] )
{ init_Comm_split(  ); }

static void call_measure_Gather( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather( struct variable *result, struct variable par[] )
{ init_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ init_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ finalize_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gather_using_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather_using_Send_Recv( struct variable *result, struct variable par[] )
{ init_Gather_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gatherv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gatherv( struct variable *result, struct variable par[] )
{ init_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Gatherv( struct variable *result, struct variable par[] )
{ finalize_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Par_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Par_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Par_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ init_Par_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Pingpong_Bsend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Bsend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Bsend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Bsend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Isend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Isend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Isend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Isend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Issend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Issend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Issend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Issend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Iprobe_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Iprobe_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Iprobe_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Iprobe_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Irecv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Recv_AT( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Recv_AT( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Recv_AT( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Recv_AT( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Sendrecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Sendrecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.datatypev, par[5].u.intv, par[6].u.intv )); }

static void call_init_Pingpong_Sendrecv( struct variable *result, struct variable par[] )
{ init_Pingpong_Sendrecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.datatypev, par[5].u.intv, par[6].u.intv ); }

static void call_measure_Pingpong_Sendrecv_replace( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Sendrecv_replace( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv )); }

static void call_init_Pingpong_Sendrecv_replace( struct variable *result, struct variable par[] )
{ init_Pingpong_Sendrecv_replace( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Pingpong_Ssend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Ssend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Ssend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Ssend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Reduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce( struct variable *result, struct variable par[] )
{ init_Reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ init_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_finalize_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ finalize_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Reduce_scatter( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce_scatter( struct variable *result, struct variable par[] )
{ init_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_finalize_Reduce_scatter( struct variable *result, struct variable par[] )
{ finalize_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Scan( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scan( par[0].u.intv, par[1].u.datatypev, par[2].u.opv )); }

static void call_init_Scan( struct variable *result, struct variable par[] )
{ init_Scan( par[0].u.intv, par[1].u.datatypev, par[2].u.opv ); }

static void call_measure_Scatter( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Scatter( struct variable *result, struct variable par[] )
{ init_Scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Scatterv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Scatterv( struct variable *result, struct variable par[] )
{ init_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Scatterv( struct variable *result, struct variable par[] )
{ finalize_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Wtime( struct variable *result, struct variable par[] )
{ store_double( result, measure_Wtime( par[0].u.intv )); }

static void call_func_alloc_datatype_array( struct variable *result, struct variable par[] )
{ store_darray( result, func_alloc_datatype_array( par[0].u.intv )); }

static void call_func_alloc_int_array( struct variable *result, struct variable par[] )
{ store_iarray( result, func_alloc_int_array( par[0].u.intv )); }

static void call_func_atoi( struct variable *result, struct variable par[] )
{ store_int( result, func_atoi( par[0].u.stringv )); }

static void call_func_cbrt( struct variable *result, struct variable par[] )
{ store_double( result, func_cbrt( par[0].u.doublev )); }

static void call_func_ceil( struct variable *result, struct variable par[] )
{ store_int( result, func_ceil( par[0].u.doublev )); }

static void call_func_choose_barrier_synchronization( struct variable *result, struct variable par[] )
{ func_choose_barrier_synchronization(  ); }

static void call_func_choose_no_synchronization( struct variable *result, struct variable par[] )
{ func_choose_no_synchronization(  ); }

static void call_func_choose_real_synchronization( struct variable *result, struct variable par[] )
{ func_choose_real_synchronization(  ); }

static void call_func_comm( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm( par[0].u.intv )); }

static void call_func_comm2( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm2( par[0].u.intv, par[1].u.intv )); }

static void call_func_comm2_max_latency_with_root( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm2_max_latency_with_root(  )); }

static void call_func_comm4( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm4( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.intv )); }

static void call_func_comm_first_half( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_first_half( par[0].u.commv )); }

static void call_func_comm_from_to( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_from_to( par[0].u.intv, par[1].u.intv )); }

static void call_func_comm_rand( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_rand( par[0].u.intv )); }

static void call_func_comm_second_half( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_second_half( par[0].u.commv )); }

static void call_func_copy_comm( struct variable *result, struct variable par[] )
{ store_comm( result, func_copy_comm( par[0].u.commv )); }

static void call_func_datatype_array_get( struct variable *result, struct variable par[] )
{ store_datatype( result, func_datatype_array_get( par[0].u.darrv, par[1].u.intv )); }

static void call_func_datatype_array_set( struct variable *result, struct variable par[] )
{ func_datatype_array_set( par[0].u.darrv, par[1].u.intv, par[2].u.datatypev ); }

static void call_measure_delayed_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_delayed_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.doublev, par[5].u.intv )); }

static void call_init_delayed_Alltoall( struct variable *result, struct variable par[] )
{ init_delayed_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.doublev, par[5].u.intv ); }

static void call_measure_delayed_Gather( struct variable *result, struct variable par[] )
{ store_double( result, measure_delayed_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev, par[6].u.intv )); }

static void call_init_delayed_Gather( struct variable *result, struct variable par[] )
{ init_delayed_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev, par[6].u.intv ); }

static void call_func_dims_create( struct variable *result, struct variable par[] )
{ store_iarray( result, func_dims_create( par[0].u.intv, par[1].u.iarrv )); }

static void call_func_floor( struct variable *result, struct variable par[] )
{ store_int( result, func_floor( par[0].u.doublev )); }

static void call_func_get_comm_rank( struct variable *result, struct variable par[] )
{ store_int( result, func_get_comm_rank( par[0].u.commv )); }

static void call_func_get_comm_size( struct variable *result, struct variable par[] )
{ store_int( result, func_get_comm_size( par[0].u.commv )); }

static void call_func_get_env( struct variable *result, struct variable par[] )
{ store_string( result, func_get_env( par[0].u.stringv )); }

static void call_func_get_np( struct variable *result, struct variable par[] )
{ store_int( result, func_get_np(  )); }

static void call_func_get_random_int( struct variable *result, struct variable par[] )
{ store_int( result, func_get_random_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_init_time_accounting( struct variable *result, struct variable par[] )
{ func_init_time_accounting(  ); }

static void call_func_int_array_get( struct variable *result, struct variable par[] )
{ store_int( result, func_int_array_get( par[0].u.iarrv, par[1].u.intv )); }

static void call_func_int_array_set( struct variable *result, struct variable par[] )
{ func_int_array_set( par[0].u.iarrv, par[1].u.intv, par[2].u.intv ); }

static void call_func_is_mpi_comm_null( struct variable *result, struct variable par[] )
{ store_int( result, func_is_mpi_comm_null( par[0].u.commv )); }

static void call_func_last_proc( struct variable *result, struct variable par[] )
{ store_int( result, func_last_proc(  )); }

static void call_func_log( struct variable *result, struct variable par[] )
{ store_double( result, func_log( par[0].u.doublev )); }

static void call_func_log10( struct variable *result, struct variable par[] )
{ store_double( result, func_log10( par[0].u.doublev )); }

static void call_func_log2( struct variable *result, struct variable par[] )
{ store_double( result, func_log2( par[0].u.doublev )); }

static void call_func_max_int( struct variable *result, struct variable par[] )
{ store_int( result, func_max_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_min_int( struct variable *result, struct variable par[] )
{ store_int( result, func_min_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_modulo( struct variable *result, struct variable par[] )
{ store_int( result, func_modulo( par[0].u.intv, par[1].u.intv )); }

static void call_func_mpi_processor_names( struct variable *result, struct variable par[] )
{ store_string( result, func_mpi_processor_names(  )); }

static void call_func_mpi_type_contiguous( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_contiguous( par[0].u.intv, par[1].u.datatypev )); }

static void call_func_mpi_type_hindexed( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_hindexed( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev )); }

static void call_func_mpi_type_hvector( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_hvector( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev )); }

static void call_func_mpi_type_indexed( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_indexed( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev )); }

static void call_func_mpi_type_struct( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_struct( par[0].u.iarrv, par[1].u.iarrv, par[2].u.darrv )); }

static void call_func_mpi_type_vector( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_vector( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev )); }

static void call_measure_myalltoallv( struct variable *result, struct variable par[] )
{ store_double( result, measure_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev )); }

static void call_init_myalltoallv( struct variable *result, struct variable par[] )
{ init_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev ); }

static void call_finalize_myalltoallv( struct variable *result, struct variable par[] )
{ finalize_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev ); }

static void call_func_power( struct variable *result, struct variable par[] )
{ store_double( result, func_power( par[0].u.doublev, par[1].u.doublev )); }

static void call_func_print_time_accounting_info( struct variable *result, struct variable par[] )
{ func_print_time_accounting_info(  ); }

static void call_iterator_range( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_range( self, par[0].u.intv, par[1].u.intv )); }

static void call_func_round( struct variable *result, struct variable par[] )
{ store_int( result, func_round( par[0].u.doublev )); }

static void call_func_set_autorefine_factor( struct variable *result, struct variable par[] )
{ func_set_autorefine_factor( par[0].u.doublev ); }

static void call_func_set_cache_size( struct variable *result, struct variable par[] )
{ func_set_cache_size( par[0].u.intv ); }

static void call_func_set_recv_buffer_alignment( struct variable *result, struct variable par[] )
{ func_set_recv_buffer_alignment( par[0].u.intv ); }

static void call_func_set_send_buffer_alignment( struct variable *result, struct variable par[] )
{ func_set_send_buffer_alignment( par[0].u.intv ); }

static void call_func_set_skampi_buffer( struct variable *result, struct variable par[] )
{ func_set_skampi_buffer( par[0].u.intv ); }

static void call_func_sqr( struct variable *result, struct variable par[] )
{ store_double( result, func_sqr( par[0].u.doublev )); }

static void call_func_sqrt( struct variable *result, struct variable par[] )
{ store_double( result, func_sqrt( par[0].u.doublev )); }

static void call_iterator_squares( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_squares( self, par[0].u.intv, par[1].u.intv )); }

static void call_func_switch_buffer_cycling_off( struct variable *result, struct variable par[] )
{ func_switch_buffer_cycling_off(  ); }

static void call_func_switch_buffer_cycling_on( struct variable *result, struct variable par[] )
{ func_switch_buffer_cycling_on(  ); }

static void call_func_topology_cartesian( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_cartesian( par[0].u.commv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.intv )); }

static void call_func_topology_graph( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_graph( par[0].u.commv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.intv )); }

static void call_func_topology_graph2( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_graph2( par[0].u.commv, par[1].u.iarrv, par[2].u.intv )); }

static void call_func_use_mpi_alloc_mem( struct variable *result, struct variable par[] )
{ func_use_mpi_alloc_mem( par[0].u.intv ); }

static void call_measure_waitpattern_down( struct variable *result, struct variable par[] )
{ store_double( result, measure_waitpattern_down( par[0].u.doublev )); }

static void call_init_waitpattern_down( struct variable *result, struct variable par[] )
{ init_waitpattern_down( par[0].u.doublev ); }

static void call_measure_waitpattern_up( struct variable *result, struct variable par[] )
{ store_double( result, measure_waitpattern_up( par[0].u.doublev )); }

static void call_init_waitpattern_up( struct variable *result, struct variable par[] )
{ init_waitpattern_up( par[0].u.doublev ); }

void init_functions(void)
{
  insert_function( &call_init_Allgather, &call_measure_Allgather, NULL, "Allgather", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Allgatherv, &call_measure_Allgatherv, &call_finalize_Allgatherv, "Allgatherv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Allreduce, &call_measure_Allreduce, NULL, "Allreduce", "ido", TYPE_DOUBLE );
  insert_function( &call_init_Allreduce_using_Reduce_Bcast, &call_measure_Allreduce_using_Reduce_Bcast, NULL, "Allreduce_using_Reduce_Bcast", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Alltoall, &call_measure_Alltoall, NULL, "Alltoall", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoall_using_Isend_Irecv, &call_measure_Alltoall_using_Isend_Irecv, &call_finalize_Alltoall_using_Isend_Irecv, "Alltoall_using_Isend_Irecv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoallv, &call_measure_Alltoallv, &call_finalize_Alltoallv, "Alltoallv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoallv_using_Isend_Irecv, &call_measure_Alltoallv_using_Isend_Irecv, &call_finalize_Alltoallv_using_Isend_Irecv, "Alltoallv_using_Isend_Irecv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Barrier, &call_measure_Barrier, NULL, "Barrier", "", TYPE_DOUBLE );
  insert_function( &call_init_Bcast, &call_measure_Bcast, NULL, "Bcast", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Bcast_using_Send_Recv, &call_measure_Bcast_using_Send_Recv, &call_finalize_Bcast_using_Send_Recv, "Bcast_using_Send_Recv", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Comm_dup, &call_measure_Comm_dup, NULL, "Comm_dup", "", TYPE_DOUBLE );
  insert_function( &call_init_Comm_split, &call_measure_Comm_split, NULL, "Comm_split", "", TYPE_DOUBLE );
  insert_function( &call_init_Gather, &call_measure_Gather, NULL, "Gather", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gather_using_Isend_Irecv_Waitall, &call_measure_Gather_using_Isend_Irecv_Waitall, &call_finalize_Gather_using_Isend_Irecv_Waitall, "Gather_using_Isend_Irecv_Waitall", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gather_using_Send_Recv, &call_measure_Gather_using_Send_Recv, NULL, "Gather_using_Send_Recv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gatherv, &call_measure_Gatherv, &call_finalize_Gatherv, "Gatherv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Par_Pingpong_Send_Recv, &call_measure_Par_Pingpong_Send_Recv, NULL, "Par_Pingpong_Send_Recv", "idiiii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Bsend_Recv, &call_measure_Pingpong_Bsend_Recv, NULL, "Pingpong_Bsend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Isend_Recv, &call_measure_Pingpong_Isend_Recv, NULL, "Pingpong_Isend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Issend_Recv, &call_measure_Pingpong_Issend_Recv, NULL, "Pingpong_Issend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Iprobe_Recv, &call_measure_Pingpong_Send_Iprobe_Recv, NULL, "Pingpong_Send_Iprobe_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Irecv, &call_measure_Pingpong_Send_Irecv, NULL, "Pingpong_Send_Irecv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Recv, &call_measure_Pingpong_Send_Recv, NULL, "Pingpong_Send_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Recv_AT, &call_measure_Pingpong_Send_Recv_AT, NULL, "Pingpong_Send_Recv_AT", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Sendrecv, &call_measure_Pingpong_Sendrecv, NULL, "Pingpong_Sendrecv", "idiidii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Sendrecv_replace, &call_measure_Pingpong_Sendrecv_replace, NULL, "Pingpong_Sendrecv_replace", "idiii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Ssend_Recv, &call_measure_Pingpong_Ssend_Recv, NULL, "Pingpong_Ssend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Reduce, &call_measure_Reduce, NULL, "Reduce", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce_Scatterv, &call_measure_Reduce_Scatterv, &call_finalize_Reduce_Scatterv, "Reduce_Scatterv", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce_scatter, &call_measure_Reduce_scatter, &call_finalize_Reduce_scatter, "Reduce_scatter", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Scan, &call_measure_Scan, NULL, "Scan", "ido", TYPE_DOUBLE );
  insert_function( &call_init_Scatter, &call_measure_Scatter, NULL, "Scatter", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Scatterv, &call_measure_Scatterv, &call_finalize_Scatterv, "Scatterv", "ididi", TYPE_DOUBLE );
  insert_function( NULL, &call_measure_Wtime, NULL, "Wtime", "i", TYPE_DOUBLE );
  insert_function( NULL, &call_func_alloc_datatype_array, NULL, "alloc_datatype_array", "i", TYPE_DARRAY );
  insert_function( NULL, &call_func_alloc_int_array, NULL, "alloc_int_array", "i", TYPE_IARRAY );
  insert_function( NULL, &call_func_atoi, NULL, "atoi", "s", TYPE_INTEGER );
  insert_function( NULL, &call_func_cbrt, NULL, "cbrt", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_ceil, NULL, "ceil", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_choose_barrier_synchronization, NULL, "choose_barrier_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_choose_no_synchronization, NULL, "choose_no_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_choose_real_synchronization, NULL, "choose_real_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_comm, NULL, "comm", "i", TYPE_COMM );
  insert_function( NULL, &call_func_comm2, NULL, "comm2", "ii", TYPE_COMM );
  insert_function( NULL, &call_func_comm2_max_latency_with_root, NULL, "comm2_max_latency_with_root", "", TYPE_COMM );
  insert_function( NULL, &call_func_comm4, NULL, "comm4", "iiii", TYPE_COMM );
  insert_function( NULL, &call_func_comm_first_half, NULL, "comm_first_half", "c", TYPE_COMM );
  insert_function( NULL, &call_func_comm_from_to, NULL, "comm_from_to", "ii", TYPE_COMM );
  insert_function( NULL, &call_func_comm_rand, NULL, "comm_rand", "i", TYPE_COMM );
  insert_function( NULL, &call_func_comm_second_half, NULL, "comm_second_half", "c", TYPE_COMM );
  insert_function( NULL, &call_func_copy_comm, NULL, "copy_comm", "c", TYPE_COMM );
  insert_function( NULL, &call_func_datatype_array_get, NULL, "datatype_array_get", "ei", TYPE_DATATYPE );
  insert_function( NULL, &call_func_datatype_array_set, NULL, "datatype_array_set", "eid", TYPE_VOID );
  insert_function( &call_init_delayed_Alltoall, &call_measure_delayed_Alltoall, NULL, "delayed_Alltoall", "ididfi", TYPE_DOUBLE );
  insert_function( &call_init_delayed_Gather, &call_measure_delayed_Gather, NULL, "delayed_Gather", "ididifi", TYPE_DOUBLE );
  insert_function( NULL, &call_func_dims_create, NULL, "dims_create", "ia", TYPE_IARRAY );
  insert_function( NULL, &call_func_floor, NULL, "floor", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_comm_rank, NULL, "get_comm_rank", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_comm_size, NULL, "get_comm_size", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_env, NULL, "get_env", "s", TYPE_STRING );
  insert_function( NULL, &call_func_get_np, NULL, "get_np", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_random_int, NULL, "get_random_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_init_time_accounting, NULL, "init_time_accounting", "", TYPE_VOID );
  insert_function( NULL, &call_func_int_array_get, NULL, "int_array_get", "ai", TYPE_INTEGER );
  insert_function( NULL, &call_func_int_array_set, NULL, "int_array_set", "aii", TYPE_VOID );
  insert_function( NULL, &call_func_is_mpi_comm_null, NULL, "is_mpi_comm_null", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_last_proc, NULL, "last_proc", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_log, NULL, "log", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_log10, NULL, "log10", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_log2, NULL, "log2", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_max_int, NULL, "max_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_min_int, NULL, "min_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_modulo, NULL, "modulo", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_mpi_processor_names, NULL, "mpi_processor_names", "", TYPE_STRING );
  insert_function( NULL, &call_func_mpi_type_contiguous, NULL, "mpi_type_contiguous", "id", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_hindexed, NULL, "mpi_type_hindexed", "aad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_hvector, NULL, "mpi_type_hvector", "iiid", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_indexed, NULL, "mpi_type_indexed", "aad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_struct, NULL, "mpi_type_struct", "aae", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_vector, NULL, "mpi_type_vector", "iiid", TYPE_DATATYPE );
  insert_function( &call_init_myalltoallv, &call_measure_myalltoallv, &call_finalize_myalltoallv, "myalltoallv", "ididif", TYPE_DOUBLE );
  insert_function( NULL, &call_func_power, NULL, "power", "ff", TYPE_DOUBLE );
  insert_function( NULL, &call_func_print_time_accounting_info, NULL, "print_time_accounting_info", "", TYPE_VOID );
  insert_iterator( &call_iterator_range, "range", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_round, NULL, "round", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_set_autorefine_factor, NULL, "set_autorefine_factor", "f", TYPE_VOID );
  insert_function( NULL, &call_func_set_cache_size, NULL, "set_cache_size", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_recv_buffer_alignment, NULL, "set_recv_buffer_alignment", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_send_buffer_alignment, NULL, "set_send_buffer_alignment", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_skampi_buffer, NULL, "set_skampi_buffer", "i", TYPE_VOID );
  insert_function( NULL, &call_func_sqr, NULL, "sqr", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_sqrt, NULL, "sqrt", "f", TYPE_DOUBLE );
  insert_iterator( &call_iterator_squares, "squares", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_switch_buffer_cycling_off, NULL, "switch_buffer_cycling_off", "", TYPE_VOID );
  insert_function( NULL, &call_func_switch_buffer_cycling_on, NULL, "switch_buffer_cycling_on", "", TYPE_VOID );
  insert_function( NULL, &call_func_topology_cartesian, NULL, "topology_cartesian", "caai", TYPE_COMM );
  insert_function( NULL, &call_func_topology_graph, NULL, "topology_graph", "caai", TYPE_COMM );
  insert_function( NULL, &call_func_topology_graph2, NULL, "topology_graph2", "cai", TYPE_COMM );
  insert_function( NULL, &call_func_use_mpi_alloc_mem, NULL, "use_mpi_alloc_mem", "i", TYPE_VOID );
  insert_function( &call_init_waitpattern_down, &call_measure_waitpattern_down, NULL, "waitpattern_down", "f", TYPE_DOUBLE );
  insert_function( &call_init_waitpattern_up, &call_measure_waitpattern_up, NULL, "waitpattern_up", "f", TYPE_DOUBLE );
}

#endif


#if !defined(USE_ONESIDED) && defined(USE_MPI_IO)

extern double measure_Allgather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Allgather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Allgatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Allgatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Allgatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Allreduce( int count, MPI_Datatype dt, MPI_Op op );
extern void init_Allreduce( int count, MPI_Datatype dt, MPI_Op op );
extern double measure_Allreduce_using_Reduce_Bcast( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Allreduce_using_Reduce_Bcast( int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoall_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoall_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoall_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoallv_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoallv_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoallv_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Barrier( void );
extern void init_Barrier( void );
extern double measure_Bcast( int count, MPI_Datatype dt, int root );
extern void init_Bcast( int count, MPI_Datatype dt, int root );
extern double measure_Bcast_using_Send_Recv( int count, MPI_Datatype dt, int root );
extern void init_Bcast_using_Send_Recv( int count, MPI_Datatype dt, int root );
extern void finalize_Bcast_using_Send_Recv(int count, MPI_Datatype dt, int root );
extern double measure_Comm_dup( void );
extern void init_Comm_dup( void );
extern double measure_Comm_split( void );
extern void init_Comm_split( void );
extern double measure_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gather_using_Isend_Irecv_Waitall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather_using_Isend_Irecv_Waitall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Gather_using_Isend_Irecv_Waitall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gather_using_Send_Recv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather_using_Send_Recv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Gatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_MPI_IO_close( int count );
extern void init_MPI_IO_close( int count );
extern void finalize_MPI_IO_close(int count );
extern double measure_MPI_IO_delete( int count );
extern void init_MPI_IO_delete( int count );
extern void finalize_MPI_IO_delete(int count );
extern double measure_MPI_IO_delete_on_close( int count );
extern void init_MPI_IO_delete_on_close( int count );
extern void finalize_MPI_IO_delete_on_close(int count );
extern double measure_MPI_IO_file_seek( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern void init_MPI_IO_file_seek( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern void finalize_MPI_IO_file_seek(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern double measure_MPI_IO_file_seek_shared( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern void init_MPI_IO_file_seek_shared( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern void finalize_MPI_IO_file_seek_shared(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern double measure_MPI_IO_open( int create_file );
extern void init_MPI_IO_open( int create_file );
extern void finalize_MPI_IO_open(int create_file );
extern double measure_MPI_IO_open_close( int create_flag );
extern void init_MPI_IO_open_close( int create_flag );
extern void finalize_MPI_IO_open_close(int create_flag );
extern double measure_MPI_IO_preallocate( int count, int with_sync );
extern void init_MPI_IO_preallocate( int count, int with_sync );
extern void finalize_MPI_IO_preallocate(int count, int with_sync );
extern double measure_MPI_IO_read_and_distribute( int count, int np, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_and_distribute( int count, int np, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_and_distribute(int count, int np, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_file( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_file( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_file(int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_noncontiguous( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_noncontiguous( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_noncontiguous(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_once( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern void init_MPI_IO_read_once( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern void finalize_MPI_IO_read_once(int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern double measure_MPI_IO_read_random( IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_random( IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_random(IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_segmented( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_segmented( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_segmented(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_separate( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_separate( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_separate(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_sequential( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_sequential( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_sequential(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_strided( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_strided( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_strided(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_and_read( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_and_read( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_and_read(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_noncontiguous( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_noncontiguous( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_noncontiguous(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_once( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern void init_MPI_IO_write_once( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern void finalize_MPI_IO_write_once(int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern double measure_MPI_IO_write_random( IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_random( IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_random(IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_segmented( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_segmented( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_segmented(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_separate( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_separate( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_separate(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_sequential( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_sequential( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_sequential(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_strided( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_strided( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_strided(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_Par_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize );
extern void init_Par_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize );
extern double measure_Pingpong_Bsend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Bsend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Isend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Isend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Issend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Issend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Iprobe_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Iprobe_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Irecv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Irecv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Recv_AT( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Recv_AT( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Sendrecv( int send_count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations );
extern void init_Pingpong_Sendrecv( int count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations );
extern double measure_Pingpong_Sendrecv_replace( int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations );
extern void init_Pingpong_Sendrecv_replace( int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations );
extern double measure_Pingpong_Ssend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Ssend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Reduce( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce( int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Reduce_Scatterv( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce_Scatterv( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void finalize_Reduce_Scatterv(int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Reduce_scatter( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce_scatter( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void finalize_Reduce_scatter(int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Scan( int count, MPI_Datatype dt, MPI_Op op );
extern void init_Scan( int count, MPI_Datatype dt, MPI_Op op );
extern double measure_Scatter( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Scatter( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Scatterv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Scatterv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Scatterv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Wtime( int iterations );
extern DArray func_alloc_datatype_array( int size );
extern IArray func_alloc_int_array( int size );
extern int func_atoi( char* s );
extern double func_cbrt( double x );
extern int func_ceil( double x );
extern void func_choose_barrier_synchronization( void );
extern void func_choose_no_synchronization( void );
extern void func_choose_real_synchronization( void );
extern MPI_Comm func_comm( int i );
extern MPI_Comm func_comm2( int a, int b );
extern MPI_Comm func_comm2_max_latency_with_root( void );
extern MPI_Comm func_comm4( int a, int b, int c, int d );
extern MPI_Comm func_comm_first_half( MPI_Comm c );
extern MPI_Comm func_comm_from_to( int first, int last );
extern MPI_Comm func_comm_rand( int n );
extern MPI_Comm func_comm_second_half( MPI_Comm c );
extern MPI_Comm func_copy_comm( MPI_Comm c );
extern int iterator_data_access_routines( void** self, int pattern );
extern MPI_Datatype func_datatype_array_get( DArray darr, int i );
extern void func_datatype_array_set( DArray darr, int i, MPI_Datatype v );
extern double measure_delayed_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node );
extern void init_delayed_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node );
extern double measure_delayed_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node );
extern void init_delayed_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node );
extern IArray func_dims_create( int nnodes, IArray dims );
extern int func_floor( double x );
extern int func_get_comm_rank( MPI_Comm comm );
extern int func_get_comm_size( MPI_Comm comm );
extern char* func_get_env( char*name );
extern int func_get_io_atomicity( void );
extern char* func_get_io_datarep( void );
extern int func_get_io_file_sync( void );
extern int func_get_io_preallocation( void );
extern int func_get_io_unique_open( void );
extern char* func_get_io_working_dir( void );
extern MPI_Info func_get_mpi_alloc_mem_info( void );
extern int func_get_np( void );
extern int func_get_random_int( int min_value, int max_value );
extern MPI_Info func_info_create( void );
extern void func_info_free( MPI_Info info );
extern void func_info_set( MPI_Info info, char* key, char* value );
extern void func_init_time_accounting( void );
extern IArray func_int_array_allgather( MPI_Comm c, int v );
extern int func_int_array_get( IArray iarr, int i );
extern void func_int_array_set( IArray iarr, int i, int v );
extern int func_is_mpi_comm_null( MPI_Comm comm );
extern int func_last_proc( void );
extern double func_log( double a );
extern double func_log10( double a );
extern double func_log2( double a );
extern int func_max_int( int a, int b );
extern int func_min_int( int a, int b );
extern int func_modulo( int a, int b );
extern char* func_mpi_processor_names( void );
extern MPI_Datatype func_mpi_type_contiguous( int count, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_create_darray( int size, int rank, IArray gsizes, IArray distribs, IArray dargs, IArray psizes, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_create_subarray( IArray sizes, IArray subsizes, IArray starts, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_hindexed( IArray blocklengths, IArray displacements, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_hvector( int count, int length, int stride, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_indexed( IArray blocklengths, IArray displacements, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_struct( IArray blocklengths, IArray displacements, DArray dtypes );
extern MPI_Datatype func_mpi_type_vector( int count, int length, int stride, MPI_Datatype oldtype );
extern double measure_myalltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern void init_myalltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern void finalize_myalltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern double func_power( double a, double b );
extern void func_print_data_access_settings( int daccess, int flag );
extern void func_print_file_info( void );
extern void func_print_time_accounting_info( void );
extern int iterator_range( void** self, int start, int end );
extern void func_reset_file_info( void );
extern int func_round( double x );
extern void func_set_autorefine_factor( double f );
extern void func_set_cache_size( int size );
extern void func_set_file_info( char*key, char*value );
extern void func_set_io_atomicity( int flag );
extern void func_set_io_datarep( char*datarep );
extern void func_set_io_file_sync( int flag );
extern void func_set_io_preallocation( int flag );
extern void func_set_io_unique_open( int flag );
extern void func_set_io_working_dir( char*cwd );
extern void func_set_mpi_alloc_mem_info( MPI_Info info );
extern void func_set_recv_buffer_alignment( int a );
extern void func_set_send_buffer_alignment( int a );
extern void func_set_skampi_buffer( int size );
extern double func_sqr( double x );
extern double func_sqrt( double x );
extern int iterator_squares( void** self, int start, int end );
extern void func_switch_buffer_cycling_off( void );
extern void func_switch_buffer_cycling_on( void );
extern MPI_Comm func_topology_cartesian( MPI_Comm c, IArray dims, IArray periodic, int reorder );
extern MPI_Comm func_topology_graph( MPI_Comm c, IArray index, IArray edges, int reorder );
extern MPI_Comm func_topology_graph2( MPI_Comm c, IArray graph, int reorder );
extern void func_use_mpi_alloc_mem( int flag );
extern double measure_waitpattern_down( double scale );
extern void init_waitpattern_down( double scale );
extern double measure_waitpattern_up( double scale );
extern void init_waitpattern_up( double scale );

static void call_measure_Allgather( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allgather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Allgather( struct variable *result, struct variable par[] )
{ init_Allgather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Allgatherv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Allgatherv( struct variable *result, struct variable par[] )
{ init_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Allgatherv( struct variable *result, struct variable par[] )
{ finalize_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Allreduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv )); }

static void call_init_Allreduce( struct variable *result, struct variable par[] )
{ init_Allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv ); }

static void call_measure_Allreduce_using_Reduce_Bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allreduce_using_Reduce_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Allreduce_using_Reduce_Bcast( struct variable *result, struct variable par[] )
{ init_Allreduce_using_Reduce_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoall( struct variable *result, struct variable par[] )
{ init_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ init_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ finalize_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoallv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoallv( struct variable *result, struct variable par[] )
{ init_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoallv( struct variable *result, struct variable par[] )
{ finalize_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ init_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ finalize_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Barrier( struct variable *result, struct variable par[] )
{ store_double( result, measure_Barrier(  )); }

static void call_init_Barrier( struct variable *result, struct variable par[] )
{ init_Barrier(  ); }

static void call_measure_Bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Bcast( struct variable *result, struct variable par[] )
{ init_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ init_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_finalize_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ finalize_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Comm_dup( struct variable *result, struct variable par[] )
{ store_double( result, measure_Comm_dup(  )); }

static void call_init_Comm_dup( struct variable *result, struct variable par[] )
{ init_Comm_dup(  ); }

static void call_measure_Comm_split( struct variable *result, struct variable par[] )
{ store_double( result, measure_Comm_split(  )); }

static void call_init_Comm_split( struct variable *result, struct variable par[] )
{ init_Comm_split(  ); }

static void call_measure_Gather( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather( struct variable *result, struct variable par[] )
{ init_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ init_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ finalize_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gather_using_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather_using_Send_Recv( struct variable *result, struct variable par[] )
{ init_Gather_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gatherv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gatherv( struct variable *result, struct variable par[] )
{ init_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Gatherv( struct variable *result, struct variable par[] )
{ finalize_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_MPI_IO_close( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_close( par[0].u.intv )); }

static void call_init_MPI_IO_close( struct variable *result, struct variable par[] )
{ init_MPI_IO_close( par[0].u.intv ); }

static void call_finalize_MPI_IO_close( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_close( par[0].u.intv ); }

static void call_measure_MPI_IO_delete( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_delete( par[0].u.intv )); }

static void call_init_MPI_IO_delete( struct variable *result, struct variable par[] )
{ init_MPI_IO_delete( par[0].u.intv ); }

static void call_finalize_MPI_IO_delete( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_delete( par[0].u.intv ); }

static void call_measure_MPI_IO_delete_on_close( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_delete_on_close( par[0].u.intv )); }

static void call_init_MPI_IO_delete_on_close( struct variable *result, struct variable par[] )
{ init_MPI_IO_delete_on_close( par[0].u.intv ); }

static void call_finalize_MPI_IO_delete_on_close( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_delete_on_close( par[0].u.intv ); }

static void call_measure_MPI_IO_file_seek( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_file_seek( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev )); }

static void call_init_MPI_IO_file_seek( struct variable *result, struct variable par[] )
{ init_MPI_IO_file_seek( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev ); }

static void call_finalize_MPI_IO_file_seek( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_file_seek( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev ); }

static void call_measure_MPI_IO_file_seek_shared( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_file_seek_shared( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev )); }

static void call_init_MPI_IO_file_seek_shared( struct variable *result, struct variable par[] )
{ init_MPI_IO_file_seek_shared( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev ); }

static void call_finalize_MPI_IO_file_seek_shared( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_file_seek_shared( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev ); }

static void call_measure_MPI_IO_open( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_open( par[0].u.intv )); }

static void call_init_MPI_IO_open( struct variable *result, struct variable par[] )
{ init_MPI_IO_open( par[0].u.intv ); }

static void call_finalize_MPI_IO_open( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_open( par[0].u.intv ); }

static void call_measure_MPI_IO_open_close( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_open_close( par[0].u.intv )); }

static void call_init_MPI_IO_open_close( struct variable *result, struct variable par[] )
{ init_MPI_IO_open_close( par[0].u.intv ); }

static void call_finalize_MPI_IO_open_close( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_open_close( par[0].u.intv ); }

static void call_measure_MPI_IO_preallocate( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_preallocate( par[0].u.intv, par[1].u.intv )); }

static void call_init_MPI_IO_preallocate( struct variable *result, struct variable par[] )
{ init_MPI_IO_preallocate( par[0].u.intv, par[1].u.intv ); }

static void call_finalize_MPI_IO_preallocate( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_preallocate( par[0].u.intv, par[1].u.intv ); }

static void call_measure_MPI_IO_read_and_distribute( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_and_distribute( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_read_and_distribute( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_and_distribute( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_and_distribute( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_and_distribute( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_read_file( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_file( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_MPI_IO_read_file( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_file( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_MPI_IO_read_file( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_file( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_MPI_IO_read_noncontiguous( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_read_noncontiguous( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_noncontiguous( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_read_once( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv )); }

static void call_init_MPI_IO_read_once( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_once( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv ); }

static void call_measure_MPI_IO_read_random( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_read_random( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_random( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_read_segmented( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_read_segmented( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_read_segmented( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_read_separate( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_read_separate( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_read_separate( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_read_sequential( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_read_sequential( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_sequential( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_read_strided( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_read_strided( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_read_strided( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_write_and_read( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_and_read( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_write_and_read( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_and_read( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_and_read( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_and_read( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_write_noncontiguous( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_write_noncontiguous( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_noncontiguous( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_write_once( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv )); }

static void call_init_MPI_IO_write_once( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_once( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv ); }

static void call_measure_MPI_IO_write_random( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_write_random( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_random( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_write_segmented( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_write_segmented( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_write_segmented( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_write_separate( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_write_separate( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_write_separate( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_write_sequential( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_write_sequential( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_sequential( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_write_strided( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_write_strided( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_write_strided( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_Par_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Par_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Par_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ init_Par_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Pingpong_Bsend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Bsend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Bsend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Bsend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Isend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Isend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Isend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Isend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Issend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Issend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Issend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Issend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Iprobe_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Iprobe_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Iprobe_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Iprobe_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Irecv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Recv_AT( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Recv_AT( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Recv_AT( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Recv_AT( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Sendrecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Sendrecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.datatypev, par[5].u.intv, par[6].u.intv )); }

static void call_init_Pingpong_Sendrecv( struct variable *result, struct variable par[] )
{ init_Pingpong_Sendrecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.datatypev, par[5].u.intv, par[6].u.intv ); }

static void call_measure_Pingpong_Sendrecv_replace( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Sendrecv_replace( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv )); }

static void call_init_Pingpong_Sendrecv_replace( struct variable *result, struct variable par[] )
{ init_Pingpong_Sendrecv_replace( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Pingpong_Ssend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Ssend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Ssend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Ssend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Reduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce( struct variable *result, struct variable par[] )
{ init_Reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ init_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_finalize_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ finalize_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Reduce_scatter( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce_scatter( struct variable *result, struct variable par[] )
{ init_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_finalize_Reduce_scatter( struct variable *result, struct variable par[] )
{ finalize_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Scan( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scan( par[0].u.intv, par[1].u.datatypev, par[2].u.opv )); }

static void call_init_Scan( struct variable *result, struct variable par[] )
{ init_Scan( par[0].u.intv, par[1].u.datatypev, par[2].u.opv ); }

static void call_measure_Scatter( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Scatter( struct variable *result, struct variable par[] )
{ init_Scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Scatterv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Scatterv( struct variable *result, struct variable par[] )
{ init_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Scatterv( struct variable *result, struct variable par[] )
{ finalize_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Wtime( struct variable *result, struct variable par[] )
{ store_double( result, measure_Wtime( par[0].u.intv )); }

static void call_func_alloc_datatype_array( struct variable *result, struct variable par[] )
{ store_darray( result, func_alloc_datatype_array( par[0].u.intv )); }

static void call_func_alloc_int_array( struct variable *result, struct variable par[] )
{ store_iarray( result, func_alloc_int_array( par[0].u.intv )); }

static void call_func_atoi( struct variable *result, struct variable par[] )
{ store_int( result, func_atoi( par[0].u.stringv )); }

static void call_func_cbrt( struct variable *result, struct variable par[] )
{ store_double( result, func_cbrt( par[0].u.doublev )); }

static void call_func_ceil( struct variable *result, struct variable par[] )
{ store_int( result, func_ceil( par[0].u.doublev )); }

static void call_func_choose_barrier_synchronization( struct variable *result, struct variable par[] )
{ func_choose_barrier_synchronization(  ); }

static void call_func_choose_no_synchronization( struct variable *result, struct variable par[] )
{ func_choose_no_synchronization(  ); }

static void call_func_choose_real_synchronization( struct variable *result, struct variable par[] )
{ func_choose_real_synchronization(  ); }

static void call_func_comm( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm( par[0].u.intv )); }

static void call_func_comm2( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm2( par[0].u.intv, par[1].u.intv )); }

static void call_func_comm2_max_latency_with_root( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm2_max_latency_with_root(  )); }

static void call_func_comm4( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm4( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.intv )); }

static void call_func_comm_first_half( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_first_half( par[0].u.commv )); }

static void call_func_comm_from_to( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_from_to( par[0].u.intv, par[1].u.intv )); }

static void call_func_comm_rand( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_rand( par[0].u.intv )); }

static void call_func_comm_second_half( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_second_half( par[0].u.commv )); }

static void call_func_copy_comm( struct variable *result, struct variable par[] )
{ store_comm( result, func_copy_comm( par[0].u.commv )); }

static void call_iterator_data_access_routines( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_data_access_routines( self, par[0].u.intv )); }

static void call_func_datatype_array_get( struct variable *result, struct variable par[] )
{ store_datatype( result, func_datatype_array_get( par[0].u.darrv, par[1].u.intv )); }

static void call_func_datatype_array_set( struct variable *result, struct variable par[] )
{ func_datatype_array_set( par[0].u.darrv, par[1].u.intv, par[2].u.datatypev ); }

static void call_measure_delayed_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_delayed_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.doublev, par[5].u.intv )); }

static void call_init_delayed_Alltoall( struct variable *result, struct variable par[] )
{ init_delayed_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.doublev, par[5].u.intv ); }

static void call_measure_delayed_Gather( struct variable *result, struct variable par[] )
{ store_double( result, measure_delayed_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev, par[6].u.intv )); }

static void call_init_delayed_Gather( struct variable *result, struct variable par[] )
{ init_delayed_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev, par[6].u.intv ); }

static void call_func_dims_create( struct variable *result, struct variable par[] )
{ store_iarray( result, func_dims_create( par[0].u.intv, par[1].u.iarrv )); }

static void call_func_floor( struct variable *result, struct variable par[] )
{ store_int( result, func_floor( par[0].u.doublev )); }

static void call_func_get_comm_rank( struct variable *result, struct variable par[] )
{ store_int( result, func_get_comm_rank( par[0].u.commv )); }

static void call_func_get_comm_size( struct variable *result, struct variable par[] )
{ store_int( result, func_get_comm_size( par[0].u.commv )); }

static void call_func_get_env( struct variable *result, struct variable par[] )
{ store_string( result, func_get_env( par[0].u.stringv )); }

static void call_func_get_io_atomicity( struct variable *result, struct variable par[] )
{ store_int( result, func_get_io_atomicity(  )); }

static void call_func_get_io_datarep( struct variable *result, struct variable par[] )
{ store_string( result, func_get_io_datarep(  )); }

static void call_func_get_io_file_sync( struct variable *result, struct variable par[] )
{ store_int( result, func_get_io_file_sync(  )); }

static void call_func_get_io_preallocation( struct variable *result, struct variable par[] )
{ store_int( result, func_get_io_preallocation(  )); }

static void call_func_get_io_unique_open( struct variable *result, struct variable par[] )
{ store_int( result, func_get_io_unique_open(  )); }

static void call_func_get_io_working_dir( struct variable *result, struct variable par[] )
{ store_string( result, func_get_io_working_dir(  )); }

static void call_func_get_mpi_alloc_mem_info( struct variable *result, struct variable par[] )
{ store_info( result, func_get_mpi_alloc_mem_info(  )); }

static void call_func_get_np( struct variable *result, struct variable par[] )
{ store_int( result, func_get_np(  )); }

static void call_func_get_random_int( struct variable *result, struct variable par[] )
{ store_int( result, func_get_random_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_info_create( struct variable *result, struct variable par[] )
{ store_info( result, func_info_create(  )); }

static void call_func_info_free( struct variable *result, struct variable par[] )
{ func_info_free( par[0].u.infov ); }

static void call_func_info_set( struct variable *result, struct variable par[] )
{ func_info_set( par[0].u.infov, par[1].u.stringv, par[2].u.stringv ); }

static void call_func_init_time_accounting( struct variable *result, struct variable par[] )
{ func_init_time_accounting(  ); }

static void call_func_int_array_allgather( struct variable *result, struct variable par[] )
{ store_iarray( result, func_int_array_allgather( par[0].u.commv, par[1].u.intv )); }

static void call_func_int_array_get( struct variable *result, struct variable par[] )
{ store_int( result, func_int_array_get( par[0].u.iarrv, par[1].u.intv )); }

static void call_func_int_array_set( struct variable *result, struct variable par[] )
{ func_int_array_set( par[0].u.iarrv, par[1].u.intv, par[2].u.intv ); }

static void call_func_is_mpi_comm_null( struct variable *result, struct variable par[] )
{ store_int( result, func_is_mpi_comm_null( par[0].u.commv )); }

static void call_func_last_proc( struct variable *result, struct variable par[] )
{ store_int( result, func_last_proc(  )); }

static void call_func_log( struct variable *result, struct variable par[] )
{ store_double( result, func_log( par[0].u.doublev )); }

static void call_func_log10( struct variable *result, struct variable par[] )
{ store_double( result, func_log10( par[0].u.doublev )); }

static void call_func_log2( struct variable *result, struct variable par[] )
{ store_double( result, func_log2( par[0].u.doublev )); }

static void call_func_max_int( struct variable *result, struct variable par[] )
{ store_int( result, func_max_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_min_int( struct variable *result, struct variable par[] )
{ store_int( result, func_min_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_modulo( struct variable *result, struct variable par[] )
{ store_int( result, func_modulo( par[0].u.intv, par[1].u.intv )); }

static void call_func_mpi_processor_names( struct variable *result, struct variable par[] )
{ store_string( result, func_mpi_processor_names(  )); }

static void call_func_mpi_type_contiguous( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_contiguous( par[0].u.intv, par[1].u.datatypev )); }

static void call_func_mpi_type_create_darray( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_create_darray( par[0].u.intv, par[1].u.intv, par[2].u.iarrv, par[3].u.iarrv, par[4].u.iarrv, par[5].u.iarrv, par[6].u.datatypev )); }

static void call_func_mpi_type_create_subarray( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_create_subarray( par[0].u.iarrv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.datatypev )); }

static void call_func_mpi_type_hindexed( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_hindexed( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev )); }

static void call_func_mpi_type_hvector( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_hvector( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev )); }

static void call_func_mpi_type_indexed( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_indexed( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev )); }

static void call_func_mpi_type_struct( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_struct( par[0].u.iarrv, par[1].u.iarrv, par[2].u.darrv )); }

static void call_func_mpi_type_vector( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_vector( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev )); }

static void call_measure_myalltoallv( struct variable *result, struct variable par[] )
{ store_double( result, measure_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev )); }

static void call_init_myalltoallv( struct variable *result, struct variable par[] )
{ init_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev ); }

static void call_finalize_myalltoallv( struct variable *result, struct variable par[] )
{ finalize_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev ); }

static void call_func_power( struct variable *result, struct variable par[] )
{ store_double( result, func_power( par[0].u.doublev, par[1].u.doublev )); }

static void call_func_print_data_access_settings( struct variable *result, struct variable par[] )
{ func_print_data_access_settings( par[0].u.intv, par[1].u.intv ); }

static void call_func_print_file_info( struct variable *result, struct variable par[] )
{ func_print_file_info(  ); }

static void call_func_print_time_accounting_info( struct variable *result, struct variable par[] )
{ func_print_time_accounting_info(  ); }

static void call_iterator_range( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_range( self, par[0].u.intv, par[1].u.intv )); }

static void call_func_reset_file_info( struct variable *result, struct variable par[] )
{ func_reset_file_info(  ); }

static void call_func_round( struct variable *result, struct variable par[] )
{ store_int( result, func_round( par[0].u.doublev )); }

static void call_func_set_autorefine_factor( struct variable *result, struct variable par[] )
{ func_set_autorefine_factor( par[0].u.doublev ); }

static void call_func_set_cache_size( struct variable *result, struct variable par[] )
{ func_set_cache_size( par[0].u.intv ); }

static void call_func_set_file_info( struct variable *result, struct variable par[] )
{ func_set_file_info( par[0].u.stringv, par[1].u.stringv ); }

static void call_func_set_io_atomicity( struct variable *result, struct variable par[] )
{ func_set_io_atomicity( par[0].u.intv ); }

static void call_func_set_io_datarep( struct variable *result, struct variable par[] )
{ func_set_io_datarep( par[0].u.stringv ); }

static void call_func_set_io_file_sync( struct variable *result, struct variable par[] )
{ func_set_io_file_sync( par[0].u.intv ); }

static void call_func_set_io_preallocation( struct variable *result, struct variable par[] )
{ func_set_io_preallocation( par[0].u.intv ); }

static void call_func_set_io_unique_open( struct variable *result, struct variable par[] )
{ func_set_io_unique_open( par[0].u.intv ); }

static void call_func_set_io_working_dir( struct variable *result, struct variable par[] )
{ func_set_io_working_dir( par[0].u.stringv ); }

static void call_func_set_mpi_alloc_mem_info( struct variable *result, struct variable par[] )
{ func_set_mpi_alloc_mem_info( par[0].u.infov ); }

static void call_func_set_recv_buffer_alignment( struct variable *result, struct variable par[] )
{ func_set_recv_buffer_alignment( par[0].u.intv ); }

static void call_func_set_send_buffer_alignment( struct variable *result, struct variable par[] )
{ func_set_send_buffer_alignment( par[0].u.intv ); }

static void call_func_set_skampi_buffer( struct variable *result, struct variable par[] )
{ func_set_skampi_buffer( par[0].u.intv ); }

static void call_func_sqr( struct variable *result, struct variable par[] )
{ store_double( result, func_sqr( par[0].u.doublev )); }

static void call_func_sqrt( struct variable *result, struct variable par[] )
{ store_double( result, func_sqrt( par[0].u.doublev )); }

static void call_iterator_squares( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_squares( self, par[0].u.intv, par[1].u.intv )); }

static void call_func_switch_buffer_cycling_off( struct variable *result, struct variable par[] )
{ func_switch_buffer_cycling_off(  ); }

static void call_func_switch_buffer_cycling_on( struct variable *result, struct variable par[] )
{ func_switch_buffer_cycling_on(  ); }

static void call_func_topology_cartesian( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_cartesian( par[0].u.commv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.intv )); }

static void call_func_topology_graph( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_graph( par[0].u.commv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.intv )); }

static void call_func_topology_graph2( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_graph2( par[0].u.commv, par[1].u.iarrv, par[2].u.intv )); }

static void call_func_use_mpi_alloc_mem( struct variable *result, struct variable par[] )
{ func_use_mpi_alloc_mem( par[0].u.intv ); }

static void call_measure_waitpattern_down( struct variable *result, struct variable par[] )
{ store_double( result, measure_waitpattern_down( par[0].u.doublev )); }

static void call_init_waitpattern_down( struct variable *result, struct variable par[] )
{ init_waitpattern_down( par[0].u.doublev ); }

static void call_measure_waitpattern_up( struct variable *result, struct variable par[] )
{ store_double( result, measure_waitpattern_up( par[0].u.doublev )); }

static void call_init_waitpattern_up( struct variable *result, struct variable par[] )
{ init_waitpattern_up( par[0].u.doublev ); }

void init_functions(void)
{
  insert_function( &call_init_Allgather, &call_measure_Allgather, NULL, "Allgather", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Allgatherv, &call_measure_Allgatherv, &call_finalize_Allgatherv, "Allgatherv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Allreduce, &call_measure_Allreduce, NULL, "Allreduce", "ido", TYPE_DOUBLE );
  insert_function( &call_init_Allreduce_using_Reduce_Bcast, &call_measure_Allreduce_using_Reduce_Bcast, NULL, "Allreduce_using_Reduce_Bcast", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Alltoall, &call_measure_Alltoall, NULL, "Alltoall", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoall_using_Isend_Irecv, &call_measure_Alltoall_using_Isend_Irecv, &call_finalize_Alltoall_using_Isend_Irecv, "Alltoall_using_Isend_Irecv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoallv, &call_measure_Alltoallv, &call_finalize_Alltoallv, "Alltoallv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoallv_using_Isend_Irecv, &call_measure_Alltoallv_using_Isend_Irecv, &call_finalize_Alltoallv_using_Isend_Irecv, "Alltoallv_using_Isend_Irecv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Barrier, &call_measure_Barrier, NULL, "Barrier", "", TYPE_DOUBLE );
  insert_function( &call_init_Bcast, &call_measure_Bcast, NULL, "Bcast", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Bcast_using_Send_Recv, &call_measure_Bcast_using_Send_Recv, &call_finalize_Bcast_using_Send_Recv, "Bcast_using_Send_Recv", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Comm_dup, &call_measure_Comm_dup, NULL, "Comm_dup", "", TYPE_DOUBLE );
  insert_function( &call_init_Comm_split, &call_measure_Comm_split, NULL, "Comm_split", "", TYPE_DOUBLE );
  insert_function( &call_init_Gather, &call_measure_Gather, NULL, "Gather", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gather_using_Isend_Irecv_Waitall, &call_measure_Gather_using_Isend_Irecv_Waitall, &call_finalize_Gather_using_Isend_Irecv_Waitall, "Gather_using_Isend_Irecv_Waitall", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gather_using_Send_Recv, &call_measure_Gather_using_Send_Recv, NULL, "Gather_using_Send_Recv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gatherv, &call_measure_Gatherv, &call_finalize_Gatherv, "Gatherv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_close, &call_measure_MPI_IO_close, &call_finalize_MPI_IO_close, "MPI_IO_close", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_delete, &call_measure_MPI_IO_delete, &call_finalize_MPI_IO_delete, "MPI_IO_delete", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_delete_on_close, &call_measure_MPI_IO_delete_on_close, &call_finalize_MPI_IO_delete_on_close, "MPI_IO_delete_on_close", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_file_seek, &call_measure_MPI_IO_file_seek, &call_finalize_MPI_IO_file_seek, "MPI_IO_file_seek", "iiddd", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_file_seek_shared, &call_measure_MPI_IO_file_seek_shared, &call_finalize_MPI_IO_file_seek_shared, "MPI_IO_file_seek_shared", "iiddd", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_open, &call_measure_MPI_IO_open, &call_finalize_MPI_IO_open, "MPI_IO_open", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_open_close, &call_measure_MPI_IO_open_close, &call_finalize_MPI_IO_open_close, "MPI_IO_open_close", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_preallocate, &call_measure_MPI_IO_preallocate, &call_finalize_MPI_IO_preallocate, "MPI_IO_preallocate", "ii", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_and_distribute, &call_measure_MPI_IO_read_and_distribute, &call_finalize_MPI_IO_read_and_distribute, "MPI_IO_read_and_distribute", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_file, &call_measure_MPI_IO_read_file, &call_finalize_MPI_IO_read_file, "MPI_IO_read_file", "idddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_noncontiguous, &call_measure_MPI_IO_read_noncontiguous, &call_finalize_MPI_IO_read_noncontiguous, "MPI_IO_read_noncontiguous", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_once, &call_measure_MPI_IO_read_once, &call_finalize_MPI_IO_read_once, "MPI_IO_read_once", "idddii", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_random, &call_measure_MPI_IO_read_random, &call_finalize_MPI_IO_read_random, "MPI_IO_read_random", "aadddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_segmented, &call_measure_MPI_IO_read_segmented, &call_finalize_MPI_IO_read_segmented, "MPI_IO_read_segmented", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_separate, &call_measure_MPI_IO_read_separate, &call_finalize_MPI_IO_read_separate, "MPI_IO_read_separate", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_sequential, &call_measure_MPI_IO_read_sequential, &call_finalize_MPI_IO_read_sequential, "MPI_IO_read_sequential", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_strided, &call_measure_MPI_IO_read_strided, &call_finalize_MPI_IO_read_strided, "MPI_IO_read_strided", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_and_read, &call_measure_MPI_IO_write_and_read, &call_finalize_MPI_IO_write_and_read, "MPI_IO_write_and_read", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_noncontiguous, &call_measure_MPI_IO_write_noncontiguous, &call_finalize_MPI_IO_write_noncontiguous, "MPI_IO_write_noncontiguous", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_once, &call_measure_MPI_IO_write_once, &call_finalize_MPI_IO_write_once, "MPI_IO_write_once", "idddii", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_random, &call_measure_MPI_IO_write_random, &call_finalize_MPI_IO_write_random, "MPI_IO_write_random", "aadddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_segmented, &call_measure_MPI_IO_write_segmented, &call_finalize_MPI_IO_write_segmented, "MPI_IO_write_segmented", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_separate, &call_measure_MPI_IO_write_separate, &call_finalize_MPI_IO_write_separate, "MPI_IO_write_separate", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_sequential, &call_measure_MPI_IO_write_sequential, &call_finalize_MPI_IO_write_sequential, "MPI_IO_write_sequential", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_strided, &call_measure_MPI_IO_write_strided, &call_finalize_MPI_IO_write_strided, "MPI_IO_write_strided", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_Par_Pingpong_Send_Recv, &call_measure_Par_Pingpong_Send_Recv, NULL, "Par_Pingpong_Send_Recv", "idiiii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Bsend_Recv, &call_measure_Pingpong_Bsend_Recv, NULL, "Pingpong_Bsend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Isend_Recv, &call_measure_Pingpong_Isend_Recv, NULL, "Pingpong_Isend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Issend_Recv, &call_measure_Pingpong_Issend_Recv, NULL, "Pingpong_Issend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Iprobe_Recv, &call_measure_Pingpong_Send_Iprobe_Recv, NULL, "Pingpong_Send_Iprobe_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Irecv, &call_measure_Pingpong_Send_Irecv, NULL, "Pingpong_Send_Irecv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Recv, &call_measure_Pingpong_Send_Recv, NULL, "Pingpong_Send_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Recv_AT, &call_measure_Pingpong_Send_Recv_AT, NULL, "Pingpong_Send_Recv_AT", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Sendrecv, &call_measure_Pingpong_Sendrecv, NULL, "Pingpong_Sendrecv", "idiidii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Sendrecv_replace, &call_measure_Pingpong_Sendrecv_replace, NULL, "Pingpong_Sendrecv_replace", "idiii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Ssend_Recv, &call_measure_Pingpong_Ssend_Recv, NULL, "Pingpong_Ssend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Reduce, &call_measure_Reduce, NULL, "Reduce", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce_Scatterv, &call_measure_Reduce_Scatterv, &call_finalize_Reduce_Scatterv, "Reduce_Scatterv", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce_scatter, &call_measure_Reduce_scatter, &call_finalize_Reduce_scatter, "Reduce_scatter", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Scan, &call_measure_Scan, NULL, "Scan", "ido", TYPE_DOUBLE );
  insert_function( &call_init_Scatter, &call_measure_Scatter, NULL, "Scatter", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Scatterv, &call_measure_Scatterv, &call_finalize_Scatterv, "Scatterv", "ididi", TYPE_DOUBLE );
  insert_function( NULL, &call_measure_Wtime, NULL, "Wtime", "i", TYPE_DOUBLE );
  insert_function( NULL, &call_func_alloc_datatype_array, NULL, "alloc_datatype_array", "i", TYPE_DARRAY );
  insert_function( NULL, &call_func_alloc_int_array, NULL, "alloc_int_array", "i", TYPE_IARRAY );
  insert_function( NULL, &call_func_atoi, NULL, "atoi", "s", TYPE_INTEGER );
  insert_function( NULL, &call_func_cbrt, NULL, "cbrt", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_ceil, NULL, "ceil", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_choose_barrier_synchronization, NULL, "choose_barrier_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_choose_no_synchronization, NULL, "choose_no_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_choose_real_synchronization, NULL, "choose_real_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_comm, NULL, "comm", "i", TYPE_COMM );
  insert_function( NULL, &call_func_comm2, NULL, "comm2", "ii", TYPE_COMM );
  insert_function( NULL, &call_func_comm2_max_latency_with_root, NULL, "comm2_max_latency_with_root", "", TYPE_COMM );
  insert_function( NULL, &call_func_comm4, NULL, "comm4", "iiii", TYPE_COMM );
  insert_function( NULL, &call_func_comm_first_half, NULL, "comm_first_half", "c", TYPE_COMM );
  insert_function( NULL, &call_func_comm_from_to, NULL, "comm_from_to", "ii", TYPE_COMM );
  insert_function( NULL, &call_func_comm_rand, NULL, "comm_rand", "i", TYPE_COMM );
  insert_function( NULL, &call_func_comm_second_half, NULL, "comm_second_half", "c", TYPE_COMM );
  insert_function( NULL, &call_func_copy_comm, NULL, "copy_comm", "c", TYPE_COMM );
  insert_iterator( &call_iterator_data_access_routines, "data_access_routines", "i", TYPE_INTEGER );
  insert_function( NULL, &call_func_datatype_array_get, NULL, "datatype_array_get", "ei", TYPE_DATATYPE );
  insert_function( NULL, &call_func_datatype_array_set, NULL, "datatype_array_set", "eid", TYPE_VOID );
  insert_function( &call_init_delayed_Alltoall, &call_measure_delayed_Alltoall, NULL, "delayed_Alltoall", "ididfi", TYPE_DOUBLE );
  insert_function( &call_init_delayed_Gather, &call_measure_delayed_Gather, NULL, "delayed_Gather", "ididifi", TYPE_DOUBLE );
  insert_function( NULL, &call_func_dims_create, NULL, "dims_create", "ia", TYPE_IARRAY );
  insert_function( NULL, &call_func_floor, NULL, "floor", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_comm_rank, NULL, "get_comm_rank", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_comm_size, NULL, "get_comm_size", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_env, NULL, "get_env", "s", TYPE_STRING );
  insert_function( NULL, &call_func_get_io_atomicity, NULL, "get_io_atomicity", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_io_datarep, NULL, "get_io_datarep", "", TYPE_STRING );
  insert_function( NULL, &call_func_get_io_file_sync, NULL, "get_io_file_sync", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_io_preallocation, NULL, "get_io_preallocation", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_io_unique_open, NULL, "get_io_unique_open", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_io_working_dir, NULL, "get_io_working_dir", "", TYPE_STRING );
  insert_function( NULL, &call_func_get_mpi_alloc_mem_info, NULL, "get_mpi_alloc_mem_info", "", TYPE_INFO );
  insert_function( NULL, &call_func_get_np, NULL, "get_np", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_random_int, NULL, "get_random_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_info_create, NULL, "info_create", "", TYPE_INFO );
  insert_function( NULL, &call_func_info_free, NULL, "info_free", "h", TYPE_VOID );
  insert_function( NULL, &call_func_info_set, NULL, "info_set", "hss", TYPE_VOID );
  insert_function( NULL, &call_func_init_time_accounting, NULL, "init_time_accounting", "", TYPE_VOID );
  insert_function( NULL, &call_func_int_array_allgather, NULL, "int_array_allgather", "ci", TYPE_IARRAY );
  insert_function( NULL, &call_func_int_array_get, NULL, "int_array_get", "ai", TYPE_INTEGER );
  insert_function( NULL, &call_func_int_array_set, NULL, "int_array_set", "aii", TYPE_VOID );
  insert_function( NULL, &call_func_is_mpi_comm_null, NULL, "is_mpi_comm_null", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_last_proc, NULL, "last_proc", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_log, NULL, "log", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_log10, NULL, "log10", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_log2, NULL, "log2", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_max_int, NULL, "max_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_min_int, NULL, "min_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_modulo, NULL, "modulo", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_mpi_processor_names, NULL, "mpi_processor_names", "", TYPE_STRING );
  insert_function( NULL, &call_func_mpi_type_contiguous, NULL, "mpi_type_contiguous", "id", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_create_darray, NULL, "mpi_type_create_darray", "iiaaaad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_create_subarray, NULL, "mpi_type_create_subarray", "aaad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_hindexed, NULL, "mpi_type_hindexed", "aad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_hvector, NULL, "mpi_type_hvector", "iiid", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_indexed, NULL, "mpi_type_indexed", "aad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_struct, NULL, "mpi_type_struct", "aae", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_vector, NULL, "mpi_type_vector", "iiid", TYPE_DATATYPE );
  insert_function( &call_init_myalltoallv, &call_measure_myalltoallv, &call_finalize_myalltoallv, "myalltoallv", "ididif", TYPE_DOUBLE );
  insert_function( NULL, &call_func_power, NULL, "power", "ff", TYPE_DOUBLE );
  insert_function( NULL, &call_func_print_data_access_settings, NULL, "print_data_access_settings", "ii", TYPE_VOID );
  insert_function( NULL, &call_func_print_file_info, NULL, "print_file_info", "", TYPE_VOID );
  insert_function( NULL, &call_func_print_time_accounting_info, NULL, "print_time_accounting_info", "", TYPE_VOID );
  insert_iterator( &call_iterator_range, "range", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_reset_file_info, NULL, "reset_file_info", "", TYPE_VOID );
  insert_function( NULL, &call_func_round, NULL, "round", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_set_autorefine_factor, NULL, "set_autorefine_factor", "f", TYPE_VOID );
  insert_function( NULL, &call_func_set_cache_size, NULL, "set_cache_size", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_file_info, NULL, "set_file_info", "ss", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_atomicity, NULL, "set_io_atomicity", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_datarep, NULL, "set_io_datarep", "s", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_file_sync, NULL, "set_io_file_sync", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_preallocation, NULL, "set_io_preallocation", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_unique_open, NULL, "set_io_unique_open", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_working_dir, NULL, "set_io_working_dir", "s", TYPE_VOID );
  insert_function( NULL, &call_func_set_mpi_alloc_mem_info, NULL, "set_mpi_alloc_mem_info", "h", TYPE_VOID );
  insert_function( NULL, &call_func_set_recv_buffer_alignment, NULL, "set_recv_buffer_alignment", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_send_buffer_alignment, NULL, "set_send_buffer_alignment", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_skampi_buffer, NULL, "set_skampi_buffer", "i", TYPE_VOID );
  insert_function( NULL, &call_func_sqr, NULL, "sqr", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_sqrt, NULL, "sqrt", "f", TYPE_DOUBLE );
  insert_iterator( &call_iterator_squares, "squares", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_switch_buffer_cycling_off, NULL, "switch_buffer_cycling_off", "", TYPE_VOID );
  insert_function( NULL, &call_func_switch_buffer_cycling_on, NULL, "switch_buffer_cycling_on", "", TYPE_VOID );
  insert_function( NULL, &call_func_topology_cartesian, NULL, "topology_cartesian", "caai", TYPE_COMM );
  insert_function( NULL, &call_func_topology_graph, NULL, "topology_graph", "caai", TYPE_COMM );
  insert_function( NULL, &call_func_topology_graph2, NULL, "topology_graph2", "cai", TYPE_COMM );
  insert_function( NULL, &call_func_use_mpi_alloc_mem, NULL, "use_mpi_alloc_mem", "i", TYPE_VOID );
  insert_function( &call_init_waitpattern_down, &call_measure_waitpattern_down, NULL, "waitpattern_down", "f", TYPE_DOUBLE );
  insert_function( &call_init_waitpattern_up, &call_measure_waitpattern_up, NULL, "waitpattern_up", "f", TYPE_DOUBLE );
}

#endif


#if defined(USE_ONESIDED) && !defined(USE_MPI_IO)

extern double measure_Accumulate( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation );
extern void init_Accumulate( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation );
extern double measure_Accumulate_activewait_twosided( int count, MPI_Info info );
extern void init_Accumulate_activewait_twosided( int count, MPI_Info info );
extern double measure_Accumulate_concurrent( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation, int disjoint );
extern void init_Accumulate_concurrent( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation, int disjoint );
extern double measure_Accumulate_concurrent_multi( int count, MPI_Datatype datatype, MPI_Info info, int msg_count, MPI_Op operator, int disjoint );
extern void init_Accumulate_concurrent_multi( int count, MPI_Datatype datatype, MPI_Info info, int msg_count, MPI_Op operator, int disjoint );
extern double measure_Allgather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Allgather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Allgatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Allgatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Allgatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Allreduce( int count, MPI_Datatype dt, MPI_Op op );
extern void init_Allreduce( int count, MPI_Datatype dt, MPI_Op op );
extern double measure_Allreduce_using_Reduce_Bcast( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Allreduce_using_Reduce_Bcast( int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoall_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoall_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoall_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoallv_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoallv_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoallv_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Barrier( void );
extern void init_Barrier( void );
extern double measure_Bcast( int count, MPI_Datatype dt, int root );
extern void init_Bcast( int count, MPI_Datatype dt, int root );
extern double measure_Bcast_using_Send_Recv( int count, MPI_Datatype dt, int root );
extern void init_Bcast_using_Send_Recv( int count, MPI_Datatype dt, int root );
extern void finalize_Bcast_using_Send_Recv(int count, MPI_Datatype dt, int root );
extern double measure_Caching( int count, MPI_Datatype datatype, MPI_Info info, int delay_us );
extern void init_Caching( int count, MPI_Datatype datatype, MPI_Info info, int delay_us );
extern double measure_Combining_Put_dedicated( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern void init_Combining_Put_dedicated( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern double measure_Combining_Put_fence( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern void init_Combining_Put_fence( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern double measure_Comm_dup( void );
extern void init_Comm_dup( void );
extern double measure_Comm_split( void );
extern void init_Comm_split( void );
extern double measure_Datatype_complex_Get( int count, MPI_Info info );
extern void init_Datatype_complex_Get( int count, MPI_Info info );
extern double measure_Datatype_complex_everytime( int count, MPI_Info info );
extern void init_Datatype_complex_everytime( int count, MPI_Info info );
extern double measure_Datatype_complex_mixed_everytime( int count, MPI_Info info );
extern void init_Datatype_complex_mixed_everytime( int count, MPI_Info info );
extern double measure_Datatype_complex_mixed_once( int count, MPI_Info info );
extern void init_Datatype_complex_mixed_once( int count, MPI_Info info );
extern void finalize_Datatype_complex_mixed_once(int count, MPI_Info info );
extern double measure_Datatype_complex_once( int count, MPI_Info info );
extern void init_Datatype_complex_once( int count, MPI_Info info );
extern void finalize_Datatype_complex_once(int count, MPI_Info info );
extern double measure_Datatype_int( int count, MPI_Info info );
extern void init_Datatype_int( int count, MPI_Info info );
extern double measure_Datatype_simple( int count, MPI_Info info );
extern void init_Datatype_simple( int count, MPI_Info info );
extern double measure_Datatype_simple_Get( int count, MPI_Info info );
extern void init_Datatype_simple_Get( int count, MPI_Info info );
extern double measure_Exchange( int count, MPI_Datatype datatype, int msg_count );
extern void init_Exchange( int count, MPI_Datatype datatype, int msg_count );
extern double measure_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gather_using_Isend_Irecv_Waitall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather_using_Isend_Irecv_Waitall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Gather_using_Isend_Irecv_Waitall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gather_using_Send_Recv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather_using_Send_Recv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Gatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Get_Pingpong( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_Pingpong( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Get_activewait( int count, MPI_Info info );
extern void init_Get_activewait( int count, MPI_Info info );
extern double measure_Get_activewait_sleep( int count, MPI_Info info, int delay_us );
extern void init_Get_activewait_sleep( int count, MPI_Info info, int delay_us );
extern double measure_Get_callduration( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_callduration( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Get_dedicated( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_dedicated( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Get_fence( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_fence( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Get_passive( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_passive( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Isend_callduration( int count, MPI_Datatype datatype );
extern void init_Isend_callduration( int count, MPI_Datatype datatype );
extern double measure_Par_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize );
extern void init_Par_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize );
extern double measure_Pingpong_Bsend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Bsend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Isend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Isend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Issend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Issend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Put_Put( int count, MPI_Datatype datatype, MPI_Info info, int iterations );
extern void init_Pingpong_Put_Put( int count, MPI_Datatype datatype, MPI_Info info, int iterations );
extern double measure_Pingpong_Send_Iprobe_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Iprobe_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Irecv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Irecv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Recv_AT( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Recv_AT( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Sendrecv( int send_count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations );
extern void init_Pingpong_Sendrecv( int count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations );
extern double measure_Pingpong_Sendrecv_replace( int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations );
extern void init_Pingpong_Sendrecv_replace( int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations );
extern double measure_Pingpong_Ssend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Ssend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Put( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_Exchange( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern void init_Put_Exchange( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern double measure_Put_Exchange_passive( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern void init_Put_Exchange_passive( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern double measure_Put_Shift( int count, MPI_Datatype datatype, MPI_Info info, int distance );
extern void init_Put_Shift( int count, MPI_Datatype datatype, MPI_Info info, int distance );
extern double measure_Put_activewait_get( int count, MPI_Info info );
extern void init_Put_activewait_get( int count, MPI_Info info );
extern double measure_Put_activewait_twosided( int count, MPI_Info info );
extern void init_Put_activewait_twosided( int count, MPI_Info info );
extern double measure_Put_activewait_twosided_sleep( int count, MPI_Info info, int delay_us );
extern void init_Put_activewait_twosided_sleep( int count, MPI_Info info, int delay_us );
extern double measure_Put_dedicated( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put_dedicated( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_fence( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put_fence( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_fence_bidirectional( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put_fence_bidirectional( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_passive( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put_passive( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_passive_concurrent( int count, MPI_Datatype datatype, MPI_Info info, int lock_type );
extern void init_Put_passive_concurrent( int count, MPI_Datatype datatype, MPI_Info info, int lock_type );
extern double measure_Reduce( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce( int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Reduce_Scatterv( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce_Scatterv( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void finalize_Reduce_Scatterv(int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Reduce_scatter( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce_scatter( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void finalize_Reduce_scatter(int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Scan( int count, MPI_Datatype dt, MPI_Op op );
extern void init_Scan( int count, MPI_Datatype dt, MPI_Op op );
extern double measure_Scatter( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Scatter( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Scatterv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Scatterv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Scatterv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Send( int count, MPI_Datatype datatype, int tag );
extern void init_Send( int count, MPI_Datatype datatype, int tag );
extern double measure_Win_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_complete_delayed_wait( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern void init_Win_complete_delayed_wait( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern double measure_Win_complete_n( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int msg_count, int delay_us );
extern void init_Win_complete_n( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int msg_count, int delay_us );
extern double measure_Win_create( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Win_create( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Win_fence_close( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_fence_close( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_fence_close_collective( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_fence_close_collective( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_fence_close_delayed( int count, MPI_Datatype datatype, MPI_Info info, int msg_count, int delay_us );
extern void init_Win_fence_close_delayed( int count, MPI_Datatype datatype, MPI_Info info, int msg_count, int delay_us );
extern double measure_Win_fence_open( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern void init_Win_fence_open( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern double measure_Win_fence_openclose( int count, MPI_Datatype datatype, MPI_Info info, int do_a_put );
extern void init_Win_fence_openclose( int count, MPI_Datatype datatype, MPI_Info info, int do_a_put );
extern double measure_Win_free( int count, MPI_Datatype datatype, MPI_Info info, int do_a_put );
extern void init_Win_free( int count, MPI_Datatype datatype, MPI_Info info, int do_a_put );
extern double measure_Win_lock( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank );
extern void init_Win_lock( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank );
extern double measure_Win_lock_desync( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int delay_us );
extern void init_Win_lock_desync( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int delay_us );
extern double measure_Win_post( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern void init_Win_post( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern double measure_Win_start( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern void init_Win_start( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern double measure_Win_start_delayed_post( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int delay_us, int delay_rank );
extern void init_Win_start_delayed_post( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int delay_us, int delay_rank );
extern double measure_Win_startcomplete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_startcomplete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_test( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_test( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_test_delayed( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern void init_Win_test_delayed( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern double measure_Win_unlock( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int do_a_put, int delay_us );
extern void init_Win_unlock( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int do_a_put, int delay_us );
extern double measure_Win_wait_delayed_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern void init_Win_wait_delayed_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern double measure_Win_wait_early_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern void init_Win_wait_early_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern double measure_Wtime( int iterations );
extern DArray func_alloc_datatype_array( int size );
extern IArray func_alloc_int_array( int size );
extern int func_atoi( char* s );
extern double func_cbrt( double x );
extern int func_ceil( double x );
extern void func_choose_barrier_synchronization( void );
extern void func_choose_no_synchronization( void );
extern void func_choose_real_synchronization( void );
extern MPI_Comm func_comm( int i );
extern MPI_Comm func_comm2( int a, int b );
extern MPI_Comm func_comm2_max_latency_with_root( void );
extern MPI_Comm func_comm4( int a, int b, int c, int d );
extern MPI_Comm func_comm_first_half( MPI_Comm c );
extern MPI_Comm func_comm_from_to( int first, int last );
extern MPI_Comm func_comm_rand( int n );
extern MPI_Comm func_comm_second_half( MPI_Comm c );
extern MPI_Comm func_copy_comm( MPI_Comm c );
extern MPI_Datatype func_datatype_array_get( DArray darr, int i );
extern void func_datatype_array_set( DArray darr, int i, MPI_Datatype v );
extern double measure_delayed_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node );
extern void init_delayed_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node );
extern double measure_delayed_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node );
extern void init_delayed_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node );
extern IArray func_dims_create( int nnodes, IArray dims );
extern int func_floor( double x );
extern int func_get_comm_rank( MPI_Comm comm );
extern int func_get_comm_size( MPI_Comm comm );
extern char* func_get_env( char*name );
extern MPI_Info func_get_mpi_alloc_mem_info( void );
extern int func_get_np( void );
extern int func_get_random_int( int min_value, int max_value );
extern MPI_Info func_info_create( void );
extern void func_info_free( MPI_Info info );
extern void func_info_set( MPI_Info info, char* key, char* value );
extern void func_init_time_accounting( void );
extern int func_int_array_get( IArray iarr, int i );
extern void func_int_array_set( IArray iarr, int i, int v );
extern int func_is_mpi_comm_null( MPI_Comm comm );
extern int func_last_proc( void );
extern double func_log( double a );
extern double func_log10( double a );
extern double func_log2( double a );
extern int func_max_int( int a, int b );
extern int func_min_int( int a, int b );
extern int func_modulo( int a, int b );
extern char* func_mpi_processor_names( void );
extern MPI_Datatype func_mpi_type_contiguous( int count, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_hindexed( IArray blocklengths, IArray displacements, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_hvector( int count, int length, int stride, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_indexed( IArray blocklengths, IArray displacements, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_struct( IArray blocklengths, IArray displacements, DArray dtypes );
extern MPI_Datatype func_mpi_type_vector( int count, int length, int stride, MPI_Datatype oldtype );
extern double measure_multi_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int alltoall_count );
extern void init_multi_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int alltoall_count );
extern double measure_myalltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern void init_myalltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern void finalize_myalltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern double measure_onesided_allreduce( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op );
extern void init_onesided_allreduce( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op );
extern double measure_onesided_alltoall( int count, MPI_Datatype datatype, MPI_Info info, int alltoall_count );
extern void init_onesided_alltoall( int count, MPI_Datatype datatype, MPI_Info info, int alltoall_count );
extern double measure_onesided_bcast( int count, MPI_Datatype datatype, MPI_Info info, int bcast_count );
extern void init_onesided_bcast( int count, MPI_Datatype datatype, MPI_Info info, int bcast_count );
extern double measure_onesided_borderexchange( int count, MPI_Info info, int iteration_count, int compute );
extern void init_onesided_borderexchange( int elemcount, MPI_Info info, int iteration_count, int compute );
extern void finalize_onesided_borderexchange(int elemcount, MPI_Info info, int iteration_count, int compute );
extern double measure_onesided_borderexchange_fence( int count, MPI_Info info, int iteration_count, int compute );
extern void init_onesided_borderexchange_fence( int elemcount, MPI_Info info, int iteration_count, int compute );
extern double measure_onesided_dummy( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_onesided_dummy( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_onesided_reduce( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op );
extern void init_onesided_reduce( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op );
extern double func_power( double a, double b );
extern void func_print_time_accounting_info( void );
extern int iterator_range( void** self, int start, int end );
extern int func_round( double x );
extern int func_round_to_fivebytes( int size );
extern int func_round_to_fourbytes( int size );
extern void func_set_autorefine_factor( double f );
extern void func_set_cache_size( int size );
extern void func_set_mpi_alloc_mem_info( MPI_Info info );
extern void func_set_recv_buffer_alignment( int a );
extern void func_set_send_buffer_alignment( int a );
extern void func_set_skampi_buffer( int size );
extern void func_set_skampi_buffer_mpi_alloc_mem( int size, MPI_Info info );
extern double func_sqr( double x );
extern double func_sqrt( double x );
extern int iterator_squares( void** self, int start, int end );
extern void func_switch_buffer_cycling_off( void );
extern void func_switch_buffer_cycling_on( void );
extern MPI_Comm func_topology_cartesian( MPI_Comm c, IArray dims, IArray periodic, int reorder );
extern MPI_Comm func_topology_graph( MPI_Comm c, IArray index, IArray edges, int reorder );
extern MPI_Comm func_topology_graph2( MPI_Comm c, IArray graph, int reorder );
extern double measure_twosided_borderexchange( int count, int iteration_count, int compute );
extern void init_twosided_borderexchange( int elemcount, int iteration_count, int compute );
extern void finalize_twosided_borderexchange(int count, int iteration_count, int compute );
extern void func_use_mpi_alloc_mem( int flag );
extern double measure_waitpattern_down( double scale );
extern void init_waitpattern_down( double scale );
extern double measure_waitpattern_up( double scale );
extern void init_waitpattern_up( double scale );

static void call_measure_Accumulate( struct variable *result, struct variable par[] )
{ store_double( result, measure_Accumulate( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv )); }

static void call_init_Accumulate( struct variable *result, struct variable par[] )
{ init_Accumulate( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv ); }

static void call_measure_Accumulate_activewait_twosided( struct variable *result, struct variable par[] )
{ store_double( result, measure_Accumulate_activewait_twosided( par[0].u.intv, par[1].u.infov )); }

static void call_init_Accumulate_activewait_twosided( struct variable *result, struct variable par[] )
{ init_Accumulate_activewait_twosided( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Accumulate_concurrent( struct variable *result, struct variable par[] )
{ store_double( result, measure_Accumulate_concurrent( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv, par[4].u.intv )); }

static void call_init_Accumulate_concurrent( struct variable *result, struct variable par[] )
{ init_Accumulate_concurrent( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv, par[4].u.intv ); }

static void call_measure_Accumulate_concurrent_multi( struct variable *result, struct variable par[] )
{ store_double( result, measure_Accumulate_concurrent_multi( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.opv, par[5].u.intv )); }

static void call_init_Accumulate_concurrent_multi( struct variable *result, struct variable par[] )
{ init_Accumulate_concurrent_multi( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.opv, par[5].u.intv ); }

static void call_measure_Allgather( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allgather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Allgather( struct variable *result, struct variable par[] )
{ init_Allgather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Allgatherv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Allgatherv( struct variable *result, struct variable par[] )
{ init_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Allgatherv( struct variable *result, struct variable par[] )
{ finalize_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Allreduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv )); }

static void call_init_Allreduce( struct variable *result, struct variable par[] )
{ init_Allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv ); }

static void call_measure_Allreduce_using_Reduce_Bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allreduce_using_Reduce_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Allreduce_using_Reduce_Bcast( struct variable *result, struct variable par[] )
{ init_Allreduce_using_Reduce_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoall( struct variable *result, struct variable par[] )
{ init_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ init_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ finalize_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoallv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoallv( struct variable *result, struct variable par[] )
{ init_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoallv( struct variable *result, struct variable par[] )
{ finalize_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ init_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ finalize_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Barrier( struct variable *result, struct variable par[] )
{ store_double( result, measure_Barrier(  )); }

static void call_init_Barrier( struct variable *result, struct variable par[] )
{ init_Barrier(  ); }

static void call_measure_Bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Bcast( struct variable *result, struct variable par[] )
{ init_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ init_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_finalize_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ finalize_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Caching( struct variable *result, struct variable par[] )
{ store_double( result, measure_Caching( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Caching( struct variable *result, struct variable par[] )
{ init_Caching( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Combining_Put_dedicated( struct variable *result, struct variable par[] )
{ store_double( result, measure_Combining_Put_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Combining_Put_dedicated( struct variable *result, struct variable par[] )
{ init_Combining_Put_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Combining_Put_fence( struct variable *result, struct variable par[] )
{ store_double( result, measure_Combining_Put_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Combining_Put_fence( struct variable *result, struct variable par[] )
{ init_Combining_Put_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Comm_dup( struct variable *result, struct variable par[] )
{ store_double( result, measure_Comm_dup(  )); }

static void call_init_Comm_dup( struct variable *result, struct variable par[] )
{ init_Comm_dup(  ); }

static void call_measure_Comm_split( struct variable *result, struct variable par[] )
{ store_double( result, measure_Comm_split(  )); }

static void call_init_Comm_split( struct variable *result, struct variable par[] )
{ init_Comm_split(  ); }

static void call_measure_Datatype_complex_Get( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_Get( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_Get( struct variable *result, struct variable par[] )
{ init_Datatype_complex_Get( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_complex_everytime( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_everytime( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_everytime( struct variable *result, struct variable par[] )
{ init_Datatype_complex_everytime( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_complex_mixed_everytime( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_mixed_everytime( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_mixed_everytime( struct variable *result, struct variable par[] )
{ init_Datatype_complex_mixed_everytime( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_complex_mixed_once( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_mixed_once( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_mixed_once( struct variable *result, struct variable par[] )
{ init_Datatype_complex_mixed_once( par[0].u.intv, par[1].u.infov ); }

static void call_finalize_Datatype_complex_mixed_once( struct variable *result, struct variable par[] )
{ finalize_Datatype_complex_mixed_once( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_complex_once( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_once( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_once( struct variable *result, struct variable par[] )
{ init_Datatype_complex_once( par[0].u.intv, par[1].u.infov ); }

static void call_finalize_Datatype_complex_once( struct variable *result, struct variable par[] )
{ finalize_Datatype_complex_once( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_int( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_int( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_int( struct variable *result, struct variable par[] )
{ init_Datatype_int( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_simple( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_simple( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_simple( struct variable *result, struct variable par[] )
{ init_Datatype_simple( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_simple_Get( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_simple_Get( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_simple_Get( struct variable *result, struct variable par[] )
{ init_Datatype_simple_Get( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Exchange( struct variable *result, struct variable par[] )
{ store_double( result, measure_Exchange( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Exchange( struct variable *result, struct variable par[] )
{ init_Exchange( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Gather( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather( struct variable *result, struct variable par[] )
{ init_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ init_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ finalize_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gather_using_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather_using_Send_Recv( struct variable *result, struct variable par[] )
{ init_Gather_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gatherv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gatherv( struct variable *result, struct variable par[] )
{ init_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Gatherv( struct variable *result, struct variable par[] )
{ finalize_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Get_Pingpong( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_Pingpong( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_Pingpong( struct variable *result, struct variable par[] )
{ init_Get_Pingpong( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Get_activewait( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_activewait( par[0].u.intv, par[1].u.infov )); }

static void call_init_Get_activewait( struct variable *result, struct variable par[] )
{ init_Get_activewait( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Get_activewait_sleep( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_activewait_sleep( par[0].u.intv, par[1].u.infov, par[2].u.intv )); }

static void call_init_Get_activewait_sleep( struct variable *result, struct variable par[] )
{ init_Get_activewait_sleep( par[0].u.intv, par[1].u.infov, par[2].u.intv ); }

static void call_measure_Get_callduration( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_callduration( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_callduration( struct variable *result, struct variable par[] )
{ init_Get_callduration( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Get_dedicated( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_dedicated( struct variable *result, struct variable par[] )
{ init_Get_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Get_fence( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_fence( struct variable *result, struct variable par[] )
{ init_Get_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Get_passive( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_passive( struct variable *result, struct variable par[] )
{ init_Get_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Isend_callduration( struct variable *result, struct variable par[] )
{ store_double( result, measure_Isend_callduration( par[0].u.intv, par[1].u.datatypev )); }

static void call_init_Isend_callduration( struct variable *result, struct variable par[] )
{ init_Isend_callduration( par[0].u.intv, par[1].u.datatypev ); }

static void call_measure_Par_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Par_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Par_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ init_Par_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Pingpong_Bsend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Bsend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Bsend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Bsend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Isend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Isend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Isend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Isend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Issend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Issend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Issend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Issend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Put_Put( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Put_Put( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Pingpong_Put_Put( struct variable *result, struct variable par[] )
{ init_Pingpong_Put_Put( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Iprobe_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Iprobe_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Iprobe_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Iprobe_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Irecv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Recv_AT( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Recv_AT( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Recv_AT( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Recv_AT( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Sendrecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Sendrecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.datatypev, par[5].u.intv, par[6].u.intv )); }

static void call_init_Pingpong_Sendrecv( struct variable *result, struct variable par[] )
{ init_Pingpong_Sendrecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.datatypev, par[5].u.intv, par[6].u.intv ); }

static void call_measure_Pingpong_Sendrecv_replace( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Sendrecv_replace( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv )); }

static void call_init_Pingpong_Sendrecv_replace( struct variable *result, struct variable par[] )
{ init_Pingpong_Sendrecv_replace( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Pingpong_Ssend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Ssend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Ssend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Ssend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Put( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put( struct variable *result, struct variable par[] )
{ init_Put( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_Exchange( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_Exchange( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Put_Exchange( struct variable *result, struct variable par[] )
{ init_Put_Exchange( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Put_Exchange_passive( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_Exchange_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Put_Exchange_passive( struct variable *result, struct variable par[] )
{ init_Put_Exchange_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Put_Shift( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_Shift( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Put_Shift( struct variable *result, struct variable par[] )
{ init_Put_Shift( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Put_activewait_get( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_activewait_get( par[0].u.intv, par[1].u.infov )); }

static void call_init_Put_activewait_get( struct variable *result, struct variable par[] )
{ init_Put_activewait_get( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Put_activewait_twosided( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_activewait_twosided( par[0].u.intv, par[1].u.infov )); }

static void call_init_Put_activewait_twosided( struct variable *result, struct variable par[] )
{ init_Put_activewait_twosided( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Put_activewait_twosided_sleep( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_activewait_twosided_sleep( par[0].u.intv, par[1].u.infov, par[2].u.intv )); }

static void call_init_Put_activewait_twosided_sleep( struct variable *result, struct variable par[] )
{ init_Put_activewait_twosided_sleep( par[0].u.intv, par[1].u.infov, par[2].u.intv ); }

static void call_measure_Put_dedicated( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put_dedicated( struct variable *result, struct variable par[] )
{ init_Put_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_fence( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put_fence( struct variable *result, struct variable par[] )
{ init_Put_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_fence_bidirectional( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_fence_bidirectional( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put_fence_bidirectional( struct variable *result, struct variable par[] )
{ init_Put_fence_bidirectional( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_passive( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put_passive( struct variable *result, struct variable par[] )
{ init_Put_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_passive_concurrent( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_passive_concurrent( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Put_passive_concurrent( struct variable *result, struct variable par[] )
{ init_Put_passive_concurrent( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Reduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce( struct variable *result, struct variable par[] )
{ init_Reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ init_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_finalize_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ finalize_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Reduce_scatter( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce_scatter( struct variable *result, struct variable par[] )
{ init_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_finalize_Reduce_scatter( struct variable *result, struct variable par[] )
{ finalize_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Scan( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scan( par[0].u.intv, par[1].u.datatypev, par[2].u.opv )); }

static void call_init_Scan( struct variable *result, struct variable par[] )
{ init_Scan( par[0].u.intv, par[1].u.datatypev, par[2].u.opv ); }

static void call_measure_Scatter( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Scatter( struct variable *result, struct variable par[] )
{ init_Scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Scatterv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Scatterv( struct variable *result, struct variable par[] )
{ init_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Scatterv( struct variable *result, struct variable par[] )
{ finalize_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Send( struct variable *result, struct variable par[] )
{ store_double( result, measure_Send( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Send( struct variable *result, struct variable par[] )
{ init_Send( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Win_complete( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_complete( struct variable *result, struct variable par[] )
{ init_Win_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_complete_delayed_wait( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_complete_delayed_wait( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_complete_delayed_wait( struct variable *result, struct variable par[] )
{ init_Win_complete_delayed_wait( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_complete_n( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_complete_n( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_complete_n( struct variable *result, struct variable par[] )
{ init_Win_complete_n( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_create( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_create( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Win_create( struct variable *result, struct variable par[] )
{ init_Win_create( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Win_fence_close( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_close( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_fence_close( struct variable *result, struct variable par[] )
{ init_Win_fence_close( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_fence_close_collective( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_close_collective( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_fence_close_collective( struct variable *result, struct variable par[] )
{ init_Win_fence_close_collective( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_fence_close_delayed( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_close_delayed( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_fence_close_delayed( struct variable *result, struct variable par[] )
{ init_Win_fence_close_delayed( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_fence_open( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_open( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_fence_open( struct variable *result, struct variable par[] )
{ init_Win_fence_open( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_fence_openclose( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_openclose( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_fence_openclose( struct variable *result, struct variable par[] )
{ init_Win_fence_openclose( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_free( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_free( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_free( struct variable *result, struct variable par[] )
{ init_Win_free( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_lock( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_lock( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_lock( struct variable *result, struct variable par[] )
{ init_Win_lock( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_lock_desync( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_lock_desync( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv, par[6].u.intv )); }

static void call_init_Win_lock_desync( struct variable *result, struct variable par[] )
{ init_Win_lock_desync( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv, par[6].u.intv ); }

static void call_measure_Win_post( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_post( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_post( struct variable *result, struct variable par[] )
{ init_Win_post( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_start( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_start( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_start( struct variable *result, struct variable par[] )
{ init_Win_start( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_start_delayed_post( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_start_delayed_post( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_start_delayed_post( struct variable *result, struct variable par[] )
{ init_Win_start_delayed_post( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_startcomplete( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_startcomplete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_startcomplete( struct variable *result, struct variable par[] )
{ init_Win_startcomplete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_test( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_test( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_test( struct variable *result, struct variable par[] )
{ init_Win_test( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_test_delayed( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_test_delayed( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_test_delayed( struct variable *result, struct variable par[] )
{ init_Win_test_delayed( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_unlock( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_unlock( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv, par[6].u.intv, par[7].u.intv )); }

static void call_init_Win_unlock( struct variable *result, struct variable par[] )
{ init_Win_unlock( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv, par[6].u.intv, par[7].u.intv ); }

static void call_measure_Win_wait_delayed_complete( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_wait_delayed_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_wait_delayed_complete( struct variable *result, struct variable par[] )
{ init_Win_wait_delayed_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_wait_early_complete( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_wait_early_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_wait_early_complete( struct variable *result, struct variable par[] )
{ init_Win_wait_early_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Wtime( struct variable *result, struct variable par[] )
{ store_double( result, measure_Wtime( par[0].u.intv )); }

static void call_func_alloc_datatype_array( struct variable *result, struct variable par[] )
{ store_darray( result, func_alloc_datatype_array( par[0].u.intv )); }

static void call_func_alloc_int_array( struct variable *result, struct variable par[] )
{ store_iarray( result, func_alloc_int_array( par[0].u.intv )); }

static void call_func_atoi( struct variable *result, struct variable par[] )
{ store_int( result, func_atoi( par[0].u.stringv )); }

static void call_func_cbrt( struct variable *result, struct variable par[] )
{ store_double( result, func_cbrt( par[0].u.doublev )); }

static void call_func_ceil( struct variable *result, struct variable par[] )
{ store_int( result, func_ceil( par[0].u.doublev )); }

static void call_func_choose_barrier_synchronization( struct variable *result, struct variable par[] )
{ func_choose_barrier_synchronization(  ); }

static void call_func_choose_no_synchronization( struct variable *result, struct variable par[] )
{ func_choose_no_synchronization(  ); }

static void call_func_choose_real_synchronization( struct variable *result, struct variable par[] )
{ func_choose_real_synchronization(  ); }

static void call_func_comm( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm( par[0].u.intv )); }

static void call_func_comm2( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm2( par[0].u.intv, par[1].u.intv )); }

static void call_func_comm2_max_latency_with_root( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm2_max_latency_with_root(  )); }

static void call_func_comm4( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm4( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.intv )); }

static void call_func_comm_first_half( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_first_half( par[0].u.commv )); }

static void call_func_comm_from_to( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_from_to( par[0].u.intv, par[1].u.intv )); }

static void call_func_comm_rand( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_rand( par[0].u.intv )); }

static void call_func_comm_second_half( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_second_half( par[0].u.commv )); }

static void call_func_copy_comm( struct variable *result, struct variable par[] )
{ store_comm( result, func_copy_comm( par[0].u.commv )); }

static void call_func_datatype_array_get( struct variable *result, struct variable par[] )
{ store_datatype( result, func_datatype_array_get( par[0].u.darrv, par[1].u.intv )); }

static void call_func_datatype_array_set( struct variable *result, struct variable par[] )
{ func_datatype_array_set( par[0].u.darrv, par[1].u.intv, par[2].u.datatypev ); }

static void call_measure_delayed_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_delayed_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.doublev, par[5].u.intv )); }

static void call_init_delayed_Alltoall( struct variable *result, struct variable par[] )
{ init_delayed_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.doublev, par[5].u.intv ); }

static void call_measure_delayed_Gather( struct variable *result, struct variable par[] )
{ store_double( result, measure_delayed_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev, par[6].u.intv )); }

static void call_init_delayed_Gather( struct variable *result, struct variable par[] )
{ init_delayed_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev, par[6].u.intv ); }

static void call_func_dims_create( struct variable *result, struct variable par[] )
{ store_iarray( result, func_dims_create( par[0].u.intv, par[1].u.iarrv )); }

static void call_func_floor( struct variable *result, struct variable par[] )
{ store_int( result, func_floor( par[0].u.doublev )); }

static void call_func_get_comm_rank( struct variable *result, struct variable par[] )
{ store_int( result, func_get_comm_rank( par[0].u.commv )); }

static void call_func_get_comm_size( struct variable *result, struct variable par[] )
{ store_int( result, func_get_comm_size( par[0].u.commv )); }

static void call_func_get_env( struct variable *result, struct variable par[] )
{ store_string( result, func_get_env( par[0].u.stringv )); }

static void call_func_get_mpi_alloc_mem_info( struct variable *result, struct variable par[] )
{ store_info( result, func_get_mpi_alloc_mem_info(  )); }

static void call_func_get_np( struct variable *result, struct variable par[] )
{ store_int( result, func_get_np(  )); }

static void call_func_get_random_int( struct variable *result, struct variable par[] )
{ store_int( result, func_get_random_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_info_create( struct variable *result, struct variable par[] )
{ store_info( result, func_info_create(  )); }

static void call_func_info_free( struct variable *result, struct variable par[] )
{ func_info_free( par[0].u.infov ); }

static void call_func_info_set( struct variable *result, struct variable par[] )
{ func_info_set( par[0].u.infov, par[1].u.stringv, par[2].u.stringv ); }

static void call_func_init_time_accounting( struct variable *result, struct variable par[] )
{ func_init_time_accounting(  ); }

static void call_func_int_array_get( struct variable *result, struct variable par[] )
{ store_int( result, func_int_array_get( par[0].u.iarrv, par[1].u.intv )); }

static void call_func_int_array_set( struct variable *result, struct variable par[] )
{ func_int_array_set( par[0].u.iarrv, par[1].u.intv, par[2].u.intv ); }

static void call_func_is_mpi_comm_null( struct variable *result, struct variable par[] )
{ store_int( result, func_is_mpi_comm_null( par[0].u.commv )); }

static void call_func_last_proc( struct variable *result, struct variable par[] )
{ store_int( result, func_last_proc(  )); }

static void call_func_log( struct variable *result, struct variable par[] )
{ store_double( result, func_log( par[0].u.doublev )); }

static void call_func_log10( struct variable *result, struct variable par[] )
{ store_double( result, func_log10( par[0].u.doublev )); }

static void call_func_log2( struct variable *result, struct variable par[] )
{ store_double( result, func_log2( par[0].u.doublev )); }

static void call_func_max_int( struct variable *result, struct variable par[] )
{ store_int( result, func_max_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_min_int( struct variable *result, struct variable par[] )
{ store_int( result, func_min_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_modulo( struct variable *result, struct variable par[] )
{ store_int( result, func_modulo( par[0].u.intv, par[1].u.intv )); }

static void call_func_mpi_processor_names( struct variable *result, struct variable par[] )
{ store_string( result, func_mpi_processor_names(  )); }

static void call_func_mpi_type_contiguous( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_contiguous( par[0].u.intv, par[1].u.datatypev )); }

static void call_func_mpi_type_hindexed( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_hindexed( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev )); }

static void call_func_mpi_type_hvector( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_hvector( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev )); }

static void call_func_mpi_type_indexed( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_indexed( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev )); }

static void call_func_mpi_type_struct( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_struct( par[0].u.iarrv, par[1].u.iarrv, par[2].u.darrv )); }

static void call_func_mpi_type_vector( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_vector( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev )); }

static void call_measure_multi_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_multi_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_multi_Alltoall( struct variable *result, struct variable par[] )
{ init_multi_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_myalltoallv( struct variable *result, struct variable par[] )
{ store_double( result, measure_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev )); }

static void call_init_myalltoallv( struct variable *result, struct variable par[] )
{ init_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev ); }

static void call_finalize_myalltoallv( struct variable *result, struct variable par[] )
{ finalize_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev ); }

static void call_measure_onesided_allreduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv )); }

static void call_init_onesided_allreduce( struct variable *result, struct variable par[] )
{ init_onesided_allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv ); }

static void call_measure_onesided_alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_onesided_alltoall( struct variable *result, struct variable par[] )
{ init_onesided_alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_onesided_bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_onesided_bcast( struct variable *result, struct variable par[] )
{ init_onesided_bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_onesided_borderexchange( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_borderexchange( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv )); }

static void call_init_onesided_borderexchange( struct variable *result, struct variable par[] )
{ init_onesided_borderexchange( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv ); }

static void call_finalize_onesided_borderexchange( struct variable *result, struct variable par[] )
{ finalize_onesided_borderexchange( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv ); }

static void call_measure_onesided_borderexchange_fence( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_borderexchange_fence( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv )); }

static void call_init_onesided_borderexchange_fence( struct variable *result, struct variable par[] )
{ init_onesided_borderexchange_fence( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv ); }

static void call_measure_onesided_dummy( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_dummy( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_onesided_dummy( struct variable *result, struct variable par[] )
{ init_onesided_dummy( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_onesided_reduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv )); }

static void call_init_onesided_reduce( struct variable *result, struct variable par[] )
{ init_onesided_reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv ); }

static void call_func_power( struct variable *result, struct variable par[] )
{ store_double( result, func_power( par[0].u.doublev, par[1].u.doublev )); }

static void call_func_print_time_accounting_info( struct variable *result, struct variable par[] )
{ func_print_time_accounting_info(  ); }

static void call_iterator_range( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_range( self, par[0].u.intv, par[1].u.intv )); }

static void call_func_round( struct variable *result, struct variable par[] )
{ store_int( result, func_round( par[0].u.doublev )); }

static void call_func_round_to_fivebytes( struct variable *result, struct variable par[] )
{ store_int( result, func_round_to_fivebytes( par[0].u.intv )); }

static void call_func_round_to_fourbytes( struct variable *result, struct variable par[] )
{ store_int( result, func_round_to_fourbytes( par[0].u.intv )); }

static void call_func_set_autorefine_factor( struct variable *result, struct variable par[] )
{ func_set_autorefine_factor( par[0].u.doublev ); }

static void call_func_set_cache_size( struct variable *result, struct variable par[] )
{ func_set_cache_size( par[0].u.intv ); }

static void call_func_set_mpi_alloc_mem_info( struct variable *result, struct variable par[] )
{ func_set_mpi_alloc_mem_info( par[0].u.infov ); }

static void call_func_set_recv_buffer_alignment( struct variable *result, struct variable par[] )
{ func_set_recv_buffer_alignment( par[0].u.intv ); }

static void call_func_set_send_buffer_alignment( struct variable *result, struct variable par[] )
{ func_set_send_buffer_alignment( par[0].u.intv ); }

static void call_func_set_skampi_buffer( struct variable *result, struct variable par[] )
{ func_set_skampi_buffer( par[0].u.intv ); }

static void call_func_set_skampi_buffer_mpi_alloc_mem( struct variable *result, struct variable par[] )
{ func_set_skampi_buffer_mpi_alloc_mem( par[0].u.intv, par[1].u.infov ); }

static void call_func_sqr( struct variable *result, struct variable par[] )
{ store_double( result, func_sqr( par[0].u.doublev )); }

static void call_func_sqrt( struct variable *result, struct variable par[] )
{ store_double( result, func_sqrt( par[0].u.doublev )); }

static void call_iterator_squares( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_squares( self, par[0].u.intv, par[1].u.intv )); }

static void call_func_switch_buffer_cycling_off( struct variable *result, struct variable par[] )
{ func_switch_buffer_cycling_off(  ); }

static void call_func_switch_buffer_cycling_on( struct variable *result, struct variable par[] )
{ func_switch_buffer_cycling_on(  ); }

static void call_func_topology_cartesian( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_cartesian( par[0].u.commv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.intv )); }

static void call_func_topology_graph( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_graph( par[0].u.commv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.intv )); }

static void call_func_topology_graph2( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_graph2( par[0].u.commv, par[1].u.iarrv, par[2].u.intv )); }

static void call_measure_twosided_borderexchange( struct variable *result, struct variable par[] )
{ store_double( result, measure_twosided_borderexchange( par[0].u.intv, par[1].u.intv, par[2].u.intv )); }

static void call_init_twosided_borderexchange( struct variable *result, struct variable par[] )
{ init_twosided_borderexchange( par[0].u.intv, par[1].u.intv, par[2].u.intv ); }

static void call_finalize_twosided_borderexchange( struct variable *result, struct variable par[] )
{ finalize_twosided_borderexchange( par[0].u.intv, par[1].u.intv, par[2].u.intv ); }

static void call_func_use_mpi_alloc_mem( struct variable *result, struct variable par[] )
{ func_use_mpi_alloc_mem( par[0].u.intv ); }

static void call_measure_waitpattern_down( struct variable *result, struct variable par[] )
{ store_double( result, measure_waitpattern_down( par[0].u.doublev )); }

static void call_init_waitpattern_down( struct variable *result, struct variable par[] )
{ init_waitpattern_down( par[0].u.doublev ); }

static void call_measure_waitpattern_up( struct variable *result, struct variable par[] )
{ store_double( result, measure_waitpattern_up( par[0].u.doublev )); }

static void call_init_waitpattern_up( struct variable *result, struct variable par[] )
{ init_waitpattern_up( par[0].u.doublev ); }

void init_functions(void)
{
  insert_function( &call_init_Accumulate, &call_measure_Accumulate, NULL, "Accumulate", "idho", TYPE_DOUBLE );
  insert_function( &call_init_Accumulate_activewait_twosided, &call_measure_Accumulate_activewait_twosided, NULL, "Accumulate_activewait_twosided", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Accumulate_concurrent, &call_measure_Accumulate_concurrent, NULL, "Accumulate_concurrent", "idhoi", TYPE_DOUBLE );
  insert_function( &call_init_Accumulate_concurrent_multi, &call_measure_Accumulate_concurrent_multi, NULL, "Accumulate_concurrent_multi", "idhioi", TYPE_DOUBLE );
  insert_function( &call_init_Allgather, &call_measure_Allgather, NULL, "Allgather", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Allgatherv, &call_measure_Allgatherv, &call_finalize_Allgatherv, "Allgatherv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Allreduce, &call_measure_Allreduce, NULL, "Allreduce", "ido", TYPE_DOUBLE );
  insert_function( &call_init_Allreduce_using_Reduce_Bcast, &call_measure_Allreduce_using_Reduce_Bcast, NULL, "Allreduce_using_Reduce_Bcast", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Alltoall, &call_measure_Alltoall, NULL, "Alltoall", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoall_using_Isend_Irecv, &call_measure_Alltoall_using_Isend_Irecv, &call_finalize_Alltoall_using_Isend_Irecv, "Alltoall_using_Isend_Irecv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoallv, &call_measure_Alltoallv, &call_finalize_Alltoallv, "Alltoallv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoallv_using_Isend_Irecv, &call_measure_Alltoallv_using_Isend_Irecv, &call_finalize_Alltoallv_using_Isend_Irecv, "Alltoallv_using_Isend_Irecv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Barrier, &call_measure_Barrier, NULL, "Barrier", "", TYPE_DOUBLE );
  insert_function( &call_init_Bcast, &call_measure_Bcast, NULL, "Bcast", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Bcast_using_Send_Recv, &call_measure_Bcast_using_Send_Recv, &call_finalize_Bcast_using_Send_Recv, "Bcast_using_Send_Recv", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Caching, &call_measure_Caching, NULL, "Caching", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Combining_Put_dedicated, &call_measure_Combining_Put_dedicated, NULL, "Combining_Put_dedicated", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Combining_Put_fence, &call_measure_Combining_Put_fence, NULL, "Combining_Put_fence", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Comm_dup, &call_measure_Comm_dup, NULL, "Comm_dup", "", TYPE_DOUBLE );
  insert_function( &call_init_Comm_split, &call_measure_Comm_split, NULL, "Comm_split", "", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_Get, &call_measure_Datatype_complex_Get, NULL, "Datatype_complex_Get", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_everytime, &call_measure_Datatype_complex_everytime, NULL, "Datatype_complex_everytime", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_mixed_everytime, &call_measure_Datatype_complex_mixed_everytime, NULL, "Datatype_complex_mixed_everytime", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_mixed_once, &call_measure_Datatype_complex_mixed_once, &call_finalize_Datatype_complex_mixed_once, "Datatype_complex_mixed_once", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_once, &call_measure_Datatype_complex_once, &call_finalize_Datatype_complex_once, "Datatype_complex_once", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_int, &call_measure_Datatype_int, NULL, "Datatype_int", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_simple, &call_measure_Datatype_simple, NULL, "Datatype_simple", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_simple_Get, &call_measure_Datatype_simple_Get, NULL, "Datatype_simple_Get", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Exchange, &call_measure_Exchange, NULL, "Exchange", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Gather, &call_measure_Gather, NULL, "Gather", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gather_using_Isend_Irecv_Waitall, &call_measure_Gather_using_Isend_Irecv_Waitall, &call_finalize_Gather_using_Isend_Irecv_Waitall, "Gather_using_Isend_Irecv_Waitall", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gather_using_Send_Recv, &call_measure_Gather_using_Send_Recv, NULL, "Gather_using_Send_Recv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gatherv, &call_measure_Gatherv, &call_finalize_Gatherv, "Gatherv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Get_Pingpong, &call_measure_Get_Pingpong, NULL, "Get_Pingpong", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Get_activewait, &call_measure_Get_activewait, NULL, "Get_activewait", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Get_activewait_sleep, &call_measure_Get_activewait_sleep, NULL, "Get_activewait_sleep", "ihi", TYPE_DOUBLE );
  insert_function( &call_init_Get_callduration, &call_measure_Get_callduration, NULL, "Get_callduration", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Get_dedicated, &call_measure_Get_dedicated, NULL, "Get_dedicated", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Get_fence, &call_measure_Get_fence, NULL, "Get_fence", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Get_passive, &call_measure_Get_passive, NULL, "Get_passive", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Isend_callduration, &call_measure_Isend_callduration, NULL, "Isend_callduration", "id", TYPE_DOUBLE );
  insert_function( &call_init_Par_Pingpong_Send_Recv, &call_measure_Par_Pingpong_Send_Recv, NULL, "Par_Pingpong_Send_Recv", "idiiii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Bsend_Recv, &call_measure_Pingpong_Bsend_Recv, NULL, "Pingpong_Bsend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Isend_Recv, &call_measure_Pingpong_Isend_Recv, NULL, "Pingpong_Isend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Issend_Recv, &call_measure_Pingpong_Issend_Recv, NULL, "Pingpong_Issend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Put_Put, &call_measure_Pingpong_Put_Put, NULL, "Pingpong_Put_Put", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Iprobe_Recv, &call_measure_Pingpong_Send_Iprobe_Recv, NULL, "Pingpong_Send_Iprobe_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Irecv, &call_measure_Pingpong_Send_Irecv, NULL, "Pingpong_Send_Irecv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Recv, &call_measure_Pingpong_Send_Recv, NULL, "Pingpong_Send_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Recv_AT, &call_measure_Pingpong_Send_Recv_AT, NULL, "Pingpong_Send_Recv_AT", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Sendrecv, &call_measure_Pingpong_Sendrecv, NULL, "Pingpong_Sendrecv", "idiidii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Sendrecv_replace, &call_measure_Pingpong_Sendrecv_replace, NULL, "Pingpong_Sendrecv_replace", "idiii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Ssend_Recv, &call_measure_Pingpong_Ssend_Recv, NULL, "Pingpong_Ssend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Put, &call_measure_Put, NULL, "Put", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_Exchange, &call_measure_Put_Exchange, NULL, "Put_Exchange", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Put_Exchange_passive, &call_measure_Put_Exchange_passive, NULL, "Put_Exchange_passive", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Put_Shift, &call_measure_Put_Shift, NULL, "Put_Shift", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Put_activewait_get, &call_measure_Put_activewait_get, NULL, "Put_activewait_get", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Put_activewait_twosided, &call_measure_Put_activewait_twosided, NULL, "Put_activewait_twosided", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Put_activewait_twosided_sleep, &call_measure_Put_activewait_twosided_sleep, NULL, "Put_activewait_twosided_sleep", "ihi", TYPE_DOUBLE );
  insert_function( &call_init_Put_dedicated, &call_measure_Put_dedicated, NULL, "Put_dedicated", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_fence, &call_measure_Put_fence, NULL, "Put_fence", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_fence_bidirectional, &call_measure_Put_fence_bidirectional, NULL, "Put_fence_bidirectional", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_passive, &call_measure_Put_passive, NULL, "Put_passive", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_passive_concurrent, &call_measure_Put_passive_concurrent, NULL, "Put_passive_concurrent", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce, &call_measure_Reduce, NULL, "Reduce", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce_Scatterv, &call_measure_Reduce_Scatterv, &call_finalize_Reduce_Scatterv, "Reduce_Scatterv", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce_scatter, &call_measure_Reduce_scatter, &call_finalize_Reduce_scatter, "Reduce_scatter", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Scan, &call_measure_Scan, NULL, "Scan", "ido", TYPE_DOUBLE );
  insert_function( &call_init_Scatter, &call_measure_Scatter, NULL, "Scatter", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Scatterv, &call_measure_Scatterv, &call_finalize_Scatterv, "Scatterv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Send, &call_measure_Send, NULL, "Send", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Win_complete, &call_measure_Win_complete, NULL, "Win_complete", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_complete_delayed_wait, &call_measure_Win_complete_delayed_wait, NULL, "Win_complete_delayed_wait", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_complete_n, &call_measure_Win_complete_n, NULL, "Win_complete_n", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_create, &call_measure_Win_create, NULL, "Win_create", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_close, &call_measure_Win_fence_close, NULL, "Win_fence_close", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_close_collective, &call_measure_Win_fence_close_collective, NULL, "Win_fence_close_collective", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_close_delayed, &call_measure_Win_fence_close_delayed, NULL, "Win_fence_close_delayed", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_open, &call_measure_Win_fence_open, NULL, "Win_fence_open", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_openclose, &call_measure_Win_fence_openclose, NULL, "Win_fence_openclose", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_free, &call_measure_Win_free, NULL, "Win_free", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_lock, &call_measure_Win_lock, NULL, "Win_lock", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_lock_desync, &call_measure_Win_lock_desync, NULL, "Win_lock_desync", "idhiiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_post, &call_measure_Win_post, NULL, "Win_post", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_start, &call_measure_Win_start, NULL, "Win_start", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_start_delayed_post, &call_measure_Win_start_delayed_post, NULL, "Win_start_delayed_post", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_startcomplete, &call_measure_Win_startcomplete, NULL, "Win_startcomplete", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_test, &call_measure_Win_test, NULL, "Win_test", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_test_delayed, &call_measure_Win_test_delayed, NULL, "Win_test_delayed", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_unlock, &call_measure_Win_unlock, NULL, "Win_unlock", "idhiiiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_wait_delayed_complete, &call_measure_Win_wait_delayed_complete, NULL, "Win_wait_delayed_complete", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_wait_early_complete, &call_measure_Win_wait_early_complete, NULL, "Win_wait_early_complete", "idhiii", TYPE_DOUBLE );
  insert_function( NULL, &call_measure_Wtime, NULL, "Wtime", "i", TYPE_DOUBLE );
  insert_function( NULL, &call_func_alloc_datatype_array, NULL, "alloc_datatype_array", "i", TYPE_DARRAY );
  insert_function( NULL, &call_func_alloc_int_array, NULL, "alloc_int_array", "i", TYPE_IARRAY );
  insert_function( NULL, &call_func_atoi, NULL, "atoi", "s", TYPE_INTEGER );
  insert_function( NULL, &call_func_cbrt, NULL, "cbrt", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_ceil, NULL, "ceil", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_choose_barrier_synchronization, NULL, "choose_barrier_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_choose_no_synchronization, NULL, "choose_no_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_choose_real_synchronization, NULL, "choose_real_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_comm, NULL, "comm", "i", TYPE_COMM );
  insert_function( NULL, &call_func_comm2, NULL, "comm2", "ii", TYPE_COMM );
  insert_function( NULL, &call_func_comm2_max_latency_with_root, NULL, "comm2_max_latency_with_root", "", TYPE_COMM );
  insert_function( NULL, &call_func_comm4, NULL, "comm4", "iiii", TYPE_COMM );
  insert_function( NULL, &call_func_comm_first_half, NULL, "comm_first_half", "c", TYPE_COMM );
  insert_function( NULL, &call_func_comm_from_to, NULL, "comm_from_to", "ii", TYPE_COMM );
  insert_function( NULL, &call_func_comm_rand, NULL, "comm_rand", "i", TYPE_COMM );
  insert_function( NULL, &call_func_comm_second_half, NULL, "comm_second_half", "c", TYPE_COMM );
  insert_function( NULL, &call_func_copy_comm, NULL, "copy_comm", "c", TYPE_COMM );
  insert_function( NULL, &call_func_datatype_array_get, NULL, "datatype_array_get", "ei", TYPE_DATATYPE );
  insert_function( NULL, &call_func_datatype_array_set, NULL, "datatype_array_set", "eid", TYPE_VOID );
  insert_function( &call_init_delayed_Alltoall, &call_measure_delayed_Alltoall, NULL, "delayed_Alltoall", "ididfi", TYPE_DOUBLE );
  insert_function( &call_init_delayed_Gather, &call_measure_delayed_Gather, NULL, "delayed_Gather", "ididifi", TYPE_DOUBLE );
  insert_function( NULL, &call_func_dims_create, NULL, "dims_create", "ia", TYPE_IARRAY );
  insert_function( NULL, &call_func_floor, NULL, "floor", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_comm_rank, NULL, "get_comm_rank", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_comm_size, NULL, "get_comm_size", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_env, NULL, "get_env", "s", TYPE_STRING );
  insert_function( NULL, &call_func_get_mpi_alloc_mem_info, NULL, "get_mpi_alloc_mem_info", "", TYPE_INFO );
  insert_function( NULL, &call_func_get_np, NULL, "get_np", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_random_int, NULL, "get_random_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_info_create, NULL, "info_create", "", TYPE_INFO );
  insert_function( NULL, &call_func_info_free, NULL, "info_free", "h", TYPE_VOID );
  insert_function( NULL, &call_func_info_set, NULL, "info_set", "hss", TYPE_VOID );
  insert_function( NULL, &call_func_init_time_accounting, NULL, "init_time_accounting", "", TYPE_VOID );
  insert_function( NULL, &call_func_int_array_get, NULL, "int_array_get", "ai", TYPE_INTEGER );
  insert_function( NULL, &call_func_int_array_set, NULL, "int_array_set", "aii", TYPE_VOID );
  insert_function( NULL, &call_func_is_mpi_comm_null, NULL, "is_mpi_comm_null", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_last_proc, NULL, "last_proc", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_log, NULL, "log", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_log10, NULL, "log10", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_log2, NULL, "log2", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_max_int, NULL, "max_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_min_int, NULL, "min_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_modulo, NULL, "modulo", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_mpi_processor_names, NULL, "mpi_processor_names", "", TYPE_STRING );
  insert_function( NULL, &call_func_mpi_type_contiguous, NULL, "mpi_type_contiguous", "id", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_hindexed, NULL, "mpi_type_hindexed", "aad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_hvector, NULL, "mpi_type_hvector", "iiid", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_indexed, NULL, "mpi_type_indexed", "aad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_struct, NULL, "mpi_type_struct", "aae", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_vector, NULL, "mpi_type_vector", "iiid", TYPE_DATATYPE );
  insert_function( &call_init_multi_Alltoall, &call_measure_multi_Alltoall, NULL, "multi_Alltoall", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_myalltoallv, &call_measure_myalltoallv, &call_finalize_myalltoallv, "myalltoallv", "ididif", TYPE_DOUBLE );
  insert_function( &call_init_onesided_allreduce, &call_measure_onesided_allreduce, NULL, "onesided_allreduce", "idho", TYPE_DOUBLE );
  insert_function( &call_init_onesided_alltoall, &call_measure_onesided_alltoall, NULL, "onesided_alltoall", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_onesided_bcast, &call_measure_onesided_bcast, NULL, "onesided_bcast", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_onesided_borderexchange, &call_measure_onesided_borderexchange, &call_finalize_onesided_borderexchange, "onesided_borderexchange", "ihii", TYPE_DOUBLE );
  insert_function( &call_init_onesided_borderexchange_fence, &call_measure_onesided_borderexchange_fence, NULL, "onesided_borderexchange_fence", "ihii", TYPE_DOUBLE );
  insert_function( &call_init_onesided_dummy, &call_measure_onesided_dummy, NULL, "onesided_dummy", "idh", TYPE_DOUBLE );
  insert_function( &call_init_onesided_reduce, &call_measure_onesided_reduce, NULL, "onesided_reduce", "idho", TYPE_DOUBLE );
  insert_function( NULL, &call_func_power, NULL, "power", "ff", TYPE_DOUBLE );
  insert_function( NULL, &call_func_print_time_accounting_info, NULL, "print_time_accounting_info", "", TYPE_VOID );
  insert_iterator( &call_iterator_range, "range", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_round, NULL, "round", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_round_to_fivebytes, NULL, "round_to_fivebytes", "i", TYPE_INTEGER );
  insert_function( NULL, &call_func_round_to_fourbytes, NULL, "round_to_fourbytes", "i", TYPE_INTEGER );
  insert_function( NULL, &call_func_set_autorefine_factor, NULL, "set_autorefine_factor", "f", TYPE_VOID );
  insert_function( NULL, &call_func_set_cache_size, NULL, "set_cache_size", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_mpi_alloc_mem_info, NULL, "set_mpi_alloc_mem_info", "h", TYPE_VOID );
  insert_function( NULL, &call_func_set_recv_buffer_alignment, NULL, "set_recv_buffer_alignment", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_send_buffer_alignment, NULL, "set_send_buffer_alignment", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_skampi_buffer, NULL, "set_skampi_buffer", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_skampi_buffer_mpi_alloc_mem, NULL, "set_skampi_buffer_mpi_alloc_mem", "ih", TYPE_VOID );
  insert_function( NULL, &call_func_sqr, NULL, "sqr", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_sqrt, NULL, "sqrt", "f", TYPE_DOUBLE );
  insert_iterator( &call_iterator_squares, "squares", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_switch_buffer_cycling_off, NULL, "switch_buffer_cycling_off", "", TYPE_VOID );
  insert_function( NULL, &call_func_switch_buffer_cycling_on, NULL, "switch_buffer_cycling_on", "", TYPE_VOID );
  insert_function( NULL, &call_func_topology_cartesian, NULL, "topology_cartesian", "caai", TYPE_COMM );
  insert_function( NULL, &call_func_topology_graph, NULL, "topology_graph", "caai", TYPE_COMM );
  insert_function( NULL, &call_func_topology_graph2, NULL, "topology_graph2", "cai", TYPE_COMM );
  insert_function( &call_init_twosided_borderexchange, &call_measure_twosided_borderexchange, &call_finalize_twosided_borderexchange, "twosided_borderexchange", "iii", TYPE_DOUBLE );
  insert_function( NULL, &call_func_use_mpi_alloc_mem, NULL, "use_mpi_alloc_mem", "i", TYPE_VOID );
  insert_function( &call_init_waitpattern_down, &call_measure_waitpattern_down, NULL, "waitpattern_down", "f", TYPE_DOUBLE );
  insert_function( &call_init_waitpattern_up, &call_measure_waitpattern_up, NULL, "waitpattern_up", "f", TYPE_DOUBLE );
}

#endif


#if defined(USE_ONESIDED) && defined(USE_MPI_IO)

extern double measure_Accumulate( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation );
extern void init_Accumulate( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation );
extern double measure_Accumulate_activewait_twosided( int count, MPI_Info info );
extern void init_Accumulate_activewait_twosided( int count, MPI_Info info );
extern double measure_Accumulate_concurrent( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation, int disjoint );
extern void init_Accumulate_concurrent( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op operation, int disjoint );
extern double measure_Accumulate_concurrent_multi( int count, MPI_Datatype datatype, MPI_Info info, int msg_count, MPI_Op operator, int disjoint );
extern void init_Accumulate_concurrent_multi( int count, MPI_Datatype datatype, MPI_Info info, int msg_count, MPI_Op operator, int disjoint );
extern double measure_Allgather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Allgather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Allgatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Allgatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Allgatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Allreduce( int count, MPI_Datatype dt, MPI_Op op );
extern void init_Allreduce( int count, MPI_Datatype dt, MPI_Op op );
extern double measure_Allreduce_using_Reduce_Bcast( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Allreduce_using_Reduce_Bcast( int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoall_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoall_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoall_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Alltoallv_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void init_Alltoallv_using_Isend_Irecv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern void finalize_Alltoallv_using_Isend_Irecv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt );
extern double measure_Barrier( void );
extern void init_Barrier( void );
extern double measure_Bcast( int count, MPI_Datatype dt, int root );
extern void init_Bcast( int count, MPI_Datatype dt, int root );
extern double measure_Bcast_using_Send_Recv( int count, MPI_Datatype dt, int root );
extern void init_Bcast_using_Send_Recv( int count, MPI_Datatype dt, int root );
extern void finalize_Bcast_using_Send_Recv(int count, MPI_Datatype dt, int root );
extern double measure_Caching( int count, MPI_Datatype datatype, MPI_Info info, int delay_us );
extern void init_Caching( int count, MPI_Datatype datatype, MPI_Info info, int delay_us );
extern double measure_Combining_Put_dedicated( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern void init_Combining_Put_dedicated( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern double measure_Combining_Put_fence( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern void init_Combining_Put_fence( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern double measure_Comm_dup( void );
extern void init_Comm_dup( void );
extern double measure_Comm_split( void );
extern void init_Comm_split( void );
extern double measure_Datatype_complex_Get( int count, MPI_Info info );
extern void init_Datatype_complex_Get( int count, MPI_Info info );
extern double measure_Datatype_complex_everytime( int count, MPI_Info info );
extern void init_Datatype_complex_everytime( int count, MPI_Info info );
extern double measure_Datatype_complex_mixed_everytime( int count, MPI_Info info );
extern void init_Datatype_complex_mixed_everytime( int count, MPI_Info info );
extern double measure_Datatype_complex_mixed_once( int count, MPI_Info info );
extern void init_Datatype_complex_mixed_once( int count, MPI_Info info );
extern void finalize_Datatype_complex_mixed_once(int count, MPI_Info info );
extern double measure_Datatype_complex_once( int count, MPI_Info info );
extern void init_Datatype_complex_once( int count, MPI_Info info );
extern void finalize_Datatype_complex_once(int count, MPI_Info info );
extern double measure_Datatype_int( int count, MPI_Info info );
extern void init_Datatype_int( int count, MPI_Info info );
extern double measure_Datatype_simple( int count, MPI_Info info );
extern void init_Datatype_simple( int count, MPI_Info info );
extern double measure_Datatype_simple_Get( int count, MPI_Info info );
extern void init_Datatype_simple_Get( int count, MPI_Info info );
extern double measure_Exchange( int count, MPI_Datatype datatype, int msg_count );
extern void init_Exchange( int count, MPI_Datatype datatype, int msg_count );
extern double measure_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gather_using_Isend_Irecv_Waitall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather_using_Isend_Irecv_Waitall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Gather_using_Isend_Irecv_Waitall(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gather_using_Send_Recv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gather_using_Send_Recv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Gatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Gatherv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Gatherv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Get_Pingpong( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_Pingpong( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Get_activewait( int count, MPI_Info info );
extern void init_Get_activewait( int count, MPI_Info info );
extern double measure_Get_activewait_sleep( int count, MPI_Info info, int delay_us );
extern void init_Get_activewait_sleep( int count, MPI_Info info, int delay_us );
extern double measure_Get_callduration( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_callduration( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Get_dedicated( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_dedicated( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Get_fence( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_fence( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Get_passive( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Get_passive( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Isend_callduration( int count, MPI_Datatype datatype );
extern void init_Isend_callduration( int count, MPI_Datatype datatype );
extern double measure_MPI_IO_close( int count );
extern void init_MPI_IO_close( int count );
extern void finalize_MPI_IO_close(int count );
extern double measure_MPI_IO_delete( int count );
extern void init_MPI_IO_delete( int count );
extern void finalize_MPI_IO_delete(int count );
extern double measure_MPI_IO_delete_on_close( int count );
extern void init_MPI_IO_delete_on_close( int count );
extern void finalize_MPI_IO_delete_on_close(int count );
extern double measure_MPI_IO_file_seek( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern void init_MPI_IO_file_seek( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern void finalize_MPI_IO_file_seek(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern double measure_MPI_IO_file_seek_shared( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern void init_MPI_IO_file_seek_shared( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern void finalize_MPI_IO_file_seek_shared(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype );
extern double measure_MPI_IO_open( int create_file );
extern void init_MPI_IO_open( int create_file );
extern void finalize_MPI_IO_open(int create_file );
extern double measure_MPI_IO_open_close( int create_flag );
extern void init_MPI_IO_open_close( int create_flag );
extern void finalize_MPI_IO_open_close(int create_flag );
extern double measure_MPI_IO_preallocate( int count, int with_sync );
extern void init_MPI_IO_preallocate( int count, int with_sync );
extern void finalize_MPI_IO_preallocate(int count, int with_sync );
extern double measure_MPI_IO_read_and_distribute( int count, int np, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_and_distribute( int count, int np, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_and_distribute(int count, int np, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_file( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_file( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_file(int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_noncontiguous( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_noncontiguous( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_noncontiguous(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_once( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern void init_MPI_IO_read_once( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern void finalize_MPI_IO_read_once(int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern double measure_MPI_IO_read_random( IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_random( IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_random(IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_segmented( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_segmented( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_segmented(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_separate( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_separate( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_separate(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_sequential( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_sequential( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_sequential(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_read_strided( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_read_strided( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_read_strided(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_and_read( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_and_read( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_and_read(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_noncontiguous( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_noncontiguous( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_noncontiguous(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_once( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern void init_MPI_IO_write_once( int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern void finalize_MPI_IO_write_once(int count, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int private, int daccess );
extern double measure_MPI_IO_write_random( IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_random( IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_random(IArray memsize, IArray blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_segmented( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_segmented( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_segmented(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_separate( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_separate( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_separate(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_sequential( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_sequential( int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_sequential(int memsize, int blocklen, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_MPI_IO_write_strided( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void init_MPI_IO_write_strided( int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern void finalize_MPI_IO_write_strided(int memsize, int blocklen, int stride, MPI_Datatype memtype, MPI_Datatype etype, MPI_Datatype filetype, int daccess );
extern double measure_Par_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize );
extern void init_Par_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations, int procs, int nodesize );
extern double measure_Pingpong_Bsend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Bsend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Isend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Isend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Issend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Issend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Put_Put( int count, MPI_Datatype datatype, MPI_Info info, int iterations );
extern void init_Pingpong_Put_Put( int count, MPI_Datatype datatype, MPI_Info info, int iterations );
extern double measure_Pingpong_Send_Iprobe_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Iprobe_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Irecv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Irecv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Send_Recv_AT( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Send_Recv_AT( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Pingpong_Sendrecv( int send_count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations );
extern void init_Pingpong_Sendrecv( int count, MPI_Datatype send_dt, int send_tag, int recv_count, MPI_Datatype recv_dt, int recv_tag, int iterations );
extern double measure_Pingpong_Sendrecv_replace( int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations );
extern void init_Pingpong_Sendrecv_replace( int count, MPI_Datatype datatype, int send_tag, int recv_tag, int iterations );
extern double measure_Pingpong_Ssend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern void init_Pingpong_Ssend_Recv( int count, MPI_Datatype datatype, int tag, int iterations );
extern double measure_Put( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_Exchange( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern void init_Put_Exchange( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern double measure_Put_Exchange_passive( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern void init_Put_Exchange_passive( int count, MPI_Datatype datatype, MPI_Info info, int msg_count );
extern double measure_Put_Shift( int count, MPI_Datatype datatype, MPI_Info info, int distance );
extern void init_Put_Shift( int count, MPI_Datatype datatype, MPI_Info info, int distance );
extern double measure_Put_activewait_get( int count, MPI_Info info );
extern void init_Put_activewait_get( int count, MPI_Info info );
extern double measure_Put_activewait_twosided( int count, MPI_Info info );
extern void init_Put_activewait_twosided( int count, MPI_Info info );
extern double measure_Put_activewait_twosided_sleep( int count, MPI_Info info, int delay_us );
extern void init_Put_activewait_twosided_sleep( int count, MPI_Info info, int delay_us );
extern double measure_Put_dedicated( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put_dedicated( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_fence( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put_fence( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_fence_bidirectional( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put_fence_bidirectional( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_passive( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Put_passive( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Put_passive_concurrent( int count, MPI_Datatype datatype, MPI_Info info, int lock_type );
extern void init_Put_passive_concurrent( int count, MPI_Datatype datatype, MPI_Info info, int lock_type );
extern double measure_Reduce( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce( int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Reduce_Scatterv( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce_Scatterv( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void finalize_Reduce_Scatterv(int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Reduce_scatter( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void init_Reduce_scatter( int count, MPI_Datatype dt, MPI_Op op, int root );
extern void finalize_Reduce_scatter(int count, MPI_Datatype dt, MPI_Op op, int root );
extern double measure_Scan( int count, MPI_Datatype dt, MPI_Op op );
extern void init_Scan( int count, MPI_Datatype dt, MPI_Op op );
extern double measure_Scatter( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Scatter( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Scatterv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void init_Scatterv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern void finalize_Scatterv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root );
extern double measure_Send( int count, MPI_Datatype datatype, int tag );
extern void init_Send( int count, MPI_Datatype datatype, int tag );
extern double measure_Win_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_complete_delayed_wait( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern void init_Win_complete_delayed_wait( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern double measure_Win_complete_n( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int msg_count, int delay_us );
extern void init_Win_complete_n( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int msg_count, int delay_us );
extern double measure_Win_create( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_Win_create( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_Win_fence_close( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_fence_close( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_fence_close_collective( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_fence_close_collective( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_fence_close_delayed( int count, MPI_Datatype datatype, MPI_Info info, int msg_count, int delay_us );
extern void init_Win_fence_close_delayed( int count, MPI_Datatype datatype, MPI_Info info, int msg_count, int delay_us );
extern double measure_Win_fence_open( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern void init_Win_fence_open( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern double measure_Win_fence_openclose( int count, MPI_Datatype datatype, MPI_Info info, int do_a_put );
extern void init_Win_fence_openclose( int count, MPI_Datatype datatype, MPI_Info info, int do_a_put );
extern double measure_Win_free( int count, MPI_Datatype datatype, MPI_Info info, int do_a_put );
extern void init_Win_free( int count, MPI_Datatype datatype, MPI_Info info, int do_a_put );
extern double measure_Win_lock( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank );
extern void init_Win_lock( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank );
extern double measure_Win_lock_desync( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int delay_us );
extern void init_Win_lock_desync( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int delay_us );
extern double measure_Win_post( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern void init_Win_post( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern double measure_Win_start( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern void init_Win_start( int count, MPI_Datatype datatype, MPI_Info info, int assertion );
extern double measure_Win_start_delayed_post( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int delay_us, int delay_rank );
extern void init_Win_start_delayed_post( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int delay_us, int delay_rank );
extern double measure_Win_startcomplete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_startcomplete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_test( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern void init_Win_test( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put );
extern double measure_Win_test_delayed( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern void init_Win_test_delayed( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern double measure_Win_unlock( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int do_a_put, int delay_us );
extern void init_Win_unlock( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int lock_type, int dest_rank, int do_a_put, int delay_us );
extern double measure_Win_wait_delayed_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern void init_Win_wait_delayed_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern double measure_Win_wait_early_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern void init_Win_wait_early_complete( int count, MPI_Datatype datatype, MPI_Info info, int assertion, int do_a_put, int delay_us );
extern double measure_Wtime( int iterations );
extern DArray func_alloc_datatype_array( int size );
extern IArray func_alloc_int_array( int size );
extern int func_atoi( char* s );
extern double func_cbrt( double x );
extern int func_ceil( double x );
extern void func_choose_barrier_synchronization( void );
extern void func_choose_no_synchronization( void );
extern void func_choose_real_synchronization( void );
extern MPI_Comm func_comm( int i );
extern MPI_Comm func_comm2( int a, int b );
extern MPI_Comm func_comm2_max_latency_with_root( void );
extern MPI_Comm func_comm4( int a, int b, int c, int d );
extern MPI_Comm func_comm_first_half( MPI_Comm c );
extern MPI_Comm func_comm_from_to( int first, int last );
extern MPI_Comm func_comm_rand( int n );
extern MPI_Comm func_comm_second_half( MPI_Comm c );
extern MPI_Comm func_copy_comm( MPI_Comm c );
extern int iterator_data_access_routines( void** self, int pattern );
extern MPI_Datatype func_datatype_array_get( DArray darr, int i );
extern void func_datatype_array_set( DArray darr, int i, MPI_Datatype v );
extern double measure_delayed_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node );
extern void init_delayed_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, double delay, int node );
extern double measure_delayed_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node );
extern void init_delayed_Gather( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int root, double delay, int node );
extern IArray func_dims_create( int nnodes, IArray dims );
extern int func_floor( double x );
extern int func_get_comm_rank( MPI_Comm comm );
extern int func_get_comm_size( MPI_Comm comm );
extern char* func_get_env( char*name );
extern int func_get_io_atomicity( void );
extern char* func_get_io_datarep( void );
extern int func_get_io_file_sync( void );
extern int func_get_io_preallocation( void );
extern int func_get_io_unique_open( void );
extern char* func_get_io_working_dir( void );
extern MPI_Info func_get_mpi_alloc_mem_info( void );
extern int func_get_np( void );
extern int func_get_random_int( int min_value, int max_value );
extern MPI_Info func_info_create( void );
extern void func_info_free( MPI_Info info );
extern void func_info_set( MPI_Info info, char* key, char* value );
extern void func_init_time_accounting( void );
extern IArray func_int_array_allgather( MPI_Comm c, int v );
extern int func_int_array_get( IArray iarr, int i );
extern void func_int_array_set( IArray iarr, int i, int v );
extern int func_is_mpi_comm_null( MPI_Comm comm );
extern int func_last_proc( void );
extern double func_log( double a );
extern double func_log10( double a );
extern double func_log2( double a );
extern int func_max_int( int a, int b );
extern int func_min_int( int a, int b );
extern int func_modulo( int a, int b );
extern char* func_mpi_processor_names( void );
extern MPI_Datatype func_mpi_type_contiguous( int count, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_create_darray( int size, int rank, IArray gsizes, IArray distribs, IArray dargs, IArray psizes, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_create_subarray( IArray sizes, IArray subsizes, IArray starts, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_hindexed( IArray blocklengths, IArray displacements, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_hvector( int count, int length, int stride, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_indexed( IArray blocklengths, IArray displacements, MPI_Datatype oldtype );
extern MPI_Datatype func_mpi_type_struct( IArray blocklengths, IArray displacements, DArray dtypes );
extern MPI_Datatype func_mpi_type_vector( int count, int length, int stride, MPI_Datatype oldtype );
extern double measure_multi_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int alltoall_count );
extern void init_multi_Alltoall( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int alltoall_count );
extern double measure_myalltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern void init_myalltoallv( int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern void finalize_myalltoallv(int send_count, MPI_Datatype send_dt, int recv_count, MPI_Datatype recv_dt, int charged_rank, double ratio );
extern double measure_onesided_allreduce( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op );
extern void init_onesided_allreduce( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op );
extern double measure_onesided_alltoall( int count, MPI_Datatype datatype, MPI_Info info, int alltoall_count );
extern void init_onesided_alltoall( int count, MPI_Datatype datatype, MPI_Info info, int alltoall_count );
extern double measure_onesided_bcast( int count, MPI_Datatype datatype, MPI_Info info, int bcast_count );
extern void init_onesided_bcast( int count, MPI_Datatype datatype, MPI_Info info, int bcast_count );
extern double measure_onesided_borderexchange( int count, MPI_Info info, int iteration_count, int compute );
extern void init_onesided_borderexchange( int elemcount, MPI_Info info, int iteration_count, int compute );
extern void finalize_onesided_borderexchange(int elemcount, MPI_Info info, int iteration_count, int compute );
extern double measure_onesided_borderexchange_fence( int count, MPI_Info info, int iteration_count, int compute );
extern void init_onesided_borderexchange_fence( int elemcount, MPI_Info info, int iteration_count, int compute );
extern double measure_onesided_dummy( int count, MPI_Datatype datatype, MPI_Info info );
extern void init_onesided_dummy( int count, MPI_Datatype datatype, MPI_Info info );
extern double measure_onesided_reduce( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op );
extern void init_onesided_reduce( int count, MPI_Datatype datatype, MPI_Info info, MPI_Op op );
extern double func_power( double a, double b );
extern void func_print_data_access_settings( int daccess, int flag );
extern void func_print_file_info( void );
extern void func_print_time_accounting_info( void );
extern int iterator_range( void** self, int start, int end );
extern void func_reset_file_info( void );
extern int func_round( double x );
extern int func_round_to_fivebytes( int size );
extern int func_round_to_fourbytes( int size );
extern void func_set_autorefine_factor( double f );
extern void func_set_cache_size( int size );
extern void func_set_file_info( char*key, char*value );
extern void func_set_io_atomicity( int flag );
extern void func_set_io_datarep( char*datarep );
extern void func_set_io_file_sync( int flag );
extern void func_set_io_preallocation( int flag );
extern void func_set_io_unique_open( int flag );
extern void func_set_io_working_dir( char*cwd );
extern void func_set_mpi_alloc_mem_info( MPI_Info info );
extern void func_set_recv_buffer_alignment( int a );
extern void func_set_send_buffer_alignment( int a );
extern void func_set_skampi_buffer( int size );
extern void func_set_skampi_buffer_mpi_alloc_mem( int size, MPI_Info info );
extern double func_sqr( double x );
extern double func_sqrt( double x );
extern int iterator_squares( void** self, int start, int end );
extern void func_switch_buffer_cycling_off( void );
extern void func_switch_buffer_cycling_on( void );
extern MPI_Comm func_topology_cartesian( MPI_Comm c, IArray dims, IArray periodic, int reorder );
extern MPI_Comm func_topology_graph( MPI_Comm c, IArray index, IArray edges, int reorder );
extern MPI_Comm func_topology_graph2( MPI_Comm c, IArray graph, int reorder );
extern double measure_twosided_borderexchange( int count, int iteration_count, int compute );
extern void init_twosided_borderexchange( int elemcount, int iteration_count, int compute );
extern void finalize_twosided_borderexchange(int count, int iteration_count, int compute );
extern void func_use_mpi_alloc_mem( int flag );
extern double measure_waitpattern_down( double scale );
extern void init_waitpattern_down( double scale );
extern double measure_waitpattern_up( double scale );
extern void init_waitpattern_up( double scale );

static void call_measure_Accumulate( struct variable *result, struct variable par[] )
{ store_double( result, measure_Accumulate( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv )); }

static void call_init_Accumulate( struct variable *result, struct variable par[] )
{ init_Accumulate( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv ); }

static void call_measure_Accumulate_activewait_twosided( struct variable *result, struct variable par[] )
{ store_double( result, measure_Accumulate_activewait_twosided( par[0].u.intv, par[1].u.infov )); }

static void call_init_Accumulate_activewait_twosided( struct variable *result, struct variable par[] )
{ init_Accumulate_activewait_twosided( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Accumulate_concurrent( struct variable *result, struct variable par[] )
{ store_double( result, measure_Accumulate_concurrent( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv, par[4].u.intv )); }

static void call_init_Accumulate_concurrent( struct variable *result, struct variable par[] )
{ init_Accumulate_concurrent( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv, par[4].u.intv ); }

static void call_measure_Accumulate_concurrent_multi( struct variable *result, struct variable par[] )
{ store_double( result, measure_Accumulate_concurrent_multi( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.opv, par[5].u.intv )); }

static void call_init_Accumulate_concurrent_multi( struct variable *result, struct variable par[] )
{ init_Accumulate_concurrent_multi( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.opv, par[5].u.intv ); }

static void call_measure_Allgather( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allgather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Allgather( struct variable *result, struct variable par[] )
{ init_Allgather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Allgatherv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Allgatherv( struct variable *result, struct variable par[] )
{ init_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Allgatherv( struct variable *result, struct variable par[] )
{ finalize_Allgatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Allreduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv )); }

static void call_init_Allreduce( struct variable *result, struct variable par[] )
{ init_Allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv ); }

static void call_measure_Allreduce_using_Reduce_Bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_Allreduce_using_Reduce_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Allreduce_using_Reduce_Bcast( struct variable *result, struct variable par[] )
{ init_Allreduce_using_Reduce_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoall( struct variable *result, struct variable par[] )
{ init_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ init_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoall_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ finalize_Alltoall_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoallv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoallv( struct variable *result, struct variable par[] )
{ init_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoallv( struct variable *result, struct variable par[] )
{ finalize_Alltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev )); }

static void call_init_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ init_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_finalize_Alltoallv_using_Isend_Irecv( struct variable *result, struct variable par[] )
{ finalize_Alltoallv_using_Isend_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev ); }

static void call_measure_Barrier( struct variable *result, struct variable par[] )
{ store_double( result, measure_Barrier(  )); }

static void call_init_Barrier( struct variable *result, struct variable par[] )
{ init_Barrier(  ); }

static void call_measure_Bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Bcast( struct variable *result, struct variable par[] )
{ init_Bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ init_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_finalize_Bcast_using_Send_Recv( struct variable *result, struct variable par[] )
{ finalize_Bcast_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Caching( struct variable *result, struct variable par[] )
{ store_double( result, measure_Caching( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Caching( struct variable *result, struct variable par[] )
{ init_Caching( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Combining_Put_dedicated( struct variable *result, struct variable par[] )
{ store_double( result, measure_Combining_Put_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Combining_Put_dedicated( struct variable *result, struct variable par[] )
{ init_Combining_Put_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Combining_Put_fence( struct variable *result, struct variable par[] )
{ store_double( result, measure_Combining_Put_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Combining_Put_fence( struct variable *result, struct variable par[] )
{ init_Combining_Put_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Comm_dup( struct variable *result, struct variable par[] )
{ store_double( result, measure_Comm_dup(  )); }

static void call_init_Comm_dup( struct variable *result, struct variable par[] )
{ init_Comm_dup(  ); }

static void call_measure_Comm_split( struct variable *result, struct variable par[] )
{ store_double( result, measure_Comm_split(  )); }

static void call_init_Comm_split( struct variable *result, struct variable par[] )
{ init_Comm_split(  ); }

static void call_measure_Datatype_complex_Get( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_Get( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_Get( struct variable *result, struct variable par[] )
{ init_Datatype_complex_Get( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_complex_everytime( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_everytime( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_everytime( struct variable *result, struct variable par[] )
{ init_Datatype_complex_everytime( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_complex_mixed_everytime( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_mixed_everytime( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_mixed_everytime( struct variable *result, struct variable par[] )
{ init_Datatype_complex_mixed_everytime( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_complex_mixed_once( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_mixed_once( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_mixed_once( struct variable *result, struct variable par[] )
{ init_Datatype_complex_mixed_once( par[0].u.intv, par[1].u.infov ); }

static void call_finalize_Datatype_complex_mixed_once( struct variable *result, struct variable par[] )
{ finalize_Datatype_complex_mixed_once( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_complex_once( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_complex_once( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_complex_once( struct variable *result, struct variable par[] )
{ init_Datatype_complex_once( par[0].u.intv, par[1].u.infov ); }

static void call_finalize_Datatype_complex_once( struct variable *result, struct variable par[] )
{ finalize_Datatype_complex_once( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_int( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_int( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_int( struct variable *result, struct variable par[] )
{ init_Datatype_int( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_simple( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_simple( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_simple( struct variable *result, struct variable par[] )
{ init_Datatype_simple( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Datatype_simple_Get( struct variable *result, struct variable par[] )
{ store_double( result, measure_Datatype_simple_Get( par[0].u.intv, par[1].u.infov )); }

static void call_init_Datatype_simple_Get( struct variable *result, struct variable par[] )
{ init_Datatype_simple_Get( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Exchange( struct variable *result, struct variable par[] )
{ store_double( result, measure_Exchange( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Exchange( struct variable *result, struct variable par[] )
{ init_Exchange( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Gather( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather( struct variable *result, struct variable par[] )
{ init_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ init_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Gather_using_Isend_Irecv_Waitall( struct variable *result, struct variable par[] )
{ finalize_Gather_using_Isend_Irecv_Waitall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gather_using_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gather_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gather_using_Send_Recv( struct variable *result, struct variable par[] )
{ init_Gather_using_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Gatherv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Gatherv( struct variable *result, struct variable par[] )
{ init_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Gatherv( struct variable *result, struct variable par[] )
{ finalize_Gatherv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Get_Pingpong( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_Pingpong( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_Pingpong( struct variable *result, struct variable par[] )
{ init_Get_Pingpong( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Get_activewait( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_activewait( par[0].u.intv, par[1].u.infov )); }

static void call_init_Get_activewait( struct variable *result, struct variable par[] )
{ init_Get_activewait( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Get_activewait_sleep( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_activewait_sleep( par[0].u.intv, par[1].u.infov, par[2].u.intv )); }

static void call_init_Get_activewait_sleep( struct variable *result, struct variable par[] )
{ init_Get_activewait_sleep( par[0].u.intv, par[1].u.infov, par[2].u.intv ); }

static void call_measure_Get_callduration( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_callduration( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_callduration( struct variable *result, struct variable par[] )
{ init_Get_callduration( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Get_dedicated( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_dedicated( struct variable *result, struct variable par[] )
{ init_Get_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Get_fence( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_fence( struct variable *result, struct variable par[] )
{ init_Get_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Get_passive( struct variable *result, struct variable par[] )
{ store_double( result, measure_Get_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Get_passive( struct variable *result, struct variable par[] )
{ init_Get_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Isend_callduration( struct variable *result, struct variable par[] )
{ store_double( result, measure_Isend_callduration( par[0].u.intv, par[1].u.datatypev )); }

static void call_init_Isend_callduration( struct variable *result, struct variable par[] )
{ init_Isend_callduration( par[0].u.intv, par[1].u.datatypev ); }

static void call_measure_MPI_IO_close( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_close( par[0].u.intv )); }

static void call_init_MPI_IO_close( struct variable *result, struct variable par[] )
{ init_MPI_IO_close( par[0].u.intv ); }

static void call_finalize_MPI_IO_close( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_close( par[0].u.intv ); }

static void call_measure_MPI_IO_delete( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_delete( par[0].u.intv )); }

static void call_init_MPI_IO_delete( struct variable *result, struct variable par[] )
{ init_MPI_IO_delete( par[0].u.intv ); }

static void call_finalize_MPI_IO_delete( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_delete( par[0].u.intv ); }

static void call_measure_MPI_IO_delete_on_close( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_delete_on_close( par[0].u.intv )); }

static void call_init_MPI_IO_delete_on_close( struct variable *result, struct variable par[] )
{ init_MPI_IO_delete_on_close( par[0].u.intv ); }

static void call_finalize_MPI_IO_delete_on_close( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_delete_on_close( par[0].u.intv ); }

static void call_measure_MPI_IO_file_seek( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_file_seek( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev )); }

static void call_init_MPI_IO_file_seek( struct variable *result, struct variable par[] )
{ init_MPI_IO_file_seek( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev ); }

static void call_finalize_MPI_IO_file_seek( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_file_seek( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev ); }

static void call_measure_MPI_IO_file_seek_shared( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_file_seek_shared( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev )); }

static void call_init_MPI_IO_file_seek_shared( struct variable *result, struct variable par[] )
{ init_MPI_IO_file_seek_shared( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev ); }

static void call_finalize_MPI_IO_file_seek_shared( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_file_seek_shared( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev ); }

static void call_measure_MPI_IO_open( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_open( par[0].u.intv )); }

static void call_init_MPI_IO_open( struct variable *result, struct variable par[] )
{ init_MPI_IO_open( par[0].u.intv ); }

static void call_finalize_MPI_IO_open( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_open( par[0].u.intv ); }

static void call_measure_MPI_IO_open_close( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_open_close( par[0].u.intv )); }

static void call_init_MPI_IO_open_close( struct variable *result, struct variable par[] )
{ init_MPI_IO_open_close( par[0].u.intv ); }

static void call_finalize_MPI_IO_open_close( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_open_close( par[0].u.intv ); }

static void call_measure_MPI_IO_preallocate( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_preallocate( par[0].u.intv, par[1].u.intv )); }

static void call_init_MPI_IO_preallocate( struct variable *result, struct variable par[] )
{ init_MPI_IO_preallocate( par[0].u.intv, par[1].u.intv ); }

static void call_finalize_MPI_IO_preallocate( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_preallocate( par[0].u.intv, par[1].u.intv ); }

static void call_measure_MPI_IO_read_and_distribute( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_and_distribute( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_read_and_distribute( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_and_distribute( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_and_distribute( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_and_distribute( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_read_file( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_file( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_MPI_IO_read_file( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_file( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_MPI_IO_read_file( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_file( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_MPI_IO_read_noncontiguous( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_read_noncontiguous( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_noncontiguous( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_read_once( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv )); }

static void call_init_MPI_IO_read_once( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_once( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv ); }

static void call_measure_MPI_IO_read_random( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_read_random( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_random( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_read_segmented( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_read_segmented( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_read_segmented( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_read_separate( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_read_separate( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_read_separate( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_read_sequential( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_read_sequential( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_read_sequential( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_read_strided( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_read_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_read_strided( struct variable *result, struct variable par[] )
{ init_MPI_IO_read_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_read_strided( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_read_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_write_and_read( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_and_read( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_write_and_read( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_and_read( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_and_read( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_and_read( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_write_noncontiguous( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_write_noncontiguous( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_noncontiguous( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_noncontiguous( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_write_once( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv )); }

static void call_init_MPI_IO_write_once( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_once( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_once( par[0].u.intv, par[1].u.datatypev, par[2].u.datatypev, par[3].u.datatypev, par[4].u.intv, par[5].u.intv ); }

static void call_measure_MPI_IO_write_random( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_write_random( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_random( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_random( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_write_segmented( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_write_segmented( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_write_segmented( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_segmented( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_write_separate( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_write_separate( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_write_separate( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_separate( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_MPI_IO_write_sequential( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv )); }

static void call_init_MPI_IO_write_sequential( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_finalize_MPI_IO_write_sequential( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_sequential( par[0].u.intv, par[1].u.intv, par[2].u.datatypev, par[3].u.datatypev, par[4].u.datatypev, par[5].u.intv ); }

static void call_measure_MPI_IO_write_strided( struct variable *result, struct variable par[] )
{ store_double( result, measure_MPI_IO_write_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv )); }

static void call_init_MPI_IO_write_strided( struct variable *result, struct variable par[] )
{ init_MPI_IO_write_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_finalize_MPI_IO_write_strided( struct variable *result, struct variable par[] )
{ finalize_MPI_IO_write_strided( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev, par[4].u.datatypev, par[5].u.datatypev, par[6].u.intv ); }

static void call_measure_Par_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Par_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Par_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ init_Par_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Pingpong_Bsend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Bsend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Bsend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Bsend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Isend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Isend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Isend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Isend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Issend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Issend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Issend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Issend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Put_Put( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Put_Put( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Pingpong_Put_Put( struct variable *result, struct variable par[] )
{ init_Pingpong_Put_Put( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Iprobe_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Iprobe_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Iprobe_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Iprobe_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Irecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Irecv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Irecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Send_Recv_AT( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Send_Recv_AT( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Send_Recv_AT( struct variable *result, struct variable par[] )
{ init_Pingpong_Send_Recv_AT( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Pingpong_Sendrecv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Sendrecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.datatypev, par[5].u.intv, par[6].u.intv )); }

static void call_init_Pingpong_Sendrecv( struct variable *result, struct variable par[] )
{ init_Pingpong_Sendrecv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.datatypev, par[5].u.intv, par[6].u.intv ); }

static void call_measure_Pingpong_Sendrecv_replace( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Sendrecv_replace( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv )); }

static void call_init_Pingpong_Sendrecv_replace( struct variable *result, struct variable par[] )
{ init_Pingpong_Sendrecv_replace( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Pingpong_Ssend_Recv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Pingpong_Ssend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv )); }

static void call_init_Pingpong_Ssend_Recv( struct variable *result, struct variable par[] )
{ init_Pingpong_Ssend_Recv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.intv ); }

static void call_measure_Put( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put( struct variable *result, struct variable par[] )
{ init_Put( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_Exchange( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_Exchange( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Put_Exchange( struct variable *result, struct variable par[] )
{ init_Put_Exchange( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Put_Exchange_passive( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_Exchange_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Put_Exchange_passive( struct variable *result, struct variable par[] )
{ init_Put_Exchange_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Put_Shift( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_Shift( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Put_Shift( struct variable *result, struct variable par[] )
{ init_Put_Shift( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Put_activewait_get( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_activewait_get( par[0].u.intv, par[1].u.infov )); }

static void call_init_Put_activewait_get( struct variable *result, struct variable par[] )
{ init_Put_activewait_get( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Put_activewait_twosided( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_activewait_twosided( par[0].u.intv, par[1].u.infov )); }

static void call_init_Put_activewait_twosided( struct variable *result, struct variable par[] )
{ init_Put_activewait_twosided( par[0].u.intv, par[1].u.infov ); }

static void call_measure_Put_activewait_twosided_sleep( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_activewait_twosided_sleep( par[0].u.intv, par[1].u.infov, par[2].u.intv )); }

static void call_init_Put_activewait_twosided_sleep( struct variable *result, struct variable par[] )
{ init_Put_activewait_twosided_sleep( par[0].u.intv, par[1].u.infov, par[2].u.intv ); }

static void call_measure_Put_dedicated( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put_dedicated( struct variable *result, struct variable par[] )
{ init_Put_dedicated( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_fence( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put_fence( struct variable *result, struct variable par[] )
{ init_Put_fence( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_fence_bidirectional( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_fence_bidirectional( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put_fence_bidirectional( struct variable *result, struct variable par[] )
{ init_Put_fence_bidirectional( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_passive( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Put_passive( struct variable *result, struct variable par[] )
{ init_Put_passive( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Put_passive_concurrent( struct variable *result, struct variable par[] )
{ store_double( result, measure_Put_passive_concurrent( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Put_passive_concurrent( struct variable *result, struct variable par[] )
{ init_Put_passive_concurrent( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Reduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce( struct variable *result, struct variable par[] )
{ init_Reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ init_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_finalize_Reduce_Scatterv( struct variable *result, struct variable par[] )
{ finalize_Reduce_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Reduce_scatter( struct variable *result, struct variable par[] )
{ store_double( result, measure_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv )); }

static void call_init_Reduce_scatter( struct variable *result, struct variable par[] )
{ init_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_finalize_Reduce_scatter( struct variable *result, struct variable par[] )
{ finalize_Reduce_scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.opv, par[3].u.intv ); }

static void call_measure_Scan( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scan( par[0].u.intv, par[1].u.datatypev, par[2].u.opv )); }

static void call_init_Scan( struct variable *result, struct variable par[] )
{ init_Scan( par[0].u.intv, par[1].u.datatypev, par[2].u.opv ); }

static void call_measure_Scatter( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Scatter( struct variable *result, struct variable par[] )
{ init_Scatter( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Scatterv( struct variable *result, struct variable par[] )
{ store_double( result, measure_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_Scatterv( struct variable *result, struct variable par[] )
{ init_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_finalize_Scatterv( struct variable *result, struct variable par[] )
{ finalize_Scatterv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_Send( struct variable *result, struct variable par[] )
{ store_double( result, measure_Send( par[0].u.intv, par[1].u.datatypev, par[2].u.intv )); }

static void call_init_Send( struct variable *result, struct variable par[] )
{ init_Send( par[0].u.intv, par[1].u.datatypev, par[2].u.intv ); }

static void call_measure_Win_complete( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_complete( struct variable *result, struct variable par[] )
{ init_Win_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_complete_delayed_wait( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_complete_delayed_wait( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_complete_delayed_wait( struct variable *result, struct variable par[] )
{ init_Win_complete_delayed_wait( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_complete_n( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_complete_n( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_complete_n( struct variable *result, struct variable par[] )
{ init_Win_complete_n( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_create( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_create( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_Win_create( struct variable *result, struct variable par[] )
{ init_Win_create( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_Win_fence_close( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_close( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_fence_close( struct variable *result, struct variable par[] )
{ init_Win_fence_close( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_fence_close_collective( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_close_collective( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_fence_close_collective( struct variable *result, struct variable par[] )
{ init_Win_fence_close_collective( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_fence_close_delayed( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_close_delayed( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_fence_close_delayed( struct variable *result, struct variable par[] )
{ init_Win_fence_close_delayed( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_fence_open( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_open( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_fence_open( struct variable *result, struct variable par[] )
{ init_Win_fence_open( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_fence_openclose( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_fence_openclose( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_fence_openclose( struct variable *result, struct variable par[] )
{ init_Win_fence_openclose( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_free( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_free( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_free( struct variable *result, struct variable par[] )
{ init_Win_free( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_lock( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_lock( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_lock( struct variable *result, struct variable par[] )
{ init_Win_lock( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_lock_desync( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_lock_desync( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv, par[6].u.intv )); }

static void call_init_Win_lock_desync( struct variable *result, struct variable par[] )
{ init_Win_lock_desync( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv, par[6].u.intv ); }

static void call_measure_Win_post( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_post( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_post( struct variable *result, struct variable par[] )
{ init_Win_post( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_start( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_start( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_Win_start( struct variable *result, struct variable par[] )
{ init_Win_start( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_Win_start_delayed_post( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_start_delayed_post( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_start_delayed_post( struct variable *result, struct variable par[] )
{ init_Win_start_delayed_post( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_startcomplete( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_startcomplete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_startcomplete( struct variable *result, struct variable par[] )
{ init_Win_startcomplete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_test( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_test( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv )); }

static void call_init_Win_test( struct variable *result, struct variable par[] )
{ init_Win_test( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv ); }

static void call_measure_Win_test_delayed( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_test_delayed( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_test_delayed( struct variable *result, struct variable par[] )
{ init_Win_test_delayed( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_unlock( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_unlock( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv, par[6].u.intv, par[7].u.intv )); }

static void call_init_Win_unlock( struct variable *result, struct variable par[] )
{ init_Win_unlock( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv, par[6].u.intv, par[7].u.intv ); }

static void call_measure_Win_wait_delayed_complete( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_wait_delayed_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_wait_delayed_complete( struct variable *result, struct variable par[] )
{ init_Win_wait_delayed_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Win_wait_early_complete( struct variable *result, struct variable par[] )
{ store_double( result, measure_Win_wait_early_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv )); }

static void call_init_Win_wait_early_complete( struct variable *result, struct variable par[] )
{ init_Win_wait_early_complete( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv, par[4].u.intv, par[5].u.intv ); }

static void call_measure_Wtime( struct variable *result, struct variable par[] )
{ store_double( result, measure_Wtime( par[0].u.intv )); }

static void call_func_alloc_datatype_array( struct variable *result, struct variable par[] )
{ store_darray( result, func_alloc_datatype_array( par[0].u.intv )); }

static void call_func_alloc_int_array( struct variable *result, struct variable par[] )
{ store_iarray( result, func_alloc_int_array( par[0].u.intv )); }

static void call_func_atoi( struct variable *result, struct variable par[] )
{ store_int( result, func_atoi( par[0].u.stringv )); }

static void call_func_cbrt( struct variable *result, struct variable par[] )
{ store_double( result, func_cbrt( par[0].u.doublev )); }

static void call_func_ceil( struct variable *result, struct variable par[] )
{ store_int( result, func_ceil( par[0].u.doublev )); }

static void call_func_choose_barrier_synchronization( struct variable *result, struct variable par[] )
{ func_choose_barrier_synchronization(  ); }

static void call_func_choose_no_synchronization( struct variable *result, struct variable par[] )
{ func_choose_no_synchronization(  ); }

static void call_func_choose_real_synchronization( struct variable *result, struct variable par[] )
{ func_choose_real_synchronization(  ); }

static void call_func_comm( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm( par[0].u.intv )); }

static void call_func_comm2( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm2( par[0].u.intv, par[1].u.intv )); }

static void call_func_comm2_max_latency_with_root( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm2_max_latency_with_root(  )); }

static void call_func_comm4( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm4( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.intv )); }

static void call_func_comm_first_half( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_first_half( par[0].u.commv )); }

static void call_func_comm_from_to( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_from_to( par[0].u.intv, par[1].u.intv )); }

static void call_func_comm_rand( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_rand( par[0].u.intv )); }

static void call_func_comm_second_half( struct variable *result, struct variable par[] )
{ store_comm( result, func_comm_second_half( par[0].u.commv )); }

static void call_func_copy_comm( struct variable *result, struct variable par[] )
{ store_comm( result, func_copy_comm( par[0].u.commv )); }

static void call_iterator_data_access_routines( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_data_access_routines( self, par[0].u.intv )); }

static void call_func_datatype_array_get( struct variable *result, struct variable par[] )
{ store_datatype( result, func_datatype_array_get( par[0].u.darrv, par[1].u.intv )); }

static void call_func_datatype_array_set( struct variable *result, struct variable par[] )
{ func_datatype_array_set( par[0].u.darrv, par[1].u.intv, par[2].u.datatypev ); }

static void call_measure_delayed_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_delayed_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.doublev, par[5].u.intv )); }

static void call_init_delayed_Alltoall( struct variable *result, struct variable par[] )
{ init_delayed_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.doublev, par[5].u.intv ); }

static void call_measure_delayed_Gather( struct variable *result, struct variable par[] )
{ store_double( result, measure_delayed_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev, par[6].u.intv )); }

static void call_init_delayed_Gather( struct variable *result, struct variable par[] )
{ init_delayed_Gather( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev, par[6].u.intv ); }

static void call_func_dims_create( struct variable *result, struct variable par[] )
{ store_iarray( result, func_dims_create( par[0].u.intv, par[1].u.iarrv )); }

static void call_func_floor( struct variable *result, struct variable par[] )
{ store_int( result, func_floor( par[0].u.doublev )); }

static void call_func_get_comm_rank( struct variable *result, struct variable par[] )
{ store_int( result, func_get_comm_rank( par[0].u.commv )); }

static void call_func_get_comm_size( struct variable *result, struct variable par[] )
{ store_int( result, func_get_comm_size( par[0].u.commv )); }

static void call_func_get_env( struct variable *result, struct variable par[] )
{ store_string( result, func_get_env( par[0].u.stringv )); }

static void call_func_get_io_atomicity( struct variable *result, struct variable par[] )
{ store_int( result, func_get_io_atomicity(  )); }

static void call_func_get_io_datarep( struct variable *result, struct variable par[] )
{ store_string( result, func_get_io_datarep(  )); }

static void call_func_get_io_file_sync( struct variable *result, struct variable par[] )
{ store_int( result, func_get_io_file_sync(  )); }

static void call_func_get_io_preallocation( struct variable *result, struct variable par[] )
{ store_int( result, func_get_io_preallocation(  )); }

static void call_func_get_io_unique_open( struct variable *result, struct variable par[] )
{ store_int( result, func_get_io_unique_open(  )); }

static void call_func_get_io_working_dir( struct variable *result, struct variable par[] )
{ store_string( result, func_get_io_working_dir(  )); }

static void call_func_get_mpi_alloc_mem_info( struct variable *result, struct variable par[] )
{ store_info( result, func_get_mpi_alloc_mem_info(  )); }

static void call_func_get_np( struct variable *result, struct variable par[] )
{ store_int( result, func_get_np(  )); }

static void call_func_get_random_int( struct variable *result, struct variable par[] )
{ store_int( result, func_get_random_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_info_create( struct variable *result, struct variable par[] )
{ store_info( result, func_info_create(  )); }

static void call_func_info_free( struct variable *result, struct variable par[] )
{ func_info_free( par[0].u.infov ); }

static void call_func_info_set( struct variable *result, struct variable par[] )
{ func_info_set( par[0].u.infov, par[1].u.stringv, par[2].u.stringv ); }

static void call_func_init_time_accounting( struct variable *result, struct variable par[] )
{ func_init_time_accounting(  ); }

static void call_func_int_array_allgather( struct variable *result, struct variable par[] )
{ store_iarray( result, func_int_array_allgather( par[0].u.commv, par[1].u.intv )); }

static void call_func_int_array_get( struct variable *result, struct variable par[] )
{ store_int( result, func_int_array_get( par[0].u.iarrv, par[1].u.intv )); }

static void call_func_int_array_set( struct variable *result, struct variable par[] )
{ func_int_array_set( par[0].u.iarrv, par[1].u.intv, par[2].u.intv ); }

static void call_func_is_mpi_comm_null( struct variable *result, struct variable par[] )
{ store_int( result, func_is_mpi_comm_null( par[0].u.commv )); }

static void call_func_last_proc( struct variable *result, struct variable par[] )
{ store_int( result, func_last_proc(  )); }

static void call_func_log( struct variable *result, struct variable par[] )
{ store_double( result, func_log( par[0].u.doublev )); }

static void call_func_log10( struct variable *result, struct variable par[] )
{ store_double( result, func_log10( par[0].u.doublev )); }

static void call_func_log2( struct variable *result, struct variable par[] )
{ store_double( result, func_log2( par[0].u.doublev )); }

static void call_func_max_int( struct variable *result, struct variable par[] )
{ store_int( result, func_max_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_min_int( struct variable *result, struct variable par[] )
{ store_int( result, func_min_int( par[0].u.intv, par[1].u.intv )); }

static void call_func_modulo( struct variable *result, struct variable par[] )
{ store_int( result, func_modulo( par[0].u.intv, par[1].u.intv )); }

static void call_func_mpi_processor_names( struct variable *result, struct variable par[] )
{ store_string( result, func_mpi_processor_names(  )); }

static void call_func_mpi_type_contiguous( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_contiguous( par[0].u.intv, par[1].u.datatypev )); }

static void call_func_mpi_type_create_darray( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_create_darray( par[0].u.intv, par[1].u.intv, par[2].u.iarrv, par[3].u.iarrv, par[4].u.iarrv, par[5].u.iarrv, par[6].u.datatypev )); }

static void call_func_mpi_type_create_subarray( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_create_subarray( par[0].u.iarrv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.datatypev )); }

static void call_func_mpi_type_hindexed( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_hindexed( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev )); }

static void call_func_mpi_type_hvector( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_hvector( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev )); }

static void call_func_mpi_type_indexed( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_indexed( par[0].u.iarrv, par[1].u.iarrv, par[2].u.datatypev )); }

static void call_func_mpi_type_struct( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_struct( par[0].u.iarrv, par[1].u.iarrv, par[2].u.darrv )); }

static void call_func_mpi_type_vector( struct variable *result, struct variable par[] )
{ store_datatype( result, func_mpi_type_vector( par[0].u.intv, par[1].u.intv, par[2].u.intv, par[3].u.datatypev )); }

static void call_measure_multi_Alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_multi_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv )); }

static void call_init_multi_Alltoall( struct variable *result, struct variable par[] )
{ init_multi_Alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv ); }

static void call_measure_myalltoallv( struct variable *result, struct variable par[] )
{ store_double( result, measure_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev )); }

static void call_init_myalltoallv( struct variable *result, struct variable par[] )
{ init_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev ); }

static void call_finalize_myalltoallv( struct variable *result, struct variable par[] )
{ finalize_myalltoallv( par[0].u.intv, par[1].u.datatypev, par[2].u.intv, par[3].u.datatypev, par[4].u.intv, par[5].u.doublev ); }

static void call_measure_onesided_allreduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv )); }

static void call_init_onesided_allreduce( struct variable *result, struct variable par[] )
{ init_onesided_allreduce( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv ); }

static void call_measure_onesided_alltoall( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_onesided_alltoall( struct variable *result, struct variable par[] )
{ init_onesided_alltoall( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_onesided_bcast( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv )); }

static void call_init_onesided_bcast( struct variable *result, struct variable par[] )
{ init_onesided_bcast( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.intv ); }

static void call_measure_onesided_borderexchange( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_borderexchange( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv )); }

static void call_init_onesided_borderexchange( struct variable *result, struct variable par[] )
{ init_onesided_borderexchange( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv ); }

static void call_finalize_onesided_borderexchange( struct variable *result, struct variable par[] )
{ finalize_onesided_borderexchange( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv ); }

static void call_measure_onesided_borderexchange_fence( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_borderexchange_fence( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv )); }

static void call_init_onesided_borderexchange_fence( struct variable *result, struct variable par[] )
{ init_onesided_borderexchange_fence( par[0].u.intv, par[1].u.infov, par[2].u.intv, par[3].u.intv ); }

static void call_measure_onesided_dummy( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_dummy( par[0].u.intv, par[1].u.datatypev, par[2].u.infov )); }

static void call_init_onesided_dummy( struct variable *result, struct variable par[] )
{ init_onesided_dummy( par[0].u.intv, par[1].u.datatypev, par[2].u.infov ); }

static void call_measure_onesided_reduce( struct variable *result, struct variable par[] )
{ store_double( result, measure_onesided_reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv )); }

static void call_init_onesided_reduce( struct variable *result, struct variable par[] )
{ init_onesided_reduce( par[0].u.intv, par[1].u.datatypev, par[2].u.infov, par[3].u.opv ); }

static void call_func_power( struct variable *result, struct variable par[] )
{ store_double( result, func_power( par[0].u.doublev, par[1].u.doublev )); }

static void call_func_print_data_access_settings( struct variable *result, struct variable par[] )
{ func_print_data_access_settings( par[0].u.intv, par[1].u.intv ); }

static void call_func_print_file_info( struct variable *result, struct variable par[] )
{ func_print_file_info(  ); }

static void call_func_print_time_accounting_info( struct variable *result, struct variable par[] )
{ func_print_time_accounting_info(  ); }

static void call_iterator_range( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_range( self, par[0].u.intv, par[1].u.intv )); }

static void call_func_reset_file_info( struct variable *result, struct variable par[] )
{ func_reset_file_info(  ); }

static void call_func_round( struct variable *result, struct variable par[] )
{ store_int( result, func_round( par[0].u.doublev )); }

static void call_func_round_to_fivebytes( struct variable *result, struct variable par[] )
{ store_int( result, func_round_to_fivebytes( par[0].u.intv )); }

static void call_func_round_to_fourbytes( struct variable *result, struct variable par[] )
{ store_int( result, func_round_to_fourbytes( par[0].u.intv )); }

static void call_func_set_autorefine_factor( struct variable *result, struct variable par[] )
{ func_set_autorefine_factor( par[0].u.doublev ); }

static void call_func_set_cache_size( struct variable *result, struct variable par[] )
{ func_set_cache_size( par[0].u.intv ); }

static void call_func_set_file_info( struct variable *result, struct variable par[] )
{ func_set_file_info( par[0].u.stringv, par[1].u.stringv ); }

static void call_func_set_io_atomicity( struct variable *result, struct variable par[] )
{ func_set_io_atomicity( par[0].u.intv ); }

static void call_func_set_io_datarep( struct variable *result, struct variable par[] )
{ func_set_io_datarep( par[0].u.stringv ); }

static void call_func_set_io_file_sync( struct variable *result, struct variable par[] )
{ func_set_io_file_sync( par[0].u.intv ); }

static void call_func_set_io_preallocation( struct variable *result, struct variable par[] )
{ func_set_io_preallocation( par[0].u.intv ); }

static void call_func_set_io_unique_open( struct variable *result, struct variable par[] )
{ func_set_io_unique_open( par[0].u.intv ); }

static void call_func_set_io_working_dir( struct variable *result, struct variable par[] )
{ func_set_io_working_dir( par[0].u.stringv ); }

static void call_func_set_mpi_alloc_mem_info( struct variable *result, struct variable par[] )
{ func_set_mpi_alloc_mem_info( par[0].u.infov ); }

static void call_func_set_recv_buffer_alignment( struct variable *result, struct variable par[] )
{ func_set_recv_buffer_alignment( par[0].u.intv ); }

static void call_func_set_send_buffer_alignment( struct variable *result, struct variable par[] )
{ func_set_send_buffer_alignment( par[0].u.intv ); }

static void call_func_set_skampi_buffer( struct variable *result, struct variable par[] )
{ func_set_skampi_buffer( par[0].u.intv ); }

static void call_func_set_skampi_buffer_mpi_alloc_mem( struct variable *result, struct variable par[] )
{ func_set_skampi_buffer_mpi_alloc_mem( par[0].u.intv, par[1].u.infov ); }

static void call_func_sqr( struct variable *result, struct variable par[] )
{ store_double( result, func_sqr( par[0].u.doublev )); }

static void call_func_sqrt( struct variable *result, struct variable par[] )
{ store_double( result, func_sqrt( par[0].u.doublev )); }

static void call_iterator_squares( struct variable *result, struct variable par[], void **self )
{ store_int( result, iterator_squares( self, par[0].u.intv, par[1].u.intv )); }

static void call_func_switch_buffer_cycling_off( struct variable *result, struct variable par[] )
{ func_switch_buffer_cycling_off(  ); }

static void call_func_switch_buffer_cycling_on( struct variable *result, struct variable par[] )
{ func_switch_buffer_cycling_on(  ); }

static void call_func_topology_cartesian( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_cartesian( par[0].u.commv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.intv )); }

static void call_func_topology_graph( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_graph( par[0].u.commv, par[1].u.iarrv, par[2].u.iarrv, par[3].u.intv )); }

static void call_func_topology_graph2( struct variable *result, struct variable par[] )
{ store_comm( result, func_topology_graph2( par[0].u.commv, par[1].u.iarrv, par[2].u.intv )); }

static void call_measure_twosided_borderexchange( struct variable *result, struct variable par[] )
{ store_double( result, measure_twosided_borderexchange( par[0].u.intv, par[1].u.intv, par[2].u.intv )); }

static void call_init_twosided_borderexchange( struct variable *result, struct variable par[] )
{ init_twosided_borderexchange( par[0].u.intv, par[1].u.intv, par[2].u.intv ); }

static void call_finalize_twosided_borderexchange( struct variable *result, struct variable par[] )
{ finalize_twosided_borderexchange( par[0].u.intv, par[1].u.intv, par[2].u.intv ); }

static void call_func_use_mpi_alloc_mem( struct variable *result, struct variable par[] )
{ func_use_mpi_alloc_mem( par[0].u.intv ); }

static void call_measure_waitpattern_down( struct variable *result, struct variable par[] )
{ store_double( result, measure_waitpattern_down( par[0].u.doublev )); }

static void call_init_waitpattern_down( struct variable *result, struct variable par[] )
{ init_waitpattern_down( par[0].u.doublev ); }

static void call_measure_waitpattern_up( struct variable *result, struct variable par[] )
{ store_double( result, measure_waitpattern_up( par[0].u.doublev )); }

static void call_init_waitpattern_up( struct variable *result, struct variable par[] )
{ init_waitpattern_up( par[0].u.doublev ); }

void init_functions(void)
{
  insert_function( &call_init_Accumulate, &call_measure_Accumulate, NULL, "Accumulate", "idho", TYPE_DOUBLE );
  insert_function( &call_init_Accumulate_activewait_twosided, &call_measure_Accumulate_activewait_twosided, NULL, "Accumulate_activewait_twosided", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Accumulate_concurrent, &call_measure_Accumulate_concurrent, NULL, "Accumulate_concurrent", "idhoi", TYPE_DOUBLE );
  insert_function( &call_init_Accumulate_concurrent_multi, &call_measure_Accumulate_concurrent_multi, NULL, "Accumulate_concurrent_multi", "idhioi", TYPE_DOUBLE );
  insert_function( &call_init_Allgather, &call_measure_Allgather, NULL, "Allgather", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Allgatherv, &call_measure_Allgatherv, &call_finalize_Allgatherv, "Allgatherv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Allreduce, &call_measure_Allreduce, NULL, "Allreduce", "ido", TYPE_DOUBLE );
  insert_function( &call_init_Allreduce_using_Reduce_Bcast, &call_measure_Allreduce_using_Reduce_Bcast, NULL, "Allreduce_using_Reduce_Bcast", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Alltoall, &call_measure_Alltoall, NULL, "Alltoall", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoall_using_Isend_Irecv, &call_measure_Alltoall_using_Isend_Irecv, &call_finalize_Alltoall_using_Isend_Irecv, "Alltoall_using_Isend_Irecv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoallv, &call_measure_Alltoallv, &call_finalize_Alltoallv, "Alltoallv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Alltoallv_using_Isend_Irecv, &call_measure_Alltoallv_using_Isend_Irecv, &call_finalize_Alltoallv_using_Isend_Irecv, "Alltoallv_using_Isend_Irecv", "idid", TYPE_DOUBLE );
  insert_function( &call_init_Barrier, &call_measure_Barrier, NULL, "Barrier", "", TYPE_DOUBLE );
  insert_function( &call_init_Bcast, &call_measure_Bcast, NULL, "Bcast", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Bcast_using_Send_Recv, &call_measure_Bcast_using_Send_Recv, &call_finalize_Bcast_using_Send_Recv, "Bcast_using_Send_Recv", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Caching, &call_measure_Caching, NULL, "Caching", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Combining_Put_dedicated, &call_measure_Combining_Put_dedicated, NULL, "Combining_Put_dedicated", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Combining_Put_fence, &call_measure_Combining_Put_fence, NULL, "Combining_Put_fence", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Comm_dup, &call_measure_Comm_dup, NULL, "Comm_dup", "", TYPE_DOUBLE );
  insert_function( &call_init_Comm_split, &call_measure_Comm_split, NULL, "Comm_split", "", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_Get, &call_measure_Datatype_complex_Get, NULL, "Datatype_complex_Get", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_everytime, &call_measure_Datatype_complex_everytime, NULL, "Datatype_complex_everytime", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_mixed_everytime, &call_measure_Datatype_complex_mixed_everytime, NULL, "Datatype_complex_mixed_everytime", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_mixed_once, &call_measure_Datatype_complex_mixed_once, &call_finalize_Datatype_complex_mixed_once, "Datatype_complex_mixed_once", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_complex_once, &call_measure_Datatype_complex_once, &call_finalize_Datatype_complex_once, "Datatype_complex_once", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_int, &call_measure_Datatype_int, NULL, "Datatype_int", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_simple, &call_measure_Datatype_simple, NULL, "Datatype_simple", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Datatype_simple_Get, &call_measure_Datatype_simple_Get, NULL, "Datatype_simple_Get", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Exchange, &call_measure_Exchange, NULL, "Exchange", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Gather, &call_measure_Gather, NULL, "Gather", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gather_using_Isend_Irecv_Waitall, &call_measure_Gather_using_Isend_Irecv_Waitall, &call_finalize_Gather_using_Isend_Irecv_Waitall, "Gather_using_Isend_Irecv_Waitall", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gather_using_Send_Recv, &call_measure_Gather_using_Send_Recv, NULL, "Gather_using_Send_Recv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Gatherv, &call_measure_Gatherv, &call_finalize_Gatherv, "Gatherv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Get_Pingpong, &call_measure_Get_Pingpong, NULL, "Get_Pingpong", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Get_activewait, &call_measure_Get_activewait, NULL, "Get_activewait", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Get_activewait_sleep, &call_measure_Get_activewait_sleep, NULL, "Get_activewait_sleep", "ihi", TYPE_DOUBLE );
  insert_function( &call_init_Get_callduration, &call_measure_Get_callduration, NULL, "Get_callduration", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Get_dedicated, &call_measure_Get_dedicated, NULL, "Get_dedicated", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Get_fence, &call_measure_Get_fence, NULL, "Get_fence", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Get_passive, &call_measure_Get_passive, NULL, "Get_passive", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Isend_callduration, &call_measure_Isend_callduration, NULL, "Isend_callduration", "id", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_close, &call_measure_MPI_IO_close, &call_finalize_MPI_IO_close, "MPI_IO_close", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_delete, &call_measure_MPI_IO_delete, &call_finalize_MPI_IO_delete, "MPI_IO_delete", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_delete_on_close, &call_measure_MPI_IO_delete_on_close, &call_finalize_MPI_IO_delete_on_close, "MPI_IO_delete_on_close", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_file_seek, &call_measure_MPI_IO_file_seek, &call_finalize_MPI_IO_file_seek, "MPI_IO_file_seek", "iiddd", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_file_seek_shared, &call_measure_MPI_IO_file_seek_shared, &call_finalize_MPI_IO_file_seek_shared, "MPI_IO_file_seek_shared", "iiddd", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_open, &call_measure_MPI_IO_open, &call_finalize_MPI_IO_open, "MPI_IO_open", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_open_close, &call_measure_MPI_IO_open_close, &call_finalize_MPI_IO_open_close, "MPI_IO_open_close", "i", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_preallocate, &call_measure_MPI_IO_preallocate, &call_finalize_MPI_IO_preallocate, "MPI_IO_preallocate", "ii", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_and_distribute, &call_measure_MPI_IO_read_and_distribute, &call_finalize_MPI_IO_read_and_distribute, "MPI_IO_read_and_distribute", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_file, &call_measure_MPI_IO_read_file, &call_finalize_MPI_IO_read_file, "MPI_IO_read_file", "idddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_noncontiguous, &call_measure_MPI_IO_read_noncontiguous, &call_finalize_MPI_IO_read_noncontiguous, "MPI_IO_read_noncontiguous", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_once, &call_measure_MPI_IO_read_once, &call_finalize_MPI_IO_read_once, "MPI_IO_read_once", "idddii", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_random, &call_measure_MPI_IO_read_random, &call_finalize_MPI_IO_read_random, "MPI_IO_read_random", "aadddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_segmented, &call_measure_MPI_IO_read_segmented, &call_finalize_MPI_IO_read_segmented, "MPI_IO_read_segmented", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_separate, &call_measure_MPI_IO_read_separate, &call_finalize_MPI_IO_read_separate, "MPI_IO_read_separate", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_sequential, &call_measure_MPI_IO_read_sequential, &call_finalize_MPI_IO_read_sequential, "MPI_IO_read_sequential", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_read_strided, &call_measure_MPI_IO_read_strided, &call_finalize_MPI_IO_read_strided, "MPI_IO_read_strided", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_and_read, &call_measure_MPI_IO_write_and_read, &call_finalize_MPI_IO_write_and_read, "MPI_IO_write_and_read", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_noncontiguous, &call_measure_MPI_IO_write_noncontiguous, &call_finalize_MPI_IO_write_noncontiguous, "MPI_IO_write_noncontiguous", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_once, &call_measure_MPI_IO_write_once, &call_finalize_MPI_IO_write_once, "MPI_IO_write_once", "idddii", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_random, &call_measure_MPI_IO_write_random, &call_finalize_MPI_IO_write_random, "MPI_IO_write_random", "aadddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_segmented, &call_measure_MPI_IO_write_segmented, &call_finalize_MPI_IO_write_segmented, "MPI_IO_write_segmented", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_separate, &call_measure_MPI_IO_write_separate, &call_finalize_MPI_IO_write_separate, "MPI_IO_write_separate", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_sequential, &call_measure_MPI_IO_write_sequential, &call_finalize_MPI_IO_write_sequential, "MPI_IO_write_sequential", "iidddi", TYPE_DOUBLE );
  insert_function( &call_init_MPI_IO_write_strided, &call_measure_MPI_IO_write_strided, &call_finalize_MPI_IO_write_strided, "MPI_IO_write_strided", "iiidddi", TYPE_DOUBLE );
  insert_function( &call_init_Par_Pingpong_Send_Recv, &call_measure_Par_Pingpong_Send_Recv, NULL, "Par_Pingpong_Send_Recv", "idiiii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Bsend_Recv, &call_measure_Pingpong_Bsend_Recv, NULL, "Pingpong_Bsend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Isend_Recv, &call_measure_Pingpong_Isend_Recv, NULL, "Pingpong_Isend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Issend_Recv, &call_measure_Pingpong_Issend_Recv, NULL, "Pingpong_Issend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Put_Put, &call_measure_Pingpong_Put_Put, NULL, "Pingpong_Put_Put", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Iprobe_Recv, &call_measure_Pingpong_Send_Iprobe_Recv, NULL, "Pingpong_Send_Iprobe_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Irecv, &call_measure_Pingpong_Send_Irecv, NULL, "Pingpong_Send_Irecv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Recv, &call_measure_Pingpong_Send_Recv, NULL, "Pingpong_Send_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Send_Recv_AT, &call_measure_Pingpong_Send_Recv_AT, NULL, "Pingpong_Send_Recv_AT", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Sendrecv, &call_measure_Pingpong_Sendrecv, NULL, "Pingpong_Sendrecv", "idiidii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Sendrecv_replace, &call_measure_Pingpong_Sendrecv_replace, NULL, "Pingpong_Sendrecv_replace", "idiii", TYPE_DOUBLE );
  insert_function( &call_init_Pingpong_Ssend_Recv, &call_measure_Pingpong_Ssend_Recv, NULL, "Pingpong_Ssend_Recv", "idii", TYPE_DOUBLE );
  insert_function( &call_init_Put, &call_measure_Put, NULL, "Put", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_Exchange, &call_measure_Put_Exchange, NULL, "Put_Exchange", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Put_Exchange_passive, &call_measure_Put_Exchange_passive, NULL, "Put_Exchange_passive", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Put_Shift, &call_measure_Put_Shift, NULL, "Put_Shift", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Put_activewait_get, &call_measure_Put_activewait_get, NULL, "Put_activewait_get", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Put_activewait_twosided, &call_measure_Put_activewait_twosided, NULL, "Put_activewait_twosided", "ih", TYPE_DOUBLE );
  insert_function( &call_init_Put_activewait_twosided_sleep, &call_measure_Put_activewait_twosided_sleep, NULL, "Put_activewait_twosided_sleep", "ihi", TYPE_DOUBLE );
  insert_function( &call_init_Put_dedicated, &call_measure_Put_dedicated, NULL, "Put_dedicated", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_fence, &call_measure_Put_fence, NULL, "Put_fence", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_fence_bidirectional, &call_measure_Put_fence_bidirectional, NULL, "Put_fence_bidirectional", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_passive, &call_measure_Put_passive, NULL, "Put_passive", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Put_passive_concurrent, &call_measure_Put_passive_concurrent, NULL, "Put_passive_concurrent", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce, &call_measure_Reduce, NULL, "Reduce", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce_Scatterv, &call_measure_Reduce_Scatterv, &call_finalize_Reduce_Scatterv, "Reduce_Scatterv", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Reduce_scatter, &call_measure_Reduce_scatter, &call_finalize_Reduce_scatter, "Reduce_scatter", "idoi", TYPE_DOUBLE );
  insert_function( &call_init_Scan, &call_measure_Scan, NULL, "Scan", "ido", TYPE_DOUBLE );
  insert_function( &call_init_Scatter, &call_measure_Scatter, NULL, "Scatter", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Scatterv, &call_measure_Scatterv, &call_finalize_Scatterv, "Scatterv", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_Send, &call_measure_Send, NULL, "Send", "idi", TYPE_DOUBLE );
  insert_function( &call_init_Win_complete, &call_measure_Win_complete, NULL, "Win_complete", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_complete_delayed_wait, &call_measure_Win_complete_delayed_wait, NULL, "Win_complete_delayed_wait", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_complete_n, &call_measure_Win_complete_n, NULL, "Win_complete_n", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_create, &call_measure_Win_create, NULL, "Win_create", "idh", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_close, &call_measure_Win_fence_close, NULL, "Win_fence_close", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_close_collective, &call_measure_Win_fence_close_collective, NULL, "Win_fence_close_collective", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_close_delayed, &call_measure_Win_fence_close_delayed, NULL, "Win_fence_close_delayed", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_open, &call_measure_Win_fence_open, NULL, "Win_fence_open", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_fence_openclose, &call_measure_Win_fence_openclose, NULL, "Win_fence_openclose", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_free, &call_measure_Win_free, NULL, "Win_free", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_lock, &call_measure_Win_lock, NULL, "Win_lock", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_lock_desync, &call_measure_Win_lock_desync, NULL, "Win_lock_desync", "idhiiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_post, &call_measure_Win_post, NULL, "Win_post", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_start, &call_measure_Win_start, NULL, "Win_start", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_Win_start_delayed_post, &call_measure_Win_start_delayed_post, NULL, "Win_start_delayed_post", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_startcomplete, &call_measure_Win_startcomplete, NULL, "Win_startcomplete", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_test, &call_measure_Win_test, NULL, "Win_test", "idhii", TYPE_DOUBLE );
  insert_function( &call_init_Win_test_delayed, &call_measure_Win_test_delayed, NULL, "Win_test_delayed", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_unlock, &call_measure_Win_unlock, NULL, "Win_unlock", "idhiiiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_wait_delayed_complete, &call_measure_Win_wait_delayed_complete, NULL, "Win_wait_delayed_complete", "idhiii", TYPE_DOUBLE );
  insert_function( &call_init_Win_wait_early_complete, &call_measure_Win_wait_early_complete, NULL, "Win_wait_early_complete", "idhiii", TYPE_DOUBLE );
  insert_function( NULL, &call_measure_Wtime, NULL, "Wtime", "i", TYPE_DOUBLE );
  insert_function( NULL, &call_func_alloc_datatype_array, NULL, "alloc_datatype_array", "i", TYPE_DARRAY );
  insert_function( NULL, &call_func_alloc_int_array, NULL, "alloc_int_array", "i", TYPE_IARRAY );
  insert_function( NULL, &call_func_atoi, NULL, "atoi", "s", TYPE_INTEGER );
  insert_function( NULL, &call_func_cbrt, NULL, "cbrt", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_ceil, NULL, "ceil", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_choose_barrier_synchronization, NULL, "choose_barrier_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_choose_no_synchronization, NULL, "choose_no_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_choose_real_synchronization, NULL, "choose_real_synchronization", "", TYPE_VOID );
  insert_function( NULL, &call_func_comm, NULL, "comm", "i", TYPE_COMM );
  insert_function( NULL, &call_func_comm2, NULL, "comm2", "ii", TYPE_COMM );
  insert_function( NULL, &call_func_comm2_max_latency_with_root, NULL, "comm2_max_latency_with_root", "", TYPE_COMM );
  insert_function( NULL, &call_func_comm4, NULL, "comm4", "iiii", TYPE_COMM );
  insert_function( NULL, &call_func_comm_first_half, NULL, "comm_first_half", "c", TYPE_COMM );
  insert_function( NULL, &call_func_comm_from_to, NULL, "comm_from_to", "ii", TYPE_COMM );
  insert_function( NULL, &call_func_comm_rand, NULL, "comm_rand", "i", TYPE_COMM );
  insert_function( NULL, &call_func_comm_second_half, NULL, "comm_second_half", "c", TYPE_COMM );
  insert_function( NULL, &call_func_copy_comm, NULL, "copy_comm", "c", TYPE_COMM );
  insert_iterator( &call_iterator_data_access_routines, "data_access_routines", "i", TYPE_INTEGER );
  insert_function( NULL, &call_func_datatype_array_get, NULL, "datatype_array_get", "ei", TYPE_DATATYPE );
  insert_function( NULL, &call_func_datatype_array_set, NULL, "datatype_array_set", "eid", TYPE_VOID );
  insert_function( &call_init_delayed_Alltoall, &call_measure_delayed_Alltoall, NULL, "delayed_Alltoall", "ididfi", TYPE_DOUBLE );
  insert_function( &call_init_delayed_Gather, &call_measure_delayed_Gather, NULL, "delayed_Gather", "ididifi", TYPE_DOUBLE );
  insert_function( NULL, &call_func_dims_create, NULL, "dims_create", "ia", TYPE_IARRAY );
  insert_function( NULL, &call_func_floor, NULL, "floor", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_comm_rank, NULL, "get_comm_rank", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_comm_size, NULL, "get_comm_size", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_env, NULL, "get_env", "s", TYPE_STRING );
  insert_function( NULL, &call_func_get_io_atomicity, NULL, "get_io_atomicity", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_io_datarep, NULL, "get_io_datarep", "", TYPE_STRING );
  insert_function( NULL, &call_func_get_io_file_sync, NULL, "get_io_file_sync", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_io_preallocation, NULL, "get_io_preallocation", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_io_unique_open, NULL, "get_io_unique_open", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_io_working_dir, NULL, "get_io_working_dir", "", TYPE_STRING );
  insert_function( NULL, &call_func_get_mpi_alloc_mem_info, NULL, "get_mpi_alloc_mem_info", "", TYPE_INFO );
  insert_function( NULL, &call_func_get_np, NULL, "get_np", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_get_random_int, NULL, "get_random_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_info_create, NULL, "info_create", "", TYPE_INFO );
  insert_function( NULL, &call_func_info_free, NULL, "info_free", "h", TYPE_VOID );
  insert_function( NULL, &call_func_info_set, NULL, "info_set", "hss", TYPE_VOID );
  insert_function( NULL, &call_func_init_time_accounting, NULL, "init_time_accounting", "", TYPE_VOID );
  insert_function( NULL, &call_func_int_array_allgather, NULL, "int_array_allgather", "ci", TYPE_IARRAY );
  insert_function( NULL, &call_func_int_array_get, NULL, "int_array_get", "ai", TYPE_INTEGER );
  insert_function( NULL, &call_func_int_array_set, NULL, "int_array_set", "aii", TYPE_VOID );
  insert_function( NULL, &call_func_is_mpi_comm_null, NULL, "is_mpi_comm_null", "c", TYPE_INTEGER );
  insert_function( NULL, &call_func_last_proc, NULL, "last_proc", "", TYPE_INTEGER );
  insert_function( NULL, &call_func_log, NULL, "log", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_log10, NULL, "log10", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_log2, NULL, "log2", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_max_int, NULL, "max_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_min_int, NULL, "min_int", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_modulo, NULL, "modulo", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_mpi_processor_names, NULL, "mpi_processor_names", "", TYPE_STRING );
  insert_function( NULL, &call_func_mpi_type_contiguous, NULL, "mpi_type_contiguous", "id", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_create_darray, NULL, "mpi_type_create_darray", "iiaaaad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_create_subarray, NULL, "mpi_type_create_subarray", "aaad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_hindexed, NULL, "mpi_type_hindexed", "aad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_hvector, NULL, "mpi_type_hvector", "iiid", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_indexed, NULL, "mpi_type_indexed", "aad", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_struct, NULL, "mpi_type_struct", "aae", TYPE_DATATYPE );
  insert_function( NULL, &call_func_mpi_type_vector, NULL, "mpi_type_vector", "iiid", TYPE_DATATYPE );
  insert_function( &call_init_multi_Alltoall, &call_measure_multi_Alltoall, NULL, "multi_Alltoall", "ididi", TYPE_DOUBLE );
  insert_function( &call_init_myalltoallv, &call_measure_myalltoallv, &call_finalize_myalltoallv, "myalltoallv", "ididif", TYPE_DOUBLE );
  insert_function( &call_init_onesided_allreduce, &call_measure_onesided_allreduce, NULL, "onesided_allreduce", "idho", TYPE_DOUBLE );
  insert_function( &call_init_onesided_alltoall, &call_measure_onesided_alltoall, NULL, "onesided_alltoall", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_onesided_bcast, &call_measure_onesided_bcast, NULL, "onesided_bcast", "idhi", TYPE_DOUBLE );
  insert_function( &call_init_onesided_borderexchange, &call_measure_onesided_borderexchange, &call_finalize_onesided_borderexchange, "onesided_borderexchange", "ihii", TYPE_DOUBLE );
  insert_function( &call_init_onesided_borderexchange_fence, &call_measure_onesided_borderexchange_fence, NULL, "onesided_borderexchange_fence", "ihii", TYPE_DOUBLE );
  insert_function( &call_init_onesided_dummy, &call_measure_onesided_dummy, NULL, "onesided_dummy", "idh", TYPE_DOUBLE );
  insert_function( &call_init_onesided_reduce, &call_measure_onesided_reduce, NULL, "onesided_reduce", "idho", TYPE_DOUBLE );
  insert_function( NULL, &call_func_power, NULL, "power", "ff", TYPE_DOUBLE );
  insert_function( NULL, &call_func_print_data_access_settings, NULL, "print_data_access_settings", "ii", TYPE_VOID );
  insert_function( NULL, &call_func_print_file_info, NULL, "print_file_info", "", TYPE_VOID );
  insert_function( NULL, &call_func_print_time_accounting_info, NULL, "print_time_accounting_info", "", TYPE_VOID );
  insert_iterator( &call_iterator_range, "range", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_reset_file_info, NULL, "reset_file_info", "", TYPE_VOID );
  insert_function( NULL, &call_func_round, NULL, "round", "f", TYPE_INTEGER );
  insert_function( NULL, &call_func_round_to_fivebytes, NULL, "round_to_fivebytes", "i", TYPE_INTEGER );
  insert_function( NULL, &call_func_round_to_fourbytes, NULL, "round_to_fourbytes", "i", TYPE_INTEGER );
  insert_function( NULL, &call_func_set_autorefine_factor, NULL, "set_autorefine_factor", "f", TYPE_VOID );
  insert_function( NULL, &call_func_set_cache_size, NULL, "set_cache_size", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_file_info, NULL, "set_file_info", "ss", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_atomicity, NULL, "set_io_atomicity", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_datarep, NULL, "set_io_datarep", "s", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_file_sync, NULL, "set_io_file_sync", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_preallocation, NULL, "set_io_preallocation", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_unique_open, NULL, "set_io_unique_open", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_io_working_dir, NULL, "set_io_working_dir", "s", TYPE_VOID );
  insert_function( NULL, &call_func_set_mpi_alloc_mem_info, NULL, "set_mpi_alloc_mem_info", "h", TYPE_VOID );
  insert_function( NULL, &call_func_set_recv_buffer_alignment, NULL, "set_recv_buffer_alignment", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_send_buffer_alignment, NULL, "set_send_buffer_alignment", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_skampi_buffer, NULL, "set_skampi_buffer", "i", TYPE_VOID );
  insert_function( NULL, &call_func_set_skampi_buffer_mpi_alloc_mem, NULL, "set_skampi_buffer_mpi_alloc_mem", "ih", TYPE_VOID );
  insert_function( NULL, &call_func_sqr, NULL, "sqr", "f", TYPE_DOUBLE );
  insert_function( NULL, &call_func_sqrt, NULL, "sqrt", "f", TYPE_DOUBLE );
  insert_iterator( &call_iterator_squares, "squares", "ii", TYPE_INTEGER );
  insert_function( NULL, &call_func_switch_buffer_cycling_off, NULL, "switch_buffer_cycling_off", "", TYPE_VOID );
  insert_function( NULL, &call_func_switch_buffer_cycling_on, NULL, "switch_buffer_cycling_on", "", TYPE_VOID );
  insert_function( NULL, &call_func_topology_cartesian, NULL, "topology_cartesian", "caai", TYPE_COMM );
  insert_function( NULL, &call_func_topology_graph, NULL, "topology_graph", "caai", TYPE_COMM );
  insert_function( NULL, &call_func_topology_graph2, NULL, "topology_graph2", "cai", TYPE_COMM );
  insert_function( &call_init_twosided_borderexchange, &call_measure_twosided_borderexchange, &call_finalize_twosided_borderexchange, "twosided_borderexchange", "iii", TYPE_DOUBLE );
  insert_function( NULL, &call_func_use_mpi_alloc_mem, NULL, "use_mpi_alloc_mem", "i", TYPE_VOID );
  insert_function( &call_init_waitpattern_down, &call_measure_waitpattern_down, NULL, "waitpattern_down", "f", TYPE_DOUBLE );
  insert_function( &call_init_waitpattern_up, &call_measure_waitpattern_up, NULL, "waitpattern_up", "f", TYPE_DOUBLE );
}

#endif

