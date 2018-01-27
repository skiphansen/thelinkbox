/* Copyright (C) 2007 GFRN systems

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

   $Log: ToneGen.h,v $
   Revision 1.8  2009/09/13 19:24:39  wb6ymh
   Added DefaultLevel argument to constructor.

   Revision 1.7  2009/04/04 16:23:38  wb6ymh
   Added bWelcome, MaxPlayWithoutPause and MinPlayBackPause.

   Revision 1.6  2009/02/14 15:48:09  wb6ymh
   Added Timer and bFilePlaybackPause.

   Revision 1.5  2009/01/03 22:44:55  wb6ymh
   Added AX25 support.

   Revision 1.4  2008/06/26 17:38:38  wb6ymh
   Added CompletionCmd.

   Revision 1.3  2008/04/11 14:13:51  wb6ymh
   Added support for "FILE" and "DTMF" tags.

   Revision 1.2  2008/03/18 20:06:03  wb6ymh
   Added support for sending CW from a tone spec.

   Revision 1.1  2007/12/01 06:40:59  wb6ymh
   Inital import - general purpose tone generator class.


*/

#ifndef _TONEGEN_H_
#define _TONEGEN_H_

class CToneGen
{
public:
   CToneGen(int DefaultLevel = 8000,int Ax25TxDelay = 0,int Ax25ToneLevel = 0);
   ~CToneGen();
   int ToneGen(int16 *ToneBuf,int Samples);
   int ToneGenSetup(char *pToneGenSpec = NULL,char **cp = NULL);
   char *CompletionCmd;

public:
   CToneGen *Link;
   int ID;
   int bIDSource;
   int bWelcome;
   int MaxPlayWithoutPause;
   int MinPlayBackPause;

private:
   int GenSamples(int16 *ToneBuf,int Samples);
   char *TestCwString(char *CwString,char **pErrPos);
   int GenCW(int16 *CwBuf,int Samples);
   int GenDtmfSamples(int16 *ToneBuf,int Samples);
   char *TestDtmfString(char *DtmfString,char **pErrPos);
   char *TestFile(char *Path,char **pErrPos);
   int GenFileSamples(int16 *ToneBuf,int Samples);
   char *TestAx25(char *DtmfString,char **pErrPos);
   int GenAx25Samples(int16 *ToneBuf,int Samples);

private:
   int bToneGenActive;
   char *pToneGenSpecStart;
   char *pToneGen;
   char *pToneGenLastRep;
   int Tone1Freq;
   int Tone1EndFreq;
   int Tone1Level;
   int Tone2Freq;
   int Tone2Level;
   int ToneDuration;    // in milliseconds
   int Ax25TxDelay;     // in milliseconds
   int Ax25ToneLevel;
   int Pause;
   int ToneRepCount;
   int wpm;
   double Tone1Angle;
   double DeltaAngle1;
   double DeltaAngle1Delta;
   double Tone2Angle;
   double DeltaAngle2;
   int ToneBufNum;
   int ToneGenSamples;
   int SamplesUsed;
   int16 LastToneSample;
   int ElementCount;
   int b8BitFile;
   int RampCount;
   char *Element;
   int DitSamples;
   int CwRampCount;     // number of samples ramping up and down from full level
   char *ToneChar;
   int16 *Ax25Buf;
   FILE *fp;
   FILE *DebugFp;
   time_t Timer;

   int bGenDtmf:1;
   int bGenCW:1;
   int bToneOn:1;
   int bElementSpace:1;
   int bFromFile:1;
   int bAx25:1;
   int bToneLevelSet:1;
   int bFilePlaybackPause:1;
};

FILE *OpenWaveFile(char *Filename,int *b8BitFile);

#endif

