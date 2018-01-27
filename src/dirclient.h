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

   $Log: dirclient.h,v $
   Revision 1.5  2008/02/26 17:38:36  wb6ymh
   Added DirCleanup prototype.

   Revision 1.4  2002/11/02 18:23:21  wb6ymh
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

   Revision 1.3  2002/09/14 17:10:17  wb6ymh
   Added OurNodeID.

   Revision 1.2  2002/08/31 23:56:06  wb6ymh
   Added ServerStats, and DuplicateClientsDeleted.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#ifndef DIRCLIENT_H_
#define DIRCLIENT_H_

extern int PullerStatusBusy;
extern int DuplicateClientsDeleted;

typedef enum {
   SERV_REQ_NONE,
   SERV_REQ_LOGIN,
   SERV_REQ_LOGOUT,
   SERV_REQ_LOGIN_AND_LIST,// Login and then get station list
   SERV_REQ_STATION_LIST,  // get station list (assumes already logged in)
   SERV_REQ_SHUTDOWN,      // logout, then exit program
   SERV_REQ_CHECK_CALL     // check if call/IP is authorized
} SERVER_REQUEST;

typedef struct {
   int Requests;
   int Success;
   int Failure;
} ServerStats_t;

extern ServerStats_t ServerStats[NUM_DIRECTORY_SERVERS];
extern unsigned long OurNodeID;  // NB: in *Network Order*
extern int ActiveDirEntries;
extern int InactiveDirEntries;

void ValidateCallsign(char *Callsign,IPAdrUnion *pIP);
void ServerRequest(SERVER_REQUEST Req,int Server,void *Arg);
void DirCleanup(void);

#endif
