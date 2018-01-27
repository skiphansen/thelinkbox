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
module("luci.controller.thelinkbox", package.seeall)

function index()
        require("luci.i18n")
        local uci = require("luci.model.uci").cursor()
        luci.i18n.loadc("thelinkbox")
        if not luci.fs.access("/etc/config/thelinkbox") then
                return
        end
        local page  = node("admin", "thelinkbox")
        page.target = alias("admin", "thelinkbox", "config", "port")
        page.title  = "Thelinkbox"
        page.order  = 50
        page.index  = true

        entry({"admin", "thelinkbox", "cmd"}, call("action_tlbcmd"), "Command line", 1)
        entry({"admin", "thelinkbox", "config"}, arcombine(cbi("thelinkbox/thelinkbox"),cbi("thelinkbox/port")), "Configuration", 10)
        local page = entry({"admin", "thelinkbox", "config", "port"}, arcombine(cbi("thelinkbox/thelinkbox"),cbi("thelinkbox/port")), "Ports", 10)
        page.leaf   = true
        page.subindex = true
        
        
        uci:foreach("thelinkbox", "port",
                function (section)
                        local port = section[".name"]
                                entry({"admin", "thelinkbox", "config", "port", port},
                                 true,
                                 port)
                end
        )

        local page = entry({"admin", "thelinkbox", "config", "conference"}, cbi("thelinkbox/conference"), "Conferencing", 20)
        page.leaf   = true
        
        local page = entry({"admin", "thelinkbox", "config", "location"}, cbi("thelinkbox/location"), "Location Reporting", 30)
        page.leaf   = true
        
        entry({"admin", "thelinkbox", "about"}, template("thelinkbox/about"), "About", 99)
end

function action_tlbcmd()
        require("luci.util")
        local void = nil
        local tlbcmd_log_path = "/tmp/tlbcmd.log"
        
        local cmd = luci.http.formvalue("cmd")
        if cmd and cmd ~= '' then
                cmd = luci.util.escape(cmd,'"')
                luci.util.exec("/bin/echo \"tlbcmd> " .. cmd .. "\" >> /tmp/tlbcmd.log")
                luci.util.exec("/usr/bin/tlbcmd -s \"" .. cmd .. "\" > /tmp/tlbcmd.tmp")
                result = luci.util.trim(luci.util.exec("/usr/bin/head -n 1 /tmp/tlbcmd.tmp"))
                
                if result ~= "0" then
                        luci.util.exec("/bin/echo \"result: " .. result .."\" > /tmp/result")
                        luci.util.exec("/bin/echo \"Error: tlbcmd returned " .. result .. "\" >> /tmp/tlbcmd.log")
                else
                        luci.util.exec("/usr/bin/tail -n +2 /tmp/tlbcmd.tmp | cat /tmp/tlbcmd.log - | tail -n100 > /tmp/tlbcmd.tmp; mv /tmp/tlbcmd.tmp /tmp/tlbcmd.log")
                end
        end
   
        if not posix.stat(tlbcmd_log_path) then
                tlbcmd_log = ""
        else
                tlbcmd_log = luci.util.exec("/bin/cat /tmp/tlbcmd.log")
        end
        
        tlbcmd_log = tlbcmd_log .. "tlbcmd>"
        
        luci.template.render("thelinkbox/tlbcmd",{tlbcmd_log=tlbcmd_log})
end
