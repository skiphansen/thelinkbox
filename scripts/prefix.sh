#!/bin/sh
#
# This script is part of a DTMF regeneration scheme for systems that use
# site prefixes.
#
# When a prefix and command are entered as one string this script simply
# regenerates the DTMF string.  If the prefix is entered alone the script
# saves the prefix, starts a dialtone and prefix timeout timer, and changes 
# the command file to tlb.regen.  The regen.cmd file simply passes all 
# command strings to regen.sh.  regen.sh prepends the saved prefix to 
# received commands and then regenerates the combined DTMF string.  The
# dialtone is cancelled and the command file is set back to the normal
# tlb.cmds.
#
# An entry in thelinkbox's command file calls this script when a prefix
# is matched.  For example if prefixes are 3 more more digits and always start 
# with a DTMF "AB" the following line would call this script for all prefixes:
# AB+ = script ./prefix.sh $1
# 
#
# $1 - prefix or prefix + commands
#
# $Id: prefix.sh,v 1.1 2008/06/21 15:55:22 wb6ymh Exp $
# 
# set this to the length of your prefix
predigits=4

firstcmddigit=$( expr $predigits + 1)

prefix=`echo $1 | cut -c 1-${predigits}`
cmd=`echo $1 | cut -c ${firstcmddigit}-`

if [ "${cmd}x" != "x" ]; then
   # we have a prefix plus a command, just regenerate it now
   tlbcmd "tonegen -p dtmf ${prefix}${cmd}"
else
   # we just have a prefix, save it for later
   echo $prefix > ${TLB_PORT}.prefix
   # change the command file to tlb.regen, start a dialtone and 
   # a prefix timeout timer
   tlbcmd 'set dtmfcommandfile = regen.cmds; tonegen #104; tonegen -c "set dtmfcommandfile = tlb.cmds" id 999 dur 10000'
fi


