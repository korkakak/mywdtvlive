##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  wills_yin@alphanetworks.com
# curl makefile fragments 
##############################################################################

#
# Defines
#
ifndef  CONF_CURL_VERSION 
CONF_CURL_VERSION := 7.16.0
endif

CURL             := curl-$(CONF_CURL_VERSION)#
CURL_SERVER_URL  := $(TOP_LIBS_URL)/curl/$(CURL)#
CURL_DIR         := $(TOP_LIBS_DIR)#
CURL_SOURCE      := $(CURL_DIR)/$(CURL)#
CURL_CONFIG      := 

ifeq ($(CONF_PLATFORM_X86), y)
ENABLE_SHLIB = yes
endif

ifeq ($(CONF_OPENSSL), y)
	CURL_CONFIGURE := --prefix=$(LIBS_INSTALL_PATH) --build=$(BUILD_HOST) --host=$(BUILD_TARGET) --enable-shared=$(ENABLE_SHLIB) --without-random --without-libidn --disable-ldap --without-zlib --disable-ipv6 --with-ssl=$(SYSLIB_PREFIX) CFLAGS="$(CFLAGS)" LDFLAGS="$(LD_PATH)"
else
	CURL_CONFIGURE := --prefix=$(LIBS_INSTALL_PATH) --build=$(BUILD_HOST) --host=$(BUILD_TARGET) --enable-shared=$(ENABLE_SHLIB) --without-random --without-libidn --without-ssl CFLAGS="$(CFLAGS)" LDFLAGS="$(LD_PATH)"
endif
#
# Download  the source 
#
.PHONY: curl-downloaded

curl-downloaded: $(CURL_DIR)/.curl_downloaded

$(CURL_DIR)/.curl_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: curl-unpacked

curl-unpacked: $(CURL_DIR)/.curl_unpacked

$(CURL_DIR)/.curl_unpacked: $(CURL_DIR)/.curl_downloaded $(CURL_SOURCE)/$(CURL).tar.bz2
	@echo "Unpacking $(CURL) ..."
	if [ -d "$(CURL_SOURCE)/$(CURL)" ]; then \
		rm -fr $(CURL_SOURCE)/$(CURL); \
	fi
	(cd $(CURL_SOURCE); tar jxvf $(CURL).tar.bz2)
	@echo "Unpacking $(CURL) done"
	@touch $@


#
# Patch the source
#
.PHONY: curl-patched

curl-patched: $(CURL_DIR)/.curl_patched

$(CURL_DIR)/.curl_patched: $(CURL_DIR)/.curl_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: curl-config

curl-config: 

#
# Configuration
#
.PHONY: curl-configured

curl-configured:  curl-config $(CURL_DIR)/.curl_configured

$(CURL_DIR)/.curl_configured: $(CURL_DIR)/.curl_patched $(CURL_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(CURL) ..."
	cd $(CURL_SOURCE)/$(CURL); ./configure $(CURL_CONFIGURE)
	(cd $(CURL_SOURCE)/$(CURL)/lib; sed -i -e "s/-DUNIX//g" Makefile;)
	@echo "Configuring $(CURL) done"
	@touch $@

#
# Compile
#
.PHONY: curl-compile

curl-compile: $(CURL_DIR)/.curl_compiled

$(CURL_DIR)/.curl_compiled: $(CURL_DIR)/.curl_configured
	@echo "Compiling $(CURL) ..."
	make -C $(CURL_SOURCE)/$(CURL)
	make -C $(CURL_SOURCE)/$(CURL) install
	@echo "Compiling $(CURL) done"
	@touch $@

#
# Update
#
.PHONY: curl-update

curl-update:
	@echo "Updating $(CURL) ..."
	@if [ -d "$(CURL_SOURCE)" ]; then\
		$(TOP_UPDATE) $(CURL_SOURCE); \
	fi
	@echo "Updating $(CURL) done"

#
# Status
#
.PHONY: curl-status

curl-status:
	@echo "Statusing $(CURL) ..."
	@if [ -d "$(CURL_SOURCE)" ]; then\
		$(TOP_STATUS) $(CURL_SOURCE); \
	fi
	@echo "Statusing $(CURL) done"

#
# Clean
#
.PHONY: curl-clean

curl-clean:
	@rm -f $(CURL_DIR)/.curl_configured
	@if [ -d "$(CURL_SOURCE)" ]; then\
		make -C $(CURL_SOURCE)/$(CURL) clean; \
	fi

#
# Dist clean
#
.PHONY: curl-distclean

curl-distclean:
	@rm -f $(CURL_DIR)/.curl*

#
# Install
#
.PHONY: curl-install

curl-install:
	cp -f $(LIBS_INSTALL_PATH)/bin/curl $(TOP_INSTALL_ROOTFS_DIR)/bin  
