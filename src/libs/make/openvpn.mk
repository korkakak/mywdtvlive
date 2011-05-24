
ifndef  OPENVPN_VERSION 
OPENVPN_VERSION := 2.1.1# 
endif

OPENVPN             := openvpn-$(OPENVPN_VERSION)#
OPENVPN_SERVER_URL  := $(TOP_LIBS_URL)/openvpn/$(OPENVPN)
OPENVPN_DIR         := $(TOP_LIBS_DIR)#
OPENVPN_SOURCE      := $(OPENVPN_DIR)/$(OPENVPN)
OPENVPN_CONFIG      := 

OPENVPN_CONFIGURE := ./configure \
		--prefix=$(DBUS_PREFIX) \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--with-ssl-headers=$(LIBS_INSTALL_PATH)/include \
		--with-ssl-lib=$(LIBS_INSTALL_PATH)/lib \
		--disable-lzo \
		--enable-password-save

export OPENVPN
#
# Download  the source 
#
.PHONY: openvpn-downloaded

openvpn-downloaded: $(OPENVPN_DIR)/.openvpn_downloaded

$(OPENVPN_DIR)/.openvpn_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: openvpn-unpacked

openvpn-unpacked: $(OPENVPN_DIR)/.openvpn_unpacked

$(OPENVPN_DIR)/.openvpn_unpacked: $(OPENVPN_DIR)/.openvpn_downloaded $(wildcard $(OPENVPN_SOURCE)/*.tar.gz)
	@echo "Unpacking $(OPENVPN) ..."
	if [ -d "$(OPENVPN_SOURCE)/$(OPENVPN)" ]; then \
		rm -fr $(OPENVPN_SOURCE)/$(OPENVPN); \
	fi
	cd $(OPENVPN_SOURCE);tar xvzf  $(OPENVPN).tar.gz
	@echo "Unpacking $(OPENVPN) done"
	@touch $@


#
# Patch the source
#
.PHONY: openvpn-patched

openvpn-patched: $(OPENVPN_DIR)/.openvpn_patched

$(OPENVPN_DIR)/.openvpn_patched: $(OPENVPN_DIR)/.openvpn_unpacked
	@touch $@

#
# config
#
.PHONY: openvpn-config

openvpn-config: 

#
# Configuration
#
.PHONY: openvpn-configured

openvpn-configured:  openvpn-config $(OPENVPN_DIR)/.openvpn_configured

$(OPENVPN_DIR)/.openvpn_configured: $(OPENVPN_DIR)/.openvpn_patched $(OPENVPN_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(OPENVPN) ..." 
	cd $(OPENVPN_SOURCE)/$(OPENVPN); $(OPENVPN_CONFIGURE)
	@echo "Configuring $(OPENVPN) done"
	@touch $@

#
# Compile
#
.PHONY: openvpn-compile

openvpn-compile: $(OPENVPN_DIR)/.openvpn_compiled

$(OPENVPN_DIR)/.openvpn_compiled: $(OPENVPN_DIR)/.openvpn_configured
	@echo "Compiling $(OPENVPN) ..."
	make -C $(OPENVPN_SOURCE)/$(OPENVPN)
	@echo "Compiling $(OPENVPN) done"
	@touch $@

#
# Update
#
.PHONY: openvpn-update

openvpn-update:
	@echo "Updating $(OPENVPN) ..."
	@if [ -d "$(OPENVPN_SOURCE)" ]; then\
		$(TOP_UPDATE) $(OPENVPN_SOURCE); \
	fi
	@echo "Updating $(OPENVPN) done"

#
# Status
#
.PHONY: openvpn-status

openvpn-status:
	@echo "Statusing $(OPENVPN) ..."
	@if [ -d "$(OPENVPN_SOURCE)" ]; then\
		$(TOP_STATUS) $(OPENVPN_SOURCE); \
	fi
	@echo "Statusing $(OPENVPN) done"

#
# Clean
#
.PHONY: openvpn-clean

openvpn-clean:
	@rm -f $(OPENVPN_DIR)/.openvpn_configured
	@if [ -d "$(OPENVPN_SOURCE)" ]; then\
		make -C $(OPENVPN_SOURCE)/$(OPENVPN) clean; \
	fi

#
# Dist clean
#
.PHONY: openvpn-distclean

openvpn-distclean:
	@rm -f $(OPENVPN_DIR)/.openvpn_*

#
# Install
#
.PHONY: openvpn-install

openvpn-install:
	cp -f $(OPENVPN_SOURCE)/$(OPENVPN)/openvpn $(TOP_INSTALL_ROOTFS_DIR)/sbin 

