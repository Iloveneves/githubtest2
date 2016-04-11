#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"



void UI_APP_ProgManagerMenu_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	Hisi_SetUpdateDlgID(ID_DLG_ProgManager);
	//hwnd = GUI_API_GetDlgItem(ID_VFM_ProgManager);
	//GUI_API_GetWindowRect(hwnd,&rect);
	//APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_progmanager,ID_STA_event2_progmanager,ID_PBR_Epg_ProgManager,260,1);
}

void UI_APP_ProgManagerMenu_Key(BU32 Key)
{
    BU32 curID;
	BU32 menulock;
	BU32 avType;
	BU32 tmp_cur_no;
	BU32 progSortCount;
    GUI_ENC_API_GetValue(ID_MNU_ProgManager,WDT_MNU_SV_CURDATAID,&curID,0);
  
	if(GUI_VK_SELECT==Key)
	{
		avType = APP_ChnDatabase_API_GetCurChnType();
		switch(curID)
		{
		case RS_TVList:
		

			if (APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			//if(APP_SUCCESS == APP_ChnDatabase_API_CreatChnList(APP_CHN_AV_TYPE_TV_e,APP_CHN_GROUP_TYPE_ALL_e))
			{
	#if 0
				if(avType == APP_CHN_AV_TYPE_RD_e)
				{
					if(!UI_APP_API_bShowNoSignal())
					{
						APP_Com_API_ShowBmp(0);//黑屏
					}
				}
	#endif
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
				UI_APP_API_SetChnListPreState(ID_DLG_ProgManager);
				GUI_ENC_API_SwitchState(ID_DLG_ProgList,1);
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
				//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}
			else
			{	
				APP_Com_API_StopProg(1);
				if(avType == APP_CHN_AV_TYPE_RD_e)
				{
					if(!UI_APP_API_bShowNoSignal())
					{
						APP_Com_API_ShowBmp(0);//黑屏
					}
				}
				APP_Com_API_ShowLED("C000");
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
				APP_Com_API_SetEcmNULL();
				//APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e,APP_CHN_GROUP_TYPE_ALL_e);
				//APP_ChnDatabase_API_GetCurChnNO();
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}	
			break;
		case RS_RadioList:
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e,APP_CHN_GROUP_TYPE_ALL_e)&&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_SetChnListPreState(ID_DLG_ProgManager);
				GUI_ENC_API_SwitchState(ID_DLG_ProgList,1);
#if 0
				if(!UI_APP_API_bShowNoSignal())
				{
					APP_Com_API_ShowBmp(1);//广播LOGO
				}
#endif
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
				//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}
			else
			{	
				APP_Com_API_StopProg(1);
				if(avType == APP_CHN_AV_TYPE_TV_e)
				{
					if(!UI_APP_API_bShowNoSignal())
					{
						APP_Com_API_ShowBmp(1);//广播LOGO
					}
				}
				APP_Com_API_ShowLED("R000");
				APP_Com_API_SetEcmNULL();
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
				//APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e,APP_CHN_GROUP_TYPE_ALL_e);
				//APP_ChnDatabase_API_GetCurChnNO();
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoRadioProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}	
			break;
		case RS_BatList:
			progSortCount = APP_ChnDatabase_API_GetSortCount();
			//APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
			if(0 == progSortCount || APP_ChnDatabase_API_GetTypeChnNum(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e)== 0)
			{
				APP_Com_API_StopProg(1);
				if(avType == APP_CHN_AV_TYPE_RD_e)
				{
					if(!UI_APP_API_bShowNoSignal())
					{
						APP_Com_API_ShowBmp(0);//黑屏
					}
				}
				APP_Com_API_ShowLED("C000");
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
				APP_Com_API_SetEcmNULL();
				//提示无节目分类
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			else
			{
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
				GUI_ENC_API_SwitchState(ID_DLG_SortProgram,1);
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			}
			break;
		case RS_AutoSearch:
		case RS_AllSearch:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_Search,1);
			}
			break;		
		case RS_ManualSearch:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_ManualSearchSet,1);
			}
			break;
#if 0
		case RS_MainFreqSet:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_MainFreqSet,1);
			}
			break;
#endif
		case RS_TVEdit:
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
				UI_APP_API_SetChnEditPreState(ID_DLG_ProgManager);
				GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit,WDT_STA_SV_DATAID, RS_TVEdit, 0);
				GUI_ENC_API_SwitchState(ID_DLG_ChnEdit,1);
			}
			else
			{
				APP_Com_API_StopProg(1);
				if(avType == APP_CHN_AV_TYPE_RD_e)
				{
					if(!UI_APP_API_bShowNoSignal())
					{
						APP_Com_API_ShowBmp(0);//黑屏
					}
				}
				APP_Com_API_ShowLED("C000");
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
				APP_Com_API_SetEcmNULL();
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			break;
		case RS_RadioEdit:
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_SetChnEditPreState(ID_DLG_ProgManager);
				GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit,WDT_STA_SV_DATAID, RS_RadioEdit, 0);
				GUI_ENC_API_SwitchState(ID_DLG_ChnEdit,1);
			}
			else
			{
				APP_Com_API_StopProg(1);
				if(avType == APP_CHN_AV_TYPE_TV_e)
				{
					if(!UI_APP_API_bShowNoSignal())
					{
						APP_Com_API_ShowBmp(1);//广播LOGO
					}
				}
				APP_Com_API_ShowLED("R000");
				APP_Com_API_SetEcmNULL();
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoRadioProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			break;
	    case RS_ReserveManager:
			UI_APP_API_SetReserveListPreState(ID_DLG_ProgManager);
			GUI_ENC_API_SwitchState(ID_DLG_ReserveList,1);
		
			break;			
		default:
			break;
		}
	}
	if(Key == GUI_VK_MENU || Key == GUI_VK_RECALL)
	{
		GUI_ENC_API_SwitchState(ID_DLG_MainMenu,1);
	}
	else if(Key == GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
	}

}

