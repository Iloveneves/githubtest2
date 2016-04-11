/*******************************************************************************
 *              Copyright 2008 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_vo_ioctl.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-01-22       z67193    HI_NULL      Create this file.
 ******************************************************************************/
#ifndef __VO_TYPE_H__
#define __VO_TYPE_H__

#include "hi_type.h"
#include "hi_tde_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/****************************************************************************
*                          VO  宏定义                                       *
****************************************************************************/
#define OSD_LAYER_NUM                   (2)
#define OSD0_USER_BUF_OFFSET            (0)
#define OSD1_USER_BUF_OFFSET            (0x10000)
#define VO_MAX_DAC_NUM (4)


/****************************************************************************
*                    VO  数据结构定义                                       *
****************************************************************************/

/****************************Public Structure Definition*******************/    

typedef HI_U32 OSD_REGION_HANDLE_T;

typedef enum hiVO_IRQ_TYPE_E
{
    VO_IRQ_FRAME = 0,
    VO_IRQ_TOP_FIELD,
    VO_IRQ_BOTTOM_FIELD,
    VO_IRQ_TOP_BOTTOM_FIELD,
    VO_IRQ_ALL
}VO_IRQ_TYPE_E;


/*矩形区域的结构体定义*/
typedef struct hiVO_RECTANGLE_S
{
    HI_U32                 u32StX;
    HI_U32                 u32StY;
    HI_U32                 u32Width;
    HI_U32                 u32Height;
}VO_RECTANGLE_S;

/*位置结构体定义*/
typedef struct hiVO_POSITION_S
{
    HI_U32                 u32StX;
    HI_U32                 u32StY;
}VO_POSITION_S;

/*窗口数据结构定义*/
typedef struct hiVO_WINDOW_S
{
    HI_U32                 u32StX;
    HI_U32                 u32StY;
    HI_U32                 u32Width;
    HI_U32                 u32Height;
    HI_U32                 u32BaseWidth;
    HI_U32                 u32BaseHeight;
}VO_WINDOW_S;

/*RGB颜色结构体定义*/
typedef struct hiVO_ARGB_S
{
    HI_U8                  u8B;                  /*blue*/
    HI_U8                  u8G;                  /*green*/
    HI_U8                  u8R;                  /*red*/
    HI_U8                  u8Alpha;              /*透明度*/
}VO_ARGB_S;

typedef union hiVO_CLUT_ENTRY_RGB_U
{
    VO_ARGB_S              sARGB;
    HI_U32                 u32REG;
}VO_CLUT_ENTRY_RGB_U;

typedef struct hiVO_CLUT_RGB_S
{
    VO_CLUT_ENTRY_RGB_U   *pClut; 
    HI_U32                 u32Len;
}VO_CLUT_RGB_S;


typedef struct hiVO_YCbCr_S
{
    HI_U8                  u8Cb;  
    HI_U8                  u8Y;
    HI_U8                  u8Cr;
}VO_YCbCr_S;

typedef struct hiVO_AYCbCr_S
{
    HI_U8                  u8Cb;  
    HI_U8                  u8Y;
    HI_U8                  u8Cr;
    HI_U8                  u8Alpha;              /*透明度*/
}VO_AYCbCr_S;

typedef union hiVO_CLUT_ENTRY_YCbCr_U
{
    VO_AYCbCr_S            sAYCbCr;
    HI_U32                 u32REG;
}VO_CLUT_ENTRY_YCbCr_U;

typedef struct hiVO_CLUT_YCbCr_S
{
    VO_CLUT_ENTRY_YCbCr_U  *pClut; 
    HI_U32                 u32Len;
}VO_CLUT_YCbCr_S;


typedef struct hiVO_CLUT_COLOR_S
{
    HI_U32                 u32Index;
    HI_U32                 u32Color;
}VO_CLUT_COLOR_S;


/* 编码制式设置 */
typedef enum hiVO_ENCODE_FORMAT_E
{
    VO_ENCODE_FORMAT_PAL = 0x0,                    /*PAL制*/
    VO_ENCODE_FORMAT_NTSC,                         /*NTSC制*/
} VO_ENCODE_FORMAT_E;

/* 输出制式设置 */
typedef enum hiVO_OUTPUT_FORMAT_E
{
    VO_OUTPUT_FORMAT_PAL = 0x0,                    /*强制PAL制输出*/
    VO_OUTPUT_FORMAT_NTSC,                         /*强制NTSC制输出*/
    VO_OUTPUT_FORMAT_AUTO,                         /*自适应*/
} VO_OUTPUT_FORMAT_E;


/* 视频输出信号端口 */
typedef enum hiVO_OUTPUT_INTERFACE_E
{
    VO_OUTPUT_INTERFACE_NONE=0x0,
    VO_OUTPUT_INTERFACE_CVBS2_SVIDEO,           //0x1
    VO_OUTPUT_INTERFACE_CVBS2,                  //0x2
    VO_OUTPUT_INTERFACE_CVBS3,                  //0x3
    VO_OUTPUT_INTERFACE_CVBS4,                  //0x4
    VO_OUTPUT_INTERFACE_CVBS_SVIDEO,            //0x5
    VO_OUTPUT_INTERFACE_CVBS_YUV,               //0x6
    VO_OUTPUT_INTERFACE_CVBS_RGB,               //0x7
    VO_OUTPUT_INTERFACE_SVIDEO_SVIDEO,          //0x8
}VO_OUTPUT_INTERFACE_E;


/* 视频输出信号端口 */
typedef enum hiVO_DAC_MODE_E
{
    VO_DAC_MODE_SILENCE = 0,
    VO_DAC_MODE_CVBS,
    VO_DAC_MODE_Y,
    VO_DAC_MODE_PB,
    VO_DAC_MODE_PR,
    VO_DAC_MODE_R,
    VO_DAC_MODE_G,
    VO_DAC_MODE_B,
    VO_DAC_MODE_SVIDEO_Y,
    VO_DAC_MODE_SVIDEO_C,
    VO_DAC_MODE_BUTT
}VO_DAC_MODE_E;

typedef struct  hiVO_DAC_ATTR_S
{
    VO_DAC_MODE_E         enDacMode[VO_MAX_DAC_NUM];
}VO_DAC_ATTR_S ;

/*显存结构定义*/
typedef struct hiOSD_FRAME_BUFFER_S
{
    HI_CHAR *pFrameBuffer;
    HI_U32  u32FrameBufferPhy;
    HI_U32  u32Size;
}OSD_FRAME_BUFFER_S;

/*OSD层次定义*/
typedef enum hiOSD_LAYER_E
{
    OSD_LAYER_0 = 0,                         /*两层OSD在通过两层layer实现*/
    OSD_LAYER_1,
    OSD_LAYER_ALL
}OSD_LAYER_E;

/*OSD采取的BUFFER刷新模式*/
typedef enum hiOSD_BUFFER_MODE_E
{
    OSD_BUFFER_MODE_ONE = 0,
    OSD_BUFFER_MODE_DOUBLE,
    OSD_BUFFER_MODE_THREE,
    OSD_BUFFER_MODE_BUTT
}OSD_BUFFER_MODE_E;

/*OSD支持的颜色模式定义*/
typedef enum hiOSD_COLOR_FORMAT_E
{
    OSD_RGB_444 = TDE2_COLOR_FMT_RGB444,
    OSD_RGB_555 = TDE2_COLOR_FMT_RGB555,
    OSD_RGB_565 = TDE2_COLOR_FMT_RGB565,
    OSD_RGB_888 = TDE2_COLOR_FMT_RGB888,
    OSD_ARGB_4444 = TDE2_COLOR_FMT_ARGB4444,
    OSD_ARGB_1555 = TDE2_COLOR_FMT_ARGB1555,
    OSD_ARGB_8565 = TDE2_COLOR_FMT_ARGB8565,
    OSD_ARGB_8888 = TDE2_COLOR_FMT_ARGB8888,
    OSD_YCbCr_888 = TDE2_COLOR_FMT_YCbCr888,
    OSD_AYCbCr_8888 = TDE2_COLOR_FMT_AYCbCr8888,
    OSD_YCbCr_422RASTER = TDE2_COLOR_FMT_YCbCr422,
    OSD_ARGB_CLUT8 = 0x50,
    OSD_AYCbCr_CLUT8 = 0x51
}OSD_COLOR_FORMAT_E;

typedef enum hiOSD_ASPECT_RATIO_E
{
    OSD_ASPECT_RATIO_4_3 = 0,
    OSD_ASPECT_RATIO_16_9
}OSD_ASPECT_RATIO_E;

/*关键色的模式定义*/
typedef enum hiOSD_COLORKEY_MODE_E
{
    OSD_COLORKEY_IGNORE = 0,                /*不做ColorKey*/
    OSD_COLORKEY_INSIDE,                    /*ColorKey在ColorMin和ColorMax之间*/
    OSD_COLORKEY_OUTSIDE                    /*ColorKey在ColorMin和ColorMax区间外*/
}OSD_COLORKEY_MODE_E;

/*帧频变换方式*/
typedef enum hiVO_FRAME_RATE_TRANS_E
{
    VO_FRAME_RATE_P2P = 0,
    VO_FRAME_RATE_N2N,
    VO_FRAME_RATE_P2N,
    VO_FRAME_RATE_N2P
}VO_FRAME_RATE_TRANS_E;

/*关键色的参数结构体定义*/
typedef struct hiOSD_COLORKEY_S
{
    OSD_COLORKEY_MODE_E    eRedMode;
    OSD_COLORKEY_MODE_E    eGreenMode;
    OSD_COLORKEY_MODE_E    eBlueMode;
    HI_U8                  u8RedMin;
    HI_U8                  u8RedMax;
    HI_U8                  u8GreenMin;
    HI_U8                  u8GreenMax;
    HI_U8                  u8BlueMin;
    HI_U8                  u8BlueMax;
    HI_U8                  u8Alpha;          /*符合ColorSpace条件时像素的透明度*/
}OSD_COLORKEY_S;

/*屏幕属性参数的结构体定义*/
typedef struct hiOSD_SCREEN_PARA_S
{
    HI_U32                 u32StX;
    HI_U32                 u32StY;
    HI_U32                 u32Width;         /*屏幕的宽度*/
    HI_U32                 u32Height;        /*屏幕的高度*/
}OSD_SCREEN_PARA_S;

/*region的属性参数结构体定义*/
typedef struct hiOSD_REGION_PARA_S
{
    OSD_LAYER_E            eLayer;
    OSD_COLORKEY_S         sColorKeyPara;
    VO_RECTANGLE_S         sClipArea;        /*裁剪区域*/
    VO_POSITION_S          sPosition;        /*起始点坐标*/
    OSD_COLOR_FORMAT_E     eColorFormat;
    HI_U32                 u32Width;
    HI_U32                 u32Height;
    HI_U32                 u32Alpha;         /*region的全局alpha*/
    HI_U32                 u32ExtAlpha0;     /*1555颜色高位为0时的透明度*/
    HI_U32                 u32ExtAlpha1;     /*1555颜色高位为1时的透明度*/
    HI_CHAR                *pBuffer;
    HI_U32                 pBufferPhy;
    HI_U32                 u32BufferSize;
    HI_U32                 *pClut;
    HI_BOOL                bShow;
    HI_BOOL                bColorKeyEn;
    HI_BOOL                bClipAreaEn;
    HI_BOOL                bAlphaPremul;
}OSD_REGION_PARA_S;

/*位置结构体定义*/
typedef struct hiCURSOR_POSITION_S
{
    HI_U32                 u32StX;
    HI_U32                 u32StY;
    HI_U32                 u32BaseWidth;
    HI_U32                 u32BaseHeight;
}CURSOR_POSITION_S;

/*Cursor装载参数定义*/
typedef struct hiCURSOR_PARA_S
{
    HI_U32                *pBmp;
    HI_U32                *pClut;
    CURSOR_POSITION_S          sPosition;
    HI_U32                 u32Alpha;
}CURSOR_PARA_S;

/*图层显示顺序枚举定义*/
typedef enum hiMIXER_PLANE_ORDER_E
{
    MIXER_OSD1_VID_OSD2 = 0,
    MIXER_OSD1_OSD2_VID,
    MIXER_VID_OSD1_OSD2,
    MIXER_VID_OSD2_OSD1,
    MIXER_OSD2_OSD1_VID,
    MIXER_OSD2_VID_OSD1
}MIXER_PLANE_ORDER_E;

typedef struct hiVO_REGISTER_S
{
    HI_U32 offset;
    HI_U32 value;
}VO_REGISTER_S;


/**************************type define for ioctl****************************/
typedef struct hiOSD_GET_BUF_S
{
    OSD_LAYER_E           eLayer;
    HI_U32                u32BufPhyAddr;
    HI_U32                u32BufSize;
}OSD_GET_BUF_S;

typedef struct hiOSD_SET_RATIO_S
{
    OSD_LAYER_E           eLayer;
    OSD_ASPECT_RATIO_E    eRatio;
}OSD_SET_RATIO_S;

typedef struct hiOSD_GET_RATIO_S
{
    OSD_LAYER_E           eLayer;
    OSD_ASPECT_RATIO_E    *pRatio;
}OSD_GET_RATIO_S;

typedef struct hiOSD_SET_SCREEN_S
{
    OSD_LAYER_E           eLayer;
    VO_RECTANGLE_S       *pScreen;
}OSD_SET_SCREEN_S;

typedef struct hiOSD_SET_RESOLUTION_S
{
    OSD_LAYER_E           eLayer;
    HI_U32                width;
    HI_U32                heigth;
}OSD_SET_RESOLUTION_S;

typedef struct hiOSD_GET_SCREEN_S
{
    OSD_LAYER_E           eLayer;
    VO_RECTANGLE_S       *pScreen;
}OSD_GET_SCREEN_S;

typedef struct hiOSD_CREATE_REGION_S
{
    OSD_REGION_PARA_S     *pRegionPara;
    OSD_REGION_HANDLE_T   *pHandle;
}OSD_CREATE_REGION_S;
    
typedef struct hiOSD_RESIZE_REGION_S
{
    OSD_REGION_HANDLE_T   Handle;
    HI_U32               u32Width;
    HI_U32               u32Height;
    HI_CHAR              *pBuffer;
    HI_U32               u32BufferPhy;
    HI_U32               u32BufferSize;
}OSD_RESIZE_REGION_S;

typedef struct hiOSD_SET_REGION_POS_S
{
    OSD_REGION_HANDLE_T   Handle; 
    VO_POSITION_S        *pPosition;
}OSD_SET_REGION_POS_S;

typedef struct hiOSD_SET_CLUT_S
{
    OSD_REGION_HANDLE_T   Handle; 
    HI_U32               *pClut;
}OSD_SET_CLUT_S;

typedef struct hiOSD_SET_CLIP_S
{
    OSD_REGION_HANDLE_T   Handle; 
    VO_RECTANGLE_S       *pClipArea;
}OSD_SET_CLIP_S;

typedef struct hiOSD_SET_COLORKEY_S
{
    OSD_REGION_HANDLE_T   Handle; 
    OSD_COLORKEY_S       *pColorKey;
}OSD_SET_COLORKEY_S;

typedef struct hiOSD_GET_PIXEL_S
{
    OSD_REGION_HANDLE_T   Handle;
    VO_POSITION_S        *pPosition;
    HI_U32               *pColor;
}OSD_GET_PIXEL_S;

typedef struct hiOSD_SET_COLOR_FORMAT_S
{
    OSD_REGION_HANDLE_T   Handle;
    OSD_COLOR_FORMAT_E    eColorFormat;
}OSD_SET_COLOR_FORMAT_S;

typedef struct hiOSD_SET_ALPHA_S
{
    OSD_REGION_HANDLE_T   Handle;
    HI_U32                u32Alpha;
}OSD_SET_ALPHA_S;

typedef struct hiOSD_SET_EXT_ALPHA_S
{
    OSD_REGION_HANDLE_T   Handle;
    HI_U32                u32ExtAlpha0;
    HI_U32                u32ExtAlpha1;
}OSD_SET_EXT_ALPHA_S;

typedef struct hiOSD_SYNC_OPERATION_S
{
    OSD_REGION_HANDLE_T   Handle;
    VO_RECTANGLE_S       *pOperationArea;
    HI_BOOL               bBlock;
}OSD_SYNC_OPERATION_S;

typedef struct hiOSD_GET_REGION_S
{
    OSD_REGION_HANDLE_T   Handle;
    OSD_REGION_PARA_S     *pRegionPara;
}OSD_GET_REGION_S;

typedef struct hiOSD_IS_COMPLETE_S
{
    OSD_REGION_HANDLE_T   Handle;
    HI_BOOL              *pbComplete;
}OSD_IS_COMPLETE_S;

typedef struct hiOSD_SET_BMPADDR_S
{
    OSD_REGION_HANDLE_T   Handle;
    HI_CHAR               *pBmpVir;
    HI_U32                u32BmpPhy;
}OSD_SET_BMPADDR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __VO_TYPE_H__ */

