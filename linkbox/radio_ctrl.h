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

   $Log: radio_ctrl.h,v $
   Revision 1.4  2010/11/20 15:17:55  wb6ymh
   Added prototypes for SetTxPower, GetTxPower.

   Revision 1.3  2009/05/22 18:57:12  beta2k
   Added hamlib interface


   Revision 1.2  2008/06/26 15:34:52  wb6ymh
   Added Sync() support.

   Revision 1.1  2008/05/14 21:58:38  wb6ymh
   Initial import - Control of rig's frequency, CTCSS, and DCS functions.
   Currently via Doug Hall protocol only. Eventually other classes willn
   be added to support other protocols.

*/

#ifndef _RADIOCTRL_H_
#define _RADIOCTRL_H_

#ifndef _BUFPRINT_H_
#include "Ctext.h"
#endif

class RadioCtrl {
public:
// public functions
   RadioCtrl(char *Name);
   virtual ~RadioCtrl();
   virtual int Init(char *StateFileName,CText *p);
   virtual int SetRxFrequency(double Frequency,CText *p);
   virtual int SetRxTone(char *Arg,CText *p);
   virtual const char *GetRxTone();
   virtual int SetTxTone(char *Arg,CText *p);
   virtual const char *GetTxTone();
   virtual int SetTxOffset(double TxOffset,CText *p);
   virtual void Sync();
   virtual int SetTxPower(char *Arg,CText *p);
   virtual const char *GetTxPower();

protected:
   char *PortName;
};

typedef int (*SendGenericData)(unsigned char *Data,int Len,void *SendPriv);


/* Doug Hall / Generic interface */
class GenericRadioCtrl : public RadioCtrl {
public:
   GenericRadioCtrl(char *Name,SendGenericData SendRoutine,void *SendPriv);
   ~GenericRadioCtrl();
   int Init(char *StateFileName,CText *p);

   int SetRxFrequency(double Frequency,CText *p);
   int SetRxTone(char *Arg,CText *p);
   const char *GetRxTone();
   int SetTxTone(char *Arg,CText *p);
   const char *GetTxTone();
   int SetTxOffset(double TxOffset,CText *p);
   void Sync();
   int SetTxPower(char *Arg,CText *p);
   const char *GetTxPower();
   #define DH_PACKET_DELAY    1000  // 50 milliseconds minimum between updates

private:
   #define DH_MAX_BYTES 7
   unsigned char Data[DH_MAX_BYTES];
   unsigned char OrigData[DH_MAX_BYTES];
   SendGenericData SendData;
   void *pSendPriv;
   CText FileName;
   int TxOffset;

   int SetTone(char *Arg,CText *p,int bRx);
   void SetFrequency(double Frequency,CText *p);

};

typedef struct GenericDataTAG {
   struct GenericDataTAG *Link;
   int Len;
   unsigned char Data[DH_MAX_BYTES];
} GenericData;

/* hamlib interface */
class hamlibRadioCtrl : public RadioCtrl {
public:
   hamlibRadioCtrl(char *Name);
   ~hamlibRadioCtrl();

   int Init(char *StateFileName,CText *p);

   int SetRxFrequency(double Frequency,CText *p);
   int SetRxTone(char *Desc,CText *p);
   const char *GetRxTone();
   int SetTxTone(char *Desc,CText *p);
   const char *GetTxTone();
   int SetTxOffset(double TxOffset,CText *p);

private:
   int SetTone(char *Desc,CText *p);
   const char *GetTone();
};


#endif
