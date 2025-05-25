#ifndef __FSL_LOAD_REL_FROM_DVD_H__
#define __FSL_LOAD_REL_FROM_DVD_H__
/*---------------------------------------------------------------------------*
    Loading REL modules from DVD into correctly-sized, correctly-aligned buffers
 *---------------------------------------------------------------------------*/

#include <revolution/os.h>

#ifdef __cplusplus
extern "C"
{
#endif

BOOL LoadRelStringTableFromDvd (char* string_table_name);

// Returns pointer to actual REL module, properly aligned.  After unlinking
// module, you must call OSFree on the pointer stored in *buffer_to_free.
OSModuleHeader* LoadRelFromDvd (char* module_name, void** buffer_to_free);

void UnloadRel (OSModuleHeader* module, void* module_buffer);

#ifdef __cplusplus
}
#endif

#endif
