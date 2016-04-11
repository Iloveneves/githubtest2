#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

extern BU08 g_ui_app_str[13][80];
//系统升级
BU08 *g_sys_sysupdate_p[2]=
{
	g_ui_app_str[8],
	g_ui_app_str[9]
};
//系统升级
void UI_APP_SysUpgrade_Enter(void)
{
	BU32 freq,symb,pid,qamIndex;
	BU32 tableid = 221;
	BU08 line1_sysupdate[40];
	BU08 line2_sysupdate[40];

	APP_Com_API_SetFullScreen();
	APP_SYS_API_GetSysInfo(APP_SYS_OTA_FREQ_e, &freq);	
	APP_SYS_API_GetSysInfo(APP_SYS_OTA_SYMB, &symb);	
	APP_SYS_API_GetSysInfo(APP_SYS_OTA_PID, &pid);	
	APP_SYS_API_GetSysInfo(APP_SYS_OTA_QAM, &qamIndex);	
	sprintf(g_sys_sysupdate_p[0],"%3dMHZ",freq);
	sprintf(g_sys_sysupdate_p[1],"%4dKbps",symb);

	GUI_ENC_API_SetValue(ID_TIM_SysUpdate, WDT_TIM_SV_STOP, 0, 0);
	GUI_ENC_API_SetValue(ID_TBX_PID_SysUpdate, WDT_TBX_SV_DATA, pid , 0);
	GUI_ENC_API_SetValue(ID_TBX_TableID_SysUpdate, WDT_TBX_SV_DATA, tableid, 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_SysUpdate, WDT_STA_SV_STRING, (BU32)g_sys_sysupdate_p[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_SysUpdate, WDT_STA_SV_STRING, (BU32)g_sys_sysupdate_p[1], 0);
	GUI_ENC_API_SetValue(ID_CBX_QAMMode_SysUpdate, WDT_CBX_SV_INDEX, qamIndex, 0);

}

void UI_APP_SysUpgrade_Key(BU32 key)
{
#if 1

	if(key == GUI_VK_SELECT)
	{
		UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_SYS_UPDATE);
		if (APP_NIT_API_GetOTACheck())
		{
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_GetUpdateData,0);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_GetUpdateDataFail,0);
		}
		GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
	}
#endif
	if(key == GUI_VK_RECALL || key == GUI_VK_MENU)
	{
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
	}
	else if(key == GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
}


