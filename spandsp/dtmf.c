/*
 * SpanDSP - a series of DSP components for telephony
 *
 * dtmf.c - DTMF generation and detection.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001-2003, 2005, 2006 Steve Underwood
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
 * $Id dtmf.c,v 1.14 2007/02/27 16:52:16 steveu Exp $
 *
 * $Log: dtmf.c,v $
 * Revision 1.1.1.1  2007/04/07 22:27:59  wb6ymh
 * Initial import of a small subset of the spandsp library for CQiNet.
 * Currently all we are using are the DTMF decoder routines.
 *
 */
 
/*! \file dtmf.h */

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
#include "spandsp/super_tone_rx.h"
#include "spandsp/dtmf.h"

#if !defined(M_PI)
/* C99 systems may not define M_PI */
#define M_PI 3.14159265358979323846264338327
#endif

//#define USE_3DNOW

#define DEFAULT_DTMF_TX_LEVEL       -10
#define DEFAULT_DTMF_TX_ON_TIME     50
#define DEFAULT_DTMF_TX_OFF_TIME    55

#define DTMF_THRESHOLD              8.0e7f
#define DTMF_NORMAL_TWIST           6.3f    /* 8dB */
#define DTMF_REVERSE_TWIST          2.5f    /* 4dB */
#define DTMF_RELATIVE_PEAK_ROW      6.3f    /* 8dB */
#define DTMF_RELATIVE_PEAK_COL      6.3f    /* 8dB */
#define DTMF_TO_TOTAL_ENERGY        42.0f

static const float dtmf_row[] =
{
     697.0f,  770.0f,  852.0f,  941.0f
};
static const float dtmf_col[] =
{
    1209.0f, 1336.0f, 1477.0f, 1633.0f
};

static const char dtmf_positions[] = "123A" "456B" "789C" "*0#D";

static goertzel_descriptor_t dtmf_detect_row[4];
static goertzel_descriptor_t dtmf_detect_col[4];

static int dtmf_tx_inited = FALSE;
static tone_gen_descriptor_t dtmf_digit_tones[16];

#if defined(USE_3DNOW)
static __inline__ void _dtmf_goertzel_update(goertzel_state_t *s,
                                             float x[],
                                             int samples)
{
    int n;
    float v;
    int i;
    float vv[16];

    vv[4] = s[0].v2;
    vv[5] = s[1].v2;
    vv[6] = s[2].v2;
    vv[7] = s[3].v2;
    vv[8] = s[0].v3;
    vv[9] = s[1].v3;
    vv[10] = s[2].v3;
    vv[11] = s[3].v3;
    vv[12] = s[0].fac;
    vv[13] = s[1].fac;
    vv[14] = s[2].fac;
    vv[15] = s[3].fac;

    //v1 = s->v2;
    //s->v2 = s->v3;
    //s->v3 = s->fac*s->v2 - v1 + x[0];

    __asm__ __volatile__ (
             " femms;\n"

             " movq        16(%%edx),%%mm2;\n"
             " movq        24(%%edx),%%mm3;\n"
             " movq        32(%%edx),%%mm4;\n"
             " movq        40(%%edx),%%mm5;\n"
             " movq        48(%%edx),%%mm6;\n"
             " movq        56(%%edx),%%mm7;\n"

             " jmp         1f;\n"
             " .align 32;\n"

             " 1: ;\n"
             " prefetch    (%%eax);\n"
             " movq        %%mm3,%%mm1;\n"
             " movq        %%mm2,%%mm0;\n"
             " movq        %%mm5,%%mm3;\n"
             " movq        %%mm4,%%mm2;\n"

             " pfmul       %%mm7,%%mm5;\n"
             " pfmul       %%mm6,%%mm4;\n"
             " pfsub       %%mm1,%%mm5;\n"
             " pfsub       %%mm0,%%mm4;\n"

             " movq        (%%eax),%%mm0;\n"
             " movq        %%mm0,%%mm1;\n"
             " punpckldq   %%mm0,%%mm1;\n"
             " add         $4,%%eax;\n"
             " pfadd       %%mm1,%%mm5;\n"
             " pfadd       %%mm1,%%mm4;\n"

             " dec         %%ecx;\n"

             " jnz         1b;\n"

             " movq        %%mm2,16(%%edx);\n"
             " movq        %%mm3,24(%%edx);\n"
             " movq        %%mm4,32(%%edx);\n"
             " movq        %%mm5,40(%%edx);\n"

             " femms;\n"
             :
             : "c" (samples), "a" (x), "d" (vv)
             : "memory", "eax", "ecx");

    s[0].v2 = vv[4];
    s[1].v2 = vv[5];
    s[2].v2 = vv[6];
    s[3].v2 = vv[7];
    s[0].v3 = vv[8];
    s[1].v3 = vv[9];
    s[2].v3 = vv[10];
    s[3].v3 = vv[11];
}
/*- End of function --------------------------------------------------------*/
#endif

int dtmf_rx(dtmf_rx_state_t *s, const int16_t amp[], int samples)
{
    float row_energy[4];
    float col_energy[4];
    float famp;
    float v1;
    int i;
    int j;
    int sample;
    int best_row;
    int best_col;
    int limit;
    uint8_t hit;

    hit = 0;
    for (sample = 0;  sample < samples;  sample = limit)
    {
        /* The block length is optimised to meet the DTMF specs. */
        if ((samples - sample) >= (102 - s->current_sample))
            limit = sample + (102 - s->current_sample);
        else
            limit = samples;
#if defined(USE_3DNOW)
        _dtmf_goertzel_update(s->row_out, amp + sample, limit - sample);
        _dtmf_goertzel_update(s->col_out, amp + sample, limit - sample);
#else
        /* The following unrolled loop takes only 35% (rough estimate) of the 
           time of a rolled loop on the machine on which it was developed */
        for (j = sample;  j < limit;  j++)
        {
            famp = amp[j];
            if (s->filter_dialtone)
            {
                /* Sharp notches applied at 350Hz and 440Hz - the two common dialtone frequencies.
                   These are rather high Q, to achieve the required narrowness, without using lots of
                   sections. */
                v1 = 0.98356f*famp + 1.8954426f*s->z350_1 - 0.9691396f*s->z350_2;
                famp = v1 - 1.9251480f*s->z350_1 + s->z350_2;
                s->z350_2 = s->z350_1;
                s->z350_1 = v1;

                v1 = 0.98456f*famp + 1.8529543f*s->z440_1 - 0.9691396f*s->z440_2;
                famp = v1 - 1.8819938f*s->z440_1 + s->z440_2;
                s->z440_2 = s->z440_1;
                s->z440_1 = v1;
            }
            s->energy += famp*famp;
            /* With GCC 2.95, the following unrolled code seems to take about 35%
               (rough estimate) as long as a neat little 0-3 loop */
            v1 = s->row_out[0].v2;
            s->row_out[0].v2 = s->row_out[0].v3;
            s->row_out[0].v3 = s->row_out[0].fac*s->row_out[0].v2 - v1 + famp;
    
            v1 = s->col_out[0].v2;
            s->col_out[0].v2 = s->col_out[0].v3;
            s->col_out[0].v3 = s->col_out[0].fac*s->col_out[0].v2 - v1 + famp;
    
            v1 = s->row_out[1].v2;
            s->row_out[1].v2 = s->row_out[1].v3;
            s->row_out[1].v3 = s->row_out[1].fac*s->row_out[1].v2 - v1 + famp;
    
            v1 = s->col_out[1].v2;
            s->col_out[1].v2 = s->col_out[1].v3;
            s->col_out[1].v3 = s->col_out[1].fac*s->col_out[1].v2 - v1 + famp;
    
            v1 = s->row_out[2].v2;
            s->row_out[2].v2 = s->row_out[2].v3;
            s->row_out[2].v3 = s->row_out[2].fac*s->row_out[2].v2 - v1 + famp;
    
            v1 = s->col_out[2].v2;
            s->col_out[2].v2 = s->col_out[2].v3;
            s->col_out[2].v3 = s->col_out[2].fac*s->col_out[2].v2 - v1 + famp;
    
            v1 = s->row_out[3].v2;
            s->row_out[3].v2 = s->row_out[3].v3;
            s->row_out[3].v3 = s->row_out[3].fac*s->row_out[3].v2 - v1 + famp;

            v1 = s->col_out[3].v2;
            s->col_out[3].v2 = s->col_out[3].v3;
            s->col_out[3].v3 = s->col_out[3].fac*s->col_out[3].v2 - v1 + famp;
        }
#endif
        s->current_sample += (limit - sample);
        if (s->current_sample < 102)
            continue;

        /* We are at the end of a DTMF detection block */
        /* Find the peak row and the peak column */
        row_energy[0] = goertzel_result(&s->row_out[0]);
        best_row = 0;
        col_energy[0] = goertzel_result(&s->col_out[0]);
        best_col = 0;

        for (i = 1;  i < 4;  i++)
        {
            row_energy[i] = goertzel_result(&s->row_out[i]);
            if (row_energy[i] > row_energy[best_row])
                best_row = i;
            col_energy[i] = goertzel_result(&s->col_out[i]);
            if (col_energy[i] > col_energy[best_col])
                best_col = i;
        }
        hit = 0;
        /* Basic signal level test and the twist test */
        if (row_energy[best_row] >= DTMF_THRESHOLD
            &&
            col_energy[best_col] >= DTMF_THRESHOLD
            &&
            col_energy[best_col] < row_energy[best_row]*s->reverse_twist
            &&
            col_energy[best_col]*s->normal_twist > row_energy[best_row])
        {
            /* Relative peak test ... */
            for (i = 0;  i < 4;  i++)
            {
                if ((i != best_col  &&  col_energy[i]*DTMF_RELATIVE_PEAK_COL > col_energy[best_col])
                    ||
                    (i != best_row  &&  row_energy[i]*DTMF_RELATIVE_PEAK_ROW > row_energy[best_row]))
                {
                    break;
                }
            }
            /* ... and fraction of total energy test */
            if (i >= 4
                &&
                (row_energy[best_row] + col_energy[best_col]) > DTMF_TO_TOTAL_ENERGY*s->energy)
            {
                hit = dtmf_positions[(best_row << 2) + best_col];
            }
        }
        /* The logic in the next test should ensure the following for different successive hit patterns:
                -----ABB = start of digit B.
                ----B-BB = start of digit B
                ----A-BB = start of digit B
                BBBBBABB = still in digit B.
                BBBBBB-- = end of digit B
                BBBBBBC- = end of digit B
                BBBBACBB = B ends, then B starts again.
                BBBBBBCC = B ends, then C starts.
                BBBBBCDD = B ends, then D starts.
           This can work with:
                - Back to back differing digits. Back-to-back digits should
                  not happen. The spec. says there should be a gap between digits.
                  However, many real phones do not impose a gap, and rolling across
                  the keypad can produce little or no gap.
                - It tolerates nasty phones that give a very wobbly start to a digit.
                - VoIP can give sample slips. The phase jumps that produces will cause
                  the block it is in to give no detection. This logic will ride over a
                  single missed block, and not falsely declare a second digit. If the
                  hiccup happens in the wrong place on a minimum length digit, however
                  we would still fail to detect that digit. Could anything be done to
                  deal with that? Packet loss is clearly a no-go zone.
                  Note this is only relevant to VoIP using A-law, u-law or similar.
                  Low bit rate codecs scramble DTMF too much for it to be recognised,
                  and often slip in units larger than a sample. */
        if (hit != s->in_digit)
        {
            if (s->last_hit != s->in_digit)
            {
                /* We have two successive indications that something has changed. */
                /* To declare digit on, the hits must agree. Otherwise we declare tone off. */
                hit = (hit  &&  hit == s->last_hit)  ?  hit   :  0;
                if (s->realtime_callback)
                {
                    /* Avoid reporting multiple no digit conditions on flaky hits */
                    if (s->in_digit  ||  hit)
                    {
                        i = (s->in_digit  &&  !hit)  ?  -99  :  rint(log10f(s->energy)*10.0f - 20.08f - 90.30F + DBM0_MAX_POWER);
                        s->realtime_callback(s->realtime_callback_data, hit, i);
                    }
                }
                else
                {
                    if (hit)
                    {
                        if (s->current_digits < MAX_DTMF_DIGITS)
                        {
                            s->digits[s->current_digits++] = (char) hit;
                            s->digits[s->current_digits] = '\0';
                            if (s->callback)
                            {
                                s->callback(s->callback_data, s->digits, s->current_digits);
                                s->current_digits = 0;
                            }
                        }
                        else
                        {
                            s->lost_digits++;
                        }
                    }
                }
                s->in_digit = hit;
            }
        }
        s->last_hit = hit;
        /* Reinitialise the detector for the next block */
        for (i = 0;  i < 4;  i++)
        {
            goertzel_reset(&s->row_out[i]);
            goertzel_reset(&s->col_out[i]);
        }
        s->energy = 0.0f;
        s->current_sample = 0;
    }
    if (s->current_digits  &&  s->callback)
    {
        s->callback(s->callback_data, s->digits, s->current_digits);
        s->digits[0] = '\0';
        s->current_digits = 0;
    }
    return 0;
}
/*- End of function --------------------------------------------------------*/

size_t dtmf_rx_get(dtmf_rx_state_t *s, char *buf, int max)
{
    if (max > s->current_digits)
        max = s->current_digits;
    if (max > 0)
    {
        memcpy(buf, s->digits, max);
        memmove(s->digits, s->digits + max, s->current_digits - max);
        s->current_digits -= max;
    }
    buf[max] = '\0';
    return  max;
}
/*- End of function --------------------------------------------------------*/

void dtmf_rx_set_realtime_callback(dtmf_rx_state_t *s,
                                   tone_report_func_t callback,
                                   void *user_data)
{
    s->realtime_callback = callback;
    s->realtime_callback_data = user_data;
}
/*- End of function --------------------------------------------------------*/

void dtmf_rx_parms(dtmf_rx_state_t *s,
                   int filter_dialtone,
                   int twist,
                   int reverse_twist)
{
    if (filter_dialtone >= 0)
    {
        s->z350_1 = 0.0f;
        s->z350_2 = 0.0f;
        s->z440_1 = 0.0f;
        s->z440_2 = 0.0f;
        s->filter_dialtone = filter_dialtone;
    }
    if (twist >= 0)
        s->normal_twist = powf(10.0f, twist/10.0f);
    if (reverse_twist >= 0)
        s->reverse_twist = powf(10.0f, reverse_twist/10.0f);
}
/*- End of function --------------------------------------------------------*/

dtmf_rx_state_t *dtmf_rx_init(dtmf_rx_state_t *s,
                              dtmf_rx_callback_t callback,
                              void *user_data)
{
    int i;
    static int initialised = FALSE;

    s->callback = callback;
    s->callback_data = user_data;
    s->realtime_callback = NULL;
    s->realtime_callback_data = NULL;
    s->filter_dialtone = FALSE;
    s->normal_twist = DTMF_NORMAL_TWIST;
    s->reverse_twist = DTMF_REVERSE_TWIST;

    s->in_digit = 0;
    s->last_hit = 0;

    if (!initialised)
    {
        for (i = 0;  i < 4;  i++)
        {
            make_goertzel_descriptor(&dtmf_detect_row[i], dtmf_row[i], 102);
            make_goertzel_descriptor(&dtmf_detect_col[i], dtmf_col[i], 102);
        }
        initialised = TRUE;
    }
    for (i = 0;  i < 4;  i++)
    {
        goertzel_init(&s->row_out[i], &dtmf_detect_row[i]);
        goertzel_init(&s->col_out[i], &dtmf_detect_col[i]);
    }
    s->energy = 0.0f;
    s->current_sample = 0;
    s->lost_digits = 0;
    s->current_digits = 0;
    s->digits[0] = '\0';
    return s;
}
/*- End of function --------------------------------------------------------*/

#if 0
static void dtmf_tx_initialise(void)
{
    int row;
    int col;

    if (dtmf_tx_inited)
        return;
    for (row = 0;  row < 4;  row++)
    {
        for (col = 0;  col < 4;  col++)
        {
            make_tone_gen_descriptor(&dtmf_digit_tones[row*4 + col],
                                     (int) dtmf_row[row],
                                     DEFAULT_DTMF_TX_LEVEL,
                                     (int) dtmf_col[col],
                                     DEFAULT_DTMF_TX_LEVEL,
                                     DEFAULT_DTMF_TX_ON_TIME,
                                     DEFAULT_DTMF_TX_OFF_TIME,
                                     0,
                                     0,
                                     FALSE);
        }
    }
    dtmf_tx_inited = TRUE;
}
/*- End of function --------------------------------------------------------*/

int dtmf_tx(dtmf_tx_state_t *s, int16_t amp[], int max_samples)
{
    int len;
    size_t dig;
    char *cp;

    len = 0;
    if (s->tones.current_section >= 0)
    {
        /* Deal with the fragment left over from last time */
        len = tone_gen(&(s->tones), amp, max_samples);
    }
    dig = 0;
    while (dig < s->current_digits  &&  len < max_samples)
    {
        /* Step to the next digit */
        if ((cp = strchr(dtmf_positions, s->digits[dig++])) == NULL)
            continue;
        tone_gen_init(&(s->tones), &(s->tone_descriptors[cp - dtmf_positions]));
        len += tone_gen(&(s->tones), amp + len, max_samples - len);
    }
    if (dig)
    {
        /* Shift out the consumed digits */
        s->current_digits -= dig;
        memmove(s->digits, s->digits + dig, s->current_digits);
    }
    return len;
}
/*- End of function --------------------------------------------------------*/

size_t dtmf_tx_put(dtmf_tx_state_t *s, const char *digits)
{
    size_t len;

    /* This returns the number of characters that would not fit in the buffer.
       The buffer will only be loaded if the whole string of digits will fit,
       in which case zero is returned. */
    if ((len = strlen(digits)) > 0)
    {
        if (s->current_digits + len <= MAX_DTMF_DIGITS)
        {
            memcpy(s->digits + s->current_digits, digits, len);
            s->current_digits += len;
            len = 0;
        }
        else
        {
            len = MAX_DTMF_DIGITS - s->current_digits;
        }
    }
    return len;
}
/*- End of function --------------------------------------------------------*/

dtmf_tx_state_t *dtmf_tx_init(dtmf_tx_state_t *s)
{
    if (!dtmf_tx_inited)
        dtmf_tx_initialise();
    s->tone_descriptors = dtmf_digit_tones;
    tone_gen_init(&(s->tones), &dtmf_digit_tones[0]);
    s->current_sample = 0;
    s->current_digits = 0;
    s->tones.current_section = -1;
    return s;
}
#endif
/*- End of function --------------------------------------------------------*/
/*- End of file ------------------------------------------------------------*/
