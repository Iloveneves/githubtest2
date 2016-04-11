/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_audio_type.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2007/1/16
Last Modified :
Description   : 外部模块调用Audio模块头文件定义。
Function List :
History       :
 ******************************************************************************/

#ifndef __HI_AUDIO_TYPE_H__
#define __HI_AUDIO_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define MAX_FRAME_COUNT_TO_BE_DECODE  10

typedef enum hiAUD_ORIGINAL_TRACK_MODE_E
{
    AUD_ORIGINAL_STEREO = 0,                        
    AUD_ORIGINAL_JOINT_STEREO = 1,   
    AUD_ORIGINAL_DUAL_CHANNEL = 2, 
    AUD_ORIGINAL_SINGLE_CHANNEL = 3, 
    AUD_ORIGINAL_UNKNOWN,
} AUD_ORIGINAL_TRACK_MODE_E;

typedef enum hiAUD_TRACK_MODE_E
{
    AUD_TRACK_MODE_STEREO = AUD_ORIGINAL_STEREO,                   
    AUD_TRACK_MODE_JOINT_STEREO = AUD_ORIGINAL_JOINT_STEREO, 
    AUD_TRACK_MODE_DOUBLE = AUD_ORIGINAL_DUAL_CHANNEL,                   

    AUD_TRACK_MODE_LEFT,
    AUD_TRACK_MODE_RIGHT,                                           
    AUD_TRACK_MODE_UNKNOWN, 
}AUD_TRACK_MODE_E;


typedef enum hiAUD_SAMPLE_FREQUENCE_E
{
    AUD_SAMPLE_FREQUENCE_2205KHZ = 0,     /* 22.05KHZ */
    AUD_SAMPLE_FREQUENCE_24KHZ,     
    AUD_SAMPLE_FREQUENCE_16KHZ,     
    AUD_SAMPLE_FREQUENCE_441KHZ,     /* 44.1KHZ */
    AUD_SAMPLE_FREQUENCE_48KHZ,     
    AUD_SAMPLE_FREQUENCE_32KHZ,     
    AUD_SAMPLE_FREQUENCE_8KHZ,
/*******add by w37134 for AI7D02933 2008.8.22 begin*********/
    AUD_SAMPLE_FREQUENCE_96KHZ,
    AUD_SAMPLE_FREQUENCE_128KHZ,
/*******add by w37134 for AI7D02933 2008.8.22 end*********/
    AUD_SAMPLE_FREQUENCE_UNKNOWN,
} AUD_SAMPLE_FREQUENCE_E;

typedef enum hiAUD_DOLPHIN_MAIN_FREQUENCE_E
{
    AUD_DOLPHIN_FREQUENCE_2205KHZ = 3,     /* 22.05KHZ */
    AUD_DOLPHIN_FREQUENCE_24KHZ   = 3,     
    AUD_DOLPHIN_FREQUENCE_16KHZ   = 2,     
    AUD_DOLPHIN_FREQUENCE_441KHZ  = 5,     /* 44.1KHZ */
    AUD_DOLPHIN_FREQUENCE_48KHZ   = 5,     
    AUD_DOLPHIN_FREQUENCE_32KHZ   = 4,     
    AUD_DOLPHIN_FREQUENCE_UNKNOWN,
} AUD_DOLPHIN_MAIN_FREQUENCE_E;

typedef enum hiAUD_STREAM_SOURCE_E 
{
    AUD_STREAM_SOURCE_DEMUX      = 0,     /* Audio data comes from DEMUX */ 
    AUD_STREAM_SOURCE_MEM        = 1,      /* Audio data comes from memory */
    AUD_STREAM_SOURCE_UNKNOWN,
} AUD_STREAM_SOURCE_E;

typedef enum hiAUD_DATA_FORMAT_E 
{
    AUD_FORMAT_MPEG              = (1),     /* Format of audio data is MPEG*/
    AUD_FORMAT_PCM               = (1<<1),      /* source must be MEM, Format of audio data is PCM */
    AUD_FORMAT_UNKNOWN,
} AUD_DATA_FORMAT_E;

typedef enum hiAUD_DECODER_STATE_E
{
    AUD_DECODER_INITIAL          = 0, 
    AUD_DECODER_RUNNING,    
    AUD_DECODER_PAUSED,
    AUD_DECODER_STOPPED,
    AUD_DECODER_UNKNOWN,
} AUD_DECODER_STATE_E;

typedef enum hiAUD_PLAYER_STATE_E
{
    AUD_PLAYER_INITIAL          = 0, 
    AUD_PLAYER_RUNNING,    
    AUD_PLAYER_PAUSED,
    AUD_PLAYER_STOPPED,
    AUD_PLAYER_STOPPING,
    AUD_PLAYER_UNKNOWN,
} AUD_PLAYER_STATE_E;

typedef enum hiAUD_BUFFER_TYPE_E
{
    AUD_ES_BUFFER,
    AUD_PCMO_BUFFER,
    AUD_CPCM_BUFFER,
    AUD_DPCM_BUFFER,
    AUD_MPCM_BUFFER
} AUD_BUFFER_TYPE_E;


typedef enum hiAUD_PCMPLAYER_STATE_E
{
    AUD_PCMPLAYER_INITIAL          = 0, 
    AUD_PCMPLAYER_RUNNING,    
    AUD_PCMPLAYER_PAUSED,
    AUD_PCMPLAYER_STOPPED,
    AUD_PCMPLAYER_UNKNOWN,
} AUD_PCMPLAYER_STATE_E;

typedef enum hiAUD_MUTE_STATUS_E
{
    AUD_UNMUTE           = 0,     
    AUD_MUTE,          
} AUD_MUTE_STATUS_E;

typedef enum hiAUD_STREAM_TYPE_E
{
    /*AI7D02674: added by tzg on 2008.7.14  begin*/
    AUD_STREAM_MPEG = 0,
    AUD_STREAM_PCM,
    AUD_STREAM_ES,   
    AUD_STREAM_PES,
    /*AI7D02674: added by tzg on 2008.7.14  end*/
    AUD_STREAM_PCM24,
    AUD_STREAM_PCM20,
    AUD_STREAM_PCM18,
    AUD_STREAM_PCM16,
    AUD_STREAM_RAW16,  
    AUD_STREAM_AC3,
    AUD_STREAM_UNKNOWN
} AUD_STREAM_TYPE_E;

typedef struct hiAUD_PLAYER_ATTR_S
{
   AUD_STREAM_SOURCE_E     enStreamSource;   /*输入源类型*/        
   AUD_STREAM_TYPE_E       enStreamType;     /*输入数据格式*/
}AUD_PLAYER_ATTR_S;

typedef HI_S32 AUD_PLAYER_HANDLE;

typedef struct hiAUD_IOCTL_ATTR_S
{
    HI_S32 s32CmdPara1;
    HI_S32 s32CmdPara2;
    HI_S32 s32CmdPara3;
}AUD_IOCTL_ATTR_S;

typedef enum hiAUD_ADJUST_DIRECTION_E
{
    AUD_ADJUST_FAST,
    AUD_ADJUST_SLOW,
    AUD_ADJUST_NONE  //add by x57522 for sync    
} AUD_ADJUST_DIRECTION_E;

typedef enum hiAUD_ADJUST_CTROL_E
{
    AUD_ADJUST_START,
    AUD_ADJUST_STOP
}AUD_ADJUST_CTROL_E;

typedef enum hiAUD_ADJUST_SPEED_E
{
    AUD_ADJUST_SPEED1            = 8,
    AUD_ADJUST_SPEED2            = 16,
    AUD_ADJUST_SPEED3            = 32
}AUD_ADJUST_SPEED_E;


typedef enum hiAUD_MCE_AUDIO_CMD_E
{
    MCE_AUDIO_START              = 1,
    MCE_SWITCH_TO_SOFT           = 6
}AUD_MCE_AUDIO_CMD_E;


typedef struct
{
    struct
    {
        HI_U32 u32MixMPEGAlpha;                /* 输出左声道的混合系数1*/
        HI_U32 u32MixPcmAlpha;                /* 输出左声道的混合系数2*/
        HI_U32 u32MixMPEGLeft;            /* 混合MPEG解码器输出的左声道声音*/
        HI_U32 u32MixMPEGRight;         /* 混合MPEG 解码器输出的右声道声音*/
        HI_U32 u32MixPcmLeft;              /* 混和PCM 播放器输出的左声道声音*/
        HI_U32 u32MixPcmRight;            /* 混和PCM 播放器输出的右声道声音*/
    }LeftChannel;


    struct
    {
        HI_U32 u32MixMPEGAlpha;                /* 输出右声道的混合系数1*/
        HI_U32 u32MixPcmAlpha;                /* 输出右声道的混合系数2*/
        HI_U32 u32MixMPEGLeft;            /* 混合MPEG解码器输出的左声道声音*/
        HI_U32 u32MixMPEGRight;         /* 混合MPEG 解码器输出的右声道声音*/
        HI_U32 u32MixPcmLeft;              /* 混和PCM 播放器输出的左声道声音*/
        HI_U32 u32MixPcmRight;            /* 混和PCM 播放器输出的右声道声音*/
    }RightChannel;
}AUD_MIX_CONFIG_S, *PTR_AUD_MIX_CONFIG_S;

/*definition for x5 */
typedef enum hiAUD_CHANNEL_E
{
    AUD_CHANNEL_LEFT = 0,
    AUD_CHANNEL_RIGHT,    
}AUD_CHANNEL_E;
    /*AI7D02674: added by tzg on 2008.7.14  begin*/
typedef struct hiAUD_PLAYER_OUTPUT_SELECT_S
{
    struct
    {
        HI_U32 u32MixWeight;
        AUD_CHANNEL_E enSourceChannel;
    }LeftChannel; /*播放器输出的左声道*/

    struct
    {
        HI_U32 u32MixWeight;
        AUD_CHANNEL_E enSourceChannel;
    }RightChannel;   /*播放器输出的右声道*/

}AUD_PLAYER_OUTPUT_SELECT_S;
    /*AI7D02674: added by tzg on 2008.7.14  end*/
typedef struct
{
    AUD_PLAYER_HANDLE  hPlayer1;             /*混合hPlayer1 播放出来的声音*/
    HI_U32             u32Used1;             /* 标识hPlayer1已经被占用 */             
    AUD_CHANNEL_E      enChannelofhPlayer1; /*混合hPlayer1的指定声道的声音*/
    HI_U32             u32MixAlpha1;         /* 混合声道的混合系数1*/
    AUD_PLAYER_HANDLE  hPlayer2;             /*混合hPlayer2 播放出来的声音*/
    HI_U32             u32Used2;             /* 标识hPlayer2已经被占用 */             
    AUD_CHANNEL_E      enChannelofhPlayer2; /*混合hPlayer2的指定声道的声音*/
    HI_U32             u32MixAlpha2;         /* 混合声道的混合系数2*/
}AUD_MIX_COMPOSER_S ;

typedef struct
{
    AUD_MIX_COMPOSER_S sLeftChannel;
    AUD_MIX_COMPOSER_S sRightChannel;
}AUD_MIX_SELECT_S;

typedef enum hiAUD_ADDR_TYPE_E
{
      AUD_VIRTUAL_ADDRESS =  0x0,
      AUD_PHYSICAL_ADDRESS,
      AUD_FLASH_ADDRESS   
}AUD_ADDR_TYPE_E;

typedef enum hiAUD_OUTPUT_INTERFACE_E
{
    AUD_OUTPUT_INTERFACE_DAC,
    AUD_OUTPUT_INTERFACE_SPDIF
} AUD_OUTPUT_INTERFACE_E;

typedef struct hiAUD_WRITE_PARA_S
{
    HI_VOID* pSourceAddr;                            /*数据起始地址*/
    HI_U32 u32StreamLen;                             /*数据流长度*/
    AUD_ADDR_TYPE_E enAddrType;                      /*地址类型*/
    AUD_STREAM_TYPE_E enStreamType;                  /*音频数据流类型*/
    HI_U32 u32TimeOut;                               /*写超时值*/
}AUD_WRITE_PARA_S, *PTR_AUD_WRITE_PARA_S;


typedef struct hiAUD_BUF_STATE_S
{
    HI_U32      u32Start;
    HI_U32      u32Offset;
    HI_U32      u32Size;    
    HI_U32      u32Idle;
    HI_U32      u32ActualIdle;
}AUD_BUF_STATE_S;

typedef struct hiAUD_PESBUF_STATE_S
{
    HI_U32      u32Start;
    HI_U32      u32End;
    HI_U32      u32Write;
    HI_U32      u32Read;
}AUD_PESBUF_STATE_S;

typedef enum hiAUD_FLUSH_MODE_E
{
    AUD_FLUSH_ALL = 0x0,
    AUD_FLUSH_MPEG_BUFFER = 0x1,
    AUD_FLUSH_PCM_BUFFER = 0x2,
}AUD_FLUSH_MODE_E;

typedef enum hiAUD_DATA_ENDIAN_E
{
    AUD_DATA_ENDIAN_BIG,
    AUD_DATA_ENDIAN_LITTLE,
    AUD_DATA_ENDIAN_UNKNOWN,
}AUD_DATA_ENDIAN_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_TYPE_H__ */

