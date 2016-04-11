

#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"
#ifdef USE_FZVOD
#include "../../../ow_vod/ow_debug.h"
#endif


#define NOKEY_COUNT            (9000)
#define WORKTIME_X 			75
#define WORKTIME_W 			 10
#define ALL_PRESS_TIMES       80
#define STEP_LONG			(400 / ALL_PRESS_TIMES)


extern BU08 bfirst;
extern BU32 GUI_IpanelState;
extern BU32 GUI_NullState;
extern BU08 bGuiTimerSuspend;
extern BU08 g_fromIpanel;
extern int recLongTime;
extern 	BS32 RegionID ;
extern int 	recStarTime;

#ifdef USE_FZVOD
extern int  optionTimeCount;
extern BU32 VodState;
extern OW_PLAY_PARA VodPlayPara;
extern int optionSocket;
extern BU32 PTSStart;
extern BU08 bPlayFromStart;			//
extern BU08 bTVODPlay;				//进入时移并播放过节目的标志,因为按选时键进入时如果超长时间未确认则直接退出，此时退出不必续播
static BU08 g_vod_year = 0;
static BU08 g_vod_month = 0;
static BU08 g_vod_date = 0;
static BU08 g_vod_startHour=0;
static BU08 g_vod_startMin=0;
static BU08 g_vod_startsecd=0;
static BU08 g_vod_endHour=23;
static BU08 g_vod_endMin=59;
static BU08 g_vod_endsecd=59;
static BU08 g_vod_offset=0;

static BU08 g_vod_AllHour=0;
static BU08 g_vod_AllMin=0;
static BU08 g_vod_Allsecd=0;
static BU32 vod_progress = 0;
static BU32 g_perPasstime = 0;


static BU16 g_tvod_startYear=0;
static BU08 g_tvod_startMonth=0;
static BU08 g_tvod_startDay=0;
static BU08 g_tvod_startHour=0;
static BU08 g_tvod_startMin=0;
static BU08 g_tvod_startsecd=0;

static BU16 g_tvod_endYear=0;
static BU08 g_tvod_endMonth=0;
static BU08 g_tvod_endDay=0;
static BU08 g_tvod_endHour=23;
static BU08 g_tvod_endMin=59;
static BU08 g_tvod_endsecd=59;

static BU08 g_tvod_offset=0;
static int g_tvod_tmp_startSecs=0;
static int g_tvod_tmp_endSecs=0;
static int g_tvod_tmp_curSecs=0;

BU08 bLarger = FYF_FALSE;
BU08 bSmaller = FYF_FALSE;			//时间超出范围

static BU32 tvod_progress = 0;
static BU32 tvod_perPasstime = 0;
static BU08 g_tvod_AllHour=0;
static BU08 g_tvod_AllMin=0;
static BU08 g_tvod_Allsecd=0;
BU16 yelBarLen = 0;

int g_CurSecs = 0;
int g_PreCurSecs = 0;
char g_adjTimeStr[30] = { 0 };
int g_startSecs = 0;
int g_endSecs = 0;
char g_hourStr[3] = { 0 };
char g_minStr[3] = { 0 };
char g_secStr[3] = { 0 };

BU08  bPressLeftKey = FYF_FALSE;

int   leftscale = 0;
int   rightscale = 0;

char clockPause[20] = { 0 };
int pauseTime = 0;
int reStarTIme = 0;

BU08 bInitJumpBar = FYF_FALSE;
BU08 bInitVodJumpBar = FYF_FALSE;
extern 	BU08 bPauseDisPly;
extern BU08 bIntvod;
extern OW_TVOD_CHANNELS   TvodChanInfo;
extern int curChannelNum;
extern 	BU08 serSetParaType; 			//服务器发给机顶盒异常消息类型 0: 节目关闭; 1:节目开头;2:节目结尾
BU08 bFullToTvodCount = 5;
BU08 bCheckChannel = FYF_FALSE;
BU08 bInTvodTimer = FYF_FALSE;
static BU32 g_fulltotvod_curState = FULLTOTVOD_NULL_STATE;
static BU32 g_vod_curState = VOD_NULL_STATE;
static BU32 g_tvod_PrevState = VOD_NULL_STATE;

static BU08 g_init = FYF_FALSE;
BU08 bHadRes = FYF_FALSE;
static BU08 bFromF10 = FYF_FALSE;
static BU08 bIntoFast = FYF_FALSE;
static BU08 g_isleft = FYF_FALSE;

extern BU08 bIsSelect;
extern BU08 bIsLeft;
extern BU08 bNetError;
extern char g_StartClock[25];
extern BU08 bDrawSpeed;
extern BU08  bNetErr;
char FirsttimeStr[50] = { 0 };
BU08 bPause = FYF_FALSE;
BU32 PTSCur = 0;
BU08 bInitVodEnter = FYF_FALSE;
int noKeytimeCount = 0;
extern BU08 bTurnerError;

BS32 g_firststate_errorcode = 0;
extern 	HI_U32 w,r;
extern HI_U32 num;
extern  int bCableDownCount;
#endif
BU32 ucNET_ON_OFF = FYF_FALSE;


extern 	void CallBackGetTableFunc(BU32 channelID, BU32 filterID, BU08* pSection, BS32 len);

void FULLTOTVOD_SetCurState(BU32 curState)
{
#ifdef USE_FZVOD
	g_fulltotvod_curState = curState;
#endif
}

BU32 FULLTOTVOD_GetCurState(void)
{
#ifdef USE_FZVOD
	return g_fulltotvod_curState;
#endif
}
void VOD_SetCurState(BU32 curState)
{
#ifdef USE_FZVOD
	g_vod_curState = curState;
#endif
}

BU32 VOD_GetCurState(void)
{
#ifdef USE_FZVOD
	return g_vod_curState;
#endif
}

int RTSP_NokeyPress(void)		//无按键150分钟后退出,9000S
{
#ifdef USE_FZVOD	
	noKeytimeCount++;
	printf("noKeytimeCount = %d\n", noKeytimeCount);
	if (noKeytimeCount == /*9000*/NOKEY_COUNT)
	{
		//noKeytimeCount = 0;
		
		GUI_ENC_API_SetValue(ID_STA_PP_PAUSE_2, WDT_STA_SV_STRING, "是否继续观看?", 0);
		GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE_2, WDT_STA_SV_STRING, "按任意键继续", 0);

		GUI_ENC_API_SetVisible(ID_STA_PP, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_PP_PAUSE_2, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE_2, WS_VISIBLE);
		
		GUI_ENC_API_Update(ID_STA_PP, 1);
		GUI_ENC_API_Update(ID_STA_PP_PAUSE_2, 1);
		GUI_ENC_API_Update(ID_STA_PP_CONTINUE_2, 1);

		g_tvod_PrevState = FULLTOTVOD_GetCurState();
		FULLTOTVOD_SetCurState(TVOD_NOKEY_PRESS_TIMEOUT_STATE);

		//FYF_API_thread_sleep(2000);
		//if (OW_DrawExit5() == FYF_OK)
		{
			//OW_SetExitFlag(FYF_TRUE);
			OW_ERROR("OW_NokeyPress for 150 min!!!\n");
			return FYF_OK;
		}
	}

	if (noKeytimeCount == (NOKEY_COUNT + 30))
	{
		OW_SetExitFlag(FYF_TRUE);
		OW_ERROR("OW_NokeyPress for 150 min!!!\n");
		noKeytimeCount = 0;
		return FYF_OK;
	}

	return FYF_ERR;
#endif


}


void FULLTOTVOD_ClearScreen(void)
{
#ifdef USE_FZVOD	

	FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
	FYF_API_OSD_Refresh(0, 0, 720, 576);
	
	GUI_ENC_API_SetVisible(ID_STA_BPLOGO, WS_VISIBLE);	
	GUI_ENC_API_Update(ID_STA_BPLOGO, 1);
	GUI_API_TextOut(GUI_API_GetDlgItem(ID_DLG_FULLTOTVOD), 36, 492, "电视回看", strlen("电视回看"), RGB(255,57,0)/*COLOR_WHITE*/, /*RGB(245,57,135)*/COLOR_WHITE, 1);
#endif
}

void VOD_ClearScreen(void)
{
#ifdef USE_FZVOD	
	FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
	FYF_API_OSD_Refresh(0, 0, 720, 576);
#endif
}

void TVOD_ExitInfomation(void)
{
#ifdef USE_FZVOD	

	APP_Date_S DateAndTime;
	BU32 curstate = 0;
	
	curstate = FULLTOTVOD_GetCurState();
	printf("curstate = %d\n", curstate);
	if ((curstate != BPZHUN_STATE) && (curstate != BPJUMP_STATE))
	{
		FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
		FYF_API_OSD_Refresh(0, 0, 720, 576);
		
		GUI_ENC_API_SetValue(ID_STA_PLAYCHN, WDT_STA_SV_DATAID, RS_PP_PlayChn, 0);
		GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE, WDT_STA_SV_STRING, "现在时间是", 0);
		APP_SYS_API_GetLocalDateAndTime(&DateAndTime);
		
		memset(FirsttimeStr, 0, 50);
		sprintf(FirsttimeStr, "%02d:%02d:%02d", DateAndTime.m_hour, DateAndTime.m_min, DateAndTime.m_sec);
		printf("\033[0;31m FirsttimeStr = %s\033[0m\n", FirsttimeStr);
		GUI_ENC_API_SetValue(ID_STA_PP_PLAYTIME, WDT_STA_SV_STRING, FirsttimeStr, 0);
		
		GUI_ENC_API_SetVisible(ID_STA_PP, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_PLAYCHN, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_PP_PLAYTIME, WS_VISIBLE);
		
		GUI_ENC_API_Update(ID_STA_PP, 1);
		GUI_ENC_API_Update(ID_STA_PLAYCHN, 1);
		GUI_ENC_API_Update(ID_STA_PP_CONTINUE, 1);
		GUI_ENC_API_Update(ID_STA_PP_PLAYTIME, 1);
		
		FYF_API_thread_sleep(2000);
		
		FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
		FYF_API_OSD_Refresh(0, 0, 720, 576);

	}
#endif
}



int UI_APP_BPJUMP_INIT(void)
{
#ifdef USE_FZVOD	

	BS32 RegionIdCache = 0xff;
	char ipStr[20] = { 0 };
	char portStr[10] = { 0 };
	int m_s = -1;
	char clockPause[20] = { 0 };
	BU08 bHadRes = FYF_FALSE;
	int ret = -1;
	BU08 stopTag = FYF_FALSE;
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
	
	if (FULLTOTVOD_GetCurState() == IPANEL_TO_TVOD_STATE) 
	{
		return;
	}

	FYF_API_OSD_FillRect(0, 0, 720, 576,0);
	FYF_API_OSD_Refresh(0, 0, 720, 576);


	OW_SetExitFlag(FYF_FALSE);
	VodPlayPara.VodOrTvod = 1;
	serSetParaType = 0xff;
	OW_VOD_SetNetErrFlag(FYF_FALSE);
	
#ifdef USE_IPANEL
	Ow_Set_entervodortvod_Flag(FYF_TRUE);
#endif
	bTVODPlay = FYF_FALSE;
	OW_SetFirstPauseFlag(FYF_TRUE);
	noKeytimeCount=0;
	if(RTSP_CheckNetOk() != FYF_OK)
	{
		OW_SetExitFlag(FYF_TRUE);
	}
	
	if (OW_GetExitFlag() == FYF_FALSE)
	{
		RTSP_DefaultPlayPara(&VodPlayPara);
		memcpy(VodPlayPara.progAddr, TvodChanInfo.TsgChannel[curChannelNum].ProgURL,
			strlen(TvodChanInfo.TsgChannel[curChannelNum].ProgURL));
		RegionIdCache = RegionID;
		
		if((RegionID == 0xff) && (Ipanel_DLRegionId() != FYF_OK))
		{
			OW_DrawInfo("无法获取区域ID!", FYF_TRUE, FYF_FALSE, FYF_FALSE);
			OW_SetExitFlag(FYF_TRUE);
			printf("Ipanel_DLRegionId Get Region Error\n");
			OW_ERROR("OW_VOD_MainMenu out from Ipanel_DLRegionId !!\n ");
		}
		else
		{
			VodPlayPara.regionID = RegionID;
			if(RegionID == 0xff)
			{
				OW_DrawInfo("服务器忙,请稍后再试", FYF_TRUE, FYF_FALSE, FYF_FALSE);
				OW_SetExitFlag(FYF_TRUE);
			}
		}
		if (OW_GetExitFlag() == FYF_FALSE)
		{
			while (1)
			{
				if (RTSP_GetIPStrAndPortStr(VodPlayPara.progAddr, ipStr, portStr) == FYF_ERR)
				{
					OW_ERROR("OW_VOD_MainMenu out from RTSP_GetIPStrAndPortStr err!\n ");
					OW_SetExitFlag(FYF_TRUE);
					break;
				}

				if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_SOCKET_OpenSocket(&m_s, ipStr, portStr) == FYF_ERR)
				{
					OW_ERROR("OW_VOD_MainMenu out from RTSP_OpenSocket err!\n ");
					OW_DrawInfo("服务器忙,请稍后再试",FYF_TRUE, FYF_FALSE, FYF_FALSE);
					
					if (!bInTvodTimer && (GUI_IpanelState != GUI_ENC_API_GetCurState()))
					{
						if (VodPlayPara.VodOrTvod == 1 && !bFromF10)
						{
							OW_ERROR("服务器忙,请稍后再试\n");
							GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "服务器忙,请稍后再试", 0);
							GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
							GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
							GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
							GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
							
							FYF_API_thread_sleep(1500);
							
							FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
							FYF_API_OSD_Refresh(0, 0, 720, 576);
						}
					}
					OW_SetExitFlag(FYF_TRUE);

					break;
				}

				if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateDescribe(m_s, &VodPlayPara) == FYF_ERR)
				{
					OW_ERROR("OW_VOD_MainMenu out from RTSP_CreateDescribe err!!\n ");
					break;
				}
				
				if (VodPlayPara.bReLocate)
				{
					VodPlayPara.bReLocate = FYF_FALSE;
					OW_SetExitFlag(FYF_FALSE);
					close(m_s);
					m_s = -1;
					FYF_API_Memset(ipStr, 0, 20);
					FYF_API_Memset(portStr, 0, 10);
					OW_DEBUG("OW_TVOD_MainMenu relocation\n ");
					continue;
				}
				else
				{
					break;
				}

			}

		}
	}

	optionSocket = m_s;
	RTSP_ChecProgAddr(VodPlayPara.progAddr);
	
	if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateSETUP(m_s, &VodPlayPara) == FYF_ERR)
	{
		OW_ERROR("OW_VOD_MainMenu out from RTSP_CreateSETUP VIDEO err!!\n ");
		OW_SetExitFlag(FYF_TRUE);
	}

	if (OW_GetExitFlag() == FYF_FALSE)
	{
		VodPlayPara.reqScale = 1;
		VodPlayPara.bEOS = FYF_TRUE;
		if (RTSP_CreatePLAY(m_s, &VodPlayPara) == FYF_ERR)
		{
			OW_ERROR("OW_VOD_MainMenu out from TP_CreatePLAY11  err!!\n ");
			OW_SetExitFlag(FYF_TRUE);
		}
		
		if (OW_GetExitFlag() == FYF_FALSE) 
		{
			if (RTSP_CreateTGET_PARAMETER(m_s, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("OW_DrawBPJump RTSP_CreateTGET_PARAMETER  err!!\n ");
				return FYF_ERR;
			}
			optionSocket = m_s;
		}

		
		printf("clockStart = %s, clockCur = %s, clockend = %s\n", VodPlayPara.clockStart, VodPlayPara.clockCur,VodPlayPara.clockEnd);
		
		RTSP_GetSecondByClock(VodPlayPara.clockStart, &g_tvod_tmp_startSecs, &startYear, &startMonth, &startDay, FYF_TRUE);
		RTSP_GetSecondByClock(VodPlayPara.clockCur, &CurSecs, &curYear, &curMonth, &curDay, FYF_FALSE);
		RTSP_GetSecondByClock(VodPlayPara.clockEnd, &g_tvod_tmp_endSecs, &endYear, &endMonth, &endDay, FYF_TRUE);
		
		hour = CurSecs / 3600;
		CurSecs %= 3600;
		min = CurSecs / 60;
		CurSecs %= 60;
		sec = (BU08) CurSecs;
		RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
			secStr);
		sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
		
		printf("\033[0:31m Curtime = %s\033[0m\n", adjTimeStr);
		
		g_tvod_startYear = atoi(yearStr) - 2000;	
		if (g_tvod_startYear < 0 || g_tvod_startYear > 100)
			g_tvod_startYear = 0;
		
		g_tvod_startMonth = (BU08) atoi(monthStr);
		if (g_tvod_startMonth < 0)
			g_tvod_startMonth = 0;
		
		g_tvod_startDay = (BU08) atoi(dayStr);
		if (g_tvod_startDay < 0)
			g_tvod_startDay = 0;
		
		g_tvod_startHour = (BU08) atoi(hourStr);
		if (g_tvod_startHour < 0)
			g_tvod_startHour = 0;
		
		g_tvod_startMin = (BU08) atoi(minStr);
		if (g_tvod_startMin < 0)
			g_tvod_startMin = 0;
		
		g_tvod_startsecd = (BU08) atoi(secStr);
		if (g_tvod_startsecd < 0)
			g_tvod_startsecd = 0;
		
		
	}
	
#endif	
}



void UI_APP_BPZHUN_OnPaint(BU32 hdc,BU32 lParam)
{
#ifdef USE_FZVOD	

	BU08 showStr[100]={0};
	char ipStr[20] = { 0 };
	char portStr[10] = { 0 };
	int m_s = -1;
	
	printf("lparam = %d\n", lParam);
	sprintf(showStr,"回放准备%d秒 按任意键退出",bFullToTvodCount);
	GUI_API_DrawStyle(hdc, 0, 0, STYLE_BPZHUN_W276_H30);
	GUI_API_TextOut(hdc, 10, 2, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);
#endif
}


void UI_APP_BPZHUN_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	
	printf("Press Key.==%d===============UI_APP_BPZHUN_ODC_Key\n",key);
	
	if (GUI_ENC_API_IsFocus(ID_ODC_BPZHUN))
	{
		if (key && (bFullToTvodCount != 0))
		{
			OW_SetExitFlag(FYF_TRUE);
		}
	}
#endif							
}


void UI_APP_BPZHUN_TIME(void)
{
#ifdef USE_FZVOD	

	int preReqScale = 0;
	int ret = -1;
	int CurSecs = 0;
	char adjTimeStr[30] = { 0 };
	int startSecs = 0;
	int endSecs = 0;
	BU08 bRunNian = FYF_FALSE;
	char yearStr[5] = { 0 };
	char monthStr[3] = { 0 };
	char dayStr[3] = { 0 };
	char hourStr[3] = { 0 };
	char minStr[3] = { 0 };
	char secStr[3] = { 0 };
	BU16 curYear = 0;
	BU08 curMonth = 0;	
	BU08 curDay = 0;
	int hour;
	int min;
	int sec;
	
	int curTime = 0;
	BU16 year = 0;
	BU08 month = 0;
	BU08 day = 0;

	
	BU08 bDataErr = FYF_FALSE;			//时间格式错误
	BU08 bDataOutRang = FYF_TRUE;		//时间超出范围
	BU08 bDataLarger = FYF_FALSE;		//时间格式错误
	BU08 bDataSmall = FYF_FALSE;			//时间超出范围

	bInTvodTimer = FYF_TRUE;
	
	if (FULLTOTVOD_GetCurState() == IPANEL_TO_TVOD_STATE) 
	{
		bFullToTvodCount = 0;
	}

	if (bFullToTvodCount > 0)
	{		
		bFullToTvodCount--;
		GUI_ENC_API_Update(ID_ODC_BPZHUN, bFullToTvodCount);
	}
	if (bFullToTvodCount < 0)
		bFullToTvodCount = 0;
	printf("UI_APP_FullToTvod_TIME , bFullToTvodCount = %d\n", bFullToTvodCount);

	if (bFullToTvodCount == 0)
	{
		if (bNetError)
		{
			bInTvodTimer = FYF_FALSE;
			GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_STOP,0,0);
			OW_SetExitFlag(FYF_TRUE);
			return;
		}
		bInTvodTimer = FYF_FALSE;
		FULLTOTVOD_ClearScreen();
		GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_DISABLE);
		
		GUI_ENC_API_SetVisible(ID_STA_BPJUMP, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);

		GUI_ENC_API_Update(ID_STA_BPJUMP, 1);
		GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);
		GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
		GUI_ENC_API_Update(ID_ODC_BPZHUN, 1);			
		GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_STOP,0,0);

		if (!bFromF10)
		{
			UI_APP_BPJUMP_INIT();
			bFromF10 = FYF_FALSE;
		}
		
		APP_IPI_StopAudioVideo();
		
		if (FULLTOTVOD_GetCurState() == IPANEL_TO_TVOD_STATE) 
		{
			if (OW_GetMPEGType() == FYF_TRUE)
			{
				if (OW_DLVodPidInfo(VodPlayPara.pFre, VodPlayPara.pPmtPid, &(VodPlayPara.pVPid), &(VodPlayPara.pAPid),&(VodPlayPara.PcrPid), &(VodPlayPara.mAudioType), &(VodPlayPara.mVideoType)) != FYF_OK)
				{
					OW_ERROR("OW_VOD_MainMenu out from TPOWVOD_DLVodPidInfo  err!!\n ");
					OW_SetExitFlag(FYF_TRUE);
				}
				else
				{
					if (OW_GetExitFlag() == FYF_FALSE)
					{
						
						bHadRes = FYF_TRUE; 		//取得服务器前端分配资源
						
						ipanel_porting_video_location(-35, -25, 720, 576);
						
						if ( 0 == VodPlayPara.pVPid)
							VodPlayPara.pVPid = 0x1fff;
						if ( 0 == VodPlayPara.pAPid)
							VodPlayPara.pAPid = 0x1fff;
						//if ( 0 == VodPlayPara.PcrPid)
							//VodPlayPara.PcrPid = 0x1fff;
						
						ret = APP_IPI_PlayProg(VodPlayPara.pVPid, VodPlayPara.pAPid, VodPlayPara.PcrPid, VodPlayPara.mVideoType, VodPlayPara.mAudioType);
						if(FYF_OK != ret)
						{
							OW_ERROR("APP_IPI_PlayProg failed! 0x%x\n", ret);
							OW_SetExitFlag(FYF_TRUE);
							
						}
						
						//OW_DLRTPInfo(VodPlayPara.pFre, VodPlayPara.pPmtPid);
				
						bTVODPlay = FYF_TRUE;
						bFromF10 = FYF_FALSE;
						FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
						
						GUI_ENC_API_SetVisible(ID_STA_INFOBP, WS_VISIBLE);	
						GUI_ENC_API_SetVisible(ID_STA_INBP, WS_VISIBLE);	
						GUI_ENC_API_Update(ID_STA_INFOBP, 1);
						GUI_ENC_API_Update(ID_STA_INBP, 1);
						
						FYF_API_thread_sleep(2000);
						FULLTOTVOD_ClearScreen();
				
						
					}
				}
				
			}
			else
			{
				if (OW_GetExitFlag() == FYF_FALSE)
				{
					
					bHadRes = FYF_TRUE; 		//取得服务器前端分配资源
					
					ipanel_porting_video_location(-35, -25, 720, 576);
									
					//OW_DLRTPInfo(VodPlayPara.pFre, VodPlayPara.pPmtPid);
			
					bTVODPlay = FYF_TRUE;
					bFromF10 = FYF_FALSE;
					FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
					
					GUI_ENC_API_SetVisible(ID_STA_INFOBP, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_STA_INBP, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_INFOBP, 1);
					GUI_ENC_API_Update(ID_STA_INBP, 1);
					
					FYF_API_thread_sleep(2000);
					FULLTOTVOD_ClearScreen();
						
				}
			}
		
			return;
		}
		else
		{
			FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
		}

		if (bFromF10)
		{
			VodPlayPara.reqScale = 1;
			if (!bPlayFromStart && RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("OW_VOD_MainMenu out from RTSP_CreatePLAY	err!!\n ");
			}
			bPlayFromStart = FYF_FALSE;
			
			
			if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR) //获取时移时间范围
			{
				OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}

		}
		else
		{
			printf("stop video ==============================\n");
			VodPlayPara.reqScale = 1;
			VodPlayPara.bEOS = FYF_TRUE;
			if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("OW_VOD_MainMenu out from TP_CreatePLAY11	err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
			if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR) //获取时移时间范围
			{
				OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
			else
			{
				sprintf(clockPause,"%s",VodPlayPara.clockCur);
			}

		}

		if (OW_GetMPEGType() == FYF_TRUE)
		{
			if (OW_DLVodPidInfo(VodPlayPara.pFre, VodPlayPara.pPmtPid, &(VodPlayPara.pVPid), &(VodPlayPara.pAPid),&(VodPlayPara.PcrPid), &(VodPlayPara.mAudioType), &(VodPlayPara.mVideoType)) != FYF_OK)
			{
				OW_ERROR("OW_VOD_MainMenu out from TPOWVOD_DLVodPidInfo  err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
			else
			{
				if (OW_GetExitFlag() == FYF_FALSE)
				{
					if (bIsSelect == FYF_TRUE && !bFromF10) //确认键进入,要暂停
					{
						VodPlayPara.reqScale = 0;
						if (RTSP_CreatePAUSE(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							OW_ERROR("OW_VOD_MainMenu out from RTSP_CreatePAUSE  err!!\n ");
							OW_SetExitFlag(FYF_TRUE);
						}
						else
						{
							OW_DrawSpeed(VodPlayPara.reqScale);
							bPauseDisPly = FYF_TRUE;
							FYF_API_thread_sleep(500);
						}
						FULLTOTVOD_ClearScreen();
						
						RTSP_GetSecondByClock(VodPlayPara.clockCur, &curTime, &year, &month, &day, FYF_FALSE);
						printf("curtime = %d, year = %d, month = %d, day = %d\n", curTime, year, month, day);
						RTSP_ParseSecToHourMin(curTime, &hour, &min, &sec);
						printf("curtime = %d, hour = %d, min = %d, sec = %d\n", curTime, hour, min, sec);
						RTSP_ParseNumTimeToStr(year, month, day, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr, secStr);
						printf("hourStr = %s, minStr = %s, secStr = %s\n", hourStr, minStr, secStr);
						memcpy(FirsttimeStr, "播放时间:", strlen("播放时间:"));
						sprintf(FirsttimeStr + strlen("播放时间:"), "%s:%s:%s", hourStr, minStr, secStr);
						printf("\033[0;31m FirsttimeStr = %s\033[0m\n", FirsttimeStr);
						
						GUI_ENC_API_SetValue(ID_STA_PP_PLAYTIME, WDT_STA_SV_STRING, FirsttimeStr, 0);
						GUI_ENC_API_SetValue(ID_STA_PP_PAUSE, WDT_STA_SV_DATAID, RS_PP_Pause, 0);
						GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE, WDT_STA_SV_DATAID, RS_PP_Contine, 0);
			
						GUI_ENC_API_SetValue(ID_STA_PP_PAUSE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
						GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
			
						GUI_ENC_API_SetVisible(ID_STA_PP, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_PP_PAUSE, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_PP_PLAYTIME, WS_VISIBLE);
						
						GUI_ENC_API_Update(ID_STA_PP, 1);
						GUI_ENC_API_Update(ID_STA_PP_PAUSE, 1);
						GUI_ENC_API_Update(ID_STA_PP_CONTINUE, 1);
						GUI_ENC_API_Update(ID_STA_PP_PLAYTIME, 1);
			
						FULLTOTVOD_SetCurState(TVOD_PAUSE_STATE);
					}
					else if (bIsLeft == FYF_TRUE) //houtui
					{
						VodPlayPara.reqScale = -2;
			
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							OW_ERROR("OW_VOD_MainMenu out from RTSP_CreatePLAY	err!!\n ");
							OW_SetExitFlag(FYF_TRUE);
							return;
						}
			
						//bIsLeft = FYF_FALSE;
						bIsSelect = FYF_FALSE;
						//FULLTOTVOD_SetCurState(FAST_STATE);
			
						GUI_ENC_API_SetVisible(ID_STA_INFOBP, WS_VISIBLE);	
						GUI_ENC_API_SetVisible(ID_STA_INBP, WS_VISIBLE);	
						GUI_ENC_API_Update(ID_STA_INFOBP, 1);
						GUI_ENC_API_Update(ID_STA_INBP, 1);
							
						FYF_API_thread_sleep(2000);
						FULLTOTVOD_ClearScreen();
						FULLTOTVOD_SetCurState(FAST_STATE);
					}
					bHadRes = FYF_TRUE; 		//取得服务器前端分配资源
					
					ipanel_porting_video_location(-35, -25, 720, 576);
					
					if ( 0 == VodPlayPara.pVPid)
						VodPlayPara.pVPid = 0x1fff;
					if ( 0 == VodPlayPara.pAPid)
						VodPlayPara.pAPid = 0x1fff;
					//if ( 0 == VodPlayPara.PcrPid)
						//VodPlayPara.PcrPid = 0x1fff;
					
					ret = APP_IPI_PlayProg(VodPlayPara.pVPid, VodPlayPara.pAPid, VodPlayPara.PcrPid, VodPlayPara.mVideoType, VodPlayPara.mAudioType);
					if(FYF_OK != ret)
					{
						OW_ERROR("APP_IPI_PlayProg failed! 0x%x\n", ret);
						OW_SetExitFlag(FYF_TRUE);
						
					}
					
					//OW_DLRTPInfo(VodPlayPara.pFre, VodPlayPara.pPmtPid);
					bTVODPlay = FYF_TRUE;
					if (bFromF10)
					{
						FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
						
						GUI_ENC_API_SetVisible(ID_STA_INFOBP, WS_VISIBLE);	
						GUI_ENC_API_SetVisible(ID_STA_INBP, WS_VISIBLE);	
						GUI_ENC_API_Update(ID_STA_INFOBP, 1);
						GUI_ENC_API_Update(ID_STA_INBP, 1);
						
						FYF_API_thread_sleep(2000);
						FULLTOTVOD_ClearScreen();
					}
					bFromF10 = FYF_FALSE;
			
					if (bIsLeft == FYF_TRUE) 
					{				
						GUI_ENC_API_SetVisible(ID_STA_FAST, WS_VISIBLE);	
						GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_VISIBLE);	
						GUI_ENC_API_Update(ID_STA_FAST, 1);
						GUI_ENC_API_Update(ID_ODC_FAST, 1);
						g_init = FYF_TRUE;
						bIsLeft = FYF_FALSE;
						bPressLeftKey = FYF_TRUE;
						g_isleft = FYF_TRUE;
						GUI_ENC_API_SetFocus(ID_ODC_FAST);
					}
				}
			}

		}
		else
		{

			if (OW_GetExitFlag() == FYF_FALSE)
			{
				if (bIsSelect == FYF_TRUE && !bFromF10) //确认键进入,要暂停
				{
					VodPlayPara.reqScale = 0;
					if (RTSP_CreatePAUSE(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_VOD_MainMenu out from RTSP_CreatePAUSE  err!!\n ");
						OW_SetExitFlag(FYF_TRUE);
					}
					else
					{
						OW_DrawSpeed(VodPlayPara.reqScale);
						bPauseDisPly = FYF_TRUE;
						FYF_API_thread_sleep(500);
					}
					FULLTOTVOD_ClearScreen();
					
					RTSP_GetSecondByClock(VodPlayPara.clockCur, &curTime, &year, &month, &day, FYF_FALSE);
					printf("curtime = %d, year = %d, month = %d, day = %d\n", curTime, year, month, day);
					RTSP_ParseSecToHourMin(curTime, &hour, &min, &sec);
					printf("curtime = %d, hour = %d, min = %d, sec = %d\n", curTime, hour, min, sec);
					RTSP_ParseNumTimeToStr(year, month, day, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr, secStr);
					printf("hourStr = %s, minStr = %s, secStr = %s\n", hourStr, minStr, secStr);
					memcpy(FirsttimeStr, "播放时间:", strlen("播放时间:"));
					sprintf(FirsttimeStr + strlen("播放时间:"), "%s:%s:%s", hourStr, minStr, secStr);
					printf("\033[0;31m FirsttimeStr = %s\033[0m\n", FirsttimeStr);
					
					GUI_ENC_API_SetValue(ID_STA_PP_PLAYTIME, WDT_STA_SV_STRING, FirsttimeStr, 0);
					GUI_ENC_API_SetValue(ID_STA_PP_PAUSE, WDT_STA_SV_DATAID, RS_PP_Pause, 0);
					GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE, WDT_STA_SV_DATAID, RS_PP_Contine, 0);
		
					GUI_ENC_API_SetValue(ID_STA_PP_PAUSE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
					GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
		
					GUI_ENC_API_SetVisible(ID_STA_PP, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_PP_PAUSE, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_PP_PLAYTIME, WS_VISIBLE);
					
					GUI_ENC_API_Update(ID_STA_PP, 1);
					GUI_ENC_API_Update(ID_STA_PP_PAUSE, 1);
					GUI_ENC_API_Update(ID_STA_PP_CONTINUE, 1);
					GUI_ENC_API_Update(ID_STA_PP_PLAYTIME, 1);
		
					FULLTOTVOD_SetCurState(TVOD_PAUSE_STATE);
				}
				else if (bIsLeft == FYF_TRUE) //houtui
				{
					VodPlayPara.reqScale = -2;
		
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_VOD_MainMenu out from RTSP_CreatePLAY	err!!\n ");
						OW_SetExitFlag(FYF_TRUE);
						return;
					}
		
					bIsSelect = FYF_FALSE;
		
					GUI_ENC_API_SetVisible(ID_STA_INFOBP, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_STA_INBP, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_INFOBP, 1);
					GUI_ENC_API_Update(ID_STA_INBP, 1);
						
					FYF_API_thread_sleep(2000);
					FULLTOTVOD_ClearScreen();
					FULLTOTVOD_SetCurState(FAST_STATE);
				}
				bHadRes = FYF_TRUE; 		//取得服务器前端分配资源
				
				ipanel_porting_video_location(-35, -25, 720, 576);
									
				//OW_DLRTPInfo(VodPlayPara.pFre, VodPlayPara.pPmtPid);
				bTVODPlay = FYF_TRUE;
				if (bFromF10)
				{
					FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
					
					GUI_ENC_API_SetVisible(ID_STA_INFOBP, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_STA_INBP, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_INFOBP, 1);
					GUI_ENC_API_Update(ID_STA_INBP, 1);
					
					FYF_API_thread_sleep(2000);
					FULLTOTVOD_ClearScreen();
				}
				bFromF10 = FYF_FALSE;
		
				if (bIsLeft == FYF_TRUE) 
				{				
					GUI_ENC_API_SetVisible(ID_STA_FAST, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_FAST, 1);
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
					g_init = FYF_TRUE;
					bIsLeft = FYF_FALSE;
					bPressLeftKey = FYF_TRUE;
					g_isleft = FYF_TRUE;
					GUI_ENC_API_SetFocus(ID_ODC_FAST);
				}
			}
			

		}
		
				

		return;
	}
#endif	
}


void UI_APP_BPJUMP_OnPaint(BU32 hdc,BU32 lParam)
{
#ifdef USE_FZVOD	

	BU08 showStr[30]={0};

	if (OW_GetExitFlag())
	{
		if (!bInTvodTimer && (GUI_IpanelState != GUI_ENC_API_GetCurState()))
		{
			if (VodPlayPara.VodOrTvod == 1 && bFromF10)
			{
				
				FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
				FYF_API_OSD_Refresh(0, 0, 720, 576);
				OW_ERROR("服务器忙,请稍后再试\n");
				GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "服务器忙,请稍后再试", 0);
				GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
				
				FYF_API_thread_sleep(1500);
				
				FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
				FYF_API_OSD_Refresh(0, 0, 720, 576);
				return;
			}
		}
	}

	sprintf(showStr,"20%02d-%02d-%02d  %02d:%02d:%02d",g_tvod_startYear,g_tvod_startMonth, g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
	//sprintf(showStr,"%02d-%02d-%02d",g_vod_startHour,g_vod_startMin,g_vod_startsecd);
	GUI_API_DrawStyle(hdc, 0, 0, STYLE_JUMPTOBK_W230_H40);	
	if(g_tvod_offset>=0&&g_tvod_offset<2)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+2)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);			
	}
	else if(g_tvod_offset>=2&&g_tvod_offset<4)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+3)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_tvod_offset>=4&&g_tvod_offset<6)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+4)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_tvod_offset>=6&&g_tvod_offset<8)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+6)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_tvod_offset>=8&&g_tvod_offset<10)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+7)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_tvod_offset>=10&&g_tvod_offset<12)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+8)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}

	GUI_API_TextOut(hdc, 0, 8, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);
#endif
}


void UI_APP_BPJUMP_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	

	int preReqScale = 0;
	int ret = -1;
	int CurSecs = 0;
	char adjTimeStr[30] = { 0 };
	int startSecs = 0;
	int endSecs = 0;
	BU08 bRunNian = FYF_FALSE;
	char yearStr[5] = { 0 };
	char monthStr[3] = { 0 };
	char dayStr[3] = { 0 };
	char hourStr[3] = { 0 };
	char minStr[3] = { 0 };
	char secStr[3] = { 0 };
	BU16 curYear = 0;
	BU08 curMonth = 0;	
	BU08 curDay = 0;
	BU08 hour = 0;
	BU08 min = 0;
	BU08 sec = 0;
	APP_Date_S DateAndTime;
	
	BU08 bDataErr = FYF_FALSE;			//时间格式错误
	BU08 bDataOutRang = FYF_TRUE;		//时间超出范围
	BU08 bDataLarger = FYF_FALSE;		//时间格式错误
	BU08 bDataSmall = FYF_FALSE;			//时间超出范围
	
	printf("Press Key.==%d===============UI_APP_BPJUMP_ODC_Key\n",key);
	
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
			switch(g_tvod_offset)
			{
			case 0:
				g_tvod_startYear = (BU08)((BU16)g_tvod_startYear%10 + key*10);
				break;
			case 1:
				g_tvod_startYear = (BU08)((BU16)g_tvod_startYear-g_tvod_startYear%10 + key);
				break;
			case 2:
				g_tvod_startMonth= (BU08)((BU16)g_tvod_startMonth%10 + key*10);
				break;
			case 3:
				g_tvod_startMonth = (BU08)((BU16)g_tvod_startMonth-g_tvod_startMonth%10 + key);
				break;
			case 4:
				g_tvod_startDay= (BU08)((BU16)g_tvod_startDay%10 + key*10);
				break;
			case 5:
				g_tvod_startDay = (BU08)((BU16)g_tvod_startDay-g_tvod_startDay%10 + key);
				break;
	
			case 6:
				g_tvod_startHour = (BU08)((BU16)g_tvod_startHour%10 + key*10);
				break;
			case 7:
				g_tvod_startHour = (BU08)((BU16)g_tvod_startHour-g_tvod_startHour%10 + key);
				break;
			case 8:
				g_tvod_startMin = (BU08)((BU16)g_tvod_startMin%10 + key*10);
				break;
			case 9:
				g_tvod_startMin = (BU08)((BU16)g_tvod_startMin-g_tvod_startMin%10 + key);
				break;
			case 10:
				g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd%10 + key*10);
				break;
			case 11:
				g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd-g_tvod_startsecd%10 + key);
				break;
			default:
				break;
			}

			if(g_tvod_startMonth >=13)	
				g_tvod_startMonth = 0;

			if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
				g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
			{
				if(g_tvod_startDay > 31)	
					g_tvod_startDay = 0;
			}
			else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
			{
				if(g_tvod_startDay > 30)	
					g_tvod_startDay = 0;
			}
			else if (g_tvod_startMonth == 2)
			{
				if ((2000 + g_tvod_startYear) % 4 == 0)
				{
					if ((2000 + g_tvod_startYear) % 100 != 0)
					{
						if(g_tvod_startDay > 29)	
							g_tvod_startDay = 0;
					}
					else if ((2000 + g_tvod_startYear) % 400 == 0)
					{
						if(g_tvod_startDay > 29)	
							g_tvod_startDay = 0;
					}
					else
					{
						if(g_tvod_startDay > 28)	
							g_tvod_startDay = 0;
					}
				}
				else
				{
					if(g_tvod_startDay > 28)	
						g_tvod_startDay = 0;
				}				
			}

			if(g_tvod_startHour>=24)	
				g_tvod_startHour = 0;
			if(g_tvod_startMin>=60) 
				g_tvod_startMin = 0;
			if(g_tvod_startsecd>=60)	
				g_tvod_startsecd = 0;
			
			g_tvod_offset++;
			if(g_tvod_offset>11)	
				g_tvod_offset=0;
			GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);
		break;

		
		case GUI_VK_LEFT:
				if(g_tvod_offset==0)	
					g_tvod_offset=11;
				else 
					g_tvod_offset--;
				GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);
			break;
		case GUI_VK_RIGHT:
				g_tvod_offset++;
				if(g_tvod_offset > 11)	
					g_tvod_offset=0;
				GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);
			break;


		case GUI_VK_UP:
		
					switch(g_tvod_offset)
					{
					case 0:
						g_tvod_startYear = (BU08)((BU16)g_tvod_startYear%10 + (g_tvod_startYear/10 + 1)*10);
						break;
					case 1:
						g_tvod_startYear = (BU08)((BU16)g_tvod_startYear + 1);
						break;
					case 2:
						g_tvod_startMonth= (BU08)((BU16)g_tvod_startMonth%10 + (g_tvod_startMonth/10 + 1)*10);
						break;
					case 3:
						g_tvod_startMonth = (BU08)((BU16)g_tvod_startMonth + 1);
						break;
					case 4:
						g_tvod_startDay= (BU08)((BU16)g_tvod_startDay%10 + (g_tvod_startDay/10 + 1)*10);
						break;
					case 5:
						g_tvod_startDay = (BU08)((BU16)g_tvod_startDay + 1);
						break;
			
					case 6:
						g_tvod_startHour = (BU08)((BU16)g_tvod_startHour%10 + (g_tvod_startHour/10 + 1)*10);
						break;
					case 7:
						g_tvod_startHour = (BU08)((BU16)g_tvod_startHour + 1);
						break;
					case 8:
						g_tvod_startMin = (BU08)((BU16)g_tvod_startMin%10 + (g_tvod_startMin/10 + 1)*10);
						break;
					case 9:
						g_tvod_startMin = (BU08)((BU16)g_tvod_startMin + 1);
						break;
					case 10:
						g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd%10 + (g_tvod_startsecd/10 + 1)*10);
						break;
					case 11:
						g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd + 1);
						break;
					default:
						break;
					}
		
		
		printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d,min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
					if(g_tvod_startYear>=100)	
					{
						g_tvod_startYear = g_tvod_startYear % 100;
					}
		
					if(g_tvod_startMonth >=13)	
						g_tvod_startMonth = 1;
		
					if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
						g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
					{
						if(g_tvod_startDay > 31)	
							g_tvod_startDay = 1;
					}
					else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
					{
						if(g_tvod_startDay > 30)	
							g_tvod_startDay = 1;
					}
					else if (g_tvod_startMonth == 2)
					{
						if ((2000 + g_tvod_startYear) % 4 == 0)
						{
							if ((2000 + g_tvod_startYear) % 100 != 0)
							{
								if(g_tvod_startDay > 29)	
									g_tvod_startDay = 1;
							}
							else if ((2000 + g_tvod_startYear) % 400 == 0)
							{
								if(g_tvod_startDay > 29)	
									g_tvod_startDay = 1;
							}
							else
							{
								if(g_tvod_startDay > 28)	
									g_tvod_startDay = 1;
							}
						}
						else
						{
							if(g_tvod_startDay > 28)	
								g_tvod_startDay = 1;
						}				
					}
					else if (g_tvod_startMonth == 0)
					{
						if(g_tvod_startDay > 30)	
							g_tvod_startDay = 1;
					}
		
					if(g_tvod_startHour>=24)	
						g_tvod_startHour = 0;
					if(g_tvod_startMin>=60) 
						g_tvod_startMin = 0;
					if(g_tvod_startsecd>=60)	
						g_tvod_startsecd = 0;
		
		
		printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d,min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
					GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);
				break;
		
			
			case GUI_VK_DOWN:
				
				switch(g_tvod_offset)
				{
				case 0:
					g_tvod_startYear = (BU08)((BU16)g_tvod_startYear%10 + (g_tvod_startYear/10 - 1)*10);
					break;
				case 1:
					g_tvod_startYear = (BU08)((BU16)g_tvod_startYear - 1);
					break;
				case 2:
					g_tvod_startMonth= (BU08)((BU16)g_tvod_startMonth%10 + (g_tvod_startMonth/10 - 1)*10);
					if(g_tvod_startMonth >=245 && g_tvod_startMonth <=255)	
					{
						g_tvod_startMonth = 12;
					}
					break;
				case 3:
					g_tvod_startMonth = (BU08)((BU16)g_tvod_startMonth - 1);
					if(g_tvod_startMonth >=245 && g_tvod_startMonth <=255)	
					{
						g_tvod_startMonth = 12 - (255 - g_tvod_startMonth);
					}
					else if (g_tvod_startMonth == 0)
					{
						g_tvod_startMonth = 12;
					}
						
					break;
				case 4:
					g_tvod_startDay= (BU08)((BU16)g_tvod_startDay%10 + (g_tvod_startDay/10 - 1)*10);
					
					if(g_tvod_startDay >=245 && g_tvod_startDay <=255)	
					{
						
						if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
							g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
						{
							g_tvod_startDay = 31;
						}
						else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
						{
							g_tvod_startDay = 30;
						}
						else if (g_tvod_startMonth == 2)
						{
							if ((2000 + g_tvod_startYear) % 4 == 0)
							{
								if ((2000 + g_tvod_startYear) % 100 != 0)
								{
									g_tvod_startDay = 29;
								}
								else if ((2000 + g_tvod_startYear) % 400 == 0)
								{
									g_tvod_startDay = 29;
								}
								else
								{
									g_tvod_startDay = 28;
								}
							}
							else
							{
								g_tvod_startDay = 28;
							}				
						}

					}
					else if (g_tvod_startDay == 0)
					{
						
						if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
							g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
						{
							g_tvod_startDay = 31;
						}
						else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
						{
							g_tvod_startDay = 30;
						}
						else if (g_tvod_startMonth == 2)
						{
							if ((2000 + g_tvod_startYear) % 4 == 0)
							{
								if ((2000 + g_tvod_startYear) % 100 != 0)
								{
									g_tvod_startDay = 29;
								}
								else if ((2000 + g_tvod_startYear) % 400 == 0)
								{
									g_tvod_startDay = 29;
								}
								else
								{
									g_tvod_startDay = 28;
								}
							}
							else
							{
								g_tvod_startDay = 28;
							}				
						}
						
					}
					break;
				case 5:
					g_tvod_startDay = (BU08)((BU16)g_tvod_startDay - 1);

					
					if(g_tvod_startDay >=245 && g_tvod_startDay <=255)	
					{
						
						if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
							g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
						{
							g_tvod_startDay = 31 - (255 - g_tvod_startDay);
						}
						else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
						{
							g_tvod_startDay = 30 - (255 - g_tvod_startDay);
						}
						else if (g_tvod_startMonth == 2)
						{
							if ((2000 + g_tvod_startYear) % 4 == 0)
							{
								if ((2000 + g_tvod_startYear) % 100 != 0)
								{
									g_tvod_startDay = 29 - (255 - g_tvod_startDay);
								}
								else if ((2000 + g_tvod_startYear) % 400 == 0)
								{
									g_tvod_startDay = 29 - (255 - g_tvod_startDay);
								}
								else
								{
									g_tvod_startDay = 28 - (255 - g_tvod_startDay);
								}
							}
							else
							{
								g_tvod_startDay = 28 - (255 - g_tvod_startDay);
							}				
						}
					}
					else if (g_tvod_startDay == 0)
					{
						
						if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
							g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
						{
							g_tvod_startDay = 31;
						}
						else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
						{
							g_tvod_startDay = 30;
						}
						else if (g_tvod_startMonth == 2)
						{
							if ((2000 + g_tvod_startYear) % 4 == 0)
							{
								if ((2000 + g_tvod_startYear) % 100 != 0)
								{
									g_tvod_startDay = 29;
								}
								else if ((2000 + g_tvod_startYear) % 400 == 0)
								{
									g_tvod_startDay = 29;
								}
								else
								{
									g_tvod_startDay = 28;
								}
							}
							else
							{
								g_tvod_startDay = 28;
							}				
						}
						
					}



					break;
				
				case 6:
					g_tvod_startHour = (BU08)((BU16)g_tvod_startHour%10 + (g_tvod_startHour/10 - 1)*10);
					
					if(g_tvod_startHour >=245 && g_tvod_startHour <=255)	
					{
						g_tvod_startHour = 23;
						
					}
					break;
				case 7:
					g_tvod_startHour = (BU08)((BU16)g_tvod_startHour - 1);
					
					if(g_tvod_startHour >=245 && g_tvod_startHour <=255)	
					{
						g_tvod_startHour = 23 - (255 - g_tvod_startHour);
						
					}
					break;
				case 8:
					g_tvod_startMin = (BU08)((BU16)g_tvod_startMin%10 + (g_tvod_startMin/10 - 1)*10);
					
					if(g_tvod_startMin >=245 && g_tvod_startMin <=255)	
					{
						g_tvod_startMin = 59;
						
					}
					break;
				case 9:
					g_tvod_startMin = (BU08)((BU16)g_tvod_startMin - 1);
					
					if(g_tvod_startMin >=245 && g_tvod_startMin <=255)	
					{
						g_tvod_startMin = 59 - (255 - g_tvod_startMin);
						
					}
					break;
				case 10:
					g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd%10 + (g_tvod_startsecd/10 - 1)*10);
					
					if(g_tvod_startsecd >=245 && g_tvod_startsecd <=255)	
					{
						g_tvod_startsecd = 59;
					}
					break;
				case 11:
					g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd - 1);
					
					if(g_tvod_startsecd >=245 && g_tvod_startsecd <=255)	
					{
						g_tvod_startsecd = 59 - (255 - g_tvod_startsecd);
					}
					break;
				default:
					break;
				}
		
		
				
				if(g_tvod_startYear >=245 && g_tvod_startYear <=255)	
				{
					g_tvod_startYear = 99 - (255 - g_tvod_startYear);
					
				}
				printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d, min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
				

				printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d, min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);

				if(g_tvod_startMonth >=13)	
					g_tvod_startMonth = 1;
				
				if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
					g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
				{
					if(g_tvod_startDay > 31)	
						g_tvod_startDay = 1;
				}
				else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
				{
					if(g_tvod_startDay > 30)	
						g_tvod_startDay = 1;
				}
				else if (g_tvod_startMonth == 2)
				{
					if ((2000 + g_tvod_startYear) % 4 == 0)
					{
						if ((2000 + g_tvod_startYear) % 100 != 0)
						{
							if(g_tvod_startDay > 29)	
								g_tvod_startDay = 1;
						}
						else if ((2000 + g_tvod_startYear) % 400 == 0)
						{
							if(g_tvod_startDay > 29)	
								g_tvod_startDay = 1;
						}
						else
						{
							if(g_tvod_startDay > 28)	
								g_tvod_startDay = 1;
						}
					}
					else
					{
						if(g_tvod_startDay > 28)	
							g_tvod_startDay = 1;
					}				
				}
				else if (g_tvod_startMonth == 0)
				{
					if(g_tvod_startDay > 30)	
						g_tvod_startDay = 1;
				}

				if(g_tvod_startHour>=24)	
					g_tvod_startHour = 0;
				if(g_tvod_startMin>=60) 
					g_tvod_startMin = 0;
				if(g_tvod_startsecd>=60)	
					g_tvod_startsecd = 0;
				
				printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d, min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
		
				GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);
				break;
		

			
		case GUI_VK_SELECT:

			startSecs = g_tvod_tmp_startSecs;
			endSecs = g_tvod_tmp_endSecs;
			curYear = g_tvod_startYear + 2000;
			curMonth = (BU08)g_tvod_startMonth;
			curDay = (BU08) g_tvod_startDay;
			hour = (BU08)g_tvod_startHour;
			min = (BU08)g_tvod_startMin;
			sec = (BU08) g_tvod_startsecd;


			if (curMonth <= 0 || curDay <= 0)
			{
				if (Ipanel_GetEntrance_Flag())
				{
					OW_DEBUG("错误的时间格式\n");
				}
				else
				{	
					OW_DEBUG("错误的时间格式\n");
				}
				
				GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET, WDT_STA_SV_STRING, "错误的时间格式", 0);
				GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
				
				FYF_API_thread_sleep(2000);
				
												
				GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_DISABLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_DISABLE);
				GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_CHECKNET, 1);
				
				FYF_API_OSD_FillRect(222, 236, 300, 60, 0);
				FYF_API_OSD_Refresh(222, 236, 300, 60);

				return;

		   }
			
			if (curYear % 4 == 0)
			{
				if (curYear % 100 != 0)
				{
					bRunNian = FYF_TRUE;
				}
				else if (curYear % 400 == 0)
				{
					bRunNian = FYF_TRUE;
				}
			}
			
			if ((int)curMonth > 12 || (int)hourStr > 23 || ((int)minStr > 59) || ((int)secStr > 59) )
			{
				bDataErr = FYF_TRUE;
			}
			else if (curMonth == 2)
			{
				if (bRunNian)
				{
					if (curDay > 29)
						bDataErr = FYF_TRUE;
				}
				else if (curDay > 28)
				{
					bDataErr = FYF_TRUE;
				}
			}
			else if (curMonth == 1 || curMonth == 3 || curMonth == 5 || curMonth == 7 || curMonth == 8 || curMonth == 10
				|| curMonth == 12)
			{
				if (curDay > 31)
					bDataErr = FYF_TRUE;
			}
			else
			{
				if (curDay > 30)
					bDataErr = FYF_TRUE;
			}
			
			if (hour > 23 || min > 59 || sec > 59)
			{
				bDataErr = FYF_TRUE;
			}
		
			if (bDataErr)
			{
				if (VodPlayPara.pVPid != -1 && VodPlayPara.pAPid != -1)
				{
					printf("\033[0:31m 已超出直播时间点\033[0m\n");
					
					if (Ipanel_GetEntrance_Flag())
					{
						printf("\033[0:31m 返回到浏览器\033[0m\n");
					}
					else
					{	
						printf("\033[0:31m 进入直播\033[0m\n");
					}
					
					FYF_API_OSD_FillRect(0, 0, 720, 576,0);
					FYF_API_OSD_Refresh(0, 0, 720, 576);
					OW_SetExitFlag(FYF_TRUE);
					
					FULLTOTVOD_SetCurState(BPZHUN_STATE);
					GUI_ENC_API_SetFocus(ID_ODC_BPZHUN);
					
					GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
					GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			

					FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
					FYF_API_OSD_Refresh(0, 0, 720, 576);
					GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
					GUI_ENC_API_Update(ID_ODC_BPZHUN, 1);			
					GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);

					return;
				}
				else
				{
					
					printf("\033[0:31m 超出时移范围\033[0m\n");
					printf("\033[0:31m 从时移起点开始播放\033[0m\n");
					
					VodPlayPara.reqScale = 1;
					VodPlayPara.bBOS = FYF_TRUE;
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
					}
					bPlayFromStart = FYF_TRUE;
		
					
					FYF_API_OSD_FillRect(0, 0, 720, 576,0);
					FYF_API_OSD_Refresh(0, 0, 720, 576);
					
					FULLTOTVOD_SetCurState(BPZHUN_STATE);
					GUI_ENC_API_SetFocus(ID_ODC_BPZHUN);
					
					GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
					GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			

					FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
					FYF_API_OSD_Refresh(0, 0, 720, 576);
					GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
					GUI_ENC_API_Update(ID_ODC_BPZHUN, 1);			
					GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);

					return;
				}
			
			}
			
			RTSP_CauAllSecs(curYear, curMonth, curDay, hour, min, sec, &CurSecs);
			
			//printf("g_tvod_tmp_startSecs = %d , g_tvod_tmp_endsec = %d, CurSecs = %d\n", g_tvod_tmp_startSecs, g_tvod_tmp_endSecs, CurSecs);
			if ((CurSecs >= startSecs) && (CurSecs <= endSecs))
			{
				bDataOutRang = FYF_FALSE;
				printf("bDataOutRang = FYF_FALSE\n");
			}
			else
			{
				if (CurSecs < startSecs)
				{
					bDataSmall = FYF_TRUE;
					
					printf("bDataSmall = FYF_TRUE\n");
				}
				else if (CurSecs > endSecs)
				{
					bDataLarger = FYF_TRUE;
					printf("bDataLarger = FYF_TRUE\n");
				}
			}
			if (bDataOutRang)
			{
				if (bDataSmall)
				{
					FYF_API_OSD_FillRect(0, 0, 720, 576,0);
					FYF_API_OSD_Refresh(0, 0, 720, 576);
			
					if (bTVODPlay)
					{
						OW_ERROR("已超出时移起点");
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutTimeshiftStart, 0);
					}	
					else
					{
						OW_ERROR("超出时移范围");
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "超出时移范围", 0);
					}
			
					OW_ERROR("从时移起点开始播放");
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_PlayFromTFStart, 0);

					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

							
					GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
					GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
					GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
			
					FYF_API_thread_sleep(2000);
			
					FYF_API_OSD_FillRect(0, 0, 720, 576,0);
					FYF_API_OSD_Refresh(0, 0, 720, 576);
										
					VodPlayPara.reqScale = 1;
					VodPlayPara.bBOS = FYF_TRUE;
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
					}
					else
					{
						RTSP_PassBadFrame();
					}
			
					bPlayFromStart = FYF_TRUE;
				
					printf("err input clock2222 is %s \n", VodPlayPara.clockCur);
				
					FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
				
					GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);
					GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);
					GUI_ENC_API_SetVisible(ID_STA_BPJUMP, WS_DISABLE);
					GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
					GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
					GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
					GUI_ENC_API_Update(ID_STA_BPJUMP, 1);
					GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);
				
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
			
					FULLTOTVOD_SetCurState(BPZHUN_STATE);
					GUI_ENC_API_SetFocus(ID_ODC_BPZHUN);
					
					GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
					GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			

					FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
					FYF_API_OSD_Refresh(0, 0, 720, 576);
					GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
					GUI_ENC_API_Update(ID_ODC_BPZHUN, 1);			
					GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);
					return;
				}
				else if (bDataLarger)
				{				
					if (bTVODPlay)
					{
						FYF_API_OSD_FillRect(0, 0, 720, 576,0);
						FYF_API_OSD_Refresh(0, 0, 720, 576);
						
						OW_ERROR("已超出直播时间点");
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutLiveTime, 0);

						if (Ipanel_GetEntrance_Flag())
						{
							OW_ERROR("返回到浏览器");
							GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "返回到浏览器", 0);
						}	
						else
						{
							OW_ERROR("进入直播");
							GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_ReturnToLive, 0);
						}	
						
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
						
						GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
						GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
						GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
						
						FYF_API_thread_sleep(2000);
						
						
						GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_STA_BPJUMP, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
						GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
						GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
						GUI_ENC_API_Update(ID_STA_BPJUMP, 1);
						GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);

						FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
						GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
						
						OW_SetExitFlag(FYF_TRUE);
						
						FYF_API_OSD_FillRect(0, 0, 720, 576,0);
						FYF_API_OSD_Refresh(0, 0, 720, 576);
						printf("err input clock is %s \n", VodPlayPara.clockCur);
						
						FULLTOTVOD_SetCurState(BPZHUN_STATE);
						GUI_ENC_API_SetFocus(ID_ODC_BPZHUN);
						
						GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
						GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			

						FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
						FYF_API_OSD_Refresh(0, 0, 720, 576);
						GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
						GUI_ENC_API_Update(ID_ODC_BPZHUN, 1);			
						GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);

						return;
				
					}
					else
					{				
					
						FYF_API_OSD_FillRect(0, 0, 720, 576,0);
						FYF_API_OSD_Refresh(0, 0, 720, 576);
						printf("\033[0:31m 超出时移范围\033[0m\n");
						printf("\033[0:31m 从时移起点开始播放\033[0m\n");
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_STRING, "超出时移范围", 0);
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_PlayFromTFStart, 0);
						
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

						GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
						GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
						GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
						
						FYF_API_thread_sleep(2000);

						
						GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_STA_BPJUMP, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
						GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
						GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
						GUI_ENC_API_Update(ID_STA_BPJUMP, 1);
						GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);


						FYF_API_OSD_FillRect(0, 0, 720, 576,0);
						FYF_API_OSD_Refresh(0, 0, 720, 576);

						
						FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
						GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);						

						VodPlayPara.reqScale = 1;
						VodPlayPara.bBOS = FYF_TRUE;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
						}
						else
						{
							RTSP_PassBadFrame();
						}
						bPlayFromStart = FYF_TRUE;
						
						FULLTOTVOD_SetCurState(BPZHUN_STATE);
						GUI_ENC_API_SetFocus(ID_ODC_BPZHUN);
						
						GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
						GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			

						FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
						FYF_API_OSD_Refresh(0, 0, 720, 576);
						GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
						GUI_ENC_API_Update(ID_ODC_BPZHUN, 1);			
						GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);

						return;
				
					}
				}
			}

			FYF_API_OSD_FillRect(0, 0, 720, 576,0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			
			VodPlayPara.reqScale = 1;
			RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
				secStr);
			sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
			
			printf("\033[0:31m input Curtime = %s\033[0m\n", adjTimeStr);
			
			memset(VodPlayPara.clockCur, 0, sizeof(VodPlayPara.clockCur));
			sprintf(VodPlayPara.clockCur, "%s%s%sT%s%s%s.00Z", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
			printf("success input clock is %s \n", VodPlayPara.clockCur);
			
			if (!bPlayFromStart && RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
			{
				RTSP_CheckNetOk();
				OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
			}
			else
			{
				RTSP_PassBadFrame();
			}
			bPlayFromStart = FYF_FALSE;


			FYF_API_OSD_FillRect(0, 0, 720, 576,0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			
			
			FULLTOTVOD_SetCurState(BPZHUN_STATE);
			GUI_ENC_API_SetFocus(ID_ODC_BPZHUN);
			
			GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
			GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			
			
			FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_VISIBLE);
			GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
			GUI_ENC_API_Update(ID_ODC_BPZHUN, 1);			
			GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);

			break;
			
		case GUI_VK_EXIT:

			
			GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
			GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			

			FYF_API_OSD_FillRect(0, 0, 720, 576,0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			
			bIntvod = FYF_FALSE;
				
			printf("not send==============\n");
				
			OW_SetExitFlag(FYF_TRUE);
			bfirst = 1;
			bGuiTimerSuspend = FYF_FALSE;
			bPauseDisPly = FYF_FALSE;
			bFromF10 = FYF_FALSE;
			bIsSelect = FYF_FALSE;
			bIsLeft = FYF_FALSE;
			break;

			
		default:
			break;	

			
	}
#endif
}



void UI_APP_TVOD_CLEANSCREEN_TIME(void)
{	
#ifdef USE_FZVOD	

	BU32 curstate;
	curstate = FULLTOTVOD_GetCurState();
	
	printf("UI_APP_TVOD_CLEANSCREEN_TIME. curstate = %d\n", curstate);

	switch (curstate)
	{
		
		case FULLTOTVOD_NULL_STATE:
			break;
			
		case BPJUMP_STATE:
			
			FYF_API_OSD_FillRect(0, 0, 720, 576,0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			bIntvod = FYF_FALSE;
				
			printf("not send==============\n");
				
			if (bHadRes)
			{
				if (serSetParaType != 0 && RTSP_CreateTEARDOWN(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					OW_ERROR("OW_VOD_MainMenu RTSP_CreateTEARDOWN  err!!\n ");
				}
				bHadRes = FYF_FALSE;				//释放服务器前端分配资源
				if (bTVODPlay)
				{
					APP_Com_API_StopProg(0);
					//APP_IPI_StopAudioVideo();
				}
			
			}
			OW_SetExitFlag(FYF_TRUE);
			if (optionSocket != -1)
			{
				close(optionSocket);
				optionSocket = -1;
				optionTimeCount = 0;
			}
										
			Ow_Set_entervodortvod_Flag(FYF_FALSE);
									
									
			OW_DEBUG("OW_TVOD_MainMenu End.\n");
								
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			FYF_API_demux_set_notify(CallBackGetTableFunc);
			GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
			//APP_Com_IPI_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), 1);
			bfirst = 1;
			bGuiTimerSuspend = FYF_FALSE;
			bPauseDisPly = FYF_FALSE;
			bFromF10 = FYF_FALSE;
			bIsSelect = FYF_FALSE;
			bIsLeft = FYF_FALSE;
			break;
			
		case BPZHUN_STATE:
			break;
		
		case TVOD_PLAY_STATE:
			break;
		
		case PP_EXIT_STATE:
			break;
		
		case FAST_STATE:

			break;
			
		case TVOD_JUMPTO_STATE:
			GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
			GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
			FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
			GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
			
			FULLTOTVOD_ClearScreen();
			break;
			
		case TVOD_JUMPBAR_STATE:
			GUI_ENC_API_SetVisible(ID_STA_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_ODC_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_PBR_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_ODC_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_PBR_JUMPBAR, 1);
			
			FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
			
			GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
			
			FULLTOTVOD_ClearScreen();
			
			g_tvod_startYear = 0;
			g_tvod_startMonth = 0;
			g_tvod_startDay = 0;
			g_tvod_startHour = 0;
			g_tvod_startMin = 0;
			g_tvod_startsecd = 0;
			break;
			
		case TVOD_PAUSE_STATE:
			break;
			
		case IPANEL_TO_TVOD_STATE:
			break;
			
		case TVOD_VOL_STATE:
			FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
			GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
			FULLTOTVOD_ClearScreen();
			break;

		default:
			break;

		
	}
	
	if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
	{
		GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
		GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
		GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
		GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
		APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
	}
#endif
}


void UI_APP_FAST_OnPaint(BU32 hdc,BU32 lparam)
{
#ifdef USE_FZVOD	

	BU08 showStr[100]={0};
	printf("UI_APP_FAST_OnPaint lparam = %d, g_init = %d\n", lparam, g_init);

	if (GUI_ENC_API_IsFocus(ID_DLG_FULLTOTVOD))
	{
		printf("ID_DLG_FULLTOTVOD is focus==========\n");
		return;
	}

	if (g_init == FYF_TRUE)
	{		
		if (bPressLeftKey == FYF_TRUE)
		{
			sprintf(showStr,"快退-%2d", leftscale);
		}
		else
		{
			sprintf(showStr,"快进x%2d", rightscale);
		}
		g_init = FYF_FALSE;
	}
	else
	{
		if (bPressLeftKey == FYF_TRUE)
		{
			sprintf(showStr,"快退-%2d", lparam);
		}
		else
		{
			sprintf(showStr,"快进x%2d", lparam);
		}
	}
	
	GUI_API_DrawStyle(hdc, 0, 0, STYLE_FAST_W100_H24);
	GUI_API_TextOut(hdc, 10, 0, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);
#endif
}


void UI_APP_JUMPTO_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	

	//printf("Press Key.==%d= offset = %d================UI_APP_TVOD_JUMPTO_ODC_KeyS\n",g_tvod_offset,key);
	int preReqScale = 0;
	unsigned int CurSecs = 0;
	char adjTimeStr[30] = { 0 };
	int startSecs = 0;
	int endSecs = 0;
	BU08 bRunNian = FYF_FALSE;
	char yearStr[5] = { 0 };
	char monthStr[3] = { 0 };
	char dayStr[3] = { 0 };
	char hourStr[3] = { 0 };
	char minStr[3] = { 0 };
	char secStr[3] = { 0 };
	BU16 curYear = 0;
	BU08 curMonth = 0;	
	BU08 curDay = 0;
	BU08 hour = 0;
	BU08 min = 0;
	BU08 sec = 0;
	
	
	BU08 bDataErr = FYF_FALSE;			//时间格式错误
	BU08 bDataOutRang = FYF_TRUE;		//时间超出范围
	BU08 bDataLarger = FYF_FALSE;		//时间格式错误
	BU08 bDataSmall = FYF_FALSE;			//时间超出范围
	
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
			switch(g_tvod_offset)
			{
			case 0:
				g_tvod_startYear = (BU08)((BU16)g_tvod_startYear%10 + key*10);
				break;
			case 1:
				g_tvod_startYear = (BU08)((BU16)g_tvod_startYear-g_tvod_startYear%10 + key);
				break;
			case 2:
				g_tvod_startMonth= (BU08)((BU16)g_tvod_startMonth%10 + key*10);
				break;
			case 3:
				g_tvod_startMonth = (BU08)((BU16)g_tvod_startMonth-g_tvod_startMonth%10 + key);
				break;
			case 4:
				g_tvod_startDay= (BU08)((BU16)g_tvod_startDay%10 + key*10);
				break;
			case 5:
				g_tvod_startDay = (BU08)((BU16)g_tvod_startDay-g_tvod_startDay%10 + key);
				break;
	
			case 6:
				g_tvod_startHour = (BU08)((BU16)g_tvod_startHour%10 + key*10);
				break;
			case 7:
				g_tvod_startHour = (BU08)((BU16)g_tvod_startHour-g_tvod_startHour%10 + key);
				break;
			case 8:
				g_tvod_startMin = (BU08)((BU16)g_tvod_startMin%10 + key*10);
				break;
			case 9:
				g_tvod_startMin = (BU08)((BU16)g_tvod_startMin-g_tvod_startMin%10 + key);
				break;
			case 10:
				g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd%10 + key*10);
				break;
			case 11:
				g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd-g_tvod_startsecd%10 + key);
				break;
			default:
				break;
			}

			if(g_tvod_startMonth >=13)	
				g_tvod_startMonth = 0;

			if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
				g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
			{
				if(g_tvod_startDay > 31)	
					g_tvod_startDay = 0;
			}
			else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
			{
				if(g_tvod_startDay > 30)	
					g_tvod_startDay = 0;
			}
			else if (g_tvod_startMonth == 2)
			{
				if ((2000 + g_tvod_startYear) % 4 == 0)
				{
					if ((2000 + g_tvod_startYear) % 100 != 0)
					{
						if(g_tvod_startDay > 29)	
							g_tvod_startDay = 0;
					}
					else if ((2000 + g_tvod_startYear) % 400 == 0)
					{
						if(g_tvod_startDay > 29)	
							g_tvod_startDay = 0;
					}
					else
					{
						if(g_tvod_startDay > 28)	
							g_tvod_startDay = 0;
					}
				}
				else
				{
					if(g_tvod_startDay > 28)	
						g_tvod_startDay = 0;
				}				
			}

			if(g_tvod_startHour>=24)	
				g_tvod_startHour = 0;
			if(g_tvod_startMin>=60)	
				g_tvod_startMin = 0;
			if(g_tvod_startsecd>=60)	
				g_tvod_startsecd = 0;
			
			g_tvod_offset++;
			if(g_tvod_offset>11)	
				g_tvod_offset=0;

			GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
		break;
	case GUI_VK_LEFT:
			if(g_tvod_offset==0)	
				g_tvod_offset=11;
			else 
				g_tvod_offset--;
			GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
		break;
	case GUI_VK_RIGHT:
			g_tvod_offset++;
			if(g_tvod_offset > 11)	
				g_tvod_offset=0;
			GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
		break;
	
	case GUI_VK_UP:

			switch(g_tvod_offset)
			{
			case 0:
				g_tvod_startYear = (BU08)((BU16)g_tvod_startYear%10 + (g_tvod_startYear/10 + 1)*10);
				break;
			case 1:
				g_tvod_startYear = (BU08)((BU16)g_tvod_startYear + 1);
				break;
			case 2:
				g_tvod_startMonth= (BU08)((BU16)g_tvod_startMonth%10 + (g_tvod_startMonth/10 + 1)*10);
				break;
			case 3:
				g_tvod_startMonth = (BU08)((BU16)g_tvod_startMonth + 1);
				break;
			case 4:
				g_tvod_startDay= (BU08)((BU16)g_tvod_startDay%10 + (g_tvod_startDay/10 + 1)*10);
				break;
			case 5:
				g_tvod_startDay = (BU08)((BU16)g_tvod_startDay + 1);
				break;
	
			case 6:
				g_tvod_startHour = (BU08)((BU16)g_tvod_startHour%10 + (g_tvod_startHour/10 + 1)*10);
				break;
			case 7:
				g_tvod_startHour = (BU08)((BU16)g_tvod_startHour + 1);
				break;
			case 8:
				g_tvod_startMin = (BU08)((BU16)g_tvod_startMin%10 + (g_tvod_startMin/10 + 1)*10);
				break;
			case 9:
				g_tvod_startMin = (BU08)((BU16)g_tvod_startMin + 1);
				break;
			case 10:
				g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd%10 + (g_tvod_startsecd/10 + 1)*10);
				break;
			case 11:
				g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd + 1);
				break;
			default:
				break;
			}


		printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d,min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
			if(g_tvod_startYear>=100)	
			{
				g_tvod_startYear = g_tvod_startYear % 100;
			}

			if(g_tvod_startMonth >=13)	
				g_tvod_startMonth = 1;

			if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
				g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
			{
				if(g_tvod_startDay > 31)	
					g_tvod_startDay = 1;
			}
			else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
			{
				if(g_tvod_startDay > 30)	
					g_tvod_startDay = 1;
			}
			else if (g_tvod_startMonth == 2)
			{
				if ((2000 + g_tvod_startYear) % 4 == 0)
				{
					if ((2000 + g_tvod_startYear) % 100 != 0)
					{
						if(g_tvod_startDay > 29)	
							g_tvod_startDay = 1;
					}
					else if ((2000 + g_tvod_startYear) % 400 == 0)
					{
						if(g_tvod_startDay > 29)	
							g_tvod_startDay = 1;
					}
					else
					{
						if(g_tvod_startDay > 28)	
							g_tvod_startDay = 1;
					}
				}
				else
				{
					if(g_tvod_startDay > 28)	
						g_tvod_startDay = 1;
				}				
			}
			else if (g_tvod_startMonth == 0)
			{
				if(g_tvod_startDay > 30)	
					g_tvod_startDay = 1;
			}

			if(g_tvod_startHour>=24)	
				g_tvod_startHour = 0;
			if(g_tvod_startMin>=60) 
				g_tvod_startMin = 0;
			if(g_tvod_startsecd>=60)	
				g_tvod_startsecd = 0;


		printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d,min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
			GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
		break;

	
	case GUI_VK_DOWN:
		
		switch(g_tvod_offset)
		{
		case 0:
			g_tvod_startYear = (BU08)((BU16)g_tvod_startYear%10 + (g_tvod_startYear/10 - 1)*10);
			break;
		case 1:
			g_tvod_startYear = (BU08)((BU16)g_tvod_startYear - 1);
			break;
		case 2:
			g_tvod_startMonth= (BU08)((BU16)g_tvod_startMonth%10 + (g_tvod_startMonth/10 - 1)*10);
			if(g_tvod_startMonth >=245 && g_tvod_startMonth <=255)	
			{
				g_tvod_startMonth = 12;
			}
			break;
		case 3:
			g_tvod_startMonth = (BU08)((BU16)g_tvod_startMonth - 1);
			if(g_tvod_startMonth >=245 && g_tvod_startMonth <=255)	
			{
				g_tvod_startMonth = 12 - (255 - g_tvod_startMonth);
			}
			else if (g_tvod_startMonth == 0)
			{
				g_tvod_startMonth = 12;
			}
				
			break;
		case 4:
			g_tvod_startDay= (BU08)((BU16)g_tvod_startDay%10 + (g_tvod_startDay/10 - 1)*10);
			
			if(g_tvod_startDay >=245 && g_tvod_startDay <=255)	
			{
				
				if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
					g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
				{
					g_tvod_startDay = 31;
				}
				else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
				{
					g_tvod_startDay = 30;
				}
				else if (g_tvod_startMonth == 2)
				{
					if ((2000 + g_tvod_startYear) % 4 == 0)
					{
						if ((2000 + g_tvod_startYear) % 100 != 0)
						{
							g_tvod_startDay = 29;
						}
						else if ((2000 + g_tvod_startYear) % 400 == 0)
						{
							g_tvod_startDay = 29;
						}
						else
						{
							g_tvod_startDay = 28;
						}
					}
					else
					{
						g_tvod_startDay = 28;
					}				
				}

			}
			else if (g_tvod_startDay == 0)
			{
				
				if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
					g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
				{
					g_tvod_startDay = 31;
				}
				else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
				{
					g_tvod_startDay = 30;
				}
				else if (g_tvod_startMonth == 2)
				{
					if ((2000 + g_tvod_startYear) % 4 == 0)
					{
						if ((2000 + g_tvod_startYear) % 100 != 0)
						{
							g_tvod_startDay = 29;
						}
						else if ((2000 + g_tvod_startYear) % 400 == 0)
						{
							g_tvod_startDay = 29;
						}
						else
						{
							g_tvod_startDay = 28;
						}
					}
					else
					{
						g_tvod_startDay = 28;
					}				
				}
				
			}
			break;
		case 5:
			g_tvod_startDay = (BU08)((BU16)g_tvod_startDay - 1);

			
			if(g_tvod_startDay >=245 && g_tvod_startDay <=255)	
			{
				
				if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
					g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
				{
					g_tvod_startDay = 31 - (255 - g_tvod_startDay);
				}
				else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
				{
					g_tvod_startDay = 30 - (255 - g_tvod_startDay);
				}
				else if (g_tvod_startMonth == 2)
				{
					if ((2000 + g_tvod_startYear) % 4 == 0)
					{
						if ((2000 + g_tvod_startYear) % 100 != 0)
						{
							g_tvod_startDay = 29 - (255 - g_tvod_startDay);
						}
						else if ((2000 + g_tvod_startYear) % 400 == 0)
						{
							g_tvod_startDay = 29 - (255 - g_tvod_startDay);
						}
						else
						{
							g_tvod_startDay = 28 - (255 - g_tvod_startDay);
						}
					}
					else
					{
						g_tvod_startDay = 28 - (255 - g_tvod_startDay);
					}				
				}
			}
			else if (g_tvod_startDay == 0)
			{
				
				if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
					g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
				{
					g_tvod_startDay = 31;
				}
				else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
				{
					g_tvod_startDay = 30;
				}
				else if (g_tvod_startMonth == 2)
				{
					if ((2000 + g_tvod_startYear) % 4 == 0)
					{
						if ((2000 + g_tvod_startYear) % 100 != 0)
						{
							g_tvod_startDay = 29;
						}
						else if ((2000 + g_tvod_startYear) % 400 == 0)
						{
							g_tvod_startDay = 29;
						}
						else
						{
							g_tvod_startDay = 28;
						}
					}
					else
					{
						g_tvod_startDay = 28;
					}				
				}
				
			}



			break;
		
		case 6:
			g_tvod_startHour = (BU08)((BU16)g_tvod_startHour%10 + (g_tvod_startHour/10 - 1)*10);
			
			if(g_tvod_startHour >=245 && g_tvod_startHour <=255)	
			{
				g_tvod_startHour = 23;
				
			}
			break;
		case 7:
			g_tvod_startHour = (BU08)((BU16)g_tvod_startHour - 1);
			
			if(g_tvod_startHour >=245 && g_tvod_startHour <=255)	
			{
				g_tvod_startHour = 23 - (255 - g_tvod_startHour);
				
			}
			break;
		case 8:
			g_tvod_startMin = (BU08)((BU16)g_tvod_startMin%10 + (g_tvod_startMin/10 - 1)*10);
			
			if(g_tvod_startMin >=245 && g_tvod_startMin <=255)	
			{
				g_tvod_startMin = 59;
				
			}
			break;
		case 9:
			g_tvod_startMin = (BU08)((BU16)g_tvod_startMin - 1);
			
			if(g_tvod_startMin >=245 && g_tvod_startMin <=255)	
			{
				g_tvod_startMin = 59 - (255 - g_tvod_startMin);
				
			}
			break;
		case 10:
			g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd%10 + (g_tvod_startsecd/10 - 1)*10);
			
			if(g_tvod_startsecd >=245 && g_tvod_startsecd <=255)	
			{
				g_tvod_startsecd = 59;
			}
			break;
		case 11:
			g_tvod_startsecd = (BU08)((BU16)g_tvod_startsecd - 1);
			
			if(g_tvod_startsecd >=245 && g_tvod_startsecd <=255)	
			{
				g_tvod_startsecd = 59 - (255 - g_tvod_startsecd);
			}
			break;
		default:
			break;
		}


		
		if(g_tvod_startYear >=245 && g_tvod_startYear <=255)	
		{
			g_tvod_startYear = 99 - (255 - g_tvod_startYear);
			
		}
		printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d, min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
		

		printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d, min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);

		if(g_tvod_startMonth >=13)	
			g_tvod_startMonth = 1;
		
		if (g_tvod_startMonth == 1 || g_tvod_startMonth == 3 || g_tvod_startMonth == 5 || g_tvod_startMonth == 7 || \
			g_tvod_startMonth == 8 || g_tvod_startMonth == 10 || g_tvod_startMonth == 12 )
		{
			if(g_tvod_startDay > 31)	
				g_tvod_startDay = 1;
		}
		else if (g_tvod_startMonth == 4 || g_tvod_startMonth == 6 || g_tvod_startMonth == 9 || g_tvod_startMonth == 11)
		{
			if(g_tvod_startDay > 30)	
				g_tvod_startDay = 1;
		}
		else if (g_tvod_startMonth == 2)
		{
			if ((2000 + g_tvod_startYear) % 4 == 0)
			{
				if ((2000 + g_tvod_startYear) % 100 != 0)
				{
					if(g_tvod_startDay > 29)	
						g_tvod_startDay = 1;
				}
				else if ((2000 + g_tvod_startYear) % 400 == 0)
				{
					if(g_tvod_startDay > 29)	
						g_tvod_startDay = 1;
				}
				else
				{
					if(g_tvod_startDay > 28)	
						g_tvod_startDay = 1;
				}
			}
			else
			{
				if(g_tvod_startDay > 28)	
					g_tvod_startDay = 1;
			}				
		}
		else if (g_tvod_startMonth == 0)
		{
			if(g_tvod_startDay > 30)	
				g_tvod_startDay = 1;
		}

		if(g_tvod_startHour>=24)	
			g_tvod_startHour = 0;
		if(g_tvod_startMin>=60) 
			g_tvod_startMin = 0;
		if(g_tvod_startsecd>=60)	
			g_tvod_startsecd = 0;
		
		printf("\033[0;31myear = %d, month = %d, day = %d,hour = %d, min = %d,sec = %d\033[0m\n",g_tvod_startYear,g_tvod_startMonth,g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);

		GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
		break;

		
	case GUI_VK_SELECT:

		startSecs = g_tvod_tmp_startSecs;
		endSecs = g_tvod_tmp_endSecs;
		curYear = g_tvod_startYear + 2000;
		curMonth = (BU08)g_tvod_startMonth;
		curDay = (BU08) g_tvod_startDay;
		hour = (BU08)g_tvod_startHour;
		min = (BU08)g_tvod_startMin;
		sec = (BU08) g_tvod_startsecd;
		
		if (curYear % 4 == 0)
		{
			if (curYear % 100 != 0)
			{
				bRunNian = FYF_TRUE;
			}
			else if (curYear % 400 == 0)
			{
				bRunNian = FYF_TRUE;
			}
		}
		
		if ((int)curMonth > 12 || (int)hourStr > 23 || ((int)minStr > 59) || ((int)secStr > 59) )
		{
			bDataErr = FYF_TRUE;
		}
		else if (curMonth == 2)
		{
			if (bRunNian)
			{
				if (curDay > 29)
					bDataErr = FYF_TRUE;
			}
			else if (curDay > 28)
			{
				bDataErr = FYF_TRUE;
			}
		}
		else if (curMonth == 1 || curMonth == 3 || curMonth == 5 || curMonth == 7 || curMonth == 8 || curMonth == 10
			|| curMonth == 12)
		{
			if (curDay > 31)
				bDataErr = FYF_TRUE;
		}
		else
		{
			if (curDay > 30)
				bDataErr = FYF_TRUE;
		}
		
		if (hour > 23 || min > 59 || sec > 59)
		{
			bDataErr = FYF_TRUE;
		}

		if (bDataErr)
		{
			if (VodPlayPara.pVPid != -1 && VodPlayPara.pAPid != -1)
			{
			
				FULLTOTVOD_ClearScreen();
				
				OW_ERROR("已超出直播时间点");
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutLiveTime, 0);
				
				if (Ipanel_GetEntrance_Flag())
				{
					OW_ERROR("返回到浏览器");
				}
				else
				{	
					OW_ERROR("进入直播");
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_ReturnToLive, 0);
				}
				
				GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
				GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
				GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
				
				FYF_API_thread_sleep(1000);
				OW_SetExitFlag(FYF_TRUE);
				
				return;
			}
			else
			{
				
				OW_ERROR("超出时移范围");
				OW_ERROR("从时移起点开始播放");
				
				VodPlayPara.reqScale = 1;
				VodPlayPara.bBOS = FYF_TRUE;
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
				}
				else
				{
					RTSP_PassBadFrame();
				}
				
				bPlayFromStart = FYF_TRUE;
				
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
				return;
			}
		
		}
		
		RTSP_CauAllSecs(curYear, curMonth, curDay, hour, min, sec, &CurSecs);
		
		//printf("g_tvod_tmp_startSecs = %d , g_tvod_tmp_endsec = %d, CurSecs = %d\n", g_tvod_tmp_startSecs, g_tvod_tmp_endSecs, CurSecs);
		if ((CurSecs > startSecs) && (CurSecs <= endSecs))
		{
			bDataOutRang = FYF_FALSE;
			printf("bDataOutRang = FYF_FALSE\n");
		}
		else
		{
			if (CurSecs <= startSecs)
			{
				bDataSmall = FYF_TRUE;
				
				printf("bDataSmall = FYF_TRUE\n");
			}
			else if (CurSecs > endSecs)
			{
				bDataLarger = FYF_TRUE;
				printf("bDataLarger = FYF_TRUE\n");
			}
		}

		if (bDataOutRang)
		{
			if (bDataSmall)
			{
				FULLTOTVOD_ClearScreen();
				if (bTVODPlay)
				{
					//printf("\033[0:31m 已超出时移起点\033[0m\n");
					OW_ERROR("已超出时移起点");
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutTimeshiftStart, 0);
				}	
				else
				{
					//printf("\033[0:31m 超出时移范围\033[0m\n");
					OW_ERROR("超出时移范围");
				}

				//printf("\033[0:31m 从时移起点开始播放\033[0m\n");
				OW_ERROR("从时移起点开始播放");
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_PlayFromTFStart, 0);

				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
				GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
					

				
				GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
				GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
				GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);

				FYF_API_thread_sleep(2000);

				FULLTOTVOD_ClearScreen();
								
				VodPlayPara.reqScale = 1;
				VodPlayPara.bBOS = FYF_TRUE;
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
				}
				else
				{
					RTSP_PassBadFrame();
				}

				bPlayFromStart = FYF_TRUE;
				
				printf("err input clock2222 is %s \n", VodPlayPara.clockCur);
				
				FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
				
				GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);
				GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);
				GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
				GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
				
				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
					GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
					GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
					APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
				}
				//GUI_ENC_API_SetFocus(ID_STA_BPLOGO);
	
				return;
			}
			else if (bDataLarger)
			{
				if (bTVODPlay)
				{
					FULLTOTVOD_ClearScreen();
					//printf("\033[0:31m 已超出直播时间点\033[0m\n");
					OW_ERROR("已超出直播时间点");
					
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutLiveTime, 0);

					if (Ipanel_GetEntrance_Flag())
					{
						//printf("\033[0:31m 返回到浏览器\033[0m\n");
						OW_ERROR("返回到浏览器");
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_STRING, "返回到浏览器", 0);
					}	
					else
					{
						//printf("\033[0:31m 进入直播\033[0m\n");
						OW_ERROR("进入直播");
						GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_ReturnToLive, 0);
					}	
					
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
					GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

					
					GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
					GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
					GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
					
					FYF_API_thread_sleep(2000);
					
					
					GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);
					GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);
					GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
					GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);

					FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
					GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
					FULLTOTVOD_ClearScreen();
					
					OW_SetExitFlag(FYF_TRUE);
					printf("err input clock is %s \n", VodPlayPara.clockCur);
					

					return;
			
				}
				else
				{			
					//printf("\033[0:31m 超出时移范围\033[0m\n");
					//printf("\033[0:31m 从时移起点开始播放\033[0m\n");
					OW_ERROR("超出时移范围");
					OW_ERROR("从时移起点开始播放");
					VodPlayPara.reqScale = 1;
					VodPlayPara.bBOS = FYF_TRUE;
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
					}
					else
					{
						RTSP_PassBadFrame();
					}
					
					FYF_API_thread_sleep(1000);
					bPlayFromStart = FYF_TRUE;
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					return;
			
				}
			}
		}
		VodPlayPara.reqScale = 1;
		
		
		RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
			secStr);
		sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
		
		printf("\033[0:31m input Curtime = %s\033[0m\n", adjTimeStr);
		
		memset(VodPlayPara.clockCur, 0, sizeof(VodPlayPara.clockCur));
		sprintf(VodPlayPara.clockCur, "%s%s%sT%s%s%s.00Z", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
		printf("success input clock is %s \n", VodPlayPara.clockCur);
		

		if (/*!bPlayFromStart && */(RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR))
		{
			RTSP_CheckNetOk();
			OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
		}
		else
		{
			RTSP_PassBadFrame();
		}
		bPlayFromStart = FYF_FALSE;


		GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);	
		GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);	
		GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
		GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
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
	case GUI_VK_EXIT:

		GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);	
		GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);	
		GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
		GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
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
	default:
		break;	
	}
#endif
}


void UI_APP_JUMPTO_OnPaint(BU32 hdc,BU32 lparam)
{
#ifdef USE_FZVOD	

	BU08 showStr[30]={0};
	
	if (GUI_ENC_API_IsFocus(ID_DLG_FULLTOTVOD))
	{
		printf("ID_DLG_FULLTOTVOD is focus==========\n");
		return;
	}

	sprintf(showStr,"20%02d-%02d-%02d  %02d:%02d:%02d",g_tvod_startYear,g_tvod_startMonth, g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);
	//sprintf(showStr,"%02d-%02d-%02d",g_vod_startHour,g_vod_startMin,g_vod_startsecd);
	GUI_API_DrawStyle(hdc, 0, 0, STYLE_JUMPTOBK_W340_H40);	
	if(g_tvod_offset>=0&&g_tvod_offset<2)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+2)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);			
	}
	else if(g_tvod_offset>=2&&g_tvod_offset<4)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+3)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_tvod_offset>=4&&g_tvod_offset<6)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+4)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_tvod_offset>=6&&g_tvod_offset<8)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+6)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_tvod_offset>=8&&g_tvod_offset<10)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+7)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_tvod_offset>=10&&g_tvod_offset<12)
	{
		GUI_API_DrawHLine(hdc, (g_tvod_offset+8)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}

	GUI_API_TextOut(hdc, 0, 8, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);

#endif
}




void UI_APP_JUMPBAR_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	
	
	BU16 startYear = 0;
	BU16 endYear = 0;
	BU16 curYear = 0;
	BU08 startMonth = 0;
	BU08 endMonth = 0;
	BU08 curMonth = 0;
	BU08 startDay = 0;
	BU08 endDay = 0;
	BU08 curDay = 0;
	BU08 hour = 0;
	BU08 min = 0;
	BU08 sec = 0;
	BU08 perPassTime = 3;					//每次偏移的时间180秒
	
	char yearStr[5] = { 0 };
	char monthStr[3] = { 0 };
	char dayStr[3] = { 0 };
	char tempChr[2] = { 0 };
	char hourStr[3] = { 0 };
	char minStr[3] = { 0 };
	char secStr[3] = { 0 };

	printf("Press Key.==%d=================UI_APP_TVOD_JUMPBAR_ODC_Key\n",key);
	//char adjTimeStr[30] = { 0 };

	//optionTimeCount = 0;
	switch(key)
	{
		case GUI_VK_LEFT:
		case GUI_VK_P_DOWN:

			
			
			curYear = g_tvod_startYear + 2000;
			curMonth = g_tvod_startMonth;
			curDay = g_tvod_startDay;
			hour = g_tvod_startHour;
			min = g_tvod_startMin;
			sec = g_tvod_startsecd;

			
			
			OW_SubTime(&curYear, &curMonth, &curDay, &hour, &min, &sec, perPassTime);
			
			RTSP_CauAllSecs(curYear, curMonth, curDay, hour, min, sec, &g_tvod_tmp_curSecs);
			
			//printf("\033[0;31m g_tvod_tmp_curSecs = %d , g_tvod_tmp_startSecs = %d, g_tvod_tmp_endSecs = %d\033[0m\n", g_tvod_tmp_curSecs, g_tvod_tmp_startSecs,g_tvod_tmp_endSecs);
			
			
			if ((g_tvod_tmp_curSecs < g_tvod_tmp_startSecs) || (g_tvod_tmp_curSecs == 0))
			{
				bSmaller = FYF_TRUE;
				bLarger = FYF_FALSE;
			}
			else
			{
				bSmaller = FYF_FALSE;
				bLarger = FYF_FALSE;
			}

			
			if (!bSmaller)
			{
				if (yelBarLen - 4 >= 0)
				{
					yelBarLen -= 4;

				}
				else
				{
					yelBarLen = 400;
				}
			}
			else
			{
				OW_AddTime(&curYear, &curMonth, &curDay, &hour, &min, &sec, perPassTime);
				
				/*RTSP_CauAllSecs(curYear, curMonth, curDay, hour, min, sec, &g_tvod_tmp_curSecs);
				
				if ((g_tvod_tmp_curSecs < g_tvod_tmp_startSecs) || (g_tvod_tmp_curSecs == 0))
				{
					bSmaller = FYF_TRUE;
					bLarger = FYF_FALSE;
				}
				else
				{
					bSmaller = FYF_FALSE;
					bLarger = FYF_FALSE;
				}*/
			}
			//printf("curYear = %d, curMonth = %d,curDay = %d,  hour = %d, min = %d, sec = %d\n", curYear, curMonth, curDay,hour,min,sec);

			
			if (!bLarger && !bSmaller)
			{
				RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr,
								minStr, secStr);
				memset(g_adjTimeStr, 0, 30);
				sprintf(g_adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);

			}

			//printf("=========g_adjTimeStr = %s\n", g_adjTimeStr);
			g_tvod_startYear = curYear - 2000;
			g_tvod_startMonth = curMonth;
			g_tvod_startDay = curDay;
			g_tvod_startHour = hour;
			g_tvod_startMin = min;
			g_tvod_startsecd = sec;

			
			printf("yelBarLen = %d, g_tvod_tmp_curSecs = %d\n", yelBarLen, g_tvod_tmp_curSecs);

			GUI_ENC_API_SetValue(ID_PBR_JUMPBAR,WDT_PBR_SV_CHANGE, yelBarLen,0);
			GUI_ENC_API_Update(ID_ODC_JUMPBAR,yelBarLen+1);


			//printf("=========g_CurSecs = %d, vod_progress = %d, g_adjTimeStr = %s\n", g_CurSecs - g_startSecs,vod_progress, g_adjTimeStr);
			break;

			
		case GUI_VK_RIGHT:
		case GUI_VK_P_UP:

			
			curYear = g_tvod_startYear + 2000;
			curMonth = g_tvod_startMonth;
			curDay = g_tvod_startDay;
			hour = g_tvod_startHour;
			min = g_tvod_startMin;
			sec = g_tvod_startsecd;

			

			
			OW_AddTime(&curYear, &curMonth, &curDay, &hour, &min, &sec, perPassTime);

			RTSP_CauAllSecs(curYear, curMonth, curDay, hour, min, sec, &g_tvod_tmp_curSecs);
			
			//printf("curYear = %d, curMonth = %d,curDay = %d,  hour = %d, min = %d, sec = %d\n", curYear, curMonth, curDay,hour,min,sec);
			//printf("\033[0;31m g_tvod_tmp_curSecs = %d , g_tvod_tmp_startSecs = %d, g_tvod_tmp_endSecs = %d\033[0m\n", g_tvod_tmp_curSecs, g_tvod_tmp_startSecs,g_tvod_tmp_endSecs);
			
			if ((g_tvod_tmp_curSecs > g_tvod_tmp_endSecs) || (g_tvod_tmp_curSecs == 0))
			{
				bLarger = FYF_TRUE;
				bSmaller = FYF_FALSE;
			}
			else
			{
				bLarger = FYF_FALSE;
				bSmaller = FYF_FALSE;
			}

			
			if (!bLarger)
			{
				if (yelBarLen + 4 <= 400)
				{
					yelBarLen += 4;
				}
				else
				{
					yelBarLen = 0;
				}

			}
			else
			{
				OW_SubTime(&curYear, &curMonth, &curDay, &hour, &min, &sec, perPassTime);
				
				/*RTSP_CauAllSecs(curYear, curMonth, curDay, hour, min, sec, &g_tvod_tmp_curSecs);
				
				
				if ((g_tvod_tmp_curSecs > g_tvod_tmp_endSecs) || (g_tvod_tmp_curSecs == 0))
				{
					bLarger = FYF_TRUE;
					bSmaller = FYF_FALSE;
				}
				else
				{
					bLarger = FYF_FALSE;
					bSmaller = FYF_FALSE;
				}*/
			}

			if (!bLarger && !bSmaller)
			{
				RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr,
								minStr, secStr);
				memset(g_adjTimeStr, 0, 30);
				sprintf(g_adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
			}
		
			g_tvod_startYear = curYear - 2000;
			g_tvod_startMonth = curMonth;
			g_tvod_startDay = curDay;
			g_tvod_startHour = hour;
			g_tvod_startMin = min;
			g_tvod_startsecd = sec;
			//printf("=========g_adjTimeStr = %s\n", g_adjTimeStr);

			
			
			GUI_ENC_API_SetValue(ID_PBR_JUMPBAR,WDT_PBR_SV_CHANGE, yelBarLen,0);
			GUI_ENC_API_Update(ID_ODC_JUMPBAR,yelBarLen+1);

			
			printf("yelBarLen = %d, g_tvod_tmp_curSecs = %d\n", yelBarLen, g_tvod_tmp_curSecs);


			//printf("=========g_CurSecs = %d, vod_progress = %d, adjTimeStr = %s\n", g_CurSecs - g_startSecs,vod_progress, g_adjTimeStr);
			break;

		case GUI_VK_EXIT:
			GUI_ENC_API_SetVisible(ID_STA_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_ODC_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_PBR_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_ODC_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_PBR_JUMPBAR, 1);
			
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
			g_tvod_startYear = 0;
			g_tvod_startMonth = 0;
			g_tvod_startDay = 0;
			g_tvod_startHour = 0;
			g_tvod_startMin = 0;
			g_tvod_startsecd = 0;
			break;
		
		case GUI_VK_SELECT:

			//OW_ClearPP();/*hqx:do it later*/
			FULLTOTVOD_ClearScreen();
			
			if (!bLarger && !bSmaller)
			{
				printf("=========g_adjTimeStr = %s\n", g_adjTimeStr);
			}
			
			RTSP_ParseNumTimeToStr((g_tvod_startYear + 2000), g_tvod_startMonth, g_tvod_startDay, g_tvod_startHour,g_tvod_startMin, g_tvod_startsecd, yearStr, monthStr, dayStr, hourStr, minStr,
				secStr);
			if (RTSP_CreatePAUSE(optionSocket, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("OW_VOD_MainMenu RTSP_CreatePAUSE  err!!\n ");
			}
			
			if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("OW_DrawJump RTSP_CreateTGET_PARAMETER  err!!\n ");
			}
			else
			{
				RTSP_GetSecondByClock(VodPlayPara.clockStart, &g_tvod_tmp_startSecs, &startYear, &startMonth, &startDay, FYF_TRUE);
				printf("clock start = %s\n",VodPlayPara.clockStart);
			}
			
			memset(VodPlayPara.clockCur, 0, sizeof(VodPlayPara.clockCur));

			printf("g_tvod_tmp_curSecs = %d, g_tvod_tmp_startSecs = %d\n",g_tvod_tmp_curSecs,g_tvod_tmp_startSecs);
			
			if ((g_tvod_tmp_curSecs < g_tvod_tmp_startSecs) || (g_tvod_tmp_curSecs == 0))
			{
				printf("in TP_OW_DrawTVODBarJump too long time!! \n");
				sprintf(VodPlayPara.clockCur, "%s", VodPlayPara.clockStart);
				VodPlayPara.bBOS = 1;
			}
			else
			{
				sprintf(VodPlayPara.clockCur, "%s%s%sT%s%s%s.00Z", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
			}
			
			VodPlayPara.reqScale = 1;
			if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
			}
			else
			{
				RTSP_PassBadFrame();
			}
			
			GUI_ENC_API_SetVisible(ID_STA_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_ODC_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_PBR_JUMPBAR, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_ODC_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_PBR_JUMPBAR, 1);
			
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
		default:
			break;
	}
#endif
}


void UI_APP_JUMPBAR_OnPaint(BU32 hdc,BU32 lparam)
{
#ifdef USE_FZVOD	

	BU08 showStr[100]={0};
	
	if (GUI_ENC_API_IsFocus(ID_DLG_FULLTOTVOD))
	{
		printf("ID_DLG_FULLTOTVOD is focus==========\n");
		return;
	}

	printf("tvod_progress = %d\n", lparam);
	//sprintf(showStr,"%02d:%02d:%02d %s",g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd,g_adjTimeStr);
	sprintf(showStr,"20%02d-%02d-%02d  %02d:%02d:%02d",g_tvod_startYear,g_tvod_startMonth, g_tvod_startDay,g_tvod_startHour,g_tvod_startMin,g_tvod_startsecd);

	GUI_API_DrawStyle(hdc, 0, 0, STYLE_JUMPBAR_W420_H24);
	if (bInitJumpBar)
	{
		bInitJumpBar = FYF_FALSE;
		GUI_API_DrawStyle(hdc, yelBarLen, 30, STYLE_VOD_SEEK_W20_H24);
		//tvod_progress = (18000 / tvod_perPasstime);
		//g_CurSecs = vod_progress * g_perPasstime;
		//GUI_API_DrawStyle(hdc, (18000 / tvod_perPasstime)*STEP_LONG, 30, STYLE_VOD_SEEK_W20_H24);
		GUI_ENC_API_SetValue(ID_PBR_JUMPBAR,WDT_PBR_SV_CHANGE,yelBarLen,0);
		GUI_ENC_API_Update(ID_ODC_JUMPBAR,yelBarLen+1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, (lparam-1), 30, STYLE_VOD_SEEK_W20_H24);
	}
		
	GUI_API_TextOut(hdc, 80, 6, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);

#endif
}

void UI_APP_TVOD_VOL_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	

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
	
	GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_PERIOD, 3000, 0);
	GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_REPLAY, 0, 0);

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
				GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE7, WDT_STA_SV_STRING, "[上翻页]/[下翻页]   进度条", 0);
			
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_BK, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE1, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE2, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE3, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE4, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE5, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE6, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE7, WS_VISIBLE);
				
				GUI_ENC_API_Update(ID_STA_TVOD_HELP_BK, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_HELP, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE1, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE2, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE3, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE4, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE5, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE6, 1);
				GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE7, 1);
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
				
				VodPlayPara.reqScale = 1;
				VodPlayPara.bBOS = FYF_TRUE;
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					RTSP_CheckNetOk();
					OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
				}
				else
				{
					RTSP_PassBadFrame();
				}
				FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
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
				

				bInitJumpBar = FYF_TRUE;
				
				
				if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					OW_ERROR("OW_DrawBPJump RTSP_CreateTGET_PARAMETER  err!!\n ");
					return FYF_ERR;
				}
				
				printf("clockStart = %s, clockCur = %s, clockend = %s\n", VodPlayPara.clockStart, VodPlayPara.clockCur,VodPlayPara.clockEnd);
				
				RTSP_GetSecondByClock(VodPlayPara.clockStart, &startSecs, &startYear, &startMonth, &startDay, FYF_TRUE);
				RTSP_GetSecondByClock(VodPlayPara.clockCur, &CurSecs, &curYear, &curMonth, &curDay, FYF_FALSE);
				RTSP_GetSecondByClock(VodPlayPara.clockEnd, &endSecs, &endYear, &endMonth, &endDay, FYF_TRUE);
				
				g_tvod_tmp_startSecs = startSecs;
				g_tvod_tmp_endSecs = endSecs;
				
				
				hour = CurSecs / 3600;
				CurSecs %= 3600;
				min = CurSecs / 60;
				CurSecs %= 60;
				sec = (BU08) CurSecs;
				RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
					secStr);
				sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
				
				printf("\033[0:31m Curtime = %s\033[0m\n", adjTimeStr);
				
				
				g_tvod_startYear = atoi(yearStr) - 2000;
				g_tvod_startMonth = (BU08) atoi(monthStr);
				g_tvod_startDay = (BU08) atoi(dayStr);
				g_tvod_startHour = (BU08) atoi(hourStr);
				g_tvod_startMin = (BU08) atoi(minStr);
				g_tvod_startsecd = (BU08) atoi(secStr);
				
				RTSP_GetSecondByClock(VodPlayPara.clockCur, &CurSecs, &curYear, &curMonth, &curDay, FYF_TRUE);
				g_tvod_tmp_curSecs = CurSecs;
				
				if (CurSecs - startSecs < (oneBarAllSec / 2))	//说明靠近起始点
				{
					yelBarLen = (CurSecs - startSecs) % (oneBarAllSec) * barLen / (oneBarAllSec);	//满条是2小时,7200秒
					printf("near start.\n");
				}
				else if (endSecs - CurSecs < (oneBarAllSec / 2))	//说明靠近结束点
				{
					yelBarLen = barLen - (endSecs - CurSecs) % (oneBarAllSec) * barLen / (oneBarAllSec);	//满条是2小时,7200秒
					printf("near end.\n");
				}
				else								//否则都取中间点
				{
					yelBarLen = barLen / 2;
				}
				printf("yelBarLen = %d\n", yelBarLen);

				FULLTOTVOD_SetCurState(TVOD_JUMPBAR_STATE);

				GUI_ENC_API_SetFocus(ID_ODC_JUMPBAR);

				GUI_ENC_API_SetVisible(ID_STA_JUMPBAR, WS_VISIBLE); 
				GUI_ENC_API_SetVisible(ID_ODC_JUMPBAR, WS_VISIBLE); 
				GUI_ENC_API_SetVisible(ID_PBR_JUMPBAR, WS_VISIBLE); 
				GUI_ENC_API_Update(ID_STA_JUMPBAR, 1);
				GUI_ENC_API_Update(ID_ODC_JUMPBAR, 1);
				GUI_ENC_API_Update(ID_PBR_JUMPBAR, 1);
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

					if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_DrawBPJump RTSP_CreateTGET_PARAMETER  err!!\n ");
						return FYF_ERR;
					}
					
					printf("clockStart = %s, clockCur = %s, clockend = %s\n", VodPlayPara.clockStart, VodPlayPara.clockCur,VodPlayPara.clockEnd);
					
					RTSP_GetSecondByClock(VodPlayPara.clockStart, &g_tvod_tmp_startSecs, &startYear, &startMonth, &startDay, FYF_TRUE);
					RTSP_GetSecondByClock(VodPlayPara.clockCur, &CurSecs, &curYear, &curMonth, &curDay, FYF_FALSE);
					RTSP_GetSecondByClock(VodPlayPara.clockEnd, &g_tvod_tmp_endSecs, &endYear, &endMonth, &endDay, FYF_TRUE);
					
					hour = CurSecs / 3600;
					CurSecs %= 3600;
					min = CurSecs / 60;
					CurSecs %= 60;
					sec = (BU08) CurSecs;
					RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
						secStr);
					sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
					
					printf("\033[0:31m Curtime = %s\033[0m\n", adjTimeStr);
					
					
					g_tvod_startYear = atoi(yearStr) - 2000;
					g_tvod_startMonth = (BU08) atoi(monthStr);
					g_tvod_startDay = (BU08) atoi(dayStr);
					g_tvod_startHour = (BU08) atoi(hourStr);
					g_tvod_startMin = (BU08) atoi(minStr);
					g_tvod_startsecd = (BU08) atoi(secStr);

					FULLTOTVOD_SetCurState(TVOD_JUMPTO_STATE);
					
					GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
					GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
					GUI_ENC_API_SetFocus(ID_ODC_JUMPTO);
					
					break;		

					
			case GUI_VK_V_DOWN:		

				printf("v_down==============\n");
				GUI_ENC_API_SetVisible(ID_VOI_TVOD, WS_VISIBLE);
				GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
				GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
				GUI_ENC_API_SetValueUnPaint(ID_VOI_TVOD, WDT_VOI_SV_KEY, key, 0);
				GUI_ENC_API_GetValue(ID_VOI_TVOD, WDT_VOI_SV_INDEX, &index, 0);
				if (index == 0)
				{
					printf("Mute.\n");
					GUI_ENC_API_Update(ID_VOI_TVOD, 1);
				}
				else
				{
					
					GUI_ENC_API_Update(ID_VOI_TVOD, 1);
				}
				printf("index = %d\n", index);
				//APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
				//UI_APP_API_SetVolume();
				
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
					
					bPressLeftKey = FYF_TRUE;
					GUI_ENC_API_SetVisible(ID_STA_FAST, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_FAST, 1);
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
					GUI_ENC_API_SetFocus(ID_ODC_FAST);
					FULLTOTVOD_SetCurState(FAST_STATE);
					g_init = FYF_TRUE;
					/*if (g_isleft)
					{
						leftscale = 4;
						g_isleft = FYF_FALSE;
						GUI_ENC_API_Update(ID_ODC_FAST,leftscale);
					}
					else*/
					{
						leftscale = 2;
						
						GUI_ENC_API_Update(ID_ODC_FAST,leftscale);

					}
					
					if (VodPlayPara.reqScale == 0)	//为了保证暂停前后播放速率一次,要先让服务器播放
					{
						if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
						}
						VodPlayPara.reqScale = 1;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
						}
					}
					if (VodPlayPara.reqScale == 1 || VodPlayPara.reqScale == 0)
					{
						VodPlayPara.reqScale = -2;
					}
					else
					{
						VodPlayPara.reqScale *= 2;
					}
					
					if (VodPlayPara.reqScale == -64)
						VodPlayPara.reqScale = -2;
					
					
					printf("rescale = %d\n", VodPlayPara.reqScale);
					if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
					}
					
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!and recover!\n ");
						//VodPlayPara.reqScale = preReqScale;
						OW_DrawSpeed(VodPlayPara.reqScale);
					}
					else
					{
						bPauseDisPly = FYF_FALSE;
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
					
					GUI_ENC_API_SetVisible(ID_STA_FAST, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_FAST, 1);
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
					GUI_ENC_API_SetFocus(ID_ODC_FAST);
					FULLTOTVOD_SetCurState(FAST_STATE);
					g_init = FYF_TRUE;
					bPressLeftKey = FYF_FALSE;
					
					/*if (g_isleft)
					{
						rightscale = 4;
						g_isleft = FYF_FALSE;
						GUI_ENC_API_Update(ID_ODC_FAST,rightscale);
					}
					else*/
					{
						rightscale = 2;
						
						GUI_ENC_API_Update(ID_ODC_FAST,rightscale);
					
					}
					
					if (VodPlayPara.reqScale == 0)	//为了保证暂停前后播放速率一次,要先让服务器播放   
					{
						if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
						}
						FYF_API_thread_sleep(100);
						VodPlayPara.reqScale = 1;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
						}
						//MILLI_DELAY(500); //重要,否则有可能播不出来
						VodPlayPara.reqScale = 2;
					}
					else
					{
						VodPlayPara.reqScale *= 2;
					}
					
					if (VodPlayPara.reqScale == 64)
						VodPlayPara.reqScale = 2;
					
					printf("rescale = %d\n", VodPlayPara.reqScale);
					
					if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
					}
					//MILLI_DELAY(50);
					
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!and recover!\n ");
						//VodPlayPara.reqScale = preReqScale;
						OW_DrawSpeed(VodPlayPara.reqScale);
					}
					break;


			case GUI_VK_V_UP:
		
									
				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK, 1);
					GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
					APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
				}
				
				GUI_ENC_API_SetVisible(ID_VOI_TVOD, WS_VISIBLE);
				GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
				GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
				GUI_ENC_API_SetValueUnPaint(ID_VOI_TVOD, WDT_VOI_SV_KEY, key, 0);
				GUI_ENC_API_GetValue(ID_VOI_TVOD, WDT_VOI_SV_INDEX, &index, 0);
				if (index == 0)
				{
					printf("Mute.\n");
					//GUI_ENC_API_SetVisible(ID_STAVOI_ChnInfo, WS_VISIBLE);
					GUI_ENC_API_Update(ID_VOI_TVOD, 1);
				}
				else
				{
					
					GUI_ENC_API_Update(ID_VOI_TVOD, 1);
				}
				printf("index = %d\n", index);
				APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
				UI_APP_API_SetVolume();
				
				break;
				
			case GUI_VK_SELECT:
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
			case GUI_VK_MUTE:
				
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

				FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
				break;
				
			default:
				break;

	}
#endif
}

void UI_APP_FAST_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	

	switch (key)
	{

		case GUI_VK_LEFT:

			
			if (g_isleft)
			{
				//leftscale = 4;
				g_isleft = FYF_FALSE;
				//GUI_ENC_API_Update(ID_ODC_FAST,leftscale);
			}

			if ((VodPlayPara.reqScale <= 0) || ((VodPlayPara.reqScale == 1)))	//保证在播放或快进状态
			{			
				bPressLeftKey = FYF_TRUE;
				if (leftscale < 32)
				{
					leftscale = 2 * leftscale;
				}
				else
				{
					leftscale = 2;
				}
				
				if (leftscale == 0)
				{
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
				}
				else
				{
					GUI_ENC_API_Update(ID_ODC_FAST, leftscale);
				}
				if (VodPlayPara.reqScale == 0)	//为了保证暂停前后播放速率一次,要先让服务器播放
				{
					if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
					}
					VodPlayPara.reqScale = 1;
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
					}
					else
					{
						RTSP_PassBadFrame();
					}
				}
				if (VodPlayPara.reqScale == 1 || VodPlayPara.reqScale == 0)
				{
					VodPlayPara.reqScale = -2;
				}
				else
				{
					VodPlayPara.reqScale *= 2;
				}
			
				if (VodPlayPara.reqScale == -64)
					VodPlayPara.reqScale = -2;
	
				printf("rescale = %d\n", VodPlayPara.reqScale);
				if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
				{
					RTSP_CheckNetOk();
					OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
				}
			
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					RTSP_CheckNetOk();
					OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!and recover!\n ");
					OW_DrawSpeed(VodPlayPara.reqScale);
				}
				else
				{
					RTSP_PassBadFrame();
					bPauseDisPly = FYF_FALSE;
				}
			
			}
	
			break;
			
		case GUI_VK_RIGHT:
			if (g_isleft)
			{
				//rightscale = 4;
				g_isleft = FYF_FALSE;
				//GUI_ENC_API_Update(ID_ODC_FAST,rightscale);
			}
			if (VodPlayPara.reqScale >= 0)	//保证在播放或快进状态
			{
				bPressLeftKey = FYF_FALSE;
				if (rightscale < 32)
				{
					rightscale = (rightscale * 2);
				}
				else
				{
					rightscale = 2;
				}
				
				if (rightscale == 0)
				{
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
				}
				else
				{
					GUI_ENC_API_Update(ID_ODC_FAST, rightscale);
				}
				
				if (VodPlayPara.reqScale == 0)	//为了保证暂停前后播放速率一次,要先让服务器播放   
				{
					if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
					}
					FYF_API_thread_sleep(100);
					VodPlayPara.reqScale = 1;
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
					}
					else
					{
						RTSP_PassBadFrame();
					}
	
					VodPlayPara.reqScale = 2;
				}
				else
				{
					VodPlayPara.reqScale *= 2;
				}
			
				if (VodPlayPara.reqScale == 64)
					VodPlayPara.reqScale = 2;
			
				printf("rescale = %d\n", VodPlayPara.reqScale);
				
				if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
				{
					RTSP_CheckNetOk();
					OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
				}
			
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					RTSP_CheckNetOk();
					OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!and recover!\n ");
					OW_DrawSpeed(VodPlayPara.reqScale);
				}
				else
				{
					RTSP_PassBadFrame();
					bPauseDisPly = FYF_FALSE;
				}
			
			}
			break;
	
		case GUI_VK_RECALL:
	
			if (VodPlayPara.reqScale != 1)
			{
				if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR) //获取时移时间范围
				{
					OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
					OW_SetExitFlag(FYF_TRUE);
				}
				VodPlayPara.reqScale = 1;
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					if (RTSP_CheckNetOk() == FYF_OK)
					{
						if (RTSP_CheckPlayErr() == FYF_ERR)
						{
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
							OW_DrawSpeed(VodPlayPara.reqScale);
						}
					}
				}
				else
				{
					RTSP_PassBadFrame();
					bPauseDisPly = FYF_FALSE;
				}
			
			}
		
			GUI_ENC_API_SetVisible(ID_STA_FAST, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_FAST, 1);
			GUI_ENC_API_Update(ID_ODC_FAST, 1);
			FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
			
			GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
			
			if (g_isleft)
			{
				g_isleft = FYF_FALSE;
			}
			FULLTOTVOD_ClearScreen();
			break;
			
		case GUI_VK_EXIT:
	
			leftscale = 1;
			rightscale = 1;
			if (g_isleft)
			{
				g_isleft = FYF_FALSE;
			}
			if (VodPlayPara.reqScale != 1)
			{
				if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR) //获取时移时间范围
				{
					OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
					OW_SetExitFlag(FYF_TRUE);
				}
				VodPlayPara.reqScale = 1;
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					if (RTSP_CheckNetOk() == FYF_OK)
					{
						if (RTSP_CheckPlayErr() == FYF_ERR)
						{
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
							OW_DrawSpeed(VodPlayPara.reqScale);
						}
					}
				}
				else
				{
					RTSP_PassBadFrame();
					bPauseDisPly = FYF_FALSE;
				}
			
			}
			GUI_ENC_API_SetVisible(ID_STA_FAST, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_FAST, 1);
			GUI_ENC_API_Update(ID_ODC_FAST, 1);
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
			if (g_isleft)
			{
				g_isleft = FYF_FALSE;
			}
			
			bPressLeftKey = FYF_FALSE;
			leftscale = 1;
			rightscale = 1;
			GUI_ENC_API_SetVisible(ID_STA_FAST, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_FAST, 1);
			GUI_ENC_API_Update(ID_ODC_FAST, 1);
			FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
			GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
			
			FULLTOTVOD_ClearScreen();
			if (VodPlayPara.reqScale != 1)	
			{
				if(bPauseDisPly)
				{
					bPauseDisPly = FYF_FALSE;
					//OW_DrawInBP();
				}
				if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
				{
					RTSP_CheckNetOk();
					OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
				}
				else
				{
					if( 0 == VodPlayPara.reqScale)//在暂停状态下要判断是否暂停太久超过时移起点
					{
						RTSP_CauClockTime(VodPlayPara.clockStart, &reStarTIme);
						RTSP_CauClockTime(clockPause, &pauseTime);
						if(pauseTime < reStarTIme)
						{
							//TPGRAPH_DrawTextInRegion(x + 100, y + 25, width, 25, "已超出时移起点", JCENTER, BLACKCOLOR);
							//TPGRAPH_DrawTextInRegion(x + 100, y + 55, width, 25, "从时移起点开始播放", JCENTER, BLACKCOLOR);
						}
					}
				}
				
				//MILLI_DELAY(100);
				VodPlayPara.reqScale = 1;
				if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					if (RTSP_CheckNetOk() == FYF_OK)
					{
						if (RTSP_CheckPlayErr() == FYF_ERR)
						{
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
							OW_DrawSpeed(VodPlayPara.reqScale);
						}
					}
				}
				else
				{
					RTSP_PassBadFrame();
					bPauseDisPly = FYF_FALSE;
					
				}
			}

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
			
	}
#endif
}


void UI_APP_FULLTOTVOD_Enter(void)
{	
#ifdef USE_FZVOD	

	int volume_index = 0;
	int ret = -1;
	printf("UI_APP_FULLTOTVOD_Enter\n");
	bfirst = 1;
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
	bGuiTimerSuspend = FYF_FALSE;
	rightscale = 2;
	leftscale = 2;
	bPause = FYF_FALSE;
	noKeytimeCount = 0;
	bCableDownCount = 0;

	if (GUI_IpanelState == 0)
		BroadCast_AppInit();

	
	FYF_API_demux_set_notify(NULL);
	TRD_SetBroadcastFreq(0);
	
	if (g_fromIpanel)
	{
		FULLTOTVOD_SetCurState(IPANEL_TO_TVOD_STATE);
		g_fromIpanel  = FYF_FALSE;
		printf("\033[0;31m g_fromIpanel \033[0m\n");
	}
	
	g_init = FYF_TRUE;
	bInTvodTimer = FYF_FALSE;
	//OW_SetExitFlag(FYF_FALSE);

	TVODInit();	
	VodPlayPara.VodOrTvod = 1;
	GUI_ENC_API_SetValue(ID_STA_PP_PLAYTIME,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_PAUSE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PLAYCHN,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_INBP,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_PAUSE_2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE_2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_TVOD_CHECKNET,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

	

	GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_BPLOGO, WS_DISABLE);	
	GUI_ENC_API_SetVisible(ID_STA_INFOBP, WS_DISABLE);	
	GUI_ENC_API_SetVisible(ID_STA_INBP, WS_DISABLE);	
	GUI_ENC_API_SetVisible(ID_STA_PP, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_PP_PAUSE, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_PP_PAUSE_2, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE_2, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_DISABLE);
	
	GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_PP_EXIT_LINE3,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	
	GUI_ENC_API_SetVisible(ID_STA_PP_EXIT_LINE1, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_PP_EXIT_LINE2, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_PP_EXIT_LINE3, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_FAST, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_JUMPBAR, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_PBR_JUMPBAR, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_ODC_JUMPBAR, WS_DISABLE);

	GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

	GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_DISABLE);

	GUI_ENC_API_SetVisible(ID_STA_PP_PLAYTIME, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_VOI_TVOD, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_TVOD_CHECKNET, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_PLAYCHN, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_BK, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE1, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE2, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE3, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE4, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE5, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE6, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE7, WS_DISABLE);

	GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PERIOD, 1000, 0);
	GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_STOP,0,0);
	
	APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &volume_index);
	printf("index = %d\n", volume_index);
	GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_RIGHT, volume_index, 0);

	bFullToTvodCount = 5;

	GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_PLAY,0,0);
	
	/*if (RTSP_CheckNetOk() != FYF_OK)
	{
		OW_SetExitFlag(FYF_TRUE);
		
		GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_STOP,0,0);

		printf("net error=======================\n");
		return;		
	}*/
	
	ret = TP_Ipanel_CheckNetOk();
	if (FYF_OK != ret)
	{
		if (ret == -2)
		{
			bNetError = FYF_TRUE;
			OW_SetExitFlag(FYF_TRUE);
			GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_STOP,0,0);

			OW_ERROR("net error=======================\n");
			bFullToTvodCount = 0;
			return;
		}
	}

	bNetError = FYF_FALSE;
	
	if ((FULLTOTVOD_GetCurState() == BPZHUN_STATE) || (FULLTOTVOD_GetCurState() == FAST_STATE)) 
	{
		FULLTOTVOD_SetCurState(BPZHUN_STATE);
		GUI_ENC_API_SetFocus(ID_ODC_BPZHUN);
		FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
		FYF_API_OSD_Refresh(0, 0, 720, 576);
		GUI_ENC_API_SetVisible(ID_STA_BAK_BPZHUN, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_ODC_BPZHUN, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_BPJUMP, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
		GUI_ENC_API_Update(ID_STA_BAK_BPZHUN, 1);
		GUI_ENC_API_Update(ID_ODC_BPZHUN, 1);	
		GUI_ENC_API_Update(ID_STA_BPJUMP, 1);
		GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			
		GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);
	}
	else if (FULLTOTVOD_GetCurState() == BPJUMP_STATE) 
	{
		FULLTOTVOD_SetCurState(BPJUMP_STATE);
		GUI_ENC_API_SetFocus(ID_ODC_BPJUMP);
		
		GUI_ENC_API_SetVisible(ID_STA_BPJUMP, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_BPJUMP, 1);
		GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);			
		
		FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
		FYF_API_OSD_Refresh(0, 0, 720, 576);
		bFromF10 = FYF_TRUE;
		#ifdef KEYTEST 
		//UI_APP_BPJUMP_INIT();		
		#else
		UI_APP_BPJUMP_INIT();		
		#endif
	} 
	else if (FULLTOTVOD_GetCurState() == IPANEL_TO_TVOD_STATE) 
	{
		printf("IPANEL_TO_TVOD_STATE ===================\n");
		FULLTOTVOD_SetCurState(IPANEL_TO_TVOD_STATE);
		FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
		FYF_API_OSD_Refresh(0, 0, 720, 576);

		GUI_ENC_API_SetVisible(ID_STA_BPJUMP, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_ODC_BPJUMP, WS_DISABLE);
		GUI_ENC_API_Update(ID_STA_BPJUMP, 1);
		GUI_ENC_API_Update(ID_ODC_BPJUMP, 1);	
		
		GUI_ENC_API_SetValue(ID_TIM_BPZHUN, WDT_TIM_SV_PLAY,0,0);

	}
		
	
	if (ipanel_GetAudMuteState())
	{
		printf("mute===================\n");
		APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
	}
	else
	{
		printf("unmute===================\n");
		APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
	}
#endif

}


void UI_APP_FULLTOTVOD_End(void)
{
#ifdef USE_FZVOD	

	printf("UI_APP_FullToF10_End\n");

	//if (OW_GetExitFlag() == FYF_TRUE)
	//FYF_API_OSD_FillRect(0, 0, 720, 576,0);
	//FYF_API_OSD_Refresh(0, 0, 720, 576);
	
	g_fromIpanel  = FYF_FALSE;

	g_tvod_startYear = 0;
	g_tvod_startMonth = 0;
	g_tvod_startDay = 0;
	g_tvod_startHour = 0;
	g_tvod_startMin = 0;
	g_tvod_startsecd = 0;
	g_tvod_tmp_startSecs = 0;
	g_tvod_tmp_endSecs = 0;

	bTurnerError = FYF_FALSE;

	
	OW_SetExitFlag(FYF_FALSE);
	//OW_VOD_SetNetErrFlag(FYF_FALSE)
	if (GUI_IpanelState == 0)
	{
		BroadCast_AppExit();		
		//CA_SetEnable(1);
		APP_Com_IPI_StopProg(0);
		Ipanel_SetEntrance_Flag(FYF_FALSE);
		OW_VOD_SetDhcpFlag(FYF_FALSE);
		Ipanel_SetEnterVodOrTvod_Flag(FYF_FALSE);
		FYF_API_demux_set_notify(CallBackGetTableFunc);
	}


	
	bFromF10 = FYF_FALSE;
	bIsSelect = FYF_FALSE;
	bIsLeft = FYF_FALSE;

	
	w = 0;
	r = 0;
	num = 0;
#endif	
}


void UI_APP_FULLTOTVOD_key(BU32 key)
{
#ifdef USE_FZVOD	

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

	
	
	GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_PERIOD, 10000, 0);
	GUI_ENC_API_SetValue(ID_TIM_CLEANSCREEN, WDT_TIM_SV_REPLAY, 0, 0);

	curstate = FULLTOTVOD_GetCurState();
	printf("Press Key.==%d= curstate = %d==bNetError = %d============UI_APP_FULLTOTVOD_key\n", key, curstate, bNetError);
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

	switch (curstate)
	{
		case TVOD_VOL_STATE:
			UI_APP_TVOD_VOL_ODC_Key(key);
			break;

		case TVOD_NOKEY_PRESS_TIMEOUT_STATE:
			if (key && (noKeytimeCount <= (NOKEY_COUNT + 30)))
			{
		
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

				noKeytimeCount = 0;

			}
			break;
			
		case TVOD_HELP_STATE:
			switch(key)
			{
				case GUI_VK_EXIT:
				case GUI_VK_SELECT:
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
					
				default:
					break;
			}
			break;

		case FAST_STATE:
			if (GUI_ENC_API_IsFocus(ID_ODC_FAST))
			{
				UI_APP_FAST_ODC_Key(key);
			}
			break;		
			
		case BPJUMP_STATE:
		
			if (GUI_ENC_API_IsFocus(ID_ODC_BPJUMP))
			{
				UI_APP_BPJUMP_ODC_Key(key);
			}
			break;
			
		case BPZHUN_STATE:
			if (GUI_ENC_API_IsFocus(ID_ODC_BPZHUN))
			{
				UI_APP_BPZHUN_ODC_Key(key);
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
					if (GUI_IpanelState == 0)
					{
						/*if (bHadRes)
						{
							if (serSetParaType != 0 && RTSP_CreateTEARDOWN(optionSocket, &VodPlayPara) == FYF_ERR)
							{
								OW_ERROR("OW_VOD_MainMenu RTSP_CreateTEARDOWN  err!!\n ");
							}
							bHadRes = FYF_FALSE;				//释放服务器前端分配资源
							if (bTVODPlay)
							{
								APP_Com_API_StopProg(0);
								//APP_IPI_StopAudioVideo();
							}
						
						}*/
						
						/*if (optionSocket != -1)
						{
							close(optionSocket);
							optionSocket = -1;
							optionTimeCount = 0;
						}
						
						Ow_Set_entervodortvod_Flag(FYF_FALSE);
						
						
						OW_DEBUG("OW_TVOD_MainMenu End.\n");
					
						//FULLTOTVOD_SetCurState(PP_EXIT_STATE);
						GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
						FYF_API_demux_set_notify(CallBackGetTableFunc);
						GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
						APP_Com_IPI_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), 1);
						bfirst = 1;
						bGuiTimerSuspend = FYF_FALSE;
						bPauseDisPly = FYF_FALSE;
						printf("Exit==============\n");*/
						
						OW_SetExitFlag(FYF_TRUE);
						APP_Com_API_StopProg(1);
						//OW_DLRTPInfo_StopChannel();
						/*FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
						FYF_API_OSD_Refresh(0, 0, 720, 576);

						GUI_ENC_API_SetValue(ID_STA_PLAYCHN, WDT_STA_SV_DATAID, RS_PP_PlayChn, 0);
						GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE, WDT_STA_SV_STRING, "   现在时间是", 0);
						APP_SYS_API_GetLocalDateAndTime(&DateAndTime);
						
						memset(FirsttimeStr, 0, 50);
						sprintf(FirsttimeStr, "    %02d:%02d:%02d", DateAndTime.m_hour, DateAndTime.m_min, DateAndTime.m_sec);
						printf("\033[0;31m FirsttimeStr = %s\033[0m\n", FirsttimeStr);
						GUI_ENC_API_SetValue(ID_STA_PP_PLAYTIME, WDT_STA_SV_STRING, FirsttimeStr, 0);

						GUI_ENC_API_SetVisible(ID_STA_PP, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_PLAYCHN, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_PP_PLAYTIME, WS_VISIBLE);
						
						GUI_ENC_API_Update(ID_STA_PP, 1);
						GUI_ENC_API_Update(ID_STA_PLAYCHN, 1);
						GUI_ENC_API_Update(ID_STA_PP_CONTINUE, 1);
						GUI_ENC_API_Update(ID_STA_PP_PLAYTIME, 1);
						
						FYF_API_thread_sleep(2000);
						
						FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
						FYF_API_OSD_Refresh(0, 0, 720, 576);*/

						

						
					}
					else
					{
						bfirst = 0;
						bGuiTimerSuspend = FYF_TRUE;
						/*GUI_ENC_API_SwitchState(GUI_IpanelState, 1);*/
						GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
						OW_SetExitFlag(FYF_TRUE);
					}
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
					GUI_ENC_API_SetValue(ID_STA_TVOD_HELP_LINE7, WDT_STA_SV_STRING, "[上翻页]/[下翻页]   进度条", 0);

					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_BK, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE1, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE2, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE3, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE4, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE5, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE6, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_TVOD_HELP_LINE7, WS_VISIBLE);
					
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_BK, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE1, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE2, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE3, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE4, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE5, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE6, 1);
					GUI_ENC_API_Update(ID_STA_TVOD_HELP_LINE7, 1);
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
					
					VodPlayPara.reqScale = 1;
					VodPlayPara.bBOS = FYF_TRUE;
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
					}
					else
					{
						RTSP_PassBadFrame();
					}

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
					
					bPressLeftKey = FYF_TRUE;
					GUI_ENC_API_SetVisible(ID_STA_FAST, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_FAST, 1);
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
					GUI_ENC_API_SetFocus(ID_ODC_FAST);
					FULLTOTVOD_SetCurState(FAST_STATE);
					g_init = FYF_TRUE;
					if (g_isleft)
					{
						leftscale = 4;
						g_isleft = FYF_FALSE;
						GUI_ENC_API_Update(ID_ODC_FAST,leftscale);
					}
					else
					{
						leftscale = 2;
						
						GUI_ENC_API_Update(ID_ODC_FAST,leftscale);

					}
					
					if (VodPlayPara.reqScale == 0)	//为了保证暂停前后播放速率一次,要先让服务器播放
					{
						if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
						}
						VodPlayPara.reqScale = 1;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
						}
						
					}
					if (VodPlayPara.reqScale == 1 || VodPlayPara.reqScale == 0)
					{
						VodPlayPara.reqScale = -2;
					}
					else
					{
						VodPlayPara.reqScale *= 2;
					}
					
					if (VodPlayPara.reqScale == -64)
						VodPlayPara.reqScale = -2;
					
					
					printf("rescale = %d\n", VodPlayPara.reqScale);
					if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
					}
					
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!and recover!\n ");
						//VodPlayPara.reqScale = preReqScale;
						OW_DrawSpeed(VodPlayPara.reqScale);
					}
					else
					{
						bPauseDisPly = FYF_FALSE;
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

					GUI_ENC_API_SetVisible(ID_STA_FAST, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_FAST, 1);
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
					GUI_ENC_API_SetFocus(ID_ODC_FAST);
					FULLTOTVOD_SetCurState(FAST_STATE);
					g_init = FYF_TRUE;
					bPressLeftKey = FYF_FALSE;
					
					if (g_isleft)
					{
						rightscale = 4;
						g_isleft = FYF_FALSE;
						GUI_ENC_API_Update(ID_ODC_FAST,rightscale);
					}
					else
					{
						rightscale = 2;
						
						GUI_ENC_API_Update(ID_ODC_FAST,rightscale);

					}

					if (VodPlayPara.reqScale == 0)	//为了保证暂停前后播放速率一次,要先让服务器播放   
					{
						if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
						}
						FYF_API_thread_sleep(100);
						VodPlayPara.reqScale = 1;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
						}
						//MILLI_DELAY(500); //重要,否则有可能播不出来
						VodPlayPara.reqScale = 2;
					}
					else
					{
						VodPlayPara.reqScale *= 2;
					}
					
					if (VodPlayPara.reqScale == 64)
						VodPlayPara.reqScale = 2;
					
					printf("rescale = %d\n", VodPlayPara.reqScale);
					
					if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
					}
					//MILLI_DELAY(50);
					
					if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						RTSP_CheckNetOk();
						OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!and recover!\n ");
						//VodPlayPara.reqScale = preReqScale;
						OW_DrawSpeed(VodPlayPara.reqScale);
					}
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
					
					//FULLTOTVOD_ClearScreen();

					if (g_isleft)
					{
						g_isleft = FYF_FALSE;
					}
					bPressLeftKey = FYF_FALSE;
					leftscale = 1;
					rightscale = 1;
					GUI_ENC_API_SetVisible(ID_STA_FAST, WS_DISABLE);	
					GUI_ENC_API_SetVisible(ID_ODC_FAST, WS_DISABLE);	
					GUI_ENC_API_Update(ID_STA_FAST, 1);
					GUI_ENC_API_Update(ID_ODC_FAST, 1);
					if (VodPlayPara.reqScale == 1)
					{
						VodPlayPara.reqScale = 0;
						if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
						}
						else
						{
							sprintf(clockPause,"%s",VodPlayPara.clockCur);
						}
						
						if (RTSP_CreatePAUSE(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePAUSE	err!!\n ");
							//VodPlayPara.reqScale = preReqScale;
							OW_DrawSpeed(VodPlayPara.reqScale);
							
						}
						else
						{
							OW_DrawSpeed(VodPlayPara.reqScale);
						}
						
						bPauseDisPly = FYF_FALSE;
						FULLTOTVOD_ClearScreen();
						
						GUI_ENC_API_SetValue(ID_STA_PP_PAUSE_2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
						GUI_ENC_API_SetValue(ID_STA_PP_CONTINUE_2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
							
						GUI_ENC_API_SetVisible(ID_STA_PP, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_PP_PAUSE_2, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_PP_CONTINUE_2, WS_VISIBLE);
						
						GUI_ENC_API_Update(ID_STA_PP, 1);
						GUI_ENC_API_Update(ID_STA_PP_PAUSE_2, 1);
						GUI_ENC_API_Update(ID_STA_PP_CONTINUE_2, 1);

						FULLTOTVOD_SetCurState(TVOD_PAUSE_STATE);
					
						if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
						{
							GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
							GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
							GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
							GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
							GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
							APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
						}
					}
					else
					{
					
						if(bPauseDisPly)
						{
							bPauseDisPly = FYF_FALSE;
							//OW_DrawInBP();
						}
						
						if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
						}
						else
						{
							if( 0 == VodPlayPara.reqScale)//在暂停状态下要判断是否暂停太久超过时移起点
							{
								RTSP_CauClockTime(VodPlayPara.clockStart, &reStarTIme);
								RTSP_CauClockTime(clockPause, &pauseTime);
								printf("reStarTIme = %d, pauseTime = %d\n", reStarTIme, pauseTime);
								if(pauseTime < reStarTIme)
								{
									//TPGRAPH_DrawTextInRegion(x + 100, y + 25, width, 25, "已超出时移起点", JCENTER, BLACKCOLOR);
									//TPGRAPH_DrawTextInRegion(x + 100, y + 55, width, 25, "从时移起点开始播放", JCENTER, BLACKCOLOR);
									OW_ERROR("已超出时移起点");
									OW_ERROR("从时移起点开始播放");
								}
							}
						}
						
						//MILLI_DELAY(100);
						//preReqScale = VodPlayPara.reqScale;
						VodPlayPara.reqScale = 1;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							if (RTSP_CheckNetOk() == FYF_OK)
							{
								if (RTSP_CheckPlayErr() == FYF_ERR)
								{
									OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
									//VodPlayPara.reqScale = preReqScale;
									OW_DrawSpeed(VodPlayPara.reqScale);
								}
							}
						}
						else
						{
							//if (preReqScale != 0) //快进快退后播放
							{
								RTSP_PassBadFrame();
							}
						}
						
						FULLTOTVOD_ClearScreen();
					
						if (bIsSelect == FYF_TRUE)
						{
							bIsSelect = FYF_FALSE;
							bIsLeft = FYF_FALSE;
							GUI_ENC_API_SetVisible(ID_STA_INFOBP, WS_VISIBLE);	
							GUI_ENC_API_SetVisible(ID_STA_INBP, WS_VISIBLE);	
							GUI_ENC_API_Update(ID_STA_INFOBP, 1);
							GUI_ENC_API_Update(ID_STA_INBP, 1);
							
							FYF_API_thread_sleep(2000);
							FULLTOTVOD_ClearScreen();
						}
						
						if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
						{
							GUI_ENC_API_SetValue(ID_STA_TVOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
							GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE_BAK, WS_VISIBLE);						
							GUI_ENC_API_SetVisible(ID_STA_TVOD_MUTE, WS_VISIBLE);
							GUI_ENC_API_Update(ID_STA_TVOD_MUTE_BAK,1);
							GUI_ENC_API_Update(ID_STA_TVOD_MUTE,1);
							APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
						}
					}

				
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

					if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_DrawBPJump RTSP_CreateTGET_PARAMETER  err!!\n ");
						return FYF_ERR;
					}
					
					printf("clockStart = %s, clockCur = %s, clockend = %s\n", VodPlayPara.clockStart, VodPlayPara.clockCur,VodPlayPara.clockEnd);
					
					RTSP_GetSecondByClock(VodPlayPara.clockStart, &g_tvod_tmp_startSecs, &startYear, &startMonth, &startDay, FYF_TRUE);
					RTSP_GetSecondByClock(VodPlayPara.clockCur, &CurSecs, &curYear, &curMonth, &curDay, FYF_FALSE);
					RTSP_GetSecondByClock(VodPlayPara.clockEnd, &g_tvod_tmp_endSecs, &endYear, &endMonth, &endDay, FYF_TRUE);
					
					hour = CurSecs / 3600;
					CurSecs %= 3600;
					min = CurSecs / 60;
					CurSecs %= 60;
					sec = (BU08) CurSecs;
					RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
						secStr);
					sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
					
					printf("\033[0:31m Curtime = %s\033[0m\n", adjTimeStr);
					
					
					g_tvod_startYear = atoi(yearStr) - 2000;
					g_tvod_startMonth = (BU08) atoi(monthStr);
					g_tvod_startDay = (BU08) atoi(dayStr);
					g_tvod_startHour = (BU08) atoi(hourStr);
					g_tvod_startMin = (BU08) atoi(minStr);
					g_tvod_startsecd = (BU08) atoi(secStr);

					FULLTOTVOD_SetCurState(TVOD_JUMPTO_STATE);
					
					GUI_ENC_API_SetVisible(ID_STA_JUMPTO, WS_VISIBLE);	
					GUI_ENC_API_SetVisible(ID_ODC_JUMPTO, WS_VISIBLE);	
					GUI_ENC_API_Update(ID_STA_JUMPTO, 1);
					GUI_ENC_API_Update(ID_ODC_JUMPTO, 1);
					GUI_ENC_API_SetFocus(ID_ODC_JUMPTO);
					
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
				

				bInitJumpBar = FYF_TRUE;
				
				
				if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)
				{
					OW_ERROR("OW_DrawBPJump RTSP_CreateTGET_PARAMETER  err!!\n ");
					return FYF_ERR;
				}
				
				printf("clockStart = %s, clockCur = %s, clockend = %s\n", VodPlayPara.clockStart, VodPlayPara.clockCur,VodPlayPara.clockEnd);
				
				RTSP_GetSecondByClock(VodPlayPara.clockStart, &startSecs, &startYear, &startMonth, &startDay, FYF_TRUE);
				RTSP_GetSecondByClock(VodPlayPara.clockCur, &CurSecs, &curYear, &curMonth, &curDay, FYF_FALSE);
				RTSP_GetSecondByClock(VodPlayPara.clockEnd, &endSecs, &endYear, &endMonth, &endDay, FYF_TRUE);
				
				g_tvod_tmp_startSecs = startSecs;
				g_tvod_tmp_endSecs = endSecs;
				
				hour = CurSecs / 3600;
				CurSecs %= 3600;
				min = CurSecs / 60;
				CurSecs %= 60;
				sec = (BU08) CurSecs;
				RTSP_ParseNumTimeToStr(curYear, curMonth, curDay, hour, min, sec, yearStr, monthStr, dayStr, hourStr, minStr,
					secStr);
				sprintf(adjTimeStr, "%s-%s-%s  %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minStr, secStr);
				
				printf("\033[0:31m Curtime = %s\033[0m\n", adjTimeStr);
				
				
				g_tvod_startYear = atoi(yearStr) - 2000;
				g_tvod_startMonth = (BU08) atoi(monthStr);
				g_tvod_startDay = (BU08) atoi(dayStr);
				g_tvod_startHour = (BU08) atoi(hourStr);
				g_tvod_startMin = (BU08) atoi(minStr);
				g_tvod_startsecd = (BU08) atoi(secStr);
				
				RTSP_GetSecondByClock(VodPlayPara.clockCur, &CurSecs, &curYear, &curMonth, &curDay, FYF_TRUE);
				g_tvod_tmp_curSecs = CurSecs;
				
				if (CurSecs - startSecs < (oneBarAllSec / 2))	//说明靠近起始点
				{
					yelBarLen = (CurSecs - startSecs) % (oneBarAllSec) * barLen / (oneBarAllSec);	//满条是2小时,7200秒
					printf("near start.\n");
				}
				else if (endSecs - CurSecs < (oneBarAllSec / 2))	//说明靠近结束点
				{
					yelBarLen = barLen - (endSecs - CurSecs) % (oneBarAllSec) * barLen / (oneBarAllSec);	//满条是2小时,7200秒
					printf("near end.\n");
				}
				else								//否则都取中间点
				{
					yelBarLen = barLen / 2;
				}
				printf("yelBarLen = %d\n", yelBarLen);

				FULLTOTVOD_SetCurState(TVOD_JUMPBAR_STATE);

				GUI_ENC_API_SetFocus(ID_ODC_JUMPBAR);

				GUI_ENC_API_SetVisible(ID_STA_JUMPBAR, WS_VISIBLE); 
				GUI_ENC_API_SetVisible(ID_ODC_JUMPBAR, WS_VISIBLE); 
				GUI_ENC_API_SetVisible(ID_PBR_JUMPBAR, WS_VISIBLE); 
				GUI_ENC_API_Update(ID_STA_JUMPBAR, 1);
				GUI_ENC_API_Update(ID_ODC_JUMPBAR, 1);
				GUI_ENC_API_Update(ID_PBR_JUMPBAR, 1);
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
			
			if (GUI_ENC_API_IsFocus(ID_ODC_JUMPTO))
			{
				UI_APP_JUMPTO_ODC_Key(key);
			}
			break;
		
		case TVOD_PAUSE_STATE:
			
			
			switch (key)
			{
				case GUI_VK_EXIT:
					//FULLTOTVOD_ClearScreen();
					if (VodPlayPara.reqScale != 1)
					{
						if(bPauseDisPly)
						{
							bPauseDisPly = FYF_FALSE;
							//OW_DrawInBP();
						}
						
						if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
						}
						else
						{
							if( 0 == VodPlayPara.reqScale)//在暂停状态下要判断是否暂停太久超过时移起点
							{
								RTSP_CauClockTime(VodPlayPara.clockStart, &reStarTIme);
								RTSP_CauClockTime(clockPause, &pauseTime);
								
								printf("reStarTIme = %d, pauseTime = %d\n", reStarTIme, pauseTime);
								if(pauseTime < reStarTIme)
								{
									//TPGRAPH_DrawTextInRegion(x + 100, y + 25, width, 25, "已超出时移起点", JCENTER, BLACKCOLOR);
									//TPGRAPH_DrawTextInRegion(x + 100, y + 55, width, 25, "从时移起点开始播放", JCENTER, BLACKCOLOR);
									OW_ERROR("已超出时移起点");
									OW_ERROR("从时移起点开始播放");
									
									FULLTOTVOD_ClearScreen();

									GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutTimeshiftStart, 0);
									GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_PlayFromTFStart, 0);
									
									GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
									GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
									GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
									GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
									GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
									GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
									FYF_API_thread_sleep(2000);
									
									FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
									GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
									FULLTOTVOD_ClearScreen();
									VodPlayPara.bBOS = FYF_TRUE;
									
								}
							}
						}
						
						//MILLI_DELAY(100);
						//preReqScale = VodPlayPara.reqScale;
						VodPlayPara.reqScale = 1;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							if (RTSP_CheckNetOk() == FYF_OK)
							{
								if (RTSP_CheckPlayErr() == FYF_ERR)
								{
									OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
									//VodPlayPara.reqScale = preReqScale;
									OW_DrawSpeed(VodPlayPara.reqScale);
								}
							}
						}
						else
						{
							//if (preReqScale != 0) //快进快退后播放
							{
								RTSP_PassBadFrame();
							}
						}
						
						FULLTOTVOD_ClearScreen();
					}
					
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
					if (VodPlayPara.reqScale != 1)
					{
						if(bPauseDisPly)
						{
							bPauseDisPly = FYF_FALSE;
							//OW_DrawInBP();
						}
						
						if (RTSP_CreateTGET_PARAMETER(optionSocket, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER	err!!\n ");
						}
						else
						{
							if( 0 == VodPlayPara.reqScale)//在暂停状态下要判断是否暂停太久超过时移起点
							{
								RTSP_CauClockTime(VodPlayPara.clockStart, &reStarTIme);
								RTSP_CauClockTime(clockPause, &pauseTime);
								printf("reStarTIme = %d, pauseTime = %d\n", reStarTIme, pauseTime);
								if(pauseTime < reStarTIme)
								{
									//TPGRAPH_DrawTextInRegion(x + 100, y + 25, width, 25, "已超出时移起点", JCENTER, BLACKCOLOR);
									//TPGRAPH_DrawTextInRegion(x + 100, y + 55, width, 25, "从时移起点开始播放", JCENTER, BLACKCOLOR);
									OW_ERROR("已超出时移起点");
									OW_ERROR("从时移起点开始播放");

									GUI_ENC_API_SetValue(ID_STA_ERROR_LINE1, WDT_STA_SV_DATAID, RS_PP_OutTimeshiftStart, 0);
									GUI_ENC_API_SetValue(ID_STA_ERROR_LINE2, WDT_STA_SV_DATAID, RS_PP_PlayFromTFStart, 0);
									
									GUI_ENC_API_SetVisible(ID_STA_PP_EXIT, WS_VISIBLE);
									GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE1, WS_VISIBLE);
									GUI_ENC_API_SetVisible(ID_STA_ERROR_LINE2, WS_VISIBLE);
									GUI_ENC_API_Update(ID_STA_PP_EXIT, 1);
									GUI_ENC_API_Update(ID_STA_ERROR_LINE1, 1);
									GUI_ENC_API_Update(ID_STA_ERROR_LINE2, 1);
									FYF_API_thread_sleep(2000);
									
									FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
									GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
									FULLTOTVOD_ClearScreen();
									VodPlayPara.bBOS = 1;
								}
							}
						}
						
						//MILLI_DELAY(100);
						//preReqScale = VodPlayPara.reqScale;
						VodPlayPara.reqScale = 1;
						
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							if (RTSP_CheckNetOk() == FYF_OK)
							{
								if (RTSP_CheckPlayErr() == FYF_ERR)
								{
									OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
									//VodPlayPara.reqScale = preReqScale;
									OW_DrawSpeed(VodPlayPara.reqScale);
								}
							}
						}
						else
						{
							//if (preReqScale != 0) //快进快退后播放
							{
								RTSP_PassBadFrame();
							}
						}
						
						FULLTOTVOD_ClearScreen();
						FULLTOTVOD_SetCurState(TVOD_PLAY_STATE);
						GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
					}
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
				UI_APP_JUMPBAR_ODC_Key(key);
			}
			break;

		
		case FULLTOTVOD_NULL_STATE:
		
			break;
			
		default:
			break;
	}
	
#endif
}





#define VOD

/*---------------------------正在连接---------------------------------------*/

void UI_APP_INFOCONNECT_Enter(void)
{
#ifdef USE_FZVOD	

	printf("UI_APP_INFOCONNECT_Enter\n");
	
	bGuiTimerSuspend = FYF_FALSE;
	bfirst = 1;
		
	if((RegionID == 0xff) /*&& (Ipanel_DLRegionId() != FYF_OK)*/)
	{
		OW_DrawInfo("无法获取区域ID!", FYF_TRUE, FYF_FALSE, FYF_FALSE);
		printf("Ipanel_DLRegionId Get Region Error\n");
		OW_ERROR("OW_VOD_MainMenu out from Ipanel_DLRegionId !!\n ");
		
		//FYF_API_thread_sleep(1000);

		GUI_ENC_API_SetValue(ID_STA_INFOCONNECT, WDT_STA_SV_STRING, "无法获取区域ID", 0);
	}
	else
		GUI_ENC_API_SetValue(ID_STA_INFOCONNECT, WDT_STA_SV_DATAID, RS_VOD_Connect, 0);
#endif
}


void UI_APP_INFOCONNECT_TIME(void)
{
#ifdef USE_FZVOD	

	printf("UI_APP_INFOCONNECT_TIME \n");
	
	if((RegionID == 0xff) /*&& (Ipanel_DLRegionId() != FYF_OK)*/)
	{
		OW_DrawInfo("无法获取区域ID!", FYF_TRUE, FYF_FALSE, FYF_FALSE);
		FYF_API_thread_sleep(1000);
		if (GUI_IpanelState != 0)
		{
			GUI_ENC_API_SwitchState(GUI_IpanelState,1);
		}
	}
	else
	{
		GUI_ENC_API_SwitchState(ID_DLG_VOD,1);
	}
#endif	
}

void UI_APP_INFOCONNECT_End(void)
{
#ifdef USE_FZVOD	

	printf("UI_APP_INFOCONNECT_End\n");
	bGuiTimerSuspend = FYF_TRUE;
	bfirst = 0;
#endif
}


/*--------------------------vod--------------------------------------*/
void UI_APP_VOD_JUMPTO_OnPaint(BU32 hdc,BU32 lparam)
{
#ifdef USE_FZVOD	

	BU08 showStr[30]={0};

	sprintf(showStr,"%02d:%02d:%02d   节目总长:%02d:%02d:%02d",g_vod_startHour,g_vod_startMin,g_vod_startsecd, g_vod_AllHour,g_vod_AllMin,g_vod_Allsecd);
	//sprintf(showStr,"%02d-%02d-%02d",g_vod_startHour,g_vod_startMin,g_vod_startsecd);
	GUI_API_DrawStyle(hdc, 0, 0, STYLE_JUMPTOBK_W340_H40);	
	if(g_vod_offset>=0&&g_vod_offset<2)
	{
		GUI_API_DrawHLine(hdc, g_vod_offset*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);			
	}
	else if(g_vod_offset>=2&&g_vod_offset<4)
	{
		GUI_API_DrawHLine(hdc, (g_vod_offset+1)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	else if(g_vod_offset>=4&&g_vod_offset<6)
	{
		GUI_API_DrawHLine(hdc, (g_vod_offset+2)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
	}
	
	GUI_API_TextOut(hdc, 0, 8, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);

#endif
}



void UI_APP_VOD_JUMPTO_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	

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
	BU08 bDataErr = FYF_FALSE;			//时间格式错误
	BU08 bDataOutRang = FYF_TRUE;		//时间超出范围
	BU08 bDataLarger = FYF_FALSE;		//时间格式错误
	BU08 bDataSmall = FYF_FALSE;			//时间超出范围
	
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
			switch(g_vod_offset)
			{
			case 0:
				g_vod_startHour = (BU08)((BU16)g_vod_startHour%10 + key*10);
				break;
			case 1:
				g_vod_startHour = (BU08)((BU16)g_vod_startHour-g_vod_startHour%10 + key);
				break;
			case 2:
				g_vod_startMin = (BU08)((BU16)g_vod_startMin%10 + key*10);
				break;
			case 3:
				g_vod_startMin = (BU08)((BU16)g_vod_startMin-g_vod_startMin%10 + key);
				break;
			case 4:
				g_vod_startsecd = (BU08)((BU16)g_vod_startsecd%10 + key*10);
				break;
			case 5:
				g_vod_startsecd = (BU08)((BU16)g_vod_startsecd-g_vod_startsecd%10 + key);
				break;
			default:
				break;
			}
			if(g_vod_startHour>=24)	g_vod_startHour = 0;
			if(g_vod_startMin>=60)	g_vod_startMin = 0;
			if(g_vod_startsecd>=60)	g_vod_startsecd = 0;
			g_vod_offset++;
			if(g_vod_offset>5)	
				g_vod_offset=0;
			GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
		break;
	case GUI_VK_LEFT:
			if(g_vod_offset==0)	
				g_vod_offset=5;
			else 
				g_vod_offset--;
			GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
		break;
	case GUI_VK_RIGHT:
			g_vod_offset++;
			if(g_vod_offset > 5)	
				g_vod_offset=0;
			GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
		break;
	case GUI_VK_SELECT:

		startSecs = VodPlayPara.pStartTime;
		endSecs = VodPlayPara.pEndTime;

		printf("time = %02d:%02d:%02d\n",g_vod_startHour,g_vod_startMin, g_vod_startsecd);


		
		hour = (BU08)g_vod_startHour;
		min = (BU08)g_vod_startMin;
		sec = (BU08) g_vod_startsecd;
		
		if (hour > 23 || min > 59 || sec > 59)
		{
			bDataErr = FYF_TRUE;
		}
		
		
		if (bDataErr)
		{
			//printf("\033[0;31m无效的时间跳转\033[0m\n");
			OW_ERROR("无效的时间跳转");
			GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET,WDT_STA_SV_STRING, "无效的时间跳转", 0);
			GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
			GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
			GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);

			FYF_API_thread_sleep(2000);
			
			FYF_API_OSD_FillRect(0, 0, 720, 350,0);
			FYF_API_OSD_Refresh(0, 0, 720, 350);
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}

			break;
		}


		
		CurSecs = hour * 3600 + min * 60 + sec;
		
		if(OW_GetRecordFlag() == FYF_TRUE)
		{
			if ((CurSecs <= recLongTime))
			{
				bDataOutRang = FYF_FALSE;
			}
			else
			{
				bDataOutRang = FYF_TRUE;
				bDataLarger = FYF_TRUE;
			}
		}
		else
		{
			if ((CurSecs >= startSecs) && (CurSecs <= endSecs))
			{
				bDataOutRang = FYF_FALSE;
			}
			else
			{
				bDataOutRang = FYF_TRUE;
				if (CurSecs < startSecs)
				{
					bDataSmall = FYF_TRUE;
				}
				else if (CurSecs > endSecs)
				{
					bDataLarger = FYF_TRUE;
				}
			}
		}

		printf("cursec = %d\n", CurSecs);


		
		if (bDataOutRang)
		{
			//TPGRAPH_DrawRectangle(x, y, width, hight, TRANSPARENTCOLOR);
		
			if (bDataLarger)
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET,WDT_STA_SV_STRING, "无效的时间跳转", 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
				GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);
				OW_ERROR("无效的时间跳转");
								
				FYF_API_thread_sleep(2000);

				FYF_API_OSD_FillRect(0, 0, 720, 350,0);
				FYF_API_OSD_Refresh(0, 0, 720, 350);
				
				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
					GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
					GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
					GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
					APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
				}
			}
			//return 2;/*hqx: do it later*/
			break;
		}

		
		if (0 == (endSecs - CurSecs))
		{
			CurSecs -= 1;
		}
		if(OW_GetRecordFlag() == FYF_TRUE)
		{
			if(CurSecs == 0)
			{
				CurSecs += 2;
			}
			else if(CurSecs == recLongTime)
			{
				CurSecs -= 2;
			}
			CurSecs += startSecs;
			
		}
		
		printf("cursec222 = %d\n", CurSecs);


		
		VOD_ClearScreen();
		
		
		VodPlayPara.pCurTime = CurSecs;
		VodPlayPara.reqScale = 1;
		
		if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
		{
			OW_ERROR("OW_DrawVODBPJump RTSP_CreatePLAY  err!!\n ");
		}
		else
		{
			VodPlayPara.reqScale = 1;
			//OW_ClearPP(); 
			RTSP_PassBadFrame();
		}/*hqx:do it later*/

		//GUI_ENC_API_SwitchState(ID_DLG_VOD_NULL, 1);
		GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPTO, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPTO, WS_DISABLE);
		GUI_ENC_API_Update(ID_STA_VOD_JUMPTO, 1);
		GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
		VOD_SetCurState(VOD_PLAY_STATE);
		
		if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
		{
			GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
			GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
			GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
			GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
			GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
			APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
		}
		break;
	case GUI_VK_EXIT:
		
		VOD_ClearScreen();
		GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPTO, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPTO, WS_DISABLE);
		GUI_ENC_API_Update(ID_STA_VOD_JUMPTO, 1);
		GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
		VOD_SetCurState(VOD_PLAY_STATE);
		
		if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
		{
			GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
			GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
			GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
			GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
			GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
			APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
		}
		break;

	case GUI_VK_UP:

			switch(g_vod_offset)
			{	
			case 0:
				g_vod_startHour = (BU08)((BU16)g_vod_startHour%10 + (g_vod_startHour/10 + 1)*10);
				break;
			case 1:
				g_vod_startHour = (BU08)((BU16)g_vod_startHour + 1);
				break;
			case 2:
				g_vod_startMin = (BU08)((BU16)g_vod_startMin%10 + (g_vod_startMin/10 + 1)*10);
				break;
			case 3:
				g_vod_startMin = (BU08)((BU16)g_vod_startMin + 1);
				break;
			case 4:
				g_vod_startsecd = (BU08)((BU16)g_vod_startsecd%10 + (g_vod_startsecd/10 + 1)*10);
				break;
			case 5:
				g_vod_startsecd = (BU08)((BU16)g_vod_startsecd + 1);
				break;
			default:
				break;
			}

			if(g_vod_startHour>=24)	
				g_vod_startHour = 0;
			if(g_vod_startMin>=60) 
				g_vod_startMin = 0;
			if(g_vod_startsecd>=60)	
				g_vod_startsecd = 0;

			GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
		break;

	
	case GUI_VK_DOWN:
		
		switch(g_vod_offset)
		{
		case 0:
			g_vod_startHour = (BU08)((BU16)g_vod_startHour%10 + (g_vod_startHour/10 - 1)*10);
			
			if(g_vod_startHour >=245 && g_vod_startHour <=255)	
			{
				g_vod_startHour = 23;
				
			}
			break;
		case 1:
			g_vod_startHour = (BU08)((BU16)g_vod_startHour - 1);
			
			if(g_vod_startHour >=245 && g_vod_startHour <=255)	
			{
				g_vod_startHour = 23 - (255 - g_vod_startHour);
				
			}
			break;
		case 2:
			g_vod_startMin = (BU08)((BU16)g_vod_startMin%10 + (g_vod_startMin/10 - 1)*10);
			
			if(g_vod_startMin >=245 && g_vod_startMin <=255)	
			{
				g_vod_startMin = 59;
				
			}
			break;
		case 3:
			g_vod_startMin = (BU08)((BU16)g_vod_startMin - 1);
			
			if(g_vod_startMin >=245 && g_vod_startMin <=255)	
			{
				g_vod_startMin = 59 - (255 - g_vod_startMin);
				
			}
			break;
		case 4:
			g_vod_startsecd = (BU08)((BU16)g_vod_startsecd%10 + (g_vod_startsecd/10 - 1)*10);
			
			if(g_vod_startsecd >=245 && g_vod_startsecd <=255)	
			{
				g_vod_startsecd = 59;
			}
			break;
		case 5:
			g_vod_startsecd = (BU08)((BU16)g_vod_startsecd - 1);
			
			if(g_vod_startsecd >=245 && g_vod_startsecd <=255)	
			{
				g_vod_startsecd = 59 - (255 - g_vod_startsecd);
			}
			break;
		default:
			break;
		}


		if(g_vod_startHour>=24)	
			g_vod_startHour = 0;
		if(g_vod_startMin>=60) 
			g_vod_startMin = 0;
		if(g_vod_startsecd>=60)	
			g_vod_startsecd = 0;
		
		GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
		break;

		
	default:
		break;	
	}
#endif	
}


void UI_APP_VOD_JUMPBAR_OnPaint(BU32 hdc,BU32 lparam)
{
#ifdef USE_FZVOD	

	BU08 showStr[100]={0};
	

	sprintf(showStr,"%02d:%02d:%02d %s 时长:%02d:%02d:%02d",g_vod_startHour,g_vod_startMin,g_vod_startsecd,g_adjTimeStr, g_vod_AllHour,g_vod_AllMin,g_vod_Allsecd);

	GUI_API_DrawStyle(hdc, 0, 0, STYLE_JUMPBAR_W420_H24);
	if (bInitVodJumpBar)
	{
		vod_progress = (g_CurSecs / g_perPasstime);
		printf("vod_progress = %d\n", vod_progress);
		//g_CurSecs = vod_progress * g_perPasstime;
		GUI_API_DrawStyle(hdc, (g_CurSecs / g_perPasstime)*STEP_LONG, 30, STYLE_VOD_SEEK_W20_H24);
		bInitVodJumpBar = FYF_FALSE;
		GUI_ENC_API_SetValue(ID_PBR_VOD_JUMPBAR,WDT_PBR_SV_CHANGE,(BU32)((g_CurSecs / g_perPasstime)*STEP_LONG),0);
		GUI_ENC_API_Update(ID_ODC_VOD_JUMPBAR,(g_CurSecs / g_perPasstime)+1);
	}
	else
	{
		printf("lparam = %d\n", lparam);
		GUI_API_DrawStyle(hdc, (lparam-1)*STEP_LONG, 30, STYLE_VOD_SEEK_W20_H24);
	}
	
	
	GUI_API_TextOut(hdc, 80, 6, showStr, strlen(showStr), COLOR_BLACK, COLOR_BLACK, 1);
#endif
}

void UI_APP_VOD_JUMPBAR_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	

	switch(key)
	{
		case GUI_VK_LEFT:
		case GUI_VK_P_DOWN:
			if(vod_progress >= 1)
			{
				vod_progress--;
			}

			
			g_CurSecs = vod_progress * g_perPasstime + g_startSecs;

			if (g_CurSecs <= g_startSecs || g_CurSecs == 0)
			{
				g_CurSecs = g_startSecs;
			}
			
			memset(g_adjTimeStr, 0, 30);
			OW_ParseSecToTimeStr(g_CurSecs - g_startSecs, g_adjTimeStr);

			printf("=========g_CurSecs = %d, vod_progress = %d, g_adjTimeStr = %s\n", g_CurSecs - g_startSecs,vod_progress, g_adjTimeStr);

			GUI_ENC_API_SetValue(ID_PBR_VOD_JUMPBAR,WDT_PBR_SV_CHANGE,(BU32)(vod_progress*STEP_LONG),0);
			GUI_ENC_API_Update(ID_ODC_VOD_JUMPBAR,vod_progress+1);

			break;
		case GUI_VK_RIGHT:
		case GUI_VK_P_UP:
			if(vod_progress <= (ALL_PRESS_TIMES - 1))
			{
				vod_progress++;
			}

			g_CurSecs = vod_progress * g_perPasstime + g_startSecs;

			if (vod_progress == ALL_PRESS_TIMES)
			{
				g_CurSecs = g_endSecs - 3;
			}
			if (g_CurSecs >= g_endSecs || g_CurSecs == 0)
			{
				g_CurSecs = g_endSecs;
			}

			
			memset(g_adjTimeStr, 0, 30);
			OW_ParseSecToTimeStr(g_CurSecs - g_startSecs, g_adjTimeStr);

			printf("=========g_CurSecs = %d, vod_progress = %d, adjTimeStr = %s\n", g_CurSecs - g_startSecs,vod_progress, g_adjTimeStr);
			GUI_ENC_API_SetValue(ID_PBR_VOD_JUMPBAR,WDT_PBR_SV_CHANGE,(BU32)(vod_progress*STEP_LONG),0);
			GUI_ENC_API_Update(ID_ODC_VOD_JUMPBAR,vod_progress+1);

			break;

		case GUI_VK_EXIT:

			VOD_ClearScreen();
			GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPBAR, WS_DISABLE);
			GUI_ENC_API_SetVisible(ID_PBR_VOD_JUMPBAR, WS_DISABLE);
			GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPBAR, WS_DISABLE);
			GUI_ENC_API_Update(ID_STA_VOD_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_PBR_VOD_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_ODC_VOD_JUMPBAR, 1);
			VOD_SetCurState(VOD_PLAY_STATE);
			
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
			
			break;
		
		case GUI_VK_SELECT:


			
			if (OW_GetRecordFlag() == FYF_TRUE)
			{	
				memset(g_adjTimeStr, 0, 30);
				OW_ParseSecToTimeStr(g_CurSecs - g_startSecs, g_adjTimeStr);				
			}
			else
			{
				memset(g_adjTimeStr, 0, 30);
				OW_ParseSecToTimeStr(g_CurSecs, g_adjTimeStr);
			}

			VodPlayPara.pCurTime = g_CurSecs;
			VodPlayPara.reqScale = 1;
			//OW_ClearPP();/*hqx:do it later*/
			VOD_ClearScreen();
			printf("\033[0;31mcursec = %d, adjTimeStr = %s\033[0m\n", g_CurSecs, g_adjTimeStr);
			if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("RTSP_CreatePAUSE  err!!\n ");
			}
			else
			{
				RTSP_PassBadFrame();
			}
			//FYF_API_thread_sleep(500);

			
			VOD_ClearScreen();
			GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPBAR, WS_DISABLE);
			GUI_ENC_API_SetVisible(ID_PBR_VOD_JUMPBAR, WS_DISABLE);
			GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPBAR, WS_DISABLE);
			GUI_ENC_API_Update(ID_STA_VOD_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_PBR_VOD_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_ODC_VOD_JUMPBAR, 1);
			VOD_SetCurState(VOD_PLAY_STATE);
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}

			break;
		default:
			break;
	}

#endif	
}


void UI_APP_VOD_VOL_ODC_Key(BU32 key)
{
#ifdef USE_FZVOD	

	BU32 index;
	int count = 0;
	int CurSecs = 0;
	char adjTimeStr[30] = { 0 };
	int startSecs = 0;
	int endSecs = 0;
	char hourStr[3] = { 0 };
	char minStr[3] = { 0 };
	char secStr[3] = { 0 };


	GUI_ENC_API_SetValue(ID_TIM_VOD, WDT_TIM_SV_PERIOD, 3000, 0);
	GUI_ENC_API_SetValue(ID_TIM_VOD, WDT_TIM_SV_REPLAY, 0, 0);
	
	switch (key)
	{
	
		case GUI_VK_V_DOWN: 	
		case GUI_VK_LEFT:
	
			printf("v_down==============\n");
			GUI_ENC_API_SetVisible(ID_VOI_VOD, WS_VISIBLE);
			GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
			GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValueUnPaint(ID_VOI_VOD, WDT_VOI_SV_KEY, key, 0);
			GUI_ENC_API_GetValue(ID_VOI_VOD, WDT_VOI_SV_INDEX, &index, 0);
			if (index == 0)
			{
				printf("Mute.\n");
				GUI_ENC_API_Update(ID_VOI_VOD, 1);
			}
			else
			{
				
				GUI_ENC_API_Update(ID_VOI_VOD, 1);
			}
			printf("index = %d\n", index);
		
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				printf("APP_AUDIO_MUTE_STATE==============\n");
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
			else
			{
				APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
				UI_APP_API_SetVolume();
			}
	
			break;
	
			
		case GUI_VK_RIGHT:
		case GUI_VK_V_UP:
		
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}
	
			GUI_ENC_API_SetVisible(ID_VOI_VOD, WS_VISIBLE);
			GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
			GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetValueUnPaint(ID_VOI_VOD, WDT_VOI_SV_KEY, key, 0);
			GUI_ENC_API_GetValue(ID_VOI_VOD, WDT_VOI_SV_INDEX, &index, 0);
			if (index == 0)
			{
				printf("Mute.\n");
				GUI_ENC_API_Update(ID_VOI_VOD, 1);
			}
			else
			{
				GUI_ENC_API_Update(ID_VOI_VOD, 1);
			}
			printf("index = %d\n", index);
	
			APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
			UI_APP_API_SetVolume();
			
			break;
		case GUI_VK_SELECT:
		case GUI_VK_EXIT:
	
			VOD_SetCurState(VOD_PLAY_STATE);
			GUI_ENC_API_SetFocus(ID_DLG_VOD);
			VOD_ClearScreen();
			
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
			break;
			
		case GUI_VK_MUTE:
			
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
				UI_APP_API_SetVolume();
			}
			else
			{
				VOD_ClearScreen();
				
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
				
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
	
			VOD_SetCurState(VOD_PLAY_STATE);
			break;
	
		case GUI_VK_P_DOWN: 	
		case GUI_VK_P_UP:
			
			bInitVodJumpBar = FYF_TRUE;
			
			g_startSecs = startSecs = VodPlayPara.pStartTime;
			g_endSecs = endSecs = VodPlayPara.pEndTime;
			if (OW_GetRecordFlag() == FYF_TRUE)
			{
				if (recLongTime > (VodPlayPara.pEndTime - VodPlayPara.pStartTime))
				{
					recLongTime = VodPlayPara.pEndTime - VodPlayPara.pStartTime;
				}
			
				if (recLongTime != -1)
				{
					if((recLongTime + startSecs) < endSecs)
					{
						endSecs = recLongTime + startSecs;
					}
				}
			}
			printf("start = %d, end = %d\n", startSecs, endSecs);
			if(OW_GetRecordFlag() == FYF_TRUE)
			{
				OW_ParseSecToTimeStr(recLongTime,adjTimeStr);
			}
			else
			{
				OW_ParseSecToTimeStr(endSecs,adjTimeStr);
			}
			
			printf("adjtimestr = %s\n",adjTimeStr);
			
			memcpy(hourStr, &adjTimeStr[0], 2);
			memcpy(minStr, &adjTimeStr[3], 2);
			memcpy(secStr, &adjTimeStr[6], 2);
			
			g_vod_AllHour = (BU08) atoi(hourStr);
			g_vod_AllMin= (BU08) atoi(minStr);
			g_vod_Allsecd= (BU08) atoi(secStr);
			
			
			printf("all time = %d:%d:%d\n",g_vod_AllHour,g_vod_AllMin, g_vod_Allsecd);
			
			
			count = 0;
			g_CurSecs = CurSecs = (OW_GetPts()	- PTSStart) ;
			
			while ((g_CurSecs < 0) && (count < 10))
			{
				g_CurSecs = (OW_GetPts()	- PTSStart);
				count++;
			}
	
			if ((g_CurSecs < 0) && (count >= 10))
			{
				count = 0;
				VOD_ClearScreen();
				APP_Com_IPI_StopProg(1);
				//OW_DLRTPInfo_StopChannel();
				GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
				GUI_ENC_API_Update(ID_STA_THANKS, 1);
	
				FYF_API_thread_sleep(1000);
				VOD_ClearScreen();
				
				OW_SetExitFlag(FYF_TRUE);	
				OW_SetIpanelSocketExit(FYF_TRUE);
				bPauseDisPly = FYF_FALSE;
				printf("((g_CurSecs < 0) && (count >= 4)), Exit vod==============\n");
				
				GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
				return;
			}
			count = 0;

			if (key == GUI_VK_P_DOWN)
			{
				if (g_CurSecs > g_PreCurSecs)
				{
					
				}
			}
			printf("cusr = %d\n", g_CurSecs);
			g_PreCurSecs = g_CurSecs;
			OW_ParseSecToTimeStr(CurSecs,adjTimeStr);
			printf("\033[0;31mCurSecs adjtimestr = %s\033[0m\n",adjTimeStr);
			memset(g_adjTimeStr, 0, 30);
			memcpy(g_adjTimeStr, adjTimeStr, strlen(adjTimeStr));
			
			memcpy(hourStr, &adjTimeStr[0], 2);
			memcpy(minStr, &adjTimeStr[3], 2);
			memcpy(secStr, &adjTimeStr[6], 2);
			
			g_vod_startHour = (BU08) atoi(hourStr);
			g_vod_startMin= (BU08) atoi(minStr);
			g_vod_startsecd= (BU08) atoi(secStr);
			
			
			g_perPasstime =  (g_endSecs - g_startSecs) / ALL_PRESS_TIMES;
			printf("time = %d:%d:%d, g_perPasstime = %d\n",g_vod_startHour,g_vod_startMin, g_vod_startsecd, g_perPasstime);
	
			
			GUI_ENC_API_SetFocus(ID_ODC_VOD_JUMPBAR);
			
			GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPBAR, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_PBR_VOD_JUMPBAR, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPBAR, WS_VISIBLE);
		
			GUI_ENC_API_Update(ID_STA_VOD_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_PBR_VOD_JUMPBAR, 1);
			GUI_ENC_API_Update(ID_ODC_VOD_JUMPBAR, 1);
			
			VOD_SetCurState(VOD_JUMPBAR_STATE);
			
			
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
				GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
				GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
			break;	
	
		case GUI_VK_F10:
		case GUI_VK_YELLOW: 
				
				VOD_ClearScreen();
				
				startSecs = VodPlayPara.pStartTime;
				endSecs = VodPlayPara.pEndTime;
				if (OW_GetRecordFlag() == FYF_TRUE)
				{
					if (recLongTime > (VodPlayPara.pEndTime - VodPlayPara.pStartTime))
					{
						recLongTime = VodPlayPara.pEndTime - VodPlayPara.pStartTime;
					}
				
					if (recLongTime != -1)
					{
						if((recLongTime + startSecs) < endSecs)
						{
							endSecs = recLongTime + startSecs;
						}
					}
				}
				printf("start = %d, end = %d\n", startSecs, endSecs);
				if(OW_GetRecordFlag() == FYF_TRUE)
				{
					OW_ParseSecToTimeStr(recLongTime,adjTimeStr);
				}
				else
				{
					OW_ParseSecToTimeStr(endSecs,adjTimeStr);
				}
				
				printf("adjtimestr = %s\n",adjTimeStr);
				
				memcpy(hourStr, &adjTimeStr[0], 2);
				memcpy(minStr, &adjTimeStr[3], 2);
				memcpy(secStr, &adjTimeStr[6], 2);
				
				g_vod_AllHour = (BU08) atoi(hourStr);
				g_vod_AllMin= (BU08) atoi(minStr);
				g_vod_Allsecd= (BU08) atoi(secStr);
				
				
				printf("all time = %d:%d:%d\n",g_vod_AllHour,g_vod_AllMin, g_vod_Allsecd);
			
			
				count = 0;
				CurSecs = (OW_GetPts()	- PTSStart);
				while ((CurSecs < 0) && (count < 10))
				{
					CurSecs = (OW_GetPts()	- PTSStart);
					count++;
				}
			
				if ((g_CurSecs < 0) && (count >= 10))
				{
					count = 0;
					VOD_ClearScreen();
					APP_Com_IPI_StopProg(1);
				
					//OW_DLRTPInfo_StopChannel();
					GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
					GUI_ENC_API_Update(ID_STA_THANKS, 1);
				
					FYF_API_thread_sleep(1000);
					VOD_ClearScreen();
					
					OW_SetExitFlag(FYF_TRUE);	
					
					OW_SetIpanelSocketExit(FYF_TRUE);
					bPauseDisPly = FYF_FALSE;
					printf("((g_CurSecs < 0) && (count >= 4)), Exit vod==============\n");
					
					GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
					return;
				}
				count = 0;
				
				printf("cusr = %d\n", CurSecs);
				OW_ParseSecToTimeStr(CurSecs,adjTimeStr);
				printf("CurSecs adjtimestr = %s\n",adjTimeStr);
				
				
				memcpy(hourStr, &adjTimeStr[0], 2);
				memcpy(minStr, &adjTimeStr[3], 2);
				memcpy(secStr, &adjTimeStr[6], 2);
				
				g_vod_startHour = (BU08) atoi(hourStr);
				g_vod_startMin= (BU08) atoi(minStr);
				g_vod_startsecd= (BU08) atoi(secStr);
				
				
				printf("time = %d:%d:%d\n",g_vod_startHour,g_vod_startMin, g_vod_startsecd);
				
			
				VOD_SetCurState(VOD_JUMPTO_STATE);
				
				GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPTO, WS_VISIBLE);
				GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPTO, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STA_VOD_JUMPTO, 1);
				GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
				GUI_ENC_API_SetFocus(ID_ODC_VOD_JUMPTO);
				
				
				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
					GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
					GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
					GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
					APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
				}
				
				break;
	
	}
#endif
}

void UI_APP_VOD_Enter(void)
{	
#ifdef USE_FZVOD	

	int volume_index = 0;
	printf("UI_APP_VOD_Enter\n");
	bfirst = 1;
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
	bGuiTimerSuspend = FYF_FALSE;
	bCableDownCount = 0;
	
	bPause = FYF_FALSE;
	bInitVodEnter = FYF_TRUE;

	VOD_ClearScreen();
	
	TVODInit();
	if (GUI_IpanelState == 0)
		OW_SetExitFlag(FYF_FALSE);
	
	VodPlayPara.VodOrTvod = 0;
	OW_VOD_SetNetErrFlag(FYF_FALSE);
	//GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "正在连接，请稍候", 0);

	GUI_ENC_API_SetValue(ID_TIM_VOD, WDT_TIM_SV_PERIOD, 1000, 0);
	//GUI_ENC_API_SetValue(ID_TIM_VOD, WDT_TIM_SV_REPLAY, 0, 0);
	
	GUI_ENC_API_SetValue(ID_STA_VOD_EXIT_PAUSE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_VOD_EXIT_CONTINUE,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	
	GUI_ENC_API_SetValue(ID_STA_VOD_PP_EXIT_LINE1,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_VOD_PP_EXIT_LINE2,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_VOD_PP_EXIT_LINE3,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	GUI_ENC_API_SetValue(ID_STA_THANKS,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);
	

	GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_DISABLE);

	GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPTO, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPTO, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_VOD_PP_EXIT, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_PP_EXIT_LINE1, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_PP_EXIT_LINE2, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_PP_EXIT_LINE3, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPBAR, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPBAR, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_PBR_VOD_JUMPBAR, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_VOI_VOD, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_DISABLE);
	
	GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_BAK, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_PAUSE, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_CONTINUE, WS_DISABLE);

	GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_BK, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_HELP, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE1, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE2, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE3, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE4, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE5, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE6, WS_DISABLE);

	if (ipanel_GetAudMuteState())
	{
		APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
	}
	else
	{
		APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
	}

	if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
	{
		VOD_ClearScreen();
		
		GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
		
		GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
		GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
		GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
		
		APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
	}
	else
	{
	
		GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
		GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
		APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &volume_index);
		printf("index = %d\n", volume_index);
		GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_RIGHT, volume_index, 0);
	}



	
	VOD_SetCurState(VOD_PLAY_STATE);
#endif
}

void UI_APP_VOD_End(void)
{
#ifdef USE_FZVOD	

	printf("UI_APP_VOD_End\n");
	
	bfirst = 0;
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
	bGuiTimerSuspend = FYF_TRUE;
	OW_SetExitFlag(FYF_FALSE);
	
	bInitVodEnter = FYF_FALSE;

	
	w = 0;
	r = 0;
	num = 0;
#endif	
}

void UI_APP_VOD_key(BU32 key)
{
#ifdef USE_FZVOD	

	BU32 curstate;

	BU32 index;
	int count = 0;
	int CurSecs = 0;
	char adjTimeStr[30] = { 0 };
	int startSecs = 0;
	int endSecs = 0;
	char hourStr[3] = { 0 };
	char minStr[3] = { 0 };
	char secStr[3] = { 0 };

	curstate = VOD_GetCurState();
	printf("Press Key.==%d= curstate = %d==============UI_APP_VOD_key\n",key, curstate);

	if (curstate != VOD_VOL_STATE)
	{
		GUI_ENC_API_SetValue(ID_TIM_VOD, WDT_TIM_SV_PERIOD, 10000, 0);
		GUI_ENC_API_SetValue(ID_TIM_VOD, WDT_TIM_SV_REPLAY, 0, 0);
	}

	//optionTimeCount = 0;
	if (bNetError || bTurnerError)
	{
		switch (key)
		{
			case GUI_VK_EXIT:
				
				printf("bNetError, OW_SetExitFlag==============UI_APP_VOD_key\n");
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
				
				GUI_ENC_API_SetVisible(ID_VOI_VOD, WS_VISIBLE);
				GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
				GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
				GUI_ENC_API_SetValueUnPaint(ID_VOI_VOD, WDT_VOI_SV_KEY, key, 0);
				
				GUI_ENC_API_GetValue(ID_VOI_VOD, WDT_VOI_SV_INDEX, &index, 0);
				if (index == 0)
				{
					printf("Mute.\n");
					//GUI_ENC_API_SetVisible(ID_STAVOI_ChnInfo, WS_VISIBLE);
					GUI_ENC_API_Update(ID_VOI_VOD, 1);
				}
				else
				{
					
					GUI_ENC_API_Update(ID_VOI_VOD, 1);
					
					if((APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState()) && ((key == GUI_VK_V_UP) || (key == GUI_VK_RIGHT)))
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
					}
				}
				printf("index = %d\n", index);
				
				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					printf("APP_AUDIO_MUTE_STATE==============\n");
					GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
					GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
					GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
					GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
					APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
				}
				else
				{
					APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
					UI_APP_API_SetVolume();
				}
				
				VOD_SetCurState(VOD_VOL_STATE);
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
						printf("vod, 服务器忙，请稍后再试\n");
						
						GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "服务器忙，请稍后再试", 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
						GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);

						FYF_API_thread_sleep(2000);
				
						VOD_ClearScreen();
					}
					else
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_CHECKNET, WDT_STA_SV_STRING, "网络不通，请退出点播", 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET_BAK, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_CHECKNET, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_CHECKNET_BAK, 1);
						GUI_ENC_API_Update(ID_STA_VOD_CHECKNET, 1);
					}

				break;
			default:
				break;
		}
		return;
	}

	switch (curstate)
	{

		case VOD_NULL_STATE: 			/*0*/

			break;

		case VOD_HELP_STATE:
			switch(key)
			{
				case GUI_VK_EXIT:
				case GUI_VK_SELECT:
					VOD_SetCurState(VOD_PLAY_STATE);
					GUI_ENC_API_SetFocus(ID_DLG_VOD);
					
					VOD_ClearScreen();
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					break;
					
				default:
					break;
			}
			break;

			
		case VOD_PAUSE_STATE:			/*6*/
			switch (key)
			{
				case GUI_VK_SELECT:
				case GUI_VK_EXIT:	
					
					/*if (VodPlayPara.reqScale == 1)
					{
						//preReqScale = owPlayPara.reqScale;
						VodPlayPara.reqScale = 0;
						bPause = FYF_TRUE;
					
						if (RTSP_CreatePAUSE(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePAUSE  err and recover!!\n ");
							//VodPlayPara.reqScale = preReqScale;
							bPause = FYF_FALSE;
						}
						else
						{
							OW_DrawSpeed(VodPlayPara.reqScale);
						}
						
						VOD_SetCurState(VOD_PAUSE_STATE);
						
						VOD_ClearScreen();
						
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_BAK, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_PAUSE, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_CONTINUE, WS_VISIBLE);
						
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_BAK, 1);
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_PAUSE, 1);
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_CONTINUE, 1);
					}*/
					if (VodPlayPara.reqScale != 1)
					{
						//preReqScale = VodPlayPara.reqScale;
						if (OW_GetRecordFlag() == FYF_TRUE)
						{
							VodPlayPara.pCurTime = recStarTime + (PTSCur - PTSStart) + 1;
							printf("bRecorded,recStarTime = %d, VodPlayPara.pCurTime = %d\n", recStarTime, VodPlayPara.pCurTime);
						}
						else
						{
							VodPlayPara.pCurTime = (PTSCur - PTSStart) + 1;
							printf("VodPlayPara.pCurTime = %d\n", VodPlayPara.pCurTime);
						}
						
						if((VodPlayPara.pCurTime + 1) < VodPlayPara.pEndTime)
						{
							VodPlayPara.pCurTime += 1;
						}
						
						bPause = FYF_FALSE;
						VodPlayPara.reqScale = 1;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
							//VodPlayPara.reqScale = preReqScale;
						}
						else
						{
							OW_DrawSpeed(VodPlayPara.reqScale);
							//RTSP_PassBadFrame();
						}

						VOD_SetCurState(VOD_PLAY_STATE);
						VOD_ClearScreen();
						
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_BAK, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_PAUSE, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_CONTINUE, WS_DISABLE);
						
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_BAK, 1);
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_PAUSE, 1);
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_CONTINUE, 1);
						
					}
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					break;


			}

			break;
			
		case VOD_VOL_STATE:
			
			UI_APP_VOD_VOL_ODC_Key(key);

			break;

			
		case VOD_PLAY_STATE: 			/*1*/
			switch(key)
			{
			
				case GUI_VK_F9:
				case GUI_VK_RED:			
				
					GUI_ENC_API_SetValue(ID_STA_VOD_HELP, WDT_STA_SV_STRING, "帮  助", 0);
					GUI_ENC_API_SetValue(ID_STA_VOD_HELP_LINE1, WDT_STA_SV_STRING, "[确认]键            暂停/继续", 0);
					GUI_ENC_API_SetValue(ID_STA_VOD_HELP_LINE2, WDT_STA_SV_STRING, "[退出]键            退出点播", 0);
					GUI_ENC_API_SetValue(ID_STA_VOD_HELP_LINE3, WDT_STA_SV_STRING, "[黄]键              选时播放", 0);
					GUI_ENC_API_SetValue(ID_STA_VOD_HELP_LINE4, WDT_STA_SV_STRING, "[红]键              帮助信息", 0);
					GUI_ENC_API_SetValue(ID_STA_VOD_HELP_LINE5, WDT_STA_SV_STRING, "[<-]/[->]键         进度条", 0);
					GUI_ENC_API_SetValue(ID_STA_VOD_HELP_LINE6, WDT_STA_SV_STRING, "[上翻页]/[下翻页]   进度条", 0);
				
					GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_BK, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_HELP, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE1, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE2, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE3, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE4, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE5, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_HELP_LINE6, WS_VISIBLE);
					
					GUI_ENC_API_Update(ID_STA_VOD_HELP_BK, 1);
					GUI_ENC_API_Update(ID_STA_VOD_HELP, 1);
					GUI_ENC_API_Update(ID_STA_VOD_HELP_LINE1, 1);
					GUI_ENC_API_Update(ID_STA_VOD_HELP_LINE2, 1);
					GUI_ENC_API_Update(ID_STA_VOD_HELP_LINE3, 1);
					GUI_ENC_API_Update(ID_STA_VOD_HELP_LINE4, 1);
					GUI_ENC_API_Update(ID_STA_VOD_HELP_LINE5, 1);
					GUI_ENC_API_Update(ID_STA_VOD_HELP_LINE6, 1);
					//GUI_ENC_API_SetFocus(ID_DLG_FULLTOTVOD);
					VOD_SetCurState(VOD_HELP_STATE);
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}

					break;
					
				case GUI_VK_MUTE:
					
					VOD_ClearScreen();
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);	
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
						UI_APP_API_SetVolume();
					}
					else
					{
						VOD_ClearScreen();
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					
					VOD_SetCurState(VOD_VOL_STATE);
					break;
					
				case GUI_VK_EXIT:
					VOD_SetCurState(VOD_PP_EXIT_STATE);
					
					VOD_ClearScreen();
					GUI_ENC_API_SetVisible(ID_STA_VOD_PP_EXIT, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_PP_EXIT_LINE1, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_PP_EXIT_LINE2, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_PP_EXIT_LINE3, WS_VISIBLE);
					
					GUI_ENC_API_Update(ID_STA_VOD_PP_EXIT, 1);
					GUI_ENC_API_Update(ID_STA_VOD_PP_EXIT_LINE1, 1);
					GUI_ENC_API_Update(ID_STA_VOD_PP_EXIT_LINE2, 1);
					GUI_ENC_API_Update(ID_STA_VOD_PP_EXIT_LINE3, 1);
				
				
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					break;
				case GUI_VK_SELECT:
					
					if (VodPlayPara.reqScale == 1)
					{
						//preReqScale = owPlayPara.reqScale;
						VodPlayPara.reqScale = 0;
						bPause = FYF_TRUE;
						PTSCur = OW_GetPts();
						printf("\033[0;31mPTSCur = %d\033[0m\n", PTSCur);
						if (RTSP_CreatePAUSE(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePAUSE  err and recover!!\n ");
							//VodPlayPara.reqScale = preReqScale;
							bPause = FYF_FALSE;
						}
						else
						{
							OW_DrawSpeed(VodPlayPara.reqScale);
						}
						
							
						VOD_SetCurState(VOD_PAUSE_STATE);
						
						VOD_ClearScreen();
						
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_BAK, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_PAUSE, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_CONTINUE, WS_VISIBLE);
						
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_BAK, 1);
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_PAUSE, 1);
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_CONTINUE, 1);
						
					}

					
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					/*else
					{
						//preReqScale = VodPlayPara.reqScale;
						if (OW_GetRecordFlag() == FYF_TRUE)
						{
							VodPlayPara.pCurTime = recStarTime + (OW_GetPts() - PTSStart) + 1;
							printf("bRecorded, VodPlayPara.pCurTime = %d\n", VodPlayPara.pCurTime);
						}
						else
						{
							VodPlayPara.pCurTime = (OW_GetPts() - PTSStart) + 1;
							printf("VodPlayPara.pCurTime = %d\n", VodPlayPara.pCurTime);
						}
						
						if((VodPlayPara.pCurTime + 1) < VodPlayPara.pEndTime)
						{
							VodPlayPara.pCurTime += 1;
						}
						
						bPause = FYF_FALSE;
						VodPlayPara.reqScale = 1;
						if (RTSP_CreatePLAY(optionSocket, &VodPlayPara) == FYF_ERR)
						{
							RTSP_CheckNetOk();
							OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  err!!\n ");
							//VodPlayPara.reqScale = preReqScale;
						}
						else
						{
							OW_DrawSpeed(VodPlayPara.reqScale);
						}
					
						VOD_ClearScreen();
						
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_BAK, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_PAUSE, WS_DISABLE);
						GUI_ENC_API_SetVisible(ID_STA_VOD_EXIT_CONTINUE, WS_DISABLE);
						
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_BAK, 1);
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_PAUSE, 1);
						GUI_ENC_API_Update(ID_STA_VOD_EXIT_CONTINUE, 1);
						
					}*/
					break;
					
				case GUI_VK_P_DOWN:	
				case GUI_VK_P_UP:
				case GUI_VK_LEFT:
				case GUI_VK_RIGHT:
					
					bInitVodJumpBar = FYF_TRUE;
					
					g_startSecs = startSecs = VodPlayPara.pStartTime;
					g_endSecs = endSecs = VodPlayPara.pEndTime;
					if (OW_GetRecordFlag() == FYF_TRUE)
					{
						if (recLongTime > (VodPlayPara.pEndTime - VodPlayPara.pStartTime))
						{
							recLongTime = VodPlayPara.pEndTime - VodPlayPara.pStartTime;
						}
					
						if (recLongTime != -1)
						{
							if((recLongTime + startSecs) < endSecs)
							{
								endSecs = recLongTime + startSecs;
							}
						}
					}
					printf("start = %d, end = %d\n", startSecs, endSecs);
					if(OW_GetRecordFlag() == FYF_TRUE)
					{
						OW_ParseSecToTimeStr(recLongTime,adjTimeStr);
					}
					else
					{
						OW_ParseSecToTimeStr(endSecs,adjTimeStr);
					}
					
					printf("adjtimestr = %s\n",adjTimeStr);
					
					memcpy(hourStr, &adjTimeStr[0], 2);
					memcpy(minStr, &adjTimeStr[3], 2);
					memcpy(secStr, &adjTimeStr[6], 2);
					
					g_vod_AllHour = (BU08) atoi(hourStr);
					g_vod_AllMin= (BU08) atoi(minStr);
					g_vod_Allsecd= (BU08) atoi(secStr);
					
					
					printf("all time = %d:%d:%d\n",g_vod_AllHour,g_vod_AllMin, g_vod_Allsecd);
					
					
					count = 0;
					g_CurSecs = CurSecs = (OW_GetPts()	- PTSStart) ;
					while ((g_CurSecs < 0) && (count < 10))
					{
						g_CurSecs = (OW_GetPts() - PTSStart);
						count++;
					}

					if ((g_CurSecs < 0) && (count >= 10))
					{
						count = 0;
						VOD_ClearScreen();
						APP_Com_IPI_StopProg(1);
						//OW_DLRTPInfo_StopChannel();
						GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
						GUI_ENC_API_Update(ID_STA_THANKS, 1);

						FYF_API_thread_sleep(1000);
						VOD_ClearScreen();
						
						OW_SetExitFlag(FYF_TRUE);	
						
						OW_SetIpanelSocketExit(FYF_TRUE);
						bPauseDisPly = FYF_FALSE;
						printf("((g_CurSecs < 0) && (count >= 4)), Exit vod==============\n");
						
						GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
						return;
					}
					count = 0;
			
					printf("cusr = %d\n", g_CurSecs);
					OW_ParseSecToTimeStr(CurSecs,adjTimeStr);
					printf("\033[0;31mCurSecs adjtimestr = %s\033[0m\n",adjTimeStr);
					memset(g_adjTimeStr, 0, 30);
					memcpy(g_adjTimeStr, adjTimeStr, strlen(adjTimeStr));
					
					memcpy(hourStr, &adjTimeStr[0], 2);
					memcpy(minStr, &adjTimeStr[3], 2);
					memcpy(secStr, &adjTimeStr[6], 2);
					
					g_vod_startHour = (BU08) atoi(hourStr);
					g_vod_startMin= (BU08) atoi(minStr);
					g_vod_startsecd= (BU08) atoi(secStr);
					
					
					g_perPasstime =  (g_endSecs - g_startSecs) / ALL_PRESS_TIMES;
					printf("time = %d:%d:%d, g_perPasstime = %d\n",g_vod_startHour,g_vod_startMin, g_vod_startsecd, g_perPasstime);

					
					GUI_ENC_API_SetFocus(ID_ODC_VOD_JUMPBAR);
					
					GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPBAR, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_PBR_VOD_JUMPBAR, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPBAR, WS_VISIBLE);
				
					GUI_ENC_API_Update(ID_STA_VOD_JUMPBAR, 1);
					GUI_ENC_API_Update(ID_PBR_VOD_JUMPBAR, 1);
					GUI_ENC_API_Update(ID_ODC_VOD_JUMPBAR, 1);
					
					VOD_SetCurState(VOD_JUMPBAR_STATE);
					
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					break;

					
				case GUI_VK_V_UP:
				case GUI_VK_V_DOWN:

											
					GUI_ENC_API_SetVisible(ID_VOI_VOD, WS_VISIBLE);
					GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_COLOR, COLOR_DUCKBLUE, 0);
					GUI_ENC_API_SetValue(ID_VOI_VOD, WDT_VOI_SV_FONTCOLOR, COLOR_WHITE, 0);
					GUI_ENC_API_SetValueUnPaint(ID_VOI_VOD, WDT_VOI_SV_KEY, key, 0);
					
					GUI_ENC_API_GetValue(ID_VOI_VOD, WDT_VOI_SV_INDEX, &index, 0);
					if (index == 0)
					{
						printf("Mute.\n");
						//GUI_ENC_API_SetVisible(ID_STAVOI_ChnInfo, WS_VISIBLE);
						GUI_ENC_API_Update(ID_VOI_VOD, 1);
					}
					else
					{
						
						GUI_ENC_API_Update(ID_VOI_VOD, 1);
						
						if((APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState()) && ((key == GUI_VK_V_UP) || (key == GUI_VK_RIGHT)))
						{
							GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
							GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
							APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
						}
					}
					printf("index = %d\n", index);
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						printf("APP_AUDIO_MUTE_STATE==============\n");
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					else
					{
						APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
						UI_APP_API_SetVolume();
					}
					
					VOD_SetCurState(VOD_VOL_STATE);
				
					
					break;
				
				case GUI_VK_F10:
				case GUI_VK_YELLOW: 
					VOD_ClearScreen();
					
					startSecs = VodPlayPara.pStartTime;
					endSecs = VodPlayPara.pEndTime;
					if (OW_GetRecordFlag() == FYF_TRUE)
					{
						if (recLongTime > (VodPlayPara.pEndTime - VodPlayPara.pStartTime))
						{
							recLongTime = VodPlayPara.pEndTime - VodPlayPara.pStartTime;
						}
					
						if (recLongTime != -1)
						{
							if((recLongTime + startSecs) < endSecs)
							{
								endSecs = recLongTime + startSecs;
							}
						}
					}
					printf("start = %d, end = %d\n", startSecs, endSecs);
					if(OW_GetRecordFlag() == FYF_TRUE)
					{
						OW_ParseSecToTimeStr(recLongTime,adjTimeStr);
					}
					else
					{
						OW_ParseSecToTimeStr(endSecs,adjTimeStr);
					}
					
					printf("adjtimestr = %s\n",adjTimeStr);
					
					memcpy(hourStr, &adjTimeStr[0], 2);
					memcpy(minStr, &adjTimeStr[3], 2);
					memcpy(secStr, &adjTimeStr[6], 2);
					
					g_vod_AllHour = (BU08) atoi(hourStr);
					g_vod_AllMin= (BU08) atoi(minStr);
					g_vod_Allsecd= (BU08) atoi(secStr);
					
					
					printf("all time = %d:%d:%d\n",g_vod_AllHour,g_vod_AllMin, g_vod_Allsecd);
				
				
					count = 0;
					CurSecs = (OW_GetPts()	- PTSStart);
					while ((CurSecs < 0) && (count < 10))
					{
						CurSecs = (OW_GetPts()	- PTSStart);
						count++;
					}
				
					if ((g_CurSecs < 0) && (count >= 10))
					{
						count = 0;
						VOD_ClearScreen();
						APP_Com_IPI_StopProg(1);
						//OW_DLRTPInfo_StopChannel();
						GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
						GUI_ENC_API_Update(ID_STA_THANKS, 1);
					
						FYF_API_thread_sleep(1000);
						VOD_ClearScreen();
						
						OW_SetExitFlag(FYF_TRUE);
						
						OW_SetIpanelSocketExit(FYF_TRUE);
						bPauseDisPly = FYF_FALSE;
						printf("((g_CurSecs < 0) && (count >= 4)), Exit vod==============\n");
						
						GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
						return;
					}
					count = 0;
					
					printf("cusr = %d\n", CurSecs);
					OW_ParseSecToTimeStr(CurSecs,adjTimeStr);
					printf("CurSecs adjtimestr = %s\n",adjTimeStr);
					
					
					memcpy(hourStr, &adjTimeStr[0], 2);
					memcpy(minStr, &adjTimeStr[3], 2);
					memcpy(secStr, &adjTimeStr[6], 2);
					
					g_vod_startHour = (BU08) atoi(hourStr);
					g_vod_startMin= (BU08) atoi(minStr);
					g_vod_startsecd= (BU08) atoi(secStr);
					
					
					printf("time = %d:%d:%d\n",g_vod_startHour,g_vod_startMin, g_vod_startsecd);
					
				
					VOD_SetCurState(VOD_JUMPTO_STATE);
					
					GUI_ENC_API_SetVisible(ID_STA_VOD_JUMPTO, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_ODC_VOD_JUMPTO, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STA_VOD_JUMPTO, 1);
					GUI_ENC_API_Update(ID_ODC_VOD_JUMPTO, 1);
					GUI_ENC_API_SetFocus(ID_ODC_VOD_JUMPTO);
					
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}
					break;
				default:
					break;

			}
			break;
			
		case VOD_PP_EXIT_STATE: 			/*2*/
			switch (key)
			{
				case GUI_VK_EXIT:
					VOD_SetCurState(VOD_PLAY_STATE);
					GUI_ENC_API_SetFocus(ID_DLG_VOD);
					
					VOD_ClearScreen();
					
					
					if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
					{
						GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
						GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
						GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
						APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
					}

					break;
				case GUI_VK_SELECT:
					//if (GUI_IpanelState == 0)
					{		
					
						VOD_ClearScreen();
						
						APP_Com_API_StopProg(1);
						//OW_DLRTPInfo_StopChannel();

						GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
						GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
						GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
						GUI_ENC_API_Update(ID_STA_THANKS, 1);

						FYF_API_thread_sleep(1000);
						//VOD_ClearScreen();
						
						OW_SetExitFlag(FYF_TRUE);	
						bPauseDisPly = FYF_FALSE;
						printf("VOD_PP_EXIT_STATE, Exit vod==============\n");
						
						//GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
					}
				
					break;
				default:
					break;
			}
			break;
			
		case VOD_JUMPTO_STATE: 			/*3*/
			
			if (GUI_ENC_API_IsFocus(ID_ODC_VOD_JUMPTO))
			{
				UI_APP_VOD_JUMPTO_ODC_Key(key);
			}
			break;
			
		case VOD_JUMPBAR_STATE: 		/*4*/
			
			if (GUI_ENC_API_IsFocus(ID_ODC_VOD_JUMPBAR))
			{
				UI_APP_VOD_JUMPBAR_ODC_Key(key);
			}
			
			break;


		default:
			break;
	}
		
#endif
}


void UI_APP_VOD_TIME(void)
{
#ifdef USE_FZVOD	

	printf("UI_APP_VOD_TIME\n");

	if (!bNetError && !bTurnerError)
	{
		if  ((VOD_GetCurState() != VOD_PP_EXIT_STATE) && (VOD_GetCurState() != VOD_PAUSE_STATE) && (VOD_GetCurState() != VOD_HELP_STATE))
		{
			VOD_SetCurState(VOD_PLAY_STATE);
			GUI_ENC_API_SetFocus(ID_DLG_VOD);
			VOD_ClearScreen();
		}
		
		if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
		{
			GUI_ENC_API_SetValue(ID_STA_VOD_MUTE, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
			GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE_BAK, WS_VISIBLE);						
			GUI_ENC_API_SetVisible(ID_STA_VOD_MUTE, WS_VISIBLE);
			GUI_ENC_API_Update(ID_STA_VOD_MUTE_BAK,1);
			GUI_ENC_API_Update(ID_STA_VOD_MUTE,1);
			APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
		}
	}

#endif	
}


#define FIRST_STATE

int SYSTEM_GetNET_ON_OFF(void)
{
	BU32 value = 0;
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

extern BU08 IPAddress[16];
void UI_APP_VOD_FIRST_STATE_Enter(void)
{
#ifdef USE_FZVOD	

	int ret = -1;
	printf("UI_APP_VOD_FIRST_STATE_Enter\n");
	
	GUI_ENC_API_SetValue(ID_STA_VOD_FIRST_INFO,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

	GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO, WS_DISABLE);
	GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO_BAK, WS_DISABLE);
	
	//Ipanel_DLRegionId(); 
	
	ret = TP_Ipanel_CheckNetOk();

	g_firststate_errorcode = ret;
	if (ret == FYF_OK)
	{
	
		if (RTSP_TVOD_GetProInfo("huabeiyoutian-tsg.wasu.cn", "5777") != FYF_OK)
		{
			OW_ERROR("RTSP_TVOD_GetProInfo err!\n ");
		}
	}
	else if (-2 == ret)
	{
		GUI_ENC_API_SetValue(ID_STA_VOD_FIRST_INFO,WDT_STA_SV_STRING,"请检查网络连接",0);
		GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO_BAK, WS_VISIBLE);

	}
	else if (-3 == ret)
	{
		GUI_ENC_API_SetValue(ID_STA_VOD_FIRST_INFO,WDT_STA_SV_STRING,"正在获取ip,请稍候",0);
		GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO_BAK, WS_VISIBLE);
	}
	
	

	g_firststateInit = FYF_TRUE;
#endif
}


void UI_APP_VOD_FIRST_STATE_End(void)
{
#ifdef USE_FZVOD	

	printf("UI_APP_VOD_FIRST_STATE_End\n");
#endif
}


void UI_APP_VOD_FIRST_STATE_Timer(void)
{
#ifdef USE_FZVOD	

	BU32 value;
	int ret = -1;

	printf("UI_APP_VOD_FIRST_STATE_Timer, g_firststate_errorcode = %d\n", g_firststate_errorcode);
	switch (g_firststate_errorcode)
	{
		case FYF_OK:
			
			if (g_firststateInit)
			{
				//g_firststateInit = FYF_FALSE;
				
				Ipanel_SetEnterVodOrTvod_Flag(FYF_TRUE);
				APP_Com_API_StopProg(1);
				Ipanel_SetEntrance_Flag(FYF_TRUE);
				OW_VOD_SetDhcpFlag(FYF_TRUE);
			
				GUI_ENC_API_SwitchState(ID_DLG_Null,1);
			}

			break;
		case -2:
			g_firststateInit = FYF_FALSE;
			g_firststate_errorcode = 0;

			FYF_API_thread_sleep(2000);
				
			FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
			FYF_API_OSD_Refresh(0, 0, 720, 576);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
			break;

			
		case -3:
			
			g_firststate_errorcode = 0;
			//g_firststateInit = FYF_FALSE;

			/*APP_SYS_API_GetSysInfo(APP_SYS_AutoIP_e, &value);
			if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
			{
				if (value == 0)
				{	
					system("killall -9 udhcpc");
					system("udhcpc -V HZCNCSTBV1 -n 2");
					system("killall -9 udhcpc");
				}

				ret = TP_Ipanel_CheckNetOk();
				if (ret == FYF_OK)
				{
					if (g_firststateInit)
					{
						//g_firststateInit = FYF_FALSE;
						
						Ipanel_SetEnterVodOrTvod_Flag(FYF_TRUE);
						APP_Com_API_StopProg(1);
						Ipanel_SetEntrance_Flag(FYF_TRUE);
						OW_VOD_SetDhcpFlag(FYF_TRUE);
					
						GUI_ENC_API_SwitchState(ID_DLG_Null,1);
					}
				}
				else if (-2 == ret)
				{
					g_firststateInit = FYF_FALSE;
					GUI_ENC_API_SetValue(ID_STA_VOD_FIRST_INFO,WDT_STA_SV_STRING,"请检查网络连接",0);
					GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO_BAK, WS_VISIBLE);
					GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
				}
				else if (-3 == ret)
				{
					g_firststateInit = FYF_FALSE;
					GUI_ENC_API_SetValue(ID_STA_VOD_FIRST_INFO,WDT_STA_SV_STRING,"无法获取IP地址",0);
					GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO, WS_VISIBLE);
					GUI_ENC_API_SetVisible(ID_STA_VOD_FIRST_INFO_BAK, WS_VISIBLE);
					GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
				}
			}
			else
			{
				
			}*/
			
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
			g_firststateInit = FYF_FALSE;
			break;
	}

#endif	
}




