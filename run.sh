set -e
../install/conceptual/bin/ncptl --backend=c_mpi pingping.ncptl 
#NCPTL_FAST_INIT=1 \
 mpirun --mca btl_tcp_if_include eth0 --mca btl_tcp_if_exclude ib0 --mca btl self,tcp \
 -machinefile ~/tmp/hostfile \
 ./pingping --pings 20 --bytes 32 --reps 50
