#!/bin/sh

get_sn() 
{

	if [ -f /tmp/static_config/config ]; then
		. /tmp/static_config/config; 
	fi

	if [ -z $HDD_SN ]; then
		HDD_DEV=`mount | grep WDTVLiveHub | gawk '{ print $1 }'`
		HDD_SN=`/bin/smartctl -i $HDD_DEV | grep -s "Serial Number" | gawk -F "-" '{ print $2}'`
	fi

	if [ -z $HDD_SN ]; then
		HDD_SN=`ifconfig eth0 | grep -o HWaddr.* | gawk ' { print $2 }'  | gawk -F ":" '{ print $1$2$3$4$5$6 }'`
	fi

	if [ -z $HDD_SN ]; then
		HDD_SN=unknown
	fi
}


get_version()
{
	if [ -f /conf_src/version ]; then
		VERSION=`cat /conf_src/version`
	else
		VERSION=unknown
	fi
}

do_upload() 
{
	if [ -d /tmp/media/usb/Local/WDTVLiveHub/console ]; then
		LISTS=`find /tmp/media/usb/Local/WDTVLiveHub/console -name log\*`
		for x in $LISTS; do
			NAME=`basename $x`
			echo $NAME
			curl -T $x ftp://ftpext2.wdc.com/WDTVLiveHub/${HDD_SN}/${VERSION}/$NAME --user wdtv:wdtv0124! --ftp-create-dirs -a
			if [ $? -eq 0 ]; then
				echo "upload " $NAME " done"
				mv $x /tmp/media/usb/Local/WDTVLiveHub/console_uploaded
			fi
		done
	fi
}

mkdir /tmp/media/usb/Local/WDTVLiveHub/console_uploaded
get_sn
get_version

while [ 1 ]; do
	do_upload
	sleep 60
done


