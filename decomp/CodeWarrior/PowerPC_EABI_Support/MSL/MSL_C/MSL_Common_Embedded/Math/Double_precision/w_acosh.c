
/* @(#)w_acosh.c 1.2 95/01/04 */
/* $Id: w_acosh.c,v 1.5 2004/02/26 17:03:29 ceciliar Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/*
 * wrapper acosh(x)
 */

#include "fdlibm.h"
#if _MSL_FLOATING_POINT

    #if _MSL_C99

        #ifdef __STDC__
double
acosh (double x) /* wrapper acosh */
        #else
double
       acosh (x)                             /* wrapper acosh */
double x;
        #endif
{
        #ifdef _IEEE_LIBM
    return __ieee754_acosh (x);
        #else
    double z;
    z = __ieee754_acosh (x);
    if (_LIB_VERSION == _IEEE_ || isnan (x))
        return z;
    if (x < 1.0)
    {
        return __kernel_standard (x, x, 29); /* acosh(x<1) */
    }
    else
        return z;
        #endif
}
    #endif       /* _MSL_C99 */
#endif           /* _MSL_FLOATING_POINT  */
