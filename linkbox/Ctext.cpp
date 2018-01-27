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

   $Log: Ctext.cpp,v $
   Revision 1.3  2010/02/23 01:12:25  wb6ymh
   Drop (const char *) operator to avoid compile errors on recent version of
   gcc which provide two strstr() functions.  On takes a const char * and the
   other takes a char * so it gets frustrated when the Ctext class provides
   both!

   Revision 1.2  2008/12/17 14:36:22  wb6ymh
   Corrected bug which prevented initial buffer size from being increased:
   continuing in a do { } while(FALSE) don't do much.

   Revision 1.1  2008/05/14 21:56:27  wb6ymh
   Initial import - Stupid little string class that's still somewhat useful.

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "Ctext.h"

#define INITIAL_BUFFER_LEN 80

CText::CText(int BufLen,int CurrentCount,char *Buffer)
{
   bFixedBuffer = true;
   Buf = Buffer;
   BufSize = BufLen;
   InitialCount = Count = CurrentCount;
}

CText::CText()
{
   bFixedBuffer = false;
   Buf = NULL;
   BufSize = 0;
   InitialCount = Count = 0;
}

CText::~CText()
{
   if(!bFixedBuffer) {
   // Free locally allocated buffer
      free(Buf);
   }
}

void CText::Clear() 
{
   if(!bFixedBuffer) {
      Count = 0;
      BufSize = 0;
      if(Buf != NULL) {
         free(Buf);
         Buf = NULL;
      }
   }
   else {
      Count = InitialCount;
      Buf[0] = 0;
   }
}

char * CText::operator=(const char *cp)
{
   Clear();
   print("%s",cp);
   return Buf;
}

CText::operator char*()
{
   return Buf;
}

int CText::print(const char *fmt,...)
{
   va_list args;
   int Wrote = 0;
   
   va_start(args,fmt);
   
   if(Buf == NULL) {
      if(!bFixedBuffer) {
         if((Buf = (char *) malloc(INITIAL_BUFFER_LEN)) != NULL) {
            BufSize = INITIAL_BUFFER_LEN;
         }
      }
   }

   if(Count < BufSize) {
      for( ; ; ) {
         Wrote = vsnprintf(&Buf[Count],BufSize-Count-1,fmt,args);
         if(Wrote == -1 || Wrote >= BufSize-Count-1) {
         // Pre-C99 style snprintf returns -1 on buffer overflow.  
         // C99 style snprintf returns the number of characters that would
         // have been written on buffer overflow.
         // In either case the output was truncated
            if(!bFixedBuffer) {
            // Dynamic buffer, double the size of the allocation and try again
               BufSize *= 2;
               char *Temp = (char *) malloc(BufSize);
               if(Temp != NULL) {
                  strcpy(Temp,Buf);
                  free(Buf);
                  Buf = Temp;
                  continue;   // Try again
               }
            }
         }

         if(Wrote > 0 && Count + Wrote < BufSize) {
            Count += Wrote;
         }
         Buf[Count] = 0;
         break;
      }
   }
   va_end(args);

   return Wrote;
}

