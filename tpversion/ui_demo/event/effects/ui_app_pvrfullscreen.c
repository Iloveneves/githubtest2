#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

static BU32 g_player_bplay = 1;
static BU32 g_player_ShowTimer = 0;
static BU32 g_Player_bShowFlag = 1;
static BU32 g_Player_bVOIShowFlag = 0;
static BU32 g_player_VOIShowTimer = 0;

void Pvr_IPI_SetSpeedStyle(BU32 speed)
{
    switch(speed)
    {
    case APP_PLAYER_SPEED_NORMAL:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
        GUI_ENC_API_SetValue(ID_STA_BackwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenBackwordNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_ForwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenForwardNor_Btn_W40_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayLight_Btn_W39_H30, 0);
		break;
    case APP_PLAYER_SPEED_2X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital2_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_ForwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenForwardLight_Btn_W40_H30, 0);
		break;
	case APP_PLAYER_SPEED_2X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital2_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_BackwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenBackwordLight_Btn_W40_H30, 0);
		break;
	case  APP_PLAYER_SPEED_4X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital4_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_ForwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenForwardLight_Btn_W40_H30, 0);
        break;
	case  APP_PLAYER_SPEED_8X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital8_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_ForwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenForwardLight_Btn_W40_H30, 0);
        break;
	case  APP_PLAYER_SPEED_16X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital1_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital6_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_ForwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenForwardLight_Btn_W40_H30, 0);
        break;
	case  APP_PLAYER_SPEED_32X_FAST_FORWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital3_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital2_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_ForwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenForwardLight_Btn_W40_H30, 0);
		break;
	case APP_PLAYER_SPEED_4X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital4_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_BackwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenBackwordLight_Btn_W40_H30, 0);
        break;
	case APP_PLAYER_SPEED_8X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital8_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_BackwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenBackwordLight_Btn_W40_H30, 0);
        break;
	case APP_PLAYER_SPEED_16X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital1_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital6_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_BackwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenBackwordLight_Btn_W40_H30, 0);
        break;
	case APP_PLAYER_SPEED_32X_FAST_BACKWORD:
		GUI_ENC_API_SetValue(ID_STA_SpeedNumX_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_SpeedNumX_W32_H24, 0);
        GUI_ENC_API_SetValue(ID_STA_SpeedNum1_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital3_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_SpeedNum2_PvrFullScreen,WDT_STA_SV_STYLE, (BU32)STYLE_Digital2_W30_H33, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
        GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseNor_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_BackwordBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenBackwordLight_Btn_W40_H30, 0);
		break;
	default:
		break;
    }
	if(g_player_bplay == 0)
	{
	    GUI_ENC_API_SetValue(ID_STA_PauseBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPauseLight_Btn_W40_H30, 0);
		GUI_ENC_API_SetValue(ID_STA_PlayBtn_PvrFullScreen, WDT_STA_SV_STYLE, (BU32)STYLE_PvrFullScreenPlayNor_Btn_W39_H30, 0);
	}
}

/*显示或隐藏Bar设置*/
void Pvr_IPI_ShowPlayerBar(BU08 showFlag)
{
    BU32 showState = 0;
	BU32 i = 0;

	GUI_WinID_Demo_e PlayerBar[7] = {ID_STA_PauseBtn_PvrFullScreen,ID_STA_PlayBtn_PvrFullScreen,ID_STA_BackwordBtn_PvrFullScreen,
		                             ID_STA_ForwordBtn_PvrFullScreen, ID_STA_TimerBtn_PvrFullScreen, ID_PBR_PvrFullScreen,
		                             ID_STA_PvrBarBk_PvrFullScreen };

	GUI_ENC_API_SetVisible(ID_STA_PvrPlayerBarBk_PvrFullScreen, WS_VISIBLE); 	
	GUI_ENC_API_Update(ID_STA_PvrPlayerBarBk_PvrFullScreen, 1);
	if(showFlag)
		{showState = WS_VISIBLE;}
	else
		{showState = WS_DISABLE;}
	for(i = 0; i < 7; i++)
	{
		GUI_ENC_API_SetVisible(PlayerBar[i], showState);
		if(showFlag)
		     {GUI_ENC_API_Update(PlayerBar[i] ,1);}
	}
}
void Pvr_IPI_ShowVoiBar(BU08 showFlag)
{
    BU32 showState = 0;
	BU32 i = 0;

	GUI_WinID_Demo_e PvrVoiBar[1] = {ID_VOI_PvrFullScreen};

	if(showFlag)
		{showState = WS_VISIBLE;}
	else
		{showState = WS_DISABLE;}
	for(i = 0; i < 1; i++)
	{
		GUI_ENC_API_SetVisible(PvrVoiBar[i], showState);
		if(showFlag)
		     {GUI_ENC_API_Update(PvrVoiBar[i] ,1);}
	}
}
void UI_APP_PvrFullScreen_Enter(void)
{
#if 0
     BU32 pvrNum;
	 pvrNum = APP_PVR_API_GetPlayIndex();/*获得播放的索引*/
	 APP_Player_API_Play(pvrNum);
#endif
	 APP_Com_API_SetFullScreen();
	 g_Player_bVOIShowFlag = 0;
	 Pvr_IPI_ShowVoiBar(g_Player_bVOIShowFlag);/*隐藏VOIBar*/
	 GUI_ENC_API_SetValue(ID_TIM_PvrFullScreen, WDT_TIM_SV_PLAY, 0, 0);
}
void UI_APP_PvrFullScreen_End(void)
{
    BU32 playSpeedIndex;
    GUI_ENC_API_SetValue(ID_TIM_PvrFullScreen, WDT_TIM_SV_STOP, 0, 0);
	APP_Player_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
	playSpeedIndex = APP_Player_API_GetPvrCurSpeedIndex();
    Pvr_IPI_SetSpeedStyle(playSpeedIndex);/*清除快进慢放图标*/
	GUI_ENC_API_SetValue(ID_PBR_PvrFullScreen, WDT_PBR_SV_CHANGE,0,0);
}
void UI_APP_PvrFullScreen_Key(BU32 key)
{   
    BU32 pvrNum;
	BU32 pvrCount;
	BU32 keyState;
	BU32 index;
	BU32 playSpeedIndex;
	pvrNum = APP_Player_API_GetPlayIndex();/*获得播放的索引*/
	pvrCount = APP_Player_API_GetCurTypeCount();/*获得播放的总数*/
    switch(key)
    {
    case GUI_VK_UP:
		g_player_ShowTimer = 0;
		if(g_Player_bShowFlag == 0)
		{
			g_Player_bShowFlag = 1;
		    Pvr_IPI_ShowPlayerBar(g_Player_bShowFlag);/*显示PlayerBar*/
		}
		
	    if(pvrNum <= 0)
	    {
	        pvrNum = pvrCount - 1;
	    }
		else
		{
		    pvrNum--;
		}
		APP_Player_API_SetPlayIndex(pvrNum);
		keyState = GUI_ENC_API_GetKeyState();
		if (!keyState)
		{
			APP_Player_API_Play(pvrNum);
		}
		g_player_bplay = 1;
		APP_Player_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
		playSpeedIndex = APP_Player_API_GetPvrCurSpeedIndex();
	    Pvr_IPI_SetSpeedStyle(playSpeedIndex);/*清除快进慢放图标*/
		
		break;
	case GUI_VK_DOWN:
		g_player_ShowTimer = 0;
		if(g_Player_bShowFlag == 0)
		{
			g_Player_bShowFlag = 1;
		    Pvr_IPI_ShowPlayerBar(g_Player_bShowFlag);/*显示PlayerBar*/
		}
		if(pvrNum >= pvrCount - 1)
		{
		    pvrNum = 0;
		}
		else
		{
		    pvrNum++;
		}
		APP_Player_API_SetPlayIndex(pvrNum);
		keyState = GUI_ENC_API_GetKeyState();
		if (!keyState)
		{
			APP_Player_API_Play(pvrNum);
		}
		g_player_bplay = 1;
		APP_Player_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
		playSpeedIndex = APP_Player_API_GetPvrCurSpeedIndex();
	    Pvr_IPI_SetSpeedStyle(playSpeedIndex);/*清除快进慢放图标*/
		
		break;
	case GUI_VK_SELECT:
		g_player_ShowTimer = 0;
		if(g_Player_bShowFlag == 0)
		{
			g_Player_bShowFlag = 1;
		    Pvr_IPI_ShowPlayerBar(g_Player_bShowFlag);/*显示PlayerBar*/
		}
		if(g_player_bplay == 0)/*播放*/
		{
			g_player_bplay = 1;
			APP_Player_API_Resume();
		}
		else/*暂停*/
		{
			g_player_bplay = 0;
			APP_Player_API_Pause();
		}
		Pvr_IPI_SetSpeedStyle(APP_PLAYER_SPEED_NORMAL);
		APP_Printf("UI_APP_PvrFullScreen_Key():g_player_bplay = %d\n", g_player_bplay);
		break;
	case GUI_VK_LEFT:
		g_player_ShowTimer = 0;
		if(g_Player_bShowFlag == 0)
		{
			g_Player_bShowFlag = 1;
		    Pvr_IPI_ShowPlayerBar(g_Player_bShowFlag);/*显示PlayerBar*/
		}
		if(g_player_bplay == 1)
		{
			APP_Player_API_Rewind();
			playSpeedIndex = APP_Player_API_GetPvrCurSpeedIndex();
			Pvr_IPI_SetSpeedStyle(playSpeedIndex);
		}
		break;
	case GUI_VK_RIGHT:
	    g_player_ShowTimer = 0;
		if(g_Player_bShowFlag == 0)
		{
			g_Player_bShowFlag = 1;
		    Pvr_IPI_ShowPlayerBar(g_Player_bShowFlag);/*显示PlayerBar*/
		}
		if(g_player_bplay == 1)
		{
			APP_Player_API_Speed();
	        playSpeedIndex = APP_Player_API_GetPvrCurSpeedIndex();
			Pvr_IPI_SetSpeedStyle(playSpeedIndex);
		}
		break;
	case GUI_VK_V_DOWN:
    case GUI_VK_V_UP:
		g_player_VOIShowTimer = 0;
		if(g_Player_bVOIShowFlag == 0)
		{
			g_Player_bVOIShowFlag = 1;
		    Pvr_IPI_ShowVoiBar(g_Player_bVOIShowFlag);/*显示VOIBar*/
		}
		GUI_ENC_API_SetValueUnPaint(ID_VOI_PvrFullScreen, WDT_VOI_SV_KEY, key, 0);
		GUI_ENC_API_GetValue(ID_VOI_PvrFullScreen, WDT_VOI_SV_INDEX, &index, 0);	if (index == 0)
	
	    GUI_ENC_API_Update(ID_VOI_PvrFullScreen, 1);
		APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
		UI_APP_API_SetVolume();
		break;
	case GUI_VK_INFO:
		g_player_ShowTimer = 0;
		if(g_Player_bShowFlag == 0)
		{
			g_Player_bShowFlag = 1;
		    Pvr_IPI_ShowPlayerBar(g_Player_bShowFlag);/*显示PlayerBar*/
		}
		playSpeedIndex = APP_Player_API_GetPvrCurSpeedIndex();
		Pvr_IPI_SetSpeedStyle(playSpeedIndex);
		break;
	case GUI_VK_EXIT:
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_PvrList, 1);
		break;
	default:
		break;
    }
}
void UI_APP_PvrFullScreen_Timer(void)
{
    Player_Time_S times;
	Player_Time_S curtime;
	BU32 cursec, totalsec;
	static BU32 start_time = 0;
	BU08 curstatus;
	BU32 pvrNum;
	BU32 pvrCount;
	BU32 playSpeedIndex;
	APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_CURSTATUS_e, &curstatus);
	if (curstatus == 1)
	{
		if (APP_FAIL == APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_TIMES_e, (void*)&times))
		{
			return;
		}
		if (APP_FAIL == APP_Player_API_GetFileAttribute(APP_PLAYER_FILEATTR_TYPE_CURTIME_e,(void*)&curtime))
		{
			return;

		}
		cursec = curtime.hour * 3600 + curtime.min * 60 + curtime.sec;
		totalsec = times.hour * 3600 + times.min * 60 + times.sec;
		APP_Printf("cursec = %d, totalsec = %d\n", cursec, totalsec);
		//cwd add 防止进度条越界界面显示错误
		if (cursec > totalsec)cursec = totalsec;
		
		if (totalsec != 0)
			GUI_ENC_API_SetValue(ID_PBR_PvrFullScreen, WDT_PBR_SV_CHANGE,(BU32)(cursec * 242 / totalsec),0);
		if (cursec == totalsec)
		{
		    GUI_ENC_API_SetValue(ID_PBR_PvrFullScreen, WDT_PBR_SV_CHANGE, 0, 0);
		    pvrNum = APP_Player_API_GetPlayIndex();/*获得播放的索引*/
			pvrCount = APP_Player_API_GetCurTypeCount();/*获得播放的总数*/
		    if(pvrNum >= pvrCount - 1)
		    {
		       pvrNum = 0;
		    }
			else
			{
			    pvrNum++;
			}
		    APP_Player_API_SetPlayIndex(pvrNum);
			APP_Player_API_Play(pvrNum);
			APP_Player_API_SetPvrCurSpeedIndex(APP_PLAYER_SPEED_NORMAL);
			playSpeedIndex = APP_Player_API_GetPvrCurSpeedIndex();
		    Pvr_IPI_SetSpeedStyle(playSpeedIndex);/*清除快进慢放图标*/
			
		}
	}
	if(g_player_ShowTimer >= 8) /*8秒隐藏PlayBar*/
	{
	    g_player_ShowTimer = 0;
		g_Player_bShowFlag = 0;
	    Pvr_IPI_ShowPlayerBar(g_Player_bShowFlag);
	}
	else
	{
	    g_player_ShowTimer++;
	}

	if(g_player_VOIShowTimer >= 3) /*3秒隐藏VOIBar*/
	{
	    g_player_VOIShowTimer = 0;
		g_Player_bVOIShowFlag = 0;
	    Pvr_IPI_ShowVoiBar(g_Player_bVOIShowFlag);
	}
	else
	{
	    g_player_VOIShowTimer++;
	}
}
