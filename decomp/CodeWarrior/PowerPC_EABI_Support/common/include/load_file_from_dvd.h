/*---------------------------------------------------------------------------*
    Loading files into correctly-sized, correctly-aligned buffers
 *---------------------------------------------------------------------------*/

// void* LoadFileFromDVD(const char* tModuleName, u32* length_out)
// - Allocates a correctly-sized buffer (using OSAlloc) for
//   a file, then loads it.
// - Stores length in (*length_out) if the function succeeds.
// - Returns NULL on failure.
// - It is the caller's responsibility to free the returned
//   memory (using OSFree).
// - Don't forget to call DVDInit() before calling this!
void* LoadFileFromDVD (const char* module_name, u32* length_out);
