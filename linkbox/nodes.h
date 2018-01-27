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

   $Log: nodes.h,v $
   Revision 1.15  2012/12/09 16:29:51  wb6ymh
   Added LNK_FLG_DUMMY.

   Revision 1.14  2009/09/13 19:45:06  wb6ymh
   1. Added PrintRxLevel, LevelMonitor, DefaultToneLevel, IRLPAnnouncements,
      MaxInSample, MinInSample, MinMaxSamples, MinMaxTotal, MinMaxReplyPort,
      InSquaredSum, LastDTMFDigit, LastDTMFDigitStart, and bPrefixWait.
   2. Added support for floating point CTCSS high pass filter for testing.
   3. Removed NoAudioInLooping, it should have been a CPort variable.

   Revision 1.13  2009/07/03 17:08:36  wb6ymh
   1. Added AudioPriority argument to AddRx.
   2. Deleted FakeToneburst, ToneBurstTimer and ToneBurstAccessCheck.

   Revision 1.12  2009/07/01 14:58:04  wb6ymh
   Added LOW_AUDIO_PRI, MED_AUDIO_PRI, HIGH_AUDIO_PRI, PriorityText,
   AudioPriority, ReducedGain, AnnouncementPriority, AudioPriority,
   NoAudioInLooping, DebugFpRxCtcssPB and DebugFpRawAudioPB.

   Revision 1.11  2009/04/04 16:19:25  wb6ymh
   1. Added MinPlayBackPause, MaxPlayWithoutPause, ExecuteCommand, and
      ExpandCommand.
   2. Deleted obsolete cw generator variables.

   Revision 1.10  2009/02/14 15:27:55  wb6ymh
   1. Made DumpStatus a Node method rather than a CPort only method.  Modified
      CmdSetPort to match.
   2. Modified CmdPCM to take a const char * instead of a char * to ease calling
      with fixed strings.
   3. Removed calls to the obsolete KickStartAudioOut kludge.
   4. Massive restructuring of the audio buffering algorithm to minimize port to
      port audio delays.
   5. Replaced orignal cw ID code with tone generator based CW to simplify things.
   6. Added bDtmfDetect status bit.
   7. Added RawAudioIn, AudioOut, and DtmfIn test points to the .pcm command.
   8. Added ToneBurstAccessCheck.

   Revision 1.9  2009/01/03 23:30:43  wb6ymh
   1. Added DebugFp*, bRepeater, bNailedUp, RxDeemphasis, TxPreemphasis,
      TxCtcssFilter, PreemphasisTemp, DeemphasisTemp, CmdPCM, and
      TxCtcssFilterTemp to the Node class.
   2. Added bNailedUp argument to AddRx.
   3. Added Add2Timer prototype.

   Revision 1.8  2008/08/09 15:54:47  wb6ymh
   Added support for port configuration variable ConnectAnnouncements to
   allow connect and disconnect announcements to be enabled/disabled on a
   per port basis.

   Revision 1.7  2008/07/23 16:30:00  wb6ymh
   1. Added AUDIO_BUF_SIZE_BYTES.
   2. Added Used, BufSN and Flags to NetAudio structure.
   3. Made MixAudio member of the Node class public and added Out argument.
   4. Added MaxSamples, and bForce arguments to GetRxAudio member..

   Revision 1.6  2008/06/26 16:37:54  wb6ymh
   Added bCoverToneRunning, LoadCommands.

   Revision 1.5  2008/05/14 21:06:38  wb6ymh
   1. Added Flags argument to SayPhrase.
   2. Deleted obsolete SayCall.
   3. Added SpellCall, ClientBufPrint calss, CTCSSAngleDelta, CtcssTxLevel.

   Revision 1.4  2008/04/25 05:13:57  wb6ymh
   Added  support for site prefix decoding.

   Revision 1.3  2008/03/18 20:04:14  wb6ymh
   Added bCwIsID and bAnnoucementIsID.

   Revision 1.2  2008/03/11 14:08:45  wb6ymh
   1. Moved NetAudio here from conference.h.
   2. Deleted LNK_FLG_AUDIO_RDY from LinkedNode.
   3. Added BufNum to LinkedNode.
   4. Added BufNum to Node class.

   Revision 1.1  2008/03/09 17:10:13  wb6ymh
   Inital import - files broken out of old audio.cpp and audio.h due to
   major restructuring of the internal infrastructure to treat VoIP connections
   as pseudo ports.

*/

#ifndef _NODE_H_
#define _NODE_H_

#ifdef USB_SUPPORT
#include <usb.h>
#endif

#include "ToneGen.h"
#include "sf.h"
#include "adpcm.h"
#include "gsm.h"
#include "ilink.h"
#include "tone_detect.h"
#include "super_tone_rx.h"
#include "tone_generate.h"
#include "dtmf.h"
#define  AUDIO_BUF_SIZE    ILINK_AUDIO_SAMPLES_PER_PACKET
#define  AUDIO_BUF_SIZE_BYTES (int) (ILINK_AUDIO_SAMPLES_PER_PACKET * sizeof(int16))

#define LOW_AUDIO_PRI   0
#define MED_AUDIO_PRI   1
#define HIGH_AUDIO_PRI  2

extern const char *PriorityText[];

#ifndef _BUFPRINT_H_
#include "Ctext.h"
#endif

typedef struct PlayPCM_TAG {
   struct PlayPCM_TAG *Link;  // NB: must be first
   int   Flags;
      #define PLAY_FLAG_ID       0x1   // voice ID
      #define PLAY_FLAG_TTS      0x2   // FileName = Text for TTS system
      #define PLAY_FLAG_TTS_OUT  0x4   // audio rendered by TTS system
      #define PLAY_FLAG_NO_PAUSE 0x7   // Types that can't be paused
      
   char  *FileName;
   int ReplyPort; 
} PlayPCM;

typedef struct DTMF_Command_Tag {
   struct DTMF_Command_Tag *Link;   // NB: must be first
   char  *DTMF_String;
   char  *Command;
} DTMF_Command;

typedef struct NetAudioTAG {
   struct NetAudioTAG *Link;  // NB: must be first
   int   Available;     // samples available
   int   Used;          // samples already consumed
   int   Users;         // number of nodes yet to consume this buffer
   int   BufNum;        // sequencial buffer number
   int   BufSN;         // debug
   char  Flags;
      #define AUDIO_FLAG_32BIT   1
   char  Buf[1];        // actual length varies
} NetAudio;

typedef struct LinkedNodeTAG {
   struct LinkedNodeTAG *pNextTx; // For the receiver nodes use
   struct LinkedNodeTAG *pNextRx; // For the transmitter nodes use
   int Flags;
      #define LNK_FLG_CTONE         1  // Send courtesy to this tx
      #define LNK_FLG_CTCSS_FILTER  2  // filter ctcss before sending to this tx
      #define LNK_FLG_DEEMPHASIS    4  // receiver wants deemphasized audio
      #define LNK_FLG_NAILED_UP     8  // ignored by .unlink all command
      #define LNK_FLG_FIRST_READ    0x10  // CPort just active recently
      #define LNK_FLG_DUMMY         0x20  // Dummy link for DTMF decoder
   class Node *pRx;
   class Node *pTx;

   NetAudio *pInHead;   // used by VoipNode to queue pcm
   int   Used;          // samples used from current NetAudio buffer
   int   BufNum;        // Last buffer consumed (used by Voip to return audio)
   int   RdNdx;         // used by CPorts to return audio
   int   AudioPriority;
} LinkedNode;

class NamedNode {
public:
   virtual ~NamedNode();   // NB: must be here for a kludge casts to work!
   char *NodeName;
};

class ClientBufPrint: public CText
{
public:
// public functions   
   ClientBufPrint(ClientInfo *pClient) : 
      CText::CText(pClient->BufSize,pClient->Count,pClient->Buf) { 
         p = pClient;
   }
   ~ClientBufPrint() {
      p->Count = Count;
   }

private:
   ClientInfo *p;
};

class Node : public NamedNode 
{
   friend class VoipSink;
public:
// public functions   
   Node();
   virtual ~Node();
   void SaveDTMF(char Digit);
   void DecodeDTMF(char *Command);
   int CancelToneGen(int id);
   void AddToneGen(CToneGen *pGenerator);
   virtual int WriteAudioOut(ClientInfo *p) = 0;
   int LoadCommandFile(struct config_entry *p, char *Arg);
   void SayPhrase(const char *Phrase,int Flags = 0,int ReplyPort = 0);
      #define SAY_FLAG_CALLSIGN     1
// Return number of "live" samples of audio that are available (i.e.
// from physical ports and VoIP connections)
   int LiveSamplesAvailable();
// Return number of samples of audio that are available
   virtual int SamplesAvailable(LinkedNode *pLink,int bForce) = 0;
// Get receiver audio.  Return number of samples available @ pAudio.  
// Ret < 0 means 32 bits samples, otherwise 16 bit samples
   virtual int GetRxAudio(LinkedNode *pLink,void **pAudio,int MaxSamples,
                          int bForce) = 0;
   virtual void AddTx(LinkedNode *pLink);
   virtual int DeleteTx(LinkedNode *pLink);
   virtual void AddRx(Node *pNode,int bNailUp = 0,int AudioPriority = -1);
   virtual int DeleteRx(Node *pNode);
   int UnlinkAll(int bNailedToo = 0);
   int Shutup();
   void UpdateCtoneFlags();
   int InitToneBuf(struct config_entry *p, char *Arg);
   virtual int Enabled();
   void CmdToneGen(ClientBufPrint *p,char *Arg);
   static void MixAudio(int *Out,void *In,int Samples);
   void CmdPCM(ClientBufPrint *p,const char *Arg);
   void DumpStatus(CText *p);
   void PrintRxLevel(ClientBufPrint *p);
   void LevelMonitor(int16 *pBuf,int Samples);

// Config variables
   int DtmfCTCSSMode;
   int Min1stDigitHoldTime;
   int MuteDTMF;
   int DtmfCoverTone;
   int MinDtmfInterdigitTime;
   int DtmfMethod;
   int Twist;
   int ReverseTwist;
   int Ctone;
   int CtoneDelay;
   int TxTimeout;
   int RxTimeout;
   int TxTimeoutTone;
   int RxTimeoutTone;
   int CtoneResetsTimeout;
   int CTCSSLevel;
   int CWToneLevel;
   int CWSpeed;
   int CWAutoJam;
   int CWToneFreq;
   int PrefixedTone;
   int DTMFCommandTimeout;
   int PrefixTimeout;
   int ConnectAnnouncements;
   int RxDeemphasis;
   int TxPreemphasis;
   int TxCtcssFilter;
   int DisableWelcome;
   int ToneBurstMode;
   int ToneBurstFreq;
   int ToneBurstMute;
   int ToneBurstTimeout;
   int ToneBurstToneID;
   int MinPlayBackPause;
   int MaxPlayWithoutPause;
   int ReducedGain;
   int AnnouncementPriority;
   int AudioPriority;
   int DefaultToneLevel;
   int IRLPAnnouncements;

   char *Dtmf1stDigits;
   char *CommandFilename;
   char *CtoneMap;
   char *DTMFPrefix;

// Variables accessed by C, eventually to be moved to protected ?
   dtmf_rx_state_t DtmfRxState;
   LinkedNode *pReceivers; // Linked list of receivers feeding this transmitter
   LinkedNode *pTransmitters; // Linked list of transmitters this receiver feeds
   int Transmitters;    // number of transmitters this receiver feeds

   int bRxActive;
   int bCtcssActive;
   int bTxKeyed;

// input level stats
   int MaxInSample;
   int MinInSample;
   int MinMaxSamples;
   int MinMaxTotal;
   int MinMaxReplyPort; 
   double InSquaredSum;

protected:
   DTMF_Command *pDTMFCommands;
   struct timeval LastDTMFDigit;       // time last DTMF digit was last decoded
   struct timeval LastDTMFDigitStart;  // time last DTMF digit was first decoded
   #undef MAX_DTMF_DIGITS
   #define MAX_DTMF_DIGITS    16
   char  DTMF_Buf[MAX_DTMF_DIGITS+1];
   int   DTMF_Digits;
   CToneGen *ToneGenerators;
   FILE *Playfp;
   PlayPCM *pPlayHead;
   PlayPCM *pPlayTail;
   int b8BitFile;
   time_t LastActivity;
   int PlayAvail;    // in samples
   int MaxMixedLevel;
   int LastMixGain;
   int bTxTimeOut;
   CToneGen *TxTimeoutToneGen;
   double CTCSSAngle;
   int ElementCount;
   int bCwToneOn;
   int bElementSpace;
   int bRxTimeout;
   int RampCount;
   const char *Element;
   time_t TxKeyTime;
   struct timeval CtoneTimer;
   double CTCSSAngleDelta;
   int CtcssTxLevel;
   int BufNum;
   int PreemphasisTemp;
   int DeemphasisTemp;
#ifdef FLOAT_CTCSS_FILTER
   double TxCtcssFilterTemp[8];
#else
   int TxCtcssFilterTemp[8];
#endif
   goertzel_state_t *ToneBurstState;
   goertzel_descriptor_t ToneBurstDesc;
   float Energy;

   FILE *DebugFpRxDeemphasis;
   FILE *DebugFpTxMix;
   FILE *DebugFpTxPreemphasis;
   FILE *DebugFpTxCtcssFilter;
   FILE *DebugFpRxCtcssFilter;
   FILE *DebugFpRxCtcssPB;
   FILE *DebugFpTxIn;
   FILE *DebugFpAudioIn;
   FILE *DebugFpAudioOut;
   FILE *DebugFpRawAudioIn;
   FILE *DebugFpRawAudioPB;
   FILE *DebugFpDtmfIn;

   int bAnnoucementIsID:1; // current annoucement is an transmitter ID
   int bPrefixed:1;        // site prefix decoded
   int bPrefixWait:1;      // waiting for 1'st digit after prefix
   int bCoverToneRunning:1;// DTMF cover tone is active
   int bDtmfDetect:1;      // DTMF tone being decoded
   int bToneBurstWasDetected:1;  // This receiver has detected a toneburst
   int bToneBurstDetected:1;  // This receiver is detecting a toneburst
   int bToneBurstAccessed:1;  // ToneBurst, if required, has been received

   int PlayStoredSound(int ID);
   int GetAnnounceAudio(int SamplesNeeded);
   void StartNextAnnoucement();
   void FreeAnnoucement();
   void PlayAnnouncementFile(char *FileName,int Flags,int ReplyPort = 0);
   void ScaleMixedAudio(int16 *OutBuf,int Samples);
   int GetTxAudio(int16 *TxBuf,int SamplesNeeded);
   void GenCTCSS(int16 *CTCSSBuf,int Samples);
   void SendCtone(int bDelayed);
   void CheckDtmfTimeouts(void);
   int CheckPrefix(void);
   void SpellCall(const char *Call,CText *pPhrase);
   int LoadCommands(char *Filename);
   void ExecuteCommand(char *Command);
   void ExpandCommand(char *Command,char *DtmfCommand,char *DtmfWildCards);

private:
};

extern int bTTS_Running;
extern int16 AudioBuf[AUDIO_BUF_SIZE]; // scratch buffer used by GetTxAudio
extern int16 MasterBuf[AUDIO_BUF_SIZE];
extern int16 SlaveBuf[AUDIO_BUF_SIZE];

int NodeCompare(const void *avl_a,const void *avl_b,void *avl_param);
char *FindToneSpec(int ID);
Node *FindNode(char *NodeName);
void DtmfCallback(void *user_data, int code, int level);
void Add2Timer(struct timeval *pTimer,int AudioSamples);

#endif   //_NODE_H_

