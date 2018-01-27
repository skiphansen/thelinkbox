/*

    Sun compatible u-law to linear conversion

   $Log: ulaw2linear.h,v $
   Revision 1.1  2008/07/22 01:45:52  wb6ymh
   Inital import - ulaw routines from Speak freely.

*/

extern short            u2s[];
extern unsigned char    s2u[];

#define audio_u2s(x)  (u2s[  (unsigned  char)(x)       ])
#define audio_s2u(x)  (s2u[ ((unsigned short)(x)) >> 3 ])
