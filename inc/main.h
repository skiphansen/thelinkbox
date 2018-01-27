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

   $Log: main.h,v $
   Revision 1.7  2008/05/14 18:14:57  wb6ymh
   Changed BufPrintf fmt argument to const char * to fix compile problems
   with GCC 4.2.x.

   Revision 1.6  2008/02/26 18:04:32  wb6ymh
   Deleted DebugLevel.

   Revision 1.5  2007/11/26 06:58:13  wb6ymh
   Added extern "C" around prototypes when compiled for C++.

   Revision 1.4  2003/08/16 13:53:22  wb6ymh
   1. Deleted Terminator from ClientInfo, added RetCount.
   2. Added prototypes and variable for child exit status tracking.

   Revision 1.3  2003/04/30 22:06:41  wb6ymh
   Added global variable AppName.

   Revision 1.2  2002/12/18 00:51:37  wb6ymh
   Make bShutdown and bRefreshConfig public.
   Added StartRunning prototype.

   Revision 1.1.1.1  2002/10/06 17:00:22  wb6ymh
   Move common header files to inc subdirectory from src.

   Revision 1.5  2002/09/02 15:25:52  wb6ymh
   Added bDelete and bCurrent to ClientInfo.

   Revision 1.4  2002/08/31 23:51:51  wb6ymh
   Added bExceptionWait and bException to ClientInfo. Added SET_WAIT4_CONNECT().
   Added prototypes for DeltaTime2String(), TimeLapse2String() and
   TimeT2String().

   Revision 1.3  2002/08/18 16:34:42  wb6ymh
   Added prototype for DumpMe().

   Revision 1.2  2002/08/12 17:10:47  wb6ymh
   Made TimeNow a global.
   Removed argument to GetTimeNow().

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#ifndef MAIN_H_
#define MAIN_H_

typedef union {
   struct sockaddr s;
   struct sockaddr_in i;
   #define ADDR   i.sin_addr.s_addr
   #define PORT   i.sin_port
} IPAdrUnion;

// ClientInfo contains data on current TCP connections
typedef struct ClientInfo_TAG {
   int      SN;
   SOCKET   Socket;
   IPAdrUnion HisAdr;
   int   Err;
   int (*State)(struct ClientInfo_TAG *p);
   int (*NextState)(struct ClientInfo_TAG *p);
   int (*Complete)(struct ClientInfo_TAG *p);
   void (*Cleanup)(struct ClientInfo_TAG *p);
   char *Buf;        // Dynamically allocated Tx or Rx buffer (user)
   int   BufSize;    // Size of Buf
   int   Count;      // Amount of Buf Tx or Rx'ed
   int   RetCount;   // Amount of data returned from Buf
   struct timeval Timeout;
   int   x;    // scratch: various uses
   void *p;    // scratch: various uses

   // Flags
   int   bReadWait:1;
   int   bReadReady:1;

   int   bWriteWait:1;
   int   bWriteReady:1;

   int   bTimeWait:1;
   int   bTimeOut:1;

   int   bExceptWait:1;
   int   bException:1;

   int   bNew:1;

   int   bInClientTree:1;

   int   bDelete:1;
   int   bCurrent:1;
} ClientInfo;


#define SET_WAIT4_RD(x) \
   x->bReadWait = TRUE; \
   x->bWriteWait = FALSE
   
#define SET_WAIT4_WR(x) \
   x->bReadWait = FALSE; \
   x->bWriteWait = TRUE
   
#define SET_WAIT4_RW(x) \
   x->bReadWait = TRUE; \
   x->bWriteWait = TRUE


#define SET_WAIT4_CONNECT(x) \
   x->bReadWait = FALSE; \
   x->bExceptWait = TRUE; \
   x->bWriteWait = TRUE

typedef void (*ExitCB)(int Pid, int Status);

extern struct avl_table *ClientTree;
extern struct avl_table *UserTree;
extern struct avl_table *UserIPAdrTree;
extern int SN;
extern int bDaemon;
extern int bRunning;
extern time_t BootTime;
extern struct timeval TimeNow;
extern int OpenSockets;
extern int bShutdown;
extern int bRefreshConfig;
extern struct avl_table *ClientTree;
extern char *AppName;
extern int CurrentChildCount;

#ifdef __cplusplus
extern "C" {
#endif

int MainLoop(void);
int DeleteClient(ClientInfo *pClient);
int SendBuffer(ClientInfo *pClient);
ClientInfo *CreateNewClient(void);
void SetTimeout(ClientInfo *p,int Timeout);
void GetTimeNow();
int ClientCompare(const void *avl_a,const void *avl_b,void *avl_param);
void OpComplete(ClientInfo *pClient, int Err);
void Shutdown();
void BufPrintf(ClientInfo *p,const char *fmt,...);
void DumpMe();
char *DeltaTime2String(time_t StartTime, time_t EndTime, int bShort);
char *TimeLapse2String(time_t StartTime,int bShort);
char *TimeT2String(time_t time);
int StartRunning();
int RegisterExitCB(int Pid,ExitCB ExitCB);
int MainLoopInit();
char *Err2String(int err);
int ReadCount(ClientInfo *p);
int GetRxData(ClientInfo *p);

#ifdef __cplusplus
}
#endif


#endif
