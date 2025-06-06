/* MSL
 * Copyright ｩ 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2007/01/29 21:23:20 $
 * $Revision: 1.7.2.6 $
 */

/*
    SUMMARY: definitions for the following

        FECLEAREXCEPT - see C99 std for exact definitions of functions.
        FEGETEXCEPTFLAG
        FERAISEEXCEPT
        FESETEXCEPTFLAG
        FETESTEXCEPT

        FEGETROUND
        FESETROUND

        FEGETENV
        FEHOLDEXCEPT
        FESETENV
        FEUPDATEENV


  NAMING CONVENTIONS USED:

    register fenv_t    		ENV = *envp;
    register const fenv_t   *ENV_Ptr = envp;
    register fenv_t    		EXCEPTIONS = excepts;
    register fexcept_t		FLAG = *flagp;
    register fexcept_t 		*FLAG_Ptr = flagp;
    register int 			ROUND = round;
    register int 			mask;
    register float	   		FPSCR;
    register int 	   		FPSCR_Temp;
    register int 	   		FPSCR_Temp1;
    register int 	   		FPSCR_Temp2;
    double			   		FPSCR_Mem;
    register double    		*FPSCR_MemPtr = &FPSCR_Mem ;

*/
#include <stdio.h>

#if _MSL_C99

    #include <fenv.h>

    #define fieldmask 255 /* which bits in the FPSCR do you want to affect? 255 = all of them  \
                           */

/* DEFINE FUNCTIONS */

/*
FECLEAREXCEPT
DESCRIPTION: The feclearexcept function clears the supported
floating-point exceptions represented by it's argument.
*/
_MSL_IMP_EXP_C int
feclearexcept (int excepts)
{
    register fenv_t  EXCEPTIONS = excepts & FE_ALL_EXCEPT;
    register float   FPSCR;
    register int     FPSCR_Temp;
    double           FPSCR_Mem;
    register double* FPSCR_MemPtr = &FPSCR_Mem;
    excepts = (excepts & 0xFFFFFFC); /* ignore attempts 2 chng rounding mode */

    if (!EXCEPTIONS)
        return 0;

    asm
    {
		mffs  FPSCR /* load FPSCR in2 GPR */								
		stfd  FPSCR, 	  0(FPSCR_MemPtr)			
		lwz   FPSCR_Temp, 4(FPSCR_MemPtr)			
		andc  FPSCR_Temp, FPSCR_Temp,    EXCEPTIONS /* clear exceptions */
		stw   FPSCR_Temp, 4(FPSCR_MemPtr)				
		lfd   FPSCR,      0(FPSCR_MemPtr)			
		mtfsf fieldmask,FPSCR                           /* load back in2 FPSCR */
    }

    return (fetestexcept (EXCEPTIONS));
}

/*
FEGETEXCEPTFLAG
DESCRIPTION: The fegetexceptflag function stores an implementation-defined representation of the
states of the floating-point status flags indicated by the argument excepts in the object
pointed to by the argument flagp.
*/
_MSL_IMP_EXP_C int
fegetexceptflag (fexcept_t* flagp, int excepts)
{
    register fenv_t     EXCEPTIONS = excepts & FE_ALL_EXCEPT;
    register fexcept_t* FLAG_Ptr   = flagp;
    register float      FPSCR;
    register int        FPSCR_Temp1;
    register int        FPSCR_Temp2;
    register int        FPSCR_Temp3;
    double              FPSCR_Mem;
    register double*    FPSCR_MemPtr = &FPSCR_Mem;
    excepts                          = (excepts & 0xFFFFFFFC);

    asm
    {
		mffs FPSCR /* load FPSCR in2 GPR */			
		stfd FPSCR,       0(FPSCR_MemPtr)				
		lwz  FPSCR_Temp1, 4(FPSCR_MemPtr)				
		and  FPSCR_Temp2, FPSCR_Temp1, EXCEPTIONS /* get pertinent values of FPSCR */
		stw  FPSCR_Temp2, 0(FLAG_Ptr) /* load in2 obj pnted 2 by flagp */	
		
		lwz FPSCR_Temp3, 0(FLAG_Ptr)
    }

    return !(FPSCR_Temp3 == FPSCR_Temp2);
}

/*
FERAISEEXCEPT
DESCRIPTION: The feraiseexcept function raises the supported floating-point exceptions
represented by the argument. The order in which these floating-point exceptions are raised is
unspecified, except as stated in ISO/IEC 9899:1999 F.7.6. Whether the feraiseexcept function
additionally raises the "inexact" floating-point exception whenever it raise the "overflow" or
"underflow" floating-point exception is implementation-defined.
*/
_MSL_IMP_EXP_C int
feraiseexcept (int excepts)
{
    register fenv_t  EXCEPTIONS = excepts & FE_ALL_EXCEPT; /*MASK */
    register float   FPSCR;
    register int     FPSCR_Temp1;
    register int     FPSCR_Temp2;
    double           FPSCR_Mem;
    register double* FPSCR_MemPtr = &FPSCR_Mem;
    excepts                       = (excepts & 0xFFFFFFFC);

    asm
    {
		mffs  FPSCR /* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)			
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)			
		or    FPSCR_Temp2, FPSCR_Temp1,  EXCEPTIONS /* raise exceptions */	
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)			
		lfd   FPSCR,       0(FPSCR_MemPtr)			
		mtfsf fieldmask,   FPSCR                                                 /* load back in2 FPSCR */
    }

    /* we need to raise exceptions in stored in FPSCR_Temp2 */

    return 0;
}

/*
FESETEXCEPTFLAG
DESCRIPTION: The fesetexceptflag function sets the floating-point
status flags indicated by the argument excepts to the states stored
in the object pointed to by flagp.  The value of *flagp
shall have been set by a previous call to fegetexceptflag whose
2nd argument represented at least those floating-point exceptions
represented by the argument excepts.  This function does not raise
floating-point exceptions, but only set the state of the flags.
*/
_MSL_IMP_EXP_C int
fesetexceptflag (const fexcept_t* flagp, int excepts)
{
    register fenv_t    EXCEPTIONS = excepts & FE_ALL_EXCEPT;
        /* what exceptions to raise */      /* mask */
    register fexcept_t FLAG       = *flagp; /* what values to raise 2 */
    register int       mask;
    register float     FPSCR;
    register int       FPSCR_Temp1;
    register int       FPSCR_Temp2;
    register int       FPSCR_Temp3;

    double           FPSCR_Mem;
    register double* FPSCR_MemPtr = &FPSCR_Mem;
    excepts                       = (excepts & 0xFFFFFFFC);

    if (!EXCEPTIONS)
        return 0;

    asm
    {
		and   mask,        EXCEPTIONS,      FLAG	
		mffs  FPSCR /* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)			
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)			
		or 	  FPSCR_Temp2, FPSCR_Temp1, mask /* raise exceptions */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)			
		lfd   FPSCR,       0(FPSCR_MemPtr)			
		mtfsf fieldmask,   FPSCR /* load back in2 FPSCR */	

		mffs  FPSCR                          /* load FPSCR in2 GPR */			
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz FPSCR_Temp3, 4(FPSCR_MemPtr)
    }

    return !(FPSCR_Temp3 & mask == mask);
}

/*
FETESTEXCEPT
DESCRIPTION: The fetestexcept function determines which of a specified subset of the
floating-point exception flags are currently set.  The excepts argument specifies the
floating-point status flags to be queried.
*/
_MSL_IMP_EXP_C int
fetestexcept (int excepts)
{
    register fenv_t  EXCEPTIONS = excepts & FE_ALL_EXCEPT; /* mask*/
    register float   FPSCR;
    register int     FPSCR_Temp1;
    register int     FPSCR_Temp2;
    double           FPSCR_Mem;
    register double* FPSCR_MemPtr = &FPSCR_Mem;
    excepts                       = (excepts & 0xFFFFFFFC);

    asm
    {
		mffs FPSCR /* load FPSCR in2 GPR */
		stfd FPSCR,		  0(FPSCR_MemPtr)			
		lwz  FPSCR_Temp1, 4(FPSCR_MemPtr)			
		and  FPSCR_Temp2, FPSCR_Temp1,	 EXCEPTIONS                                            /* test exceptions */
    }
    return FPSCR_Temp2;                                    /* return result of test */
}

/*
FEGETROUND
DESCRIPTION: The fegetround function returns the value of the rounding direction macro
representing the current rounding direction or a negative number if there is no such rounding
macro or the current rounding direction is not determinable.
*/
_MSL_IMP_EXP_C int
fegetround (void)
{
    register int     ROUND;
    register int     mask = 0x4; /* only want bits 30 and 31 of FPSCR */
    register float   FPSCR;
    register int     FPSCR_Temp;
    double           FPSCR_Mem;
    register double* FPSCR_MemPtr = &FPSCR_Mem;

    asm
    {
		mffs FPSCR /* load FPSCR in2 GPR */
		stfd FPSCR,      0(FPSCR_MemPtr)			
		lwz  FPSCR_Temp, 4(FPSCR_MemPtr)			
		and  ROUND,      FPSCR_Temp,     mask                        /* get rounding mode */
    }
    return ROUND;                /* return rounding mode */
}

/*
FESETROUND
DESCRIPTION: fesetround function establishes the rounding direction represented by its argument
round.  If the argument is not not equal to the value of a rounding direction macro, the
rounding direction is not changed.
*/
_MSL_IMP_EXP_C int
fesetround (int excepts)
{
    if (excepts <= 0x4) /* verify argument is a rounding mode */
    {
        register fenv_t  EXCEPTIONS = excepts & FE_ALL_EXCEPT;
        register float   FPSCR;
        register int     FPSCR_Temp1;
        register int     FPSCR_Temp2;
        double           FPSCR_Mem;
        register double* FPSCR_MemPtr = &FPSCR_Mem;

        asm
        {
			mffs  FPSCR /* load FPSCR in2 GPR */
			stfd  FPSCR,       0(FPSCR_MemPtr)			
			lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)			
			or    FPSCR_Temp2, FPSCR_Temp1,EXCEPTIONS /* set rounding mode */	
			stw   FPSCR_Temp2, 4(FPSCR_MemPtr)			
			lfd   FPSCR,	   0(FPSCR_MemPtr)			
			mtfsf fieldmask,   FPSCR          /* load back in2 FPSCR */
        }
        return 0;
    }
    else                /* argument is not a rounding mode! return any # */
        return -1;
}

/*
FEGETENV
DESCRIPTION: The fegetenv function stores the current
floating-point environment in the object pointed to by flagp.
*/
_MSL_IMP_EXP_C int
fegetenv (fexcept_t* flagp)
{
    register fexcept_t* FLAG_Ptr = flagp;
    register float      FPSCR;
    register int        FPSCR_Temp;
    register int        FPSCR_Temp1;
    register fenv_t     EXCEPTIONS = FE_ALL_EXCEPT;
    double              FPSCR_Mem;
    register double*    FPSCR_MemPtr = &FPSCR_Mem;

    asm
    {
		mffs FPSCR										
		stfd FPSCR,    0(FPSCR_MemPtr)					
		lwz FPSCR_Temp,4(FPSCR_MemPtr)					
		and   FPSCR_Temp, FPSCR_Temp, EXCEPTIONS /* we need to mask with fe_all exept */					
		stw FPSCR_Temp,0(FLAG_Ptr) /* save in2 obj pointed 2 by flagp */
		
		lwz FPSCR_Temp1, 4(FLAG_Ptr)
    }

    return !(FPSCR_Temp1 == FPSCR_Temp);
}

/*
FEHOLDEXCEPT
DESCRIPTION: The feholdexcept function saves the current
floating-point environment in the object pointed to by envp,
clears the floating-point status flags, and then installs a
non-stop (continues on floating-point exceptions) mode,
if available, for all floating-point exceptions.
*/
_MSL_IMP_EXP_C int
feholdexcept (fenv_t* envp)
{
    register const fenv_t* ENV_Ptr = envp;
    register int           mask    = 0x4; /* clear everything except rounding mode */
    register float         FPSCR;
    register int           FPSCR_Temp;
    register int           FPSCR_Temp1;
    register int           FPSCR_Temp2;
    double                 FPSCR_Mem;
    register double*       FPSCR_MemPtr = &FPSCR_Mem;
    register fenv_t        EXCEPTIONS   = FE_ALL_EXCEPT;

    asm
    {
		mffs  FPSCR /* load FPSCR in2 GPR */						
		stfd  FPSCR,       0(FPSCR_MemPtr)				
		lwz   FPSCR_Temp,  4(FPSCR_MemPtr)				
		and   FPSCR_Temp, FPSCR_Temp, EXCEPTIONS /* we need to mask with fe_all exept */					
		stw   FPSCR_Temp,  0(ENV_Ptr) /* save FPSCR to obj pnted 2 by envp */		
		and   FPSCR_Temp1, FPSCR_Temp, mask /* clear fp environment bits in FPSCR */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)				
		lfd   FPSCR,       0(FPSCR_MemPtr)				
		mtfsf fieldmask,   FPSCR                                /* load into FPSCR */
    }
    return -1;                            /* "non-stop" was not installed */
}

/*
FESETENV
DESCRIPTION: The fesetenv function establishes the floating-point
environment represented by the oject pointed to by envp.  The argument
envp shall point to an object set by a call to fegetenv or feholdenv,
or equal a floating-point macro.  Note that fesetenv merely installs
the state of the floating-point status flags represented through its
argument, and does not raise these floating-point exceptions.
*/
_MSL_IMP_EXP_C int
fesetenv (const fenv_t* envp)
{
    register fenv_t  ENV = *envp & FE_ALL_EXCEPT;
    register float   FPSCR;
    double           FPSCR_Mem;
    register double* FPSCR_MemPtr = &FPSCR_Mem;
    register fenv_t  EXCEPTIONS   = 	‾FE_ALL_EXCEPT;
    register int     FPSCR_Temp;
    register int     FPSCR_Temp1;
    register int     FPSCR_Temp2;

    asm
    {
		mffs  FPSCR /* load FPSCR in2 GPR */			
		stfd  FPSCR,     0(FPSCR_MemPtr)					
		lwz FPSCR_Temp1,4(FPSCR_MemPtr) /* store whats important */
		and   FPSCR_Temp1, FPSCR_Temp1, EXCEPTIONS /* remove the FE exceptions */					
		or 	  FPSCR_Temp1, FPSCR_Temp, ENV
		stw   FPSCR_Temp1, 4(FPSCR_MemPtr)			
		lfd   FPSCR,     0(FPSCR_MemPtr)					
		mtfsf fieldmask, FPSCR /* load back into FPSCR */
		
		mffs  FPSCR /* reload FPSCR in2 GPR */			
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz   FPSCR_Temp2,  4(FPSCR_MemPtr) /* remove the FE exceptions */

    }
    return !(FPSCR_Temp1 == FPSCR_Temp2);
}

/*
FEUPDATEENV
DESCRIPTION: The feupdateenv function saves the currently raised
floating-point exceptions in its automatic storage, installs the
floating-point environment represented by the object pointed to
by envp, and then raises the saved floating-point exceptions.
The argument envp shall point to an object set by a call to feholdexcept
or fegetenv, or equal a floating-point macro.
*/

_MSL_IMP_EXP_C int
feupdateenv (const fenv_t* envp)
{
    register fenv_t        ENV     = *envp & FE_ALL_EXCEPT;
    register const fenv_t* ENV_Ptr = envp;
    register float         FPSCR;
    register int           FPSCR_Temp1;
    register int           FPSCR_Temp2;
    register int           FPSCR_Temp3;

    double           FPSCR_Mem;
    register double* FPSCR_MemPtr = &FPSCR_Mem;
    register fenv_t  EXCEPTIONS   = 	‾FE_ALL_EXCEPT;

    asm
    {
		mffs  FPSCR /* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)		
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr) /* store whats important */
		and FPSCR_Temp2, FPSCR_Temp1, EXCEPTIONS /* remove the FE exceptions */
		or    FPSCR_Temp2, FPSCR_Temp2,ENV /* update FPSCR */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)		
		lfd   FPSCR,       0(FPSCR_MemPtr)		
		mtfsf fieldmask,   FPSCR /* load into FPSCR */	

		mffs  FPSCR /* reload FPSCR in2 GPR */			
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz   FPSCR_Temp3,  4(FPSCR_MemPtr) /* store whats important */

                     /* we need to raise exceptions in stored in FPSCR_Temp1 */

    }
    return !(FPSCR_Temp3 == FPSCR_Temp2);
}
#endif /* _MSL_C99 */

/*  Change Record
 *	20020416  PMK  be sure that prototypes come before assignemnts.
 *  20040423  cc   Added C99 flag
 */
