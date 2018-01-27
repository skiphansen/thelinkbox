#!/bin/sh
# cronjob to fetch the weekly Amateur Radio Newsline report and
# format it for playback by thelinkbox.
#
# Installation: 
# 1. Check and possibly edit paths for wget, sox and thelinkbox's working 
#    directory.
# 2. Add entry to /etc/crontab to run this script.
# 3. Add commands to thelinkbox's DTMF command table to play the file.
#
# This script should be run by a crontab entry similar to:
# 55      11      *       *       5       root    /home/tlb/getnews.sh
# 
# A DTMF command to play the news similar to the following should be
# added to thelinkbox's command table:
#
# ;Play newline "news"
# *6397 = say news.wav
#
# 6397 was chosen as a memory aid (it spells "news" if you have 
# alphanumerics on your touch tone pad.)
#
# The following command can be added to allow playback to be cancelled:
# *73 = shutup
#
# Requirements: Sox with mp3 support, wget.
#
# $Id: getnews.sh,v 1.1 2008/01/28 15:49:08 wb6ymh Exp $
cd /home/tlb
while true ; do
  time=`date "+%m/%d %R:%S"`
  echo "$time: Getting news.mp3" >> newsline.log
  /usr/local/bin/wget ftp://ftp.arnewsline.org/quincy/News/news.mp3
  if [ $? -eq 0 ]; then
    break;
  fi
  time=`date "+%m/%d %R:%S"`
  echo "$time: wget failed ($?), waiting to try again" >> newsline.log
  sleep 1800
done

/usr/local/bin/sox news.mp3 -r 8000 temp.wav
if [ $? -ne 0 ]; then
   time=`date "+%m/%d %R:%S"`
   echo "$time: sox failed ($?)" >> newsline.log
   exit
fi
rm news.mp3
rm news.wav
mv temp.wav news.wav
time=`date "+%m/%d %R:%S"`
echo "$time: Newsline updated" >> newsline.log
