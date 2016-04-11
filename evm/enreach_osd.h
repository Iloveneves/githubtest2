/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_osd.h
* Description:
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#ifndef __ENREACH_OSD_H__
#define __ENREACH_OSD_H__

#include "hi_type.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern HI_U8 *evm_osd_buf;
extern HI_U8  *osd_tmp_buf;

HI_S32 OSD_EnreachGetBufRect(HI_U32 *pu32Width, HI_U32 *pu32Height);
HI_S32 OSD_EnreachGetBufAddr(HI_U32 *pu32ScreenBuf);
void OSD_EnreachFreeBufAddr(void);
HI_VOID OSD_DrawLogo(HI_VOID);

HI_VOID OSD_LockOSD(HI_VOID);
HI_VOID OS_UnlockOSD(HI_VOID);

#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

