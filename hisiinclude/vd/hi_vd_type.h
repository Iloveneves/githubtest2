/*********************************************************************************** 
*             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd. 
*                           ALL RIGHTS RESERVED 
* FileName    : hi_vd_type.h 
* Description : data structure definition provide to user and driver  
* 
* History: 
* Version   Date              Author     DefectNum    Description 
* main\1    2008-3-28         z67193     NULL         Create this file. 
***********************************************************************************/
#ifndef __HI_VD_TYPE_H__
#define __HI_VD_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

/*****************************************************************************
 *                                   宏定义                                  *
 *****************************************************************************/
#define VD_DISPLAY_ALPHA_MIN                       0x0
#define VD_DISPLAY_ALPHA_MAX                       0xFF

/* Default screen width and height */
#define VD_PAL_SCREEN_WIDTH                        720UL               /* 720 */
#define VD_PAL_SCREEN_HEIGHT                       576UL               /* 576 */
#define VD_NTSC_SCREEN_WIDTH                       720UL               /* 720 */
#define VD_NTSC_SCREEN_HEIGHT                      480UL               /* 480 */
#define VD_DEFAULT_SCREEN_WIDTH                    VD_PAL_SCREEN_WIDTH 
#define VD_DEFAULT_SCREEN_HEIGHT                   VD_PAL_SCREEN_HEIGHT
#define VD_WINDOW_WIDTH_MIN                        8UL
#define VD_WINDOW_HEIGHT_MIN                       6UL
#define VD_WINDOW_WIDTH_MAX                        VD_PAL_SCREEN_WIDTH
#define VD_WINDOW_HEIGHT_MAX                       VD_PAL_SCREEN_HEIGHT


/* Default window size */
#define VD_DEFAULT_WINDOW_X                        0x0
#define VD_DEFAULT_WINDOW_Y                        0x0
#define VD_DEFAULT_WINDOW_WIDTH                    VD_DEFAULT_SCREEN_WIDTH    /* 720 */
#define VD_DEFAULT_WINDOW_HEIGHT                   VD_DEFAULT_SCREEN_HEIGHT   /* 576 */

/*****************************************************************************
*                                   枚举定义                                 *
*****************************************************************************/

/* 视频层开关状态 */
typedef enum hiVD_DISPLAY_STATUS_E
{
    VD_DISPLAY_CLOSE = 0x0,
    VD_DISPLAY_OPEN
}VD_DISPLAY_STATUS_E;

/* 视频数据源*/
typedef enum hiVD_FRAME_SOURCE_E
{
    VD_FRAME_SOURCE_NONE = 0x0,                    /*来自解码器*/
    VD_FRAME_SOURCE_MPEG2_DECODER,                 /*来自解码器*/
    VD_FRAME_SOURCE_MJPEG_DECODER,                 /*来自MJPEG解码器*/
    VD_FRAME_SOURCE_USER                           /*来自用户*/
}VD_FRAME_SOURCE_E;

/* 视频显示帧格式*/
typedef enum hiVD_FRAME_COMPRESS_TYPE_E
{
    VD_FRAME_COMPRESS_MPEG2 = 0x0,                 /*MPEG2*/
    VD_FRAME_COMPRESS_MPEG1,                       /*MPEG1*/
    VD_FRAME_COMPRESS_MJPEG                        /*MJPEG*/
}VD_FRAME_COMPRESS_TYPE_E;


/* 视频输出幅型比设置 */
typedef enum hiVD_ASPECT_RATIO_E
{
    VD_ASPECT_RATIO_AUTO= 0x0,
    VD_ASPECT_RATIO_SQUARE,
    VD_ASPECT_RATIO_3_4,
    VD_ASPECT_RATIO_9_16 ,
    VD_ASPECT_RATIO_1_221,
    VD_ASPECT_RATIO_INVALID,
}VD_ASPECT_RATIO_E;

/* 视频层外观转换模式 */
typedef enum hiVD_ASPECT_CONVERSION_MODE_E
{
    VD_ASPECT_CONVERSION_MODE_IGNORE=0x0,
    VD_ASPECT_CONVERSION_MODE_LETTERBOX,
    VD_ASPECT_CONVERSION_MODE_PANANDSCAN,
    VD_ASPECT_CONVERSION_MODE_COMBINED,
}VD_ASPECT_CONVERSION_MODE_E;
#if 0
/* 视频显示重复模式 */
typedef enum hiVID_DISPLAY_REPEAT_TYPE_E
{
    VID_DISPLAY_REPEAT_FRAME = 0x0,                 /*按帧显示*/
    VID_DISPLAY_REPEAT_FIELD_TOP,                   /*重复显示顶场*/
    VID_DISPLAY_REPEAT_FIELD_BOTTOM,                /*重复显示底场*/
} VID_DISPLAY_REPEAT_TYPE_E;
#endif
/*帧的亮色取样方式*/
typedef enum hiVD_COLOR_FORMAT_E
{
    VD_YCbCr400MB = 0x0,
    VD_YCbCr420MB,
    VD_YCbCr422MBH,
    VD_YCbCr422MBV,
    VD_YCbCr444MB
}VD_COLOR_FORMAT_E;

/*帧的结构类型*/
typedef enum hiVD_PIC_STRUCTURE_E
{
    VD_PIC_STRUCTURE_FRAME = 0x0,                  /*帧图*/
    VD_PIC_STRUCTURE_FIELD,                        /*场图*/
}VD_PIC_STRUCTURE_E;

/*帧的结构类型*/
typedef enum hiVD_DISPLAY_DATA_TYPE_E
{
    VD_DISPLAY_DATA_FRAME = 0x0,                   /*显示一帧数据*/
    VD_DISPLAY_DATA_FIELD_TOP,                     /*显示顶场数据*/
    VD_DISPLAY_DATA_FIELD_BOTTOM,                  /*显示底场数据*/
}VD_DISPLAY_DATA_TYPE_E;

/*帧的显示时序*/
typedef enum hiVD_DISPLAY_SEQUENCE_E
{
    VD_DISPLAY_SEQUENCE_TB = 0x0,                  /*先显示顶场，再显示底场*/
    VD_DISPLAY_SEQUENCE_BT,                        /*先显示底场，再显示顶场*/
//    VID_DISPLAY_SEQUENCE_T,                         /*显示顶场*/
//    VID_DISPLAY_SEQUENCE_B,                         /*显示底场*/
    VD_DISPLAY_SEQUENCE_TBT,                       /*先显示顶场，再显示底场，再显示一场顶场*/
    VD_DISPLAY_SEQUENCE_BTB,                       /*先显示底场，再显示顶场，再显示一场底场*/
} VD_DISPLAY_SEQUENCE_E;

/*亮度垂直滤波方式*/
typedef enum hiVD_FILTER_Y_VERTICAL_E
{
    VD_FILTER_Y_VERTICAL_DISABLE = 0,              /*不做滤波*/
    VD_FILTER_Y_VERTICAL_2TAP,                     /*2阶滤波*/
    VD_FILTER_Y_VERTICAL_4TAP,                     /*4阶滤波*/
}VD_FILTER_Y_VERTICAL_E;

/*垂直滤波模式*/
typedef enum hiVD_FILTER_VERTICAL_MODE_E
{
    VD_FILTER_VERTICAL_FRAME = 0,                  /*帧内滤波*/
    VD_FILTER_VERTICAL_FIELD,                      /*场内滤波*/
}VD_FILTER_VERTICAL_MODE_E;

/*截屏方式*/
typedef enum hiVD_CAPTURE_TYPE_E
{
    VD_CAPTURE_TOP_FIELD= 0x0,
    VD_CAPTURE_BOTTOM_FIELD,
    VD_CAPTURE_FRAME,
}VD_CAPTURE_TYPE_E;
/*****************************************************************************
 *                                   结构体定义                              *
*****************************************************************************/

/*视频显示帧*/
typedef struct hiVD_DISPLAY_FRAME_S
{
    HI_VOID*                    pVirAddr;           /*图片数据虚拟地址*/
    HI_U32                      u32YAddr;           /*图片数据Y分量物理地址*/
    HI_U32                      u32CAddr;           /*图片数据UV分量物理地址*/
    HI_U32                      u32YStride;         /*图片亮度数据跨距*/
    HI_U32                      u32CStride;         /*图片色差数据跨距*/
    HI_U32                      u32Width;           /*图片宽度*/
    HI_U32                      u32Height;          /*图片高度*/
    VD_FRAME_COMPRESS_TYPE_E    eCompressType;      /*帧的压缩方式、MJPGE或者MPEG*/
    VD_COLOR_FORMAT_E           eColorFormat;       /*亮色取样类型*/
    VD_ASPECT_RATIO_E           eAspectRatio;       /*图片原始幅型比*/
    VO_ENCODE_FORMAT_E          eEncodeFormat;      /*图片原始制式*/
    VD_PIC_STRUCTURE_E          ePicStructure;      /*图片结构，场图、帧图*/
    VD_DISPLAY_DATA_TYPE_E      eDispDataType;      /*显示图片的全部，顶场数据、底场数据*/
    VD_DISPLAY_SEQUENCE_E       eDispSequence;      /*显示时序*/
    HI_U32                      u32DispTimes;       /*该帧的显示次数*/
    HI_U32                      u32VOffset1;        /*相位偏移*/
    HI_U32                      u32HOffset1;
    HI_U32                      u32VOffset2;
    HI_U32                      u32HOffset2;
    HI_U32                      u32VOffset3;
    HI_U32                      u32HOffset3;
}VD_DISPLAY_FRAME_S;

/* 视频宽高尺寸 */
typedef struct hiVD_FRAME_SIZE_S
{
    HI_U32                      u32Width;
    HI_U32                      u32Height;
}VD_FRAME_SIZE_S;


/*滤波器设置*/
typedef struct hiVD_FILTER_CONFIG_S
{
    VD_FILTER_Y_VERTICAL_E      eYVerticalEn;           /*亮度垂直滤波使能*/
    HI_BOOL                     bCVerticalEn;           /*色差垂直滤波使能*/
    HI_BOOL                     bYHorizontalEn;         /*亮度水平滤波使能*/
    HI_BOOL                     bCHorizontalEn;         /*色差水平滤波使能*/
    HI_BOOL                     bYVerticalMidianEn;     /*亮度垂直中值滤波使能*/
    HI_BOOL                     bYHorizontalMidianEn;   /*亮度水平中值滤波使能*/
    HI_BOOL                     bCHorizontalMidianEn;   /*色差水平中值滤波使能*/
}VD_FILTER_CONFIG_S;

/*截屏的输出颜色格式*//*AI7D03104*/
typedef enum hiVD_CAPTURE_COLOR_FORMAT_E
{
    VD_CAPTURE_RGB_444 = OSD_RGB_444,
    VD_CAPTURE_RGB_555 = OSD_RGB_555,
    VD_CAPTURE_RGB_565 = OSD_RGB_565,
    VD_CAPTURE_RGB_888 = OSD_RGB_888,
    VD_CAPTURE_ARGB_4444 = OSD_ARGB_4444,
    VD_CAPTURE_ARGB_1555 = OSD_ARGB_1555,
    VD_CAPTURE_ARGB_8565 = OSD_ARGB_8565,
    VD_CAPTURE_ARGB_8888 = OSD_ARGB_8888,
    VD_CAPTURE_YCbCr_888 = OSD_YCbCr_888,
    VD_CAPTURE_AYCbCr_8888 = OSD_AYCbCr_8888,
    VD_CAPTURE_YCbCr_422RASTER = OSD_YCbCr_422RASTER
}VD_CAPTURE_COLOR_FORMAT_E;

/*截屏参数*//*AI7D03104*/
typedef struct hiVD_CAPTURE_PICTURE_S
{
    HI_U32                      u32StartAddr;
    VD_CAPTURE_COLOR_FORMAT_E   eOutColorFormat;
    HI_U32                      u32Width;
    HI_U32                      u32Height;
    HI_U32                      u32Stride;
    VD_CAPTURE_TYPE_E           eCaptureType;
    HI_S32                      s32TimeOut;
}VD_CAPTURE_PICTURE_S;




#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif


