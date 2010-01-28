#!/bin/bash

JAVA_OPTION="-Xmx1500m -Xms1500m -Xns750m -XXaggressive -Xlargepages -XXlazyUnlocking -Xgc:genpar -XXtlasize:min=16k,preferred=64k"
HUGEMEMSIZE=3g
LOOP_CLIENT_COUNT=25000
LOOP_CLIENT_REPEAT_TIMES=3

sudo su -c 'echo "1">/proc/sys/kernel/sched_compat_yield'

#set hugememsize to $HUGEMEMSIZE
hugepage_dir=/mnt/hugepages
if [ ! -d ${hugepage_dir} ] ; then
	sudo mkdir -m 0777 -p ${hugepage_dir}
	sudo chmod a+rxw ${hugepage_dir}
fi

if ! (mount|grep "/mnt/hugepages") ; then
	sudo mount -t hugetlbfs none ${hugepage_dir}
fi

hugepagesize=`cat /proc/meminfo|grep Hugepagesize|awk '{print $2}'`
hugememsize=`echo "${HUGEMEMSIZE}"|sed "s/[g,G]/ /g"|awk '{print $1}'`
hugememsize=$((${hugememsize}*1024*1024))

nr_hugepages=$((${hugememsize}/${hugepagesize}))
echo $nr_hugepages

if ! sudo /bin/sh -c "echo $nr_hugepages >/proc/sys/vm/nr_hugepages" ;then return 1 ; fi
if ! sudo /bin/sh -c "echo \"1\" >/proc/sys/vm/overcommit_memory" ;then return 1; fi
if ! sudo chmod a+rxw ${hugepage_dir} ; then return 1; fi

rm -rf volano

java_version=jrockit-R27.3.1-jre1.5.0_11.x86_64
tar zxvf volano.tgz > /dev/null
tar zxvf ${java_version}.tgz -C volano > /dev/null
cd volano
mv jrockit* $java_version
export JAVA_HOME=`pwd`/${java_version}
export java_version JAVA_OPTION LOOP_CLIENT_COUNT LOOP_CLIENT_REPEAT_TIMES

cd volano
sudo killall loopserver > /dev/null 2>&1
./loopserver.sh bea50 &
sleep 5
./loopclient.sh bea50
