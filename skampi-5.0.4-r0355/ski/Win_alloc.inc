begin measurement "MPI_Win_create"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_create(262144, datatype, MPI_INFO_NULL)
  od
end measurement

begin measurement "MPI_Win_free"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_free(262144, datatype, MPI_INFO_NULL, 1)
  od
end measurement

begin measurement "MPI_Win_free_noput"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_free(262144, datatype, MPI_INFO_NULL, 0)
  od
end measurement
