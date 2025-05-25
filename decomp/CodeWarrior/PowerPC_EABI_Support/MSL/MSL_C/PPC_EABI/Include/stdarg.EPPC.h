/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:28:44 $
 * $Revision: 1.7.2.1 $
 */

#ifndef _MSL_STDARG_EPPC_H
    #define _MSL_STDARG_EPPC_H

_MSL_BEGIN_EXTERN_C

extern void* __va_arg (void*, int);

    #define __va_start(ap, fmt) ((void)fmt, __builtin_va_info (&ap))
    #define __va_arg(ap, t)     (*((t*)__va_arg (ap, _var_arg_typeof (t))))

    #define va_start            __va_start
    #define va_arg              __va_arg
    #define va_end              __va_end

    #define __va_end(ap)        ((void)0)

    /* macro for copying one va_list to another */
    #define __va_copy(a, b)     ((a)[0] = (b)[0])
    #if _MSL_C99
        #define va_copy __va_copy
    #endif

_MSL_END_EXTERN_C

#endif /* _MSL_STDARG_EPPC_H */

       /* Change record:
        * JWW 011101 New file to make stdarg information platform independent
        * cc  020709 Removed the elif __PPC_EABI__
        */
