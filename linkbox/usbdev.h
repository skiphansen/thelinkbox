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

   $Log: usbdev.h,v $
   Revision 1.1  2007/11/26 15:21:45  wb6ymh
   Inital import.

   
*/ 

#ifndef _USBDEV_H_
#define _USBDEV_H_

#ifdef __cplusplus
extern "C" {
#endif

int UsbInit(void);
void UsbKeyRadio(int bKey);

#define USB_TYPE_UAC3556   1
#define USB_TYPE_CM108     2

#ifdef __cplusplus
}
#endif

#endif   /* _USBDEV_H_ */

