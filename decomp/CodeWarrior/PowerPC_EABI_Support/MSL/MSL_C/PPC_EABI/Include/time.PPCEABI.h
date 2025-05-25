/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:28:45 $
 * $Revision: 1.7.2.1 $
 */

#ifndef _MSL_TIME_PPCEABI_H
    #define _MSL_TIME_PPCEABI_H

    #if _MSL_CLOCK_T_AVAILABLE
        #if defined(_Generic_PPC_EABI_OS)
            #define _MSL_CLOCKS_PER_SEC 60
            #define _MSL_CLOCK_T        unsigned long
        #endif
    #endif

#endif /* _MSL_TIME_PPCEABI_H */

       /* Change record:
        * JWW 011101 New file to make time information platform independent
        */
