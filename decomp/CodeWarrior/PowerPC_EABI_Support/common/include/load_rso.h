#include <revolution/rso.h>

/*---------------------------------------------------------------------------*
    Loading RSO modules
 *---------------------------------------------------------------------------*/

// Pointer to function which takes a filename as input, and a pointer
typedef void* (*LoadFile_func_t) (const char*, u32*);

// Load and link the static module info
RSOObjectHeader* LoadStaticModuleInfo (char* tModuleName, LoadFile_func_t LoadFile_func);

// Load and link the specified module
RSOObjectHeader* LoadRSO (const char* tModuleName, LoadFile_func_t LoadFile_func);

// Unlink and unload the specified module
void UnloadRSO (RSOObjectHeader* module);

// Load and unload functions that call prolog/epilog automatically
RSOObjectHeader* LoadRSOandCallProlog (const char* tModuleName, LoadFile_func_t LoadFile_func);

void UnloadRSOandCallEpilog (RSOObjectHeader* module);

/*---------------------------------------------------------------------------*
     RSO symbol resolution & error handling
 *---------------------------------------------------------------------------*/

void* ResolveRsoSymbol (RSOObjectHeader* module, const char* tSymbolName);
void* UnresolveRsoSymbol ();
