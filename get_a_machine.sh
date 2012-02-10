echo "this file is not intended to be executed directly."
echo "instead, pick lines one by one and copy them (after adapting) into your terminal"
echo
echo "It seems impossible to improve this, since most of these commands start interactive subshells..."
exit 1

# Survive ADSL deconnexions
dtach -A /tmp/mquinson-dtach-socket bash

# Get a bunch of machines
oarsub -I -l 'nodes=2,walltime=10' -t deploy
oarsub -I -l 'nodes=1,walltime=10' -p "cluster='parapluie'" -t deploy

# Gain root privilege on them
kadeploy3 -e squeeze-x64-nfs -f $OAR_NODE_FILE -k ~/.ssh/id_rsa.pub

# Install the stuff that we need
~/SMPI-modeling/tune_all_machines.sh

# Run stuff
mpirun --mca btl self,tcp -machinefile ~/tmp/hostfile BLA

ssh `head -1 ~/tmp/hostfile` 'sh -c "cd SMPI-modeling; ./run.sh aggreg.ncptl"'


for n in `sort -u $OAR_NODE_FILE` ; do ssh $n "rm /tmp/mq-dtach;killall testall.sh java time run.gridsim;sleep 1; kill -KILL -1" ; done; killall ssh
for n in `sort -u $OAR_NODE_FILE` ; do echo "dtach -c /tmp/mq-dtach ~/precious.git/run.gridsim" | ssh -tt -o StrictHostKeyChecking=no -o BatchMode=yes  $n &  done
for n in `sort -u $OAR_NODE_FILE` ; do echo "dtach -c /tmp/mq-dtach ~/precious.git/run.simgrid-masterslave" | ssh -tt -o StrictHostKeyChecking=no -o BatchMode=yes  $n &  done
for n in `sort -u $OAR_NODE_FILE` ; do echo "dtach -c /tmp/mq-dtach ~/precious.git/run.simgrid-goal" | ssh -tt -o StrictHostKeyChecking=no -o BatchMode=yes  $n &  done

ssh `head -1 $OAR_NODE_FILE`
