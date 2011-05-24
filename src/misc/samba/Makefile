##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  joshua_lee@alphanetworks.com
# dns_damemon top-level Makefile
##############################################################################

############################# global variables section #######################

TOP        := $(shell pwd)/../../..#

COMPONENT  := misc/smbtree# 
LEAF_BINS  := smbtree

include $(TOP)/Target.mk
include $(TOP)/Target.options

############################ object rules section ############################

include objects.mk

############################ common rules section ############################

include $(TOP_SCRIPTS_DIR)/rules.mk

########################### target section ###################################

LEAF := smbtree
$(LEAF)_OBJECTS := $(OBJECTS)
$(LEAF)_DEPLIBS :=  lib_event_proxy.a #libcommon_util.a
ifeq ($(CONF_SAMBA_PACKAGE_3_5_1_SUPPORT_3_0_28A_CLIENT), xy)
$(LEAF)_SYSLIBS := -lcrypt -lresolv -ldl -lpopt -lpthread -lcurl \
	$(TOP_BUILD_DIR)/binary/third_party_build/lib/libsmbclient.a
else
ifeq ($(CONF_SAMBA_VERSION), 3.0.28a)
$(LEAF)_SYSLIBS := -lcrypt -lresolv -ldl -lpopt -lpthread -lcurl \
	$(TOP_BUILD_DIR)/binary/third_party_build/lib/libsmbclient.a
else
$(LEAF)_SYSLIBS := -lcrypt -lresolv -ldl -lpopt -lpthread -ltalloc -laddns -lcurl\
	$(TOP_BUILD_DIR)/binary/third_party_build/lib/libsmbclient.a \
	$(TOP_BUILD_DIR)/binary/third_party_build/lib/libaddns.a \
	$(TOP_BUILD_DIR)/binary/third_party_build/lib/libwbclient.a \
	$(TOP_BUILD_DIR)/binary/third_party_build/lib/libsmbsharemodes.a \
	$(TOP_BUILD_DIR)/binary/third_party_build/lib/libtdb.a \
	$(TOP_BUILD_DIR)/binary/third_party_build/lib/libnetapi.a
endif
endif
ifeq (x$(CONF_PLATFORM_X86), xy)
	$(LEAF)_SYSLIBS :=  -lkrb5 -lldap -lgss -lpopt -lcrypto -lssl -lcurl $(TOP_BUILD_DIR)/binary/third_party_build/lib/libsmbclient.a
endif

$(BUILD_BIN_CMD)

########################### install section ##################################

install:
	-cp -f $(BIN_DIR)/smbtree $(TOP_INSTALL_ROOTFS_DIR)/bin
	-chmod +x $(TOP_INSTALL_ROOTFS_DIR)/bin/smbtree

