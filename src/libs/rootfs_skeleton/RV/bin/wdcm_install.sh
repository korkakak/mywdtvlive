#!/bin/sh

WDCM_FILE="wdcm"
TMP_FILE="/tmp/$WDCM_FILE"
WDCM_PATH="/tmp/WDTVPriv/.wd_tv/"
SRC_FILE="wdcm.zip"

mount | grep USB1 > $TMP_FILE;

val=$(cut -d\  -f 3 $TMP_FILE);

######debug######
echo "mount point is: $val";
#################

if [ -z $val ] 
then 
	echo "Please insert USB device with WDCM to port1."
	exit 0
fi	

#################################################################
###################INSTALL WDCM TO APPOINTED PATH################
#################################################################

echo "";
echo "################START TO INSTALL WDCM...################";
echo "";

rm -rf $WDCM_PATH/$WDCM_FILE;

unzip -o $val/$SRC_FILE -d $WDCM_PATH;

if [ "$?" == "0" ]; then
	echo "";                       
	echo "###################WDCM INSTALLED SUCCEED.###################";
	echo ""; 
else
	echo "";                      
	echo "###################WDCM INSTALL FAILED.######################";
	echo "";           
fi
rm -rf $TMP_FILE;
