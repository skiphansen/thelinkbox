# $Log: Makefile.am,v $
# Revision 1.11  2010/01/14 19:34:53  wb6ymh
# Added libhrprof to link when profiling.
#
# Revision 1.10  2008/12/30 16:11:55  wb6ymh
# 1. Added devices.h, devices.cpp, libsoundmodem.a.
# 2. Moved -lm for math library to where it belongs in tlb_LDADD from AM_CLFAGS.
#
# Revision 1.9  2008/07/23 15:42:56  wb6ymh
# Added ulaw2linear.h, ulaw.c.
#
# Revision 1.8  2008/05/14 18:20:27  wb6ymh
# Added Ctext.cpp, Ctext.h, radio_ctrl.h, dh.cpp.
#
# Revision 1.7  2008/04/24 23:55:20  wb6ymh
# Added filters.cpp, filters.h.
#
# Revision 1.6  2008/03/09 17:02:03  wb6ymh
# 1. Added dmalloc support.
# 2. Deleted audio.h, audio.cpp.
# 3. Added voip.h, voip.cpp, nodes.cpp, linkbox.h.
#
# Revision 1.5  2007/12/14 23:23:50  wb6ymh
# Turned on -Wall for c++ compiles.
#
# Revision 1.4  2007/12/01 01:03:30  wb6ymh
# Added ToneGen.h, ToneGen.cpp.
#
# Revision 1.3  2007/11/26 07:05:18  wb6ymh
# 1. Converted audio.c devinput.c, and usbdev.c to .cpp.
# 2. Added define for SYSCON_DIR.
#
# Revision 1.2  2007/11/21 16:30:18  wb6ymh
# 1. Added devinput.c and devinput.h.
# 2. Added usbdev.c conditioned by USB_SUPPORT.
#
# Revision 1.1  2007/06/30 00:04:25  wb6ymh
# Initial import - source files for thelinkbox.
#
#

libexec_PROGRAMS = tlb
tlb_SOURCES = ports.cpp ctcss.c ctcss.h devinput.cpp devinput.h\
        configvars.cpp ports.h usbdev.h ToneGen.h ToneGen.cpp voip.cpp \
        voip.h nodes.h nodes.cpp linkbox.h filters.cpp filters.h \
        Ctext.cpp Ctext.h radio_ctrl.h dh.cpp ulaw2linear.h ulaw.c \
        devices.h devices.cpp
        

tlb_LDADD = $(top_builddir)/src/libtbd.a $(top_builddir)/common/libcommon.a \
        $(top_builddir)/zlib/libzlib.a $(top_builddir)/gsm/src/libgsm.a \
        $(top_builddir)/spandsp/libspandsp.a $(top_builddir)/adpcm/libadpcm.a \
        $(top_builddir)/src/libtbd.a $(LIBDMALLOC) \
        $(top_builddir)/soundmodem/libsoundmodem.a

if PROFILED_BUILD
tlb_LDADD += $(top_builddir)/hrprof/libhrprof.a
endif
    

AM_CPPFLAGS = -I$(top_srcdir)/inc -I$(top_srcdir)/gsm/inc -DLINK_BOX \
        -I$(top_srcdir)/zlib -I$(top_srcdir)/spandsp -I$(top_srcdir)/adpcm \
        -I$(top_srcdir)/src -DSYSCON_DIR=\"$(sysconfdir)\"
        
AM_CFLAGS = -Wall -DSYSCON_DIR=\"$(sysconfdir)\"
AM_CXXFLAGS = $(AM_CFLAGS)

tlb_LDADD += -lm

if USB_SUPPORT
tlb_SOURCES += usbdev.cpp
tlb_LDADD += -lusb
AM_CPPFLAGS += -DUSB_SUPPORT 
endif

if USE_DMALLOC
AM_CFLAGS += -DUSE_DMALLOC
AM_CPPFLAGS += -DUSE_DMALLOC
endif



