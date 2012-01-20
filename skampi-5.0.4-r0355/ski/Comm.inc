begin measurement "MPI_Comm_split-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Comm_split()
   od
end measurement

begin measurement "MPI_Comm_dup-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Comm_dup()
   od
end measurement
