#
# Defines
#
ifndef  CONF_LIBEVENT_VERSION 
CONF_LIBEVENT_VERSION := 2.0.10-stable#
endif

LIBEVENT             := libevent-$(CONF_LIBEVENT_VERSION)#
LIBEVENT_SERVER_URL  := $(TOP_LIBS_URL)/libevent/$(LIBEVENT)
LIBEVENT_DIR         := $(TOP_LIBS_DIR)#
LIBEVENT_SOURCE      := $(LIBEVENT_DIR)/$(LIBEVENT)
LIBEVENT_CONFIG      := 

#
# Download  the source 
#
.PHONY: libevent-downloaded

libevent-downloaded: $(LIBEVENT_DIR)/.libevent_downloaded

$(LIBEVENT_DIR)/.libevent_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libevent-unpacked

libevent-unpacked: $(LIBEVENT_DIR)/.libevent_unpacked

$(LIBEVENT_DIR)/.libevent_unpacked: $(LIBEVENT_DIR)/.libevent_downloaded $(LIBEVENT_SOURCE)/$(LIBEVENT).tar.gz
	@echo "Unpacking $(LIBEVENT) ..."
	if [ -d "$(LIBEVENT_SOURCE)/$(LIBEVENT)" ]; then \
		rm -fr $(LIBEVENT_SOURCE)/$(LIBEVENT); \
	fi
	cd $(LIBEVENT_SOURCE); tar xzvf $(LIBEVENT).tar.gz
	@echo "Unpacking $(LIBEVENT) done"
	@touch $@


#
# Patch the source
#
.PHONY: libevent-patched

libevent-patched: $(LIBEVENT_DIR)/.libevent_patched

$(LIBEVENT_DIR)/.libevent_patched: $(LIBEVENT_DIR)/.libevent_unpacked
	@touch $@

#
# config
#
.PHONY: libevent-config

libevent-config: 

#
# Configuration
#
.PHONY: libevent-configured

libevent-configured:  libevent-config $(LIBEVENT_DIR)/.libevent_configured

$(LIBEVENT_DIR)/.libevent_configured: $(LIBEVENT_DIR)/.libevent_patched $(LIBEVENT_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(LIBEVENT) ..."
	cd $(LIBEVENT_SOURCE)/$(LIBEVENT); ./configure  --prefix=$(LIBS_INSTALL_PATH) \
		--target=$(BUILD_TARGET) \
		--host=$(BUILD_TARGET) \
		--build=$(BUILD_HOST)  \
		--enable-shared=no
	@echo "Configuring $(LIBEVENT) done"
	@touch $@

#
# Compile
#
.PHONY: libevent-compile

libevent-compile: $(LIBEVENT_DIR)/.libevent_compiled

$(LIBEVENT_DIR)/.libevent_compiled: $(LIBEVENT_DIR)/.libevent_configured
	@echo "Compiling $(LIBEVENT) ..."
	make -C $(LIBEVENT_SOURCE)/$(LIBEVENT) 
	make -C $(LIBEVENT_SOURCE)/$(LIBEVENT)  install
	@echo "Compiling $(LIBEVENT) done"
	@touch $@

#
# Update
#
.PHONY: libevent-update

libevent-update:
	@echo "Updating $(LIBEVENT) ..."
	@if [ -d "$(LIBEVENT_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBEVENT_SOURCE); \
	fi
	@echo "Updating $(LIBEVENT) done"

#
# Status
#
.PHONY: libevent-status

libevent-status:
	@echo "Statusing $(LIBEVENT) ..."
	@if [ -d "$(LIBEVENT_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBEVENT_SOURCE); \
	fi
	@echo "Statusing $(LIBEVENT) done"

#
# Clean
#
.PHONY: libevent-clean

libevent-clean:
	@rm -f $(LIBEVENT_DIR)/.libevent_configured
	@if [ -d "$(LIBEVENT_SOURCE)" ]; then\
		make -C $(LIBEVENT_SOURCE)/$(LIBEVENT) clean; \
	fi

#
# Dist clean
#
.PHONY: libevent-distclean

libevent-distclean:
	@rm -f $(LIBEVENT_DIR)/.libevent_*

#
# Install
#
.PHONY: libevent-install

libevent-install:


