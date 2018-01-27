/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /cvsroot/cqinet/gsm/src/gsm_destroy.c,v 1.1.1.1 2002/10/06 16:40:27 wb6ymh Exp $ */

#include "gsm.h"
#include "proto.h"

void gsm_destroy P1((S), gsm S)
{
	if (S) free((char *)S);
}
