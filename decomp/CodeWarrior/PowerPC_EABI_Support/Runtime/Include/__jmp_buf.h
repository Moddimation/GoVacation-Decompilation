/* Metrowerks PowerPC EABI Runtime Support Library
 * Copyright ｩ 1995-2002 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2003/01/25 00:19:41 $
 * $Revision: 1.3 $
 */

#ifndef __jmp_buf_h
    #define __jmp_buf_h

/* 	__jmp_buf.h is a private runtime file which declares the structure __jmp_buf */

    #if __PPCe500__ || __PPCZen__
typedef union MWE_ElfVector64
{
    __ev64_opaque__ v;
    unsigned long   l[2];
    unsigned short  s[4];
    unsigned char   c[8];
} MWE_ElfVector64;
    #endif

/*
 *	Private Types
 */

typedef struct __jmp_buf
{
    unsigned long pc;                /*	0: saved PC			*/
    unsigned long cr;                /*	4: saved CR			*/
    unsigned long sp;                /*  8: saved SP			*/
    unsigned long rtoc;              /* 12: saved RTOC		*/

    #if __VEC__                      /* true if altivec model is true */
    unsigned long vrsave;            /* 16: old vrsave		*/
    #else
    unsigned long reserved; /* 16: not used			*/
    #endif

    #if __PPCe500__ || __PPCZen__
    MWE_ElfVector64 gprs[19];        /* 20: saved R13-R31	*/
    #else
    unsigned long gprs[19]; /* 20: saved R13-R31	*/
    #endif

    #ifndef _No_Floating_Point_Regs
    double fp14;                     /* 96: saved FP14-FP31	*/
    double fp15;
    double fp16;
    double fp17;
    double fp18;
    double fp19;
    double fp20;
    double fp21;
    double fp22;
    double fp23;
    double fp24;
    double fp25;
    double fp26;
    double fp27;
    double fp28;
    double fp29;
    double fp30;
    double fp31;
    double fpscr;                    /* 240: saved FPSCR		*/
    #endif                           /* ndef _No_Floating_Point_Regs */
    #if __VEC__
    unsigned long        reserved_2; /* 248			*/
    unsigned long        reserved_3; /* 252			*/
    __vector signed long vr20;       /* 256			*/
    __vector signed long vr21;
    __vector signed long vr22;
    __vector signed long vr23;
    __vector signed long vr24;
    __vector signed long vr25;
    __vector signed long vr26;
    __vector signed long vr27;
    __vector signed long vr28;
    __vector signed long vr29;
    __vector signed long vr30;
    __vector signed long vr31;
    unsigned long        reserved_4;
    unsigned long        reserved_5;
    unsigned long        reserved_6;
    unsigned long        vscr;       /* 16-byte align VSCR	*/
    #endif
} __jmp_buf;

#endif                               /* __jmp_buf_h */

                                     /* Change record:
                                      * MEA 990723 created from setjmp.c
                                      * EJS 030106 merged with elf__jmp_buf.h
                                      */
