/* Copyright (C) 2008 GFRN systems

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

   $Log: linkbox.h,v $
   Revision 1.8  2010/11/17 04:51:15  wb6ymh
   Added CmdTxPower prototype.

   Revision 1.7  2009/09/13 19:20:37  wb6ymh
   Added CmdRxLevel prototype.

   Revision 1.6  2009/01/03 22:53:39  wb6ymh
   CmdPCMRecord -> CmdPCM.

   Revision 1.5  2008/12/30 16:19:41  wb6ymh
   Added CmdSendAx25Beacon, CmdPCM prototypes.

   Revision 1.4  2008/06/26 15:35:26  wb6ymh
   Added CmdFrequency.

   Revision 1.3  2008/05/14 18:16:40  wb6ymh
   Added CmdRxTone, CmdTxTone, CmdRxFrequency, CmdTxOffset.

   Revision 1.2  2008/04/11 16:55:23  wb6ymh
   Deleted obsolete CmdDtmfGen.

   Revision 1.1  2008/03/09 17:10:13  wb6ymh
   Inital import - files broken out of old audio.cpp and audio.h due to
   major restructuring of the internal infrastructure to treat VoIP connections
   as pseudo ports.

*/
#ifndef _LINKBOX_H_
#define _LINKBOX_H_

#ifdef __cplusplus
extern "C" {
#endif

void CmdSetPort(ClientInfo *p,ConfClient *pCC1,char *Arg);
int EndPointInit();
int EndPoint(ClientInfo *p,ConfClient *pCC,EventType Event);
void CmdDtmfDecode(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdSay(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdShutup(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdSayResult(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdSweep(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdForceID(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdSayStatus(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdScript(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdLink(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdUnLink(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdRxTone(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdTxTone(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdFrequency(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdRxFrequency(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdTxOffset(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdSendAx25Beacon(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdRxLevel(ClientInfo *p,ConfClient *pCC1,char *Args);
void CmdTxPower(ClientInfo *p,ConfClient *pCC1,char *Arg);
void StartTalking(int bTalk);
void EndPointEvent(char *EventText);
int LoadCommandFile(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
void CmdToneGen(ClientInfo *p,ConfClient *pCC1,char *Arg);
void CmdPCM(ClientInfo *p,ConfClient *pCC1,char *Arg);
#ifdef __cplusplus
}
#endif
#endif   //_LINKBOX_H_

