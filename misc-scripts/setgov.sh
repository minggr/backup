#!/bin/bash

gov=performance
[ ! -z $1 ] && gov=$1

echo -n avaliable governors:   
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors
[ $gov == "list" ] && exit 0
echo set to: $gov

for f in `find /sys/devices/system/cpu/ -name scaling_governor` ; do
	sudo su -c "echo $gov > $f" > /dev/null 2>&1
	cat $f
done
