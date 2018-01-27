/* Copyright (C) 2007 GFRN systems

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

   $Log: nodes.cpp,v $
   Revision 1.26  2012/12/09 16:29:15  wb6ymh
   Corrected memset() argument order in ExecuteCommand().

   Revision 1.25  2010/11/17 04:52:00  wb6ymh
   Added CmdTxPower.

   Revision 1.24  2010/01/07 18:49:58  wb6ymh
   Modified StartNextAnnoucement to ensure TTS input file ends with a newline.
   Some (flite) TTS programs don't process input w/o an newline at the end
   of the line.

   Revision 1.23  2009/10/02 17:09:03  wb6ymh
   1. Changed "Total" column label to "Ave" on rxlevel command.
   2. Modified the .rxlevel command to reset the minimum value to 32767 and the
      maximum value to -32768 rather than clearing them.

   Revision 1.22  2009/09/13 19:40:46  wb6ymh
   1. Removed SayConnectResult handler, this was useless as it just said
      "connecting to".
   2. Added GetConfigVarsPtr to return a pointer to the appropriate configuration
      variable array for the CurrentNode.
   3. Replaced all references to bInConf with references to bConnected.
   4. Added CmdRxLevel and supporting code.
   5. Added support for the new configuration variable DefaultToneLevel.
   6. Added support for IRLPAnnouncements configuration variable.
   7. Corrected bugs in DTMF digit timing measurement.

   Revision 1.21  2009/07/19 14:06:58  wb6ymh
   Added initialization of LastActivity to Node constructor to keep valgrind
   happy.

   Revision 1.20  2009/07/03 18:11:51  wb6ymh
   1. Modified CmdSayStatus to announce stations which are being disconnected
      as well as stations that are in the process of being connected.
   2. Added audio priority support.
   3. Added -v command line to CmdLink to dump audio priorities.
   4. Added -P command line to CmdLink to override a ports default audio priority
      for a particular link.
   5. Modified CmdLink properly handle a .link -m <port1> <port2> command when
      the ports were previously linked in transceiver mode.
   6. Modified StartNextAnnoucement to use the TempDir for temp TTS files.
   7. Corrected a minor memory leak that occurred when a specific link was
      unlinked (.unlink <port1> <port2>).
   8. Deleted all of the toneburst code from GetTxAudio, it's now handled in
      PollCOS.
   9. Modified the PCM command to allow PCM files RawAudioIn and RxCtcssFilter
      to be played back for debugging.
   10. Added support for recording the "PCM" debug streams CtcssPeakDet and
       CtcssDecode.
   11. Deleted all of the toneburst cruft from SendCtone.
   12. Deleted the obsolete ToneBurstAccessCheck().

   Revision 1.19  2009/07/01 15:35:20  wb6ymh
   Added PriorityText.

   Revision 1.18  2009/04/04 18:03:38  wb6ymh
   1. Made ExecuteCommand and ExpandCommand members of the Node class.
   2. Added code to ExecuteCommand to save and restore CurrentNode.
   3. Corrected a bug in the saystatus command that caused pending connections to
      be announced as connected.  The saystatus command will now say:
      "I am currently connected to <station list>" and
      "I am trying to connect to <station list>" separately.
   4. Modified DoLinks to set any underlying VoIP connections to permanent when
      creating permanent port links.
   5. Modified UnlinkAll to allow nailedup links to be deleted by the ~Node.
   6. Modified Node::AddRx to set LNK_FLG_NAILED_UP on existing links so a
      link can be made permanent after it's been established.
   7. Added code to GetTxAudio to disconnect a connection after playing the
      Welcome file when DisconnectAfterWelcome is set.
   8. Added code to Node::CmdToneGen() to initialize the generators
      MaxPlayWithoutPause and MinPlayBackPause variables based on the VoIP
      port or global configuration variables.
   9. Added checks for CurrentNode == NULL to CmdToneGen.

   Revision 1.17  2009/02/16 00:24:29  wb6ymh
   Modified SendCTone to prevent Ctones from being sent to tone burst controlled
   transmitters that have not been enabled by a user's tone burst.

   Revision 1.16  2009/02/14 15:26:31  wb6ymh
   1. Made DumpStatus a Node method rather than a CPort only method.  Modified
      CmdSetPort to match.
   2. Added -a command line option to the CmdSendAx25Beacon to force an update
      to be sent to the APRS-IS system.
   3. Modified CmdPCM to take a const char * instead of a char * to ease calling
      with fixed strings.
   4. Modified UnlinkAll to ignore permanent ports.
   5. Removed calls to the obsolete KickStartAudioOut kludge.
   6. Massive restructuring of the audio buffering algorithm to minimize port to
      port audio delays.
   7. Replaced orignal cw ID code with tone generator based CW to simplify things.
   8. Added bDtmfDetect status bit.
   9. Added RawAudioIn, AudioOut, and DtmfIn test points to the .pcm command.
   10. Modified CmdDtmfDecode to check CurrentNode before using it blindly.
   11. Corrected a premature DTMF command timeout when buttons where pressed
       slowly.  Previously the command timeout was started on button press, now
       it's started on button release.
   12. Added ToneBurstAccessCheck.

   Revision 1.15  2009/01/04 03:43:49  wb6ymh
   1. Added incomplete support for permanent links (.link -p).
   2. Added CmdSendAx25Beacon.
   3. Added .pcm command.
   4. Corrected bug in SpellCall: don't convert to lower case unless configured
      to use a TTS system.
   5. Added support for the new tonegenerator IDsource attribute.
   6. Modified ScaleMixedAudio to leave headroom for CTCSS to be added after the
      mix and scalling.
   7. Added TxCtcssFilter support.
   8. Added TxPreemphasis support.

   Revision 1.14  2008/08/09 15:54:47  wb6ymh
   Added support for port configuration variable ConnectAnnouncements to
   allow connect and disconnect announcements to be enabled/disabled on a
   per port basis.

   Revision 1.13  2008/07/23 15:54:02  wb6ymh
   1. Modified MixAudio to mix 32 bit samples when Samples < 0 and 16 bit
      samples when Samples > 0.
   2. Added an argument to point to the output buffer to MixAudio.
   3. Modified GetTxAudio to loop calling GetRxAudio until audio buffer is filled
      or GetRxAudio fails to provide data.
   4. Modified GetTxAudio to handle negative number of samples returned by
      GetRxAudio (which indicates that the returned samples are 32 bits).

   Revision 1.12  2008/06/26 16:37:03  wb6ymh
   1. Modified the DTMF muting logic.  The MuteDTMF port configuration variable
      now specifies if DTMF tones should be muted before reaching the port's
      transmitter.

   2. Modified logic to announce VoIP connect and disconnect events to ports
      listed in the VoipInMap and VoipOutMap configuration variables.

   3. Modified ExpandCommand to replace "$TLB_PORT" within command strings with
      the name of the port that issued the command.

   4. Modified DoLinks to fully unlink ports, previously only half of the link
      was unlinked.

   5. Added RadioCtrl Sync method calls to CmdFrequency, CmdTxOffset, CmdTxTone,
      and CmdRxTone.

   6. Added CmdFrequency.

   7. Added decrement of Transmitters to the Unlink all command.  Fixes massive
      memory leak caused by failure to delete uncompressed audio data after an
      Unlink all command is used.

   8. Added call to DeleteTx in DeleteRx to fully unlink ports.

   9. Added support for tonegen completion command to GetTxAudio.

   10. Modified SaveDTMF's handling of DtmfCTCSSMode to ensure CTCSS/COS timing
       doesn't mask the end of a decode.

   11. Added include file support to LoadCommandFile.

   Revision 1.11  2008/05/14 22:56:16  wb6ymh
   Replaced <sys/limits.h> file with <sys/param.h>.  FreeBSD bitches if you
   try to include <sys/limits.h>

   Revision 1.10  2008/05/14 21:34:17  wb6ymh
   1. Moved include of common.h to after all standard headers to correct compile
      errors with GCC 4.2.x.  GCC 4.2.x's include files #undef the min macro!
   2. Changed a bunch of functions to take const char * rather than char *
      to fix compile problems with GCC 4.2.x.
   3. Added CmdRxTone, CmdTxTone, CmdRxFrequency,and CmdTxOffset.
   4. Added support for -s (spell) switch to CmdSay.  This causes the phrase
      being spoken to be treated as a callsign, forcing the TTS system to spell
      the phrase out.
   5. Replaced calls to obsolete function SayCall to calls to SayPhrase with
      a SAY_FLAG_CALLSIGN flag.
   6. Added support for phonetic spelling of callsigns.
   7. Added support for the new configuration variable CallSignSpelling.

   Revision 1.8  2008/04/25 05:24:59  wb6ymh
   Removed initialization of DisableRateConversion for Nodes constructor.

   Revision 1.7  2008/04/25 05:05:49  wb6ymh
   1. Added support for command prefix decoding.
   2. Added numerious checks to make sure the current port still exists before
      trying to use it.
   3. Added code to DecodeDTMF to set CurrentPort so the TLB_PORT environment
      variable gets set to the port issuing the command.

   Revision 1.6  2008/04/11 14:12:03  wb6ymh
   Removed .dtmfgen command, dtmf generation is now handled by the .tonegen
   command.

   Revision 1.5  2008/03/18 20:03:40  wb6ymh
   Modified code to use bCwIsID and bAnnoucementIsID to track which 
   transmissions should set LastActivity.  Previously LastActivity wasn't set 
   by anything other than an announcement which prevented IDs from running 
   when they should.

   Revision 1.4  2008/03/12 21:43:05  wb6ymh
   Added code to free NodeName to Node::~Node.

   Revision 1.3  2008/03/11 14:06:30  wb6ymh
   Modified SendCTone to check LNK_FLG_CTONE (set by CToneMap)  before sending
   Ctones to a node.

   Revision 1.2  2008/03/09 18:19:47  wb6ymh
   Added headers needed by FreeBSD.

   Revision 1.1  2008/03/09 17:10:13  wb6ymh
   Inital import - files broken out of old audio.cpp and audio.h due to
   major restructuring of the internal infrastructure to treat VoIP connections
   as pseudo ports.

*/ 

// FreeBSD, Linux, etc..
#include <stdio.h>
#include <stdlib.h>
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
#include <unistd.h>
#include <errno.h>
#include <string.h>
#ifdef HAVE_FCNTL_H
   #include <fcntl.h>
#endif
#include <netdb.h>
#include <ctype.h>
#ifdef HAVE_SYS_TIMEB_H
   #include <sys/timeb.h>
#endif
#include <math.h>
#include <typeinfo>
#include <sys/param.h> // for PATH_MAX

#include <sys/file.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "common.h"
#include "avl.h"
#include "main.h"
#include "configvars.h"
#include "configfile.h"
#include "conference.h"
#include "linkbox.h"
#include "eventhook.h"
#include "tbd.h"

#include "nodes.h"
#include "ports.h"
#include "bitstream.h"
#include "g726.h"
#include "voip.h"
#include "filters.h"
#include "ctcss.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define TTS_OUT_FILENAME   "tts.wav"
#define TTS_IN_FILENAME    "tts.in"

#define MAX_MIXED_LEVEL 32000

#define LLOG(x) if(Debug & DLOG_LINKS) LogErr x
#define CLOG(x) if(Debug & DLOG_DTMF) LogErr x
#define ALOG(x) if(Debug & DLOG_VOIP_AUDIO) LogErr x
#define TLOG(x) if(Debug & DLOG_TBURST) LogErr x

/*
bytes      variable      description
0  - 3     'RIFF'/'RIFX' Little/Big-endian
4  - 7     wRiffLength   length of file minus the 8 byte riff header
8  - 11    'WAVE'
12 - 15    'fmt '
16 - 19    wFmtSize       length of format chunk minus 8 byte header 
20 - 21    wFormatTag     identifies PCM, ULAW etc
22 - 23    wChannels      
24 - 27    dwSamplesPerSecond  samples per second per channel
28 - 31    dwAvgBytesPerSec    non-trivial for compressed formats
32 - 33    wBlockAlign         basic block size
34 - 35    wBitsPerSample      non-trivial for compressed formats
*/

typedef struct {
   char  Riff[4];             // 'RIFF'/'RIFX' Little/Big-endian
   int   wRiffLength;         // length of file minus the 8 byte riff header
   char  Wave[4];             // 'WAVE'
   char  Fmt[4];              // 'fmt '
   int   wFmtSize;            // length of format chunk minus 8 byte header 
   short wFormatTag;          // identifies PCM, ULAW etc
   short wChannels;
   int   dwSamplesPerSecond;  // samples per second per channel
   int   dwAvgBytesPerSec;    // non-trivial for compressed formats
   short wBlockAlign;         // basic block size
   short wBitsPerSample;      // non-trivial for compressed formats
} WaveHdr;

struct avl_table *NodeTree;   // sorted by node name
int bTTS_Running = FALSE;
int MixBuffer[AUDIO_BUF_SIZE];

// scratch buffers for audio transmission
int16 AudioBuf[AUDIO_BUF_SIZE];  
int16 MasterBuf[AUDIO_BUF_SIZE];  
int16 SlaveBuf[AUDIO_BUF_SIZE];  

int SayDisconnectResult(ClientInfo *p,ConfClient *pCC,char *Arg);

struct SayResultHandler {
   const char *Cmd;
   int (*Handler)(ClientInfo *p,ConfClient *pCC,char *Arg);
} SayResultHandlers[] = {
   {"disconnect", SayDisconnectResult},
   { NULL}  // end of table
};

const char *StdTbdCommandText[] = {
   // command not found or ambiguous
   "unknown command",      
   
   // command requires disk and disk is disabled
   "no disk is available"  
   
   // Specified station not found
   "station not found",    
   
   // no info available for station (.info cmd)
   "no information on station is available", 

   // invalid command argument count
   "error, invalid number command arguments",

   // Already connected (.connect cmd)
   "you are already connected to that station",

   // no one is talking (.mute cmd)
   "no one is talking",

   // invalid command argument
   "command argument is not valid",

   // error opening a file
   "error can not open file",

   // data is chat text
   "",

   // timeout waiting for response from tbd ... it's probably not running
   "error timeout",

   // last "command" was sent as chat text
   "",

   // Specified port not found
   "port not found",
   
   // Say command has completed
   ""
};

/* A -> Z */
const char *CwAlphaLookup[] = {
   ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",
   ".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",
   ".--","-..-","-.--","--.."
};

/* 0 -> 9 */
const char *CwNumericLookup[] = {
   "-----",".----","..---","...--","....-",".....","-....",
   "--...","---..","----."
};

const struct CwMiscLookupTAG {
   char  Char;
   const char  *Morse;
} CwMiscLookup[] = {
   {'?', "..--.."},
   {'/', "-..-."},
   {'.', ".-.-.-"},
   {',', "--..--"},
   {'-', "-...-"},   // aka BT
   {0}   // end of table
};

const char *PriorityText[] = { "low", "medium", "high" };

struct EventHandler_TAG {
   const char *EventDesc;
   void (*EventHandler)(char *Args);
} EventHandlers[] = {
   { NULL } // end of table
};

void TTSCB(int Pid, int Status)
{
// The TTS program has exited...
   bTTS_Running = FALSE;
}

int StringIsInList(char *List,char *String)
{
   int Ret = FALSE;
   char *cp = strstr(List,String);
   char LastChar;

   if(cp != NULL) do {
      if(cp == List || cp[-1] == ' ' || cp[-1] == ',') {
      // substring starts at the right place
         LastChar = cp[strlen(String)];
         if(LastChar == 0 || LastChar == ' ' || LastChar == ',') {
         // substring ends at the right place
            Ret = TRUE;
         }
      }
   } while(FALSE);
   return Ret;
}

struct config_entry *GetConfigVarsPtr()
{
   struct config_entry *Ret = ConfigVars;

   if(dynamic_cast<VoipNode*> (CurrentNode) != NULL) {
      Ret = VoipConfigVars;
   }

   return Ret;
}


void Node::ExecuteCommand(char *Command)
{
   ConfClient cc;
   char Call[] = "RF";
   Node *SavedNode= CurrentNode;
   
   CurrentNode = this;

   memset(&cc,0,sizeof(cc));
   cc.bCmdLine = TRUE;
   cc.bSysop = TRUE;
   cc.bAdmin = TRUE;
   cc.bRfUser = TRUE;
   cc.Callsign = Call;
   cc.CallPlus = cc.Callsign;
   cc.pCS = piLinkConf;

   LOG_NORM(("%s executing command \"%s\"\n",
             CurrentNode == NULL ? "(NULL)" : CurrentNode->NodeName,Command));
   ConferenceCmd(cc.pCS->pAudio,&cc,Command);
   CurrentNode = SavedNode;
}

void Node::ExpandCommand(char *Command,char *DtmfCommand,char *DtmfWildCards)
{
   char *cp;

   if(strchr(Command,'$') == NULL) {
   // No special variables to expand, just execute it as is
      ExecuteCommand(Command);
   }
   else {
      CText Cmd;

   // Count the space that will be needed when the command is expanded
      cp = Command;

      while(*cp != 0) {
         if(*cp == '$') {
         // Expand special variable
            cp++;
            switch(*cp++) {
               case '1':
               // We'll replace "$1" with DtmfCommand
                  Cmd.print(DtmfCommand);
                  break;

               case '2':
               // We'll replace "$2" with DtmfWildCards
                  Cmd.print(DtmfWildCards);
                  break;

               case 'T':
               // TLB_PORT ?
                  if(strncmp(cp,"LB_PORT",7) == 0) {
                     Cmd.print(CurrentNode->NodeName);
                     cp += 7;
                  }
                  else {
                     Cmd.print("?T");
                  }
                  break;

               default:
                  Cmd.print("?%c",cp[-1]);
                  break;
            }
         }
         else {
            Cmd.print("%c",*cp++);
         }
      }
      ExecuteCommand(Cmd.Buf);
   }
}

const char *CwLookUp(char CwChar)
{
   int   i;
   const char *Ret = "";

   if(isalpha(CwChar)) {
      Ret = CwAlphaLookup[toupper(CwChar)-'A'];
   }
   else if(isdigit(CwChar)) {
      Ret = CwNumericLookup[CwChar-'0'];
   }
   else for(i = 0; CwMiscLookup[i].Char != 0; i++) {
      if(CwChar == CwMiscLookup[i].Char) {
         Ret = CwMiscLookup[i].Morse;
         break;
      }
   }

   return Ret;
}

int NodeCompare(const void *avl_a,const void *avl_b,void *avl_param)
{
   char *NodeNameA = ((NamedNode *) avl_a)->NodeName;
   char *NodeNameB = ((NamedNode *) avl_b)->NodeName;
   
   return STRCMP(NodeNameA,NodeNameB);
}

Node *FindNode(char *NodeName)
{
   NamedNode NodeLookup;

   NodeLookup.NodeName = NodeName;
   return (Node *) avl_find(NodeTree,&NodeLookup);
}

// .port <port name>
void CmdSetPort(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   struct avl_traverser avl_trans;
   Node *pNode;
   NamedNode NodeLookup;
   ClientBufPrint prn(p);

   if(*Arg == 0) {
   // No argument, list ports
      prn.print("Available ports:\r");
      pNode = (Node *) avl_t_first(&avl_trans,NodeTree);
      while(pNode != NULL) {
         if(pNode->Enabled()) {
            pNode->DumpStatus(&prn);
         }
         pNode = (Node *) avl_t_next(&avl_trans);
      }
   }
   else {
      NodeLookup.NodeName = Arg;
      if((pNode = (Node *) avl_find(NodeTree,&NodeLookup)) != NULL) {
         CurrentNode = pNode;
      }
      else {
         prn.print("Port \"%s\" not found\r",Arg);
         Rcode = TBD_PORT_NOT_FOUND;
      }
   }
}

char *FindToneSpec(int ID)
{
   char *Ret = NULL;
   SavedToneSpec *pSpec = ToneSpecs;

   while(pSpec != NULL) {
      if(pSpec->ID == ID) {
      // Match
         Ret = pSpec->ToneSpec;
         break;
      }
      pSpec = pSpec->Link;
   }
   return Ret;
}

void DtmfCallback(void *user_data, int code, int level)
{
   Node *pClass = (Node *) user_data;

   pClass->SaveDTMF(code == 0 ? ' ' : code);
}

void EndPointEvent(char *EventText)
{
   int Len;
   char *cp;
   int i;

   for(i = 0; EventHandlers[i].EventDesc != NULL; i++) {
      Len = strlen(EventHandlers[i].EventDesc);
      if(strncmp(EventText,EventHandlers[i].EventDesc,Len) == 0) {
         if((cp = strchr(EventText,' ')) != NULL) {
            cp++;
         }

         EventHandlers[i].EventHandler(cp);
         break;
      }
   }
}

// A callsign is entered as using two digits for each letter and number 
// in the callsign. The first digit is the key on which the letter appears 
// (using 1 for Q and Z), and the second digit is 1, 2, or 3, to indicate 
// which letter is being entered.  Numbers are enter as the digit followed 
// by a 0.  The pound key is used to indicate the end of the string.
// For example, the letter "K" is entered as "52", the letter "Q" is 
// entered as "11", and the digit "7" is entered as "70".
//
// Callsigns need not be entered in full.  If a partial callsign is entered, 
// thebridge will find the first match among the stations currently logged on.
// If no match is found among the stations currently logged on, 
// thebridge will say "NOT FOUND".

char DTMF2CharLookup[] = {
   0x21, // 'A'
   0x22, // 'B'
   0x23, // 'C'
   
   0x31, // 'D'
   0x32, // 'E'
   0x33, // 'F'
   
   0x41, // 'G'
   0x42, // 'H'
   0x43, // 'I'

   0x51, // 'J'
   0x52, // 'K'
   0x53, // 'L'

   0x61, // 'M'
   0x62, // 'N'
   0x63, // 'O'

   0x71, // 'P'
   0x11, // 'Q'
   0x72, // 'R'
   0x73, // 'S'

   0x81, // 'T'
   0x82, // 'U'
   0x83, // 'V'

   0x91, // 'W'
   0x92, // 'X'
   0x93, // 'Y'
   0x12, // 'Z'
};

char *DTMF2Call(char *Digit)
{
   static char Callsign[MAX_CALL_LEN+1];
   int   i;
   unsigned int   j;
   char  FirstDigit;
   char  c = 0;
   char  lookup;

   Callsign[0] = 0;

   for(i = 0; i < MAX_CALL_LEN; ) {
   // Get first digit of pair
      c = *Digit++;
      if(!isdigit(c)) {
      // we're done
         break;
      }
      FirstDigit = c;

   // Get second digit of pair
      c = *Digit++;
      if(!isdigit(c)) {
      // we're done
         break;
      }
      
      if(c == '0') {
      // digit
         Callsign[i] = FirstDigit;
      }
      else {
         lookup = ((FirstDigit << 4) & 0xf0) | (c & 0xf);
         for(j = 0; j < sizeof(DTMF2CharLookup); j++) {
            if(DTMF2CharLookup[j] == lookup) {
               Callsign[i] = (char) j + 'A';
               break;
            }
         }

         if(j == sizeof(DTMF2CharLookup)) {
         // invalid combination
            Callsign[0] = 0;
            break;
         }
      }
   }

   if(c != 0 && c != '#') {
   // invalid combination
      Callsign[0] = 0;
   }
   else {
   // Add terminating null
      Callsign[i] = 0;
   }

   return Callsign;
}

void CmdSayStatus(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ConfClient *pCC;
   struct avl_traverser avl_trans;
   const char *Msg[] = {
      "I am currently connected to ",
      "I am trying to connect to ",
      "I am disconnecting from "
   };
   
   for(int i = 0; i < 3; i++) {
      int Connections;
      int Announced;
   // Count active connections

      Connections = 0;
      Announced = 0;

      pCC = (ConfClient *) avl_t_first(&avl_trans,piLinkConf->ConfTree);
      while(pCC != NULL) {
         pCC->bScratch = FALSE;
         switch(i) {
            case 0:  // connected stations
               if(pCC->bConnected) {
                  Connections++;
                  pCC->bScratch = TRUE;
               }
               break;

            case 1:  // stations we're in the process of connecting to
               if(!pCC->bConnected && !pCC->bKicked) {
                  Connections++;
                  pCC->bScratch = TRUE;
               }
               break;

            case 2:  // stations we're disconnecting from
               if(!pCC->bConnected && pCC->bKicked) {
                  Connections++;
                  pCC->bScratch = TRUE;
               }
               break;   
         }
         pCC = (ConfClient *) avl_t_next(&avl_trans);
      }
   
      if(Connections > 0) {
      // Announce current connections
         CurrentNode->SayPhrase(Msg[i]);
         if(Connections > 1) {
            CurrentNode->SayPhrase("the following stations, ");
         }
         pCC = (ConfClient *) avl_t_first(&avl_trans,piLinkConf->ConfTree);
         while(pCC != NULL) {
            if(pCC->bScratch) {
               if(Connections > 1 && (Connections - Announced) == 1) {
                  CurrentNode->SayPhrase("and ");
               }
               CurrentNode->SayPhrase(pCC->Callsign,SAY_FLAG_CALLSIGN);
               Announced++;
            }
            pCC = (ConfClient *) avl_t_next(&avl_trans);
         }
      }
   }

   CurrentNode->SayPhrase(". I have been running for ");
   CurrentNode->SayPhrase(TimeLapse2String(BootTime,FALSE));
}

void ScriptExitCB(int Pid, int Status)
{  
   // Nothing to do, this is just here to prevent creating a zombie army
}

void CmdScript(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
#define MAX_ARGS  16
   char *argv[MAX_ARGS];
   pid_t Pid = -1;
   char  *cp = Arg;
   int   i = 0;

// NB: make sure all buffered streams are written to disk,
// otherwise the data will be written twice, once by the main
// process and once by the forked process !

   if(fflush(NULL)) {
      LOG_ERROR(("%s: fflush failed %s",__FUNCTION__,Err2String(errno)));
   }

   while(i < MAX_ARGS && cp != 0) {
      argv[i++] = cp;
      if((cp = strchr(cp,' ')) != NULL) {
         *cp++ = 0;
         while(isspace(*cp)) {
            cp++;
         }
      }
   }

   if(i == MAX_ARGS) {
      BufPrintf(p,"too many arguments\r");
      LOG_ERROR(("%s: error: too many arguments\n",__FUNCTION__));
   }
   else {
      argv[i] = NULL;
      if((Pid = fork()) < 0) {
         LOG_ERROR(("%s: fork failed %s",__FUNCTION__,Err2String(errno)));
      }
      else if(Pid == 0) {
         int Error;
      // child
         setenv("TLB_PORT",CurrentNode->NodeName,TRUE);
         Error = execvp(argv[0],argv);
         exit(1);
      }
      else {
      // Parent
         LOG_ERROR(("%s: script \"%s\" started, Pid %d\n",__FUNCTION__,Arg,
                    (int) Pid));
         RegisterExitCB(Pid,ScriptExitCB);
      }
   }
#undef MAX_ARGS
}


// If this is an VoipNode then make the underlying connection permanent
void MakeConnectionPermanent(Node *pNode)
{
   VoipNode *pVoip = dynamic_cast<VoipNode*> (pNode);
   
   if(pVoip != NULL) {
      pVoip->mCC->bPermanent = TRUE;
   }
}

// <To> <From> [<From>] ...
// <To> and <From> can be a Voip connection (callsign) or a port name
void DoLinks(
   ClientInfo *p,
   char *Arg,
   int bLink,
   int bMonitor,
   int bPermanent,
   int AudioPriority)
{
#define MAX_ARGS  16
   char *PortNames[MAX_ARGS];
   Node *TargetNode[MAX_ARGS];
   char  *cp = Arg;
   int i;
   int ArgCount = 0;
   Node *pToNode;

   memset(TargetNode,0,sizeof(TargetNode));

   do {
      while(ArgCount < MAX_ARGS && *cp) {
         PortNames[ArgCount++] = cp;
         while(*cp && !(isspace(*cp) || *cp == ',')) {
            cp++;
         }

         while(*cp && (isspace(*cp) || *cp == ',')) {
            *cp++ = 0;
         }
      }

      if(ArgCount == MAX_ARGS) {
         BufPrintf(p,"Error: too many arguments\r",cp);
         Rcode = TBD_ERR_INVALID_ARG;
         break;
      }

      for(i = 0; i < ArgCount; i++) {
      // Find specified port or Voip connection
         if((TargetNode[i] = FindNode(PortNames[i])) == NULL) {
         // node not found
            BufPrintf(p,"Error: can't find port or connection named \"%s\"\r",
                      PortNames[i]);
            Rcode = TBD_ERR_INVALID_ARG;
            break;
         }
      }

      if(Rcode != 0) {
         break;
      }

      pToNode = TargetNode[0];

      if(pToNode == NULL) {
         LOG_ERROR(("%s#%d: Internal error!\n",__FUNCTION__,__LINE__));
         BufPrintf(p,"Internal error @ %s#%d\r",__FUNCTION__,__LINE__);
         Rcode = TBD_ERR_INVALID_ARG;
         break;
      }

      if(ArgCount == 1 && !bLink) {
      // unlink everything from specified port/connection
         if(pToNode->UnlinkAll()) {
            BufPrintf(p,"Internal error @ %s#%d\r",__FUNCTION__,__LINE__);
            Rcode = TBD_ERR_INVALID_ARG;
         }
         break;
      }

      if(ArgCount < 2) {
         BufPrintf(p,"Error: not enough arguments\r");
         Rcode = TBD_ERR_INVALID_ARG;
         break;
      }

      BufPrintf(p,"%slinking ",bLink ? "" : "un");

      if(bLink && bPermanent) {
         MakeConnectionPermanent(pToNode);
      }

      for(i = 1; i < ArgCount; i++) {
         BufPrintf(p,"%s%s",i > 1 ? ", " : "",TargetNode[i]->NodeName);
         if(bLink) {
            pToNode->AddRx(TargetNode[i],bPermanent,AudioPriority);
            if(bMonitor) {
               TargetNode[i]->DeleteRx(pToNode);
            }
            else {
               TargetNode[i]->AddRx(pToNode,bPermanent,AudioPriority);
            }

            if(bPermanent) {
               MakeConnectionPermanent(TargetNode[i]);
            }
         }
         else {
            pToNode->DeleteRx(TargetNode[i]);
            TargetNode[i]->DeleteRx(pToNode);
         }
      }
      if(bLink) {
         BufPrintf(p," to %s%s\r",PortNames[0],bMonitor ? " (monitor only)":"");
      }
      else {
         BufPrintf(p," from %s\r",PortNames[0]);
      }
   } while(FALSE);
#undef MAX_ARGS
}

void CmdLink(ClientInfo *p,ConfClient *pCC1,char *Args)
{
   int bFirst;
   int Option = 0;
   int bMonitor = FALSE;
   int bPermanent = FALSE;
   int bVerbose = FALSE;
   int bUsageDisplayed = FALSE;
   int AudioPriority = -1;
   char *Arg = Args;

   if(*Args) {
      while((Option = GetCmdOptions(&Arg,"hmpP:v")) != -1) {
         switch(Option) {
            case 'm':
            // Monitor command
               bMonitor = TRUE;
               break;

            case 'p':
            // Permanent link
               bPermanent = TRUE;
               break;
   
            case 'P':
            // Set audio priority of receivers
               if(sscanf(CmdArg,"%d",&AudioPriority) != 1 || 
                  AudioPriority < LOW_AUDIO_PRI || 
                  AudioPriority > HIGH_AUDIO_PRI)
               {
                  BufPrintf(p,"Error: Invalid audio priority \"%s\"\r",CmdArg);
                  Rcode = TBD_ERR_INVALID_ARG;
               }
               break;

            case 'v':
            // Verbose
               bVerbose = TRUE;
               break;

            case '?':
               Rcode = TBD_ERR_INVALID_ARG;
            case 'h':
               BufPrintf(p,"Usage: .link [-p] [-P priority] [-m] <destination>"
                         " <source> [<source>...]\r");
               BufPrintf(p,"       .link [-v]\r");
               bUsageDisplayed = TRUE;
               break;
   
            default:
               BufPrintf(p,"Error: Unknown switch \"%c\"\r",Arg[1]);
               Rcode = TBD_ERR_INVALID_ARG;
               break;
         }
      }
   }
   
   if(*Args == 0 || (bVerbose && !bUsageDisplayed)) {
   // No argument or verbose, list linked ports
      BufPrintf(p,"Port mapping:\n");
      struct avl_traverser avl_trans;
      LinkedNode *pLink;

      Node *pNode = (Node *) avl_t_first(&avl_trans,NodeTree);
      while(pNode != NULL) {
         if(pNode->Enabled()) {
            BufPrintf(p,"%10s: ",pNode->NodeName);
            bFirst = TRUE;

            pLink = pNode->pReceivers;
            while(pLink != NULL) {
               if(pLink->pRx->Enabled()) {
                  BufPrintf(p,"%s%s",bFirst ? "" : ", ",pLink->pRx->NodeName);
                  bFirst = FALSE;
                  if(bVerbose) {
                     BufPrintf(p,"(%s%s)",PriorityText[pLink->AudioPriority],
                               pLink->Flags & LNK_FLG_NAILED_UP ? ",p" : "");
                  }
                  else {
                     if(pLink->Flags & LNK_FLG_NAILED_UP) {
                        BufPrintf(p,"(p)");
                     }
                  }
               }
               pLink = pLink->pNextRx;
            }
            BufPrintf(p,"%s\r",bFirst ? "none" : "");
         }
         pNode = (Node *) avl_t_next(&avl_trans);
      }
   }
   else if(!bUsageDisplayed) {
      DoLinks(p,Arg,TRUE,bMonitor,bPermanent,AudioPriority);
   }
}

void CmdUnLink(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   Node *pNode;
   struct avl_traverser avl_trans;
   int UnLinkAll = 0;
   const char *UnlinkMsg[] = {
      "ports and voip connections",
      "ports",
      "voip connections",
   };

   if(*Arg == 0) {
   // No argument
      BufPrintf(p,"usage:\r");
      BufPrintf(p,"unlink all\r");
      BufPrintf(p,"unlink rf\r");
      BufPrintf(p,"unlink voip\r");
      BufPrintf(p,"unlink <destination> [<source>...]\r");
      Rcode = TBD_ERR_INVALID_ARG;
   }
   else if(STRCMP(Arg,"all") == 0) {
      UnLinkAll = 1;
   }
   else if(STRCMP(Arg,"rf") == 0) {
      UnLinkAll = 2;
   }
   else if(STRCMP(Arg,"voip") == 0) {
      UnLinkAll = 3;
   }

   if(UnLinkAll != 0) {
      pNode = (Node *) avl_t_first(&avl_trans,NodeTree);
      while(pNode != NULL) {
         if(UnLinkAll == 1 ||
            (UnLinkAll == 2 && dynamic_cast<CPort *> (pNode) != NULL) || 
            (UnLinkAll == 3 && dynamic_cast<VoipNode *> (pNode) != NULL))
         {
            if(pNode->UnlinkAll()) {
               BufPrintf(p,"Internal error @ %s#%d\r",__FUNCTION__,__LINE__);
               Rcode = TBD_ERR_INVALID_ARG;
               break;
            }
         }
         pNode = (Node *) avl_t_next(&avl_trans);
      }
      BufPrintf(p,"all %s unlinked\r",UnlinkMsg[UnLinkAll-1]);
   }
   else {
      DoLinks(p,Arg,FALSE,FALSE,FALSE,-1);
   }
}

CPort *CheckCurrentPort(CText *p,const char *Function)
{
   CPort *pPort = NULL;

   if(CurrentNode == NULL) {
      Rcode = TBD_PORT_NOT_FOUND;
      p->print("Error: no port is selected\r");
   }
   else if((pPort = dynamic_cast<CPort *> (CurrentNode)) == NULL) {
      Rcode = TBD_PORT_NOT_FOUND;
      p->print("Node %s does support %s\r",CurrentNode->NodeName,Function);
   }

   return pPort;
}


void CmdRxTone(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ClientBufPrint Prn(p);
   CPort *pPort = CheckCurrentPort(&Prn,"RxTone");

   if(pPort != NULL) {
      pPort->SetRxTone(Arg,&Prn);
      pPort->SyncRadioCtrl();
   }
}

void CmdTxTone(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ClientBufPrint Prn(p);
   CPort *pPort = CheckCurrentPort(&Prn,"TxTone");

   if(pPort != NULL) {
      pPort->SetTxTone(Arg,&Prn);
      pPort->SyncRadioCtrl();
   }
}


// set everything at once:
// <rxfrequency>,<txoffset>,<txtone>,<rxtone>
void CmdFrequency(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ClientBufPrint Prn(p);
   CPort *pPort = CheckCurrentPort(&Prn,"Frequency");

   if(pPort != NULL) {
      pPort->SetFrequency(Arg,&Prn);
      pPort->SyncRadioCtrl();
   }
}

void CmdRxFrequency(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ClientBufPrint Prn(p);
   CPort *pPort = CheckCurrentPort(&Prn,"RxFreq");

   if(pPort != NULL) {
      pPort->SetRxFreq(Arg,&Prn);
      pPort->SyncRadioCtrl();
   }
}

void CmdTxOffset(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ClientBufPrint Prn(p);
   CPort *pPort = CheckCurrentPort(&Prn,"TxOffset");

   if(pPort != NULL) {
      pPort->SetTxOffset(Arg,&Prn);
      pPort->SyncRadioCtrl();
   }
}

void CmdTxPower(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ClientBufPrint Prn(p);
   CPort *pPort = CheckCurrentPort(&Prn,"TxPower");

   if(pPort != NULL) {
      pPort->SetTxPower(Arg,&Prn);
      pPort->SyncRadioCtrl();
   }
}

void CmdSendAx25Beacon(ClientInfo *p,ConfClient *pCC1,char *Args)
{
   ClientBufPrint Prn(p);
   int bAprsIs = FALSE;
   int bError = FALSE;
   int bAx25 = FALSE;
   char *Arg = Args;
   int Option = 0;

   while((Option = GetCmdOptions(&Arg,"ax")) != -1) {
      switch(Option) {
         case 'a':   // bare
            bAprsIs = TRUE;
            break;

         case 'x':
            bAx25 = TRUE;
            break;

         case '?':   // invalid switch
            bError = TRUE;
            Prn.print("Usage:\r");
            Prn.print("sendbeacon [-a] [-x]\r");
            Prn.print("  -a - Send APRS-IS update\r");
            Prn.print("  -x - Send AX25 beacon to the current port\r");
            break;
      }
   }

   if(!bError) {
      if(!bAprsIs && !bAx25) {
      // backward compatibility: send AX25 beacon if no switches are specfied
         bAx25 = TRUE;
      }

      if(bAprsIs) {
         if(AprsIsEnable) {
         // Force an update to be sent
            LasttAprsIsTime = 0;
         }
         else {
            Prn.print("Error: APRS-IS is not enabled\r");
         }
      }

      if(bAx25) {
         CPort *pPort = dynamic_cast<CPort *> (CurrentNode);
         if(pPort != NULL) {
            pPort->CmdSendAx25Beacon(&Prn);
         }
         else {
            Prn.print("Error: APRS beacons can only be sent to RF ports\r");
         }
      }
   }
}

void CmdRxLevel(ClientInfo *p,ConfClient *pCC,char *Args)
{
   char *Arg = Args;
   int Option = 0;
   struct avl_traverser avl_trans;
   Node *pNode;
   ClientBufPrint Prn(p);
   Node *n;
   int bQuiet = FALSE;
   int bContinuous = FALSE;
   
   do {
      if((n = CurrentNode) == NULL) {
         Rcode = TBD_PORT_NOT_FOUND;
         Prn.print("Error: no port is selected\r");
         break;
      }


      while((Option = GetCmdOptions(&Arg,"cq")) != -1) {
         switch(Option) {
            case 'c':
               bContinuous = TRUE;
               break;

            case 'q':
               bQuiet = TRUE;
               break;

            case '?':   // invalid switch
               Rcode = TBD_ERR_INVALID_ARG;
               Prn.print("Usage:\r");
               Prn.print("rxlevel [-c] [-q]\r");
               Prn.print("  -c - Send level reports continuously\r"); 
               Prn.print("  -q - Quiet\r");
               break;
         }
      }

      if(Rcode != TBD_OK) {
         break;
      }

   // Turn off monitoring for this client for all ports, 
   // one at a time is confusing enough!
      pNode = (Node *) avl_t_first(&avl_trans,NodeTree);
      while(pNode != NULL) {
         if(pNode->MinMaxReplyPort == pCC->HisAdr.PORT) {
            pNode->MinMaxReplyPort = -1;
         }
         pNode = (Node *) avl_t_next(&avl_trans);
      }

      n->MinMaxReplyPort = bContinuous ? pCC->HisAdr.PORT : -1;

      if(!bQuiet) {
         Prn.print("Min\tMax\tAve\tRMS\tSamples\r");
      }

      if(n->MinMaxSamples == 0) {
         Prn.print("No data%s",bContinuous ? "" : "\r");
      }
      else {
         if(bContinuous) {
         // Wait for new data to come in to avoid two lines of data
            n->MinMaxSamples = 0;
         }
         else {
            n->PrintRxLevel(&Prn);
         }
      }
   } while(FALSE);
}

NamedNode::~NamedNode() 
{
}

Node::Node()
{
   bCwToneOn = FALSE;
   bElementSpace = FALSE;
   bRxActive = 0;
   bRxTimeout = FALSE;
   bTxTimeOut = 0;
   CommandFilename = NULL;
   CTCSSAngle = 0.0;
   CTCSSLevel = 0;
   CtcssTxLevel = 0;
   Ctone = 0;
   CtoneDelay = 0;
   CtoneMap = NULL;
   CtoneResetsTimeout = 0;
   CtoneTimer.tv_sec = 0;
   CWAutoJam = 0;
   CWSpeed = 20;
   CWToneLevel = 5000;
   Dtmf1stDigits = NULL;
   DTMF_Digits = 0;
   DtmfCoverTone = 0;
   DtmfMethod = 0;
   Element = NULL;
   ElementCount = 0;
   LastDTMFDigit.tv_sec = 0;
   LastDTMFDigitStart.tv_sec = 0;
   LastMixGain = 1024;  // 1:1
   Min1stDigitHoldTime = 0;
   MinDtmfInterdigitTime = 0;
   MuteDTMF = 1;
   NodeName = NULL;
   pDTMFCommands = NULL;
   Playfp = NULL;
   pPlayHead = NULL;
   pReceivers = NULL;
   pTransmitters = NULL;
   RampCount = 0;
   ReverseTwist = -1;
   RxTimeout = 0;
   RxTimeoutTone = 0;
   ToneGenerators = NULL;
   Transmitters = 0;
   Twist = -1;
   TxKeyTime = 0;
   TxTimeout = 0;
   TxTimeoutTone = 0;
   TxTimeoutToneGen = NULL;
   BufNum = 1;
   bAnnoucementIsID = 0;
   PrefixedTone = 0;
   DTMFPrefix = NULL;
   DTMFCommandTimeout = 1000;    // 1 second
   PrefixTimeout = 8000;         // 8 seconds
   bPrefixed = 0;
   bPrefixWait = 0;
   bCoverToneRunning = 0;
   ConnectAnnouncements = 1;
   bCtcssActive = FALSE;
   RxDeemphasis = 0;
   TxPreemphasis = 0;
   TxCtcssFilter = 0;
   DeemphasisTemp = 0;
   PreemphasisTemp = 0;
   bDtmfDetect = FALSE;
   ToneBurstFreq = 1750;
   ToneBurstMode = 0;
   ToneBurstMute = 0;
   bToneBurstWasDetected = 0;
   ToneBurstState = NULL;
   Energy = 0.0;
   ToneBurstToneID = 0;
   bToneBurstAccessed = 0;
   MaxPlayWithoutPause = -1;
   MinPlayBackPause = -1;
   ReducedGain = 25; // default to 6db drop
   AnnouncementPriority = AudioPriority = HIGH_AUDIO_PRI;
   LastActivity = 0;
   DefaultToneLevel = 8000;
   MaxInSample = 0;
   MinInSample = 0;
   MinMaxSamples = 0;
   MinMaxTotal = 0;
   InSquaredSum = 0.0;
   MinMaxReplyPort = -1;
   IRLPAnnouncements = FALSE;

   memset(TxCtcssFilterTemp,0,sizeof(TxCtcssFilterTemp));
   CmdPCM(NULL,"_init_");
}

Node::~Node()
{
   UnlinkAll(TRUE);
   CmdPCM(NULL,"close all");
   if(NodeName != NULL) {
      free(NodeName);
   }

   if(CurrentNode == this) {
      CurrentNode = NULL;
   }
}

// unlink everthing except local repeat from this port
int Node::UnlinkAll(int bNailedToo)
{
   int Ret = 0;
   LinkedNode *pLink;
   LinkedNode *pLastLink = NULL;
   LinkedNode *pNextLink;

// unlink everything from this node
   pLink = pReceivers;
   while(pLink != NULL) {
      pNextLink = pLink->pNextRx;
      if(!(pLink->Flags & LNK_FLG_NAILED_UP) || bNailedToo) {
         if(DeleteRx(pLink->pRx)) {
         // Shouldn't happen !
            LOG_ERROR(("%s#%d: Internal error!\n",__FUNCTION__,__LINE__));
            DumpMe();
            Ret = 1;
         }
         pLink = pReceivers;
      }
      else {
         pLink = pNextLink;
      }
   }

   pLastLink = NULL;
   pLink = pTransmitters;
   while(pLink != NULL) {
      pNextLink = pLink->pNextTx;
      if(!(pLink->Flags & LNK_FLG_NAILED_UP) || bNailedToo) {
         if(pLastLink == NULL) {
            pTransmitters = pLink->pNextTx;
         }
         else {
            pLastLink->pNextTx = pLink->pNextTx;
         }
         char *TempNodeName = pLink->pTx->NodeName;
         if(pLink->pTx->DeleteRx(pLink->pRx)) {
         // Shouldn't happen !
            LOG_ERROR(("%s#%d: Internal error!\n",__FUNCTION__,__LINE__));
            DumpMe();
            Ret = 1;
         }
         Transmitters--;
         LLOG(("%s: %s - deleted %s, now have %d Transmitters\n",__FUNCTION__,
               NodeName,TempNodeName,Transmitters));
      }
      else {
         pLastLink = pLink;
      }
      pLink = pNextLink;
   }

   return Ret;
}

void Node::FreeAnnoucement()
{
   PlayPCM *p;

   if(Playfp != NULL) {
      fclose(Playfp);
      Playfp = NULL;
   }

   if((p = pPlayHead) != NULL) {
      pPlayHead = p->Link;
      if(p->ReplyPort != 0) {
         ConfClient *pCC = &CmdLineCC;
         ClientInfo *pCInfo = CmdClient;
      // Restore the saved reply port.  
      // (we can't just use pCC->HisAdr.PORT since another client may have 
      // sent a command since the client that started the playback.)
         pCC->HisAdr.PORT = p->ReplyPort;
         pCInfo->Count = 0;
         BufPrintf(pCInfo,"%d\nPlayback of %s complete\n",TBD_SAY_COMPLETE,
                   p->FileName);
         sendto(pCInfo->Socket,pCInfo->Buf,pCInfo->Count+1,0,&pCC->HisAdr.s,
                sizeof(IPAdrUnion));
      }
      free(p->FileName);
      free(p);
   }
}

void Node::StartNextAnnoucement()
{
   PlayPCM *p = pPlayHead;
   FILE *TTSInFp = NULL;
   char Filename[PATH_MAX];
   int Bytes2Write;
   char *argv[4];
   char OutFilename[PATH_MAX];
   pid_t TTSPid = -1;
   int ReplyPort = 0;
   
   while((p = pPlayHead) != NULL) {
      if(p->Flags & PLAY_FLAG_TTS) {
         if(TTSInFp == NULL) {
         // Open TTS input file
            snprintf(Filename,PATH_MAX,"%s/%s",TempDir,TTS_IN_FILENAME);
            if((TTSInFp = fopen(Filename,"w")) == NULL) {
               LOG_ERROR(("%s#%d: Couldn't open \"%s\", %s",__FUNCTION__,
                          __LINE__,Filename,Err2String(errno)));
            }
         }
         else {
         // File already open, append a space so we don't run words together
            if(fputs(" ",TTSInFp) == EOF) {
               LOG_ERROR(("%s: Error reading \"%s\", %s",__FUNCTION__,
                          Filename,Err2String(errno)));
               fclose(TTSInFp);
               TTSInFp = NULL;
            }
         }

         if(TTSInFp != NULL) {
            Bytes2Write = strlen(p->FileName);
            if(fwrite(p->FileName,Bytes2Write,1,TTSInFp) != 1) {
               LOG_ERROR(("%s: Error reading \"%s\", %s",__FUNCTION__,
                          Filename,Err2String(errno)));
               fclose(TTSInFp);
               TTSInFp = NULL;
            }
         }
         if(ReplyPort == 0) {
         // Forward ReplyPort to the playback of the generated wave file
            ReplyPort = p->ReplyPort;
         }
         p->ReplyPort = 0;
         FreeAnnoucement();
      }
      else {
         if(TTSInFp != NULL) {
         // Don't start PCM file playback until TTS finishes
            break;
         }
         if((Playfp = fopen(p->FileName,"r")) == NULL) {
            LOG_ERROR(("%s#%d: Couldn't open \"%s\", %s",__FUNCTION__,__LINE__,
                       p->FileName,Err2String(errno)));
            FreeAnnoucement();
         }
         else {
            WaveHdr Hdr;
         // read 44 byte WAV header
            if(fread(&Hdr,sizeof(Hdr),1,Playfp) != 1) {
               LOG_ERROR(("%s: fread failed reading %s - %s\n",__FUNCTION__,
                          p->FileName,Err2String(errno)));
               FreeAnnoucement();
            }
            else {
            // Stupid test for now... fix me someday
               if(Hdr.wBitsPerSample == 8) {
                  b8BitFile = TRUE;
                  break;
               }
               else if(Hdr.wBitsPerSample == 16) {
                  b8BitFile = FALSE;
                  break;
               }
               else {
                  LOG_ERROR(("%s: %s has unsupported wBitsPerSample (%d)\n",
                             __FUNCTION__,p->FileName,Hdr.wBitsPerSample));
                  FreeAnnoucement();
               }
            }
         }
      }
   }

   if(TTSInFp != NULL) {
   // An TTS input file has been written, start the TTS program to 
   // render it to audio
      fprintf(TTSInFp,"\n");  // Some TTS programs (flite) need a linefeed
      fclose(TTSInFp);

      snprintf(OutFilename,PATH_MAX,"%s/%s",TempDir,TTS_OUT_FILENAME);
      unlink(OutFilename);
   // Create a new announcement entry at the front of the queue for playback

      PlayAnnouncementFile(OutFilename,PLAY_FLAG_TTS_OUT,ReplyPort);

   // NB: make sure all buffered streams are written to disk,
   // otherwise the data will be written twice, once by the main
   // process and once by the forked process !

      if(fflush(NULL)) {
         LOG_ERROR(("%s: fflush failed %s",__FUNCTION__,Err2String(errno)));
      }

      bTTS_Running = TRUE;
      if((TTSPid = fork()) < 0) {
         FreeAnnoucement();
         bTTS_Running = FALSE;
         LOG_ERROR(("%s: fork failed %s",__FUNCTION__,Err2String(errno)));
      }
      else if(TTSPid == 0) {
      // child
         argv[0] = TTS_ExePath;
         argv[1] = Filename;
         argv[2] = OutFilename;
         argv[3] = NULL;
         execvp(TTS_ExePath,argv);
      // NB: if execvp fails, BAIL!
         exit(1);
      }
      else {
      // Parent
         RegisterExitCB(TTSPid,TTSCB);
      }
   }

   if(pPlayHead != NULL && pPlayHead->Flags & PLAY_FLAG_ID) {
   // This annoucement is an voice ID 
      bAnnoucementIsID = TRUE;
   }
   else {
      bAnnoucementIsID = FALSE;
   }
}


void Node::PlayAnnouncementFile(char *FileName,int Flags,int ReplyPort)
{
   PlayPCM *p;

   if((p = (PlayPCM *) malloc(sizeof(PlayPCM))) == NULL) {
      LOG_ERROR(("PlayAnnouncementFile(): Error: out of memory\n"));
   }
   else {
      p->FileName = strdup(FileName);
      p->Flags = Flags;
      p->ReplyPort = ReplyPort;
      if(Flags & PLAY_FLAG_TTS_OUT) {
      // put on head of queue
         p->Link = pPlayHead;
         pPlayHead = p;
         if(p->Link == NULL) {
            pPlayTail = p;
         }
      }
      else {
      // put on tail of queue
         p->Link = NULL;
         if(pPlayHead == NULL) {
            pPlayHead = p;
         }
         else {
            pPlayTail->Link = p;
         }
         pPlayTail = p;
      }
      if(Playfp == NULL && !(Flags & PLAY_FLAG_TTS_OUT)) {
         StartNextAnnoucement();
      }
   }
}

// Say the specified phrase
void Node::SayPhrase(const char *phrase,int Flags,int ReplyPort)
{
   PlayPCM *p;
   CText FileName;
   char *cp;
   char *Temp;
   struct stat FileStats;
   CText Phrase;

   if(Flags & SAY_FLAG_CALLSIGN) {
      SpellCall(phrase,&Phrase);
   }
   else {
      Phrase = phrase;
   }

   if((cp = strstr(Phrase,".wav")) != NULL && strlen(cp) == 4) {
   // a filename was specified
      FileName = (char *) Phrase;
   }
   else if(TTS_ExePath == NULL) {
   // Lookup PCM file that contains the Phrase we want to say
      if(strlen(Phrase) == 1) {
      // Special case just a single character
         char c = *((char *)Phrase);
         FileName.print("pcm/%c.wav",c);
      }
      else {
      // replace all spaces with underscores
         Temp = strdup(Phrase);
         cp = Temp;
         while((cp = strchr(cp,' ')) != NULL) {
            *cp++ = '_';
         }
         FileName.print("pcm/%s.wav",Temp);
         free(Temp);
         if(stat(FileName,&FileStats) != 0) {
         // no such file
            FileName.Clear();
            LOG_ERROR(("%s: Unable to find file for phrase \"%s\"\n",
                       __FUNCTION__,(char *) Phrase));
         }
      }
   }

   if(TTS_ExePath != NULL || !FileName.IsEmpty()) {
      if((p = (PlayPCM *) malloc(sizeof(PlayPCM))) == NULL) {
         LOG_ERROR(("%s: Error: out of memory\n",__FUNCTION__));
      }
      else {
         p->Link = NULL;
         p->ReplyPort = ReplyPort;
         if(pPlayHead == NULL) {
            pPlayHead = p;
         }
         else {
            pPlayTail->Link = p;
         }
         pPlayTail = p;
         if(!FileName.IsEmpty()) {
            p->Flags = 0;
            p->FileName = strdup(FileName);
            if(Playfp == NULL) {
               StartNextAnnoucement();
            }
         }
         else {
            p->Flags = PLAY_FLAG_TTS;
            p->FileName = strdup(Phrase);
         }
      }
   }
}

void Node::SpellCall(const char *Call,CText *pPhrase)
{
   int i;
   CText FileName;
   CText Temp;
   int CallLen = strlen(Call);
   char  *cp;
   struct stat FileStats;

// Try for a exact match filename

   Temp.print("%s",*Call == '*' ? &Call[1] : Call);
   if((cp = strchr(Temp,'*')) != NULL) {
   // remove trailing '*'
      *cp = 0;
   }
   if((cp = strchr(Temp,' ')) != NULL) {
   // remove trailing spaces
      *cp = 0;
   }
   FileName.print("pcm/%s.wav",(char *) Temp);
   if(stat(FileName,&FileStats) == 0) {
   // we have a file for this callsign/conference name, play it.
      *pPhrase = (char *) FileName;
   }
   else {
   // Don't have an exact match, spell the call letter by letter
      for(i = 0; i < CallLen; i++) {
         if(isalnum(Call[i])) {
            if(Phonetic[0] != NULL) {
            // Spell it phonetically
               if(isalpha(Call[i])) {
                  pPhrase->print("%s ",Phonetic[tolower(Call[i])-'a']);
               }
               else {
                  pPhrase->print("%s ",Phonetic[Call[i]-'0'+'z'-'a'+1]);
               }
            }
            else if(TTS_ExePath != NULL) {
            // NB: convert callsign to lower case to prevent Cepstral from
            // pronouncing "S" as "South", etc.
               pPhrase->print("%c%s",tolower(Call[i]),
                              CallSignSpelling == NULL ? " " : CallSignSpelling);
            }
            else {
               pPhrase->print("%c ",Call[i]);
            }
         }
         else if(Call[i] == '*') {
            if(i != 0) {
               pPhrase->print("conference ");
            }
         }
         else if(Call[i] == '-') {
            i++;
            if(toupper(Call[i]) == 'L') {
            // Link
               pPhrase->print("link ");
            }
            else if(toupper(Call[i]) == 'R') {
            // Repeater
               pPhrase->print("repeater ");
            }
         }
         else if(Call[i] == ' ') {
            break;
         }
      }
   }
}


// Put PCM for audio annoucements into static AudioBuf
int Node::GetAnnounceAudio(int SamplesNeeded)
{
   int Read;
   int Bytes2Read;
   int BytesNeeded = SamplesNeeded * sizeof(int16);
   
   PlayAvail = 0;

   while(!bTTS_Running && pPlayHead != NULL && PlayAvail < BytesNeeded) {
      if(Playfp == NULL) {
         StartNextAnnoucement();
      }

      if(Playfp != NULL) {
         if(b8BitFile) {
            unsigned char TempBuf[AUDIO_BUF_SIZE];
            int i;

            Bytes2Read = SamplesNeeded - PlayAvail;
            Read = fread(TempBuf,1,Bytes2Read,Playfp);
            if(Read > 0) {
            // Convert to 16 bit samples in AudioBuf
               for(i = 0; i < Read; i++) {
                  AudioBuf[PlayAvail++] = (TempBuf[i] - 0x80) << 8;
               }
            }
         }
         else {
            Bytes2Read = BytesNeeded - PlayAvail * sizeof(int16);
            Read = fread(&AudioBuf[PlayAvail],1,Bytes2Read,Playfp);
            if(Read > 0) {
               PlayAvail += Read / sizeof(int16);
            }
         }

         if(Read > 0) {
            if(PlayAvail == SamplesNeeded) {
               break;
            }
         }
         else {
            if(!feof(Playfp)) {
               LOG_ERROR(("%s: read error - %s",__FUNCTION__,
                          Err2String(errno)));
            }
            FreeAnnoucement();
         }
      }
   }

   return PlayAvail;
}

// Mix audio from In into MixBuffer.  
// If Samples > 0 then input is 16 bit samples, otherwise it's 32 bit samples.
void Node::MixAudio(int *Out,void *Input,int Samples)
{
   int i;

   if(Samples > 0) {
   // 16 bit input, 32 bit output
      int16 *In = (int16 *) Input;
      for(i = 0; i < Samples; i++) {
         *Out++ += *In++;
      }
   }
   else {
   // 32 bit input, 32 bit output
      int *In = (int *) Input;
      Samples = -Samples;
      for(i = 0; i < Samples; i++) {
         *Out++ += *In++;
      }
   }
}

void Node::UpdateCtoneFlags()
{
   if(CtoneMap != NULL) {
      LinkedNode *pLink = pTransmitters;
      while(pLink != NULL) {
         if(StringIsInList(CtoneMap,pLink->pTx->NodeName)) {
            pLink->Flags |= LNK_FLG_CTONE;
         }
         else {
            pLink->Flags &= ~LNK_FLG_CTONE;
         }
         pLink = pLink->pNextTx;
      }
   }
}

void Node::AddTx(LinkedNode *pNewLink)
{
   LinkedNode *pLink = pTransmitters;

// Make sure this transmitter isn't already linked to this receiver
   while(pLink != NULL) {
      if(pLink->pTx == pNewLink->pTx) {
      // Already linked
         LOG_ERROR(("%s: Error: %s already linked to %s\n",__FUNCTION__,
                    pLink->pRx->NodeName,pLink->pTx->NodeName));
         break;
      }
      pLink = pLink->pNextTx;
   }

   if(pLink == NULL) {
      pNewLink->pNextTx = pTransmitters;
      pNewLink->AudioPriority = AudioPriority;
      pTransmitters = pNewLink;
      Transmitters++;
      UpdateCtoneFlags();
      LLOG(("%s: %s - Added %s, now have %d Transmitters\n",__FUNCTION__,
            NodeName,pNewLink->pTx->NodeName,Transmitters));
   }
}

int Node::DeleteTx(LinkedNode *pDeleteLink)
{
   LinkedNode *pLastLink = NULL;
   LinkedNode *pLink = pTransmitters;

   while(pLink != NULL) {
      if(pLink == pDeleteLink) {
         if(pLastLink == NULL) {
            pTransmitters = pLink->pNextTx;
         }
         else {
            pLastLink->pNextTx = pLink->pNextTx;
         }
         Transmitters--;
         LLOG(("%s: %s - deleted %s, now have %d Transmitters\n",__FUNCTION__,
               NodeName,pDeleteLink->pTx->NodeName,Transmitters));
         break;
      }
      pLastLink = pLink;
      pLink = pLink->pNextTx;
   }

   return (pLink == NULL);
}

void Node::AddRx(Node *pRx,int bNailUp,int Priority)
{
   LinkedNode *pLink = pReceivers;

// Make sure this receiver isn't already linked to this transmitter
   while(pLink != NULL) {
      if(pLink->pRx == pRx) {
      // Already linked
         if(bNailUp) {
            pLink->Flags |= LNK_FLG_NAILED_UP;
         }
         break;
      }
      pLink = pLink->pNextRx;
   }

   if(pLink == NULL) {
      if((pLink = new LinkedNode) == NULL) {
         LOG_ERROR(("%s#%d: new failed!\n",__FUNCTION__,__LINE__));
      }
      else {
         memset(pLink,0,sizeof(LinkedNode));
         if(bNailUp) {
            pLink->Flags |= LNK_FLG_NAILED_UP;
         }

         pLink->pRx = pRx;
         pLink->pTx = this;
         pRx->AddTx(pLink);
      // TODO: insert into link in sorted order so links display nicely
         pLink->pNextRx = pReceivers;
         pReceivers = pLink;
      }
   }

   if(pLink != NULL && Priority != -1) {
   // priority specified by link command
      pLink->AudioPriority = Priority;
   }
}

int Node::DeleteRx(Node *pRx)
{
   LinkedNode *pLink;
   LinkedNode *pLastLink = NULL;
   
   pLink = pReceivers;
   while(pLink != NULL) {
      if(pLink->pRx == pRx) {
         if(pLastLink == NULL) {
            pReceivers = pLink->pNextRx;
         }
         else {
            pLastLink->pNextRx = pLink->pNextRx;
         }
         pLink->pRx->DeleteTx(pLink);
         delete pLink;
         break;
      }
      pLastLink = pLink;
      pLink = pLink->pNextRx;
   }

   return (pLink == NULL);
}

// Scale the mixed audio so that the maximum level does not exceed 
// MAX_MIXED_LEVEL.  When the mix gain changes try to change the gain
// at a zero crossing to avoid thumps.
void Node::ScaleMixedAudio(int16 *OutBuf,int Samples)
{
   int MixGain;
   int i;
   int bFindNegCrossing = MixBuffer[0] > 0 ? TRUE : FALSE;
   int Sample;
   int MaxLevel = MAX_MIXED_LEVEL - CtcssTxLevel;

   MaxMixedLevel = 0;
   for(i = 0; i < Samples; i++) {
      if((Sample = MixBuffer[i]) < 0) { 
         Sample = -Sample;
      }
      if(Sample > MaxMixedLevel) {
         MaxMixedLevel = Sample;
      }
   }

   if(MaxMixedLevel > MaxLevel) {
   // calculate gain to reduce maximum mixed level to 
      MixGain = (MaxLevel * 1024) / MaxMixedLevel;
   }
   else {
      MixGain = 1024;   // 1:1
   }

   if(MixGain == 1024 && LastMixGain == 1024) {
   // Both this gain and the last gain were 1:1, so no need to multiply
      for(i = 0; i < Samples; i++) {
         OutBuf[i] = (int16) MixBuffer[i];
      }
   }
   else {
      if((MixGain - LastMixGain) > 2) {
      // increase gain slowly to make it less noticeable
         MixGain = LastMixGain + (MixGain - LastMixGain)/2;
      }

      for(i = 0; i < Samples; i++) {
         if(MixBuffer[i] > MaxLevel || MixBuffer[i] < -MaxLevel) {
         // need to apply new MixGain now
            break;
         }

         if(bFindNegCrossing) {
            if(MixBuffer[i] < 0) {
            // apply new gain now
               break;
            }
         }
         else {
            if(MixBuffer[i] > 0) {
            // apply new gain now
               break;
            }
         }

      // AudioBuf[i] = MixBuffer[i] * LastMixGain / 1024;
         OutBuf[i] = (int16) ((MixBuffer[i] * LastMixGain) >> 10);
      }

   // change to the new mix gain for the remainder of the buffer
      LastMixGain = MixGain;

      for(; i < Samples; i++) {
         OutBuf[i] = (int16) ((MixBuffer[i] * LastMixGain) >> 10);
      }
   }
}

int Node::LiveSamplesAvailable()
{
   int NewSamples;
   LinkedNode *pLink;
   int Samples = 0;

// Find out how much "live" audio is available from our ports
   pLink = pReceivers;
   while(pLink != NULL) {
      NewSamples = pLink->pRx->SamplesAvailable(pLink,FALSE);
      if(NewSamples > 0 && (Samples == 0 || Samples > NewSamples)) {
         Samples = NewSamples;
      }
      pLink = pLink->pNextRx;
   }

   if(Samples == 0) {
   // Do it again, but this time force VoIP nodes to give up anything
   // that may have be waiting for a out of sequence packet to arrive
      pLink = pReceivers;
      while(pLink != NULL) {
         NewSamples = pLink->pRx->SamplesAvailable(pLink,TRUE);
         if(NewSamples > 0 && (Samples == 0 || Samples > NewSamples)) {
            Samples = Samples;
         }
         pLink = pLink->pNextRx;
      }
   }
   return Samples;
}

// Returns number of samples written to TxBuf
int Node::GetTxAudio(int16 *TxBuf,int SamplesNeeded)
{
   CToneGen *pToneGen;
   CToneGen *pLastGen;
   CToneGen *pNextGen;
   LinkedNode *pLink;
   LinkedNode *pNext;
   int Sources = 0;
   int IDSources = 0;
   int Samples = 0;
   int PriorCnt[3] = { 0, 0 , 0};
   int Ret = 0;
   int bForced = FALSE;
   int bDisconnectNode = FALSE;
   int bAudioMixed;

   if(SamplesNeeded > AUDIO_BUF_SIZE) {
      SamplesNeeded = AUDIO_BUF_SIZE;
   }

   for(bForced = FALSE; !bForced; bForced = TRUE) {
   // Find out how much "live" audio is available from our ports
      pLink = pReceivers;
      while(pLink != NULL) {
         int NewSamples;
         NewSamples = pLink->pRx->SamplesAvailable(pLink,bForced);

         if(NewSamples > 0) {
            if(pLink->AudioPriority > HIGH_AUDIO_PRI) {
               LOG_ERROR(("%s: Error: AudioPriority for %s->%s is bogus (%d)\n",
                          __FUNCTION__,pLink->pRx->NodeName,NodeName,
                          pLink->AudioPriority));
               pLink->AudioPriority = MED_AUDIO_PRI;
            }
            PriorCnt[pLink->AudioPriority]++;
            if(Samples == 0 || Samples > NewSamples) {
               Samples = NewSamples;
            }
         }
         pLink = pLink->pNextRx;
      }

      if(Samples > 0) {
         break;
      }
   }

   if(Samples > 0 && Samples < SamplesNeeded) {
   // Limit request to what we can get from our ports at the moment
      SamplesNeeded = Samples;
   }
   memset(MixBuffer,0,SamplesNeeded*sizeof(int));
   
   for(int Priority = LOW_AUDIO_PRI; Priority <= HIGH_AUDIO_PRI; Priority++) {
      bAudioMixed = FALSE;
      pNext = pReceivers;
      while((pLink = pNext) != NULL) {
         void *pAudio;
         int NewSamples;

         pNext = pLink->pNextRx;
         if(pLink->AudioPriority != Priority) {
            continue;
         }
         for(Samples = 0; Samples < SamplesNeeded;) {
            NewSamples = pLink->pRx->GetRxAudio(pLink,
                                                &pAudio,
                                                SamplesNeeded-Samples,
                                                Samples == 0);
            if(NewSamples == 0) {
               break;
            }
         // Receiver active
#if 0
            if(DebugFpTxMix != NULL) {
               fwrite(pAudio,NewSamples*sizeof(int),1,DebugFpTxMix);
            }
#endif

            ALOG(("%s<-%s %d NewSamples\n",NodeName,pLink->pRx->NodeName,
                  NewSamples));

            if(MuteDTMF && pLink->pRx->LastDTMFDigit.tv_sec != 0) {
            // DTMF mute active
               if(DtmfCoverTone != 0 && this == pLink->pRx && !bCoverToneRunning) {
               // This is our port's receiver, start cover tone 
                  PlayStoredSound(DtmfCoverTone);
                  bCoverToneRunning = TRUE;
               }
            }
            else if(NewSamples != 0) {
            // DTMF mute not active
               Sources++;
               bAudioMixed = TRUE;
               MixAudio(&MixBuffer[Samples],pAudio,NewSamples);
               if(NewSamples < 0) {
                  NewSamples = -NewSamples;
               }
               Samples += NewSamples;
            }
         }

         if(Ret < Samples) {
            Ret = Samples;
         }

         if(Samples > 0 && Samples < SamplesNeeded) {
            ALOG(("%s<-%s only returned %d of %d needed\n",NodeName,
                  pLink->pRx->NodeName,Samples,SamplesNeeded));
         }

         if(bCoverToneRunning && this == pLink->pRx && LastDTMFDigit.tv_sec == 0) {
         // Cover tone is running and dtmf is no longer active
            CancelToneGen(DtmfCoverTone);
            bCoverToneRunning = FALSE;
         }
      }

      if(AnnouncementPriority == Priority && 
         (Samples = GetAnnounceAudio(SamplesNeeded)) > 0) 
      {
         if(Ret < Samples) {
            Ret = Samples;
         }
         if(bAnnoucementIsID) {
            IDSources++;
         }
         Sources++;
         bAudioMixed = TRUE;
         MixAudio(MixBuffer,AudioBuf,Samples);
      }

      if(bAudioMixed) {
         if(Priority == LOW_AUDIO_PRI && 
            (PriorCnt[MED_AUDIO_PRI] > 0 || PriorCnt[HIGH_AUDIO_PRI] > 0)) 
         {  // mute the low priority audio
            memset(MixBuffer,0,SamplesNeeded*sizeof(int));
         }

         if(Priority == MED_AUDIO_PRI && PriorCnt[HIGH_AUDIO_PRI] > 0) {
         // mix down the medium priority audio
            for(int i = 0; i < SamplesNeeded; i++) {
               MixBuffer[i] = MixBuffer[i] * ReducedGain / 100;
            }
         }
      }
   }

   pToneGen = ToneGenerators;
   pLastGen = NULL;
   while(pToneGen != NULL) {
      pNextGen = pToneGen->Link;
      if((Samples = pToneGen->ToneGen(AudioBuf,SamplesNeeded)) == -1) {
      // This generator has finished
         if(pLastGen == NULL) {
            ToneGenerators = pNextGen;
         }
         else {
            pLastGen->Link = pNextGen;
         }

         if(pToneGen->CompletionCmd != NULL) {
         // Execute the completion command
            ExecuteCommand(pToneGen->CompletionCmd);
         }

         if(pToneGen->bWelcome) {
            VoipNode *pVoip = dynamic_cast<VoipNode*> (this);
            if(pVoip != NULL) {
               if(pVoip->bDisconnectAfterWelcome) {
                  bDisconnectNode = TRUE;
               }
            }
         }
         delete pToneGen;
      }
      else {
         if(Ret < Samples) {
            Ret = Samples;
         }
         pLastGen = pToneGen;
         Sources++;
         if(pToneGen->bIDSource) {
            IDSources++;
         }
         MixAudio(MixBuffer,AudioBuf,Samples);
      }
      pToneGen = pNextGen;
   }


// Now apply the mix gain to prevent over modulation
   if(Sources > 0) {
      if(DebugFpTxMix != NULL) {
         fwrite(MixBuffer,Ret*sizeof(int),1,DebugFpTxMix);
      }
      ScaleMixedAudio(TxBuf,Ret);
      if(DebugFpTxIn != NULL) {
         fwrite(TxBuf,Ret*sizeof(int16),1,DebugFpTxIn);
      }

      if(Sources > IDSources) {
      // A source other than an ID is active, update the LastActivity time
         LastActivity = TimeNow.tv_sec;
      }

      if(TxCtcssFilter) {
      // Filter out any Ctcss
         ApplyCtcssFilter(TxBuf,TxBuf,Ret,TxCtcssFilterTemp);
         if(DebugFpTxCtcssFilter != NULL) {
            fwrite(TxBuf,Ret*sizeof(int16),1,DebugFpTxCtcssFilter);
         }
      }

      if(TxPreemphasis) {
      // Apply Preemphasis to TX audio
         ApplyTxPreemphasis(TxBuf,TxBuf,Ret,&PreemphasisTemp);
         if(DebugFpTxPreemphasis != NULL) {
            fwrite(TxBuf,Ret*sizeof(int16),1,DebugFpTxPreemphasis);
         }
      }

      if(CtcssTxLevel > 0) {
      // Add CTCSS
         GenCTCSS(TxBuf,Ret);
      }
   }

   if(bTxTimeOut) {
   // Only return audio from the Timeout tone if timed out
      if(TxTimeoutToneGen != NULL) {
         if((Ret = TxTimeoutToneGen->ToneGen(TxBuf,SamplesNeeded)) == -1) {
         // This generator has finished
            D2PRINTF(("%s: Deleting timeout tonegen #%d\n",__FUNCTION__,
                      TxTimeoutToneGen->ID));
            delete TxTimeoutToneGen;
            TxTimeoutToneGen = NULL;
         }
      }
      else {
         Ret = 0;
      }
   }

   if(bDisconnectNode) {
      CText Cmd;
      Cmd.print("disconnect %s",NodeName);
      ExecuteCommand(Cmd.Buf);
   }

   return Ret;
}

int Node::WriteAudioOut(ClientInfo *p)
{
   return 0;
}

void Node::DecodeDTMF(char *Command)
{
   DTMF_Command *p = pDTMFCommands;
   char  WildCardBuf[MAX_DTMF_DIGITS+1];
   int i;
   int j;
   int WildCardCount = 0;
   char *cp;
   char *CmdPtr;
   int Digits = strlen(Command);

   CurrentNode = this;

   LOG_NORM(("%s: Decoding \"%s\"\n",__FUNCTION__,Command));

   while(p != NULL) {
      for(i = 0, j = 0; i < Digits; i++) {
         if(p->DTMF_String[j] == '?' || p->DTMF_String[j] == '+') {
         // Wildcard match, save the matched character
            WildCardBuf[WildCardCount++] = Command[i];
            if(p->DTMF_String[j] == '?') {
               j++;
            }
         }
         else if(p->DTMF_String[j] == Command[i]) {
         // match
            j++;
         }
         else {
         // no match
            break;
         }
      }

      if(i == Digits && (p->DTMF_String[j] == 0 || p->DTMF_String[j] == '+')) {
      // Found a matching command
         break;
      }
   // No match, keep looking
      WildCardCount = 0;
      p = p->Link;
   }

   if(p != NULL) {
   // Found the command, execute it
      char *Temp;
      WildCardBuf[WildCardCount] = 0;

   // Make a copy of the command in case the command causes a new
   // command file to be loaded (which will free the passed in string).
      Temp = strdup(p->Command);
      CmdPtr = Temp;
      while(CmdPtr != NULL) {
         if((cp = strchr(CmdPtr,';')) != NULL) {
            *cp = 0;
         }
      // Expand and execute the command
         ExpandCommand(CmdPtr,Command,WildCardBuf);

         if(cp != NULL) {
         // restore the command seperator character we removed earlier
            *cp++ = ';';
         // Skip any white space to make the logs look pretty
            while(isspace(*cp)) {
               cp++;
            }
         }

         CmdPtr = cp;
      }
      free(Temp);
   }
   else {
      LOG_ERROR(("Failed to decode touchtone command \"%s\"\n",Command));
   }
}

// Return:
// 1  - prefix match
// 0  - prefix incomplete
// -1 - prefix match fail
int Node::CheckPrefix()
{
   int Ret = 0;
   int i;

   for(i = 0; i < DTMF_Digits; i++) {
      if(DTMF_Buf[i] != DTMFPrefix[i]) {
         DLOG(DLOG_DTMF,("%s: node: %s, prefix digit %d mismatch %c != %c\n",
              __FUNCTION__,NodeName,i+1,DTMF_Buf[i],DTMFPrefix[i]));
         Ret = -1;
         break;
      }
   }

   if(DTMFPrefix[i] == 0) {
      DLOG(DLOG_DTMF,("%s: node: %s, prefix digit \"%c\" accepted, prefix complete\n",
                      __FUNCTION__,NodeName,DTMF_Buf[DTMF_Digits-1]));
      Ret = 1;
   }

   if(Ret == 0) {
      DLOG(DLOG_DTMF,("%s: node: %s, prefix digit \"%c\" accepted\n",__FUNCTION__,
                      NodeName,DTMF_Buf[DTMF_Digits-1]));
   }
   return Ret;
}

void Node::SaveDTMF(char Digit)
{
   int bIgnoreDigit = FALSE;
   int Elapsed = TimeLapse(&LastDTMFDigitStart);
   
   LastDTMFDigit = TimeNow;
   if(Digit == ' ') {
   // End of decode
      bDtmfDetect = FALSE;
      if(DTMF_Digits <= 1 && Min1stDigitHoldTime != 0) {
      // End of decode of first digit
         if(Elapsed < Min1stDigitHoldTime) {
         // First digit wasn't held long enough, ignore it
            LOG_ERROR(("%s: Ignoring 1st digit \'%c\', only held %d ms.\n",
                       __FUNCTION__,DTMF_Buf[0],Elapsed));
            DTMF_Digits = 0;
            LastDTMFDigit.tv_sec = 0;
         }
      }
   }
   else {
   // DTMF Digit decoded 
      bDtmfDetect = TRUE;
      LastDTMFDigitStart = TimeNow;
      DLOG(DLOG_DTMF,("Node: %s, DTMF decode \"%c\"\n",NodeName,Digit));
      DPRINTF(("Node %s, DTMF decode: %c\n",NodeName,Digit));
      DFLUSH();
      if((DtmfCTCSSMode == 1 && !bRxActive) || 
         (DtmfCTCSSMode == 2 && !bCtcssActive))
      {
         bIgnoreDigit = TRUE;
         if(Digit != ' ') {
            DPRINTF(("Port %s: Ignoring DTMF '%c', DtmfCTCSSMode: %d,"
                     " bRxActive: %d, bCtcssActive: %d\n",NodeName,Digit,
                     DtmfCTCSSMode,bRxActive,bCtcssActive));
         }
      }

      if(DTMF_Digits > 0 && Digit == DTMF_Buf[DTMF_Digits-1]) {
      // Repeated digit

         if(MinDtmfInterdigitTime != 0 && Elapsed < MinDtmfInterdigitTime) {
            bIgnoreDigit = TRUE;
         }

         LOG_ERROR(("%s: Repeated decode of \'%c\' %s, last was %d ms ago\n",
                    __FUNCTION__,Digit,
                    bIgnoreDigit ? "ignored" : "accepted",
                    Elapsed));
      }

      if(!bIgnoreDigit) {
         if(DTMF_Digits < MAX_DTMF_DIGITS) {
            DTMF_Buf[DTMF_Digits++] = Digit;
            DTMF_Buf[DTMF_Digits] = 0;
         }

         if(DTMFPrefix != NULL && !bPrefixed) {
            int Prefixed;

            if((Prefixed = CheckPrefix()) == 1) {
            // End of prefix reached
               bPrefixed = TRUE;
               bPrefixWait = TRUE;
               DTMF_Digits = 0;
               if(PrefixedTone != 0) {
               // Start dialtone
                  PlayStoredSound(PrefixedTone);
               }
            }
            else if(Prefixed == -1) {
            // Prefix failed, search for another possible starting position
               while(--DTMF_Digits > 0) {
               // Remove first digit from in prefix buffer
                  memmove(DTMF_Buf,&DTMF_Buf[1],DTMF_Digits);
                  if(CheckPrefix() == 0) {
                  // another possible prefix, press on
                     break;
                  }
               }
            }
         }
         else if(DTMF_Digits == 1) {
         // First digit
            bPrefixWait = FALSE;
            if(Dtmf1stDigits != NULL) {
               if(strchr(Dtmf1stDigits,Digit) == NULL) {
                  LOG_ERROR(("%s: Ignoring invalid first digit \'%c\'\n",
                             __FUNCTION__,Digit));
                  bIgnoreDigit = TRUE;
               }
            }

            if(DTMFPrefix != NULL && PrefixedTone != 0) {
            // Cancel dialtone on first digit received
               CancelToneGen(PrefixedTone);
            }
         }
      }
   }

   EventHook("dtmfdecode %c %s",Digit == ' ' ? 'N' : Digit,NodeName);
}

void Node::GenCTCSS(int16 *CTCSSBuf,int Samples)
{
   int i;

   for(i = 0; i < Samples; i++) {
      CTCSSBuf[i] += (int16) (CtcssTxLevel * sin(CTCSSAngle));
      CTCSSAngle += CTCSSAngleDelta;
   }
}

/* tone generation

   .tonegen [-p|-P] [-c "<completion command>"] tonespec
   
   -p = don't send to this port, sent to ports this port is connected to (peers)
   -P = send to this port and ports this port is connected to
   -c = run completion command when tonespec completes
*/
void Node::CmdToneGen(ClientBufPrint *p,char *Arg)
{
   char *pErrPos;
   char *ToneSpec = Arg;
   char *CompletionCmd = NULL;
   int ToneID;
   int Option = 0;
   int bSend2Peers = FALSE;
   int bSend2Self = TRUE;

   do {
      while((Option = GetCmdOptions(&ToneSpec,"c:pP")) != -1) {
         switch(Option) {
            case 'c':
               if(CmdArg == NULL) {
                  Rcode = TBD_ERR_INVALID_ARG;
               }
               else {
                  CompletionCmd = strdup(CmdArg);
               }
               break;

            case 'p':   
            // Send tone to the nodes we're connected excluding ourself
               bSend2Peers = TRUE;
               bSend2Self = FALSE;
               break;

            case 'P':
            // Send tone to the nodes we're connected include ourself
               bSend2Peers = TRUE;
               break;
         }
      }

      if(Option == '?') {
      // command parsing error
         p->print("Error: invalid switch\r");
         Rcode = TBD_ERR_INVALID_ARG;
         break;
      }

      if(bSend2Peers) {
         LinkedNode *pLink = pReceivers;
         while(Rcode == 0 && pLink != NULL) {
            if(this != pLink->pRx) {
               pLink->pRx->CmdToneGen(p,ToneSpec);
            }
            pLink = pLink->pNextRx;
         }
      }

   // Erase any error messages the peer generated, we'll regenerate them
   // shortly

      if(!bSend2Self) {
         break;
      }
      p->Clear();

      if(*ToneSpec == '#') {
      // play a stored tonespec
         if(sscanf(&ToneSpec[1],"%d",&ToneID) != 1 || ToneID <= 0) {
            p->print("Error: invalid tone ID \"%s\"\r",ToneSpec);
            Rcode = TBD_ERR_INVALID_ARG;
            break;
         }

         if((ToneSpec = FindToneSpec(ToneID)) == NULL) {
            p->print("Error: tone #%d is not defined.\r",ToneID);
            Rcode = TBD_ERR_INVALID_ARG;
            break;
         }
      }
      else if(*ToneSpec == '!') {
      // Cancel a tonespec that's currently running
         if(sscanf(&ToneSpec[1],"%d",&ToneID) != 1 || ToneID <= 0) {
            p->print("Error: invalid tone ID \"%s\"\r",ToneSpec);
            Rcode = TBD_ERR_INVALID_ARG;
            break;
         }

         if(!CancelToneGen(ToneID)) {
            p->print("tone #%d was not running\r",ToneID);
         }
         break;
      }
      
      CToneGen *pGenerator = NULL;
      CPort *pPort = dynamic_cast<CPort *> (this);

      if(pPort != NULL) {
         pGenerator = new CToneGen(DefaultToneLevel,
                                   pPort->Ax25TxDelay,pPort->Ax25ToneLevel);
      }
      else {
         pGenerator = new CToneGen(DefaultToneLevel);
      }

      if(pGenerator == NULL) {
         LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
         p->print("Error: new failed\r");
         break;
      }

      if(MaxPlayWithoutPause == -1) {
         pGenerator->MaxPlayWithoutPause = ::MaxPlayWithoutPause;
      }
      else {
         pGenerator->MaxPlayWithoutPause = MaxPlayWithoutPause;
      }

      if(MinPlayBackPause == -1) {
         pGenerator->MinPlayBackPause = ::MinPlayBackPause;
      }
      else {
         pGenerator->MinPlayBackPause = MinPlayBackPause;
      }

      pGenerator->CompletionCmd = CompletionCmd;

      if(pGenerator->ToneGenSetup(ToneSpec,&pErrPos)) {
      // error
         p->print("Error: couldn't parse \"%s\"\r",pErrPos);
         Rcode = TBD_ERR_INVALID_ARG;
         delete pGenerator;
         break;
      }
      else {
         AddToneGen(pGenerator);
      }
   } while(FALSE);
}

void CmdToneGen(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ClientBufPrint Prn(p);
   if(CurrentNode != NULL) {
      CurrentNode->CmdToneGen(&Prn,Arg);
   }
   else {
      Prn.print("Error: no port is currently selected\r");
   }
}

extern int CtccssSample;
// pcm close <test point>
// pcm record <test point>
// pcm record
void Node::CmdPCM(ClientBufPrint *p,const char *Arg1)
{
   int bFirst = TRUE;
   int bClose = FALSE;
   int bOpen = FALSE;
   int bAll = FALSE;
   int bUsage = FALSE;
   int bInit = FALSE;
   int bPlay = FALSE;
   const char  *WhiteSpace = "\t\n ";
   char *Arg = strdup(Arg1);
   char *Source = strtok(Arg,WhiteSpace);
   typedef struct {
      const char *Name;
      FILE **pHandle;
   } DebugFileHandles;

   DebugFileHandles Sources[] = {
      { "RxDeemphasis",&DebugFpRxDeemphasis },
      { "RawAudioIn",&DebugFpRawAudioIn},
      { "AudioIn",&DebugFpAudioIn},
      { "AudioOut",&DebugFpAudioOut},
      { "TxIn", &DebugFpTxIn},
      { "TxMix", &DebugFpTxMix},
      { "TxPreemphasis", &DebugFpTxPreemphasis},
      { "TxCtcssFilter", &DebugFpTxCtcssFilter},
      { "RxCtcssFilter", &DebugFpRxCtcssFilter},
      { "CtcssPeakDet", &DebugCtcssPeakDet},
      { "CtcssDecode", &DebugCtcssDecode},
      { "DtmfIn", &DebugFpDtmfIn},
      { NULL}
   };

   DebugFileHandles Sinks[] = {
      { "RawAudioIn",&DebugFpRawAudioPB},
      { "RxCtcssFilter", &DebugFpRxCtcssPB},
      { NULL}
   };

   DebugFileHandles *pHandles;


   if(Source != NULL && *Source != 0) {
      if(STRCMP(Source,"record") == 0) {
         bOpen = TRUE;
         bPlay = FALSE;
      }
      else if(STRCMP(Source,"play") == 0) {
         bOpen = TRUE;
         bPlay = TRUE;
         CtccssSample = 0;
      }
      else if(STRCMP(Source,"close") == 0) {
         bClose = TRUE;
      }
      else if(STRCMP(Source,"_init_") == 0) {
         bInit = TRUE;
      }
      else {
         bUsage = TRUE;
      }
   }
   else {
      bUsage = TRUE;
   }

   if(bOpen || bClose) {
      while((Source = strtok(NULL,WhiteSpace)) != NULL) {
         if(STRCMP(Source,"all") == 0) {
            bAll = TRUE;
         }

         pHandles = bPlay ? Sinks : Sources;

         for(int i = 0; pHandles[i].Name != NULL; i++) {
            if(bAll || STRCMP(Source,pHandles[i].Name) == 0) {
               bFirst = FALSE;
               if(bOpen) {
                  if(*pHandles[i].pHandle == NULL) {
                     CText FileName;
   
                     FileName.print("%s_%s.raw",NodeName,pHandles[i].Name);

                     *pHandles[i].pHandle = fopen(FileName.Buf,bPlay ? "r":"w");
   
                     if(*pHandles[i].pHandle == NULL && p != NULL) {
                        p->print("fopen of %s failed - %s\r",FileName.Buf,
                                 strerror(errno));
                     }
                  }
               }
               else if(bClose) {
                  if(*pHandles[i].pHandle != NULL) {
                     fclose(*pHandles[i].pHandle);
                     *pHandles[i].pHandle = NULL;
                  }
               }
            }
         }
         if(bFirst && p != NULL) {
            p->print("Error: unknown test point %s\r",Source);
            break;
         }

         if(bAll) {
            break;
         }
      }
   }

   if(bInit) {
      for(int i = 0; Sources[i].Name != NULL; i++) {
         *Sources[i].pHandle = NULL;
      }

      for(int i = 0; Sinks[i].Name != NULL; i++) {
         *Sinks[i].pHandle = NULL;
      }
   }
   else if(!bUsage && p != NULL) {
   // list currently open streams
      bFirst = TRUE;
      for(int i = 0; Sources[i].Name != NULL; i++) {
         if(*Sources[i].pHandle != NULL) {
            if(bFirst) {
               bFirst = FALSE;
               p->print(bPlay ? "Playing: " : "Recording: ");
            }
            else {
               p->print(", ");

            }
            p->print("%s",Sources[i].Name);
         }
      }

      if(bFirst) {
         p->print("Nothing is being %s\r",bPlay ? "played" : "recorded");
      }
      p->print("\n");
   }

   if(bUsage && p != NULL) {
      p->print("Usage:\r"
               "  pcm play <test point>\r"
               "  pcm record <test point>\r"
               "  pcm close <test point>\r"
               "  where <test point> := \r   \"all\"\r");
      for(int i = 0; Sources[i].Name != NULL; i++) {
         p->print("   \"%s\"\r",Sources[i].Name);
      }
      for(int i = 0; Sinks[i].Name != NULL; i++) {
         p->print("   \"%s\"\r",Sources[i].Name);
      }
   }

   free(Arg);
}

void CmdPCM(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   if(CurrentNode != NULL) {
      ClientBufPrint Prn(p);
      CurrentNode->CmdPCM(&Prn,Arg);
   }
}


int Node::Shutup()
{
   int Ret = FALSE;

   while(pPlayHead != NULL) {
      Ret = TRUE;
      FreeAnnoucement();
   }

   return Ret;
}

void CmdShutup(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   if(CurrentNode->Shutup()) {
      BufPrintf(p,"ok\r");
   }
   else {
      BufPrintf(p,"no announcements are playing\r");
   }
}

void CmdSayResult(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   char ErrMsg[120];
   int  bHandled = FALSE;
   int i;

// Search for a custom say result handler for this command

   for(i = 0; SayResultHandlers[i].Cmd != NULL; i++) {
      if(strcmp(SayResultHandlers[i].Cmd,LastCmd) == 0) {
      // Found it
         bHandled = SayResultHandlers[i].Handler(p,pCC1,Arg);
         break;
      }
   }

   if(!bHandled) {
      if(TTS_ExePath) {
      // We have TTS

         if(LastCmdText != NULL) {
         // Something to say, just say it
            CurrentNode->SayPhrase(LastCmdText);
         }
         else if(LastRcode == TBD_OK) {
            CurrentNode->SayPhrase("O K");
         }
         else if(LastRcode > TBD_ERR_BASE && LastRcode < TBD_LAST_ERR_CODE) {
            CurrentNode->SayPhrase(StdTbdCommandText[LastRcode - TBD_ERR_BASE - 1]);
         }
         else {
            snprintf(ErrMsg,sizeof(ErrMsg),"command failed %s",
                     Err2String(LastRcode));
            CurrentNode->SayPhrase(ErrMsg);
         }
      }
      else {
      // No TTS
         if(LastRcode == TBD_OK) {
            CurrentNode->SayPhrase("O");
            CurrentNode->SayPhrase("K");
         }
         else {

         }
      }
   }
}

int Comment(char *cp)
{
   while(*cp && isspace(*cp))
      cp++;
   if(*cp == ';' || *cp == 0 )
      return 1;
   return 0;
}

#define MAX_LINE_LEN 1024
#define MAX_INCLUDES 8
int Node::LoadCommands(char *Path)
{
   int Ret = 0;
   FILE *fp = NULL;
   char  Line[MAX_LINE_LEN];
   char  DTMF_String[MAX_LINE_LEN+1];
   char  CommandString[MAX_LINE_LEN+1];
   char  *cp;
   char  *cp1;
   DTMF_Command *pDTMFCommand;
   DTMF_Command *pTail = (DTMF_Command *) &pDTMFCommands;
   DTMF_Command *pCmd;
   static int Nested = 0;
   
   if(++Nested >= MAX_INCLUDES) {
      LOG_ERROR(("Error: Command files nested is too deep.\n"));
      Ret = CONFIG_NESTING_ERROR;
   }
   else if((fp = fopen(Path,"r")) == NULL) {
      LOG_ERROR(("%s: Unable to open \"%s\" - %s\n",__FUNCTION__,Path,
                 Err2String(errno)));
   }

   while(fp != NULL) {
      if(fgets(Line,sizeof(Line),fp) == NULL) {
         if(ferror(fp)) {
            LOG_ERROR(("%s: read error while reading \"%s\" - %s\n",
                       __FUNCTION__,Path,Err2String(errno)));
            Ret = errno;
         }
         break;
      }
      if((cp = strrchr(Line,'\n')) != NULL) {
         // remove terminating <lf>
         *cp = 0;
      }

      if((cp = strrchr(Line,'\r')) != NULL) {
      // remove terminating <cr> just in case some one uses an editor that
      // changes the end of line convention in *nix
         *cp = 0;
      }

      if(Comment(Line)) {
      // ignore this line if it's a comment
         continue;
      }

      if(strncmp(Line,"include ",8) == 0) {
      // include another file here
         cp = &Line[8];
         while(*cp && (*cp == ' ' || *cp == '\t' || *cp == '"')) {
            cp++;
         }

         if((cp1 = strchr(cp,'\n')) != NULL) {
            *cp1 = 0;
         }

         if((cp1 = strchr(cp,'"')) != NULL) {
            *cp1 = 0;
         }

      // Process the included file
         Ret = LoadCommands(cp);
         if(Ret == 0) {
            continue;
         }
         else {
            break;
         }
      }

      if(sscanf(Line," %[^= ] = %[^\n]",DTMF_String,CommandString) != 2) {
      // line didn't convert
         LOG_WARN(("%s: Unable to parse line:\n  %s\n",__FUNCTION__,Line));
         continue;
      }

   // Remove spaces from DTMF string and test for valid characters
      cp1 = cp = DTMF_String;
      while(*cp) {
         *cp = toupper(*cp);
         if((*cp >= 'A' && *cp <= 'D') || (*cp >= '0' && *cp <= '9') ||
            *cp == '#' || *cp == '*' || *cp == '?' || *cp == '+')
         {  // Valid DTMF or wildcard character, copy it

            *cp1++ = *cp++;
         }
         else if(*cp == ' ' || *cp == '\t') {
         // space or tab, ignore it
            cp++;
         }
         else {
         // error
            LOG_WARN(("Error: \"%c\" is not an DTMF character, ignoring line:\n"
                      "%s\n",*cp,Line));
         }
      }

      if(*cp) {
         continue;
      }

      *cp1 = 0;

      cp = CommandString;
   // Skip white space at beginning of the command string
      while(isspace(*cp)) {
         cp++;
      }

   // Search existing commands for an exact match
      pCmd = pDTMFCommands;
      while(pCmd != NULL) {
         if(strcmp(pCmd->DTMF_String,DTMF_String) == 0) {
         // Match, replace this definition with the new one
            free(pCmd->Command);
            pCmd->Command = strdup(cp);
            break;
         }
         else {
            pCmd = pCmd->Link;
         }
      }

      if(pCmd == NULL) {
      // New command, add it to the command chain
   
         if((pDTMFCommand = (DTMF_Command *) malloc(sizeof(DTMF_Command))) == NULL) {
            LOG_ERROR(("%s:#d: malloc failed.\n",__FUNCTION__,__LINE__));
            break;
         }
   
         pDTMFCommand->Link = NULL;
         pDTMFCommand->DTMF_String = strdup(DTMF_String);
         pDTMFCommand->Command = strdup(cp);
         pTail->Link = pDTMFCommand;
         pTail = pDTMFCommand;
      }
   }

   if(fp != NULL) {
      fclose(fp);
   }

   Nested--;
   if(Nested == 0 && Ret == 0) {
      if(CommandFilename != NULL) {
         free(CommandFilename);
      }
      CommandFilename = strdup(Path);
   }

   return Ret;
}

int Node::LoadCommandFile(struct config_entry *p, char *Arg)
{
   int Ret = 0;
   DTMF_Command *pDTMFCommand;
   
   if(Arg != NULL) {
      while((pDTMFCommand = pDTMFCommands) != NULL) {
      // Clean up old commands before loading new ones
         pDTMFCommands = pDTMFCommand->Link;
         if(pDTMFCommand->DTMF_String != NULL) {
            free(pDTMFCommand->DTMF_String);
         }
         if(pDTMFCommand->Command != NULL) {
            free(pDTMFCommand->Command);
         }
         free(pDTMFCommand);
      }

      Ret = LoadCommands(Arg);
   }
   
   return Ret;
}

int LoadCommandFile(struct config_entry *p, char *Arg, AfCmd Cmd, int x)
{
   int Ret = -1;

   switch(Cmd) {
      case AF_SET_VAR:
         if(ConfigPass > 0) {
            Ret = CurrentNode->LoadCommandFile(p,Arg);
         }
         break;

      case AF_SET_DEFAULT:
         break;

      case AF_DISPLAY_VAR:
         if(CurrentNode->CommandFilename == NULL) {
            snprintf(Arg,x,"(not set)");
         }
         else {
            strncpy(Arg,CurrentNode->CommandFilename,x);
         }
         break;
   }
   
   return Ret;
}

void CmdSay(ClientInfo *p,ConfClient *pCC,char *Arg)
{
   char *cp = Arg;
   int ReplyPort = 0;
   int Flags= 0;

   if(pCC->bCmdLine) {
   // look for -c, if found then send TBD_SAY_COMPLETE on completion
   // look for -s, if found then spell the phrase out
      while(*cp == ' ') {
         cp++;
      }
      if(*cp == '-' && cp[1] == 'c') {
         ReplyPort = pCC->HisAdr.PORT;
         cp += 2;
      }
      else if(*cp == '-' && cp[1] == 's') {
         Flags |= SAY_FLAG_CALLSIGN;
         cp += 2;
      }

      while(*cp == ' ') {
         cp++;
      }
   }
   
   if(CurrentNode != NULL) {
      CurrentNode->SayPhrase(cp,Flags,ReplyPort);
   }
}

void CmdSweep(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   int StartFreq;
   int EndFreq;
   int SweepTime;
   int Sweeps;
   int Level = 0;
   int Converted;

   if(CurrentNode != NULL) {
      Converted = sscanf(Arg,"%d %d %d %d %d",
                         &StartFreq,&EndFreq,&SweepTime,&Sweeps,&Level);
      switch(Converted) {
         case 1:
         // Just start frequency, default the rest
            EndFreq = StartFreq;
         case 2:
            SweepTime = 15;
         case 3:
            Sweeps = 1;
         case 4:
            Level = 30000;
            break;

         default:
            break;
      }

      if(Level != 0) {
         char Line[80];

         snprintf(Line,sizeof(Line),"TF1 %d EF1 %d TL %d DUR %d RPT %d",
                  StartFreq,EndFreq,Level,SweepTime*1000,Sweeps);

         char *pErrPos;
         CToneGen *pGenerator = new CToneGen(CurrentNode->DefaultToneLevel);

         if(pGenerator->ToneGenSetup(Line,&pErrPos)) {
         // error
            BufPrintf(p,"Error: couldn't parse \"%s\"\r",pErrPos);
            Rcode = TBD_ERR_INVALID_ARG;
            delete pGenerator;
         }
         else {
            CurrentNode->AddToneGen(pGenerator);
         }
      }
      else {
         BufPrintf(p,"Usage: .sweep <start freq> <end freq> <sweep time> "
                   "<sweeps> <sweep level>\r");
      }
   }
}

void CmdDtmfDecode(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   if(CurrentNode != NULL) {
      DLOG(DLOG_DTMF,("%s: Calling DecodeDTMF, node: %s\n",__FUNCTION__,
           CurrentNode->NodeName));
      CurrentNode->DecodeDTMF(Arg);
   }
   else {
      ClientBufPrint prn(p);
      prn.print("Error: no node selected\n");
   }
}

void Node::AddToneGen(CToneGen *pGenerator)
{
   CToneGen *p = NULL;

   if(pGenerator->ID != 0) {
   // Only allow one tone generator of the same id to be running at a time
   // prevents arcade like sounds on weak signals among other annoying things!
      p = ToneGenerators;
      while(p != NULL) {
         if(p->ID == pGenerator->ID) {
         // Duplicate generator
            delete pGenerator;
            break;
         }
         p = p->Link;
      }
   }

   if(p == NULL) {
      pGenerator->Link = ToneGenerators;
      ToneGenerators = pGenerator;
   }
}

int Node::CancelToneGen(int id)
{
   int Ret = FALSE;
   CToneGen *pToneGen;
   CToneGen *pLastGen;
   CToneGen *pNextGen;

   pToneGen = ToneGenerators;
   pLastGen = NULL;
   while(pToneGen != NULL) {
      pNextGen = pToneGen->Link;
      if(pToneGen->ID == id) {
      // Delete this generator
         Ret = TRUE;
         if(pLastGen == NULL) {
            ToneGenerators = pNextGen;
         }
         else {
            pLastGen->Link = pNextGen;
         }
         delete pToneGen;
         break;
      }
      else {
         pLastGen = pToneGen;
      }
      pToneGen = pNextGen;
   }
   return Ret;
}

// Send CTone or RxTimeout tone to all transmitters that this receiver
// is connected to
void Node::SendCtone(int bDelayed)
{
   int ToneID = bRxTimeout ? RxTimeoutTone : Ctone;
   int bDelayedCtone = FALSE;

   if(ToneID != 0 && CtoneMap != NULL && LastDTMFDigit.tv_sec == 0) do {
   // We have a tone ID and need to send it to least one port and we're
   // not muting DTMF at the moment
      char *ToneSpec = FindToneSpec(ToneID);

      if(ToneSpec == NULL) {
         LOG_ERROR(("%s: Tone ID #%d is not defined, disabling.\n",
                    __FUNCTION__,ToneID));
         if(bRxTimeout) {
            RxTimeoutTone = 0;
         }
         else {
            Ctone = 0;
         }
         break;
      }

      LinkedNode *pLink = pTransmitters;
      while(pLink != NULL) {
      // This port is listening to our Rx
         class Node *pTx = pLink->pTx;

         do {
            if(pTx->bTxTimeOut) {
            // Don't send CTone to a timed out transmitter
               break;
            }

            if(!bRxTimeout && CtoneDelay != 0 && !bDelayed) {
            // Send it later
               bDelayedCtone = TRUE;
               break;
            }

            if(!bRxTimeout && pTx->CtoneResetsTimeout) {
            // Reset the transmitter's timeout timer
               pTx->TxKeyTime = TimeNow.tv_sec;
            }

            if(pLink->Flags & LNK_FLG_CTONE) {
            // CTone enabled to this transmitter
               CToneGen *pToneGen = new CToneGen(DefaultToneLevel);
               if(pToneGen != NULL) {
                  if(pToneGen->ToneGenSetup(ToneSpec,NULL)) {
                      delete pToneGen;
                  }
                  else {
                      pTx->AddToneGen(pToneGen);
                  }
               }
               else {
                  LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
               }
            }
         } while(FALSE);
         pLink = pLink->pNextTx;
      }
      
      if(bDelayedCtone != 0) {
      // Generating a delayed courtesy tone, start the timer
         CtoneTimer = TimeNow;
      }
   } while(FALSE);
}

// return TRUE on error
int Node::PlayStoredSound(int ID)
{
   int Ret = 0;
   do {
      if(ID == 0) {
         LOG_ERROR(("%s: Called with ID #%d.\n",__FUNCTION__));
         break;
      }
      char *ToneSpec = FindToneSpec(ID);

      if(ToneSpec == NULL) {
         LOG_ERROR(("%s: ID #%d is not defined.\n",__FUNCTION__,ID));
         Ret = TRUE;
         break;
      }
      CToneGen *pGenerator = new CToneGen(DefaultToneLevel);

      if(pGenerator != NULL) {
         if(pGenerator->ToneGenSetup(ToneSpec,NULL)) {
             delete pGenerator;
         }
         else {
             AddToneGen(pGenerator);
         }
      }
      else {
         LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
         Ret = TRUE;
      }
   } while(FALSE);

   return Ret;
}

int Node::Enabled()
{
   return TRUE;
}

void Node::CheckDtmfTimeouts(void)
{
   int TimeLapsed;

   if(LastDTMFDigit.tv_sec > 0 && !bDtmfDetect && !bPrefixWait) {
   // Check DTMF command timeout
      if((TimeLapsed = TimeLapse(&LastDTMFDigit)) > DTMFCommandTimeout) {
         DLOG(DLOG_DTMF,("Node: %s DTMF timeout\n",NodeName));
         if(DTMF_Digits > 0 && (DTMFPrefix == NULL || bPrefixed)) {
            DecodeDTMF(DTMF_Buf);
         }
         DTMF_Digits = 0;
         LastDTMFDigit.tv_sec = 0;  // force a prefix timeout on command timeout
      }
   }

   if(DTMF_Digits == 0 && bPrefixed) {
   // Check for prefix timeout
      if(TimeLapse(&LastDTMFDigit) > PrefixTimeout) {
         if(PrefixedTone != 0) {
            CancelToneGen(PrefixedTone);
         }
         bPrefixed = 0;
      }
   }
}

void Node::DumpStatus(CText *p)
{
   int bFirst = TRUE;

   p->print("%c %s",CurrentNode == this ? '>' : ' ',NodeName);
   if(bRxActive) {
      p->print("%sRx",bFirst ? " " : ", ");
      bFirst = FALSE;
   }
   if(bTxKeyed) {
      p->print("%sTx",bFirst ? " " : ", ");
      bFirst = FALSE;
   }
   if(bTxTimeOut) {
      p->print("%sTxTo",bFirst ? " " : ", ");
      bFirst = FALSE;
   }
   if(LastDTMFDigit.tv_sec != 0) {
      p->print("%sDtmf",bFirst ? " " : ", ");
      bFirst = FALSE;
   }
   if(bRxTimeout) {
      p->print("%sRxTo",bFirst ? " " : ", ");
      bFirst = FALSE;
   }

   p->print("\r");
}


void Node::PrintRxLevel(ClientBufPrint *p)
{
   p->print("%-5d\t%-5d\t%-5d\t%-5.0f\t%-4d\n",MinInSample,MaxInSample,
             MinMaxTotal/MinMaxSamples,
             sqrt(InSquaredSum/MinMaxSamples),MinMaxSamples);
   MinMaxSamples = 0;
   MinMaxTotal = 0;
   MinInSample = 32767;
   MaxInSample = -32786;
   InSquaredSum = 0.0;
}

void Node::LevelMonitor(int16 *pBuf,int Samples)
{
   int i;
   int16 x;

   MinMaxSamples += Samples;
   for(i = 0; i < Samples; i++) {
      x = *pBuf++;
      if(MaxInSample < x) {
         MaxInSample = x;
      }
      else if(MinInSample > x) {
         MinInSample = x;
      }
      MinMaxTotal += x;
      InSquaredSum += x * x;
   }
   
   if(MinMaxReplyPort != -1 && MinMaxSamples >= 2000) {
   // Send level reports about 4 times a second
      ConfClient *pCC = &CmdLineCC;
      ClientInfo *pCInfo = CmdClient;
      int Err;

   // Restore the saved reply port.  
   // (we can't just use pCC->HisAdr.PORT since another client may have 
   // sent a command since the client that is monitoring levels)
      
      pCC->HisAdr.PORT = MinMaxReplyPort;
      pCInfo->Count = 0;
      ClientBufPrint *prn = new ClientBufPrint(pCInfo);

      prn->print("%d\n",TBD_RX_LEVEL_RPT);
      PrintRxLevel(prn);
      delete prn;

      Err = sendto(pCInfo->Socket,pCInfo->Buf,pCInfo->Count+1,0,&pCC->HisAdr.s,
                   sizeof(IPAdrUnion));
      printf("%s: sento returned %d\n",__FUNCTION__,Err);
      if(Err == -1) {
      // Send failed, tlbcmd probably exited, stop sending
         MinMaxReplyPort = -1;
      }
   }
}



int SayDisconnectResult(ClientInfo *p,ConfClient *pCC,char *Arg)
{
   int Ret = FALSE;
   
   if(LastRcode == TBD_OK && CurrentNode != NULL) {
      if(StationDisconnected != NULL) {
         CurrentNode->SayPhrase(StationDisconnected,SAY_FLAG_CALLSIGN);
         CurrentNode->SayPhrase("disconnected");
         Ret = TRUE;
      }
   }

   return Ret;
}

