#
# Defines
#
NMAP_VERSION := 5.51#

NMAP             := nmap-$(NMAP_VERSION)#
NMAP_SERVER_URL  := $(TOP_LIBS_URL)/nmap/nmap-$(NMAP_VERSION)
NMAP_DIR         := $(TOP_LIBS_DIR)#
NMAP_SOURCE      := $(NMAP_DIR)/nmap-$(NMAP_VERSION)
NMAP_CONFIG      := 

#
# Download  the source 
#
.PHONY: nmap-downloaded

nmap-downloaded: $(NMAP_DIR)/.nmap_downloaded

$(NMAP_DIR)/.nmap_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: nmap-unpacked

nmap-unpacked: $(NMAP_DIR)/.nmap_unpacked

$(NMAP_DIR)/.nmap_unpacked: $(NMAP_DIR)/.nmap_downloaded $(wildcard $(NMAP_SOURCE)/*.tgz) $(wildcard $(NMAP_SOURCE)/*.patch)
	@echo "Unpacking $(NMAP) ..."
	if [ -d "$(NMAP_SOURCE)/$(NMAP)" ]; then \
		rm -fr $(NMAP_SOURCE)/$(NMAP); \
	fi
	cd $(NMAP_SOURCE); tar xzvf $(NMAP).tgz
	@echo "Unpacking $(NMAP) done"
	@touch $@

#
# config
#
.PHONY: nmap-config

nmap-config: $(NMAP_DIR)/.nmap-config

#
# Patch the source
#
.PHONY: nmap-patched

nmap-patched: $(NMAP_DIR)/.nmap-config

$(NMAP_DIR)/.nmap_patched: $(NMAP_DIR)/.nmap_unpacked 
	@touch $@

#
# Configuration
#
.PHONY: nmap-configured

nmap-configured:  nmap-configured $(NMAP_DIR)/.nmap_configured

$(NMAP_DIR)/.nmap_configured: $(NMAP_DIR)/.nmap_patched $(NMAP_CONFIG) $(TOP_CURRENT_SET)
	@cd $(NMAP_SOURCE)/$(NMAP); \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib " \
		CFLAGS=-I$(LIBS_INSTALL_PATH)/include \
		./configure \
		--prefix=/opt \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--with-libpcap=included \
		--with-pcap=linux \
		--with-libdnet=included \
		--without-openssl \
		ac_cv_linux_vers=2.6.22  \
		--without-ncat \
		--with-libpcre=included \
		--with-liblua=included
	@echo "Configuring $(NMAP) done"
	@touch $@

#
# Compile
#
.PHONY: nmap-compile

nmap-compile: $(NMAP_DIR)/.nmap_compiled

$(NMAP_DIR)/.nmap_compiled: $(NMAP_DIR)/.nmap_configured 
	@echo "Compiling $(NMAP) ..."
	make -C $(NMAP_SOURCE)/$(NMAP)
	make -C $(NMAP_SOURCE)/$(NMAP) install DESTDIR=$(LIBS_INSTALL_PATH)
	@echo "Compiling $(NMAP) done"
	@touch $@

#
# Update
#
.PHONY: nmap-update

nmap-update:
	@echo "Updating $(NMAP) ..."
	@if [ -d "$(NMAP_SOURCE)" ]; then\
		$(TOP_UPDATE) $(NMAP_SOURCE); \
	fi
	@echo "Updating $(NMAP) done"

#
# Status
#
.PHONY: nmap-status

nmap-status:
	@echo "Statusing $(NMAP) ..."
	@if [ -d "$(NMAP_SOURCE)" ]; then\
		$(TOP_STATUS) $(NMAP_SOURCE); \
	fi
	@echo "Statusing $(NMAP) done"

#
# Clean
#
.PHONY: nmap-clean

nmap-clean:
	@rm -f $(NMAP_DIR)/.nmap_configured
	@if [ -d "$(NMAP_SOURCE)" ]; then\
		make -C $(NMAP_SOURCE)/$(NMAP) clean; \
	fi

#
# Dist clean
#
.PHONY: nmap-distclean

nmap-distclean:
	@rm -f $(NMAP_DIR)/.nmap_*

#
# Install
#
.PHONY: nmap-install

nmap-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/bin
	cp -af $(LIBS_INSTALL_PATH)/opt/bin/nmap $(TOP_INSTALL_ROOTFS_DIR)/opt/bin
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/share/
	cp -raf $(LIBS_INSTALL_PATH)/opt/share/nmap $(TOP_INSTALL_ROOTFS_DIR)/opt/share/

