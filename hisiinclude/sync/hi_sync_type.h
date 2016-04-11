/**********************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

  File Name     : hi_sync_type.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2006/7/18
Last Modified :
Description   :     This file declares  some routines which are used to register AVSYNC deriver 
and dvice into operation system or notify the other modules (audio or video)
advent of audio or video stream. In addition other routines which are called
by video module are also defined in this file.
Function List :
History       :
 **********************************************************************/

#ifndef __HI_SYNC_TYPE_H__
#define __HI_SYNC_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */


typedef enum hiAVSYNC_MODE_E
{
    AVSYNC_BYPASS      = 0,    
    AVSYNC_PCR_MASTER,
    AVSYNC_VIDEO_MASTER,
    AVSYNC_AUDIO_MASTER,
    AVSYNC_INVALID_MASTER
}AVSYNC_MODE_E;
typedef enum hiAVSYNC_ADJUST_E
{
    AVSYNC_VIDEO_ADJUST      = 1,    
    AVSYNC_AUDIO_ADJUST,
    AVSYNC_INVALID_ADJUST
}AVSYNC_ADJUST_E;

typedef  struct hiPTS_DTS_S
{
    HI_U32 u32PtsHigh;
    HI_U32 u32PtsLow;
    HI_U32 u32DtsHigh;
    HI_U32 u32DtsLow;
    HI_U32 u32Valid;
}PTS_DTS_S;

typedef enum hiPLAYBACK_TYPE_E
{
    SYNC_AUDIO_PLAYBACK = 0,
    SYNC_VIDEO_PLAYBACK,
    SYNC_AV_PLAYBACK,
    SYNC_NO_PLAYBACK
}PLAYBACK_TYPE_E;
typedef struct hiPLAYBACK_S
{
    PLAYBACK_TYPE_E eplayback;
    HI_U32 playBackTime;
}PLAYBACK_S;

typedef enum hiSYNC_VID_PES_BUFFER_SOURCE_E
{
    SYNC_VID_PES_BUFFER_SOURCE_LIVE = 0,
    SYNC_VID_PES_BUFFER_SOURCE_HDD
}SYNC_VID_PES_BUFFER_SOURCE_E;

typedef struct hiSYNC_GET_AV_PTSValue_S				/*AI7D04362*/
{
    HI_U32 nvidPts;
	HI_U32 naudPts;
    HI_S32 vidDiff;
    HI_S32 audDiff;
}SYNC_GET_AV_PTSValue_S;

/* begin:add by x57522 */
typedef enum hiSYNC_PRESYNC_MODE_E
{
    SYNC_PRESYNC_MODE_NONE = 0,          /*关闭预同步功能*/
    SYNC_PRESYNC_MODE_BLACK,             /*开启预同步功能，同步前黑屏 同步后开始播放*/
    SYNC_PRESYNC_MODE_QUICKOUTPUT,       /*开启预同步功能，同步前快速输出第一帧 同步后开始播放*/
    SYNC_PRESYNC_MODE_BUTT
}SYNC_PRESYNC_MODE_E;


typedef struct hiSYNC_RANGE_S
{
    HI_S32 s32VidMaxRange;   /* 单位:ms */
    HI_S32 s32VidMinRange;
    HI_S32 s32AudMaxRange;
    HI_S32 s32AudMinRange;
}SYNC_RANGE_S;
/* end */


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /* __HI_SYNC_TYPE_H__ */

