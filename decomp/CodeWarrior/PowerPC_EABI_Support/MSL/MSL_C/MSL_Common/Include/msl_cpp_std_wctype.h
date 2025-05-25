/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:26:12 $
 * $Revision: 1.5.2.1 $
 */

#ifndef _MSL_CPP_STD_WCTYPE_H
    #define _MSL_CPP_STD_WCTYPE_H

    #ifdef __cplusplus

        #if _MSL_OS_X_VERSION > 2

namespace std
{

            #if _MSL_WIDE_CHAR
using ::iswalnum;
using ::iswalpha;
using ::wctrans_t;
using ::wctype_t;
using ::wint_t;
                #if _MSL_C99
using ::iswblank;
                #endif /* _MSL_C99 */
using ::iswcntrl;
using ::iswctype;
using ::iswdigit;
using ::iswgraph;
using ::iswlower;
using ::iswprint;
using ::iswpunct;
using ::iswspace;
using ::iswupper;
using ::iswxdigit;
using ::towctrans;
using ::towlower;
using ::towupper;
using ::wctrans;
using ::wctype;
            #endif     /* _MSL_WIDE_CHAR */

} /* namespace std */

        #endif /* _MSL_OS_X_VERSION > 2 */

    #endif     /* __cplusplus */

#endif         /* _MSL_CPP_STD_WCTYPE_H */

               /* Change record:
                * JWW 050128 Put C items into the std namespace
                */
