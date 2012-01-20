begin measurement "MPI_Scatter-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Scatter(256, datatype, 256, datatype, 0)
   od
end measurement

begin measurement "MPI_Scatter-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Scatter(64kb, datatype, 64kb, datatype, 0)
   od
end measurement

begin measurement "MPI_Scatter-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Scatter(count, datatype, count, datatype, 0)
   od
end measurement 
