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


   rtp.h  --  RTP header file (RFC 1889)
   
   This header file is originally from RFC 1889.
   I've modified it to enable the use of bit fields on little endian machines.
   
   K&R states: 
   "Almost everything about bit fields is implementation-dependent.  
   Whether a field may overlap a word boundary is implementation-defined. ...
   Fields are assigned left to right on some machines and right to left on 
   others."
    
   However the BSD tcp/ip stack appears to assume that bits are assigned
   starting with the LSB on little endian machines and starting with the
   MSB on big endian machines.  Both gcc and the Microsoft C compilers
   assign bit fields starting from the LSB on x86 platforms.  
   
   That's good enough for me.
   
   $Log: rtp.h,v $
   Revision 1.3  2010/11/11 05:48:06  wb6ymh
   Added RTP_PT_DTMF and RTP_PT_G726.  This are non standard values that
   have been agreeded to by "other means" between WB6NIL and WB6YMH.

   Revision 1.2  2007/06/12 23:42:12  wb6ymh
   1. Moved typedefs for basic integer types to common.h.
   2. Added rtp_pl_type_t enum.

   Revision 1.1.1.1  2002/10/06 17:00:22  wb6ymh
   Move common header files to inc subdirectory from src.

   Revision 1.2  2002/09/15 14:49:22  wb6ymh
   Corrected rtp_hdr_t for little endian machines.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

 
*/
#ifndef RTP_H_
#define RTP_H_

#include <sys/types.h>

#ifdef _MSC_VER
#pragma pack(1)
#define GCC_PACKED
#else
#define GCC_PACKED __attribute__ ((packed))
#endif

/*
 * Current protocol version.
 */
#define RTP_VERSION 2

#define RTP_SEQ_MOD (1<<16)
#define RTP_MAX_SDES 255      /* maximum text length for SDES */

typedef enum {
    RTCP_SR   = 200,
    RTCP_RR   = 201,
    RTCP_SDES = 202,
    RTCP_BYE  = 203,
    RTCP_APP  = 204
} rtcp_type_t;

typedef enum {
    RTCP_SDES_END   = 0,
    RTCP_SDES_CNAME = 1,
    RTCP_SDES_NAME  = 2,
    RTCP_SDES_EMAIL = 3,
    RTCP_SDES_PHONE = 4,
    RTCP_SDES_LOC   = 5,
    RTCP_SDES_TOOL  = 6,
    RTCP_SDES_NOTE  = 7,
    RTCP_SDES_PRIV  = 8
} rtcp_sdes_type_t;

typedef enum {
   RTP_PT_PCMU = 0,
   RTP_PT_GSM = 3,
   RTP_PT_G723 = 4,
   RTP_PT_DVI4_8K = 5,
   RTP_PT_DVI4_16K = 6,
   RTP_PT_LPC = 7,
   RTP_PT_PCMA = 8,
   RTP_PT_G722 = 9,
   RTP_PT_L16_STEREO = 10,
   RTP_PT_L16_MONO = 11,
   RTP_PT_QCELP = 12,
   RTP_PT_CN = 13,
   RTP_PT_MPA = 14,
   RTP_PT_G728 = 15,
   RTP_PT_DVI4_11025 = 16,
   RTP_PT_DVI4_22050 = 17,
   RTP_PT_G729 = 18,
   RTP_PT_CELB = 25,
   RTP_PT_JPEG = 26,
   RTP_PT_NV = 28,
   RTP_PT_H261 = 31,
   RTP_PT_MPV = 32,
   RTP_PT_MP2T = 33,
   RTP_PT_H263 = 34,
// The following assignments are NOT standard.  WB6YMH and WB6NIL used 
// "other means" to come to an agreement to use this numbers for
// app_rpt <-> tlb communications
   RTP_PT_DTMF = 96,
   RTP_PT_G726 = 97
} rtp_pl_type_t;

/*
 * RTP data header
 */
typedef struct {
#ifdef BIG_ENDIAN_MACHINE
    unsigned int version:2;   /* protocol version */
    unsigned int p:1;         /* padding flag */
    unsigned int x:1;         /* header extension flag */
    unsigned int cc:4;        /* CSRC count */
    unsigned int m:1;         /* marker bit */
    unsigned int pt:7;        /* payload type */
    u_int16 seq;              /* sequence number */
    u_int32 ts;               /* timestamp */
    u_int32 ssrc;             /* synchronization source */
    u_int32 csrc[1];          /* optional CSRC list */
#else
    unsigned char cc:4;        /* CSRC count */
    unsigned char x:1;         /* header extension flag */
    unsigned char p:1;         /* padding flag */
    unsigned char version:2;   /* protocol version */
    unsigned char pt:7;        /* payload type */
    unsigned char m:1;         /* marker bit */
    u_int16 seq;              /* sequence number */
    u_int32 ts;               /* timestamp */
    u_int32 ssrc;             /* synchronization source */
    u_int32 csrc[1];         /* optional CSRC list */
#endif
} GCC_PACKED rtp_hdr_t;

#define  RTP_HDR_SIZE   (sizeof(rtp_hdr_t) - sizeof(u_int32))

/*
 * RTCP common header word
 */
typedef struct {
#ifdef BIG_ENDIAN_MACHINE
    unsigned int version:2;   /* protocol version */
    unsigned int p:1;         /* padding flag */
    unsigned int count:5;     /* varies by packet type */
    unsigned int pt:8;        /* RTCP packet type */
    u_int16 length;           /* pkt len in words, w/o this word */
#else  
   u_int16 count:5;           /* varies by payload type */
   u_int16 p:1;               /* padding flag */
   u_int16 version:2;         /* protocol version */
   u_int16 pt:8;              /* payload type */
   u_int16 length;            /* packet length in words, without this word */
#endif
} GCC_PACKED rtcp_common_t;

/*
 * Big-endian mask for version, padding bit and packet type pair
 */
#define RTCP_VALID_MASK (0xc000 | 0x2000 | 0xfe)
#define RTCP_VALID_VALUE ((RTP_VERSION << 14) | RTCP_SR)

/*
 * Reception report block
 */
typedef struct {
    u_int32 ssrc;             /* data source being reported */
#ifdef BIG_ENDIAN_MACHINE
    unsigned int fraction:8;  /* fraction lost since last SR/RR */
    int lost:24;              /* cumul. no. pkts lost (signed!) */
#else
    long lost:24;             /* cumulative number of packets lost (signed!) */
    unsigned long fraction:8; /* fraction lost since last SR/RR */
#endif
    u_int32 last_seq;         /* extended last seq. no. received */
    u_int32 jitter;           /* interarrival jitter */
    u_int32 lsr;              /* last SR packet from this source */
    u_int32 dlsr;             /* delay since last SR packet */
} GCC_PACKED rtcp_rr_t;

/*
 * SDES item
 */
typedef struct {
    u_int8 type;              /* type of item (rtcp_sdes_type_t) */
    u_int8 length;            /* length of item (in octets) */
    char data[1];             /* text, not null-terminated */
} GCC_PACKED rtcp_sdes_item_t;

/*
 * One RTCP packet
 */
typedef struct {
    rtcp_common_t common;     /* common header */
    union {
        /* sender report (SR) */
        struct {
            u_int32 ssrc;     /* sender generating this report */
            u_int32 ntp_sec;  /* NTP timestamp */
            u_int32 ntp_frac;
            u_int32 rtp_ts;   /* RTP timestamp */
            u_int32 psent;    /* packets sent */
            u_int32 osent;    /* octets sent */
            rtcp_rr_t rr[1];  /* variable-length list */
        } sr;

        /* reception report (RR) */
        struct {
            u_int32 ssrc;     /* receiver generating this report */
            rtcp_rr_t rr[1];  /* variable-length list */
        } rr;

        /* source description (SDES) */
        struct rtcp_sdes {
            u_int32 src;      /* first SSRC/CSRC */
            rtcp_sdes_item_t item[1]; /* list of SDES items */
        } sdes;

        /* BYE */
        struct {
            u_int32 src[1];   /* list of sources */
            /* can't express trailing text for reason */
        } bye;
    } r;
} GCC_PACKED rtcp_t;

typedef struct rtcp_sdes rtcp_sdes_t;

/*
 * Per-source state information
 */
typedef struct {
    u_int16 max_seq;        /* highest seq. number seen */
    u_int32 cycles;         /* shifted count of seq. number cycles */
    u_int32 base_seq;       /* base seq number */
    u_int32 bad_seq;        /* last 'bad' seq number + 1 */
    u_int32 probation;      /* sequ. packets till source is valid */
    u_int32 received;       /* packets received */
    u_int32 expected_prior; /* packet expected at last interval */
    u_int32 received_prior; /* packet received at last interval */
    u_int32 transit;        /* relative trans time for prev pkt */
    u_int32 jitter;         /* estimated jitter */
    /* ... */
} GCC_PACKED source;


#ifdef _MSC_VER
#pragma pack()
#endif

#endif
