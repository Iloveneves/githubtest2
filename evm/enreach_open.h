/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_open.h
* Description:
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#ifndef __ENREACH_OPEN_H__
#define __ENREACH_OPEN_H__

#include "hi_type.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern HI_U8* s_pu8Browser_buf;
extern HI_U8* s_pu8Browser_cach;
extern HI_U8* s_pu8JS_buf;


HI_S32 HI_StartBrowser(HI_U32 u32Freq, HI_U32 u32SymbolRate, HI_U32 u32Qam, HI_U32 u32Service_id);
HI_S32 HI_OpenBrowser(HI_U8 *pu8WebUrl);
HI_S32 HI_BrowserInput(HI_U32 u32Key);
HI_S32 HI_DestroyBrowser();

#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

