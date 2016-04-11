/**
 \file
 \brief Winc模块头文件
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-8-5
 */

#ifndef __HI_GO_WINC_H__
#define __HI_GO_WINC_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/

/** @defgroup hi_go_winc_data_structure Winc模块
 *  @ingroup H2
 *  @brief 详细描述WM模块的数据结构和数据类型
 *  @{  */

/**窗口Z序调整方式*/
typedef enum
{
    HIGO_ZORDER_MOVETOP = 0,  /**<移到最顶部*/
    HIGO_ZORDER_MOVEUP,       /**<向上移*/
    HIGO_ZORDER_MOVEBOTTOM,   /**<移到最底部*/
    HIGO_ZORDER_MOVEDOWN,     /**<向下移*/
    HIGO_ZORDER_BUTT
} HIGO_ZORDER_E;
#if 0
/** 窗口类型 */
typedef enum 
{
    HIGO_WINSTYLE_NORMAL = 0,  /**< 一般窗口 */ 
    HIGO_WINSTYLE_TOPMOST,     /**< 顶层窗口 */
    HIGO_WINSTYLE_BUTT
} HIGO_WINSTYLE_E;
#endif
/** 最大窗口层数 */
#define HIGO_WINTREE_NUM 16

/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/** @defgroup hi_go_winc_api WM模块
 *  @ingroup H1
 *  @brief 详细描述WM模块的API函数
 *  @{  */

/**
\brief 创建桌面
\attention \n
一个图层只能创建一个桌面
\param[in] Layer 图层句柄
\param[out] pDesktop 桌面句柄的地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_ALREADYBIND

\see \n
::HI_GO_DestroyDesktop
 */
HI_S32 HI_GO_CreateDesktop(HI_HANDLE Layer, HI_HANDLE *pDesktop);

/**
\brief 销毁桌面
\attention \n
无
\param[in] Desktop 桌面句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateDesktop
*/
HI_S32 HI_GO_DestroyDesktop(HI_HANDLE Desktop);

/**
\brief 在指定桌面上创建窗口
\attention \n
当桌面的像素格式带有像素alpha时，窗口的surface可以选择是否使用像素alpha
\param[in] Desktop  桌面句柄
\param[in] pRect 矩形窗口区域，为空表示整个桌面区域
\param[in] LayerNum 窗口所处的层号,序号为0-15。
\param[in] bHasPixelAlpha 窗口是否带像素alpha
\param[out] pWindow  存储窗口句柄的地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_DestroyWindow
*/
HI_S32 HI_GO_CreateWindow(HI_HANDLE Desktop, const HI_RECT* pRect, 
                                    HI_U32 LayerNum, HI_BOOL bHasPixelAlpha,
                                    HI_HANDLE* pWindow);


/**
\brief 销毁窗口
\attention \n
新创建的窗口始终在最上面
\param[in] Window 待销毁窗口句柄
\param[in] LayerNum 窗口所处的层号

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_CreateWindow
*/
HI_S32 HI_GO_DestroyWindow(HI_HANDLE Window, HI_U32 LayerNum);

/**
\brief 获取窗口矩形
\attention \n
无
\param[in] Window 窗口句柄
\param[out] pRect  窗口矩形地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetWindowPos \n
::HI_GO_ResizeWindow
*/
HI_S32 HI_GO_GetWindowRect(HI_HANDLE Window, HI_RECT* pRect);

/**
\brief 设置窗口在绑定桌面上的起始位置
\attention \n
无
\param[in] Window 窗口句柄
\param[in] StartX X坐标
\param[in] StartY Y坐标

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_SetWindowPos(HI_HANDLE Window, HI_S32 StartX, HI_S32 StartY);

/**
\brief 改变窗口大小
\attention \n
改变窗口大小后，窗口需要重绘
\param[in] Window 窗口句柄
\param[in] Width 窗口宽度
\param[in] Height 窗口高度

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_NOMEM

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_ResizeWindow(HI_HANDLE Window, HI_S32 Width, HI_S32 Height);

/**
\brief 获取窗口Z序
\attention \n
无
\param[in] Window 窗口句柄
\param[in] LayerNum 窗口所处的层号
\param[out] pZOrder 存储窗口Z序的地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_ChangeWindowZOrder
*/
HI_S32 HI_GO_GetWindowZOrder(HI_HANDLE Window, HI_U32 LayerNum, HI_U32* pZOrder);

/**
\brief 更改窗口Z序
\attention \n
无
\param[in] Window 窗口句柄
\param[in] EnType 窗口Z序调整方式
\param[in] LayerNum 窗口所处的层号

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVZORDERTYPE
\retval ::HIGO_ERR_LOCKED

\see \n
::HI_GO_GetWindowZOrder
*/
HI_S32 HI_GO_ChangeWindowZOrder(HI_HANDLE Window, HIGO_ZORDER_E EnType, HI_U32 LayerNum);

/**
\brief 获取窗口可见度
\attention \n
无
\param[in] Window 窗口句柄
\param[out] pOpacity 存储窗口不透明度的地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetWindowOpacity
*/
HI_S32 HI_GO_GetWindowOpacity(HI_HANDLE Window, HI_U8* pOpacity);

/**
\brief 设置窗口不透明度
\attention \n
无
\param[in] Window 窗口句柄
\param[in] Opacity 窗口不透明度

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
::HI_GO_GetWindowOpacity
*/
HI_S32 HI_GO_SetWindowOpacity(HI_HANDLE Window, HI_U8 Opacity);

/**
\brief 设置窗口的colorkey
\attention \n
设置colorkey操作不能与其他操作同时进行，例如窗口可见度不为0xFF时无法设置窗口的colorkey
\param[in] Window 窗口句柄
\param[in] ColorKey Colorkey值

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
无
*/
HI_S32 HI_GO_SetWindowColorkey(HI_HANDLE Window, HI_COLOR ColorKey);

/**
\brief 获取窗口的surface句柄
\attention \n
无
\param[in] Window   窗口句柄
\param[in] pSurface 存储窗口surface句柄的地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
无
*/
HI_S32 HI_GO_GetWindowSurface(HI_HANDLE Window, HI_HANDLE* pSurface);

/**
\brief 刷新窗口
\attention \n
更新窗口surface内容之后需要调用此接口才能生效
\param[in] Window 窗口句柄
\param[in] pRect 更新的窗口区域

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/
HI_S32 HI_GO_UpdateWindow(HI_HANDLE Window, HI_RECT* pRect);

/**
\brief 设置桌面的背景颜色
\attention \n
无
\param[in] Desktop 桌面句柄
\param[in] Color 桌面背景颜色

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/
HI_S32 HI_GO_SetDesktopBGColor(HI_HANDLE Desktop, HI_COLOR Color);

/**
\brief 将窗口移至另一个桌面中
\attention \n
若两个桌面的像素格式不一致，移动窗口后需要重绘窗口
\param[in] Window 窗口句柄
\param[in] Desktop 目标桌面句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_NOMEM

\see \n
无
*/
HI_S32 HI_GO_MoveWindow(HI_HANDLE Window, HI_HANDLE Desktop);

/**
\brief 刷新桌面
\attention \n
无
\param[in] Desktop 桌面句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/
HI_S32 HI_GO_RefreshDesktop(HI_HANDLE Desktop);


/**
\brief 窗口加锁
\attention \n
无
\param[in] Window 窗口句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/

HI_S32 HI_GO_LockWindow(HI_HANDLE Window);

/**
\brief 窗口解锁
\attention \n
无
\param[in] Window 窗口句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
无
*/

HI_S32 HI_GO_UnlockWindow(HI_HANDLE Window);

/**
\brief 释放窗口surface
\attention \n
此接口的目的是为了在隐藏窗口的时候节省内存而释放窗口的内存资源而设置的。
\param[in] Window 窗口句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
HI_GO_RebuildWindowSurface
*/
HI_S32 HI_GO_FreeWindowSurface(HI_HANDLE Window);

/**
\brief 重建窗口surface
\attention \n
无
\param[in] Window 窗口句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
HI_GO_FreeWindowSurface
*/
HI_S32 HI_GO_RebuildWindowSurface(HI_HANDLE Window);


/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_WINC_H__ */
