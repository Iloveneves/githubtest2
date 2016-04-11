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

/*JPEGͼƬ����*/
typedef enum hiJPG_IMGTYPE_E
{
    JPG_IMGTYPE_NORMAL = 0, /*��ͨJPEGͼƬ*/
    JPG_IMGTYPE_MOV_MJPEGA, /*MOV Motion JPG A*/
    JPG_IMGTYPE_MOV_MJPEGB, /*MOV Motion JPG B*/
    JPG_IMGTYPE_AVI_MJPEG,  /*AVI Motion JPG*/
    JPG_IMGTYPE_BUTT
} JPG_IMGTYPE_E;

/*JPGԭͼ�ı�������*/
typedef enum hiJPG_SOURCEFMT_E
{
    JPG_SOURCE_COLOR_FMT_YCBCR400,    /*YUV400*/
    JPG_SOURCE_COLOR_FMT_YCBCR420 = 3,    /*YUV420*/
    JPG_SOURCE_COLOR_FMT_YCBCR422BHP, /*YUV 422 2x1*/  
    JPG_SOURCE_COLOR_FMT_YCBCR422BVP, /*YUV 422 1x2*/  
    JPG_SOURCE_COLOR_FMT_YCBCR444,    /*YUV 444*/  
    JPG_SOURCE_COLOR_FMT_BUTT         /*������֧�ֵĸ�ʽ*/
}JPG_SOURCEFMT_E;

/*JPG֧�ֵ������ɫģʽ����*/
typedef enum hiJPG_OUTTYPE_E
{
    JPG_OUTTYPE_INTERLEAVE = 0,  /*��֯���ͣ�ָRGB��YUV��֯����*/
    JPG_OUTTYPE_MACROBLOCK,      /*������ͣ�ָY������CbCr�����Ƿֿ���ŵ�*/
    JPG_OUTTYPE_BUTT
}JPG_OUTTYPE_E;

/* ��֯���͵���ɫ��ʽ */
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

/* ������͵���ɫ��ʽ */
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

/*JPEGͼ�������࣬��ǰֻ֧��baseline��*/
typedef enum hiJPG_PICTYPE_E
{
    JPG_PICTYPE_BASELINE = 0, /*baseline����*/
    JPG_PICTYPE_EXTENDED,     /*Extended����*/
    JPG_PICTYPE_PROGRESSIVE,  /*Progressive����*/
    JPG_PICTYPE_LOSSLESS,     /*Lossless����*/
    JPG_PICTYPE_BUTT          /*������֧�ֵ�����*/
}JPG_PICTYPE_E;

/*JPGһ��ͼ����Ϣ*/
typedef struct hiJPG_PICINFO_S
{
    JPG_PICTYPE_E Profile;        /*�������*/
    HI_U32 Width;                 /*���*/
    HI_U32 Height;                /*�߶�*/
    HI_U32 SamplePrecision;       /*��������*/
    HI_U32 ComponentNum;          /*������*/
    JPG_SOURCEFMT_E EncodeFormat; /*�����ʽ*/
}JPG_PICINFO_S;

/*JPGͼƬ��Ϣ*/
typedef struct hiJPG_PRIMARYINFO_S
{    
    HI_U32         Count;    /*ͼ��������0��Ϊ��ͼ��Ϣ������Ϊ����ͼ*/    
    JPG_PICINFO_S* pPicInfo; /*ͼ��Ϣ*/
}JPG_PRIMARYINFO_S;

/* �û����õ�λͼ��Ϣ�ṹ */
typedef struct hiJPG_ILSURFACE_S
{
    JPG_COLORFMT_E ColorFmt;    /* ��ɫ��ʽ */
    HI_U32 PhyAddr;             /* λͼ�׵�ַ */ 
    HI_VOID* VirtAddr;          /* λͼ�����ַ��������õ� */
    HI_U32 Height;              /* λͼ�߶� */
    HI_U32 Width;               /* λͼ��� */ 
    HI_U32 Stride;              /* λͼ��� */
    HI_U8* pClutPhyAddr;        /*Clut���ַ,������ɫ��չ����ɫУ��*/ 
    HI_BOOL AlphaMax255;        /* λͼalpha���ֵΪ255����128 */ 
    HI_BOOL SubbyteAlign;       /* ���ش�С����byte��ʽ,��λ��Ӧ���Ҳ�����*/
    HI_U8 Alpha0;               /* Alpha0��Alpha1ֵ������ARGB1555��ʽ */
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

/*������λͼ�����Ϣ*/
typedef struct hiJPG_SURFACE_S
{
    JPG_OUTTYPE_E  OutType;        /*������ͣ���Ϊ��֯�ͺ������*/
    union
    {
        JPG_ILSURFACE_S Surface;   /*��֯����λͼ�洢��Ϣ*/
        JPG_MBSURFACE_S MbSurface; /*�������λͼ�洢��Ϣ*/
    }SurfaceInfo;
}JPG_SURFACE_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_HI_JPG_TYPE_H_*/

