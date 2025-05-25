#ifndef __RELMODULE_SUPPORT__H__
#define __RELMODULE_SUPPORT__H__

#include <dolphin.h>
#include <dolphin/os/OSModule.h>

#ifdef __cplusplus
extern "C"
{
#endif

void MWRelModules ();

// If you get errors here, please comment
// out the OSNotifyLink and OSNotifyUnlink
// in the osmodule.h. The reason for this
// is that an extra parameter is needed
// on the call to get the address of
// the module being loaded.

void OSNotifyLink (OSModuleInfo* inModule);

void OSNotifyUnlink (OSModuleInfo* inModule);

// redefine the OSNotify* calls so they
// pass the address of the module actually
// being loaded.

#define OSLink(x, y)                                                                           \
    OSLink (x, y);                                                                             \
    OSNotifyLink (x);
#define OSUnlink(x)                                                                            \
    OSUnlink (x);                                                                              \
    OSNotifyUnlink (x);

#ifdef __cplusplus
}
#endif

#endif
