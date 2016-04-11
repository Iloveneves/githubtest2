/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/09/23                                           	  */
/******************************************************************************/ 
#ifndef __FYF_TUNER_H__
#define __FYF_TUNER_H__
typedef enum
{
	FYF_TUNER_STATUS_SUCCESS,
	FYF_TUNER_STATUS_FAIL
}FYF_TUNER_STATUS;
#define FYF_TUNER_LOCKED FYF_TUNER_STATUS_SUCCESS
#define FYF_TUNER_LOST FYF_TUNER_STATUS_FAIL
#define FYF_TUNER_NOSENDMSG	(0x80)
typedef void (*FYF_TUNER_NOTIFY_FUNC)(BU32 id, FYF_TUNER_STATUS status); 
typedef enum
{
	FYF_TUNER_GET_QUALITY=1,
	FYF_TUNER_GET_STRENGTH,
	FYF_TUNER_GET_BER,
	FYF_TUNER_GET_LEVEL,
	FYF_TUNER_GET_SNR,
	FYF_TUNER_TIME_OUT		/* ËøtunerÑÓÊ± */
}FYF_TUNER_IOCTL_e;
typedef struct
{
	BS32 (*FYF_CPI_tuner_set_notify)(FYF_TUNER_NOTIFY_FUNC func);
	BS32 (*FYF_CPI_tuner_lock_delivery)(BS32 tunerid, BS32 frequency, BS32 symbol_rate, BS32 modulation, BS32 request_id);
	BS32 (*FYF_CPI_tuner_get_signal_quality)(BS32 tunerid);
	BS32 (*FYF_CPI_tuner_get_signal_strength)(BS32 tunerid);
	BS32 (*FYF_CPI_tuner_get_signal_ber)(BS32 tunerid);
	BS32 (*FYF_CPI_tuner_get_status)(BS32 tunerid);
	BS32 (*FYF_CPI_tuner_ioctl)(BS32 tunerid, FYF_TUNER_IOCTL_e op, void *arg); 
}FYF_TUNER_PORTING;
typedef enum
{
	FYF_TUNER_MOD_QAM16 = 0,
	FYF_TUNER_MOD_QAM32 = 1,
	FYF_TUNER_MOD_QAM64 = 2,
	FYF_TUNER_MOD_QAM128= 3,
	FYF_TUNER_MOD_QAM256= 4
}FYF_TUNER_MOD_e;
/******************************************************************************/
/*Description: register tuner                                                 */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_tuner_register(FYF_TUNER_PORTING *porting);
/******************************************************************************/
/*Description: set notify func                                                */
/*Input      : func														      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_tuner_set_notify(FYF_TUNER_NOTIFY_FUNC func);
/******************************************************************************/
/*Description: tuner lock delivery                                            */
/*Input      : tunerid,frequency,symbol_rate,modulation,request_id  	      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_tuner_lock_delivery(BS32 tunerid, BS32 frequency, BS32 symbol_rate, BS32 modulation, BS32 request_id);
/******************************************************************************/
/*Description: get signal quality                                             */
/*Input      : tunerid												  	      */
/*Output     : no                                                             */
/*Return     : >0 success, -1 failure                                         */
/******************************************************************************/
BS32 FYF_API_tuner_get_signal_quality(BS32 tunerid);
/******************************************************************************/
/*Description: get signal strength                                            */
/*Input      : tunerid												  	      */
/*Output     : no                                                             */
/*Return     : >0 success, -1 failure                                         */
/******************************************************************************/
BS32 FYF_API_tuner_get_signal_strength(BS32 tunerid);
/******************************************************************************/
/*Description: get signal ber                                                 */
/*Input      : tunerid												  	      */
/*Output     : no                                                             */
/*Return     : >0 success, -1 failure                                         */
/******************************************************************************/
BS32 FYF_API_tuner_get_signal_ber(BS32 tunerid);
/******************************************************************************/
/*Description: get status                                                     */
/*Input      : tunerid												  	      */
/*Output     : no                                                             */
/*Return     : FYF_TUNER_LOST£ºno locked,FYF_TUNER_LOCKED£ºlocked            */
/*			   FYF_ERR: failure                                               */
/******************************************************************************/
BS32 FYF_API_tuner_get_status(BS32 tunerid);
/******************************************************************************/
/*Description: ioctl                                                          */
/*Input      : tunerid,op       									  	      */
/*Output     : arg                                                            */
/*Return     : FYF_OK£ºsuccess£»FYF_ERR£ºfail								  */
/******************************************************************************/
BS32 FYF_API_tuner_ioctl(BS32 tunerid, FYF_TUNER_IOCTL_e op, void *arg);

BS32 FYF_API_tuner_set_dev_addr(BU08 addr);
#endif







