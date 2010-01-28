#!/bin/bash

dir=$1
iASL=iasl
Signatures="FACP FACS RSDT XSDT RSDP"
Signatures="$Signatures BERT CPEP ECDT EINJ ERST HEST APIC MSCT SBST SLIT SRAT"
#TDB: SLIC SPMI
Signatures="$Signatures ASF! BOOT DBGP DMAR HPET IBFT IVRS MCFG SPCR TCPA UEFI WAET WDAT WDRT"

prefix=`date +%H%M%S`

#compare_files xxxx.dat xxxx.aml
compare_files()
{
	result=`diff $1 $2`
	if [ ! -z $result ] ; then
		#Error, the 2 files differ
		return 0
	else
		return 1
	fi
}

#check if the file has a valid signature
check_file()
{
	file=$1

	if [ ! -f $file ] ; then
		return 0
	fi

	#msg must be " data"
	msg=`file $file | awk -F: '{print $2}'`
	if [ "$msg" != " data" ] ; then
		return 0
	fi

	read line < $file
	sig=${line:0:4}	

	for s in $Signatures ; do
		if [ "$s" == "$sig" ] ; then
			return 1
		fi
	done

	return 0
}

#test_one <file>
test_one()
{
	file=$1	
        t=`basename $file | awk -F. '{print $1}'`
	#copy file for test
	test_file=`dirname $file`/$prefix.dat
	cp $file $test_file
        test_file_dsl=`dirname $file`/$prefix.dsl
        test_file_aml=`dirname $file`/$prefix.aml

	echo -n $file: 

	if [ -f $test_file_aml ] ; then
		echo `basename $test_file_aml` ALREADY EXIST
		return
	fi

	$iASL -d $test_file > /dev/null 2>&1	
	$iASL -z $test_file_dsl > /dev/null 2>&1	
	if [ ! -f $test_file_aml ] ; then
		echo " Error - compile error"
	else
		result=`diff $file $test_file_aml`
		if [ ! -z "$result" ] ; then
			echo " Fail"
		else
			echo " OK"
		fi
	fi

	rm -f $test_file $test_file_aml $test_file_dsl
}

#test_all <dir>
test_all()
{
	files=`find $dir`

	for f in $files ; do
		if [ -f $f ] ; then
			check_file $f
			result=$?
			if [ "$result" == "1" ] ; then
				test_one $f
			fi
		fi
	done
}

if [ -z $dir ] ; then
	echo "Usage: test-dt.sh <dir>"
	exit 1
fi

if [[ ! -f $dir && ! -d $dir ]] ; then
	echo "$dir does not exist"
	exit 1
fi

test_all $dir
