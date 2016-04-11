/*
2010-04-26 cwd create
*/

#include <pthread.h>
#include "app_common.h"
#include "app_ipi_common.h"
#include "../enreach/evtc.h"

extern BU32 g_Year;
extern BU32 g_Month;
extern BU32 g_Date;
extern BU32 g_Minute;
extern BU32 g_Hour;
extern BU08 g_Sec;
extern BS08 *vaild_protocol_str;
extern BU32 g_play_times_mutex;
extern BU32 g_play_times_remaind;

extern BU08 move_id[1000];

EVTC_TS g_cur_evtc_ts;
EVTC_VOD g_cur_evtc_vod;
extern char *np_homepage1;
extern EVTC_TSLIST *Timeshiftlist;
extern BU32 g_play_timeshift_mutex;
static BS32 g_cur_speed = 1;//快进快退的倍数，1为正常速度
static BU32 g_speed_flag = 0;//0代表正常播放，1代表快进，2代表快退
BU08 host[100] = {0};
EnrichVod_DateTime_S g_curprog_start_datetime, g_curprog_end_datetime;
BU32 g_cur_pbrlength = 0;
BS32 g_curprog_totaltimelength = 0;//当前节目的时间总长 秒钟
BS32 g_curprog_playtimelength = 0; //当前节目已播放的时间长度
BU32 g_switch_falg = 0;
BU08 g_net_connect_flag = 1;

BS08 g_bEnterEnrichVod = 0;//是否进入时移

extern BU32 GUI_IpanelState;//是epg时移还是直播下的时移


BU32 g_enrichvod_checkerror_thread = 0;
void APP_EnrichVod_CheckError_Thread(void* pvArgv);

BU32 APP_EnrichVod_GetNetStatus()
{
	return g_net_connect_flag;
}

BS32 APP_EnrichVod_DataTime_Sub(EnrichVod_DateTime_S * datatime1,  EnrichVod_DateTime_S * datatime2)
{
	BS32 all_sec1, all_sec2;
	evtc_cau_all_secs(datatime1->year, datatime1->month, datatime1->date, datatime1->hour, datatime1->minute, datatime1->second, &all_sec1);
	evtc_cau_all_secs(datatime2->year, datatime2->month, datatime2->date, datatime2->hour, datatime2->minute, datatime2->second, &all_sec2);
	APP_PRINT_DEBUG("sec1 = %d %d\n", all_sec1, all_sec2);
	return (all_sec1 - all_sec2);
}


BS32 APP_EnrichVod_GetPlayTimeLength()
{
	return g_curprog_playtimelength;
}

BS32 APP_EnrichVod_GetTotalTimeLength()
{
	return g_curprog_totaltimelength;
}
BS32 APP_EnrichVod_GetCurSpeed()
{
	return g_cur_speed;
}

BS32 APP_EnrichVod_GetSpeedFlag()
{
	return g_speed_flag;
}

BS32 APP_EnrichVod_TransTime(BU08 *begintime, EnrichVod_DateTime_S *datetime)
{
	BS32 i = 0;
	BS32 sum = 0;
	if (begintime != NULL)
	{
		APP_PRINT_DEBUG("begintime = %s\n", begintime);
	}
	for (i = 0; i < 4; i++)
	{
		sum = sum * 10 + begintime[i] - '0'; 
	}

	datetime->year = sum;
	sum = 0;
	for (i = 4; i < 6; i++)
	{
		sum = sum * 10 + begintime[i] - '0'; 
	}
	datetime->month = sum;
	sum = 0;
	for (i = 6; i < 8; i++)
	{
		sum = sum * 10 + begintime[i] - '0'; 
	}
	datetime->date = sum;

	sum = 0;
	for (i = 8; i < 10; i++)
	{
		sum = sum * 10 + begintime[i] - '0'; 
	}
	datetime->hour= sum;
		sum = 0;
	for (i = 10; i < 12; i++)
	{
		sum = sum * 10 + begintime[i] - '0'; 
	}
	datetime->minute = sum;
		sum = 0;
	for (i = 12; i < 14; i++)
	{
		sum = sum * 10 + begintime[i] - '0'; 
	}
	datetime->second = sum;
	APP_PRINT_DEBUG("time = %d %d %d %d %d %d\n", datetime->year, datetime->month, datetime->date, datetime->hour, datetime->minute, datetime->second);
}


//判断用户是否被授权
BS32 APP_EnrichVod_UserVaild()
{
	BS32 ret;
	sprintf(host, "%s", np_homepage1);
	if((ret = evtc_register(host)) == envod_fail)
	{
		APP_PRINT_ERROR(" evtc_register err!! \n");
		return APP_FAIL;
	}
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_Init()
{
	BS32 ret;
	EVTC_INIT  INITPARA;
	sprintf(host, "%s", np_homepage1);

	if((ret = evtc_init(host, &INITPARA)) == envod_fail)
	{
		APP_PRINT_ERROR(" evtc_init err!! \n");
		return APP_FAIL;
	}
	else
	{
		if (g_enrichvod_checkerror_thread == 0)
		{
			g_enrichvod_checkerror_thread = FYF_API_thread_create("EnrichVod_CheckError", APP_EnrichVod_CheckError_Thread, NULL, 0, 0); 
		}
		regist_callback_task_destroy(evtc_task_destroy);
		//regist_callback_task_destroyself(evtc_task_destroyself);
		APP_PRINT_DEBUG("result = %d, %d, %d, %d, %d, %d, %d, %s\n", INITPARA.result, INITPARA.mainfreq,\
			INITPARA.maintsid, INITPARA.maintsid, INITPARA.sndfreq, INITPARA.sndtsid, INITPARA.hearttime, INITPARA.serverIP);
	}
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_DeInit()
{
	BS32 ret;
	ret = evtc_close();
	
	g_bEnterEnrichVod = 0;
	return ret;
}

BS32 APP_EnrichVod_GetCurVodInfo(EVTC_VOD *cur_evtc_vod)
{
	if (cur_evtc_vod == NULL)
	{
		return APP_FAIL;
	}
	FYF_API_mutex_lock(g_play_timeshift_mutex, 0);
	memcpy(cur_evtc_vod, &g_cur_evtc_vod, sizeof(EVTC_VOD));
	FYF_API_mutex_unlock(g_play_timeshift_mutex);
	return APP_SUCCESS;
}
BS32 APP_EnrichVod_GetCurTsInfo(EVTC_TS *cur_evtc_ts)
{
	if (cur_evtc_ts == NULL)
	{
		return APP_FAIL;
	}
	FYF_API_mutex_lock(g_play_timeshift_mutex, 0);
	memcpy(cur_evtc_ts, &g_cur_evtc_ts, sizeof(EVTC_TS));
	FYF_API_mutex_unlock(g_play_timeshift_mutex);
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_UpdateCurTsInfo(BU16 vpid, BU16 apid, BU16 pcrpid)
{
	FYF_API_mutex_lock(g_play_timeshift_mutex, 0);
	g_cur_evtc_ts.apid = apid;
	g_cur_evtc_ts.vpid = vpid;
	g_cur_evtc_ts.pcrpid = pcrpid;
	FYF_API_mutex_unlock(g_play_timeshift_mutex);
	return APP_SUCCESS;
}


BS32 APP_EnrichVod_UpdateCurVodInfo(BU16 vpid, BU16 apid, BU16 pcrpid)
{
	FYF_API_mutex_lock(g_play_timeshift_mutex, 0);
	if (GUI_IpanelState)
	{
		g_cur_evtc_vod.apid = apid;
		g_cur_evtc_vod.vpid = vpid;
		g_cur_evtc_vod.pcrpid = pcrpid;
	}
	else
	{
		g_cur_evtc_ts.apid = apid;
		g_cur_evtc_ts.vpid = vpid;
		g_cur_evtc_ts.pcrpid = pcrpid;
	}
	FYF_API_mutex_unlock(g_play_timeshift_mutex);
	return APP_SUCCESS;
}
BS32 APP_EnrichVod_PlayTimeShift()
{
	FYF_QUEUE_MESSAGE msgSend;
	BS32 ret;

	BS32 index = evtc_get_curtsindex();
	if (index < 0) return APP_FAIL;
	FYF_API_mutex_lock(g_play_timeshift_mutex, 0);
	if ((ret = evtc_relay(host, Timeshiftlist[index].freq, Timeshiftlist[index].serviceid, &g_cur_evtc_ts)) == envod_fail)
	{
		APP_PRINT_ERROR(" evtc_relay err!! \n");
		FYF_API_mutex_unlock(g_play_timeshift_mutex);
		return APP_FAIL;
	}
	else
	{
		APP_PRINT_DEBUG("RELAYPARA playmod= %d, feq = %d, qam = %d, symbolrate = %d, vpid = %d, apid = %d, pcrpid = %d, pmtpid = %d, serviceid = %d, begintime = %s, endtime = %s, curtime = %s, prevprog = %s, curprog = %s ,nexprog = %s\n", g_cur_evtc_ts.playmod, g_cur_evtc_ts.freq, g_cur_evtc_ts.qam, g_cur_evtc_ts.symbolrate, \
					g_cur_evtc_ts.vpid, g_cur_evtc_ts.apid, g_cur_evtc_ts.pcrpid, g_cur_evtc_ts.pmtpid, g_cur_evtc_ts.serviceid,g_cur_evtc_ts.begintime, g_cur_evtc_ts.endtime, g_cur_evtc_ts.curtime, g_cur_evtc_ts.preprg, g_cur_evtc_ts.curprg,g_cur_evtc_ts.nextprg);
		APP_EnrichVod_TransTime(g_cur_evtc_ts.begintime, &g_curprog_start_datetime);
		APP_EnrichVod_TransTime(g_cur_evtc_ts.endtime, &g_curprog_end_datetime);
	}
	FYF_API_mutex_unlock(g_play_timeshift_mutex);

	FYF_API_mutex_lock(g_play_times_mutex, 0);
	g_play_times_remaind++;
	FYF_API_mutex_unlock(g_play_times_mutex);
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PlayTimeShift;
	msgSend.q2ndWordOfMsg= 0;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	
	
	return APP_SUCCESS;
}


BS32 APP_EnrichVod_PlayVod(BS32 moveid)
{
	FYF_QUEUE_MESSAGE msgSend;
	BS32 ret;
	char buf[100] = "channel_0220100506090600&mtv_vodip=10.10.0.20&return_url=http%3A%2F%2F10.10.0.11%2Feums%2Fclient%2Ftemplate17%2Fshiyi%2Fdshk-read.ecgi%3Fmd%3D1%26selected_tvstation%3DCCTV-3";
	FYF_API_mutex_lock(g_play_timeshift_mutex, 0);
	APP_PRINT_DEBUG("vaild_protocol_str = %s\n", move_id);
	if ((ret = evtc_vodplay(host, move_id, &g_cur_evtc_vod)) == envod_fail)
	{
		APP_PRINT_ERROR(" evtc_vodplay err!! \n");
		FYF_API_mutex_unlock(g_play_timeshift_mutex);
		return APP_FAIL;
		
	}
	else
	{
		APP_PRINT_DEBUG("g_cur_evtc_vod %d %d %d %d %d %d %d %d %d %d,  begintime = %s, endtime = %s, curtime = %s, prevprog = %s, curprog = %s ,nexprog = %s\n", g_cur_evtc_vod.result, g_cur_evtc_vod.freq, g_cur_evtc_vod.symbolrate, g_cur_evtc_vod.qam,\
			g_cur_evtc_vod.pmtpid, g_cur_evtc_vod.serviceid, g_cur_evtc_vod.apid, g_cur_evtc_vod.vpid, g_cur_evtc_vod.pcrpid, g_cur_evtc_vod.playmod, g_cur_evtc_vod.begintime, g_cur_evtc_vod.endtime, g_cur_evtc_vod.curtime, g_cur_evtc_vod.preprg,\
			g_cur_evtc_vod.curprg, g_cur_evtc_vod.nextprg);
		APP_EnrichVod_TransTime(g_cur_evtc_vod.begintime, &g_curprog_start_datetime);
		APP_EnrichVod_TransTime(g_cur_evtc_vod.endtime, &g_curprog_end_datetime);
	}
	FYF_API_mutex_unlock(g_play_timeshift_mutex);

		FYF_API_mutex_lock(g_play_times_mutex, 0);
	g_play_times_remaind++;
	FYF_API_mutex_unlock(g_play_times_mutex);
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PlayVod;
	msgSend.q2ndWordOfMsg= 0;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	
	
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_Stop()
{
	return evtc_end(host, 0);
}

BS32 APP_EnrichVod_Pause()
{
	return evtc_pause (host, 0);
}

BS32 APP_EnrichVod_Resume()
{
	g_cur_speed = 1;
	g_speed_flag = 0;
	return evtc_replay (host, 0);
}

BS32 APP_EnrichVod_Speed()
{
	g_cur_speed = evtc_ff(host, 0) * 2;
	if (g_cur_speed == APP_FAIL)
	{
		APP_PRINT_ERROR("APP_EnrichVod_Speed error\n");
		g_cur_speed = 1;
		
	}
 	if (g_cur_speed < 0 || g_cur_speed > 16)
 		g_cur_speed = 2;
	
	g_speed_flag = 1;
	APP_PRINT_DEBUG("g_cur_speed = %d %d\n", g_cur_speed, g_speed_flag);
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_Rewind()
{
	g_cur_speed = evtc_fb(host, 0) * 2;
	if (g_cur_speed == APP_FAIL)
	{
		APP_PRINT_ERROR("APP_EnrichVod_Speed error\n");
		g_cur_speed = 1;
		
	}
	if (g_cur_speed < 0 || g_cur_speed > 16)
 		g_cur_speed = 2;
	g_speed_flag = 2;
	APP_PRINT_DEBUG("g_cur_speed = %d\n", g_cur_speed);
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_SetShiftTime(EnrichVod_DateTime_S *datetime)
{
	BU08 timestr[15]; 
	BS32 ret;
	EnrichVod_DateTime_S cur_datatime;
	cur_datatime.year = g_Year;
	cur_datatime.month = g_Month;
	cur_datatime.date = g_Date;
	cur_datatime.hour = g_Hour;
	cur_datatime.minute = g_Minute;
	cur_datatime.second = g_Sec;
	ret = APP_EnrichVod_DataTime_Sub(datetime, &cur_datatime);
	sprintf(timestr, "%04d%02d%02d%02d%02d%02d",datetime->year, datetime->month, datetime->date,\
		datetime->hour, datetime->minute, datetime->second);
	APP_PRINT_ERROR("g_Year = %d, g_Month = %d, timestr = %s, ret = %d\n", g_Year, g_Month, timestr, ret);
	if (ret >= -60)
	{
		return APP_FAIL;
	}

	return evtc_settime(host, timestr, 0);

}

BS32 APP_EnrichVod_PlayTimeAdd(EnrichVod_DateTime_S *datetime, BU32 *move_time_len)
{
	BU32 tmp;
	BU32 i = datetime->year;
	tmp = g_curprog_totaltimelength - *move_time_len;
	BU16 monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	if (i % 4 == 0)
	{
		if (i % 100 != 0)
		{
			monthDays[1]++;
		}
		else if (i % 400 == 0)
		{
			monthDays[1]++;
		}
	}
	APP_PRINT_DEBUG("move_time_len = %d\n", *move_time_len);
	if (tmp == 0 || *move_time_len == 0)
	{
		return;
	}
	
	if (tmp > 60)
	{
		*move_time_len += 60;
		datetime->minute++;
		if (datetime->minute == 60)
		{
			datetime->hour++;
			datetime->minute = 0;
		}

		if (datetime->hour == 24)
		{
			datetime->hour = 0;
			datetime->date++;
		}

		if (datetime->date > monthDays[datetime->month - 1])
		{
			datetime->date = 1;
			datetime->month++;
		}

		if (datetime->month == 13)
		{
			datetime->year++;
			datetime->month = 1;
		}
	}
	else
	{
		*move_time_len += (tmp - 1);
		datetime->second += (tmp - 1);
		if (datetime->second >= 60)
		{
			datetime->second -= 60;
			datetime->minute++;
			if (datetime->minute == 60)
			{
				datetime->hour++;
				datetime->minute = 0;
			}

			if (datetime->hour == 24)
			{
				datetime->hour = 0;
				datetime->date++;
			}

			if (datetime->date > monthDays[datetime->month - 1])
			{
				datetime->date = 1;
				datetime->month++;
			}

			if (datetime->month == 13)
			{
				datetime->year++;
				datetime->month = 1;
			}
		}
	}
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_PlayTimeSub(EnrichVod_DateTime_S *datetime, BU32 *move_time_len)
{
	BU32 i = datetime->year;
	BU16 monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	if (i % 4 == 0)
	{
		if (i % 100 != 0)
		{
			monthDays[1]++;
		}
		else if (i % 400 == 0)
		{
			monthDays[1]++;
		}
	}
	APP_PRINT_DEBUG("move_time_len = %d\n", *move_time_len);
	if (*move_time_len == 0)
	{
		return;
	}
	
	if (*move_time_len > 60)
	{
		*move_time_len -= 60;
		if (datetime->minute == 0)
		{
			datetime->minute = 59;
			if (datetime->hour == 0)
			{
				datetime->hour = 23;
				datetime->date--;

				if (datetime->date == 0)
				{
					datetime->month --;
					if (datetime->month == 0)
					{
						datetime->month = 12;
						datetime->year --;
					}
					datetime->date = monthDays[datetime->month - 1];
				}
			}
			else
			{
				datetime->hour --;
			}

		}
		else
		{
			datetime->minute --;
		}
	}
	else
	{
		if (datetime->second < (*move_time_len - 1))
		{
			datetime->second = 60 + datetime->second - (*move_time_len - 1);
			*move_time_len = 1;
			if (datetime->minute == 0)
			{
				datetime->minute = 59;
				if (datetime->hour == 0)
				{
					datetime->hour = 23;
					datetime->date--;
					if (datetime->date == 0)
					{
						datetime->month --;
						if (datetime->month == 0)
						{
							datetime->month = 12;
							datetime->year --;
						}
						datetime->date = monthDays[datetime->month - 1];
					}
				}
				else
				{
					datetime->hour --;
				}
			}
			else
			{
				datetime->minute --;
			}
			
		}
		else
		{
			datetime->second -= (*move_time_len - 1);
			*move_time_len = 1;
		}
	}

	return APP_SUCCESS;

}

BS32 APP_EnrichVod_GetPBRPosition()
{
	
	return g_cur_pbrlength;
}


BS32 APP_EnrichVod_GetCurProgStartTime(EnrichVod_DateTime_S *datetime)
{
	memcpy(datetime, &g_curprog_start_datetime, sizeof(EnrichVod_DateTime_S));
	return APP_SUCCESS;
}
BS32 APP_EnrichVod_GetCurProgEndTime(EnrichVod_DateTime_S *datetime)
{
	memcpy(datetime, &g_curprog_end_datetime, sizeof(EnrichVod_DateTime_S));
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_GetProgAttr(APP_EnrichVod_ATTR_TYPE_E type, BU08 *prog_name)
{
	if (prog_name == NULL)
	{
		APP_PRINT_ERROR("APP_EnrichVod_GetProgAttr error!\n");
		return APP_FAIL;
	}
	switch(type)
	{
	case APP_EnrichVod_ATTR_PrevProg_TYPE:
		if (GUI_IpanelState)
		{
			memcpy(prog_name, g_cur_evtc_vod.preprg, 20);
		}
		else
		{
			memcpy(prog_name, g_cur_evtc_ts.preprg, 20);
		}
		break;
	case APP_EnrichVod_ATTR_CurProg_Type:
		if (GUI_IpanelState)
		{
			memcpy(prog_name, g_cur_evtc_vod.curprg, 20);
		}
		else
		{
			memcpy(prog_name, g_cur_evtc_ts.curprg, 20);
		}
		break;
	case APP_EnrichVod_ATTR_NextProg_Type:
		if (GUI_IpanelState)
		{
			memcpy(prog_name, g_cur_evtc_vod.nextprg, 20);
		}
		else
		{
			memcpy(prog_name, g_cur_evtc_ts.nextprg, 20);
		}
		break;
	}
	return APP_SUCCESS;
}
BS32 APP_EnrichVod_GetPlayTime(EnrichVod_DateTime_S *datetime)
{
	BU08 curtime[15];
	BS32 ret;
	BS32 time1, time2;
	if (evtc_getcurtime(host, 0, curtime) != envod_fail)
	{
		APP_EnrichVod_TransTime(curtime, datetime);
		g_curprog_totaltimelength = ((g_curprog_end_datetime.hour - g_curprog_start_datetime.hour + 24)%24) * 3600 + (g_curprog_end_datetime.minute - g_curprog_start_datetime.minute) * 60\
			+ (g_curprog_end_datetime.second - g_curprog_start_datetime.second);
		if (g_curprog_totaltimelength <= 0)g_curprog_totaltimelength = 60;
		g_curprog_playtimelength = ((datetime->hour - g_curprog_start_datetime.hour + 24)%24) * 3600 + (datetime->minute - g_curprog_start_datetime.minute) * 60\
			+ (datetime->second - g_curprog_start_datetime.second);
		//if (g_curprog_playtimelength <= 0)g_curprog_playtimelength = 60;
		g_cur_pbrlength = (g_curprog_playtimelength * 101) / g_curprog_totaltimelength;
		if (g_cur_pbrlength > 101)g_cur_pbrlength = 101;
	}
	else
	{
		return APP_FAIL;
	}
	return APP_SUCCESS;
}


BS32 APP_EnrichVod_BeNext()
{
	BS32 ret;
	EnrichVod_DateTime_S datatime;
	datatime.year = g_Year;
	datatime.month = g_Month;
	datatime.date = g_Date;
	datatime.hour = g_Hour;
	datatime.minute = g_Minute;
	datatime.second = g_Sec;
	ret = APP_EnrichVod_DataTime_Sub(&g_curprog_end_datetime, &datatime);
	if (ret >= -60)
	{
		return APP_FAIL;
	}
	return APP_SUCCESS;
}
BS32 APP_EnrichVod_PlayNext()
{
	BS32 ret;
	EnrichVod_DateTime_S datatime;
	FYF_QUEUE_MESSAGE msgSend;
	EVTC_TS tmpcur_evtc_ts;
	FYF_API_mutex_lock(g_play_timeshift_mutex, 0);
	g_switch_falg = 2;
	if ((ret = evtc_pgdn(host,&tmpcur_evtc_ts)) == envod_fail)
	{
		APP_PRINT_ERROR(" evtc_pgdn err!! \n");
		FYF_API_mutex_unlock(g_play_timeshift_mutex);
		return APP_FAIL;
	}
	else
	{
		memcpy(&g_cur_evtc_ts, &tmpcur_evtc_ts, sizeof(EVTC_TS));
		APP_PRINT_DEBUG("RELAYPARA playmod= %d, feq = %d, qam = %d, symbolrate = %d, vpid = %d, apid = %d, pcrpid = %d, pmtpid = %d, serviceid = %d, begintime = %s, endtime = %s, curtime = %s, prevprog = %s, curprog = %s ,nexprog = %s\n", g_cur_evtc_ts.playmod, g_cur_evtc_ts.freq, g_cur_evtc_ts.qam, g_cur_evtc_ts.symbolrate, \
			g_cur_evtc_ts.vpid, g_cur_evtc_ts.apid, g_cur_evtc_ts.pcrpid, g_cur_evtc_ts.pmtpid, g_cur_evtc_ts.serviceid, g_cur_evtc_ts.begintime, g_cur_evtc_ts.endtime, g_cur_evtc_ts.curtime, g_cur_evtc_ts.preprg, g_cur_evtc_ts.curprg,g_cur_evtc_ts.nextprg);
		APP_EnrichVod_TransTime(g_cur_evtc_ts.begintime, &g_curprog_start_datetime);
		APP_EnrichVod_TransTime(g_cur_evtc_ts.endtime, &g_curprog_end_datetime);
	}
	FYF_API_mutex_unlock(g_play_timeshift_mutex);
	datatime.year = g_Year;
	datatime.month = g_Month;
	datatime.date = g_Date;
	datatime.hour = g_Hour;
	datatime.minute = g_Minute;
	datatime.second = g_Sec;
	ret = APP_EnrichVod_DataTime_Sub(&g_curprog_start_datetime, &datatime);
	if (ret >= -60)
	{
		return APP_FAIL;
	}

	FYF_API_mutex_lock(g_play_times_mutex, 0);
	g_play_times_remaind++;
	FYF_API_mutex_unlock(g_play_times_mutex);
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PlayTimeShift;
	msgSend.q2ndWordOfMsg= 0;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
}

BS32 APP_EnrichVod_PlayPrev()
{
	BS32 ret;
	FYF_QUEUE_MESSAGE msgSend;
	EVTC_TS tmpcur_evtc_ts;
	FYF_API_mutex_lock(g_play_timeshift_mutex, 0);
	g_switch_falg = 2;
	if ((ret = evtc_pgup(host,&tmpcur_evtc_ts)) == envod_fail)
	{
		APP_PRINT_ERROR(" evtc_pgup err!! \n");
		FYF_API_mutex_unlock(g_play_timeshift_mutex);
		return APP_FAIL;
	}
	else
	{
		memcpy(&g_cur_evtc_ts, &tmpcur_evtc_ts, sizeof(EVTC_TS));
		APP_PRINT_DEBUG("RELAYPARA playmod= %d, feq = %d, qam = %d, symbolrate = %d, vpid = %d, apid = %d, pcrpid = %d, pmtpid = %d, serviceid = %d, begintime = %s, endtime = %s, curtime = %s, prevprog = %s, curprog = %s ,nexprog = %s\n", g_cur_evtc_ts.playmod, g_cur_evtc_ts.freq, g_cur_evtc_ts.qam, g_cur_evtc_ts.symbolrate, \
					g_cur_evtc_ts.vpid, g_cur_evtc_ts.apid, g_cur_evtc_ts.pcrpid, g_cur_evtc_ts.pmtpid, g_cur_evtc_ts.serviceid, g_cur_evtc_ts.begintime, g_cur_evtc_ts.endtime, g_cur_evtc_ts.curtime, g_cur_evtc_ts.preprg, g_cur_evtc_ts.curprg,g_cur_evtc_ts.nextprg);
		APP_EnrichVod_TransTime(g_cur_evtc_ts.begintime, &g_curprog_start_datetime);
		APP_EnrichVod_TransTime(g_cur_evtc_ts.endtime, &g_curprog_end_datetime);
	}
	FYF_API_mutex_unlock(g_play_timeshift_mutex);
	FYF_API_mutex_lock(g_play_times_mutex, 0);
	g_play_times_remaind++;
	FYF_API_mutex_unlock(g_play_times_mutex);
	
	msgSend.qlstWordOfMsg= (unsigned int)APP_Queue_PlayTimeShift;
	msgSend.q2ndWordOfMsg= 0;
	FYF_API_queue_send(APP_IPI_GetQueuePlayChn(), &msgSend);
	return APP_SUCCESS;
}

APP_EnrichVod_Error_Type APP_EnrichVod_CheckError()
{
	EnrichVod_DateTime_S datetime;
	APP_EnrichVod_GetPlayTime(&datetime);
	BS32 ret =  APP_EnrichVod_GetPBRPosition();
	APP_PRINT_ERROR("ret = %d\n", ret);

	if (ret == 0)
	{
		return APP_EnrichVod_Error_PlayStart;
	}
	else if (ret >= 100)
	{
		//if (g_speed_flag == 1)
		return APP_EnrichVod_Error_PlayEnd;
	}
	return APP_EnrichVod_Error_None;
}


void APP_EnrichVod_CheckError_Thread(void* pvArgv)
{
	BS32 cur_state;
	EnrichVod_DateTime_S datetime, datatime2;
	BS32 ret ;
	static BU08 old_connect_status = 1;
	while(1)
	{
		
		FYF_API_thread_sleep(2000);
		cur_state = FULLTOTVOD_GetCurState();
		if (cur_state == -1)
		{
			continue;
		}
		
		if (g_switch_falg > 0)
		{
			g_switch_falg --;
			continue;
		}
		
		g_net_connect_flag = FYF_API_GetLinkStatus();
		APP_PRINT_DEBUG("g_net_connect_flag = %d", g_net_connect_flag);
		if (g_net_connect_flag && !old_connect_status)
		{
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_Show_EnrichVodError, APP_EnrichVod_Error_NetConnect_Error, 0);
			old_connect_status = g_net_connect_flag;
			continue;
		}
		else if (!g_net_connect_flag && old_connect_status)
		{
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_Show_EnrichVodError, APP_EnrichVod_Error_NetUnConnect_Error, 0);
			old_connect_status = g_net_connect_flag;
			continue;
		}

		if (envod_is_exit())
		{
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_Show_EnrichVodError, APP_EnrichVod_Error_PlayError, 0);
			envod_set_exit(0);
		}
		
		
#if 0		
		switch(cur_state)
		{
		case FULLTOTVOD_NULL_STATE:
		case TVOD_VOL_STATE:
		case TVOD_HELP_STATE:
		case PP_EXIT_STATE:
			APP_EnrichVod_GetPlayTime(&datetime);
			ret = APP_EnrichVod_DataTime_Sub(&datetime, &g_curprog_start_datetime);
			APP_PRINT_ERROR("ret = %d\n", ret);
			if (ret == 0)
			{
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_Show_EnrichVodError, APP_EnrichVod_Error_PlayEnd, 0);
			}
			break;
		case FAST_STATE:
			if (g_speed_flag == 1)
			{
				APP_EnrichVod_GetPlayTime(&datetime);
				datatime2.year = g_Year;
				datatime2.month = g_Month;
				datatime2.date = g_Date;
				datatime2.hour = g_Hour;
				datatime2.minute = g_Minute;
				datatime2.second = g_Sec;
				ret = APP_EnrichVod_DataTime_Sub(&datetime, &datatime2);
				APP_PRINT_ERROR("ret = %d\n", ret);
				if (ret >= -60)
				{
					GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_Show_EnrichVodError, APP_EnrichVod_Error_PlayReach, 0);
					break;
				}
				ret = APP_EnrichVod_DataTime_Sub(&datetime, &g_curprog_start_datetime);
					APP_PRINT_ERROR("ret = %d\n", ret);
				if (ret == 0)
				{
					GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_Show_EnrichVodError, APP_EnrichVod_Error_PlayEnd, 0);

				}
			}
			else if (g_speed_flag == 2)
			{
				APP_EnrichVod_GetPlayTime(&datetime);
			
				ret = APP_EnrichVod_DataTime_Sub(&datetime, &g_curprog_start_datetime);
				APP_PRINT_ERROR("ret = %d\n", ret);
				if (ret > 0 && ret <= 2)
				{
					GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_Show_EnrichVodError, APP_EnrichVod_Error_PlayStart, 0);

				}
			}
			break;
			
		}
#endif
	}

}
	

