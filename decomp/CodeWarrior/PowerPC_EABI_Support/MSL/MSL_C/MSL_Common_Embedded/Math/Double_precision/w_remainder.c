/* @(#)w_remainder.c 1.2 95/01/04 */
/* $Id: w_remainder.c,v 1.6 2004/02/26 17:03:30 ceciliar Exp $ */
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
 * wrapper remainder(x,p)
 */

#include "fdlibm.h"
#if _MSL_FLOATING_POINT
    #if !defined(__arm)

        #if _MSL_C99

            #ifdef __STDC__
double
remainder (double x, double y) /* wrapper remainder */
            #else
double
       remainder (x, y)                      /* wrapper remainder */
double x, y;
            #endif
{
            #ifdef _IEEE_LIBM
    return __ieee754_remainder (x, y);
            #else
    double z;
    z = __ieee754_remainder (x, y);
    if (_LIB_VERSION == _IEEE_ || isnan (y))
        return z;
    if (y == 0.0)
        return __kernel_standard (x, y, 28); /* remainder(x,0) */
    else
        return z;
            #endif
}
        #endif                 /* _MSL_C99 */
    #endif                     /*!defined(__arm) */
#endif                         /* _MSL_FLOATING_POINT  */
