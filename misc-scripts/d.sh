#!/bin/bash

start_sec=`date +%s`
/bm/bin/runs -t volano -r /bm/recipes/lkp-tt02-x8664.recipe
stop_sec=`date +%s`

((value=$stop_sec-$start_sec))

echo
echo $value secs
