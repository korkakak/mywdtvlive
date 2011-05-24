#
# Defines
#
TCPDUMP_VERSION := 4.1.1#

TCPDUMP             := tcpdump-$(TCPDUMP_VERSION)#
TCPDUMP_SERVER_URL  := $(TOP_LIBS_URL)/tcpdump/tcpdump-$(TCPDUMP_VERSION)
TCPDUMP_DIR         := $(TOP_LIBS_DIR)#
TCPDUMP_SOURCE      := $(TCPDUMP_DIR)/tcpdump-$(TCPDUMP_VERSION)
TCPDUMP_CONFIG      := 


#
# Download  the source 
#
.PHONY: tcpdump-downloaded

tcpdump-downloaded: $(TCPDUMP_DIR)/.tcpdump_downloaded

$(TCPDUMP_DIR)/.tcpdump_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: tcpdump-unpacked

tcpdump-unpacked: $(TCPDUMP_DIR)/.tcpdump_unpacked

$(TCPDUMP_DIR)/.tcpdump_unpacked: $(TCPDUMP_DIR)/.tcpdump_downloaded $(wildcard $(TCPDUMP_SOURCE)/*.tgz) $(wildcard $(TCPDUMP_SOURCE)/*.patch)
	@echo "Unpacking $(TCPDUMP) ..."
	if [ -d "$(TCPDUMP_SOURCE)/$(TCPDUMP)" ]; then \
		rm -fr $(TCPDUMP_SOURCE)/$(TCPDUMP); \
	fi
	cd $(TCPDUMP_SOURCE); tar zxvf $(TCPDUMP).tar.gz
	@echo "Unpacking $(TCPDUMP) done"
	@touch $@

#
# config
#
.PHONY: tcpdump-config

tcpdump-config: $(TCPDUMP_DIR)/.tcpdump-config

#
# Patch the source
#
.PHONY: tcpdump-patched

tcpdump-patched: $(TCPDUMP_DIR)/.tcpdump-config

$(TCPDUMP_DIR)/.tcpdump_patched: $(TCPDUMP_DIR)/.tcpdump_unpacked 
	@touch $@

#
# Configuration
#
.PHONY: tcpdump-configured

tcpdump-configured:  tcpdump-configured $(TCPDUMP_DIR)/.tcpdump_configured

$(TCPDUMP_DIR)/.tcpdump_configured: $(TCPDUMP_DIR)/.tcpdump_patched $(TCPDUMP_CONFIG) $(TOP_CURRENT_SET)
	@cd $(TCPDUMP_SOURCE)/$(TCPDUMP); \
		LDFLAGS=-L$(LIBS_INSTALL_PATH)/lib \
		CFLAGS=-I$(LIBS_INSTALL_PATH)/include \
		./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=$(LIBS_INSTALL_PATH) \
		--with-pcap=linux \
		--enable-smb=no \
		--disable-ipv6 \
		ac_cv_linux_vers=2
	cd $(TCPDUMP_SOURCE)/$(TCPDUMP); sed -i -e  s/"\-I\/usr\/include"// Makefile
	@echo "Configuring $(TCPDUMP) done"
	@touch $@

#
# Compile
#
.PHONY: tcpdump-compile

tcpdump-compile: $(TCPDUMP_DIR)/.tcpdump_compiled

$(TCPDUMP_DIR)/.tcpdump_compiled: $(TCPDUMP_DIR)/.tcpdump_configured 
	@echo "Compiling $(TCPDUMP) ..."
	make -C $(TCPDUMP_SOURCE)/$(TCPDUMP) 
	make -C $(TCPDUMP_SOURCE)/$(TCPDUMP) install
	@echo "Compiling $(TCPDUMP) done"
	@touch $@

#
# Update
#
.PHONY: tcpdump-update

tcpdump-update:
	@echo "Updating $(TCPDUMP) ..."
	@if [ -d "$(TCPDUMP_SOURCE)" ]; then\
		$(TOP_UPDATE) $(TCPDUMP_SOURCE); \
	fi
	@echo "Updating $(TCPDUMP) done"

#
# Status
#
.PHONY: tcpdump-status

tcpdump-status:
	@echo "Statusing $(TCPDUMP) ..."
	@if [ -d "$(TCPDUMP_SOURCE)" ]; then\
		$(TOP_STATUS) $(TCPDUMP_SOURCE); \
	fi
	@echo "Statusing $(TCPDUMP) done"

#
# Clean
#
.PHONY: tcpdump-clean

tcpdump-clean:
	@rm -f $(TCPDUMP_DIR)/.tcpdump_configured
	@if [ -d "$(TCPDUMP_SOURCE)" ]; then\
		make -C $(TCPDUMP_SOURCE)/$(TCPDUMP) clean; \
	fi

#
# Dist clean
#
.PHONY: tcpdump-distclean

tcpdump-distclean:
	@rm -f $(TCPDUMP_DIR)/.tcpdump_*

#
# Install
#
.PHONY: tcpdump-install

tcpdump-install:
	cp -af $(LIBS_INSTALL_PATH)/sbin/tcpdump $(TOP_INSTALL_ROOTFS_DIR)/sbin

