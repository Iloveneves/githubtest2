/*******************************************************************************
 *              Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_jpg_type.h
 * Description:
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-03-26       d37024    HI_NULL      Create this file.
 ******************************************************************************/
#ifndef _HI_JPG_TYPE_H_
#define _HI_JPG_TYPE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_type.h"

/*invalid handle*/
#define JPG_INVALID_HANDLE 0xFFFFFFFF

/*handle type*/
typedef HI_U32 JPG_HANDLE;

/*JPEG图片类型*/
typedef enum hiJPG_IMGTYPE_E
{
    JPG_IMGTYPE_NORMAL = 0, /*普通JPEG图片*/
    JPG_IMGTYPE_MOV_MJPEGA, /*MOV Motion JPG A*/
    JPG_IMGTYPE_MOV_MJPEGB, /*MOV Motion JPG B*/
    JPG_IMGTYPE_AVI_MJPEG,  /*AVI Motion JPG*/
    JPG_IMGTYPE_BUTT
} JPG_IMGTYPE_E;

/*JPG原图的编码类型*/
typedef enum hiJPG_SOURCEFMT_E
{
    JPG_SOURCE_COLOR_FMT_YCBCR400,    /*YUV400*/
    JPG_SOURCE_COLOR_FMT_YCBCR420 = 3,    /*YUV420*/
    JPG_SOURCE_COLOR_FMT_YCBCR422BHP, /*YUV 422 2x1*/  
    JPG_SOURCE_COLOR_FMT_YCBCR422BVP, /*YUV 422 1x2*/  
    JPG_SOURCE_COLOR_FMT_YCBCR444,    /*YUV 444*/  
    JPG_SOURCE_COLOR_FMT_BUTT         /*其它不支持的格式*/
}JPG_SOURCEFMT_E;

/*JPG支持的输出颜色模式定义*/
typedef enum hiJPG_OUTTYPE_E
{
    JPG_OUTTYPE_INTERLEAVE = 0,  /*交织类型，指RGB或YUV交织类型*/
    JPG_OUTTYPE_MACROBLOCK,      /*宏块类型，指Y分量与CbCr分量是分开存放的*/
    JPG_OUTTYPE_BUTT
}JPG_OUTTYPE_E;

/* 交织类型的颜色格式 */
typedef enum hiJPG_COLORFMT_E
{
    JPG_COLORFMT_FMT_RGB444,
    JPG_COLORFMT_FMT_RGB555,
    JPG_COLORFMT_FMT_RGB565,
    JPG_COLOR_FMT_RGB888,
    JPG_COLOR_FMT_ARGB4444,
    JPG_COLOR_FMT_ARGB1555,
    JPG_COLOR_FMT_ARGB8565,
    JPG_COLOR_FMT_ARGB8888,
    JPG_COLOR_FMT_CLUT1,
    JPG_COLOR_FMT_CLUT2,
    JPG_COLOR_FMT_CLUT4,
    JPG_COLOR_FMT_CLUT8,
    JPG_COLOR_FMT_ACLUT44,
    JPG_COLOR_FMT_ACLUT88,
    JPG_COLOR_FMT_A1,
    JPG_COLOR_FMT_A8,
    JPG_COLOR_FMT_YCBCR888,
    JPG_COLOR_FMT_AYCBCR8888,
    JPG_COLOR_FMT_YCBCR422,
    JPG_COLOR_FMT_BYTE,
    JPG_COLOR_FMT_HALFWORD,
    JPG_ILCOLOR_FMT_BUTT
} JPG_COLORFMT_E;

/* 宏块类型的颜色格式 */
typedef enum hiJPG_MBCOLORFMT_E
{
    JPG_MBCOLOR_FMT_JPG_YCbCr400MBP,
    JPG_MBCOLOR_FMT_JPG_YCbCr422MBHP,
    JPG_MBCOLOR_FMT_JPG_YCbCr422MBVP,
    JPG_MBCOLOR_FMT_MP1_YCbCr420MBP,
    JPG_MBCOLOR_FMT_MP2_YCbCr420MBP,
    JPG_MBCOLOR_FMT_MP2_YCbCr420MBI,
    JPG_MBCOLOR_FMT_JPG_YCbCr420MBP,
    JPG_MBCOLOR_FMT_JPG_YCbCr444MBP,
    JPG_MBCOLOR_FMT_BUTT
} JPG_MBCOLORFMT_E;

/*JPEG图编码种类，当前只支持baseline的*/
typedef enum hiJPG_PICTYPE_E
{
    JPG_PICTYPE_BASELINE = 0, /*baseline类型*/
    JPG_PICTYPE_EXTENDED,     /*Extended类型*/
    JPG_PICTYPE_PROGRESSIVE,  /*Progressive类型*/
    JPG_PICTYPE_LOSSLESS,     /*Lossless类型*/
    JPG_PICTYPE_BUTT          /*其它不支持的类型*/
}JPG_PICTYPE_E;

/*JPG一幅图像信息*/
typedef struct hiJPG_PICINFO_S
{
    JPG_PICTYPE_E Profile;        /*编码类别*/
    HI_U32 Width;                 /*宽度*/
    HI_U32 Height;                /*高度*/
    HI_U32 SamplePrecision;       /*采样精度*/
    HI_U32 ComponentNum;          /*分量数*/
    JPG_SOURCEFMT_E EncodeFormat; /*编码格式*/
}JPG_PICINFO_S;

/*JPG图片信息*/
typedef struct hiJPG_PRIMARYINFO_S
{    
    HI_U32         Count;    /*图个数，第0个为主图信息，其它为缩略图*/    
    JPG_PICINFO_S* pPicInfo; /*图信息*/
}JPG_PRIMARYINFO_S;

/* 用户设置的位图信息结构 */
typedef struct hiJPG_ILSURFACE_S
{
    JPG_COLORFMT_E ColorFmt;    /* 颜色格式 */
    HI_U32 PhyAddr;             /* 位图首地址 */ 
    HI_VOID* VirtAddr;          /* 位图虚拟地址，软解码用到 */
    HI_U32 Height;              /* 位图高度 */
    HI_U32 Width;               /* 位图宽度 */ 
    HI_U32 Stride;              /* 位图跨度 */
    HI_U8* pClutPhyAddr;        /*Clut表地址,用作颜色扩展或颜色校正*/ 
    HI_BOOL AlphaMax255;        /* 位图alpha最大值为255还是128 */ 
    HI_BOOL SubbyteAlign;       /* 像素大小不足byte格式,低位对应最右侧像素*/
    HI_U8 Alpha0;               /* Alpha0、Alpha1值，用作ARGB1555格式 */
    HI_U8 Alpha1;
} JPG_ILSURFACE_S;

typedef struct hiJPG_MBSURFACE_S
{
    JPG_MBCOLORFMT_E    MbFmt;
    HI_U32              YPhyAddr;
    HI_VOID*            YVirtAddr;
    HI_U32              YWidth;
    HI_U32              YHeight;
    HI_U32              YStride;
    HI_U32              CbCrPhyAddr;
    HI_VOID*            CbCrVirtAddr;
    HI_U32              CbCrStride;
} JPG_MBSURFACE_S;

/*解码后的位图存放信息*/
typedef struct hiJPG_SURFACE_S
{
    JPG_OUTTYPE_E  OutType;        /*输出类型，分为交织和宏块两类*/
    union
    {
        JPG_ILSURFACE_S Surface;   /*交织类型位图存储信息*/
        JPG_MBSURFACE_S MbSurface; /*宏块类型位图存储信息*/
    }SurfaceInfo;
}JPG_SURFACE_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_HI_JPG_TYPE_H_*/

