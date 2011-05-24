#
# Defines
#
GMP_VERSION := 	4.3.2#

GMP             := gmp-$(GMP_VERSION)#
GMP_SERVER_URL  := $(TOP_LIBS_URL)/gmp/gmp-$(GMP_VERSION)
GMP_DIR         := $(TOP_LIBS_DIR)#
GMP_SOURCE      := $(GMP_DIR)/gmp-$(GMP_VERSION)
GMP_CONFIG      := 

#
# Download  the source 
#
.PHONY: gmp-downloaded

gmp-downloaded: $(GMP_DIR)/.gmp_downloaded

$(GMP_DIR)/.gmp_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: gmp-unpacked

gmp-unpacked: $(GMP_DIR)/.gmp_unpacked

$(GMP_DIR)/.gmp_unpacked: $(GMP_DIR)/.gmp_downloaded $(wildcard $(GMP_SOURCE)/*.tar.bz2) $(wildcard $(GMP_SOURCE)/*.patch)
	@echo "Unpacking $(GMP) ..."
	if [ -d "$(GMP_SOURCE)/$(GMP)" ]; then \
		rm -fr $(GMP_SOURCE)/$(GMP); \
	fi
	cd $(GMP_SOURCE); tar xjvf $(GMP).tar.bz2
	@echo "Unpacking $(GMP) done"
	@touch $@

#
# config
#
.PHONY: gmp-config

gmp-config: $(GMP_DIR)/.gmp-config

#
# Patch the source
#
.PHONY: gmp-patched

gmp-patched: $(GMP_DIR)/.gmp-config

$(GMP_DIR)/.gmp_patched: $(GMP_DIR)/.gmp_unpacked 
	@touch $@

#
# Configuration
#
.PHONY: gmp-configured

gmp-configured:  gmp-configured $(GMP_DIR)/.gmp_configured

$(GMP_DIR)/.gmp_configured: $(GMP_DIR)/.gmp_patched $(GMP_CONFIG) $(TOP_CURRENT_SET)
	@cd $(GMP_SOURCE)/$(GMP); \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib -liconv" \
		CFLAGS=-I$(LIBS_INSTALL_PATH)/include \
		./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=$(LIBS_INSTALL_PATH) \
		--enable-shared=no 
	@echo "Configuring $(GMP) done"
	@touch $@

#
# Compile
#
.PHONY: gmp-compile

gmp-compile: $(GMP_DIR)/.gmp_compiled

$(GMP_DIR)/.gmp_compiled: $(GMP_DIR)/.gmp_configured 
	@echo "Compiling $(GMP) ..."
	make -C $(GMP_SOURCE)/$(GMP)
	make -C $(GMP_SOURCE)/$(GMP) install
	@echo "Compiling $(GMP) done"
	@touch $@

#
# Update
#
.PHONY: gmp-update

gmp-update:
	@echo "Updating $(GMP) ..."
	@if [ -d "$(GMP_SOURCE)" ]; then\
		$(TOP_UPDATE) $(GMP_SOURCE); \
	fi
	@echo "Updating $(GMP) done"

#
# Status
#
.PHONY: gmp-status

gmp-status:
	@echo "Statusing $(GMP) ..."
	@if [ -d "$(GMP_SOURCE)" ]; then\
		$(TOP_STATUS) $(GMP_SOURCE); \
	fi
	@echo "Statusing $(GMP) done"

#
# Clean
#
.PHONY: gmp-clean

gmp-clean:
	@rm -f $(GMP_DIR)/.gmp_configured
	@if [ -d "$(GMP_SOURCE)" ]; then\
		make -C $(GMP_SOURCE)/$(GMP) clean; \
	fi

#
# Dist clean
#
.PHONY: gmp-distclean

gmp-distclean:
	@rm -f $(GMP_DIR)/.gmp_*

#
# Install
#
.PHONY: gmp-install

gmp-install:

