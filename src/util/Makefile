TOP        := $(shell pwd)/../..#
include $(TOP)/Target.mk
include $(TOP)/Target.options
############################# global variables section #######################

LIB_EVENT_PROXY := lib_event_proxy

COMPONENT  := util# comment to remove trailing space
LEAF_LIBS  := $(LIB_EVENT_PROXY)


############################ object rules section ############################

include objects.mk

############################ common rules section ############################

SUBDIRS += $(SUBDIRS_y)

include $(TOP_SCRIPTS_DIR)/rules.mk

########################### target section ###################################

LEAF := $(LIB_EVENT_PROXY)
$(LEAF)_OBJECTS := $($(LIB_EVENT_PROXY)_OBJECTS)
$(BUILD_LIB_CMD)

$(LEAF)_SYSLIBS := -liconv $(FONT_LIB) -lpthread -lz -lm -lmxml
$(BUILD_BIN_CMD)

########################### instal section ###################################

install: 
