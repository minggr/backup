#!/bin/bash

function profile_start()
{
	sudo opcontrol --reset
	sudo opcontrol --start
}

function profile_stop()
{
	mykernel=`uname -r`
	postfix=$mykernel.`date +%m%d%H%M%S`

	sudo opcontrol --stop
	sudo opcontrol --dump
	opreport -l ~/vmlinux-`uname -r` > ~/log.$postfix
}

	cd ~/cpu2000
	path_run=`pwd`
	for cpu2000f in extract_config extract_raw license.pl printkids.pl printpath.pl rawformat runspec specdiff verify_md5 benchmark.pm ; do
		sed "s%<cpu2000_path>%$path_run%" bin/${cpu2000f} > ./tmp
		mv ./tmp bin/${cpu2000f} 
		chmod a+rwx bin/${cpu2000f} 
	done
	export PATH=${path_run}/bin:$PATH
	rm -rf benchspec/C*/*/run result/*

	source shrc

	configfile="cpu2000.v1.3.ic91p.p4.sse3.linux64.em64t.apr2006.cfg"

	#--noreportable int
	otheroptions="--noreportable fp"
	#profile_start
	runspec -c $configfile $otheroptions
	#profile_stop

	value=`grep "SPEC[a-zA-Z_]*_base2000" result/C*2000.*.asc | awk '{print $3}'`
	if echo "${otheroptions}" | grep -w int; then
		printf "int,${value}\n"
	else
		printf "fp,${value}\n"
	fi
