#
# These library is scratched from openssh
#

ifndef  OPENSSH_VERSION 
OPENSSH_VERSION := 4.7p1# 
endif

OPENSSH             := openssh-$(OPENSSH_VERSION)#
OPENSSH_SERVER_URL  := $(TOP_LIBS_URL)/openssh/$(OPENSSH)
OPENSSH_DIR         := $(TOP_LIBS_DIR)#
OPENSSH_SOURCE      := $(OPENSSH_DIR)/$(OPENSSH)
OPENSSH_CONFIG      := 

OPENSSH_CONFIGURE :=	CC=$(CC) LD=$(CC) CFLAGS=-I$(LIBS_INSTALL_PATH)/include \
			LDFLAGS=-L$(LIBS_INSTALL_PATH)/lib \
			./configure --prefix=/usr/local/sshd \
				--with-privsep-path=/tmp/empty \
				--build=$(BUILD_HOST) \
				--host=$(BUILD_TARGET) 
			

export OPENSSH
#
# Download  the source 
#
.PHONY: openssh-downloaded

openssh-downloaded: $(OPENSSH_DIR)/.openssh_downloaded

$(OPENSSH_DIR)/.openssh_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: openssh-unpacked

openssh-unpacked: $(OPENSSH_DIR)/.openssh_unpacked

$(OPENSSH_DIR)/.openssh_unpacked: $(OPENSSH_DIR)/.openssh_downloaded $(wildcard $(OPENSSH_SOURCE)/*.tar.gz) $(wildcard $(OPENSSH_SOURCE)/*.patch)
	@echo "Unpacking $(OPENSSH) ..."
	if [ -d "$(OPENSSH_SOURCE)/$(OPENSSH)" ]; then \
		rm -fr $(OPENSSH_SOURCE)/$(OPENSSH); \
	fi
	cd $(OPENSSH_SOURCE);tar xvzf  $(OPENSSH).tar.gz
	@echo "Unpacking $(OPENSSH) done"
	@touch $@


#
# Patch the source
#
.PHONY: openssh-patched

openssh-patched: $(OPENSSH_DIR)/.openssh_patched

$(OPENSSH_DIR)/.openssh_patched: $(OPENSSH_DIR)/.openssh_unpacked
	@touch $@

#
# config
#
.PHONY: openssh-config

openssh-config: 

#
# Configuration
#
.PHONY: openssh-configured

openssh-configured:  openssh-config $(OPENSSH_DIR)/.openssh_configured

$(OPENSSH_DIR)/.openssh_configured: $(OPENSSH_DIR)/.openssh_patched $(OPENSSH_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(OPENSSH) ..." 
	cd $(OPENSSH_SOURCE)/$(OPENSSH); $(OPENSSH_CONFIGURE)
	@echo "Configuring $(OPENSSH) done"
	@touch $@

#
# Compile
#
.PHONY: openssh-compile

openssh-compile: $(OPENSSH_DIR)/.openssh_compiled

$(OPENSSH_DIR)/.openssh_compiled: $(OPENSSH_DIR)/.openssh_configured
	@echo "Compiling $(OPENSSH) ..."
	make -C $(OPENSSH_SOURCE)/$(OPENSSH)
	#make -C $(OPENSSH_SOURCE)/$(OPENSSH) install
	@echo "Compiling $(OPENSSH) done"
	@touch $@

#
# Update
#
.PHONY: openssh-update

openssh-update:
	@echo "Updating $(OPENSSH) ..."
	@if [ -d "$(OPENSSH_SOURCE)" ]; then\
		$(TOP_UPDATE) $(OPENSSH_SOURCE); \
	fi
	@echo "Updating $(OPENSSH) done"

#
# Status
#
.PHONY: openssh-status

openssh-status:
	@echo "Statusing $(OPENSSH) ..."
	@if [ -d "$(OPENSSH_SOURCE)" ]; then\
		$(TOP_STATUS) $(OPENSSH_SOURCE); \
	fi
	@echo "Statusing $(OPENSSH) done"

#
# Clean
#
.PHONY: openssh-clean

openssh-clean:
	@rm -f $(OPENSSH_DIR)/.openssh_configured
	@if [ -d "$(OPENSSH_SOURCE)" ]; then\
		make -C $(OPENSSH_SOURCE)/$(OPENSSH) clean; \
	fi

#
# Dist clean
#
.PHONY: openssh-distclean

openssh-distclean:
	@rm -f $(OPENSSH_DIR)/.openssh_*

#
# Install
#
.PHONY: openssh-install

openssh-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/usr/local/sshd/etc
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/usr/local/sshd/libexec
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/usr/local/sshd/sbin
	cp -a $(OPENSSH_SOURCE)/$(OPENSSH)/../etc/* $(TOP_INSTALL_ROOTFS_DIR)/usr/local/sshd/etc
	cp -a $(OPENSSH_SOURCE)/$(OPENSSH)/sftp-server $(TOP_INSTALL_ROOTFS_DIR)/usr/local/sshd/libexec
	cp -a $(OPENSSH_SOURCE)/$(OPENSSH)/ssh-keysign $(TOP_INSTALL_ROOTFS_DIR)/usr/local/sshd/libexec
	cp -a $(OPENSSH_SOURCE)/$(OPENSSH)/sshd $(TOP_INSTALL_ROOTFS_DIR)/usr/local/sshd/sbin









