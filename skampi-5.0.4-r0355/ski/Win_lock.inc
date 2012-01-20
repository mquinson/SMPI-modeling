#begin measurement "MPI_Win_lock EXCLUSIVE"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_lock(262144, datatype, MPI_INFO_NULL, 0, MPI_LOCK_EXCLUSIVE)
#  od
#end measurement
#
#begin measurement "MPI_Win_lock SHARED"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_lock(262144, datatype, MPI_INFO_NULL, 0, MPI_LOCK_SHARED)
#  od
#end measurement
#
#begin measurement "MPI_Win_lock EXCLUSIVE NOCHECK"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_lock(262144, datatype, MPI_INFO_NULL, MPI_MODE_NOCHECK, MPI_LOCK_EXCLUSIVE)
#  od
#end measurement
#
#begin measurement "MPI_Win_lock SHARED NOCHECK"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_lock(262144, datatype, MPI_INFO_NULL, MPI_MODE_NOCHECK, MPI_LOCK_SHARED)
#  od
#end measurement
