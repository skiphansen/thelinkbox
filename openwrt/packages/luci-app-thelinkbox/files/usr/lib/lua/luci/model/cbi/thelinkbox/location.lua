--[[
        Copyright (C) 2009 GFRN systems
        
        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License as
        published by the Free Software Foundation; either version 2 of the
        License, or (at your option) any later version.
        
        This program is distributed in the hope that it will be useful, but
        WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
        See the GNU General Public License for more details.
        
        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
        02111-1307, USA.
        
        The latest version of this program may be found at
        http://CQiNet.sourceforge.net
        
        $Log: location.lua,v $
        Revision 1.1  2009/06/05 16:24:17  wb6ymh
        Initial import - OpenWRT/LuCI app for thelinkbox.

]]--
require("luci.tools.webadmin")

local created

m = Map("thelinkbox", translate("location_page_info"), "")
s = m:section(TypedSection, "location","")
s.anonymous = true
s.addremove = false

s:option(Flag, "AvrsEnable")
s:option(Flag, "AprsIsEnable")

p = s:option(Value, "Latitude")
p = s:option(Value, "Longitude")
p = s:option(Value, "Haat")
p = s:option(Value, "AntGain")
p = s:option(Value, "AntDir")
p = s:option(Value, "TxPower")
p = s:option(Value, "Frequency")
p = s:option(Value, "RptOffset")
p = s:option(Value, "CTCSS")

p = s:option(Value, "AprsObjectName")
p:depends("AprsIsEnable", 1)
p.optional = true

p = s:option(Value, "AprsSymbolTable")
p:depends("AprsIsEnable", 1)
p.optional = true

p = s:option(Value, "AprsSymbolCode")
p:depends("AprsIsEnable", 1)
p.optional = true

p = s:option(Value, "AprsIsInterval")
p:depends("AprsIsEnable", 1)
p.optional = true

return m
