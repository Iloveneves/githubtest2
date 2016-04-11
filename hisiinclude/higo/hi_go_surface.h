/**
 \file
 \brief Surface模块头文件
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
/** @defgroup hi_go_surface_error_code Surface模块
 *  @ingroup H3 
 *  @brief 详细描述Surface模块的错误码
 *  @{  */

typedef enum
{
    HIGO_PF_CLUT8 = 0,  /**< 调色板 */
    HIGO_PF_CLUT1,
    HIGO_PF_CLUT4,
    HIGO_PF_4444,       /**< 一个像素占16bit，ARGB每分量占4bit，按地址由高至低排列 */
    HIGO_PF_0444,       /**< 一个像素占16bit，ARGB每分量占4bit，按地址由高至低排列, A分量不起作用 */

    HIGO_PF_1555,       /**< 一个像素占16bit，RGB每分量占5bit，A分量1bit，按地址由高至低排列 */
    HIGO_PF_0555,       /**< 一个像素占16bit，RGB每分量占5bit，A分量1bit，按地址由高至低排列, A分量不起作用 */
    HIGO_PF_565,        /**< 一个像素占16bit，RGB每分量分别占5bit、6bit和5bit，按地址由高至低排列 */
    HIGO_PF_8565,       /**< 一个像素占24bit，ARGB每分量分别占8bit、5bit、6bit和5bit，按地址由高至低排列，暂不支持*/
    HIGO_PF_8888,       /**< 一个像素占32bit，ARGB每分量占8bit，按地址由高至低排列 */
    HIGO_PF_0888,       /**< 一个像素占32bit，ARGB每分量占8bit，按地址由高至低排列，A分量不起作用 */

    HIGO_PF_YUV400,     /**< 海思定义的semi-planar YUV 400格式 */    
    HIGO_PF_YUV420,     /**< 海思定义的semi-planar YUV 420格式 */
    HIGO_PF_YUV422,     /**< 海思定义的semi-planar YUV 422格式  水平采样格式*/
    HIGO_PF_YUV422_V,   /**< 海思定义的semi-planar YUV 422格式  垂直采样格式*/    
    HIGO_PF_YUV444,     /**< 海思定义的semi-planar YUV 444格式 */

	HIGO_PF_A1, 
	HIGO_PF_A8,
    HIGO_PF_BUTT
} HIGO_PF_E;

/** 分量类型 */
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
    HIGO_PDFORMAT_E Format;     /**< 分量类型 */
    HI_VOID*        pData;      /**< 分量虚拟地址指针 */
    HI_VOID*        pPhyData;   /**< 分量物理地址指针 */    
    HI_U32          Pitch;      /**< 分量行宽 */
    HI_U32          Bpp;        /**< bytes per pixel */
    HI_U32          Offset;
} HIGO_PIXELDATA_S;

typedef struct
{
    HI_S32 Width;               /**< surface宽度 */
    HI_S32 Height;              /**< surface高度 */
    HIGO_PF_E PixelFormat;      /**< surface像素格式*/
    HI_U32 Pitch;               /**< surface行间距离*/
    HI_CHAR* pPhyAddr;          /**< surface物理地址*/
    HI_CHAR* pVirAddr;          /**< surface虚拟地址*/
}HIGO_SURINFO_S;

/** 最大数据分量数 */
#define MAX_PARTPIXELDATA 3

/** 数据分量结构 */
typedef HIGO_PIXELDATA_S HI_PIXELDATA[MAX_PARTPIXELDATA];

/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** @defgroup hi_go_surface_api Surface模块
 *  @ingroup H1
 *  @brief 详细描述Surface模块的API函数
 *  @{  */

/** 
\brief 设置surface的alpha值
\attention \n
无
\param[in]  Surface Surface句柄
\param[in]  Alpha Alpha值，范围是0-255。0表示透明，255表示不透明

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetSurfaceAlpha
*/
HI_S32 HI_GO_SetSurfaceAlpha(HI_HANDLE Surface, HI_U8 Alpha);

/** 
\brief 获取surface的alpha值
\attention \n
无
\param[in]  Surface Surface句柄
\param[out] pAlpha 接收alhpa值的空间指针，不能为空

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_SetSurfaceAlpha
*/
HI_S32 HI_GO_GetSurfaceAlpha(HI_HANDLE Surface, HI_U8* pAlpha);

/** 
\brief 设置是否使能surface的colorkey
\attention \n
无
\param[in] Surface Surface句柄
\param[in] Enable 是否使能colorKey。HI_TRUE：使能；HI_FALSE：不使能

\retval ::HI_SUCCESS 

\see \n
无
*/
HI_S32 HI_GO_EnableSurfaceColorKey(HI_HANDLE Surface, HI_BOOL Enable);

/** 
\brief 设置surface的colorKey值
\attention \n
无
\param[in] Surface Surface句柄
\param[in] ColorKey Colorkey值

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetSurfaceColorKey
*/
HI_S32 HI_GO_SetSurfaceColorKey(HI_HANDLE Surface, HI_COLOR ColorKey);

/** 
\brief 获取surface的colorkey值
\attention \n
无
\param[in]  Surface Surface句柄
\param[out] pColorKey 接收colorkey值的空间指针，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_NOCOLORKEY

\see \n
::HI_GO_SetSurfaceColorKey
*/
HI_S32 HI_GO_GetSurfaceColorKey(HI_HANDLE Surface, HI_COLOR* pColorKey);

/** 
\brief 设置Surface的调色板
\attention \n
无
\param[in] Surface Surface句柄
\param[in] Palette 调色板

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVSURFACEPF

\see \n
::HI_GO_GetSurfaceColorKey
*/
HI_S32 HI_GO_SetSurfacePalette(HI_HANDLE Surface, const HI_PALETTE Palette);

/** 
\brief 获取surface的调色板
\attention \n
无
\param[in]  Surface Surface句柄
\param[out] Palette 接收调色板的空间指针 

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVSURFACEPF

\see \n
::HI_GO_SetSurfacePalette
*/
HI_S32 HI_GO_GetSurfacePalette(HI_HANDLE Surface, HI_PALETTE Palette);

/** 
\brief 锁定surface，获取其内存指针
\attention \n
访问surface内容前需要调用该接口锁定surface \n
不能对同一surface重复锁定
\param[in] Surface Surface句柄
\param[out] pData 与内存访问相关的像素格式信息

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
::HI_GO_UnlockSurface
*/
HI_S32 HI_GO_LockSurface(HI_HANDLE Surface, HI_PIXELDATA pData);

/** 
\brief 解锁定surface
\attention \n
对surface内容访问结束后，要及时调用该接口解锁定surface
\param[in] Surface Surface句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOTLOCKED

\see \n
::HI_GO_LockSurface
*/
HI_S32 HI_GO_UnlockSurface(HI_HANDLE Surface);

/** 
\brief 获取surface尺寸
\attention \n
pWidth与pHeight不能同时为空
\param[in]  Surface Surface句柄
\param[out]  pWidth Surface宽度输出地址
\param[out]  pHeight Surface高度输出地址

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR 

\see \n
无
*/
HI_S32 HI_GO_GetSurfaceSize(HI_HANDLE Surface, HI_S32* pWidth, HI_S32* pHeight);

/** 
\brief 获取surface像素格式
\attention \n
无
\param[in]  Surface Surface句柄
\param[out] pPixelFormat 像素格式输出地址，不可为空

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
无
*/
HI_S32 HI_GO_GetSurfacePixelFormat(HI_HANDLE Surface, HIGO_PF_E* pPixelFormat);

/** 
\brief 将用户的内存封装成surface
\attention \n
pSurInfo所有属性都必须设置正确。
\param[in]  pSurInfo 用户内存信息
\param[out] 句柄信息

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
无
*/
HI_S32 HI_GO_CreateSurfaceFromMem(HIGO_SURINFO_S *pSurInfo, HI_HANDLE * pSurface);


/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_SURFACE_H__ */
