##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  carey_chou@alphanetworks.com
# libmms makefile fragments 
##############################################################################

#
# Defines
#
ifndef  CONF_LIBMMS_VERSION 
CONF_LIBMMS_VERSION := 0.5-radiotime
endif

LIBMMS             := libmms-$(CONF_LIBMMS_VERSION)#
LIBMMS_SERVER_URL  := $(TOP_LIBS_URL)/libmms/$(LIBMMS)
LIBMMS_DIR         := $(TOP_LIBS_DIR)#
LIBMMS_SOURCE      := $(LIBMMS_DIR)/$(LIBMMS)
LIBMMS_CONFIG      := 

#
# Download  the source 
#
.PHONY: libmms-downloaded

libmms-downloaded: $(LIBMMS_DIR)/.libmms_downloaded

$(LIBMMS_DIR)/.libmms_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libmms-unpacked

libmms-unpacked: $(LIBMMS_DIR)/.libmms_unpacked

$(LIBMMS_DIR)/.libmms_unpacked: $(LIBMMS_DIR)/.libmms_downloaded $(LIBMMS_SOURCE)/$(LIBMMS).tgz
	@echo "Unpacking $(LIBMMS) ..."
	if [ -d "$(LIBMMS_SOURCE)/$(LIBMMS)" ]; then \
		rm -fr $(LIBMMS_SOURCE)/$(LIBMMS); \
	fi
	cd $(LIBMMS_SOURCE); tar zxvf $(LIBMMS).tgz
	@echo "Unpacking $(LIBMMS) done"
	@touch $@


#
# Patch the source
#
.PHONY: libmms-patched

libmms-patched: $(LIBMMS_DIR)/.libmms_patched

$(LIBMMS_DIR)/.libmms_patched: $(LIBMMS_DIR)/.libmms_unpacked
	# Patching...
	@echo "Patching $(LIBMMS) ..."
	-cd $(LIBMMS_SOURCE); patch -p0 < $(LIBMMS).patch
	@echo "Patching $(LIBMMS) done"
	@touch $@

#
# config
#
.PHONY: libmms-config

libmms-config: 

#
# Configuration
#
.PHONY: libmms-configured

libmms-configured:  libmms-config $(LIBMMS_DIR)/.libmms_configured

$(LIBMMS_DIR)/.libmms_configured: $(LIBMMS_DIR)/.libmms_patched $(LIBMMS_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(LIBMMS) ..."
       	
	cd $(LIBMMS_SOURCE)/$(LIBMMS);./configure  --prefix=$(LIBS_INSTALL_PATH)  --build=$(BUILD_HOST) --host=$(BUILD_TARGET) --target=$(BUILD_TARGET) LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib/" --enable-static=yes --enable-shared=no
	@echo "Configuring $(LIBMMS) done"
	@touch $@

#
# Compile
#
.PHONY: libmms-compile

libmms-compile: $(LIBMMS_DIR)/.libmms_compiled

$(LIBMMS_DIR)/.libmms_compiled: $(LIBMMS_DIR)/.libmms_configured
	@echo "Compiling $(LIBMMS) ..."
	make -C $(LIBMMS_SOURCE)/$(LIBMMS) 
	make -C $(LIBMMS_SOURCE)/$(LIBMMS)  install
	@echo "Compiling $(LIBMMS) done"
	@touch $@

#
# Update
#
.PHONY: libmms-update

libmms-update:
	@echo "Updating $(LIBMMS) ..."
	@if [ -d "$(LIBMMS_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBMMS_SOURCE); \
	fi
	@echo "Updating $(LIBMMS) done"

#
# Status
#
.PHONY: libmms-status

libmms-status:
	@echo "Statusing $(LIBMMS) ..."
	@if [ -d "$(LIBMMS_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBMMS_SOURCE); \
	fi
	@echo "Statusing $(LIBMMS) done"

#
# Clean
#
.PHONY: libmms-clean

libmms-clean:
	@rm -f $(LIBMMS_DIR)/.libmms_configured
	@if [ -d "$(LIBMMS_SOURCE)" ]; then\
		make -C $(LIBMMS_SOURCE)/$(LIBMMS) clean; \
	fi

#
# Dist clean
#
.PHONY: libmms-distclean

libmms-distclean:
	@rm -f $(LIBMMS_DIR)/.libmms_*

#
# Install
#
.PHONY: libmms-install

libmms-install:
	





