/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:23:19 $
 * $Revision: 1.5.2.1 $
 */

#include "critical_regions.h"

#if _MSL_THREADSAFE

void
__init_critical_regions (void)
{}

void
__kill_critical_regions (void)
{}

void
__begin_critical_region (int region)
{
    #pragma unused(region)
}

void
__end_critical_region (int region)
{
    #pragma unused(region)
}

#endif

/*  Change Record
 *	16-Oct-95 JFH  First code release.
 */
