/*
 * SpanDSP - a series of DSP components for telephony
 *
 * dtmf.h - 
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
 * $Id dtmf.h,v 1.7 2007/02/27 16:52:16 steveu Exp $
 *
 * $Log: dtmf.h,v $
 * Revision 1.1.1.1  2007/04/07 22:27:59  wb6ymh
 * Initial import of a small subset of the spandsp library for CQiNet.
 * Currently all we are using are the DTMF decoder routines.
 *
 */

#if !defined(_DTMF_H_)
#define _DTMF_H_

/*! \page dtmf_rx_page DTMF receiver
\section dtmf_rx_page_sec_1 What does it do?
The DTMF receiver detects the standard DTMF digits. It is compliant with
ITU-T Q.23, ITU-T Q.24, and the local DTMF specifications of most administrations.
Its passes the test suites. It also scores *very* well on the standard
talk-off tests. 

The current design uses floating point extensively. It is not tolerant of DC.
It is expected that a DC restore stage will be placed before the DTMF detector.
Unless the dial tone filter is switched on, the detector has poor tolerance
of dial tone. Whether this matter depends on your application. If you are using
the detector in an IVR application you will need proper echo cancellation to
get good performance in the presence of speech prompts, so dial tone will not
exist. If you do need good dial tone tolerance, a dial tone filter can be
enabled in the detector.

\section dtmf_rx_page_sec_2 How does it work?
Like most other DSP based DTMF detector's, this one uses the Goertzel algorithm
to look for the DTMF tones. What makes each detector design different is just how
that algorithm is used.

Basic DTMF specs:
    - Minimum tone on = 40ms
    - Minimum tone off = 50ms
    - Maximum digit rate = 10 per second
    - Normal twist <= 8dB accepted
    - Reverse twist <= 4dB accepted
    - S/N >= 15dB will detect OK
    - Attenuation <= 26dB will detect OK
    - Frequency tolerance +- 1.5% will detect, +-3.5% will reject

TODO:
*/

/*! \page dtmf_tx_page DTMF tone generation
\section dtmf_tx_page_sec_1 What does it do?

The DTMF tone generation module provides for the generation of the
repertoire of 16 DTMF dual tones. 

\section dtmf_tx_page_sec_2 How does it work?
*/

#define MAX_DTMF_DIGITS 128

typedef void (*dtmf_rx_callback_t)(void *user_data, const char *digits, int len);

/*!
    DTMF generator state descriptor. This defines the state of a single
    working instance of a DTMF generator.
*/
typedef struct
{
    tone_gen_descriptor_t *tone_descriptors;
    tone_gen_state_t tones;
    char digits[MAX_DTMF_DIGITS + 1];
    int current_sample;
    size_t current_digits;
} dtmf_tx_state_t;

/*!
    DTMF digit detector descriptor.
*/
typedef struct
{
    /*! Optional callback funcion to deliver received digits. */
    dtmf_rx_callback_t callback;
    /*! An opaque pointer passed to the callback function. */
    void *callback_data;
    /*! Optional callback funcion to deliver real time digit state changes. */
    tone_report_func_t realtime_callback;
    /*! An opaque pointer passed to the real time callback function. */
    void *realtime_callback_data;
    /*! TRUE if dialtone should be filtered before processing */
    int filter_dialtone;
    /*! Maximum acceptable "normal" (lower bigger than higher) twist ratio */
    float normal_twist;
    /*! Maximum acceptable "reverse" (higher bigger than lower) twist ratio */
    float reverse_twist;

    /*! 350Hz filter state for the optional dialtone filter */
    float z350_1;
    float z350_2;
    /*! 440Hz filter state for the optional dialtone filter */
    float z440_1;
    float z440_2;

    /*! Tone detector working states */
    goertzel_state_t row_out[4];
    goertzel_state_t col_out[4];
    /*! The accumlating total energy on the same period over which the Goertzels work. */
    float energy;
    /*! The result of the last tone analysis. */
    uint8_t last_hit;
    /*! The confirmed digit we are currently receiving */
    uint8_t in_digit;
    /*! The current sample number within a processing block. */
    int current_sample;

    /*! The received digits buffer. This is a NULL terminated string. */
    char digits[MAX_DTMF_DIGITS + 1];
    /*! The number of digits currently in the digit buffer. */
    int current_digits;
    /*! The number of digits which have been lost due to buffer overflows. */
    int lost_digits;
} dtmf_rx_state_t;

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Generate a buffer of DTMF tones.
    \param s The DTMF generator context.
    \param amp The buffer for the generated signal.
    \param max_samples The required number of generated samples.
    \return The number of samples actually generated. This may be less than 
            samples if the input buffer empties. */
int dtmf_tx(dtmf_tx_state_t *s, int16_t amp[], int max_samples);

/*! \brief Put a string of digits in a DTMF generator's input buffer.
    \param s The DTMF generator context.
    \param digits The string of digits to be added.
    \return The number of digits actually added. This may be less than the
            length of the digit string, if the buffer fills up. */
size_t dtmf_tx_put(dtmf_tx_state_t *s, const char *digits);

/*! \brief Initialise a DTMF tone generator context.
    \param s The DTMF generator context.
    \return A pointer to the DTMF generator context. */
dtmf_tx_state_t *dtmf_tx_init(dtmf_tx_state_t *s);

/*! Set a optional realtime callback for a DTMF receiver context. This function
    is called immediately a confirmed state change occurs in the received DTMF. It
    is called with the ASCII value for a DTMF tone pair, or zero to indicate no tone
    is being received.
    \brief Set a realtime callback for a DTMF receiver context.
    \param s The DTMF receiver context.
    \param callback Callback routine used to report the start and end of digits.
    \param user_data An opaque pointer which is associated with the context,
           and supplied in callbacks. */
void dtmf_rx_set_realtime_callback(dtmf_rx_state_t *s,
                                   tone_report_func_t callback,
                                   void *user_data);

/*! \brief Adjust a DTMF receiver context.
    \param s The DTMF receiver context.
    \param filter_dialtone TRUE to enable filtering of dialtone, FALSE
           to disable, < 0 to leave unchanged.
    \param twist Acceptable twist, in dB. < 0 to leave unchanged.
    \param reverse_twist Acceptable reverse twist, in dB. < 0 to leave unchanged. */
void dtmf_rx_parms(dtmf_rx_state_t *s, int filter_dialtone, int twist, int reverse_twist);

/*! Process a block of received DTMF audio samples.
    \brief Process a block of received DTMF audio samples.
    \param s The DTMF receiver context.
    \param amp The audio sample buffer.
    \param samples The number of samples in the buffer.
    \return The number of samples unprocessed. */
int dtmf_rx(dtmf_rx_state_t *s, const int16_t amp[], int samples);

/*! \brief Get a string of digits from a DTMF receiver's output buffer.
    \param s The DTMF receiver context.
    \param digits The buffer for the received digits.
    \param max The maximum  number of digits to be returned,
    \return The number of digits actually returned. */
size_t dtmf_rx_get(dtmf_rx_state_t *s, char *digits, int max);

/*! \brief Initialise a DTMF receiver context.
    \param s The DTMF receiver context.
    \param callback An optional callback routine, used to report received digits. If
           no callback routine is set, digits may be collected, using the dtmf_rx_get()
           function.
    \param user_data An opaque pointer which is associated with the context,
           and supplied in callbacks.
    \return A pointer to the DTMF receiver context. */
dtmf_rx_state_t *dtmf_rx_init(dtmf_rx_state_t *s,
                              dtmf_rx_callback_t callback,
                              void *user_data);

#ifdef __cplusplus
}
#endif

#endif
/*- End of file ------------------------------------------------------------*/
