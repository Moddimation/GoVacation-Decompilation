/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:27:01 $
 * $Revision: 1.21.2.1 $
 */

// numeric.h          // hh 971207 Changed filename from numeric to numeric.h

#ifndef _NUMERIC_H                // hh 971207 added standard include guards
    #define _NUMERIC_H

    #include <numeric>

    #ifndef _MSL_NO_CPP_NAMESPACE // hh 971207 Added backward compatibility
using std::accumulate;
using std::adjacent_difference;
using std::inner_product;
using std::partial_sum;
    #endif // _MSL_NO_CPP_NAMESPACE           // hh 971207 Added backward compatibility

#endif     // _NUMERIC_H             // hh 971207 added standard include guards

// hh 971207 Changed filename from numeric to numeric.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
