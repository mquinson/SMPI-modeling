begin measurement "MPI_Win_complete"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_complete(262144, datatype, MPI_INFO_NULL, no_assert, 1)
  od
end measurement

begin measurement "MPI_Win_complete without MPI_Put"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_complete(262144, datatype, MPI_INFO_NULL, no_assert, 0)
  od
end measurement

begin measurement "MPI_Win_complete delayed"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_complete_delayed_wait(262144, datatype, MPI_INFO_NULL, no_assert, 1, 50000)
  od
end measurement

begin measurement "MPI_Win_complete delayed, small"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_complete_delayed_wait(1, datatype, MPI_INFO_NULL, no_assert, 1, 50000)
  od
end measurement

begin measurement "MPI_Win_complete delayed without MPI_Put"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_complete_delayed_wait(262144, datatype, MPI_INFO_NULL, no_assert, 0, 50000)
  od
end measurement
