# $Id: Makefile,v 1.3 2009/06/03 05:04:00 wb6ymh Exp $
include $(TOPDIR)/rules.mk

PKG_NAME:=luci-theme-thelinkbox
PKG_VERSION:=0.02
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
  
define Package/luci-theme-thelinkbox
  SECTION:=admin
  CATEGORY:=Administration
  SUBMENU:=LuCI Themes
  TITLE:=Thelinkbox         
  MAINTAINER:=Skip Hansen <wb6ymh-at-gfrn-dot-org>
  DEPENDS:=+luci-web
endef

define Package/luci-theme-thelinkbox/description
	A Ham radio Luci theme for thelinkbox
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/luci-theme-thelinkbox/conffiles
endef

define Package/luci-theme-thelinkbox/install
	$(INSTALL_DIR) $(1)
	$(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,luci-theme-thelinkbox))

