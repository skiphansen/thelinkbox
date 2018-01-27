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

   $Log: dirclient.c,v $
   Revision 1.25  2008/09/14 17:04:51  wb6ymh
   Corrected a memory leak introduced in 0.84 that occured when an existing
   station's status was updated. ParseStationList now frees the previous
   copy of the users QTH before updating it.

   Revision 1.24  2008/02/26 17:38:15  wb6ymh
   1. Added dmalloc support.
   2. Modified ParseStationList to log messages received from the Echolink
      directory server.
   3. Added DirCleanup.

   Revision 1.23  2008/02/09 16:55:21  wb6ymh
   Added support for a new configuration variable ShowStatusInInfo.

   Revision 1.22  2007/06/29 18:16:16  wb6ymh
   Added QthInHostsFile configuration variable to allow Qth information to
   be optionally included in the host file.  NB: As of today the inclusion
   of Qth information breaks the EchoIRLP scripts.

   Revision 1.21  2007/06/27 19:26:25  wb6ymh
   Enhancements from Scott KI4LKF:
   1. Modified ParseStationList to save the user's QTH in UserInfo structure.
   2. Modified CleanUserList to save node QTH in host file.

   Revision 1.20  2007/04/27 14:29:02  wb6ymh
   Added FileUserByNodeID.

   Revision 1.19  2006/08/05 23:30:04  wb6ymh
   Corrected warnings and errors with GCC 4.x.x.

   Revision 1.18  2004/11/29 00:31:31  wb6ymh
   1. Added (off line) to comment to mark inactive stations in host file.
   2. Corrected number of stations listed in debug message for a full station
      list download.

   Revision 1.17  2004/11/27 22:09:26  wb6ymh
   1. Modified version reporting to use bEchoIrlpMode flag.
   2. Changed EOL convention back to Unix sytle(!).

   Revision 1.16  2004/11/22 00:52:56  wb6ymh
   1. Detect EchoIRLP by SF_Port != SF_ReplyPort instead of SF_ReplyPort !=
      SF_AUDIO_PORT.  Allows conferences to use nonstandard ports without
      triggering an EchoIRLP version string.
   2. Removed broken, buggy ClientIPAdrUpdate stuff.  If you ISP forces you to
      change your IP address all of the time, CHANGE YOUR ISP !
   3. Added support for the IncludeAllHosts configuration variable.

   Revision 1.15  2004/05/29 17:25:22  wb6ymh
   1. Added code to update the IP address of persistent clients when the client's
      IP address changes in the directory.

   2. Corrected memory leak created when compressed directory support was added.

   3. Changed version string sent to EchoLink directory servers from v.vvB to
      v.vvI when tbd is used with the EchoIRLP project.

   4. Eliminated unused and unneeded fields from the UserInfo structure to reduce
      memory requirements.

   Revision 1.14  2003/08/31 23:21:03  wb6ymh
   1. Added support for compressed and differential station lists.
   2. Corrected portability problems that occured when u_long was assumed to
      be the same size as an IP address.
   3. Added code to clear sockaddr_in before attempting to bind. If the structure
      is not cleared bind will fail on FreeBSD from time to time.
   4. Corrected a bug that prevented thebridge from operating properly on NT 4.0.
      Thanks to K1RFD for the fix and N4LED for reporting the bug and testing
      the fix.
   5. Added the EmailAdr to information provided to directory server on login.
   6. Added calls to DoConfigPass() at end of first station list download and
      second successful directory server login.

   Revision 1.13  2003/04/30 21:52:58  wb6ymh
   1. Added support for Bind2IP configuration variable to ServerRequest().
   2. Added code to cause an exit if the directory server returns "NO" to login
      attempt.

   Revision 1.12  2003/01/05 17:05:02  wb6ymh
   1. Report real version number to directory server on login instead of
      old iLink client string.
   2. Changed logout string from "OFF-" to "OFF-V".
   3. Modified CheckCallResp() to add authorized clients only, unauthorized
      clients are simply ignored. (This change was lost before the last
      release and was not actually present in 0.45).

   Revision 1.11  2003/01/01 19:12:40  wb6ymh
   1.Added 30 second timeout to all directory server requests.
   2.Corrected infinite loop in PullerLoginAck which occurred when read()
     returned an error.
   3.Modified CheckCallResp() to add authorized clients only, unauthorized
     clients are simply ignored.

   Revision 1.10  2002/12/21 18:14:09  wb6ymh
   Added code to generate *nix style host file containing active EchoLink
   users.

   Revision 1.9  2002/12/18 00:59:51  wb6ymh
   Removed extra call to DescrambleIP(), added newlines to several log messages.

   Revision 1.8  2002/11/02 18:23:21  wb6ymh
   1. Added ValidateCallsign() routine to check if a  specified user is logged
      in and has a vaild callsign.
   2. Modified to call new caching GetHostByName() instead of library function
      gethostbyname().
   3. Modified code to list conference in directory as "Private conference"
      when the PrivateConference flag is set.
   4. Modified CleanUserList() to keep entries for stations that have logged
      out for InactiveDirTimeout seconds. This reduces the number of times we
      need to call ValidateCallsign() when a user connects right after starting
      EchoLink.
   5. Added code to log DeScrambleIP() failures.

   Revision 1.7  2002/10/25 14:28:52  wb6ymh
   Added support for iLink directory servers.

   Revision 1.6  2002/09/14 17:09:25  wb6ymh
   Added code to save the conference's node id in OurNodeID in *Network* order.
   Corrected bug in the message logged when an attempt to resolve the directory
   server's IP address failed.

   Revision 1.5  2002/09/02 15:24:36  wb6ymh
   Reworked (yet again) recovery from hung server request by clearing 
   bInClientTree before calling DeleteClient().  Moved code that starts second 
   half of two step requests into PullerCleanup(), because DeleteClient() no 
   longer allows the current client to be deleted until the mainloop is done 
   with it.

   Revision 1.4  2002/09/01 15:52:51  wb6ymh
   Modified ServerRequest() to avl_replace() new clients rather than avl_insert(),
   avl_delete, avl_insert().  Deleting clients while the tree is being
   transversed is potential death!  (It's ok to delete the current client
   because the mainloop has already saved the index of the next client,
   but if you delete what would be the next client, the mainloop will
   try to service him anyway leading to a crash)

   Revision 1.3  2002/09/01 00:05:42  wb6ymh
   1. Added support for multiple directory servers.
   2. Modified all log entries to use %m for error message display when available.
   3. Added code to increment request/success/failure counters in ServerStats.
   4. Added code to track number of open sockets.
   5. Modified code to delete any zombie clients that may have been created by
      earlier calls to ServerRequest().
   6. Modified code to wake on socket exception in addition to write. Windows
      sets an exception when a connect() fails, Unix sets write.  This was
      causing the Windows version to fail to detect failed connection attempts
      which caused both memory and socket handles leaks.  Eventually this lead to
      a crash after about 48 hours of operation.

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
   #include <sys/types.h>
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
   #include <unistd.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <arpa/inet.h>
   #include <errno.h>
   #include <string.h>
   #include <ctype.h>
#ifdef HAVE_FCNTL_H
   #include <fcntl.h>
#endif
   #include <netdb.h>
#else
   // Windoze
   #include <stdio.h>
   #include <time.h>
   #include <io.h>
   #include <winsock2.h>
   #include <ctype.h>
#endif

#include "avl.h"
#include "main.h"
#include "configvars.h"
#include "ilink.h"
#include "users.h"
#include "conference.h"
#include "dirclient.h"
#include "hostfile.h"
#include "eventhook.h"
#include "zlib.h"
#include "sf.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define  ECHOLINK_HOSTS_FILE     "hosts"
#define  ECHOLINK_HOSTS_TMP_FILE "hosts.temp"

#define  SERVER_REQ_TO           30000 /* 30 seconds (in Milliseconds) */

int DuplicateClientsDeleted = 0;
int PullerStatusBusy = 0;
ServerStats_t ServerStats[NUM_DIRECTORY_SERVERS];
unsigned long OurNodeID = 0;
int ActiveDirEntries = 0;
int InactiveDirEntries = 0;
char  SnapShortID[MAX_SNAPSHOT_LEN] = {'0',0};

typedef struct {
   SERVER_REQUEST Req;
   char  Callsign[MAX_CALL_LEN+1];
   char  Qth[MAX_QTH_LEN+1];
   int   NodeID;
   int   LocalHr;
   int   LocalMin;
   int   NewStations;
   int   DeletedStations;
   int   UpDatedStations;
   int   Server;
      
   void *Arg;
   z_stream    c_stream;
   char  *ZBuf;      // Decompressed data buffer
   int   ZCount;     // Amount of data in ZCount
   int   ZRetCount;  // Amount of data consumed from ZBuf
   int   ZBufSize;   // allocation size of ZBuf
   int   StationCount;

   int   bIgnore:1;     
   int   bBusy:1;
   int   bPreambleRead:1;
   int   bCompressed:1;
   int   bUnCompressed:1;
   int   bAbort:1;
   int   bComplete:1;
   int   bCompressedEOF:1;
   int   bDifferential:1;
   int   bStationListEndFound:1;
} PullerInfo;

PullerInfo *pPullerDebug = NULL;

char *Req2String[] = {
   "?",           // SERV_REQ_NONE,
   "Login",       // SERV_REQ_LOGIN,
   "Logout",      // SERV_REQ_LOGOUT,
   "Login&List",  // SERV_REQ_LOGIN_AND_LIST
   "StationList", // SERV_REQ_STATION_LIST
   "Shutdown",    // SERV_REQ_SHUTDOWN
   "CheckCall"    // SERV_REQ_CHECK_CALL
};

int PullerLogin(ClientInfo *pClient);
int PullerLoginAck(ClientInfo *pClient);
int PullerConnected(ClientInfo *pClient);
int GetStationList(ClientInfo *pClient);
void ParseStationList(ClientInfo *pClient,char *Buf,int Count);
void CleanUserList(int bDifferential);
int CheckCall(ClientInfo *pClient);
int PullerHandler(ClientInfo *pPuller);
void PullerCleanup(ClientInfo *pPuller);
int ServerRequestComplete(ClientInfo *p);
void DeScrambleIP(char *IO);
void RightTrim(char *Buf);
int ReadStationData(ClientInfo *p);

void ValidateCallsign(char *Callsign,IPAdrUnion *pIP)
{
   char  Temp[80];

   snprintf(Temp,sizeof(Temp),"%s\r%s\r",Callsign,inet_ntoa(pIP->i.sin_addr));
   ServerRequest(SERV_REQ_CHECK_CALL,0,strdup(Temp));
}

void DeScrambleIP(char *IO);

void ServerRequest(SERVER_REQUEST Req,int Server,void *Arg)
{
   int i;
   ClientInfo *pClient;
   ClientInfo *pClient1;
   PullerInfo *pPuller;
   PullerInfo *pPuller1;
   struct hostent *pTemp;
   IPAdrUnion MyAdr;

   ServerStats[Server].Requests++;
   pClient = CreateNewClient();
   if(pClient != NULL) {
      pClient->Complete = ServerRequestComplete;
      pPuller = malloc(sizeof(PullerInfo));
      if(pPuller != NULL) {
         pClient->p = pPuller;
         pClient->Cleanup = PullerCleanup;
         memset(pPuller,0,sizeof(PullerInfo));
         pPuller->Server = Server;
         pPuller->Req = Req;
         pPuller->Arg = Arg;

         pClient->Socket = socket(AF_INET,SOCK_STREAM,0);
         if(pClient->Socket == SOCKET_ERROR) {
            LOG_ERROR(("ServerRequest(): Socket() failed %s",
                       Err2String(ERROR_CODE)));
            DeleteClient(pClient);
            pClient = NULL;
         }
         else {
            OpenSockets++;
         }
      }
   }
      
   if(pClient != NULL && Bind2IP != NULL) {
      memset(&MyAdr,0,sizeof(MyAdr));
      MyAdr.i.sin_family = AF_INET;
      MyAdr.PORT = 0;      // Any port
      MyAdr.ADDR = inet_addr(Bind2IP);
      if(MyAdr.ADDR == INADDR_NONE) {
         LOG_ERROR(("ServerRequest(): failed to convert \"%s\" to IP address.\n",
                    Bind2IP));
         DeleteClient(pClient);
         pClient = NULL;
      }
      else if(bind(pClient->Socket,&MyAdr.s,sizeof(MyAdr)) == SOCKET_ERROR) {
         LOG_ERROR(("ServerRequest(): bind() failed, %s",
                    Err2String(ERROR_CODE)));
         DeleteClient(pClient);
         pClient = NULL;
      }
   }

   if(pClient != NULL) {
   // Set the socket to non-blocking
   #ifdef _WIN32
      i = 1;
      ioctlsocket(pClient->Socket,FIONBIO,&i);
   #else
      fcntl(pClient->Socket,F_SETFL,O_NONBLOCK);
   #endif
      
      pClient->HisAdr.i.sin_family = AF_INET;
      pClient->HisAdr.i.sin_port = htons(ILINK_DIRSERVER_PORT);
   
      pTemp = GetHostByName(DirServerHost[Server]);
      if(pTemp != NULL) {
         pClient->HisAdr.i.sin_addr.s_addr = IP_FROM_HOSTENT(pTemp,0);
      }
      else {
      // That didn't work.  On some stacks a numeric IP address
      // will not parse with gethostbyname.  Try to convert it as a
      // numeric address
         pClient->HisAdr.i.sin_addr.s_addr = inet_addr(DirServerHost[Server]);
      }
   
      if(pClient->HisAdr.i.sin_addr.s_addr == INADDR_NONE) {
         LOG_ERROR(("ServerRequest(): Couldn't convert \"%s\" to an IP address.\n",
                  DirServerHost[Server]));
         DeleteClient(pClient);
         pClient = NULL;
      }
      else {
         if((pClient1 = avl_replace(ClientTree,pClient)) != NULL) {
            pClient1->bInClientTree = FALSE;
            DuplicateClientsDeleted++;
            pPuller1 = (PullerInfo *) pClient1->p;
            if(pPuller1 != NULL) {
               ServerStats[pPuller1->Server].Failure++;
            }
            pClient1->Err = ERR_SERVER_TIMEOUT;
            LOG_ERROR(("ServerRequest(): Duplicate client removed from tree.\n"));
            DeleteClient(pClient1);
         }
         pClient->bInClientTree = TRUE;
      }
   }

   if(pClient != NULL) {
      pClient->State = PullerConnected;
      pClient->NextState = PullerLogin;

      i = connect(pClient->Socket,&pClient->HisAdr.s,sizeof(pClient->HisAdr));

      if(i == SOCKET_ERROR) {
         if(ERROR_CODE == ERROR_IN_PROGRESS) {
         // The expected result, connect takes a while
            D2PRINTF(("ServerRequest(), connect() returned ERROR_IN_PROGRESS.\n"));
         // Wait for up to SERVER_REQ_TO millseconds for the connection
            SetTimeout(pClient,SERVER_REQ_TO); 
            SET_WAIT4_CONNECT(pClient);
         }
         else {
         // Some kind of error other than in progress
            LOG_ERROR(("ServerRequest(), connect() failed, %s",
                       Err2String(ERROR_CODE)));
            OpComplete(pClient,ERROR_CODE);
         }
      }
      else {
      // That certainly was quick !
         LOG_WARN(("ServerRequest(), connect() didn't return an error(!).\n"));
         PullerConnected(pClient);
      }
   }
}

UserInfo *FindUserByNodeID(int NodeID)
{
   struct avl_traverser avl_trans;
   UserInfo *pUser;

   pUser = (UserInfo *) avl_t_first(&avl_trans,UserTree);
   while(pUser != NULL) {
      if(pUser->NodeID == NodeID) {
         break;
      }
      pUser = (UserInfo *) avl_t_next(&avl_trans);
   }

   return pUser;
}

int PullerConnected(ClientInfo *pClient)
{
   int i;
   int Err = 0;
   PullerInfo *pPuller = (PullerInfo *) pClient->p;
   int Ret = FALSE;

   if(pClient->x == 0 && pClient->bTimeOut) {
      i = 1;
      Err = ERR_SERVER_TIMEOUT;
      LOG_WARN(("PullerConnected(): connect() timed out for client %u\n",
                pClient->SN));
   }
   else {
      i = connect(pClient->Socket,&pClient->HisAdr.s,sizeof(pClient->HisAdr));

      if(i != 0 && (Err = ERROR_CODE) != ERROR_CONNECTED)
      // Connection failed.
#ifndef _WIN32
      {
         LOG_WARN(("PullerConnected(): connect() failed for client %u %s",
                   pClient->SN,Err2String(Err)));
      }
   }

#else
      {
      // Heavy sigh...  
      // Apparently there are bugs in Microsoft's TCP/IP stacks (shock !)
      // that causes connect() to return an error other than WSAEISCONN even
      // after select() has indicated that the socket is ready for write.
      // Try it a few times before giving up.

         if(pClient->x > 0) {
         // The first attempt almost always fails, don't fill the log with junk
            LOG_WARN(("PullerConnected(): connect() failed for client %u, "
                      "try %d, %s",pClient->SN,pClient->x+1,Err2String(Err)));
         }

         if(pClient->x++ < 10) {
         // Wait 20 milliseconds and try again
            pClient->bReadWait = FALSE;
            pClient->bWriteWait = FALSE;
            SetTimeout(pClient,20);
         }
         else {
            pClient->x = 0;
         }
      }
      else {
         if(pClient->x > 1) {
         // The second attempt almost always works, don't fill the log with junk
            LOG_WARN(("PullerConnected(): connect() succeeded for client %u, "
                      "try %d\n",pClient->SN,pClient->x+1));
         }
         pClient->x = 0;
      }
   }

   if(pClient->x == 0)
   // Only handle sucess/failure if not a retry
#endif

// BSD returns an error with an error code of EISCONN here.  That agrees
// with the man page description of connect().
//
// Linux does not return an error.  The Linux man page (at least Red Hat's) 
// doesn't include any details on the expected behavour of connect when used
// with non-blocking sockets and select as we are doing here.
//
   
   if(i != 0 && Err != ERROR_CONNECTED) {
   // Connection failed.
      OpComplete(pClient,Err);
   }
   else {
      switch(pPuller->Req) {
         case SERV_REQ_LOGIN:
         case SERV_REQ_LOGOUT:
         case SERV_REQ_SHUTDOWN:
         case SERV_REQ_LOGIN_AND_LIST:
         // Login into the server
            Ret = PullerLogin(pClient);
            break;

         case SERV_REQ_STATION_LIST:
         // Refresh server list
            Ret = GetStationList(pClient);
            break;

         case SERV_REQ_CHECK_CALL:
            Ret = CheckCall(pClient);
            break;

         default:
            LOG_ERROR(("PullerConnected(), unexpected request %d!\n",
                     pPuller->Req));
            DeleteClient(pClient);
            break;
      }
   }

   return Ret;
}

int PullerLogin(ClientInfo *pClient)
{
   int WriteSize;
   char c = 'l';
   time_t timenow = (time_t) TimeNow.tv_sec;
   struct tm *tm = localtime(&timenow);
   int Ret = 0; 
   PullerInfo *pPuller = (PullerInfo *) pClient->p;
   char *Status;
   char Qth[MAX_QTH_LEN+1];
   
// no timeout on write, we'll get an error if it fails
   pClient->bTimeWait = FALSE;   
   pClient->BufSize = 256;
   pClient->Buf = (char *) malloc(pClient->BufSize);

   if((WRITE(pClient->Socket,&c,1) != 1)) {
   // This isn't good.  It's the first thing we've sent.
   // The buffer can't be full yet !
      LOG_WARN(("PullerLogin(): write() failed for client %u, %s",
               pClient->SN,Err2String(ERROR_CODE)));
      OpComplete(pClient,ERROR_CODE);
   }
   else {
      switch(pPuller->Req) {
         default:
            LOG_ERROR(("PullerLogin(): Unknown server request\n"));

         case SERV_REQ_LOGIN:
         case SERV_REQ_LOGIN_AND_LIST:
            Status = PullerStatusBusy ? "BUSY" : "ONLINE";
            break;

         case SERV_REQ_SHUTDOWN:
         case SERV_REQ_LOGOUT:
            Status = "OFF-V";
            break;
      }

      Qth[0] = 0;
      if(ShowStatusInInfo) {
         UpdateConnectedStatus();
         if(ConnectedStatus[0] != 0) {
         // We have connect status to display
            snprintf(Qth,sizeof(Qth)-1,"%s",ConnectedStatus);
         }
      }
      
      if(Qth[0] == 0) {
      // Qth not set yet
         snprintf(Qth,sizeof(Qth)-1,"%s",
                  PrivateConference ? "Private conference" : ConferenceQth);
         Qth[MAX_QTH_LEN] = 0;

         if(UserCountInLocation) {
            char *UserCount = GetUserCountString();
            if(strlen(Qth) + strlen(UserCount) <= MAX_QTH_LEN) {
               strcat(Qth,UserCount);
            }
            else {
               strcpy(&Qth[MAX_QTH_LEN - strlen(UserCount)],UserCount);
            }
         }
      }
      Qth[MAX_QTH_LEN] = 0;

      WriteSize = snprintf(pClient->Buf,pClient->BufSize,
                           "%s%c%c%s\r%s" VERSION "%c(%d:%2d)\r%s\r%s\r",
                           ConferenceCall,0xac,iLinkDirServer ? 0x3d : 0xac,
                           ConferencePass,Status,
                           bEchoIrlpMode ? 'I' : 'B',
                           tm->tm_hour,tm->tm_mday,Qth,
                           EmailAdr == NULL ? "" : EmailAdr);

      if(WriteSize == -1 || WriteSize >= pClient->BufSize) {
         LOG_ERROR(("PullerLogin(): Temp buffer too small !\n"));
         ServerStats[pPuller->Server].Failure++;
         DeleteClient(pClient);
      }
      else {
         pClient->BufSize = WriteSize;
         pClient->Count = 0;
         pClient->State = SendBuffer;
         pClient->NextState = PullerLoginAck;
         Ret = 1;
      }
   }

   return Ret;
}

int PullerLoginAck(ClientInfo *pClient)
{
   int  BytesRead;
   char Data;
   PullerInfo *pPuller = (PullerInfo *) pClient->p;
   int Server = pPuller->Server;

   if(pClient->BufSize > 2) {
   // We've sent our info, now wait for the response from the server
      pClient->BufSize = 2;
      pClient->Count = 0;
   }
   else if(pClient->bTimeOut) {
      LOG_WARN(("PullerLoginAck(), server timeout\n"));
      OpComplete(pClient,ERR_SERVER_TIMEOUT);
      pClient->Count = 3;
   }

   while(pClient->Count < 2) {
      if((BytesRead = READ(pClient->Socket,&Data,1)) == 1) {
            pClient->Buf[pClient->Count++] = Data;
      }
      else if(BytesRead == 0) {
      // Connection has closed
         LOG_WARN(("PullerLoginAck(), server closed connection.\n"));
         OpComplete(pClient,ERR_CONNECTION_CLOSED);
         break;
      }
      else if(ERROR_CODE == ERROR_WOULD_BLOCK) {
      // Nothing available to read now, keep waiting
         SET_WAIT4_RD(pClient);
      // Wait for up to SERVER_REQ_TO millseconds for the response
         SetTimeout(pClient,SERVER_REQ_TO); 
         break;
      }
      else {
      // Some kind of error other than no data available
         LOG_WARN(("PullerLoginAck(), READ() failed, %s",
                   Err2String(ERROR_CODE)));
         OpComplete(pClient,ERROR_CODE);
         break;
      }
   }

   if(pClient->Count == 2) {
   // If we succeeded in loging in our buffer should contain "ok"
      pClient->Buf[2] = 0;
      if(strcmp(pClient->Buf,"OK") == 0) {
         DPRINTF(("PullerLoginAck(): Client %u successfully updated status.\n",
                  pClient->SN));

         if(ConfigPass == 3) {
         // Do another yet another pass on the configuration file now that 
         // we've downloaded a station list and have logged in twice.  
         // This gives time for our IP address to propagate between directory 
         // servers to ensure that connect commands embedded in the 
         // configuration file are not refused because our IP address isn't 
         // listed in the directory server yet...
            DoConfigPass(4);
         }

         switch(pPuller->Req) {
            case SERV_REQ_LOGIN:
            // We're done
               OpComplete(pClient,0);
               break;

            case SERV_REQ_LOGIN_AND_LIST:
            // Now list the stations
               ServerStats[Server].Success++;
               DeleteClient(pClient);
               break;

            case SERV_REQ_SHUTDOWN:
               LOG_NORM(("Logged out, exiting.\n"));
               DeleteClient(pClient);
               bRunning = FALSE;
               break;

            default:
               LOG_ERROR(("PullerLoginAck(), unexpected request %d!\n",
                        pPuller->Req));
               DeleteClient(pClient);
               break;
         }
      }
      else if(strcmp(pClient->Buf,"NO") == 0) {
         LOG_WARN(("PullerLoginAck(): login server rejected login attempt for "
                  "client %u, exiting.\n",pClient->SN));
         bRunning = FALSE;
         OpComplete(pClient,ERR_SERVER_ERR);
      }
      else {
         LOG_WARN(("PullerLoginAck(): login error server returned \"%s\" for"
                  "client %u.\n",pClient->Buf,pClient->SN));
         OpComplete(pClient,ERR_SERVER_ERR);
      }
   }

   return 0;
}

int GetStationList(ClientInfo *pClient)
{
   char Request[12];
   int WriteLen;
   PullerInfo *pPuller = (PullerInfo *) pClient->p;
   
   pPuller->Req = SERV_REQ_STATION_LIST;
   pClient->BufSize = 256;
   pClient->Buf = (char *) malloc(pClient->BufSize);
   pClient->State = ReadStationData;
   SET_WAIT4_RD(pClient);

// Wait for up to SERVER_REQ_TO millseconds for data
   SetTimeout(pClient,SERVER_REQ_TO); 

   if(DirCompression) {
   // Ask for a compressed, differential station list
      snprintf(Request,sizeof(Request),"F%s\r",SnapShortID);
   }
   else if(iLinkDirServer) {
      strcpy(Request,"S");
   }
   else {
      strcpy(Request,"s");
   }

   WriteLen = strlen(Request);

   if((WRITE(pClient->Socket,Request,WriteLen) != WriteLen)) {
   // This isn't good.  It's the first thing we've sent.
   // The buffer can't be full yet !
      LOG_WARN(("GetStationList(): write() failed, %s",Err2String(ERROR_CODE)));
      DeleteClient(pClient);
   }

   return 0;
}

// Read station list from the directory server
int ReadStationData(ClientInfo *p)
{
   char *cp;
   PullerInfo *pPull = (PullerInfo *) p->p;
   int Err;

   while(p->Err == 0 && !pPull->bAbort) {
      if(p->bTimeOut) {
         p->Err = ERR_SERVER_TIMEOUT;
         LOG_ERROR(("ReadStationData(): server time out client %u.\n",p->SN));
         break;
      }

      if(!pPull->bPreambleRead) {
      // looking for first 4 bytes to determine if stream is compressed or not
         if(p->Count < 4) {
            if(!GetRxData(p)) {
               break;
            }
         }
         else {
            ParseStationList(p,&p->Buf[p->RetCount],4);
         }
      }
      else if(pPull->bCompressed) {
         if(pPull->ZRetCount > 0 && pPull->ZCount > 0) {
         // move remaining data in buffer to start
            memmove(pPull->ZBuf,&pPull->ZBuf[pPull->ZRetCount],pPull->ZCount+1);
         }
         pPull->ZRetCount = 0;
         pPull->c_stream.next_out = (Bytef *) &pPull->ZBuf[pPull->ZCount];
         pPull->c_stream.avail_out = pPull->ZBufSize - pPull->ZCount - 1;

         if(!pPull->bCompressedEOF) {
            if(pPull->c_stream.avail_in == 0) {
            // need more data to decompress
               if(p->Count == 0 && !GetRxData(p)) {
                  break;
               }
               pPull->c_stream.next_in = (Bytef *) &p->Buf[p->RetCount];
               pPull->c_stream.avail_in = p->Count;
               p->Count = 0;
               p->RetCount = 0;
            }

            if(pPull->c_stream.avail_in > 0) {
               if(pPull->c_stream.avail_out == 0) {
                  LOG_ERROR(("ReadStationData(): Zbuf overflow for client "
                             "%u.\n",p->SN));
                  p->Err = ERR_DECOMPRESS_ERR;
                  break;
               }

               if((Err = inflate(&pPull->c_stream,Z_NO_FLUSH)) != Z_OK) {
                  if(Err == Z_STREAM_END) {
                     pPull->bCompressedEOF = TRUE;
                  }
                  else {
                     LOG_ERROR(("ReadStationData(): inflate returned 0x%x for "
                                "client %u.\n",Err,p->SN));
                     p->Err = ERR_DECOMPRESS_ERR;
                     break;
                  }
               }
               pPull->ZCount = (char *) pPull->c_stream.next_out - pPull->ZBuf;
               pPull->ZBuf[pPull->ZCount] = 0;
            }
         }
         else if(pPull->ZCount > 0) {
         // Consume the remaining data
            p->Err = ERR_CONNECTION_CLOSED;
            ParseStationList(p,&pPull->ZBuf[pPull->ZRetCount],pPull->ZCount);
            break;
         }
         else {
            LOG_ERROR(("ReadStationData(): bCompressedEOF detected before end "
                       "of station data detected for client %u.\n",p->SN));
            p->Err = ERR_DECOMPRESS_ERR;
            break;
         }

         while(pPull->ZCount > 0 && 
            (cp = strchr((char *) &pPull->ZBuf[pPull->ZRetCount],'\n')) != NULL)
         {  // We have a complete line
            int   Count;

            Count = (cp - &pPull->ZBuf[pPull->ZRetCount]) + 1;
            *cp = 0; // remove the line feed
            ParseStationList(p,&pPull->ZBuf[pPull->ZRetCount],Count-1);
            pPull->ZCount -= Count;
            pPull->ZRetCount += Count;
            if(pPull->bAbort) {
               break;
            }
         }
      }
      else {   // Not compressed
         while(p->Count > 0 && (cp = strchr(&p->Buf[p->RetCount],'\n')) != NULL)
         {  // We have a complete line
            int   Count;

            Count = (cp - &p->Buf[p->RetCount]) + 1;
            *cp = 0; // remove the line feed
            ParseStationList(p,&p->Buf[p->RetCount],Count-1);
            p->Count -= Count;
            p->RetCount += Count;
            if(pPull->bAbort) {
               break;
            }
         }

         if(pPull->bAbort || !GetRxData(p)) {
            break;
         }

         if(p->Err != 0 && p->Count > 0) {
         // Consume the remaining data
            ParseStationList(p,&p->Buf[p->RetCount],p->Count);
         }
      }
   }
   
   if(p->Err != 0 || pPull->bAbort) {
      if(pPull->bComplete) {
         if(pPull->bDifferential) {
            DPRINTF(("Differental station list completed, %d added, %d deleted,"
                     " %d updated.\n",pPull->NewStations,
                     pPull->DeletedStations,pPull->UpDatedStations));
         }
         else {
            DPRINTF(("Full station list downloaded successfully, "
                     "%d stations listed.\n",pPull->NewStations));
         }
         OpComplete(p,0);
         if(ConfigPass == 2) {
         // Do another pass on the configuration file now that we've downloaded
         // a valid station list
            DoConfigPass(3);
         }
      }
      else {
         OpComplete(p,p->Err);
      }
   }

   return 0;
}

// Parse a line of text from the directory server.
// Return true when complete
void ParseStationList(ClientInfo *pClient,char *Buf,int Count)
{
   PullerInfo *pPuller = (PullerInfo *) pClient->p;
   UserInfo *pUser;
   UserInfo UserLookup;
   char *cp;
   int i;
   int DataLen = 0;
   int bNewUser = FALSE;
   int bNewIPAdr = FALSE;
   int State = pClient->x;
   int Err;
   int bEmptyField = FALSE;

   if(pPuller->bDifferential && Count == 1 && *Buf == '.') {
      bEmptyField = TRUE;
   }

   if(Count == sizeof(EndOfData)-1 && 
      strncmp(Buf,EndOfData,sizeof(EndOfData)-1) == 0)
   {  // End of data marker
      if(pPuller->bDifferential) {
         if(pPuller->bStationListEndFound) {
            pPuller->bComplete = TRUE;
            pPuller->bAbort = TRUE;
            CleanUserList(TRUE);
         }
         pPuller->bStationListEndFound = TRUE;
         State = 6;
         if(pPuller->StationCount != 0) {
            LOG_ERROR(("ParseStationList(): Warning: deleted station marker"
                       "found @ StationCount %d.\n",pPuller->StationCount));
         }
      }
      else {
         CleanUserList(FALSE);
         pPuller->bComplete = TRUE;
         pPuller->bAbort = TRUE;
      }
   }
   else if(pPuller->bIgnore) {
   // ignoring a station entry
      if(State++ == 5) {
         pPuller->bIgnore = FALSE;
      // Back to expecting a callsign
         State = 2;
      }
   }
   else switch(State) {
      case 0:  // First 4 bytes of data are available
         if(strncmp(Buf,StartOfData,sizeof(StartOfData)-1) == 0) {
         // Uncompressed station list
            State++;
            DataLen = sizeof(StartOfData);
         }
         else if(strncmp(Buf,DiffData,sizeof(DiffData)-1) == 0) {
         // Differential station list
            State++;
            pPuller->bDifferential= TRUE;
            DataLen = sizeof(DiffData)-1;
         }
         else if(!pPuller->bCompressed) {
         // not start of data mark, check to see if it's a length
            DataLen = 0;
            for(i = 3; i >= 0; i--) {
               DataLen <<= 8;
               DataLen += (unsigned char) Buf[i];
            }

            // Assume the station list will be < 1 megabyte
            if(DataLen < MAX_STATION_LIST_SIZE) {
            // Assume it's a compressed station list
               pPuller->bCompressed = TRUE;
               DataLen = 4;

               if((Err = inflateInit(&pPuller->c_stream)) != Z_OK) {
                  LOG_ERROR(("ParseStationList(): inflateInit returned 0x%x\n",
                             Err));
                  pClient->Err = ERR_DECOMPRESS_ERR;
                  pPuller->bAbort = TRUE;
               }
               else {
                  pPuller->ZBufSize = pClient->BufSize * 4;
                  pPuller->ZBuf = malloc(pPuller->ZBufSize);
                  if(pPuller->ZBuf == NULL) {
                     LOG_ERROR(("ParseStationList(): malloc failed.\n"));
                     pClient->Err = ERR_MALLOC;
                     pPuller->bAbort = TRUE;
                  }
               }
            }
            else {
               pPuller->bAbort = TRUE;
               LOG_ERROR(("ParseStationList(): Expected StartOfData got "
                          "\"%s\".\n",Buf));
            }
         }
         else {
            pPuller->bAbort = TRUE;
            LOG_ERROR(("ParseStationList(): Expected StartOfData got "
                       "\"%s\".\n",Buf));
         }

         if(!pPuller->bPreambleRead) {
         // Consume Preamble
            pPuller->bPreambleRead = TRUE;
            pClient->RetCount += DataLen;
            pClient->Count -= DataLen;
         }
         break;

      case 1:  // parse record count and snapshot ID
         sscanf(Buf,"%d",&pPuller->StationCount);
         if((cp = strchr(Buf,':')) != NULL) {
         // We have a snapshot ID, save it.
            snprintf(SnapShortID,sizeof(SnapShortID),"%s",cp+1);
         }
         State++;
         break;

      case 2:  // Callsign
         State++;
         if(Count > MAX_CALL_LEN) {
            LOG_ERROR(("ParseStationList(): Callsign \"%s\" is too long.\n",
                     Buf));
            pPuller->bIgnore = TRUE;
         }
         else {
            strcpy(pPuller->Callsign,Buf);
         }
         break;

      case 3:  // Qth / status
         State++;
         if(bEmptyField) {
            pPuller->Qth[0] = 0;
            break;
         }
         
         if((cp = strrchr(Buf,'[')) != NULL) {
            *cp++ = 0;
            i = sscanf(cp,"%*s %d:%d",&pPuller->LocalHr,&pPuller->LocalMin);
            if(i != 2) {
               pPuller->LocalHr = -1;
            }

            if(strncmp(cp,"BUSY",4) == 0) {
               pPuller->bBusy = TRUE;
            }
            else if(strncmp(cp,"ON",2) == 0) {
               pPuller->bBusy = FALSE;
            }
            else {
            // Invalid entry, unknown state
               pPuller->bIgnore = TRUE;
            }
         }
         else {
         // Invalid entry, no status
            pPuller->bIgnore = TRUE;
         }

         if(pPuller->bIgnore) {
            int x = pPuller->NewStations + pPuller->DeletedStations + 
                    pPuller->UpDatedStations;
            RightTrim(pPuller->Callsign);
            RightTrim(Buf);
            if(pPuller->Callsign[0] == 0 && (DebugLevel > 0 || x == 0)) {
            // Message from EchoLink Directory server, log it
               LOG_ERROR(("Msg from EchoLink: %s\n",Buf));
            }
            break;
         }

         if(strlen(Buf) > MAX_QTH_LEN) {
            LOG_ERROR(("ParseStationList(): Qth \"%s\" is too long.\n",
                     Buf));
            pPuller->bIgnore = TRUE;
         }
         else {
            strcpy(pPuller->Qth,Buf);
         }
         break;

      case 4:  // NodeID
         State++;
         if(bEmptyField) {
            pPuller->NodeID = 0;
         }
         else if(sscanf(Buf,"%d",&pPuller->NodeID) != 1) {
            LOG_ERROR(("ParseStationList(): can't convert NodeID \"%s\".\n",
                     Buf));
            pPuller->bIgnore = TRUE;
         }
         break;

      case 5:  // IP address
         UserLookup.Callsign = pPuller->Callsign;
         if((pUser = avl_find(UserTree,&UserLookup)) == NULL) {
         // New user
            D2PRINTF(("ParseStationList(): Adding new user \"%s\".\n",
                     pPuller->Callsign));
            pUser = CreateNewUser(pPuller->Callsign);
            bNewUser = TRUE;
            bNewIPAdr = TRUE;
            pPuller->NewStations++;
         }
         else {
            pPuller->UpDatedStations++;
            D2PRINTF(("ParseStationList(): Updating user \"%s\".\n",
                     pPuller->Callsign));
         }

         if(pUser != NULL) {
            pUser->bActive = pUser->bRefreshed = TRUE;
            pUser->bAuthorized = TRUE;
            pUser->LastHeard = TimeNow.tv_sec;

            if(pPuller->Qth[0] != 0) {
               pUser->bBusy = pPuller->bBusy;
               if(pUser->Qth != NULL) {
                  free(pUser->Qth);
               }
               pUser->Qth = strdup(pPuller->Qth);
            }

            if(!bEmptyField) {
               if(!bNewUser && pUser->HisAdr.ADDR != inet_addr(Buf)) {
               // The user's IP address has changed, remove the old reference
               // from the IP address tree
                  avl_delete(UserIPAdrTree,pUser);
                  bNewIPAdr = TRUE;
               }
               if(iLinkDirServer) {
                  DeScrambleIP(Buf);
               }

               pUser->HisAdr.ADDR = inet_addr(Buf);

               if(pUser->HisAdr.ADDR == INADDR_NONE) {
                  LOG_ERROR(("Invalid IP address \"%s\" for %s\n",Buf,
                             pUser->Callsign));
               }
            }

            pUser->NodeID = pPuller->NodeID;

            if(bNewUser) {
               avl_insert(UserTree,pUser);
               pPuller->NewStations++;
            }

            if(bNewIPAdr) {
               avl_insert(UserIPAdrTree,pUser);
            }
         }
         if(--pPuller->StationCount == 0 && pPuller->bDifferential) {
         // done with the active stations next are the deleted stations
            State = 6;
         }
         else {
            State = 2;  // back to looking for callsign
         }
         break;

      case 6:  // Deleted stations
         UserLookup.Callsign = Buf;
         if((pUser = avl_find(UserTree,&UserLookup)) == NULL) {
         // user not found 
            LOG_ERROR(("ParseStationList(): Deleted user \"%s\" not found.\n",
                     Buf));
         }
         else {
            pUser->bActive = FALSE;
            pPuller->DeletedStations++;
         }
         break;
   }

   pClient->x = State;
}

// Remove any stations from the user list that are no longer present
// in the list we just pulled from the directory server
void CleanUserList(int bDifferential)
{
   UserInfo *pUser;
   UserInfo *pNextUser;
   struct avl_traverser avl_trans;
   FILE  *fp = NULL;
   int   bDeleteUser;

   ActiveDirEntries = 0;
   InactiveDirEntries = 0;

   if(WriteHostFile) {
      if((fp = fopen(ECHOLINK_HOSTS_TMP_FILE,"w")) != NULL) {
         fprintf(fp,"#IP Adr\tCallsign\tNode number\tQth\n");
      }
   }

   pNextUser = (UserInfo *) avl_t_first(&avl_trans,UserTree);
   while((pUser = pNextUser) != NULL) {
   // NB: Get the next user now in case we deleted the current user
      pNextUser = (UserInfo *) avl_t_next(&avl_trans);
      bDeleteUser = FALSE;

      if(strcmp(pUser->Callsign,ConferenceCall) == 0) {
      // It's us !
         OurNodeID = ntohl(pUser->NodeID);
      }

      if(bDifferential) {
         if(!pUser->bActive) {
         // He's gone
            if((TimeNow.tv_sec - pUser->LastHeard) > InactiveDirTimeout) {
               bDeleteUser = TRUE;
            }
         }
      }
      else {
      // Full list
         if(!pUser->bRefreshed) {
         // He's gone
            if((TimeNow.tv_sec - pUser->LastHeard) > InactiveDirTimeout) {
               bDeleteUser = TRUE;
            }
            else {
            // Keep him around for a while to minimize the number of time 
            // we have to ask the directory server about users who have just
            // logged in.
               pUser->bActive = FALSE;
            }
         }
      }


      if(bDeleteUser) {
         DeleteUser(pUser,FALSE);
      }
      else {
         pUser->bRefreshed = FALSE;
         if(pUser->bActive || IncludeAllHosts) {
            ActiveDirEntries++;
            if(fp != NULL && pUser->bAuthorized) {
               if(QthInHostsFile) {
                  fprintf(fp,"%s\t%s\t# %d\t%s%s\n",
                          inet_ntoa(pUser->HisAdr.i.sin_addr),
                          pUser->Callsign,pUser->NodeID,
                          pUser->Qth,  
                          pUser->bActive ? "" : " (off line)");
               }
               else {
                  fprintf(fp,"%s\t%s\t# %d%s\n",
                          inet_ntoa(pUser->HisAdr.i.sin_addr),
                          pUser->Callsign,pUser->NodeID,
                          pUser->bActive ? "" : " (off line)");
               }
            }
         }
         else {
            InactiveDirEntries++;
         }
      }
   }

   if(fp != NULL) {
      fclose(fp);
      unlink(ECHOLINK_HOSTS_FILE);
      rename(ECHOLINK_HOSTS_TMP_FILE,ECHOLINK_HOSTS_FILE);
      EventHook("hostfile");
   }
}

int CheckCallResp(ClientInfo *pClient)
{
   int  BytesRead;
   char Data;
   PullerInfo *pPuller = (PullerInfo *) pClient->p;
   UserInfo *pUser;
   UserInfo UserLookup;
   char *CheckArg = (char *) pPuller->Arg;
   char *cp;
   int Err = 0;
   char temp[32];

   if(pClient->bReadReady) {
      if((BytesRead = READ(pClient->Socket,&Data,1)) == 1) {
      // we have our answer
         cp = strchr(CheckArg,'\r');
         *cp++ = 0;
         UserLookup.HisAdr.ADDR = inet_addr(cp);
         UserLookup.Callsign = CheckArg;
         if(Data == '1') {
            if((pUser = avl_find(UserTree,&UserLookup)) == NULL) {
            // User doesn't exists, create him
               pUser = CreateNewUser(CheckArg);
               pUser->HisAdr.ADDR = UserLookup.HisAdr.ADDR;
               pUser->bActive = TRUE;
               pUser->bAuthorized = TRUE;
               avl_insert(UserTree,pUser);
               avl_insert(UserIPAdrTree,pUser);
               LOG_NORM(("CheckCallResp(): added authorized client %s.\n",
                         pUser->Callsign));
            }
            else {
            // Existing user
               if(pUser->HisAdr.ADDR != UserLookup.HisAdr.ADDR) {
                  strcpy(temp,inet_ntoa(pUser->HisAdr.i.sin_addr));

                  LOG_NORM(("%s's IP address changed from %s to %s.\n",
                            pUser->Callsign,temp,
                            inet_ntoa(UserLookup.HisAdr.i.sin_addr)));
                  pUser->HisAdr.ADDR = UserLookup.HisAdr.ADDR;
                  pUser->bAuthorized = TRUE;
               }
            }
         }
         else {
            LOG_NORM(("CheckCallResp(): server returned 0x%x for client %s.\n",
                      Data,CheckArg));
         }
      }
      else {
         Err = errno;
      }
   }
   else if(pClient->bTimeOut) {
      Err = ERR_SERVER_TIMEOUT;
      LOG_NORM(("CheckCallResp(): Timed out waiting for a response.\n"));
   }

   OpComplete(pClient,Err);
   
   return 0;
}

int CheckCall(ClientInfo *pClient)
{
   char c = 'v';
   int ReqLen;
   char *cp;

   PullerInfo *pPuller = (PullerInfo *) pClient->p;
   cp = (char *) pPuller->Arg;
   
   ReqLen = strlen(cp);
   if(WRITE(pClient->Socket,&c,1) != 1 ||
      WRITE(pClient->Socket,cp,ReqLen) != ReqLen)
   {
   // This isn't good.  It's the first thing we've sent.
   // The buffer can't be full yet !
      LOG_WARN(("CheckCall(): write() failed, %s",Err2String(ERROR_CODE)));
      DeleteClient(pClient);
   }

   pClient->State = CheckCallResp;
   SET_WAIT4_RD(pClient);

// Wait for up to SERVER_REQ_TO milliseconds for the response
   SetTimeout(pClient,SERVER_REQ_TO); 

   return 0;
}

void PullerCleanup(ClientInfo *p)
{
   PullerInfo *pPuller = (PullerInfo *) p->p;
   
   if(pPuller->Req == SERV_REQ_LOGIN_AND_LIST && p->Err == 0) {
   // Start second half of request
      ServerRequest(SERV_REQ_STATION_LIST,pPuller->Server,NULL);
   }

   if(pPuller->Arg != NULL) {
      free(pPuller->Arg);
   }

   if(pPuller->ZBuf != NULL) {
      free(pPuller->ZBuf);
   }

   if(pPuller->ZBufSize > 0) {
      inflateEnd(&pPuller->c_stream);
   }

   free(p->p);
   p->p = NULL;
}

int ServerRequestComplete(ClientInfo *p)
{
   PullerInfo *pPuller = (PullerInfo *) p->p;
   SERVER_REQUEST Request = pPuller->Req;
   void *Arg = pPuller->Arg;
   int Server = pPuller->Server;
   int Err = p->Err;

   pPuller->Arg = NULL;
   DeleteClient(p);

   if(Err == 0) {
      ServerStats[Server].Success++;
   }
   else {
   // Requested operation failed
      ServerStats[Server].Failure++;
      LOG_ERROR(("ServerRequest %s failed with server %s, Err %d\n",
                 Req2String[Request],DirServerHost[Server],Err));

      if(++Server < NUM_DIRECTORY_SERVERS && DirServerHost[Server] != NULL) {
      // Another directory server to try
         LOG_WARN(("Trying %s with backup server %s\n",
                   Req2String[Request],DirServerHost[Server]));
         ServerRequest(Request,Server,Arg);
         Arg = NULL;
      }
      else if(Request == SERV_REQ_SHUTDOWN) {
      // Oh well, we tried.
         LOG_ERROR(("Couldn't Log out, exiting anyway.\n"));
         bRunning = FALSE;
      }
   }

   if(Arg != NULL) {
      free(Arg);
   }
   return 0;
}


void DeScrambleIP(char *IO)
{
   int InputLen;
   int i;
   int KeyOffset = 0;
   char Key[] = "91182092262753893";
   char Substute1[] = "4325198076";
   char Substute2[] = "3719458602";
   char *InputSave = strdup(IO);

   InputLen = strlen(IO);

   D3PRINTF(("DeScrambleIP(): Input string \"%s\".\n",IO));

   for(i = 0; i < InputLen-2; i++) {
      IO[i] = IO[i] - Key[KeyOffset++];
      if(KeyOffset > 14) {
         KeyOffset = 0;
      }
   }

   for(i = 0; i < InputLen; i++) {
      if(isdigit(IO[i])) {
         IO[i] = Substute1[IO[i] - '0'];
      }
      else if(IO[i] == '}') {
         IO[i] = '.';
      }
   }

   for(i = 0; i < InputLen; i++) {
      if(isdigit(IO[i])) {
         IO[i] = Substute2[IO[i] - '0'];
      }
      else if(IO[i] == '=') {
         IO[i] = '.';
      }
   }

   if(inet_addr(IO) == INADDR_NONE) {
      LOG_ERROR(("DeScrambleIP() failed: in \"%s\", out \"%s\"\n",InputSave,IO));
   }
   free(InputSave);

   D3PRINTF(("DeScrambleIP(): Output string \"%s\".\n",IO));
}

void RightTrim(char *Buf)
{
   char *cp;
   int Strlen = strlen(Buf);

   if(Strlen > 0) {
      cp = Buf + Strlen - 1;
      for( ; ; ) {
         if(*cp == ' ') {
            *cp = 0;
         }
         else {
            break;
         }

         if(cp-- == Buf) {
            break;
         }
      }
   }
}

void DirCleanup()
{
   UserInfo *pUser;
   UserInfo *pNextUser;
   struct avl_traverser avl_trans;

   pNextUser = (UserInfo *) avl_t_first(&avl_trans,UserTree);
   while((pUser = pNextUser) != NULL) {
      pNextUser = (UserInfo *) avl_t_next(&avl_trans);

      DeleteUser(pUser,FALSE);
   }
}

