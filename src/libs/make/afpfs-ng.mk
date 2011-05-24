#
# Defines
#
AFPFS-NG_VERSION := 	0.8.1#

AFPFS-NG             := afpfs-ng-$(AFPFS-NG_VERSION)#
AFPFS-NG_SERVER_URL  := $(TOP_LIBS_URL)/afpfs-ng/afpfs-ng-$(AFPFS-NG_VERSION)
AFPFS-NG_DIR         := $(TOP_LIBS_DIR)#
AFPFS-NG_SOURCE      := $(AFPFS-NG_DIR)/afpfs-ng-$(AFPFS-NG_VERSION)
AFPFS-NG_CONFIG      := 

#
# Download  the source 
#
.PHONY: afpfs-ng-downloaded

afpfs-ng-downloaded: $(AFPFS-NG_DIR)/.afpfs-ng_downloaded

$(AFPFS-NG_DIR)/.afpfs-ng_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: afpfs-ng-unpacked

afpfs-ng-unpacked: $(AFPFS-NG_DIR)/.afpfs-ng_unpacked

$(AFPFS-NG_DIR)/.afpfs-ng_unpacked: $(AFPFS-NG_DIR)/.afpfs-ng_downloaded $(wildcard $(AFPFS-NG_SOURCE)/*.tar.bz2) $(wildcard $(AFPFS-NG_SOURCE)/*.patch)
	@echo "Unpacking $(AFPFS-NG) ..."
	if [ -d "$(AFPFS-NG_SOURCE)/$(AFPFS-NG)" ]; then \
		rm -fr $(AFPFS-NG_SOURCE)/$(AFPFS-NG); \
	fi
	cd $(AFPFS-NG_SOURCE); tar xjvf $(AFPFS-NG).tar.bz2
	@echo "Unpacking $(AFPFS-NG) done"
	@touch $@

#
# config
#
.PHONY: afpfs-ng-config

afpfs-ng-config: $(AFPFS-NG_DIR)/.afpfs-ng-config

#
# Patch the source
#
.PHONY: afpfs-ng-patched

afpfs-ng-patched: $(AFPFS-NG_DIR)/.afpfs-ng-config

$(AFPFS-NG_DIR)/.afpfs-ng_patched: $(AFPFS-NG_DIR)/.afpfs-ng_unpacked 
	@touch $@

#
# Configuration
#
.PHONY: afpfs-ng-configured

afpfs-ng-configured:  afpfs-ng-configured $(AFPFS-NG_DIR)/.afpfs-ng_configured

$(AFPFS-NG_DIR)/.afpfs-ng_configured: $(AFPFS-NG_DIR)/.afpfs-ng_patched $(AFPFS-NG_CONFIG) $(TOP_CURRENT_SET)
	@cd $(AFPFS-NG_SOURCE)/$(AFPFS-NG); \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib -liconv -lgpg-error" \
		CFLAGS=-I$(LIBS_INSTALL_PATH)/include \
		./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=/opt \
		--enable-shared=no \
		ac_cv_func_malloc_0_nonnull=yes
	@echo "Configuring $(AFPFS-NG) done"
	@touch $@

#
# Compile
#
.PHONY: afpfs-ng-compile

afpfs-ng-compile: $(AFPFS-NG_DIR)/.afpfs-ng_compiled

$(AFPFS-NG_DIR)/.afpfs-ng_compiled: $(AFPFS-NG_DIR)/.afpfs-ng_configured 
	@echo "Compiling $(AFPFS-NG) ..."
	make -C $(AFPFS-NG_SOURCE)/$(AFPFS-NG)/lib 
	make -C $(AFPFS-NG_SOURCE)/$(AFPFS-NG)/fuse 
	make -C $(AFPFS-NG_SOURCE)/$(AFPFS-NG)/lib install DESTDIR=$(LIBS_INSTALL_PATH)
	make -C $(AFPFS-NG_SOURCE)/$(AFPFS-NG)/fuse install DESTDIR=$(LIBS_INSTALL_PATH)
	@echo "Compiling $(AFPFS-NG) done"
	@touch $@

#
# Update
#
.PHONY: afpfs-ng-update

afpfs-ng-update:
	@echo "Updating $(AFPFS-NG) ..."
	@if [ -d "$(AFPFS-NG_SOURCE)" ]; then\
		$(TOP_UPDATE) $(AFPFS-NG_SOURCE); \
	fi
	@echo "Updating $(AFPFS-NG) done"

#
# Status
#
.PHONY: afpfs-ng-status

afpfs-ng-status:
	@echo "Statusing $(AFPFS-NG) ..."
	@if [ -d "$(AFPFS-NG_SOURCE)" ]; then\
		$(TOP_STATUS) $(AFPFS-NG_SOURCE); \
	fi
	@echo "Statusing $(AFPFS-NG) done"

#
# Clean
#
.PHONY: afpfs-ng-clean

afpfs-ng-clean:
	@rm -f $(AFPFS-NG_DIR)/.afpfs-ng_configured
	@if [ -d "$(AFPFS-NG_SOURCE)" ]; then\
		make -C $(AFPFS-NG_SOURCE)/$(AFPFS-NG) clean; \
	fi

#
# Dist clean
#
.PHONY: afpfs-ng-distclean

afpfs-ng-distclean:
	@rm -f $(AFPFS-NG_DIR)/.afpfs-ng_*

#
# Install
#
.PHONY: afpfs-ng-install

afpfs-ng-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/bin
	cp -af $(LIBS_INSTALL_PATH)/opt/bin/afpfsd $(TOP_INSTALL_ROOTFS_DIR)/opt/bin
	cp -af $(LIBS_INSTALL_PATH)/opt/bin/mount_afp $(TOP_INSTALL_ROOTFS_DIR)/opt/bin

