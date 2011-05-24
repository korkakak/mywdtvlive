TOP        := $(shell pwd)/../../..#
include $(TOP)/Target.mk

LINUX_SRC = $(TOP_LIBS_DIR)/$(CONF_SIGMA_SDK_VERSION)/linux_kernel_$(CONF_LINUX_KERNEL_VERSION)/
CROSS_COMPILE = $(CROSS)
CROSS_COMPILE_INCLUDE = $(CONF_CROSS_PREFIX)include
LINUX_SRC_MODULE = $(TOP)/build/root/lib/modules

export LINUX_SRC CROSS_COMPILE CROSS_COMPILE_INCLUDE LINUX_SRC_MODULE

all: 
	make  -C  $(LINUX_SRC) SUBDIRS=$(shell pwd)/oal/Otus/Linux modules

clean:
	make  -C  $(shell pwd)/oal/Otus/Linux clean
	
install:
	make  -C  $(shell pwd)/oal/Otus/Linux install
