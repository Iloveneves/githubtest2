#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


void UI_APP_Game_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_Game);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Àı∑≈ ”∆µ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_Game,ID_STA_Event2_Game,ID_PBR_Epg_Game,260,1);

}
void UI_APP_Game_End(void)
{
	APP_Com_API_SetFullScreen();
}

void UI_APP_Game_Key(BU32 key)
{
	BU32 curID;
	GUI_ENC_API_GetValue(ID_MNU_Game,WDT_MNU_SV_CURDATAID,&curID,0);
	if(key == GUI_VK_SELECT)
	{
		switch(curID)
		{
		case RS_Teltris:
			GUI_ENC_API_SwitchState(ID_DLG_Russia,1);	
			break;
		case RS_Snake:
			GUI_ENC_API_SwitchState(ID_DLG_Snake,1);	
			break;
		case RS_Sokoban:
			GUI_ENC_API_SwitchState(ID_DLG_Box,1);	
			break;
		case RS_EffectsMenu:
#ifdef ENABLE_USB
			GUI_ENC_API_SwitchState(ID_DLG_Effects,1);
#endif
			break;
		default:
			break;
		}
	}
	else if(key == GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);	
	}
	else if(key == GUI_VK_RECALL || key == GUI_VK_MENU)
	{
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu,1);	
	}

}

