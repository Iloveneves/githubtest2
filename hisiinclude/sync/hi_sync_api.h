/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_sync_api.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2007/1/19
Last Modified :
Description   : 
Function List :
History       :
 ******************************************************************************/
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_sync_errcode.h"
#include "hi_sync_type.h"

#ifndef __HI_SYNC_API_H__
#define __HI_SYNC_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

HI_S32 HI_SYNC_Open(HI_VOID);
HI_S32 HI_SYNC_Close(HI_VOID);
HI_S32 HI_SYNC_SetSyncMode(AVSYNC_MODE_E enMode);
HI_S32 HI_SYNC_GetSyncMode(AVSYNC_MODE_E *penMode);
HI_S32 HI_SYNC_Playback(PLAYBACK_S* pPlayBack);
HI_S32 HI_SYNC_Reset(HI_VOID);
HI_S32 HI_SYNC_Get_AV_PTSValue(HI_UL *nvideoPts,HI_UL *naudioPts,HI_S32 *VidDiff,HI_S32 *AudDiff);			/*AI7D04362*/

/* begin:add by x57522 */
HI_S32 HI_SYNC_SetPreSyncMode(SYNC_PRESYNC_MODE_E enPreSyncMode);
HI_S32 HI_SYNC_GetPreSyncMode(SYNC_PRESYNC_MODE_E *penPreSyncMode);
HI_S32 HI_SYNC_SetSyncRange(SYNC_RANGE_S *pstAVSyncRange);
HI_S32 HI_SYNC_GetSyncRange(SYNC_RANGE_S *pstAVSyndRange);
/* end */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HI_AUDIO_API_H__ */

