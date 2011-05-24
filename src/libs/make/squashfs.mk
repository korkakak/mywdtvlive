#
# These library is scratched from squashfs
#

ifndef  SQUASHFS_VERSION 
SQUASHFS_VERSION := 3.4#
endif

SQUASHFS             := squashfs$(SQUASHFS_VERSION)#
SQUASHFS_SERVER_URL  := $(TOP_LIBS_URL)/squashfs/$(SQUASHFS)
SQUASHFS_DIR         := $(TOP_LIBS_DIR)#
SQUASHFS_SOURCE      := $(SQUASHFS_DIR)/$(SQUASHFS)
SQUASHFS_CONFIG      := 

ifeq (x$(CONF_PLATFORM_X86), xy)
SQUASHFS_CONFIGURE := ./config --prefix=$(LIBS_INSTALL_PATH) shared
else
SQUASHFS_CONFIGURE := ./Configure linux-$(ARCH) --prefix=$(LIBS_INSTALL_PATH) shared
endif

export SQUASHFS
#
# Download  the source 
#
.PHONY: squashfs-downloaded

squashfs-downloaded: $(SQUASHFS_DIR)/.squashfs_downloaded

$(SQUASHFS_DIR)/.squashfs_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: squashfs-unpacked

squashfs-unpacked: $(SQUASHFS_DIR)/.squashfs_unpacked

$(SQUASHFS_DIR)/.squashfs_unpacked: $(SQUASHFS_DIR)/.squashfs_downloaded $(wildcard $(SQUASHFS_SOURCE)/*.tar.gz) $(wildcard $(SQUASHFS_SOURCE)/*.patch)
	@echo "Unpacking $(SQUASHFS) ..."
	if [ -d "$(SQUASHFS_SOURCE)/$(SQUASHFS)" ]; then \
		rm -fr $(SQUASHFS_SOURCE)/$(SQUASHFS); \
	fi
	cd $(SQUASHFS_SOURCE);tar xvzf  $(SQUASHFS).tar.gz
	@echo "Unpacking $(SQUASHFS) done"
	@touch $@


#
# Patch the source
#
.PHONY: squashfs-patched

squashfs-patched: $(SQUASHFS_DIR)/.squashfs_patched

$(SQUASHFS_DIR)/.squashfs_patched: $(SQUASHFS_DIR)/.squashfs_unpacked
	@touch $@

#
# config
#
.PHONY: squashfs-config

squashfs-config: 

#
# Configuration
#
.PHONY: squashfs-configured

squashfs-configured:  squashfs-config $(SQUASHFS_DIR)/.squashfs_configured

$(SQUASHFS_DIR)/.squashfs_configured: $(SQUASHFS_DIR)/.squashfs_patched $(SQUASHFS_CONFIG) $(TOP_CURRENT_SET)
	@touch $@

#
# Compile
#
.PHONY: squashfs-compile

squashfs-compile: $(SQUASHFS_DIR)/.squashfs_compiled

$(SQUASHFS_DIR)/.squashfs_compiled: $(SQUASHFS_DIR)/.squashfs_configured
	@echo "Compiling $(SQUASHFS) ..."
	CC=gcc make -C $(SQUASHFS_SOURCE)/$(SQUASHFS)/squashfs-tools
	cp -f $(SQUASHFS_SOURCE)/$(SQUASHFS)/squashfs-tools/mksquashfs $(LIBS_INSTALL_PATH)/bin 
	@echo "Compiling $(SQUASHFS) done"
	@touch $@

#
# Update
#
.PHONY: squashfs-update

squashfs-update:
	@echo "Updating $(SQUASHFS) ..."
	@if [ -d "$(SQUASHFS_SOURCE)" ]; then\
		$(TOP_UPDATE) $(SQUASHFS_SOURCE); \
	fi
	@echo "Updating $(SQUASHFS) done"

#
# Status
#
.PHONY: squashfs-status

squashfs-status:
	@echo "Statusing $(SQUASHFS) ..."
	@if [ -d "$(SQUASHFS_SOURCE)" ]; then\
		$(TOP_STATUS) $(SQUASHFS_SOURCE); \
	fi
	@echo "Statusing $(SQUASHFS) done"

#
# Clean
#
.PHONY: squashfs-clean

squashfs-clean:
	@rm -f $(SQUASHFS_DIR)/.squashfs_configured
	@if [ -d "$(SQUASHFS_SOURCE)" ]; then\
		make -C $(SQUASHFS_SOURCE)/$(SQUASHFS) clean; \
	fi

#
# Dist clean
#
.PHONY: squashfs-distclean

squashfs-distclean:
	@rm -f $(SQUASHFS_DIR)/.squashfs_*

#
# Install
#
.PHONY: squashfs-install

squashfs-install:
