#!/bin/bash

cd ~/Code/SMPI-modeling/collectives

SGPATH=$1
if [ -z "$SGPATH" ]
then
  if [ -e /home/mquinson/simgrid-3.12 ] ; then
    SGPATH=/home/mquinson/simgrid-3.12
  elif [ -e /home/mquinson/install-3.12 ] ; then
    SGPATH=/home/mquinson/install-3.12
  else
    echo "Usage: $0 /path/to/simgrid/install"
    exit 1
  fi
fi
export PATH="$SGPATH/bin:$PATH"

platformfile="graphene_1024.xml"

# Max sizes to test
maxsize=`expr 2 \* 1024 \* 1024`
maxproc=1024

# The command to run 
cmd='smpirun -platform '"$platformfile"' -np %d ./alltoall %d --cfg=smpi/running_power:20000 --cfg=smpi/alltoall:%s --cfg=maxmin/precision:1e-7'

timefmt="clock:%e user:%U sys:%S swapped:%W exitval:%x max:%Mk"

# Path where to store logs, and filenames of times table, host info
log_folder="log/"
me=`hostname -s`
if [ ! -d "$log_folder" ]; then
    echo "Creating $log_folder folder"
    mkdir -p $log_folder
fi
cpt=0
today=`date +%Y-%m-%d`
while [ -e $log_folder/${today}_$me-$cpt.org ] ; do
  cpt=`expr $cpt + 1`
done
logfile=$log_folder/${today}_$me-$cpt.org

# Some initial cleanups
rm -f $me.timings $me.stdout $me.stderr tmp*

# Make sure that no job will run amok
ulimit -m `expr 1024 \* 1024 \* 100` # no more than 100G per process

echo "#+TITLE: SMPI Collective evaluation" > $logfile
./simgrid_hostinfo.sh >> $logfile

echo "Logging started in file $logfile"
echo "(recompile the binary against $SGPATH)"
echo "* Compilation result" >> $logfile
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$SGPATH/lib"
rm alltoall
smpicc alltoall.c -o alltoall 2>&1 | tee >> $logfile

function roll() {
  max=$1
  rand=`dd if=/dev/urandom count=1 2> /dev/null | cksum | cut -f1 -d" "`
  res=`expr $rand % $max`
  echo $res
}

echo "* Platform file" >> $logfile
cat $platformfile >> $logfile

for nbtest in `seq 1 200` ; do
  echo "* Experiment $nbtest" >> $logfile
  echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

  size=$maxsize;proc=$maxproc
  mem=`cat /proc/meminfo |grep MemFree|sed -e 's/ kB//' -e 's/.* //'` # get the amount of free memory
  while [ $size -eq $maxsize -o `expr $size \* $proc \* 2` -gt `expr $mem` -o $proc -le 2 ] ; do # don't try to malloc more than available or to communicate alone
    #  Consumption is *2 to account for internal SMPI buffers as we use the same buffer for all processes, and for both send and receive
    #  Availability should be $mem*1024 instead of $mem (I'm puzzled) but doing so leads to many overconsumption
    if [ $size -ne $maxsize ] ; then
      echo "Don't do proc:$proc size:$size as there is not enough memory (free mem: $mem kb)"
      echo "Don't do proc:$proc size:$size as there is not enough memory (free mem: $mem kb)" >> $logfile
    fi
    size=`roll $maxsize`
    proc=`roll $maxproc`
  done

  echo "Do proc:$proc size:$size freemem:${mem}k mem usage:"`expr $size \* $proc \* 2`
  echo "Do proc:$proc size:$size freemem:${mem}k mem usage:"`expr $size \* $proc \* 2`>> $logfile

  # pair_* algorithm will often fail as they request processes counts that are powers of two
  # pair_rma pair_light_barrier pair_mpi_barrier pair_one_barrier \
  for algo in   2dmesh 3dmesh basic_linear bruck  \
                pair rdb \
                ring  ring_light_barrier  ring_mpi_barrier  ring_one_barrier  mvapich2_scatter_dest \
                mvapich2 ompi mpich impi ; do
      

      echo "** Test with algorithm $algo" >> $logfile
      (ulimit -t `expr 60 \* 60 \* 1`; # Time watchdog set to 1 hour
       /usr/bin/time -f "$timefmt" -o $me.timings `printf "$cmd" $proc $size "$algo"` > $me.stdout 2> $me.stderr)
      echo "*** Command" >> $logfile
      echo "#+BEGIN_EXAMPLE" >> $logfile
      printf "$cmd" $proc $size $algo >> $logfile
      echo >> $logfile
      echo "#+END_EXAMPLE" >> $logfile

      echo "*** raw stdout" >> $logfile
      echo "#+BEGIN_EXAMPLE" >> $logfile
      cat $me.stdout >> $logfile
      echo >> $logfile
      echo "#+END_EXAMPLE" >> $logfile

      echo "*** raw stderr" >> $logfile
      echo "#+BEGIN_EXAMPLE" >> $logfile
      cat $me.stderr >> $logfile
      echo >> $logfile
      echo "#+END_EXAMPLE" >> $logfile

      echo "*** raw timing information" >> $logfile
      echo "#+BEGIN_EXAMPLE" >> $logfile
      cat $me.timings >> $logfile
      echo >> $logfile
      echo "#+END_EXAMPLE" >> $logfile

      clock=`grep -v Command $me.timings | sed -e 's/ .*//' | sed 's/clock/hostTime/'`
      max=`grep -v Command $me.timings | sed -e 's/.* max/hostMem/'`
      if grep "Command terminated by signal" -q $me.timings ; then
	  echo "*** Result" >> $logfile
	  echo "FAILED_RESULT: signal detected (algo:$algo numproc:$proc msgsize:$size $clock $max)" >> $logfile
	  grep "Command terminated by signal" $me.timings |sed "s/Command/Command for algo $algo/"
      elif grep "Command exited with non-zero status" -q $me.timings ; then
	  echo "*** Result" >> $logfile
	  echo "FAILED_RESULT: non-zero status (algo:$algo numproc:$proc msgsize:$size $clock $max)" >> $logfile
	  grep "Command exited with non-zero status" $me.timings |sed "s/Command/Command for algo $algo/"
      else
	  echo "*** Result" >> $logfile
	  res="PRECIOUS_RESULT algo:$algo numproc:$proc msgsize:$size $clock $max "`grep Success $me.stdout |sed s/Success.*//`
	  echo "$res"
	  echo "$res" >> $logfile
      fi
  done
done
echo "Commit what we have"
git add $logfile
git commit -m "automatic commit of experimental data" $logfile
git push

# Clean everything up
rm -f $me.timings $me.stdout $me.stderr tmp*
