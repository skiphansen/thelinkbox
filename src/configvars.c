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

   $Log: configvars.c,v $
   Revision 1.37  2012/12/09 19:32:13  wb6ymh
   1. Removed old chan_rptdir style Asterisk support variables AsteriskIP,
   AsteriskBind2IP, AsteriskDesc, AsteriskPort, and AsteriskEnable.
   Asterisk is now supported by G.726 and Ulaw RTP connections.
   2. Added ConfTextEnable and ConfCmdEnable.

   Revision 1.36  2012/12/09 18:52:40  wb6ymh
   Corrected bug in LoadFile which caused odd crashes and general bad behavor
   when the configuration variables HelpFile or InfoFile where used.

   Revision 1.35  2011/11/06 14:34:06  wb6ymh
   Added RTP_Port.  NB: not tested in thebridge build, added to fix
   compile error.  Tested on thelinkbox build, probably works here too.

   Revision 1.34  2009/09/14 21:45:32  wb6ymh
   1. Added TempDir.
   2. Added new argument to the StartCmdHandler call.

   Revision 1.33  2009/04/04 20:17:39  wb6ymh
   Added LogIPAddresses.

   Revision 1.32  2008/07/23 14:47:12  wb6ymh
   Added AsteriskEnable, CompressionType, and FullDuplex configuration variables.

   Revision 1.31  2008/07/16 22:40:23  wb6ymh
   Added AsteriskBind2IP, AsteriskIP, AsteriskDesc, EnableRemoteDTMF,
   FullDuplex, and AsteriskPort.

   Revision 1.30  2008/04/11 14:08:24  wb6ymh
   Added missing argument to error logging in CheckEmailAdr.  Fixes segfault
   when email address lacks '@' or '.' characters.

   Revision 1.29  2008/02/26 17:35:47  wb6ymh
   1. Added dmalloc support.
   2. Added AllowADPCM, EchoLinkEnable, and Debug config variables.

   Revision 1.28  2008/02/09 16:53:26  wb6ymh
   Added ShowStatusInInfo.

   Revision 1.27  2008/01/03 16:07:15  wb6ymh
   1. Added SFBind2IP.
   2. Restored default AppName to "tbd".

   Revision 1.26  2007/12/27 18:36:05  wb6ymh
   Added FullName.

   Revision 1.25  2007/12/22 18:58:13  wb6ymh
   Changed calling convention of all SetFuncs (now called AccessFunc).

   Revision 1.24  2007/12/07 22:41:08  wb6ymh
   1. Removed thelinkbox variables, they are now elsewhere.
   2. Added ConfigGetVarPtr() stub.

   Revision 1.23  2007/11/26 15:03:27  wb6ymh
   Undeleted.  Converting to C++ was a bad idea because it would have required
   a C++ compiler to build thebrige as well as thelinkbox.  This file is now
   used for thebrige only.  Configuration variables for thelinkbox are defined
   in ../linkbox/configvars.cpp.

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
#include <string.h>
#include <errno.h>
#ifndef _WIN32
   #include <unistd.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <arpa/inet.h>
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
   #include <sys/stat.h>
   #define CONFIG_FILE_PATH   SYSCON_DIR "/tbd.conf"

   #include <netdb.h>
#else
   #include <direct.h>
   #include <time.h>
   #include <sys\timeb.h>
   #include <winsock2.h>
   #include <sys\stat.h>
   #define CONFIG_FILE_PATH   "tbd.conf"
#endif

#include "configfile.h"
#include "configvars.h"
#include "main.h"
#include "conference.h"
#include "sf.h"
#include "hostfile.h"
#include "rtp.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

// Configuration variables

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
char *RunAsUser;
char *RTP_Pass;
char *WelcomeFile;
char *Bind2IP;
char *SFBind2IP;
char *AppName;
char *RunDir;
char *EventScript;
char *CustomHelp;
char *AntType;
char *UserName;
char *FullName;
char *TempDir;
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
int bAllowADPCM         = 0;
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
int EnableRemoteDTMF    = 0;
int CompressionType     = RTP_PT_GSM;
int FullDuplex          = 0;
int LogIPAddresses      = 0;

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
int bEchoIrlpMode       = 0;
int ConfEnable          = 1;
int ConfTextEnable      = 1;
int ConfCmdEnable       = 1;
int ShowStatusInInfo    = 0;     // Default to disabled

float Latitude         = 200.0;
float Longitude        = 200.0;
float TxPower          = 0.0;
float Haat             = 0.0;
float AntGain          = 0.0;
float AntDir           = 0.0; // 0.0 = omni, heading of zero encoded as 360.0
float Frequency        = 0.0;
float CTCSS            = 0.0;

int AddBannerLine(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int SetWorkingDir(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int SetConferenceCall(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int SetQth(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int CheckEmailAdr(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int StartupCommand(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int LoadFile(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
int AVRS_Validate(void);
int AccessRtpPort(struct config_entry *p, char *Arg, AfCmd Cmd, int x);

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
   { "UserName", "%s", &UserName, 0},
   { "IncludeAllHosts", "%d", &IncludeAllHosts, 0},
   { "QthInHostsFile", "%d", &QthInHostsFile, 0},
   { "Latitude", "%f", &Latitude, 0},
   { "Longitude", "%f", &Longitude, 0},
   { "TxPower", "%f", &TxPower, 0},
   { "Haat", "%f", &Haat, 0},
   { "AntGain", "%f", &AntGain, 0},
   { "Frequency", "%f", &Frequency, 0},
   { "CTCSS", "%f", &CTCSS, 0},
   { "AntType", "%s", &AntType, 0},
   { "AntDir", "%f", &AntDir, 0},
   { "AvrsEnable", "%d", &AvrsEnable, 0},
   { "AvrsInterval", "%d", &AvrsInterval, 0},
   { "ShowStatusInInfo", "%d", &ShowStatusInInfo},
   { "AllowADPCM", "%d", &bAllowADPCM, 0},
   { "EchoLinkEnable", "%d", &bEchoLinkEnabled, 0},
   { "Debug", "%d", &Debug, 0},
   { "EnableRemoteDTMF", "%d", &EnableRemoteDTMF, 0},
   { "CompressionType", "%d", &CompressionType, 0},
   { "FullDuplex", "%d", &FullDuplex, 0},
   { "LogIPAddresses", "%d", &LogIPAddresses, 0},
   { "RTP_Port", "%F", NULL, 0, AccessRtpPort},
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
         ConfigVars[i].AccessFunc(&ConfigVars[i],NULL,AF_SET_DEFAULT,0);
      }
   }

   if(bSet) {
      ConferenceID = strdup(PACKAGE " V " VERSION);
      DirServerHost[0] = strdup("server1.echolink.org");
      AppName = strdup("tbd");
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
         StartCmdHandler(Arg,"StartupCmd");
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
   char *ConfigFile = ConfigFilePath;
   static FILE *fp = NULL;
   int Ret = 0;
    
   ConfigPass = Pass;
   if(ConfigFile == NULL) {
      ConfigFile = CONFIG_FILE_PATH;
   }
   
   if(Pass == 1 && (fp = fopen(ConfigFile,"r")) == NULL) {
      LOG_ERROR(("Fatal error opening configuration file, \"%s\".\n",
                 ConfigFile));
      Ret = CONFIG_OPEN_ERROR;
   }
   else {
      fseek(fp,0,SEEK_SET);
      Ret = read_config(fp,ConfigVars,Pass);
      if(Pass == NUM_CONFIG_PASSES) {
         fclose(fp);
         fp = NULL;
      }
   }

   if(Pass == 1 && Ret == 0) do {
   // Require that the callsign be a link, repeater or conference callsign
      if(ConferenceCall != NULL && 
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
   } while(FALSE);

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

void *ConfigGetVarPtr(struct config_entry *pConfig)
{
   return NULL;
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

