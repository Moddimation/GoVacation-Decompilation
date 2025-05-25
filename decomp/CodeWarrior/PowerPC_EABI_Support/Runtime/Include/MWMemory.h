/*
 *  MWMemory.h	-	CodeWarrior runtime suport definitions
 *
 *  Copyright ｩ 1995-2006 Freescale Semiconductor, Inc.  All rights reserved.
 *
 * $Date: 2006/02/18 00:03:55 $
 * $Revision: 1.3 $
 */

#ifndef __MWMEMORY_H__
#define __MWMEMORY_H__

typedef char* Ptr;
typedef Ptr*  Handle;
typedef int   OSErr;

extern pascal Handle NewHandle (long dataSize);
extern pascal void   DisposeHandle (Handle h);

#endif /* __MWMEMORY_H__ */
