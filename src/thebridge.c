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

   $Log: thebridge.c,v $
   Revision 1.24  2009/09/15 16:16:50  wb6ymh
   Corrected segfault that occured when WorkingDir was not set.

   Revision 1.23  2009/09/13 20:11:04  wb6ymh
   Added PID, and version information to the "starting" event.

   Revision 1.22  2009/07/03 17:03:30  wb6ymh
   1. Corrected PID logging.
   2. Added code to ReadConnfig to set TempDir to WorkingDir if it isn't set
      by the configuration file.

   Revision 1.21  2009/05/22 19:39:20  beta2k
   Added check to see if we're compiling tlb or tbd and change the usage message as per.  Output's package name and pid on startup

   Revision 1.20  2009/04/04 18:07:15  wb6ymh
   Moved RunAsUser code block outside of if(!Debug) block so the we'll change
   identities even in debug mode.

   Revision 1.19  2008/11/01 21:59:13  wb6ymh
   Changed the Option variable in main from a char to an int.  With some
   compilers (gcc 3.3.4 for the ARM) the default char type is unsigned which
   causes an infinite loop because Option can't hold a -1.

   Revision 1.18  2008/05/14 21:00:29  wb6ymh
   Corrected bug in -D argument processing.

   Revision 1.17  2008/02/26 17:45:38  wb6ymh
   1. Added dmalloc support
   2. Added -D command line option to set Debug (and DebugLevel) variables.
      Debug is a bitmapped variable, while DebugLevel is an integer.
   3. Minor memory leak at exit fixes: Call close log for all platforms,
      free ConfigFilePath, destroy all AVL trees, call runtime libs freeres
      function, call new cleanup reoutines.  This are really cosmetic fixes
      so we can "see the forest for the trees" when using dmalloc.

   Revision 1.16  2007/11/26 14:59:56  wb6ymh
   Added code to display version number and compile date when run in debug
   mode.

   Revision 1.15  2007/07/02 13:41:08  wb6ymh
   Added missed changes for tbdQt in main.

   Revision 1.14  2007/06/27 18:43:47  wb6ymh
   Added hooks for tbdQt by Scott KI4LKF.

   Revision 1.13  2007/06/12 23:49:33  wb6ymh
   Modified StartRunning to check for errors returned by  ConferenceInit and
   SFConfInit and exit on initialization failure.

   Revision 1.12  2006/08/05 23:42:46  wb6ymh
   Set rlim.rlim_max to RLIM_INFINITY to the fix ages old "Unable to enable
   core dumps" error/warning.  Later versions of Linux (at least) return an
   invalid argument error when rlim.rlim_max is less than rlim.rlim_cur.

   Revision 1.11  2005/01/08 23:58:46  wb6ymh
   Fix typo and add "warning" to error message about failure to
   enable core (not "code") dumps.  Apparently Fedora has changed
   something as this always fails with "Invalid argument".

   Revision 1.10  2004/11/29 00:40:20  wb6ymh
   Send AVRS update before shutting down.

   Revision 1.9  2003/08/16 14:07:27  wb6ymh
   1. Added eventhooks on startup and shutdown.
   2. Added compile date to initialization messages.
   3. Replaced *nix syslog specific %m in messages with calls to Err2String().
   4. Use initgroups() instead of setgid() when changing use identies so that
      any secondary groups thebridge's user belongs to accessable.
   5. Added call to process pass 2 configuration variables after basic system
      initialization has been completed.
   6. Modified Shutdown() shutdown immediately when directory server logins
      have been disabled.

   Revision 1.8  2003/04/30 21:59:45  wb6ymh
   Modified StartRunning() to use the AppName configuration variable's value
   as the application name when creating pid files rather than a hardcoded "tbd".

   Revision 1.7  2002/12/21 18:29:40  wb6ymh
   Added code to install, uninstall and run as an NT service.

   Revision 1.6  2002/11/02 18:27:08  wb6ymh
   1. Added -h command line option to force tbd.hosts to be loaded at startup.
   2. Added -q command line option to allow a quick restart by loading the
      host file and also trusting EchoLink clients that login before the
      first directory download has completed.
   3. Added error message for Winsock initialization failures.
   4. Made failure to enable core dumps non-fatal.
   5. Added code to wait for a key press when the Windows version exits
      with an error.

   Revision 1.5  2002/09/14 17:01:37  wb6ymh
   Move code that enables core dumps to beginning of main so we get core dumps
   on failures of config file processing routines.
   Added code to change user IDs after opening syslog and creating pid file.
   Reordered ReadConfig so working directory is set before LoadBulletinList()
   and LoadBannedList() are callled.

   Revision 1.4  2002/09/01 04:54:37  wb6ymh
   Added call to GetTimeNow() before call to ConferenceInit() so first directory
   list timeout is calculated correctly.

   Revision 1.3  2002/08/31 23:53:22  wb6ymh
   Added calls to LoadBulletinList() and LoadBannedList() in ReadConfig().

   Revision 1.2  2002/08/18 16:38:05  wb6ymh
   Added code to enable core dumps (thanks to WD4NMQ).

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#include "common.h"

#ifndef _WIN32
   // FreeBSD, Linux, etc..
   #include <stdio.h>
   #include <stdlib.h>

   #include <sys/types.h>

   #ifdef HAVE_GRP_H
      #include <grp.h>
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
   #ifdef HAVE_SYS_TIMEB_H
      #include <sys/timeb.h>
   #endif
   #include <unistd.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <arpa/inet.h>
   #include <errno.h>
   #include <string.h>
#ifdef HAVE_FCNTL_H
   #include <fcntl.h>
#endif
#ifdef HAVE_LIMITS_H
   #include <limits.h>
#else
   #define  INT_MAX     0x7fffffff  /* assume 32 bit ints */
#endif
   #include <syslog.h>
   #include <sys/resource.h>
   #include <pwd.h>
#else
   // Windoze
   #include <stdio.h>
   #include <time.h>
   #include <sys\timeb.h>
   #include <sys\stat.h>
   #include <io.h>
   #include <winsock2.h>
   #include <limits.h>
   #include <conio.h>
   #include <direct.h>
   #include "ntservice.h"
#endif

#include "avl.h"
#include "main.h"
#include "configvars.h"
#include "conference.h"
#include "dirclient.h"
#include "users.h"
#include "hostfile.h"
#include "eventhook.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#ifdef _WIN32
// Windoze
   WSADATA wsad;
   #define OPTION_STRING      "dD:f:shqiuw:"
#else
   #define PID_FILE_EXT      "pid"
   #define OPTION_STRING      "dD:f:shq"
#endif

#ifndef HAVE_GETOPT
// globals needed by getopt
int optind = 1;
int optopt;
char *optarg;
int getopt(int argc,char **argv,char *opts);
#endif

char *ConfigFilePath = NULL;
int ConfigPass = 0;

#ifdef _WIN32
   #ifdef _WIN32GUI
      int GuiWinMain(int argc, char *argv[], char *envp[])
   #else
      int main(int argc, char *argv[], char *envp[])
   #endif
#else
  #ifdef _X11GUI
    int GuiX11Main(int argc, char *argv[], char *envp[])
  #else
    int main(int argc, char *argv[], char *envp[])
  #endif
#endif
{
   int Err = 0;
   int Option;
   int Slient = 0;
#ifndef _WIN32
   struct rlimit rlim;


// On some systems core dumps are disabled by default,
// enable them so we can debug this thing !

   rlim.rlim_cur = RLIM_INFINITY;
   rlim.rlim_max = RLIM_INFINITY;
   if(setrlimit(RLIMIT_CORE, &rlim)) {
      perror("Warning - Unable to enable core dumps: ");
   }

#ifdef USE_DMALLOC
   dmalloc_debug_setup("debug=0x4000503,log=/home/skip/dmalloc.log");
#endif

#else
   /* Initialize Winsock*/
   if(WSAStartup(0x0101, &wsad) != 0) {
      printf("Fatal error: unable to initialize Winsock\n");
      exit(3);
   }
#endif
   MainLoopInit();

   while((Option = getopt(argc, argv,OPTION_STRING)) != -1 && Err == 0) {
      switch(Option) {
         case 'd':   // Debug mode
            if(++DebugLevel > 3) {
               printf("Sorry, -d x 3 is as verbose as I get!\n");
               Err = ERR_CMD_LINE_OPTION;
            }
            else {
               Debug |= DebugLevel;
            }
            break;

         case 'D':   // Set Debug mode using bitmap
            if(sscanf(optarg,"%x",&Debug) != 1) {
               printf("Error: invalid argument (try a hex number)\n");
            }
            else {
               DebugLevel = Debug & 0x7;
            }
            break;

         case 'h':   // load host file
            bLoadHostFileOnStart = TRUE;
            break;

         case 'f':   // configuration file path
            if(ConfigFilePath != NULL) {
               free(ConfigFilePath);
            }
            ConfigFilePath = strdup(optarg);
            break;

#ifdef _WIN32
#ifndef _WIN32GUI
         case 'i':   // install service
            InstallService();
            Err = ERR_SERVICE_OPTION;
            break;

         case 'u':   // uninstall service
            RemoveService();
            Err = ERR_SERVICE_OPTION;
            break;

         case 'w':
            if(WorkingDir != NULL) {
               free(WorkingDir);
            }
            WorkingDir = strdup(optarg);
            break;
#endif
#endif
         case 'q':
         // quick restart, load hosts file and trust stations that connect
         // before the first directory download has completed.
            bLoadHostFileOnStart = TRUE;
            bQuickStart = TRUE;
            break;

         case 's':
            Slient++;
            break;

         case '?':
#ifndef _WIN32
#ifdef LINKBOX
            printf("Usage: tlb [-d] [-h] [-f <config_file>] [-s]\n");
#else
            printf("Usage: tbd [-d] [-h] [-f <config_file>] [-s]\n");
#endif
            printf("\t-d - Debug (with increasing verbosity if used "
                   "multiple times)\n");
            printf("\t-f - set configuration file name.\n");
            printf("\t-h - load tbd.hosts on startup.\n");
            printf("\t-q - Quick restart.\n");
            printf("\t-s - slient - suppress startup messages.\n");
#else    // Windoze version has install/remove service switches
#ifdef _WIN32GUI
            printf("Usage: tbd [-d] [-h] [-f <config_file>] [-s] \n");
#else
            printf("Usage: tbd [-d] [-h] [-f <config_file>] "
                   "[-w <working_dir> [-i]] [-s] [-u]\n");
#endif
            printf("\t-d - Debug (with increasing verbosity if used "
                   "multiple times)\n");
            printf("\t-f - set configuration file name.\n");
            printf("\t-h - load tbd.hosts on startup.\n");
#ifndef _WIN32GUI
            printf("\t-i - Install tbd as a service (NT, Win200 and WinXP only).\n");
            printf("\t-u - Remove tbd service (NT, Win200 and WinXP only).\n");
            printf("\t-w - Set working directory.\n");
#endif
            printf("\t-q - Quick restart.\n");
            printf("\t-s - slient - suppress startup messages.\n");
#endif
            Err = ERR_CMD_LINE_OPTION;
            break;
      }
   }

/* KI4LKF start: dont run it as a service if started it as a GUI in Windows */
#ifdef _WIN32
#ifndef _WIN32GUI
   if(argc == 1 && !isatty(fileno(stdin))) {
   // If there are no commandline arguments and stdin is not a tty then
   // attempt to run as a service
      Err = RunAsService();
   }
#endif
#endif
   if(Err == 0) {
      if(!Slient) {
         printf(PACKAGE " Version " VERSION " compiled " __DATE__ " " __TIME__ "\n");
      }
      Err = StartRunning();
   }
   return Err;
}

int StartRunning()
{
   int Err = 0;
   int Slient = 0;
#ifndef _WIN32
   FILE *fp;
   struct passwd *pPasswd;
   uid_t MyUid;
   char  PidFilename[80];
#endif

   do {
      ClientTree = avl_create(ClientCompare,NULL,NULL);
      UserIPAdrTree = avl_create(UserIPCompare,NULL,NULL);
      UserTree = avl_create(UserCompare,NULL,NULL);
      ACL_Call_Tree = avl_create(ACL_CallCompare,NULL,NULL);
      ACL_IP_Tree = avl_create(ACL_IPCompare,NULL,NULL);

      if(ClientTree == NULL || UserTree == NULL || UserIPAdrTree == NULL ||
         ACL_Call_Tree == NULL || ACL_IP_Tree == NULL)
      {
      // That ain't good !
         printf("StartRunning(): Fatal error, unable to create AVL trees.\n");
         Err = ERR_CREATE_AVL;
         break;
      }

      GetTimeNow();

      if(ReadConfig()) {
         Err = ERR_CONFIG_FILE;
         break;
      }


#ifndef _WIN32
   #ifdef _X11GUI
         printf("Will run as a Gui app not a Daemon ...\n");
   #else
      if(!DebugLevel) {
         pid_t pid;

         if(!Slient) {
            printf("Becoming a Daemon ...\n");
         }
         openlog(AppName,LOG_NOWAIT, LOG_LOCAL5);
         bDaemon = TRUE;

         // Fork daemon
         if((pid = fork()) < 0) {
            perror("Forking");
            exit(1);
         }
         else if(pid != 0) {
         // parent
            exit(0);
         }

      // must be the child
         if(setsid() == -1) {
            LOG_ERROR(("setsid() failed: %s",Err2String(errno)));
            exit(2);
         }
         if(daemon(1, 0) < 0) {
            LOG_ERROR(("daemon(1,0) failed: %s",Err2String(errno)));
         }

         snprintf(PidFilename,sizeof(PidFilename),"%s%s." PID_FILE_EXT,RunDir,
                  AppName);

         if((fp = fopen(PidFilename,"w")) != NULL) {
            fprintf(fp,"%d\n",getpid());
            fclose(fp);
         }
         else {
            LOG_ERROR(("Unable to create pid file \"%s\": %s",PidFilename,
                       Err2String(errno)));
         }
      }

      LOG_NORM(("%s V %s compiled %s %s (pid: %d)\n", PACKAGE, VERSION, 
                __DATE__, __TIME__,getpid()));

      if(RunAsUser == NULL) {
         RunAsUser = strdup(AppName);
      }

      if((pPasswd = getpwnam(RunAsUser)) != NULL) {
      // Change our groups and user ID to that of the specified user
         if(initgroups(RunAsUser,pPasswd->pw_gid)) {
            LOG_ERROR(("StartRunning(): initgroups() failed, %s",
                       Err2String(errno)));
         }
         if(setuid(pPasswd->pw_uid)) {
            LOG_ERROR(("StartRunning(): setuid() failed, %s",Err2String(errno)));
         }
      }
      else {
         MyUid = getuid();
         pPasswd = getpwuid(MyUid);
         LOG_WARN(("Can't find user %s running as %s\n",
                   RunAsUser,pPasswd == NULL ? "Unknown!" : pPasswd->pw_name));
      }

   #endif
#endif

   // Don't emit any events before changing user IDs or we'll never get
   // notification that the event script exited (and the event script will
   // probably run as ROOT !)

      EventHook("starting %d %s %s %s %s",getpid(),PACKAGE, VERSION,__DATE__,
                __TIME__);

      if((Err = ConferenceInit()) != 0) {
         break;
      }

      if(SF_Enable) {
         if((Err = SFConfInit()) != 0) {
            break;
         }
      }

#ifdef _WIN32
      if(bDaemon) {
         ReportStatusToSCMgr(SERVICE_RUNNING,NO_ERROR,0);
      }
#endif
   // Initialization is complete, run pass 2 on the configuration variables
      DoConfigPass(2);
      if(StationListInterval == 0) {
      // We're not updating our station list, run pass 3 & 4 now.
         DoConfigPass(3);
         DoConfigPass(4);
      }
      MainLoop();
      EventHook("shutdown");
      DirCleanup();
      HostCleanup();
      ConferenceCleanup();
      SetConfigDefaults(FALSE);  // free dynamic allocated configuration strings
      if(ConfigFilePath != NULL) {
         free(ConfigFilePath);
      }

      avl_destroy(ClientTree,NULL);
      avl_destroy(UserIPAdrTree,NULL);
      avl_destroy(UserTree,NULL);
      avl_destroy(ACL_Call_Tree,NULL);
      avl_destroy(ACL_IP_Tree,NULL);

#ifdef _WIN32

   // Heavy sigh, Windoze users seem to insist on double clicking on everything
   // in sight including thebridge.  If we have an error require the user to
   // hit a key before exiting so he can read any error messages that are on
   // the screen before the window closes.

      if(Err != 0 && isatty(fileno(stdin))) {
         printf("\nPress any key to continue...");
         getch();
      }
#endif
   } while(FALSE);
   CloseLog();

#ifdef USE_DMALLOC
   __libc_freeres();
   dmalloc_shutdown();
#endif

   return Err;
}

int ReadConfig()
{
   int Ret;

   SetConfigDefaults(TRUE);
   Ret = DoConfigPass(1);
   if(TempDir == NULL) {
   // No Tempdir was set, use the WorkingDir
      if(WorkingDir != NULL) {
         TempDir = strdup(WorkingDir);
      }
      else {
         TempDir = strdup("./");
      }
   }
   LoadBulletinList();
   LoadACL();
   return Ret;
}

void Shutdown()
{
   if(LoginInterval == 0) {
   // Not using a directory server, just exit immediately
      bRunning = FALSE;
   }
   else {
      if(AvrsEnable) {
         SendAvrsUpdate(AVRS_OFF,NULL);
      // Disable AVRS so we don't send any further updates while we're
      // shutting down.
         AvrsEnable = FALSE;
      }
      ServerRequest(SERV_REQ_SHUTDOWN,0,NULL);
   }
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
      printf("%s: illegal option '%c'\n", argv[0],c);
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
         printf("%s: option '%d' requires an argument\n", argv[0],c);
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


