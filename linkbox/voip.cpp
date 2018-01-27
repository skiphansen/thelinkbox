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

   $Log: voip.cpp,v $
   Revision 1.24  2012/12/09 18:38:02  wb6ymh
   1. Added code to service software DTMF decoder when no physical ports are
   configured.
   2. Removed old chan_rptdir style Asterisk support.
   3. Corrected memset() argument order in VoipInit.

   Revision 1.23  2010/11/17 04:56:34  wb6ymh
   Added G.726 support.

   Revision 1.22  2009/09/13 23:26:41  wb6ymh
   1. Replaced all references to bInConf with references to bConnected.
   2. Added support for the .rxlevel command.
   3. Improved the implementation of bRxActive, it's now based on the same
      logic as the courtesy tone.
   4. Added code to process any waiting DTMF commands when a VoIP ports
      bRxActive drops.
   5. Modified disconnection announcement code to prevent the unpopular
      double announcements.
   6. Implemented the new IRLPAnnouncements configuration variable.

   Revision 1.21  2009/07/19 14:17:43  wb6ymh
   Added initialization of ConnectAnnoucements to constructor.

   Revision 1.20  2009/07/03 17:38:57  wb6ymh
   1. Deleted FakeToneBurst.
   2. Modified DeleteTx to call Node::DeleteTx after freeing buffers.

   Revision 1.19  2009/05/28 15:55:31  wb6ymh
   Improved SLOG debug logging.

   Revision 1.18  2009/04/04 18:22:01  wb6ymh
   1. Turned off debug level DLOG_RTP that was inadvertently left on in the last
      release.
   2. Added Welcome file support for VoIP connections.
   3. Corrected DtmfMethod type for software DTMF decoding in Node::GetRxAudio.
      Prevented DTMF from being decoded while multiple streams were active.
   4. Added ConnectScript support.

   Revision 1.17  2009/02/16 00:29:45  wb6ymh
   1. Modified VoipInit to prevent welcome message from being played for the
      file player pseudo VoIP client.  Caused no real harm other than a bunch
      of odd log messages.
   2. Added RTP sequence number handling debug logging enabled by DLOG_RTP.
   3. Modified InsertInRTPQ to detect duplicate packets when they occur back
      to back.

   Revision 1.16  2009/02/14 18:21:51  wb6ymh
   Initialize bTxKeyed to FALSE in constructor.

   Revision 1.15  2009/02/14 15:51:38  wb6ymh
   1. Massive restructuring of the audio buffering algorithm to minimize port to
      port audio delays.
   2. Added uLaw support to QueueRTPData.
   3. Added .wav format "welcome" file support to VoipInit.

   Revision 1.14  2009/01/03 22:58:11  wb6ymh
   Removed bogus error message generated when DtmfMethod was 0 (no DTMF decode).

   Revision 1.13  2008/12/31 21:24:53  wb6ymh
   Corrected bug in ~VoipNode which caused disconnect messages to be routed
   to the wrong ports.  (The Transmitter list was being walked using the wrong
   link).

   Revision 1.12  2008/12/30 16:25:43  wb6ymh
   1. Added support for .pcm record AudioIn.
   2. Enable transmitter CTCSS filter by default.
   3. Generalized Add2CtoneTimer to Add2Timer and removed from VoIP class.
   4. Changed software DTMFMethod from 1 to 3 to match physical ports.
   5. Replaced the poorly conceived VoipOutMap and VoipInMap configuration file
      variables with LinkVoipIn and LinkVoipOut port configuration variables.

   Revision 1.11  2008/08/09 15:54:47  wb6ymh
   Added support for port configuration variable ConnectAnnouncements to
   allow connect and disconnect announcements to be enabled/disabled on a
   per port basis.

   Revision 1.10  2008/07/27 22:01:43  wb6ymh
   1. Update Asterisk support for chan_rtpdir version 0.5 which added sequence
      numbers and DTMF support.
   2. Improved Debug = 0x10 logging a lot for debugging.
   3. Modified VoipNode::~VoipNode to only Consume audio that hasn't been
      consumed previously.  Corrects assert failure when disconnecting from an
      active link when multiple transmitters were linked.
   4. Corrected bug in VoipNode::AddTx properly increment the Users count on
      available buffers when a new transmitter is added to an active receiver.
      Corrects assert failure when adding a transmitter to an active receiver.
   5. Corrected a bug in VoipNode::GetRxAudio that corrupted audio when driving
      multiple transmitters.
   6. Corrected a bug in VoipNode::GetRxAudio that caused audio corruption on
      protocols/codecs which carried other than 640 audio samples per packet.
      (Everything other than Asterisk and EchoLink).
   7. Corrected a bug in VoipNode::GetRxAudio that caused the CTone time to be
      miscalculated when there were multiple transmitters.

   Revision 1.9  2008/07/23 17:22:14  wb6ymh
   1. Fairly massive changes to support full duplex operation via a conference.
      Moved all inbound stream routines and variables from VoipNode to a
      new class VoipSink.  Modified VoipNode to handle multiple inbound streams
      from a single Voip client identified by different ssrc (EchoLink or RTP) or
      SendingHost (Speak Freely).

   2. Added support for PROTO_ASTERISK.

   3. Added uLaw support.

   4. Added <callsign>_fd.conf, "port####.conf" and port####_fd.conf to the list
      of configuration files which are searched for configuration information
      when a new VoIP client connects.

   5. Corrected Internet dejitter buffer operation for the Speak freely protocol.

   Revision 1.8  2008/06/26 20:19:21  wb6ymh
   1. Added code to ~VoipNode to send disconnect announcements to ports listed 
      in the VoipInMap or VoipOutMap configuration variables.

   2. Added code to VoipInit send connect announcements to ports listed in the
      VoipInMap or VoipOutMap configuration variables.

   Revision 1.7  2008/05/14 22:56:16  wb6ymh
   Replaced <sys/limits.h> file with <sys/param.h>.  FreeBSD bitches if you
   try to include <sys/limits.h>

   Revision 1.6  2008/05/14 18:22:26  wb6ymh
   Corrected compile problems with GCC 4.2.x in VoipInit.

   Revision 1.5  2008/04/24 23:59:13  wb6ymh
   Added callto CheckDtmfTimeouts to PollTx.

   Revision 1.4  2008/04/11 16:48:40  wb6ymh
   Modified GetRxAudio to clear CToneTimer before calling SendCtone.  Fixes
   infinite loop crash under certain rare timing conditions).

   Revision 1.3  2008/03/12 21:44:41  wb6ymh
   Modified VoipNode::VoipNode to make a copy of the Callsign rather than
   saving a pointer.  The address keeps changing as SDES packets are processed
   by conference.c.

   Revision 1.2  2008/03/11 14:21:43  wb6ymh
   1. Moved include for common.h to after system headers.  Fixes compile
      problems (min macro) on some systems.
   2. Added ALOG macro and logging in numerious places to track audio processing.
      Enabled by DLOG_VOIP_AUDIO bit (0x10).
   3. Added BufNum to NetAudio structure to track transmitter's current position
      in the receive audio stream.  Fixes bug that caused one transmitter to
      consume a receiver's audio N times when the receiver was linked to N
      transmitters.
   4. Corrected a bug in GetRxAudio which caused a crash when the CToneTimer was
      started when only a single buffer was queued for consumption.  This
      occurs when there's packet loss.

   Revision 1.1  2008/03/09 17:10:14  wb6ymh
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/soundcard.h>
#ifdef HAVE_FCNTL_H
   #include <fcntl.h>
#endif
#include <netdb.h>
#include <ctype.h>
#ifdef HAVE_SYS_TIMEB_H
   #include <sys/timeb.h>
#endif
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <math.h>
#include <typeinfo>
#include <sys/param.h> // for PATH_MAX
#include <sys/file.h>

#include "common.h"
#include "rtp.h"
#include "avl.h"
#include "main.h"
#include "configvars.h"
#include "configfile.h"
#include "dirclient.h"
#include "conference.h"
#include "users.h"
#include "eventhook.h"
#include "ctcss.h"
#include "tbd.h"
#include "linkbox.h"
#include "nodes.h"
#include "ports.h"
#include "ulaw2linear.h"
#include "bitstream.h"
#include "g726.h"
#include "voip.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define ALOG(x) if(Debug & DLOG_VOIP_AUDIO) LogErr x
#define SLOG(x) if(Debug & DLOG_RTP) LogErr x

int VoipNode::AudioBufSn = 1;

/*
   return if a <= b < c in a circular sense.
   (From Tanenbaum 1981, pg 158)
*/
int Between(u_int16 a, u_int16 b, u_int16 c)
{
   if( (a <= b && b < c) || (c < a && a <= b) || (b < c && c < a) ) {
      return TRUE;
   }
   return FALSE;
}

// Called by "conference" code when an rtp event occurs
int EndPoint(ClientInfo *p,ConfClient *pCC,EventType Event)
{
   RtpData *pRtpData;
   VoipNode *pVoipNode = (VoipNode *) pCC->pVoipNode;

   if(pVoipNode != NULL || Event == EVENT_INIT) {
      switch(Event) {
         case EVENT_INIT:
            VoipNode::VoipInit(pCC);
            break;

         case EVENT_RTP_RX:
           if(pCC->bConnected && !pCC->bMuted) {
               if(pCC->Proto == PROTO_ILINK || pCC->Proto == PROTO_RTP) {
                  pRtpData = (RtpData *) malloc(sizeof(RtpData));
                  if(pRtpData != NULL) {
                     pRtpData->pData = (rtp_hdr_t *) p->Buf;
                     pRtpData->Len = p->Count;
                     pRtpData->Link = NULL;
                     p->Buf = (char *) malloc(p->BufSize);
                  // For debug convenience convert the sequence number into 
                  // host order
                     pRtpData->pData->seq = ntohs(pRtpData->pData->seq);
                  }
                  pVoipNode->QueueRTPData(pRtpData);
                  DFLUSH();
               }
               else if(pCC->Proto == PROTO_SF) {
                  pVoipNode->QueueSFData((soundbuf *) p->Buf);
               }
               else {
               // This shouldn't happen !
                  CORE_DUMP();
               }
            }
            break;

         case EVENT_RTP_TO:
            D2PRINTF(("e"));
            pVoipNode->StopTalking();
            DFLUSH();
            break;

         case EVENT_RTCP_RX:
            break;

         case EVENT_RTCP_TO:
            pVoipNode->AnnounceDisconnection();
            break;

         case EVENT_DELETE:
         // cleanup, this guy is going to be history shortly.
#if 0
            if(fp != NULL) {
               fclose(fp);
               fp = NULL;
            }
#endif
            EndPointCleanup(pCC);
            break;

         case EVENT_DTMF_RX:
            if(pVoipNode != NULL && pVoipNode->DtmfMethod == 5) {
               pVoipNode->SaveDTMF(p->Buf[0]);
            // We don't get button release events so queue one now
               pVoipNode->SaveDTMF(' ');
            }
            break;
      }
   }
   else {
      LOG_ERROR(("%s: pVoipNode == NULL, Event: %d, client: %s.\n",__FUNCTION__,
                 Event,pCC->Callsign));
   }
   return FALSE;
}

VoipSink::VoipSink(VoipNode *pVoipNode)
{
   Link = NULL;
   AudioQCount = 0;
   pParent = pVoipNode;
   mCC = pParent->mCC;
   NodeName = pParent->NodeName;
   bRxBuffering = FALSE;
   bRxIdle = TRUE;
   gsmh = NULL;
   g726h = NULL;
   LatePackets = 0;
   MissedPackets = 0;
   NextSeq = 0;
   OutofSeqPackets = 0;
   pInHead = NULL;
   pRtpQueue = NULL;
   RtpQCount = 0;
   SendingHost[0] = 0;
}

VoipSink::~VoipSink()
{
   NetAudio *pAudio;
   RtpData *pRtp;

   if(gsmh != NULL) {
      gsm_destroy(gsmh);
   }

   if(g726h != NULL) {
      g726_release(g726h);
   }

   while((pAudio = pInHead) != NULL) {
      pInHead = pAudio->Link;
      AudioQCount--;
      free(pAudio);
   }

   while((pRtp = pRtpQueue) != NULL) {
      pRtpQueue = pRtp->Link;
      free(pRtp->pData);
      free(pRtp);
   }
}

VoipNode::VoipNode(ConfClient *pCC)
{
   mCC = pCC;
   NodeName = strdup(pCC->Callsign);
   EncodedBytes = 0;
   bRxActive = FALSE;
   DtmfCTCSSMode = 0;
   hGSM_Encode = NULL;
   hG726_Encode = NULL;
   RtpTxSequence = 0;
   TxSequence = 0;
   pSources = NULL;
   pInHead = NULL;
   AudioQCount = 0;
   TxAudioBuf = NULL;
   TxBufSize = 0;
   AudioAvailable = 0;
   TxCtcssFilter = 1;   // enable CTCSS filter by default for VoIP nodes
   TxStartTime.tv_sec = 0;
   TxPackets = 0;
   DisableWelcome = 0;
   bTxKeyed = FALSE;
   WelcomeFile = NULL;
   bDisconnectAfterWelcome = FALSE;
   ConnectScript = NULL;
   ConnectAnnouncements = 0;
   memset(&DummyLink,0,sizeof(LinkedNode));
   DummyLink.pTx = this;
   DummyLink.Flags = LNK_FLG_DUMMY;
   avl_insert(NodeTree,this);
}

void VoipSink::QueueAudio(NetAudio *pAudio) 
{
   if(pAudio != NULL) {
      if(pParent->DebugFpAudioIn != NULL) {
         fwrite(pAudio->Buf,pAudio->Available*sizeof(int16),1,
                pParent->DebugFpAudioIn);
      }

      if(pParent->MinMaxSamples < 8000) {
         pParent->LevelMonitor((int16 *) pAudio->Buf,pAudio->Available);
      }

      if(pParent->Transmitters > 0 || pParent->DtmfMethod == 3) {
         AudioQCount++;
         if(SendingHost[0] == 0) {
            ALOG(("%s ssrc 0x%x Queuing SN %d QCount %d samples %d\n",NodeName,
                  ssrc,pAudio->BufSN,AudioQCount,pAudio->Available));
         }
         else {
            ALOG(("%s Host %s Queuing SN %d QCount: %d samples: %d\n",
                  NodeName,SendingHost,pAudio->BufSN,AudioQCount,
                  pAudio->Available));
         }
         if(pAudio->Used != 0) {
            CORE_DUMP();
         }
         pAudio->Link = NULL;
         if(pInHead == NULL) {
            pInHead = pAudio;
         }
         else {
            pInTail->Link = pAudio;
         }
         pInTail = pAudio;
      }
      else {
      // If a tree falls in the forest and nobody is listening ...
         ALOG(("%s No transmitters, freeing SN %d, QCount: %d\n",
               NodeName,pAudio->BufSN,AudioQCount));
         free(pAudio);
      }
   }
}

NetAudio *VoipNode::AllocAudioBuf(int Bytes)
{
   NetAudio *pAudio;
    
   pAudio = (NetAudio *) malloc(sizeof(NetAudio) + Bytes);
   if(pAudio != NULL) {
      pAudio->Link = NULL;
      pAudio->Used = 0;
      pAudio->Flags = 0;
      pAudio->BufSN = AudioBufSn++;
   }
   return pAudio;
}

void VoipSink::DecompressGSM(gsm_byte *pGSM,int Len,gsm gsmh)
{
   int i;
   int GsmFrames = 0;
   NetAudio *pAudio = NULL;
   int AudioSamples = 0;
   gsm_signal *pDecoded;
   
   GsmFrames = Len / sizeof(gsm_frame);
   AudioSamples = GsmFrames * AUDIO_SAMPLES_PER_GSM_FRAME;

   if(AudioSamples > 0) {
      pAudio = VoipNode::AllocAudioBuf(AudioSamples * sizeof(int16));
      if(pAudio == NULL) {
         LOG_ERROR(("%s: AllocAudioBuf failed.\n",__FUNCTION__));
      }
      else {
         pAudio->Available = AudioSamples;
         pDecoded = (gsm_signal *) pAudio->Buf;
         for(i = 0; i < GsmFrames; i++) {
            gsm_decode(gsmh,pGSM,pDecoded);
            pGSM += sizeof(gsm_frame);
            pDecoded += AUDIO_SAMPLES_PER_GSM_FRAME;
         }
         QueueAudio(pAudio);
      }
   }
}

void VoipSink::DecompressULaw(char *p,int AudioSamples)
{
   NetAudio *pAudio = NULL;

   if(AudioSamples > 0) {
      pAudio = VoipNode::AllocAudioBuf(AudioSamples * sizeof(int16));
      if(pAudio == NULL) {
         LOG_ERROR(("%s: AllocAudioBuf failed.\n",__FUNCTION__));
      }
      else {
         pAudio->Available = AudioSamples;
         int16 *sp = (int16 *) pAudio->Buf;

         for(int i = 0; i < AudioSamples; i++) {
            *sp++ = audio_u2s(*p++);
         }
         QueueAudio(pAudio);
      }
   }
}

void VoipSink::DecompressADPCM(char *p,int Len,struct adpcm_state *pState)
{
   int AudioSamples = Len * 2;
   NetAudio *pAudio = NULL;

   if(AudioSamples > 0) {
      pAudio = VoipNode::AllocAudioBuf(AudioSamples * sizeof(int16));
      if(pAudio == NULL) {
         LOG_ERROR(("%s: AllocAudioBuf failed.\n",__FUNCTION__));
      }
      else {
         pAudio->Available = AudioSamples;
         adpcm_decoder(p,(short *)pAudio->Buf,AudioSamples,pState);
         QueueAudio(pAudio);
      }
   }
}

void VoipSink::DecompressG726(uint8_t *p,int Len)
{
   int AudioSamples = Len * 2;
   NetAudio *pAudio = NULL;

   if(AudioSamples > 0) {
      pAudio = VoipNode::AllocAudioBuf(AudioSamples * sizeof(int16));
      if(pAudio == NULL) {
         LOG_ERROR(("%s: AllocAudioBuf failed.\n",__FUNCTION__));
      }
      else {
         pAudio->Available = AudioSamples;
         g726_decode(g726h,(int16_t *)pAudio->Buf,p,Len);
         QueueAudio(pAudio);
      }
   }
}


// decompress as many rtp packets as possible
void VoipSink::DecompressRTP()
{
   RtpData *pRtp = pRtpQueue;
   RtpData *pNext;
   gsm_byte *pGSM;
   int DataLen;
   char *dp;
   struct adpcm_state adpcm;      /* ADPCM compression state */
   u_int8 *sp;
   
   while(pRtp != NULL && NextSeq == pRtp->pData->seq) {
      NextSeq++;
      pRtpQueue = pRtp->Link;
      RtpQCount--;
      SLOG(("%s: Decompressing %d\n",__FUNCTION__,pRtp->pData->seq));
      switch(pRtp->pData->pt) {
         case RTP_PT_PCMU:
            dp = (char *) pRtp->pData + RTP_HDR_SIZE;
            DataLen = pRtp->Len - RTP_HDR_SIZE;
            DecompressULaw(dp,DataLen);
            break;

         case RTP_PT_GSM:
            if(gsmh == NULL) {
               gsmh = gsm_create();
            }
            pGSM = (gsm_byte *) pRtp->pData + RTP_HDR_SIZE;
            DataLen = pRtp->Len - RTP_HDR_SIZE;
            DecompressGSM(pGSM,DataLen,gsmh);
            break;

         case RTP_PT_DVI4_8K:
            sp = (u_int8 *) pRtp->pData + RTP_HDR_SIZE;
            dp = (char *) sp  + 4;
            DataLen = pRtp->Len - RTP_HDR_SIZE - 4;
            adpcm.valprev = (sp[0] << 8) | sp[1];
            adpcm.index = sp[2];
            DecompressADPCM(dp,DataLen,&adpcm);
            break;

         case RTP_PT_G726:
            if(g726h == NULL) {
               g726h = g726_init(NULL,32000,G726_ENCODING_LINEAR,
                                 G726_PACKING_RIGHT);
            }
            sp = (uint8_t *) pRtp->pData + RTP_HDR_SIZE;
            DataLen = pRtp->Len - RTP_HDR_SIZE;
            DecompressG726(sp,DataLen);
            break;

         default:
            break;
      }

      pNext = pRtp->Link;
      free(pRtp->pData);
      free(pRtp);
      pRtp = pNext;
   }
}

void VoipSink::DecompressAudio()
{
   if(mCC->Proto == PROTO_ILINK || mCC->Proto == PROTO_RTP) {
      DecompressRTP();
   }
   else if(mCC->Proto == PROTO_SF) {
   // Speak Freely doesn't have sequence numbers so any received 
   // data has already been decompressed.  
   // No need to do anything here.
   }
   else {
   // This shouldn't happen !
      CORE_DUMP();
   }
}


// Called when VoIP stream stops transmitting
void VoipSink::StopTalking()
{
   RtpData *pRtp;
   u_int16 NextSeqAvailable;
   
   if(pRtpQueue != NULL) {
   // flush everything that's left to audio
      while((pRtp = pRtpQueue) != NULL) {
      // There's uncompressed RTP data available
         NextSeqAvailable = pRtp->pData->seq;
         while(NextSeq != NextSeqAvailable) {
            SLOG(("%s: Skipping missed seq %d\n",__FUNCTION__,NextSeq));
            D2PRINTF(("m1 %d",NextSeq));
            NextSeq++;
            MissedPackets++;
         }
         D2PRINTF(("\n"));
      // Make some audio
         DecompressAudio();
      }
   }

// Back to idle state
   bRxIdle = TRUE;
   bRxBuffering = bRxActive = FALSE;
}


void VoipNode::StopTalking()
{
   VoipSink *pSrc = pSources;

   while(pSrc != NULL) {
      pSrc->StopTalking();
      pSrc = pSrc->Link;
   }
}

void VoipSink::InsertInRTPQ(RtpData *pRtpData,int MaxNetQueueLen)
{
   RtpData *pLast = pRtpQueue;
   u_int16 Seq = pRtpData->pData->seq;
   
   if(pLast == NULL) {
   // The Queue is empty
      pLast = (RtpData *) &pRtpQueue;
      SLOG(("%s: Adding %d to empty Queue\n",__FUNCTION__,Seq));
   }
   else {
      RtpData *p = pLast->Link;

      if(p == NULL) {
      // only one packet on the queue
         if(Seq == pLast->pData->seq) {
            SLOG(("%s#%d: tossing duplicate %d\n",__FUNCTION__,__LINE__,Seq));
            DuplicatePackets++;
            free(pRtpData->pData);
            free(pRtpData);
            pLast = NULL;
         }
         else if(Between(Seq,pLast->pData->seq,pLast->pData->seq + MaxNetQueueLen)) {
         // this packet is now the first on the queue
            SLOG(("%s: inserted %d before %d\n",Seq,pLast->pData->seq));
            D2PRINTF(("\ns(%d,%d)",pLast->pData->seq,Seq));
            OutofSeqPackets++;
            pRtpData->Link = pRtpQueue;
            pLast = (RtpData *) &pRtpQueue;
         }
      }
      else  {
         while(p != NULL) {
            if(Seq == pLast->pData->seq || Seq == p->pData->seq) {
               SLOG(("%s#%d: tossing duplicate %d\n",__FUNCTION__,__LINE__,Seq));
               DuplicatePackets++;
               free(pRtpData->pData);
               free(pRtpData);
               pLast = NULL;
               break;
            }
            else if(Between(pLast->pData->seq,Seq,p->pData->seq)) {
            // insert new packet here
               pRtpData->Link = p;
               OutofSeqPackets++;
               SLOG(("%s: inserting %d between %d and %d\n",__FUNCTION__,
                     pLast->pData->seq,Seq,p->pData->seq));
               D2PRINTF(("\ns(%d,%d,%d)",pLast->pData->seq,Seq,p->pData->seq));
               break;
            }
            pLast = p;
            p = p->Link;
         }
      }

      if(pLast != NULL) {
         SLOG(("%s: Inserted %d, RtpQCount %d\n",__FUNCTION__,Seq,RtpQCount));
      }
   }

   if(pLast != NULL) {
      D2PRINTF(("q"));
      pLast->Link = pRtpData;
      RtpQCount++;
   }
}

void VoipNode::ConsumeAudio(NetAudio *pAudio)
{
   if(pAudio->Users > 0) {
      pAudio->Users--;
   }
   else {
      CORE_DUMP();
   }
}

int VoipNode::DeleteTx(LinkedNode *pDeleteLink)
{
// Decrement the usage count for all buffers this link hasn't used yet
   NetAudio *pAudio = pDeleteLink->pInHead;
   NetAudio *pNextAudio;
   while(pAudio != NULL) {
      pNextAudio = pAudio->Link;
      if(pAudio->BufNum > pDeleteLink->BufNum) {
         ConsumeAudio(pAudio);
      }
      pAudio = pNextAudio;
   }
   return Node::DeleteTx(pDeleteLink);
}

void VoipNode::AddTx(LinkedNode *pLink)
{
   Node::AddTx(pLink);

// Increment the usage count for all queued buffers
   NetAudio *pAudio = pLink->pInHead = pInHead;
   while(pAudio != NULL) {
      pAudio->Users++;
      pAudio = pAudio->Link;
   }
}

// Advance the Timer by the amount of time it will take to consume
// the specified number of samples @ 8 Khz
void Add2Timer(struct timeval *pTimer,int AudioSamples)
{
   int Duration = AudioSamples / 8;    // in milliseconds

   int microseconds = pTimer->tv_usec + ((Duration % 1000) * 1000);
   pTimer->tv_usec = microseconds % 1000000;
   pTimer->tv_sec  = pTimer->tv_sec + (Duration / 1000) + 
                     microseconds / 1000000;
}

// returns number of samples available
int VoipNode::SamplesAvailable(LinkedNode *pLink,int bForce)
{
   NetAudio *pAudio = NULL;
   int Ret = 0;

   pAudio = pLink->pInHead;
   if(pAudio == NULL && pInHead != NULL && pLink->BufNum < BufNum) {
      pAudio = pInHead;
      while(pAudio->BufNum <= pLink->BufNum) {
      // We've already consumed this buffer, try the next one
         ALOG(("%s Skipping BufNum %d already consumed by %s\n",
               NodeName,pAudio->BufNum,pLink->pTx->NodeName));

         if((pAudio = pAudio->Link) == NULL) {
            break;
         }
      }

      if(pAudio != NULL) {
         pLink->pInHead = pAudio;
         pLink->Used = 0;
         pLink->BufNum = pAudio->BufNum;
      }
   }

   if(pAudio == NULL) {
   // Need more audio
      VoipSink *pSrc = pSources;
      int Available;
      int MinAvailable = 0x7fffffff;

      while(pSrc != NULL) {
         if((Available = pSrc->AudioAvailable(bForce)) > 0) {
         // This source has audio
            if(MinAvailable > Available) {
               MinAvailable = Available;
               Ret = MinAvailable;
            }
         }
         pSrc = pSrc->Link;
      }
   }
   else while(pAudio != NULL) {
      Ret += pAudio->Available - pLink->Used;
      pAudio = pAudio->Link;
   }

// return number of samples available
   return Ret;
}

// returns number of samples available @ AudioOut
// if < 0 then samples are 32 bits otherwise they are 16 bit.
// The number of samples will be <= AUDIO_BUF_SIZE
int VoipNode::GetRxAudio(
   LinkedNode *pLink,
   void **AudioOut,
   int MaxSamples,
   int bForce)
{
   NetAudio *pAudio = NULL;
   int ActiveSources = 0;
   NetAudio *pNewAudioBuf = NULL;
   int Ret = 0;
   int bWasActive = bRxActive;

// Free any empty audio buffers

   while((pAudio = pInHead) != NULL) {
      if(pAudio->Users <= 0) {
         AudioQCount--;
         ALOG(("%s Freeing BufSN %d, BufNum %d, AudioQCount: %d\n",NodeName,
               pAudio->BufSN,pAudio->BufNum,AudioQCount));
         pInHead = pAudio->Link;
         free(pAudio);
      }
      else {
         break;
      }
   }

   pAudio = pLink->pInHead;
   if(pAudio == NULL && pInHead != NULL && pLink->BufNum < BufNum) {
      pAudio = pInHead;
      while(pAudio->BufNum <= pLink->BufNum) {
      // We've already consumed this buffer, try the next one
         ALOG(("%s Skipping BufNum %d already consumed by %s\n",
               NodeName,pAudio->BufNum,pLink->pTx->NodeName));

         if((pAudio = pAudio->Link) == NULL) {
            break;
         }
      }

      if(pAudio != NULL) {
         pLink->pInHead = pAudio;
         pLink->Used = 0;
         pLink->BufNum = pAudio->BufNum;
      }
   }

   if(pAudio == NULL) {
   // Need more audio
      VoipSink *pSrc = pSources;
      VoipSink *pFirstSrc = NULL;
      VoipSink *pLastSrc = NULL;
      VoipSink *pNextSrc;
      ActiveSources = 0;
      int Available;
      int MinAvailable = 0x7fffffff;

      while(pSrc != NULL) {
         pNextSrc = pSrc->Link;
         if((Available = pSrc->AudioAvailable(bForce)) > 0) {
         // This source has audio
            ActiveSources++;
            pFirstSrc = pSrc;
            if(MinAvailable > Available) {
               MinAvailable = Available;
            }
         }
         else {
         // This source doesn't have any audio
            if(TimeLapse(&pSrc->LastRx) > 30000) {
            // Haven't heard from this guy in 30 seconds, delete him
               if(pLastSrc == NULL) {
                  pSources = pNextSrc;
               }
               else {
                  pLastSrc->Link = pNextSrc;
               }
               delete pSrc;
            }
            else {
               pLastSrc = pSrc;
            }
         }
         pSrc = pNextSrc;
      }

      if(ActiveSources == 1) {
      // Single source, just use it's buffer directly
         pNewAudioBuf = pFirstSrc->GetAudioBuf();
         ALOG(("%s 1 source took SN %d samples %d\n",NodeName,
               pNewAudioBuf->BufSN,pNewAudioBuf->Available));
         if(DtmfMethod == 3) {
            dtmf_rx(&DtmfRxState,(const int16_t *) pNewAudioBuf->Buf,
                    pNewAudioBuf->Available);
         }
      }
      else if(ActiveSources > 1) do {
      // multiple sources, we need to mix
         int Samples;
         int BufLen = MinAvailable * sizeof(int);

         pNewAudioBuf = AllocAudioBuf(BufLen);

         if(pNewAudioBuf == NULL) {
            LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
            break;
         }
         ALOG(("%s allocated %d byte SN %d for 32bit data\n",NodeName,BufLen,
               pNewAudioBuf->BufSN));
         pNewAudioBuf->Flags |= AUDIO_FLAG_32BIT;
         pNewAudioBuf->Available = MinAvailable;
      // Zero the buffer
         memset(pNewAudioBuf->Buf,0,BufLen);

         VoipSink *pSrc = pSources;

         while(pSrc != NULL) {
            int *Out;
            int16 *In;
            if((pAudio = pSrc->pInHead) != NULL) {
               Samples = pAudio->Available - pAudio->Used;
               if(Samples > MinAvailable) {
                  Samples = MinAvailable;
               }
               pAudio->Used += Samples;
               ALOG(("%s used %d samples from SN %d, %d left\n",NodeName,
                     Samples,pNewAudioBuf->BufSN,
                     pAudio->Available - pAudio->Used));
   
               Out = (int *) pNewAudioBuf->Buf;
               In = (int16 *) pAudio->Buf;
   
               while(Samples--) {
                  *Out++ += *In++;
               }
   
               if(pAudio->Used == pAudio->Available) {
               // The buffer is empty
                  pSrc->pInHead = pAudio->Link;
                  free(pAudio);
               }
            }
            pSrc = pSrc->Link;
         }

         if(DtmfMethod == 3) {
         // Convert buffer to 16 bit for dtmf_rx
            int16 *TempBuf = (int16 *) malloc(MinAvailable * sizeof(int16));
            int16 *Out = TempBuf;
            int *In = (int *) pNewAudioBuf->Buf;

            if(TempBuf == NULL) {
               LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
               break;
            }
            Samples = MinAvailable;
            while(Samples--) {
               *Out++ = (int16) *In++;
            }

            dtmf_rx(&DtmfRxState,TempBuf,MinAvailable);
            free(TempBuf);
         }
      } while(FALSE);

      if((pAudio = pNewAudioBuf) != NULL) {
         pLink->pInHead = pAudio;
         pAudio->Link = NULL;
         pAudio->BufNum = BufNum++;
         pAudio->Users = Transmitters;
         if(pLink->Flags & LNK_FLG_DUMMY) {
         // Special case for PollTx calling for audio to keep DTMF rx
         // going...
            pAudio->Users++;
            if(pAudio->Users != 1) {
               CORE_DUMP();
            }
         }

         if(pInHead == NULL) {
            pInHead = pAudio;
         }
         else {
            pInTail->Link = pAudio;
         }
         pInTail = pAudio;
         AudioQCount++;
      // Update the CtoneTimer to go off after the audio we just queued
      // is consumed
         if(CtoneTimer.tv_sec == 0) {
         // start the CtoneTimer
            CtoneTimer = TimeNow;
         }
         Add2Timer(&CtoneTimer,pAudio->Available - pAudio->Used);
      }
   }
   
   if(pAudio != NULL) {
      ALOG(("%s using SN %d available %d used %d\n",NodeName,pAudio->BufSN,
            pAudio->Available,pLink->Used));
      Ret = min(MaxSamples,pAudio->Available - pLink->Used);
      if(pAudio->Flags & AUDIO_FLAG_32BIT) {
      // Buffer contains 32 bit audio
         *AudioOut = pAudio->Buf + (pLink->Used * sizeof(int));
         pLink->Used += Ret;
      // Return negative number of samples for 32 bit samples
         Ret = -Ret;
      }
      else {
      // Buffer contains 16 bit audio
         *AudioOut = pAudio->Buf + (pLink->Used * sizeof(int16));
         pLink->Used += Ret;
      }

      if(pLink->Used == pAudio->Available) {
      // Finished with this buffer
         pLink->pInHead = pAudio->Link;
         pLink->BufNum = pAudio->BufNum;
         pLink->Used = 0;
         ALOG(("%s %s consumed all of SN %d BufNum %d\n",NodeName,
               pLink->pTx->NodeName,pAudio->BufSN,pAudio->BufNum));
         ConsumeAudio(pAudio);
      }
   }

   if(Ret == 0) {
      if(CtoneTimer.tv_sec != 0) {
      // No audio was available, check if it's time to generate a CTone
         int TimeLapsed = TimeLapse(&CtoneTimer);
         if(TimeLapsed >= 0) {
            bRxActive = FALSE;
            if(Ctone == 0) {
               CtoneTimer.tv_sec = 0;
            }
            if(DTMF_Digits > 0) {
            // There's something in the DTMF buffer
               if(DTMFPrefix == NULL || bPrefixed) {
                  DLOG(DLOG_DTMF,("%s: Calling DecodeDTMF, node: %s\n",
                                  __FUNCTION__,NodeName));
                  DecodeDTMF(DTMF_Buf);
               }
            }
         }
         
         if(Ctone != 0) {
            if(TimeLapsed >= CtoneDelay) {
               CtoneTimer.tv_sec = 0;
               SendCtone(TRUE);
            }
         }
         if(!bRxActive && DTMF_Digits) {
            DTMF_Digits = 0;
            LastDTMFDigit.tv_sec = 0;
         }
      }
   }
   else {
      bRxActive = TRUE;
   }

   if(bWasActive != bRxActive) {
      DPRINTF(("Port %s: COS %sactive\n",NodeName,bRxActive ? "" : "in"));
   }

// return number of samples available
   return Ret;
}

NetAudio *VoipSink::GetAudioBuf()
{
   RtpData *pRtp;
   u_int16 NextSeqAvailable;
   NetAudio *pAudio = NULL;

   if(pInHead != NULL) {
      pAudio = pInHead;
      pInHead = pAudio->Link;
      AudioQCount--;
   }
   else if(bRxActive && (pRtp = pRtpQueue) != NULL) {
   // We're not buffering and there's uncompressed RTP data available
      NextSeqAvailable = pRtp->pData->seq;
      while(NextSeq != NextSeqAvailable) {
         D2PRINTF(("m2 %d ",NextSeq));
         NextSeq++;
         MissedPackets++;
      }
      D2PRINTF(("\n"));
   // Try to decompress some audio
      ALOG(("%s Missed packet\n",NodeName));
      DecompressAudio();
      if(pInHead != NULL) {
         pAudio = pInHead;
         pInHead = pAudio->Link;
         AudioQCount--;
      }
      else {
      // This function should never be called unless audio is available
         CORE_DUMP();
      }
   }
   else {
   // This function should never be called unless audio is available
      CORE_DUMP();
   }

   return pAudio;
}

// return the number of samples of audio available
// if bForce is true then give up waiting for any out of sequence packets
// to arrive
int VoipSink::AudioAvailable(int bForce)
{
   RtpData *pRtp;
   u_int16 NextSeqAvailable;
   int Ret = 0;

   if(!bRxBuffering) {
      if(pInHead == NULL && bForce && (pRtp = pRtpQueue) != NULL) {
      // No audio is available and we're not buffering and there's 
      // uncompressed RTP data available
         NextSeqAvailable = pRtp->pData->seq;
         while(NextSeq != NextSeqAvailable) {
            SLOG(("%s: Skipping missed seq %d\n",__FUNCTION__,NextSeq));
            NextSeq++;
            MissedPackets++;
         }
      // Try to decompress some audio
         DecompressAudio();
      }

      if(pInHead != NULL) {
         Ret = pInHead->Available - pInHead->Used;
      }
   }

   if(bForce && Ret == 0) {
   // Back to buffering, we're empty
      if(bRxActive) {
         bRxActive = FALSE;
         bRxBuffering = TRUE;
         bRxIdle = FALSE;
      }
   }

   return Ret;
}

VoipSink *VoipNode::FindSink(u_int32 ssrc,const char *SendingHost)
{
   VoipSink *pSrc = pSources;
   VoipSink *pLastSrc = NULL;
   VoipSink *pNextSrc;

   while(pSrc != NULL) {
      if(SendingHost == NULL) {
         if(pSrc->ssrc == ssrc) {
         // This is the droid we're looking for
            break;
         }
      }
      else {
         if(strcmp(pSrc->SendingHost,SendingHost) == 0) {
            break;
         }
      }
   // This is not the droid we're looking for 
      pNextSrc = pSrc->Link;

      if(TimeLapse(&pSrc->LastRx) > 30000) {
      // Haven't heard from this guy in 30 seconds, delete him
         if(SendingHost == NULL) {
            SLOG(("%s, deleting VoipSink for ssrc 0x%x\n",NodeName,ssrc));
         }
         else {
            SLOG(("%s, deleting VoipSink for host (%s)\n",NodeName,SendingHost));
         }

         if(pLastSrc == NULL) {
            pSources = pNextSrc;
         }
         else {
            pLastSrc->Link = pNextSrc;
         }
         delete pSrc;
      }
      else {
         pLastSrc = pSrc;
      }
      pSrc = pNextSrc;
   }

   if(pSrc == NULL) {
      pSrc = new VoipSink(this);
      if(pSrc != NULL) {
         pSrc->Link = pSources;
         pSources = pSrc;

         if(SendingHost == NULL) {
            pSrc->ssrc = ssrc;
            SLOG(("%s, created new VoipSink for ssrc 0x%x\n",NodeName,ssrc));
         }
         else {
            strcpy(pSrc->SendingHost,SendingHost);
            SLOG(("%s, created new VoipSink for host (%s)\n",NodeName,SendingHost));
         }
      }
      else {
         LOG_ERROR(("%s: new failed\n",__FUNCTION__));
      }
   }

   return pSrc;
}

void VoipNode::QueueRTPData(RtpData *pRtp)
{
   VoipSink *pSrc = FindSink(pRtp->pData->ssrc,NULL);

   if(pSrc != NULL) {
      pSrc->LastRx = TimeNow;
      pSrc->QueueRTPData(pRtp);
   }
}

//
// RxState  
void VoipSink::QueueRTPData(RtpData *pRtp)
{
   u_int16 Seq = pRtp->pData->seq;
   int NetQueueLen = 0;
   int MaxNetQueueLen;

   switch(pRtp->pData->pt) {
      case RTP_PT_GSM:
         NetQueueLen = VoipDejitterDelay / 80;
         break;

      case RTP_PT_DVI4_8K:
         NetQueueLen = VoipDejitterDelay / 122;
         break;

      case RTP_PT_PCMU:
         NetQueueLen = VoipDejitterDelay / 31;
         break;

      case RTP_PT_G726:
         NetQueueLen = VoipDejitterDelay / 25;
         break;

      default:
         break;
   }
   MaxNetQueueLen = NetQueueLen * 2;

   if(bRxIdle) {
   // First packet received, assume this is the first sequence #
      SLOG(("%s: set NextSeq to %d\n",__FUNCTION__,NextSeq));
      D2PRINTF(("t%d",Seq));
      NextSeq = Seq;
      bRxIdle = FALSE;
      bRxBuffering = TRUE;
   }

   if(bRxBuffering) {
      if(Between(Seq,NextSeq,Seq + MaxNetQueueLen)) {
      // This packet is before the current NextSeq, backup NextSeq
         NextSeq = Seq;
         SLOG(("%s: backup NextSeq to %d\n",__FUNCTION__,NextSeq));
      }
   }

   if(Between(NextSeq,Seq,NextSeq + MaxNetQueueLen)) {
      InsertInRTPQ(pRtp,MaxNetQueueLen);
      if(bRxBuffering) {
         if(RtpQCount >= NetQueueLen) {
         // We're buffering and it's time to rock and roll
            D2PRINTF(("S"));
            bRxBuffering = FALSE;
            bRxActive = TRUE;
            DecompressAudio();
         }
      }
      else if(bRxActive) {
      // try to make some more audio
         DecompressAudio();
      }
      else {
      // this shouldn't happen
         CORE_DUMP();
      }
   }
   else {
   // This packet is out of range, must have been delayed too long,
   // toss it
      SLOG(("%s: tossing %d, NextSeq: %d, NextSeq+MaxNetQueueLen: %d\n",
            __FUNCTION__,Seq,NextSeq,NextSeq+MaxNetQueueLen));
      LatePackets++;
      free(pRtp->pData);
      free(pRtp);
   }
}


void VoipSink::QueueSFData(soundbuf *p)
{
   int compression = (int) htonl(p->compression);
   char *pData;
   int Len = htonl(p->buffer.buffer_len);
   struct adpcm_state adpcm;      /* ADPCM compression state */
   u_int8 *sp;
   int NetQueueLen = 0;

   if(bRxIdle) {
   // First packet received, assume this is the first sequence #
      D2PRINTF(("t"));
      bRxIdle = FALSE;
      bRxBuffering = TRUE;
   }

// Speak Freely doesn't have sequence numbers so decompress data now
   switch(compression) {
      case fProtocol:
         NetQueueLen = VoipDejitterDelay / (Len/8);
         DecompressULaw(p->buffer.buffer_val,Len);
         break;

      case fCompGSM | fProtocol:
         if(gsmh == NULL) {
            gsmh = gsm_create();
         }
         Len -= 2;
         pData = &p->buffer.buffer_val[2];
         NetQueueLen = VoipDejitterDelay / 200;
         DecompressGSM((gsm_byte *) pData,Len,gsmh);
         break;

      case fCompADPCM | fProtocol:
       // Restore the decoder state from the copy saved in the 
       // packet, independent of the byte order of the machine 
       // we're running on. 
         pData = p->buffer.buffer_val;
         Len -= 3;
         sp = (u_int8 *) &pData[Len];
         adpcm.valprev = (sp[0] << 8) | sp[1];
         adpcm.index = sp[2];
         NetQueueLen = VoipDejitterDelay / 122;
         DecompressADPCM(pData,Len,&adpcm);
         break;

      default:
         CORE_DUMP();
         break;
   }

   if(bRxBuffering) {
      if(AudioQCount >= NetQueueLen) {
      // We're buffering and it's time to rock and roll
         if(SendingHost[0] == 0) {
            ALOG(("%s, ssrc 0x%x AudioQCount: %d >= NetQueueLen: %d\n",
                  NodeName,ssrc,AudioQCount,NetQueueLen));
         }
         else {
            ALOG(("%s, SendingHost %s AudioQCount: %d >= NetQueueLen: %d (%d)\n",
                  NodeName,SendingHost,AudioQCount,NetQueueLen,Len));
         }
         D2PRINTF(("S"));
         bRxBuffering = FALSE;
         bRxActive = TRUE;
      }
      else {
         D2PRINTF(("q"));
      }
   }
   else if(!bRxActive) {
   // this shouldn't happen
      CORE_DUMP();
   }
}

void VoipNode::QueueSFData(soundbuf *p)
{
   VoipSink *pSrc = FindSink(0,p->sendinghost);

   if(pSrc != NULL) {
      pSrc->LastRx = TimeNow;
      pSrc->QueueSFData(p);
   }
}

void EndPointCleanup(ConfClient *pCC)
{
   VoipNode *pVoipNode = (VoipNode *) pCC->pVoipNode;
   if(pVoipNode != NULL) {
      delete pVoipNode;
   }
}

VoipNode::~VoipNode()
{
   AnnounceDisconnection();
   if(avl_delete(NodeTree,this) == NULL) {
      LOG_ERROR(("%s: avl_delete() failed to find me.\n",__FUNCTION__));
   }

   if(TxAudioBuf != NULL) {
      free(TxAudioBuf);
   }

   if(hGSM_Encode != NULL) {
      gsm_destroy(hGSM_Encode);
   }

   if(hG726_Encode != NULL) {
      g726_release(hG726_Encode);
   }
}

void VoipNode::AnnounceDisconnection()
{
   if(!mCC->bFilePlayer && mCC->bConnected) {
   // sent a disconnect message to any ports we're linked to 
      LinkedNode *pLink = pTransmitters;

      mCC->bConnected = FALSE;
      ConferenceClients--;

      while(pLink != NULL) {
         if(pLink->pTx->ConnectAnnouncements && 
            (!mCC->bIRLP || pLink->pTx->IRLPAnnouncements))
         {
            pLink->pTx->SayPhrase(NodeName,SAY_FLAG_CALLSIGN);
            pLink->pTx->SayPhrase("disconnected");
         }
         pLink = pLink->pNextTx;
      }
   }
}

int VoipNode::WriteAudioOut(ClientInfo *p)
{
   LOG_ERROR(("%s: I should never be called!.\n",__FUNCTION__));
   CORE_DUMP();
   return 0;
}

void VoipNode::GsmTx()
{
   gsm_byte *pGSM;
   int i;
   rtp_hdr_t *pRtpHdr;

   pGSM = (gsm_byte *) &GSM_Packet[SIZEOF_SOUNDBUF+2];

// Compress the audio

   for(i = 0; i < SamplesPerPacket; i += AUDIO_SAMPLES_PER_GSM_FRAME) {
      gsm_encode(hGSM_Encode,&TxAudioBuf[i],pGSM);
      pGSM += sizeof(gsm_frame);
   }
   pGSM = (gsm_byte *) &GSM_Packet[SIZEOF_SOUNDBUF+2];

   if(mCC->Proto == PROTO_RTP || mCC->Proto == PROTO_ILINK) {
   // Send Echolink or RTP packets

      pRtpHdr = (rtp_hdr_t *) (((char *) pGSM) - RTP_HDR_SIZE);
      memset(pRtpHdr,0,RTP_HDR_SIZE);
      pRtpHdr->pt = RTP_PT_GSM;
      pRtpHdr->seq = htons(TxSequence++);
      pRtpHdr->ssrc = OurNodeID;

      pRtpHdr->version = mCC->Proto == PROTO_ILINK ? ILINK_RTP_VERSION : 2;
      SendAudioPacket(pRtpHdr,ILINK_RTP_FRAME_LEN);
   }
   else if(mCC->Proto == PROTO_SF) {
   // Send SF frames
      do {
      // Fill in the Send Speak Freely header
         soundbuf *pSFHdr = (soundbuf *) GSM_Packet;
         int TxLen;
   
      // Hide the original number of samples in first 2 bytes of buffer.
         *((short *) pSFHdr->buffer.buffer_val) = 
                                    htons(AUDIO_SAMPLES_PER_GSM_FRAME*10);
   
         pSFHdr->compression = (int) htonl(fCompGSM | fProtocol);
         pSFHdr->buffer.buffer_len = (int) htonl(2 + (10*GSM_FRAME_LEN));
         TxLen = SIZEOF_SOUNDBUF + 2 + (10*GSM_FRAME_LEN);
   
         memset(pSFHdr->sendinghost,0,sizeof(pSFHdr->sendinghost));
         strncpy(pSFHdr->sendinghost,ConferenceCall,sizeof(pSFHdr->sendinghost));
   
      // Send Speak Freely ADPCM packets
   
         SendAudioPacket(pSFHdr,TxLen);
   
      } while(FALSE);
   }
   else {
      LOG_ERROR(("%s: Error protocol = %d.\n",__FUNCTION__,mCC->Proto));
   }
}

void VoipNode::SendAudioPacket(void *Buf,int TxLen)
{
#if 0
   if(ConfEnable && mCC->bInConf) {
   // Conferencing is enabled and this client is in the conference,
   // let the conference code send the data
   }
   else {
#endif
   // Just send it to him directly
      Send2(mCC,Buf,TxLen,FALSE);
// }
}

void VoipNode::AdpcmTx()
{
   adpcm_coder(TxAudioBuf,Adpcm.Buf,AUDIO_SAMPLES_PER_ADPCM_PACKET,&TxADPCM);

   if(mCC->Proto == PROTO_SF) {
   // Fill in the Send Speak Freely header
      soundbuf *pSFHdr = (soundbuf *) &Adpcm.Buf[-SIZEOF_SOUNDBUF];
      u_int8 *pData = (u_int8 *) Adpcm.Buf;
      u_int8 *sp = (u_int8 *) &pData[ADPCM_PACKET_DATA_BYTES];
      int TxLen = SIZEOF_SOUNDBUF + ADPCM_PACKET_DATA_BYTES + 3;

      pSFHdr->compression = (int) htonl(fCompADPCM| fProtocol);
      pSFHdr->buffer.buffer_len = (int) htonl(ADPCM_PACKET_DATA_BYTES+3);
      memset(pSFHdr->sendinghost,0,sizeof(pSFHdr->sendinghost));
      strncpy(pSFHdr->sendinghost,ConferenceCall,
              sizeof(pSFHdr->sendinghost));
      sp[0] = (u_int8) (((u_int16)TxADPCMStart.valprev) >> 8);
      sp[1] = (TxADPCMStart.valprev & 0xff);
      sp[2] = TxADPCMStart.index;

   // Send Speak Freely ADPCM packets
      SendAudioPacket(pSFHdr,TxLen);
   }
   else if(mCC->Proto == PROTO_RTP) {
   // Fill in the RTP header
      rtp_hdr_t *pRtpHdr = (rtp_hdr_t *) &Adpcm.Buf[-(RTP_HDR_SIZE+4)];
      u_int8 *sp = (u_int8 *) &Adpcm.Buf[-4];
      int TxLen = RTP_HDR_SIZE + ADPCM_PACKET_DATA_BYTES + 4;

      memset(pRtpHdr,0,sizeof(rtp_hdr_t));
      pRtpHdr->pt = RTP_PT_DVI4_8K;
      pRtpHdr->seq = htons(RtpTxSequence++);
      pRtpHdr->ssrc = OurNodeID;
      pRtpHdr->version = 2;
      sp[0] = (u_int8) (((u_int16)TxADPCMStart.valprev) >> 8);
      sp[1] = (TxADPCMStart.valprev & 0xff);
      sp[2] = TxADPCMStart.index;

   // Send RTP ADPCM packets
      SendAudioPacket(pRtpHdr,TxLen);
   }
   else {
      LOG_ERROR(("%s: Error protocol = %d.\n",__FUNCTION__,mCC->Proto));
   }
   TxADPCMStart = TxADPCM;
}

void VoipNode::UlawTx()
{
   char *cp = Adpcm.Buf;
   int16 *sp = TxAudioBuf;
   int TxSamples = mCC->Proto == PROTO_SF ? AUDIO_SAMPLES_PER_SF_ULAW_PKT :
                                            AUDIO_SAMPLES_PER_RTP_ULAW_PKT;

   for(int i = 0; i < TxSamples; i++) {
      *cp++ = audio_s2u(*sp++);
   }

   if(mCC->Proto == PROTO_SF) {
   // Fill in the Send Speak Freely header
      soundbuf *pSFHdr = (soundbuf *) &Adpcm.Buf[-SIZEOF_SOUNDBUF];

      pSFHdr->compression = (int) htonl(fProtocol);
      pSFHdr->buffer.buffer_len = (int) htonl(TxSamples);
      memset(pSFHdr->sendinghost,0,sizeof(pSFHdr->sendinghost));
      strncpy(pSFHdr->sendinghost,ConferenceCall,
              sizeof(pSFHdr->sendinghost));

   // Send Speak Freely uLaw packets
      SendAudioPacket(pSFHdr,SIZEOF_SOUNDBUF + TxSamples);
   }
   else if(mCC->Proto == PROTO_RTP) {
   // Fill in the RTP header
      rtp_hdr_t *pRtpHdr = (rtp_hdr_t *) &Adpcm.Buf[-RTP_HDR_SIZE];

      memset(pRtpHdr,0,RTP_HDR_SIZE);
      pRtpHdr->pt = RTP_PT_PCMU;
      pRtpHdr->seq = htons(RtpTxSequence++);
      pRtpHdr->ssrc = OurNodeID;
      pRtpHdr->version = 2;

   // Send RTP uLaw packets
      SendAudioPacket(pRtpHdr,RTP_HDR_SIZE + TxSamples);
   }
   else {
      LOG_ERROR(("%s: Error protocol = %d.\n",__FUNCTION__,mCC->Proto));
      CORE_DUMP();
   }
}

void VoipNode::G726Tx()  // 20101109 - NIL   Entire Subroutine replaced
{
   g726_encode(hG726_Encode,(uint8_t *)Adpcm.Buf,TxAudioBuf,
               AUDIO_SAMPLES_PER_G726_PACKET);
   if(mCC->Proto == PROTO_RTP) {
   // Fill in the RTP header
      rtp_hdr_t *pRtpHdr = (rtp_hdr_t *) &Adpcm.Buf[-RTP_HDR_SIZE];

      memset(pRtpHdr,0,RTP_HDR_SIZE);
      pRtpHdr->pt = RTP_PT_G726;
      pRtpHdr->seq = htons(RtpTxSequence++);
      pRtpHdr->ssrc = OurNodeID;
      pRtpHdr->version = 2;

   // Send RTP G726 packets
      SendAudioPacket(pRtpHdr,RTP_HDR_SIZE + G726_PACKET_DATA_BYTES);
   }
   else {
      LOG_ERROR(("%s: Error protocol = %d.\n",__FUNCTION__,mCC->Proto));
      CORE_DUMP();
   }
}

void VoipNode::PollTx()
{
   int NewSamples;
   int PacketDelay;
   int PacketsPending = 1;
   SamplesPerPacket = 0;

   do {
      switch(mCC->CompressionType) {
         case RTP_PT_DVI4_8K:
            SamplesPerPacket = AUDIO_SAMPLES_PER_ADPCM_PACKET;
            break;

         case RTP_PT_GSM:
            switch(mCC->Proto) {
               case PROTO_SF:
                  SamplesPerPacket = SF_AUDIO_SAMPLES_PER_PACKET;
                  break;

               case PROTO_RTP:
               case PROTO_ILINK:
                  SamplesPerPacket = ILINK_AUDIO_SAMPLES_PER_PACKET;
                  break;

               default:
                  LOG_ERROR(("%s: Error: Unsupported Proto for GSM (%d).\n",
                             __FUNCTION__,mCC->Proto));
                  CORE_DUMP();
                  break;
            }
            break;

         case RTP_PT_PCMU:
            switch(mCC->Proto) {
               case PROTO_SF:
                  SamplesPerPacket = AUDIO_SAMPLES_PER_SF_ULAW_PKT;
                  break;

               case PROTO_RTP:
                  SamplesPerPacket = AUDIO_SAMPLES_PER_RTP_ULAW_PKT;
                  break;

               default:
                  LOG_ERROR(("%s: Error: Unsupported Proto for GSM (%d).\n",
                             __FUNCTION__,mCC->Proto));
                  CORE_DUMP();
                  break;
            }
            break;

         case RTP_PT_G726:
            switch(mCC->Proto) {
               case PROTO_RTP:
                  SamplesPerPacket = AUDIO_SAMPLES_PER_G726_PACKET;
                  break;

               default:
                  LOG_ERROR(("%s: Error: Unsupported Proto for G.726 (%d).\n",
                             __FUNCTION__,mCC->Proto));
                  CORE_DUMP();
                  break;
            }
            break;


         default:
            LOG_ERROR(("%s: Unknown CompressionType %d.\n",__FUNCTION__,
                       mCC->CompressionType));
            break;
      }

      if(SamplesPerPacket == 0) {
      // Unknown CompressionType, bail
         break;
      }

      if(SamplesPerPacket * sizeof(int16) > TxBufSize) {
         int16 *Temp = TxAudioBuf;
         TxBufSize = SamplesPerPacket * sizeof(int16);
         if((TxAudioBuf = (int16 *) malloc(TxBufSize)) == NULL) {
            LOG_ERROR(("%s#%d: malloc failed.\n",__FUNCTION__,__LINE__));
            break;
         }

         if(Temp != NULL) {
         // Buffer size changed. This only occurs if the compression type 
         // changes on the fly.  Move the unused data down to the beginning 
         // of the new TxAudioBuf
            if(AudioAvailable > 0) {
               memcpy(TxAudioBuf,Temp,AudioAvailable * sizeof(int16));
            }
            free(Temp);
         }
      }

   // calculate delay between packets in milliseconconds
      PacketDelay = SamplesPerPacket * 1000 / 8000;  

      if(TxStartTime.tv_sec != 0) {
         PacketsPending = TimeLapse(&TxStartTime) / PacketDelay - TxPackets + 1;
      }

      while(PacketsPending > 0) {
         if(AudioAvailable < SamplesPerPacket) {
            NewSamples = GetTxAudio(&TxAudioBuf[AudioAvailable],
                                    SamplesPerPacket-AudioAvailable);

            if(NewSamples == 0) {
            // not enough audio is available to transmit yet
               if(PacketsPending > 2) {
               // if we get more than 2 packets behind we're not transmitting
               // any more, setup for the next transmission
                  TxPackets = 0;
                  TxStartTime.tv_sec = 0;
                  bTxKeyed = FALSE;
               }
               break;
            }
            AudioAvailable += NewSamples;
         }

         if(AudioAvailable >= SamplesPerPacket) {
            if(TxStartTime.tv_sec == 0) {
               TxStartTime = TimeNow;
               bTxKeyed = TRUE;
            }
            switch(mCC->CompressionType) {
               case RTP_PT_DVI4_8K:
                  AdpcmTx();
                  break;

               case RTP_PT_GSM:
                  if(hGSM_Encode == NULL) {
                     if((hGSM_Encode = gsm_create()) == NULL) {
                        LOG_ERROR(("%s: gsm_create failed.\n",__FUNCTION__));
                     }
                  }

                  if(hGSM_Encode != NULL) {
                     GsmTx();
                  }
                  break;

               case RTP_PT_PCMU:
                  UlawTx();
                  break;

               case RTP_PT_G726:
                  if(hG726_Encode == NULL) {
                     hG726_Encode = g726_init(NULL,32000,G726_ENCODING_LINEAR,
                                              G726_PACKING_RIGHT);
                  }
                  G726Tx();
                  break;

               default:
                  CORE_DUMP();
                  break;
            }

            TxPackets++;
            PacketsPending--;
            AudioAvailable -= SamplesPerPacket;
            if(AudioAvailable > 0) {
            // This only occurs if the compression type changes on the fly
            // move the unused data down to the beginning of the buffer
               memmove(TxAudioBuf,&TxAudioBuf[SamplesPerPacket],AudioAvailable);
            }
         }
      }
   } while(FALSE);

   if(Transmitters == 0 && DtmfMethod == 3) {
   // No one is listening and we're decoding DTMF.
   // Try to get some Rx audio so the DTMF receiver will be fed
      void *pAudio;
   // Just keep reading and tossing audio until none is left, GetRxAudio 
   // has already fed it to the DTMF decoder
      while(GetRxAudio(&DummyLink,&pAudio,102,FALSE) != 0);
   }

   CheckDtmfTimeouts();
}

extern struct config_entry VoipConfigVars[];

void VoipNode::VoipInit(ConfClient *pCC)
{
   Node *CurrentSave = CurrentNode; // save the current node pointer
   Node *pNode = NULL;
   VoipNode *pVoip = NULL;
   char *cp;
   char *CallConfig = NULL;
   char *CallConfig_FD = NULL;
   char PortConf_FD[] = "portxxxxx_fd.conf";
   char PortConf[] = "portxxxxx.conf";
   char VoipConf_FD[] = "voip_fd.conf";
   char VoipConf[] = "voip.conf";
   int Configs = 0;
   int CallLen = strlen(pCC->Callsign);

   char *ConfigFile[6];

   do {
      pNode = FindNode(pCC->Callsign);
      if(pNode != NULL) {
      // Hmmm... the node already exists !
         LOG_ERROR(("%s: node %s already exists!\n",__FUNCTION__,pCC->Callsign));
         pVoip = dynamic_cast<VoipNode *> (pNode);
         if(pVoip == NULL) {
            LOG_ERROR(("%s: node %s is not an VoipNode!\n",__FUNCTION__,
                       pCC->Callsign));
            CORE_DUMP();
         }
         break;
      }

      if(pCC->pVoipNode != NULL) {
         LOG_ERROR(("%s: pVoipNode != NULL!\n",__FUNCTION__));
         break;
      }

      if((pVoip = new VoipNode(pCC)) == NULL) {
         LOG_ERROR(("%s: new failed\n",__FUNCTION__));
         break;
      }
      pCC->pVoipNode = CurrentNode = pNode = pVoip;

      memset(ConfigFile,0,sizeof(ConfigFile));
      if(pCC->bFullDuplex) {
      // Top priority <callsign>_fd.conf
         if((CallConfig_FD = (char *) malloc(CallLen+9)) == NULL) {
            LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
            break;
         }

         strcpy(CallConfig_FD,pCC->Callsign);
         cp = CallConfig_FD;

         while(*cp) {
            *cp = tolower(*cp);
            cp++;
         }
         strcat(CallConfig_FD,"_fd.conf");
         ConfigFile[Configs++] = CallConfig_FD;
      }

   // Next priority callsign.conf
      if((CallConfig = (char *) malloc(CallLen+6)) == NULL) {
         LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
         break;
      }
      strcpy(CallConfig,pCC->Callsign);
      strcat(CallConfig,".conf");
      cp = CallConfig;

      while(*cp) {
         *cp = tolower(*cp);
         cp++;
      }

      ConfigFile[Configs++] = CallConfig;

      if(pCC->bFullDuplex) {
      // Next priority portxxxxx_fd.conf
         ConfigFile[Configs++] = PortConf_FD;
         snprintf(PortConf_FD,sizeof(PortConf_FD),"port%d_fd.conf",
                  pCC->pCS->AudioPort);
      }

   // Next priority portxxxxx.conf
      snprintf(PortConf,sizeof(PortConf),"port%d.conf",pCC->pCS->AudioPort);
      ConfigFile[Configs++] = PortConf;

      if(pCC->bFullDuplex) {
      // Next priority voip_fd.conf
         ConfigFile[Configs++] = VoipConf_FD;
      }

   // Lowest priority voip.conf
      ConfigFile[Configs++] = VoipConf;

      for(int i = 0; i < Configs; i++) {
         FILE *fp;
         if((fp = fopen(ConfigFile[i],"r")) != NULL) {
            LOG_NORM(("%s: Configuring new VoIP client using %s\n",
                      __FUNCTION__,ConfigFile[i]));

            for(int Pass = 1; Pass < 3; Pass++) {
               int Err;
               if((Err = read_config(fp,VoipConfigVars,Pass)) != 0) {
                  LOG_ERROR(("%s: read_config failed (%d)\n",__FUNCTION__,Err));
               }
               fseek(fp,0,SEEK_SET);
            }
            fclose(fp);
            break;
         }
      }

      switch(CurrentNode->DtmfMethod) {
         case 0:
         // no DtmfMethod is ok
            break;

         case 3:
         // Software DTMF decoder enabled, initialize it
            dtmf_rx_init(&pNode->DtmfRxState,NULL,pNode);
            dtmf_rx_set_realtime_callback(&pNode->DtmfRxState,DtmfCallback,pNode);
            break;

         default:
            LOG_ERROR(("%s: DtmfMethod %d is undefined or inappropriate for "
                       "VoIP clients\n",__FUNCTION__,CurrentNode->DtmfMethod));
            break;
      }
   } while(FALSE);

   if(pNode != NULL) do {
      struct avl_traverser avl_trans;
      Node *pNode2Link = (Node *) avl_t_first(&avl_trans,NodeTree);
      while(pNode2Link != NULL) {
         CPort *pPort = dynamic_cast<CPort *> (pNode2Link);
         if(pPort != NULL && pNode2Link->Enabled()) {

            if((!pCC->bPermanent && pPort->LinkVoipIn) ||
                (pCC->bPermanent && pPort->LinkVoipOut))
            {  // connect this client to this port
               pNode2Link->AddRx(pNode);
               if(!pCC->bFilePlayer) {
                  pNode->AddRx(pNode2Link);
                  if(pNode2Link->ConnectAnnouncements &&
                     (!pCC->bIRLP || pNode2Link->IRLPAnnouncements)) 
                  {
                     pNode2Link->SayPhrase("connected to");
                     pNode2Link->SayPhrase(pNode->NodeName,SAY_FLAG_CALLSIGN);
                  }
               }
            }
         }
         pNode2Link = (Node *) avl_t_next(&avl_trans);
      }

      if(pVoip->WelcomeFile == NULL) {
         pVoip->WelcomeFile = ::WelcomeFile;
      }

      if(!pCC->bFilePlayer && 
         !pNode->DisableWelcome && 
         pVoip->WelcomeFile != NULL && 
         strstr(pVoip->WelcomeFile,".wav") != NULL) 
      {
      // .wav format welcome file, play it for the new connection
         do {
            CText ToneSpec;
            ToneSpec.print("FILE %s",pVoip->WelcomeFile);
            CToneGen *pGenerator = new CToneGen;

            if(pGenerator == NULL) {
               LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
               break;
            }
            char *pErrPos;
            if(pVoip->MaxPlayWithoutPause == -1) {
               pGenerator->MaxPlayWithoutPause = ::MaxPlayWithoutPause;
            }
            else {
               pGenerator->MaxPlayWithoutPause = pVoip->MaxPlayWithoutPause;
            }

            if(pVoip->MinPlayBackPause == -1) {
               pGenerator->MinPlayBackPause = ::MinPlayBackPause;
            }
            else {
               pGenerator->MinPlayBackPause = pVoip->MinPlayBackPause;
            }
            pGenerator->bWelcome = 1;

            if(pGenerator->ToneGenSetup(ToneSpec.Buf,&pErrPos)) {
            // error
               LOG_ERROR(("%s: ToneGenSetup(\"%s\") failed\n",__FUNCTION__,
                          (char *) ToneSpec));
               delete pGenerator;
               break;
            }
            pNode->AddToneGen(pGenerator);
         } while(FALSE);
      }

      if(pVoip->ConnectScript != NULL) {
         CText Cmd;

         Cmd.print("script %s %s connected",pVoip->ConnectScript,pVoip->NodeName);
         pVoip->ExecuteCommand(Cmd.Buf);
      }
   } while(FALSE);

   if(CallConfig != NULL) {
      free(CallConfig);
   }

   if(CallConfig_FD != NULL) {
      free(CallConfig_FD);
   }

   CurrentNode = CurrentSave;
}

int VoipNode::Enabled()
{
   int Ret = TRUE;

   if(!mCC->bConnected) {
   // not connected
      Ret = FALSE;
   }
   return Ret;
}


