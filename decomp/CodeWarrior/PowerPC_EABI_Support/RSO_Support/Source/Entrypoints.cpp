#include <revolution.h>

#include "FSL_RSO_Support.h"

MODULE_EXPORT void
_prolog (void)
{
    ModuleConstructors();
}

MODULE_EXPORT void
_epilog (void)
{
    ModuleDestructors();
}
