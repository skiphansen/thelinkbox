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

   $Log: ntservice.c,v $
   Revision 1.2  2003/08/16 13:39:18  wb6ymh
   Added compiled date to initializing message.

   Revision 1.1  2002/12/18 00:46:12  wb6ymh
   Initial commit.


*/
#include "common.h"

#include <stdio.h>
#include <time.h>
#include <sys\timeb.h>
#include <sys\stat.h>
#include <io.h>
#include <winsock2.h>
#include <limits.h>
#include <conio.h>
#include <direct.h>
#include "main.h"
#include "ntservice.h"

extern char *WorkingDir;
extern char *ConfigFilePath;

#define REGISTRY_KEY "SYSTEM\\CurrentControlSet\\Services\\" \
                     SERVICE_NAME "\\Parameters"

SERVICE_STATUS SvcStatus;
SERVICE_STATUS_HANDLE hStatus;

BOOL ReportStatusToSCMgr(DWORD CurrentState,DWORD ExitCode,DWORD WaitHint)
{
   static DWORD CheckPoint = 1;
   BOOL Ret = TRUE;

   if(CurrentState == SERVICE_START_PENDING) {
      SvcStatus.dwControlsAccepted = 0;
   }
   else {
      SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
   }

   SvcStatus.dwCurrentState = CurrentState;
   SvcStatus.dwWin32ExitCode = ExitCode;
   SvcStatus.dwWaitHint = WaitHint;

   if(CurrentState == SERVICE_RUNNING || CurrentState == SERVICE_STOPPED) {
      SvcStatus.dwCheckPoint = 0;
   }
   else {
      SvcStatus.dwCheckPoint = CheckPoint++;
   }


// Report the status of the service to the service control manager.
   if(!(Ret = SetServiceStatus(hStatus,&SvcStatus))) {
      LOG_ERROR(("SetServiceStatus() failed: Err = %d\n",GetLastError()));
   }
   return Ret;
}

VOID WINAPI service_ctrl(DWORD CtrlCode)
{
   // Handle the requested control code.
   if(CtrlCode == SERVICE_CONTROL_STOP) {
   // Stop the service.
      ReportStatusToSCMgr(SERVICE_STOP_PENDING,NO_ERROR,0);
      bShutdown = TRUE;
   }
   else {
      ReportStatusToSCMgr(SvcStatus.dwCurrentState,NO_ERROR,0);
   }
}

void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv)
{
   DWORD Err = 0;

// register our service control handler:
   hStatus = RegisterServiceCtrlHandler(SERVICE_NAME,service_ctrl);

   if(hStatus) {
      SvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
      SvcStatus.dwServiceSpecificExitCode = 0;

   // report the status to the service control manager.
      if(ReportStatusToSCMgr(SERVICE_START_PENDING,NO_ERROR,3000)) {
         LOG_NORM((PACKAGE " V " VERSION " compiled " __DATE__ " " __TIME__
                   " initializing\n"));
         Err = StartRunning();
      }
   // try to report the stopped status to the service control manager.
      ReportStatusToSCMgr(SERVICE_STOPPED,Err,0);
   }

   return;
}

SERVICE_TABLE_ENTRY dispatchTable[] = {
  {SERVICE_NAME,(LPSERVICE_MAIN_FUNCTION) service_main},
  {NULL, NULL}
};

void RemoveService()
{
   SC_HANDLE hService;
   SC_HANDLE hManager;
   SERVICE_STATUS SvcStatus;

   if((hManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS)) != NULL) {
      hService = OpenService(hManager,SERVICE_NAME,SERVICE_ALL_ACCESS);
      if(hService) {
         // try to stop the service
         if(ControlService(hService,SERVICE_CONTROL_STOP,&SvcStatus )) {
            printf("Stopping " SERVICE_NAME);
            Sleep(1000);

            while(QueryServiceStatus(hService,&SvcStatus)) {
               if(SvcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
                  printf(".");
                  Sleep(1000);
               }
               else {
                  break;
               }
            }

            if(SvcStatus.dwCurrentState == SERVICE_STOPPED) {
               printf("\n%s stopped.\n",SERVICE_NAME);
            }
            else {
               printf("\n%s failed to stop.\n",SERVICE_NAME);
            }
         }

         // now remove the service
         if(DeleteService(hService)) {
            printf("%s service uninstalled.\n",SERVICE_NAME);
         }
         else {
            printf("DeleteService() failed, Err = %d\n",GetLastError());
         }
         CloseServiceHandle(hService);
      }
      else {
         printf("OpenService() failed, Err = %d\n",GetLastError());
      }
      CloseServiceHandle(hManager);
   }
   else {
      printf("OpenSCManager() failed, Err = %d\n",GetLastError());
   }
}

void InstallService()
{
   SC_HANDLE   hService;
   SC_HANDLE   hManager;
   HKEY        hkey;
   int         Err = -1;
   DWORD       dwcSecDesc;
   struct stat FileStats;
   char        exepath[MAX_PATH];
   char        ConfigPath[MAX_PATH];
   char        Sep = '\\';
   char        LastPathChar;

   if(WorkingDir != NULL) {
      if(ConfigFilePath != NULL) {
         snprintf(ConfigPath,sizeof(ConfigPath),"%s",ConfigFilePath);
      }
      else {
         LastPathChar = WorkingDir[strlen(WorkingDir)-1];
         if(LastPathChar == '/' || LastPathChar == '\\') {
            Sep = 0;
         }
         snprintf(ConfigPath,sizeof(ConfigPath),"%s%c" SERVICE_NAME ".conf",
                  WorkingDir,Sep);
      }
   }
   
   if(WorkingDir == NULL) {
      printf("Error: Working directory not specified.\n");
   }
   else if(stat(WorkingDir,&FileStats) != 0) {
   // Can't working directory
      printf("Error: Working directory \"%s\" not found.\n",WorkingDir);
   }
   else if(GetModuleFileName(NULL,exepath,sizeof(exepath)) == 0) {
      printf("Error: GetModuleFileName() failed, Err = %d\n",GetLastError());
   }
   else if(stat(ConfigPath,&FileStats) != 0) {
   // Can't find config file in path specified
      printf("Error: configuration file \"%s\" not found.\n",ConfigPath);
   }
   else if((hManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS)) != NULL) {
      hService = CreateService(hManager,SERVICE_NAME,SERVICE_NAME,
                               SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,
                               SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,
                               exepath,NULL,NULL,NULL,NULL,NULL);
      if(hService) {
         CloseServiceHandle(hService);
         Err = RegCreateKeyEx(HKEY_LOCAL_MACHINE,REGISTRY_KEY,0,NULL,
                              REG_OPTION_NON_VOLATILE,
                              KEY_ALL_ACCESS | KEY_QUERY_VALUE,NULL,&hkey,
                              &dwcSecDesc);

         if(Err == ERROR_SUCCESS) {
            Err = RegSetValueEx(hkey,"ConfigFilePath",0,REG_SZ,ConfigPath,
                                strlen(ConfigPath));
            if(Err != ERROR_SUCCESS) {
               printf("RegSetValueEx() failed, Err = %d\n",GetLastError());
            }
            Err = RegSetValueEx(hkey,"WorkingDir",0,REG_SZ,WorkingDir,
                                strlen(WorkingDir));
            if(Err != ERROR_SUCCESS) {
               printf("RegSetValueEx() failed, Err = %d\n",GetLastError());
            }

            RegCloseKey(hkey);
         }
         else {
            printf("RegCreateKeyEx() failed, Err = %d.\n",GetLastError());
         }
      }
      else {
         printf("CreateService() failed, Err = %d\n",GetLastError());
      }
      CloseServiceHandle(hManager);
   }
   else {
      printf("OpenSCManager() failed, Err = %d\n",GetLastError());
      printf("NOTE: Installation of " SERVICE_NAME " as a service requires "
             "Windows 2000 or later.\n");
   }

   if(Err == ERROR_SUCCESS) {
      printf(SERVICE_NAME " service installed successfully.\n");
   }
}

int RunAsService()
{
   long Err;
   HKEY hkey;
   BYTE Temp[MAX_PATH];
   DWORD Type = 0;
   DWORD Size = sizeof(Temp);

   Err = RegOpenKeyEx(HKEY_LOCAL_MACHINE,REGISTRY_KEY,0,KEY_QUERY_VALUE,&hkey);
   if(Err == ERROR_SUCCESS) {
      RegQueryValueEx(hkey,"ConfigFilePath",NULL,&Type,Temp,&Size);
      if(Size > 0) {
         ConfigFilePath = strdup(Temp);
      }
      Size = sizeof(Temp);
      RegQueryValueEx(hkey,"WorkingDir",NULL,&Type,Temp,&Size);
      if(Size > 0) {
         WorkingDir = strdup(Temp);
         chdir(WorkingDir);
      }
      RegCloseKey(hkey);
   }

   if(ConfigFilePath != NULL) {
   // We have a path to our configuration file, go for it...
      bDaemon = TRUE;
      if(StartServiceCtrlDispatcher(dispatchTable)) {
      // We're running as a service ...
         return ERR_SERVICE_OPTION;
      }
      LOG_ERROR(("StartServiceCtrlDispatcher() failed, Err = %d\n",
                 GetLastError()));
   }

   return 0;
}

