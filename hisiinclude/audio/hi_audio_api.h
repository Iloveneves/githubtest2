/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : Audio_api.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2006/7/18
Last Modified :
Description   :
Function List :
History       :
 ******************************************************************************/


#ifndef __AUDIO_API_H__
#define __AUDIO_API_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_audio_errcode.h"
#include "hi_audio_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Audio device open and close operation */
HI_S32 HI_AUD_Open(HI_VOID);
HI_S32 HI_AUD_Close(HI_VOID);
/* Create and destory audio player */
HI_S32 HI_AUD_CreatePlayer(AUD_PLAYER_ATTR_S  sAttr, AUD_PLAYER_HANDLE *pHandle);
HI_S32 HI_AUD_DestroyPlayer(AUD_PLAYER_HANDLE hPlayer);
HI_S32 HI_AUD_StartPlayer(AUD_PLAYER_HANDLE hPlayer);
HI_S32 HI_AUD_StopPlayer(AUD_PLAYER_HANDLE hPlayer);
HI_S32 HI_AUD_PausePlayer(AUD_PLAYER_HANDLE hPlayer);
/* AI7D03062 add by l00129785*/
HI_S32 HI_AUD_GetAudState(HI_VOID);
/* AI7D03062 add by l0129785*/
/* AI7D03062 add by q65271 */
HI_S32 HI_AUD_PausePlayer_Pvr(AUD_PLAYER_HANDLE hPlayer);
/* AI7D03062 add by q65271 */
HI_S32 HI_AUD_ResumePlayer(AUD_PLAYER_HANDLE hPlayer);
HI_S32 HI_AUD_GetPlayerState(AUD_PLAYER_HANDLE hPlayer,AUD_PLAYER_STATE_E *penState);
HI_S32 HI_AUD_SetPlayerInputDataEndian(AUD_PLAYER_HANDLE hPlayer,AUD_DATA_ENDIAN_E enEndian);
HI_S32 HI_AUD_GetPlayerInputDataEndian(AUD_PLAYER_HANDLE hPlayer,AUD_DATA_ENDIAN_E *penEndian);
HI_S32 HI_AUD_GetPlayerOriginalTrackMode(AUD_PLAYER_HANDLE hPlayer, AUD_ORIGINAL_TRACK_MODE_E *penMode);
HI_S32 HI_AUD_GetPlayerOriginalSampleFrequency(AUD_PLAYER_HANDLE hPlayer,AUD_SAMPLE_FREQUENCE_E *penSampleFrequency);
HI_S32 HI_AUD_SetPlayerOriginalSampleFrequency(AUD_PLAYER_HANDLE hPlayer,AUD_SAMPLE_FREQUENCE_E enSampleFrequency);
HI_S32 HI_AUD_PutPlayerStream(AUD_PLAYER_HANDLE hPlayer,AUD_WRITE_PARA_S*psConfig, HI_U32 *pu32Words);
HI_S32 HI_AUD_FlushPlayerBuffer(AUD_PLAYER_HANDLE hPlayer,AUD_FLUSH_MODE_E enMode);
HI_S32 HI_AUD_GetPlayerIdleBufferSize(AUD_PLAYER_HANDLE hPlayer,AUD_BUFFER_TYPE_E enType, HI_U32 *pu32Size);
HI_S32 HI_AUD_GetPlayerRemainingStreamSize(AUD_PLAYER_HANDLE hPlayer,AUD_BUFFER_TYPE_E enType, HI_U32 *pu32Size);
HI_S32 HI_AUD_GetPlayerRemainingTime(AUD_PLAYER_HANDLE hPlayer,AUD_BUFFER_TYPE_E enType, HI_U32 *pu32uSeconds);
HI_S32 HI_AUD_SetPlayerMixWeight(AUD_PLAYER_HANDLE hPlayer,AUD_OUTPUT_INTERFACE_E enInterface, HI_U32 u32MixWeight);
HI_S32 HI_AUD_GetPlayerMixWeight(AUD_PLAYER_HANDLE hPlayer,AUD_OUTPUT_INTERFACE_E enInterface, HI_U32 *pu32MixWeight);
HI_S32 HI_AUD_EnablePlayerOutput(AUD_PLAYER_HANDLE hPlayer, AUD_OUTPUT_INTERFACE_E enInterface);
HI_S32 HI_AUD_DisablePlayerOutput(AUD_PLAYER_HANDLE hPlayer, AUD_OUTPUT_INTERFACE_E enInterface);
HI_S32 HI_AUD_ClearPesPackCount(AUD_PLAYER_HANDLE hPlayer);
HI_S32 HI_AUD_GetPesPackCount(AUD_PLAYER_HANDLE hPlayer,HI_U32* pPesPackCount);
HI_S32 HI_AUD_Get_ESPara(AUD_PLAYER_HANDLE hPlayer,HI_U32* pBPS,HI_U32* pStreamsize);

/* Set or Get stream source of the audio decoder*/
/* Operation of the audio decoder */
HI_S32 HI_AUD_SetStreamSource(AUD_STREAM_SOURCE_E enSource);
HI_S32 HI_AUD_GetStreamSource(AUD_STREAM_SOURCE_E *penSource);
HI_S32 HI_AUD_SetStreamType(AUD_STREAM_TYPE_E enType);
HI_S32 HI_AUD_GetStreamType(AUD_STREAM_TYPE_E *penType);
HI_S32 HI_AUD_Start(HI_VOID);
HI_S32 HI_AUD_Stop(HI_VOID);
HI_S32 HI_AUD_Pause(HI_VOID);
/* AI7D03062 add by q65271 */
HI_S32 HI_AUD_Pause_Pvr(HI_VOID);
/* AI7D03062 add by q65271 */
HI_S32 HI_AUD_Resume(HI_VOID);
HI_S32 HI_AUD_GetStatus(AUD_DECODER_STATE_E *penStatus);
HI_S32 HI_AUD_GetOriginalTrackMode( AUD_ORIGINAL_TRACK_MODE_E *penMode);
HI_S32 HI_AUD_GetOriginalSampleFrequency(AUD_SAMPLE_FREQUENCE_E *penSampleFrequence);
/*Operation of the pcm player*/
HI_S32 HI_AUD_StartPcmPlayer(HI_VOID);
HI_S32 HI_AUD_StopPcmPlayer(HI_VOID);
HI_S32 HI_AUD_PausePcmPlayer(HI_VOID);
HI_S32 HI_AUD_ResumePcmPlayer(HI_VOID);
HI_S32 HI_AUD_GetPcmPlayerStatus( AUD_PCMPLAYER_STATE_E *penState);
HI_S32 HI_AUD_SetSampleFrequency(AUD_SAMPLE_FREQUENCE_E enSampleFrequence);
HI_S32 HI_AUD_GetSampleFrequency(AUD_SAMPLE_FREQUENCE_E *penSampleFrequence);
/* Write or flush circular buffer of audio decodr and pcmplayer */
HI_S32 HI_AUD_WriteData(AUD_WRITE_PARA_S*psConfig, HI_U32 *pu32Words);
HI_S32 HI_AUD_FlushBuffer(AUD_FLUSH_MODE_E enMode);
/* Set or get audio mixer status of  output*/
/* Set or get audio output properties*/
HI_S32 HI_AUD_SetMixConfig(AUD_MIX_CONFIG_S*psMixConfig);
HI_S32 HI_AUD_GetMixConfig(AUD_MIX_CONFIG_S*psMixConfig);

HI_S32 HI_AUD_SetAnalogTrackMode(AUD_TRACK_MODE_E enMode);
HI_S32 HI_AUD_GetAnalogTrackMode(AUD_TRACK_MODE_E *penMode);
HI_S32 HI_AUD_SetTrackMode(AUD_TRACK_MODE_E enMode);

#define HI_AUD_GetTrackMode HI_AUD_GetAnalogTrackMode

HI_S32 HI_AUD_SetDigitalTrackMode(AUD_TRACK_MODE_E enMode);
HI_S32 HI_AUD_GetDigitalTrackMode(AUD_TRACK_MODE_E *penMode);

HI_S32 HI_AUD_SetAnalogAudOutpVolume(HI_U32 u32Volume);
HI_S32 HI_AUD_GetAnalogAudOutpVolume(HI_U32 *pu32Volume);
HI_S32 HI_AUD_SetDigitalAudOutpVolume(HI_U32 u32Volume);
HI_S32 HI_AUD_GetDigitalAudOutpVolume(HI_U32 *pu32Volume);
HI_S32 HI_AUD_MuteAnalogAudOutp(HI_VOID);
HI_S32 HI_AUD_UnmuteAnalogAudOutp(HI_VOID);
HI_S32 HI_AUD_GetAnalogMuteStatus(AUD_MUTE_STATUS_E *penMuteStatus);
HI_S32 HI_AUD_MuteDigitalAudOutp(HI_VOID);
HI_S32 HI_AUD_UnmuteDigitalAudOutp(HI_VOID);
HI_S32 HI_AUD_GetDigitalMuteStatus(AUD_MUTE_STATUS_E *penMuteStatus);
HI_S32 HI_AUD_Set_PlayerPts(AUD_PLAYER_HANDLE hPlayer,HI_U32 u32Pts);
HI_S32 HI_AUD_Select_PlayerPts(AUD_PLAYER_HANDLE hPlayer);

HI_S32 HI_AUD_GetPlayerBuf(AUD_PLAYER_HANDLE hPlayer, AUD_BUF_STATE_S *pBufSta);
HI_S32 HI_AUD_PutPlayerBuf(AUD_PLAYER_HANDLE hPlayer, HI_U32 u32AddLen);

HI_S32 HI_AUD_LR_Reverse(HI_BOOL bReverse);

/*begin: added by tzg on 2009.04.29 */
HI_S32 HI_AUD_GetPesBufState(AUD_PESBUF_STATE_S *pBufState);
/*end: added by tzg on 2009.04.29 */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_API_H__ */


