begin measurement "MPI_Alltoall-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Alltoall(256, datatype, 256, datatype)
   od
end measurement

begin measurement "MPI_Alltoall-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Alltoall(64kb, datatype, 64kb, datatype)
   od
end measurement

begin measurement "MPI_Alltoall-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Alltoall(count, datatype, count, datatype)
   od
end measurement 

begin measurement "Alltoall_using_Isend_Irecv-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Alltoall_using_Isend_Irecv(256, datatype, 256, datatype)
   od
end measurement

begin measurement "Alltoall_using_Isend_Irecv-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Alltoall_using_Isend_Irecv(64kb, datatype, 64kb, datatype)
   od
end measurement

begin measurement "Alltoall_using_Isend_Irecv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Alltoall_using_Isend_Irecv(count, datatype, count, datatype)
   od
end measurement 
