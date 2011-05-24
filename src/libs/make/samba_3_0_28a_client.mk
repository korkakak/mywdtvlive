##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  sh_yang@alphanetworks.com
# samba_3_0_28a_client makefile fragments 
##############################################################################

#
# Defines
#
SAMBA_CLIENT_VERSION := 3.0.28a

SAMBA_CLIENT             := samba-$(SAMBA_CLIENT_VERSION)#
SAMBA_CLIENT_SERVER_URL  := $(TOP_LIBS_URL)/samba/$(SAMBA_CLIENT)#b
SAMBA_CLIENT_DIR         := $(TOP_LIBS_DIR)#
SAMBA_CLIENT_SOURCE      := $(SAMBA_CLIENT_DIR)/$(SAMBA_CLIENT)
SAMBA_CLIENT_CONFIG      := 

#ifeq (x$(ENABLE_SHLIB),xyes)
#        SAMBA_CLIENT_SHARED_FLAGS      := --shared
#endif

SAMBA_CLIENT_SOURCE_FOLDER := source

#
# Download  the source 
#
.PHONY: samba_3_0_28a_client-downloaded

samba_3_0_28a_client-downloaded: $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_downloaded

$(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: samba_3_0_28a_client-unpacked

samba_3_0_28a_client-unpacked: $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_unpacked

$(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_unpacked: $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_downloaded $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT).tar.gz
	@echo "Unpacking $(SAMBA_CLIENT) ..."
	if [ -d "$(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)" ]; then \
		rm -fr $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT); \
	fi
	cd $(SAMBA_CLIENT_SOURCE); tar xvzf  $(SAMBA_CLIENT).tar.gz
	@echo "Unpacking $(SAMBA_CLIENT) done"
	@touch $@


#
# Patch the source
#
.PHONY: samba_3_0_28a_client-patched

samba_3_0_28a_client-patched: $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_patched

$(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_patched: $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_unpacked
	# Patching...
# Patching...
	cd $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT); patch -p1 < ../$(SAMBA_CLIENT).patch
	if [ x$(CONF_SAMBA_CLIENT_FTRUNCATE_PATCH) == xy ]; then \
		cd $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT); patch -p2 < ../ftruncate.patch ;\
	fi
	@touch $@

#
# config
#
.PHONY: samba_3_0_28a_client-config

samba_3_0_28a_client-config: 

#
# Configuration
#
.PHONY: samba_3_0_28a_client-configured

samba_3_0_28a_client-configured:  samba_3_0_28a_client-config $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_configured

$(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_configured: $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_patched $(SAMBA_CLIENT_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(SAMBA_CLIENT) ..."
	cd $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)/$(SAMBA_CLIENT_SOURCE_FOLDER);./configure \
		--prefix=$(LIBS_INSTALL_PATH) \
		--host=$(BUILD_TARGET) \
		--disable-cups \
		$(SAMBA_CLIENT_SHARED_FLAGS) \
		--with-ldap=no \
		--with-ads=no \
		--enable-shared=no \
		--enable-swat=no \
		--enable-pie=no \
		--enable-relro=no \
		--enable-shared-libs \
		--enable-fam=no \
		--enable-avahi=no \
		--enable-gnutls=no \
		--enable-netapi=no \
		--with-acl-support=no \
		--with-wbclient=no \
		--with-winbind=no
	#cd $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)/$(SAMBA_CLIENT_SOURCE_FOLDER);./configure --prefix=$(LIBS_INSTALL_PATH) --host=$(BUILD_TARGET) --disable-cups $(SAMBA_CLIENT_SHARED_FLAGS)
	@echo "Configuring $(SAMBA_CLIENT) done"
	@touch $@

#
# Compile
#
.PHONY: samba_3_0_28a_client-compile

samba_3_0_28a_client-compile: $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_compiled

$(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_compiled: $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_configured
	@echo "Compiling $(SAMBA_CLIENT) ..."
	make -C $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)/$(SAMBA_CLIENT_SOURCE_FOLDER) 
	make -C $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)/$(SAMBA_CLIENT_SOURCE_FOLDER)  install
	@echo "Compiling $(SAMBA_CLIENT) done"
	@touch $@

#
# Update
#
.PHONY: samba_3_0_28a_client-update

samba_3_0_28a_client-update:
	@echo "Updating $(SAMBA_CLIENT) ..."
	@if [ -d "$(SAMBA_CLIENT_SOURCE)" ]; then\
		$(TOP_UPDATE) $(SAMBA_CLIENT_SOURCE); \
	fi
	@echo "Updating $(SAMBA_CLIENT) done"

#
# Status
#
.PHONY: samba_3_0_28a_client-status

samba_3_0_28a_client-status:
	@echo "Statusing $(SAMBA_CLIENT) ..."
	@if [ -d "$(SAMBA_CLIENT_SOURCE)" ]; then\
		$(TOP_STATUS) $(SAMBA_CLIENT_SOURCE); \
	fi
	@echo "Statusing $(SAMBA_CLIENT) done"

#
# Clean
#
.PHONY: samba_3_0_28a_client-clean

samba_3_0_28a_client-clean:
	@rm -f $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_configured
	@if [ -d "$(SAMBA_CLIENT_SOURCE)" ]; then\
		make -C $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)/$(SAMBA_CLIENT_SOURCE_FOLDER) clean; \
	fi

#
# Dist clean
#
.PHONY: samba_3_0_28a_client-distclean

samba_3_0_28a_client-distclean:
	@rm -f $(SAMBA_CLIENT_DIR)/.samba_3_0_28a_client_*

#
# Install
#
.PHONY: samba_3_0_28a_client-install

samba_3_0_28a_client-install:
	@echo $(TOP_BUILD_ROOTFS_DIR)
	#cp -f $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)/$(SAMBA_CLIENT_SOURCE_FOLDER)/bin/net $(TOP_INSTALL_ROOTFS_DIR)/bin
	cp -f $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)/$(SAMBA_CLIENT_SOURCE_FOLDER)/bin/nmblookup $(TOP_INSTALL_ROOTFS_DIR)/bin
	cp -f $(SAMBA_CLIENT_SOURCE)/$(SAMBA_CLIENT)/$(SAMBA_CLIENT_SOURCE_FOLDER)/bin/smbclient $(TOP_INSTALL_ROOTFS_DIR)/usr/sbin/
