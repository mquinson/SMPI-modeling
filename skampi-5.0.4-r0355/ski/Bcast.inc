begin measurement "MPI_Bcast-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Bcast(256, datatype, 0)
   od
end measurement

begin measurement "MPI_Bcast-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Bcast(64kb, datatype, 0)
   od
end measurement

begin measurement "MPI_Bcast-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Bcast(count, datatype, 0)
   od
end measurement 

begin measurement "Bcast_using_Send_Recv-procs-short"
   for procs = 2 to num_procs do
      measure comm(procs) : Bcast_using_Send_Recv(256, datatype, 0)
   od
end measurement

begin measurement "Bcast_using_Send_Recv-procs-long"
   for procs = 2 to num_procs do
      measure comm(procs) : Bcast_using_Send_Recv(64kb, datatype, 0)
   od
end measurement

begin measurement "Bcast_using_Send_Recv-length"
   for count = 1 to ... step *sqrt(2) do
      measure comm_all : Bcast_using_Send_Recv(count, datatype, 0)
   od
end measurement 
