/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_oc.h
* Description:
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#ifndef __ENREACH_OC_H__
#define __ENREACH_OC_H__

#include "hi_type.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern HI_U8*    s_pu8Oc_addr;

HI_S32 HI_StartEnreachOC(HI_U32 u32Freq, HI_U32 u32Symbol, HI_U32 u32Qam, HI_U32 u32SerID);
HI_S32 OC_EnreachGetWebUrl(HI_U8 **ppszWebUrl);
void HI_DestroyOC(void);
void HI_CleanCallback(void);


#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

