/* @(#)s_copysign.c 1.2 95/01/04 */
/* $Id: s_copysign.c,v 1.4 2004/11/29 17:41:11 cgalvan Exp $ */
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
 * copysign(double x, double y)
 * copysign(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#include "fdlibm.h"

#if _MSL_FLOATING_POINT
    #if _MSL_C99 || _MSL_USES_SUN_MATH_LIB
        #ifdef __STDC__
double
copysign (double x, double y)
        #else
double
       __copysign (x, y)
double x, y;
        #endif
{
    __HI (x) = (__HI (x) & 0x7fffffff) | (__HI (y) & 0x80000000);
    return x;
}
    #endif /* _MSL_C99 || _MSL_USES_SUN_MATH_LIB */
#endif     /* _MSL_FLOATING_POINT  */
