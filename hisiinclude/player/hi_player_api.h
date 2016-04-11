/*******************************************************************************
 *              Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_player_api.h
 * Description:
 *
 * History:
 * Version   Date             Author    DefectNum    Description
 * main\1    2008-03-22       d37024    HI_NULL      Create this file.
 ******************************************************************************/
#ifndef _PLAYER_API_H_
#define _PLAYER_API_H_

#include "hi_type.h"
#include "hi_player_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

/****************************************************************************
*                       player API ��������                                 *
****************************************************************************/
/******************************************************************************
* Function:      HI_Player_Open
* Description:   �򿪲�����
* Input:         pFileName �ļ���
* Output:        ��
* Return:        HI_SUCCESS:                  �ɹ�
*                HI_ERR_JPG_PTR_NULL:         ��ָ��
*                HI_ERR_PLAYER_INVALID_FILE:  �ļ�����
*                HI_ERR_PLAYER_NOSUPPORT_FMT: ��֧�ֵ��ļ���ʽ
*                HI_ERR_PLAYER_CREATE_DECODE: ����������ʧ��
*                HI_ERR_PLAYER_NO_MEM:        �ڴ治��
*                HI_FAILURE:                  ����ϵͳ����
* Others:        ͬһʱ��ֻ�ܴ�һ��������
******************************************************************************/
HI_S32 HI_Player_Open(const HI_CHAR *pFileName);

/******************************************************************************
* Function:      HI_Player_Close
* Description:   �رղ�����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:            �ɹ�
*                HI_ERR_PLAYER_NO_OPEN: ������δ��
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_Close(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Start
* Description:   ��������
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                   �ɹ�
*                HI_ERR_PLAYER_NO_OPEN:        ������δ��
*                HI_ERR_PLAYER_NO_MEM:         �ڴ治��
*                HI_ERR_PLAYER_INVALID_STATUS: �����״̬ת��
*                HI_FAILURE:                   ����ϵͳ����
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_Start(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Stop
* Description:   ֹͣ����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:               �ɹ�
*                HI_ERR_PLAYER_NO_OPEN:    ������δ��
*                HI_FAILURE:               ϵͳ����
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_Stop(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Pause
* Description:   ��ͣ����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                   �ɹ�
*                HI_ERR_PLAYER_NO_OPEN:        ������δ��
*                HI_ERR_PLAYER_INVALID_STATUS: �����״̬ת��
*                HI_FAILURE:                   ����ϵͳ����
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_Pause(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Resume
* Description:   �ָ�����
* Input:         ��
* Output:        ��
* Return:        HI_SUCCESS:                   �ɹ�
*                HI_ERR_PLAYER_NO_OPEN:        ������δ��
*                HI_ERR_PLAYER_INVALID_STATUS: �����״̬ת��
*                HI_FAILURE:                   ����ϵͳ����
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_Resume(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Seek
* Description:   ���Ŷ�λ
* Input:         MsTime ��Ҫ��λ����ʱ�䣬��λ:����
* Output:        ��
* Return:        HI_SUCCESS:                   �ɹ�
*                HI_ERR_PLAYER_NO_OPEN:        ������δ��
*                HI_ERR_PLAYER_OUT_RANGE:      ����ӰƬ��Χ
*                HI_FAILURE:                   ����ϵͳ����
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_Seek(HI_U32 MsTime);

/******************************************************************************
* Function:      HI_Player_GetDuration
* Description:   ��ȡý�����ʱ��
* Input:         ��
* Output:        pMsTime ʱ������λ:����
* Return:        HI_SUCCESS:                   �ɹ�
*                HI_ERR_PLAYER_NO_OPEN:        ������δ��
*                HI_ERR_PLAYER_PTR_NULL:       ��ָ��
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_GetDuration(HI_U32 *pMsTime);

/******************************************************************************
* Function:      HI_Player_GetCurrentTime
* Description:   ��ȡý�嵱ǰ���ŵ�ʱ���
* Input:         ��
* Output:        pMsTime ʱ������λ:����
* Return:        HI_SUCCESS:                   �ɹ�
*                HI_ERR_PLAYER_NO_OPEN:        ������δ��
*                HI_ERR_PLAYER_PTR_NULL:       ��ָ��
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_GetCurrentTime(HI_U32 *pMsTime);


HI_S32  HI_Mp3Player_Open(const MP3_SRCDESC_S *pSrcDesc);
HI_S32  HI_Mp3Player_Close(HI_VOID);
HI_S32  HI_Mp3Player_GetBuffer(HI_U32 u32ReqLen, MP3_BUF_S *pBuf, HI_U32 u32TimeOut);
HI_S32  HI_Mp3Player_PutBuffer(HI_U32 u32ValidLen);
HI_S32  HI_Mp3Player_Start(HI_VOID);
HI_S32  HI_Mp3Player_Stop(HI_VOID);
HI_S32  HI_Mp3Player_Pause(HI_VOID);
HI_S32  HI_Mp3Player_Resume(HI_VOID);
//HI_S32  HI_Mp3Player_SendStream(HI_U8* pData, HI_U32 u32DataLen, HI_U32 *pCopyLen);
HI_S32  HI_Mp3Player_Seek(HI_U32 MsTime);
HI_S32  HI_Mp3Player_GetDuration(HI_U32 *pMsTime);
HI_S32  HI_Mp3Player_GetCurrentTime(HI_U32 *pMsTime);

/******************************************************************************
* Function:      HI_Player_GetAudioSpectrum
* Description:   ��ȡý�嵱ǰ��Ƶ�Ĳ���
* Input:         u32BandNum Ƶ������(ֻ֧��20,80��512)
* Output:        pSpectrum ��������,Ƶ�����������ֵ��Χ��(0~96),��λdB
* Return:        HI_SUCCESS:                   �ɹ�
                 HI_FAILURE                    ʧ�ܻ�Ƶ�������Ƿ�
*                HI_ERR_PLAYER_NO_OPEN:        ������δ��
*                HI_ERR_PLAYER_PTR_NULL:       ��ָ��
* Others:        ��
******************************************************************************/
HI_S32 HI_Player_GetAudioSpectrum(HI_U16 *pSpectrum, HI_U32 u32BandNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
#endif /*_MJPEGPLAYER_API_H_*/

