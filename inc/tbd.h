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

   $Log: tbd.h,v $
   Revision 1.8  2009/09/13 19:49:16  wb6ymh
   Added TBD_RX_LEVEL_RPT.

   Revision 1.7  2008/03/09 16:00:16  wb6ymh
   Added TBD_SAY_COMPLETE.

   Revision 1.6  2007/12/27 17:41:15  wb6ymh
   Added TBD_PORT_NOT_FOUND.

   Revision 1.5  2007/06/12 23:43:39  wb6ymh
   Added TBD_LAST_ERR_CODE.

   Revision 1.4  2004/11/27 22:05:19  wb6ymh
   Added TBD_CHAT_TEXT_SENT.  Changed EOL convention back to Unix sytle(!).

   Revision 1.3  2004/11/22 00:59:23  wb6ymh
   Added TBD_CHAT_TEXT and TBD_ERR_TIMEOUT.

   Revision 1.2  2003/08/31 23:03:35  wb6ymh
   Added TBD_ERR_* (error codes returned by the scripting interface).

   Revision 1.1.1.1  2002/10/06 17:00:22  wb6ymh
   Move common header files to inc subdirectory from src.

*/

#ifndef TBD_H_
#define TBD_H_

typedef struct {
   int   Flags;
      #define FILE_FLAG_DATA_PKT    0x1
      #define FILE_FLAG_RATE_LIMIT  0x2
      #define FILE_FLAG_TIME_STAMP  0x4
   int   Len;
} FilePacketHdr;

#define TBD_ERR_BASE          200000

#define TBD_OK                0  // command executed without error

// command not found or ambiguous
#define TBD_ERR_INVALID_CMD   (TBD_ERR_BASE+1)  

// command requires disk and disk is disabled
#define TBD_ERR_NO_DISK       (TBD_ERR_BASE+2)

// Specified station not found
#define TBD_STATION_NOT_FOUND (TBD_ERR_BASE+3)

// no info available for station (.info cmd)
#define TBD_STATION_NO_INFO   (TBD_ERR_BASE+4)

// invalid command argument count
#define TBD_ERR_ARG_COUNT     (TBD_ERR_BASE+5)

// Already connected (.connect cmd)
#define TBD_ERR_CONNECTED     (TBD_ERR_BASE+6)

// no one is talking (.mute cmd)
#define TBD_ERR_NO_TALKER     (TBD_ERR_BASE+7)

// invalid command argument
#define TBD_ERR_INVALID_ARG   (TBD_ERR_BASE+8)

// error opening a file
#define TBD_ERR_FILE_OPEN     (TBD_ERR_BASE+9)

// data is chat text
#define TBD_CHAT_TEXT         (TBD_ERR_BASE+10)

// timeout waiting for response from tbd ... it's probably not running
#define TBD_ERR_TIMEOUT       (TBD_ERR_BASE+11)

// last "command" was sent as chat text
#define TBD_CHAT_TEXT_SENT    (TBD_ERR_BASE+12)

// Specified port not found
#define TBD_PORT_NOT_FOUND    (TBD_ERR_BASE+13)

// Say command has completed
#define TBD_SAY_COMPLETE      (TBD_ERR_BASE+14)

// Rx level report
#define TBD_RX_LEVEL_RPT      (TBD_ERR_BASE+15)

#define TBD_LAST_ERR_CODE     (TBD_RX_LEVEL_RPT + 1)


#endif   /* TBD_H_ */
