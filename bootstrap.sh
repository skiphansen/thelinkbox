#!/bin/sh
# $Log: bootstrap,v $
# Revision 1.1.1.1  2007/04/07 23:55:39  wb6ymh
# Inital import - top level for thelinkbox project.
#
#

set -x
if [ ! -d config ]; then
   echo 'creating ./config'
   mkdir config
fi
aclocal -I config \
&& autoheader \
&& automake --foreign --add-missing --copy \
&& autoconf \
&& ./configure
