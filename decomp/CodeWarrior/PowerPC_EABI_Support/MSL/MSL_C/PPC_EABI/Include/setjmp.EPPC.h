/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:28:44 $
 * $Revision: 1.9.2.1 $
 */

#ifndef _MSL_SETJMP_EPPC_H
    #define _MSL_SETJMP_EPPC_H

    #pragma options align = native

_MSL_BEGIN_NAMESPACE_STD
_MSL_BEGIN_EXTERN_C

    #if __VEC__

typedef __vector unsigned long jmp_buf[29];

        #ifndef __SETJMP_NOT_INTERNAL__
            #pragma internal on
        #endif

_MSL_IMP_EXP_C int __vec_setjmp (jmp_buf);

        #ifndef __SETJMP_NOT_INTERNAL__
            #pragma internal reset
        #endif

int __vec_longjmp (jmp_buf, int);

        #define setjmp(jmp_buf) __std (__vec_setjmp (jmp_buf)) /*- hh 990521 -*/

_MSL_INLINE void
longjmp (jmp_buf j, int val)
{
    __vec_longjmp (j, val);
}

    #else

        #if __PPCGEKKO__
typedef long jmp_buf[100];
        #elif __PPCe500__ || __PPCZen__ || __PPCe500v2
typedef long jmp_buf[44];
        #else
typedef long jmp_buf[70]; /* really 68... */
        #endif

        #ifndef __SETJMP_NOT_INTERNAL__
            #pragma internal on
        #endif

_MSL_IMP_EXP_C int __setjmp (jmp_buf);

        #ifndef __SETJMP_NOT_INTERNAL__
            #pragma internal reset
        #endif

        #define setjmp(jmp_buf) __std (__setjmp (jmp_buf))     /*- hh 990521 -*/
void longjmp (jmp_buf, int);

    #endif

_MSL_END_EXTERN_C
_MSL_END_NAMESPACE_STD

    #pragma options align = reset

#endif                                                         /* _MSL_SETJMP_EPPC_H */

                                                               /* Change record:
                                                                * JWW 011101 New file to make setjmp information platform independent
                                                                * EJS 020110 Merged e500 features
                                                                */
