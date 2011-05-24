##############################################################################
# Copyright (C) alphanetworks 2006-- 
# All Rights Reserved -- Company Confidential
# Author:  joshua_lee@alphanetworks.com
# dns_daemon object rules makefile fragments
##############################################################################
#define

ifdef CONF_SAMBA_PACKAGE_3_5_1_SUPPORT_3_0_28A_CLIENT
	SAMBA_VERSION := 3.0.28a#
	SMBTREE_SOURCE_FOLDER := source
else #/*CONF_SAMBA_PACKAGE_3_5_1_SUPPORT_3_0_28A_CLIENT*/
ifndef  CONF_SAMBA_VERSION
	SAMBA_VERSION := 3.0.28a#
else
	SAMBA_VERSION := $(CONF_SAMBA_VERSION)#
endif
ifeq ($(CONF_SAMBA_VERSION), 3.0.28a)
	SMBTREE_SOURCE_FOLDER := source
else
	SMBTREE_SOURCE_FOLDER := source3
endif
endif #/*CONF_SAMBA_PACKAGE_3_5_1_SUPPORT_3_0_28A_CLIENT*/


SMBTREE_SOURCE  = $(TOP)/src/libs/samba-$(SAMBA_VERSION)/samba-$(SAMBA_VERSION)/$(SMBTREE_SOURCE_FOLDER)
SAMBA_POPT_SOURCE = $(SMBTREE_SOURCE)/../lib/popt
#
# include search path section
#
ifeq ($(SAMBA_VERSION), 3.0.28a)
INCLUDES += \
	-I$(TOP)                           \
	-include $(TOP)/Target.h	\
	-I$(SMBTREE_SOURCE)/iniparser/src \
	-I$(SMBTREE_SOURCE)/libaddns  \
	-I$(SMBTREE_SOURCE)/librpc  \
	-I$(SMBTREE_SOURCE)/lib/talloc \
	-I$(SMBTREE_SOURCE)/tdb/include  \
	-I$(SMBTREE_SOURCE)/lib/replace \
	-I$(SMBTREE_SOURCE)/include/  \
	-I$(SMBTREE_SOURCE)/ \
	-I$(SAMBA_POPT_SOURCE)  \
	-I$(TOP_UTILS_DIR)/include         
else
INCLUDES += \
	-I$(TOP)                           \
	-include $(TOP)/Target.h	\
	-I$(SMBTREE_SOURCE)/.. \
	-I$(SMBTREE_SOURCE)/iniparser/src \
	-I$(SMBTREE_SOURCE)/libaddns  \
	-I$(SMBTREE_SOURCE)/../librpc  \
	-I$(SMBTREE_SOURCE)/lib/talloc \
	-I$(SMBTREE_SOURCE)/tdb/include  \
	-I$(SMBTREE_SOURCE)/../lib/replace \
	-I$(SMBTREE_SOURCE)/../lib/tevent \
	-I$(SMBTREE_SOURCE)/../lib/cli/util \
	-I$(SMBTREE_SOURCE)/include/  \
	-I$(SMBTREE_SOURCE)/ \
	-I$(SAMBA_POPT_SOURCE)  \
	-I$(TOP_UTILS_DIR)/include         
endif
#
# CFLAGS section
#
CFLAGS += \
	-DHAVE_CONFIG_H                     \
	-D_SAMBA_BUILD_=3

#
# objects list  section
#
OBJECTS          := smbtree_$(SAMBA_VERSION).o  popt_common_$(SAMBA_VERSION).o

OBJECTS_ALL	:= $(OBJECTS)

