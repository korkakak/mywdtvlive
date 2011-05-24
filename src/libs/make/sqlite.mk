##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  joshua_lee@alphanetworks.com
# sqlite makefile fragments 
##############################################################################

#
# Defines
#
ifndef  CONF_SQLITE_VERSION 
CONF_SQLITE_VERSION := 3.7.5#
endif

SQLITE             := sqlite-amalgamation-$(CONF_SQLITE_VERSION)#
SQLITE_SERVER_URL  := $(TOP_LIBS_URL)/sqlite/$(SQLITE)
SQLITE_DIR         := $(TOP_LIBS_DIR)#
SQLITE_SOURCE      := $(SQLITE_DIR)/$(SQLITE)
SQLITE_CONFIG      := 


#
# Download  the source 
#
.PHONY: sqlite-downloaded

sqlite-downloaded: $(SQLITE_DIR)/.sqlite_downloaded

$(SQLITE_DIR)/.sqlite_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: sqlite-unpacked

sqlite-unpacked: $(SQLITE_DIR)/.sqlite_unpacked

$(SQLITE_DIR)/.sqlite_unpacked: $(SQLITE_DIR)/.sqlite_downloaded
	@echo "Unpacking $(SQLITE) ..."
	if [ -d "$(SQLITE_SOURCE)/$(SQLITE)" ]; then \
		rm -fr $(SQLITE_SOURCE)/$(SQLITE); \
	fi
	cd $(SQLITE_SOURCE); tar zxvf $(SQLITE).tar.gz
	@echo "Unpacking $(SQLITE) done"
	@touch $@


#
# Patch the source
#
.PHONY: sqlite-patched

sqlite-patched: $(SQLITE_DIR)/.sqlite_patched

$(SQLITE_DIR)/.sqlite_patched: $(SQLITE_DIR)/.sqlite_unpacked
	# Patching...
	(cd $(SQLITE_SOURCE)/$(SQLITE);sed -i -e "s/sqlite 3.5.9/sqlite_3.5.9/" configure;)
	(cd $(SQLITE_SOURCE)/$(SQLITE);sed -i -e "s/sqlite 3.7.5/sqlite_3.7.5/" configure;)
	@touch $@


#
# Configuration
#
.PHONY: sqlite-configured

sqlite-configured:  $(SQLITE_DIR)/.sqlite_configured

$(SQLITE_DIR)/.sqlite_configured: $(SQLITE_DIR)/.sqlite_patched $(TOP_CURRENT_SET)
	@echo "Configuring $(SQLITE) ..."
	cd $(SQLITE_SOURCE)/$(SQLITE);CFLAGS=$(SQLITE_CFLAGS) LDFLAGS=$(SQLITE_LDFLAGS)  ./configure --prefix=$(LIBS_INSTALL_PATH) --build=$(BUILD_HOST) --host=$(BUILD_TARGET) --target=$(BUILD_TARGET) --enable-shared=$(ENABLE_SHLIB) --enable-readline=no --enable-dynamic-extensions=no
	@echo "Configuring $(SQLITE) done"
	@touch $@

#
# Compile
#
.PHONY: sqlite-compile

sqlite-compile: sqlite-configured $(SQLITE_DIR)/.sqlite_compile

$(SQLITE_DIR)/.sqlite_compile:
	@echo "Compiling $(SQLITE) ..." 
	-make -C $(SQLITE_SOURCE)/$(SQLITE) CC=$(CC) AR=$(AR) RANLIB=$(RANLIB) STRIP=$(CROSS)strip LD=$(LD) CFLAGS=$(SQLITE_CFLAGS) LDFLAGS=$(SQLITE_LDFLAGS) install
	@echo "Compiling $(SQLITE) done" 
	touch $@

#
# Update
#
.PHONY: sqlite-update

sqlite-update:
	@echo "Updating $(SQLITE) ..."
	@if [ -d "$(SQLITE_SOURCE)" ]; then\
		$(TOP_UPDATE) $(SQLITE_SOURCE); \
	fi
	@echo "Updating $(SQLITE) done"

#
# Status
#
.PHONY: sqlite-status

sqlite-status:
	@echo "Statusing $(SQLITE) ..."
	@echo "Statusing $(SQLITE) done"

#
# Clean
#
.PHONY: sqlite-clean

sqlite-clean:
	@rm -f $(SQLITE_DIR)/.sqlite_configured
	@if [ -d "$(SQLITE_SOURCE)" ]; then\
		make -C $(SQLITE_SOURCE)/$(SQLITE) clean; \
	fi

#
# Dist clean
#
.PHONY: sqlite-distclean

sqlite-distclean:
	@rm -f $(SQLITE_DIR)/.sqlite_*

#
# Install
#
.PHONY: sqlite-install

sqlite-install:
	-cp -f $(SQLITE_SOURCE)/$(SQLITE)/.libs/sqlite3 $(TOP_INSTALL_ROOTFS_DIR)/bin
