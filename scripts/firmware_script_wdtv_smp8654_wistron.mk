
ROMFS_BIN_NAME	:= romfs.bin

SIGMA_SDK_DIR         := $(TOP_LIBS_DIR)#
SIGMA_SDK_SOURCE      := $(SIGMA_SDK_DIR)/$(CONF_SIGMA_SDK_VERSION)#

ZBOOT_FILE            := $(TOP_BUILD_DIR)/zboot.bin
IMAT_FILE             := $(TOP_BUILD_DIR)/imat.bin
XMAT_FILE             := $(TOP_BUILD_DIR)/xmat.bin
KERNEL_FILE           := $(TOP_BUILD_DIR)/zbimage-linux-xload
FF_TMP_DIR            := $(TOP_BUILD_DIR)/full_firmware
PKG_TMP_DIR           := $(TOP_BUILD_DIR)/package

firmware: rootfs_for_application_system firmware_full generate_md5sum_info


flash_image: generate_16M_production_image generate_16M_f1_image

generate_16M_production_image:
	(cd $(TOP_BUILD_DIR);echo "okok" > okok;cat boot_loader /dev/zero|head -c $(CONF_BOOT_LOADER_SIZE) > 16M-flash-img-prod;cat primary_system.bin /dev/zero|head -c $(CONF_PRIMARY_SYSTEM_SIZE) >> 16M-flash-img-prod;cat $(ROMFS_BIN_NAME) /dev/zero|head -c `expr $(CONF_APP_ROMFS_SIZE) - 4` >> 16M-flash-img-prod;cat okok /dev/zero|head -c 262148 >> 16M-flash-img-prod;rm okok)

generate_16M_f1_image:
	(cd $(TOP_BUILD_DIR);echo "f1f1" > f1f1;cat boot_loader /dev/zero|head -c $(CONF_BOOT_LOADER_SIZE) > 16M-flash-img-f1;cat primary_system.bin /dev/zero|head -c $(CONF_PRIMARY_SYSTEM_SIZE) >> 16M-flash-img-f1;cat romfs.bin /dev/zero|head -c `expr $(CONF_APP_ROMFS_SIZE) - 4` >> 16M-flash-img-f1;cat f1f1 /dev/zero|head -c 262148 >> 16M-flash-img-f1;rm f1f1)

firmware_full:
	@echo -en "\n\\033[0;33m Createing full firmware... \\033[0m\n"	
	rm -rf $(FF_TMP_DIR);
	mkdir -p $(FF_TMP_DIR); mkdir -p $(FF_TMP_DIR)/dev;
	echo VERSION=\'$(CONF_VERSION)\' > $(FF_TMP_DIR)/version;
	# Todo : file size check
	if [ -f $(ZBOOT_FILE) ] && [ x$(CONF_BOOTLOADER_UPGRADE) == xy ]; then \
		cp -vf $(ZBOOT_FILE) $(FF_TMP_DIR)/$(CONF_BOOTLOADER_MTD_PARTITION).bin; \
	fi
	if [ -f $(KERNEL_FILE) ]; then \
		md5sum $(KERNEL_FILE) | head -c 32  > $(FF_TMP_DIR)/md5_kernel; \
		cp -vf $(KERNEL_FILE) $(FF_TMP_DIR)/$(CONF_KERNEL_MTD_PARTITION).bin; \
		if [ x$(CONF_KERNEL_MTD_MIRROR_PARTITION) != x ]; then \
			(cd $(FF_TMP_DIR) && ln -s ../$(CONF_KERNEL_MTD_PARTITION).bin ./$(CONF_KERNEL_MTD_MIRROR_PARTITION).bin); \
		fi; \
	fi
	if [ -f $(IMAT_FILE) ]; then \
		cp -vf $(IMAT_FILE) $(FF_TMP_DIR)/$(CONF_MTD_IMAT_PARTITION).bin; \
	fi
	if [ -f $(XMAT_FILE) ]; then \
		cp -vf $(XMAT_FILE) $(FF_TMP_DIR)/$(CONF_MTD_XMAT_PARTITION).bin; \
	fi
	genromfs -d $(FF_TMP_DIR) -f $(TOP_BUILD_DIR)/$(CONF_FIRMWARE_FULL_FILE)
	rm -rf $(FF_TMP_DIR)
	@echo -en "\n\\033[0;33m Createing full firmware... Done \\033[0m\n"	

component_check:
	if [ ! -f $(TOP_BUILD_DIR)/zboot2.bin ] || [ ! -f $(TOP_BUILD_DIR)/zbimage-yamon-2.11 ]; then \
		echo "ERROR: =You have to build zboot and yamon...";\
		false;\
	fi
	if [ x$(CONF_BUILD_ROOTFS_PRIMARY_SYSTEM) != xy ] || [ x$(CONF_BUILD_ROOTFS) != xy ]; then	\
		echo "ERROR: You have to build rootfs and rootfs of primary system to make a firmware...";\
		false;\
	fi
	echo "Build rootfs of primary system...";\
	if [ ! -d $(TOP_LIBS_DIR)/$(CONF_SIGMA_SDK_VERSION)/${CONF_SIGMA_SDK_CPU_KEY} ]; then	\
		echo "ERROR: CPU key directory not found..."	;\
		false;\
	fi;\
	if [ ! -d $(TOP_LIBS_DIR)/$(CONF_SIGMA_SDK_VERSION)/linux_kernel_${CONF_LINUX_KERNEL_VERSION} ]; then \
		echo "ERROR: There is no linux kernel...";\
		false;\
	fi;\
	if [ ! -d $(TOP_LIBS_DIR)/$(CONF_RESOURCE)/splash_screen/8654 ]; then	\
		echo "ERROR: No splash screen found...";\
		false;\
	fi;\
	if [ ! -d $(TOP_INSTALL_ROOTFS_PRIMARY_SYSTEM_DIR) ]; then \
		echo "ERROR: rootfs of primary system is not installed...";\
		false;\
	fi;\

rootfs_for_application_system: generate_romfs generate_bin_file generate_ver_file
	if [ `stat -c %s ${TOP_BUILD_DIR}/$(ROMFS_BIN_NAME)` -gt ${CONF_APP_ROMFS_SIZE} ]; then \
		echo -e "\E[31;47mThe rootfs size exceededs (${CONF_APP_ROMFS_SIZE}), please check it\E[0m"; \
		false;\
	else \
		cp ${TOP_BUILD_DIR}/$(ROMFS_BIN_NAME) ${TOP_BUILD_DIR}/rootfs; \
	fi
		
check_mkcramfs:
ifneq (x${CONF_KERNEL_PAGE_SIZE}, x4096)
	@echo -en "\n\\033[0;33m Check if mkcramfs support -b block_size ... \\033[0m\n"	
	mkdir -p ./test_mkcramfs ; touch ./test_mkcramfs/test && mkcramfs -b ${CONF_KERNEL_PAGE_SIZE} ./test_mkcramfs ./foo_mkcramfs
endif

generate_romfs: check_mkcramfs
	mkcramfs -b ${CONF_KERNEL_PAGE_SIZE} ${TOP_BUILD_ROOTFS_DIR} ${TOP_BUILD_DIR}/$(ROMFS_BIN_NAME)

generate_bin_file:
	(cd ${TOP_BUILD_DIR}/; ./host/bin/mkfw romfs.bin.tail 2 $(ROMFS_BIN_NAME))
	(cd ${TOP_BUILD_DIR}/; md5sum romfs.bin.tail > md5_fw;  cat md5_fw |head -c 32|cat - romfs.bin.tail > $(CONF_FIRMWARE_BIN_FILE); rm romfs.bin.tail )
	
generate_ver_file:
	(cd ${TOP_BUILD_DIR}/;echo VERSION=\'$(CONF_VERSION)\' > $(CONF_FIRMWARE_VER_FILE); echo LOCATION=\'$(CONF_FIRMWARE_BIN_FILE)\' >> $(CONF_FIRMWARE_VER_FILE))
	if [ x$(CONF_FIRMWARE_UPGRADE_WORKAROUND) == xy ]; then \
                (cd ${TOP_BUILD_DIR}/;echo KERNEL=\'Yes\' >> $(CONF_FIRMWARE_VER_FILE));\
        fi
ifneq (x$(CONF_FIRMWARE_PACKAGE_FILE), x)
	(cd ${TOP_BUILD_DIR}/;echo PKG_LOCATION=\'$(CONF_FIRMWARE_PACKAGE_FILE)\' >> $(CONF_FIRMWARE_VER_FILE))
endif

generate_md5sum_info:
ifneq (x$(CONF_FIRMWARE_PACKAGE_FILE), x)
	(cd ${TOP_BUILD_DIR}/; echo VERSION=\'$(CONF_VERSION)\'> ./md5sum.txt && md5sum $(CONF_FIRMWARE_BIN_FILE) >> ./md5sum.txt && md5sum $(CONF_FIRMWARE_FULL_FILE) >> ./md5sum.txt && md5sum $(CONF_FIRMWARE_PACKAGE_FILE) >> ./md5sum.txt)
else
	(cd ${TOP_BUILD_DIR}/; echo VERSION=\'$(CONF_VERSION)\'> ./md5sum.txt && md5sum $(CONF_FIRMWARE_BIN_FILE) >> ./md5sum.txt && md5sum $(CONF_FIRMWARE_FULL_FILE) >> ./md5sum.txt)
endif


