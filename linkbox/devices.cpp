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

   $Log: devices.cpp,v $
   Revision 1.10  2010/11/17 04:50:10  wb6ymh
   Cosmetic.

   Revision 1.9  2010/11/17 04:49:20  wb6ymh
   Init both DTR and RTS to low int SerialDevice:Init() to prevent PTT lockup
   on rig blaster interfaces.

   Revision 1.8  2010/03/13 15:08:35  wb6ymh
   Corrected bug which prevented the 'C' DTMF button from being recognized
   on IRLP interfaces: the ioctl to read the parallel port status does
   *SIGN* extention (0xff -> 0xffffffff) !

   Revision 1.7  2009/09/13 19:17:20  wb6ymh
   Modified ParallelDevice::ChangeBit to read the current bit settings
   from port before writting new bits when ShareIRLP_Port is set.
   Prevents clobbering bits set by external scripts when the parallel port
   is shared.

   Revision 1.6  2009/07/19 14:06:03  wb6ymh
   1. Added initialization of Bits to Device constructor, InputBits to
      ParallelDevice constructor. Reported by valgrind, probably benign.

   2. Added unused third argument to ioctl(PPCLAIM) call to keep valgrind happy.

   Revision 1.5  2009/04/04 16:16:20  wb6ymh
   Initalize Baudrate to zero in SerialDevice constructor.

   Revision 1.4  2009/03/28 15:48:44  wb6ymh
   Imported changes from va3jss:
   1. Replaced references to SerialDevName and IRLP_DEVICE with DevName.
   2. Added support for multiple IRLP lock files.

   Revision 1.3  2009/02/08 16:47:43  wb6ymh
   Added include for sys/time.h needed to compile on FreeBSD 4.x.

   Revision 1.2  2009/02/01 18:23:25  wb6ymh
   Corrected ParallelDevice::GetBits - input is via the *status* bits, not
   the data bits. Thanks to Erik oh2lak for reporting the problem and testing
   the fix.

   Revision 1.1  2008/12/30 15:56:59  wb6ymh
   Inital import - abstract serial and parallel port access into Device
   class.  Mostly code yanked out of ports.cpp.


*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#ifdef HAVE_SYS_TIMEB_H
   #include <sys/timeb.h>
#endif
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#include "common.h"
#include "devices.h"
#include "main.h"
#include "configvars.h"

// IRLP interface board equates
#define IRLP_LOCKFILE_BASE   "/tmp/irlp-lockfile-"

#ifdef __linux__
#include <linux/ppdev.h>
#include <linux/parport.h>
#define PP_RD_DATA   PPRDATA
#define PP_WR_DATA   PPWDATA
#define PP_RD_STATUS PPRSTATUS
#define PP_DATA_TYPE char
#define IRLP_DEVICE     "/dev/parport0"
#endif

#ifdef __FreeBSD__
#include <dev/ppbus/ppi.h>
#include <dev/ppbus/ppbconf.h>
#define PP_RD_DATA   PPIGDATA
#define PP_WR_DATA   PPISDATA
#define PP_RD_STATUS PPIGSTATUS
#define PP_DATA_TYPE int
#define IRLP_DEVICE     "/dev/ppi0"
#endif

#define SERIAL_OPEN_FLAGS O_FSYNC | O_APPEND | O_NONBLOCK | O_RDWR | \
        O_NOCTTY | O_NDELAY

struct BaudRateTbl_TAG {
   int   Baudrate;
   int   SpeedConstant;
} BaudRateTbl[] = {
   {110,B110},
   {300,B300},
   {600,B600},
   {1200,B1200},
   {2400,B2400},
   {4800,B4800},
   {9600,B9600},
   {19200,B19200},
   {0}
};


Device *Device::Devices;

Device::Device(const char *Name)
{
   if(FindDevice(Name) != NULL) {
   // Doing a new for a second device with the same name is a bozo NO NO!
      CORE_DUMP();
   }
   DeviceName = strdup(Name);
   Link = Devices;
   Devices = this;
   Fd = -1;
   Bits = 0;
}

Device::~Device()
{
   if(DeviceName != NULL) {
      free(DeviceName);
   }
   
   if(Fd != -1) {
      close(Fd);
   }
}

Device *Device::FindDevice(const char *DevName)
{
   Device *pDev = Devices;

   while(pDev != NULL) {
      if(strcmp(pDev->DeviceName,DevName) == 0) {
         break;
         break;
      }
      pDev = pDev->Link;
   }

   return pDev;
}

int Device::Write(const char *Buf,int Len)
{
   CORE_DUMP();
   return 0;
}

int Device::Read(char *Buf,int Len)
{
   CORE_DUMP();
   return 0;
}

int Device::SendGenericData(u_int8 *Data,int Len,int DataBit,int ClockBit)
{
   CORE_DUMP();
   return 0;
}

SerialDevice::SerialDevice(char *Name)
   :Device(Name)
{
   Baudrate = 0;
}

int SerialDevice::Write(const char *Buf,int Len)
{
   return write(Fd,Buf,Len);
}

int SerialDevice::Read(char *Buf,int Len)
{
   return read(Fd,Buf,Len);
}


int SerialDevice::GetBits()
{
   if(ioctl(Fd,TIOCMGET,&Bits) == -1) {
      LOG_ERROR(("%s: ioctl(TIOCMGET) on %s failed: %s",__FUNCTION__,
                 DeviceName,Err2String(errno)));
   }
   return Bits;
}


// This routine should only be called once per serial port by
// the port's initialization code.  If it's called more than once 
// that indiates two ports are trying to share the data stream which
// is now allowed.
int SerialDevice::SetBaudrate(int Baud)
{
   int Ret = 0;
   int i;

   do {
      if(Baudrate != 0) {
         LOG_ERROR(("%s: Error %s has already been claimed for data\n",
                    __FUNCTION__,DeviceName));
         Ret = ERR_SERIAL_SHARE_ERR;
         break;
      }

      Baudrate = Baud;
      struct termios options;

      if(tcgetattr(Fd,&options) < 0) {
         LOG_ERROR(("%s: Unable to read port configuration for %s: %s",
                    __FUNCTION__,DeviceName,Err2String(errno)));
         Ret = ERR_SERIAL_GET_ATTR;
         break;
      }

      options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);    /* raw input */
      options.c_iflag &= ~(IXON | IXOFF | IXANY);  /* disable software flow control */
      options.c_iflag |=  IGNBRK;            /* ignore break */
      options.c_iflag &= ~ISTRIP;            /* do not strip high bit */
      options.c_iflag &= ~(INLCR | ICRNL);   /* do not modify CR/NL   */
      options.c_cflag |=  (CLOCAL | CREAD);  /* enable the receiver and set local mode */
      options.c_cflag &= ~CRTSCTS;           /* disable hardware flow control */
      options.c_cflag &= ~CSIZE;             /* Mask the character size bits */
      options.c_cflag |= CS8;                /* 8 data bits */
      options.c_cflag &= ~PARENB;            /* no parity */
      options.c_cflag &= ~CSTOPB;            /* 1 stop bit */

      for(i = 0; BaudRateTbl[i].Baudrate != 0; i++) {
         if(BaudRateTbl[i].Baudrate == Baud) {
            if(cfsetspeed(&options,BaudRateTbl[i].Baudrate) == -1) {
               LOG_ERROR(("%s: cfsetspeed() failed: %s",__FUNCTION__,
                          Err2String(errno)));
               Ret = ERR_SERIAL_SET_BAUD;
            }
            break;
         }
      }

      if(BaudRateTbl[i].Baudrate == 0) {
         LOG_ERROR(("%s: Baudrate %d is not supported.\n",__FUNCTION__));
         Ret = ERR_SERIAL_SET_BAUD;
         break;
      }

      if(tcsetattr(Fd,TCSANOW,&options) < 0) {
         LOG_ERROR(("%s: tcsetattr() failed: %s",__FUNCTION__,
                    Err2String(errno)));
         Ret = ERR_SERIAL_SET_ATTR;
         break;
      }
   } while(FALSE);

   return Ret;
}

void SerialDevice::ChangeBit(int Bit,int bSet)
{
   if(bSet) {
      Bits |= Bit;
   }
   else {
      Bits &= ~Bit;
   }

   if(ioctl(Fd,TIOCMSET,&Bits) == -1) {
      LOG_ERROR(("%s: ioctl(TIOCMSET) on %s failed: %s",__FUNCTION__,DeviceName,
                 Err2String(errno)));
   }
}

int SerialDevice::Init()
{
   int Ret = 0;

   do {
      if((Fd = open(DeviceName,SERIAL_OPEN_FLAGS)) < 0) {
         LOG_ERROR(("%s: Couldn't open \"%s\": %s",__FUNCTION__,DeviceName,
                    Err2String(errno)));
         Ret = ERR_SERIAL_DEV_OPEN;
         break;
      }
   // Prime Bits in case someone does a bit set before reading the
   // port the first time
      GetBits();
   // The rig blaster nomic uses *BOTH* RTS and DTR to key the transmitter.
   // Since tlb only drives *ONE* handshaking line make sure that both
   // RTS and DTR are low by default so the "unused" output won't stick the
   // transmitter on.
      ChangeBit(TIOCM_DTR | TIOCM_RTS,FALSE);
   } while(FALSE);

   return Ret;
}

ParallelDevice::ParallelDevice(const char *Name)
   : Device(Name)
{
   PortLocked = FALSE;
   LockIRLPfd = -1;
   InputBits = -1;
}

ParallelDevice::~ParallelDevice()
{
   if(LockIRLPfd != -1) {
      close(LockIRLPfd);
   }
}

int ParallelDevice::Init()
{
   int Ret = 0;
   char DevLockFileName[PATH_MAX];
   char *DevShortNamePtr;

   do {
      if((Fd = open(DeviceName,O_RDWR)) < 0 ) { 
         LOG_ERROR(("%s: open failed for \"%s\": %s",__FUNCTION__,DeviceName,
                    Err2String(errno)));
         Ret = ERR_IRLP_DEV_OPEN;
         break;
      }

      if(ShareIRLP_Port) {

       DevShortNamePtr = strrchr(DeviceName, '/' ) + 1;

       strncpy(DevLockFileName, IRLP_LOCKFILE_BASE, PATH_MAX);
       strncat(DevLockFileName, DevShortNamePtr, PATH_MAX);

         if((LockIRLPfd = open(DevLockFileName,O_WRONLY|O_CREAT, 0664)) < 0 ) {
            LOG_ERROR(("%s: open failed for \"%s\": %s",__FUNCTION__,
                       DevLockFileName,Err2String(errno)));
            Ret = ERR_IRLP_LOCKFILE_OPEN;
            break;
         }
      }
      else {
#ifdef __linux__
         LOG_ERROR(("%s: Attempting to claim parallel port\n",__FUNCTION__));
         if(ioctl(Fd,PPCLAIM,0) != 0) {
             LOG_ERROR(("%s: ioctl(PPCLAIM) failed: %s",__FUNCTION__,
                        Err2String(errno)));
             Ret = ERR_IRLP_DEV_CLAIM;
             break;
         } 
#endif
      }

      if(InitPPData != -1) {
         Bits = (char) InitPPData;

         if(ClaimIRLP()) {
            LOG_ERROR(("%s: ClaimIRLP failed\n",__FUNCTION__));
            break;
         }
         if(ioctl(Fd,PP_WR_DATA,&Bits)) {
            LOG_ERROR(("%s: ioctl(PPWDATA) failed: %s",__FUNCTION__,
                       Err2String(errno)));
         }
         ReleaseIRLP();
      }
   } while(FALSE);

   return Ret;
}

int ParallelDevice::ClaimIRLP() 
{
   int Ret = 0;   // Assume the best
   if(flock(LockIRLPfd,LOCK_EX) != 0) {
      Ret = errno;
      LOG_ERROR(("%s: flock(LOCK_EX) failed: %s",__FUNCTION__,
                 Err2String(errno)));
   }
#ifdef __linux__
   else if(ioctl(Fd,PPCLAIM) != 0) {
      Ret = errno;
      LOG_ERROR(("%s: ioctl(PPCLAIM) failed: %s",__FUNCTION__,
                 Err2String(errno)));
      if(flock(LockIRLPfd,LOCK_UN) != 0) {
         LOG_ERROR(("%s: flock(LOCK_UN) failed: %s",__FUNCTION__,
                    Err2String(errno)));
      }
   }
#endif
   else {
      PortLocked = TRUE;
   }

   return Ret;
}

void ParallelDevice::ReleaseIRLP()
{
#ifdef __linux__
   if(ioctl(Fd,PPRELEASE) != 0) {
      LOG_ERROR(("%s: ioctl(PPRELEASE) failed: %s",__FUNCTION__,
                 Err2String(errno)));
   } else 
#endif
   if(flock(LockIRLPfd,LOCK_UN) != 0) {
      LOG_ERROR(("%s: flock(LOCK_UN) failed: %s",__FUNCTION__,
                 Err2String(errno)));
   }
   else {
      PortLocked = FALSE;
   }
}

void ParallelDevice::ChangeBit(int Bit,int bSet)
{
   do {
      if(ShareIRLP_Port) {
         if(ClaimIRLP()) {
            LOG_ERROR(("%s: ClaimIRLP failed\n",__FUNCTION__));
            break;
         }
         if(ioctl(Fd,PP_RD_DATA,&Bits)) {
            LOG_ERROR(("%s: ioctl(PP_RD_DATA) on %s failed: %s",__FUNCTION__,
                       DeviceName,Err2String(errno)));
         }
      }

      if(bSet) {
         Bits |= Bit;
      }
      else {
         Bits &= ~Bit;
      }
      
      if(ioctl(Fd,PP_WR_DATA,&Bits)) {
         LOG_ERROR(("%s: ioctl(PP_WR_DATA) on %s failed: %s",__FUNCTION__,
                    DeviceName,Err2String(errno)));
      }

      if(ShareIRLP_Port && PortLocked) {
         ReleaseIRLP();
      }
   } while(FALSE);
}


int ParallelDevice::GetBits()
{
   do {
      if(ShareIRLP_Port && ClaimIRLP()) {
         InputBits = -1;
         LOG_ERROR(("%s: ClaimIRLP failed\n",__FUNCTION__));
         break;
      }

      if(ioctl(Fd,PP_RD_STATUS,&InputBits) != 0) {
         InputBits = -1;
         LOG_ERROR(("%s: ioctl(PP_RD_STATUS) on %s failed: %s",__FUNCTION__,
                    DeviceName,Err2String(errno)));
      }
      else if(InputBits == -1) {
      // Kill Sign extension so a DTMF 'C' does look like an error!
         InputBits = 0xff;
      }
   
      if(ShareIRLP_Port && PortLocked) {
         ReleaseIRLP();
      }
   } while(FALSE);

   return InputBits;
}

// sigh can't use usleep 'cuz it can't really just sleep for a few microseconds
void ParallelDevice::SpinSleep(int Microseconds)
{
   struct timeval StartTime;
   int Delta;

   GetTimeNow();
   StartTime = TimeNow;
   for( ; ; ) {
      GetTimeNow();
      Delta = ((TimeNow.tv_sec - StartTime.tv_sec) * 1000000) + 
               TimeNow.tv_usec - StartTime.tv_usec;
      if(Delta >= Microseconds) {
         break;
      }
   }
}

int ParallelDevice::SendGenericData(u_int8 *Data,int Len,int DataBit,int ClockBit)
{
   unsigned char Mask;
   int j;
   int i;

   if(Debug & DLOG_RIG_CTRL) {
      LOG_ERROR(("%s: Sending %d bytes:\n",__FUNCTION__,Len));
      LogHex(Data,Len);
   }

   do {
      if(ClaimIRLP()) {
         LOG_ERROR(("%s: ClaimIRLP failed\n",__FUNCTION__));
         break;
      }

      for(i = 0; i < Len; i++) {
         Mask = 1;
         for(j = 0; j < 8; j++) {
            Bits |= ClockBit;
            if(Data[i] & Mask) {
            // Data bit is high
               Bits |= DataBit;
            }
            else {
            // Data bit is low
               Bits &= ~DataBit;
            }
         // Output clock high and new data bit
            if(ioctl(Fd,PP_WR_DATA,&Bits)) {
               LOG_ERROR(("%s: ioctl(PP_WR_DATA) failed: %s",__FUNCTION__,
                          Err2String(errno)));
               break;
            }
         // Delay at least 15 microseconds from data to clock low
            SpinSleep(20);

         // Drop clock to clock in new data
            Bits &= ~ClockBit;
            if(ioctl(Fd,PP_WR_DATA,&Bits)) {
               LOG_ERROR(("%s: ioctl(PP_WR_DATA) failed: %s",__FUNCTION__,
                          Err2String(errno)));
               break;
            }
         // keep clock low for at least 10 microseconds
            SpinSleep(20);

            Bits |= ClockBit;
            if(ioctl(Fd,PP_WR_DATA,&Bits)) {
               LOG_ERROR(("%s: ioctl(PP_WR_DATA) failed: %s",__FUNCTION__,
                          Err2String(errno)));
               break;
            }
         // keep clock high for at least 30 microseconds before the next bit
            SpinSleep(35);
            Mask <<= 1;
         }
      }
   } while(FALSE);
   ReleaseIRLP();
   return 0;
}
