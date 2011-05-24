
#
# Defines
#
ifndef  CONF_AVAHI_VERSION 
CONF_AVAHI_VERSION := 0.6.27# 
endif

AVAHI             := avahi-$(CONF_AVAHI_VERSION)#
AVAHI_SERVER_URL  := $(TOP_LIBS_URL)/avahi/$(AVAHI)
AVAHI_DIR         := $(TOP_LIBS_DIR)#
AVAHI_SOURCE      := $(AVAHI_DIR)/$(AVAHI)
AVAHI_CONFIG      := 


#
# Download  the source 
#
.PHONY: avahi-downloaded

avahi-downloaded: $(AVAHI_DIR)/.avahi_downloaded

$(AVAHI_DIR)/.avahi_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: avahi-unpacked

avahi-unpacked: $(AVAHI_DIR)/.avahi_unpacked

$(AVAHI_DIR)/.avahi_unpacked: $(AVAHI_DIR)/.avahi_downloaded $(AVAHI_SOURCE)/$(AVAHI).tar.gz
	@echo "Unpacking $(AVAHI) ..."
	if [ -d "$(AVAHI_SOURCE)/$(AVAHI)" ]; then \
		rm -fr $(AVAHI_SOURCE)/$(AVAHI); \
	fi
	cd $(AVAHI_SOURCE); tar zxvf $(AVAHI).tar.gz
	find $(AVAHI_SOURCE)/$(AVAHI) -exec touch {} \;
	@echo "Unpacking $(AVAHI) done"
	@touch $@


#
# Patch the source
#
.PHONY: avahi-patched

avahi-patched: $(AVAHI_DIR)/.avahi_patched

$(AVAHI_DIR)/.avahi_patched: $(AVAHI_DIR)/.avahi_unpacked
	#@( automake --version | grep 1.11.1; \
	#	if [ $$? != "0" ]; then \
	#		echo "You must install automake version 1.11.1 in order to compile avahi."; \
	#		false; \
	#	fi; )
	#sed -i -e s,-DDEBUG_TRAP=__asm__\(\"int\ \$$\$$\3\"\),, `find $(AVAHI_SOURCE)/$(AVAHI) -name Makefile.am` ;
	#cd $(AVAHI_SOURCE)/$(AVAHI); automake;
	#cd $(AVAHI_SOURCE)/$(AVAHI); patch -p0 < $(AVAHI_SOURCE)/avahi.patch;
	@touch $@

#
# config
#
.PHONY: avahi-config

avahi-config: 

#
# Configuration
#
.PHONY: avahi-configured

avahi-configured:  avahi-config $(AVAHI_DIR)/.avahi_configured

$(AVAHI_DIR)/.avahi_configured: $(AVAHI_DIR)/.avahi_patched $(AVAHI_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(AVAHI) ..."
	cd $(AVAHI_SOURCE)/$(AVAHI);\
		PKG_CONFIG_PATH=$(LIBS_INSTALL_PATH)/opt/lib/pkgconfig/:$(LIBS_INSTALL_PATH)/lib/pkgconfig \
		CFLAGS="-I$(LIBS_INSTALL_PATH)/include " \
		LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib " \
		LIBDAEMON_CFLAGS="-I$(LIBS_INSTALL_PATH)/include " \
		LIBDAEMON_LIBS="-L$(LIBS_INSTALL_PATH)/lib -ldaemon " \
		DBUS_CFLAGS="-I$(LIBS_INSTALL_PATH)/opt/include/dbus-1.0/ -DHAVE_DBUS_CONNECTION_CLOSE " \
		DBUS_LIBS="-L$(LIBS_INSTALL_PATH)/opt/lib -ldbus-1" \
		./configure --prefix=$(LIBS_INSTALL_PATH) \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--with-distro=none \
		--disable-qt3 \
		--disable-qt4 \
		--disable-gtk \
		--disable-gtk3 \
		--with-xml=expat \
		--enable-libdaemon \
		--disable-monodoc \
		--disable-mono \
		--disable-gdbm \
		--enable-dbus \
		--disable-python \
		--disable-pygtk \
		--disable-python-dbus \
		--disable-glib \
		--disable-gobject \
		--enable-shared=yes
	@echo "Configuring $(AVAHI) done"
	@touch $@

#
# Compile
#
.PHONY: avahi-compile

avahi-compile: $(AVAHI_DIR)/.avahi_compiled

$(AVAHI_DIR)/.avahi_compiled: $(AVAHI_DIR)/.avahi_configured
	@echo "Compiling $(AVAHI) ..."
	#make -C $(AVAHI_SOURCE)/$(AVAHI) CC=$(CC) AR=$(AR) RANLIB=$(RANLIB) STRIP=$(CROSS)strip LD=$(LD)
	PKG_CONFIG_PATH=$(LIBS_INSTALL_PATH)/opt/lib/pkgconfig/:$(LIBS_INSTALL_PATH)/lib/pkgconfig \
	CFLAGS="-I$(LIBS_INSTALL_PATH)/include " \
	LDFLAGS="-L$(LIBS_INSTALL_PATH)/lib " \
	LIBDAEMON_CFLAGS="-I$(LIBS_INSTALL_PATH)/include " \
	LIBDAEMON_LIBS="-L$(LIBS_INSTALL_PATH)/lib -ldaemon " \
	DBUS_CFLAGS="-I$(LIBS_INSTALL_PATH)/opt/include/dbus-1.0/ -DHAVE_DBUS_CONNECTION_CLOSE " \
	DBUS_LIBS="-L$(LIBS_INSTALL_PATH)/opt/lib -ldbus-1" \
	make -C $(AVAHI_SOURCE)/$(AVAHI)  install
	@echo "Compiling $(AVAHI) done"
	@touch $@

#
# Update
#
.PHONY: avahi-update

avahi-update:
	@echo "Updating $(AVAHI) ..."
	@if [ -d "$(AVAHI_SOURCE)" ]; then\
		$(TOP_UPDATE) $(AVAHI_SOURCE); \
	fi
	@echo "Updating $(AVAHI) done"

#
# Status
#
.PHONY: avahi-status

avahi-status:
	@echo "Statusing $(AVAHI) ..."
	@if [ -d "$(AVAHI_SOURCE)" ]; then\
		$(TOP_STATUS) $(AVAHI_SOURCE); \
	fi
	@echo "Statusing $(AVAHI) done"

#
# Clean
#
.PHONY: avahi-clean

avahi-clean:
	@rm -f $(AVAHI_DIR)/.avahi_configured
	@if [ -d "$(AVAHI_SOURCE)" ]; then\
		make -C $(AVAHI_SOURCE)/$(AVAHI) clean; \
	fi

#
# Dist clean
#
.PHONY: avahi-distclean

avahi-distclean:
	@rm -f $(AVAHI_DIR)/.avahi_*

#
# Install
#
.PHONY: avahi-install

avahi-install:
	cp -af $(LIBS_INSTALL_PATH)/lib/libavahi-c*so* $(TOP_INSTALL_ROOTFS_DIR)/lib

