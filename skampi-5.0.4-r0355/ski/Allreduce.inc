begin measurement "MPI_Allreduce-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Allreduce(256, datatype, MPI_SUM)
   od
end measurement

begin measurement "MPI_Allreduce-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Allreduce(count, datatype, MPI_SUM)
   od
end measurement 

begin measurement "Allreduce_using_Reduce_Bcast-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Allreduce_using_Reduce_Bcast(256, datatype, MPI_SUM, 0)
   od
end measurement

begin measurement "Allreduce_using_Reduce_Bcast-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Allreduce_using_Reduce_Bcast(count, datatype, MPI_SUM, 0)
   od
end measurement 
