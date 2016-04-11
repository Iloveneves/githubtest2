#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define WORKTIME_X 			10
#define WORKTIME_W 			 10

#define PAGE_CHN_NUM  8
#define CHN_NAME_LEN 30

static BU08 g_manualrec_startHour=0;
static BU08 g_manualrec_startMin=0;
static BU08 g_manualrec_endHour=23;
static BU08 g_manualrec_endMin=59;
static BU08 g_manualrec_offset=0;
static BU32 g_manualrec_conflictIndex = 0xff;

static BU32 g_manualrec_curPage = 0;
static BU32 g_manualrec_pageNum = 0;
static BU32 g_manualrec_rest= 0;
static BU08 g_manualrec_chnName[PAGE_CHN_NUM][CHN_NAME_LEN];
static CHN_LIST_S g_manualrec_chnlist_s[PAGE_CHN_NUM] = 
{
	{g_manualrec_chnName[0],0,0},
	{g_manualrec_chnName[1],0,0},
	{g_manualrec_chnName[2],0,0},
	{g_manualrec_chnName[3],0,0},
	{g_manualrec_chnName[4],0,0},
	{g_manualrec_chnName[5],0,0},
	{g_manualrec_chnName[6],0,0},
	{g_manualrec_chnName[7],0,0},
};

static BU08 g_manualrec_week[7][40];
static BU08* g_manualrec_week_s[7] = 
{
	g_manualrec_week[0],
	g_manualrec_week[1],
	g_manualrec_week[2],
	g_manualrec_week[3],
	g_manualrec_week[4],
	g_manualrec_week[5],
	g_manualrec_week[6]
};

static BU32 ManualRec_IPI_GetConflictRec(BU32 *conflictIndex,APP_ManualRecData_S* RecData)
{
	BU32 reservedNum,ret=0;
	BU32 recMin,reserveMin;
	BU32 i=0,index =0;
	Epg_Info_S reserveInfo[MAX_RESERVE_NUM];
	Epg_Info_S EpgInfo;
	APP_ManualRecData_S recData;
	*conflictIndex = 0xff;

	memcpy(&recData,RecData,sizeof(APP_ManualRecData_S));
	reservedNum = APP_EPG_API_GetReserveAmount();
	
	if(reservedNum == 0) return 0;
	
	APP_EPG_API_GetReservedInfo(0, reservedNum, reserveInfo);
	for(i=0;i<reservedNum;i++)
	{
		if(reserveInfo[i].week == recData.week)
		{
			reserveMin = reserveInfo[i].startHour*60 + reserveInfo[i].startMin;
			recMin   = recData.startHour*60 + recData.startMin;
			if(reserveMin >= recMin -1 && reserveMin <= recMin +1)
			{
				*conflictIndex  = i;
				ret =1;
				break;
			}
		}
	}
	return ret;
}

void ManualRec_IPI_SetPageParam(void)
{
	BU16 chnNum,curChn;
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	g_manualrec_pageNum = (chnNum +  PAGE_CHN_NUM -1) / PAGE_CHN_NUM;	
	g_manualrec_rest = chnNum % PAGE_CHN_NUM;
	g_manualrec_curPage = curChn / PAGE_CHN_NUM;
	if(g_manualrec_rest == 0)
	{
		g_manualrec_rest = PAGE_CHN_NUM;
	}

	if(chnNum == 0)
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_Prog_ManualRec, WDT_SBR_SV_MAX, (BU32)chnNum, 0);}
	else
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_Prog_ManualRec, WDT_SBR_SV_MAX, (BU32)chnNum-1, 0);}
	GUI_ENC_API_SetValueUnPaint(ID_SBR_Prog_ManualRec, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_Prog_ManualRec, WDT_SBR_SV_VALUE, (BU32)curChn, 0);
	
}
void ManualRec_IPI_ShowCurPage(void)
{
	ChnDataBase_ChnInfo_S ChnInfo;
	BU16 ChnNo,i;
	BU16 pageNum;
	if(g_manualrec_pageNum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_Prog_ManualRec, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(g_manualrec_curPage >= g_manualrec_pageNum)
	{
		g_manualrec_curPage = 0;
	}
	
	if(g_manualrec_curPage+1 == g_manualrec_pageNum)
	{	
		pageNum = g_manualrec_rest;
	}
	else
	{
		pageNum = PAGE_CHN_NUM;
	}
	
	for(i=0; i<pageNum; i++)
	{	
		ChnNo = i+g_manualrec_curPage*PAGE_CHN_NUM;
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ChnNo,&ChnInfo))
		{
			GUI_ENC_API_SetValue(ID_LBP_Prog_ManualRec, WDT_LBP_SV_DATA, (BU32)g_manualrec_chnlist_s, i);
			return;
		}
		ChnInfo.m_ChnName[CHN_NAME_LEN-7]='\0';
		if(ChnInfo.m_Ca)
		{
			sprintf(g_manualrec_chnlist_s[i].chnName,"%03d  %-12s $",ChnNo + APP_ChnDatabase_API_GetChn0Flag(),ChnInfo.m_ChnName);
		}
		else
		{
			sprintf(g_manualrec_chnlist_s[i].chnName,"%03d  %-12s",ChnNo + APP_ChnDatabase_API_GetChn0Flag(),ChnInfo.m_ChnName);
		}
		g_manualrec_chnlist_s[i].fav= ChnInfo.m_Fav;
		g_manualrec_chnlist_s[i].lock= ChnInfo.m_Lock;
	}
	GUI_ENC_API_SetValue(ID_LBP_Prog_ManualRec, WDT_LBP_SV_DATA, (BU32)g_manualrec_chnlist_s, pageNum);
}

static void ManualRec_IPI_ShowWeek(void)
{
	BU32 CodeDate;
	BU16 year;
	BU08 weekday,month,day;
	APP_Date_S now;
	BU32 i = 0;
	APP_SYS_API_GetLocalDateAndTime(&now);
  	g_manualrec_startHour = now.m_hour;
 	g_manualrec_startMin = now.m_min + 2;
	if(g_manualrec_startMin > 59)
	{
		if(g_manualrec_startHour == 23)
		{
			g_manualrec_startHour = 0;
		}
		else
		{
			g_manualrec_startHour++;
		}
		g_manualrec_startMin -= 60;
	}
  	g_manualrec_endHour = now.m_hour;
  	g_manualrec_endMin = now.m_min + 7;
	if(g_manualrec_endMin > 59)
	{
		if(g_manualrec_endMin == 23)
		{
			g_manualrec_endMin = 0;
		}
		else
		{
			g_manualrec_endMin++;
		}
		g_manualrec_endMin -= 60;
	}
	GUI_ENC_API_SetValue(ID_LBP_Week_ManualRec, WDT_LBP_SV_INDEX, 0, 0);
	for(i = 0;i < 7;i++)
	{
		APP_Com_API_NormalDateToCodeDate(now.m_year,now.m_month,now.m_day,&CodeDate);
		CodeDate = CodeDate + (BU32)(i);
		APP_Com_API_CodeDateToNormalDate(CodeDate,&year,&month,&day,&weekday);
		sprintf(g_manualrec_week_s[i],"星期%d -- %d/%d/%d",weekday,year,month,day);
	}
	GUI_ENC_API_SetValue(ID_LBP_Week_ManualRec, WDT_LBP_SV_DATA, (BU32)g_manualrec_week_s, 7);
}

void UI_APP_ManualRec_Enter(void)
{
	BU16 chnNum;
	APP_CHN_AV_TYPE_E AvType = APP_CHN_AV_TYPE_TV_e;
	APP_CHN_GROUP_TYPE_E GroupType = APP_CHN_GROUP_TYPE_ALL_e;
	APP_ChnDatabase_API_SetCurChnType(AvType, GroupType);
	APP_ChnDatabase_API_JudgeChn0();
	APP_Com_API_SetFullScreen();
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();

	if(chnNum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_Prog_ManualRec, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValue(ID_LBP_Week_ManualRec, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_Prog_ManualRec, WDT_SBR_SV_MAX, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_Prog_ManualRec, WDT_SBR_SV_MIN, 0, 0); 
		GUI_ENC_API_SetValue(ID_SBR_Prog_ManualRec, WDT_SBR_SV_VALUE, 0, 0);
	}
	else
	{
		GUI_ENC_API_SetValueUnPaint(ID_LBP_Prog_ManualRec, WDT_LBP_SV_INDEX, (BU32)(APP_ChnDatabase_API_GetCurChnNO()%PAGE_CHN_NUM), 0);
		ManualRec_IPI_SetPageParam();
		ManualRec_IPI_ShowCurPage();
		ManualRec_IPI_ShowWeek();
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);//播放节目
	}
}
void UI_APP_ManualRec_End(void)
{
}
void UI_APP_ManualRec_StartTime_OnPaint(BU32 hdc,BU32 lParam)
{
	BU08 showStr[6]={0};
	sprintf(showStr,"%02d:%02d",g_manualrec_startHour,g_manualrec_startMin);
	if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_ManualRec))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneOrangeBar_W200_H36);	
		if(g_manualrec_offset>=0&&g_manualrec_offset<2)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+g_manualrec_offset*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);			
		}
		else if(g_manualrec_offset>=2&&g_manualrec_offset<4)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+(g_manualrec_offset+1)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
		}
		else if(g_manualrec_offset>=4&&g_manualrec_offset<6)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+(g_manualrec_offset+2)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
		}
		GUI_API_TextOut(hdc, WORKTIME_X, 9, showStr, 8, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneBlueBar_W200_H36);
		GUI_API_DrawHLine(hdc, WORKTIME_X, 30, WORKTIME_W, 1);			
		GUI_API_TextOut(hdc, WORKTIME_X, 9, showStr, 8, COLOR_WHITE, COLOR_BLACK, 1);
	}
}

void UI_APP_ManualRec_EndTime_OnPaint(BU32 hdc,BU32 lParam)
{
	BU08 showStr[6]={0};
	sprintf(showStr,"%02d:%02d",g_manualrec_endHour,g_manualrec_endMin);
	if(GUI_ENC_API_IsFocus(ID_ODC_EndTime_ManualRec))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneOrangeBar_W200_H36);	
		if(g_manualrec_offset>=0&&g_manualrec_offset<2)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+g_manualrec_offset*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);			
		}
		else if(g_manualrec_offset>=2&&g_manualrec_offset<4)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+(g_manualrec_offset+1)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
		}
		else if(g_manualrec_offset>=4&&g_manualrec_offset<6)
		{
			GUI_API_DrawHLine(hdc, WORKTIME_X+(g_manualrec_offset+2)*WORKTIME_W, 30, WORKTIME_W, COLOR_BLACK);
		}
		GUI_API_TextOut(hdc, WORKTIME_X, 9, showStr, 8, COLOR_BLACK, COLOR_BLACK, 1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TimeZoneBlueBar_W200_H36);
		GUI_API_DrawHLine(hdc, WORKTIME_X, 30, WORKTIME_W, 1);			
		GUI_API_TextOut(hdc, WORKTIME_X, 9, showStr, 8, COLOR_WHITE, COLOR_BLACK, 1);
	}
}

void UI_APP_ManualRec_Key(BU32 key)
{
	BU32 index = 0;
	BU16 curChn = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	UserData_S UserData;
	BU32 keyState;
	APP_ManualRecData_S RecData;
	APP_Date_S now;
	BU32 CodeDate;
	BU16 year;
	BU08 weekday,month,day;
	Epg_Info_S conflictInfo;
	BU32 language;
	BU32 nowMin,recStartMin;
	switch(key)
	{
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
		if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_ManualRec))
		{
			switch(g_manualrec_offset)
			{
			case 0:
				g_manualrec_startHour = (BU08)((BU16)g_manualrec_startHour%10 + key*10);
				break;
			case 1:
				g_manualrec_startHour = (BU08)((BU16)g_manualrec_startHour-g_manualrec_startHour%10 + key);
				break;
			case 2:
				g_manualrec_startMin = (BU08)((BU16)g_manualrec_startMin%10 + key*10);
				break;
			case 3:
				g_manualrec_startMin = (BU08)((BU16)g_manualrec_startMin-g_manualrec_startMin%10 + key);
				break;
			default:
				break;
			}
			if(g_manualrec_startHour>=24)	g_manualrec_startHour = 0;
			if(g_manualrec_startMin>=60)	g_manualrec_startMin = 0;
			g_manualrec_offset++;
			if(g_manualrec_offset > 3)
			{
				g_manualrec_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_EndTime_ManualRec);
			}
				GUI_ENC_API_Update(ID_ODC_StartTime_ManualRec, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_EndTime_ManualRec))
		{
			switch(g_manualrec_offset)
			{
			case 0:
				g_manualrec_endHour = (BU08)((BU16)g_manualrec_endHour%10 + key*10);
				break;
			case 1:
				g_manualrec_endHour = (BU08)((BU16)g_manualrec_endHour-g_manualrec_endHour%10 + key);
				break;
			case 2:
				g_manualrec_endMin = (BU08)((BU16)g_manualrec_endMin%10 + key*10);
				break;
			case 3:
				g_manualrec_endMin = (BU08)((BU16)g_manualrec_endMin-g_manualrec_endMin%10 + key);
				break;
			default:
				break;
			}
			if(g_manualrec_endHour>=24)	g_manualrec_endHour = 23;
			if(g_manualrec_endMin>=60)	g_manualrec_endMin = 59;
			g_manualrec_offset++;
			if(g_manualrec_offset > 3)
			{
				g_manualrec_offset = 0;
				GUI_ENC_API_SetFocus(ID_LBP_Prog_ManualRec);
			}
				GUI_ENC_API_Update(ID_ODC_EndTime_ManualRec, 1);
		}
		break;
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		if(GUI_ENC_API_IsFocus(ID_LBP_Prog_ManualRec))
		{
			if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)return;
			GUI_ENC_API_GetValue(ID_LBP_Prog_ManualRec,WDT_LBP_SV_INDEX,&index,0);
			curChn = (BU16)(index+g_manualrec_curPage*PAGE_CHN_NUM);
			APP_ChnDatabase_API_SetCurChnNO(curChn);
			GUI_ENC_API_GetValue(ID_LBP_Prog_ManualRec,WDT_LBP_SV_KEYSTATE,&keyState,0);
			if(!keyState)//连续键处理
			{
			//UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_ProgList,ID_STA_event2_ProgList,1);
				UI_APP_API_PlayChn(curChn, curChn, 1);
			//	UI_APP_API_CheckLock_PlayChn_led(curChn,curChn,1);//播放节目
			}
			GUI_ENC_API_SetValue(ID_SBR_ProgList, WDT_SBR_SV_VALUE, (BU32)curChn, 0);
		}
		else if (ID_LBP_Week_ManualRec)
		{
			GUI_ENC_API_GetValue(ID_LBP_Week_ManualRec, WDT_LBP_SV_INDEX, &index, 0);
	//		GUI_ENC_API_SetValue(ID_LBP_Week_ManualRec, WDT_LBP_SV_INDEX, 6, 0);
			GUI_ENC_API_SetValue(ID_LBP_Week_ManualRec, WDT_LBP_SV_INDEX, index, 0);

		//	ManualRec_IPI_ShowWeek();
		}
		break;
	case GUI_VK_LEFT:
		if(GUI_ENC_API_IsFocus(ID_LBP_Prog_ManualRec))
		{
			GUI_ENC_API_SetFocus(ID_ODC_EndTime_ManualRec);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_EndTime_ManualRec))
		{
			if(g_manualrec_offset == 0)
			{
				g_manualrec_offset = 3;
				GUI_ENC_API_SetFocus(ID_ODC_StartTime_ManualRec);
			}
			else
			{
				g_manualrec_offset--;
			}
			GUI_ENC_API_Update(ID_ODC_EndTime_ManualRec, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_ManualRec))
		{
			if(g_manualrec_offset == 0)
			{
				g_manualrec_offset = 3;
				GUI_ENC_API_SetFocus(ID_LBP_Week_ManualRec);
			}
			else
			{
				g_manualrec_offset--;
			}
			GUI_ENC_API_Update(ID_ODC_StartTime_ManualRec, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_LBP_Week_ManualRec))
		{
			GUI_ENC_API_SetFocus(ID_LBP_Prog_ManualRec);
		}
		break;
	case GUI_VK_RIGHT:
		if(GUI_ENC_API_IsFocus(ID_LBP_Prog_ManualRec))
		{
			GUI_ENC_API_SetFocus(ID_LBP_Week_ManualRec);
		}
		else if(GUI_ENC_API_IsFocus(ID_LBP_Week_ManualRec))
		{
			GUI_ENC_API_SetFocus(ID_ODC_StartTime_ManualRec);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_StartTime_ManualRec))
		{
			if(g_manualrec_offset == 3)
			{
				g_manualrec_offset = 0;
				GUI_ENC_API_SetFocus(ID_ODC_EndTime_ManualRec);
			}
			else
			{
				g_manualrec_offset++;
			}
			GUI_ENC_API_Update(ID_ODC_StartTime_ManualRec, 1);
		}
		else if(GUI_ENC_API_IsFocus(ID_ODC_EndTime_ManualRec))
		{
			if(g_manualrec_offset == 3)
			{
				g_manualrec_offset = 0;
				GUI_ENC_API_SetFocus(ID_LBP_Prog_ManualRec);
			}
			else
			{
				g_manualrec_offset++;
			}
			GUI_ENC_API_Update(ID_ODC_EndTime_ManualRec, 1);
		}
		break;
	case GUI_VK_SELECT:
		curChn = APP_ChnDatabase_API_GetCurChnNO();
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo))
		{
			return;
		}
		GUI_ENC_API_GetValue(ID_LBP_Week_ManualRec, WDT_LBP_SV_INDEX, &index, 0);
		APP_SYS_API_GetLocalDateAndTime(&now);
		APP_Com_API_NormalDateToCodeDate(now.m_year,now.m_month,now.m_day,&CodeDate);
		CodeDate = CodeDate + (BU32)index;
		APP_Com_API_CodeDateToNormalDate(CodeDate,&year,&month,&day,&weekday);
		RecData.serviceId = chnInfo.m_ProgId;
		strcpy(RecData.chnName,chnInfo.m_ChnName);
		RecData.day = year*10000 + month*100 + day;
		RecData.week = weekday;
		if(RecData.week == now.m_weekday )
		{
			recStartMin = g_manualrec_startHour*60 + g_manualrec_startMin;
			nowMin = now.m_hour*60 + now.m_min;
			if(recStartMin < nowMin + 2)
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo, WDT_STA_SV_STRING, (BU32)"预录时间至少间隔当前时间2分钟", 0);
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
				return;
			}
		}
		RecData.startHour = g_manualrec_startHour;
		RecData.startMin = g_manualrec_startMin;
		RecData.endHour = g_manualrec_endHour;
		RecData.endMin = g_manualrec_endMin;
		APP_Printf("day = %d,week = %d,starthour =%d,startmin = %d,endhour = %d,endmin = %d\n",RecData.day,
		RecData.week,RecData.startHour,RecData.startMin,RecData.endHour,RecData.endMin);
		if(ManualRec_IPI_GetConflictRec(&g_manualrec_conflictIndex,&RecData))//冲突
		{
			APP_EPG_API_GetReservedInfo(g_manualrec_conflictIndex, 1, &conflictInfo);
			APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &language);
			if(language == APP_SYS_LANGUAGE_CHINESE_e)
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo, WDT_STA_SV_STRING, (BU32)"当前时间已预约", 0);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo, WDT_STA_SV_STRING, (BU32)"Cur Time Have Been Reservered", 0);
			}
			GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
		}
		else if(APP_EPG_API_GetReserveAmount() >= MAX_RESERVE_NUM)//预约列表满
		{
			GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo, WDT_STA_SV_DATAID, RS_ReserveFull, 0);
			GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
		}
		else
		{
			APP_EPG_API_SetManualRecState(&RecData);
			UI_APP_API_SetbSaveFlag(1);
		//APP_EPG_API_UpDataToFlash();
			GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo, WDT_STA_SV_DATAID, RS_SaveWait, 0);
			GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
			GUI_ENC_API_SwitchState(ID_DLG_Effects, 1);
		}
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_Effects,1);
		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);
		break;
	default:
		break;
	}
}

void UI_APP_ManualRec_Prog_Change(void)
{
	BS32 state;

	GUI_ENC_API_GetValue(ID_LBP_Prog_ManualRec, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_manualrec_curPage++;
			break;
		case -1:
			if(g_manualrec_curPage == 0)
				g_manualrec_curPage = g_manualrec_pageNum -1;
			else
				g_manualrec_curPage--;
			
			if(g_manualrec_curPage == g_manualrec_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_Prog_ManualRec, WDT_LBP_SV_INDEX, PAGE_CHN_NUM-1, 0);
			}
			break;
		default:
			break;
	}
	ManualRec_IPI_ShowCurPage();//显示当前页

}
void UI_APP_ManualRec_Week_Change(void)
{
	BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_Week_ManualRec, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	//GUI_ENC_API_Update(ID_LBP_Week_ManualRec, 1);


	APP_PRINT_DEBUG("UI_APP_ManualRec_Week_Change state = %d\n", state);
}
