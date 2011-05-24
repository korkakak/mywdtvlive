##############################################################################
# Copyright (C) alphanetworks 2011-- 
# All Rights Reserved -- Company Confidential
# Author:  ivan_yen@alphanetworks.com
# libmrss makefile fragments 
##############################################################################

#
# Defines
#
ifndef  CONF_LIBMRSS_VERSION 
CONF_LIBMRSS_VERSION := 0.19.2
endif

LIBMRSS             := libmrss-$(CONF_LIBMRSS_VERSION)#
LIBMRSS_SERVER_URL  := $(TOP_LIBS_URL)/libmrss/$(LIBMRSS)#
LIBMRSS_DIR         := $(TOP_LIBS_DIR)#
LIBMRSS_SOURCE      := $(LIBMRSS_DIR)/$(LIBMRSS)#
LIBMRSS_CONFIG      := 

LD_PATH := -L$(LIBS_INSTALL_PATH)/lib -Wl,-rpath-link -Wl,$(SYSLIB_PREFIX)/lib
CFLAGS := -I$(LIBS_INSTALL_PATH)/include

ifeq ($(CONF_PLATFORM_X86), y)
ENABLE_SHLIB = yes
endif

LIBMRSS_CONFIGURE := --prefix=$(LIBS_INSTALL_PATH) --build=$(BUILD_HOST) --host=$(BUILD_TARGET) --enable-shared=$(ENABLE_SHLIB) CFLAGS="$(CFLAGS)" LDFLAGS="$(LD_PATH)"

#
# Download  the source 
#
.PHONY: libmrss-downloaded

libmrss-downloaded: $(LIBMRSS_DIR)/.libmrss_downloaded

$(LIBMRSS_DIR)/.libmrss_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libmrss-unpacked

libmrss-unpacked: $(LIBMRSS_DIR)/.libmrss_unpacked

$(LIBMRSS_DIR)/.libmrss_unpacked: $(LIBMRSS_DIR)/.libmrss_downloaded $(LIBMRSS_SOURCE)/$(LIBMRSS).tar.gz
	@echo "Unpacking $(LIBMRSS) ..."
	if [ -d "$(LIBMRSS_SOURCE)/$(LIBMRSS)" ]; then \
		rm -fr $(LIBMRSS_SOURCE)/$(LIBMRSS); \
	fi
	(cd $(LIBMRSS_SOURCE); tar zxvf $(LIBMRSS).tar.gz)
	@echo "Unpacking $(LIBMRSS) done"
	@touch $@


#
# Patch the source
#
.PHONY: libmrss-patched

libmrss-patched: $(LIBMRSS_DIR)/.libmrss_patched

$(LIBMRSS_DIR)/.libmrss_patched: $(LIBMRSS_DIR)/.libmrss_unpacked
	# Patching...
	@echo "Patching $(LIBMRSS) ..."
	@touch $@

#
# config
#
.PHONY: libmrss-config

libmrss-config: 

#
# Configuration
#
.PHONY: libmrss-configured

libmrss-configured:  libmrss-config $(LIBMRSS_DIR)/.libmrss_configured

$(LIBMRSS_DIR)/.libmrss_configured: $(LIBMRSS_DIR)/.libmrss_patched
	@echo "Configuring $(LIBMRSS) ..."
	cd $(LIBMRSS_SOURCE)/$(LIBMRSS); ./configure $(LIBMRSS_CONFIGURE)
	@echo "Configuring $(LIBMRSS) done"
	@touch $@

#
# Compile
#
.PHONY: libmrss-compile

libmrss-compile: $(LIBMRSS_DIR)/.libmrss_compiled

$(LIBMRSS_DIR)/.libmrss_compiled: $(LIBMRSS_DIR)/.libmrss_configured
	@echo "Compiling $(LIBMRSS) ..."
	make -C $(LIBMRSS_SOURCE)/$(LIBMRSS)
	make -C $(LIBMRSS_SOURCE)/$(LIBMRSS) install
	@echo "Compiling $(LIBMRSS) done"
	@touch $@

#
# Update
#
.PHONY: libmrss-update

libmrss-update:
	@echo "Updating $(LIBMRSS) ..."
	@if [ -d "$(LIBMRSS_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBMRSS_SOURCE); \
	fi
	@echo "Updating $(LIBMRSS) done"

#
# Status
#
.PHONY: libmrss-status

libmrss-status:
	@echo "Statusing $(LIBMRSS) ..."
	@if [ -d "$(LIBMRSS_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBMRSS_SOURCE); \
	fi
	@echo "Statusing $(LIBMRSS) done"

#
# Clean
#
.PHONY: libmrss-clean

libmrss-clean:
	@rm -f $(LIBMRSS_DIR)/.libmrss_configured
	@if [ -d "$(LIBMRSS_SOURCE)" ]; then\
		make -C $(LIBMRSS_SOURCE)/$(LIBMRSS) clean; \
	fi

#
# Dist clean
#
.PHONY: libmrss-distclean

libmrss-distclean:
	@rm -f $(LIBMRSS_DIR)/.libmrss*

#
# Install
#
.PHONY: libmrss-install

libmrss-install:

