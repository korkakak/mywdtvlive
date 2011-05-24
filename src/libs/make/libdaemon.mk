
#
# Defines
#
ifndef  CONF_LIBDAEMON_VERSION 
CONF_LIBDAEMON_VERSION := 0.14# 
endif

LIBDAEMON             := libdaemon-$(CONF_LIBDAEMON_VERSION)#
LIBDAEMON_SERVER_URL  := $(TOP_LIBS_URL)/libdaemon/$(LIBDAEMON)
LIBDAEMON_DIR         := $(TOP_LIBS_DIR)#
LIBDAEMON_SOURCE      := $(LIBDAEMON_DIR)/$(LIBDAEMON)
LIBDAEMON_CONFIG      := 


#
# Download  the source 
#
.PHONY: libdaemon-downloaded

libdaemon-downloaded: $(LIBDAEMON_DIR)/.libdaemon_downloaded

$(LIBDAEMON_DIR)/.libdaemon_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libdaemon-unpacked

libdaemon-unpacked: $(LIBDAEMON_DIR)/.libdaemon_unpacked

$(LIBDAEMON_DIR)/.libdaemon_unpacked: $(LIBDAEMON_DIR)/.libdaemon_downloaded $(LIBDAEMON_SOURCE)/$(LIBDAEMON).tar.gz
	@echo "Unpacking $(LIBDAEMON) ..."
	if [ -d "$(LIBDAEMON_SOURCE)/$(LIBDAEMON)" ]; then \
		rm -fr $(LIBDAEMON_SOURCE)/$(LIBDAEMON); \
	fi
	cd $(LIBDAEMON_SOURCE); tar zxvf $(LIBDAEMON).tar.gz
	@echo "Unpacking $(LIBDAEMON) done"
	@touch $@


#
# Patch the source
#
.PHONY: libdaemon-patched

libdaemon-patched: $(LIBDAEMON_DIR)/.libdaemon_patched

$(LIBDAEMON_DIR)/.libdaemon_patched: $(LIBDAEMON_DIR)/.libdaemon_unpacked
	cd $(LIBDAEMON_SOURCE)/$(LIBDAEMON); patch -p0 < $(LIBDAEMON_SOURCE)/libdaemon.patch
	@touch $@

#
# config
#
.PHONY: libdaemon-config

libdaemon-config: 

#
# Configuration
#
.PHONY: libdaemon-configured

libdaemon-configured:  libdaemon-config $(LIBDAEMON_DIR)/.libdaemon_configured

$(LIBDAEMON_DIR)/.libdaemon_configured: $(LIBDAEMON_DIR)/.libdaemon_patched $(LIBDAEMON_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(LIBDAEMON) ..."
	cd $(LIBDAEMON_SOURCE)/$(LIBDAEMON); \
	./configure --prefix=$(LIBS_INSTALL_PATH) \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--enable-shared=no
	@echo "Configuring $(LIBDAEMON) done"
	@touch $@

#
# Compile
#
.PHONY: libdaemon-compile

libdaemon-compile: $(LIBDAEMON_DIR)/.libdaemon_compiled

$(LIBDAEMON_DIR)/.libdaemon_compiled: $(LIBDAEMON_DIR)/.libdaemon_configured
	@echo "Compiling $(LIBDAEMON) ..."
	make -C $(LIBDAEMON_SOURCE)/$(LIBDAEMON) 
	make -C $(LIBDAEMON_SOURCE)/$(LIBDAEMON)  install
	@echo "Compiling $(LIBDAEMON) done"
	@touch $@

#
# Update
#
.PHONY: libdaemon-update

libdaemon-update:
	@echo "Updating $(LIBDAEMON) ..."
	@if [ -d "$(LIBDAEMON_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBDAEMON_SOURCE); \
	fi
	@echo "Updating $(LIBDAEMON) done"

#
# Status
#
.PHONY: libdaemon-status

libdaemon-status:
	@echo "Statusing $(LIBDAEMON) ..."
	@if [ -d "$(LIBDAEMON_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBDAEMON_SOURCE); \
	fi
	@echo "Statusing $(LIBDAEMON) done"

#
# Clean
#
.PHONY: libdaemon-clean

libdaemon-clean:
	@rm -f $(LIBDAEMON_DIR)/.libdaemon_configured
	@if [ -d "$(LIBDAEMON_SOURCE)" ]; then\
		make -C $(LIBDAEMON_SOURCE)/$(LIBDAEMON) clean; \
	fi

#
# Dist clean
#
.PHONY: libdaemon-distclean

libdaemon-distclean:
	@rm -f $(LIBDAEMON_DIR)/.libdaemon_*

#
# Install
#
.PHONY: libdaemon-install

libdaemon-install:
