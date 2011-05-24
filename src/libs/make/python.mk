#
# Defines
#
PYTHON_VERSION := 2.7.1#

PYTHON             := Python-$(PYTHON_VERSION)#
PYTHON_SERVER_URL  := $(TOP_LIBS_URL)/Python/Python-$(PYTHON_VERSION)
PYTHON_DIR         := $(TOP_LIBS_DIR)#
PYTHON_SOURCE      := $(PYTHON_DIR)/Python-$(PYTHON_VERSION)
PYTHON_CONFIG      := 

PYTHON_PREFIX	:= opt#

PYTHON_ENABLE_SHLIB	:= no
ifeq (x$(PYTHON_ENABLE_SHLIB),xno)
	PYTHON_SHARED_FLAGS      := --enable-shared=no
endif

#
# Download  the source 
#
.PHONY: python-downloaded

python-downloaded: $(PYTHON_DIR)/.python_downloaded

$(PYTHON_DIR)/.python_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: python-unpacked

python-unpacked: $(PYTHON_DIR)/.python_unpacked

$(PYTHON_DIR)/.python_unpacked: $(PYTHON_DIR)/.python_downloaded $(wildcard $(PYTHON_SOURCE)/*.tgz) $(wildcard $(PYTHON_SOURCE)/*.patch)
	@echo "Unpacking $(PYTHON) ..."
	if [ -d "$(PYTHON_SOURCE)/$(PYTHON)" ]; then \
		rm -fr $(PYTHON_SOURCE)/$(PYTHON); \
	fi
	cd $(PYTHON_SOURCE); tar zxvf $(PYTHON).tgz
	@echo "Unpacking $(PYTHON) done"
	@touch $@

#
# config
#
.PHONY: python-config

python-config: $(PYTHON_DIR)/.python-config

#
# Patch the source
#
.PHONY: python-patched

python-patched: $(PYTHON_DIR)/.python-config

$(PYTHON_DIR)/.python_patched: $(PYTHON_DIR)/.python_unpacked 
	cd $(PYTHON_SOURCE)/$(PYTHON); CC=/usr/bin/gcc CXX=/usr/bin/g++ ./configure 
	cd $(PYTHON_SOURCE)/$(PYTHON); make
	cd $(PYTHON_SOURCE)/$(PYTHON); make install prefix=$(PYTHON_SOURCE)/$(PYTHON)/../_install_host
	cd $(PYTHON_SOURCE)/$(PYTHON); mv Parser/pgen ../hostpgen 
	@touch $@

#
# Configuration
#
.PHONY: python-configured

python-configured:  python-configured $(PYTHON_DIR)/.python_configured

$(PYTHON_DIR)/.python_configured: $(PYTHON_DIR)/.python_patched $(PYTHON_CONFIG) $(TOP_CURRENT_SET)
	rm -rf $(PYTHON_SOURCE)/$(PYTHON);
	cd $(PYTHON_SOURCE); tar zxvf $(PYTHON).tgz
	@echo "Patching $(PYTHON) ..."
	cd $(PYTHON_SOURCE)/$(PYTHON); patch -p1 < ../$(PYTHON)-xcompile.patch
	@echo "Patching $(PYTHON) done"
	@echo "Configuring $(PYTHON) ..."
	@cd $(PYTHON_SOURCE)/$(PYTHON); \
		CC=mipsel-linux-gcc \
		CXX=mipsel-linux-g++ \
		AR=mipsel-linux-ar \
		RANLIB=mipsel-linux-ranlib \
		./configure \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--target=$(BUILD_TARGET) \
		--prefix=$(LIBS_INSTALL_PATH)/$(PYTHON_PREFIX) \
		ac_cv_have_long_long_format=yes ;
	@echo "Configuring $(PYTHON) done"
	@touch $@

#
# Compile
#
.PHONY: python-compile

python-compile: $(PYTHON_DIR)/.python_compiled

$(PYTHON_DIR)/.python_compiled: $(PYTHON_DIR)/.python_configured 
	@echo "Compiling $(PYTHON) ..."
	@cd $(PYTHON_SOURCE)/$(PYTHON); \
		CC=mipsel-linux-gcc \
		CXX=mipsel-linux-g++ \
		AR=mipsel-linux-ar \
		RANLIB=mipsel-linux-ranlib \
		THIRD_PARTY_LIB=$(LIBS_INSTALL_PATH)/lib \
		THIRD_PARTY_INCLUDE=$(THIRD_PARTY_INCLUDE)/include \
		make
	@cd $(PYTHON_SOURCE)/$(PYTHON); \
		THIRD_PARTY_LIB=$(LIBS_INSTALL_PATH)/lib \
		THIRD_PARTY_INCLUDE=$(THIRD_PARTY_INCLUDE)/include \
		make install prefix=$(LIBS_INSTALL_PATH)/$(PYTHON_PREFIX)
	@echo "Compiling $(PYTHON) done"
	@touch $@

#
# Update
#
.PHONY: python-update

python-update:
	@echo "Updating $(PYTHON) ..."
	@if [ -d "$(PYTHON_SOURCE)" ]; then\
		$(TOP_UPDATE) $(PYTHON_SOURCE); \
	fi
	@echo "Updating $(PYTHON) done"

#
# Status
#
.PHONY: python-status

python-status:
	@echo "Statusing $(PYTHON) ..."
	@if [ -d "$(PYTHON_SOURCE)" ]; then\
		$(TOP_STATUS) $(PYTHON_SOURCE); \
	fi
	@echo "Statusing $(PYTHON) done"

#
# Clean
#
.PHONY: python-clean

python-clean:
	@rm -f $(PYTHON_DIR)/.python_configured
	@if [ -d "$(PYTHON_SOURCE)" ]; then\
		make -C $(PYTHON_SOURCE)/$(PYTHON) clean; \
	fi

#
# Dist clean
#
.PHONY: python-distclean

python-distclean:
	@rm -f $(PYTHON_DIR)/.python_*

#
# Install
#
.PHONY: python-install

python-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/bin
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/include
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/opt/lib
	cp -af $(LIBS_INSTALL_PATH)/opt/bin/python $(TOP_INSTALL_ROOTFS_DIR)/opt/bin
	cp -af $(LIBS_INSTALL_PATH)/opt/include/python2.7 $(TOP_INSTALL_ROOTFS_DIR)/opt/include
	cp -ra $(LIBS_INSTALL_PATH)/opt/lib/python2.7 $(TOP_INSTALL_ROOTFS_DIR)/opt/lib
	find $(TOP_INSTALL_ROOTFS_DIR)/opt/lib -name \*.py -exec rm -f {} \;
	find $(TOP_INSTALL_ROOTFS_DIR)/opt/lib -name \*.pyo -exec rm -f {} \;
	find $(TOP_INSTALL_ROOTFS_DIR)/opt/lib -name test -exec rm -rf {} \; ; true
	find $(TOP_INSTALL_ROOTFS_DIR)/opt/lib -name \*.a -exec rm -rf {} \;

	
