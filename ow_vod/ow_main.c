/*----------------------------------------------------------------------------
   File Name:ow_main.c

   Description: 

   Version:REL_V1.0.0

   Copyright (c) 2006-2100 Triumph Corporation.  All rights reserved.
-----------------------------------------------------------------------------*/

/* Includes ---------------------------------------------------------------- */

#include "ow_debug.h"
//#include "../tpversion/ui_demo/osd/include/wndId.h"
#include "../tpmiddle/include/app_sys.h"
#include "../tpmiddle/include/app_common.h"


OW_PLAY_PARA VodPlayPara;

static BU08  bDisplayTimeshift = FYF_FALSE;		/*是否时移节目*/
static BU08  bOW_VOD_Exit = FYF_FALSE;			/*是否退出点播或者时移*/
static int 	 optionMSGErrCount = 0;				/*心跳信息容错5次,即option连续收发5次错误,判断为网络故障*/
static BU08  bFirstDrawPause = FYF_FALSE;		/*暂停键进入时移时第一次画暂停要显示时间*/
static BU08  bRecorded = FYF_FALSE;				/*设置录播标志*/
static BU08  bInVODorTVod = FYF_FALSE;
BU32 VodState = 0;
int optionSocket = -1;
BU32 PTSStart = 0;
static BU08  bOW_Setsocket_Exit = FYF_FALSE;

BU08 bTVODPlay = FYF_FALSE;				//进入时移并播放过节目的标志,因为按选时键进入时如果超长时间未确认则直接退出，此时退出不必续播
BU08 bPlayFromStart = FYF_FALSE;			//
BU08 bPauseDisPly = FYF_FALSE;
BU08 bExitSocket = FYF_FALSE;

#ifdef USE_IPANEL
//extern 	void Ipanel_SetEnterVodOrTvod_Flag(BU08 flag);
extern  BU32 GUI_IpanelState;

#endif
extern  BS32 Ipanel_DLRegionId(void);
extern  int  recStarTime;
extern  int  recLongTime;
extern  BU08 serSetParaType;				//服务器发给机顶盒异常消息类型 0: 节目关闭; 1:节目开头;2:节目结尾

#ifdef USE_FZVOD
//extern BS32 s_pVodMsgQueue;
#endif
extern BU08 bGuiTimerSuspend;
extern BU08 bfirst;
extern 	BS32 RegionID;
extern int noKeytimeCount;
extern BU08 bDrawSpeed;
extern 	OW_TVOD_CHANNELS   TvodChanInfo;
extern 	int curChannelNum;
extern BU08 bGetTsgSuc;
static BS32 s_tvod_threadId = 0;


BU08 bIntvod = FYF_FALSE;
int  optionTimeCount = 0;
int  NetErrorCount = 0;
BU08 bNetError = FYF_FALSE;
extern BU08 bInitVodEnter;
extern char getipaddr[20];

extern BU08 bPause;
extern BU08 IPAddress[16];
extern BU08 SubNetMark[16];
extern	BU08 DefaultGateway[16];
extern NetSetInfo IpInfomation;
extern BU08 FirstChoseDNS[16];
extern BU08 ReserveDNS[16];

extern BU08 bHadRes;
extern 	void CallBackGetTableFunc(BU32 channelID, BU32 filterID, BU08* pSection, BS32 len);
extern BU32 ptsValueInMS;
extern 	HI_U32 w,r;
extern HI_U32 num;
extern BU08 bTVOD;
BU08  g_UseMP2 = FYF_TRUE;
/*---------------------------------------------------------------------------*/

void OW_SetMPEGType(BU08 flag)
{
	g_UseMP2 = flag;
}

BU08 OW_GetMPEGType(void)
{
	printf("g_usemp2 = %d\n", g_UseMP2);
	return g_UseMP2;
}



int Tp_Ow_CheckTvodChannel(void)
{
	if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
	{
		//if (TPUSIF_GetbRestartOK())
		{
			return RTSP_CheckChannel();
		}
	}
	
	return FYF_ERR;
}



void OW_SetFirstPauseFlag(BU08 flag)
{
	bFirstDrawPause = flag;
}

BU08 OW_GetFirstPauseFlag(void)
{
	return bFirstDrawPause;
}

void OW_SetVODFlag(BU08 flag)
{
	bInVODorTVod = flag;
}

BU08 OW_GetVODFlag(void)
{
	return bInVODorTVod;
}



void OW_SetRecordFlag(BU08 flag)
{
	bRecorded = flag;
}

BU08 OW_GetRecordFlag(void)
{
	return bRecorded;
}

void OW_SetIpanelSocketExit(BU08 flag)
{
	bOW_Setsocket_Exit = flag;
}

BU08 OW_GetIpanelSocketExit(void)
{
	return bOW_Setsocket_Exit;
}



void OW_SetExitFlag(BU08 flag)
{
	bOW_VOD_Exit = flag;
}

BU08 OW_GetExitFlag(void)
{
	return bOW_VOD_Exit;
}

void OW_IncreOptionErrCount(void)
{
	optionMSGErrCount++;
}

void OW_DefaultOptionErrCount(void)
{
	optionMSGErrCount = 0;
}

BU08 OW_GetOptionErrCount(void)
{
	return optionMSGErrCount;
}

int OW_CheckOption(void)							/*option中断5分钟后退出*/
{
	if(OW_GetOptionErrCount() > 5)
	{
		if(OW_GetOptionErrCount() == 10)
		{
			OW_SetExitFlag(FYF_TRUE);
			printf("TP_OW_CheckOption option stop for 5 min!!!out from\n");
			return FYF_OK;
		}
	}
	return FYF_ERR;
}


/*-----------------------------------------------------------------------------
	函数名：OW_VOD_GetDisplayTimeshift_Flag
		
	描述：
			在显示频道时，获取是否需要显示该节目为时移节目的标志
	参数：
			无
	返回：
			标志位
------------------------------------------------------------------------------*/

BU08 OW_VOD_GetDisplayTimeshift_Flag(void)
{
	return bDisplayTimeshift;
}

/*-----------------------------------------------------------------------------
	函数名：OW_VOD_SetDisplayTimeshift_Flag
		
	描述：
			在显示频道时，设置该节目为时移节目的标志
	参数：
			in: 
				flag: 设置该节目为时移节目的标志
	返回：
			无
------------------------------------------------------------------------------*/

void OW_VOD_SetDisplayTimeshift_Flag(BU08 flag)
{
	bDisplayTimeshift = flag;
}



BU64 OW_GetPts(void)
{
	BU64 pts = 0;
	int count = 0;
#ifdef USE_HI3110

	FYF_API_SetPtsValue(pts);
	FYF_API_GetPts();
	for (count = 0; count < 160; count++)
	{
		pts = FYF_API_GetPtsValue();
		
		//printf("\033[0;31m@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22   count = %d, pts = %lld\033[0m\n",count, pts);
		if (pts != 0)
		{
			//printf("count = %d,pts = %lld\n",count, pts);
			FYF_API_InitRegist();
			break;
		}
		FYF_API_thread_sleep(10);
	}
#else
		
	if (OW_GetMPEGType() == FYF_TRUE)
	{
		for (count = 0; count < 160; count++)
		{
			pts = FYF_API_GetPtsValue();
			if	(pts != 0)
			{
				break;
			}
			FYF_API_thread_sleep(10);
		}
	}
	else
	{
		pts = ptsValueInMS ;
		printf("\033[0;31mpts = %d\n\033[0m",pts);
	}
	
#endif


	return pts;
}

/*---------------------------------------------------------------------------*/

/*int Ip_Getip(void)
{
	
	FYF_API_ETH_IPAddressGet(getipaddr);
	if (strlen(getipaddr) == 0)
	{
		OW_ERROR("FYF_API_ETH_IPAddressGet getip = %s\n",getipaddr);
		return -3;
	}
	printf("get_ipaddr = %s\n", getipaddr);

	return FYF_OK;
}*/

void  IpDhcpMonitor( void *pvParam )	
{
	int ret = -1;
	static int tsg_count = 0;

	while (1)
	{
		/*ret = FYF_API_GetLinkStatus();
		if (ret != 1)
		{

		}
		else if (Ip_Getip != FYF_OK)
		{
			
		}*/
		FYF_API_thread_sleep(1000);
		if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE && !Inthird_GetFlag() && GUI_ENC_API_GetCurState() == ID_DLG_ChnInfo) 
		{
			tsg_count++;
			if (tsg_count == 7)
			{	
				tsg_count = 0;
				ret = RTSP_RetryTsgService();
				if (ret != FYF_OK)
				{
					ret = Tp_Ow_CheckTvodChannel();
				}
			}
		}
	}
}


void TVOD_MainThread(void *p)
{
	int ret = -1;
	
	BU08 stopTag = FYF_FALSE;
	unsigned int playTime = 0;
	BU16 pauseTimeCount = 0;
	
	while (1)
	{
		
		FYF_API_thread_sleep(1000);

		if (!Inthird_GetFlag() && (Ow_Get_entervodortvod_Flag() == FYF_FALSE))
		{
			//printf("not Inthird_GetFlag()               EXITFLAG = %d ================================================\n", OW_GetExitFlag());
			continue;
		}
				
		//printf("Inthird_GetFlag()				  EXITFLAG = %d================================================\n", OW_GetExitFlag());
		if (OW_GetExitFlag() == FYF_FALSE)
		{
			RTSP_CheckTunerOk();
			ret = RTSP_CheckNetOk();
			if (ret != FYF_OK)
			{
				//FULLTOTVOD_ClearScreen();
				NetErrorCount++;

				if (NetErrorCount == 290)
				{
					OW_SetExitFlag(FYF_TRUE);
					NetErrorCount = 0;
				}
				continue;
			}

			if (bNetError)
			{
				bNetError = FYF_FALSE;
				NetErrorCount = 0;
				if (VodPlayPara.VodOrTvod == 0)
					VOD_ClearScreen();
				else if (VodPlayPara.VodOrTvod == 1)
					FULLTOTVOD_ClearScreen();
			}

			if (optionSocket != -1)
			{
				if (VodPlayPara.VodOrTvod == 1)
				{
					RTSP_NokeyPress();
				}

				RTSP_CheckSerKickOff(optionSocket);
				RTSP_PauseTimeCount(&bPause, &pauseTimeCount);
				
				optionTimeCount++;
				//printf("optionTimeCount = %d\n", optionTimeCount);
				if (optionTimeCount == OPTION_TIME)
				{
					optionTimeCount = 0;
					
					//RTSP_CheckSerKickOff(optionSocket);
					if (RTSP_CreateOPTIONS(optionSocket, &stopTag, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_VOD_MainMenu RTSP_CreateOPTIONS	err!!\n ");
					}
					else
					{
						OW_VOD_SetNetErrFlag(FYF_FALSE);
						//bCableDownCount = 0;
					}
						
					if (stopTag)
					{
						OW_ERROR("OW_VOD_MainMenu RTSP_CreateOPTIONS	err!!out from\n ");
				
						OW_SetExitFlag(FYF_TRUE);
					}

					
				}
			}

		}
		else
		{
			if (GUI_IpanelState == 0)
			{
					if (bHadRes)
					{
						if (!bNetError)
						{
							if (serSetParaType != 0 && RTSP_CreateTEARDOWN(optionSocket, &VodPlayPara) == FYF_ERR)
							{
								OW_ERROR("OW_VOD_MainMenu RTSP_CreateTEARDOWN  err!!\n ");
							}
						}
					}
					bHadRes = FYF_FALSE;				//释放服务器前端分配资源
					if (bTVODPlay)
					{
						APP_Com_API_StopProg(0);
						//OW_DLRTPInfo_StopChannel();
						APP_IPI_StopAudioVideo();
					}
					
					OW_SetExitFlag(FYF_TRUE);
					if (optionSocket != -1)
					{
						close(optionSocket);
						optionSocket = -1;
						optionTimeCount = 0;
					}
					
					NetErrorCount = 0;
					bNetError = FYF_FALSE;

					Ow_Set_entervodortvod_Flag(FYF_FALSE);
					
					OW_SetIpanelSocketExit(FYF_TRUE);

					OW_DEBUG("OW_TVOD_MainMenu End.\n");

					
					bfirst = 1;
					TVOD_ExitInfomation();	
					
					GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
					FYF_API_demux_set_notify(CallBackGetTableFunc);
					GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
					//APP_Com_IPI_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), 1);
					UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
					//APP_Com_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO());
					bGuiTimerSuspend = FYF_FALSE;
					bPauseDisPly = FYF_FALSE;
					bExitSocket = FYF_TRUE;
					printf("GUI_IpanelState == 0, Exit222==============\n");
					
								
					w = 0;
					r = 0;
					num = 0;
					break;
			}
			else 
			{
				if (VodPlayPara.VodOrTvod == 0)
				{
					if (bHadRes)
					{
						if (!bNetError)
						{
							if (RTSP_CreateTEARDOWN(optionSocket, &VodPlayPara) == FYF_ERR)
							{
								OW_ERROR("OW_VOD_MainMenu RTSP_CreateTEARDOWN  err!!\n ");
							}
						}
					}	
						bHadRes = FYF_FALSE;				//释放服务器前端分配资源
						APP_Com_API_StopProg(1);
						//OW_DLRTPInfo_StopChannel();
						OW_SetExitFlag(FYF_TRUE);
						if (optionSocket != -1)
						{
							close(optionSocket);
							optionSocket = -1;
						}
						
						Ow_Set_entervodortvod_Flag(FYF_FALSE);
						
						OW_SetIpanelSocketExit(FYF_TRUE);

						bRecorded = FYF_FALSE;
						recLongTime = -1;
						
						bNetError = FYF_FALSE;
						NetErrorCount = 0;

						OW_SetVODFlag(FYF_FALSE);
						GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
						bfirst = 0;
						bGuiTimerSuspend = FYF_TRUE;
						bExitSocket = FYF_TRUE;		
						if (GUI_ENC_API_GetCurState() != GUI_IpanelState)
							GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
						
						printf("GUI_IpanelState == 1,vod, Exit\n");
					
						w = 0;
						r = 0;
						num = 0;
						break;

				}
				else if (VodPlayPara.VodOrTvod == 1)
				{
					if (bHadRes)
					{
						if (!bNetError)
						{
							if (serSetParaType != 0 && RTSP_CreateTEARDOWN(optionSocket, &VodPlayPara) == FYF_ERR)
							{
								OW_ERROR("OW_VOD_MainMenu RTSP_CreateTEARDOWN  err!!\n ");
							}
						}
					}
						bHadRes = FYF_FALSE;				//释放服务器前端分配资源
						if (bTVODPlay)
						{
							APP_Com_API_StopProg(1);
							printf("stop video =====\n");
							//OW_DLRTPInfo_StopChannel();
						}
					
						OW_SetExitFlag(FYF_TRUE);
						if (optionSocket != -1)
						{
							optionSocket = -1;
							optionTimeCount = 0;
						}
							
						NetErrorCount = 0;
						bNetError = FYF_FALSE;
						Ow_Set_entervodortvod_Flag(FYF_FALSE);
						OW_SetIpanelSocketExit(FYF_TRUE);
						GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
						bfirst = 0;
						bGuiTimerSuspend = FYF_TRUE;
						bExitSocket = FYF_TRUE;
						
						if (GUI_ENC_API_GetCurState() != GUI_IpanelState)
							GUI_ENC_API_SwitchState(GUI_IpanelState, 1);
						
						w = 0;
						r = 0;
						num = 0;
						printf("GUI_IpanelState == 1,Tvod, Exit\n");
						break;
					}


			}

		}
		

	}


	s_tvod_threadId = FYF_NULL;
	
}


int TVODInit(void)
{
	//bGetTsgSuc = FYF_FALSE;

	if (FYF_NULL == s_tvod_threadId)
	{
		s_tvod_threadId = FYF_API_thread_create("TVOD_MainThread",TVOD_MainThread, NULL, 0, 0);  
		if(FYF_NULL == s_tvod_threadId )
		{
			OW_ERROR("TVODInit error\n");
			return FYF_ERR;
		}	
	}
}



/*-----------------------------------------------------------------------------
	函数名：OW_VOD_MainMenu
		
	描述：
			点播函数主函数
	参数：
			in: 
				progAdd: url
	返回：
			无
------------------------------------------------------------------------------*/

int OW_VOD_MainMenu(char *progAdd)
{
	char ipStr[20] 		= {0};
	char portStr[10] 	= {0};
	int  m_s 			= -1;
	char curClock;
	int  ret 			= -1;
    BS32 vKey           = 0xff;
	static int  pts_errcount = 0;
	unsigned int playTime = 0;
	//int  optionTimeCount = 0;
	//BU08 bHadRes = FYF_FALSE;	
	BU08 stopTag = FYF_FALSE;
    //vKey = TRD_IPANEL_MapKey(key);
	FYF_QUEUE_MESSAGE vodmsg;

	printf("in OW_VOD_MainMenu.\n");

	OW_SetVODFlag(FYF_TRUE);

	RTSP_DefaultPlayPara(&VodPlayPara);
	VodPlayPara.VodOrTvod = VOD;
	OW_VOD_SetNetErrFlag(FYF_FALSE);
	OW_SetExitFlag(FYF_FALSE);
	OW_DefaultOptionErrCount();
	OW_SetFirstPauseFlag(FYF_TRUE);
	
	PTSStart = 0;

	OW_DrawConnect();/*hqx:do it later*/

#ifdef USE_IPANEL
	Ow_Set_entervodortvod_Flag(FYF_TRUE);
#endif
	OW_SetIpanelSocketExit(FYF_FALSE);

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
		printf("REGIONID = %x\n", RegionID);
	}
	
	memcpy(VodPlayPara.progAddr, progAdd, strlen(progAdd));

	while (1)
	{		
	 	if((OW_GetExitFlag() == FYF_FALSE) && (RTSP_GetIPStrAndPortStr(VodPlayPara.progAddr, ipStr, portStr) == FYF_ERR))
	 	{
			OW_ERROR("OW_VOD_MainMenu RTSP_GetIPStrAndPortStr Error, out from\n ");
			OW_SetExitFlag(FYF_TRUE);
	 	}
		
		if((OW_GetExitFlag() == FYF_FALSE) && (RTSP_SOCKET_OpenSocket(&m_s, ipStr, portStr) == FYF_ERR))
		{
			OW_ERROR("OW_VOD_MainMenu RTSP_GetIPStrAndPortStr Error\n ");
			OW_DrawSerErr();
			OW_SetExitFlag(FYF_TRUE);
		}

		if((OW_GetExitFlag() == FYF_FALSE) && (RTSP_CreateDescribe(m_s, &VodPlayPara) == FYF_ERR))
		{
			OW_ERROR("OW_VOD_MainMenu RTSP_CreateDescribe Error\n ");
			OW_SetExitFlag(FYF_TRUE);
		}
		if(VodPlayPara.bReLocate)
		{
			VodPlayPara.bReLocate = FYF_FALSE;
			OW_SetExitFlag(FYF_FALSE);
			close(m_s);
			m_s = -1;
			FYF_API_Memset(ipStr,0,20);
			FYF_API_Memset(portStr,0,10);
			printf("OW_VOD_MainMenu Relocation.\n ");
			continue;
		}
		else
		{
			break;
		}
	}

	RTSP_ChecProgAddr(VodPlayPara.progAddr);
	
	if((OW_GetExitFlag() == FYF_FALSE) && (RTSP_CreateSETUP(m_s, &VodPlayPara) == FYF_ERR))
	{
		OW_ERROR("OW_VOD_MainMenu RTSP_CreateSETUP VIDEO Error\n ");
		OW_SetExitFlag(FYF_TRUE);
	}

	if(OW_GetRecordFlag())		/*录播的话要直接跳到时间点*/
	{
		printf("直接跳转!\n");
		VodPlayPara.pCurTime 	= recStarTime;
		VodPlayPara.pStartTime	= recStarTime;
		VodPlayPara.reqScale 	= 1;
		
		printf("pStartTime = %d, endtime = %d\n", VodPlayPara.pStartTime, VodPlayPara.pEndTime);
		if((OW_GetExitFlag() == FYF_FALSE) && (RTSP_CreatePLAY(m_s, &VodPlayPara) == FYF_ERR))
		{
			OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  ERROR\n ");
		}
	}
	else
	{
	
		VodPlayPara.pCurTime = 0;
		VodPlayPara.reqScale = 1;
		printf("pStartTime = %d, endtime = %d\n", VodPlayPara.pStartTime, VodPlayPara.pEndTime);
		if((OW_GetExitFlag() == FYF_FALSE) && (RTSP_CreatePLAY(m_s, &VodPlayPara) == FYF_ERR))
		{
			OW_ERROR("OW_VOD_MainMenu RTSP_CreatePLAY  Error\n ");
			OW_SetExitFlag(FYF_TRUE);
		}
		
	}
		
	optionSocket = m_s;
	if (OW_GetExitFlag() == FYF_FALSE)
	{
		bHadRes = FYF_TRUE;			//取得服务器前端分配资源
		FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
		FYF_API_OSD_Refresh(0, 0, 720, 576);
		
		printf("freq = %d, pid = %d\n", VodPlayPara.pFre, VodPlayPara.pPmtPid);

		if (OW_GetMPEGType() == FYF_TRUE)
		{
			if(OW_DLVodPidInfo(VodPlayPara.pFre, VodPlayPara.pPmtPid, &(VodPlayPara.pVPid), &(VodPlayPara.pAPid),&(VodPlayPara.PcrPid), &(VodPlayPara.mAudioType), &(VodPlayPara.mVideoType)) != FYF_OK)
			{
				OW_ERROR("OW_VOD_MainMenu OW_DLVodPidInfo  Error\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
			else    
			{
				int TryCount = 0;
					
				OW_ClearConnect();

				ipanel_porting_video_location(-35, -25, 720, 576);
				
					if ( 0 == VodPlayPara.pVPid)
						VodPlayPara.pVPid = 0x1fff;
					if ( 0 == VodPlayPara.pAPid)
						VodPlayPara.pAPid = 0x1fff;
					
					ret = APP_IPI_PlayProg(VodPlayPara.pVPid, VodPlayPara.pAPid, VodPlayPara.PcrPid, VodPlayPara.mVideoType, VodPlayPara.mAudioType);
					
					if(FYF_OK != ret)
					{
						OW_ERROR("APP_IPI_PlayProg failed! 0x%x\n", ret);
						OW_SetExitFlag(FYF_TRUE);
					}
					else
					{		
						for (TryCount = 0; TryCount < 4; TryCount++)	
						{
							FYF_API_thread_sleep(500);
							printf("PTSSTART = %d\n",PTSStart);
							PTSStart = OW_GetPts();
							
							if (PTSStart != 0)
							{
								printf("PTSSTART = %d\n",PTSStart);
								break;
							}
						}
						optionSocket = m_s;
					}
				
			}

		}
		else
		{				
			OW_ClearConnect();

			ipanel_porting_video_location(-35, -25, 720, 576);
			
		    //OW_DLRTPInfo(VodPlayPara.pFre, VodPlayPara.pPmtPid);

			optionSocket = m_s;

		}

	}

	VodState = GUI_ENC_API_GetCurState();

	printf("VodState = %d\n", VodState);

	while (OW_GetIpanelSocketExit() == FYF_FALSE)
	{
		FYF_API_thread_sleep(10);
		
		if (OW_GetExitFlag() == FYF_TRUE)
		{
			break;
		}

		if ((VodPlayPara.VodOrTvod == 0) && (GUI_ENC_API_GetCurState() == ID_DLG_VOD) && (VOD_GetCurState() != VOD_PAUSE_STATE /*6*/))
		{
			if (bRecorded && recLongTime != -1)
			{
				if (OW_GetMPEGType() == FYF_TRUE)
				{
					playTime = OW_GetPts() - PTSStart;
					if (playTime > recLongTime)
					{
						pts_errcount++;
						if (pts_errcount == 3)
						{
							pts_errcount = 0;
							FYF_API_thread_sleep(1000);
					
							VOD_ClearScreen();
						
							GUI_ENC_API_SetVisible(ID_STA_THANKS_BAK, WS_VISIBLE);
							GUI_ENC_API_SetVisible(ID_STA_THANKS, WS_VISIBLE);
							GUI_ENC_API_Update(ID_STA_THANKS_BAK, 1);
							GUI_ENC_API_Update(ID_STA_THANKS, 1);
					
							FYF_API_thread_sleep(1000);
							
							VOD_ClearScreen();
							
							OW_SetExitFlag(FYF_TRUE);	
							bPauseDisPly = FYF_FALSE;
							
							OW_ERROR("TP_OW_VOD_MainMenu out  news play end!!playTime = %d,recLongTime = %d out from\n ",
								playTime, recLongTime);
					
						}
					}
					else
					{
						pts_errcount = 0;
					}

				}
				else
				{

				}
			
			}
		}
		
	}
	
	OW_DEBUG("OW_VOD_MainMenu End.\n");

	return FYF_OK;
}

/*********
**enterType:
 **                   0,确认键进入
**		       1,选时键进入
**			2,浏览器进入
**			3,快退键进入
**			4,快进键进入
**********/

int OW_TVOD_MainMenu(char *progAdd, BU08 enterType, char *startClock)	//0表示快捷键进入,1表示从浏览器进入
{

	BS32 RegionIdCache = 0xff;
	char ipStr[20] = { 0 };
	char portStr[10] = { 0 };
	int m_s = -1;
	char clockPause[20] = { 0 };
	//BU08 bHadRes = FYF_FALSE;
	int ret = -1;
	BU08 stopTag = FYF_FALSE;



	printf("in OW_TVOD_MainMenu \n");	
	OW_VOD_SetNetErrFlag(FYF_FALSE);
	OW_SetIpanelSocketExit(FYF_FALSE);
	OW_SetExitFlag(FYF_FALSE);
	if (enterType != 1)
	{
		OW_SetExitFlag(FYF_FALSE);
		VodPlayPara.VodOrTvod = TVOD;
		serSetParaType = 0xff;
		OW_VOD_SetNetErrFlag(FYF_FALSE);
		//enterTVODType = enterType; /*hqx: do it later*/
#ifdef USE_IPANEL
		Ow_Set_entervodortvod_Flag(FYF_TRUE);
#endif
		bTVODPlay = FYF_FALSE;
		OW_SetFirstPauseFlag(FYF_TRUE);
		noKeytimeCount=0;
	
		//bGetTsgSuc = FYF_FALSE;/*hqx*/

		
		if(RTSP_CheckNetOk() != FYF_OK)
		{
			OW_SetExitFlag(FYF_TRUE);
		}

		if (OW_GetExitFlag() == FYF_FALSE)
		{
		    //TPSYSTEM_GetSystemVolLevel();/*hqx: do it later*/
			//RTSP_RetryTsgService();
			RTSP_DefaultPlayPara(&VodPlayPara);
			if (enterType != 2)				//只要不是从浏览器进入时移,则应该检测到cur_tvod_num值
			{
				memcpy(VodPlayPara.progAddr, TvodChanInfo.TsgChannel[curChannelNum].ProgURL,
					strlen(TvodChanInfo.TsgChannel[curChannelNum].ProgURL));
			}
			else							//从浏览器进入时移,此时没经过频道检测,cur_tvod_num一定为负值
			{
				memcpy(VodPlayPara.progAddr, progAdd, strlen(progAdd));
			}

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
					if(enterType != 2)
					{
						//TPCHANNEL_TuneToChannel(iChannelCurListingNo , true, false ); 
					}
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
						//OW_DrawInfo("服务器忙,请稍后再试",FYF_TRUE, FYF_FALSE, FYF_FALSE);
						OW_DrawSerErr();
						OW_SetExitFlag(FYF_TRUE);
						OW_SetIpanelSocketExit(FYF_TRUE);
						if(enterType != 2 && RegionIdCache == 0xffff)
						{
							//TPCHANNEL_TuneToChannel(iChannelCurListingNo , true, false ); 
						}

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

		if (enterType == 0 || enterType == 3 || enterType == 4) //快捷键进入,为防止误按有5秒等待
		{
			if ((OW_GetExitFlag() == FYF_FALSE) && OW_DrawBPZhun() == FYF_ERR)
			{
				OW_ERROR("OW_VOD_MainMenu out from TP_OW_DrawBPZhun err!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
		}
	
		RTSP_ChecProgAddr(VodPlayPara.progAddr);
	
		if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateSETUP(m_s, &VodPlayPara) == FYF_ERR)
		{
			OW_ERROR("OW_VOD_MainMenu out from RTSP_CreateSETUP VIDEO err!!\n ");
			OW_SetExitFlag(FYF_TRUE);
		}

	}
	
	if (OW_GetExitFlag() == FYF_FALSE) 
	{
		if (enterType == 0 || enterType == 3 || enterType == 4)	//快捷键进入,为防止误按有5秒等待
		{
			//APP_Com_API_StopProg(0);
			//FYF_API_vdec_ioctl(NULL, FYF_VDEC_STOP, NULL);
			APP_IPI_StopAudioVideo(1);
			//BU32 arg = 1;
			//FYF_API_adec_ioctl(NULL, FYF_ADEC_STOP, &arg);
			//FYF_API_vdec_ioctl(NULL, FYF_VDEC_STOP, &arg);
		    printf("stop video ==============================\n");
			VodPlayPara.reqScale = 1;
			VodPlayPara.bEOS = FYF_TRUE;
			if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreatePLAY(m_s, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("OW_VOD_MainMenu out from TP_CreatePLAY11  err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
			if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(m_s, &VodPlayPara) == FYF_ERR)	//获取时移时间范围
			{
				OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER  err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
			else
			{
				sprintf(clockPause,"%s",VodPlayPara.clockCur);
			}

		}
		else if (enterType == 1)		//选时键进入
		{	
			m_s = optionSocket;
			if (OW_DrawBPJump(m_s, 0) == FYF_ERR)
			{
				OW_ERROR("OW_VOD_MainMenu out from TP_OW_DrawBPJump  err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
			else
			{
				/*if ((OW_GetExitFlag() == FYF_FALSE) && OW_DrawBPZhun() == FYF_ERR)
				{
					OW_ERROR("OW_VOD_MainMenu out from TP_OW_DrawBPZhun err!\n ");
					OW_SetExitFlag(FYF_TRUE);
				}
				else
				{
					VodPlayPara.reqScale = 1;
					if (!bPlayFromStart && RTSP_CreatePLAY(m_s, &VodPlayPara) == FYF_ERR)
					{
						OW_ERROR("OW_VOD_MainMenu out from RTSP_CreatePLAY  err!!\n ");
					}
					bPlayFromStart = FYF_FALSE;
				}*/
			}
			
			if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreateTGET_PARAMETER(m_s, &VodPlayPara) == FYF_ERR) //获取时移时间范围
			{
				OW_ERROR("OW_DrawBPJump out from RTSP_CreateTGET_PARAMETER  err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
		}
		else if (enterType == 2 && startClock != NULL)	//浏览器进入
		{
			bDrawSpeed = FYF_FALSE;
			VodPlayPara.reqScale = 1;
			sprintf(VodPlayPara.clockCur,"%s",startClock);
			if ((OW_GetExitFlag() == FYF_FALSE) && RTSP_CreatePLAY(m_s, &VodPlayPara) == FYF_ERR)
			{
				OW_ERROR("OW_OW_VOD_MainMenu out from TP_CreatePLAY22  err!!\n ");
				OW_SetExitFlag(FYF_TRUE);
			}
		}

		
		if (OW_GetExitFlag() == FYF_FALSE)
		{
			optionSocket = m_s;
			
			OW_DrawInBP(); /*hqx*/
			bTVODPlay = FYF_TRUE;
			bHadRes = FYF_TRUE; 
		}
		else
		{
			OW_SetIpanelSocketExit(FYF_TRUE);
			return ;
		}
		
	}
	
	while (OW_GetIpanelSocketExit() == FYF_FALSE)
	{	
		if (OW_GetExitFlag() == FYF_TRUE)
		{
			break;
		}
	}

	OW_DEBUG("OW_TVOD_MainMenu End.\n");
}


