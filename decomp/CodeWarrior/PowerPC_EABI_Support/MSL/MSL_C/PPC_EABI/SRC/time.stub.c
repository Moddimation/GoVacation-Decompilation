/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:23:23 $
 * $Revision: 1.7.2.1 $
 */

/* Include your OS dependent include files */
#include <time.h>

#if !_MSL_OS_TIME_SUPPORT
    #error time.stub.c can't be in your project if _MSL_OS_TIME_SUPPORT is defined to 0.
#endif

clock_t
__get_clock (void)
{
#warning "replace 0 with an OS call"
    return 0;
}

time_t
__get_time (void)
{
#warning "replace 0 with an OS call"
    return 0;
}

int
__to_gm_time (time_t* time)
{
#warning "replace 0 with an OS call"
    return 0;
}

/*  Change Record
 *	20-Jul-97 MEA  First code release.
 */
