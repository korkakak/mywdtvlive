#
# Defines
#
ifndef  CONF_UMSP_VERSION 
CONF_UMSP_VERSION := v0.1.2_gpl# 
endif

UMSP             := umsp_$(CONF_UMSP_VERSION)#
UMSP_SERVER_URL  := $(TOP_LIBS_URL)/umsp/$(UMSP)
UMSP_DIR         := $(TOP_LIBS_DIR)#
UMSP_SOURCE      := $(UMSP_DIR)/$(UMSP)
UMSP_CONFIG      := 

#
# Download  the source 
#
.PHONY: umsp-downloaded

umsp-downloaded: $(UMSP_DIR)/.umsp_downloaded

$(UMSP_DIR)/.umsp_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: umsp-unpacked

umsp-unpacked: $(UMSP_DIR)/.umsp_unpacked

$(UMSP_DIR)/.umsp_unpacked: $(UMSP_DIR)/.umsp_downloaded $(UMSP_SOURCE)/$(UMSP).zip
	@echo "Unpacking $(UMSP) ..."
	if [ -d "$(UMSP_SOURCE)/$(UMSP)" ]; then \
		rm -fr $(UMSP_SOURCE)/$(UMSP); \
	fi
	cd $(UMSP_SOURCE); unzip $(UMSP).zip
	@echo "Unpacking $(UMSP) done"
	@touch $@


#
# Patch the source
#
.PHONY: umsp-patched

umsp-patched: $(UMSP_DIR)/.umsp_patched

$(UMSP_DIR)/.umsp_patched: $(UMSP_DIR)/.umsp_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: umsp-config

umsp-config: 

#
# Configuration
#
.PHONY: umsp-configured

umsp-configured:  umsp-config $(UMSP_DIR)/.umsp_configured

$(UMSP_DIR)/.umsp_configured: $(UMSP_DIR)/.umsp_patched $(UMSP_CONFIG) $(TOP_CURRENT_SET)
	@touch $@

#
# Compile
#
.PHONY: umsp-compile

umsp-compile: $(UMSP_DIR)/.umsp_compiled

$(UMSP_DIR)/.umsp_compiled: $(UMSP_DIR)/.umsp_configured
	@touch $@

#
# Update
#
.PHONY: umsp-update

umsp-update:
	@echo "Updating $(UMSP) ..."
	@if [ -d "$(UMSP_SOURCE)" ]; then\
		$(TOP_UPDATE) $(UMSP_SOURCE); \
	fi
	@echo "Updating $(UMSP) done"

#
# Status
#
.PHONY: umsp-status

umsp-status:
	@echo "Statusing $(UMSP) ..."
	@if [ -d "$(UMSP_SOURCE)" ]; then\
		$(TOP_STATUS) $(UMSP_SOURCE); \
	fi
	@echo "Statusing $(UMSP) done"

#
# Clean
#
.PHONY: umsp-clean

umsp-clean:
	@rm -f $(UMSP_DIR)/.umsp_configured

#
# Dist clean
#
.PHONY: umsp-distclean

umsp-distclean:
	@rm -f $(UMSP_DIR)/.umsp_*

#
# Install
#
.PHONY: umsp-install

umsp-install:
	cp -ra $(UMSP_SOURCE)/umsp $(TOP_INSTALL_ROOTFS_DIR)/webserver/htdocs 
	chmod 755 $(TOP_INSTALL_ROOTFS_DIR)/webserver/htdocs/umsp/bin/*.sh

