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

   $Log: ToneGen.cpp,v $
   Revision 1.15  2009/09/13 19:24:11  wb6ymh
   Added DefaultLevel argument to constructor.

   Revision 1.14  2009/07/19 14:11:39  wb6ymh
   Added initialization of ID to constructor.

   Revision 1.13  2009/04/04 16:23:38  wb6ymh
   Added bWelcome, MaxPlayWithoutPause and MinPlayBackPause.

   Revision 1.12  2009/02/14 15:47:45  wb6ymh
   1. Added some headers needed to compile under FreeBSD 4.x.
   2. Added support for the MaxPlayWithoutPause and MinPlayBackPause configuration
      variables to the FILE playback "tone" generator.
   3. Modified ToneGen to return the number of samples generated or -1 on
      completion.

   Revision 1.11  2009/01/03 22:44:55  wb6ymh
   Added AX25 support.

   Revision 1.10  2008/07/23 16:43:23  wb6ymh
   Added handling of space at end of DTMF string.  Fixes infinite loop when
   there was one.

   Revision 1.9  2008/06/26 17:38:12  wb6ymh
   1. Added initialization of CompletionCmd to constructor.
   2. Free CompletionCmd if allocated to destructor.

   Revision 1.8  2008/04/26 14:06:29  wb6ymh
   Initialize ToneGenSamples to zero in constructor.  Fixes hang on dtmf
   generation caused by ToneGenSamples being uninitialized.

   Revision 1.7  2008/04/25 00:04:10  wb6ymh
   Warning cleanups.

   Revision 1.6  2008/04/11 14:13:51  wb6ymh
   Added support for "FILE" and "DTMF" tags.

   Revision 1.5  2008/03/18 20:06:03  wb6ymh
   Added support for sending CW from a tone spec.

   Revision 1.4  2008/03/09 17:04:14  wb6ymh
   Added dmalloc support.

   Revision 1.3  2008/01/13 17:07:58  wb6ymh
   1. Removed self nuking code from ToneGenSetup.  ToneGenSetup is called by
      the class as well as externally.  Nuking oneself while running isn't
      wise.

   Revision 1.2  2007/12/02 23:42:39  wb6ymh
   Added initialization of Tone1Angle and Tone2Angle to constructor.  Fixes
   random failure of tone generation when TonexAngle's random uninitialzied
   garbage look like NAN.

   Revision 1.1  2007/12/01 06:40:59  wb6ymh
   Inital import - general purpose tone generator class.


*/
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#include "common.h"
#include "configvars.h"
#include "main.h"
#include "ToneGen.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define DPRINT if(Debug & DLOG_TONEGEN) LogErr

#ifndef  M_PI
#define  M_PI     3.14159265358979323846  /* pi */
#endif

/*
bytes      variable      description
0  - 3     'RIFF'/'RIFX' Little/Big-endian
4  - 7     wRiffLength   length of file minus the 8 byte riff header
8  - 11    'WAVE'
12 - 15    'fmt '
16 - 19    wFmtSize       length of format chunk minus 8 byte header 
20 - 21    wFormatTag     identifies PCM, ULAW etc
22 - 23    wChannels      
24 - 27    dwSamplesPerSecond  samples per second per channel
28 - 31    dwAvgBytesPerSec    non-trivial for compressed formats
32 - 33    wBlockAlign         basic block size
34 - 35    wBitsPerSample      non-trivial for compressed formats
*/

typedef struct {
   char  Riff[4];             // 'RIFF'/'RIFX' Little/Big-endian
   int   wRiffLength;         // length of file minus the 8 byte riff header
   char  Wave[4];             // 'WAVE'
   char  Fmt[4];              // 'fmt '
   int   wFmtSize;            // length of format chunk minus 8 byte header 
   short wFormatTag;          // identifies PCM, ULAW etc
   short wChannels;
   int   dwSamplesPerSecond;  // samples per second per channel
   int   dwAvgBytesPerSec;    // non-trivial for compressed formats
   short wBlockAlign;         // basic block size
   short wBitsPerSample;      // non-trivial for compressed formats
} WaveHdr;

char *CwLookUp(char CwChar);

CToneGen::CToneGen(int DefaultLevel,int Ax25Delay,int Ax25Level)
{
   bToneGenActive = TRUE;
   bGenCW = FALSE;
   bGenDtmf = FALSE;
   bFromFile = FALSE;
   bAx25 = FALSE;
   bIDSource = FALSE;
   pToneGenSpecStart = NULL;
   pToneGen = NULL;
   Tone1Level = DefaultLevel;
   Tone2Level = DefaultLevel;
   Tone1EndFreq = 0;
   ToneDuration = 0;
   Tone1Angle = Tone2Angle = 0.0;
   fp = NULL;
#if 1
   DebugFp = NULL;
#else
   DebugFp = fopen("tonegen.raw","w");
#endif

   wpm = 20;
   ToneGenSamples = 0;
   CompletionCmd = NULL;
   Ax25Buf = NULL;
   Ax25TxDelay = Ax25Delay;
   Ax25ToneLevel = Ax25Level;
   bFilePlaybackPause = FALSE;
   Timer = 0;
   MaxPlayWithoutPause = 0;
   MinPlayBackPause = 0;
   bWelcome = FALSE;
   ID = 0;
}

CToneGen::~CToneGen()
{
   if(fp != NULL) {
      fclose(fp);
   }
   if(DebugFp != NULL) {
      fclose(DebugFp);
   }

   if(pToneGenSpecStart != NULL) {
      free(pToneGenSpecStart);
   }

   if(CompletionCmd != NULL) {
      free(CompletionCmd);
   }

   if(Ax25Buf != NULL) {
      free(Ax25Buf);
   }
}


// Generate a tones based on compiled tone specification
// return the number of samples in ToneBuf
int CToneGen::GenSamples(int16 *ToneBuf,int Samples)
{
   int i = 0;

   if(bToneGenActive && ToneGenSamples == 0) {
      ToneGenSetup();
   }

   D2PRINTF(("%s: bToneGenActive: %d, Tone1Freq: %d, Tone2Freq: %d, ToneGenSamples: %d\n",
             __FUNCTION__,bToneGenActive,Tone1Freq,Tone2Freq,ToneGenSamples));

   if(bToneGenActive && (Tone1Freq != 0 || Tone2Freq != 0)) {
       if(Tone1Freq != 0 && Tone2Freq != 0) {
       // Both tones running
          for(; i < Samples; i++) {
             if(ToneGenSamples == 0) {
                break;
             }
             ToneGenSamples--;
             ToneBuf[i] = (int16) (Tone1Level * sin(Tone1Angle) + 
                                   Tone2Level * sin(Tone2Angle));
             Tone1Angle += DeltaAngle1;
             DeltaAngle1 += DeltaAngle1Delta;
             Tone2Angle += DeltaAngle2;
          }
       }
       else if(Tone1Freq != 0) {
       // Just tone 1 running
          for(; i < Samples; i++) {
             if(ToneGenSamples == 0) {
                break;
             }
             ToneGenSamples--;
             ToneBuf[i] = (int16) (Tone1Level * sin(Tone1Angle));
             Tone1Angle += DeltaAngle1;
             DeltaAngle1 += DeltaAngle1Delta;
          }
       }
       else {
       // Just tone 2 running
          for(; i < Samples; i++) {
             if(ToneGenSamples == 0) {
                break;
             }
             ToneGenSamples--;
             ToneBuf[i] = (int16) (Tone2Level * sin(Tone2Angle));
             Tone2Angle += DeltaAngle2;
          }
       }
   }
   else {
   // No tones
      int Samples2Copy;
      int j;

      if(bToneGenActive) {
         Samples2Copy = ToneGenSamples;
         if(Samples2Copy > Samples - i) {
            Samples2Copy = Samples - i;
         }
         ToneGenSamples -= Samples2Copy;
      }
      else {
         Samples2Copy = Samples - i;
      }

      if(LastToneSample != 0) {
      // Ramp the last tone sample out to prevent thumps
         for(j = 0; j < Samples2Copy; j++) {
            if(LastToneSample == 0) {
               Tone1Angle = Tone2Angle = 0.0;
               break;
            }
            LastToneSample /= 2;
            ToneBuf[i++] = LastToneSample;
         }
         Samples2Copy -= j;
      }

      if(Samples2Copy > 0) {
         memset(&ToneBuf[i],0,Samples2Copy * sizeof(int16));
         i += Samples2Copy;
      }
   }
   return i;
}

// Return number of samples written to ToneBuf or -1 when complete
int CToneGen::ToneGen(int16 *ToneBuf,int Samples)
{
   int i = 0;
   int NewSamples;

   if(bToneGenActive || LastToneSample != 0) {
   // We're not done
      while(i < Samples) {
         if(bGenCW) {
            i += GenCW(&ToneBuf[i],Samples-i);
            if(ToneChar == NULL) {
            // We're done
               bGenCW = FALSE;
            }
         }
         else if(bGenDtmf) {
            i += GenDtmfSamples(&ToneBuf[i],Samples-i);
         }
         else if(bFromFile) {
            if((NewSamples = GenFileSamples(&ToneBuf[i],Samples-i)) > 0) {
               i += NewSamples;
            }
            else if(NewSamples == -1) {
            // Temporary pause in output
               break;
            }
         }
         else if(bAx25) {
            i += GenAx25Samples(&ToneBuf[i],Samples-i);
         }
         else {
            i += GenSamples(&ToneBuf[i],Samples-i);
         }
      }

      if(DebugFp != NULL) {
         fwrite(ToneBuf,i,1,DebugFp);
      }
   // Save last tone sample for rampout
      LastToneSample = ToneBuf[Samples-1];
   }
   else {
   // We're done
      i = -1;
   }

   D2PRINTF(("%s: returning %d\n",__FUNCTION__,i));
   return i;
}

int CToneGen::ToneGenSetup(char *pToneGenSpec,char **pErrPos)
{
   char *cp;
   char Tag[6];
   int x;
   int ParseErrorLine = 0;
   int i;
   int ActionTags = 0;
   char *pParseStart = NULL;
   char *pError;

   if(pErrPos == NULL) {
      pErrPos = &pError;
   }
   *pErrPos = NULL;

// Set defaults
   if(pToneGenSpec != NULL) {
      DPRINT("Starting: %s\n",pToneGenSpec);
      ToneRepCount = -1;
      pToneGenLastRep = pToneGen = pToneGenSpecStart = strdup(pToneGenSpec);
   }

   cp = pToneGen;

   bToneGenActive = FALSE;
   bGenCW = FALSE;
   bGenDtmf = FALSE;
   bFromFile = FALSE;
   Tone1Freq = 0;
   Tone2Freq = 0;
   Tone1EndFreq = 0;
   ToneDuration = 0;
   Pause = 0;
   bToneLevelSet = FALSE;

   if(*cp == 0) {
   // End of the spec, we're done
      bToneGenActive = FALSE;
   }
   else {
      DPRINT("Parsing: %s\n",cp);
      while(*cp) {
         while(*cp == ' ') {
            cp++;
         }

         if(*cp == 0) {
         // that's it folks!
            break;
         }

         if(*cp == ':') {
         // End of this segment, go for it
            cp++;
            break;
         }

         pParseStart = cp;

      // copy the command tag
         for(i = 0; i < (int) (sizeof(Tag) - 1); i++) {
            if(!isprint(*cp) || *cp == ' ' || *cp == ':') {
               break;
            }
            Tag[i] = *cp++;
         }
         Tag[i] = 0;

         while(*cp == ' ') {
            cp++;
         }

         if(STRCMP(Tag,"CW") == 0) {
            bGenCW = TRUE;
            ToneChar = cp;
            if((cp = TestCwString(cp,pErrPos)) == NULL) {
               ParseErrorLine = __LINE__;
               break;
            }
            ActionTags++;
            continue;
         }

         if(STRCMP(Tag,"DTMF") == 0) {
            bGenDtmf = TRUE;
            ToneChar = cp;
            if((cp = TestDtmfString(cp,pErrPos)) == NULL) {
               ParseErrorLine = __LINE__;
               break;
            }
            ActionTags++;
            continue;
         }

         if(STRCMP(Tag,"FILE") == 0) {
            bFromFile = TRUE;
            ToneChar = cp;
            if((cp = TestFile(cp,pErrPos)) == NULL) {
               ParseErrorLine = __LINE__;
               break;
            }
            ActionTags++;
            continue;
         }

         if(STRCMP(Tag,"AX25") == 0) {
            bAx25 = TRUE;
            ToneChar = cp;
            if((cp = TestAx25(cp,pErrPos)) == NULL) {
               ParseErrorLine = __LINE__;
               break;
            }
            ActionTags++;
            continue;
         }


      // All of the remaining tags require an numeric argument

         if(sscanf(cp,"%d",&x) != 1) {
         // Parse error
            ParseErrorLine = __LINE__;
            *pErrPos = cp;
            break;
         }

         while(*cp != 0 && *cp != ' ' && *cp != ':') {
            cp++;
         }

         if(STRCMP(Tag,"TF1") == 0) {
            Tone1Freq = x;
         }
         else if(STRCMP(Tag,"EF1") == 0) {
            Tone1EndFreq = x;
         }
         else if(STRCMP(Tag,"TL") == 0) {
            Tone1Level = Tone2Level = x;
            bToneLevelSet = TRUE;
         }
         else if(STRCMP(Tag,"TF2") == 0) {
            Tone2Freq = x;
         }
         else if(STRCMP(Tag,"TL2") == 0) {
            Tone2Level = x;
         }
         else if(STRCMP(Tag,"WPM") == 0) {
            wpm = x;
         }
         else if(STRCMP(Tag,"DUR") == 0) {
            ToneDuration = x;
         }
         else if(STRCMP(Tag,"TXD") == 0) {
            Ax25TxDelay = x;
         }
         else if(STRCMP(Tag,"PAUSE") == 0) {
            Pause = x;
         }
         else if(STRCMP(Tag,"RPT") == 0) {
            if(ToneRepCount == -1) {
            // Rep count hasn't been set yet
               ToneRepCount = x;
               DPRINT("Init ToneRepCount: %d\n",ToneRepCount);
            }

            if(--ToneRepCount > 0) {
            // Repeat count not satisfied  yet, do it all again
               cp = pToneGenLastRep;
               DPRINT("Repeat ToneRepCount: %d\n",ToneRepCount);
            }
            else {
            // Finished with this group, setup for the next group
               DPRINT("Finish ToneRepCount: %d\n",ToneRepCount);
               ToneRepCount = -1;
            // Skip to end of tone segment
               while(*cp != 0 && *cp != ':') {
                  cp++;
               }

               if(*cp == ':') {
                  cp++;
               }
               pToneGenLastRep = cp;
            }
         }
         else if(STRCMP(Tag,"ID") == 0) {
            ID = x;
         }
         else {
            *pErrPos = Tag;
            ParseErrorLine = __LINE__;
            break;
         }
      }
      pToneGen = cp;

      if(ParseErrorLine != 0 || ActionTags > 1) {
         if(ParseErrorLine != 0) {
            LOG_ERROR(("%s#%d: unable to parse tonegen spec \"%s\"\n",
                       __FUNCTION__,ParseErrorLine,pParseStart));
         }
         else {
            LOG_ERROR(("%s: Error: more than one action tag\n",__FUNCTION__));
         }
         if(*pErrPos == NULL) {
            *pErrPos = pToneGenSpec + (pParseStart - pToneGenSpecStart);
         }
         bToneGenActive = FALSE;
         Tone1Freq = 0;
         Tone2Freq = 0;
         Tone1EndFreq = 0;
         ToneDuration = 0;
      }
      else if(bGenCW) {
         if(Tone1Freq == 0) {
            Tone1Freq = 1000;
         }
         bToneGenActive = TRUE;
         DeltaAngle1 = 2 * M_PI * Tone1Freq / 8000.0;

      // Calculate number of cycles of tone in a dit.
      //
      // The word PARIS was chosen as the standard length for CW code speed. 
      // Each dit counts for one count, each dah counts for three counts, 
      // intra-character spacing is one count, inter-character spacing is three 
      // counts and inter-word spacing is seven counts, so the word PARIS is 
      // exactly 50 counts.
      // So 1 wpm = 50 counts/minute or 50/60 counts/second.
      // or Samples/dit = (60/50 secs/dit) * (8000 samples / sec)
      
         DitSamples = (int) ((60.0 * 8000) / (50.0 * wpm));  
      
      // Calculate attach and decay time for tone envelope
      // 10% of dit time ramping up, 80% @ full tone level, 10% ramping down
      
         CwRampCount = DitSamples * 10 / 100;
         ElementCount = 0;
         RampCount = 0;
         bElementSpace = FALSE;
         bToneOn = FALSE;
      }
      else if(bGenDtmf || bFromFile) {
         bToneGenActive = TRUE;
      }
      else if(ToneDuration != 0) {
      // There's something to do 
         bToneGenActive = TRUE;

      // calculate number of samples for the specified duration
         ToneGenSamples = (ToneDuration * 8000) / 1000;

         DeltaAngle1 = 2 * M_PI * Tone1Freq / 8000.0;
         DeltaAngle2 = 2 * M_PI * Tone2Freq / 8000.0;
         if(Tone1EndFreq != 0) {
         // Setup tone sweep
            DeltaAngle1Delta = ((2 * M_PI * Tone1EndFreq / 8000.0) 
                               - DeltaAngle1) / ToneGenSamples;
         }
         else {
            DeltaAngle1Delta = 0.0;
         }
      }
   }
   return ParseErrorLine;
}

char *CToneGen::TestCwString(char *CwString,char **pErrPos)
{
   char *cp = CwString;
   while(*cp) {
      if(*cp == ':') {
      // end of string
         break;
      }
      if(CwLookUp(*cp++) == NULL)  {
      // Invalid character
         cp = NULL;
         break;
      }
   }

   return cp;
}

int CToneGen::GenCW(int16 *CwBuf,int Samples)
{
   int i = 0;
   char c;

   while(i < Samples) {
      if(ElementCount == 0) {
         if(bToneOn) {
         // Send intra-character space
            bToneOn = FALSE;
            ElementCount = DitSamples;
            bElementSpace = TRUE;
            RampCount = CwRampCount;
         }
         else if(bElementSpace) {
         // next element
            bElementSpace = FALSE;
            if((c = *Element++) != 0) {
               bToneOn = TRUE;
               RampCount = CwRampCount;
               if(c == '-') {
                  ElementCount = DitSamples * 3;
               }
               else if(c == '.') {
                  ElementCount = DitSamples;
               }
            }
            else {
            // Next character, already did a one dit time for the element
            // space, do 2 more for character space
               ElementCount = DitSamples * 2;
            }
         }
         else {
         // character space complete, do next character
            c = *ToneChar;

            if(c == 0) {
            // That's it
               ToneChar = NULL;
               if(i > 0) {
                  memset(&CwBuf[i],0,(Samples-i) * sizeof(int16));
                  i = Samples;
               }
               break;
            }
            else if(c == ' ') {
            // Word space, 3 dit time space already sent.  Send 4 more
               ElementCount = DitSamples * 4;
               ToneChar++;
            }
            else {
               for( ; ; ) {
                  if((c = *ToneChar) == 0 || c == ':') {
                  // End of string
                     ToneChar = NULL;
                     break;
                  }
                  ToneChar++;
                  Element = CwLookUp(c);
                  if(*Element == '-' || *Element == '.') {
                  // Start sending the next character
                     bElementSpace = TRUE;
                     break;
                  }
               }
            }
         }
      }

      if(ToneChar == NULL) {
         break;
      }

      if(RampCount > 0) {
      // Ramping tone level up or down
         if(bToneOn) {
         // Ramping up
            while(ElementCount-- > 0 && i < Samples && RampCount > 0) {
               CwBuf[i++] = (int16) ((Tone1Level * sin(Tone1Angle) *
                                     (CwRampCount - RampCount)) / CwRampCount);
               RampCount--;
               Tone1Angle += DeltaAngle1;
            }
         }
         else {
            // Ramping tone level down
            while(ElementCount-- > 0 && i < Samples && RampCount > 0) {
               CwBuf[i++] = (int16) ((Tone1Level * sin(Tone1Angle) *
                                     RampCount--) / CwRampCount);
               Tone1Angle += DeltaAngle1;
            }
         }
      }
      else if(bToneOn) {
      // Copy full level tone
         int LoopCount = min(ElementCount,Samples-i);
         ElementCount -= LoopCount;
         while(LoopCount-- > 0) {
            CwBuf[i++] = (int16) (Tone1Level * sin(Tone1Angle));
            Tone1Angle += DeltaAngle1;
         }
      }
      else {
      // No tone, just zero the buffer
         int CopyCount = min(ElementCount,Samples-i);
         memset(&CwBuf[i],0,CopyCount*sizeof(int16));
         i += CopyCount;
         ElementCount -= CopyCount;
      }
   }
   return i;
}

static struct DtmfCode {
   int Row;
   int Col;
} DtmfFreqLookup[] = {
   { 941, 1336},  // 0
   { 697, 1209},  // 1
   { 697, 1336},  // 2
   { 697, 1477},  // 3
   { 770, 1209},  // 4
   { 770, 1336},  // 5
   { 770, 1477},  // 6
   { 852, 1209},  // 7
   { 852, 1336},  // 8
   { 852, 1477},  // 9
   { 941, 1209},  // *
   { 941, 1477},  // #
   { 697, 1633},  // A
   { 770, 1633},  // B
   { 852, 1633},  // C
   { 941, 1633}   // D
};

int CToneGen::GenDtmfSamples(int16 *ToneBuf,int Samples)
{
   int Ret = 0;

   if(ToneGenSamples == 0) {
      if(bToneOn) {
      // Generate pause between digits
         bToneOn = FALSE;
      // calculate number of samples for the specified duration
         ToneGenSamples = Pause == 0 ? DtmfEncodePause : Pause;
         ToneGenSamples *= 8000;
         ToneGenSamples /= 1000;
         Tone1Freq = 0;
         Tone2Freq = 0;
         DPRINT("Pausing for %d samples\n",ToneGenSamples);
      }
      else {
      // Generate next DTMF digit
         int TimeMultiplier = 1;
         char Digit;
         int Index;
         for( ;  ; ) {
            if(*ToneChar != 0) {
               DPRINT("processing DTMF digit '%c'\n",*ToneChar);
            }
            else {
               DPRINT("processing end of DTMF string\n");
            }
            switch((Digit = tolower(*ToneChar++))) {
               case ' ':
               // ignore spaces
                  continue;
                  break;

               case ':':
               // We're done
                  bGenDtmf = FALSE;
                  ToneGenSetup();
                  Index = -2;
                  break;

               case 0:
                  Index = -2;
                  bGenDtmf = FALSE;
                  break;

               case 'a':
               case 'b':
               case 'c':
               case 'd':
                  Index = 12 + Digit-'a';
                  break;

               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  Index = Digit-'0';
                  break;

               case '*':
                  Index = 10;
                  break;

               case '#':
                  Index = 11;
                  break;

               case '+':
                  TimeMultiplier++;
                  continue;
                  break;

               default:
                  Index = -1;
                  break;
            }

            if(Index < 0) {
               if(Index == -1) {
                  LOG_ERROR(("%s: invalid DTMF digit \"%c\"\n",__FUNCTION__,Digit));
               }
               break;
            }

            bToneGenActive = TRUE;
            bToneOn = TRUE;

         // calculate number of samples for the specified duration
            ToneGenSamples = ToneDuration == 0 ? DtmfEncodeDuration : ToneDuration;
            ToneGenSamples *= TimeMultiplier * 8000;
            ToneGenSamples /= 1000;
            Tone1Freq = DtmfFreqLookup[Index].Row;
            Tone2Freq = DtmfFreqLookup[Index].Col;
            DeltaAngle1 = 2 * M_PI * Tone1Freq / 8000.0;
            DeltaAngle2 = 2 * M_PI * Tone2Freq / 8000.0;
            DeltaAngle1Delta = 0.0;
            break;
         }
      }
   }

   if(ToneGenSamples != 0) {
      Ret = GenSamples(ToneBuf,Samples);
   }

   return Ret;
}


extern "C" {
int GetPacketAudio(char *PacketText,int16 **Buf,int *BufLen,int Ax25TxDelay);
}

char *CToneGen::TestAx25(char *String,char **pErrPos)
{
   char *cp = String;

   SamplesUsed = 0;
   while(*cp) {
      cp++;
   }

   if(GetPacketAudio(String,&Ax25Buf,&ToneGenSamples,Ax25TxDelay)) {
   // Opps
      cp = NULL;
   }
   else {
   // Apply the tone level
      if(bToneLevelSet) {
         Ax25ToneLevel = Tone1Level;
      }
      for(int i = 0; i < ToneGenSamples; i++) {
         Ax25Buf[i] = (int16) ((Ax25ToneLevel *  Ax25Buf[i]) >> 15);
      }
   }
   return cp;
}

char *CToneGen::TestDtmfString(char *DtmfString,char **pErrPos)
{
   char *cp = DtmfString;
   char Digit;

   while(*cp) {
      if(*cp == ':' || *cp == ' ') {
      // end of string
         break;
      }
      Digit = tolower(*cp++);
      if(!(Digit >= '0' && Digit <= '9') &&
         !(Digit >= 'a' && Digit <= 'd') &&
         Digit != '*' && Digit != '#' && Digit != '+')
      {  // Bogus digit
         *pErrPos = cp;
         cp = NULL;
         break;
      }
   }

   return cp;
}

char *CToneGen::TestFile(char *Path,char **pErrPos)
{
   char *cp;
   char *Ret;
   char CharSave = 0;

   if((cp = strchr(Path,':')) != NULL || (cp = strchr(Path,' ')) != NULL) {
      CharSave = *cp;
      *cp = 0;
      Ret = cp;
   }
   else {
      Ret = Path + strlen(Path);
   }

   if((fp = OpenWaveFile(Path,&b8BitFile)) == NULL) {
      *pErrPos = Path;
      Ret = NULL;
   }

   if(cp != NULL) {
      *cp = CharSave;
   }
   return Ret;
}


int CToneGen::GenFileSamples(int16 *ToneBuf,int Samples)
{
   int SamplesRead = 0;

   if(MaxPlayWithoutPause != 0) {
      if(Timer == 0) {
      // First output, start the timer
         Timer = TimeNow.tv_sec;
      }
      else if(bFilePlaybackPause) {
      // Pausing playback
         if(TimeNow.tv_sec - Timer > MinPlayBackPause) {
         // Resume playback
            bFilePlaybackPause = FALSE;
            Timer = TimeNow.tv_sec;
         }
      }
      else if(TimeNow.tv_sec - Timer > MaxPlayWithoutPause) {
      // ... and now for a brief message from our sponsor ...
         bFilePlaybackPause = TRUE;
         Timer = TimeNow.tv_sec;
      }
   }

   if(bFilePlaybackPause) {
      SamplesRead = -1;
   }
   else {
      if(b8BitFile) {
         char *TempBuf = (char *) malloc(Samples);
         if(TempBuf != NULL) {
            SamplesRead = fread(TempBuf,1,Samples,fp);
            if(SamplesRead > 0) {
               for(int i = 0; i < SamplesRead; i++) {
                  ToneBuf[i] = (TempBuf[i] - 0x80) << 8;
               }
            }
            free(TempBuf);
         }
         else {
            LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
         }
      }
      else {
         SamplesRead = fread(ToneBuf,sizeof(int16),Samples,fp);
      }

      if(SamplesRead < Samples) {
         if(!feof(fp)) {
            char *cp;
            char CharSave;
            if((cp = strchr(ToneChar,':')) != NULL) {
               CharSave = *cp;
               *cp = 0;
            }
            LOG_ERROR(("%s: error reading \"%s\" - %s \n",__FUNCTION__,
                       ToneChar,strerror(errno)));
            if(cp != NULL) {
               *cp = CharSave;
            }
         }
         fclose(fp);
         fp = NULL;
         bFromFile = FALSE;
      }
   }

   return SamplesRead;
}


FILE *OpenWaveFile(char *Filename,int *b8BitFile)
{
   WaveHdr Hdr;
   FILE *fp;
   int bErr = FALSE; // Assume the best

   do {
      if((fp = fopen(Filename,"r")) == NULL) {
         break;
      }

      if(fread(&Hdr,sizeof(Hdr),1,fp) != 1) {
         LOG_ERROR(("%s: fread of \"%s\" failed - %s\n",__FUNCTION__,Filename,
                    strerror(errno)));
         bErr = TRUE;
         break;
      }

   // Stupid test for now... fix me someday
      if(Hdr.wBitsPerSample == 8) {
         *b8BitFile = TRUE;
      }
      else if(Hdr.wBitsPerSample == 16) {
         *b8BitFile = FALSE;
      }
      else {
         LOG_ERROR(("%s: %s has unsupported sample width (%d)\n",
                    __FUNCTION__,Filename,Hdr.wBitsPerSample));
         bErr = TRUE;
      }

      if(Hdr.dwSamplesPerSecond != 8000) {
         LOG_ERROR(("%s: %s has unsupported sampling rate (%d)\n",
                    __FUNCTION__,Filename,Hdr.dwSamplesPerSecond));
         bErr = TRUE;
      }
   } while(FALSE);

   if(bErr && fp != NULL) {
      fclose(fp);
      fp = NULL;
   }

   return fp;
}

int CToneGen::GenAx25Samples(int16 *ToneBuf,int Samples)
{
   int Ret = Samples;

   if(Ret > ToneGenSamples - SamplesUsed) {
      Ret = ToneGenSamples - SamplesUsed;
   }

   if(Ret > 0) {
      memcpy(ToneBuf,&Ax25Buf[SamplesUsed],Ret*sizeof(int16));
      SamplesUsed += Ret;
   }
   else if(Ax25Buf != NULL) {
      bAx25 = FALSE;
      free(Ax25Buf);
      Ax25Buf = NULL;
   }

   return Ret;
}

