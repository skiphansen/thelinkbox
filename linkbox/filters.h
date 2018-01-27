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

   $Log: filters.h,v $
   Revision 1.4  2009/09/13 19:20:12  wb6ymh
   1. Modified BIQUAD macro to limit output to -32768/32767. Modified to
      scale Wn and Yn before final sum to extend range w/o overflow.
   2. Added BIQUAD_F macro for floating point filters.

   Revision 1.3  2008/12/30 16:17:33  wb6ymh
   1. Added FM preemphasis and deemphasis filters, CTCSS high pass filter.
   2. Added Shift argument to Filter* functions.

   Revision 1.2  2008/07/23 15:44:08  wb6ymh
   1. Added BIQUAD macro.
   2. Added prototypes for Filter2, Filter4, Filter6 and Filter8.

   Revision 1.1  2008/04/24 23:54:46  wb6ymh
   Inital import - Basic fixed point BiQuad filters.

*/ 

#ifndef _FILTERS_H_
#define _FILTERS_H_

#ifdef __cplusplus
extern "C" {
#endif

int16 BiQuad(int16 Sample,int *Coefficients,int *W);
int16 FirstOrder(int16 Sample,int *Coefficients,int *W);
void CtcssLowPass(int16 *In,int16 *Out,int Samples,int *Temp);
void ApplyTxPreemphasis(int16 *In,int16 *Out,int Samples,int *Temp);
void ApplyRxDeemphasis(int16 *In,int16 *Out,int Samples,int *Temp);
#ifndef FLOAT_CTCSS_FILTER
void ApplyCtcssFilter(int16 *pIn,int16 *pOut,int Samples,int *Temp);
#else
void ApplyCtcssFilter(int16 *pIn,int16 *pOut,int Samples,double *Temp);
#endif

void Filter2(int16 *In,int16 *Out,int Samples,int *Temp,int *Coef,int Shift);
void Filter4(int16 *In,int16 *Out,int Samples,int *Temp,int *Coef,int Shift);
void Filter6(int16 *In,int16 *Out,int Samples,int *Temp,int *Coef,int Shift);
void Filter8(int16 *In,int16 *Out,int Samples,int *Temp,int *Coef,int Shift);
void Filter8Float(int16 *In,int16 *Out,int Samples,double *Temp,double *Coef);

#define BIQUAD(_sample_in,_Coefficients,_Scratch,_sample_out,_shift)  \
                                                               \
   Wn = ((((int *)_Coefficients)[0] * _sample_in) -             \
        (((int *)_Coefficients)[1] * ((int *)_Scratch)[0]) -   \
        (((int *)_Coefficients)[2] * ((int *)_Scratch)[1])) >> _shift;    \
                                                               \
   Yn = Wn +                                                   \
        (((((int *)_Coefficients)[3] * ((int *)_Scratch)[0]) +   \
        (((int *)_Coefficients)[4] * ((int *)_Scratch)[1])) >> _shift);    \
                                                               \
   ((int *)_Scratch)[1] = ((int *)_Scratch)[0];                \
   ((int *)_Scratch)[0] = Wn;                                  \
                                                               \
   if(Yn > 32767) {                                            \
      _sample_out = 32767;                                     \
   }                                                           \
   else if(Yn < -32768) {                                      \
      _sample_out = -32768;                                    \
   }                                                           \
   else {                                                      \
      _sample_out = Yn;                                        \
   }
   
#define FIRST_ORDER(_sample_in,_Coefficients,_Scratch,_sample_out)   \
                                                                     \
   Wn = (((int *)_Coefficients)[0] * _sample_in) -                   \
        (((int *)_Coefficients)[1] * ((int *)_Scratch)[0]);          \
                                                                     \
   Yn = Wn +                                                         \
        (((int *)_Coefficients)[2] * ((int *)_Scratch)[0]);          \
                                                                     \
   ((int *)_Scratch)[0] = (Wn >> 14);                                \
                                                                     \
   _sample_out = Yn >> 14;
   
#define BIQUAD_F(_sample_in,_Coefficients,_Scratch,_sample_out)      \
                                                                     \
   Wn = (((double *)_Coefficients)[0] * _sample_in) -                \
        (((double *)_Coefficients)[1] * ((double *)_Scratch)[0]) -   \
        (((double *)_Coefficients)[2] * ((double *)_Scratch)[1]);    \
                                                                     \
   Yn = Wn +                                                         \
        (((double *)_Coefficients)[3] * ((double *)_Scratch)[0]) +   \
        (((double *)_Coefficients)[4] * ((double *)_Scratch)[1]);    \
                                                                     \
   ((double *)_Scratch)[1] = ((double *)_Scratch)[0];                \
   ((double *)_Scratch)[0] = Wn;                                     \
                                                                     \
   _sample_out = Yn;


#ifdef __cplusplus
}
#endif

#endif   /* _FILTERS_H_ */


