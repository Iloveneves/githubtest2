#include "app_common.h"
#include "app_ipi_common.h"

static BU32	totChannelID = 0;
static BU32 totFilterID = 0;
static BU08 isStopParserTot = 0;
extern BU32 iSystemCodeDate;

void APP_TDT_IPI_SaveTotInfo(TOT_Content_s *totContents)
{
	FYF_API_printf("week = %d, hour = %d, min = %d, sec = %d, year = %d, month = %d, date = %d\n",
		totContents->week, totContents->hour + totContents->timeOffset, totContents->min, totContents->sec, totContents->year, totContents->month, totContents->date);
//	APP_IPI_SetTime(totContents->year, totContents->month,totContents->date, totContents->week, totContents->hour + APP_CONFIG_TIME_ZONE, totContents->min, totContents->sec);
}

void APP_TDT_IPI_StartDemuxTot(void)
{
	BU08 Coef[1] = {0x73};
	BU08 Mask[1] = {0xFF};
	BU08 Excl[1] = {0x00};
	struct ParserFuncNode_s parFuncNode;
	BU32 returnValue = 0;
	
	if(totChannelID != 0 || totFilterID != 0)
		APP_TDT_IPI_StopDemuxTot();
	isStopParserTot = 0;
	APP_Parser_IPI_StartDemux(1, 0x0014, Coef, Mask, Excl, &totChannelID, &totFilterID);
	parFuncNode.channelId = totChannelID;
	parFuncNode.filterId = totFilterID;
	parFuncNode.ParserFunc = APP_TDT_IPI_StartParserTdtTot;
	APP_IPI_AddPasFuncNode(parFuncNode);

}

void APP_TDT_IPI_StopDemuxTot(void)
{
	isStopParserTot = 1;
	APP_Parser_IPI_StopDemux(&totChannelID, &totFilterID);
}

/*
将CodeDate转换为年月日
*/
void APP_TDT_TransCodeDateToNormalDate(int iCodedDate, short int *psYear, signed char *pcMonth, signed char *pcDate)
{
	int y1, m1, d, wd, k, y, m, test1, test2;
	
	/*计算并显示码流中提取出来的时间信息 */
	y1 = ((iCodedDate - 15078.2) / 365.25) / 1;
	test1 = y1 * 365.25 / 1;
	m1 = ((iCodedDate - 14956.1 - test1) / 30.6001) / 1;
	test2 = m1 * 30.6001 / 1;
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
}
/*
*/
BU32 APP_TDT_IPI_ParserTot(BU08 * section, TOT_Content_s *totContents)
{
	BU16 length;
	int iCodeDate;
	short int psYear;
	signed char pcMonth;
	signed char pcDate;
	if(0x73!= section[0])
	{
		return FYF_ERR;
	}
	
	iCodeDate = (section[3] << 8)|section[4];
	iSystemCodeDate = iCodeDate;
	totContents->hour=(section[5]&0x0f)+((section[5]>>4)&0x0f)*10;
	totContents->min=(section[6]&0x0f)+((section[6]>>4)&0x0f)*10;
	totContents->sec=(section[7]&0x0f)+((section[7]>>4)&0x0f)*10;
	APP_TDT_TransCodeDateToNormalDate(iCodeDate,&psYear,&pcMonth,&pcDate);
	totContents->week = (iCodeDate+2)%7+1; 
	totContents->year = psYear;
	totContents->month = pcMonth;
	totContents->date = pcDate;

	length = ((section[8]&0x0f)<<8)|section[9];
	if(length < 15)
	{
		totContents->timeOffset = 0;
		return APP_SUCCESS;
	}
	else
	{
		totContents->timeOffset = section[16] << 8| section[17];
	}
	return APP_SUCCESS;
}


extern BU08 isStopParserTdt;

void APP_TDT_IPI_StartParserTot(BU08 *section)
{
	static FYF_QUEUE_MESSAGE msgSend;
	TOT_Content_s totContents;

#if 0	/* szhy:09-04-09 */
	if (isStopParserTot)
#else
	if (isStopParserTdt)
#endif
	{
		return;
	}

#if 0/* szhy:09-04-09 */
	APP_TDT_IPI_StopDemuxTot();
#else
	APP_TDT_IPI_StopDemuxTdt();
#endif

	APP_TDT_IPI_ParserTot(section, &totContents);
	
	APP_TDT_IPI_SaveTotInfo(&totContents);
	//msgSend.qlstWordOfMsg= 1;
	//FYF_API_queue_send(GetQueueParserSdtEnd(), &msgSend);
}










