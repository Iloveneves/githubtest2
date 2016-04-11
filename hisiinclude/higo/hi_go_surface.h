/**
 \file
 \brief Surfaceģ��ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-5-13
 */

#ifndef __HI_GO_SURFACE_H__
#define __HI_GO_SURFACE_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
/** @defgroup hi_go_surface_error_code Surfaceģ��
 *  @ingroup H3 
 *  @brief ��ϸ����Surfaceģ��Ĵ�����
 *  @{  */

typedef enum
{
    HIGO_PF_CLUT8 = 0,  /**< ��ɫ�� */
    HIGO_PF_CLUT1,
    HIGO_PF_CLUT4,
    HIGO_PF_4444,       /**< һ������ռ16bit��ARGBÿ����ռ4bit������ַ�ɸ��������� */
    HIGO_PF_0444,       /**< һ������ռ16bit��ARGBÿ����ռ4bit������ַ�ɸ���������, A������������ */

    HIGO_PF_1555,       /**< һ������ռ16bit��RGBÿ����ռ5bit��A����1bit������ַ�ɸ��������� */
    HIGO_PF_0555,       /**< һ������ռ16bit��RGBÿ����ռ5bit��A����1bit������ַ�ɸ���������, A������������ */
    HIGO_PF_565,        /**< һ������ռ16bit��RGBÿ�����ֱ�ռ5bit��6bit��5bit������ַ�ɸ��������� */
    HIGO_PF_8565,       /**< һ������ռ24bit��ARGBÿ�����ֱ�ռ8bit��5bit��6bit��5bit������ַ�ɸ��������У��ݲ�֧��*/
    HIGO_PF_8888,       /**< һ������ռ32bit��ARGBÿ����ռ8bit������ַ�ɸ��������� */
    HIGO_PF_0888,       /**< һ������ռ32bit��ARGBÿ����ռ8bit������ַ�ɸ��������У�A������������ */

    HIGO_PF_YUV400,     /**< ��˼�����semi-planar YUV 400��ʽ */    
    HIGO_PF_YUV420,     /**< ��˼�����semi-planar YUV 420��ʽ */
    HIGO_PF_YUV422,     /**< ��˼�����semi-planar YUV 422��ʽ  ˮƽ������ʽ*/
    HIGO_PF_YUV422_V,   /**< ��˼�����semi-planar YUV 422��ʽ  ��ֱ������ʽ*/    
    HIGO_PF_YUV444,     /**< ��˼�����semi-planar YUV 444��ʽ */

	HIGO_PF_A1, 
	HIGO_PF_A8,
    HIGO_PF_BUTT
} HIGO_PF_E;

/** �������� */
typedef enum
{
    HIGO_PDFORMAT_RGB = 0,
    HIGO_PDFORMAT_RGB_PM,   /**< pre-multiplied */
    HIGO_PDFORMAT_Y,
    HIGO_PDFORMAT_UV,
    HIGO_PDFORMAT_UV_LE,    

    HIGO_PDFORMAT_BUTT
} HIGO_PDFORMAT_E;

typedef struct
{
    HIGO_PDFORMAT_E Format;     /**< �������� */
    HI_VOID*        pData;      /**< ���������ַָ�� */
    HI_VOID*        pPhyData;   /**< ���������ַָ�� */    
    HI_U32          Pitch;      /**< �����п� */
    HI_U32          Bpp;        /**< bytes per pixel */
    HI_U32          Offset;
} HIGO_PIXELDATA_S;

typedef struct
{
    HI_S32 Width;               /**< surface��� */
    HI_S32 Height;              /**< surface�߶� */
    HIGO_PF_E PixelFormat;      /**< surface���ظ�ʽ*/
    HI_U32 Pitch;               /**< surface�м����*/
    HI_CHAR* pPhyAddr;          /**< surface�����ַ*/
    HI_CHAR* pVirAddr;          /**< surface�����ַ*/
}HIGO_SURINFO_S;

/** ������ݷ����� */
#define MAX_PARTPIXELDATA 3

/** ���ݷ����ṹ */
typedef HIGO_PIXELDATA_S HI_PIXELDATA[MAX_PARTPIXELDATA];

/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** @defgroup hi_go_surface_api Surfaceģ��
 *  @ingroup H1
 *  @brief ��ϸ����Surfaceģ���API����
 *  @{  */

/** 
\brief ����surface��alphaֵ
\attention \n
��
\param[in]  Surface Surface���
\param[in]  Alpha Alphaֵ����Χ��0-255��0��ʾ͸����255��ʾ��͸��

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetSurfaceAlpha
*/
HI_S32 HI_GO_SetSurfaceAlpha(HI_HANDLE Surface, HI_U8 Alpha);

/** 
\brief ��ȡsurface��alphaֵ
\attention \n
��
\param[in]  Surface Surface���
\param[out] pAlpha ����alhpaֵ�Ŀռ�ָ�룬����Ϊ��

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_SetSurfaceAlpha
*/
HI_S32 HI_GO_GetSurfaceAlpha(HI_HANDLE Surface, HI_U8* pAlpha);

/** 
\brief �����Ƿ�ʹ��surface��colorkey
\attention \n
��
\param[in] Surface Surface���
\param[in] Enable �Ƿ�ʹ��colorKey��HI_TRUE��ʹ�ܣ�HI_FALSE����ʹ��

\retval ::HI_SUCCESS 

\see \n
��
*/
HI_S32 HI_GO_EnableSurfaceColorKey(HI_HANDLE Surface, HI_BOOL Enable);

/** 
\brief ����surface��colorKeyֵ
\attention \n
��
\param[in] Surface Surface���
\param[in] ColorKey Colorkeyֵ

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetSurfaceColorKey
*/
HI_S32 HI_GO_SetSurfaceColorKey(HI_HANDLE Surface, HI_COLOR ColorKey);

/** 
\brief ��ȡsurface��colorkeyֵ
\attention \n
��
\param[in]  Surface Surface���
\param[out] pColorKey ����colorkeyֵ�Ŀռ�ָ�룬����Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_NOCOLORKEY

\see \n
::HI_GO_SetSurfaceColorKey
*/
HI_S32 HI_GO_GetSurfaceColorKey(HI_HANDLE Surface, HI_COLOR* pColorKey);

/** 
\brief ����Surface�ĵ�ɫ��
\attention \n
��
\param[in] Surface Surface���
\param[in] Palette ��ɫ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVSURFACEPF

\see \n
::HI_GO_GetSurfaceColorKey
*/
HI_S32 HI_GO_SetSurfacePalette(HI_HANDLE Surface, const HI_PALETTE Palette);

/** 
\brief ��ȡsurface�ĵ�ɫ��
\attention \n
��
\param[in]  Surface Surface���
\param[out] Palette ���յ�ɫ��Ŀռ�ָ�� 

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVSURFACEPF

\see \n
::HI_GO_SetSurfacePalette
*/
HI_S32 HI_GO_GetSurfacePalette(HI_HANDLE Surface, HI_PALETTE Palette);

/** 
\brief ����surface����ȡ���ڴ�ָ��
\attention \n
����surface����ǰ��Ҫ���øýӿ�����surface \n
���ܶ�ͬһsurface�ظ�����
\param[in] Surface Surface���
\param[out] pData ���ڴ������ص����ظ�ʽ��Ϣ

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
::HI_GO_UnlockSurface
*/
HI_S32 HI_GO_LockSurface(HI_HANDLE Surface, HI_PIXELDATA pData);

/** 
\brief ������surface
\attention \n
��surface���ݷ��ʽ�����Ҫ��ʱ���øýӿڽ�����surface
\param[in] Surface Surface���

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOTLOCKED

\see \n
::HI_GO_LockSurface
*/
HI_S32 HI_GO_UnlockSurface(HI_HANDLE Surface);

/** 
\brief ��ȡsurface�ߴ�
\attention \n
pWidth��pHeight����ͬʱΪ��
\param[in]  Surface Surface���
\param[out]  pWidth Surface��������ַ
\param[out]  pHeight Surface�߶������ַ

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR 

\see \n
��
*/
HI_S32 HI_GO_GetSurfaceSize(HI_HANDLE Surface, HI_S32* pWidth, HI_S32* pHeight);

/** 
\brief ��ȡsurface���ظ�ʽ
\attention \n
��
\param[in]  Surface Surface���
\param[out] pPixelFormat ���ظ�ʽ�����ַ������Ϊ��

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
��
*/
HI_S32 HI_GO_GetSurfacePixelFormat(HI_HANDLE Surface, HIGO_PF_E* pPixelFormat);

/** 
\brief ���û����ڴ��װ��surface
\attention \n
pSurInfo�������Զ�����������ȷ��
\param[in]  pSurInfo �û��ڴ���Ϣ
\param[out] �����Ϣ

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
��
*/
HI_S32 HI_GO_CreateSurfaceFromMem(HIGO_SURINFO_S *pSurInfo, HI_HANDLE * pSurface);


/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_SURFACE_H__ */
