#include "app_common.h"
#include "app_ipi_common.h"

extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];
extern BU32 searchProgCount;
extern BU08 isStopSearch;

static BU32	sdtChannelID = 0;
static BU32 sdtFilterID = 0;

static BU08	isStopParserSdt = 0;
static BU32	queueParserSdtEnd = 0;

static BU16 glb_SIE_SearchONID=0;

static SectionNode_s *pSdtFirstSectionNode = 0;
static SectionNode_s *pSdtOnTimeFirstSectionNode = 0;


/* 
 * 节目类型常量定义
 */
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


void ParserSdt_printf (const char *fmt ,...)
{
/*
	char	        report_buffer[4*1024];
	va_list      list;	
	va_start(list, fmt);
	vsprintf(report_buffer, fmt, list);
	va_end(list);
	printf(( report_buffer ));
*/
}

BU32 APP_SDT_IPI_ParserSdt(BU08 * section, void (*SaveSdtInfo)(SDT_Content_s *))
{
	SDT_Content_s sdtContent;
	BU16 sectionLen,SID,i,descriptorsLoopLen,j;
	BU08 descriptorLen,descriptorTag,progNameLen;
	
	
	BU08* sectionPos;
	BU08* progNamepos;
	BU32 sdtContentCount = 0;
	BU32 oriNetId = 0;
	//ParserSdt_printf("section_number = %d, last_section_number = %d\n", section[6], section[7]);
	sectionLen = SIE_Search_GetSetionLen(section);
	
	if(0 == section[6])//ON ID
	{
		glb_SIE_SearchONID = (section[3]<<8)|section[4];
		//ParserSdt_printf("transport_Stream_id = %d\n", glb_SIE_SearchONID);
	}
	
	oriNetId = ((section[8] & 0xff)<<8) | (section[9]);
	sectionPos = section+11;
	i = 8;
	while(i < sectionLen-4)
	{
		SID = (BU16)((sectionPos[0]<<8)|sectionPos[1]);
		descriptorsLoopLen = (BU16)(((sectionPos[3]&0X0F)<<8)|sectionPos[4]);

		/*清除sdtContent结构体,并赋值OriNetId*/
		memset(&sdtContent,0,sizeof(SDT_Content_s));
		sdtContent.OriNetId = oriNetId;
		sectionPos = sectionPos+5;
		for(j = 0;j < descriptorsLoopLen;j=j+descriptorLen+2)
		{
			descriptorTag = sectionPos[0];
			descriptorLen = sectionPos[1];
			switch(descriptorTag)
			{
			case 0x48:
				switch (sectionPos[2])
				{
				case TYPE_OF_TV_SERVICE:
					sdtContent.service_type = APP_CHN_AV_TYPE_TV_e;
					break;
				case TYPE_OF_RADIO_SERVICE:
					sdtContent.service_type = APP_CHN_AV_TYPE_RD_e;
					break;
				case TYPE_OF_BROADCAST_SERVICE:
					sdtContent.service_type = APP_CHN_AV_TYPE_BROADCAST_e;
					break;
				case TYPE_OF_NVOD_REFERENCE_SERVICE:
					sdtContent.service_type = APP_CHN_AV_TYPE_NVOD_REFERENCE_SERVICE;
#ifdef ENABLE_NVOD
					FYF_API_nvod_ref_service_descriptor (SID, &sectionPos[2]);
#endif
					break;
				case TYPE_OF_NVOD_TIMESHIFT_SERVICE:
					sdtContent.service_type = APP_CHN_AV_TYPE_NVOD_TIMESHIFT_SERVICE;
					break;
				default:
					sdtContent.service_type = APP_CHN_AV_TYPE_NULL;
					break;
				}
				
				progNamepos = &sectionPos[4+sectionPos[3]+1];
				progNameLen = *(progNamepos-1);
				if(progNameLen>50)
				{
					progNameLen = 50;
				}
				
				memset(sdtContent.service_name, '\0', 51);
				memcpy(sdtContent.service_name,progNamepos,progNameLen);
				sdtContent.service_id = SID;
				//(*SaveSdtInfo)(&sdtContent);
				sdtContentCount++;
				break;
#ifdef ENABLE_NVOD
			case 0x4C:
				/*防止不正规码流源中，当sdt中只有时移描述符(0x4c),没有业务描述符时的也能保存sid*/
				if(sdtContent.service_type == APP_CHN_AV_TYPE_NULL)
				{
					sdtContent.service_type = APP_CHN_AV_TYPE_NVOD_TIMESHIFT_SERVICE_DESC;
					memset(sdtContent.service_name, '\0', 51);
					sdtContent.service_id = SID;
				}
				FYF_API_time_shifted_service_descriptor (SID, &sectionPos[2]);
				break;
#endif
			default:
				break;
			}
			(*SaveSdtInfo)(&sdtContent);
			sectionPos = sectionPos+2+descriptorLen;
		}
		i = i + descriptorsLoopLen + 5;
	}
	
	return sdtContentCount;
}

void APP_SDT_IPI_SaveSdtInfo(SDT_Content_s *sdtContent)
{
	BU32 dbNum = 0;
	
	//写入program表

	SEARCH_DBG("Save SDT serviceid = %d, type = %d, name = %s\n", sdtContent->service_id, sdtContent->service_type, sdtContent->service_name);
	if(GUI_API_Strlen(sdtContent->service_name) == 0)
	{
		sprintf(sdtContent->service_name, "未命名%d", sdtContent->service_id);
	}

	DBA_API_AddSdtInfo(JoinSevIdFreq(sdtContent->service_id, APP_IPI_GetTunerFreq())
		,sdtContent->service_name,sdtContent->service_type,sdtContent->OriNetId);
#if 0
#ifdef FYF_NVOD		
	/*
	保存NVOD参考业务
	*/
	
	if(sdtContent->service_type == APP_CHN_AV_TYPE_NVOD_REFERENCE_SERVICE)
	{
		SQL_API_Semaphore_Wait();
		sprintf(g_StrSQL, "SELECT service_id FROM program WHERE service_id = %d;", JoinSevIdFreq(sdtContent->service_id, APP_IPI_GetTunerFreq()));
		SQL_API_Select(g_StrSQL);

		if(SQL_API_GetTotalNum() == 0)
		{
			if(++searchProgCount >= APP_MAX_PROG)
			{
				APP_Search_API_HaltSearch();
			}
			sprintf(g_StrSQL,"INSERT INTO program \
(service_id, 编号, 节目名, 加密, 加锁, \
喜爱, 删除, 跳过, 移动, 声道, \
音量补偿, 频率, 调制方式, 符号率, 服务类型, \
视频PID, 音频PID, PCRPID , ecmPID , sCAT_Sys_ID, \
TSID, PID, OriNetId) \
VALUES(%d, %d, '%s', %d, 0, \
0, 0, 0, 0, 1, \
0, %d, %d, %d, %d, \
%d, %d, %d, %d, %d, \
%d, %d, %d);",
				JoinSevIdFreq(sdtContent->service_id,APP_IPI_GetTunerFreq()),JoinSevIdFreq(sdtContent->service_id,APP_IPI_GetTunerFreq()), sdtContent->service_name, 0,
				APP_IPI_GetTunerFreq(), APP_IPI_GetTunerMod(), APP_IPI_GetTunerSym(), sdtContent->service_type,
				0, 0, 0, 0, 0,
				0, 0, sdtContent->OriNetId);
		}
		else
		{
			sprintf(g_StrSQL, "UPDATE program SET \
节目名 = '%s', 频率 = %d, 符号率 = %d, 调制方式 = %d, 视频PID = %d, \
音频PID = %d, 加密 = %d, 服务类型 = %d, PCRPID = %d, ecmPID = %d, \
sCAT_Sys_ID = %d, 跳过 = 0, 音量补偿 = 0, TSID = %d , PID = %d ,OriNetId = %d \
WHERE service_id = %d;",
				sdtContent->service_name, APP_IPI_GetTunerFreq(), APP_IPI_GetTunerSym(), APP_IPI_GetTunerMod(),0,
				0, 0, sdtContent->service_type, 0, 0, 
				0, 0, 0, sdtContent->OriNetId,
				JoinSevIdFreq(sdtContent->service_id, APP_IPI_GetTunerFreq()));
		}
		SQL_API_Exec(g_StrSQL);
		FYF_API_printf("%s\n", g_StrSQL);
		SQL_API_Semaphore_Release();
	}
#endif	
#endif
	//APP_Printf("SID = %d, progName = %s------------------------------OK!\n", sdtContents->service_id, sdtContents->service_name);
}

BU32 APP_SDT_IPI_GetQueueParserSdtEnd(void)
{
	return queueParserSdtEnd;
}

void APP_SDT_IPI_StartDemuxSdt(void)
{
	BU08 sdtCoef[1] = {0x42};
	BU08 sdtMask[1] = {0xFF};
	BU08 sdtExcl[1] = {0x00};
	struct ParserFuncNode_s parFuncNode;
	BU32 returnValue = 0;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	
	if(queueParserSdtEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserSdtEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserSdtEnd = FYF_API_queue_create("qPSdtE", MAX_DEMUX_PMT_CHANNEL, FYF_THREAD_WAIT_FIFO);		
	}
	
	APP_SDT_IPI_StopDemuxSdt();
/*
	SQL_API_Semaphore_Wait();
	sprintf(g_StrSQL, "DELETE FROM %s;", SDT_TABLE_NAME);
	SQL_API_Exec(g_StrSQL);
	SQL_API_Semaphore_Release();
*/
	DBA_API_ClearSdtInfo();
	
	APP_IPI_InitSectionNode(&pSdtFirstSectionNode);
	isStopParserSdt = 0;
	APP_Parser_IPI_StartDemux(1, 0x0011, sdtCoef, sdtMask, sdtExcl, &sdtChannelID, &sdtFilterID);
	parFuncNode.channelId = sdtChannelID;
	parFuncNode.filterId = sdtFilterID;
	parFuncNode.ParserFunc = APP_SDT_IPI_StartParserSdt;
	APP_IPI_AddPasFuncNode(parFuncNode);
}

void APP_SDT_IPI_StopDemuxSdt(void)
{
	isStopParserSdt = 1;
	APP_Parser_IPI_StopDemux(&sdtChannelID, &sdtFilterID);
	APP_IPI_ClearSection(&pSdtFirstSectionNode);
}

void APP_SDT_IPI_StartParserSdt(BU08 *section, BU32 channelId, BU32 filterId)
{
	BU32 sdtContentCount = 0;
  	static FYF_QUEUE_MESSAGE msgSend;

	if(0x42 != section[0])
	{
		APP_PRINT_ERROR("APP_SDT_IPI_StartParserSdt table_id = 0x%x\n",section[0]);
		return;
	}
	
	if (isStopParserSdt)
	{
		return;
	}

	
	sdtContentCount = APP_SDT_IPI_ParserSdt(section, APP_SDT_IPI_SaveSdtInfo);
	if(pSdtFirstSectionNode == 0)
		return ;
		
	APP_IPI_AddSection(pSdtFirstSectionNode, section[6]);
	if ((section[7] + 1) == (BU08)APP_IPI_GetSectionCount(pSdtFirstSectionNode))
	{
		APP_SDT_IPI_StopDemuxSdt();
		msgSend.qlstWordOfMsg= 1;
		FYF_API_queue_send(APP_SDT_IPI_GetQueueParserSdtEnd(), &msgSend);
	}
}



static BU32 sdtOnTimeChannelId = 0;
static BU32 sdtOnTimeFilterId = 0;
void APP_SDT_IPI_StopDemuxSdtOnTime(void)
{
//	APP_Printf("APP_SDT_IPI_StopDemuxSdtOnTime\n");
	APP_Parser_IPI_StopDemux(&sdtOnTimeChannelId, &sdtOnTimeFilterId);
	APP_IPI_ClearSection(&pSdtOnTimeFirstSectionNode);
}


void APP_SDT_IPI_SaveSdtOnTimeInfo(SDT_Content_s *sdtContent)
{
//	APP_Printf("APP_SDT_IPI_SaveSdtOnTimeInfo\n");


	ChnDataBase_ChnInfo_S chninfo;
	//写入program表
#if 0
	SEARCH_DBG("Save SDT serviceid = %d, type = %d, name = %s\n", sdtContent->service_id, sdtContent->service_type, sdtContent->service_name);
	if(GUI_API_Strlen(sdtContent->service_name) == 0)
	{
		sprintf(sdtContent->service_name, "未命名%d", sdtContent->service_id);
	}

	DBA_API_AddSdtInfo(JoinSevIdFreq(sdtContent->service_id, APP_IPI_GetTunerFreq())
		,sdtContent->service_name,sdtContent->service_type,sdtContent->OriNetId);
#endif
	//APP_Printf("SID = %d, progName = %s------------------------------OK!\n", sdtContents->service_id, sdtContents->service_name);
	//APP_Printf("APP_SDT_IPI_SaveSdtOnTimeInfo %d %d %s %d\n", sdtContent->service_id, sdtContent->service_type, sdtContent->service_name, sdtContent->OriNetId);

	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chninfo))
	{
		return ;
	}

	if (chninfo.m_ProgId != JoinSevIdFreq(sdtContent->service_id, APP_IPI_GetTunerFreq()))
	{
		return; 
	}

	if (strlen(sdtContent->service_name) != 0 && strcmp(chninfo.m_ChnName, sdtContent->service_name) != 0)
	{
		 APP_PRINT_DEBUG("%s %s\n", chninfo.m_ChnName, sdtContent->service_name);
		 SQL_API_Semaphore_Wait();
		 sprintf(g_StrSQL, "UPDATE program SET \
				节目名 = '%s' WHERE service_id = %d;", sdtContent->service_name, chninfo.m_ProgId);
		 SQL_API_Exec(g_StrSQL);
		 SQL_API_Semaphore_Release();
		// APP_ChnDatabase_API_SetChnInfo (app_system_data.g_CurChnNo, &chninfo);
		APP_ChnDatabase_API_SetChnNameByServiceid(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),chninfo.m_ProgId, sdtContent->service_name);
		APP_ChnDatabase_API_UpDataToFlash(0);
	}
	
}


void APP_SDT_IPI_StartParserSdtOnTime(BU08 *section, BU32 channelId, BU32 filterId)
{
//	APP_Printf("APP_SDT_IPI_StartParserSdtOnTime\n");
	if(0x42 != section[0])
	{
		APP_PRINT_ERROR("APP_SDT_IPI_StartParserSdt table_id = 0x%x\n",section[0]);
		return;
	}

	APP_SDT_IPI_ParserSdt(section, APP_SDT_IPI_SaveSdtOnTimeInfo);
	if(pSdtOnTimeFirstSectionNode == 0)
		return ;
		
	APP_IPI_AddSection(pSdtOnTimeFirstSectionNode, section[6]);
	if ((section[7] + 1) == (BU08)APP_IPI_GetSectionCount(pSdtOnTimeFirstSectionNode))
	{
		APP_SDT_IPI_StopDemuxSdtOnTime();
	}
	//APP_SDT_IPI_ParserSdt(section, APP_SDT_IPI_SaveSdtOnTimeInfo);	
	//APP_SDT_IPI_StopDemuxSdtOnTime();

}
void APP_SDT_IPI_StartDemuxSdtOnTime(void)
{
	BU08 sdtCoef[1] = {0x42};
	BU08 sdtMask[1] = {0xFF};
	BU08 sdtExcl[1] = {0x00};

//	APP_Printf("APP_SDT_IPI_StartDemuxSdtOnTime !!!!!!!!!!!!!!!!!!!!!\n");

	struct ParserFuncNode_s parFuncNode;
	

	APP_SDT_IPI_StopDemuxSdtOnTime();
	APP_IPI_InitSectionNode(&pSdtOnTimeFirstSectionNode);

	APP_Parser_IPI_StartDemux(1, 0x0011, sdtCoef, sdtMask, sdtExcl, &sdtOnTimeChannelId, &sdtOnTimeFilterId);
	parFuncNode.channelId = sdtOnTimeChannelId;
	parFuncNode.filterId = sdtOnTimeFilterId;
	parFuncNode.ParserFunc = APP_SDT_IPI_StartParserSdtOnTime;
	APP_IPI_AddPasFuncNode(parFuncNode);
	APP_IPI_ResetTimeGetSdt();
	
}


