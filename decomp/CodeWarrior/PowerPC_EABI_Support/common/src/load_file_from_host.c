/*---------------------------------------------------------------------------*
    Loading files into correctly-sized, correctly-aligned buffers
 *---------------------------------------------------------------------------*/

#include <revolution.h>
#include <stdio.h>

void*
LoadFileFromHost (const char* tModuleName, u32* length_out)
{
    s32   s32_result;
    FILE* file = 0;
    u32   length;
    void* filebuf = 0;

    if (!tModuleName || !length_out)
    {
        return NULL;
    }

    file = fopen (tModuleName, "rb");
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

    length = ftell (file);
    if ((s32)length == -1 || length == 0)
    {
        fclose (file);
        return NULL;
    }

    filebuf = OSAlloc (OSRoundUp32B (length));
    if (!filebuf)
    {
        fclose (file);
        return NULL;
    }

    rewind (file);
    s32_result = fread (filebuf, 1, length, file);
    if (s32_result != length)
    {
        OSFree (filebuf);
        fclose (file);
        return NULL;
    }

    fclose (file);

    // Force data cache to write to memory for this region.
    // DVD API does this for us, but Host I/O does not.
    DCStoreRange (filebuf, length);
    ICInvalidateRange (filebuf, length);

    (*length_out) = length;
    return filebuf;
}
