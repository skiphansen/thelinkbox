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
   
   This code was derived from:
    ---------------------------------------------------------------------------
    * xpmr.c - Xelatec Private Mobile Radio Processes
    *
    * All Rights Reserved. Copyright (C)2007, Xelatec, LLC
    *
    * 20070808 1235 Steven Henke, W9SH, sph@xelatec.com
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
    * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
    *
    * This version may be optionally licenced under the GNU LGPL licence.
    *
    * A license has been granted to Digium (via disclaimer) for the use of
    * this code.
    *
    * 20070914 0800 sph@xelatec.com adding radio control via frame message
    ---------------------------------------------------------------------------

   $Log: ctcss.c,v $
   Revision 1.7  2009/07/03 17:43:30  wb6ymh
   Added support for the "PCM" debug streams CtcssPeakDet and CtcssDecode.

   Revision 1.6  2009/06/07 20:45:32  wb6ymh
   Added code to buffer FRAME_SIZE (160 samples) before processing data.
   The algorythm wasn't designed to be called with a few samples at a time.
   Fixes intermittent decode since the audio buffering logic changes were
   made in version 0.39.

   Revision 1.5  2009/05/25 16:13:17  wb6ymh
   Assigned a debug level to enable debug logging.

   Revision 1.4  2008/04/25 14:06:22  wb6ymh
   1. Changed end of line convention back to Unix format.
   2. Fixec compile errors under FreeBSD (u_int8_t, etc).

   Revision 1.3  2008/04/24 23:58:12  wb6ymh
   Esentially new import - replace old unusable attempt with new code
   derived from Xelatec/app_rpt's xpmr.c.

   Revision 1.2  2008/03/09 17:00:12  wb6ymh
   Added dmalloc support.

   Revision 1.1  2007/06/30 00:04:25  wb6ymh
   Initial import - source files for thelinkbox.
   
*/ 


#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "common.h"
#include "ctcss.h"

#define DPRINT if(Debug & DLOG_RX_CTCSS) LogErr

#define CTCSS_NULL              -1
#define SAMPLE_RATE_NETWORK     8000
#define CTCSS_SCOUNT_MUL      100
#define M_Q15           0x00008000     // 32768
#define M_Q8            0x00000100     // 256
// FRAME_SIZE  the size of an audio frame, in samples.
#define FRAME_SIZE   160

FILE *DebugCtcssPeakDet = NULL;
FILE *DebugCtcssDecode = NULL;

static int16 coef_ctcss_div[]=
{
2985,    // 00   067.0
2782,    // 01   071.9
2688,    // 02   074.4
2597,    // 03   077.0
2509,    // 04   079.7
2424,    // 05   082.5
2342,    // 06   085.4
2260,    // 07   088.5
2186,    // 08   091.5
2110,    // 09   094.8
2053,    // 10   097.4
2000,    // 11   100.0
1932,    // 12   103.5
1866,    // 13   107.2
1803,    // 14   110.9
1742,    // 15   114.8
1684,    // 16   118.8
1626,    // 17   123.0
1571,    // 18   127.3
1517,    // 19   131.8
1465,    // 20   136.5
1415,    // 21   141.3
1368,    // 22   146.2
1321,    // 23   151.4
1276,    // 24   156.7
1233,    // 25   162.2
1191,    // 26   167.9
1151,    // 27   173.8
1112,    // 28   179.9
1074,    // 29   186.2
1037,    // 30   192.8
983,    // 31   203.5
949,    // 32   210.7
917,    // 33   218.1
886,    // 34   225.7
856,    // 35   233.6
827,    // 36   241.8
799     // 37   250.3
};

char *freq_ctcss[]=
{
   "67.0",     // 00   
   "71.9",     // 01   
   "74.4",     // 02   
   "77.0",     // 03   
   "79.7",     // 04   
   "82.5",     // 05   
   "85.4",     // 06   
   "88.5",     // 07   
   "91.5",     // 08   
   "94.8",     // 09   
   "97.4",     // 10   
   "100.0",    // 11   
   "103.5",    // 12   
   "107.2",    // 13   
   "110.9",    // 14   
   "114.8",    // 15   
   "118.8",    // 16   
   "123.0",    // 17   
   "127.3",    // 18   
   "131.8",    // 19   
   "136.5",    // 20   
   "141.3",    // 21   
   "146.2",    // 22   
   "151.4",    // 23   
   "156.7",    // 24   
   "162.2",    // 25   
   "167.9",    // 26   
   "173.8",    // 27   
   "179.9",    // 28   
   "186.2",    // 29   
   "192.8",    // 30   
   "203.5",    // 31  
   "210.7",    // 32  
   "218.1",    // 33  
   "225.7",    // 34  
   "233.6",    // 35  
   "241.8",    // 36  
   "250.3"     // 37  
};

int Freq2ToneNum[]=
{
   670,     // 00   
   719,     // 01   
   744,     // 02   
   770,     // 03   
   797,     // 04   
   825,     // 05   
   854,     // 06   
   885,     // 07   
   915,     // 08   
   948,     // 09   
   974,     // 10   
   1000,    // 11   
   1035,    // 12   
   1072,    // 13   
   1109,    // 14   
   1148,    // 15   
   1188,    // 16   
   1230,    // 17   
   1273,    // 18   
   1318,    // 19   
   1365,    // 20   
   1413,    // 21   
   1462,    // 22   
   1514,    // 23   
   1567,    // 24   
   1622,    // 25   
   1679,    // 26   
   1738,    // 27   
   1799,    // 28   
   1862,    // 29   
   1928,    // 30   
   2035,    // 31  
   2107,    // 32  
   2181,    // 33  
   2257,    // 34  
   2336,    // 35  
   2418,    // 36  
   2503     // 37  
};

int CtccssSample = 0;
/*
   one structure for each ctcss tone to decode 
*/
typedef struct
{
   int16 counter;         // counter to next sample
   int16 counterFactor;   // full divisor used to increment counter
   int16 peak;            // peak amplitude now maw sph now
   int16 zIndex;          // z bucket index
   int16 z[4];             
   int16 dvu;
   int16 dvd;
   int16 zd;
   int16 decode;
   u_int8 enable;
} t_tdet;

typedef struct
{
   int BlankingTimer;
   int16 decode;                   // current ctcss decode index
   int16 hyst;
   int16 binFactor;
   int16 fudgeFactor;
   int16 relax;
   int BiQuad1Temp[2];
   int BiQuad2Temp[2];
   t_tdet tdet[CTCSS_NUM_CODES];
   int16 RxBuf[FRAME_SIZE];
   int RxBufWr;
} t_dec_ctcss;

void UpdateCTCSSInternal(t_dec_ctcss *rxCtcss,int Threshold);

void *CtcssInit()
{
   int i;
   t_dec_ctcss *p = (t_dec_ctcss *) malloc(sizeof(t_dec_ctcss));

   if(p != NULL) {
      memset(p,0,sizeof(t_dec_ctcss));
      p->relax = 1;  // reduce talkoff from radios w/o CTCSS Tx HPF
      p->decode = CTCSS_NULL;
      p->hyst =(M_Q15*0.0130);
      p->binFactor = (M_Q15*0.135);           // was 0.140
      p->fudgeFactor = 8;

      for(i = 0; i < CTCSS_NUM_CODES; i++)
      {
         t_tdet *ptdet;
         ptdet = &(p->tdet[i]);
         ptdet->counterFactor = coef_ctcss_div[i];
         ptdet->enable = 0;
      }
   }
   
   return p;
}

void CtcssDestroy(void *Priv)
{
   free(Priv);
}

// >= 0 tone index of tone to enable
// == -1 disable all tones
// == -2 enable all tones
int EnableCtcssTone(int ToneNum,void *Priv)
{
   int Ret = 0;
   int i;
   t_dec_ctcss *p = (t_dec_ctcss *) Priv;

   if(ToneNum == -1 || ToneNum == -2) {
   // Enable or Disable all tones
      t_tdet *ptdet = p->tdet;
      for(i = 0; i < CTCSS_NUM_CODES; i++) {
         ptdet->enable = ToneNum == -1 ? 0 : 1;
         ptdet++;
      }
   }
   else if(ToneNum >= 0  && ToneNum < CTCSS_NUM_CODES) {
      p->tdet[ToneNum].enable = 1;
   }
   else {
   // Say what ?
      Ret = 1;
   }
   
   return Ret;
}

/*
   Continuous Tone Coded Squelch (CTCSS) Detector
   return detected tone index or -1
*/
int UpdateCTCSS(int16 *pInput,int SamplesIn,void *Priv,int Threshold) 
{
   t_dec_ctcss *rxCtcss = (t_dec_ctcss *) Priv;
   int Samples = 0;

   while(Samples < SamplesIn) {
      int Samples2Copy = SamplesIn - Samples;
      if(Samples2Copy > FRAME_SIZE - rxCtcss->RxBufWr) {
         Samples2Copy = FRAME_SIZE - rxCtcss->RxBufWr;
      }
      memcpy(&rxCtcss->RxBuf[rxCtcss->RxBufWr],pInput,Samples2Copy);
      pInput += Samples2Copy;
      rxCtcss->RxBufWr += Samples2Copy;
      Samples += Samples2Copy;
      if(rxCtcss->RxBufWr == FRAME_SIZE) {
         rxCtcss->RxBufWr = 0;
         UpdateCTCSSInternal(rxCtcss,Threshold);
         CtccssSample += FRAME_SIZE;
      }
   }

   return rxCtcss->decode;
}

void UpdateCTCSSInternal(t_dec_ctcss *rxCtcss,int Threshold) 
{
   int16 points2do,hit,thit,relax;
   int16 tnum, tmp,indexNow,diffpeak;
   int16 difftrig;
   int16 points = 0;
   int16 indexWas = 0;
   int16 BestHit = CTCSS_NULL;
   int16 BestHitPeak = 0;
   t_tdet   *ptdet;
   int16 DebugBuf[FRAME_SIZE];

   relax  = rxCtcss->relax;

   difftrig = relax ? -0.1*M_Q15 : -0.05*M_Q15;

   thit = hit = -1;

   for(tnum = 0; tnum < CTCSS_NUM_CODES; tnum++) {
      int accum, peak;
      int16 fudgeFactor;
      int16 binFactor;

      ptdet = &(rxCtcss->tdet[tnum]);
      if(!ptdet->enable) {
      // Tone is not enabled, ignore it
         continue;
      }
      points = points2do = FRAME_SIZE;
      fudgeFactor = rxCtcss->fudgeFactor;
      binFactor = rxCtcss->binFactor;

      while(ptdet->counter < (points2do*CTCSS_SCOUNT_MUL)) {
         tmp = (ptdet->counter/CTCSS_SCOUNT_MUL)+1;
         ptdet->counter -= (tmp*CTCSS_SCOUNT_MUL);
         points2do -= tmp;
         indexNow = points-points2do;

         if(DebugCtcssPeakDet != NULL) {
            int i;
            peak = ptdet->peak;
            for(i = indexWas; i < indexNow; i++) {
               DebugBuf[i] = peak;
            }
         }
         if(DebugCtcssDecode != NULL) {
            int i;
            peak = ptdet->decode * 255;
            for(i = indexWas; i < indexNow; i++) {
               DebugBuf[i] = peak;
            }
         }
         ptdet->counter += ptdet->counterFactor;

         accum = rxCtcss->RxBuf[indexNow-1]; // duuuude's major bug fix!
#if 0
         printf("%d: counter: %d, index: %d, accum: %d, decode: %d, z: %d %d %d %d, peak: %d\n",
                CtccssSample,
                ptdet->counter,indexNow,accum,ptdet->decode,
                ptdet->z[0],ptdet->z[1],ptdet->z[2],ptdet->z[3],
                ptdet->peak);
#endif


         ptdet->z[ptdet->zIndex]+=
         (((accum - ptdet->z[ptdet->zIndex])*binFactor)/M_Q15);

         peak = abs(ptdet->z[0]-ptdet->z[2]) + abs(ptdet->z[1]-ptdet->z[3]);

         if(ptdet->peak < peak) {
            ptdet->peak += ((peak-ptdet->peak)*binFactor)/M_Q15;
         }
         else {
            ptdet->peak = peak;
         }

         {
            static const int16 a0 = 13723;
            static const int16 a1 = -13723;
            int temp0,temp1;
            int16 x0;

            //differentiate
            x0 = ptdet->zd;
            temp0 =  x0 * a1;
            ptdet->zd = ptdet->peak;
            temp1 = ptdet->peak * a0;
            diffpeak = (temp0 + temp1)/1024;
         }

         if(diffpeak < -0.03*M_Q15) {
            ptdet->dvd -= 4;
         }
         else if(ptdet->dvd<0) {
            ptdet->dvd++;
         }

         if(ptdet->dvd < -12 && diffpeak > -0.02*M_Q15) {
            ptdet->dvu += 2;
         }
         else if(ptdet->dvu) {
            ptdet->dvu--;
         }

         tmp = (int16) Threshold;
         if(rxCtcss->decode == tnum) {
            tmp = relax ? (tmp*55)/100 : (tmp*80)/100;
         }

         if(ptdet->peak > tmp) {
            if(ptdet->decode < fudgeFactor*32) {
               ptdet->decode++;
            }
         }
         else if(rxCtcss->decode == tnum) {
            if(ptdet->peak > rxCtcss->hyst) {
               ptdet->decode--;
            }
            else if(relax) {
               ptdet->decode--;
            }
            else {
               ptdet->decode -= 4;
            }
         }
         else {
            ptdet->decode = 0;
         }

         if(rxCtcss->decode == tnum && !relax && ptdet->dvu > 0.00075*M_Q15) {
            ptdet->decode = 0;
            ptdet->z[0] = ptdet->z[1] = ptdet->z[2] = ptdet->z[3] = ptdet->dvu = 0;
            DPRINT("ctcss_detect() turnoff detected by dvdt for tnum = %i.\n",tnum);
         }

         if(ptdet->decode < 0) {
            if(rxCtcss->decode == tnum) {
               rxCtcss->decode = CTCSS_NULL;
               DPRINT("ctcss decode  NULL\n");
            }
            ptdet->decode = 0;
         }

         if(ptdet->decode >= fudgeFactor) {
            thit = tnum;
            if(ptdet->peak > BestHitPeak) {
               BestHitPeak = ptdet->peak;
               BestHit = thit;
            }

            if(rxCtcss->decode != tnum) {
               ptdet->zd = ptdet->dvu = ptdet->dvd = 0;  
            }
         }
         indexWas = indexNow;
         ptdet->zIndex = (++ptdet->zIndex) % 4;
      }
      ptdet->counter -= points2do*CTCSS_SCOUNT_MUL;
      if(DebugCtcssPeakDet != NULL) {
         int i;
         peak = ptdet->peak;
         for(i = indexWas; i < FRAME_SIZE; i++) {
            DebugBuf[i] = peak;
         }
         fwrite(DebugBuf,sizeof(DebugBuf),1,DebugCtcssPeakDet);
      }
      
      if(DebugCtcssDecode != NULL) {
         int i;
         peak = ptdet->decode * 255;
         for(i = indexWas; i < FRAME_SIZE; i++) {
            DebugBuf[i] = peak;
         }
         fwrite(DebugBuf,sizeof(DebugBuf),1,DebugCtcssDecode);
      }
   }

   if(BestHit > CTCSS_NULL && 
      rxCtcss->decode > CTCSS_NULL && 
      rxCtcss->decode != BestHit) 
   {  // Best hit is not the currently decoded tone
      t_tdet *pBestDet = &(rxCtcss->tdet[BestHit]);

      ptdet = &(rxCtcss->tdet[rxCtcss->decode]);

      ptdet->decode -= 4;
      if(ptdet->decode < pBestDet->decode) {
         // We have a better choice now
         DPRINT("%s: Changed from %d (%s) to %d (%s)\n",__FUNCTION__,
                rxCtcss->decode,freq_ctcss[rxCtcss->decode],
                BestHit,freq_ctcss[BestHit]);
         rxCtcss->decode = BestHit;
      }
   }

   //TRACEX((" ctcss_detect() thit %i\n",thit));

   if(rxCtcss->BlankingTimer > 0) {
      rxCtcss->BlankingTimer -= points;
   }
   
   if(rxCtcss->BlankingTimer < 0) {
      rxCtcss->BlankingTimer = 0;
   }

   if(BestHit > CTCSS_NULL && 
      rxCtcss->decode <= CTCSS_NULL && 
      !rxCtcss->BlankingTimer) 
   {
      rxCtcss->decode = BestHit;
      DPRINT("ctcss decode %s (%i)\n",freq_ctcss[BestHit],BestHit);
   }
   else if(BestHit <= CTCSS_NULL && rxCtcss->decode > CTCSS_NULL) {
      rxCtcss->BlankingTimer = SAMPLE_RATE_NETWORK/5;
      rxCtcss->decode = CTCSS_NULL;
      DPRINT("ctcss decode inactive\n");
      for(tnum = 0; tnum < CTCSS_NUM_CODES; tnum++) {
         t_tdet  *ptdet = NULL;
         ptdet = &(rxCtcss->tdet[tnum]);
         ptdet->decode = 0;
         ptdet->z[0] = ptdet->z[1] = ptdet->z[2] = ptdet->z[3] = 0;
      }
   }
   {
      int peak = 0;
      t_tdet   *ptdet;
      int16 PeakTone = -1;
      for(tnum = 0; tnum < CTCSS_NUM_CODES; tnum++) {
         ptdet = &(rxCtcss->tdet[tnum]);
         if(ptdet->peak > peak) {
            peak = ptdet->peak;
            PeakTone = tnum;
         }
      }
#if 0
      printf("Peak tone %d, %d/%d, decode %d, %d/%d\n",
             PeakTone,peak,rxCtcss->tdet[PeakTone].decode,
             rxCtcss->decode,
             rxCtcss->tdet[rxCtcss->decode].peak,
             rxCtcss->tdet[rxCtcss->decode].decode);
#endif
   }
}

