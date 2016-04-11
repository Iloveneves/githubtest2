#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

//系统升级

void UI_APP_OTA_Enter(void)
{
    BU32 freq,symb,pid,qamIndex;
    BU32 tableid = 254;

    BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_OTA);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_OTA,ID_STA_Event2_OTA,ID_PBR_Epg_OTA,260,1);
	APP_SYS_API_GetSysInfo(APP_SYS_OTA_FREQ_e, &freq);	
	APP_SYS_API_GetSysInfo(APP_SYS_OTA_SYMB, &symb);	
	APP_SYS_API_GetSysInfo(APP_SYS_OTA_PID, &pid);	
	APP_SYS_API_GetSysInfo(APP_SYS_OTA_QAM, &qamIndex);	
 
	GUI_ENC_API_SetValue(ID_TIM_OTA, WDT_TIM_SV_STOP, 0, 0);
	GUI_ENC_API_SetValue(ID_TBX_PID_OTA, WDT_TBX_SV_DATA, pid , 0);
	GUI_ENC_API_SetValue(ID_TBX_Freq_OTA, WDT_TBX_SV_DATA, freq , 0);
	GUI_ENC_API_SetValue(ID_TBX_Symb_OTA, WDT_TBX_SV_DATA, symb, 0);
	GUI_ENC_API_SetValue(ID_CBX_QAMMode_OTA, WDT_CBX_SV_INDEX, qamIndex, 0);

	GUI_ENC_API_SetValue(ID_TBX_PID_OTA,  GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_TBX_Freq_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
    GUI_ENC_API_SetValue(ID_CBX_QAMMode_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_Symb_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
}

void UI_APP_OTA_Key(BU32 key)
{
    BU32 freq,symb,pid,qamIndex;

    switch(key)
    {
	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_TBX_PID_OTA,  WDT_TBX_SV_DATA, &pid , 0);
		GUI_ENC_API_GetValue(ID_TBX_Freq_OTA, WDT_TBX_SV_DATA, &freq , 0);
		GUI_ENC_API_GetValue(ID_TBX_Symb_OTA, WDT_TBX_SV_DATA, &symb, 0);
		GUI_ENC_API_GetValue(ID_CBX_QAMMode_OTA, WDT_CBX_SV_INDEX, &qamIndex, 0);

		APP_SYS_API_SetSysInfo(APP_SYS_OTA_FREQ_e, freq*1000);	
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_SYMB,   symb*1000);
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_PID,     pid);
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_QAM, qamIndex);
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_TYPE, APP_OTA_TYPE_AUTO);

		
		APP_SYS_API_SaveSysInfo();
		GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo, WDT_STA_SV_DATAID, RS_Wait, 0);
		GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
        break;
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(ID_DLG_FacMenu, 1);
        break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
        break;
	case GUI_VK_UP:
		if(GUI_ENC_API_IsFocus(ID_TBX_PID_OTA))
		{
		    GUI_ENC_API_SetValue(ID_TBX_PID_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_QAMMode_OTA, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Freq_OTA))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Freq_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_PID_OTA, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Symb_OTA))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Symb_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Freq_OTA, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_QAMMode_OTA))
		{
		    GUI_ENC_API_SetValue(ID_CBX_QAMMode_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Symb_OTA, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		
		break;
	case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_TBX_PID_OTA))
		{
		    GUI_ENC_API_SetValue(ID_TBX_PID_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Freq_OTA, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Freq_OTA))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Freq_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_Symb_OTA, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_Symb_OTA))
		{
		    GUI_ENC_API_SetValue(ID_TBX_Symb_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_QAMMode_OTA, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_QAMMode_OTA))
		{
		    GUI_ENC_API_SetValue(ID_CBX_QAMMode_OTA, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_PID_OTA, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
		}
		break;
    }
}

void UI_APP_OTA_Timer(void)
{
	//APP_SYS_API_WirteUpdateFlag();

}
