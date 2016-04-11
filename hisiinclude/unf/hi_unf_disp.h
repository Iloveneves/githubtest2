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

#ifndef  __HI_UNF_DISP_H__
#define  __HI_UNF_DISP_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


/********************************DISPLAY STRUCT********************************/
#define MAX_DAC_NUM (4)

typedef enum hiUNF_OUTPUT_DAC_MODE_E
{
    HI_UNF_DAC_MODE_SILENCE = 0,
    HI_UNF_DAC_MODE_CVBS,
    HI_UNF_DAC_MODE_Y,
    HI_UNF_DAC_MODE_PB,
    HI_UNF_DAC_MODE_PR,
    HI_UNF_DAC_MODE_R,
    HI_UNF_DAC_MODE_G,
    HI_UNF_DAC_MODE_B,
    HI_UNF_DAC_MODE_SVIDEO_Y,
    HI_UNF_DAC_MODE_SVIDEO_C,
    HI_UNF_DAC_MODE_BUTT
}HI_UNF_OUTPUT_DAC_MODE_E;

typedef struct  hiUNF_OUTPUT_INTERFACE_S
{
    HI_UNF_OUTPUT_DAC_MODE_E         enDacMode[MAX_DAC_NUM];
}HI_UNF_OUTPUT_INTERFACE_S ;


typedef enum hiUNF_DISP_LAYER_E
{
    HI_UNF_DISP_LAYER_VIDEO_0    = 0x1,  /* 视频图象层 */
    HI_UNF_DISP_LAYER_OSD_0         =  0x2 , /* OSD 0层 */
    HI_UNF_DISP_LAYER_OSD_1         =  0x4 , /* OSD 1层 */
    HI_UNF_DISP_LAYER_OSD_2,    /* 硬件鼠标层 */

    HI_UNF_DISP_LAYER_BUTT
} HI_UNF_DISP_LAYER_E;

typedef enum hiUNF_COLOR_FMT_E
{
    HI_UNF_COLOR_FMT_RGB_444 = 0,
    HI_UNF_COLOR_FMT_RGB_555,
    HI_UNF_COLOR_FMT_RGB_565,
    HI_UNF_COLOR_FMT_RGB_888,
    HI_UNF_COLOR_FMT_ARGB_4444,
    HI_UNF_COLOR_FMT_ARGB_1555,
    HI_UNF_COLOR_FMT_ARGB_8565,
    HI_UNF_COLOR_FMT_ARGB_8888,
    HI_UNF_COLOR_FMT_YCbCr_888,
    HI_UNF_COLOR_FMT_AYCbCr_8888,
    HI_UNF_COLOR_FMT_YCbCr_422RASTER
}HI_UNF_COLOR_FMT_E;

typedef struct  hiUNF_BG_COLOR_S
{
    HI_U8 u8Red ;
    HI_U8 u8Green ;
    HI_U8 u8Blue ;
} HI_UNF_BG_COLOR_S;

/********************************ENCODER STRUCT********************************/

typedef enum hiUNF_MACROVISION_MODE_E
{
    HI_UNF_MACROVISION_MODE_TYPE0,  /* 典型配置0 */
    HI_UNF_MACROVISION_MODE_TYPE1,  /* 典型配置1 */
    HI_UNF_MACROVISION_MODE_TYPE2,  /* 典型配置2 */
    HI_UNF_MACROVISION_MODE_TYPE3,  /* 典型配置3 */
    HI_UNF_MACROVISION_MODE_CUSTOM, /* 用户自定义配置 */
    HI_UNF_MACROVISION_MODE_BUTT
} HI_UNF_MACROVISION_MODE_E;

typedef struct hiUNF_TTX_DATA_S
{
    HI_U8   *pu8DataAddr;               /*Ttx数据用户虚拟地址*/
    HI_U32   u32DataLen;                /*Ttx数据长度*/
} HI_UNF_TTX_DATA_S;

typedef struct hiUNF_VBI_DATA_S
{
    HI_U8  *pu8DataAddr;                 /*Vbi数据用户虚拟地址*/
    HI_U32  u32DataLen;                 /*Vbi数据长度*/
} HI_UNF_VBI_DATA_S;

typedef struct hiUNF_WSS_DATA_S
{
    HI_BOOL bEnable;
    HI_U16  u16Data;                /*Wss数据*/
} HI_UNF_WSS_DATA_S;

typedef struct  hiUNF_DISP_INIT_PARA
{
    HI_U32          u32ID ;     //可以输入无效值，自由分配
} HI_UNF_DISP_INIT_PARA_S;

/******************DISPLAY API********************/
HI_S32  HI_UNF_DISP_Init();
HI_S32  HI_UNF_DISP_DeInit();
HI_S32  HI_UNF_DISP_Create(const HI_UNF_DISP_INIT_PARA_S *pstPara, HI_HANDLE *pHandle);
HI_S32  HI_UNF_DISP_Destroy(HI_HANDLE  handle);
HI_S32  HI_UNF_DISP_GetLayerOrder(HI_HANDLE handle, HI_U32 *pu32Order, HI_U32 u32LayerNum);
HI_S32  HI_UNF_DISP_SetLayerOrder(HI_HANDLE handle , HI_U32 *pu32Order, HI_U32 u32LayerNum);
HI_S32  HI_UNF_DISP_SetBgColor(HI_HANDLE handle, HI_UNF_BG_COLOR_S *pstBgColor);
HI_S32  HI_UNF_DISP_GetBgColor(HI_HANDLE handle, HI_UNF_BG_COLOR_S *pstBgColor);
HI_S32  HI_UNF_DISP_SetBrightness(HI_HANDLE handle, HI_U32 u32Brightness);
HI_S32  HI_UNF_DISP_GetBrightness(HI_HANDLE handle, HI_U32 *pu32Brightness);
HI_S32  HI_UNF_DISP_SetContrast(HI_HANDLE handle, HI_U32 u32Contrast);
HI_S32  HI_UNF_DISP_GetContrast(HI_HANDLE handle, HI_U32 *pu32Contrast);
HI_S32  HI_UNF_DISP_SetSaturation(HI_HANDLE handle, HI_U32 u32Saturation);
HI_S32  HI_UNF_DISP_GetSaturation(HI_HANDLE handle, HI_U32 *pu32Saturation);
HI_S32  HI_UNF_DISP_SetHuePlus(HI_HANDLE handle, HI_U32 u32HuePlus);
HI_S32  HI_UNF_DISP_GetHuePlus(HI_HANDLE handle, HI_U32 *pu32HuePlus);

/******************ENCODER API********************/
HI_S32  HI_UNF_DISP_SetFormat(HI_HANDLE handle, HI_UNF_ENC_FMT_E enEncodingFormat);
HI_S32  HI_UNF_DISP_GetFormat(HI_HANDLE handle, HI_UNF_ENC_FMT_E * penEncodingFormat);
HI_S32  HI_UNF_DISP_SendTtxData(HI_HANDLE handle, HI_UNF_TTX_DATA_S *pstTtxData);
HI_S32  HI_UNF_DISP_SendVbiData(HI_HANDLE handle, HI_UNF_VBI_DATA_S *pstVbiData);
HI_S32  HI_UNF_DISP_SetWss(HI_HANDLE handle, HI_UNF_WSS_DATA_S *pstWssData);
HI_S32  HI_UNF_DISP_SetMacrovision(HI_HANDLE handle, HI_UNF_MACROVISION_MODE_E enMode, HI_VOID * pData);
HI_S32  HI_UNF_DISP_GetMacrovision(HI_HANDLE handle, HI_UNF_MACROVISION_MODE_E * penMode, HI_VOID * pData);
HI_S32  HI_UNF_DISP_SetDacMode(HI_HANDLE handle, HI_UNF_OUTPUT_INTERFACE_S *pstDacMode);
HI_S32  HI_UNF_DISP_GetDacMode(HI_HANDLE handle, HI_UNF_OUTPUT_INTERFACE_S *pstDacMode);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /*__HI_UNF_DISP_H__*/

