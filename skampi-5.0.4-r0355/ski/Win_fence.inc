begin measurement "MPI_Win_fence_open"
  for count = 1 to 268435456 step *sqrt(2) do
    measure MPI_COMM_WORLD : Win_fence_open(count, datatype, MPI_INFO_NULL, no_assert)
  od
end measurement

begin measurement "MPI_Win_fence_close"
  for count = 1 to 262144 step *sqrt(2) do
    measure MPI_COMM_WORLD : Win_fence_close(count, datatype, MPI_INFO_NULL, no_assert, 1)
  od
end measurement

begin measurement "MPI_Win_fence_close-noput"
  for count = 1 to 262144 step *sqrt(2) do
    measure MPI_COMM_WORLD : Win_fence_close(count, datatype, MPI_INFO_NULL, no_assert, 0)
  od
end measurement

begin measurement "MPI_Win_fence_close-nodes-short"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_fence_close(4, datatype, MPI_INFO_NULL, no_assert, 1)
  od
end measurement

begin measurement "MPI_Win_fence_close-nodes-long"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_fence_close(262144, datatype, MPI_INFO_NULL, no_assert, 1)
  od
end measurement

begin measurement "MPI_Win_fence_openclose"
  for procs = 1 to num_procs do
    measure comm(procs) : Win_fence_openclose(262144, datatype, MPI_INFO_NULL, 1)
  od
end measurement
