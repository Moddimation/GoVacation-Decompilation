/* MSL
 * Copyright � 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:24:13 $
 * $Revision: 1.45.2.1 $
 */

/*
 *	Routines
 *	--------
 *		wcstold
 *		wcstod
 *		wcstof
 *
 *	Implementation
 *	--------------
 *
 *		The string scanner is implemented as an extended Finite State Machine.
 *		A state diagram for it can be found in an accompanying TeachText
 *		document, 'wcstod syntax' (too bad pictures can't be imbedded in
 *		comments) in the "MSL Technical Notes" directory. A textual description
 *      of it follows.
 *
 *		The state transition loop dispatches to the appropriate code for the
 *		current state, while simultaneously watching for terminating
 *		conditions (field width exhausted, EOF encountered, final state
 *		reached).
 *
 *		start
 *
 *			Skip leading spaces. Once a non-space is seen, process sign (if any)
 *			and trasition to next state.
 *
 *		sig_start
 *
 *			Look for either a digit or a decimal point. If it is a digit zero,
 *			treat it specially.
 *
 *		leading_sig_zeroes
 *
 *			Leading zero digits are discarded, as they add nothing to the result.
 *
 *		int_digit_loop
 *
 *			Process digits from the integer part of the significand. We accept
 *			only so many significant digits (DBL_DIG), but the ones we discard
 *			have to be accounted for in the exponent.
 *
 *			If a decimal point is seen, proceed to process a fractional part (if
 *			one is present).
 *
 *		frac_start
 *
 *			Having seen a leading decimal point, we must see at least one digit.
 *			If the field width expires before the transition from this state to
 *			the next, we fail.
 *
 *		frac_digit_loop
 *
 *			Process digits from the fractional part of the significand. We accept
 *			only so many significant digits (DBL_DIG), but the ones we discard
 *			have to be accounted for in the exponent.
 *		sig_end
 *
 *			If an 'E' (or 'e') follows we go after an exponent; otherwise we're
 *			done.
 *
 *		exp_start
 *
 *			Process the sign (if any).
 *
 *		leading_exp_digit
 *
 *			Check the leading exponent digit. If it is a digit zero, treat it
 *			specially.
 *
 *		leading_exp_zeroes
 *
 *			Leading zero digits are discarded, as they add nothing to the result.
 *
 *		exp_digit_loop
 *
 *			Process digits from the exponent. We watch for short int overflow,
 *			even though the maximum exponent is probably considerably less than
 *			this. The latter will be checked during the actual decimal to binary
 *			conversion.
 *
 *		finished
 *
 *			Successful exit.
 *
 *		failure
 *
 *			Invalid input exit.
 *
 *		The end product is just the parsed input and its conversion to a
 *		'decimal' record � la SANE and MathLib. '__dec2num' is used for conversion
 *		to binary. For other systems that don't provide decimal to binary
 *		conversion in this or a similar way, a routine will be provided.
 */

#pragma ANSI_strict off /*- vss 990729 -*/

#include <ansi_parms.h>

#if _MSL_FLOATING_POINT && !defined(_MSL_NO_MATH_LIB)

    #if _MSL_WIDE_CHAR  /*- mm 980204 -*/

        #pragma ANSI_strict reset

        #include <ansi_fp.h>
        #include <ctype.h>
        #include <errno.h>
        #include <float.h>
        #include <lconv.h>
        #include <limits.h>
        #include <locale.h>
        #include <msl_thread_local_data.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <wchar.h>
        #include <wctype.h>

        /*
            The hex-float scanning routine assumes an IEEE 754 float layout, i.e.:

            [sign_bit] [biased_exponent] [partial_mantissa]

            where the real_exponent is (biased_exponent - TARGET_FLOAT_MAX_EXP + 1),
            where the real_mantissa is "1.partial_mantissa" for 32 and 64 bit doubles
            and ".partial_mantissa" for 80-bit doubles (with an explicit 1 bit),
            and the value is (real_mantissa)*2^real_exponent*(sign_bit?-1:1).
        */
        #define TARGET_FLOAT_BITS  _MSL_LONG_DOUBLE_SIZE
        #define TARGET_FLOAT_BYTES (TARGET_FLOAT_BITS / 8)

        /*	Define "maximum" exponent
            The bias is max_exp-1, and the maximal value fitting
            in the exponent field is max_exp*2-1 */
        #ifndef TARGET_FLOAT_MAX_EXP
            #define TARGET_FLOAT_MAX_EXP LDBL_MAX_EXP
        #endif

        /*	Define number of effective bits in mantissa
            This may include an implicit J-bit (i.e. for 1)
            in singles and short doubles. */
        #ifndef TARGET_FLOAT_MANT_DIG
            #define TARGET_FLOAT_MANT_DIG LDBL_MANT_DIG
        #endif

        /*	Define whether the one bit is implicit in the format. */
        #ifndef TARGET_FLOAT_IMPLICIT_J_BIT
            #if TARGET_FLOAT_BITS == 32
                #define TARGET_FLOAT_IMPLICIT_J_BIT 1
            #elif TARGET_FLOAT_BITS == 64
                #define TARGET_FLOAT_IMPLICIT_J_BIT 1
            #elif TARGET_FLOAT_BITS == 80
                #define TARGET_FLOAT_IMPLICIT_J_BIT 0 /* for x86 */
            #else
                #error define TARGET_FLOAT_IMPLICIT_J_BIT
            #endif
        #endif

        /*	Derive values sizes */
        #define TARGET_FLOAT_MANT_BITS (TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
        #define TARGET_FLOAT_EXP_BITS  (TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS - 1)

enum scan_states
{
    start              = 0x0001,
    sig_start          = 0x0002,
    leading_sig_zeroes = 0x0004,
    int_digit_loop     = 0x0008,
    frac_start         = 0x0010,
    frac_digit_loop    = 0x0020,
    sig_end            = 0x0040,
    exp_start          = 0x0080,
    leading_exp_digit  = 0x0100,
    leading_exp_zeroes = 0x0200,
    exp_digit_loop     = 0x0400,
    finished           = 0x0800,
    failure            = 0x1000,
    nan_state          = 0x2000,           /*- mm 990921 -*/
    infin_state        = 0x4000,           /*- mm 990921 -*/
    hex_state          = 0x8000            /*- mm 990921 -*/
};

enum hex_scan_states
{
    not_hex                = 0x0000,
    hex_start              = 0x0001,
    hex_leading_sig_zeroes = 0x0002,
    hex_int_digit_loop     = 0x0004,
    hex_frac_digit_loop    = 0x0008,
    hex_sig_end            = 0x0010,
    hex_exp_start          = 0x0020,
    hex_leading_exp_digit  = 0x0040,
    hex_leading_exp_zeroes = 0x0080,
    hex_exp_digit_loop     = 0x0100
};

        #define MAX_SIG_DIG             20 /*- mm 970609 -*/

        #define final_state(scan_state) (scan_state & (finished | failure))

        #define success(scan_state)                                                            \
            (scan_state & (leading_sig_zeroes | int_digit_loop | frac_digit_loop |             \
                           leading_exp_zeroes | exp_digit_loop | finished))

        /* for hex, we can't accept "0x" */
        #define hex_success(count, scan_state)                                                 \
            (count - 1 > 2 &&                                                                  \
             scan_state & (hex_leading_sig_zeroes | hex_int_digit_loop | hex_frac_digit_loop | \
                           hex_leading_exp_zeroes | hex_exp_digit_loop))

        #define fetch()    (count++, (*wReadProc) (wReadProcArg, 0, __GetAwChar)) /*- mm 990311 \
                                                                                     -*/
        #define unfetch(c) (*wReadProc) (wReadProcArg, c, __UngetAwChar) /*- mm 990311 -*/

long double _MSL_CDECL
__wcstold (int    max_width,
           wint_t (*wReadProc) (void*, wint_t, int),                     /*- mm 990311 -*/
           void*  wReadProcArg,                                          /*- mm 990311 -*/
           int*   chars_scanned,
           int*   overflow)
{
    int     scan_state     = start;
    int     hex_scan_state = not_hex;
    int     count          = 0;
    int     spaces         = 0;                                          /*- mm 970708 -*/
    wchar_t c;
    decimal d = {
        0,
        0,
        0,
        { 0, "" }
    };
    int         sig_negative = 0;
    int         exp_negative = 0;
    long        exp_value    = 0;
    int         exp_adjust   = 0;
    long double result;
    int         sign_detected = 0;                                       /*- mm 990921 -*/

        #if _MSL_C_LOCALE_ONLY
    wchar_t radix_marker = L'.';
        #else
    wchar_t radix_marker =
        (wchar_t)(*(unsigned char*)_MSL_LOCALDATA (__lconv).decimal_point); /*- mm 011130 -*/
        #endif

    /* hex float scanner variables */                                    /*- ejs 020924 -*/

    unsigned char* chptr;
    unsigned char  mantissa[TARGET_FLOAT_BYTES];                         /* big-endian layout */
    unsigned       mantissa_digits;
    unsigned long  exponent = 0;

    int           ui;
    unsigned char uch, uch1;
    int           NibbleIndex;
    int           expsign    = 0;
    int           exp_digits = 0;
    unsigned      intdigits  = 0;

    *overflow                = 0;
    c                        = fetch();

    while (count <= max_width && c != EOF && !final_state (scan_state))
    {
        switch (scan_state)
        {
            case start:
                if (iswspace (c))
                {
                    c = fetch();
                    count--;                                             /*- mani 970101 -*/
                    spaces++;                                            /*- mani 970101 -*/
                    break;
                }

                                                                         /*- mm 990921 -*/
                switch (towupper (c))
                {
                    case L'-':
                        sig_negative = 1;

                    case L'+':
                        c             = fetch();
                        /*scan_state = sig_start;*/
                        sign_detected = 1;
                        break;

        #if _MSL_C99                                                  /*- mm 030625 -*/
                    case 'I':
                        c          = fetch();
                        scan_state = infin_state;
                        break;

                    case 'N':
                        c          = fetch();
                        scan_state = nan_state;
                        break;
        #endif /* _MSL_C99 */                                         /*- mm 030625 -*/

                    default:
                        scan_state = sig_start;
                        break;
                }
                break;
        #if _MSL_C99                                                  /*- mm 030625 -*/
            case infin_state:
                {
                    int  i       = 1;
                    char model[] = "INFINITY";
                    while ((i < 8) && (towupper (c) == model[i]))
                    {
                        i++;
                        c = fetch();
                    }
                    if ((i == 3) || (i == 8))
                    {
                        if (sig_negative)
                            result = -INFINITY;
                        else
                            result = INFINITY;
                        *chars_scanned = spaces + i + sign_detected;
                        return (result);
                    }
                    else
                        scan_state = failure;
                    break;
                }

            case nan_state:
                {
                    int  i = 1, j = 0;
                    char model[]     = "NAN(";
                    char nan_arg[32] = "";
                    while ((i < 4) && (toupper (c) == model[i]))
                    {
                        i++;
                        c = fetch();
                    }
                    if ((i == 3) || (i == 4))
                    {
                        if (i == 4)
                        {
                            while ((j < 32) && (isdigit (c) || isalpha (c) ||
                                                (c == radix_marker))) /*- mm 021003 -*/
                            {
                                nan_arg[j++] = c;
                                c            = fetch();
                            }
                            if (c != L')')
                            {
                                scan_state = failure;
                                break;
                            }
                            else
                                j++;
                        }
                        nan_arg[j] = 'L\0';

                        if (sig_negative)
                            result = -nan (nan_arg);
                        else
                            result = nan (nan_arg);
                        *chars_scanned = spaces + i + j + sign_detected;
                        return (result);
                    }
                    else
                        scan_state = failure;
                    break;
                }
                        /*- mm 990921 -*/
        #endif /* _MSL_C99 */ /*- mm 030625 -*/

                              /*if (c == L'+')
                                  c = fetch();
                              else if (c == L'-')
                              {
                                  c = fetch();
              
                                  sig_negative = 1;
                              }
              
                              scan_state = sig_start;
              
                              break;
                          */
            case sig_start:
                if (c == radix_marker)             /*- mm 011130 -*/
                {
                    scan_state = frac_start;
                    c          = fetch();
                    break;
                }
                if (!iswdigit (c))
                {
                    scan_state = failure;
                    break;
                }
                if (c == L'0')
                {
                    c = fetch();
                    if (towupper (c) == L'X')
                    {
                        scan_state     = hex_state;
                        hex_scan_state = hex_start;
                    }
                    else
                        scan_state = leading_sig_zeroes;
                    break;
                }
                scan_state = int_digit_loop;
                break;

            case leading_sig_zeroes:

                if (c == L'0')
                {
                    c = fetch();

                    break;
                }
                scan_state = int_digit_loop;
                break;

            case int_digit_loop:
                if (!iswdigit (c))
                {
                    if (c == radix_marker)         /*- mm 011130 -*/
                    {
                        scan_state = frac_digit_loop;
                        c          = fetch();
                    }
                    else
                        scan_state = sig_end;
                    break;
                }
                if (d.sig.length < MAX_SIG_DIG)    /*- mm 970609 -*/
                    d.sig.text[d.sig.length++] = c;
                else
                    exp_adjust++;
                c = fetch();
                break;

            case frac_start:
                if (!iswdigit (c))
                {
                    scan_state = failure;
                    break;
                }
                scan_state = frac_digit_loop;
                break;

            case frac_digit_loop:
                if (!iswdigit (c))
                {
                    scan_state = sig_end;
                    break;
                }
                if (d.sig.length < MAX_SIG_DIG)    /*- mm 970609 -*/
                {
                    if (c != L'0' || d.sig.length) /* __dec2num doesn't like leading zeroes*/
                        d.sig.text[d.sig.length++] = c;
                    exp_adjust--;
                }
                c = fetch();
                break;

            case sig_end:
                if (c == L'E' || c == L'e')
                {
                    scan_state = exp_start;
                    c          = fetch();
                    break;
                }
                scan_state = finished;
                break;

            case exp_start:
                if (c == L'+')
                    c = fetch();
                else if (c == L'-')
                {
                    c            = fetch();
                    exp_negative = 1;
                }
                scan_state = leading_exp_digit;
                break;

            case leading_exp_digit:
                if (!iswdigit (c))
                {
                    scan_state = failure;
                    break;
                }
                if (c == L'0')
                {
                    scan_state = leading_exp_zeroes;
                    c          = fetch();
                    break;
                }
                scan_state = exp_digit_loop;
                break;

            case leading_exp_zeroes:
                if (c == L'0')
                {
                    c = fetch();
                    break;
                }
                scan_state = exp_digit_loop;
                break;

            case exp_digit_loop:
                if (!iswdigit (c))
                {
                    scan_state = finished;
                    break;
                }
                exp_value = exp_value * 10 + (c - '0');
                if (exp_value > SHRT_MAX)
                    *overflow = 1;
                c = fetch();
                break;

            /*- mm 990921 -*/
            case hex_state:
                {
                    switch (hex_scan_state)
                    {
                        case hex_start:
                            memset (mantissa, 0, sizeof (mantissa));
                            chptr           = mantissa;
                            mantissa_digits = (LDBL_MANT_DIG + 3) / 4;
                            intdigits       = 0;
                            NibbleIndex     = 0;
                            hex_scan_state  = hex_leading_sig_zeroes;
                            c               = fetch();
                            break;

                        case hex_leading_sig_zeroes:
                            if (c == L'0')
                            {
                                c = fetch();
                                break;
                            }
                            hex_scan_state = hex_int_digit_loop;

                            break;

                        case hex_int_digit_loop:
                            if (!iswxdigit (c))
                            { /* DETECT DECIMAL POINT, CHANGE SCAN STATE */
                                if (c == radix_marker) /*- mm 011130 -*/
                                {
                                    hex_scan_state = hex_frac_digit_loop;
                                    c              = fetch();
                                }
                                /* DETECT END OF INT PART OF HEX #, CHANGE SCAN STATE  # */
                                else
                                    hex_scan_state = hex_sig_end;
                                break;
                            }

                            /* CONVERT ASCII # (0,1,2 ... A,B,C,D,E,F) TO NUMERICAL EQUIVALENT
                             */
                            if (intdigits < mantissa_digits)
                            {
                                intdigits++;
                                uch = *(chptr + NibbleIndex / 2);
                                /* GET ASCII #, STRIP OFF ASCII PART */
                                ui  = towupper (c);
                                if (ui >= L'A')
                                    ui = ui - 'A' + 10;
                                else
                                    ui -= L'0';
                                uch1 = (unsigned char)ui;
                                /* CONFIGURE # APPROPRIATELY BY MAINTAINING MSB*/
                                if ((NibbleIndex % 2) != 0)
                                    uch |= uch1;
                                else
                                    uch |= (unsigned char)(uch1 << 4);
                                /* ASSSIGN CORRECTLY CONFIGURED NUMBER TO RESULT, GET NEW ASCII
                                 * NUMBER */
                                *(chptr + NibbleIndex++ / 2) = uch;
                                c                            = fetch();
                            }
                            /* # HAS NO NUMERICAL EQUIVALENT */
                            else
                                c = fetch(); /* ignore extra hexadecimal digits */
                            break;

                        case hex_frac_digit_loop:
                            if (!isxdigit (c))
                            { /* DETECT END OF FRAC PART OF HEX #, CHANGE SCAN STATE */
                                hex_scan_state = hex_sig_end;
                                break;
                            }
                            /* CONVERT ASCII # (0,1,2 ... A,B,C,D,E,F) TO NUMERICAL EQUIVALENT
                             */
                            if (intdigits < mantissa_digits)
                            {
                                uch = *(chptr + NibbleIndex / 2);
                                ui  = towupper (c);
                                /* GET ASCII #, STRIP OFF ASCII PART */
                                if (ui >= L'A')
                                    ui = ui - L'A' + 10;
                                else
                                    ui -= L'0';
                                /* CONFIGURE # APPROPRIATELY BY MAINTAINING MSB*/
                                uch1 = (unsigned char)ui;
                                if ((NibbleIndex % 2) != 0)
                                    uch |= uch1;
                                else
                                    uch |= (unsigned char)(uch1 << 4);
                                /* ASSSIGN CORRECTLY CONFIGURED NUMBER TO RESULT, GET NEW ASCII
                                 * NUMBER */
                                *(chptr + NibbleIndex++ / 2) = uch;
                                c                            = fetch();
                            }
                            else
                                c = fetch(); /* ignore extra hexadecimal digits */
                            break;

                        /* LOOK FOR EXPONENT */
                        case hex_sig_end:
                            if (towupper (c) == L'P')
                            {
                                hex_scan_state = hex_exp_start;
                                exp_digits++;
                                c = fetch();
                            }
                            else
                                scan_state = finished;
                            break;

                        case hex_exp_start:
                            exp_digits++;
                            if (c == '-')
                                expsign = 1;
                            else if (c != '+')
                            {
                                c = unfetch (c);
                                count--;
                                exp_digits--;
                            }
                            hex_scan_state = hex_leading_exp_digit;
                            c              = fetch();
                            break;

                        case hex_leading_exp_digit:

                            if (!isdigit (c))
                            {
                                scan_state = failure;
                                break;
                            }
                            if (c == L'0')
                            {
                                exp_digits++;
                                hex_scan_state = hex_leading_exp_zeroes;
                                c              = fetch();
                                break;
                            }
                            hex_scan_state = hex_exp_digit_loop;
                            break;

                        case hex_leading_exp_zeroes:
                            if (c == L'0')
                            {
                                c = fetch();
                                break;
                            }
                            hex_scan_state = hex_exp_digit_loop;
                            break;

                        case hex_exp_digit_loop:
                            if (!iswdigit (c))
                            {
                                scan_state = finished;
                                break;
                            }

                            exp_value = exp_value * 10 + (c - L'0');
                            if (exp_value > SHRT_MAX)
                                *overflow = 1;
                            exp_digits++;
                            c = fetch();
                            break;
                    }
                }
                /*- mm 990921 -*/
                break;
        }
    }

    if (scan_state != hex_state ? !success (scan_state)
                                : !hex_success (count, hex_scan_state)) /*- ejs 020924 -*/
    {
        count          = 0;                                             /*- mf 092497 -*/
        *chars_scanned = 0;
    }
    else
    {
        count--;
        *chars_scanned = count + spaces;                                /*- mani 970101 -*/
    }

    unfetch (c);
    /* CALCULATE RESULT -  The input was in decimal */
    if (hex_scan_state == not_hex)
    {
        if (exp_negative)
            exp_value = -exp_value;

        {
            int            n = d.sig.length;
            unsigned char* p = &d.sig.text[n];

            while (n-- && *--p == '0') exp_adjust++;

            d.sig.length = n + 1;

            if (d.sig.length == 0)
                d.sig.text[d.sig.length++] = '0';
        }

        exp_value += exp_adjust;

        if (exp_value < SHRT_MIN || exp_value > SHRT_MAX)
            *overflow = 1;

        if (*overflow)
            if (exp_negative)
                return (0.0);
            else
                return (sig_negative ? -HUGE_VAL : HUGE_VAL);

        d.exp  = exp_value;

        result = __dec2num (&d);

        /*
         *	Note: If you look at <ansi_fp.h> you'll see that __dec2num only supports double.
         *				If you look at <float.h> you'll see that long double == double. Ergo,
         *the difference is moot *until* a truly long double type is supported.
         */

        if (result != 0.0 && result < LDBL_MIN)
        {
            *overflow = 1;
            /*	result    = 0.0; */
        }
        else if (result > LDBL_MAX)
        {
            *overflow = 1;
            result    = HUGE_VAL;
        }

        if ((sig_negative) && success (scan_state))
            result = -result;

        return (result);
    }
    else
    {                                                                /* INPUT WAS IN HEX */
        unsigned       mantissa_bit, dbl_bit;
        unsigned       one_bit;
        long double    dbl_bits_storage;                             /*- cc 040625 -*/
        unsigned char* dbl_bits = (unsigned char*)&dbl_bits_storage; /*- cc 040625 -*/

        /* get exponent sign */
        if (expsign)
            exponent = -exponent;

        /* count the significant digits */
        exponent += intdigits * 4;

        /* FIND LEADING ONE BIT */
        /* we've skipped leading zeroes above, so this should be quick */
        one_bit   = 0;
        while (one_bit < 4 && !(mantissa[0] & (0x80 >> one_bit)))
        {
            one_bit++;
            exponent--;
        }
        exponent--;

        /* NORMALIZE MANTISSA
        --	we represent a value between 1 and 2.
        --	the leading 1 bit is implied for IEEE 754 32- and 64-bit doubles.
        -- 	shift the bits up into the big-endian mantissa[]
        */

        /* lose the leading one bit */
        if (TARGET_FLOAT_IMPLICIT_J_BIT)
        {
            one_bit++;
        }

        /* one_bit should NEVER be >= 8 */
        if (one_bit)
        {
            unsigned char carry = 0;
            for (chptr = mantissa + sizeof (mantissa) - 1; chptr >= mantissa; chptr--)
            {
                unsigned char a = *chptr;
                *chptr          = (unsigned char)((a << one_bit) | carry);
                carry           = (unsigned char)(a >> (8 - one_bit));
            }
        }

        /* PACK BITS INTO (BIG-ENDIAN) DOUBLE */
        memset (dbl_bits, 0, sizeof (dbl_bits_storage));  /*- cc 040625 -*/
        dbl_bit =
            (TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS); /* point to mantissa inside double */

        /* pack eight bits at a time */
        for (mantissa_bit = 0; mantissa_bit < TARGET_FLOAT_MANT_BITS; mantissa_bit += 8)
        {
            unsigned char ui = mantissa[mantissa_bit >> 3];
            int           halfbits;

            /* clip off low bits for end of mantissa
                !!! detect this for compiler? */
            if (mantissa_bit + 8 > TARGET_FLOAT_MANT_BITS)
                ui &= 0xff << (TARGET_FLOAT_MANT_BITS - mantissa_bit);

            /* pack bits into representation */
            halfbits                = (dbl_bit & 7);
            dbl_bits[dbl_bit >> 3] |= (unsigned char)(ui >> halfbits);

            dbl_bit                += 8;
            dbl_bits[dbl_bit >> 3] |= (unsigned char)(ui << (8 - halfbits));
        }

        /* GET EXPONENT */

        /* exponent is biased by LDBL_MAX_EXP so p0 = LDBL_MAX_EXP, p1=...+1, p-1=...-1 */
        exponent += TARGET_FLOAT_MAX_EXP - 1 + exp_value;

        /* test range */
        if ((exponent & ~(TARGET_FLOAT_MAX_EXP * 2 - 1)))
        {
            *overflow = 1;
            return 0.0;
        }

        #if TARGET_FLOAT_EXP_BITS > 32
            #error fix for greater-than-32-bit exponents
        #endif

        /* shift significant bits up */
        exponent    <<= 32 - TARGET_FLOAT_EXP_BITS;

        /* the exponent lives past the sign bit (i.e. at bit 1) */
        dbl_bits[0]  |= exponent >> 25;
        if (TARGET_FLOAT_EXP_BITS > 7)
            dbl_bits[1] |= exponent >> 17;
        if (TARGET_FLOAT_EXP_BITS > 15)
            dbl_bits[2] |= exponent >> 9;
        if (TARGET_FLOAT_EXP_BITS > 23)
            dbl_bits[3] |= exponent >> 1;

        /* IF SIGNIFICAND IS NEGATIVE */
        if (sig_negative)
            dbl_bits[0] |= 0x80;

        #if _MSL_LITTLE_ENDIAN
        /* CONVERT TO LITTLE-ENDIAN */
        {
            int            i;
            unsigned char* c = dbl_bits;
            unsigned char  temp;
            for (i = 0; i < TARGET_FLOAT_BYTES / 2; i++)
            {
                temp                          = c[i];
                c[i]                          = c[TARGET_FLOAT_BYTES - 1 - i];
                c[TARGET_FLOAT_BYTES - 1 - i] = temp;
            }
        }
        #endif

        result = *(long double*)dbl_bits;

        return result;
    }
}

double _MSL_CDECL
wcstod (const wchar_t* _MSL_RESTRICT str, wchar_t** _MSL_RESTRICT end)
{
    long double  value, abs_value;
    int          count, overflow;
    __wInStrCtrl wisc;
    wisc.wNextChar         = (wchar_t*)str;
    wisc.wNullCharDetected = 0;

    value = __wcstold (INT_MAX, &__wStringRead, (void*)&wisc, &count, &overflow);

    if (end)
        *end = (wchar_t*)str + count;

    abs_value = fabs (value);

    if (overflow || (value != 0.0 && (abs_value < DBL_MIN || abs_value > DBL_MAX)))
        errno = ERANGE;

    return (value);
}

        #if _MSL_C99 /*- mm 030304 -*/
long double _MSL_CDECL
wcstold (const wchar_t* _MSL_RESTRICT str, wchar_t** _MSL_RESTRICT end)
{
    long double  value, abs_value;
    int          count, overflow;
    __wInStrCtrl wisc;
    wisc.wNextChar         = (wchar_t*)str;
    wisc.wNullCharDetected = 0;

    value = __wcstold (INT_MAX, &__wStringRead, (void*)&wisc, &count, &overflow);

    if (end)
        *end = (wchar_t*)str + count;

    abs_value = fabs (value);

    if (overflow || (/*value != 0.0 && */ (abs_value < DBL_MIN || abs_value > DBL_MAX)))
        errno = ERANGE;

    return (value);
}

float _MSL_CDECL
wcstof (const wchar_t* _MSL_RESTRICT str, wchar_t** _MSL_RESTRICT end)
{
    long double  value, abs_value;
    int          count, overflow;
    __wInStrCtrl wisc;
    wisc.wNextChar         = (wchar_t*)str;
    wisc.wNullCharDetected = 0;

    value = __wcstold (INT_MAX, &__wStringRead, (void*)&wisc, &count, &overflow);

    if (end)
        *end = (wchar_t*)str + count;

    abs_value = fabs (value);

    if (overflow || (value != 0.0 && (abs_value < DBL_MIN || abs_value > DBL_MAX)))
        errno = ERANGE;

    return ((float)value);
}

        /*
        double _MSL_CDECL watof(const wchar_t * str)
        {
            return(wcstod(str, NULL));
        } */

        #endif /* _MSL_C99 */  /*- mm 030304 -*/
    #endif /* _MSL_WIDE_CHAR*/ /*- mm 981020 -*/
#endif                         /* _MSL_FLOATING_POINT && !defined(_MSL_NO_MATH_LIB) */

                               /* Change record:
                                * JFH 950622 First code release.
                                * JFH 950727 Removed stray SysBreak(). Added code to make use of the remembered sign of
                                *			  of the significand.
                                * JFH 950929 Discovered __dec2num doesn't like leading zeroes except for zeroes, so numbers
                                *			  like .01 would get interpreted as zero. Fixed by suppressing leading zeroes.
                                * JFH 951114 Fixed bug in wcstod where value was checked against DBL_MIN and DBL_MAX instead
                                *			  of the absolute value.
                                * JFH 960425 Changed __wcstold to return -HUGE_VAL instead of HUGE_VAL on overflow if a
                                *			  minus sign was previously detected.
                                * mani970101 Fix a scanf bug dealing with white space. Things like
                                *			  scanf("%5lx") weren't working properly when there was
                                *			  white space.
                                * mm  970609 Changed the max number of significant digits to MAX_SIG_DIG(==32) instead of
                                *DBL_DIG                                mm  970708 Inserted Be changes                                mf  970924 If there are no digits in the string then
                                *the value of &endp must remain unchanged                                In this case the variable chars_scanned of wcstold
                                *should be 0                                JCM 980121 First wchar release.                                mm  980206 Added cide to handle wide char unget
                                *properly and implementation of fwide.                                blc 980324 Fixed prototypes to match latest C9X standard
                                *(wchar_t** as                                second parameter)                                mm  981020 Added #if _MSL_WIDE_CHAR wrappers                                mm  990315
                                *Corrected encoding of dot for wide characters IL9903_1178                                mm  990325 Changes to separate char
                                *input from file i/o                                mm  990817 Deleted include of <string_io.h>                                mm  010503 Added code for
                                *thread local storage for lconv                                mm  010521 Added _MWMT wrappers                                cc  010531 Added
                                *_GetThreadLocalData's flag                                mm  011130 Changes for implementation of locale                                cc  011203 Added
                                *_MSL_CDECL for new name mangling                                blc 011211 Added _MSL_NO_MATH_LIB macro check                                JWW 020130
                                *Changed _MWMT to _MSL_THREADSAFE for consistency's sake                                JWW 020604 Added _MSL_LOCALDATA macro
                                *to make multithread code easier to read                                JWW 020917 Added _MSL_RESTRICT to get the restrict
                                *type specifier for certain C99 functions                                cc  021022 Added _MSL_C_LOCALE_ONLY around the
                                *radix_marker                                JWW 021031 Made thread local data available for Mach-O                                mm  021112 Added wcstold()
                                *and wcstof().                                cc  021119 Moved watof from the C lib to extras                                JWW 030224 Changed
                                *__NO_WIDE_CHAR flag into the new more configurable _MSL_WIDE_CHAR                                mm  030304 Added _MSL_C99
                                *wrapper                                cc  040217 Changed _No_Floating_Point to _MSL_FLOATING_POINT
                                */
