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

   $Log: configvars.cpp,v $
   Revision 1.40  2013/05/13 16:41:06  beta2k

   Added patches from Kristoff - ON1ARF for sysfs GPIO access for PTT

   Revision 1.39  2012/12/09 16:27:41  wb6ymh
   1. Removed old chan_rptdir style Asterisk support variables AsteriskIP,
   AsteriskBind2IP, AsteriskDesc, AsteriskPort, and AsteriskEnable.
   Asterisk is now supported by G.726 and Ulaw RTP connections.
   2. Added ConfTextEnable and ConfCmdEnable.
   3. Corrected bug in LoadFile which caused odd crashes and general bad behavor
      when the configuration variables HelpFile or InfoFile where used.

   Revision 1.38  2010/11/20 15:28:23  wb6ymh
   Correct last commit, the Asterisk configuration variables had not been
   deleted yet in the 0.47 release.

   Revision 1.36  2010/01/07 18:47:53  wb6ymh
   Corrected compile problems on 64 bit platforms in ConfigGetVarPrt.

   Revision 1.35  2009/09/13 19:26:50  wb6ymh
   1. Added DefaultToneLeve, IRLPAnnouncements, ConnectCmd and InConf
      configuration variables.
   2. Added AccessInConf.

   Revision 1.34  2009/07/19 14:10:35  wb6ymh
   Added ConnectAnnouncements to VoIP port configuration variables.

   Revision 1.33  2009/07/07 02:33:06  beta2k
   Removed Aux_PTT_Bit, IdAuxPTT, InvertAuxPTT and replaced with IdNoCTCSS, PTT_CTCSS_Bit, InvertPTT_CTCS

   Revision 1.32  2009/07/03 17:06:35  wb6ymh
   Added TempDir, NoHangtimeOnID, deleted FakeToneBurst.

   Revision 1.31  2009/07/01 14:56:05  wb6ymh
   Added TxTone, NoAudioInLooping, ReducedGain, AnnouncementPriority,
   AudioPriority,  AccessAudioPriority and AccessTxTone.

   Revision 1.30  2009/06/29 13:35:39  beta2k
   Aux_PTT_Bit went missing

   Revision 1.29  2009/06/25 23:52:33  beta2k
   ooops, forgot to add InvertAuxPTT...

   Revision 1.28  2009/06/25 23:50:42  beta2k
   Added IdAuxPTT.

   Revision 1.27  2009/05/25 16:13:50  wb6ymh
   Added EnableDtmfCtcssFilter.

   Revision 1.26  2009/05/22 19:10:48  beta2k
   Added HamlibPortName, HamlibIPAddress, HamlibDaemon, HamlibDaemonArgs, HamlibRigModel, HamlibPortSpeed, HamlibCIVAddr; HamlibTCPLocal, HamlibTCPPort variables for hamlib support

   Revision 1.25  2009/05/22 13:39:17  wb6ymh
   Added CosPollRate to give the ability to reduce reduce the polling rate where
   it matters.  Fixes badly broken audio when an CM108 based interface is used
   on a "Little Blue" patform (677 Mhz Via CPU and USB 1.1 ports).

   Revision 1.24  2009/04/04 15:38:52  wb6ymh
   1. Added VoIP port configuration variables MinPlayBackPause,
      MaxPlayWithoutPause, WelcomeFile, ConnectScript and DisconnectAfterWelcome.
   2. Modified the callsign validation routine to allow bare callsigns when tlb
      is not being used on the EchoLink network.

   Revision 1.23  2009/03/29 16:25:54  wb6ymh
   Added LogIPAddresses.

   Revision 1.22  2009/03/28 15:38:57  wb6ymh
   Merged patches from va3jss:
   1. Renamed SerialDevName to DevName to encompass both Parallel and Serial
      devices
   2. Made SerialDevName an alias for DevName - Note, SerialDevName should be
      considered DEPRICATED and may disappear in the future.

   Revision 1.21  2009/02/14 02:07:22  wb6ymh
   Removed references to InitToneBuf, it's history.

   Revision 1.20  2009/02/13 19:23:08  wb6ymh
   Added ToneBurstTimeout, ToneBurstToneID, FakeToneBurst.

   Revision 1.19  2009/02/09 23:26:49  wb6ymh
   1. Deleted AudioIn8Bit ... got to draw the line somewhere, 8 bit hardware
      seems like a good place.
   2. Added MinAudioWrite, DriverAudioOutDelay, RepeaterPort, ToneBurstFreq,
      ToneBurstMode, ToneBurstMute, and DisableWelcome.

   Revision 1.18  2009/01/03 22:49:46  wb6ymh
   1. Added Ax25BeaconPath,*Ax25BeaconText, AprsIsComment, AprsSymbolTable,
      AprsSymbolCode, AprsObjectName, AprsIsInterval, AprsIsEnable, EnableRfPGH,
      RptOffset, AccessRxFrequency, AccessRxTone, AccessNodeFloat, and
      AccessNodeString.
   2. Deleted unused configuration variable AntDir.
   3. Replaced VoipInMap and VoipOutMap with port configuration variables
      LinkVoipIn and LinkVoipOut.

   Revision 1.17  2008/08/09 15:54:47  wb6ymh
   Added support for port configuration variable ConnectAnnouncements to
   allow connect and disconnect announcements to be enabled/disabled on a
   per port basis.

   Revision 1.16  2008/07/23 15:40:49  wb6ymh
   1. Added configuration varialbles AsteriskIP, AsteriskBind2IP, AsteriskDesc,
      EnableRemoteDTMF, FullDuplex, AsteriskPort, and AsteriskEnable.
   2. Replaced configuration variable AllowADPCM with CompressionType.

   Revision 1.15  2008/06/26 15:32:37  wb6ymh
   1. Lengthened default DTMF times to 60 milliseconds.
   2. Changed format of Debug variable to hex.

   Revision 1.14  2008/05/14 21:04:48  wb6ymh
   1. Added CallSignSpelling, PhoneticFilePath, TxCtcssMethod, RadioCtrlMethod,
      DH_DataBit, and DH_ClockBit.
   2. Deleted EchoCTCSSFreq (replaced by TxTone command).
   3. Deleted RxTone (replaced by RxTone command).

   Revision 1.13  2008/04/27 15:33:08  wb6ymh
   Create NodeTree at start of pass 1 rather than when first node is created.

   Revision 1.12  2008/04/25 00:02:52  wb6ymh
   1. Changed DTMFCommandTimeout to a port variable.
   2. Added AudioChannel, PrefixedTone, PrefixTimeout, DisableRateConversion,
      CtcssRxThreshold, and RxTone.
   3. Added DTMFCommandFile to Voip config variables.
   4. Modified SetConfigDefaults to bypass class members, the class constructor
      handles defaults and the classes haven't been created yet when
      SetConfigDefaults is called.

   Revision 1.11  2008/04/11 14:10:57  wb6ymh
   Added missing argument to error logging in CheckEmailAdr.  Fixes segfault
   when email address lacks '@' or '.' characters.

   Revision 1.10  2008/03/12 21:30:45  wb6ymh
   Drop NodeName from configurable variable list.

   Revision 1.9  2008/03/09 16:58:53  wb6ymh
   1. Deleted DefaultMap, VoipCtone, VoipCtoneMap, VoipCtoneDelay, PortName
      CurrentPort and NumPorts..
   2. Added NodeName, CurrentNode, and COS_Bit.
   3. Changed all map variables from bitmaps to strings.
   4. Changed bAllowADPCM default to enabled.
   5. Added configuration variables for VoIP nodes.
   6. Modifed CreatePort to add newly created ports to avl NodeTree instead
      of the obsolete Ports array.
   7. Updated ConfigGetVarPtr to use new CurrentNode pointer.
   8. Removed obsolete AccessBitMap function.
   9. Added AccessCtoneMap.

   Revision 1.8  2008/03/01 17:30:01  wb6ymh
   Modifications to get thelinkbox to build with current (0.94) thebridge
   sources.

   Revision 1.7  2008/01/28 15:41:21  wb6ymh
   1. Added RxCtcssMethod, RxCTCSSMode, CTCSS_Bit, InvertCTCSS, CTCSS_KeyCode,
      and DtmfCTCSSMode.
   2. Deleted DecodeCTCSS, replaced by RxCtcssMethod.

   Revision 1.6  2008/01/13 17:03:33  wb6ymh
   1. Added RxTimeout, RxTimeoutTone, DTMFCoverTone.
   2. Renamed TimeoutTone to TxTimeoutTone.
   3. Modified AccessCtone to reject IDs of 0.

   Revision 1.5  2007/12/27 17:56:31  wb6ymh
   1. Added configuration variables FullName, MinDtmfInterdigitTime,
      Min1stDigitHoldTime and Dtmf1stDigits.
   2. Removed configuration variable port.
   3. Modified CreatePort to set CurrentPort on all passes.  Previously
      CreatePort did nothing except on pass 1.  Caused class variables set
      on other than pass 1 to be set on wrong port.

   Revision 1.4  2007/12/15 00:52:29  wb6ymh
   Modifed AccessBitMap to prevent it from calling ConfigGetVarPtr for
   AF_SET_DEFAULT since class hasn't been created yet.

   Revision 1.3  2007/12/07 16:53:56  wb6ymh
   Added VoipDejitterDelay and CtoneResetsTimeout.

   Revision 1.2  2007/12/01 05:57:36  wb6ymh
   1. Added configuration variables VoipCtoneDelay, VoipCtoneMap, VoipCtone,
      DtmfEncodeDuration, and DtmEncodePause.
   2. Added port configuration variables Ctone, CtoneMap, CtoneDelay, HangTime
      TxTimeout and TxTimeoutTone.
   3. Added SetToneSpec handler to create a linked list of stored tone specs.

   Revision 1.1  2007/11/26 07:41:55  wb6ymh
   Inital import - previously src/configvars.c.  Moved to linkbox subdirectory
   to avoid needing C++ to compile thebridge.

   Revision 1.21  2007/11/21 17:29:43  wb6ymh
   1. Added configuration variables AudioDongleSN, COS_KeyCode, and ShareIRLP_Port.
   2. Added bMonitor so it's accessable to the set command.

   Revision 1.20  2007/06/29 18:16:15  wb6ymh
   Added QthInHostsFile configuration variable to allow Qth information to
   be optionally included in the host file.  NB: As of today the inclusion
   of Qth information breaks the EchoIRLP scripts.

   Revision 1.19  2007/06/29 17:14:55  wb6ymh
   Edit, COMPILE, then commit!

   Revision 1.18  2007/06/29 17:11:28  wb6ymh
   Corrected CONFIG_FILE_PATH, thelinkbox and thebridge version were swapped.

   Revision 1.17  2007/06/13 00:05:08  wb6ymh
   1. Added ChatPort.
   2. Added thelinkbox configuration variables.
   3. Changed LoadHelp file to LoadFile and made it more generic so it can
      be used to load command files as well as help files.

   Revision 1.16  2004/11/27 22:12:45  wb6ymh
   Added new configuration variables UserName, ConfEnable, AvrsEnable,
   AvrsInterval, bEchoIrlpMode, Latitude, Longitude, TxPower, Hatt, AntGain,
   Frequency and CTCSS.  Added AVRS_Validate() to check entered values.

   Revision 1.15  2004/11/22 00:42:20  wb6ymh
   1. "%f" -> "%F" so we can support floating point numbers in the
      configuration file.
   2. Added configuration variable IncludeAllHosts.
   3. Warn about needing a "-L", "-R", or conference callsign for all
      configurations, not just EchoIRLP.

   Revision 1.14  2004/05/29 17:22:47  wb6ymh
   Add warning about "conference" callsign needing to be a "-R" or "-L" when
   thebridge is configured for use with EchoIRLP.

   Revision 1.13  2003/09/06 19:05:16  wb6ymh
   Added MaxDups.
   Changed default value for StationListInterval to 600 (10 minutes).

   Revision 1.12  2003/09/01 15:15:01  wb6ymh
   1. Added configuration variables EmailAdr, CmdPort, IdleTimeout, SF_ReplyPort,
      DirCompression, EventScript, StartupCmd, HelpFile and WelcomeDelay.
   2. Modified for multipass initialization, primarily to support StartupCmd.

   Revision 1.11  2003/04/30 21:35:58  wb6ymh
   1. Added configuration varibles Bind2IP, AppName, RunDir, AutoLurkTimeout,
      DefaultAutoLurk, BlabOffTimer, and LogLevel.
   2. Added SetQth() to verify that the default Qth has been edited (sigh).

   Revision 1.10  2002/12/21 18:17:24  wb6ymh
   Added configuration variables MaxPlayWithoutPause, SaveInfoFiles,
   ShowStationInfo and WriteHostFile.

   Revision 1.9  2002/12/18 00:56:59  wb6ymh
   Added WelcomeFile, MaxPlayBackPause, MinPlayBackPause, LogFileRolloverType,
   and AudioTestConf.

   Revision 1.8  2002/11/02 18:15:23  wb6ymh
   1. Added config variables RTP_Pass, PrivateConference, EnableDiskCommands,
      iLinkDirServer, DnsUpdateInterval, BelchTime, InactiveDirTimeout and
      bQuickStart.
   2. Added routine to set ConferenceCall so we can check for an unedited
      config file earlier in the game. (Otherwise a bad working directory
      is reported before the default conference call is detected confusing
      lusers).

   Revision 1.7  2002/10/25 14:27:30  wb6ymh
   Added iLinkDirServer config variable.

   Revision 1.6  2002/09/29 16:32:55  wb6ymh
   Added configuration variables SysopPass and PauseTime.

   Revision 1.5  2002/09/14 16:49:21  wb6ymh
   Added WorkingDir, RunAsUser, Banner, UserCountEnable, MaxCountEnable, and
   UserCountInLocation variables. Deleted BannerEable and ConnectTo variables.

   Revision 1.4  2002/09/01 04:55:40  wb6ymh
   Fixed yet another more pointer to pointer bug.... sigh.

   Revision 1.3  2002/09/01 03:05:16  wb6ymh
   Pointers to pointers make my head hurt!  Fixed bug in SetConfigDefaults().
   GCC V 2.95.3 under FreeBSD didn't issue a warning, but GCC V 2.95.3 on
   Linux and Visual C++ did.  Weird!

   Revision 1.2  2002/08/31 23:42:36  wb6ymh
   Added configuration variables DirServerHost1 ...3, AdminPass, BannerEnable,
   SF_Port, SF_Enable, SF_SWL_Only, LinkConferences. Modified SetConfigDefaults()
   to free all "%s" type variables.  Changed DirServerHost to be an array of
   NUM_DIRECTORY_SERVERS strings.  Changed ConfAudioTimeout default to 800
   milliseconds.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <ctype.h>
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
#include <sys/stat.h>
#define CONFIG_FILE_PATH   SYSCON_DIR "/tlb.conf"

#include <netdb.h>

#include "avl.h"
#include "configfile.h"
#include "configvars.h"
#include "main.h"
#include "conference.h"
#include "sf.h"
#include "hostfile.h"
#include "linkbox.h"
#include "nodes.h"
#include "ports.h"
#include "bitstream.h"
#include "g726.h"
#include "voip.h"
#include "rtp.h"

#ifdef HAMLIB_SUPPORT
#include <hamlib/rig.h>
#endif

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define CON_FLG_VOIP    0x80000000  // variable appropriate for Voip nodes

// Configuration variables

char *DummyString = NULL;
SavedToneSpec *ToneSpecs = NULL;

// String variables: Dynamically allocated
char *ConferenceCall;
char *ConferencePass;
char *ConferenceQth;
char *EmailAdr;
char *ConferenceID;
char *DirServerHost[NUM_DIRECTORY_SERVERS];
char *AdminPass;
char *SysopPass;
char *Banner;
char *WorkingDir;
char *TempDir;
char *RunAsUser;
char *RTP_Pass;
char *WelcomeFile;
char *Bind2IP;
char *SFBind2IP;
char *RTPBind2IP;
char *AppName;
char *RunDir;
char *EventScript;
char *CustomHelp;
char *UserName;
char *FullName;
char *InfoFile;
char *TTS_ExePath;
char *DTMFCommandFile;
char *Ax25BeaconPath;
char *Ax25BeaconText;
char *AprsIsComment;
char *AprsSymbolTable;
char *AprsSymbolCode;
char *AprsObjectName;
char *DevName;
char *HamlibPortName;
char *HamlibIPAddress;
char *HamlibDaemon;
char *HamlibDaemonArgs;
char *RTP_Port;

// Integer variables
int ConfAudioTimeout    = 800;   // in milliseconds (> 8 frames)
int ConfMemberTimeout   = 40;    // in seconds (>= 4 missed frames)
int LoginInterval       = 360;   // in seconds
int StationListInterval = 600;   // in seconds
int SDES_Interval       = 10;    // in seconds
int MaxConferenceClients= 8;
int SF_Port             = SF_AUDIO_PORT;
int SF_Enable           = 0;
int bEchoLinkEnabled    = 1;
int LinkConferences     = 1;
int SF_SWL_Only         = 0;
int SB_Enable           = 0;
int UserCountEnable     = 1;
int MaxCountEnable      = 1;
int UserCountInLocation = 1;
int PauseTime           = 0;
int iLinkDirServer      = 0;
int PrivateConference   = 0;
int EnableDiskCommands  = 1;
int DnsUpdateInterval   = 15 * 60;
int BelchTime           = 0;
int InactiveDirTimeout  = 60 * 60 * 24;   // one day

extern int LogFileRolloverType;

int MaxPlayBackPause    = 5;
int MinPlayBackPause    = 0;
int AudioTestConf       = 0;
int MaxPlayWithoutPause = (10 * 60);   // 10 minutes
int SaveInfoFiles       = 0;
int ShowStationInfo     = 0;
int WriteHostFile       = 0;
int AutoLurkTimeout     = (15 * 60);   // 15 minutes
int DefaultAutoLurk     = 0;
int BlabOffTimer        = 0;
int LogLevel            = 0;
int CmdPort             = 0;
int ChatPort            = 0;
int IdleTimeout         = 3 * 60;
int SF_ReplyPort        = SF_AUDIO_PORT;
int DirCompression      = 1;
int WelcomeDelay        = 0;
int MaxDups             = 100;
int IncludeAllHosts     = 0;
int QthInHostsFile      = 0;
int AvrsEnable          = 0;
int AvrsInterval        = (10 * 60);   // 10 minutes
int AprsIsInterval      = (60 * 60);   // 1 hour
int bEchoIrlpMode       = 0;
int ConfEnable          = 1;
int ConfTextEnable      = 1;
int ConfCmdEnable       = 1;
int VoipDejitterDelay   = 500;   // 500 milliseconds
int ShowStatusInInfo    = 2;     // Default to enabled
int EnableRemoteDTMF    = 1;
int FullDuplex          = 0;
int LogIPAddresses     = 0;
int CompressionType     = RTP_PT_GSM;
int AprsIsEnable        = 0;
int EnableRfPHG         = 0;
int EnableHamlib     = 0;
int HamlibRigModel      = 0;
int HamlibPortSpeed     = 9600;
int HamlibCIVAddr    = 0;
int HamlibTCPLocal      = 1;
int HamlibTCPPort    = 4532;

float Latitude         = 200.0;
float Longitude        = 200.0;
float TxPower          = 0.0;
float Haat             = 0.0;
float AntGain          = 0.0;
float AntDir           = 0.0; // 0.0 = omni, heading of zero encoded as 360.0
float Frequency        = 0.0;
float CTCSS            = 0.0;
float RptOffset        = 0.0;

// 300 milliseconds decode time @ 8 Khz sample rate.
int CtcssDecoderSamples = 3200;
int COS_KeyCode = 0;
int ShareIRLP_Port = 0;
Node *CurrentNode = NULL;

char *CallSignSpelling = NULL;
char *PhoneticFilePath = NULL;
char *Phonetic[36];
int InitPPData = -1;

/* ITU spec requires that
   a. Tones with a duration less than 23 ms must be rejected.
   b. Tones with a duration more than 40 ms must be accepted.
   c. An interruption of more than 40 ms must be accepted as a pause.
   d. An interruption of less than 10 ms must be tolerated.

   So Duration should be between 40 and 40+23 to ensure a decode and
   preclude a double decode.
*/

int DtmfEncodeDuration = 60;
int DtmfEncodePause = 60;

int AddBannerLine(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int SetWorkingDir(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int SetConferenceCall(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int SetQth(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int CheckEmailAdr(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int StartupCommand(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int LoadFile(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int CreatePort(struct config_entry *pConf, char *Arg, AfCmd Cmd, int x);
int AVRS_Validate(void);
extern "C" {
int SetToneSpec(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessCtone(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessCtoneMap(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessAudioPriority(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessRxFrequency(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessRxTone(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessTxTone(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessPhoneticFile(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessNodeFloat(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessNodeString(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessInConf(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AccessRtpPort(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
}

#define classoffset(TYPE, MEMBER) ((size_t) &((TYPE *)4)->MEMBER - 4)

#define CLASS_VAR(x,y) \
   (void *) classoffset(class CPort,x),CON_FLG_BASED_VAR | y

#define VOIP_VAR(x,y) \
   (void *) classoffset(class VoipNode,x),CON_FLG_BASED_VAR | y


// Pass 1 variables: processed before initialization
// Pass 2 variables: processed immediately after initialization
// Pass 3 variables: processed after first station list has been downloaded
// Pass 4 variables: processed after second successful directory server login
//                   (approximately one minute after pass 3)
// Pass 5: Manual set command
#define NUM_CONFIG_PASSES  4

struct config_entry ConfigVars[] = {
// Required parameters
   { "ConferenceCall", "%F", &ConferenceCall, CON_FLG_REQUIRED, SetConferenceCall},
   { "ConferencePass", "%s", &ConferencePass, CON_FLG_REQUIRED},
   { "ConferenceQth", "%s", &ConferenceQth, CON_FLG_REQUIRED},
   { "WorkingDir", "%F", &WorkingDir, CON_FLG_REQUIRED | CON_FLG_ONCE_ONLY, SetWorkingDir},
   { "EmailAdr", "%F", &EmailAdr, CON_FLG_REQUIRED, CheckEmailAdr },
// Optional parameters
   { "TempDir", "%s", &TempDir, 0},
   { "ConferenceID", "%s", &ConferenceID, 0},
   { "DirServerHost", "%s", &DirServerHost[0], 0},
   { "DirServerHost1", "%s", &DirServerHost[1], 0},
   { "DirServerHost2", "%s", &DirServerHost[2], 0},
   { "DirServerHost3", "%s", &DirServerHost[3], 0},
   { "RunAsUser", "%s", &RunAsUser, 0},
   { "AdminPass", "%s", &AdminPass, 0},
   { "SysopPass", "%s", &SysopPass, 0},
   { "RTP_Pass", "%s", &RTP_Pass, 0},
   { "WelcomeFile", "%s", &WelcomeFile, 0},
   { "Bind2IP", "%s", &Bind2IP, 0},
   { "SFBind2IP", "%s", &SFBind2IP, 0},
   { "RTPBind2IP", "%s", &RTPBind2IP, 0},
   { "AppName", "%s", &AppName, 0},
   { "RunDir", "%s", &RunDir, 0},
   { "Banner", "%F", &Banner, 0, AddBannerLine},
   { "ConfMemberTimeout", "%d", &ConfMemberTimeout, 0},
   { "ConfAudioTimeout", "%d", &ConfAudioTimeout, 0},
   { "LoginInterval", "%d", &LoginInterval, 0},
   { "StationListInterval", "%d", &StationListInterval, 0},
   { "SDES_Interval", "%d", &SDES_Interval, 0},
   { "MaxConferenceClients", "%d", &MaxConferenceClients, 0},
   { "SF_Port", "%d", &SF_Port, 0},
   { "SF_Enable", "%d", &SF_Enable, 0},
   { "SF_SWL_Only", "%d", &SF_SWL_Only, 0},
   { "SB_Enable", "%d", &SB_Enable, 0},
   { "UserCountEnable", "%d", &UserCountEnable, 0},
   { "MaxCountEnable", "%d", &MaxCountEnable, 0},
   { "UserCountInLocation", "%d", &UserCountInLocation, 0},
   { "LinkConferences", "%d", &LinkConferences, 0},
   { "PauseTime", "%d", &PauseTime, 0},
   { "iLinkDirServer", "%d", &iLinkDirServer, 0},
   { "PrivateConference", "%d", &PrivateConference, 0},
   { "EnableDiskCommands", "%d", &EnableDiskCommands, 0},
   { "DnsUpdateInterval", "%d", &DnsUpdateInterval, 0},
   { "BelchTime", "%d", &BelchTime, 0},
   { "InactiveDirTimeout", "%d", &InactiveDirTimeout, 0},
   { "LogFileRolloverType", "%d", &LogFileRolloverType, 0},
   { "MaxPlayBackPause", "%d", &MaxPlayBackPause, 0},
   { "MinPlayBackPause", "%d", &MinPlayBackPause, 0},
   { "AudioTestConf", "%d", &AudioTestConf, 0},
   { "MaxPlayWithoutPause", "%d", &MaxPlayWithoutPause, 0},
   { "SaveInfoFiles", "%d", &SaveInfoFiles, 0},
   { "ShowStationInfo", "%d", &ShowStationInfo, 0},
   { "WriteHostFile", "%d", &WriteHostFile, 0},
   { "AutoLurkTimeout", "%d", &AutoLurkTimeout, 0},
   { "DefaultAutoLurk", "%d", &DefaultAutoLurk, 0},
   { "BlabOffTimer", "%d", &BlabOffTimer, 0},
   { "LogLevel", "%d", &LogLevel, 0},
   { "CmdPort", "%d", &CmdPort, 0},
   { "ChatPort", "%d", &ChatPort, 0},
   { "IdleTimeout", "%d", &IdleTimeout, 0},
   { "SF_ReplyPort", "%d", &SF_ReplyPort, 0},
   { "DirCompression", "%d", &DirCompression, 0},
   { "EventScript", "%s", &EventScript, 0},
   { "StartupCmd", "%F", NULL, CON_FLG_PASS4, StartupCommand},
   { "HelpFile", "%F", &CustomHelp, 0, LoadFile},
   { "WelcomeDelay", "%d", &WelcomeDelay, 0},
   { "MaxDups", "%d", &MaxDups, 0},
   { "ConfEnable", "%d", &ConfEnable, 0},
   { "ConfTextEnable", "%d", &ConfTextEnable, 0},
   { "ConfCmdEnable", "%d", &ConfCmdEnable, 0},
   { "UserName", "%s", &UserName, 0},
   { "FullName", "%s", &FullName, 0},
   { "IncludeAllHosts", "%d", &IncludeAllHosts, 0},
   { "QthInHostsFile", "%d", &QthInHostsFile, 0},
   { "Latitude", "%f", &Latitude, 0},
   { "Longitude", "%f", &Longitude, 0},
   { "TxPower", "%F", &TxPower, 0, AccessNodeFloat},
   { "Haat", "%F", &Haat, 0, AccessNodeFloat},
   { "AntGain", "%F", &AntGain, 0, AccessNodeFloat},
   { "Frequency", "%F", &Frequency, 0, AccessNodeFloat},
   { "CTCSS", "%F", &CTCSS, 0, AccessNodeFloat},
   { "AntDir", "%F", &AntDir, 0, AccessNodeFloat},
   { "Ax25BeaconText", "%F", &Ax25BeaconText, 0, AccessNodeString},
   { "AvrsEnable", "%d", &AvrsEnable, 0},
   { "AvrsInterval", "%d", &AvrsInterval, 0},
   { "InfoFile", "%F", &InfoFile, 0, LoadFile},
   { "CreatePort", "%F", NULL, CON_FLG_ALL_PASSES, CreatePort},
   { "TTS_ExePath", "%s", &TTS_ExePath, 0, 0},
   { "ShareIRLP_Port", "%d", &ShareIRLP_Port, 0, 0},
   { "CallSignSpelling", "%s", &CallSignSpelling, 0, 0},
   { "PhoneticFilePath", "%s", &PhoneticFilePath, 0, AccessPhoneticFile},
   { "InitPPData", "%d", &InitPPData},
   { "DtmfEncodeDuration", "%d", &DtmfEncodeDuration},
   { "DtmfEncodePause", "%d", &DtmfEncodePause},
   { "ToneSpec", "%s", &DummyString,0,SetToneSpec},
   { "VoipDejitterDelay", "%d", &VoipDejitterDelay},
   { "ShowStatusInInfo", "%d", &ShowStatusInInfo},
   { "EchoLinkEnable", "%d", &bEchoLinkEnabled, 0},
   { "Debug", "%x", &Debug, 0},
   { "EnableRemoteDTMF", "%d", &EnableRemoteDTMF, 0},
   { "FullDuplex", "%d", &FullDuplex, 0},
   { "CompressionType", "%d", &CompressionType, 0},
   { "AprsIsEnable", "%d", &AprsIsEnable, 0},
   { "RptOffset", "%f", &RptOffset, 0},
   { "AprsIsInterval", "%d", &AprsIsInterval, 0},
   { "AprsIsComment", "%s", &AprsIsComment, 0},
   { "EnableRfPHG", "%d", &EnableRfPHG, 0},
   { "AprsSymbolTable", "%s", &AprsSymbolTable, 0},
   { "AprsSymbolCode", "%s", &AprsSymbolCode, 0},
   { "AprsObjectName", "%s", &AprsObjectName, 0},
   { "LogIPAddresses", "%d", &LogIPAddresses, 0},
   { "RTP_Port", "%F", NULL, 0, AccessRtpPort},

   { "SerialDevName", "%s", CLASS_VAR(DevName, 0)},
   { "DevName", "%s", CLASS_VAR(DevName,0)},
   { "VoxThreshold", "%d", CLASS_VAR(VoxThreshold,0)},
   { "VoxHoldTime", "%d", CLASS_VAR(VoxHoldTime,0)},
   { "VoxTripDelay", "%d", CLASS_VAR(VoxTripDelay,0)},
   { "TxKeyMethod", "%d", CLASS_VAR(TxKeyMethod,CON_FLG_REQUIRED)},
// ON1ARF
   { "GpioSysClassId", "%d", CLASS_VAR(GpioSysClassId,0)},
   { "RxCosMethod", "%d", CLASS_VAR(RxCosMethod,CON_FLG_REQUIRED)},
   { "DtmfMethod", "%d", CLASS_VAR(DtmfMethod,CON_FLG_REQUIRED)},
   { "RxCtcssMethod", "%d", CLASS_VAR(RxCtcssMethod,0)},
   { "TxCtcssMethod", "%d", CLASS_VAR(TxCtcssMethod,0)},
   { "RadioCtrlMethod", "%d", CLASS_VAR(RadioCtrlMethod,0)},
   { "SerialBaudRate", "%d", CLASS_VAR(SerialBaudRate,0)},
   { "DTMFPollTime", "%d", CLASS_VAR(DTMFPollTime,0)},
   { "InvertPTT", "%d", CLASS_VAR(InvertPTT,0)},
   { "InvertPTT_CTCSS", "%d", CLASS_VAR(InvertPTT_CTCSS,0)},
   { "InvertCOS", "%d", CLASS_VAR(InvertCOS,0)},
   { "CWToneFreq", "%d", CLASS_VAR(CWToneFreq,0)},
   { "CWToneLevel", "%d", CLASS_VAR(CWToneLevel,0)},
   { "CWSpeed", "%d", CLASS_VAR(CWSpeed,0)},
   { "CWAutoJam", "%d", CLASS_VAR(CWAutoJam,0)},
   { "IdInterval", "%d", CLASS_VAR(IdInterval,0)},
   { "IdNoCTCSS", "%d", CLASS_VAR(IdNoCTCSS,0)},
   { "CWId", "%s", CLASS_VAR(CWId,0)},
   { "VoiceID", "%s", CLASS_VAR(VoiceID,0)},
   { "VoiceIDIdleTime", "%d", CLASS_VAR(VoiceIDIdleTime,0)},
   { "HalfDuplexRF", "%d", CLASS_VAR(HalfDuplexRF,0)},
   { "PCMRate", "%d", CLASS_VAR(PCMRate,0), 0},
   { "CTCSSLevel", "%d", CLASS_VAR(CTCSSLevel,0)},
   { "twist", "%d", CLASS_VAR(Twist,0)},
   { "reversetwist", "%d", CLASS_VAR(ReverseTwist,0)},
   { "CtcssDecoderSamples", "%d", CLASS_VAR(CtcssDecoderSamples,0)},
   { "MuteDTMF", "%d", CLASS_VAR(MuteDTMF,0)},
   { "DTMFCommandTimeout", "%d", CLASS_VAR(DTMFCommandTimeout,0)},
   { "AudioDevice", "%s", CLASS_VAR(AudioDevice,CON_FLG_REQUIRED)},
   { "Monitor", "%d", CLASS_VAR(bMonitor,0)},
   { "COS_KeyCode", "%d", CLASS_VAR(COS_KeyCode,0)},
   { "AudioDongleSN", "%s", CLASS_VAR(AudioDongleSN,0)},
   { "AudioDongleAdr", "%s", CLASS_VAR(AudioDongleAdr,0)},
   { "DiscFeed", "%d", CLASS_VAR(bDiscFeed,0)},
   { "UngatedRx", "%d", CLASS_VAR(bUngatedRx,0)},
   { "PTT_Bit", "%d", CLASS_VAR(PTT_Bit,0)},
   { "PTT_CTCSS_Bit", "%d", CLASS_VAR(PTT_CTCSS_Bit,0)},
   { "Ctone", "%d", CLASS_VAR(Ctone,CON_FLG_PASS2),AccessCtone},
   { "CtoneMap", "%F", CLASS_VAR(CtoneMap,0),AccessCtoneMap},
   { "CtoneDelay", "%d", CLASS_VAR(CtoneDelay,0)},
   { "HangTime", "%d", CLASS_VAR(HangTime,0)},
   { "TxTimeout", "%d", CLASS_VAR(TxTimeout,0)},
   { "RxTimeout", "%d", CLASS_VAR(RxTimeout,0)},
   { "TxTimeoutTone", "%d", CLASS_VAR(TxTimeoutTone,CON_FLG_PASS2),AccessCtone},
   { "RxTimeoutTone", "%d", CLASS_VAR(RxTimeoutTone,CON_FLG_PASS2),AccessCtone},
   { "CtoneResetsTimeout", "%d", CLASS_VAR(CtoneResetsTimeout,0)},
   { "MinDtmfInterdigitTime", "%d", CLASS_VAR(MinDtmfInterdigitTime,0)},
   { "Min1stDigitHoldTime", "%d", CLASS_VAR(Min1stDigitHoldTime,0)},
   { "Dtmf1stDigits", "%s", CLASS_VAR(Dtmf1stDigits,0)},
   { "DtmfCoverTone", "%d", CLASS_VAR(DtmfCoverTone,0)},
   { "RxCTCSSMode", "%d", CLASS_VAR(RxCTCSSMode,0)},
   { "CTCSS_Bit", "%d", CLASS_VAR(CTCSS_Bit,0)},
   { "COS_Bit", "%d", CLASS_VAR(COS_Bit,0)},
   { "DH_DataBit", "%d", CLASS_VAR(DH_DataBit,0)},
   { "DH_ClockBit", "%d", CLASS_VAR(DH_ClockBit,0)},
   { "InvertCTCSS", "%d", CLASS_VAR(InvertCTCSS,0)},
   { "CTCSS_KeyCode", "%d", CLASS_VAR(CTCSS_KeyCode,0)},
   { "DtmfCTCSSMode", "%d", CLASS_VAR(DtmfCTCSSMode,0)},
   { "AudioChannel", "%d", CLASS_VAR(AudioChannel,0)},
   { "DTMFPrefix", "%s", CLASS_VAR(DTMFPrefix,0)},
   { "PrefixedTone", "%d", CLASS_VAR(PrefixedTone,0)},
   { "PrefixTimeout", "%d", CLASS_VAR(PrefixTimeout,0)},
   { "DisableRateConversion", "%d", CLASS_VAR(DisableRateConversion,0)},
   { "CtcssRxThreshold", "%d", CLASS_VAR(CtcssRxThreshold,0)},
   { "RxTone", "%F", CLASS_VAR(RxTone,CON_FLG_PASS2),AccessRxTone},
   { "TxTone", "%F", CLASS_VAR(RxTone,CON_FLG_PASS2),AccessTxTone},
   { "ConnectAnnouncements", "%d", CLASS_VAR(ConnectAnnouncements,0)},
   { "DTMFCommandFile", "%F", &DTMFCommandFile, 0, LoadCommandFile},
   { "RxDeemphasis", "%d", CLASS_VAR(RxDeemphasis,0)},
   { "TxPreemphasis", "%d", CLASS_VAR(TxPreemphasis,0)},
   { "TxCtcssFilter", "%d", CLASS_VAR(TxCtcssFilter,0)},
   { "Ax25BeaconInterval", "%d", CLASS_VAR(Ax25BeaconInterval, 0)},
   { "Ax25ToneLevel", "%d", CLASS_VAR(Ax25ToneLevel, 0)},
   { "Ax25BeaconText", "%s", CLASS_VAR(Ax25BeaconText, 0)},
   { "Ax25BeaconPath", "%s", CLASS_VAR(Ax25BeaconPath, 0)},
   { "Ax25TxDelay", "%d", CLASS_VAR(Ax25TxDelay, 0)},
   { "Ax25BeaconAPRS", "%d", CLASS_VAR(Ax25BeaconAPRS, 0)},
   { "LinkVoipIn", "%d", CLASS_VAR(LinkVoipIn, 0)},
   { "LinkVoipOut", "%d", CLASS_VAR(LinkVoipOut, 0)},
   { "MinAudioWrite", "%d", CLASS_VAR(MinAudioWrite, 0)},
   { "DriverAudioOutDelay", "%d", CLASS_VAR(DriverAudioOutDelay, 0)},
   { "RepeaterPort", "%d", CLASS_VAR(RepeaterPort, 0)},
   { "ToneBurstFreq", "%d", CLASS_VAR(ToneBurstFreq, 0)},
   { "ToneBurstMode", "%d", CLASS_VAR(ToneBurstMode, 0)},
   { "ToneBurstMute", "%d", CLASS_VAR(ToneBurstMute, 0)},
   { "ToneBurstTimeout", "%d", CLASS_VAR(ToneBurstTimeout, 0)},
   { "ToneBurstToneID", "%d", CLASS_VAR(ToneBurstToneID, 0)},
   { "EnableHamlib", "%d", CLASS_VAR(EnableHamlib, 0)},
   { "HamlibPortName", "%s", CLASS_VAR(HamlibPortName, 0)},
   { "HamlibRigModel", "%d", CLASS_VAR(HamlibRigModel, 0)},
   { "HamlibPortSpeed", "%d", CLASS_VAR(HamlibPortSpeed, 0)},
   { "HamlibCIVAddr", "%d", CLASS_VAR(HamlibCIVAddr, 0)},
   { "HamlibTCPLocal", "%d", CLASS_VAR(HamlibTCPLocal, 0)},
   { "HamlibTCPPort", "%d", CLASS_VAR(HamlibTCPPort, 0)},
   { "HamlibIPAddress", "%s", CLASS_VAR(HamlibIPAddress, 0)},
   { "HamlibDaemon","%s",CLASS_VAR(HamlibDaemon,0)},
   { "HamlibDaemonArgs","%s",CLASS_VAR(HamlibDaemonArgs,0)},
   { "CosPollRate", "%d", CLASS_VAR(CosPollRate,0)},
   { "EnableDtmfCtcssFilter", "%d", CLASS_VAR(EnableDtmfCtcssFilter,0)},
   { "ReducedGain", "%d", CLASS_VAR(ReducedGain,0)},
   { "AnnouncementPriority", "%d", CLASS_VAR(AnnouncementPriority,0)},
   { "AudioPriority", "%F", CLASS_VAR(AudioPriority,0),AccessAudioPriority},
   { "NoAudioInLooping", "%d", CLASS_VAR(NoAudioInLooping,0)},
   { "NoHangtimeOnID", "%d", CLASS_VAR(NoHangtimeOnID,0)},
   { "DefaultToneLevel", "%d", CLASS_VAR(DefaultToneLevel,0)},
   { "IRLPAnnouncements", "%d", CLASS_VAR(IRLPAnnouncements,0)},
   { "" }   /* end of table marker */
};

// Configuration variables for Voip nodes.
struct config_entry VoipConfigVars[] = {
// Required parameters
   { "DtmfMethod", "%d", VOIP_VAR(DtmfMethod,0)},
   { "twist", "%d", VOIP_VAR(Twist,0)},
   { "reversetwist", "%d", VOIP_VAR(ReverseTwist,0)},
   { "MuteDTMF", "%d", VOIP_VAR(MuteDTMF,0)},
   { "DTMFCommandTimeout", "%d", VOIP_VAR(DTMFCommandTimeout,0)},
   { "Ctone", "%d", VOIP_VAR(Ctone,CON_FLG_PASS2),AccessCtone},
   { "CtoneMap", "%s", VOIP_VAR(CtoneMap,CON_FLG_PASS2),0},
   { "CtoneDelay", "%d", VOIP_VAR(CtoneDelay,0)},
   { "TxTimeout", "%d", VOIP_VAR(TxTimeout,0)},
   { "RxTimeout", "%d", VOIP_VAR(RxTimeout,0)},
   { "TxTimeoutTone", "%d", VOIP_VAR(TxTimeoutTone,CON_FLG_PASS2),AccessCtone},
   { "RxTimeoutTone", "%d", VOIP_VAR(RxTimeoutTone,CON_FLG_PASS2),AccessCtone},
   { "CtoneResetsTimeout", "%d", VOIP_VAR(CtoneResetsTimeout,0)},
   { "MinDtmfInterdigitTime", "%d", VOIP_VAR(MinDtmfInterdigitTime,0)},
   { "Min1stDigitHoldTime", "%d", VOIP_VAR(Min1stDigitHoldTime,0)},
   { "Dtmf1stDigits", "%s", VOIP_VAR(Dtmf1stDigits,0)},
   { "DtmfCoverTone", "%d", VOIP_VAR(DtmfCoverTone,0)},
   { "DTMFPrefix", "%s", VOIP_VAR(DTMFPrefix,0)},
   { "PrefixedTone", "%d", VOIP_VAR(PrefixedTone,0)},
   { "PrefixTimeout", "%d", VOIP_VAR(PrefixTimeout,0)},
   { "TxCtcssFilter", "%d", VOIP_VAR(TxCtcssFilter,0)},
   { "DisableWelcome", "%d", VOIP_VAR(DisableWelcome,0)},
   { "MinPlayBackPause", "%d", VOIP_VAR(MinPlayBackPause, 0)},
   { "MaxPlayWithoutPause", "%d", VOIP_VAR(MaxPlayWithoutPause, 0)},
   { "WelcomeFile", "%s", VOIP_VAR(WelcomeFile, 0)},
   { "ConnectScript", "%s", VOIP_VAR(ConnectScript, 0)},
   { "DisconnectAfterWelcome", "%d", VOIP_VAR(bDisconnectAfterWelcome, 0)},
   { "DTMFCommandFile", "%F", &DTMFCommandFile, 0, LoadCommandFile},
   { "ReducedGain", "%d", VOIP_VAR(ReducedGain,0)},
   { "AnnouncementPriority", "%d", VOIP_VAR(AnnouncementPriority,0)},
   { "AudioPriority", "%F", VOIP_VAR(AudioPriority,0),AccessAudioPriority},
   { "ConnectAnnouncements", "%d", VOIP_VAR(ConnectAnnouncements,0)},
   { "DefaultToneLevel", "%d", VOIP_VAR(DefaultToneLevel,0)},
   { "ConnectCmd", "%F", NULL, CON_FLG_PASS2, StartupCommand},
   { "IRLPAnnouncements", "%d", VOIP_VAR(IRLPAnnouncements,0)},
   { "InConf", "%F", NULL, CON_FLG_PASS2, AccessInConf},
   { "" }   /* end of table marker */
};


void SetConfigDefaults(int bSet)
{
   int i;
   void *var_ptr = NULL;

   for(i = 0; ConfigVars[i].var_name[0] != 0; i++) {
      if(strcmp(ConfigVars[i].var_type,"%s") == 0) {
         if(ConfigVars[i].Flags & CON_FLG_BASED_VAR) {
            if(ConfigPass == 0) {
            // Prior to first pass, class variables haven't been created yet
               continue;
            }
         }
         else {
            var_ptr = ConfigVars[i].var_ptr;
         }

         if(*((char **) var_ptr) != NULL)
         {
            free(*((char **) var_ptr));
            *((char **) var_ptr) = NULL;
         }
      }
      else if(strcmp(ConfigVars[i].var_type,"%F") == 0) {
         if(!(ConfigVars[i].Flags & CON_FLG_BASED_VAR)) {
            ConfigVars[i].AccessFunc(&ConfigVars[i],NULL,AF_SET_DEFAULT,0);
         }
      }
   }

   if(bSet) {
      ConferenceID = strdup(PACKAGE " V " VERSION);
      DirServerHost[0] = strdup("server1.echolink.org");
      AppName = strdup("tlb");
      RunDir = strdup("/var/run/");
   }
}

int AddBannerLine(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   char *Temp;

   switch(Cmd) {
      case AF_SET_VAR:
         if(Banner != NULL) {
         // Banner already has line(s)
            Temp = Banner;
            Banner = (char *) malloc(strlen(Temp)+strlen(Arg)+2);
            if(Banner != NULL) {
               strcpy(Banner,Temp);
               strcat(Banner,"\r");
               strcat(Banner,Arg);
            }
            free(Temp);
         }
         else {
         // First banner line
            Banner = strdup(Arg);
         }
         break;

      case AF_SET_DEFAULT:
         if(Banner != NULL) {
            free(Banner);
            Banner = NULL;
         }
         break;

      case AF_DISPLAY_VAR:
         break;
   }

   return 0;
}

int SetConferenceCall(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;

   switch(Cmd) {
      case AF_SET_VAR:
         if(ConferenceCall != NULL) {
            free(ConferenceCall);
         }

         ConferenceCall = strdup(Arg);

         if(strcmp(ConferenceCall,"W1AW") == 0) {
            LOG_ERROR(("Come on now, your call isn't really W1AW is it!\n"));
            LOG_ERROR(("Please read the documentation, edit tbd.conf and try again.\n"));
            Ret = ERR_CONFIG_FILE;
         }
         p->Flags |= CON_FLG_SET;
         break;

      case AF_SET_DEFAULT:
         if(ConferenceCall != NULL) {
            free(ConferenceCall);
            ConferenceCall = NULL;
         }
         break;

      case AF_DISPLAY_VAR:
         strncpy(Arg,ConferenceCall,x);
         break;
   }
   return Ret;
}

int SetQth(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;

   switch(Cmd) {
      case AF_SET_VAR:
         if(ConferenceQth!= NULL) {
            free(ConferenceQth);
         }

         ConferenceQth = strdup(Arg);

         if(strcmp(ConferenceQth,"Newington, Ct.") == 0) {
            LOG_ERROR(("Come on now, you don't live in \"Newington, Ct.\" do you!\n"));
            LOG_ERROR(("Please read the documentation, edit tbd.conf and try again.\n"));
            Ret = ERR_CONFIG_FILE;
         }
         p->Flags |= CON_FLG_SET;
         break;

      case AF_SET_DEFAULT:
         if(ConferenceQth != NULL) {
            free(ConferenceQth);
            ConferenceQth = NULL;
         }
         break;

      case AF_DISPLAY_VAR:
         strncpy(Arg,ConferenceQth,x);
         break;
   }

   return Ret;
}

int SetWorkingDir(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;

   switch(Cmd) {
      case AF_SET_VAR:
         if(WorkingDir != NULL) {
            free(WorkingDir);
         }
         WorkingDir = strdup(Arg);
         if(WorkingDir != NULL) {
            if(chdir(WorkingDir)) {
               LOG_ERROR(("Unable to change to working directory \"%s\", %s",
                          WorkingDir,Err2String(errno)));
               free(WorkingDir);
               WorkingDir = NULL;
               Ret = ERR_WORKING_DIR;
            }
            p->Flags |= CON_FLG_SET;
         }
         break;

      case AF_SET_DEFAULT:
         if(WorkingDir != NULL) {
            free(WorkingDir);
            WorkingDir = NULL;
         }
         break;

      case AF_DISPLAY_VAR:
         strncpy(Arg,WorkingDir,x);
         break;
   }

   return Ret;
}

int CheckEmailAdr(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;

   switch(Cmd) {
      case AF_SET_VAR:
         if(EmailAdr != NULL) {
            free(EmailAdr);
         }

         EmailAdr = strdup(Arg);

         if(strcmp(EmailAdr,"w1aw@arrl.net") == 0) {
            LOG_ERROR(("Come on now, your email address isn't \"w1aw@arrl.net\" "
                       "is it?\n"));
            Ret = ERR_CONFIG_FILE;
         }
         else if(!strchr(EmailAdr,'@') || !strchr(EmailAdr,'.')) {
            LOG_ERROR(("Error: Invalid email address \"%s\".\n",EmailAdr));
            Ret = ERR_CONFIG_FILE;
         }

         if(Ret != 0) {
            LOG_ERROR(("The email address is optional, please either delete the "
                       "entry\n"));
            LOG_ERROR(("or provide a valid email address.\n"));
         }
         p->Flags |= CON_FLG_SET;
         break;

      case AF_SET_DEFAULT:
         if(EmailAdr != NULL) {
            free(EmailAdr);
            EmailAdr = NULL;
         }
         break;

      case AF_DISPLAY_VAR:
         break;
   }
   return Ret;
}

int StartupCommand(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   switch(Cmd) {
      case AF_SET_VAR:
         if(strcmp(p->var_name,"ConnectCmd") == 0) {
            StartCmdHandler(Arg,CurrentNode->NodeName);
         }
         else {
            StartCmdHandler(Arg,(char *) "StartupCmd");
         }
         break;

      case AF_SET_DEFAULT:
         break;

      case AF_DISPLAY_VAR:
         break;
   }
   return 0;
}

int LoadFile(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   struct stat FileStats;
   FILE *fp;
   char *cp;
   char **pBuf;
   int Ret = 0;

   pBuf = (char **) p->var_ptr;
   if(p->var_ptr != NULL && *pBuf != NULL) {
      free(*pBuf);
      *pBuf = NULL;
   }

   switch(Cmd) {
      case AF_SET_VAR:
         if(stat(Arg,&FileStats) != 0 || (fp = fopen(Arg,"r")) == NULL) {
            LOG_ERROR(("Error: unable to open %s \"%s\" %s",p->var_name,Arg,
                       Err2String(errno)));
            Ret = ERR_CONFIG_FILE;
         }
         else if((*pBuf = (char *) malloc(FileStats.st_size)) != NULL) {
            if(fread(*pBuf,FileStats.st_size,1,fp) != 1) {
               LOG_ERROR(("Error reading help file \"%s\", %s",Arg,
                          Err2String(errno)));
               free(*pBuf);
               *pBuf = NULL;
               Ret = ERR_CONFIG_FILE;
            }
            else {
            // Convert the '\n' end of line convention to '\r' used by EchoLink
               cp = (char *) p->var_ptr;
               while((cp = strchr(cp,'\n')) != NULL) {
                  *cp++ = '\r';
               }
            }
            fclose(fp);
         }
         break;

      case AF_SET_DEFAULT:
         break;

      case AF_DISPLAY_VAR:
         break;
   }
   return Ret;
}


// Configuration passes:
// 1 - first pass (duh!)
// 2 - Initialization complete.
// 3 - Station list downloaded from EchoLink directory server.
// 4 - Station list downloaded a second time from EchoLink directory server.
// 5 - Manual variable set
int DoConfigPass(int Pass)
{
   const char *ConfigFile = ConfigFilePath;
   FILE *fp = NULL;
   int Ret = 0;

   do {
      if(ConfigFile == NULL) {
         ConfigFile = CONFIG_FILE_PATH;
      }

      if((fp = fopen(ConfigFile,"r")) == NULL) {
         LOG_ERROR(("Fatal error opening configuration file, \"%s\".\n",
                    ConfigFile));
         Ret = CONFIG_OPEN_ERROR;
         break;
      }

      if(NodeTree == NULL) {
         NodeTree = avl_create(NodeCompare,NULL,NULL);
      }

      Ret = read_config(fp,ConfigVars,Pass);

      if(Pass == 1 && Ret == 0) {
      // Require that the callsign be a link, repeater or conference callsign
         if(bEchoLinkEnabled && LoginInterval != 0 &&
            ConferenceCall != NULL &&
            ConferenceCall[0] != '*' &&
            strstr(ConferenceCall,"-R") == NULL &&
            strstr(ConferenceCall,"-L") == NULL)
         {  // Must be a bare callsign ... not allowed.
            LOG_ERROR(("Error: Invalid ConferenceCall \"%s\".\n"
                       "ConferenceCall must be a callsign folllowed by \"-R\", "
                       "or \"-L\" or \n"
                       "be an assigned conference name.\n",
                       ConferenceCall));
            Ret = ERR_CONFIG_FILE;
            break;
         }

         if(AvrsEnable && !AVRS_Validate()) {
            Ret = ERR_CONFIG_FILE;
            break;
         }

         if(SF_Port != SF_ReplyPort) {
            bEchoIrlpMode = TRUE;
         }
      }
   } while(FALSE);

   if(fp != NULL) {
      fclose(fp);
   }

   return Ret;
}


// We're connected to the EchoLink network see if we have any AVRS
// info to send
int AVRS_Validate()
{
   struct hostent *pHost;
   int Ret = FALSE;

// We require Longitude, Latitude, and Frequency to be set
   if(ConferenceCall[0] == '*') {
      LOG_ERROR(("Error: AVRS reporting not available for conferences, "
                 "AVRS disabled.\n"));
   }
   else if(Longitude > 180.0 || Longitude < -180.0) {
      LOG_ERROR(("Error: Invalid Longitude, AVRS disabled.\n"));
   }
   else if(Latitude > 90.0 || Latitude < -90.0) {
      LOG_ERROR(("Error: Invalid Latitude, AVRS disabled.\n"));
   }
   else if(AntDir > 360.0 || AntDir < 0.0) {
      LOG_ERROR(("Error: Invalid Antenna direction, AVRS disabled.\n"));
   }
   else if(CTCSS < 0.0 || CTCSS > 229.1) {
      LOG_ERROR(("Error: Invalid CTCSS frequency, AVRS disabled.\n"));
   }
   else {
      Ret = TRUE;

   // Prime the DNS cache so we'll have the IP address later when we need it
      pHost = GetHostByName("aprs.echolink.org");
   }

   AvrsEnable = Ret;

   return Ret;
}

int CreatePort(struct config_entry *pConf, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;
   CPort *pPort;
   char *cp;
   struct config_entry *p;

   if(Cmd == AF_SET_VAR ) do {
      if((cp = strchr(Arg,' ')) != NULL) {
      // Truncate port name @ first space
         *cp = 0;
      }

      if(ConfigPass == 1) {
         if(CurrentNode != NULL) {
         // Starting a new port, check and clear the loaded bits for
            p = ConfigVars;
            while(p->var_name[0]) {
               if(p->Flags & CON_FLG_BASED_VAR) {
                  if(p->Flags & CON_FLG_REQUIRED && !(p->Flags & CON_FLG_SET)) {
                  // required variable was not set
                     Ret = ERR_CONFIG_FILE;
                     LOG_ERROR(("Error: Required configuration variable \"%s\""
                                " is not set for port %s.\n",p->var_name,
                                CurrentNode->NodeName));
                     break;
                  }
                  p->Flags &= ~CON_FLG_SET;
               }
               p++;
            }
         }

         if(Ret != 0) {
            break;
         }

         if(FindNode(Arg) != NULL) {
            LOG_ERROR(("Error: Port \"%s\" has already been created\n",Arg));
            Ret = ERR_CONFIG_FILE;
            break;
         }

         if((pPort = new CPort) == NULL) {
            LOG_ERROR(("%s#%d: new failed.\n",__FUNCTION__,__LINE__));
            Ret = ERR_CONFIG_FILE;
            break;
         }
         pPort->NodeName = strdup(Arg);
         CurrentNode = pPort;
         avl_insert(NodeTree,pPort);
      }
      else {
      // Not pass 1, just lookup the port
         if((CurrentNode = FindNode(Arg)) == NULL) {
            LOG_ERROR(("%s#%d: Internal error, couldn't find %s!\n",__FUNCTION__,
                       __LINE__,Arg));
         }
      }
   } while(FALSE);

   return Ret;
}


void *ConfigGetVarPtr(struct config_entry *pConfig)
{
   void *p = NULL;

   if(CurrentNode == NULL) {
      LOG_ERROR(("Error: Attempt to access node variable \"%s\" before a node"
                 "was created\n",pConfig->var_name));
   }
   else {
      p = (char *) CurrentNode + ((char *) pConfig->var_ptr - (char *) NULL);
   }

   return p;
}

// argument: ToneSpec which must include id <number>
int SetToneSpec(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   char *cp;
   int ToneId;
   SavedToneSpec *pSpec = ToneSpecs;
   SavedToneSpec *pLastSpec = (SavedToneSpec *) &ToneSpecs;

   if(Cmd == AF_SET_VAR) do {
      if((cp = strstr(Arg,"ID")) == NULL && (cp = strstr(Arg,"id")) == NULL) {
         LOG_ERROR(("Error: ID not found in ToneSpec \"%s\", ignored.\n",Arg));
         break;
      }
      if(sscanf(&cp[3],"%d",&ToneId) != 1 || ToneId <= 0) {
         LOG_ERROR(("Error: invalid ID in ToneSpec \"%s\", ignored.\n",Arg));
         break;
      }
   // Remove any existing tonespecs with this ID
      while(pSpec != NULL) {
         if(pSpec->ID == ToneId) {
         // Match, delete the old one
            pLastSpec->Link = pSpec->Link;
            free(pSpec);
            break;
         }
         pSpec = pSpec->Link;
      }

      pSpec = (SavedToneSpec *) malloc(sizeof(SavedToneSpec) + strlen(Arg));
      if(pSpec != NULL) {
         strcpy(pSpec->ToneSpec,Arg);
         pSpec->ID = ToneId;
         pSpec->Link = ToneSpecs;
         ToneSpecs = pSpec;
      }
   } while(FALSE);

   return 0;
}

int AccessCtone(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int ID;
   int Ret = 0;
   int *var_ptr;

   if(p->Flags & CON_FLG_BASED_VAR) {
      var_ptr = (int *) ConfigGetVarPtr(p);
   }
   else {
      var_ptr = (int *) p->var_ptr;
   }

   switch(Cmd) {
      case AF_SET_VAR:
         if(sscanf(Arg,"%d",&ID) != 1) {
            Ret = ERR_CONFIG_FILE;
         }
         else if(ID != 0 && FindToneSpec(ID) == NULL) {
            LOG_ERROR(("Error: ToneSpec ID %d is not defined, ignored.\n",ID));
            Ret = ERR_CONFIG_FILE;
         }
         else {
            *var_ptr = ID;
         }
         break;

      case AF_SET_DEFAULT:
         *var_ptr = 0;
         break;

      case AF_DISPLAY_VAR:
         snprintf(Arg,x,"%d",*var_ptr);
         break;
   }

   return Ret;
}

int AccessCtoneMap(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;

   switch(Cmd) {
      case AF_SET_VAR:
         if(CurrentNode != NULL) {
            if(CurrentNode->CtoneMap != NULL) {
               free(CurrentNode->CtoneMap);
            }
            CurrentNode->CtoneMap = strdup(Arg);
            CurrentNode->UpdateCtoneFlags();
         }
         else {
            LOG_ERROR(("%s: called with CurrentNode == NULL\n",__FUNCTION__));
         }
         break;

      case AF_SET_DEFAULT:
         if(CurrentNode != NULL) {
            if(CurrentNode->CtoneMap != NULL) {
               free(CurrentNode->CtoneMap);
            }
            CurrentNode->CtoneMap = NULL;
         }
         break;

      case AF_DISPLAY_VAR:
         if(CurrentNode != NULL) {
            snprintf(Arg,x,"%s",CurrentNode->CtoneMap);
         }
         else {
            LOG_ERROR(("%s: called with CurrentNode == NULL\n",__FUNCTION__));
         }
         break;
   }

   return Ret;
}

int AccessAudioPriority(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Priority;
   int Ret = 0;
   int *var_ptr;

   if(p->Flags & CON_FLG_BASED_VAR) {
      var_ptr = (int *) ConfigGetVarPtr(p);
   }
   else {
      var_ptr = (int *) p->var_ptr;
   }

   switch(Cmd) {
      case AF_SET_VAR:
         if(sscanf(Arg,"%d",&Priority) != 1) {
            Ret = ERR_CONFIG_FILE;
         }
         else if(Priority < LOW_AUDIO_PRI || Priority > HIGH_AUDIO_PRI) {
            LOG_ERROR(("Invalid priority, must be 0, 1 or 2\n"));
            Ret = ERR_CONFIG_FILE;
         }
         else {
            *var_ptr = Priority;
         // Update audiopriority of existing links
            LinkedNode *pLink = CurrentNode->pTransmitters;
            while(pLink != NULL) {
               pLink->AudioPriority = Priority;
               pLink = pLink->pNextTx;
            }
         }
         break;

      case AF_SET_DEFAULT:
         *var_ptr = MED_AUDIO_PRI;
         break;

      case AF_DISPLAY_VAR:
         snprintf(Arg,x,"%d (%s)",*var_ptr,PriorityText[*var_ptr]);
         break;
   }

   return Ret;
}

void FreePhoneticList()
{
   for(int i = 0; i < 36; i++) {
      if(Phonetic[i] != NULL) {
         free(Phonetic[i]);
         Phonetic[i] = NULL;
      }
   }
}

int AccessRxTone(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;
   CPort *pPort;
   CText ErrMsg;

   if(CurrentNode == NULL) {
      LOG_ERROR(("%s: called with CurrentNode == NULL\n",__FUNCTION__));
   }
   else if((pPort = dynamic_cast<CPort *> (CurrentNode)) == NULL) {
      LOG_ERROR(("%s: Error node %s does support RxTone\n",__FUNCTION__,
                 CurrentNode->NodeName));
   }
   else {
      switch(Cmd) {
         case AF_SET_VAR:
            Ret = pPort->SetRxTone(Arg,&ErrMsg);
            if(Ret != 0) {
               LOG_ERROR(("%s: %s\n",__FUNCTION__,ErrMsg.Buf));
            }
            break;

         case AF_SET_DEFAULT:
            break;

         case AF_DISPLAY_VAR:
            snprintf(Arg,x,"%s",(char *)pPort->RxToneDesc);
            break;
      }
   }

   return Ret;
}

int AccessTxTone(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;
   CPort *pPort;
   CText ErrMsg;

   if(CurrentNode == NULL) {
      LOG_ERROR(("%s: called with CurrentNode == NULL\n",__FUNCTION__));
   }
   else if((pPort = dynamic_cast<CPort *> (CurrentNode)) == NULL) {
      LOG_ERROR(("%s: Error node %s does support RxTone\n",__FUNCTION__,
                 CurrentNode->NodeName));
   }
   else {
      switch(Cmd) {
         case AF_SET_VAR:
            Ret = pPort->SetTxTone(Arg,&ErrMsg);
            if(Ret != 0) {
               LOG_ERROR(("%s: %s\n",__FUNCTION__,ErrMsg.Buf));
            }
            break;

         case AF_SET_DEFAULT:
            break;

         case AF_DISPLAY_VAR:
            snprintf(Arg,x,"%s",(char *)pPort->TxToneDesc);
            break;
      }
   }

   return Ret;
}


int AccessPhoneticFile(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;
   char Line[120];
   FILE *fp;
   char *cp;

   switch(Cmd) {
      case AF_SET_VAR:
         if(STRCMP(Arg,"none") == 0) {
         // Special case
            if(PhoneticFilePath != NULL) {
               free(PhoneticFilePath);
            }
            PhoneticFilePath = strdup(Arg);
            FreePhoneticList();
         }
         else if((fp = fopen(Arg,"r")) == NULL) {
            LOG_ERROR(("%s: couldn't open \"%s\" - %s.\n",__FUNCTION__,Arg,
                       strerror(errno)));
            Ret = ERR_CONFIG_FILE;
         }
         else {
            if(PhoneticFilePath != NULL) {
               free(PhoneticFilePath);
            }
            PhoneticFilePath = strdup(Arg);
            FreePhoneticList();
            for(int i = 0; i < 36; i++) {
               for( ; ; ) {
                  if(fgets(Line,sizeof(Line),fp) == NULL) {
                  // This should never happen if we have enough lines
                     LOG_ERROR(("%s: \"%s\" don't have enough lines.\n",
                                __FUNCTION__,Arg));
                     Ret = ERR_CONFIG_FILE;
                     break;
                  }
                  if(!is_comment(Line)) {
                     if((cp = strchr(Line,'\n')) != NULL) {
                        *cp = 0;
                     }
                     Phonetic[i] = strdup(Line);
                     break;
                  }
               }
               if(Ret != 0) {
                  break;
               }
            }
            fclose(fp);
         }
         break;

      case AF_SET_DEFAULT:
         if(PhoneticFilePath != NULL) {
            free(PhoneticFilePath);
            PhoneticFilePath = NULL;
         }
         FreePhoneticList();
         break;

      case AF_DISPLAY_VAR:
         snprintf(Arg,x,"%s",PhoneticFilePath == NULL ? "none" : PhoneticFilePath);
         break;
   }

   return Ret;
}


// Access floating point APRS numbers.  If CurrentNode is not set then
// store the configuration variables in the global (backward compatibility)
// otherwise store them in the node
// Variables: TxPower, Haat, AntGain, AntDir, Frequency, CTCSS, RptOffset
int AccessNodeFloat(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;
   float *pFloat = NULL;
   CPort *pPort = dynamic_cast<CPort *> (CurrentNode);

   if(pPort != NULL) {
   // ugly to say the least!
      if(p->var_ptr == &TxPower){
         pFloat = &pPort->TxPower;
      }
      else if(p->var_ptr == &Haat){
         pFloat = &pPort->Haat;
      }
      else if(p->var_ptr == &AntGain){
         pFloat = &pPort->AntGain;
      }
      else if(p->var_ptr == &AntDir){
         pFloat = &pPort->AntDir;
      }
      else if(p->var_ptr == &Frequency){
         pFloat = &pPort->Frequency;
      }
      else if(p->var_ptr == &CTCSS){
         pFloat = &pPort->CTCSS;
      }
      else if(p->var_ptr == &RptOffset){
         pFloat = &pPort->RptOffset;
      }
   }
   else {
      pFloat = (float *)p->var_ptr;
   }

   switch(Cmd) {
      case AF_SET_VAR:
         sscanf(Arg,"%f",pFloat);
         pFloat = (float *)p->var_ptr;
         if(*pFloat == 0.0) {
         // global is not set, set it now
            sscanf(Arg,"%f",pFloat);
         }
         break;

      case AF_SET_DEFAULT:
         *pFloat = 0.0;
         break;

      case AF_DISPLAY_VAR:
         snprintf(Arg,x,"%f",*pFloat);
         break;
   }

   return Ret;
}

int AccessNodeString(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = 0;
   char **pcp = NULL;
   CPort *pPort = dynamic_cast<CPort *> (CurrentNode);

   if(pPort != NULL) {
   // ugly to say the least!
      if(p->var_ptr == &Ax25BeaconText){
         pcp = &pPort->Ax25BeaconText;
      }
   }
   else {
      pcp = (char **)p->var_ptr;
   }

   switch(Cmd) {
      case AF_SET_VAR:
         if(*pcp != NULL) {
            free(*pcp);
         }
         *pcp = strdup(Arg);
         break;

      case AF_SET_DEFAULT:
         if(*pcp != NULL) {
            free(*pcp);
            *pcp = NULL;
         }
         break;

      case AF_DISPLAY_VAR:
         snprintf(Arg,x,"%s",*pcp);
         break;
   }

   return Ret;
}


// Only called for VoIP clients
int AccessInConf(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int val;
   int Ret = 0;
   VoipNode *pVoipNode = dynamic_cast<VoipNode*> (CurrentNode);

   switch(Cmd) {
      case AF_SET_VAR:
         if(sscanf(Arg,"%d",&val) != 1) {
            Ret = ERR_CONFIG_FILE;
         }
         else if(val < 0 || val > 1) {
            LOG_ERROR(("Invalid, must be 0 or 1\n"));
            Ret = ERR_CONFIG_FILE;
         }
         else {
            pVoipNode->mCC->bInConf = val;
         }
         break;

      case AF_SET_DEFAULT:
         break;

      case AF_DISPLAY_VAR:
         snprintf(Arg,x,"%d",pVoipNode->mCC->bInConf ? 1 : 0);
         break;
   }

   return Ret;
}

int CheckRTP_PortSpec(char *Spec)
{
   int Ret = ERR_CONFIG_FILE; // assume the worse
   char *cp = strchr(Spec,'-');
   int FirstPort;
   int LastPort;

   do {
      if(sscanf(Spec,"%d",&FirstPort) != 1) {
         LOG_ERROR(("Error:  Invalid RTP_Port \"%s\"\n",Spec));
         break;
      }
      else if(FirstPort & 1) {
         LOG_ERROR(("Error: First RTP port must be even\n"));
         break;
      }

      if(cp != NULL) {
      // Range
         if(sscanf(&cp[1],"%d",&LastPort) != 1) {
            LOG_ERROR(("Error: Invalid RTP_Port \"%s\"\n",Spec));
            break;
         }
         else if(!(LastPort & 1)) {
            LOG_ERROR(("Error: Last RTP port must be odd\n"));
            break;
         }
         else if(LastPort <= FirstPort) {
            LOG_ERROR(("Error: The last port must larger the first port\n"));
            break;
         }
      }
      Ret = 0;
   } while(FALSE);


   return Ret;
}


// Rtp_port = 0   // disable
// Rtp_port = <evenport>
// Rtp_port = <evenport>-<oddport> for a range
int AccessRtpPort(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = ERR_CONFIG_FILE; // assume the worse

   switch(Cmd) {
      case AF_SET_VAR:
         if((Ret = CheckRTP_PortSpec(Arg)) != 0) {
            break;
         }
         if(RTP_Port != NULL) {
            free(RTP_Port);
         }
         RTP_Port = strdup(Arg);

         printf("ConfigPass: %d\n",ConfigPass);
         if(ConfigPass == MANUAL_SET) {
            RtpInit();
         }
         break;

      case AF_SET_DEFAULT:
         Ret = 0;
         break;

      case AF_DISPLAY_VAR:
         snprintf(Arg,x,"%s",RTP_Port != NULL ? RTP_Port : "not set");
         Ret = 0;
         break;
   }

   return Ret;
}

