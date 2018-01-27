--[[
LuCI - Lua Configuration Interface
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
        
        $Log: conference.lua,v $
        Revision 1.1  2009/06/05 16:24:17  wb6ymh
        Initial import - OpenWRT/LuCI app for thelinkbox.

]]--
require("luci.tools.webadmin")

m = Map("thelinkbox", translate("conference_page_info"), "")
s = m:section(TypedSection, "conference","")
s.anonymous = true
s.addremove = false
s.dynamic = true

s:option(Flag, "ConfEnable")
s:option(Value, "MaxConferenceClients")
s:option(Flag, "PrivateConference")
s:option(Value, "ConferenceID")
p=s:option(Value, "AdminPass")
p.password = true

p=s:option(Value, "SysopPass")
p.password = true

s:option(Flag, "UserCountInLocation")
p = s:option(Flag, "UserCountEnable")
p:depends("UserCountInLocation", 1)

p = s:option(Flag, "MaxCountEnable")
p:depends("UserCountInLocation", 1)


s:option(Flag, "AsteriskEnable").optional = true
s:option(Value, "AsteriskPort").optional = true

return m
