#
# Defines
#
ifndef  CONF_LIBNFSIDMAP_VERSION 
CONF_LIBNFSIDMAP_VERSION := 0.24#
endif

LIBNFSIDMAP             := libnfsidmap-$(CONF_LIBNFSIDMAP_VERSION)#
LIBNFSIDMAP_SERVER_URL  := $(TOP_LIBS_URL)/libnfsidmap/$(LIBNFSIDMAP)
LIBNFSIDMAP_DIR         := $(TOP_LIBS_DIR)#
LIBNFSIDMAP_SOURCE      := $(LIBNFSIDMAP_DIR)/$(LIBNFSIDMAP)
LIBNFSIDMAP_CONFIG      := 

#
# Download  the source 
#
.PHONY: libnfsidmap-downloaded

libnfsidmap-downloaded: $(LIBNFSIDMAP_DIR)/.libnfsidmap_downloaded

$(LIBNFSIDMAP_DIR)/.libnfsidmap_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: libnfsidmap-unpacked

libnfsidmap-unpacked: $(LIBNFSIDMAP_DIR)/.libnfsidmap_unpacked

$(LIBNFSIDMAP_DIR)/.libnfsidmap_unpacked: $(LIBNFSIDMAP_DIR)/.libnfsidmap_downloaded $(LIBNFSIDMAP_SOURCE)/$(LIBNFSIDMAP).tar.gz
	@echo "Unpacking $(LIBNFSIDMAP) ..."
	if [ -d "$(LIBNFSIDMAP_SOURCE)/$(LIBNFSIDMAP)" ]; then \
		rm -fr $(LIBNFSIDMAP_SOURCE)/$(LIBNFSIDMAP); \
	fi
	cd $(LIBNFSIDMAP_SOURCE); tar xzvf $(LIBNFSIDMAP).tar.gz
	@echo "Unpacking $(LIBNFSIDMAP) done"
	@touch $@


#
# Patch the source
#
.PHONY: libnfsidmap-patched

libnfsidmap-patched: $(LIBNFSIDMAP_DIR)/.libnfsidmap_patched

$(LIBNFSIDMAP_DIR)/.libnfsidmap_patched: $(LIBNFSIDMAP_DIR)/.libnfsidmap_unpacked
	@touch $@

#
# config
#
.PHONY: libnfsidmap-config

libnfsidmap-config: 

#
# Configuration
#
.PHONY: libnfsidmap-configured

libnfsidmap-configured:  libnfsidmap-config $(LIBNFSIDMAP_DIR)/.libnfsidmap_configured

$(LIBNFSIDMAP_DIR)/.libnfsidmap_configured: $(LIBNFSIDMAP_DIR)/.libnfsidmap_patched $(LIBNFSIDMAP_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(LIBNFSIDMAP) ..."
	cd $(LIBNFSIDMAP_SOURCE)/$(LIBNFSIDMAP); \
		sed -i -e s,PACKAGE_STRING=\'libnfsidmap\ 0.24\',PACKAGE_STRING=\'libnfsidmap-0.24\', ./configure
	cd $(LIBNFSIDMAP_SOURCE)/$(LIBNFSIDMAP);\
		export PACKAGE_STRING=libnfsidmap-0.24 ; \
		./configure  --prefix=$(LIBS_INSTALL_PATH) \
		--target=$(BUILD_TARGET) \
		--host=$(BUILD_TARGET) \
		--build=$(BUILD_HOST) \
		--enable-shared=no \
		ac_cv_func_malloc_0_nonnull=yes
	@echo "Configuring $(LIBNFSIDMAP) done"
	@touch $@

#
# Compile
#
.PHONY: libnfsidmap-compile

libnfsidmap-compile: $(LIBNFSIDMAP_DIR)/.libnfsidmap_compiled

$(LIBNFSIDMAP_DIR)/.libnfsidmap_compiled: $(LIBNFSIDMAP_DIR)/.libnfsidmap_configured
	@echo "Compiling $(LIBNFSIDMAP) ..."
	make -C $(LIBNFSIDMAP_SOURCE)/$(LIBNFSIDMAP) 
	make -C $(LIBNFSIDMAP_SOURCE)/$(LIBNFSIDMAP)  install
	@echo "Compiling $(LIBNFSIDMAP) done"
	@touch $@

#
# Update
#
.PHONY: libnfsidmap-update

libnfsidmap-update:
	@echo "Updating $(LIBNFSIDMAP) ..."
	@if [ -d "$(LIBNFSIDMAP_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LIBNFSIDMAP_SOURCE); \
	fi
	@echo "Updating $(LIBNFSIDMAP) done"

#
# Status
#
.PHONY: libnfsidmap-status

libnfsidmap-status:
	@echo "Statusing $(LIBNFSIDMAP) ..."
	@if [ -d "$(LIBNFSIDMAP_SOURCE)" ]; then\
		$(TOP_STATUS) $(LIBNFSIDMAP_SOURCE); \
	fi
	@echo "Statusing $(LIBNFSIDMAP) done"

#
# Clean
#
.PHONY: libnfsidmap-clean

libnfsidmap-clean:
	@rm -f $(LIBNFSIDMAP_DIR)/.libnfsidmap_configured
	@if [ -d "$(LIBNFSIDMAP_SOURCE)" ]; then\
		make -C $(LIBNFSIDMAP_SOURCE)/$(LIBNFSIDMAP) clean; \
	fi

#
# Dist clean
#
.PHONY: libnfsidmap-distclean

libnfsidmap-distclean:
	@rm -f $(LIBNFSIDMAP_DIR)/.libnfsidmap_*

#
# Install
#
.PHONY: libnfsidmap-install

libnfsidmap-install:


