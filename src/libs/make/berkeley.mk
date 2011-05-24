##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  sh_yang@alphanetworks.com
# berkeley makefile fragments 
##############################################################################

#
# Defines
#
ifndef CONF_BERKELEY_DB_VERSION 
BERKELEY_VERSION := 4.2.52
else
BERKELEY_VERSION := $(CONF_BERKELEY_DB_VERSION)
endif

BERKELEY             := berkeley_db-$(BERKELEY_VERSION)#
BERKELEY_SERVER_URL  := $(TOP_LIBS_URL)/berkeley_db/$(BERKELEY)#b
BERKELEY_DIR         := $(TOP_LIBS_DIR)#
BERKELEY_SOURCE      := $(BERKELEY_DIR)/$(BERKELEY)
BERKELEY_CONFIG      := 

#ifeq (x$(ENABLE_SHLIB),xyes)
#        BERKELEY_SHARED_FLAGS      := --shared
#endif

#
# Download  the source 
#
.PHONY: berkeley-downloaded

berkeley-downloaded: $(BERKELEY_DIR)/.berkeley_downloaded

$(BERKELEY_DIR)/.berkeley_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: berkeley-unpacked

berkeley-unpacked: $(BERKELEY_DIR)/.berkeley_unpacked

$(BERKELEY_DIR)/.berkeley_unpacked: $(BERKELEY_DIR)/.berkeley_downloaded $(BERKELEY_SOURCE)/$(BERKELEY).tar.gz
	@echo "Unpacking $(BERKELEY) ..."
	if [ -d "$(BERKELEY_SOURCE)/$(BERKELEY)" ]; then \
		rm -fr $(BERKELEY_SOURCE)/$(BERKELEY); \
	fi
	cd $(BERKELEY_SOURCE); tar zxvf  $(BERKELEY).tar.gz
	@echo "Unpacking $(BERKELEY) done"
	@touch $@


#
# Patch the source
#
.PHONY: berkeley-patched

berkeley-patched: $(BERKELEY_DIR)/.berkeley_patched

$(BERKELEY_DIR)/.berkeley_patched: $(BERKELEY_DIR)/.berkeley_unpacked
	# Patching...
# Patching...
	@touch $@

#
# config
#
.PHONY: berkeley-config

berkeley-config: 

#
# Configuration
#
.PHONY: berkeley-configured

berkeley-configured:  berkeley-config $(BERKELEY_DIR)/.berkeley_configured

$(BERKELEY_DIR)/.berkeley_configured: $(BERKELEY_DIR)/.berkeley_patched $(BERKELEY_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(BERKELEY) ..."
	cd $(BERKELEY_SOURCE)/$(BERKELEY)/build_unix;../dist/./configure --prefix=/opt --host=$(BUILD_TARGET) $(BERKELEY_SHARED_FLAGS)
	@echo "Configuring $(BERKELEY) done"
	@touch $@

#
# Compile
#
.PHONY: berkeley-compile

berkeley-compile: $(BERKELEY_DIR)/.berkeley_compiled

$(BERKELEY_DIR)/.berkeley_compiled: $(BERKELEY_DIR)/.berkeley_configured
	@echo "Compiling $(BERKELEY) ..."
	make -C $(BERKELEY_SOURCE)/$(BERKELEY)/build_unix
	make -C $(BERKELEY_SOURCE)/$(BERKELEY)/build_unix install DESTDIR=$(LIBS_INSTALL_PATH)
	@echo "Compiling $(BERKELEY) done"
	@touch $@

#
# Update
#
.PHONY: berkeley-update

berkeley-update:
	@echo "Updating $(BERKELEY) ..."
	@if [ -d "$(BERKELEY_SOURCE)" ]; then\
		$(TOP_UPDATE) $(BERKELEY_SOURCE); \
	fi
	@echo "Updating $(BERKELEY) done"

#
# Status
#
.PHONY: berkeley-status

berkeley-status:
	@echo "Statusing $(BERKELEY) ..."
	@if [ -d "$(BERKELEY_SOURCE)" ]; then\
		$(TOP_STATUS) $(BERKELEY_SOURCE); \
	fi
	@echo "Statusing $(BERKELEY) done"

#
# Clean
#
.PHONY: berkeley-clean

berkeley-clean:
	@rm -f $(BERKELEY_DIR)/.berkeley_configured
	@if [ -d "$(BERKELEY_SOURCE)" ]; then\
		make -C $(BERKELEY_SOURCE)/$(BERKELEY)/build_unix clean; \
	fi

#
# Dist clean
#
.PHONY: berkeley-distclean

berkeley-distclean:
	@rm -f $(BERKELEY_DIR)/.berkeley_*

#
# Install
#
.PHONY: berkeley-install

berkeley-install:
	@echo $(TOP_BUILD_ROOTFS_DIR)
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/lib
	cp -af $(LIBS_INSTALL_PATH)/opt/lib/libdb*so* $(TOP_INSTALL_ROOTFS_DIR)/opt/lib

