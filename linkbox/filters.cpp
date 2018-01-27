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

   $Log: filters.cpp,v $
   Revision 1.5  2009/09/13 19:29:41  wb6ymh
   1. Added floating point version of CTCSS high pass filter for testing.
   2. Added PlHighPass8 coefficients arrays with 10, 9 and 8 bit scalling
      for testing.  Experimentation / audio sweeping seems to indidate that
      11 bit scalling is optimum for overflow performance.

   Revision 1.4  2009/01/03 22:52:49  wb6ymh
   Cleaned up unused variables warnings.

   Revision 1.3  2008/12/30 16:16:33  wb6ymh
   1. Added FM preemphasis and deemphasis filters, CTCSS high pass filter.
   2. Added Shift argument to Filter* functions.

   Revision 1.2  2008/07/23 15:48:52  wb6ymh
   1. Added Filter2, Filter4, Filter6 and Filter8 which use the BIQUAD macro
      rather than calls to the BiQuad routine for improved performance.
   2. Modified CtcssLowPass to use Filter4.

   Revision 1.1  2008/04/24 23:54:46  wb6ymh
   Inital import - Basic fixed point BiQuad filters.

*/ 

#include "common.h"
#include "filters.h"
#include "math.h"

/*
"<S>IIR Filter Specifications -- CHEBYSHEV 1"
"   Type=","LOWPASS"
"   Order= ",4
"   Sampling frequency=",8000
"   Passband:",200,0
"   Stopband:",780.6665,0
"   Passband ripple:",1
"   Stopband loss:",60
"<C>Coefficients (A,D,E,B,C) for wordlength ",24
1,1.653416E-03,2,1,-1.933499,.9572726
2,5.256554E-03,2,1,-1.892893,.8994564
*/

int PlDecoderLPCoef[] = {
   27,      // A 1.653416E-03
   -31678,  // B -1.933499
   15684,   // C .9572726
   32768,   // D 2
   16384,   // E 1
// section 2
   86,      // A 5.256554E-03
   -31013,  // B -1.892893
   14737,   // C .8994564
   32768,   // D 2
   16384,   // E 1
};


/*
FM DeEmphsis filter.  

"<S>IIR Filter Specifications -- BUTTERWORTH"
"   Type=","LOWPASS"
"   Order= ",1
"   Sampling frequency=",8000
"   Passband:",500,0
"   Stopband:",3000,0
"   Passband ripple:",3
"   Stopband loss:",21.69117
"<C>Coefficients (A,D,E,B,C) for wordlength ",24
1,.1662395,1,1,1,-.6675209
*/

int DeEmphsis[] = {
   2724,    // A .1662395
   -10937,  // C -.6675209
   16384,   // E 1
};

/*
FM PreEmphasis filter.  

"<S>IIR Filter Specifications -- BUTTERWORTH"
"   Type=","HIGHPASS"
"   Order= ",1
"   Sampling frequency=",8000
"   Passband:",2300,0
"   Stopband:",100,0
"   Passband ripple:",3
"   Stopband loss:",30.16367
"<C>Coefficients (A,D,E,B,C) for wordlength ",24
1,.4414065,1,-1,1,.1171871

 Frequency(Hz)    Magnitude    Magnitude(dB)   Error from 6db/dec ref @ 1khz (dB)
   100.000        0.0310325     -30.1637       -0.021        
   200.000        0.0620710     -24.1422       -0.020099913  
   300.000        0.0931212     -20.6190       -0.018725094  
   400.000        0.124188      -18.1184       -0.016899827  
   500.000        0.155276      -16.1779       -0.014600087  
   600.000        0.186388      -14.5916       -0.011925008  
   700.000        0.217526      -13.2498       -0.0090608    
   800.000        0.248689      -12.0869       -0.00599974   
   900.000        0.279874      -11.0608       -0.002950189  
   1.00000E+03    0.311076      -10.1427       0             
   1.10000E+03    0.342287      -9.31219       0.002656297   
   1.20000E+03    0.373495      -8.55431       0.004765079   
   1.30000E+03    0.404683      -7.85770       0.006132954   
   1.40000E+03    0.435831      -7.21363       0.006509286   
   1.50000E+03    0.466914      -6.61527       0.005604819   
   1.60000E+03    0.497898      -6.05718       0.003120347   
   1.70000E+03    0.528748      -5.53502       -0.001298428  
   1.80000E+03    0.559418      -5.04527       -0.008020102  
   1.90000E+03    0.589855      -4.58509       -0.017462019  
   2.00000E+03    0.620000      -4.15216       -0.030059913  
   2.10000E+03    0.649784      -3.74462       -0.046305895  
   2.20000E+03    0.679128      -3.36097       -0.066723616  
   2.30000E+03    0.707946      -3.00000       -0.09185672   
   2.40000E+03    0.736140      -2.66079       -0.122314834  
   2.50000E+03    0.763606      -2.34261       -0.158710173  
   2.60000E+03    0.790227      -2.04496       -0.201726959  
   2.70000E+03    0.815881      -1.76746       -0.252035283  
   2.80000E+03    0.840436      -1.50991       -0.310370627  
   2.90000E+03    0.863755      -1.27219       -0.377449958  
   3.00000E+03    0.885696      -1.05430       -0.454025094  
   3.10000E+03    0.906115      -0.856331      -0.540864877  
   3.20000E+03    0.924868      -0.678405      -0.638704566  
   3.30000E+03    0.941813      -0.520704      -0.748282798  
   3.40000E+03    0.956816      -0.383436      -0.870314341  
   3.50000E+03    0.969749      -0.266816      -1.005476887  
   3.60000E+03    0.980499      -0.171057      -1.154407015  
   3.70000E+03    0.988968      -0.0963525     -1.317686981  
   3.80000E+03    0.995077      -0.0428661     -1.495838032  
   3.90000E+03    0.998766      -0.0107224     -1.689314541  
   4.00000E+03    1.00000        1.03544E-06   -1.898498791  
*/

int PreEmphasis[] = {
   7232,    // A .4414065
   -1920,   // C -.1171871
   -16384,  // E -1

// gain adjustment to give about the same peak deviation on noise as
// before PreEmphasis is applied.  Probably varies from receiver to receiver
// but this isn't critical, it's just an attempt to equalize the levels so
// the effect of preemphasis can be heard on the fly.  
   7045        // 1.72
};

/*
"<S>IIR Filter Specifications -- BUTTERWORTH"
"   Type=","HIGHPASS"
"   Order= ",8
"   Sampling frequency=",8000
"   Passband:",300,0
"   Stopband:",203.5,0
"   Passband ripple:",.7595257
"   Stopband loss:",20
"<C>Coefficients (A,D,E,B,C) for wordlength ",24
1,.5790657,-2,1,-1.87764,.9209083
2,.999999,-2,1,-1.749781,.7901033
3,.999999,-2,1,-1.663104,.7014284
4,.999999,-2,1,-1.619682,.6570061
 
 Frequency(Hz)    Magnitude    Magnitude(dB)   Phase(Deg)    Delay(Samples)
   0.00000        1.00000E-15   -300.000        0.00000    
   25.0000        5.12741E-09   -165.802        692.957        0.00000    
   50.0000        1.31363E-06   -117.631        665.745        24.1882    
   75.0000        33.7101E-06   -89.4448        638.184        24.4987    
   100.000        337.328E-06   -69.4390        610.066        24.9938    
   125.000        0.00201529    -53.9133        581.133        25.7184    
   150.000        0.00868961    -41.2200        551.034        26.7549    
   175.000        0.0299123     -30.4830        519.231        28.2690    
   200.000        0.0870975     -21.1999        484.785        30.6186    
   225.000        0.219801      -13.1594        445.993        34.4822    
   250.000        0.465515      -6.64133        400.839        40.1363    
   275.000        0.749917      -2.49974        351.792        43.5973    
   300.000        0.916263      -0.759593       308.327        38.6358    
   500.000        0.999973      -237.636E-06    163.645        8.59337    
   1.00000E+03    0.999999      -9.31895E-06    76.2772        1.96132    
   1.50000E+03    0.999999      -4.65948E-06    47.0522        0.907830   
   2.00000E+03    1.000000      -3.10632E-06    31.3867        0.554782   
   2.50000E+03    1.000000      -2.07088E-06    20.9564        0.399012   
   3.00000E+03    1.000000      -1.55316E-06    12.9864        0.322011   
*/

#ifdef FLOAT_CTCSS_FILTER
double PlHighPass8[] = {
   0.5790657,  // A 
   -1.87764,   // B 
   .9209083,   // C 
   -2.0,       // D 
   1.0,        // E 
   
   .999999,    // A 
   -1.749781,  // B 
   .7901033,   // C 
   -2.0,       // D 
   1.0,        // E 
   
   .999999,    // A 
   -1.663104,  // B 
   .7014284,   // C 
   -2.0,       // D 
   1.0,        // E 
   
   .999999,    // A 
   -1.619682,  // B 
   .6570061,   // C 
   -2.0,       // D 
   1.0,        // E 
};

#else

#define PL_HI_PASS_BITS 9

#if PL_HI_PASS_BITS == 11
int PlHighPass8[] = {
   1186,    // A .5790657
   -3845,   // B -1.87764
   1886,    // C .9209083
   -4096,   // D -2
   2048,    // E 1

   2048,    // A .999999
   -3584,   // B -1.749781,
   1618,    // C .7901033
   -4096,   // D -2
   2048,    // E 1

   2048,    // A .999999
   -3406,   // B -1.663104
   1437,    // C .7014284
   -4096,   // D -2
   2048,    // E 1

   2048,    // A .999999
   -3317,   // B -1.619682
   1346,    // C .6570061
   -4096,   // D -2
   2048,    // E 1
};
#endif

#if PL_HI_PASS_BITS == 10
int PlHighPass8[] = {
   593,     // A .5790657
   -1923,   // B -1.87764
   943,     // C .9209083
   -2048,   // D -2
   1024,    // E 1

   1024,    // A .999999
   -1792,   // B -1.749781,
   809,     // C .7901033
   -2048,   // D -2
   1024,    // E 1

   1024,    // A .999999
   -1703,   // B -1.663104
   718,     // C .7014284
   -2048,   // D -2
   1024,    // E 1

   1024,    // A .999999
   -1659,   // B -1.619682
   763,     // C .6570061
   -2048,   // D -2
   1024,    // E 1
};
#endif

#if PL_HI_PASS_BITS == 9
int PlHighPass8[] = {
   296,     // A .5790657
   -961,    // B -1.87764
   472,     // C .9209083
   -1024,   // D -2
   512,     // E 1

   512,     // A .999999
   -896,    // B -1.749781,
   405,     // C .7901033
   -1024,   // D -2
   512,     // E 1

   512,     // A .999999
   -852,    // B -1.663104
   359,     // C .7014284
   -1024,   // D -2
   512,     // E 1

   512,     // A .999999
   -829,    // B -1.619682
   336,     // C .6570061
   -1024,   // D -2
   512,     // E 1
};
#endif

#if PL_HI_PASS_BITS == 8
// Awful !
int PlHighPass8[] = {
   148,     // A .5790657
   -481,    // B -1.87764
   236,     // C .9209083
   -512,    // D -2
   256,     // E 1

   256,     // A .999999
   -448,    // B -1.749781,
   202,     // C .7901033
   -512,    // D -2
   256,     // E 1

   256,     // A .999999
   -426,    // B -1.663104
   180,     // C .7014284
   -512,    // D -2
   256,     // E 1

   256,     // A .999999
   -415,    // B -1.619682
   168,     // C .6570061
   -512,    // D -2
   256,     // E 1
};
#endif
#endif   //FLOAT_CTCSS_FILTER


/*
 BiQuad, design coefficients 
 Wn = A1 * Xn - B * Wn-1 - C * Wn-2
 Yn = A2 * Wn + (A2*D) * Wn-1 + (A2*E) * Wn-2
 
 where A = A1 * A2
 
 if a2 = 1  then
 
 Wn = A * Xn - B * Wn-1 - C * Wn-2
 Yn = Wn + D * Wn-1 + E * Wn-2

 update
 
 Wn-2 = Wn-1
 Wn-1 = Wn
*/

#define A   Coefficients[0]
#define B   Coefficients[1]
#define C   Coefficients[2]
#define D   Coefficients[3]
#define E   Coefficients[4]
int16 BiQuad(int16 Sample,int *Coefficients,int *W)
{
   int Wn;
   int Yn;

   Wn = (A * Sample) - (B * W[0]) - (C * W[1]);
   Yn = Wn + (D * W[0]) + (E * W[1]);

   W[1] = W[0];
   W[0] = (Wn >> 14);

   return (Yn >> 14);
}
#undef A
#undef B
#undef C
#undef D
#undef E

/*
   Wn = A1 * Xn - C * Wn-1
   Yn = A2 * Wn + (A2 * E) * Wn-1
   
   Wn-1 = Wn
   
   where A = A1 * A2
 
   if a2 = 1  then
   
   Wn = A * Xn - C * Wn-1
   Yn = Wn + E * Wn-1
   
*/ 
#define A   Coefficients[0]
#define C   Coefficients[1]
#define E   Coefficients[2]
int16 FirstOrder(int16 Sample,int *Coefficients,int *W)
{
   int Wn;
   int Yn;

   Wn = (A * Sample) - (C * W[0]);
   Yn = Wn + (E * W[0]);

   W[0] = (Wn >> 14);

   return (Yn >> 14);
}
#undef  A
#undef  C
#undef  E

void CtcssLowPass(int16 *In,int16 *Out,int Samples,int *Temp)
{
   Filter4(In,Out,Samples,Temp,PlDecoderLPCoef,14);
}

#if 1
void ApplyTxPreemphasis(int16 *pIn,int16 *pOut,int Samples,int *Temp)
{
   int Wn;
   int Yn;
#ifdef DO_PEAK
   double PreRMS = 0.0;
   double RMS = 0.0;
   int Peak = 0;
   int PrePeak = 0;
   static int Count = 0;
   static double PeakGain = 0.0;
#endif

   for(int i = 0; i < Samples; i++) {
#ifndef DO_PEAK
      Wn = (PreEmphasis[0] * *pIn++) - (PreEmphasis[1] * Temp[0]);
      Yn = Wn + (PreEmphasis[2] * Temp[0]);
      Temp[0] = Wn >> 14;
      *pOut++ = ((Yn >> 14) * PreEmphasis[3]) >> 12;
#else
// calculate RMS values

      RMS += pow(*pIn,2.0);
      if(*pIn > 0) {
         if(Peak < *pIn) {
            Peak = *pIn;
         }
      }
      else {
         if(Peak < -(*pIn)) {
            Peak = -(*pIn);
         }
      }

      Wn = (PreEmphasis[0] * *pIn++) - (PreEmphasis[1] * Temp[0]);
      Yn = Wn + (PreEmphasis[2] * Temp[0]);
      Temp[0] = Wn >> 14;
      Yn = Yn >> 14;
      PreRMS += pow(Yn,2.0);
      if(Yn > 0) {
         if(PrePeak < Yn) {
            PrePeak = Yn;
         }
      }
      else {
         if(PrePeak < -Yn) {
            PrePeak = -Yn;
         }
      }
      *pOut++ = (Yn * PreEmphasis[3]) >> 12;
#endif
   }

#ifdef DO_PEAK
   RMS = sqrt(RMS/Samples);
   PreRMS = sqrt(PreRMS/Samples);

   if(PrePeak != 0) {
      PeakGain += (double) Peak/PrePeak;
      Count++;
   }

   printf("RMS: %3.3lf, %3.3lf, %3.3lf\n",RMS,PreRMS,RMS/PreRMS);
   printf("%d, %d, %3.3lf (%3.3lf)\n",Peak,PrePeak,(double) Peak/PrePeak,
          (double)(PeakGain/Count));

#endif
}

#else
void ApplyTxPreemphasis(int16 *In,int16 *Out,int Samples,int *Temp)
{
   int Wn;
   int Yn;

   for(int i = 0; i < Samples; i++) {
      FIRST_ORDER(In[i],PreEmphasis,Temp,Out[i]);
   }
}
void ApplyTxPreemphasis(int16 *In,int16 *Out,int Samples,int *Temp)
{
   for(int i = 0; i < Samples; i++) {
      Out[i] = FirstOrder(In[i],PreEmphasis,Temp);
   }
}
#endif

#if 1

// #define DO_PEAK
/*   This code was derived from:
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
 
 One would think it would be simple to implement a single pole RC filter
 in software but that was not my experience!  I was unable to achieve 
 an accurate attenuation at 3Khz in relationship to 1Khz with a 8Khz sampling
 rate.  Steve's code gives a much better result than I was able to achieve
 with my limited DSP knowledge, but I'm not sure it actually meets the
 TIA-603 spec.  I may revisit this "someday", in the mean time I think this
 is "good enough".
*/
void ApplyRxDeemphasis(int16 *pIn,int16 *pOut,int Samples,int *Temp)
{
   int   i;
   int state00;
   int16 coeff00, coeff01;

#ifdef DO_PEAK
   double PreRMS = 0.0;
   double RMS = 0.0;
   int Peak = 0;
   int PrePeak = 0;
   static int Count = 0;
   static double PeakGain = 0.0;
#endif

   coeff00=6878;
   coeff01=25889;
   state00=*Temp;

   // note fixed gain of 2 to compensate for attenuation
   // in passband

   for(i=0; i < Samples; i++) {
// calculate RMS values

#ifdef DO_PEAK
      RMS += pow(pIn[i],2.0);
      if(*pIn > 0) {
         if(Peak < *pIn) {
            Peak = *pIn;
         }
      }
      else {
         if(Peak < -(*pIn)) {
            Peak = -(*pIn);
         }
      }
#endif
      state00 = pIn[i] + ((state00*coeff01)>>15);
      pOut[i] = (state00*coeff00)>>14;

#ifdef DO_PEAK
      PreRMS += pow(pOut[i],2.0);
      if(pOut[i] > 0) {
         if(PrePeak < pOut[i]) {
            PrePeak = pOut[i];
         }
      }
      else {
         if(PrePeak < -pOut[i]) {
            PrePeak = -pOut[i];
         }
      }
#endif
   }
   *Temp=state00;

#ifdef DO_PEAK
   RMS = sqrt(RMS/Samples);
   PreRMS = sqrt(PreRMS/Samples);

   if(PrePeak != 0) {
      PeakGain += (double) Peak/PrePeak;
      Count++;
   }

   printf("RMS: %3.3lf, %3.3lf, %3.3lf\n",RMS,PreRMS,RMS/PreRMS);
   printf("%d, %d, %3.3lf (%3.3lf)\n",Peak,PrePeak,(double) Peak/PrePeak,
          (double)(PeakGain/Count));
#endif
}
#else

/* my feeble attempt at deemphsis */
void ApplyRxDeemphasis(int16 *pIn,int16 *pOut,int Samples,int *Temp)
{
   int Wn;
   int Yn;

   for(int i = 0; i < Samples; i++) {
      Wn = (DeEmphsis[0] * *pIn++) - (DeEmphsis[1] * Temp[0]);
      Yn = Wn + (DeEmphsis[2] * Temp[0]);
      Temp[0] = Wn >> 14;
      *pOut++ = ((Yn >> 14) * DeEmphsis[3]) >> 14;
   }
}
void ApplyRxDeemphasis(int16 *In,int16 *Out,int Samples,int *Temp)
{
   int Wn;
   int Yn;
   
   for(int i = 0; i < Samples; i++) {
      FIRST_ORDER(In[i],DeEmphsis,Temp,Out[i]);
   }
}
#endif

#ifdef FLOAT_CTCSS_FILTER
void ApplyCtcssFilter(int16 *pIn,int16 *pOut,int Samples,double *Temp)
{
   Filter8Float(pIn,pOut,Samples,Temp,PlHighPass8);
}
#else
void ApplyCtcssFilter(int16 *pIn,int16 *pOut,int Samples,int *Temp)
{
   Filter8(pIn,pOut,Samples,Temp,PlHighPass8,PL_HI_PASS_BITS);
}
#endif

void Filter2(int16 *In,int16 *Out,int Samples,int *Temp,int *Coef,int Shift)
{
   int i;
   int Wn;
   int Yn;

   for(i = 0; i < Samples; i++) {
      BIQUAD(In[i],Coef,Temp,Out[i],Shift);
   }
}

void Filter4(int16 *In,int16 *Out,int Samples,int *Temp,int *Coef,int Shift)
{
   int i;
   int16 tmp;
   int Wn;
   int Yn;

   for(i = 0; i < Samples; i++) {
      BIQUAD(In[i],Coef,Temp,tmp,Shift);
      BIQUAD(tmp,&Coef[5],&Temp[2],Out[i],Shift);
   }
}

void Filter6(int16 *In,int16 *Out,int Samples,int *Temp,int *Coef,int Shift)
{
   int i;
   int16 tmp;
   int Wn;
   int Yn;

   for(i = 0; i < Samples; i++) {
      BIQUAD(In[i],Coef,Temp,tmp,Shift);
      BIQUAD(tmp,&Coef[5],&Temp[2],tmp,Shift);
      BIQUAD(tmp,&Coef[10],&Temp[4],Out[i],Shift);
   }
}

void Filter8(int16 *In,int16 *Out,int Samples,int *Temp,int *Coef,int Shift)
{
   int i;
   int16 tmp;
   int Wn;
   int Yn;

   for(i = 0; i < Samples; i++) {
      BIQUAD(In[i],Coef,Temp,tmp,Shift);
      BIQUAD(tmp,&Coef[5],&Temp[2],tmp,Shift);
      BIQUAD(tmp,&Coef[10],&Temp[4],tmp,Shift);
      BIQUAD(tmp,&Coef[15],&Temp[6],Out[i],Shift);
   }
}

#ifndef FLOAT_CTCSS_FILTER
void Filter8Float(int16 *In,int16 *Out,int Samples,double *Temp,double *Coef)
{
   int i;
   double tmp;
   double Wn;
   double Yn;
   int x;

   for(i = 0; i < Samples; i++) {
      BIQUAD_F(In[i],Coef,Temp,tmp);
      BIQUAD_F(tmp,&Coef[5],&Temp[2],tmp);
      BIQUAD_F(tmp,&Coef[10],&Temp[4],tmp);
      BIQUAD_F(tmp,&Coef[15],&Temp[6],tmp);
      x = (int) tmp;
      if(x > 32767) {
         Out[i] = 32767;
      }
      else if(x < -32768) {
         Out[i] = -32768;
      }
      else {
         Out[i] = (short int) x;
      }
   }
}
#endif
