# $Log: Makefile.am,v $
# Revision 1.10  2008/08/16 15:24:51  wb6ymh
# Moved -lm to tbd_LDADD where it belongs.
#
# Revision 1.9  2008/02/26 17:40:01  wb6ymh
# Added dmalloc support.
#
# Revision 1.8  2007/11/26 14:47:59  wb6ymh
# 1. Removed configvars.c from thelinkbox build, it is still used for thebridge
#    builds.
# 2. Added define for USB_SUPPORT when configured for USB.
#
# Revision 1.7  2007/06/30 00:09:12  wb6ymh
# Added -lm back to AM_CFLAGS for thebridge builds.
#
# Revision 1.6  2007/06/29 23:37:18  wb6ymh
# Added support for thelinkbox - put Echolink conference objects into a library,
# don't build the executable here.
#
# Revision 1.5  2004/11/27 22:11:06  wb6ymh
# Added new source files avrs.c, avrs.h to tbd_SOURCES.
# Added math library to tbd_LDADD.
#
# Revision 1.4  2003/10/09 14:46:43  wb6ymh
# Added zlib to include directories.
#
# Revision 1.3  2003/08/16 14:10:43  wb6ymh
# 1. Added eventhook.c, eventhook.h to tbd_SOURCES.
# 2. Added libzlib to tbd_LDADD.
#
# Revision 1.2  2002/11/02 18:37:31  wb6ymh
# Moved source files common with other projects to common.
# Moved header files comon to other projects to inc.
# Added compiler switch to include inc directory when compiling.
# Add link of new libcommon.a which contains the common files.
#
# Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
# initial import
#

if LINK_BOX
noinst_LIBRARIES = libtbd.a
libtbd_a_SOURCES = conference.c conference.h configvars.h \
                   dirclient.c dirclient.h thebridge.c users.c users.h \
                   hostfile.c hostfile.h eventhook.c eventhook.h avrs.c avrs.h 
              
AM_CPPFLAGS = -I$(top_srcdir)/inc -I$(top_srcdir)/gsm/inc -DLINK_BOX \
        -I$(top_srcdir)/zlib -I$(top_srcdir)/linkbox \
        -DSYSCON_DIR=\"$(sysconfdir)\"
AM_CFLAGS = -Wall 

if USB_SUPPORT
AM_CPPFLAGS += -DUSB_SUPPORT
endif

else

libexec_PROGRAMS = tbd
tbd_SOURCES = conference.c conference.h configvars.c configvars.h dirclient.c \
        dirclient.h thebridge.c users.c users.h hostfile.c hostfile.h \
        eventhook.c eventhook.h avrs.c avrs.h
              
tbd_LDADD = $(top_builddir)/common/libcommon.a $(top_builddir)/zlib/libzlib.a \
            $(LIBDMALLOC) -lm


AM_CPPFLAGS = -I$(top_srcdir)/inc -I$(top_srcdir)/zlib -I$(top_srcdir)/spandsp \
              -DSYSCON_DIR=\"$(sysconfdir)\"
AM_CFLAGS = -Wall

endif

if USE_DMALLOC
AM_CFLAGS += -DUSE_DMALLOC
endif

