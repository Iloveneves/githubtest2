/******************************************************************************
  Copyright (C), 2004-2050, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : hi_unf_vo.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/1/22
  Last Modified :
  Description   : header file for audio and video output control
  Function List :
  History       :
  1.Date        :
  Author        : z67193
  Modification  : Created file
******************************************************************************/

#ifndef  __HI_UNF_SND_H__
#define  __HI_UNF_SND_H__

#include "hi_unf_common.h"
#include "hi_debug.h" 

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/********************************SND STRUCT********************************/



/******************SND API********************/

//typedef HI_VOID * HI_HANDLE ;

typedef struct  hiUNF_SND_INIT_PARA
{
    HI_U32          u32ID ;     //可以输入无效值，自由分配
    //功能选项
} HI_UNF_SND_INIT_PARA_S;



/********************************SND STRUCT********************************/
typedef enum hiUNF_SND_INTERFACE_E
{
    HI_UNF_SND_INTERFACE_I2S,
    HI_UNF_SND_INTERFACE_SPDIF,
    HI_UNF_SND_INTERFACE_BUTT
}HI_UNF_SND_INTERFACE_E;

typedef struct hiUNF_SND_INTERFACE_S
{
    HI_UNF_SND_INTERFACE_E enInterface;
    HI_BOOL bEnable;
/*union 用于指定类似spdif还需要指定是否bypass属性，以及后续新增接口类型的扩展*/

}HI_UNF_SND_INTERFACE_S;

/*合并为两个接口*/
HI_S32 HI_UNF_SND_Init();
HI_S32 HI_UNF_SND_DeInit();
HI_S32 HI_UNF_SND_Create( const HI_UNF_SND_INIT_PARA_S *pstPara  , HI_HANDLE  *pHandle);
HI_S32 HI_UNF_SND_Destroy(HI_HANDLE  handle );
HI_S32 HI_UNF_SND_SetInterface(HI_HANDLE  handle  , HI_UNF_SND_INTERFACE_S *pInterface);
HI_S32 HI_UNF_SND_GetInterface(HI_HANDLE  handle  , HI_UNF_SND_INTERFACE_S *pInterface);
HI_S32 HI_UNF_SND_SetAnalogMute(HI_HANDLE  handle  ,HI_BOOL bMute);
HI_S32 HI_UNF_SND_GetAnalogMute(HI_HANDLE  handle  ,HI_BOOL *pbMute);
HI_S32 HI_UNF_SND_SetDigitalMute(HI_HANDLE  handle  ,HI_BOOL bMute);
HI_S32 HI_UNF_SND_GetDigitalMute(HI_HANDLE  handle  ,HI_BOOL *pbMute);
HI_S32 HI_UNF_SND_SetVolume(HI_HANDLE  handle  ,HI_U32 u32Volume);
HI_S32 HI_UNF_SND_GetVolume(HI_HANDLE  handle  ,HI_U32 * pu32Volume);
HI_S32 HI_UNF_SND_SetSampleRate(HI_HANDLE  handle  ,HI_UNF_SAMPLE_RATE_E enSampleRate);
HI_S32 HI_UNF_SND_GetSampleRate(HI_HANDLE  handle  ,HI_UNF_SAMPLE_RATE_E * penSampleRate);
HI_S32 HI_UNF_SND_SetAutoResample(HI_HANDLE  handle  ,HI_BOOL bAutoResample);
HI_S32 HI_UNF_SND_GetAutoResample(HI_HANDLE  handle  ,HI_BOOL * pbAutoResample);
HI_S32 HI_UNF_SND_SetTrackMode(HI_HANDLE  handle  , HI_UNF_TRACK_MODE_E enMode);
HI_S32 HI_UNF_SND_GetTrackMode(HI_HANDLE  handle  , HI_UNF_TRACK_MODE_E * penMode);
HI_S32 HI_UNF_SND_Attach(HI_HANDLE  hSndhandle  , HI_HANDLE s32AvplayHandle,  HI_U32 u32MixWeight);
HI_S32 HI_UNF_SND_Detach(HI_HANDLE  hSndhandle  , HI_HANDLE s32AvplayHandle);
HI_S32 HI_UNF_SND_SetMixWeight(HI_HANDLE  hSndhandle  , HI_HANDLE s32AvplayHandle,  HI_U32 u32MixWeight);
HI_S32 HI_UNF_SND_GetMixWeight(HI_HANDLE  hSndhandle  , HI_HANDLE s32AvplayHandle,  HI_U32 *pu32MixWeight);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /*__HI_UNF_SND_H__*/

