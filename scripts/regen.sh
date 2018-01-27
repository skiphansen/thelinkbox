#!/bin/sh
# This script file is part of a DTMF regeneration scheme for systems that use
# site prefixes.  See prefix.sh for more information.
#
# We're in DTMF regeneration mode, prepend the active prefix to the commands 
# received and then send them to our peers.  Change the command file
# back to tlb.cmds to exit DTMF regeneration mode.
#
# $Id: regen.sh,v 1.1 2008/06/21 15:55:22 wb6ymh Exp $
prefix=`cat ${TLB_PORT}.prefix`
tlbcmd "tonegen !104;tonegen -p dtmf ${prefix}${1};set dtmfcommandfile = tlb.cmds; tonegen !999"


