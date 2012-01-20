begin measurement "MPI_Barrier-procs"
   for procs = 2 to num_procs do
      measure comm(procs) : Barrier()
   od
end measurement
