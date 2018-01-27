/* Copyright (C) 2003 GFRN systems

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

   $Log: tbdcmd.c,v $
   Revision 1.19  2009/09/13 23:14:36  wb6ymh
   Added support for .rxlevel command.

   Revision 1.18  2009/04/04 16:29:21  wb6ymh
   1. Added code to set port from the TLB_CMDPORT environment variable if present.
   2. Added code to set the maximum wait time for play emumlation fomr the
      TLB_MAX_PLAY environment variable if present.  The default timeout is
      30 seconds.

   Revision 1.17  2009/02/08 16:43:46  wb6ymh
   Added missing break in -x switch in EmulateImike.

   Revision 1.16  2008/08/15 17:30:27  wb6ymh
   Added include for sys/types.h before sys/socket.h as needed by OSX.
   Thanks to Wyatt KJ4CTD for the fix.

   Revision 1.15  2008/07/23 15:30:35  wb6ymh
   1. Added full duplex (-x) and uLaw (-n) support to the iMike enulation.
   2. Added dtmfregen script emulation profile.

   Revision 1.14  2008/05/28 18:04:50  wb6ymh
   1. TLBPORT -> TLB_PORT.
   2. Added code to Read/save history file to ~./.tlbcmd_history rather than
      from the current directory.

   Revision 1.13  2008/04/12 14:27:06  wb6ymh
   Moved TlbPort assignment past the last declaration at the top of main.
   GCC 4.x allows declarations anywhere within a block in C mode, but most other
   complilers (and earlier versions of GCC) do not.

   Revision 1.12  2008/03/08 18:10:51  wb6ymh
   Added TLBPORT support for imike, ispeaker and play emulations.

   Revision 1.11  2008/03/08 06:40:18  wb6ymh
   1. Modified EmulatePlay to add a -c (send completion status) optoin to the
      say commadn and wait for an TBD_SAY_COMPLETE to be returned before exiting.
   2. Added code to prefix commands with port <TLBPORT>; when the TLBPORT env
      variable is set. This variable is set to the port that issued the
      command that caused the script to be executed.  In many cases external
      scripts can be blissfully ignorant of the port selection and just rely
      on the man behind the curtain to do the right thing.
   3. Added the -b (bare) command line switch to disable the automatic port
      selection described above.
   4. Removed the signal handler for SIGTERM and SIGINT for the play emulation
      so play can be aborted by SIGTERM.

   Revision 1.10  2008/02/26 18:16:16  wb6ymh
   1. Modified code to check if stdout is a tty and bypass readline if not.
   2. Added check for NULL returned by readline() to avoid segfault when
      user uses ^D to exit.

   Revision 1.9  2007/12/14 22:44:04  wb6ymh
   1. Added GNU readline support.
   2. Added support for the -p <port> iMike switch to EmulateImike.
   3. Corrected log filename generation (thelinkbox builds).
   4. Modified code to put logs into $TLB_HOME.

   Revision 1.8  2007/12/07 23:15:23  wb6ymh
   Added emulation of play, imike, and ispeaker.

   Revision 1.7  2007/07/02 13:43:35  wb6ymh
   Corrected C99 C declaration mixed with code in tbdcmd.c that's not accepted
   by pre C89 compilers.  This is the first time I've run into code gcc 4.x
   liked that other compilers don't, it's usually the other way around.

   Revision 1.6  2007/06/29 14:30:22  wb6ymh
   1. Modified code to use 5199 by default instead of 5198 when involked as
      tbdchat, tlbchat, or chat.
   2. Modified all hard coded references to "tbdcmd" to use the current
      program alias.
   3. Added code to suppress stations lists received from conference bridges.
   4. Added -S command line switch to disable station list suppression logic.

   Revision 1.5  2006/08/21 16:32:33  wb6ymh
   Added code to set rlim_max to RLIM_INFINITY.  Fixes ages old "Unable to
   enable core dumps" warning which occured on recent versions of the Linux
   kernel.

   Revision 1.4  2005/01/09 00:00:30  wb6ymh
   1. Added -t option to enable display of a timestamp before each line
      of chat text.

   2. Fix typo and add "warning" to error message about failure to
      enable core (not "code") dumps.  Apparently Fedora has changed
      something as this always fails with "Invalid argument".

   3. Modified new tbdcmd misfeature that suppressed "error" codes of
      zero to only suppress them when tbdcmd is run interactively.

   4. Corrected bug caused by uninitialized select timeout structure in
      tbdcmd.  This bug only occurred on a few systems, thanks to ke4tte
      for providing access to one where it did so I could debug it.

   Revision 1.3  2004/12/01 14:30:27  wb6ymh
   1. Don't watch stdin unless it's a tty!
   2. Don't suppress "error" codes of zero unless we're running interactively.

   Revision 1.2  2004/11/29 00:38:50  wb6ymh
   1. Added timeout to avoid hanging if thebridge doesn't respond.
   2. Added support for text chat.

   Revision 1.1  2003/08/16 14:27:27  wb6ymh
   Initial import: command line utility to send commands to thebridge.

*/

#include "common.h"

#ifndef _WIN32
   // FreeBSD, Linux, etc..
   #include <stdio.h>
#ifdef HAVE_LIBREADLINE
   #include <readline/readline.h>
   #include <readline/history.h>
#endif   
   #include <stdarg.h>
   #ifdef HAVE_UNISTD_H
      #include <unistd.h>
   #endif
   
   #ifdef STDC_HEADERS
      #include <stdlib.h>
   #endif
   
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
   #include <errno.h>
   
   #ifdef HAVE_STRINGS_H
      #include <string.h>
   #endif
   #include <assert.h>
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <sys/resource.h>
   #include <arpa/inet.h>
   #include <string.h>
   #include <signal.h>
   #include <termios.h>
#else
   // Windoze
   #include <stdio.h>
   #include <winsock2.h>
   #include <string.h>
   #include <conio.h>
   #include <time.h>
#endif

#include "ilink.h"
#include "tbd.h"

#ifndef  FALSE
#define  FALSE    0
#endif

#ifndef  TRUE
#define  TRUE  !FALSE
#endif

#undef  LOG_NORM
#define LOG_NORM(x)  Log x

#undef  LOG_ERROR
#define LOG_ERROR(x) Log x

#define RESP_TO      10    // wait for 10 seconds from responses from tbd

typedef union {
   struct sockaddr s;
   struct sockaddr_in i;
   #define ADDR   i.sin_addr.s_addr
   #define PORT   i.sin_port
} IPAdrUnion;


#ifdef _WIN32
// Windoze
   WSADATA wsad;
#endif

int bQuiet = 0;
int bSlient = 0;
int bTimeStamp = 0;
int bAllowStationList = FALSE;
int Port = ILINK_RTP_PORT;
int bUseReadline = FALSE;

#ifdef LINK_BOX
#define PROMPT "tlb> "

int bShutdown = FALSE;
int LogFileRolloverType = -1;  // no rollover, single logfile
FILE *LogFp = NULL;

static char *LogNames[] = {
   "sun.log",
   "mon.log",
   "tue.log",
   "wed.log",
   "thr.log",
   "fri.log",
   "sat.log"
};

static char *MonthNames[] = {
   "Jan",
   "Feb",
   "Mar",
   "Apr",
   "May",
   "Jun",
   "Jul",
   "Aug",
   "Sep",
   "Oct",
   "Nov",
   "Dec"
};
#else
#define PROMPT "tbd> "
#endif

char *AppName = NULL;
char *TlbPort = NULL;
char *HistoryPath = NULL;
int MaxPlayWait = 30;

#ifndef HAVE_GETOPT
// globals needed by getopt
int optind = 1;
int optopt;
char *optarg;
int getopt(int argc,char **argv,char *opts);
#endif

char *TimeT2String(time_t time);
int IsStationList(char *Line);

#ifdef LINK_BOX
void SigHandler(int Signal);
int EmulateImike(char **argv,int argc);
int EmulateIspeaker(char **argv,int argc);
int EmulatePlay(char **argv,int argc);
int EmulateDtmfRegen(char **argv,int argc);

#endif

void SetTerminalMode(int mode);

void Usage(char *Command)
{
   printf("Usage: %s [-b] [-p<port>] [-q] [-s] [-S] [-t] [commands] \n",Command);
   printf("\t-b - Bare, do not prepend port <TLBPORT>; to commands.\n");
   printf("\t-p - Specify port (default %d).\n",Port);
   printf("\t-q - Quiet, numeric result code only.\n");
   printf("\t-s - Slient, suppress banner.\n");
   printf("\t-S - Allow conference bridge's station list (default is suppress).\n");
   printf("\t-t - Add timestamps to text chat messages.\n");
}

int main(int argc, char* argv[])
{
   int Ret = 0;
   int Option;
   SOCKET   MySocket;
   IPAdrUnion HisAdr;
   char  Line[1024];
   char  *cp;
   char  *cp1;
   int   BytesRxed;
   int   BytesSent;
   int   FirstArg = 1;
   int   ArgCount = argc;
   int   WritePoint;
   int   Wrote;
   int   i;
   fd_set ReadFDset;
   struct timeval SelTO;
   int   Selret;
   int   bResponseWait = FALSE;
   int   bDisplayPrompt = TRUE;
   int   bNeedNewline = FALSE;
   int   bInteractive = FALSE;
   int   bCleanupCmd = FALSE;
   time_t   TimeNow;
   time_t   TimeCmdSent;
   int      bChat = FALSE;
   int bBare = FALSE;
   int Response = 0;

#ifndef _WIN32
   struct rlimit rlim;

// On some systems core dumps are disabled by default, 
// enable them so we can debug this thing !

   rlim.rlim_cur = RLIM_INFINITY;
   rlim.rlim_max = RLIM_INFINITY;
   if(setrlimit(RLIMIT_CORE, &rlim)) {
      perror("Warning - Unable to enable core dumps: ");
   }
#else
   /* Initialize Winsock*/
   if(WSAStartup(0x0101, &wsad) != 0) {
      printf("Fatal error: unable to initialize Winsock\n");
      exit(3);
   }
#endif

   TlbPort = getenv("TLB_PORT");
   AppName = strrchr(argv[0],PATH_SEP);

   if(AppName != NULL) {
      AppName++;
   }
   else {
      AppName = argv[0];
   }

   if(strcmp(AppName,"tbdchat") == 0 || 
      strcmp(AppName,"tlbchat") == 0 ||
      strcmp(AppName,"chat") == 0)
   {
      Port = ILINK_RTCP_PORT;
      bChat = TRUE;
   }

   if((cp = getenv("TLB_CMDPORT")) != NULL) {
      if(sscanf(cp,"%d",&Port) != 1) {
         printf("Warning: invalid TLB_CMDPORT \"%s\" ignored.\n",cp);
      }
   }

   if((cp = getenv("TLB_MAX_PLAY")) != NULL) {
      if(sscanf(cp,"%d",&MaxPlayWait) != 1) {
         printf("Warning: invalid TLB_MAX_PLAY \"%s\" ignored.\n",cp);
      }
   }

   if(isatty(fileno(stdin))) {
      bUseReadline = TRUE;
   }

#ifdef LINK_BOX
   if(strcmp(AppName,"imike") == 0) {
      return EmulateImike(argv,argc);
   }
   else if(strcmp(AppName,"ispeaker") == 0) {
      return EmulateIspeaker(argv,argc);
   }
   else if(strcmp(AppName,"play") == 0) {
      return EmulatePlay(argv,argc);
   }
   else if(strcmp(AppName,"dtmfregen") == 0) {
      return EmulateDtmfRegen(argv,argc);
   }
#endif

   while((Option = getopt(argc, argv, "bp:qtsS")) != -1 && !Ret) {
      FirstArg++;
      ArgCount--;
      switch(Option) {
         case 'b':   // bare
            bBare = TRUE;
            break;

         case 'p':   // port
            if(sscanf(optarg,"%d",&Port) != 1 || Port <= 0 || Port > 0xffff) {
               printf("%s: Error invalid port number \"%s\"\n",AppName,optarg);
               exit(3);
            }
            break;

         case 'q':   // quiet 
            bQuiet = TRUE;
            break;

         case 's':   // slient
            bSlient = TRUE;
            break;

         case 'S':   // Allow station list
            bAllowStationList = TRUE;
            break;

         case 't':   // Timestamps
            bTimeStamp = TRUE;
            break;

         case '?':
            Usage(AppName);
            exit(3);
      }
   }

   if(ArgCount == 1) {
      bInteractive = TRUE;
#ifdef HAVE_LIBREADLINE
      if(bUseReadline) {
         char *HistoryFilename = ".tlbcmd_history";
         char *HomeDir = getenv("HOME");
         int PathLen = strlen(HistoryFilename) + 1;

         if(HomeDir != NULL) {
            PathLen += strlen(HomeDir) + 1;
         }

         HistoryPath = malloc(PathLen);

         if(HistoryPath != NULL) {
            if(HomeDir != NULL) {
               strcpy(HistoryPath,HomeDir);
               strcat(HistoryPath,"/");
               strcat(HistoryPath,HistoryFilename);
            }
            else {
               strcpy(HistoryPath,HistoryFilename);
            }
            read_history(HistoryPath);
         }
      }
#endif
   }

   if(!bQuiet && !bSlient && !Ret) {
      printf("%s Version " VERSION " compiled "__DATE__  " " __TIME__"\n",
             AppName);
   }

   if(ArgCount > 1) {
      WritePoint = 0;
      for(i = 0; i < ArgCount -1 ; i++) {
         Wrote = snprintf(&Line[WritePoint],sizeof(Line)-WritePoint-1,"%s%s",
                          WritePoint == 0 ? "" : " ",argv[FirstArg+i]);
         if(Wrote > 0 && WritePoint + Wrote < sizeof(Line)) {
            WritePoint += Wrote;
         }
         else {
         // Buffer overflow
            break;
         }
      }
      Line[WritePoint] = 0;
   }
   else {
      Line[0] = 0;
   }

   if((MySocket = socket(AF_INET,SOCK_DGRAM,0)) == SOCKET_ERROR) {
      printf("Socket() failed.\n");
   }
   else {
      HisAdr.i.sin_family = AF_INET;
      HisAdr.PORT = htons((u_short) Port);
      HisAdr.ADDR = inet_addr("127.0.0.1");

      SetTerminalMode(0);

      do {
         if(bInteractive && bDisplayPrompt && Line[0] == 0) {
            bDisplayPrompt = FALSE;
            printf(bChat ? "chat> " : PROMPT);
            fflush(stdout);
         }

         if(strlen(Line) > 0) {
            if(!bBare && !bInteractive && TlbPort != NULL) {
            // prefix commands with tlb port selection from the environment
               char *Temp = strdup(Line);
               snprintf(Line,sizeof(Line),"port %s;%s",TlbPort,Temp);
               free(Temp);
            }
            BytesSent = sendto(MySocket,Line,strlen(Line)+1,0,
                               &HisAdr.s,sizeof(HisAdr));

            if(BytesSent == SOCKET_ERROR) {
               Ret = ERROR_CODE;
               printf("Error: sendto() failed (%d)\n",Ret);
               break;
            }
            Line[0] = 0;
            if(bCleanupCmd) {
               break;
            }
            else {
               bResponseWait = TRUE;
               time(&TimeCmdSent);
            }
         }

         FD_ZERO(&ReadFDset);
         FD_SET(MySocket,&ReadFDset);

#ifdef _WIN32
      // Windoze can't select on standard in 'cuz it ain't so standard
      // so wake up every 100 milliseconds to poll the keyboard

         SelTO.tv_sec = 0;
         SelTO.tv_usec = 100000;
         Selret = select(FD_SETSIZE,&ReadFDset,NULL,NULL,&SelTO);
#else
         if(bInteractive) {
            FD_SET((SOCKET) fileno(stdin),&ReadFDset);
         }
         if(bResponseWait) {
         // Wait a second for the response
            SelTO.tv_sec = 1;
            SelTO.tv_usec = 0;
            Selret = select(FD_SETSIZE,&ReadFDset,NULL,NULL,&SelTO);
         }
         else {
            Selret = select(FD_SETSIZE,&ReadFDset,NULL,NULL,NULL);
         }
#endif
         time(&TimeNow);


#ifndef _WIN32
         if(FD_ISSET(fileno(stdin),&ReadFDset))
#else
         if(kbhit())
#endif
         {
#ifdef HAVE_LIBREADLINE
            if(bUseReadline) {
               if(bNeedNewline) {
                  bNeedNewline = FALSE;
                  printf("\n%s",bChat ? "chat> " : PROMPT);
               }
               SetTerminalMode(2);
               rl_already_prompted = 1;
               cp = readline(bChat ? "chat> " : PROMPT);
               if(cp != NULL) {
                  add_history(cp);
                  memcpy(Line,cp,sizeof(Line));
                  free(cp);
                  Line[sizeof(Line)-1] = 0;
               }
               else {
               // EOF read by readline
                  printf("\n");
                  Line[0] = 0;
               }
               SetTerminalMode(1);
            } 
            else {
#endif
            if(bNeedNewline) {
               bNeedNewline = FALSE;
               printf("\n%s",bChat ? "chat> " : PROMPT);
            }
            if(fgets(Line,sizeof(Line),stdin) == NULL) {
               break;
            }
#ifdef HAVE_LIBREADLINE
            }
#endif

            if((cp = strchr(Line,'\n')) != NULL) {
               *cp = 0;
            }

            bDisplayPrompt = TRUE;
            bResponseWait = TRUE;
            time(&TimeCmdSent);
            if(strlen(Line) == 0) {
            // exit on empty line
               if(Response != TBD_RX_LEVEL_RPT) {
                  break;
               }
               else {
               // We're receiving continous level reports, send a final
               // .rxlevel command to silence them! (You think this is a
               // kludge?  You betcha !)
                  bCleanupCmd = TRUE;
                  strcpy(Line,".rxlevel");
                  continue;
               }
            }
         }
         
         if(FD_ISSET(MySocket,&ReadFDset)) {
            BytesRxed = recv(MySocket,Line,sizeof(Line)-1,0);
            if(BytesRxed == SOCKET_ERROR) {
               Ret = ERROR_CODE;
               printf("Error: recv() failed (%d)\n",Ret);
               break;
            }

            Line[BytesRxed] = 0;
            bDisplayPrompt = TRUE;

            Ret = atoi(Line);
            Response = Ret;
            if(Ret != TBD_CHAT_TEXT && Ret != TBD_RX_LEVEL_RPT) {
               bResponseWait = FALSE;
            }

            if(bQuiet) {
               if((cp = strchr(Line,'\n')) != NULL) {
               // Only want a single line, truncate the output
                  cp[1] = 0;
               }
            }
            if(bInteractive) {
            // Interactive mode, overwrite the prompt
               printf("\r      \r");
            }

         // Remove any extra line feeds at the end of the line.  Thebridge's
         // output contains a single <lf>, but text chat messages have two.

            if((cp = strrchr(Line,'\n')) != NULL && cp[-1] == '\n') {
            // Only want a single linefeed, remove the second one
               *cp = 0;
            }

            time(&TimeNow);

            if(Ret == TBD_CHAT_TEXT && bTimeStamp) {
               printf("%s: ",TimeT2String(TimeNow));
            }
            
            if(Ret == TBD_CHAT_TEXT_SENT && bInteractive) {
            // Interactive mode and this is just an ack for a message we
            // sent, don't display it
            }
            else if(!bAllowStationList && bInteractive && IsStationList(Line)) {
            // Suppress station list.
            }
            else if(bInteractive && Ret == TBD_RX_LEVEL_RPT &&
                    (cp = strchr(Line,'\n')) != NULL) 
            {
            // Rx levels, supress return code and remove newline
               cp++;
               if((cp1 = strchr(cp,'\n')) != NULL) {
                  *cp1 = 0;
               }
               if(!bResponseWait) {
                  printf("\r%s",cp);
                  fflush(stdout);
                  bDisplayPrompt = FALSE;
                  bNeedNewline = TRUE;
               }
            }
            else if(bInteractive && 
                    (Ret == 0 || Ret == TBD_CHAT_TEXT) && 
                    (cp = strchr(Line,'\n')) != NULL) 
            {  // return code is not interesting, don't display it
               printf("%s",&cp[1]);
            }
            else {
               printf("%s",Line);
            }
            Line[0] = 0;
         }

         if(bResponseWait && (TimeNow - TimeCmdSent) >= RESP_TO) {
         // Timeout waiting for a response from tbd

            Ret = TBD_ERR_TIMEOUT;
            printf("%d\n",Ret);
            break;
         }
      } while(bInteractive);
   }
   
   SetTerminalMode(3);

   if(HistoryPath != NULL) {
      free(HistoryPath);
   }
   return Ret;
}

char *TimeT2String(time_t time)
{
   struct tm *pTm;
   static char TimeString[] = "mm/dd hh:mm";

   if(time != 0) {
      pTm = localtime(&time);
      if(pTm != NULL) {
         snprintf(TimeString,sizeof(TimeString),"%2d/%02d %2d:%02d",
                 pTm->tm_mon + 1,pTm->tm_mday,pTm->tm_hour,pTm->tm_min);
      }
      else {
         return "";
      }
   }
   else {
      return "";
   }
   return TimeString;
}


// If we have ">CONF " on the first line then assume this is a station list
int IsStationList(char *Line)
{
   char *cp;
   int Ret = FALSE;

   if((cp = strchr(Line,'\n')) != NULL && (cp = strchr(cp+1,'\n')) != NULL) {
      *cp = 0;
      if(strstr(Line,">CONF ") != NULL) {
         Ret = TRUE;
      }
      *cp = '\n';
   }
   return Ret;
}

#ifdef LINK_BOX
int SendCommand(char *Cmd)
{
   static int MySocket = -1;
   int BytesSent;
   int BytesRxed;
   int Ret = -1; 
   int Selret;
   IPAdrUnion HisAdr;
   fd_set ReadFDset;
   struct timeval SelTO;
   char  Line[1024];

   do {
      if(MySocket == -1) {
         if((MySocket = socket(AF_INET,SOCK_DGRAM,0)) == SOCKET_ERROR) {
            Ret = errno;
            LOG_ERROR(("%s: socket() failed - %s\n",__FUNCTION__,
                       strerror(errno)));
            break;
         }
      }

      HisAdr.i.sin_family = AF_INET;
      HisAdr.PORT = htons((u_short) Port);
      HisAdr.ADDR = inet_addr("127.0.0.1");

      if(Cmd != NULL) {
         LOG_ERROR(("%s: sending command \"%s\"\n",__FUNCTION__,Cmd));
         if(TlbPort != NULL) {
         // prefix commands with tlb port selection from the environment
            char *Temp = strdup(Cmd);
            snprintf(Line,sizeof(Line),"port %s;%s",TlbPort,Temp);
            free(Temp);
            Cmd = Line;
         }
         BytesSent = sendto(MySocket,Cmd,strlen(Cmd)+1,0,&HisAdr.s,
                            sizeof(HisAdr));

         if(BytesSent == SOCKET_ERROR) {
            Ret = errno;
            LOG_ERROR(("%s: sendto() failed - %s\n",__FUNCTION__,
                       strerror(errno)));
            break;
         }
      }
      Line[0] = 0;

      FD_ZERO(&ReadFDset);
      FD_SET(MySocket,&ReadFDset);

   // Wait a second for the response
      SelTO.tv_sec = 1;
      SelTO.tv_usec = 0;
      Selret = select(FD_SETSIZE,&ReadFDset,NULL,NULL,&SelTO);

      if(FD_ISSET(MySocket,&ReadFDset)) {
         BytesRxed = recv(MySocket,Line,sizeof(Line)-1,0);
         if(BytesRxed == SOCKET_ERROR) {
            Ret = errno;
            LOG_ERROR(("%s: recv() failed - %s\n",__FUNCTION__,
                       strerror(errno)));
         }
         else {
            int LineLen;

            Line[BytesRxed] = 0;
            LineLen = strlen(Line);
            if(LineLen > 0) {
               if(Line[LineLen-1] == '\n') {
               // remove terminating linefeed if present
                  LineLen--;
               }

               if(Line[LineLen-1] == '\n') {
               // remove second line if it is blank
                  LineLen--;
               }
               Line[LineLen] = 0;
            }
            LOG_ERROR(("%s: tlb returned %s\n",__FUNCTION__,Line));
            Ret = atoi(Line);
         }
      }
   } while(FALSE);
   return Ret;
}

void InstallSigHandler()
{
   struct sigaction SigAction;
   
// Setup signal handlers using sigaction() rather than the signal() quagmire!

   SigAction.sa_handler = SigHandler;
   SigAction.sa_flags = 0;
   sigemptyset(&SigAction.sa_mask);

   sigaction(SIGTERM,&SigAction,NULL);
   sigaction(SIGINT,&SigAction,NULL);
}

void LogCmdLine(char **argv,int argc)
{
   char Line[120];
   int BytesLeft = sizeof(Line) - 1;
   char *cp = Line;
   int i;
   int Count;

   for(i = 0; i < argc; i++) {
      Count = snprintf(cp,BytesLeft,"%s ",argv[i]);
   
      if(Count == -1 || Count >= BytesLeft) {
      // Pre-C99 style snprintf returns -1 on buffer overflow.  
      // C99 style snprintf returns the number of characters that would
      // have been written on buffer overflow.
      // In either case the output was truncated and p->Count is bogus.
   
         Count = BytesLeft;
      }
      cp += Count;
      if((BytesLeft -= Count) == 0) {
         break;
      }
   }
   LOG_NORM(("CmdLine: %s\n",Line));
}

char *GetIRLPCall()
{
   char *Ret = NULL;
   char *Local = getenv("LOCAL");
   char  Temp[1024];
   FILE *fp;
   char *cp;

   if(Local != NULL) {
      snprintf(Temp,sizeof(Temp),"%s/active",Local);
      if((fp = fopen(Temp,"r")) != NULL) {
         fgets(Temp,sizeof(Temp),fp);
         if((cp = strchr(Temp,'\n')) != NULL) {
            *cp = 0;
         }

         Ret = strdup(Temp);
         fclose(fp);
      }
      else {
         LOG_ERROR(("%s: Couldn't open \"%s\" - %s\n",__FUNCTION__,Temp,
                    strerror(errno)));
      }
   }
   else {
      LOG_ERROR(("%s: Error environment variable \"LOCAL\" not set.\n",
                 __FUNCTION__));
   }
   return Ret;
}

/* Usage: imike hostname[:port] [options] [ file1 / . ]...
   -A         Always transmit
   -B         Push to talk using keyboard
   -CELP      CELP compression
   -D         Enable debug output
   -F         ADPCM compression
   -H         Disables RADIO COS mode (monitor only)
   -L         Remote loopback
   -N         No compression
   -Phostname[:port] Party line, add host to list
*  -Q         Disable debug output
*  -T         Telephone (GSM) compression
   -TD        Debug: send GSM with swapped byte order
   -U         Print this message
   -x         Full duplex audio
   -Yindev[:ctldev] Override default audio device file name or specify open #fd
   -8         Set audio device to 8-bit mu-law
*/
int EmulateImike(char **argv,int argc)
{
   int i;
   char *Hostname = NULL;
   char *cp;
   int bFullDuplex = FALSE;
   int Port = 2074;  // default audio port
   char CmdLine[120];
   char *Station = NULL;
   char *CodecOption = "";

   InstallSigHandler();
   LogCmdLine(argv,argc);

   for(i = 1; i < argc; i++) {
      if(argv[i][0] == '-') {
         switch(argv[i][1]) {
            case 't':
            case 'T':
               LOG_ERROR(("%s: set GSM mode\n",__FUNCTION__));
               CodecOption = "";
               break;

            case 'x':
            case 'X':
               bFullDuplex = TRUE;
               break;

            case 'n':
            case 'N':
               CodecOption = "-u ";
               break;

            case 'f':
            case 'F':
               CodecOption = "-a ";
               break;

            default:
               LOG_ERROR(("%s: ignoring switch \"%s\"\n",__FUNCTION__,argv[i]));
               break;
         }
      }
      else {
      // Not a switch, hostname hopefully
         Hostname = argv[i];
         LOG_ERROR(("%s: hostname \"%s\"\n",__FUNCTION__,Hostname));
      }
   }

   do {
      if(Hostname == NULL) {
         break;
      }

      if((cp = strchr(Hostname,':')) != NULL) {
      // Port number specified
         *cp++ = 0;
         if(sscanf(cp,"%d",&Port) != 1) {
            LOG_ERROR(("%s: unable to parse port \"%s\"\n",__FUNCTION__,cp));
         }
      }

      if(strcmp(Hostname,"127.0.0.1") != 0) {
         if((Station = GetIRLPCall()) == NULL) {
            Station = "IRLP";
         }

         snprintf(CmdLine,sizeof(CmdLine),"connect -p %d -s %s%s%s %s",
                  Port,CodecOption,bFullDuplex ? "-f " : "",
                  Hostname,Station);

         SendCommand(CmdLine);
      }

      while(!bShutdown) {
         sleep(1000);
      }

      if(strcmp(Hostname,"127.0.0.1") != 0) {
      // EchoIRLP connects to localhost to talk to tbd, ignore it
         snprintf(CmdLine,sizeof(CmdLine),".disconnect %s",Station);
         SendCommand(CmdLine);
      }
   } while(FALSE);

   return 0;
}

/* 
Options:
           -D               Force debug output
           -fip_address     Sets firewall to only allow from ip_address
           -Jwait,idle      Jitter delay wait and idle in milliseconds
           -Mhost/ip        Join multicast to given name or IP address
           -pport           Listen on given port
           -Q               Prevent debug output
           -U               Print this message
           -x               Full duplex audio
           -Youtdev[:ctldev] Audio device names
           -8               Set audio device to 8-bit mu-law
*/
int EmulateIspeaker(char **argv,int argc)
{
   InstallSigHandler();
   LogCmdLine(argv,argc);

   while(!bShutdown) {
      sleep(1000);
   }
   printf("Exiting\n");

    return 0;
}

#define MAX_PLAY_WAIT      30

int EmulatePlay(char **argv,int argc)
{
   int i;
   int j;
   char CmdLine[120];
   int Response;

   LogCmdLine(argv,argc);

   for(i = 1; i < argc; i++) {
      snprintf(CmdLine,sizeof(CmdLine),"say -c %s",argv[i]);
      if((Response = SendCommand(CmdLine)) != TBD_OK) {
         LOG_ERROR(("%s: tlb returned %d for \"%s\"\n",__FUNCTION__,
                    Response,CmdLine));
         break;
      }

      for(j = 0; j < MaxPlayWait; j++) {
         Response = SendCommand(NULL);
         if(Response == TBD_SAY_COMPLETE) {
            break;
         }
         else if(Response != -1) {
            LOG_ERROR(("%s: tlb returned %d for \"%s\"\n",__FUNCTION__,
                       Response,CmdLine));
         }
      }

      if(j == MaxPlayWait) {
         LOG_ERROR(("%s: timed out waiting for TBD_SAY_COMPLETE\n",__FUNCTION__));
      }
   }

   return 0;
}

void Substitute(char *String,char From,char To)
{
   char *cp = String;

   while((cp = strchr(cp,From)) != NULL) {
      *cp++ = To;
   }
}

// 
int EmulateDtmfRegen(char **argv,int argc)
{
   char CmdLine[120];
   char *Station = NULL;
   char *Dtmf;
   int i;

   LogCmdLine(argv,argc);

   if((Station = GetIRLPCall()) == NULL) {
      Station = "IRLP";
   }

   if(argc > 1) {
      Dtmf = strdup(argv[1]);

      Substitute(Dtmf,'P','#');
      Substitute(Dtmf,'p','#');
      Substitute(Dtmf,'S','*');
      Substitute(Dtmf,'s','*');

      snprintf(CmdLine,sizeof(CmdLine),"port %s; dtmfdecode %s",Station,Dtmf);

   // EchoIRLP connects to localhost to talk to tbd, ignore it
      SendCommand(CmdLine);
   }

   if(argc != 2) {
      LOG_ERROR(("Unexpected argument count (%d):\n",argc));
      for(i = 1; i < argc; i++) {
         LOG_ERROR(("   %d: %s\n",i,argv[i]));
      }
   }

   return 0;
}

void Log(char *fmt, ...)
{
   int WriteLen;
   char  Temp[1024];
   char  Temp1[1024];
   time_t ltime;
   struct tm *tm;
   static int LastLogDay = -1;
   static int LastLogType;
   char *LogDir;
   va_list args;
   va_start(args,fmt);

   time(&ltime);
   tm = localtime(&ltime);

   if(LogFileRolloverType != 0 && 
         (LogFp == NULL || 
          tm->tm_mday != LastLogDay || 
          LastLogType != LogFileRolloverType)) 
   {
      LastLogType = LogFileRolloverType;

      if(LogFp != NULL) {
         fclose(LogFp);
         LogFp = NULL;
      }

      do {
         if((LogDir = getenv("TLB_HOME")) != NULL) {
            break;
         }

         if((LogDir = getenv("LOG")) != NULL) {
            break;
         }
         LogDir = ".";
      } while(FALSE);
   
   // Assume the name will be tbd.log
      snprintf(Temp,sizeof(Temp),"%s/%s.log",LogDir,AppName);

      switch(LogFileRolloverType) {
         case 2:  // daily log
            snprintf(Temp,sizeof(Temp),"%s/%s%02d%02d%02d.log",LogDir,AppName,
                     tm->tm_mon+1,tm->tm_mday,tm->tm_year % 100);
            break;

         case 3:  // daily log
            if(LastLogDay != -1 && tm->tm_mday != LastLogDay) {
            // A new day is born, rename the old log to .bak and open a new log
               snprintf(Temp1,sizeof(Temp1),"%s/%s.bak",LogDir,AppName);
               unlink(Temp1);
               rename(Temp,Temp1);
            }
            break;

         case 4:  // weekly
            snprintf(Temp,sizeof(Temp1),"%s/%s",LogDir,
                     LogNames[tm->tm_wday]);
            if(LastLogDay != -1 && tm->tm_mday != LastLogDay) {
            // delete last weeks log
               unlink(Temp);
            }
            break;
      }

      LastLogDay = tm->tm_mday;
      LogFp = fopen(Temp,"a");
   }

   WriteLen = vsnprintf(Temp,sizeof(Temp),fmt,args);
   va_end(args);

   if(LogFp != NULL) {
      if(LogFileRolloverType == 1 || LogFileRolloverType == -1) {
      // one big file log date & time
         fprintf(LogFp,"%s %d %d:%02d:%02d %s",MonthNames[tm->tm_mon],
                 tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec,Temp);
      }
      else {
      // Not one big file, just log time
         fprintf(LogFp,"%d:%02d:%02d %s",tm->tm_hour,tm->tm_min,
                 tm->tm_sec,Temp);
      }
      fflush(LogFp);

      if(ftell(LogFp) > 10000000) {
      // shit, get out of dodge
         fprintf(LogFp,"LOG FILE OVERFLOW, shutting down !\n");
         fclose(LogFp);
         LogFp = NULL;
         if(isatty(fileno(stdin))) {
            printf("LOG FILE OVERFLOW, shutting down !\n");
         }
      }
   }
}

void SigHandler(int Signal)
{
   switch(Signal) {
      case SIGINT:
         LOG_NORM(("Received SIGINT, shutting down\n"));
         break;

      case SIGTERM:
         LOG_NORM(("Received SIGTERM, shutting down\n"));
         break;
   }

   if(Signal == SIGINT || Signal == SIGTERM) {
   // Try an orderly shutdown
      bShutdown = TRUE;
   }
}
#endif

// Mode: 
// 0 - startup, save initial settings, turn off ICANON and ECHO
// 1 - disable ECHO, ICANON
// 2 - enable ECHO
// 3 - restore original terminal settings for exit
void SetTerminalMode(int mode)
{
#ifdef HAVE_LIBREADLINE
   struct termios options;
   static struct termios InitialMode;

   if(bUseReadline) do {
      if(tcgetattr(fileno(stdin),&options) < 0) {
         printf("%s: Unable to read port configuration: %s",__FUNCTION__,
                strerror(errno));
         break;
      }

      switch(mode) {
         case 0:
            InitialMode = options;
         // intentional fall through

         case 1:
            options.c_lflag &= ~(ICANON | ECHO);
            break;

         case 2:
            options.c_lflag |= ECHO;
            break;

         case 3:
            if(HistoryPath != NULL) {
               write_history(HistoryPath);
            }
            options = InitialMode;
            break;
         
      }

      if(tcsetattr(fileno(stdin),TCSANOW,&options) < 0) {
         printf("%s: tcsetattr() failed: %s",__FUNCTION__,strerror(errno));
      }
   } while(FALSE);
#endif
}

#ifndef HAVE_GETOPT

/*
 * getopt a wonderful little function that handles the command line.
 * available courtesy of AT&T.
 */
int getopt(int argc,char **argv,char *opts)
{
   static int sp = 1;
   register int c;
   register char *cp;

   if(sp == 1)
      if(optind >= argc ||
         argv[optind][0] != '-' || argv[optind][1] == '\0')
         return(EOF);
      else if(strcmp(argv[optind], "--") == 0) {
         optind++;
         return(EOF);
      }
   optopt = c = argv[optind][sp];
   if(c == ':' || (cp=strchr(opts, c)) == NULL) {
      printf("%s: illegal option -- ", argv[0],c);
      if(argv[optind][++sp] == '\0') {
         optind++;
         sp = 1;
      }
      return('?');
   }
   if(*++cp == ':') {
      if(argv[optind][sp+1] != '\0')
         optarg = &argv[optind++][sp+1];
      else if(++optind >= argc) {
         printf("%s: option requires an argument -- ", argv[0],c);
         sp = 1;
         return('?');
      }
      else
         optarg = argv[optind++];
      sp = 1;
   }
   else {
      if(argv[optind][++sp] == '\0') {
         sp = 1;
         optind++;
      }
      optarg = NULL;
   }
   return(c);
}
#endif
