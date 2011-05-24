#
# Defines
#
LIBFFI_VERSION := 3.0.8#

LIBFFI             := libffi-$(LIBFFI_VERSION)#
LIBFFI_SERVER_URL  := $(TOP_LIBS_URL)/libffi/$(LIBFFI)
LIBFFI_DIR         := $(TOP_LIBS_DIR)#
LIBFFI_SOURCE      := $(LIBFFI_DIR)/$(LIBFFI)
LIBFFI_CONFIG      := 

LIBFFI_PREFIX	:= opt#

LIBFFI_ENABLE_SHLIB	:= no
ifeq (x$(LIBFFI_ENABLE_SHLIB),xno)
	LIBFFI_SHARED_FLAGS      := --enable-shared=no
endif

#
# Download  the source 
#
.PHONY: libffi-downloaded

libffi-downloaded: $(LIBFFI_DIR)/.libffi_downloaded

$(LIBFFI_DIR)/.libffi_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libffi-unpacked

libffi-unpacked: $(LIBFFI_DIR)/.libffi_unpacked

$(LIBFFI_DIR)/.libffi_unpacked: $(LIBFFI_DIR)/.libffi_downloaded $(wildcard $(LIBFFI_SOURCE)/*.tgz)
	echo "Unpacking $(LIBFFI) ..."
	if [ -d "$(LIBFFI_SOURCE)/$(LIBFFI)" ]; then \
		rm -fr $(LIBFFI_SOURCE)/$(LIBFFI); \
	fi
	cd $(LIBFFI_SOURCE); tar zxvf $(LIBFFI).tar.gz
	@echo "Unpacking $(LIBFFI) done"
	@touch $@

#
# Patch the source
#
.PHONY: libffi-patched

libffi-patched: $(LIBFFI_DIR)/.libffi-patched

$(LIBFFI_DIR)/.libffi_patched: $(LIBFFI_DIR)/.libffi_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: readline-config

readline-config: 


#
# Configuration
#
.PHONY: libffi-configured

libffi-configured:  libffi-configured $(LIBFFI_DIR)/.libffi_configured

$(LIBFFI_DIR)/.libffi_configured: $(LIBFFI_DIR)/.libffi_patched $(LIBFFI_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(LIBFFI) ..."
	cd $(LIBFFI_SOURCE)/$(LIBFFI); \
		CC=mipsel-linux-gcc \
		CXX=mipsel-linux-g++ \
		AR=mipsel-linux-ar \
		RANLIB=mipsel-linux-ranlib \
	   	CFLAGS="-I$(LIBS_INSTALL_PATH)/include " \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib" \
		./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=$(LIBS_INSTALL_PATH)/$(LIBFFI_PREFIX) \
	echo "Configuring $(LIBFFI) done"
	@touch $@

#
# Compile
#
.PHONY: libffi-compile

libffi-compile: $(LIBFFI_DIR)/.libffi_compiled

$(LIBFFI_DIR)/.libffi_compiled: $(LIBFFI_DIR)/.libffi_configured 
	@echo "Compiling $(LIBFFI) ..."
	@cd $(LIBFFI_SOURCE)/$(LIBFFI); \
	   	CFLAGS="-I$(LIBS_INSTALL_PATH)/include " \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib " \
		CC=mipsel-linux-gcc \
		CXX=mipsel-linux-g++ \
		AR=mipsel-linux-ar \
		RANLIB=mipsel-linux-ranlib \
		THIRD_PARTY_LIB=$(LIBS_INSTALL_PATH)/lib \
		THIRD_PARTY_INCLUDE=$(THIRD_PARTY_INCLUDE)/include \
		make
	echo "Compiling $(LIBFFI) done"
	@cd $(LIBFFI_SOURCE)/$(LIBFFI); \
		THIRD_PARTY_LIB=$(LIBS_INSTALL_PATH)/lib \
		THIRD_PARTY_INCLUDE=$(THIRD_PARTY_INCLUDE)/include \
		make install prefix=$(LIBS_INSTALL_PATH)/$(LIBFFI_PREFIX)
	echo "installing $(LIBFFI) done"
	@touch $@

#
# Update
#
.PHONY: libffi-update

libffi-update:
	@echo "Updating $(LIBFFI) ..."
	@if [ -d "$(LIBFFI_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBFFI_SOURCE); \
	fi
	@echo "Updating $(LIBFFI) done"

#
# Status
#
.PHONY: libffi-status

libffi-status:
	@echo "Statusing $(LIBFFI) ..."
	@if [ -d "$(LIBFFI_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBFFI_SOURCE); \
	fi
	@echo "Statusing $(LIBFFI) done"

#
# Clean
#
.PHONY: libffi-clean

libffi-clean:
	@rm -f $(LIBFFI_DIR)/.libffi_configured
	@if [ -d "$(LIBFFI_SOURCE)" ]; then\
		make -C $(LIBFFI_SOURCE)/$(LIBFFI) clean; \
	fi

#
# Dist clean
#
.PHONY: libffi-distclean

libffi-distclean:
	@rm -f $(LIBFFI_DIR)/.libffi_*

#
# Install
#
.PHONY: libffi-install

libffi-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/lib
	cp -af $(LIBS_INSTALL_PATH)/$(LIBFFI_PREFIX)/lib/libffi*so* $(TOP_INSTALL_ROOTFS_DIR)/opt/lib

	
