/*
 * SpanDSP - a series of DSP components for telephony
 *
 * tone_generate.h - General telephony tone generation.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001 Steve Underwood
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
 * $Id tone_generate.h,v 1.26 2007/03/03 10:40:33 steveu Exp $
 *
 * $Log: tone_generate.h,v $
 * Revision 1.1.1.1  2007/04/07 22:27:59  wb6ymh
 * Initial import of a small subset of the spandsp library for CQiNet.
 * Currently all we are using are the DTMF decoder routines.
 *
 */

/*! \file */

#if !defined(_TONE_GENERATE_H_)
#define _TONE_GENERATE_H_

/*! \page tone_generation_page Tone generation
\section tone_generation_page_sec_1 What does it do?
The tone generation module provides for the generation of cadenced tones,
suitable for a wide range of telephony applications. 

\section tone_generation_page_sec_2 How does it work?
Oscillators are a problem. They oscillate due to instability, and yet we need
them to behave in a stable manner. A look around the web will reveal many papers
on this subject. Many describe rather complex solutions to the problem. However,
we are only concerned with telephony applications. It is possible to generate
the tones we need with a very simple efficient scheme. It is also practical to
use an exhaustive test to prove the oscillator is stable under all the
conditions in which we will use it. 
*/

/*!
    Cadenced dual tone generator descriptor.
*/
typedef struct
{
    int32_t phase_rate[2];
    float gain[2];
    int modulate;
    
    int duration[4];

    int repeat;
} tone_gen_descriptor_t;

/*!
    Cadenced dual tone generator state descriptor. This defines the state of
    a single working instance of a generator.
*/
typedef struct
{
    int32_t phase_rate[2];
    float gain[2];
    int modulate;

    uint32_t phase[2];

    int duration[4];
    
    int repeat;

    int current_section;
    int current_position;
} tone_gen_state_t;

#ifdef __cplusplus
extern "C" {
#endif

/*! Create a tone generator descriptor
    \brief Create a tone generator descriptor
    \param s The descriptor
    \param f1 The first frequency, in Hz
    \param l1 The level of the first frequency, in dBm0
    \param f2 0 for no second frequency, a positive number for the second frequency,
           in Hz, or a negative number for an AM modulation frequency, in Hz
    \param l2 The level of the second frequency, in dBm0, or the percentage modulation depth
           for an AM modulated tone.
    \param d1 x
    \param d2 x
    \param d3 x
    \param d4 x
    \param repeat x */
void make_tone_gen_descriptor(tone_gen_descriptor_t *s,
                              int f1,
                              int l1,
                              int f2,
                              int l2,
                              int d1,
                              int d2,
                              int d3,
                              int d4,
                              int repeat);

void tone_gen_init(tone_gen_state_t *s, tone_gen_descriptor_t *t);

int tone_gen(tone_gen_state_t *s, int16_t amp[], int max_samples);

#ifdef __cplusplus
}
#endif

#endif
/*- End of file ------------------------------------------------------------*/
