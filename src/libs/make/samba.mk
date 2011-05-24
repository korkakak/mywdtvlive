##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  sh_yang@alphanetworks.com
# samba makefile fragments 
##############################################################################

#
# Defines
#
ifndef  CONF_SAMBA_VERSION 
SAMBA_VERSION := 3.0.28a
else
SAMBA_VERSION := $(CONF_SAMBA_VERSION)
endif

SAMBA             := samba-$(SAMBA_VERSION)#
SAMBA_SERVER_URL  := $(TOP_LIBS_URL)/samba/$(SAMBA)#b
SAMBA_DIR         := $(TOP_LIBS_DIR)#
SAMBA_SOURCE      := $(SAMBA_DIR)/$(SAMBA)
SAMBA_CONFIG      := 

#ifeq (x$(ENABLE_SHLIB),xyes)
#        SAMBA_SHARED_FLAGS      := --shared
#endif

ifeq ($(SAMBA_VERSION), 3.0.28a)
SOURCE_FOLDER := source
else
SOURCE_FOLDER := source3
endif

#
# Download  the source 
#
.PHONY: samba-downloaded

samba-downloaded: $(SAMBA_DIR)/.samba_downloaded

$(SAMBA_DIR)/.samba_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: samba-unpacked

samba-unpacked: $(SAMBA_DIR)/.samba_unpacked

$(SAMBA_DIR)/.samba_unpacked: $(SAMBA_DIR)/.samba_downloaded $(SAMBA_SOURCE)/$(SAMBA).tar.gz
	@echo "Unpacking $(SAMBA) ..."
	if [ -d "$(SAMBA_SOURCE)/$(SAMBA)" ]; then \
		rm -fr $(SAMBA_SOURCE)/$(SAMBA); \
	fi
	cd $(SAMBA_SOURCE); tar xvzf  $(SAMBA).tar.gz
	@echo "Unpacking $(SAMBA) done"
	@touch $@


#
# Patch the source
#
.PHONY: samba-patched

samba-patched: $(SAMBA_DIR)/.samba_patched

$(SAMBA_DIR)/.samba_patched: $(SAMBA_DIR)/.samba_unpacked
	# Patching...
# Patching...
	if [ -f $(SAMBA_SOURCE)/Bugzilla_Bug_7577_SPNEGO_auth_fails_when_contacting_Win7_system_using_Microsoft_Live_Sign-in_Assistant.patch ]; then \
		cd $(SAMBA_SOURCE)/$(SAMBA); patch -p1 < ../Bugzilla_Bug_7577_SPNEGO_auth_fails_when_contacting_Win7_system_using_Microsoft_Live_Sign-in_Assistant.patch;\
	fi
	cd $(SAMBA_SOURCE)/$(SAMBA); patch -p1 < ../$(SAMBA).patch
	if [ -f $(SAMBA_SOURCE)/ftruncate.patch ]; then \
		cd $(SAMBA_SOURCE)/$(SAMBA); patch -p2 < ../ftruncate.patch ;\
	fi
	@touch $@

#
# config
#
.PHONY: samba-config

samba-config: 

#
# Configuration
#
.PHONY: samba-configured

samba-configured:  samba-config $(SAMBA_DIR)/.samba_configured

$(SAMBA_DIR)/.samba_configured: $(SAMBA_DIR)/.samba_patched $(SAMBA_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(SAMBA) ..."
	cd $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER);./configure \
		--prefix=$(LIBS_INSTALL_PATH) \
		--host=$(BUILD_TARGET) \
		--disable-cups \
		$(SAMBA_SHARED_FLAGS) \
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
		--with-winbind=no \
		samba_cv_CC_NEGATIVE_ENUM_VALUES=yes
	#cd $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER);./configure --prefix=$(LIBS_INSTALL_PATH) --host=$(BUILD_TARGET) --disable-cups $(SAMBA_SHARED_FLAGS)
	@echo "Configuring $(SAMBA) done"
	@touch $@

#
# Compile
#
.PHONY: samba-compile

samba-compile: $(SAMBA_DIR)/.samba_compiled

$(SAMBA_DIR)/.samba_compiled: $(SAMBA_DIR)/.samba_configured
	@echo "Compiling $(SAMBA) ..."
	make -C $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER) 
	make -C $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER)  install
	@echo "Compiling $(SAMBA) done"
	@touch $@

#
# Update
#
.PHONY: samba-update

samba-update:
	@echo "Updating $(SAMBA) ..."
	@if [ -d "$(SAMBA_SOURCE)" ]; then\
		$(TOP_UPDATE) $(SAMBA_SOURCE); \
	fi
	@echo "Updating $(SAMBA) done"

#
# Status
#
.PHONY: samba-status

samba-status:
	@echo "Statusing $(SAMBA) ..."
	@if [ -d "$(SAMBA_SOURCE)" ]; then\
		$(TOP_STATUS) $(SAMBA_SOURCE); \
	fi
	@echo "Statusing $(SAMBA) done"

#
# Clean
#
.PHONY: samba-clean

samba-clean:
	@rm -f $(SAMBA_DIR)/.samba_configured
	@if [ -d "$(SAMBA_SOURCE)" ]; then\
		make -C $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER) clean; \
	fi

#
# Dist clean
#
.PHONY: samba-distclean

samba-distclean:
	@rm -f $(SAMBA_DIR)/.samba_*

#
# Install
#
.PHONY: samba-install

samba-install:
	@echo $(TOP_BUILD_ROOTFS_DIR)
	#cp -f $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER)/bin/net $(TOP_INSTALL_ROOTFS_DIR)/bin
	cp -f $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER)/bin/nmblookup $(TOP_INSTALL_ROOTFS_DIR)/bin
	cp -f $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER)/bin/smbclient $(TOP_INSTALL_ROOTFS_DIR)/usr/sbin/
ifeq (x$(CONF_SAMBA_SERVER), xy)
	mkdir -p ${TOP_BUILD_ROOTFS_DIR}/usr/sbin/
	cp -f $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER)/bin/smbd $(TOP_INSTALL_ROOTFS_DIR)/usr/sbin/
	cp -f $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER)/bin/nmbd $(TOP_INSTALL_ROOTFS_DIR)/usr/sbin/
ifeq (x$(CONF_SAMBA_SERVER_SUPPORT_SECURITY), xy)
	cp -f $(SAMBA_SOURCE)/$(SAMBA)/$(SOURCE_FOLDER)/bin/smbpasswd $(TOP_INSTALL_ROOTFS_DIR)/bin
endif
endif
