#!/bin/sh

if [ -f /usr/bin/transmission-remote ]; then
	MISS_PATH=`dirname "$4"`/.missed/
	echo "MISS_PATH is: <$MISS_PATH>"
	if [ ! -d $MISS_PATH ]; then 
		mkdir -p $MISS_PATH
	fi
	cp -arf "$4" $MISS_PATH/.tmp.torrent
	/usr/bin/transmission-remote 127.0.0.1:$1 -n $2:$3 --torrent $5 -r
	echo "start add torrent..................."
	/usr/bin/transmission-remote 127.0.0.1:$1 -n $2:$3 -a $MISS_PATH/.tmp.torrent
	rm -rf $MISS_PATH/.tmp.torrent
	rm -rf "$6"
fi
