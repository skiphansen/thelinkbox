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

   $Log: devinput.h,v $
   Revision 1.2  2007/11/26 07:03:31  wb6ymh
   1. Added extern "C" around prototypes when compiled for C++.
   2. Added AudioDongleSN and pClass arguments to InitDeviceInput prototype.

   Revision 1.1  2007/11/21 17:38:29  wb6ymh
   Inital import - support for COS info from Linux event system.

   
*/ 

#ifndef _DEVINPUT_H_
#define _DEVINPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

int GetDevInCOS(void);
int InitDeviceInput(char *AudioDongleSN,void *pClass);

#ifdef __cplusplus
}
#endif

#endif   // _DEVINPUT_H_


