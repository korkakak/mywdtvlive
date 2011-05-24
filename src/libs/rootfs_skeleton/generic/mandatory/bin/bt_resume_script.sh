#!/bin/sh

if [ -f /usr/bin/transmission-remote ]; then
	/usr/bin/transmission-remote 127.0.0.1:$1 -n $2:$3 -t all -s
fi
