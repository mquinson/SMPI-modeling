begin measurement "Reduce_Scatterv-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Reduce_Scatterv(256, datatype, MPI_SUM, 0)
   od
end measurement

begin measurement "Reduce_Scatterv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Reduce_Scatterv(count, datatype, MPI_SUM, 0)
   od
end measurement 
