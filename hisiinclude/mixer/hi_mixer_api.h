/*******************************************************************************
 *              Copyright 2008 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_vo_api.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-01-22       z67193    HI_NULL      Create this file.
 ******************************************************************************/

#ifndef _MIXER_API_H_
#define _MIXER_API_H_

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_vo_errcode.h"
#include "hi_vo_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */


/****************************************************************************
*                            MIXER 部分                                     *
****************************************************************************/
/*****************************************************************************
* Function:      HI_MIXER_Reset
* Description:   复位MIXER
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_Reset(HI_VOID);

/*****************************************************************************
* Function:      HI_MIXER_SetBGColor
* Description:   设置背景层颜色
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetBGColor(VO_YCbCr_S *pColor);

/*****************************************************************************
* Function:      HI_MIXER_GetBGColor
* Description:   获取背景层颜色
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetBGColor(VO_YCbCr_S *pColor);

/*****************************************************************************
* Function:      HI_MIXER_SetPlaneOrder
* Description:   设置图层显示顺序
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_MIXER_ORDER          无效的图层次序
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetPlaneOrder(MIXER_PLANE_ORDER_E eOrder);

/*****************************************************************************
* Function:      HI_MIXER_GetPlaneOrder
* Description:   获取图层显示顺序
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetPlaneOrder(MIXER_PLANE_ORDER_E *pOrder);

/*****************************************************************************
* Function:      HI_MIXER_SetBrightness
* Description:   设置亮度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_MIXER_BRI            无效的亮度值
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetBrightness(HI_S32 s32Bri);

/*****************************************************************************
* Function:      HI_MIXER_GetBrightness
* Description:   获取亮度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetBrightness(HI_S32 *pBri);

/*****************************************************************************
* Function:      HI_MIXER_SetContrast
* Description:   设置对比度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_MIXER_CON            无效的对比度值
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetContrast(HI_S32 s32Con);

/*****************************************************************************
* Function:      HI_MIXER_GetContrast
* Description:   获取对比度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetContrast(HI_S32 *pCon);

/*****************************************************************************
* Function:      HI_MIXER_SetSaturation
* Description:   设置饱和度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_MIXER_SAT            无效的饱和度值
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetSaturation(HI_S32 s32Sat);

/*****************************************************************************
* Function:      HI_MIXER_GetSaturation
* Description:   获取饱和度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetSaturation(HI_S32 *pSat);


/*****************************************************************************
* Function:      HI_MIXER_EnableEdgeDeflicker
* Description:   使能边缘抗闪烁
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_EnableEdgeDeflicker(HI_VOID);


/*****************************************************************************
* Function:      HI_MIXER_DisableEdgeDeflicker
* Description:   关闭边缘抗闪烁
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_DisableEdgeDeflicker(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_VO_API_H_*/

