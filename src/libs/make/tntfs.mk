


TNTFS_SERVER_URL	:= $(TOP_LIBS_URL)/tntfs/$(CONF_TNTFS_VERSION)
TNTFS_SOURCE	:= $(TOP_LIBS_DIR)/tntfs


#
# Download  the source 
#
.PHONY: tntfs-downloaded

tntfs-downloaded: $(TOP_LIBS_DIR)/.tntfs_downloaded

$(TOP_LIBS_DIR)/.tntfs_downloaded:
	@touch $@


#
# Unpack the source
#
.PHONY: tntfs-unpacked

tntfs-unpacked: $(TOP_LIBS_DIR)/.tntfs_unpacked

$(TOP_LIBS_DIR)/.tntfs_unpacked: $(TOP_LIBS_DIR)/.tntfs_downloaded


#
# Patch the source
#
.PHONY: tntfs-patched

tntfs-patched: $(TOP_LIBS_DIR)/.tntfs_patched

$(TOP_LIBS_DIR)/.tntfs_patched: $(TOP_LIBS_DIR)/.tntfs_unpacked

#
# config
#
.PHONY: tntfs-config

tntfs-config: 

#
# Configuration
#
.PHONY: tntfs-configured

tntfs-configured:  tntfs-config $(TOP_LIBS_DIR)/.tntfs_configured

$(TOP_LIBS_DIR)/.tntfs_configured: $(TOP_LIBS_DIR)/.tntfs_patched

#
# Compile
#
.PHONY: tntfs-compile

tntfs-compile: $(TOP_LIBS_DIR)/.tntfs_compiled

$(TOP_LIBS_DIR)/.tntfs_compiled: $(TOP_LIBS_DIR)/.tntfs_configured

#
# Update
#
.PHONY: tntfs-update

tntfs-update:

#
# Status
#
.PHONY: tntfs-status

tntfs-status:

#
# Clean
#
.PHONY: tntfs-clean

tntfs-clean:

#
# Dist clean
#
.PHONY: tntfs-distclean

tntfs-distclean:

#
# Install
#
.PHONY: tntfs-install

tntfs-install:
	cp -f $(TNTFS_SOURCE)/tntfs.ko $(TOP_INSTALL_ROOTFS_DIR)/lib/modules 

