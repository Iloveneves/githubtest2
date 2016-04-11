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
*                   VIDEO DIAPLAY API ��������                              *
*                                                                           *
****************************************************************************/

/*****************************************************************************
* Function:      HI_VD_Init
* Description:   ��ʼ����Ƶ��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_Init(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_Exit
* Description:   �˳���Ƶ��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_Exit(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_EnableDisplay
* Description:   ʹ����Ƶ����ʾ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_EnableDisplay(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_DisableDisplay
* Description:   �ر���Ƶ����ʾ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_DisableDisplay(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_DisableDisplay
* Description:   ��ȡ��Ƶ�㿪��״̬
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetDisplayStatus(VD_DISPLAY_STATUS_E *pStatus);


/*****************************************************************************
* Function:      HI_VD_SetVideoSource
* Description:   ������Ƶ����Դ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetVideoSource(VD_FRAME_SOURCE_E eSource);


/*****************************************************************************
* Function:      HI_VD_GetVideoSource
* Description:   ��ѯ��Ƶ����Դ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetVideoSource(VD_FRAME_SOURCE_E *pSource);


/*****************************************************************************
* Function:      HI_VD_ShowFrame
* Description:   ��һ����ʾ֡����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_ShowFrame(VD_DISPLAY_FRAME_S *pFrame);


/*****************************************************************************
* Function:      HI_VD_SetInputWindow
* Description:   �������봰��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetInputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_GetInputWindow
* Description:   ��ѯ���봰��
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetInputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_SetOutputWindow
* Description:   �����������
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetOutputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_SetOutputWindow
* Description:   ��ѯ�������
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOutputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_GetRealOutputWindow
* Description:   ��ѯ������������ڳߴ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        �û�����ʱ��ѯ
*****************************************************************************/
HI_S32 HI_VD_GetRealOutputWindow(VO_WINDOW_S* pWindow);


/*****************************************************************************
* Function:      HI_VD_SetOutputAspectRatio
* Description:   ����������ͱ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetOutputAspectRatio(VD_ASPECT_RATIO_E eRatio);


/*****************************************************************************
* Function:      HI_VD_GetOutputAspectRatio
* Description:   ��ѯ������ͱ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOutputAspectRatio(VD_ASPECT_RATIO_E* pRatio);


/*****************************************************************************
* Function:      HI_VD_GetOriginalAspectRatio
* Description:   ��ѯ��ʾ֡ԭʼ���ͱ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOriginalAspectRatio(VD_ASPECT_RATIO_E* pRatio);


/*****************************************************************************
* Function:      HI_VD_SetAspectConversionMode
* Description:   ��ѯ���ͱ�ת��ģʽ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetAspectConversionMode(VD_ASPECT_CONVERSION_MODE_E eMode);


/*****************************************************************************
* Function:      HI_VD_GetAspectConversionMode
* Description:   ��ѯ���ͱ�ת��ģʽ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetAspectConversionMode(VD_ASPECT_CONVERSION_MODE_E* pMode);


/*****************************************************************************
* Function:      HI_VD_GetScreenWidth
* Description:   ��ѯ��Ļ���
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetScreenWidth(HI_U32 *pWidth);


/*****************************************************************************
* Function:      HI_VD_GetScreenHeight
* Description:   ��ѯ��Ļ�߶�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetScreenHeight(HI_U32 *pHeight);


/*****************************************************************************
* Function:      HI_VD_GetOriginalVideoSize
* Description:   ��ѯ��ʾ֡ԭʼ��߳ߴ�
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOriginalVideoSize(VD_FRAME_SIZE_S *pSize);


/*****************************************************************************
* Function:      HI_VD_GetEncodingFormat
* Description:   ��ѯ������ʽ
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetOriginalVideoFormat(VO_ENCODE_FORMAT_E* pFormat);


/*****************************************************************************
* Function:      HI_VD_SetAlpha
* Description:   ������Ƶ���͸����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetAlpha(HI_U32 u32Alpha);


/*****************************************************************************
* Function:      HI_VD_GetAlpha
* Description:   ������Ƶ���͸����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetAlpha(HI_U32 *pAlpha);


/*****************************************************************************
* Function:      HI_VD_EnableAntiflicker
* Description:   ʹ�ܿ���˸
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_EnableDeflicker(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_DisableAntiflicker
* Description:   �رտ���˸
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_DisableDeflicker(HI_VOID);


/*****************************************************************************
* Function:      HI_VD_SetFilter
* Description:   ʹ�ܿ���˸
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_SetFilter(VD_FILTER_CONFIG_S *pFilter);


/*****************************************************************************
* Function:      HI_VD_GetFilter
* Description:   ʹ�ܿ���˸
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_GetFilter(VD_FILTER_CONFIG_S *pFilter);


/*****************************************************************************
* Function:      HI_VD_CapturePicture
* Description:   ����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                    �ɹ�
*                HI_FAILURE:                    ϵͳ����
* Others:        
*****************************************************************************/
HI_S32 HI_VD_CapturePicture(VD_CAPTURE_PICTURE_S *pCapture);





#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*_VD_API_H_*/

