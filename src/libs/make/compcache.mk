##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  wills_yin@alphanetworks.com
# COMPCACHE makefile fragments 
##############################################################################

#
# Defines
#
ifndef  COMPCACHE_VERSION 
COMPCACHE_VERSION := 0.6.2
endif

COMPCACHE             := compcache#
COMPCACHE_MODULE_NAME := ramzswap.ko#
LINUX_SOURCE 	      := $(TOP_LIBS_DIR)/$(CONF_SIGMA_SDK_VERSION)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/#
COMPCACHE_SERVER_URL  := $(TOP_LIBS_URL)/$(COMPCACHE)/
COMPCACHE_DIR         := $(TOP_LIBS_DIR)#
COMPCACHE_SOURCE      := $(TOP_LIBS_DIR)/$(COMPCACHE)/$(COMPCACHE)-$(COMPCACHE_VERSION)
COMPCACHE_CONFIG      :=

#
# Download  the source 
#
.PHONY: compcache-downloaded

compcache-downloaded: $(COMPCACHE_DIR)/.compcache_downloaded

$(COMPCACHE_DIR)/.compcache_downloaded:
	touch $@

#
# Unpack the source
#
.PHONY: compcache-unpacked

compcache-unpacked: $(COMPCACHE_DIR)/.compcache_unpacked

$(COMPCACHE_DIR)/.compcache_unpacked: $(COMPCACHE_DIR)/.compcache_downloaded
	@if [ ! -d "$(COMPCACHE_SOURCE)" ]; then \
		cd $(TOP_LIBS_DIR)/$(COMPCACHE); \
		tar -zxvf $(COMPCACHE_SOURCE).tar.gz;	\
	fi
	@touch $@


#
# Patch the source
#
.PHONY: compcache-patched

compcache-patched: $(COMPCACHE_DIR)/.compcache_patched

$(COMPCACHE_DIR)/.compcache_patched: $(COMPCACHE_DIR)/.compcache_unpacked
	patch -N -p1 -d $(COMPCACHE_SOURCE) < $(TOP_LIBS_DIR)/$(COMPCACHE)/$(COMPCACHE)-$(COMPCACHE_VERSION).patch
	touch $@


#
# config
#
.PHONY: compcache-config

compcache-config: 

#
# Configuration
#
.PHONY: compcache-configured

compcache-configured:$(COMPCACHE_DIR)/.compcache_configured

$(COMPCACHE_DIR)/.compcache_configured: $(COMPCACHE_DIR)/.compcache_patched
	@echo "Configured $(COMPCACHE) ..."
	@echo "Configured $(COMPCACHE) done"
	@touch $@

#
# Compile
#
.PHONY: compcache-compile

compcache-compile: compcache-configured $(COMPCACHE_DIR)/.compcache_compiled

$(COMPCACHE_DIR)/.compcache_compiled:
	@echo "Compiling $(COMPCACHE) ..."
	make -C $(COMPCACHE_SOURCE) LINUX_KERNEL_SOURCE=$(LINUX_SOURCE)
	@echo "Compiling $(COMPCACHE) done"
	@touch $@

#
# Update
#
.PHONY: compcache-update

compcache-update:
	@echo "Updating $(COMPCACHE) done"

#
# Clean
#
.PHONY: compcache-clean

compcache-clean:
	@if [ -d "$(COMPCACHE_SOURCE)" ]; then\
		make -C $(COMPCACHE_SOURCE) LINUX_KERNEL_SOURCE=$(LINUX_SOURCE) clean; \
	fi

#
# Dist clean
#
.PHONY: compcache-distclean

compcache-distclean:
	rm -f $(TOP_LIBS_DIR)/.compcache*

#
# Install
#
.PHONY: compcache-install

compcache-install:
	if [ -f $(COMPCACHE_SOURCE)/$(COMPCACHE_MODULE_NAME) ]; then \
		cp -f $(COMPCACHE_SOURCE)/$(COMPCACHE_MODULE_NAME) $(TOP_INSTALL_ROOTFS_DIR)/lib/modules/;			\
	fi

