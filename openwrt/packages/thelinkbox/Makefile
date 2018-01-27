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
# Revision 1.2  2009/09/15 17:55:14  wb6ymh
# Updated to the  0.44 release.
#
# Revision 1.1  2009/06/05 16:25:59  wb6ymh
# Inital import OpenWRT package for thelinkbox.
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:=thelinkbox
PKG_VERSION:=0.44
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tgz
PKG_SOURCE_URL:=http://download.thelinkbox.net/

PKG_MD5SUM:=92543967c5ec11ac572bc38c8319392e
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/thelinkbox
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libusb +libreadline +libstdcpp
  TITLE:=Ham Radio repeater controller and RoIP package  
  URL:=http://groups.yahoo.com/group/thelinkbox/
  MAINTAINER:=Skip Hansen <wb6ymh-at-gfrn-dot-org>
endef

define Package/thelinkbox/description
 Thelinkbox (tlb) is a repeater controller and radio over IP (RoIP) linking
 package for Ham radio operators.
endef

define Package/thelinkbox/conffiles
/etc/tlb.conf.sample
/etc/port.conf.sample
/etc/tlb.cmds.sample
endef

define Build/Configure
	$(call Build/Configure/Default, \
		--enable-usb \
		--with-readline \
	)
endef

define Package/thelinkbox/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/linkbox/tlb $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tlbcmd/tlbcmd $(1)/usr/bin/
	(cd $(1)/usr/bin; ln -sf tlbcmd tlbchat)
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/tlb.conf.sample $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/port.conf.sample $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/tlb.cmds.sample $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/thelinkbox.init $(1)/etc/init.d/thelinkbox
endef

$(eval $(call BuildPackage,thelinkbox))
