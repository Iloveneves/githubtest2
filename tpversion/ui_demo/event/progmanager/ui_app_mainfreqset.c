#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


//主频点设置
void UI_APP_MainFreqSet_Enter(void)
{
	BU32 main_freq=0;
	BU32 hwnd;
	GUI_Rect_s rect;
	APP_SYS_API_GetSysInfo(APP_SYS_MAIN_FREQ_e, &main_freq);	
	GUI_ENC_API_SetValue(ID_CBX_Ctr_MainFreqSet, WDT_CBX_SV_INDEX, 0, 0);
	GUI_ENC_API_SetValue(ID_TBX_MainFreqSet_MainFreqSet, WDT_TBX_SV_DATA, main_freq, 0);
//	GUI_ENC_API_Enable(ID_TBX_MainFreqSet_MainFreqSet, 0);
	hwnd = GUI_API_GetDlgItem(ID_VFM_MainFreqSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_MainFreqSet,ID_STA_event2_MainFreqSet,ID_PBR_Epg_MainFreqSet,260,1);
	GUI_ENC_API_SetValue(ID_CBX_Ctr_MainFreqSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_TBX_MainFreqSet_MainFreqSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
}

void UI_APP_MainFreqSet_Key(BU32 key)
{
	BU32 ctrl_flag=0;
	BU32 main_freq=0;
	switch(key)
	{
		case GUI_VK_LEFT:
		case GUI_VK_RIGHT:
			if(GUI_ENC_API_IsFocus(ID_CBX_Ctr_MainFreqSet))
			{
				GUI_ENC_API_GetValue(ID_CBX_Ctr_MainFreqSet, WDT_CBX_SV_INDEX, &ctrl_flag, 0);
				if(ctrl_flag)
				{
					GUI_ENC_API_SetValue(ID_TBX_MainFreqSet_MainFreqSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
					//GUI_ENC_API_Enable(ID_TBX_MainFreqSet_MainFreqSet, 0);
				}
				else
				{
					GUI_ENC_API_SetValue(ID_TBX_MainFreqSet_MainFreqSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
					//GUI_ENC_API_Enable(ID_TBX_MainFreqSet_MainFreqSet, 1);
				}
			}
			break;
		case GUI_VK_UP:
		case GUI_VK_DOWN:
			if(GUI_ENC_API_IsFocus(ID_CBX_Ctr_MainFreqSet))
			{
				GUI_ENC_API_GetValue(ID_CBX_Ctr_MainFreqSet, WDT_CBX_SV_INDEX, &ctrl_flag, 0);
				if(ctrl_flag)
				{
					GUI_ENC_API_SetFocus(ID_TBX_MainFreqSet_MainFreqSet);
					GUI_ENC_API_SetValue(ID_CBX_Ctr_MainFreqSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
					GUI_ENC_API_SetValue(ID_TBX_MainFreqSet_MainFreqSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
				}
			}
			else
			{
				GUI_ENC_API_SetFocus(ID_CBX_Ctr_MainFreqSet);
				GUI_ENC_API_SetValue(ID_CBX_Ctr_MainFreqSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
				GUI_ENC_API_SetValue(ID_TBX_MainFreqSet_MainFreqSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			}
			break;
		case GUI_VK_SELECT:
			GUI_ENC_API_GetValue(ID_TBX_MainFreqSet_MainFreqSet, WDT_TBX_SV_DATA, &main_freq, 0);
			APP_SYS_API_SetSysInfo(APP_SYS_MAIN_FREQ_e, main_freq);	
			APP_SYS_API_SaveSysInfo();
			GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
			break;
		case GUI_VK_EXIT:
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			break;
		case GUI_VK_RECALL:
		case GUI_VK_MENU:
			GUI_ENC_API_SwitchState(ID_DLG_SysSet,1);
			break;

	}

}

