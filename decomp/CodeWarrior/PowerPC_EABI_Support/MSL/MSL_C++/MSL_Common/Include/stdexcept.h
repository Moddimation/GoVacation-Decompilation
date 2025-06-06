/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:27:05 $
 * $Revision: 1.21.2.1 $
 */

// stdexcept.h

#ifndef _STDEXCEPT_H
    #define _STDEXCEPT_H

    #include <stdexcept>

    #ifndef _MSL_NO_CPP_NAMESPACE
using std::domain_error;
using std::invalid_argument;
using std::length_error;
using std::logic_error;
using std::out_of_range;
using std::overflow_error;
using std::range_error;
using std::runtime_error;
using std::underflow_error;
    #endif // _MSL_NO_CPP_NAMESPACE

#endif     // _STDEXCEPT_H

// hh 971206  File created.
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
