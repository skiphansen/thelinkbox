# $Log: Makefile.am,v $
# Revision 1.9  2008/12/30 00:37:27  wb6ymh
# 1. Removed symlink generation for play, imike, ispeaker.  Eventually these
#    will be created elsewhere.
# 2. Added code to remove any old tbdchat or tlbchat symlinks on install
#    before attempting to create new links.
#
# Revision 1.8  2008/02/26 18:19:01  wb6ymh
# Added dmalloc support.:
#
# Revision 1.7  2007/12/23 21:18:59  wb6ymh
# Added readline support for tbdcmd.
#
# Revision 1.6  2007/12/14 22:59:01  wb6ymh
# Added readline lib to link.
#
# Revision 1.5  2007/12/07 23:16:17  wb6ymh
# Modfied to build the tlbcmd variation of tbdcmd when LINK_BOX is defined.
#
# Revision 1.4  2007/06/29 16:55:07  wb6ymh
# Added $(DESTDIR) to path for install and uninstall hooks.
#
# Revision 1.3  2007/06/29 14:32:39  wb6ymh
# 1. Added install hook to create a link from tbdcmd to tbdchat.
# 2. Added an uninstall hook to delete tbdchat symbolic link.
#
# Revision 1.2  2003/09/01 00:38:33  wb6ymh
# Uncomment EXTRA_DIST of tbdcmd.dsp.
#
# Revision 1.1  2003/08/16 14:27:27  wb6ymh
# Initial import: command line utility to send commands to thebridge.
#
#


AM_CPPFLAGS = -I$(srcdir)/../inc -I$(srcdir)/..
AM_CFLAGS = -Wall

if LINK_BOX
# building for thelinkbox ########

bin_PROGRAMS = tlbcmd
tlbcmd_SOURCES = tbdcmd.c
tlbcmd_LDADD = $(LIBREADLINE) $(LIBDMALLOC)
AM_CFLAGS += -DLINK_BOX

install-exec-hook:
	if test -f $(DESTDIR)$(bindir)/tlbchat; then \
		rm $(DESTDIR)$(bindir)/tlbchat; \
	fi
	ln -s $(DESTDIR)$(bindir)/tlbcmd $(DESTDIR)$(bindir)/tlbchat
        
uninstall-hook:
	rm $(DESTDIR)$(bindir)/tlbchat

else

# building for thebridge  ########
EXTRA_DIST = tbdcmd.dsp
bin_PROGRAMS = tbdcmd
tbdcmd_SOURCES = tbdcmd.c
tbdcmd_LDADD = $(LIBREADLINE) $(LIBDMALLOC)

install-exec-hook:
	if test -f $(DESTDIR)$(bindir)/tbdchat; then \
		rm $(DESTDIR)$(bindir)/tbdchat; \
	fi
	ln -s $(DESTDIR)$(bindir)/tbdcmd $(DESTDIR)$(bindir)/tbdchat
        
uninstall-hook:
	rm $(DESTDIR)$(bindir)/tbdchat

endif

if USE_DMALLOC
AM_CFLAGS += -DUSE_DMALLOC
endif

