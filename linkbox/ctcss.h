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

   $Log: ctcss.h,v $
   Revision 1.4  2009/07/03 17:43:56  wb6ymh
   Added DebugCtcssPeakDet, DebugCtcssDecode.

   Revision 1.3  2008/04/24 23:58:12  wb6ymh
   Esentially new import - replace old unusable attempt with new code
   derived from Xelatec/app_rpt's xpmr.c.

   Revision 1.2  2007/11/26 07:02:00  wb6ymh
   Added extern "C" around prototypes when compiled for C++.

   Revision 1.1  2007/06/30 00:04:25  wb6ymh
   Initial import - source files for thelinkbox.

   
*/ 

#ifdef __cplusplus
extern "C" {
#endif

void *CtcssInit();
void CtcssDestroy(void *Priv);
int EnableCtcssTone(int ToneNum,void *Priv);
int UpdateCTCSS(int16 *pInput,int Samples,void *Priv,int Threshold);
extern char *freq_ctcss[];
extern int Freq2ToneNum[];
extern FILE *DebugCtcssPeakDet;
extern FILE *DebugCtcssDecode;

#define CTCSS_NUM_CODES       38          // 0 - 37

#ifdef __cplusplus
}
#endif


