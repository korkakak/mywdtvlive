# 254     0     130560 sigmblocka
# 254     1        512 sigmblockb
# 254     2        256 sigmblockc
# 254     3       3072 sigmblockd
# 254     4       3072 sigmblocke
# 254     5      16384 sigmblockf
# 254     6       8192 sigmblockg
# 254     7      30464 sigmblockh
# 254     8      30464 sigmblocki
# 254     9        128 sigmblockj
# 254    10        128 sigmblockk
# 254    11        128 sigmblockl

# The NAND flash total size
FLASH_TOTAL_SIZE        := ($(CONF_NAND_FLASH_SIZE)/1024/1024)
# Logical Block start
LOGICBLOCK_FLASH_BASE   := 0
# yamon: 0x00080000
YAMON_FLASH_BASE        := ($(CONF_MTD_BEFORE_YAMON_SIZE)/1024)
# linux kernel: 0xc0000
KERNEL1_FLASH_BASE		:= ($(YAMON_FLASH_BASE)+($(CONF_MTD_YAMON_SIZE)/1024))
# linux kernel2: 0x003c0000
KERNEL2_FLASH_BASE		:= ($(KERNEL1_FLASH_BASE)+($(CONF_PRIMARY_SYSTEM_SIZE)/1024))
# imat: 0x006c0000
IMAT_FLASH_BASE			:= ($(KERNEL2_FLASH_BASE)+($(CONF_PRIMARY_SYSTEM_SIZE)/1024))
# xmat: 0x016c0000
XMAT_FLASH_BASE			:= ($(IMAT_FLASH_BASE)+($(CONF_MTD_IMAT_SIZE)/1024))
# rootfs1: 0x01ec0000
ROOTFS1_FLASH_BASE		:= ($(XMAT_FLASH_BASE)+($(CONF_MTD_XMAT_SIZE)/1024))
# rootfs2: 0x078c0000
ROOTFS2_FLASH_BASE		:= ($(ROOTFS1_FLASH_BASE)+($(CONF_APP_ROMFS_SIZE)/1024))
# setting1: 0x0d2c0000
SETTING1_FLASH_BASE		:= ($(ROOTFS2_FLASH_BASE)+($(CONF_STORAGE_PARTITION_SIZE)/1024))

# Counting logical block offset from YAMON starting address
OFFSET_YAMON2KERNEL 	:= ($(KERNEL1_FLASH_BASE)-$(YAMON_FLASH_BASE))
OFFSET_YAMON2ROOTFS 	:= ($(ROOTFS1_FLASH_BASE)-$(YAMON_FLASH_BASE))
OFFSET_YAMON2IMAT 		:= ($(IMAT_FLASH_BASE)-$(YAMON_FLASH_BASE))
OFFSET_YAMON2XMAT 		:= ($(XMAT_FLASH_BASE)-$(YAMON_FLASH_BASE))
TOTAL_ROMFS_SIZE 		:= ($(ROOTFS2_FLASH_BASE)-$(YAMON_FLASH_BASE))
OFFSET_OKOK 			:= (($(ROOTFS2_FLASH_BASE)-$(YAMON_FLASH_BASE))*1024-4)

# Input file
#ZBOOT_FILE=zboot.bin
#YAMON_FILE=zbimage-yamon-2.13
#KERNEL_FILE=zbimage-linux-xload
#ROOTFS_FILE=romfs.bin
#IMAT_FILE=imat.bin
#XMAT_FILE=xmat.bin

# Nand flash image raw data
DATAFILE_YK			:= $(TOP_BUILD_DIR)/datafile_yk.img
DATAFILE_YKR			:= $(TOP_BUILD_DIR)/datafile_ykr.img

# Build raw data for nand flash programer
RAW_NAND_ZY				:= $(TOP_BUILD_DIR)/raw_zy.img
RAW_NAND_ZYK			:= $(TOP_BUILD_DIR)/raw_zyk.img
RAW_NAND_ZYKR			:= $(TOP_BUILD_DIR)/raw_zykr.img

ifeq (x$(CONF_PLATFORM_SMP8670), xy)
RAW_NAND_GEN_PROG		:= $(SIGMA_SDK_SOURCE)/gennandraw/gen_nandv3_raw_img
else
RAW_NAND_GEN_PROG		:= $(SIGMA_SDK_SOURCE)/gennandraw/gen_nand_raw_img
endif

# Building SATA 
SATA_FLASH_SIZE=16384
#0x0
SATA_ZBOOT2_FLASH_BASE=0
#0x100000
#SATA_YAMON_FLASH_BASH=1024
#0x1C0000
#SATA_KERNEL_FLASH_BASH=1792
SATA_KERNEL_FLASH_BASH=1024

# SATA image raw data
SATA_IMAGE_KERNEL=./datafile_sata.img

CREATE_FIFO=./create_ff_fifo
FIFO=./sss

#all: create_fifo nand_raw_image
#	killall -9 $(CREATE_FIFO)

#create_fifo:
#	$(CREATE_FIFO) $(FIFO) &


export RAW_ZY_SIZE=`stat --printf="%s" $(RAW_NAND_ZY)`
export RAW_ZYK_SIZE=`stat --printf="%s" $(RAW_NAND_ZYK)`
export RAW_ZYKR_SIZE=`stat --printf="%s" $(RAW_NAND_ZYKR)`

nand_image: nand_raw_image
#	@echo "RAW_ZY_SIZE = $(RAW_ZY_SIZE) =================="
#	export SIZE=$(RAW_ZY_SIZE) && TOTAL_BLOCK=`expr $$SIZE / 2112 / 64` && printf "end = 0x%x \t" `expr $$TOTAL_BLOCK + 32` && printf "count = 0x%x\n" `expr $$TOTAL_BLOCK - 8` 
#	@echo "RAW_ZYK_SIZE = $(RAW_ZYK_SIZE) ================="
#	export SIZE=$(RAW_ZYK_SIZE) && TOTAL_BLOCK=`expr $$SIZE / 2112 / 64` && printf "end = 0x%x \t" `expr $$TOTAL_BLOCK + 32` && printf "count = 0x%x\n" `expr $$TOTAL_BLOCK - 8` 
	@echo "========================================================"
	@echo "Flash image RAW_ZYKR_SIZE = $(RAW_ZYKR_SIZE) bytes "
	@# Now sigma only need 2 groups in group define
	@# 1. start = 0, end = 3, count = 2. 
	@#    It means we only need to write 2 blocks. The range is 0 ~ 3.
	@#    Only the start 2 blocks in image has data. 
	@# 2. start = 4, end = (TOTAL_BLOCK + 32), count = (TOTAL_BLOCK - 4)
	@#    Parameters :
	@#        start = 4 (because the start 4 blocks are in 1st group.)
	@#        count = TOTAL_BLOCK - 4 (because the start 4 blocks are in 1st group.)
	@#        end = TOTAL_BLOCK + 32 (32 is the bad block number we allow.)
	@printf "group0 : start = 0x%x \t end = 0x%x \t count = 0x%x\n" 0 3 2
	@export SIZE=$(RAW_ZYKR_SIZE) && TOTAL_BLOCK=`expr $$SIZE / 2112 / 64` && \
			printf "group1 : start = 0x%x \t end = 0x%x " 4 `expr $$TOTAL_BLOCK + 32` && printf "\t count = 0x%x\n" `expr $$TOTAL_BLOCK - 4` 
	@echo "========================================================"
	@echo ""
	
nand_raw_image: raw_image_zkyr
	ls -l $(TOP_BUILD_DIR)/*.img

raw_image_zkyr: nand_image_with_romfs
ifeq (x$(CONF_PLATFORM_SMP8670), xy)
	$(RAW_NAND_GEN_PROG) b0f=$(ZBOOT_FILE) df=$(DATAFILE_YKR) nf=$(RAW_NAND_ZYKR) size=$$[$(FLASH_TOTAL_SIZE)] count=1000 percent=3 lb=4 mlc_nand_cfg=$(SIGMA_SDK_SOURCE)/gennandraw/mlc_nand.cfg mlc_devname=NAND02GW3B2D
else
	$(RAW_NAND_GEN_PROG) b0f=$(ZBOOT_FILE) df=$(DATAFILE_YKR) nf=$(RAW_NAND_ZYKR) size=$$[$(FLASH_TOTAL_SIZE)] count=1000 percent=3 lb=4
endif

nand_image_with_romfs:
	-rm -vf $(DATAFILE_YKR)
	dd if=/dev/zero of=$(DATAFILE_YKR) conv=notrunc bs=1k count=$$[$(TOTAL_ROMFS_SIZE)]
ifneq (x$(CONF_DATAFILE_NO_YAMON), xy)
	dd if=$(YAMON_FILE) of=$(DATAFILE_YKR) conv=notrunc seek=$$[0] bs=1k
endif
	dd if=$(KERNEL_FILE) of=$(DATAFILE_YKR) conv=notrunc seek=$$[$(OFFSET_YAMON2KERNEL)] bs=1k
	dd if=$(IMAT_FILE) of=$(DATAFILE_YKR) conv=notrunc seek=$$[$(OFFSET_YAMON2IMAT)] bs=1k
	dd if=$(XMAT_FILE) of=$(DATAFILE_YKR) conv=notrunc seek=$$[$(OFFSET_YAMON2XMAT)] bs=1k
	dd if=$(ROOTFS_FILE) of=$(DATAFILE_YKR) conv=notrunc seek=$$[$(OFFSET_YAMON2ROOTFS)] bs=1k
	echo "sisi" > /tmp/sisi; dd if=/tmp/sisi of=$(DATAFILE_YKR) conv=notrunc seek=$$[$(OFFSET_OKOK)] bs=1 count=4

nand_image_clean:
	rm -vf $(DATAFILE_ZBOOT_YAMON) $(DATAFILE_KERNEL) $(SATA_IMAGE_KERNEL) $(DATAFILE_ROOTFS)


