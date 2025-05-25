/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:27:02 $
 * $Revision: 1.21.2.1 $
 */

// queue.h             // hh 971207 Changed filename from queue to queue.h

#ifndef _QUEUE_H                  // hh 971207 added standard include guards
    #define _QUEUE_H

    #include <queue>

    #ifndef _MSL_NO_CPP_NAMESPACE // hh 971207 Added backward compatibility
using std::priority_queue;
using std::queue;
    #endif // _MSL_NO_CPP_NAMESPACE          // hh 971207 Added backward compatibility

#endif     // _QUEUE_H               // hh 971207 added standard include guards

// hh 971207 Changed filename from queue to queue.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
