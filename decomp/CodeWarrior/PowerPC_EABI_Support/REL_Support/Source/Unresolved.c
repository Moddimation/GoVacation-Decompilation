#include <revolution.h>

#include "FSL_REL_Support.h"

__declspec (weak) MODULE_EXPORT void
_unresolved (void)
{
    u32  i;
    u32* p;

    OSReport ("\nError: Unlinked function called in module %s.\n", __FILE__);
    OSReport ("Address:      Back Chain    LR Save\n");
    for (i = 0, p = (u32*)OSGetStackPointer(); // get current sp
         p && (u32)p != 0xffffffff && i++ < 16;
         p = (u32*)*p)                         // get caller sp
    {
        OSReport ("0x%08x:   0x%08x    0x%08x\n", p, p[0], p[1]);
    }
    OSReport ("\n");
}
