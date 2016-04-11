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

/*������*/
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
    HI_UNF_AVPLAY_MEDIA_CHAN_AUD  = 0x01,  /* ��Ƶͨ�� */
    HI_UNF_AVPLAY_MEDIA_CHAN_VID  = 0x02,  /* ��Ƶͨ�� */
    HI_UNF_AVPLAY_MEDIA_CHAN_SUBT = 0x04,  /* ��Ļͨ�� */

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
    HI_U32 u32VidBufSize; /* ��Ƶ�����С */
    HI_U32 u32AudBufSize; /* ��Ƶ�����С */
} HI_UNF_AVPLAY_TSI_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_ES_ATTR_S
{
    HI_U32 u32VidEsBufSize;  /* ��Ƶ�����С     */
    HI_U32 u32AudEsBufSize;  /* ��Ƶ�����С     */
    HI_U32 u32SubtEsBufSize; /* ��Ļ���ݻ����С */
} HI_UNF_AVPLAY_ES_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_TS_ATTR_S
{
    HI_U32 u32BufferSize; /* buffer ��С */
} HI_UNF_AVPLAY_TS_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_PES_ATTR_S
{
    HI_U32 u32VidPesBufSize;  /* ��Ƶ�����С     */
    HI_U32 u32AudPesBufSize;  /* ��Ƶ�����С     */
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
    HI_UNF_AVPLAY_STREAM_TYPE_E enStreamType;   /* �������� */

    union
    {
        HI_UNF_AVPLAY_ES_ATTR_S stEsAttr;  /* ES ������  */
        HI_UNF_AVPLAY_TS_ATTR_S stTsAttr;  /* TS ������  */
        HI_UNF_AVPLAY_TSI_ATTR_S stTsiAttr; /* TSI ������ */
        HI_UNF_AVPLAY_PES_ATTR_S stPesAttr; /* PES ������ */
    } unStreamAttr;
} HI_UNF_AVPLAY_STREAM_ATTR_S;
#if 0
typedef enum hiUNF_AVPLAY_STATUS_E
{
    HI_UNF_AVPLAY_STATUS_STOP = 0, /* ֹͣ                 */
    HI_UNF_AVPLAY_STATUS_PLAY,     /* ����                 */
    HI_UNF_AVPLAY_STATUS_TPLAY,    /* Trick Play, ������� */
    HI_UNF_AVPLAY_STATUS_PAUSE,    /* ��ͣ                 */
    HI_UNF_AVPLAY_STATUS_EOS,      /* �������Ž���         */
    HI_UNF_AVPLAY_STATUS_RESET,    /* ��λ״̬             */

    HI_UNF_AVPLAY_STATUS_END = HI_UNF_AVPLAY_STATUS_RESET,

    HI_UNF_AVPLAY_STATUS_BUTT
} HI_UNF_AVPLAY_STATUS_E;
#endif

typedef struct hiUNF_AVPLAY_BUF_S
{
    HI_U8 * pu8Data;         /* ����ָ��            */
    HI_U32  u32Size;         /* ����                */
} HI_UNF_AVPLAY_BUF_S;

/* ֧�ֵ�ͬ������ģʽ */
typedef enum hiUNF_AVPLAY_SYNC_REF_E
{
    HI_UNF_AVPLAY_SYNC_REF_NONE = 0, /* ���ɲ���    */
    HI_UNF_AVPLAY_SYNC_REF_AUDIO,    /* ����ƵΪ׼  */
    HI_UNF_AVPLAY_SYNC_REF_VIDEO,    /* ����ƵΪ׼  */
    HI_UNF_AVPLAY_SYNC_REF_PCR,      /* �� PCR Ϊ׼ */

    HI_UNF_AVPLAY_SYNC_REF_BUTT
} HI_UNF_AVPLAY_SYNC_REF_E;

/*�¼��ص��������� */
typedef enum hiUNF_AVPLAY_EVENT_E
{
    HI_UNF_AVPLAY_EVENT_EOS = 0,           /* EOSִ�н���, NULL    */
    HI_UNF_AVPLAY_EVENT_STOP = 1,                  /* STOPִ�н���, NULL   */
    HI_UNF_AVPLAY_EVENT_SUBT_DISP = 2,             /* subtitle������ʾ, HI_UNF_SUBT_DISP_ELE_S    */
    HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE = 3, /* ý�建�����״̬�仯, HI_UNF_ES_BUF_STATE_E */
    HI_UNF_AVPLAY_EVENT_NORM_SWITCH = 4,           /* P/N ���л�, HI_UNF_ENC_FMT_E        */
    HI_UNF_AVPLAY_EVENT_NEW_SEQ = 5,               /* ��������Ϣ, HI_UNF_VIDEO_SEQUENCE_INFO_S    */
    HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME = 6,         /* ����Ƶ֡, HI_UNF_VO_FRAMEINFO_S             */
    HI_UNF_AVPLAY_EVENT_NEW_AUD_FRAME = 7,         /* ����Ƶ֡, HI_UNF_AO_FRAMEINFO_S             */
    HI_UNF_AVPLAY_EVENT_NEW_USER_DATA = 8,         /* ����Ƶ�û�����, HI_UNF_VIDEO_USERDATA_S     */
    HI_UNF_AVPLAY_EVENT_VID_STREAM = 9,            /* ��ƵPES BUF�¼�, HI_UNF_AVPLAY_STREAM_STATE_E*/

    HI_UNF_AVPLAY_EVENT_BUTT
} HI_UNF_AVPLAY_EVENT_E;

typedef HI_S32 (*HI_UNF_AVPLAY_EVENT_CB_FN)(HI_HANDLE Handle, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para);

typedef enum hiUNF_AVPLAY_PRESYNC_MODE_E
{
    HI_UNF_AVPLAY_PRESYNC_MODE_NONE = 0,    /*�ر�Ԥͬ������                               */
    HI_UNF_AVPLAY_PRESYNC_MODE_NORMAL,      /*����Ԥͬ�����ܣ������ӳٺ��ٲ���             */
    HI_UNF_AVPLAY_PRESYNC_MODE_QUICKOUTPUT, /*����Ԥͬ�����ܣ��ȿ��������һ֡������Ԥͬ�� */
    HI_UNF_AVPLAY_PRESYNC_MODE_NONE_QUICKOUTPUT,  /*�ȿ��������һ֡�����Ҳ���Ԥͬ��       */

    HI_UNF_AVPLAY_PRESYNC_MODE_BUTT
} HI_UNF_AVPLAY_PRESYNC_MODE_E;

typedef enum hiUNF_AVPLAY_REAL_MODE_E
{
    HI_UNF_AVPLAY_REAL_MODE_DISABLE = 0, /*��ʵʱ�ĵ㲥ҵ��ģʽ*/
    HI_UNF_AVPLAY_REAL_MODE_ENABLE,      /*ʵʱ��ֱ��ҵ��ģʽ*/

    HI_UNF_AVPLAY_REAL_MODE_BUTT
} HI_UNF_AVPLAY_REAL_MODE_E;

typedef enum hiUNF_AVPLAY_ATTR_ID_E
{
    HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE = 0,  /* ����ģʽ , HI_UNF_AVPLAY_STREAM_ATTR_S */

    HI_UNF_AVPLAY_ATTR_ID_ADEC,            /* ��Ƶ���� , HI_UNF_ACODEC_ATTR_S */
    HI_UNF_AVPLAY_ATTR_ID_VDEC,            /* ��Ƶ���� , HI_UNF_VCODEC_ATTR_S  */
    HI_UNF_AVPLAY_ATTR_ID_SUBT,            /* ��Ļ���� , HI_UNF_AVPLAY_SUBT_ATTR_S*/

    HI_UNF_AVPLAY_ATTR_ID_AUD_PID,     /* ��ƵPID , HI_U32*/
    HI_UNF_AVPLAY_ATTR_ID_VID_PID,      /* ��ƵPID , HI_U32*/
    HI_UNF_AVPLAY_ATTR_ID_PCR_PID,     /* PCR PID , HI_U32*/

    HI_UNF_AVPLAY_ATTR_ID_SYNC,            /* ͬ������ , HI_UNF_AVPLAY_SYNC_ATTR_S */
    HI_UNF_AVPLAY_ATTR_ID_AFD,             /* AFD �Ƿ����� HI_BOOL * */

    HI_UNF_AVPLAY_ATTR_ID_BUTT
} HI_UNF_AVPLAY_ATTR_ID_E;

/*******************************************************************************/
typedef struct hiUNF_AVPLAY_SYNC_ATTR_S
{
    HI_UNF_AVPLAY_SYNC_REF_E enSyncRef; /* ��ƵΪ׼����ƵΪ׼�����ɲ��ŵ� */
    HI_S32                   s32VidMaxSyncRange; /* ��Ƶͬ���жϵ�ʱ�䷶Χ               */
    HI_S32                   s32VidMinSyncRange; 
    HI_S32                   s32AudMaxSyncRange; /* ��Ƶͬ���жϵ�ʱ�䷶Χ               */
    HI_S32                   s32AudMinSyncRange; 
    HI_S32                   s32TimeOffset; /* ��֪������Ƶʱ��ֵΪ ��Ƶ- ��Ƶ  */

    HI_UNF_AVPLAY_PRESYNC_MODE_E enPreSyncMode; /* Ԥͬ��ģʽ */
    HI_UNF_AVPLAY_REAL_MODE_E    enRealMode;    /* ��ʵʱģʽ */

    HI_BOOL bSmoothPlay;            /* ����ʹ�� */
    HI_BOOL bClockAdjust;           /* ʱ�ӵ���ʹ�� */
} HI_UNF_AVPLAY_SYNC_ATTR_S;

typedef struct hiUNF_AVPLAY_ATTR_S
{
    /* �������� */
    HI_UNF_AVPLAY_STREAM_ATTR_S stStreamAttr; /* �������� */
} HI_UNF_AVPLAY_ATTR_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_SYNC_STATUS_S
{
    HI_U32 u32FirstAudPts;    /* ��һ����Ƶ֡ PTS         */
    HI_U32 u32FirstVidPts;    /* ��һ����Ƶ֡ PTS         */
    HI_U32 u32LastAudPts;     /* ������ŵ�һ����Ƶ֡ PTS */
    HI_U32 u32LastVidPts;     /* ������ŵ�һ����Ƶ֡ PTS */
    HI_S32 s32DiffAvPlayTime; /* ����Ƶ ����ʱ��          */
    HI_U64 u64PlayTime;       /* ��ǰ����ʱ��             */
} HI_UNF_AVPLAY_SYNC_STATUS_S;

typedef struct hiUNF_AVPLAY_BUF_STATUS_S
{
    HI_U32 u32BufId;    /* ý�建�� ��ʶ      */
    HI_U32 u32BufSize;  /* ý�建���С       */
    HI_U32 u32UsedSize; /* ý�建����ʹ�ô�С */
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
    HI_U32                          u32BitWidth;            /*  ����λ��    */
    HI_BOOL                         bBigEndian;         /*  ��С�˴洢  */
    HI_UNF_SAMPLE_RATE_E            enSampleRate;              /*PCM����������*/
        HI_BOOL                     bRawPCM;                  /*Raw��ʽPCM������ʶ*/  
}HI_UNF_PCM_PARAM_S ;

typedef struct hiUNF_ACODEC_ATTR_S{
    HI_UNF_ACODEC_TYPE_E        enType;                 /* ��������            */
    union   
    {
        HI_UNF_AAC_RAW_PARAM_S      stAacRawParam;          /* RAW AAC ��������     */
        HI_UNF_AC3_PARAM_S          stAc3Param;             /* AC3 ��������         */
        HI_UNF_PCM_PARAM_S          stPcmParam;             /*  PCM ��������        */
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
    HI_UNF_VCODEC_TYPE_E  enType;       /*��Ƶ�����豸�Ľ�������*/
    HI_UNF_VCODEC_MODE_E  enMode;       /*��Ƶ�����豸�Ľ���ģʽ*/
    HI_U32               u32ErrCover;   /*��Ƶ�����豸�����֡�����������ޣ�0:���ִ��󼴲������100:���ܴ������ȫ�����*/
    HI_U32               u32RefCover;   /*��Ƶ�����豸�Ĳο�֡�����������ޣ�0:���ִ��󼴲��ο��������������У�100:���ܴ������ȫ�������ο�*/
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
    HI_UNF_AVPLAY_STATUS_STOP = 0, /* ֹͣ                 */
    HI_UNF_AVPLAY_STATUS_PLAY,     /* ����                 */
    HI_UNF_AVPLAY_STATUS_TPLAY,    /* Trick Play, ������� */
    HI_UNF_AVPLAY_STATUS_PAUSE,    /* ��ͣ                 */
    HI_UNF_AVPLAY_STATUS_EOS,      /* �������Ž���         */
    HI_UNF_AVPLAY_STATUS_RESET,    /* ��λ״̬             */
    HI_UNF_AVPLAY_STATUS_SEEK ,    /* ��λ����             */

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
    HI_UNF_AVPLAY_SYNC_STATUS_S stSyncStatus;   /* ͬ��״̬ */
    HI_UNF_AVPLAY_STATUS_E      enRunStatus;    /* ����״̬ */

    HI_UNF_AVPLAY_BUF_STATUS_S stBufStatus[HI_UNF_AVPLAY_BUF_TYPE_NUM]; /*ý�建��״̬ */
    HI_UNF_VCODEC_STATUSINFO_S stVcodecStatus; /* ��Ƶ������״̬ */
    HI_UNF_ACODEC_STATUSINFO_S stAcodecStatus; /* ��Ƶ������״̬ */
} HI_UNF_AVPLAY_STATUS_INFO_S;

/********************************************************************************/

typedef struct hiUNF_ACODEC_DEBUGINFO_S
{
    HI_U32 u32DecFrameNum;
    HI_U32 u32ErrorFrameNum;
    HI_U32 u32BufferSize;           /* ����buffer����     */
}HI_UNF_ACODEC_DEBUGINFO_S;

typedef struct hiUNF_VCODEC_DEBUGINFO_S
{
    HI_BOOL          bEnable;       /* ʹ��״̬     */
    HI_UNF_VCODEC_TYPE_E enVCodecType;  /* ����Э��     */
    HI_U32 u32VdecThreadState;      /* �����߳�״̬ */

    /*video frame buffer status*/
    HI_U32 u32FrameQueueSize;       /* ����֡���г���   */
    HI_U32 u32CurrentQueueFrameNum; /* ��ǰδ��ȡ����֡ */

    /*video stream buffer status*/
    HI_U32 u32BufferSize;           /* ����buffer����     */
    HI_U32 u32BufferUsed;           /* ����buffer���ó��� */

    HI_U32 u32ErrorNum;             /* ��ǰ�����ĳ������� */

    HI_U32 u32TotalBits;            /* ��ǰ�����ܽ���bit��, Kbits                      */
    HI_U32 u32TotalFrame;           /* ��ǰ�����ܽ���֡��                              */
    HI_U32 u32TotalTime;            /* ��ǰ��������ʱ�䣬second                        */
    HI_U32 u32Averagebps;           /* ��ǰ������ƽ������, Kbits per second            */
    HI_U32 u32Average_fps_integer;  /* ��ǰ������ƽ��֡�ʵ���������, frames per second */
    HI_U32 u32Average_fps_decimal;  /* ��ǰ������ƽ��֡�ʵ�С������, frames per second */
}HI_UNF_VCODEC_DEBUGINFO_S, *HI_UNF_VCODEC_DEBUGINFO_S_PTR;
typedef struct hiUNF_AVPLAY_DEBUG_INFO_S
{
    HI_U32 u32VidDecFrameNum;    /* ��Ƶ����֡��           */
    HI_U32 u32VidRepeatFrameCnt; /* ��Ƶ֡��(repeat)������ */
    HI_U32 u32VidSkipFrameCnt;   /* ��Ƶ֡����(skip)����   */
    HI_U32 u32VidPlayFrameCnt;   /* ��Ƶ֡��ʾ��ʾ����     */

    HI_U32 u32AudDecFrameNum;  /* ��Ƶ����֡��   */
    HI_U32 u32AudPlayFrameCnt; /* ��Ƶ֡���Ÿ��� */

    HI_U32 u32VidRcvEsPackNum; /* �յ���ƵES������ */
    HI_U32 u32AudRcvEsPackNum; /* �յ���ƵES������ */

    HI_UNF_VCODEC_DEBUGINFO_S stVcodecInfo; /* ��Ƶ������������Ϣ */
    HI_UNF_ACODEC_DEBUGINFO_S stAcodecInfo; /* ��Ƶ������������Ϣ */
} HI_UNF_AVPLAY_DEBUG_INFO_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_STREAM_INFO_S
{
    HI_UNF_VCODEC_STREAMINFO_S stVidStreamInfo; /* ��Ƶ����Ϣ */
    HI_UNF_ACODEC_STREAMINFO_S stAudStreamInfo; /* ��Ƶ����Ϣ */
} HI_UNF_AVPLAY_STREAM_INFO_S;

/********************************************************************************/
typedef struct hiUNF_AVPLAY_I_FRAME_S
{
    HI_U8  *             pu8Addr; /*֡�����û�̬�����ַ*/
    HI_U32               u32BufSize; /*֡���ݴ�С����λ�ֽ�*/
    HI_UNF_VCODEC_TYPE_E enType;  /*��Ƭ���ݵ�Э������*/
} HI_UNF_AVPLAY_I_FRAME_S;

/********************************************************************************/
typedef enum hiUNF_AVPLAY_STOP_MODE_E
{
    HI_UNF_AVPLAY_STOP_MODE_STILL = 0,  /*stop�������һ֡*/
    HI_UNF_AVPLAY_STOP_MODE_BLACK = 1,  /*stop�����*/
    HI_UNF_AVPLAY_STOP_MODE_BUTT
} HI_UNF_AVPLAY_STOP_MODE_E;

typedef struct hiUNF_AVPLAY_STOP_OPT_S
{
    /*
      s32Timeout: end of stream timeout
      1. s32Timeout = 0   �������ȴ��������Ž�������������
      2. s32Timeout != 0  �����ȴ��������Ž���
         s32Timeout > 0   ������ʱʱ�䣬��λΪ����
         s32Timeout = -1  ���޵ȴ�
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
