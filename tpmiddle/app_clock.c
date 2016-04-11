#include "app_common.h"
#include "app_ipi_common.h"
/*
BU32 g_Year = 0;
BU32 g_Month = 0;
BU32 g_Date = 0;
BU32 g_Hour = 8;
BU32 g_Minute = 0;
BU32 g_Sec = 0;
*/
BU32 g_MSec = 0;


extern BU32 g_Year;
extern BU32 g_Month;
extern BU32 g_Date;
extern BU32 g_Hour;
extern BU32 g_Minute;
extern BU32 g_Sec;
BU08 g_Week = 1;//1~7

BU32 g_lastGetPmtTime = 0;
BU08 bIsClockRun = FYF_FALSE;

extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];
extern BU08 isStopSearch;
extern BU16 g_CurPlayChn;//但前正在播放的节目号

BU32 pvr_endday = 0;
BU08 pvr_endhour = 0;
BU08 pvr_endmin = 0;
/*
判断是否有预约到时
*/
BU08 isRemindReserve = 1;
/*----------------------------------------------------------------------
Function Prototypes
----------------------------------------------------------------------*/
void APP_IPI_IsReserveOnTime(void);


void APP_API_SetPVREndTime(BU32 day, BU32 hour, BU32 min)
{
	pvr_endday = day;
	pvr_endhour = hour;
	pvr_endmin = min;
}
/*-------------------------------------------------------------------------------

	Description: Enable or Disable Reserve Monitor

	Parameters:

-------------------------------------------------------------------------------*/
void APP_EPG_API_SetRemindReserve(BU08 state)
{
	if(state)
	{
		APP_EPG_IPI_InitReserve();
	}
	isRemindReserve = state;
}

static BU32 timeGetPat = 0;
void APP_IPI_ResetTimeGetPat(void)
{
	timeGetPat = 0;
}
void APP_IPI_TimeGetPat(void)
{
	if(++timeGetPat < APP_CONFIG_PAT_MONTIOR_TIME)
	{
		return;
	}

	//APP_PRINT_DEBUG("-----------------APP_IPI_GetIsPlayStop() = %d---------------------\n", APP_IPI_GetIsPlayStop());

	if(APP_IPI_GetIsPlayStop() == 1)
	{
		timeGetPat = 0;
		return;
	}
	

	if (APP_Search_API_GetStopState() == 0)
	{
		timeGetPat = 0;
		APP_PRINT_DEBUG("Error:-----------------APP_IPI_GetIsPlayStop() = %d---------------------\n", APP_IPI_GetIsPlayStop());
		return;
	}
	APP_PAT_IPI_StartDemuxPatOnTime();
}


static BU32 timeGetPmt = 0;
void APP_IPI_ResetTimeGetPmt(void)
{
	timeGetPmt = 0;
}

void APP_IPI_SetTimeGetPmt(BU32 time)
{
	timeGetPmt = time;
}

void APP_IPI_TimeGetPmt(void)
{
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	
	if(++timeGetPmt < APP_CONFIG_PMT_MONTIOR_TIME)
	{
		return;
	}

	if(APP_IPI_GetIsPlayStop() == 1)
	{
		timeGetPmt = 0;
		return;
	}

	if (APP_Search_API_GetStopState() == 0)
	{
		APP_PRINT_DEBUG("Error:-----------------APP_IPI_GetIsPlayStop() = %d---------------------\n", APP_IPI_GetIsPlayStop());
		timeGetPmt = 0;
		return;
	}
	
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO(), &ProgInfoListtemp))
	{
		timeGetPmt = 0;
		APP_PRINT_DEBUG("TimeGetPmt chn over = %d\n",app_system_data.g_CurChnNo);
		return; 
	}
	

	PMTMONTIOR_DBG("Enter pmtmonitor chno = %d, pmtpid = %d\n", app_system_data.g_CurChnNo, ProgInfoListtemp.m_pmtpid);
	APP_PMT_IPI_StartDemuxPmtPcrEcm(ProgInfoListtemp.m_pmtpid);
	
#if 0
	SQL_API_Semaphore_Wait();
 
	timeGetPmt = 0;
	sprintf(g_StrSQL,"SELECT PID FROM program WHERE service_id = %d;", ProgInfoListtemp.m_ProgId);
	SQL_API_Select(g_StrSQL);
	if(SQL_API_GetTotalNum() != 0 && (BS16)SQL_API_GetData(0,0,0,APP_DATA_TYPE_NUM) != 0)
	{
		pmtpid = (BS16)SQL_API_GetData(0,0,0,APP_DATA_TYPE_NUM);
		SQL_API_Semaphore_Release();
		APP_PMT_IPI_StartDemuxPmtPcrEcm(pmtpid);
	}
	else
	{
		SQL_API_Semaphore_Release();
	}
#endif
	
}

static BU32 timeGetSdt = 0;
void APP_IPI_ResetTimeGetSdt(void)
{
	timeGetSdt = 0;
}
void APP_IPI_TimeGetSdt(void)
{
	//APP_PRINT_INFO("\n!!!!!!!!_APP_IPI_TimeGetSdt_!!!!!!!!!!!!!!!!!!\n");

	if(++timeGetSdt < APP_CONFIG_SDT_MONITOR_Time)
	{
		return;
	}

	if(APP_IPI_GetIsPlayStop() == 1)
	{
		timeGetSdt = 0;
		return;
	}

	if (APP_Search_API_GetStopState() == 0)
	{
		APP_PRINT_DEBUG("Error:-----------------APP_IPI_GetIsPlayStop() = %d---------------------\n", APP_IPI_GetIsPlayStop());
		timeGetSdt = 0;
		return;
	}


	//PMTMONTIOR_DBG("Enter pmtmonitor chno = %d, pmtpid = %d\n", app_system_data.g_CurChnNo, ProgInfoListtemp.m_pmtpid);
	APP_SDT_IPI_StartDemuxSdtOnTime();

	
}
void APP_IPI_IsReserveOnTime(void)
{
	static Epg_Info_S eventInfo[1];
	BU32 Hour = 8;
	BU32 Minute = 0;	
	
	if(isRemindReserve == 1)
	{
		SQL_API_Semaphore_Wait();
		Hour = g_Hour;
		Minute = g_Minute + 1; //当前分钟弹出预约时间到框09.5.15
		if(Minute == 60)
		{
			Hour++;
			Minute=0;
		}
		sprintf(g_StrSQL, "SELECT * FROM eventReserved WHERE 开始小时 = %d AND 开始分钟 = %d AND 日期 = %d;",
			Hour, Minute, g_Year * 10000 + g_Month * 100 + g_Date);
		APP_PRINT_DEBUG("Date = %d\n", g_Year * 10000 + g_Month * 100 + g_Date);
		SQL_API_Select(g_StrSQL);
		if (SQL_API_GetTotalNum() > 0)
		{
			eventInfo->serviceId = SQL_API_GetData(EVENTRESERVED_SERVICE_ID, 0, 0, APP_DATA_TYPE_NUM);
			eventInfo->eventId 	 = SQL_API_GetData(EVENTRESERVED_EVENT_ID, 0, 0, APP_DATA_TYPE_NUM);
			SQL_API_GetData(EVENTRESERVED_CHN_NAME, eventInfo->chnName, APP_CHN_NAME_LEN, APP_DATA_TYPE_STRING);
			SQL_API_GetData(EVENTRESERVED_EPG_INFO, eventInfo->epgName, APP_EPG_NAME_LEN, APP_DATA_TYPE_STRING);
			eventInfo->week      = (BU08)SQL_API_GetData(EVENTRESERVED_WEEK, 0, 0, APP_DATA_TYPE_NUM);
			eventInfo->startHour = (BU08)SQL_API_GetData(EVENTRESERVED_START_HOUR, 0, 0, APP_DATA_TYPE_NUM);
			eventInfo->startMin  = (BU08)SQL_API_GetData(EVENTRESERVED_START_MIN, 0, 0, APP_DATA_TYPE_NUM);
			eventInfo->endHour   = (BU08)SQL_API_GetData(EVENTRESERVED_END_HOUR, 0, 0, APP_DATA_TYPE_NUM);
			eventInfo->endMin    = (BU08)SQL_API_GetData(EVENTRESERVED_END_MIN, 0, 0, APP_DATA_TYPE_NUM);
			eventInfo->day		 = SQL_API_GetData(EVENTRESERVED_DAY, 0, 0, APP_DATA_TYPE_NUM);
			eventInfo->style	 = (BU08)SQL_API_GetData(EVENTRESERVED_STYLE, 0, 0, APP_DATA_TYPE_NUM);
			eventInfo->reserveState = (BU08)SQL_API_GetData(EVENTRESERVED_RESERVE_STATE, 0, 0, APP_DATA_TYPE_NUM);
			SQL_API_Semaphore_Release();
			
//如果是预录任务，把结束时间保存起来
#ifdef ENABLE_PVR
			if((eventInfo->reserveState == 2 || eventInfo->reserveState == 3 )&& APP_PVR_API_GetRecStatus(1) == 0)
			{
				pvr_endday = eventInfo->day;
				pvr_endhour = eventInfo->endHour;
				pvr_endmin = eventInfo->endMin;
				APP_PRINT_DEBUG("pvr_endday = %d,pvr_endhour = %d,pvr_endmin = %d\n",pvr_endday,pvr_endhour,pvr_endmin);
			}
#endif
			APP_EPG_IPI_ReserveCallBack(0, eventInfo);
		}
		else
		{
			SQL_API_Semaphore_Release();
		}
		//定时录制结束
#ifdef ENABLE_PVR
		if(pvr_endday == (g_Year * 10000 + g_Month * 100 + g_Date) 
			&& pvr_endhour == Hour && pvr_endmin == Minute && APP_PVR_API_GetRecStatus(1) == 1)
		{
			APP_EPG_IPI_ReserveCallBack(1, NULL);
		}
#endif
	}

}
void APP_API_IsReserveOnTime(void)
{
	APP_IPI_IsReserveOnTime();
}
void APP_API_TimeGetPmt(void)
{
	APP_IPI_TimeGetPmt();
}
void APP_API_TimeGetPat(void)
{
	APP_IPI_TimeGetPat();
}

void APP_API_TimeGetSdt(void)
{
	APP_IPI_TimeGetSdt();
}
static BU32 timeGetNit = 0;
void APP_API_TimeGetNitAndOta()
{
	if(++timeGetNit < APP_NIT_MONITOR_SLEEP_TIME_SEC)
	{
		return;
	}

	/*if(APP_IPI_GetIsPlayStop() == 1)
	{
		timeGetNit = 0;
		return;
	}*/

	if (APP_Search_API_GetStopState() == 0)
	{
		APP_PRINT_DEBUG("Error:-----------------APP_IPI_GetIsPlayStop() = %d---------------------\n", APP_IPI_GetIsPlayStop());
		timeGetNit = 0;
		return;
	}
	APP_Printf("APP_API_TimeGetNitAndOta\n");

	//PMTMONTIOR_DBG("Enter pmtmonitor chno = %d, pmtpid = %d\n", app_system_data.g_CurChnNo, ProgInfoListtemp.m_pmtpid);
	//APP_SDT_IPI_StartDemuxSdtOnTime();
	APP_NIT_IPI_StartDemuxNitOta(APP_NIT_IPI_StartParserNitGetVerAndOTA);
	timeGetNit = 0;
}
void APP_IPI_GotoNextDay(BU32 *year, BU32 *month, BU32 *date, BU08 *week)
{
	BU08 leap = 0;
	BU08 monthDay = 0;
	
	++(*week);
	++(*date);
	
	if((*week) > 7)
		(*week) = 1;
		
	if(((*year) % 4 == 0 && (*year) % 100 != 0) 
		||((*year) % 400 == 0))
	{
		leap = 1;
	}
	else
	 	leap = 0;

	if((*month) == 1 || (*month) == 3 || (*month) == 5 
		|| (*month) == 7 || (*month) == 8 || (*month) == 10 
		|| (*month) == 12)
		monthDay = 31;
	else
		monthDay = 30;

	if((*month) == 2)
	{
		if(leap == 1)
			monthDay = 29;
		else
			monthDay = 28;
	}
	
	if((*date) > monthDay)
	{
		(*date) = 1;
		++(*month);
		if((*month) > 12)
		{
			(*month) = 1;
			++(*year);
		}
	}
}
#define THREAD_CLOCK_TICK 	(1000)



void ThreadClock(void * param)
{
	FYF_VDEC_STATE_e vdec_state;
	BU32 vdec_state_stop = FYF_FALSE;
	BU32 count = 0;
	BU32 timeStart = 0;
	BU32 timeUsed = 0;
	BU32 ota_check_times = 0;

	while (1)
	{
		if (!bIsClockRun)
		{

			timeStart = FYF_API_time_ms();
			g_MSec += THREAD_CLOCK_TICK;
			if(g_MSec >= 1000)
			{
				g_Sec++;
				g_MSec = 0;
			#ifdef APP_CONFIG_PAT_MONITOR
				if ((APP_Search_API_GetStopState() == 1)
					&&(APP_Search_Get_OTA_Status()==1))
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_GETPATONTIME, 0, 0);
			#endif
			
			#ifdef APP_CONFIG_PMT_MONITOR
				if ((APP_Search_API_GetStopState() == 1)
					&&(APP_Search_Get_OTA_Status()==1))
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_GETPMTONTIME, 0, 0);
			#endif
			#ifdef APP_CONFIG_SDT_MONITOR
				if ((APP_Search_API_GetStopState() == 1)
					&&(APP_Search_Get_OTA_Status()==1))
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_GETSDTONTIME, 0, 0);
			#endif
			}
			if (g_Sec >= 60)
			{
				++g_Minute;
				g_Sec = 0;
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWSYSTIME, 0, 0);
			}
			if (g_Minute >= 60)
			{
				++g_Hour;
				g_Minute = 0;
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWSYSTIME, 0, 0);
			}
			if (g_Hour >= 24)
			{
				g_Hour = 0;
				APP_IPI_GotoNextDay(&g_Year, &g_Month, &g_Date, &g_Week);
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWSYSTIME, 0, 0);
			}

			if((g_Sec == 0) || (g_Sec == 30))
			{
				APP_PRINT_DEBUG("time : %02d:%02d.%02d\n", g_Hour, g_Minute, g_Sec);
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_ISRESERVEONTIME, 0, 0);
			}
#if APP_CONFIG_NIT_FULL_MONITOR
			/*NIT Monitor operation*/
#if defined (CD_CA)
			if ((CDCA_API_FinishNitFilter() == FYF_TRUE)
				&&(APP_Search_API_GetStopState() == 1)
				&&(APP_Search_Get_OTA_Status()==1))
				//&&(ota_check_times >= APP_NIT_MONITOR_SLEEP_TIME_SEC))
			{
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_GETNITOTAONTIME, 0, 0);
				//APP_Search_API_DetectOTA();
				//ota_check_times = 0;
			}
#else
			if ((APP_Search_API_GetStopState() == 1)
				&&(APP_Search_Get_OTA_Status()==1))
				//&&(ota_check_times >= APP_NIT_MONITOR_SLEEP_TIME_SEC))
			{
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_GETNITOTAONTIME, 0, 0);
				//APP_Search_API_DetectOTA();
				//ota_check_times = 0;
			}
#endif
#endif
			ota_check_times++;
			timeUsed = FYF_API_time_ms() - timeStart;
			if(timeUsed > THREAD_CLOCK_TICK)
			{
				continue;
			}
			FYF_API_thread_sleep(THREAD_CLOCK_TICK - timeUsed);
		}
		else
		{
			FYF_API_thread_sleep(1000);
		}
	}
}


void StartClockThread(void)
{
#ifndef _STOP_APP_THREAD_
	FYF_API_thread_create("tClock", ThreadClock, FYF_NULL, FYF_THREAD_PRIORITY_CLOCK, FYF_THREAD_STACK_CLOCK);
#endif
}

/*
描述：获得当前时间（ 小时*60 + 分钟）
time_t timep;
struct tm* nowTime;
time(&timep);
nowTime = localtime(&timep);
return nowTime->tm_hour * 60 + nowTime->tm_min;
*/

BU32 APP_IPI_GetTime(void)
{
	return g_Hour * 60 + g_Minute;
}

void APP_IPI_SetTime(BU32 Year,BU32 Month, BU32 Date, BU32 Week, BU32 Hour, BU32 Min, BU32 Sec)
{
	g_Year = Year;
	g_Month = Month;
	g_Date = Date;
	g_Week = Week;
	g_Hour = Hour;
	g_Minute = Min;
	g_Sec = Sec;

	//ASSERT(g_Month <= 12);
	if(g_Month > 12)
	{
		g_Month = 12;
	}
	//ASSERT(g_Date <= 31);
	if(g_Date > 31)
	{
		g_Date = 31;
	}
	//ASSERT(g_Week <= 7);
	if(g_Week > 7)
	{
		g_Week = 7;
	}
	//ASSERT(g_Hour <= 24);
	if(g_Hour >= 24)
	{
		g_Hour %= 24;
	}
	//ASSERT(g_Minute <= 60);
	if(g_Minute >= 60)
	{
		g_Minute = 59;
	}
	//ASSERT(g_Sec <= 60);
	if(g_Sec >= 60)
	{
		g_Sec = 59;
	}
	//2009-11-27 cwd modify这里不能调用设置时间的函数，否则会导致获取系统时间的不准确
	//最好在按待机键的时候调用FYF_API_OS_Set_SysTime设置一次操作系统时间
	//FYF_API_OS_Set_SysTime(g_Year, g_Month, g_Date, g_Hour, g_Minute, g_Sec);
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWSYSTIME, 0, 0);
	FYF_API_printf("Set Time = %02d:%02d.%02d\n", g_Hour, g_Minute, g_Sec);
}
/*
描述：获得当前时间（ 小时*60*60 + 分钟*60 + 秒）
*/
BU32 APP_IPI_GetTimeWithSec(void)
{
	return g_Hour * 60 * 60 + g_Minute * 60 + g_Sec;
}

BU32 APP_IPI_GetDay(void)
{
	return 10000 * g_Year + 100 * g_Month + g_Date;
}













