# $Log: Makefile.am,v $
# Revision 1.9  2010/01/14 19:36:21  wb6ymh
# Modified to use the Pentium performance counter based hrprof library for
# profiling.
#
# Revision 1.8  2008/12/30 16:33:14  wb6ymh
# Added soundmodem to SUBDIRS.
#
# Revision 1.7  2008/03/09 17:46:02  wb6ymh
# Added voip.conf.sample.
#
# Revision 1.6  2008/01/28 15:47:13  wb6ymh
# Added scripts to SUBDIRS.
#
# Revision 1.5  2007/12/23 22:07:36  wb6ymh
# Cosmetic: tbdcmd->tlbcmd for thelinkbox.
#
# Revision 1.4  2007/11/26 06:53:57  wb6ymh
# 1. Added new sample port configuration files.
# 2. Added TODO SCRIPTING.txt to EXTRA_DIST.
# 3. Added code to dist-hook to copy thebridge's README to README.tbd.
#
# Revision 1.3  2007/11/21 17:33:23  wb6ymh
# 1. Added tlb.cmds.sample.
# 2. Removed copy of tlb.conf.sample from dist-hook, that's not how it's done
#    since tlb.conf.sample is already in sysconf_DATA.
#
# Revision 1.2  2007/06/29 23:41:03  wb6ymh
# 1. Added adpcm and linkbox to SUBDIRS.
# 2. Removed config from EXTRA_DIST.
#
# Revision 1.1.1.1  2007/04/07 23:55:38  wb6ymh
# Inital import - top level for thelinkbox project.
#
#

SUBDIRS = spandsp gsm adpcm common zlib src inc soundmodem 

if PROFILED_BUILD
SUBDIRS += hrprof
endif

SUBDIRS += linkbox tlbcmd scripts

sysconf_DATA = tlb.conf.sample tlb.cmds.sample port.conf.sample

EXTRA_DIST = FreeBSD RedHat TODO cm108.conf.sample iMic.conf.sample \
	irlp.conf.sample wb2rem.conf.sample tlb.conf.sample \
	tlb.cmds.sample port.conf.sample SCRIPTING.txt rigblaster.conf.sample \
	voip.conf.sample

dist-hook:
	rm -rf $(distdir)/FreeBSD/CVS
	rm -rf $(distdir)/RedHat/CVS
	if test -d thebridge; then \
		cp  -p thebridge/README $(distdir)/README.tbd; \
	fi;

