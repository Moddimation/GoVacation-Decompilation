#include "load_rso.h"

#include <revolution.h>
#include <revolution/rso.h>
#include <revolution/RSOLink.h>

/*---------------------------------------------------------------------------*
    Loading RSO modules
 *---------------------------------------------------------------------------*/

// Typedef function pointer types for clarity
typedef void (*pfn_void_to_void_t) (void);

// Load and link the static module info
RSOObjectHeader*
LoadStaticModuleInfo (char* tModuleName, LoadFile_func_t LoadFile_func)
{
    BOOL             BOOL_result;
    RSOObjectHeader* module = 0;
    u32              length = 0;

    module                  = (RSOObjectHeader*)LoadFile_func (tModuleName, &length);
    if (!module)
    {
        return NULL;
    }

    OSReport ("    Module info '%s', loaded at %p, size is %d bytes.\n",
              tModuleName,
              module,
              length,
              module->bssSize);

    BOOL_result = RSOListInit (module);
    if (BOOL_result == FALSE)
    {
        OSFree (module);
        return NULL;
    }

    return module;
}

// Load and link the specified module
RSOObjectHeader*
LoadRSO (const char* tModuleName, LoadFile_func_t LoadFile_func)
{
    BOOL             BOOL_result;
    RSOObjectHeader* module = 0;
    u32              length = 0;
    u8*              bss    = 0;

    module                  = (RSOObjectHeader*)LoadFile_func (tModuleName, &length);
    if (!module)
    {
        return NULL;
    }

    OSReport ("    Module '%s', loaded at %p, size is %d bytes, bss section is %d bytes.\n",
              tModuleName,
              module,
              length,
              module->bssSize);

    if (module->bssSize > 0)
    {
        // alloc bss area
        bss = (u8*)OSAlloc (module->bssSize);
        if (!bss)
        {
            OSFree (module);
            return NULL;
        }
    }

    BOOL_result = RSOLinkList (module, bss);
    if (BOOL_result == FALSE)
    {
        if (bss)
            OSFree (bss);
        OSFree (module);
        return NULL;
    }

    return module;
}

// Unlink and unload the specified module
void
UnloadRSO (RSOObjectHeader* module)
{
    void* bss = 0;

    if (!module)
        return;

    if (module->bssSize)
    {
        RSOSectionInfo* si;

        // Retrieve pointer to the bss section and free it.
        si   = RSOGetSectionInfo (module);
        si  += module->bssSection;
        bss  = (void*)si->offset;
    }

    RSOUnLinkList (module);

    if (bss)
        OSFree (bss);
    OSFree (module);
}

// Load and unload functions that call prolog/epilog automatically
RSOObjectHeader*
LoadRSOandCallProlog (const char* tModuleName, LoadFile_func_t LoadFile_func)
{
    RSOObjectHeader*   module = 0;
    pfn_void_to_void_t prolog;

    module = LoadRSO (tModuleName, LoadFile_func);

    if (module)
    {
        prolog = (pfn_void_to_void_t)module->prolog;
        prolog();
    }

    return module;
}

void
UnloadRSOandCallEpilog (RSOObjectHeader* module)
{
    pfn_void_to_void_t epilog;

    if (module)
    {
        epilog = (pfn_void_to_void_t)module->epilog;
        epilog();
    }

    UnloadRSO (module);
}

/*---------------------------------------------------------------------------*
     RSO symbol resolution & error handling
 *---------------------------------------------------------------------------*/

// The main app keeps pointers to symbols in RSO modules.  When these symbols
// are not resolved, set them to this function's address.  This helps for two
// reasons: (1) You can test to see if a symbol is unresolved by comparing it
// to this function's address, and (2) your app will not crash if you attempt
// to call an unresolved function -- this function will be called instead.
static void
error_function ()
{
    OSReport ("    M: Unresolved function called from main app!\n");
}

void*
ResolveRsoSymbol (RSOObjectHeader* module, const char* tSymbolName)
{
    void* symbol_address;

    symbol_address = (void*)RSOFindExportSymbolAddr (module, tSymbolName);

    return symbol_address ? symbol_address : (void*)error_function;
}

void*
UnresolveRsoSymbol ()
{
    return (void*)error_function;
}
