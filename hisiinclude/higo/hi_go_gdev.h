/**
 \file
 \brief Gdev模块头文件
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author w66592
 \date 2008-5-16
 */

#ifndef __HI_GO_GDEV_H__
#define __HI_GO_GDEV_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_surface.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/
/** @defgroup hi_go_gdev_data_structure Gdev模块 
 *  @ingroup H2 
 *  @brief 详细描述Gdev模块的数据结构和数据类型
 *  @{  */

/** 硬件图层ID的宏定义 */
/** 视频层 */
#define HIGO_LAYER_VIDEO0 0

/** 图形叠加层0 */
#define HIGO_LAYER_RGB0 1

/** 图形叠加层1 */
#define HIGO_LAYER_RGB1 2

/** 硬件鼠标层0 */
#define HIGO_LAYER_CURSOR0 3


/**图层的刷新方式*/
typedef enum 
{
    HIGO_LAYER_FLUSH_ANTIFLICKER = 0x01,     /**< 支持抗闪烁,不能单独存在必须和FLIP或DOUBBUFFER一起存在 */
    HIGO_LAYER_FLUSH_FLIP = 0x02,            /**< 支持flip */
    HIGO_LAYER_FLUSH_DOUBBUFER = 0x04,       /**< 支持双缓冲 */
    HIGO_LAYER_FLUSH_NORMAL = 0x08,          /**< 单BUFFER模式,用于调试环境,只能单独使用,不能和其他模式混用 */    
    HIGO_LAYER_FLUSH_OVER = 0x10,            /**< 更新过于频繁的时候丢帧还是直接覆盖，默认表示丢帧，OVER表示进行覆盖*/  
    HIGO_LAYER_FLUSH_BUTT
} HIGO_LAYER_FLUSHTYPE_E;

/**图层属性参数*/
typedef struct 
{
    HI_S32                 Width;          /**< 图层的虚拟高度，用于显存的创建，必须大于0 */
    HI_S32                 Height;         /**< 图层的虚拟宽度，用于显存的创建，必须大于0 */
    HIGO_LAYER_FLUSHTYPE_E LayerFlushType; /**< 图层的刷新方式，用户可根据使用场景选择不同的刷新模式来提高刷新效率,等于0时默认使用双缓冲+Flip刷新模式 */
    HI_U32                 FlipCount;      /**< 用做Flip的内存块数，小于2时默认为2 */
    HIGO_PF_E              PixelFormat;    /**< 图层的像素格式，此格式必须为硬件图层所支持的格式，请根据不同硬件设备来选择图层的像素格式参数 */
    HI_U32                 LayerID;        /**< 图层硬件ID，目前仅支持2个图形叠加层，即HIGO_LAYER_RGB0、HIGO_LAYER_RGB1 */
} HIGO_LAYER_INFO_S;

/**Z序调整的方式*/
typedef enum 
{
    HIGO_LAYER_ZORDER_MOVETOP = 0,  /**<移到最顶部*/
    HIGO_LAYER_ZORDER_MOVEUP,       /**<向上移到*/
    HIGO_LAYER_ZORDER_MOVEBOTTOM,   /**<移到最底部*/
    HIGO_LAYER_ZORDER_MOVEDOWN,     /**<向下移到*/
    HIGO_LAYER_ZORDER_BUTT
} HIGO_LAYER_ZORDER_E;


/** 图层状态结构 */
typedef struct 
{
    HI_BOOL bShow;             /**< 图层是否显示 */
    HI_BOOL bEnableCK;         /**< 图层是否使能colorkey */
} HIGO_LAYER_STATUS_S;

/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** @defgroup hi_go_gdev_api Gdev模块
*   @ingroup H1
*   @brief 详细描述Gdev模块的API函数
*   @{  */

/** 
\brief 初始化显示设备
\attention \n
::HI_GO_Init已包含了对该接口的调用
\param 无

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INITFAILED

\see \n
::HI_GO_Init \n
::HI_GO_DeinitDisplay
*/
HI_S32 HI_GO_InitDisplay(HI_VOID);

/** 
\brief 去初始化显示设备
\attention \n
::HI_GO_Deinit已包含了对该接口的调用
\param 无

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_Deinit \n
::HI_GO_InitDisplay
*/
HI_S32 HI_GO_DeinitDisplay(HI_VOID);

/** 
\brief 创建图层
\attention \n
无
\param[in]  pLayerInfo 图层基本属性，不可为空
\param[out] pLayer 图层的句柄指针，不可为空

\retval ::HIGO_ERR_NOTINIT 
\retval ::HIGO_ERR_NULLPTR 
\retval ::HIGO_ERR_INVSIZE 
\retval ::HIGO_ERR_INVLAYERID 
\retval ::HIGO_ERR_INVPIXELFMT 
\retval ::HIGO_ERR_INVFLUSHTYPE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_DestroyLayer
*/
HI_S32 HI_GO_CreateLayer (const HIGO_LAYER_INFO_S *pLayerInfo, HI_HANDLE* pLayer);

/** 
\brief 销毁图层
\attention \n
当图层正在被使用时（例如基于此图层的桌面未销毁），则图层无法被销毁
\param[in] Layer 图层句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_CLOSELAYERFAILED
\retval ::HIGO_ERR_FREEMEM
\retval ::HIGO_ERR_INUSE


\see \n
::HI_GO_CreateLayer
*/
HI_S32 HI_GO_DestroyLayer(HI_HANDLE Layer);


/** 
\brief 设置图层在屏幕中的位置
\attention \n
立即生效，无需刷新。当设置的图层的起始位置超出屏幕范围时会自动调整至屏幕的边界值
\param[in] Layer  图层句柄
\param[in] StartX 图层在屏幕上的显示位置X坐标
\param[in] StartY 图层在屏幕上的显示位置Y坐标

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVLAYERPOS
\retval ::HIGO_ERR_INTERNAL

\see \n
::HI_GO_GetLayerPos
*/
HI_S32 HI_GO_SetLayerPos(HI_HANDLE Layer, HI_U32 StartX, HI_U32 StartY);

/** 
\brief 获取图层在屏幕上的位置
\attention \n
无
\param[in] Layer 图层句柄
\param[out] pStartX 图层在屏幕上的显示位置X坐标指针，不可为空
\param[out] pStartY 图层在屏幕上的显示位置Y坐标指针，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INTERNAL

\see \n
::HI_GO_SetLayerPos
*/
HI_S32 HI_GO_GetLayerPos(HI_HANDLE Layer, HI_U32 *pStartX, HI_U32 *pStartY);


/** 
\brief 设置在输出显示设备上的输出的高度和宽度。
\attention \n
该接口只能进行缩小操作
\param[in]  Layer 图层句柄
\param[in] SWidth  实际输出宽度，默认为0表示与虚拟宽度相同,否则会对虚拟宽度进行缩小,按照实际宽度进行输出
\param[in] SHeight 实际输出高度，默认为0表示与虚拟高度相同,否则会对虚拟高度进行缩小,按照实际高度进行输出 
\retval none

\see \n
::HI_GO_GetScreenSize
*/
HI_S32 HI_GO_SetScreenSize(HI_HANDLE Layer, HI_U32 SWidth, HI_U32 SHeight);


/** 
\brief 获取在输出显示设备上的输出的高度和宽度。
\attention \n
该接口只能进行缩小操作,并且只有在双缓冲模式和抗闪烁模式下有效。
\param[in]  Layer 图层句柄
\param[out] pSWidth  实际输出宽度指针，默认为0表示与虚拟宽度相同,否则会对虚拟宽度进行缩小,按照实际宽度进行输出
\param[out] pSHeight 实际输出高度指针，默认为0表示与虚拟高度相同,否则会对虚拟高度进行缩小,按照实际高度进行输出 
\retval none

\see \n
::HI_GO_SetScreenSize
*/
HI_S32 HI_GO_GetScreenSize(HI_HANDLE Layer, HI_U32 *pSWidth, HI_U32 *pSHeight);

/** 
\brief 设置图层surface的全局alpha值
\attention \n
立即生效，无需刷新
\param[in] Layer 图层句柄
\param[in] Alpha 全局alpha值，不可为空，范围0-255

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_SETALPHAFAILED
\retval ::HIGO_ERR_INTERNAL

\see \n
::HI_GO_GetLayerAlpha
*/

HI_S32 HI_GO_SetLayerAlpha(HI_HANDLE Layer, HI_U8 Alpha);

/** 
\brief 获取图层surface的全局alpha值
\attention \n
无
\param[in] Layer 图层句柄
\param[out] Alpha 全局alpha指针

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetLayerAlpha
*/
HI_S32 HI_GO_GetLayerAlpha(HI_HANDLE Layer, HI_U8* Alpha);

/** 
\brief 改变图层的Z序
\attention \n
该功能需要硬件支持Z序的修改，立即生效，无需刷新
\param[in] Layer 图层句柄
\param[in] ZFlag 修改Z序标志

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVORDERFLAG
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_CANNOTCHANGE

\see \n
::HI_GO_GetLayerZorder
*/
HI_S32 HI_GO_ChangeLayerZorder(HI_HANDLE Layer, HIGO_LAYER_ZORDER_E ZFlag);

/** 
\brief 获取图层的Z序
\attention \n
Z序越小的图层越靠下
\param[in] Layer 图层句柄
\param[out] pZOrder 图层在硬件层上的Z序指针，不可为空 

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_ChangeLayerZorder
*/
HI_S32 HI_GO_GetLayerZorder(HI_HANDLE Layer, HI_U32 *pZOrder);

/** 
\brief 获取图层的surface
\attention \n
图层surface，不能使用HI_GO_FreeSurface来释放。只有在销毁图曾的时候才会被释放
\param[in] Layer 图层句柄
\param[out] pSurface surface句柄指针，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/
HI_S32 HI_GO_GetLayerSurface(HI_HANDLE Layer, HI_HANDLE *pSurface);

/** 
\brief 显示或隐藏图层
\attention \n
立即生效,无需刷新
\param[in] Layer 图层句柄
\param[in] bVisbile 显示隐藏标志。HI_TRUE：显示；HI_FALSE：隐藏

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_ALREADYSHOW
\retval ::HIGO_ERR_ALREADYHIDE
\retval ::HIGO_ERR_INTERNAL

\see \n
::HI_GO_GetLayerStatus
*/
HI_S32 HI_GO_ShowLayer(HI_HANDLE Layer, HI_BOOL bVisbile);

/** 
\brief 获取图层当前状态
\attention \n
无
\param[in] Layer 图层句柄
\param[out] pLayerStatus 图层当前状态结构指针，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_ShowLayer
*/
HI_S32 HI_GO_GetLayerStatus(HI_HANDLE Layer, HIGO_LAYER_STATUS_S* pLayerStatus);

/** 
\brief 刷新图层
\attention \n
绘制完成后，需刷新图层才能显示绘制后结果。
注意此接口调用频度过高，有可以返回失败，用户可以不用对错误码进行处理。
\param[in] Layer 图层句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INTERNAL

\see \n
无
*/
HI_S32 HI_GO_RefreshLayer(HI_HANDLE Layer);


/** @} */  /*! <!-- API declaration end */


#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_GDEV_H__ */
