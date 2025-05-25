#include "FSL_RSO_Support.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*voidfunctionptr) (void); /* ptr to function returning void */
__declspec (section ".init") extern voidfunctionptr _ctors[];
__declspec (section ".init") extern voidfunctionptr _dtors[];

#ifdef __cplusplus
}
#endif

void
ModuleConstructors (void)
{
    voidfunctionptr* constructor;

    /*
     *  call static initializers
     */
    for (constructor = _ctors; *constructor; constructor++) { (*constructor)(); }
}

void
ModuleDestructors (void)
{
    voidfunctionptr* destructor;

    /*
     *  call destructors
     */
    for (destructor = _dtors; *destructor; destructor++) { (*destructor)(); }
}
