##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  wills_yin@alphanetworks.com
# json-c makefile fragments 
##############################################################################

#
# Defines
#
ifndef  CONF_JSON_C_VERSION 
CONF_JSON_C_VERSION := 0.9
endif

JSON_C             := json-c-$(CONF_JSON_C_VERSION)#
JSON_C_SERVER_URL  := $(TOP_LIBS_URL)/json-c/$(JSON_C)#
JSON_C_DIR         := $(TOP_LIBS_DIR)#
JSON_C_SOURCE      := $(JSON_C_DIR)/$(JSON_C)#
JSON_C_CONFIG      := 

JSON_C_CONFIGURE := --prefix=$(LIBS_INSTALL_PATH) --build=$(BUILD_HOST) --host=$(BUILD_TARGET)

## Download  the source 
#
.PHONY: json-c-downloaded

json-c-downloaded: $(JSON_C_DIR)/.json-c_downloaded

$(JSON_C_DIR)/.json-c_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: json-c-unpacked

json-c-unpacked: $(JSON_C_DIR)/.json-c_unpacked

$(JSON_C_DIR)/.json-c_unpacked: $(JSON_C_DIR)/.json-c_downloaded $(JSON_C_SOURCE)/$(JSON_C).tar.gz
	@echo "Unpacking $(JSON_C) ..."
	if [ -d "$(JSON_C_SOURCE)/$(JSON_C)" ]; then \
		rm -fr $(JSON_C_SOURCE)/$(JSON_C); \
	fi
	(cd $(JSON_C_SOURCE); tar zxvf $(JSON_C).tar.gz)
	@echo "Unpacking $(JSON_C) done"
	@touch $@


#
# Patch the source
#
.PHONY: json-c-patched

json-c-patched: $(JSON_C_DIR)/.json-c_patched

$(JSON_C_DIR)/.json-c_patched: $(JSON_C_DIR)/.json-c_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: json-c-config

json-c-config: 

#
# Configuration
#
.PHONY: json-c-configured

json-c-configured:  json-c-config $(JSON_C_DIR)/.json-c_configured

$(JSON_C_DIR)/.json-c_configured: $(JSON_C_DIR)/.json-c_patched $(JSON_C_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(JSON_C) ..."
	cd $(JSON_C_SOURCE)/$(JSON_C); ./configure $(JSON_C_CONFIGURE)
	@echo "Configuring $(JSON_C) done"
	@touch $@

#
# Compile
#
.PHONY: json-c-compile

json-c-compile: $(JSON_C_DIR)/.json-c_compiled

$(JSON_C_DIR)/.json-c_compiled: $(JSON_C_DIR)/.json-c_configured
	@echo "Compiling $(JSON_C) ..."
	make -C $(JSON_C_SOURCE)/$(JSON_C)
	make -C $(JSON_C_SOURCE)/$(JSON_C) install
	@echo "Compiling $(JSON_C) done"
	@touch $@

#
# Update
#
.PHONY: json-c-update

json-c-update:
	@echo "Updating $(JSON_C) ..."
	@if [ -d "$(JSON_C_SOURCE)" ]; then\
		$(TOP_UPDATE) $(JSON_C_SOURCE); \
	fi
	@echo "Updating $(JSON_C) done"

#
# Status
#
.PHONY: json-c-status

json-c-status:
	@echo "Statusing $(JSON_C) ..."
	@if [ -d "$(JSON_C_SOURCE)" ]; then\
		$(TOP_STATUS) $(JSON_C_SOURCE); \
	fi
	@echo "Statusing $(JSON_C) done"

#
# Clean
#
.PHONY: json-c-clean

json-c-clean:
	@rm -f $(JSON_C_DIR)/.json-c_configured
	@if [ -d "$(JSON_C_SOURCE)" ]; then\
		make -C $(JSON_C_SOURCE)/$(JSON_C) clean; \
	fi

#
# Dist clean
#
.PHONY: json-c-distclean

json-c-distclean:
	@rm -f $(JSON_C_DIR)/.json-c*

#
# Install
#
.PHONY: json-c-install

json-c-install:
