#
# Defines
#
ifndef  CONF_PHP_VERSION 
CONF_PHP_VERSION := 5.2.17#
endif

PHP             := php-$(CONF_PHP_VERSION)#
PHP_SERVER_URL  := $(TOP_LIBS_URL)/php/$(PHP)
PHP_DIR         := $(TOP_LIBS_DIR)#
PHP_SOURCE      := $(PHP_DIR)/$(PHP)
PHP_CONFIG      := 

#
# Download  the source 
#
.PHONY: php-downloaded

php-downloaded: $(PHP_DIR)/.php_downloaded

$(PHP_DIR)/.php_downloaded:
	@touch $@

#
# Unpack the source
#
.PHONY: php-unpacked

php-unpacked: $(PHP_DIR)/.php_unpacked

$(PHP_DIR)/.php_unpacked: $(PHP_DIR)/.php_downloaded $(PHP_SOURCE)/$(PHP).tar.bz2
	@echo "Unpacking $(PHP) ..."
	if [ -d "$(PHP_SOURCE)/$(PHP)" ]; then \
		rm -fr $(PHP_SOURCE)/$(PHP); \
	fi
	cd $(PHP_SOURCE); tar xjvf $(PHP).tar.bz2
	@echo "Unpacking $(PHP) done"
	@touch $@


#
# Patch the source
#
.PHONY: php-patched

php-patched: $(PHP_DIR)/.php_patched

$(PHP_DIR)/.php_patched: $(PHP_DIR)/.php_unpacked
	@touch $@

#
# config
#
.PHONY: php-config

php-config: 

#
# Configuration
#
.PHONY: php-configured

php-configured:  php-config $(PHP_DIR)/.php_configured

$(PHP_DIR)/.php_configured: $(PHP_DIR)/.php_patched $(PHP_CONFIG) $(TOP_CURRENT_SET)
	@echo "Configuring $(PHP) ..."
	export ac_cv_php_xml2_config_path=$(LIBS_INSTALL_PATH)/bin/xml2-config; \
	cd $(PHP_SOURCE)/$(PHP); \
	export LIBTOOL=$(LIBS_INSTALL_PATH)/$(CONF_WEB_SERVER_APACHE_INSTALL_PATH)/build/libtool;\
	./configure  --prefix=$(CONF_WEB_SERVER_APACHE_INSTALL_PATH) \
		--build=$(BUILD_HOST) \
		--host=$(BUILD_TARGET) \
		--enable-sockets \
		--without-iconv \
		--with-pcre-regex \
		--with-openssl=$(LIBS_INSTALL_PATH) \
		--with-pcre-dir=$(LIBS_INSTALL_PATH) \
		--with-zlib-dir=$(LIBS_INSTALL_PATH) \
		--with-libxml-dir=$(LIBS_INSTALL_PATH) \
		--with-curl=$(LIBS_INSTALL_PATH) \
		--with-gettext=$(LIBS_INSTALL_PATH) \
		--enable-simplexml \
		--enable-libxml \
		--enable-pdo \
		--with-pdo-sqlite=$(LIBS_INSTALL_PATH) \
		--with-jpeg-dir=$(LIBS_INSTALL_PATH) \
		--with-png-dir=$(LIBS_INSTALL_PATH) \
		--with-zlib-dir=$(LIBS_INSTALL_PATH) \
		--enable-json \
		--enable-dom \
		--enable-reflection \
		--enable-pcre \
		--enable-hash \
		--enable-session \
		--with-gd \
		--enable-libxml \
		--enable-exif \
		--enable-mbstring \
		--enable-dom \
		--enable-xmlreader \
		--disable-xmlwriter  \
		--without-pear \
		--with-apxs2=$(LIBS_INSTALL_PATH)/$(CONF_WEB_SERVER_APACHE_INSTALL_PATH)/bin/apxs 
	@echo "Configuring $(PHP) done"
	@touch $@

#
# Compile
#
.PHONY: php-compile

php-compile: $(PHP_DIR)/.php_compiled

$(PHP_DIR)/.php_compiled: $(PHP_DIR)/.php_configured
	@echo "Compiling $(PHP) ..."
	export EXTRA_CFLAGS=-I$(LIBS_INSTALL_PATH)/$(CONF_WEB_SERVER_APACHE_INSTALL_PATH)/include; make -C $(PHP_SOURCE)/$(PHP) 
	export LIBTOOL=$(LIBS_INSTALL_PATH)/$(CONF_WEB_SERVER_APACHE_INSTALL_PATH)/build/libtool; export INSTALL_ROOT=$(LIBS_INSTALL_PATH) ; make -C $(PHP_SOURCE)/$(PHP)  install
	@echo "Compiling $(PHP) done"
	@touch $@

#
# Update
#
.PHONY: php-update

php-update:
	@echo "Updating $(PHP) ..."
	@if [ -d "$(PHP_SOURCE)" ]; then\
		$(TOP_UPDATE) $(PHP_SOURCE); \
	fi
	@echo "Updating $(PHP) done"

#
# Status
#
.PHONY: php-status

php-status:
	@echo "Statusing $(PHP) ..."
	@if [ -d "$(PHP_SOURCE)" ]; then\
		$(TOP_STATUS) $(PHP_SOURCE); \
	fi
	@echo "Statusing $(PHP) done"

#
# Clean
#
.PHONY: php-clean

php-clean:
	@rm -f $(PHP_DIR)/.php_configured
	@if [ -d "$(PHP_SOURCE)" ]; then\
		make -C $(PHP_SOURCE)/$(PHP) clean; \
	fi

#
# Dist clean
#
.PHONY: php-distclean

php-distclean:
	@rm -f $(PHP_DIR)/.php_*

#
# Install
#
.PHONY: php-install

php-install:
	cp -f $(PHP_SOURCE)/$(PHP)/./php.ini-dist $(TOP_INSTALL_ROOTFS_DIR)/$(CONF_WEB_SERVER_APACHE_INSTALL_PATH)/lib/php.ini 
	sed -i -e s,^LoadModule\ php5_module.*,LoadModule\ php5_module\ $(CONF_WEB_SERVER_APACHE_INSTALL_PATH)/modules/libphp5.so, $(TOP_INSTALL_ROOTFS_DIR)/$(CONF_WEB_SERVER_APACHE_INSTALL_PATH)/conf/httpd.conf
	cd $(TOP_INSTALL_ROOTFS_DIR)/$(CONF_WEB_SERVER_APACHE_INSTALL_PATH)/conf; patch -p0 < $(PHP_SOURCE)/httpd.conf.patch 


