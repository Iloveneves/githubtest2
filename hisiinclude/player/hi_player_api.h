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
*                       player API 函数声明                                 *
****************************************************************************/
/******************************************************************************
* Function:      HI_Player_Open
* Description:   打开播放器
* Input:         pFileName 文件名
* Output:        无
* Return:        HI_SUCCESS:                  成功
*                HI_ERR_JPG_PTR_NULL:         空指针
*                HI_ERR_PLAYER_INVALID_FILE:  文件错误
*                HI_ERR_PLAYER_NOSUPPORT_FMT: 不支持的文件格式
*                HI_ERR_PLAYER_CREATE_DECODE: 创建解码器失败
*                HI_ERR_PLAYER_NO_MEM:        内存不足
*                HI_FAILURE:                  其它系统错误
* Others:        同一时刻只能打开一个播放器
******************************************************************************/
HI_S32 HI_Player_Open(const HI_CHAR *pFileName);

/******************************************************************************
* Function:      HI_Player_Close
* Description:   关闭播放器
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:            成功
*                HI_ERR_PLAYER_NO_OPEN: 播放器未打开
* Others:        无
******************************************************************************/
HI_S32 HI_Player_Close(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Start
* Description:   启动播放
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                   成功
*                HI_ERR_PLAYER_NO_OPEN:        播放器未打开
*                HI_ERR_PLAYER_NO_MEM:         内存不足
*                HI_ERR_PLAYER_INVALID_STATUS: 错误的状态转换
*                HI_FAILURE:                   其它系统错误
* Others:        无
******************************************************************************/
HI_S32 HI_Player_Start(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Stop
* Description:   停止播放
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:               成功
*                HI_ERR_PLAYER_NO_OPEN:    播放器未打开
*                HI_FAILURE:               系统错误
* Others:        无
******************************************************************************/
HI_S32 HI_Player_Stop(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Pause
* Description:   暂停播放
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                   成功
*                HI_ERR_PLAYER_NO_OPEN:        播放器未打开
*                HI_ERR_PLAYER_INVALID_STATUS: 错误的状态转换
*                HI_FAILURE:                   其它系统错误
* Others:        无
******************************************************************************/
HI_S32 HI_Player_Pause(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Resume
* Description:   恢复播放
* Input:         无
* Output:        无
* Return:        HI_SUCCESS:                   成功
*                HI_ERR_PLAYER_NO_OPEN:        播放器未打开
*                HI_ERR_PLAYER_INVALID_STATUS: 错误的状态转换
*                HI_FAILURE:                   其它系统错误
* Others:        无
******************************************************************************/
HI_S32 HI_Player_Resume(HI_VOID);

/******************************************************************************
* Function:      HI_Player_Seek
* Description:   播放定位
* Input:         MsTime 需要定位到的时间，单位:毫秒
* Output:        无
* Return:        HI_SUCCESS:                   成功
*                HI_ERR_PLAYER_NO_OPEN:        播放器未打开
*                HI_ERR_PLAYER_OUT_RANGE:      超出影片范围
*                HI_FAILURE:                   其它系统错误
* Others:        无
******************************************************************************/
HI_S32 HI_Player_Seek(HI_U32 MsTime);

/******************************************************************************
* Function:      HI_Player_GetDuration
* Description:   获取媒体持续时长
* Input:         无
* Output:        pMsTime 时长，单位:毫秒
* Return:        HI_SUCCESS:                   成功
*                HI_ERR_PLAYER_NO_OPEN:        播放器未打开
*                HI_ERR_PLAYER_PTR_NULL:       空指针
* Others:        无
******************************************************************************/
HI_S32 HI_Player_GetDuration(HI_U32 *pMsTime);

/******************************************************************************
* Function:      HI_Player_GetCurrentTime
* Description:   获取媒体当前播放的时间点
* Input:         无
* Output:        pMsTime 时长，单位:毫秒
* Return:        HI_SUCCESS:                   成功
*                HI_ERR_PLAYER_NO_OPEN:        播放器未打开
*                HI_ERR_PLAYER_PTR_NULL:       空指针
* Others:        无
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
* Description:   获取媒体当前音频的波谱
* Input:         u32BandNum 频带数量(只支持20,80和512)
* Output:        pSpectrum 波形数据,频带能量，输出值范围在(0~96),单位dB
* Return:        HI_SUCCESS:                   成功
                 HI_FAILURE                    失败或频带数量非法
*                HI_ERR_PLAYER_NO_OPEN:        播放器未打开
*                HI_ERR_PLAYER_PTR_NULL:       空指针
* Others:        无
******************************************************************************/
HI_S32 HI_Player_GetAudioSpectrum(HI_U16 *pSpectrum, HI_U32 u32BandNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
#endif /*_MJPEGPLAYER_API_H_*/

