/* Copyright (C) 2004 GFRN systems

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

   $Log: avrs.c,v $
   Revision 1.9  2012/12/09 18:41:35  wb6ymh
   Added call to DeleteClient in ReadAprsData on server timeout.  Corrects
   100% CPU utilization following timeout.
   Thanks to Rob VK5TRM for reporting and help debugging this problem.

   Revision 1.8  2009/02/01 18:20:56  wb6ymh
   1. Corrected formating of APRS-IS packets when configured for certain locations
      (longitude less that 100 degrees for one). Thanks to Erik oh2lak for
      reporting the problem and testing the fix.

   2. Corrected crash when APRS-IS is enabled and the ConferenceCall was longer
      than 9 characters.  It is not possible to use APRS-IS which such "calls"
      since the APRS-IS object name is a fixed size, but thelinkbox will now
      report the error instead of crashing.

   Revision 1.7  2009/01/04 15:50:10  wb6ymh
   Added some ifdef's to correct thebridge builds broken by thelinkbox 0.83
   commits.

   Revision 1.6  2008/12/30 00:53:23  wb6ymh
   Added APRS-IS support.

   Revision 1.5  2008/02/26 17:34:44  wb6ymh
   Added dmalloc support.

   Revision 1.4  2007/11/13 15:07:38  wb6ymh
   1. Restored logging of AVRS string, but only in debug mode.
   2. Added code to GenAVRS to round negative values for antenna height and
      gain to zero.

   Revision 1.3  2007/04/27 14:27:30  wb6ymh
   Removed left over debug logging from GenAVRS.

   Revision 1.2  2005/01/08 23:55:52  wb6ymh
   Added rounding to frequency conversion.

   Revision 1.1  2004/11/29 00:29:14  wb6ymh
   Initial import - report repeater/link location and parameters to the
   EchoLink servers.

   
EchoLink sends a packet once every 10 minutes to aprs.echolink.org.  It also
sends a packet when the node status changes (i.e., when somebody connects or
disconnects, or the node comes online or goes offline).

The packet is an RTP control packet, and thus is sent to port 5199.

The CNAME portion is the node callsign (including -L/-R), followed by a
slash, followed by a "password", which is actually a hash of the callsign.
However, at this time, the password field is ignored by the server; a future
release of the server will verify the authenticity of the packet by its
source IP address.  So the callsign can be repeated in this second field.

The format of the LOC portion is as follows:


 )EL-nnnnnn!DDMM.HHNXDDDMM.HHW0PHGphgd/FFFFFF/PPPyyyyyyyyyyzzzzzzzz

 Where:

 nnnnnn           is the 6 character node Name or Callsign (right-padded
                  with spaces to 6 chars)
 DDMM.HHN DDDMM.HHW  is the LAT/LONG of the node, using degrees, minutes,
                     and 100ths of minutes, and E/W/N/S
X                    is the letter "E", for EchoLink
 0                   is a "zero",the ICON character
p                    is the power (encoded)
h                    is the antenna height (encoded)
g                    is the antenna gain (encoded)
d                    is the antenna directivity (encoded)
 FFFFFF              is the frequency in KHz
 /PPP                is the PL in whole number Hz
 yyyyyyyyyy          Second line on a D7 (11th to 20th on a D700)
 zzzzzzzz            last 8 not shown on a D7 but are on the D700

 Formats for the variable field YYYYYYYYYY and ZZZZZZZZ:

    Offline:    " Off @HHMM........"  can contain some comment
    On-Line:    " On @ HHMM........"  Time it returned to ON-Line
    Busy:       " Busy HHMM........"  Time it went busy
    Connected:  "=NNnnnnnnn at HHMM"  Shows call/Name of connection

The encoding of the p/h/g/d fields is as follows:

     DIGITS   0  1  2   3   4   5   6    7    8    9
     -------------------------------------------------------------------
     POWER    0, 1, 4,  9, 16, 25, 36,  49,  64,  81  watts  SQR(P)
     HEIGHT  10,20,40, 80,160,320,640,1280,2560,5120  feet   LOG2(H/10)
     GAIN     0, 1, 2,  3,  4,  5,  6,   7,   8,   9  dB
     DIR   omni,45,90,135,180,225,270, 315, 360,   .  deg    (D/45)

For example:   // )EL-K1RFD !4118.52NE07329.97W0PHG7430/144300/000 On @0444
Rdgfld
*/

#include "common.h"
#ifndef _WIN32
   // FreeBSD, Linux, etc..
   #include <stdio.h>
   #include <stdlib.h>
   #include <stdarg.h>
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
   #include <unistd.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <string.h>
   #include <math.h>
   #include <ctype.h>
   #include <errno.h>
#ifdef HAVE_FCNTL_H
   #include <fcntl.h>
#endif
   #include <netdb.h>
#else
   // Windoze
   #include <stdio.h>
   #include <time.h>
   #include <winsock2.h>
   #include <math.h>
#endif

#include "rtp.h"
#include "main.h"
#include "configvars.h"
#include "ilink.h"
#include "dirclient.h"
#include "conference.h"
#include "hostfile.h"
#include "avl.h"
#include "avrs.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define APRS_CONNECT_TO 30000 /* 30 seconds (in Milliseconds) */
#define APRS_RESP_TO    30000 /* 30 seconds (in Milliseconds) */
#define APRS_PORT       14580

char AprsIsData[512];
int AprsIsLen = 0;


void GenAVRS(AVRS_State State,char *Connected2,char *Buf,int BufLen)
{
   static AVRS_State LastState = -1;
   static time_t StateChangeTime;
   int Wrote = 0;
   int Power;
   int Height;
   int Gain;
   int Dir;
   int Lat;
   int LatMin;
   int LatHunders;
   int Long;
   int LongHunders;
   int LongMin;
   int Freq;
   int Pl;
   char  NorthSouth;
   char  EastWest;
   char *cp;
   struct tm *pTm;
   char  SepChar;
   char  *Callsign;
   float x;

   if(State != LastState) {
   // New state, save the time
      LastState = State;
      time(&StateChangeTime);
   }

   pTm = localtime(&StateChangeTime);

   if((cp = strchr(ConferenceCall,'-')) != NULL) {
   // Remove -L or -R from conference callsign for now
      *cp = 0;
   }

   Callsign = strdup(ConferenceCall);

   if(cp != NULL) {
   // Restore -L or -R to conference callsign
      *cp = '-';
   }

   cp = Callsign;
   while(*cp) {
   // Convert to lower case
      *cp = tolower(*cp);
      cp++;
   }

   if(Latitude > 0.0) {
   // North
      NorthSouth = 'N';
      x = Latitude;
   }
   else {
   // South
      NorthSouth = 'S';
      x = -Latitude;
   }

   Lat = (int) x;
   x -= Lat;
   x *= 60.0;
   LatMin = (int) x;
   x -= LatMin;
   x *= 100.0;
   LatHunders = (int) (x + 0.5);

   if(Longitude > 0.0) {
   // East
      EastWest = 'E';
      x = Longitude;
   }
   else {
   // West
      x = -Longitude;
      EastWest = 'W';
   }

   Long = (int) x;
   x -= Long;
   x *= 60.0;
   LongMin = (int) x;
   x -= LongMin;
   x *= 100.0;
   LongHunders = (int) (x + 0.5);

   Power = (int) (sqrt(TxPower) + 0.5);
   if(Power > 9) {
      Power = 9;
   }

   Height = (int) (log(Haat/10.0) / log(2.0) + 0.5);
   if(Height > 9) {
      Height = 9;
   }

   if(Height < 0) {
      Height = 0;
   }

   Gain = (int) (AntGain + 0.5);
   if(Gain > 9) {
      Gain = 9;
   }

   if(Gain < 0) {
      Gain = 0;
   }

   Dir = (int) ((AntDir / 45.0) + 0.5);
   Freq = (int) ((Frequency * 1e3) + 0.5);
   Pl = (int) CTCSS;

   Wrote = snprintf(Buf,BufLen,
                    ")EL-%6s!%02d%02d.%02d%cE%03d%02d.%02d%c0",Callsign,
                    Lat,LatMin,LatHunders,NorthSouth,
                    Long,LongMin,LongHunders,EastWest);

   Wrote += snprintf(&Buf[Wrote],BufLen - Wrote,"PHG%d%d%d%d/%06d/%03d",
                     Power,Height,Gain,Dir,Freq,Pl);
   
   switch(State) {
      case AVRS_OFF:
         Wrote += snprintf(&Buf[Wrote],BufLen - Wrote," Off @");
         break;

      case AVRS_ON:
         Wrote += snprintf(&Buf[Wrote],BufLen - Wrote," On  @");
         break;

      case AVRS_BUSY:
         Wrote += snprintf(&Buf[Wrote],BufLen - Wrote," Busy ");
         break;

      case AVRS_CONNECTED:
         if((bEchoIrlpMode && ConferenceClients > 2) || 
            (!bEchoIrlpMode && ConferenceClients > 1))
         {  // multiple connections
            SepChar = '+';
         }
         else {
         // Single connection
            SepChar = '=';
         }

         Wrote += snprintf(&Buf[Wrote],BufLen - Wrote,"%c%s at ",SepChar,
                           Connected2);
         break;
   }

   Wrote += snprintf(&Buf[Wrote],BufLen - Wrote,"%02d%02d\r",
                     pTm->tm_hour,pTm->tm_min);

   DPRINTF(("GenAVRS(): sending string:\n%s\n",Buf));

   free(Callsign);
}


int CreateAvrsPacket(AVRS_State State,char *Connected2,char *Buf,int BufLen)
{
   char PadCount;
   char *pFirstData;
   union {
      char *cp;
      rtcp_t *p;
   } u;

   union {
      char *cp;
      rtcp_sdes_item_t *p;
   } Item;
   int CallLen;

   u.cp = Buf;

   u.p->common.version = ILINK_RTP_VERSION;
   u.p->common.p = 0;
   u.p->common.count = 0;
   u.p->common.pt = RTCP_RR;
   u.p->common.length = htons(1);
   u.p->r.rr.ssrc = 0;

   u.cp += sizeof(u.p->common) + sizeof(u.p->r.rr.ssrc);

   u.p->common.version = ILINK_RTP_VERSION;
   u.p->common.p = 1;
   u.p->common.count = 1;
   u.p->common.pt = RTCP_SDES;

   u.p->r.sdes.src = 0;
   pFirstData = (char *) &u.p->r.sdes.src;

   Item.p = &u.p->r.sdes.item[0];
   Item.p->type = RTCP_SDES_CNAME;
   CallLen = strlen(ConferenceCall);
   Item.p->length = (u_int8) (1 + CallLen + CallLen);
   memcpy(Item.p->data,ConferenceCall,CallLen);
   Item.p->data[CallLen] = '/';
// Eventually hash of the Echolink password
   memcpy(&Item.p->data[CallLen+1],ConferenceCall,CallLen);

   Item.cp = Item.cp + sizeof(rtcp_sdes_item_t) + Item.p->length - 1;
   Item.p->type = RTCP_SDES_LOC;

   GenAVRS(State,Connected2,Item.p->data,BufLen - (Item.cp - Buf));
   Item.p->length = (u_int8) strlen(Item.p->data);

   Item.cp = Item.cp + sizeof(rtcp_sdes_item_t) + Item.p->length - 1;
   Item.p->type = RTCP_SDES_END ;
   Item.p->length = 0;
   Item.cp = Item.cp + sizeof(rtcp_sdes_item_t) + Item.p->length - 1;
   PadCount = 4 - ((Item.cp - pFirstData) % 4);

   if(PadCount > 0 && PadCount < 4) {
   // Padding is needed
      u.p->common.p = 1;
      if(PadCount > 1) {
         memset(Item.cp,0,PadCount-1);
      }
      Item.cp[PadCount-1] = PadCount;
      Item.cp += PadCount;
   }
   else {
   // No padding needed
      u.p->common.p = 0;
   }
   u.p->common.length = htons((u_short) ((Item.cp - pFirstData)/4));

   return Item.cp - Buf;
}

#ifdef   LINK_BOX
/*
 user mycall[-ss] pass passcode[ vers softwarename softwarevers[ UDP udpport]
 [ servercommand]]

 * mycall-ss is your unique callsign-SSID (-SSID is optional but equates to 
   zero if omitted).  This must be unique throughout APRS and AX.25
 * passcode is the computed passcode for your callsign.  -1 is used for 
   receive-only.
 * softwarename is the one word name of the software. Do not use spaces.
 * softwarevers is the version number of the software. Do not use spaces.
 * udpport is the UDP port number that the client is listening to
 * servercommand is any command string (spaces OK) understood by the server.  
   This will normally be something like filter r/33/-96/25
   to cause all packets from stations with 25 km of 33N 96W to be passed to 
   the client.
*/

#define kKey 0x73e2     // This is the key for the data

/* As of April 11 2000 Steve Dimse has released this code to the open
source aprs community */
short doHash(const char* theCall) 
{
   char rootCall[10];      // need to copy call to remove ssid from parse
   char *p1 = rootCall;
   short hash = kKey;      // Initialize with the key value
   short i = 0;
   int len;
   char *ptr = rootCall;

   while((*theCall != '-') && (*theCall != 0)) *p1++ = toupper(*theCall++);
   *p1 = 0;

   len = strlen(rootCall);

   while(i < len) {           // Loop through the string two bytes at a time
      hash ^= (*ptr++)<<8;    // xor high byte with accumulated hash
      hash ^= (*ptr++);       // xor low byte with accumulated hash
      i += 2;
   }
   return hash & 0x7fff;       // mask off the high bit so number is always positive
}

/*
 javAPRSSrvr responds to a connection with a line starting with a # and 
 acknowledges the login with another line starting with a #.  Lines starting 
 with # are considered comments and ignored.  Most servers will also 
 periodically send lines beginning with a # to check the connection if 
 there has been no activity on the TCP port.
 
 So... disconnect after receiving the second line beginning with a #
*/
int ParseAprsResponse(ClientInfo *pClient,char *Buf,int Count)
{
   int Ret = 0;

   DPRINTF(("ParseAprsResponse: received: %s\n",Buf));
   if(pClient->x++ == 1) {
   // Second line received, disconnect
      if(strstr(Buf," verified") == NULL) {
         LOG_ERROR(("ParseAprsResponse: login apparently failed: %s\n",Buf));
      }
      DPRINTF(("ParseAprsResponse: disconnecting\n"));
      DeleteClient(pClient);
      Ret = 1;
   }

   return Ret;
}

// Read station list from the directory server
int ReadAprsData(ClientInfo *p)
{
   char *cp;
   int Count = 0;

   while(p->Err == 0) {
      if(p->bTimeOut) {
         p->Err = ERR_SERVER_TIMEOUT;
         LOG_ERROR(("ReadAprsData(): server time out client %u.\n",p->SN));
			DeleteClient(p);
         break;
      }

      while(p->Count > 0 && (cp = strchr(&p->Buf[p->RetCount],'\n')) != NULL)
      {  // We have a complete line

         Count = (cp - &p->Buf[p->RetCount]) + 1;
         *cp = 0; // remove the line feed
         if(ParseAprsResponse(p,&p->Buf[p->RetCount],Count-1) != 0) {
            break;
         }
         p->Count -= Count;
         p->RetCount += Count;
      }

      if(!GetRxData(p)) {
         break;
      }

      if(p->Err != 0 && p->Count > 0) {
      // Consume the remaining data
         ParseAprsResponse(p,&p->Buf[p->RetCount],Count-1);
      }
   }
   
   return 0;
}

void AprsPrintf(const char *fmt,...)
{
   va_list args;
   int Count;
   int BytesAvailable = sizeof(AprsIsData)-AprsIsLen;

   va_start(args,fmt);
   if(BytesAvailable > 0) {
      Count = vsnprintf(&AprsIsData[AprsIsLen],BytesAvailable,fmt,args);
      if(Count == -1 || Count >= BytesAvailable) {
      // Pre-C99 style snprintf returns -1 on buffer overflow.  
      // C99 style snprintf returns the number of characters that would
      // have been written on buffer overflow.
      // In either case the output was truncated

         Count = BytesAvailable - 1;
         AprsIsData[AprsIsLen+Count] = 0;
      }
      AprsIsLen += Count;
   }

   va_end(args);
}


/*
APRS Object Report Format
   EL-xxxxxx
+-+---------+-+-------+--------+-+---------+-+-------+------//-------+
|;|IRLP-xxxx|*|DDHHMMz|  LAT   | |   LON   | |PHGphgd|COMMENT 36 Chrs|
+-+---------+-+-------+--------+-+---------+-+-------+------//-------+
 1 234567891 1 1111111 12222222 2 223333333 3 3344444 444455--7777778
           0 1 2345678 90123456 7 890123456 7 8912345 678901--4567890

CHARS   DESCRIPTION
-----   --------------------------------------------------------------
    1 - The APRS Data Type Identifier; ";" = Object Report
 2-10 - Object Name; 9 characters, space padded on the right
   11 - * = Live Object; _ = Killed Object
12-18 - Timestamp: DDHHMMz - DD=DOM; HH=Hours; MM=Minutes; z=Zulu
19-26 - Latitude: DDMM.MMH - DD=Degrees (00-90); MM=Minutes (00-59);
                             .MM=Fractional Minutes (00-99);
                             H=Hemisphere (N or S)
   27 - Symbol Table ID (Alternate Symbol Table Overlay char)
28-36 - Longitude: DDDMM.MMH - DDD=Degrees (000-179);
                               MM=Minutes (00-59);
                               .MM=Fractional Minutes (00-99);
                               H=Hemisphere (E or W)
   37 - Symbol Code
38-45 - Power Height Gain 
46-80 - Comments: This is where the Node status goes
*/
char *CreateAprsPacket(int bForAprsd,AprsVals *p)
{
   struct tm *pTm = gmtime(&TimeNow.tv_sec);
   int Degrees;
   float Minutes;
   float Temp;
   int Power;
   int Height;
   int Gain;
   int Dir;
   char *BareCall;
   char *cp;
   char OffsetChar = 's';

   do {
      AprsIsLen = 0;

      if((BareCall = strdup(ConferenceCall)) == NULL) {
         LOG_ERROR(("CreateAprsPacket: out of memory!\n"));
         break;
      }
      if((cp = strchr(BareCall,'-')) != NULL) {
      // remove -L or -R from callsign
         *cp = 0;
      }

      if(strlen(BareCall) > 9) {
         LOG_ERROR(("CreateAprsPacket: Error ConferenceCall is too long for "
                    "APRS (9 characters maximum)\n"));
         break;
      }

      if(bForAprsd) {
      // For transmission over the internet
      // login
         AprsPrintf("User %s pass %d vers " PACKAGE " " VERSION "\n",BareCall,
                    doHash(BareCall));

      // from/to
         AprsPrintf("%s>APVR30:",BareCall);
      }
      else {
      // For transmission over RF
      // from/to
         AprsPrintf("%s>%s ",BareCall,
                    p->Ax25BeaconPath == NULL ? "QST" : p->Ax25BeaconPath);
      }

      if(AprsObjectName == NULL) {
         AprsPrintf(";EL-%-6s*",BareCall);
      }
      else {
         AprsPrintf(";%-9s*",AprsObjectName);
      }

   // Time Stamp
      AprsPrintf("%02d%02d%02dz",pTm->tm_mday,pTm->tm_hour,pTm->tm_min);

   // Latitude
      Temp = fabsf(Latitude);
      Degrees = (int) Temp;
      Minutes = (Temp - Degrees) * 60.0;
      AprsPrintf("%02d%02.2f%c%c",Degrees,Minutes,Latitude < 0.0 ? 'S' : 'N',
                 AprsSymbolTable == NULL ? '/' : *AprsSymbolTable);

   // Longitude
      Temp = fabsf(Longitude);
      Degrees = (int) Temp;
      Minutes = (Temp - Degrees) * 60.0;
      AprsPrintf("%03d%02.2f%c%c",Degrees,Minutes,Longitude < 0.0 ? 'W' : 'E',
                 AprsSymbolCode == NULL ? 'r' : *AprsSymbolCode);


      if(bForAprsd || EnableRfPHG) {
         Power = (int) (sqrt(p->TxPower) + 0.5);
         if(Power > 9) {
            Power = 9;
         }

         Height = (int) (log(p->Haat/10.0) / log(2.0) + 0.5);
         if(Height > 9) {
            Height = 9;
         }

         if(Height < 0) {
            Height = 0;
         }

         Gain = (int) (p->AntGain + 0.5);
         if(Gain > 9) {
            Gain = 9;
         }

         if(Gain < 0) {
            Gain = 0;
         }

         Dir = (int) ((p->AntDir / 45.0) + 0.5);

      // Power, Height, Gain
         AprsPrintf("PHG%d%d%d%d",Power,Height,Gain,Dir);
      }

      if(p->RptOffset > .001) {
         OffsetChar ='+';
      }
      else if(p->RptOffset < -.001) {
         OffsetChar ='-';
      }

      if(!bForAprsd && p->Ax25BeaconText != NULL) {
         AprsPrintf("%s",p->Ax25BeaconText);
      }
      else if(bForAprsd && AprsIsComment != NULL) {
         AprsPrintf("%s",AprsIsComment);
      }
      else {
      // Frequency, offset, PL
         AprsPrintf("%d%c%d",(int) (p->Frequency * 1000.0),OffsetChar,
                    (int) p->CTCSS);
      }
   } while(FALSE);

   if(BareCall != NULL) {
      free(BareCall);
   }
   return AprsIsData;
}


int AprsConnected(ClientInfo *pClient)
{
   int i;
   int Err = 0;
   int Ret = FALSE;

   if(pClient->bTimeOut) {
      i = 1;
      Err = ERR_SERVER_TIMEOUT;
      LOG_WARN(("AprsConnected(): connect() timed out for client %u\n",
                pClient->SN));
   }
   else {
      i = connect(pClient->Socket,&pClient->HisAdr.s,sizeof(pClient->HisAdr));

      if(i != 0 && (Err = ERROR_CODE) != ERROR_CONNECTED)
      // Connection failed.
#ifndef _WIN32
      {
         LOG_WARN(("PullerConnected(): connect() failed for client %u %s",
                   pClient->SN,Err2String(Err)));
      }
   }

#else
      {
      // Heavy sigh...  
      // Apparently there are bugs in Microsoft's TCP/IP stacks (shock !)
      // that causes connect() to return an error other than WSAEISCONN even
      // after select() has indicated that the socket is ready for write.
      // Try it a few times before giving up.

         if(pClient->x > 0) {
         // The first attempt almost always fails, don't fill the log with junk
            LOG_WARN(("PullerConnected(): connect() failed for client %u, "
                      "try %d, %s",pClient->SN,pClient->x+1,Err2String(Err)));
         }

         if(pClient->x++ < 10) {
         // Wait 20 milliseconds and try again
            pClient->bReadWait = FALSE;
            pClient->bWriteWait = FALSE;
            SetTimeout(pClient,20);
         }
         else {
            pClient->x = 0;
         }
      }
      else {
         if(pClient->x > 1) {
         // The second attempt almost always works, don't fill the log with junk
            LOG_WARN(("PullerConnected(): connect() succeeded for client %u, "
                      "try %d\n",pClient->SN,pClient->x+1));
         }
         pClient->x = 0;
      }
   }

   if(pClient->x == 0)
   // Only handle sucess/failure if not a retry
#endif

// BSD returns an error with an error code of EISCONN here.  That agrees
// with the man page description of connect().
//
// Linux does not return an error.  The Linux man page (at least Red Hat's) 
// doesn't include any details on the expected behavour of connect when used
// with non-blocking sockets and select as we are doing here.
//
   
   if(i != 0 && Err != ERROR_CONNECTED) {
   // Connection failed.
      OpComplete(pClient,Err);
   }
   else {
   // Send our data
      DPRINTF(("AprsConnected: sending: %s\n",AprsIsData));
      if(WRITE(pClient->Socket,AprsIsData,AprsIsLen) != AprsIsLen) {
      // This isn't good.  It's the first thing we've sent.
      // The buffer can't be full yet !
         LOG_WARN(("AprsConnected: write() failed, %s",Err2String(ERROR_CODE)));
         DeleteClient(pClient);
      }
      else {
         pClient->State = ReadAprsData;
         pClient->BufSize = 256;
         pClient->Buf = (char *) malloc(pClient->BufSize);

         SET_WAIT4_RD(pClient);

      // Wait for up to APRS_RESP_TO milliseconds for the response
         SetTimeout(pClient,APRS_RESP_TO); 
      }
   }

   return Ret;
}

/*
 user mycall[-ss] pass passcode[ vers softwarename softwarevers[ UDP udpport]
 [ servercommand]]

   mycall-ss is your unique callsign-SSID (-SSID is optional but equates to 
   zero if omitted).  This must be unique throughout APRS and AX.25
   passcode is the computed passcode for your callsign.  -1 is used for 
   receive-only.
   softwarename is the one word name of the software. Do not use spaces.
   softwarevers is the version number of the software. Do not use spaces.
   udpport is the UDP port number that the client is listening to
   servercommand is any command string (spaces OK) understood by the server.  
   This will normally be something like filter r/33/-96/25
   to cause all packets from stations with 25 km of 33N 96W to be passed to 
   the client.
*/
void Send2Aprsd(AVRS_State State,char *Connected2)
{
   ClientInfo *pClient = NULL;
   ClientInfo *pClient1 = NULL;
   int Ret = -1;
   struct hostent *pTemp;
   int i;
   AprsVals Vals;


   Vals.TxPower = TxPower;
   Vals.Haat = Haat;
   Vals.AntGain = AntGain;
   Vals.Frequency = Frequency;
   Vals.CTCSS = CTCSS;
   Vals.AntDir = AntDir;
   Vals.RptOffset = RptOffset;

   CreateAprsPacket(TRUE,&Vals);

   switch(State) {
      case AVRS_OFF:
         AprsPrintf(" OFFLINE");
         break;

      case AVRS_ON:
         AprsPrintf(" IDLE");
         break;

      case AVRS_BUSY:
         AprsPrintf(" BUSY");
         break;

      case AVRS_CONNECTED:
         AprsPrintf("C %s",Connected2);
         break;
   }

   AprsPrintf("\n");

   DPRINTF(("Send2Aprsd(): sending string:\n%s\n",AprsIsData));

   do {
      if((pClient = CreateNewClient()) == NULL) {
         break;
      }

      pClient->Socket = socket(AF_INET,SOCK_STREAM,0);
      if(pClient->Socket == SOCKET_ERROR) {
         LOG_ERROR(("ServerRequest(): Socket() failed %s",
                    Err2String(ERROR_CODE)));
         break;
      }
      else {
         OpenSockets++;
      }

   // Set the socket to non-blocking
#ifdef _WIN32
      i = 1;
      ioctlsocket(pClient->Socket,FIONBIO,&i);
#else
      fcntl(pClient->Socket,F_SETFL,O_NONBLOCK);
#endif

      pClient->HisAdr.i.sin_family = AF_INET;
      pClient->HisAdr.i.sin_port = htons(APRS_PORT);

      if((pTemp = GetHostByName("rotate.aprs2.net")) == NULL) {
         break;
      }
      pClient->HisAdr.i.sin_addr.s_addr = IP_FROM_HOSTENT(pTemp,0);

      if((pClient1 = avl_replace(ClientTree,pClient)) != NULL) {
         pClient1->bInClientTree = FALSE;
         DuplicateClientsDeleted++;
         LOG_ERROR(("ServerRequest(): Duplicate client removed from tree.\n"));
         DeleteClient(pClient1);
      }
      pClient->bInClientTree = TRUE;

      pClient->State = AprsConnected;

      i = connect(pClient->Socket,&pClient->HisAdr.s,sizeof(pClient->HisAdr));

      if(i == SOCKET_ERROR) {
         if(ERROR_CODE == ERROR_IN_PROGRESS) {
         // The expected result, connect takes a while
            D2PRINTF(("ServerRequest(), connect() returned ERROR_IN_PROGRESS.\n"));
         // Wait for up to SERVER_REQ_TO millseconds for the connection
            SetTimeout(pClient,APRS_CONNECT_TO); 
            SET_WAIT4_CONNECT(pClient);
            Ret = 0;
         }
         else {
         // Some kind of error other than in progress
            LOG_ERROR(("ServerRequest(), connect() failed, %s",
                       Err2String(ERROR_CODE)));
            OpComplete(pClient,ERROR_CODE);
         }
      }
      else {
      // That certainly was quick !
         LOG_WARN(("ServerRequest(), connect() didn't return an error(!).\n"));
         AprsConnected(pClient);
         Ret = 0;
      }
   } while(FALSE);

   if(Ret != 0) {
      if(pClient != NULL) {
         DeleteClient(pClient);
      }
   }
}
#endif
