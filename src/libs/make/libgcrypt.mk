#
# Defines
#
LIBGCRYPT_VERSION := 1.4.6#

LIBGCRYPT             := libgcrypt-$(LIBGCRYPT_VERSION)#
LIBGCRYPT_SERVER_URL  := $(TOP_LIBS_URL)/libgcrypt/libgcrypt-$(LIBGCRYPT_VERSION)
LIBGCRYPT_DIR         := $(TOP_LIBS_DIR)#
LIBGCRYPT_SOURCE      := $(LIBGCRYPT_DIR)/libgcrypt-$(LIBGCRYPT_VERSION)
LIBGCRYPT_CONFIG      := 

#
# Download  the source 
#
.PHONY: libgcrypt-downloaded

libgcrypt-downloaded: $(LIBGCRYPT_DIR)/.libgcrypt_downloaded

$(LIBGCRYPT_DIR)/.libgcrypt_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libgcrypt-unpacked

libgcrypt-unpacked: $(LIBGCRYPT_DIR)/.libgcrypt_unpacked

$(LIBGCRYPT_DIR)/.libgcrypt_unpacked: $(LIBGCRYPT_DIR)/.libgcrypt_downloaded $(wildcard $(LIBGCRYPT_SOURCE)/*.tar.bz2) $(wildcard $(LIBGCRYPT_SOURCE)/*.patch)
	@echo "Unpacking $(LIBGCRYPT) ..."
	if [ -d "$(LIBGCRYPT_SOURCE)/$(LIBGCRYPT)" ]; then \
		rm -fr $(LIBGCRYPT_SOURCE)/$(LIBGCRYPT); \
	fi
	cd $(LIBGCRYPT_SOURCE); tar xjvf $(LIBGCRYPT).tar.bz2
	@echo "Unpacking $(LIBGCRYPT) done"
	@touch $@

#
# config
#
.PHONY: libgcrypt-config

libgcrypt-config: $(LIBGCRYPT_DIR)/.libgcrypt-config

#
# Patch the source
#
.PHONY: libgcrypt-patched

libgcrypt-patched: $(LIBGCRYPT_DIR)/.libgcrypt-config

$(LIBGCRYPT_DIR)/.libgcrypt_patched: $(LIBGCRYPT_DIR)/.libgcrypt_unpacked 
	@touch $@

#
# Configuration
#
.PHONY: libgcrypt-configured

libgcrypt-configured:  libgcrypt-configured $(LIBGCRYPT_DIR)/.libgcrypt_configured

$(LIBGCRYPT_DIR)/.libgcrypt_configured: $(LIBGCRYPT_DIR)/.libgcrypt_patched $(LIBGCRYPT_CONFIG) $(TOP_CURRENT_SET)
	@cd $(LIBGCRYPT_SOURCE)/$(LIBGCRYPT); \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib " \
		CFLAGS=-I$(LIBS_INSTALL_PATH)/include \
		./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=/opt \
		--with-gpg-error-prefix=$(LIBS_INSTALL_PATH) 
	@echo "Configuring $(LIBGCRYPT) done"
	@touch $@

#
# Compile
#
.PHONY: libgcrypt-compile

libgcrypt-compile: $(LIBGCRYPT_DIR)/.libgcrypt_compiled

$(LIBGCRYPT_DIR)/.libgcrypt_compiled: $(LIBGCRYPT_DIR)/.libgcrypt_configured 
	@echo "Compiling $(LIBGCRYPT) ..."
	make -C $(LIBGCRYPT_SOURCE)/$(LIBGCRYPT)
	make -C $(LIBGCRYPT_SOURCE)/$(LIBGCRYPT) install DESTDIR=$(LIBS_INSTALL_PATH)
	@echo "Compiling $(LIBGCRYPT) done"
	@touch $@

#
# Update
#
.PHONY: libgcrypt-update

libgcrypt-update:
	@echo "Updating $(LIBGCRYPT) ..."
	@if [ -d "$(LIBGCRYPT_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBGCRYPT_SOURCE); \
	fi
	@echo "Updating $(LIBGCRYPT) done"

#
# Status
#
.PHONY: libgcrypt-status

libgcrypt-status:
	@echo "Statusing $(LIBGCRYPT) ..."
	@if [ -d "$(LIBGCRYPT_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBGCRYPT_SOURCE); \
	fi
	@echo "Statusing $(LIBGCRYPT) done"

#
# Clean
#
.PHONY: libgcrypt-clean

libgcrypt-clean:
	@rm -f $(LIBGCRYPT_DIR)/.libgcrypt_configured
	@if [ -d "$(LIBGCRYPT_SOURCE)" ]; then\
		make -C $(LIBGCRYPT_SOURCE)/$(LIBGCRYPT) clean; \
	fi

#
# Dist clean
#
.PHONY: libgcrypt-distclean

libgcrypt-distclean:
	@rm -f $(LIBGCRYPT_DIR)/.libgcrypt_*

#
# Install
#
.PHONY: libgcrypt-install

libgcrypt-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/lib
	cp -af $(LIBS_INSTALL_PATH)/opt/lib/libgcrypt*so* $(TOP_INSTALL_ROOTFS_DIR)/opt/lib

