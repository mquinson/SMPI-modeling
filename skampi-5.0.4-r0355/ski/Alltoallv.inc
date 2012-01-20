begin measurement "MPI_Alltoallv-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Alltoallv(256, datatype, 256, datatype)
   od
end measurement

begin measurement "MPI_Alltoallv-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Alltoallv(64kb, datatype, 64kb, datatype)
   od
end measurement

begin measurement "MPI_Alltoallv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Alltoallv(count, datatype, count, datatype)
   od
end measurement 

begin measurement "Alltoallv_using_Isend_Irecv-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Alltoallv_using_Isend_Irecv(256, datatype, 256, datatype)
   od
end measurement

begin measurement "Alltoallv_using_Isend_Irecv-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Alltoallv_using_Isend_Irecv(64kb, datatype, 64kb, datatype)
   od
end measurement

begin measurement "Alltoallv_using_Isend_Irecv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Alltoallv_using_Isend_Irecv(count, datatype, count, datatype)
   od
end measurement 
