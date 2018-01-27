# Microsoft Developer Studio Project File - Name="gsm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gsm - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GSM.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GSM.MAK" CFG="gsm - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gsm - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gsm - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "gsm - Win32 No Crypto Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gsm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "SASR" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "gsm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I ".\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "SASR" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "gsm - Win32 No Crypto Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "No Crypto Release"
# PROP BASE Intermediate_Dir "No Crypto Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Nocrypto"
# PROP Intermediate_Dir ".\Nocrypto"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "SASR" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "gsm - Win32 Release"
# Name "gsm - Win32 Debug"
# Name "gsm - Win32 No Crypto Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\src\Add.c
# End Source File
# Begin Source File

SOURCE=.\src\Code.c
# End Source File
# Begin Source File

SOURCE=.\src\Debug.c
# End Source File
# Begin Source File

SOURCE=.\src\Decode.c
# End Source File
# Begin Source File

SOURCE=.\src\Gsm_create.c
# End Source File
# Begin Source File

SOURCE=.\src\Gsm_decode.c
# End Source File
# Begin Source File

SOURCE=.\src\Gsm_destroy.c
# End Source File
# Begin Source File

SOURCE=.\src\Gsm_encode.c
# End Source File
# Begin Source File

SOURCE=.\src\Gsm_explode.c
# End Source File
# Begin Source File

SOURCE=.\src\Gsm_implode.c
# End Source File
# Begin Source File

SOURCE=.\src\Gsm_option.c
# End Source File
# Begin Source File

SOURCE=.\src\Gsm_print.c
# End Source File
# Begin Source File

SOURCE=.\src\Long_term.c
# End Source File
# Begin Source File

SOURCE=.\src\Lpc.c
# End Source File
# Begin Source File

SOURCE=.\src\Preprocess.c
# End Source File
# Begin Source File

SOURCE=.\src\Rpe.c
# End Source File
# Begin Source File

SOURCE=.\src\Short_term.c
# End Source File
# Begin Source File

SOURCE=.\src\Table.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\src\inc\config.h
# End Source File
# Begin Source File

SOURCE=.\src\inc\gsm.h
# End Source File
# Begin Source File

SOURCE=.\src\inc\private.h
# End Source File
# Begin Source File

SOURCE=.\src\inc\proto.h
# End Source File
# Begin Source File

SOURCE=.\src\Inc\Unproto.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
