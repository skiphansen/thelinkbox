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

   $Log: configvars.h,v $
   Revision 1.36  2012/12/09 16:27:41  wb6ymh
   1. Removed old chan_rptdir style Asterisk support variables AsteriskIP,
   AsteriskBind2IP, AsteriskDesc, AsteriskPort, and AsteriskEnable.
   Asterisk is now supported by G.726 and Ulaw RTP connections.
   2. Added ConfTextEnable and ConfCmdEnable.
   3. Corrected bug in LoadFile which caused odd crashes and general bad behavor
      when the configuration variables HelpFile or InfoFile where used.

   Revision 1.35  2010/11/20 15:48:56  wb6ymh
   1. Added RTPBin2IP, RTP_Port.
   2. Deleted ConfigPass.
   3. Added prototype for CheckRTP_PortSpec.

   Revision 1.34  2009/09/13 20:08:38  wb6ymh
   Added VoipConfigVars.

   Revision 1.33  2009/07/03 17:05:21  wb6ymh
   Added TempDir.

   Revision 1.32  2009/03/29 16:32:33  wb6ymh
   Added LogIPAddresses.

   Revision 1.31  2008/12/31 20:09:40  wb6ymh
   1. Added Ax25BeaconPath, Ax25BeaconText, AprsIsComment, AprsSymbolTable,
      AprsSymbolCode, AprsObjectName, AprsIsInterval, Ax25BeaconInterval,
      AprsIsEnable,EnableRfPHG and RptOffset.
   2. Deleted VoipInMap and VoipOutMap.

   Revision 1.30  2008/07/23 14:47:12  wb6ymh
   Added AsteriskEnable, CompressionType, and FullDuplex configuration variables.

   Revision 1.29  2008/07/16 22:40:23  wb6ymh
   Added AsteriskBind2IP, AsteriskIP, AsteriskDesc, EnableRemoteDTMF,
   FullDuplex, and AsteriskPort.

   Revision 1.28  2008/05/14 21:01:37  wb6ymh
   Added CallSignSpelling, PhoneticFilePath, Phonetic variables.

   Revision 1.27  2008/03/09 17:42:34  wb6ymh
   Deleted CWId, CWToneFreq, VoiceIDIdleTime.

   Revision 1.26  2008/03/09 17:15:30  wb6ymh
   Deleted obsolete variables VoipCtoneSpec, VoipCtoneMap, VoipCtoneDelay and
   VoipCtone.

   Revision 1.25  2008/03/08 06:44:25  wb6ymh
   1. Changed VoipInMap, VoipOutMap and VoipCToneMap to char *.
   2. Deleted CurrentPort, SerialDevName.

   Revision 1.24  2008/02/26 17:36:38  wb6ymh
   Added bAllowADPCM and bEchoLinkEnable.

   Revision 1.23  2008/02/09 16:53:26  wb6ymh
   Added ShowStatusInInfo.

   Revision 1.22  2008/01/03 16:07:59  wb6ymh
   Added SFBind2IP.

   Revision 1.21  2007/12/27 18:00:11  wb6ymh
   Added FullName.

   Revision 1.20  2007/12/14 23:18:47  wb6ymh
   Deleted DefaultMap, added NumPorts, VoipInMap and VoipOutMap.

   Revision 1.19  2007/12/07 19:11:56  wb6ymh
   Added VoipDejitterDelay.

   Revision 1.18  2007/12/01 01:01:49  wb6ymh
   1. Added VoipCtoneSpec, SavedToneSpec, DtmfEncodeDuration, DtmfEncodePause,
      VoipCtoneDelay, VoipCtoneMap and VoipCtone..
   2. Deleted VoiceID, it's now a port variable.

   Revision 1.17  2007/11/26 14:55:17  wb6ymh
   1. Removed all configuration variables that are now class members.  These
      variables were all port specific.
   2. Added extern "C" around prototypes when built for C++.
   3. Added configuration variables CurrentPort, DefaultMap, and InitPPData.

   Revision 1.16  2007/11/21 17:29:43  wb6ymh
   1. Added configuration variables AudioDongleSN, COS_KeyCode, and ShareIRLP_Port.
   2. Added bMonitor so it's accessable to the set command.

   Revision 1.15  2007/06/29 18:16:16  wb6ymh
   Added QthInHostsFile configuration variable to allow Qth information to
   be optionally included in the host file.  NB: As of today the inclusion
   of Qth information breaks the EchoIRLP scripts.

   Revision 1.14  2007/06/13 00:06:10  wb6ymh
   1. Added ChatPort.
   2. Added thelinkbox configuration variables.

   Revision 1.13  2004/11/27 22:07:23  wb6ymh
   Added new configuration variables UserName, ConfEnable, AvrsEnable,
   AvrsInterval, bEchoIrlpMode, Latitude, Longitude, TxPower, Hatt, AntGain,
   Frequency and CTCSS.

   Revision 1.12  2004/11/22 00:45:44  wb6ymh
   Added IncludeAllHosts.

   Revision 1.11  2003/09/06 19:05:37  wb6ymh
   Added MaxDups.

   Revision 1.10  2003/09/01 15:16:38  wb6ymh
   1. Added configuration variables EmailAdr, CmdPort, IdleTimeout, SF_ReplyPort,
      DirCompression, EventScript, StartupCmd, HelpFile and WelcomeDelay.
   2. Added DoConfigPass() prototype, ConfigPass global.

   Revision 1.9  2003/04/30 21:35:58  wb6ymh
   1. Added configuration varibles Bind2IP, AppName, RunDir, AutoLurkTimeout,
      DefaultAutoLurk, BlabOffTimer, and LogLevel.
   2. Added SetQth() to verify that the default Qth has been edited (sigh).

   Revision 1.8  2002/12/21 18:17:24  wb6ymh
   Added configuration variables MaxPlayWithoutPause, SaveInfoFiles,
   ShowStationInfo and WriteHostFile.

   Revision 1.7  2002/12/18 00:57:55  wb6ymh
   Added WelcomeFile, MaxPlayBackPause, MinPlayBackPause, LogFileRolloverType,
   and AudioTestConf.

   Revision 1.6  2002/11/02 18:13:04  wb6ymh
   Added config variables RTP_Pass, PrivateConference, EnableDiskCommands,
   iLinkDirServer, DnsUpdateInterval, BelchTime, InactiveDirTimeout and
   bQuickStart.

   Revision 1.5  2002/10/25 14:27:30  wb6ymh
   Added iLinkDirServer config variable.

   Revision 1.4  2002/09/29 16:32:55  wb6ymh
   Added configuration variables SysopPass and PauseTime.

   Revision 1.3  2002/09/14 16:49:58  wb6ymh
   Added WorkingDir, RunAsUser, Banner, UserCountEnable, MaxCountEnable, and
   UserCountInLocation variables. Deleted BannerEable and ConnectTo variables.

   Revision 1.2  2002/08/31 23:44:32  wb6ymh
   Added configuration variables DirServerHost1 ...3, AdminPass, BannerEnable,
   SF_Port, SF_Enable, SF_SWL_Only, LinkConferences. Changed DirServerHost to
   an array of NUM_DIRECTORY_SERVERS strings.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#ifndef CONFIGVARS_H_
#define CONFIGVARS_H_

#include "configfile.h"

#ifdef   LINK_BOX
typedef struct SavedToneSpecTAG {
   struct SavedToneSpecTAG *Link;   // NB must be first
   int ID;
   char ToneSpec[1]; // actually variable length
} SavedToneSpec;

extern SavedToneSpec *ToneSpecs;
#endif
#define NUM_DIRECTORY_SERVERS 4

extern char *ConferenceCall;
extern char *ConferencePass;
extern char *ConferenceQth;
extern char *EmailAdr;
extern char *DirServerHost[NUM_DIRECTORY_SERVERS];
extern char *ConferenceID;
extern char *AdminPass;
extern char *SysopPass;
extern char *Banner;
extern char *WorkingDir;
extern char *TempDir;
extern char *ConfigFilePath;
extern char *RunAsUser;
extern char *RTP_Pass;
extern char *WelcomeFile;
extern char *Bind2IP;
extern char *SFBind2IP;
extern char *RTPBind2IP;
extern char *RunDir;
extern char *EventScript;
extern char *CustomHelp;
extern char *UserName;
extern char *FullName;
extern char *RTP_Port;
#ifdef   LINK_BOX
extern char *InfoFile;
extern char *TTS_ExePath;
extern char *AudioDevice;
extern char *Ax25BeaconPath;
extern char *Ax25BeaconText;
extern char *AprsIsComment;
extern char *AprsSymbolTable;
extern char *AprsSymbolCode;
extern char *AprsObjectName;
extern struct config_entry VoipConfigVars[];
#endif

extern int ConfAudioTimeout;
extern int ConfMemberTimeout;
extern int LoginInterval;
extern int StationListInterval;
extern int SDES_Interval;
extern int MaxConferenceClients;
extern struct config_entry ConfigVars[];
extern int SF_Port;
extern int SF_Enable;
extern int bEchoLinkEnabled;
extern int LinkConferences;
extern int SF_SWL_Only;
extern int SB_Enable;
extern int UserCountEnable;
extern int MaxCountEnable;
extern int UserCountInLocation;
extern int PauseTime;
extern int PrivateConference;
extern int EnableDiskCommands;
extern int iLinkDirServer;
extern int DnsUpdateInterval;
extern int BelchTime;
extern int InactiveDirTimeout;
extern int MaxPlayBackPause;
extern int MinPlayBackPause;
extern int AudioTestConf;
extern int MaxPlayWithoutPause;
extern int SaveInfoFiles;
extern int ShowStationInfo;
extern int WriteHostFile;
extern int AutoLurkTimeout;
extern int DefaultAutoLurk;
   #define AUTOLURK_REPEATERS    1
   #define AUTOLURK_LINKS        2
   #define AUTOLURK_HEADSETS     4
   #define AUTOLURK_CONFERENCES  8

extern int BlabOffTimer;
extern int LogLevel;
extern int CmdPort;
extern int ChatPort;
extern int IdleTimeout;
extern int SF_ReplyPort;
extern int DirCompression;
extern int WelcomeDelay;
extern int MaxDups;
extern int ConfEnable;
extern int ConfTextEnable;
extern int ConfCmdEnable;
extern int IncludeAllHosts;
extern int QthInHostsFile;
extern int AvrsEnable;
extern int AvrsInterval;
extern int AprsIsInterval;
extern int Ax25BeaconInterval;
extern int bEchoIrlpMode;
extern int ShowStatusInInfo;
extern int EnableRemoteDTMF;
extern int FullDuplex;
extern int LogIPAddresses;
extern int CompressionType;
extern int AprsIsEnable;
extern int EnableRfPHG;

extern float Latitude;
extern float Longitude;
extern float TxPower;
extern float Haat;
extern float AntGain;
extern float AntDir;
extern float Frequency;
extern float CTCSS;
extern float RptOffset;


#ifdef   LINK_BOX
extern int CtcssDecoderSamples;
extern int DTMFCommandTimeout;
extern int ShareIRLP_Port;
extern char *CallSignSpelling;
extern char *PhoneticFilePath;
extern int InitPPData;
extern int DtmfEncodeDuration;
extern int DtmfEncodePause;
extern int VoipDejitterDelay;
extern char *Phonetic[36];
#endif
// Variable set on command line
extern int bQuickStart;
extern int iLinkDirServer;

typedef struct StartupCmd_TAG {
   struct StartupCmd_TAG *Link;     // NB: must be first
   char  *Cmd;
} StartupCmd;

extern StartupCmd StartupCmds;

#ifdef __cplusplus
extern "C" {
#endif

int DoConfigPass(int Pass);

#ifdef __cplusplus
}
int CheckRTP_PortSpec(char *Spec);

#endif

#endif
