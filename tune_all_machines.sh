set -ex
# we want to share some files between frontend and deployed nodes, and we use the NFS of home for that
mkdir -p ~/tmp 
if [ "x$OAR_NODE_FILE" != "x" ] ; then
  # we are within the shell launched by OAR
  cp $OAR_NODE_FILE ~/tmp/oar-node-file   # here are the files
  sort -u $OAR_NODE_FILE > ~/tmp/hostfile
fi

# run some post-processing on all remote nodes
cat > ~/tmp/cmd <<EOF
apt-get update
apt-get -y install debconf-utils 
# don't ask me about the dummy licence of Java packages
echo "sun-java6-bin   shared/accepted-sun-dlj-v1-1    boolean true" | debconf-set-selections
apt-get -y install dtach git-core jed python-dev cmake-curses-gui
# cmake-curses-gui libpcre3-dev sun-java6-jdk  dc cpufrequtils # I don't need them now, but sometimes

# Reduce the potential issues in the experiment machine
swapoff -va
/etc/init.d/cron stop

# Stop the fast networks to make sure that we use the ethernet that we are testing
/etc/init.d/openibd stop
/etc/init.d/mx stop

# Install conceptual and akypuera brutally (HOME doesn't work as root)
cp -r /home/mquinson/install/conceptual/lib/* /usr/lib
cp -r /home/mquinson/install/conceptual/bin/* /usr/bin
cp -r /home/mquinson/install/akypuera/lib/* /usr/lib
cp -r /home/mquinson/install/akypuera/bin/* /usr/bin
EOF

chmod o+x ~/tmp/cmd
for node in `cat ~/tmp/hostfile` ; do 
    ssh root@$node "sh -c $HOME/tmp/cmd" ;
    echo "XXX Succesfully tuned $node"
done
    echo "XXX Succesfully tuned all nodes. Yuhu."


# Add this before EOF above when you want to change the CPU steping. But we don't.
#if hostname |grep -q pastel ; then
#  echo "Changing the CPU freq to 1000MHz" # because we are on pastel"
#  for cpu in `cpufreq-info |grep analyzing|sed 's/[^0-9]*//g'` ; do
#    cpufreq-set -r -d 1000MHz -u 1000MHz -c $cpu
#  done
#fi



