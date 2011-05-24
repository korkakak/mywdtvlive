#
# Defines
#
ifndef  CONF_NETCAT_VERSION 
CONF_NETCAT_VERSION := 1.10-38# 
endif

NETCAT             := netcat-$(CONF_NETCAT_VERSION)#
NETCAT_SERVER_URL  := $(TOP_LIBS_URL)/netcat/$(NETCAT)
NETCAT_DIR         := $(TOP_LIBS_DIR)#
NETCAT_SOURCE      := $(NETCAT_DIR)/$(NETCAT)
NETCAT_CONFIG      := 

#
# Download  the source 
#
.PHONY: netcat-downloaded

netcat-downloaded: $(NETCAT_DIR)/.netcat_downloaded

$(NETCAT_DIR)/.netcat_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: netcat-unpacked

netcat-unpacked: $(NETCAT_DIR)/.netcat_unpacked

$(NETCAT_DIR)/.netcat_unpacked: $(NETCAT_DIR)/.netcat_downloaded $(NETCAT_SOURCE)/$(NETCAT).tgz
	@echo "Unpacking $(NETCAT) ..."
	if [ -d "$(NETCAT_SOURCE)/$(NETCAT)" ]; then \
		rm -fr $(NETCAT_SOURCE)/$(NETCAT); \
	fi
	cd $(NETCAT_SOURCE); tar zxvf $(NETCAT).tgz
	@echo "Unpacking $(NETCAT) done"
	@touch $@


#
# Patch the source
#
.PHONY: netcat-patched

netcat-patched: $(NETCAT_DIR)/.netcat_patched

$(NETCAT_DIR)/.netcat_patched: $(NETCAT_DIR)/.netcat_unpacked
	# Patching...
	cd $(NETCAT_SOURCE)/$(NETCAT); patch -p0 < ../Makefile.patch

	@touch $@

#
# config
#
.PHONY: netcat-config

netcat-config: 

#
# Configuration
#
.PHONY: netcat-configured

netcat-configured:  netcat-config $(NETCAT_DIR)/.netcat_configured

$(NETCAT_DIR)/.netcat_configured: $(NETCAT_DIR)/.netcat_patched $(NETCAT_CONFIG) $(TOP_CURRENT_SET)
	@touch $@

#
# Compile
#
.PHONY: netcat-compile

netcat-compile: $(NETCAT_DIR)/.netcat_compiled

$(NETCAT_DIR)/.netcat_compiled: $(NETCAT_DIR)/.netcat_configured
	@echo "Compiling $(NETCAT) ..."
	make -C $(NETCAT_SOURCE)/$(NETCAT) CC=$(CC) AR=$(AR) RANLIB=$(RANLIB) STRIP=$(CROSS)strip LD=$(CC) linux
	@echo "Compiling $(NETCAT) done"
	@touch $@

#
# Update
#
.PHONY: netcat-update

netcat-update:
	@echo "Updating $(NETCAT) ..."
	@if [ -d "$(NETCAT_SOURCE)" ]; then\
		$(TOP_UPDATE) $(NETCAT_SOURCE); \
	fi
	@echo "Updating $(NETCAT) done"

#
# Status
#
.PHONY: netcat-status

netcat-status:
	@echo "Statusing $(NETCAT) ..."
	@if [ -d "$(NETCAT_SOURCE)" ]; then\
		$(TOP_STATUS) $(NETCAT_SOURCE); \
	fi
	@echo "Statusing $(NETCAT) done"

#
# Clean
#
.PHONY: netcat-clean

netcat-clean:
	@rm -f $(NETCAT_DIR)/.netcat_configured
	@if [ -d "$(NETCAT_SOURCE)" ]; then\
		make -C $(NETCAT_SOURCE)/$(NETCAT) clean; \
	fi

#
# Dist clean
#
.PHONY: netcat-distclean

netcat-distclean:
	@rm -f $(NETCAT_DIR)/.netcat_*

#
# Install
#
.PHONY: netcat-install

netcat-install:
	cp -f $(NETCAT_SOURCE)/$(NETCAT)/nc $(TOP_INSTALL_ROOTFS_DIR)/bin  
