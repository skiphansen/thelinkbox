#!/bin/sh
# $Log: build_files.sh,v $
# Revision 1.1  2009/06/05 16:41:26  wb6ymh
# Inital import - OpenWRT configuration files for the "Little Blue"
# platform.
#

CWD=`pwd`
openwrt_dir="$HOME/openwrt"
build_dir_root="$openwrt_dir/build_dir/target-i386_glibc-2.3.6/root-x86"

cd $openwrt_dir/files
files=`grep '^diff' $HOME/openwrt.modified/openwrt.fs.patch | cut -d' ' -f3 | cut -d/ -f2-`

(cd $build_dir_root; tar -cf- $files) | tar -xf-

patch -p1 < $CWD/openwrt.fs.patch


