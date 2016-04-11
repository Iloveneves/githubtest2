#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

void UI_APP_FactoryMenu_Enter(void)
{
    BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_FacMenu);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Àı∑≈ ”∆µ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_FacMenu,ID_STA_event2_FacMenu,ID_PBR_Epg_FacMenu,260,1);
}

void UI_APP_FactoryMenu_End(void)
{

}

void UI_APP_FactoryMenu_Key(BU32 key)
{
    BU32 curMnuID;
    GUI_ENC_API_GetValue(ID_MNU_FacMenu,WDT_MNU_SV_CURDATAID, &curMnuID, 0);
	if(key == GUI_VK_SELECT)
	{
	    switch(curMnuID)
	    {
	    case RS_FreqSet:
			GUI_ENC_API_SwitchState(ID_DLG_FreqSet, 0);
			APP_PRINT_DEBUG("RS_FreqSet");
			break;
		case RS_Sequence:
			GUI_ENC_API_SwitchState(ID_DLG_Sequence, 0);
			break;
		case RS_OTA:
			GUI_ENC_API_SwitchState(ID_DLG_OTA, 0);
			break;
		case RS_CKSequence:
			GUI_ENC_API_SwitchState(ID_DLG_ComSequence, 0);
			break;
		case RS_DetectLed:
			APP_Com_API_LedDetect();
			GUI_ENC_API_SwitchState(ID_DLG_FacMenu, 0);
			break;
		case RS_ProduceResume:
			UI_APP_FactoryDefault_SetPrevState(ID_DLG_FacMenu);
			GUI_ENC_API_SwitchState(ID_DLG_Default, 1);
			break;
		case RS_DefaultDB:
			APP_SYS_API_SetDefaultDbase();
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			break;
		default:
			break;
	    }
	}
	else if(key == GUI_VK_MENU || key == GUI_VK_RECALL)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
	}
	else if(key == GUI_VK_EXIT)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
}