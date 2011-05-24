############################# global variables section #######################

# Comment/uncomment the following line to disable/enable debugging
DEBUG = n
CFLAGS_$(DEBUG) += -g -D_DEBUG

############################# base rules section #############################

#
# VPATH section
#
VPATH += \
	./src                 \
	./src/event_proxy     \

#
# include search path section
#
INCLUDES += \
	-I./include         \
	-include $(TOP)/Target.h

#
# CFLAGS section
#
CFLAGS += \
	-DSIMPLE_WRITE_CACHE                     \
	-DDATA_SOURCE_FRAMEWORK                  \
	-DDMA_UPNP_SUPPORT_LOOPBACK              \
	-DHAVE_STRERROR -DHAVE_STRDUP            \
	-DBSD_COMP -DHAVE_GETTIMEOFDAY           \
	-D__$(CURRENT_SET)__										          

#
# objects list  section
#

EVENT_PROXY_OBJS := \
	event_proxy.o         \
	event_transfer_api.o  \
	fifo_event_process.o  \
	list_event_process.o	\
	ILibLinkedList.o


#don't add the files directly into OBJECTS.Please group it by components
OBJECTS := \
	$(EVENT_PROXY_OBJS)  \


############################# feature rules section #############################
#
# INCLUDES
#

#
# VPATH
#
#
# CFLAGS
#
CFLAGS_$(CONF_EVENT_PROXY)    += -DFLAVOUR_EVENT_PROXY


############################# target rules section ###########################

CFLAGS      += $(CFLAGS_y)
VPATH       += $(VPATH_y)
INCLUDES    += $(INCLUDES_y)
OBJECTS     += $(OBJECTS_y)

$(LIB_EVENT_PROXY)_OBJECTS := $(EVENT_PROXY_OBJS)

OBJECTS_ALL  := $(OBJECTS) $(AUTOTEST_OBJ) $(MKCONFIG_MAIN_OBJS) $(CONFIG_CMD_OBJ)
