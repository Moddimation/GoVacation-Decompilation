
/* @(#)e_cosh.c 1.2 95/01/04 */
/* $Id: e_cosh.c,v 1.5 2004/05/20 16:11:04 ceciliar Exp $ */
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

/* __ieee754_cosh(x)
 * Method :
 * mathematically cosh(x) if defined to be (exp(x)+exp(-x))/2
 *	1. Replace x by |x| (cosh(x) = cosh(-x)).
 *	2.
 *		                                        [ exp(x) - 1 ]^2
 *	    0        <= x <= ln2/2  :  cosh(x) := 1 + -------------------
 *			       			           2*exp(x)
 *
 *		                                  exp(x) +  1/exp(x)
 *	    ln2/2    <= x <= 22     :  cosh(x) := -------------------
 *			       			          2
 *	    22       <= x <= lnovft :  cosh(x) := exp(x)/2
 *	    lnovft   <= x <= ln2ovft:  cosh(x) := exp(x/2)/2 * exp(x/2)
 *	    ln2ovft  <  x	    :  cosh(x) := big*big (overflow)
 *
 * Special cases:
 *	cosh(x) is |x| if x is +INF, -INF, or NaN.
 *	only cosh(0)=1 is exact for finite x.
 */

#include "fdlibm.h"
#if _MSL_FLOATING_POINT

    #ifdef __STDC__
static const double one = 1.0, half = 0.5, big = 1.0e300;
    #else
static double one = 1.0, half = 0.5, big = 1.0e300;
    #endif

    #ifdef __STDC__
double
__ieee754_cosh (double x)
    #else
double
       __ieee754_cosh (x)
double x;
    #endif
{
    double  t, w;
    _INT32  ix; /*- cc 020130 -*/
    _UINT32 lx; /*- cc 020130 -*/

                /* High word of |x|. */
    ix  = __HI (x);
    ix &= 0x7fffffff;

    /* x is INF or NaN */
    if (ix >= 0x7ff00000)
        return x * x;

    /* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */
    if (ix < 0x3fd62e43)
    {
        t = expm1 (fabs (x));
        w = one + t;
        if (ix < 0x3c800000)
            return w; /* cosh(tiny) = 1 */
        return one + (t * t) / (w + w);
    }

                      /* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
    if (ix < 0x40360000)
    {
        t = __ieee754_exp (fabs (x));
        return half * t + half / t;
    }

    /* |x| in [22, log(maxdouble)] return half*exp(|x|) */
    if (ix < 0x40862E42)
        return half * __ieee754_exp (fabs (x));

    /* |x| in [log(maxdouble), overflowthresold] */
    lx = *((((*(_UINT32*)&one) >> 29)) + (_UINT32*)&x); /*- cc 020130 -*/
    if (ix < 0x408633CE || (ix == 0x408633ce) && (lx <= (_UINT32)0x8fb9f87d))
    {
        w = __ieee754_exp (half * fabs (x));
        t = half * w;
        return t * w;
    }

    /* |x| > overflowthresold, cosh(x) overflow */
    return big * big;
}
#endif /* _MSL_FLOATING_POINT  */
