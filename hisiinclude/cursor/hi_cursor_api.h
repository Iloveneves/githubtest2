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
*                            CURSOR ����                                    *
****************************************************************************/
/*****************************************************************************
* Function:      HI_CURSOR_Reset
* Description:   ��λcursor
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_Reset(HI_VOID);

/*****************************************************************************
* Function:      HI_CURSOR_Show
* Description:   ��ʾcursor
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_CURSOR_NO_PARA       Ϊת������
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_Show(HI_VOID);

/*****************************************************************************
* Function:      HI_CURSOR_Hide
* Description:   ����cursor
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_Hide(HI_VOID);

/*****************************************************************************
* Function:      HI_CURSOR_InstallPara
* Description:   װ��cursor����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_CLUT_BEYOND          CLUT����Խ��
*                HI_ERR_VO_ALPHA                ͸������Ч
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_InstallPara(CURSOR_PARA_S *pPara);

/*****************************************************************************
* Function:      HI_CURSOR_SetClut
* Description:   װ�ص�ɫ��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_CLUT_BEYOND          CLUT����Խ��
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetClut(HI_U32 *pClut );

/*****************************************************************************
* Function:      HI_CURSOR_SetClutColor
* Description:   ���õ�ɫ����ĳһ����ɫ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_CLUT_BEYOND          CLUT����Խ��
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetClutColor(VO_CLUT_COLOR_S *pColor );

/*****************************************************************************
* Function:      HI_CURSOR_GetClutColor
* Description:   ��ȡ��ɫ����ĳһ����ɫ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
*                HI_ERR_VO_CLUT_BEYOND          CLUT����Խ��
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_GetClutColor(VO_CLUT_COLOR_S *pColor );

/*****************************************************************************
* Function:      HI_CURSOR_SetBitmap
* Description:   װ��cursorͼƬ����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetBitmap(HI_U32 *pBmp);

/*****************************************************************************
* Function:      HI_CURSOR_SetAlpha
* Description:   ����cursor͸����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_ALPHA                ͸������Ч
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetAlpha(HI_U32 u32Alpha);

/*****************************************************************************
* Function:      HI_CURSOR_GetAlpha
* Description:   ��ȡcursor͸����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_GetAlpha(HI_U32 *pAlpha);

/*****************************************************************************
* Function:      HI_CURSOR_SetPosition
* Description:   ����cursor��λ��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_SetPosition(CURSOR_POSITION_S *pPosition);    /*AI7D02559*/

/*****************************************************************************
* Function:      HI_CURSOR_GetPosition
* Description:   ��ȡcursor��λ��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_CURSOR_GetPosition(CURSOR_POSITION_S *pPosition);/*AI7D02559*/



#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_CURSOR_API_H_*/

