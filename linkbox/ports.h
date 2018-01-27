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

   $Log: ports.h,v $
   Revision 1.27  2013/05/13 16:41:06  beta2k

   Added patches from Kristoff - ON1ARF for sysfs GPIO access for PTT

   Revision 1.26  2010/11/20 15:16:33  wb6ymh
   Added SetTxPower, Write8754 prototypes.

   Revision 1.25  2009/09/13 19:23:16  wb6ymh
   1. Added NoAudioInLooping, support for floating point CTCSS filter.
   2. Deleted ActivePorts prototype.

   Revision 1.24  2009/07/07 02:32:43  beta2k
   Removed Aux_PTT_Bit, IdAuxPTT, InvertAuxPTT and replaced with IdNoCTCSS, PTT_CTCSS_Bit, InvertPTT_CTCS

   Revision 1.23  2009/07/03 17:39:43  wb6ymh
   Added NoHangtimeOnID.

   Revision 1.22  2009/06/26 02:38:48  beta2k
   Added Aux_PTT_Bit, bAuxPTT, IdAuxPTT and InvertAuxPTT to allow for the IDer to be keyed via a different pin then the main PTT

   Revision 1.21  2009/05/25 16:18:40  wb6ymh
   1. Moved DTMF processing to after receiver audio has been deemphasized.
   2. Added CTCSS high pass filter before DTMF decoder.

   Revision 1.20  2009/05/22 19:04:23  beta2k
   Added HamlibPortName, HamlibIPAddress, HamlibDaemon, HamlibDaemonArgs, HamlibRigModel, HamlibPortSpeed, HamlibCIVAddr; HamlibTCPLocal, HamlibTCPPort variables for hamlib support

   Revision 1.19  2009/05/22 13:39:17  wb6ymh
   Added CosPollRate to give the ability to reduce reduce the polling rate where
   it matters.  Fixes badly broken audio when an CM108 based interface is used
   on a "Little Blue" patform (677 Mhz Via CPU and USB 1.1 ports).

   Revision 1.18  2009/04/04 16:22:38  wb6ymh
   Deleted obsolete ActiveRxMap.

   Revision 1.17  2009/03/28 15:57:21  wb6ymh
   Change from va3jss: SerialDevName -> DevName.

   Revision 1.16  2009/02/14 15:42:40  wb6ymh
   1. Massive restructuring of the audio buffering algorithm to minimize port to
      port audio delays.
   2. Added tone burst support.
   3. Replaced orignal cw ID code with tone generator based CW to simplify things.
   4. Completed debugging and implementation of stereo support.
   5. Restructured code to eliminate the KickStartAudioOut kludge.
   6. Removed untested support for 8 bit sound cards.
   7. Made DumpStatus a Node method rather than a CPort only method.

   Revision 1.15  2009/01/03 23:35:46  wb6ymh
   1. Added KickStartAudioOut, CmdSendAx25Beacon, Ax25* configuration variables,
      LinkVoipIn, LinkVoipOut, per port APRS veraibles, LastAudioOutTime,
      LastStereoOutTime, pAudioMaster, pDevice, bWroteAudioOut, OpenParallelPort,
      CheckUnkey and DoPolls to CPort class.
   2. Deleted pSerialSlave, pParallelSlave, SerialFD, SeralInit,
      FindSerialMaster and SpinSleep from CPort class.

   Revision 1.14  2008/07/23 16:41:56  wb6ymh
   1. Changed DumpStatus argument from a ClientInfo * to a CText *.
   2. Added MaxSamples and bForce arguments to GetRxAudio.

   Revision 1.13  2008/06/26 17:35:40  wb6ymh
   1. Added return code to SetRxTone, SetTxTone, SetFrequenyc, SetTxOffset.
   2. Added SetFrequency, syncRadioCtrl, LastGenericDataSent, GenericDataHead,
      GenericDataTail and SpinSleeep.

   Revision 1.12  2008/05/14 21:38:24  wb6ymh
   1. Added SetRxTone, SetTxTone, SetRxFreq, SetTxOffset, SendGenericData,
      TxCtcssMethod, RadioCtrlMethod, DH_DataBit, DH_ClockBit, TxToneDesc,
      pRadioCtrl, UsbGPIO, RxFreq, SendGenericDataUSB, SendGenericDataParallel,
      UsbShutdown.
   2. Changed RxToneDesc to a CText class.

   Revision 1.11  2008/04/25 05:23:23  wb6ymh
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

   Revision 1.10  2008/03/11 14:12:13  wb6ymh
   Added AddTx override.

   Revision 1.9  2008/03/09 17:41:54  wb6ymh
   Deleted CWToneFreq.

   Revision 1.8  2008/03/09 17:03:53  wb6ymh
   Massive restructuring: moved most CPort members to new Nodes class. All
   that remains are variables specific to physical ports.

   Revision 1.7  2008/01/28 15:39:59  wb6ymh
   1. Added support for external CTCSS decoders controlled by configuration
      variables RxCtcssMethod, RxCtCSSMode, InverCTCSS, and DtfmCTCSSMode.
   2. Corrected bug in CPort::Shutup that prevented it from ever doing anything.

   Revision 1.6  2008/01/13 17:06:17  wb6ymh
   1. Added RxTimeout, TxTimeoutTone, DtmfConverTone, bRxTimeout, bEvenRxBuf,
      TossRxIn, AudioInBuf15, RxActiveTime.
   2. Modified CancelToneGen to return status.
   3. Added bDelayed argument to SendCtone.

   Revision 1.5  2007/12/27 17:58:10  wb6ymh
   1. Added DumpStatus class member.
   2. Added MinDtmfInterdigitTime, Min1stDigitHoldTime and Dtmf1stDigits
      variables.

   Revision 1.4  2007/12/14 23:28:21  wb6ymh
   Moved globals AudioBuf, AudioAvailable, TxSequency, RtpTxSequence,
   DtmfRxState and all GSM variables into CPort class.

   Revision 1.3  2007/12/07 16:57:23  wb6ymh
   1. Moved global ADPCM variables into CPort class.
   2. Added prototypes for CancelToneGen, SourcesReady, and GetClientAudio.
   3. Added TxTimeoutToneGen, CtoneResetsTimeout, and b8BitFile variables.

   Revision 1.2  2007/12/01 06:03:49  wb6ymh
   1. Added include for new ToneGen.h.
   2. Deleted GenTestSweep.
   3. Added class members and variables to implement courtesy tone, hangtime
      and transmitter timeout functions.

   Revision 1.1  2007/11/26 15:19:55  wb6ymh
   Inital import - header for CPort class.

*/

#ifndef _PORTS_H_
#define _PORTS_H_

#include "radio_ctrl.h"
#include "devices.h"

#define MAX_PORTS 32


class CPort : public Node
{
public:
// public functions
   CPort();
   ~CPort();
// entry points from C code
   int EndPointInit();
   int WriteAudioOut(ClientInfo *p);
   int Audio_Handler(ClientInfo *p);
   void GetAudioIn(ClientInfo *p);
   void AudioCleanup(ClientInfo *p);
   int DeviceInHandler(ClientInfo *p);
   int PausePlayback();
   int ResumePlayback();
// Return number of samples of audio that are available
   int SamplesAvailable(LinkedNode *pLink,int bForce);
// Get receiver audio.  Return number of samples available @ pAudio.
// Ret < 0 means 32 bits samples, otherwise 16 bit samples
   int GetRxAudio(LinkedNode *pLink,void **pAudio,int MaxSamples,int bForce);
   void AddTx(LinkedNode *pLink);
   int SetRxTone(char *Arg,CText *p);
   int SetTxTone(char *Arg,CText *p);
   int SetFrequency(char *Arg,CText *p);
   int SetRxFreq(char *Arg,CText *p);
   int SetTxOffset(char *Arg,CText *p);
   int SetTxPower(char *Arg,CText *p);
   void SyncRadioCtrl();
   void SendGenericData(unsigned char *Data,int Len);
   void CmdSendAx25Beacon(ClientBufPrint *p);
   void SendCwId();

// Public variables

// variables accessed by Node class
   ClientInfo *pAudioC;

// Config variables
   int VoxThreshold;
   int VoxHoldTime;
   int VoxTripDelay;
   int TxKeyMethod;
// ON1ARF
	int GpioSysClassId;
   int RxCosMethod;
   int RxCtcssMethod;
   int TxCtcssMethod;
   int RadioCtrlMethod;
   int SerialBaudRate;
   int DTMFPollTime;
   int InvertAuxPTT;
   int InvertPTT;
   int InvertPTT_CTCSS;
   int InvertCOS;
   int InvertCTCSS;
   int IdInterval;
   int IdNoCTCSS;
   int VoiceIDIdleTime;
   int HalfDuplexRF;
   int PCMRate;
   int CtcssDecoderSamples;
   int bMonitor;
   int COS_KeyCode;
   int bDiscFeed;
   int bUngatedRx;
   int PTT_CTCSS_Bit;
   int PTT_Bit;
   int COS_Bit;
   int CTCSS_Bit;
   int DH_DataBit;
   int DH_ClockBit;
   int HangTime;
   int RxCTCSSMode;
   int CTCSS_KeyCode;
   int AudioChannel;
   int RxCTCSSTone;  // Tone index or -1 for carrier
   int CtcssRxThreshold;
   int DisableRateConversion;
   int Ax25BeaconInterval;
   int Ax25ToneLevel;
   int Ax25BeaconAPRS;
   int Ax25TxDelay;
   int LinkVoipIn;
   int LinkVoipOut;
   int MinAudioWrite;
   int DriverAudioOutDelay;
   int RepeaterPort;
   int EnableHamlib;
   int HamlibRigModel;
   int HamlibPortSpeed;
   int HamlibCIVAddr;
   int HamlibTCPLocal;
   int HamlibTCPPort;
   int CosPollRate;
   int EnableDtmfCtcssFilter;
   int NoHangtimeOnID;
   int NoAudioInLooping;

   float RxFrequency;
   float TxTone;
   float RxTone;
   float TxOffset;
   float TxPower;
   float Haat;
   float AntGain;
   float Frequency;
   float CTCSS;
   float AntDir;
   float RptOffset;

   char *DevName;
   char *CWId;
   char *VoiceID;
   char *AudioDevice;
   char *AudioDongleSN;
   char *AudioDongleAdr;
   char *Ax25BeaconText;
   char *Ax25BeaconPath;
   char *HamlibPortName;
   char *HamlibIPAddress;
   char *HamlibDaemon;
   char *HamlibDaemonArgs;
   CText RxToneDesc;
   CText TxToneDesc;

private:
   struct timeval LastCOSPoll;
   struct timeval LastSpoke;
   struct timeval LastVoxTrip;
   struct timeval LastDTMFPoll;
   struct timeval HangTimer;
   struct timeval CtcssDecodeTime;
   struct timeval LastGenericDataSent;
   struct timeval LastAudioOutTime;
   struct timeval LastStereoOutTime;
   time_t LastID;
   time_t LastAx25Beacon;

   int bTalking;     // receiving signal from RF user
   int bTxIsKeyed;   // WB2REM serial box PTT has been activated
   int bPTT_CTCSS;   // CTCSS Mute
   int bDevInCOS;
   int bDevInCTCSS;
   int TossRxIn;
   int bCosActive;

   int16 *StereoInBuf;
   int16 *StereoOutBuf;
   int16 *AudioBuf48Khz;
   int16 *AudioInBuf48Khz;
   int16 *AudioOutBuf;
   CPort *pAudioSlave;
   CPort *pAudioMaster;
   RadioCtrl *pRadioCtrl;
   GenericData *GenericDataHead;
   GenericData *GenericDataTail;
   void *CtcssDecoderState;
   int CurrentRxCTCSS;
   int DCTemp[4];
   int UCTemp[4];
   int CtcssTemp[4];
#ifdef FLOAT_CTCSS_FILTER
   double DtmfCtcssFilterTemp[8];
#else   
   int DtmfCtcssFilterTemp[8];
#endif
   int16 UCLastSample;
   char UsbGPIO[4];

   time_t RxActiveTime;

   long int PausedOffset;
   char *PausedFilename;
   double RxFreq;
#ifdef USB_SUPPORT
   usb_dev_handle *udev;
   int UsbDevType;
#endif
   int bRxCtcssSearch;
   int bRxCtcssAny;
// receiver audio ring buffer
   int16 *RxRing;
   int16 *AudioInPtr;   // write index into RxRing

   int RxRingSize;      // length of the ring buffer in samples
   int WrNdx;           // write index into RxRing
   int RxActiveNdx;     // WrNdx the last time the receiver was active
   int VoxSamples;
   int VoxAveTemp;
   int VoxAveLevel;
   int WriteBufferSize;    // in bytes
   int WriteBufferLen;     // in bytes
   int AudioIn48;

   Device *pDevice;
   int bConvertSampleRate:1;

   void KeyTx(int bKey);
   void ID();
   void FillAudioBuf();
   void Vox(int Samples);
   void ProcessAudioIn(int Samples);
   void PollCOS();
   void PollDTMF();
   void FillCTCSSBuf();
   int InitDeviceInput();
   int UsbInit(void);
   void StartTalking(int bTalk);
   void UsbKeyRadio(int bKey);
   int UsbPollGPIO();
   int SourcesReady(ConfClient **pTalker);
   int VoipAudioAvailable(ConfClient *pCC);
   int GetClientAudio(ConfClient *pCC);
   int AudioInit(void);
   int FindAudioMaster();
   void DownConvert(int Samples);
   void UpConvert(int Samples);
   int SetSoftRxTone(int ToneNum);
   void SendGenericDataUSB(unsigned char *Data,int Len);
   void SendGenericDataParallel(unsigned char *Data,int Len);
   void UsbShutdown();
   int OpenParallelPort();
   void CheckUnkey();
   void DoPolls();
   void checkCTCSS(void);
   void Write8754(char Data);

// ON1ARF
// gpiosc = GPIO /sys/class/gpio methode of accessing GPIO pins

// Use the /sys/class/gpio kernel-drivers to do GPIO operations.
// The application assumes that the kernel-driver filestructure has
// been correctly initialised and preconfigured

// E.g. on a raspberry pi, the initialion to write data to GPIO pin 17
// (i.e. the 11th (!!!) pin of the GPIO pins of the RPi) requires root
// priviledges and can be does as follows:

// # echo "17" > /sys/class/gpio/export
// (This will create a directory /sys/class/gpio/gpio17)
// # echo "out" > /sys/class/gpio/gpio17/direction
// # chmod 666 /sys/class/gpio/gpio17/value

// Note, if the proper file-priviledges are set on
// /sys/class/gpio/gpioXX/value, reading or writing gpio data
// does not require root access

// The status of the GPIO pin can be set as follows:
// $ echo "0" > /sys/class/gpio/gpio17/value
// $ echo "1" > /sys/class/gpio/gpio17/value

// to read / write gpio pins of a file that is already open, first
// reset the file-pointer to the beginning of the file using
// lseek(fd,0,FD_SET);

	int gpiosc_init;
	char gpiosc_fname[40];
	int gpiosc_fd;


};

extern Node *CurrentNode;
extern struct avl_table *NodeTree;   // sorted by node name


#endif   // _PORTS_H_

