/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: tunersvr_ext.h
 * Description: 
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2006-12-4   zhengguangping                
 ******************************************************************************/
#ifndef __TUNERSVR_EXT_H__
#define __TUNERSVR_EXT_H__

#include "hi_type.h"

#include "hardware.h"
#include "software.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef struct hiTuner_Propety_S
{
	HI_U8 	IsLocked;
	HI_U8 	strength;
	HI_U8 	quality;
	HI_DOUBLE ber; /*ÎóÂëÂÊ*/	
}Tuner_Propety_S;

typedef HI_VOID (*T_TunerCallback) (HI_U32 MessageID,  HI_U16 FrontEndDeviceID, HI_U16 FrontEndID );

pthread_t  TUNERSVR_TunerCreateTask(HI_VOID );

HI_S32   TUNERSVR_TunerDestroyTask ( HI_VOID);

HI_BOOL TUNERSVR_TunerTaskReady(HI_VOID);

HI_S32 TUNERSVR_TunerRegisterCallBack( T_TunerCallback pFunCallBack );

HI_S32 TUNERSVR_TunerStartLock(HI_U16 FrontEndDeviceID,  HI_U16 FrontEndID, DB_FRONTEND_S *pTunerPara);

HI_VOID  TUNERSVR_TunerStopLock(HI_U16 FrontEndDeviceID, HI_U16 FrontEndID );

HI_S32 TUNERSVR_TunerSignal(Tuner_Propety_S *pinfo);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

