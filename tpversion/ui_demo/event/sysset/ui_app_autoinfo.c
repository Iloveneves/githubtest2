#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

//无节目提示自动搜索
void UI_APP_AutoInfo_Enter(void)
{
	if(APP_SYS_API_GetUpdateType()!= APP_SYS_OTA_UD_FORCE)
	{
		GUI_ENC_API_SetValue(ID_TIM_AutoInfo, WDT_TIM_SV_STOP, 0, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_TIM_AutoInfo, WDT_TIM_SV_PLAY, 0, 0);
	}
	APP_Com_API_SetFullScreen();

}
void UI_APP_AutoInfo_Key(BU32 key)
{
	BU32 lang;
	OTA_Info_S otaInfo;
	BU32 update_type = APP_SYS_API_GetUpdateType();
	switch(update_type)
	{
	case APP_SYS_OTA_UD_NULL:
		if(key == GUI_VK_SELECT)
		{
			GUI_ENC_API_SwitchState(ID_DLG_Search, 1);
			APP_NIT_API_SetNitVerCheck(0);
		}
		else if(key == GUI_VK_EXIT)
		{
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
			{
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
			}
			else
			{
			//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			}
		}
		break;
	case APP_SYS_OTA_UD_MANUAL:
		if(key == GUI_VK_SELECT)
		{
			GUI_ENC_API_SetValue(ID_STA_Line2_AutoInfo, WDT_STA_SV_DATAID, DATAID_NULL, 0);
			GUI_ENC_API_SetValue(ID_STA_Line3_AutoInfo, WDT_STA_SV_DATAID, RS_Wait, 0);
			APP_OTA_API_GetOTAType(&otaInfo);
		#if 1
			APP_SYS_API_SetSysInfo(APP_SYS_OTA_FREQ_e, otaInfo.freqMHz);	
//			APP_SYS_API_SetSysInfo(APP_SYS_OTA_FREQ_e, 363);
			APP_SYS_API_SetSysInfo(APP_SYS_OTA_SYMB, otaInfo.symKbps);	
			APP_SYS_API_SetSysInfo(APP_SYS_OTA_PID, otaInfo.downloadPid);	
			APP_SYS_API_SetSysInfo(APP_SYS_OTA_QAM, otaInfo.qam);	
			APP_SYS_API_SetSysInfo(APP_SYS_OTA_TYPE, APP_OTA_TYPE_AUTO);	
			APP_SYS_API_SaveSysInfo();
			//APP_SYS_IPI_WriteStbInfo();
		#endif
			GUI_ENC_API_SetValue(ID_TIM_AutoInfo, WDT_TIM_SV_PLAY, 0, 0);

		}
		else if(key == GUI_VK_EXIT)
		{
		//	APP_SYS_API_SetUpdateType(APP_SYS_OTA_UD_NULL);
		//	UI_APP_FirstState();
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
			{
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
			}
			else
			{
				//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			}
		}
		break;
	default:
		break;
	}
}
void UI_APP_AutoInfo_Timer(void)
{
	APP_SYS_API_WirteUpdateFlag();
}

