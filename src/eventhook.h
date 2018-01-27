/* Copyright (C) 2003 GFRN systems

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

   $Log: eventhook.h,v $
   Revision 1.3  2008/05/14 20:58:55  wb6ymh
   Changed EventHook fmt argument to a const char * to fix compile problems
   with GCC 4.2.x.

   Revision 1.2  2007/11/26 14:58:15  wb6ymh
   Added extern "C" around prototypes when compiled for C++.

   Revision 1.1  2003/08/16 14:30:26  wb6ymh
   Initial import.


*/

#ifndef  _EVENTHOOK_H_
#define  _EVENTHOOK_H_

#ifdef __cplusplus
extern "C" {
#endif

void EventHook(const char *fmt,...);

#ifdef __cplusplus
}
#endif


extern int EventProcessRunning;


#endif

