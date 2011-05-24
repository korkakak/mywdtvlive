
#
# Defines
#
ifndef  CONF_READLINE_VERSION 
CONF_READLINE_VERSION := 6.2# 
endif

READLINE             := readline-$(CONF_READLINE_VERSION)#
READLINE_SERVER_URL  := $(TOP_LIBS_URL)/readline/$(READLINE)
READLINE_DIR         := $(TOP_LIBS_DIR)#
READLINE_SOURCE      := $(READLINE_DIR)/$(READLINE)
READLINE_CONFIG      := 

#
# Download  the source 
#
.PHONY: readline-downloaded

readline-downloaded: $(READLINE_DIR)/.readline_downloaded

$(READLINE_DIR)/.readline_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: readline-unpacked

readline-unpacked: $(READLINE_DIR)/.readline_unpacked

$(READLINE_DIR)/.readline_unpacked: $(READLINE_DIR)/.readline_downloaded $(READLINE_SOURCE)/$(READLINE).tar.gz
	@echo "Unpacking $(READLINE) ..."
	if [ -d "$(READLINE_SOURCE)/$(READLINE)" ]; then \
		rm -fr $(READLINE_SOURCE)/$(READLINE); \
	fi
	cd $(READLINE_SOURCE); tar xzvf $(READLINE).tar.gz
	@echo "Unpacking $(READLINE) done"
	@touch $@


#
# Patch the source
#
.PHONY: readline-patched

readline-patched: $(READLINE_DIR)/.readline_patched

$(READLINE_DIR)/.readline_patched: $(READLINE_DIR)/.readline_unpacked
	# Patching...
	@touch $@

#
# config
#
.PHONY: readline-config

readline-config: 

#
# Configuration
#
.PHONY: readline-configured

readline-configured:  readline-config $(READLINE_DIR)/.readline_configured

$(READLINE_DIR)/.readline_configured: $(READLINE_DIR)/.readline_patched $(READLINE_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(READLINE) ..."
	cd $(READLINE_SOURCE)/$(READLINE);./configure --prefix=$(LIBS_INSTALL_PATH)   --build=$(BUILD_HOST) --host=$(BUILD_TARGET) --target=$(BUILD_TARGET)
	@echo "Configuring $(READLINE) done"
	@touch $@

#
# Compile
#
.PHONY: readline-compile

readline-compile: $(READLINE_DIR)/.readline_compiled

$(READLINE_DIR)/.readline_compiled: $(READLINE_DIR)/.readline_configured
	@echo "Compiling $(READLINE) ..."
	#make -C $(READLINE_SOURCE)/$(READLINE) CC=$(CC) AR=$(AR) RANLIB=$(RANLIB) STRIP=$(CROSS)strip LD=$(LD)
	make -C $(READLINE_SOURCE)/$(READLINE) 
	make -C $(READLINE_SOURCE)/$(READLINE)  install
	@echo "Compiling $(READLINE) done"
	@touch $@

#
# Update
#
.PHONY: readline-update

readline-update:
	@echo "Updating $(READLINE) ..."
	@if [ -d "$(READLINE_SOURCE)" ]; then\
		$(TOP_UPDATE) $(READLINE_SOURCE); \
	fi
	@echo "Updating $(READLINE) done"

#
# Status
#
.PHONY: readline-status

readline-status:
	@echo "Statusing $(READLINE) ..."
	@if [ -d "$(READLINE_SOURCE)" ]; then\
		$(TOP_STATUS) $(READLINE_SOURCE); \
	fi
	@echo "Statusing $(READLINE) done"

#
# Clean
#
.PHONY: readline-clean

readline-clean:
	@rm -f $(READLINE_DIR)/.readline_configured
	@if [ -d "$(READLINE_SOURCE)" ]; then\
		make -C $(READLINE_SOURCE)/$(READLINE) clean; \
	fi

#
# Dist clean
#
.PHONY: readline-distclean

readline-distclean:
	@rm -f $(READLINE_DIR)/.readline_*

#
# Install
#
.PHONY: readline-install

readline-install:
