/**
 \file
 \brief MemSurface模块头文件
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
/** @defgroup hi_go_memsurface_api MemSurface模块
 *  @ingroup H1
 *  @brief 详细描述MemSurface模块的API函数
 *  @{  */

/** 
\brief 初始化MemSurface
\attention \n
::HI_GO_Init已经包含对该接口的调用
\param 无

\retval ::HI_SUCCESS

\see \n
::HI_GO_DeinitMemSurface
*/
HI_S32 HI_GO_InitMemSurface(HI_VOID);

/** 
\brief 去初始化MemSurface
\attention \n
::HI_GO_Deinit已经包含对该接口的调用
\param 无

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_InitMemSurface
*/
HI_S32 HI_GO_DeinitMemSurface(HI_VOID);

/** 
\brief 创建内存surface
\attention \n
不能创建::HIGO_PF_CLUT1、::HIGO_PF_CLUT4、::HIGO_PF_YUV420、::HIGO_PF_YUV422格式的surface
\param[in] Width Surface宽度
\param[in] Height Surface高度
\param[in] PixelFormat Surface像素格式
\param[out] pSurface Surface句柄

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
\brief 创建子surface，子surface与其父surface共享同一块内存
\attention \n
无

\param[in] Surface 父surface句柄
\param[in] pRect 子surface在父surface中的区域
\param[out] pSubSurface 子surface句柄

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
\brief 销毁surface
\attention \n
无
\param[in] Surface Surface句柄

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
