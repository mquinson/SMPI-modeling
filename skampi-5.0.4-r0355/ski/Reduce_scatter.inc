begin measurement "MPI_Reduce_scatter-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Reduce_scatter(256, datatype, MPI_SUM, 0)
   od
end measurement

begin measurement "MPI_Reduce_scatter-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Reduce_scatter(count, datatype, MPI_SUM, 0)
   od
end measurement 
