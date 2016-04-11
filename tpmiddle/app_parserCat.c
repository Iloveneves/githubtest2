#include "app_common.h"
#include "app_ipi_common.h"

#define	NO_OF_CRC_DATA_BYTES			4

extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];

static BU32	catChannelID = 0;
static BU32 catFilterID = 0;
static BU32 queueParserCatEnd = 0;
static BU08	isStopParserCat = 0;







BU32 APP_Cat_IPI_GetQueueParserCatEnd(void)
{
	return queueParserCatEnd;
}

void APP_Cat_IPI_ParserCat(BU08* section, CAT_Content_s *catContent,BU32 GetEmm)
{
	BU32 iInfoLength, section_length;
	BU32 iNoOfBytesParsedSoFar;
	BU32 iDesLength = 0;
	BU08 ucCurNextIndicator, ucVersionNumber;
	BU08 ucLastSectionNumber, ucSectionNumber;
	BU08 *paucDataBuff;
	BU16 stTempData8;
	BU08 aucSectionData = 0;
	
	stTempData8 = section[1];
	section_length = ((stTempData8 & 0x0f) << 8) | (section[2]);
	
	ucVersionNumber = (section[5] & 0x3E) >> 1;
	ucCurNextIndicator = section[5] & 0x01;
	ucSectionNumber = section[6];
	ucLastSectionNumber = section[7];
	
	
	iInfoLength = section_length - NO_OF_CRC_DATA_BYTES - 5;
	iNoOfBytesParsedSoFar = 0;
	
	paucDataBuff = &section[8];
	APP_Printf("cat get start\n");
	while (iNoOfBytesParsedSoFar < iInfoLength)
	{
		iDesLength = paucDataBuff[1] + 2;
		
		if (paucDataBuff[0] == SIE_CA_DESC)
		{
			BU16	sCAT_Sys_ID;	
			BU16	wEMMPid = 0 ;
			
			sCAT_Sys_ID = ((short)(paucDataBuff[ 2 ] << 8) )& 0xff00 ;
			sCAT_Sys_ID = sCAT_Sys_ID |paucDataBuff[ 3 ] ;
			
			wEMMPid =( (short)(paucDataBuff[ 4 ]&0x1f))<<8;
			wEMMPid = wEMMPid|paucDataBuff[ 5 ];
			if (1 == FYF_API_ca_get_value(FYF_CA_SV_CAS_ID_VERIRY,(BU32 *)&sCAT_Sys_ID,0))
			{
				catContent->sCAT_Sys_ID = sCAT_Sys_ID;
				catContent->wEMMPid = wEMMPid;
				if(GetEmm == 1)
				{
					FYF_API_SetCurEmmInfo(wEMMPid,sCAT_Sys_ID);
				}
				APP_Printf("cat get id = 0x%x emm = 0x%x\n",sCAT_Sys_ID,wEMMPid);
			}

		}
		paucDataBuff += iDesLength;
		iNoOfBytesParsedSoFar += iDesLength;
	}
	APP_Printf("cat get end\n");
	return;
}

void APP_Cat_IPI_StopDemuxCat(void)
{
	if (isStopParserCat == 0)
	{
		isStopParserCat = 1;
		APP_Parser_IPI_StopDemux(&catChannelID, &catFilterID);
	}
}

void APP_Cat_IPI_StartDemuxCat(APP_ParserFunc parserFunc)
{
	BS16 PID = 0x0001;
	BU08 Coef[1] = {0x01};
	BU08 Mask[1] = {0xff};
	BU08 Excl[1] = {0x00};
	BU08 temp[100]={0};
	struct ParserFuncNode_s parFuncNode;
	FYF_QUEUE_MESSAGE msgRecvTemp;
	
	if(queueParserCatEnd != 0)
	{
		while(FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE != FYF_API_queue_recv(queueParserCatEnd, &msgRecvTemp, FYF_NO_WAIT))
		{}
	}
	else
	{
		queueParserCatEnd = FYF_API_queue_create("qPCatE", 1, FYF_THREAD_WAIT_FIFO);		
	}
	
	APP_Cat_IPI_StopDemuxCat();
	isStopParserCat = 0;
	APP_Parser_IPI_StartDemux(1, PID, Coef, Mask, Excl, &catChannelID, &catFilterID);
	parFuncNode.channelId = catChannelID;
	parFuncNode.filterId = catFilterID;
	parFuncNode.ParserFunc = parserFunc;
	APP_IPI_AddPasFuncNode(parFuncNode);
}

void APP_Cat_IPI_ParCatSetEmm(BU08 *section, BU32 channelId, BU32 filterId)
{
	CAT_Content_s catContent;
	if(0x01 != section[0])
	{
		APP_PRINT_ERROR("APP_Cat_IPI_ParCatSetEmm table_id = 0x%x\n",section[0]);
		return;
	}
	if (isStopParserCat)
	{
		return;
	}
	
	APP_Cat_IPI_StopDemuxCat();
	
	APP_Cat_IPI_ParserCat(section, &catContent,1);
	
}

void APP_Cat_IPI_ParCatOnChangeFreq(BU08 *section, BU32 channelId, BU32 filterId)
{
	CAT_Content_s catContent;

	if(0x01 != section[0])
	{
		APP_PRINT_ERROR("APP_Cat_IPI_ParCatOnChangeFreq table_id = 0x%x\n",section[0]);
		return;
	}
	if (isStopParserCat)
	{
		return;
	}
	
	APP_Cat_IPI_StopDemuxCat();
	
	APP_Cat_IPI_ParserCat(section, &catContent,1);
	//APP_Cat_IPI_SetCatInfo(&catContent);
}

void APP_Cat_IPI_ParCatOnSearchProg(BU08 *section, BU32 channelId, BU32 filterId)
{
	static FYF_QUEUE_MESSAGE msgSend;
	CAT_Content_s catContent;

	if(0x01 != section[0])
	{
		APP_PRINT_ERROR ("APP_Cat_IPI_ParCatOnSearchProg table_id = 0x%x\n",section[0]);
		return;
	}
	if (isStopParserCat)
	{
		return;
	}
	
	APP_Cat_IPI_StopDemuxCat();
	
	APP_Cat_IPI_ParserCat(section, &catContent,0);
	msgSend.qlstWordOfMsg = 1;
	FYF_API_queue_send(APP_Cat_IPI_GetQueueParserCatEnd(), &msgSend);
}

void APP_Cat_API_GetCatSetEmm(void)
{
	APP_Cat_IPI_StartDemuxCat(APP_Cat_IPI_ParCatSetEmm);
}




















