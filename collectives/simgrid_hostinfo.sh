#!/bin/bash

# This script is to capture some relevant information about a machine running a given experiment.

set +e
echo "#+DATE: $(eval date)" 
echo "#+AUTHOR: $(eval whoami)" 
echo " "  

echo "* People logged when experiment started:" 
who 
echo "* Hostname" 
hostname 
echo "* System information" 
uname -a 
echo "* CPU info" 
cat /proc/cpuinfo 
echo "* CPU governor" 
if [ -f /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor ];
then
    cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 
else
    echo "Unknown (information not available)" 
fi
echo "* CPU frequency" 
if [ -f /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq ];
then
    cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq 
else
    echo "Unknown (information not available)" 
fi
echo "* Meminfo" 
cat /proc/meminfo 
echo "* Memory hierarchy" 
lstopo --of console 2>&1
echo "* Environment Variables" 
printenv 
echo "* Tools Versions" 
echo "** SimGrid Full Version" 
smpirun -version
echo "** SimGrid Commit Hash" 
smpirun -git-version
echo "** Linux and gcc versions" 
cat /proc/version 
echo "** Gcc info" 
gcc -v 2>&1
echo "** Make tool" 
LC_ALL=C make -v 
echo "** CMake" 
cmake --version 
