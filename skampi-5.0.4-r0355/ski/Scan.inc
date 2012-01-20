begin measurement "MPI_Scan-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Scan(256, datatype, MPI_SUM)
   od
end measurement

begin measurement "MPI_Scan-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Scan(count, datatype, MPI_SUM)
   od
end measurement 
