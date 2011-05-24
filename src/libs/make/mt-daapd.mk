##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  wills_yin@alphanetworks.com
# mt-daapd makefile fragments 
##############################################################################

#
# Defines
#
ifndef  CONF_MT_DAAPD_VERSION 
CONF_MT_DAAPD_VERSION := 0.9~r1696.dfsg_itune10_patched#
endif

MT_DAAPD             := mt-daapd-$(CONF_MT_DAAPD_VERSION)#
MT_DAAPD_SERVER_URL  := $(TOP_LIBS_URL)/mt-daapd/$(MT_DAAPD)#
MT_DAAPD_DIR         := $(TOP_LIBS_DIR)#
MT_DAAPD_SOURCE      := $(MT_DAAPD_DIR)/$(MT_DAAPD)#
MT_DAAPD_CONFIG      := 

ENABLE_SHLIB	:= yes
ifeq (x$(ENABLE_SHLIB),xno)
	DAAPD_SHARED_FLAGS      := --enable-shared=no
endif

#
# Download  the source 
#
.PHONY: mt-daapd-downloaded

mt-daapd-downloaded: $(MT_DAAPD_DIR)/.mt-daapd_downloaded

$(MT_DAAPD_DIR)/.mt-daapd_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: mt-daapd-unpacked

mt-daapd-unpacked: $(MT_DAAPD_DIR)/.mt-daapd_unpacked

$(MT_DAAPD_DIR)/.mt-daapd_unpacked: $(MT_DAAPD_DIR)/.mt-daapd_downloaded $(wildcard $(MT_DAAPD_SOURCE)/*.tar.gz)
	@echo "Unpacking $(MT_DAAPD) ..."
	if [ -d "$(MT_DAAPD_SOURCE)/$(MT_DAAPD)" ]; then \
		rm -fr $(MT_DAAPD_SOURCE)/$(MT_DAAPD); \
	fi
	(cd $(MT_DAAPD_SOURCE); tar xvzf $(MT_DAAPD).tar.gz)
	@echo "Unpacking $(MT_DAAPD) done"
	@touch $@


#
# Patch the source
#
.PHONY: mt-daapd-patched

mt-daapd-patched: $(MT_DAAPD_DIR)/.mt-daapd_patched

$(MT_DAAPD_DIR)/.mt-daapd_patched: $(MT_DAAPD_DIR)/.mt-daapd_unpacked
	@echo "Patching $(MT_DAAPD) ..."
	cd $(MT_DAAPD_SOURCE); patch -p0 < mt-daapd.patch
	@echo "Patching $(MT_DAAPD) done"
	@touch $@

#
# config
#
.PHONY: mt-daapd-config

mt-daapd-config: 

#
# Configuration
#
.PHONY: mt-daapd-configured

mt-daapd-configured:  mt-daapd-config $(MT_DAAPD_DIR)/.mt-daapd_configured

$(MT_DAAPD_DIR)/.mt-daapd_configured: $(MT_DAAPD_DIR)/.mt-daapd_patched $(MT_DAAPD_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(MT_DAAPD) ..."
	cd $(MT_DAAPD_SOURCE)/$(MT_DAAPD); \
		CFLAGS="-I$(LIBS_INSTALL_PATH)/include " \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib " \
		AVAHI_CFLAGS="-I$(LIBS_INSTALL_PATH)/include " \
		AVAHI_LIBS="-L$(LIBS_INSTALL_PATH)/lib -lavahi-core -lavahi-common -lavahi-client" \
		./configure --prefix=$(LIBS_INSTALL_PATH) \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--enable-sqlite3 \
		$(DAAPD_SHARED_FLAGS) \
		--enable-avahi
	@echo "Configuring $(MT_DAAPD) done"
	@touch $@

#
# Compile
#
.PHONY: mt-daapd-compile

mt-daapd-compile: $(MT_DAAPD_DIR)/.mt-daapd_compiled

$(MT_DAAPD_DIR)/.mt-daapd_compiled: $(MT_DAAPD_DIR)/.mt-daapd_configured
	@echo "Compiling $(MT_DAAPD) ..."
	make -C $(MT_DAAPD_SOURCE)/$(MT_DAAPD)
	make -C $(MT_DAAPD_SOURCE)/$(MT_DAAPD) install
	@echo "Compiling $(MT_DAAPD) done"
	@touch $@

#
# Update
#
.PHONY: mt-daapd-update

mt-daapd-update:
	@echo "Updating $(MT_DAAPD) ..."
	@if [ -d "$(MT_DAAPD_SOURCE)" ]; then\
		$(TOP_UPDATE) $(MT_DAAPD_SOURCE); \
	fi
	@echo "Updating $(MT_DAAPD) done"

#
# Status
#
.PHONY: mt-daapd-status

mt-daapd-status:
	@echo "Statusing $(MT_DAAPD) ..."
	@if [ -d "$(MT_DAAPD_SOURCE)" ]; then\
		$(TOP_STATUS) $(MT_DAAPD_SOURCE); \
	fi
	@echo "Statusing $(MT_DAAPD) done"

#
# Clean
#
.PHONY: mt-daapd-clean

mt-daapd-clean:
	@rm -f $(MT_DAAPD_DIR)/.mt-daapd_configured
	@if [ -d "$(MT_DAAPD_SOURCE)" ]; then\
		make -C $(MT_DAAPD_SOURCE)/$(MT_DAAPD) clean; \
	fi

#
# Dist clean
#
.PHONY: mt-daapd-distclean

mt-daapd-distclean:
	@rm -f $(MT_DAAPD_DIR)/.mt-daapd*

#
# Install
#
.PHONY: mt-daapd-install

mt-daapd-install:
	@echo "Installing $(MT_DAAPD) ..."
	-cp -af $(LIBS_INSTALL_PATH)/sbin/mt-daapd $(TOP_INSTALL_ROOTFS_DIR)/bin
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/lib/mt-daapd/plugins
	-cp -af $(LIBS_INSTALL_PATH)/lib/mt-daapd/plugins/*.so $(TOP_INSTALL_ROOTFS_DIR)/lib/mt-daapd/plugins

