begin measurement "MPI_Accumulate MPI_REPLACE"
  for count = 1 to ... step *sqrt(2) do
    measure comm_pt2pt_slow : Accumulate(count, datatype, MPI_INFO_NULL, MPI_REPLACE)
  od
end measurement

begin measurement "MPI_Accumulate MPI_MAX"
  for count = 1 to ... step *sqrt(2) do
    measure comm_pt2pt_slow : Accumulate(count, datatype, MPI_INFO_NULL, MPI_MAX)
  od
end measurement

begin measurement "MPI_Accumulate MPI_SUM"
  for count = 1 to ... step *sqrt(2) do
    measure comm_pt2pt_slow : Accumulate(count, datatype, MPI_INFO_NULL, MPI_SUM)
  od
end measurement
