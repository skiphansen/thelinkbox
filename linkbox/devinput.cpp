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

   $Log: devinput.cpp,v $
   Revision 1.6  2009/01/03 22:51:44  wb6ymh
   1. Replaced some left over debug printfs with LOG_ERROR.
   2. Added check for HAVE_EVIOCGUNIQ.

   Revision 1.5  2008/05/14 18:16:07  wb6ymh
   Changed FindDevByAdr ClassPath argument to a const char *to fix compile
   problems with GCC 4.2.x.

   Revision 1.4  2008/03/09 17:00:50  wb6ymh
   1. Added dmalloc support.
   2. audio.h -> nodes.h.

   Revision 1.3  2007/12/14 23:22:48  wb6ymh
   1. Moved configvars.h include to fix compile errors.
   2. Corrected a couple of unused variable warnings.

   Revision 1.2  2007/12/01 06:00:26  wb6ymh
   1. Removed old broken debug code.
   2. Modified InitDeviceInput to check by SN before Address, makes logging
      less confusing and more useful.

   Revision 1.1  2007/11/26 07:29:02  wb6ymh
   1. Fixed compile errors under FreeBSD. None of this code is functional
      on FreeBSD, but it should comiple.
   2. Modified InitDeviceInput to search for input device based on USB device
      rather than requiring the user to figure it out and statically configure
      the device entry.
   3. Converted to C++.

   Revision 1.1  2007/11/21 17:48:36  wb6ymh
   Inital import - support for COS info from Linux event system.

   
*/ 
#ifdef __linux__
#include "common.h"

// FreeBSD, Linux, etc..
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <linux/input.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <dirent.h>
#ifdef HAVE_FCNTL_H
   #include <fcntl.h>
#endif

#include "avl.h"
#include "main.h"
#include "conference.h"
#include "configvars.h"
#include "ctcss.h"
#include "devinput.h"
#include "nodes.h"
#include "ports.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define OPEN_FLAGS   O_RDONLY | O_NONBLOCK

int InDevFd = -1;


int CPort::DeviceInHandler(ClientInfo *p)
{
#ifdef __linux__
   struct input_event Event;
   int Read;

   if(p->bReadReady) {
      for( ; ; ) {
         Read = read(p->Socket,&Event,sizeof(Event));
         if(Read <= 0) {
            if(errno != EAGAIN) {
               LOG_ERROR(("%s: read() failed: %s",__FUNCTION__,Err2String(errno)));
            }
            p->bReadReady = FALSE;
            break;
         }
         else if(Read == sizeof(Event)) {
         // Got an event
            if(Event.type == EV_KEY && Event.code == COS_KeyCode) {
            // COS changed state
               if(Event.value) {
                  bDevInCOS = InvertCOS ? FALSE : TRUE;
               }
               else {
                  bDevInCOS = InvertCOS ? TRUE : FALSE;
               }
            }
         }
         else {
            LOG_ERROR(("%s: read() returned %d bytes, what's up with that?\n",
                       __FUNCTION__));
            p->bReadReady = FALSE;
            break;
         }
      }
   }
#endif
   return FALSE;
}

int DeviceInHandler(ClientInfo *p)
{
    CPort *pClass = (CPort *) p->p;
    
    return pClass->DeviceInHandler(p);
}


void DeviceInCleanup(ClientInfo *p)
{
   if(p->Socket != -1) {
      close(p->Socket);
   }
}

int FindDevByAdr(char *Link,const char *ClassPath,const char *Type,char *UsbAdr)
{
   struct dirent *dirEntry;
   struct stat FileStat;
   char Path[PATH_MAX];
   DIR *dp;
   int BytesRead;
   char *cp;
   char *cp1;
   int Ret = ERR_INPUT_DEV_OPEN; // assume the worse

   if((dp = opendir(ClassPath)) != NULL) {
      while(Ret != 0 && (dirEntry = readdir(dp)) != NULL) {
         do {
            snprintf(Path,PATH_MAX,"%s/%s",ClassPath,dirEntry->d_name);

            if(strstr(Path,Type) == NULL) {
               break;
            }

            if(lstat(Path,&FileStat)) {
            // Couldn't stat the filename, ignore it
               break;
            }

            strcat(Path,"/device");

            BytesRead = readlink(Path,Link,PATH_MAX - 1);
            if(BytesRead < 0) {
               break;
            }

            Link[BytesRead] = 0;

            if(strstr(Link,"/usb") == NULL) {
            // Not on a USB bus, ignore
               break;
            }

            if((cp = strrchr(Link,'/')) == NULL) {
               LOG_ERROR(("%s#%d: Parse error\n",__FUNCTION__,__LINE__));
               break;
            }
            cp++;
            if((cp1 = strchr(cp,':')) == NULL) {
               LOG_ERROR(("%s#%d: Parse error (%s)\n",__FUNCTION__,__LINE__,cp));
               break;
            }
            *cp1 = 0;

            if(strcmp(cp,UsbAdr) == 0) {
               if((cp = strrchr(Path,'/')) == NULL) {
                  LOG_ERROR(("%s#%d: Parse error\n",__FUNCTION__,__LINE__));
                  break;
               }
               *cp = 0;
               
               if((cp = strrchr(Path,'/')) == NULL) {
                  LOG_ERROR(("%s#%d: Parse error\n",__FUNCTION__,__LINE__));
                  break;
               }
               cp++;
               snprintf(Link,PATH_MAX,"/dev/input/%s",cp);
               LOG_ERROR(("%s: event device for \"%s\" is %s\n",
                          __FUNCTION__,UsbAdr,Link));
               Ret = 0;
            }
         } while(FALSE);
      }
      closedir(dp);
   }

   return Ret;
}

int FindInputDevBySn(char *Path,char *AudioDongleSN) 
{
#ifdef HAVE_EVIOCGUNIQ
   struct dirent *dirEntry;
   struct stat FileStat;
   char DeviceSn[32];
   int Version;
   DIR *dp;
   int fd = -1;
   int i;
   int Ret = ERR_INPUT_DEV_OPEN; // assume the worse

   if((dp = opendir("/dev/input")) != NULL) {
      while(Ret != 0 && (dirEntry = readdir(dp)) != NULL) {
         do {
            snprintf(Path,PATH_MAX,"%s/%s","/dev/input",dirEntry->d_name);
            if(stat(Path,&FileStat)) {
            // Couldn't stat the filename, ignore it
               break;
            }

            if(!(FileStat.st_mode & S_IFCHR)) {
            // Not a a character device, ignore it
               break;
            }

            if((fd = open(Path,O_RDONLY)) < 0) {
               LOG_ERROR(("open(%s) failed - %s\n",Path,Err2String(errno)));
               break;
            }

            if(ioctl(fd,EVIOCGVERSION,&Version) < 0) {
               break;
            }

            if((i = ioctl(fd,EVIOCGUNIQ(sizeof(DeviceSn)),DeviceSn)) < 0) {
               break;
            }

            DeviceSn[i] = 0;
            if(strcmp(DeviceSn,AudioDongleSN) == 0) {
               // This is our guy
               Ret = 0;
               LOG_ERROR(("%s: event device for \"%s\" is %s\n",
                          __FUNCTION__,AudioDongleSN,Path));
            }
         } while(FALSE);

         if(fd != -1) {
            close(fd);
            fd = -1;
         }
      }
      closedir(dp);
   }
   else {
      LOG_ERROR(("%s: opendir(/dev/input) failed - %s\n",__FUNCTION__,
                 Err2String(errno)));
   }

   return Ret;
#else
   LOG_ERROR(("%s: Error: your OS does not support device serial numbers\n",
              __FUNCTION__));
   return ERR_NOT_SUPPORTED;
#endif
}

// The event subsystem isn't specific to USB ... maybe allow user to 
// specify the /dev/input device name someday???
int CPort::InitDeviceInput() 
{
   char Path[PATH_MAX];
   int Ret = ERR_INPUT_DEV_OPEN; // assume the worse
   ClientInfo *pInputC = NULL;

   if(AudioDongleSN != NULL) {
      Ret = FindInputDevBySn(Path,AudioDongleSN);
   }
   else if(AudioDongleAdr != NULL) {
      Ret = FindDevByAdr(Path,"/sys/class/input","/event",AudioDongleAdr);
   }

   if(Ret == 0) do {
      pInputC = CreateNewClient();
      if(pInputC == NULL) {
         Ret = ERR_MALLOC;
         break;
      }

      pInputC->p = this;
      pInputC->Cleanup = DeviceInCleanup;
      pInputC->State = ::DeviceInHandler;
      if((pInputC->Socket = open(Path,OPEN_FLAGS)) < 0) {
         LOG_ERROR(("%s: open(\"%s\") failed: %s",__FUNCTION__,Path,
                    Err2String(errno)));
         break;
      }

      avl_insert(ClientTree,pInputC);
      pInputC->bInClientTree = TRUE;
      pInputC->bReadWait = TRUE;
      Ret = 0;
   } while(FALSE);

   return Ret;
}
#endif

