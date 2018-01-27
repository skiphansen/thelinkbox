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
   
   $Log: users.h,v $
   Revision 1.7  2008/12/31 20:05:59  wb6ymh
   Added bChatMuted and bChatUnMuted.

   Revision 1.6  2007/06/27 19:21:57  wb6ymh
   Added Qth to UserInfo structure.

   Revision 1.5  2007/04/27 14:31:05  wb6ymh
   Added FindUserByNodeID.

   Revision 1.4  2004/05/29 17:30:53  wb6ymh
   1. Eliminated unused and unneeded fields from the UserInfo structure to reduce
      memory requirements.
   2. Added bMuted and bUnMuted to UserInfo structure.

   Revision 1.3  2003/08/31 23:25:33  wb6ymh
   Deleted unused member bRemote in UserInfo structure.

   Revision 1.2  2002/11/02 18:33:00  wb6ymh
   Added prototypes for new routines that load, save, modify and refresh the
   access control list.  Added ACL_User typedef.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#ifndef USERS_H_
#define USERS_H_

extern struct avl_table *UserTree;
extern struct avl_table *UserIPAdrTree;
extern struct avl_table *ACL_Call_Tree;
extern struct avl_table *ACL_IP_Tree;

// UserInfo contains data on current user who may or may not be
// connected currently
typedef struct {
   IPAdrUnion HisAdr;
   time_t   LastHeard;
   char     *Callsign;
   char     *Qth;
   int      NodeID;

   int      bAuthorized:1;
   int      bActive:1;
   int      bBusy:1;
   int      bRefreshed:1;
   int      bMuted:1;
   int      bUnMuted:1;
   int      bChatMuted:1;
   int      bChatUnMuted:1;
} UserInfo;

typedef struct {
   IPAdrUnion HisAdr;
   char *Callsign;
   char *CallPlus;
   char *HostName;      // NULL if no host name specified
   char *Password;      // NULL if no password specified
   time_t LastResolved;
   int   bAuthorized:1;
   int   bRefreshed:1;
} ACL_User;

void DeleteUser(UserInfo *pUser,int Virgin);
int UserCompare(const void *avl_a,const void *avl_b,void *avl_param);
int UserIPCompare(const void *avl_a,const void *avl_b,void *avl_param);
int ACL_CallCompare(const void *avl_a,const void *avl_b,void *avl_param);
int ACL_IPCompare(const void *avl_a,const void *avl_b,void *avl_param);
UserInfo *CreateNewUser(char *Callsign);
void AddNewUser(UserInfo *p);
int ACLAddUser(ACL_User *pACL);
void LoadACL();
void SaveACL();
void DeleteACLUser(ACL_User *pACL);
void RefreshACL();
UserInfo *FindUserByNodeID(int NodeID);
#endif
