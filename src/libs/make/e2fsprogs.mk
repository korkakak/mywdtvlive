#
# Defines
#
ifndef  CONF_E2FSPROGS_VERSION 
CONF_E2FSPROGS_VERSION := 1.41.12#
endif

E2FSPROGS             := e2fsprogs-$(CONF_E2FSPROGS_VERSION)#
E2FSPROGS_SERVER_URL  := $(TOP_LIBS_URL)/e2fsprogs/$(E2FSPROGS)#
E2FSPROGS_DIR         := $(TOP_LIBS_DIR)#
E2FSPROGS_SOURCE      := $(E2FSPROGS_DIR)/$(E2FSPROGS)#
E2FSPROGS_CONFIG      := 


# Download  the source 
#
.PHONY: e2fsprogs-downloaded

e2fsprogs-downloaded: $(E2FSPROGS_DIR)/.e2fsprogs_downloaded

$(E2FSPROGS_DIR)/.e2fsprogs_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: e2fsprogs-unpacked

e2fsprogs-unpacked: $(E2FSPROGS_DIR)/.e2fsprogs_unpacked

$(E2FSPROGS_DIR)/.e2fsprogs_unpacked: $(E2FSPROGS_DIR)/.e2fsprogs_downloaded $(E2FSPROGS_SOURCE)/$(E2FSPROGS).tar.gz 
	@echo "Unpacking $(E2FSPROGS) ..."
	if [ -d "$(E2FSPROGS_SOURCE)/$(E2FSPROGS)" ]; then \
		rm -fr $(E2FSPROGS_SOURCE)/$(E2FSPROGS); \
	fi
	(cd $(E2FSPROGS_SOURCE); tar xvf $(E2FSPROGS).tar.gz)
	@echo "Unpacking $(E2FSPROGS) done"
	@touch $@


#
# Patch the source
#
.PHONY: e2fsprogs-patched

e2fsprogs-patched: $(E2FSPROGS_DIR)/.e2fsprogs_patched

$(E2FSPROGS_DIR)/.e2fsprogs_patched: $(E2FSPROGS_DIR)/.e2fsprogs_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: e2fsprogs-config

e2fsprogs-config: 

#
# Configuration
#
.PHONY: e2fsprogs-configured

e2fsprogs-configured:  e2fsprogs-config $(E2FSPROGS_DIR)/.e2fsprogs_configured

$(E2FSPROGS_DIR)/.e2fsprogs_configured: $(E2FSPROGS_DIR)/.e2fsprogs_patched $(E2FSPROGS_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(E2FSPROGS) ..."
	cd $(E2FSPROGS_SOURCE)/$(E2FSPROGS); ./configure --prefix=$(LIBS_INSTALL_PATH) --build=$(BUILD_HOST) --target=$(BUILD_TARGET) --host=$(BUILD_TARGET)
	@echo "Configuring $(E2FSPROGS) done"
	@touch $@

#
# Compile
#
.PHONY: e2fsprogs-compile

e2fsprogs-compile: $(E2FSPROGS_DIR)/.e2fsprogs_compiled

$(E2FSPROGS_DIR)/.e2fsprogs_compiled: $(E2FSPROGS_DIR)/.e2fsprogs_configured
	@echo "Compiling $(E2FSPROGS) ..."
	make -C $(E2FSPROGS_SOURCE)/$(E2FSPROGS)
	@echo "Compiling $(E2FSPROGS) done"
	@touch $@

#
# Update
#
.PHONY: e2fsprogs-update

e2fsprogs-update:
	@echo "Updating $(E2FSPROGS) ..."
	@if [ -d "$(E2FSPROGS_SOURCE)" ]; then\
		$(TOP_UPDATE) $(E2FSPROGS_SOURCE); \
	fi
	@echo "Updating $(E2FSPROGS) done"

#
# Status
#
.PHONY: e2fsprogs-status

e2fsprogs-status:
	@echo "Statusing $(E2FSPROGS) ..."
	@if [ -d "$(E2FSPROGS_SOURCE)" ]; then\
		$(TOP_STATUS) $(E2FSPROGS_SOURCE); \
	fi
	@echo "Statusing $(E2FSPROGS) done"

#
# Clean
#
.PHONY: e2fsprogs-clean

e2fsprogs-clean:
	@rm -f $(E2FSPROGS_DIR)/.e2fsprogs_configured
	@if [ -d "$(E2FSPROGS_SOURCE)" ]; then\
		make -C $(E2FSPROGS_SOURCE)/$(E2FSPROGS) clean; \
	fi

#
# Dist clean
#
.PHONY: e2fsprogs-distclean

e2fsprogs-distclean:
	@rm -f $(E2FSPROGS_DIR)/.e2fsprogs*

#
# Install
#
.PHONY: e2fsprogs-install

e2fsprogs-install:
	cp -fa $(E2FSPROGS_SOURCE)/$(E2FSPROGS)/misc/badblocks $(TOP_INSTALL_ROOTFS_DIR)/bin
	cp -fa $(E2FSPROGS_SOURCE)/$(E2FSPROGS)/misc/mke2fs $(TOP_INSTALL_ROOTFS_DIR)/bin
	cp -fa $(E2FSPROGS_SOURCE)/$(E2FSPROGS)/misc/tune2fs $(TOP_INSTALL_ROOTFS_DIR)/bin


