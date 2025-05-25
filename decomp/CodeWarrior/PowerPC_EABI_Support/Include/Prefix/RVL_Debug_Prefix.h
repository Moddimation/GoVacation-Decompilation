// make sure _DEBUG is defined if NDEBUG is not defined
#ifndef NDEBUG
    #define _DEBUG
#endif

// defines for RVL target
#define RVL
#define RVL_SDK
#define TRK_INTEGRATION
#define RVL_OS
#define EPPC
#define GEKKO
#define HOLLYWOOD_REV 1
#define BROADWAY_REV  1
#define IOP_REV       1

/*
 * Enabling C++ extensions if compiling as C++
 * sw on 09102002
 */

#ifdef __cplusplus
    #pragma cpp_extensions on
#endif

// Disable CATS information for Wii .elfs
#pragma cats off

#include <ansi_prefix.PPCEABI.Gamecube.h>
