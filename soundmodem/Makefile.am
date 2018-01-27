# $Log: Makefile.am,v $
# Revision 1.1  2008/12/25 15:36:35  wb6ymh
# Inital import - soundmodem files for AX25 packet generation.
#
#

EXTRA_DIST = AUTHORS COPYING README README.1ST ChangeLog

noinst_LIBRARIES = libsoundmodem.a

libsoundmodem_a_SOURCES = modem.c modem.h costab.h ax25.c costab.c costab.h

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/inc -I$(top_srcdir)/src
AM_CFLAGS = -Wall 


