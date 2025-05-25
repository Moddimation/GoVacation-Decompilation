/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:23:21 $
 * $Revision: 1.6.2.1 $
 */

/* Include your OS dependent include files */
#include "pool_alloc.h"

#ifdef _No_Alloc_OS_Support
    #error pool_alloc.stub.c can't be in your project if _No_Alloc_OS_Support is defined.
#endif

void*
__sys_alloc (__std (size_t)) /*  mf 990715 */
{
#warning "replace 0 with a call to your OS'es allocator"
    return 0;
}

void
__sys_free (void*)           /*  mf 990715 */
{
#warning "replace with a call to your OS'es deallocator"
}

/*  Change Record
 *	20-Jul-97 MEA  First code release.
 */
