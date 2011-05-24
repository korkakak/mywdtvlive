#
# Defines
#
LIBPCAP_VERSION := 1.1.1#

LIBPCAP             := libpcap-$(LIBPCAP_VERSION)#
LIBPCAP_SERVER_URL  := $(TOP_LIBS_URL)/libpcap/libpcap-$(LIBPCAP_VERSION)
LIBPCAP_DIR         := $(TOP_LIBS_DIR)#
LIBPCAP_SOURCE      := $(LIBPCAP_DIR)/libpcap-$(LIBPCAP_VERSION)
LIBPCAP_CONFIG      := 

#
# Download  the source 
#
.PHONY: libpcap-downloaded

libpcap-downloaded: $(LIBPCAP_DIR)/.libpcap_downloaded

$(LIBPCAP_DIR)/.libpcap_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libpcap-unpacked

libpcap-unpacked: $(LIBPCAP_DIR)/.libpcap_unpacked

$(LIBPCAP_DIR)/.libpcap_unpacked: $(LIBPCAP_DIR)/.libpcap_downloaded $(wildcard $(LIBPCAP_SOURCE)/*.tgz) $(wildcard $(LIBPCAP_SOURCE)/*.patch)
	@echo "Unpacking $(LIBPCAP) ..."
	if [ -d "$(LIBPCAP_SOURCE)/$(LIBPCAP)" ]; then \
		rm -fr $(LIBPCAP_SOURCE)/$(LIBPCAP); \
	fi
	cd $(LIBPCAP_SOURCE); tar zxvf $(LIBPCAP).tar.gz
	@echo "Unpacking $(LIBPCAP) done"
	@touch $@

#
# config
#
.PHONY: libpcap-config

libpcap-config: $(LIBPCAP_DIR)/.libpcap-config

#
# Patch the source
#
.PHONY: libpcap-patched

libpcap-patched: $(LIBPCAP_DIR)/.libpcap-config

$(LIBPCAP_DIR)/.libpcap_patched: $(LIBPCAP_DIR)/.libpcap_unpacked 
	@touch $@

#
# Configuration
#
.PHONY: libpcap-configured

libpcap-configured:  libpcap-configured $(LIBPCAP_DIR)/.libpcap_configured

$(LIBPCAP_DIR)/.libpcap_configured: $(LIBPCAP_DIR)/.libpcap_patched $(LIBPCAP_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(LIBPCAP) ..."
	@cd $(LIBPCAP_SOURCE)/$(LIBPCAP); \
		./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=$(LIBS_INSTALL_PATH) \
		--with-pcap=linux \
		ac_cv_linux_vers=2
	@echo "Configuring $(LIBPCAP) done"
	@touch $@

#
# Compile
#
.PHONY: libpcap-compile

libpcap-compile: $(LIBPCAP_DIR)/.libpcap_compiled

$(LIBPCAP_DIR)/.libpcap_compiled: $(LIBPCAP_DIR)/.libpcap_configured 
	@echo "Compiling $(LIBPCAP) ..."
	make -C $(LIBPCAP_SOURCE)/$(LIBPCAP) 
	make -C $(LIBPCAP_SOURCE)/$(LIBPCAP) install 
	@echo "Compiling $(TCPDUMP) done"
	@touch $@

#
# Update
#
.PHONY: libpcap-update

libpcap-update:
	@echo "Updating $(LIBPCAP) ..."
	@if [ -d "$(LIBPCAP_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBPCAP_SOURCE); \
	fi
	@echo "Updating $(LIBPCAP) done"

#
# Status
#
.PHONY: libpcap-status

libpcap-status:
	@echo "Statusing $(LIBPCAP) ..."
	@if [ -d "$(LIBPCAP_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBPCAP_SOURCE); \
	fi
	@echo "Statusing $(LIBPCAP) done"

#
# Clean
#
.PHONY: libpcap-clean

libpcap-clean:
	@rm -f $(LIBPCAP_DIR)/.libpcap_configured
	@if [ -d "$(LIBPCAP_SOURCE)" ]; then\
		make -C $(LIBPCAP_SOURCE)/$(LIBPCAP) clean; \
	fi

#
# Dist clean
#
.PHONY: libpcap-distclean

libpcap-distclean:
	@rm -f $(LIBPCAP_DIR)/.libpcap_*

#
# Install
#
.PHONY: libpcap-install

libpcap-install:
	cp -af $(LIBS_INSTALL_PATH)/lib/libpcap*so* $(TOP_INSTALL_ROOTFS_DIR)/lib

	
