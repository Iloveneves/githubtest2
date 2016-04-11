#include "app_common.h"
#include "app_ipi_common.h"

static BU32 tdtChannelID = 0;
static BU32 tdtFilterID = 0;
BU08 isStopParserTdt = 0;
BU32 iSystemCodeDate = 52954;		/*当天标准的编码日期，此值即2003年11月11日的编码日期 */


void ParserTdt_printf(const char* fmt, ...)
{
	/*
		char			report_buffer[4*1024];
		va_list 	 list;	
		va_start(list, fmt);
		vsprintf(report_buffer, fmt, list);
		va_end(list);
		printf(( report_buffer ));
	*/
}

void APP_TDT_IPI_ParserTdt(BU08* section, TDT_Content_s* tdtContents)
{
	iSystemCodeDate = (section[3] << 8)|section[4];
	FYF_API_parser_tdt(section, tdtContents);
}

/**********************************************************************************
取得系统的iCodeDate(标准时间)
**********************************************************************************/
BU32 APP_TDT_IPI_GetSystemCodedate(void)
{
	return iSystemCodeDate;
}



void APP_TDT_IPI_SaveTdtInfo(TDT_Content_s* tdtContents)
{
	BU32 now_time_zone_type;
	APP_SYS_API_GetSysInfo(APP_SYS_TimeZone_e, &now_time_zone_type);
	if (now_time_zone_type == 0)
	{
		if (tdtContents->hour + APP_CONFIG_TIME_ZONE >= 24)
		{
			APP_IPI_GotoNextDay(&tdtContents->year, &tdtContents->month, &tdtContents->date, &tdtContents->week);
		
		}
		FYF_API_printf("1week = %d, hour = %d, min = %d, sec = %d, year = %d, month = %d, date = %d\n", 
			tdtContents->week, tdtContents->hour + APP_CONFIG_TIME_ZONE, tdtContents->min, tdtContents->sec, 
			tdtContents->year, tdtContents->month, tdtContents->date);
		APP_IPI_SetTime(tdtContents->year, tdtContents->month, tdtContents->date, 
			tdtContents->week, tdtContents->hour + APP_CONFIG_TIME_ZONE, tdtContents->min, tdtContents->sec);
	}
	else
	{
			APP_IPI_SetTime(tdtContents->year, tdtContents->month, tdtContents->date, 
			tdtContents->week, tdtContents->hour, tdtContents->min, tdtContents->sec);
	}
	
}


void APP_TDT_IPI_StartDemuxTdt(void)
{
	BU08 sdtCoef[2] ={0x70,0x73};
	BU08 sdtMask[2] ={0xFF,0xff};
	BU08 sdtExcl[2] ={0x00,0x00};
	struct ParserFuncNode_s parFuncNode;
	BU32 returnValue = 0;


	APP_TDT_IPI_StopDemuxTdt();
	isStopParserTdt = 0;
//	APP_Parser_IPI_StartDemux(0x00010001, 0x0014, sdtCoef, sdtMask, sdtExcl, &tdtChannelID, &tdtFilterID);
	APP_Parser_IPI_StartDemux(0x01, 0x0014, sdtCoef, sdtMask, sdtExcl, &tdtChannelID, &tdtFilterID);
	APP_Printf("tdtChannelID = %d , tdtFilterID= %d\n", tdtChannelID, tdtFilterID);
	parFuncNode.channelId = tdtChannelID;
	parFuncNode.filterId = tdtFilterID;
	parFuncNode.ParserFunc = APP_TDT_IPI_StartParserTdtTot;
	APP_IPI_AddPasFuncNode(parFuncNode);
}



void APP_TDT_IPI_StopDemuxTdt(void)
{
	isStopParserTdt = 1;
	APP_Parser_IPI_StopDemux(&tdtChannelID, &tdtFilterID);
}

void APP_TDT_IPI_StartParserTdtTot(BU08* section, BU32 channelId, BU32 filterId)
{
	APP_Printf("APP_TDT_IPI_StartParserTdtTot channelId = %d, filter = %d, tableid = %d\n", channelId, filterId, section[0]); 
	if (section[0] == 0x70)
		APP_TDT_IPI_StartParserTdt(section);
	else
		APP_TDT_IPI_StartParserTot(section);
	return;
}

void APP_TDT_IPI_StartParserTdt(BU08* section)
{
	static FYF_QUEUE_MESSAGE msgSend;
	TDT_Content_s tdtContents;

	if (isStopParserTdt)
	{
		return;
	}

	APP_TDT_IPI_StopDemuxTdt();

	APP_TDT_IPI_ParserTdt(section, &tdtContents);

	APP_TDT_IPI_SaveTdtInfo(&tdtContents);
	//msgSend.qlstWordOfMsg= 1;
	//FYF_API_queue_send(GetQueueParserSdtEnd(), &msgSend);
}










