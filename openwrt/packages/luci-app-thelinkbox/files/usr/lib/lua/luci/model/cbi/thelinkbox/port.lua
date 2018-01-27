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
        
        $Log: port.lua,v $
        Revision 1.1  2009/06/05 16:24:17  wb6ymh
        Initial import - OpenWRT/LuCI app for thelinkbox.

]]--
require("luci.tools.webadmin")
local util = require("luci.util")
local instanceof = util.instanceof
local class      = util.class
arg[1] = arg[1] or ""

m = Map("thelinkbox", translate("port_configuration"), translate("port_page_info"))

OurNamedSection = class(NamedSection)

function OurNamedSection.option(self, class, option, title, description)
        local obj  = NamedSection.option(self, class, option, title, description)

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


s = m:section(OurNamedSection, arg[1], "port")
s.addremove = true
s.anonymous = true
s.dynamic = true

p = s:option(Flag, "PortEnabled")
p.rmempty = false

p = s:option(Value, "AudioDevice")

--[[
; The audio channel to use on the specified audio device.
;  0 = Mono
;  1 = right
;  2 = left
; 
AudioChannel = 0
]]--
p = s:option(ListValue, "AudioChannel")
p:value(0, "Mono")
p:value(1, "right")
p:value(2, "left")
p.optional = true

p = s:option(ListValue, "control")
p.override_values = true
p:value("serial", "serial port")
p:value("parallel", "parallel port")
p:value("usb", "USB GPIO")

p = s:option(Value, "DevName")
p:depends("control", "serial")

p = s:option(ListValue, "TxKeyMethod")
p.override_values = true
p:value("0", "none")
p:value("1", "WB2REM / VA3TO style Echolink interface",{control="serial"})
p:value("2", "Parallel port",{control="parallel"})
p:value("3", translate("RTS"),{control="serial"})
p:value("4", "DTR",{control="serial"})
p:value("5", "USB GPIO",{control="usb"})

s:option(Value, "PTT_Bit").optional = true
s:option(Flag, "InvertPTT").optional = true

p = s:option(ListValue, "RxCosMethod")
p:value("0", "none")
p:value("1", "vox")
p:value("2", "Parallel port",{control="parallel"})
p:value("3", "CTS",{control="serial"})
p:value("4", "DSR",{control="serial"})
p:value("5", "DCD",{control="serial"})
p:value("6", "Linux HID events",{control="usb"})
p:value("7", "USB GPIO",{control="usb"})

s:option(Value, "COS_Bit").optional = true
s:option(Flag, "InvertCOS").optional = true

p = s:option(ListValue, "RxCtcssMethod")
p:value("0", "none")
p:value("1", "Software")
p:value("2", "Parallel port",{control="parallel"})
p:value("3", "CTS",{control="serial"})
p:value("4", "DSR",{control="serial"})
p:value("5", "DCD",{control="serial"})
p:value("6", "Linux HID events",{control="usb"})
p:value("7", "USB GPIO",{control="usb"})

p = s:option(Value, "CTCSS_Bit")
p:depends("RxCtcssMethod", "2")
p:depends("RxCtcssMethod", "3")
p:depends("RxCtcssMethod", "4")
p:depends("RxCtcssMethod", "5")
p:depends("RxCtcssMethod", "7")

p = s:option(Flag, "InvertCTCSS")
p:depends("RxCtcssMethod", "2")
p:depends("RxCtcssMethod", "3")
p:depends("RxCtcssMethod", "4")
p:depends("RxCtcssMethod", "5")
p:depends("RxCtcssMethod", "7")
p.optional = true

--[[
; The following variable sets the mode of CTCSS operation.
; 0 - Carrier squelch only
; 1 - Carrier and CTCSS
; 2 - Carrier or CTCSS
; 3 - CTCSS only
; 4 - Carrier and NO CTCSS (anti-CTCSS)
;
; Mode 1 requires both CTCSS and carrier detect for the squelch to open, this
;   is the mode typically used for CTCSS access.
;
; Mode 2 is sometimes useful when the carrier squelch is tight, in this mode
;   the carrier squelch must open to begin reception, but the presence of
;   CTCSS is enough to hold the squelch open.
;
; Mode 3 only requires CTCSS to open the squelch, COS is ignored. This 
;   usually provides the most sensitive squelch.
;
; Mode 4 (Anti-CTCSS) maybe useful for a carrier squelch repeater with 
;   overlapping coverage with a repeater that requires CTCSS.
;
; Note: Modes 2 and 3 usually result in an unacceptably long squelch tails
;   when used with transmitters that do not implement reverse burst.
;RxCTCSSMode = 1
]]--

p = s:option(ListValue, "RxCTCSSMode", "Rx CTCSS mode")
p:value("0", "Carrier")
p:value("1", "COS and CTCSS")
p:value("2", "COS or CTCSS")
p:value("3", "CTCSS only")
p:value("4", "COS and NO CTCSS")
p:depends("RxCtcssMethod", "1")
p:depends("RxCtcssMethod", "2")
p:depends("RxCtcssMethod", "3")
p:depends("RxCtcssMethod", "4")
p:depends("RxCtcssMethod", "5")
p:depends("RxCtcssMethod", "6")
p:depends("RxCtcssMethod", "7")


--[[
; The following variable selects the method thelinkbox will use to decode
; DTMF tones:
; 0 - none
; 1 - WB2REM / VA3TO style serial EchoLink interface boards
; 2 - IRLP parallel port interface
; 3 - Software based DTMF decoder
; 4 - Event based DTMF decoding from USB HID device (See DTMF_x_KeyCode)
; 5 - Asterisk DTMF tunnelling protocol (Voip ports only) 
DtmfMethod = 3
]]--

p = s:option(ListValue, "DtmfMethod")
p:value("0", "none")
p:value("1", "WB2REM / VA3TO style Echolink interface",{control="serial"})
p:value("2", "IRLP parallel interface",{control="parallel"})
p:value("3", "Software")

p=s:option(Value, "DTMFCommandFile")
p:depends("DtmfMethod", 1)
p:depends("DtmfMethod", 2)
p:depends("DtmfMethod", 3)

--[[
; The following variable specifies if CTCSS is required to enter DTMF commands.
;
; 0 - CTCSS not required (even if CTCSS is enabled by RxCTCSSMode)
; 1 - CTCSS required if CTCSS is enabled by RxCTCSSMode
; 2 - CTCSS always required
;
;DtmfCTCSSMode = 1
]]--

p=s:option(ListValue, "DtmfCTCSSMode")
p:value("0", translate("ctcssmode0"))
p:value("1", translate("ctcssmode1"))
p:value("2", translate("ctcssmode2"))
p:depends("RxCtcssMethod", "2")
p:depends("RxCtcssMethod", "3")
p:depends("RxCtcssMethod", "4")
p:depends("RxCtcssMethod", "5")
p:depends("RxCtcssMethod", "7")
p.optional = true

p=s:option(Value, "Twist")
p.optional = true
p:depends("DtmfMethod","3")

p=s:option(Value, "ReverseTwist")
p.optional = true
p:depends("DtmfMethod","3")

s:option(Flag, "MuteDTMF").optional = true

s:option(Flag, "RepeaterPort").optional = true

p=s:option(Value, "Ctone")
p:depends("RepeaterPort","1")

p=s:option(Value, "CtoneDelay")
p:depends("RepeaterPort","1")

p=s:option(Flag, "CtoneResetsTimeout")
p:depends("RepeaterPort","1")


p = s:option(ListValue, "RadioCtrlMethod")
p:value("0", "none")
p:value("1", "Doug Hall",{control="parallel"})
p:value("2", "Doug Hall",{control="usb"})
p:depends("control","parallel")
p:depends("control","serial")
p.optional = true

p = s:option(Flag, "TxCtcssMethod", "Software CTCSS generator")
p.optional = true

p = s:option(Value, "CTCSSLevel")
p:depends("TxCtcssMethod",1)
p.optional = true


s:option(Value, "HangTime").optional = true

s:option(Value, "TxTimeout").optional = true
s:option(Value, "TxTimeoutTone").optional = true
s:option(Value, "RxTimeout").optional = true
s:option(Value, "RxTimeoutTone").optional = true
s:option(Flag, "RxDeemphasis").optional = true
s:option(Flag, "TxPreemphasis").optional = true
s:option(Flag, "TxCtcssFilter").optional = true

--[[
; Set the following variable to require a toneburst to activate this port's
; transmitter.
; 0 - tone burst not required
; 1 - tone burst required to key up repeater initially.
; 2 - tone burst or CTCSS required to key up repeater initially.
;ToneBurstMode = 0
--]]
p = s:option(ListValue, "ToneBurstMode", "Tone Burst Mode")
p:value("0", "Tone burst not required")
p:value("1", "Tone burst required to access repeater")
p:value("2", "Tone burst or CTCSS required to access the repeater")
p.optional = true

p=s:option(Value, "ToneBurstFreq")
p:depends("ToneBurstMode","1")
p:depends("ToneBurstMode","2")

p=s:option(Flag, "ToneBurstMute")
p:depends("ToneBurstMode","1")
p:depends("ToneBurstMode","2")

p=s:option(Value, "ToneBurstTimeout")
p:depends("ToneBurstMode","1")
p:depends("ToneBurstMode","2")

p=s:option(Value, "ToneBurstToneID")
p:depends("ToneBurstMode","1")
p:depends("ToneBurstMode","2")

p=s:option(Flag, "FakeToneBurst")
p:depends("ToneBurstMode","1")
p:depends("ToneBurstMode","2")

s:option(Value, "CWId").optional = true

p=s:option(Flag, "LinkVoipIn")
p.override_scheme = true
p.rmempty = false
p.default = "1"


p=s:option(Flag, "LinkVoipOut")
p.rmempty = false


return m
