#begin measurement "MPI_Win_wait"
#  for procs = 2 to num_procs do
#    measure comm(procs) : Win_wait(262144, datatype, MPI_INFO_NULL, 0, 1)
#  od
#end measurement
#
#begin measurement "MPI_Win_wait without MPI_Put"
#  for procs = 1 to num_procs do
#    measure comm(procs) : Win_wait(262144, datatype, MPI_INFO_NULL, 0, 0)
#  od
#end measurement
#
#begin measurement "MPI_Win_wait with MPI_MODE_NOPUT"
#  for procs = 1 to num_procs do
#    measure comm(procs) : Win_wait(262144, datatype, MPI_INFO_NULL, MPI_MODE_NOPUT", 0)
#  od
#end measurement

begin measurement "MPI_Win_wait delayed"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_wait_delayed_complete(262144, datatype, MPI_INFO_NULL, no_assert, 1, 50000)
  od
end measurement

begin measurement "MPI_Win_wait delayed,small"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_wait_delayed_complete(1, datatype, MPI_INFO_NULL, no_assert, 1, 50000)
  od
end measurement

begin measurement "MPI_Win_wait delayed without MPI_Put"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_wait_delayed_complete(262144, datatype, MPI_INFO_NULL, no_assert, 0, 50000)
  od
end measurement
