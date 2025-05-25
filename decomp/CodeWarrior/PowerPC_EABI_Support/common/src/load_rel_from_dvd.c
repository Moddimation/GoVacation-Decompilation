/*---------------------------------------------------------------------------*
    Loading REL modules from DVD into correctly-sized, correctly-aligned buffers
 *---------------------------------------------------------------------------*/

#include "load_rel_from_dvd.h"

#include <revolution.h>

#include "load_file_from_dvd.h"

static void*
RoundAddressUpToMultipleOf (void* p, u32 n)
{
    return (void*)((((u32)p + n - 1) / n) * n);
}

BOOL
LoadRelStringTableFromDvd (char* string_table_name)
{
    void* file = 0;
    u32   length;

    file = LoadFileFromDVD (string_table_name, &length);
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
LoadRelFromDvd (char* module_name, void** buffer_to_free)
{
    BOOL            BOOL_result;
    s32             s32_result;
    DVDFileInfo     file_info;
    u32             file_size;
    OSModuleHeader* module_header = 0;
    u32             module_header_size;
    u32             module_header_size_rounded;
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

    BOOL_result = DVDOpen (module_name, &file_info);
    if (!BOOL_result)
    {
        return NULL;
    }

    file_size = OSRoundUp32B (DVDGetLength (&file_info));
    if (file_size < sizeof (OSModuleHeader))
    {
        DVDClose (&file_info);
        return NULL;
    }

    module_header_size = sizeof (OSModuleHeader);
    module_header      = (OSModuleHeader*)OSRoundUp32B (__alloca (module_header_size + 32));
    if (!module_header)
    {
        DVDClose (&file_info);
        return NULL;
    }

    module_header_size_rounded = OSRoundUp32B (module_header_size);
    s32_result = DVDRead (&file_info, module_header, (long)module_header_size_rounded, 0);
    if (s32_result != module_header_size_rounded)
    {
        DVDClose (&file_info);
        return NULL;
    }

    if (module_header->info.version >= 2)
    {
        align     = module_header->align < 32 ? 32 : module_header->align;
        bss_align = module_header->bssAlign;
    }
    else
    {
        align     = 32;
        bss_align = 32;
    }

    if (module_header->info.version >= 3)
    {
        fix_size = module_header->fixSize;
    }
    else
    {
        fix_size = file_size;
    }

    module_fixed_size = fix_size + bss_align + module_header->bssSize;

    module_buffer_size =
        align + ((module_fixed_size > file_size) ? module_fixed_size : file_size);

    module_buffer = OSAlloc (OSRoundUp32B (module_buffer_size));
    if (!module_buffer)
    {
        DVDClose (&file_info);
        return NULL;
    }

    module     = RoundAddressUpToMultipleOf (module_buffer, align);

    s32_result = DVDRead (&file_info, module, (long)file_size, 0);
    if (s32_result != file_size)
    {
        OSFree (module_buffer);
        DVDClose (&file_info);
        return NULL;
    }

    DVDClose (&file_info);

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
