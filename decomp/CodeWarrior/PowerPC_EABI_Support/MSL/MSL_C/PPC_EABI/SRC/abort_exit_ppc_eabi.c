/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:23:18 $
 * $Revision: 1.7.2.1 $
 */

/*
 *	Routines
 *	--------
 *		abort
 *
 *		atexit
 *		exit
 *
 *		__atexit
 *		__exit
 *
 *		__setup_exit
 */

#include <abort_exit.h>
#include <ansi_parms.h>
#include <critical_regions.h>
#include <misc_io.h>
#include <pool_alloc.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define max_funcs 64

int __aborting = 0;

void       (*__atexit_funcs[max_funcs]) (void);
long       __atexit_curr_func = 0;
extern int __atexit (void (*func) (void));

void _MSL_CDECL (*__stdio_exit) (void)   = 0;
void _MSL_CDECL (*__console_exit) (void) = 0;

void _MSL_CDECL
abort (void)
{
    raise (SIGABRT);

    __aborting = 1;

    exit (EXIT_FAILURE);
}

int _MSL_CDECL
atexit (void (_MSL_CDECL* func) (void))
{
    int result;

    __begin_critical_region (atexit_funcs_access);

    result = __register_atexit (func);

    __end_critical_region (atexit_funcs_access);

    return result;
}

int
__atexit (void (_MSL_CDECL* func) (void))
{
    if (__atexit_curr_func == max_funcs)
        return (-1);

    __begin_critical_region (atexit_funcs_access);

    __atexit_funcs[__atexit_curr_func++] = func;

    __end_critical_region (atexit_funcs_access);

    return 0;
}

void _MSL_CDECL
__exit (int status)
{
#pragma unused(status)

    __begin_critical_region (atexit_funcs_access);

    while (__atexit_curr_func > 0) (*__atexit_funcs[--__atexit_curr_func])();

    __end_critical_region (atexit_funcs_access);

    __kill_critical_regions();

    if (__console_exit)
    {
        (*__console_exit)();
        __console_exit = 0;
    }

    _ExitProcess();
}

/* Change record:
 * JFH 950914 First code release.
 * JFH 951012 Added #include of <SegLoad.h> for ExitToShell() (in case
 *			  MacHeaders not included).
 * JFH 951031 Fixed exit() to longjmp(__program_exit,1) instead of call _exit on PPC
 * JFH 951215 Reworked abort/exit handling to conform to new runtime architecture.
 * JFH 951220 Renamed _atexit/_exit to __atexit/__exit for ANSI naming conformance
 * JFH 951227 Pulled guts out of __setup_exit for new runtime. Tossed __program_exit
 *								 and added __aborting for PPC and CFM68K projects.
 * JFH 960212 Tossed __setup_exit, which had become a NOP.
 * JFH 960301 Merged Be code into source. For the moment that means: __setup_exit(),
 *								 it's baaack!
 * JFH 960426 Merged Win32 changes in.
 *						CTV
 * mm  960927 Made sure dest_os set for Macintosh
 * KO  961218 Moved the begin_critical_region call after the error check. Before, if
 *            there was an error, the critical section would be entered and never left.
 * KO  961219 Added CleanUpMSL.
 * bkoz970218 line 154 moved call of __destroy_global_chain() up to
 *			  exit() before stdio closed
 * MEA 970720 Changed __ppc_eabi_bare to __ppc_eabi.
 * mdf 970821 added printf/scanf to exit for win32 to allow control of
 *            console apps
 * mdf 970930 wrapped immediately above change in GetFileType to prevent app from
 *			  stopping when output is redirected to a file.
 *            Also add fflush so getc will have an empty stream
 *            when called in the exit routine
 * mdf 971021 fixed exiting for windows console apps in cases where
 *            the app explicitly closes stdin before exit is reached.
 *            this can be done with fclose or freopen and then close on
 *            the file associated with the stdin stream.
 * mdf 971119 removed fflush on stdin
 * hh  980122 #include "critical_regions.h" doesn't belong here
 * mf  980512 changes for wince
 * mf  980810 ( & blc ) fix to x86 runtime dll crash(when > 1 dll is attached to MSL)
 * mf  980825 Deallocate memory pools allocated by system - this fixes a
 *			  problem with memory leaking when multiple dll's are loaded and
 *			  unloaded.
 * mm  981015 Added version of exit for __DSP568
 * mea 981022 added calls to destructors
 * mm  981029 Changed __DSP568 to __m56800__
 * mm  981227 Avoided closing console windows during abort. WB1-1897
 * mm  990122 Avoided closing console windows during exit.
 * hh  990227 Interface to __pool_free_all changed
 * beb 990727 added __emb_68k to the guard around call to _ExitProcess.
 * mf  000514 call to __kill_critical_regions() in __exit was deleting data used
 *            by the dll upon unloading. This was crashing on windows.  The call to
 *            _CleanUpMSL() will simply tell the dll that the process is exiting and
 *            allow the dll to cleanup up it's own resources when it is sure all processes
 *            are detached.
 * mf  000731 reversed the order of calling __destroy_global_chain, and (*__stdio_exit)().
 *            __destroy_global_chain must occur first since one of the destructors it calls
 *            could rely on one of the standard handles (which __stdio_exit destroys).
 * mf  000808 moved _CleanUpMSL() into extras.win32.c.  This must always be in the dll.
 *            it can't be in the same source as abort_exit.c because of complications with
 *            the shared library.  See additional notes in extras.win32.c.  Also, atexit
 *            will be moved into atexit.c  for similar reasons. It is just preprocessed out
 *            for windows until next release.
 * as  000823 added mcore guard to __exit() so we call _ExitProcess()
 * hh  010307 Merged atexit chain with global destructor chain
 * cc  010821 Added eppc to core
 * cc  010821 Added __dest_os  __dolphin_os
 * sw  011005 Reordered the PPC_EABI in front of !win32 for __atexit_funcs, since it was defined
 *twice JWW 011027 Added test for Mach-O to use the old Mac code path cc  011203 Added
 *_MSL_CDECL for new name mangling cc  020225 Removed register_atexit prototype moved to header
 * JWW 020330 Call __msl_console_shutdown() at Mach-O atexit time to kill the Console before
 *Carbon JWW 020423 Use new INIT_BEFORE_TERM_AFTER to ensure the console is always killed before
 *Carbon cc  040226 Duplicate exit has been removed
 */
