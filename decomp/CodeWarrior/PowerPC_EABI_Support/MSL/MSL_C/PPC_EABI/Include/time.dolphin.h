/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:28:44 $
 * $Revision: 1.6.2.1 $
 */

#ifndef _MSL_TIME_DOLPHIN_H
    #define _MSL_TIME_DOLPHIN_H

    #if _MSL_CLOCK_T_AVAILABLE

        #include <dolphin/os.h>

        #define _MSL_CLOCKS_PER_SEC OS_TIMER_CLOCK
        #define _MSL_CLOCK_T        long long

    #endif

#endif /* _MSL_TIME_DOLPHIN_H */

       /* Change record:
        * JWW 011101 New file to make time information platform independent
        * cc  020801 Added os.h
        */
