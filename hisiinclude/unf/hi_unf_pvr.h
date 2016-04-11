/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_pvr.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/09
  Description   : hi_pvr_api.h header file
  History       :
  1.Date        : 2008/04/09
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_UNF_PVR_H__
#define __HI_UNF_PVR_H__


#include "hi_type.h"
#include "hi_ecs_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef __HI_PVR_TYPE_DEF__
#define __HI_PVR_TYPE_DEF__

#define PVR_STUB_SC_BUF_SZIE (7*4) /*(sizeof(DMX_IDX_DATA_S)*20)*/
#define PVR_STUB_TSDATA_SIZE (188*12)

/***********************************************************
                  Macro Definition
 ***********************************************************/
#define PVR_MAX_PID_NUM                 32
#define PVR_MAX_FILENAME_LEN            128
#define PVR_MAX_CIPHER_KEY_LEN          128
#define PVR_MAX_USERDATA_LEN            (1024*1024)


/***********************************************************
                Data Structure Definition
 ***********************************************************/

/***** data structure for common *****/

/* struct for cipher                                                        */
typedef struct hiPVR_CIPHER_S
{
    HI_BOOL             bDoCipher;                             /* whether encryt/decrypt */
    CIPHER_ALG_E        enType;                              /* 3DES/DES/AES */
    HI_U8               au8Key[PVR_MAX_CIPHER_KEY_LEN];        /* key */
    HI_U32              u32KeyLen;                             /* length of key */
}PVR_CIPHER_S;

/* type of event                                                            */
typedef enum hiPVR_EVENT_E
{

    PVR_EVENT_PLAY_EOF        = 0x001,                         /* end fo file */
    PVR_EVENT_PLAY_SOF        = 0x002,                         /* start of file */
    PVR_EVENT_PLAY_ERROR      = 0x003,                         /* play error */
    PVR_EVENT_PLAY_REACH_REC  = 0x004,                         /* timeshift play reach to record end */
    PVR_EVENT_PLAY_RESV       = 0x00f,                         /* reserved for play */
    PVR_EVENT_REC_DISKFULL    = 0x010,                         /* disk full */
    PVR_EVENT_REC_ERROR       = 0x011,                         /* record error */
    PVR_EVENT_REC_OVER_FIX    = 0x012,                         /* record size will over fix size */
    PVR_EVENT_REC_REACH_PLAY  = 0x013,                         /* timeshift record reach to play positon */
    PVR_EVENT_REC_RESV        = 0x01f,                         /* reserved for record */
    PVR_EVENT_BUTT            = 0x020
} PVR_EVENT_E;


/* judgement of event class                                                 */
#define PVR_EVENT_PLAYEVENT(event) (((event) >= PVR_EVENT_PLAY_EOF) && ((event) <= PVR_EVENT_PLAY_RESV))
#define PVR_EVENT_RECEVENT(event)  (((event) >= PVR_EVENT_REC_DISKFULL) && ((event) <= PVR_EVENT_REC_RESV))

/* callback function define *//* PVR模块，建议将事件作为参数也传给回调函数, AI7D02612 */
typedef void (*eventCallBack)(HI_U32 u32ChnID, PVR_EVENT_E EventType, HI_S32 s32EventValue, HI_VOID *args);


/* type of stream                                                           */
typedef enum hiPVR_STREAM_TYPE_E
{
    PVR_STREAM_TYPE_TS,
    PVR_STREAM_TYPE_PES,
    PVR_STREAM_TYPE_ALL_TS,
    PVR_STREAM_TYPE_OTHER,
    PVR_STREAM_TYPE_BUTT
} PVR_STREAM_TYPE_E;

/* 非问题单, 加入整机需要的文件属性获取功能 */
typedef struct hiPVR_FILE_ATTR_S
{
    HI_U32               u32FrameNum;  /* when index for audio, frame number means pes number */
    HI_U32               u32PlayTimeInMs;
    HI_U32               u32StartTimeInMs;
    HI_U32               u32EndTimeInMs;
    HI_U64               u64ValidSizeInByte;
} PVR_FILE_ATTR_S;

/***** data structs for rec *****/

#define PVR_REC_MIN_DAV_BUF  (188)
#define PVR_REC_MAX_DAV_BUF  (188*60)
#define PVR_REC_MIN_SC_BUF   (7)
#define PVR_REC_MAX_SC_BUF   (7*20)

/* record states                                                            */
/*
INVALID -> INIT -> RUNNING -> STOPPING ->    STOP ->    INVALID
                             (saveIndex)  (saveStream)
*/
typedef enum hiPVR_REC_STATE_E
{
  PVR_REC_STATE_INVALID,                                      /* invalid  */
  PVR_REC_STATE_INIT,                                         /* initial */
  PVR_REC_STATE_RUNNING,                                      /* running */
  PVR_REC_STATE_STOPPING,                                     /* stopping */
  PVR_REC_STATE_STOP,                                         /* stopped */
  PVR_REC_STATE_BUTT
} PVR_REC_STATE_E;

typedef enum hiPVR_REC_INDEX_TYPE_E
{
    PVR_REC_INDEX_TYPE_NONE,
    PVR_REC_INDEX_TYPE_VIDEO,
    PVR_REC_INDEX_TYPE_AUDIO,
    PVR_REC_INDEX_TYPE_BUTT
} PVR_REC_INDEX_TYPE_E;

/* record attribute structure                                               */
typedef struct hiPVR_REC_ATTR_S
{
    PVR_REC_INDEX_TYPE_E enIndexType;                           /* index for video or audio */
    HI_U32               u32IndexPid;                           /* index PID */
    HI_U32               u32DemuxID;                            /* record demux ID */

    HI_U32               u32DavBufSize;                         /* buffer size for data stream */
    HI_U32               u32ScdBufSize;                         /* buffer size for startCode */
    HI_U32               u32UsrDataInfoSize;                    /* max size of manage info for user(save to idx file) */

    HI_BOOL              bIsClearStream;                        /* the souce stream is clear stream or descrambered */
    PVR_STREAM_TYPE_E    enStreamType;                          /* stream type */
    PVR_CIPHER_S         stEncryptCfg;                          /* cipher structure */

    HI_U64               u64MaxFileSize;                        /* 0 means no limit, else rewind */
    HI_BOOL              bRewind;                               /* HI_TRUE: rewind; HI_FALSE:stop when reach maxFileSize */
    HI_CHAR              szFileName[PVR_MAX_FILENAME_LEN];      /* name of record file */
    HI_U32               u32FileNameLen;                        /* length of record file name */
}PVR_REC_ATTR_S;

/* record status                                                            */
typedef struct hiPVR_REC_STATUS_S
{
    PVR_REC_STATE_E     enState;                           /* record state */
    HI_U64              u64CurWritePos;                    /* current write position */
    HI_U32              u32CurWriteFrame;
    HI_U32              u32CurTimeInMs;
    HI_U32              u32StartTimeInMs;
    HI_U32              u32EndTimeInMs;
}PVR_REC_STATUS_S;


/***** data structs for play *****/

/* play direction for trick mode                                            */
#define PVR_PLAY_DIRECTION_FORWARD             (1)             /* forward */
#define PVR_PLAY_DIRECTION_BACK                (-1)            /* backward */

/* play position type                                                       */
typedef enum hiPVR_PLAY_POS_TYPE_E
{
  PVR_PLAY_POS_TYPE_SIZE,                                   /* by file size  */
  PVR_PLAY_POS_TYPE_TIME,                                   /* by time stamp */
  PVR_PLAY_POS_TYPE_FRAME,                                  /* by frame */
  PVR_PLAY_POS_TYPE_BUTT
} PVR_PLAY_POS_TYPE_E;

/* play states                                                              */
typedef enum hiPVR_PALY_STATE_E
{
    PVR_PLAY_STATE_INVALID,                                  /* invalid */
    PVR_PLAY_STATE_INIT,                                     /* initial */
    PVR_PLAY_STATE_PLAY,                                     /* play */
    PVR_PLAY_STATE_PAUSE,                                    /* pause */
    PVR_PLAY_STATE_FF,                                       /* fast forward */
    PVR_PLAY_STATE_FB,                                       /* fast backward */
    PVR_PLAY_STATE_SF,                                       /* slow forward */
    PVR_PLAY_STATE_STEPF,                                    /* step forward, not supported now */
    PVR_PLAY_STATE_STEPB,                                    /* step back */
    PVR_PLAY_STATE_STOP,                                     /* stop */
    PVR_PLAY_STATE_BUTT
} PVR_PLAY_STATE_E;

/* speed of trick mode                                                        */
typedef enum tagPVR_PLAY_SPEED_E
{
    PVR_PLAY_SPEED_NORMAL            = 1024,
    PVR_PLAY_SPEED_2X_FAST_FORWORD   = 2 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_4X_FAST_FORWORD   = 4 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_8X_FAST_FORWORD   = 8 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_16X_FAST_FORWORD  = 16 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_32X_FAST_FORWORD  = 32 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_2X_FAST_BACKWORD  = -2 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_4X_FAST_BACKWORD  = -4 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_8X_FAST_BACKWORD  = -8 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_16X_FAST_BACKWORD = -16 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_32X_FAST_BACKWORD = -32 * PVR_PLAY_SPEED_NORMAL,
    PVR_PLAY_SPEED_2X_SLOW_FORWORD   = PVR_PLAY_SPEED_NORMAL / 2,
    PVR_PLAY_SPEED_4X_SLOW_FORWORD   = PVR_PLAY_SPEED_NORMAL / 4,
    PVR_PLAY_SPEED_8X_SLOW_FORWORD   = PVR_PLAY_SPEED_NORMAL / 8,
    PVR_PLAY_SPEED_16X_SLOW_FORWORD  = PVR_PLAY_SPEED_NORMAL / 16,
    PVR_PLAY_SPEED_32X_SLOW_FORWORD  = PVR_PLAY_SPEED_NORMAL / 32,
    PVR_PLAY_SPEED_2X_SLOW_BACKWORD  = PVR_PLAY_SPEED_NORMAL / (-2),
    PVR_PLAY_SPEED_4X_SLOW_BACKWORD  = PVR_PLAY_SPEED_NORMAL / (-4),
    PVR_PLAY_SPEED_8X_SLOW_BACKWORD  = PVR_PLAY_SPEED_NORMAL / (-8),
    PVR_PLAY_SPEED_16X_SLOW_BACKWORD = PVR_PLAY_SPEED_NORMAL / (-16),
    PVR_PLAY_SPEED_32X_SLOW_BACKWORD = PVR_PLAY_SPEED_NORMAL / (-32),
    PVR_PLAY_SPEED_BUTT
} PVR_PLAY_SPEED_E;

/* play speed                                                               */
typedef struct hiPVR_PLAY_MODE_S
{
    PVR_PLAY_SPEED_E               enSpeed;                             /* will be descripted by enum above */
}PVR_PLAY_MODE_S;

/* play attribute structure                                                 */
typedef struct hiPVR_PLAY_ATTR_S
{
    HI_U32               u32DemuxID;                           /* play demux ID */

    PVR_STREAM_TYPE_E    enStreamType;                         /* stream type */
    HI_BOOL              bIsClearStream;                       /* whether to the source stream is clear */

    PVR_CIPHER_S         stDecryptCfg;                         /* need cipher? */

    HI_CHAR              szFileName[PVR_MAX_FILENAME_LEN];     /* play file name */
    HI_U32               u32FileNameLen;                       /* length of play file name */
}PVR_PLAY_ATTR_S;


/* play position                                                            */
typedef struct hiPVR_PLAY_POSITION_S
{
    PVR_PLAY_POS_TYPE_E  enPositionType;                       /* type of position */
    HI_S64               s64Offset;                            /* offset time: ms */
    HI_S32               s32Whence;                            /* start where */
}PVR_PLAY_POSITION_S;

/* create idx file attr */
typedef struct hiPVR_GEN_IDX_ATTR_S
{
    HI_U32 u32UsrDataInfoLen;
    HI_U32 u32IdxPid;
    PVR_REC_INDEX_TYPE_E enIdxType;
}PVR_GEN_IDX_ATTR_S;

/* play status                                                              */
typedef struct hiPVR_PLAY_STATUS_S
{
    PVR_PLAY_STATE_E     enState;                              /* play state */
    PVR_PLAY_SPEED_E     enSpeed;                              /* will be descripted by enum above */
    HI_U64               u64CurPlayPos;                       /* current write position */
    HI_U32               u32CurPlayFrame;
    HI_U32               u32CurPlayTimeInMs;
}PVR_PLAY_STATUS_S;

#endif /* __HI_PVR_TYPE_DEF__ */

/***********************************************************
                API Declaration
 ***********************************************************/

/***** APIs for PVR recode *****/

/* initialize and de-initialize of record module                            */
HI_S32 HI_UNF_PVR_RecInit(HI_VOID);
HI_S32 HI_UNF_PVR_RecDeInit(HI_VOID);

/* applay and release new record channel                                    */
HI_S32 HI_UNF_PVR_RecNewChn(HI_U32 *pu32ChnID, const PVR_REC_ATTR_S *pstRecAttr);
HI_S32 HI_UNF_PVR_RecFreeChn(HI_U32 u32ChnID);

/* set and get attributes of record channel                                 */
HI_S32 HI_UNF_PVR_RecSetChn(HI_U32 u32ChnID, const PVR_REC_ATTR_S *pstRecAttr);
HI_S32 HI_UNF_PVR_RecGetChn(HI_U32 u32ChnID, PVR_REC_ATTR_S *pstRecAttr);

/* start and stop record channel                                            */
HI_S32 HI_UNF_PVR_RecStartChn(HI_U32 u32ChnID);
HI_S32 HI_UNF_PVR_RecStopChn(HI_U32 u32ChnID);

/* get record status                                                        */
HI_S32 HI_UNF_PVR_RecGetStatus(HI_U32 u32ChnID, PVR_REC_STATUS_S *pstRecStatus);


/***** APIs for PVR play *****/

/* initialize and de-initialize of play module                              */
HI_S32 HI_UNF_PVR_PlayInit(HI_VOID);
HI_S32 HI_UNF_PVR_PlayDeInit(HI_VOID);

/* apply and release new play channel                                       */
HI_S32 HI_UNF_PVR_PlayNewChn(HI_U32 *pu32ChnID, const PVR_PLAY_ATTR_S *pstPlayAttr);
HI_S32 HI_UNF_PVR_PlayFreeChn(HI_U32 u32ChnID);

/* start and stop time shift                                                */
HI_S32 HI_UNF_PVR_PlayStartTimeShift(HI_U32 *pu32PlayChnID, HI_U32 u32DemuxID, HI_U32 u32RecChnID);
HI_S32 HI_UNF_PVR_PlayStopTimeShift(HI_U32 u32PlayChnID);

/* set and get attributes of play channel                                   */
HI_S32 HI_UNF_PVR_PlaySetChn(HI_U32 u32ChnID, const PVR_PLAY_ATTR_S *pstPlayAttr);
HI_S32 HI_UNF_PVR_PlayGetChn(HI_U32 u32ChnID, PVR_PLAY_ATTR_S *pstPlayAttr);

/* start and stop play channel                                              */
HI_S32 HI_UNF_PVR_PlayStartChn(HI_U32 u32ChnID);
HI_S32 HI_UNF_PVR_PlayStopChn(HI_U32 u32ChnID);

/* pause and resume play channel                                            */
HI_S32 HI_UNF_PVR_PlayPauseChn(HI_U32 u32ChnID);
HI_S32 HI_UNF_PVR_PlayResumeChn(HI_U32 u32ChnID);

/* get play status                                                          */
HI_S32 HI_UNF_PVR_PlayGetStatus(HI_U32 u32ChnID, PVR_PLAY_STATUS_S *pstStatus);

/* locate play position                                                     */
HI_S32 HI_UNF_PVR_PlaySeek(HI_U32 u32ChnID, const PVR_PLAY_POSITION_S *pstPosition);

/* start trick mode of playing                                              */
HI_S32 HI_UNF_PVR_PlayTrickMode(HI_U32 u32ChnID, const PVR_PLAY_MODE_S *pstTrickMode);

/* start step back frame by frame                                           */
HI_S32 HI_UNF_PVR_PlayStep(HI_U32 u32ChnID, HI_S32 s32Direction);

/* get file info */
HI_S32 HI_UNF_PVR_PlayGetFileAttr(HI_U32 u32ChnID, PVR_FILE_ATTR_S *pAttr);

/* get file attr, no need to new play channel */
HI_S32 HI_UNF_PVR_GetFileAttrByFileName(const HI_CHAR *pFileName, PVR_FILE_ATTR_S *pAttr);

HI_S32 HI_UNF_PVR_SetUsrDataInfoByFileName(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32UsrDataLen);

HI_S32 HI_UNF_PVR_GetUsrDataInfoByFileName(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32BufLen, HI_U32* pUsrDataLen);


/***** APIs for PVR event callback *****/
/* register and un-register envent callback function    AI7D02612                    */
HI_S32 HI_UNF_PVR_RegisterEvent(PVR_EVENT_E enEventType, eventCallBack callBack, HI_VOID *args);

/* we adapt macro define for unregister function, it'll cut code scale.     */
#define HI_UNF_PVR_UnRegisterEvent(enEventType) HI_UNF_PVR_RegisterEvent(enEventType, HI_NULL_PTR, HI_NULL_PTR)

HI_S32 HI_UNF_PVR_CreateIdxFile(HI_U8* pstTsFileName, HI_U8* pstIdxFileName, PVR_GEN_IDX_ATTR_S* pAttr);

HI_VOID HI_UNF_PVR_ConfigDebugInfo(HI_U32 u32DebugLevel);

/* remove ts data file(etc: "data.ts") and the "data.ts.idx" index file,
*  and if file size > 4G, remove all ts data file, like:
*      data.ts, data.ts.0001, data.ts.0002, data.ts.0003 ...
*  just pass "data.ts" to pFileName
*/
HI_VOID HI_UNF_PVR_RemoveFile(const HI_CHAR *pFileName);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __HI_UNF_PVR_H__ */
