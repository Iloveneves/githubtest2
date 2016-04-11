#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

extern BU08 bfirst;
extern BU08 bGuiTimerSuspend;

BS32 ErrorCode = 0;

/* record the ui null prev*/
BS32  g_ui_null_prevstat; 


void CallBackGetTableFunc(BU32 channelID, BU32 filterID, BU08* pSection, BS32 len);
#ifdef ENABLE_BROADCAST_IPANEL
extern BU08 		g_Destroy;
extern BU32 		GUI_IpanelState;
extern BU08  		socket_buf[720*576*2];
extern BU08 		bDrawAgain;
#endif


void UI_APP_Null_SavePrevstat()
{
	g_ui_null_prevstat = GUI_ENC_API_GetPrevState();
}

BS32 UI_APP_Null_GetPrevStat(void)
{
	return g_ui_null_prevstat;
}


void BroadCast_AppInit(void)
{
	
	BU08 str[] = "doto";

	APP_Com_API_ShowLED(str);

	APP_EPG_API_StopGetEpg();
	APP_EPG_API_SetRemindReserve(0); /*关预约*/
	APP_TDT_IPI_StopDemuxTdt();
	APP_Search_Set_OTA_Status(0);

//	CA_SetEnable(0);
#ifdef ENABLE_BROADCAST_IPANEL
	Inthird_SetFlag(FYF_TRUE);
#endif

	APP_Com_API_Stop_Signal_Dectect();
	APP_SDT_IPI_StopDemuxSdtOnTime();
	APP_PAT_IPI_StopDemuxPat();
	APP_PMT_IPI_StopDemuxPmtPcrEcm();
	APP_NIT_IPI_StopDemuxNit();


	/*-------------------------------------*/
	APP_Com_API_StopProg(1);
	APP_Com_API_SetEcmNULL();
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);


#if 1	
	if (1 == APP_Audio_API_GetMuteState())
	{

#ifdef ENABLE_BROADCAST_IPANEL	
		ipanel_SetAudMuteState(FYF_TRUE);
#endif

#ifdef ENABLE_BROADCAST_ENREACH
		GUI_ENC_API_SetVisible(ID_STA_MUTE_EVM, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_MUTE_EVM, 1);
#endif
		APP_Audio_API_SetMuteState(1);
	}
	else
	{
		//APP_Audio_API_SetMuteState(0);
		printf("5555555555555555555\n");

#ifdef ENABLE_BROADCAST_IPANEL
		ipanel_SetAudMuteState(FYF_FALSE);
#endif

#ifdef ENABLE_BROADCAST_ENREACH
		GUI_ENC_API_SetVisible(ID_STA_MUTE_EVM, WS_DISABLE);		
#endif

	}
#endif

#ifdef ENABLE_BROADCAST_ENREACH
		GUI_ENC_API_SetVisible(ID_VOI_EVM, WS_DISABLE);
#endif

}

void BroadCast_AppExit(void)
{

printf("================= BroadCast_AppExit ===========================\n");

	//APP_IPI_StopAudioVideo(1);
	APP_EPG_API_SetRemindReserve(1);

	//CA_SetEnable(1);
	APP_Com_API_Start_Signal_Dectect();

#ifdef ENABLE_BROADCAST_IPANEL
	Inthird_SetFlag(FYF_FALSE);
#endif	

	APP_Search_Set_OTA_Status(1);

	/*--------------------------*/
	APP_IPI_StopAudioVideo(1);
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);

	FYF_API_demux_set_notify(CallBackGetTableFunc);

	if (1 == APP_Audio_API_GetMuteState())
	{
	
#ifdef ENABLE_BROADCAST_IPANEL	
		ipanel_SetAudMuteState(FYF_TRUE);
#endif
		APP_Audio_API_SetMuteState(1);
	}
	else
	{
		APP_Audio_API_SetMuteState(0);
		UI_APP_API_SetVolume();
		
#ifdef ENABLE_BROADCAST_IPANEL
		ipanel_SetAudMuteState(FYF_FALSE);
#endif
	}
	
}
void UI_APP_Null_Enter(void)
{
	BU32 broadcastNo;
	BU32 broadcastnum = 0;
	BU32 index;
	BS32 ret  = -1;
	BroadCast_ChnInfo_S broadcastInfo;
	
	FYF_API_OSD_FillRect(0, 0, 720, 576,0);
	FYF_API_OSD_Refresh(0, 0, 720, 576);

	
	GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET_BAK, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET, WS_DISABLE);
	GUI_ENC_API_SetValue(ID_STA_NULL_CHECKNET,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

	GUI_ENC_API_SetValue(ID_TIM_NULL, WDT_TIM_SV_PLAY, 0, 0);
/*=====================================================================
		broadcast into point 
=====================================================================*/

	UI_APP_Null_SavePrevstat();


	BroadCast_AppInit();

	if (SYSTEM_GetNET_ON_OFF() == FYF_FALSE)
	{
		broadcastnum = APP_DBA_API_GetBroadCastNum();
		if(broadcastnum == 0)
		{
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			return;
		}
		else
		{
			broadcastNo = UI_APP_API_GetCurBrdcastNo();
			if(APP_FAIL == APP_DBA_API_GetBroadcastInfo(broadcastNo, &broadcastInfo))
			{
				return;
			}
		}
		
#ifdef ENABLE_BROADCAST_IPANEL		
		ret = HI_TRD_IPANEL_Init(broadcastInfo.m_FreqMHz*1000, broadcastInfo.m_SymKbps, broadcastInfo.m_QAM, broadcastInfo.m_ProgId);
#endif

#ifdef ENABLE_BROADCAST_ENREACH
		ret = HI_TRD_ENREACH_Init(broadcastInfo.m_FreqMHz, broadcastInfo.m_SymKbps, broadcastInfo.m_QAM,  broadcastInfo.m_ProgId);
#endif
		printf("############ for jlin test: HI_TRD_IPANEL_Init end, ...######\n");
		
	}
	else
	{
		printf("######### for jlin test , into signal direction broadcast##########\n");
#ifdef ENABLE_BROADCAST_IPANEL	
		if (Ipanel_GetEntrance_Flag() == FYF_FALSE)
		{
			broadcastnum = APP_DBA_API_GetBroadCastNum();
			if (broadcastnum == 0)
			{
				GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
				return;
			}
			else
			{
				broadcastNo = UI_APP_API_GetCurBrdcastNo();
				if(APP_FAIL == APP_DBA_API_GetBroadcastInfo(broadcastNo,&broadcastInfo))
				{
					return;
				}
			}
							
			ret = HI_TRD_IPANEL_Init(broadcastInfo.m_FreqMHz*1000,broadcastInfo.m_SymKbps,broadcastInfo.m_QAM, broadcastInfo.m_ProgId);
		}
		else
		{
			ret = HI_TRD_IPANEL_Init(broadcastInfo.m_FreqMHz*1000,broadcastInfo.m_SymKbps,broadcastInfo.m_QAM, broadcastInfo.m_ProgId);
		}

#endif


#ifdef ENABLE_BROADCAST_ENREACH
			broadcastnum = APP_DBA_API_GetBroadCastNum();
			if (broadcastnum == 0)
			{
				GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
				return;
			}
			else
			{
				broadcastNo = UI_APP_API_GetCurBrdcastNo();
				if(APP_FAIL == APP_DBA_API_GetBroadcastInfo(broadcastNo,&broadcastInfo))
				{
					return;
				}
			}

			HI_TRD_ENREACH_SetEnterFlag(0);
			ret = HI_TRD_ENREACH_Init(broadcastInfo.m_FreqMHz, broadcastInfo.m_SymKbps, broadcastInfo.m_QAM,  broadcastInfo.m_ProgId);
			if (ret != 0)
			{
				return;	
			}
#endif

		printf("############ for jlin test: HI_TRD_IPANEL_Init end, ...######\n");

	}
		

		/*if (ret == FYF_OK)
		{
			bfirst = 0;
			ErrorCode = ret;
		}
		else if (-2 == ret)
		{
			ErrorCode = ret;
			GUI_ENC_API_SetValue(ID_STA_NULL_CHECKNET, WDT_STA_SV_STRING, "请检查网络连接", 0);
			GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET_BAK, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET, WS_VISIBLE);
			bGuiTimerSuspend = FYF_FALSE;
		}*/
		
		ErrorCode = ret;

		switch (ret)
		{
			case FYF_OK:
				
				bfirst = 0;

				break;
			
				
			case -2:
				if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
				{
					GUI_ENC_API_SetValue(ID_STA_NULL_CHECKNET, WDT_STA_SV_STRING, "请检查网络连接", 0);
					GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET_BAK, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET, WS_VISIBLE);
				}
				
				bGuiTimerSuspend = FYF_FALSE;

				break;

			case -3:
				
				if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
				{
					GUI_ENC_API_SetValue(ID_STA_NULL_CHECKNET, WDT_STA_SV_STRING, "请检查网络", 0);
					GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET_BAK, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_NULL_CHECKNET, WS_VISIBLE);
				}
				bGuiTimerSuspend = FYF_FALSE;
				GUI_ENC_API_SetValue(ID_TIM_NULL, WDT_TIM_SV_PERIOD, 1000, 0);

				break;

			default:
				break;
		}
#if 0
		if (bDrawAgain)
		{
			GUI_API_DrawBitmap(GUI_API_GetDlgItem(ID_DLG_Null), 0, 0, 720, 576, /*(void *)buf*/(void *)socket_buf);
			bDrawAgain = 0;
			memset(socket_buf, 0, sizeof(socket_buf));
		}
#endif
}

void UI_APP_NULL_TIME(void)
{
	printf("===================UI_APP_NULL_TIME=========================\n");
	/*if (ErrorCode == -2)
	{
		FYF_API_OSD_FillRect(0, 0, 720, 576,0);
		FYF_API_OSD_Refresh(0, 0, 720, 576);
		HI_TRD_IPANEL_DeInit();
		ErrorCode = 0;
	}*/

#ifdef ENABLE_BROADCAST_IPANEL
	switch (ErrorCode)
	{
		case FYF_OK:
			break;
			
		case -2:
			FYF_API_OSD_FillRect(0, 0, 720, 576,0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);

			HI_TRD_IPANEL_DeInit();

			ErrorCode = 0;
			break;
			
		case -3:
			
			FYF_API_thread_sleep(2000);
			
			FYF_API_OSD_FillRect(0, 0, 720, 576,0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);

			HI_TRD_IPANEL_DeInit();
			ErrorCode = 0;
			break;

		default:
			break;
	}
#endif

#ifdef ENABLE_BROADCAST_ENREACH	
	HI_TRD_ENREACH_TimeWork();
#endif
}

void UI_APP_Null_End(void)
{
	printf("===================end=========================\n");

#ifdef ENABLE_BROADCAST_ENREACH
	//HI_TRD_ENREACH_Exit();
#endif

	BroadCast_AppExit();
}


void UI_APP_Null_Key(void)
{

		printf("################### UI_APP_Null_Key #################\n");

}

