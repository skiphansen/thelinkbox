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

   $Log: devices.h,v $
   Revision 1.2  2009/02/01 18:24:31  wb6ymh
   Added InputBits member to ParallelDevice class.

   Revision 1.1  2008/12/30 15:56:59  wb6ymh
   Inital import - abstract serial and parallel port access into Device
   class.  Mostly code yanked out of ports.cpp.

*/

#ifndef _DEVICE_H_
#define _DEVICE_H_

class Device
{
public:
   Device(const char *Name);
   virtual ~Device();
   virtual int Init() = 0;
   static Device *FindDevice(const char *DeviceName);
   virtual void ChangeBit(int Bit,int bSet) = 0;
   virtual int GetBits() = 0;
   virtual int Read(char *Buf,int Len);
   virtual int Write(const char *Buf,int Len);
   virtual int SendGenericData(u_int8 *Data,int Len,int DataBit,int ClockBit);
   char *DeviceName;

protected:
   static Device *Devices;
   Device *Link;
   int Fd;
   int Bits;
};

class SerialDevice: public Device
{
public:
   SerialDevice(char *Name);
   int Init();
   int SetBaudrate(int Baud);
   void ChangeBit(int Bit,int bSet);
   int GetBits();
   int Read(char *Buf,int Len);
   int Write(const char *Buf,int Len);

private:
   int Baudrate;
};


class ParallelDevice: public Device
{
public:
   ParallelDevice(const char *Name);
   ~ParallelDevice();
   int Init();
   void ChangeBit(int Bit,int bSet);
   int GetBits();
   int SendGenericData(u_int8 *Data,int Len,int DataBit,int ClockBit);

private:
   int ClaimIRLP();
   void ReleaseIRLP();
   void SpinSleep(int Microseconds);
   int PortLocked;
   int LockIRLPfd;
   int InputBits;
};

#endif   // _DEVICE_H_
