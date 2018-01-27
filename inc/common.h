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

   This is the common header for all project files.

   $Log: common.h,v $
   Revision 1.25  2009/09/13 19:48:52  wb6ymh
   Added DLOG_PTT.

   Revision 1.24  2009/05/25 16:19:26  wb6ymh
   Added DLOG_RX_CTCSS.

   Revision 1.23  2009/05/24 03:55:58  beta2k
   Added ERR_SPAWN_FAIL

   Revision 1.22  2009/03/28 15:36:51  wb6ymh
   Added PATH_MAX checks from va3jss.

   Revision 1.21  2009/02/16 00:22:34  wb6ymh
   Added DLOG_RTP.

   Revision 1.20  2009/02/13 19:22:04  wb6ymh
   Added DLOG_TBURST.

   Revision 1.19  2008/12/30 00:20:13  wb6ymh
   Added DLOG_COS_CTSS, ERR_SERIAL_SHARE_ERR, ERR_NOT_SUPPORTED.

   Revision 1.18  2008/06/26 20:44:12  wb6ymh
   Added DLOG_LINKS, DLOG_DTMF, DLOG_EVENTS.

   Revision 1.17  2008/06/15 13:35:37  wb6ymh
   LogErr -> Log for Windoze builds.

   Revision 1.16  2008/05/14 18:12:59  wb6ymh
   1. Changed a bunch of functions to take const char * rather than char *
      to fix compile problems with GCC 4.2.x.
   2. Added LogHex, DLOG_RIG_CTRL.
   3. Changed Debug to unsigned int.

   Revision 1.15  2008/04/25 00:04:49  wb6ymh
   Added ERR_CTCSS_RX_INIT.

   Revision 1.14  2008/04/11 16:54:31  wb6ymh
   Added DLOG_TONEGEN.

   Revision 1.13  2008/03/11 14:03:50  wb6ymh
   Added DLOG_VOIP_AUDIO.

   Revision 1.12  2008/03/09 15:59:41  wb6ymh
   Added min, max and CORE_DUMP.

   Revision 1.11  2008/02/26 18:04:04  wb6ymh
   1. Added DLOG macro to allow logging to be conditioned on new config
      variable Debug.
   2. Added Debug and DLOG_CODEC_TYPE.

   Revision 1.10  2007/12/01 00:55:52  wb6ymh
   1. Move STRCMP macro here from various places.
   2. Added DFLUSH macro.

   Revision 1.9  2007/11/26 06:58:13  wb6ymh
   Added extern "C" around prototypes when compiled for C++.

   Revision 1.8  2007/11/21 17:27:47  wb6ymh
   Added ERR_INPUT_DEV_OPEN, ERR_USB_DEV_OPEN, ERR_KRLP_LOCKFILE_OPEN.

   Revision 1.7  2007/06/12 23:38:59  wb6ymh
   1. Added PATH_SEP, new error codes for thelinkbox.
   2. Added typedefs for basic integer types.

   Revision 1.6  2006/08/05 23:50:08  wb6ymh
   Added #define for socklen_t for Windoze builds.

   Revision 1.5  2003/08/16 13:45:59  wb6ymh
   Added FOPEN macro.
   Moved prototypes for Log(), CloseLog() outside of _WIN32 ifdef.
   Added ERR_DECOMPRESS_ERR.

   Revision 1.4  2003/04/30 22:06:12  wb6ymh
   Added ERR_BIND_IP.

   Revision 1.3  2002/12/18 00:49:54  wb6ymh
   Enabled logging for Windows.
   Added ERR_SERVICE_OPTION.

   Revision 1.2  2002/11/02 19:06:51  wb6ymh
   Added ERR_CMD_LINE_OPTION.

   Revision 1.1.1.1  2002/10/06 17:00:22  wb6ymh
   Move common header files to inc subdirectory from src.

   Revision 1.6  2002/09/14 16:42:57  wb6ymh
   Added ERR_WORKING_DIR.

   Revision 1.5  2002/09/02 15:18:44  wb6ymh
   Added ERR_SERVER_TIMEOUT.

   Revision 1.4  2002/08/31 23:38:15  wb6ymh
   Added MODE_RD_BIN, ERROR_CODE2MSG, ERR_SERVER_ERR.

   Revision 1.3  2002/08/15 05:34:17  wb6ymh
   Added #define OUR_HOST for Windows build

   Revision 1.2  2002/08/12 17:10:47  wb6ymh
   Made TimeNow a global.
   Removed argument to GetTimeNow().

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#ifndef COMMON_H_
#define COMMON_H_

#ifndef _WIN32
   // FreeBSD, Linux, etc..
   #include <config.h>
   #include <stdio.h>
   #define  READ(x,y,z) read(x,y,z)
   #define  WRITE(x,y,z) write(x,y,z)
   #define  CLOSE(x) close(x)
   #define  ERROR_CODE errno
   #define  ERROR_WOULD_BLOCK EAGAIN
   #define  ERROR_CONNECTED EISCONN
   #define  ERROR_IN_PROGRESS EINPROGRESS
   #define  SOCKET_ERROR   -1
   #define  SOCKET         int
   #define  INVALID_SOCKET -1
   #define  MODE_WRITE_TXT "w"
   #define  MODE_WRITE_BIN "w"
   #define  MODE_RDWR_BIN  "w+"
   #define  MODE_RD_BIN    "r"
   #define  ERROR_CODE2MSG "%m"
   #define  PATH_SEP       '/'
   #ifndef PATH_MAX
      #define  PATH_MAX    124
   #endif
   FILE *FOPEN(const char *fmt,const char *mode);
#else
   // Windoze
   #include <config.h.win32>
   // The following defines replace config.h for Windows
   #define HAVE_FTIME
   #define HAVE_STRICMP
   #define  READ(x,y,z) recv(x,y,z,0)
   #define  WRITE(x,y,z) send(x,y,z,0)
   #define  CLOSE(x) closesocket(x)
   #define  ERROR_CODE  WSAGetLastError()
   #define  ERROR_WOULD_BLOCK WSAEWOULDBLOCK
   #define  ERROR_CONNECTED WSAEISCONN
   #define  ERROR_IN_PROGRESS WSAEWOULDBLOCK
   #define  snprintf _snprintf
   #define  vsnprintf _vsnprintf
   #define  MODE_WRITE_TXT "w"
   #define  MODE_WRITE_BIN "wb"
   #define  MODE_RDWR_BIN  "w+b"
   #define  MODE_RD_BIN    "rb"
   #define  ERROR_CODE2MSG "err=%d.\n"
   #define  OUR_HOST "Windows"
   #define  FOPEN fopen
   #define  socklen_t size_t
   #define  PATH_SEP       '\\'
   #ifndef PATH_MAX
      #define PATH_MAX     124
   #endif
#endif

// Use a non-case sensitive compare function if one is available
#ifdef HAVE_STRICMP
   #define STRCMP stricmp
#else
#ifdef HAVE_STRCASECMP
   #define STRCMP strcasecmp
#else
   #define STRCMP strcmp
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifndef  FALSE
   #define FALSE  0
#endif

#ifndef  TRUE
   #define TRUE   !FALSE
#endif

#define DPRINTF(x)   if(DebugLevel) printf x
#define D2PRINTF(x)  if(DebugLevel > 1) printf x
#define D3PRINTF(x)  if(DebugLevel > 2) printf x

#define DFLUSH()     if(DebugLevel > 0) fflush(stdout)

#ifndef _WIN32
   void LogNorm(const char *fmt, ...);
   void LogWarn(const char *fmt, ...);
   void LogErr(const char *fmt, ...);
   #define LOG_NORM(x)  LogNorm x
   #define LOG_WARN(x)  LogWarn x
   #define LOG_ERROR(x) LogErr x
   #define DLOG(x,y) if(Debug & x) LogErr y
#else
   #define LOG_NORM(x)  Log x
   #define LOG_WARN(x)  Log x
   #define LOG_ERROR(x) Log x
   #define DLOG(x,y) if(Debug & x) Log y
#endif

void LogHex(void *AdrIn,int Len);

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif  /* min */

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif  /* min */

// Bits for DLOG
#define DLOG_CODEC_TYPE 0x8      // Verbose logging of RTP SF codec problems
#define DLOG_VOIP_AUDIO 0x10     // Verbose audio queuing in voip.cpp
#define DLOG_TONEGEN    0x20     // Verbose tonegen parser logging
#define DLOG_RIG_CTRL   0x40     // Verbose frequency control logging
#define DLOG_LINKS      0x80     // Verbose node linking logging
#define DLOG_DTMF       0x100    // Verbose DTMF and command decoding logging
#define DLOG_EVENTS     0x200    // Verbose event generation logging
#define DLOG_COS_CTCSS  0x400    // Verbose cos and ctcss detect logging
#define DLOG_TBURST     0x800    // Verbose tone burst access logging
#define DLOG_RTP        0x1000   // Verbose RTP sequence number logging
#define DLOG_RX_CTCSS   0x2000   // Verbose software CTCSS logging
#define DLOG_PTT        0x4000   // Verbose PTT logging

// Local Error codes
#define  ERR_LOCAL_BASE          100000
#define  ERR_CONNECTION_CLOSED   ERR_LOCAL_BASE + 1
#define  ERR_CONFIG_FILE         ERR_LOCAL_BASE + 2
#define  ERR_CREATE_AVL          ERR_LOCAL_BASE + 3
#define  ERR_SERVER_ERR          ERR_LOCAL_BASE + 4
#define  ERR_SERVER_TIMEOUT      ERR_LOCAL_BASE + 5
#define  ERR_WORKING_DIR         ERR_LOCAL_BASE + 6
#define  ERR_AVL_CREATE          ERR_LOCAL_BASE + 7
#define  ERR_MALLOC              ERR_LOCAL_BASE + 8
#define  ERR_CMD_LINE_OPTION     ERR_LOCAL_BASE + 9
#define  ERR_SERVICE_OPTION      ERR_LOCAL_BASE + 10
#define  ERR_BIND_IP             ERR_LOCAL_BASE + 11
#define  ERR_DECOMPRESS_ERR      ERR_LOCAL_BASE + 12
#define  ERR_AUDIO_DEV_OPEN      ERR_LOCAL_BASE + 13
#define  ERR_AUDIO_FMT           ERR_LOCAL_BASE + 14
#define  ERR_GSM_INIT            ERR_LOCAL_BASE + 15
#define  ERR_SERIAL_DEV_OPEN     ERR_LOCAL_BASE + 16
#define  ERR_SERIAL_GET_ATTR     ERR_LOCAL_BASE + 17
#define  ERR_SERIAL_SET_ATTR     ERR_LOCAL_BASE + 18
#define  ERR_SERIAL_SET_BAUD     ERR_LOCAL_BASE + 19
#define  ERR_AUDIO_SET_FRAG      ERR_LOCAL_BASE + 20
#define  ERR_IRLP_DEV_OPEN       ERR_LOCAL_BASE + 21
#define  ERR_IRLP_DEV_CLAIM      ERR_LOCAL_BASE + 22
#define  ERR_INPUT_DEV_OPEN      ERR_LOCAL_BASE + 23
#define  ERR_USB_DEV_OPEN        ERR_LOCAL_BASE + 24
#define  ERR_IRLP_LOCKFILE_OPEN  ERR_LOCAL_BASE + 25
#define  ERR_CTCSS_RX_INIT       ERR_LOCAL_BASE + 26
#define  ERR_SERIAL_SHARE_ERR    ERR_LOCAL_BASE + 27
#define  ERR_NOT_SUPPORTED       ERR_LOCAL_BASE + 28
#define  ERR_SPAWN_FAIL        ERR_LOCAL_BASE + 29


/*
 * The type definitions below are valid for 32-bit architectures and
 * may have to be adjusted for 16- or 64-bit architectures.
 */
typedef unsigned char  u_int8;
typedef unsigned short u_int16;
typedef unsigned int   u_int32;
typedef          short int16;

extern int DebugLevel;  // 0 = no debug, 1 = debug, 2 = more debug ...
// Same as above but a bit map, bottom 3 bits are the same as DebugLevel
extern unsigned int Debug;

int ReadConfig();
void Log(char *fmt, ...);
void CloseLog();

#define CORE_DUMP()  \
   LOG_ERROR(("%s#%d: dumping core\n",__FUNCTION__,__LINE__)); \
   DumpMe()

#ifdef __cplusplus
}
#endif

#endif
