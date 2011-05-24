
#
# Defines
#
ifndef  CONF_GAWK_VERSION 
CONF_GAWK_VERSION := 3.1.8# 
endif

GAWK             := gawk-$(CONF_GAWK_VERSION)#
GAWK_SERVER_URL  := $(TOP_LIBS_URL)/gawk/$(GAWK)
GAWK_DIR         := $(TOP_LIBS_DIR)#
GAWK_SOURCE      := $(GAWK_DIR)/$(GAWK)
GAWK_CONFIG      := 

#
# Download  the source 
#
.PHONY: gawk-downloaded

gawk-downloaded: $(GAWK_DIR)/.gawk_downloaded

$(GAWK_DIR)/.gawk_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: gawk-unpacked

gawk-unpacked: $(GAWK_DIR)/.gawk_unpacked

$(GAWK_DIR)/.gawk_unpacked: $(GAWK_DIR)/.gawk_downloaded $(GAWK_SOURCE)/$(GAWK).tar.gz
	@echo "Unpacking $(GAWK) ..."
	if [ -d "$(GAWK_SOURCE)/$(GAWK)" ]; then \
		rm -fr $(GAWK_SOURCE)/$(GAWK); \
	fi
	cd $(GAWK_SOURCE); tar xzvf $(GAWK).tar.gz
	@echo "Unpacking $(GAWK) done"
	@touch $@


#
# Patch the source
#
.PHONY: gawk-patched

gawk-patched: $(GAWK_DIR)/.gawk_patched

$(GAWK_DIR)/.gawk_patched: $(GAWK_DIR)/.gawk_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: gawk-config

gawk-config: 

#
# Configuration
#
.PHONY: gawk-configured

gawk-configured:  gawk-config $(GAWK_DIR)/.gawk_configured

$(GAWK_DIR)/.gawk_configured: $(GAWK_DIR)/.gawk_patched $(GAWK_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(GAWK) ..."
	cd $(GAWK_SOURCE)/$(GAWK);./configure --prefix=$(LIBS_INSTALL_PATH)   --build=$(BUILD_HOST) --host=$(BUILD_TARGET) --target=$(BUILD_TARGET)
	@echo "Configuring $(GAWK) done"
	@touch $@

#
# Compile
#
.PHONY: gawk-compile

gawk-compile: $(GAWK_DIR)/.gawk_compiled

$(GAWK_DIR)/.gawk_compiled: $(GAWK_DIR)/.gawk_configured
	@echo "Compiling $(GAWK) ..."
	#make -C $(GAWK_SOURCE)/$(GAWK) CC=$(CC) AR=$(AR) RANLIB=$(RANLIB) STRIP=$(CROSS)strip LD=$(LD)
	make -C $(GAWK_SOURCE)/$(GAWK) 
	make -C $(GAWK_SOURCE)/$(GAWK)  install
	@echo "Compiling $(GAWK) done"
	@touch $@

#
# Update
#
.PHONY: gawk-update

gawk-update:
	@echo "Updating $(GAWK) ..."
	@if [ -d "$(GAWK_SOURCE)" ]; then\
		$(TOP_UPDATE) $(GAWK_SOURCE); \
	fi
	@echo "Updating $(GAWK) done"

#
# Status
#
.PHONY: gawk-status

gawk-status:
	@echo "Statusing $(GAWK) ..."
	@if [ -d "$(GAWK_SOURCE)" ]; then\
		$(TOP_STATUS) $(GAWK_SOURCE); \
	fi
	@echo "Statusing $(GAWK) done"

#
# Clean
#
.PHONY: gawk-clean

gawk-clean:
	@rm -f $(GAWK_DIR)/.gawk_configured
	@if [ -d "$(GAWK_SOURCE)" ]; then\
		make -C $(GAWK_SOURCE)/$(GAWK) clean; \
	fi

#
# Dist clean
#
.PHONY: gawk-distclean

gawk-distclean:
	@rm -f $(GAWK_DIR)/.gawk_*

#
# Install
#
.PHONY: gawk-install

gawk-install:
	cp -f $(GAWK_SOURCE)/$(GAWK)/gawk $(TOP_INSTALL_ROOTFS_DIR)/bin 
	$(CROSS)strip $(TOP_INSTALL_ROOTFS_DIR)/bin/gawk
