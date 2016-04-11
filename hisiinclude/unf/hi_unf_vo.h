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
    Author      : z67193
    Modification: Created file
******************************************************************************/

#ifndef  __HI_UNF_VO_H__
#define  __HI_UNF_VO_H__

#include "hi_unf_common.h"
#include "hi_unf_disp.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

typedef HI_S32 VO_LAYER_HANDLE;

/********************************VO STRUCT********************************/


/* Video aspect mode */
typedef enum hiUNF_ASPECT_CVRS_E
{
    HI_UNF_ASPECT_CVRS_IGNORE = 0x0,
    HI_UNF_ASPECT_CVRS_LETTERBOX,
    HI_UNF_ASPECT_CVRS_PANANDSCAN,
    HI_UNF_ASPECT_CVRS_COMBINED,
    HI_UNF_ASPECT_CONVERSION_MODE_BUTT
} HI_UNF_ASPECT_CVRS_E;

typedef struct hiUNF_WINDOW_S
{
    HI_U32 u32X;
    HI_U32 u32Y;
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U32 u32BaseWidth;
    HI_U32 u32BaseHeight;
} HI_UNF_WINDOW_S;

typedef enum hiUNF_CAPTURE_TYPE_E
{
    HI_UNF_CAPTURE_TYPE_TOP_FIELD = 0x0,    /* 只捕获顶场          */
    HI_UNF_CAPTURE_TYPE_BOTTOM_FIELD,       /* 只捕获底场          */
    HI_UNF_CAPTURE_TYPE_FRAME,              /* 捕获一帧，含顶/底场 */
    HI_UNF_CAPTURE_TYPE_BUTT
} HI_UNF_CAPTURE_TYPE_E;

typedef struct hiUNF_CAPTURE_PICTURE_S
{
    HI_U32                u32StartAddr; /* 存放捕获图象的内存地址 */
    HI_U32                u32Width; /* 捕获图象的宽度 */
    HI_U32                u32Height; /* 捕获图象的高度 */
    HI_U32                u32Stride; /* 捕获图象的行距 */
    HI_UNF_COLOR_FMT_E    enCapColorFmt; /* 捕获截屏颜色格式 */
    HI_UNF_CAPTURE_TYPE_E enCapType;
    HI_S32                s32TimeOut; /* 图象捕获的超时值 */
} HI_UNF_CAPTURE_PICTURE_S;

/*帧的结构类型*/
typedef enum hiUNF_VO_DISP_DATA_MODE_E
{
    HI_UNF_VO_DISP_DATA_FRAME = 0x0,                   /*显示一帧数据*/
    HI_UNF_VO_DISP_DATA_FIELD_TOP,                     /*显示顶场数据*/
    HI_UNF_VO_DISP_DATA_FIELD_BOTTOM,                  /*显示底场数据*/
}HI_UNF_VO_DISP_DATA_MODE_E;

/*================ VIDEO OUTPUT API =================================*/
//typedef HI_VOID * HI_HANDLE ;

typedef struct  hiUNF_VO_INIT_PARA
{
    HI_U32          u32ID ;     //可以输入无效值，自由分配
    //功能选项
} HI_UNF_VO_INIT_PARA_S;

typedef enum hiUNF_VO_SOURCE_TYPE_E
{
    HI_UNF_VO_SOURCE_TYPE_AVPLAY = 0x0,                 
    HI_UNF_VO_SOURCE_TYPE_MANUAL ,
}HI_UNF_VO_SOURCE_TYPE_E;

typedef struct  hiUNF_VO_SOURCE_PARA
{
    HI_U32          u32StartX;
    HI_U32          u32StartY;
    HI_U32          u32Width;
    HI_U32          u32Heigth;
    HI_U32          u32Alpha;
} HI_UNF_VO_SOURCE_PARA_S;

typedef struct  hiHI_UNF_VO_ATTACH_PARA_S
{
    HI_UNF_VO_SOURCE_TYPE_E     eSourceType ;
    HI_UNF_VO_SOURCE_PARA_S     stSourcePara ;
} HI_UNF_VO_ATTACH_PARA_S;

HI_S32 HI_UNF_VO_Init();
HI_S32 HI_UNF_VO_DeInit();
HI_S32 HI_UNF_VO_CreateLayer(HI_UNF_VO_INIT_PARA_S *pPara, HI_HANDLE *pHandle);
HI_S32 HI_UNF_VO_DestroyLayer(HI_HANDLE Handle);
HI_S32 HI_UNF_VO_Attach(HI_HANDLE Handle, HI_HANDLE s32SrcHandle, HI_UNF_VO_ATTACH_PARA_S *pPara);
HI_S32 HI_UNF_VO_Detach(HI_HANDLE Handle, HI_HANDLE s32SrcHandle);
HI_S32 HI_UNF_VO_SetMixWeight(HI_HANDLE Handle, HI_HANDLE s32AvplayHandle, HI_UNF_VO_SOURCE_PARA_S *pPara);
HI_S32 HI_UNF_VO_GetMixWeight(HI_HANDLE Handle, HI_HANDLE s32AvplayHandle, HI_UNF_VO_SOURCE_PARA_S *pstPara);
HI_S32 HI_UNF_VO_SetEnable(HI_HANDLE Handle, HI_BOOL bEnable);
HI_S32 HI_UNF_VO_GetEnable(HI_HANDLE Handle, HI_BOOL* pbEnable);
HI_S32 HI_UNF_VO_SetAspectRatio(HI_HANDLE Handle, HI_UNF_ASPECT_RATIO_E enAspectRatio);
HI_S32 HI_UNF_VO_GetAspectRatio(HI_HANDLE Handle, HI_UNF_ASPECT_RATIO_E * penAspectRatio);
HI_S32 HI_UNF_VO_SetAspectCvrs(HI_HANDLE Handle, HI_UNF_ASPECT_CVRS_E enAspectCvrs);
HI_S32 HI_UNF_VO_GetAspectCvrs(HI_HANDLE Handle, HI_UNF_ASPECT_CVRS_E * penAspectCvrs);
HI_S32 HI_UNF_VO_SetInputWindow(HI_HANDLE Handle, HI_UNF_WINDOW_S *pstWindow);
HI_S32 HI_UNF_VO_GetInputWindow(HI_HANDLE Handle, HI_UNF_WINDOW_S * pstWindow);
HI_S32 HI_UNF_VO_SetOutputWindow(HI_HANDLE Handle, HI_UNF_WINDOW_S *pstWindow);
HI_S32 HI_UNF_VO_GetOutputWindow(HI_HANDLE Handle, HI_UNF_WINDOW_S * pstWindow);
HI_S32 HI_UNF_VO_GetRealOutputWindow(HI_HANDLE Handle, HI_UNF_WINDOW_S * pstWindow);
HI_S32 HI_UNF_VO_SetAlpha(HI_HANDLE Handle, HI_U32 u32Alpha);
HI_S32 HI_UNF_VO_GetAlpha(HI_HANDLE Handle, HI_U32 *pu32Alpha);
HI_S32 HI_UNF_VO_CapturePicture(HI_HANDLE Handle, HI_UNF_CAPTURE_PICTURE_S *pstCapPicture);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /*__HI_UNF_VO_H__*/
