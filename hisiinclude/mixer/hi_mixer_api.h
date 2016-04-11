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
*                            MIXER ����                                     *
****************************************************************************/
/*****************************************************************************
* Function:      HI_MIXER_Reset
* Description:   ��λMIXER
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_Reset(HI_VOID);

/*****************************************************************************
* Function:      HI_MIXER_SetBGColor
* Description:   ���ñ�������ɫ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetBGColor(VO_YCbCr_S *pColor);

/*****************************************************************************
* Function:      HI_MIXER_GetBGColor
* Description:   ��ȡ��������ɫ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetBGColor(VO_YCbCr_S *pColor);

/*****************************************************************************
* Function:      HI_MIXER_SetPlaneOrder
* Description:   ����ͼ����ʾ˳��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_MIXER_ORDER          ��Ч��ͼ�����
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetPlaneOrder(MIXER_PLANE_ORDER_E eOrder);

/*****************************************************************************
* Function:      HI_MIXER_GetPlaneOrder
* Description:   ��ȡͼ����ʾ˳��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetPlaneOrder(MIXER_PLANE_ORDER_E *pOrder);

/*****************************************************************************
* Function:      HI_MIXER_SetBrightness
* Description:   ��������
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_MIXER_BRI            ��Ч������ֵ
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetBrightness(HI_S32 s32Bri);

/*****************************************************************************
* Function:      HI_MIXER_GetBrightness
* Description:   ��ȡ����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetBrightness(HI_S32 *pBri);

/*****************************************************************************
* Function:      HI_MIXER_SetContrast
* Description:   ���öԱȶ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_MIXER_CON            ��Ч�ĶԱȶ�ֵ
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetContrast(HI_S32 s32Con);

/*****************************************************************************
* Function:      HI_MIXER_GetContrast
* Description:   ��ȡ�Աȶ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetContrast(HI_S32 *pCon);

/*****************************************************************************
* Function:      HI_MIXER_SetSaturation
* Description:   ���ñ��Ͷ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_MIXER_SAT            ��Ч�ı��Ͷ�ֵ
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_SetSaturation(HI_S32 s32Sat);

/*****************************************************************************
* Function:      HI_MIXER_GetSaturation
* Description:   ��ȡ���Ͷ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
*                HI_ERR_VO_PTR_NULL             ��ָ�����
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_GetSaturation(HI_S32 *pSat);


/*****************************************************************************
* Function:      HI_MIXER_EnableEdgeDeflicker
* Description:   ʹ�ܱ�Ե����˸
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_EnableEdgeDeflicker(HI_VOID);


/*****************************************************************************
* Function:      HI_MIXER_DisableEdgeDeflicker
* Description:   �رձ�Ե����˸
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_ERR_VO_NO_OPEN              VO�豸δ��
* Others:        
******************************************************************************/
HI_S32 HI_MIXER_DisableEdgeDeflicker(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_VO_API_H_*/

