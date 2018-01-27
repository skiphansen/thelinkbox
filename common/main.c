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

   This is a generic main loop for an application that's primarily an
   communications server.  It's primarily a just a small wrapper function
   for the magic select() system call.  
   
   $Log: main.c,v $
   Revision 1.18  2012/12/09 16:21:46  wb6ymh
   Added support for LogFileRolloverType '5' - monthly logs.

   Revision 1.17  2010/01/14 19:40:34  wb6ymh
   Modified to call select() from SelectCall when built for profiling.
   Hopefully tbd/tlb will spend most of its life in select so it's a
   particulary interesting function call to get stats on.

   Revision 1.16  2009/02/08 16:42:42  wb6ymh
   Added fflush to DumpMe to flush files before crashing.

   Revision 1.15  2008/05/14 18:10:43  wb6ymh
   1. Changed a bunch of functions to take const char * rather than char *
      to fix compile problems with GCC 4.2.x.
   2. Added LogHex.

   Revision 1.14  2008/02/26 18:02:08  wb6ymh
   Added dmalloc support.

   Revision 1.13  2007/12/27 15:59:13  wb6ymh
   Modified ChildSigHandler to save and restore errno! The new waitpid
   loop clobbers errno which causes ECHILD to overwrite EINTR causing
   interrupted system calls to appear to return ECHILD causing confusion
   and an exit!  God I hate signal and global variables!

   Revision 1.12  2007/12/22 17:27:15  wb6ymh
   Modified ChildSigHandler to loop until waitpid finds no more children.
   Previously we used wait and could miss children exiting if more than one
   child exited before we ran.  Fixes event script failures.

   Revision 1.11  2007/12/01 00:54:44  wb6ymh
   Changed a bunch of D2PRINTFs to D3PRINTFs to quiet things down at
   debuglevel 2.

   Revision 1.10  2007/07/02 13:36:23  wb6ymh
   Changed EOL convention back to Unix style.

   Revision 1.9  2007/07/02 13:34:32  wb6ymh
   Added hooks for tbdQt project by Scott KI4LKF.

   Revision 1.8  2006/08/05 23:27:33  wb6ymh
   Applied patch supplied by Satoshi Yasuda(7m3tjz/ad6gz) to correct logfile
   filename generation for the daily log option.  Thanks Satoshi!

   Revision 1.7  2003/08/31 23:09:10  wb6ymh
   Modified Err2String() to eliminate the use of sys_nerr.  It's not portable
   and although it's exported by SunOS 5.9 it doesn't appear in any header
   file.

   Revision 1.6  2003/08/16 13:37:15  wb6ymh
   1. Added generalized handling to retrieve exit status from child processes.
   2. Modified logging routine to allow it to be used under *nix as well as
      Windoze.
   3. Replaced ReadLine() kludge with GetRxData(). End of line detection and
      processing is now the responsibility of the client.
   4. Modified BufPrintf() to work with more flavors of snprintf correctly.
      (The C99 standard defined the behavour differently than eariler
      implementations.)
   5. Replaced *nix syslog specific %m with calls to new Err2String() routine.

   Revision 1.5  2003/04/30 22:05:01  wb6ymh
   Modified Log() to use the AppName configuration variable's value as the
   application name when creating pid files rather than a hardcoded "tbd".

   Revision 1.4  2003/01/05 17:01:41  wb6ymh
   1. Fixed infinite loop in ReadLine which occurs when read() fails with
      anything other than no data available.
   2. Modified Log() to avoid erasing the current day's log on startup for
      LogFileRolloverType of 3 or 4.
   3. Modified Log() to call DumpMe() when the log file grows too large
      rather than attempting to do a graceful exit.  The assumption is that
      the excessively large log was caused by an infinite loop and a
      graceful exit is impossible anyway.

   Revision 1.3  2003/01/01 19:06:14  wb6ymh
   1. Added timeout detection to ReadLine().
   2. Modified ClientCompare to sort by SN rather than HisAdr. This is necessary
      to allow multiple connections to the same remote host:port.
   3. Added infinite loop detection logic to CallCurrentState().
   4. Modified Log() to close and (possibly) reopen log when LogFileRolloverType
      changes.
   5. Added code to cause the application to exit if the current log file
      exceeds 10mb in size.

   Revision 1.2  2002/12/18 00:53:36  wb6ymh
   Added logging routines for Windows.

   Revision 1.1.1.1  2002/11/02 17:03:54  wb6ymh
   Moved common source files to common subdirectory from src.

   Revision 1.6  2002/09/14 16:53:46  wb6ymh
   Modified DeltaTime2String() to suppress day display unless days > 0.
   Removed extra linefeeds in Log*() when logging directly to console.

   Revision 1.5  2002/09/02 15:32:19  wb6ymh
   Changed the way clients are deleted from the ClientTree.  Previously the
   mainloop kept a pointer to the next client to be processed while calling
   the current client so the current client could deleted itself.  This caused
   problems when the current client deleted what would have been the next client
   to be processed (like when dirclient deleted hung clients when it tried to
   start a new request).  Now DeleteClient() doesn't delete the current client,
   it only flags it for deletion by the main loop when the mainloop finishes
   with it.

   Revision 1.4  2002/08/31 23:48:33  wb6ymh
   Added ability to wake on socket exceptions.  Added DeltaTime2String().
   Added debug code to track the number of open sockets in OpenSockets variable.
   Modified SigHandler to cause an immediate exit if a second SIGINT or
   SIGTERM is received.

   Revision 1.3  2002/08/18 16:34:10  wb6ymh
   Added DumpMe() to cause core dumps on demand for debugging.
   Added call to DumpMe() in DeleteClient() when client being deleted isn't
   found in AVL tree. (caused infinite loop when EchoLink server went down
   recently).  I don't see the problem, the coredump will tell all.

   Revision 1.2  2002/08/12 17:10:47  wb6ymh
   Made TimeNow a global.
   Removed argument to GetTimeNow().

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/
#include "common.h"

#ifndef _WIN32
   // FreeBSD, Linux, etc..
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include <netinet/in.h>
   #include <sys/socket.h>
   #include <ctype.h>
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
   #include <signal.h>
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
   #include <sys/types.h>
   #include <sys/wait.h>
   #include <fcntl.h>
#else
   // Windoze
   #include <stdio.h>
   #include <time.h>
   #include <sys\timeb.h>
   #include <io.h>
   #include <winsock2.h>
   #include <limits.h>
   #include <stdarg.h>
   #include <ctype.h>
   #define  ProcessExitStatus()
#endif

#include "avl.h"
#include "main.h"
#include "tbdthread_common.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define MAX_SEND_SIZE      1024
#define CLIENT_RX_BUF_SIZE 1024

int SN;
int DebugLevel;
unsigned int Debug;  // Same as above but a bit map
int bShutdown = FALSE;
int bRunning = TRUE;
int bRefreshConfig = FALSE;
int bDaemon = FALSE;

time_t BootTime;
struct timeval TimeNow;

struct avl_table *ClientTree;

ClientInfo *pClientDebug = NULL;
int OpenSockets = 0;


void CallCurrentState(ClientInfo *pClient);

#ifndef _WIN32
// FreeBSD, Linux, etc..
void SigHandler(int Signal);
void ChildSigHandler(int Signal);
void ProcessExitStatus();
int LogFileRolloverType = 0;  // use syslog

#define MAX_CHILDREN 16
// Exit status ring buffer
struct {
   int ExitStatus;
   int Pid;
} ChildExitStatus[MAX_CHILDREN];
int CurrentChildCount = 0;

int ExitStatWr = 0;
int ExitStatRd = 0;
int bExitStatRingOvfl = 0;
int ExitStatRingOvfls = 0;

struct {
   int ExitStatus;
   int Pid;
   ExitCB ExitCB;
} ExitCallBacks[MAX_CHILDREN];


// child exit callbacks

#else
BOOL SigHandler(DWORD Type);
int LogFileRolloverType = -1;  // no rollover, single logfile
#endif

void vLog(const char *fmt,va_list args);

#ifdef PROFILED_BUILD
// wrap certain system calls in a routine so the profiling code can
// track them.
int SelectCall(int a1,fd_set *a2,fd_set *a3,fd_set *a4,struct timeval *a5)
{
   return select(a1,a2,a3,a4,a5);
}

#define select(a1,a2,a3,a4,a5) SelectCall(a1,a2,a3,a4,a5)

#endif

int MainLoopInit()
{
#ifndef _WIN32
   struct sigaction SigAction;
   
// Setup signal handlers using sigaction() rather than the signal() quagmire!

   SigAction.sa_handler = SigHandler;
   SigAction.sa_flags = 0;
   sigemptyset(&SigAction.sa_mask);

   sigaction(SIGHUP,&SigAction,NULL);
   sigaction(SIGTERM,&SigAction,NULL);
   sigaction(SIGINT,&SigAction,NULL);

   SigAction.sa_handler = ChildSigHandler;
   sigaction(SIGCHLD,&SigAction,NULL);
#else
   SetConsoleCtrlHandler((PHANDLER_ROUTINE) SigHandler,TRUE);                           // add to list 
#endif
   BootTime = time(NULL);
   return 0;
}

int MainLoop()
{
   fd_set ReadFDset;
   fd_set WriteFDset;
   fd_set ExceptFDset;
   int Selret;
   ClientInfo *pClient;
   ClientInfo *pClient1;
   struct avl_traverser avl_trans;
   struct timeval SelTO;
   long t;
   int ReadWaitCnt;
   int WriteWaitCnt;
   int ExceptWaitCnt;
   int Err = 0;
   long TimeOutSeconds;

   while(bRunning) {
      if(bRefreshConfig) {
         bRefreshConfig = FALSE;
         if(ReadConfig()) {
            LOG_ERROR(("parse error reloading configuration file, exiting\n"));
            Err = ERR_CONFIG_FILE;
            break;
         }
      }

      if(bShutdown) {
         bShutdown = FALSE;
         LOG_NORM(("Starting shut down\n"));
         Shutdown();
         if(!bRunning) {
            break;
         }
      }

      FD_ZERO(&ReadFDset);
      FD_ZERO(&WriteFDset);
      FD_ZERO(&ExceptFDset);

#ifndef _WIN32
      SelTO.tv_sec = INT_MAX;
#else
   // No infinite wait in Windoze even if no clients request a timeout
      SelTO.tv_sec = INT_MAX - 1;
#endif

      SelTO.tv_usec = 0;
      ReadWaitCnt = WriteWaitCnt = ExceptWaitCnt = 0;

      pClient = (ClientInfo *) avl_t_first(&avl_trans,ClientTree);
      while(pClient != NULL) {
         pClient->bReadReady = 
         pClient->bWriteReady = 
         pClient->bTimeOut =
         pClient->bNew = FALSE;

         if(pClient->bReadWait) {
            ReadWaitCnt++;
            FD_SET(pClient->Socket,&ReadFDset);
         }

         if(pClient->bWriteWait) {
            WriteWaitCnt++;
            FD_SET(pClient->Socket,&WriteFDset);
         }

         if(pClient->bExceptWait) {
            ExceptWaitCnt++;
            FD_SET(pClient->Socket,&ExceptFDset);
         }

         if(pClient->bTimeWait) {
         // Set SelTO to the shortest timeout requested
            t = SelTO.tv_sec - pClient->Timeout.tv_sec;
            if(t == 0) {
               t = SelTO.tv_usec - pClient->Timeout.tv_usec;
            }
            
            if(t > 0) {
               SelTO = pClient->Timeout;
            }
         }
         pClient = (ClientInfo *) avl_t_next(&avl_trans);
      }

      if(SelTO.tv_sec != INT_MAX) {
      // Calculate delta T to shortest timeout requested
         GetTimeNow();
         SelTO.tv_usec -= TimeNow.tv_usec;
         SelTO.tv_sec -= TimeNow.tv_sec;
         if(SelTO.tv_usec < 0) {
            SelTO.tv_usec += 1000000;
            SelTO.tv_sec--;
         }
   
         if(SelTO.tv_sec < 0) {
         // Timeout time has already passed
            SelTO.tv_sec = SelTO.tv_usec = 0;
         }
         else if(SelTO.tv_sec == 0) {
            if(SelTO.tv_usec < 0) {
            // Timeout time has already passed
               SelTO.tv_usec = 0;
            }
         }

#ifdef _WIN32
         // The Windoze control-C handler can't abort the select call so
         // don't wait for more than a second so we can exit quickly when the 
         // user hits control-C

         if(SelTO.tv_sec > 1) {
            SelTO.tv_sec = 1;
            SelTO.tv_usec = 0;
         }
#endif
         TimeOutSeconds = SelTO.tv_sec;

         if(SelTO.tv_sec == 0 && SelTO.tv_usec == 0) {
            D3PRINTF(("Calling select() %d read, %d write, %d except, "
                      "immediate timeout\n",ReadWaitCnt,WriteWaitCnt,
                      ExceptWaitCnt));
         }
         else {
            D3PRINTF(("Calling select() %d read, %d write, %d except, "
                      "%ld sec timeout\n",ReadWaitCnt,WriteWaitCnt,
                      ExceptWaitCnt,TimeOutSeconds));
         }
         Selret = select(FD_SETSIZE,&ReadFDset,&WriteFDset,&ExceptFDset,&SelTO);
      }
      else {
         D3PRINTF(("Calling select() %d read, %d write, %d except, no timeout\n",
                  ReadWaitCnt,WriteWaitCnt,ExceptWaitCnt));
         Selret = select(FD_SETSIZE,&ReadFDset,&WriteFDset,&ExceptFDset,NULL);
      }

      GetTimeNow();
      ProcessExitStatus();

#ifndef _WIN32
      if(Selret == SOCKET_ERROR) {
         if(errno != EINTR) {
            LOG_ERROR(("select() failed, %s",Err2String(errno)));
            Err = ERROR_CODE;
            break;
         }

         if(!bRunning) {
            break;
         }
         else {
            continue;
         }
      }

#else 
      if(Selret == SOCKET_ERROR) {
         Err = ERROR_CODE;
         LOG_ERROR(("select() failed, %s",Err2String(Err)));
         break;
      }
#endif

      pClient1 = (ClientInfo *) avl_t_first(&avl_trans,ClientTree);

      while((pClient = pClient1) != NULL) {
         if(pClient->bNew) {
         // Ignore this guy, he was created since the last time we called select.
            pClient1 = (ClientInfo *) avl_t_next(&avl_trans);
            continue;
         }

         if(pClient->bReadWait && FD_ISSET(pClient->Socket,&ReadFDset)) {
            pClient->bReadReady = TRUE;
            D3PRINTF(("client %u socket ready for read\n",pClient->SN));
         }
         
         if(pClient->bWriteWait && FD_ISSET(pClient->Socket,&WriteFDset)) {
            pClient->bWriteReady = TRUE;
            D3PRINTF(("client %u socket ready for write\n",pClient->SN));
         }

         if(pClient->bExceptWait && FD_ISSET(pClient->Socket,&ExceptFDset)) {
            pClient->bException = TRUE;
            D3PRINTF(("client %u socket has an exception\n",pClient->SN));
         }


         if(pClient->bTimeWait && 
            (TimeNow.tv_sec > pClient->Timeout.tv_sec ||
            (TimeNow.tv_sec == pClient->Timeout.tv_sec &&
             TimeNow.tv_usec >= pClient->Timeout.tv_usec)) )
         {
               pClient->bTimeOut = TRUE;
               D3PRINTF(("client %u timer timeout\n",pClient->SN));
         }
         
         if(pClient->bReadReady || pClient->bWriteReady || 
            pClient->bException || pClient->bTimeOut)
         {
            pClient->bCurrent = TRUE;
            CallCurrentState(pClient);
            pClient->bCurrent = FALSE;
         }

         pClient1 = (ClientInfo *) avl_t_next(&avl_trans);
         if(pClient->bDelete) {
         // Delete the last client
            DeleteClient(pClient);
         }
      }
   }
   return Err;
}


int DeleteClient(ClientInfo *pClient)
{
   if(pClient->bCurrent) {
   // This is the current client, NEVER delete the current client while
   // transversing the tree or the client tree will be corrupted.  
   // Just flag it for deletion later.
      pClient->bDelete = TRUE;
   }
   else {
      if(pClient->Socket != INVALID_SOCKET) {
         CLOSE(pClient->Socket);
         OpenSockets--;
      }
      if(pClient->bInClientTree && avl_delete(ClientTree,pClient) == NULL) {
      // This should never happen, log it and core dump
         LOG_ERROR(("DeleteClient(): avl_delete() failed to client %d\n",
                    pClient->SN));
         DumpMe();
      }
      else {
         if(pClient->Buf != NULL) {
            free(pClient->Buf);
         }

         if(pClient->Cleanup != NULL) {
            pClient->Cleanup(pClient);
         }

         free(pClient);
      }
   }

   return FALSE;
}


int SendBuffer(ClientInfo *pClient)
{
   int SendLen;
   int BytesSent;
   int Ret = 0;

   SET_WAIT4_WR(pClient);

   while((SendLen = pClient->BufSize - pClient->Count) > 0) {
      if(SendLen > MAX_SEND_SIZE) {
         SendLen = MAX_SEND_SIZE;
      }
      D3PRINTF(("SendBuffer(), sending %d of %d bytes.\n",SendLen,
               pClient->BufSize));

      BytesSent = WRITE(pClient->Socket,&pClient->Buf[pClient->Count],SendLen);
      if(BytesSent == SOCKET_ERROR ) {
         if(ERROR_CODE == ERROR_WOULD_BLOCK) {
         // buffer full, try again later
            break;
         }
         else {
         // Some kind of error other than buffer full
            LOG_WARN(("SendBuffer(), WRITE() failed, %s",
                      Err2String(ERROR_CODE)));
            DeleteClient(pClient);
            break;
         }
      }
      else {
         pClient->Count += SendLen;
      }
   }

   if(SendLen == 0) {
   // We're done with the guy for now, close the socket
      D3PRINTF(("SendBuffer(), send complete.\n"));
      pClient->State = pClient->NextState;
      Ret = 1;
   }

   return Ret;
}


// Return true if more data read or an error occurs
int GetRxData(ClientInfo *p)
{
   int BytesRead;
   int Ret = FALSE;

   if(p->Count > 0 && p->RetCount > 0) {
   // move remaining data down to bottom of buffer
      memmove(p->Buf,&p->Buf[p->RetCount],p->Count+1);
   }
   p->RetCount = 0;

   // Read some more data
   BytesRead = READ(p->Socket,&p->Buf[p->Count],p->BufSize-p->Count-1);

   if(BytesRead > 0) {
      p->Count += BytesRead;
      p->Buf[p->Count] = 0;
      Ret = TRUE;
   }
   else if(BytesRead == 0) {
   // Connection has closed
      D2PRINTF(("GetRxData(): Client %u connection closed.\n",p->SN));
      p->Err = ERR_CONNECTION_CLOSED;
      Ret = TRUE;
   }
   else {
      if(ERROR_CODE == ERROR_WOULD_BLOCK) {
      // Nothing available to read now, keep waiting
         SET_WAIT4_RD(p);
      }
      else {
      // Some kind of error other than no data available
         LOG_WARN(("GetRxData(): READ() failed client %u, %s",
                  p->SN,Err2String(ERROR_CODE)));
         p->Err = ERROR_CODE;
         Ret = TRUE;
      }
   }

   return Ret;
}


// Client compares by SN
int ClientCompare(const void *avl_a,const void *avl_b,void *avl_param)
{
   ClientInfo *p_a = (ClientInfo *) avl_a;
   ClientInfo *p_b = (ClientInfo *) avl_b;

   return p_a->SN - p_b->SN;
}


ClientInfo *CreateNewClient()
{
   ClientInfo *pClient = malloc(sizeof(ClientInfo));

   if(pClient != NULL) {
      memset(pClient,0,sizeof(ClientInfo));
      pClient->SN = SN++;
      pClient->Socket = INVALID_SOCKET;
      pClient->bNew = TRUE;
   }
   return pClient;
}


void CallCurrentState(ClientInfo *pClient)
{
   int bLoop = FALSE;
   int LoopCounter = 100000;

   do {
      if(pClient->State == NULL) {
         LOG_ERROR(("CallCurrentState(): Client %d attempted to call "
                    "undefined state!\n",pClient->SN));
         DumpMe();
      }
      else {
         bLoop = pClient->State(pClient);
      }
   } while(bLoop && --LoopCounter);
   
   if(LoopCounter == 0) {
      LOG_ERROR(("CallCurrentState(): Client %d looped in state 0x%lx!\n",
                 pClient->SN,(long) pClient->State));
      DumpMe();
   }
}

// Set a timeout for <Timeout> milliseconds from now
void SetTimeout(ClientInfo *p,int Timeout)
{
   long microseconds;

   GetTimeNow();
   microseconds = TimeNow.tv_usec + ((Timeout % 1000) * 1000);

   p->bTimeWait = TRUE;
   p->Timeout.tv_usec = microseconds % 1000000;
   p->Timeout.tv_sec  = TimeNow.tv_sec + (Timeout / 1000) + 
                        microseconds / 1000000;
}


void GetTimeNow()
{
#ifdef HAVE_GETTIMEOFDAY
   struct timezone tz;
   gettimeofday(&TimeNow,&tz);
#else
   #ifdef HAVE_FTIME
      // Use ftime() and convert
      #ifndef _WIN32
         struct timeb FTimeNow;
         ftime(&FTimeNow);
      #else
         struct _timeb FTimeNow;
         _ftime(&FTimeNow);
      #endif
      
      TimeNow.tv_sec = FTimeNow.time;
      TimeNow.tv_usec = FTimeNow.millitm * 1000;
   #else
   // How the heck do you get the time of day on this OS ?
      #error Need either ftime() or gettimeofday() !
   #endif
#endif
}

char *DeltaTime2String(time_t StartTime, time_t EndTime, int bShort)
{
   static char TimeString[] = "xxxx days, xx hours and xx minutes";
   int Days, Hours, Minutes;
   
   time_t Seconds = EndTime - StartTime;
   Days = Seconds / (60 * 60 * 24);
   Seconds %= 60 * 60 * 24;
   Hours = Seconds / (60 * 60);
   Seconds %= 60 * 60;
   Minutes = Seconds / 60;
   Seconds %= 60;

   if(bShort) {
      if(Days > 0) {
         snprintf(TimeString,sizeof(TimeString),"%d/%d:%02d:%02d",
                  Days,Hours,Minutes,(int) Seconds);
      }
      else {
         snprintf(TimeString,sizeof(TimeString),"%d:%02d:%02d",
                  Hours,Minutes,(int) Seconds);
      }
   }
   else if(Days > 0) {
      snprintf(TimeString,sizeof(TimeString),"%d days, %d hours and %d minutes",
               Days,Hours,Minutes);
   }
   else if(Hours > 0) {
      snprintf(TimeString,sizeof(TimeString),"%d hours and %d minutes",Hours,
               Minutes);
   }
   else {
      snprintf(TimeString,sizeof(TimeString),"%d minutes and %d seconds",
               Minutes,(int) Seconds);
   }
   
   return TimeString;
}

char *TimeT2String(time_t time)
{
   struct tm *pTm;
   static char TimeString[] = "mm/dd hh:mm";

   if(time != 0) {
      pTm = localtime(&time);
      if(pTm != NULL) {
         snprintf(TimeString,sizeof(TimeString),"%2d/%02d %2d:%02d",
                 pTm->tm_mon + 1,pTm->tm_mday,pTm->tm_hour,pTm->tm_min);
      }
      else {
         return "";
      }
   }
   else {
      return "";
   }
   return TimeString;
}

char *TimeLapse2String(time_t StartTime,int bShort)
{
   return DeltaTime2String(StartTime,TimeNow.tv_sec,bShort);
}

void OpComplete(ClientInfo *pClient, int Err)
{
   pClient->Err = Err;
   if(pClient->Complete != NULL) {
      pClient->Complete(pClient);
   }
   else {
      DeleteClient(pClient);
   }
}


void BufPrintf(ClientInfo *p,const char *fmt,...)
{
   va_list args;
   int   Wrote;
   
   va_start(args,fmt);
   
   if(p->Count < p->BufSize) {
      Wrote = vsnprintf(&p->Buf[p->Count],p->BufSize-p->Count-1,fmt,args);
      if(Wrote > 0 && p->Count + Wrote < p->BufSize) {
         p->Count += Wrote;
      }
      p->Buf[p->Count] = 0;
   }
   va_end(args);
}


#ifndef _WIN32
void LogNorm(const char *fmt, ...)
{
   va_list args;
   va_start(args,fmt);

   if(LogFileRolloverType == 0) {
      if(bDaemon) {
         vsyslog(LOG_INFO,fmt,args);
      }
      else {
         vprintf(fmt,args);
      }
   }
   else {
      vLog(fmt,args);
   }
   va_end(args);
}

void LogWarn(const char *fmt, ...)
{
   va_list args;
   va_start(args,fmt);

   if(LogFileRolloverType == 0) {
      if(bDaemon) {
         vsyslog(LOG_WARNING,fmt,args);
      }
      else {
         vprintf(fmt,args);
      }
   }
   else {
      vLog(fmt,args);
   }
   va_end(args);
}

void LogErr(const char *fmt, ...)
{
   va_list args;
   va_start(args,fmt);

   if(LogFileRolloverType == 0) {
      if(bDaemon) {
         vsyslog(LOG_ERR,fmt,args);
      }
      else {
         vprintf(fmt,args);
      }
   }
   else {
      vLog(fmt,args);
   }
   va_end(args);
}

#endif

void LogHex(void *AdrIn,int Len)
{
   char Line[80];
   char *cp;
   unsigned char *Adr = (unsigned char *) AdrIn;
   int i = 0;
   int j;

   while(i < Len) {
      cp = Line;
      for(j = 0; j < 16; j++) {
         if((i + j) == Len) {
            break;
         }
         sprintf(cp,"%02x ",Adr[i+j]);
         cp += 3;
      }

      *cp++ = ' ';
      for(j = 0; j < 16; j++) {
         if((i + j) == Len) {
            break;
         }
         if(isprint(Adr[i+j])) {
            *cp++ = Adr[i+j];
         }
         else {
            *cp++ = '.';
         }
      }
      *cp = 0;
      LOG_ERROR(("%s\n",Line));
      i += 16;
   }
}

FILE *LogFp = NULL;

static char *LogNames[] = {
   "sun.log",
   "mon.log",
   "tue.log",
   "wed.log",
   "thr.log",
   "fri.log",
   "sat.log"
};

static char *MonthNames[] = {
   "Jan",
   "Feb",
   "Mar",
   "Apr",
   "May",
   "Jun",
   "Jul",
   "Aug",
   "Sep",
   "Oct",
   "Nov",
   "Dec"
};

/* 
We have a bunch of possible logfile formats selected by the configuration
file variable LogFileRolloverType.
   0 - logging disabled.
   1 - Never: single logfile, filename = tbd.log
   2 - Never: new logfile opened everyday, filename = tbdmmddyy.log
   3 - Daily: new logfile opened everyday, filename = tbd.log, tbd.bak
   4 - Weekly: new logfile opened everyday, filename = mon.log, tues.log, ...
*/    
void vLog(const char *fmt,va_list args)
{
   int WriteLen;
   char  Temp[1024];
   char  Temp1[1024];
   time_t ltime;
   struct tm *tm;
   static int LastLogDay = -1;
   static int LastLogType;

   time(&ltime);
   tm = localtime(&ltime);

   if(LogFileRolloverType != 0 && 
         (LogFp == NULL || 
          tm->tm_mday != LastLogDay || 
          LastLogType != LogFileRolloverType)) 
   {
      LastLogType = LogFileRolloverType;

      if(LogFp != NULL) {
         fclose(LogFp);
         LogFp = NULL;
      }
   
   // Assume the name will be tbd.log
      snprintf(Temp,sizeof(Temp),"%s.log",AppName == NULL ? "tbd" : AppName);

      switch(LogFileRolloverType) {
         case 2:  // daily log
            snprintf(Temp,sizeof(Temp),"%s%02d%02d%02d.log",AppName,
                     tm->tm_mon+1,tm->tm_mday,tm->tm_year % 100);
            break;

         case 3:  // daily log
            if(LastLogDay != -1 && tm->tm_mday != LastLogDay) {
            // A new day is born, rename the old log to .bak and open a new log
               snprintf(Temp1,sizeof(Temp1),"%s.bak",AppName);
               unlink(Temp1);
               rename(Temp,Temp1);
            }
            break;

         case 4:  // weekly
            if(LastLogDay != -1 && tm->tm_mday != LastLogDay) {
            // delete last weeks log
               unlink(LogNames[tm->tm_wday]);
            }
            strcpy(Temp,LogNames[tm->tm_wday]);
            break;

			case 5:	// Monthly
            snprintf(Temp,sizeof(Temp),"%s%02d%02d.log",AppName,
                     tm->tm_mon+1,tm->tm_year % 100);
				break;
      }

      LastLogDay = tm->tm_mday;
      LogFp = FOPEN(Temp,"a");
   }

   WriteLen = vsnprintf(Temp,sizeof(Temp),fmt,args);

/* KI4LKF: same function for either Win GUI or X11 Gui,
   but just in case */
#ifdef _WIN32
   #ifdef _WIN32GUI
   UpdateGui(GUIUPDATETYPE_LOG,Temp,WriteLen);
   #endif
#else
   #ifdef _X11GUI
   UpdateGui(GUIUPDATETYPE_LOG,Temp,WriteLen);
   #endif
#endif

   if(!bDaemon && isatty(fileno(stdin))) {
      printf("%d:%02d:%02d %s",tm->tm_hour,tm->tm_min,tm->tm_sec,Temp);
   }

   if(LogFp != NULL) {
      if(LogFileRolloverType == 1 || LogFileRolloverType == -1) {
      // one big file log date & time
         fprintf(LogFp,"%s %d %d:%02d:%02d %s",MonthNames[tm->tm_mon],
                 tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec,Temp);
      }
      else {
      // Not one big file, just log time
         fprintf(LogFp,"%d:%02d:%02d %s",tm->tm_hour,tm->tm_min,
                 tm->tm_sec,Temp);
      }
      fflush(LogFp);

      if(ftell(LogFp) > 10000000) {
      // shit, get out of dodge
         fprintf(LogFp,"LOG FILE OVERFLOW, shutting down !\n");
         fclose(LogFp);
         LogFp = NULL;
         if(isatty(fileno(stdin))) {
            printf("LOG FILE OVERFLOW, shutting down !\n");
         }
         DumpMe();
      }
   }
}

void Log(char *fmt, ...)
{
   va_list args;
   va_start(args,fmt);
   
   vLog(fmt,args);
   va_end(args);
}

void CloseLog()
{
   if(LogFp != NULL) {
      fclose(LogFp);
      LogFp = NULL;
   }
}


/* Force a code dump */
void DumpMe()
{
   char *cp = NULL;

   fflush(NULL);
   *cp = 0;
}

char *Err2String(int err)
{
   static char Ret[80];
   char *ErrMsg = strerror(err);

   if(ErrMsg != NULL) {
      snprintf(Ret,sizeof(Ret),"%s (%d)\n",strerror(err),err);
   }
   else {
      snprintf(Ret,sizeof(Ret),"err=%d\n",err);
   }
   return Ret;
}

#ifndef _WIN32
void SigHandler(int Signal)
{
   switch(Signal) {
      case SIGHUP:
         LOG_NORM(("Received SIGHUP, reloading conf file\n"));
         bRefreshConfig = TRUE;
         break;

      case SIGINT:
         LOG_NORM(("Received SIGINT, shutting down\n"));
         break;

      case SIGTERM:
         LOG_NORM(("Received SIGTERM, shutting down\n"));
         break;
   }

   if(Signal == SIGINT || Signal == SIGTERM) {
      if(!bShutdown) {
      // Try an orderly shutdown
         bShutdown = TRUE;
      }
      else {
      // Already tried an orderly shutdown, really exit now.
         bRunning = FALSE;
      }
   }
}

int SigChilds = 0;
int ChildExits = 0;
void ChildSigHandler(int Signal)
{
   int Status;
   int Pid;
// NB: save errno, our waitpid loop is going to clobber it!
   int ErrnoSave = errno;  

   SigChilds++;

   for(; ; ) {
   // Read the exit status
      if((Pid = waitpid(-1,&Status,WNOHANG)) == -1 || Pid == 0) {
         break;
      }
   
      ChildExits++;
      if(((ExitStatWr + 1) % MAX_CHILDREN) == ExitStatRd) {
      // status buffer overflow
         bExitStatRingOvfl = TRUE;
         ExitStatRingOvfls++;
      }
      else {
         ChildExitStatus[ExitStatWr].ExitStatus = Status;
         ChildExitStatus[ExitStatWr].Pid = Pid;
         ExitStatWr = (ExitStatWr + 1) % MAX_CHILDREN;
      }
   }
   errno = ErrnoSave;
}

void ProcessExitStatus()
{
   int Status;
   int Pid;
   int i;
   ExitCB ExitCB;

   while(ExitStatRd != ExitStatWr) {
      Status = ChildExitStatus[ExitStatRd].ExitStatus;
      Pid = ChildExitStatus[ExitStatRd].Pid;
      ExitStatRd = (ExitStatRd + 1) % MAX_CHILDREN;
      for(i = 0; i < MAX_CHILDREN; i++) {
         if(ExitCallBacks[i].Pid == Pid && ExitCallBacks[i].ExitCB != NULL) {
            ExitCB = ExitCallBacks[i].ExitCB;
            ExitCallBacks[i].ExitCB = NULL;
            CurrentChildCount--;
            ExitCB(Pid,Status);
            break;
         }
      }
      if(i == MAX_CHILDREN) {
         LOG_ERROR(("Warning: Callback for PID %d not found\n",Pid));
      }
   }

   if(bExitStatRingOvfl) {
      bExitStatRingOvfl = FALSE;
      LOG_ERROR(("Error: The exit status ring buffer has overflowed %d times\n",
                 ExitStatRingOvfls));
   }
}

int CallBacksRegistered = 0;
int RegisterExitCB(int Pid,ExitCB ExitCB)
{
   int i;

   CallBacksRegistered++;

   for(i = 0; i < MAX_CHILDREN; i++) {
      if(ExitCallBacks[i].ExitCB == NULL) {
         ExitCallBacks[i].Pid = Pid;
         ExitCallBacks[i].ExitCB = ExitCB;
         CurrentChildCount++;
         break;
      }
   }

   if(i == MAX_CHILDREN) {
      LOG_ERROR(("Error: RegisterExitCB() - ExitCallBacks array overflow\n"));
   }
   
   return i != MAX_CHILDREN;
}

// 
FILE *FOPEN(const char *fmt,const char *mode)
{
   FILE *fp;

   if((fp = fopen(fmt,mode)) != NULL) {
   // Set close on exec flag
      fcntl(fileno(fp),F_SETFD,FD_CLOEXEC);
   }
   return fp;
}
#else // Windoze
   
BOOL SigHandler(DWORD Type) 
{ 
   if(Type == CTRL_C_EVENT) {
   // Handle the CTRL+C signal. 
      LOG_NORM(("Received SIGINT, shutting down\n"));
      if(!bShutdown) {
      // Try an orderly shutdown
         bShutdown = TRUE;
      }
      else {
      // Already tried an orderly shutdown, really exit now.
         return FALSE;
      }
      return TRUE; 
   } 
   return FALSE; 
} 
 
#endif


