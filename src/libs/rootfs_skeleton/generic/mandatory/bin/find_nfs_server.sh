#!/bin/sh
#
# Arg1: ipaddress/bits of netmask
# 172.19.148.0/23
#

nmap -p2049 -T4 -sU -sT $1 -oG /tmp/nmap-port-2049-list > /dev/null

LIST=`grep -s "2049/open/" /tmp/nmap-port-2049-list | gawk ' { print $2 }'`

for x in $LIST ; do
	/bin/showmount -e $x; 
done	



