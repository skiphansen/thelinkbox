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

   $Log: ntservice.h,v $
   Revision 1.1  2002/12/18 00:46:48  wb6ymh
   Initial commit.


*/

#ifndef NTSERVICE_H_
#define NTSERVICE_H_

void InstallService();
void RemoveService();
int RunAsService();
BOOL ReportStatusToSCMgr(DWORD CurrentState,DWORD ExitCode,DWORD WaitHint);

#endif
