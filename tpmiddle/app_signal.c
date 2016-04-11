#include "app_common.h"
#include "app_ipi_common.h"

BU32 signalState = APP_SIGNAL_LOCK;
BU32 signalLever = 0;
BU32 signalSNR = 0;
BU32 signalBER = 0x09630008;



/*-------------------------------------------------------------------------------

	Description:获得信号锁定状态
	返回值：APP_SIGNAL_LOCK      锁定
			 	 APP_SIGNAL_UNLOCK    未锁定

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Signal_API_GetSignalState(void)
{
	if (FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
	{
		return APP_SIGNAL_LOCK;
	}	
	return APP_SIGNAL_UNLOCK;
}

/*-------------------------------------------------------------------------------

	Description:获得信号电平
	返回值：信号电平大小  单位dbuV
			 范围 0 ~ 103 dbuV

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Signal_API_GetSignalLever(void)
{
	FYF_API_tuner_ioctl(0, FYF_TUNER_GET_LEVEL, (void *)&signalLever);
	return signalLever;
}

/*-------------------------------------------------------------------------------

	Description:获得信噪比
	返回值：信噪比大小  单位db
			 范围 0 ~ 40 db

	Parameters:

-------------------------------------------------------------------------------*/
BU08 APP_Signal_API_GetSignalSNR(void)
{
	FYF_API_tuner_ioctl(0, FYF_TUNER_GET_SNR, (void *)&signalSNR);
	return signalSNR;
}

/*-------------------------------------------------------------------------------

	Description:获得误码率
	返回值：误码率
	         数据格式:0xAA BB CC DD 代表误码率为 0xAA.0XBB E-0XDD
	         比如函数返回值为0x09 63 XX 08 代表误码率为 9.99 E-8

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Signal_API_GetSignalBER(void)
{
	FYF_API_tuner_ioctl(0, FYF_TUNER_GET_BER, (void *)&signalBER);
	return signalBER;
}

void APP_signal_IPI_LockTuner(FYF_QUEUE_MESSAGE msgRev)
{
	BU32 i = 0;
	BU32 tunerstate = FYF_TUNER_STATUS_FAIL;
	//APP_IPI_SetPlayFlag(0);
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
	APP_IPI_SaveTunerInfo(msgRev.q2ndWordOfMsg, msgRev.q3rdWordOfMsg, msgRev.q4thWordOfMsg);
	FYF_API_tuner_lock_delivery(0, APP_IPI_GetTunerFreq() * 1000, APP_IPI_GetTunerSym(), APP_IPI_GetTunerMod(), APP_IPI_GetTunerFreq());
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());

	tunerstate = FYF_TUNER_STATUS_FAIL;
	for(i =0 ;i < 40 ;i++)
	{
		if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
		{
			tunerstate = FYF_TUNER_STATUS_SUCCESS;
			break;
		}
		/*if(APP_IPI_GetPlayFlag() == 1)
		{
			APP_IPI_SetPlayFlag(0);
			tunerstate = FYF_TUNER_STATUS_FAIL;
			break;				
		}
		*/
		FYF_API_thread_sleep(50);
	}
	
}

/*-------------------------------------------------------------------------------

	Description:锁定到新频点

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Signal_API_TuneToNewParam(BU32 freqMHZ,BU32 sysKbps,APP_CHN_QAM_E qam)
{
	static FYF_QUEUE_MESSAGE msgSend;

	msgSend.qlstWordOfMsg = APP_Queue_LockTuner_e;
	msgSend.q2ndWordOfMsg = freqMHZ;
	msgSend.q3rdWordOfMsg = sysKbps;
	msgSend.q4thWordOfMsg = qam;


	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
	APP_IPI_SaveTunerInfo(msgSend.q2ndWordOfMsg, msgSend.q3rdWordOfMsg, msgSend.q4thWordOfMsg);
	FYF_API_tuner_lock_delivery(0, APP_IPI_GetTunerFreq() * 1000, APP_IPI_GetTunerSym(), APP_IPI_GetTunerMod(), APP_IPI_GetTunerFreq());
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
	//FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
}

/*-------------------------------------------------------------------------------

	Description:发消息锁定到新频点

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Signal_API_SendMsgToTune(BU32 freqMHZ,BU32 sysKbps,APP_CHN_QAM_E qam)
{
	static FYF_QUEUE_MESSAGE msgSend;

	msgSend.qlstWordOfMsg = APP_Queue_LockTuner_e;
	msgSend.q2ndWordOfMsg = freqMHZ;
	msgSend.q3rdWordOfMsg = sysKbps;
	msgSend.q4thWordOfMsg = qam;

	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
}




