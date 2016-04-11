/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: Trd_enreach_init.h
* Description:
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#ifndef __TRD_ENREACH_INIT_H__
#define __TRD_ENREACH_INIT_H__

#include "hi_type.h"
//#include "hi_s2_svr.h"
#include "fyf_def.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct hiTRD_FREQINFO_S
{
    BU32       frq;  
    BU32       sym;           
    BU32       eqamType;
} HI_TRD_FREQINFO_S;

typedef struct hiTRD_ENREACH_DB_S
{
    HI_TRD_FREQINFO_S 	stFreqInfo;  /* 频点信息          */
    HI_U32 				u32ServiceId;           /* 数据广播serviceid */
    HI_VOID 				*pNext;
} HI_TRD_ENREACH_DB_S;


#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __TRD_ENREACH_INIT_H__ */

