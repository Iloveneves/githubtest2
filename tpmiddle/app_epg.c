#include <stdio.h>
#include <time.h>
#include "app_common.h"
#include "app_ipi_common.h"

extern BU08 isAddNewEpg;
extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];

extern BU32 g_Year;
extern BU32 g_Month;
extern BU32 g_Date;
extern BU32 g_Minute;
extern BU32 g_Hour;
extern BU08 g_Week;

extern BU32 g_DemuxEitStartTime;

Epg_Info_S g_EpgInfo[APP_EPG_MAX_DAY_COUNT];
BU32 g_DayOffset = 0;//记录上次查询的星期

ReservedList epg_reservedlist[MAX_RESERVE_NUM]; //临时存放预约事件的部分信息
int g_RerservedCount = 0;//记录当前预约的总个数
BU08 bReserveChange = true;

/*-------------------------------------------------------------------------------

	Description:
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_API_StopGetEpg(void)
{
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreEit(), FYF_WAIT_FOREVER);
	APP_EIT_IPI_StopDemuxEit();
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());
}

/*-------------------------------------------------------------------------------

	Description:获得当前下一事件信息
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_GetCurAndNextEvent(BU32 ChnNo, Epg_Info_S* CurEpgInfo, Epg_Info_S* NextEpgInfo)
{
	BU32 serviceId;
	BU08 eventName[APP_EPG_NAME_LEN];
	struct progInfoNode_s* pProgInfoNode = 0;
	BU32 sH, sM, eH, eM;
	BU32 epgCount = 0;
	BU32 epgIndex = 0;
	Epg_Info_S epgInfo;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	
	if (CurEpgInfo == 0 || NextEpgInfo == 0)
	{
		APP_PRINT_ERROR("APP_EPG_API_GetCurAndNextEvent  input empty \n");
		return APP_FAIL;
	}
	
	memset(CurEpgInfo, 0, sizeof(Epg_Info_S));
	memset(NextEpgInfo, 0, sizeof(Epg_Info_S));
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), ChnNo, &ProgInfoListtemp))
	{
		APP_PRINT_ERROR("GetCurAndNextEvent chn  over = %d\n",APP_ChnDatabase_API_GetCurChnNO());
		return APP_FAIL; 
	}


	APP_EPG_IPI_Semaphore_Wait(FYF_WAIT_FOREVER);

	serviceId = ProgInfoListtemp.m_ProgId;
	pProgInfoNode = DBA_API_GetProgInfoNode(serviceId, 1);
	if (pProgInfoNode == 0)
	{
		APP_EPG_IPI_Semaphore_Release();
		return APP_FAIL;
	}

	if (pProgInfoNode->progInfo.pCurEpgNode != 0)// && pProgInfoNode->progInfo.pCurEpgNode->epgInfo.eventId != 0)
	{
		memcpy(CurEpgInfo->epgName, pProgInfoNode->progInfo.pCurEpgNode->epgInfo.strEpgInfo, APP_EPG_NAME_LEN);
		CurEpgInfo->epgName[APP_EPG_NAME_LEN - 1] = '\0';
		CurEpgInfo->startHour = GetHour(pProgInfoNode->progInfo.pCurEpgNode->epgInfo.startTime);
		CurEpgInfo->startMin = GetMinute(pProgInfoNode->progInfo.pCurEpgNode->epgInfo.startTime);
		CurEpgInfo->endHour = GetHour(pProgInfoNode->progInfo.pCurEpgNode->epgInfo.endTime);
		CurEpgInfo->endMin = GetMinute(pProgInfoNode->progInfo.pCurEpgNode->epgInfo.endTime);

		if(GetTime(CurEpgInfo->startHour, CurEpgInfo->startMin) > APP_IPI_GetTime()
			|| GetTime(CurEpgInfo->endHour, CurEpgInfo->endMin) < APP_IPI_GetTime())
		{
			APP_EPG_IPI_Semaphore_Release();
			if (APP_FAIL == APP_EPG_API_GetEpgCountAndIndex(ChnNo, 0,&epgCount,&epgIndex))
			{
				APP_PRINT_ERROR("can not get epg count and index\n");
			}
			APP_EPG_IPI_Semaphore_Wait(FYF_WAIT_FOREVER);
			if (epgCount != 0 && epgIndex < epgCount)
			{
				APP_EPG_API_GetEpgInfo(0, epgIndex, 1,&epgInfo);
				if(GetTime(epgInfo.startHour, epgInfo.startMin) <= APP_IPI_GetTime()
					&& GetTime(epgInfo.endHour, epgInfo.endMin) >= APP_IPI_GetTime())
				{
					memcpy(CurEpgInfo->epgName, epgInfo.epgName, APP_EPG_NAME_LEN);
					CurEpgInfo->epgName[APP_EPG_NAME_LEN - 1] = '\0';
					CurEpgInfo->startHour = epgInfo.startHour;
					CurEpgInfo->startMin = epgInfo.startMin;
					CurEpgInfo->endHour = epgInfo.endHour;
					CurEpgInfo->endMin = epgInfo.endMin;
				}
			}
			else if (epgCount != 0 && epgIndex >= epgCount)
			{
				APP_PRINT_ERROR("error 1: epgindex %d > epgcount %d", epgIndex, epgCount);
			}
			else
			{
				APP_EIT_IPI_StartDemuxPFActualEit();
			}
		}

	}
	else
	{
		APP_EIT_IPI_StartDemuxPFActualEit();
	}



	
	if (pProgInfoNode->progInfo.pNextEpgNode != 0 && pProgInfoNode->progInfo.pNextEpgNode->epgInfo.eventId != 0)
	{

		memcpy(NextEpgInfo->epgName, pProgInfoNode->progInfo.pNextEpgNode->epgInfo.strEpgInfo, APP_EPG_NAME_LEN);
		NextEpgInfo->epgName[APP_EPG_NAME_LEN - 1] = '\0';
		NextEpgInfo->startHour = GetHour(pProgInfoNode->progInfo.pNextEpgNode->epgInfo.startTime);
		NextEpgInfo->startMin = GetMinute(pProgInfoNode->progInfo.pNextEpgNode->epgInfo.startTime);
		NextEpgInfo->endHour = GetHour(pProgInfoNode->progInfo.pNextEpgNode->epgInfo.endTime);
		NextEpgInfo->endMin = GetMinute(pProgInfoNode->progInfo.pNextEpgNode->epgInfo.endTime);

		if(GetTime(NextEpgInfo->startHour, NextEpgInfo->startMin) < APP_IPI_GetTime())
		{
			APP_EPG_IPI_Semaphore_Release();
			if (APP_FAIL == APP_EPG_API_GetEpgCountAndIndex(ChnNo, 0,&epgCount,&epgIndex))
			{
				APP_PRINT_ERROR("can not get epg count and index\n");
			}
			APP_EPG_IPI_Semaphore_Wait(FYF_WAIT_FOREVER);
			if(epgCount != 0 && epgIndex + 1 < epgCount)
			{
				APP_EPG_API_GetEpgInfo(0, epgIndex + 1, 1,&epgInfo);
				if(GetTime(epgInfo.startHour, epgInfo.startMin) >= APP_IPI_GetTime())
				{
					memcpy(NextEpgInfo->epgName, epgInfo.epgName, APP_EPG_NAME_LEN);
					NextEpgInfo->epgName[APP_EPG_NAME_LEN - 1] = '\0';
					NextEpgInfo->startHour = epgInfo.startHour;
					NextEpgInfo->startMin = epgInfo.startMin;
					NextEpgInfo->endHour = epgInfo.endHour;
					NextEpgInfo->endMin = epgInfo.endMin;
				}
			}
			else if (epgCount != 0 && epgIndex + 1 >= epgCount)
			{
				APP_PRINT_ERROR("error 2: epgindex %d > epgcount %d", epgIndex + 1, epgCount);
			}
			else
			{
				APP_EIT_IPI_StartDemuxPFActualEit();
			}
		}
		
	}
	else
	{
		APP_EIT_IPI_StartDemuxPFActualEit();
	}

	APP_EPG_IPI_Semaphore_Release();
	return 1;
}


/*-------------------------------------------------------------------------------

	Description:获得当前事件在当天事件库中
	的序号(一天的事件库是按时间顺序存储的)
	
	Parameters:
	
-------------------------------------------------------------------------------*/

BU32 APP_EPG_API_GetEpgCountAndIndex(BU32 ChnNo, BU08 DayOffset, BU32* Count, BU32* Index)
{
	BU32 serviceId;
	BU32 i = 0;
	struct epgInfoNode_s* epgInfoNode = 0;
	struct epgInfoNode_s* epgInfoNodeTemp = 0;
	BU32 nowTime = 0;
	BU32 week = 0;
	ChnDataBase_ChnInfo_S 	chnInfo;
	Epg_Info_S CurEpgInfo,NextEpgInfo;
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&chnInfo))
	{
		APP_PRINT_ERROR("APP_EPG_API_GetEpgCountAndIndex  over = %d\n",APP_ChnDatabase_API_GetCurChnNO());
		return APP_FAIL; 
	}

	week = g_Week + DayOffset - 1;
	if (week > 6)
	{
		week = week % 7;
	}

	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreEit(), FYF_WAIT_FOREVER);
	APP_EIT_IPI_StartDemuxCurChnAllEit();
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());

	APP_EPG_IPI_Semaphore_Wait(FYF_WAIT_FOREVER);
	nowTime = APP_IPI_GetTime();
	g_DayOffset = DayOffset;

	serviceId = chnInfo.m_ProgId;
	epgInfoNode = DBA_API_GetEpgNode(serviceId, g_Week + DayOffset);
	/*找不到要查询的ID*/
	if (epgInfoNode == 0)
	{
		*Count = 0;
		*Index = 0;
		APP_EPG_IPI_Semaphore_Release();
		return APP_SUCCESS;
	}

	epgInfoNodeTemp = epgInfoNode;
	i = 0;
	//app_printf("\n nowday: %d nowTime: %d\n",	nowTime,APP_IPI_GetDay());

	while (epgInfoNodeTemp->next != 0 && i < APP_EPG_MAX_DAY_COUNT)
	{
		epgInfoNodeTemp = epgInfoNodeTemp->next;
		/*
		APP_Printf("event: %d %d-%d %s\n",
		epgInfoNodeTemp->epgInfo.day,			
		epgInfoNodeTemp->epgInfo.startTime,
		epgInfoNodeTemp->epgInfo.endTime,
		epgInfoNodeTemp->epgInfo.strEpgInfo);
		*/
		//前端传输错误的结束时间
		if((DayOffset == 0 && epgInfoNodeTemp->next != 0) && epgInfoNodeTemp->next->epgInfo.startTime <= nowTime)
			//|| epgInfoNodeTemp->epgInfo.week > week + 1)//发生过星期混乱，原因未知，暂时解决方法09.5.8 //显示下周一的跨天事件09.5.15
		{		
			continue;
		}

		#ifndef APP_EPG_SAVE_PASSED_EPG
		if(DayOffset == 0 
			&& epgInfoNodeTemp->epgInfo.endTime <= nowTime 
			&& epgInfoNodeTemp->epgInfo.startTime <= nowTime
			&& epgInfoNodeTemp->epgInfo.startTime < epgInfoNodeTemp->epgInfo.endTime)
		{
			continue;
		}
		#endif

		g_EpgInfo[i].serviceId = serviceId;
		g_EpgInfo[i].eventId = epgInfoNodeTemp->epgInfo.eventId;
		g_EpgInfo[i].reserveState = epgInfoNodeTemp->epgInfo.reserve;
		g_EpgInfo[i].detailState = epgInfoNodeTemp->epgInfo.hasDetail;
		memcpy(g_EpgInfo[i].epgName, epgInfoNodeTemp->epgInfo.strEpgInfo, APP_EPG_NAME_LEN);
		g_EpgInfo[i].epgName[APP_EPG_NAME_LEN - 1] = '\0';
		memcpy(g_EpgInfo[i].chnName, chnInfo.m_ChnName, APP_CHN_NAME_LEN);
		g_EpgInfo[i].epgName[APP_CHN_NAME_LEN - 1] = '\0';
		g_EpgInfo[i].startHour = GetHour(epgInfoNodeTemp->epgInfo.startTime);
		g_EpgInfo[i].startMin = GetMinute(epgInfoNodeTemp->epgInfo.startTime);
		g_EpgInfo[i].endHour = GetHour(epgInfoNodeTemp->epgInfo.endTime);
		g_EpgInfo[i].endMin = GetMinute(epgInfoNodeTemp->epgInfo.endTime);
		g_EpgInfo[i].week = epgInfoNodeTemp->epgInfo.week;
		g_EpgInfo[i].day = epgInfoNodeTemp->epgInfo.day;
		g_EpgInfo[i].style = APP_ChnDatabase_API_GetCurChnType();
		
		if (g_EpgInfo[i].day < APP_IPI_GetDay())
		{
			g_EpgInfo[i].playState = APP_EPG_PLAYSTATE_PLAYED_e;
			if(DayOffset == 0)
			{
				if((BU32)(g_EpgInfo[i].endHour * 60 + g_EpgInfo[i].endMin) >= nowTime)
				{
					g_EpgInfo[i].playState = APP_EPG_PLAYSTATE_PLAYING_e;
				}
			}
		}
		else if (g_EpgInfo[i].day > APP_IPI_GetDay())
		{
			g_EpgInfo[i].playState = APP_EPG_PLAYSTATE_UNPLAY_e;
		}
		else
		{
#ifndef APP_EPG_SAVE_PASSED_EPG
			g_EpgInfo[i].playState = APP_EPG_PLAYSTATE_UNPLAY_e;
#else
			if ((g_EpgInfo[i].startHour * 60 + g_EpgInfo[i].startMin) <= nowTime 
				&& (g_EpgInfo[i].endHour * 60 + g_EpgInfo[i].endMin) >= nowTime)
			{
				g_EpgInfo[i].playState = APP_EPG_PLAYSTATE_PLAYING_e;
			}
			else if((g_EpgInfo[i].startHour * 60 + g_EpgInfo[i].startMin) > nowTime)
			{
				g_EpgInfo[i].playState = APP_EPG_PLAYSTATE_UNPLAY_e;
			}
			else
			{
				g_EpgInfo[i].playState = APP_EPG_PLAYSTATE_PLAYED_e;
				*Index = i;
			}
#endif
		}

		++i;
	}
#ifndef APP_EPG_SAVE_PASSED_EPG
	if(DayOffset == 0)
	{
		if(/*GetTime(g_EpgInfo[0].startHour, g_EpgInfo[0].startMin) <= nowTime && */
			 GetTime(g_EpgInfo[0].endHour, g_EpgInfo[0].endMin) >= nowTime )
		{
			g_EpgInfo[0].playState = APP_EPG_PLAYSTATE_PLAYING_e;
		}
		else
		{
			g_EpgInfo[0].playState = APP_EPG_PLAYSTATE_UNPLAY_e;
		}
	}
	*Index = 0;
#endif
	*Count = i;

	//	GUI_API_Printf("APP_EPG_API_GetEpgCountAndIndex End\n");
	APP_EPG_IPI_Semaphore_Release();
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:取得预约事件的serviceId eventId,用于比较是否被预约
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_IPI_GetReservedShortInfo(void)
{

	int Num = 0;
	int i;

	SQL_API_Semaphore_Wait();
	SQL_API_Select("SELECT 编号,事件号,预约状态 FROM eventReserved");
	Num = SQL_API_GetTotalNum();
	g_RerservedCount = Num;
	if(Num == 0)
	{
		SQL_API_Semaphore_Release();
		return ;
	}

	for (i = 0; i < Num; i++)
	{
		if (SQL_API_Eof())
			break;
		epg_reservedlist[i].serviceId = SQL_API_GetData(0, 0, 0, APP_DATA_TYPE_NUM); //编号
		epg_reservedlist[i].eventId = SQL_API_GetData(1, 0, 0, APP_DATA_TYPE_NUM);	//事件号
		epg_reservedlist[i].reserveState = SQL_API_GetData(2, 0, 0, APP_DATA_TYPE_NUM);	//预约状态
		SQL_API_QueryNext();
	}
	SQL_API_Semaphore_Release();
}

/*--------------------------------------------------------------

/*-------------------------------------------------------------------------------

	Description:
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_GetEpgInfo(BU08 DayOffset, BU32 Index, BU32 Num, Epg_Info_S* EpgInfo)
{
	if (Index + Num >= APP_EPG_MAX_DAY_COUNT)
	{
		APP_PRINT_ERROR("APP_EPG_API_GetEpgInfo over %d, %d\n", Index, Num);
		return APP_FAIL;
	}
	BU32 i = 0;
	Epg_Info_S* pEpgInfo = EpgInfo;

	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreEit(), FYF_WAIT_FOREVER);
	APP_EIT_IPI_StartDemuxCurChnAllEit();
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());

	if(bReserveChange)
	{
		APP_EPG_IPI_GetReservedShortInfo();
		bReserveChange = false;
	}

	while (i < Num)
	{
		memcpy(pEpgInfo, &g_EpgInfo[i + Index], sizeof(Epg_Info_S));
		pEpgInfo->reserveState = APP_EPG_API_ifReserved(pEpgInfo);
		++i;
		++pEpgInfo;
	}

	return APP_SUCCESS;
}


/*-------------------------------------------------------------------------------

	Description:获得事件的详细信息
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_GetEventDetailText(BU32 EventNo, BU08* EventText, BU32 textLength)
{
	BU32 rV = APP_FAIL;
	ChnDataBase_ChnInfo_S chnnnfo;
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnnnfo))
	{
		APP_PRINT_ERROR("APP_ChnDatabase_API_GetChnInfo() error!\n");
	}
	else
	{
		APP_EPG_IPI_Semaphore_Wait(FYF_WAIT_FOREVER);
		rV = DBA_API_GetDetail(chnnnfo.m_ProgId,g_EpgInfo[EventNo].eventId,EventText,textLength);
		APP_EPG_IPI_Semaphore_Release();	
	
	}

	return rV;
}

/*-------------------------------------------------------------------------------

	Description:设置Nvod预约
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_API_SetNvodReserveState(APP_NvodReserve_S *NvodReserve,BU08 ReserveState)
{
	SQL_API_Semaphore_Wait();
	if(ReserveState)
	{
	sprintf(g_StrSQL, "INSERT INTO eventReserved \
(编号,节目名, 星期,事件号,显示信息状态,详细信息, \
事件内容, 预约状态,开始小时,开始分钟,结束小时, \
结束分钟, 日期, 类型) \
VALUES (%d, '点播节目', 0, %d, 0, '', \
'%s', 4, %d, %d, %d, \
%d, %d, %d);",
	NvodReserve->serviceId,NvodReserve->eventId,
	NvodReserve->eventName,NvodReserve->startHour, NvodReserve->startMin, NvodReserve->endHour,
	NvodReserve->endMin, NvodReserve->day, APP_CHN_AV_TYPE_NVOD_TIMESHIFT_SERVICE);
	}
	else
	{
		sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 预约状态 = 4 AND 编号 = %d AND 事件号 = %d;",  NvodReserve->serviceId,  NvodReserve->eventId);
	}
	//app_printf("SetNvodReserveState ----%s\n",g_StrSQL);
	SQL_API_Exec(g_StrSQL);
	SQL_API_Semaphore_Release();
	bReserveChange = true;
}

/*-------------------------------------------------------------------------------

	Description:设置手动选时录制
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_API_SetManualRecState(APP_ManualRecData_S *RecData)
{
	SQL_API_Semaphore_Wait();
	sprintf(g_StrSQL, "INSERT INTO eventReserved \
(编号,节目名, 星期,事件号,显示信息状态,详细信息, \
事件内容, 预约状态,开始小时,开始分钟,结束小时, \
结束分钟, 日期, 类型) \
VALUES (%d, '%s', %d, 0, 0, '', \
'', 3, %d, %d, %d, \
%d, %d, %d);",
	RecData->serviceId, RecData->chnName, RecData->week,
	RecData->startHour, RecData->startMin, RecData->endHour,
	RecData->endMin, RecData->day, APP_ChnDatabase_API_GetCurChnType());
	//app_printf("SetManualRecState ----%s\n",g_StrSQL);
	SQL_API_Exec(g_StrSQL);
	SQL_API_Semaphore_Release();
	bReserveChange = true;
}
/*-------------------------------------------------------------------------------

	Description:设置事件预约/预录
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_API_SetEvntReserveState(BU32 EventNo, BU08 dayOffset, BU08 ReserveState)
{
	BU32 serviceId;
	Epg_Info_S epgInfo[1];
	BU08 reserveWeek = 0;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(),&ProgInfoListtemp))
	{
		APP_PRINT_ERROR("APP_EPG_API_SetEvntReserveState  over = %d\n",APP_ChnDatabase_API_GetCurChnNO());
		return; 
	}

	reserveWeek = g_Week + dayOffset;
	if (reserveWeek > 7)
		reserveWeek -= 7;
	serviceId = ProgInfoListtemp.m_ProgId;

	if (APP_FAIL == APP_EPG_API_GetEpgInfo(0, EventNo, 1, epgInfo))
	{
		return;
	}

	SQL_API_Semaphore_Wait();
	g_EpgInfo[EventNo].reserveState = ReserveState;
	if (ReserveState)
	{
		sprintf(g_StrSQL, "INSERT INTO eventReserved \
(编号,节目名, 星期,事件号,显示信息状态,详细信息, \
事件内容, 预约状态,开始小时,开始分钟,结束小时, \
结束分钟, 日期, 类型) \
VALUES (%d, '%s', %d, %d, %d, '', \
'%s', %d, %d, %d, %d, \
%d, %d, %d);",
			serviceId, ProgInfoListtemp.m_ChnName, epgInfo[0].week, epgInfo[0].eventId, epgInfo[0].detailState, 
			epgInfo[0].epgName, ReserveState, epgInfo[0].startHour, epgInfo[0].startMin, epgInfo[0].endHour,
			epgInfo[0].endMin, epgInfo[0].day, APP_ChnDatabase_API_GetCurChnType());
	}
	else
	{
		sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 编号 = %d AND 事件号 = %d;", serviceId, epgInfo[0].eventId);
	}
	//app_printf("SetEvntReserveState ----%s\n",g_StrSQL);
	SQL_API_Exec(g_StrSQL);
	SQL_API_Semaphore_Release();
	bReserveChange = true;
}
/*-------------------------------------------------------------------------------

	Description:取得预约事件内容
	
	Parameters: APP_SUCCESS not reserved, APP_FAIL reserved
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_ifReserved(Epg_Info_S* EventInfo)
{

	int i;

	if(g_RerservedCount == 0)
		return APP_FAIL;
		
	for (i = 0; i < g_RerservedCount; i++)
	{
		if((EventInfo->serviceId == epg_reservedlist[i].serviceId) && (epg_reservedlist[i].eventId == EventInfo->eventId))
			return epg_reservedlist[i].reserveState;
	}

	return APP_FAIL;
#if 0
	SQL_API_Semaphore_Wait();
	sprintf(g_StrSQL, "SELECT * FROM eventReserved WHERE 编号 = %d AND 事件号 = %d;", EventInfo->serviceId, EventInfo->eventId);

	SQL_API_Select(g_StrSQL);
	
	if (0 == SQL_API_GetTotalNum())
	{
		SQL_API_Semaphore_Release();
		return APP_FAIL;
	}
	else
	{
		SQL_API_Semaphore_Release();
		return APP_SUCCESS;
	}	
#endif
}

/*-------------------------------------------------------------------------------

	Description:取得不同预录/预约类型事件总数
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_GetReserveTypeAmount(APP_ReserveType_e type)
{
	BU32 reserveCount = 0;
	SQL_API_Semaphore_Wait();
	switch(type)
		{
		case APP_ReserveType_All_e:
			SQL_API_Select("SELECT * FROM eventReserved;");
			break;
		case APP_ReserveType_Epg_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 1 OR 预约状态 = 2;");
			break;
		case APP_ReserveType_Rec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 2 OR 预约状态 = 3;");
			break;
		case APP_ReserveType_EpgReserve_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 1;");
			break;
		case APP_ReserveType_EpgRec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 2;");
			break;
		case APP_ReserveType_ManualRec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 3;");
			break;
		case APP_ReserveType_Nvod_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 4;");
			break;
		default:
			break;
		}
	reserveCount = SQL_API_GetTotalNum();
	SQL_API_Semaphore_Release();
	return reserveCount;
}

/*-------------------------------------------------------------------------------

	Description:取得预约事件总数
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_GetReserveAmount(void)
{
	BU32 reserveCount = 0;
	
	SQL_API_Semaphore_Wait();
	SQL_API_Select("SELECT * FROM eventReserved;");
	reserveCount = SQL_API_GetTotalNum();
	SQL_API_Semaphore_Release();
	return reserveCount;
}
/*-------------------------------------------------------------------------------

	Description:取得不同预录/预约类型事件内容
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_API_GetReserveTypeInfo(BU32 Index, BU32 Num, APP_ReserveType_e type,Epg_Info_S* EventInfo)
{
	BU32 i = 0;
	BU32 j = 0;
	Epg_Info_S* eventInfo;
	ChnDataBase_ChnInfo_S chnInfo;
	
	eventInfo = EventInfo;
	SQL_API_Semaphore_Wait();
	switch(type)
		{
		case APP_ReserveType_All_e:
			SQL_API_Select("SELECT * FROM eventReserved ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_Epg_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 1 OR 预约状态 = 2 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_Rec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 2 OR 预约状态 = 3 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_EpgReserve_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 1 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_EpgRec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 2 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_ManualRec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 3 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_Nvod_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 4 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		default:
			SQL_API_Select("SELECT * FROM eventReserved ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		}
	SQL_API_GotoRow(Index);
	for (i = 0; i < Num; i++)
	{
		if (SQL_API_Eof())
			break;

		eventInfo->serviceId = SQL_API_GetData(EVENTRESERVED_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData(EVENTRESERVED_CHN_NAME, eventInfo->chnName, APP_CHN_NAME_LEN, APP_DATA_TYPE_STRING);
		eventInfo->eventId = SQL_API_GetData(EVENTRESERVED_EVENT_ID, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData(EVENTRESERVED_EPG_INFO, eventInfo->epgName, APP_EPG_NAME_LEN, APP_DATA_TYPE_STRING);
		eventInfo->week = (BU08) SQL_API_GetData(EVENTRESERVED_WEEK, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->startHour = (BU08) SQL_API_GetData(EVENTRESERVED_START_HOUR, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->startMin = (BU08) SQL_API_GetData(EVENTRESERVED_START_MIN, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->endHour = (BU08) SQL_API_GetData(EVENTRESERVED_END_HOUR, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->endMin = (BU08) SQL_API_GetData(EVENTRESERVED_END_MIN, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->day = SQL_API_GetData(EVENTRESERVED_DAY, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->style = (BU08) SQL_API_GetData(EVENTRESERVED_STYLE, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->reserveState = (BU08)SQL_API_GetData(EVENTRESERVED_RESERVE_STATE, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_QueryNext();
		eventInfo++;
	}
	SQL_API_Semaphore_Release();
}

/*-------------------------------------------------------------------------------

	Description:取得预约事件内容
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_API_GetReservedInfo(BU32 Index, BU32 Num, Epg_Info_S* EventInfo)
{
	BU32 i = 0;
	BU32 j = 0;
	Epg_Info_S* eventInfo;
	ChnDataBase_ChnInfo_S chnInfo;
	
	eventInfo = EventInfo;
	SQL_API_Semaphore_Wait();
	SQL_API_Select("SELECT * FROM eventReserved ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");

	SQL_API_GotoRow(Index);
	for (i = 0; i < Num; i++)
	{
		if (SQL_API_Eof())
			break;

		eventInfo->serviceId = SQL_API_GetData(EVENTRESERVED_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData(EVENTRESERVED_CHN_NAME, eventInfo->chnName, APP_CHN_NAME_LEN, APP_DATA_TYPE_STRING);
		eventInfo->eventId = SQL_API_GetData(EVENTRESERVED_EVENT_ID, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_GetData(EVENTRESERVED_EPG_INFO, eventInfo->epgName, APP_EPG_NAME_LEN, APP_DATA_TYPE_STRING);
		eventInfo->week = (BU08) SQL_API_GetData(EVENTRESERVED_WEEK, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->startHour = (BU08) SQL_API_GetData(EVENTRESERVED_START_HOUR, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->startMin = (BU08) SQL_API_GetData(EVENTRESERVED_START_MIN, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->endHour = (BU08) SQL_API_GetData(EVENTRESERVED_END_HOUR, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->endMin = (BU08) SQL_API_GetData(EVENTRESERVED_END_MIN, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->day = SQL_API_GetData(EVENTRESERVED_DAY, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->style = (BU08) SQL_API_GetData(EVENTRESERVED_STYLE, 0, 0, APP_DATA_TYPE_NUM);
		eventInfo->reserveState = (BU08)SQL_API_GetData(EVENTRESERVED_RESERVE_STATE, 0, 0, APP_DATA_TYPE_NUM);
		SQL_API_QueryNext();
		eventInfo++;
	}
	SQL_API_Semaphore_Release();
}

/*-------------------------------------------------------------------------------

	Description:从选时预录列表中取消一条不同预录/预约类型

	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_CancelReserveType(BU32 CancelIndex,APP_ReserveType_e type)
{
	BU08 rV = 0;
	BU32 reserveCount, serviceId,eventId;
	BU32 day;
	BU08 startHour,startMin;
	//BU08 endHour,endMin;
	SQL_API_Semaphore_Wait();
	switch(type)
		{
		case APP_ReserveType_All_e:
			SQL_API_Select("SELECT * FROM eventReserved ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_Epg_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 1 OR 预约状态 = 2 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_Rec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 2 OR 预约状态 = 3 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_EpgReserve_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 1 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_EpgRec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 2 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_ManualRec_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 3 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		case APP_ReserveType_Nvod_e:
			SQL_API_Select("SELECT * FROM eventReserved WHERE 预约状态 = 4 ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		default:
			SQL_API_Select("SELECT * FROM eventReserved ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
			break;
		}
	reserveCount = SQL_API_GetTotalNum();
	if (reserveCount < CancelIndex)
	{
		SQL_API_Semaphore_Release();
		APP_PRINT_ERROR("Invalid Cancle Index\n");
		return APP_FAIL;
	}
	SQL_API_GotoRow(CancelIndex);
	serviceId = SQL_API_GetData(EVENTRESERVED_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
	eventId = SQL_API_GetData(EVENTRESERVED_EVENT_ID, 0, 0, APP_DATA_TYPE_NUM);
	startHour = (BU08)SQL_API_GetData(EVENTRESERVED_START_HOUR, 0, 0, APP_DATA_TYPE_NUM);
	startMin = (BU08)SQL_API_GetData(EVENTRESERVED_START_MIN, 0, 0, APP_DATA_TYPE_NUM);
	day = SQL_API_GetData(EVENTRESERVED_DAY, 0, 0, APP_DATA_TYPE_NUM);
	switch(type)
		{
		case APP_ReserveType_All_e:
			sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 编号 = %d AND 日期 = %d AND 开始小时 = %d AND 开始分钟 = %d;", serviceId, day,startHour,startMin);
			break;
		case APP_ReserveType_Epg_e:
		case APP_ReserveType_EpgReserve_e:
		case APP_ReserveType_EpgRec_e:
			sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 编号 = %d AND 事件号 = %d;", serviceId, eventId);
			break;
		case APP_ReserveType_Rec_e:
			sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 预约状态 = 2 OR 预约状态 = 3 AND 编号 = %d AND 日期 = %d AND 开始小时 = %d AND 开始分钟 = %d;", serviceId, day,startHour,startMin);
			break;
		case APP_ReserveType_ManualRec_e:
			sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 预约状态 = 3 AND 编号 = %d AND 日期 = %d AND 开始小时 = %d AND 开始分钟 = %d;", serviceId, day,startHour,startMin);
			break;
		case APP_ReserveType_Nvod_e:
			sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 预约状态 = 4 AND 编号 = %d AND 事件号 = %d;", serviceId, eventId);
			break;
		default:
			sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 编号 = %d AND 日期 = %d AND 开始小时 = %d AND 开始分钟 = %d;", serviceId, day,startHour,startMin);
			break;
		}
	rV = SQL_API_Exec(g_StrSQL);
	if(!rV)
	{
		SQL_API_Semaphore_Release();
		return APP_FAIL;
	}
	SQL_API_Semaphore_Release();
	bReserveChange = true;
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:从预约列表中取消一条预约
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_CancelReserve(BU32 CancelIndex)
{
	BU08 rV = 0;
	BU32 reserveCount, serviceId, eventId;
	
	SQL_API_Semaphore_Wait();
	
	SQL_API_Select("SELECT * FROM eventReserved ORDER BY 日期 ASC, 开始小时 ASC, 开始分钟 ASC;");
	reserveCount = SQL_API_GetTotalNum();
	if (reserveCount < CancelIndex)
	{
		SQL_API_Semaphore_Release();
		APP_PRINT_ERROR("Invalid Cancle Index\n");
		return APP_FAIL;
	}
	SQL_API_GotoRow(CancelIndex);

	serviceId = SQL_API_GetData(EVENTRESERVED_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
	eventId = SQL_API_GetData(EVENTRESERVED_EVENT_ID, 0, 0, APP_DATA_TYPE_NUM);

	sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 编号 = %d AND 事件号 = %d;", serviceId, eventId);
	rV = SQL_API_Exec(g_StrSQL);
	if(!rV)
	{
		SQL_API_Semaphore_Release();
		return APP_FAIL;
	}
	SQL_API_Semaphore_Release();
	bReserveChange = true;
	return APP_SUCCESS;
}

void APP_EPG_IPI_InitReserve(void)
{
	SQL_API_Semaphore_Wait();
	sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 日期 < %d;", g_Year * 10000 + g_Month * 100 + g_Date);
	SQL_API_Exec(g_StrSQL);
	sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 日期 = %d AND 开始小时 < %d;", g_Year * 10000 + g_Month * 100 + g_Date, g_Hour);
	SQL_API_Exec(g_StrSQL);
	sprintf(g_StrSQL, "DELETE FROM eventReserved WHERE 日期 = %d AND 开始小时 = %d AND 开始分钟 < %d;", g_Year * 10000 + g_Month * 100 + g_Date, g_Hour, g_Minute);
	SQL_API_Exec(g_StrSQL);

	SQL_API_Semaphore_Release();
	bReserveChange = true;
}

/*-------------------------------------------------------------------------------

	Description:保存预约更改
	
	Parameters:
	
-------------------------------------------------------------------------------*/
BU32 APP_EPG_API_UpDataToFlash(void)
{
	SQL_API_SaveToFlash("flash.db");
	return APP_SUCCESS;
}

ReserveCallBackFun ReserveCallBack = 0;

/*-------------------------------------------------------------------------------

	Description:设置预约到时回调
	
	Parameters:
	
-------------------------------------------------------------------------------*/
void APP_EPG_API_SetReserveCallBack(ReserveCallBackFun CallBack)
{
	ReserveCallBack = CallBack;
}

void APP_EPG_IPI_ReserveCallBack(BU32 callback_type, Epg_Info_S* EpgInfo)
{
	if (ReserveCallBack != 0)
		ReserveCallBack(callback_type, EpgInfo);
}

/********************************************************
	描述:检测预约列表中是否有与当前nvod预约冲突，
		 有的话返回预约列表中的序号
*********************************************************/
BU32 APP_EPG_API_GetNvodConflictIndex(BU32 *conflictIndex,APP_NvodReserve_S* NvodReserve)
{
	BU32 reservedNum,ret=0;
	BU32 nvodReservestartMin,reserveMin;
	BU32 i=0,index =0;
	Epg_Info_S reserveInfo[MAX_RESERVE_NUM];
	Epg_Info_S EpgInfo;
	APP_NvodReserve_S nvod_ReserveInfo;
	*conflictIndex = 0xff;

	memcpy(&nvod_ReserveInfo,NvodReserve,sizeof(APP_NvodReserve_S));
	reservedNum = APP_EPG_API_GetReserveAmount();
	
	if(reservedNum == 0)
	{
		return 0;
	}
	APP_EPG_API_GetReservedInfo(0, reservedNum, reserveInfo);
	for(i=0;i<reservedNum;i++)
	{
		if(reserveInfo[i].day == NvodReserve->day)
		{
			reserveMin = reserveInfo[i].startHour*60 + reserveInfo[i].startMin;
			nvodReservestartMin = NvodReserve->startHour*60 + NvodReserve->startMin;
			if(reserveMin >= nvodReservestartMin -1 && reserveMin <= nvodReservestartMin +1)
			{
				*conflictIndex  = i;
				ret =1;
				break;
			}
		}
	}
	return ret;
}

