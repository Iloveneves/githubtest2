#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"



#define NOVD_PLAY_TIMER_PERIOD 	1000

#define NVOD_EVENT_NUM_OF_PAGE     (4)
#define NVOD_EVENT_TIME_NUM_OF_PAGE   (4)

extern BU16 nvodPlayServiceId;
extern BU32	semaphoreNvod ;
extern BU08 g_epg_reserveStr[4][40];
extern RESERVE_TYPE_E g_epg_reserveType;
extern BU32 serviceInfo_cas_other;
extern BU32 semaphoreEcm;

static BU32 g_nvod_event_num = 0;
static BU32 g_nvod_event_page = 0;
static BU32 g_nvod_event_rest = 0;
static BU32 g_nvod_event_curPage = 0;
static BU08 g_nvod_event_item[NVOD_EVENT_NUM_OF_PAGE][17];
static BU08 *g_nvod_event_item_str[NVOD_EVENT_NUM_OF_PAGE]=
{
	g_nvod_event_item[0],
	g_nvod_event_item[1],
	g_nvod_event_item[2],
	g_nvod_event_item[3],
};

static BU32 g_nvod_event_time_num=0;
static BU32 g_nvod_event_time_page = 0;
static BU32 g_nvod_event_time_rest = 0;
static BU32 g_nvod_event_time_curPage = 0;
static BU08 g_nvod_event_time_item[NVOD_EVENT_TIME_NUM_OF_PAGE][40];
static BU08 *g_nvod_event_time_item_str[NVOD_EVENT_TIME_NUM_OF_PAGE]=
{
	g_nvod_event_time_item[0],
	g_nvod_event_time_item[1],
	g_nvod_event_time_item[2],
	g_nvod_event_time_item[3],
};

APP_NvodReserve_S nvodReserveInfo;
static BU08 nvodProgName[NVOD_MAX_EVENT_NAME_LENGTH+1];
static BU32 toReserveListPreFocus = ID_LBP_EventList_Nvod;
//NvodPlay
static BU08 duringTimeStr[20];
static BU08 playTimeStr[6];
static BU08 curTimeStr[6];
static BU08 curPlayProgressStr[5];
static BU32 event_time_index = 0;
static BU32 nvod_lastKeyState = 0;
static BU32 nvod_numKeyTime = 0;
static BU08 nvod_strTemp[40];
static BU32 progInfobShow = 0;
static BU32 g_nvod_play_curState = NVOD_FULLPLAY_NULL_STATE;
static BU08 timeStr[10];
static BU32 startTime = 0;
static BU32 duringTime = 0;
static BU32 dateTime = 0;
//进入Nvod时等待界面
static BU32 timerNum = 0;
static BU32 progress = 0;

BU32 nvodPlayFlag = 0;
extern BU16 nvodPlayServiceId;

void Nvod_IPI_EventList_SetPageParam(void)
{
	g_nvod_event_num = APP_Nvod_GetAllShiftServiceNumAndIndex();
	
	if(g_nvod_event_num)
	{
		g_nvod_event_page = (g_nvod_event_num-1)/NVOD_EVENT_NUM_OF_PAGE+1;
		g_nvod_event_rest = g_nvod_event_num%NVOD_EVENT_NUM_OF_PAGE;
		if(g_nvod_event_rest == 0)
		{
			g_nvod_event_rest = NVOD_EVENT_NUM_OF_PAGE;
		}
		g_nvod_event_curPage = 0;
		GUI_ENC_API_SetValue(ID_SBR_EventList_Nvod, WDT_SBR_SV_MAX, g_nvod_event_num-1, 0);
		GUI_ENC_API_SetValue(ID_SBR_EventList_Nvod, WDT_SBR_SV_MIN, 0, 0);
		GUI_ENC_API_SetValue(ID_SBR_EventList_Nvod, WDT_SBR_SV_VALUE, 0, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValue(ID_STA_MovieNameContent_Nvod, WDT_STA_SV_STRING, (BU32)"", 0);
		GUI_ENC_API_SetValue(ID_STA_MovieTimeLengContent_Nvod, WDT_STA_SV_STRING, (BU32)"", 0);
		GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_VALUE, (BU32)"", 0);
	}
	
}

void Nvod_IPI_EventList_ShowCurPage(void)
{
	BU32 index = 0;
	BU32 itemIndex = 0;
	BU32 itemNum = 0;
	APP_Nvod_Program_List_S shiftTimeService;
	
	if(0 == g_nvod_event_num)
	{
		return;
	}
	if(g_nvod_event_curPage >= g_nvod_event_page)
	{
		g_nvod_event_curPage = 0;
	}
	if(g_nvod_event_curPage+1 == g_nvod_event_page)
	{	
		itemNum = g_nvod_event_rest;
	}
	else
	{
		itemNum = NVOD_EVENT_NUM_OF_PAGE;
	}

	for(index=0;index<itemNum;index++)
	{
		itemIndex = g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE+index;
		if(itemIndex < g_nvod_event_num)
		{
			if(APP_SUCCESS == APP_Nvod_GetShiftServiceNodeByIndex(itemIndex,&shiftTimeService))
			{
				#if 0
				if(STATUS_RUNNING == shiftTimeService.uProgRunStatus)
				{
					sprintf(g_nvod_event_item_str[index], "%02d.%s(播出中)",itemIndex+1,shiftTimeService.acProgNvodName);
				}
				else
				{
					sprintf(g_nvod_event_item_str[index], "%02d.%s",itemIndex+1,shiftTimeService.acProgNvodName);
				}
				#endif
				GUI_API_Memcpy(g_nvod_event_item_str[index], shiftTimeService.acProgNvodName, 16);
				g_nvod_event_item_str[index][16] = 0;
				APP_Printf("index = %d %d %s\n", index, itemIndex,  g_nvod_event_item_str[index]);
			}
			else
			{
				g_nvod_event_item_str[index][0] = " ";
				g_nvod_event_item_str[index][1] = 0;
			}
		}
	}
	GUI_ENC_API_SetValue (ID_LBP_EventList_Nvod, WDT_LBP_SV_DATA, (BU32)g_nvod_event_item_str, itemNum);
}
void NVOD_IPI_ShowEventDetail(void)
{
	BU32 index;
	BU32 len;
	BU32 lang;
	APP_Nvod_Program_List_S shiftTimeService;
	memset(duringTimeStr,0,20);

	GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, &index, 0);
	if(APP_SUCCESS == APP_Nvod_GetShiftServiceNodeByIndex(index+g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE,&shiftTimeService))
	{
		GUI_API_Memset(nvodProgName,0,NVOD_MAX_EVENT_NAME_LENGTH+1);
		GUI_API_Strcpy(nvodProgName, shiftTimeService.acProgNvodName);
		GUI_ENC_API_SetValue (ID_STA_MovieNameContent_Nvod, WDT_STA_SV_STRING, nvodProgName, 0);
		GUI_API_Memset(duringTimeStr,0,20);
		APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &lang);
		if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
		{
		 	sprintf(duringTimeStr,"%dMin",shiftTimeService.sDuringTime);
		}
		else
		{
			sprintf(duringTimeStr,"%d分钟",shiftTimeService.sDuringTime);
		}
		GUI_ENC_API_SetValue(ID_STA_MovieTimeLengContent_Nvod, WDT_STA_SV_STRING, duringTimeStr, 0);
		len = GUI_API_Strlen(shiftTimeService.acProgNvodText);
		if(len)
		{
			GUI_ENC_API_SetValue(ID_EDT_Nvod,WDT_EDT_SV_PAGE_INDEX,0,0);
			GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_VALUE, (BU32)(shiftTimeService.acProgNvodText), len);
			GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_SCROLL_STATE, WDT_EDT_SCROLL_STATE_PLAY, 0);
		}
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_MovieNameContent_Nvod, WDT_STA_SV_STRING, (BU32)"", 0);
		GUI_ENC_API_SetValue(ID_STA_MovieTimeLengContent_Nvod, WDT_STA_SV_STRING, (BU32)"", 0);
		GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_VALUE, (BU32)"", 0);
	}
	GUI_ENC_API_Update(ID_STA_MovieNameContent_Nvod, 1);
}

void Nvod_IPI_EventPlayTime_SetPageParam(void)
{
	BU32 index;
	APP_Nvod_Program_List_S shiftTimeService;
	GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, &index, 0);
	if(APP_SUCCESS == APP_Nvod_GetShiftServiceNodeByIndex(index+g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE,&shiftTimeService))
	{
		g_nvod_event_time_num = APP_Nvod_GetAllRefEventNumAndIndexByEventId(shiftTimeService.uProgNvodEventID);
		if(g_nvod_event_time_num)
		{
			g_nvod_event_time_page = (g_nvod_event_time_num-1)/NVOD_EVENT_TIME_NUM_OF_PAGE+1;
			g_nvod_event_time_rest = g_nvod_event_time_num%NVOD_EVENT_TIME_NUM_OF_PAGE;
			if(g_nvod_event_time_rest == 0)
			{
				g_nvod_event_time_rest = NVOD_EVENT_TIME_NUM_OF_PAGE;
			}
			g_nvod_event_time_curPage = 0;
			GUI_ENC_API_SetValue(ID_SBR_EventPlayTime_Nvod, WDT_SBR_SV_MAX, g_nvod_event_time_num-1, 0);
			GUI_ENC_API_SetValue(ID_SBR_EventPlayTime_Nvod, WDT_SBR_SV_MIN, 0, 0);
			GUI_ENC_API_SetValue(ID_SBR_EventPlayTime_Nvod, WDT_SBR_SV_VALUE, 0, 0);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_NUM, 0, 0);
		}
	}
}
void Nvod_IPI_EventPlayTime_ShowCurPage(void)
{
	BU32 index = 0;
	BU32 itemIndex = 0;
	BU32 itemNum = 0;
	BS32 psYear = 0;
	BS08 pcMonth = 0;
	BS08 pcDate = 0;
	BU32 year = 0;
	BU32 month =0;
	BU32 date = 0;
	BU08 week = 0;
	BU32 startHour;
	BU32 startMin;
	BU32 endTime;
	BU32 endHour;
	BU32 endMin;
	APP_Nvod_Prog_TimeInfo_S refEvent;
	BU32 time_zone_type;

	if(0 == g_nvod_event_time_num)
	{
		return;
	}
	if(g_nvod_event_time_curPage >= g_nvod_event_time_page)
	{
		g_nvod_event_time_curPage = 0;
	}
	if(g_nvod_event_time_curPage+1 == g_nvod_event_time_page)
	{	
		itemNum = g_nvod_event_time_rest;
	}
	else
	{
		itemNum = NVOD_EVENT_TIME_NUM_OF_PAGE;
	}
	for(index = 0;index<itemNum;index++)
	{
		itemIndex = g_nvod_event_time_curPage*NVOD_EVENT_TIME_NUM_OF_PAGE+index;
		if(itemIndex < g_nvod_event_time_num)
		{
			if(APP_SUCCESS == APP_Nvod_GetRefEventNodeByIndex(itemIndex,&refEvent))
			{
				/*	APP_TDT_TransCodeDateToNormalDate(refEvent.uEventDateCode, &psYear, &pcMonth, &pcDate);
				year = (BU32)psYear;
				month = (BU32)pcMonth;
				date = (BU32)pcDate;
				startHour = refEvent.uStartHour;
				if (startHour >= 24)
				{
					startHour = (startHour+24)%24;
					APP_IPI_GotoNextDay(&year, &month, &date, &week);
				}
				*/
				year = refEvent.uEventDateCode / 10000;
				month = (refEvent.uEventDateCode - year * 10000) / 100;
				date = (refEvent.uEventDateCode) % 100;
				startHour = refEvent.uStartHour;
				APP_Printf("year = %d, month = %d, date = %d %d\n", year ,month, date, startHour);
				startMin = refEvent.uStartMinute;
				endTime = startHour*60+startMin+refEvent.sDuringTime;
				endHour = (endTime/60+24)%24;
				endMin = endTime%60;
				if(RUNNING_PLAY == refEvent.uEventRunStatus)
				{
					//sprintf(g_nvod_event_time_item_str[index], "%02d.%02d年%02d月%02d日  %02d:%02d  播出中",itemIndex+1, psYear,pcMonth,pcDate,refEvent.uStartHour,refEvent.uStartMinute);
					sprintf(g_nvod_event_time_item_str[index], "%02d.%04d-%02d-%02d %02d:%02d-%02d:%02d  播出中",itemIndex+1,year,month,date,startHour,startMin,endHour,endMin);
				}
				else if(RUNNING_RESERVED == refEvent.uEventRunStatus)
				{
					//sprintf(g_nvod_event_time_item_str[index], "%02d.%02d年%02d月%02d日  %02d:%02d",itemIndex+1,psYear, pcMonth,pcDate,refEvent.uStartHour,refEvent.uStartMinute);
					sprintf(g_nvod_event_time_item_str[index], "%02d.%04d-%02d-%02d %02d:%02d-%02d:%02d  预约",itemIndex+1,year,month,date,startHour,startMin,endHour,endMin);
				}
				else
				{
					sprintf(g_nvod_event_time_item_str[index], "%02d.%04d-%02d-%02d %02d:%02d-%02d:%02d",itemIndex+1,year,month,date,startHour,startMin,endHour,endMin);
				}
				g_nvod_event_time_item_str[index][39] = 0;
			}
		}
	}
	GUI_ENC_API_SetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_DATA, (BU32)g_nvod_event_time_item_str, itemNum);
}

void Nvod_IPI_PlayProg(void)
{
	BU32 index;
	APP_Nvod_Prog_TimeInfo_S refEvent;
	BU32 ret;
	GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, &index, 0);
	if(APP_SUCCESS == APP_Nvod_GetRefEventNodeByIndex(index+g_nvod_event_time_curPage*NVOD_EVENT_TIME_NUM_OF_PAGE,&refEvent))
	{
		if(RUNNING_PLAY == refEvent.uEventRunStatus)
		{
			UI_APP_API_PlayByServiceID(refEvent.uProgNvodSeviceID, "nuod");
			//APP_Com_API_PlayNvodChn(refEvent.uProgNvodSeviceID);
			//FYF_API_time_get_pid(refEvent.uProgNvodSeviceID,&shiftServiceInfo);
			//APP_Printf("vpid = %d , apid = %d, pcrpid = %d\n",shiftServiceInfo.vpid, shiftServiceInfo.apid,shiftServiceInfo.pcrpid);
			//APP_IPI_PlayProg(shiftServiceInfo.vpid, shiftServiceInfo.apid,shiftServiceInfo.pcrpid,FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
		
		}
		else
		{
			//APP_Com_API_StopProg(1);
		}
	}
	else
	{
		APP_Com_API_StopProg(1);
	}
}

//播放最早的时移节目
void Nvod_IPI_PlayFirstProg()
{
	BU32 index = 0, ItemIndex = 0;
	BU32 u32ServiceID;
	BU32 count = 0;
	APP_Nvod_Prog_TimeInfo_S* tmp;
	APP_Nvod_Program_List_S shiftTimeService;
	GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, &index, 0);
	ItemIndex = g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE+index;
	if(APP_SUCCESS == APP_Nvod_GetShiftServiceNodeByIndex(ItemIndex, &shiftTimeService))
	{
		if (shiftTimeService.uProgRunStatus == STATUS_RUNNING)
		{
			tmp = shiftTimeService.pTimeInfo->next;
			while (1)
			{
				if (tmp->uEventRunStatus == RUNNING_PLAY)
				{
					count++;
					u32ServiceID = tmp->uProgNvodSeviceID;
					tmp = tmp->next;
				}	
				else
				{
					break;
				}
				if (tmp == NULL)
				{
					break;
				}
			}
			APP_PRINT_DEBUG("count = %d", count);
			GUI_ENC_API_SetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, count - 1, 0);
			UI_APP_API_PlayByServiceID(u32ServiceID, "nuod");
		}
		else
		{
			nvodPlayServiceId = 0;
			GUI_ENC_API_SetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, count, 0);
			APP_Com_API_StopProg(1);
		}
	}
	else
	{
		APP_Com_API_StopProg(1);
	}
	
}
void UI_APP_Nvod_Enter(void)
{
	BU32 index;
	BU32 hwnd;
	GUI_Rect_s rect;
	BU32 shiftNum = 0;
	BU32 preState;
	BU16 videoPID;
	BU16 audioPID;
	BU16 pcrPID;
	APP_Nvod_Prog_TimeInfo_S refEvent;
	FYF_time_shifted_service_s shiftServiceInfo;
	BU32 shiftServiceNum;
	nvodPlayFlag = 1;
	//APP_Com_API_StopProg(1);
	hwnd = GUI_API_GetDlgItem(ID_VFM_Nvod);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_CREATE_TIMER, 0, 0);
	preState = GUI_ENC_API_GetPrevState();
//	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	if(preState != ID_DLG_NvodWait)
	{
		if(APP_Nvod_GetAllShiftServiceNumAndIndex())
		{
			Nvod_IPI_EventList_ShowCurPage();
			NVOD_IPI_ShowEventDetail();
			//Nvod_IPI_EventPlayTime_SetPageParam();
			Nvod_IPI_EventPlayTime_ShowCurPage();
			if(preState == ID_DLG_NvodPlayInfo)
			{
				GUI_ENC_API_SetFocus(ID_LBP_EventList_Nvod);
			}
			else if(preState == ID_DLG_ReserveList)
			{
				GUI_ENC_API_SetFocus(toReserveListPreFocus);
			}
			else
			{
				GUI_ENC_API_SetFocus(ID_LBP_EventPlayTime_Nvod);
			}
		}
	}
	else
	{
		APP_Com_API_StopProg(1);
		shiftServiceNum = APP_Nvod_GetAllShiftServiceNumAndIndex();
		APP_Printf("shiftServiceNum = %d\n",shiftServiceNum);
		if(shiftServiceNum != 0)
		{
			Nvod_IPI_EventList_SetPageParam();
			Nvod_IPI_EventList_ShowCurPage();
			NVOD_IPI_ShowEventDetail();
			Nvod_IPI_EventPlayTime_SetPageParam();
			Nvod_IPI_EventPlayTime_ShowCurPage();
			Nvod_IPI_PlayFirstProg();
		}
		else
		{
			GUI_ENC_API_SetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_NUM, 0, 0);
			GUI_ENC_API_SetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_NUM, 0, 0);
			GUI_ENC_API_SetValue(ID_STA_MovieNameContent_Nvod, WDT_STA_SV_STRING, (BU32)"", 0);
			GUI_ENC_API_SetValue(ID_STA_MovieTimeLengContent_Nvod, WDT_STA_SV_STRING, (BU32)"", 0);
			GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_VALUE, (BU32)"", 0);
		}
	}
//	FYF_API_semaphore_release(semaphoreNvod);
}

void UI_APP_Nvod_End(void)
{
	nvodPlayFlag = 0;
	APP_Nvod_API_StopGetNvodProg();
	GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_SCROLL_STATE, WDT_EDT_SCROLL_STATE_STOP, COLOR_INFOBK);
}

void UI_APP_Nvod_Key(BU32 key)
{
}

void UI_APP_Nvod_EventList_Key(BU32 key)
{
	BU32 index;
	BU32 keyState;
	switch(key)
	{
	case GUI_VK_EXIT:
		APP_Com_IPI_StopProg(1);
		if(UI_APP_API_GetEpgSaveFlag() == 1)
		{
			GUI_ENC_API_SetValue(ID_STAInfoText_EpgInfo, WDT_STA_SV_DATAID, RS_SaveWait, 0);
			GUI_ENC_API_SwitchState(ID_DLG_EpgInfo, 1);
		}
		else
		{
			GUI_ENC_API_SwitchState(UI_APP_API_GetEpgPreState(), 1);
		}
		break;
	#if 0
	case GUI_VK_RECALL:
		if(UI_APP_API_GetEpgSaveFlag() == 1)
		{
			GUI_ENC_API_SetValue(ID_STAInfoText_EpgInfo, WDT_STA_SV_DATAID, RS_SaveWait, 0);
			GUI_ENC_API_SwitchState(ID_DLG_EpgInfo, 1);
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			if(APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}
		}
 		break;
	#endif
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		if(g_nvod_event_num)
		{
			GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, &index, 0);
			GUI_ENC_API_SetValue(ID_SBR_EventList_Nvod, WDT_SBR_SV_VALUE, index+g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE, 0);
			GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod,WDT_LBP_SV_KEYSTATE,&keyState,0);
			if(!keyState)//连续键处理
			{
				//FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
				NVOD_IPI_ShowEventDetail();
				Nvod_IPI_EventPlayTime_SetPageParam();
				Nvod_IPI_EventPlayTime_ShowCurPage();
				Nvod_IPI_PlayFirstProg();//播放节目
				//FYF_API_semaphore_release(semaphoreNvod);
			}
		}
		break;
	case GUI_VK_RIGHT:
		if(g_nvod_event_time_num)
		{
			GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_SCROLL_STATE, WDT_EDT_SCROLL_STATE_STOP, COLOR_INFOBK);
			GUI_ENC_API_SetFocus(ID_LBP_EventPlayTime_Nvod);
		}
		break;
	case GUI_VK_GREEN:
		toReserveListPreFocus = ID_LBP_EventList_Nvod;
		UI_APP_API_SetReserveListPreState(ID_DLG_Nvod);
		GUI_ENC_API_SwitchState(ID_DLG_ReserveList,1);
		break;
	default:
		break;
	}	
}

void UI_APP_Nvod_EventList_Change(void)
{
	BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_nvod_event_curPage++;
			break;
		case -1:
			if(g_nvod_event_curPage == 0)
			{
				g_nvod_event_curPage = g_nvod_event_page -1;
			}
			else
			{
				g_nvod_event_curPage--;
			}
			if(g_nvod_event_curPage == g_nvod_event_page-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, NVOD_EVENT_NUM_OF_PAGE-1, 0);
			}
			break;
		default:
			break;
	}
	//FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	Nvod_IPI_EventList_ShowCurPage();
	//FYF_API_semaphore_release(semaphoreNvod);
}

void UI_APP_Nvod_EventPlayTime_Key(BU32 key)
{
	BU32 index;
	BU32 keyState;
	BU32 ret;
	BU32 nvodEndTime;
	BS32 psYear = 0;
	BS08 pcMonth = 0;
	BS08 pcDate = 0;
	APP_Nvod_Program_List_S shiftTimeService;
	APP_Nvod_Prog_TimeInfo_S refEvent;
	Epg_Info_S conflictInfo;
	BU32 language;
	BU32 eventIndex;
	BU32 conflictIndex;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &language);
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		if(g_nvod_event_time_num)
		{
			GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, &index, 0);
			GUI_ENC_API_SetValue(ID_SBR_EventPlayTime_Nvod, WDT_SBR_SV_VALUE, index+g_nvod_event_time_curPage*NVOD_EVENT_NUM_OF_PAGE, 0);
			//FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
			GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod,WDT_LBP_SV_KEYSTATE,&keyState,0);
			if(!keyState)//连续键处理
			{
				Nvod_IPI_PlayProg();//播放节目
			}
			//FYF_API_semaphore_release(semaphoreNvod);
		}
		break;
	case GUI_VK_LEFT:
	case GUI_VK_EXIT:
		GUI_ENC_API_SetValue(ID_EDT_Nvod, WDT_EDT_SV_SCROLL_STATE, WDT_EDT_SCROLL_STATE_PLAY, 0);
		GUI_ENC_API_SetFocus(ID_LBP_EventList_Nvod);
		Nvod_IPI_PlayFirstProg();
		Nvod_IPI_EventPlayTime_SetPageParam();
		Nvod_IPI_EventPlayTime_ShowCurPage();
		break;
	#if 0
	case GUI_VK_RECALL:
		if(UI_APP_API_GetEpgSaveFlag() == 1)
		{
			GUI_ENC_API_SetValue(ID_STAInfoText_EpgInfo, WDT_STA_SV_DATAID, RS_SaveWait, 0);
			GUI_ENC_API_SwitchState(ID_DLG_EpgInfo, 1);
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			if(APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}
		}
 		break;
	#endif
	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, &index, 0);
		eventIndex = index+g_nvod_event_time_curPage*NVOD_EVENT_TIME_NUM_OF_PAGE;
		UI_APP_API_SetNvodEventIndex(eventIndex);
		//FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
		ret = APP_Nvod_GetRefEventNodeByIndex(eventIndex,&refEvent);
		if(APP_SUCCESS == ret)
		{
			nvodEndTime = refEvent.sDuringTime+(refEvent.uStartHour*60+refEvent.uStartMinute);
			nvodReserveInfo.serviceId = refEvent.uProgNvodSeviceID;
			nvodReserveInfo.eventId = refEvent.uProgNvodEventID;
			nvodReserveInfo.startHour = refEvent.uStartHour;
			nvodReserveInfo.startMin = refEvent.uStartMinute;
			nvodReserveInfo.endHour = (BU08)(nvodEndTime/60);
			nvodReserveInfo.endMin= (BU08)(nvodEndTime%60);
			//APP_TDT_TransCodeDateToNormalDate(refEvent.uEventDateCode, &psYear, &pcMonth, &pcDate);
			//nvodReserveInfo.day = (BU32)(psYear*10000 + pcMonth*100 + pcDate);
			nvodReserveInfo.day = refEvent.uEventDateCode;
			GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, &index, 0);
			if(APP_SUCCESS == APP_Nvod_GetShiftServiceNodeByIndex(index+g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE,&shiftTimeService))
			{
				GUI_API_Memcpy(nvodReserveInfo.eventName,shiftTimeService.acProgNvodName,NVOD_MAX_EVENT_NAME_LENGTH+1);
				nvodReserveInfo.eventName[APP_EPG_NAME_LEN-1] = 0;
			}
			else
			{
				GUI_API_Memset(nvodReserveInfo.eventName, 0, sizeof(nvodReserveInfo.eventName));
			}
			if(RUNNING_PLAY == refEvent.uEventRunStatus)
			{
				
				APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &index);
		        GUI_ENC_API_SetValue(ID_VOI_NvodPlay, WDT_VOI_SV_RIGHT, index, 0);
				GUI_ENC_API_SwitchState(ID_DLG_NvodPlay, 1);
			}
			else if(RUNNING_RESERVED == refEvent.uEventRunStatus)
			{
				UI_APP_API_SetEpgSaveFlag(1);
				g_epg_reserveType = RESERVE_CANCEL;
				APP_EPG_API_SetNvodReserveState(&nvodReserveInfo,0);
				APP_Nvod_SetRefEventRunStatusByIndex(eventIndex,RUNNING_NONE);
				Nvod_IPI_EventPlayTime_ShowCurPage();
			}
			else if(RUNNING_NONE == refEvent.uEventRunStatus)
			{
				UI_APP_API_SetEpgSaveFlag(1);
				if(APP_EPG_API_GetNvodConflictIndex(&conflictIndex,&nvodReserveInfo))//冲突
				{
					UI_APP_API_SetConflictIndex(conflictIndex);
					g_epg_reserveType = RESERVE_CONFLICT_NVOD;
					APP_EPG_API_GetReservedInfo(conflictIndex, 1, &conflictInfo);
					if(language == APP_SYS_LANGUAGE_CHINESE_e)
					{				
						sprintf(g_epg_reserveStr[0],"当前节目与已预约节目:");
						sprintf(g_epg_reserveStr[1],"频道: 点播节目");
						sprintf(g_epg_reserveStr[2],"节目: %s",conflictInfo.epgName);
					}
					else
					{
						sprintf(g_epg_reserveStr[0],"current Chn with reserved Chn:");
						sprintf(g_epg_reserveStr[1],"Chn: Nvod");
						sprintf(g_epg_reserveStr[2],"Epg: %s",conflictInfo.epgName);
					}
					g_epg_reserveStr[2][39]=0;
					GUI_ENC_API_SetValue(ID_STA_Line1_ConflictInfo,WDT_STA_SV_STRING, (BU32)g_epg_reserveStr[0],0);
					GUI_ENC_API_SetValue(ID_STA_Line2_ConflictInfo,WDT_STA_SV_STRING, (BU32)g_epg_reserveStr[1],0);
					GUI_ENC_API_SetValue(ID_STA_Line3_ConflictInfo,WDT_STA_SV_STRING, (BU32)g_epg_reserveStr[2],0);
					GUI_ENC_API_SetValue(ID_STA_Line4_ConflictInfo,WDT_STA_SV_DATAID, RS_ConflictLine1,0);
					GUI_ENC_API_SetValue(ID_STA_Line5_ConflictInfo,WDT_STA_SV_DATAID, RS_ConflictLine2,0);
					GUI_ENC_API_SwitchState(ID_DLG_ConflictInfo, 1);
				}
				else if(APP_EPG_API_GetReserveAmount() >=MAX_RESERVE_NUM)//预约列表满
				{
					g_epg_reserveType = RESERVE_FULL;
					GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo, WDT_STA_SV_DATAID, RS_ReserveFull, 0);
					GUI_ENC_API_SwitchState(ID_DLG_EpgReserveInfo, 1);
				}
				else
				{
					g_epg_reserveType = RESERVE_OK;
					APP_Nvod_SetRefEventRunStatusByIndex(eventIndex,RUNNING_RESERVED);
					APP_EPG_API_SetNvodReserveState(&nvodReserveInfo,1);
		 			Nvod_IPI_EventPlayTime_ShowCurPage();
				}
			}
		}
		//FYF_API_semaphore_release(semaphoreNvod);
		break;
	case GUI_VK_GREEN:
		toReserveListPreFocus = ID_LBP_EventPlayTime_Nvod;
		UI_APP_API_SetReserveListPreState(ID_DLG_Nvod);
		GUI_ENC_API_SwitchState(ID_DLG_ReserveList,1);
		break;
	default:
		break;
	}
}

void UI_APP_Nvod_EventPlayTime_Change(void)
{
	BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			if(g_nvod_event_time_curPage == g_nvod_event_time_page -1)
			{
				g_nvod_event_time_curPage = 0;
			}
			else
			{
				g_nvod_event_time_curPage++;
			}
			break;
		case -1:
			if(g_nvod_event_time_curPage == 0)
			{
				g_nvod_event_time_curPage = g_nvod_event_time_page -1;
			}
			else
			{
				g_nvod_event_time_curPage--;
			}
			
			if(g_nvod_event_time_curPage == g_nvod_event_time_page-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, NVOD_EVENT_TIME_NUM_OF_PAGE-1, 0);
			}
			break;
		default:
			break;
	}
//	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	Nvod_IPI_EventPlayTime_ShowCurPage();

//	FYF_API_semaphore_release(semaphoreNvod);
}


void UI_APP_Nvod_Timer(void)
{
	BU32 index,itemIndex;
	APP_Nvod_Program_List_S shiftTimeService;
	BU32 ret;
//	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	APP_Nvod_GetAllShiftServiceNumAndIndex();
	APP_Nvod_CheckShiftServiceStatus();
	if(APP_Nvod_GetShiftServiceUpdateFlag())
	{
		Nvod_IPI_EventList_SetPageParam();
		Nvod_IPI_EventList_ShowCurPage();
		NVOD_IPI_ShowEventDetail();
		APP_Nvod_SetShiftServiceUpdateFlag(0);
		
	}
	
	APP_Nvod_CheckRefEventStatus();
	GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, &index, 0);
	itemIndex = g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE+index;
	ret = APP_Nvod_GetShiftServiceNodeByIndex(itemIndex,&shiftTimeService);
	if(APP_SUCCESS == ret)
	{
		if(shiftTimeService.update == 1)
		{
			Nvod_IPI_EventPlayTime_SetPageParam();
			Nvod_IPI_EventPlayTime_ShowCurPage();
			Nvod_IPI_PlayProg();
			APP_Nvod_SetRefEventUpdateStatusByIndex(itemIndex,0);;
		}
	}
	
//	FYF_API_semaphore_release(semaphoreNvod);
}

//NvodPlay全屏视频

void NvodPlay_IPI_ClearScreen(void)
{
	GUI_ENC_API_SetVisible(ID_STA_FullTrans_NvodPlay,WS_VISIBLE); 	
	GUI_ENC_API_Update(ID_STA_FullTrans_NvodPlay,1);
}
void NvodPlay_IPI_SetShowInfo(BU08 ProgInfobShow,BU08 ProgDetailbShow)
{
	BU32 lang;
	BU32 index;
	BU32 value;
	BU32 duringHour;
	BU32 duringMin;
	BU32 startHour;
	BU32 startMin;
	BU32 curHour;
	BU32 curMin;
	BU32 curPlayProgress;
	APP_Date_S now;
	APP_Nvod_Prog_TimeInfo_S refEvent;
	APP_Nvod_Program_List_S shiftTimeService;
	BU32 len;
	if(ProgInfobShow)
	{
		GUI_API_Memset(duringTimeStr, 0, 20);
		GUI_API_Memset(playTimeStr, 0, 5);
		GUI_API_Memset(curTimeStr, 0, 5);
		GUI_API_Memset(curPlayProgressStr, 0, 5);

		GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, &index, 0);
		GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, &value, 0);
//		FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
		APP_Nvod_GetShiftServiceNodeByIndex(index+g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE,&shiftTimeService);
		APP_Nvod_GetRefEventNodeByIndex(value+g_nvod_event_time_curPage*NVOD_EVENT_TIME_NUM_OF_PAGE,&refEvent);
//		FYF_API_semaphore_release(semaphoreNvod);
		duringHour = refEvent.sDuringTime/60;
		duringMin = refEvent.sDuringTime%60;
		APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &lang);
		if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
		{
		 	sprintf(duringTimeStr,"%dHour%dMin",duringHour,duringMin);
		}
		else
		{
			sprintf(duringTimeStr,"%d小时%d分钟",duringHour,duringMin);
		}
		startHour = refEvent.uStartHour;
		startMin = refEvent.uStartMinute;
		sprintf(playTimeStr,"%d:%d",startHour,startMin);
		APP_SYS_API_GetLocalDateAndTime(&now);
		curHour = (BU32)now.m_hour;
		curMin = (BU32)now.m_min;
		sprintf(curTimeStr,"%d:%d",curHour,curMin);
		curPlayProgress = (BU32)((((curHour*60+curMin)-(startHour*60+startMin))*100)/(BU32)(refEvent.sDuringTime));
		sprintf(curPlayProgressStr,"%d%%",curPlayProgress);
		APP_Printf("curPlayProgress = %d,curPlayProgressStr = %s\n",curPlayProgress,curPlayProgressStr);
		GUI_ENC_API_SetValue(ID_STA_Line6_NvodPlay, WDT_STA_SV_STRING, shiftTimeService.acProgNvodName, 0);
		GUI_ENC_API_SetValue(ID_STA_Line7_NvodPlay, WDT_STA_SV_STRING, duringTimeStr, 0);
		GUI_ENC_API_SetValue(ID_STA_Line8_NvodPlay, WDT_STA_SV_STRING, playTimeStr, 0);
		GUI_ENC_API_SetValue(ID_STA_Line9_NvodPlay, WDT_STA_SV_STRING, curTimeStr, 0);
		GUI_ENC_API_SetValue(ID_PBR_NvodPlay,WDT_PBR_SV_CHANGE,(BU32)(curPlayProgress*2),0);//进度条的长度为200，所以要乘以2
		GUI_ENC_API_SetValue(ID_STA_PbrText_NvodPlay, WDT_STA_SV_STRING, curPlayProgressStr, 0);
	}
	if(ProgDetailbShow)
	{
		GUI_ENC_API_GetValue(ID_LBP_EventList_Nvod, WDT_LBP_SV_INDEX, &index, 0);
		APP_Nvod_GetShiftServiceNodeByIndex(index+g_nvod_event_curPage*NVOD_EVENT_NUM_OF_PAGE,&shiftTimeService);
		GUI_ENC_API_SetValue (ID_STA_EventDetail_MovieName_NvodPlay, WDT_STA_SV_STRING, shiftTimeService.acProgNvodName, 0);
		len = GUI_API_Strlen(shiftTimeService.acProgNvodText);
		if(len)
		{
			GUI_ENC_API_SetValue(ID_EDT_NvodPlay,WDT_EDT_SV_PAGE_INDEX,0,0);
			GUI_ENC_API_SetValue(ID_EDT_NvodPlay, WDT_EDT_SV_VALUE, (BU32)(shiftTimeService.acProgNvodText), len);
		}
	}
}
void NvodPlay_IPI_ShowConfig(BU08 InfobShow, BU08 NumbShow, BU08 VoicebShow,BU08 ProgInfobShow,BU08 ProgDetailbShow)
{
	BU16 curChn;
 	if(InfobShow)
 	{
 		GUI_ENC_API_SetVisible(ID_STA_TextBk_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Text_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Title_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_Update(ID_STA_TextBk_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Text_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Title_NvodPlay ,1);
 	}
 	else
 	{
 		GUI_ENC_API_SetVisible(ID_STA_TextBk_NvodPlay,WS_DISABLE); 
 		GUI_ENC_API_SetVisible(ID_STA_Text_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Title_NvodPlay,WS_DISABLE);	
 	}
	if(VoicebShow)
	{
		GUI_ENC_API_SetVisible(ID_VOI_NvodPlay,WS_VISIBLE);
		GUI_ENC_API_Update(ID_VOI_NvodPlay ,1);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_VOI_NvodPlay,WS_DISABLE);
	}
	
	if(NumbShow)
	{
		curChn = APP_ChnDatabase_API_GetCurChnNO();
		//GUI_ENC_API_SetValue(ID_CHN_NvodPlay, WDT_CHN_SV_NUM, (BU32)(curChn+APP_ChnDatabase_API_GetChn0Flag()), 0);
		GUI_ENC_API_SetVisible(ID_CHN_NvodPlay, WS_VISIBLE);
		GUI_ENC_API_Update(ID_CHN_NvodPlay ,1);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_CHN_NvodPlay, WS_DISABLE);
	}
	if(ProgInfobShow)
	{
 		GUI_ENC_API_SetVisible(ID_STA_ChnInfoBk_NvodPlay,WS_VISIBLE);	
		GUI_ENC_API_SetVisible(ID_STA_ChnInfoTitle_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line1_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line2_NvodPlay,WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Line3_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line4_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_PBR_NvodPlay,WS_VISIBLE);
 		GUI_ENC_API_SetVisible(ID_STA_PbrText_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line5_NvodPlay,WS_VISIBLE);
 		GUI_ENC_API_SetVisible(ID_STA_Line6_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line7_NvodPlay,WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Line8_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line9_NvodPlay,WS_VISIBLE);
 		GUI_ENC_API_Update(ID_STA_ChnInfoBk_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_ChnInfoTitle_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line1_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line2_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line3_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line4_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_PBR_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_PbrText_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line5_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line6_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line7_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line8_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_Line9_NvodPlay ,1);
	}
	else
	{
 		GUI_ENC_API_SetVisible(ID_STA_ChnInfoBk_NvodPlay,WS_DISABLE);	
		GUI_ENC_API_SetVisible(ID_STA_ChnInfoTitle_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line1_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line2_NvodPlay,WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Line3_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line4_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_PBR_NvodPlay,WS_DISABLE);
 		GUI_ENC_API_SetVisible(ID_STA_PbrText_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line5_NvodPlay,WS_DISABLE);
 		GUI_ENC_API_SetVisible(ID_STA_Line6_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line7_NvodPlay,WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Line8_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Line9_NvodPlay,WS_DISABLE);	
	}
	if(ProgDetailbShow)
	{	
 		GUI_ENC_API_SetVisible(ID_STA_EventDetailBk_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_EventDetai_Title_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_EventDetail_MovieName_NvodPlay,WS_VISIBLE);
 		GUI_ENC_API_SetVisible(ID_EDT_NvodPlay,WS_VISIBLE);	
 		GUI_ENC_API_Update(ID_STA_EventDetailBk_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_EventDetai_Title_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_STA_EventDetail_MovieName_NvodPlay ,1);
 		GUI_ENC_API_Update(ID_EDT_NvodPlay ,1);
 	}
 	else
 	{
 		GUI_ENC_API_SetVisible(ID_STA_EventDetailBk_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_EventDetai_Title_NvodPlay,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_EventDetail_MovieName_NvodPlay,WS_DISABLE);
 		GUI_ENC_API_SetVisible(ID_EDT_NvodPlay,WS_DISABLE);	
	}
}

void NvodPlay_IPI_SetCurState(BU32 curState)
{
	switch(curState)
	{
	case NVOD_FULLPLAY_NULL_STATE:
		NvodPlay_IPI_ClearScreen();
		NvodPlay_IPI_ShowConfig(0, 0, 0, 0, 0);
		break;
	case NVOD_FULLPLAY_VOICE_STATE:
		NvodPlay_IPI_ShowConfig(0, 0, 1, 0, 0);
		break;
	case NVOD_FULLPLAY_INFO_STATE:
		NvodPlay_IPI_ShowConfig(1, 0, 0, 0, 0);
		break;
	case NVOD_FULLPLAY_CHNUM_STATE:
		NvodPlay_IPI_ShowConfig(0, 1, 0, 0, 0);
		break;
	case NVOD_FULLPLAY_PROG_INFO_STATE:
		NvodPlay_IPI_ShowConfig(0, 0, 0, 1, 0);
		break;
	case NVOD_FULLPLAY_PROG_DETAIL_STATE:
		NvodPlay_IPI_ShowConfig(0, 0, 0, 0, 1);
		break;
	default:
		break;
	}
	g_nvod_play_curState = curState;
}

//快进，播放上一个时间的节目
void NvodPlay_IPI_Prev(void)
{
	BU32 itemIndex;
	BU32 ret;
	APP_Nvod_Prog_TimeInfo_S refEvent;
	FYF_time_shifted_service_s shiftServiceInfo;
	GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, &event_time_index, 0);
	itemIndex = event_time_index+g_nvod_event_time_curPage*NVOD_EVENT_TIME_NUM_OF_PAGE;
	if(itemIndex == 0)
	{
		itemIndex = g_nvod_event_time_num - 1;
	}
	else
	{
		itemIndex--;
	}
//	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	ret = APP_Nvod_GetRefEventNodeByIndex(itemIndex,&refEvent);
//	FYF_API_semaphore_release(semaphoreNvod);
	if(APP_FAIL != ret && RUNNING_PLAY == refEvent.uEventRunStatus)
	{
		startTime = refEvent.uStartHour*3600+refEvent.uStartMinute*60+refEvent.uStartSecond;
		duringTime = refEvent.sDuringTime*60+refEvent.uDuringSecond;
		dateTime = refEvent.uEventDateCode;

		
		UI_APP_API_PlayByServiceID(refEvent.uProgNvodSeviceID, "nuod");
		//APP_Com_API_PlayNvodChn(refEvent.uProgNvodSeviceID);
		//FYF_API_time_get_pid(refEvent.uProgNvodSeviceID,&shiftServiceInfo);
		//APP_IPI_PlayProg(shiftServiceInfo.vpid, shiftServiceInfo.apid,shiftServiceInfo.pcrpid,FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
		if(event_time_index == 0)
		{
			event_time_index = NVOD_EVENT_TIME_NUM_OF_PAGE - 1;
		}
		else
		{
			event_time_index--;
		}
		progInfobShow = 1;
		GUI_ENC_API_SetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, event_time_index, 0);
		GUI_ENC_API_SetValue(ID_STA_Text_NvodPlay, WDT_STA_SV_DATAID, RS_NvodProgSpeed, 0);
	}
	else
	{
		//提示没有上一个播放节目
		progInfobShow = 0;
		GUI_ENC_API_SetValue(ID_STA_Text_NvodPlay, WDT_STA_SV_DATAID, RS_NotNvodProgSpeed, 0);
	}
	if(g_nvod_play_curState != NVOD_FULLPLAY_INFO_STATE)
	{
		if(g_nvod_play_curState != NVOD_FULLPLAY_NULL_STATE)
		{
			NvodPlay_IPI_ClearScreen();
		}
		NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_INFO_STATE);
		GUI_ENC_API_SetValue(ID_TIM_NvodPlay, WDT_TIM_SV_REPLAY, 0, 0);
	}
}
//快退，播放下一个时间的节目
void NvodPlay_IPI_Next(void)
{
	BU32 itemIndex;
	BU32 ret;
	APP_Nvod_Prog_TimeInfo_S refEvent;
	FYF_time_shifted_service_s shiftServiceInfo;
	GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, &event_time_index, 0);
	APP_Printf("event_time_index = %d\n",event_time_index);
	itemIndex = event_time_index+g_nvod_event_time_curPage*NVOD_EVENT_TIME_NUM_OF_PAGE;
	if(itemIndex == g_nvod_event_time_num - 1)
	{
		itemIndex = 0;
	}
	else
	{
		itemIndex++;
	}
//	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	ret = APP_Nvod_GetRefEventNodeByIndex(itemIndex,&refEvent);
//	FYF_API_semaphore_release(semaphoreNvod);
	if(APP_FAIL != ret && RUNNING_PLAY == refEvent.uEventRunStatus)
	{
		startTime = refEvent.uStartHour*3600+refEvent.uStartMinute*60+refEvent.uStartSecond;
		duringTime = refEvent.sDuringTime*60+refEvent.uDuringSecond;
		dateTime = refEvent.uEventDateCode;

		
		UI_APP_API_PlayByServiceID(refEvent.uProgNvodSeviceID, "nuod");
		//APP_Com_API_PlayNvodChn(refEvent.uProgNvodSeviceID);
		//FYF_API_time_get_pid(refEvent.uProgNvodSeviceID,&shiftServiceInfo);
		//APP_IPI_PlayProg(shiftServiceInfo.vpid, shiftServiceInfo.apid,shiftServiceInfo.pcrpid,FYF_VCODEC_TYPE_MPEG2, FYF_ACODEC_TYPE_MP3);
		if(event_time_index == NVOD_EVENT_TIME_NUM_OF_PAGE - 1)
		{
			event_time_index = 0;
		}
		else
		{
			event_time_index++;
		}
		progInfobShow = 1;
		GUI_ENC_API_SetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, event_time_index, 0);
		GUI_ENC_API_SetValue(ID_STA_Text_NvodPlay, WDT_STA_SV_DATAID, RS_NvodProgBack, 0);
	}
	else
	{
		//提示没有下一个播放节目
		progInfobShow = 0;
		GUI_ENC_API_SetValue(ID_STA_Text_NvodPlay, WDT_STA_SV_DATAID, RS_NotNvodProgBack, 0);
	}
	if(g_nvod_play_curState != NVOD_FULLPLAY_INFO_STATE)
	{
		if(g_nvod_play_curState != NVOD_FULLPLAY_NULL_STATE)
		{
			NvodPlay_IPI_ClearScreen();
		}
		NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_INFO_STATE);
		GUI_ENC_API_SetValue(ID_TIM_NvodPlay, WDT_TIM_SV_REPLAY, 0, 0);
	}
}
void NvodPlay_IPI_ShowMovieTimeLeng(void)
{
	BU32 index;
	BU32 ret;
	APP_Nvod_Prog_TimeInfo_S timeInfo;
	BU32 systemCodeDate;
	BU32 hour = 0;
	BU32 minute = 0;
	BU32 sec = 0;
	BU32 interval;
	APP_Date_S now;
	BU32 hourTmp = 0;
	BU32 minTmp = 0;
	BU32 secTmp = 0;
	memset(timeStr,0,sizeof(timeStr));
	APP_SYS_API_GetLocalDateAndTime(&now);
	hour = (BU32)now.m_hour;
	minute = (BU32)now.m_min;
	sec = (BU32)now.m_sec;
	systemCodeDate = APP_IPI_GetDay();//APP_TDT_IPI_GetSystemCodedate();
	if(systemCodeDate == dateTime)
	{
		interval = (duringTime) - ((hour*3600+minute*60+sec) - startTime);
		if(interval == 0)
		{
			GUI_ENC_API_SwitchState(ID_DLG_Nvod, 1);
		}
		else
		{
			hourTmp =  interval/3600;
			if(hourTmp >= 24 || hourTmp < 0)
			{
				hourTmp = 0;
			}
			minTmp =  interval%3600/60;
			if(minTmp >= 60 || minTmp < 0)
			{
				minTmp = 0;
			}
			secTmp = interval%3600%60;
			if(secTmp >= 60 || secTmp < 0)
			{
				secTmp = 0;
			}		
			//printf("cur hour:%02d,min:%02d,sec:%02d\n",hour,minute,sec);
			sprintf(timeStr, "%02d:%02d:%02d",hourTmp,minTmp,secTmp);
			//printf("time-- %s\n",timeStr);
			GUI_ENC_API_SetValue(ID_STA_Time_NvodPlay, WDT_STA_SV_STRING, timeStr, 0);
		}
	}
}

void NvodPlay_IPI_JudgeProgExist(void)
{
	BU32 number=0;
	BU08 bExist = 0;
	BU32 lang;
	
	GUI_ENC_API_GetValue(ID_CHN_NvodPlay, WDT_CHN_SV_NUM, (BU32 *)&number, 0);
	if(APP_ChnDatabase_API_GetChn0Flag() == 0)
	{
		if(number <= APP_ChnDatabase_API_GetCurTypeChnNum()-1)
		{	
			bExist = 1;
		}
		else
		{
			bExist = 0;
		}
	}
	else
	{
		if(number >= 1 && number <= APP_ChnDatabase_API_GetCurTypeChnNum())
		{	
			bExist = 1;
		}
		else
		{
			bExist = 0;
		}
	}
	
	if(bExist)
	{
		APP_ChnDatabase_API_SetCurChnNO(number- APP_ChnDatabase_API_GetChn0Flag());
		APP_Com_IPI_StopProg(1);
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	}
	else
	{
		APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &lang);
 		if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
 		{
			sprintf(nvod_strTemp, "%s%03d%s", DicDataEnglish[RS_Program],number, DicDataEnglish[RS_NotExist]);
 		}
 		else
 		{
 			sprintf(nvod_strTemp, "%s%03d%s", DicData[RS_Program],number, DicData[RS_NotExist]);
 		}
 		GUI_ENC_API_SetValue(ID_STA_Text_NvodPlay, WDT_STA_SV_STRING, (BU32)nvod_strTemp, 0);
		NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_INFO_STATE);
	}
}

void UI_APP_NvodPlay_Enter(void)
{
	BU32 index;
	BU32 ret;
	APP_Nvod_Prog_TimeInfo_S timeInfo;
	nvodPlayFlag = 1;
	if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
	{
		GUI_ENC_API_SetValue(ID_STAMute_NvodPlay, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
		APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
	}
	else
	{
		GUI_ENC_API_Update(ID_STAMuteBk_NvodPlay,1);
		GUI_ENC_API_SetValue(ID_STAMute_NvodPlay, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
		APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
		UI_APP_API_SetVolume();
	}
	#if 0
 	APP_SYS_API_GetSysInfo(APP_SYS_TIMEOUT_e, &g_nvod_play_timeout);
	if(g_nvod_play_timeout != 0)
	{
		GUI_ENC_API_SetValue(ID_TIM_NvodPlay, WDT_TIM_SV_PERIOD, g_nvod_play_timeout*1000,0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_TIM_NvodPlay, WDT_TIM_SV_PERIOD, NOVD_PLAY_TIMER_PERIOD,0);
	}
	#endif
	NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_NULL_STATE);
	APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_JudgeChn0();	
	APP_Com_API_SetFullScreen();
	GUI_ENC_API_GetValue(ID_LBP_EventPlayTime_Nvod, WDT_LBP_SV_INDEX, &index, 0);
//	FYF_API_semaphore_wait(semaphoreNvod, FYF_WAIT_FOREVER);
	ret = APP_Nvod_GetRefEventNodeByIndex(index+g_nvod_event_time_curPage*NVOD_EVENT_TIME_NUM_OF_PAGE,&timeInfo);
//	FYF_API_semaphore_release(semaphoreNvod);
	if(APP_FAIL != ret && RUNNING_PLAY == timeInfo.uEventRunStatus)
	{
		startTime = timeInfo.uStartHour*3600+timeInfo.uStartMinute*60+timeInfo.uStartSecond;
		duringTime = timeInfo.sDuringTime*60+timeInfo.uDuringSecond;
		dateTime = timeInfo.uEventDateCode;
	}
	NvodPlay_IPI_ShowMovieTimeLeng();

}

void UI_APP_NvodPlay_End(void)
{
	nvodPlayFlag = 0;
}

void UI_APP_NvodPlay_Key(BU32 key)
{
	BU32 index;
	if(g_nvod_play_curState != NVOD_FULLPLAY_PROG_DETAIL_STATE)
	{
		switch(key)
		{
		case GUI_VK_MENU:
		case GUI_VK_RECALL:
		case GUI_VK_EXIT:
			GUI_ENC_API_SwitchState(ID_DLG_NvodPlayInfo, 1);
			break;
		case GUI_VK_UP:
			//快进
			/*
			判断是否有上一个播放状态的节目
			是:先提示1秒"节目快进"，再播放上一个节目并显示2秒节目信息如下
				影片名称:****
				影片长度:**分钟
				播放时间:**:**-**:**
				播放进度:PBR进度条
				当前时间:**:**
			否:提示1秒"不能节目快进"
			*/
			NvodPlay_IPI_Prev();
			break;
		case GUI_VK_DOWN:
			//快退
			/*
			判断是否有下一个播放状态的节目
			是:先提示1秒"节目快退"，再播放上一个节目并显示2秒节目信息如下
				影片名称:****
				影片长度:**分钟
				播放时间:**:**-**:**
				播放进度:PBR进度条
				当前时间:**:**
			否:提示1秒"不能节目快退"
			*/
			NvodPlay_IPI_Next();
			break;
		case GUI_VK_V_DOWN:
		case GUI_VK_V_UP:		
		case GUI_VK_LEFT:
		case GUI_VK_RIGHT:
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_Update(ID_STAMuteBk_NvodPlay, 1);
				GUI_ENC_API_SetValue(ID_STAMute_NvodPlay, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}
			if(g_nvod_play_curState != NVOD_FULLPLAY_VOICE_STATE)
			{
				if(g_nvod_play_curState != NVOD_FULLPLAY_NULL_STATE)
				{
					NvodPlay_IPI_ClearScreen();
				}
				NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_VOICE_STATE);
				GUI_ENC_API_SetValue(ID_TIM_NvodPlay, WDT_TIM_SV_REPLAY, 0, 0);
			}
			GUI_ENC_API_SetValueUnPaint(ID_VOI_NvodPlay, WDT_VOI_SV_KEY, key, 0);
			GUI_ENC_API_GetValue(ID_VOI_NvodPlay, WDT_VOI_SV_INDEX, &index, 0);
			GUI_ENC_API_Update(ID_VOI_NvodPlay, 1);
			APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
			UI_APP_API_SetVolume();
			break;
		case GUI_VK_MUTE:
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_Update(ID_STAMuteBk_NvodPlay,1);
				GUI_ENC_API_SetValue(ID_STAMute_NvodPlay, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
				UI_APP_API_SetVolume();
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STAMute_NvodPlay, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
			break;
		case GUI_VK_STATE:
			//信息显示
			break;
		case GUI_VK_INFO:
			//内容简介
			if(g_nvod_play_curState != NVOD_FULLPLAY_PROG_DETAIL_STATE)
			{
				if(g_nvod_play_curState != NVOD_FULLPLAY_NULL_STATE)
				{
					NvodPlay_IPI_ClearScreen();
				}
				NvodPlay_IPI_SetShowInfo(0,1);
				NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_PROG_DETAIL_STATE);
				GUI_ENC_API_SetValue(ID_TIM_NvodPlay, WDT_TIM_SV_REPLAY, 0, 0);
			}
			break;
//暂时不做从nvod切换到电视中
#if 0
		case GUI_VK_0:
		case GUI_VK_1:
		case GUI_VK_2:
		case GUI_VK_3:
		case GUI_VK_4:
		case GUI_VK_5:
		case GUI_VK_6:
		case GUI_VK_7:
		case GUI_VK_8:
		case GUI_VK_9:
			//切换到正常节目播放
			if(GUI_ENC_API_GetKeyState() == 0)
			{
				if(nvod_lastKeyState == 0)
				{
					GUI_ENC_API_SetValue(ID_TIM_NvodPlay, WDT_TIM_SV_REPLAY, 0, 0);
					if(g_nvod_play_curState != NVOD_FULLPLAY_CHNUM_STATE)
					{
						GUI_ENC_API_SetValue(ID_CHN_NvodPlay, WDT_CHN_SV_CLR, 0, 0);
						if(g_nvod_play_curState != NVOD_FULLPLAY_NULL_STATE)
						{
							NvodPlay_IPI_ClearScreen();
						}
						NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_CHNUM_STATE);
						nvod_numKeyTime = 0;
					}
					GUI_ENC_API_SetValue(ID_CHN_NvodPlay, WDT_CHN_SV_KEY, key, 0);
					nvod_numKeyTime++;
					if(nvod_numKeyTime == 3)
					{
						GUI_ENC_API_SetValue(ID_CHN_NvodPlay, WDT_CHN_SV_CLR, 0, 0);
						nvod_numKeyTime = 0;
					}
				}
				else
				{
					nvod_numKeyTime = 0;
				}
			}
			nvod_lastKeyState = GUI_ENC_API_GetKeyState();
			break;
#endif
		case GUI_VK_POWER:
			//待机
			break;
		default:
			break;
		}
	}
	else
	{
		NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_NULL_STATE);
		GUI_ENC_API_SetValue(ID_TIM_NvodPlay,WDT_TIM_SV_PERIOD,NOVD_PLAY_TIMER_PERIOD,0);
	}
}

void UI_APP_NvodPlay_Timer(void)
{
	switch(g_nvod_play_curState)
	{
	case NVOD_FULLPLAY_NO_SERVICE:
	case NVOD_FULLPLAY_VOICE_STATE:
	case NVOD_FULLPLAY_PROG_INFO_STATE:
		NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_NULL_STATE);
		GUI_ENC_API_SetValue(ID_TIM_NvodPlay,WDT_TIM_SV_PERIOD,NOVD_PLAY_TIMER_PERIOD,0);
		break;
	case NVOD_FULLPLAY_INFO_STATE:
		if(progInfobShow)
		{
			progInfobShow = 0;
			if(g_nvod_play_curState != NVOD_FULLPLAY_PROG_INFO_STATE)
			{
				if(g_nvod_play_curState != NVOD_FULLPLAY_NULL_STATE)
				{
					NvodPlay_IPI_ClearScreen();
				}
				NvodPlay_IPI_SetShowInfo(1,0);
				NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_PROG_INFO_STATE);
				GUI_ENC_API_SetValue(ID_TIM_NvodPlay, WDT_TIM_SV_REPLAY, 0, 0);
			}
		}
		else
		{
			NvodPlay_IPI_SetCurState(NVOD_FULLPLAY_NULL_STATE);
			GUI_ENC_API_SetValue(ID_TIM_NvodPlay,WDT_TIM_SV_PERIOD,NOVD_PLAY_TIMER_PERIOD,0);
		}
		break;
	case NVOD_FULLPLAY_CHNUM_STATE:
		NvodPlay_IPI_JudgeProgExist();
		break;
	default:
		break;
	}
	NvodPlay_IPI_ShowMovieTimeLeng();
}

//NvodPlay全屏播放退出提示框

void UI_APP_NvodPlayInfo_Enter(void)
{
	nvodPlayFlag = 1;
}

void UI_APP_NvodPlayInfo_End(void)
{
	nvodPlayFlag = 0;
}

void UI_APP_NvodPlayInfo_Key(BU32 key)
{
	BU32 index;
	
	switch(key)
	{
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_NvodPlay, 1);
		break;
	case GUI_VK_SELECT:
		GUI_ENC_API_SwitchState(ID_DLG_Nvod, 1);
		break;
	default:
		break;
	}
}

//进入Nvod时等待界面
void UI_APP_NvodWait_Enter(void)
{
	APP_Printf("UI_APP_NvodWait_Enter\n");
	BU32 hwnd;
	GUI_Rect_s rect;
	timerNum = 0;
	progress = 0;
	APP_Com_API_Stop_Signal_Dectect();
	GUI_ENC_API_SetValue(ID_PBR_NvodWait,WDT_PBR_SV_CHANGE,(BU32)(progress*3),0);//进度条的长度为300，所以要乘以3
	APP_Com_API_StopProg(1);
	APP_Com_API_SetFullScreen();
}

void UI_APP_NvodWait_End(void)
{
	APP_Com_API_Start_Signal_Dectect();
}
void UI_APP_NvodWait_Key(BU32 key)
{
	APP_Printf("UI_APP_NvodWait_Key--key = %d\n",key);
	switch(key)
	{
		case GUI_VK_EXIT:
			GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);
			if (GUI_ENC_API_GetPrevState() == ID_DLG_MainMenu)
			{
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}
			break;
		case GUI_VK_RECALL:
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			break;
		default:
			break;
	}
}
void UI_APP_NvodWait_Timer(void)
{
	timerNum++;
	progress += 10;
	APP_Printf("UI_APP_NvodWait_Timer-- timerNum = %d\n",timerNum);
	GUI_ENC_API_SetValue(ID_PBR_NvodWait,WDT_PBR_SV_CHANGE,(BU32)(progress*3),0);//进度条的长度为300，所以要乘以3
	if(timerNum == 10)
	{
		timerNum = 0;
		progress = 0;
		GUI_ENC_API_SwitchState(ID_DLG_Nvod,1);
	}
}
