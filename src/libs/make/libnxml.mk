##############################################################################
# Copyright (C) alphanetworks 2011-- 
# All Rights Reserved -- Company Confidential
# Author:  ivan_yen@alphanetworks.com
# libnxml makefile fragments 
##############################################################################

#
# Defines
#
ifndef  CONF_LIBNXML_VERSION 
CONF_LIBNXML_VERSION := 0.18.3
endif

LIBNXML             := libnxml-$(CONF_LIBNXML_VERSION)#
LIBNXML_SERVER_URL  := $(TOP_LIBS_URL)/libnxml/$(LIBNXML)#
LIBNXML_DIR         := $(TOP_LIBS_DIR)#
LIBNXML_SOURCE      := $(LIBNXML_DIR)/$(LIBNXML)#
LIBNXML_CONFIG      := 

LD_PATH := -L$(LIBS_INSTALL_PATH)/lib -Wl,-rpath-link -Wl,$(SYSLIB_PREFIX)/lib
CFLAGS := -I$(LIBS_INSTALL_PATH)/include

ifeq ($(CONF_PLATFORM_X86), y)
ENABLE_SHLIB = yes
endif

LIBNXML_CONFIGURE := --prefix=$(LIBS_INSTALL_PATH) --build=$(BUILD_HOST) --host=$(BUILD_TARGET) --enable-shared=$(ENABLE_SHLIB) CFLAGS="$(CFLAGS)" LDFLAGS="$(LD_PATH)"

#
# Download  the source 
#
.PHONY: libnxml-downloaded

libnxml-downloaded: $(LIBNXML_DIR)/.libnxml_downloaded

$(LIBNXML_DIR)/.libnxml_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libnxml-unpacked

libnxml-unpacked: $(LIBNXML_DIR)/.libnxml_unpacked

$(LIBNXML_DIR)/.libnxml_unpacked: $(LIBNXML_DIR)/.libnxml_downloaded $(LIBNXML_SOURCE)/$(LIBNXML).tar.gz
	@echo "Unpacking $(LIBNXML) ..."
	if [ -d "$(LIBNXML_SOURCE)/$(LIBNXML)" ]; then \
		rm -fr $(LIBNXML_SOURCE)/$(LIBNXML); \
	fi
	(cd $(LIBNXML_SOURCE); tar zxvf $(LIBNXML).tar.gz)
	@echo "Unpacking $(LIBNXML) done"
	@touch $@


#
# Patch the source
#
.PHONY: libnxml-patched

libnxml-patched: $(LIBNXML_DIR)/.libnxml_patched

$(LIBNXML_DIR)/.libnxml_patched: $(LIBNXML_DIR)/.libnxml_unpacked
	# Patching...
	@echo "Patching $(LIBNXML) ..."
	@touch $@

#
# config
#
.PHONY: libnxml-config

libnxml-config: 

#
# Configuration
#
.PHONY: libnxml-configured

libnxml-configured:  libnxml-config $(LIBNXML_DIR)/.libnxml_configured

$(LIBNXML_DIR)/.libnxml_configured: $(LIBNXML_DIR)/.libnxml_patched
	@echo "Configuring $(LIBNXML) ..."
	cd $(LIBNXML_SOURCE)/$(LIBNXML); ./configure $(LIBNXML_CONFIGURE)
	@echo "Configuring $(LIBNXML) done"
	@touch $@

#
# Compile
#
.PHONY: libnxml-compile

libnxml-compile: $(LIBNXML_DIR)/.libnxml_compiled

$(LIBNXML_DIR)/.libnxml_compiled: $(LIBNXML_DIR)/.libnxml_configured
	@echo "Compiling $(LIBNXML) ..."
	make -C $(LIBNXML_SOURCE)/$(LIBNXML)
	make -C $(LIBNXML_SOURCE)/$(LIBNXML) install
	@echo "Compiling $(LIBNXML) done"
	@touch $@

#
# Update
#
.PHONY: libnxml-update

libnxml-update:
	@echo "Updating $(LIBNXML) ..."
	@if [ -d "$(LIBNXML_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBNXML_SOURCE); \
	fi
	@echo "Updating $(LIBNXML) done"

#
# Status
#
.PHONY: libnxml-status

libnxml-status:
	@echo "Statusing $(LIBNXML) ..."
	@if [ -d "$(LIBNXML_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBNXML_SOURCE); \
	fi
	@echo "Statusing $(LIBNXML) done"

#
# Clean
#
.PHONY: libnxml-clean

libnxml-clean:
	@rm -f $(LIBNXML_DIR)/.libnxml_configured
	@if [ -d "$(LIBNXML_SOURCE)" ]; then\
		make -C $(LIBNXML_SOURCE)/$(LIBNXML) clean; \
	fi

#
# Dist clean
#
.PHONY: libnxml-distclean

libnxml-distclean:
	@rm -f $(LIBNXML_DIR)/.libnxml*

#
# Install
#
.PHONY: libnxml-install

libnxml-install:

