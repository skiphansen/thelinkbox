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
   
   $Log: usbdev.cpp,v $
   Revision 1.13  2012/12/09 16:36:03  wb6ymh
   1. Added new USB device ID for URI (CM108AH) courtesy of Roy K6MF.
   2. Removed some debug logging in Write8754 (I2C port expander).

   Revision 1.12  2010/11/20 15:19:30  wb6ymh
   Corrected bug in last commit: Dont' set CTCSS_BIT unless RxCTCSSMethod is
   set, AND set CTCSS_BIT rather than COS_Bit for UAC3556 case.

   Revision 1.11  2010/11/17 04:54:28  wb6ymh
   1. Added PCF8754 support.
   2. Added default support for the CTCSS input on the URI courtesy of Jim WB6NIL.

   Revision 1.10  2010/02/23 01:07:07  wb6ymh
   Added support for the CM108AH found in recent URI dongles.

   Revision 1.9  2010/01/07 18:52:07  wb6ymh
   Added CM109/CM119 device ID seen on some cheap USB dongles that were
   advertised as being CM108 based.  Turns out the GPIO interface on the
   CM109/CM119 devices is compatible with CM108s anyway.

   Revision 1.8  2009/01/03 22:56:31  wb6ymh
   Cosmetic: delete some old #ifdef'ed out dead code.

   Revision 1.7  2008/05/14 21:43:00  wb6ymh
   1. Modified USB PTT routine to use the PTT_Bit and InvertPTT configuration
      file variables.  Previously the USB GPIO bits were hardcoded for the
      "standard" bits.
   2. Added support for the iMic to UsbPollGPIO.  NB: do not use!  Causes
      kernel panics currently.  Old Event code interface should be used
      intead until this is resolved.
   3. Changed some char * to const char * to fix compile problems with GCC 4.2.x.
   4. Added UsbShutdown,and SendGenericDataUSB..

   Revision 1.6  2008/03/09 17:05:05  wb6ymh
   1. Added dmalloc support.
   2. PortName -> NodeName.

   Revision 1.5  2008/01/28 15:43:15  wb6ymh
   Renamed UsbPollCOS to UsbPollGPIO and modified it to return raw GPIO bits.

   Revision 1.4  2007/12/14 23:30:02  wb6ymh
   1. Added display of port name to audio device identification log entry.
   2. Corrected a couple of warnings.

   Revision 1.3  2007/12/07 16:59:19  wb6ymh
   Applied fix from wb5tts to UsbInit - strcpy not strcat into uninitialized
   string AudioDevicePath.

   Revision 1.2  2007/12/01 06:04:20  wb6ymh
   Cleaned up logging.

   Revision 1.1  2007/11/26 14:45:33  wb6ymh
   1. Added support for CM108 based audio dongles.
   2. Converted to C++ and made most routines members of the CPort class.
   3. Modified UsbInit to look for devices based on the physical USB address
      in addition to USB serial number.
   4. Modified UsbInit to use GetUsbType to determine if a random device
      is supported rather than checking the USB vendor and product IDs locally.
   5. Added code to UsbInit to set PTT_Bit and COS_Bit based on UsbDevType if
      they weren't specified in the configuration file.

   Revision 1.1  2007/11/21 17:54:38  wb6ymh
   Inital import - support for PTT via iMic USB audio dongle.

   
*/ 
#include "common.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <usb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "main.h"
#include "usbdev.h"
#include "configvars.h"
#include "conference.h"
#include "nodes.h"
#include "ports.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#define C108_HID_INTERFACE 3

#define HID_REPORT_GET  0x01
#define HID_REPORT_SET  0x09

#define HID_RT_INPUT    0x01
#define HID_RT_OUTPUT   0x02

#define UAC3556_SETMEM     4
#define UAC3556_GETMEM     5
#define UAC3566_IO_DIR_L   0xa184   // 1 = output, 0 = input
#define UAC3566_IO_DIR_H   0xa185   // 1 = output, 0 = input
#define UAC3556_DOUT_L     0xb0a0   // outputs 0 -> 7
#define UAC3556_DIO_H      0xb0a1   // inputs/outputs 8 -> 11
#define UAC3556_DIN_L      0xb0a2   // inputs 0 -> 7
#define UAC3556_HID_INTERFACE 3

// Adjust for bizarre hardware.  The register containing the upper 4 bits 
// of GPIO has input bits in the lower nibble and output bits in the
// upper nibble.
#define UAC3556_BitAdjust(x) (x > 0xff ? x << 4 : x)

typedef struct {
   u_int16_t idVendor;
   u_int16_t idProduct;
   int   UsbDevType;
} SupportedUsbDev;
   
SupportedUsbDev SupportedUsbDevs[] = {
// Micronas uac3556 (iMic w/ EEPROM removed or corrupted)
   { 0x074d, 0x3556, USB_TYPE_UAC3556},   
// Griffin iMic
   { 0x077d, 0x07af, USB_TYPE_UAC3556},   
// C-Media CM108
   { 0x0d8c, 0x000c, USB_TYPE_CM108},
// C-Media CM108AH, 0x0138 -> 0x13f depending on
// pin strapping.  URIs has been seen as both 0x13c and 0x13a
   { 0x0d8c, 0x0138, USB_TYPE_CM108},
   { 0x0d8c, 0x0139, USB_TYPE_CM108},
   { 0x0d8c, 0x013a, USB_TYPE_CM108},
   { 0x0d8c, 0x013b, USB_TYPE_CM108},
   { 0x0d8c, 0x013c, USB_TYPE_CM108},
   { 0x0d8c, 0x013d, USB_TYPE_CM108},
   { 0x0d8c, 0x013e, USB_TYPE_CM108},
   { 0x0d8c, 0x013f, USB_TYPE_CM108},
// C-Media CM119 (report as CM108, it's compatible)
   { 0x0d8c, 0x000e, USB_TYPE_CM108},
};

#define NUM_KNOWN_DEVS   (sizeof(SupportedUsbDevs) / sizeof(SupportedUsbDev))

void CPort::UsbKeyRadio(int bKey)
{
   int ret;
   
   if(UsbDevType == USB_TYPE_UAC3556 && TxKeyMethod == 5) {
   // The UAC3556 has 12 bits of GPIO
      int16 *GPIO = (int16 *) &UsbGPIO;
      int16 BitPTT = UAC3556_BitAdjust(PTT_Bit);
      if((!bKey && !InvertPTT) || (bKey && InvertPTT)) {
      // Set PTT bit low
         *GPIO &= ~BitPTT;
      }
      else {
      // Set PTT bit high
         *GPIO |= BitPTT;
      }

      ret = usb_control_msg(udev,USB_TYPE_VENDOR,UAC3556_SETMEM,0,
                            UAC3556_DOUT_L,UsbGPIO,2,1000);
      if(ret != 2) {
         LOG_ERROR(("%s#%d: usb_control_msg returned %d (%s)\n",__FUNCTION__,
                    __LINE__,ret,Err2String(errno)));
      }
   }
   else if(UsbDevType == USB_TYPE_UAC3556 && TxKeyMethod == 6) {
   // iMic with PCF8754 I2C expander
      if((!bKey && !InvertPTT) || (bKey && InvertPTT)) {
      // Set PTT bit low
         UsbGPIO[0] &= ~PTT_Bit;
      }
      else {
      // Set PTT bit high
         UsbGPIO[0] |= PTT_Bit;
      }
      Write8754(UsbGPIO[0]);
   }
   else if(UsbDevType == USB_TYPE_CM108) {
/* CM108 output report format:

   byte 0 bits:
   7-6 R / W 0: HID_OR1-2 are used for GPO; 
                HID_OR0, 3 are used for SPDIF 
             1: Reserved 
             2: Values written to HID_OR0-3 are also mapped to 
                EEPROM_DATA0-1, EEPROM_CTRL (See Note) 
             3: Reserved 0x0 
             
   5 R / W Reserved 0x0 
   4 R / W When HID_OR0[7] == 1’b0: Valid bit in SPDIF frame 
           When HID_OR0[7] == 1’b1: Reserved 0x0 
           
   3-0 R / W When HID_OR0[7] == 1’b0: First nibble of SPDIF status channel 
             When HID_OR0[7] == 1’b1: Reserved

   byte 1 bits:             
   7-0 R / W When HID_OR0[7:6] == 2’b00: 
            HID_OR1[3:0] is the output to GPIO4 ~ GPIO1 in output mode 
               0: GPO drives L 
               1: GPO drives H 
               
            When HID_OR0[7:6] == 2’b01: 
               Reserved 
               
            When HID_OR0[7:6] == 2’b1x:
               Mapped to EEPROM__DATA0
               
   byte 2 bits:
   7-0 R / W When HID_OR0[7:6] == 2’b00: 
            HID_OR2[3:0] is the mode setting GPIO4 ~ GPIO1 
               0: Set GPIO to input mode 
               1: Set GPIO to output mode 
            
            When HID_OR0[7:6] == 2’b01: 
               Reserved 
               
            When HID_OR0[7:6] == 2’b1x: 
               Mapped to EEPROM_DATA1

   byte 3 bits:
   7-0 R / W When HID_OR0[7] == 1’b0:
               Category byte of SPDIF status channel 
               
            When HID_OR0[7] == 1’b1: 
               Mapped to EEPROM_CTRL
*/ 
      if((bKey && !InvertPTT) || (!bKey && InvertPTT)) {
         UsbGPIO[1] = PTT_Bit;   // set GPIO high
      }
      else {
         UsbGPIO[1] &= ~PTT_Bit; // set GPIO low
      }

      ret = usb_control_msg(udev,
                            USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
                            HID_REPORT_SET,0 + (HID_RT_OUTPUT << 8),
                            C108_HID_INTERFACE,UsbGPIO,4,1000);
      if(ret != 4) {
         LOG_ERROR(("%s#%d: usb_control_msg returned %d (%s)\n",__FUNCTION__,
                    __LINE__,ret,strerror(errno)));
      }
   }
}

/*
   CM108 HID input report byte 1 bits:
      7-6 R 00: HID_IR1 is used as GPI 
            10: Values written to HID_IR0-3 are also mapped to 
                EEPROM_DATA0-1,  EEPROM_CTRL 
            Others: Reserved
      5-4 R Reserved
      3 R 0: No activity on Record-Mute button 
          1: Record-Mute button pressed then released
      2 R 0: No activity on Playback-Mute button 
          1: Playback-Mute button pressed then released
      1 R 0: Volume-Down button released 
          1: Volume-Down button pressed
      0 R 0: Volume-Up button released 
          1: Volume-Up button pressed

   HID input report byte 2 bits:
      7-0 R When HID_IR0[7:6] == 2’b00: HID_IR1[3:0] is the input from GPIO4
            GPIO1 in input mode 
            
            When HID_OR0[7] == 1’b1: Mapped from EEPROM_DATA0
            
   HID input report byte 3 bits:
      7-0 R When HID_OR0[7] == 1’b1: Mapped from EEPROM_DATA1        
      
   HID input report byte 4 bits:
      7-0 R When HID_OR0[7] == 1’b1: Mapped from EEPROM_CTRL          
          
*/
int CPort::UsbPollGPIO()
{
   int Ret = 0;
   int Err;
   char Buf[4] = {0};

   if(UsbDevType == USB_TYPE_CM108) {
      usb_control_msg(udev,
                      USB_ENDPOINT_IN + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
                      HID_REPORT_GET,0 + (HID_RT_INPUT << 8),
                      C108_HID_INTERFACE,Buf,4,1000);
      Ret = Buf[0];
   }
   else if(UsbDevType == USB_TYPE_UAC3556 && RxCosMethod == 7) {
      Err = usb_control_msg(udev,USB_TYPE_VENDOR | USB_ENDPOINT_IN,
                            UAC3556_GETMEM,0,UAC3556_DIO_H,Buf,2,1000);
   // Adjust for bizarre hardware.  The register containing the upper 4 bits 
   // of GPIO has input bits in the lower nibble and output bits in the
   // upper nibble.
      Ret = ((Buf[0] & 0xf) << 8) | Buf[1];
   }
   else if(UsbDevType == USB_TYPE_UAC3556 && RxCosMethod == 8) {
      char ReadSetAdr[] = {
         0x71,          // i2c address && read bit = 1 
         0x00,0x02,     // telegram length (msb, lsb)
         0x00,          // sub address length
         0xff,0xff      // data buffer address
      };
      int i;

      do {
      // Read expander
         Err = usb_control_msg(udev,USB_TYPE_VENDOR,8,0,0,ReadSetAdr,6,1000);
         if(Err != 6) {
            LOG_ERROR(("%s#%d: Error: usb_control_msg returned %d reading\n",
                       __FUNCTION__,__LINE__,Err));
            break;
         }
         for(i = 0; i < 100; i++) {
         // wait for completion status
            /* '00' : telegram running
               'ff' : telegram ended successfully
               '01' : bus Error
               '80' : data checksum Error
            */
   
            Err = usb_control_msg(udev,USB_TYPE_VENDOR | USB_ENDPOINT_IN,
                                  9,1,0,Buf,1,1000);
            if(Err != 1) {
               LOG_ERROR(("%s#%d: Error: usb_control_msg returned %d\n",
                          __FUNCTION__,__LINE__,Err));
               break;
            }
   
            Err = 1;
            switch((unsigned char) Buf[0]) {
               case 1:
                  LOG_ERROR(("%s#%d: bus Error\n",__FUNCTION__,__LINE__));
                  break;
   
               case 0:
                  Err = 0;
                  break;
   
               case 0x80:
               case 0xff:
                  break;
   
               default:
                  LOG_ERROR(("%s#%d: Error: unknown status byte 0x%x\n",
                             __FUNCTION__,__LINE__,Buf[0]));
                  break;
            }
   
            if(Err)  {
               break;
            }
         }
         // read the data
         Buf[0] = 0x55;
         Buf[1] = 0x55;
         Err = usb_control_msg(udev,USB_TYPE_VENDOR | USB_ENDPOINT_IN,9,0,0,Buf,
                               2,1000);
         if(Err != 2) {
            LOG_ERROR(("%s#%d: usb_control_msg returned %d while reading\n",
                       __FUNCTION__,__LINE__,Err));
            break;
         }
         else {
            Ret = UsbGPIO[0] = Buf[0];
         }
      } while(FALSE);
   }
   return Ret;
}

int FindSoundDev(char *DevAdr)
{
   int i;
   char Path[PATH_MAX];
   char Line[80];
   FILE *fp = NULL;
   char *cp;
   int bDeviceFound = FALSE;

   for(i = 0; i < 32; i++) {
      do {
         sprintf(Path,"/proc/asound/card%d/usbbus",i);
         if((fp = fopen(Path,"r")) == NULL) {
            break;
         }
         if(!fgets(Line,sizeof(Line),fp)) {
            break;
         }
         if((cp = strchr(Line,'\n')) != NULL) {
            *cp = 0;
         }

         if(strcasecmp(Line,DevAdr) == 0) {
            bDeviceFound = TRUE;
         }
      } while(FALSE);

      if(fp != NULL) {
         fclose(fp);
         fp = NULL;
      }

      if(bDeviceFound) {
         break;
      }
   }

   if(i == 32) {
   // return error
      i = -1;
   }

   return i;
}

int GetUsbType(struct usb_device_descriptor *pDesc)
{
   SupportedUsbDev *p = SupportedUsbDevs;
   int UsbDevType = 0;

   for(unsigned int i = 0; i < NUM_KNOWN_DEVS; i++) {
      if(p->idVendor == pDesc->idVendor && p->idProduct == pDesc->idProduct) {
         UsbDevType = p->UsbDevType;
         break;
      }
      p++;
   }
   return UsbDevType;
}

// return the physical USB address for this Devnum
char *GetUsbBusAdr(int Devnum)
{
   struct dirent *dirEntry;
   char Path[PATH_MAX];
   DIR *dp;
   char *cp;
   char *Ret = NULL;
   int Dev;
   const char *RootDir = "/sys/bus/usb/devices";
   FILE *fp;

   if((dp = opendir(RootDir)) != NULL) {
      while(Ret == NULL && (dirEntry = readdir(dp)) != NULL) {
         do {
            snprintf(Path,PATH_MAX,"%s/%s/devnum",RootDir,dirEntry->d_name);

            if((fp = fopen(Path,"r")) != NULL) {
               if(fscanf(fp,"%d",&Dev) != 1) {
                  LOG_ERROR(("%s: couldn't get device number from - %s\n",
                             __FUNCTION__,Path));
               }
               else {
                  if(Dev == Devnum) {
                  // This is our guy
                     if((cp = strchr(dirEntry->d_name,':')) != NULL) {
                     // remove endpoint info
                        *cp = 0;
                     }
                     Ret = strdup(dirEntry->d_name);
                  }
               }
               fclose(fp);
            }
         } while(FALSE);
      }
      closedir(dp);
   }

   return Ret;
}

// Find USB device for this port.
// if AudioDongleSN != NULL then look for a device with the specified SN
// if AudioDongleAdr != NULL then look for an device at the specified address
// otherwise use the first compatible device
int CPort::UsbInit(void) 
{
   int Ret = ERR_USB_DEV_OPEN;   // assume the worse
#ifdef __linux__
   struct usb_bus *bus;
   struct usb_device *dev = NULL;
   int err;
   char Path[PATH_MAX];
   char DeviceSn[32];
   char DevAdr[32];
   int bDeviceFound = FALSE;
   int Devnum = -1;
   int BusNum = -1;
   FILE *fp;

   if(AudioDongleAdr != NULL) {
   // Convert fully specified USB device address into bus/devnum
      if(sscanf(AudioDongleAdr,"%d",&BusNum) != 1) {
         LOG_ERROR(("%s: couldn't get bus number from %s\n",__FUNCTION__,
                    AudioDongleAdr));
      }

      snprintf(Path,PATH_MAX,"/sys/bus/usb/devices/%s/devnum",AudioDongleAdr);
      if((fp = fopen(Path,"r")) != NULL) {
         if(fscanf(fp,"%d",&Devnum) != 1) {
            LOG_ERROR(("%s: couldn't get device number from - %s\n",
                       __FUNCTION__,Path));
         }
         fclose(fp);
      }
      else {
         LOG_ERROR(("%s: couldn't open %s - %s\n",__FUNCTION__,Path,
                    strerror(errno)));
      }
   }

   usb_init();
   usb_find_busses();
   usb_find_devices();

   for(bus = usb_busses; bus && dev == NULL; bus = bus->next) {
      for(dev = bus->devices; dev; dev = dev->next) {
         snprintf(DevAdr,sizeof(DevAdr),"%s/%s",bus->dirname,dev->filename);
         if(AudioDongleSN != NULL) {
         // Looking for device by SN
            if(dev->descriptor.iSerialNumber) {
               if((udev = usb_open(dev)) == NULL) {
                  LOG_ERROR(("%s#%d: usb_open failed.\n",__FUNCTION__,__LINE__));
               }
               err = usb_get_string_simple(udev,dev->descriptor.iSerialNumber,
                                           DeviceSn,sizeof(DeviceSn));
               usb_close(udev);
               udev = NULL;

               if(err > 0) {
                  if(strcmp(DeviceSn,AudioDongleSN) == 0) {
                     // This is our guy
                     bDeviceFound = TRUE;
                     break;
                  }
               }
            }
         }
         else if(AudioDongleAdr != NULL) {
         // Looking for device by address
            if(atoi(bus->dirname) == BusNum && atoi(dev->filename) == Devnum) {
               bDeviceFound = TRUE;
               break;
            }
         }
         else if(GetUsbType(&dev->descriptor) != 0) {
            bDeviceFound = TRUE;
            Devnum = atoi(dev->filename);
            AudioDongleAdr = GetUsbBusAdr(Devnum);
            break;
         }
      }

      if(bDeviceFound) {
         break;
      }
   }

   do {
      if(dev == NULL ) {
         LOG_ERROR(("%s: USB device not found\n",__FUNCTION__));
         break;
      }

      if((UsbDevType = GetUsbType(&dev->descriptor)) == 0) {
         LOG_ERROR(("%s: ERROR - USB device 0x%04x:0x%04x is not known\n",
                    __FUNCTION__,dev->descriptor.idVendor,
                    dev->descriptor.idProduct));
         break;
      }
      
      if(AudioDongleAdr == NULL) {
         AudioDongleAdr = GetUsbBusAdr(atoi(dev->filename));
      }

      if(AudioDongleSN != NULL) {
         LOG_ERROR(("%s: found USB device \"%s\" @ %s\n",__FUNCTION__,
                    AudioDongleSN,AudioDongleAdr));
      }
      else {
         LOG_ERROR(("%s: found USB device @ %s\n",__FUNCTION__,AudioDongleAdr));
      }

      if(PTT_Bit == -1) {
      // PTT_Bit not specified in configuration file, set defaults
         switch(UsbDevType) {
            case USB_TYPE_UAC3556:
               PTT_Bit = 0x20;   // GPIO 5
               InvertPTT = 1;    // active low
               break;

            case USB_TYPE_CM108:
               PTT_Bit   = 0x04; // GPIO 3
               InvertPTT = 0;    // active high
               break;
         }
      }

      if(COS_Bit == -1) {
      // COS_Bit not specified in configuration file, set defaults
         switch(UsbDevType) {
            case USB_TYPE_UAC3556:
               COS_Bit = 1;      // GPIO 0
               break;

            case USB_TYPE_CM108:
               COS_Bit = 0x02; // volume down
               break;
         }
      }

      if(RxCtcssMethod != 0 && CTCSS_Bit == -1) {
      // CTCSS_Bit not specified in configuration file, set defaults
         switch(UsbDevType) {
            case USB_TYPE_UAC3556:
               CTCSS_Bit = 2;      // GPIO 1
               break;

            case USB_TYPE_CM108:
               CTCSS_Bit = 0x01; // volume up
               break;
         }
      }


      if((udev = usb_open(dev)) != NULL) {
         Ret = 0;
      }
      else {
         LOG_ERROR(("%s: usb_open failed: %s\n",__FUNCTION__,strerror(errno)));
         break;
      }

      if(UsbDevType == USB_TYPE_UAC3556) {
      // Take over the HID interface from the kernel

      // Initialize GPIO direction
         int16 Buf = PTT_Bit | DH_ClockBit | DH_DataBit;
         err = usb_control_msg(udev,USB_TYPE_VENDOR,UAC3556_SETMEM,0,
                               UAC3566_IO_DIR_L,(char *) &Buf,2,1000);
         if(err != 2) {
            LOG_ERROR(("%s: usb_control_msg failed (%d)\n",__FUNCTION__,err));
            break;
         }
      }
      else if(UsbDevType == USB_TYPE_CM108) {
      // The C-Media chip uses HID to talk to the GPIO bits, so we need to
      // take over the HID interface from the kernel
         if((err = usb_detach_kernel_driver_np(udev,C108_HID_INTERFACE)) != 0) {
            LOG_ERROR(("%s#%d: usb_detach_kernel_driver_np returned %d (%s)\n",
                       __FUNCTION__,__LINE__,err,strerror(errno)));
         }

         if((err = usb_claim_interface(udev,C108_HID_INTERFACE)) != 0) {
            LOG_ERROR(("%s#%d: usb_claim_interface returned %d (%s)\n",
                       __FUNCTION__,__LINE__,err,strerror(errno)));
            break;
         }
      // Initialize GPIO direction
         UsbGPIO[0] = 0; // set GPIO mode
      // Precondition Doug Hall clock low
         UsbGPIO[1] = 0;
      // Set input/output sense
         UsbGPIO[2] = PTT_Bit | DH_ClockBit | DH_DataBit;
         UsbGPIO[3] = 0; // not used
      }

   // Unkey the radio
      UsbKeyRadio(FALSE);

   // Find the /dev/dsp entry for this device
      if((err = FindSoundDev(DevAdr)) >= 0) {
         char AudioDevicePath[80];
         if(err > 0) {
            snprintf(AudioDevicePath,sizeof(AudioDevicePath),"/dev/dsp%d",err);
         }
         else {
            strcpy(AudioDevicePath,"/dev/dsp");
         }
         LOG_ERROR(("%s: USB audio device for %s is %s\n",__FUNCTION__,
                    NodeName,AudioDevicePath));
         if(AudioDevice != NULL && strcmp(AudioDevice,AudioDevicePath) != 0) {
            LOG_ERROR(("%s: Changing audio device from %s to %s\n",
                       __FUNCTION__,AudioDevice,AudioDevicePath));
         }
         if(AudioDevice != NULL) {
            free(AudioDevice);
         }

         AudioDevice = strdup(AudioDevicePath);
      }
      else {
         LOG_ERROR(("%s: USB audio device file not found\n",__FUNCTION__));
      }

   } while(FALSE);
#else
   LOG_ERROR(("%s: USB is only supported on Linux\n",__FUNCTION__));
#endif
   return Ret;
}

void CPort::UsbShutdown()
{
   int Err;

   if(UsbDevType == USB_TYPE_UAC3556) {
#if 0
      if((Err = usb_release_interface(udev,UAC3556_HID_INTERFACE)) != 0) {
         LOG_ERROR(("%s#%d: usb_release_interface returned %d (%s)\n",
                    __FUNCTION__,__LINE__,Err,strerror(errno)));
      }
#ifdef LIBUSB_HAS_ATTACH_KERNEL_DRIVER_NP
      if((Err = usb_attach_kernel_driver_np(udev,UAC3556_HID_INTERFACE)) != 0) {
         LOG_ERROR(("%s#%d: usb_attach_kernel_driver_np returned %d (%s)\n",
                    __FUNCTION__,__LINE__,Err,strerror(errno)));
      }
#endif
#endif
   }
   else if(UsbDevType == USB_TYPE_CM108) {
      if((Err = usb_release_interface(udev,C108_HID_INTERFACE)) != 0) {
         LOG_ERROR(("%s#%d: usb_release_interface returned %d (%s)\n",
                    __FUNCTION__,__LINE__,Err,strerror(errno)));
      }
#ifdef LIBUSB_HAS_ATTACH_KERNEL_DRIVER_NP
      if((Err = usb_attach_kernel_driver_np(udev,C108_HID_INTERFACE)) != 0) {
         LOG_ERROR(("%s#%d: usb_attach_kernel_driver_np returned %d (%s)\n",
                    __FUNCTION__,__LINE__,Err,strerror(errno)));
      }
#endif
   }
}

void CPort::SendGenericDataUSB(unsigned char *Data,int Len)
{
   unsigned char Mask;
   int j;
   int i;
   int ret;

   if(Debug & DLOG_RIG_CTRL) {
      LOG_ERROR(("%s: Sending %d bytes:\n",__FUNCTION__,Len));
      LogHex(Data,Len);
   }

   if(UsbDevType == USB_TYPE_UAC3556 && RadioCtrlMethod == 2) {
   // The UAC3556 has 12 bits of GPIO
      int16 *GPIO = (int16 *) &UsbGPIO;
      int16 ClockBit = UAC3556_BitAdjust(DH_ClockBit);
      int16 DataBit = UAC3556_BitAdjust(DH_DataBit);

      for(i = 0; i < Len; i++) {
         Mask = 1;
         for(j = 0; j < 8; j++) {
            *GPIO |= ClockBit;
            if(Data[i] & Mask) {
            // Data bit is high
               *GPIO |= DataBit;
            }
            else {
            // Data bit is low
               *GPIO &= ~DataBit;
            }
         // Output clock high and new data bit
            ret = usb_control_msg(udev,USB_TYPE_VENDOR,UAC3556_SETMEM,0,
                                  UAC3556_DOUT_L,UsbGPIO,2,1000);
            if(ret != 2) {
               LOG_ERROR(("%s#%d: usb_control_msg returned %d (%s)\n",
                          __FUNCTION__,__LINE__,ret,Err2String(errno)));
               break;
            }
   
         // Drop clock to clock in new data
            *GPIO &= ~ClockBit;
            ret = usb_control_msg(udev,USB_TYPE_VENDOR,UAC3556_SETMEM,0,
                                  UAC3556_DOUT_L,UsbGPIO,2,1000);
            if(ret != 2) {
               LOG_ERROR(("%s#%d: usb_control_msg returned %d (%s)\n",
                          __FUNCTION__,__LINE__,ret,Err2String(errno)));
               break;
            }
            Mask <<= 1;
         }
      }
   }
   else if(UsbDevType == USB_TYPE_UAC3556 && RadioCtrlMethod == 3) {
   // iMic with PCF8754 I2C expander
      for(i = 0; i < Len; i++) {
         Mask = 1;
         for(j = 0; j < 8; j++) {
            UsbGPIO[0] |= DH_ClockBit;
            if(Data[i] & Mask) {
            // Data bit is high
               UsbGPIO[0] |= DH_DataBit;
            }
            else {
            // Data bit is low
               UsbGPIO[0] &= ~DH_DataBit;
            }
            Write8754(UsbGPIO[0]);
   
         // Drop clock to clock in new data
            UsbGPIO[0] &= ~DH_ClockBit;
            Write8754(UsbGPIO[0]);
            Mask <<= 1;
         }
      }
   }
   else if(UsbDevType == USB_TYPE_CM108) {
      LOG_ERROR(("%s: DH_ClockBit: 0x%x, DH_DataBit: 0x%x\n",
                 __FUNCTION__,DH_ClockBit,DH_DataBit));
      for(i = 0; i < Len; i++) {
         Mask = 1;
         for(j = 0; j < 8; j++) {
            UsbGPIO[1] |= DH_ClockBit;
            if(Data[i] & Mask) {
            // Data bit is high
               UsbGPIO[1] |= DH_DataBit;
            }
            else {
            // Data bit is low
               UsbGPIO[1] &= ~DH_DataBit;
            }
         // Output clock high and new data bit
            
            ret = usb_control_msg(udev,
                                  USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
                                  HID_REPORT_SET,0 + (HID_RT_OUTPUT << 8),
                                  C108_HID_INTERFACE,UsbGPIO,4,1000);
            if(ret != 4) {
               LOG_ERROR(("%s#%d: usb_control_msg returned %d (%s)\n",
                          __FUNCTION__,__LINE__,ret,Err2String(errno)));
               break;
            }
#if 0
            if(Debug & DLOG_RIG_CTRL) {
               LOG_ERROR(("%s: Sent 4 bytes:\n",__FUNCTION__));
               LogHex(UsbGPIO,4);
            }
#endif
   
         // Drop clock to clock in new data
            UsbGPIO[1] &= ~DH_ClockBit;
            ret = usb_control_msg(udev,
                                  USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
                                  HID_REPORT_SET,0 + (HID_RT_OUTPUT << 8),
                                  C108_HID_INTERFACE,UsbGPIO,4,1000);
            if(ret != 4) {
               LOG_ERROR(("%s#%d: usb_control_msg returned %d (%s)\n",
                          __FUNCTION__,__LINE__,ret,Err2String(errno)));
               break;
            }
#if 0
            if(Debug & DLOG_RIG_CTRL) {
               LOG_ERROR(("%s: Sent 4 bytes:\n",__FUNCTION__));
               LogHex(UsbGPIO,4);
            }
#endif
            Mask <<= 1;
         }
      }
   }
}

// Write to I2C port expander.
void CPort::Write8754(char Data)
{
   int ret;
   char WriteSetAdr[7] = {
      0x70,          // i2c address && read bit = 0 
      0x00,0x01,     // telegram length (msb, lsb)
      0x00,          // sub address length
      0xff,0xff      // data buffer address
   };
   int i;
   char Buf[8];

   // Since the port expander is only quasi bi-directional we must write
   // ones to the input bits.
   
   WriteSetAdr[6] = Data;
   if(COS_Bit != -1) {
      WriteSetAdr[6] |= COS_Bit;
   }
   
   if(CTCSS_Bit != -1) {
      WriteSetAdr[6] |= CTCSS_Bit;
   }
   
   
   // Write the data to the buffer
   ret = usb_control_msg(udev,USB_TYPE_VENDOR,8,0,0,WriteSetAdr,7,1000);
   if(ret != 7) {
      LOG_ERROR(("%s#%d: usb_control_msg returned %d (%s) writing data\n",
                 __FUNCTION__,__LINE__,ret,Err2String(errno)));
   }
   else for(i = 0; i < 10000; i++) {
   // wait for completion status
      /* '00' : telegram running
         'ff' : telegram ended successfully
         '01' : bus error
         '80' : data checksum error
      */
   
      ret = usb_control_msg(udev,USB_TYPE_VENDOR | USB_ENDPOINT_IN,9,1,0,
                            Buf,1,1000);
      if(ret != 1) {
         LOG_ERROR(("%s#%d: usb_control_msg returned %d (%s) polling status\n",
                    __FUNCTION__,__LINE__,ret,Err2String(errno)));
         break;
      }
   
      ret = 1;
      switch((unsigned char) Buf[0]) {
         case 0:
         // keep waiting
            ret = 0;
            break;
   
         case 1:
            LOG_ERROR(("%s#%d: Bus Error\n",__FUNCTION__,__LINE__));
            break;
   
         case 0x80:
         case 0xff:
            ret = 2;
            break;
   
         default:
            LOG_ERROR(("%s#%d: Error: unknown status byte 0x%x returned\n",
                       __FUNCTION__,__LINE__,Buf[0]));
            break;
      }
   
      if(ret)  {
         break;
      }
   }
}

