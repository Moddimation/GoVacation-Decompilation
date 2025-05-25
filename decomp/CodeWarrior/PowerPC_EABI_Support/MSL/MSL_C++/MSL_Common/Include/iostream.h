/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:26:57 $
 * $Revision: 1.22.2.1 $
 */

// iostream.h               // hh 971207 Changed filename from iostream to iostream.h

#ifndef _IOSTREAM_H               // hh 971207 added standard include guards
    #define _IOSTREAM_H

    #include <ios.h>
    #include <iostream>
    #include <istream.h>
    #include <ostream.h>
    #include <streambuf.h>

    #ifndef _MSL_NO_CPP_NAMESPACE // hh 971207 Added backward compatibility
        #ifndef _MSL_NO_IO
using std::cerr;
using std::cin;
using std::clog;
using std::cout;
            #ifndef _MSL_NO_WCHART_CPP_SUPPORT
using std::wcerr;
using std::wcin;
using std::wclog;
using std::wcout;
            #endif                // _MSL_NO_WCHART_CPP_SUPPORT
        #endif                    // _MSL_NO_IO
    #endif // _MSL_NO_CPP_NAMESPACE         // hh 971207 Added backward compatibility

#endif     // _IOSTREAM_H              // hh 971207 added standard include guards

// hh 971207 Changed filename from iostream to iostream.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
