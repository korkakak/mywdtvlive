#
# Defines
#
LIBGPG-ERROR_VERSION := 1.4#

LIBGPG-ERROR             := libgpg-error-$(LIBGPG-ERROR_VERSION)#
LIBGPG-ERROR_SERVER_URL  := $(TOP_LIBS_URL)/libgpg-error/libgpg-error-$(LIBGPG-ERROR_VERSION)
LIBGPG-ERROR_DIR         := $(TOP_LIBS_DIR)#
LIBGPG-ERROR_SOURCE      := $(LIBGPG-ERROR_DIR)/libgpg-error-$(LIBGPG-ERROR_VERSION)
LIBGPG-ERROR_CONFIG      := 

#
# Download  the source 
#
.PHONY: libgpg-error-downloaded

libgpg-error-downloaded: $(LIBGPG-ERROR_DIR)/.libgpg-error_downloaded

$(LIBGPG-ERROR_DIR)/.libgpg-error_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libgpg-error-unpacked

libgpg-error-unpacked: $(LIBGPG-ERROR_DIR)/.libgpg-error_unpacked

$(LIBGPG-ERROR_DIR)/.libgpg-error_unpacked: $(LIBGPG-ERROR_DIR)/.libgpg-error_downloaded $(wildcard $(LIBGPG-ERROR_SOURCE)/*.tar.bz2) $(wildcard $(LIBGPG-ERROR_SOURCE)/*.patch)
	@echo "Unpacking $(LIBGPG-ERROR) ..."
	if [ -d "$(LIBGPG-ERROR_SOURCE)/$(LIBGPG-ERROR)" ]; then \
		rm -fr $(LIBGPG-ERROR_SOURCE)/$(LIBGPG-ERROR); \
	fi
	cd $(LIBGPG-ERROR_SOURCE); tar xjvf $(LIBGPG-ERROR).tar.bz2
	@echo "Unpacking $(LIBGPG-ERROR) done"
	@touch $@

#
# config
#
.PHONY: libgpg-error-config

libgpg-error-config: $(LIBGPG-ERROR_DIR)/.libgpg-error-config

#
# Patch the source
#
.PHONY: libgpg-error-patched

libgpg-error-patched: $(LIBGPG-ERROR_DIR)/.libgpg-error-config

$(LIBGPG-ERROR_DIR)/.libgpg-error_patched: $(LIBGPG-ERROR_DIR)/.libgpg-error_unpacked 
	@touch $@

#
# Configuration
#
.PHONY: libgpg-error-configured

libgpg-error-configured:  libgpg-error-configured $(LIBGPG-ERROR_DIR)/.libgpg-error_configured

$(LIBGPG-ERROR_DIR)/.libgpg-error_configured: $(LIBGPG-ERROR_DIR)/.libgpg-error_patched $(LIBGPG-ERROR_CONFIG) $(TOP_CURRENT_SET)
	@cd $(LIBGPG-ERROR_SOURCE)/$(LIBGPG-ERROR); \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib" \
		CFLAGS=-I$(LIBS_INSTALL_PATH)/include \
		./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=/opt 
	@echo "Configuring $(LIBGPG-ERROR) done"
	@touch $@

#
# Compile
#
.PHONY: libgpg-error-compile

libgpg-error-compile: $(LIBGPG-ERROR_DIR)/.libgpg-error_compiled

$(LIBGPG-ERROR_DIR)/.libgpg-error_compiled: $(LIBGPG-ERROR_DIR)/.libgpg-error_configured 
	@echo "Compiling $(LIBGPG-ERROR) ..."
	make -C $(LIBGPG-ERROR_SOURCE)/$(LIBGPG-ERROR)
	make -C $(LIBGPG-ERROR_SOURCE)/$(LIBGPG-ERROR) install DESTDIR=$(LIBS_INSTALL_PATH)
	@echo "Compiling $(LIBGPG-ERROR) done"
	@touch $@

#
# Update
#
.PHONY: libgpg-error-update

libgpg-error-update:
	@echo "Updating $(LIBGPG-ERROR) ..."
	@if [ -d "$(LIBGPG-ERROR_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBGPG-ERROR_SOURCE); \
	fi
	@echo "Updating $(LIBGPG-ERROR) done"

#
# Status
#
.PHONY: libgpg-error-status

libgpg-error-status:
	@echo "Statusing $(LIBGPG-ERROR) ..."
	@if [ -d "$(LIBGPG-ERROR_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBGPG-ERROR_SOURCE); \
	fi
	@echo "Statusing $(LIBGPG-ERROR) done"

#
# Clean
#
.PHONY: libgpg-error-clean

libgpg-error-clean:
	@rm -f $(LIBGPG-ERROR_DIR)/.libgpg-error_configured
	@if [ -d "$(LIBGPG-ERROR_SOURCE)" ]; then\
		make -C $(LIBGPG-ERROR_SOURCE)/$(LIBGPG-ERROR) clean; \
	fi

#
# Dist clean
#
.PHONY: libgpg-error-distclean

libgpg-error-distclean:
	@rm -f $(LIBGPG-ERROR_DIR)/.libgpg-error_*

#
# Install
#
.PHONY: libgpg-error-install

libgpg-error-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/lib
	cp -af $(LIBS_INSTALL_PATH)/opt/lib/libgpg-error*so* $(TOP_INSTALL_ROOTFS_DIR)/opt/lib

