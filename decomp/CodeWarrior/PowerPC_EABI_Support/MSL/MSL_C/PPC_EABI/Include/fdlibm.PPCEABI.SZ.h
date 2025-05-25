/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:28:41 $
 * $Revision: 1.3.2.1 $
 */

/*
 *	fdlibm.PPCEABI.h
 */

#ifndef __fdlibm_PPCEABI_h__
#define __fdlibm_PPCEABI_h__

/* This flag is for when building fdlibm, the inlines in math_ppc.h won't conflict
   with the definition in the single precision files. But the inlines will be the
   default behavior for users since __USING_IEEE_MATH is on by default
*/
#define __BUILD_FDLIBM 1

#include <ansi_prefix.PPCEABI.bare.SZ.h>

#endif /* __fdlibm_PPCEABI_h__ */
