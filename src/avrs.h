/* Copyright (C) 2004 GFRN systems

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

   $Log: avrs.h,v $
   Revision 1.2  2008/12/30 00:54:10  wb6ymh
   Added struct AprsVals, prototypes for Send2Aprsd, CreateAprsPacket.

   Revision 1.1  2004/11/29 00:29:14  wb6ymh
   Initial import - report repeater/link location and parameters to the
   EchoLink servers.

*/

#ifndef _AVRS_H_
#define _AVRS_H_

typedef enum {
   AVRS_OFF = 1,
   AVRS_ON,
   AVRS_BUSY,
   AVRS_CONNECTED
} AVRS_State;

typedef struct {
   int Ax25TxDelay;
   char *Ax25BeaconText;
   char *Ax25BeaconPath;
   float TxPower;
   float Haat;
   float AntGain;
   float Frequency;
   float CTCSS;
   float AntDir;
   float RptOffset;
} AprsVals;

#ifdef __cplusplus
extern "C" {
#endif
int CreateAvrsPacket(AVRS_State State,char *Connected2,char *Buf,int BufLen);
void Send2Aprsd(AVRS_State State,char *Connected2);
char *CreateAprsPacket(int bForAprsd,AprsVals *pVals);
#ifdef __cplusplus
}
#endif

#endif

