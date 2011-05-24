#!/bin/sh

while true; do
	/usr/sbin/rtc -s -w -g pool.ntp.org; 
	if [ $? -eq 0 ]; then
		break
	else
		/usr/sbin/rtc -s -w -g 172.19.10.99;
		if [ $? -eq 0 ]; then
			break
		fi
	fi
	sleep 5;
done

