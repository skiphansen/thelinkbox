# $Log: Makefile.am,v $
# Revision 1.1.1.1  2002/10/06 16:40:27  wb6ymh
# Initial import - GSM library from Speak Freely.  See README.1ST for details.
#
# Revision 1.1.1.1  2002/10/04 13:05:10  skip
# Initial import.
#

noinst_LIBRARIES = libgsm.a

libgsm_a_SOURCES = add.c code.c debug.c decode.c long_term.c lpc.c \
		   preprocess.c rpe.c gsm_destroy.c gsm_decode.c 	\
		   gsm_encode.c gsm_explode.c gsm_implode.c gsm_create.c \
		   gsm_print.c gsm_option.c short_term.c table.c

AM_CPPFLAGS = -I$(top_srcdir)/gsm/inc

AM_CFLAGS = -Wall -DSASR

