/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 *  $Date: 2007/01/29 21:27:09 $
 *  $Revision: 1.21.2.1 $
 */

// valarray.h             // hh 971207 Changed filename from valarray to valarray.h

#ifndef _VALARRAY_H               // hh 971207 added standard include guards
    #define _VALARRAY_H

    #include <valarray>

    #ifndef _MSL_NO_CPP_NAMESPACE // hh 971207 Added backward compatibility
using std::gslice;
using std::slice;
using std::valarray;
    #endif // _MSL_NO_CPP_NAMESPACE            // hh 971207 Added backward compatibility

#endif     // _VALARRAY_H               // hh 971207 added standard include guards

// hh 971207 Changed filename from valarray to valarray.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
