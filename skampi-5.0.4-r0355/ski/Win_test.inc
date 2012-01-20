begin measurement "MPI_Win_test"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_test(262144, datatype, MPI_INFO_NULL, no_assert, 1)
  od
end measurement

begin measurement "MPI_Win_test without MPI_Put"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_test(262144, datatype, MPI_INFO_NULL, no_assert, 0)
  od
end measurement

begin measurement "MPI_Win_test delayed"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_test_delayed(262144, datatype, MPI_INFO_NULL, no_assert, 1, 10000)
  od
end measurement

begin measurement "MPI_Win_test delayed without MPI_Put"
  for procs = 2 to num_procs do
    measure comm(procs) : Win_test_delayed(262144, datatype, MPI_INFO_NULL, no_assert, 0, 10000)
  od
end measurement
