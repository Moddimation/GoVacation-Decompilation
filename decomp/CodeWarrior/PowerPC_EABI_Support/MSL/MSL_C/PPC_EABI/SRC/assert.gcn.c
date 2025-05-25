/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:23:18 $
 * $Revision: 1.6.2.1 $
 */

#include <ansi_parms.h>
#include <assert.h>
#include <assert_api.h>
#include <stdio.h>
#include <stdlib.h>

_MSL_IMP_EXP_C void _MSL_CDECL __assertion_failed (char const * condition,
                                                   char const * filename,
                                                   int          lineno);

void
__assertion_failed (char const * condition, char const * filename, int lineno)
{
    __msl_assertion_failed (condition, filename, NULL, lineno);
}

/* Change record:
 * sw 021209 First code release.
 * sw 021209 Added __assertion_failed function so older libraries will link
 */
