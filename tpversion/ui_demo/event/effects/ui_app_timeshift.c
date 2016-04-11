#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


#define WORKTIME_X 			75
#define WORKTIME_W 			10

static BU16 g_timeShift_startYear=0;
static BU08 g_timeShift_startMonth=1;
static BU08 g_timeShift_startDay=1;
static BU08 g_timeShift_startHour=0;
static BU08 g_timeShift_startMin=0;
static BU08 g_timeShift_startsecd=0;

static BU08 g_timeShift_offset=0;

static BU08 g_timeShift_SelectFlag = 0;

void TimeShift_IPI_SetSpeedStyle(BU32 speed)
{
    switch(speed)
    {
    case APP_PLAYER_SPEED_NORMAL:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
       
		break;
    case APP_PLAYER_SPEED_2X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital2_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		break;
	case APP_PLAYER_SPEED_2X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_RewindIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital2_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		break;
	case  APP_PLAYER_SPEED_4X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital4_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
        break;
	case  APP_PLAYER_SPEED_8X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital8_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
        break;
	case  APP_PLAYER_SPEED_16X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital1_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital6_W30_H33, 0);
        break;
	case  APP_PLAYER_SPEED_32X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital3_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital2_W30_H33, 0);
		break;
	case APP_PLAYER_SPEED_4X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_RewindIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital4_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
        break;
	case APP_PLAYER_SPEED_8X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_RewindIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital8_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, STYLE_NULL, 0);
        break;
	case APP_PLAYER_SPEED_16X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_RewindIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital1_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital6_W30_H33, 0);
        break;
	case APP_PLAYER_SPEED_32X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedIcon_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_RewindIcon_W32_H24, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital3_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_TimeShift,WDT_STA_SV_STYLE, (BU32)STYLE_Digital2_W30_H33, 0);
		break;
	default:
		break;
    }
}
void TimeShift_IPI_CleanFullScreen(void)
{
    GUI_ENC_API_SetVisible(ID_STA_FullScreenBk_TimeShift, WS_VISIBLE);
	GUI_ENC_API_Update(ID_STA_FullScreenBk_TimeShift, 1);
}
void TimeShift_IPI_ShowConfig(BU08 bTimeJumpToShow, BU08 bTimeJumpBarShow, BU08 bHelpInfoShow, BU08 bPauseShow, BU08 bSpeedShow, BU08 bVoiBarShow)
{
    BU32 showState = 0;
	BU32 i = 0;
	GUI_WinID_Demo_e bTimeJumpTo[] = {ID_STA_TimeJumpToBk_TimeShift, ID_ODC_JumpTO_TimeShift};
	GUI_WinID_Demo_e bTimeJumpBar[] = {ID_STA_JumpBarBk_TimeShift, ID_ODC_NowTime_TimeShift, ID_PBR_Record_TimeShift};
	GUI_WinID_Demo_e bHelpInfo[] = {ID_STA_HelpBk_TimeShift, ID_STA_HelpTitle_TimeShift, ID_STA_HelpLine1_TimeShift,
		                          ID_STA_HelpLine2_TimeShift, ID_STA_HelpLine3_TimeShift, ID_STA_HelpLine4_TimeShift,
		                          ID_STA_HelpLine5_TimeShift, ID_STA_HelpLine6_TimeShift, ID_STA_HelpLine7_TimeShift};
	GUI_WinID_Demo_e bPause[] = {ID_STA_PauseInfoBK_TimeShift, ID_STA_PauseStr_TimeShift, ID_STA_PlayStr_TimeShift};

	if(bTimeJumpToShow)
	{
	   showState = WS_VISIBLE;
	}
	else
	{
	   showState = WS_DISABLE;
	}
	for(i = 0; i< 2; i++)
	{
	    GUI_ENC_API_SetVisible(bTimeJumpTo[i],showState);
		if(showState)
		{
		     GUI_ENC_API_Update(bTimeJumpTo[i],1);
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
	for(i = 0; i< 3; i++)
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
	for(i = 0; i< 9; i++)
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
	   GUI_ENC_API_Update(ID_STA_SpeedIcon_TimeShift,1);
	   GUI_ENC_API_Update(ID_STA_SpeedNumX_TimeShift,1);
	   GUI_ENC_API_Update(ID_STA_SpeedNum1_TimeShift,1);
	   GUI_ENC_API_Update(ID_STA_SpeedNum2_TimeShift,1);
	}
    if(bVoiBarShow)
    {
	    GUI_ENC_API_SetVisible(ID_VOI_TimeShift, WS_VISIBLE);
		GUI_ENC_API_Update(ID_VOI_TimeShift,1);
    }
	else
	{
	    GUI_ENC_API_SetVisible(ID_VOI_TimeShift, WS_DISABLE);
	}
	
	
}

void TimeShift_IPI_SetCurState(BU32 curState)
{
    UI_APP_API_SetTimeShiftCurState(curState);
	switch(curState)
	{
	case TIMESHIFT_NULL_STATE:
		TimeShift_IPI_CleanFullScreen();
		TimeShift_IPI_ShowConfig(0,0,0,0,0,0);
		break;
	case TIMESHIFT_JUMPTO_STATE:
		TimeShift_IPI_CleanFullScreen();
		TimeShift_IPI_ShowConfig(1,0,0,0,0,0);
		break;
	case TIMESHIFT_JUMPBAR_STATE:
		TimeShift_IPI_CleanFullScreen();
		TimeShift_IPI_ShowConfig(0,1,0,0,0,0);
		break;
	case TIMESHIFT_HELP_STATE:
		TimeShift_IPI_CleanFullScreen();
		TimeShift_IPI_ShowConfig(0,0,1,0,0,0);
		break;
	case TIMESHIFT_PAUSE_STATE:
		TimeShift_IPI_CleanFullScreen();
		TimeShift_IPI_ShowConfig(0,0,0,1,0,0);
		break;
	case TIMESHIFT_SPEED_STATE:
		TimeShift_IPI_CleanFullScreen();
		TimeShift_IPI_ShowConfig(0,0,0,0,1,0);
		break;
	case TIMESHIFT_VOIBAR_STATE:
		TimeShift_IPI_CleanFullScreen();
		TimeShift_IPI_ShowConfig(0,0,0,0,0,1);
		break;
	default:
		break;
	}
}
void UI_APP_TimeShift_Enter(void)
{
    TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
	GUI_ENC_API_SetValue(ID_TIM_TimeShift, WDT_TIM_SV_PLAY, 0, 0);
}
void UI_APP_TimeShift_End(void)
{
    APP_PVR_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
	TimeShift_IPI_SetSpeedStyle(APP_PLAYER_SPEED_NORMAL);
}

void UI_APP_TimeShift_key(BU32 key)
{
    BU32 curState;
	BU32 play_time;
	BS32 speed;
	BU32 speedvalue;
	BU32 pvrSpeedIndex;
	BU32 voiIndex;
	curState = UI_APP_API_GetTimeShiftCurState();
    switch(key)
    {
    case GUI_VK_SELECT:
		if(curState == TIMESHIFT_NULL_STATE || curState == TIMESHIFT_VOIBAR_STATE)
		{
		    APP_PVR_API_PauseRePlay();
			TimeShift_IPI_SetCurState(TIMESHIFT_PAUSE_STATE);
		}
		else if(curState == TIMESHIFT_PAUSE_STATE)
		{
		    APP_PVR_API_ResumeRePlay();
			TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
		}
		else if(curState == TIMESHIFT_SPEED_STATE)
		{   
			APP_PVR_API_SetNormalSpeed();
			APP_PVR_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
			TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
		}
		else if(curState == TIMESHIFT_HELP_STATE)
		{
		    TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
		}
		break;
	case GUI_VK_LEFT:
		if(curState != TIMESHIFT_PAUSE_STATE)
		{
			APP_PVR_API_Rewind();
			pvrSpeedIndex = APP_PVR_API_GetPvrCurSpeedIndex();
			TimeShift_IPI_SetSpeedStyle(pvrSpeedIndex);
			TimeShift_IPI_SetCurState(TIMESHIFT_SPEED_STATE);
	    }
		break;
	case GUI_VK_RIGHT:
	    if(curState != TIMESHIFT_PAUSE_STATE)
	    {
			APP_PVR_API_Speed();
			pvrSpeedIndex = APP_PVR_API_GetPvrCurSpeedIndex();
			TimeShift_IPI_SetSpeedStyle(pvrSpeedIndex);
			TimeShift_IPI_SetCurState(TIMESHIFT_SPEED_STATE);
		}
		break;
	case GUI_VK_YELLOW:
		if(curState == TIMESHIFT_SPEED_STATE)/*当前为快进快退状态*/
		{
		    APP_PVR_API_SetNormalSpeed();
			APP_PVR_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
			TimeShift_IPI_SetSpeedStyle(APP_PLAYER_SPEED_NORMAL);
		}
		else if(curState == TIMESHIFT_PAUSE_STATE)/*当前为暂停状态*/
		{
		    APP_PVR_API_ResumeRePlay();
		}
		TimeShift_IPI_SetCurState(TIMESHIFT_JUMPTO_STATE);
		GUI_ENC_API_SetFocus(ID_ODC_JumpTO_TimeShift);
		break;
	case GUI_VK_RED:
		if(curState == TIMESHIFT_SPEED_STATE)/*当前为快进快退状态*/
		{
		     APP_PVR_API_SetNormalSpeed();
			APP_PVR_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
			TimeShift_IPI_SetSpeedStyle(APP_PLAYER_SPEED_NORMAL);
		}
		else if(curState == TIMESHIFT_PAUSE_STATE)/*当前为暂停状态*/
		{
		    APP_PVR_API_ResumeRePlay();
		}
		TimeShift_IPI_SetCurState(TIMESHIFT_HELP_STATE);
		break;
	case GUI_VK_P_DOWN:
	case GUI_VK_P_UP:
        if(curState == TIMESHIFT_SPEED_STATE)/*当前为快进快退状态*/
		{
			APP_PVR_API_SetNormalSpeed();
			APP_PVR_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
			TimeShift_IPI_SetSpeedStyle(APP_PLAYER_SPEED_NORMAL);
		}
		else if(curState == TIMESHIFT_PAUSE_STATE)/*当前为暂停状态*/
		{
		    APP_PVR_API_ResumeRePlay();
		}
		
		TimeShift_IPI_SetCurState(TIMESHIFT_JUMPBAR_STATE);
		GUI_ENC_API_SetFocus(ID_ODC_NowTime_TimeShift);
		if (APP_FAIL == APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_PLAYTIME, &play_time))
		{
			return;
		}
		GUI_ENC_API_Update(ID_ODC_NowTime_TimeShift, play_time);
		
		break;
	case GUI_VK_V_DOWN:
	case GUI_VK_V_UP:
		if(curState == TIMESHIFT_NULL_STATE || curState == TIMESHIFT_VOIBAR_STATE)
		{
		    if(curState != TIMESHIFT_VOIBAR_STATE)
		    {
			   TimeShift_IPI_SetCurState(TIMESHIFT_VOIBAR_STATE);
		    }
			GUI_ENC_API_SetValue(ID_TIM_BrushScreen_TimeShift, WDT_TIM_SV_REPLAY, 0, 0);
			GUI_ENC_API_SetValueUnPaint(ID_VOI_TimeShift, WDT_VOI_SV_KEY, key, 0);
	        GUI_ENC_API_GetValue(ID_VOI_TimeShift,WDT_VOI_SV_INDEX,(BU32 *)&voiIndex, 0);
		
		    GUI_ENC_API_Update(ID_VOI_TimeShift,1);
			APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, voiIndex);
			UI_APP_API_SetVolume();
		}
		break;
	case GUI_VK_EXIT:
		if(curState == TIMESHIFT_HELP_STATE)
		{
		   TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
		}
		else
		{
			APP_PVR_API_StopRePlay(1);
			APP_PVR_API_SetDemux(0);
			APP_PVR_API_AttachAV();
			APP_Com_API_StopProg(0);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			APP_Com_API_Start_Signal_Dectect();
		}
		break;
	default:
		break;
    }
}
void UI_APP_ODCNowTime_TimeShift_key(BU32 key)
{
    static BU32 play_time;
	BU32 rec_time;
	BU32 cur_play_len;
	if(g_timeShift_SelectFlag == 0)
	{
		if (APP_FAIL == APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_PLAYTIME, &play_time))
		{
			return;
		}
	}
	if (APP_FAIL ==APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_RECTIME, &rec_time))
	{
		return;
	}
    switch(key)
    {
    case GUI_VK_P_UP:
	case GUI_VK_RIGHT:
		g_timeShift_SelectFlag = 1;
		play_time += 10;
		play_time = (play_time > rec_time) ? rec_time:play_time;
		GUI_ENC_API_Update(ID_ODC_NowTime_TimeShift, play_time);
		APP_PRINT_INFO("(UI_APP_ODCNowTime_TimeShift_key play_time )=====%d\n",play_time);
		break;
	case GUI_VK_P_DOWN:
	case GUI_VK_LEFT:
		g_timeShift_SelectFlag = 1;
		if(play_time>10)
		{play_time -= 10;}
		else
		{
		   play_time = 0;
		}
		GUI_ENC_API_Update(ID_ODC_NowTime_TimeShift, play_time);
		break;
	
	case GUI_VK_SELECT:
		if(g_timeShift_SelectFlag == 1)
		{
		   g_timeShift_SelectFlag = 0;
		   GUI_ENC_API_SetValue(ID_TIM_TimeShift, WDT_TIM_SV_STOP, 0, 0);
		   APP_PVR_API_PlaySeek(0,  1, play_time);
		   APP_Com_API_Wait();
		   APP_PRINT_DEBUG("UI_APP_ODCNowTime_TimeShift_key--------------play_time = %d \n", play_time);
		   GUI_ENC_API_Update(ID_ODC_NowTime_TimeShift, play_time);
		   GUI_ENC_API_SetValue(ID_TIM_TimeShift, WDT_TIM_SV_PLAY, 0, 0);
		}
		break;
	case GUI_VK_EXIT:
		TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
		GUI_ENC_API_SetFocus(ID_DLG_TimeShift);
		g_timeShift_SelectFlag = 0;
		break;
    default:
		break;
    }
}
void UI_APP_NowTime_TimeShift_OnPaint(BU32 hdc, BU32 lParam)
{
    BU32 x, h, m, s;
	BU32 play_time;
	BU08 strPlayTime[20];
	GUI_Rect_sp rect;
	BU32 start_time, end_time;
	play_time = lParam;
	if(play_time == 0)
	{
	   play_time = 1;
	}
	APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_STARTTIME, &start_time);
	APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_ENDTIME, &end_time);
	x = (play_time * 400 / (end_time - start_time));;
    GUI_API_DrawStyle(hdc, 0, 0, STYLE_JUMPBAR_W420_H54);
    GUI_API_DrawStyle(hdc, x, 30, STYLE_VOD_SEEK_W20_H24);

	h = play_time / 3600;
	m = (play_time - h * 3600)/60;
	s = play_time % 60;
	sprintf(strPlayTime,"%02d:%02d:%02d", h, m, s);
	GUI_API_TextOut(hdc, 80, 6, strPlayTime, strlen(strPlayTime), COLOR_BLACK, COLOR_BLACK, 1);
	APP_Printf("UI_APP_NowTime_TimeShift_OnPaint====play_time = %d\n",play_time);
}

void UI_APP_ODCJumpTO_TimeShift_key(BU32 key)
{
    BU32 preReqScale = 0;
	BU32 jumpSecs = 0;
	BU32 rec_time = 0; 
	BU08 adjTimeStr[30] = { 0 };
	BU32 startSecs = 0;
	BU32 endSecs = 0;
	BU08 bRunNian = FYF_FALSE;
	BU08 yearStr[5] = { 0 };
	BU08 monthStr[3] = { 0 };
	BU08 dayStr[3] = { 0 };
	BU08 hourStr[3] = { 0 };
	BU08 minStr[3] = { 0 };
	BU08 secStr[3] = { 0 };
	BU16 curYear = 0;
	BU08 curMonth = 0;	
	BU08 curDay = 0;
	BU08 hour = 0;
	BU08 min = 0;
	BU08 sec = 0;
	
	BU08 bDataErr = FYF_FALSE;			//时间格式错误
	BU08 bDataOutRang = FYF_TRUE;		//时间超出范围
	BU08 bDataLarger = FYF_FALSE;		//时间格式错误
	BU08 bDataSmall = FYF_FALSE;	    //时间超出范围
	
	switch(key)
	{
	case GUI_VK_0:
	case GUI_VK_1:
	case GUI_VK_2:
	case GUI_VK_3:
	case GUI_VK_4:
	case GUI_VK_5:
	case GUI_VK_6:
	case GUI_VK_7:
	case GUI_VK_8:
	case GUI_VK_9:
			switch(g_timeShift_offset)
			{
			case 0:
				g_timeShift_startYear = (BU08)((BU16)g_timeShift_startYear%10 + key*10);
				break;
			case 1:
				g_timeShift_startYear = (BU08)((BU16)g_timeShift_startYear-g_timeShift_startYear%10 + key);
				break;
			case 2:
				g_timeShift_startMonth= (BU08)((BU16)g_timeShift_startMonth%10 + key*10);
				break;
			case 3:
				g_timeShift_startMonth = (BU08)((BU16)g_timeShift_startMonth-g_timeShift_startMonth%10 + key);
				break;
			case 4:
				g_timeShift_startDay= (BU08)((BU16)g_timeShift_startDay%10 + key*10);
				break;
			case 5:
				g_timeShift_startDay = (BU08)((BU16)g_timeShift_startDay-g_timeShift_startDay%10 + key);
				break;
	
			case 6:
				g_timeShift_startHour = (BU08)((BU16)g_timeShift_startHour%10 + key*10);
				break;
			case 7:
				g_timeShift_startHour = (BU08)((BU16)g_timeShift_startHour-g_timeShift_startHour%10 + key);
				break;
			case 8:
				g_timeShift_startMin = (BU08)((BU16)g_timeShift_startMin%10 + key*10);
				break;
			case 9:
				g_timeShift_startMin = (BU08)((BU16)g_timeShift_startMin-g_timeShift_startMin%10 + key);
				break;
			case 10:
				g_timeShift_startsecd = (BU08)((BU16)g_timeShift_startsecd%10 + key*10);
				break;
			case 11:
				g_timeShift_startsecd = (BU08)((BU16)g_timeShift_startsecd-g_timeShift_startsecd%10 + key);
				break;
			default:
				break;
			}

			if(g_timeShift_startMonth >=13)	
				g_timeShift_startMonth = 1;

			if (g_timeShift_startMonth == 1 || g_timeShift_startMonth == 3 || g_timeShift_startMonth == 5 || g_timeShift_startMonth == 7 || \
				g_timeShift_startMonth == 8 || g_timeShift_startMonth == 10 || g_timeShift_startMonth == 12 )
			{
				if(g_timeShift_startDay > 31)	
					g_timeShift_startDay = 1;
			}
			else if (g_timeShift_startMonth == 4 || g_timeShift_startMonth == 6 || g_timeShift_startMonth == 9 || g_timeShift_startMonth == 11)
			{
				if(g_timeShift_startDay > 30)	
					g_timeShift_startDay = 1;
			}
			else if (g_timeShift_startMonth == 2)
			{
				if ((2000 + g_timeShift_startYear) % 4 == 0)
				{
					if ((2000 + g_timeShift_startYear) % 100 != 0)
					{
						if(g_timeShift_startDay > 29)	
							g_timeShift_startDay = 1;
					}
					else if ((2000 + g_timeShift_startYear) % 400 == 0)
					{
						if(g_timeShift_startDay > 29)	
							g_timeShift_startDay = 1;
					}
					else
					{
						if(g_timeShift_startDay > 28)	
							g_timeShift_startDay = 1;
					}
				}
				else
				{
					if(g_timeShift_startDay > 28)	
						g_timeShift_startDay = 1;
				}				
			}

			if(g_timeShift_startHour>=24)	
				g_timeShift_startHour = 0;
			if(g_timeShift_startMin>=60)	
				g_timeShift_startMin = 0;
			if(g_timeShift_startsecd>=60)	
				g_timeShift_startsecd = 0;
			
			g_timeShift_offset++;
			if(g_timeShift_offset>11)	
				g_timeShift_offset=0;

			GUI_ENC_API_Update(ID_ODC_JumpTO_TimeShift, 1);
		break;
	case GUI_VK_UP:

			switch(g_timeShift_offset)
			{
			case 0:
				g_timeShift_startYear = (BU08)((BU16)g_timeShift_startYear%10 + (g_timeShift_startYear/10 + 1)*10);
				break;
			case 1:
				g_timeShift_startYear = (BU08)((BU16)g_timeShift_startYear + 1);
				break;
			case 2:
				g_timeShift_startMonth= (BU08)((BU16)g_timeShift_startMonth%10 + (g_timeShift_startMonth/10 + 1)*10);
				break;
			case 3:
				g_timeShift_startMonth = (BU08)((BU16)g_timeShift_startMonth + 1);
				break;
			case 4:
				g_timeShift_startDay= (BU08)((BU16)g_timeShift_startDay%10 + (g_timeShift_startDay/10 + 1)*10);
				break;
			case 5:
				g_timeShift_startDay = (BU08)((BU16)g_timeShift_startDay + 1);
				break;
	
			case 6:
				g_timeShift_startHour = (BU08)((BU16)g_timeShift_startHour%10 + (g_timeShift_startHour/10 + 1)*10);
				break;
			case 7:
				g_timeShift_startHour = (BU08)((BU16)g_timeShift_startHour + 1);
				break;
			case 8:
				g_timeShift_startMin = (BU08)((BU16)g_timeShift_startMin%10 + (g_timeShift_startMin/10 + 1)*10);
				break;
			case 9:
				g_timeShift_startMin = (BU08)((BU16)g_timeShift_startMin + 1);
				break;
			case 10:
				g_timeShift_startsecd = (BU08)((BU16)g_timeShift_startsecd%10 + (g_timeShift_startsecd/10 + 1)*10);
				break;
			case 11:
				g_timeShift_startsecd = (BU08)((BU16)g_timeShift_startsecd + 1);
				break;
			default:
				break;
			}
            
            APP_Printf("g_timeShift_startDay = %d\n",g_timeShift_startDay);
			if(g_timeShift_startYear>=100)	
			{
				g_timeShift_startYear = g_timeShift_startYear % 100;
			}

			if(g_timeShift_startMonth >=13)	
				g_timeShift_startMonth = 1;

			if (g_timeShift_startMonth == 1 || g_timeShift_startMonth == 3 || g_timeShift_startMonth == 5 || g_timeShift_startMonth == 7 || \
				g_timeShift_startMonth == 8 || g_timeShift_startMonth == 10 || g_timeShift_startMonth == 12 )
			{
				if(g_timeShift_startDay > 31)	
					g_timeShift_startDay = 1;
			}
			else if (g_timeShift_startMonth == 4 || g_timeShift_startMonth == 6 || g_timeShift_startMonth == 9 || g_timeShift_startMonth == 11)
			{
				if(g_timeShift_startDay > 30)	
					g_timeShift_startDay = 1;
			}
			else if (g_timeShift_startMonth == 2)
			{
				if ((2000 + g_timeShift_startYear) % 4 == 0)
				{
					if ((2000 + g_timeShift_startYear) % 100 != 0)
					{
						if(g_timeShift_startDay > 29)	
							g_timeShift_startDay = 1;
					}
					else if ((2000 + g_timeShift_startYear) % 400 == 0)
					{
						if(g_timeShift_startDay > 29)	
							g_timeShift_startDay = 1;
					}
					else
					{
						if(g_timeShift_startDay > 28)	
							g_timeShift_startDay = 1;
					}
				}
				else
				{
					if(g_timeShift_startDay > 28)	
						g_timeShift_startDay = 1;
				}				
			}

			if(g_timeShift_startHour>=24)	
				g_timeShift_startHour = 0;
			if(g_timeShift_startMin>=60)	
				g_timeShift_startMin = 0;
			if(g_timeShift_startsecd>=60)	
				g_timeShift_startsecd = 0;
			APP_Printf("\033[0;31myear = %d, month = %d, day = %d,min = %d,sec = %d\033[0m\n",g_timeShift_startYear,g_timeShift_startMonth,g_timeShift_startDay,g_timeShift_startHour,g_timeShift_startMin,g_timeShift_startsecd);
			GUI_ENC_API_Update(ID_ODC_JumpTO_TimeShift, 1);
		break;
	case GUI_VK_DOWN:
		
		switch(g_timeShift_offset)
		{
		case 0:
			g_timeShift_startYear = (BU08)((BU16)g_timeShift_startYear%10 + (g_timeShift_startYear/10 - 1)*10);
			break;
		case 1:
			g_timeShift_startYear = (BU08)((BU16)g_timeShift_startYear - 1);
			break;
		case 2:
			g_timeShift_startMonth= (BU08)((BU16)g_timeShift_startMonth%10 + (g_timeShift_startMonth/10 - 1)*10);
			break;
		case 3:
			g_timeShift_startMonth = (BU08)((BU16)g_timeShift_startMonth - 1);
			break;
		case 4:
			g_timeShift_startDay= (BU08)((BU16)g_timeShift_startDay%10 + (g_timeShift_startDay/10 - 1)*10);
			break;
		case 5:
			g_timeShift_startDay = (BU08)((BU16)g_timeShift_startDay - 1);
			break;
		
		case 6:
			g_timeShift_startHour = (BU08)((BU16)g_timeShift_startHour%10 + (g_timeShift_startHour/10 - 1)*10);
			break;
		case 7:
			g_timeShift_startHour = (BU08)((BU16)g_timeShift_startHour - 1);
			break;
		case 8:
			g_timeShift_startMin = (BU08)((BU16)g_timeShift_startMin%10 + (g_timeShift_startMin/10 - 1)*10);
			break;
		case 9:
			g_timeShift_startMin = (BU08)((BU16)g_timeShift_startMin - 1);
			break;
		case 10:
			g_timeShift_startsecd = (BU08)((BU16)g_timeShift_startsecd%10 + (g_timeShift_startsecd/10 - 1)*10);
			break;
		case 11:
			g_timeShift_startsecd = (BU08)((BU16)g_timeShift_startsecd - 1);
			break;
		default:
			break;
		}


		
		if(g_timeShift_startYear >=245 && g_timeShift_startYear <=255)	
		{
			g_timeShift_startYear = 99 - (255 - g_timeShift_startYear);
			
		}
		
		
		if(g_timeShift_startMonth >=13 || g_timeShift_startMonth < 1)	
			g_timeShift_startMonth = 1;
		
		if (g_timeShift_startMonth == 1 || g_timeShift_startMonth == 3 || g_timeShift_startMonth == 5 || g_timeShift_startMonth == 7 || \
			g_timeShift_startMonth == 8 || g_timeShift_startMonth == 10 || g_timeShift_startMonth == 12 )
		{
			if(g_timeShift_startDay > 31 || g_timeShift_startDay < 1)	
				g_timeShift_startDay = 1;
		}
		else if (g_timeShift_startMonth == 4 || g_timeShift_startMonth == 6 || g_timeShift_startMonth == 9 || g_timeShift_startMonth == 11)
		{
			if(g_timeShift_startDay > 30 || g_timeShift_startDay < 1)	
				g_timeShift_startDay = 1;
		}
		else if (g_timeShift_startMonth == 2)
		{
			if ((2000 + g_timeShift_startYear) % 4 == 0)
			{
				if ((2000 + g_timeShift_startYear) % 100 != 0)
				{
					if(g_timeShift_startDay > 29 || g_timeShift_startDay < 1)	
						g_timeShift_startDay = 1;
				}
				else if ((2000 + g_timeShift_startYear) % 400 == 0)
				{
					if(g_timeShift_startDay > 29 || g_timeShift_startDay < 1)	
						g_timeShift_startDay = 1;
				}
				else
				{
					if(g_timeShift_startDay > 28 || g_timeShift_startDay < 1)	
						g_timeShift_startDay = 1;
				}
			}
			else
			{
				if(g_timeShift_startDay > 28 || g_timeShift_startDay < 1)	
					g_timeShift_startDay = 1;
			}				
		}
		
		if(g_timeShift_startHour>=24)	
			g_timeShift_startHour = 0;
		if(g_timeShift_startMin>=60) 
			g_timeShift_startMin = 0;
		if(g_timeShift_startsecd>=60)	
			g_timeShift_startsecd = 0;
		
		APP_Printf("\033[0;31myear = %d, month = %d, day = %d,min = %d,sec = %d\033[0m\n",g_timeShift_startYear,g_timeShift_startMonth,g_timeShift_startDay,g_timeShift_startHour,g_timeShift_startMin,g_timeShift_startsecd);

		GUI_ENC_API_Update(ID_ODC_JumpTO_TimeShift, 1);
		break;
	case GUI_VK_LEFT:
			if(g_timeShift_offset==0)	
				g_timeShift_offset=11;
			else 
				g_timeShift_offset--;
			GUI_ENC_API_Update(ID_ODC_JumpTO_TimeShift, 1);
		break;
	case GUI_VK_RIGHT:
			g_timeShift_offset++;
			if(g_timeShift_offset > 11)	
				g_timeShift_offset=0;
			GUI_ENC_API_Update(ID_ODC_JumpTO_TimeShift, 1);
		break;
	case GUI_VK_SELECT:
		if (APP_FAIL ==APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_RECTIME, &rec_time))
	    {
		    return;
	    }
		jumpSecs = (g_timeShift_startHour*3600)+(g_timeShift_startMin*60)+g_timeShift_startsecd;
	    jumpSecs = (jumpSecs > rec_time) ? rec_time:jumpSecs;
		APP_PVR_API_PlaySeek(0,  1, jumpSecs);
		TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
		GUI_ENC_API_SetFocus(ID_DLG_TimeShift);
		break;
    case GUI_VK_EXIT:
		TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
		GUI_ENC_API_SetFocus(ID_DLG_TimeShift);
		break;
	default:
		break;
	}
}
void UI_APP_JumpTO_TimeShift_OnPaint(BU32 hdc, BU32 lParam)
{
    BU08 showStr[30]={0};
	
	sprintf(showStr,"20%02d-%02d-%02d  %02d:%02d:%02d",g_timeShift_startYear,g_timeShift_startMonth, g_timeShift_startDay,g_timeShift_startHour,g_timeShift_startMin,g_timeShift_startsecd);
	GUI_API_DrawStyle(hdc, 0, 0, STYLE_JUMPTOBK_W230_H40);	
	if(g_timeShift_offset>=0&&g_timeShift_offset<2)
	{
		GUI_API_DrawHLine(hdc, (g_timeShift_offset+2)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);			
	}
	else if(g_timeShift_offset>=2&&g_timeShift_offset<4)
	{
		GUI_API_DrawHLine(hdc, (g_timeShift_offset+3)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_timeShift_offset>=4&&g_timeShift_offset<6)
	{
		GUI_API_DrawHLine(hdc, (g_timeShift_offset+4)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_timeShift_offset>=6&&g_timeShift_offset<8)
	{
		GUI_API_DrawHLine(hdc, (g_timeShift_offset+6)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_timeShift_offset>=8&&g_timeShift_offset<10)
	{
		GUI_API_DrawHLine(hdc, (g_timeShift_offset+7)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_timeShift_offset>=10&&g_timeShift_offset<12)
	{
		GUI_API_DrawHLine(hdc, (g_timeShift_offset+8)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}

	GUI_API_TextOut(hdc, 0, 8, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);
}
void UI_APP_TimeShift_Timer(void)
{
    BU32 start_time, end_time;
	BU32 play_time, rec_time;
	BU32 cur_pbr_len = 0;
	APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_STARTTIME, &start_time);
	APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_ENDTIME, &end_time);
	APP_PRINT_DEBUG("UI_APP_TimeShift_Timer %d %d \n", start_time, end_time);
	if (APP_FAIL == APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_PLAYTIME, &play_time))
	{
		return;
	}
	if (APP_FAIL ==APP_PVR_API_GetPVRAttribute(APP_PVR_ATTR_TYPE_RECTIME, &rec_time))
	{
		return;
	}
	APP_Printf("%d %d %d %d\n", start_time, end_time, play_time, rec_time);
	cur_pbr_len = (rec_time * 400 / (end_time - start_time));
	//if (last_pbr_len != cur_pbr_len)
	{
		GUI_ENC_API_SetValue(ID_PBR_Record_TimeShift, WDT_PBR_SV_CHANGE,cur_pbr_len,0);
		if(g_timeShift_SelectFlag == 0)/*在没按下翻页键时，才自动往前移*/
		{
		   GUI_ENC_API_Update(ID_ODC_NowTime_TimeShift, play_time);
		}
	}
}
void UI_APP_BrushScreen_TimeShift_Timer(void)
{
    BU32 curState;
	curState = UI_APP_API_GetTimeShiftCurState();
	switch(curState)
	{
	case TIMESHIFT_VOIBAR_STATE:
		TimeShift_IPI_SetCurState(TIMESHIFT_NULL_STATE);
		break;
	default:
		break;
	}
}