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

   $Log: conference.h,v $
   Revision 1.36  2012/12/09 18:50:43  wb6ymh
   Removed old chan_rptdir style Asterisk support.

   Revision 1.35  2010/11/20 15:47:44  wb6ymh
   1. Added bRTPConf to ConfServer.
   2. Added bNailed to ConfClient.
   3. Deleted unused pSFConf.
   4. Added prototype for RtpInit.

   Revision 1.34  2009/09/13 20:08:02  wb6ymh
   1. Added bDisconnected, bConnected, bIRLP.
   2. Added Callsign argument to StartCmdHandler prototype.
   3. Added GetConfigVarsPtr.

   Revision 1.33  2009/06/30 23:13:46  wb6ymh
   Added bScratch to ConfClient.

   Revision 1.32  2009/04/04 18:07:59  wb6ymh
   Added Tool and bCallsignOverride to ConfClient.

   Revision 1.31  2009/03/29 16:30:23  wb6ymh
   Added CallLog.

   Revision 1.30  2009/02/14 15:03:25  wb6ymh
   Added LasttAprsIsTime.

   Revision 1.29  2008/12/31 20:11:23  wb6ymh
   Added bMuteChat to ConfCleint and ConfServer structures.

   Revision 1.28  2008/07/26 18:03:59  wb6ymh
   Added EVENT_DTMF_RX, SSO, AsteriskTxSeq.

   Revision 1.27  2008/07/23 14:46:28  wb6ymh
   Added PKT_TYPE_OTHER_AUDIO, CompressionType and AsteriskDisconnect.

   Revision 1.26  2008/07/16 22:39:19  wb6ymh
   1. Increased NUM_PROTOCOLS to 4.
   2. Added PROTO_ASTERISK, bFullDuplex, bAsterisk, AsteriskClient.

   Revision 1.25  2008/06/26 20:25:03  wb6ymh
   Added CmdArg, GetCmdOptions.

   Revision 1.24  2008/03/11 14:23:45  wb6ymh
   Deleted RtpData and NetAudio, they have been moved into thelinkbox headers.

   Revision 1.23  2008/03/09 17:14:45  wb6ymh
   Deleted obsolete TxMap variable.

   Revision 1.22  2008/03/08 06:42:23  wb6ymh
   1. Removed the majority of variables specific to thelinkbox from ConfClient.
   2. Added externs for CmdLineCC and CmdClient.

   Revision 1.21  2008/02/29 16:55:12  wb6ymh
   Added pMasterCS to ConfServer structure.

   Revision 1.20  2008/02/28 23:08:35  wb6ymh
   1. Corrected bug in Play4 (without -u option) command introduced in version
      0.90.  Version 0.90 -> 0.92 were unable to play a file for all users.
   2. Fairly massive changes to fileplay back to be able to support .play4 -u
      when the selected user is an RTP or SF client.  ConvertProtocol is now
      passed a pointer to the ProtoData structure so more than a single
      protocol conversion may be active at one time.

   Revision 1.19  2008/02/26 18:17:19  wb6ymh
   Added ConferenceCleanup prototype.

   Revision 1.18  2008/02/09 16:54:04  wb6ymh
   Added ConnectedStatus, UpdateConnectedStatus.

   Revision 1.17  2007/12/14 23:17:53  wb6ymh
   1. Removed ControlPort member from ConfServer, added Users, bDynamicConf
      pPcmHead, and bSlaveConf.
   2. Removed Used member from NetAudio, added TxMap.
   3. Added Used, pNetAudio and Port members to PCMAudio.
   4. Removed ControlPort argument from CreateConference prototype.

   Revision 1.16  2007/12/14 22:29:20  wb6ymh
   Removed ClientInfo *p parameter from Send2, it's no longer used.

   Revision 1.15  2007/12/07 19:17:13  wb6ymh
   Removed bRxActive variable from ConfServer ... was never used, it's actually
   in ConfClient.

   Revision 1.14  2007/11/26 14:52:31  wb6ymh
   1. Added extern "C" around prototypes when built for C++.
   2. Added RxMap and TxMap members to ConfClient structure.

   Revision 1.13  2007/06/27 19:43:41  wb6ymh
   1. Added variables, externs, and prototypes needed for the upcoming thelinkbox
      project.
   2. Removed pCmdLine and Err members from ConfServer structure.
   3. Moved PacketType enum here from conference.c.
   4. Modified CreateConference prototype to return an error code and add a new
      argument pointing to created conference on success.

   Revision 1.12  2004/11/29 00:36:41  wb6ymh
   Added SendAvrsUpdate prototype.  Include avrs.h for AVRS_State.

   Revision 1.11  2004/11/27 22:14:24  wb6ymh
   Added ConferenceClients.

   Revision 1.10  2004/11/22 01:01:52  wb6ymh
   Removed ClientIPAdrUpdate ... bad bad buggy code.

   Revision 1.9  2004/05/29 17:19:35  wb6ymh
   1. Include "users.h" if it has not already been included.
   2. Added prototype for ClientIPAdrUpdate();

   Revision 1.8  2003/09/09 21:02:08  wb6ymh
   Added bMuteTBD member to ConfServer.

   Revision 1.7  2003/09/07 15:16:47  wb6ymh
   1. Added DupsReceived, DupDisconnects, bPlayWhenIdle, bPlayWhenFree,
      bMuteConf, bMuteUsers, bMuteRF, and pCmdLine members to ConfServer.
   2. Added ssrc, DupsReceived, pCS, bCmdLine, bFilePlayer, and bTxtExtension
      members to ConfClient.
   3. Added prototypes for CmdLine_Handler, GenSDES, StartCmdHandler.

   Revision 1.6  2003/04/30 21:32:59  wb6ymh
   Added bAutoLurk, bTimedOut, bMonitor, bLink, and bRepeater to ConfClient.

   Revision 1.5  2002/12/21 18:49:00  wb6ymh
   1. Added bSentQuotedCmd, bSentInfo to ConfClient.
   2. Added Filename, FirstTimeStamp, LastTimeStamp, bTooSlowLogged and
      bDeleteOnClose to ClientFileIO.

   Revision 1.4  2002/11/02 18:42:04  wb6ymh
   1. Moved EventType, ConfServer, ConfClient and ClientFileIO here from
      conference.c
   2. Added Protocol enum.
   3. Added prototypes for CreateConference(), GenSDES(), and SFConfInit().

   Revision 1.3  2002/09/14 16:43:45  wb6ymh
   Added prototype for GetUserCountString.

   Revision 1.2  2002/08/31 23:56:47  wb6ymh
   Added prototypes for LoadBannedList() and LoadBulletinList().

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#ifndef CONFERENCE_H_
#define CONFERENCE_H_

#ifndef USERS_H_
#include "users.h"
#endif

#ifndef _AVRS_H_
#include "avrs.h"
#endif

#ifdef   LINK_BOX
   #include "gsm.h"
   #include "rtp.h"
#endif

#define CONF_BUF_SIZE      1024
#define MAX_CONV_PACKETS   3

typedef enum {
   EVENT_INIT = 1,
   EVENT_RTP_RX,
   EVENT_RTP_TO,
   EVENT_RTCP_RX,
   EVENT_RTCP_TO,
   EVENT_DELETE,
   EVENT_DTMF_RX
} EventType;

typedef enum {
// NB: Protocols must be the RTCP version minus one
   PROTO_SF,
   PROTO_RTP,
   PROTO_ILINK,
} Protocol;
#define  NUM_PROTOCOLS  3

typedef enum {
   PKT_TYPE_UNKNOWN,
   PKT_TYPE_AUDIO,
   PKT_TYPE_DATA,
   PKT_TYPE_CMD,
   PKT_TYPE_IGNORE,
   PKT_TYPE_UNSUPPORTED,
   PKT_TYPE_OTHER_AUDIO
} PacketType;

typedef struct ConfServer_TAG {
   FILE *fp;
   struct avl_table *ConfTree;   // sorted by IP address
   ClientInfo *pAudio;
   ClientInfo *pControl;
   struct ConfServer_TAG *pMasterCS;
   time_t   TimeNextSDES;
   struct timeval LastAudioIn;
   struct timeval *pLastAudioIn;
   struct ConfClient_TAG *pFilePlayer;

// All time totals
   u_int TxMBytes;    
   u_int TxBytes;    
   u_int TxCount;       // packet count
   u_int RxBytes;
   u_int RxMBytes;
   u_int RxCount;       // packet count

   u_int PeakTxBandWidth;
   time_t PeakTxBandWidthTime;

   u_int PeakRxBandWidth;
   time_t PeakRxBandWidthTime;
   
   struct timeval StartTime;
   u_int TxBytesStart;
   u_int RxBytesStart;

   u_int RxErrs;
   u_int TxErrs;

// Last measurement
   u_int TxBandWidth;      // 1000 bits per second
   u_int RxBandWidth;

   int   AudioPort;  

   int   DupsReceived;
   int   DupDisconnects;
   int   Users;
   int   CompressionType;

   int   bRecording:1;
   int   bLinkedConfs:1;
   int   bSendStationList:1;
   int   biLinkConf:1;
   int   bSFConf:1;
   int   bRTPConf:1;
   int   bPlayWhenIdle:1;
   int   bPlayWhenFree:1;
   int   bMuteConf:1;
   int   bMuteUsers:1;
   int   bMuteRF:1;
   int   bMuteTBD:1;
   int   bDynamicConf:1;   // Dynamically created "conference" for odd port #
   int   bSlaveConf:1;     // ConfTree belongs to another conference
   int   bMuteChat:1;      // mute inbound chat for new connections
} ConfServer;

typedef struct ConfClient_TAG {
   struct ConfClient_TAG *Link;   // NB: must be the first member
   IPAdrUnion HisAdr;   // NB: port is rtcp (audio) port in network order
   int      SN;
   int (*State)(ClientInfo *p,struct ConfClient_TAG *pCC,EventType Event);
   time_t   LoginTime;
   time_t   LastHeard;
   char  *CallPlus;
   char  *Callsign;
   char  *Password;  // Used for RTP & Speak Freely only
   char  *Info;
   char  *Tool;
   struct timeval FirstAudioIn;
   struct timeval LastAudioIn;
   u_int TxBytes;
   u_int RxBytes;
   int   ssrc;
   int   DupsReceived;
   ConfServer *pCS;
   void  *p;         // various uses

   Protocol Proto;
   u_int8   CompressionType;

   int   bInConf:1;
   int   bLurking:1;
   int   bTalking:1; // This client has the conference's "talk" token
   int   bAdmin:1;
   int   bSysop:1;
   int   bDoublingWarningSent:1;
   int   bPauseWarningSent:1;
   int   bSWL:1;        // ignore all received data, including text, commands
   int   bMuted:1;      // ignore received audio
   int   bKicked:1;     // disconnecting, waiting for rtcp timeout
   int   bPermanent:1;  // connection not dropped by rtcp timeout
   int   bConf:1;
   int   bTBD:1;
   int   bSendSSRC:1;
   int   bFileIOActive:1;
   int   bFirstAudio:1;
   int   bNoChat:1;     // Don't send chat to this user
   int   bMuteChat:1;   // Don't forward chat from this user
   int   bBelcher:1;
   int   bSentQuotedCmd:1;
   int   bSentInfo:1;
   int   bAutoLurk:1;
   int   bTimedOut:1;
   int   bMonitor:1;    // don't send audio to this client
   int   bLink:1;
   int   bRepeater:1;
   int   bCmdLine:1;
   int   bFilePlayer:1;
   int   bTxtExtension:1;
   int   bWelcomePending:1;
   int   bFullDuplex:1;
   int   bCallsignOverride:1; // callsign specified by .connect command
   int   bScratch:1;          // misc temporary usage
   int   bDisconnected:1;     // client was explicitly .disconnected
   int   bConnected:1;        // client is not connecting or disconnecting
   int   bIRLP:1;             // client is an IRLP node (call == stnxxxx)
   int   bNailed:1;           // client is a "nailed" up connection

#ifdef   LINK_BOX    // variables needed for client endpoint
   int   bRfUser:1;
   void *pVoipNode;
#endif
} ConfClient;

typedef struct {
   int Clients;
   int SDESLen;
   PacketType Type;
   char *Data[MAX_CONV_PACKETS];
   int DataLen[MAX_CONV_PACKETS];
   char OurSDES[512];
   char Temp[CONF_BUF_SIZE];
   int iLinkCount;
   char *Callsign;
   int   bDataValid:1;
   int   bTxtExtension:1;
} ProtoData;

// Variables used while recording or playing back files
typedef struct {
   ConfClient *pCC;
   ClientInfo *pClient;
   FILE  *fp;
   char  *Filename;
   struct timeval TimeFirstTx;
   struct timeval TimeLastTx;
   int   Timeout;       // in milliseconds
   int   x;
   time_t FirstTimeStamp;
   time_t LastTimeStamp;
   ProtoData *pPDat;
   int   bTooSlowLogged:1;
   int   bDeleteOnClose:1;
} ClientFileIO;

extern int ConferenceClients;

extern ConfClient *ClientTalking;
extern ConfServer *piLinkConf;
extern char *StationDisconnected;

extern int Rcode;
extern int LastRcode;
extern char *LastCmdText;
extern char *LastCmd;

extern struct avl_table *Conferences;   // sorted by port number
extern char ConnectedStatus[];

extern ConfClient CmdLineCC;
extern ClientInfo *CmdClient;

extern char *CmdArg;
extern time_t LasttAprsIsTime;

#ifdef __cplusplus
extern "C" {
#endif

int ConferenceInit(void);
int SFConfInit(void);
int RTP_Handler(ClientInfo *pRTP);
int RTCP_Handler(ClientInfo *p);
void LoadBannedList();
void LoadBulletinList();
char *GetUserCountString();

int CreateConference(int AudioPort,ConfServer *pMainConf,ConfServer **pRet);
int CmdLine_Handler(ClientInfo *p);

void GenSDES(int rtp_version,int bTxtExtension);
void StartCmdHandler(char *Cmd,char *Callsign);
void SendAvrsUpdate(AVRS_State State,char *Callsign);
PacketType GetPacketType(ClientInfo *p,ConfClient *pCC);
int TimeLapse(struct timeval *p);
void Send2(ConfClient *pCC,void *Data,int Count,int bControPort);
void ConferenceCmd(ClientInfo *p,ConfClient *pCC,char *cmd);
void UpdateConnectedStatus(void);
void ConferenceCleanup(void);
int GetCmdOptions(char **args,const char *opts);
void AsteriskDisconnect(ClientInfo *p);
char *CallLog(ConfClient *pCC);
int RtpInit(void);

#ifdef   LINK_BOX
struct config_entry *GetConfigVarsPtr();
#endif

#ifdef __cplusplus
}
#endif

#endif
