/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:28:38 $
 * $Revision: 1.5.2.1 $
 */

#ifndef __ansi_prefix_SZ__
    #define __ansi_prefix_SZ__

/************************************************/
/* 		Flags to reduce code and data size 		*/
/************************************************/

    #ifndef _MSL_STRERROR_KNOWS_ERROR_NAMES
        #define _MSL_STRERROR_KNOWS_ERROR_NAMES                                                \
            0              /* 0: for reduced code and data size by reducing MSL functionality */
    #endif

    #ifndef _MSL_ASSERT_DISPLAYS_FUNC
        #define _MSL_ASSERT_DISPLAYS_FUNC                                                      \
            0              /* 0: for reduced code and data size by reducing MSL functionality */
    #endif

    #ifndef _MSL_C_LOCALE_ONLY
        #define _MSL_C_LOCALE_ONLY                                                             \
            1              /* 1: for reduced code and data size by reducing MSL functionality */
    #endif

    #ifndef _MSL_C99
        #define _MSL_C99 0 /* 0: for reduced code and data size by reducing MSL functionality  \
                            */
    #endif

    #ifndef _MSL_C_HAS_CPP_EXTENSIONS
        #define _MSL_C_HAS_CPP_EXTENSIONS 1
    #endif

    #ifndef _MSL_FLOATING_POINT_IO
        #define _MSL_FLOATING_POINT_IO                                                         \
            0              /* 0: for reduced code and data size by reducing MSL functionality */
    #endif

    #include <ansi_prefix.PPCEABI.bare.h>

#endif                     /* __ansi_prefix_SZ__ */

                           /* Change record:
                            * MEA 040506 created.
                            */
