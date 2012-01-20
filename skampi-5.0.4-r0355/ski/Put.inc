begin measurement "Pingpong_Put_Put_fence"
  for count = 1 to 65536 step *sqrt(2) do
    measure comm_pt2pt_slow : Pingpong_Put_Put(count, datatype, MPI_INFO_NULL, 1)
  od
end measurement

begin measurement "MPI_Put callduration"
  for count = 1 to ... step *sqrt(2) do
    measure comm_pt2pt_slow : Put(count, datatype, MPI_INFO_NULL)
  od
end measurement

begin measurement "MPI_Put_dedicated"
  for count = 65536 to ... step *sqrt(2) do
    measure comm_pt2pt_slow : Put_dedicated(count, datatype, MPI_INFO_NULL)
  od
end measurement

begin measurement "MPI_Put_passive"
  for count = 65536 to ... step *sqrt(2) do
    measure comm_pt2pt_slow : Put_passive(count, datatype, MPI_INFO_NULL)
  od
end measurement

begin measurement "MPI_Put_fence"
  for count = 65536 to ... step *sqrt(2) do
    measure comm_pt2pt_slow : Put_fence(count, datatype, MPI_INFO_NULL)
  od
end measurement

#begin measurement "MPI_Put_minlatency"
#  for count = 1 to ... step *sqrt(2) do
#    measure comm_pt2pt_fast : Put(count, datatype, MPI_INFO_NULL)
#  od
#end measurement

begin measurement "MPI_Put_maxlatency"
  for count = 1 to ... step *sqrt(2) do
    measure comm_pt2pt_slow : Put(count, datatype, MPI_INFO_NULL)
  od
end measurement

begin measurement "Combining_short_Puts_fence"
  for count = 1 to 262144 step *sqrt(2) do
    for msgcount = 1 to 16 step *sqrt(2) do
      measure comm_pt2pt_slow : Combining_Put_fence(count, datatype, MPI_INFO_NULL, msgcount)
    od
  od
end measurement

begin measurement "Combining_measure_large_Put"
  for count = 1 to 262144 step *sqrt(2) do
    for msgcount = 1 to 16 step *sqrt(2) do
      measure comm_pt2pt_slow : Put(msgcount*count, datatype, MPI_INFO_NULL)
    od
  od
end measurement
