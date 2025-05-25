/*---------------------------------------------------------------------------*
    Loading REL modules from host into correctly-sized, correctly-aligned buffers
 *---------------------------------------------------------------------------*/

#include <revolution.h>
#include <stdio.h>

#include "load_file_from_host.h"

static void*
RoundAddressUpToMultipleOf (void* p, u32 n)
{
    return (void*)((((u32)p + n - 1) / n) * n);
}

BOOL
LoadRelStringTableFromHost (char* string_table_name)
{
    void* file = 0;
    u32   length;

    file = LoadFileFromHost (string_table_name, &length);
    if (!file)
    {
        return FALSE;
    }

    OSSetStringTable (file);

    return TRUE;
}

// Returns pointer to actual REL module, properly aligned.  After unlinking
// module, you must call OSFree on the pointer stored in *buffer_to_free.
// On failure, returns zero and doesn't overwrite *buffer_to_free.
OSModuleHeader*
LoadRelFromHost (char* module_name, void** buffer_to_free)
{
    BOOL            BOOL_result;
    s32             s32_result;
    FILE*           file = 0;
    u32             file_size;
    OSModuleHeader  module_header;
    void*           module_buffer = 0;
    u32             module_buffer_size;
    u32             module_fixed_size;
    OSModuleHeader* module = 0;
    u8*             bss    = 0;
    u32             align;
    u32             bss_align;
    u32             fix_size;

    if (!module_name || !buffer_to_free)
    {
        return NULL;
    }

    file = fopen (module_name, "rb");
    if (!file)
    {
        return NULL;
    }

    s32_result = fseek (file, 0, SEEK_END);
    if (s32_result)
    {
        fclose (file);
        return NULL;
    }

    file_size = ftell (file);
    if ((s32)file_size == -1 || file_size == 0)
    {
        fclose (file);
        return NULL;
    }

    if (file_size < sizeof (OSModuleHeader))
    {
        fclose (file);
        return NULL;
    }

    rewind (file);
    s32_result = fread (&module_header, 1, sizeof (OSModuleHeader), file);
    if (s32_result != sizeof (OSModuleHeader))
    {
        fclose (file);
        return NULL;
    }

    if (module_header.info.version >= 2)
    {
        align     = module_header.align;
        bss_align = module_header.bssAlign;
    }
    else
    {
        align     = 32;
        bss_align = 32;
    }

    if (module_header.info.version >= 3)
    {
        fix_size = module_header.fixSize;
    }
    else
    {
        fix_size = file_size;
    }

    module_fixed_size = fix_size + bss_align + module_header.bssSize;

    module_buffer_size =
        align + ((module_fixed_size > file_size) ? module_fixed_size : file_size);

    module_buffer = OSAlloc (OSRoundUp32B (module_buffer_size));
    if (!module_buffer)
    {
        fclose (file);
        return NULL;
    }

    module = RoundAddressUpToMultipleOf (module_buffer, align);

    rewind (file);
    s32_result = fread (module, 1, file_size, file);
    if (s32_result != file_size)
    {
        OSFree (module_buffer);
        fclose (file);
        return NULL;
    }

    fclose (file);

    // Force data cache to write to memory for this region.
    // DVD API does this for us, but Host I/O does not.
    DCStoreRange (module_buffer, OSRoundUp32B (module_buffer_size));
    ICInvalidateRange (module_buffer, OSRoundUp32B (module_buffer_size));

    bss = (u8*)module + fix_size;

    // Make sure bss is properly aligned
    bss = RoundAddressUpToMultipleOf (bss, bss_align);

    // Link the module
    if (module->info.version >= 3)
    {
        BOOL_result = OSLinkFixed (&module->info, bss);
    }
    else
    {
        BOOL_result = OSLink (&module->info, bss);
    }

    if (!BOOL_result) // If the link failed
    {
        OSFree (module_buffer);
        return NULL;
    }

    (*buffer_to_free) = module_buffer;
    return module;
}

void
UnloadRel (OSModuleHeader* module, void* module_buffer)
{
    OSUnlink (&module->info);
    OSFree (module_buffer);
}
