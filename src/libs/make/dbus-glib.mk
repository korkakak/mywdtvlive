#
# Defines
#
DBUS-GLIB_VERSION := 0.88# 

DBUS-GLIB             := dbus-glib-$(DBUS-GLIB_VERSION)#
DBUS-GLIB_SERVER_URL  := $(TOP_LIBS_URL)/dbus-glib/dbus-glib-$(DBUS-GLIB_VERSION)
DBUS-GLIB_DIR         := $(TOP_LIBS_DIR)#
DBUS-GLIB_SOURCE      := $(DBUS-GLIB_DIR)/dbus-glib-$(DBUS-GLIB_VERSION)
DBUS-GLIB_BUILD       := $(DBUS-GLIB_SOURCE)/dbus-glib-$(DBUS-GLIB_VERSION)
DBUS-GLIB_CONFIG      := 

#
# Download  the source 
#
.PHONY: dbus-glib-downloaded

dbus-glib-downloaded: $(DBUS-GLIB_DIR)/.dbus-glib_downloaded

$(DBUS-GLIB_DIR)/.dbus-glib_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: dbus-glib-unpacked

dbus-glib-unpacked: $(DBUS-GLIB_DIR)/.dbus-glib_unpacked

$(DBUS-GLIB_DIR)/.dbus-glib_unpacked: $(DBUS-GLIB_DIR)/.dbus-glib_downloaded $(wildcard $(DBUS-GLIB_SOURCE)/*.tar.gz) $(wildcard $(DBUS-GLIB_SOURCE)/$(DBUS-GLIB)*.patch)
	@echo "Unpacking $(DBUS-GLIB) ..."
	if [ -d "$(DBUS-GLIB_SOURCE)/$(DBUS-GLIB)" ]; then \
		rm -fr $(DBUS-GLIB_SOURCE)/$(DBUS-GLIB); \
	fi
	cd $(DBUS-GLIB_SOURCE); tar zxvf $(DBUS-GLIB).tar.gz
	@echo "Unpacking $(DBUS-GLIB) done"
	@touch $@


#
# Patch the source
#
.PHONY: dbus-glib-patched

dbus-glib-patched: $(DBUS-GLIB_DIR)/.dbus-glib_patched

$(DBUS-GLIB_DIR)/.dbus-glib_patched: $(DBUS-GLIB_DIR)/.dbus-glib_unpacked 
	@echo "Patching $(DBUS-GLIB) ..."
	cd $(DBUS-GLIB_BUILD); patch -p0 < ../configure.patch
	cd $(DBUS-GLIB_BUILD); patch -p0 < ../dbus_Makefile.am.patch
	@echo "Patching $(DBUS-GLIB) done"
	@touch $@

#
# config
#
.PHONY: dbus-glib-config

dbus-glib-config: 

#
# Configuration
#
.PHONY: dbus-glib-configured

dbus-glib-configured:  dbus-glib-config $(DBUS-GLIB_DIR)/.dbus-glib_configured

$(DBUS-GLIB_DIR)/.dbus-glib_configured: $(DBUS-GLIB_DIR)/.dbus-glib_patched $(DBUS-GLIB_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(DBUS-GLIB) ..."
	cd $(DBUS-GLIB_BUILD); aclocal
	cd $(DBUS-GLIB_BUILD); automake; true
	cd $(DBUS-GLIB_BUILD); \
		export PKG_CONFIG_PATH=$(LIBS_INSTALL_PATH)/lib/pkgconfig:$(LIBS_INSTALL_PATH)/opt/lib/pkgconfig ;\
		$(DBUS-GLIB_SOURCE)/$(DBUS-GLIB)/configure \
		--prefix=$(LIBS_INSTALL_PATH) \
		--build=$(BUILD_HOST)  \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--enable-tests=no \
		--disable-tests \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib -L$(LIBS_INSTALL_PATH)/opt/lib" \
	       	CFLAGS="-I$(LIBS_INSTALL_PATH)/include -I$(LIBS_INSTALL_PATH)/opt/include/dbus-1.0"
	@echo "Configuring $(DBUS-GLIB) done"
	@touch $@

#
# Compile
#
.PHONY: dbus-glib-compile

dbus-glib-compile: $(DBUS-GLIB_DIR)/.dbus-glib_compiled

$(DBUS-GLIB_DIR)/.dbus-glib_compiled: $(DBUS-GLIB_DIR)/.dbus-glib_configured 
	@echo "Compiling $(DBUS-GLIB) ..."
	@make -C $(DBUS-GLIB_BUILD)/dbus CC=$(CC) AR=$(AR) RANLIB=$(RANLIB) STRIP=$(CROSS)strip LD=$(LD) \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib -L$(LIBS_INSTALL_PATH)/opt/lib" \
	       	CFLAGS="-I$(LIBS_INSTALL_PATH)/include -I$(LIBS_INSTALL_PATH)/opt/include/dbus-1.0" 
	#@make -C $(DBUS-GLIB_BUILD) install 
	@echo "Compiling $(DBUS-GLIB) done"
	@touch $@

#
# Update
#
.PHONY: dbus-glib-update

dbus-glib-update:
	@echo "Updating $(DBUS-GLIB) ..."
	@if [ -d "$(DBUS-GLIB_SOURCE)" ]; then\
		$(TOP_UPDATE) $(DBUS-GLIB_SOURCE); \
	fi
	@echo "Updating $(DBUS-GLIB) done"

#
# Status
#
.PHONY: dbus-glib-status

dbus-glib-status:
	@echo "Statusing $(DBUS-GLIB) ..."
	@if [ -d "$(DBUS-GLIB_SOURCE)" ]; then\
		$(TOP_STATUS) $(DBUS-GLIB_SOURCE); \
	fi
	@echo "Statusing $(DBUS-GLIB) done"

#
# Clean
#
.PHONY: dbus-glib-clean

dbus-glib-clean:
	@rm -f $(DBUS-GLIB_DIR)/.dbus-glib_configured
	@if [ -d "$(DBUS-GLIB_BUILD)" ]; then\
		make -C $(DBUS-GLIB_BUILD) clean; \
	fi

#
# Dist clean
#
.PHONY: dbus-glib-distclean

dbus-glib-distclean:
	@rm -f $(DBUS-GLIB_DIR)/.dbus-glib_*

#
# Install
#
.PHONY: dbus-glib-install

dbus-glib-install:

