#
# arusb_lnx - USB 802.11a/b/g/n driver for Atheros AR9170/AR9171 chipsets
#
#
#

CC=$(CROSS_COMPILE)gcc
CPP=$(CROSS_COMPILE)g++
LD=$(CROSS_COMPILE)ld
rM=rm -f -r

KERNEL_SOURCE=$(LINUX_SRC)
HAL_TYPE=OTUS_FB50

# sanity check: does KERNEL_SOURCE exist?
ifeq ($(shell cd $(KERNEL_SOURCE) && pwd),)
$(error $(KERNEL_SOURCE) is missing, please set KERNEL_SOURCE)
endif
#$(error test $(DEPTH) end)

KERN_26=y

# sanity check
ifeq ($(KERN_24), $(KERN_26))
$(error Kernel can only be either 2.4 or 2.6)
endif


SRC_DIR=
DEFINES=-D__KERNEL__ -DMODULE=1

KERNRELEASE := $(shell uname -r;)
MODPATH := /lib/modules/$(KERNRELEASE)

# Load the configuration file
ifeq ($(KERN_26), y)

MODULE = arusb_lnx.ko

INCLUDES=-I$(KERNEL_SOURCE)/include

#EXTRA_CFLAGS += -I/root/wrapper_5D18/zdsta/src/wlan -I/root/wrapper_5D18/zdsta/src
#EXTRA_CFLAGS += -I$(obj)/src/wlan -I$(obj)/src
EXTRA_CFLAGS += -I$(KERNEL_SOURCE)/include -I$(obj)/../../../80211Core -I$(obj) -I$(obj)/../../../HalPlus/$(HAL_TYPE)

#PWD := $(shell pwd)

#EXTRA_CFLAGS += -I./wlan -I.

ifndef CONFIG_FRAME_POINTER
EXTRA_CFLAGS += -fomit-frame-pointer
endif
         
ifdef CONFIG_SMP
EXTRA_CFLAGS += -D__SMP__ -DSMP
endif

KDIR := $(LINUX_SRC)
PWD := $(shell pwd)

WLAN_SRC=$(PWD)

#EXTRA_CFLAGS += -I$(PWD)/src/wlan -I$(PWD)/src
EXTRA_CFLAGS += -O2 -Wall -Wstrict-prototypes -pipe 
#EXTRA_CFLAGS += -Wa,-a,-ad -g
EXTRA_CFLAGS += -DAMAC
EXTRA_CFLAGS += -DGCCK
EXTRA_CFLAGS += -DOFDM
EXTRA_CFLAGS += -DTXQ_IN_ISR
EXTRA_CFLAGS += -DWLAN_HOSTIF=0 #0:USB, 1:PCI

#Test Mode
EXTRA_CFLAGS += -DZM_USB_STREAM_MODE=1
EXTRA_CFLAGS += -DZM_USB_TX_STREAM_MODE=0
EXTRA_CFLAGS += -DZM_PCI_DMA_TEST=0
EXTRA_CFLAGS += -DZM_LARGEPAYLOAD_TEST=0
EXTRA_CFLAGS += -DZM_FW_LOOP_BACK=0
EXTRA_CFLAGS += -DZM_LINUX_TPC=1
#EXTRA_CFLAGS += -DZM_DONT_COPY_RX_BUFFER

EXTRA_CFLAGS += -DZM_HOSTAPD_SUPPORT
#EXTRA_CFLAGS += -DfTX_GAIN_OFDM=0
#EXTRA_CFLAGS += -DZM_CONFIG_BIG_ENDIAN -DBIG_ENDIAN
EXTRA_CFLAGS += -DZM_HALPLUS_LOCK
#EXTRA_CFLAGS += -DZM_OTUS_LINUX_PHASE_2

obj-m := arusb_lnx.o
arusb_lnx-objs := $(SRC_DIR)../../../80211Core/ccmd.o \
$(SRC_DIR)../../../80211Core/chb.o \
$(SRC_DIR)../../../80211Core/cinit.o \
$(SRC_DIR)../../../80211Core/cmm.o \
$(SRC_DIR)../../../80211Core/cmmap.o \
$(SRC_DIR)../../../80211Core/cmmsta.o \
$(SRC_DIR)../../../80211Core/cfunc.o \
$(SRC_DIR)../../../80211Core/coid.o \
$(SRC_DIR)../../../80211Core/ctkip.o \
$(SRC_DIR)../../../80211Core/ctxrx.o \
$(SRC_DIR)../../../80211Core/cic.o \
$(SRC_DIR)../../../80211Core/cpsmgr.o \
$(SRC_DIR)../../../80211Core/cscanmgr.o \
$(SRC_DIR)../../../80211Core/ratectrl.o \
$(SRC_DIR)../../../80211Core/ledmgr.o \
$(SRC_DIR)../../../80211Core/amsdu.o \
$(SRC_DIR)../../../80211Core/cwm.o \
$(SRC_DIR)../../../80211Core/cagg.o \
$(SRC_DIR)../../../80211Core/queue.o \
$(SRC_DIR)../../../80211Core/freqctrl.o \
$(SRC_DIR)../../../80211Core/cwep.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hprw.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpmain.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpusb.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpreg.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpfwu.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpDKfwu.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpfwspiu.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpani.o \
$(SRC_DIR)usbdrv.o \
$(SRC_DIR)zdusb.o \
$(SRC_DIR)ioctl.o \
$(SRC_DIR)wrap_buf.o \
$(SRC_DIR)wrap_mem.o \
$(SRC_DIR)wrap_ev.o \
$(SRC_DIR)wrap_usb.o \
$(SRC_DIR)wrap_pkt.o \
$(SRC_DIR)wrap_dbg.o \
$(SRC_DIR)wrap_mis.o \
$(SRC_DIR)wrap_sec.o \
$(SRC_DIR)wwrap.o

all:

ifneq ($(KERNELRELEASE),)

else
	@echo -e $(KDIR)
	@echo -e $(PWD)
	@echo -e $(EXTRA_CFLAGS)
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

endif

else # kernel 2.4

INCLUDES=-I$(KERNEL_SOURCE)/include -I$(PWD)/../../../80211Core -I$(PWD) -I$(PWD)/../../../HalPlus/$(HAL_TYPE)

ifdef CONFIG_CARDBUS
MODULE = arusb_lnx_cb.o
else
MODULE = arusb_lnx.o
endif

OBJECTS = $(SRC_DIR)../../../80211Core/ccmd.o \
$(SRC_DIR)../../../80211Core/chb.o \
$(SRC_DIR)../../../80211Core/cinit.o \
$(SRC_DIR)../../../80211Core/cmm.o \
$(SRC_DIR)../../../80211Core/cmmap.o \
$(SRC_DIR)../../../80211Core/cmmsta.o \
$(SRC_DIR)../../../80211Core/cfunc.o \
$(SRC_DIR)../../../80211Core/coid.o \
$(SRC_DIR)../../../80211Core/ctkip.o \
$(SRC_DIR)../../../80211Core/ctxrx.o \
$(SRC_DIR)../../../80211Core/cic.o \
$(SRC_DIR)../../../80211Core/cpsmgr.o \
$(SRC_DIR)../../../80211Core/cscanmgr.o \
$(SRC_DIR)../../../80211Core/ratectrl.o \
$(SRC_DIR)../../../80211Core/ledmgr.o \
$(SRC_DIR)../../../80211Core/amsdu.o \
$(SRC_DIR)../../../80211Core/cwm.o \
$(SRC_DIR)../../../80211Core/cagg.o \
$(SRC_DIR)../../../80211Core/queue.o \
$(SRC_DIR)../../../80211Core/freqctrl.o \
$(SRC_DIR)../../../80211Core/cwep.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hprw.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpmain.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpusb.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpreg.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpfwu.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpDKfwu.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpfwspiu.o \
$(SRC_DIR)../../../HalPlus/$(HAL_TYPE)/hpani.o \
$(SRC_DIR)usbdrv.o \
$(SRC_DIR)zdusb.o \
$(SRC_DIR)wwrap.o \
$(SRC_DIR)wrap_buf.o \
$(SRC_DIR)wrap_mem.o \
$(SRC_DIR)wrap_ev.o \
$(SRC_DIR)wrap_usb.o \
$(SRC_DIR)wrap_pkt.o \
$(SRC_DIR)wrap_dbg.o \
$(SRC_DIR)wrap_mis.o \
$(SRC_DIR)wrap_sec.o \
$(SRC_DIR)ioctl.o

CFLAGS=-O -Wall -Wstrict-prototypes -pipe # -Wa,-a,-ad -g

#ifdef CONFIG_MODVERSIONS
CFLAGS += -DMODVERSIONS -include $(KERNEL_SOURCE)/include/linux/modversions.h   #kernel 2.4
#endif

ifndef CONFIG_FRAME_POINTER
CFLAGS += -fomit-frame-pointer
endif

ifdef CONFIG_SMP
CFLAGS += -D__SMP__ -DSMP
endif

CFLAGS += -DAMAC
CFLAGS += -DGCCK
CFLAGS += -DOFDM
CFLAGS += -DTXQ_IN_ISR
CFLAGS += -DWLAN_HOSTIF=0

#Test Mode
CFLAGS += -DZM_USB_STREAM_MODE=1
CFLAGS += -DZM_USB_TX_STREAM_MODE=0
CFLAGS += -DZM_PCI_DMA_TEST=0
CFLAGS += -DZM_LARGEPAYLOAD_TEST=0
CFLAGS += -DZM_FW_LOOP_BACK=0
CFLAGS += -DZM_LINUX_TPC=1
#CFLAGS += -DZM_DONT_COPY_RX_BUFFER

CFLAGS += -DZM_HOSTAPD_SUPPORT
#CFLAGS += -DfTX_GAIN_OFDM=0
#CFLAGS += -DZM_CONFIG_BIG_ENDIAN -DBIG_ENDIAN
CFLAGS += -DZM_HALPLUS_LOCK
#CFLAGS += -DZM_OTUS_LINUX_PHASE_2

all: $(MODULE)

$(MODULE): $(OBJECTS)
	ld -r $(OBJECTS) -o $(MODULE)
	chmod -x $(MODULE)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) $(DEBUG) -c $< -o $@                              
    
endif


install: all
	mkdir -p $(LINUX_SRC_MODULE)
	cp $(MODULE) $(LINUX_SRC_MODULE)
	#depmod -a
ifdef CONFIG_CARDBUS
	cp zdpci.conf /etc/pcmcia
endif

clean:
	rm -rf ../../../.tmp_versions ../../../.*.cmd .*.o.cmd .*.ko.cmd *.ko *.mod.c *.mod.o *.o ../../../80211Core/*.o ../../../80211Core/.*.o.cmd ../../../HalPlus/OTUS_FB50/*.o ../../../HalPlus/OTUS_FB50/.*.o.cmd

