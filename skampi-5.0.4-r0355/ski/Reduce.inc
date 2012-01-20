begin measurement "MPI_Reduce-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Reduce(256, datatype, MPI_SUM, 0)
   od
end measurement

begin measurement "MPI_Reduce-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Reduce(count, datatype, MPI_SUM, 0)
   od
end measurement 
