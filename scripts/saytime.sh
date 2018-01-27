#!/bin/sh
# Example DTMF command script to announce the current time.
# Currently writen for the Cesptral (http://cepstral.com/) speech synthesizer
# but it can be modified easily for other programs.
# 
# An entry in tlb.cmds should call this script using the .script command.
# For example:
#
# *8531212 = script ./saytime.sh

cd /home/tbd

echo "the time is " > time.txt
if [ `date +%M` == "00" ]; then
#top of the hour
date "+%l oclock %p" >> time.txt
else
date "+%l %M %p" >> time.txt
fi

/usr/local/bin/swift -f time.txt -o time.wav
/usr/local/bin/tbdcmd "say time.wav"

