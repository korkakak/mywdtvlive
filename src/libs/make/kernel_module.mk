##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  wills_yin@alphanetworks.com
# KERNEL_MODULE makefile fragments 
##############################################################################

#
# Defines
#

SIGMA_SDK_SERVER_URL            := $(TOP_LIBS_URL)/sigma_sdk/$(CONF_SIGMA_CHIP_NO)/$(CONF_SIGMA_SDK_VERSION)#
SIGMA_SDK_DIR                   := $(TOP_LIBS_DIR)#
SIGMA_SDK_SOURCE                := $(SIGMA_SDK_DIR)/$(CONF_SIGMA_SDK_VERSION)#
LINUX_KERNEL_SOURCE             := $(SIGMA_SDK_SOURCE)/linux_kernel_${CONF_LINUX_KERNEL_VERSION}#
LINUX_KERNEL_PATCH_SOURCE       := $(SIGMA_SDK_SOURCE)/linux_kernel_${CONF_LINUX_KERNEL_VERSION}_patch#
LINUX_KERNEL_SOURCE_PACKAGE     := $(LINUX_KERNEL_SOURCE).tar.gz#
LINUX_KERNEL_CONFIG             := $(TOP_CONFIG_DIR)/$(CONF_LINUX_KERNEL_CONFIG_FILE)


KERNEL_MODULE             := kernel_module#
KERNEL_MODULE_SERVER_URL  := $(TOP_LIBS_URL)/$(KERNEL_MODULE)/
KERNEL_MODULE_DIR         := $(TOP_LIBS_DIR)#
KERNEL_MODULE_SOURCE      := $(SIGMA_SDK_SOURCE)#
KERNEL_MODULE_CONFIG      := 


#
# Download  the source 
#
.PHONY: kernel_module-downloaded

kernel_module-downloaded: $(KERNEL_MODULE_DIR)/.kernel_module_downloaded

$(KERNEL_MODULE_DIR)/.kernel_module_downloaded:
	touch $@

#
# Unpack the source
#
.PHONY: kernel_module-unpacked

kernel_module-unpacked: $(KERNEL_MODULE_DIR)/.kernel_module_unpacked

$(KERNEL_MODULE_DIR)/.kernel_module_unpacked: $(KERNEL_MODULE_DIR)/.kernel_module_downloaded
	@if [  -d "$(KERNEL_MODULE_SOURCE)/proc_pipe" ]; then \
		rm -rf "$(KERNEL_MODULE_SOURCE)/proc_pipe";	\
	fi
	cd $(KERNEL_MODULE_SOURCE); 				\
	tar -zxvf $(KERNEL_MODULE_SOURCE)/proc_pipe.tgz;	
	@touch $@


#
# Patch the source
#
.PHONY: kernel_module-patched

kernel_module-patched: $(KERNEL_MODULE_DIR)/.kernel_module_patched

$(KERNEL_MODULE_DIR)/.kernel_module_patched: $(KERNEL_MODULE_DIR)/.kernel_module_unpacked
	touch $@


#
# config
#
.PHONY: kernel_module-config

kernel_module-config: 

#
# Configuration
#
.PHONY: kernel_module-configured

kernel_module-configured:$(KERNEL_MODULE_DIR)/.kernel_module_configured

$(KERNEL_MODULE_DIR)/.kernel_module_configured: $(KERNEL_MODULE_DIR)/.kernel_module_patched
	@echo "Configured $(KERNEL_MODULE) ..."
	@echo "Configured $(KERNEL_MODULE) done"
	@touch $@

#
# Compile
#
.PHONY: kernel_module-compile

kernel_module-compile: kernel_module-configured $(KERNEL_MODULE_DIR)/.kernel_module_compiled

$(KERNEL_MODULE_DIR)/.kernel_module_compiled:
	@echo "Compiling $(KERNEL_MODULE) ..."
	(cd $(KERNEL_MODULE_SOURCE)/; tar -zxvf proc_pipe.tgz; export RMCFLAGS='';export UCLINUX_KERNEL=$(LINUX_KERNEL_SOURCE)/ ; cd proc_pipe; make;)
	@echo "Compiling $(KERNEL_MODULE) done"
	@touch $@

#
# Update
#
.PHONY: kernel_module-update

kernel_module-update:
	@echo "Updating $(KERNEL_MODULE) done"


#
# Clean
#
.PHONY: kernel_module-clean

kernel_module-clean:
	@if [ -d "$(KERNEL_MODULE_SOURCE)" ]; then\
		make -C $(KERNEL_MODULE_SOURCE) clean; \
	fi

#
# Dist clean
#
.PHONY: kernel_module-distclean

kernel_module-distclean:
	rm -f $(TOP_LIBS_DIR)/.kernel_module*
	#rm -rf $(KERNEL_MODULE_SOURCE)

#
# Install
#
.PHONY: kernel_module-install

kernel_module-install:
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/lib/modules;								\
	cp -f $(KERNEL_MODULE_SOURCE)/proc_pipe/proc_pipe.ko $(TOP_INSTALL_ROOTFS_DIR)/lib/modules;			
	if [ -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/scsi/scsi_wait_scan.ko ]; then \
                cp -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/scsi/scsi_wait_scan.ko $(TOP_INSTALL_ROOTFS_DIR)/lib/modules/;\
        fi
	if [ -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/char/irkernel.ko ]; then \
                cp -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/char/irkernel.ko $(TOP_INSTALL_ROOTFS_DIR)/lib/modules/;\
        fi
	if [ -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/char/i2c_hal.ko ]; then \
                cp -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/char/i2c_hal.ko $(TOP_INSTALL_ROOTFS_DIR)/lib/modules/;\
        fi
	if [ -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/char/thermal_g751.ko ]; then \
                cp -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/char/thermal_g751.ko $(TOP_INSTALL_ROOTFS_DIR)/lib/modules/;\
        fi
	if [ -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/char/wdtv_rv_microp.ko ]; then \
                cp -f $(SIGMA_SDK_SOURCE)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/drivers/char/wdtv_rv_microp.ko $(TOP_INSTALL_ROOTFS_DIR)/lib/modules/;\
        fi
