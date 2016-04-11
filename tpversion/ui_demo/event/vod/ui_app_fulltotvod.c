#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


BU32 ucNET_ON_OFF = FYF_FALSE;

int SYSTEM_GetNET_ON_OFF(void)
{
	BU32 value = 0;
	return 1;
	APP_SYS_API_GetSysInfo(APP_SYS_SwitchItem_e,&value);
	if (value == 1)
		ucNET_ON_OFF = 0;
	else
		ucNET_ON_OFF = 1;
	//printf("\033[0:31m NET_ON_OFF=%d \033[0m\n",ucNET_ON_OFF);
	return ucNET_ON_OFF;
}

void SYSTEM_SetNET_ON_OFF(BU32 flag)
{
	if (flag == 1)
		ucNET_ON_OFF = 0;
	else
		ucNET_ON_OFF = 1;

	APP_SYS_API_SetSysInfo(APP_SYS_SwitchItem_e, ucNET_ON_OFF);
	printf("\033[0:31m NET_ON_OFF=%d \033[0m\n",ucNET_ON_OFF);
}

static BS32 g_fulltotvod_curState = -1;//FULLTOTVOD_NULL_STATE;

void FULLTOTVOD_SetCurState(BU32 curState)
{
	g_fulltotvod_curState = curState;

}

BU32 FULLTOTVOD_GetCurState(void)
{

	return g_fulltotvod_curState;

}

#ifdef USE_ENREACH

static BU08 bFullToTvodCount = 5;
static BU08 g_wait_flag = 0;

static BU32 g_play_timelen = 0;
static BU32 g_total_timelen = 0;
BU08 g_firststateInit = FYF_FALSE;


extern BU08 bfirst;

#ifdef ENABLE_BROADCAST_IPANEL
extern BU32 GUI_IpanelState;
#endif
extern BU08 bGuiTimerSuspend;
extern 	void CallBackGetTableFunc(BU32 channelID, BU32 filterID, BU08* pSection, BS32 len);
void FULLTOTVOD_ClearScreen(void)
{


    GUI_ENC_API_SetVisible(ID_STA_FullScreenBk_TVOD, WS_VISIBLE);
	GUI_ENC_API_Update(ID_STA_FullScreenBk_TVOD, 1);
	#ifdef ENABLE_BROADCAST_IPANEL
	if (!ipanel_get_playmove_flag())
	{
		GUI_ENC_API_SetVisible(ID_STA_BPLOGO, WS_VISIBLE);	
		GUI_ENC_API_Update(ID_STA_BPLOGO, 1);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_BPLOGO, WS_DISABLE);	
	}
	#endif
//		GUI_API_TextOut(GUI_API_GetDlgItem(ID_DLG_FULLTOTVOD), 36, 492, "电视回看", strlen("电视回看"), RGB(255,57,0)/*COLOR_WHITE*/, /*RGB(245,57,135)*/COLOR_WHITE, 1);

}



void FULLTOTVOD_IPI_ShowConfig(BU08 bExit, BU08 bTimeJumpBarShow, BU08 bHelpInfoShow, BU08 bPauseShow, BU08 bSpeedShow, BU08 bVoiBarShow, BU08 bWaitFor, BU08 bError)
{
    BU32 showState = 0;
	BU32 i = 0;
	GUI_WinID_Demo_e bTimeJumpBar[] = { ID_ODC_JUMPBAR };
	GUI_WinID_Demo_e bHelpInfo[] = {ID_STA_TVOD_HELP_BK, ID_STA_TVOD_HELP, ID_STA_TVOD_HELP_LINE1,
		                          ID_STA_TVOD_HELP_LINE2, ID_STA_TVOD_HELP_LINE3, ID_STA_TVOD_HELP_LINE4,
		                          ID_STA_TVOD_HELP_LINE5, ID_STA_TVOD_HELP_LINE6, ID_PBR_SigVoltage_FULLTOTVOD,
		                          ID_PBR_SigSNR_FULLTOTVOD, ID_STA_SigVoltage_FULLTOTVOD, ID_STA_SigSNR_FULLTOTVOD,
		                          ID_STA_Line12_FULLTOTVOD, ID_STA_Line11_FULLTOTVOD, ID_STA_Line10_FULLTOTVOD, ID_STA_Line10_FULLTOTVOD,
		                          ID_PBR_BER_FULLTOTVOD, ID_STA_BER_FULLTOTVOD};
	GUI_WinID_Demo_e bPause[] = {ID_STA_PP, ID_STA_PP_PAUSE_2, ID_STA_PP_CONTINUE_2};
	GUI_WinID_Demo_e bWait[] = {ID_STA_BAK_BPZHUN, ID_ODC_BPZHUN};
	GUI_WinID_Demo_e bDisable[] = {ID_STA_PP_EXIT, ID_STA_PP_EXIT_LINE1, ID_STA_PP_EXIT_LINE2,
							ID_STA_INFOBP, ID_STA_PP_PAUSE, ID_STA_INBP, ID_STA_PP_CONTINUE, ID_STA_PP_PLAYTIME,
							ID_STA_PLAYCHN,};
	GUI_WinID_Demo_e bExitInfo[] = {ID_STA_PP_EXIT, ID_STA_PP_EXIT_LINE1, ID_STA_PP_EXIT_LINE2, ID_STA_PP_EXIT_LINE3};

	GUI_WinID_Demo_e bErrorInfo[] = {ID_STA_PP_EXIT, ID_STA_ERROR_LINE1, ID_STA_ERROR_LINE2};


#ifdef ENABLE_BROADCAST_IPANEL
	if (ipanel_get_playmove_flag())
	{
		GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE1, WDT_STA_SV_STRING, "是否退出点播状态", 0);
		GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE2, WDT_STA_SV_DATAID, RS_PP_ExitVodSelect, 0);
		GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE3, WDT_STA_SV_DATAID, RS_PP_ExitVodReturn, 0);
		GUI_ENC_API_SetValue(ID_STA_PP_PAUSE_2, WDT_STA_SV_STRING, "点播暂停", 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE1, WDT_STA_SV_STRING, "是否退出回看状态", 0);
		GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE2, WDT_STA_SV_DATAID, RS_PP_ExitSelect, 0);
		GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE3, WDT_STA_SV_DATAID, RS_PP_ExitReturn, 0);
		GUI_ENC_API_SetValue(ID_STA_PP_PAUSE_2, WDT_STA_SV_DATAID, RS_PP_Pause, 0);
	}
#endif
	
	if(bExit)
	{
	   showState = WS_VISIBLE;
	}
	else
	{
	   showState = WS_DISABLE;
	}
	if (bExit || bError)
	{
		GUI_ENC_API_SetVisible(ID_STA_PP_EXIT,WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_PP_EXIT,1);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_PP_EXIT,WS_DISABLE);
	}
	for(i = 1; i< 4; i++)
	{
	    GUI_ENC_API_SetVisible(bExitInfo[i],showState);
		if(showState)
		{
		     GUI_ENC_API_Update(bExitInfo[i],1);
		}
	}
	if(bError)
	{
	   showState = WS_VISIBLE;
	}
	else
	{
	   showState = WS_DISABLE;
	}
	for(i = 1; i< 3; i++)
	{
	    GUI_ENC_API_SetVisible(bErrorInfo[i],showState);
		if(showState)
		{
		     GUI_ENC_API_Update(bErrorInfo[i],1);
		}
	}
	if(bTimeJumpBarShow)
	{
	   showState = WS_VISIBLE;
	}
	else
	{
	   showState = WS_DISABLE;
	}
	for(i = 0; i< 1; i++)
	{
	    GUI_ENC_API_SetVisible(bTimeJumpBar[i],showState);
		if(showState)
		{
		     GUI_ENC_API_Update(bTimeJumpBar[i],1);
		}
	}

	if(bHelpInfoShow)
	{
	   showState = WS_VISIBLE;
	}
	else
	{
	   showState = WS_DISABLE;
	}
	for(i = 0; i< 18; i++)
	{
	    GUI_ENC_API_SetVisible(bHelpInfo[i],showState);
		if(showState)
		{
		     GUI_ENC_API_Update(bHelpInfo[i],1);
		}
	}

	if(bPauseShow)
	{
	   showState = WS_VISIBLE;
	}
	else
	{
	   showState = WS_DISABLE;
	}
	for(i = 0; i< 3; i++)
	{
	    GUI_ENC_API_SetVisible(bPause[i],showState);
		if(showState)
		{
		     GUI_ENC_API_Update(bPause[i],1);
		}
	}

	if(bSpeedShow)
	{
	    GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_VISIBLE);
		GUI_ENC_API_Update(ID_ODC_FAST,2);
	}
	else
	{
		 GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_DISABLE);
	}
	
    if(bVoiBarShow)
    {
	    GUI_ENC_API_SetVisible(ID_VOI_TVOD, WS_VISIBLE);
		GUI_ENC_API_Update(ID_VOI_TVOD,1);
    }
	else
	{
	    GUI_ENC_API_SetVisible(ID_VOI_TVOD, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_VOI_TVOD, WS_DISABLE);
	}

	if(bWaitFor)
	{
	   showState = WS_VISIBLE;
	}
	else
	{
	   showState = WS_DISABLE;
	}
	for(i = 0; i< 2; i++)
	{
	    GUI_ENC_API_SetVisible(bWait[i],showState);
		if(showState)
		{
		     GUI_ENC_API_Update(bWait[i],1);
		}
	}

	GUI_ENC_API_Update(ID_STA_TVOD_MUTE ,1);
//cwd add
 	for(i = 0; i< 9; i++)
	{
	    GUI_ENC_API_SetVisible(bDisable[i],WS_DISABLE);

		 GUI_ENC_API_Update(bDisable[i],1);
	
	}
	
}

void FULLTOTVOD_IPI_SetCurState(BU32 curState)
{
    FULLTOTVOD_SetCurState(curState);
	switch(curState)
	{
	case FULLTOTVOD_NULL_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(0,0,0,0,0,0,0,0);
		break;
	case PP_EXIT_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(1,0,0,0,0,0,0,0);
		break;
	case TVOD_JUMPBAR_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(0,1,0,0,0,0,0,0);
		break;
	case TVOD_HELP_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(0,0,1,0,0,0,0,0);
		break;
	case TVOD_PAUSE_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(0,0,0,1,0,0,0,0);
		break;
	case FAST_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(0,0,0,0,1,0,0,0);
		break;
	case TVOD_VOL_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(0,0,0,0,0,1,0,0);
		break;
	case BPZHUN_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(0,0,0,0,0,0,1,0);
		break;
	case TVOD_ERROR_STATE:
		FULLTOTVOD_ClearScreen();
		FULLTOTVOD_IPI_ShowConfig(0,0,0,0,0,0,0,1);
		break;
	default:
		break;
	}
}

void FullTOTVOD_IPI_Exit(void)
{
	FYF_QUEUE_MESSAGE Msg;

#ifdef USE_ENREACH
	envod_set_exit(0);
	APP_EnrichVod_Stop();
#endif

	FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
	g_fulltotvod_curState = -1;

#ifdef USE_ENREACH	
	APP_EnrichVod_DeInit();
#endif

	APP_Com_API_StopProg(1);

#ifdef ENABLE_BROADCAST_IPANEL
	Msg.qlstWordOfMsg= (unsigned int)APP_Queue_Ipannel_Start;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &Msg);	
	Msg.qlstWordOfMsg = 0;	
	FYF_API_queue_recv(APP_IPI_GetQueueIpanel(), &Msg, 5000);
	if (Msg.qlstWordOfMsg == 1)
	{
		printf("APP_IPI_GetQueueIpanel success\n!");
	}
	else
	{
		printf("APP_IPI_GetQueueIpanel error!\n");
	}


	g_wait_flag = 0;
	
	if (GUI_IpanelState == 0)
	{
		BroadCast_AppExit();
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
	else
	{
		GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
		bfirst = 0;
		FYF_API_demux_set_notify(NULL);
		bGuiTimerSuspend = FYF_FALSE;
	}
#endif

}


void UI_APP_BPZHUN_OnPaint(BU32 hdc,BU32 lParam)
{

	BU08 showStr[100]={0};
	char ipStr[20] = { 0 };
	char portStr[10] = { 0 };
	int m_s = -1;
	
	printf("lparam = %d\n", lParam);
	sprintf(showStr,"回放准备%d秒 按任意键退出",bFullToTvodCount);
	GUI_API_DrawStyle(hdc, 0, 0, STYLE_BPZHUN_W276_H30);
	GUI_API_TextOut(hdc, 10, 2, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);

}


void UI_APP_BPZHUN_ODC_Key(BU32 key)
{

	printf("Press Key.==%d===============UI_APP_BPZHUN_ODC_Key\n",key);
#ifdef ENABLE_BROADCAST_IPANEL
	if (GUI_IpanelState == 0)
	{
		BroadCast_AppExit();
	//	GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
#endif
	g_fulltotvod_curState = -1;
	g_wait_flag = 0;
	GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
#if 0
	
	if (GUI_ENC_API_IsFocus(ID_ODC_BPZHUN))
	{
		/*if (key && (bFullToTvodCount != 0))
		{
			OW_SetExitFlag(FYF_TRUE);
		}
		*/
	}
#endif
						
}

void UI_APP_BPZHUN_TIME(void)
{

	
	if (bFullToTvodCount > 0)
	{		
		bFullToTvodCount--;
		GUI_ENC_API_Update(ID_ODC_BPZHUN, bFullToTvodCount);
	}

	if (bFullToTvodCount == 0 && g_wait_flag == 0)
	{
		g_wait_flag = 1;
		GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_STOP,0,0);
		if (APP_FAIL == APP_EnrichVod_UserVaild())
		{
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "  服务未授权!", 0);
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "详情请咨询: 968839", 0);
			FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
			FYF_API_thread_sleep(2000);
			FullTOTVOD_IPI_Exit();
			return;
		}
		
		if  (APP_FAIL == APP_EnrichVod_Init())
		{
		//	APP_EnrichVod_Init();
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "连接不上服务器", 0);
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "即将退出回看", 0);
			FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
			FYF_API_thread_sleep(2000);
			FullTOTVOD_IPI_Exit();
			return;
			
		}
		GUI_ENC_API_SetFocus(ID_ODC_FAST);
		APP_EnrichVod_PlayTimeShift();
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
#if 0
		sprintf(host,"%s",np_homepage1);
		if((ret = evtc_init(host, &INITPARA)) == envod_fail)
		{
			APP_PRINT_ERROR(" evtc_init err!! \n");
		}
		else
		{
			APP_PRINT_DEBUG("result = %d, %d, %d, %d, %d, %d, %d, %s\n", INITPARA.result, INITPARA.mainfreq,\
				INITPARA.maintsid, INITPARA.maintsid, INITPARA.sndfreq, INITPARA.sndtsid, INITPARA.hearttime, INITPARA.serverIP);
		}

		if ((ret = evtc_relay(host, Timeshiftlist[evtc_get_curtsindex()].freq, Timeshiftlist[evtc_get_curtsindex()].serviceid, &RELAYPARA)) == envod_fail)
		{
			APP_PRINT_ERROR(" evtc_relay err!! \n");
		}
		else
		{
			APP_PRINT_DEBUG("RELAYPARA playmod= %d, feq = %d, qam = %d, symbolrate = %d, vpid = %d, apid = %d, pcrpid = %d, pmtpid = %d, begintime = %s, endtime = %s, curtime = %s, prevprog = %s, curprog = %s ,nexprog = %s\n", RELAYPARA.playmod, RELAYPARA.freq, RELAYPARA.qam, RELAYPARA.symbolrate, \
						RELAYPARA.vpid, RELAYPARA.apid, RELAYPARA.pcrpid, RELAYPARA.pmtpid, RELAYPARA.begintime, RELAYPARA.endtime, RELAYPARA.curtime, RELAYPARA.preprg, RELAYPARA.curprg,RELAYPARA.nextprg);
		}
		APP_Com_API_StopProg(0);
		FYF_API_thread_sleep(200);
		FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
		APP_IPI_SaveTunerInfo(RELAYPARA.freq / 1000, RELAYPARA.symbolrate, RELAYPARA.qam - 1);
		FYF_API_tuner_lock_delivery(0, RELAYPARA.freq, RELAYPARA.symbolrate, RELAYPARA.qam - 1, APP_SYS_IPI_GetMainFreq());
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
	

		tunerstate = FYF_TUNER_STATUS_FAIL;
		for(i =0 ;i < 40 ;i++)
		{
			if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
			{
				tunerstate = FYF_TUNER_STATUS_SUCCESS;
				break;
			}

			FYF_API_thread_sleep(50);
		}
		APP_IPI_PlayProg(RELAYPARA.vpid, RELAYPARA.apid, RELAYPARA.pcrpid, FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
#endif
	}
}


void UI_APP_FAST_OnPaint(BU32 hdc,BU32 lparam)
{
	BU08 showStr[100]={0};
	APP_PRINT_DEBUG("UI_APP_FAST_OnPaint lparam = %d\n", lparam);
	if (lparam == 2)
	{
		if (APP_EnrichVod_GetSpeedFlag() == 2)
		{
			sprintf(showStr,"快退-%2d", APP_EnrichVod_GetCurSpeed());
		}
		else if (APP_EnrichVod_GetSpeedFlag() == 1)
		{
			sprintf(showStr,"快进x%2d", APP_EnrichVod_GetCurSpeed());
		}
		APP_PRINT_INFO("showStr = %s\n", showStr);
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_FAST_W100_H24);
		GUI_API_TextOut(hdc, 10, 2, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);
	}

}

static EnrichVod_DateTime_S cur_datetime;
void UI_APP_JUMPBAR_OnPaint(BU32 hdc,BU32 lparam)
{
	BU08 showStr[100]={0};
	BU08 showCurProgNameAndTime[200];
	BU08 curProgName[30];
	EnrichVod_DateTime_S curProgStartTime;
	EnrichVod_DateTime_S curProgEndTime;
	
	BU32 postion; 
	if (lparam == 1)
	{
		if (APP_SUCCESS == APP_EnrichVod_GetPlayTime(&cur_datetime))
		{

			sprintf(showStr,"%04d-%02d-%02d %02d:%02d:%02d",cur_datetime.year, cur_datetime.month, cur_datetime.date, cur_datetime.hour,\
				cur_datetime.minute, cur_datetime.second);
		}
		postion = APP_EnrichVod_GetPBRPosition();
		g_play_timelen = APP_EnrichVod_GetPlayTimeLength();
		g_total_timelen = APP_EnrichVod_GetTotalTimeLength();
	}
	else if (lparam == 2)
	{
		if (g_total_timelen == 0) return;
		APP_EnrichVod_PlayTimeAdd(&cur_datetime, &g_play_timelen);
		postion = ((g_play_timelen )*101) / g_total_timelen;
	}
	else if (lparam == 3)
	{
		if (g_total_timelen == 0) return;
		APP_EnrichVod_PlayTimeSub(&cur_datetime, &g_play_timelen);
		postion = ((g_play_timelen )*101) / g_total_timelen;
	}
	
	sprintf(showStr,"%04d-%02d-%02d %02d:%02d:%02d",cur_datetime.year, cur_datetime.month, cur_datetime.date, cur_datetime.hour,\
			cur_datetime.minute, cur_datetime.second);
	APP_EnrichVod_GetCurProgStartTime(&curProgStartTime);
	APP_EnrichVod_GetCurProgEndTime(&curProgEndTime);
	APP_EnrichVod_GetProgAttr(APP_EnrichVod_ATTR_CurProg_Type, curProgName);
	curProgName[20] = 0;
	sprintf(showCurProgNameAndTime, "%02d:%02d:%02d      %-20s   %02d:%02d:%02d",\
		curProgStartTime.hour, curProgStartTime.minute, curProgStartTime.second,\
		curProgName, curProgEndTime.hour, curProgEndTime.minute, curProgEndTime.second);
	//画背景
	GUI_API_DrawStyle(hdc, 0, 0, STYLE_Jump2_W500_H120);
	GUI_API_TextOut(hdc, 30, 12, showCurProgNameAndTime, sizeof(showCurProgNameAndTime), COLOR_BLACK, COLOR_BLACK, 1);
	//画光标的位置
	GUI_API_DrawStyle(hdc, 38 + postion * 4, 30, STYLE_VOD_SEEK_W20_H24);
	GUI_API_FillRect(hdc, 47, 56, 404, 14, COLOR_WHITE);
	GUI_API_FillRect(hdc, 47, 56, postion * 4, 14, COLOR_BARBK);
	GUI_API_TextOut(hdc, 130, 70, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);
	
}

void UI_APP_JUMPBAR_key(BU32 key)
{
	switch(key)
	{
    case GUI_VK_P_UP:
	case GUI_VK_RIGHT:
		
		GUI_ENC_API_Update(ID_ODC_JUMPBAR, 2);
	
		break;
	case GUI_VK_P_DOWN:
	case GUI_VK_LEFT:
		GUI_ENC_API_Update(ID_ODC_JUMPBAR, 3);

		break;
	
	case GUI_VK_SELECT:
		if (APP_FAIL == APP_EnrichVod_SetShiftTime(&cur_datetime))
		{
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "选时超出直播点", 0);
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "", 0);
			FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
			FYF_API_thread_sleep(1000);
		}
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		GUI_ENC_API_SetFocus(ID_ODC_FAST);
		
		break;
	case GUI_VK_EXIT:
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		GUI_ENC_API_SetFocus(ID_ODC_FAST);
		break;
    default:
		break;

	}
}

void UI_APP_FULLTOTVOD_Timer(void)
{
	APP_PRINT_DEBUG("wUI_APP_FULLTOTVOD_Timer\n");
	BU32 curState;
	GUI_ENC_API_SetValue(ID_TIM_TVOD, WDT_TIM_SV_STOP, 0, 0);
	curState = FULLTOTVOD_GetCurState();
	switch(curState)
	{
	case TVOD_VOL_STATE:
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		break;
	default:
		break;
	}
}


static BU08 g_tvod_signal_info[3][80];
void UI_APP_FULLTOTVOD_SignalCheck_Timer(void)
{

	static BU32 s_lever=0,s_snr=1,s_ber=0;
	BU32 lever;
	BU32 snr;
	BU32 ber,berTempA,berTempB,berTempC;

	if(APP_SIGNAL_LOCK == APP_Signal_API_GetSignalState())		
	{
		GUI_ENC_API_SetValue(ID_PBR_SigVoltage_FULLTOTVOD,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_SigSNR_FULLTOTVOD,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_BER_FULLTOTVOD,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		lever = (BU32)APP_Signal_API_GetSignalLever();
		snr = (BU32)APP_Signal_API_GetSignalSNR();	
		ber = APP_Signal_API_GetSignalBER();
	}
	else
	{
		GUI_ENC_API_SetValue(ID_PBR_SigVoltage_FULLTOTVOD,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_SigSNR_FULLTOTVOD,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_BER_FULLTOTVOD,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		lever = 24;
		snr = 0;
		ber = 0x09630008;
	}
	
	if(s_lever != lever)
	{
		s_lever = lever;
		sprintf(g_tvod_signal_info[0],"%ddbuV",lever);
		GUI_ENC_API_SetValue(ID_STA_Line10_FULLTOTVOD,WDT_STA_SV_STRING,(BU32)g_tvod_signal_info[0],0);
		lever = lever*200/103;
		GUI_ENC_API_SetValue(ID_PBR_SigVoltage_FULLTOTVOD,WDT_PBR_SV_CHANGE,(BU32)lever,0);

	}

	if(s_snr != snr)
	{
		s_snr = snr;
		sprintf(g_tvod_signal_info[1],"%ddB",snr);
		GUI_ENC_API_SetValue(ID_STA_Line11_FULLTOTVOD,WDT_STA_SV_STRING,(BU32)g_tvod_signal_info[1],0);
		snr = snr*200/40;
		GUI_ENC_API_SetValue(ID_PBR_SigSNR_FULLTOTVOD,WDT_PBR_SV_CHANGE,(BU32)snr,0);
	}
	
	if(s_ber != ber)
	{
		s_ber = ber;
		berTempA = ber&0xff000000;
		berTempA = berTempA>>24;
		berTempB = ber&0x00ff0000;
		berTempB = berTempB>>16;
		berTempC = ber&0x000000ff;
		sprintf(g_tvod_signal_info[2],"%d.%02dE-%d",berTempA,berTempB,berTempC + 1);
		if(berTempC>20)berTempC = 20;
		GUI_ENC_API_SetValue(ID_STA_Line12_FULLTOTVOD,WDT_STA_SV_STRING,(BU32)g_tvod_signal_info[2],0);
		GUI_ENC_API_SetValue(ID_PBR_BER_FULLTOTVOD,WDT_PBR_SV_CHANGE,(20-berTempC)*10,0);

	}
	
}

void UI_APP_TVOD_CLEANSCREEN_TIME(void)
{
	GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_STOP, 0, 0);
	if (APP_FAIL == APP_EnrichVod_UserVaild())
	{
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "  服务未授权!", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "详情请咨询: 968839", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		FYF_API_thread_sleep(2000);
		FullTOTVOD_IPI_Exit();
		return;
	}
	if (APP_FAIL == APP_EnrichVod_Init())
	{
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "连接不上服务器", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "即将退出", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		FYF_API_thread_sleep(2000);
		FullTOTVOD_IPI_Exit();
		return;
	}
	GUI_ENC_API_SetFocus(ID_ODC_FAST);
	FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
	APP_EnrichVod_PlayVod(0);
}

#endif

void UI_APP_FULLTOTVOD_Enter(void)
{	
	BU32 index;
	APP_PRINT_DEBUG("FULLTOTVOD_GetCurState = %d", FULLTOTVOD_GetCurState())
	GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);

	if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
	{
		GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
		APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
	}
	else
	{
		GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
		GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
		APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
		UI_APP_API_SetVolume();
	}
	
	//设置音量控件的音量与直播音量同步
	APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &index);
	GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_RIGHT, index, 0);
#ifdef USE_ENREACH
	envod_set_exit(0);
	
	//是否从浏览器进入时移
	if (GUI_IpanelState == 0)
	{
		
		BroadCast_AppInit();

	}
	else
	{
		bfirst = 1;
		GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
		FYF_API_demux_set_notify(CallBackGetTableFunc);
		bGuiTimerSuspend = FYF_FALSE;
	}
	
	
	if (FULLTOTVOD_GetCurState() == BPZHUN_STATE)
	{

		bFullToTvodCount = 5;
		GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);
		GUI_ENC_API_SetFocus(ID_ODC_BPZHUN);
		FULLTOTVOD_IPI_SetCurState(BPZHUN_STATE);
	}
	else
	{
		APP_Com_API_SetFullScreen();
		GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_PLAY,0,0);
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);

	}
#endif

}


void UI_APP_FULLTOTVOD_End(void)
{

}


void UI_APP_FULLTOTVOD_key(BU32 key)
{
	
#ifdef USE_ENREACH
	BU32 curstate;
	int preReqScale = 0;
	int CurSecs = 0;
	char adjTimeStr[30] = { 0 };
	int startSecs = 0;
	int endSecs = 0;
	char hourStr[3] = { 0 };
	char minStr[3] = { 0 };
	char secStr[3] = { 0 };
	BU08 hour = 0;
	BU08 min = 0;
	BU08 sec = 0;
	int oneBarAllSec = 18000;			//整条时间5小时
	BU16 barLen = 400;					//进度条总像素
	BU16 startYear = 0;
	BU16 endYear = 0;
	BU16 curYear = 0;
	BU08 startMonth = 0;
	BU08 endMonth = 0;
	BU08 curMonth = 0;
	BU08 startDay = 0;
	BU08 endDay = 0;
	BU08 curDay = 0;
	int tmp;
	char yearStr[5] = { 0 };
	char monthStr[3] = { 0 };
	char dayStr[3] = { 0 };
	char tempChr[2] = { 0 };
	BU32 index;
	APP_Date_S DateAndTime;
	BS32 ret;
	BU32 tunerstate;
	BS32 i;
	BU32 voiIndex;
	BU08 prog_name[30];
	curstate = FULLTOTVOD_GetCurState();

	if (bfirst == 0)
	{
		APP_PRINT_ERROR("UI_APP_FULLTOTVOD_key key = %d\n", key);
		return;
	}

	if (!APP_EnrichVod_GetNetStatus())
	{
		if (key == GUI_VK_EXIT)
		{
			FullTOTVOD_IPI_Exit();
		}
		return;
	}
	
	switch(key)
	{
	case GUI_VK_SELECT:
		if (curstate == FULLTOTVOD_NULL_STATE || curstate == TVOD_VOL_STATE)
		{
			FULLTOTVOD_IPI_SetCurState(TVOD_PAUSE_STATE);
			APP_EnrichVod_Pause();
		}
		else if (curstate == TVOD_PAUSE_STATE || curstate == FAST_STATE)
		{
			FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
			APP_EnrichVod_Resume();
		}
		else if (curstate == TVOD_HELP_STATE)
		{
			FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		}
		else if (curstate == PP_EXIT_STATE)
		{
			FullTOTVOD_IPI_Exit();
		}
		
		break;
	case GUI_VK_LEFT:
		if (curstate == FULLTOTVOD_NULL_STATE)
		{
			APP_EnrichVod_Rewind();
			FULLTOTVOD_IPI_SetCurState(FAST_STATE);
		
		}
		else if (curstate == FAST_STATE)
		{
			if (APP_EnrichVod_GetSpeedFlag() == 2)
			{
				APP_EnrichVod_Rewind();
				GUI_ENC_API_Update(ID_ODC_FAST, 2);
			}
		}
		break;
	case GUI_VK_RIGHT:
		if (curstate == FULLTOTVOD_NULL_STATE)
		{
			APP_EnrichVod_Speed();
			FULLTOTVOD_IPI_SetCurState(FAST_STATE);
			
		}
		else if (curstate == FAST_STATE)
		{
			if (APP_EnrichVod_GetSpeedFlag() == 1)
			{
				APP_EnrichVod_Speed();
				GUI_ENC_API_Update(ID_ODC_FAST, 2);
			}
		}
		break;
		
	case GUI_VK_RED:
		if (curstate == FULLTOTVOD_NULL_STATE || curstate == TVOD_VOL_STATE)
		{
			FULLTOTVOD_IPI_SetCurState(TVOD_HELP_STATE);
		}
		break;
	case GUI_VK_P_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_YELLOW:
		//if (GUI_IpanelState != 0)
		//	return;

		// 点播不做处理
		if (ipanel_get_playmove_flag())
		{
			return;
		}
		if(curstate == FULLTOTVOD_NULL_STATE)
		{
			FULLTOTVOD_IPI_SetCurState(TVOD_JUMPBAR_STATE);
			GUI_ENC_API_SetFocus(ID_ODC_JUMPBAR);
		}

		break;
	case GUI_VK_V_DOWN:
	case GUI_VK_V_UP:
		if(curstate == FULLTOTVOD_NULL_STATE || curstate == TVOD_VOL_STATE)
		{
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK, 1);
				GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}
		    if(curstate != TVOD_VOL_STATE)
		    {
			   FULLTOTVOD_IPI_SetCurState(TVOD_VOL_STATE);
		    }
			GUI_ENC_API_SetValue(ID_TIM_TVOD, WDT_TIM_SV_PLAY, 0, 0);
			GUI_ENC_API_SetValueUnPaint(ID_VOI_TVOD, WDT_VOI_SV_KEY, key, 0);
	        GUI_ENC_API_GetValue(ID_VOI_TVOD,WDT_VOI_SV_INDEX,(BU32 *)&voiIndex, 0);
			if (voiIndex == 0)
			{
				GUI_ENC_API_SetVisible(ID_STA_VOI_TVOD, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOI_TVOD, 1);
			}
			else
			{
				GUI_ENC_API_Update(ID_VOI_TVOD, 1);
			}

			APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, voiIndex);
			UI_APP_API_SetVolume();
		}
		break;
	case GUI_VK_UP:
		if (GUI_IpanelState != 0)
			return;
		if(curstate == FULLTOTVOD_NULL_STATE || curstate == TVOD_VOL_STATE)
		{
			APP_EnrichVod_GetProgAttr(APP_EnrichVod_ATTR_PrevProg_TYPE, prog_name);
			prog_name[20] = 0;
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "即将播放的节目是:", 0);
			GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, (BU32)prog_name, 0);
			FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
			
			if (APP_FAIL == APP_EnrichVod_PlayPrev())
			{
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "服务器忙，请稍后再试", 0);
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "", 0);
				FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
			}
			FYF_API_thread_sleep(1000);
			FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		}
		break;
	case GUI_VK_DOWN:
		if (GUI_IpanelState != 0)
			return;
		if(curstate == FULLTOTVOD_NULL_STATE || curstate == TVOD_VOL_STATE)
		{
			if (APP_FAIL == APP_EnrichVod_BeNext())
			{
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "下一个节目超出直播点", 0);
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "将继续播放当前节目", 0);
				FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
				//APP_EnrichVod_PlayPrev();
				FYF_API_thread_sleep(2000);
				FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
			}
			else
			{	
				APP_EnrichVod_GetProgAttr(APP_EnrichVod_ATTR_NextProg_Type, prog_name);
				prog_name[20] = 0;
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "即将播放的节目是:", 0);
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, (BU32)prog_name, 0);
				FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
				if (APP_FAIL == APP_EnrichVod_PlayNext())
				{
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "服务器忙，请稍后再试", 0);
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "", 0);
					FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
				}
				FYF_API_thread_sleep(1000);
				FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
			}
		}
		break;
	case GUI_VK_EXIT:
		if (curstate == TVOD_HELP_STATE || curstate == TVOD_VOL_STATE || curstate == PP_EXIT_STATE)
		{
			FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		}
		else if (curstate == FULLTOTVOD_NULL_STATE)
		{
			FULLTOTVOD_IPI_SetCurState(PP_EXIT_STATE);
		}
		break;
	case GUI_VK_MUTE:
		if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
		{
			GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
			GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
			APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			UI_APP_API_SetVolume();
		}
		else
		{
			GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
			APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
		}
		break;
	default:
		break;
	}

	return;
	
#if 0	
	GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_PERIOD, 10000, 0);
	GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_REPLAY, 0, 0);

	curstate = FULLTOTVOD_GetCurState();
//	printf("Press Key.==%d= curstate = %d==bNetError = %d============UI_APP_FULLTOTVOD_key\n", key, curstate, bNetError);
	if (curstate != TVOD_NOKEY_PRESS_TIMEOUT_STATE)
	{
		noKeytimeCount = 0;
	}
	
	if ((curstate != BPJUMP_STATE) && (curstate != BPZHUN_STATE))
	{	
		GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PERIOD, 5000, 0);
		GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_REPLAY, 0, 0);
	}

	if (bNetError || bTurnerError)
	{
		switch (key)
		{
			case GUI_VK_EXIT:
				OW_SetExitFlag(FYF_TRUE);
				break;
			default:
				break;
		}
		return;
	}
	if (OW_VOD_GetNetErrFlag() == FYF_TRUE)
	{
		switch (key)
		{
			case GUI_VK_EXIT:
				OW_SetExitFlag(FYF_TRUE);
				//OW_VOD_SetNetErrFlag(FYF_FALSE);
				break;

				
			case GUI_VK_V_DOWN:
			case GUI_VK_V_UP:
				
				GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_PERIOD, 3000, 0);
				GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_REPLAY, 0, 0);
				
				GUI_ENC_API_SetVisible(ID_VOI_TVOD, WS_VISIBLE);
				GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
				GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
				GUI_ENC_API_SetValueUnPaint(ID_VOI_TVOD, WDT_VOI_SV_KEY, key, 0);
				GUI_ENC_API_GetValue(ID_VOI_TVOD, WDT_VOI_SV_INDEX, &index, 0);
				if (index == 0)
				{
					printf("Mute.\n");
					GUI_ENC_API_Update(ID_VOI_TVOD, 1);
					APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
				}
				else
				{
					GUI_ENC_API_Update(ID_VOI_TVOD, 1);
					
					if((APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState()) && (key == GUI_VK_V_UP))
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
						//APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
						//UI_APP_API_SetVolume();
					}
				
				}
				printf("index = %d\n", index);
				//APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
				//UI_APP_API_SetVolume();
				FULLTOTVOD_SetCurState(TVOD_VOL_STATE);
				
				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					printf("APP_AUDIO_MUTE_STATE==============\n");
					GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
					GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
					GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
					APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
				}
				else
				{
					APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
					UI_APP_API_SetVolume();
				}
				break;
				
			case GUI_VK_P_DOWN:
			case GUI_VK_P_UP:
			case GUI_VK_LEFT:
			case GUI_VK_RIGHT:
			case GUI_VK_SELECT:
			case GUI_VK_F10:
			case GUI_VK_YELLOW:	

					if (bCableDownCount < CABLEDOWN)
					{
						printf("tvod, 服务器忙，请稍后再试\n");
						GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "服务器忙，请稍后再试", 0);
						GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
						GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
						
						FYF_API_thread_sleep(2000);
						
						FULLTOTVOD_ClearScreen();
					}
					else
					{
						printf("tvod, 网络不通，请退出回看\n");
						GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "网络不通，请退出回看", 0);
						GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
						GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
					}

				break;
			default:
				break;
		}
		return;
	}
#endif
	switch (curstate)
	{
		case TVOD_VOL_STATE:
			//UI_APP_TVOD_VOL_ODC_Key(key);
			break;

		case TVOD_NOKEY_PRESS_TIMEOUT_STATE:
			break;
			
		case TVOD_HELP_STATE:
			switch(key)
			{
				case GUI_VK_EXIT:
				case GUI_VK_SELECT:
				case GUI_VK_RECALL:
					FULLTOTVOD_SetCurState(FULLTOTVOD_NULL_STATE);
					break;
				default:
					break;
			}
			break;

		case FAST_STATE:
			if (GUI_ENC_API_IsFocus(ID_ODC_FAST))
			{
				//UI_APP_FAST_ODC_Key(key);
			}
			break;		
			
		case BPJUMP_STATE:
		
			break;
			
		case BPZHUN_STATE:
			if (GUI_ENC_API_IsFocus(ID_ODC_BPZHUN))
			{
				//UI_APP_BPZHUN_ODC_Key(key);
			}
			break;
			
		case PP_EXIT_STATE:
			switch (key)
			{
				case GUI_VK_EXIT:
					FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
					GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
					
					FULLTOTVOD_ClearScreen();
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					break;
				case GUI_VK_SELECT:
				
					break;
				default:
					break;
			}
			break;
			
		case TVOD_PLAY_STATE:
			
			switch (key)
			{
				case GUI_VK_F9:
				case GUI_VK_RED: 
					
					FULLTOTVOD_ClearScreen();

					
					GUI_ENC_API_SetValue(ID_STA_TVOD_HELP, WDT_STA_SV_STRING, "帮  助", 0);
					GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE1, WDT_STA_SV_STRING, "[确认]键            暂停/继续", 0);
					GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE2, WDT_STA_SV_STRING, "[退出]键            退出回看", 0);
					GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE3, WDT_STA_SV_STRING, "[黄]键              选时回看", 0);
					GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE4, WDT_STA_SV_STRING, "[返回]键            跳到回看起点", 0);
					GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE5, WDT_STA_SV_STRING, "[红]键              帮助信息", 0);
					GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE6, WDT_STA_SV_STRING, "[<-]/[->]键         倍数快进/快退", 0);
					//GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE7, WDT_STA_SV_STRING, "[上翻页]/[下翻页]   进度条", 0);

					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_BK, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE1, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE2, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE3, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE4, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE5, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE6, WS_VISIBLE);
					//GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE7, WS_VISIBLE);
					
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_BK, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE1, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE2, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE3, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE4, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE5, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE6, 1);
					//GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE7, 1);
					//GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
					FULLTOTVOD_SetCurState(TVOD_HELP_STATE);
					break;
					
				case GUI_VK_RECALL:

					FULLTOTVOD_ClearScreen();

					GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "从回看起点开始播放", 0);
					GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);

					FYF_API_thread_sleep(3000);

					FULLTOTVOD_ClearScreen();
				

					break;			
				case GUI_VK_MUTE:
				
					FULLTOTVOD_ClearScreen();
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
						UI_APP_API_SetVolume();
					}
					else
					{
						FULLTOTVOD_ClearScreen();
						
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);

						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					FULLTOTVOD_SetCurState(TVOD_VOL_STATE);
					break;
					
				case GUI_VK_V_UP:
				case GUI_VK_V_DOWN:

					GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_PERIOD, 3000, 0);
					GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_REPLAY, 0, 0);

					GUI_ENC_API_SetVisible(ID_VOI_TVOD, WS_VISIBLE);
					GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
					GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
					GUI_ENC_API_SetValueUnPaint(ID_VOI_TVOD, WDT_VOI_SV_KEY, key, 0);
					GUI_ENC_API_GetValue(ID_VOI_TVOD, WDT_VOI_SV_INDEX, &index, 0);
					if (index == 0)
					{
						printf("Mute.\n");
						GUI_ENC_API_Update(ID_VOI_TVOD, 1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					else
					{
						GUI_ENC_API_Update(ID_VOI_TVOD, 1);
						
						if((APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState()) && (key == GUI_VK_V_UP))
						{
							GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
							GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
							APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
							//APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
							//UI_APP_API_SetVolume();
						}

					}
					printf("index = %d\n", index);
					//APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
					//UI_APP_API_SetVolume();
					FULLTOTVOD_SetCurState(TVOD_VOL_STATE);
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						printf("APP_AUDIO_MUTE_STATE==============\n");
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					else
					{
						APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
						UI_APP_API_SetVolume();
					}
					
					break;

			
				case GUI_VK_LEFT:

					FULLTOTVOD_ClearScreen();
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					
				
					
					break;
					
				case GUI_VK_RIGHT:
					
					FULLTOTVOD_ClearScreen();
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}

					//GUI_ENC_API_SetVisible(ID_STA_FAST, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_VISIBLE);	
					//GUI_ENC_API_Update(ID_STA_FAST, 1);
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
					GUI_ENC_API_SetFocus(ID_ODC_FAST);
					FULLTOTVOD_SetCurState(FAST_STATE);
					
					break;
				
				case GUI_VK_EXIT:
					printf("in  TVOD_PLAY_STATE == > GUI_VK_EXIT.\n");
					FULLTOTVOD_SetCurState(PP_EXIT_STATE);
					
					FULLTOTVOD_ClearScreen();
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}

					GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_PP_EXIT_LINE1, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_PP_EXIT_LINE2, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_PP_EXIT_LINE3, WS_VISIBLE);
					
					GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
					GUI_ENC_API_Update(ID_STA_PP_EXIT_LINE1, 1);
					GUI_ENC_API_Update(ID_STA_PP_EXIT_LINE2, 1);
					GUI_ENC_API_Update(ID_STA_PP_EXIT_LINE3, 1);

					break;

				case GUI_VK_SELECT:
					


				
					break;

				case GUI_VK_F10:
				case GUI_VK_YELLOW:	
					FULLTOTVOD_ClearScreen();
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}


					break;
			
			case GUI_VK_P_DOWN:
			case GUI_VK_P_UP:

				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
					GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
					GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
					APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
				}
				

	
					break;
				default:
					break;
			}
			
			break;

		case TVOD_JUMPTO_STATE:
			
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
		
			break;
		
		case TVOD_PAUSE_STATE:
			
			
			switch (key)
			{
				case GUI_VK_EXIT:
	
				
					GUI_ENC_API_SetValue(ID_STA_PP_PAUSE_2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
					GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE_2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
						
					GUI_ENC_API_SetVisible(ID_STA_PP, WS_DISABLE);
					GUI_ENC_API_SetVisible(ID_STA_PP_PAUSE_2, WS_DISABLE);
					GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE_2, WS_DISABLE);
					
					GUI_ENC_API_Update(ID_STA_PP, 1);
					GUI_ENC_API_Update(ID_STA_PP_PAUSE_2, 1);
					GUI_ENC_API_Update(ID_STA_PP_CONTINUE_2, 1);
					
					FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
					FULLTOTVOD_ClearScreen();
					GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}

					break;
				case GUI_VK_SELECT:
				
			
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}

					break;
			
				default:
					break;
					
			}

			break;
		
		case TVOD_JUMPBAR_STATE:
		
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
			
			if (GUI_ENC_API_IsFocus(ID_ODC_JUMPBAR))
			{
				//UI_APP_JUMPBAR_ODC_Key(key);
			}
			break;

		
		case FULLTOTVOD_NULL_STATE:
			
			switch (key)
			{
			case GUI_VK_UP:
				APP_EnrichVod_PlayPrev();
				break;
			case GUI_VK_DOWN:
				APP_EnrichVod_PlayNext();
				break;	
			case GUI_VK_LEFT:
				break;
			case GUI_VK_RIGHT:
				break;
			case GUI_VK_SELECT:
				FULLTOTVOD_IPI_SetCurState(TVOD_PAUSE_STATE);
				break;
			case GUI_VK_RED:
				FULLTOTVOD_IPI_SetCurState(TVOD_HELP_STATE);
				break;
			case GUI_VK_EXIT:
				APP_EnrichVod_Stop();
				APP_EnrichVod_DeInit();
				GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
				break;
			}
					
		default:
			break;
	}
#endif

}


void UI_APP_FULLTOTVOD_Dispose(APP_EnrichVod_Error_Type error_type)
{
#ifdef USE_ENREACH
	switch(error_type)
	{
	case APP_EnrichVod_Error_PlayStart:
		if (APP_EnrichVod_GetSpeedFlag() != 2)return;
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "已快退到当前节目的起点", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "将重新播放当前节目", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		APP_EnrichVod_Resume();
		FYF_API_thread_sleep(2000);
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		break;
	case APP_EnrichVod_Error_PlayEnd:
	
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "当前节目播放已结束", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "将进入直播状态", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		FYF_API_thread_sleep(2000);
		FullTOTVOD_IPI_Exit();
		break;
	case APP_EnrichVod_Error_PlayReach:
		if (APP_EnrichVod_GetSpeedFlag() == 2)return;
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "已超出直播时间点", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "将进入直播状态", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);

		FYF_API_thread_sleep(2000);
		FullTOTVOD_IPI_Exit();
		break;
	case APP_EnrichVod_Error_PlayNextError:
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "没有下一个时移节目", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		FYF_API_thread_sleep(2000);
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		break;
	case APP_EnrichVod_Error_NetUnConnect_Error:
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "请检查网络连接", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		GUI_ENC_API_SetFocus(ID_ODC_FAST);
		break;
	case APP_EnrichVod_Error_NetConnect_Error:
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		GUI_ENC_API_SetFocus(ID_ODC_FAST);
		APP_EnrichVod_Resume();
		break;
	case APP_EnrichVod_Error_PlayError:
		FullTOTVOD_IPI_Exit();
		break;
	default:
		break;
		
	}
#endif	
}
void UI_APP_FULLTOTVOD_CheckError_Timer()
{
	APP_PRINT_DEBUG("UI_APP_FULLTOTVOD_CheckError_TIME\n");

#ifdef USE_ENREACH
	APP_EnrichVod_Error_Type error_type;
	error_type = APP_EnrichVod_CheckError();
	APP_PRINT_DEBUG("error_type = %d\n", error_type);
	switch(error_type)
	{
	case APP_EnrichVod_Error_PlayStart:
		if (APP_EnrichVod_GetSpeedFlag() != 2)return;
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "已快退到当前节目的起点", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "将重新播放当前节目", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		APP_EnrichVod_Resume();
		FYF_API_thread_sleep(2000);
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		break;
	case APP_EnrichVod_Error_PlayEnd:
	
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "当前节目播放已结束", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "将进入直播状态", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		FYF_API_thread_sleep(2000);
		FullTOTVOD_IPI_Exit();
		break;
	case APP_EnrichVod_Error_PlayReach:
		if (APP_EnrichVod_GetSpeedFlag() == 2)return;
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "已超出直播时间点", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "将进入直播状态", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		FYF_API_thread_sleep(2000);
		FullTOTVOD_IPI_Exit();
		break;
	case APP_EnrichVod_Error_PlayNextError:
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "没有下一个时移节目", 0);
		GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "", 0);
		FULLTOTVOD_IPI_SetCurState(TVOD_ERROR_STATE);
		FYF_API_thread_sleep(2000);
		FULLTOTVOD_IPI_SetCurState(FULLTOTVOD_NULL_STATE);
		break;
	default:
		break;
		
	}

	#endif
}


