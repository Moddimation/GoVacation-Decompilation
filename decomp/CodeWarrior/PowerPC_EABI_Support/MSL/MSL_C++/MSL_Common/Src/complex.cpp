/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:25:17 $
 * $Revision: 1.20.2.1 $
 */

// complex.cpp

#include <complex>

#if !defined(_MSL_NO_FLOATING_POINT) && !defined(_MSL_NO_MATH_LIB)

#endif // !defined(_MSL_NO_FLOATING_POINT) && !defined(_MSL_NO_MATH_LIB)

// hh  971220 fixed MOD_INCLUDE
// mf  980130 merged in division algorithm
// hh  980408 wrapped in #ifndef _No_Floating_Point
// hh  990120 changed name of MSIPL flags
// hh  991230 Fixed up _MSL_CX_LIMITED_RANGE version s*= and /= to work when this == &rhs
// blc 011211 Added _MSL_NO_MATH_LIB check
// hh  020226 No longer support compilers that don't support member templates
// hh 040217 Changed _No_Floating_Point to _MSL_NO_FLOATING_POINT
