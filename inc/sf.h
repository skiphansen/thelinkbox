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

   $Log: sf.h,v $
   Revision 1.6  2010/11/17 04:44:18  wb6ymh
   1. Added G.726 support.
   2. Changed number of samples for uLaw/Rtp from 256 to 320 for AllStar
   compatibility.

   Revision 1.5  2008/07/23 15:26:33  wb6ymh
   Added ULAW_PACKET_DATA_BYTES, AUDIO_SAMPLES_PER_SF_ULAW_PKT.

   Revision 1.4  2008/02/26 18:18:09  wb6ymh
   Cosmetic cleanup.

   Revision 1.3  2007/12/07 16:53:11  wb6ymh
   Corrected error in comments - packet has 972 samples, not 1372.

   Revision 1.2  2007/06/12 23:43:00  wb6ymh
   Added AUDIO_SAMPLES_PER_ADPCM_PACKET and ADPCM_PACKET_DATA_BYTES.

   Revision 1.1  2002/11/03 15:51:57  wb6ymh
   Initial import.


*/

#ifndef SF_H_
#define SF_H_

#ifdef _MSC_VER
   #pragma pack(1)
   #define GCC_PACKED
#else
   #define GCC_PACKED __attribute__ ((packed))
#endif


#define SF_AUDIO_PORT    2074
#define SF_RTCP_PORT     2075
#define SF_RTP_VERSION   1

#define SF_AUDIO_SAMPLES_PER_PACKET    (10*AUDIO_SAMPLES_PER_GSM_FRAME)

/*
   Each Speak Freely ADCPM audio packet consists of a 24 byte Speak Freely
   header plus 3 bytes of ADCPM state plus 486 bytes of compressed audio
   representing 972 audio samples.
   
   So:
   
   (8000 samples/second) / (972 samples/packet) =
   8.23 packets/second.  or 1 packet every .1215 seconds.
   
   Additionally each packet consists of an IP header and an UDP header so
   
   (8.23 packets/second) * (20+8+24+3+486 bytes/packet) * 8 bits/byte =
   35620 bits/second.
   
*/

#define AUDIO_SAMPLES_PER_ADPCM_PACKET (972)
#define ADPCM_PACKET_DATA_BYTES        (AUDIO_SAMPLES_PER_ADPCM_PACKET/2)

#define AUDIO_SAMPLES_PER_SF_ULAW_PKT  (256)
#define AUDIO_SAMPLES_PER_RTP_ULAW_PKT (320)


/* 
   G.726 

   (8000 samples/second) / (320 samples/packet) =
   25 packets/second.  or 1 packet every .040 seconds.
   
   4 encoded bits / sample
*/

#define AUDIO_SAMPLES_PER_G726_PACKET  (320)
#define G726_PACKET_DATA_BYTES         (AUDIO_SAMPLES_PER_G726_PACKET/2)



typedef struct {
   int compression;
   char sendinghost[16];
   struct {
      int  buffer_len;
      char buffer_val[1];
   } buffer;
} GCC_PACKED soundbuf;

#define SIZEOF_SOUNDBUF (offsetof(soundbuf,buffer.buffer_val))

#define SF_GSM_FRAME_LEN   (SIZEOF_SOUNDBUF+(GSM_FRAME_LEN*10)+sizeof(u_int16))

/* compression flags. */
#define fComp2X     0x00000001   /* Simple 2 to 1 compression */
#define fDebug      0x00000002   /* Debug mode */
#define fSetDest    0x00000004   /* Set sound output destination */
#define fDestSpkr   0x00000000   /* Destination: speaker */
#define fDestJack   0x00000008   /* Destination: audio output jack */
#define fLoopBack   0x00000010   /* Loopback packets for testing */
#define fCompGSM    0x00000020   /* GSM compression */
#define fEncDES     0x00000040   /* DES encrypted */
#define fEncOTP     0x00000080   /* One-time pad encrypted */
#define fEncIDEA    0x00000100   /* IDEA encrypted */
#define fCompADPCM  0x00000200   /* ADPCM compressed */
#define fEncPGP     0x00000400   /* PGP encrypted */
#define fKeyPGP     0x00000800   /* Buffer contains PGP-encrypted session key */
#define fCompLPC    0x00001000   /* LPC compressed */
#define fFaceData   0x00002000   /* Request/reply for face data */
#define fFaceOffer  0x00004000   /* Offer face image to remote host */
#define fCompVOX    0x00010000   /* VOX compressed */
#define fCompLPC10  0x00020000   /* LPC-10 compressed */
#define fCompRobust 0x00040000   /* Robust packet replication ? */
#define fEncBF      0x00080000   /* Blowfish encrypted */
#define fProtocol   0x40000000   /* Speak Freely protocol flag */

#endif
