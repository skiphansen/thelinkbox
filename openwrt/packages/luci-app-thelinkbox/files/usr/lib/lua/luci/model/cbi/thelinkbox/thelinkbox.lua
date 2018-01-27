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
        
        $Log: thelinkbox.lua,v $
        Revision 1.1  2009/06/05 16:24:17  wb6ymh
        Initial import - OpenWRT/LuCI app for thelinkbox.

]]--
require("luci.tools.webadmin")
local util = require("luci.util")
local instanceof = util.instanceof
local class      = util.class

local created

OurTypedSection = class(TypedSection)

function OurTypedSection.option(self, class, option, title, description)
        local obj  = AbstractSection.option(self, class, option, title, description)

        if instanceof(class, AbstractValue) and self.sectiontype then
                if option  then 
                -- remove underscore from option name
                        local no_underline = tostring(option):gsub("_", "")
                        Node._i18n(obj, self.config, self.sectiontype, no_underline, title, description)
                else
                        Node._i18n(obj, self.config, self.sectiontype, option, title, description)
                end
        end
        return obj
end

m = Map("thelinkbox",translate("global_page_title"))

s = m:section(TypedSection, "global","")
s.anonymous = true
s.dynamic = true
conf_call = s:option(Value, "ConferenceCall")

p = s:option(Flag, "EchoLinkEnable")
p.rmempty = false

p = s:option(Value, "ConferencePass")
p.password = true

s:option(Value, "ConferenceQth")
s:option(Value, "EmailAdr")
s:option(Flag, "FullDuplex")

p = s:option(Flag, "EnableRemoteDTMF")
p:depends("EchoLinkEnable","1")

s:option(Value, "TTS_ExePath").optional = true
s:option(Flag, "SF_Enable").optional = true
p = s:option(Value, "RTP_Pass")
p.optional = true
p.password = true
s:option(Value, "SF_Port").optional = true

s1 = m:section(TypedSection, "port")
s1.template  = "cbi/tblsection"
s1.extedit   = luci.dispatcher.build_url("admin", "thelinkbox", "config", "port") .. "/%s"
s1.override_scheme = true
s1.defaults.LinkVoipIn = 1
s1.defaults.LinkVoipOut = 1
s1.defaults.PortEnabled = 1

function s1.create(self, section)
        if TypedSection.create(self, section) then
                created = section
        else
                self.invalid_cts = true
        end
end

function s1.parse(self, ...)
        TypedSection.parse(self, ...)
        if created then
                m.uci:save("thelinkbox")
                luci.http.redirect(luci.dispatcher.build_url("admin", "thelinkbox", "config", "port")
                 .. "/" .. created)
        end
end

return m
