


WD_ORION_SERVER_URL	:= $(TOP_LIBS_URL)/wd_orion
WD_ORION_SOURCE	:= $(TOP_LIBS_DIR)/wd_orion


#
# Download  the source 
#
.PHONY: wd_orion-downloaded

wd_orion-downloaded: $(TOP_LIBS_DIR)/.wd_orion_downloaded

$(TOP_LIBS_DIR)/.wd_orion_downloaded:


#
# Unpack the source
#
.PHONY: wd_orion-unpacked

wd_orion-unpacked: $(TOP_LIBS_DIR)/.wd_orion_unpacked

$(TOP_LIBS_DIR)/.wd_orion_unpacked: $(TOP_LIBS_DIR)/.wd_orion_downloaded


#
# Patch the source
#
.PHONY: wd_orion-patched

wd_orion-patched: $(TOP_LIBS_DIR)/.wd_orion_patched

$(TOP_LIBS_DIR)/.wd_orion_patched: $(TOP_LIBS_DIR)/.wd_orion_unpacked

#
# config
#
.PHONY: wd_orion-config

wd_orion-config: 

#
# Configuration
#
.PHONY: wd_orion-configured

wd_orion-configured:  wd_orion-config $(TOP_LIBS_DIR)/.wd_orion_configured

$(TOP_LIBS_DIR)/.wd_orion_configured: $(TOP_LIBS_DIR)/.wd_orion_patched

#
# Compile
#
.PHONY: wd_orion-compile

wd_orion-compile: $(TOP_LIBS_DIR)/.wd_orion_compiled

$(TOP_LIBS_DIR)/.wd_orion_compiled: $(TOP_LIBS_DIR)/.wd_orion_configured

#
# Update
#
.PHONY: wd_orion-update

wd_orion-update:

#
# Status
#
.PHONY: wd_orion-status

wd_orion-status:

#
# Clean
#
.PHONY: wd_orion-clean

wd_orion-clean:

#
# Dist clean
#
.PHONY: wd_orion-distclean

wd_orion-distclean:

#
# Install
#
.PHONY: wd_orion-install

wd_orion-install:
	cp -fra $(WD_ORION_SOURCE)/usr $(TOP_INSTALL_ROOTFS_DIR) 
	cp -fra $(WD_ORION_SOURCE)/webserver $(TOP_INSTALL_ROOTFS_DIR) 
