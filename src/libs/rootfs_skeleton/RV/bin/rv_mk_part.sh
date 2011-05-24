#!/bin/sh

if [ -f /sysconfig ]; then
	. /sysconfig
	if [ -f $SYSCONF_STATIC_CONFIG_MOUNT_POINT/config ]; then
		. $SYSCONF_STATIC_CONFIG_MOUNT_POINT/config
	fi
fi


rm -f /tmp/format_sata_done /tmp/no_sata_drive; 

LIST=`cd /sys/block; ls -d sd*`

FOUND=0;

for DEV in $LIST; do
	ls -ld /sys/block/${DEV}/device | grep -q "Tangox\ SATA";
	if [ $? == 0 ]; then
		FOUND=1;
		break;
	fi
done

if [ $FOUND == 0 ]; then
	touch /tmp/format_sata_done;
	touch /tmp/no_sata_drive;
	exit 0;
fi

if [ x$1 == xformat_user_only ]; then
	#
	# If there is /tmp/fake_format, we don't do format. 
	# This is used to test and debug.
	#
	if [ ! -f /tmp/fake_format ]; then
		if [ x${CONF_XFS} = xy ]; then 
			mkfs.xfs -f -l lazy-count=0 -L "WDTVLiveHub" /dev/${DEV}3 > /dev/console;
		else
			echo y | /bin/mkntfs -v:WDTVLiveHub /dev/${DEV}3;
		fi
	fi

	sleep 1;

	echo 1 > /sys/block/${DEV}/uevent

	touch /tmp/format_sata_done
	exit 0;
fi

swapoff /dev/${DEV}2

dd if=/dev/zero of=/dev/${DEV} bs=1048576 count=1

if [ x${CONF_XFS} = xy ]; then 

echo "n
p
1
1
+1000M
n
p
2

+128M
t
2
82
p
n
p
3


p
w
" | fdisk /dev/${DEV} > /dev/console;

else

echo "n
p
1
1
+1000M
n
p
2

+128M
t
2
82
p
n
p
3


t
1
7
t
2
82
t
3
7
p
w
" | fdisk /dev/${DEV} > /dev/console;

fi

sleep 3;

mkswap /dev/${DEV}2
swapon /dev/${DEV}2
if [ x${CONF_XFS} = xy ]; then 
	mkfs.xfs -f -l lazy-count=0 -L "WDTVPriv" /dev/${DEV}1 > /dev/console;
	mkfs.xfs -f -l lazy-count=0 -L "WDTVLiveHub" /dev/${DEV}3 > /dev/console;
else
	echo y | /bin/mkntfs -v:WDTVPriv /dev/${DEV}1;
	echo y | /bin/mkntfs -v:WDTVLiveHub /dev/${DEV}3;
fi

sleep 1;

echo 1 > /sys/block/${DEV}/uevent

touch /tmp/format_sata_done

