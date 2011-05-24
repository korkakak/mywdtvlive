#
# Defines
#
DBUS-PYTHON_VERSION := 0.83.0#

DBUS-PYTHON             := dbus-python-$(DBUS-PYTHON_VERSION)#
DBUS-PYTHON_SERVER_URL  := $(TOP_LIBS_URL)/dbus-python/$(DBUS-PYTHON)
DBUS-PYTHON_DIR         := $(TOP_LIBS_DIR)#
DBUS-PYTHON_SOURCE      := $(DBUS-PYTHON_DIR)/$(DBUS-PYTHON)
DBUS-PYTHON_CONFIG      := 
DBUS-PYTHON_PATCH       := $(DBUS-PYTHON_SOURCE)/$(DBUS-PYTHON).patch

DBUS-PYTHON_PREFIX	:= opt#

DBUS-PYTHON_ENABLE_SHLIB	:= no
ifeq (x$(DBUS-PYTHON_ENABLE_SHLIB),xno)
	DBUS-PYTHON_SHARED_FLAGS      := --enable-shared=no
endif

#
# Download  the source 
#
.PHONY: dbus-python-downloaded

dbus-python-downloaded: $(DBUS-PYTHON_DIR)/.dbus-python_downloaded

$(DBUS-PYTHON_DIR)/.dbus-python_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: dbus-python-unpacked

dbus-python-unpacked: $(DBUS-PYTHON_DIR)/.dbus-python_unpacked

$(DBUS-PYTHON_DIR)/.dbus-python_unpacked: $(DBUS-PYTHON_DIR)/.dbus-python_downloaded $(wildcard $(DBUS-PYTHON_SOURCE)/*.tgz)
	echo "Unpacking $(DBUS-PYTHON) ..."
	if [ -d "$(DBUS-PYTHON_SOURCE)/$(DBUS-PYTHON)" ]; then \
		rm -fr $(DBUS-PYTHON_SOURCE)/$(DBUS-PYTHON); \
	fi
	cd $(DBUS-PYTHON_SOURCE); tar zxvf $(DBUS-PYTHON).tar.gz
	@echo "Unpacking $(DBUS-PYTHON) done"
	@touch $@

#
# Patch the source
#
.PHONY: dbus-python-patched

dbus-python-patched: $(DBUS-PYTHON_DIR)/.dbus-python-patched

$(DBUS-PYTHON_DIR)/.dbus-python_patched: $(DBUS-PYTHON_DIR)/.dbus-python_unpacked
	cd $(DBUS-PYTHON_SOURCE)/$(DBUS-PYTHON); patch  -p1 < $(DBUS-PYTHON_PATCH)
	@touch $@

#
# config
#
.PHONY: readline-config

readline-config: 


#
# Configuration
#
.PHONY: dbus-python-configured

dbus-python-configured:  dbus-python-configured $(DBUS-PYTHON_DIR)/.dbus-python_configured

$(DBUS-PYTHON_DIR)/.dbus-python_configured: $(DBUS-PYTHON_DIR)/.dbus-python_patched $(DBUS-PYTHON_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(DBUS-PYTHON) ..."
	export PYTHON=$(LIBS_INSTALL_PATH)/opt/bin/python2.7; \
	export HOST_PYTHON=/usr/bin/python2.7; \
	cd $(DBUS-PYTHON_SOURCE)/$(DBUS-PYTHON); \
	CC=mipsel-linux-gcc \
	CXX=mipsel-linux-g++ \
	AR=mipsel-linux-ar \
	RANLIB=mipsel-linux-ranlib \
	CFLAGS="-I$(LIBS_INSTALL_PATH)/include -I$(LIBS_INSTALL_PATH)/opt/include/python2.7 -I$(LIBS_INSTALL_PATH)/include" \
	CPPFLAGS="-I$(LIBS_INSTALL_PATH)/include -I$(LIBS_INSTALL_PATH)/opt/include/python2.7 -I$(LIBS_INSTALL_PATH)/include" \
	LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib -L$(LIBS_INSTALL_PATH)/opt/lib" \
	GIOUNIX_CFLAGS="-pthread -I$(LIBS_INSTALL_PATH)/include/gio-unix-2.0/ -I$(LIBS_INSTALL_PATH)/include/glib-2.0 -I$(LIBS_INSTALL_PATH)/lib/glib-2.0/include" \
	GIO_CFLAGS="-pthread -I$(LIBS_INSTALL_PATH)/include/glib-2.0 -I/$(LIBS_INSTALL_PATH)/lib/glib-2.0/include" \
	GLIB_CFLAGS="-pthread -I$(LIBS_INSTALL_PATH)/include/glib-2.0 -I$(LIBS_INSTALL_PATH)/lib/glib-2.0/include" \
	PYCAIRO_CFLAGS="-pthread -I$(LIBS_INSTALL_PATH)/include/pycairo -I$(LIBS_INSTALL_PATH)/include/cairo -I$(LIBS_INSTALL_PATH)/include/glib-2.0 -I$(LIBS_INSTALL_PATH)/lib/glib-2.0/include -I$(LIBS_INSTALL_PATH)/include/pixman-1 -I$(LIBS_INSTALL_PATH)/include/freetype2 -I$(LIBS_INSTALL_PATH)/include/libpng12" \
	./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=$(LIBS_INSTALL_PATH)/opt/ \
		--without-ffi \
		--disable-glibtest  \
		--disable-introspection
	@echo "Configuring $(DBUS-PYTHON) done"
	@touch $@

	#GI_CFLAGS='-pthread -I/$(LIBS_INSTALL_PATH)/local/include/gobject-introspection-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include  '
	#GI_DATADIR='/usr/local/share/gobject-introspection-1.0'
#
# Compile
#
.PHONY: dbus-python-compile

dbus-python-compile: $(DBUS-PYTHON_DIR)/.dbus-python_compiled

$(DBUS-PYTHON_DIR)/.dbus-python_compiled: $(DBUS-PYTHON_DIR)/.dbus-python_configured 
	@echo "Compiling $(DBUS-PYTHON) ..."
	@cd $(DBUS-PYTHON_SOURCE)/$(DBUS-PYTHON); \
		make
	echo "Compiling $(DBUS-PYTHON) done"
	@cd $(DBUS-PYTHON_SOURCE)/$(DBUS-PYTHON); \
		export HOST_PYTHON=/usr/bin/python2.7; \
		make install prefix=$(LIBS_INSTALL_PATH)/$(DBUS-PYTHON_PREFIX)
	echo "installing $(DBUS-PYTHON) done"
	@touch $@

#
# Update
#
.PHONY: dbus-python-update

dbus-python-update:
	@echo "Updating $(DBUS-PYTHON) ..."
	@if [ -d "$(DBUS-PYTHON_SOURCE)" ]; then\
		$(TOP_UPDATE) $(DBUS-PYTHON_SOURCE); \
	fi
	@echo "Updating $(DBUS-PYTHON) done"

#
# Status
#
.PHONY: dbus-python-status

dbus-python-status:
	@echo "Statusing $(DBUS-PYTHON) ..."
	@if [ -d "$(DBUS-PYTHON_SOURCE)" ]; then\
		$(TOP_STATUS) $(DBUS-PYTHON_SOURCE); \
	fi
	@echo "Statusing $(DBUS-PYTHON) done"

#
# Clean
#
.PHONY: dbus-python-clean

dbus-python-clean:
	@rm -f $(DBUS-PYTHON_DIR)/.dbus-python_configured
	@if [ -d "$(DBUS-PYTHON_SOURCE)" ]; then\
		make -C $(DBUS-PYTHON_SOURCE)/$(DBUS-PYTHON) clean; \
	fi

#
# Dist clean
#
.PHONY: dbus-python-distclean

dbus-python-distclean:
	@rm -f $(DBUS-PYTHON_DIR)/.dbus-python_*

#
# Install
#
.PHONY: dbus-python-install

dbus-python-install:


	
