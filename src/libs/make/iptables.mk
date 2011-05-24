#
# Defines
#
ifndef  CONF_IPTABLES_VERSION 
CONF_IPTABLES_VERSION := 1.4.10#
endif

IPTABLES             := iptables-$(CONF_IPTABLES_VERSION)#
IPTABLES_SERVER_URL  := $(TOP_LIBS_URL)/iptables/$(IPTABLES)
IPTABLES_DIR         := $(TOP_LIBS_DIR)#
IPTABLES_SOURCE      := $(IPTABLES_DIR)/$(IPTABLES)
IPTABLES_CONFIG      := 

ifeq (x$(ENABLE_SHLIB),xyes)
        IPTABLES_SHARED_FLAGS      := --shared
endif

#
# Download  the source 
#
.PHONY: iptables-downloaded

iptables-downloaded: $(IPTABLES_DIR)/.iptables_downloaded

$(IPTABLES_DIR)/.iptables_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: iptables-unpacked

iptables-unpacked: $(IPTABLES_DIR)/.iptables_unpacked

$(IPTABLES_DIR)/.iptables_unpacked: $(IPTABLES_DIR)/.iptables_downloaded $(IPTABLES_SOURCE)/$(IPTABLES).tar.bz2
	@echo "Unpacking $(IPTABLES) ..."
	if [ -d "$(IPTABLES_SOURCE)/$(IPTABLES)" ]; then \
		rm -fr $(IPTABLES_SOURCE)/$(IPTABLES); \
	fi
	cd $(IPTABLES_SOURCE); tar xjvf $(IPTABLES).tar.bz2
	@echo "Unpacking $(IPTABLES) done"
	@touch $@


#
# Patch the source
#
.PHONY: iptables-patched

iptables-patched: $(IPTABLES_DIR)/.iptables_patched

$(IPTABLES_DIR)/.iptables_patched: $(IPTABLES_DIR)/.iptables_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: iptables-config

iptables-config: 

#
# Configuration
#
.PHONY: iptables-configured

iptables-configured:  iptables-config $(IPTABLES_DIR)/.iptables_configured

$(IPTABLES_DIR)/.iptables_configured: $(IPTABLES_DIR)/.iptables_patched $(IPTABLES_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(IPTABLES) ..."
	cd $(IPTABLES_SOURCE)/$(IPTABLES);./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--enable-static \
		--disable-shared \
		--with-kernel=$(LINUX_KERNEL_SOURCE)
	@echo "Configuring $(IPTABLES) done"
	@touch $@

#
# Compile
#
.PHONY: iptables-compile

iptables-compile: $(IPTABLES_DIR)/.iptables_compiled

$(IPTABLES_DIR)/.iptables_compiled: $(IPTABLES_DIR)/.iptables_configured
	@echo "Compiling $(IPTABLES) ..."
	make -C $(IPTABLES_SOURCE)/$(IPTABLES) 
	@echo "Compiling $(IPTABLES) done"
	@touch $@

#
# Update
#
.PHONY: iptables-update

iptables-update:
	@echo "Updating $(IPTABLES) ..."
	@if [ -d "$(IPTABLES_SOURCE)" ]; then\
		$(TOP_UPDATE) $(IPTABLES_SOURCE); \
	fi
	@echo "Updating $(IPTABLES) done"

#
# Status
#
.PHONY: iptables-status

iptables-status:
	@echo "Statusing $(IPTABLES) ..."
	@if [ -d "$(IPTABLES_SOURCE)" ]; then\
		$(TOP_STATUS) $(IPTABLES_SOURCE); \
	fi
	@echo "Statusing $(IPTABLES) done"

#
# Clean
#
.PHONY: iptables-clean

iptables-clean:
	@rm -f $(IPTABLES_DIR)/.iptables_configured
	@if [ -d "$(IPTABLES_SOURCE)" ]; then\
		make -C $(IPTABLES_SOURCE)/$(IPTABLES) clean; \
	fi

#
# Dist clean
#
.PHONY: iptables-distclean

iptables-distclean:
	@rm -f $(IPTABLES_DIR)/.iptables_*

#
# Install
#
.PHONY: iptables-install

iptables-install:
	cp -a $(IPTABLES_SOURCE)/$(IPTABLES)/iptables-multi $(TOP_INSTALL_ROOTFS_DIR)/bin/iptables 

