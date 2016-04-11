/*
author     :caiwende
create date:2009-08-03
*/

#include <pthread.h>
#include "hi_stb_api.h"
#include "software.h"
#include "fyf_def.h"
#include "os/fyf_queue.h"
#include "os/fyf_thread.h"
#define __FYF_TUNER_PORTING__
#ifdef  __FYF_TUNER_PORTING__
#include "dev/fyf_tuner.h"

static FYF_TUNER_NOTIFY_FUNC FYF_tuner_notify_fun;
static BU32 pTunerQueue;
#define FYF_TUNER_TIME_OUT_DEFAULT (5000)
static BU32 fyf_tuner_time_out = FYF_TUNER_TIME_OUT_DEFAULT;
//add by cwd 2009-12-1
//进行tuner的保护
static pthread_mutex_t tuner_mutex;
static BU32 g_cur_freq = 0; 
static BU08 g_tuner_dev_addr = 0;
/******************************************************************************/
/*Description: set notify func                                                */
/*Input      : func														      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_CPI_tuner_set_notify_porting(FYF_TUNER_NOTIFY_FUNC func)
{
	FYF_tuner_notify_fun = func;
	return FYF_OK;
}
/******************************************************************************/
/*Description: tuner lock delivery                                            */
/*Input      : tunerid,frequency,symbol_rate,modulation,request_id  	      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/


BS32 FYF_API_tuner_set_dev_addr(BU08 addr)
{
	g_tuner_dev_addr = addr;
}
BS32 FYF_CPI_tuner_lock_delivery_porting(BS32 tunerid, BS32 frequency, BS32 symbol_rate, BS32 modulation, BS32 request_id)
{

	BS32 ret;
	HI_UNF_TUNER_CONNECT_PARA_S struTunerSet;
	pthread_mutex_lock(&tuner_mutex);
	struTunerSet.unConnectPara.stCab.u32Freq = frequency;
	struTunerSet.unConnectPara.stCab.u32SymbolRate = symbol_rate;
	struTunerSet.unConnectPara.stCab.enModType = modulation + 0x100;
	
	struTunerSet.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
	struTunerSet.unConnectPara.stCab.bReverse = HI_FALSE;


	struTunerSet.unConnectPara.stCab.u32SymbolRate = symbol_rate * 1000;//6875000;
	printf("FYF_CPI_tuner_lock_delivery_porting freq = %d, symbol_rate = %d, modulat = %d\n", struTunerSet.unConnectPara.stCab.u32Freq, struTunerSet.unConnectPara.stCab.u32SymbolRate, 
		struTunerSet.unConnectPara.stCab.enModType);
	g_cur_freq = frequency;
	ret =  HI_UNF_TUNER_Connect(tunerid, &struTunerSet, 0);
#if 0
	if (ret != 0)
	{	
		printf("\n +++ tuner connect error 0x%x +++ \n", ret);
#ifdef _NEWLAND_DEMO_			
		HI_KEYLED_Set_LockLed(HI_FALSE);
#endif
	}
	else
	{
#ifdef _NEWLAND_DEMO_						
		HI_KEYLED_Set_LockLed(HI_TRUE);
#endif
	}
#endif
	pthread_mutex_unlock(&tuner_mutex);
	
	return ret;

}

/******************************************************************************/
/*Description: get signal quality                                             */
/*Input      : tunerid												  	      */
/*Output     : no                                                             */
/*Return     : >0 success, -1 failure                                         */
/******************************************************************************/
BS32 FYF_CPI_tuner_get_signal_quality_porting(BS32 tunerid)
{
	BU32 snr;
	if (HI_SUCCESS == HI_UNF_TUNER_GetSNR(tunerid, &snr))
	{
		//printf("snr = %d\n", snr);
		//snr = snr*8/9;
		if(snr>39)
		{
			snr=39;
		}
	}
	else
	{
		return 0;
	}
	return (BS32)snr;
}
/******************************************************************************/
/*Description: get signal strength                                            */
/*Input      : tunerid												  	      */
/*Output     : no                                                             */
/*Return     : >0 success, -1 failure                                         */
/******************************************************************************/
BS32 FYF_CPI_tuner_get_signal_strength_porting(BS32 tunerid)
{
	BU32 agc;

	if (HI_SUCCESS == HI_UNF_TUNER_GetSignalStrength(tunerid, &agc))
	{
		if (g_tuner_dev_addr == 0)    //汤姆逊70707
		{
			if (g_cur_freq >= 379000 || (g_cur_freq <= 227000 && g_cur_freq >= 171000))
			{
				if (agc <= 602)
				{
					agc = 60;
				}
				else if (agc <= 623 && agc >= 602)
				{
					agc = (623 - agc) / 3  + 53;
				}
				else if (agc <= 683 && agc >= 623)
				{
					agc = (683 - agc) / 5 + 41;
				}
				else if (agc <= 711 && agc >= 683)
				{
					agc = (711 - agc) / 7 +37;
				}
				else if (agc <= 743 && agc >= 711)
				{
					agc = (743 - agc) / 8 + 33;
				}
				else if (agc <= 770 && agc >= 743)
				{
					agc = (770 - agc) / 9 + 30;
				}
				else
				{
					agc = 30;   
				}
					
			}
			else if ((g_cur_freq <= 379000 && g_cur_freq >= 227000) || (g_cur_freq <= 139000))
			{
				if (agc <= 590)
				{
					agc = 60;
				}
				else if (agc <= 611 && agc >= 590)
				{
					agc = (611 - agc) / 3  + 53;
				}
				else if (agc <= 671 && agc >= 611)
				{
					agc = (671 - agc) / 5 + 41;
				}
				else if (agc <= 699 && agc >= 671)
				{
					agc = (699 - agc) / 7 +37;
				}
				else if (agc <= 731 && agc >= 699)
				{
					agc = (731 - agc) / 8 + 33;
				}
				else if (agc <= 758 && agc >= 731)
				{
					agc = (758 - agc) / 9 + 30;
				}
				else
				{
					agc = 30;
				}
			}
			else if (g_cur_freq <= 171000 && g_cur_freq >= 139000)
			{
				if (agc <= 622)
				{
					agc = 60;
				}
				else if (agc <= 650 && agc >= 622)
				{
					agc = (650 - agc) / 4  + 53;
				}
				else if (agc <= 710 && agc >= 650)
				{
					agc = (710 - agc) / 5 + 41;
				}
				else if (agc <= 738 && agc >= 710)
				{
					agc = (730 - agc) / 7 +37;
				}
				else if (agc <= 770 && agc >= 738)
				{
					agc = (770 - agc) / 8 + 33;
				}
				else if (agc <= 800 && agc >= 770)
				{
					agc = (800 - agc) / 10 + 30;
				}
				else
				{
					agc = 30;
				}
			}

			/* 将dbm转成dbuv */
			printf("agc = %d\n", agc);
			//agc += 30;
			if(agc>102)
			{
				agc=102;
			}
		}
		else                                  //TCL
		{
			if (g_cur_freq >= 379000)
			{
				if (agc <= 514)
				{
					agc = 60;
				}
				else if (agc <= 522 && agc >= 514)
				{
					agc = (522 - agc) / 4  + 58;
				}
				else if (agc <= 658 && agc >= 522)
				{
					agc = (658 - agc) / 8 +41;
				}
				else if (agc <= 730 && agc >= 658)
				{
					agc = (730 - agc) / 9 + 33;
				}
				else if (agc <= 760 && agc >= 730)
				{
					agc = (760 - agc) / 10 + 30;
				}
				else
				{
					agc = 30;   
				}
					
			}
			else if ((g_cur_freq <= 379000 && g_cur_freq >= 203000))
			{
				if (agc <= 514)
				{
					agc = 60;
				}
				else if (agc <= 516 && agc >= 514)
				{
					agc = (516 - agc) / 3  + 55;
				}
				else if (agc <= 628 && agc >= 516)
				{
					agc = (628 - agc) / 8 + 41;
				}
				else if (agc <= 700 && agc >= 628)
				{
					agc = (700 - agc) / 9 + 33;
				}
				else if (agc <= 730 && agc >= 700)
				{
					agc = (730 - agc) / 10 + 30;
				}
				else
				{
					agc = 30;
				}
			}
			else if (g_cur_freq <= 203000 && g_cur_freq >= 163000 || g_cur_freq <= 115000)
			{
				if (agc <= 514)
				{
					agc = 60;
				}
				else if (agc <= 517 && agc >= 514)
				{
					agc = (517 - agc) / 2  + 57;
				}
				else if (agc <= 645 && agc >= 517)
				{
					agc = (645 - agc) / 8 + 41;
				}
				else if (agc <= 717 && agc >= 645)
				{
					agc = (717 - agc) / 9 + 33;
				}
				else if (agc <= 744 && agc >= 717)
				{
					agc = (747 - agc) / 10 + 30;
				}
				else
				{
					agc = 30;
				}
			}
			else
			{
				if (agc <= 519)
				{
					agc = 60;
				}
				else if (agc <= 540 && agc >= 519)
				{
					agc = (540 - agc) / 7  + 57;
				}
				else if (agc <= 668 && agc >= 540)
				{
					agc = (668 - agc) / 8 + 41;
				}
				else if (agc <= 740 && agc >= 668)
				{
					agc = (740 - agc) / 9 + 33;
				}
				else if (agc <= 770 && agc >= 740)
				{
					agc = (770 - agc) / 10 + 30;
				}
				else
				{
					agc = 30;
				}
			}
		}

	}
	else
	{
		return 0;
	}
//	printf("agc = %d\n", agc);
	return (BS32)agc;
}
/******************************************************************************/
/*Description: get signal ber                                                 */
/*Input      : tunerid												  	      */
/*Output     : no                                                             */
/*Return     : >0 success, -1 failure                                         */
/******************************************************************************/
BS32 FYF_CPI_tuner_get_signal_ber_porting(BS32 tunerid)
{
	HI_S32 ret;
	BU32 BerBuf[3];
	BU32 Ber;
	ret = HI_UNF_TUNER_GetBER(tunerid, BerBuf);

	//printf("Berbuf 0x%x 0x%x 0x%x\n", BerBuf[0], BerBuf[1], BerBuf[2]);
	BerBuf[1] /= 10;

	if (ret == 0)
	{
		Ber = (BerBuf[0] << 24) | (BerBuf[1] << 16) | BerBuf[2];
	}
	else
	{
		Ber = 0;
		printf("%s error\n", __FUNCTION__);
	}
	
	return (BS32)Ber;
}

/******************************************************************************/
/*Description: get status                                                     */
/*Input      : tunerid												  	      */
/*Output     : no                                                             */
/*Return     : FYF_TUNER_LOST：no locked,FYF_TUNER_LOCKED：locked            */
/*			   FYF_ERR: failure                                               */
/******************************************************************************/
BS32 FYF_CPI_tuner_get_status_porting(BS32 tunerid)
{
	BS32 ret;
	HI_UNF_TUNER_STATUS_S TunerStatus;
	pthread_mutex_lock(&tuner_mutex);
	ret = HI_UNF_TUNER_GetStatus(tunerid, &TunerStatus);
	pthread_mutex_unlock(&tuner_mutex);
	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_TUNER_GetStatus error\n", ret);
#ifdef _NEWLAND_DEMO_				
		HI_KEYLED_Set_LockLed(HI_FALSE);
#endif
		return FYF_TUNER_LOST;
		
	}

	if (TunerStatus.enLockStatus)
	{
#ifdef _NEWLAND_DEMO_			
		HI_KEYLED_Set_LockLed(HI_TRUE);
	//	printf("TunerStatus.enLockStatus = lock!!!\n");
#endif
		return FYF_TUNER_LOCKED;
	}
	else
	{
#ifdef _NEWLAND_DEMO_				
		HI_KEYLED_Set_LockLed(HI_FALSE);
#endif
		return FYF_TUNER_LOST;
	}
}

/******************************************************************************/
/*Description: ioctl                                                          */
/*Input      : tunerid,op       									  	      */
/*Output     : arg                                                            */
/*Return     : FYF_OK：success；FYF_ERR：fail								  */
/******************************************************************************/
BS32 FYF_CPI_tuner_ioctl_porting(BS32 tunerid, FYF_TUNER_IOCTL_e op, void *arg)
{
	BS32 ret = FYF_OK;
	BU32 * value = (BU32 *)arg;
	pthread_mutex_lock(&tuner_mutex);
	switch(op)
	{
	case FYF_TUNER_GET_QUALITY:
		*value = FYF_CPI_tuner_get_signal_quality_porting(tunerid);
		break;
	case FYF_TUNER_GET_STRENGTH:
		*value = FYF_CPI_tuner_get_signal_strength_porting(tunerid);
		break;
	case FYF_TUNER_GET_BER:
		*value = FYF_CPI_tuner_get_signal_ber_porting(tunerid);
		break;
	case FYF_TUNER_GET_LEVEL:
		*value = FYF_CPI_tuner_get_signal_strength_porting(tunerid);
		break;
	case FYF_TUNER_GET_SNR:
		*value = FYF_CPI_tuner_get_signal_quality_porting(tunerid);
		break;
	case FYF_TUNER_TIME_OUT:
		fyf_tuner_time_out = *value;
		break;
	default:
		ret = FYF_ERR;
		break;
	}
	pthread_mutex_unlock(&tuner_mutex);
	return ret;
}

/******************************************************************************/
/*Description: no                                                             */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
static FYF_TUNER_PORTING FYF_TUNER_porting=
{
	FYF_CPI_tuner_set_notify_porting,
	FYF_CPI_tuner_lock_delivery_porting,
	FYF_CPI_tuner_get_signal_quality_porting,
	FYF_CPI_tuner_get_signal_strength_porting,
	FYF_CPI_tuner_get_signal_ber_porting,
	FYF_CPI_tuner_get_status_porting,
	FYF_CPI_tuner_ioctl_porting
};
/******************************************************************************/
/*Description: tuner module init											  */
/*Input 	 : no         		    										  */
/*Output	 : no															  */
/*Return	 : no                   										  */
/******************************************************************************/
void FYF_API_tuner_init(void)
{
	pthread_mutex_init(&tuner_mutex, NULL);
	FYF_API_tuner_register(&FYF_TUNER_porting);
}
#endif

