/* @(#)s_tanh.c 1.2 95/01/04 */
/* $Id: s_tanh.c,v 1.4 2004/02/26 17:03:28 ceciliar Exp $ */
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

/* Tanh(x)
 * Return the Hyperbolic Tangent of x
 *
 * Method :
 *				       x    -x
 *				      e  - e
 *	0. tanh(x) is defined to be -----------
 *				       x    -x
 *				      e  + e
 *	1. reduce x to non-negative by tanh(-x) = -tanh(x).
 *	2.  0      <= x <= 2**-55 : tanh(x) := x*(one+x)
 *					        -t
 *	    2**-55 <  x <=  1     : tanh(x) := -----; t = expm1(-2x)
 *					       t + 2
 *						     2
 *	    1      <= x <=  22.0  : tanh(x) := 1-  ----- ; t=expm1(2x)
 *						   t + 2
 *	    22.0   <  x <= INF    : tanh(x) := 1.
 *
 * Special cases:
 *	tanh(NaN) is NaN;
 *	only tanh(0)=0 is exact for finite argument.
 */

#include "fdlibm.h"
#if _MSL_FLOATING_POINT
    #ifdef __STDC__
static const double one = 1.0, two = 2.0, tiny = 1.0e-300;
    #else
static double one = 1.0, two = 2.0, tiny = 1.0e-300;
    #endif

    #ifdef __STDC__
double
tanh (double x)
    #else
double
       tanh (x)
double x;
    #endif
{
    double t, z;
    _INT32 jx, ix; /*- cc 020130 -*/

                   /* High word of |x|. */
    jx = __HI (x);
    ix = jx & 0x7fffffff;

    /* x is INF or NaN */
    if (ix >= 0x7ff00000)
    {
        if (jx >= 0)
            return one / x + one; /* tanh(+-inf)=+-1 */
        else
            return one / x - one; /* tanh(NaN) = NaN */
    }

                                  /* |x| < 22 */
    if (ix < 0x40360000)
    {                             /* |x|<22 */
        if (ix < 0x3c800000)      /* |x|<2**-55 */
            return x * (one + x); /* tanh(small) = small */
        if (ix >= 0x3ff00000)
        {                         /* |x|>=1  */
            t = expm1 (two * fabs (x));
            z = one - two / (t + two);
        }
        else
        {
            t = expm1 (-two * fabs (x));
            z = -t / (t + two);
        }
        /* |x| > 22, return +-1 */
    }
    else
    {
        z = one - tiny; /* raised inexact flag */
    }
    return (jx >= 0) ? z : -z;
}
#endif                  /* _MSL_FLOATING_POINT  */
