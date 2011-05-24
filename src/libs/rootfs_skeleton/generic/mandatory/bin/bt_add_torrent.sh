#!/bin/sh

if [ -f /usr/bin/transmission-remote ]; then
	echo "add torrent start............"
	/usr/bin/transmission-remote 127.0.0.1:$1 -n $2:$3 -a "$4"
	rm -rf "$4"
fi
