begin measurement "MPI_Scatterv-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Scatterv(256, datatype, 256, datatype, 0)
   od
end measurement

begin measurement "MPI_Scatterv-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Scatterv(64kb, datatype, 64kb, datatype, 0)
   od
end measurement

begin measurement "MPI_Scatterv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Scatterv(count, datatype, count, datatype, 0)
   od
end measurement 
