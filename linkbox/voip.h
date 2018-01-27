/* Copyright (C) 2008 GFRN systems

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

   $Log: voip.h,v $
   Revision 1.10  2012/12/09 18:39:15  wb6ymh
   Removed QueueAsterisckData, added DummyLink.

   Revision 1.9  2010/11/17 04:56:56  wb6ymh
   Added G.726 support.

   Revision 1.8  2009/09/13 19:25:23  wb6ymh
   Added AnnouceDisconnection member.

   Revision 1.7  2009/04/04 18:17:08  wb6ymh
   Added bDisconnectAfterWelcome, WelcomeFile and ConnectScript.

   Revision 1.6  2009/02/14 15:52:26  wb6ymh
   Massive restructuring of the audio buffering algorithm to minimize port to
   port audio delays.

   Revision 1.5  2008/12/30 16:26:16  wb6ymh
   Removed Add2CtoneTimer.

   Revision 1.4  2008/07/23 17:23:09  wb6ymh
   Fairly massive changes to support full duplex operation via a conference.
   Moved all inbound stream routines and variables from VoipNode to a
   new class VoipSink.

   Revision 1.3  2008/06/26 20:19:42  wb6ymh
   Added EndPointCleanup.

   Revision 1.2  2008/03/11 14:22:26  wb6ymh
   1. Moved RtpData here from conference.h.
   2. Added AllocAudioBuf.

   Revision 1.1  2008/03/09 17:10:14  wb6ymh
   Inital import - files broken out of old audio.cpp and audio.h due to
   major restructuring of the internal infrastructure to treat VoIP connections
   as pseudo ports.

*/
#ifndef _VOIP_H_
#define _VOIP_H_

typedef struct RtpDataTAG {
   struct RtpDataTAG *Link;   // NB: must be first
   rtp_hdr_t   *pData;
   int   Len;           // length of packet
} RtpData;

class VoipNode;

class VoipSink
{
public:
   VoipSink(VoipNode *pVoipNode);
   ~VoipSink();
   VoipSink *Link;
   void InsertInRTPQ(RtpData *pRtpData,int MaxNetQueueLen);
   void QueueAudio(NetAudio *pAudio);
   void DecompressAudio();
   void DecompressRTP();
   void DecompressULaw(char *p,int Len);
   void DecompressGSM(gsm_byte *pGSM,int Len,gsm gsmh);
   void DecompressADPCM(char *p,int Len,struct adpcm_state *pState);
	void DecompressG726(uint8_t *p,int Len);
   void StopTalking();
   int AudioAvailable(int bForce);
   NetAudio *GetAudioBuf();
   void QueueSFData(soundbuf *p);
   void QueueRTPData(RtpData *pRtp);
   
   ConfClient *mCC;
   char *NodeName;
   char SendingHost[16];   // SF protocol
   u_int32 ssrc;           // iLink, RTP protocols
   struct timeval LastRx;
// Queue of audio packets
   NetAudio *pInHead;

private:
   VoipNode *pParent;

   int   MissedPackets;
   int   OutofSeqPackets;
   int   DuplicatePackets;
   int   LatePackets;


// sequence number of next packet that will be converted to audio when needed
   u_int16  NextSeq;

   NetAudio *pInTail;
   int      AudioQCount;

   RtpData  *pRtpQueue;
   int   RtpQCount;
   gsm   gsmh;
	g726_state_t *g726h;

// receive state transisions: 
// idle -> buffering -> active -> idle
//           ^            |
//           +------------+
// 
   
   int   bRxIdle:1;
   int   bRxBuffering:1;
   int   bRxActive:1;
};

class VoipNode : public Node
{
public:
// public functions   
   VoipNode(ConfClient *pCC);
   ~VoipNode();
   int WriteAudioOut(ClientInfo *p);
   int SamplesAvailable(LinkedNode *pLink,int bForce);
   int GetRxAudio(LinkedNode *pLink,void **pAudio,int MaxSamples,int bForce);
   void StopTalking();
   void DecompressAudio();
   void QueueSFData(soundbuf *p);
   void QueueRTPData(RtpData *pRtp);
   void PollTx();
   static void VoipInit(ConfClient *pCC);
   int Enabled();
   static NetAudio *AllocAudioBuf(int Bytes);
   void AnnounceDisconnection();

   ConfClient *mCC;

// Configuration variables
   int bDisconnectAfterWelcome;
   char *WelcomeFile;
   char *ConnectScript;

private:
// Queue of audio packets
   NetAudio *pInHead;
   NetAudio *pInTail;
   int      AudioQCount;
   struct AdpcmTAG {
      union {
         soundbuf SfHdr;
         rtp_hdr_t RtpHdr;
      } u;
      char AdpcmStateBuf[4];
      char Buf[ADPCM_PACKET_DATA_BYTES];
      char AdpcmStateBuf1[3];
   } GCC_PACKED Adpcm;
   gsm hGSM_Encode;
	g726_state_t *hG726_Encode;
   
   int EncodedBytes; // number of bytes of compressed ADPCM data in buffer
   
   struct adpcm_state TxADPCMStart;
   struct adpcm_state TxADPCM;
   u_int16 TxSequence;
   u_int16 RtpTxSequence;
   char GSM_Packet[SIZEOF_SOUNDBUF + (GSM_FRAME_LEN * 12) + 2];
   int GsmFramesAvail;
   static int AudioBufSn;
   int16 *TxAudioBuf;
   u_int32 TxBufSize;
   int AudioAvailable;

   VoipSink *pSources;

   struct timeval TxStartTime;
   int TxPackets;
   int SamplesPerPacket;
   LinkedNode DummyLink;

   int  DeleteTx(LinkedNode *pLink);
   void AddTx(LinkedNode *pLink);
   void ConsumeAudio(NetAudio *pAudio);
   void UlawTx();
   void AdpcmTx();
   void GsmTx();
	void G726Tx();
   void SendAudioPacket(void *Buf,int TxLen);
   VoipSink *FindSink(u_int32 ssrc,const char *SendingHost);
};

void EndPointCleanup(ConfClient *pCC);

#endif // __VOIP_H_
