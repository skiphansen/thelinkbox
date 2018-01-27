# $Log: Makefile.am,v $
# Revision 1.3  2008/02/26 18:02:08  wb6ymh
# Added dmalloc support.
#
# Revision 1.2  2002/12/18 00:54:08  wb6ymh
# Added ntservice.c.
#
# Revision 1.1.1.1  2002/11/02 17:03:55  wb6ymh
# Moved common source files to common subdirectory from src.
#

noinst_LIBRARIES = libcommon.a

libcommon_a_SOURCES = avl.c configfile.c main.c
EXTRA_DIST = ntservice.c

AM_CPPFLAGS = -I$(srcdir)/../inc
AM_CFLAGS = -Wall

if USE_DMALLOC
AM_CFLAGS += -DUSE_DMALLOC
endif


