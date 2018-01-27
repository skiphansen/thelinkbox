/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

static char	ident[] = "$Header: /cvsroot/cqinet/gsm/src/gsm_create.c,v 1.1.1.1 2002/10/06 16:40:27 wb6ymh Exp $";

#ifndef _WIN32
	#include	"config.h"
#else
	#include	"config.h.win32"
#endif

#ifdef	HAVE_STDLIB_H
#include	<stdlib.h>
#else
#	include "proto.h"
	extern char	* memset P((char *, int, int));
#endif
#include <stdio.h>

#include "gsm.h"
#include "private.h"
#include "proto.h"

gsm gsm_create P0()
{
	gsm  r;

#ifdef	USE_TABLE_MUL

	static int mul_init = 0;
	if (!mul_init) {
		mul_init = 1;
		init_umul_table();
	}

#endif

	r = (gsm)malloc(sizeof(struct gsm_state));
	if (!r) return r;

	memset((char *)r, 0, sizeof(*r));
	r->nrp = 40;

	return r;
}
