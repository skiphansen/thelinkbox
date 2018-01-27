/*****************************************************************************/

/*
 *      costab.h  --  User soundmodem commonly used tables.
 *
 *      Copyright (C) 1996-2000
 *        Thomas Sailer (sailer@ife.ee.ethz.ch)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: costab.h,v 1.1 2008/12/25 15:36:35 wb6ymh Exp $
 */

#ifndef _COSTAB_H
#define _COSTAB_H

/* --------------------------------------------------------------------- */

#include "modem.h"

/* --------------------------------------------------------------------- */

extern const int16_t afsk_costab[512];

#define COS(x) afsk_costab[((x)>>7)&0x1ff]
#define SIN(x) COS((x)+0xc000)

/* --------------------------------------------------------------------- */
#endif /* _COSTAB_H */
