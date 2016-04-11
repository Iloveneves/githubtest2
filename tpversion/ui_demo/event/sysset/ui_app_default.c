#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

//≥ˆ≥ß…Ë÷√
static BU08 g_sys_saveData=0; 

static BU32 g_factory_prev_state;
void UI_APP_FactoryDefault_SetPrevState(BU32 state)
{
	g_factory_prev_state = state;
}

void UI_APP_FactoryDefault_Enter(void)
{
	g_sys_saveData = 0;
	APP_Com_API_SetFullScreen();
	GUI_ENC_API_SetValue(ID_STA_Line1_Default,WDT_STA_SV_DATAID,RS_Becarefull,0);
	GUI_ENC_API_SetValue(ID_STA_Line2_Default,WDT_STA_SV_DATAID,RS_ResetInfo,0);
	GUI_ENC_API_SetValue(ID_STA_Line3_Default,WDT_STA_SV_DATAID,RS_Ok_Cancel,0);
}
void UI_APP_FactoryDefault_End(void)
{

}
void UI_APP_FactoryDefault_Key(BU32 key)
{
	if(key == GUI_VK_SELECT)
	{
		g_sys_saveData = 1;
		GUI_ENC_API_SetValue(ID_STA_Line1_Default,WDT_STA_SV_STRING,(BU32)" ",0);
		GUI_ENC_API_SetValue(ID_STA_Line2_Default,WDT_STA_SV_DATAID,RS_Waitfactory,0);
		GUI_ENC_API_SetValue(ID_STA_Line3_Default,WDT_STA_SV_STRING,(BU32)" ",0);
		GUI_ENC_API_Update(ID_STA_Line1_Default,1);
		GUI_ENC_API_Update(ID_STA_Line2_Default,1);
		GUI_ENC_API_Update(ID_STA_Line3_Default,1);
	}
	else if(key == GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(g_factory_prev_state, 1);
	}
}
void UI_APP_FactoryDefault_Timer(void)
{
	if(g_sys_saveData)
	{
		g_sys_saveData = 0;
		if (g_factory_prev_state == ID_DLG_FacMenu)
		{
			APP_SYS_API_ClearCAData();
		}
		APP_SYS_API_SetToDefault();
#ifdef SYS_CONFIG_LANG_ENG
		GUI_API_SetDictionaryDataAddr(DicDataEnglish);
		UI_APP_SetIcoToEn();
#else
		GUI_API_SetDictionaryDataAddr(DicData);
		UI_APP_SetIcoToCn();
#endif
		UI_APP_Game_Russia_Default ();
		//UI_APP_CA_InitOsdShow();
#if 0
		if(g_sys_defaultToState == ID_DLG_FactoryMenu)
		{
			GUI_ENC_API_SwitchState(ID_DLG_FactoryMenu, 1);
		}
		else
#endif
		{
			APP_SYS_API_SetUpdateType(APP_SYS_OTA_UD_NULL);
			GUI_ENC_API_SetValue(ID_STA_Line2_AutoInfo, WDT_STA_SV_DATAID, RS_EnterSearch, 0);
			GUI_ENC_API_SetValue(ID_STA_Line3_AutoInfo, WDT_STA_SV_DATAID, RS_Exit, 0);
			GUI_ENC_API_SetValue(ID_STA_Line1_AutoInfo, WDT_STA_SV_DATAID, RS_Message, 0);
			GUI_ENC_API_SwitchState(ID_DLG_AutoInfo, 1);
		}
		APP_Com_API_ShowLED("C000");
		UI_APP_SYS_ShowSysTime();	
	}
}
