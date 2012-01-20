# ----------------------------------------------------------------------------
# the following is interesting for IBM SP; don't know about others
info_nocache = info_create()
info_set(info_nocache,"IBM_win_cache","0")

begin measurement "MPI_Put dedicated CACHE NOCACHE"
  for i = 2 to ... step *sqrt(2) do
    for count = 32768 to ... step *sqrt(2) do
      measure comm_pt2pt_slow : Put(i*count, datatype, MPI_INFO_NULL)
    od
  od
end measurement

begin measurement "MPI_Put dedicated CACHE NOCACHE"
  for i in [MPI_INFO_NULL, info_nocache] do
    for count = 131072 to ... step *sqrt(2) do
      measure comm_pt2pt_slow : Put(count, datatype, i)
    od
  od
end measurement

begin measurement "MPI_Combining NOCACHE"
  for count = 1 to 262144 step *sqrt(2) do
    for msgcount = 1 to 16 step *sqrt(2) do
      measure comm_pt2pt_slow : Combining(count, datatype, info_nocache, msgcount)
    od
  od
end measurement

begin measurement "MPI_Put_Combining NOCACHE"
  for count = 1 to 262144 step *sqrt(2) do
    for msgcount = 1 to 16 step *sqrt(2) do
      measure comm_pt2pt_slow : Put(msgcount*count, datatype, info_nocache)
    od
  od
end measurement
