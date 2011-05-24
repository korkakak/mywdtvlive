##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  wills_yin@alphanetworks.com
# Makfefile fragments for target EM86xx boards.
# This file contails 4 parts: 
#     platform, OS,  compiler, additonal SDK 
##############################################################################

ifndef TARGET_EM8622_MK_INCLUDED

TARGET_EM8622_MK_INCLUDED := 1

############################## OS section ####################################

# Name of the host operating system and the processor on which it's running
MY_OS		:=	$(shell uname -s)
MY_PROC		:=	$(shell uname -p)

CFLAGS          += -D_POSIX  -D_UNIX -DLINUX -DUNIX 

# Use ccache to speed up the compiling 
CROSS_CCACHE    := $(CROSS_PREFIX)/ccache

############################## platform section ##############################

ARCH            := arm
ENABLE_SHLIB    := no

CFLAGS          += -D__ARM__ -D__EM86XX__
CFLAGS			+= -I$(SYSLIB_PREFIX)/include
CFLAGS			+= -I$(CROSS_PREFIX)/include

BUILD_HOST      :=      $(shell uname -m)
BUILD_TARGET    :=      $(ARCH)-elf

# em8622 SDK lib flags(for sample code)
SDK_LIB_FLAGS   += \
             -DEM86XX_CHIP=EM86XX_CHIPID_TANGO15     \
             -DDEMUX_PSF=1      \
             -DEM86XX_MODE=EM86XX_MODEID_STANDALONE  \
             -DWITHOUT_DTS=1 
export SDK_LIB_FLAGS

RMCFLAGS        += $(SDK_LIB_FLAGS)
RMCFLAGS        += -DOWNER="AlphaNetworks"
export RMCFLAGS
COMPILKIND      = release
export COMPILKIND
CFLAGS          += $(SDK_LIB_FLAGS)

# Link flags
LDFLAGS         := -Wl,-elf2flt="-s32768"
LDFLAGS			+= -L$(SYSLIB_PREFIX)/lib
LDFLAGS			+= -L$(CROSS_PREFIX)/lib
LDFLAGS			+= -L$(SIGMA_HWCIPHER_LIB_PATH)/src
export LDFLAGS

# endian
CFLAGS          += -D__LITTLE_ENDIAN__

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
CXX             = $(CROSS)g++
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
PATH            := ${PATH}:$(CROSS_PREFIX)/bin
export AS LD CC CXX CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB MAKELIB LINKER PATH

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
        rmavicore               \
        rmscc                   \
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
        rmwmdrmndupnp           \
        rmcpputils              \
        rmmm_t                  \
        rmcdfs                  \
        rmec3transcoder         \
        ndsp                    \
        rmpthreadw

LIBS_MRUA_A = $(addprefix $(SIGMALIB_MRUA_PATH)/lib, $(addsuffix .$(LIB_EXT),$(lib_list))) 


endif           # TARGET_EM8622_MK_INCLUDED
