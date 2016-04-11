#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

void UI_APP_FreqSet_Enter(void)
{
    BU32 mainFreq;
	BU32 AdFreq;
	BU32 NvodFreq;
	BU32 BroadFreq;
	BU32 StockFreq;

    BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_FreqSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Àı∑≈ ”∆µ
    UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_FreqSet,ID_STA_Event2_FreqSet,ID_PBR_Epg_FreqSet,260,1);
	APP_SYS_API_GetSysInfo(APP_SYS_MAIN_FREQ_e, &mainFreq);
	APP_SYS_API_GetSysInfo(APP_SYS_AD_FREQ_e, &AdFreq);
	APP_SYS_API_GetSysInfo(APP_SYS_NVOD_FREQ_e, &NvodFreq);
	APP_SYS_API_GetSysInfo(APP_SYS_DB_FREQ_e, &BroadFreq);
	APP_SYS_API_GetSysInfo(APP_SYS_STOCK_FREQ_e, &StockFreq);

	GUI_ENC_API_SetValue(ID_TBX_MainFreq_FreqSet, WDT_TBX_SV_DATA, (BU32)mainFreq, 0);
	GUI_ENC_API_SetValue(ID_TBX_AdFreq_FreqSet, WDT_TBX_SV_DATA, (BU32)AdFreq, 0);
	GUI_ENC_API_SetValue(ID_TBX_NvodFreq_FreqSet, WDT_TBX_SV_DATA, (BU32)NvodFreq, 0);
	GUI_ENC_API_SetValue(ID_TBX_BroadFreq_FreqSet, WDT_TBX_SV_DATA, (BU32)BroadFreq, 0);
	GUI_ENC_API_SetValue(ID_TBX_StockFreq_FreqSet, WDT_TBX_SV_DATA, (BU32)StockFreq, 0);

	GUI_ENC_API_SetValue(ID_TBX_MainFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_TBX_AdFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_NvodFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_BroadFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_StockFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
}

void UI_APP_FreqSet_Key(BU32 key)
{
    BU32 mainFreq;
	BU32 AdFreq;
	BU32 NvodFreq;
	BU32 BroadFreq;
	BU32 StockFreq;
    switch(key)
    {
    case GUI_VK_SELECT:
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_GetValue(ID_TBX_MainFreq_FreqSet, WDT_TBX_SV_DATA, &mainFreq, 0);
		GUI_ENC_API_GetValue(ID_TBX_AdFreq_FreqSet, WDT_TBX_SV_DATA, &AdFreq, 0);
		GUI_ENC_API_GetValue(ID_TBX_NvodFreq_FreqSet, WDT_TBX_SV_DATA, &NvodFreq, 0);
		GUI_ENC_API_GetValue(ID_TBX_BroadFreq_FreqSet, WDT_TBX_SV_DATA, &BroadFreq, 0);
		GUI_ENC_API_GetValue(ID_TBX_StockFreq_FreqSet, WDT_TBX_SV_DATA, &StockFreq, 0);
		APP_SYS_API_SetSysInfo(APP_SYS_MAIN_FREQ_e, mainFreq);
		APP_SYS_API_SetSysInfo(APP_SYS_AD_FREQ_e, AdFreq);
		APP_SYS_API_SetSysInfo(APP_SYS_NVOD_FREQ_e, NvodFreq);
		APP_SYS_API_SetSysInfo(APP_SYS_DB_FREQ_e, BroadFreq);
		APP_SYS_API_SetSysInfo(APP_SYS_STOCK_FREQ_e, StockFreq);
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_FacMenu, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_GetValue(ID_TBX_MainFreq_FreqSet, WDT_TBX_SV_DATA, &mainFreq, 0);
		GUI_ENC_API_GetValue(ID_TBX_AdFreq_FreqSet, WDT_TBX_SV_DATA, &AdFreq, 0);
		GUI_ENC_API_GetValue(ID_TBX_NvodFreq_FreqSet, WDT_TBX_SV_DATA, &NvodFreq, 0);
		GUI_ENC_API_GetValue(ID_TBX_BroadFreq_FreqSet, WDT_TBX_SV_DATA, &BroadFreq, 0);
		GUI_ENC_API_GetValue(ID_TBX_StockFreq_FreqSet, WDT_TBX_SV_DATA, &StockFreq, 0);
		APP_SYS_API_SetSysInfo(APP_SYS_MAIN_FREQ_e, mainFreq);
		APP_SYS_API_SetSysInfo(APP_SYS_AD_FREQ_e, AdFreq);
		APP_SYS_API_SetSysInfo(APP_SYS_NVOD_FREQ_e, NvodFreq);
		APP_SYS_API_SetSysInfo(APP_SYS_DB_FREQ_e, BroadFreq);
		APP_SYS_API_SetSysInfo(APP_SYS_STOCK_FREQ_e, StockFreq);
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_UP:
		if(GUI_ENC_API_IsFocus(ID_TBX_MainFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_MainFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_StockFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_AdFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_AdFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_MainFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_NvodFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_NvodFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_AdFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_BroadFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_BroadFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_NvodFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_StockFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_StockFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_BroadFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		break;
	case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_TBX_MainFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_MainFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_AdFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_AdFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_AdFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_NvodFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_NvodFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_NvodFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_BroadFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_BroadFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_BroadFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_StockFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_StockFreq_FreqSet))
		{
		    GUI_ENC_API_SetValue(ID_TBX_StockFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_MainFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		break;
	default:
		break;
    }
}

