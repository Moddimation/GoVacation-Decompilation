/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:26:50 $
 * $Revision: 1.22.2.1 $
 */

// exception.h

#ifndef _EXCEPTION_H
    #define _EXCEPTION_H

    #include <exception>

    #ifndef _MSL_NO_CPP_NAMESPACE
using std::bad_exception;
using std::exception;
using std::set_unexpected;
using std::unexpected;
using std::unexpected_handler;
        #ifndef _MSL_NO_UNCAUGHT_EXCEPTION
using std::uncaught_exception;
        #endif
using std::set_terminate;
using std::terminate;
using std::terminate_handler;
    #endif // _MSL_NO_CPP_NAMESPACE

#endif     // _EXCEPTION_H

// hh 971206  Added namespace support
// hh 990120 changed name of MSIPL flags
// hh 991112 modified using policy
