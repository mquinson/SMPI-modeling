begin measurement "MPI_Allgather-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Allgather(256, datatype, 256, datatype)
   od
end measurement

begin measurement "MPI_Allgather-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Allgather(64kb, datatype, 64kb, datatype)
   od
end measurement

begin measurement "MPI_Allgather-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Allgather(count, datatype, count, datatype)
   od
end measurement 
