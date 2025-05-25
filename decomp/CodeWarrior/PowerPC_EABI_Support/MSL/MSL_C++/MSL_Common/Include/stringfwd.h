/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:27:07 $
 * $Revision: 1.13.2.1 $
 */

// stringfwd.h

// WARNING
//
// stringfwd.h is a non-standard, non-portable header.
//
// WARNING

#ifndef _STRINGFWD_H
    #define _STRINGFWD_H

    #include <stringfwd>

    #ifndef _MSL_NO_CPP_NAMESPACE

using std::allocator;
using std::basic_string;
using std::char_traits;
using std::string;
        #ifndef _MSL_NO_WCHART_CPP_SUPPORT
using std::wstring;
        #endif

    #endif // _MSL_NO_CPP_NAMESPACE

#endif     // _STRINGFWD_H

// hh 991114 Created
