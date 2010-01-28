#!/bin/bash

netperf_dir=/home/mlin/netperf/src
cd $netperf_dir

        sudo killall netserver > /dev/null 2>&1

        cpu0=`grep "physical id" /proc/cpuinfo |head -n 1|cut -f 3 -d ' '`
        cpu2=`grep "physical id" /proc/cpuinfo |nl|grep -v " ${cpu0}\$"|tail -n 1|cut -f 1`
        if [ -z "${cpu2}" ]; then
                cpu2=`grep "physical id" /proc/cpuinfo | nl | tail -n 1 | cut -f 1`
        fi
        cpu2=$(($cpu2-1))
        task_srv="taskset -c ${cpu2}"

        sudo ${task_srv} ./netserver &

        sleep 5

        #testname
        flag=UDP_STREAM1

        #source ${path_bmbin}/taskget
        task_cln="taskset -c 0"

        sudo ${task_cln} ./snapshot_script ${flag} 127.0.0.1 

