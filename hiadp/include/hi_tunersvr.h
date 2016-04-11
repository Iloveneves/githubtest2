/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: tunersvr.h
 * Description: 
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2008-09-01   diaoqiangwei/d60770                
 ******************************************************************************/

#ifndef __TUNERSVR_H__
#define __TUNERSVR_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif



#define SI_POSITIVE 				1
#define SI_NEGATIVE				0
#define SI_AUTO 					2
#define TUNER_DEFAULT_SI    		SI_NEGATIVE 

#define FRE_MIN_VAL 			51000 /*频率的最小值*/
#define FRE_MAX_VAL  			858000 /*频率的最大值*/

#define SYMRATE_MIN_VAL 		3500000/*符号率的最小值*/
#define SYMRATE_MAX_VAL 		7000000/*符号率的最大值*/



typedef enum hiTUNERSVR_STATE_E
{
    TUNER_IDLE  		= 0x01,   /* IDLE状态*/
    TUNER_LOCKING   	= 0x02,	/* 正在锁频状态*/
    TUNER_LOCKED  	= 0x03	/* 锁定状态*/
} TUNERSVR_STATE_E;

static HI_VOID TunerTimerProc(HI_VOID *pArg);
static HI_S32 TunerStartTimer(HI_VOID);
static HI_VOID  TunerTunerCallBack( HI_U32 u32MessageID, HI_U16 FrontEndDeviceID,  HI_U16 FrontEndID);
static HI_S32 TunerConnectTuner(HI_U16 DeviceID, DB_FRONTEND_S  struTunerFrontEnd );
static HI_S32 TunerLockCheck(HI_VOID);
static HI_VOID TunerGetSignalProperty(HI_VOID);
static HI_S32  TunerSendMsg(MSG_MESSAGE_S *pstruTunerMsg);
static HI_VOID TunerTaskProc (HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
