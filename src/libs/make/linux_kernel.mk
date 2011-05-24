#
# Defines
#
ifndef  CONF_SIGMA_SDK_VERSION 
CONF_SIGMA_SDK_VERSION := 2.8.3.0# 
endif

SIGMA_SDK_SERVER_URL            := $(TOP_LIBS_URL)/sigma_sdk/$(CONF_SIGMA_CHIP_NO)/$(CONF_SIGMA_SDK_VERSION)#
SIGMA_SDK_DIR                   := $(TOP_LIBS_DIR)#
SIGMA_SDK_SOURCE                := $(SIGMA_SDK_DIR)/$(CONF_SIGMA_SDK_VERSION)#
LINUX_KERNEL_SOURCE             := $(SIGMA_SDK_SOURCE)/linux_kernel_${CONF_LINUX_KERNEL_VERSION}#
LINUX_KERNEL_PATCH_SOURCE       := $(SIGMA_SDK_SOURCE)/linux_kernel_${CONF_LINUX_KERNEL_VERSION}_patch#
LINUX_KERNEL_SOURCE_PACKAGE     := $(LINUX_KERNEL_SOURCE).tar.gz#
LINUX_KERNEL_CONFIG             := $(TOP_CONFIG_DIR)/$(CONF_LINUX_KERNEL_CONFIG_FILE)


#
# Download  the source 
#
.PHONY: linux_kernel-downloaded
linux_kernel-downloaded:

#
# Unpack the source
#
.PHONY: linux_kernel-unpacked

linux_kernel-unpacked: $(SIGMA_SDK_DIR)/.linux_kernel_unpacked

$(SIGMA_SDK_DIR)/.linux_kernel_unpacked:
	@touch $@


#
# Patch the source
#
.PHONY: linux_kernel-patched

linux_kernel-patched: $(SIGMA_SDK_DIR)/.linux_kernel_patched

$(SIGMA_SDK_DIR)/.linux_kernel_patched: $(SIGMA_SDK_DIR)/.linux_kernel_unpacked
	echo "Patching $@ done"
	@touch $@
#
# config
#
.PHONY: linux_kernel-config

linux_kernel-config: linux_kernel-patched 

#
# Configuration
#
.PHONY: linux_kernel-configured

linux_kernel-configured:  linux_kernel-config $(SIGMA_SDK_DIR)/.linux_kernel_configured

$(SIGMA_SDK_DIR)/.linux_kernel_configured: $(SIGMA_SDK_DIR)/.linux_kernel_patched $(SIGMA_SDK_CONFIG) $(TOP_CURRENT_SET)
	@echo "apply kernel config..."
	-make -C $(LINUX_KERNEL_SOURCE) oldconfig 
	@touch $@

#
# Compile
#
.PHONY: linux_kernel-compile

linux_kernel-compile: linux_kernel-configured $(SIGMA_SDK_DIR)/.linux_kernel_compiled

$(SIGMA_SDK_DIR)/.linux_kernel_compiled:
	@echo "Compiling kernel..."
	-mkdir -p $(LINUX_KERNEL_SOURCE)/initramfs_tmp
	-(export INITRAMFS_PATH=$(LINUX_KERNEL_SOURCE)/initramfs_tmp; make -C $(LINUX_KERNEL_SOURCE))
	-(export INITRAMFS_PATH=$(LINUX_KERNEL_SOURCE)/initramfs_tmp; make -C $(LINUX_KERNEL_SOURCE) modules)
	touch $@

#
# Update
#
.PHONY: linux_kernel-update

linux_kernel-update:

#
# Status
#
.PHONY: linux_kernel-status

linux_kernel-status:

#
# Clean
#
.PHONY: linux_kernel-clean

linux_kernel-clean:
	@rm -f .linux_kernel_configured
	@make -C $(LINUX_KERNEL_SOURCE) clean   
	rm -f $(SIGMA_SDK_DIR)/.linux_kernel_compiled
	

#
# Dist clean
#
.PHONY: linux_kernel-distclean

linux_kernel-distclean:
	rm -f .linux*
	@make -C $(LINUX_KERNEL_SOURCE) clean   
#
# Install
#
.PHONY: linux_kernel-install

linux_kernel-install:
	cd $(LINUX_KERNEL_SOURCE); make modules_install INSTALL_MOD_PATH=$(TOP_INSTALL_ROOTFS_DIR);	\
	if [ "x$(CONF_NFS_DEBUG)" == "xy" ]; then	\
		echo "Install modules for NFS debug....";	\
		#cd $(LINUX_KERNEL_SOURCE); make modules_install INSTALL_MOD_PATH=$(TOP_INSTALL_ROOTFS_DIR);	\
		touch $(TOP_INSTALL_ROOTFS_DIR)/nfs_debug;	\
	fi


