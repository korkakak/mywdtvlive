
#
# Defines
#
ifndef CONF_IOZONE3_VERSION 
IOZONE3_VERSION := 347# 
else
IOZONE3_VERSION := $(CONF_IOZONE3_VERSION)
endif

IOZONE3             := iozone3_$(IOZONE3_VERSION)#
IOZONE3_SERVER_URL  := $(TOP_LIBS_URL)/iozone3/$(IOZONE3)#b
IOZONE3_DIR         := $(TOP_LIBS_DIR)#
IOZONE3_SOURCE      := $(IOZONE3_DIR)/$(IOZONE3)
IOZONE3_SOURCE_CODE := $(IOZONE3_SOURCE)/$(IOZONE3)/src/current#
IOZONE3_CONFIG      := 

#ifeq (x$(ENABLE_SHLIB),xyes)
#        IOZONE3_SHARED_FLAGS      := --shared
#endif

#
# Download  the source 
#
.PHONY: iozone3-downloaded

iozone3-downloaded: $(IOZONE3_DIR)/.iozone3_downloaded

$(IOZONE3_DIR)/.iozone3_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: iozone3-unpacked

iozone3-unpacked: $(IOZONE3_DIR)/.iozone3_unpacked

$(IOZONE3_DIR)/.iozone3_unpacked: $(IOZONE3_DIR)/.iozone3_downloaded $(IOZONE3_SOURCE)/$(IOZONE3).tar.gz
	@echo "Unpacking $(IOZONE3) ..."
	if [ -d "$(IOZONE3_SOURCE)/$(IOZONE3)" ]; then \
		rm -fr $(IOZONE3_SOURCE)/$(IOZONE3); \
	fi
	cd $(IOZONE3_SOURCE); tar xvzf  $(IOZONE3).tar.gz
	@echo "Unpacking $(IOZONE3) done"
	@touch $@


#
# Patch the source
#
.PHONY: iozone3-patched

iozone3-patched: $(IOZONE3_DIR)/.iozone3_patched

$(IOZONE3_DIR)/.iozone3_patched: $(IOZONE3_DIR)/.iozone3_unpacked
	@touch $@

#
# config
#
.PHONY: iozone3-config

iozone3-config: 

#
# Configuration
#
.PHONY: iozone3-configured

iozone3-configured:  iozone3-config $(IOZONE3_DIR)/.iozone3_configured

$(IOZONE3_DIR)/.iozone3_configured: $(IOZONE3_DIR)/.iozone3_patched $(IOZONE3_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(IOZONE3) ..."
	@echo "Configuring $(IOZONE3) done"
	@touch $@

#
# Compile
#
.PHONY: iozone3-compile

iozone3-compile: $(IOZONE3_DIR)/.iozone3_compiled

$(IOZONE3_DIR)/.iozone3_compiled: $(IOZONE3_DIR)/.iozone3_configured
	@echo "Compiling $(IOZONE3) ..."
	make -C $(IOZONE3_SOURCE_CODE) linux CC=$(CC)
	@echo "Compiling $(IOZONE3) done"
	@touch $@

#
# Update
#
.PHONY: iozone3-update

iozone3-update:
	@echo "Updating $(IOZONE3) ..."
	@if [ -d "$(IOZONE3_SOURCE)" ]; then\
		$(TOP_UPDATE) $(IOZONE3_SOURCE); \
	fi
	@echo "Updating $(IOZONE3) done"

#
# Status
#
.PHONY: iozone3-status

iozone3-status:
	@echo "Statusing $(IOZONE3) ..."
	@if [ -d "$(IOZONE3_SOURCE)" ]; then\
		$(TOP_STATUS) $(IOZONE3_SOURCE); \
	fi
	@echo "Statusing $(IOZONE3) done"

#
# Clean
#
.PHONY: iozone3-clean

iozone3-clean:
	@rm -f $(IOZONE3_DIR)/.iozone3_configured
	@if [ -d "$(IOZONE3_SOURCE)" ]; then\
		make -C $(IOZONE3_SOURCE_CODE) clean 
	fi

#
# Dist clean
#
.PHONY: iozone3-distclean

iozone3-distclean:
	@rm -f $(IOZONE3_DIR)/.iozone3_*

#
# Install
#
.PHONY: iozone3-install

iozone3-install:
	@echo $(TOP_BUILD_ROOTFS_DIR)
	cp -f $(IOZONE3_SOURCE_CODE)/iozone $(TOP_INSTALL_ROOTFS_DIR)/bin


