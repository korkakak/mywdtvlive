#
# Defines
#
ifndef  CONF_QT_VERSION 
CONF_QT_VERSION := 4.5.0-1.2
endif

ifndef  CONF_DCCHD_VERSION
	CONF_DCCHD_VERSION := SMP8652_3_8_1_rc_1_aqua.mips
endif

$(warning $(CONF_QT_VERSION))
$(warning $(CONF_DCCHD_VERSION))
QT_SERVER_URL      := $(TOP_LIBS_URL)/qt/qt_SMP86xx_src_$(CONF_QT_VERSION)#

QT_BUILD_DIR       := $(TOP_LIBS_DIR)/3_8_0/qt#

QT_DIR             := $(QT_BUILD_DIR)/qt_SMP86xx_src_$(CONF_QT_VERSION)#
QT_SOURCE_DIR      := $(QT_BUILD_DIR)/qt_SMP86xx_src_$(CONF_QT_VERSION)#
QT_PATCH_DIR       := $(QT_BUILD_DIR)/patch#
QT_CONFIG_DIR      := $(QT_DIR)/config#
QT_ETC_DIR         := $(QT_DIR)/etc#

DCCHD_DIR          := $(TOP_LIBS_DIR)/3_8_0/DCCHD/dcchd_$(CONF_DCCHD_VERSION)/dcchd#
DCCHD_CFG_DIR      := $(TOP_LIBS_DIR)/3_8_0/DCCHD/dcchd_$(CONF_DCCHD_VERSION)/dcchd/config/default#
DIRECTFB_DIR       := $(TOP_LIBS_DIR)/3_8_0/DCCHD/dcchd_$(CONF_DCCHD_VERSION)/directfb#
DIRECTFB_INSTALL_DIR    := $(TOP_LIBS_DIR)/3_8_0/DCCHD/dcchd_$(CONF_DCCHD_VERSION)/directfb#
DCCHD_TOOLCHAIN_DIR     := $(TOP_LIBS_DIR)/toolchain#

#
# Download  the source 
#
.PHONY: qt-download

qt-download: 

#
# Unpack the source
#
.PHONY: qt-unpack

qt-unpack: $(TOP_LIBS_DIR)/.qt_unpacked

$(TOP_LIBS_DIR)/.qt_unpacked: 
	@echo "Unpacking the ${QT_BUILD_DIR}/qt_SMP86xx_src_${CONF_QT_VERSION} package ......"
	# Remove the source file firstly
	@rm -fr $(QT_SOURCE_DIR)
	@(cd ${QT_BUILD_DIR}; tar jxf qt_SMP86xx_src_${CONF_QT_VERSION}.tar.bz2)
	@( [ -d ${QT_SOURCE_DIR}/src ] || (zcat ${QT_BUILD_DIR}/qt-embedded-linux-opensource-src-4.5.0.tar.gz | tar -C ${QT_SOURCE_DIR} -xf -;) )
	@echo "Unpacking $(CONF_QT_VERSION) done"
	@touch $@

#
# Patch the source
#
.PHONY: qt-patch

qt-patch: $(TOP_LIBS_DIR)/.qt_patched

$(TOP_LIBS_DIR)/.qt_patched: $(TOP_LIBS_DIR)/.qt_unpacked
	@echo "Patching the qt_SMP86xx_src_${CONF_QT_VERSION} package ......"
	(cd ${QT_SOURCE_DIR}; patch -p1 < ${QT_PATCH_DIR}/qt_smp86xx_src_$(CONF_QT_VERSION)_config.patch)
ifeq ($(CONF_QT_VERSION),4.5.0-1.2)
	(cd ${QT_SOURCE_DIR}; patch -p0 < ${QT_PATCH_DIR}/qt-embedded-linux-opensource-src-4.5.0_demo_browser.patch)
	(cd ${QT_SOURCE_DIR}; patch -p0 < ${QT_PATCH_DIR}/qt-embedded-linux-opensource-src-4.5.0_linuxfb.patch)
	(cd ${QT_SOURCE_DIR}; mv qt-embedded-linux-opensource-src-4.5.0 src; patch -p0 < QT_4.5.0_to_Sigma.patch)
endif
# Todo : need to support font in the future
#	(tar xzvf $(QT_PATCH_DIR)/arphic-fontplugin.tgz -C $(QT_SOURCE_DIR)/src/src/plugins/)
#	(echo "SUBDIRS *= fontplugins" >> $(QT_SOURCE_DIR)/src/src/plugins/plugins.pro)
	@echo "Patching $(CONF_QT_VERSION) done"
	@touch $@

#
# config
#
.PHONY: qt-config

qt-config: 

#
# Configuration
#
.PHONY: qt-configure

qt-configure:  qt-config $(TOP_LIBS_DIR)/.qt_configured

$(TOP_LIBS_DIR)/.qt_configured: $(TOP_LIBS_DIR)/.qt_patched $(QT_CONFIG) $(TOP_CURRENT_SET)
	# Configuring ...
	@touch $@


#
# Compile
#
.PHONY: qt-compile

DCCHD_PATH := $(DCCHD_TOOLCHAIN_DIR)/bin:$(PATH)

qt-compile: $(TOP_LIBS_DIR)/.qt_compiled

QT_PATH := $(QT_TOOLCHAIN_DIR)/bin:$(PATH)
$(TOP_LIBS_DIR)/.qt_compiled: $(TOP_LIBS_DIR)/.qt_configured
	@echo "Compiling the  -I$(SYSLIB_PREFIX)  qt_SMP86xx_src_${CONF_QT_VERSION} package ......"
	( \
		export PATH=$(DCCHD_PATH); \
		rm -rf $(SYSLIB_PREFIX)/cross_rootfs/lib;ln -s $(SYSLIB_PREFIX)/lib $(SYSLIB_PREFIX)/cross_rootfs;\
		unset AS LD CC CPP CXX AR NM STRIP OBJCOPY OBJDUMP RANLIB MAKELIB LINKER;\
		export AS LD CC CPP CXX AR NM STRIP OBJCOPY OBJDUMP RANLIB MAKELIB LINKER;\
		export EXTRA_INCDIR="$(SYSLIB_PREFIX)/include/freetype2 -I$(SYSLIB_PREFIX)/include/freetype2/freetype -I$(SYSLIB_PREFIX)/include -I$(SYSLIB_PREFIX)/include/openssl -I$(SYSLIB_PREFIX)/include -I$(SYSLIB_PREFIX)/opt/include/  -I$(SYSLIB_PREFIX)/opt/include/dbus-1.0 -I$(SYSLIB_PREFIX)/include/glib-2.0 -I$(SYSLIB_PREFIX)/lib/glib-2.0/include"; \
		export EXTRA_LIBDIR="$(SYSLIB_PREFIX)/lib -L$(SYSLIB_PREFIX)/opt/lib"; \
		export RUA_DIR="$(SIGMA_SDK_SOURCE)/$(CONF_SIGMA_RUA_SDK)/MRUA_src"; \
		export DCCHD_DIR=${DCCHD_DIR}; \
		export DCCHD_CFG_DIR=${DCCHD_CFG_DIR}; \
		export DIRECTFB_DIR=${DIRECTFB_DIR}; \
		export DIRECTFB_INSTALL_DIR=${DIRECTFB_INSTALL_DIR}; \
		export QT_DIR=${QT_SOURCE_DIR}; \
		export QT_INSTALL_DIR=${SYSLIB_PREFIX}; \
		export COMPILKIND='codesourcery hardfloat glibc release';\
		export SMP86XX_ROOTFS_PATH=$(SYSLIB_PREFIX);\
		export SMP86XX_TOOLCHAIN_PATH=$(CONF_CROSS_PREFIX); \
		export QT_BUILD_TYPE=release; \
		cd ${QT_SOURCE_DIR}; source ./qt.env;  make -C ${QT_SOURCE_DIR} getqt=no \
	)
	@cp ${QT_SOURCE_DIR}/build-release/plugins/gfxdrivers/* $(SYSLIB_PREFIX)/lib

	@touch $@

#
# Update
#
.PHONY: qt-update

qt-update:
	@echo "Updating $(CONF_QT_VERSION) ..."
	@if [ -d "$(QT_DIR)" ]; then\
		$(TOP_UPDATE) $(QT_DIR); \
	fi
	@echo "Updating $(CONF_QT_VERSION) done"

#
# Status
#
.PHONY: qt-status

qt-status:
	@echo "Statusing $(QT) ..."
	@if [ -d "$(QT_DIR)" ]; then\
		$(TOP_STATUS) $(QT_DIR); \
	fi
	@echo "Statusing $(QT) done"

#
# Clean
#
.PHONY: qt-clean

qt-clean:
	@rm -f $(TOP_LIBS_DIR)/.qt_configured

#
# Dist clean
#
.PHONY: qt-distclean

qt-distclean:
	@rm -f $(TOP_LIBS_DIR)/.qt_*
	@rm -fr $(QT_DIR) $(QT_TOOLCHAIN_DIR)

#
# Install
#
.PHONY: qt-install

qt-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/qt/
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/qt/bin
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/qt/lib
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/bin/
	@mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/usr/lib	
	#@cp -af $(SYSLIB_PREFIX)/lib/libQt*so* $(TOP_INSTALL_ROOTFS_DIR)/opt/qt/lib
	#@cp -af $(SYSLIB_PREFIX)/lib/libQtGui*so* $(TOP_INSTALL_ROOTFS_DIR)/usr/lib
	@cp -af $(SYSLIB_PREFIX)/lib/libQtNetwork*so* $(TOP_INSTALL_ROOTFS_DIR)/lib
#	@cp -af $(SYSLIB_PREFIX)/lib/libQtDBus*so* $(TOP_INSTALL_ROOTFS_DIR)/lib	# We don't need Dbus anymore
	@cp -af $(SYSLIB_PREFIX)/lib/libQtWebKit*so* $(TOP_INSTALL_ROOTFS_DIR)/lib
ifeq (x$(CONF_QT), xy)
	@cp -af $(BUILD_ROOT)/lib/libbrowser*so* $(TOP_INSTALL_ROOTFS_DIR)/lib
endif
	@cp -af $(SYSLIB_PREFIX)/plugins $(TOP_INSTALL_ROOTFS_DIR)/opt/qt
	@cp -af $(QT_BUILD_DIR)/run_qt $(TOP_INSTALL_ROOTFS_DIR)/opt/qt/bin
	@cp -af $(QT_SOURCE_DIR)/build-release/demos/browser $(TOP_INSTALL_ROOTFS_DIR)/opt/qt/bin
	#@cp -rf $(QT_BUILD_DIR)/lib/libfont $(TOP_INSTALL_ROOTFS_DIR)/opt/qt/
	#@cp -af ${QT_BUILD_DIR}/lib/*so* $(SYSLIB_PREFIX)/lib
	@cp -af $(QT_PATCH_DIR)/libfont $(TOP_INSTALL_ROOTFS_DIR)/opt/qt/
	@cp -af $(QT_PATCH_DIR)/libfontconfig.so.1.4.4 $(TOP_INSTALL_ROOTFS_DIR)/lib
	(cd $(SYSLIB_PREFIX)/lib; ln -sf libfontconfig.so.1.4.4 libfontconfig.so; ln -sf libfontconfig.so.1.4.4 libfontconfig.so.1)
	@cp -af $(QT_PATCH_DIR)/libflashplayer.so $(TOP_INSTALL_ROOTFS_DIR)/lib  &&  mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/lib/adobe  &&  cd $(TOP_INSTALL_ROOTFS_DIR)/lib/adobe  &&  ln -fs ../libflashplayer.so libflashplayer.so

