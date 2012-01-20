begin measurement "MPI_Win_start"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_start(262144, datatype, MPI_INFO_NULL, no_assert)
  od
end measurement

begin measurement "MPI_Win_start with MPI_MODE_NOCHECK"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_start(262144, datatype, MPI_INFO_NULL, MPI_MODE_NOCHECK)
  od
end measurement

begin measurement "MPI_Win_start with delayed MPI_Win_post"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_start_delayed_post(262144, datatype, MPI_INFO_NULL, no_assert, 10000, num_procs-1)
  od
end measurement
