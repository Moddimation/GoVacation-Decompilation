/***************************************************************************/
/*

FILE
    __ppc_eabi_init.h

    $Date: 2004/05/11 03:39:08 $
    $Revision: 1.6 $

DESCRIPTION

    Interface for board-level initialization and user-level initialization.

    __start
        (registers initialized)
        __init_hardware called
        (data initialized, .data/.bss/.sdata/.sbss...)
        __init_cpp
        (exceptions initialized and static constructors called)
        __init_user
        main

    Define __init_hardware and __init_user or use the default stub functions.

    Note: __init_hardware should be written so as to not reference memory
    i.e., it should not be written in C or allocate a stackframe.

COPYRIGHT
    (c) 2002 Metrowerks Corporation
    All rights reserved.

HISTORY
    97 APR 17 LLY	Created.
    97 JUN 21 MEA	added __init_cpp() and __init().
    97 JUL 20 MEA	added _ExitProcess.
    97 JUL 20 MEA	added ADSInit.

*/
/***************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <macros.h>

__declspec (section ".init") extern void __init_hardware (void);
extern void                              __init_user (void);
__declspec (weak) extern void            _ExitProcess (void);
__declspec (section ".init") extern void __flush_cache (void* address, unsigned int size);
__declspec (section ".init") extern void ADSInit ();
__declspec (section ".init") extern void __init_registers (
    void); /* overloadable, set up PPC regs (r1, r2, r13) */
__declspec (section ".init") extern void __init_data (
    void); /* overloadable ROM-to-RAM copy routine */
__declspec (section ".init") extern void __copy_rom_section (void*         dst,
                                                             const void*   src,
                                                             unsigned long size);
__declspec (section ".init") extern void __init_bss_section (void* dst, unsigned long size);
__declspec (section ".init") __declspec (weak) extern void __exception_info_constants (
    void** info,
    char** R2);
__declspec (section ".init") __declspec (weak) extern int __find_exception_addresses (
    void*  info,
    char*  returnaddr,
    void** ex_start,
    void** ex_end);

#ifdef __cplusplus
}
#endif
