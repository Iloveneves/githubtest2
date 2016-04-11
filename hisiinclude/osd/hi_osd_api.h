/*******************************************************************************
 *              Copyright 2008 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_osd_api.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-01-22       z67193    HI_NULL      Create this file.
 ******************************************************************************/

#ifndef _OSD_API_H_
#define _OSD_API_H_

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_vo_errcode.h"
#include "hi_vo_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

/*****************************************************************************
* Function:      HI_OSD_Init
* Description:   初始化OSD
* Input:         eLayer                         要复位的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Init(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_Exit
* Description:   退出OSD
* Input:         eLayer                         要复位的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Exit(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_Reset
* Description:   复位OSD
* Input:         eLayer                         要复位的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Reset(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_Show
* Description:   显示OSD
* Input:         eLayer                         要显示的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Show(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_Hide
* Description:   隐藏OSD
* Input:         eLayer                         要隐藏的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_Hide(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_GetFrameBuffer
* Description:   隐藏OSD
* Input:         无
* Output:        pFrameBuffer                   显存结构指针
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        OSD初步方案为用户管理显存，后期修改为驱动管理显存时，可去掉
******************************************************************************/
HI_S32 HI_OSD_GetFrameBuffer(OSD_LAYER_E eLayer, 
                             OSD_FRAME_BUFFER_S *pFrameBuffer);

/*****************************************************************************
* Function:      HI_OSD_ReleaseFrameBuffer
* Description:   释放OSD的显存
* Input:         无
* Output:        eLayer                   
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
* Others:        
******************************************************************************/
HI_S32 HI_OSD_ReleaseFrameBuffer(OSD_LAYER_E eLayer);/*AI7D02566*/

/*****************************************************************************
* Function:      HI_OSD_SetScreenPara
* Description:   设置屏幕参数
* Input:         pScreenPara                    屏幕参数
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_SCREEN_SIZE      屏幕大小无效
*                HI_ERR_VO_OSD_WINDOW_SIZE      显示窗口无效
*                HI_ERR_VO_OSD_AR               幅型比无效
*                HI_ERR_VO_OSD_HAS_REGION       当前有region存在
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetScreenPara(OSD_LAYER_E eLayer, VO_RECTANGLE_S *pScreenPara);

/*****************************************************************************
* Function:      HI_OSD_GetScreenPara
* Description:   查询屏幕参数
* Input:         pScreenPara                    屏幕参数指针
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_OSD_GetScreenPara(OSD_LAYER_E eLayer, VO_RECTANGLE_S *pScreenPara);

/*****************************************************************************
* Function:      HI_OSD_SetOutputAspectRatio
* Description:   设置屏幕参数
* Input:         pScreenPara                    屏幕参数
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_SCREEN_SIZE      屏幕大小无效
*                HI_ERR_VO_OSD_WINDOW_SIZE      显示窗口无效
*                HI_ERR_VO_OSD_AR               幅型比无效
*                HI_ERR_VO_OSD_HAS_REGION       当前有region存在
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetOutputAspectRatio(OSD_LAYER_E eLayer, OSD_ASPECT_RATIO_E eRatio);

/*****************************************************************************
* Function:      HI_OSD_GetOutputAspectRatio
* Description:   设置屏幕参数
* Input:         pScreenPara                    屏幕参数
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_SCREEN_SIZE      屏幕大小无效
*                HI_ERR_VO_OSD_WINDOW_SIZE      显示窗口无效
*                HI_ERR_VO_OSD_AR               幅型比无效
*                HI_ERR_VO_OSD_HAS_REGION       当前有region存在
* Others:        
******************************************************************************/
HI_S32 HI_OSD_GetOutputAspectRatio(OSD_LAYER_E eLayer, OSD_ASPECT_RATIO_E *pRatio);

/*****************************************************************************
* Function:      HI_OSD_EnableP2N
* Description:   使能P2N功能
* Input:         eLayer                         要使能P2N的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableP2N(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_DisableP2N
* Description:   禁止P2N功能
* Input:         eLayer                         要禁止P2N的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableP2N(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_EnableDeflicker
* Description:   使能抗闪烁功能
* Input:         eLayer                         要使能抗闪烁的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableDeflicker(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_DisableDeflicker
* Description:   禁止抗闪烁功能
* Input:         eLayer                         要禁止抗闪烁的OSD的层次
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableDeflicker(OSD_LAYER_E eLayer);

/*****************************************************************************
* Function:      HI_OSD_CreateRegion
* Description:   创建region
* Input:         pRegionPara                    region参数指针
* Output:        pHandle                        region的句柄指针
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_LAYER    无效的OSD层次
*                HI_ERR_VO_OSD_INVALID_PICMOD   无效的颜色格式
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_BYTENOALIGN          未字节对齐
*                HI_ERR_VO_OSD_MEM_BEYOND       内存越界
*                HI_ERR_VO_OSD_REGION_SIZE      region大小无效
*                HI_ERR_VO_OSD_BEYOND           超出屏幕范围
*                HI_ERR_VO_OSD_OVERLAP          region之间有重叠
*                HI_ERR_VO_OSD_CLIPPING         裁剪区域无效
*                HI_ERR_VO_ALPHA                无效的透明度
*                HI_ERR_VO_OSD_COLORKEY         关键色的参数无效
*                HI_ERR_VO_OSD_REGION_NUM       region数目过多
* Others:        
******************************************************************************/
HI_S32 HI_OSD_CreateRegion(OSD_REGION_PARA_S *pRegionPara, 
                          OSD_REGION_HANDLE_T *pHandle);

/*****************************************************************************
* Function:      HI_OSD_DestroyRegion
* Description:   消除一个region
* Input:         Handle                         region的句柄
* Output:
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DestroyRegion(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_ResizeRegion
* Description:   重新定义region的大小
* Input:         Handle                         region的句柄
*                u32Width                       region的宽度
*                u32Height                      region的高度
* Output:        pBuffer                        新的buffer指针
*                pBufferPhy                     新的buffer物理指针
*                u32BufferSize                  新的buffer大小
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
*                HI_ERR_VO_OSD_OVERLAP          导致region重叠
* Others:        
******************************************************************************/
HI_S32 HI_OSD_ResizeRegion(OSD_REGION_HANDLE_T Handle, HI_U32 u32Width, 
                           HI_U32 u32Height, HI_U32 *pBuffer, HI_U32 pBufferPhy,
                           HI_U32 u32BufferSize);

/*****************************************************************************
* Function:      HI_OSD_ShowRegion
* Description:   显示region
* Input:         Handle                         region的句柄
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_ShowRegion(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_HideRegion
* Description:   隐藏region
* Input:         Handle                         region的句柄
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_HideRegion(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_SetClut
* Description:   重新装载clut表参数
* Input:         Handle                         region的句柄
*                pClutPhy                       CLUT表指针
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetClut(OSD_REGION_HANDLE_T Handle, HI_U32 *pClut);

/*****************************************************************************
* Function:      HI_OSD_SetBmpAddr
* Description:   重新指定图片数据缓冲区地址
* Input:         Handle                         region的句柄
*                pBmpAddr                       图片数据缓冲区的逻辑地址
*                u32BmpPhy                      图片数据缓冲区的物理地址
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetBmpAddr(OSD_REGION_HANDLE_T Handle, HI_U32 *pBmpVir, HI_U32 u32BmpPhy);

/*****************************************************************************
* Function:      HI_OSD_SetRegionPosition
* Description:   设置region的起始点坐标
* Input:         Handle                         region的句柄
*                pDisplayArea                   显示区域参数
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
*                HI_ERR_VO_OSD_BEYOND           超出屏幕范围
*                HI_ERR_VO_OSD_OVERLAP          region之间有重叠
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetRegionPosition(OSD_REGION_HANDLE_T Handle, 
                                VO_POSITION_S *pPosition);


/*****************************************************************************
* Function:      HI_OSD_EnableAlphaPremul
* Description:   启动REGION的预乘设置
* Input:         Handle                         region的句柄
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableAlphaPremul(OSD_REGION_HANDLE_T Handle);


/*****************************************************************************
* Function:      HI_OSD_DisableAlphaPremul
* Description:   关闭REGION的预乘设置
* Input:         Handle                         region的句柄
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableAlphaPremul(OSD_REGION_HANDLE_T Handle);


/*****************************************************************************
* Function:      HI_OSD_EnableClipArea
* Description:   启动Clipping功能
* Input:         Handle                         region的句柄
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableClipArea(OSD_REGION_HANDLE_T Handle);


/*****************************************************************************
* Function:      HI_OSD_DisableClipArea
* Description:   关闭Clipping功能
* Input:         Handle                         region的句柄
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableClipArea(OSD_REGION_HANDLE_T Handle);


/*****************************************************************************
* Function:      HI_OSD_SetClippingArea
* Description:   设置region的裁剪区域
* Input:         Handle                         region的句柄
*                sClipArea                      裁剪区域参数
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
*                HI_ERR_VO_OSD_BEYOND           裁剪区域越界
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetClipArea(OSD_REGION_HANDLE_T Handle, 
                          VO_RECTANGLE_S *pClipArea);

/*****************************************************************************
* Function:      HI_OSD_EnableColorKey
* Description:   启动ColorKey功能
* Input:         Handle                         region的句柄
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_EnableColorKey(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_DisableColorKey
* Description:   关闭ColorKey功能
* Input:         Handle                         region的句柄
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_DisableColorKey(OSD_REGION_HANDLE_T Handle);

/*****************************************************************************
* Function:      HI_OSD_SetColorKeyPara
* Description:   获取视频输入源
* Input:         Handle                         region的句柄
*                pColorSpace                    ColorSpace参数
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
*                HI_ERR_VO_OSD_COLORKEY         关键色参数不正确
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetColorKeyPara(OSD_REGION_HANDLE_T Handle, 
                              OSD_COLORKEY_S *pColorKey);

/*****************************************************************************
* Function:      HI_OSD_GetPixel
* Description:   获取单个像素颜色
* Input:         Handle                         region的句柄
*                pPosition                      位置参数
* Output:        pColor                         颜色指针
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
*                HI_ERR_VO_OSD_BEYOND           裁剪区域越界
* Others:        
******************************************************************************/
HI_S32 HI_OSD_GetPixel(OSD_REGION_HANDLE_T Handle, VO_POSITION_S *pPosition,
                       HI_U32 *pColor);

/*****************************************************************************
* Function:      HI_OSD_SetColorFormat
* Description:   设置颜色模式
* Input:         Handle                         region的句柄
*                eColorFormat                   颜色模式
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
*                HI_ERR_VO_OSD_INVALID_PICMOD   无效的颜色格式
* Others:        同样像素宽度可以设置
******************************************************************************/
HI_S32 HI_OSD_SetColorFormat(OSD_REGION_HANDLE_T Handle, 
                             OSD_COLOR_FORMAT_E eColorFormat);

/*****************************************************************************
* Function:      HI_OSD_SetAlpha
* Description:   设置region的全局透明度
* Input:         Handle                         region的句柄
*                u32Alpha                       透明度
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
*                HI_ERR_VO_ALPHA                无效的透明度
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetAlpha(OSD_REGION_HANDLE_T Handle, HI_U32 u32Alpha);

/*****************************************************************************
* Function:      HI_OSD_SetExtAlpha
* Description:   设置针对1555颜色模式的扩展透明度
* Input:         Handle                         region的句柄
*                u32ExtAlpha0                   1555首位为0时的透明度
*                u32ExtAlpha1                   1555首位为1时的透明度
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
*                HI_ERR_VO_ALPHA                无效的透明度
* Others:        
******************************************************************************/
HI_S32 HI_OSD_SetExtAlpha(OSD_REGION_HANDLE_T Handle, HI_U32 u32ExtAlpha0,
                          HI_U32 u32ExtAlpha1);

/*****************************************************************************
* Function:      HI_OSD_OperationSync
* Description:   同步更新图片数据操作
* Input:         Handle                         region的句柄
*                pOperationArea                 操作过的区域
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_OperationSync(OSD_REGION_HANDLE_T Handle, 
                            VO_RECTANGLE_S *pOperationArea, HI_BOOL bBlock);

/*****************************************************************************
* Function:      HI_OSD_IsSyncComplete
* Description:   查询同步更新图片数据操作是否已经完成，用于operationSync的异步机制
* Input:         Handle                         region的句柄
*                pbComplete                     是否已经完成
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_IsSyncComplete(OSD_REGION_HANDLE_T Handle, HI_BOOL *pbComplete);

/*****************************************************************************
* Function:      HI_OSD_GetRegionPara
* Description:   获取region的参数
* Input:         Handle                         region的句柄
* Output:        pRegionPara                    region的参数指针
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_OSD_INVALID_REGION   无效的region handle
* Others:        
******************************************************************************/
HI_S32 HI_OSD_GetRegionPara(OSD_REGION_HANDLE_T Handle, 
                            OSD_REGION_PARA_S *pRegionPara);



#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_VO_API_H_*/

