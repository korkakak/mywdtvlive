#
# Defines
#
PYGOBJECT_VERSION := 2.21.5#

PYGOBJECT             := pygobject-$(PYGOBJECT_VERSION)#
PYGOBJECT_SERVER_URL  := $(TOP_LIBS_URL)/pygobject/$(PYGOBJECT)
PYGOBJECT_DIR         := $(TOP_LIBS_DIR)#
PYGOBJECT_SOURCE      := $(PYGOBJECT_DIR)/$(PYGOBJECT)
PYGOBJECT_CONFIG      := 
PYGOBJECT_PATCH       := $(PYGOBJECT_SOURCE)/$(PYGOBJECT).patch

PYGOBJECT_PREFIX	:= opt#

PYGOBJECT_ENABLE_SHLIB	:= no
ifeq (x$(PYGOBJECT_ENABLE_SHLIB),xno)
	PYGOBJECT_SHARED_FLAGS      := --enable-shared=no
endif

#
# Download  the source 
#
.PHONY: pygobject-downloaded

pygobject-downloaded: $(PYGOBJECT_DIR)/.pygobject_downloaded

$(PYGOBJECT_DIR)/.pygobject_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: pygobject-unpacked

pygobject-unpacked: $(PYGOBJECT_DIR)/.pygobject_unpacked

$(PYGOBJECT_DIR)/.pygobject_unpacked: $(PYGOBJECT_DIR)/.pygobject_downloaded $(wildcard $(PYGOBJECT_SOURCE)/*.tgz)
	echo "Unpacking $(PYGOBJECT) ..."
	if [ -d "$(PYGOBJECT_SOURCE)/$(PYGOBJECT)" ]; then \
		rm -fr $(PYGOBJECT_SOURCE)/$(PYGOBJECT); \
	fi
	cd $(PYGOBJECT_SOURCE); tar zxvf $(PYGOBJECT).tar.gz
	@echo "Unpacking $(PYGOBJECT) done"
	@touch $@

#
# Patch the source
#
.PHONY: pygobject-patched

pygobject-patched: $(PYGOBJECT_DIR)/.pygobject-patched

$(PYGOBJECT_DIR)/.pygobject_patched: $(PYGOBJECT_DIR)/.pygobject_unpacked
	cd $(PYGOBJECT_SOURCE)/$(PYGOBJECT); patch  -p1 < $(PYGOBJECT_PATCH)
	@touch $@

#
# config
#
.PHONY: readline-config

readline-config: 


#
# Configuration
#
.PHONY: pygobject-configured

pygobject-configured:  pygobject-configured $(PYGOBJECT_DIR)/.pygobject_configured

$(PYGOBJECT_DIR)/.pygobject_configured: $(PYGOBJECT_DIR)/.pygobject_patched $(PYGOBJECT_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(PYGOBJECT) ..."
	export PYTHON=$(LIBS_INSTALL_PATH)/opt/bin/python2.7; \
	export HOST_PYTHON=/usr/bin/python2.7; \
	cd $(PYGOBJECT_SOURCE)/$(PYGOBJECT); \
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
	@echo "Configuring $(PYGOBJECT) done"
	@touch $@

	#GI_CFLAGS='-pthread -I/$(LIBS_INSTALL_PATH)/local/include/gobject-introspection-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include  '
	#GI_DATADIR='/usr/local/share/gobject-introspection-1.0'
#
# Compile
#
.PHONY: pygobject-compile

pygobject-compile: $(PYGOBJECT_DIR)/.pygobject_compiled

$(PYGOBJECT_DIR)/.pygobject_compiled: $(PYGOBJECT_DIR)/.pygobject_configured 
	@echo "Compiling $(PYGOBJECT) ..."
	@cd $(PYGOBJECT_SOURCE)/$(PYGOBJECT); \
		make GLIB_CFLAGS="-pthread -I$(LIBS_INSTALL_PATH)/include/glib-2.0 -I$(LIBS_INSTALL_PATH)/lib/glib-2.0/include"
	echo "Compiling $(PYGOBJECT) done"
	@cd $(PYGOBJECT_SOURCE)/$(PYGOBJECT); \
		export HOST_PYTHON=/usr/bin/python2.7; \
		make install prefix=$(LIBS_INSTALL_PATH)/$(PYGOBJECT_PREFIX)
	echo "installing $(PYGOBJECT) done"
	@touch $@

#
# Update
#
.PHONY: pygobject-update

pygobject-update:
	@echo "Updating $(PYGOBJECT) ..."
	@if [ -d "$(PYGOBJECT_SOURCE)" ]; then\
		$(TOP_UPDATE) $(PYGOBJECT_SOURCE); \
	fi
	@echo "Updating $(PYGOBJECT) done"

#
# Status
#
.PHONY: pygobject-status

pygobject-status:
	@echo "Statusing $(PYGOBJECT) ..."
	@if [ -d "$(PYGOBJECT_SOURCE)" ]; then\
		$(TOP_STATUS) $(PYGOBJECT_SOURCE); \
	fi
	@echo "Statusing $(PYGOBJECT) done"

#
# Clean
#
.PHONY: pygobject-clean

pygobject-clean:
	@rm -f $(PYGOBJECT_DIR)/.pygobject_configured
	@if [ -d "$(PYGOBJECT_SOURCE)" ]; then\
		make -C $(PYGOBJECT_SOURCE)/$(PYGOBJECT) clean; \
	fi

#
# Dist clean
#
.PHONY: pygobject-distclean

pygobject-distclean:
	@rm -f $(PYGOBJECT_DIR)/.pygobject_*

#
# Install
#
.PHONY: pygobject-install

pygobject-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/lib
	cp -af $(LIBS_INSTALL_PATH)/$(PYGOBJECT_PREFIX)/lib/libpyg*so* $(TOP_INSTALL_ROOTFS_DIR)/opt/lib


	
