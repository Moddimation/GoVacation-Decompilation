/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:25:49 $
 * $Revision: 1.30.2.1 $
 */

#ifdef __MWERKS__
    #if __MWERKS__ >= 0x3000
        #pragma notonce
    #endif /* __MWERKS__ >= 0x3000 */
#endif     /* __MWERKS__ */

#include <ansi_parms.h>

#if !_MSL_USING_MW_C_HEADERS
    #error You must have the non-MSL C header file access path before the MSL access path
#else
    #include <cassert>
#endif     /* _MSL_USING_MW_C_HEADERS */

           /*
            * Change record
            * hh  971206 Created.
            * mm  980724 Deleted wrappers to allow multiple includes.  MW 08171
            * hh  991112 Removed namespace support
            * JWW 001208 Added case for targeting Mach-O
            * JWW 011027 Use _MSL_USING_MW_C_HEADERS as generic header test instead of specific Mach-O test
            */
