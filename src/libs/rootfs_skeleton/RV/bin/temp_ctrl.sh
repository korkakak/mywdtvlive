#!/bin/sh

rm -f /tmp/format_sata_done /tmp/no_sata_drive; 

LIST=`cd /sys/block; ls -d sd*`

FOUND=0;

for DEV in $LIST; do
	ls -ld /sys/block/${DEV}/device | grep -q SATA;
	if [ $? == 0 ]; then
		FOUND=1;
		break;
	fi
done

if [ $FOUND == 0 ]; then
	exit 0;
fi

if [ ! -f /proc/wdtvrv/fan ]; then 
	exit 0;
fi

LIST=`smartctl -A /dev/${DEV} | grep Temperature_Celsius`


CNT=0;
for X in $LIST; do 
	CNT=`expr $CNT + 1`;
	if [ $CNT == 10 ]; then
		break;
	fi
done

#echo "HD Temp.:" $X

if [ $X -gt 64 ]; then
	echo 10 > /proc/wdtvrv/fan;
	echo "It reaches 64 degree. Shutdown system.";
	touch /tmp/over_heated;
	touch /tmp/over_heated_shutdown_device;
elif [ $X -gt 60 ]; then
	echo 10 > /proc/wdtvrv/fan;
	echo "It reaches 60 degree. Show error message on screen";
	touch /tmp/over_heated;
	touch /tmp/over_heated_warning;
fi

if [ -f /tmp/over_heated ]; then
	if [ $X -lt 40 ]; then
		echo 5 > /proc/wdtvrv/fan;
		echo "Lower than 40, half speed";
		rm -f /tmp/over_heated
	else
		echo "Fan full speed";
		echo 10 > /proc/wdtvrv/fan;
	fi
else
	if [ $X -gt 50 ]; then
		echo "Greater than 50, full speed";
		echo 10 > /proc/wdtvrv/fan;
		touch /tmp/over_heated;
	else
		echo 5 > /proc/wdtvrv/fan;
	fi
fi

