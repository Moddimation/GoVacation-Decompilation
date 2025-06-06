/* @(#)w_atanh.c 1.2 95/01/04 */
/* $Id: w_atanh.c,v 1.5 2004/02/26 17:03:29 ceciliar Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/*
 * wrapper atanh(x)
 */

#include "fdlibm.h"
#if _MSL_FLOATING_POINT
    #if _MSL_C99

        #ifdef __STDC__
double
atanh (double x) /* wrapper atanh */
        #else
double
       atanh (x)                                 /* wrapper atanh */
double x;
        #endif
{
        #ifdef _IEEE_LIBM
    return __ieee754_atanh (x);
        #else
    double z, y;
    z = __ieee754_atanh (x);
    if (_LIB_VERSION == _IEEE_ || isnan (x))
        return z;
    y = fabs (x);
    if (y >= 1.0)
    {
        if (y > 1.0)
            return __kernel_standard (x, x, 30); /* atanh(|x|>1) */
        else
            return __kernel_standard (x, x, 31); /* atanh(|x|==1) */
    }
    else
        return z;
        #endif
}
    #endif       /* _MSL_C99 */
#endif           /* _MSL_FLOATING_POINT  */
