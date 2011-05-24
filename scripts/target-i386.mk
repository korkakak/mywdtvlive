#############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  wills_yin@alphanetworks.com
# Makfefile fragments for target intel x86 boards.
# This file contails 4 parts: 
#     platform, OS,  compiler, additonal SDK 
#############################################################################

ifndef TARGET_I386_MK_INCLUDED

TARGET_I386_MK_INCLUDED := 1

CFLAGS := -ggdb -Wall 

############################## OS section ####################################

# Name of the host operating system and the processor on which it's running
MY_OS		:=	$(shell uname -s)
MY_PROC		:=	$(shell uname -p)



CFLAGS += -D_POSIX  -D_UNIX -DLINUX -DUNIX -D_GNU_SOURCE 
CFLAGS                  += -I$(SYSLIB_PREFIX)/include

############################## platform section ##############################
WHICH_GCC       := $(shell which gcc)
CROSS_PREFIX    := $(shell dirname $(WHICH_GCC))
CROSS           := $(CROSS_PREFIX)/
ARCH            := i386
ENABLE_SHLIB    := no
CFLAGS += -D__x86__
export ARCH  CROSS CROSS_PREFIX

LDFLAGS			+= -L$(SYSLIB_PREFIX)/lib
export LDFLAGS

# endian
CFLAGS += -D__LITTLE_ENDIAN__
CFLAGS += -I $(SYSLIB_PREFIX)/include
CFLAGS += -I $(SYSLIB_PREFIX)/include/freetype2

############################## compiler section ##############################

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
#CPP             = -E
AR              = $(CROSS)ar
NM              = $(CROSS)nm
STRIP           = strip --remove-section=.comment
OBJCOPY	        = $(CROSS)objcopy
OBJDUMP         = $(CROSS)objdump
RANLIB          = $(CROSS)ranlib
LINKER          = $(CROSS)gcc
MAKELIB		= $(CROSS)ar -rcs 
export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB MAKELIB LINKER

############################## common flags section ##########################

CFLAGS += $(CONF_COMPILE_WITH_GFLAG) 
CFLAGS += $(CONF_COMPILE_WITH_OPTIMIZE) 

# compiling flags
COMPILER_FLAGS	:=	-c -o # Comment to preserve trailing space

# preprocess flags
PREPROC_FLAGS	:=	-E -o # Comment to preserve trailing space

# dependence flags
CFLAGS_DEP = -MM -MG -DCHECKING

# warning flags
WARNINGS     :=	-Wall -W -Wpointer-arith -Wcast-align \
		-Wwrite-strings -Wstrict-prototypes \
		-Wmissing-prototypes \
		-Wmissing-declarations -Wnested-externs \
                -Werror-implicit-function-declaration -Wundef

#CFLAGS += $(WARNINGS) 


CFLAGS += 	-DDESKTOP_WIDTH=\($(DESKTOP_WIDTH)\) \
	-DDESKTOP_HEIGHT=\($(DESKTOP_HEIGHT)\) 


############################## extra section ###################################


endif            # TARGET_I386_MK_INCLUDED
