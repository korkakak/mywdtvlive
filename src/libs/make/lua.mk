#
# Defines
#
LUA_VERSION := 	5.1#

LUA             := lua-$(LUA_VERSION)#
LUA_SERVER_URL  := $(TOP_LIBS_URL)/lua/lua-$(LUA_VERSION)
LUA_DIR         := $(TOP_LIBS_DIR)#
LUA_SOURCE      := $(LUA_DIR)/lua-$(LUA_VERSION)
LUA_CONFIG      := 

#
# Download  the source 
#
.PHONY: lua-downloaded

lua-downloaded: $(LUA_DIR)/.lua_downloaded

$(LUA_DIR)/.lua_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: lua-unpacked

lua-unpacked: $(LUA_DIR)/.lua_unpacked

$(LUA_DIR)/.lua_unpacked: $(LUA_DIR)/.lua_downloaded $(wildcard $(LUA_SOURCE)/*.tar.gz) $(wildcard $(LUA_SOURCE)/*.patch)
	@echo "Unpacking $(LUA) ..."
	if [ -d "$(LUA_SOURCE)/$(LUA)" ]; then \
		rm -fr $(LUA_SOURCE)/$(LUA); \
	fi
	cd $(LUA_SOURCE); tar xzvf $(LUA).tar.gz
	@echo "Unpacking $(LUA) done"
	@touch $@

#
# config
#
.PHONY: lua-config

lua-config: $(LUA_DIR)/.lua-config

#
# Patch the source
#
.PHONY: lua-patched

lua-patched: $(LUA_DIR)/.lua-config

$(LUA_DIR)/.lua_patched: $(LUA_DIR)/.lua_unpacked 
	@touch $@

#
# Configuration
#
.PHONY: lua-configured

lua-configured:  lua-configured $(LUA_DIR)/.lua_configured

$(LUA_DIR)/.lua_configured: $(LUA_DIR)/.lua_patched $(LUA_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(LUA) done"
	sed -i -e s,CC=.*gcc,CC=$(CC), $(LUA_SOURCE)/$(LUA)/src/Makefile
	sed -i -e s,^MYCFLAGS=,MYCFLAGS=-I$(LIBS_INSTALL_PATH)/include, $(LUA_SOURCE)/$(LUA)/src/Makefile
	sed -i -e s,^MYLDFLAGS=,MYLDFLAGS=-L$(LIBS_INSTALL_PATH)/lib, $(LUA_SOURCE)/$(LUA)/src/Makefile
	sed -i -e s,^INSTALL_TOP=.*,INSTALL_TOP=$(LIBS_INSTALL_PATH), $(LUA_SOURCE)/$(LUA)/Makefile
	@touch $@

#
# Compile
#
.PHONY: lua-compile

lua-compile: $(LUA_DIR)/.lua_compiled

$(LUA_DIR)/.lua_compiled: $(LUA_DIR)/.lua_configured 
	@echo "Compiling $(LUA) ..."
	make -C $(LUA_SOURCE)/$(LUA) posix 
	make -C $(LUA_SOURCE)/$(LUA) install 
	@echo "Compiling $(LUA) done"
	@touch $@

#
# Update
#
.PHONY: lua-update

lua-update:
	@echo "Updating $(LUA) ..."
	@if [ -d "$(LUA_SOURCE)" ]; then\
		$(TOP_UPDATE) $(LUA_SOURCE); \
	fi
	@echo "Updating $(LUA) done"

#
# Status
#
.PHONY: lua-status

lua-status:
	@echo "Statusing $(LUA) ..."
	@if [ -d "$(LUA_SOURCE)" ]; then\
		$(TOP_STATUS) $(LUA_SOURCE); \
	fi
	@echo "Statusing $(LUA) done"

#
# Clean
#
.PHONY: lua-clean

lua-clean:
	@rm -f $(LUA_DIR)/.lua_configured
	@if [ -d "$(LUA_SOURCE)" ]; then\
		make -C $(LUA_SOURCE)/$(LUA) clean; \
	fi

#
# Dist clean
#
.PHONY: lua-distclean

lua-distclean:
	@rm -f $(LUA_DIR)/.lua_*

#
# Install
#
.PHONY: lua-install

lua-install:

