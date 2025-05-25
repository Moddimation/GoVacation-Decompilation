/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:27:08 $
 * $Revision: 1.21.2.1 $
 */

// typeinfo.h

#ifndef _TYPEINFO_H
    #define _TYPEINFO_H

    #include <typeinfo>

    #ifndef _MSL_NO_CPP_NAMESPACE
using std::bad_cast;
using std::bad_typeid;
using std::type_info;
    #endif // _MSL_NO_CPP_NAMESPACE

#endif     // _TYPEINFO_H

// hh 971206  Added namespace support
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
