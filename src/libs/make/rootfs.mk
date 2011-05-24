
ROOTFS_SKELETON     := rootfs_skeleton

ROOTFS_SERVER_URL	:= $(TOP_LIBS_URL)/$(ROOTFS_SKELETON)
ROOTFS_SRC_DIR		:= $(TOP_LIBS_DIR)/$(ROOTFS_SKELETON)

UCLIBC_SERVER_URL	:= $(TOP_LIBS_URL)/uClibc_binary/$(CONF_UCLIBC_VERSION)
UCLIBC_SRC_DIR		:= $(TOP_LIBS_DIR)/$(CONF_UCLIBC_VERSION)

GLIBC_SERVER_URL	:= $(TOP_LIBS_URL)/GLIBC_binary/$(CONF_GLIBC_VERSION)
GLIBC_SRC_DIR		:= $(TOP_LIBS_DIR)/$(CONF_GLIBC_VERSION)

RESOURCE_SERVER_URL	:= $(TOP_RESOURCE_URL)/$(CONF_RESOURCE)
RESOURCE_SRC_DIR	:= $(TOP_LIBS_DIR)/$(CONF_RESOURCE)/resource
EXTRA_RESOURCE_SRC_DIR	:= $(TOP_LIBS_DIR)/$(CONF_EXTRA_RESOURCE)/resource

ifeq (x$(CONF_SUPPORT_EXTRA_RESOURCE), xy)
EXTRA_RESOUCRE_SERVER_URL	:= $(TOP_RESOURCE_URL)/$(CONF_EXTRA_RESOURCE)
EXTRA_RESOURCE_SRC_DIR		:= $(TOP_LIBS_DIR)/$(CONF_EXTRA_RESOURCE)/resource
endif

DATE                := $(CONF_PRODUCT)_$(CONF_MODEL)_$(STAMP)

#
# Download  the source 
#
.PHONY: rootfs-downloaded

rootfs-downloaded: $(TOP_LIBS_DIR)/.rootfs_downloaded

$(TOP_LIBS_DIR)/.rootfs_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: rootfs-unpacked

rootfs-unpacked: $(TOP_LIBS_DIR)/.rootfs_unpacked

$(TOP_LIBS_DIR)/.rootfs_unpacked: $(TOP_LIBS_DIR)/.rootfs_downloaded


#
# Patch the source
#
.PHONY: rootfs-patched

rootfs-patched: $(TOP_LIBS_DIR)/.rootfs_patched

$(TOP_LIBS_DIR)/.rootfs_patched: $(TOP_LIBS_DIR)/.rootfs_unpacked

#
# config
#
.PHONY: rootfs-config

rootfs-config: 

#
# Configuration
#
.PHONY: rootfs-configured

rootfs-configured:  rootfs-config $(TOP_LIBS_DIR)/.rootfs_configured

$(TOP_LIBS_DIR)/.rootfs_configured: $(TOP_LIBS_DIR)/.rootfs_patched

#
# Compile
#
.PHONY: rootfs-compile

rootfs-compile: $(TOP_LIBS_DIR)/.rootfs_compiled

$(TOP_LIBS_DIR)/.rootfs_compiled: $(TOP_LIBS_DIR)/.rootfs_configured

#
# Update
#
.PHONY: rootfs-update

rootfs-update:
	@echo "Updating resource: $(CONF_RESOURCE) ..."
	@if [ -d "$(CONF_RESOURCE)" ]; then\
		$(TOP_UPDATE) $(CONF_RESOURCE); \
	fi
	@if [ -d "$(CONF_EXTRA_RESOURCE)" ]; then\
		$(TOP_UPDATE) $(CONF_EXTRA_RESOURCE); \
	fi
	@echo "Updating $(CONF_RESOURCE) done"
	@echo "Updating : $(ROOTFS_SKELETON) ..."
	@if [ -d "$(ROOTFS_SKELETON)" ]; then\
		$(TOP_UPDATE) $(ROOTFS_SKELETON); \
	fi
	@echo "Updating $(ROOTFS_SKELETON) done"

#
# Status
#
.PHONY: rootfs-status

rootfs-status:
	@echo "Statusing resource: $(CONF_RESOURCE) ..."
	@if [ -d "$(CONF_RESOURCE)" ]; then\
		$(TOP_STATUS) $(CONF_RESOURCE); \
	fi
	@if [ -d "$(CONF_EXTRA_RESOURCE)" ]; then\
		$(TOP_STATUS) $(CONF_EXTRA_RESOURCE); \
	fi
	@echo "Statusing $(CONF_RESOURCE) done"
	@echo "Statusing : $(ROOTFS_SKELETON) ..."
	@if [ -d "$(ROOTFS_SKELETON)" ]; then\
		$(TOP_STATUS) $(ROOTFS_SKELETON); \
	fi
	@echo "Statusing $(ROOTFS_SKELETON) done"

#
# Clean
#
.PHONY: rootfs-clean

rootfs-clean:

#
# Dist clean
#
.PHONY: rootfs-distclean

rootfs-distclean:



#
# Install
#
.PHONY: rootfs-install

rootfs-install:
	@echo -en "\n\\033[0;33mInstalling rootfs...\\033[0m\n"
	@mkdir -p $(TOP_INSTALL_ROOTFS_DIR)
	@mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/etc
	echo "export SYSCONF_BUILD_DATE=$(STAMP)" > $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
	echo "export SYSCONF_FIRMWARE_BIN=$(CONF_FIRMWARE_BIN_FILE)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
	echo "export SYSCONF_FIRMWARE_VER=$(CONF_FIRMWARE_VER_FILE)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
	echo "export SYSCONF_BOOTLOADER_MTD_PARTITION=$(CONF_BOOTLOADER_MTD_PARTITION)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
	echo "export SYSCONF_KERNEL_MTD_PARTITION=$(CONF_KERNEL_MTD_PARTITION)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
	echo "export SYSCONF_FIRMWARE_MTD_PARTITION=$(CONF_FIRMWARE_MTD_PARTITION)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
	echo "export SYSCONF_FIRMWARE_MTD_SIZE=$(CONF_APP_ROMFS_SIZE)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
ifeq (x$(CONF_DUAL_ROMFS), xy)
	echo "export SYSCONF_DUAL_ROMFS=$(CONF_DUAL_ROMFS)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
	echo "export SYSCONF_FIRMWARE_MTD_MIRROR_PARTITION=$(CONF_FIRMWARE_MTD_MIRROR_PARTITION)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
endif
ifeq (x$(CONF_SAVE_DEFAULT_SETTING_IN_LAST_PARTITON), xy)
	echo "export SYSCONF_LAST_PARTITION_NODE_NAME=$(CONF_LAST_PARTITION_NODE_NAME)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
	echo "export SYSCONF_STATIC_CONFIG_MOUNT_POINT=$(CONF_STATIC_CONFIG_MOUNT_POINT)" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
endif
ifeq (x$(CONF_DTS_CHIP_SET), xy)
	echo "export IS_DTS=y" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
else
	echo "export IS_DTS=n" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
endif
ifeq (x$(CONF_DMA_DTCP_SIGMA), xy)
	echo "export IS_DTCP=y" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
else
	echo "export IS_DTCP=n" >> $(TOP_INSTALL_ROOTFS_DIR)/sysconfig
endif
	@if [ x$(CONF_ROOTFS_SKELETON_GENERIC) == xy ]; then \
		( cd $(ROOTFS_SRC_DIR)/generic/mandatory/; tar cf - * ) | ( cd $(TOP_INSTALL_ROOTFS_DIR) && tar xf -);	\
	fi
	@if [ x$(CONF_PLATFORM_SMP8634) == xy ]; then \
		( cd $(ROOTFS_SRC_DIR)/platform_smp8634/mandatory/; tar cf - *) | (cd $(TOP_INSTALL_ROOTFS_DIR) && tar xf -);  \
	fi
	@if [ x$(CONF_PLATFORM_SMP8654) == xy ]; then \
		( cd $(ROOTFS_SRC_DIR)/platform_smp8654/mandatory/; tar cf - *) | (cd $(TOP_INSTALL_ROOTFS_DIR) && tar xf -);  \
	fi
	@mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/lib
	@echo -en "\n\\033[0;33m Installing shared objects... ...\\033[0m\n"
ifeq (x$(CONF_PLATFORM_X86),xy)
	-cp -a $(SYSLIB_PREFIX)/lib/*so* $(TOP_INSTALL_ROOTFS_DIR)/lib
else
	cp -a $(SYSLIB_PREFIX)/lib/*so* $(TOP_INSTALL_ROOTFS_DIR)/lib
endif
	@if [ x$(CONF_INSTALL_UCLIBC) == xy ]; then	\
		(cd $(UCLIBC_SRC_DIR); tar cf - * ) | ( cd $(TOP_INSTALL_ROOTFS_DIR)/lib && tar xf - );\
	fi
	@if [ x$(CONF_INSTALL_GLIBC) == xy ]; then	\
		(cd $(GLIBC_SRC_DIR); tar cf - * ) | ( cd $(TOP_INSTALL_ROOTFS_DIR)/lib && tar xf - );\
	fi
	($(STRIP) $(TOP_INSTALL_ROOTFS_DIR)/lib/*so*; true );
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/sbin
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/usr/bin
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/usr/sbin
ifeq (x$(CONF_DIRECTFB),xy)
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/etc
endif
	if [ -f $(BIN_DIR)/config_tool ]; then \
		cp -fa $(BIN_DIR)/config_tool	$(TOP_INSTALL_ROOTFS_DIR)/sbin/; \
	fi
ifeq (x$(CONF_WIN7_WAKEONLAN),xy)
	cp -fa $(SYSLIB_PREFIX)/bin/wol $(TOP_INSTALL_ROOTFS_DIR)/usr/bin/
endif
	-ln -s /tmp/hostname $(TOP_INSTALL_ROOTFS_DIR)/etc/hostname
	-ln -s /tmp/passwd.conf $(TOP_INSTALL_ROOTFS_DIR)/etc/passwd
	-ln -s /tmp/shadow.conf $(TOP_INSTALL_ROOTFS_DIR)/etc/shadow
	mkdir -p $(TOP_INSTALL_ROOTFS_DIR)/osd
###############################################################################################
#		install resource (font, image, mo, screenshot, wmdrm, xml)
###############################################################################################
	cp -fra $(RESOURCE_SRC_DIR)/font $(TOP_INSTALL_ROOTFS_DIR)/osd
	cp -fra $(RESOURCE_SRC_DIR)/image $(TOP_INSTALL_ROOTFS_DIR)/osd
	cp -fra $(RESOURCE_SRC_DIR)/mo $(TOP_INSTALL_ROOTFS_DIR)/osd
ifeq (x$(CONF_THEME_ENABLED),xy)
	cp -fra $(RESOURCE_SRC_DIR)/screenshot $(TOP_INSTALL_ROOTFS_DIR)/osd
endif
ifeq (x$(CONF_DMA_MS_JANUS_SIGMA),xy)
	cp -f $(RESOURCE_SRC_DIR)/wmdrm/*.dat $(TOP_INSTALL_ROOTFS_DIR)/conf_src/	
	if [ -d $(EXTRA_RESOURCE_SRC_DIR)/wmdrm ]; then \
		cp -f $(EXTRA_RESOURCE_SRC_DIR)/wmdrm/*.dat $(TOP_INSTALL_ROOTFS_DIR)/conf_src/; \
	fi
endif
	cp -fra $(RESOURCE_SRC_DIR)/xml/* $(TOP_INSTALL_ROOTFS_DIR)/osd

###############################################################################################
#		install extra resource (xml)
###############################################################################################
ifeq (x$(CONF_SUPPORT_EXTRA_RESOURCE), xy)
	cp -fra $(EXTRA_RESOURCE_SRC_DIR)/xml/* $(TOP_INSTALL_ROOTFS_DIR)/osd
	cp -fra $(EXTRA_RESOURCE_SRC_DIR)/image $(TOP_INSTALL_ROOTFS_DIR)/osd
endif


	if [ -f $(RESOURCE_SRC_DIR)/conf_src/config_factory_default ]; then \
		cp -fra $(RESOURCE_SRC_DIR)/conf_src/config_factory_default $(TOP_INSTALL_ROOTFS_DIR)/conf_src/ ; \
	fi
	echo $(CONF_VERSION) > ${TOP_INSTALL_ROOTFS_DIR}/conf_src/version
	echo "$(DATE)" > ${TOP_INSTALL_ROOTFS_DIR}/conf_src/date
	@if [ x$(CONF_PHOTO_BROWSE_DISPLAY_DEFAULT_IS_THUMBNAIL) == xy ]; then \
		(sed -i -e s/DMA_PHOTO_BROWSEMODE=\'[0-9]\'/DMA_PHOTO_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_PHOTO_BROWSEMODE=\'[0-9]\'/DMA_PHOTO_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
	@if [ x$(CONF_PHOTO_BROWSE_DISPLAY_DEFAULT_IS_LIST) == xy ]; then \
		(sed -i -e s/DMA_PHOTO_BROWSEMODE=\'[0-9]\'/DMA_PHOTO_BROWSEMODE=\'0\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_PHOTO_BROWSEMODE=\'[0-9]\'/DMA_PHOTO_BROWSEMODE=\'0\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
	@if [ x$(CONF_VIDEO_BROWSE_DISPLAY_DEFAULT_IS_THUMBNAIL) == xy ]; then \
		(sed -i -e s/DMA_MOVIE_BROWSEMODE=\'[0-9]\'/DMA_MOVIE_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_MOVIE_BROWSEMODE=\'[0-9]\'/DMA_MOVIE_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
	@if [ x$(CONF_VIDEO_BROWSE_DISPLAY_DEFAULT_IS_LIST) == xy ]; then \
		(sed -i -e s/DMA_MOVIE_BROWSEMODE=\'[0-9]\'/DMA_MOVIE_BROWSEMODE=\'0\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_MOVIE_BROWSEMODE=\'[0-9]\'/DMA_MOVIE_BROWSEMODE=\'0\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
	@if [ x$(CONF_VIDEO_BROWSE_DISPLAY_DEFAULT_IS_VIDEO) == xy ]; then \
		(sed -i -e s/DMA_MOVIE_BROWSEMODE=\'[0-9]\'/DMA_MOVIE_BROWSEMODE=\'3\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_MOVIE_BROWSEMODE=\'[0-9]\'/DMA_MOVIE_BROWSEMODE=\'3\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
	@if [ x$(CONF_PRODUCT_EXT_WDTV_RV) == xy ]; then \
		(sed -i -e s/DMA_MOVIE_BROWSEMODE=\'[0-9]\'/DMA_MOVIE_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
                (sed -i -e s/DMA_MOVIE_BROWSEMODE=\'[0-9]\'/DMA_MOVIE_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
		(echo "VIDEO_SOURCE_SELECT='/tmp/WDTVPriv/.wd_tv_rv_cas_folder/wdtv.cas'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
		(echo "AUDIO_SOURCE_SELECT='/tmp/WDTVPriv/.wd_tv_rv_cas_folder/wdtv.cas'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
		(echo "PHOTO_SOURCE_SELECT='/tmp/WDTVPriv/.wd_tv_rv_cas_folder/wdtv.cas'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
		(echo "FILE_SOURCE_SELECT='/tmp/WDTVPriv/.wd_tv_rv_cas_folder/wdtv.cas'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
		(echo "VIDEO_SORT_SELECT='SORT_BY_ALPHABETICAL_ASC'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
		(echo "VIDEO_FILTER_SELECT='FILTER_BY_FOLDER'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
		(echo "DMA_ONLINE_BROWSEMODE='1'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
		(echo "DMA_DASHBOARD_BROWSEMODE='1'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
		(echo "DASHBOARD_LAST_ACCESSED_ITEM='0'" >> $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default); \
	fi
	@if [ x$(CONF_MUSIC_BROWSE_DISPLAY_DEFAULT_IS_THUMBNAIL) == xy ]; then \
		(sed -i -e s/DMA_MUSIC_BROWSEMODE=\'[0-9]\'/DMA_MUSIC_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_MUSIC_BROWSEMODE=\'[0-9]\'/DMA_MUSIC_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
	@if [ x$(CONF_MUSIC_BROWSE_DISPLAY_DEFAULT_IS_LIST) == xy ]; then \
		(sed -i -e s/DMA_MUSIC_BROWSEMODE=\'[0-9]\'/DMA_MUSIC_BROWSEMODE=\'0\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_MUSIC_BROWSEMODE=\'[0-9]\'/DMA_MUSIC_BROWSEMODE=\'0\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
	@if [ x$(CONF_BROWSE_DISPLAY_DEFAULT_IS_THUMBNAIL) == xy ]; then \
		(sed -i -e s/DMA_BROWSEMODE=\'[0-9]\'/DMA_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_BROWSEMODE=\'[0-9]\'/DMA_BROWSEMODE=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
	@if [ x$(CONF_BROWSE_DISPLAY_DEFAULT_IS_LIST) == xy ]; then \
		(sed -i -e s/DMA_BROWSEMODE=\'[0-9]\'/DMA_BROWSEMODE=\'0\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);  \
		(sed -i -e s/DMA_BROWSEMODE=\'[0-9]\'/DMA_BROWSEMODE=\'0\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_for_mass_product);  \
	fi
		(sed -i -e s/DMA_SLIDESHOW=\'1\'/DMA_SLIDESHOW=\'$(CONF_SLIDESHOW_INTERVAL)\'/ $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default);
	(cd ${TOP_BUILD_ROOTFS_DIR}; mv ./etc/securetty ./etc/securetty.ori)
	(cd ${TOP_BUILD_ROOTFS_DIR}/etc; ln -sf /tmp/securetty.ori ./securetty)

ifeq (x$(CONF_SECURITY_PROPRIETARY_ENCRYPT),xy)
ifeq (x$(CONF_SECURITY_USE_JANUS_SHA1SUM),xy)	
	(cp $(TOP_MISC_DIR)/mount_script/init_1 $(TOP_INSTALL_ROOTFS_DIR)/bin);
	(sed -i -e 's/fw_reload_t3.sh/fw_reload_t3.sh\n\init_1/' $(TOP_INSTALL_ROOTFS_DIR)/init);
endif
ifeq (x$(CONF_SECURITY_USE_CPU_KEY_SHA1SUM),xy)	
	(cp $(TOP_MISC_DIR)/mount_script/init_2 $(TOP_INSTALL_ROOTFS_DIR)/bin);
	(sed -i -e 's/fw_reload_t3.sh/fw_reload_t3.sh\n\init_2/' $(TOP_INSTALL_ROOTFS_DIR)/init);
endif
ifeq (x$(CONF_SECURITY_USE_IPTOKEN_SHA1SUM),xy)	
	(cp $(TOP_MISC_DIR)/mount_script/init_3 $(TOP_INSTALL_ROOTFS_DIR)/bin);
	(sed -i -e 's/fw_reload_t3.sh/fw_reload_t3.sh\n\init_3/' $(TOP_INSTALL_ROOTFS_DIR)/init);
endif
endif

ifeq (x$(CONF_CONSOLE_SECURE),xy)
	(sed -i -e 's/cp \/etc\/shadow.conf \/tmp -f/cp \/etc\/shadow.conf \/tmp -f\n\n\gbus_read_serial_num\n/' $(TOP_INSTALL_ROOTFS_DIR)/init);
	(sed -i -e 's/\t\/bin\/sh/\/sbin\/getty -L ttyS0 115200 vt100/' $(TOP_INSTALL_ROOTFS_DIR)/init);
endif

ifeq (x$(CONF_PLATFORM_X86),xy)
	mkdir -p ${TOP_BUILD_ROOTFS_DIR}/$(CONF_SETTING_CONFIG_FILE_DIR)
	cp -f ${TOP_BUILD_ROOTFS_DIR}/$(CONF_SETTING_ORIGINAL_CONFIG_FILE_DIR)/$(CONF_SETTING_FACTORY_SETTINGS) ${TOP_BUILD_ROOTFS_DIR}/$(CONF_SETTING_CONFIG_FILE_DIR)/$(CONF_SETTING_CONFIG_FILE_NAME)
endif
ifneq (x$(CONF_TWONKYMEDIA_SERVER_SUPPORT), xy)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/init ]; then sed -i s,/bin/run_twonky_server,\#/bin/run_twonky_server, $(TOP_INSTALL_ROOTFS_DIR)/init; fi;)
endif
ifeq (x$(CONF_TWONKYMEDIA_SERVER_SUPPORT), xy)
	#TMS5.1 ignore to scan /tmp folder so I create mediaitems to / and link to /tmp/media/usb to workaround this issue.
	ln -s /tmp/media/usb $(TOP_INSTALL_ROOTFS_DIR)/mediaitems
endif
ifeq (x$(CONF_WEBGUI_SUPPORT), xy)
	if [ -f $(BIN_DIR)/getServerValue.cgi ]; then \
		cp -fa $(BIN_DIR)/getServerValue.cgi	$(TOP_INSTALL_ROOTFS_DIR)/webserver/cgi-bin/; \
	fi
	if [ -f $(BIN_DIR)/toServerValue.cgi ]; then \
		cp -fa $(BIN_DIR)/toServerValue.cgi	$(TOP_INSTALL_ROOTFS_DIR)/webserver/cgi-bin/; \
	fi
endif
ifeq (x$(CONF_MULTI_PLAYER_CACHE),xy)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/init ]; then sed -i s,\#/bin/memory_cache_server,/bin/memory_cache_server, $(TOP_INSTALL_ROOTFS_DIR)/init; fi;)
endif

ifeq (x$(CONF_PRODUCT_EXT_WDTV_VILLA), xy)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,DMA_NAME="'My Media Player'",DMA_NAME="'WDTV'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
endif
ifeq (x$(CONF_SAMBA_VERSION_3_5_1), xy)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/bin/run_samba ]; then sed -i s,"exec smbd -D --configfile=/tmp/smb.conf --piddir=/tmp --private-dir=/tmp/private --lockdir=/tmp/ --smb-passwd-file=/tmp/private/smbpasswd","exec smbd -D --configfile=/tmp/smb.conf --piddir=/tmp --private-dir=/tmp/private --lockdir=/tmp/ --smb-passwd-file=/tmp/private/smbpasswd --statedir=/tmp --cachedir=/tmp", $(TOP_INSTALL_ROOTFS_DIR)/bin/run_samba; fi;)
endif
ifeq (x$(CONF_PRODUCT_EXT_LMP555), xy)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,VIDEO_OUT="'COMPOSITE'",VIDEO_OUT="'HDMI'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,VIDEO_OUT_RESOLUTION="'480i'",VIDEO_OUT_RESOLUTION="'auto'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i -e s/DMA_CONTENT_AGGREGATION=\'[0-9]\'/DMA_CONTENT_AGGREGATION=\'1\'/ $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,DMA_ENABLE_AUTO_STANDBY="'0'",DMA_ENABLE_AUTO_STANDBY="'20'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,DMA_BROWSEMODE="'1'",DMA_BROWSEMODE="'0'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,PICFIT="'0'",PICFIT="'1'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,PHOTO_SLIDESHOW_EFFECT="'0'",PHOTO_SLIDESHOW_EFFECT="'1'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,DMA_SLIDESHOW="'3'",DMA_SLIDESHOW="'5'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,SUBTITLE_FONT_SIZE="'40'",SUBTITLE_FONT_SIZE="'36'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,ENABLE_AUTO_LOGIN="'0'",ENABLE_AUTO_LOGIN="'1'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,DMA_VOLUME="'80'",DMA_VOLUME=\'$(CONF_DEFAULT_VOLUME)\', $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
endif
ifeq (x$(CONF_SINGLE_RENDERER),xy)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/bin/run_all ]; then sed -i 's:exec picrender.sh:\#exec picrender.sh:' $(TOP_INSTALL_ROOTFS_DIR)/bin/run_all; fi;)
endif
ifeq (x$(CONF_PRODUCT_EXT_WDTV_RV), xy)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,ENABLE_TRANSFER_SYNC="'0'",ENABLE_TRANSFER_SYNC="'1'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,PHOTO_SLIDESHOW_EFFECT="'0'",PHOTO_SLIDESHOW_EFFECT="'3'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,VIDEO_OUT="'COMPOSITE'",VIDEO_OUT="'HDMI'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	(if [ -f $(TOP_INSTALL_ROOTFS_DIR)/conf_src/config_factory_default ]; then sed -i s,VIDEO_OUT_RESOLUTION="'480i'",VIDEO_OUT_RESOLUTION="'auto'", $(TOP_INSTALL_ROOTFS_DIR)//conf_src/config_factory_default; fi;)
	cp -raf $(ROOTFS_SRC_DIR)/RV/* $(TOP_INSTALL_ROOTFS_DIR) 
endif
	find $(TOP_INSTALL_ROOTFS_DIR) -name .svn -exec rm -rf {} \; &> /dev/null; true
ifneq (x$(CONF_PLATFORM_X86), xy)
	@echo -en "\n\\033[0;33m Striping binary executable (Please keep this in the last)\\033[0m\n"
	find $(TOP_BUILD_ROOTFS_DIR) -type f -exec file {} \; | grep -v "\.ko" | grep "not\ stripped" | awk -F ":" '{ print $$1 }' | xargs $(STRIP) 
	(cd $(TOP_BUILD_ROOTFS_DIR); tar xzvf ../alpha.tar.gz )
	(cd $(TOP_BUILD_ROOTFS_DIR); rm -f md5sum.txt )
	(cd ${TOP_BUILD_ROOTFS_DIR}; find . -type f -exec md5sum {} \; > /tmp/md5sum.txt.`whoami` )
	cp -f /tmp/md5sum.txt.`whoami` ${TOP_BUILD_ROOTFS_DIR}/md5sum.txt
endif

