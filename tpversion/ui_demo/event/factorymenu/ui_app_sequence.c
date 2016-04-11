#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


//–Ú¡–∫≈
BU08 g_factory_snStr[23];
void UI_APP_Sequence_Enter(void)
{
	BU32 stbtype,factory,year,week,sn;
	BU08 year_str[5]={0};

    BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_Sequence);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Àı∑≈ ”∆µ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_Sequence,ID_STA_Event2_Sequence,ID_PBR_Epg_Sequence,260,1);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_NUM, &factory);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_STBTYPE, &stbtype);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_YEAR, &year);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_WEEK, &week);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &sn);
	
	GUI_ENC_API_SetValue(ID_TBX_Manufacture_Sequence, WDT_TBX_SV_DATA, factory, 0);
	GUI_ENC_API_SetValue(ID_TBX_StbType_Sequence, WDT_TBX_SV_DATA, stbtype, 0);
	GUI_ENC_API_SetValue(ID_TBX_Productyear_Sequence, WDT_TBX_SV_DATA, year, 0);
	GUI_ENC_API_SetValue(ID_TBX_Productweek_Sequence, WDT_TBX_SV_DATA, week, 0);
	GUI_ENC_API_SetValue(ID_TBX_Sequence_Sequence, WDT_TBX_SV_DATA, sn, 0);

	GUI_API_AddDecStr(year, 4, year_str);
	year = GUI_API_AddStrDec(year_str+2);
	sprintf(g_factory_snStr,"%02d-%02d-%02d-%02d-%010d",factory,stbtype,year,week,sn);
	GUI_ENC_API_SetValue(ID_STA_Info_Sequence, WDT_STA_SV_STRING, (BU32)g_factory_snStr,0);

	GUI_ENC_API_SetValue(ID_TBX_Manufacture_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_TBX_StbType_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_Productyear_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_Productweek_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_Sequence_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
//	APP_Com_API_SetSmallScreen(70, 100, 275, 205);
}
void UI_APP_Sequence_Key(BU32 key)
{
    BU32 year,week,type,factory_num,sn;
	BU08 year_str[5]={0};
	
	switch(key)
	{
	case GUI_VK_0:
	case GUI_VK_1:
	case GUI_VK_2:
	case GUI_VK_3:
	case GUI_VK_4:
	case GUI_VK_5:
	case GUI_VK_6:
	case GUI_VK_7:
	case GUI_VK_8:
	case GUI_VK_9:
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		GUI_ENC_API_GetValue(ID_TBX_Manufacture_Sequence, WDT_TBX_SV_DATA, &factory_num, 0);
		GUI_ENC_API_GetValue(ID_TBX_StbType_Sequence, WDT_TBX_SV_DATA, &type, 0);
		GUI_ENC_API_GetValue(ID_TBX_Productyear_Sequence, WDT_TBX_SV_DATA, &year, 0);
		GUI_ENC_API_GetValue(ID_TBX_Productweek_Sequence, WDT_TBX_SV_DATA, &week, 0);
		GUI_ENC_API_GetValue(ID_TBX_Sequence_Sequence, WDT_TBX_SV_DATA, &sn, 0);
		GUI_API_AddDecStr(year, 4, year_str);
		year = GUI_API_AddStrDec(year_str+2);
		sprintf(g_factory_snStr,"%02d-%02d-%02d-%02d-%010d",factory_num,type,year,week,sn);
		GUI_ENC_API_SetValue(ID_STA_Info_Sequence, WDT_STA_SV_STRING, (BU32)g_factory_snStr,0);
		break;
	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_TBX_Manufacture_Sequence, WDT_TBX_SV_DATA, &factory_num, 0);
		GUI_ENC_API_GetValue(ID_TBX_StbType_Sequence, WDT_TBX_SV_DATA, &type, 0);
		GUI_ENC_API_GetValue(ID_TBX_Productyear_Sequence, WDT_TBX_SV_DATA, &year, 0);
		GUI_ENC_API_GetValue(ID_TBX_Productweek_Sequence, WDT_TBX_SV_DATA, &week, 0);
		GUI_ENC_API_GetValue(ID_TBX_Sequence_Sequence, WDT_TBX_SV_DATA, &sn, 0);
		APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_NUM, factory_num);
		APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_STBTYPE, type);
		APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_YEAR, year);	
		APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_WEEK, week);	
		APP_SYS_API_SetSysInfo(APP_SYS_FACTORY_SN, sn);
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SwitchState(ID_DLG_FacMenu, 1);
		break;
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_FacMenu,1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
		break;
	case GUI_VK_UP:
		if(GUI_ENC_API_IsFocus(ID_TBX_Manufacture_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Manufacture_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Sequence_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_StbType_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_StbType_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Manufacture_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Productyear_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Productyear_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_StbType_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Productweek_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Productweek_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Productyear_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Sequence_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Sequence_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Productweek_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		break;
	case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_TBX_Manufacture_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Manufacture_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_StbType_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_StbType_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_StbType_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Productyear_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Productyear_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Productyear_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_BroadFreq_FreqSet, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Productweek_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Productweek_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Sequence_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Sequence_Sequence))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Sequence_Sequence, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Manufacture_Sequence, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		break;
	default:
		break;
			
	}
}