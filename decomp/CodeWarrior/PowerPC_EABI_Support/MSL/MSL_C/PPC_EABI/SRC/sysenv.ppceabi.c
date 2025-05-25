/*  MSL  */

/*  $Date: 2007/01/29 21:23:23 $
 *  $Revision: 1.4.2.1 $
 *  $NoKeywords: $
 *
 *		Copyright ｩ 1995-2007 Freescale Corporation.
 *		All rights reserved.
 */

/*
 *	sysenv.ppceabi.c
 *
 *	Routines
 *	--------
 *		getenv
 *		system
 *
 */

#include <cstdlib> /* for getenv */

char*
getenv (const char* inVarName)
{
#pragma unused(inVarName)
    return 0;
}

int
system (const char* cmdLine)
{
#pragma unused(cmdLine)
    return 0;
}

/*  Change History
 *  swell - 09262001 - initial version to fix missing symbols for GCN
 */
