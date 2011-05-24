#!/bin/sh

if [ -f /opt/qt/bin/run_qt ]; then
	. /opt/qt/bin/run_qt;
fi

cp /lib/part1 /tmp/libQtWebKit.so.4.7.0
cat /lib/part2 >> /tmp/libQtWebKit.so.4.7.0

while [ 1 ]; do
	cd /osd;
	dmaosd $1 $2 $3 $4 $5 $6;
	echo "We are going to reboot...";
	killall -USR1 console_sniffer ;
	sync;
	sleep 3;
	reboot;
done

