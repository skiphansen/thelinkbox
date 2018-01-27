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

   $Log: ports.cpp,v $
   Revision 1.29  2013/05/13 16:41:06  beta2k

   Added patches from Kristoff - ON1ARF for sysfs GPIO access for PTT

   Revision 1.28  2012/12/09 16:33:31  wb6ymh
   Delete parallel port object if OpenParallelPort fails.  Prevents later
   attempt to unkey transmitter via unopended parallel port.

   Revision 1.27  2010/11/20 15:16:00  wb6ymh
   1. Added support for the G.726 codec.
   2. Added support for an PCF8754 i2c port expander.
   3. Added SetTxPower.

   Revision 1.26  2010/03/13 15:17:50  wb6ymh
   1. Modified 48kHz sampling rate support to allow it to be enabled for any sound
   device, not just the CM108.  This may allow sound devices to be used that do
   not support the native 8Khz sampling rate used by the VoIP codec, but do
   support 48kHz.

   2. Added an error message if a method is selected that requires a serial port
   and the device name wasn't specified.  Corrects segfault when user forgets
   to set DevName.

   3. Modified WriteAudioOut to clear bWriteWait when the output buffer is empty.
   This now seems wrong, but I've been running this code for months and the
   profiler says this change is a big win in CPU utilization.  More thought
   and investigation is needed!

   Revision 1.25  2009/09/15 16:57:27  wb6ymh
   1. Changed DownConvertLPCoef array from 14 bits to 13 bits to prevent math
      overflow problems.
   2. Modified GetAudioIn to correct buffer overwrite in the 8Khz ring buffer
      which occurred when a sound device was configured to support two seperate
      ports in stereo mode.
   3. Added .rxlevel support.
   4. Added support for the new configuration variable DefaultToneLevel.
   5. Corrected bug in transmitter timeout routine that prevented it from being
      reset when the offending input was removed.
   6. Added code to AudioInit to set DriverAudioOutDelay to 80 if it is
      specified in the port configuration file for non-CM108 devices.

   Revision 1.24  2009/07/07 02:32:08  beta2k
   Removed Aux_PTT_Bit, IdAuxPTT, InvertAuxPTT and replaced with IdNoCTCSS, 
   PTT_CTCSS_Bit, InvertPTT_CTCSS.  Logic is now setup so that instead of 
   keying the radio differently we mute the CTCSS encoder

   Revision 1.23  2009/07/03 17:32:22  wb6ymh
   1. Added NoAudioInLooping  to workaround for an apparent driver bug for
      the Via sound device driver used on the "Little Blue" platform.
   2. Added NoHangtimeOnID support.
   3. Massive changes to the toneburst code.
      The port's receiver is now enabled when:
        1. A tone burst is received.
        2. The ToneBurstMode is 2 and the proper CTCSS tone is decoded.
        3. The port's transmitter has transmitted something other than an ID
      within the last ToneBurstTimeout seconds.
   4. Restored support for the CWSpeed port configuration parameter which was
      lost when the original CW ID code was replaced with tone generator based
      code in version 0.40.
   5. Corrected a sampling rate conversion bug in GetAudioIn: copy all of the
      unprocessed audio, not just 1/2 of it!
   6. Added support for the PCM playback of RawAudioIn and RxCtcssFilter for
      debugging.

   Revision 1.22  2009/07/01 01:11:41  beta2k
   Skip nicely pointed out that 0x0F on IRLP_AUX3 would give 0b00001111, not 
   0b00010000 as my mind first said it would.  Changed to 0x10.

   Revision 1.21  2009/06/26 02:39:33  beta2k
   Added Aux_PTT_Bit, bAuxPTT, IdAuxPTT and InvertAuxPTT to allow for the IDer 
   to be keyed via a different pin then the main PTT.  Added logic to allow 
   the ID() function to tell the KeyTx() function to use the alternate pin.

   Revision 1.20  2009/05/25 16:18:40  wb6ymh
   1. Moved DTMF processing to after receiver audio has been deemphasized.
   2. Added CTCSS high pass filter before DTMF decoder.

   Revision 1.19  2009/05/24 18:17:31  wb6ymh
   1. Modified VoipClock to reset timing average every 10 minutes to prevent
      math overflows that occured after running for a few weeks (24.855 days
      to be precise).
   2. Added code to CPort::GetAudioIn to prevent DownConvert from writing
      past end of 8Khz ring buffer. Fixes clicking in audio when CM108 is
      used on a Little Blue.

   Revision 1.18  2009/05/22 13:39:17  wb6ymh
   Added CosPollRate to give the ability to reduce reduce the polling rate where
   it matters.  Fixes badly broken audio when an CM108 based interface is used
   on a "Little Blue" patform (677 Mhz Via CPU and USB 1.1 ports).

   Revision 1.17  2009/04/04 16:20:22  wb6ymh
   Added code to set default parallel port name if it wasn't specified in the
   configuration file.

   Revision 1.16  2009/03/28 16:04:11  wb6ymh
   Change from va3jss: SerialDevName -> DevName.

   Revision 1.15  2009/02/16 00:25:50  wb6ymh
   Corrected bug in UpConvert introduced by the stereo changes.
   AudioBuf -> MasterBuf.

   Revision 1.14  2009/02/14 18:21:17  wb6ymh
   Modified WriteAudioOut to take audio from MasterBuf not AudioBuf for mono,
   no sampling rate conversion case.

   Revision 1.13  2009/02/14 15:41:35  wb6ymh
   1. Massive restructuring of the audio buffering algorithm to minimize port to
      port audio delays.
   2. Added tone burst support.
   3. Replaced orignal cw ID code with tone generator based CW to simplify things.
   4. Completed debugging and implementation of stereo support.
   5. Reworked Vox code so it's treated more like a "real" cos.
   6. Added (undocumented) configuration variable DriveAudioOutDelay to provide
      the ability to fine tune PTT timing.  Cm108 based interfaces set
      DriveAudioOutDelay to 3 to compensate for the audio buffering int USB
      audio device itself.
   7. Restructured code to eliminate the KickStartAudioOut kludge.
   8. Added KeyTx(FALSE) call to AutioCleanup for TxKeyMethods other than WB2REM.
   9. Removed untested support for 8 bit sound cards.
   10. Added support for configuration variable RepeaterPort to EndPointInit.
   11. Made DumpStatus a Node method rather than a CPort only method.

   Revision 1.12  2009/01/04 03:25:36  wb6ymh
   1. Abstracted serial and parallel port access by moving code that accessed the
      serial and parallel port into Device class.
   2. Additional (incomplete) changes to support 2 radios with one stereo capable
      sound device.
   3. Added support for the .pcm command.
   4. Corrected bogus read failures reported by GetAudioIn with some sound
      card drivers.  Thanks to KB7RSI for his help debugging this problem.
   5. Added additional port configuration sanity checking and error reporting
      to EndPointInit.
   6. Added CmdSendAx25Beacon.

   Revision 1.11  2008/07/23 16:33:01  wb6ymh
   1. Optimized UpConvert and DownConvert by using the BIQUAD macro instead of
      calling the BiQuad function.
   2. Modified DumpStatus to take a CText pointer rather than a ClientInfo
      pointer.

   Revision 1.10  2008/06/26 17:30:41  wb6ymh
   1. Added code to ProcessAudioIn to call SendGenericData when queued generic
      data is waiting to be sent and the time since the last call to
      SendGenericData exceeds DH_PACKET_DELAY.

   2. Removed DTMF mute logic from ProcessAudioIn.

   3. Added code to initialize RxToneDesc and TxToneDesc to EndPointInit.

   4. Modified DumpStatus to support new DTMF mute logic.

   5. Added status return to SetTxTone, SetRxTone, SetRxFrequency, and
      SetTxOffset.

   6. Added SetFrequency, SyncRadioCtrl.

   7. Modified SendGenericDataParallel to call new SpinSleep function for delays,
      the OS usleep function sleeps for way too long.

   8. Modified SendGenericData to queue data for the future if called faster
      than DH_PACKET_DELAY.

   Revision 1.9  2008/05/14 22:56:16  wb6ymh
   Replaced <sys/limits.h> file with <sys/param.h>.  FreeBSD bitches if you
   try to include <sys/limits.h>

   Revision 1.8  2008/05/14 21:21:56  wb6ymh
   Embedded commit log comment cleanup only.

   Revision 1.7  2008/05/14 21:20:26  wb6ymh
   1. Changed a bunch of functions to take const char * rather than char *
      to fix compile problems with GCC 4.2.x.
   2. Added support for RigCtrl functions for Doug Hall protocol via USB GPIO
      or (untested) parallel port.
   3. Reimplmented Rx status display for the Ports command.
   4. Modifed SetRxTone to report errors via a CText class now passed as an
      argument.
   5. Replaced some malloc'ed/free'ed text strings with new CTexts class to
      clean up code somewhat.

   Revision 1.6  2008/04/27 15:35:11  wb6ymh
   Added code to start VoipClock process if no ports are defined.  Allows
   tlb to be run without any devices for other purposes.

   Revision 1.5  2008/04/25 05:23:23  wb6ymh
   1. Began modifications to allow a single stereo sound device to be use to
      interface to two separate radios.
   2. Modified the code to use 48khz to talk CM108 based USB dongles.  Added
      local sampling rate conversion routines to convert between 8Khz and 48Khz.
   3. Added support for site prefix decoding.
   4. Removed unneeded ClientInfo pointer arguments to ProcessAudioIn, PollCOS,
      and PollDTMF.
   5. Completed support for software based CTCSS decoding including search
      and decode all tones options.
   6. Added SetRxTone function.

   Revision 1.4  2008/03/18 20:05:19  wb6ymh
   Added code to set bCWIsID before sending CW id.

   Revision 1.3  2008/03/11 14:11:30  wb6ymh
   1. Moved include for common.h to after system headers.  Fixes compile problems
      (min macro) on some systems.
   2. Modified GetRxAudio to use BufNum to determine if new audio is ready rather
      than LNK_FLG_AUDIO_RDY. Just an optimization and code cleanup.

   Revision 1.2  2008/03/09 18:19:47  wb6ymh
   Added headers needed by FreeBSD.

   Revision 1.1  2008/03/09 17:10:13  wb6ymh
   Inital import - files broken out of old audio.cpp and audio.h due to
   major restructuring of the internal infrastructure to treat VoIP connections
   as pseudo ports.

*/
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/param.h> // for PATH_MAX
#include <sys/file.h>

#include "common.h"
#include "avl.h"
#include "main.h"
#include "configvars.h"
#include "configfile.h"
#include "dirclient.h"
#include "conference.h"
#include "linkbox.h"
#include "eventhook.h"
#include "ctcss.h"
#include "tbd.h"

#include "nodes.h"
#include "devices.h"
#include "ports.h"
#include "bitstream.h"
#include "g726.h"
#include "voip.h"
#include "filters.h"
#include "usbdev.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif


#define IRLP_PTT        0x02
#define IRLP_AUX1       0x04
#define IRLP_AUX2       0x08
#define IRLP_AUX3       0x10

#define IRLP_DTMF_SHIFT 3
#define IRLP_COS        0x80  // Busy

#ifdef __linux__
#define IRLP_DEVICE     "/dev/parport0"
#endif

#ifdef __FreeBSD__
#define IRLP_DEVICE     "/dev/ppi0"
#endif

#if 0
// IRLP interface board equates
#define IRLP_LOCKFILE_BASE   "/tmp/irlp-lockfile-"

#ifdef __linux__
#include <linux/ppdev.h>
#include <linux/parport.h>
#define PP_RD_DATA   PPRDATA
#define PP_WR_DATA   PPWDATA
#define PP_RD_STATUS PPRSTATUS
#define PP_DATA_TYPE char
#define IRLP_DEVICE     "/dev/parport0"
#endif

#ifdef __FreeBSD__
#include <dev/ppbus/ppi.h>
#include <dev/ppbus/ppbconf.h>
#define PP_RD_DATA   PPIGDATA
#define PP_WR_DATA   PPISDATA
#define PP_RD_STATUS PPIGSTATUS
#define PP_DATA_TYPE int
#define IRLP_DEVICE     "/dev/ppi0"
#endif

#define SERIAL_OPEN_FLAGS O_FSYNC | O_APPEND | O_NONBLOCK | O_RDWR | \
        O_NOCTTY | O_NDELAY

#endif

#ifndef  DEBUG_PCM_IN
#define AUDIO_OPEN_FLAGS   O_RDWR | O_NONBLOCK
#else
#define AUDIO_OPEN_FLAGS   O_RDWR
#endif

#define RX_IDLE_TOSS_COUNT    2

#define TLOG(x) if(Debug & DLOG_TBURST) LogErr x
#define PTTLOG(x) if(Debug & DLOG_PTT) LogErr x

int WriteWaits = 0;

FILE *fp = NULL;
FILE *fp1 = NULL;


CPort *pMasterPort = NULL;

int16 SilenceBuf[AUDIO_BUF_SIZE];

/*
"<S>IIR Filter Specifications -- BUTTERWORTH"
"   Type=","LOWPASS"
"   Order= ",4
"   Sampling frequency=",48000
"   Passband:",3000,0
"   Stopband:",4000,0
"   Passband ripple:",.5
"   Stopband loss:",3.660441
"<C>Coefficients (A,D,E,B,C) for wordlength ",24
1,3.742148E-02,2,1,-1.475217,.6869044
2,6.127672E-02,2,1,-1.207816,.3811328
*/

int DownConvertLPCoef[] = {
   307,     // A 3.742148E-02
   -12085,  // B -1.475217
   5627,    // C .6869044
   16384,   // D 2
   8192,   // E 1
// section 2
   502,     // A 6.127672E-02
   -9894,   // B -1.207816
   3122,    // C .3811328
   16384,   // D 2
   8192,   // E 1
};


void ID();
void PlayAnnouncementFile(char *FileName,int Flags);
void SetTimeout(struct timeval *pTimer,int Timeout);
int SendGenericDataCB(unsigned char *Data,int Len,void *pPriv);
int VoipClock(ClientInfo *p);
int OpenParallelPort(void);

CPort::CPort()
{
// set default configuration variable values
   VoxThreshold = 0;
   VoxHoldTime = 0;
   VoxTripDelay = 0;
   TxKeyMethod = 0;
// ON1ARF
	GpioSysClassId = -1;
   RxCosMethod = 0;
   SerialBaudRate = 2400;
   DTMFPollTime = 50;
   InvertPTT_CTCSS = 0;
   InvertPTT = 0;
   InvertCOS = 0;
   IdInterval = 0;
   VoiceIDIdleTime = 0;
   HalfDuplexRF = 0;
   PCMRate = 8000;
// set CtcssDecoderSamples to 300 milliseconds decode time @ 8 Khz sample rate.
   CtcssDecoderSamples = 3200;
   bMonitor = 0;
   COS_KeyCode = 0;
   CTCSS_KeyCode = 0;

   CWToneFreq = 1000;

   DevName = NULL;
   CWId = NULL;
   VoiceID = NULL;
   AudioDevice = NULL;
   AudioDongleSN = NULL;

// Init class variables
   bDevInCOS = FALSE;
   bCosActive = FALSE;
   bDiscFeed = 0;
   bUngatedRx = 0;
   TossRxIn = 0;
   PTT_CTCSS_Bit = -1;
   PTT_Bit = -1;
   COS_Bit = -1;
   CTCSS_Bit = -1;
   HangTimer.tv_sec = 0;
   LastCOSPoll.tv_sec = 0;
   RxCtcssMethod = 0;
   TxCtcssMethod = 0;
   RadioCtrlMethod = 0;
   RxCTCSSMode = 0;
   InvertCTCSS = 0;
   DtmfCTCSSMode = 1;   // require CTCSS if CTCSS is enabled
   pAudioC = NULL;
   AudioChannel = 0;    // default to mono
   pAudioSlave = NULL;
   pAudioMaster = NULL;
   pRadioCtrl = NULL;
   StereoInBuf = NULL;
   StereoOutBuf = NULL;
   RxCTCSSTone = -1;    // default to carrier squelch
   UCLastSample = 0;
   memset(DCTemp,0,sizeof(DCTemp));
   memset(UCTemp,0,sizeof(UCTemp));
   CtcssDecoderState = NULL;
   bRxCtcssSearch = FALSE;
   bRxCtcssAny = FALSE;
   CtcssRxThreshold = 6000;
   bConvertSampleRate = 0;
   DisableRateConversion = 0;
   RxFreq = 0.0;
   LastGenericDataSent.tv_sec = 0;
   GenericDataHead = NULL;
   LastID = 0;
   bTalking = FALSE;
   RxDeemphasis = 0;
   TxPreemphasis = 0;
   Ax25BeaconInterval = 0;
   Ax25ToneLevel = 16000;
   Ax25BeaconText = NULL;
   Ax25BeaconPath = NULL;
   Ax25TxDelay = 100;

   TxPower = -100;
   Haat = -100;
   AntGain = -100;
   Frequency = -100;
   CTCSS = -100;
   AntDir = -100;
   RptOffset = -100;
   LastAx25Beacon = 0;
   LinkVoipIn = TRUE;
   LinkVoipOut = TRUE;
   RxRingSize = AUDIO_BUF_SIZE_BYTES * 2;
   WrNdx = 0;
   RxActiveNdx = 0;
   VoxSamples = 0;
   VoxAveTemp = 0;
   VoxAveLevel = 0;
   MinAudioWrite = 0;
   WriteBufferSize = 0;
   WriteBufferLen = 0;
   AudioOutBuf = NULL;
   DriverAudioOutDelay = -1;
   AudioIn48 = 0;
   RepeaterPort = 0;
   CosPollRate = -1;
   EnableDtmfCtcssFilter = 1;
   NoAudioInLooping = 0;
   NoHangtimeOnID = 0;
   IdNoCTCSS = FALSE;
   pDevice = NULL;
   HangTime = 0;

   memset(DCTemp,0,sizeof(DCTemp));
   memset(UCTemp,0,sizeof(UCTemp));
   memset(CtcssTemp,0,sizeof(CtcssTemp));
   memset(DtmfCtcssFilterTemp,0,sizeof(DtmfCtcssFilterTemp));
   memset(&LastAudioOutTime,0,sizeof(LastAudioOutTime));
   memset(&LastStereoOutTime,0,sizeof(LastStereoOutTime));

#ifdef USB_SUPPORT
   udev = NULL;
   UsbDevType = 0;
#endif

// ON1ARF
	gpiosc_fd=-1;
};

CPort::~CPort()
{
   if(StereoInBuf != NULL) {
      free(StereoInBuf);
   }

   if(StereoOutBuf != NULL) {
      free(StereoOutBuf);
   }

   if(AudioOutBuf != NULL) {
      free(AudioOutBuf);
   }

   if(CtcssDecoderState != NULL)  {
      CtcssDestroy(CtcssDecoderState);
   }

   if(pRadioCtrl != NULL) {
      delete pRadioCtrl;
   }

   if(ToneBurstState != NULL) {
      free(ToneBurstState);
   }
#ifdef USB_SUPPORT
   UsbShutdown();
#endif

	if (gpiosc_fd >= 0) {
		close(gpiosc_fd);
	}; // end if
}

void CPort::KeyTx(int bKey)
{
   bTxKeyed = bKey;

   if(Debug & DLOG_VOIP_AUDIO) {
      LogErr("Port %s: %skey\n",NodeName,bKey ? "" : "un");
   }

   DPRINTF(("Port %s: %skey\n",NodeName,bKey ? "" : "un"));
   if(bKey) {
      TxKeyTime = TimeNow.tv_sec;
      if((TimeNow.tv_sec - LastID) > IdInterval) {
      // Time to ID
         ID();
      }
   }

   switch(TxKeyMethod) {
      case 1:  // WB2REM / VA3TO interface
         if(DtmfMethod != 1) {
         // For DtmfMethod 1, tx keying is handled in PollDTMF()
            if(bTxIsKeyed != bTxKeyed) {
               int wrote;
               bTxIsKeyed = bTxKeyed;
               wrote = pDevice->Write(bTxKeyed ? "T" : "R",1);
               if(wrote != 1) {
                  LOG_ERROR(("%s: write failed: %s",__FUNCTION__,
                             Err2String(errno)));
               }
            }
         }
         break;

      case 2:  // Parallel port interface
      case 3:  // RTS
      case 4:  // DTR
          if((bKey && !InvertPTT) || (!bKey && InvertPTT)) {
            pDevice->ChangeBit(PTT_Bit,TRUE);
            checkCTCSS();
          }
          else {
            pDevice->ChangeBit(PTT_Bit,FALSE);
            checkCTCSS();
          }
          break;

      case 5:  // USB GPIO
      case 6:  // PCF8754 I2C port expander on iMic
#ifdef USB_SUPPORT
         UsbKeyRadio(bKey);
#endif
         break;

		case 7: // GPIO /sys/class/gpio/... method
					// ON1ARF

			// try opening file to /sys/class/... if not yet done
			if (gpiosc_fd < 0) {
				snprintf(gpiosc_fname,40,"/sys/class/gpio/gpio%d/value",GpioSysClassId);

				gpiosc_fd = open (gpiosc_fname, O_WRONLY | O_NDELAY, 0);

				if (gpiosc_fd < 0) {
					LOG_ERROR(("%s: cannot open GPIO device %s for writing: %d (%s): %s",__FUNCTION__, gpiosc_fname, errno, strerror(errno)));
				}; // end if
				
			}; // end if

			// write to gpio device if possible
			if (gpiosc_fd >= 0) {
				int ret;

				const char on = '1';
				const char off = '0';

				if((bKey && !InvertPTT) || (!bKey && InvertPTT)) {
					// turn ON
					lseek(gpiosc_fd,0,SEEK_SET);
					ret=write(gpiosc_fd,&on,1);
				} else {
					// turn OFF
					lseek(gpiosc_fd,0,SEEK_SET);
					ret=write(gpiosc_fd,&off,1);
				}; // end else - if
                        
				if (!ret) {
					// something went wrong
					LOG_ERROR(("%s: Error write to GPIO file %s failed. Reason %d (%s)",__FUNCTION__, gpiosc_fname, errno, strerror(errno)));
				}; // end if


				
			}; 


			break;
   }
}


void CPort::ID()
{
   int TimeSinceLastActvity = TimeNow.tv_sec - LastActivity;

   if(!bTxKeyed && HalfDuplexRF &&
      (bTalking || TimeLapse(&LastSpoke) < 5000))
   {  // We're running on a half duplex link and we're talking or have been
      // talking recently.  Don't key the transmitter !
   }
   else {
      if(CWAutoJam || TimeSinceLastActvity < IdInterval) {
      // Send an ID
         LastID = TimeNow.tv_sec;

         if(VoiceID != NULL && TimeSinceLastActvity > VoiceIDIdleTime) {
         // We have a voice ID and the channel has been idle long enough
         // to play it
            PlayAnnouncementFile(VoiceID,PLAY_FLAG_ID);
         }
         else if(CWId != NULL) {
            SendCwId();
         }
      }
      else {
      // Don't ID again until we have some new activity
         LastID = 0;
      }
   }
}

void CPort::SendCwId()
{
   do {
      if(CWId == NULL) {
         break;
      }
      CText ToneSpec;
      ToneSpec.print("tf1 %d tl %d wpm %d cw %s",CWToneFreq,CWToneLevel,CWSpeed,
                     CWId);
      CToneGen *pGenerator = new CToneGen;

      if(pGenerator == NULL) {
         LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
         break;
      }
      pGenerator->bIDSource = TRUE;
      char *pErrPos;
      if(pGenerator->ToneGenSetup(ToneSpec.Buf,&pErrPos)) {
      // error
         LOG_ERROR(("%s: ToneGenSetup(\"%s\") failed\n",__FUNCTION__,
                    (char *) ToneSpec));
         delete pGenerator;
         break;
      }
      AddToneGen(pGenerator);
   } while(FALSE);
}

int CPort::SamplesAvailable(LinkedNode *pLink,int bForce)
{
   int Ret = 0;

   do {
      int RdNdx = pLink->RdNdx;

      if(RdNdx == RxActiveNdx) {
      // No unread audio available
         break;
      }
      else if(RxActiveNdx > RdNdx) {
         Ret = RxActiveNdx - RdNdx;
      }
      else {
         Ret = RxRingSize - RdNdx + RxActiveNdx;
      }

   } while(FALSE);

   return Ret;
}

int CPort::GetRxAudio(LinkedNode *pLink,void **AudioOut,int MaxSamples,int bForce)
{
   int Ret = 0;

   do {
      int RdNdx = pLink->RdNdx;

      if(RdNdx == RxActiveNdx) {
      // No unread audio available
         if(bForce) {
         // Didn't return any data, need to start with some buffering next time
            pLink->Flags |= LNK_FLG_FIRST_READ;
         }
         break;
      }
      else if(RxActiveNdx > RdNdx) {
         Ret = RxActiveNdx - RdNdx;
      }
      else {
         Ret = RxRingSize - RdNdx + RxActiveNdx;
      }

      if(pLink->Flags & LNK_FLG_FIRST_READ) {
      // don't return any audio until we have at least 2 buffers worth.
         if(Ret < 128) {
            Ret = 0;
            break;
         }
         D3PRINTF(("%s: First return: %d samples\n",__FUNCTION__,Ret));
         pLink->Flags &= ~LNK_FLG_FIRST_READ;
      }

      if(Ret > MaxSamples) {
         Ret = MaxSamples;
      }

      if(RdNdx + Ret > RxRingSize) {
      // reduce the returned size so we don't go past the end of the ring
         Ret = RxRingSize - RdNdx;
      }
      *AudioOut = &RxRing[RdNdx];
      D3PRINTF(("%s: returning %d samples from %d\n",__FUNCTION__,Ret,RdNdx));

      pLink->RdNdx = (RdNdx + Ret) % RxRingSize;
   } while(FALSE);

   return Ret;
}

// Convert sampling rate of 8Khz data in AudioBuf to 48Khz data in AudioBuf48Khz
void CPort::UpConvert(int Samples)
{
   int i;
   int j;
   int k = 0;
   int16 Sample = UCLastSample;
   int16 Delta;
   int16 Temp;
   int Wn;
   int Yn;

   for(i = 0; i < Samples; i++) {
      Delta = (MasterBuf[i] - Sample) / 6;
      for(j = 0; j < 6; j++) {
         Sample += Delta;
         BIQUAD(Sample,DownConvertLPCoef,UCTemp,Temp,13);
         BIQUAD(Temp,&DownConvertLPCoef[5],&UCTemp[2],AudioBuf48Khz[k],13);
         k++;
      }
      Sample = MasterBuf[i];
   }
   UCLastSample = Sample;
}

// Convert sampling rate of 48Khz data in AudioInBuf48 to 8Khz data in AudioInBuf
void CPort::DownConvert(int Samples)
{
   int i;
   int j;
   int k = 0;
   int16 Temp;
   int Wn;
   int Yn;

   for(i = 0; i < Samples; i++) {
      BIQUAD(AudioInBuf48Khz[k++],DownConvertLPCoef,DCTemp,Temp,13);
      BIQUAD(Temp,&DownConvertLPCoef[5],&DCTemp[2],AudioInPtr[i],13);
      for(j = 1; j < 6; j++) {
         BIQUAD(AudioInBuf48Khz[k++],DownConvertLPCoef,DCTemp,Temp,13);
         BIQUAD(Temp,&DownConvertLPCoef[5],&DCTemp[2],Temp,13);
      }
   }
}

// return TRUE if we write audio to the sound device
int CPort::WriteAudioOut(ClientInfo *p)
{
   int wrote;
   char *pWriteData;
   int Ret = FALSE;
   audio_buf_info BufInfo;
   int SamplesNeeded;
   int MasterSamples = 0;
   int SlaveSamples = 0;
   int SamplesAvailable;

   for( ;  ;) {
      SamplesAvailable = 0;
      if(ioctl(p->Socket,SNDCTL_DSP_GETOSPACE,&BufInfo) == -1) {
         LOG_ERROR(("%s: SNDCTL_DSP_GETOSPACE failed: %s",__FUNCTION__,
                    Err2String(errno)));
         break;
      }
      D3PRINTF(("BufInfo1: bytes: %d, fragments: %d, fragsize: %d\n",
                BufInfo.bytes,BufInfo.fragments,BufInfo.fragsize));

      if(BufInfo.fragments == 0) {
         D3PRINTF(("BufInfo: bytes: %d, fragments: %d, fragsize: %d, bailing\n",
                   BufInfo.bytes,BufInfo.fragments,BufInfo.fragsize));
         p->bWriteWait = FALSE;
         break;
      }
      SamplesNeeded = BufInfo.fragsize / sizeof(int16);

      if(AudioChannel != 0) {
         SamplesNeeded /= 2;
      }
      else if(bConvertSampleRate) {
         SamplesNeeded /= 6;
      }

   // Limit request to the minimum amount of "live" audio that's available
   // if any "live" audio is available
      MasterSamples = LiveSamplesAvailable();

      if(MasterSamples != 0 && MasterSamples < SamplesNeeded) {
         SamplesNeeded = MasterSamples;
      }

      if(pAudioSlave != NULL) {
         SlaveSamples = pAudioSlave->LiveSamplesAvailable();
         if(SlaveSamples != 0 && SlaveSamples < SamplesNeeded) {
            SamplesNeeded = SlaveSamples;
         }
      }

      MasterSamples = GetTxAudio(MasterBuf,SamplesNeeded);

      if(pAudioSlave == NULL) {
         SamplesAvailable = MasterSamples;
      }
      else {
         if(MasterSamples > 0) {
         // Try to get the same number of samples from the slave to
         // keep the channels in sync.  If the slave can't provide that
         // much audio then silence will be inserted (life's a bitch)
            SamplesNeeded = MasterSamples;
         }
         SlaveSamples = pAudioSlave->GetTxAudio(SlaveBuf,SamplesNeeded);

         if(SlaveSamples < MasterSamples) {
         // Fill the missing samples with silence
            SamplesAvailable = MasterSamples;
            if(SlaveSamples > 0) {
               memset(&SlaveBuf[SlaveSamples],0,MasterSamples-SlaveSamples);
            }
         }
         else {
            SamplesAvailable = SlaveSamples;
         }
      }

      if(SamplesAvailable == 0) {
         break;
      }

#if 1
      D3PRINTF(("BufInfo: bytes: %d, fragments: %d, fragsize: %d\n",BufInfo.bytes,
             BufInfo.fragments,BufInfo.fragsize));
#endif
      if(AudioChannel != 0) {
      // pack data from the two mono buffers into the stereo buffer
         int16 *pRight;
         int16 *pLeft;
         int16 *pOut = StereoOutBuf;

         if(AudioChannel == 1) {
         // Master channel is right channel
            pRight = MasterSamples > 0 ? MasterBuf : SilenceBuf;
            if(pAudioSlave == NULL || SlaveSamples == 0) {
               pLeft = SilenceBuf;
            }
            else {
               pLeft = SlaveBuf;
            }
         }
         else {
         // Master channel is left channel
            pLeft = MasterSamples > 0 ? MasterBuf : SilenceBuf;
            if(pAudioSlave == NULL || SlaveSamples == 0) {
               pRight = SilenceBuf;
            }
            else {
               pRight = SlaveBuf;
            }
         }

         for(int i = 0; i < SamplesAvailable; i++) {
            *pOut++ = *pLeft++;
            *pOut++ = *pRight++;
         }
      }

      if(bConvertSampleRate) {
      // Upconvert from 8Khz to 48Khz sampling rate
         UpConvert(SamplesAvailable);
      }

      if(TimeLapse(&LastStereoOutTime) >= 0) {
      // Sound card buffer is empty
         LastStereoOutTime = TimeNow;
      }

      if(MasterSamples > 0) {
         if(TimeLapse(&LastAudioOutTime) >= 0) {
         // Master channel's buffer had emptied
            LastAudioOutTime = LastStereoOutTime;
         }
         Add2Timer(&LastAudioOutTime,MasterSamples);
      }

      if(SlaveSamples > 0) {
         if(TimeLapse(&pAudioSlave->LastAudioOutTime) >= 0) {
         // Slave audio channel's audio buffer has emptied
            pAudioSlave->LastAudioOutTime = LastStereoOutTime;
         }
         Add2Timer(&pAudioSlave->LastAudioOutTime,SlaveSamples);
      }
      Add2Timer(&LastStereoOutTime,SamplesAvailable);

      int WriteSize;
      if(bConvertSampleRate) {
         pWriteData = (char *) AudioBuf48Khz;
         WriteSize = SamplesAvailable * 6 * sizeof(int16);
      }
      else if(AudioChannel == 0) {
         pWriteData = (char *) MasterBuf;
         WriteSize = SamplesAvailable * sizeof(int16);
      }
      else {
         pWriteData = (char *) StereoOutBuf;
         WriteSize = SamplesAvailable * 2 * sizeof(int16);
      }

      if(MinAudioWrite != 0 && BufInfo.fragments == BufInfo.fragstotal) {
      // writing to an empty driver, make sure we're writing enough audio
      // to keep from underrunning
         int BufSizeNeeded = max(MinAudioWrite,WriteSize+WriteBufferLen);
         if(WriteBufferSize < BufSizeNeeded) {
         // need to (re)allocate a buffer
            int16 *Temp = AudioOutBuf;
            if((AudioOutBuf = (int16 *) malloc(BufSizeNeeded)) == NULL) {
               LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
               break;
            }
            D3PRINTF(("%s: allocated %d bytes AudioOutBuf\n",__FUNCTION__,
                     BufSizeNeeded));
            WriteBufferSize = BufSizeNeeded;

            if(Temp != NULL) {
            // Copy data from old buffer
               D3PRINTF(("%s: copying %d bytes from old AudioOutBuf to AudioOutBuf\n",
                      __FUNCTION__,WriteBufferLen));
               memcpy(AudioOutBuf,Temp,WriteBufferLen);
               free(Temp);
            }
         }

         if(WriteBufferLen != 0 || WriteSize < MinAudioWrite) {
         // something is in the buffer or this write is too small.
         // copy the data into write buffer
            D3PRINTF(("%s: copying %d bytes to AudioOutBuf[%d]\n",
                   __FUNCTION__,WriteSize,WriteBufferLen/sizeof(int16)));
            memcpy(&AudioOutBuf[WriteBufferLen/sizeof(int16)],pWriteData,
                   WriteSize);
            WriteBufferLen += WriteSize;
            WriteSize = WriteBufferLen;
            if(WriteSize < MinAudioWrite) {
            // can't write yet
               D3PRINTF(("%s: Can't write yet, %d < %d\n",__FUNCTION__,WriteSize,
                      MinAudioWrite));
               WriteSize = 0;
            }
            else {
             // ok to write now
               pWriteData = (char *) AudioOutBuf;
               D3PRINTF(("%s: writing %d bytes from AudioOutBuf\n",__FUNCTION__,
                      WriteSize));
               WriteBufferLen = 0;
            }
         }
      }

   // FreeBSD will allocate a huge output buffer if we don't write in
   // fragment sized hunks... so make sure that we do
      while(WriteSize > 0) {
         int Bytes2Write;

         Bytes2Write = min(BufInfo.fragsize,WriteSize);
         wrote = write(p->Socket,pWriteData,Bytes2Write);

         D3PRINTF(("Wr: requested %d, wrote %d/%d, pid: %d\n",Bytes2Write,wrote,
                BufInfo.bytes,(int) getpid()));

         if(wrote <= 0) {
            if(errno == EAGAIN) {
               WriteWaits++;
               p->bWriteWait = TRUE;
               D2PRINTF(("W"));
            }
            else {
               LOG_ERROR(("WriteAudioOut(): write() failed: %s",
                          Err2String(errno)));
            }
            break;
         }
         else {
            if(DebugFpAudioOut != NULL) {
               fwrite(pWriteData,wrote,1,DebugFpAudioOut);
            }
            else if(pAudioSlave != NULL &&
                    pAudioSlave->DebugFpAudioOut != NULL)
            {
               fwrite(pWriteData,wrote,1,pAudioSlave->DebugFpAudioOut);
            }
            D2PRINTF(("w"));
            HangTimer.tv_sec = 0;
            if(pAudioSlave != NULL) {
               pAudioSlave->HangTimer.tv_sec = 0;
            }
            Ret = TRUE;
         }
         pWriteData += wrote;
         WriteSize -= wrote;
         if(MasterSamples > 0 && !bTxKeyed) {
            KeyTx(TRUE);
         }
         if(SlaveSamples > 0 && !pAudioSlave->bTxKeyed) {
            pAudioSlave->KeyTx(TRUE);
         }
      }

      if(SamplesAvailable == 0) {
      // No more audio available now
         p->bWriteWait = FALSE;
         break;
      }
   }

   if(BufInfo.fragments == BufInfo.fragstotal) {
      D3PRINTF(("%s: Returning with output buffer empty\n",__FUNCTION__));
      p->bWriteWait = FALSE;
   }

   return Ret;
}

void CPort::StartTalking(int bTalk)
{
   if(bTalking != bTalk) {
      bTalking = bTalk;

      if(!bTalk) {
         LastSpoke = TimeNow;
      }
   }
}

void CPort::Vox(int Samples)
{
   int i;

   for(i = 0; i < Samples; i++) {
      VoxAveTemp += abs(AudioInPtr[i]);
   }
   VoxSamples += Samples;

   if(VoxSamples >= 800) {
   // average of 100 milliseconds or more
      VoxAveLevel = VoxAveTemp / VoxSamples;
      VoxSamples = VoxAveTemp = 0;
      if(VoxAveLevel > VoxThreshold) {
         LastVoxTrip = TimeNow;
D2PRINTF(("V"));
         if(!bRxActive && (Debug & DLOG_COS_CTCSS)) {
            LOG_ERROR(("%s: AveLevel: %d/%d\n",__FUNCTION__,VoxAveLevel,
                       VoxThreshold));
         }
      }
   }
}

// Return the number of milliseconds between two times
int DeltaTime(struct timeval *pTimeA,struct timeval *pTimeB)
{
   return 0;
}

void CPort::ProcessAudioIn(int Samples)
{
   int Elapsed;
   int bRxWasActive = bRxActive;

   if(AudioInPtr+Samples > RxRing + RxRingSize) {
      CORE_DUMP();
   }

   D3PRINTF(("%s: Samples %d\n",__FUNCTION__,Samples));

   if(HalfDuplexRF && bTxKeyed) {
      StartTalking(FALSE);
   }
   else {
      if(RxCosMethod == 1) {
         Vox(Samples);
      }
   }

   if(bCosActive && DebugFpAudioIn != NULL) {
      fwrite(AudioInPtr,Samples*sizeof(int16),1,DebugFpAudioIn);
   }


   if(MinMaxSamples < 8000) {
      LevelMonitor(AudioInPtr,Samples);
   }

   if(CtcssDecoderState != NULL) {
      int16 CtcssBuf[AUDIO_BUF_SIZE];
      int RxCTCSS;

   // low pass filter input
      CtcssLowPass(AudioInPtr,CtcssBuf,Samples,CtcssTemp);
      if(DebugFpRxCtcssFilter != NULL) {
         fwrite(CtcssBuf,Samples*sizeof(int16),1,DebugFpRxCtcssFilter);
      }

      if(DebugFpRxCtcssPB != NULL) {
         int DummyRead = fread(CtcssBuf,Samples,sizeof(int16),DebugFpRxCtcssPB);
         if(DummyRead < Samples) {
            if(feof(DebugFpRxCtcssPB)) {
               fseek(DebugFpRxCtcssPB,0,SEEK_SET);
            }
            else {
               fclose(DebugFpRxCtcssPB);
               DebugFpRxCtcssPB = NULL;
            }
         }
      }
      RxCTCSS = UpdateCTCSS(CtcssBuf,Samples,CtcssDecoderState,
                            CtcssRxThreshold);
      if(CurrentRxCTCSS != RxCTCSS) {
      // CTCSS decode state has changed
         CurrentRxCTCSS = RxCTCSS;
         if(bRxCtcssAny || bRxCtcssSearch) {
            if(RxCTCSS != -1)  {
               RxToneDesc = freq_ctcss[RxCTCSS];
            }
            else {
               RxToneDesc = bRxCtcssAny ? "any" : "search";
            }
         }

         if(bCosActive) {
            if(bRxCtcssAny || bRxCtcssSearch || DebugLevel > 0) {
               if(RxCTCSS != -1)  {
                  LOG_ERROR(("%s: Node %s CTCSS %s\n",__FUNCTION__,
                             NodeName,freq_ctcss[RxCTCSS]));
               }
               else {
                  LOG_ERROR(("%s: Node %s CTCSS decode inactive\n",__FUNCTION__,
                             NodeName));
               }
            }
            if(bRxCtcssAny || bRxCtcssSearch) {
               if(RxCTCSS != -1 && bRxCtcssAny) {
                  EventHook("ctcss_decode %s %s",NodeName,(char *) RxToneDesc);
               }
            }
         }
         CtcssDecodeTime = TimeNow;
      }

      if(bRxCtcssSearch && RxCTCSS != -1) {
         if(TimeLapse(&CtcssDecodeTime) > 1000) {
         // Found tone!
            SetSoftRxTone(RxCTCSS);
            EventHook("ctcss_decode %s %s",NodeName,(char *)RxToneDesc);

            LOG_ERROR(("Tone search complete, port %s tone %s\n",
                       NodeName,(char *)RxToneDesc));
         }
      }
   }

   if(RxDeemphasis) {
   // Deemphasize receiver audio

      ApplyRxDeemphasis(AudioInPtr,AudioInPtr,Samples,&DeemphasisTemp);
      if(bCosActive && DebugFpRxDeemphasis != NULL) {
         fwrite(AudioInPtr,Samples*sizeof(int16),1,DebugFpRxDeemphasis);
      }
   }

   if(ToneBurstMode != 0) do {
      if(ToneBurstState == NULL) {
         make_goertzel_descriptor(&ToneBurstDesc,ToneBurstFreq,102);
         ToneBurstState = (goertzel_state_t *) malloc(sizeof(goertzel_state_t));
         if(ToneBurstState == NULL) {
            LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
            break;
         }
         goertzel_init(ToneBurstState,&ToneBurstDesc);
      }

      int SamplesProcessed;
      float Result;

      for(int i = 0; i < Samples; i += SamplesProcessed) {
         #define THRESHOLD 8.0e7
         SamplesProcessed = goertzel_update(ToneBurstState,AudioInPtr + i,
                                            Samples-i);
         for(int j = 0; j < SamplesProcessed; j++) {
            Energy += AudioInPtr[j+i] * AudioInPtr[j+i];
         }

         if(ToneBurstState->current_sample >= ToneBurstState->samples) {
            Result = goertzel_result(ToneBurstState);
         // Scale the energy so it can be compared to the results from the
         // Goertzel filters.
            Energy *= ToneBurstState->samples/2;
            if(Result > 0.5 * Energy) {
               if(!bToneBurstDetected) {
                  TLOG(("%s: Toneburst decoded on port %s\n",__FUNCTION__,
                       NodeName));
               }
               bToneBurstWasDetected = bToneBurstDetected = TRUE;
            }
            else {
               if(bToneBurstDetected) {
                  TLOG(("%s: End of toneburst decode on port %s\n",
                        __FUNCTION__,NodeName));
               }
               bToneBurstDetected = FALSE;
            }
            goertzel_init(ToneBurstState,&ToneBurstDesc);
            Energy = 0.0;
         }
      }
   } while(FALSE);
   
   if(bTalking) {
      if(DtmfMethod == 3) {
         int16 *pDtmf = AudioInPtr;
         int16 DtmfBuf[AUDIO_BUF_SIZE];
         if(EnableDtmfCtcssFilter) {
            pDtmf = DtmfBuf;
            ApplyCtcssFilter(AudioInPtr,DtmfBuf,Samples,DtmfCtcssFilterTemp);
         }
         if(DebugFpDtmfIn != NULL) {
            fwrite(pDtmf,Samples*sizeof(int16),1,DebugFpDtmfIn);
         }
         dtmf_rx(&DtmfRxState,pDtmf,Samples);
      }
   }

   CheckDtmfTimeouts();

   if(GenericDataHead != NULL &&
      (Elapsed = TimeLapse(&LastGenericDataSent)) > DH_PACKET_DELAY)
   {
      GenericData *pDataSave = GenericDataHead;
      GenericDataHead = pDataSave->Link;

      if(Debug & DLOG_RIG_CTRL) {
         LOG_ERROR(("%s: Sending queued data, last was sent %d milliseconds ago\n",
                    __FUNCTION__,Elapsed));
      }

      SendGenericData(pDataSave->Data,pDataSave->Len);
      delete pDataSave;
   }

   if(!bTalking && bUngatedRx && TossRxIn < RX_IDLE_TOSS_COUNT) {
   // Unsquelched audio source, toss the first audio buffer after COS goes
   // active to eliminate possible squelch tail on keyup
      D2PRINTF(("I"));
      TossRxIn++;
   }
   else if(bTalking && TossRxIn == RX_IDLE_TOSS_COUNT) {
      TossRxIn = 0;
      D2PRINTF(("i"));
   }
   else if(bTalking && !bMonitor) {
      BufNum++;
      D2PRINTF(("r"));
   }

   u_int32 LastWrNdx = WrNdx;
   WrNdx = (WrNdx + Samples) % RxRingSize;

   if(CosPollRate <= 0 || TimeLapse(&LastCOSPoll) >= CosPollRate) {
      LastCOSPoll = TimeNow;
      PollCOS();
   }

   if(bRxActive) {
      RxActiveNdx = WrNdx;
      if(!bRxWasActive) {
      // Receiver just went active, update all of our receiver's read
      // indexs to point to the point in the ring where the receiver went
      // active and set the first read flag
         D3PRINTF(("%s: LastWrNdx %d, RxActiveNdx %d\n",__FUNCTION__,LastWrNdx,
                RxActiveNdx));
         LinkedNode *pLink = pTransmitters;
         while(pLink != NULL) {
            pLink->RdNdx = LastWrNdx;
            pLink = pLink->pNextTx;
         }
      }
   }
}

void CPort::GetAudioIn(ClientInfo *p)
{
   int Read;
   int MaxRead;
   char *pWriteData;
   u_int32 i;
   u_int32 NewSamples;
   u_int32 RingNdx;

   audio_buf_info BufInfo;
   if(ioctl(p->Socket,SNDCTL_DSP_GETISPACE,&BufInfo) == -1) {
      LOG_ERROR(("%s: SNDCTL_DSP_GETOSPACE failed: %s",__FUNCTION__,
                 Err2String(errno)));
   }
   D3PRINTF(("%s: BufInfo: bytes: %d, fragments: %d/%d, fragsize: %d\n",__FUNCTION__,
          BufInfo.bytes,
          BufInfo.fragments,BufInfo.fragstotal,
          BufInfo.fragsize));

   if(MinAudioWrite == -1) {
   // Assume the size of the first read in bytes is the MinAudioWrite
      MinAudioWrite = BufInfo.bytes;
   }
   else if(MinAudioWrite == -2) {
   // Assume the size of the first read in whole fragments is the MinAudioWrite
      MinAudioWrite = BufInfo.fragments * BufInfo.fragsize;
   }

#ifndef  DEBUG_PCM_IN
   for( ; ; ) {
      RingNdx = WrNdx % RxRingSize;
      AudioInPtr = &RxRing[RingNdx];
      if(bConvertSampleRate) {
      // Read 48Khz mono data
         int MaxRead8khz = (RxRingSize - RingNdx) * sizeof(int16);
         pWriteData = (char *) &AudioInBuf48Khz[AudioIn48];
         MaxRead = (AUDIO_BUF_SIZE_BYTES * 6) - AudioIn48;
         if(MaxRead / 6 > MaxRead8khz) {
         // Don't wrap off the end of the 8Khz audio ring buffer either
            MaxRead = MaxRead8khz * 6;
         }
      }
      else if(AudioChannel != 0) {
      // Read stereo data
         MaxRead = (RxRingSize - RingNdx) * sizeof(int16);
         if(MaxRead > AUDIO_BUF_SIZE_BYTES) {
            MaxRead = AUDIO_BUF_SIZE_BYTES;
         }
         MaxRead *= 2;
         pWriteData = (char *) StereoInBuf;
      }
      else {
      // Read 16 bit mono data
         pWriteData = (char *) AudioInPtr;
         MaxRead = (RxRingSize - RingNdx) * sizeof(int16);
         if(MaxRead > AUDIO_BUF_SIZE_BYTES) {
            MaxRead = AUDIO_BUF_SIZE_BYTES;
         }
      }
      Read = read(p->Socket,pWriteData,MaxRead);

      if(Read > 0 && DebugFpRawAudioPB != NULL) {
         int DummyRead = fread(pWriteData,1,Read,DebugFpRawAudioPB);
         if(DummyRead < Read) {
            Read = 0;
            if(feof(DebugFpRawAudioPB)) {
               fseek(DebugFpRawAudioPB,0,SEEK_SET);
               printf("rewinding RawAudioIn\n");
            }
            else {
               fclose(DebugFpRawAudioPB);
               DebugFpRawAudioPB = NULL;
            }
         }
      }

      if(Read <= 0) {
         if(Read < 0 && errno != EAGAIN) {
            LOG_ERROR(("GetAudioIn(): read() failed: %s",Err2String(errno)));
         }
         p->bReadReady = FALSE;
         break;
      }
      else {
         NewSamples = Read / sizeof(int16);
         if(/* bCosActive && */DebugFpRawAudioIn != NULL) {
            static int TotalSamples = 0;
            if(Read & 1) {
               LOG_ERROR(("GetAudioIn(): read odd number of bytes (%d)!\n",Read));
            }
#if 0
            LOG_ERROR(("GetAudioIn(): sample %d, read %d, MaxRead %d\n",
                       TotalSamples/2,Read,MaxRead));
#endif
            TotalSamples += Read;

            fwrite(pWriteData,Read,1,DebugFpRawAudioIn);
         }

         if(bConvertSampleRate) {
         // Convert 48Khz input data to 8Khz
            AudioIn48 += NewSamples;
            NewSamples = AudioIn48 / 6;
            DownConvert(NewSamples);
            if(AudioIn48 % 6 != 0) {
            // didn't process all of the samples, move the unprocessed samples
            // to the beginning of the buffer for next time.
               AudioIn48 = AudioIn48 - (NewSamples*6);
               memcpy(AudioInBuf48Khz,&AudioInBuf48Khz[NewSamples*6],
                      AudioIn48*sizeof(int16));
            }
            else {
               AudioIn48 = 0;
            }
         }
         else if(AudioChannel != 0) {
            int16 *pRight;
            int16 *pLeft;
            int16 *pIn = StereoInBuf;

            NewSamples /= 2;
            if(pAudioSlave != NULL) {
            // Unpack the stereo data into two mono buffers
               int16 *SlaveAudioInPtr = &pAudioSlave->RxRing[RingNdx];
               if(AudioChannel == 1) {
                  pRight = AudioInPtr;
                  pLeft = SlaveAudioInPtr;
               }
               else {
                  pRight = SlaveAudioInPtr;
                  pLeft = AudioInPtr;
               }

               if(NewSamples > RxRingSize - RingNdx) {
               // copy wraps across end of buffer, do in two pieces
                  u_int32 Samples2Copy = RxRingSize - RingNdx;
                  for(i = 0; i < Samples2Copy; i++) {
                     *pLeft++ = *pIn++;
                     *pRight++ = *pIn++;
                  }
               // Wrap pointers
                  Samples2Copy = NewSamples - Samples2Copy;
                  pLeft -= RxRingSize;
                  pRight -= RxRingSize;
                  for(i = 0; i < Samples2Copy; i++) {
                     *pLeft++ = *pIn++;
                     *pRight++ = *pIn++;
                  }
               }
               else {
                  for(i = 0; i < NewSamples; i++) {
                     *pLeft++ = *pIn++;
                     *pRight++ = *pIn++;
                  }
               }

               pAudioSlave->AudioInPtr = SlaveAudioInPtr;
               pAudioSlave->ProcessAudioIn(NewSamples);
            }
            else {
            // Unpack the stereo data into a mono buffer
               pRight = AudioInPtr;
               if(AudioChannel == 1) {
                  pIn++;
               }

               for(i = 0; i < NewSamples; i++) {
                  *pRight++ = *pIn++;
                  pIn++;
               }
            }
         }

         ProcessAudioIn(NewSamples);

         if(this == pMasterPort) {
         // This port is the master port which acts as a timing reference
         // for all VoIP connections.  Poll all VoIP connections for Tx
         // audio
            struct avl_traverser avl_trans;
            VoipNode *pVoip;
            Node *pNode = (Node *) avl_t_first(&avl_trans,NodeTree);
            while(pNode != NULL) {
               pVoip = dynamic_cast<VoipNode *> (pNode);
               if(pVoip != NULL) {
                  pVoip->PollTx();
               }
               pNode = (Node *) avl_t_next(&avl_trans);
            }
         }
      }
      if(NoAudioInLooping) {
         break;
      }
   }
#endif
}

void GetAudioIn(ClientInfo *p)
{
   CPort *pClass = (CPort *) p->p;

   pClass->GetAudioIn(p);
}

void CPort::PollCOS()
{
   int bCos = FALSE;
   int bRx;
   int bCTCSS = FALSE;
   int Bits = 0;
   int bStatusChange = FALSE;
#ifdef USB_SUPPORT
   int UsbGPIOData = 0;
#endif

   switch(RxCosMethod) {
      case 1:  {  // Vox
         int LastTrip = TimeLapse(&LastVoxTrip);
         if(LastTrip < VoxHoldTime) {
            bCos = TRUE;
         }
         else if((Debug & DLOG_COS_CTCSS) && bRxActive) {
            LOG_ERROR(("%s: AveLevel: %d/%d, last trip %d ms ago\n",
                       __FUNCTION__,VoxAveLevel,VoxThreshold,LastTrip));
         }
         break;
      }

      case 2:  // IRLP interface
      case 3:  // serial port CTS
      case 4:  // serial port DSR
      case 5:  // serial port DCD
         if((Bits = pDevice->GetBits()) == -1) {
            LOG_ERROR(("%s: GetBits failed\n"));
         }
         else if(Bits & COS_Bit) {
            bCos = TRUE;
         }
         break;

      case 6:  // /dev/input/event*
         bCos = bDevInCOS;
         break;

      case 7:  // USB device GPIO
      case 8:  // PCF8754 I2C port expander on iMic
#ifdef USB_SUPPORT
         UsbGPIOData = UsbPollGPIO();
         if(UsbGPIOData & COS_Bit) {
            bCos = TRUE;
         }
#endif
         break;
   }


   if(InvertCOS) {
      bCos = !bCos;
   }

   if(bCosActive != bCos) {
   // COS has changed state
      bCosActive = bCos;
      bStatusChange = TRUE;
   }

   switch(RxCtcssMethod) {
      case 1:  // Software CTCSS decoder
         if(bRxCtcssSearch || bRxCtcssAny) {
            if(CurrentRxCTCSS != -1) {
               bCTCSS = TRUE;
            }
         }
         else if(CurrentRxCTCSS == RxCTCSSTone || RxCTCSSTone == -1) {
         // Correct tone or no tone is programmed.
            bCTCSS = TRUE;
         }
         break;

      case 2:  // IRLP interface
      case 3:  // serial port CTS
      case 4:  // serial port DSR
      case 5:  // serial port DCD
         if(Bits & CTCSS_Bit) {
            bCTCSS = TRUE;
         }
         break;

      case 6:  // /dev/input/event*
         bCTCSS = bDevInCTCSS;
         break;

      case 7:  // USB device GPIO
      case 8:  // PCF8754 I2C port expander on iMic
#ifdef USB_SUPPORT
         if(UsbGPIOData & CTCSS_Bit) {
            bCTCSS = TRUE;
         }
#endif
         break;
   }

   if(InvertCTCSS) {
      bCTCSS = !bCTCSS;
   }

   if(bCtcssActive != bCTCSS) {
   // CTCSS  has changed state
      bCtcssActive = bCTCSS;
      bStatusChange = TRUE;
   }

   switch(RxCTCSSMode) {
      default:
         LOG_ERROR(("%s: Unknown RxCTCSSMode %d on port %s, changing to 0\n",
                    __FUNCTION__,RxCTCSSMode,NodeName));
         RxCTCSSMode = 0;
      // Intentional fall through to case 0
      case 0:  // Carrier squelch
         bRx = bCos;
         break;

      case 1:  // Carrier and CTCSS
         bRx = bCos && bCTCSS;
         break;

      case 2:  // Carrier or CTCSS
         if(bRxActive) {
         // Signal active, only need CTCSS to keep squelch open
            bRx = bCTCSS;
         }
         else {
         // Signal not active, need CTCSS and COS to open squelch
            bRx = bCos && bCTCSS;
         }
         break;

      case 3:  // CTCSS only
         bRx = bCTCSS;
         break;

      case 4:  // Carrier and NO CTCSS
         bRx = bCos && !bCTCSS;
         break;
   }

   if(ToneBurstMode != 0 && bRx && !bToneBurstAccessed) {
   // We have a signal but the has not been enabled receiver yet
      do {
         if(ToneBurstMode == 2 && bCtcssActive) {
         // CTCSS keyup enabled and we have CTCSS
            TLOG(("%s: Port %s: enabled by CTCSS\n",__FUNCTION__,NodeName));
            bToneBurstAccessed = TRUE;
            break;
         }

         if(ToneBurstMute) {
            if(!bToneBurstDetected && bToneBurstWasDetected) {
            // There was a tone burst and it has ended, enable the receiver
               TLOG(("%s: Port %s: !bToneBurstDetected && "
                     "bToneBurstWasDetected\n",__FUNCTION__,NodeName));
               bToneBurstAccessed = TRUE;
               break;
            }
         }
         else if(bToneBurstDetected) {
            TLOG(("%s: Port %s: bToneBurstDetected\n",__FUNCTION__,NodeName));
            bToneBurstAccessed = TRUE;
            break;
         }

         if(TimeNow.tv_sec - LastActivity < ToneBurstTimeout + HangTime/1000) 
         {
            TLOG(("%s: Port %s: enabled by recent traffic\n",__FUNCTION__,
                  NodeName));
            bToneBurstAccessed = TRUE;
            break;
         }
      } while(FALSE);

      if(!bToneBurstAccessed) {
         bRx = FALSE;
      }
   }

   if(ToneBurstMode && !bRx && bToneBurstAccessed && 
      (TimeNow.tv_sec - LastActivity) > ToneBurstTimeout + HangTime/1000) 
   {  // ToneBurstTimeout has expired
      TLOG(("%s: Port %s: ToneBurstTimeout expired\n",__FUNCTION__,NodeName));
      bToneBurstAccessed = FALSE;

      if(ToneBurstToneID != 0) do {
         char *ToneSpec = FindToneSpec(ToneBurstToneID);
         if(ToneSpec == NULL) {
            LOG_ERROR(("%s: ToneBurstToneID ID #%d is not defined, disabling.\n",
                       __FUNCTION__,ToneBurstToneID));
            ToneBurstToneID = 0;
            break;
         }

         CToneGen *pGenerator = new CToneGen(DefaultToneLevel);

         if(pGenerator == NULL) {
            LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
            break;
         }
         char *pErrPos;
         if(pGenerator->ToneGenSetup(ToneSpec,&pErrPos)) {
         // error
            LOG_ERROR(("%s: ToneGenSetup(\"%s\") failed\n",__FUNCTION__,
                       (char *) ToneSpec));
            delete pGenerator;
            break;
         }
         pGenerator->bIDSource = TRUE;
         AddToneGen(pGenerator);
      } while(FALSE);
   }

   if(RxTimeout != 0) {
      if(bRx) {
         if(RxActiveTime == 0) {
            RxActiveTime = TimeNow.tv_sec;
         }
         if(!bRxTimeout) {
         // Check for COS timeout
            if(TimeNow.tv_sec - RxActiveTime > RxTimeout) {
               LOG_NORM(("%s: Port %s Rx timeout\n",__FUNCTION__,NodeName));
               bRxTimeout = TRUE;
            }
         }
      }
      else {
         RxActiveTime = 0;
         if(bRxTimeout) {
         // COS Timeout cleared
            LOG_NORM(("%s: Port %s Rx timeout cleared\n",__FUNCTION__,NodeName));
            bRxTimeout = FALSE;
         }
      }

      if(bRxTimeout) {
         bRx = FALSE;
      }
   }

   if(bRxActive != bRx) {
   // Rx active has changed state
      bRxActive = bRx;
      bStatusChange = TRUE;

      if(bRx) {
         CtoneTimer.tv_sec = 0;
      }
      else {
      // User unkeyed
         SendCtone(FALSE);
         bToneBurstWasDetected = 0;
         if(DTMF_Digits > 0) {
         // There's something in the DTMF buffer
            if(DTMFPrefix == NULL || bPrefixed) {
               DLOG(DLOG_DTMF,("%s: Calling DecodeDTMF, node: %s\n",
                               __FUNCTION__,NodeName));
               DecodeDTMF(DTMF_Buf);
            }
            DTMF_Digits = 0;
            LastDTMFDigit.tv_sec = 0;
         }
      }

      StartTalking(bRx);
   }

   if(bStatusChange) {
      if(Debug & DLOG_COS_CTCSS) {
         if(RxCtcssMethod != 0) {
            LOG_ERROR(("Port %s: COS %sactive, CTCSS %sactive, Rx %sactive\n",
                     NodeName,
                     bCosActive ? "" : "in",
                     bCtcssActive ? "" : "in",
                     bRx ? "" : "in"));
         }
         else {
            LOG_ERROR(("Port %s: COS %sactive\n",NodeName,
                       bCosActive ? "" : "in"));
         }
      }
      else {
         if(RxCtcssMethod != 0) {
            DPRINTF(("Port %s: COS %sactive, CTCSS %sactive, Rx %sactive\n",
                     NodeName,
                     bCosActive ? "" : "in",
                     bCtcssActive ? "" : "in",
                     bRx ? "" : "in"));
         }
         else {
            DPRINTF(("Port %s: COS %sactive\n",NodeName,
                     bCosActive ? "" : "in"));
         }
      }
   }
}

void CPort::PollDTMF()
{
   int wrote;
   int BytesRead;
   static char LastDTMF;
   char CurrentDTMF;
   int Bits = 0;

   switch(DtmfMethod) {
      case 1:  // WB2REM / VA3TO interface
         while((BytesRead = pDevice->Read(&CurrentDTMF,1)) == 1) {
            if((CurrentDTMF >= '0' && CurrentDTMF <= '9') ||
               (CurrentDTMF == '*' || CurrentDTMF == '#') ||
               (CurrentDTMF >= 'A' && CurrentDTMF <= 'D') ||
               (CurrentDTMF == ' '))
            {
               if(LastDTMF != CurrentDTMF) {
                  LastDTMF = CurrentDTMF;
                  SaveDTMF(CurrentDTMF);
               }
            }
         }

         if(BytesRead < 0 && errno != EAGAIN) {
            LOG_ERROR(("PollDTMF(): read failed: %s",Err2String(errno)));
         }

         if(bTxIsKeyed != bTxKeyed) {
            bTxIsKeyed = bTxKeyed;
            wrote = pDevice->Write(bTxKeyed ? "T" : "R",1);
         }
         else {
         // don't need to key or unkey, just poll for DTMF
            wrote = pDevice->Write("C",1);
         }

         if(wrote != 1) {
            LOG_ERROR(("PollDTMF(): write failed: %s",Err2String(errno)));
         }
         break;

      case 2:  // IRLP interface
         if((Bits = pDevice->GetBits()) == -1) {
            LOG_ERROR(("%s: GetBits failed\n",__FUNCTION__));
         }
         else {
            CurrentDTMF = (Bits >> IRLP_DTMF_SHIFT) & 0xf;
            if(CurrentDTMF == 0) {
            // IRLP hardware doesn't include the data valid flag from
            // the DTMF decoder, only the 4 data bits.  Since the decoder
            // used outputs a "D" (0x0) when no digit is decoded it must
            // be tossed since we can't tell if it's valid or not.
               CurrentDTMF = ' ';
            }
            else if(CurrentDTMF < 10) {
               CurrentDTMF += '0';
            }
            else if(CurrentDTMF == 10) {
               CurrentDTMF = '0';
            }
            else if(CurrentDTMF == 11) {
               CurrentDTMF = '*';
            }
            else if(CurrentDTMF == 12) {
               CurrentDTMF = '#';
            }
            else {
               CurrentDTMF = 'A' + (CurrentDTMF - 13);
            }

            if(LastDTMF != CurrentDTMF) {
               LastDTMF = CurrentDTMF;
               SaveDTMF(CurrentDTMF);
            }
         }
         break;
   }
}

void CPort::CheckUnkey()
{
   if(bTxKeyed && TimeLapse(&LastAudioOutTime) > DriverAudioOutDelay) {
   // This port's audio has left the sound card
      if(bTxTimeOut) {
      // The transmitter has timed out, dump it
         LOG_ERROR(("%s: Port %s Tx timeout\n",__FUNCTION__,NodeName));
         KeyTx(FALSE);
      }
      else if(Ax25BeaconInterval != 0 &&
         TimeNow.tv_sec - LastAx25Beacon > Ax25BeaconInterval)
      {  // Time to send a beacon
         CmdSendAx25Beacon(NULL);
      }
      else if(NoHangtimeOnID && TimeNow.tv_sec - LastActivity >= HangTime/1000) {
      // No non-ID activity since HangTime, unkey now
         KeyTx(FALSE);
      }
      else if(HangTime == 0) {
      // Transmitter has no hang time, unkey now
         KeyTx(FALSE);
      }
      else if(HangTimer.tv_sec == 0) {
      // Start HangTimer
         HangTimer = TimeNow;
      }
      else if(TimeLapse(&HangTimer) >= HangTime) {
         HangTimer.tv_sec = 0;
         KeyTx(FALSE);
      }
   }
}

void CPort::DoPolls()
{
   if(LastID != 0 && (TimeNow.tv_sec - LastID) > IdInterval) {
   // time to ID
      ID();
   }

   if(bTxKeyed && !bTxTimeOut && TxTimeout != 0 &&
      (TimeNow.tv_sec - TxKeyTime) > TxTimeout)
   do {  // Timeout !
      bTxTimeOut = TRUE;
      if(TxTimeoutTone != 0) {
         char *ToneSpec = FindToneSpec(TxTimeoutTone);

         if(ToneSpec == NULL) {
            LOG_ERROR(("%s: Timeout tone ID #%d is not defined, disabling.\n",
                       __FUNCTION__,TxTimeoutTone));
            TxTimeoutTone = 0;
            break;
         }

         if((TxTimeoutToneGen = new CToneGen(DefaultToneLevel)) == NULL) {
            LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
            break;
         }

         if(TxTimeoutToneGen->ToneGenSetup(ToneSpec,NULL) != 0) {
            LOG_ERROR(("%s#%d: ToneGenSetup failed\n",__FUNCTION__,__LINE__));
            delete TxTimeoutToneGen;
            TxTimeoutToneGen = NULL;
         }
      }
   } while(FALSE);

   if(DtmfMethod == 2 ||
      (DtmfMethod == 1 && TimeLapse(&LastDTMFPoll) >= DTMFPollTime))
   {
      PollDTMF();
      LastDTMFPoll = TimeNow;
   }


   if(CtoneTimer.tv_sec != 0) {
      if(TimeLapse(&CtoneTimer) >= CtoneDelay) {
         SendCtone(TRUE);
         CtoneTimer.tv_sec = 0;
      }
   }

   if(!bTxKeyed && bTxTimeOut) {
      LinkedNode *pLink = pReceivers;
      while(pLink != NULL) {
         if(pLink->pRx->bRxActive) {
         // Still have and active receiver
            break;
         }
         pLink = pLink->pNextRx;
      }

      if(pLink == NULL) {
         LOG_ERROR(("%s: Port %s Tx timeout cleared\n",__FUNCTION__,NodeName));
         bTxTimeOut = 0;
      }
   }
}

int CPort::Audio_Handler(ClientInfo *p)
{
   int bWroteOutput = FALSE;

   if(p->bReadReady) {
      GetAudioIn(p);
   }

   DoPolls();
   if(pAudioSlave != NULL) {
      pAudioSlave->DoPolls();
   }

   if(p->bWriteReady || !p->bWriteWait) {
   // Audio output needs another buffer or is idle
      bWroteOutput = WriteAudioOut(p);
   }
//   p->bWriteWait = FALSE;

   if(!bWroteOutput || pAudioSlave != NULL) {
   // NB always check for unkey when running stereo even if we wrote
   // something.  The slave or master may need to be unkeyed even
   // if the other channel is still transmitting.
      CheckUnkey();
      if(pAudioSlave != NULL) {
         pAudioSlave->CheckUnkey();
      }
   }

   DFLUSH();

   return FALSE;
}

int Audio_Handler(ClientInfo *p)
{
   CPort *pClass = (CPort *) p->p;

   return pClass->Audio_Handler(p);
}

void CPort::AudioCleanup(ClientInfo *p)
{
   if(pDevice != NULL) {
      if(TxKeyMethod == 1) {
      // WB2REM / VA3TO interface
         usleep(1000*DTMFPollTime);
         pDevice->Write("R",1);
      }
      else {
         KeyTx(FALSE);
      }
   }
}

void AudioCleanup(ClientInfo *p)
{
   CPort *pClass = (CPort *) p->p;
   pClass->AudioCleanup(p);
}

int CPort::AudioInit()
{
   int Ret = 0;
   int SampleSize = 16;
   int SampleRate;
   int Stereo = 0;
   int Fmt = AFMT_S16_LE;
   int bits;
   audio_buf_info BufInfo;
#ifndef SNDCTL_DSP_SETFRAGMENT
   int BlkSz;
#else
   unsigned long frag_size;
   unsigned long x;
#endif

   do {
      if((RxRing = (int16 *) malloc(RxRingSize*sizeof(int16))) == NULL) {
         LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
         Ret = ERR_MALLOC;
         break;
      }
      DPRINTF(("%s: RxRing @ %p, RxRingSize: %d\n",__FUNCTION__,RxRing,
               RxRingSize));

      if(AudioChannel == 0) {
      // Open audio port in Mono mode
         Stereo = 0;
#ifndef SNDCTL_DSP_SETFRAGMENT
         BlkSz = 1280;
#else
         frag_size = 0x50008;  // 5 x 256 byte fragments
#endif
      }
      else {
      // Check to see if this audio device has been been opened by a prior
      // port
         Ret = FindAudioMaster();
         if(Ret == -1) {
         // We're a slave of another port, clear the error and return
            Ret = 0;
            break;
         }
         else if(Ret != 0) {
         // Error detected
            break;
         }

      // Open audio port in Stereo mode
         Stereo = 1;
#ifndef SNDCTL_DSP_SETFRAGMENT
         BlkSz = 1280*2;
#else
         frag_size = 0xa0008;  // 10 x 256 byte fragments
#endif
         if((StereoInBuf = (int16 *) malloc(AUDIO_BUF_SIZE_BYTES*2)) == NULL) {
            LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
            Ret = ERR_MALLOC;
            break;
         }

         if((StereoOutBuf = (int16 *) malloc(AUDIO_BUF_SIZE_BYTES*2)) == NULL) {
            LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
            Ret = ERR_MALLOC;
            break;
         }
      }

#ifdef USB_SUPPORT
      if(UsbDevType == USB_TYPE_CM108) {
         if(Stereo) {
            LOG_ERROR(("%s: Invalid configuration, the CM108 chip does not "
                       "support stereo input\n",__FUNCTION__));
            Ret = ERR_CONFIG_FILE;
            break;
         }

         LOG_ERROR(("%s: Internal sampling rate conversion for CM108 %sabled\n",
                    __FUNCTION__,
                    DisableRateConversion ? "dis" : "en"));

         if(!DisableRateConversion) {
            bConvertSampleRate = TRUE;
            PCMRate = 48000;
            if(DriverAudioOutDelay < 0) {
               DriverAudioOutDelay = 3;
            }
         }
      }

      if((RxCosMethod == 7 || RxCosMethod ==  8) && CosPollRate == -1) {
      // User didn't specify a CosPollRate, default it to 50 milliseconds
         CosPollRate = 50;
         LOG_ERROR(("%s: set CosPollRate to %d\n",__FUNCTION__,CosPollRate));
      }
#endif

      pAudioC = CreateNewClient();

      if(pAudioC == NULL) {
         Ret = ERR_MALLOC;
         break;
      }

      pAudioC->Cleanup = ::AudioCleanup;
      pAudioC->State = ::Audio_Handler;
      pAudioC->p = this;

      LOG_ERROR(("%s#%d: opening \"%s\"\n",__FUNCTION__,__LINE__,AudioDevice));

      if((pAudioC->Socket = open(AudioDevice,AUDIO_OPEN_FLAGS)) < 0) {
         LOG_ERROR(("%s#%d: open(\"%s\") failed: %s",__FUNCTION__,__LINE__,AudioDevice,
                   Err2String(errno)));
         Ret = ERR_AUDIO_DEV_OPEN;
         break;
      }

      if(ioctl(pAudioC->Socket,SNDCTL_DSP_SETDUPLEX,NULL) == -1) {
      // Failure is not necessarily fatal.
      // FreeBSD doesn't require this call for fullduplex
         LOG_WARN(("%s: SNDCTL_DSP_SETDUPLEX failed: %s",__FUNCTION__,
                   Err2String(errno)));
      }

      if(ioctl(pAudioC->Socket,SNDCTL_DSP_GETFMTS,&bits) == -1) {
         LOG_WARN(("%s: SNDCTL_DSP_GETFMTS failed: %s",__FUNCTION__,
                   Err2String(errno)));
      }
      else if(!(bits & AFMT_S16_LE)) {
         LOG_ERROR(("%s: Error AFMT_S16_LE not supported\n",__FUNCTION__));
         Ret = ERR_AUDIO_FMT;
         break;
      }

      if(ioctl(pAudioC->Socket,SNDCTL_DSP_SAMPLESIZE,&SampleSize) == -1) {
         LOG_ERROR(("%s: SNDCTL_DSP_SAMPLESIZE failed: %s",__FUNCTION__,
                    Err2String(errno)));
         Ret = ERR_AUDIO_FMT;
         break;
      }

      if(ioctl(pAudioC->Socket,SNDCTL_DSP_STEREO,&Stereo) == -1) {
         LOG_ERROR(("%s: SNDCTL_DSP_STEREO failed: %s",__FUNCTION__,
                    Err2String(errno)));
         Ret = ERR_AUDIO_FMT;
         break;
      }

      SampleRate = PCMRate;
      if(ioctl(pAudioC->Socket,SNDCTL_DSP_SPEED,&SampleRate) == -1) {
         LOG_ERROR(("%s: SNDCTL_DSP_SPEED failed: %s",__FUNCTION__,
                    Err2String(errno)));
         Ret = ERR_AUDIO_FMT;
         break;
      }

      LOG_WARN(("%s: Asked for a sampling rate of %d, got %d\n",__FUNCTION__,
                PCMRate,SampleRate));

      if(SampleRate != PCMRate && PCMRate != 48000) {
         if(DisableRateConversion) {
            LOG_ERROR(("%s: Couldn't set sampling rate and rate conversion "
                       "is disabled\n",__FUNCTION__));
            Ret = ERR_AUDIO_FMT;
            break;
         }
      // Try 48k
         SampleRate = PCMRate = 48000;
         if(ioctl(pAudioC->Socket,SNDCTL_DSP_SPEED,&SampleRate) == -1) {
            LOG_ERROR(("%s: SNDCTL_DSP_SPEED failed: %s",__FUNCTION__,
                       Err2String(errno)));
            Ret = ERR_AUDIO_FMT;
            break;
         }
         LOG_WARN(("%s: Asked for a sampling rate of %d, got %d\n",
                   __FUNCTION__,PCMRate,SampleRate));

         if(SampleRate != 48000) {
            LOG_ERROR(("%s: Unable to find supported sampling rate\n",
                       __FUNCTION__));
            Ret = ERR_AUDIO_FMT;
            break;
         }
      }

      if(PCMRate == 48000) {
         bConvertSampleRate = TRUE;
      // Allocate buffers for sampling rate conversion
         if((AudioBuf48Khz = (int16 *) malloc(AUDIO_BUF_SIZE*12)) == NULL) {
            LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
            Ret = ERR_MALLOC;
            break;
         }

         if((AudioInBuf48Khz = (int16 *) malloc(AUDIO_BUF_SIZE*12)) == NULL) {
            LOG_ERROR(("%s#%d: malloc failed\n",__FUNCTION__,__LINE__));
            Ret = ERR_MALLOC;
            break;
         }
         LOG_ERROR(("%s: sampling rate conversion from %d to %d enabled\n",
                    __FUNCTION__,PCMRate,8000));
      }

      if(ioctl(pAudioC->Socket,SNDCTL_DSP_SETFMT,&Fmt) == -1) {
         LOG_ERROR(("%s: SNDCTL_DSP_SETFMT failed: %s",__FUNCTION__,
                    Err2String(errno)));
         Ret = ERR_AUDIO_FMT;
         break;
      }

#ifndef SNDCTL_DSP_SETFRAGMENT
      if(ioctl(pAudioC->Socket,SNDCTL_DSP_SETBLKSIZE,&BlkSz) == -1) {
         LOG_ERROR(("%s: SNDCTL_DSP_SETBLKSIZE failed: %s",__FUNCTION__,
                    Err2String(errno)));
         Ret = ERR_AUDIO_FMT;
         break;
      }
#else
      x = frag_size;
      if(ioctl(pAudioC->Socket,SNDCTL_DSP_SETFRAGMENT,&frag_size) == -1) {
         LOG_ERROR(("%s: SNDCTL_DSP_SETFRAGMENT failed: %s",__FUNCTION__,
                    Err2String(errno)));
         Ret = ERR_AUDIO_SET_FRAG;
         break;
      }
      if(frag_size != x) {
         LOG_ERROR(("%s: requested frag_size 0x%x, got 0x%x\n",__FUNCTION__,
                    x,frag_size));
      }
#endif

      if(ioctl(pAudioC->Socket,SNDCTL_DSP_GETOSPACE,&BufInfo) == -1) {
         LOG_ERROR(("%s: SNDCTL_DSP_GETOSPACE failed: %s",__FUNCTION__,
                    Err2String(errno)));
         if(DriverAudioOutDelay < 0) {
            DriverAudioOutDelay = 80;
         }
      }
      else {
         LOG_ERROR(("Fragsize: %d, fragstotal: %d, bytes: %d\n",
                    BufInfo.fragsize,BufInfo.fragstotal,BufInfo.bytes));

         if(DriverAudioOutDelay < 0) {
         // Calculate the delay in the sound device and driver in milliseconds
            DriverAudioOutDelay = (BufInfo.bytes / 2) * 1000 / PCMRate;
         }
      }
   } while(FALSE);

   return Ret;
}

int CPort::EndPointInit()
{
   int Ret = 0;

   LOG_NORM(("%s: called, TxKeyMethod: %d, RxCosMethod: %d IdNoCTCSS\n",
             __FUNCTION__,TxKeyMethod,RxCosMethod, IdNoCTCSS));

   bEchoIrlpMode = TRUE;

   for( ; ; ) {
   /*
      Sanity check I/O methods

      TxKeyMethod:
       0 - none
       1 - WB2REM / VA3TO style serial EchoLink interface boards
       2 - Parallel port (IRLP interface)
       3 - RTS
       4 - DTR
       5 - USB Audio dongle GPIO
       6 - PCF8754 I2C expander on iMic

// ON1ARF
		7 - /sys/class/gpio/gpioXX/... GPIO writing

   */
      if(TxKeyMethod < 0 || TxKeyMethod > 7) {
         LOG_ERROR(("%s: TxKeyMethod %d is invalid\n",__FUNCTION__,TxKeyMethod));
         Ret = ERR_CONFIG_FILE;
         break;
      };


		// ON1ARF
		// GpioSysClassId should be set (i.e. != -1) for TxKeyMethod 7
		if ((TxKeyMethod == 7) && (GpioSysClassId < 0)) {
         LOG_ERROR(("%s: GpioSysClassId not set for TxKeyMethod 7\n",__FUNCTION__));
         Ret = ERR_CONFIG_FILE;
         break;
		}; // end if


      /*
      RxCosMethod:
         ; 0 - none
         ; 1 - Vox
         ; 2 - Parallel port (IRLP interface)
         ; 3 - serial port CTS
         ; 4 - serial port DSR
         ; 5 - serial port DCD
         ; 6 - device supporting /dev/input such as USB HID (Linux only)
         ; 7 - USB device GPIO (use method 6 for the iMic)
         ; 8 - PCF8754 I2C expander on iMic
      */

      if(RxCosMethod < 0 || RxCosMethod > 8) {
         LOG_ERROR(("%s: RxCosMethod %d is invalid\n",__FUNCTION__,RxCosMethod));
         Ret = ERR_CONFIG_FILE;
         break;
      }

      /*
      RxCtcssMethod:
         ; 0 - none
         ; 1 - Software CTCSS decoder
         ; 2 - Parallel port (IRLP interface)
         ; 3 - serial port CTS
         ; 4 - serial port DSR
         ; 5 - serial port DCD
         ; 6 - device supporting /dev/input such as USB HID (Linux only)
         ; 7 - USB device GPIO (use method 6 for the iMic)
         ; 8 - PCF8754 I2C expander on iMic
      */

      if(RxCtcssMethod < 0 || RxCtcssMethod > 8) {
         LOG_ERROR(("%s: RxCtcssMethod %d is invalid\n",__FUNCTION__,
                    RxCtcssMethod));
         Ret = ERR_CONFIG_FILE;
         break;
      }

   /*
      0 - none
      1 - WB2REM / VA3TO style serial EchoLink interface boards
      2 - IRLP parallel port interface
      3 - Software based DTMF decoder
      4 - Event based DTMF decoding from USB HID device (See DTMF_x_KeyCode)
      5 - Asterisk DTMF tunnelling protocol (Voip ports only)
   */
      if(DtmfMethod < 0 || DtmfMethod > 4) {
         LOG_ERROR(("%s: DtmfMethod %d is undefined or inappropriate for "
                    "physical ports\n",__FUNCTION__,DtmfMethod));
         Ret = ERR_CONFIG_FILE;
         break;
      }

      if(IdNoCTCSS == 1) {
         bPTT_CTCSS = TRUE;
      }
      else if(IdNoCTCSS == 0) {
         bPTT_CTCSS = FALSE;
      }
      else {
         LOG_ERROR(("%s: IdNoCTCSS %d is invalid, valid states are 1 and 0\n",
                    __FUNCTION__,IdNoCTCSS));
      }

      if(TxKeyMethod == 5 || TxKeyMethod == 6) {
#ifdef USB_SUPPORT
         if((Ret = UsbInit()) != 0) {
            break;
         }
#else
         LOG_ERROR(("%s: Error - thelinkbox was not configured with USB support.\n",
                    __FUNCTION__));
         Ret = ERR_USB_DEV_OPEN;
         break;
#endif
      }

      if(RxCosMethod == 6) {
#ifdef __linux__
         if((Ret = InitDeviceInput()) != 0) {
            LOG_ERROR(("%s: Couldn't find /dev/inputX entry for RxCosMethod 6\n",
                       __FUNCTION__));
            break;
         }
#else
         LOG_ERROR(("Error: RxCosMethod 6 is only supported on Linux\n"));
         Ret = ERR_INPUT_DEV_OPEN;
         break;
#endif
      }

      if((Ret = AudioInit()) != 0) {
         break;
      }

   // Set CosBit for serial ports
      switch(RxCosMethod) {
         case 3:  // serial port CTS
            COS_Bit = TIOCM_CTS;
            break;

         case 4:  // serial port DSR
            COS_Bit = TIOCM_DSR;
            break;

         case 5:  // serial port DCD
            COS_Bit = TIOCM_CD;
            break;

         default:
            break;
      }

   // Set CTCSS bit for Serial ports
      switch(RxCtcssMethod) {
         case 3:  // serial port CTS
            CTCSS_Bit = TIOCM_CTS;
            break;

         case 4:  // serial port DSR
            CTCSS_Bit = TIOCM_DSR;
            break;

         case 5:  // serial port DCD
            CTCSS_Bit = TIOCM_CD;
            break;

         default:
            break;
      }

      if((TxKeyMethod == 1 || TxKeyMethod == 3 || TxKeyMethod == 4 ||
          RxCosMethod == 3 || RxCosMethod == 4 || RxCosMethod == 5 ||
          DtmfMethod == 1))
      {  // Need serial port
         if(DevName == NULL) {
            LOG_ERROR(("%s: Error - serial port DevName is not set\n",
                       __FUNCTION__));
            Ret = ERR_CONFIG_FILE;
            break;
         }

         if(TxKeyMethod == 3) {
            PTT_Bit = TIOCM_RTS;
         }
         else if(TxKeyMethod == 4) {
            PTT_Bit = TIOCM_DTR;
         }

         if((pDevice = Device::FindDevice(DevName)) == NULL) {
         // Serial device doesn't exist, create it
            SerialDevice *pSerial = new SerialDevice(DevName);
            if(pSerial == NULL) {
               LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
               Ret = ERR_MALLOC;
               break;
            }
            pDevice = pSerial;
            if((Ret = pSerial->Init()) != 0) {
               break;
            }
         }

         if(TxKeyMethod == 1 || DtmfMethod == 1) {
         // WB2REM / VA3TO
            SerialDevice *pSerial = dynamic_cast<SerialDevice *>(pDevice);
            if(pSerial == NULL) {
               CORE_DUMP();
            }
            if((Ret = pSerial->SetBaudrate(SerialBaudRate)) != 0) {
               break;
            }
         }
      }

      if(TxKeyMethod == 2 || RxCosMethod == 2) {
      // IRLP port needed by TxKeyMethod or RxCosMethod
         if(PTT_Bit == -1) {
            PTT_Bit = IRLP_PTT;
         }

         if(COS_Bit == -1) {
            COS_Bit = IRLP_COS;

         }
         if((Ret = OpenParallelPort()) != 0) {
            break;
         }
      }

      if(DtmfMethod == 3) {
         dtmf_rx_init(&DtmfRxState,NULL,this);
         dtmf_rx_set_realtime_callback(&DtmfRxState,DtmfCallback,this);
         dtmf_rx_parms(&DtmfRxState,0,Twist,ReverseTwist);
      }

      if(RadioCtrlMethod != 0) {
         switch(RadioCtrlMethod) {
            case 1:  // Doug Hall via parallel port
               if((Ret = OpenParallelPort()) != 0) {
                  break;
               }
               break;

            case 2:  // Doug Hall via USB GPIO
            case 3:  // Doug Hall via PCF8754 I2C port expander
            {
#ifdef USB_SUPPORT
               if(udev == NULL) {
                  if((Ret = UsbInit()) != 0) {
                     break;
                  }
               }
               break;
#else
               LOG_ERROR(("%s: Error - RadioCtrlMethod 5 is unavailable because "
                          "thelinkbox was not built with USB support.\n",
                          __FUNCTION__));
               Ret = ERR_USB_DEV_OPEN;
               break;
#endif
            }

            default:
               LOG_ERROR(("%s: Invalid RadioCtrlMethod %d\n",__FUNCTION__,
                          RadioCtrlMethod));
               Ret = ERR_CONFIG_FILE;
               break;
         }

         if(Ret != 0) {
            break;
         }

         pRadioCtrl = new GenericRadioCtrl(NodeName,SendGenericDataCB,this);
         if(pRadioCtrl == NULL) {
            LOG_ERROR(("%s: Error - new GenericRadioCtrl failed\n",
                       __FUNCTION__));
            Ret = ERR_MALLOC;
            break;
         }

         CText Err;
         if((Ret = pRadioCtrl->Init(NodeName,&Err)) != 0) {
            LOG_ERROR(("%s: RadioCtrl init failed - %s.\n",__FUNCTION__,
                       Err.Buf));
            break;
         }

         RxToneDesc = pRadioCtrl->GetRxTone();
         TxToneDesc = pRadioCtrl->GetTxTone();
      }

      if(pAudioC != NULL) {
      // Not a slave channel
         avl_insert(ClientTree,pAudioC);
         pAudioC->bInClientTree = TRUE;
         pAudioC->bReadWait = TRUE;
         GetAudioIn(pAudioC);
      }

      if(RepeaterPort) {
      // This node is a repeater, setup the repeat link
         AddRx(this,true);
         LOG_ERROR(("%s: Setup repeat path on port %s.\n",__FUNCTION__,NodeName));
      }
      break;
   }

   if(Ret != 0) {
   // Init error, clean up
      if(pAudioC != NULL) {
         if(pAudioC->Socket >= 0) {
            close(pAudioC->Socket);
         }
         DeleteClient(pAudioC);
         pAudioC = NULL;
      }

      if(pDevice != NULL) {
         delete pDevice;
      }
   }
   else {
      KeyTx(FALSE);
   }

   LOG_NORM(("%s: returning %d\n",__FUNCTION__,Ret));
   return Ret;
}

int EndPointInit()
{
   int Ret = 0;
   struct avl_traverser avl_trans;
   CPort *pPort;
   Node *pNode = NULL;

   pNode = (Node *) avl_t_first(&avl_trans,NodeTree);
   while(pNode != NULL) {
      pPort = dynamic_cast<CPort *> (pNode);
      if(pPort != NULL) {
         LOG_ERROR(("%s: Initializing %s port\n",__FUNCTION__,pPort->NodeName));
         if((Ret = pPort->EndPointInit()) != 0) {
            break;
         }
         if(pMasterPort == NULL) {
            pMasterPort = pPort;
            CurrentNode = pNode;
         }
      }
      pNode = (Node *) avl_t_next(&avl_trans);
   }


   if(Ret == 0 && pMasterPort == NULL) {
   // Running without any ports.  Startup a timer to poll VoIP clients
      LOG_ERROR(("%s: WARNING no ports defined.\n",__FUNCTION__));
      ClientInfo *pVoipClk = CreateNewClient();
      if(pVoipClk == NULL) {
         LOG_ERROR(("%s: CreateNewClient failed.\n",__FUNCTION__));
      }
      else {
         pVoipClk->Socket = INVALID_SOCKET;
         pVoipClk->State = VoipClock;
         pVoipClk->Cleanup = NULL;
         SetTimeout(pVoipClk,1);
         avl_insert(ClientTree,pVoipClk);
         pVoipClk->bInClientTree = TRUE;
      }
   }
   return Ret;
}

void SetTimeout(struct timeval *pTimer,int Timeout)
{
   long microseconds;

   microseconds = TimeNow.tv_usec + ((Timeout % 1000) * 1000);

   pTimer->tv_usec = microseconds % 1000000;
   pTimer->tv_sec  = TimeNow.tv_sec + (Timeout / 1000) + microseconds / 1000000;
}

void CmdForceID(ClientInfo *p,ConfClient *pCC1,char *Arg)
{
   ClientBufPrint Prn(p);
   CPort *pPort = dynamic_cast<CPort *> (CurrentNode);

   if(pPort != NULL) {
      if(pPort->CWId == NULL) {
         Prn.print("CWID is not set for port %s\r",pPort->NodeName);
         Rcode = TBD_ERR_INVALID_ARG;
      }
      else {
         pPort->SendCwId();
      }
   }
   else {
      Rcode = TBD_PORT_NOT_FOUND;
      Prn.print("Port %s is not a radio port\r",pPort->NodeName);
   }
}

void CPort::CmdSendAx25Beacon(ClientBufPrint *p)
{
   CText Temp;

   do {
      Temp.print("tl %d ax25 ",Ax25ToneLevel);
      if(Ax25BeaconAPRS) {
         AprsVals Vals;

         Vals.Ax25BeaconText = Ax25BeaconText == NULL ?
               ::Ax25BeaconText : Ax25BeaconText;
         Vals.Ax25BeaconPath = Ax25BeaconPath == NULL ?
               ::Ax25BeaconPath : Ax25BeaconPath;
         Vals.TxPower = TxPower < -99.9 ? ::TxPower : TxPower;
         Vals.Haat = Haat < -99.9 ? ::Haat : Haat;
         Vals.AntGain = AntGain < -99.9 ? ::AntGain : AntGain;
         Vals.Frequency = Frequency < -99.9 ? ::Frequency : Frequency;
         Vals.CTCSS = CTCSS < -99.9 ? ::CTCSS : CTCSS;
         Vals.AntDir = AntDir < -99.9 ? ::AntDir : AntDir;
         Vals.RptOffset = RptOffset < -99.9 ? ::RptOffset : RptOffset;
         Vals.Ax25TxDelay = Ax25TxDelay;
         Temp.print("%s",CreateAprsPacket(FALSE,&Vals));
      }
      else if(Ax25BeaconText != NULL) {
         char *cp;
         char BareCall[MAX_CALL_LEN];

         strcpy(BareCall,ConferenceCall);
         if((cp = strchr(BareCall,'-')) != NULL) {
         // remove -L or -R from callsign
            *cp = 0;
         }

         Temp.print("%s>%s %s",BareCall,
                    Ax25BeaconPath == NULL ? "QST" : Ax25BeaconPath,
                    Ax25BeaconText);
      }
      else {
         p->print("Error: Ax25BeaconText is not set for port \"%s\"\r",NodeName);
         Rcode = TBD_ERR_INVALID_ARG;
         break;
      }

      CToneGen *pGenerator = new CToneGen(DefaultToneLevel,Ax25TxDelay,
                                          Ax25ToneLevel);

      if(pGenerator == NULL) {
         LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
         p->print("Error: new failed\r");
         break;
      }
      pGenerator->bIDSource = TRUE; // Don't cause a new ID interval to start
      char *pErrPos;
      if(pGenerator->ToneGenSetup(Temp.Buf,&pErrPos)) {
      // error
         p->print("Error: couldn't parse \"%s\"\r",pErrPos);
         Rcode = TBD_ERR_INVALID_ARG;
         delete pGenerator;
         break;
      }
      AddToneGen(pGenerator);
      LastAx25Beacon = TimeNow.tv_sec;
   } while(FALSE);
}

void CPort::AddTx(LinkedNode *pNewLink)
{
   Node::AddTx(pNewLink);
   pNewLink->RdNdx = RxActiveNdx;
   pNewLink->Flags |= LNK_FLG_FIRST_READ;
}

// Return 0 to continue audio port initialization,
// -1 if we're a slave of another port or error code.
int CPort::FindAudioMaster()
{
   struct avl_traverser avl_trans;
   CPort *pPort;
   Node *pNode;
   int Ret = 0;
   int PortsFound = 0;

   pNode = (Node *) avl_t_first(&avl_trans,NodeTree);
   while(pNode != NULL) {
      pPort = dynamic_cast<CPort *> (pNode);
      if(pPort != NULL && pPort != this &&
         strcmp(AudioDevice,pPort->AudioDevice) == 0)
      {
      // Same audio device
         PortsFound++;
         if(pPort->AudioChannel == 0) {
         // Other port is configured as mono
            LOG_ERROR(("%s: Invalid audio port sharing, port %s is configured "
                       "for mono\n",__FUNCTION__,pPort->NodeName));
            Ret = ERR_AUDIO_DEV_OPEN;
            break;
         }
         else if(pPort->AudioChannel == AudioChannel) {
         // Other port is configured as for the same channel
            LOG_ERROR(("%s: Invalid audio port sharing, ports %s and %s are "
                       "configured for the same channel\n",__FUNCTION__,
                       NodeName,pPort->NodeName));
            Ret = ERR_AUDIO_DEV_OPEN;
            break;
         }
         else {
            if(pPort->pAudioC != NULL) {
            // We're his slave
               pPort->pAudioSlave = this;
               pAudioMaster = pPort;
               Ret = -1;
            }
         }
      }
      pNode = (Node *) avl_t_next(&avl_trans);
   }

   if(PortsFound > 1) {
      LOG_ERROR(("%s: Invalid audio port sharing, more than two ports share "
                 "%s\n",__FUNCTION__,pPort->AudioDevice));
      Ret = ERR_AUDIO_DEV_OPEN;
   }
   return Ret;
}

int CPort::SetSoftRxTone(int ToneNum)
{
   int Ret = 0;

   do {
      bRxCtcssSearch = bRxCtcssAny = FALSE;
      RxCTCSSTone = ToneNum;
      if(ToneNum == -1) {
      // Disable CTCSS
         if(CtcssDecoderState != NULL)  {
            CtcssDestroy(CtcssDecoderState);
            CtcssDecoderState = NULL;
         }
         RxToneDesc = "carrier";
      }
      else {
         if(CtcssDecoderState == NULL) {
            if((CtcssDecoderState = CtcssInit()) == NULL) {
               LOG_ERROR(("%s: CtcssInit() failed\n",__FUNCTION__));
               Ret = ERR_CTCSS_RX_INIT;
               break;
            }
         }
         EnableCtcssTone(-1,CtcssDecoderState); // Disable all tones
      // Enable the selected tone plus the guard tones
         if(ToneNum != 0) {
            EnableCtcssTone(ToneNum - 1,CtcssDecoderState);
         }
         if(ToneNum + 1 < CTCSS_NUM_CODES) {
            EnableCtcssTone(ToneNum + 1,CtcssDecoderState);
         }
         EnableCtcssTone(ToneNum,CtcssDecoderState);
         RxToneDesc = freq_ctcss[ToneNum];
      }
   } while(FALSE);
   return Ret;
}

//
int CPort::SetRxTone(char *Arg,CText *p)
{
   int CtcssFreq = -99;
   float CtcssFloat;
   int i;
   int bToneFreqSet = FALSE;

   do {
      if(*Arg == 0) {
      // No argument
         p->print("RxTone %s\n",(char *)RxToneDesc);
         break;
      }

      if(RxCtcssMethod == 1) {
      // the software decoder can do some special tricks!
         int bSearch = FALSE;
         int bAny = FALSE;
         if((bAny = (STRCMP(Arg,"any") == 0)) ||
            (bSearch = (STRCMP(Arg,"search") == 0)))
         {
            if(CtcssDecoderState == NULL) {
               if((CtcssDecoderState = CtcssInit()) == NULL) {
                  p->print("Error: CtcssInit() failed\n");
                  LOG_ERROR(("%s: CtcssInit() failed\n",__FUNCTION__));
                  break;
               }
            }
            RxToneDesc = Arg;
            EnableCtcssTone(-2,CtcssDecoderState); // enable all tones
            bRxCtcssSearch = bSearch;
            bRxCtcssAny = bAny;
            break;
         }
         if(strchr(Arg,'.') != NULL) {
         // There's a decimal point, must be a CTCSS frequency
            if(sscanf(Arg,"%f",&CtcssFloat) == 1) {
               bToneFreqSet = TRUE;
               CtcssFreq = (int) ((CtcssFloat * 10.0) + .5);
            }
         }
         else {
         // decimal (freq x 10) CTCSS frequency or (eventually) DCS code
            if(sscanf(Arg,"%d",&CtcssFreq) == 1) {
               bToneFreqSet = TRUE;
            }
         }

         if(!bToneFreqSet) {
            Rcode = TBD_ERR_INVALID_ARG;
            p->print("Unable to parse RxTone \"%s\" for Port %s\n",Arg,NodeName);
            break;
         }

         if(CtcssFreq == 0) {
         // disable CTCSS
            i = -1;
         }
         else {
         // Find Ctcss tone number
            for(i = 0; i < CTCSS_NUM_CODES; i++) {
               if(CtcssFreq == Freq2ToneNum[i]) {
               // Found it
                  break;
               }
            }
         }

         if(i == CTCSS_NUM_CODES) {
            Rcode = TBD_ERR_INVALID_ARG;
            p->print("Invalid RxTone \"%s\" for Port %s\r",Arg,NodeName);
            break;
         }
         SetSoftRxTone(i);
         break;
      }

      if(pRadioCtrl == NULL) {
         Rcode = TBD_ERR_INVALID_ARG;
         p->print("RxCtcssMethod %d for port %s, does not support "
                  "RxTone selection\n",RxCtcssMethod,NodeName);
         break;
      }
      Rcode = pRadioCtrl->SetRxTone(Arg,p);
      if(Rcode == 0) {
         RxToneDesc = pRadioCtrl->GetRxTone();
      }
   } while(FALSE);

   return Rcode;
}

int CPort::SetTxTone(char *Arg,CText *p)
{
   float CtcssFloat;

   do {
      if(*Arg == 0) {
      // No argument
         p->print("TxTone %s\n",(char *)TxToneDesc);
         break;
      }

      if(TxCtcssMethod == 1) {
         if(sscanf(Arg,"%f",&CtcssFloat) == 1) {
            if(CtcssFloat == 0.0) {
            // disable CTCSS
               CtcssTxLevel = 0;
               TxToneDesc = strdup("disabled");
            }
            else {
               CtcssTxLevel = CTCSSLevel;
               CTCSSAngleDelta = 2 * M_PI * CtcssFloat / 8000.0;
               CText Temp;
               Temp.print("%3.1f",CtcssFloat);
               TxToneDesc = strdup(Temp.Buf);
            }
         }
         else {
            Rcode = TBD_ERR_INVALID_ARG;
            p->print("Unable to parse TxTone \"%s\" for Port %s\n",Arg,NodeName);
         }
      }
      else if(pRadioCtrl == NULL) {
         Rcode = TBD_ERR_INVALID_ARG;
         p->print("Port %s does not support TxTone selection\n",NodeName);
      }
      else {
         Rcode = pRadioCtrl->SetTxTone(Arg,p);
         if(Rcode == 0) {
            TxToneDesc = pRadioCtrl->GetTxTone();
         }
      }
   } while(FALSE);

   return Rcode;
}


// setfrequency rxfrequency[,txoffset[,txtone[,rxtone]]]
// if txoffset isn't specified then simplex
// if txtone isn't specified then no tone
// if rxtone isn't specified then carrier squelch
int CPort::SetFrequency(char *Arg,CText *p)
{
   char *TxOff = NULL;
   char *TxTone = NULL;
   char *RxTone = NULL;
   char Zero[] = "0";

   do {
      if(pRadioCtrl == NULL) {
         Rcode = TBD_ERR_INVALID_ARG;
         p->print("Error: No RadioCtrlMethod was specified for port %s\r",
                   NodeName);
         break;
      }

      if(*Arg == 0) {
      // No argument
         p->print("Frequency: %3.3lf, %1.3f, %s, %s\r",RxFreq,TxOffset,
                  (char *)TxToneDesc,(char *)RxToneDesc);
         break;
      }

      if((TxOff = strchr(Arg,',')) != NULL) {
         *TxOff++ = 0;
         while(isspace(*TxOff)) {
            TxOff++;
         }
         if((TxTone = strchr(TxOff,',')) != NULL) {
            *TxTone++ = 0;
            while(isspace(*TxTone)) {
               TxTone++;
            }
            if((RxTone = strchr(TxTone,',')) != NULL) {
               *RxTone++ = 0;
               while(isspace(*RxTone)) {
                  RxTone++;
               }
            }
         }
      }


      if(SetRxFreq(Arg,p) == 0 &&
         SetTxOffset(TxOff != NULL ? TxOff : Zero,p) == 0 &&
         SetTxTone(TxTone != NULL ? TxTone : Zero,p) == 0 &&
         SetRxTone(RxTone != NULL ? RxTone : Zero,p) == 0)
      {  // Everyone was happy sync with the hardware
         SyncRadioCtrl();
      }
   } while(FALSE);

   return Rcode;
}

int CPort::SetRxFreq(char *Arg,CText *p)
{
   double FreqFloat;

   do {
      if(*Arg == 0) {
      // No argument
         p->print("RxFreq %3.3lf\n",RxFreq);
         break;
      }

      if(sscanf(Arg,"%lf",&FreqFloat) != 1) {
         Rcode = TBD_ERR_INVALID_ARG;
         p->print("Unable to parse RxFreq \"%s\"\r",Arg);
         break;
      }

      if(pRadioCtrl == NULL) {
         Rcode = TBD_ERR_INVALID_ARG;
         p->print("Error: No RadioCtrlMethod was specified for port %s\r",
                   NodeName);
         break;
      }

      if(pRadioCtrl->SetRxFrequency(FreqFloat,p) == 0) {
         RxFreq = FreqFloat;
      }
      else {
         Rcode = TBD_ERR_INVALID_ARG;
         if(p->IsEmpty()) {
            p->print("Invalid frequency\r");
         }
      }
   } while(FALSE);
   return Rcode;
}


int CPort::SetTxOffset(char *Arg,CText *p)
{
   double FreqFloat;

   do {
      if(*Arg == 0) {
      // No argument
         p->print("TxOffset %1.3f\n",TxOffset);
         break;
      }

      if(sscanf(Arg,"%lf",&FreqFloat) != 1) {
         Rcode = TBD_ERR_INVALID_ARG;
         p->print("Unable to parse TxOffset \"%s\"\r",Arg);
         break;
      }

      if(pRadioCtrl == NULL) {
         Rcode = TBD_ERR_INVALID_ARG;
         p->print("Error: No RadioCtrlMethod was specified for port %s\r",
                   NodeName);
         break;
      }

      if(pRadioCtrl->SetTxOffset(FreqFloat,p) == 0) {
         TxOffset = FreqFloat;
      }
      else {
         Rcode = TBD_ERR_INVALID_ARG;
         if(p->IsEmpty()) {
            p->print("Invalid frequency\r");
         }
      }
   } while(FALSE);

   return Rcode;
}

int CPort::SetTxPower(char *Arg,CText *p)
{
   if(pRadioCtrl == NULL) {
      Rcode = TBD_ERR_INVALID_ARG;
      p->print("Error: No RadioCtrlMethod was specified for port %s\r",
                NodeName);
   }
   else if(*Arg == 0) {
   // No argument
      p->print("Tx power is %s\n",pRadioCtrl->GetTxPower());
   }
   else {
      if(pRadioCtrl->SetTxPower(Arg,p)) {
         Rcode = TBD_ERR_INVALID_ARG;
      }
   }

   return Rcode;
}


void CPort::SyncRadioCtrl()
{
   if(pRadioCtrl != NULL) {
      pRadioCtrl->Sync();
   }
}


void CPort::SendGenericDataParallel(unsigned char *Data,int Len)
{
   ParallelDevice *pParallel = dynamic_cast<ParallelDevice *>(pDevice);

   if(pDevice == NULL || pParallel == NULL) {
      LOG_ERROR(("%s: ERROR: %sdevice not open\n",__FUNCTION__,
                 pDevice == NULL ? "" : "parallel "));
   }
   else {
      pParallel->SendGenericData(Data,Len,DH_DataBit,DH_ClockBit);
   }
}

void CPort::SendGenericData(unsigned char *Data,int Len)
{
   int Elapsed;
   if((Elapsed = TimeLapse(&LastGenericDataSent)) < DH_PACKET_DELAY) {
   // Can't send yet, queue it for later

      if(Debug & DLOG_RIG_CTRL) {
         LOG_ERROR(("%s: Too soon to send, last update was %d milliseconds ago\n",
                    __FUNCTION__,Elapsed));
      }

      GenericData *pDataSave = new GenericData;
      if(pDataSave != NULL) {
         pDataSave->Link = NULL;
         memcpy(pDataSave->Data,Data,Len);
         pDataSave->Len = Len;
         if(GenericDataHead == NULL) {
            GenericDataHead = pDataSave;
            GenericDataTail = pDataSave;
         }
         else {
            GenericDataTail->Link = pDataSave;
         }
      }
   }
   else {
      LastGenericDataSent = TimeNow;
      if(RadioCtrlMethod == 1) {
         SendGenericDataParallel(Data,Len);
      }
#ifdef USB_SUPPORT
      else if(RadioCtrlMethod == 2 || RadioCtrlMethod == 3) {
      // Doug Hall via USB
         SendGenericDataUSB(Data,Len);
      }
#endif
      else {
         LOG_ERROR(("%s: Error: %s called with RadioCtrlMethod %d\n",
                    __FUNCTION__,RadioCtrlMethod));
      }
   }
}

int SendGenericDataCB(unsigned char *Data,int Len,void *pPriv)
{
   CPort *pPort = (CPort *) pPriv;

   if(pPort != NULL) {
      pPort->SendGenericData(Data,Len);
   }
   else {
      LOG_ERROR(("%s: Error: pPriv == NULL !\n",__FUNCTION__));
   }
   return 0;
}

int VoipClock(ClientInfo *p)
{
   int TotalDelta = 0;
   int ThisDelta;
   int NewDelay;
   int Polls = 0;
   static int TotalPolls = 0;
   static int TotalNextPolls = 0;
   static int Timeout = 80;
   static struct timeval TimeFirstPoll = {0};
   static struct timeval TimeNextFirstPoll = {0};
   static struct timeval TimeLastPoll = {0};
   static int bTooSlowLogged = FALSE;

   if(TimeFirstPoll.tv_sec != 0) {
      TotalDelta = ((TimeNow.tv_sec - TimeFirstPoll.tv_sec) * 1000) +
                   ((TimeNow.tv_usec - TimeFirstPoll.tv_usec) / 1000);
      if(TotalDelta > 5 * 60 * 1000 && TimeNextFirstPoll.tv_sec == 0) {
      // we've been running with this average for more than 5 minutes,
      // start a new running average
         TimeNextFirstPoll = TimeNow;
         TotalNextPolls = 0;
      }

   // Call all VoIP client's PollTx function once every 80 milliseconds
      Polls = (TotalDelta / 80) - TotalPolls;

   // Adjust the next Timeout
      ThisDelta = ((TimeNow.tv_sec - TimeLastPoll.tv_sec) * 1000) +
                  ((TimeNow.tv_usec - TimeLastPoll.tv_usec) / 1000);

      D3PRINTF(("%s: Last delta %d milliseconds.\n",__FUNCTION__,ThisDelta));

      Timeout -= ((ThisDelta - 80)/2);
      if(Timeout <= 0) {
      // Hmmm not keeping up... but we can't make time go backwards!
         Timeout = 1;
         if(!bTooSlowLogged) {
            bTooSlowLogged = TRUE;
            LOG_ERROR(("%s: Desired timeout reached zero after %d polls.\n",
                       __FUNCTION__,TotalPolls));
         }
      }
   }
   else {
   // First wakeup
      Polls = 1;
      TimeFirstPoll = TimeNow;
   }

   if(Polls <= 0) {
      Polls = 0;
      NewDelay = ((TotalPolls + 1) * 80) - TotalDelta;
      if(NewDelay <= 0) {
      // Hmmm again !
         NewDelay = 1;
      }
      SetTimeout(p,NewDelay);
      D3PRINTF(("%s: Too soon to poll, waiting %d milliseconds.\n",
                __FUNCTION__,NewDelay));
   }
   else {
      if(Polls > 1) {
         D3PRINTF(("%s: Polling %d times.\n",__FUNCTION__,Polls));
      }
      TimeLastPoll = TimeNow;
      TotalPolls += Polls;
      TotalNextPolls += Polls;

      struct avl_traverser avl_trans;
      VoipNode *pVoip;
      Node *pNode = NULL;

      if(avl_count(NodeTree) > 0) {
         pNode = (Node *) avl_t_first(&avl_trans,NodeTree);
      }
      while(pNode != NULL) {
         pVoip = dynamic_cast<VoipNode *> (pNode);
         if(pVoip != NULL) {
            for(int i = 0; i < Polls; i++) {
               pVoip->PollTx();
            }
         }
         pNode = (Node *) avl_t_next(&avl_trans);
      }
   }

   if(TotalDelta > 10 * 60 * 1000) {
   // we've been running with this average for more than 10 minutes, switch
   // to the new running average
      TimeFirstPoll= TimeNextFirstPoll;
      TotalPolls = TotalNextPolls;
      TimeNextFirstPoll.tv_sec = 0;
   }

   return FALSE;
}

int CPort::OpenParallelPort()
{
   int Ret = 0;

   if(DevName == NULL) {
   // Use the default device name if user didn't provide one
      DevName = strdup(IRLP_DEVICE);
   }

   if(pDevice != NULL && strcmp(pDevice->DeviceName,DevName) != 0) {
      LOG_ERROR(("%s: Invalid mixing of serial and parallel I/O methods, "
                 "port %s\n",__FUNCTION__,NodeName));
      Ret = ERR_CONFIG_FILE;
   }
   else if((pDevice = Device::FindDevice(DevName)) == NULL) {
   // Parallel device doesn't exist, create it
      ParallelDevice *pParallel = new ParallelDevice(DevName);
      if(pParallel == NULL) {
         LOG_ERROR(("%s#%d: new failed\n",__FUNCTION__,__LINE__));
         Ret = ERR_MALLOC;
      }
      else {
         if((Ret = pParallel->Init()) == 0) {
				pDevice = pParallel;
			}
			else {
				delete pParallel;
			}
      }
   }

   return Ret;
}

void CPort::checkCTCSS(void) 
{
   if(PTT_CTCSS_Bit != -1) {
      if((bPTT_CTCSS && !InvertPTT_CTCSS) || (!bPTT_CTCSS && InvertPTT_CTCSS)) {
         pDevice->ChangeBit(PTT_CTCSS_Bit,TRUE);
      }
      else {
         pDevice->ChangeBit(PTT_CTCSS_Bit,FALSE);
      }
   }
}
