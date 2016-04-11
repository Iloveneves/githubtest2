#include "trd_ipanel_init.h"
#include "ipaneldtv_api.h"
#include "ipanel_porting_event.h"
#include "ipanel_demux.h"
#include "msg_event.h"
#include "./../tpmiddle/include/app_basic_type.h"
#include "../tpversion/ui_demo/osd/include/wndId.h"
#include "./../tpmiddle/include/app_sys.h"
//#include "hi_api_eth.h"
#include "hi_stb_api.h"
#include "hi_osdgal.h"
#include "hi_go.h"
#include "../tpmiddle/include/app_com.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DEBUG_VOD            (1)

#define DMUX_MAX_VOD_TS    				5
#define NIT_PID              (0x0010)
#define NIT_TABLE_ID         (0x40)

static BU32 DMUX_ParseNitSection(BU08 *pSectionData, BU08 bParseVodTs);




void *g_hIpanel = NULL;

BU08 g_bRefreshOsd = FYF_FALSE; /* 防止退出时ipanel还要调用刷新导致消息死锁*/
BU32 g_u32SaveAvVol = 0;
 BU32 s_ipanel_threadId = 0;
static BU08 s_bIpanelStart  = FYF_FALSE;
HI_TRD_FREQINFO_S g_stPreFreqInfo; /* 用于保存上个已锁定的频点*/
BU32 s_pIpanelMsgQueue = FYF_NULL;
#if defined(USE_FZVOD) || defined(USE_ENREACH)	
extern BU08 g_firststateInit;
#endif
extern BU08 bfirst;
void HI_TRD_IPANEL_DeInit(void);
void HI_TRD_IPANEL_ExitToReserve(void);
extern BU08 bGuiTimerSuspend;
BU08 g_Destroy = FYF_FALSE;
extern BU08 bIsClockRun;
extern BU32 signal_detect_status;
extern void CallBackGetTableFunc(BU32 channelID, BU32 filterID, BU08* pSection, BS32 len);
static BU08  bIpanel_ip_entrance = FYF_FALSE;
static BU08  bEnterVodOrTvod = 0;
static BS32 VodTsNum = 0;
static DMUX_VodTsParams_t VodTsParams[DMUX_MAX_VOD_TS];
BS32 RegionID = 0xff;
static BS16 DmuxNitVer = -1;
BU08 EnableSecDL = 0;
BU32 GUI_IpanelState = 0;
BU32 GUI_NullState = 0;
BU08 bHaveDL = FYF_FALSE;
int bNetErrs = FYF_FALSE;
static BU08 bOw_entervodortvod = FYF_FALSE;
char getipaddr[20] = {0};
extern BU08 bNetError;
static BU08  dhcp_start = FYF_FALSE;			/*动态ip*/
BU08  bNetErr = FYF_FALSE;				/*网络错误*/
extern BU08 g_fromIpanel;
BU08 g_demux_flag = 1;


BS32 FYF_API_ETH_DNSGet(BU08 *dns)
{
	HI_ETH_DNSGet(0 , dns);
	return 0;
}

BS32 FYF_API_ETH_DNSSet(BU08 *dns)
{
	return HI_ETH_DNSSet(0, FYF_TRUE, dns);
}

BS32 FYF_API_ETH_IPAddressGet(BS08 *ipAdd)
{
    HI_ETH_IPAddressGet(0, ipAdd);
	return 0;
}

BS32 FYF_API_ETH_IPAddressSet(BS08 *ipAdd)
{
	return HI_ETH_IPAddressSet(0,ipAdd);
}

BS32 FYF_API_GetLinkStatus(void)
{
	BS32 ret = -1;
	Eth_LinkStatus_E eLinkStatus; 
	HI_ETH_GetLinkStatus(0, &eLinkStatus); 
	//printf("FYF_API_GetLinkStatus, status = %d\n", eLinkStatus);

	/*switch (eLinkStatus)
	{
		case ETH_LINK_STATUS_OFF:
			break;

		case ETH_LINK_STATUS_ON:
			break;

		case ETH_LINK_STATUS_MAX:
			break;
	}*/

	return eLinkStatus;
}

/*-----------------------------------------------------------------------------
	函数名：OW_VOD_SetDhcpFlag
		
	描述：
			设置动态获取ip的标志
	参数：
			in: 
				flag: 设置动态获取ip的标志
	返回：
			无
------------------------------------------------------------------------------*/

void OW_VOD_SetDhcpFlag(BU08 flag)
{
	dhcp_start = flag;
}

/*-----------------------------------------------------------------------------
	函数名：OW_VOD_GetDhcpFlag
		
	描述：
			获取是否动态获取ip的标志
	参数：
			无
	返回：
			标志位
------------------------------------------------------------------------------*/

BU08 OW_VOD_GetDhcpFlag(void)
{
	//printf("bdhcp_start=%d\n",dhcp_start);
	return dhcp_start;
}


/*-----------------------------------------------------------------------------
	函数名：OW_VOD_SetNetErrFlag
		
	描述：
			设置网络错误的标志
	参数：
			in: 
				flag: 设置网络错误的标志
	返回：
			无
------------------------------------------------------------------------------*/

void OW_VOD_SetNetErrFlag(BU08 flag)
{
	bNetErr = flag;
}

/*-----------------------------------------------------------------------------
	函数名：OW_VOD_GetDhcpFlag
		
	描述：
			获取网络是否出错的标志
	参数：
			无
	返回：
			标志位
------------------------------------------------------------------------------*/

BU08 OW_VOD_GetNetErrFlag(void)
{
	return bNetErr;
}

void ipanel_porting_sleep_ms(int time)
{
	printf("sleep_ms=%d\n",time);
	//MILLI_DELAY(time);
	FYF_API_thread_sleep(time);
}

//当进入互动浏览的时候，禁止ipanel滤表的标志
BU08 Ipanel_GetEntrance_Flag(void)
{
	return bIpanel_ip_entrance;
}
/***********************************************************************************************/
/***********************************************************************************************/
void Ipanel_SetEntrance_Flag(BU08 flag)
{
	bIpanel_ip_entrance = flag;
}

//进入时移或者点播的标志
//1代表浏览器进时移，2代表浏览器就点播，3代表实时时移

BU08 Ipanel_GetEnterVodOrTvod_Flag(void)
{
	return bEnterVodOrTvod;
}

void Ipanel_SetEnterVodOrTvod_Flag(BU08 flag)
{
	bEnterVodOrTvod = flag;
}

BU08 Ow_Get_entervodortvod_Flag(void)
{
	return bOw_entervodortvod;
}

void Ow_Set_entervodortvod_Flag(BU08 flag)
{
	bOw_entervodortvod = flag;
}

int TP_Ipanel_CheckNetOk(void)
{
	int ret = -1;
	
	ret = FYF_API_GetLinkStatus();
	if (ret != 1)
	{
		return -2;
	}
	
	FYF_API_ETH_IPAddressGet(getipaddr);
	if (strlen(getipaddr) == 0 || strcmp(getipaddr, "000.000.000.000") == 0)
	{
		IPANEL_ERROR("FYF_API_ETH_IPAddressGet getip = %s\n",getipaddr);
		return -3;
	}
	IPANEL_ERROR("strlen(getipaddr) = %d, getipaddr = %s\n", strlen(getipaddr),getipaddr);

	return FYF_OK;
}

BU32 Ipanel_GetStatus()
{
	if(s_bIpanelStart)
		return 1;
	else
		return 0;
}

/* ----------------------------------------------------------------------------
   Name: DMUX_ResetVodTsParams()

   Description:
   Default and reset vod transport stream parameters in VodTsParams.

   Parameters:

   Return Value:
   ---------------------------------------------------------------------------- */
static void DMUX_ResetVodTsParams(void)
{
	int i;

	for (i = 0; i < DMUX_MAX_VOD_TS; i++)
	{
		VodTsParams[i].bUsed = FYF_FALSE;
		VodTsParams[i].FreqMhz = 0;
		VodTsParams[i].FEC_inner = 0;
		VodTsParams[i].FEC_outer = 0;
		VodTsParams[i].Modulation = 0;
		VodTsParams[i].RegionID = 0;
		VodTsParams[i].RegionPid = 0;
		VodTsParams[i].Symbol_rate = 0;
	}
	
	VodTsNum = 0;
}

/* ----------------------------------------------------------------------------
   Name: DMUX_SetVodTsParams()

   Description:
   Modify vod transport stream parameters in VodTsParams.

   Parameters:
   VodTsParams:			Input vod transport stream parameters

   Return Value:
   ---------------------------------------------------------------------------- */
static void DMUX_SetVodTsParams(DMUX_VodTsParams_t VodTsInfo)
{
	int i;

	for (i = 0; i < DMUX_MAX_VOD_TS; i++)
	{
		if ((VodTsParams[i].bUsed == FYF_FALSE))
		{
			VodTsParams[i] = VodTsInfo;
			VodTsParams[i].bUsed = FYF_TRUE;
			VodTsNum++;
			break;
		}
	}
}


BS32 HI_TRD_AV_GetFreqByPid(HI_TRD_FREQINFO_S * pstFreqInfo, BU32 video_pid, BU32 audio_pid, BU32 pcr_pid)
{
	BroadCast_ChnInfo_S chinfo;
	BS32 ret = -1;

	IPANEL_DEBUG("HI_TRD_AV_GetFreqByPid video_pid = %d  audio_pid = %d pcr_pid = %d\n",video_pid,audio_pid,pcr_pid);
	ret = DBA_API_AV_GetFreqByAVID(&chinfo, video_pid, audio_pid, pcr_pid);
	if (-1 == ret )
	{
		IPANEL_ERROR("ret = %d\n",ret);
		return FYF_ERR;
	}
	
	IPANEL_DEBUG("freq = %d, sym = %d, qam = %d\n",chinfo.m_FreqMHz, chinfo.m_SymKbps, chinfo.m_QAM);

	pstFreqInfo->frq = (chinfo.m_FreqMHz * 1000);
	pstFreqInfo->sym = (chinfo.m_SymKbps * 1000);
	pstFreqInfo->eqamType = chinfo.m_QAM;
	
	return FYF_OK;
	
}

BS32 HI_TRD_AV_GetPreqByServiceId(HI_TRD_FREQINFO_S *  pstFreqInfo, BU32 network_id, BU32 ts_id,  BU32 service_id)
{
	BroadCast_ChnInfo_S chinfo;
	BS32 ret = -1;
	IPANEL_DEBUG("HI_TRD_AV_GetPreqByServiceId network_id = %x  ts_id = %x service_id = %x\n",network_id,ts_id,service_id);

	ret = DBA_API_AV_GetFreqByServiceId(&chinfo, network_id, ts_id, service_id);
	if (-1 == ret )
	{
		IPANEL_ERROR("ret = %d\n",ret);
		return FYF_ERR;
	}
	IPANEL_DEBUG("freq = %d, sym = %d, qam = %d\n",chinfo.m_FreqMHz, chinfo.m_SymKbps, chinfo.m_QAM);
	pstFreqInfo->frq = (chinfo.m_FreqMHz * 1000);
	pstFreqInfo->sym = (chinfo.m_SymKbps * 1000);
	pstFreqInfo->eqamType = chinfo.m_QAM;

	return FYF_OK;
}

BS32 HI_TRD_AV_GetAVpidByServiceId(BU32 *pu16VidPid, BU32 *pu16AudPid, BU32 * pu16PcrPid,
                    BU32 network_id, BU32 ts_id, BU32 service_id)
{
//	if(pu16VidPid == 0 || pu16AudPid ==0 || pu16PcrPid == 0)
//		return FYF_ERR;

	BroadCast_ChnInfo_S chinfo;
	BS32 ret = -1;
	
	IPANEL_DEBUG("HI_TRD_AV_GetAVpidByServiceId network_id = %x  ts_id = %x service_id = %x\n",network_id,ts_id,service_id);

	
	ret = DBA_API_AV_GetFreqByServiceId(&chinfo, network_id, ts_id, service_id);
	if (-1 == ret )
	{
		IPANEL_ERROR("ret = %d\n",ret);
		return FYF_ERR;
	}

	IPANEL_DEBUG("audio = %d, video = %d, pcr = %d\n", chinfo.m_AudioPid, chinfo.m_VideoPid, chinfo.m_PcrPid);
	
	*pu16VidPid = chinfo.m_VideoPid;
	*pu16AudPid = chinfo.m_AudioPid;
	*pu16PcrPid = chinfo.m_PcrPid;
	return FYF_OK;

}

BS32 HI_TRD_AV_GetServiceId(BU16 *pu32service_id)
{
	if(pu32service_id == 0)
		return FYF_ERR;

	*pu32service_id= 738;
	return FYF_OK;

}
extern BU08 bConnectState;

void TRD_IPANEL_MainThread(void* pvArgv)
{
    HI_U32 event[3];
    HI_U32 u32Ret = FYF_ERR;
    FYF_QUEUE_MESSAGE Msg ;
    
    unsigned long time_count = 0;
    int  bTuner_FirstUnlock = FYF_FALSE;
    int  bTuner_FirstLock = FYF_TRUE;

	//2009-11-24 cwd modify
	//去掉g_bExecute_flag变量，该线程一被创建就不被销毁
	
    IPANEL_DEBUG("\n\n");

    while(1)
    {
//    	printf("TRD_IPANEL_MainThread!!\n");
    	if (g_Destroy)
		{
			if (g_Destroy == 1)
				HI_TRD_IPANEL_DeInit();
			else if (g_Destroy == 2)
				HI_TRD_IPANEL_ExitToReserve();
			time_count = 0;
	    }
		
		if ((0 != g_hIpanel) && (s_pIpanelMsgQueue != 0))
    	{		

			if (Ow_Get_entervodortvod_Flag() == FYF_FALSE)
			{
				time_count++;
				TRD_Check_Tuner(&time_count, &bTuner_FirstLock, &bTuner_FirstUnlock, &bNetErrs);
			}
			
	    		u32Ret = FYF_API_queue_recv(s_pIpanelMsgQueue, &Msg, 10);
			
	        	if(FYF_OK != u32Ret)
	        	{
	            		event[0] = EIS_EVENT_TYPE_TIMER;
	            		event[1] = 0;
	            		event[2] = 0;
	        	}
	        	else
	        	{
	            		event[0] = Msg.qlstWordOfMsg;
	            		event[1] = Msg.q2ndWordOfMsg;
	            		event[2] = Msg.q3rdWordOfMsg;            
	        			printf("receive a key %d \n",event[0]);
	        	}

				if ((event[0] == EIS_EVENT_TYPE_KEYDOWN) && (event[1] == EIS_IRKEY_VOLUME_MUTE))
				{
					if(ipanel_GetAudMuteState())
					{
						ipanel_SetAudMuteState(FYF_FALSE);
					}
					else
					{
						ipanel_SetAudMuteState(FYF_TRUE);
					}
				}
				else if((event[0] == EIS_EVENT_TYPE_KEYDOWN) && (event[1] == EIS_IRKEY_VOLUME_DOWN || event[1] == EIS_IRKEY_VOLUME_UP))
				{
					ipanel_SetAudMuteState(FYF_FALSE);
				}
				ipaneldtv_proc(g_hIpanel, event[0], event[1], event[2]);
//				printf("TRD_IPANEL_MainThread!!22\n");
    		}
        	else
        	{
            		//IPANEL_ERROR("g_hIpanel is null \n");            
        	}        
        	//usleep(1000);    
        	FYF_API_thread_sleep(10);
    }
    /* 确保从ipanel内退出才掉destroy否则可能导致死机*/
    /* 并且确保从ipanel退出再释放osd的内存*/
    IPANEL_DEBUG("ipaneldtv_destroy.");

    /*if (0 != g_hIpanel)
    {
        ipaneldtv_destroy(g_hIpanel);
        g_hIpanel = NULL;    
    }*/
	
    return;
}

void TRD_IPANEL_Input(BU32 key)
{
    /* 调用统一接口的按键映射*/
    FYF_QUEUE_MESSAGE stMsg, stVodMsg;    
    BU32 event[3];
    BS32 vKey;

	if (key == GUI_VK_F8)
	{
		event[0] = EIS_EVENT_TYPE_KEYDOWN;    
    	event[1] = (BU32)(EIS_IRKEY_EXIT);       
   		event[2] = 0;

		stMsg.qlstWordOfMsg     = (BU32)event[0];
	    stMsg.q2ndWordOfMsg     = (BU32)event[1];
	    stMsg.q3rdWordOfMsg     = (BU32)event[2];   
		stMsg.q4thWordOfMsg     = 0;
	    if (0 != s_pIpanelMsgQueue)
			FYF_API_queue_send(s_pIpanelMsgQueue, &stMsg);

		FYF_API_thread_sleep(300);
		event[0] = EIS_EVENT_TYPE_KEYDOWN;    
    	event[1] = (BU32)(EIS_IRKEY_SELECT);       
   		event[2] = 0;

		stMsg.qlstWordOfMsg     = (BU32)event[0];
	    stMsg.q2ndWordOfMsg     = (BU32)event[1];
	    stMsg.q3rdWordOfMsg     = (BU32)event[2];   
		stMsg.q4thWordOfMsg     = 0;
	    if (0 != s_pIpanelMsgQueue)
			FYF_API_queue_send(s_pIpanelMsgQueue, &stMsg);
		return;
	}
	
	
    vKey = TRD_IPANEL_MapKey(key);
    if(vKey == FYF_ERR)
    	return;
    
    event[0] = EIS_EVENT_TYPE_IRKEY;    
    event[1] = (BU32)vKey;       
    event[2] = 0;
    stMsg.qlstWordOfMsg     = (BU32)event[0];
    stMsg.q2ndWordOfMsg     = (BU32)event[1];
    stMsg.q3rdWordOfMsg     = (BU32)event[2];   
	stMsg.q4thWordOfMsg     = 0;
    if (0 != s_pIpanelMsgQueue)
	{
		FYF_API_queue_send(s_pIpanelMsgQueue, &stMsg);
		printf("send event = %d\n", event[1]);
    }
    	
/*
   event[0] = EIS_EVENT_TYPE_KEYUP;    
    event[1] = (BU32)vKey;       
    event[2] = 0;
    stMsg.qlstWordOfMsg     = (BU32)event[0];
    stMsg.q2ndWordOfMsg     = (BU32)event[1];
    stMsg.q3rdWordOfMsg     = (BU32)event[2];   
	stMsg.q4thWordOfMsg     = 0;
    if (0 != s_pIpanelMsgQueue)
	{
		FYF_API_queue_send(s_pIpanelMsgQueue, &stMsg);
		printf("send event22 = %d\n", event[1]);
    }
    */
	
}

BU32   BroadcastFreq=0;
extern BU32   IpanelServiceid;
BU32 TRD_GetBroadcastFreq(void)
{
	printf("TRD_GetBroadcastFreq: %d\n",BroadcastFreq);
	return BroadcastFreq;
}

void TRD_SetBroadcastFreq(BU32 freq)
{
	BroadcastFreq = freq;
}

void TRD_SetServiceId(BU32 id)
{
	IpanelServiceid = id;
	printf("============IpanelServiceid: %d\n",IpanelServiceid);
}

static BU08 bInThird = FYF_FALSE;
void Inthird_SetFlag(BU08 flag)
{
	bInThird = flag;
}

BU08 Inthird_GetFlag(void)
{
	return bInThird;
}

/******************************************************************************
 * Function:          HI_TRD_OSD_IPANEL_Init
 * Description:     ipanel init
 * Data Accessed: 
 * Data Updated:  
 * Input:            HI_HANDLE hSurface  为stb传入的surface 
                        trdtype 为初始化哪个第三方(ipanel,htld,enreach等)
 * Output:            
 * Return:          
 * Others:            
******************************************************************************/

BS32 HI_TRD_IPANEL_Init(BU32 freq,BU32 sym,BU32 qam, BU32 serid)
{
	BS32 ret = -1;
	FYF_QUEUE_MESSAGE stMsg = {0};    
	int j = 0;
	BU32 event[3];
	HI_U32 u32Ret = FYF_ERR;
	FYF_QUEUE_MESSAGE Msg, msgRecv ;
	BU32 value = 0;

	
    if (FYF_TRUE == s_bIpanelStart)
    {
        IPANEL_ERROR("ipanel is not deinit and not init !\n");        
        return FYF_OK;
    }
	
	s_bIpanelStart = FYF_TRUE;
	//2009-11-24 cwd addd
	//等待停止音视频和黑屏后才进行下一步操作
	APP_Com_API_StopProg(1);
	Msg.qlstWordOfMsg= (unsigned int)APP_Queue_Ipannel_Start;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &Msg);
	/* 统一接口好像不能清除视频帧,所以用底层驱动*/ 

	BroadCast_AppInit();
	APP_Com_API_SetEcmNULL();
	msgRecv.qlstWordOfMsg = 0;
	FYF_API_queue_recv(APP_IPI_GetQueueIpanel(), &msgRecv, 2000);
	if (msgRecv.qlstWordOfMsg == 1)
	{
		printf("APP_IPI_GetQueueIpanel success\n!");
	}
	else
	{
		printf("APP_IPI_GetQueueIpanel error!\n");
	}
	
#if defined(USE_FZVOD) || defined(USE_ENREACH)	
	if (g_firststateInit == FYF_FALSE)
	{
		GUI_NullState = GUI_ENC_API_GetPrevState();
	}
#endif

#ifdef USE_FZVOD
	OW_SetExitFlag(FYF_FALSE);
	//APP_Com_API_StopProg(0);
#endif

	FYF_API_demux_set_notify(NULL);
	bGuiTimerSuspend = FYF_TRUE;
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
	FYF_API_OSD_FillRect(0, 0, 720, 576, 0x8000);
	FYF_API_OSD_Refresh(0, 0, 720, 576);
	TRD_SetBroadcastFreq(0);
	s_pIpanelMsgQueue = FYF_NULL;


    IPANEL_DEBUG("HI_TRD_IPANEL_Init !\n");


	if (Ipanel_GetEntrance_Flag() == FYF_FALSE)
	{
		/*if (freq == 786000)
		{
			g_demux_flag = 0;
		}
		else
		{
			g_demux_flag = 1;
		}
		*/
		ret = TRD_LockTuner( freq,  sym,  qam);
		if (ret == FYF_ERR)
		{
			IPANEL_ERROR("TRD_LockTuner ERROR.\n");
			//s_bIpanelStart = FYF_FALSE;
			//return ret;
		}
		g_stPreFreqInfo.frq = freq;
		g_stPreFreqInfo.sym = sym;
		g_stPreFreqInfo.eqamType = qam;
		TRD_SetBroadcastFreq(freq);
		APP_IPI_SaveTunerInfo(freq, sym, qam);
	   	TRD_SetServiceId(serid);
		Ipanel_SetEnterVodOrTvod_Flag(0);
	}
#if defined(USE_FZVOD) || defined(USE_ENREACH)
	else if (Ipanel_GetEntrance_Flag())
	{
		printf("双向=====================\n");

		ret = TP_Ipanel_CheckNetOk();
		if (FYF_OK != ret)
		{
			if (ret == -2)
			{
		//		bNetError = FYF_TRUE;
			}

			return ret;
		}

		//bNetError = FYF_FALSE;
	}
#endif	
	


    g_bRefreshOsd = FYF_TRUE;   
	g_Destroy = FYF_FALSE;
	

	s_pIpanelMsgQueue =  FYF_API_queue_create("IpanelMsgQueue",100, 0);
	if (0 == s_pIpanelMsgQueue)
	{
		IPANEL_ERROR(" ipanel msg create failed!\n");
		return FYF_ERR;
	}

    /*创建消息接收任务*/
	if (FYF_NULL == s_ipanel_threadId)
    {
		s_ipanel_threadId = FYF_API_thread_create("TRD_IPANEL_MainThread",TRD_IPANEL_MainThread, NULL, 0, 0);    
	    if(FYF_NULL == s_ipanel_threadId )
	    {
	        IPANEL_ERROR("HI_PthreadCreate error:%x\n",ret);
			if (0 != s_pIpanelMsgQueue)
			{		
				FYF_API_queue_destroy(s_pIpanelMsgQueue);
				
		    }   
			s_pIpanelMsgQueue = 0;
			return FYF_ERR;
	    }
		
		
	}

    IPANEL_DEBUG("ipaneldtv_create  before !\n");

	ipanel_media_install_protocol("rtsp://");
	g_hIpanel = ipaneldtv_create(0xc00000);
	if(NULL == g_hIpanel)
	{
    	IPANEL_ERROR("ipaneldtv_create failed!\n");
		s_bIpanelStart = FYF_FALSE;
		if (FYF_NULL != s_pIpanelMsgQueue)
		{
			FYF_API_queue_destroy(s_pIpanelMsgQueue);
		}
		s_pIpanelMsgQueue = 0;

        return FYF_ERR;
    }


	IPANEL_DEBUG("ipaneldtv_create !  after\n");

	GUI_IpanelState = GUI_ENC_API_GetCurState();
	IPANEL_DEBUG("ipanel init finish!\n");
    return FYF_OK;    
}


/******************************************************************************
 * Function:          HI_TRD_OSD_IPANEL_DeInit
 * Description:     IPANEL deinit
 * Data Accessed: 
 * Data Updated:  
 * Input:            
 * Output:            
 * Return:          
 * Others:            
******************************************************************************/

void HI_TRD_IPANEL_Exit()
{
	
	g_bRefreshOsd = FYF_FALSE; 
	g_Destroy = 2;
}

void HI_TRD_IPANEL_ExitToReserve()
{
	 BS32 s32Ret = -1;

	//FYF_API_thread_sleep(50);
	g_bRefreshOsd = FYF_FALSE; 
	g_Destroy = FYF_FALSE;
	
	
    IPANEL_DEBUG("HI_TRD_IPANEL_DeInit !\n");

    if (FYF_FALSE == s_bIpanelStart)
    {
        IPANEL_ERROR("ipanel is  not init !\n");        
        return ;
    }
	else
	{
		s_bIpanelStart = FYF_FALSE;
	}

	APP_IPI_StopAudioVideo(1);

	 if (FYF_NULL != s_pIpanelMsgQueue)
	{
		s32Ret = FYF_API_queue_destroy(s_pIpanelMsgQueue);
		if(FYF_OK != s32Ret)
		{
			IPANEL_ERROR("Destroy s_pIpanelMsgQueue fail, ret = %x\n", s32Ret);
		}
		IPANEL_DEBUG("Destroy s_pIpanelMsgQueue success");
	    s_pIpanelMsgQueue = 0;

	}

  
     if (FYF_NULL !=  g_hIpanel)
     {
        	ipaneldtv_destroy(g_hIpanel);
        	g_hIpanel = FYF_NULL;
	 }

	Ipanel_SetEntrance_Flag(FYF_FALSE);


	bfirst = 1;
	
	FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
	FYF_API_OSD_Refresh(0, 0, 720, 576);
	
   	bGuiTimerSuspend = FYF_FALSE;

	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
	

	FYF_API_demux_set_notify(CallBackGetTableFunc);
	APP_Com_API_Start_Signal_Dectect();

	APP_Search_Set_OTA_Status(1);

    IPANEL_DEBUG("ipanel deinit finish!\n");    
	
}

void HI_TRD_IPANEL_DeInit(void)
{

    BS32 s32Ret = -1;
	g_bRefreshOsd = FYF_FALSE; 
	g_Destroy = FYF_FALSE;
	
	
    //IPANEL_DEBUG("HI_TRD_IPANEL_DeInit !\n");

    if (FYF_FALSE == s_bIpanelStart)
    {
        IPANEL_ERROR("ipanel is  not init !\n");        
        return ;
    }
	else
	{
		s_bIpanelStart = FYF_FALSE;
	}

	APP_IPI_StopAudioVideo(1);
	        
    if (FYF_NULL != s_pIpanelMsgQueue)
	{
		s32Ret = FYF_API_queue_destroy(s_pIpanelMsgQueue);
		if(FYF_OK != s32Ret)
		{
			IPANEL_ERROR("Destroy s_pIpanelMsgQueue fail, ret = %x\n", s32Ret);
		}
		//IPANEL_DEBUG("Destroy s_pIpanelMsgQueue success");
	    s_pIpanelMsgQueue = 0;

	}

  
	if (FYF_NULL !=  g_hIpanel)
	{
		ipaneldtv_destroy(g_hIpanel);
		g_hIpanel = FYF_NULL;
	}


	BroadCast_AppExit();

	OW_VOD_SetDhcpFlag(FYF_FALSE);
	Ipanel_SetEnterVodOrTvod_Flag(0);
	OW_VOD_SetNetErrFlag(FYF_FALSE);
	g_fromIpanel  = FYF_FALSE;
	
#ifdef USE_FZVOD
	OW_SetExitFlag(FYF_FALSE);
#endif

	bfirst = 1;
	FYF_API_OSD_FillRect(0, 0, 720, 576, 0);
	FYF_API_OSD_Refresh(0, 0, 720, 576);
   	bGuiTimerSuspend = FYF_FALSE;
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
	FYF_API_demux_set_notify(CallBackGetTableFunc);

	
	//if (Ipanel_GetEntrance_Flag() == FYF_TRUE)

	Ipanel_SetEntrance_Flag(FYF_FALSE);

	

	FYF_API_thread_sleep(1000);
#if defined(USE_FZVOD) || defined(USE_ENREACH)	
	if (g_firststateInit)
	{
		g_firststateInit = FYF_FALSE;
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		if (GUI_IpanelState != 0)
			GUI_IpanelState = 0;

		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//APP_Com_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO());
	}
	else
#endif		
	{
		if (GUI_NullState != ID_DLG_MainMenu && GUI_NullState != ID_DLG_ChnInfo && GUI_NullState != ID_DLG_BroadcastList)
			GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
		else
			GUI_ENC_API_SwitchState(/*GUI_ENC_API_GetPrevState()*/GUI_NullState,1);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		if (GUI_IpanelState != 0)
			GUI_IpanelState = 0;
	}

    printf("ipanel deinit finish!\n");    
}

void NewFilter(BU32 ch,BU32 *filter,BU08 coef[], BU08 mask[], BU08 excl[])
{
	BU32 filterid;
	BS32 ret = -1;

	FYF_API_demux_create_filter(ch, &filterid);
	ret = FYF_API_demux_set_filter(ch, 
									filterid,  
									1,
									coef,  
									mask, 
									excl);
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("FYF_API_demux_set_filter ERROR\n");
		return;
	}

	ret = FYF_API_demux_enable_filter(ch, filterid);
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("ipanel_porting_enable_demux_filter ERROR\n");
		return ;
	}

	*filter = filterid;
}



void DeleteFilter(BU32 ch,BU32 filter)
{
	BU32 filterid;
	BS32 ret = FYF_ERR;

	ret = FYF_API_demux_disable_filter(ch, filter);
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("FYF_API_demux_disable_filter ERROR\n");
		return;
	}

	ret = FYF_API_demux_destroy_filter(ch, filter);
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("ipanel_porting_enable_demux_filter ERROR\n");
		return ;
	}

}


void Ipanel_DLRegionId_Callback(BU32 ch,BU32 filter,BU32 DataStartAddr,BU32 len)
{
	BU08 *buff = (BU08 *)DataStartAddr;

	if (0 == VodTsNum)
	{
		DMUX_ParseNitSection(buff, FYF_FALSE);
	}
	else
	{	
		EnableSecDL = 1;
		DMUX_ParseNitSection(buff, FYF_TRUE);
	}
}



//char *tmpurl = "rtsp://125.210.242.11:2554/hntv_fz?token=666B085513E6C48F1F5411543F9B9D64DC93A6764028D7FB9C3C960E027440E889EA31EE303D17EFCB14D89818D99506823510373B828E1DAAC1F0098A5019693F7E38C0D8430747796728EEBB4BC256D7C9E3B1BA2C4C7A2249CB897EC6B12FACE8124DE21A5FE4966567CBF6FDE60C8EAA883F89037B39D6F6D85AE469C21FC8AEA43403D9FE7C&start-time=20090717190000&stop-time=20090717190020";
//char *tmpurl2 = "rtsp://218.108.226.132:2554/fzsan05/tv/4/d09070200020010001_z.ts?token=40A795806E749E51F7317BC1A650D5284945433F8FF49843845C0EBCE1830B4CF703F89DD301275A79D5E7AC28BF70C0346C9FB2E257EACA925BB7DDCE93C817777DBFEAF7C7C712D719BFE083DD3828E38F76&beginTime=0";
BS32 Ipanel_DLRegionId(void)
{
	BU32 channel_id;
	BU32 filter[10];
	BS32 ret = -1;
	int i = 0;
	BU08 coef[4] = {NIT_TABLE_ID, 0, 0, 0};
	BU08 mask[4] = {0xff, 0, 0, 0}; 
	BU08 excl[4] = {0};
	int VodFreqId = 0;
	static int count = 0;
	int DLNitCounter = 0;
	int DLNitCounterMax = 120;
	int socketid = 0;
	char XmlData[1024] = {0};
	char stbid[27] = {"21040010102052544c1e643a11"};
	//APP_EPG_API_StopGetEpg();
	BU08 g_dns[20] = {0};
	BU08 u8ipAdd[20] = {0};
	BU32 ip = 0;
	BU32 FreqValue;

	IPANEL_DEBUG("Ipanel_DLRegionId. time = %d\n",ipanel_porting_time_ms());

	//RTSP_CreateChannelXMLDATA(XmlData, stbid);
	//RTSP_SOCKET_OpenSocket(&socketid, "192.168.1.21", "80");

	//HI_ETH_IPAddressGet(0,u8ipAdd);
	//printf("u8ipAdd = %s\n", u8ipAdd);
	//HI_ETH_DNSSet(0,1,"218.85.157.99");
	//HI_ETH_DNSGet(0, g_dns[0]);
	//FYF_API_ETH_DNSGet(u8ipAdd);
	//printf("dns ip = %s\n", u8ipAdd);

	//RTSP_ParseIpString2Value(u8ipAdd, &ip);
	//printf("ip11111=%x\n", ip);
	//system("udhcpc");
	printf("===================Ipanel_DLRegionId\n");
	if (bHaveDL)
	{
		printf("===================bHaveDL = FYF_TRUE\n");
		return;
	}

	APP_SYS_API_GetSysInfo(APP_SYS_MAIN_FREQ_e, &FreqValue);
	printf("FreqValue = %d\n", FreqValue);
	ret = TRD_LockTuner( FreqValue*1000,  6875,  2);
	if (ret != FYF_OK)
	{
		IPANEL_ERROR("TRD_LockTuner ERROR.\n");
		return ret;
	}

	FYF_API_demux_set_notify(NULL);
	FYF_API_demux_set_notify(Ipanel_DLRegionId_Callback);

	FYF_API_demux_create_channel(0, FYF_DEMUX_DATA_PSI_CHANNEL, &channel_id, NIT_PID);
	FYF_API_demux_create_filter(channel_id, &filter[0]);
	ret = FYF_API_demux_set_filter(channel_id, 
									filter[0],  
									4,
									coef,  
									mask, 
									excl);
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("FYF_API_demux_set_filter ERROR\n");
		return;
	}

	ret = FYF_API_demux_enable_filter(channel_id, filter[0]);
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("ipanel_porting_enable_demux_filter ERROR\n");
		return ;
	}
	
	ret = FYF_API_demux_start_channel(channel_id);
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("FYF_CPI_demux_start_channel ERROR\n");
		return ;
	}
	
	while (1)
	{
		if (VodTsNum > 0)
		{
			for (VodFreqId = 0; VodFreqId < VodTsNum; VodFreqId++)
			{
				DLNitCounter = 0;

				FYF_API_demux_stop_channel(channel_id);
				DeleteFilter(channel_id, filter[0]);
				
				printf("freq = %d\n", VodTsParams[VodFreqId].FreqMhz);
				ret = TRD_LockTuner( VodTsParams[VodFreqId].FreqMhz * 1000,  6875,  2);
				if (ret == FYF_ERR)
				{
					IPANEL_ERROR("TRD_LockTuner ERROR.\n");
					return ret;
				}

				FYF_API_demux_set_channel_pid(channel_id, VodTsParams[VodFreqId].RegionPid);
				coef[0] = NIT_TABLE_ID;
				mask[0] = 0xff;
				NewFilter(channel_id, &filter[0], coef,  mask,  excl);
				ret = FYF_API_demux_start_channel(channel_id);
				if (FYF_ERR == ret)
				{
					IPANEL_ERROR("FYF_CPI_demux_start_channel ERROR\n");
					return ;
				}	

				while (DLNitCounter < DLNitCounterMax)
				{
					if (EnableSecDL == FYF_FALSE)
					{
						FYF_API_thread_sleep(100);
						DLNitCounter++;
					}
					else
					{
						IPANEL_DEBUG("Get the nit data\n");
						break;
					}
				}
				
				if (DLNitCounter >= DLNitCounterMax)
				{
					IPANEL_ERROR("Can't Get the nit data\n");
				}
				else
				{
					printf("region id RegionPid = %d\n", VodTsParams[VodFreqId].RegionPid);
					break;
				}
			}

			break;

		}
		else
		{
			if (count >= 30)
			{
				IPANEL_ERROR("Can't Get RegionID\n");
				break;
			}
			FYF_API_thread_sleep(100);
			count++;
		}
	}


	FYF_API_demux_stop_channel(channel_id);
	DeleteFilter(channel_id, filter[0]);
	FYF_API_demux_set_notify(CallBackGetTableFunc);

	if (RegionID == 0xff)
	{
		return FYF_ERR;
	}
	else
	{
		printf("\033[0;31mDLRegionId ok, RegionID  = %x, time = %d\033[0m\n", RegionID, ipanel_porting_time_ms());
		bHaveDL = FYF_TRUE;
		return FYF_OK;
	}
}





/* ----------------------------------------------------------------------------
   Name: DMUX_ParseNitSection()

   Description:
   Parse the NIT section data, once if the particular NIT section is collected
   completely.

   Parameters:
   pSectionData:   The pointer of receive nit section data from table_id.

   Return Value:
   TP_NO_ERROR:                    				The operation was carried out without error.
   TPOWVOD_ERROR_NOT_CURRENT_SECT:				Receive not current section.
   TPOWVOD_ERROR_NOT_SERVICE_ENTRANCE:			Illogical service entrance descriptor received.
   ---------------------------------------------------------------------------- */
static BU32 DMUX_ParseNitSection(BU08 *pSectionData, BU08 bParseVodTs)
{
	BU08 *pucSectionData;
	BU08 ucLastSectionNumber;
	BU08 ucSectionNumber;
	BU08 ucVersionNumber;
	BU08 ucCurNextIndicator;
	BU16 network_id;
	BS16 sSectionLength;
	BU16 network_description_length;
	BS32 i;
	BS32 j;
	BU08 bDescriptor_tag = FALSE;
	BU08 Descriptor_length = 0;
	BU16 Service_entrance_type = 0;
	BU16 Frequency_para_length = 0;
	DMUX_VodTsParams_t ow_vod_fre_temp;
	BU08 Param_length = 0;
	BU08 small_length = 0;
	BU32 ivalue = 0;

	pucSectionData = pSectionData;
	sSectionLength = (pSectionData[1] & 0x0f) << 8 | pSectionData[2];
	network_id = (pucSectionData[3] << 8) | pucSectionData[4];

	if (bParseVodTs)
	{
		for (i = 0; i < VodTsNum; i++)
		{
			VodTsParams[i].RegionID = network_id;
		}
		
		RegionID = (BS32)network_id;
		printf("RegionID = %x\n",RegionID);
		return 0;
	}

	pucSectionData = (BU08 *) pSectionData + 10;
	ucVersionNumber = (pSectionData[5] & 0x3E) >> 1;
	ucCurNextIndicator = pSectionData[5] & 0x01;

	ucSectionNumber = pSectionData[6];
	ucLastSectionNumber = pSectionData[7];

	if (ucCurNextIndicator != 1)
	{
		IPANEL_ERROR("ucCurNextIndicator err\n");
		return 1;
	}

	network_description_length = (pSectionData[8] & 0x0f) << 8 | pSectionData[9];

	for (i = 0; i < network_description_length;)
	{
		if (0x96 == pucSectionData[i])
		{
			bDescriptor_tag = FYF_TRUE;
			break;
		}
		else
		{
			small_length = pucSectionData[i + 1];
			i += (small_length + 2);
		}
	}

	if (!bDescriptor_tag)
	{
		printf("get nit 0x96 Descriptor_tag err!!!  \n");
		return 2;
	}

	bDescriptor_tag = FYF_FALSE;

	pucSectionData += i;

	if ((DmuxNitVer < ucVersionNumber) || (31 == DmuxNitVer && 0 == ucVersionNumber))
	{
		DmuxNitVer = ucVersionNumber;
		DMUX_ResetVodTsParams();
	}

	Descriptor_length = pucSectionData[1];
	pucSectionData += 2;

	for (i = 0; i < Descriptor_length; i++)
	{
		Service_entrance_type = (pucSectionData[0] << 8) | pucSectionData[1];
		Frequency_para_length = pucSectionData[3];

		if (Service_entrance_type != 0x3)
		{
			printf("get nit 0x03 Descriptor_tag err!!!  \n");
		}

		pucSectionData += 4;
		for (j = 0; j < Frequency_para_length; j++)
		{
			ow_vod_fre_temp.FreqMhz = (pucSectionData[0] << 8) | pucSectionData[1];
			ivalue = ow_vod_fre_temp.FreqMhz;

			ow_vod_fre_temp.FreqMhz =
				((ivalue & 0xf000) >> 12) * 1000 + ((ivalue & 0x0f00) >> 8) * 100 + ((ivalue & 0x00f0) >> 4) * 10 + (ivalue & 0x000f);

			ow_vod_fre_temp.FEC_outer = pucSectionData[5] & 0x0f;
			ow_vod_fre_temp.Modulation = pucSectionData[6];
			ow_vod_fre_temp.Symbol_rate = (pucSectionData[8] << 8) | pucSectionData[9];
			ivalue = ow_vod_fre_temp.Symbol_rate;

			ow_vod_fre_temp.Symbol_rate =
				((ivalue & 0xf000) >> 12) * 1000 + ((ivalue & 0x0f00) >> 8) * 100 + ((ivalue & 0x00f0) >> 4) * 10 + (ivalue & 0x000f);

			ow_vod_fre_temp.FEC_inner = pucSectionData[10] & 0x0f;
			Param_length = pucSectionData[11];

			ow_vod_fre_temp.RegionPid = pucSectionData[12] << 8 | pucSectionData[13];
			j = 12 + Param_length;
		}

		DMUX_SetVodTsParams(ow_vod_fre_temp);

		pucSectionData += Frequency_para_length;
		i += (4 + Frequency_para_length);
		printf("main fre = %d,sym = %d,ridpid = 0x%x\n", ow_vod_fre_temp.FreqMhz, ow_vod_fre_temp.Symbol_rate,
			ow_vod_fre_temp.RegionPid);
	}

	return 0;
}


#ifdef __cplusplus
}
#endif



