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
 *                                   �궨��                                  *
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
*                                   ö�ٶ���                                 *
*****************************************************************************/

/* ��Ƶ�㿪��״̬ */
typedef enum hiVD_DISPLAY_STATUS_E
{
    VD_DISPLAY_CLOSE = 0x0,
    VD_DISPLAY_OPEN
}VD_DISPLAY_STATUS_E;

/* ��Ƶ����Դ*/
typedef enum hiVD_FRAME_SOURCE_E
{
    VD_FRAME_SOURCE_NONE = 0x0,                    /*���Խ�����*/
    VD_FRAME_SOURCE_MPEG2_DECODER,                 /*���Խ�����*/
    VD_FRAME_SOURCE_MJPEG_DECODER,                 /*����MJPEG������*/
    VD_FRAME_SOURCE_USER                           /*�����û�*/
}VD_FRAME_SOURCE_E;

/* ��Ƶ��ʾ֡��ʽ*/
typedef enum hiVD_FRAME_COMPRESS_TYPE_E
{
    VD_FRAME_COMPRESS_MPEG2 = 0x0,                 /*MPEG2*/
    VD_FRAME_COMPRESS_MPEG1,                       /*MPEG1*/
    VD_FRAME_COMPRESS_MJPEG                        /*MJPEG*/
}VD_FRAME_COMPRESS_TYPE_E;


/* ��Ƶ������ͱ����� */
typedef enum hiVD_ASPECT_RATIO_E
{
    VD_ASPECT_RATIO_AUTO= 0x0,
    VD_ASPECT_RATIO_SQUARE,
    VD_ASPECT_RATIO_3_4,
    VD_ASPECT_RATIO_9_16 ,
    VD_ASPECT_RATIO_1_221,
    VD_ASPECT_RATIO_INVALID,
}VD_ASPECT_RATIO_E;

/* ��Ƶ�����ת��ģʽ */
typedef enum hiVD_ASPECT_CONVERSION_MODE_E
{
    VD_ASPECT_CONVERSION_MODE_IGNORE=0x0,
    VD_ASPECT_CONVERSION_MODE_LETTERBOX,
    VD_ASPECT_CONVERSION_MODE_PANANDSCAN,
    VD_ASPECT_CONVERSION_MODE_COMBINED,
}VD_ASPECT_CONVERSION_MODE_E;
#if 0
/* ��Ƶ��ʾ�ظ�ģʽ */
typedef enum hiVID_DISPLAY_REPEAT_TYPE_E
{
    VID_DISPLAY_REPEAT_FRAME = 0x0,                 /*��֡��ʾ*/
    VID_DISPLAY_REPEAT_FIELD_TOP,                   /*�ظ���ʾ����*/
    VID_DISPLAY_REPEAT_FIELD_BOTTOM,                /*�ظ���ʾ�׳�*/
} VID_DISPLAY_REPEAT_TYPE_E;
#endif
/*֡����ɫȡ����ʽ*/
typedef enum hiVD_COLOR_FORMAT_E
{
    VD_YCbCr400MB = 0x0,
    VD_YCbCr420MB,
    VD_YCbCr422MBH,
    VD_YCbCr422MBV,
    VD_YCbCr444MB
}VD_COLOR_FORMAT_E;

/*֡�Ľṹ����*/
typedef enum hiVD_PIC_STRUCTURE_E
{
    VD_PIC_STRUCTURE_FRAME = 0x0,                  /*֡ͼ*/
    VD_PIC_STRUCTURE_FIELD,                        /*��ͼ*/
}VD_PIC_STRUCTURE_E;

/*֡�Ľṹ����*/
typedef enum hiVD_DISPLAY_DATA_TYPE_E
{
    VD_DISPLAY_DATA_FRAME = 0x0,                   /*��ʾһ֡����*/
    VD_DISPLAY_DATA_FIELD_TOP,                     /*��ʾ��������*/
    VD_DISPLAY_DATA_FIELD_BOTTOM,                  /*��ʾ�׳�����*/
}VD_DISPLAY_DATA_TYPE_E;

/*֡����ʾʱ��*/
typedef enum hiVD_DISPLAY_SEQUENCE_E
{
    VD_DISPLAY_SEQUENCE_TB = 0x0,                  /*����ʾ����������ʾ�׳�*/
    VD_DISPLAY_SEQUENCE_BT,                        /*����ʾ�׳�������ʾ����*/
//    VID_DISPLAY_SEQUENCE_T,                         /*��ʾ����*/
//    VID_DISPLAY_SEQUENCE_B,                         /*��ʾ�׳�*/
    VD_DISPLAY_SEQUENCE_TBT,                       /*����ʾ����������ʾ�׳�������ʾһ������*/
    VD_DISPLAY_SEQUENCE_BTB,                       /*����ʾ�׳�������ʾ����������ʾһ���׳�*/
} VD_DISPLAY_SEQUENCE_E;

/*���ȴ�ֱ�˲���ʽ*/
typedef enum hiVD_FILTER_Y_VERTICAL_E
{
    VD_FILTER_Y_VERTICAL_DISABLE = 0,              /*�����˲�*/
    VD_FILTER_Y_VERTICAL_2TAP,                     /*2���˲�*/
    VD_FILTER_Y_VERTICAL_4TAP,                     /*4���˲�*/
}VD_FILTER_Y_VERTICAL_E;

/*��ֱ�˲�ģʽ*/
typedef enum hiVD_FILTER_VERTICAL_MODE_E
{
    VD_FILTER_VERTICAL_FRAME = 0,                  /*֡���˲�*/
    VD_FILTER_VERTICAL_FIELD,                      /*�����˲�*/
}VD_FILTER_VERTICAL_MODE_E;

/*������ʽ*/
typedef enum hiVD_CAPTURE_TYPE_E
{
    VD_CAPTURE_TOP_FIELD= 0x0,
    VD_CAPTURE_BOTTOM_FIELD,
    VD_CAPTURE_FRAME,
}VD_CAPTURE_TYPE_E;
/*****************************************************************************
 *                                   �ṹ�嶨��                              *
*****************************************************************************/

/*��Ƶ��ʾ֡*/
typedef struct hiVD_DISPLAY_FRAME_S
{
    HI_VOID*                    pVirAddr;           /*ͼƬ���������ַ*/
    HI_U32                      u32YAddr;           /*ͼƬ����Y���������ַ*/
    HI_U32                      u32CAddr;           /*ͼƬ����UV���������ַ*/
    HI_U32                      u32YStride;         /*ͼƬ�������ݿ��*/
    HI_U32                      u32CStride;         /*ͼƬɫ�����ݿ��*/
    HI_U32                      u32Width;           /*ͼƬ���*/
    HI_U32                      u32Height;          /*ͼƬ�߶�*/
    VD_FRAME_COMPRESS_TYPE_E    eCompressType;      /*֡��ѹ����ʽ��MJPGE����MPEG*/
    VD_COLOR_FORMAT_E           eColorFormat;       /*��ɫȡ������*/
    VD_ASPECT_RATIO_E           eAspectRatio;       /*ͼƬԭʼ���ͱ�*/
    VO_ENCODE_FORMAT_E          eEncodeFormat;      /*ͼƬԭʼ��ʽ*/
    VD_PIC_STRUCTURE_E          ePicStructure;      /*ͼƬ�ṹ����ͼ��֡ͼ*/
    VD_DISPLAY_DATA_TYPE_E      eDispDataType;      /*��ʾͼƬ��ȫ�����������ݡ��׳�����*/
    VD_DISPLAY_SEQUENCE_E       eDispSequence;      /*��ʾʱ��*/
    HI_U32                      u32DispTimes;       /*��֡����ʾ����*/
    HI_U32                      u32VOffset1;        /*��λƫ��*/
    HI_U32                      u32HOffset1;
    HI_U32                      u32VOffset2;
    HI_U32                      u32HOffset2;
    HI_U32                      u32VOffset3;
    HI_U32                      u32HOffset3;
}VD_DISPLAY_FRAME_S;

/* ��Ƶ��߳ߴ� */
typedef struct hiVD_FRAME_SIZE_S
{
    HI_U32                      u32Width;
    HI_U32                      u32Height;
}VD_FRAME_SIZE_S;


/*�˲�������*/
typedef struct hiVD_FILTER_CONFIG_S
{
    VD_FILTER_Y_VERTICAL_E      eYVerticalEn;           /*���ȴ�ֱ�˲�ʹ��*/
    HI_BOOL                     bCVerticalEn;           /*ɫ�ֱ�˲�ʹ��*/
    HI_BOOL                     bYHorizontalEn;         /*����ˮƽ�˲�ʹ��*/
    HI_BOOL                     bCHorizontalEn;         /*ɫ��ˮƽ�˲�ʹ��*/
    HI_BOOL                     bYVerticalMidianEn;     /*���ȴ�ֱ��ֵ�˲�ʹ��*/
    HI_BOOL                     bYHorizontalMidianEn;   /*����ˮƽ��ֵ�˲�ʹ��*/
    HI_BOOL                     bCHorizontalMidianEn;   /*ɫ��ˮƽ��ֵ�˲�ʹ��*/
}VD_FILTER_CONFIG_S;

/*�����������ɫ��ʽ*//*AI7D03104*/
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

/*��������*//*AI7D03104*/
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


