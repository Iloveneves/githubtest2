#include "app_common.h"
#include "app_ipi_common.h"

//#define USE_NEW_PMT_FUNC

static BU32 queueParserPmtEnd = 0;

extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];
extern APP_Search_Param_S app_Search_Param_S;
extern BU16 g_CurPlayChn;//但前正在播放的节目号
extern BU32 searchProgCount;
extern BU08 isStopSearch;

static BU32 semaphoreDemuxPmt = 0;
PMT_Channel_s pmtChannels[MAX_DEMUX_PMT_CHANNEL];


BU32	 pmtChannelID = 0;
BU32     pmtFilterID = 0;
static BU32 g_search_nvod_num = 0;

void APP_PMT_IPI_StartParserPmtPcrEcm(BU08 *section, BU32 channelId, BU32 filterId);
//void APP_IPI_PlayProg(BU32 videoPID, BU32 audioPID, BU32 pcrPID);



#define	SERVICE_DESC					0x48/*zsy:1220*/

#define TYPE_OF_NONE_SERVICE			0	/* 节目服务类型常量 */
#define TYPE_OF_TV_RADIO_SERVICE		0x30
#define TYPE_OF_BAT_SERVICE				0x31
#define TYPE_OF_TV_SERVICE				0x01
#define TYPE_OF_RADIO_SERVICE			0x02
#define TYPE_OF_NVOD_REFERENCE_SERVICE	0x04
#define TYPE_OF_NVOD_TIMESHIFT_SERVICE	0x05
#define TYPE_OF_MOSAIC_SERVICE			0x06
#define TYPE_OF_BROADCAST_SERVICE		0x0c
#define TYPE_OF_BROADCAST_OTHER_SERVICE	0x0f/*zsy:0114*/
#define TYPE_OF_STOCK_SERVICE			0x0a


void APP_PMT_IPI_ParserPmt(BU08 * section, void (*SavePmtInfo)(PMT_Content_s *))
{
	PMT_Content_s pmtContent;
	BU16 sectionLen,progNum,progInfoLen,esInfoLen,i,elementaryPID,j;
	BU32 dbNum;
	BU08 descLen = 0,streamType, audio_type = FYF_ACODEC_TYPE_MP3, video_type = FYF_VCODEC_TYPE_MPEG2;
	BU08 *sectionPos;
	BU08 *sectionPos2;
	BU32 pmtContentCount = 0;
	APP_CHN_AV_TYPE_E avType = APP_CHN_AV_TYPE_RD_e;

	BU32 progID = 0;
	BU08 bCA = 0;
	BU16 PCRPID = 0,audioPID = 0,videoPID = 0;
	BU08 bProg = 0;

	BU16			sCAT_Sys_ID = 0;
	SHORT2BYTE		stTempData16;
	BU08 ecmType = 0;

	memset(&pmtContent, 0, sizeof(PMT_Content_s));

	progNum = (BU16)((section[3]<<8)|section[4]);//SID
	
	//progID = ((BU16)(__glb_SIE_SearchTuningParam.posFreqMHz)<<16)|progNum;
	progID = progNum;
	PCRPID = ((section[8]&0x1f)<<8)|section[9];

	sectionLen = SIE_Search_GetSetionLen(section);
	progInfoLen = ((section[10]&0x0f)<<8)|section[11];

	sectionPos = &section[12];

	for(i = 0;i < progInfoLen; i = i+descLen)
	{
		descLen = sectionPos[1];
		if(0x09 == sectionPos[0])//CA_DESCRIPTOR
		{
			bCA = 1;

			sCAT_Sys_ID = (sectionPos[2] << 8) | sectionPos[3];
			if (1 == FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)&sCAT_Sys_ID,0))
			{
				pmtContent.sCAT_Sys_ID = (sectionPos[2] << 8) | sectionPos[3];
				pmtContent.ecmPID = ((sectionPos[4] & 0x1F)<< 8) | sectionPos[5];
			}

			SEARCH_DBG("sCAT_Sys_ID= 0x%x, ecmPID = %d\n", pmtContent.sCAT_Sys_ID, pmtContent.ecmPID);
		}

		sectionPos = sectionPos+2+descLen;
	}

	sectionPos = &section[12+progInfoLen];
	for(i = 12+progInfoLen;i<sectionLen-4+3;i=i+esInfoLen+5)
	{
		streamType = sectionPos[0];
		elementaryPID = ((sectionPos[1]&0x1f)<<8)|sectionPos[2];
		switch(streamType)
		{
		case SIE_MPEG1_AUDIO_STREAM:
		case SIE_MPEG2_AUDIO_STREAM:
		case SIE_AAC_ADTS_AUDIO_STREAM:
		case SIE_AAC_LATM_AUDIO_STREAM:
		case SIE_AC3_AUDIO_STREAM:
			bProg = 1;
			if(0 == audioPID)
			{
				audioPID = elementaryPID;
			}
			ecmType = 0;
			switch(streamType)
			{
			case SIE_MPEG1_AUDIO_STREAM:
			case SIE_MPEG2_AUDIO_STREAM:
				audio_type = FYF_ACODEC_TYPE_MP3;
				break;
			case SIE_AAC_ADTS_AUDIO_STREAM:
			case SIE_AAC_LATM_AUDIO_STREAM:
				audio_type = FYF_ACODEC_TYPE_AAC;
				break;
			case SIE_AC3_AUDIO_STREAM:
				audio_type = FYF_ACODEC_TYPE_AC3;
				break;
			}
			break;	
		case SIE_MPEG1_VIDEO_STREAM:
		case SIE_MPEG2_VIDEO_STREAM:
		case SIE_MPEG4_VIDEO_STREAM:
		case SIE_H264_VIDEO_STREAM:	
			bProg = 1;
			avType = APP_CHN_AV_TYPE_TV_e;
			videoPID = elementaryPID;
			ecmType = 1;
			switch (streamType)
			{
			case SIE_MPEG1_VIDEO_STREAM:
			case SIE_MPEG2_VIDEO_STREAM:
				video_type = FYF_VCODEC_TYPE_MPEG2;
				break;
			case SIE_MPEG4_VIDEO_STREAM:
				video_type = FYF_VCODEC_TYPE_MPEG4;
				break;
			case SIE_H264_VIDEO_STREAM:	
				video_type = FYF_VCODEC_TYPE_H264;
				break;
			}
			
			break;
		case SIE_CA_DESC:

			sCAT_Sys_ID = ((BU16)(sectionPos[ 2 ] << 8) )& 0xff00 ;
			sCAT_Sys_ID = sCAT_Sys_ID |sectionPos[ 3 ] ;
			
			stTempData16 . byte .ucByte1 =  sectionPos[ 4 ] &0x1f ; 
			stTempData16 . byte .ucByte0 = sectionPos[ 5 ];

			FYF_API_printf("sCAT_Sys_ID = %d+++++++++++++++++++++++++++++++++++++++++++++\n", sCAT_Sys_ID);
		
			break;
		case 0x0b:
			bProg = 1;
			avType = APP_CHN_AV_TYPE_BROADCAST_e;
			APP_Printf("====================progid = %d========================\n", progNum);
			break;
		default:
			break;

		}
		esInfoLen = ((sectionPos[3]&0x0f)<<8)|sectionPos[4];
		sectionPos2 = &sectionPos[5];
		for(j = 0;j < esInfoLen; j = j+descLen+2)
		{
			descLen = sectionPos2[1];
			if(0x09 == sectionPos2[0])//CA_DESCRIPTOR
			{
				bCA = 1;

				sCAT_Sys_ID = (sectionPos2[2] << 8) | sectionPos2[3];
				if (1 == FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)&sCAT_Sys_ID,0))
				{
					pmtContent.sCAT_Sys_ID = (sectionPos2[2] << 8) | sectionPos2[3];
					if(ecmType == 0)
						pmtContent.audioEcmPid= ((sectionPos2[4] & 0x1F)<< 8) | sectionPos2[5];
					else if(ecmType == 1)
						pmtContent.videoEcmPid= ((sectionPos2[4] & 0x1F)<< 8) | sectionPos2[5];
					SEARCH_DBG("ecmType = %d, PID = %d\n", ecmType, ((sectionPos2[4] & 0x1F)<< 8) | sectionPos2[5]);
				}
			}
			sectionPos2 = sectionPos2+2+descLen;
		}
		sectionPos = sectionPos+5+esInfoLen;
	}

	if (bProg)//&& (audioPID != 0 || videoPID != 0))//有节目
	{	
		pmtContent.audioPID = audioPID;
		pmtContent.avType = avType;
		pmtContent.bCA = bCA;
		pmtContent.progNum = progNum;
		pmtContent.videoPID = videoPID;
		pmtContent.PCRPID = PCRPID;
		pmtContent.audiotype = audio_type;
		pmtContent.videotype = video_type;


		(*SavePmtInfo)(&pmtContent);
		

	}
}
extern BU32 semaphoreEcm;
extern BU32 serviceInfo_cas_other;

void APP_PMT_IPI_SavePmtInfoPcrEcm(PMT_Content_s *pmtContents)
{
	BU32 pcrPid = 0;
	BU32 ecmPid = 0;
	BU16 sCatId = 0;
	BU32 videoPid = 0;
	BU32 audioPid = 0;
	BU32 audioEcmPid = 0;
	BU32 videoEcmPid = 0;
	BU32 progid = 0;
	FYF_Ecm_Info_s serviceInfo;
	ChnDataBase_ChnInfo_S chninfo;
	BS32 ret = 0;

	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chninfo))
	{
		return ;
	}
	/*
	else
	{
		progid = chninfo.m_ProgId;
		memset(&chninfo,0,sizeof(ChnDataBase_ChnInfo_S));
	}
	*/

	if (chninfo.m_ProgId != JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()))
	{
		return; 
	}

	pcrPid = chninfo.m_PcrPid;
	ecmPid = chninfo.m_EcmPID;
	sCatId = chninfo.m_CAT_Sys_ID;
	videoPid = chninfo.m_VideoPid;
	audioPid = chninfo.m_AudioPid;
	audioEcmPid = chninfo.m_audioEcmPID;
	videoEcmPid = chninfo.m_videoEcmPID;

	if(pcrPid != pmtContents->PCRPID
		|| ecmPid != pmtContents->ecmPID
		|| audioEcmPid != pmtContents->audioEcmPid
		|| videoEcmPid != pmtContents->videoEcmPid
		|| sCatId != pmtContents->sCAT_Sys_ID
		|| videoPid != pmtContents->videoPID
		|| audioPid != pmtContents->audioPID)
	{
#if 0
		if(videoPid != pmtContents->videoPID || audioPid != pmtContents->audioPID)
		{
			APP_PRINT_DEBUG("change av PID play prog\n");
			APP_PRINT_DEBUG("pmtContents->videoPID = %d, pmtContents->audioPID = %d, pmtContents->PCRPID = %d,pmtContents->videotype = %d, pmtContents->audiotype = %d\n", pmtContents->videoPID, pmtContents->audioPID, pmtContents->PCRPID,pmtContents->videotype, pmtContents->audiotype);
			APP_IPI_PlayProg(pmtContents->videoPID, pmtContents->audioPID, pmtContents->PCRPID,pmtContents->videotype, pmtContents->audiotype);
		}
		APP_PRINT_DEBUG("change av PID play prog!!!===============================================\n");
		FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
		ret = FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)(&sCatId),0);	
		if(FYF_TRUE == ret)
		{
			serviceInfo.m_wCaSysID = (unsigned short)sCatId;
			if(pmtContents->videoEcmPid == 0)
			{
				serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = pmtContents->ecmPID;
			}
			else
			{
				serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = pmtContents->videoEcmPid;
			}
			if(pmtContents->audioEcmPid == 0)
			{
				serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = pmtContents->ecmPID;
			}
			else
			{
				serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = pmtContents->audioEcmPid;
			}
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(pmtContents->progNum);
			serviceInfo_cas_other = 0;
		 }
	     else
		 {
			serviceInfo.m_wCaSysID = 0;
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = 0;
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = 0;
			serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(pmtContents->progNum);
			serviceInfo_cas_other = 1;
		 }
		 if(!sCatId)
		 {
			serviceInfo_cas_other = 0;					
	     }
		 FYF_API_SetCurEcmInfo(&serviceInfo);
		 FYF_API_semaphore_release (semaphoreEcm);
#endif
			APP_PRINT_DEBUG("change av PID play prog!!!===============================================\n");	
		 chninfo.m_PcrPid = pmtContents->PCRPID;
		 chninfo.m_EcmPID = pmtContents->ecmPID;
	     chninfo.m_audioEcmPID = pmtContents->audioEcmPid;
		 chninfo.m_videoEcmPID = pmtContents->videoEcmPid;
		 chninfo.m_CAT_Sys_ID = pmtContents->sCAT_Sys_ID;
		 chninfo.m_VideoPid = pmtContents->videoPID;
		 chninfo.m_AudioPid = pmtContents->audioPID;
		 
		 chninfo.m_AudioType = pmtContents->audiotype;
		 chninfo.m_VideoType = pmtContents->videotype;
		 SQL_API_Semaphore_Wait();
		 sprintf(g_StrSQL, "UPDATE program SET \
				PCRPID = %d, ecmPID = %d, audioEcmPID = %d, videoEcmPID = %d, sCAT_Sys_ID = %d, \
				视频PID = %d, 音频PID = %d, 视频类型 = %d, 音频类型 = %d\
				WHERE service_id = %d;",
				pmtContents->PCRPID, pmtContents->ecmPID, pmtContents->audioEcmPid, pmtContents->videoEcmPid, 
				pmtContents->sCAT_Sys_ID, pmtContents->videoPID, pmtContents->audioPID, pmtContents->videotype, pmtContents->audiotype,
				JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()));
		 SQL_API_Exec(g_StrSQL);
		 SQL_API_Semaphore_Release();
		// APP_ChnDatabase_API_SetChnInfo (app_system_data.g_CurChnNo, &chninfo);
		APP_ChnDatabase_API_SetChnInfoByServiceid(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),chninfo.m_ProgId, &chninfo);

		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_CHN_CHANGE, 0, 0);
		APP_ChnDatabase_API_UpDataToFlash(0);
	 }



	
	
	
#if 0
	SQL_API_Semaphore_Wait();
	sprintf(g_StrSQL, "SELECT PCRPID, ecmPID, sCAT_Sys_ID, 视频PID, 音频PID, \
			audioEcmPID, videoEcmPID FROM program WHERE service_id = %d;", 
			JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()));
	SQL_API_Select(g_StrSQL);
	if(SQL_API_GetTotalNum() != 0)
	{
		pcrPid = SQL_API_GetData(0,0,0,APP_DATA_TYPE_NUM);
		ecmPid = SQL_API_GetData(1,0,0,APP_DATA_TYPE_NUM);
		sCatId = (BU16)SQL_API_GetData(2,0,0,APP_DATA_TYPE_NUM);
		videoPid = SQL_API_GetData(3,0,0,APP_DATA_TYPE_NUM);
		audioPid = SQL_API_GetData(4,0,0,APP_DATA_TYPE_NUM);
		audioEcmPid = SQL_API_GetData(5,0,0,APP_DATA_TYPE_NUM);
		videoEcmPid = SQL_API_GetData(6,0,0,APP_DATA_TYPE_NUM);
		if(pcrPid != pmtContents->PCRPID
			|| ecmPid != pmtContents->ecmPID
			|| audioEcmPid != pmtContents->audioEcmPid
			|| videoEcmPid != pmtContents->videoEcmPid
			|| sCatId != pmtContents->sCAT_Sys_ID
			|| videoPid != pmtContents->videoPID
			|| audioPid != pmtContents->audioPID)
		{
			if( app_system_data.g_CurChnNo < g_ChnNum && pmtContents->progNum == SeparateSevId(progid))
			{
				if(videoPid != pmtContents->videoPID || audioPid != pmtContents->audioPID)
				{
					FYF_API_printf("change av PID play prog\n");
					APP_IPI_PlayProg(pmtContents->videoPID, pmtContents->audioPID, pmtContents->PCRPID);
				}
				FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
				ret = FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)(&sCatId),0);	
				if(FYF_TRUE == ret)
				{
					serviceInfo.m_wCaSysID = (unsigned short)sCatId;
					if(pmtContents->videoEcmPid == 0)
					{
						serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = pmtContents->ecmPID;
					}
					else
					{
						serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = pmtContents->videoEcmPid;
					}
					if(pmtContents->audioEcmPid == 0)
					{
						serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = pmtContents->ecmPID;
					}
					else
					{
						serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = pmtContents->audioEcmPid;
					}
					serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(pmtContents->progNum);
					serviceInfo_cas_other = 0;
				}
				else
				{
					serviceInfo.m_wCaSysID = 0;
					serviceInfo.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid = 0;
					serviceInfo.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid = 0;
					serviceInfo.ca_ecm.dvt_ca_ecm.m_wServiceID = SeparateSevId(pmtContents->progNum);
					serviceInfo_cas_other = 1;
				}
				if(!sCatId)
				{
					serviceInfo_cas_other = 0;					
				}
				FYF_API_SetCurEcmInfo(&serviceInfo);
				FYF_API_semaphore_release (semaphoreEcm);
			
				chninfo.m_PcrPid = pmtContents->PCRPID;
				chninfo.m_EcmPID = pmtContents->ecmPID;
				chninfo.m_audioEcmPID = pmtContents->audioEcmPid;
				chninfo.m_videoEcmPID = pmtContents->videoEcmPid;
				chninfo.m_CAT_Sys_ID = pmtContents->sCAT_Sys_ID;
				chninfo.m_VideoPid = pmtContents->videoPID;
				chninfo.m_AudioPid = pmtContents->audioPID;
			}

			sprintf(g_StrSQL, "UPDATE program SET \
				PCRPID = %d, ecmPID = %d, audioEcmPID = %d, videoEcmPID = %d, sCAT_Sys_ID = %d, \
				视频PID = %d, 音频PID = %d \
				WHERE service_id = %d;",
				pmtContents->PCRPID, pmtContents->ecmPID, pmtContents->audioEcmPid, pmtContents->videoEcmPid, 
				pmtContents->sCAT_Sys_ID, pmtContents->videoPID, pmtContents->audioPID,
				JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()));
			SQL_API_Exec(g_StrSQL);
			SQL_API_Semaphore_Release();
			APP_ChnDatabase_API_SetChnInfo (app_system_data.g_CurChnNo, &chninfo);
		}
		else
		{
			SQL_API_Semaphore_Release();
		}
	}
	else
	{
		SQL_API_Semaphore_Release();
	}
	#endif
}

/*******************************************
Desic:    设置和获取nvod时移业务数
********************************************/
BU32 APP_PMT_API_GetSearchNvodNum(void)
{
	return g_search_nvod_num;
}
void APP_PMT_API_SetSearchNvodNum(BU32 num)
{
	g_search_nvod_num = num;
}
void APP_PMT_IPI_SavePmtInfo(PMT_Content_s *pmtContents)
{
	int i,j;
	BU32 service_id_tmp;
	BU08 reduplicate_prog  = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	struct pmtPIDInfo_s pmtPidInfo;
	struct sdtInfo_s sdtInfo;
	FYF_time_shifted_service_s shiftServiceInfo;
	BU08 chnname[APP_CHN_NAME_LEN+10];
	if(APP_Search_API_GetStopState() == 1)
	{
		return;
	}
	
	SEARCH_DBG("Save pmt prognum = %d, videopid = %d, audiopid = %d, pcrpid = %d\n", pmtContents->progNum, pmtContents->videoPID, pmtContents->audioPID, pmtContents->PCRPID);
	DBA_API_GetSdtInfoBySerId(JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()), &sdtInfo);
	if(sdtInfo.serId != 0)
	{
		strcpy(pmtContents->m_ChnName, sdtInfo.progName);
		strcpy(chnInfo.m_ChnName, sdtInfo.progName);
		chnInfo.m_OriNetId = sdtInfo.OriNetId;

		if(sdtInfo.serType == APP_CHN_AV_TYPE_NVOD_TIMESHIFT_SERVICE 
			|| sdtInfo.serType == APP_CHN_AV_TYPE_NVOD_TIMESHIFT_SERVICE_DESC)//时移业务
		{
#ifdef ENABLE_NVOD
			APP_SYS_API_SetSysInfo(APP_SYS_NVOD_FREQ_e, APP_IPI_GetTunerFreq());
			//FYF_API_time_save_pid(pmtContents->progNum, pmtContents->videoPID, pmtContents->audioPID, pmtContents->PCRPID);
			shiftServiceInfo.service_id = pmtContents->progNum;
			shiftServiceInfo.vpid= pmtContents->videoPID;
			shiftServiceInfo.apid= pmtContents->audioPID;
			shiftServiceInfo.pcrpid= pmtContents->PCRPID;
			shiftServiceInfo.ecmPID= pmtContents->ecmPID;
			shiftServiceInfo.audioEcmPid= pmtContents->audioEcmPid;
			shiftServiceInfo.videoEcmPid= pmtContents->videoEcmPid;
			shiftServiceInfo.sCAT_Sys_ID= pmtContents->sCAT_Sys_ID;
			FYF_API_time_save_pid(&shiftServiceInfo);
			g_search_nvod_num++;
#endif
			return;
		}

 	}
	else
	{
		if(pmtContents->avType != sdtInfo.serType)
		{
			if(pmtContents->avType != sdtInfo.serType)
			{
			if (APP_Search_API_GetSearchType() != APP_SEARCH_TYPE_AUTO_e)
			{
				APP_PRINT_ERROR("void APP_PMT_IPI_SavePmtInfo(PMT_Content_s *pmtContents)\n");
				//return;

				sprintf(chnInfo.m_ChnName, "未命名%d", pmtContents->progNum);
				sprintf(pmtContents->m_ChnName, "未命名%d", pmtContents->progNum);
				chnInfo.m_OriNetId = 0;
			}
			else
			{
				return;
			}
		}
		
		}

	}
	for(i =0,j=0;i<APP_CHN_NAME_LEN;i++,j++)
	{
		if((pmtContents->m_ChnName)[i] == 0)
		{
			chnname[j] = (pmtContents->m_ChnName)[i];
			break;
		}
		else if((pmtContents->m_ChnName)[i] == '\'')
		{
			chnname[j] = '\'';
			j++;
			chnname[j] = '\'';
		}
		else
		{
			chnname[j] = (pmtContents->m_ChnName)[i];
		}
	}
	chnInfo.m_ProgId = pmtContents->progNum;
	chnInfo.m_FreqMHz =APP_IPI_GetTunerFreq();
	chnInfo.m_SymKbps = APP_IPI_GetTunerSym();
	chnInfo.m_QAM = APP_IPI_GetTunerMod();
	chnInfo.m_ChnIndex = pmtContents->progNum;
	chnInfo.m_Fav   = 0;
	chnInfo.m_Lock  = 0;
	chnInfo.m_Ca    = pmtContents->bCA;
	chnInfo.m_Skip  = 0;
	chnInfo.m_Del   = 0;
	chnInfo.m_AudioChn = 0;
	chnInfo.m_AudioOffset = 0;
	chnInfo.m_ProgId   = pmtContents->progNum;
	chnInfo.m_VideoPid = pmtContents->videoPID;
	chnInfo.m_AudioPid = pmtContents->audioPID;
	chnInfo.m_PcrPid = pmtContents->PCRPID;
	chnInfo.m_style = pmtContents->avType;

	chnInfo.m_AudioType = pmtContents->audiotype;
	chnInfo.m_VideoType = pmtContents->videotype;

	if(/*strstr(pmtContents->m_ChnName, "高清") != 0 
		|| strstr(pmtContents->m_ChnName, "HD") != 0
		|| */((strstr(pmtContents->m_ChnName, "高清") == 0 && strstr(pmtContents->m_ChnName, "HD") == 0)&& pmtContents->videoPID != 0 && pmtContents->audioPID == 0))
	{
		return;
	}


	DBA_API_GetPmtPidInfoBySID(JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()), &pmtPidInfo);

	if(pmtPidInfo.TSID==0 && pmtPidInfo.PID==0)
	{
		return ;
	}
	chnInfo.m_pmtpid = pmtPidInfo.PID;
	chnInfo.m_TsId = pmtPidInfo.TSID;


	if (sdtInfo.serType == APP_CHN_AV_TYPE_BROADCAST_e)
	{
#ifndef NO_BROADCAST
		APP_PRINT_DEBUG("m_ProgId=%d, chnname = %s, chnInfo.m_FreqMHz = %d, chnInfo.m_QAM = %d, chnInfo.m_SymKbps = %d,\n chnInfo.m_style = %d, \
			chnInfo.m_VideoPid = %d, chnInfo.m_AudioPid = %d, chnInfo.m_PcrPid = %d, \
			chnInfo.m_TsId = %d, chnInfo.m_pmtpid = %d,chnInfo.m_OriNetId = %d\n", \
			chnInfo.m_ProgId, chnname,chnInfo.m_FreqMHz, chnInfo.m_QAM, chnInfo.m_SymKbps, chnInfo.m_style, \
			chnInfo.m_VideoPid, chnInfo.m_AudioPid, chnInfo.m_PcrPid, chnInfo.m_TsId, \
			chnInfo.m_pmtpid,chnInfo.m_OriNetId);
		SQL_API_Semaphore_Wait();
		sprintf(g_StrSQL, "SELECT service_id FROM broadcast WHERE service_id = %d;", chnInfo.m_ProgId);
		SQL_API_Select(g_StrSQL);
		if(SQL_API_GetTotalNum() == 0)
		{

			sprintf(g_StrSQL,"INSERT INTO broadcast \
			(service_id, 编号, 节目名, 声道, \
			 频率, 调制方式, 符号率, 服务类型, \
			视频PID, 音频PID, PCRPID, TSID, PID,OriNetId) \
			VALUES(%d, %d, '%s', 0, \
			%d, %d, %d, %d, %d, \
			%d, %d, %d, %d, %d);",
			chnInfo.m_ProgId, chnInfo.m_ProgId, chnname,
			chnInfo.m_FreqMHz, chnInfo.m_QAM, chnInfo.m_SymKbps, chnInfo.m_style,
			chnInfo.m_VideoPid, chnInfo.m_AudioPid, chnInfo.m_PcrPid, 
			chnInfo.m_TsId, chnInfo.m_pmtpid,chnInfo.m_OriNetId);

		}
		else
		{
			sprintf(g_StrSQL,"UPDATE broadcast SET \
			节目名 = '%s', 频率 = %d, 符号率 = %d, 调制方式 = %d, 视频PID = %d, \
			音频PID = %d, 服务类型 = %d, PCRPID = %d, \
			TSID = %d , PID = %d, OriNetId = %d\
			WHERE service_id = %d;",
			chnname, chnInfo.m_FreqMHz, chnInfo.m_SymKbps, chnInfo.m_QAM,
			chnInfo.m_VideoPid, chnInfo.m_AudioPid, chnInfo.m_style, chnInfo.m_PcrPid, 
			chnInfo.m_TsId, chnInfo.m_pmtpid,chnInfo.m_OriNetId, chnInfo.m_ProgId);
		}
		SQL_API_Exec(g_StrSQL);
		SQL_API_Semaphore_Release();
		if (app_Search_Param_S.m_Callback_AddAChn != 0)
		{
			app_Search_Param_S.m_Callback_AddAChn(&chnInfo, sdtInfo.serType);
		}
#endif
		//DBA_API_ADD_BroadCast(chnInfo);
		return;
	}


#if 1
	SQL_API_Semaphore_Wait();
//2010-10-28 cwd add 为了防止进行频点改动的情况
	sprintf(g_StrSQL, "SELECT service_id FROM program WHERE TSID = %d;", chnInfo.m_TsId);
	SQL_API_Select(g_StrSQL);
	for ( i = 0; i < SQL_API_GetTotalNum(); i++)
	{
		SQL_API_GotoRow(i);
		service_id_tmp = SQL_API_GetData((BU32)PROGRAM_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		if (pmtContents->progNum == SeparateSevId(service_id_tmp))
		{
			reduplicate_prog = 1;
			break;
		}
		
	}

	sprintf(g_StrSQL, "SELECT service_id FROM program WHERE service_id = %d;", JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()));
	SQL_API_Select(g_StrSQL);

	if(SQL_API_GetTotalNum() == 0 && reduplicate_prog == 0)
	{
		if(++searchProgCount >= APP_MAX_PROG)
		{
			APP_Search_API_HaltSearch();
		}

		sprintf(g_StrSQL,"INSERT INTO program \
(service_id, 编号, 节目名, 加密, 加锁, \
喜爱, 删除, 跳过, 移动, 声道, \
音量补偿, 频率, 调制方式, 符号率, 服务类型, \
视频PID, 音频PID, PCRPID, ecmPID, audioEcmPID, \
videoEcmPID, sCAT_Sys_ID, \
TSID, PID,OriNetId, 视频类型, 音频类型, 音量大小) \
VALUES(%d, %d, '%s', %d, 0, \
0, 0, 0, 0, 1, \
0, %d, %d, %d, %d, \
%d, %d, %d, %d, %d, \
%d, %d, \
%d, %d,%d,%d,%d,16);",
			JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()), JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()), chnname, pmtContents->bCA,
			APP_IPI_GetTunerFreq(), APP_IPI_GetTunerMod(), APP_IPI_GetTunerSym(), pmtContents->avType,
			pmtContents->videoPID, pmtContents->audioPID, pmtContents->PCRPID, pmtContents->ecmPID,
			pmtContents->audioEcmPid, pmtContents->videoEcmPid, pmtContents->sCAT_Sys_ID,
			chnInfo.m_TsId, chnInfo.m_pmtpid,chnInfo.m_OriNetId,chnInfo.m_VideoType,chnInfo.m_AudioType);
	}
	else if (SQL_API_GetTotalNum() == 0 && reduplicate_prog == 1)// 两个频点的节目互换的情况
	{
		APP_Printf("update update UPDATE program SET reduplicate_prog \n");
		sprintf(g_StrSQL, "UPDATE program SET \
service_id = %d,节目名 = '%s', 频率 = %d, 符号率 = %d, 调制方式 = %d, 视频PID = %d, \
音频PID = %d, 加密 = %d, 服务类型 = %d, PCRPID = %d, ecmPID = %d, \
audioEcmPID = %d, videoEcmPID = %d, sCAT_Sys_ID = %d, 跳过 = 0, 音量补偿 = 0, \
TSID = %d , PID = %d, OriNetId = %d, 视频类型 = %d, 音频类型 = %d  \
WHERE service_id = %d;",
			JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()), chnname, APP_IPI_GetTunerFreq(), APP_IPI_GetTunerSym() , APP_IPI_GetTunerMod(),	pmtContents->videoPID,
			pmtContents->audioPID, pmtContents->bCA, pmtContents->avType, pmtContents->PCRPID, pmtContents->ecmPID, 
			pmtContents->audioEcmPid, pmtContents->videoEcmPid, pmtContents->sCAT_Sys_ID, chnInfo.m_TsId, chnInfo.m_pmtpid, chnInfo.m_OriNetId,
			chnInfo.m_VideoType,chnInfo.m_AudioType,
			service_id_tmp);
	}
	else
	{
		APP_Printf("update update UPDATE program SET\n");
		sprintf(g_StrSQL, "UPDATE program SET \
节目名 = '%s', 频率 = %d, 符号率 = %d, 调制方式 = %d, 视频PID = %d, \
音频PID = %d, 加密 = %d, 服务类型 = %d, PCRPID = %d, ecmPID = %d, \
audioEcmPID = %d, videoEcmPID = %d, sCAT_Sys_ID = %d, 跳过 = 0, 音量补偿 = 0, \
TSID = %d , PID = %d, OriNetId = %d, 视频类型 = %d, 音频类型 = %d  \
WHERE service_id = %d;",
			chnname, APP_IPI_GetTunerFreq(), APP_IPI_GetTunerSym() , APP_IPI_GetTunerMod(),	pmtContents->videoPID,
			pmtContents->audioPID, pmtContents->bCA, pmtContents->avType, pmtContents->PCRPID, pmtContents->ecmPID, 
			pmtContents->audioEcmPid, pmtContents->videoEcmPid, pmtContents->sCAT_Sys_ID, chnInfo.m_TsId, chnInfo.m_pmtpid, chnInfo.m_OriNetId,
			chnInfo.m_VideoType,chnInfo.m_AudioType,
			JoinSevIdFreq(pmtContents->progNum, APP_IPI_GetTunerFreq()));
	}
	SQL_API_Exec(g_StrSQL);

	SQL_API_Semaphore_Release();
#endif
	
	if (app_Search_Param_S.m_Callback_AddAChn != 0)
	{
		app_Search_Param_S.m_Callback_AddAChn(&chnInfo, pmtContents->avType);
	}
}

void APP_PMT_IPI_StartDemuxPmt(BU32 pmtPID)
{
	BU32 i = 0;
	BU32 returnValue = 0;
	BU08 pmtCoef[1] = {0x02};
	BU08 pmtMask[1] = {0xFF};
	BU08 pmtExcl[1] = {0x00};
	struct ParserFuncNode_s parFuncNode;

	if(semaphoreDemuxPmt == 0)
		{semaphoreDemuxPmt = FYF_API_semaphore_create("sDemPmt", 1, FYF_THREAD_WAIT_FIFO);}
	FYF_API_semaphore_wait(semaphoreDemuxPmt, FYF_WAIT_FOREVER);
	for (i = 0; i < MAX_DEMUX_PMT_CHANNEL; i++)
	{
		if (pmtChannels[i].pmtPID == 0)
		{
			pmtChannels[i].pmtPID = pmtPID;
			if(pmtChannels[i].pmtChannelID != 0 || pmtChannels[i].pmtFilterID != 0)
				APP_PMT_IPI_StopDemuxPmt(i);
			APP_Parser_IPI_StartDemux(1, pmtPID & 0xffff, pmtCoef, pmtMask, pmtExcl, &(pmtChannels[i].pmtChannelID), &(pmtChannels[i].pmtFilterID));
			pmtChannels[i].isStop = 0;
			parFuncNode.channelId = pmtChannels[i].pmtChannelID;
			parFuncNode.filterId = pmtChannels[i].pmtFilterID;
			parFuncNode.ParserFunc = APP_PMT_IPI_StartParserPmt;
			APP_IPI_AddPasFuncNode(parFuncNode);
			break;
		}
	}
	FYF_API_semaphore_release(semaphoreDemuxPmt);
}

BU32 APP_PMT_IPI_GetQueueParserPmtEnd(void)
{
	return queueParserPmtEnd;
}



static BU32 pmtTimeShiftChannelId = 0;
static BU32 pmtTimeShiftFilterId = 0;

extern BU16 g_enrichvod_serviceid;

void APP_PMT_IPI_SaveTimeShiftPmtInfo(PMT_Content_s *pmtContents)
{
#ifdef USE_ENREACH
	APP_EnrichVod_UpdateCurVodInfo(pmtContents->videoPID, pmtContents->audioPID, pmtContents->PCRPID);
#endif
}
void APP_PMT_IPI_StopDemuxTimeShiftPmt(void)
{
	APP_Parser_IPI_StopDemux(&pmtTimeShiftChannelId, &pmtTimeShiftFilterId);
}

BU08 APP_PMT_IPI_ParserPmtTimeShift(BU08 * section, void (*SavePmtInfo)(PMT_Content_s *))
{
	PMT_Content_s pmtContent;
	BU16 sectionLen,progNum,progInfoLen,esInfoLen,i,elementaryPID,j;
	BU32 dbNum;
	BU08 descLen = 0,streamType, audio_type = FYF_ACODEC_TYPE_MP3, video_type = FYF_VCODEC_TYPE_MPEG2;
	BU08 *sectionPos;
	BU08 *sectionPos2;
	BU32 pmtContentCount = 0;
	APP_CHN_AV_TYPE_E avType = APP_CHN_AV_TYPE_RD_e;

	BU32 progID = 0;
	BU08 bCA = 0;
	BU16 PCRPID = 0,audioPID = 0,videoPID = 0;
	BU08 bProg = 0;

	BU16			sCAT_Sys_ID = 0;
	SHORT2BYTE		stTempData16;
	BU08 ecmType = 0;

	memset(&pmtContent, 0, sizeof(PMT_Content_s));

	progNum = (BU16)((section[3]<<8)|section[4]);//SID
	
	//progID = ((BU16)(__glb_SIE_SearchTuningParam.posFreqMHz)<<16)|progNum;
	progID = progNum;
	PCRPID = ((section[8]&0x1f)<<8)|section[9];

	if (progNum != g_enrichvod_serviceid)
	{
		return 0;
	}

	sectionLen = SIE_Search_GetSetionLen(section);
	progInfoLen = ((section[10]&0x0f)<<8)|section[11];

	sectionPos = &section[12];

	for(i = 0;i < progInfoLen; i = i+descLen)
	{
		descLen = sectionPos[1];
		if(0x09 == sectionPos[0])//CA_DESCRIPTOR
		{
			bCA = 1;

			if(descLen == 4)
			{
				sCAT_Sys_ID = (sectionPos[2] << 8) | sectionPos[3];
				if (1 == FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)&sCAT_Sys_ID,0))
				{
					pmtContent.sCAT_Sys_ID = (sectionPos[2] << 8) | sectionPos[3];
					pmtContent.ecmPID = ((sectionPos[4] & 0x1F)<< 8) | sectionPos[5];
				}
			}
			else
			{
				pmtContent.ecmPID = 0;
				pmtContent.sCAT_Sys_ID = 0;
			}
			SEARCH_DBG("sCAT_Sys_ID= 0x%x, ecmPID = %d\n", pmtContent.sCAT_Sys_ID, pmtContent.ecmPID);
		}

		sectionPos = sectionPos+2+descLen;
	}

	sectionPos = &section[12+progInfoLen];
	for(i = 12+progInfoLen;i<sectionLen-4+3;i=i+esInfoLen+5)
	{
		streamType = sectionPos[0];
		elementaryPID = ((sectionPos[1]&0x1f)<<8)|sectionPos[2];
		switch(streamType)
		{
		case SIE_MPEG1_AUDIO_STREAM:
		case SIE_MPEG2_AUDIO_STREAM:
		case SIE_AAC_ADTS_AUDIO_STREAM:
		case SIE_AAC_LATM_AUDIO_STREAM:
		case SIE_AC3_AUDIO_STREAM:
			bProg = 1;
			if(0 == audioPID)
			{
				audioPID = elementaryPID;
			}
			ecmType = 0;
			switch(streamType)
			{
			case SIE_MPEG1_AUDIO_STREAM:
			case SIE_MPEG2_AUDIO_STREAM:
				audio_type = FYF_ACODEC_TYPE_MP3;
				break;
			case SIE_AAC_ADTS_AUDIO_STREAM:
			case SIE_AAC_LATM_AUDIO_STREAM:
				audio_type = FYF_ACODEC_TYPE_AAC;
				break;
			case SIE_AC3_AUDIO_STREAM:
				audio_type = FYF_ACODEC_TYPE_AC3;
				break;
			}
			break;	
		case SIE_MPEG1_VIDEO_STREAM:
		case SIE_MPEG2_VIDEO_STREAM:
		case SIE_MPEG4_VIDEO_STREAM:
		case SIE_H264_VIDEO_STREAM:	
			bProg = 1;
			avType = APP_CHN_AV_TYPE_TV_e;
			videoPID = elementaryPID;
			ecmType = 1;
			switch (streamType)
			{
			case SIE_MPEG1_VIDEO_STREAM:
			case SIE_MPEG2_VIDEO_STREAM:
				video_type = FYF_VCODEC_TYPE_MPEG2;
				break;
			case SIE_MPEG4_VIDEO_STREAM:
				video_type = FYF_VCODEC_TYPE_MPEG4;
				break;
			case SIE_H264_VIDEO_STREAM:	
				video_type = FYF_VCODEC_TYPE_H264;
				break;
			}
			
			break;
		case SIE_CA_DESC:

			sCAT_Sys_ID = ((BU16)(sectionPos[ 2 ] << 8) )& 0xff00 ;
			sCAT_Sys_ID = sCAT_Sys_ID |sectionPos[ 3 ] ;
			
			stTempData16 . byte .ucByte1 =  sectionPos[ 4 ] &0x1f ; 
			stTempData16 . byte .ucByte0 = sectionPos[ 5 ];

			FYF_API_printf("sCAT_Sys_ID = %d+++++++++++++++++++++++++++++++++++++++++++++\n", sCAT_Sys_ID);
		
			break;
		case 0x0b:
			bProg = 1;
			avType = APP_CHN_AV_TYPE_BROADCAST_e;
			APP_Printf("====================progid = %d========================\n", progNum);
			break;
		default:
			break;

		}
		esInfoLen = ((sectionPos[3]&0x0f)<<8)|sectionPos[4];
		sectionPos2 = &sectionPos[5];
		for(j = 0;j < esInfoLen; j = j+descLen+2)
		{
			descLen = sectionPos2[1];
			if(0x09 == sectionPos2[0])//CA_DESCRIPTOR
			{
				bCA = 1;
				if(descLen == 4)
				{
					pmtContent.sCAT_Sys_ID = (sectionPos2[2] << 8) | sectionPos2[3];
					if(ecmType == 0)
						pmtContent.audioEcmPid= ((sectionPos2[4] & 0x1F)<< 8) | sectionPos2[5];
					else if(ecmType == 1)
						pmtContent.videoEcmPid= ((sectionPos2[4] & 0x1F)<< 8) | sectionPos2[5];
					SEARCH_DBG("ecmType = %d, PID = %d\n", ecmType, ((sectionPos2[4] & 0x1F)<< 8) | sectionPos2[5]);
				}
			}
			sectionPos2 = sectionPos2+2+descLen;
		}
		sectionPos = sectionPos+5+esInfoLen;
	}

	if (bProg)//&& (audioPID != 0 || videoPID != 0))//有节目
	{	
		pmtContent.audioPID = audioPID;
		pmtContent.avType = avType;
		pmtContent.bCA = bCA;
		pmtContent.progNum = progNum;
		pmtContent.videoPID = videoPID;
		pmtContent.PCRPID = PCRPID;
		pmtContent.audiotype = audio_type;
		pmtContent.videotype = video_type;


		(*SavePmtInfo)(&pmtContent);
		
	}
	return 1;
}

void APP_PMT_IPI_StartParserTimeShiftPmt(BU08 *section, BU32 channelId, BU32 filterId)
{
	FYF_QUEUE_MESSAGE msgSend;
	if(0x02 != section[0])
	{
		APP_PRINT_ERROR ("APP_PMT_IPI_StartParserPmtPcrEcm table_id = 0x%x\n",section[0]);
		return;
	}

	if (APP_PMT_IPI_ParserPmtTimeShift(section, APP_PMT_IPI_SaveTimeShiftPmtInfo))
	{
		APP_PMT_IPI_StopDemuxTimeShiftPmt();
		msgSend.qlstWordOfMsg = 1;
		FYF_API_queue_send(APP_PMT_IPI_GetQueueParserPmtEnd(), &(msgSend));
	}
}


void APP_PMT_IPI_StartDemuxTimeShiftPmt(BU32 pmtPid)
{
	BU32 returnValue = 0;
	BU08 pmtCoef[1] = {0x02};
	BU08 pmtMask[1] = {0xFF};
	BU08 pmtExcl[1] = {0x00};
	struct ParserFuncNode_s parFuncNode;
	
	APP_PMT_IPI_StopDemuxTimeShiftPmt();
	APP_Parser_IPI_StartDemux(1, (pmtPid & 0xffff), pmtCoef, pmtMask, pmtExcl, &pmtTimeShiftChannelId, &pmtTimeShiftFilterId);
	parFuncNode.channelId = pmtTimeShiftChannelId;
	parFuncNode.filterId = pmtTimeShiftFilterId;
	parFuncNode.ParserFunc = APP_PMT_IPI_StartParserTimeShiftPmt;
	APP_IPI_AddPasFuncNode(parFuncNode);
	APP_IPI_ResetTimeGetPmt();
}

#if 0
void APP_PMT_IPI_SaveVodPmtInfo(PMT_Content_s *pmtContents)
{
	APP_EnrichVod_UpdateCurVodInfo(pmtContents->videoPID, pmtContents->audioPID, pmtContents->PCRPID);
}
void APP_PMT_IPI_StartParserVodPmt(BU08 *section, BU32 channelId, BU32 filterId)
{
	FYF_QUEUE_MESSAGE msgSend;
	if(0x02 != section[0])
	{
		APP_PRINT_ERROR ("APP_PMT_IPI_StartParserPmtPcrEcm table_id = 0x%x\n",section[0]);
		return;
	}
	
	APP_PMT_IPI_StopDemuxTimeShiftPmt();

	APP_PMT_IPI_ParserPmt(section, APP_PMT_IPI_SaveVodPmtInfo);
	msgSend.qlstWordOfMsg = 1;
	FYF_API_queue_send(APP_PMT_IPI_GetQueueParserPmtEnd(), &(msgSend));
}
void APP_PMT_IPI_StartDemuxVodPmt(BU32 pmtPid)
{
	BU32 returnValue = 0;
	BU08 pmtCoef[1] = {0x02};
	BU08 pmtMask[1] = {0xFF};
	BU08 pmtExcl[1] = {0x00};
	struct ParserFuncNode_s parFuncNode;
	
	APP_PMT_IPI_StopDemuxTimeShiftPmt();
	APP_Parser_IPI_StartDemux(1, (pmtPid & 0xffff), pmtCoef, pmtMask, pmtExcl, &pmtTimeShiftChannelId, &pmtTimeShiftFilterId);
	parFuncNode.channelId = pmtTimeShiftChannelId;
	parFuncNode.filterId = pmtTimeShiftFilterId;
	parFuncNode.ParserFunc = APP_PMT_IPI_StartParserVodPmt;
	APP_IPI_AddPasFuncNode(parFuncNode);
	APP_IPI_ResetTimeGetPmt();
}
#endif
static BU32 pmtPcrEcmChannelId = 0;
static BU32 pmtPcrEcmFilterId = 0;

void APP_PMT_IPI_StopDemuxPmtPcrEcm(void)
{
	APP_Parser_IPI_StopDemux(&pmtPcrEcmChannelId, &pmtPcrEcmFilterId);
}

void APP_PMT_IPI_StartDemuxPmtPcrEcm(BS16 pmtPID)
{
	BU32 returnValue = 0;
	BU08 pmtCoef[1] = {0x02};
	BU08 pmtMask[1] = {0xFF};
	BU08 pmtExcl[1] = {0x00};
	struct ParserFuncNode_s parFuncNode;
	
	APP_PMT_IPI_StopDemuxPmtPcrEcm();
	APP_Parser_IPI_StartDemux(1, pmtPID, pmtCoef, pmtMask, pmtExcl, &pmtPcrEcmChannelId, &pmtPcrEcmFilterId);
	parFuncNode.channelId = pmtPcrEcmChannelId;
	parFuncNode.filterId = pmtPcrEcmFilterId;
	parFuncNode.ParserFunc = APP_PMT_IPI_StartParserPmtPcrEcm;
	APP_IPI_AddPasFuncNode(parFuncNode);
	APP_IPI_ResetTimeGetPmt();
}


void APP_PMT_IPI_StopAllDemuxPmt(void)
{
	BU32 i = 0;
	
	FYF_API_semaphore_wait(semaphoreDemuxPmt, FYF_WAIT_FOREVER);
	for (i = 0; i < MAX_DEMUX_PMT_CHANNEL; i++)
	{
		if (pmtChannels[i].pmtPID != 0)
		{
			APP_Parser_IPI_StopDemux(&(pmtChannels[i].pmtChannelID), &(pmtChannels[i].pmtFilterID));
			pmtChannels[i].isStop = 1;
			pmtChannels[i].pmtPID = 0;
			pmtChannels[i].pmtChannelID = 0;
			pmtChannels[i].pmtFilterID = 0;
		}
	}
	FYF_API_semaphore_release(semaphoreDemuxPmt);
}

void APP_PMT_IPI_StopDemuxPmt(BU16 index)
{
	BU32 i = 0;
	

	APP_Parser_IPI_StopDemux(&(pmtChannels[index].pmtChannelID), &(pmtChannels[index].pmtFilterID));
	pmtChannels[index].isStop = 1;
	pmtChannels[index].pmtPID = 0;
	pmtChannels[index].pmtChannelID = 0;
	pmtChannels[index].pmtFilterID = 0;			

}




void APP_PMT_IPI_ResetPmtQueue(void)
{
	FYF_QUEUE_MESSAGE msgRecvTemp;
	
	if(queueParserPmtEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserPmtEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserPmtEnd = FYF_API_queue_create("qPPmtE", MAX_DEMUX_PMT_CHANNEL, FYF_THREAD_WAIT_FIFO);		
	}
}

void APP_PMT_IPI_StartParserPmt(BU08 *section, BU32 channelId, BU32 filterId)
{
	BU32 i = 0;
	static FYF_QUEUE_MESSAGE msgSend;//[MAX_DEMUX_PMT_CHANNEL];
//	static BU32 msgIndex = 0;

	if(0x02 != section[0])
	{
		APP_PRINT_ERROR("APP_PMT_IPI_StartParserPmt table_id = 0x%x\n",section[0]);
		return;
	}
	FYF_API_semaphore_wait(semaphoreDemuxPmt, FYF_WAIT_FOREVER);
//	FYF_API_printf("APP_PMT_IPI_StartParserPmt()\n");
	for(i = 0; i < MAX_DEMUX_PMT_CHANNEL; i++)
	{
		if(pmtChannels[i].pmtChannelID == channelId && pmtChannels[i].pmtFilterID == filterId)
		{
			if (pmtChannels[i].isStop)
			{
				FYF_API_semaphore_release(semaphoreDemuxPmt);
				return;
			}
			break;
		}
	}

	if(i == MAX_DEMUX_PMT_CHANNEL)
	{
		FYF_API_semaphore_release(semaphoreDemuxPmt);
		return;
	}
//	APP_Printf("pmtpid = 0x%x\n", pmtChannels[i].pmtPID);
	if (section[3] != ((pmtChannels[i].pmtPID >>24)&0xff) || section[4] != ((pmtChannels[i].pmtPID >> 16)&0xff))
	{
		APP_PRINT_ERROR("pmt pid chongfu error \n");
		FYF_API_semaphore_release(semaphoreDemuxPmt);
		return;
	}
	APP_PMT_IPI_StopDemuxPmt(i);//pmtChannels[i].pmtPID);

#ifdef USE_NEW_PMT_FUNC
	BUILDER_ParsePMT(section, APP_PMT_IPI_SavePmtInfo);
#else
	APP_PMT_IPI_ParserPmt(section, APP_PMT_IPI_SavePmtInfo);
#endif
	//TPCA_ParsePMT(section, &pmtContents);

	msgSend.qlstWordOfMsg = 1;
	FYF_API_queue_send(APP_PMT_IPI_GetQueueParserPmtEnd(), &(msgSend));

	FYF_API_semaphore_release(semaphoreDemuxPmt);
}

void APP_PMT_IPI_StartParserPmtPcrEcm(BU08 *section, BU32 channelId, BU32 filterId)
{
	if(0x02 != section[0])
	{
		APP_PRINT_ERROR ("APP_PMT_IPI_StartParserPmtPcrEcm table_id = 0x%x\n",section[0]);
		return;
	}
	
	APP_PMT_IPI_StopDemuxPmtPcrEcm();

	APP_PMT_IPI_ParserPmt(section, APP_PMT_IPI_SavePmtInfoPcrEcm);
}












