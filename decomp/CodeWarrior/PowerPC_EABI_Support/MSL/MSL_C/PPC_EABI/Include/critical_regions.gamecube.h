/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:28:39 $
 * $Revision: 1.3.2.1 $
 */

/*
 *	Routines
 *	--------
 *		__init_critical_regions
 *		__begin_critical_region
 *		__end_critical_region
 *
 */

#ifndef _MSL_CRITICAL_REGIONS_GAMECUBE_H
#define _MSL_CRITICAL_REGIONS_GAMECUBE_H

_MSL_BEGIN_EXTERN_C

//_MSL_IMP_EXP_C extern CRITICAL_SECTION __cs[num_critical_regions];

void __init_critical_regions (void);

void __kill_critical_regions (void);

void __begin_critical_region (int region);

void __end_critical_region (int region);

_MSL_END_EXTERN_C

#endif /* _MSL_CRITICAL_REGIONS_GAMECUBE_H */
