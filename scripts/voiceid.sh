#!/bin/sh
# Example script to play a rotating set of prerecorded voice IDs 
# periodically to a port when idle.
#
# Installation:  
# Replace "<port>" in all of the following with the actual name of the port
# on your system where the IDs are to be played.
#
# 1. Create a subdirectory named "voiceid.<port>" under thelinkbox's 
#    working directory. 
# 2. Copy your voice ID wave files into "voiceid.<port>".  The wave files
#    can be either 8 bit or 16 bit PCM files with a 8Khz sampling rate.
# 3. Copy this script into thelinkbox's working directory.
# 4. Set the workingdir variable below to thelinkbox's working directory
# 5. Add an entry to /etc/crontab to run this script.
#
# Assuming that you've created a user account for thelinkbox called
# tlb this script could be run by a crontab entry similar to:
#
# 00      *      *       *       *       tlb    /home/tlb/voiceid.sh <port>
#
# If you're running tlb under another account use that name above instead 
# of "tlb".
#
# This crontab entry will attempt to generate a voice ID at the top of every 
# hour.  If the port is busy (transmitting) when the script executes it will 
# wait for the port to be idle before playing the ID.
#
# $Id: voiceid.sh,v 1.2 2008/08/03 00:04:04 wb6ymh Exp $

# Set the following to point to thelinkbox's working directory
workingdir="/home/tlb"

port=$1
cd $workingdir

counter=`cat idcounter.${port}`
counter=$( expr $counter + 1)

count=1
firstfile=""
for file in voiceid.${port}/* ; do
   if [ "${firstfile}x" = "x" ] ; then
      firstfile=$file
   fi
   if [ $count -eq $counter ] ; then
      idfile=$file
      break
   fi
   count=$( expr $count + 1)
done

if [ "${idfile}x" = "x" ] ; then
   idfile=$firstfile
   counter=1
fi
echo "$counter" > idcounter.${port}

/usr/local/bin/tlbcmd port | grep "$port" | grep "Tx"
if [ $? -eq 0 ] ; then
   time=`date "+%m/%d %R:%S"`
   echo "$time: port $port is busy" >> voiceid.log

   # wait for the channel to be idle
   while /usr/local/bin/tlbcmd port | grep "$port" | grep "Tx"; [ $? -eq 0 ] ; do
      sleep 60
   done
   time=`date "+%m/%d %R:%S"`
   echo "$time: port $port has gone idle, IDing" >> voiceid.log
fi

/usr/local/bin/tlbcmd "port $port; say $idfile"
