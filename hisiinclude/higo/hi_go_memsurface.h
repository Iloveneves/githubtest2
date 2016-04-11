/**
 \file
 \brief MemSurfaceģ��ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-5-15
 */

#ifndef __HI_GO_MEMSURFACE_H__
#define __HI_GO_MEMSURFACE_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_surface.h"
#include "hi_go_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** @defgroup hi_go_memsurface_api MemSurfaceģ��
 *  @ingroup H1
 *  @brief ��ϸ����MemSurfaceģ���API����
 *  @{  */

/** 
\brief ��ʼ��MemSurface
\attention \n
::HI_GO_Init�Ѿ������Ըýӿڵĵ���
\param ��

\retval ::HI_SUCCESS

\see \n
::HI_GO_DeinitMemSurface
*/
HI_S32 HI_GO_InitMemSurface(HI_VOID);

/** 
\brief ȥ��ʼ��MemSurface
\attention \n
::HI_GO_Deinit�Ѿ������Ըýӿڵĵ���
\param ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_InitMemSurface
*/
HI_S32 HI_GO_DeinitMemSurface(HI_VOID);

/** 
\brief �����ڴ�surface
\attention \n
���ܴ���::HIGO_PF_CLUT1��::HIGO_PF_CLUT4��::HIGO_PF_YUV420��::HIGO_PF_YUV422��ʽ��surface
\param[in] Width Surface���
\param[in] Height Surface�߶�
\param[in] PixelFormat Surface���ظ�ʽ
\param[out] pSurface Surface���

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVSURFACESIZE
\retval ::HIGO_ERR_INVSURFACEPF
\retval ::HIGO_ERR_NOMEM

\see \n
::HI_GO_FreeSurface
*/
HI_S32 HI_GO_CreateSurface(HI_S32 Width, HI_S32 Height, HIGO_PF_E PixelFormat, HI_HANDLE* pSurface);

/** 
\brief ������surface����surface���丸surface����ͬһ���ڴ�
\attention \n
��

\param[in] Surface ��surface���
\param[in] pRect ��surface�ڸ�surface�е�����
\param[out] pSubSurface ��surface���

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_OUTOFBOUNDS
\return HI_S32

\see \n
::HI_GO_FreeSurface
*/
HI_S32 HI_GO_CreateSubSurface(HI_HANDLE Surface, const HI_RECT *pRect, HI_HANDLE* pSubSurface);


/** 
\brief ����surface
\attention \n
��
\param[in] Surface Surface���

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateSurface \n
::HI_GO_DecImgData
*/
HI_S32 HI_GO_FreeSurface(HI_HANDLE Surface);

/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_MEMSURFACE_H__ */
