begin measurement "MPI_Gather-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Gather(256, datatype, 256, datatype, 0)
   od
end measurement

begin measurement "MPI_Gather-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Gather(64kb, datatype, 64kb, datatype, 0)
   od
end measurement

begin measurement "MPI_Gather-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Gather(count, datatype, count, datatype, 0)
   od
end measurement 

begin measurement "Gather_using_Send_Recv-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Gather_using_Send_Recv(256, datatype, 256, datatype, 0)
   od
end measurement

begin measurement "Gather_using_Send_Recv-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Gather_using_Send_Recv(64kb, datatype, 64kb, datatype, 0)
   od
end measurement

begin measurement "Gather_using_Send_Recv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Gather_using_Send_Recv(count, datatype, count, datatype, 0)
   od
end measurement 

begin measurement "Gather_using_Isend_Irecv_Waitall-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Gather_using_Isend_Irecv_Waitall(256, datatype, 256, datatype, 0)
   od
end measurement

begin measurement "Gather_using_Isend_Irecv_Waitall-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Gather_using_Isend_Irecv_Waitall(64kb, datatype, 64kb, datatype, 0)
   od
end measurement

begin measurement "Gather_using_Isend_Irecv_Waitall-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Gather_using_Isend_Irecv_Waitall(count, datatype, count, datatype, 0)
   od
end measurement 
