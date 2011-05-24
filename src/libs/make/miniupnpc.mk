#
# Defines
#
ifndef  CONF_MINIUPNPC_VERSION 
CONF_MINIUPNPC_VERSION := 1.4#
endif

MINIUPNPC             := miniupnpc-$(CONF_MINIUPNPC_VERSION)#
MINIUPNPC_SERVER_URL  := $(TOP_LIBS_URL)/miniupnpc/$(MINIUPNPC)
MINIUPNPC_DIR         := $(TOP_LIBS_DIR)#
MINIUPNPC_SOURCE      := $(MINIUPNPC_DIR)/$(MINIUPNPC)
MINIUPNPC_CONFIG      := 

#
# Download  the source 
#
.PHONY: miniupnpc-downloaded

miniupnpc-downloaded: $(MINIUPNPC_DIR)/.miniupnpc_downloaded

$(MINIUPNPC_DIR)/.miniupnpc_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: miniupnpc-unpacked

miniupnpc-unpacked: $(MINIUPNPC_DIR)/.miniupnpc_unpacked

$(MINIUPNPC_DIR)/.miniupnpc_unpacked: $(MINIUPNPC_DIR)/.miniupnpc_downloaded $(MINIUPNPC_SOURCE)/$(MINIUPNPC).tar.gz
	@echo "Unpacking $(MINIUPNPC) ..."
	if [ -d "$(MINIUPNPC_SOURCE)/$(MINIUPNPC)" ]; then \
		rm -fr $(MINIUPNPC_SOURCE)/$(MINIUPNPC); \
	fi
	cd $(MINIUPNPC_SOURCE); tar xzvf $(MINIUPNPC).tar.gz
	@echo "Unpacking $(MINIUPNPC) done"
	@touch $@


#
# Patch the source
#
.PHONY: miniupnpc-patched

miniupnpc-patched: $(MINIUPNPC_DIR)/.miniupnpc_patched

$(MINIUPNPC_DIR)/.miniupnpc_patched: $(MINIUPNPC_DIR)/.miniupnpc_unpacked
	cd $(MINIUPNPC_SOURCE); patch -p0 < miniupnpc.patch
	@touch $@

#
# config
#
.PHONY: miniupnpc-config

miniupnpc-config: 

#
# Configuration
#
.PHONY: miniupnpc-configured

miniupnpc-configured:  miniupnpc-config $(MINIUPNPC_DIR)/.miniupnpc_configured

$(MINIUPNPC_DIR)/.miniupnpc_configured: $(MINIUPNPC_DIR)/.miniupnpc_patched $(MINIUPNPC_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(MINIUPNPC) ..."
	@echo "Configuring $(MINIUPNPC) done"
	@touch $@

#
# Compile
#
.PHONY: miniupnpc-compile

miniupnpc-compile: $(MINIUPNPC_DIR)/.miniupnpc_compiled

$(MINIUPNPC_DIR)/.miniupnpc_compiled: $(MINIUPNPC_DIR)/.miniupnpc_configured
	@echo "Compiling $(MINIUPNPC) ..."
	cd $(MINIUPNPC_SOURCE)/$(MINIUPNPC) ; make CC=$(CC)
	@echo "Compiling $(MINIUPNPC) done"
	@touch $@

#
# Update
#
.PHONY: miniupnpc-update

miniupnpc-update:
	@echo "Updating $(MINIUPNPC) ..."
	@if [ -d "$(MINIUPNPC_SOURCE)" ]; then\
		$(TOP_UPDATE) $(MINIUPNPC_SOURCE); \
	fi
	@echo "Updating $(MINIUPNPC) done"

#
# Status
#
.PHONY: miniupnpc-status

miniupnpc-status:
	@echo "Statusing $(MINIUPNPC) ..."
	@if [ -d "$(MINIUPNPC_SOURCE)" ]; then\
		$(TOP_STATUS) $(MINIUPNPC_SOURCE); \
	fi
	@echo "Statusing $(MINIUPNPC) done"

#
# Clean
#
.PHONY: miniupnpc-clean

miniupnpc-clean:
	@rm -f $(MINIUPNPC_DIR)/.miniupnpc_configured
	@if [ -d "$(MINIUPNPC_SOURCE)" ]; then\
		make -C $(MINIUPNPC_SOURCE)/$(MINIUPNPC) clean; \
	fi

#
# Dist clean
#
.PHONY: miniupnpc-distclean

miniupnpc-distclean:
	@rm -f $(MINIUPNPC_DIR)/.miniupnpc_*

#
# Install
#
.PHONY: miniupnpc-install

miniupnpc-install:
	cp -f $(MINIUPNPC_SOURCE)/$(MINIUPNPC)/libminiupnpc.so $(TOP_INSTALL_ROOTFS_DIR)/lib 
	( cd $(TOP_INSTALL_ROOTFS_DIR)/lib; ln -s libminiupnpc.so libminiupnpc.so.4)
	cp -f $(MINIUPNPC_SOURCE)/$(MINIUPNPC)/upnpc-shared $(TOP_INSTALL_ROOTFS_DIR)/bin/upnpc

