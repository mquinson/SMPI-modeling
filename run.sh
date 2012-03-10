#! /bin/bash

set -e # fail fast

hostfile=~/tmp/hostfile

case $1 in 
 --host=*)
    hostfile=`echo $1|sed 's/^--host=//'`
    shift
    ;;
esac

if [ $# ==  0 ] ; then
  echo "Usage: run.sh <basename of ncptl file>"
  exit 1
fi
BASE=$1
shift
BASE=`basename $BASE .ncptl`

# Recompile the test file
MPILDFLAGS="-laky" \
 ncptl --backend=c_mpi ${BASE}.ncptl

# Pick the right network easily
GIMEIB="--mca btl self,openib"
GIMETCP="--mca btl_tcp_if_include eth0 --mca btl_tcp_if_exclude ib0 --mca btl self,tcp"

# So that Rastro resync the events in the produced trace
rastro_timesync `cat $hostfile` > /tmp/rasto-sync

# Run the test.
#   FAST_INIT should avoid the heavy initialization phase of conceptual
#   NOFORK is mandatory when using IB (or OpenMPI gets really mad at us) and no problem with TCP

#NCPTL_FAST_INIT=1 \
NCPTL_NOFORK=1 \
 mpirun $GIMETCP \
 -machinefile $hostfile ./$BASE
# --pings 200 --pongs 50 --bytes 32 --reps 50

# Also for trace resynchronization
rastro_timesync `cat $hostfile` >> /tmp/rasto-sync

# Pick a good (original) name for the trace file
bkup=0
while [ -e traces/${BASE}.${bkup}.trace ] || [ -e traces/${BASE}.${bkup}.trace.gz ] || [ -e traces/${BASE}.${bkup}.trace.bz2 ]; do
  bkup=`expr $bkup + 1`
done

# Merge the conceptual log files so that they can be embeeded into the final trace without loss of information
if [ -e ${BASE}-0.log ] ; then
  ncptl-logmerge -o ${BASE}.conceptual.log ${BASE}-*.log 
else
  echo "no conceptual log file found. Did you issue a 'all tasks log ...' command?"
  echo "no conceptual log file to be found, sorry" > ${BASE}.conceptual.log
fi

# Actually produces the trace file
aky_converter --sync=/tmp/rasto-sync --commentfile=${BASE}.conceptual.log *.rst > traces/${BASE}.${bkup}.trace

echo "Cleaning ${BASE} ${BASE}.conceptual.log *rst ${BASE}-*.log"
rm -f ${BASE} ${BASE}.conceptual.log *rst ${BASE}-*.log 

echo "Produced traces/${BASE}.${bkup}.trace file; enjoy"
