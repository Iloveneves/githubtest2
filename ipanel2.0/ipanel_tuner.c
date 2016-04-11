/*********************************************************************
  Copyright (c) 2005 Embedded Internet Solutions, Inc
  All rights reserved. You are not allowed to copy or distribute
  the code without permission.
  This is the demo implenment of the EPG Porting APIs needed by iPanel MiddleWare.
  Maybe you should modify it accorrding to Platform.

Note: the "int" in the file is 32bits

$ver0.0.0.1 $author Zouxianyun 2005/04/28
 *********************************************************************/

/*********************************************************************
  Copyright (c) 2005 Embedded Internet Solutions, Inc
  All rights reserved. You are not allowed to copy or distribute
  the code without permission.
  This is the demo implenment of the EPG Porting APIs needed by iPanel MiddleWare.
  Maybe you should modify it accorrding to Platform.

Note: the "int" in the file is 32bits

$ver0.0.0.1 $author Zouxianyun 2005/04/28
 *********************************************************************/
#define IPANEL_API_VERSION
#include "trd_ipanel_init.h"
#include "ipanel_tuner.h"
#include "ipanel_porting_event.h"
#include "ipanel_media.h"
#include "./../tpmiddle/include/app_basic_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_PID             0x1fff

BU32 IpanelServiceid = 0;
BU08  ipanel_ture_tag = false;

extern BU08  g_bRefreshOsd;
extern void *g_hIpanel;
extern HI_TRD_FREQINFO_S g_stPreFreqInfo; 
extern BS32  s_pIpanelMsgQueue;
BU32	   frq;  
static p1message msg_tuner;

void ipanel_tuner_send_msg( unsigned int *event)
{
    FYF_QUEUE_MESSAGE stMsg;   
	
    stMsg.qlstWordOfMsg    = (unsigned int)event[0];
    stMsg.q2ndWordOfMsg    = (unsigned int)event[1];
    stMsg.q3rdWordOfMsg    = (unsigned int)event[2]; 
	stMsg.q4thWordOfMsg    = 0;
	if (0 != s_pIpanelMsgQueue)
		FYF_API_queue_send(s_pIpanelMsgQueue, &stMsg);
}

static int iPanelNoSignal(int id, int action, unsigned int p1, unsigned int p2)
{
	printf("iPanelNoSignal called !\n"); 
}


void  TRD_Check_Tuner(unsigned long *time_count ,int *bTuner_Firstlock ,int*bTuner_FirstUnlock, int *bNetErr)
{
   	unsigned char string[20];
	unsigned int event[3] = {0,0,0};
    FYF_QUEUE_MESSAGE Msg;
	int ret = -1;

					  
	                  
	if(15 == *time_count)
	{
		*time_count = 0;


		if	(Ipanel_GetEntrance_Flag())
		{
			ret = FYF_API_GetLinkStatus();
			if (ret != 1)		
			{
				if(*bNetErr == FYF_FALSE)
				{
					*bNetErr = FYF_TRUE;
					memset(string,0,20);
					memset(&msg_tuner,0,sizeof(p1message));
		
					msg_tuner.id		 =	0x1001; 		  /* 退出ID */
					msg_tuner.type		 = MESSAGE_TYPE_INSTANT;
					msg_tuner.priority	 = PRIORITY_NONE;
					msg_tuner.start_time  = 0;
					msg_tuner.duration	  = 0;
					msg_tuner.modifier	 = 0;
					msg_tuner.msg_len	 = 48;
		
					msg_tuner.button_lan  = BUTTON_LAN_CHS;
					msg_tuner.msg_text	  = "请插入网线!";
					msg_tuner.cbf	= iPanelNoSignal;
								
					event[0] = EIS_EVENT_TYPE_MESSAGE;
					event[1] = (int)&msg_tuner;
					event[2] = 0;
					
					printf("ipanel net work err!\n");
					Msg.qlstWordOfMsg = event[0];
					Msg.q2ndWordOfMsg = event[1];
					Msg.q3rdWordOfMsg = event[2];
					Msg.q4thWordOfMsg = 0;
					
					IPANEL_ERROR("ipanel unlock!\n");
					if (0 != s_pIpanelMsgQueue)
						FYF_API_queue_send(s_pIpanelMsgQueue, &Msg);
				}
			}
			else if((*bNetErr) == FYF_TRUE)
			{
				*bNetErr = FYF_FALSE;
				memset(string,0,20);
				memset(&msg_tuner,0,sizeof(p1message));
		
				msg_tuner.id		 =	0x1001; 		  /* 退出ID */
				msg_tuner.type		 = MESSAGE_TYPE_MOVESTRING;
				msg_tuner.priority	 = PRIORITY_NO_CANCEL_CURR_RESET_OTHER;
				msg_tuner.start_time  = 0;
				msg_tuner.duration	  = 0;
				msg_tuner.modifier	 = 0;
				msg_tuner.msg_len	 = 48;
		
				msg_tuner.button_lan  = BUTTON_LAN_CHS;
				msg_tuner.msg_text	  = "网络恢复!";
				msg_tuner.cbf	= iPanelNoSignal;
							
				event[0] = EIS_EVENT_TYPE_MESSAGE;
				event[1] = (int)&msg_tuner;
				event[2] = 0;
				
				printf("ipanel net ok again!\n");
				Msg.qlstWordOfMsg = event[0];
				Msg.q2ndWordOfMsg = event[1];
				Msg.q3rdWordOfMsg = event[2];
				Msg.q4thWordOfMsg = 0;
				
				IPANEL_DEBUG("ipanel net ok again!\n");
				if (0 != s_pIpanelMsgQueue)
					FYF_API_queue_send(s_pIpanelMsgQueue, &Msg);
			}
		}



		if(!ipanel_ture_tag)
		{
	        if( FYF_API_tuner_get_status(0) != 0)
	        {
				if (*bTuner_Firstlock == FYF_TRUE)
				{
					*bTuner_Firstlock = FYF_FALSE;
				}
				if (*bTuner_FirstUnlock == FYF_FALSE)
				{
					*bTuner_FirstUnlock = FYF_TRUE;
					
					memset(string, 0, 20);
					memset(&msg_tuner,0,sizeof(p1message));

					msg_tuner.id          = 0x1011;           /* 退出ID */
					msg_tuner.type        = MESSAGE_TYPE_INSTANT;
					msg_tuner.priority    = PRIORITY_NONE;
					msg_tuner.start_time  = 0;
					msg_tuner.duration    = 0;
					msg_tuner.modifier    = 0;
					msg_tuner.msg_len     = 48;

					msg_tuner.button_lan  = BUTTON_LAN_CHS;
					msg_tuner.msg_text    = "无信号，请检查信号连接线";
					msg_tuner.cbf  = iPanelNoSignal;
								
					event[0] = EIS_EVENT_TYPE_MESSAGE;
					event[1] = (int)&msg_tuner;
					event[2] = 0;
					Msg.qlstWordOfMsg = event[0];
					Msg.q2ndWordOfMsg = event[1];
					Msg.q3rdWordOfMsg = event[2];
					Msg.q4thWordOfMsg = 0;
					
					IPANEL_ERROR("ipanel unlock!\n");
					if (0 != s_pIpanelMsgQueue)
						FYF_API_queue_send(s_pIpanelMsgQueue, &Msg);

				}
	        }
			else
			{

				if (*bTuner_FirstUnlock == true)
				{
					*bTuner_FirstUnlock = false;
					
					memset(string,0,20);
					memset(&msg_tuner,0, sizeof(p1message));

					msg_tuner.id          =  0x1011;           /* 退出ID */
					msg_tuner.type        = MESSAGE_TYPE_MOVESTRING;
					msg_tuner.priority    = PRIORITY_NO_CANCEL_CURR_RESET_OTHER;
					msg_tuner.start_time  = 0;
					msg_tuner.duration    = 0;
					msg_tuner.modifier    = 0;
					msg_tuner.msg_len     = 48;

					msg_tuner.button_lan  = BUTTON_LAN_CHS;
					msg_tuner.msg_text    = "信号重锁!";
					msg_tuner.cbf  = iPanelNoSignal;
								
					event[0] = EIS_EVENT_TYPE_MESSAGE;
					event[1] = (int)&msg_tuner;
					event[2] = 0;
					
					Msg.qlstWordOfMsg = event[0];
					Msg.q2ndWordOfMsg = event[1];
					Msg.q3rdWordOfMsg = event[2];
					Msg.q4thWordOfMsg = 0;
					
					IPANEL_DEBUG("ipanel lock again!\n");
					if (0 != s_pIpanelMsgQueue)
						FYF_API_queue_send(s_pIpanelMsgQueue, &Msg);
				}
				
				if (*bTuner_Firstlock==false)
				{
					*bTuner_Firstlock = true;
				}
			}
		}
	}
}

BS32 TRD_LockTuner(BU32 freq, BU32 sym, BU32 qam)
{
	BS32 s32Ret = -1;
	BU32 j= 0;
	
	s32Ret = FYF_API_tuner_lock_delivery(0, freq, sym, qam, NULL);

	for(j = 0; j < 10; j++)
	{
		if((s32Ret = FYF_API_tuner_get_status(0)) == FYF_OK)
		{
			printf("Tuner Lock Success.\n");
			break;
		}
		printf(" wait to lock tuner j = %d\n", j);
		FYF_API_thread_sleep(100);
	}

	if (j == 10)
	{
		printf("unable to lock fre =%d, sym = %d, qam = %d.\n",freq, sym, qam);
		s32Ret = FYF_ERR;
	}

	return s32Ret;
}


BS32 TRD_GetTsidBySrvId(int usSrvId, int *Tsid)
{
	BroadCast_ChnInfo_S chinfo;
	BS32 ret = -1;
	int i = 0;
	ChnDataBase_ChnInfo_S ChannelInfo;

	printf("serid = %d\n", usSrvId);
	ret = DBA_API_AV_GetTsidByServID(&chinfo, usSrvId);
	if (-1 == ret )
	{
		for(i = 0; i < APP_ChnDatabase_API_GetCurTypeChnNum(); i++)
		{
			APP_ChnDatabase_API_GetChnInfo(i, &ChannelInfo);
			printf("serviceid = %x, progid = %x\n",usSrvId , ((ChannelInfo.m_ProgId /*>> 16*/) & 0xffff));
			if (usSrvId == ((ChannelInfo.m_ProgId /*>> 16*/) & 0xffff))
			{
				printf("got22========================\n");
				chinfo.m_TsId = ChannelInfo.m_TsId;
				ret = FYF_OK;
				break;
			}
		}	
		
		if (i == APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			ret = FYF_ERR;
		}

		if (ret == FYF_ERR)
		{
			IPANEL_ERROR("ret = %d\n",ret);
			return FYF_ERR;
		}
	}
	*Tsid = chinfo.m_TsId;
	IPANEL_DEBUG("tsid = %d\n", *Tsid);
	return FYF_OK;
}


BS32 TRD_SetTuner(int network_id, int ts_id, int service_id, BU32 *uProgVidPid, BU32 *uProgAudPid)
{
	BroadCast_ChnInfo_S chinfo;
	BS32 ret = -1;
	BS32 Freq=-1;
    int  i = 0;
	ChnDataBase_ChnInfo_S ChannelInfo;

	IPANEL_DEBUG("HI_TRD_AV_GetAVpidByServiceId network_id = %x  ts_id = %x service_id = %x\n",network_id,ts_id,service_id);

	
	ret = DBA_API_AV_GetFreqByServiceId(&chinfo, network_id, ts_id, service_id);
	if (-1 == ret )
	{	
		for(i = 0; i < APP_ChnDatabase_API_GetCurTypeChnNum(); i++)
		{
			APP_ChnDatabase_API_GetChnInfo(i, &ChannelInfo);
			printf("serviceid = %x, progid = %x\n",service_id , ((ChannelInfo.m_ProgId /*>> 16*/) & 0xffff));
			if (service_id == ((ChannelInfo.m_ProgId /*>> 16*/) & 0xffff))
			{
				printf("got========================\n");
				chinfo.m_VideoPid = ChannelInfo.m_VideoPid;
				chinfo.m_AudioPid = ChannelInfo.m_AudioPid;
				chinfo.m_FreqMHz  = ChannelInfo.m_FreqMHz;
				chinfo.m_QAM	  = ChannelInfo.m_QAM;
				chinfo.m_SymKbps  = ChannelInfo.m_SymKbps;
				memcpy(chinfo.m_ChnName, ChannelInfo.m_ChnName, strlen(ChannelInfo.m_ChnName));
				chinfo.m_OriNetId = ChannelInfo.m_OriNetId;
				chinfo.m_PcrPid   = ChannelInfo.m_PcrPid;
				chinfo.m_pmtpid   = ChannelInfo.m_pmtpid;
				chinfo.m_ProgId   = ChannelInfo.m_ProgId;
				ret = FYF_OK;
				break;
			}
		}

		if (i == APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			ret = FYF_ERR;
		}

		if (ret == FYF_ERR)
		{
			IPANEL_ERROR("ret = %d\n",ret);
			return FYF_ERR;
		}
		
	}

	IPANEL_DEBUG("chinfo.m_VideoPid = %d, m_AudioPid = %d\n", chinfo.m_VideoPid, chinfo.m_AudioPid);
	*uProgVidPid = (BU32)chinfo.m_VideoPid;
	*uProgAudPid = (BU32)chinfo.m_AudioPid;
	IPANEL_DEBUG("vid = %d, audio = %d\n", *uProgVidPid, *uProgAudPid);
	Freq = (chinfo.m_FreqMHz * 1000);



	printf("freq = %d======pcrid = %d===========================================================\n", Freq, chinfo.m_PcrPid);
	if (Freq != TRD_GetBroadcastFreq())
	{
		ipanel_ture_tag = true;
		printf("OC_SetTuner,Freq=%d\n",Freq);

		ret = TRD_LockTuner(Freq, (chinfo.m_SymKbps), chinfo.m_QAM);
		if (FYF_ERR == ret)
		{
			ipanel_ture_tag = false;
			IPANEL_ERROR("cannot tune to freq  = %d\n", Freq);
		}
		TRD_SetBroadcastFreq(Freq);
		APP_IPI_SaveTunerInfo(Freq, (chinfo.m_SymKbps), chinfo.m_QAM);
		ipanel_ture_tag = false;
		return ret;
	}
	else
	{
		ipanel_ture_tag = false;
		return 0;
	}
}

/**********************************************************************************/
/*Description: Get Signal Quality (0 ~ 100), 0 -- min, and 100 -- max             */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : signal quality                                                     */
/**********************************************************************************/
int ipanel_porting_get_delSignalQuality(void)
{

    BS32 quality = 0;

    IPANEL_DEBUG("\n");
    //status = HI_TUNERGetSNR(0);
	quality = FYF_API_tuner_get_signal_quality(0);
	if (quality == FYF_ERR) 
    {
        IPANEL_DEBUG("\n->[ipanel_porting_get_delSignalQuality] HI_TUNER_GetSNR is failed ! ret=0x%x \n",quality);
    }

    if (quality > 100)
    {
        quality = 100;
    }

    if (quality < 0)
    {
        quality = 0;
    }

    return quality;
}



/**********************************************************************************/
/*Description: Get Signal Strength (0 ~ 100), 0 -- min, and 100 -- max            */
/*Input      : No                                                                 */
/*Output     : No                                                                 */
/*Return     : signal strength                                                    */
/**********************************************************************************/
int ipanel_porting_get_delSignalStrength(void)
{
    
    int strength = 0;
     
    //status = HI_TUNERGetStrength(0);
    
    IPANEL_DEBUG("\n");

	strength = FYF_API_tuner_get_signal_strength(0);
	if (strength == FYF_ERR) 
    {
        IPANEL_DEBUG("\n->[ipanel_porting_get_delSignalQuality]  HI_TUNER_GetSignalStrength is failed ! ret=0x%x \n",strength);
    }

    if (strength > 100) strength = 100;
    if (strength < 0) strength = 0;

    return strength;
}


/**********************************************************************************/
/*Description: Select a audio or video program                                    */
/*Input      : pcr, emm, video, audio, audio_ecm, video_ecm description           */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail.                                          */
/**********************************************************************************/
int ipanel_porting_dvb_select_av(int pcr_pid,   int emm_pid, int video_pid,  int audio_pid, int audio_ecm_pid, int video_ecm_pid)
{
 #if 0    
 	unsigned int event[3] = {0};
    BS32 s32Ret = -1;
	int j = 0;
    HI_TRD_FREQINFO_S stFreqInfo = {0};   

    IPANEL_DEBUG("ipanel_porting_dvb_select_av");
    IPANEL_DEBUG("pcr_pid %d,video_pid %d, audio_pid %d\n", pcr_pid, video_pid, audio_pid);

   
    if ((0 != video_pid) || (0 != audio_pid))
    {
        /* 获得频点锁频点*/
        s32Ret = HI_TRD_AV_GetFreqByPid(&stFreqInfo, video_pid, audio_pid,pcr_pid);
		if (FYF_OK != s32Ret)
        {
            IPANEL_DEBUG("get freq form pid is fail\n");
            event[1] = EIS_DVB_DELIVERY_TUNE_FAILED;    
            event[2] = 0;            
        }
        else
        {
       
            IPANEL_DEBUG("TunerConnect : frq %d, sym %d, eqamType %d\n", 
                stFreqInfo.frq, stFreqInfo.sym, stFreqInfo.eqamType);

            //s32Ret = HI_TUNERLock(0, stFreqInfo.frq, stFreqInfo.sym,  stFreqInfo.eqamType, NULL);
            
			s32Ret = TRD_LockTuner(stFreqInfo.frq, stFreqInfo.sym,  stFreqInfo.eqamType);

			if(FYF_OK != s32Ret)
            {
                IPANEL_ERROR("TunerConnect fail and lock pre freq  frq %d,sym %d, qam %d\n",
                g_stPreFreqInfo.frq, g_stPreFreqInfo.sym,  g_stPreFreqInfo.eqamType);
                
				s32Ret = FYF_API_tuner_lock_delivery(0, 
												g_stPreFreqInfo.frq, 
												g_stPreFreqInfo.sym,
												g_stPreFreqInfo.eqamType,
												NULL);
                if(FYF_OK != s32Ret)
                {
                    event[0] = EIS_EVENT_TYPE_DVB;
                    event[1] = EIS_DVB_DELIVERY_TUNE_FAILED;
                    event[2] = 0;
                }
                else
                {
                    event[0] = EIS_EVENT_TYPE_DVB;
                    event[1] = EIS_DVB_DELIVERY_TUNE_SUCCESS;
                    event[2] = 0;            
                }
            }
            else
            {   
                memcpy(&g_stPreFreqInfo, &stFreqInfo, sizeof(HI_TRD_FREQINFO_S));
            
                /*if (0 == video_pid)
                {
                    video_pid = 0x1fff;
                }            
                if (0 == audio_pid)
                {
                    audio_pid = 0x1fff;
                }*/               

				s32Ret = APP_IPI_PlayProg( video_pid, audio_pid, pcr_pid);
				
				if(FYF_OK != s32Ret)
                {
                    IPANEL_DEBUG("HI_TRD_AV_StartByPid failed! 0x%x\n", s32Ret);
                    event[1] = EIS_DVB_DELIVERY_TUNE_FAILED;                        
                }
                else
                {
                    event[1] = EIS_DVB_DELIVERY_TUNE_SUCCESS;                    
                }
                event[0] = EIS_EVENT_TYPE_DVB;
                event[2] = 0;
            }
        }
		
        ipanel_tuner_send_msg(event);
    }
    return 1;
#else

	int ret = -1;
	int Serid;
	
    IPANEL_DEBUG("ipanel_porting_dvb_select_av");

	if(  pcr_pid >= 0 && emm_pid >= 0 ||  video_pid > 0 ||
	    audio_pid >0 && audio_ecm_pid >= 0 && video_ecm_pid >= 0 ) 
	{


		if(pcr_pid == 0)
			pcr_pid = 0x1fff;
		
		if(video_pid == 0)
		{ 
			video_pid = 0x1fff;   
		}
		else if(audio_pid == 0)
		{ 
			audio_pid = 0x1fff;   
		}
		else
		{ 
		
		}
		
		IPANEL_DEBUG("pcr_pid %d,video_pid %d, audio_pid %d\n", pcr_pid, video_pid, audio_pid);
		APP_IPI_PlayProg( video_pid,  audio_pid,  pcr_pid, FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
		ret = 0;
	}
	
	return ret;  

#endif
}

//static void physical_delivery_tune(void);
/**********************************************************************************/
/*Description: Tuner the frequency                                                */
/*Input      : frequency -- uints is 100Hz, symbol_rate -- uints is 100sym/s,     */
/*           : modulation (1 - 4) 1 - QAM16, 2 - QAM32, 3 - QAM64, 4 - QAM128     */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail.                                          */
/**********************************************************************************/
int ipanel_porting_delivery_tune(int frequency, int symbol_rate, int modulation)
{    
    int ret = -1;
    static int flagtuner = 0, freq, symrate, modl;
        
   /* if ( flagtuner == 0 && frequency > 0 && symbol_rate > 0 && (modulation > 0 && modulation <= 4) )
    {
        freq = frequency;
        symrate = symbol_rate;
        modl = modulation;

        flagtuner = 1;

        ret = 0;
    }*/
    IPANEL_DEBUG("ipanel_porting_delivery_tune\n");
    if (modulation > 0)
	{
		ret = FYF_API_tuner_lock_delivery(0,  frequency,  symbol_rate,  modulation, NULL);
	}


	
    return ret;
}

/**********************************************************************************/
/*Description: Open a service                                                     */
/*Input      : service play information                                           */
/*Output     : No                                                                 */
/*Return     : 0 -- success, -1 -- fail.                                          */
/**********************************************************************************/
int ipanel_porting_service_open(DVBServicePlay *serviceplay)
{    
    int ret = -1;
    if (NULL != serviceplay) 
    {
        /*physical deal here.
          such as play programmer.etc.
          play_program(serviceplay->serviceid, serviceplay->pcr_pid, serviceplay->video_pids[0],  serviceplay->audio_pids[0]);
         */
        ret = 0;
    }
    return ret;
}

/**********************************************************************************/
/*Description: Select a TS service                                                */
/*Input      : network id, ts stream id, service id                               */
/*Output     : No                                                                 */
/*Return     : 0 -- iMW admin service, 1 -- iMW don't admin service.              */
/**********************************************************************************/
int ipanel_porting_select_service(int network_id, int ts_id, int service_id)
{

    unsigned int event[3] = {0};
    int s32Ret;
    BU32 mode;
    BU32 u32VidPid;
    BU32 u32AudPid;
    BU32 u32PcrPid = 0;
	int j = 0;
    HI_TRD_FREQINFO_S stFreqInfo = {0};
	
	IPANEL_DEBUG("ipanel_porting_select_service");
    IPANEL_DEBUG("network_id 0x%x, ts_id 0x%x service_id 0x%x", network_id, ts_id, service_id);    


	s32Ret = TRD_SetTuner( network_id, ts_id, service_id, &u32VidPid, &u32AudPid);

	if(network_id == 0 && ts_id == 0)
	{
		IPANEL_DEBUG("network_id == 0 && ts_id == 0\n");
		if(s32Ret == 0)
		{
			int tsid; 
			BU08 tmpppp;

			if ( 0 == u32VidPid)
				u32VidPid = 0x1fff;
			if ( 0 == u32AudPid)
				u32AudPid = 0x1fff;
			//if ( 0 == u32PcrPid)
				//u32PcrPid = 0x1fff;
			printf("u32VidPid = %d,u32AudPid = %d, u32PcrPid = %x\n", u32VidPid, u32AudPid, u32PcrPid);
			
			s32Ret = APP_IPI_PlayProg(u32VidPid, u32AudPid, u32PcrPid, FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);

			if(FYF_OK != s32Ret)
			{
				IPANEL_ERROR("APP_IPI_PlayProg failed! 0x%x\n", s32Ret);                    
			}

			TRD_GetTsidBySrvId(service_id,&ts_id);
			
			event[0] = EIS_EVENT_TYPE_DVB;
			event[1] = EIS_DVB_DELIVERY_TUNE_SUCCESS;
			event[2] = tsid;

			IpanelServiceid = (BU32)service_id;
			
			ipanel_tuner_send_msg(event);

			IPANEL_DEBUG("TUNE_SUCCESS\n");
			


			return FYF_TRUE;
		}
		else if(s32Ret == FYF_ERR)
		{
		    int tsid;
			   
			printf("Ipanel_SetTuner err!");
			
			TRD_GetTsidBySrvId(IpanelServiceid, &tsid);

			event[0] = EIS_EVENT_TYPE_DVB;
			event[1] = EIS_DVB_DELIVERY_TUNE_FAILED;
			event[2] = tsid;
			
			ipanel_tuner_send_msg(event);
			
			IPANEL_DEBUG("TUNE_FAILED\n");
			
	        return FYF_TRUE;
		}
	}
	else
	{
			
			if(s32Ret == 0)
			{
				event[0] = EIS_EVENT_TYPE_DVB;
				event[1] = EIS_DVB_DELIVERY_TUNE_SUCCESS;
				event[2] = ts_id;
				
				IpanelServiceid = (BU32)service_id;
				
				ipanel_tuner_send_msg(event);
	
				IPANEL_DEBUG("TUNE_SUCCESS\n");
				return FYF_TRUE;
	
			}
			else if(s32Ret == FYF_ERR)
			{

				int tsid;
				
				printf("Ipanel_SetTuner err!");

				TRD_GetTsidBySrvId(IpanelServiceid, &tsid);

				event[0] = EIS_EVENT_TYPE_DVB;
				event[1] = EIS_DVB_DELIVERY_TUNE_FAILED;
				event[2] = ts_id;
				
				ipanel_tuner_send_msg(event);
	
				IPANEL_DEBUG("TUNE_FAILED\n");

				return FYF_TRUE;
			}
			/*else if(tuner_lock_type == 2)	 
			{			
				event[0] = EIS_EVENT_TYPE_DVB;
				event[1] = EIS_DVB_DELIVERY_TUNE_FAILED;
				event[2] = ts_id;
				
				ipanel_tuner_send_msg(event);
					  return FYF_TRUE;
			}*/
		}

#if 0
    /*要进行同频点或跨频点的数据广播业务*/
    if((0 == network_id) && (0 == ts_id)) 
    {
        printf("ipanel_porting_select_service : AV mode\n");
        mode = 1;
    }
    else 
    {
        printf("ipanel_porting_select_service : OC mode\n");
        mode = 0;
    }
	

	IpanelServiceid = (BU32)service_id;
	
    s32Ret = HI_TRD_AV_GetPreqByServiceId(&stFreqInfo, network_id, ts_id, service_id);
    if (FYF_OK != s32Ret)
    {
        IPANEL_ERROR("get freq form seviceid is fail\n");
        event[1] = EIS_DVB_DELIVERY_TUNE_FAILED;                                                       
    }
    else
    {
        IPANEL_DEBUG("TunerConnect : %d, %d, %d\n", 
            stFreqInfo.frq, stFreqInfo.sym, stFreqInfo.eqamType);
        
        //s32Ret = HI_TUNERLock(0, stFreqInfo.frq, 
        //                stFreqInfo.sym,  stFreqInfo.eqamType, NULL);
		s32Ret = FYF_API_tuner_lock_delivery(0, 
									stFreqInfo.frq,
									stFreqInfo.sym, 
									stFreqInfo.eqamType, 
									NULL);

		for(j = 0; j < 30; j++)
		{
			if(FYF_API_tuner_get_status(0) == 0)
			{
				s32Ret = FYF_OK;
				break;
			}
			printf(" wait to lock tuner \n" );
			FYF_API_thread_sleep(100);
			j++;
		}
		
		if (j == 30)
		{
			IPANEL_ERROR("unable to lock fre =%d, sym = %d, qam = %d.\n",stFreqInfo.frq, stFreqInfo.sym, stFreqInfo.eqamType);
			s32Ret = FYF_ERR;
		}

        if(FYF_OK != s32Ret)
        {
            IPANEL_ERROR("TunerConnect fail and lock pre freq  frq %d,sym %d, qam %d\n",
            g_stPreFreqInfo.frq, g_stPreFreqInfo.sym,  g_stPreFreqInfo.eqamType);
            
            //s32Ret = HI_TUNERLock(0, g_stPreFreqInfo.frq, 
            //            g_stPreFreqInfo.sym,  g_stPreFreqInfo.eqamType, NULL);
			s32Ret = FYF_API_tuner_lock_delivery(0, 
								g_stPreFreqInfo.frq, 
								g_stPreFreqInfo.sym,
								g_stPreFreqInfo.eqamType,
								NULL);

            if(FYF_OK != s32Ret)
            {
                event[0] = EIS_EVENT_TYPE_DVB;
                event[1] = EIS_DVB_DELIVERY_TUNE_FAILED;
                event[2] = 0;
            }
            else
            {
                event[0] = EIS_EVENT_TYPE_DVB;
                event[1] = EIS_DVB_DELIVERY_TUNE_SUCCESS;
                event[2] = 0;            
            }
        }
        else
        {
            /*锁频成功*/
            memcpy(&g_stPreFreqInfo, &stFreqInfo, sizeof(HI_TRD_FREQINFO_S));
            if(mode > 0)
            {                      
                IPANEL_DEBUG("start by svr id play av\n");
                s32Ret = HI_TRD_AV_GetAVpidByServiceId(&u16VidPid, &u16AudPid, &u16PcrPid,
                    network_id, ts_id, service_id);    
                
                /*if (0 == u16VidPid)
                {
                    u16VidPid = 0x1fff;
                }
                if (0 == u16AudPid)
                {
                    u16AudPid = 0x1fff;
                }
                if (0 == u16VidPid)
                {
                    u16VidPid = 0x1fff;
                }    
                */
				
                IPANEL_DEBUG("play av vidpid is %d, audpid is %d, prcpid is %d\n\n",  u16VidPid, u16AudPid, u16PcrPid);                   

				s32Ret = APP_IPI_PlayProg(u16VidPid, u16AudPid, u16PcrPid);

				if(FYF_OK != s32Ret)
				{
                    IPANEL_ERROR("APP_IPI_PlayProg failed! 0x%x\n", s32Ret);                    
                }
				
            }
            event[0] = EIS_EVENT_TYPE_DVB;
            event[1] = EIS_DVB_DELIVERY_TUNE_SUCCESS;
            event[2] = ts_id;
        }
        ipanel_tuner_send_msg(event);

        return 1;
        
    }
    return 1;
#endif
}


/**********************************************************************************/
/*Description: Get the startup service id                                         */
/*Input      : network id, ts stream id, service id                               */
/*Output     : No                                                                 */
/*Return     : 0 -- iMW admin service, 1 -- iMW don't admin service.              */
/**********************************************************************************/
unsigned short ipanel_porting_get_startup_serviceid(void)
{
    //HI_TRD_AV_GetServiceId(&serviceid);
    IPANEL_DEBUG("get serverid is %d", IpanelServiceid);
	
    return IpanelServiceid;
}
int ipanel_porting_isMute(void)
{    
   /* BU08 bMute;
    //Hi_AV_GetMuteStatus(&bMute);
    FYF_API_adec_ioctl(NULL, FYF_ADEC_GET_MUTE,&bMute);

	
    IPANEL_DEBUG("bMute is %d\n", bMute);    
    return bMute;*/
    
   printf("============ipanel_porting_isMute=%d\n",!ipanel_GetAudMuteState());
   return !ipanel_GetAudMuteState();
}

extern BU08 g_Destroy;

int ipanel_porting_system_close()
{
	IPANEL_DEBUG("ipanel_porting_system_close\n");    
    g_bRefreshOsd = FYF_FALSE;
	g_Destroy = FYF_TRUE;
	//HI_TRD_IPANEL_DeInit();
	return 0;
}

int ipanel_porting_device_read()
{
     return -1;
}

int ipanel_porting_device_write()
{
  return -1;
}

int ipanel_porting_get_smartcard_id(char *buf, int len)
{
   return -1;
}



#ifdef __cplusplus
}
#endif

