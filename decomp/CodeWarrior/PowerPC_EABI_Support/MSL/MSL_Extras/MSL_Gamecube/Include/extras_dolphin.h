/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:29:17 $
 * $Revision: 1.2.4.1 $
 */

#ifndef _MSL_EXTRAS_IO_WIN32_H
    #define _MSL_EXTRAS_IO_WIN32_H

    #include <ansi_parms.h>

_MSL_BEGIN_EXTERN_C

_MSL_IMP_EXP_C int _MSL_CDECL _filelength (int fileno) _MSL_CANT_THROW;
    #if _MSL_OS_DISK_FILE_SUPPORT
_MSL_IMP_EXP_C int _MSL_CDECL _chsize (int, long) _MSL_CANT_THROW;
    #endif /* _MSL_OS_DISK_FILE_SUPPORT */

_MSL_END_EXTERN_C

#endif     /* _MSL_EXTRAS_IO_WIN32_H */

           /* Change record:
            * cc  030620 Extras split to reduce namespace pollution
            * ejs 030721 _chsize is needed in extras_io.h
            * cc  030722 Moved underscored functions out of extras common headers/source.
            *            They now live in the extras_xxxx_win32.h headers/source.
            * cc  040217 Changed _No_Disk_File_OS_Support to _MSL_OS_DISK_FILE_SUPPORT
            */
