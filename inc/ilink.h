/* Copyright (C) 2002 GFRN systems

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.

   The latest version of this program may be found at
   http://CQiNet.sourceforge.net

   The following data is specific to the iLink "protocol".  This information
   was determined by "sniffing" the Ether(net).  A offical source this file
   is not !
   
   $Log: ilink.h,v $
   Revision 1.5  2007/06/12 23:40:56  wb6ymh
   Added GSM_PACKET_DATA_BYTES.

   Revision 1.4  2006/08/05 23:28:14  wb6ymh
   Increased MAX_SNAPSHOT_LEN to 16.  On *some* platforms the 10-digit
   snapshot ID was being truncated to 9 digits.  Apparently this was caused
   by snprintf run time library differences.  Thanks to Jonathan K1RFD for
   noticing the problem and finding the solution.

   Revision 1.3  2003/08/16 13:49:31  wb6ymh
   Added DiffData, MAX_STATION_LIST_SIZE, MAX_SNAPSHOT_LEN.

   Revision 1.2  2002/11/02 19:07:53  wb6ymh
   Added AUDIO_SAMPLES_PER_GSM_FRAME.

   Revision 1.1.1.1  2002/10/06 17:00:22  wb6ymh
   Move common header files to inc subdirectory from src.

   Revision 1.3  2002/09/14 17:04:44  wb6ymh
   Added VER_REQUEST.

   Revision 1.2  2002/08/31 23:54:21  wb6ymh
   Added MAX_STATIONS_DISPLAYED and ILINK_UNKNOWN_PACKET.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#ifndef ILINK_H_
#define ILINK_H_

/*
   Each iLink/EchoLink audio packet consists of a 12 byte (modified) RTP
   header plus 4 GSM 6.10 encoded audio frames.  Each GSM audio frame 
   consists of 33 bytes corresponding to 160 audio samples.
   
   So:
   
   (8000 samples/second) / (160 samples/gsm frame) / (4 gsm frames/packet) =
   12.5 packets/second.  or 1 packet every .08 seconds.
   
   Additionally each packet consists of an IP header and an UDP header so
   
   (12.5 packets/second) * (20+8+12+(33 * 4)) bytes/packet) * 8 bits/byte =
   17200 bits/second.
   
*/

#define AUDIO_SAMPLES_PER_GSM_FRAME    (160)
#define ILINK_AUDIO_SAMPLES_PER_PACKET (4*160)
#define GSM_FRAME_LEN                  (33)
#define GSM_PACKET_DATA_BYTES          (4*GSM_FRAME_LEN)
#define ILINK_RTP_FRAME_LEN            (12+GSM_PACKET_DATA_BYTES)

#define StartOfData        "@@@"
#define DiffData           "DDD"    /* EchoLink only */
#define DummyCallsign      "    "
#define DummyIPAdr         "127.0.0.1"
#define DummyNodeID        "     "
#define EndOfData          "+++"

#define ILINK_RTP_PORT     5198
#define ILINK_RTCP_PORT    5199
#define ILINK_RTP_VERSION  3

#define ILINK_DIRSERVER_PORT     5200

// Text data packets are set to the RTP port and begin with a 0x6f 

#define ILINK_DATA_PACKET     0x6f
#define NDATA              "NDATA"

#define VER_REQUEST        "//ver"

// first byte of unknown 4 byte packet sent to RTP port on connect, 
// what's it mean ?
#define ILINK_UNKNOWN_PACKET  0x6d  

#define MAX_CALL_LEN       10
#define MAX_QTH_LEN        27

#define MAX_STATIONS_DISPLAYED   17

// The following is for sanity check only, there really isn't a max size.
#define MAX_STATION_LIST_SIZE    1000000

#define MAX_SNAPSHOT_LEN         16

#endif
