#include <math.h>
#include "app_common.h"
#include "app_ipi_common.h"

extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];

static BU32	patChannelID = 0;
static BU32 patFilterID = 0;
static BU08	isStopParserPat = 0;
static BU32	queueParserPatEnd = 0;

static BU16 __glb_SIE_SearchTSID=0;

PAT_AreaCode nanping_areacode[] = {{6, 7101},{7, 7201},{11, 7102}, {12, 7202}, {16, 7103}, {17, 7203}, \
	{21, 7104}, {22, 7204}, {26, 7105},{27, 7205}, {31, 7106}, {32, 7206}, {36, 7107}, {37, 7207}, {41, 7108}, {42, 7208}, {46, 7109}, {47, 7209} };


void APP_PAT_IPI_StartParserPatOnTime(BU08 *pSection, BU32 channelId, BU32 filterId);

/*
void ParserPat_printf (const char *fmt ,...)
{
	char	        report_buffer[4*1024];
	va_list      list;	
	va_start(list, fmt);
	vsprintf(report_buffer, fmt, list);
	va_end(list);
	FYF_API_printf(( report_buffer ));
}
*/

BU08 APP_PAT_IPI_CheckHide(BU32 area_code, BU16 service_id)
{
	BS32 i = 0;
	//if (626 != APP_IPI_GetTunerFreq()) return APP_SUCCESS;
	//APP_PRINT_DEBUG("area_code = %d, service_id = %d\n", area_code, service_id);
	for (i = 0; i < 18; i++)
	{
		if (nanping_areacode[i].service_id == service_id)
		{
			break;
		}
	}

	if (i >= 18)
	{
		return APP_SUCCESS;
	}
	else
	{
		if (abs(nanping_areacode[i].area_code - area_code) <= 1)
			return APP_SUCCESS;
		else
			return APP_FAIL;
	}
	
}

BU32 APP_PAT_IPI_ParserPat(BU08 * section, void (*SavePatInfo)(PAT_Content_s *))
{
	PAT_Content_s patContent;
	BU16 sectionLen,pmtPID,progNum;
	BS32 parserNum;
	BU08* sectionPos;
	BU32 i = 0;
	BU32 iDebug = 0;
	static BU32 init_first = 0;
	BU08 DVTAreaCode[4];
	BU32 area_code = 0;
	APP_SYS_API_GetSysInfo(APP_SYS_DVTCAAreaCode_e, (BU32 *)DVTAreaCode);
	area_code = DVTAreaCode[0];
	area_code = (area_code<<8)|DVTAreaCode[1];
	area_code = (area_code<<8)|DVTAreaCode[2];
	area_code = (area_code<<8)|DVTAreaCode[3];
	sectionLen = SIE_Search_GetSetionLen(section);
	parserNum  = sectionLen - 5 - 4;
	parserNum  = parserNum/4;
	if(0 == section[6])//TS ID
	{
		__glb_SIE_SearchTSID = (section[3]<<8)|section[4];
	}
	sectionPos = section+8;
	while(parserNum > 0)
	{
		progNum = (BU16)((sectionPos[0]<<8)|sectionPos[1]);
		pmtPID  = (BU16)(((sectionPos[2]&0x1f)<<8)|sectionPos[3]);
		patContent.service_id = progNum;
		patContent.service_PID = pmtPID;
		patContent.ts_id = __glb_SIE_SearchTSID;
		
		sectionPos+=4;
		parserNum--;
		//if (APP_PAT_IPI_CheckHide(area_code, progNum))
		{
			(*SavePatInfo)(&patContent);
			i++;
		}
	}
	return i;
}


void APP_PAT_IPI_SavePatInfo(PAT_Content_s *patContents)
{
	BU32 dbNum = 0;

	SEARCH_DBG("Save pat serviceid = %d, pmtpid = %d, ts_id = %d\n", patContents->service_id, patContents->service_PID, patContents->ts_id);

	if(patContents->service_id != 0)
	{
//		FYF_API_printf("PAT progNum = %d, pmtPID = %d\n", JoinSevIdFreq(patContents->service_id, APP_IPI_GetTunerFreq()), patContents->service_PID);
/*
		SQL_API_Semaphore_Wait();
		sprintf(g_StrSQL,"SELECT SID FROM %s WHERE SID = %d;",
			PMT_PID_TABLE_NAME, JoinSevIdFreq(patContents->service_id, APP_IPI_GetTunerFreq()));
		SQL_API_Select(g_StrSQL);
		dbNum=SQL_API_GetTotalNum();
		if(0 == dbNum)
		{
			sprintf(g_StrSQL,"INSERT INTO %s (SID, PID, TSID) VALUES(%d,%d,%d);",
				PMT_PID_TABLE_NAME, 
				JoinSevIdFreq(patContents->service_id, APP_IPI_GetTunerFreq()), 
				patContents->service_PID,
				patContents->ts_id);
			SQL_API_Exec(g_StrSQL);
		}
		SQL_API_Semaphore_Release();
*/
		DBA_API_AddPmtPidInfo(JoinSevIdFreq(patContents->service_id, APP_IPI_GetTunerFreq())
			,patContents->service_PID,patContents->ts_id);
	}

	

}
void APP_PAT_IPI_SavePatInfoOnTime(PAT_Content_s *patContents)
{
	BU32 pid = 0;
	BU32 progId = 0;
	ChnDataBase_ChnInfo_S progtemp;

	if(patContents->service_id != 0)
	{

		progId = JoinSevIdFreq(patContents->service_id, APP_IPI_GetTunerFreq());
		if (APP_FAIL == APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),progId, &progtemp))
		{
			PATMONTIOR_DBG("service_id = %d, freq = %d\n ", patContents->service_id, APP_IPI_GetTunerFreq());
			return;
		}
		pid = progtemp.m_pmtpid;
#if 0
		if(oldProgId != progId)
		{
			oldProgId = progId;
			sprintf(g_StrSQL,"SELECT PID FROM program WHERE service_id = %d;",
				JoinSevIdFreq(patContents->service_id, APP_IPI_GetTunerFreq()));
			SQL_API_Select(g_StrSQL);
			if(SQL_API_GetTotalNum() != 0)
			{
				oldPID= SQL_API_GetData(0,0,0,APP_DATA_TYPE_NUM);
			}
			else
			{
				SQL_API_Semaphore_Release();
				return;
			}
		}
		pid = oldPID;
	//	FYF_API_printf("APP_PAT_IPI_SavePatInfoOnTime enter 2 pid = %d\n",pid);
#endif

		if(pid != (BU32)(patContents->service_PID))
		{
			progtemp.m_pmtpid = patContents->service_PID;
			APP_ChnDatabase_API_SetChnInfoByServiceid(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), progId, &progtemp);
			SQL_API_Semaphore_Wait();
			sprintf(g_StrSQL, "UPDATE program SET PID = %d WHERE service_id = %d;",
				patContents->service_PID, progId);				
			SQL_API_Exec(g_StrSQL);
			SQL_API_Semaphore_Release();
			APP_IPI_SetTimeGetPmt(APP_CONFIG_PMT_MONTIOR_TIME);
			APP_PRINT_DEBUG("Get different PID old pid = %d, new pid = %d\n", pid, patContents->service_PID);
		}
		
	}
//	FYF_API_printf("APP_PAT_IPI_SavePatInfoOnTime out\n");
}




BU32 APP_PAT_IPI_GetQueueParserPatEnd(void)
{
	return queueParserPatEnd;
}



void APP_PAT_IPI_StartDemuxPatOnTime(void)
{
	BU08 patCoef[1] = {0x00};
	BU08 patMask[1] = {0xFF};
	BU08 patExcl[1] = {0x00};
	BU32 returnValue = 0;
	//APP_PRINT_ERROR("void APP_PAT_IPI_StartDemuxPatOnTime() start\n");
	struct ParserFuncNode_s parFuncNode;

	APP_PAT_IPI_StopDemuxPat();
	isStopParserPat = 0;
	APP_Parser_IPI_StartDemux(1, 0x0000, patCoef, patMask, patExcl, &patChannelID, &patFilterID);
	parFuncNode.channelId = patChannelID;
	parFuncNode.filterId = patFilterID;
	parFuncNode.ParserFunc = APP_PAT_IPI_StartParserPatOnTime;
	APP_IPI_AddPasFuncNode(parFuncNode);
	APP_IPI_ResetTimeGetPat();
	//APP_PRINT_ERROR("void APP_PAT_IPI_StartDemuxPatOnTime() end\n");
}


void APP_PAT_IPI_StartDemuxPat(void)
{
	BU08 patCoef[1] = {0x00};
	BU08 patMask[1] = {0xFF};
	BU08 patExcl[1] = {0x00};
	BU32 returnValue = 0;
	struct ParserFuncNode_s parFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;

	if(queueParserPatEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserPatEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserPatEnd = FYF_API_queue_create("qPPatE", 3, FYF_THREAD_WAIT_FIFO);		
	}


	DBA_API_ClearPmtPidInfo();
	
	APP_PAT_IPI_StopDemuxPat();
	isStopParserPat = 0;
	APP_PRINT_ERROR("APP_Parser_IPI_StartDemux(1, 0x0000, patCoef, patMask, patExcl, &patChannelID, &patFilterID) = %d\n", APP_Parser_IPI_StartDemux(1, 0x0000, patCoef, patMask, patExcl, &patChannelID, &patFilterID));
	parFuncNode.channelId = patChannelID;
	parFuncNode.filterId = patFilterID;
	parFuncNode.ParserFunc = APP_PAT_IPI_StartParserPat;
	APP_IPI_AddPasFuncNode(parFuncNode);
}

void APP_PAT_IPI_StopDemuxPat(void)
{
	isStopParserPat = 1;
	APP_Parser_IPI_StopDemux(&patChannelID, &patFilterID);
}

void APP_PAT_IPI_StartParserPat(BU08 *pSection, BU32 channelId, BU32 filterId)
{
	static FYF_QUEUE_MESSAGE msgSend;

	BU32 patContentCount = 0;

	if(0x00 != pSection[0])
	{
		APP_PRINT_ERROR("APP_PAT_IPI_StartParserPat table_id = 0x%x\n",pSection[0]);
		return;
	}
	
	if (isStopParserPat)
	{
		return;
	}
	
	APP_PAT_IPI_StopDemuxPat();
	
	patContentCount = APP_PAT_IPI_ParserPat(pSection, APP_PAT_IPI_SavePatInfo);
	
	msgSend.qlstWordOfMsg = 1;
	FYF_API_queue_send(APP_PAT_IPI_GetQueueParserPatEnd(), &msgSend);
	//APP_PRINT_ERROR("msgSend.qlstWordOfMsg = 1\n");
}

void APP_PAT_IPI_StartParserPatOnTime(BU08 *pSection, BU32 channelId, BU32 filterId)
{
	if(0x00 != pSection[0])
	{
		APP_PRINT_ERROR("APP_PAT_IPI_StartParserPatOnTime table_id = 0x%x\n",pSection[0]);
		return;
	}

	APP_PAT_IPI_StopDemuxPat();

	
	APP_PAT_IPI_ParserPat(pSection, APP_PAT_IPI_SavePatInfoOnTime);

}

BU16 g_enrichvod_serviceid = 0;
BU32 APP_PAT_IPI_ParserEnrichVodPat(BU08 * section, void (*SavePatInfo)(PAT_Content_s *))
{
	PAT_Content_s patContent;
	BU16 sectionLen,pmtPID,progNum;
	BS32 parserNum;
	BU08* sectionPos;
	BU32 i = 0;
	BU32 iDebug = 0;
	sectionLen = SIE_Search_GetSetionLen(section);
	parserNum  = sectionLen - 5 - 4;
	parserNum  = parserNum/4;
	if(0 == section[6])//TS ID
	{
		__glb_SIE_SearchTSID = (section[3]<<8)|section[4];
	}
	sectionPos = section+8;
	while(parserNum > 0)
	{
		progNum = (BU16)((sectionPos[0]<<8)|sectionPos[1]);
		pmtPID  = (BU16)(((sectionPos[2]&0x1f)<<8)|sectionPos[3]);
		patContent.service_id = progNum;
		patContent.service_PID = pmtPID;
		patContent.ts_id = __glb_SIE_SearchTSID;
		
		sectionPos+=4;
		parserNum--;
		if (progNum == g_enrichvod_serviceid)
		{
			(*SavePatInfo)(&patContent);
			i++;
			break;
		}
	}
	return i;
}

void APP_PAT_IPI_StartParserEnrichVodPat(BU08 *pSection, BU32 channelId, BU32 filterId)
{
	static FYF_QUEUE_MESSAGE msgSend;

	BU32 patContentCount = 0;

	if(0x00 != pSection[0])
	{
		APP_PRINT_ERROR("APP_PAT_IPI_StartParserPat table_id = 0x%x\n",pSection[0]);
		return;
	}
	
	if (isStopParserPat)
	{
		return;
	}

	
	if (APP_PAT_IPI_ParserEnrichVodPat(pSection, APP_PAT_IPI_SavePatInfo))
	{
		APP_PAT_IPI_StopDemuxPat();
		msgSend.qlstWordOfMsg = 1;
		APP_PRINT_ERROR("msgSend.qlstWordOfMsg = 1\n");
		FYF_API_queue_send(APP_PAT_IPI_GetQueueParserPatEnd(), &msgSend);
	}

		
}


void APP_PAT_IPI_StartDemuxEnrichVodPat(BU16 serviceid)
{
	BU08 patCoef[1] = {0x00};
	BU08 patMask[1] = {0xFF};
	BU08 patExcl[1] = {0x00};
	BU32 returnValue = 0;
	struct ParserFuncNode_s parFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;

	if(queueParserPatEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserPatEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserPatEnd = FYF_API_queue_create("qPPatE", 3, FYF_THREAD_WAIT_FIFO);		
	}


	DBA_API_ClearPmtPidInfo();
	
	APP_PAT_IPI_StopDemuxPat();
	g_enrichvod_serviceid = serviceid;
	isStopParserPat = 0;
	APP_PRINT_ERROR("APP_Parser_IPI_StartDemux(1, 0x0000, patCoef, patMask, patExcl, &patChannelID, &patFilterID) = %d\n", APP_Parser_IPI_StartDemux(1, 0x0000, patCoef, patMask, patExcl, &patChannelID, &patFilterID));
	parFuncNode.channelId = patChannelID;
	parFuncNode.filterId = patFilterID;
	parFuncNode.ParserFunc = APP_PAT_IPI_StartParserEnrichVodPat;
	APP_IPI_AddPasFuncNode(parFuncNode);
}









