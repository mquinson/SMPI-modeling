begin measurement "MPI_Gatherv-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Gatherv(256, datatype, 256, datatype, 0)
   od
end measurement

begin measurement "MPI_Gatherv-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Gatherv(64kb, datatype, 64kb, datatype, 0)
   od
end measurement

begin measurement "MPI_Gatherv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Gatherv(count, datatype, count, datatype, 0)
   od
end measurement 
