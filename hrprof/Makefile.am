# $Log: Makefile.am,v $
# Revision 1.2  2010/01/14 19:34:00  wb6ymh
# String -finstrument-functions from CFLAGS or else we try to profile
# the profiling library which, well, doesn't work very well!
#
# Revision 1.1  2010/01/14 19:12:12  wb6ymh
# Inital import - profiling library that uses the Pentium performance
# counters from http://sourceforge.net/projects/hrprof.
#
#

EXTRA_DIST = COPYING README README.1ST tprog.c Makefile.orig

CFLAGS := $(subst -finstrument-functions,,$(CFLAGS))

noinst_LIBRARIES = libhrprof.a

libhrprof_a_SOURCES = hrprof.c


