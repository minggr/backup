#!/bin/bash

tiob_thr_num=32;
((tiob_file_sz = 2560 / tiob_thr_num))

r_flag=3
        # Osmr: r is  0-File Create+Seq.Write, 1-Random Write, 2 -Seq.Read,
        #        3-Random Read, 4 -Seq.Write
        case ${r_flag} in
                0) mode="-k1 -k2 -k3";;
                1) mode="-k0 -k2 -k3";;
                2) mode="-k0 -k1 -k3";;
                3) mode="-k0 -k1 -k3";;
                4) mode="-k1 -k2 -k3";;
                *) mode="";;
        esac


# "c" = not create file again
if [ "$1" == "c" ] ; then
	pids=""
	for ((i=0; i<$tiob_thr_num; i++)) do
		dd if=/dev/zero of=tmp_tiotest.$i bs=1M count=$tiob_file_sz > /dev/null 2>&1 &
		pids="${pids} `echo $!`"
	done

	wait ${pids}
fi

sudo bash -c 'echo 1 > /proc/sys/vm/drop_caches'
tiotest $mode -f $tiob_file_sz -t $tiob_thr_num

