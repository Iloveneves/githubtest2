/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_osdgal.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/09/17
  Description   : 
  History       :
  1.Date        : 2008/09/17
    Author      : g00123327
    Modification: Created file
******************************************************************************/
#ifndef  __HI_OSDGAL_H__
#define  __HI_OSDGAL_H__

//#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef IN
#define IN                    /* 输入参数 */
#endif

#ifndef OUT
#define OUT                   /* 输出参数 */
#endif

#define HI_OSD_MAXWIDTH     720
#define HI_OSD_MAXHEIGHT    576

typedef enum hiOSDGAL_MODE_E
{
    OSDGAL_MODE_2BIT,         /* 2bits */
    OSDGAL_MODE_4BIT,         /* 4bits */
    OSDGAL_MODE_8BIT,         /* RGB888 */
    OSDGAL_MODE_16BIT,        /* ARGB */
    OSDGAL_MODE_32BIT,        /* ARGB */
    OSDGAL_MODE_BUTT
} HI_OSDGAL_MODE_E;

typedef enum hiOSDGAL_ENABLE_E
{
    OSDGAL_OFF,               /* 关闭OSD */
    OSDGAL_ON                 /* 开启OSD */
} HI_OSDGAL_ENABLE_E;

typedef enum hiOSDGAL_LAYER_E
{
    OSDGAL_LAYER1,            /* OSD图层1 */
    OSDGAL_LAYER2,            /* OSD图层2 */
    OSDGAL_LAYER_BUTT
} HI_OSDGAL_LAYER_E;

typedef struct hiOSDGAL_RECT_S
{
    HI_U32 u32x;              /* 图形起始位置 */
    HI_U32 u32y;
    HI_U32 u32Width;          /* 图像的宽度 */
    HI_U32 u32Height;         /* 图形的高度 */
} HI_OSDGAL_RECT_S;

typedef HI_OSDGAL_LAYER_E HI_OSDGAL_HANDLE;
typedef HI_OSDGAL_RECT_S  HI_OSDGAL_IMAGE_S;

/* ===================== 错误码定义 ===================== */
#define HI_OSDGAL_SUCCESS           HI_SUCCESS /*操作成功*/
#define HI_OSDGAL_FAILED            HI_FAILURE /*操作失败*/
#define HI_ERR_OSDGAL_INVARG        -2 /*错误的参数*/

#define HI_ERR_OSDGAL_NOTINIT       -3 /*没有初始化*/
#define HI_ERR_OSDGAL_NOPEN         -4 /*图层没有打开*/
#define HI_ERR_OSDGAL_INVSTATE      -5 /*状态错误*/
#define HI_ERR_OSDGAL_NOTSUPPORT    -6 /*比支持此操作*/




/* ===================== 函数定义 ===================== */

/* OSD初始化函数 */
HI_S32 OSDGAL_Init();

/* 打开OSD图层设备，返回图层句柄 */
HI_S32 OSDGAL_OpenOSDLayer(IN HI_OSDGAL_LAYER_E eOSDLayer,
                           OUT HI_OSDGAL_HANDLE *pHandle);

/* 关闭OSD图层设备 */
HI_S32 OSDGAL_CloseOSDLayer(IN HI_OSDGAL_HANDLE hHandle);

/* 获取图形显存 */
HI_S32 OSDGAL_GetSurfaceAddr(IN HI_OSDGAL_HANDLE hHandle,
				OUT HI_U32 * SurfaceBuffAddr);


/* 使能OSD图层 */
HI_S32 OSDGAL_Enable(IN HI_OSDGAL_HANDLE hHandle,
                     IN HI_OSDGAL_ENABLE_E eEnable);

/* OSD去初始化函数 */
HI_S32 OSDGAL_DeInit();
	   
HI_S32 OSDGAL_DrawPixel(IN HI_OSDGAL_HANDLE hHandle,
						IN HI_U32 u32X,
                        IN HI_U32 u32Y,
                        IN HI_U32 u32Color);

/* 绘制矩形 */
HI_S32 OSDGAL_DrawRectangle(IN HI_OSDGAL_HANDLE hHandle,
                            IN const HI_OSDGAL_RECT_S *pstruRectSize,
                            IN HI_U32 u32Color);

/* 绘制椭圆 */
HI_S32 OSDGAL_DrawEllipse(IN HI_OSDGAL_HANDLE hHandle,
                          IN const HI_OSDGAL_RECT_S *pstruEllipseSize,
                          IN HI_U32 u32Color);

/* 绘制线 */
HI_S32 OSDGAL_DrawLine(IN HI_OSDGAL_HANDLE hHandle,
                       IN HI_U32 u32StartX,
                       IN HI_U32 u32StartY,
                       IN HI_U32 u32EndX,
                       IN HI_U32 u32EndY,
                       IN HI_U32 u32Color);

/* 填充椭圆 */
HI_S32 OSDGAL_FillEllipse(IN HI_OSDGAL_HANDLE hHandle,
                          IN const HI_OSDGAL_RECT_S* pstruEllipseSize,
                          IN HI_U32 u32Color);

/* 填充矩形 */
HI_S32 OSDGAL_FillRectangle(IN HI_OSDGAL_HANDLE hHandle,
                            IN const HI_OSDGAL_RECT_S* pstruRectSize,
                            IN HI_U32 u32Color);

/* 设置颜色模式，参考HI_OSDGAL_MODE_E定义 */
HI_S32 OSDGAL_SetColorMode(IN HI_OSDGAL_HANDLE hHandle,
                           IN HI_OSDGAL_MODE_E eColorMode);

/* 获取颜色模式，参考HI_OSDGAL_MODE_E定义 */
HI_S32 OSDGAL_GetColorMode(IN HI_OSDGAL_HANDLE hHandle,
                           OUT HI_OSDGAL_MODE_E *peColorMode);

/* 读取指定矩形框内的数据 */
HI_S32 OSDGAL_ReadRectangleData(IN HI_OSDGAL_HANDLE hHandle,
                                IN const HI_OSDGAL_RECT_S *pstruRectSize,
                                IN HI_U32 u32DataLen,                                
                                IN OUT HI_U8 *pu8Data);

/* 往指定矩形框内写数据 */
HI_S32 OSDGAL_WriteRectangleData(IN HI_OSDGAL_HANDLE hHandle,
                                 IN const HI_OSDGAL_RECT_S *pstruRectSize,
                                 IN HI_U32 u32DataLen,                                
                                 IN OUT HI_U8 *pu8Data);

HI_S32 OSDGAL_WriteRectangleDataMask(IN HI_OSDGAL_HANDLE hHandle,
                                 IN const HI_OSDGAL_RECT_S *pstruRectSize,
                                 IN HI_U32 u32DataLen,
                                 IN OUT HI_U8 *pu8Data,
                                 IN HI_U32 maskcolor);

/* 获取某个像素的值 */
HI_S32 OSDGAL_GetPixel(IN HI_OSDGAL_HANDLE hHandle,
                       IN HI_U32 u32x,
                       IN HI_U32 u32y,
                       IN OUT HI_U8 *pu8Data);

/* 设置调色板 */
HI_S32 OSDGAL_SetColorCult(IN HI_OSDGAL_HANDLE hHandle,
                           IN HI_U32 u32CultCount,                            
                           IN HI_U32  u32Start,
                           IN HI_U16* pu16Red,
                           IN HI_U16* pu16Green,
                           IN HI_U16* pu16Blue,
                           IN HI_U16* pu16alph);

/* 设置调色板扩展接口 */
HI_S32 OSDGAL_SetColorCultEx(IN HI_OSDGAL_HANDLE hHandle,
                             IN HI_U32  u32CultCount,
                             IN HI_U32  u32Start,
                             IN HI_U32* pColor);


HI_S32 OSDGAL_SetAlpha(IN HI_OSDGAL_HANDLE hHandle,
                     HI_U8 u8value);


HI_S32 OSDGAL_Refresh(IN HI_OSDGAL_HANDLE hHandle,
	IN const HI_OSDGAL_RECT_S *pstruRectSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /*__HI_OSDGAL_H__*/


