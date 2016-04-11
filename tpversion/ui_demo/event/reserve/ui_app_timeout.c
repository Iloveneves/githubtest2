#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define TIMEOUT_TIME  59
extern BU08 bfirst;
extern BU08 bGuiTimerSuspend;
extern BU08 g_bRefreshOsd;
BU08 dlg_timeout_state = 0;

static Epg_Info_S onTimeEpgInfo;
static BU08 g_reserve_countdown = 0;
static BU08 g_reserve_countdownStr[3];
static BU08 g_timeout_str[2][40];


void UI_APP_TimeOut_Callback(BU32 callback_type, Epg_Info_S *EpgInfo)
{
	BU32 curState,preState;

	if (callback_type == 0)
	{
	
		memcpy(&onTimeEpgInfo, EpgInfo, sizeof(Epg_Info_S));
		//Epg预约
		if(onTimeEpgInfo.reserveState == 1)
		{
			APP_EPG_API_CancelReserve(0);
			sprintf(g_timeout_str[0],"你预约的: %s",onTimeEpgInfo.chnName);
			sprintf(g_timeout_str[1],"节目: %s",onTimeEpgInfo.epgName);
			g_timeout_str[0][39]=0;
			g_timeout_str[1][39]=0;
			GUI_ENC_API_SetValue(ID_STA_Line1_TimeOut, WDT_STA_SV_STRING, (BU32) g_timeout_str[0], 0);
			GUI_ENC_API_SetValue(ID_STA_Line2_TimeOut, WDT_STA_SV_STRING, (BU32) g_timeout_str[1], 0);
		}
#ifdef ENABLE_NVOD
		else if(onTimeEpgInfo.reserveState == 4)
		{
			APP_EPG_API_CancelReserveType(0,APP_ReserveType_Nvod_e);
			sprintf(g_timeout_str[0],"你预约的: %s",onTimeEpgInfo.chnName);
			sprintf(g_timeout_str[1],"节目: %s",onTimeEpgInfo.epgName);
			g_timeout_str[0][39]=0;
			g_timeout_str[1][39]=0;
			GUI_ENC_API_SetValue(ID_STA_Line1_TimeOut, WDT_STA_SV_STRING, (BU32) g_timeout_str[0], 0);
			GUI_ENC_API_SetValue(ID_STA_Line2_TimeOut, WDT_STA_SV_STRING, (BU32) g_timeout_str[1], 0);
		}
#endif
#ifdef ENABLE_PVR
		//Epg预录/选时预录
		else
		{
			//Epg预录
			if(onTimeEpgInfo.reserveState == 2)
			{
				APP_EPG_API_CancelReserveType(0,APP_ReserveType_EpgRec_e);
			}
			//选时预录
			else if(onTimeEpgInfo.reserveState == 3)
			{
				APP_EPG_API_CancelReserveType(0,APP_ReserveType_ManualRec_e);
			}
			sprintf(g_timeout_str[0],"你预录的: %s",onTimeEpgInfo.chnName);
			sprintf(g_timeout_str[1],"节目: %s",onTimeEpgInfo.epgName);
			g_timeout_str[0][39]=0;
			g_timeout_str[1][39]=0;	
			GUI_ENC_API_SetValue(ID_STA_Line1_TimeOut, WDT_STA_SV_STRING, (BU32) g_timeout_str[0], 0);
			GUI_ENC_API_SetValue(ID_STA_Line2_TimeOut, WDT_STA_SV_STRING, (BU32) g_timeout_str[1], 0);
			if(APP_PVR_API_GetRecStatus(1) == 1)
			{
				GUI_ENC_API_SetValue(ID_STA_Line3_TimeOut, WDT_STA_SV_STRING, (BU32) "秒后您即将录制节目，当前正在录制其他节目", 0);
				GUI_ENC_API_SetValue(ID_STA_Line4_TimeOut, WDT_STA_SV_STRING, (BU32) "确认键: 取消录制其他节目,立即录制当前节目", 0);
				GUI_ENC_API_SetValue(ID_STA_Line5_TimeOut, WDT_STA_SV_STRING, (BU32) "退出键: 继续录制其他节目,取消录制当前节目", 0);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Line3_TimeOut, WDT_STA_SV_STRING, (BU32) "秒后您即将录制预约节目", 0);
				GUI_ENC_API_SetValue(ID_STA_Line4_TimeOut, WDT_STA_SV_STRING, (BU32) "确认键: 立即录制预约节目", 0);
				GUI_ENC_API_SetValue(ID_STA_Line5_TimeOut, WDT_STA_SV_STRING, (BU32) "退出键: 取消录制预约节目", 0);
			}
		}
#endif
		curState = GUI_ENC_API_GetCurState();
		preState = GUI_ENC_API_GetPrevState();
		switch(curState)
		{
		case ID_DLG_ConflictInfo:
		case ID_DLG_EpgReserveInfo:
		//case ID_DLG_Detail:
			if(ID_DLG_EPG == preState)
			{
				GUI_ENC_API_SwitchState(ID_DLG_EPG, 1);
			}
			break;
		default:
			break;
		}
		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_RESERVEPROMPT, 0, 0);		
	}
#ifdef ENABLE_PVR
	else if (callback_type == 1)
	{
		UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_REC_STOP);
		GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"预约录制结束",0);
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
	}
#endif
	
}

void UI_APP_TimeOut_Enter(void)
{
	GUI_ENC_API_SetValue(ID_TIM_TimeOut, WDT_TIM_SV_PLAY,0,0);
	g_reserve_countdown = 59+1;
	dlg_timeout_state = 1;
}
void UI_APP_TimeOut_End(void)
{
	GUI_ENC_API_SetValue(ID_TIM_TimeOut, WDT_TIM_SV_STOP,0,0);
	APP_Com_API_SetFullScreen();
	dlg_timeout_state = 0;
}
void UI_APP_TimeOut_OnPaint(BU32 hdc,BU32 lpara)
{
	if(lpara == 1)
	{
		sprintf(g_reserve_countdownStr,"60");
		GUI_API_TextOut(hdc, 0,0, g_reserve_countdownStr, 2, 0, COLOR_INFOBK, 0);	
	}
	else if(lpara == 2)
	{
		sprintf(g_reserve_countdownStr,"%2d",g_reserve_countdown);
		GUI_API_TextOut(hdc, 0,0, g_reserve_countdownStr, 2, 0, COLOR_INFOBK, 0);
	}
}

void UI_APP_TimeOut_Key(BU32 key)
{
	BU32 preState = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 cur_time;
	FYF_time_shifted_service_s shiftServiceInfo;
	if(g_reserve_countdown > TIMEOUT_TIME)	return;
	if(key == GUI_VK_EXIT)
	{
		if(GUI_ENC_API_GetPrevState() == ID_DLG_ChnLock)
		{
			GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevPrevState(), 1);
			GUI_ENC_API_SwitchState (ID_DLG_ChnLock, 1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		}
		if(GUI_ENC_API_GetPrevState() == ID_DLG_MenuLock)
		{
			GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevPrevState(), 1);
			GUI_ENC_API_SwitchState(ID_DLG_MenuLock, 1);
		}
		else if(GUI_ENC_API_GetPrevState () == ID_DLG_Unlock)
		{
			GUI_ENC_API_SwitchState (UI_APP_API_GetLockCurState(), 1);
			GUI_ENC_API_SwitchState (ID_DLG_Unlock, 1);
		}
#ifdef ENALE_BROADCAST_IPANEL
		else if (g_bRefreshOsd)//数据广播中出现预约的处理
		{
			bfirst = 0;
			bGuiTimerSuspend = FYF_TRUE;
			dlg_timeout_state = 0;
			FYF_API_gdi_draw_image(0, 0, 640, 526, 0, 0);
			GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
		}
#endif
		else
		{
			GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);
		}
		APP_EPG_API_UpDataToFlash();
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_RESERVEPROMPT_END, 0, 0);
	}
	else if(key == GUI_VK_SELECT)
	{
		if(onTimeEpgInfo.reserveState == 1)
		{
#ifdef ENABLE_PVR
			if(APP_PVR_API_GetRecStatus(1) == APP_REC_STATUS_PLAY_e)
			{
			//提示已停止录制
				APP_PRINT_INFO("stop rec\n");
	 			APP_PVR_API_StopRec(1);
			}
#endif
			preState = GUI_ENC_API_GetPrevState();
#ifdef ENALE_BROADCAST_IPANEL
			if (preState == ID_DLG_Null)
			{
				HI_TRD_IPANEL_Exit();
			}
#endif
			APP_ChnDatabase_API_SetCurChnType(onTimeEpgInfo.style, APP_CHN_GROUP_TYPE_ALL_e);
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),onTimeEpgInfo.serviceId, &chnInfo);
			APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_PLAY_RESERVE);
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_PlayReserve,0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
			APP_EPG_API_UpDataToFlash();
		
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		}
#ifdef ENABLE_PVR
		else if(onTimeEpgInfo.reserveState == 2 || onTimeEpgInfo.reserveState == 3)
		{
			if(APP_PVR_API_GetRecStatus(1) == APP_REC_STATUS_PLAY_e)
			{
			//提示已停止录制
				APP_PRINT_INFO("stop rec\n");
	 			APP_PVR_API_StopRec(1);
				APP_API_SetPVREndTime(onTimeEpgInfo.day, onTimeEpgInfo.endHour, onTimeEpgInfo.endMin);
				/*pvr_endday = onTimeEpgInfo.day;
				pvr_endhour = onTimeEpgInfo.endHour;
				pvr_endmin = onTimeEpgInfo.endMin;
				*/
			}
			preState = GUI_ENC_API_GetPrevState();
			APP_ChnDatabase_API_SetCurChnType(onTimeEpgInfo.style, APP_CHN_GROUP_TYPE_ALL_e);
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),onTimeEpgInfo.serviceId, &chnInfo);
			APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
			APP_EPG_API_UpDataToFlash();
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_PLAY_RESERVE);
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"播放并录制预录节目",0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			APP_PRINT_INFO("start rec\n");
			if (APP_SUCCESS  == APP_PVR_API_StartRec(APP_ChnDatabase_API_GetCurChnNO(), 1))
			{
				cur_time = APP_IPI_GetTimeWithSec();
				APP_PVR_API_SetRecTime(cur_time, onTimeEpgInfo.endHour*60 + onTimeEpgInfo.endMin);
			}

		}
#endif
	//Nvod预约
#ifdef ENABLE_NVOD
		else if(onTimeEpgInfo.reserveState == 4)
		{
			APP_EPG_API_UpDataToFlash();
			//FYF_API_time_get_pid((BU16)(onTimeEpgInfo.serviceId),&shiftServiceInfo);
			//APP_IPI_PlayProg(shiftServiceInfo.vpid, shiftServiceInfo.apid,shiftServiceInfo.pcrpid,FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
			//APP_Com_API_PlayNvodChn(onTimeEpgInfo.serviceId);
			UI_APP_API_PlayByServiceID(onTimeEpgInfo.serviceId, "nuod");
			GUI_ENC_API_SwitchState(ID_DLG_NvodPlay, 1);
		}
#endif

	}
}
void UI_APP_TimeOut_Timer(void)
{
	BU32 preState = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 cur_time;
	FYF_time_shifted_service_s shiftServiceInfo;
	g_reserve_countdown --;
 	if(g_reserve_countdown == 19)
	{
		if(onTimeEpgInfo.reserveState == 1)
		{
#ifdef ENABLE_PVR
			if(APP_PVR_API_GetRecStatus(1) == APP_REC_STATUS_PLAY_e)
			{
				APP_PRINT_INFO("stop rec\n");
	 			APP_PVR_API_StopRec(1);
			}
#endif
			preState = GUI_ENC_API_GetPrevState();
#ifdef ENALE_BROADCAST_IPANEL
			if (preState == ID_DLG_Null)
			{
				HI_TRD_IPANEL_Exit();
			}
#endif
			APP_ChnDatabase_API_SetCurChnType(onTimeEpgInfo.style, APP_CHN_GROUP_TYPE_ALL_e);
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),onTimeEpgInfo.serviceId, &chnInfo);
			APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_PLAY_RESERVE);
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_PlayReserve,0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
			APP_EPG_API_UpDataToFlash();
			//
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		}
#ifdef ENABLE_PVR
		else if(onTimeEpgInfo.reserveState == 2 || onTimeEpgInfo.reserveState == 3)
		{
			if(APP_PVR_API_GetRecStatus(1) == APP_REC_STATUS_PLAY_e)
			{
				APP_PRINT_INFO("stop rec\n");
	 			APP_PVR_API_StopRec(1);
				APP_API_SetPVREndTime(onTimeEpgInfo.day, onTimeEpgInfo.endHour, onTimeEpgInfo.endMin);
			/*	pvr_endday = onTimeEpgInfo.day;
				pvr_endhour = onTimeEpgInfo.endHour;
				pvr_endmin = onTimeEpgInfo.endMin;
				*/
			}
			preState = GUI_ENC_API_GetPrevState();
			APP_ChnDatabase_API_SetCurChnType(onTimeEpgInfo.style, APP_CHN_GROUP_TYPE_ALL_e);
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),onTimeEpgInfo.serviceId, &chnInfo);
			APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
			APP_EPG_API_UpDataToFlash();
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_PLAY_RESERVE);
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"播放并录制预录节目",0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
		//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			if (APP_SUCCESS  == APP_PVR_API_StartRec(APP_ChnDatabase_API_GetCurChnNO(), 1))
			{
				cur_time = APP_IPI_GetTimeWithSec();
				APP_PVR_API_SetRecTime(cur_time, (onTimeEpgInfo.endHour*60 + onTimeEpgInfo.endMin) * 60);
				APP_PRINT_INFO("start rec\n");
			}
		}
#endif
#ifdef ENABLE_NVOD
		else if(onTimeEpgInfo.reserveState == 4)
		{
			APP_EPG_API_UpDataToFlash();
			//FYF_API_time_get_pid((BU16)(onTimeEpgInfo.serviceId),&shiftServiceInfo);
			//APP_IPI_PlayProg(shiftServiceInfo.vpid, shiftServiceInfo.apid,shiftServiceInfo.pcrpid,FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
			//APP_Com_API_PlayNvodChn(onTimeEpgInfo.serviceId);
			UI_APP_API_PlayByServiceID(onTimeEpgInfo.serviceId, "nuod");
			GUI_ENC_API_SwitchState(ID_DLG_NvodPlay, 1);
		}
#endif
	}
	else
	{
		GUI_ENC_API_Update(ID_ODC_TimeOut, 2);
	}
}

