#!/bin/bash

flags=`find /proc/sys/kernel/sched_domain -name flags`

for f in $flags ; do
	value=`cat $f`
	((value=$value & ~0x1000))
	sudo su -c "echo $value > $f"
done
