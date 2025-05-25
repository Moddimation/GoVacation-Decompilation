/*
 * MW_Rel_Module_Support.h
 * Copyright Metrowerks 1995-2002
 */

#ifndef __RELMODULE_SUPPORT__H__
#define __RELMODULE_SUPPORT__H__

#include <dolphin.h>
#include <dolphin/os/OSModule.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * This symbol tells the debugger that the OSNotify*
 * calls are being overridden, so the address will
 * be passed to OSNotify.
 */

void MWRelModules ();

/*
  These OSNotify routines will override the versions
  in the Dolphin OS.

  If you get errors here (below), please comment out the
  OSNotifyLink and OSNotifyUnlink in osmodule.h. The reason
  for this is that an extra parameter is needed on the call
  to get the address of  the module being loaded.
*/

void OSNotifyLink (OSModuleInfo* inModule);

void OSNotifyUnlink (OSModuleInfo* inModule);

/*
 redefine the OSNotify* calls so they
 pass the address of the module actually
 being loaded.
 */

BOOL MWNotifyLink (OSModuleInfo* newModule, void* bss);
BOOL MWNotifyUnlink (OSModuleInfo* newModule);

#define OSLink   MWNotifyLink
#define OSUnlink MWNotifyUnlink

#ifdef __cplusplus
}
#endif

#endif
