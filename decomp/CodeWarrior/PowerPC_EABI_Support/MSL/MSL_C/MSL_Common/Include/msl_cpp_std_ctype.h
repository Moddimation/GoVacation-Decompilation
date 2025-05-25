/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:26:07 $
 * $Revision: 1.5.2.1 $
 */

#ifndef _MSL_CPP_STD_CTYPE_H
    #define _MSL_CPP_STD_CTYPE_H

    #ifdef __cplusplus

namespace std
{

        #if _MSL_OS_X_VERSION > 1
using ::isalnum;
using ::isalpha;
            #if _MSL_C99
using ::isblank;
            #endif /* _MSL_C99 */
using ::iscntrl;
using ::isdigit;
using ::isgraph;
using ::islower;
using ::isprint;
using ::ispunct;
using ::isspace;
using ::isupper;
using ::isxdigit;
        #endif     /* _MSL_OS_X_VERSION > 1 */
using ::tolower;
using ::toupper;

} /* namespace std */

    #endif /* __cplusplus */

#endif     /* _MSL_CPP_STD_CTYPE_H */

           /* Change record:
            * JWW 050128 Put C items into the std namespace
            */
