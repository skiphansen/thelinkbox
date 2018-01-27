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

   $Log: eventhook.c,v $
   Revision 1.11  2009/09/13 20:09:37  wb6ymh
   Quieted QueueEvent logging, only log if more than on event is outstanding.

   Revision 1.10  2008/06/12 23:28:45  wb6ymh
   Modified ExecHook to pass all chat text in the second argument for
   "chat" and "sent_chat" events.  Previously everything including the
   event type was passed in the first argument which made life for scripts
   harder rather than easier.

   Revision 1.9  2008/05/14 20:58:22  wb6ymh
   Changed EventHook fmt argument to a const char * to fix compile problems
   with GCC 4.2.x.

   Revision 1.8  2008/04/11 18:31:36  wb6ymh
   Modified ExecHook to pass all text for "chat" and "sent_chat" events in
   the first argument.

   Revision 1.7  2008/03/08 06:44:57  wb6ymh
   Changed audio.h -> linkbox.h.

   Revision 1.6  2008/02/26 17:34:44  wb6ymh
   Added dmalloc support.

   Revision 1.5  2007/12/07 19:14:00  wb6ymh
   Added exit(1) after execvp call in ExecHook. Previously if execvp failed
   (bogus configuration) we'd endup with two copies of tbd/tlb trying to run.
   Nothing good can come from this!

   Revision 1.4  2007/11/26 14:57:41  wb6ymh
   Changed include file order.

   Revision 1.3  2007/06/12 23:47:06  wb6ymh
   Added call to EndPointEvent from EventHook.

   Revision 1.2  2003/08/31 23:05:42  wb6ymh
   Moved include for configvars.h below include for stdlib.h.

   Revision 1.1  2003/08/16 14:30:26  wb6ymh
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
   #define InitializeCriticalSection(x)
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

#include "configvars.h"
#include "main.h"
#include "eventhook.h"

#ifdef   LINK_BOX
   #include "conference.h"
   #include "linkbox.h"
#else
   #define EndPointEvent(x)
#endif

#define  STACKSIZE         8192

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

typedef struct EventTAG {
   struct EventTAG *Link;  // NB: must be first
   char  *CmdTail;
} Event;

Event *pEventHead;
Event *pEventTail;
int EventProcessRunning = 0;

void ExecHook(Event *pEvent);

#ifdef _WIN32

CRITICAL_SECTION HookMutex;

void ExecHook(Event *pEvent)
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   DWORD Err;
   char  CmdLine[1024];

   ZeroMemory( &si, sizeof(si) );
   si.cb = sizeof(si);
   ZeroMemory( &pi, sizeof(pi) );

   snprintf(CmdLine,sizeof(CmdLine),"%s %s",EventScript,pEvent->CmdTail);

   // Start the child process.
   if(!CreateProcess(NULL,    // module name
                     CmdLine, // Command line.
                     NULL,    // Process handle not inheritable.
                     NULL,    // Thread handle not inheritable.
                     FALSE,   // Set handle inheritance to FALSE.
                     0,       // No creation flags.
                     NULL,    // Use parent's environment block.
                     NULL,    // Use parent's starting directory.
                     &si,     // Pointer to STARTUPINFO structure.
                     &pi))    // Pointer to PROCESS_INFORMATION structure.
   {
      Err = GetLastError();
   }
   else {
   // Wait until child process exits.
      WaitForSingleObject(pi.hProcess,INFINITE);

      // Close process and thread handles.
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
   }
}

unsigned STDCALL Hooker(void *Dummy)
{
   Event *pEvent;

   TAKE_MUTEX(&HookMutex);
   while((pEvent = pEventHead) != NULL) {
      GIVE_MUTEX(&HookMutex);
      ExecHook(pEvent);
      TAKE_MUTEX(&HookMutex);
      pEventHead = pEvent->Link;
      EventProcessRunning--;
      free(pEvent);
   }
   GIVE_MUTEX(&HookMutex);

   _endthreadex(0);

   return 0;
}

void ExecHooks()
{
   HANDLE ThreadHandle;
   DWORD ThreadID;

   EventProcessRunning++;
   ThreadHandle = (HANDLE) _beginthreadex(NULL,STACKSIZE,Hooker,NULL,0,
                                          &ThreadID);

   if(ThreadHandle == NULL) {
      LOG_ERROR(("ExecHook(): CreateThread failed, Err = %u\n",errno));
   }
   else {
      CloseHandle(ThreadHandle);
   }
}

#else // ! Windoze

void EventHookCB(int Pid, int Status)
{
   Event *pEvent;

   EventProcessRunning--;
   pEvent = pEventHead;
   if(pEvent != NULL) {
      pEventHead = pEvent->Link;
      free(pEvent);
   }

   if(pEventHead != NULL) {
      ExecHook(pEventHead);
   }
}

void ExecHook(Event *pEvent)
{
   pid_t EventHookPid;
   char *cp, *cp1;
   #define MAX_ARGUMENTS   10
   char *argv[MAX_ARGUMENTS+2];
   int i = 0;

// NB: make sure all buffered streams are written to disk,
// otherwise the data will be written twice, once by the main
// process and once by the forked process !

   if(fflush(NULL)) {
      LOG_ERROR(("ExecHook(): fflush failed %s",Err2String(errno)));
   }

   D2PRINTF(("Forking event hook process\n"));
   if((EventHookPid = fork()) < 0) {
      LOG_ERROR(("ExecHook(): fork failed %s",Err2String(errno)));
   }
   else if(EventHookPid == 0) {
   // child
      argv[i++] = EventScript;
      cp = pEvent->CmdTail;

      if(strstr(cp,"chat ") == cp || strstr(cp,"sent_chat ") == cp) {
      // Special handling for chat text, pass all text in the second argument
         argv[i++] = cp;
         cp1 = strchr(cp,' ');
         while(*cp1 == ' ') {
            *cp1++ = 0;
         }
         argv[i++] = cp1;
      }
      else {
         while((cp1 = strchr(cp,' ')) != NULL) {
            if(i < MAX_ARGUMENTS) {
               argv[i++] = cp;
            }
            while(*cp1 == ' ') {
               *cp1++ = 0;
            }
            cp  = cp1;
         }

         argv[i++] = cp;
      }
      argv[i] = NULL;
      execvp(EventScript,argv);
   // NB: if execvp fails, BAIL!
      exit(1);
   }
   else {
   // Parent
      EventProcessRunning++;
      RegisterExitCB(EventHookPid,EventHookCB);
   }
}

void ExecHooks()
{
   if(pEventHead != NULL) {
      ExecHook(pEventHead);
   }
}
#endif   // !Windoze

void QueueEvent(Event *pEvent)
{
   static int bFirstCall = TRUE;

   if(bFirstCall) {
      bFirstCall = FALSE;
      InitializeCriticalSection(&HookMutex);
   }

   pEvent->Link = NULL;
   TAKE_MUTEX(&HookMutex);
   if(pEventHead == NULL) {
      pEventHead = pEvent;
   }
   else {
      pEventTail->Link = pEvent;
   }
   pEventTail = pEvent;

   if(pEventHead == pEvent) {
   // First event on queue, start it now
      GIVE_MUTEX(&HookMutex);
      ExecHooks();
   }
   else {
   // event queue
      GIVE_MUTEX(&HookMutex);
      if(EventProcessRunning > 1) {
         LOG_ERROR(("Queued event, EventProcessRunning: %d\n",
                    EventProcessRunning));
      }
   }
}

void EventHook(const char *fmt,...)
{
   va_list args;
   int   Wrote;
   char  Line[1024];
   Event *pEvent;
   
   va_start(args,fmt);
   Wrote = vsnprintf(Line,sizeof(Line),fmt,args);
   va_end(args);
   
   EndPointEvent(Line);

   if(EventScript != NULL) {
      pEvent = malloc(sizeof(Event) + strlen(Line) + 1);
      if(pEvent != NULL) {
         pEvent->CmdTail = (char *)(pEvent+1);
         strcpy(pEvent->CmdTail,Line);
         QueueEvent(pEvent);
      }
   }
}

