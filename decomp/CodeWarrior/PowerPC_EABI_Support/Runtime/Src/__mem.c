/****************************************************************************/
/*

FILE
    __mem.c
    $Date: 2008/02/27 17:08:09 $
    $Revision: 1.8 $

DESCRIPTION

    Standalone Standard C Library Implementation for memory operations.
    This is a target-specific implementation for PowerPC.  It will
    probably work for other target processors as well, but this one
    has been improved specifically for PowerPC.

    memcpy and memset are  put into the .init section so that they will
    be available for use in copying sections (e.g. .text) from rom into ram.

    Derived from MSL C (CodeWarrior 10).

    memcpy
    memset
    strlen

COPYRIGHT

    (c) 1996-2006 Freescale Semiconductor, Inc.
    All rights reserved.

*/
/****************************************************************************/

#if __MWERKS__
    #pragma ANSI_strict off
#endif

#include <__mem.h>
#include <string.h>

// default off
#ifndef USE_FAST_MEMCPY
    #define USE_FAST_MEMCPY 0
#endif

// check if user setup really allows to use fast memcpy
#if USE_FAST_MEMCPY &&                                                                         \
    (!(__supports(target_instruction, "lfd") && __option(floatingpoint) != 0) &&               \
         !(__supports(target_instruction, "evldd")) ||                                         \
     __PPCZen__)

    #undef USE_FAST_MEMCPY
    #define USE_FAST_MEMCPY 0
    #warning("Disabled fast memcpy since current options is not compatible")
#endif

#if __PPCGEKKO__ || USE_FAST_MEMCPY
asm void*
memcpy (void* dst, const void* src, size_t n)
{
    cmplwi cr1, r5, 0;
    beqlr  cr1;

    // If source < destination we must reverse copy to handle possible overlap
    cmplw cr1, r4, r3;
    blt   cr1, REVERSE;
    beqlr cr1;

    // Forward Copy Block

    // Copying less then four cache lines use word / byte copy.
    li    r6, 128;
    cmplw cr5, r5, r6;
    blt - cr5, TEST_WORD_ALIGNMENT; // (-) Assume this branch not taken

    // Test if source and destination are double aligned
    rlwinm r9, r4, 0, 29, 31;  // r9 = r4 & 7
    rlwinm r10, r3, 0, 29, 31; // r10 = r3 & 7
    subf   r8, r10, r3;        // r8 = r3 - (r3 & 7)

    #if __PPCGEKKO__
    dcbt 0, r4;                // Touch cache, hint we will be reading here soon.
    #endif

    // Test if source and destination are the same distance from alignment
    xor.r11, r10, r9;      // (.) Enables record mode. Tests target vs 0 and sets CR0
    bne - cr0, BYTE_SETUP; // Fail here src & dst are misaligned not by the same distance
                           // Here we must byte copy

    //  Test if they are double aligned
    andi.r10, r10, 7;        // If (r10 & 7 == 0), then goto DOUBLE_COPY_SETUP
    beq + DOUBLE_COPY_SETUP; //(+) Assume branch will be taken

    // Here source and destination are off by the same distance.
    // Align for double word copy after
    // We can never be more then 7 bytes off double word (8 byte) alignment
    // Therefore 8 - ( source & 0x07) = distance from alignment
    li    r6, 8;
    subf  r9, r9, r6;
    addi  r8, r3, 0;  // Copy destination, we do this to preserve the return which is
                      // already in R3
    mtctr r9;         // Move alignment distance into count register
    subf  r5, r9, r5; // n -= 8 - (source & 0x7)

    .align 4;
    // Word align loop
    // Copy bytes until both source and destination are double word aligned
BYTE_LOOP_DOUBLE_ALIGN:
    lbz  r9, 0(r4);
    addi r4, r4, 1;
    stb  r9, 0(r8);
    addi r8, r8, 1;
    bdnz BYTE_LOOP_DOUBLE_ALIGN;

DOUBLE_COPY_SETUP:
    // already substract above
    srwi  r6, r5, 5;  // n / 32
    mtctr r6;         // Set count register for double words to copy

    .align 4;
DOUBLE_LOOP:
    #if __supports(target_instruction, "lfd")
    lfd  f1, 0(r4);
    lfd  f2, 8(r4);
    lfd  f3, 16(r4);
    lfd  f4, 24(r4);
    addi r4, r4, 32;
    stfd f1, 0(r8);
    stfd f2, 8(r8);
    stfd f3, 16(r8);
    stfd f4, 24(r8);
    #else
    evldd  r7, 0(r4);
    evldd  r9, 8(r4);
    evldd  r10, 16(r4);
    evldd  r11, 24(r4);
    addi   r4, r4, 32;
    evstdd r7, 0(r8);
    evstdd r9, 8(r8);
    evstdd r10, 16(r8);
    evstdd r11, 24(r8);
    #endif

    addi r8, r8, 32;
    bdnz DOUBLE_LOOP;

    andi.r6, r5, 31;  // Number of bytes remaining after unrolled loop
    beqlr cr0;        // Return now if done (r6 == 0)

    addi r4, r4, -1;  // Using update load / stores so fix address
                      // The updated address is applied before use

    mtctr r6;         // Move number of bytes to count register
    addi  r8, r8, -1; // Fix up other address for update forms

                      // Clean up remaining bytes to copy
    .align 4;
BYTE_LOOP_1:
    lbzu r9, 1(r4);
    stbu r9, 1(r8);
    bdnz BYTE_LOOP_1;

    blr;                       // Return (dst)

TEST_WORD_ALIGNMENT:
    li    r6, 20;              // 20 or less byte copy. Note this MUST
                               // be greater (>) then
                               // (WORD SIZE - 1) + word loop unroll factor
                               // Otherwise the alignment loop may produce a remainder
                               // NOT divisible by the unrolling factor!
    cmplw cr5, r5, r6;
    ble - cr5, BYTE_SETUP;     // (-) Assume this branch not taken

    rlwinm r9, r4, 0, 30, 31;  // r7 = r4 & 3
    rlwinm r10, r3, 0, 30, 31; // r10 = r3 & 3

    xor.r11, r10, r9;          // Are they the same distance from alignment
    bne - cr0, BYTE_SETUP;

    // Here source and destination are off by the same distence.
    // Align for word copy after
    li    r6, 4;
    subf  r9, r9, r6; //  Update the number of bytes we copied to align
    addi  r8, r3, 0;
    subf  r5, r9, r5;
    mtctr r9;

                      // Byte copy to align on word boundary
    .align 4;
BYTE_LOOP_WORD_ALIGN:
    lbz  r9, 0(r4);
    addi r4, r4, 1;
    stb  r9, 0(r8);
    addi r8, r8, 1;
    bdnz BYTE_LOOP_WORD_ALIGN;

WORD_COPY_SETUP:
    srwi  r6, r5, 4;  // n / 16
    mtctr r6;

    .align 4;
WORD_LOOP:
    lwz  r9, 0(r4);
    lwz  r10, 4(r4);
    lwz  r11, 8(r4);
    lwz  r12, 12(r4);
    addi r4, r4, 16;
    stw  r9, 0(r8);
    stw  r10, 4(r8);
    stw  r11, 8(r8);
    stw  r12, 12(r8);
    addi r8, r8, 16;
    bdnz WORD_LOOP;

    andi.r6, r5, 15;  // Number of bytes remaining to copy from unrolled word loop
    beqlr cr0;        // If 0 return (dst)

    addi  r4, r4, -1;
    mtctr r6;         // Move number of bytes to count register
    addi  r8, r8, -1; // Fix offset since last store wrote 4 bytes from current address

                      // Byte copy remaining bytes from unrolled word copy
    .align 4;
BYTE_LOOP_2:
    lbzu r9, 1(r4);
    stbu r9, 1(r8);
    bdnz BYTE_LOOP_2;

    blr; // Return (dst)

    // Byte Copy - Below we copy bytes only. Here we have a source and destinations with
    // 		differing alignments.

BYTE_SETUP:
    addi  r7, r4, -1;
    addi  r8, r3, -1; // Using update loads and stores for pre-decrement
    mtctr r5;         // Move number of bytes to count register

    .align 4;
BYTE_LOOP_3:
    lbzu r9, 1(r7);
    stbu r9, 1(r8);
    bdnz BYTE_LOOP_3;

    blr;              // Return (dst)

    // ------------------------------------------------------------------------
    // Reverse Copy Block
    //
    //	r3	dst
    //	r4	src
    //	r5	n
    // ------------------------------------------------------------------------
REVERSE:
    // r4	= src + n
    // r12	= dst + n
    add r4, r4, r5;  // source + n
    add r12, r3, r5; // destination + n

    // Coping less then four cache lines use word / byte copy.
    li    r6, 128;
    cmplw cr5, r5, r6;
    blt - cr5, REVERSE_TEST_WORD_ALIGNMENT; // (-) Assume this branch not taken

    // Test if source and destination are double aligned
    // if ((r4 & 7) != (r12 & 7)) then goto REVERSE_BYTE_SETUP
    rlwinm r9, r4, 0, 29, 31;   // r9  = r4 & 7
    rlwinm r10, r12, 0, 29, 31; // r10 = r12 & 7

    // Test if source and destination are the same distance from alignment
    xor.r11, r10, r9;              // (.) Enables record mode. Tests target vs 0 and sets CR0
    bne - cr0, REVERSE_BYTE_SETUP; // Fail here src & dst are misaligned not by the same
                                   // distance Here we must byte copy

    //  Test if they are double aligned  // NOTE: we already know that r9 == r10
    andi.r10, r10, 7;                // If (r10 & 7 == 0), then goto REVERSE_DOUBLE_COPY_SETUP
    beq + REVERSE_DOUBLE_COPY_SETUP; // (+) Assume branch will be taken

    // Here source and destination are off by the same distance.
    // Align for double word copy after
    // We can never be more then 7 bytes off double word (8 byte) alignment
    // Therefore (source & 0x07) == distance from alignment
    mtctr r10; // Move alignment distance into count register

    // Copy bytes until both source and destination are double word aligned
    .align 4;          // Word align loop
REVERSE_BYTE_LOOP_DOUBLE_ALIGN:
    lbzu r9, -1(r4);
    stbu r9, -1(r12);
    bdnz REVERSE_BYTE_LOOP_DOUBLE_ALIGN;

REVERSE_DOUBLE_COPY_SETUP:
    subf  r5, r10, r5; // Update the number of bytes we copied to align
    srwi  r6, r5, 5;   // n / 32
    mtctr r6;          // Set count register for double words to copy

    .align 4;
REVERSE_DOUBLE_LOOP:
    #if __supports(target_instruction, "lfd")
    lfd   f1, -8(r4);
    lfd   f2, -16(r4);
    lfd   f3, -24(r4);
    lfd   f4, -32(r4);
    addi  r4, r4, -32;
    stfd  f1, -8(r12);
    stfd  f2, -16(r12);
    stfd  f3, -24(r12);
    stfdu f4, -32(r12);
    #else
    addi   r4, r4, -32;
    evldd  r7, 0(r4);
    evldd  r9, 8(r4);
    evldd  r10, 16(r4);
    evldd  r11, 24(r4);
    addi   r12, r12, -32;
    evstdd r7, 0(r12);
    evstdd r9, 8(r12);
    evstdd r10, 16(r12);
    evstdd r11, 24(r12);
    #endif

    bdnz REVERSE_DOUBLE_LOOP;

    andi.r6, r5, 31;   // Number of bytes remaining after unrolled loop
    beqlr cr0;         // Return now if done (r6 == 0)

    mtctr r6;          // Move number of bytes to count register

                       // Clean up remaining bytes to copy
    .align 4;
REVERSE_BYTE_LOOP_1:
    lbzu r9, -1(r4);
    stbu r9, -1(r12);
    bdnz REVERSE_BYTE_LOOP_1;

    blr;                           // Return (dst);

REVERSE_TEST_WORD_ALIGNMENT:
    li r6, 20;                     // 20 or less byte copy. Note this MUST
                                   // be greater (>) then
                                   // (WORD SIZE - 1) + word loop unroll factor
                                   // Otherwise the alignment loop may produce a
                                   // remainder NOT divisible by the unrolling factor!

    cmplw cr5, r5, r6;
    ble - cr5, REVERSE_BYTE_SETUP; // (-) Assume this branch not taken

    rlwinm r9, r4, 0, 30, 31;      // r9 = r4 & 3
    rlwinm r10, r12, 0, 30, 31;    // r10 = r12 & 3

    xor.r11, r10, r9;              // Are they the same distance from alignment
    bne - cr0, REVERSE_BYTE_SETUP;

    //  Test if they are double aligned  // NOTE: we already know that r9 == r10
    andi.r10, r10, 7;              // If (r10 & 7 == 0), then goto REVERSE_WORD_LOOP_SETUP
    beq + REVERSE_WORD_LOOP_SETUP; // (+) Assume branch will be taken

    // Here source and destination are off by the same distance.
    // Align for double word copy after
    // We can never be more then 7 bytes off double word (8 byte) alignment
    // Therefore ( source & 0x07) == distance from alignment
    mtctr r10; // Move alignment distance into count register

               // Byte copy to align on word boundary
    .align 4;
REVERSE_BYTE_LOOP_WORD_ALIGN:
    lbzu r9, -1(r4);
    stbu r9, -1(r12);
    bdnz REVERSE_BYTE_LOOP_WORD_ALIGN;

REVERSE_WORD_LOOP_SETUP:
    subf  r5, r10, r5; // Update the number of bytes we copied to align
    srwi  r6, r5, 4;   // n / 16
    mtctr r6;

    .align 4;
REVERSE_WORD_LOOP:
    lwz  r9, -4(r4);
    lwz  r10, -8(r4);
    lwz  r11, -12(r4);
    lwz  r8, -16(r4);
    addi r4, r4, -16;
    stw  r9, -4(r12);
    stw  r10, -8(r12);
    stw  r11, -12(r12);
    stwu r8, -16(r12);
    bdnz REVERSE_WORD_LOOP;

    andi.r6, r5, 15;   // Number of bytes remaining to copy from unrolled word loop
    beqlr cr0;         // If 0 return (dst)

    mtctr r6;          // Move number of bytes to count register

                       // Byte copy remaining bytes from unrolled word copy
    .align 4;
REVERSE_BYTE_LOOP_2:
    lbzu r9, -1(r4);
    stbu r9, -1(r12);
    bdnz REVERSE_BYTE_LOOP_2;

    blr; // Return (dst)

    // Byte Copy - Below we copy bytes only. Here we have a source and destinations with
    // 		differing alignments.

REVERSE_BYTE_SETUP:
    mtctr r5; // Move number of bytes to count register

    .align 4;
REVERSE_BYTE_LOOP_3:
    lbzu r9, -1(r4);
    stbu r9, -1(r12);
    bdnz REVERSE_BYTE_LOOP_3;

    blr;      // Return (dst)
}

#else

void*
memcpy (void* dst, const void* src, size_t n)
{
    const char* p;
    char*       q;
    int         rev = ((unsigned long)src < (unsigned long)dst);

    if (!rev)
    {
        for (p = (const char*)src - 1, q = (char*)dst - 1, n++; --n;) *++q = *++p;
    }
    else
    {
        for (p = (const char*)src + n, q = (char*)dst + n, n++; --n;) *--q = *--p;
    }
    return (dst);
}
#endif        // __PPCGEKKO__

/*
    mem_funcs.c
*/

#define cps               ((unsigned char*)src)
#define cpd               ((unsigned char*)dst)
#define lps               ((unsigned long*)src)
#define lpd               ((unsigned long*)dst)
#define deref_auto_inc(p) *++(p)

void
__fill_mem (void* dst, int val, unsigned long n)
{
    unsigned long v = (unsigned char)val;
    unsigned long i;

    cpd = ((unsigned char*)dst) - 1;

    if (n >= 32)
    {
        i = (~(unsigned long)dst) & 3;

        if (i)
        {
            n -= i;

            do deref_auto_inc (cpd) = (unsigned char)v;
            while (--i);
        }

        if (v)
            v |= v << 24 | v << 16 | v << 8;

        lpd = ((unsigned long*)(cpd + 1)) - 1;

        i   = n >> 5;

        if (i)
            do {
                deref_auto_inc (lpd) = v;
                deref_auto_inc (lpd) = v;
                deref_auto_inc (lpd) = v;
                deref_auto_inc (lpd) = v;
                deref_auto_inc (lpd) = v;
                deref_auto_inc (lpd) = v;
                deref_auto_inc (lpd) = v;
                deref_auto_inc (lpd) = v;
            }
            while (--i);

        i = (n & 31) >> 2;

        if (i)
            do deref_auto_inc (lpd) = v;
            while (--i);

        cpd  = ((unsigned char*)(lpd + 1)) - 1;

        n   &= 3;
    }

    if (n)
        do deref_auto_inc (cpd) = (unsigned char)v;
        while (--n);

    return;
}

void*
memset (void* dst, int val, size_t n)
{
    __fill_mem (dst, val, n);

    return (dst);
}

#if !__MC68K__ || _No_String_Inlines || !defined(__cplusplus)

__declspec (weak) size_t (strlen) (const char* str);

size_t (strlen) (const char* str)
{
    size_t len       = -1;

    unsigned char* p = (unsigned char*)str - 1;

    do len++;
    while (*++p);

    return (len);
}

#endif /* !__MC68K__ || _No_String_Inlines || !defined(__cplusplus) */
