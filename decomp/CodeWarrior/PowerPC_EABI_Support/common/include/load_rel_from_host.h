/*---------------------------------------------------------------------------*
    Loading REL modules from host into correctly-sized, correctly-aligned buffers
 *---------------------------------------------------------------------------*/

#include <revolution/os.h>

BOOL LoadRelStringTableFromHost (char* string_table_name);

// Returns pointer to actual REL module, properly aligned.  After unlinking
// module, you must call OSFree on the pointer stored in *buffer_to_free.
OSModuleHeader* LoadRelFromHost (char* module_name, void** buffer_to_free);

void UnloadRel (OSModuleHeader* module, void* module_buffer);
