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

   $Log: hostfile.h,v $
   Revision 1.5  2008/05/14 20:59:39  wb6ymh
   Changed a bunch of functions to take const char * rather than char *
   to fix compile problems with GCC 4.2.x.

   Revision 1.4  2008/02/26 17:39:33  wb6ymh
   Added HostCleanup prototype.

   Revision 1.3  2007/11/26 14:58:55  wb6ymh
   Added extern "C" around prototypes when compiled for C++.

   Revision 1.2  2003/08/31 23:23:18  wb6ymh
   Added IP_FROM_HOSTENT macro.

   Revision 1.1  2002/11/02 15:50:25  wb6ymh
   Initial import.


*/

#ifndef  _HOSTFILE_H_
#define  _HOSTFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct hostent *GetHostByName(const char *HostName);
void UpdateDNSCache(int bForced);
void DumpDNSCache(ClientInfo *p);
void HostCleanup(void);

#ifdef __cplusplus
}
#endif

extern int bLoadHostFileOnStart;

// Convenience macro to get N'th IP address from the hostent pointer
// returned by GetHostByName()
#define  IP_FROM_HOSTENT(x,n) (((struct in_addr *) x->h_addr_list[n])->s_addr)

#endif
