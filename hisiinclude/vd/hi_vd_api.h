/*******************************************************************************
 *              Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_vd_api.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-03-19       z67193    HI_NULL      Create this file.
 ******************************************************************************/

#ifndef _VD_API_H_
#define _VD_API_H_

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_vo_errcode.h"
#include "hi_vo_type.h"
#include "hi_vd_errcode.h"
#include "hi_vd_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */


/****************************************************************************
*                                                                           *
*                   VIDEO DIAPLAY API 函数声明                              *
*                                                                           *
****************************************************************************/

/*****************************************************************************
* Function:      HI_VD_Init
* Description:   初始化视频层
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_Init(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_Exit
* Description:   退出视频层
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_Exit(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_EnableDisplay
* Description:   使能视频层显示
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_EnableDisplay(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_DisableDisplay
* Description:   关闭视频层显示
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_DisableDisplay(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_DisableDisplay
* Description:   获取视频层开关状态
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetDisplayStatus(VD_DISPLAY_STATUS_E *pStatus);


/*****************************************************************************
* Function:      HI_VD_SetVideoSource
* Description:   设置视频数据源
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetVideoSource(VD_FRAME_SOURCE_E eSource);


/*****************************************************************************
* Function:      HI_VD_GetVideoSource
* Description:   查询视频数据源
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetVideoSource(VD_FRAME_SOURCE_E *pSource);


/*****************************************************************************
* Function:      HI_VD_ShowFrame
* Description:   将一个显示帧送显
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_ShowFrame(VD_DISPLAY_FRAME_S *pFrame);


/*****************************************************************************
* Function:      HI_VD_SetInputWindow
* Description:   设置输入窗口
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetInputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_GetInputWindow
* Description:   查询输入窗口
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetInputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_SetOutputWindow
* Description:   设置输出窗口
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetOutputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_SetOutputWindow
* Description:   查询输出窗口
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOutputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_GetRealOutputWindow
* Description:   查询真正的输出窗口尺寸
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        用户截屏时查询
*****************************************************************************/
HI_S32 HI_VD_GetRealOutputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_SetOutputAspectRatio
* Description:   设置输出幅型比
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetOutputAspectRatio(VD_ASPECT_RATIO_E eRatio);


/*****************************************************************************
* Function:      HI_VD_GetOutputAspectRatio
* Description:   查询输出幅型比
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOutputAspectRatio(VD_ASPECT_RATIO_E* pRatio);


/*****************************************************************************
* Function:      HI_VD_GetOriginalAspectRatio
* Description:   查询显示帧原始幅型比
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOriginalAspectRatio(VD_ASPECT_RATIO_E* pRatio);


/*****************************************************************************
* Function:      HI_VD_SetAspectConversionMode
* Description:   查询幅型比转换模式
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetAspectConversionMode(VD_ASPECT_CONVERSION_MODE_E eMode);


/*****************************************************************************
* Function:      HI_VD_GetAspectConversionMode
* Description:   查询幅型比转换模式
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetAspectConversionMode(VD_ASPECT_CONVERSION_MODE_E* pMode);


/*****************************************************************************
* Function:      HI_VD_GetScreenWidth
* Description:   查询屏幕宽度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetScreenWidth(HI_U32 *pWidth);


/*****************************************************************************
* Function:      HI_VD_GetScreenHeight
* Description:   查询屏幕高度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetScreenHeight(HI_U32 *pHeight);


/*****************************************************************************
* Function:      HI_VD_GetOriginalVideoSize
* Description:   查询显示帧原始宽高尺寸
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOriginalVideoSize(VD_FRAME_SIZE_S *pSize);


/*****************************************************************************
* Function:      HI_VD_GetEncodingFormat
* Description:   查询码流制式
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOriginalVideoFormat(VO_ENCODE_FORMAT_E* pFormat);


/*****************************************************************************
* Function:      HI_VD_SetAlpha
* Description:   设置视频层的透明度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetAlpha(HI_U32 u32Alpha);


/*****************************************************************************
* Function:      HI_VD_GetAlpha
* Description:   设置视频层的透明度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetAlpha(HI_U32 *pAlpha);


/*****************************************************************************
* Function:      HI_VD_EnableAntiflicker
* Description:   使能抗闪烁
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_EnableDeflicker(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_DisableAntiflicker
* Description:   关闭抗闪烁
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_DisableDeflicker(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_SetFilter
* Description:   使能抗闪烁
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetFilter(VD_FILTER_CONFIG_S *pFilter);


/*****************************************************************************
* Function:      HI_VD_GetFilter
* Description:   使能抗闪烁
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetFilter(VD_FILTER_CONFIG_S *pFilter);


/*****************************************************************************
* Function:      HI_VD_CapturePicture
* Description:   截屏
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VD_CapturePicture(VD_CAPTURE_PICTURE_S *pCapture);





#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_VD_API_H_*/

