#!/bin/bash

#sudo su -c "echo +5 > /sys/class/rtc/rtc0/wakealarm"
#sudo su -c "echo mem > /sys/power/state"
#exit 0

>tmp.log

suspend_log=""
resume_log=""
suspend_total="0"
resume_total="0"
loop=3
for i in `seq $loop` ; do
	sudo dmesg -c > /dev/null
	#rtc
	sudo su -c "echo +5 > /sys/class/rtc/rtc0/wakealarm"
	sudo su -c "echo mem > /sys/power/state"

	val=`dmesg | grep "suspend devices took" | awk '{print $7}'`
	suspend_total="$suspend_total + $val"
	suspend_log="$suspend_log, $val"	

	val=`dmesg | grep "resume devices took" | awk '{print $7}'`
	resume_total="$resume_total + $val"
	resume_log="$resume_log, $val"	

	sleep 5
done

avg=`echo "scale=3;($suspend_total) / $loop" | bc`
suspend_log="$suspend_log, avg=$avg"	
avg=`echo "scale=3;($resume_total) / $loop" | bc`
resume_log="$resume_log, avg=$avg"	

echo "suspend: $suspend_log" >> tmp.log
echo " resume: $resume_log" >> tmp.log
