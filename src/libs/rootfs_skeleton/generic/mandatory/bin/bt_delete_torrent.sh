#!/bin/sh

if [ -f /usr/bin/transmission-remote ]; then
	echo "delete torrent start............"
	/usr/bin/transmission-remote 127.0.0.1:$1 -n $2:$3 --torrent $4 -r
fi