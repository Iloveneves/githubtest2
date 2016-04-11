  #include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

/*-------------------------static global variables---------------------*/
static BU08 g_bAreaLockok = false; /*记录CA模块是否调用了DVTSTBCA_AreaLock_OK()接口*/
/*----------------------------------------------------------------*/
extern BU08 arealock_falg;
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
	BU32 req_first =0;
	arealock_falg = 1;

	FYF_API_printf("----------UI_APP_AreaLock_SwitchChannel--Enter-------------\n");
	/*
	while(!IsMainTaskEnd ())
	{
		FYF_API_thread_sleep(100);
	}
	*/
    APP_EPG_API_SetRemindReserve(0);
  	APP_Com_API_Stop_Signal_Dectect ();	
	GUI_API_SetKeyReceiveMode (KEY_RECEIVE_CLOSE);	
	
	APP_SYS_API_GetSysInfo (APP_SYS_AREA_LOCK_e, &AreaLock_Frq);
	if((AreaLock_Frq<51) ||( AreaLock_Frq>865))
	{FYF_API_printf("----------UI_APP_AreaLock_SwitchChannel= 0---------------\n");
		FYF_API_ca_set_value (FYF_CA_SV_SWITCH_CHANNEL_OVER, 2, 0);	
		APP_EPG_API_SetRemindReserve(1);
		APP_Com_API_Start_Signal_Dectect ();	
		GUI_API_SetKeyReceiveMode (KEY_RECEIVE_OPEN);
		arealock_falg = 0;
		return 10;
	}
	else
	{
		curDlgID = GUI_ENC_API_GetCurState();

#if 1
		while((curDlgID == ID_DLG_Search) || (curDlgID == ID_DLG_ManualSearchSet)
			  || (curDlgID == ID_DLG_ReserveList) || (curDlgID == ID_DLG_TimeOut))
		{
			FYF_API_thread_sleep(500);
			curDlgID = GUI_ENC_API_GetCurState();
		}
#endif



		/*--提示用户正在获取区域锁定信息--*/   
		if(curDlgID == ID_DLG_ChnInfo)
		{
			FullScreen_IPI_SetCurState(NULL_STATE);
		}

	
		APP_Com_API_StopProg ( FALSE );		
	

		//GUI_ENC_API_SwitchState(ID_DLG_AreaLockInfo, 1);
	
		FYF_API_thread_sleep(100);
		//FYF_API_OSD_FillRect(255, 275, 250, 24, COLOR_WHITE);
		bitmap = 3;
		APP_Com_API_ShowBmp(bitmap);//把logo的显示入栈操作
		GUI_API_TextOut(GUI_API_GetDesktopWindow(), 200, 50, "正在获取区域锁定信息", 20, COLOR_BLACK, COLOR_INFOBK, 0);	
		FYF_API_OSD_Refresh(0, 0, 720, 576);
		
		/*--切换到区域锁定的频点--*/
		APP_Signal_API_TuneToNewParam(AreaLock_Frq, 6875, APP_CHN_QAM_64_e);
		time_start = FYF_API_time_ms();
		ret = 10;
		while ((FYF_API_time_ms() - time_start) <= 1000 * wWaitSeconds)
		{
		    if(((FYF_API_time_ms() - time_start) >= 1000 * 4) && APP_SIGNAL_LOCK != APP_Signal_API_GetSignalState ())
		    {
		    	ret = 20;
				break;
		    }
			if(APP_SIGNAL_LOCK == APP_Signal_API_GetSignalState ())
			{
				if(req_first == 0)
				{		
					req_first = 1;
					APP_Cat_API_GetCatSetEmm(); //滤CAT表获得EMM并赋给CA
				}
				if(g_bAreaLockok)
				{
					g_bAreaLockok = false;
					ret = 0;
					break;
				}

			}		
			FYF_API_thread_sleep(100);
		}
		if(ret == 10)
		{
			FYF_API_ca_set_value (FYF_CA_SV_SWITCH_CHANNEL_OVER, 1, 0);
		}

		if(ret == 20)
		{
			FYF_API_ca_set_value (FYF_CA_SV_SWITCH_CHANNEL_OVER, 2, 0);
		}

		//bitmap = 0;
		//APP_Com_API_ShowBmp(bitmap);//把logo的显示入栈操作
		GUI_API_FillRect(GUI_API_GetDesktopWindow(), 200, 50, 250, 24, COLOR_TRANS);	
		FYF_API_OSD_Refresh(0, 0, 720, 576);
		
		if (curDlgID == ID_DLG_AutoInfo)
		{
			if(IsMainTaskEnd ())
			GUI_ENC_API_SwitchState(ID_DLG_AutoInfo,1);
		}
		else
		{
			if(IsMainTaskEnd ())
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
		}
		APP_EPG_API_SetRemindReserve(1);
		APP_Com_API_Start_Signal_Dectect ();	
		GUI_API_SetKeyReceiveMode (KEY_RECEIVE_OPEN);
		arealock_falg = 0;
		return ret;		

	}

}

void UI_APP_AreaLock_ok(void)
{
	if(g_bAreaLockok == false)
	{
		g_bAreaLockok = true;
		FYF_API_ca_set_value (FYF_CA_SV_SWITCH_CHANNEL_OVER, 0, 0);
	}
}

/*单频点*/
void UI_APP_AreaLock_Enter(void)
{
	BU32 freq = 0;
	APP_SYS_API_GetSysInfo (APP_SYS_AREA_LOCK_e, &freq); 
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
	APP_Com_API_SetFullScreen ();
}


