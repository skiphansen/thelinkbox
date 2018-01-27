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

   $Log: Ctext.h,v $
   Revision 1.2  2010/02/23 01:12:25  wb6ymh
   Drop (const char *) operator to avoid compile errors on recent version of
   gcc which provide two strstr() functions.  On takes a const char * and the
   other takes a char * so it gets frustrated when the Ctext class provides
   both!

   Revision 1.1  2008/05/14 21:56:27  wb6ymh
   Initial import - Stupid little string class that's still somewhat useful.

*/

#ifndef _BUFPRINT_H_
#define _BUFPRINT_H_
class CText
{
public:
   CText(int BufLen,int CurrentCount,char *Buffer);
   CText();
   ~CText();
   int print(const char *fmt,...);
   bool IsEmpty() { return Count == InitialCount; }
   void Clear();
   char * operator=(const char *cp);
   operator char*();
   char *Buf;

protected:
   bool bFixedBuffer;
   int BufSize;
   int Count;
   int InitialCount;
};

#endif
