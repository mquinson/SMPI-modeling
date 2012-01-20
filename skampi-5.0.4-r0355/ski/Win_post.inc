begin measurement "MPI_Win_post"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_post(262144, datatype, MPI_INFO_NULL, no_assert)
  od
end measurement

begin measurement "MPI_Win_post with MPI_MODE_NOCHECK"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_post(262144, datatype, MPI_INFO_NULL, MPI_MODE_NOCHECK)
  od
end measurement

begin measurement "MPI_Win_post with MPI_MODE_NOSTORE"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_post(262144, datatype, MPI_INFO_NULL, MPI_MODE_NOSTORE)
  od
end measurement
