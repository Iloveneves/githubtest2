/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
 File Name     : tde_type.h
Version       : Initial Draft
Author        : w54130
Created       : 2007/5/21
Last Modified :
Description   : TDE public type
Function List :
History       :
1.Date        : 2007/5/21
Author      : w54130
Modification: Created file

 ******************************************************************************/
#ifndef __TDE_TYPE_H__
#define __TDE_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_tde_errcode.h"

/****************************************************************************/
/*                             TDE2 types define                             */
/****************************************************************************/

typedef HI_S32 TDE_HANDLE;

typedef HI_VOID (* TDE_FUNC_CB) (HI_VOID *);



/* 颜色格式 */
typedef enum hiTDE2_COLOR_FMT_E
{
    TDE2_COLOR_FMT_RGB444 = 0,
    TDE2_COLOR_FMT_RGB555,
    TDE2_COLOR_FMT_RGB565,
    TDE2_COLOR_FMT_RGB888,
    TDE2_COLOR_FMT_ARGB4444,
    TDE2_COLOR_FMT_ARGB1555,
    TDE2_COLOR_FMT_ARGB8565,
    TDE2_COLOR_FMT_ARGB8888,
    TDE2_COLOR_FMT_CLUT1,
    TDE2_COLOR_FMT_CLUT2,
    TDE2_COLOR_FMT_CLUT4,
    TDE2_COLOR_FMT_CLUT8,
    TDE2_COLOR_FMT_ACLUT44,
    TDE2_COLOR_FMT_ACLUT88,
    TDE2_COLOR_FMT_A1,
    TDE2_COLOR_FMT_A8,
    TDE2_COLOR_FMT_YCbCr888,
    TDE2_COLOR_FMT_AYCbCr8888,
    TDE2_COLOR_FMT_YCbCr422,
    TDE2_COLOR_FMT_byte,
    TDE2_COLOR_FMT_halfword,
    TDE2_COLOR_FMT_BUTT
} TDE2_COLOR_FMT_E;

typedef enum hiTDE2_MB_COLORFMT_E
{
    TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP = 0,
    TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP,
    TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP,
    TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP,
    TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP,
    TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI,
    TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP,
    TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
    TDE2_MB_COLOR_FMT_BUTT
} TDE2_MB_COLOR_FMT_E;

/* 用户设置的位图信息结构 */
typedef struct hiTDE2_SURFACE_S
{
    /* 位图首地址 */
    HI_U32 u32PhyAddr;

    /* 颜色格式 */
    TDE2_COLOR_FMT_E enColorFmt;

    /* 位图高度 */
    HI_U32 u32Height;

    /* 位图宽度 */
    HI_U32 u32Width;

    /* 位图跨度 */
    HI_U32 u32Stride;

    /*Clut表地址,用作颜色扩展或颜色校正*/
    HI_U8* pu8ClutPhyAddr;

    /* Clut表是否位于YCbCr空间 */
    HI_BOOL bYCbCrClut;

    /* 位图alpha最大值为255还是128 */
    HI_BOOL bAlphaMax255;

    /* Alpha0、Alpha1值，用作ARGB1555格式 */
    HI_BOOL bAlphaExt1555; /* 是否使能1555的Alpha扩展 */
    HI_U8 u8Alpha0;
    HI_U8 u8Alpha1;
} TDE2_SURFACE_S;

typedef struct hiTDE2_MB_S
{
    TDE2_MB_COLOR_FMT_E enMbFmt;
    HI_U32              u32YPhyAddr;
    HI_U32              u32YWidth;
    HI_U32              u32YHeight;
    HI_U32              u32YStride;
    HI_U32              u32CbCrPhyAddr;
    HI_U32              u32CbCrStride;
} TDE2_MB_S;

typedef struct hiTDE2_RECT_S
{
    HI_S32 s32Xpos;
    HI_S32 s32Ypos;
    HI_U32 u32Height;
    HI_U32 u32Width;
} TDE2_RECT_S;

/* 逻辑运算方式 */
typedef enum hiTDE2_ALUCMD_E
{
    TDE2_ALUCMD_NONE = 0, 
    TDE2_ALUCMD_BLEND,            /*Alpha混合,源2没有预乘模式*/
    TDE2_ALUCMD_BLEND_PREMUL,     /*Alpha混合,源2已经预乘模式*/
    TDE2_ALUCMD_ROP,
    TDE2_ALUCMD_BUTT
} TDE2_ALUCMD_E;

typedef enum hiTDE2_ROP_CODE_E
{
    TDE2_ROP_BLACK = 0,         /*Blackness*/
    TDE2_ROP_NOTMERGEPEN,   /*~(S2+S1)*/
    TDE2_ROP_MASKNOTPEN,    /*~S2&S1*/
    TDE2_ROP_NOTCOPYPEN,    /* ~S2*/
    TDE2_ROP_MASKPENNOT,    /* S2&~S1 */
    TDE2_ROP_NOT,           /* ~S1 */
    TDE2_ROP_XORPEN,        /* S2^S1 */
    TDE2_ROP_NOTMASKPEN,    /* ~(S2&S1) */
    TDE2_ROP_MASKPEN,       /* S2&S1 */
    TDE2_ROP_NOTXORPEN,     /* ~(S2^S1) */
    TDE2_ROP_NOP,           /* S1 */
    TDE2_ROP_MERGENOTPEN,   /* ~S2+S1 */
    TDE2_ROP_COPYPEN,       /* S2 */
    TDE2_ROP_MERGEPENNOT,   /* S2+~S1 */
    TDE2_ROP_MERGEPEN,      /* S2+S1 */
    TDE2_ROP_WHITE,         /* Whiteness */
    TDE2_ROP_BUTT
} TDE2_ROP_CODE_E;

typedef enum hiTDE2_MIRROR_E
{
    TDE2_MIRROR_NONE = 0,
    TDE2_MIRROR_HORIZONTAL,
    TDE2_MIRROR_VERTICAL,
    TDE2_MIRROR_BOTH,
    TDE2_MIRROR_BUTT
} TDE2_MIRROR_E;

/*Clip操作类型*/
typedef enum hiTDE2_CLIPMODE_E
{
    TDE2_CLIPMODE_NONE = 0,
    TDE2_CLIPMODE_INSIDE,
    TDE2_CLIPMODE_OUTSIDE,
    TDE2_CLIPMODE_BUTT
} TDE2_CLIPMODE_E;

/*宏块格式缩放类型*/
typedef enum hiTDE2_MBRESIZE_E
{
    TDE2_MBRESIZE_NONE = 0,
    TDE2_MBRESIZE_QUALITY_LOW,
    TDE2_MBRESIZE_QUALITY_MIDDLE,
    TDE2_MBRESIZE_QUALITY_HIGH,
    TDE2_MBRESIZE_BUTT
} TDE2_MBRESIZE_E;

typedef struct hiTDE2_FILLCOLOR_S
{
    TDE2_COLOR_FMT_E enColorFmt;
    HI_U32           u32FillColor;
} TDE2_FILLCOLOR_S;

typedef enum hiTDE2_COLORKEY_MODE_E
{
    TDE2_COLORKEY_MODE_NONE = 0,     /* 不做color key */
    TDE2_COLORKEY_MODE_FOREGROUND,   /* 对前景位图进行color key，说明:对于颜色扩展，在CLUT前做color key；
                                        对于颜色校正:在CLUT后做color key */
    TDE2_COLORKEY_MODE_BACKGROUND,    /* 对背景位图进行color key*/
    TDE2_COLORKEY_MODE_BUTT
} TDE2_COLORKEY_MODE_E;

typedef struct hiTDE2_COLORKEY_COMP_S
{
    HI_U8 u8CompMin;           /* 分量最小值*/
    HI_U8 u8CompMax;           /* 分量最大值*/
    HI_U8 bCompOut;            /* 分量关键色在范围内/范围外*/
    HI_U8 bCompIgnore;         /* 分量是否忽略*/
} TDE2_COLORKEY_COMP_S;

typedef union hiTDE2_COLORKEY_U
{
    struct
    {
        TDE2_COLORKEY_COMP_S stAlpha;
        TDE2_COLORKEY_COMP_S stRed;
        TDE2_COLORKEY_COMP_S stGreen;
        TDE2_COLORKEY_COMP_S stBlue;
    } struCkARGB;
    struct
    {
        TDE2_COLORKEY_COMP_S stAlpha;
        TDE2_COLORKEY_COMP_S stY;
        TDE2_COLORKEY_COMP_S stCb;
        TDE2_COLORKEY_COMP_S stCr;
    } struCkYCbCr;
    struct
    {
        TDE2_COLORKEY_COMP_S stAlpha;
        TDE2_COLORKEY_COMP_S stClut;
    } struCkClut;
} TDE2_COLORKEY_U;

typedef enum hiTDE2_OUTALPHA_FROM_E
{
    TDE2_OUTALPHA_FROM_NORM = 0,    /* 来源于alpha blending的结果或者抗闪烁的结果 */
    TDE2_OUTALPHA_FROM_BACKGROUND,  /* 来源于背景位图 */
    TDE2_OUTALPHA_FROM_FOREGROUND,  /* 来源于前景位图 */
    TDE2_OUTALPHA_FROM_GLOBALALPHA, /* 来源于全局alpha */
    TDE2_OUTALPHA_FROM_BUTT
} TDE2_OUTALPHA_FROM_E;

typedef enum hiTDE2_FILTER_MODE_E
{
    TDE2_FILTER_MODE_COLOR = 0, /* 对颜色进行滤波 */
    TDE2_FILTER_MODE_ALPHA,     /* 对alpha通道滤波 */
    TDE2_FILTER_MODE_BOTH,      /* 对颜色和alpha通道同时滤波 */
    TDE2_FILTER_MODE_BUTT
} TDE2_FILTER_MODE_E;

typedef struct hiTDE2_OPT_S
{
    /*逻辑运算类型*/
    TDE2_ALUCMD_E enAluCmd;

    /*颜色空间ROP类型*/
    TDE2_ROP_CODE_E enRopCode_Color;

    /*Alpha的ROP类型*/
    TDE2_ROP_CODE_E enRopCode_Alpha;

    /*color key方式*/
    TDE2_COLORKEY_MODE_E enColorKeyMode;

    /*color key设置值*/
    TDE2_COLORKEY_U unColorKeyValue;

    /*区域内作clip还是区域外作clip*/
    TDE2_CLIPMODE_E enClipMode;

    /*clip区域定义*/
    TDE2_RECT_S stClipRect;

    /*是否抗闪烁*/
    HI_BOOL bDeflicker;

    /*是否缩放*/
    HI_BOOL bResize;

    /* 缩放或deflicker时使用的滤波模式 */
    TDE2_FILTER_MODE_E enFilterMode;

    /*镜像类型*/
    TDE2_MIRROR_E enMirror;

    /*是否重新加载Clut表*/
    HI_BOOL bClutReload;

    /*全局Alpha值*/
    HI_U8   u8GlobalAlpha;

    /*输出alpha来源*/
    TDE2_OUTALPHA_FROM_E enOutAlphaFrom;
} TDE2_OPT_S;

typedef struct hiTDE2_MBOPT_S
{
    /*Clip模式选择*/
    TDE2_CLIPMODE_E enClipMode;

    /*clip区域定义*/
    TDE2_RECT_S stClipRect;

    /*是否抗闪烁*/
    HI_BOOL bDeflicker;

    /*缩放信息*/
    TDE2_MBRESIZE_E enResize;

    /*如果不设置Alpha,则默认输出最大Alpha值*/
    HI_BOOL bSetOutAlpha;
    HI_U8   u8OutAlpha;
} TDE2_MBOPT_S;


typedef enum hiTDE_ROTATE_ANGLE_E
{
    TDE_ROTATE_CLOCKWISE_90 = 0,    /* 顺时针旋转90度 */
    TDE_ROTATE_CLOCKWISE_180,   /* 顺时针旋转180度 */
    TDE_ROTATE_CLOCKWISE_270,   /* 顺时针旋转270度 */
    TDE_ROTATE_BUTT
} TDE_ROTATE_ANGLE_E;


typedef struct hiTDE_BITBLIT_CMD_S
{
    TDE_HANDLE     s32Handle;
    TDE2_SURFACE_S stBackGround;
    TDE2_RECT_S    stBackGroundRect;
    TDE2_SURFACE_S stForeGround;
    TDE2_RECT_S    stForeGroundRect;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S    stDstRect;
    TDE2_OPT_S     stOpt;
    HI_U32         u32NullIndicator;
} TDE_BITBLIT_CMD_S;

typedef struct hiTDE_SOLIDDRAW_CMD_S
{
    TDE_HANDLE       s32Handle;
    TDE2_SURFACE_S   stForeGround;
    TDE2_RECT_S      stForeGroundRect;
    TDE2_SURFACE_S   stDst;
    TDE2_RECT_S      stDstRect;
    TDE2_FILLCOLOR_S stFillColor;
    TDE2_OPT_S       stOpt;
    HI_U32           u32NullIndicator;
} TDE_SOLIDDRAW_CMD_S;

typedef struct hiTDE_QUICKCOPY_CMD_S
{
    TDE_HANDLE     s32Handle;
    TDE2_SURFACE_S stSrc;
    TDE2_RECT_S    stSrcRect;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S    stDstRect;
} TDE_QUICKCOPY_CMD_S;

typedef struct hiTDE_QUICKFILL_CMD_S
{
    TDE_HANDLE     s32Handle;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S    stDstRect;
    HI_U32         u32FillData;
} TDE_QUICKFILL_CMD_S;

typedef struct hiTDE_QUICKDEFLICKER_CMD_S
{
    TDE_HANDLE     s32Handle;
    TDE2_SURFACE_S stSrc;
    TDE2_RECT_S    stSrcRect;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S    stDstRect;
} TDE_QUICKDEFLICKER_CMD_S;

typedef struct hiTDE_QUICKRESIZE_CMD_S
{
    TDE_HANDLE     s32Handle;
    TDE2_SURFACE_S stSrc;
    TDE2_RECT_S    stSrcRect;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S    stDstRect;
} TDE_QUICKRESIZE_CMD_S;

typedef struct hiTDE_MBBITBLT_CMD_S
{
    TDE_HANDLE     s32Handle;
    TDE2_MB_S      stMB;
    TDE2_RECT_S    stMbRect;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S    stDstRect;
    TDE2_MBOPT_S   stMbOpt;
} TDE_MBBITBLT_CMD_S;

typedef struct hiTDE_ENDJOB_CMD_S
{
    TDE_HANDLE s32Handle;
    HI_BOOL    bSync;
    HI_BOOL    bBlock;
    HI_U32     u32TimeOut;
} TDE_ENDJOB_CMD_S;

typedef struct hiTDE_BITMAP_MASKROP_CMD_S
{
    TDE_HANDLE     s32Handle;
    TDE2_SURFACE_S stBackGround;
    TDE2_RECT_S    stBackGroundRect;
    TDE2_SURFACE_S stForeGround;
    TDE2_RECT_S    stForeGroundRect;
    TDE2_SURFACE_S stMask;
    TDE2_RECT_S    stMaskRect;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S    stDstRect;
    TDE2_ROP_CODE_E enRopCode_Color;
    TDE2_ROP_CODE_E enRopCode_Alpha;
} TDE_BITMAP_MASKROP_CMD_S;

typedef struct hiTDE_BITMAP_MASKBLEND_CMD_S
{
    TDE_HANDLE     s32Handle;
    TDE2_SURFACE_S stBackGround;
    TDE2_RECT_S    stBackGroundRect;
    TDE2_SURFACE_S stForeGround;
    TDE2_RECT_S    stForeGroundRect;
    TDE2_SURFACE_S stMask;
    TDE2_RECT_S    stMaskRect;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S    stDstRect;
    HI_U8   u8Alpha;
    TDE2_ALUCMD_E enBlendMode;
}TDE_BITMAP_MASKBLEND_CMD_S;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __TDE_TYPE_H__ */


