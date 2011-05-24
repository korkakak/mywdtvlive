#
# Defines
#
ifndef  CONF_LZO_VERSION 
CONF_LZO_VERSION := 2.03#
endif

LZO             := lzo-$(CONF_LZO_VERSION)#
LZO_SERVER_URL  := $(TOP_LIBS_URL)/lzo/$(LZO)#
LZO_DIR         := $(TOP_LIBS_DIR)#
LZO_SOURCE      := $(LZO_DIR)/$(LZO)#
LZO_CONFIG      := 


# Download  the source 
#
.PHONY: lzo-downloaded

lzo-downloaded: $(LZO_DIR)/.lzo_downloaded

$(LZO_DIR)/.lzo_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: lzo-unpacked

lzo-unpacked: $(LZO_DIR)/.lzo_unpacked

$(LZO_DIR)/.lzo_unpacked: $(LZO_DIR)/.lzo_downloaded $(LZO_SOURCE)/$(LZO).tar.gz 
	@echo "Unpacking $(LZO) ..."
	if [ -d "$(LZO_SOURCE)/$(LZO)" ]; then \
		rm -fr $(LZO_SOURCE)/$(LZO); \
	fi
	(cd $(LZO_SOURCE); tar xvf $(LZO).tar.gz)
	@echo "Unpacking $(LZO) done"
	@touch $@


#
# Patch the source
#
.PHONY: lzo-patched

lzo-patched: $(LZO_DIR)/.lzo_patched

$(LZO_DIR)/.lzo_patched: $(LZO_DIR)/.lzo_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: lzo-config

lzo-config: 

#
# Configuration
#
.PHONY: lzo-configured

lzo-configured:  lzo-config $(LZO_DIR)/.lzo_configured

$(LZO_DIR)/.lzo_configured: $(LZO_DIR)/.lzo_patched $(LZO_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(LZO) ..."
	cd $(LZO_SOURCE)/$(LZO); ./configure --prefix=$(LIBS_INSTALL_PATH) --build=$(BUILD_HOST) --target=$(BUILD_TARGET) --host=$(BUILD_TARGET) --enable-shared=yes
	@echo "Configuring $(LZO) done"
	@touch $@

#
# Compile
#
.PHONY: lzo-compile

lzo-compile: $(LZO_DIR)/.lzo_compiled

$(LZO_DIR)/.lzo_compiled: $(LZO_DIR)/.lzo_configured
	@echo "Compiling $(LZO) ..."
	make -C $(LZO_SOURCE)/$(LZO)
	make -C $(LZO_SOURCE)/$(LZO) install
	@echo "Compiling $(LZO) done"
	@touch $@

#
# Update
#
.PHONY: lzo-update

lzo-update:
	@echo "Updating $(LZO) ..."
	@if [ -d "$(LZO_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LZO_SOURCE); \
	fi
	@echo "Updating $(LZO) done"

#
# Status
#
.PHONY: lzo-status

lzo-status:
	@echo "Statusing $(LZO) ..."
	@if [ -d "$(LZO_SOURCE)" ]; then\
		$(TOP_STATUS) $(LZO_SOURCE); \
	fi
	@echo "Statusing $(LZO) done"

#
# Clean
#
.PHONY: lzo-clean

lzo-clean:
	@rm -f $(LZO_DIR)/.lzo_configured
	@if [ -d "$(LZO_SOURCE)" ]; then\
		make -C $(LZO_SOURCE)/$(LZO) clean; \
	fi

#
# Dist clean
#
.PHONY: lzo-distclean

lzo-distclean:
	@rm -f $(LZO_DIR)/.lzo*

#
# Install
#
.PHONY: lzo-install

lzo-install:
	cp -ra $(LIBS_INSTALL_PATH)/lib/liblzo2.so* $(TOP_INSTALL_ROOTFS_DIR)/lib



