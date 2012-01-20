#begin measurement "MPI_Win_unlock SHARED"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_unlock(262144, datatype, MPI_INFO_NULL, no_assert, MPI_LOCK_SHARED, 1, 0)
#  od
#end measurement
#
#begin measurement "MPI_Win_unlock SHARED delayed"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_unlock(262144, datatype, MPI_INFO_NULL, no_assert, MPI_LOCK_SHARED, 1, 10000)
#  od
#end measurement
#
#begin measurement "MPI_Win_unlock SHARED small"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_unlock(1, datatype, MPI_INFO_NULL, no_assert, MPI_LOCK_SHARED, 1, 0)
#  od
#end measurement
#
#begin measurement "MPI_Win_unlock SHARED without MPI_Put"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_unlock(262144, datatype, MPI_INFO_NULL, no_assert, MPI_LOCK_SHARED, 0, 0)
#  od
#end measurement
