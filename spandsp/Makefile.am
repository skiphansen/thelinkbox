# $Log: Makefile.am,v $
# Revision 1.2  2010/11/11 05:44:26  wb6ymh
# Added files needed to support g.726.
#
# Revision 1.1.1.1  2007/04/07 22:27:59  wb6ymh
# Initial import of a small subset of the spandsp library for CQiNet.
# Currently all we are using are the DTMF decoder routines.
#
#
#

EXTRA_DIST = AUTHORS COPYING DueDiligence README README.1ST

noinst_LIBRARIES = libspandsp.a

libspandsp_a_SOURCES = dtmf.c super_tone_rx.h tone_detect.h tone_generate.h \
                       tone_detect.c telephony.h dtmf.h g726.c bitstream.c \
		       g711.h g726.h bitstream.h dc_restore.h bit_operations.h

AM_CPPFLAGS = -I$(top_srcdir)
AM_CFLAGS = -Wall 


