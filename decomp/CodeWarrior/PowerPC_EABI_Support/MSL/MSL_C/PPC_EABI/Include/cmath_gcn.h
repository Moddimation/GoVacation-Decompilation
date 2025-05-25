/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:28:39 $
 * $Revision: 1.8.2.1 $
 */

/*
 *  --  Not intended for MacOS Platforms!  --
 *      See cmath.macos.h instead
 */

/*
 * Note:  This is a complete rewrite (Jan 1999).
 * It was created to be a portable cmath header for
 * all mwerks compilers/targets - except Mac OS.
 * Mac OS is unique in that it is currently the only
 * platform where CodeWarrior does not provide the math
 * portion of the C standard library. We simply hook into
 * the library provided by Apple as part of the Mac OS.
 */

#ifndef _MSL_CMATH_GCN
    #define _MSL_CMATH_GCN

    #ifndef __cmath__
        #define __cmath__ /* JWW - Keep the Apple Universal Interfaces happy */
    #endif

    #include <ansi_parms.h>

    #if !defined(_No_Floating_Point) && !defined(_MSL_NO_MATH_LIB)

        #define MATH_ERRNO       1
        #define MATH_ERREXCEPT   2

        #define math_errhandling _MSL_MATH_ERRHANDLING

        /* common macro definitions */
        #define FP_NAN           1 /*   quiet NaN (signaling Nan supported on MAC but nowhere else. */
        #define FP_INFINITE      2 /*   + or - infinity      */
        #define FP_ZERO          3 /*   + or - zero          */
        #define FP_NORMAL        4 /*   all normal numbers   */
        #define FP_SUBNORMAL     5 /*   denormal numbers     */

        #define DECIMAL_DIG      17

        #include <msl_t.h>

        #if __option(little_endian)
            #define __HI(x)  (sizeof (x) == 8 ? *(1 + (_INT32*)&x) : (*(_INT32*)&x))
            #define __LO(x)  (*(_INT32*)&x)
            #define __UHI(x) (sizeof (x) == 8 ? *(1 + (_UINT32*)&x) : (*(_UINT32*)&x))
            #define __ULO(x) (*(_UINT32*)&x)
        #else
            #define __LO(x)  (sizeof (x) == 8 ? *(1 + (_INT32*)&x) : (*(_INT32*)&x))
            #define __HI(x)  (*(_INT32*)&x)
            #define __ULO(x) (sizeof (x) == 8 ? *(1 + (_UINT32*)&x) : (*(_UINT32*)&x))
            #define __UHI(x) (*(_UINT32*)&x)
        #endif

_MSL_BEGIN_EXTERN_C

int __fpclassifyf (float);
int __fpclassifyd (double);

_MSL_END_EXTERN_C

        #if _MSL_USE_INLINE

_MSL_INLINE int
__fpclassifyf (float x)
{
    switch ((*(_INT32*)&x) & 0x7f800000)
    {
        case 0x7f800000:
            {
                if ((*(_INT32*)&x) & 0x007fffff)
                    return FP_NAN;
                else
                    return FP_INFINITE;
                break;
            }
        case 0:
            {
                if ((*(_INT32*)&x) & 0x007fffff)
                    return FP_SUBNORMAL;
                else
                    return FP_ZERO;
                break;
            }
    }
    return FP_NORMAL;
}

_MSL_INLINE int
__fpclassifyd (double x)
{
    switch (__HI (x) & 0x7ff00000)
    {
        case 0x7ff00000:
            {
                if ((__HI (x) & 0x000fffff) || (__LO (x) & 0xffffffff))
                    return FP_NAN;
                else
                    return FP_INFINITE;
                break;
            }
        case 0:
            {
                if ((__HI (x) & 0x000fffff) || (__LO (x) & 0xffffffff))
                    return FP_SUBNORMAL;
                else
                    return FP_ZERO;
                break;
            }
    }
    return FP_NORMAL;
}

        #endif                     /* _MSL_USE_INLINE */

        #define signbit(x) ((int)(__HI (x) & 0x80000000))

    /* 7.7 Defines */

        #if (FLT_EVAL_METHOD == 0)

typedef float  float_t;
typedef double double_t;

        #elif (FLT_EVAL_METHOD == 1)

typedef double float_t;
typedef double double_t;

        #elif (FLT_EVAL_METHOD == 2)

typedef long double float_t;
typedef long double double_t;

        #endif

    /*  7.7.3  Classification macros */

        #define fpclassify(x)                                                                  \
            ((sizeof (x) == sizeof (float)) ? __fpclassifyf ((float)(x))                       \
                                            : __fpclassifyd ((double)(x)))

_MSL_BEGIN_EXTERN_C

_MSL_IMP_EXP _INT32 __float_huge[];
_MSL_IMP_EXP _INT32 __float_nan[];
_MSL_IMP_EXP _INT32 __double_huge[];
_MSL_IMP_EXP _INT32 __extended_huge[];

    /*  special number macros */

        #define HUGE_VAL  (*(double*)__double_huge)
        #define INFINITY  (*(float*)__float_huge)
        #define NAN       (*(float*)__float_nan)
        #define HUGE_VALF (*(float*)__float_huge)
        #define HUGE_VALL (*(long double*)__extended_huge)

/* macros map onto condtion codes of x86 fpu status word
 * (it is more efficient to return the condition codes of the status word)
 */

_MSL_END_EXTERN_C

        #define isnormal(x) (fpclassify (x) == FP_NORMAL)
        #define isnan(x)    (fpclassify (x) == FP_NAN)
        #define isinf(x)    (fpclassify (x) == FP_INFINITE)
        #define isfinite(x) ((fpclassify (x) > FP_INFINITE))

        #if __MIPS__
float __s_abs (float x);
            #if __MIPS_ISA2__ && __fpu__
float __s_sqrt (float x);
            #endif
        #endif

_MSL_BEGIN_NAMESPACE_STD
_MSL_BEGIN_EXTERN_C

/*  we have/will have portable versions of the 22 K&R math.h functions only
 *  we may add C9X additions as necessary
 */

double pow (double, double);      /*- pmk 010404 -*/
double copysign (double, double); /*- pmk 010404 -*/
double exp (double);
double acos (double);
double asin (double);
double cos (double);
double sin (double);
double tan (double);
double cosh (double);
double sinh (double);
double tanh (double);
double atan (double);
double atan2 (double, double);
double fmod (double, double);
double fabs (double);
double sqrt (double);
double frexp (double, int* exp);
double ldexp (double, int exp);
double log (double);
double log10 (double);
double modf (double, double* iptr);
double ceil (double);
double floor (double);
double log2 (double);
double acosh (double);
double asinh (double);
double atanh (double);
double exp2 (double);
double expm1 (double);
double log1p (double);
double logb (double);

/* scalbn only differs from ldexp on non base 2 computers(i.e. nothing we support--m.f.
 * 02/05/99) */

double scalbn (double, int);
double scalbln (double, long);

        #if _MSL_USE_INLINE
_MSL_INLINE double
scalbn (double x, int n)
{
    return ldexp (x, n);
}

_MSL_INLINE double
scalbln (double x, long n)
{
    return ldexp (x, (int)n);
}
        #endif /* _MSL_USE_INLINE */

               /* scalbln can be changed whenever sizeof(long) > sizeof(int)
                * however, since x*2^^INT_MAX=Inf, the additional range allowed by a larger long
                * adds nothing useful to this, not sure why C9X even has scalbln???
                */

double hypot (double, double);

        #ifndef __INTEL__
double erf (double);
double erfc (double);
double gamma (double);
double lgamma (double);
double nextafter (double, double);
        #endif

double nearbyint (double);
double rint (double);
long   rinttol (double);

        /* GCN does not have round */
        #if 0  
	 double round(double);
	 long   roundtol(double);
        #endif
double trunc (double);
double remainder (double, double);
double remquo (double, double, int* quo);
double fdim (double, double);
double fmax (double, double);
double fmin (double, double);

long int lrint (double);
long int lround (double);

        #if _MSL_LONGLONG
long long llrint (double);
long long llround (double);
        #endif                                     /*_MSL_LONGLONG*/

        #if ((__dest_os == __mac_os) || __INTEL__) /*- mm 991216 -*/
double nan (const char*);                          /*- mm 991216 -*/
        #else                                      /*- mm 991216 -*/
            #ifdef __cplusplus                     /*- hh 010411 -*/
inline double
nan (const char*)
{
    return NAN;
} /*- hh 010411 -*/
            #else                                  /*- hh 010411 -*/
                #define nan(x) NAN                 /*- mm 991216 -*/
            #endif                                 /*- hh 010411 -*/
        #endif                                     /*- mm 991216 -*/

_MSL_END_EXTERN_C
_MSL_END_NAMESPACE_STD

_MSL_BEGIN_NAMESPACE_STD

/* long double math functions(fool's), we do not support true long double outside of macos 68K
 */

long double acosl (long double);
long double asinl (long double);
long double atanl (long double);
long double atan2l (long double, long double);
long double cosl (long double);
long double sinl (long double);
long double tanl (long double);
long double coshl (long double);
long double sinhl (long double);
long double tanhl (long double);
long double acoshl (long double);
long double asinhl (long double);
long double atanhl (long double);
long double expl (long double);
long double frexpl (long double, int*);
long double ldexpl (long double, int);
long double logl (long double);
long double log10l (long double);
long double modfl (long double, long double*);
long double exp2l (long double);
long double expm1l (long double);
long double log1pl (long double);
long double log2l (long double);
long double logbl (long double);
long double scalbnl (long double, int);
long double scalblnl (long double, long int);
long double fabsl (long double);
long double powl (long double, long double);
long double sqrtl (long double);
long double hypotl (long double, long double);

        #ifndef __INTEL__
long double erfl (long double);
long double erfcl (long double);
long double gammal (long double);
long double lgammal (long double);
long double nextafterl (long double, long double);
        #endif

long double ceill (long double);
long double floorl (long double);
long double nearbyintl (long double);
long double rintl (long double);
long int    lrintl (long double);

        #if _MSL_LONGLONG
long long llrintl (long double);
long long llroundl (long double);
        #endif /*_MSL_LONGLONG*/

long double roundl (long double);
long int    lroundl (long double);
long double truncl (long double);
long double fmodl (long double, long double);
long double remainderl (long double, long double);
long double copysignl (long double, long double);
long double remquol (long double, long double, int*);
long double fdiml (long double, long double);
long double fmaxl (long double, long double);
long double fminl (long double, long double);

_MSL_INLINE long double
acosl (long double x)
{
    return acos ((double_t)x);
}

_MSL_INLINE long double
asinl (long double x)
{
    return asin ((double_t)x);
}

_MSL_INLINE long double
atanl (long double x)
{
    return atan ((double_t)x);
}

_MSL_INLINE long double
atan2l (long double y, long double x)
{
    return atan2 ((double_t)y, (double_t)x);
}

_MSL_INLINE long double
cosl (long double x)
{
    return cos ((double_t)x);
}

_MSL_INLINE long double
sinl (long double x)
{
    return sin ((double_t)x);
}

_MSL_INLINE long double
tanl (long double x)
{
    return tan ((double_t)x);
}

_MSL_INLINE long double
coshl (long double x)
{
    return cosh ((double_t)x);
}

_MSL_INLINE long double
sinhl (long double x)
{
    return sinh ((double_t)x);
}

_MSL_INLINE long double
tanhl (long double x)
{
    return tanh ((double_t)x);
}

_MSL_INLINE long double
acoshl (long double x)
{
    return acosh ((double_t)x);
}

_MSL_INLINE long double
asinhl (long double x)
{
    return asinh ((double_t)x);
}

_MSL_INLINE long double
atanhl (long double x)
{
    return atanh ((double_t)x);
}

_MSL_INLINE long double
expl (long double x)
{
    return exp ((double_t)x);
}

_MSL_INLINE long double
frexpl (long double x, int* exp)
{
    return frexp ((double_t)x, exp);
}

_MSL_INLINE long double
ldexpl (long double x, int exp)
{
    return ldexp ((double_t)x, exp);
}

_MSL_INLINE long double
logl (long double x)
{
    return log ((double_t)x);
}

_MSL_INLINE long double
log10l (long double x)
{
    return log10 ((double_t)x);
}

_MSL_INLINE long double
modfl (long double x, long double* iptr)
{
    double      iptrd;
    long double result = modf ((double)x, &iptrd);
    *iptr              = iptrd;
    return result;
}

_MSL_INLINE long double
exp2l (long double x)
{
    return exp2 ((double_t)x);
}

_MSL_INLINE long double
expm1l (long double x)
{
    return expm1 ((double_t)x);
}

_MSL_INLINE long double
log1pl (long double x)
{
    return log1p ((double_t)x);
}

_MSL_INLINE long double
log2l (long double x)
{
    return log2 ((double_t)x);
}

_MSL_INLINE long double
logbl (long double x)
{
    return logb ((double_t)x);
}

_MSL_INLINE long double
scalbnl (long double x, int n)
{
    return ldexpl (x, n);
}

_MSL_INLINE long double
scalblnl (long double x, long int n)
{
    return ldexp (x, (int)n);
}

_MSL_INLINE long double
fabsl (long double x)
{
    return fabs ((double_t)x);
}

_MSL_INLINE long double
powl (long double x, long double y)
{
    return pow ((double_t)x, (double_t)y);
}

_MSL_INLINE long double
sqrtl (long double x)
{
    return sqrt ((double_t)x);
}

_MSL_INLINE long double
hypotl (long double x, long double y)
{
    return hypot ((double_t)x, (double_t)y);
}

        #ifndef __INTEL__
_MSL_INLINE long double
erfl (long double x)
{
    return erf ((double_t)x);
}

_MSL_INLINE long double
erfcl (long double x)
{
    return erfc ((double_t)x);
}

_MSL_INLINE long double
gammal (long double x)
{
    return gamma ((double_t)x);
}

_MSL_INLINE long double
lgammal (long double x)
{
    return lgamma ((double_t)x);
}

_MSL_INLINE long double
nextafterl (long double x, long double y)
{
    return nextafter ((double)x, (double)y);
}
        #endif

_MSL_INLINE long double
ceill (long double x)
{
    return ceil ((double_t)x);
}

_MSL_INLINE long double
floorl (long double x)
{
    return floor ((double_t)x);
}

_MSL_INLINE long double
nearbyintl (long double x)
{
    return nearbyint ((double_t)x);
}

_MSL_INLINE long double
rintl (long double x)
{
    return rint ((double_t)x);
}

_MSL_INLINE long int
lrintl (long double x)
{
    return lrint ((double)x);
}

        #if _MSL_LONGLONG
_MSL_INLINE long long
llrintl (long double x)
{
    return llrint ((double)x);
}

            /* GCN does not have round */
            #if 0 	
			_MSL_INLINE long long llroundl(long double x)
			{return llround((double)x);}
            #endif

        #endif /*_MSL_LONGLONG*/

        /* GCN does not have round */
        #if 0 
		_MSL_INLINE long double roundl(long double x)
			{return round((double_t)x);}
		_MSL_INLINE long int lroundl(long double x)
			{return lround((double)x);}
        #endif

_MSL_INLINE long double
truncl (long double x)
{
    return trunc ((double_t)x);
}

_MSL_INLINE long double
fmodl (long double x, long double y)
{
    return fmod ((double_t)x, (double_t)y);
}

_MSL_INLINE long double
remainderl (long double x, long double y)
{
    return remainder ((double_t)x, (double_t)y);
}

_MSL_INLINE long double
copysignl (long double x, long double y)
{
    return copysign ((double_t)x, (double_t)y);
}

_MSL_INLINE long double
remquol (long double x, long double y, int* quo)
{
    return remquo ((double_t)x, (double_t)y, quo);
}

_MSL_INLINE long double
fdiml (long double x, long double y)
{
    return fdim ((double_t)x, (double_t)y);
}

_MSL_INLINE long double
fmaxl (long double x, long double y)
{
    return fmax ((double_t)x, (double_t)y);
}

_MSL_INLINE long double
fminl (long double x, long double y)
{
    return fmin ((double_t)x, (double_t)y);
}

/*	  #endif      */

/* C9X foof's-- only the 22 K&R math functions have actual foof implementations(prototypes),
 *  keep the others as inlines returning double until/if they are actually written.
 */

_MSL_END_NAMESPACE_STD

        #if !(__INTEL__ && __option(k63d) || __SH__)

_MSL_BEGIN_NAMESPACE_STD

float acosf (float);
float asinf (float);
float atanf (float);
float atan2f (float, float);
float cosf (float);
float sinf (float);
float tanf (float);
float coshf (float);
float sinhf (float);
float tanhf (float);
float expf (float);
float frexpf (float, int*);
float ldexpf (float, int);
float logf (float);
float log10f (float);
float fabsf (float);
float powf (float, float);
float sqrtf (float);
float ceilf (float);
float floorf (float);
float fmodf (float, float);
float log2f (float);

_MSL_INLINE float
acosf (float x)
{
    return (float)acos ((double_t)x);
}

_MSL_INLINE float
asinf (float x)
{
    return (float)asin ((double_t)x);
}

_MSL_INLINE float
atanf (float x)
{
    return (float)atan ((double_t)x);
}

_MSL_INLINE float
atan2f (float y, float x)
{
    return (float)atan2 ((double_t)y, (double_t)x);
}

_MSL_INLINE float
cosf (float x)
{
    return (float)cos ((double_t)x);
}

_MSL_INLINE float
sinf (float x)
{
    return (float)sin ((double_t)x);
}

_MSL_INLINE float
tanf (float x)
{
    return (float)tan ((double_t)x);
}

_MSL_INLINE float
coshf (float x)
{
    return (float)cosh ((double_t)x);
}

_MSL_INLINE float
sinhf (float x)
{
    return (float)sinh ((double_t)x);
}

_MSL_INLINE float
tanhf (float x)
{
    return (float)tanh ((double_t)x);
}

_MSL_INLINE float
expf (float x)
{
    return (float)exp ((double_t)x);
}

_MSL_INLINE float
frexpf (float x, int* exp)
{
    return (float)frexp ((double_t)x, exp);
}

_MSL_INLINE float
ldexpf (float x, int exp)
{
    return (float)ldexp ((double_t)x, exp);
}

_MSL_INLINE float
logf (float x)
{
    return (float)log ((double_t)x);
}

_MSL_INLINE float
log10f (float x)
{
    return (float)log10 ((double_t)x);
}

_MSL_INLINE float
fabsf (float x)
{
            #if __MIPS__
    return __s_abs (x);
            #else
    return (float)fabs ((double_t)x);
            #endif
}

_MSL_INLINE float
powf (float x, float y)
{
    return (float)pow ((double_t)x, (double_t)y);
}

/*		use D.S. version of sqrtf 			*/

/*
_MSL_INLINE float sqrtf(float x)
        {
             #if __MIPS__ && __MIPS_ISA2__ && __fpu__

                    if( x >= 0.0)
                     return __s_sqrt(x);
                    else
                     return NAN;
             #else
                     return (float)sqrt((double_t)x);
             #endif
        }
*/
_MSL_INLINE float
ceilf (float x)
{
    return (float)ceil ((double_t)x);
}

_MSL_INLINE float
floorf (float x)
{
    return (float)floor ((double_t)x);
}

_MSL_INLINE float
fmodf (float x, float y)
{
    return (float)fmod ((double_t)x, (double_t)y);
}

_MSL_INLINE float
log2f (float x)
{
    return (float)log2 ((double_t)x);
}

_MSL_END_NAMESPACE_STD

        #endif                          /*( __INTEL__ && __option(k63d) )*/

        #include <math_config.h>

_MSL_BEGIN_NAMESPACE_STD

        #pragma cplusplus on

float modff (float, float*);

_MSL_INLINE float
modff (float x, float* iptr)
{
    switch (fpclassify (x))
    {
        case FP_NAN:
        case FP_ZERO:
            *iptr = x;
            return x;
            break;
        case FP_INFINITE:
            *iptr = x;
            return 0.0f;
            break;
        case FP_SUBNORMAL:
            *iptr = 0.0f;
            return x;
            break;
        default:                        /*normal case */
            const _INT32 i = (_INT32)x; /*truncated toward zero */
            const float  frac =
                x - (float)i; /* fractional part of x(subtraction is exact in this case) */
            if ((!(*(_INT32*)&frac)) || ((*(_INT32*)&x) & 0x7f800000) >= 0x4B800000)
            {
                *iptr = x;
                return 0.0f;
            }
            *iptr = i;
            return frac;
    }
}

        #pragma cplusplus reset

float acoshf (float);
float asinhf (float);
float atanhf (float);
float exp2f (float);
float expm1f (float);
float log1pf (float);
float logbf (float);
float scalbnf (float, int);
float scalblnf (float, long int);
float hypotf (float, float);

        #ifndef __INTEL__
float erff (float);
float erfcf (float);
float gammaf (float);
float lgammaf (float);
float nextafterf (float, float);
        #endif

float    nearbyintf (float);
float    rintf (float);
long int lrintf (float);
float    roundf (float);
long int lroundf (float);

        #if _MSL_LONGLONG
long long llroundf (float);
long long llrintf (float);
        #endif                /*_MSL_LONGLONG*/

float truncf (float);
float remainderf (float, float);
float copysignf (float, float);
float remquof (float, float, int*);
float fdimf (float, float);
float fmaxf (float, float);
float fminf (float, float);

_MSL_INLINE float
acoshf (float x)
{
    return (float)acosh ((double_t)x);
}

_MSL_INLINE float
asinhf (float x)
{
    return (float)asinh ((double_t)x);
}

_MSL_INLINE float
atanhf (float x)
{
    return (float)atanh ((double_t)x);
}

_MSL_INLINE float
exp2f (float x)
{
    return (float)exp2 ((double_t)x);
}

_MSL_INLINE float
expm1f (float x)
{
    return (float)expm1 ((double_t)x);
}

_MSL_INLINE float
log1pf (float x)
{
    return (float)log1p ((double_t)x);
}

_MSL_INLINE float
logbf (float x)
{
    return (float)logb ((double_t)x);
}

_MSL_INLINE float
scalbnf (float x, int n)
{
    return (float)ldexpf (x, n);
}

_MSL_INLINE float
scalblnf (float x, long int n)
{
    return (float)ldexpf (x, (int)n);
}

_MSL_INLINE float
hypotf (float x, float y)
{
    return (float)hypot ((double_t)x, (double_t)y);
}

        #ifndef __INTEL__
_MSL_INLINE float
erff (float x)
{
    return (float)erf ((double_t)x);
}

_MSL_INLINE float
erfcf (float x)
{
    return (float)erfc ((double_t)x);
}

_MSL_INLINE float
gammaf (float x)
{
    return (float)gamma ((double_t)x);
}

_MSL_INLINE float
lgammaf (float x)
{
    return (float)lgamma ((double_t)x);
}

_MSL_INLINE float
nextafterf (float x, float y)
{
    return (float)nextafter ((double_t)x, (double_t)y);
}
        #endif

_MSL_INLINE float
nearbyintf (float x)
{
    return (float)nearbyint ((double_t)x);
}

_MSL_INLINE float
rintf (float x)
{
    return (float)rint ((double_t)x);
}

_MSL_INLINE long int
lrintf (float x)
{
    return lrint ((double)x);
}

        /* GCN does not have round */
        #if 0 
		_MSL_INLINE float roundf(float x)
			{return (float)round((double_t)x);}
		_MSL_INLINE long int lroundf(float x)
			{return lround((double)x);}
        #endif

        #if _MSL_LONGLONG
_MSL_INLINE long long
llroundf (float x)
{
    return llround ((double)x);
}

_MSL_INLINE long long
llrintf (float x)
{
    return llrint ((double)x);
}
        #endif                      /*_MSL_LONGLONG*/

_MSL_INLINE float
truncf (float x)
{
    return (float)trunc ((double_t)x);
}

_MSL_INLINE float
remainderf (float x, float y)
{
    return (float)remainder ((double_t)x, (double_t)y);
}

_MSL_INLINE float
copysignf (float x, float y)
{
    return (float)copysign ((double_t)x, (double_t)y);
}

_MSL_INLINE float
remquof (float x, float y, int* quo)
{
    return (float)remquo ((double_t)x, (double_t)y, quo);
}

_MSL_INLINE float
fdimf (float x, float y)
{
    return (float)fdim ((double_t)x, (double_t)y);
}

_MSL_INLINE float
fmaxf (float x, float y)
{
    return (float)fmax ((double_t)x, (double_t)y);
}

_MSL_INLINE float
fminf (float x, float y)
{
    return (float)fmin ((double_t)x, (double_t)y);
}

        #ifdef __cplusplus
            #ifdef __ANSI_OVERLOAD__

_MSL_INLINE float
abs (float x)
{
    return fabsf (x);
}

_MSL_INLINE double
abs (double x)
{
    return fabs (x);
}

_MSL_INLINE long double
abs (long double x)
{
    return fabsl (x);
}
            #endif

_MSL_END_NAMESPACE_STD

            #if defined(__ANSI_OVERLOAD__)

                #include <tgmath.h> /* tgmath has only overloads for the 22 standard */
                /* K&R math functions, the C9X extensions are below */
                #include <math_integral.h>

_MSL_BEGIN_NAMESPACE_STD

float acosh (float);
float asinh (float);
float atanh (float);
float exp2 (float);
float expm1 (float);
float log1p (float);
float log2 (float);
float logb (float);
float scalbn (float, int);
float scalbln (float, long int);
float hypot (float, float);

                #ifndef __INTEL__
float erf (float);
float erfc (float);
float gamma (float);
float lgamma (float);
float nextafter (float, float);
                #endif

float    nearbyint (float);
float    rint (float);
long int lrint (float);

                /* GCN does not have round */
                #if 0 
				float round(float);
				long int lround(float);

                    #if _MSL_LONGLONG 
					long long llround(float);
					long long llrint(float);
                    #endif /*_MSL_LONGLONG*/
                #endif

float trunc (float);
float remainder (float, float);
float copysign (float, float);
float remquo (float, float, int*);
float fdim (float, float);
float fmax (float, float);
float fmin (float, float);

/*  foo(long double) support */

long int lrint (long double);

                #if _MSL_LONGLONG
long long llrint (long double);
long long llround (long double);
                #endif /*_MSL_LONGLONG*/

long int lround (long double);

                #ifndef __INTEL__
long double nextafter (long double, long double);
                #endif

long double acosh (long double);
long double asinh (long double);
long double atanh (long double);
long double exp2 (long double);
long double expm1 (long double);
long double log1p (long double);
long double log2 (long double);
long double logb (long double);
long double hypot (long double, long double);

                #ifndef __INTEL__
long double erf (long double);
long double erfc (long double);
long double gamma (long double);
long double lgamma (long double);
                #endif

long double nearbyint (long double);
long double rint (long double);

                /* GCN does not have round */
                #if 0 
				long double round(long double);
                #endif

long double trunc (long double);
long double remainder (long double, long double);
long double copysign (long double, long double);
long double remquo (long double, long double, int*);
long double fdim (long double, long double);
long double fmax (long double, long double);
long double fmin (long double, long double);
long double scalbn (long double, int);
long double scalbln (long double, long int);

_MSL_INLINE float
acosh (float x)
{
    return acoshf (x);
}

_MSL_INLINE float
asinh (float x)
{
    return asinhf (x);
}

_MSL_INLINE float
atanh (float x)
{
    return atanhf (x);
}

_MSL_INLINE float
exp2 (float x)
{
    return exp2f (x);
}

_MSL_INLINE float
expm1 (float x)
{
    return expm1f (x);
}

_MSL_INLINE float
log1p (float x)
{
    return log1pf (x);
}

_MSL_INLINE float
log2 (float x)
{
    return log2f (x);
}

_MSL_INLINE float
logb (float x)
{
    return logbf (x);
}

_MSL_INLINE float
scalbn (float x, int n)
{
    return ldexpf (x, n);
}

_MSL_INLINE float
scalbln (float x, long int n)
{
    return ldexpf (x, (int)n);
}

_MSL_INLINE float
hypot (float x, float y)
{
    return hypotf (x, y);
}

                #ifndef __INTEL__
_MSL_INLINE float
erf (float x)
{
    return erff (x);
}

_MSL_INLINE float
erfc (float x)
{
    return erfcf (x);
}

_MSL_INLINE float
gamma (float x)
{
    return gammaf (x);
}

_MSL_INLINE float
lgamma (float x)
{
    return lgammaf (x);
}

_MSL_INLINE float
nextafter (float x, float y)
{
    return nextafterf (x, y);
}
                #endif

_MSL_INLINE float
nearbyint (float x)
{
    return nearbyintf (x);
}

_MSL_INLINE float
rint (float x)
{
    return rintf (x);
}

_MSL_INLINE long int
lrint (float x)
{
    return lrintf (x);
}

                /* GCN does not have round */
                #if 0 
					_MSL_INLINE float round(float x)
						{return roundf(x);}
					_MSL_INLINE long int lround(float x)
						{return lroundf(x);}
                #endif

                #if _MSL_LONGLONG
_MSL_INLINE long long
llround (float x)
{
    return llroundf (x);
}

_MSL_INLINE long long
llrint (float x)
{
    return llrintf (x);
}
                #endif /*_MSL_LONGLONG*/

_MSL_INLINE float
trunc (float x)
{
    return truncf (x);
}

_MSL_INLINE float
remainder (float x, float y)
{
    return remainderf (x, y);
}

_MSL_INLINE float
copysign (float x, float y)
{
    return copysignf (x, y);
}

_MSL_INLINE float
remquo (float x, float y, int* quo)
{
    return remquof (x, y, quo);
}

_MSL_INLINE float
fdim (float x, float y)
{
    return fdimf (x, y);
}

_MSL_INLINE float
fmax (float x, float y)
{
    return fmaxf (x, y);
}

_MSL_INLINE float
fmin (float x, float y)
{
    return fminf (x, y);
}

                       /*  foo(long double) support */

_MSL_INLINE long int
lrint (long double x)
{
    return lrintl (x);
}

                #if _MSL_LONGLONG
_MSL_INLINE long long
llrint (long double x)
{
    return llrintl (x);
}

_MSL_INLINE long long
llround (long double x)
{
    return llroundl (x);
}
                #endif /*_MSL_LONGLONG*/

_MSL_INLINE long int
lround (long double x)
{
    return lroundl (x);
}

                #ifndef __INTEL__
_MSL_INLINE long double
nextafter (long double x, long double y)
{
    return nextafterl (x, y);
}
                #endif

_MSL_INLINE long double
acosh (long double x)
{
    return acoshl (x);
}

_MSL_INLINE long double
asinh (long double x)
{
    return asinhl (x);
}

_MSL_INLINE long double
atanh (long double x)
{
    return atanhl (x);
}

_MSL_INLINE long double
exp2 (long double x)
{
    return exp2l (x);
}

_MSL_INLINE long double
expm1 (long double x)
{
    return expm1l (x);
}

_MSL_INLINE long double
log1p (long double x)
{
    return log1pl (x);
}

_MSL_INLINE long double
log2 (long double x)
{
    return log2l (x);
}

_MSL_INLINE long double
logb (long double x)
{
    return logbl (x);
}

_MSL_INLINE long double
hypot (long double x, long double y)
{
    return hypotl (x, y);
}

                #ifndef __INTEL__
_MSL_INLINE long double
erf (long double x)
{
    return erfl (x);
}

_MSL_INLINE long double
erfc (long double x)
{
    return erfcl (x);
}

_MSL_INLINE long double
gamma (long double x)
{
    return gammal (x);
}

_MSL_INLINE long double
lgamma (long double x)
{
    return lgammal (x);
}
                #endif

_MSL_INLINE long double
nearbyint (long double x)
{
    return nearbyintl (x);
}

_MSL_INLINE long double
rint (long double x)
{
    return rintl (x);
}

                /* GCN does not have round */
                #if 0 
					_MSL_INLINE long double round(long double x)
						{return roundl(x);}
                #endif
_MSL_INLINE long double
trunc (long double x)
{
    return truncl (x);
}

_MSL_INLINE long double
remainder (long double x, long double y)
{
    return remainderl (x, y);
}

_MSL_INLINE long double
copysign (long double x, long double y)
{
    return copysignl (x, y);
}

_MSL_INLINE long double
remquo (long double x, long double y, int* quo)
{
    return remquol (x, y, quo);
}

_MSL_INLINE long double
fdim (long double x, long double y)
{
    return fdiml (x, y);
}

_MSL_INLINE long double
fmax (long double x, long double y)
{
    return fmaxl (x, y);
}

_MSL_INLINE long double
fmin (long double x, long double y)
{
    return fminl (x, y);
}

_MSL_INLINE long double
scalbn (long double x, int n)
{
    return ldexpl (x, n);
}

_MSL_INLINE long double
scalbln (long double x, long int n)
{
    return ldexpl (x, (int)n);
}

_MSL_END_NAMESPACE_STD

            #endif /* defined(__ANSI_OVERLOAD__) */

        #endif     /*defined(__cplusplus) */

    #endif         /* !defined(_No_Floating_Point) && !defined(_MSL_NO_MATH_LIB) */

#endif             /* _MSL_CMATH_GCN */

                   /* Change record:
                    * mf  981019 created portable cmath for all mwerks compilers/targets--not intended for mac_os
                    * mf  990115 rewrote for cross platform portability
                    * mf  990118 added inclusion of amd_k63dnow!/tgmath headers for amd+ changes from Brian Stern
                    * mf  990127 tgmath for ansi_overloads
                    * mf  990202 move k63d pragma to allow classification macros to have k6 calling conv(prevents
                    *            femms from being generated(i.e return value is not on fp stack))
                    * mf  990205 fixed up scalbn mess, now it just returns ldexp.-- note: scalbn files
                    *            can be removed from fdlibm
                    * mf  990217 moved modff to actual inlined implementation in __inline section
                    * mf  990309 pragma at line 566 removed(wrong place)
                    * mf  990513 fixed implicit type conversion warning for signbitf
                    *            this is an irrelevant fix because signbitf/d have been
                    *            removed in other branches of msl, this comment should be removed when the merge
                    *            of these two version occurs
                    * mm  990520 Added wrappers to long long function definitions
                    * vss 990803 Cleaned up code a little bit
                    * vss 990803 Make __float_nan, __float_huge __double_huge and __extended_huge agree
                    *            with the types they are in cfloat and float.c
                    * mm  991216 Added prototype for nan()
                    * mf  000417 cleanup namespace stuff changed __inline to __inline
                    * mf  000417 only export some of the foofs for k6--they are inlined
                    * mf  000619 turned off k6 calling convention for double functions temporarily.
                    * as  000823 _MSL_USING_NAMESPACE was out of wack when __ANSI_OVERLOAD__ was not defined.
                    * JWW 001208 Added case for targeting Mach-O
                    * cc  010124 Changed FLT_EVAL_METHOD from 0 to 2
                    * pmk 010404 removed pragma k63d_calls wrappers around pow and copysign
                    * cc  010405 removed pragma options align native and reset
                    * cc  010409 updated defines to JWW new namespace macros
                    * hh  010411 changed the #define nan to an inline function when C++ is turned on
                    * cc  010510 removed __STDC_IEC_559__
                    * ejs 010622 FLT_EVAL_METHOD is 0 for most targets *except* Intel using x87 FP.
                    * JWW 010918 Added definitions for C99 math error handling macros
                    * JWW 011027 Use _MSL_USING_MW_C_HEADERS as generic header test instead of specific Mach-O test
                    * cc  011029 Changed || to &&
                    * cc  011115 fixed the INLINE vs __inline
                    * blc 011211 Added _MSL_NO_MATH_LIB check
                    * JWW 030224 Changed __MSL_LONGLONG_SUPPORT__ flag into the new more configurable _MSL_LONGLONG
                    */
