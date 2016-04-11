/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : audio_errcode.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2006/3/6
Last Modified :
Description   : ϵͳ���õĴ����붨��
Function List :
History       :
 ******************************************************************************/
#ifndef __AUDIO_ERRCODE_H__
#define __AUDIO_ERRCODE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/********************************************************
  AUDIO�����붨��
 *********************************************************/
#define MID_AUDIO 0x23/*AUDIOģ��ID*/

enum hiAUD_ErrorCode
{
    ERR_AUD_INVALID_STREAM_SOURCE    = 0x0,
    ERR_AUD_CHANNEL_OPENNING         = 0x1,
    ERR_AUD_RUNNING_STATUS           = 0x2,
    ERR_AUD_INVALID_DATA_FORMAT      = 0x3,
    ERR_AUD_DEMUX_INPUT_NOT_PCM_PLAY = 0x4,
    ERR_AUD_UNSUITED_STREAM_SOURCE   = 0x5,
    ERR_AUD_INVALID_STATUS_TRANSFER  = 0x6,
    ERR_AUD_INVALID_VOLUME           = 0x7,
    ERR_AUD_INVALID_TRACK_MODE       = 0x8,
    ERR_AUD_INVALID_SAMPLE_FREQUENCY = 0x9,
    ERR_AUD_TOO_FEW_TO_COPY          = 0xa,
    ERR_AUD_INVALID_OPERATION        = 0xb,
    ERR_AUD_NO_OPEN      = 0xc,
    ERR_AUD_OPENED        = 0xd,
    ERR_AUD_PTR_NULL = 0xe,
    ERR_AUD_NO_RUNNING_STATUS = 0xf,
    ERR_AUD_WORK_MODE_MIX = 0x10,
    ERR_AUD_INVALID_WORK_MODE = 0x11,
    ERR_AUD_INVALID_FLUSH_MODE = 0x12,
    ERR_AUD_SYNC_PALYBACK = 0x13,  /* AI7D01991 */
    ERR_AUD_CHANGE_SAMPLE_FREQ_FAILED = 0x14,

    /*added for X5*/    
    ERR_AUD_CREATE_DECODER_HANDLE_FAILED = 0x15,
    ERR_AUD_CREATE_RESAMPLER_HANDLE_FAILED = 0x16,
    ERR_AUD_CREATE_MP2_HANDLE_FAILED = 0x17,
    ERR_AUD_CREATE_PLAYER_HANDLE_FAILED = 0x18,
    ERR_AUD_INVALID_PLAYER_HANDLE = 0x19,
    ERR_AUD_START_PARSER_THREAD_FAILED = 0x20,
    ERR_AUD_CREATE_EXPORTER_HANDLE_FAILED = 0x21,
    ERR_AUD_CREATE_PCMPLAYER_HANDLE_FAILED = 0x22,
    /*AI7D02674: added by tzg on 2008.7.14  begin*/
    ERR_AUD_INVALID_STREAM_TYPE = 0x23,

    ERR_SND_INV_HANDLE         = 0x31, /* at lease one parameter is illegal*/
    ERR_SND_INV_PARA           = 0x32, /* at lease one parameter is illegal*/
    ERR_SND_NULL_PTR           = 0x33, /* using a NULL pointer*/
    ERR_SND_NOT_INIT           = 0x34, /* using a NULL pointer*/
    ERR_SND_USED               = 0x35,
    ERR_SND_NOT_SUPPORT        = 0x36,  /*current not supported*/

    /*common error*/
    ERR_AUD_INVALID_PARAMETER = 0xfe,
    ERR_AUD_UNSUPPORTED_FEATURE = 0xff,  /* for debug */
};

#define HI_ERR_AUD_INVALID_STREAM_TYPE\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_STREAM_TYPE)
    /*AI7D02674: added by tzg on 2008.7.14  begin*/

#define HI_ERR_AUD_INVALID_STREAM_SOURCE\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_STREAM_SOURCE)

#define HI_ERR_AUD_CHANNEL_OPENNING\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_CHANNEL_OPENNING)

#define HI_ERR_AUD_RUNNING_STATUS\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_RUNNING_STATUS)

#define HI_ERR_AUD_INVALID_DATA_FORMAT\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_DATA_FORMAT)

#define HI_ERR_AUD_DEMUX_INPUT_NOT_PCM_PLAY\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_DEMUX_INPUT_NOT_PCM_PLAY)

#define HI_ERR_AUD_UNSUITED_STREAM_SOURCE\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_UNSUITED_STREAM_SOURCE)

#define HI_ERR_AUD_INVALID_STATUS_TRANSFER\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_STATUS_TRANSFER)

#define HI_ERR_AUD_INVALID_VOLUME\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_VOLUME)

#define HI_ERR_AUD_INVALID_TRACK_MODE\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_TRACK_MODE)

#define HI_ERR_AUD_INVALID_SAMPLE_FREQUENCY\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_SAMPLE_FREQUENCY)

#define HI_ERR_AUD_TOO_FEW_TO_COPY\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_TOO_FEW_TO_COPY)

#define HI_ERR_AUD_INVALID_OPERATION\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_OPERATION)

#define HI_ERR_AUD_OPENED\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_OPENED)

#define HI_ERR_AUD_NO_OPEN\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_NO_OPEN)

#define HI_ERR_AUD_PTR_NULL\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_PTR_NULL)

#define HI_ERR_AUD_NO_RUNNING_STATUS\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_NO_RUNNING_STATUS)

#define HI_ERR_AUD_WORK_MODE_MIX\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_WORK_MODE_MIX)

#define HI_ERR_AUD_INVALID_WORK_MODE\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_WORK_MODE)

#define HI_ERR_AUD_INVALID_FLUSH_MODE\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_FLUSH_MODE)

#define HI_ERR_AUD_UNSUPPORTED_FEATURE \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_UNSUPPORTED_FEATURE)
    
#define HI_ERR_AUD_SYNC_PALYBACK\
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_SYNC_PALYBACK)/* AI7D01991 */
   
#define HI_ERR_AUD_CHANGE_SAMPLE_FREQ_FAILED \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_CHANGE_SAMPLE_FREQ_FAILED)

#define HI_ERR_AUD_CHANGE_SAMPLE_FREQ_FAILED \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_CHANGE_SAMPLE_FREQ_FAILED)
    

#define HI_ERR_AUD_CREATE_DECODER_HANDLE_FAILED \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_CREATE_DECODER_HANDLE_FAILED)

#define HI_ERR_AUD_CREATE_RESAMPLER_HANDLE_FAILED \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_CREATE_RESAMPLER_HANDLE_FAILED)

#define HI_ERR_AUD_CREATE_PLAYER_HANDLE_FAILED \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_CREATE_PLAYER_HANDLE_FAILED)
    
#define HI_ERR_AUD_INVALID_PLAYER_HANDLE \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_PLAYER_HANDLE)

#define HI_ERR_AUD_START_PARSER_THREAD_FAILED \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_START_PARSER_THREAD_FAILED)

#define HI_ERR_AUD_CREATE_EXPORTER_HANDLE_FAILED \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_CREATE_EXPORTER_HANDLE_FAILED)
    
#define HI_ERR_AUD_CREATE_PCMPLAYER_HANDLE_FAILED \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_CREATE_PCMPLAYER_HANDLE_FAILED)

    /*AI7D02674: added by tzg on 2008.7.14  begin*/
#define HI_ERR_AUD_INVALID_PARAMETER \
    HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_AUD_INVALID_PARAMETER)
    /*AI7D02674: added by tzg on 2008.7.14  end*/

#define HI_UNF_ERR_SND_INV_HANDLE HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_SND_INV_HANDLE)
#define HI_UNF_ERR_SND_INV_PARA HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_SND_INV_PARA)
#define HI_UNF_ERR_SND_NULL_PTR HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_SND_NULL_PTR)
#define HI_UNF_ERR_SND_NOT_INIT HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_SND_NOT_INIT)
#define HI_UNF_ERR_SND_USED HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_SND_USED)
#define HI_UNF_ERR_SND_NOTSUPPORT HI_DEF_ERR(MID_AUDIO, HI_LOG_LEVEL_ERROR, ERR_SND_NOT_SUPPORT)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* _AUDIO_ERRDEF_H__ */
