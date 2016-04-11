#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

extern BU32 g_first_mainmenu;
static BU32 g_key_count = 1;
static BU32 g_key_receive = 0;
BU08 g_MainMenu_MatchTime = 0;
BU08 g_MainMenu_Region_MatchTime = 0;
BU08 g_MainMenu_StbID_MatchTime = 0;
void UI_APP_MainMenu_Enter(void)
{
	BU32 hwnd;
	GUI_Rect_s rect;
	BU32 index;
	 Hisi_SetUpdateDlgID(ID_DLG_MainMenu);
//	g_key_count = 1;
	if(GUI_ENC_API_GetPrevPrevState() == ID_DLG_Nvod || GUI_ENC_API_GetPrevState() == ID_DLG_Nvod)
	{
		APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_JudgeChn0();	
		if(APP_ChnDatabase_API_GetCurTypeChnNum() != 0)
		{			
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);//播放节目
		}
		else
		{
			APP_Com_IPI_StopProg(1);
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
		}
	}
	/*hwnd = GUI_API_GetDlgItem(ID_VFM_MainMenu);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	*/
	GUI_ENC_API_Update(ID_ODC_MainMenu,g_key_count);

	g_MainMenu_MatchTime = 0;
	g_MainMenu_Region_MatchTime = 0;
	g_MainMenu_StbID_MatchTime = 0;
	if (g_first_mainmenu == 1)
	{
		GUI_ENC_API_SetValue(ID_TIM_MainMenu, WDT_TIM_SV_PLAY, 0, 0);
		g_first_mainmenu = 0;
	}
}

void UI_APP_MainMenu_End(void)
{

}
void UI_APP_MainMenu_OnPaint(BU32 hdc,BU32 lparam)
{
	lparam = g_key_count;
	if(lparam==1)//初始化
	{
		GUI_API_DrawStyle(hdc,0,107,STYLE_ChnManageMenu1On_W300_H70);
		GUI_API_DrawStyle(hdc,0,177,STYLE_VodMenu2No_W300_H45);
		GUI_API_DrawStyle(hdc,0,222,STYLE_StockMenu3No_W300_H45);
		GUI_API_DrawStyle(hdc,0,267,STYLE_BrdCastMenu4No_W300_H45);
		GUI_API_DrawStyle(hdc,0,312,STYLE_ChnGuideMenu5No_W300_H45);
		GUI_API_DrawStyle(hdc,0,357,STYLE_CaMenu6No_W300_H45);
		GUI_API_DrawStyle(hdc,0,402,STYLE_GameMenu7No_W300_H45);
		GUI_API_DrawStyle(hdc,0,447,STYLE_SysSetMenu8No_W300_H45);
	}
	else if(lparam==2)
	{
		GUI_API_DrawStyle(hdc,0,107,STYLE_ChnManageMenu1No_W300_H45);
		GUI_API_DrawStyle(hdc,0,152,STYLE_VodMenu2On_W300_H70);
		GUI_API_DrawStyle(hdc,0,222,STYLE_StockMenu3No_W300_H45);
		GUI_API_DrawStyle(hdc,0,267,STYLE_BrdCastMenu4No_W300_H45);
		GUI_API_DrawStyle(hdc,0,312,STYLE_ChnGuideMenu5No_W300_H45);
		GUI_API_DrawStyle(hdc,0,357,STYLE_CaMenu6No_W300_H45);
		GUI_API_DrawStyle(hdc,0,402,STYLE_GameMenu7No_W300_H45);
		GUI_API_DrawStyle(hdc,0,447,STYLE_SysSetMenu8No_W300_H45);
	}
	else if(lparam==3)
	{
		GUI_API_DrawStyle(hdc,0,107,STYLE_ChnManageMenu1No_W300_H45);
		GUI_API_DrawStyle(hdc,0,152,STYLE_VodMenu2No_W300_H45);
		GUI_API_DrawStyle(hdc,0,197,STYLE_StockMenu3On_W300_H70);
		GUI_API_DrawStyle(hdc,0,267,STYLE_BrdCastMenu4No_W300_H45);
		GUI_API_DrawStyle(hdc,0,312,STYLE_ChnGuideMenu5No_W300_H45);
		GUI_API_DrawStyle(hdc,0,357,STYLE_CaMenu6No_W300_H45);
		GUI_API_DrawStyle(hdc,0,402,STYLE_GameMenu7No_W300_H45);
		GUI_API_DrawStyle(hdc,0,447,STYLE_SysSetMenu8No_W300_H45);
	}
	else if(lparam==4)
	{
		GUI_API_DrawStyle(hdc,0,107,STYLE_ChnManageMenu1No_W300_H45);
		GUI_API_DrawStyle(hdc,0,152,STYLE_VodMenu2No_W300_H45);
		GUI_API_DrawStyle(hdc,0,197,STYLE_StockMenu3No_W300_H45);
		GUI_API_DrawStyle(hdc,0,242,STYLE_BrdCastMenu4On_W300_H70);
		GUI_API_DrawStyle(hdc,0,312,STYLE_ChnGuideMenu5No_W300_H45);
		GUI_API_DrawStyle(hdc,0,357,STYLE_CaMenu6No_W300_H45);
		GUI_API_DrawStyle(hdc,0,402,STYLE_GameMenu7No_W300_H45);
		GUI_API_DrawStyle(hdc,0,447,STYLE_SysSetMenu8No_W300_H45);
	}
	else if(lparam==5)
	{
		GUI_API_DrawStyle(hdc,0,107,STYLE_ChnManageMenu1No_W300_H45);
		GUI_API_DrawStyle(hdc,0,152,STYLE_VodMenu2No_W300_H45);
		GUI_API_DrawStyle(hdc,0,197,STYLE_StockMenu3No_W300_H45);
		GUI_API_DrawStyle(hdc,0,242,STYLE_BrdCastMenu4No_W300_H45);
		GUI_API_DrawStyle(hdc,0,287,STYLE_ChnGuideMenu5On_W300_H70);
		GUI_API_DrawStyle(hdc,0,357,STYLE_CaMenu6No_W300_H45);
		GUI_API_DrawStyle(hdc,0,402,STYLE_GameMenu7No_W300_H45);
		GUI_API_DrawStyle(hdc,0,447,STYLE_SysSetMenu8No_W300_H45);
	}
	else if(lparam==6)
	{
		GUI_API_DrawStyle(hdc,0,107,STYLE_ChnManageMenu1No_W300_H45);
		GUI_API_DrawStyle(hdc,0,152,STYLE_VodMenu2No_W300_H45);
		GUI_API_DrawStyle(hdc,0,197,STYLE_StockMenu3No_W300_H45);
		GUI_API_DrawStyle(hdc,0,242,STYLE_BrdCastMenu4No_W300_H45);
		GUI_API_DrawStyle(hdc,0,287,STYLE_ChnGuideMenu5No_W300_H45);
		GUI_API_DrawStyle(hdc,0,332,STYLE_CaMenu6On_W300_H70);
		GUI_API_DrawStyle(hdc,0,402,STYLE_GameMenu7No_W300_H45);
		GUI_API_DrawStyle(hdc,0,447,STYLE_SysSetMenu8No_W300_H45);
	}
	else if(lparam==7)
	{
		GUI_API_DrawStyle(hdc,0,107,STYLE_ChnManageMenu1No_W300_H45);
		GUI_API_DrawStyle(hdc,0,152,STYLE_VodMenu2No_W300_H45);
		GUI_API_DrawStyle(hdc,0,197,STYLE_StockMenu3No_W300_H45);
		GUI_API_DrawStyle(hdc,0,242,STYLE_BrdCastMenu4No_W300_H45);
		GUI_API_DrawStyle(hdc,0,287,STYLE_ChnGuideMenu5No_W300_H45);
		GUI_API_DrawStyle(hdc,0,332,STYLE_CaMenu6No_W300_H45);
		GUI_API_DrawStyle(hdc,0,377,STYLE_GameMenu7On_W300_H70);
		GUI_API_DrawStyle(hdc,0,447,STYLE_SysSetMenu8No_W300_H45);
	}
	else if(lparam==8)
	{
		GUI_API_DrawStyle(hdc,0,107,STYLE_ChnManageMenu1No_W300_H45);
		GUI_API_DrawStyle(hdc,0,152,STYLE_VodMenu2No_W300_H45);
		GUI_API_DrawStyle(hdc,0,197,STYLE_StockMenu3No_W300_H45);
		GUI_API_DrawStyle(hdc,0,242,STYLE_BrdCastMenu4No_W300_H45);
		GUI_API_DrawStyle(hdc,0,287,STYLE_ChnGuideMenu5No_W300_H45);
		GUI_API_DrawStyle(hdc,0,332,STYLE_CaMenu6No_W300_H45);
		GUI_API_DrawStyle(hdc,0,377,STYLE_GameMenu7No_W300_H45);
		GUI_API_DrawStyle(hdc,0,422,STYLE_SysSetMenu8On_W300_H70);
	}
}
void UI_APP_MainMenu_OnKey(BU32 key)
{
	BU32 broadcastNum = 0;
	BU32 netEnable = 1;/*1表示关(单向),0表示开(双向)*/
	BU32 avType;
	avType = APP_ChnDatabase_API_GetCurChnType();
	g_key_receive++;
	
	if(GUI_VK_0 == key)
	{
		g_MainMenu_MatchTime = 1;
	}
	else if(g_MainMenu_MatchTime == 1 && GUI_VK_1 == key)
	{
		g_MainMenu_MatchTime = 2;
	}
	else if(g_MainMenu_MatchTime == 2 && GUI_VK_2 == key)
	{
		//APP_Com_API_StopProg(1);
		UI_APP_SetDefaultIP();
		GUI_ENC_API_SwitchState(ID_DLG_FacMenu,1);
		
	}
	else
	{
		g_MainMenu_MatchTime = 0;
	}

#if 0
	if(GUI_VK_RED== key)
	{
		g_MainMenu_Region_MatchTime = 1;
	}
	else if(g_MainMenu_Region_MatchTime == 1 && GUI_VK_GREEN== key)
	{
		g_MainMenu_Region_MatchTime = 2;
	}
	else if(g_MainMenu_Region_MatchTime == 2 && GUI_VK_YELLOW== key)
	{
		g_MainMenu_Region_MatchTime = 3;
		
	}
	else if(g_MainMenu_Region_MatchTime == 3 && GUI_VK_BLUE== key)
	{
		GUI_ENC_API_SwitchState(ID_DLG_RegionSet,1);
	
	}
	else
	{
		g_MainMenu_Region_MatchTime = 0;
	}

#endif

	if(GUI_VK_0 == key)
	{
		g_MainMenu_StbID_MatchTime = 1;
	}
	else if(g_MainMenu_StbID_MatchTime == 1 && GUI_VK_1 == key)
	{
		g_MainMenu_StbID_MatchTime = 2;
	}
	else if (g_MainMenu_StbID_MatchTime == 2 && GUI_VK_5 == key)
	{
		g_MainMenu_StbID_MatchTime = 3;
	}
	else if(g_MainMenu_StbID_MatchTime == 3 && GUI_VK_8 == key)
	{
		APP_Com_API_StopProg(1);
		GUI_ENC_API_SwitchState(ID_DLG_StbIDSetting,1);
	}
	else
	{
		g_MainMenu_StbID_MatchTime = 0;
	}
	
	switch(key)
	{
	case GUI_VK_UP:
		if(g_key_count==1)
		{
			g_key_count=8;
		}
		else
		{
			g_key_count--;
		}
		GUI_ENC_API_Update(ID_ODC_MainMenu, 1);
		break;
	case GUI_VK_DOWN:
		if(g_key_count==8)
		{
			g_key_count=1;
		}
		else 
		{
			g_key_count++;
		}
		GUI_ENC_API_Update(ID_ODC_MainMenu, 1);
		break;
	case GUI_VK_SELECT:
		if(g_key_count==1)
		{
		//	GUI_ENC_API_SetValue(ID_MNU_ProgManager,WDT_MNU_SV_SELECT,RS_ProgManager,0);
		//	GUI_ENC_API_SetValue(ID_MNU_ProgManager,WDT_MNU_SV_INDEX,0,0);
		//	GUI_ENC_API_SwitchState(ID_DLG_ProgManager,1);
		//	GUI_ENC_API_SwitchState(ID_DLG_ProgList, 1);

			if (APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			//if(APP_SUCCESS == APP_ChnDatabase_API_CreatChnList(APP_CHN_AV_TYPE_TV_e,APP_CHN_GROUP_TYPE_ALL_e))
			{

				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
				UI_APP_API_SetChnListPreState(ID_DLG_MainMenu);
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
		}
		else if(g_key_count==2)
		{

	#if defined(USE_FZVOD) || defined(USE_ENREACH)		
			if(SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
			{
	#ifdef ENABLE_BROADCAST_IPANEL
				Ipanel_SetEnterVodOrTvod_Flag(2);
				APP_Com_API_StopProg(1);
				Ipanel_SetEntrance_Flag(FYF_TRUE);
				OW_VOD_SetDhcpFlag(FYF_TRUE);
				GUI_ENC_API_SwitchState(ID_DLG_Null,1);
	#endif
			}
	#endif
			
		}
		else if(g_key_count==3)
		{
		//	GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoService,0);
		//	GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);

#if defined(USE_FZVOD) || defined(USE_ENREACH)		
				if(SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
				{
#ifdef ENABLE_BROADCAST_IPANEL		
					Ipanel_SetEnterVodOrTvod_Flag(1);
					APP_Com_API_StopProg(1);
					Ipanel_SetEntrance_Flag(FYF_TRUE);
					OW_VOD_SetDhcpFlag(FYF_TRUE);
					GUI_ENC_API_SwitchState(ID_DLG_Null,1);
#endif				
				}
#endif

		}
		else if(g_key_count==4)
		{
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_RD_e,APP_CHN_GROUP_TYPE_ALL_e)&&
				APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_SetChnListPreState(ID_DLG_MainMenu);
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
		}
		else if(g_key_count==5)
		{
#ifndef NO_BROADCAST
			broadcastNum = APP_DBA_API_GetBroadCastNum();
			if(broadcastNum == 0)
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoBroadcast,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			/*
			else if(broadcastNum == 1)
			{
				GUI_ENC_API_SwitchState(ID_DLG_Null,1);	
			}
			*/
			else
			{
				UI_APP_API_SetBrdcastListPreState(ID_DLG_MainMenu);
				GUI_ENC_API_SwitchState(ID_DLG_BroadcastList,1);
			}
#endif
		}
		else if(g_key_count==6)
		{
			if (APP_ChnDatabase_API_GetCurTypeChnNum())
		    {
		    	UI_APP_API_SetEpgPreState(ID_DLG_MainMenu);
				GUI_ENC_API_SwitchState(ID_DLG_EPG,1);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoProg,0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
		}
		else if(g_key_count==7)
		{
			GUI_ENC_API_SetValue(ID_MNU_Ca, WDT_MNU_SV_SELECT, 0, 0);
			#ifndef NO_CA
				GUI_ENC_API_SwitchState(ID_DLG_Ca,1);
			#endif
		}
		else if(g_key_count==8)
		{   
		    GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_SELECT, RS_SystemSetting, 0);
			GUI_ENC_API_SetValue(ID_MNU_SysSet,WDT_MNU_SV_INDEX,0,0);
			GUI_ENC_API_SwitchState(ID_DLG_SysSet,1);
		}
		break;
	case GUI_VK_EXIT:
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
		break;
	default:
		break;	
	}
}


void UI_APP_MainMenu_Timer(void)
{
 	static  BU32 count = 0;
	GUI_ENC_API_SetValue(ID_TIM_MainMenu, WDT_TIM_SV_STOP, 0, 0);
 	if (++count > 1 || g_key_receive > 0)return;

	GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);

}
