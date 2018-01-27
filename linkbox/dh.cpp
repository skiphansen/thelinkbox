/* Copyright (C) 2008 GFRN systems

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

   $Log: dh.cpp,v $
   Revision 1.7  2010/11/17 04:50:45  wb6ymh
   Added Tx power support.

   Revision 1.6  2010/08/29 00:43:21  beta2k
   Added 430MHz-440MHz band

   Revision 1.5  2010/03/13 15:06:31  wb6ymh
   Corrected several typos in 1.2 Ghz frequency range checking... not enough
   zeros!

   Revision 1.4  2010/02/16 03:43:10  beta2k
   Merged in changes from HEAD, other little things to do with hamlib

   Revision 1.3  2009/02/01 18:25:22  wb6ymh
   Corrected an unnoticed long standing bug in the Doug Hall support caught by
   gcc 4.3.2's anal warnings ( '&' -> '&&').

   Revision 1.2  2008/06/26 15:34:52  wb6ymh
   Added Sync() support.

   Revision 1.1  2008/05/14 21:58:38  wb6ymh
   Initial import - Control of rig's frequency, CTCSS, and DCS functions.
   Currently via Doug Hall protocol only. Eventually other classes willn
   be added to support other protocols.

*/
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "common.h"
#include "radio_ctrl.h"

RadioCtrl::RadioCtrl(char *Name)
{
   PortName = strdup(Name);
}

RadioCtrl::~RadioCtrl()
{
   if(PortName != NULL) {
      free(PortName);
      PortName = NULL;
   }
}

int RadioCtrl::SetRxFrequency(double Frequency,CText *p)
{
   return 0;
}


int RadioCtrl::SetRxTone(char *Desc,CText *p)
{
   return 0;
}

const char *RadioCtrl::GetRxTone()
{
   return "Unknown";
}


int RadioCtrl::SetTxTone(char *Desc,CText *p)
{
   return 0;
}

const char *RadioCtrl::GetTxTone()
{
   return "Unknown";
}


int RadioCtrl::Init(char *StateFileName,CText *p)
{
   return 0;
}

int RadioCtrl::SetTxOffset(double TxOffset,CText *p)
{
   return 0;
}

void RadioCtrl::Sync()
{
}

int RadioCtrl::SetTxPower(char *Desc,CText *p)
{
   return 0;
}

const char *RadioCtrl::GetTxPower()
{
   return "Unknown";
}


static int PlIndexLookup[] = {
670,719,744,770,797,825,854,885,
915,948,974,1000,1035,1072,1109,1148,
1188,1230,1273,1318,1365,1413,1462,1514,
1567,1622,1679,1738,1799,1862,1928,2035,
2107,2181,2257,2336,2418,2503,2541, 693,
1598,1655,1713,1773,1835,1899,1966,1995,
2065,2291,
-1 /* end of table */
};

static const char *PlIndex2Desc[] = {
"67.0","71.9","74.4","77.0","79.7","82.5","85.4","88.5",
"91.5","94.8","97.4","100.0","103.5","107.2","110.9","114.8",
"118.8","123.0","127.3","131.8","136.5","141.3","146.2","151.4",
"156.7","162.2","167.9","173.8","179.9","186.2","192.8","203.5",
"210.7","218.1","225.7","233.6","241.8","250.3","254.1"," 69.3",
"159.8","165.5","171.3","177.3","183.5","189.9","196.6","199.5",
"206.5","229.1"
};

static const char *PowerDesc[] = {"low", "high", "medium", "not set"};

GenericRadioCtrl::~GenericRadioCtrl()
{
}

int GenericRadioCtrl::Init(char *StateFileName,CText *p)
{
   FILE *fp;
   int bSetDefaults = TRUE;
// Try to restore prior settings from state file .<portname>.dh

   FileName.print(".%s_dh.bin",StateFileName);

   if((fp = fopen(FileName.Buf,"r")) != NULL) {
      if(fread(Data,7,1,fp) != 1) {
         p->print("%s: error reading %s - %s\n",__FUNCTION__,FileName.Buf,
                  strerror(errno));
      }
      else {
         bSetDefaults = FALSE;
      }

      if(fread(&TxOffset,sizeof(TxOffset),1,fp) != 1) {
         p->print("%s: error reading %s - %s\n",__FUNCTION__,FileName.Buf,
                  strerror(errno));
      }
      fclose(fp);
   }

   if(bSetDefaults) {
      Data[0] = 0xff;   // UF off
      Data[1] = 0xF0;   // Tx, Rx on
      Data[2] = 0xe0;   // radio power on, simplex
      Data[3] = 0;      // 100Khz, 10 Khz
      Data[4] = 0;      // PL disabled
      Data[5] = 0;      // Rx level, Squelch level @ 50 %
      Data[6] = 0;      // Not memory save, squelch closed, scan off
      TxOffset = 0;
   }
   memcpy(OrigData,Data,sizeof(OrigData));

   return 0;
}

GenericRadioCtrl::GenericRadioCtrl(char *Name,SendGenericData SendRoutine,
                                   void *SendPriv)
   : RadioCtrl(Name)
{
   SendData = SendRoutine;
   pSendPriv = SendPriv;
   TxOffset = 0;
}

/*------------------------------------------------------------------------------
"Link Comm RLC3" / "Generic" / "Doug Hall" format:

 Byte lsb bit of byte 1 shifted in first
  Byte 1: 8 user functions, high = off, low = on
 (first byte clocked in)

  Byte 2: B7 - TX power, 1 = on
          B6 - RX power, 1 = on
          B5, B4 - Tx power:
                 B5 0, B4 0 = low
                 B5 1, B4 0 = medium
                 B5 0, B4 1 = high
                 B5 1, B4 1 = no power change
          B0 -> B3 Band select:
                 0 - UHF 430   (43x.xxx Mhz)
                 1 - 1250      (125x.xxx Mhz)
                 2 - 2 meters  (14x.xxx Mhz)
                 3 - 220       (22x.xxx Mhz)
                 4 - UHF 440   (44x.xxx Mhz)
                 5 - 1270      (127x.xxx Mhz)
                 6 - 1280      (128x.xxx Mhz)
                 7 - 1290      (129x.xxx Mhz)
                 8 - 1260      (126x.xxx Mhz)
                 9 - 1240      (124.xxx Mhz)
                 A - UHF 420   (42x.xxx Mhz)
                 B - 900       (90x.xxx Mhz)
                 C - 6 meters  (05x.xxx Mhz)
                 D - 10 meters (02x.xxx Mhz)
                 E - 130 Mhz   (13x.xxx Mhz)
               If the band select nibble is not in the above list then
               the frequency will be used as the transmitter offset for
               duplex operations.

  Byte 3: B7 - Radio power, 1 = on
          B6 - 5 Khz bit, 1 = +5 Khz
          B5 -> B4 Offset:
               B5 0, B4 0 = negative Tx offset
               B5 0, B4 1 = positive Tx offset
               B5 1, B4 0 = simplex
               B5 1, B4 1 = negative 20 Mhz Tx offset (1200 radios only)
          B0 -> B3 Mhz digit (note: 100 Mhz and 10 Mhz digits are implied)

  Byte 4: B4 -> B7 100 Khz digit
          B0 -> B3 10 Khz digit

  Byte 5: B7 - 1 = PL decode enable
          B6 - 1 = PL encode enable
          B0 -> B5 = PL tone (Communications Specialists TS64 number - 1)

  Byte 6: B4 -> B7 Rx level
          B0 -> B3 Squelch level

  Byte 7: B4 -> B7 Memory channel
          B3 - Memory channel 16
          B2 - Memory save
          B1 - Open squelch
          B0 - Scan on

Communications Specialists TS64 numbers:
1 = 67.0Hz 2 = 71.9Hz 3 = 74.4Hz 4 = 77.0Hz 5 = 79.7Hz
6 = 82.5Hz 7 = 85.4Hz 8 = 88.5Hz 9 = 91.5Hz 10 = 94.8Hz
11 = 97.4Hz 12 = 100.0Hz 13 = 103.5Hz 14 = 107.2Hz 15 = 110.9Hz
16 = 114.8Hz 17 = 118.8Hz 18 = 123.0Hz 19 = 127.3Hz 20 = 131.8Hz
21 = 136.5Hz 22 = 141.3Hz 23 = 146.2Hz 24 = 151.4Hz 25 = 156.7Hz
26 = 162.2Hz 27 = 167.9Hz 28 = 173.8Hz 29 = 179.9Hz 30 = 186.2Hz
31 = 192.8Hz 32 = 203.5Hz 33 = 210.7Hz 34 = 218.1Hz 35 = 225.7Hz
36 = 233.6Hz 37 = 241.8Hz 38 = 250.3Hz 39 = 254.1Hz 40 =  69.3Hz
41 = 159.8Hz 42 = 165.5Hz 43 = 171.3Hz 44 = 177.3Hz 45 = 183.5Hz
46 = 189.9Hz 47 = 196.6Hz 48 = 199.5Hz 49 = 206.5Hz 50 = 229.1Hz

Data is clocked in on falling edge of clock
------------------------------------------------------------------------------*/

void GenericRadioCtrl::Sync()
{
   int Bytes = 0;
   int i;

   for(i = 6; i >= 0; i--) {
      if(OrigData[i] != Data[i]) {
         break;
      }
   }

   if(i >= 0) {
      if(i >= 5) {
      // Send all 7 bytes
         Bytes = 7;
      }
      else if(i > 0) {
      // Send 5 bytes
         Bytes = 5;
      }
      else {
      // Send one byte
         Bytes = 1;
      }

   // Send it
      memcpy(OrigData,Data,sizeof(OrigData));
      SendData(Data,Bytes,pSendPriv);

      FILE *fp;

      if((fp = fopen(FileName.Buf,"w")) != NULL) {
         if(fwrite(Data,7,1,fp) != 1 ||
            fwrite(&TxOffset,sizeof(TxOffset),1,fp) != 1)
         {
            LOG_ERROR(("%s: error writing %s - %s\n",__FUNCTION__,FileName.Buf,
                       strerror(errno)));
         }
         fclose(fp);
      }
      else {
         LOG_ERROR(("%s: couldn't open %s - %s\n",__FUNCTION__,FileName.Buf,
                    strerror(errno)));
      }
   }
}

void GenericRadioCtrl::SetFrequency(double Frequency,CText *p)
{
   unsigned char Khz;
   unsigned char TenKhz;
   unsigned char OnehundredKhz;
   unsigned char Mhz;
   double RoundedFreq;
   unsigned int frequency = (unsigned int) ((Frequency * 1e6) + .5);

   Khz = (frequency % 10000) / 1000;
   frequency /= 10000;
   TenKhz = frequency % 10;
   frequency /= 10;
   OnehundredKhz = frequency % 10;
   frequency /= 10;
   Mhz = frequency % 10;

   Data[2] &= 0xf0;
   Data[2] |= Mhz;

   if(Khz > 5) {
      frequency = (int) ((Frequency * 1e6) + .5);
      frequency = ((frequency / 10000) * 10000) + 5000;
      RoundedFreq = (double) frequency / 1e6;
      p->print("%lf rounded down to %3.3lf\n",Frequency,RoundedFreq);
      Data[2] |= 0x40;  // + 5khz
   }
   else if(Khz == 5) {
      Data[2] |= 0x40;  // + 5khz
   }
   else if(Khz != 0) {
      Data[2] &= ~0x40;
      frequency = (int) ((Frequency * 1e6) + .5);
      frequency = (frequency / 10000) * 10000;
      RoundedFreq = (double) frequency / 1e6;
      p->print("%lf rounded down to %3.3lf\n",Frequency,RoundedFreq);
   }
   else {
      Data[2] &= ~0x40;
   }

   Data[3] = TenKhz | (OnehundredKhz << 4);
}

int GenericRadioCtrl::SetRxFrequency(double Frequency,CText *p)
{
   unsigned int OrigFreq;
   unsigned int frequency = (unsigned int) ((Frequency * 1e6) + .5);
   int Ret = 1;

   frequency = frequency - (frequency % 100);
   OrigFreq = frequency;

   Data[1] &= 0xf0;

   do {
      if (frequency > 430000000 && frequency < 440000000) {
         // This doesn't do anything, but it excludes
         // 430MHz-440MHz from the catch-all else below
         Data[1] |= 0;
      }
      else if(frequency > 1250000000 && frequency < 1259999999) {
         Data[1] |= 1;
      }
      else if(frequency > 144000000 && frequency < 148000000) {
         Data[1] |= 2;   // 2 meters
      }
      else if(frequency > 220000000 && frequency < 225000000) {
         Data[1] |= 3;
      }
      else if(frequency > 440000000 && frequency < 450000000) {
         Data[1] |= 4;
      }
      else if(frequency > 1270000000 && frequency < 1280000000) {
         Data[1] |= 5;
      }
      else if(frequency > 1280000000 && frequency < 1290000000) {
         Data[1] |= 6;
      }
      else if(frequency > 1290000000 && frequency < 1300000000) {
         Data[1] |= 7;
      }
      else if(frequency > 1260000000 && frequency < 1270000000) {
         Data[1] |= 8;
      }
      else if(frequency > 1240000000 && frequency < 1250000000) {
         Data[1] |= 9;
      }
      else if(frequency > 420000000 && frequency < 430000000) {
         Data[1] |= 0xa;
      }
      else if(frequency > 902000000 && frequency < 908000000) {
         Data[1] |= 0xb;
      }
      else if(frequency > 50000000 && frequency < 54000000) {
         Data[1] |= 0xc;
      }
      else if(frequency > 20000000 && frequency < 30000000) {
         Data[1] |= 0xd;
      }
      else if(frequency > 130000000 && frequency < 140000000) {
         Data[1] |= 0xe;
      }
      else {
         p->print("Error: Invalid frequency\n");
         break;
      }

      SetFrequency(Frequency,p);
      Ret = 0;
   } while(false);

   return Ret;
}

int GenericRadioCtrl::SetTone(char *Arg,CText *p,int bRx)
{
   int CtcssFreq = -99;
   float CtcssFloat;
   int i;
   int bToneFreqSet = FALSE;
   int Ret = 1;   // Assume the worse

   do {
      if(strchr(Arg,'.') != NULL) {
      // There's a decimal point, must be a CTCSS frequency
         if(sscanf(Arg,"%f",&CtcssFloat) == 1) {
            bToneFreqSet = TRUE;
            CtcssFreq = (int) ((CtcssFloat * 10.0) + .5);
         }
      }
      else {
      // decimal (freq x 10) CTCSS frequency or (eventually) DCS code
         if(sscanf(Arg,"%d",&CtcssFreq) == 1) {
            bToneFreqSet = TRUE;
         }
      }

      if(!bToneFreqSet) {
         p->print("Unable to parse %sTone \"%s\"\n",bRx ? "Rx" : "Tx",Arg);
         break;
      }

      if(CtcssFreq != 0) {
      // Find Ctcss tone number
         for(i = 0; PlIndexLookup[i] > 0; i++) {
            if(CtcssFreq == PlIndexLookup[i]) {
            // Found it
               break;
            }
         }
      }

      if(CtcssFreq != 0 && PlIndexLookup[i] == -1) {
         p->print("Error: CTCSS frequency %d.%d is not supported.\n",
                  CtcssFreq/10,CtcssFreq%10);
      }
      else {
         if(CtcssFreq == 0) {
            if(bRx) {
            // Disable PL Rx
               Data[4] &= ~0x80;
            }
            else {
            // Disable PL Tx
               Data[4] &= ~0x40;
            }
         }
         else {
         // Set PL
            Data[4] |= bRx ? 0x80 : 0x40;
            Data[4] &= ~0x3f;
            Data[4] |= (unsigned char ) i;
         }
      }
      Ret = 0;
   } while(FALSE);
   return Ret;
}

const char *GenericRadioCtrl::GetRxTone()
{
   const char *Ret;

   if(Data[4] & 0x80) {
      Ret = PlIndex2Desc[Data[4] & 0x3f];
   }
   else {
      Ret = "disabled";
   }
   return Ret;
}

const char *GenericRadioCtrl::GetTxTone()
{
   const char *Ret;

   if(Data[4] & 0x40) {
      Ret = PlIndex2Desc[Data[4] & 0x3f];
   }
   else {
      Ret = "carrier";
   }
   return Ret;
}

int GenericRadioCtrl::SetRxTone(char *Arg,CText *p)
{
   return SetTone(Arg,p,true);
}

int GenericRadioCtrl::SetTxTone(char *Arg,CText *p)
{
   return SetTone(Arg,p,false);
}

int GenericRadioCtrl::SetTxOffset(double Offset,CText *p)
{
   int TempOffset = (int) ((Offset * 1e6) + .5);
   unsigned int AbsOffset;
   unsigned char NewOffsetBits;
   int Ret = 0;   // Assume the best

   AbsOffset = TempOffset < 0 ? -TempOffset : TempOffset;

   do {
      if(TempOffset == -20000000) {
      // negative 20 Mhz offset
         NewOffsetBits = 0x30;
      }
      else if(AbsOffset >= 10000000) {
         Ret = 1;
         p->print("Transmitter offset of %d is invalid (too large)\n",
                  TempOffset);
         break;
      }
      else if(TempOffset < 0) {
      // negative offset
         NewOffsetBits = 0;
      }
      else if(TempOffset > 0) {
      // positive offset
         NewOffsetBits = 0x10;
      }
      else {
      // simplex
         NewOffsetBits = 0x20;
      }

      if((Data[2] & 0x30) == NewOffsetBits && TempOffset == TxOffset) {
      // Nothing to do
         break;
      }

      Data[2] &= ~0x30;
      Data[2] |= NewOffsetBits;
      if(TempOffset != 0 && TempOffset != TxOffset && TempOffset != -TxOffset) {
      // The magnitude of offset has changed, send new offset to radio
         unsigned char DataSave[DH_MAX_BYTES];
         memcpy(DataSave,Data,DH_MAX_BYTES);
         if(Debug & DLOG_RIG_CTRL) {
            LOG_ERROR(("%s: Tx offset magnitude changed from %d to %d\n",
                       __FUNCTION__,TxOffset,TempOffset));
         }

         Data[1] &= 0xf0;
         Data[1] |= 0xf;   // set Tx offset
         TxOffset = TempOffset;
         SetFrequency(fabs(Offset),p);
         SendData(Data,5,pSendPriv);
      // Restore the data we clobbered setting the Txoffset
         memcpy(Data,DataSave,DH_MAX_BYTES);
      }
   } while(FALSE);

   return Ret;
}

int GenericRadioCtrl::SetTxPower(char *Arg,CText *p)
{
   int Ret = 0;   // Assume the best

   if(STRCMP(Arg,"low") == 0) {
   // set B5, B4 of byte 2 to 0, 0
      Data[1] &= 0xcf;  
   }
   else if(STRCMP(Arg,"medium") == 0) {
   // set B5, B4 of byte 2 to 1, 0
      Data[1] &= 0xcf;  
      Data[1] |= 0x20;
   }
   else if(STRCMP(Arg,"high") == 0) {
   // set B5, B4 of byte 2 to 0, 1
      Data[1] &= 0xcf;  
      Data[1] |= 0x10;
   }
   else {
      p->print("Error: unknown power level \"%s\".\n",Arg);
      Ret = 1;
   }

   return Ret;
}

const char *GenericRadioCtrl::GetTxPower()
{
   return PowerDesc[(Data[1] & 0x30) >> 4];
}

