#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"


//#define AREA_LOCK_IF_TO_FIRST_CHN
//#define DEBUG_CA   
#ifdef  DEBUG_CA
#define DEBUG_MSG       printf
#else 
#define DEBUG_MSG(...)  do{}while(0)
#endif

BU32 __glb_AreaLock_SwitchChannel = 0;
BU32 __glb_AreaLock_Success = 0;
BU32 __glb_AreaLock_TimeoutOrFail = 0;
/*-------------------------static global variables---------------------*/
static BU08 __glb_AreaLock_ok = 1; /*记录CA模块是否调用了DVTSTBCA_AreaLock_OK()接口*/
static BU32 __glb_AreaLock_prestate = 0;
static BU32 __glb_AreaLock_RrestTimeMs = 0;
static BU08 __glb_AreaLock_str_En[60];
static BU08 __glb_AreaLock_str_Cn[60];

//struct nim_device * __glb_arealock_nim_dev;
/*-----------------------Static Function---------------------------*/
static BS32 __AreaLock_SwitchToAreaLockChannel(BU16 wWaitSeconds)
{
	BU32 time_start, time_end;
	BU32 AreaLock_freq;
	BU08 is_lock;
	BU32 i = 10;
	BU32 waitTimeMs = (BU32)(1000*wWaitSeconds);
	
	time_start = FYF_API_time_ms();
	APP_SYS_API_GetSysInfo (APP_SYS_AREA_LOCK_e, &AreaLock_freq);
	APP_Signal_API_TuneToNewParam(AreaLock_freq, 6875, APP_CHN_QAM_64_e);
	FYF_API_thread_sleep(1000);

	while (i)
	{		
		is_lock = APP_Signal_API_GetSignalState ();
		if(APP_SIGNAL_LOCK == is_lock) 
		{
			time_end = FYF_API_time_ms();
			if((time_end - time_start) < waitTimeMs)
			{
				__glb_AreaLock_RrestTimeMs = waitTimeMs - (time_end - time_start);
				return 0;
			}
			else
			{
				__glb_AreaLock_RrestTimeMs = 0;
				return 2;
			}			
		}
		FYF_API_thread_sleep(10);
		i--;
	}
	return 1;
}

/*----------------------------------------------------------------*/
static BU16 __Is_AreaLock_OK(void)
{
	if(__glb_AreaLock_ok)
	{
//		__glb_AreaLock_ok = false;
		return 1;
	}
	else
	{
		return 0;
	}
}
#ifdef AREA_LOCK_IF_TO_FIRST_CHN
static BU32 __AreaLock_GetChnByFreq(BU32 freq)
{
	BU32 chnNo,maxChn;
	ChnDataBase_ChnInfo_S chnInfo;
	APP_ChnDatabase_API_CreatChnList(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
	maxChn = APP_ChnDatabase_API_GetCurTypeChnNum();

	for(chnNo=0; chnNo<maxChn-1; chnNo++)
	{
		APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),chnNo, &chnInfo);
		chnInfo.m_TsId 
		if(freq == chnInfo.m_TsId)
		{
			return chnNo;
		}
	}

	return 0xffff;
	
}
static void __AreaLock_ToGivenFreqFirstChn()
{
	BU32 areaLockFreq;
	BU32 chnNo = 0xffff;
	
	APP_SYS_API_GetSysInfo(APP_SYS_AREA_LOCK_e, &areaLockFreq);
	chnNo = __AreaLock_GetChnByFreq(areaLockFreq);
	if(chnNo == 0xffff)
	{
		return;
	}
	else
	{
		//UI_APP_API_CheckPlay_PlayChn_led(chnNo,UI_APP_CurChnbPlay());
	}
}
#endif

BS32 UI_APP_AreaLock_SwitchChannel(BU16 wWaitSeconds)
{
	BU32 bitmap;
	BU32 CurChnNo;
	BU32 time_start = 0;
	BU32 time_end = 0;
	BU32 AreaLock_Frq;
	BU32 EMMPID;
	BS32 ret;
	BU32 curDlgID;
#ifdef AREA_LOCK_IF_TO_FIRST_CHN
	BU08 groupIndex;
#endif
    APP_EPG_API_SetRemindReserve(0);//2009-04-03
    APP_Com_API_Stop_Signal_Dectect ();
//	FYF_API_semaphore_wait (FYF_API_GetPowerOnSemaphore (), FYF_WAIT_FOREVER);

	FYF_API_printf("----------UI_APP_AreaLock_SwitchChannel---------------\n");
	while(!IsMainTaskEnd ())
	{
		FYF_API_thread_sleep(100);
	}
	CurChnNo = APP_ChnDatabase_API_GetCurChnNO();
	APP_SYS_API_GetSysInfo (APP_SYS_AREA_LOCK_e, &AreaLock_Frq);
	__glb_AreaLock_SwitchChannel++;
	if(AreaLock_Frq == 0)
	{
		FYF_API_ca_set_value (FYF_CA_SV_SWITCH_CHANNEL_OVER, 2, 0);
		__glb_AreaLock_TimeoutOrFail++;
		
//		FYF_API_semaphore_release (FYF_API_GetPowerOnSemaphore ());
		APP_EPG_API_SetRemindReserve(1);//2009-04-03
		APP_Com_API_Start_Signal_Dectect ();
		return 10;
	}
	else
	{
		curDlgID = GUI_ENC_API_GetCurState();
/*add by hzhb 2008.2.15*/
#if 1
		while((curDlgID == ID_DLG_Search) || (curDlgID == ID_DLG_ManualSearchSet)
			  || (curDlgID == ID_DLG_ReserveList) || (curDlgID == ID_DLG_TimeOut))
		{
			FYF_API_thread_sleep(500);
			curDlgID = GUI_ENC_API_GetCurState();
		}
#endif
/*end add*/
		
		/*--保存当前ServiceID--*/
#ifdef AREA_LOCK_IF_TO_FIRST_CHN		
		__AreaLock_ToGivenFreqFirstChn();
#endif
		/*--提示用户正在获取区域锁定信息--*/   
		if(curDlgID == ID_DLG_ChnInfo)
		{
			FullScreen_IPI_ClearScreen();
		}
		
		bitmap = 3;
		//FYF_API_vdec_ioctl(FYF_API_vdec_open (), FYF_VDEC_BITMAP, &bitmap);
		APP_Com_API_ShowBmp(bitmap);//把logo的显示入栈操作

		GUI_ENC_API_SwitchState(ID_DLG_AreaLockInfo, 1);
		
		FYF_API_thread_sleep(10);
		
		/*--切换到区域锁定的频点--*/
		/*add by hzhb*/
		APP_Com_API_StopProg ( FALSE );
		/*end add*/
		ret = __AreaLock_SwitchToAreaLockChannel(wWaitSeconds);
		
		if(ret == 1)
		{
			FYF_API_ca_set_value (FYF_CA_SV_SWITCH_CHANNEL_OVER, 2, 0);
			__glb_AreaLock_TimeoutOrFail++;

			GUI_ENC_API_SwitchState(curDlgID,1);
			if(APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				//UI_APP_API_CheckPlay_PlayChn_led(CurChnNo,UI_APP_CurChnbPlay());
				//UI_APP_API_CheckLock_PlayChn_led (CurChnNo, CurChnNo, 0);
			}
			bitmap = 0;
			//FYF_API_vdec_ioctl(FYF_API_vdec_open (), FYF_VDEC_BITMAP, &bitmap);
			APP_Com_API_ShowBmp(bitmap);//把logo的显示入栈操作
//			FYF_API_semaphore_release (FYF_API_GetPowerOnSemaphore ());
			APP_EPG_API_SetRemindReserve(1);//2009-04-03
			APP_Com_API_Start_Signal_Dectect ();
			return 10;
		}
		else if(ret == 2)//超时
		{
			FYF_API_ca_set_value (FYF_CA_SV_SWITCH_CHANNEL_OVER, 1, 0);
			__glb_AreaLock_TimeoutOrFail++;
			
			GUI_ENC_API_SwitchState(curDlgID,1);
			if(APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				//UI_APP_API_CheckPlay_PlayChn_led(CurChnNo,UI_APP_CurChnbPlay());
			//	UI_APP_API_CheckLock_PlayChn_led (CurChnNo, CurChnNo, 0);
			}
			bitmap = 0;
		    //FYF_API_vdec_ioctl(FYF_API_vdec_open (), FYF_VDEC_BITMAP, &bitmap);
		    APP_Com_API_ShowBmp(bitmap);//把logo的显示入栈操作
//			FYF_API_semaphore_release (FYF_API_GetPowerOnSemaphore ());
			APP_EPG_API_SetRemindReserve(1);//2009-04-03
			APP_Com_API_Start_Signal_Dectect ();
			return 10;
		}
		else if(ret == 0)
		{	
			time_start = FYF_API_time_ms();
			//滤CAT表获得EMM并赋给CA
			APP_Cat_API_GetCatSetEmm (); /* szhy:090302 */
			__glb_AreaLock_ok = false;
			time_end= FYF_API_time_ms();
			ret = 10;
		}
		while( (time_end - time_start) <= __glb_AreaLock_RrestTimeMs )
		{
			if(__Is_AreaLock_OK())
			{
				ret = 0;
				goto exit;
			}
			FYF_API_thread_sleep(100);
			time_end= FYF_API_time_ms();//add by hzhb
		}
		FYF_API_ca_set_value (FYF_CA_SV_SWITCH_CHANNEL_OVER, 1, 0);
		__glb_AreaLock_TimeoutOrFail++;
exit:

		bitmap = 0;
		//FYF_API_vdec_ioctl(FYF_API_vdec_open (), FYF_VDEC_BITMAP, &bitmap);
		APP_Com_API_ShowBmp(bitmap);//把logo的显示入栈操作

		GUI_ENC_API_SwitchState(curDlgID,1);
		if(APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			//UI_APP_API_CheckPlay_PlayChn_led(CurChnNo,UI_APP_CurChnbPlay());
		//	UI_APP_API_CheckLock_PlayChn_led (CurChnNo, CurChnNo, 0);
		}
//		FYF_API_semaphore_release (FYF_API_GetPowerOnSemaphore ());
		APP_EPG_API_SetRemindReserve(1);//2009-04-03
		APP_Com_API_Start_Signal_Dectect ();
		return ret;		

	}

}

void UI_APP_AreaLock_ok(void)
{
	if(__glb_AreaLock_ok == false)
	{
		__glb_AreaLock_ok = true;
		/* 区域锁定成功次数 */
		__glb_AreaLock_Success++;
	}
}

static BU08 __glb_arealock_freq[4] = {0};
/*单频点*/
void UI_APP_AreaLock_Enter(void)
{
	BU32 freq = 0;
	APP_SYS_API_GetSysInfo (APP_SYS_AREA_LOCK_e, &freq); /* szhy:090302 */ 
	GUI_ENC_API_SetValue(ID_TBX_SingleSlot, WDT_TBX_SV_DATA, (BU32)freq, 0);
	APP_Com_API_SetFullScreen();
	
}
void UI_APP_AreaLock_End(void)
{

}
void UI_APP_AreaLock_Key(BU32 key)
{
	BU32 AreaLock_freq;
	if((key == GUI_VK_SELECT))
	{
		GUI_ENC_API_GetValue(ID_TBX_SingleSlot, WDT_TBX_SV_DATA, (&AreaLock_freq), 0);
		APP_SYS_API_SetSysInfo(APP_SYS_AREA_LOCK_e, AreaLock_freq);
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
	}
	else if(GUI_VK_EXIT == key)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
}

void UI_APP_AreaLockInfo_Enter(void)
{
	//BU32 bitmap;
	//bitmap = 3;
	//FYF_API_vdec_ioctl(FYF_API_vdec_open (), FYF_VDEC_BITMAP, &bitmap);
	//APP_Com_API_ShowBmp(bitmap);//把logo的显示入栈操作
	APP_Com_API_SetFullScreen ();
}




