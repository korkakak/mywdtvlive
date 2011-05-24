#
# Defines
#
ifndef  CONF_NFS-UTILS_VERSION 
CONF_NFS-UTILS_VERSION := 1.2.3#
endif

NFS-UTILS             := nfs-utils-$(CONF_NFS-UTILS_VERSION)#
NFS-UTILS_SERVER_URL  := $(TOP_LIBS_URL)/nfs-utils/$(NFS-UTILS)
NFS-UTILS_DIR         := $(TOP_LIBS_DIR)#
NFS-UTILS_SOURCE      := $(NFS-UTILS_DIR)/$(NFS-UTILS)
NFS-UTILS_CONFIG      := 

#
# Download  the source 
#
.PHONY: nfs-utils-downloaded

nfs-utils-downloaded: $(NFS-UTILS_DIR)/.nfs-utils_downloaded

$(NFS-UTILS_DIR)/.nfs-utils_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: nfs-utils-unpacked

nfs-utils-unpacked: $(NFS-UTILS_DIR)/.nfs-utils_unpacked

$(NFS-UTILS_DIR)/.nfs-utils_unpacked: $(NFS-UTILS_DIR)/.nfs-utils_downloaded $(NFS-UTILS_SOURCE)/$(NFS-UTILS).tar.bz2
	@echo "Unpacking $(NFS-UTILS) ..."
	if [ -d "$(NFS-UTILS_SOURCE)/$(NFS-UTILS)" ]; then \
		rm -fr $(NFS-UTILS_SOURCE)/$(NFS-UTILS); \
	fi
	cd $(NFS-UTILS_SOURCE); tar xjvf $(NFS-UTILS).tar.bz2
	@echo "Unpacking $(NFS-UTILS) done"
	@touch $@


#
# Patch the source
#
.PHONY: nfs-utils-patched

nfs-utils-patched: $(NFS-UTILS_DIR)/.nfs-utils_patched

$(NFS-UTILS_DIR)/.nfs-utils_patched: $(NFS-UTILS_DIR)/.nfs-utils_unpacked
	@touch $@

#
# config
#
.PHONY: nfs-utils-config

nfs-utils-config: 

#
# Configuration
#
.PHONY: nfs-utils-configured

nfs-utils-configured:  nfs-utils-config $(NFS-UTILS_DIR)/.nfs-utils_configured

$(NFS-UTILS_DIR)/.nfs-utils_configured: $(NFS-UTILS_DIR)/.nfs-utils_patched $(NFS-UTILS_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(NFS-UTILS) ..."
	cd $(NFS-UTILS_SOURCE)/$(NFS-UTILS);\
	       	export CFLAGS=-I$(LIBS_INSTALL_PATH)/include ;\
		export LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib -lrt -ldl";\
		./configure  --prefix=$(LIBS_INSTALL_PATH) \
		--target=$(BUILD_TARGET) \
		--host=$(BUILD_TARGET) \
		--build=$(BUILD_HOST) \
		--enable-tirpc=no \
		--enable-mount=no \
		--enable-gss=no \
		--enable-tirpc=no \
		--without-tcp-wrappers \
		--enable-shared=no
	@echo "Configuring $(NFS-UTILS) done"
	@touch $@

#
# Compile
#
.PHONY: nfs-utils-compile

nfs-utils-compile: $(NFS-UTILS_DIR)/.nfs-utils_compiled

$(NFS-UTILS_DIR)/.nfs-utils_compiled: $(NFS-UTILS_DIR)/.nfs-utils_configured
	@echo "Compiling $(NFS-UTILS) ..."
	make -C $(NFS-UTILS_SOURCE)/$(NFS-UTILS) 
	#make -C $(NFS-UTILS_SOURCE)/$(NFS-UTILS)  install
	@echo "Compiling $(NFS-UTILS) done"
	@touch $@

#
# Update
#
.PHONY: nfs-utils-update

nfs-utils-update:
	@echo "Updating $(NFS-UTILS) ..."
	@if [ -d "$(NFS-UTILS_SOURCE)" ]; then\
		$(TOP_UPDATE) $(NFS-UTILS_SOURCE); \
	fi
	@echo "Updating $(NFS-UTILS) done"

#
# Status
#
.PHONY: nfs-utils-status

nfs-utils-status:
	@echo "Statusing $(NFS-UTILS) ..."
	@if [ -d "$(NFS-UTILS_SOURCE)" ]; then\
		$(TOP_STATUS) $(NFS-UTILS_SOURCE); \
	fi
	@echo "Statusing $(NFS-UTILS) done"

#
# Clean
#
.PHONY: nfs-utils-clean

nfs-utils-clean:
	@rm -f $(NFS-UTILS_DIR)/.nfs-utils_configured
	@if [ -d "$(NFS-UTILS_SOURCE)" ]; then\
		make -C $(NFS-UTILS_SOURCE)/$(NFS-UTILS) clean; \
	fi

#
# Dist clean
#
.PHONY: nfs-utils-distclean

nfs-utils-distclean:
	@rm -f $(NFS-UTILS_DIR)/.nfs-utils_*

#
# Install
#
.PHONY: nfs-utils-install

nfs-utils-install:
	cp -f  $(NFS-UTILS_SOURCE)/$(NFS-UTILS)/utils/showmount/showmount $(TOP_INSTALL_ROOTFS_DIR)/bin



