#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define SYS_LANG_SET

//”Ô—‘…Ë÷√
void UI_APP_LangSet_Enter(void)
{
	BU32 g_sys_language=0;
	BU32 hwnd;
	GUI_Rect_s rect;
	APP_EPG_API_StopGetEpg();
	hwnd = GUI_API_GetDlgItem(ID_VFM_LanguageSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Àı∑≈ ”∆µ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_LanguageSet,ID_STA_Event2_LanguageSet,ID_PBR_Epg_LanguageSet,260,1);

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e,&g_sys_language);
	
	GUI_ENC_API_SetValue(ID_CBX_LangChoice_LanguageSet,WDT_CBX_SV_INDEX,(BU32)g_sys_language,0);
	GUI_ENC_API_SetValue(ID_CBX_LangChoice_LanguageSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);

}
void UI_APP_LangSet_End(void)
{

}
void UI_APP_LangSet_Key(BU32 key)
{
	BU32 index;
	switch(key)
	{
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		GUI_ENC_API_GetValue(ID_CBX_LangChoice_LanguageSet,WDT_CBX_SV_INDEX,&index,0);
		if(index)
		{		
			GUI_API_SetDictionaryDataAddr(DicData);
			UI_APP_SetIcoToCn();
		}
		else
		{
			GUI_API_SetDictionaryDataAddr(DicDataEnglish);
			UI_APP_SetIcoToEn();
		}
		APP_SYS_API_SetSysInfo(APP_SYS_LANGUAGE_e,index);
		UI_APP_SYS_ShowSysTime();
		GUI_ENC_API_Update(ID_DLG_LanguageSet,1);
		break;
	case GUI_VK_UP:
	case GUI_VK_DOWN:
		break;
    case GUI_VK_EXIT:
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_SELECT:
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	default:
		break;
	}
}

