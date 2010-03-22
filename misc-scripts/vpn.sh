#!/bin/bash

user=ming.m.lin@intel.com
pass=******
server=192.102.204.70
cmd="sudo openconnect -c /home/mlin/.cert/certificate.p12  --script /etc/vpnc/vpnc-script $server --cafile /home/mlin/.cert/intel-certchain.crt --key-password-from-fsid"

expect_cmd="set timeout 3600; spawn ${cmd};"
expect_cmd="${expect_cmd} expect Username:; send ${user}\r;"
expect_cmd="${expect_cmd} expect Password:; send ${pass}\r;"

expect_cmd="${expect_cmd} expect eof;"

expect -c "${expect_cmd}"
