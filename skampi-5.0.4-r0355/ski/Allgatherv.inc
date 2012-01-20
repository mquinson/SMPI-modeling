begin measurement "MPI_Allgatherv-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Allgatherv(256, datatype, 256, datatype)
   od
end measurement

begin measurement "MPI_Allgatherv-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Allgatherv(64kb, datatype, 64kb, datatype)
   od
end measurement

begin measurement "MPI_Allgatherv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Allgatherv(count, datatype, count, datatype)
   od
end measurement 
