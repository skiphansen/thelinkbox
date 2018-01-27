/* Copyright (C) 2002 GFRN systems

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
   
   The basic idea of this module is to wrap the gethostbyname() libary
   call with some code that prevents it's use by the mainloop most of the
   time.  gethostbyname() is a blocking call which is death to a time critical
   application such as a VoIP conference bridge.  To prevent the cache from 
   becoming stale the cache is updated periodically by forking (in *nix) or 
   a seperate thread (Windoze).
   

   $Log: hostfile.c,v $
   Revision 1.8  2009/07/03 17:04:51  wb6ymh
   1. Moved temp hosts file to TempDir.
   2. Don't log ip address changes on any host with "rotate" in its name.

   Revision 1.7  2008/05/14 20:59:39  wb6ymh
   Changed a bunch of functions to take const char * rather than char *
   to fix compile problems with GCC 4.2.x.

   Revision 1.6  2008/02/26 17:39:14  wb6ymh
   1. Added dmalloc support.
   2. Added HostCleanup.

   Revision 1.5  2003/08/31 23:22:39  wb6ymh
   Corrected portability problem caused by the assumption that u_long was the
   same size as an IP address.

   Revision 1.4  2003/08/16 14:00:14  wb6ymh
   1. Modified to use generalized child exit status handler.
   2. Modified DumpDNSCache() to check that the host tree has been created
      before attempting to use it.  This only occurs if the DNS cached is
      dumped before the first host lookup is attempted. This is only likely
      to occur when logins with the directory server have been disabled.

   Revision 1.3  2003/04/30 21:57:42  wb6ymh
   Modified CreateHostFile() and LoadHostFile() to use the AppName
   configuration variable's value as the application name when creating
   filenames rather than a hardcoded "tbd".

   Revision 1.2  2002/11/05 05:29:33  wb6ymh
   Added fflush() just before fork in UpdateDNSCache() to prevent dirty stream
   files from being corrupted when the forked process exits.

   Revision 1.1  2002/11/02 15:50:25  wb6ymh
   Initial import.


*/

#include "common.h"

#ifndef _WIN32
   // FreeBSD, Linux, etc..
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <arpa/inet.h>
   #ifdef TIME_WITH_SYS_TIME
      #include <sys/time.h>
      #include <time.h>
   #else
      #ifdef HAVE_SYS_TIME_H
         #include <sys/time.h>
      #else
         #include <time.h>
      #endif
   #endif
#ifdef HAVE_SYS_SIGNAL_H
   #include <sys/signal.h>
#endif
   #include <string.h>

#ifdef HAVE_LIMITS_H
   #include <limits.h>
#else
   #define  INT_MAX     0x7fffffff  /* assume 32 bit ints */
#endif
   #include <errno.h>
   #include <syslog.h>
   #include <stdarg.h>
   #ifdef HAVE_SYS_TIMEB_H
      #include <sys/timeb.h>
   #endif
   #include <netdb.h>
   #include <sys/types.h>
   #include <sys/wait.h>
   #define TAKE_MUTEX(x)
   #define GIVE_MUTEX(x)
   #define STDCALL
#else
   // Windoze
   #include <stdio.h>
   #include <time.h>
   #include <sys\timeb.h>
   #include <io.h>
   #include <winsock2.h>
   #include <limits.h>
   #include <stdarg.h>
   #include <process.h>
   #define TAKE_MUTEX(x) EnterCriticalSection(x);
   #define GIVE_MUTEX(x) LeaveCriticalSection(x);
   #define STDCALL   __stdcall
#endif

#include "avl.h"
#include "configvars.h"
#include "main.h"
#include "users.h"
#include "hostfile.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define  HOST_FILENAME_EXT "hosts"
#define  TEMP_EXT          "temp"
#define  STACKSIZE         8192

struct avl_table *HostTree;

u_long HostIP;
static struct hostent HostEnt;
time_t LastHostFileUpdate;
char *AdrPtrs[2] = {(char *) &HostIP, NULL };

int bLoadHostFileOnStart;
int bUpdateProcessRunning;
int bHostFileUpdateRequested;
int bQuickStart;

#ifdef _WIN32
CRITICAL_SECTION HostMutex;
#endif

typedef struct {
   char *HostName;
   u_long   IPAdr;
} HostEntry;


int HostCompare(const void *avl_a,const void *avl_b,void *avl_param)
{
   HostEntry *p_a = (HostEntry *) avl_a;
   HostEntry *p_b = (HostEntry *) avl_b;

   return strcmp(p_a->HostName,p_b->HostName);
}


// NB: this function runs in a different thread (Windows) or process (*nix) 
// than the main thread.  It simply updates the hostfile and exits.

unsigned STDCALL CreateHostFile(void *Dummy)
{
   struct avl_traverser avl_trans;
   struct hostent *pTemp = NULL;
   HostEntry *pHost;
   IPAdrUnion IPAdr;
   char  TempFilename[80];
   char  HostFilename[80];
   FILE *fp;

   snprintf(HostFilename,sizeof(HostFilename),"%s/%s." HOST_FILENAME_EXT,
            TempDir,AppName);
   snprintf(TempFilename,sizeof(TempFilename),"%s." TEMP_EXT,HostFilename);

   if((fp = fopen(TempFilename,"w")) != NULL) {
      TAKE_MUTEX(&HostMutex);
      pHost = (HostEntry *) avl_t_first(&avl_trans,HostTree);
      GIVE_MUTEX(&HostMutex);

      while(pHost != NULL) {
         pTemp = gethostbyname(pHost->HostName);
         if(pTemp != NULL) {
            IPAdr.ADDR = IP_FROM_HOSTENT(pTemp,0);
            if(IPAdr.ADDR != INADDR_NONE) {
               fprintf(fp,"%s\t%s\n",inet_ntoa(IPAdr.i.sin_addr),
                       pHost->HostName);
               pHost->IPAdr = IPAdr.ADDR;
            }
         }
         TAKE_MUTEX(&HostMutex);
         pHost = (HostEntry *) avl_t_next(&avl_trans);
         GIVE_MUTEX(&HostMutex);
      }
      fclose(fp);
      unlink(HostFilename);
      rename(TempFilename,HostFilename);
   }

#ifdef _WIN32
   bUpdateProcessRunning = FALSE;
   _endthreadex(0);
#endif

   return 0;
}

void AddDNSEntry(const char *HostName,u_long Adr)
{
   HostEntry Lookup;
   HostEntry *pHost;
   IPAdrUnion IPAdr;
   char temp[32];

   Lookup.HostName = (char *) HostName;

   TAKE_MUTEX(&HostMutex);
   pHost = avl_find(HostTree,&Lookup);
   GIVE_MUTEX(&HostMutex);

   if(pHost != NULL) {
   // Entry already exists, just update the IP address
      if(pHost->IPAdr != Adr) {
         IPAdr.ADDR = Adr;
         strcpy(temp,inet_ntoa(IPAdr.i.sin_addr));
         IPAdr.ADDR = pHost->IPAdr;
         if(strstr(pHost->HostName,"rotate") == NULL) {
            LOG_NORM(("Host %s IP address changed from %s to %s.\n",
                      pHost->HostName,inet_ntoa(IPAdr.i.sin_addr),temp));
         }
         pHost->IPAdr = Adr;
      }
   }
   else {
      pHost = malloc(sizeof(HostEntry) + strlen(HostName)+1);
      
      if(pHost != NULL) {
         pHost->HostName = (char *) (pHost+1);
         strcpy(pHost->HostName,HostName);
         pHost->IPAdr = Adr;
         TAKE_MUTEX(&HostMutex);
         avl_insert(HostTree,pHost);
         GIVE_MUTEX(&HostMutex);
      }
   }
}

void LoadHostFile()
{
   char Line[80];
   char  *WhiteSpace = "\t\n ";
   FILE *fp;
   char  *IPAdrString;
   u_long IPAdr;
   char  *HostName;
   int   LineNum = 0;
   char  HostFilename[80];

   HostEnt.h_addr_list[0] = (char *) &HostIP;
   LastHostFileUpdate = TimeNow.tv_sec;
   snprintf(HostFilename,sizeof(HostFilename),"%s/%s." HOST_FILENAME_EXT,
            TempDir,AppName);

   if((fp = fopen(HostFilename,"r")) != NULL) {
      while(fgets(Line,sizeof(Line),fp) != NULL) {
         LineNum++;
         IPAdrString = strtok(Line,WhiteSpace);
         if(IPAdrString == NULL || *IPAdrString == '#') {
         // Empty line or comment
            continue;
         }
         HostName = strtok(NULL,WhiteSpace);
         if(HostName != NULL) {
            if((IPAdr = inet_addr(IPAdrString)) == INADDR_NONE) {
               LOG_WARN(("Ignoring line %d in host file, invalid IP address.\n",
                         LineNum));
               continue;
            }
            AddDNSEntry(HostName,IPAdr);
         }
         else {
            LOG_WARN(("Ignoring line %d in host file, missing hostname.\n",
                      LineNum));
         }
      }
      fclose(fp);
   }
}

#ifdef _WIN32
// Windows start a new thread to update the host file
void UpdateDNSCache(int bForced)
{
   HANDLE ThreadHandle;
   DWORD ThreadID;

   if(!bUpdateProcessRunning && HostTree->avl_count > 0 && 
      (bForced || 
       (DnsUpdateInterval != 0 && 
        (TimeNow.tv_sec - LastHostFileUpdate) > DnsUpdateInterval)))
   {
      bUpdateProcessRunning = TRUE;
      LastHostFileUpdate = TimeNow.tv_sec;

      D2PRINTF(("Starting DNS cache update thread\n"));
      ThreadHandle = (HANDLE) _beginthreadex(NULL,STACKSIZE,CreateHostFile,
                                             NULL,0,&ThreadID);

      if(ThreadHandle == NULL) {
         LOG_ERROR(("UpdateDNSCache(): CreateThread failed, Err = %u\n",errno));
      }
      else {
         CloseHandle(ThreadHandle);
      }
   }
}

#else // *nix


void HostUpdateCB(int Pid, int Status)
{
   bUpdateProcessRunning = FALSE;
}

// *Nix - fork a new process to update the host file
void UpdateDNSCache(int bForced)
{
   pid_t UpdateHostFilepid;

   if(!bUpdateProcessRunning) {
      if(bHostFileUpdateRequested) {
      // An updated hosts file is available, load it
         bHostFileUpdateRequested = FALSE;
         D2PRINTF(("Host file updated, loading it\n"));
         LoadHostFile();
         RefreshACL();
      }

      if(HostTree->avl_count > 0 && 
         (bForced || 
         (DnsUpdateInterval != 0 && 
         (TimeNow.tv_sec - LastHostFileUpdate) > DnsUpdateInterval)))
      {
         bHostFileUpdateRequested = TRUE;
         bUpdateProcessRunning = TRUE;
         LastHostFileUpdate = TimeNow.tv_sec;

      // NB: make sure all buffered streams are written to disk,
      // otherwise the data will be written twice, once by the main
      // process and once by the forked process !

         if(fflush(NULL)) {
            LOG_ERROR(("UpdateDNSCache(): fflush failed %s",Err2String(errno)));
         }

         D2PRINTF(("Forking DNS cache update process\n"));
         if((UpdateHostFilepid = fork()) < 0) {
            bHostFileUpdateRequested = FALSE;
            bUpdateProcessRunning = FALSE;
            LOG_ERROR(("UpdateDNSCache(): fork failed %s",Err2String(errno)));
         }
         else if(UpdateHostFilepid == 0) {
         // child
            CreateHostFile(NULL);
            exit(0);
         }
         else {
         // Parent
            RegisterExitCB(UpdateHostFilepid,HostUpdateCB);
         }
      }
   }
}
#endif

void DumpDNSCache(ClientInfo *p)
{
   struct avl_traverser avl_trans;
   HostEntry *pHost = NULL;
   IPAdrUnion IPAdr;

   if(HostTree != NULL) {
      TAKE_MUTEX(&HostMutex);
      pHost = (HostEntry *) avl_t_first(&avl_trans,HostTree);
      GIVE_MUTEX(&HostMutex);
   }

   if(pHost == NULL) {
      BufPrintf(p,"DNS cache is empty\r");
   }
   else {
      BufPrintf(p,"%s since the DNS cache was last updated.\r",
                TimeLapse2String(LastHostFileUpdate,TRUE));
      while(pHost != NULL) {
         IPAdr.ADDR = pHost->IPAdr;
         BufPrintf(p,"%s\t%s\r",pHost->HostName,inet_ntoa(IPAdr.i.sin_addr));
         TAKE_MUTEX(&HostMutex);
         pHost = (HostEntry *) avl_t_next(&avl_trans);
         GIVE_MUTEX(&HostMutex);
      }
   }
}

struct hostent *GetHostByName(const char *HostName)
{
   struct timeval StartTime;
   int DeltaT;
   HostEntry *pHost;
   HostEntry Lookup;
   struct hostent *pTemp = NULL;

   if(HostTree == NULL) {
   // First call, initialize
#ifdef _WIN32
      InitializeCriticalSection(&HostMutex);
#endif
      HostTree = avl_create(HostCompare,NULL,NULL);
      HostEnt.h_addr_list = (char **) &AdrPtrs;
      if(bLoadHostFileOnStart) {
         LoadHostFile();
      }
   // set up the DNS cache refresh timer to go off a minute from now so
   // we don't generate a host file with one entry during the startup
   // frenzy.
      LastHostFileUpdate = TimeNow.tv_sec - DnsUpdateInterval + 60;
   }

   if((HostIP = inet_addr(HostName)) != INADDR_NONE) {
   // Converted as a numeric IP address, don't cache it
      pTemp = &HostEnt;
   }
   else if(HostTree != NULL) {
      UpdateDNSCache(FALSE);
      
      Lookup.HostName = (char *) HostName;
      
      TAKE_MUTEX(&HostMutex);
      pHost = avl_find(HostTree,&Lookup);
      GIVE_MUTEX(&HostMutex);
      if(pHost != NULL) {
      // Return value from cache
         HostIP = pHost->IPAdr;
         pTemp = &HostEnt;
      }
      else {
      // Not in our cache, add it
         GetTimeNow();
         StartTime = TimeNow;
         pTemp = gethostbyname(HostName);
         GetTimeNow();
         DeltaT = (TimeNow.tv_sec - StartTime.tv_sec)*1000 + 
                  ((TimeNow.tv_usec - StartTime.tv_usec)/1000);

         if(DeltaT > 1500 || DebugLevel > 1) {
         // Lookup took > 1.5 seconds
            LOG_WARN(("It took %d milliseconds for gethostbyname to %s %s.\n",
                      DeltaT,pTemp == NULL ? "fail to resolve" : "resolve",
                      HostName));
         }

         if(pTemp != NULL) {
            AddDNSEntry(HostName,IP_FROM_HOSTENT(pTemp,0));
         }
      }
   }
   
   return pTemp;
}

void HostCleanup()
{
   struct avl_traverser avl_trans;
   HostEntry *pHost;
   HostEntry *pNextHost;

   if(HostTree != NULL) {
      pNextHost = (HostEntry *) avl_t_first(&avl_trans,HostTree);
      while((pHost = pNextHost) != NULL) {
         pNextHost = (HostEntry *) avl_t_next(&avl_trans);
         if(avl_delete(HostTree,pHost) == NULL) {
            LOG_ERROR(("HostCleanup: avl_delete() failed to find host \"%s\".\n",
                       pHost->HostName));
         }
         printf("Freeing entry for %s\n",pHost->HostName);
         free(pHost);
      }
      avl_destroy(HostTree,NULL);
   }
}

