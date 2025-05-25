/*
 *  MWMemory.h	-	MW runtime suport definitions
 *
 *  Copyright ｩ 1995-2002 Metrowerks, Corporation.  All rights reserved.
 *
 * $Date: 2008/03/10 02:29:54 $
 * $Revision: 1.1.4.1 $
 */

#ifndef __MWMEMORY_H__
#define __MWMEMORY_H__

typedef char* Ptr;
typedef Ptr*  Handle;
typedef int   OSErr;

extern pascal Handle NewHandle (long dataSize);
extern pascal void   DisposeHandle (Handle h);

#endif /* __MWMEMORY_H__ */
