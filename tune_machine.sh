apt-get -y install debconf-utils
echo "sun-java6-bin   shared/accepted-sun-dlj-v1-1    boolean true" | debconf-set-selections
apt-get -y install dtach git-core jed python-dev 
# cmake-curses-gui libpcre3-dev sun-java6-jdk  dc cpufrequtils

swapoff -va
/etc/init.d/openibd stop
/etc/init.d/mx stop
ln -s /home/mquinson/install/conceptual/lib/libncptl.so.6 /usr/lib

#if hostname |grep -q pastel ; then
#  echo "Changing the CPU freq to 1000MHz" # because we are on pastel"
#  for cpu in `cpufreq-info |grep analyzing|sed 's/[^0-9]*//g'` ; do
#    cpufreq-set -r -d 1000MHz -u 1000MHz -c $cpu
#  done
#fi
