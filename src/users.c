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

   $Log: users.c,v $
   Revision 1.8  2008/02/26 17:34:44  wb6ymh
   Added dmalloc support.

   Revision 1.7  2007/12/01 01:02:21  wb6ymh
   Deleted STRCMP macro, it's now in common.h.

   Revision 1.6  2007/06/27 19:21:15  wb6ymh
   Added code to DeleteUser to free the QTH string.

   Revision 1.5  2004/05/29 17:32:49  wb6ymh
   1. Eliminated unused and unneeded fields from the UserInfo structure to reduce
      memory requirements.
   2. Added delete of the user structure itself in DeleteUser() (!).

   Revision 1.4  2003/08/31 23:24:07  wb6ymh
   Corrected portability problem caused by the assumption that u_long was the
   same size as an IP address.

   Revision 1.3  2003/04/30 22:01:25  wb6ymh
   Modified LoadACL() and SaveACL() to use the AppName configuration variable's
   value as the application name when creating pid files rather than a
   hardcoded "tbd".

   Revision 1.2  2002/11/02 18:31:10  wb6ymh
   Added routines to load, save, modify and refresh the access control list.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#include "common.h"

#ifndef _WIN32
   // FreeBSD, Linux, etc..
   #include <stdio.h>
   #include <stdlib.h>
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
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <arpa/inet.h>
   #include <string.h>
   #include <ctype.h>
   #include <netdb.h>
   #include <sys/stat.h>
#else
   // Windoze
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <time.h>
   #include <winsock2.h>
   #include <ctype.h>
   #include <sys/stat.h>
#endif

#include "avl.h"
#include "main.h"
#include "users.h"
#include "configvars.h"
#include "hostfile.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define ACL_FILENAME_EXT   "acl"

struct avl_table *UserTree;
struct avl_table *UserIPAdrTree;
struct avl_table *ACL_Call_Tree;
struct avl_table *ACL_IP_Tree;

UserInfo *pUserDebug = NULL;

int Id = 1001;

void DeleteUser(UserInfo *pUser,int Virgin)
{
   D2PRINTF(("Deleting user \"%s\".\n",pUser->Callsign));
   if(!Virgin && avl_find(UserIPAdrTree,pUser) != NULL) {
      avl_delete(UserIPAdrTree,pUser);
   }

   if(!Virgin) {
      if(avl_delete(UserTree,pUser) == NULL) {
         LOG_ERROR(("DeleteUser(): avl_delete() failed to find user \"%s\".\n",
                    pUser->Callsign));
      }
   }

   if(pUser->Callsign != NULL) {
      free(pUser->Callsign);
   }

   if(pUser->Qth != NULL) {
       free(pUser->Qth);
   }

   free(pUser);
}

// User compares by callsign
int UserCompare(const void *avl_a,const void *avl_b,void *avl_param)
{
   UserInfo *p_a = (UserInfo *) avl_a;
   UserInfo *p_b = (UserInfo *) avl_b;

   return STRCMP(p_a->Callsign,p_b->Callsign);
}

// User compares by callsign
int UserIPCompare(const void *avl_a,const void *avl_b,void *avl_param)
{
   UserInfo *p_a = (UserInfo *) avl_a;
   UserInfo *p_b = (UserInfo *) avl_b;

   if(p_a->HisAdr.ADDR == p_b->HisAdr.ADDR) {
      return 0;
   }

   if(p_a->HisAdr.ADDR > p_b->HisAdr.ADDR) {
      return 1;
   }
   else {
      return -1;
   }
}

int ACL_CallCompare(const void *avl_a,const void *avl_b,void *avl_param)
{
   ACL_User *p_a = (ACL_User *) avl_a;
   ACL_User *p_b = (ACL_User *) avl_b;

   return STRCMP(p_a->Callsign,p_b->Callsign);
}

// User compares by callsign
int ACL_IPCompare(const void *avl_a,const void *avl_b,void *avl_param)
{
   ACL_User *p_a = (ACL_User *) avl_a;
   ACL_User *p_b = (ACL_User *) avl_b;

   if(p_a->HisAdr.ADDR == p_b->HisAdr.ADDR) {
      return 0;
   }

   if(p_a->HisAdr.ADDR > p_b->HisAdr.ADDR) {
      return 1;
   }
   else {
      return -1;
   }
}


UserInfo *CreateNewUser(char *Callsign)
{
   UserInfo *pUser = malloc(sizeof(UserInfo));

   if(pUser != NULL) {
      memset(pUser,0,sizeof(UserInfo));
      pUser->Callsign = strdup(Callsign);
      pUser->NodeID = Id++;
   }
   return pUser;
}

void DeleteACLUser(ACL_User *pACL)
{
   avl_delete(ACL_IP_Tree,pACL);
   avl_delete(ACL_Call_Tree,pACL);

   if(pACL->Callsign != NULL) {
      free(pACL->Callsign);
   }

   if(pACL->CallPlus != NULL) {
      free(pACL->CallPlus);
   }

   if(pACL->HostName != NULL) {
      free(pACL->HostName);
   }

   if(pACL->Password != NULL) {
      free(pACL->Password);
   }

   free(pACL);
}

// The following fields must be filled in: 
//    Callsign
//    HostName ("-" for no hostname checking) 
//    Password ("-" for no password checking)
//    bAuthorized
//    Optionally:
//    CallPlus - User's name (appended to Callsign to create CallPlus)
int ACLAddUser(ACL_User *pACL)
{
   int Ret = TRUE;
   ACL_User *pOldACL = NULL;
   ACL_User *pNewACL;
   struct hostent *pTemp;
   char *cp;
   int StrLen;

   pACL->HisAdr.ADDR = INADDR_NONE;

   if(*pACL->HostName != '-') {
   // A host name was specified, make sure it's valid
      pTemp = GetHostByName(pACL->HostName);
      if(pTemp == NULL) {
         Ret = FALSE;
      }
      else {
         pACL->HisAdr.ADDR = IP_FROM_HOSTENT(pTemp,0);
      }
   }

   if(Ret) {
   // The ACL Callsign field does not contain a "-L" or "-R", but the
   // callplus field does...
      if((cp = strchr(pACL->Callsign,'-')) != NULL) {
      // Remove -R or -L from the callsign
         *cp = 0;
      }

      if((pOldACL = avl_find(ACL_Call_Tree,pACL)) != NULL) {
      // delete old entry
         DeleteACLUser(pOldACL);
      }

      pNewACL = (ACL_User *) malloc(sizeof(ACL_User));
      if(pNewACL != NULL) {
         memset(pNewACL,0,sizeof(ACL_User));
         pNewACL->bRefreshed = pACL->bRefreshed;
         pNewACL->HostName = strdup(pACL->HostName);
         pNewACL->Callsign = strdup(pACL->Callsign);
         if(cp != NULL) {
            *cp = '-';
         }
         
         if(pACL->CallPlus != NULL) {
            StrLen = strlen(pACL->Callsign) + strlen(pACL->CallPlus) + 2;
            pNewACL->CallPlus = malloc(StrLen);
            if(pNewACL->CallPlus != NULL) {
               strcpy(pNewACL->CallPlus,pACL->Callsign);
               if(*pACL->CallPlus != '-') {
                  strcat(pNewACL->CallPlus," ");
               }
               strcat(pNewACL->CallPlus,pACL->CallPlus);
            }
         }
         else {
            pNewACL->CallPlus = strdup(pACL->Callsign);
         }
         
         if(pACL->Password != NULL) {
            pNewACL->Password = strdup(pACL->Password);
         }
         pNewACL->HisAdr.ADDR = pACL->HisAdr.ADDR;
         pNewACL->bAuthorized = pACL->bAuthorized;
         pNewACL->LastResolved = pACL->LastResolved;
         avl_insert(ACL_Call_Tree,pNewACL);
         if(*pNewACL->HostName != '-') {
            avl_insert(ACL_IP_Tree,pNewACL);
         }
      }
   }

   return Ret;
}

void RefreshACL()
{
   ACL_User *pACL = NULL;
   struct avl_traverser avl_trans;
   struct hostent *pTemp;

   pACL = (ACL_User *) avl_t_first(&avl_trans,ACL_Call_Tree);
   while(pACL != NULL) {
      if(*pACL->HostName != '-') {
         pTemp = GetHostByName(pACL->HostName);
         if(pTemp != NULL) {
            pACL->HisAdr.ADDR = IP_FROM_HOSTENT(pTemp,0);
         }
      }
      pACL = (ACL_User *) avl_t_next(&avl_trans);
   }
}

void LoadACL()
{
   char Line[80];
   char *Allow;
   ACL_User *pACL = NULL;
   ACL_User *pNextACL = NULL;
   struct avl_traverser avl_trans;
   char  *WhiteSpace = "\t\n ";
   int LineNum = 0;
   FILE *fp = NULL;
   ACL_User ACL_New;
   struct stat FileStats;
   char  AclFilename[80];

   snprintf(AclFilename,sizeof(AclFilename),"%s." ACL_FILENAME_EXT,AppName);
   
   if((fp = fopen(AclFilename,"r")) == NULL) {
      if(stat(AclFilename,&FileStats) == 0) {
      // The ACL file exists but we couldn't open it, complain
         LOG_ERROR(("LoadACL(): Unable to open ACL file.\n"));
      }
      return;
   }

   memset(&ACL_New,0,sizeof(ACL_New));
   ACL_New.bRefreshed = TRUE;
   while(fgets(Line,sizeof(Line),fp) != NULL) {
      LineNum++;

      Allow = strtok(Line,WhiteSpace);
      if(Allow == NULL || *Allow == '#' || *Allow == ';') {
      // Empty line or comment
         continue;
      }

      if(STRCMP(Allow,"allow") == 0) {
         ACL_New.bAuthorized = TRUE;
      }
      else if(STRCMP(Allow,"deny") == 0) {
         ACL_New.bAuthorized = FALSE;
      }
      else {
      // Invalid line
         LOG_WARN(("LoadACL(): Ignoring line %d, invalid action \"%s\".\n",
                   LineNum,Allow));
         continue;
      }

      ACL_New.Callsign = strtok(NULL,WhiteSpace);
      if(ACL_New.Callsign == NULL) {
      // Invalid line
         LOG_WARN(("LoadACL(): Ignoring line %d, callsign missing.\n",LineNum));
         continue;
      }

      if((ACL_New.HostName = strtok(NULL,WhiteSpace)) == NULL) {
         LOG_WARN(("LoadACL(): Ignoring line %d, hostname missing.\n",LineNum));
         continue;
      }

      if((ACL_New.Password = strtok(NULL,WhiteSpace)) == NULL) {
         LOG_WARN(("LoadACL(): Ignoring line %d, password missing.\n",LineNum));
         continue;
      }
      
      ACL_New.CallPlus = strtok(NULL,"\n");

   // Skip any leading whitespace

      if(ACL_New.CallPlus != NULL) {
         while(*ACL_New.CallPlus && isspace(*ACL_New.CallPlus)) {
            ACL_New.CallPlus++;
         }
      }

      if(!ACLAddUser(&ACL_New)) {
         LOG_WARN(("LoadACL(): Ignoring line %d, unable to resolve "
                   "hostname \"%s\".\n",LineNum,ACL_New.HostName));
      }
   }
   fclose(fp);

// Remove any old entries from tree
   
   pNextACL = (ACL_User *) avl_t_first(&avl_trans,ACL_Call_Tree);
   while((pACL = pNextACL) != NULL) {
   // NB: Get the next user now in case we deleted the current user
      pNextACL = (ACL_User *) avl_t_next(&avl_trans);
      if(!pACL->bRefreshed) {
         DeleteACLUser(pACL);
      }
      pACL->bRefreshed = FALSE;
   }
}

// ACL format:
// <"allow"|"deny>"<tab><callsign><tab><hostname/IP Adr><tab><password><tab>
// <Name/Location>
//
// The callsign field is required and should be entered in upper case without 
// any suffix (-R or -L).  Stations are allowed/denied based on the base 
// callsign.
//
// hostname can be a numeric IP address, actual resolvable hostname or "-" for 
// no hostname
//
// The password can be "-" for no password or the actual password. The password
// may not contain spaces or tabs.
//
// The Name/location field is free text and extends to the end of the line.
// If it is desired to display <callsign>-R or <callsign>-L on the Echolink
// station list start the name/location field with "-R" or "-L".
//
void SaveACL()
{
   struct avl_traverser avl_trans;
   ACL_User *pACL;
   char *cp;
   FILE *fp = NULL;
   char  AclFilename[80];

   snprintf(AclFilename,sizeof(AclFilename),"%s." ACL_FILENAME_EXT,AppName);
   if((fp = fopen(AclFilename,"w")) == NULL) {
      LOG_ERROR(("SaveACL(): Unable to open ACL file for write.\n"));
      return;
   }

   pACL = (ACL_User *) avl_t_first(&avl_trans,ACL_Call_Tree);
   while(pACL != NULL) {
      if(pACL->CallPlus == NULL) {
         cp = NULL;
      }
      else if((cp = strchr(pACL->CallPlus,'-')) == NULL) {
         cp = strchr(pACL->CallPlus,' ');
         if(cp != NULL) {
         // Skip the blank
            cp++;
         }
      }
      
      fprintf(fp,"%s\t%s\t%s\t%s\t%s\n",
              pACL->bAuthorized ? "allow" : "deny",
              pACL->Callsign,
              pACL->HostName,
              pACL->Password,
              cp == NULL ? "" : cp);

      pACL = (ACL_User *) avl_t_next(&avl_trans);
   }
   fclose(fp);
}


