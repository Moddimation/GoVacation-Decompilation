/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/03/09 20:28:01 $
 * $Revision: 1.23.2.1.2.1 $
 */

/*
 * Turn off CATS information
 */

/*#pragma cats off*/

#ifndef __ansi_prefix__
    #define __ansi_prefix__

    #include <os_enum.h>

    #define __dest_os __dolphin_os

/*********************************************/
/* 		OS Specific Configuration Flags		 */
/*********************************************/

    #ifndef _MSL_OS_DISK_FILE_SUPPORT
        #define _MSL_OS_DISK_FILE_SUPPORT 1
    #endif

    #ifndef _MSL_LOCALDATA_AVAILABLE
        #define _MSL_LOCALDATA_AVAILABLE 0
    #endif

/* Memory Allocation Flags */

/*  See alloc.c for more details on configuration options */

    #ifndef _MSL_OS_ALLOC_SUPPORT
        #define _MSL_OS_ALLOC_SUPPORT 1
    #endif

    #ifndef _MSL_HEAP_EXTERN_PROTOTYPES
        #define _MSL_HEAP_EXTERN_PROTOTYPES                                                    \
            extern char __heap_addr[];                                                         \
            extern char __heap_size[];
    #endif

    #ifndef _MSL_HEAP_START
        #define _MSL_HEAP_START __heap_addr
    #endif

    #ifndef _MSL_HEAP_SIZE
        #define _MSL_HEAP_SIZE __heap_size
    #endif

/* #define _MSL_NO_MALLOC_DESIGN_2 */  /* lower overhead but slower allocation */
/* #define _MSL_NO_FIX_MALLOC_POOLS */ /* For heaps that are less than 64k */

    #define NEWMODE NEWMODE_MALLOC     /* always use malloc for new */

                                       /********************************************/
/* 		Additional Portability Flags 		*/
/********************************************/

    #ifndef _MSL_WIDE_CHAR
        #define _MSL_WIDE_CHAR 1
    #endif

    #ifndef _MSL_USES_SUN_MATH_LIB
        #define _MSL_USES_SUN_MATH_LIB 1
    #endif

    #ifndef _MSL_POSIX
        #define _MSL_POSIX 1
    #endif

    #ifndef _MSL_NEEDS_EXTRAS
        #define _MSL_NEEDS_EXTRAS 1
    #endif

    #ifndef _MSL_BUFSIZ
        #define _MSL_BUFSIZ 1024
    #endif

    #ifndef _MSL_USES_EXIT_PROCESS
        #define _MSL_USES_EXIT_PROCESS 1
    #endif

    #ifndef _MSL_FLT_EVAL_METHOD
        #define _MSL_FLT_EVAL_METHOD 0
    #endif

    #if defined(__cplusplus)
        #define _MSL_USING_NAMESPACE
    #endif

    #define __ANSI_OVERLOAD__

    #define _MSL_INTEGRAL_MATH

    /* __VEC__ is defined to 1 if altivec_model is on */
    #if !__VEC__
        #undef __ALTIVEC__
    #endif

    #define __CW_MAX_PROCESSES__ 32 /* see ExceptionPPC.cp */

/********************************************************
 * Platform Specific Configuration:						*
 * va_list.h - Platform specific typedef of _va_list	*
 ********************************************************/

typedef struct __va_list_struct
{
    char  gpr;
    char  fpr;
    char  reserved[2];
    char* input_arg_area;
    char* reg_save_area;
} __va_list[1];

    #define _MSL_VA_LIST_TYPE __va_list

/************************************************************
 * Platform Specific Configuration:						    *
 * ansi_fp.h - Platform specific define of SIGDIGLEN		*
 ************************************************************/

    #define _MSL_SIGDIGLEN    36

/************************************************************
 * Platform Specific Configuration:	floating support		*
 ************************************************************/

/* Two macros determine what the floating support will be 	*/
/* in the MSL and runtime support.							*/
/*															*/
/*	_No_Floating_Point 	is defined if we don't want	any		*/
/*						floating point.  It is undefined 	*/
/*						for software and hardware support.	*/
/*															*/
/*	_No_Floating_Point_Regs	is defined unless we actually	*/
/*							want hardware floating point	*/
/*							because the chip has an FPU.	*/
/*							Note that the 8xx chips do NOT	*/
/*							have an FPU.					*/

/* We use the project settings to determine what type of	*/
/* of floating point support to enable.	 This is a 2 step	*/
/* process since __option() will only return 0 or 1 and 	*/
/* there are 3 states to test.								*/
/* __option(floatingpoint) will return 0 if you have 		*/
/* selected no floating point support and 1 if you have		*/
/* selected either software or hardware floating point.		*/
/*		0 - no floating point								*/
/*		1 - software or hardware floating point				*/
/* __option(sfp_emulation) will return 1 if you have 		*/
/* selected software floating point support and 0			*/
/* otherwise.		*/
/*		0 - no floating point or hardware floating point	*/
/*		1 - software floating point							*/

    #if __option(floatingpoint) == 0

    /* No floating point support 		*/

        #if _MSL_FLOATING_POINT
            #undef _MSL_FLOATING_POINT
            #define _MSL_FLOATING_POINT 0
        #endif

        #if _MSL_FLOATING_POINT_IO
            #undef _MSL_FLOATING_POINT_IO
            #define _MSL_FLOATING_POINT_IO 0
        #endif

        #define _No_Floating_Point_Regs
        #define _MSL_NO_MATH_LIB

    #elif __option(floatingpoint) == 1

    /* Comment out if faster, non-IEEE 	*/
    /* routines are desired 			*/

        #ifndef __USING_IEEE_MATH__
            #define __USING_IEEE_MATH__ 1
        #endif

    /* Software floating point support 	*/
        #if __option(sfp_emulation) == 1
            #define _No_Floating_Point_Regs
        #else

            #if !__USING_IEEE_MATH__ && __supports(target_instruction, "frsqrte")

                /* This intrinsic is fast but not IEEE accurate					*/
                #ifndef __USING_INTRINSIC_SQRT__
                    #define __USING_INTRINSIC_SQRT__ 1
                #endif

                /* Enable for faster but slightly less accurate sqrtf() */
                #ifndef __LESS_ACCURATE_FP__
                    #define __LESS_ACCURATE_FP__ 0
                #endif

            #endif /* __USING_IEEE_MATH__ */

            /* This intrinsic is fast and 	 	*/
            /* accurate							*/
            #ifndef __USING_INTRINSIC_FABS__
                #define __USING_INTRINSIC_FABS__ 1
            #endif

        #endif     /* sfp_emulation */
    #endif         /* floatingpoint */

#endif

/* Change Record:
 * MEA 970720 changed __ppc_eabi_bare to __ppc_eabi.  Added
 * 			  specific defines for OS services.
 * MEA 970804 Added automatic support for floating point.
 * MEA 040598 undefined windows only __tls
 * MEA 990824 Removed outdated defines and added defines and comments
 *			  for new allocation methods.
 * MEA 991025 Added _No_Altivec when Altivec isn't enabled
 * MEA 010104 namespaces are enabled by defining _MSL_USING_NAMESPACE
 * MEA 010111 allow control of IEEE or faster math routines
 * cc  011022 Added _MSL_USES_EXIT_PROCESS and removed #include to ansi_parms.h
 * cc  011022 Added _MSL_NEEDS_EXTRAS
 * cc  011211 Added _MSL_SIGDIGLEN
 * hh  020214 Renamed _MSL_PRO4_MALLOC to _MSL_CLASSIC_MALLOC
 * cc  020730 Added _MSL_POSIX
 * cc  040217 Changed _No_Disk_File_OS_Support to _MSL_OS_DISK_FILE_SUPPORT
 * JWW 040401 Changed _No_Alloc_OS_Support to _MSL_OS_ALLOC_SUPPORT
 * JWW 040405 Changed _Unbuffered_Console to _MSL_BUFFERED_CONSOLE
 */
