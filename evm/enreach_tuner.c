/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_tuner.c
* Description: 该文件实现浏览器、oc库定义的tuner接口
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#include "hi_type.h"
//#include "hi_svr_public.h"
//#include "hi_unf_tuner.h"
//#include "hi_unf_trd.h"

#include "fyf_def.h"
#include "fyf_tuner.h"

#include "enreach_pub.h"
#include "enreach_erro.h"
#include "enreach_oc_drv.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static HI_S32 enreachTunerLock(HI_U32 u32Freq, HI_U32 u32SymbolRate, HI_U32 u32Qam)
{

#if 0
	HI_S32 s32Ret = HI_NULL;
	HI_UNF_MODULATION_TYPE_E eQam = HI_UNF_MOD_TYPE_QAM_64;

	/* ucQam==3 表示QAM64, */

	if (3 == u32Qam)
	{
		eQam = TUNER_MODULATION_QAM64;
	}

	EVM_INFO("lock tuner, freq = %d, symbol = %d, qam = %d ",u32Freq * 1000, u32SymbolRate * 1000, eQam);
	s32Ret = HI_TRD_TUNER_Lock(0, u32Freq * 1000, u32SymbolRate * 1000, eQam, NULL);

	ENREACH_RETURN((HI_SUCCESS != s32Ret), OS_ERROR);

	return OS_OK;
#else

	EVM_INFO("Enter, freq:%u, symbolRate:%u, Qam:%u", u32Freq, u32SymbolRate, u32Qam);

	BS32 ret = 0;

     u32Qam = FYF_TUNER_MOD_QAM64;
 
     FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
     ret = FYF_API_tuner_lock_delivery(0, (BS32)(u32Freq * 1000), (BS32)u32SymbolRate, (BS32)(u32Qam), (BS32)u32Freq);
     if (ret != FYF_OK)
     {
         FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());

	return 1;
     }
	 
     APP_IPI_SaveTunerInfo((BS32)u32Freq, (BS32)u32SymbolRate, (BS32)(u32Qam));
     FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());

	return FYF_OK;

#endif

}

/************************** OC库定义的锁频接口 **************************/

/* ---- OC库定义的锁频接口 ---- */

OC_OS_STATUS OC_SetTuner(OC_SERVICE_INFO OCSrvInfo)
{
    return enreachTunerLock(OCSrvInfo.uiFreq, OCSrvInfo.uiSymRat, OCSrvInfo.ucQam);
}

/************************** 浏览器定义的锁频接口 ************************/

/* ---- 浏览器定义的锁频接口 ---- */

char Ebrowser_SetTuner(unsigned int u32Freq, unsigned int u32SymbolRate, unsigned int u32Qam)
{
    return enreachTunerLock(u32Freq, u32SymbolRate, u32Qam);    
}

char Callback_setTuner(unsigned int freq, unsigned int SymbolRate,unsigned int qam)
{
    return enreachTunerLock(freq, SymbolRate, qam);    
}

#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

