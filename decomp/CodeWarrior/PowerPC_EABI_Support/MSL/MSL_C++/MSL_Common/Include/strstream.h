/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:27:07 $
 * $Revision: 1.22.2.1 $
 */

// strstream.h

#ifndef _STRSTREAM_H
    #define _STRSTREAM_H

    #include <strstream>

    #ifndef _MSL_NO_CPP_NAMESPACE
        #ifndef _MSL_NO_IO
using std::istrstream;
using std::ostrstream;
using std::strstream;
using std::strstreambuf;
        #endif // _MSL_NO_IO
    #endif     // _MSL_NO_CPP_NAMESPACE

#endif         // _STRSTREAM_H

// hh 990120 changed name of MSIPL flags
