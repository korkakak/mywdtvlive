##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  wills_yin@alphanetworks.com
# Makfefile fragments for target EM86xx boards.
# This file contails 4 parts: 
#     platform, OS,  compiler, additonal SDK 
##############################################################################

ifndef TARGET_EM8620_MK_INCLUDED

TARGET_EM8620_MK_INCLUDED := 1

############################## OS section ####################################

# Name of the host operating system and the processor on which it's running
MY_OS		:=	$(shell uname -s)
MY_PROC		:=	$(shell uname -p)

#CFLAGS += -D_POSIX  -D_UNIX -DLINUX -DUNIX -D_GNU_SOURCE
CFLAGS += -D_POSIX  -D_UNIX -DLINUX -DUNIX 

# Use ccache to speed up the compiling 
CROSS_CCACHE := $(CROSS_PREFIX)/ccache

############################## platform section ##############################

ARCH            :=      arm
ENABLE_SHLIB    := no
CFLAGS += -D__ARM__ -D__EM86XX__

# em8620 SDK lib flags(for sample code)
SDK_LIB_FLAGS += \
             -DEM86XX_CHIP=EM86XX_CHIPID_TANGOLIGHT  \
             -DEM86XX_MODE=EM86XX_MODEID_STANDALONE  \
             -DWITH_AES_CBC=1      
export SDK_LIB_FLAGS
CFLAGS += $(SDK_LIB_FLAGS)

# Link flags
LDFLAGS = -Wl,-elf2flt="-s32768"
export LDFLAGS

# endian
CFLAGS += -D__LITTLE_ENDIAN__

CFLAGS += -D_FILE_OFFSET_BITS=64
############################## compiler section ##############################

# Note: CROSS variable can be set by diffrent product to select 
#       different toolchains.

# Filename extensions
LIB_EXT		:=	a
OBJ_EXT		:=	o
ASM_EXT		:=	s
DEPEND_OBJ_EXT	:=	o
DEPEND_EXT	:=	d
PREPROC_EXT	:=	i
EXE_SUFFIX      :=
EXE_PREFIX      :=      x

# 
AS              = $(CROSS)as
LD              = $(CROSS)ld
CC              = $(CROSS)gcc
DEPEND_CC       = $(CROSS)gcc
CPP             = $(CC) -E
AR              = $(CROSS)ar
NM              = $(CROSS)nm
STRIP           = $(CROSS)strip --remove-section=.comment
OBJCOPY	        = $(CROSS)objcopy
OBJDUMP         = $(CROSS)objdump
RANLIB          = $(CROSS)ranlib
LINKER          = $(CROSS)gcc
MAKELIB		= $(CROSS)ar -rcus 
export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB MAKELIB LINKER

############################## common flags section ##########################

# compiling flags
COMPILER_FLAGS	:=	-c -o # Comment to preserve trailing space

# preprocess flags
PREPROC_FLAGS	:=	-E -o # Comment to preserve trailing space

# dependence flags
CFLAGS_DEP      := -MM -MG -DCHECKING # Comment to preserve trailing space

# warning flags
WARNINGS     :=	-Wall -W -Wpointer-arith -Wcast-align \
		-Wwrite-strings -Wstrict-prototypes \
		-Wmissing-prototypes \
		-Wmissing-declarations -Wnested-externs \
                -Werror-implicit-function-declaration -Wundef
#CFLAGS_COMMON = $(WARNINGS) --unsigned-char
CFLAGS_COMMON = -Wall

CFLAGS += $(CFLAGS_COMMON) 


CFLAGS += 	-DDESKTOP_WIDTH=\($(DESKTOP_WIDTH)\) \
	-DDESKTOP_HEIGHT=\($(DESKTOP_HEIGHT)\) 


############################## extra SDK section #############################

lib_list :=                     \
        rua                     \
        llad                    \
        dcc                     \
        rmvdemux                \
        rmjpeg                  \
        rmungif                 \
        rmpng                   \
        rmzlib                  \
        rmpidfilter             \
        rmscc                   \
        rmaviapi                \
        rmavi                   \
        rmmp4api                \
        rmmp4                   \
        rmmp4core               \
        rmdetectorapi           \
        rmdetector              \
        rmhttp                  \
        rmdescriptordecoder     \
        rmmpeg4framework        \
        rmrtk86                 \
        rmasfdemux              \
        rmasfdemuxcore          \
        rmdtcp                  \
        rmsymboltable           \
        rmcw                    \
        rmwmaprodecoder         \
        rmwmaprodecodercore     \
        rmwmdrmnd               \
        rmcore                  \
        rmwmdrmndcore           \
        rmcpputils			

LIBS_MRUA_A = -Wl,--start-group $(addprefix $(SIGMALIB_MRUA_PATH)/lib,$(addsuffix .$(LIB_EXT),$(lib_list))) -Wl,--end-group

endif            # TARGET_EM8620_MK_INCLUDED
