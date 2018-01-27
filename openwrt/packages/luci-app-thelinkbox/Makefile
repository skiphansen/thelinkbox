#  Copyright (C) 2009 GFRN systems
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License as
#  published by the Free Software Foundation; either version 2 of the
#  License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
#  02111-1307, USA.
#
#  The latest version of this program may be found at
#  http://CQiNet.sourceforge.net
#
# $Log: Makefile,v $
# Revision 1.1  2009/06/05 16:24:17  wb6ymh
# Initial import - OpenWRT/LuCI app for thelinkbox.
#
#
include $(TOPDIR)/rules.mk

PKG_NAME:=luci-app-thelinkbox
PKG_VERSION:=0.01
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
  
define Package/luci-app-thelinkbox
  SECTION:=admin
  CATEGORY:=Administration
  SUBMENU:=LuCI Components
  TITLE:=Thelinkbox         
  DEPENDS:=+luci-web +flite
endef

define Package/luci-app-thelinkbox/description
	Luci control and configuration for thelinkbox
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/luci-app-thelinkbox/conffiles
/etc/tlb.defaults
endef

define Package/luci-app-thelinkbox/install
	$(INSTALL_DIR) $(1)
	$(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,luci-app-thelinkbox))

