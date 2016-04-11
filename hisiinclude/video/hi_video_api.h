/***********************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_video_api.h
 * Description:
 *
 * History:
 * Version   Date              Author     DefectNum    Description
 * main\1    2007-07-18        t52067     NULL         Create this file.
 ***********************************************************************************/

#ifndef __HI_VIDEO_API_H__
#define __HI_VIDEO_API_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_video_errcode.h"
#include "hi_vo_type.h"
#include "hi_vd_type.h"
#include "hi_video_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

HI_S32 HI_VID_Open(void );
HI_S32 HI_VID_Close(void);
HI_S32 HI_VID_Start(void );
HI_S32 HI_VID_Stop(void );
HI_S32 HI_VID_Pause(void );
HI_S32 HI_VID_Freeze(void);
HI_S32 HI_VID_Resume(void);
HI_S32 HI_VID_Reset(void);
HI_S32 HI_VID_GetStatus(PTR_VID_DECODER_STATE_E pState);
HI_S32 HI_VID_FlushBuffer(VID_FLUSH_MODE_E eMode);
HI_S32 HI_VID_WriteBuffer(HI_U8 * pStream , HI_U32 u32Length ,HI_U32 * pu32Length);
HI_S32 HI_VID_SetStreamSource(VID_STREAM_SOURCE_E eSource);
HI_S32 HI_VID_GetStreamSource(PTR_VID_STREAM_SOURCE_E pSource);
HI_S32 HI_VID_AbstractUserDataEnable(HI_VOID);
HI_S32 HI_VID_AbstractUserDataDisable(HI_VOID);
HI_S32 HI_VID_AbstractUserData(VID_USERDATA_S *pUserData);
HI_S32 HI_VID_SetDisplayMode(VID_DISPLAY_MODE_E eMode);
HI_S32 HI_VID_SetDisplayRate(HI_U32 u32DispTime);
HI_S32 HI_VID_SetPvrPlayMode(VID_PVR_PLAY_STATE_E enState, VID_PVR_PLAY_SPEED_E enSpeed);
HI_S32 HI_VID_GetDispIFrameNumInPvr(HI_U32* pu32FrameNum);
HI_S32 HI_VID_GetPVRTimeStamp(HI_U32 *pPCRTime,HI_U32 *pFrameNum);
HI_S32 HI_VID_SkipToNextIFrame(HI_VOID);
HI_S32 HI_VID_SetDecFrameType(VID_DECODE_FRAME_TYPE enDecFrameType);
HI_S32 HI_VID_GetPesBufferSize(HI_U32 *pu32Size);/*AI7D02836*/
HI_S32 HI_VID_GetPesBufferDataSize(HI_U32 *pu32DataSize);
HI_S32 HI_VID_GetPesBuffer(VID_PES_BUF_S *pPesBuf);
HI_S32 HI_VID_SetPesBufWtPtr(HI_U32 u32WtPtr);

HI_S32 HI_VID_GetErrorFrameNum(VID_ERROR_FRAME_NUM_S errnum);
HI_S32 HI_VID_GetPesBufState(VID_PESBUF_STATE_S *pBufState);
/*AI7D04486 09.02.04 begin*/
HI_S32 HI_VID_GetDecFirstIFrameStatus(HI_BOOL *pbDecDone);
/*AI7D04486 09.02.04 end*/
HI_S32 HI_VID_SetErrCover(VID_ERR_COVER_S *pstErrCover);
/*begin AI7D05181 2009.04.21*/
HI_S32 HI_VID_GetTotalDecodedFrames(HI_U32 *pu32Num);
/*end AI7D05181 2009.04.21*/
/*begin: added by tzg on 2009.04.29 */
HI_S32 HI_VID_GetVersatileState(VID_VERSATILE_STATE_S *pstVersatileState);
/*end: added by tzg on 2009.04.29 */
/*begin: added by tzg on 2009.05.18 */
/*
    描述: 解码I帧，具有两个特点
    1)可以不用解码到视频层，显示后通过截屏实现。
    2) 直接将解码得到的I 帧输出到应用指定的物理缓冲区中，内部完成格式转换和缩放功能。
*/
HI_S32 HI_VID_DecodeIFrame(VID_DECODE_IFRAME_S *pstIFrame);
/*begin: added by tzg on 2009.04.18 */



#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif

