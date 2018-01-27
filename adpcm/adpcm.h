/*
** adpcm.h - include file for adpcm coder.
**
** Version 1.0, 7-Jul-92.
**
** $Log: adpcm.h,v $
** Revision 1.2  2007/11/26 06:55:07  wb6ymh
** Added extern "C" around prototypes when compiled for C++.
**
** Revision 1.1  2007/06/29 23:18:15  wb6ymh
** Initial import - adpcm library for cqinet apps.
**
*/

struct adpcm_state {
    short   valprev; /* Previous output value */
    char index;      /* Index into stepsize table */
};

#ifdef __STDC__
#define ARGS(x) x
#else
#define ARGS(x) ()
#endif

#ifdef __cplusplus
extern "C" {
#endif

void adpcm_coder ARGS((short [], char [], int, struct adpcm_state *));
void adpcm_decoder ARGS((char [], short [], int, struct adpcm_state *));

#ifdef __cplusplus
}
#endif

