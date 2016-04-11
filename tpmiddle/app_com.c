#include <stdio.h>
#include "app_common.h"
#include "app_ipi_common.h"

#include "../enreach/evtc.h"

static BU32 audioChannelID = 0;
BU32 g_AudioDecoderID = 0;
BU32 serviceInfo_cas_other = 1;

/*
cwd add
播放剩余次数，用来处理快速切台的丢掉
不需要处理的按键
*/
BU32 g_play_times_remaind = 0;
BU32 g_play_times_mutex = 0;
BU32 g_play_timeshift_mutex = 0;
static BU32 adecArg = 0;
static BU32 vdecArg = 0;
static BU32 videoChannelID = 0;
static BU32 videoDecoderID = 0;

static BU32 pcrChannelID = 0;
static BU32 pcrDecoderID = 0;

BU32 videoOutput = 0;
BU32 g_AudioOutput = 0;

static BU32 g_PlayChnFlag = 0;
static BU32 g_ScrnBlkState = 0;
static BU32 g_PlayProgramChange = 0;

static BU08 g_u8bShowLogoFlag = 0;

BU32 g_IsStopPlay = 1;

BU32 g_videoPlayState = 0;
BU32 g_audioPlayState = 0;

extern BU16 nvodPlayServiceId;
extern BU32 nvodPlayFlag;

FYF_Rect_s		g_rect =
{
0, 0, 432, 288
};

static BU08 led[4];

extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];

extern BS32 e2h_g_CurChnNo;
extern BU08 isStopSearch;

static SIGNALNOTIFY SignalNotify = NULL;

typedef enum
{
	APP_IPI_PLAY_STATE_NOSIGNAL,
	APP_IPI_PLAY_STATE_RADIO,
	APP_IPI_PLAY_STATE_VIDEO,
	APP_IPI_PLAY_STATE_NULL
}APP_IPI_PLAY_STATE_e;

static APP_IPI_PLAY_STATE_e app_ipi_play_state = APP_IPI_PLAY_STATE_NULL;
static APP_IPI_PLAY_STATE_e app_ipi_screen_state = APP_IPI_PLAY_STATE_NULL;
static BU32 APP_IPI_old_videoPID = 0;
static BU32 APP_IPI_old_audioPID = 0;
BU32 APP_COM_IPI_GetAudioHandle()
{
	return audioChannelID;
}
BU32 APP_COM_IPI_GetVideoHandle()
{
	return videoChannelID;
}

void APP_COM_IPI_InitAudio(void)
{
//	adecArg 			= FYF_AUDIO_CODEC_DEFAULT;
	//audioChannelID 		= FYF_API_demux_create_channel(32 * 1024, FYF_DEMUX_AUDIO_CHANNEL, &audioChannelID);
	FYF_API_demux_create_channel(32 * 1024, FYF_DEMUX_AUDIO_CHANNEL, &audioChannelID, 0);
	APP_PRINT_DEBUG("audiochannelid = %d\n", audioChannelID);
	g_AudioDecoderID 	= FYF_API_adec_open();

//	videoChannelID 		= FYF_API_demux_create_channel(32 * 1024, FYF_DEMUX_VIDEO_CHANNEL);
	FYF_API_demux_create_channel(32 * 1024, FYF_DEMUX_VIDEO_CHANNEL, &videoChannelID, 0);
	videoDecoderID 		= FYF_API_vdec_open();
	APP_PRINT_DEBUG("videochannelid = %d\n", videoChannelID);
//	pcrChannelID 		= FYF_API_demux_create_channel(32 * 1024, FYF_DEMUX_PCR_CHANNEL);
	FYF_API_demux_create_channel(32 * 1024, FYF_DEMUX_PCR_CHANNEL, &pcrChannelID, 0);

	g_AudioOutput 		= FYF_API_aout_open();
	videoOutput         = FYF_API_vout_open();
	

}



void APP_IPI_SetPlayFlag(BU32 value)
{
	g_PlayChnFlag = value;
}

BU32 APP_IPI_GetPlayFlag(void)
{
	return g_PlayChnFlag ;
}


//表示当前是处于节目播放还是节目停止状态，当要显示logo或者黑屏的时候可以通过该变量来判断是否需要先停止节目
//以及停止节目后是否要恢复播放
void APP_IPI_SetIsPlayStop(BU32 value)
{
	g_IsStopPlay = value;
}

BU32 APP_IPI_GetIsPlayStop(void)
{
	return g_IsStopPlay ;
}

void APP_IPI_SetPlayState(APP_IPI_PLAY_STATE_e value)
{
	app_ipi_play_state = value;
}

APP_IPI_PLAY_STATE_e APP_IPI_GetPlayState(void)
{
	return app_ipi_play_state ;
}
void APP_IPI_SetVideoState(BU32 value)
{
	g_videoPlayState = value;
}

BU32 APP_IPI_GetVideoState(void)
{
	return g_videoPlayState ;
}
void APP_IPI_SetAudioState(BU32 value)
{
	g_audioPlayState = value;
}

BU32 APP_IPI_GetAudioState(void)
{
	return g_audioPlayState ;
}
void APP_IPI_SetPlayChnChange(BU32 value)
{
	g_PlayProgramChange = value;
}

BU32 APP_IPI_GetPlayChnChange(void)
{
	return g_PlayProgramChange ;
}

//停止音视频
void APP_IPI_StopAudioVideo(BU32 black_screen)
{
	BU32  videostate = black_screen;

	if (APP_IPI_GetAudioState() != 0)
	{
		FYF_API_adec_ioctl(g_AudioDecoderID, FYF_ADEC_STOP, &adecArg);
		APP_IPI_SetAudioState(0);
	}
	if (APP_IPI_GetVideoState() != 0)
	{
		FYF_API_vdec_ioctl(videoDecoderID, FYF_VDEC_STOP, &videostate);
		APP_IPI_SetVideoState(0);
	}
}

void APP_IPI_SetVideoOutRect(BU32 x, BU32 y, BU32 width, BU32 height)
{
	g_rect.x = x;
	g_rect.y = y;
	g_rect.w = width;
	g_rect.h = height;
	FYF_API_vout_ioctl(videoOutput, FYF_VOUT_SET_WIN_LOCATION, &g_rect);
}



static BU32 oldFreq = 0;



BS32 APP_IPI_StopProg(BU32 videoPID, BU32 audioPID)
{
	if (videoPID == 0 || videoPID == 0x1fff)
	{
		APP_IPI_StopAudioVideo(0);
	}
	else if (videoPID != 0)
	{
		APP_IPI_StopAudioVideo(0);
	}
}

BS32 APP_IPI_PlayProg(BU32 videoPID, BU32 audioPID, BU32 pcrPID, BU32 videoType, BU32 audioType)
{
	BS32 ret = FYF_OK;

	if(oldFreq != APP_IPI_GetTunerFreq())
	{
		oldFreq = APP_IPI_GetTunerFreq();
		APP_IPI_old_audioPID = 0;
		APP_IPI_old_videoPID = 0;
	}
	
	if (videoPID == 0 || videoPID == 0x1fff)
	{
		if(APP_IPI_old_audioPID != audioPID || (APP_IPI_GetPlayState() != APP_IPI_PLAY_STATE_RADIO))
		{
			//APP_IPI_SetPlayChnChange(1);
			APP_IPI_StopAudioVideo(0);
			ret = FYF_API_demux_set_channel_pid(audioChannelID, (BU16) audioPID);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, set_channel_pid audioChannelID error\n");
			}
			
			ret = FYF_API_demux_start_channel(audioChannelID);

			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, start_channel audioChannelID error\n");
			}

			ret = FYF_API_adec_ioctl(g_AudioDecoderID, FYF_ADEC_START, &audioType);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, FYF_API_adec_ioctl  g_AudioDecoderID error1 \n");
			}
			
			APP_IPI_SetAudioState(1);
			APP_IPI_old_audioPID = audioPID;
			APP_IPI_old_videoPID = 0x1fff;
			APP_PRINT_DEBUG("ps = %d, ss = %d\n", APP_IPI_GetPlayState(), app_ipi_screen_state);

			APP_IPI_SetPlayState(APP_IPI_PLAY_STATE_RADIO);
			
		}
	}
	else if (videoPID != 0)
	{
		if(APP_IPI_old_videoPID != videoPID || (APP_IPI_GetPlayState() != APP_IPI_PLAY_STATE_VIDEO))
		{
			//APP_IPI_SetPlayChnChange(1);
			APP_IPI_StopAudioVideo(0);
			ret = FYF_API_demux_set_channel_pid(pcrChannelID, (BU16) pcrPID);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, set_channel_pid pcrChannelID error\n");
			}
			ret = FYF_API_demux_start_channel(pcrChannelID);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, start_channel pcrChannelID error\n");
			}
			
			ret = FYF_API_demux_set_channel_pid(audioChannelID, (BU16) audioPID);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, set_channel_pid audioChannelID error\n");
			}
			
			ret = FYF_API_demux_start_channel(audioChannelID);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, start_channel audioChannelID error\n");
			}
			
			ret = FYF_API_adec_ioctl(g_AudioDecoderID, FYF_ADEC_START, &audioType);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, FYF_API_adec_ioctl g_AudioDecoderID error2\n");
			}
		
			APP_IPI_SetAudioState(1);
			APP_IPI_old_videoPID = videoPID;
			ret = FYF_API_demux_set_channel_pid(videoChannelID, (BU16) videoPID);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, set_channel_pid videoChannelID error\n");
			}
	
			ret = FYF_API_demux_start_channel(videoChannelID);
			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, start_channel videoChannelID error\n");
			}

			ret = FYF_API_vdec_ioctl(videoDecoderID, FYF_VDEC_START, &videoType);

			if (FYF_OK != ret)
			{
				APP_PRINT_ERROR("APP_IPI_PlayProg, FYF_API_vdec_ioctl videoDecoderID error\n");
			}
			APP_IPI_SetVideoState(1);
			APP_IPI_SetPlayState(APP_IPI_PLAY_STATE_VIDEO);
		}
	}

	return ret;
}
extern BU32 semaphoreEcm;
void APP_IPI_PlayProg_NoPsiSi(ChnDataBase_ChnInfo_NoPsiSi_s* pChnInfo)
{
	FYF_Ecm_Info_s serviceInfo;
	APP_IPI_PlayProg(pChnInfo->m_VideoPid, pChnInfo->m_AudioPid, pChnInfo->m_PcrPid, FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
	FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
	FYF_API_Memset(&serviceInfo, 0, sizeof(serviceInfo));
	serviceInfo_cas_other = 1;
	FYF_API_SetCurEcmInfo(&serviceInfo);
	FYF_API_semaphore_release (semaphoreEcm);
}
#define FYF_COM_IPI_FUNTION
#ifdef FYF_COM_IPI_FUNTION
extern BU32 semaphoreEcm;

BU32 APP_Com_API_SendPlayChn(BU16 chnNO);

BU32 APP_Com_API_Wait()
{
	FYF_QUEUE_MESSAGE msgSend, msgRecv;
#ifdef ENABLE_BROADCAST_IPANEL
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_Ipannel_Start;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	msgRecv.qlstWordOfMsg = 0;
	FYF_API_queue_recv(APP_IPI_GetQueueIpanel(), &msgRecv, 1000);
#endif
	return 	msgRecv.qlstWordOfMsg;
	
}
void APP_Com_IPI_SetEcmPid(BU16 ChnNo)
{
	FYF_Ecm_Info_s serviceInfo;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	BS32 ret = 0;
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), ChnNo,&ProgInfoListtemp))
	{
		APP_PRINT_ERROR("APP_Com_IPI_PlayChn  over = %d\n",ChnNo);
		return; 
	}
	FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);	
	APP_PRINT_DEBUG("m_CAT_Sys_ID = 0x%x, %d\n", ProgInfoListtemp.m_CAT_Sys_ID, ProgInfoListtemp.m_EcmPID);
	ret = FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)&(ProgInfoListtemp.m_CAT_Sys_ID),0);	
	if(FYF_TRUE == ret)
	{
		serviceInfo.m_wCaSysID = ProgInfoListtemp.m_CAT_Sys_ID;
		if(ProgInfoListtemp.m_videoEcmPID == 0)
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = ProgInfoListtemp.m_EcmPID;
		}
		else
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = ProgInfoListtemp.m_videoEcmPID;
		}
		if(ProgInfoListtemp.m_audioEcmPID == 0)
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = ProgInfoListtemp.m_EcmPID;
		}
		else
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = ProgInfoListtemp.m_audioEcmPID;
		}
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(ProgInfoListtemp.m_ProgId);
		serviceInfo_cas_other = 0;
	}
	else
	{
		serviceInfo.m_wCaSysID = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(ProgInfoListtemp.m_ProgId);
		serviceInfo_cas_other = 1;
	}
	if(!(ProgInfoListtemp.m_CAT_Sys_ID))
	{
		serviceInfo_cas_other = 0;					
	}
	FYF_API_SetCurEcmInfo(&serviceInfo);
	FYF_API_semaphore_release (semaphoreEcm);

	FYF_API_SetProgramChange();
}

/*
根据节目号播放节目
*/
void APP_Com_IPI_PlayChn(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 u16ChnNO, BS32 s32ServiceID)
{
	BU32 pcrPID 	= 0;
	BU32 video 		= 0;
	BU32 audio 		= 0;
	BU32 freq 		= 0;
	BU32 symbol 	= 0;
	BU32 modulation = 0;
	BU16 emmpid = 0;
	BU16 caid = 0;
	BU08 videotype  = 0;
	BU08 audiotype  = 0;
	BU08 audio_track = APP_CHN_TRACK_SINGLE_e;
	BU16 audio_volume;
	BU32 i = 0;
	FYF_Ecm_Info_s serviceInfo;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	FYF_time_shifted_service_s shiftServiceInfo;
	BU32 tunerState = FYF_TUNER_STATUS_FAIL;
	BS32 ret = 0;
	/*cwd add 09-11-09
	当g_play_times_remaind不为1的时候，表示
	不是最后一个切的台，则直接返回
	*/
	FYF_API_mutex_lock(g_play_times_mutex, 0);
	if (g_play_times_remaind == 0)
	{
		while(1)
		{
			APP_PRINT_ERROR("play_times = %d\n", g_play_times_remaind);
		}
	}
	APP_PRINT_DEBUG("play_times = %d\n", g_play_times_remaind);
	if (--g_play_times_remaind > 0)
	{
		FYF_API_mutex_unlock(g_play_times_mutex);
		return;
	}
	FYF_API_mutex_unlock(g_play_times_mutex);


	//播放电视或者广播
	if (s32ServiceID == -1 && AvType <= APP_CHN_AV_TYPE_RD_e)
	{
		APP_PRINT_DEBUG(" APP_Com_IPI_PlayChn   want  to %d \n ",u16ChnNO);

		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(AvType, GroupType,u16ChnNO,&ProgInfoListtemp))
		{
			APP_PRINT_ERROR("APP_Com_IPI_PlayChn  over = %d\n",u16ChnNO);
			return; 
		}

		pcrPID	= ProgInfoListtemp.m_PcrPid;
		video 	= ProgInfoListtemp.m_VideoPid;
		audio 	= ProgInfoListtemp.m_AudioPid;
		freq 	= ProgInfoListtemp.m_FreqMHz;
		symbol 	= ProgInfoListtemp.m_SymKbps;
		modulation = ProgInfoListtemp.m_QAM;	
		videotype  = ProgInfoListtemp.m_VideoType;
		audiotype  = ProgInfoListtemp.m_AudioType;
		audio_track = ProgInfoListtemp.m_AudioChn;
		audio_volume = APP_Audio_API_GetActualVolume(ProgInfoListtemp.m_AudioOffset, ProgInfoListtemp.m_AudioVolume);
#if 0
			sys_volume_tmp + ProgInfoListtemp.m_AudioOffset;
		if (sys_volume_tmp)
		{
			if (sys_volume_tmp +  ProgInfoListtemp.m_AudioOffset > 0)
			{
				if (sys_volume_tmp +  ProgInfoListtemp.m_AudioOffset > 32)
				{
					audio_volume = 64;
				}
				else
				{
					audio_volume = (sys_volume_tmp +  ProgInfoListtemp.m_AudioOffset)*2;
				}
				
			}
			else
			{
				audio_volume = 1;
			}
		}
		else
		{
			audio_volume = 0;
		}
#endif
	}
	else
	{
		APP_SYS_API_GetSysInfo(APP_SYS_NVOD_FREQ_e, &freq);
		//freq = APP_NVOD_FREQ_MHZ;
		if (APP_FAIL == FYF_API_time_get_pid(s32ServiceID,&shiftServiceInfo))
		{
			APP_PRINT_ERROR("FYF_API_time_get_pid error nvodServiceId = %d\n", s32ServiceID);
			return;
		}

		pcrPID	= shiftServiceInfo.pcrpid;
		video 	= shiftServiceInfo.vpid;
		audio 	= shiftServiceInfo.apid;
		symbol 	= 6875;
		modulation = 2;	
		videotype  = FYF_VCODEC_TYPE_MPEG2;
		audiotype  = FYF_ACODEC_TYPE_MP3;
		audio_track = APP_CHN_TRACK_SINGLE_e;
		audio_volume = APP_Audio_API_GetActualVolume(0);
		ProgInfoListtemp.m_CAT_Sys_ID = shiftServiceInfo.sCAT_Sys_ID;
		ProgInfoListtemp.m_ProgId = s32ServiceID;
		ProgInfoListtemp.m_videoEcmPID = shiftServiceInfo.videoEcmPid;
		ProgInfoListtemp.m_audioEcmPID = shiftServiceInfo.audioEcmPid;
		ProgInfoListtemp.m_EcmPID = shiftServiceInfo.ecmPID;
	}
		


	if ((freq != APP_IPI_GetTunerFreq() && freq >=51 && freq <= 859) || (symbol != APP_IPI_GetTunerSym()) || (modulation != APP_IPI_GetTunerMod()))
	{

		oldFreq = 0;
		
		FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);	
		APP_IPI_SaveTunerInfo((BS32)freq, (BS32)symbol, (BS32)(modulation));
		//2010-05-22 cwd add
		//锁频前先停止节目，防止两个频点之间出现音频pid与其他pid复用的情况而产生噪音
		APP_IPI_StopProg(video, audio);
		FYF_API_tuner_lock_delivery(0, (BS32)(freq * 1000), (BS32)symbol, (BS32)(modulation), (BS32)freq);
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
		
		tunerState = FYF_TUNER_STATUS_FAIL; 	

		for(i =0 ;i < 40 ;i++)
		{
			FYF_API_thread_sleep(50);
			if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
			{
				tunerState = FYF_TUNER_STATUS_SUCCESS;
				break;
			}

			if (g_play_times_remaind > 0)
			{
				return;
			}
		
		}
		
		APP_PRINT_DEBUG("********PlayChn tuner lock state = %d time = %dms********\n",tunerState,FYF_API_time_ms());

	}
	
	APP_TDT_IPI_StartDemuxTdt();
	
	APP_Cat_IPI_StartDemuxCat(APP_Cat_IPI_ParCatOnChangeFreq);

	APP_Audio_API_SetTrack(audio_track);
	APP_Audio_API_SetVolume(audio_volume);
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_Hide_HDPROMPT, 0, 0);
	APP_IPI_PlayProg(video, audio, pcrPID, videotype, audiotype);


	

	APP_SYS_API_SaveSysInfo ();

	FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);	

	//app_print_info("m_CAT_Sys_ID = 0x%x\n", ProgInfoListtemp.m_CAT_Sys_ID);
	ret = FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)&(ProgInfoListtemp.m_CAT_Sys_ID),0);	
	if(FYF_TRUE == ret)
	{
		serviceInfo.m_wCaSysID = ProgInfoListtemp.m_CAT_Sys_ID;
		if(ProgInfoListtemp.m_videoEcmPID == 0)
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = ProgInfoListtemp.m_EcmPID;
		}
		else
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = ProgInfoListtemp.m_videoEcmPID;
		}
		if(ProgInfoListtemp.m_audioEcmPID == 0)
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = ProgInfoListtemp.m_EcmPID;
		}
		else
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = ProgInfoListtemp.m_audioEcmPID;
		}
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(ProgInfoListtemp.m_ProgId);
		serviceInfo_cas_other = 0;
	}
	else
	{
		serviceInfo.m_wCaSysID = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(ProgInfoListtemp.m_ProgId);
		serviceInfo_cas_other = 1;
	}
	if(!(ProgInfoListtemp.m_CAT_Sys_ID))
	{
		serviceInfo_cas_other = 0;					
	}
	if (strstr(ProgInfoListtemp.m_ChnName, "高清") != NULL || strstr(ProgInfoListtemp.m_ChnName, "HD") != NULL)
	{
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = 0;
	}
	FYF_API_SetCurEcmInfo(&serviceInfo);
	FYF_API_semaphore_release (semaphoreEcm);

	FYF_API_SetProgramChange();
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_Show_HDPROMPT, 0, 0);

}


#if 0
/*
根据节目号播放节目
*/
void APP_Com_IPI_PlayChn(BU32 chnNO)
{
	BU32 pcrPID 	= 0;
	BU32 video 		= 0;
	BU32 audio 		= 0;
	BU32 freq 		= 0;
	BU32 symbol 	= 0;
	BU32 modulation = 0;
	BU16 emmpid = 0;
	BU16 caid = 0;
	BU08 videotype  = 0;
	BU08 audiotype  = 0;
	BU32 i = 0;
	FYF_Ecm_Info_s serviceInfo;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	BU32 tunerState = FYF_TUNER_STATUS_FAIL;
	BS32 ret = 0;
	BU32 novd_freq;
	/*cwd add 09-11-09
	当g_play_times_remaind不为1的时候，表示
	不是最后一个切的台，则直接返回
	*/
	FYF_API_mutex_lock(g_play_times_mutex, 0);
	if (g_play_times_remaind == 0)
	{
		while(1)
		{
			APP_PRINT_ERROR("play_times = %d\n", g_play_times_remaind);
		}
	}
	APP_PRINT_DEBUG("play_times = %d\n", g_play_times_remaind);
	if (--g_play_times_remaind > 0)
	{
		FYF_API_mutex_unlock(g_play_times_mutex);
		return;
	}
	FYF_API_mutex_unlock(g_play_times_mutex);
		

	APP_PRINT_DEBUG(" APP_Com_IPI_PlayChn   want  to %d \n ",chnNO);

	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),chnNO,&ProgInfoListtemp))
	{
		APP_PRINT_ERROR("APP_Com_IPI_PlayChn  over = %d\n",chnNO);
		return; 
	}



	pcrPID	= ProgInfoListtemp.m_PcrPid;
	video 	= ProgInfoListtemp.m_VideoPid;
	audio 	= ProgInfoListtemp.m_AudioPid;
	freq 	= ProgInfoListtemp.m_FreqMHz;
	symbol 	= ProgInfoListtemp.m_SymKbps;
	modulation = ProgInfoListtemp.m_QAM;	
	videotype  = ProgInfoListtemp.m_VideoType;
	audiotype  = ProgInfoListtemp.m_AudioType;
	

	if (freq != APP_IPI_GetTunerFreq() && freq >=51 && freq <= 859)
	{

		oldFreq = 0;

		
		FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);	
		APP_IPI_SaveTunerInfo((BS32)freq, (BS32)symbol, (BS32)(modulation));
		FYF_API_tuner_lock_delivery(0, (BS32)(freq * 1000), (BS32)symbol, (BS32)(modulation), (BS32)freq);
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
		
		tunerState = FYF_TUNER_STATUS_FAIL; 	

		for(i =0 ;i < 40 ;i++)
		{
			if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
			{
				tunerState = FYF_TUNER_STATUS_SUCCESS;
				break;
			}
		/*	if(APP_IPI_GetPlayFlag() == 1)
			{
				APP_IPI_SetPlayFlag(0);
				tunerState = FYF_TUNER_STATUS_FAIL;
				break;				
			}
			*/
			if (g_play_times_remaind > 0)
			{
				return;
			}
			FYF_API_thread_sleep(50);
		}
		
		APP_PRINT_DEBUG("********PlayChn tuner lock state = %d time = %dms********\n",tunerState,FYF_API_time_ms());



	}
	
	APP_TDT_IPI_StartDemuxTdt();
	
	APP_Cat_IPI_StartDemuxCat(APP_Cat_IPI_ParCatOnChangeFreq);

	
//	APP_IPI_PlayProg(video, audio, pcrPID);
	APP_IPI_PlayProg(video, audio, pcrPID, videotype, audiotype);
	
	/*设置静音*/
//	APP_Audio_API_SetMuteState(APP_Audio_API_GetMuteState());
	
//	app_system_data.g_CurChnNo = chnNO;
	APP_SYS_API_SaveSysInfo ();

	FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);	

//	app_print_info("m_CAT_Sys_ID = 0x%x\n", ProgInfoListtemp.m_CAT_Sys_ID);
	ret = FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)&(ProgInfoListtemp.m_CAT_Sys_ID),0);	
	if(FYF_TRUE == ret)
	{
		serviceInfo.m_wCaSysID = ProgInfoListtemp.m_CAT_Sys_ID;
		if(ProgInfoListtemp.m_videoEcmPID == 0)
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = ProgInfoListtemp.m_EcmPID;
		}
		else
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = ProgInfoListtemp.m_videoEcmPID;
		}
		if(ProgInfoListtemp.m_audioEcmPID == 0)
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = ProgInfoListtemp.m_EcmPID;
		}
		else
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = ProgInfoListtemp.m_audioEcmPID;
		}
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(ProgInfoListtemp.m_ProgId);
		serviceInfo_cas_other = 0;
	}
	else
	{
		serviceInfo.m_wCaSysID = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(ProgInfoListtemp.m_ProgId);
		serviceInfo_cas_other = 1;
	}
	if(!(ProgInfoListtemp.m_CAT_Sys_ID))
	{
		serviceInfo_cas_other = 0;					
	}
	FYF_API_SetCurEcmInfo(&serviceInfo);
	FYF_API_semaphore_release (semaphoreEcm);

	FYF_API_SetProgramChange();

}
#endif

void APP_Com_IPI_PlayNvodChn(BU16 nvodServiceId)
{
	BU32 pcrPID 	= 0;
	BU32 video 		= 0;
	BU32 audio 		= 0;
	BU32 symbol 	= 0;
	BU32 modulation = 0;
	BU08 videotype  = 0;
	BU08 audiotype  = 0;
	BU32 i = 0;
	FYF_Ecm_Info_s serviceInfo;
	FYF_time_shifted_service_s shiftServiceInfo;
	BU32 tunerState = FYF_TUNER_STATUS_FAIL;
	BS32 ret = 0;
	BU32 novd_freq;

	FYF_API_mutex_lock(g_play_times_mutex, 0);
	if (g_play_times_remaind == 0)
	{
		while(1)
		{
			APP_PRINT_ERROR("play_times = %d\n", g_play_times_remaind);
		}
	}
	APP_PRINT_DEBUG("play_times = %d\n", g_play_times_remaind);
	if (--g_play_times_remaind > 0)
	{
		FYF_API_mutex_unlock(g_play_times_mutex);
		return;
	}
	FYF_API_mutex_unlock(g_play_times_mutex);

		
	printf(" APP_Com_IPI_PlayNvodChn---nvodServiceId = %d \n ",nvodServiceId);
	nvodPlayServiceId = nvodServiceId;
	APP_Com_API_ShowLED("nuod");
	APP_SYS_API_GetSysInfo(APP_SYS_NVOD_FREQ_e, &novd_freq);
	novd_freq = APP_NVOD_FREQ_MHZ;
	if (APP_FAIL == FYF_API_time_get_pid(nvodServiceId,&shiftServiceInfo))
	{
		APP_PRINT_ERROR("FYF_API_time_get_pid error nvodServiceId = %d\n", nvodServiceId);
		return;
	}

	pcrPID	= shiftServiceInfo.pcrpid;
	video 	= shiftServiceInfo.vpid;
	audio 	= shiftServiceInfo.apid;
	symbol 	= 6875;
	modulation = 2;	
	videotype  = FYF_VCODEC_TYPE_MPEG2;
	audiotype  = FYF_ACODEC_TYPE_MP3;

	if(novd_freq != APP_IPI_GetTunerFreq())
	{
		oldFreq = 0;
		FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
		APP_IPI_SaveTunerInfo(novd_freq, 6875, 2);
		FYF_API_tuner_lock_delivery(0, novd_freq * 1000, 6875, 2, novd_freq);
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
		tunerState = FYF_TUNER_STATUS_FAIL; 	

		for(i =0 ;i < 50 ;i++)
		{
			if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
			{
				tunerState = FYF_TUNER_STATUS_SUCCESS;
				break;
			}
		/*	
			if(APP_IPI_GetPlayFlag() == 1)
			{
				APP_IPI_SetPlayFlag(0);
				tunerState = FYF_TUNER_STATUS_FAIL;
				break;				
			}
			*/
			if (g_play_times_remaind > 0)
			{
				return;
			}
			FYF_API_thread_sleep(20);
		}
		
		printf("********PlayNvodChn tuner lock state = %d time = %dms********\n",tunerState,FYF_API_time_ms());
	}
	
//	APP_TDT_IPI_StartDemuxTdt();
	
	//APP_Cat_IPI_StopDemuxCat();
	APP_Cat_IPI_StartDemuxCat(APP_Cat_IPI_ParCatOnChangeFreq);

	APP_IPI_PlayProg(video, audio, pcrPID, videotype, audiotype);
	#if 1
	//APP_Cat_IPI_StartDemuxCat(APP_Cat_IPI_ParCatOnChangeFreq);
	FYF_API_semaphore_wait(semaphoreEcm, FYF_WAIT_FOREVER);	

	printf("sCAT_Sys_ID = 0x%x\n", shiftServiceInfo.sCAT_Sys_ID);
	ret = FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)&(shiftServiceInfo.sCAT_Sys_ID),0);	
	if(FYF_TRUE == ret)
	{
		serviceInfo.m_wCaSysID = shiftServiceInfo.sCAT_Sys_ID;
		if(shiftServiceInfo.videoEcmPid == 0)
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = shiftServiceInfo.ecmPID;
		}
		else
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = shiftServiceInfo.videoEcmPid;
		}
		if(shiftServiceInfo.audioEcmPid == 0)
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = shiftServiceInfo.ecmPID;
		}
		else
		{
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = shiftServiceInfo.audioEcmPid;
		}
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = shiftServiceInfo.service_id;
		serviceInfo_cas_other = 0;
	}
	else
	{
		serviceInfo.m_wCaSysID = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = 0;
		serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = 0;
		serviceInfo_cas_other = 1;
	}
	if(!(shiftServiceInfo.sCAT_Sys_ID))
	{
		serviceInfo_cas_other = 0;					
	}
	FYF_API_SetCurEcmInfo(&serviceInfo);
	FYF_API_semaphore_release (semaphoreEcm);
	#endif
}

#if 0
BU08 APP_Com_IPI_SearchPMT(BU16 serviceid)
{
	BU32 pmtTableNum = 0;
	BU32 pmtTableDemuxNum = 0;
	BU32 pmtPID = 0;
	BU32 currentDemuxPmtTable = 0;//当前已经打开的PMT通道数量
	FYF_QUEUE_MESSAGE msgRecv;
	BU32 i = 0;
	BU32 a, b;
	struct pmtPIDInfo_s pmtPidInfo;
	
	APP_PMT_IPI_ResetPmtQueue();

	pmtTableNum = DBA_API_GetPmtPidInfoCount();

	/*
	一次性打开多个通道滤取PMT
	当达到最大通道数量时等待一个通道滤取完毕,再打开下一个通道
	*/
	if (serviceid != 0)
	{
		for (currentDemuxPmtTable = 0 ; currentDemuxPmtTable < pmtTableNum && currentDemuxPmtTable < MAX_START_DEMUX_PMT; currentDemuxPmtTable++)
		{
			DBA_API_GetPmtPidInfo(currentDemuxPmtTable, &pmtPidInfo);
			a = SeparateSevId(pmtPidInfo.SID)  ;
			b = pmtPidInfo.PID;
			if (a == serviceid)
			{
				pmtPID = (a << 16)  | b;
				APP_PMT_IPI_StartDemuxTimeShiftPmt(pmtPID);
				APP_PRINT_DEBUG("currentDemuxPmtTable = %d, serviceid = %d\n", currentDemuxPmtTable, serviceid);
				break;
			}
		}
	}
	else
	{
		for (currentDemuxPmtTable = pmtTableNum - 1 ; currentDemuxPmtTable < pmtTableNum && currentDemuxPmtTable < MAX_START_DEMUX_PMT; currentDemuxPmtTable++)
		{
			DBA_API_GetPmtPidInfo(currentDemuxPmtTable, &pmtPidInfo);
			a = SeparateSevId(pmtPidInfo.SID)  ;
			b = pmtPidInfo.PID;
			pmtPID = (a << 16)  | b;
			APP_PMT_IPI_StartDemuxTimeShiftPmt(pmtPID);
			APP_PRINT_DEBUG("currentDemuxPmtTable = %d, serviceid = %d\n", currentDemuxPmtTable, serviceid);
		
		}
		
	}

	msgRecv.qlstWordOfMsg = 0;
	FYF_API_queue_recv(APP_PMT_IPI_GetQueueParserPmtEnd(), &msgRecv, TIME_WAIT_FOR_PMT);
	if (msgRecv.qlstWordOfMsg == 0)
	{
		APP_Search_printf("Parser Pmt======================================================ERROR!\n");
		APP_PMT_IPI_StopDemuxTimeShiftPmt();
		return 0;
	}
	return 1;
}
#endif
BU08 APP_Com_IPI_SearchPMTVod(BU16 serviceid)
{
	BU32 pmtTableNum = 0;
	BU32 pmtTableDemuxNum = 0;
	BU32 pmtPID = 0;
	BU32 currentDemuxPmtTable = 0;//当前已经打开的PMT通道数量
	FYF_QUEUE_MESSAGE msgRecv;
	BU32 i = 0;
	BU32 a, b;
	struct pmtPIDInfo_s pmtPidInfo;
	
	APP_PMT_IPI_ResetPmtQueue();

	pmtTableNum = DBA_API_GetPmtPidInfoCount();
	APP_PRINT_DEBUG("pmtTableNum = %d\n", pmtTableNum);

	/*
	一次性打开多个通道滤取PMT
	当达到最大通道数量时等待一个通道滤取完毕,再打开下一个通道
	*/
	if (serviceid != 0)
	{
		for (currentDemuxPmtTable = 0 ; currentDemuxPmtTable < pmtTableNum && currentDemuxPmtTable < MAX_START_DEMUX_PMT; currentDemuxPmtTable++)
		{
			DBA_API_GetPmtPidInfo(currentDemuxPmtTable, &pmtPidInfo);
			a = SeparateSevId(pmtPidInfo.SID)  ;
			b = pmtPidInfo.PID;
			if (a == serviceid)
			{
				pmtPID = (a << 16)  | b;
				APP_PMT_IPI_StartDemuxTimeShiftPmt(pmtPID);
				APP_PRINT_DEBUG("currentDemuxPmtTable = %d, serviceid = %d\n", currentDemuxPmtTable, serviceid);
				break;
			}
		}
	}
	else
	{
		for (currentDemuxPmtTable = pmtTableNum - 1 ; currentDemuxPmtTable < pmtTableNum && currentDemuxPmtTable < MAX_START_DEMUX_PMT; currentDemuxPmtTable++)
		{
			DBA_API_GetPmtPidInfo(currentDemuxPmtTable, &pmtPidInfo);
			a = SeparateSevId(pmtPidInfo.SID)  ;
			b = pmtPidInfo.PID;
			pmtPID = (a << 16)  | b;
			APP_PMT_IPI_StartDemuxTimeShiftPmt(pmtPID);
			APP_PRINT_DEBUG("currentDemuxPmtTable = %d, serviceid = %d\n", currentDemuxPmtTable, serviceid);
		
		}
		
	}

	msgRecv.qlstWordOfMsg = 0;
	FYF_API_queue_recv(APP_PMT_IPI_GetQueueParserPmtEnd(), &msgRecv, TIME_WAIT_FOR_PMT);
	if (msgRecv.qlstWordOfMsg == 0)
	{
		APP_Search_printf("Parser Pmt======================================================ERROR!\n");
		APP_PMT_IPI_StopDemuxTimeShiftPmt();
		return 0;
	}
	return 1;
}
void APP_Com_IPI_PlayVod()
{

#ifdef USE_ENREACH
	BU32 pcrPID 	= 0;
	BU32 video 		= 0;
	BU32 audio 		= 0;
	BU32 freq 		= 0;
	BU32 symbol 	= 0;
	BU32 modulation = 0;
	BU08 videotype  = 0;
	BU08 audiotype  = 0;
	BU16 serviceid;
	BU08 audio_track = APP_CHN_TRACK_SINGLE_e;
	BU16 audio_volume;
	BU32 i = 0;
	BU32 tunerState = FYF_TUNER_STATUS_FAIL;
	BS32 ret = 0;
	EVTC_VOD cur_evtc_vod;
	FYF_QUEUE_MESSAGE msgRecv;

	FYF_API_mutex_lock(g_play_times_mutex, 0);
	if (g_play_times_remaind == 0)
	{
		while(1)
		{
			APP_PRINT_ERROR("play_times = %d\n", g_play_times_remaind);
		}
	}
	APP_PRINT_DEBUG("play_times = %d\n", g_play_times_remaind);
	if (--g_play_times_remaind > 0)
	{
		FYF_API_mutex_unlock(g_play_times_mutex);
		return;
	}
	FYF_API_mutex_unlock(g_play_times_mutex);
	
	
	if (APP_SUCCESS == APP_EnrichVod_GetCurVodInfo(&cur_evtc_vod))
	{
		pcrPID	= cur_evtc_vod.pcrpid;
		video 	= cur_evtc_vod.vpid;
		audio 	= cur_evtc_vod.apid;
		freq 	= cur_evtc_vod.freq / 1000;
		symbol 	= cur_evtc_vod.symbolrate;
		serviceid = cur_evtc_vod.serviceid;
		modulation = cur_evtc_vod.qam - 1;	
		videotype  = FYF_VCODEC_TYPE_MPEG2;
		audiotype  = FYF_ACODEC_TYPE_MP3;
		
	}
	
	if ((freq != APP_IPI_GetTunerFreq() && freq >=51 && freq <= 859) || (symbol != APP_IPI_GetTunerSym()) || (modulation != APP_IPI_GetTunerMod()))
	{

		oldFreq = 0;
		
		FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);	
		APP_IPI_SaveTunerInfo((BS32)freq, (BS32)symbol, (BS32)(modulation));
		FYF_API_tuner_lock_delivery(0, (BS32)(freq * 1000), (BS32)symbol, (BS32)(modulation), (BS32)freq);
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
		
		tunerState = FYF_TUNER_STATUS_FAIL; 	

		for(i =0 ;i < 40 ;i++)
		{
			if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
			{
				tunerState = FYF_TUNER_STATUS_SUCCESS;
				break;
			}
			FYF_API_thread_sleep(50);
		}
		
		APP_PRINT_DEBUG("********PlayChn tuner lock state = %d time = %dms********\n",tunerState,FYF_API_time_ms());

	}

	if (cur_evtc_vod.playmod == SERVICEID_MOD || serviceid != 0)
	{
		APP_PAT_IPI_StartDemuxEnrichVodPat(serviceid);
		FYF_API_queue_recv(APP_PAT_IPI_GetQueueParserPatEnd(), &msgRecv, TIME_WAIT_FOR_PAT);
		APP_Search_printf("---333---\n");
		if (msgRecv.qlstWordOfMsg == 0)
		{
			APP_Search_printf("Parser Pat======================================================ERROR %d ms!\n",FYF_API_time_ms());
			APP_PAT_IPI_StopDemuxPat();
		}
		else
		{
			if (APP_Com_IPI_SearchPMTVod(serviceid))
			{
				if (APP_SUCCESS == APP_EnrichVod_GetCurVodInfo(&cur_evtc_vod))
				{
					pcrPID	= cur_evtc_vod.pcrpid;
					video 	= cur_evtc_vod.vpid;
					audio 	= cur_evtc_vod.apid;
					freq 	= cur_evtc_vod.freq / 1000;
					symbol 	= cur_evtc_vod.symbolrate;
					modulation = cur_evtc_vod.qam - 1;	
					videotype  = FYF_VCODEC_TYPE_MPEG2;
					audiotype  = FYF_ACODEC_TYPE_MP3;
					APP_PRINT_DEBUG("pcrpid = %d %d %d\n", video, audio, pcrPID);
					
				}
			}
		}
	}
	else if (cur_evtc_vod.playmod == PMTPID_MOD)
	{
		
	}
	
	APP_IPI_PlayProg(video, audio, pcrPID, videotype, audiotype);
#endif

}
	
void APP_Com_IPI_PlayTimeShift()
{
#ifdef USE_ENREACH
	BU32 pcrPID 	= 0;
	BU32 video 		= 0;
	BU32 audio 		= 0;
	BU32 freq 		= 0;
	BU32 symbol 	= 0;
	BU32 modulation = 0;
	BU08 videotype  = 0;
	BU08 audiotype  = 0;
	BU16 serviceid;
	BU08 audio_track = APP_CHN_TRACK_SINGLE_e;
	BU16 audio_volume;
	BU32 i = 0;
	BU32 tunerState = FYF_TUNER_STATUS_FAIL;
	BS32 ret = 0;
	EVTC_TS cur_evtc_ts;
	FYF_QUEUE_MESSAGE msgRecv;

	FYF_API_mutex_lock(g_play_times_mutex, 0);
	if (g_play_times_remaind == 0)
	{
		while(1)
		{
			APP_PRINT_ERROR("play_times = %d\n", g_play_times_remaind);
		}
	}
	
	APP_PRINT_DEBUG("play_times = %d\n", g_play_times_remaind);
	if (--g_play_times_remaind > 0)
	{
		FYF_API_mutex_unlock(g_play_times_mutex);
		return;
	}
	FYF_API_mutex_unlock(g_play_times_mutex);
	
	if (APP_SUCCESS == APP_EnrichVod_GetCurTsInfo(&cur_evtc_ts))
	{
		pcrPID	= cur_evtc_ts.pcrpid;
		video 	= cur_evtc_ts.vpid;
		audio 	= cur_evtc_ts.apid;
		freq 	= cur_evtc_ts.freq / 1000;
		symbol 	= cur_evtc_ts.symbolrate;
		serviceid = cur_evtc_ts.serviceid;
		modulation = cur_evtc_ts.qam - 1;	
		videotype  = FYF_VCODEC_TYPE_MPEG2;
		audiotype  = FYF_ACODEC_TYPE_MP3;
		
	}
	
	if ((freq != APP_IPI_GetTunerFreq() && freq >=51 && freq <= 859) || (symbol != APP_IPI_GetTunerSym()) || (modulation != APP_IPI_GetTunerMod()))
	{

		oldFreq = 0;
		
		FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);	
		APP_IPI_SaveTunerInfo((BS32)freq, (BS32)symbol, (BS32)(modulation));
		FYF_API_tuner_lock_delivery(0, (BS32)(freq * 1000), (BS32)symbol, (BS32)(modulation), (BS32)freq);
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
		
		tunerState = FYF_TUNER_STATUS_FAIL; 	

		for(i =0 ;i < 40 ;i++)
		{
			if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
			{
				tunerState = FYF_TUNER_STATUS_SUCCESS;
				break;
			}
			FYF_API_thread_sleep(50);
		}
		
		APP_PRINT_DEBUG("********PlayChn tuner lock state = %d time = %dms********\n",tunerState,FYF_API_time_ms());

	}

	if (cur_evtc_ts.playmod == SERVICEID_MOD || serviceid != 0)
	{
		APP_PAT_IPI_StartDemuxEnrichVodPat(serviceid);
		FYF_API_queue_recv(APP_PAT_IPI_GetQueueParserPatEnd(), &msgRecv, TIME_WAIT_FOR_PAT);
		APP_Search_printf("---333---\n");
		if (msgRecv.qlstWordOfMsg == 0)
		{
			APP_Search_printf("Parser Pat======================================================ERROR %d ms!\n",FYF_API_time_ms());
			APP_PAT_IPI_StopDemuxPat();
		}
		else
		{
			if (APP_Com_IPI_SearchPMTVod(serviceid))
			{
				if (APP_SUCCESS == APP_EnrichVod_GetCurTsInfo(&cur_evtc_ts))
				{
					pcrPID	= cur_evtc_ts.pcrpid;
					video 	= cur_evtc_ts.vpid;
					audio 	= cur_evtc_ts.apid;
					freq 	= cur_evtc_ts.freq / 1000;
					symbol 	= cur_evtc_ts.symbolrate;
					modulation = cur_evtc_ts.qam - 1;	
					videotype  = FYF_VCODEC_TYPE_MPEG2;
					audiotype  = FYF_ACODEC_TYPE_MP3;
					APP_PRINT_DEBUG("pcrpid = %d %d %d\n", video, audio, pcrPID);
					
				}
			}
		}
	}
	else if (cur_evtc_ts.playmod == PMTPID_MOD)
	{
		
	}
	
	APP_IPI_PlayProg(video, audio, pcrPID, videotype, audiotype);
#endif

}
BU32 FYF_DVT_GetCasOther(void)
{
	return serviceInfo_cas_other;
}

/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
BU32 APP_Com_IPI_StopProg(BU32 isNeedBlkScrn)
{
	BU32 bitmap = 0;

	APP_IPI_StopAudioVideo(isNeedBlkScrn);

	if(isNeedBlkScrn)//黑屏操作，前提是停止节目才有效
	{
		g_ScrnBlkState = (BU32)FYF_DISABLE;
		FYF_API_printf("black scream\n");
	//	FYF_API_vout_ioctl(videoOutput,FYF_VOUT_SET_VISABLE,&g_ScrnBlkState);
		FYF_API_vdec_ioctl(videoDecoderID, FYF_VDEC_BITMAP, &bitmap);
	}
	else
	{
		g_ScrnBlkState = (BU32)FYF_ENABLE;
	//	FYF_API_vout_ioctl(videoOutput,FYF_VOUT_SET_VISABLE,&g_ScrnBlkState);
		
	}
	APP_IPI_SetPlayState(APP_IPI_PLAY_STATE_NULL);
	return APP_SUCCESS;
}
/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_IPI_ShowBmp(BU32	bmp)
{
	BU32	bitmap = 0;
	bitmap = bmp;
	FYF_API_vdec_ioctl(videoDecoderID, FYF_VDEC_BITMAP, &bitmap);
	FYF_API_printf("APP_IPI_PlayProg bitmap1 = %d++++++++++++++++++++++\n", bitmap);
}

/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_IPI_StartVedioDecode(void)
{
	FYF_API_vdec_ioctl (videoDecoderID, FYF_VDEC_START, 0);
}
/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_IPI_StopVedioDecode(void)
{
	FYF_API_vdec_ioctl (videoDecoderID, FYF_VDEC_STOP, 0);
}

void APP_Com_IPI_SetVideoOutRect(BU32 x, BU32 y, BU32 width, BU32 height)
{
	g_rect.x = x;
	g_rect.y = y;
	g_rect.w = width;
	g_rect.h = height;
	FYF_API_vout_ioctl(videoOutput, FYF_VOUT_SET_WIN_LOCATION, &g_rect);
}

void APP_Com_IPI_SetEcmNull()
{
		FYF_Ecm_Info_s serviceInfo;
		memset(&serviceInfo, 0, sizeof(FYF_Ecm_Info_s));
		FYF_API_SetCurEcmInfo(&serviceInfo);
}

extern BU32 serviceInfo_cas_other;
/*-------------------------------------------------------------------------------

	Description: 

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_IPI_SignalDetect(void )
{
	BU32 signalState = APP_SIGNAL_LOCK;
	static BU32 lastSignalState = APP_SIGNAL_LOCK;
	static BU32 lastFreq = 0;
	BU32 vdec_state;
	BU32 bitmap;
	BU32 video_stream_input,audio_stream_input;
	APP_CHN_AV_TYPE_E curAvType;

	signalState = APP_Signal_API_GetSignalState();
	if(signalState == APP_SIGNAL_UNLOCK)
	{
		lastSignalState = signalState;
		if(SignalNotify != NULL)
		{
			(*SignalNotify)(0,APP_SIGNAL_UNLOCK);
		}
	}
	else
	{
		if(lastSignalState == APP_SIGNAL_UNLOCK)
		{
				lastSignalState = signalState;
				if(SignalNotify != NULL)
				{
					(*SignalNotify)(0,APP_SIGNAL_LOCK);
				}	
				return ;
		}
#if 0
		if(APP_IPI_GetPlayChnChange() == 1)
		{
			APP_IPI_SetPlayChnChange(0);
			lastSignalState = signalState;
			if(SignalNotify != NULL)
			{
				(*SignalNotify)(0,APP_SIGNAL_PLAYCHANGE);
			}	
			return ;			
		}
#endif
		FYF_API_vdec_ioctl (videoDecoderID, FYF_VDEC_STREAM_INPUT, &video_stream_input);
		FYF_API_adec_ioctl (g_AudioDecoderID, FYF_ADEC_STREAM_INPUT, &audio_stream_input);
//		APP_Printf("video_stream_input = %d, audio_stream_input = %d\n", video_stream_input, audio_stream_input);

		{
			curAvType = APP_ChnDatabase_API_GetCurChnType ();
			if(curAvType == APP_CHN_AV_TYPE_TV_e)
			{
				if(FYF_FALSE == video_stream_input)//YF_FALSE == audio_stream_input &&
				{
					if(APP_IPI_GetIsPlayStop() != 1)
					{
						lastSignalState = signalState;
						if(SignalNotify != NULL)
						{
							(*SignalNotify)(0,APP_SIGNAL_STOP);
						}
					}	
				}
				else if(FYF_TRUE == video_stream_input)
				{
					lastSignalState = signalState;
					if(SignalNotify != NULL)
					{
						(*SignalNotify)(0,APP_SIGNAL_PLAY);
					}
				}
			}
			else if(curAvType == APP_CHN_AV_TYPE_RD_e)
			{
				if(FYF_FALSE == audio_stream_input)
				{
					if(APP_IPI_GetIsPlayStop() != 1)
					{
						lastSignalState = signalState;
						if(SignalNotify != NULL)
						{
						//	(*SignalNotify)(0,APP_SIGNAL_STOP);
						}
					}		
				}
				else
				{
					lastSignalState = signalState;
					if(SignalNotify != NULL)
					{
					//	(*SignalNotify)(0,APP_SIGNAL_PLAY);
					}
				}
			}
		}
	}
}

#endif
#define APP_PROCESS_THREAD
#ifdef APP_PROCESS_THREAD
void ThreadPlayChn(void * param)
{
	FYF_QUEUE_MESSAGE msgRev;
	FYF_QUEUE_MESSAGE msgSend;
	while(1)
	{
		FYF_API_queue_recv(APP_IPI_GetQueuePlayChn(), &msgRev, FYF_WAIT_FOREVER);
		
		
		APP_PRINT_DEBUG("msgRev.qlstWordOfMsg = %d\n",msgRev.qlstWordOfMsg);
		switch(msgRev.qlstWordOfMsg)
		{
		case APP_Queue_PlayChn_e:
			APP_Com_IPI_PlayChn(((msgRev.q2ndWordOfMsg >> 16) & 0xffff), (msgRev.q2ndWordOfMsg & 0xffff), msgRev.q3rdWordOfMsg, msgRev.q4thWordOfMsg);
			break;
		case APP_Queue_StopPlayChn_e:
			APP_Com_IPI_StopProg((BU32)msgRev.q2ndWordOfMsg);
			break;
		case APP_Queue_LockTuner_e:
			APP_signal_IPI_LockTuner(msgRev);
			break;
		case APP_Queue_ShowBmp_e:
			APP_Com_IPI_ShowBmp((BU32)msgRev.q2ndWordOfMsg);
			break;
		case APP_Queue_StartVediodecode_e:
			APP_Com_IPI_StartVedioDecode();
			break;
		case APP_Queue_StopVediodecode_e:
			APP_Com_IPI_StopVedioDecode();
			break;
		case APP_Queue_SetVideoOutRect_e:
			break;
#ifdef ENABLE_JPEG
		case APP_Queue_ShowPhoto:
			MID_PictureDecoder_API_Start(msgRev.q2ndWordOfMsg);
			break;
#endif
#ifdef ENABLE_PVR
		case APP_Queue_PlayMp3:
			MID_Player_IPI_MP3Play(msgRev.q2ndWordOfMsg);
			break;
		case APP_Queue_StopMp3:
			MID_Player_IPI_MP3Stop();
			break;
		case APP_Queue_PauseMp3:
			MID_Player_IPI_MP3Pause();
			break;
		case APP_Queue_ResumeMp3:
			MID_Player_IPI_MP3Resume();
			break;
		case APP_Queue_SpeedMP3:
			MID_Player_IPI_MP3Speed();
			break;
		case APP_Queue_RewindMP3:
			MID_Player_IPI_MP3Rewind();
			break;
		case APP_Queue_PlayFilm:
			MID_Player_IPI_PVRPlay(msgRev.q2ndWordOfMsg);
			break;
		case APP_Queue_StopFilm:
			MID_Player_IPI_PVRStop();
			break;
		case APP_Queue_ResumeFilm:
			MID_Player_IPI_PVRResume();
			break;
		case APP_Queue_PauseFilm:
			MID_Player_IPI_PVRPause();
			break;
		case APP_Queue_AttachAV:
			FYF_API_av_attach();
			break;
		case APP_Queue_DetachAV:
			FYF_API_av_detach();
			break;
		case APP_Queue_StartRec:
			MID_PVR_API_StartRec(msgRev.q2ndWordOfMsg, msgRev.q3rdWordOfMsg);
			break;
		case APP_Queue_StopRec:
			MID_PVR_API_StopRec(msgRev.q2ndWordOfMsg);
			break;
		case APP_Queue_StartRePlay:
			MID_PVR_API_StartRePlay(msgRev.q2ndWordOfMsg);
			break;
		case APP_Queue_StopRePlay:
			MID_PVR_API_StopRePlay();
			break;
		case APP_Queue_ResumeRePlay:
			MID_PVR_API_ResumeRePlay();
			break;
		case APP_Queue_PauseRePlay:
			MID_PVR_API_PauseRePlay();
			break;
		case APP_Queue_SeekRePlay:
			MID_PVR_API_PlaySeek(msgRev.q2ndWordOfMsg, msgRev.q3rdWordOfMsg);
			break;
#endif
		case APP_Queue_PlayChn_NoPsiSi_e:
			APP_IPI_PlayProg_NoPsiSi((ChnDataBase_ChnInfo_NoPsiSi_s *)(msgRev.q2ndWordOfMsg));
			break;
		case APP_Queue_Ipannel_Start:
			msgSend.qlstWordOfMsg = 1;
			FYF_API_queue_send(APP_IPI_GetQueueIpanel(), &msgSend);
			break;
		case APP_Queue_PlayNvodChn_e:
			APP_Com_IPI_PlayNvodChn(msgRev.q2ndWordOfMsg);	
			break;
		case APP_Queue_SetEcmNull_e:
			APP_Com_IPI_SetEcmNull();
			break;
		case APP_Queue_PlayTimeShift:
			APP_Com_IPI_PlayTimeShift();
			break;
		case APP_Queue_PlayVod:
			APP_Com_IPI_PlayVod();
			break;
		default:
			break;
		}


	}
}
void APP_Com_API_SignalDetect(void)
{
	APP_Com_IPI_SignalDetect();
}
#if 1
BU32 signal_detect_status = FYF_TRUE;
void ThreadSignalDetect(void * param)
{
	FYF_API_thread_sleep(2000);
	while(1)
	{
		FYF_API_thread_sleep(200);
	//	APP_PRINT_DEBUG("signal_detect_status = %d\n", signal_detect_status);
		if(signal_detect_status == FYF_TRUE)
		{
			APP_Com_IPI_SignalDetect();
		}
	}
}
void APP_Com_API_Start_Signal_Dectect(void)
{
	signal_detect_status = FYF_TRUE;
}
void APP_Com_API_Stop_Signal_Dectect(void)
{
	signal_detect_status = FYF_FALSE;
}
BU32 APP_Com_API_GetSignal_Dectect(void)
{
	return signal_detect_status;
}

#endif
#endif


#define FYF_COM_FUNTION
#ifdef FYF_COM_FUNTION

/*-------------------------------------------------------------------------------

	Description:播放节目的通用接口，包括播放电视，广播，nvod，以及数据广播中音频的播放

	Parameters:

-------------------------------------------------------------------------------*/

BU32 APP_Com_API_PlayChn(APP_CHN_AV_TYPE_E AvType, APP_CHN_GROUP_TYPE_E GroupType, BU16 u16ChnNO, BS32 s32ServiceID)
{
	
	static FYF_QUEUE_MESSAGE msgSend;
	APP_Printf("APP_Com_API_PlayChn  send program %d  to play \n",u16ChnNO);

	APP_IPI_SetIsPlayStop(0);
	
	FYF_API_mutex_lock(g_play_times_mutex, 0);
	g_play_times_remaind++;
	FYF_API_mutex_unlock(g_play_times_mutex);

	if (s32ServiceID != -1 && nvodPlayServiceId != s32ServiceID)
	{
		nvodPlayServiceId = s32ServiceID;
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_SIGNAL_PLAYCHANGE, 0, 0);
		
	}

	msgSend.qlstWordOfMsg = (unsigned int)APP_Queue_PlayChn_e;
	msgSend.q2ndWordOfMsg = (AvType << 16) | GroupType;
	msgSend.q3rdWordOfMsg = u16ChnNO;
	msgSend.q4thWordOfMsg = s32ServiceID;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	
	return APP_SUCCESS;
	
}
#if 0
/*-------------------------------------------------------------------------------

	Description:开始播放节目

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Com_API_PlayChn(BU16 chnNO)
{
	static FYF_QUEUE_MESSAGE msgSend;

	APP_Printf("APP_Com_API_PlayChn  send program %d  to play \n",chnNO);
	APP_IPI_SetIsPlayStop(0);
	
	FYF_API_mutex_lock(g_play_times_mutex, 0);
	g_play_times_remaind++;
	FYF_API_mutex_unlock(g_play_times_mutex);

	//APP_IPI_SetPlayFlag(1);

	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PlayChn_e;
	msgSend.q2ndWordOfMsg= chnNO;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);

	return APP_SUCCESS;
}
#endif


void APP_Com_API_SetEcmNULL()
{
	static FYF_QUEUE_MESSAGE msgSend;
	msgSend.qlstWordOfMsg = (unsigned int)APP_Queue_SetEcmNull_e;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
}

/*-------------------------------------------------------------------------------

	Description:开始播放Nvod 节目

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Com_API_PlayNvodChn(BU16 nvodServiceId)
{
	static FYF_QUEUE_MESSAGE msgNvodSend;

	APP_IPI_SetIsPlayStop(0);
	
	FYF_API_mutex_lock(g_play_times_mutex, 0);
	g_play_times_remaind++;
	FYF_API_mutex_unlock(g_play_times_mutex);

	printf("APP_Com_API_PlayNvodChn--nvodServiceId = %d\n",nvodServiceId);

	msgNvodSend.qlstWordOfMsg= (unsigned int)APP_Queue_PlayNvodChn_e;
	msgNvodSend.q2ndWordOfMsg= nvodServiceId;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgNvodSend);
}

/*-------------------------------------------------------------------------------

	Description:显示广播logo 

	Paramenters: 只有在广播状态下调用才有意义

-------------------------------------------------------------------------------*/

void APP_Com_API_ShowRadioLogo()
{
	FYF_QUEUE_MESSAGE msgSend;
	g_u8bShowLogoFlag = 1;
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_ShowBmp_e;
	msgSend.q2ndWordOfMsg= 1;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);	
}

BU32 APP_Com_API_GetShowLogoFlag()
{
	return g_u8bShowLogoFlag;
}

/*-------------------------------------------------------------------------------

	Description:显示logo 

	Paramenters: 0为黑屏，1为radio，2为无信号

-------------------------------------------------------------------------------*/
void APP_Com_API_ShowBmp(BU32 bmp)
{
	APP_PRINT_INFO("APP_Com_API_ShowBmp bmp = %d\n", bmp);
	static FYF_QUEUE_MESSAGE msgSend;
	BU08 IsStopPlayFlag = 0;

	g_u8bShowLogoFlag = bmp;

	IsStopPlayFlag = 0;
	if(APP_IPI_GetIsPlayStop() == 0 )
	{
		APP_Com_API_StopProg (0);//黑屏之前,停止节目播放
		IsStopPlayFlag = 1;
	}

	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_ShowBmp_e;
	msgSend.q2ndWordOfMsg= bmp;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);	

	if(IsStopPlayFlag)
	{
		if(1 == nvodPlayFlag)
		{
			APP_Com_API_PlayChn(APP_CHN_AV_TYPE_NVOD_REFERENCE_SERVICE, APP_CHN_GROUP_TYPE_ALL_e, 0xffff, nvodPlayServiceId);
		}
		else
		{
			APP_Com_API_PlayChn (APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO (), -1);
		}

	}
}

void APP_Com_API_ShowOtherBmp(BU32 bmp)
{
	static FYF_QUEUE_MESSAGE msgSend;

	g_u8bShowLogoFlag = bmp;
	if(APP_IPI_GetIsPlayStop() == 0 )
	{
		APP_Com_API_StopProg (0);//黑屏之前,停止节目播放
	}

	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_ShowBmp_e;
	msgSend.q2ndWordOfMsg= bmp;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
}

/*-------------------------------------------------------------------------------

	Description:  启动视频解码器

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_API_StartVedioDecode(void)
{
	static FYF_QUEUE_MESSAGE msgSend;

	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StartVediodecode_e;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);

}
/*-------------------------------------------------------------------------------

	Description: 停止视频解码器

	Paramenters: 

-------------------------------------------------------------------------------*/
void APP_Com_API_StopVedioDecode(void)
{
	static FYF_QUEUE_MESSAGE msgSend;

	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StopVediodecode_e;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);

}

/*-------------------------------------------------------------------------------

	Description:停止播放节目

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Com_API_StopProg(BU32 isNeedBlkScrn)
{
	static FYF_QUEUE_MESSAGE msgSend;
	if (isNeedBlkScrn)
	{
		g_u8bShowLogoFlag = 0;
		if (UI_APP_API_bShowNoSignal())
		{
			UI_APP_API_HideNoSignal();
		}
	}


	APP_IPI_SetIsPlayStop(1);

	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_StopPlayChn_e;
	msgSend.q2ndWordOfMsg= (unsigned int)isNeedBlkScrn;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:设置小视频框

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Com_API_SetSmallScreen(BU32 x,BU32 y,BU16 w,BU32 h)
{
	APP_IPI_SetVideoOutRect(x, y, w, h);
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:恢复视频为全视频

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Com_API_SetFullScreen(void)
{
	APP_IPI_SetVideoOutRect(0, 0, 720, 576);
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:显示前面板LED
	参数：str[4] 要显示的4个字符

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Com_API_ShowLED(BU08 str[4])
{
	if (str[0] == led[0] && str[1] == led[1] && str[2] == led[2] && str[3] == led[3])
		return;
	memcpy(led, str, 4);
	FYF_API_panel_show_led( 0, str, 4);
	return APP_SUCCESS;
}



#ifdef _HAISI_DEMO_

BU08 UCharDis[] = {
        0xFF, 0xFF, 0x63, 0xFF, 0x61,
        0x71, 0xFF, 0x91, 0xFF, 0xFF,
        0xFF, 0xE3, 0xFF, 0xFF, 0x03,
        0x31, 0xFF, 0x11,/*R*/ 0x49, 0xFF,
        0x83, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };

BU08 LCharDis[] = {
        0xFF, 0xC1, 0xE5, 0x85, 0x21,
        0xFF, 0x09, 0xD1, 0xDF, 0xFF,
        0xFF, 0x9F, 0xFF, 0xD5, 0xC5,
        0x31, 0x19, 0xFF, 0x49, 0xE1/*t*/,
        0xC7, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };

#else

BU08 UCharDis[] = {
        0xFF, 0xFF, 0x39, 0xFF, 0x79,
        0x71, 0xFF, 0x76, 0xFF, 0xFF,
        0xFF, 0x38, 0xFF, 0xFF, 0x3f,
        0x73, 0xFF, 0xFF, 0x6d, 0xFF,
        0x3e, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };

    //大写字母只能够显示'C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U'，从左至右分别对应此数组中的非0xFF值
BU08 LCharDis[] = {
        0xFF, 0x7c, 0x58, 0x5e, 0x79,
        0xFF, 0x6f, 0x74, 0x30, 0xFF,
        0xFF, 0x38, 0xFF, 0x54, 0x5c,
        0x73, 0x67, 0xFF, 0x6d, 0x78,
        0x1c, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };
#endif    

	
BU32 APP_Com_API_LedDetect()
{
	BU32 i, j;
	BU08 str[4];
	for(i = 0; i < 10; i++)
	{
		for (j = 0; j < 4; j++)
		{
			str[j] = i + 48;
		}
		FYF_API_panel_show_led(0, str, 4);
		FYF_API_thread_sleep(500);
	}

	for (i = 0; i < 26; i++)
	{
		if (LCharDis[i] == 0xff)continue;
		for (j = 0; j < 4; j++)
		{
			str[j] = i + 97;
		}
		FYF_API_panel_show_led(0, str, 4);
		FYF_API_thread_sleep(500);
	}

	for (i = 0; i < 26; i++)
	{
		if (UCharDis[i] == 0xff)continue;
		for (j = 0; j < 4; j++)
		{
			str[j] = i + 65;
		}
		FYF_API_panel_show_led(0, str, 4);
		FYF_API_thread_sleep(500);
	}
	FYF_API_panel_show_led(0, led, 4);
}
/*-------------------------------------------------------------------------------

	Description: 设置信号检测的回调函数

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_SetSignalNotifyFun(SIGNALNOTIFY Notify )
{
	if(Notify != NULL)
	{
		SignalNotify = Notify;
	}

}
/*-------------------------------------------------------------------------------

	Description: 将CodeDate转换为年月日周

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_CodeDateToNormalDate(BU32 iCodedDate,BU16 *psYear, BU08 *pcMonth, BU08 *pcDate, BU08 *pWeekDay)
{
	int y1, m1, d, wd, k, y, m, test1, test2;

	/*计算并显示码流中提取出来的时间信息 */
	y1 = (int)((iCodedDate - 15078.2) / 365.25) / 1;
	test1 = (int)(y1 * 365.25 )/ 1;
	m1 =(int) ((iCodedDate - 14956.1 - test1) / 30.6001) / 1;
	test2 =(int)( m1 * 30.6001) / 1;
	d = iCodedDate - 14956 - test1 - test2;

	if (m1 == 14 || m1 == 15)
	{
		k = 1;
	}
	else
	{
		k = 0;
	}
	y = y1 + k;
	m = m1 - 1 - k * 12;
	wd = (iCodedDate + 2) % 7 + 1;

	*psYear = y + 1900;
	*pcMonth = m;
	*pcDate = d;
	*pWeekDay = wd;
	
}

/*-------------------------------------------------------------------------------

	Description: 将年月日转换为CodeDate


	Parameters:

-------------------------------------------------------------------------------*/
void APP_Com_API_NormalDateToCodeDate(BU16 sYear, BU08 cMonth, BU08 cDate, BU32 *piCodedDate)
{
	int test1 = 0;
	int test2 = 0;
	int temp = 0;

	if (cMonth == 1 || cMonth == 2)
	{
		temp = 1;
	}
	else
	{
		temp = 0;
	}

	test1 = (int)(((sYear - 1900 - temp) * 365.25) / 1);
	test2 = (int)((cMonth + 1 + temp * 12) * 30.6001) / 1;

	*piCodedDate = 14956 + cDate + test1 + test2;
}


void APP_API_PlayProg_NoPsiSi(ChnDataBase_ChnInfo_NoPsiSi_s* pChnInfo)
{
	static FYF_QUEUE_MESSAGE msgSend;

	msgSend.qlstWordOfMsg = APP_Queue_PlayChn_NoPsiSi_e;
	msgSend.q2ndWordOfMsg = (BU32)pChnInfo;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
}

#endif







