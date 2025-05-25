/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:26:51 $
 * $Revision: 1.22.2.1 $
 */

// fstream.h           // hh 971207 Changed filename from fstream to fstream.h

#ifndef _FSTREAM_H                // hh 971207 added standard include guards
    #define _FSTREAM_H

    #include <fstream>

    #ifndef _MSL_NO_CPP_NAMESPACE // hh 971207 Added backward compatibility
        #ifndef _MSL_NO_IO
using std::basic_filebuf;
using std::basic_fstream;
using std::basic_ifstream;
using std::basic_ofstream;
using std::filebuf;
using std::fstream;
using std::ifstream;
using std::ofstream;
            #ifndef _MSL_NO_WCHART_CPP_SUPPORT
using std::wfilebuf;
using std::wfstream;
using std::wifstream;
using std::wofstream;
            #endif                // _MSL_NO_WCHART_CPP_SUPPORT
        #endif                    // _MSL_NO_IO
    #endif // _MSL_NO_CPP_NAMESPACE        // hh 971207 Added backward compatibility

#endif     // _FSTREAM_H      // hh 971207 added standard include guards

// hh 971207 Changed filename from fstream to fstream.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991112 modified using policy
