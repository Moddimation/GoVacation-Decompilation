#ifndef __FSL_RSO_SUPPORT_H__
#define __FSL_RSO_SUPPORT_H__

#ifdef __cplusplus
extern "C"
{
#endif

void ModuleConstructors (void);
void ModuleDestructors (void);
void _prolog ();
void _epilog ();
void _unresolved ();

/*
 * Specify MODULE_EXPORT for any functions you
 * want to export from your module. This guarantees
 * they will not be deadstripped from the module.
 */
#define MODULE_EXPORT __declspec (export)

#ifdef __cplusplus
}
#endif

#endif
