/*---------------------------------------------------------------------------*
    Loading files into correctly-sized, correctly-aligned buffers
 *---------------------------------------------------------------------------*/

#include "load_file_from_dvd.h"

#include <revolution.h>
#include <revolution/dvd.h>

void*
LoadFileFromDVD (const char* module_name, u32* length_out)
{
    DVDFileInfo file_info;
    BOOL        BOOL_result;
    u32         length;
    void*       filebuf = 0;
    (*length_out)       = 0;

    if (!module_name || !length_out)
    {
        return NULL;
    }

    BOOL_result = DVDOpen (module_name, &file_info);
    if (!BOOL_result)
    {
        return NULL;
    }

    length = OSRoundUp32B (DVDGetLength (&file_info));
    if (!length)
    {
        return NULL;
    }

    filebuf = OSAlloc (length);
    if (filebuf)
    {
        BOOL_result = DVDRead (&file_info, filebuf, (long)length, 0);
        if (BOOL_result)
        {
            (*length_out) = length;
        }
        else
        {
            OSFree (filebuf);
        }
    }

    DVDClose (&file_info);
    return filebuf;
}
