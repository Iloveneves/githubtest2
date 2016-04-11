#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

BU32 ProgManager_IPI_SwitchChannel(void)
{
	BU32 avType;
	BU32 menulock;
	BU32 num = APP_ChnDatabase_API_GetTypeChnNum(APP_CHN_AV_TYPE_TV_e,APP_CHN_GROUP_TYPE_ALL_e);


	return (num > 0)?APP_SUCCESS:APP_FAIL;
#if 0
	avType = APP_ChnDatabase_API_GetCurChnType();
	if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e,APP_CHN_GROUP_TYPE_ALL_e))
	{

		if(avType == APP_CHN_AV_TYPE_RD_e)
		{	
			if(!UI_APP_API_bShowNoSignal())
			{
				APP_Com_API_ShowBmp(1);//ºÚÆÁ
			}
			//APP_Com_API_StopProg(1);//¹ã²¥µ½µçÊÓ ºÚÆÁ
		}
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		return APP_SUCCESS;
	}
	else
	{		
		APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e,APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_GetCurChnNO();
		return APP_FAIL;
	}	
#endif
}


BU32 ProgManager_IPI_SwitchRadio(void)
{
	BU32 menulock;
	BU32 num = APP_ChnDatabase_API_GetTypeChnNum(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);


	return (num > 0)?APP_SUCCESS:APP_FAIL;
#if 0
	if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e,APP_CHN_GROUP_TYPE_ALL_e))
	{

		if(!UI_APP_API_bShowNoSignal())
		{
			APP_Com_API_ShowBmp(1);//¹ã²¥LOGO
		}

		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		return APP_SUCCESS;
	}
	else
	{		
		APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e,APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_GetCurChnNO();
		return APP_FAIL;
	}	
#endif
}
void UI_APP_SysSetMenu_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_SysSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//Ëõ·ÅÊÓÆµ
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_SysSet,ID_STA_Event2_SysSet,ID_PBR_Epg_SysSet,260,1);
}
void UI_APP_SysSetMenu_End(void)
{

}
void UI_APP_SysSetMenu_Key(BU32 key)
{
	BU32 curID;
	BU32 menulock;
	BU32 tmp_cur_no;
	BU16 curChn,chnNum ;
	APP_CHN_AV_TYPE_E avType;
	GUI_ENC_API_GetValue(ID_MNU_SysSet,WDT_MNU_SV_CURDATAID,&curID,0);

	
 	if(key == GUI_VK_SELECT)
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
						APP_Com_API_ShowBmp(0);//ºÚÆÁ
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
						APP_Com_API_ShowBmp(0);//ºÚÆÁ
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
					APP_Com_API_ShowBmp(1);//¹ã²¥LOGO
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
						APP_Com_API_ShowBmp(1);//¹ã²¥LOGO
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

#if 0
		case RS_FavChn:
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_SetChnListPreState(ID_DLG_ProgManager);
				GUI_ENC_API_SwitchState(ID_DLG_ChannelEdit,1);
			}
			else
			{		
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			break;
		case RS_DelChn:
		case RS_ChangeChn:
		case RS_LockChn:
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_SetChnListPreState(ID_DLG_ProgManager);
				APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
				if(menulock)
				{
					GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
				}
				else
				{
					GUI_ENC_API_SwitchState(ID_DLG_ChannelEdit,1);
				}
			}
			else
			{		
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			break;
		case RS_FavRadio:
			if (APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_SetChnListPreState(ID_DLG_ProgManager);
				GUI_ENC_API_SwitchState(ID_DLG_ChannelEdit,1);
			}
			else
			{		
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoRadioProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			break;
		case RS_DelRadio:
		case RS_ChangeRadio:
		case RS_LockRadio:
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_SetChnListPreState(ID_DLG_ProgManager);
				APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
				if(menulock)
				{
					GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
				}
				else
				{
					GUI_ENC_API_SwitchState(ID_DLG_ChannelEdit,1);
				}
			}
			else
			{		
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoRadioProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			break;
#else
		case RS_TVEdit:

			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock, 1);
			}
			else
			{
				if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
					APP_ChnDatabase_API_GetCurTypeChnNum())
				{
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
					UI_APP_API_SetChnEditPreState(0);
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
							APP_Com_API_ShowBmp(0);//ºÚÆÁ
						}
					}
					APP_Com_API_ShowLED("C000");
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
					APP_Com_API_SetEcmNULL();
					GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
					GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
				}
			}
			break;
		case RS_RadioEdit:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock, 1);
			}
			else
			{
				if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e) &&
					APP_ChnDatabase_API_GetCurTypeChnNum())
				{
					UI_APP_API_SetChnEditPreState(0);
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
							APP_Com_API_ShowBmp(1);//¹ã²¥LOGO
						}
					}
					APP_Com_API_ShowLED("R000");
					APP_Com_API_SetEcmNULL();
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
					GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoRadioProg,0);
					GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
				}
			}
			break;
#endif
	    case RS_ReserveManager:
			UI_APP_API_SetReserveListPreState(ID_DLG_SysSet);
			GUI_ENC_API_SwitchState(ID_DLG_ReserveList,1);
			break;
		case RS_OperationGuide:
			GUI_ENC_API_SwitchState(ID_DLG_OperationGuide, 1);
			break;
		case RS_Info:
			GUI_ENC_API_SwitchState(ID_DLG_StbInfo, 1);
			break;
		case RS_TimeZoneSetting:
			GUI_ENC_API_SwitchState(ID_DLG_TimeZoneSet, 1);
			break;
		case RS_FactoryDefault:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				UI_APP_FactoryDefault_SetPrevState(ID_DLG_SysSet);
				GUI_ENC_API_SwitchState(ID_DLG_Default, 1);
			}
			break;
		case RS_SoftWareUpdate:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_SysUpdate, 1);
			}
			break;
		case RS_SignalDetect:
			GUI_ENC_API_SwitchState(ID_DLG_SignalCheck, 1);
			break;
		case RS_TVSetting:
			GUI_ENC_API_SwitchState(ID_DLG_AVSet, 1);			
			break;
		case RS_PasswordSetting:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
			    GUI_ENC_API_SwitchState(ID_DLG_MenuLock,1);
			}
			else
			{
			    GUI_ENC_API_SwitchState(ID_DLG_PasswordSet, 1);	
			}
			break;
		case RS_LanguageSetting:
			GUI_ENC_API_SwitchState(ID_DLG_LanguageSet, 1);			
			break;
		case RS_NetSetting:
#if 1
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock, 1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_NetSet, 1);
			}
#endif
			break;
		case RS_ParameterSet:
			GUI_ENC_API_SwitchState(ID_DLG_DisplaySet, 1);
			break;
#if 0
		case RS_OutPutSet:
			GUI_ENC_API_SwitchState(ID_DLG_OutPutSet, 1);	
			break;
		case RS_DisplaySet:
			GUI_ENC_API_SwitchState(ID_DLG_DisplaySet, 1);
			break;
		case RS_ChannelSet:
	
			if(APP_FAIL == APP_ChnDatabase_API_GetTypeChnNum(avType,APP_CHN_GROUP_TYPE_ALL_e))
			{
			    GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
			}
			else
			{
			    GUI_ENC_API_SwitchState(ID_DLG_ChannelSet, 1);
			}
			break;
#endif
		case RS_ParentControl:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock, 1);
			}
			else
			{
			    GUI_ENC_API_SwitchState(ID_DLG_ParentControl, 1);
			}
			break;
		case RS_LockChn:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock, 1);
			}
			else
			{
				if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
				{
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
					UI_APP_API_SetChnEditPreState(1);
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
							APP_Com_API_ShowBmp(0);//ºÚÆÁ
						}
					}
					APP_Com_API_ShowLED("C000");
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
					APP_Com_API_SetEcmNULL();
					GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoTVProg,0);
					GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
				}
			}
			break;
		case RS_LockRadio:
			APP_SYS_API_GetSysInfo(APP_SYS_MENU_LOCK_e, &menulock);
			if(menulock)
			{
				GUI_ENC_API_SwitchState(ID_DLG_MenuLock, 1);
			}
			else
			{
				if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
				{
					
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
					UI_APP_API_SetChnEditPreState(1);
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
							APP_Com_API_ShowBmp(1);//¹ã²¥LOGO
						}
					}
					APP_Com_API_ShowLED("R000");
					APP_Com_API_SetEcmNULL();
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
					GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoRadioProg,0);
					GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
				}
			}
			break;
		default:
			break;
		}
 	}
	else if(key == GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
	else if(key == GUI_VK_RECALL || key == GUI_VK_MENU)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
	}

}

