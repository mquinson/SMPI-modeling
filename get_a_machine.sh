dtach -A /tmp/mquinson-dtach-socket bash
oarsub -I -l 'nodes=2,walltime=10' -t deploy
oarsub -I -l 'nodes=1,walltime=10' -p "cluster='parapluie'" -t deploy
kadeploy3 -e squeeze-x64-nfs -f $OAR_NODE_FILE -k ~/.ssh/id_rsa.pub

for node in `cat $OAR_NODE_FILE|sort -u` ; do ssh root@$node "sh ~mquinson/skampi/tune_machine.sh" ; done
cp $OAR_NODE_FILE ~/tmp/oar-node-file
sort -u $OAR_NODE_FILE > ~/tmp/hostfile

mpirun --mca btl self,tcp -machinefile ~/tmp/hostfile BLA

for n in `sort -u $OAR_NODE_FILE` ; do ssh $n "rm /tmp/mq-dtach;killall testall.sh java time run.gridsim;sleep 1; kill -KILL -1" ; done; killall ssh
for n in `sort -u $OAR_NODE_FILE` ; do echo "dtach -c /tmp/mq-dtach ~/precious.git/run.gridsim" | ssh -tt -o StrictHostKeyChecking=no -o BatchMode=yes  $n &  done
for n in `sort -u $OAR_NODE_FILE` ; do echo "dtach -c /tmp/mq-dtach ~/precious.git/run.simgrid-masterslave" | ssh -tt -o StrictHostKeyChecking=no -o BatchMode=yes  $n &  done
for n in `sort -u $OAR_NODE_FILE` ; do echo "dtach -c /tmp/mq-dtach ~/precious.git/run.simgrid-goal" | ssh -tt -o StrictHostKeyChecking=no -o BatchMode=yes  $n &  done

ssh `head -1 $OAR_NODE_FILE`
