#!/bin/sh
# Start thelinkbox
# $Id: tlb.sh,v 1.1.1.1 2007/04/07 23:55:39 wb6ymh Exp $

pidfiledir=/var/run
tbd=/usr/local/libexec/tlb

# start
if [ "x$1" = "x" -o "x$1" = "xstart" ]; then
        if [ -f $tbd ]; then
                echo -n ' thelinkbox'
                $tlb -s
        fi

# stop
elif [ "x$1" = "xstop" ]; then
        kill `cat $pidfiledir/tlb.pid`
fi

