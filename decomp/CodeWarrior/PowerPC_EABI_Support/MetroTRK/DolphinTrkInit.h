/****************************************************************************/
/*

FILE

    $Header:
/u/product_layouts/nintendo/Wii/PowerPC_EABI_Support/MetroTRK/DolphinTrkInit.h,v 1.1 2008/07/07
20:23:05 dfreeland Exp $

DESCRIPTION

    Header for Dolphin MetroTRK initialization API.

HISTORY


    $History: $

AUTHORS

    (c) 2000 Metrowerks Corporation
    All rights reserved.

*/
/****************************************************************************/
#ifndef __DOLPHIN_TRK_H__
#define __DOLPHIN_TRK_H__

#ifdef __cplusplus
extern "C"
{
#endif

void InitMetroTRK (void);
void EnableMetroTRKInterrupts (void);
void InitMetroTRK_BBA (void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __DOLPHIN_TRK_H__ */
