#include "app_common.h"
#include "app_ipi_common.h"

extern BU32 g_DayOffset;
extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];
extern BU08 g_Week;

BU32 g_DemuxEitStartTime = 0;
BU08 isAddNewEpg = 0;

static BU08 __glb_SIE_EpgTextBuff[4096]={0};	//event text buffer
static BU08 demuxEitState = Demux_Eit_Sate_Stop;
static BU32 eitChannelID = 0;
static BU32 eitFilterID = 0;

void APP_IPI_GotoNextDay(BU32 *year, BU32 *month, BU32 *date, BU08 *week);

/*-------------------------------------------------------------------------------
	Static Function Declare
-------------------------------------------------------------------------------*/
static BU32 APP_EIT_SIE_GetEventInfo( BU16 service_id,BU08* event_buf, BU32 event_len, BU08 event_type, BU08 table_id);
static BU08 APP_EIT_SIE_Hex2Bcd(BU08 hex);
static BS32 APP_EIT_SIE_EitGetShtDesc(struct EIT_EVENT_DESC *pDsc, BU08 dsc_cnt, BU08* desc_buf, BU16 buf_len);
static BS32 APP_EIT_SIE_EitGetContentDesc(BU08 *content_nibble1, BU08* desc_buf, BU16 buf_len);
static BS32 APP_EIT_SIE_EitGetExtDesc(struct EIT_EVENT_DESC *pDsc, BU08 dsc_cnt, BU08* desc_buf, BU16 buf_len);
static void APP_EIT_SIE_ProcessEventDesc(struct DB_NODE *node,struct EIT_EVENT_DESC *pDes,BU08 desc_cnt);
static BU16 APP_EIT_SIE_Lang3ToLang2(BU08* lang);
static BU08 APP_EIT_SIE_ChIndex(BU08 ch);
static BS32 APP_EIT_SIE_AddEventToDb(struct DB_NODE *node, BU08 table_id);
static BU32 APP_EIT_SIE_ConventDateTime(EitTempDateTime_S *DateTime,struct DB_NODE *node);
static BU32 APP_EIT_SIE_GetStartAndEndTime(EitTempDateTime_S *DateTime,struct DB_NODE *node);
static BU32 APP_EIT_SIE_GetWeekDay(BU08 *WeekDay,BU32 Mjd);
static BU32 APP_EIT_SIE_ConventToStcTime(EitTempDateTime_S *DateTime);	

static BU08 isPauseDemuxEit = 0;
void APP_EIT_IPI_PauseDemuxEit(void)
{
	BU32 returnValue;
	APP_IPI_Semaphore_Wait_Demux();
	if(eitChannelID != 0 && isPauseDemuxEit == 0)
	{
		returnValue = FYF_API_demux_disable_filter(eitChannelID, eitFilterID);
		isPauseDemuxEit = 1;
		//ASSERT(returnValue == 0);
	}
	APP_IPI_Semaphore_Release_Demux();
}

void APP_EIT_IPI_ReStartDemuxEit(void)
{
	BU32 returnValue;
	APP_IPI_Semaphore_Wait_Demux();

	if(eitChannelID != 0 && isPauseDemuxEit == 1)
	{

		returnValue = FYF_API_demux_enable_filter(eitChannelID, eitFilterID);
		isPauseDemuxEit = 0;

		//ASSERT(returnValue == 0);
	}

	APP_IPI_Semaphore_Release_Demux();
}

void APP_EIT_IPI_StartDemuxPFActualEit(void)
{

#ifndef _DO_NOT_GET_EPG_PF
	static BU32 oldFreq = 0;
	BS16 eitPID = 0x0012;
#if 0
	BU08 eitCoef[3] = {0x4e};
	BU08 eitMask[3] = {0xFE,0x0F,0xFF};
	BU08 eitExcl[3] = {0x00,0x00,0x00};
#else
	BU08 eitCoef[1] = {0x4e};
	BU08 eitMask[1] = {0xfe};
	BU08 eitExcl[1] = {0x00};
#endif
	struct ParserFuncNode_s parserFuncNode;
	BU32 i = 0;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
#if 0	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),app_system_data.g_CurChnNo,&ProgInfoListtemp))
	{
		APP_PRINT_ERROR("StartDemuxCurChnAllEit chn over = %d\n",app_system_data.g_CurChnNo);
		return; 
	}
#endif
	
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreEit(), FYF_WAIT_FOREVER);

	g_DemuxEitStartTime = APP_IPI_GetTimeWithSec();

	if(demuxEitState == Demux_Eit_Sate_PF && oldFreq == APP_IPI_GetTunerFreq())
	{

		FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());
		return;
	}

	oldFreq = APP_IPI_GetTunerFreq();

	APP_EIT_IPI_StopDemuxEit();
#if 0
	eitCoef[1] = SeparateSevId(ProgInfoListtemp.m_ProgId) >> 8;
	eitCoef[2] = SeparateSevId(ProgInfoListtemp.m_ProgId) & 0xFF;
#endif
	APP_Parser_IPI_StartDemux(1, eitPID, eitCoef, eitMask, eitExcl, &eitChannelID, &eitFilterID);
	parserFuncNode.channelId = eitChannelID;
	parserFuncNode.filterId = eitFilterID;
	parserFuncNode.ParserFunc = APP_EIT_IPI_StartParserEit;
    APP_IPI_AddPasFuncNode(parserFuncNode);
	demuxEitState = Demux_Eit_Sate_PF;

	FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());
#endif

}

void APP_EIT_IPI_StartDemuxCurChnAllEit(void)
{

#ifndef _DO_NOT_GET_EPG_ALL
	static BU32 oldChnNo = 99999;
	BS16 eitPID = 0x0012;
#if (GUI_PLATFORM == Hisi)
	BU08 eitCoef[3] = {0x50, 0x00, 0x00};
	BU08 eitMask[3] = {0xF0, 0x0F, 0xFF};
	BU08 eitExcl[3] = {0x00, 0x00, 0x00};
#else
	BU08 eitCoef[5] = {0x50, 0x00, 0x00, 0x00, 0x00};
	BU08 eitMask[5] = {0xF0, 0x00, 0x00, 0x0F, 0xFF};
	BU08 eitExcl[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
#endif
	struct ParserFuncNode_s parserFuncNode;

	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&ProgInfoListtemp))
	{
		APP_PRINT_ERROR("StartDemuxCurChnAllEit chn over = %d\n",APP_ChnDatabase_API_GetCurChnNO());
		APP_EIT_IPI_StopDemuxEit();
		return; 
	}
	
	
	if(oldChnNo == app_system_data.g_CurChnNo 
		&& demuxEitState != Demux_Eit_Sate_PF 
		&& demuxEitState != Demux_Eit_Sate_Stop)
	{

		return;
	}

	oldChnNo = app_system_data.g_CurChnNo;
	
	g_DemuxEitStartTime = APP_IPI_GetTimeWithSec();
	
	APP_EIT_IPI_StopDemuxEit();
#if (GUI_PLATFORM == Hisi)
	eitCoef[1] = SeparateSevId(ProgInfoListtemp.m_ProgId) >> 8;
	eitCoef[2] = SeparateSevId(ProgInfoListtemp.m_ProgId) & 0xFF;
	APP_Parser_IPI_StartDemux(3, eitPID, eitCoef, eitMask, eitExcl, &eitChannelID, &eitFilterID);
#else
	eitCoef[3] = SeparateSevId(ProgInfoListtemp.m_ProgId) >> 8;
	eitCoef[4] = SeparateSevId(ProgInfoListtemp.m_ProgId) & 0xFF;
	APP_Parser_IPI_StartDemux(5, eitPID, eitCoef, eitMask, eitExcl, &eitChannelID, &eitFilterID);
#endif
	parserFuncNode.channelId = eitChannelID;
	parserFuncNode.filterId = eitFilterID;
	parserFuncNode.ParserFunc = APP_EIT_IPI_StartParserEit;
    APP_IPI_AddPasFuncNode(parserFuncNode);
	demuxEitState = Demux_Eit_Sate_CurChn_All;
#else
	APP_EIT_IPI_StopDemuxEit();
#endif


}


void APP_EIT_IPI_StartDemuxAllEit(void)
{
#ifndef _DO_NOT_GET_EPG_ALL
	static BU32 oldFreq = 0;
	BS16 eitPID = 0x0012;
	BU08 eitCoef[1] = {0x50};
	BU08 eitMask[1] = {0xF0};
	BU08 eitExcl[1] = {0x00};
	struct ParserFuncNode_s parserFuncNode;

	if(demuxEitState == Demux_Eit_Sate_All && oldFreq == APP_IPI_GetTunerFreq())
	{
		return;
	}
	g_DemuxEitStartTime = APP_IPI_GetTimeWithSec();
	FYF_API_printf("void StartDemuxAllEit(void) set g_DemuxEitStartTime = %d\n", g_DemuxEitStartTime);

	oldFreq = APP_IPI_GetTunerFreq();

	APP_EIT_IPI_StopDemuxEit();
	APP_Parser_IPI_StartDemux(1, eitPID, eitCoef, eitMask, eitExcl, &eitChannelID, &eitFilterID);
	parserFuncNode.channelId = eitChannelID;
	parserFuncNode.filterId = eitFilterID;
	parserFuncNode.ParserFunc = APP_EIT_IPI_StartParserEit;
    APP_IPI_AddPasFuncNode(parserFuncNode);
	demuxEitState = Demux_Eit_Sate_All;
#else
	APP_EIT_IPI_StopDemuxEit();
#endif

}


void APP_EIT_IPI_StopDemuxEit(void)
{
	demuxEitState = Demux_Eit_Sate_Stop;
	
	if(eitChannelID != 0 || eitFilterID != 0)
	{
		if(isPauseDemuxEit == 1)
			APP_EIT_IPI_ReStartDemuxEit();
		APP_Parser_IPI_StopDemux(&eitChannelID, &eitFilterID);
	}
}



void APP_EIT_IPI_SavePFEitInfo(EIT_Content_s *eitContent)
{
	struct epgInfo_s epgInfo;

	epgInfo.eventId = eitContent->event_id;
	epgInfo.startTime = GetTime(eitContent->startHour, eitContent->startMinute);
	epgInfo.endTime = GetTime(eitContent->endHour, eitContent->endMinute);
	memcpy(epgInfo.strEpgInfo, eitContent->eventName, APP_EPG_NAME_LEN);
	epgInfo.strEpgInfo[APP_EPG_NAME_LEN - 1] = '\0';
	DBA_API_AddCurNextEpg(JoinSevIdFreq(eitContent->service_id, APP_IPI_GetTunerFreq()),  &epgInfo, eitContent->eventType);
}

BU08 isNowPrintTime = 0;
BU32 saveEpgCostTime = 0;
void APP_EIT_IPI_SaveAllEitInfo(EIT_Content_s *eitContent)
{
	struct epgInfo_s epgInfo;
	//不合法的详细信息
  	if (eitContent->hasDetail && strcmp(eitContent->detailInfo, eitContent->eventName) == 0)	
  	{
		eitContent->hasDetail = 0;
	}
	epgInfo.eventId = eitContent->event_id;
	epgInfo.reserve = 0;
	epgInfo.week = eitContent->week;
	//app_printf("eitContent->week = %d\n",eitContent->week);
	epgInfo.startTime = GetTime(eitContent->startHour, eitContent->startMinute);
	epgInfo.endTime = GetTime(eitContent->endHour, eitContent->endMinute);
	epgInfo.hasDetail = eitContent->hasDetail;
	epgInfo.day = eitContent->day;
	memcpy(epgInfo.strEpgInfo, eitContent->eventName, APP_EPG_NAME_LEN - 1);
	epgInfo.strEpgInfo[APP_EPG_NAME_LEN - 1] = '\0';
	if(DBA_API_AddEpg(JoinSevIdFreq(eitContent->service_id, APP_IPI_GetTunerFreq()), eitContent->week, &epgInfo) != 0)
		{isAddNewEpg = 1;}
		/*添加详细信息*/
#if 0
	if(eitContent->hasDetail != 0)
	{
		sprintf(g_StrSQL, "INSERT INTO detail (service_id, eventId, week, detailInfo) VALUES(%d, %d, %d, '%s');",
			JoinSevIdFreq(eitContent->service_id, APP_IPI_GetTunerFreq()), eitContent->event_id, eitContent->week, eitContent->detailInfo);
		APP_Printf("len = %d, %s\n", strlen(eitContent->detailInfo),g_StrSQL);
		{
			static BU32 detailCount = 0;
			detailCount++;
			APP_Printf("===========================detailCount = %d\n", detailCount);
		}
		SQL_API_Exec(g_StrSQL);
	}
#endif			
	if(eitContent->hasDetail != 0)
	{
		DBA_API_AddDetail(eitContent);
	}

}



/*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:eit表解释函数入口
History:
========================================================*/
BU32 ParserEit(BU08* buf)
{
	BU08 table_id=0,sec_num=0,event_type=0;
	BU16 sec_len=0,service_id=0;
	if(buf == NULL)
	{
		return 0;
	}

	table_id = buf[0];
	if(table_id < 0x4E || table_id > 0x6F)
	{
		APP_PRINT_ERROR("no eit pid =0x%x\n",table_id);
		return 0;
	}
	sec_len = ((buf[1]&0x0F)<<8) | buf[2];

	if (sec_len < 18 || sec_len > 4096)
	{
		return 0;
	}

	service_id = (buf[3]<<8) | buf[4];
	sec_num = buf[6];
	
	event_type = IS_PF(table_id)?sec_num:SCHEDULE_EVENT;
	APP_EIT_SIE_GetEventInfo(service_id, buf+14, sec_len-15/*no CRC*/, event_type, table_id);
	return 1;
}

 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:解析事件信息函数
History:
========================================================*/
static BU32 APP_EIT_SIE_GetEventInfo( BU16 service_id,BU08* event_buf, BU32 event_len, BU08 event_type, BU08 table_id)
{
	BU08 desc_len=0,desc_tag=0, nibble=0,rating=0,stream_content=0;
	BU16 event_loop_start = 0,desc_loop_start = 0,desc_loop_len=0;

	BU08 *p = event_buf;
	struct DB_NODE event_node;
	struct EIT_EVENT_DESC Desc[SI_MAX_EPG_LANGUAGE_COUNT];
	//process event loop
	do
	{
		//init event node
		memset(&event_node, 0, sizeof(event_node));
		event_node.service_id = service_id;
		event_node.event_type = event_type;

		//get event info
		event_node.event_id = *p<<8 | *(p+1);
		p += 2;
		
		event_node.mjd_num = *p<<8 | *(p+1);
		p += 2;
		
		event_node.time.hour = APP_EIT_SIE_Hex2Bcd(*p++);
		event_node.time.minute = APP_EIT_SIE_Hex2Bcd(*p++);
		event_node.time.second = APP_EIT_SIE_Hex2Bcd(*p++);

		event_node.duration.hour = APP_EIT_SIE_Hex2Bcd(*p++);
		event_node.duration.minute = APP_EIT_SIE_Hex2Bcd(*p++);
		event_node.duration.second = APP_EIT_SIE_Hex2Bcd(*p++);
		
		desc_loop_len = ((*p&0x0f)<<8) | *(p+1);
		p += 2;

		event_loop_start += 12;
		if ((BU32)(event_loop_start+desc_loop_len) > event_len)
		{
			return !APP_SUCCESS;		
		}

		//process event's desc loop
		desc_loop_start = 0;
		memset(Desc, 0, sizeof(Desc));
		
		while (desc_loop_start < desc_loop_len)
		{
			desc_tag = *p;
			desc_len = *(p+1);

			if (desc_loop_start+desc_len+2 <= desc_loop_len)
			{
				switch (desc_tag)
				{
					case 0x4D:
						APP_EIT_SIE_EitGetShtDesc(Desc, SI_MAX_EPG_LANGUAGE_COUNT, p, desc_len+2);
						break;
					case CONTENT_DESC:
						APP_EIT_SIE_EitGetContentDesc(&nibble, p, desc_len+2);
						event_node.nibble = nibble;
						break;
					case EXTENTED_EVENT_DESC:
						APP_EIT_SIE_EitGetExtDesc(Desc, SI_MAX_EPG_LANGUAGE_COUNT, p, desc_len+2);
						break;
					default:
						break;
				}
			}

			p += (desc_len+2);
			desc_loop_start += (desc_len+2);
		}

		//process short/extended event descriptor's language code ,event name & text char
		APP_EIT_SIE_ProcessEventDesc(&event_node, Desc, SI_MAX_EPG_LANGUAGE_COUNT);

		//add this event into epg db
		if (FYF_ERR == APP_EIT_SIE_AddEventToDb(&event_node, table_id))
		{
			//add some code
		}
		event_loop_start += desc_loop_len;
		
	} while(event_loop_start < event_len);
	return SUCCESS;
}
 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:
History:
========================================================*/
static BU08 APP_EIT_SIE_Hex2Bcd(BU08 hex)
{
	return ((hex >> 4) * 10 + (hex & 0x0f));
}
 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:获得短事件描述符
History:
========================================================*/
static BS32 APP_EIT_SIE_EitGetShtDesc(struct EIT_EVENT_DESC *pDsc, BU08 dsc_cnt, BU08* desc_buf, BU16 buf_len)
{
	BS32 i=0, n = -1;
	BU08 *lang = desc_buf+2;

	if (*(desc_buf+1) + 2 <= buf_len && *(desc_buf+1) >= 3)
	{
		for (i=0; i<dsc_cnt; i++)
		{
			if (pDsc[i].lang != NULL&& memcpy(lang, pDsc[i].lang, 3) == 0)
			{
				if (pDsc[i].sht_text != NULL)
				{
					return ERR_FAILUE;
				}   		 
				else
				{
                   			 //maybe sometime get ext_desc before sht_desc
                 				   n = i;
                   			 break;
				}
			}
			else if (pDsc[i].lang == NULL && n == -1)
			{
				n = i;
			}		
		}
		
		if (n != -1)
		{
			pDsc[n].sht_len = *(desc_buf+1) - 3;
			pDsc[n].lang = lang;
			pDsc[n].sht_text = desc_buf + 5;
			return SUCCESS;
		}	
	}

	return ERR_FAILUE;
}

 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:parse content descriptor
History:
========================================================*/
static BS32 APP_EIT_SIE_EitGetContentDesc(BU08 *content_nibble1, BU08* desc_buf, BU16 buf_len)
{
	*content_nibble1 = (*(desc_buf+2)) >> 4;
	
	return SUCCESS;
}

 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:parse extended event descriptor
History:
========================================================*/
static BS32 APP_EIT_SIE_EitGetExtDesc(struct EIT_EVENT_DESC *pDsc, BU08 dsc_cnt, BU08* desc_buf, BU16 buf_len)
{
	BU08  item_len=0,desc_num=0;
	BS32 i=0, n = -1;
	BU08 *lang=0;

	if (*(desc_buf+1) + 2 > buf_len || *(desc_buf+1) < 3)
	{
		return ERR_FAILUE;
	}

	desc_num = desc_buf[2] >> 4;
	lang = desc_buf + 3;


//    soc_printf("lang: %c%c%c\n",lang[0],lang[1],lang[2]);
	for (i=0; i<dsc_cnt; i++)
	{
		if (pDsc[i].lang != NULL&& memcpy(lang, pDsc[i].lang, 3) == 0)
		{
			n = i;
			break;
		}
		else if (pDsc[i].lang == NULL && n == -1)
		{
			n = i;
		}			
	}
	
	if (n != -1)
	{
		if (pDsc[n].lang == NULL)
		{
			pDsc[n].lang = lang;
		}
			
		item_len = *(desc_buf+6);
		pDsc[n].ext[desc_num].len = *(desc_buf+item_len+7);
		pDsc[n].ext[desc_num].text = desc_buf+item_len+8;
//        		soc_printf("n: %d, desc_num: %d, len: %d\n",n,desc_num,pDsc[n].ext[desc_num].len);
		return SUCCESS;
	}

	return ERR_FAILUE;
}

 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:copy short/extend event descriptors' lang/name/text to the event node
History:
========================================================*/
static void APP_EIT_SIE_ProcessEventDesc(struct DB_NODE *node,struct EIT_EVENT_DESC *pDes,BU08 desc_cnt)
{
	BS32 i=0, j=0, n = 0/*language number of node*/;
	BU08  *text_char=NULL , *ext_text_char = NULL;
	BU16   text_length=0, ext_text_len=0;
	
	text_char = __glb_SIE_EpgTextBuff;

	for (i=0; i<desc_cnt; i++)
	{
		if (pDes[i].lang != NULL && n < SI_MAX_EPG_LANGUAGE_COUNT)
		{
			node->lang[n].lang_code2 = APP_EIT_SIE_Lang3ToLang2(pDes[i].lang);
			text_length = 0;
			ext_text_len = 0;
			ext_text_char = NULL;
			
			//short event descriptor
			if (pDes[i].sht_len != 0 && pDes[i].sht_text != NULL)
			{
				memcpy(text_char, pDes[i].sht_text, pDes[i].sht_len);
				text_char += pDes[i].sht_len;
				text_length = pDes[i].sht_len;
			}
			//extend event descriptor
			for (j=0; j<16; j++)
			{
				if (pDes[i].ext[j].len == 0 || pDes[i].ext[j].text == NULL)
				{
					continue;
				}
					
				if (text_length == 0)	//no short event descriptor
				{
					text_char[0] = text_char[1] = 0;
					text_char += 2;
					text_length = 2;
				}
				//if (j == 0)	//first extend event descriptor
				if (ext_text_char == NULL)
				{
					ext_text_char = text_char;
					text_char += 2;
					text_length += 2;
				}
				
				memcpy(text_char, pDes[i].ext[j].text, pDes[i].ext[j].len);
				text_char += pDes[i].ext[j].len;
				ext_text_len += pDes[i].ext[j].len;
				text_length += pDes[i].ext[j].len;
				
			}
			//node: text_char & text_length
			if (text_length > 0)
			{
				if (ext_text_len == 0)	//no extend event descriptor
				{
					ext_text_char = text_char;
					text_char += 2;
					text_length += 2;
				}
				//extend event descriptor text length
				ext_text_char[0] = ext_text_len >> 8;
				ext_text_char[1] = ext_text_len & 0xFF;
				//all event name + short text + extend text
				node->lang[n].text_char = text_char - text_length;
			}
			else
			{
				node->lang[n].text_char = NULL;
			}
			
			node->lang[n].text_length = text_length;
			n++;
		}
	}
}

 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:
History:
========================================================*/
static BU16 APP_EIT_SIE_Lang3ToLang2(BU08* lang)
{
	return ((APP_EIT_SIE_ChIndex(lang[0])<<10) | (APP_EIT_SIE_ChIndex(lang[1])<<5) | APP_EIT_SIE_ChIndex(lang[2]));
}
 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:
History:
========================================================*/
static BU08 APP_EIT_SIE_ChIndex(BU08 ch)
{
	if (ch >= 'a' && ch <= 'z')
	{
		return ch - 'a';
	}
	else if (ch >= 'A' && ch <= 'Z')
	{
		return ch - 'A';
	}	
	else
	{
		return 0;	//error
	}		
}

BU08 isNeedSaveThisEpg(EIT_Content_s* eitContent)
{
	if(eitContent->day < APP_IPI_GetDay())
		return 0;
	return 1;
}

BU32 APP_EIT_SIE_ConventDay(BU32 hour,BU16 mjd)
{
	BU32 Y,M,K;
	BU32 D;
	BU08 tempWeek = 0;
	
	BU32 now_time_zone_type;
	APP_SYS_API_GetSysInfo(APP_SYS_TimeZone_e, &now_time_zone_type);
	Y = (BU16)((mjd -15078.2) / 365.25);
	M = (BU16)((mjd -14956.1 - (BU16)(Y * 365.25))/ 30.6001);
	D = mjd -14956 - (BU16)(Y * 365.25) - (BU16)(M * 30.6001);

	if(M == 14 || M == 15)
		K = 1;
	else
		K = 0;
	Y = Y + K + 1900;
	M = M - 1 - K * 12;
	if (now_time_zone_type == 0)
	{
		if(hour < APP_CONFIG_TIME_ZONE)
		{
			APP_IPI_GotoNextDay(&Y, &M, &D, &tempWeek);
		}
	}
	D = 10000 * Y + M * 100 + D;
	return D;
}
 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:
History:
========================================================*/
static BS32 APP_EIT_SIE_AddEventToDb(struct DB_NODE *node, BU08 table_id)
{
	BU32 weekReal = 0;

	EIT_Content_s eitContent;
	BU16 NameLen=0,TextLen=0;
	BU16 NameLenUse = 0;
	BU08 NameStr[EVENT_NAME_LENGTH]={0};
	EitTempDateTime_S DateTime;

	if(node->lang[0].text_char != 0)
	{
		NameLen = node->lang[0].text_char[0];
		NameLenUse = NameLen;
		if(NameLenUse >= EVENT_NAME_LENGTH)
		{
			NameLenUse = EVENT_NAME_LENGTH - 1;
		}			
		memcpy(NameStr,node->lang[0].text_char+1,NameLenUse);
		NameStr[NameLenUse] = '\0';
		//app_printf("%s \n",NameStr);

		NameLenUse = node->lang[0].text_char[NameLen+1];
		if(NameLenUse != 0)
		{
			eitContent.hasDetail = 1;
			if(NameLenUse >= DETAIL_INFO_LENGTH)
			{
				NameLenUse = DETAIL_INFO_LENGTH - 1;
			}
			memcpy(eitContent.detailInfo, node->lang[0].text_char + NameLen + 2, NameLenUse);
			eitContent.detailInfo[NameLenUse] = '\0';
		}
		else
		{
			eitContent.hasDetail = 0;
		}
	}
	else
	{

		return FYF_ERR;
	}

	APP_EIT_SIE_ConventDateTime(&DateTime, node);
	
	eitContent.service_id = node->service_id;
	eitContent.event_id = node->event_id;
	eitContent.eventType = node->event_type;
	eitContent.startHour = DateTime.StartHour;
	eitContent.startMinute = DateTime.StartMin;
	eitContent.endHour = DateTime.EndHour;
	eitContent.endMinute = DateTime.EndMin;
	eitContent.week = DateTime.StartWeekDay;
	eitContent.day = APP_EIT_SIE_ConventDay(eitContent.startHour, node->mjd_num);
	memcpy(eitContent.eventName, NameStr, EVENT_NAME_LENGTH);
	eitContent.eventName[EVENT_NAME_LENGTH - 1] = '\0';

	if(APP_EPG_IPI_Semaphore_Wait(100) == 0)
	{
		return FYF_ERR;
	}
	if(table_id >= 0x50)
	{
		//if(isNeedSaveThisEpg(&eitContent))/*今天第一条是跨天*/
		{
			APP_EIT_IPI_SaveAllEitInfo(&eitContent);
		}
	}
	else
	{
		APP_EIT_IPI_SavePFEitInfo(&eitContent);
	}
	APP_EPG_IPI_Semaphore_Release();
	return FYF_OK;

}
 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:
History:
========================================================*/
static BU32 APP_EIT_SIE_ConventDateTime(EitTempDateTime_S *DateTime,struct DB_NODE *node)
{
	BU32 now_time_zone_type;
	APP_SYS_API_GetSysInfo(APP_SYS_TimeZone_e, &now_time_zone_type);
	APP_EIT_SIE_GetStartAndEndTime(DateTime,node);
	if (now_time_zone_type == 0)
	{
	APP_EIT_SIE_ConventToStcTime(DateTime);	
	
	if(DateTime->StartHour < APP_CONFIG_TIME_ZONE)
		DateTime->StartWeekDay = DateTime->StartWeekDay + 1;
	if(DateTime->StartWeekDay == 8)
		DateTime->StartWeekDay = 1;
	
	if(DateTime->EndHour < APP_CONFIG_TIME_ZONE)
		DateTime->EndWeekDay = DateTime->EndWeekDay + 1;
	if(DateTime->EndWeekDay == 8)
		DateTime->EndWeekDay = 1;
	}
	return 0;
}
 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:
History:
========================================================*/
static BU32 APP_EIT_SIE_GetStartAndEndTime(EitTempDateTime_S *DateTime,struct DB_NODE *node)
{
 	//start time
	DateTime->StartMjd = node->mjd_num;
	DateTime->StartHour = node->time.hour;
	DateTime->StartMin = node->time.minute;
	DateTime->StartSec = node->time.second;
	APP_EIT_SIE_GetWeekDay(&DateTime->StartWeekDay,DateTime->StartMjd);


	//end time
	DateTime->EndMjd = node->mjd_num;
	DateTime->EndHour = node->time.hour;
	DateTime->EndMin = node->time.minute;
	DateTime->EndSec = node->time.second;
	APP_EIT_SIE_GetWeekDay(&DateTime->EndWeekDay,DateTime->EndMjd);
	
	DateTime->EndSec += node->duration.second;
	DateTime->EndMin += node->duration.minute;
	DateTime->EndHour += node->duration.hour;
	DateTime->EndMin += (DateTime->EndSec / 60);
	DateTime->EndSec = DateTime->EndSec % 60;
	DateTime->EndHour += (DateTime->EndMin / 60);
	DateTime->EndMin %= 60;
	DateTime->EndMjd += (DateTime->EndHour / 24);
	DateTime->EndHour %= 24;
	return 0;
	
}
 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:
History:
========================================================*/
static BU32 APP_EIT_SIE_GetWeekDay(BU08 *WeekDay,BU32 Mjd)
{
	if (Mjd <= 15078)   //fix bug:stream's mjd error
	{
 			Mjd = 53371;    //set to default:2005/1/1
	}

	*WeekDay = ((Mjd + 2) % 7) + 1;
	return 0;
}
 /*========================================================
File Name:epg_demo.c
Author:   
Create Time:
Description:
History:
========================================================*/
static BU32 APP_EIT_SIE_ConventToStcTime(EitTempDateTime_S *DateTime)
{
	DateTime->StartHour = (DateTime->StartHour+APP_CONFIG_TIME_ZONE+24)%24;
	DateTime->EndHour = (DateTime->EndHour+APP_CONFIG_TIME_ZONE+24)%24;
	return 0;
}
 
void APP_EIT_IPI_StartEventEit(BU08 *section, BU32 channelId, BU32 filterId)
{
 BU32 nowTime = 0;
 BU32 sameEPG = 0;
 

 nowTime = APP_IPI_GetTimeWithSec();

 FYF_API_semaphore_wait(APP_IPI_GetSemaphoreEit(), FYF_WAIT_FOREVER);


 if (demuxEitState == Demux_Eit_Sate_PF 
 	 && g_DemuxEitStartTime != 0 
	 && ((nowTime - g_DemuxEitStartTime) > 3 || nowTime < g_DemuxEitStartTime))
 {

	 APP_EIT_IPI_StartDemuxCurChnAllEit();

	 FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());

	 return;
 }

 if(demuxEitState == Demux_Eit_Sate_All 
 	 && g_DemuxEitStartTime != 0 
	 && (nowTime - g_DemuxEitStartTime) > 60)
 {

	 g_DemuxEitStartTime = 0;
// 	 APP_EIT_IPI_StopDemuxEit();
	 FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());

	 APP_EIT_IPI_StartDemuxPFActualEit();


	 return;
 }

 if(demuxEitState == Demux_Eit_Sate_CurChn_All
 	 && g_DemuxEitStartTime != 0 
	 && (nowTime - g_DemuxEitStartTime) > 3)
 {

	 g_DemuxEitStartTime = 0;
	 APP_EIT_IPI_StartDemuxAllEit();

	 FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());


	 return;
 }
 FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());


}

void APP_EIT_IPI_StartParserEit(BU08 *section, BU32 channelId, BU32 filterId)
{
	
	BU32 nowTime = 0;
	BU32 sameEPG = 0;
	//APP_EIT_IPI_StartEventEit(section, channelId, filterId);
 
	 nowTime = APP_IPI_GetTimeWithSec();

	 FYF_API_semaphore_wait(APP_IPI_GetSemaphoreEit(), FYF_WAIT_FOREVER);


	 if (demuxEitState == Demux_Eit_Sate_PF 
	 	 && g_DemuxEitStartTime != 0 
		 && ((nowTime - g_DemuxEitStartTime) > 10 || nowTime < g_DemuxEitStartTime))
	 {

		 APP_EIT_IPI_StartDemuxCurChnAllEit();

		 FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());

		 return;
	 }

	 if(demuxEitState == Demux_Eit_Sate_All 
	 	 && g_DemuxEitStartTime != 0 
		 && (nowTime - g_DemuxEitStartTime) > 40)
	 {

		 g_DemuxEitStartTime = 0;
	// 	 APP_EIT_IPI_StopDemuxEit();
		 FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());
		 APP_EIT_IPI_StartDemuxPFActualEit();

		 return;
	 }

	 if(demuxEitState == Demux_Eit_Sate_CurChn_All
	 	 && g_DemuxEitStartTime != 0 
		 && (nowTime - g_DemuxEitStartTime) > 10)
	 {
		 g_DemuxEitStartTime = 0;
		 APP_EIT_IPI_StartDemuxAllEit();
		 FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());
		 return;
	 }

	 FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());
	/*
	解表
	*/

	sameEPG = ParserEit(section);
}

















