/*
 * SpanDSP - a series of DSP components for telephony
 *
 * tone_detect.c - General telephony tone detection.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001-2003, 2005 Steve Underwood
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id tone_detect.c,v 1.31 2007/03/03 10:40:33 steveu Exp $
 *
 * $Log: tone_detect.c,v $
 * Revision 1.1.1.1  2007/04/07 22:27:59  wb6ymh
 * Initial import of a small subset of the spandsp library for CQiNet.
 * Currently all we are using are the DTMF decoder routines.
 *
 */
 
/*! \file tone_detect.h */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <inttypes.h>
#include <stdlib.h>
#if defined(HAVE_TGMATH_H)
#include <tgmath.h>
#endif
#if defined(HAVE_MATH_H)
#include <math.h>
#endif
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>

#include "spandsp/telephony.h"
#include "spandsp/tone_detect.h"
#include "spandsp/tone_generate.h"

#if !defined(M_PI)
/* C99 systems may not define M_PI */
#define M_PI 3.14159265358979323846264338327
#endif

void make_goertzel_descriptor(goertzel_descriptor_t *t, float freq, int samples)
{
    t->fac = 2.0f*cosf(2.0f*M_PI*(freq/(float) SAMPLE_RATE));
    t->samples = samples;
}
/*- End of function --------------------------------------------------------*/

goertzel_state_t *goertzel_init(goertzel_state_t *s,
                                goertzel_descriptor_t *t)
{
    if (s  ||  (s = malloc(sizeof(goertzel_state_t))))
    {
        s->v2 =
        s->v3 = 0.0;
        s->fac = t->fac;
        s->samples = t->samples;
        s->current_sample = 0;
    }
    return s;
}
/*- End of function --------------------------------------------------------*/

void goertzel_reset(goertzel_state_t *s)
{
    s->v2 =
    s->v3 = 0.0;
    s->current_sample = 0;
}
/*- End of function --------------------------------------------------------*/

int goertzel_update(goertzel_state_t *s,
                    const int16_t amp[],
                    int samples)
{
    int i;
    float v1;

    if (samples > s->samples - s->current_sample)
        samples = s->samples - s->current_sample;
    for (i = 0;  i < samples;  i++)
    {
        v1 = s->v2;
        s->v2 = s->v3;
        s->v3 = s->fac*s->v2 - v1 + amp[i];
    }
    s->current_sample += samples;
    return samples;
}
/*- End of function --------------------------------------------------------*/

float goertzel_result(goertzel_state_t *s)
{
    float v1;

    /* Push a zero through the process to finish things off. */
    v1 = s->v2;
    s->v2 = s->v3;
    s->v3 = s->fac*s->v2 - v1;
    /* Now calculate the non-recursive side of the filter. */
    /* The result here is not scaled down to allow for the magnification
       effect of the filter (the usual DFT magnification effect). */
    return s->v3*s->v3 + s->v2*s->v2 - s->v2*s->v3*s->fac;
}
/*- End of function --------------------------------------------------------*/
/*- End of file ------------------------------------------------------------*/
