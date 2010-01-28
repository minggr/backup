#!/bin/bash
#计算方差

v1=$1
v2=$2
v3=$3
count=3

total=`echo "$v1+$v2+$v3" | bc`
avg=`echo "scale=2;($total)/$count" | bc`

result=`echo "scale=2; (($v1-$avg)*($v1-$avg) + ($v2-$avg)*($v2-$avg) + ($v3-$avg)*($v3-$avg)) / ($count -1)" | bc`

echo $result
