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
*                   GRAPHICS API ��������                                   *
*                                                                           *
****************************************************************************/

/****************************************************************************
*                            OSD ����                                       *
****************************************************************************/

/*****************************************************************************
* Function:      HI_VO_Open
* Description:   ��ͼ�β��豸
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
*                HI_ERR_VO_OPENED               VO�豸�Ѵ�
* Others:        
*****************************************************************************/
HI_S32 HI_VO_Open(HI_VOID);


/******************************************************************************
* Function:      HI_VO_Close
* Description:   �ر�ͼ�β��豸
* Input:         ��
* Output:        ��
* Return:        ��
* Others:        
*****************************************************************************/
HI_S32 HI_VO_Close(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_SetOutputFormat
* Description:   ���������ʽ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        ������е�ͼ��
*****************************************************************************/
HI_S32 HI_VO_SetOutputFormatMode(VO_OUTPUT_FORMAT_E eFormat);


/*****************************************************************************
* Function:      HI_VD_GetOutputFormat
* Description:   ��ѯ�����ʽ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VO_GetOutputFormatMode(VO_OUTPUT_FORMAT_E* pFormat);


/*****************************************************************************
* Function:      HI_VO_GetRealOutputFormat
* Description:   ��ѯʵ�������ʽ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        ��������Ϊ�����ʽ����Ӧʱ��ѯ�����ʽ
*****************************************************************************/
HI_S32 HI_VO_GetRealOutputFormat(VO_ENCODE_FORMAT_E* pFormat);


/*****************************************************************************
* Function:      HI_VD_SetOutputInterface
* Description:   �����ź�����˿�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VO_SetOutputInterface(VO_OUTPUT_INTERFACE_E eInterface);


/*****************************************************************************
* Function:      HI_VD_GetOutputInterface
* Description:   ��ѯ�ź�����˿�״̬
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VO_GetOutputInterface(VO_OUTPUT_INTERFACE_E *pInterface);

/*****************************************************************************
* Function:      HI_VO_SetDacMode
* Description:   �����ź�����˿�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VO_SetDacMode(VO_DAC_ATTR_S *pDacMode);

/*****************************************************************************
* Function:      HI_VO_GetDacMode
* Description:   ��ѯ�ź�����˿�״̬
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
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

