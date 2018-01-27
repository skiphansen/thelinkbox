/* Copyright (C) 2004 GFRN systems

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

   Several routines in this file were stolen from:
 *
 * soundmodem.c  --  FlexNet driver for SoundModem.
 *
 * Copyright (C) 1999-2000
 *          Thomas Sailer (sailer@ife.ee.ethz.ch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *  History:
 *   0.1  23.06.1999  Created
 *   0.2  07.01.2000  Expanded to usbdevfs capabilities
 *

   $Log: ax25.c,v $
   Revision 1.2  2009/02/07 15:40:15  wb6ymh
   1. Fixed EOL convention.
   2. Added includes for time.h & sys/time.h needed under FreeBSD 4.x.

   Revision 1.1  2008/12/25 15:36:35  wb6ymh
   Inital import - soundmodem files for AX25 packet generation.

*/

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>

#include "common.h"
#include "configfile.h"
#include "configvars.h"
#include "main.h"
#include "modem.h"

void vLog(const char *fmt,va_list args);

static FILE *DebugFp;

 
/* ---------------------------------------------------------------------- */
/*
 * the CRC routines are stolen from WAMPES
 * by Dieter Deyke
 */

const u_int16_t crc_ccitt_table[0x100] = {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/* ---------------------------------------------------------------------- */

static inline u_int16_t calc_crc_ccitt(const u_int8_t *buffer, int len)
{
        u_int16_t crc = 0xffff;

        for (;len>0;len--)
                crc = (crc >> 8) ^ crc_ccitt_table[(crc ^ *buffer++) & 0xff];
        crc ^= 0xffff;
   return crc;
}

static inline void append_crc_ccitt(u_int8_t *buffer, int len)
{
        u_int16_t crc = calc_crc_ccitt(buffer, len);
        buffer[len] = crc;
        buffer[len+1] = crc >> 8;
}

static inline int check_crc_ccitt(const u_int8_t *buffer, int len)
{
        u_int16_t crc = calc_crc_ccitt(buffer, len);
        return (crc & 0xffff) == 0x0f47;
}

/* ---------------------------------------------------------------------- */

/*
 * high performance HDLC encoder
 * yes, it's ugly, but generates pretty good code
 */

#define ENCODEITERA(j)                         \
        if (!(notbitstream & (0x1f0 << j)))    \
                goto stuff##j;                 \
  encodeend##j: ;                              \

#define ENCODEITERB(j)                                          \
  stuff##j:                                                     \
        bitstream &= ~(0x100 << j);                             \
        bitbuf = (bitbuf & (((2 << j) << numbit) - 1)) |        \
                ((bitbuf & ~(((2 << j) << numbit) - 1)) << 1);  \
        numbit++;                                               \
        notbitstream = ~bitstream;                              \
        goto encodeend##j;                                      \

// return the number bytes in buffer for -1 on error
int hdlc_encode(u_int8 *buf,int buflen,u_int8 *pkt,int len)
{
   unsigned bitstream, notbitstream, bitbuf, numbit;
   int wr = 0;

   append_crc_ccitt(pkt, len);
   len += 2;
   bitstream = 0;
   bitbuf = 0x7e;
   numbit = 8; /* opening flag */
   while (numbit >= 8) {
      buf[wr++] = bitbuf;
      if (wr == buflen)
         return -1;  /* must not happen! */
      bitbuf >>= 8;
      numbit -= 8;
   }
   for (; len > 0; len--, pkt++) {
      bitstream >>= 8;
      bitstream |= ((unsigned int)*pkt) << 8;
      bitbuf |= ((unsigned int)*pkt) << numbit;
      notbitstream = ~bitstream;
      ENCODEITERA(0);
      ENCODEITERA(1);
      ENCODEITERA(2);
      ENCODEITERA(3);
      ENCODEITERA(4);
      ENCODEITERA(5);
      ENCODEITERA(6);
      ENCODEITERA(7);
      goto enditer;
      ENCODEITERB(0);
      ENCODEITERB(1);
      ENCODEITERB(2);
      ENCODEITERB(3);
      ENCODEITERB(4);
      ENCODEITERB(5);
      ENCODEITERB(6);
      ENCODEITERB(7);
   enditer:
      numbit += 8;
      while (numbit >= 8) {
         buf[wr++] = bitbuf;
         if (wr == buflen)
            return -1;  /* must not happen! */
         bitbuf >>= 8;
         numbit -= 8;
      }
   }
   bitbuf |= 0x7e7e << numbit;
   numbit += 16;
   while (numbit >= 8) {
      buf[wr++] = bitbuf;
      if (wr == buflen)
         return -1;  /* must not happen! */
      bitbuf >>= 8;
      numbit -= 8;
   }

   return wr;
}

#define OpeningFlags 8
#define ClosingFlags 8

// Return pointer to next character to parse or NULL on error
char *EncodeCallsign(char *Call,u_int8 *Buf)
{
   char *cp = Call;
   int CharCount = 0;
   int SSID = 0;

   for( ; ; ) {
      if(*cp == 0 || *cp == ' ' || *cp == ',' || *cp == '>') {
      // End of call
         break;
      }

      if(*cp == '-') {
      // SSSID coming
         if(sscanf(cp+1,"%d",&SSID) != 1 || SSID > 15) {
         // invalid SSID
            cp = NULL;
            break;
         }
         if(SSID > 9) {
         // Skip -##
            cp += 3;
         }
         else {
         // Skip -#
            cp += 2;
         }
      }
      else if(!isalpha(*cp) && !isdigit(*cp)) {
      // invalid character
         cp = NULL;
         break;
      }
      else if(SSID == 0) {
      // Valid character, store it in the buffer
         if(CharCount++ == 6) {
         // Callsign is too long
            cp = NULL;
            break;
         }
         if(Buf != NULL) {
            *Buf++ = (u_int8) (toupper(*cp) << 1);
         }
         cp++;
      }
      else {
      // anything following an SSID other than a delimiter is an error
         cp = NULL;
         break;
      }
   }

   if(cp != NULL && Buf != NULL) {
      while(CharCount++ < 6) {
      // pad callsign out to 6 characters
         *Buf++ = ' ' << 1;
      }
      // Add SSID
      *Buf++ = (u_int8) (('0' + SSID) << 1);
   }

   return cp;
}

struct modemchannel {
   u_int8 *HdlcBuf;  // input data
   int HdlcLen;      // input data len
   int HdlcRd;       // amount of input data processed

   int16 *AudioBuf;  // output buffer
   int AudioBufLen;  // output buffer len (in samples)
   int AudioBufWr;   // output buffer write pointer
};

// Encode text into AX25 ui frame
// Format: {callsign}>{callsign}[,{callsign}...] {text}"
int GetPacketAudio(char *PacketText,int16 **Buf,int *BufLen,int Ax25TxDelay)
{
   u_int8 *HdlcBuf = NULL;
   u_int8 *TempBuf = NULL;
   u_int8 *pTemp;
   int AllocLen;
   int Ret = -1;  // assume the worse
   char *cp;
   int bEndofHeader = FALSE;
   int Callsigns = 0;
   int BodyLen;
   int FromCalls = 0;
   int i;
   struct modemchannel Chan;
   unsigned int Baud1200 = 1200;
   void *ModState = NULL;

   const char *ModemParams[] = {
      NULL, // Bits per second
      NULL, // Frequency 0
      NULL, // Frequency 1
      NULL, // Differential Encoding
   };

   DebugFp = fopen("packet.raw","w");

   Chan.AudioBufWr = Chan.HdlcRd = 0;
   do {
   // Count the callsigns
      cp = PacketText;
      while(!bEndofHeader) {
         cp = EncodeCallsign(cp,NULL);
         if(cp == NULL) {
         // Bogus callsign
            break;
         }
         Callsigns++;
         
         switch(*cp++) {
            case '>':
               if(FromCalls++ != 0) {
               // Invalid format
                  bEndofHeader = TRUE;
                  cp = NULL;
               }
               break;

            case ' ':
            // end of header
               bEndofHeader = TRUE;
               break;
         }
      }

      if(Callsigns > 4 || Callsigns < 2) {
      // Too many or too few callsigns
         cp = NULL;
      }

      if(cp == NULL) {
         break;
      }

      BodyLen = strlen(cp);

      AllocLen = OpeningFlags;

   // Short callsigns are padded
      AllocLen += Callsigns * 7;

      AllocLen += 1; // control byte

      AllocLen += BodyLen; // data

   // plus closing flags and a CRC
      AllocLen += 2 + ClosingFlags;

      if((TempBuf = malloc(AllocLen)) == NULL) {
         break;
      }

   // worse case encoding to hdlc will add one bit for every 7 consecutive
      AllocLen = (AllocLen * 8) / 7;

      if((Chan.HdlcBuf = malloc(AllocLen)) == NULL) {
         break;
      }

      cp = strchr(PacketText,'>');
      pTemp = TempBuf;

   // Encode the "to" callsign
      cp = EncodeCallsign(cp+1,pTemp);
      pTemp += 7;
      cp++;
      
   // encoded the from callsign
      EncodeCallsign(PacketText,pTemp);
      pTemp += 7;

   // encode any digipeaters
      for(i = 2; i < Callsigns; i++) {
         cp = EncodeCallsign(cp,pTemp);
         pTemp += 7;
         cp++;
      }
   
      pTemp[-1] |= 1;   // end of addresses
      *pTemp++ = 0x3;   // control field = UI
      *pTemp++ = 0xf0;  // PID - no layer 3 protocol implemented
      memcpy(pTemp,cp,BodyLen);
      pTemp += BodyLen;
      Chan.HdlcLen = hdlc_encode(Chan.HdlcBuf,AllocLen,TempBuf,pTemp-TempBuf);
      if(Chan.HdlcLen < 0) {
         break;
      }
   // Whew!
      free(TempBuf);
      TempBuf = NULL;


      Chan.AudioBufLen = Ax25TxDelay * 8000 / 1000;   // initial flags
      Chan.AudioBufLen += (Chan.HdlcLen * 8 * 8000) / 1200;
   // both hdlc_encode() and modulate() include 2 closing flags
      Chan.AudioBufLen += (2 * 8 * 8000) / 1200;
   
   // include enough for an extra byte time because modulate rounds
   // up to the next bit boundary
      Chan.AudioBufLen += (8 * 8000) / 1200;

   // Allocate a buffer for the generated audio
      if((Chan.AudioBuf = malloc(Chan.AudioBufLen * sizeof(int16))) == NULL) {
         break;
      }

   // Now fill it with tones
      ModState = afskmodulator.config(&Chan,&Baud1200,ModemParams);
      if(ModState == NULL) {
         break;
      }
      afskmodulator.init(ModState,8000);
      afskmodulator.modulate(ModState,Ax25TxDelay);
      Ret = 0;
   } while(FALSE);

   if(HdlcBuf != NULL) {
      free(HdlcBuf);
   }

   if(TempBuf != NULL) {
      free(TempBuf);
   }

   if(ModState != NULL) {
      free(ModState);
   }

   *Buf = Chan.AudioBuf;
   *BufLen = Chan.AudioBufLen;

   if(DebugFp != NULL) {
      fclose(DebugFp);
   }

   return Ret;
}


int pktget(struct modemchannel *Chan, unsigned char *data, unsigned int len)
{
   int Ret = len;

   if(Chan->HdlcLen - Chan->HdlcRd < Ret) {
      Ret = Chan->HdlcLen - Chan->HdlcRd;
   }
   memcpy(data,&Chan->HdlcBuf[Chan->HdlcRd],Ret);
   Chan->HdlcRd += Ret;
   return Ret;
}

void audiowrite(struct modemchannel *Chan, const int16_t *samples, unsigned int nr)
{
   if(DebugFp != NULL) {
      fwrite(samples,nr*sizeof(int16_t),1,DebugFp);
   }
   if(Chan->AudioBufWr + nr <= Chan->AudioBufLen) {
      memcpy(&Chan->AudioBuf[Chan->AudioBufWr],samples,nr*sizeof(int16_t));
      Chan->AudioBufWr += nr;
   }
   else {
      LOG_ERROR(("%s: buffer overflow!\n",__FUNCTION__));
      CORE_DUMP();
   }
}

void logprintf(unsigned int level, const char *fmt,...)
{
   va_list args;
   va_start(args,fmt);

   if(level <= MLOG_WARNING) {
      vLog(fmt,args);
   }
   va_end(args);
}


