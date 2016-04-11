/*******************************************************************************
 *              Copyright 2008 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_cursor_api.h
 * Description: 
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-01-22       z67193    HI_NULL      Create this file.
 ******************************************************************************/

#ifndef _CURSOR_API_H_
#define _CURSOR_API_H_

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
*                            CURSOR 部分                                    *
****************************************************************************/
/*****************************************************************************
* Function:      HI_CURSOR_Reset
* Description:   复位cursor
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_Reset(HI_VOID);

/*****************************************************************************
* Function:      HI_CURSOR_Show
* Description:   显示cursor
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_CURSOR_NO_PARA       为转载数据
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_Show(HI_VOID);

/*****************************************************************************
* Function:      HI_CURSOR_Hide
* Description:   隐藏cursor
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_Hide(HI_VOID);

/*****************************************************************************
* Function:      HI_CURSOR_InstallPara
* Description:   装载cursor参数
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_CLUT_BEYOND          CLUT表长度越界
*                HI_ERR_VO_ALPHA                透明度无效
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_InstallPara(CURSOR_PARA_S *pPara);

/*****************************************************************************
* Function:      HI_CURSOR_SetClut
* Description:   装载调色板
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_CLUT_BEYOND          CLUT表长度越界
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetClut(HI_U32 *pClut );

/*****************************************************************************
* Function:      HI_CURSOR_SetClutColor
* Description:   设置调色板中某一个颜色
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_CLUT_BEYOND          CLUT表长度越界
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetClutColor(VO_CLUT_COLOR_S *pColor );

/*****************************************************************************
* Function:      HI_CURSOR_GetClutColor
* Description:   获取调色板中某一个颜色
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
*                HI_ERR_VO_CLUT_BEYOND          CLUT表长度越界
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_GetClutColor(VO_CLUT_COLOR_S *pColor );

/*****************************************************************************
* Function:      HI_CURSOR_SetBitmap
* Description:   装载cursor图片数据
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetBitmap(HI_U32 *pBmp);

/*****************************************************************************
* Function:      HI_CURSOR_SetAlpha
* Description:   设置cursor透明度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_ALPHA                透明度无效
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetAlpha(HI_U32 u32Alpha);

/*****************************************************************************
* Function:      HI_CURSOR_GetAlpha
* Description:   获取cursor透明度
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_GetAlpha(HI_U32 *pAlpha);

/*****************************************************************************
* Function:      HI_CURSOR_SetPosition
* Description:   设置cursor的位置
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetPosition(CURSOR_POSITION_S *pPosition);    /*AI7D02559*/

/*****************************************************************************
* Function:      HI_CURSOR_GetPosition
* Description:   获取cursor的位置
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                    成功
*                HI_ERR_VO_NO_OPEN              VO设备未打开
*                HI_ERR_VO_PTR_NULL             空指针错误
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_GetPosition(CURSOR_POSITION_S *pPosition);/*AI7D02559*/



#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_CURSOR_API_H_*/

