/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : vbi_api.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2007/6/22
Last Modified :
Description   : 
Function List :
History       :
 ******************************************************************************/


#ifndef __VBI_API_H__
#define __VBI_API_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_vbi_errcode.h"
#include "hi_vbi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

HI_S32 HI_VBI_Open(HI_VOID);
HI_S32 HI_VBI_Close(HI_VOID);
HI_S32 HI_VBI_SetStreamSource(VBI_STREAM_SOURCE_E eSource);
HI_S32 HI_VBI_GetStreamSource(VBI_STREAM_SOURCE_E *pSource);
HI_S32 HI_VBI_SetVbiType(VBI_TYPE_E eFormat);
HI_S32 HI_VBI_GetVbiType(VBI_TYPE_E *pFormat);
HI_S32 HI_VBI_WriteStream(HI_U8 *Source, HI_U32 CopyLenth, HI_U32 *pCount);
HI_S32 HI_VBI_Start(HI_VOID);

HI_S32 HI_VBI_Stop(HI_VOID);
HI_S32 HI_VBI_GetStatus(VBI_STATUS_E *pStatus);
HI_S32 HI_VBI_EnableMacrovision(VBI_MACROVISION_CONFIG_S *pConfig);
HI_S32 HI_VBI_DisableMacrovision(HI_VOID);
HI_S32 HI_VBI_SetChannelId(HI_U32 ChannelId);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_API_H__ */

