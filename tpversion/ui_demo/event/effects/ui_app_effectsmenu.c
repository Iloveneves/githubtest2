#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


void UI_APP_EffectsMenu_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_Effects);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Àı∑≈ ”∆µ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_Effects,ID_STA_event2_Effects,ID_PBR_Epg_Effects,260,1);
}


void UI_APP_EffectsMenu_Key(BU32 Key)
{
    BU32 curID;
	APP_PLAYER_TYPE_E play_type;
    GUI_ENC_API_GetValue(ID_MNU_Effects,WDT_MNU_SV_CURDATAID,&curID,0);
	if(GUI_VK_SELECT == Key)
	{
		switch(curID)
		{
		case RS_FileManager:
			if(APP_OK == APP_USB_API_DetectedUSB())
			{
				if(APP_USB_API_InitCurPath() == APP_FAIL)
				{
					break;
				}
				APP_USB_API_SaveCurDirInfo();
				GUI_ENC_API_SwitchState(ID_DLG_UsbManager,1);
			}
			else
			{
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);	
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_NoUSBDevice,0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
			}
			break;
#ifdef ENABLE_PVR
		case RS_MusicPlay:
			if (APP_OK == APP_USB_API_DetectedUSB())
			{
				play_type = APP_PLAYER_TYPE_MP3_e;
				APP_Player_API_SetCurPlayerType(play_type);
				GUI_ENC_API_SwitchState(ID_DLG_Player,1);
				APP_Com_API_ShowOtherBmp(4);
				//APP_Com_API_StopProg(1);
			}
			else
			{
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_NoUSBDevice,0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
			}
			break;
#endif

#ifdef ENABLE_JPEG
		case RS_PhotoFrame:
			if (APP_OK == APP_USB_API_DetectedUSB())
			{
				GUI_ENC_API_SwitchState(ID_DLG_PhotoFrame, 1);
				APP_Com_API_StopProg(1);
			}
			else
			{
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_NoUSBDevice,0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
			}
			break;
#endif
#ifdef ENABLE_PVR
		case RS_RecordPlay:
			if (APP_OK == APP_USB_API_DetectedUSB())
			{
				play_type = APP_PLAYER_TYPE_PVR_e;
				APP_Player_API_SetCurPlayerType(play_type);
				GUI_ENC_API_SwitchState(ID_DLG_PvrList, 1);
				APP_Com_API_StopProg(0);
				APP_player_API_DetachAV();
			}
			else
			{
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_NoUSBDevice,0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
			}
			break;
#if 0
		case RS_ManualRec:
			if (APP_OK == APP_USB_API_DetectedUSB())
			{
				GUI_ENC_API_SwitchState(ID_DLG_ManualRec, 1);
			}
			else
			{
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_DATAID,RS_NoUSBDevice,0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);
			}
			break;
#endif
#endif
		default:
			break;
		}
	}
	if (Key == GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
	}
	else if(Key == GUI_VK_RECALL)
	{
		GUI_ENC_API_SwitchState(ID_DLG_Game,1);
	}

}

