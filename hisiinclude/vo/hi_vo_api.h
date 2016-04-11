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

#ifndef _VO_API_H_
#define _VO_API_H_

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
*                                                                           *
*                   GRAPHICS API 函数声明                                   *
*                                                                           *
****************************************************************************/

/****************************************************************************
*                            OSD 部分                                       *
****************************************************************************/

/*****************************************************************************
* Function:      HI_VO_Open
* Description:   打开图形层设备
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
*                HI_ERR_VO_OPENED               VO设备已打开
* Others:        
*****************************************************************************/
HI_S32 HI_VO_Open(HI_VOID);


/******************************************************************************
* Function:      HI_VO_Close
* Description:   关闭图形层设备
* Input:         无
* Output:        无
* Return:        无
* Others:        
*****************************************************************************/
HI_S32 HI_VO_Close(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_SetOutputFormat
* Description:   设置输出制式
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        针对所有的图层
*****************************************************************************/
HI_S32 HI_VO_SetOutputFormatMode(VO_OUTPUT_FORMAT_E eFormat);


/*****************************************************************************
* Function:      HI_VD_GetOutputFormat
* Description:   查询输出制式
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VO_GetOutputFormatMode(VO_OUTPUT_FORMAT_E* pFormat);


/*****************************************************************************
* Function:      HI_VO_GetRealOutputFormat
* Description:   查询实际输出制式
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        用于设置为输出制式自适应时查询输出制式
*****************************************************************************/
HI_S32 HI_VO_GetRealOutputFormat(VO_ENCODE_FORMAT_E* pFormat);


/*****************************************************************************
* Function:      HI_VD_SetOutputInterface
* Description:   设置信号输出端口
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VO_SetOutputInterface(VO_OUTPUT_INTERFACE_E eInterface);


/*****************************************************************************
* Function:      HI_VD_GetOutputInterface
* Description:   查询信号输出端口状态
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VO_GetOutputInterface(VO_OUTPUT_INTERFACE_E *pInterface);

/*****************************************************************************
* Function:      HI_VO_SetDacMode
* Description:   设置信号输出端口
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VO_SetDacMode(VO_DAC_ATTR_S *pDacMode);

/*****************************************************************************
* Function:      HI_VO_GetDacMode
* Description:   查询信号输出端口状态
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_FAILURE:                    系统错误
* Others:        
*****************************************************************************/
HI_S32 HI_VO_GetDacMode(VO_DAC_ATTR_S *pDacMode);
/*FOR TEST*/
HI_S32 HI_VO_ReadReg( VO_REGISTER_S *pReg );
HI_S32 HI_VO_WriteReg( VO_REGISTER_S *pReg);
HI_S32 HI_VO_SET_LOG_LEVEL(LOG_ERRLEVEL_E Level);
HI_S32 HI_VO_SET_LOG_MODULE(HI311X_MODULE_E Module);
HI_S32 HI_VO_CLEAR_LOG_MODULE(HI311X_MODULE_E Module);


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_VO_API_H_*/

