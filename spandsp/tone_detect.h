/*
 * SpanDSP - a series of DSP components for telephony
 *
 * tone_detect.h - General telephony tone detection.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001, 2005 Steve Underwood
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
 * $Id tone_detect.h,v 1.28 2007/03/03 10:40:33 steveu Exp $
 *
 * $Log: tone_detect.h,v $
 * Revision 1.1.1.1  2007/04/07 22:28:00  wb6ymh
 * Initial import of a small subset of the spandsp library for CQiNet.
 * Currently all we are using are the DTMF decoder routines.
 *
 */

#if !defined(_TONE_DETECT_H_)
#define _TONE_DETECT_H_

/*!
    Floating point Goertzel filter descriptor.
*/
typedef struct
{
    float fac;
    int samples;
} goertzel_descriptor_t;

/*!
    Floating point Goertzel filter state descriptor.
*/
typedef struct
{
    float v2;
    float v3;
    float fac;
    int samples;
    int current_sample;
} goertzel_state_t;

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Create a descriptor for use with either a Goertzel transform */
void make_goertzel_descriptor(goertzel_descriptor_t *t,
                              float freq,
                              int samples);

/*! \brief Initialise the state of a Goertzel transform.
    \param s The Goertzel context. If NULL, a context is allocated with malloc.
    \param t The Goertzel descriptor.
    \return A pointer to the Goertzel state. */
goertzel_state_t *goertzel_init(goertzel_state_t *s,
                                goertzel_descriptor_t *t);

/*! \brief Reset the state of a Goertzel transform.
    \param s The Goertzel context.
    \param t The Goertzel descriptor.
    \return A pointer to the Goertzel state. */
void goertzel_reset(goertzel_state_t *s);

/*! \brief Update the state of a Goertzel transform.
    \param s The Goertzel context
    \param amp The samples to be transformed
    \param samples The number of samples
    \return The number of samples unprocessed */
int goertzel_update(goertzel_state_t *s,
                    const int16_t amp[],
                    int samples);

/*! \brief Evaluate the final result of a Goertzel transform.
    \param s The Goertzel context
    \return The result of the transform. */
float goertzel_result(goertzel_state_t *s);

/*! \brief Update the state of a Goertzel transform.
    \param s The Goertzel context
    \param amp The sample to be transformed. */
static __inline__ void goertzel_sample(goertzel_state_t *s, int16_t amp)
{
    float v1;

    v1 = s->v2;
    s->v2 = s->v3;
    s->v3 = s->fac*s->v2 - v1 + amp;
    s->current_sample++;
}
/*- End of function --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*- End of file ------------------------------------------------------------*/
