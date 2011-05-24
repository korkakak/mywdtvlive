TOP = $(shell pwd)
export TOP

PREREQS	:= init
SUBDIRS = src

include $(TOP)/Target.mk
ifneq (x$(TOP_SCRIPTS_DIR),x)
include $(TOP_SCRIPTS_DIR)/rules.mk
endif

.PHONY: init
init: .config Target.h Target.options 

scripts/kconfig/mconf: 
	make -C scripts/kconfig/

scripts/kconfig/lxdialog/lxdialog:
	make -C scripts/kconfig/lxdialog

.config:
	@if [ ! -f .config ]; then	\
		echo "";\
		echo "Use default configuration";	\
		echo "";\
		echo "	1. WDTV x86 (WD UI)";\
		echo "	2. WDTV_VILLA_smp8654_WD_UI (256MB flash)";\
		echo "	3. WDTV_BUNGALOW_smp8654_WD_UI (Netflix)";\
		echo "	4. WDTV_RV With alpha board";\
		echo "	5. LaCinema Mediaplayer ";\
		echo "	6. LaCinema DMA (with miniPCI) ";\
		echo "	7. IODATA_AVLS700L_smp8654_UI";\
		echo "	8. GOOGLE_UNO_smp8652_UI";\
		echo "";\
	fi;
	@read -p "Please select a default config: ";\
	if [ x$$REPLY == x1 ]; then	\
		cp config/default_config_WDTV_x86 .config;\
	fi;\
	if [ x$$REPLY == x2 ]; then	\
		cp config/default_config_WDTV_VILLA_smp8654_WD_UI_D2 .config;\
	fi;\
	if [ x$$REPLY == x3 ]; then	\
		cp config/default_config_WDTV_BUNGALOW_smp8654_WD_UI_D2 .config;\
	fi;\
	if [ x$$REPLY == x4 ]; then	\
		cp config/default_config_WDTV_RV_smp8654_ALPHA_BOARD .config;\
	fi;\
	if [ x$$REPLY == x5 ]; then	\
		cp config/default_config_LMP555 .config;\
	fi;\
	if [ x$$REPLY == x6 ]; then	\
		cp config/default_config_LMP555_DMA .config;\
	fi;\
	if [ x$$REPLY == x7 ]; then	\
		cp config/default_config_AV-LS700L_UI .config;\
	fi;\
	if [ x$$REPLY == x8 ]; then	\
		cp config/default_config_GOOGLE_UNO_smp8652_UI_D2 .config;\
	fi;\
	sed '/SECURITY_USE_CPU_BINDING/d' .config > .config.tmp && mv -f .config.tmp .config; \
	sed '/SECURITY_PROPRIETARY_ENCRYPT/d' .config > .config.tmp && mv -f .config.tmp .config; \
	sed '/BOOT_AUTH/d' .config > .config.tmp && mv -f .config.tmp .config; \
	make oldconfig

include $(TOP_SCRIPTS_DIR)/Target.mk

.PHONY: oldconfig
oldconfig:
	./scripts/kconfig/conf -o Kconfig 
	make Target.h 
	make Target.options

.PHONY: menuconfig
menuconfig: init
	./scripts/kconfig/mconf Kconfig 
	make Target.h 
	make Target.options

.PHONY:	install
install:
	rm -rf $(TOP_INSTALL_ROOTFS_DIR)
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)
	rm -rf $(TOP_INSTALL_ROOTFS_PRIMARY_SYSTEM_DIR)
	mkdir -p $(TOP_INSTALL_ROOTFS_PRIMARY_SYSTEM_DIR)
	make -C $(TOP_MISC_DIR) install
	make -C $(TOP_LIBS_DIR) install

rootfs: init
	make install

update up: init
	@echo "Updating the core_repos ..."	
	$(TOP_UPDATE);
	@echo "Updating the core_repos done"
	
	@echo "Updating the external libs ..."
	make -C $(TOP_LIBS_DIR) update
	@echo "Updating the external libs done" 

status st: init
	@$(TOP_STATUS);
	@make -C $(TOP_LIBS_DIR) status

ifneq (x$(CONF_FIRMWARE_SCRIPT),x)
include $(TOP_SCRIPTS_DIR)/$(CONF_FIRMWARE_SCRIPT)
endif


