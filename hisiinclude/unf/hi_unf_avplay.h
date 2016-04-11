/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_unf_avplay.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/10
  Description   :
  History       :
  1.Date        : 2008/04/10
    Author      : qushen
    Modification: Created file

  2.Date        : 2008/06/03
    Author      : qushen
    Modification: Modify by review suggestions

******************************************************************************/

#ifndef __HI_UNF_AVPLAY_H__
#define __HI_UNF_AVPLAY_H__

#include "hi_type.h"
#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

#if 0
typedef enum hiAVPLAY_ERR_CODE_E
{
    AVPLAY_ERR_INV_PARA = 1, /* at lease one parameter is illegal*/
    AVPLAY_ERR_NULL_PTR,  /* using a NULL pointer*/
    AVPLAY_ERR_CREATED,    /* object created*/
    AVPLAY_ERR_UNCREATED,  /* object uncreate*/
    AVPLAY_ERR_NOT_SUPPORT,  /*current not supported*/
    AVPLAY_ERR_MEM_ERR ,
    AVPLAY_ERR_BUF_FULL,
    AVPLAY_ERR_BUF_EMPTY,
    AVPLAY_ERR_OPEN,
    AVPLAY_ERR_CLOSE,
    AVPLAY_ERR_SYS_BUSY,
    AVPLAY_ERR_BUTT
        
} AVPLAY_ERR_CODE_E;

/*错误码*/
#define HI_UNF_ERR_AVPLAY_INV_PARA HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_INV_PARA)
#define HI_UNF_ERR_AVPLAY_NULL_PTR HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_NULL_PTR)
#define HI_UNF_ERR_AVPLAY_CREATED HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_CREATED)
#define HI_UNF_ERR_AVPLAY_UNCREATED HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_UNCREATED)
#define HI_UNF_ERR_AVPLAY_NOT_SUPPORT HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_NOT_SUPPORT)
#define HI_UNF_ERR_AVPLAY_MEM_ERR HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_MEM_ERR)
#define HI_UNF_ERR_AVPLAY_BUF_FULL HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_BUF_FULL)
#define HI_UNF_ERR_AVPLAY_BUF_EMPTY HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_BUF_EMPTY)
#define HI_UNF_ERR_AVPLAY_OPEN HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_OPEN)
#define HI_UNF_ERR_AVPLAY_CLOSE HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_CLOSE)
#define HI_UNF_ERR_AVPLAY_SYS_BUSY HI_DEF_ERR(HI_UNF_ID_AVPLAY, HI_LOG_LEVEL_ERROR, AVPLAY_ERR_SYS_BUSY)
#endif
typedef enum hiUNF_AVPLAY_BUFID_E
{
    HI_UNF_AVPLAY_BUF_ID_TS = 0,
    HI_UNF_AVPLAY_BUF_ID_ES_VID  = 1,
    HI_UNF_AVPLAY_BUF_ID_ES_AUD  = 2,
    HI_UNF_AVPLAY_BUF_ID_PES_VID = 3,
    HI_UNF_AVPLAY_BUF_ID_PES_AUD = 4,
    HI_UNF_AVPLAY_BUF_ID_BUTT
} HI_UNF_AVPLAY_BUFID_E;

/********************************************************************************/
#define HI_UNF_AVPLAY_INV_TS_PID 8192
#define HI_UNF_AVPLAY_BUF_TYPE_NUM 9

/********************************************************************************/

//typedef HI_VOID* HI_UNF_AVPLAY_HANDLE;

typedef enum hiUNF_AVPLAY_MEDIA_CHAN
{
    HI_UNF_AVPLAY_MEDIA_CHAN_AUD  = 0x01,  /* 音频通道 */
    HI_UNF_AVPLAY_MEDIA_CHAN_VID  = 0x02,  /* 视频通道 */
    HI_UNF_AVPLAY_MEDIA_CHAN_SUBT = 0x04,  /* 字幕通道 */

    HI_UNF_AVPLAY_MEDIA_CHAN_BUTT
} HI_UNF_AVPLAY_MEDIA_CHAN_E;

typedef enum hiUNF_AVPLAY_STREAM_TYPE_E
{
    HI_UNF_AVPLAY_STREAM_TYPE_TS = 0,   /* mp2 ts stream        */
    HI_UNF_AVPLAY_STREAM_TYPE_ES,       /* av es stream         */
    HI_UNF_AVPLAY_STREAM_TYPE_TSI,      /* ts from ts interface */
    HI_UNF_AVPLAY_STREAM_TYPE_PES,      /* av pes stream        */

    HI_UNF_AVPLAY_STREAM_TYPE_BUTT
} HI_UNF_AVPLAY_STREAM_TYPE_E;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_TSI_ATTR_S
{
    HI_U32 u32VidBufSize; /* 视频缓冲大小 */
    HI_U32 u32AudBufSize; /* 音频缓冲大小 */
} HI_UNF_AVPLAY_TSI_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_ES_ATTR_S
{
    HI_U32 u32VidEsBufSize;  /* 视频缓冲大小     */
    HI_U32 u32AudEsBufSize;  /* 音频缓冲大小     */
    HI_U32 u32SubtEsBufSize; /* 字幕数据缓冲大小 */
} HI_UNF_AVPLAY_ES_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_TS_ATTR_S
{
    HI_U32 u32BufferSize; /* buffer 大小 */
} HI_UNF_AVPLAY_TS_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_PES_ATTR_S
{
    HI_U32 u32VidPesBufSize;  /* 视频缓冲大小     */
    HI_U32 u32AudPesBufSize;  /* 音频缓冲大小     */
} HI_UNF_AVPLAY_PES_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_SUBT_ATTR_S
{
    HI_U32 u32SubtPid;    /* subt pid */
    HI_U32 u32SubtPageId; /* subt page id */
    HI_U32 u32SubtAncId;  /* subt ancillary id */
} HI_UNF_AVPLAY_SUBT_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_STREAM_ATTR_S
{
    HI_UNF_AVPLAY_STREAM_TYPE_E enStreamType;   /* 码流类型 */

    union
    {
        HI_UNF_AVPLAY_ES_ATTR_S stEsAttr;  /* ES 流属性  */
        HI_UNF_AVPLAY_TS_ATTR_S stTsAttr;  /* TS 流属性  */
        HI_UNF_AVPLAY_TSI_ATTR_S stTsiAttr; /* TSI 流属性 */
        HI_UNF_AVPLAY_PES_ATTR_S stPesAttr; /* PES 流属性 */
    } unStreamAttr;
} HI_UNF_AVPLAY_STREAM_ATTR_S;
#if 0
typedef enum hiUNF_AVPLAY_STATUS_E
{
    HI_UNF_AVPLAY_STATUS_STOP = 0, /* 停止                 */
    HI_UNF_AVPLAY_STATUS_PLAY,     /* 播放                 */
    HI_UNF_AVPLAY_STATUS_TPLAY,    /* Trick Play, 快进快退 */
    HI_UNF_AVPLAY_STATUS_PAUSE,    /* 暂停                 */
    HI_UNF_AVPLAY_STATUS_EOS,      /* 码流播放结束         */
    HI_UNF_AVPLAY_STATUS_RESET,    /* 复位状态             */

    HI_UNF_AVPLAY_STATUS_END = HI_UNF_AVPLAY_STATUS_RESET,

    HI_UNF_AVPLAY_STATUS_BUTT
} HI_UNF_AVPLAY_STATUS_E;
#endif

typedef struct hiUNF_AVPLAY_BUF_S
{
    HI_U8 * pu8Data;         /* 数据指针            */
    HI_U32  u32Size;         /* 长度                */
} HI_UNF_AVPLAY_BUF_S;

/* 支持的同步控制模式 */
typedef enum hiUNF_AVPLAY_SYNC_REF_E
{
    HI_UNF_AVPLAY_SYNC_REF_NONE = 0, /* 自由播放    */
    HI_UNF_AVPLAY_SYNC_REF_AUDIO,    /* 以音频为准  */
    HI_UNF_AVPLAY_SYNC_REF_VIDEO,    /* 以视频为准  */
    HI_UNF_AVPLAY_SYNC_REF_PCR,      /* 以 PCR 为准 */

    HI_UNF_AVPLAY_SYNC_REF_BUTT
} HI_UNF_AVPLAY_SYNC_REF_E;

/*事件回调函数类型 */
typedef enum hiUNF_AVPLAY_EVENT_E
{
    HI_UNF_AVPLAY_EVENT_EOS = 0,           /* EOS执行结束, NULL    */
    HI_UNF_AVPLAY_EVENT_STOP = 1,                  /* STOP执行结束, NULL   */
    HI_UNF_AVPLAY_EVENT_SUBT_DISP = 2,             /* subtitle数据显示, HI_UNF_SUBT_DISP_ELE_S    */
    HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE = 3, /* 媒体缓存队列状态变化, HI_UNF_ES_BUF_STATE_E */
    HI_UNF_AVPLAY_EVENT_NORM_SWITCH = 4,           /* P/N 制切换, HI_UNF_ENC_FMT_E        */
    HI_UNF_AVPLAY_EVENT_NEW_SEQ = 5,               /* 新序列信息, HI_UNF_VIDEO_SEQUENCE_INFO_S    */
    HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME = 6,         /* 新视频帧, HI_UNF_VO_FRAMEINFO_S             */
    HI_UNF_AVPLAY_EVENT_NEW_AUD_FRAME = 7,         /* 新音频帧, HI_UNF_AO_FRAMEINFO_S             */
    HI_UNF_AVPLAY_EVENT_NEW_USER_DATA = 8,         /* 新视频用户数据, HI_UNF_VIDEO_USERDATA_S     */
    HI_UNF_AVPLAY_EVENT_VID_STREAM = 9,            /* 视频PES BUF事件, HI_UNF_AVPLAY_STREAM_STATE_E*/

    HI_UNF_AVPLAY_EVENT_BUTT
} HI_UNF_AVPLAY_EVENT_E;

typedef HI_S32 (*HI_UNF_AVPLAY_EVENT_CB_FN)(HI_HANDLE Handle, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para);

typedef enum hiUNF_AVPLAY_PRESYNC_MODE_E
{
    HI_UNF_AVPLAY_PRESYNC_MODE_NONE = 0,    /*关闭预同步功能                               */
    HI_UNF_AVPLAY_PRESYNC_MODE_NORMAL,      /*开启预同步功能，短暂延迟后再播放             */
    HI_UNF_AVPLAY_PRESYNC_MODE_QUICKOUTPUT, /*开启预同步功能，先快速输出第一帧后再做预同步 */
    HI_UNF_AVPLAY_PRESYNC_MODE_NONE_QUICKOUTPUT,  /*先快速输出第一帧，并且不做预同步       */

    HI_UNF_AVPLAY_PRESYNC_MODE_BUTT
} HI_UNF_AVPLAY_PRESYNC_MODE_E;

typedef enum hiUNF_AVPLAY_REAL_MODE_E
{
    HI_UNF_AVPLAY_REAL_MODE_DISABLE = 0, /*非实时的点播业务模式*/
    HI_UNF_AVPLAY_REAL_MODE_ENABLE,      /*实时的直播业务模式*/

    HI_UNF_AVPLAY_REAL_MODE_BUTT
} HI_UNF_AVPLAY_REAL_MODE_E;

typedef enum hiUNF_AVPLAY_ATTR_ID_E
{
    HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE = 0,  /* 播放模式 , HI_UNF_AVPLAY_STREAM_ATTR_S */

    HI_UNF_AVPLAY_ATTR_ID_ADEC,            /* 音频类型 , HI_UNF_ACODEC_ATTR_S */
    HI_UNF_AVPLAY_ATTR_ID_VDEC,            /* 视频类型 , HI_UNF_VCODEC_ATTR_S  */
    HI_UNF_AVPLAY_ATTR_ID_SUBT,            /* 字幕类型 , HI_UNF_AVPLAY_SUBT_ATTR_S*/

    HI_UNF_AVPLAY_ATTR_ID_AUD_PID,     /* 音频PID , HI_U32*/
    HI_UNF_AVPLAY_ATTR_ID_VID_PID,      /* 视频PID , HI_U32*/
    HI_UNF_AVPLAY_ATTR_ID_PCR_PID,     /* PCR PID , HI_U32*/

    HI_UNF_AVPLAY_ATTR_ID_SYNC,            /* 同步属性 , HI_UNF_AVPLAY_SYNC_ATTR_S */
    HI_UNF_AVPLAY_ATTR_ID_AFD,             /* AFD 是否开启， HI_BOOL * */

    HI_UNF_AVPLAY_ATTR_ID_BUTT
} HI_UNF_AVPLAY_ATTR_ID_E;

/*******************************************************************************/
typedef struct hiUNF_AVPLAY_SYNC_ATTR_S
{
    HI_UNF_AVPLAY_SYNC_REF_E enSyncRef; /* 音频为准，视频为准，自由播放等 */
    HI_S32                   s32VidMaxSyncRange; /* 视频同步判断的时间范围               */
    HI_S32                   s32VidMinSyncRange; 
    HI_S32                   s32AudMaxSyncRange; /* 音频同步判断的时间范围               */
    HI_S32                   s32AudMinSyncRange; 
    HI_S32                   s32TimeOffset; /* 已知的音视频时间差，值为 视频- 音频  */

    HI_UNF_AVPLAY_PRESYNC_MODE_E enPreSyncMode; /* 预同步模式 */
    HI_UNF_AVPLAY_REAL_MODE_E    enRealMode;    /* 流实时模式 */

    HI_BOOL bSmoothPlay;            /* 慢放使能 */
    HI_BOOL bClockAdjust;           /* 时钟调整使能 */
} HI_UNF_AVPLAY_SYNC_ATTR_S;

typedef struct hiUNF_AVPLAY_ATTR_S
{
    /* 类型属性 */
    HI_UNF_AVPLAY_STREAM_ATTR_S stStreamAttr; /* 码流属性 */
} HI_UNF_AVPLAY_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_SYNC_STATUS_S
{
    HI_U32 u32FirstAudPts;    /* 第一个音频帧 PTS         */
    HI_U32 u32FirstVidPts;    /* 第一个视频帧 PTS         */
    HI_U32 u32LastAudPts;     /* 最近播放的一个音频帧 PTS */
    HI_U32 u32LastVidPts;     /* 最近播放的一个视频帧 PTS */
    HI_S32 s32DiffAvPlayTime; /* 音视频 播放时差          */
    HI_U64 u64PlayTime;       /* 当前播放时间             */
} HI_UNF_AVPLAY_SYNC_STATUS_S;

typedef struct hiUNF_AVPLAY_BUF_STATUS_S
{
    HI_U32 u32BufId;    /* 媒体缓冲 标识      */
    HI_U32 u32BufSize;  /* 媒体缓冲大小       */
    HI_U32 u32UsedSize; /* 媒体缓冲已使用大小 */
} HI_UNF_AVPLAY_BUF_STATUS_S;

typedef enum  hiUNF_ACODEC_TYPE_E
{
    HI_UNF_ACODEC_TYPE_AAC,
    HI_UNF_ACODEC_TYPE_AAC_RAW,
    HI_UNF_ACODEC_TYPE_MP3,
    HI_UNF_ACODEC_TYPE_AC3,
    HI_UNF_ACODEC_TYPE_PCM,

    HI_UNF_ACODEC_TYPE_BUTT
}HI_UNF_ACODEC_TYPE_E;

typedef enum hiUNF_AC3_WORK_MODE_E
{ 
    HI_UNF_AC3_PCM = 0,         /* normal decoding, output pcm datas    */
    HI_UNF_AC3_RAW = 1,     /* not decoding, output 61937 datas         */
    HI_UNF_AC3_P_R = 2,     /* output pcm datas and iec61937 datas  */
    HI_UNF_AC3_BUTT
}HI_UNF_AC3_WORK_MODE_E;

typedef enum hiUNF_AAC_PROFILE_E
{
    HI_UNF_AAC_PROFILE_MP  = 0,
    HI_UNF_AAC_PROFILE_LC  = 1,
    HI_UNF_AAC_PROFILE_SSR  = 2,
    HI_UNF_AAC_PROFILE_BUTT
}HI_UNF_AAC_PROFILE_E;

/* Audio Decoder Attribute */
typedef struct hiUNF_AAC_RAW_PARAM_S
{
    HI_UNF_AAC_PROFILE_E  enProfile;
    HI_UNF_SAMPLE_RATE_E  enSampleRate;
    HI_UNF_TRACK_MODE_E   enChannels;
}HI_UNF_AAC_RAW_PARAM_S;

/* Audio Decoder Attribute */
typedef struct hiUNF_AC3_PARAM_S
{
    HI_UNF_AC3_WORK_MODE_E  enAc3workmode;
}HI_UNF_AC3_PARAM_S;

typedef struct hiUNF_PCM_PARAM_S
{
    HI_U32                          u32BitWidth;            /*  采样位宽    */
    HI_BOOL                         bBigEndian;         /*  大小端存储  */
    HI_UNF_SAMPLE_RATE_E            enSampleRate;              /*PCM采样率设置*/
        HI_BOOL                     bRawPCM;                  /*Raw格式PCM码流标识*/  
}HI_UNF_PCM_PARAM_S ;

typedef struct hiUNF_ACODEC_ATTR_S{
    HI_UNF_ACODEC_TYPE_E        enType;                 /* 解码类型            */
    union   
    {
        HI_UNF_AAC_RAW_PARAM_S      stAacRawParam;          /* RAW AAC 码流属性     */
        HI_UNF_AC3_PARAM_S          stAc3Param;             /* AC3 码流属性         */
        HI_UNF_PCM_PARAM_S          stPcmParam;             /*  PCM 码流属性        */
    } unAdecPara ;
}HI_UNF_ACODEC_ATTR_S;

typedef enum hiUNF_VCODEC_MODE_E
{
    HI_UNF_VCODEC_MODE_NORMAL = 0,
    HI_UNF_VCODEC_MODE_IP,
    HI_UNF_VCODEC_MODE_I,
    HI_UNF_VCODEC_MODE_BUTT
}HI_UNF_VCODEC_MODE_E;

typedef struct hiUNF_VCODEC_ATTR_S
{
    HI_UNF_VCODEC_TYPE_E  enType;       /*视频解码设备的解码类型*/
    HI_UNF_VCODEC_MODE_E  enMode;       /*视频解码设备的解码模式*/
    HI_U32               u32ErrCover;   /*视频解码设备的输出帧错误隐藏门限，0:出现错误即不输出；100:不管错误比例全部输出*/
    HI_U32               u32RefCover;   /*视频解码设备的参考帧错误隐藏门限，0:出现错误即不参考，丢弃整个序列；100:不管错误比例全部用作参考*/
}HI_UNF_VCODEC_ATTR_S ;

typedef struct hiUNF_ACODEC_STREAMINFO_S
{
    HI_UNF_ACODEC_TYPE_E enACodecType;
    HI_UNF_SAMPLE_RATE_E enSampleRate;
    HI_UNF_BIT_DEPTH_E   enBitDepth;
    HI_UNF_TRACK_MODE_E  enTrackMode;
}HI_UNF_ACODEC_STREAMINFO_S;

typedef enum hiUNF_AVPLAY_STATUS_E
{
    HI_UNF_AVPLAY_STATUS_STOP = 0, /* 停止                 */
    HI_UNF_AVPLAY_STATUS_PLAY,     /* 播放                 */
    HI_UNF_AVPLAY_STATUS_TPLAY,    /* Trick Play, 快进快退 */
    HI_UNF_AVPLAY_STATUS_PAUSE,    /* 暂停                 */
    HI_UNF_AVPLAY_STATUS_EOS,      /* 码流播放结束         */
    HI_UNF_AVPLAY_STATUS_RESET,    /* 复位状态             */
    HI_UNF_AVPLAY_STATUS_SEEK ,    /* 定位播放             */

    HI_UNF_AVPLAY_STATUS_END = HI_UNF_AVPLAY_STATUS_RESET,

    HI_UNF_AVPLAY_STATUS_BUTT
}HI_UNF_AVPLAY_STATUS_E;

typedef struct hiUNF_VCODEC_STATUSINFO_S
{
    HI_U32 u32VersionNumber;
     HI_UNF_AVPLAY_STATUS_E enStatus;   
}HI_UNF_VCODEC_STATUSINFO_S;

typedef struct hiUNF_ACODEC_STATUSINFO_S
{
    HI_BOOL bWorkState;
    HI_U32  u32OutBufferNum;
    HI_U32  u32UsedBufferNum;
    HI_UNF_AVPLAY_STATUS_E enStatus;    
}HI_UNF_ACODEC_STATUSINFO_S;
typedef struct hiUNF_AVPLAY_STATUS_INFO_S
{
    HI_UNF_AVPLAY_SYNC_STATUS_S stSyncStatus;   /* 同步状态 */
    HI_UNF_AVPLAY_STATUS_E      enRunStatus;    /* 运行状态 */

    HI_UNF_AVPLAY_BUF_STATUS_S stBufStatus[HI_UNF_AVPLAY_BUF_TYPE_NUM]; /*媒体缓冲状态 */
    HI_UNF_VCODEC_STATUSINFO_S stVcodecStatus; /* 视频解码器状态 */
    HI_UNF_ACODEC_STATUSINFO_S stAcodecStatus; /* 音频解码器状态 */
} HI_UNF_AVPLAY_STATUS_INFO_S;

/********************************************************************************/

typedef struct hiUNF_ACODEC_DEBUGINFO_S
{
    HI_U32 u32DecFrameNum;
    HI_U32 u32ErrorFrameNum;
    HI_U32 u32BufferSize;           /* 码流buffer长度     */
}HI_UNF_ACODEC_DEBUGINFO_S;

typedef struct hiUNF_VCODEC_DEBUGINFO_S
{
    HI_BOOL          bEnable;       /* 使能状态     */
    HI_UNF_VCODEC_TYPE_E enVCodecType;  /* 解码协议     */
    HI_U32 u32VdecThreadState;      /* 解码线程状态 */

    /*video frame buffer status*/
    HI_U32 u32FrameQueueSize;       /* 解码帧队列长度   */
    HI_U32 u32CurrentQueueFrameNum; /* 当前未读取解码帧 */

    /*video stream buffer status*/
    HI_U32 u32BufferSize;           /* 码流buffer长度     */
    HI_U32 u32BufferUsed;           /* 码流buffer已用长度 */

    HI_U32 u32ErrorNum;             /* 当前码流的出错总数 */

    HI_U32 u32TotalBits;            /* 当前码流总解码bit数, Kbits                      */
    HI_U32 u32TotalFrame;           /* 当前码流总解码帧数                              */
    HI_U32 u32TotalTime;            /* 当前码流运行时间，second                        */
    HI_U32 u32Averagebps;           /* 当前码流的平均码率, Kbits per second            */
    HI_U32 u32Average_fps_integer;  /* 当前码流的平均帧率的整数部分, frames per second */
    HI_U32 u32Average_fps_decimal;  /* 当前码流的平均帧率的小数部分, frames per second */
}HI_UNF_VCODEC_DEBUGINFO_S, *HI_UNF_VCODEC_DEBUGINFO_S_PTR;
typedef struct hiUNF_AVPLAY_DEBUG_INFO_S
{
    HI_U32 u32VidDecFrameNum;    /* 视频解码帧数           */
    HI_U32 u32VidRepeatFrameCnt; /* 视频帧重(repeat)复次数 */
    HI_U32 u32VidSkipFrameCnt;   /* 视频帧丢弃(skip)次数   */
    HI_U32 u32VidPlayFrameCnt;   /* 视频帧显示显示次数     */

    HI_U32 u32AudDecFrameNum;  /* 音频解码帧数   */
    HI_U32 u32AudPlayFrameCnt; /* 音频帧播放个数 */

    HI_U32 u32VidRcvEsPackNum; /* 收到视频ES包个数 */
    HI_U32 u32AudRcvEsPackNum; /* 收到音频ES包个数 */

    HI_UNF_VCODEC_DEBUGINFO_S stVcodecInfo; /* 视频解码器调试信息 */
    HI_UNF_ACODEC_DEBUGINFO_S stAcodecInfo; /* 音频解码器调试信息 */
} HI_UNF_AVPLAY_DEBUG_INFO_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_STREAM_INFO_S
{
    HI_UNF_VCODEC_STREAMINFO_S stVidStreamInfo; /* 视频流信息 */
    HI_UNF_ACODEC_STREAMINFO_S stAudStreamInfo; /* 音频流信息 */
} HI_UNF_AVPLAY_STREAM_INFO_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_I_FRAME_S
{
    HI_U8  *             pu8Addr; /*帧数据用户态虚拟地址*/
    HI_U32               u32BufSize; /*帧数据大小，单位字节*/
    HI_UNF_VCODEC_TYPE_E enType;  /*该片数据的协议类型*/
} HI_UNF_AVPLAY_I_FRAME_S;

/********************************************************************************/
typedef enum hiUNF_AVPLAY_STOP_MODE_E
{
    HI_UNF_AVPLAY_STOP_MODE_STILL = 0,  /*stop后保留最后一帧*/
    HI_UNF_AVPLAY_STOP_MODE_BLACK = 1,  /*stop后黑屏*/
    HI_UNF_AVPLAY_STOP_MODE_BUTT
} HI_UNF_AVPLAY_STOP_MODE_E;

typedef struct hiUNF_AVPLAY_STOP_OPT_S
{
    /*
      s32Timeout: end of stream timeout
      1. s32Timeout = 0   非阻塞等待码流播放结束，立即返回
      2. s32Timeout != 0  阻塞等待码流播放结束
         s32Timeout > 0   阻塞超时时间，单位为毫秒
         s32Timeout = -1  无限等待
     */
    HI_S32                    s32Timeout;
    HI_UNF_AVPLAY_STOP_MODE_E enMode;
} HI_UNF_AVPLAY_STOP_OPT_S;

HI_S32 HI_UNF_AVPLAY_Init(HI_VOID);
HI_S32 HI_UNF_AVPLAY_DeInit(HI_VOID);
HI_S32 HI_UNF_AVPLAY_GetDefaultConfig(HI_UNF_AVPLAY_ATTR_S * pstAttr, HI_UNF_AVPLAY_STREAM_TYPE_E enCfg);
HI_S32 HI_UNF_AVPLAY_Create(const HI_UNF_AVPLAY_ATTR_S * pstAttr, HI_HANDLE * pHandle);
HI_S32 HI_UNF_AVPLAY_Destroy(HI_HANDLE Handle);
HI_S32 HI_UNF_AVPLAY_SetAttr(HI_HANDLE Handle, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);
HI_S32 HI_UNF_AVPLAY_GetAttr(HI_HANDLE Hanldle, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);
HI_S32 HI_UNF_AVPLAY_DecodeIFrame(HI_HANDLE Handle, HI_UNF_AVPLAY_I_FRAME_S *pstFrame);
HI_S32 HI_UNF_AVPLAY_RegisterEvent(HI_HANDLE      Handle,
                                   HI_UNF_AVPLAY_EVENT_E     enEvent,
                                   HI_UNF_AVPLAY_EVENT_CB_FN pfnEventCB);
HI_S32 HI_UNF_AVPLAY_UnRegisterEvent(HI_HANDLE Handle, HI_UNF_AVPLAY_EVENT_E enEvent);
HI_S32 HI_UNF_AVPLAY_ChnOpen(HI_HANDLE Handle, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_VOID *pPara);
HI_S32 HI_UNF_AVPLAY_ChnClose(HI_HANDLE Handle, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn);
HI_S32 HI_UNF_AVPLAY_Start(HI_HANDLE Handle, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_VOID *pOption);
HI_S32 HI_UNF_AVPLAY_Stop(HI_HANDLE Handle, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_VOID *pOption);
HI_S32 HI_UNF_AVPLAY_Pause(HI_HANDLE Handle, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_VOID *pOption);
HI_S32 HI_UNF_AVPLAY_Resume(HI_HANDLE Handle, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_VOID *pOption);
HI_S32 HI_UNF_AVPLAY_Reset(HI_HANDLE Handle, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, HI_VOID *pOption);
HI_S32 HI_UNF_AVPLAY_Tplay(HI_HANDLE Handle, HI_VOID  *pOption );
HI_S32 HI_UNF_AVPLAY_GetBuf(HI_HANDLE  Handle,
                            HI_UNF_AVPLAY_BUFID_E BufId,
                            HI_S32                s32ReqLen,
                            HI_UNF_AVPLAY_BUF_S  *pstData,
                            HI_S32                s32Timeout);

HI_S32 HI_UNF_AVPLAY_PutBuf(HI_HANDLE Handle, HI_UNF_AVPLAY_BUFID_E BufId,
                         HI_U32 u32ValidDataLen, HI_U64 u64pts);
HI_S32 HI_UNF_AVPLAY_GetStatusInfo(HI_HANDLE          Handle,
                                   HI_UNF_AVPLAY_STATUS_INFO_S * pstStatusInfo);
HI_S32 HI_UNF_AVPLAY_GetDebugInfo(HI_HANDLE        Handle,
                                  HI_UNF_AVPLAY_DEBUG_INFO_S *pstDebugInfo);
HI_S32 HI_UNF_AVPLAY_GetStreamInfo(HI_HANDLE          Handle,
                                   HI_UNF_AVPLAY_STREAM_INFO_S * pstStreamInfo);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __HI_UNF_AVPLAY_H__ */
