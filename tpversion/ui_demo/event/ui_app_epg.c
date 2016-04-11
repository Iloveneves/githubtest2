#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"


#define EPG_PAGE_CHN_NUM  5
#define EPG_PAGE_EVENT_NUM 9
#define EPG_CHN_NAME_LEN 20
#define EPG_EVENT_NAME_LEN 33


static BU32 g_chnEPGList_curSortIndex = 0;
static BU08 g_chnEPGList_sortName[APP_CHN_NAME_LEN];
extern APP_NvodReserve_S nvodReserveInfo;
static BU08 g_epg_chnName[EPG_PAGE_CHN_NUM][EPG_CHN_NAME_LEN];
static BU08 g_epg_eventName[EPG_PAGE_EVENT_NUM][EPG_EVENT_NAME_LEN];


static EPG_CHN_LIST_S g_epg_chnList_s[EPG_PAGE_CHN_NUM] = 
{
   { g_epg_chnName[0],0,0},
   { g_epg_chnName[1],0,0},
   { g_epg_chnName[2],0,0},
   { g_epg_chnName[3],0,0},
   { g_epg_chnName[4],0,0},
};


static EPG_EVENT_LIST_S g_epg_eventList_s[EPG_PAGE_EVENT_NUM] = 
{
   { g_epg_eventName[0],0,0,0,0},
   { g_epg_eventName[1],0,0,0,0},
   { g_epg_eventName[2],0,0,0,0},
   { g_epg_eventName[3],0,0,0,0},
   { g_epg_eventName[4],0,0,0,0},
   { g_epg_eventName[5],0,0,0,0},
   { g_epg_eventName[6],0,0,0,0},
   { g_epg_eventName[7],0,0,0,0},
   { g_epg_eventName[8],0,0,0,0},

};

static BU32 g_epg_chnCurPage = 0;
static BU32 g_epg_chnPageNum = 0;
static BU32 g_epg_chnRest = 0;

static BU32 g_epg_eventCurPage = 0;
static BU32 g_epg_eventPageNum = 0;
static BU32 g_epg_eventRest = 0;

static BU08 g_epg_dayOffset = 0;
static BU08 g_epg_dateStr[30];

static BU32 g_epg_curEvent;
static BU32 g_epg_eventNum;
static BU32 epgTimer = 0;

/*其实暂时可以不需要这个变量*/
RESERVE_TYPE_E g_epg_reserveType = RESERVE_NULL;

BU08 g_epg_reserveStr[4][40];
static BU32 g_epg_conflictIndex = 0xff;

static BU08 g_detail = 0;
static BU08 g_epg_eventNameStr[APP_EPG_NAME_LEN];
static BU08 g_epg_detailNameStr[256];

static BU32 EPG_IPI_GetConflictEvent(BU32 *conflictIndex)
{
	BU32 reservedNum,ret=0;
	BU32 eventMin,reserveMin;
	BU32 i=0,index =0;
	Epg_Info_S reserveInfo[MAX_RESERVE_NUM];
	Epg_Info_S EpgInfo;

	*conflictIndex = 0xff;
	
	reservedNum = APP_EPG_API_GetReserveAmount();
	
	if(reservedNum == 0) return 0;
	
	GUI_ENC_API_GetValue(ID_LBP_Event_EPG,WDT_LBP_SV_INDEX,&index,0);
	index = index+g_epg_eventCurPage*EPG_PAGE_EVENT_NUM;
	APP_EPG_API_GetEpgInfo(g_epg_dayOffset, index, 1, &EpgInfo);

	//APP_Printf("event: %u\t%u\t%u\n",EpgInfo.week,EpgInfo.startHour,EpgInfo.startMin);

	APP_EPG_API_GetReservedInfo(0, reservedNum, reserveInfo);
	for(i=0;i<reservedNum;i++)
	{
		//APP_Printf("reser: %u\t%u\t%u\n",reserveInfo[i].week,reserveInfo[i].startHour,reserveInfo[i].startMin);
		if(reserveInfo[i].week == EpgInfo.week || reserveInfo[i].day == EpgInfo.day)
		{
			reserveMin = reserveInfo[i].startHour*60 + reserveInfo[i].startMin;
			eventMin   = EpgInfo.startHour*60 + EpgInfo.startMin;
			if(reserveMin >= eventMin -1 && reserveMin <= eventMin +1)
			{
				*conflictIndex  = i;
				ret =1;
				break;
			}
		}
	}
	return ret;
}

/*----------------------------------------------------------------------------
描述：每次创建列表成功后调用
-----------------------------------------------------------------------------*/
static void EPG_IPI_SetChnPageParam(void)
{
	BU16 chnNum,curChn;
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	g_epg_chnPageNum = (chnNum +  EPG_PAGE_CHN_NUM -1) / EPG_PAGE_CHN_NUM;	
	g_epg_chnRest = chnNum % EPG_PAGE_CHN_NUM;
	g_epg_chnCurPage = curChn / EPG_PAGE_CHN_NUM;
	if(g_epg_chnRest == 0)
	{
		g_epg_chnRest = EPG_PAGE_CHN_NUM;
	}

	GUI_ENC_API_SetValueUnPaint(ID_SBR_Prog_EPG, WDT_SBR_SV_MAX, (BU32)chnNum-1, 0);
	GUI_ENC_API_SetValueUnPaint(ID_SBR_Prog_EPG, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_Prog_EPG, WDT_SBR_SV_VALUE, (BU32)curChn, 0);
}

static void EPG_IPI_ShowCurChnPage(void)
{
	ChnDataBase_ChnInfo_S ChnInfo;
	BU16 ChnNo,i;
	BU16 pageNum;
	if(g_epg_chnPageNum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_Prog_EPG, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(g_epg_chnCurPage >= g_epg_chnPageNum)
	{
		g_epg_chnCurPage = 0;
	}
	
	if(g_epg_chnCurPage+1 == g_epg_chnPageNum)
	{	
		pageNum = g_epg_chnRest;
	}
	else
	{
		pageNum = EPG_PAGE_CHN_NUM;
	}
	
	for(i=0; i<pageNum; i++)
	{	
		ChnNo = i+g_epg_chnCurPage*EPG_PAGE_CHN_NUM;
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ChnNo,&ChnInfo))
		{
			GUI_ENC_API_SetValue(ID_LBP_Prog_EPG, WDT_LBP_SV_DATA, (BU32)g_epg_chnList_s, i);
			return;
		}
		ChnInfo.m_ChnName[EPG_CHN_NAME_LEN-7]=0;
		if(ChnInfo.m_Ca)
		{
			sprintf(g_epg_chnList_s[i].chnName,"￥%03d %s",ChnNo + APP_ChnDatabase_API_GetChn0Flag(),ChnInfo.m_ChnName);
		}
		else
		{
			sprintf(g_epg_chnList_s[i].chnName,"  %03d %s",ChnNo + APP_ChnDatabase_API_GetChn0Flag(),ChnInfo.m_ChnName);
		}
		g_epg_chnList_s[i].fav= ChnInfo.m_Fav;
		g_epg_chnList_s[i].lock= ChnInfo.m_Lock;
	}
	GUI_ENC_API_SetValue(ID_LBP_Prog_EPG, WDT_LBP_SV_DATA, (BU32)g_epg_chnList_s, pageNum);
}
static void EPG_IPI_SetEventPageParam(void)
{
	g_epg_eventPageNum = (g_epg_eventNum +  EPG_PAGE_EVENT_NUM -1) / EPG_PAGE_EVENT_NUM;	
	g_epg_eventRest = g_epg_eventNum % EPG_PAGE_EVENT_NUM;
	g_epg_eventCurPage = g_epg_curEvent / EPG_PAGE_EVENT_NUM;
	if(g_epg_eventRest == 0)
	{
		g_epg_eventRest = EPG_PAGE_EVENT_NUM;
	}
	GUI_ENC_API_SetValueUnPaint(ID_SBR_Event_EPG, WDT_SBR_SV_MAX, (BU32)g_epg_eventNum-1, 0);
	GUI_ENC_API_SetValueUnPaint(ID_SBR_Event_EPG, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_Event_EPG, WDT_SBR_SV_VALUE, (BU32)g_epg_curEvent, 0);
}

static void EPG_IPI_ShowCurEventPageEx(void)
{
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOW_CUR_EPG, 0, 0);
}
static void EPG_IPI_ShowCurEventPage(void)
{
	BU32 pageNum,i=0;
	BU16 curChn;
	Epg_Info_S epgInfo[EPG_PAGE_EVENT_NUM];
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	BU32 language;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &language);
	
	if(g_epg_eventPageNum == 0)
	{
		if(language == APP_SYS_LANGUAGE_CHINESE_e)
		{
			sprintf(g_epg_eventList_s[i].eventName,"正在搜索预告信息...");
		}
		else
		{
			sprintf(g_epg_eventList_s[i].eventName,"Searching EPG...");
		}
		g_epg_eventList_s[0].now = 0;
		g_epg_eventList_s[0].pass= 0;
		g_epg_eventList_s[0].reserved = 0;
		g_epg_eventList_s[0].info = 0;
		GUI_ENC_API_SetValue(ID_LBP_Event_EPG, WDT_LBP_SV_DATA, (BU32)g_epg_eventList_s, 1);
		epgTimer = 0;
		return;
	}

	if(g_epg_eventCurPage >= g_epg_eventPageNum)
	{
		g_epg_eventCurPage = 0;
	}
	
	pageNum = (g_epg_eventCurPage+1 == g_epg_eventPageNum)?g_epg_eventRest:EPG_PAGE_EVENT_NUM;
	
	APP_EPG_API_GetEpgInfo(0, g_epg_eventCurPage*EPG_PAGE_EVENT_NUM, pageNum, epgInfo);
	for(i=0;i<pageNum;i++)
	{
		epgInfo[i].epgName[EPG_EVENT_NAME_LEN-7]=0;
		sprintf(g_epg_eventList_s[i].eventName,"%02d:%02d %s",epgInfo[i].startHour,epgInfo[i].startMin,epgInfo[i].epgName);
		g_epg_eventList_s[i].now = (APP_EPG_PLAYSTATE_PLAYING_e == epgInfo[i].playState)?1:0;
		g_epg_eventList_s[i].pass= (epgInfo[i].reserveState == 2)?1:0; 
		g_epg_eventList_s[i].reserved = (epgInfo[i].reserveState == 1)?1:0;
		g_epg_eventList_s[i].info = epgInfo[i].detailState;
		//APP_Printf("%-30s %u\n",g_epg_eventList_s[i].eventName,g_epg_eventList_s[i].reserved );
	}
	GUI_ENC_API_SetValue(ID_LBP_Event_EPG, WDT_LBP_SV_DATA, (BU32)g_epg_eventList_s, pageNum);
}
static void EPG_IPI_ShowHelp(BU08 bFocusOnEvent)
{
	GUI_ENC_API_Update(ID_STA_HelpBk_EPG, 1);
	if(bFocusOnEvent)
	{
//	    GUI_ENC_API_SetValue(ID_STA_LRMenu1_EPG, WDT_STA_SV_DATAID, DATAID_NULL, 0);
        GUI_ENC_API_SetValue(ID_STA_GreenIcon_EPG, WDT_STA_SV_STYLE, STYLE_NULL, 0);
        GUI_ENC_API_SetValue(ID_STA_ResList_EPG, WDT_STA_SV_DATAID, DATAID_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_ReturnIco_EPG, WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_Return_EPG, WDT_STA_SV_DATAID, DATAID_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_LRIcon_EPG, WDT_STA_SV_STYLE, STYLE_NULL, 0);
		GUI_ENC_API_SetValue(ID_STA_Info_EPG, WDT_STA_SV_DATAID, DATAID_NULL, 0);

		GUI_ENC_API_SetValue(ID_STA_OkIcon_EPG,WDT_STA_SV_STYLE,  STYLE_OkCn_W46_H28,0);
		GUI_ENC_API_SetValue(ID_STA_ResProg_EPG,WDT_STA_SV_DATAID, RS_ReserveProg,0);
		GUI_ENC_API_SetValue(ID_STA_PreDayIco_EPG,WDT_STA_SV_STYLE,  STYLE_EpgHelpL_W48_H26,0);
		GUI_ENC_API_SetValue(ID_STA_PreDay_EPG,WDT_STA_SV_DATAID, RS_PreDay,0);
		GUI_ENC_API_SetValue(ID_STA_ReturnIco_EPG,WDT_STA_SV_STYLE,STYLE_EpgHelpR_W48_H26,0);
		GUI_ENC_API_SetValue(ID_STA_Return_EPG,WDT_STA_SV_DATAID, RS_NexDay,0);
		GUI_ENC_API_SetValue(ID_STA_LRIcon_EPG,WDT_STA_SV_STYLE,STYLE_GreenKey_W20_H20,0);
		GUI_ENC_API_SetValue(ID_STA_Info_EPG,WDT_STA_SV_DATAID, RS_EpgInfo,0);
//		GUI_ENC_API_SetValue(ID_STA_LRMenu2_EPG,WDT_STA_SV_DATAID,  RS_FnKey,0);

	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_OkIcon_EPG,WDT_STA_SV_STYLE,  STYLE_NULL,0);
		GUI_ENC_API_SetValue(ID_STA_ResProg_EPG,WDT_STA_SV_DATAID, DATAID_NULL,0);
		GUI_ENC_API_SetValue(ID_STA_PreDayIco_EPG,WDT_STA_SV_STYLE,  STYLE_NULL,0);
		GUI_ENC_API_SetValue(ID_STA_PreDay_EPG,WDT_STA_SV_DATAID, DATAID_NULL,0);
		GUI_ENC_API_SetValue(ID_STA_ReturnIco_EPG,WDT_STA_SV_STYLE,STYLE_NULL,0);
		GUI_ENC_API_SetValue(ID_STA_Return_EPG,WDT_STA_SV_DATAID, DATAID_NULL,0);
		GUI_ENC_API_SetValue(ID_STA_LRIcon_EPG,WDT_STA_SV_STYLE,STYLE_NULL,0);
		GUI_ENC_API_SetValue(ID_STA_Info_EPG,WDT_STA_SV_DATAID, DATAID_NULL,0);
//		GUI_ENC_API_SetValue(ID_STA_LRMenu2_EPG,WDT_STA_SV_DATAID,  DATAID_NULL,0);
	
//        GUI_ENC_API_SetValue(ID_STA_LRMenu1_EPG, WDT_STA_SV_DATAID, RS_FnKey, 0);
        GUI_ENC_API_SetValue(ID_STA_GreenIcon_EPG, WDT_STA_SV_STYLE, STYLE_GreenKey_W20_H20, 0);
        GUI_ENC_API_SetValue(ID_STA_ResList_EPG, WDT_STA_SV_DATAID, RS_ReserveList, 0);
		GUI_ENC_API_SetValue(ID_STA_ReturnIco_EPG, WDT_STA_SV_STYLE, STYLE_ReturnCn_W46_H28, 0);
		GUI_ENC_API_SetValue(ID_STA_Return_EPG, WDT_STA_SV_DATAID, RS_Return, 0);
		GUI_ENC_API_SetValue(ID_STA_LRIcon_EPG, WDT_STA_SV_STYLE, STYLE_SelectLR_W46_H28, 0);
		GUI_ENC_API_SetValue(ID_STA_Info_EPG, WDT_STA_SV_DATAID, RS_SwitchWindown, 0);
	}

}
static void EPG_IPI_ShowDate(void)
{

	BU32 CodeDate;
	BU16 year;
	BU08 weekday,month,day;
	APP_Date_S now;
	static BU08 index =0;
	
	APP_SYS_API_GetLocalDateAndTime(&now);
	if(g_epg_dayOffset == 0)
	{
		weekday = now.m_weekday;
		year = now.m_year;
		month = now.m_month;
		day = now.m_day;
	}
	else
	{
		APP_Com_API_NormalDateToCodeDate(now.m_year,now.m_month,now.m_day,&CodeDate);
		CodeDate = CodeDate + (BU32)g_epg_dayOffset;
		APP_Com_API_CodeDateToNormalDate(CodeDate,&year,&month,&day,&weekday);
	}
	if(index != weekday)
	{
		index = weekday;
		GUI_ENC_API_SetValue(ID_TAB_EPG, WDT_TAB_SV_INDEX, weekday-1, 0);
	}
	
}


void EPG_IPI_ChangeSort()
{
	BU32 chnNum;
	BU08 flag = 0;//是否需要进行检查上一个分类的节目是否再下一个分类的标志
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 serviceID;
	BU32 curIndex = 0;
	
	APP_PRINT_DEBUG("g_chnList_curSortIndex = %d\n", g_chnEPGList_curSortIndex);
	if (APP_ChnDatabase_API_GetCurTypeChnNum())
	{
		flag = 1;
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),  APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
		{
			flag = 0;
		}
		else
		{
			serviceID = chnInfo.m_ProgId;
			APP_PRINT_DEBUG("serviceID = %d\n", serviceID);
		}
	}
	APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), g_chnEPGList_curSortIndex);
	APP_ChnDatabase_API_GetSortName(g_chnEPGList_curSortIndex,g_chnEPGList_sortName);
	g_chnEPGList_sortName[16]=0;
	//GUI_ENC_API_SetValue(ID_STA_SortName_EPG,WDT_STA_SV_STRING,(BU32)g_chnEPGList_sortName,0);

	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	if(chnNum)
	{
		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
		if (flag)
		{
			if (APP_FAIL == APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), serviceID,&chnInfo))
			{
				APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),  0, &chnInfo);
				APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e, chnInfo.m_ProgId, &chnInfo);
				APP_ChnDatabase_API_SetTypeAllCurChnNO(chnInfo.m_ChnIndex);

				APP_ChnDatabase_API_SetCurChnNO(0);
				UI_APP_API_PlayChn(0, 0, 1);		
			}
			else
			{
				APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
				//curIndex = chnInfo.m_ChnIndex;
				UI_APP_API_PlayChn(chnInfo.m_ChnIndex, chnInfo.m_ChnIndex, 1);
			}
		}
		else
		{
			APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),  0, &chnInfo);
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e, chnInfo.m_ProgId, &chnInfo);
			APP_ChnDatabase_API_SetTypeAllCurChnNO(chnInfo.m_ChnIndex);
			APP_ChnDatabase_API_SetCurChnNO(0);
			UI_APP_API_PlayChn(0, 0, 1);
		}
	}
	else
	{
		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
		APP_Com_API_StopProg(1);
		if (APP_CHN_AV_TYPE_TV_e == APP_ChnDatabase_API_GetCurChnType())
			APP_Com_API_ShowLED("C000");
		else
			APP_Com_API_ShowLED("R000");
		APP_Com_API_SetEcmNULL();
	}
	GUI_ENC_API_SetValueUnPaint(ID_LBP_Prog_EPG, WDT_LBP_SV_INDEX, (BU32)(APP_ChnDatabase_API_GetCurChnNO()%EPG_PAGE_CHN_NUM), 0);
	EPG_IPI_SetChnPageParam();
	EPG_IPI_ShowCurChnPage();
}

void EPG_IPI_UpdateEventList(void)
{
	static BU32 oldTime = 0;
	APP_Date_S dateAndTime;
	BU32 eventNum_new=0, curEvent_new=0;
	BU32 language;
	BU32 curPage,index;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &language);

	APP_SYS_API_GetLocalDateAndTime(&dateAndTime);
	APP_EPG_API_GetEpgCountAndIndex(APP_ChnDatabase_API_GetCurChnNO(), g_epg_dayOffset, &eventNum_new, &curEvent_new); 

	if(epgTimer++ < 3)
		return;
	if(eventNum_new == 0)
	{
		if(epgTimer == 8)
		{
			if(language == APP_SYS_LANGUAGE_CHINESE_e)
			{
				sprintf(g_epg_eventList_s[0].eventName,"未搜到任何节目信息");
			}
			else
			{
				sprintf(g_epg_eventList_s[0].eventName,"No EPG information");
			}
			g_epg_eventList_s[0].now = 0;
			g_epg_eventList_s[0].pass= 0;
			g_epg_eventList_s[0].reserved = 0;
			g_epg_eventList_s[0].info = 0;
			GUI_ENC_API_SetValue(ID_LBP_Event_EPG, WDT_LBP_SV_DATA, (BU32)g_epg_eventList_s, 1);
		}
	}
	else if(eventNum_new == g_epg_eventNum && oldTime == dateAndTime.m_min)
	{
		return;
	}
	else
	{
		g_epg_eventNum = eventNum_new;
		g_epg_curEvent = curEvent_new;
		oldTime = dateAndTime.m_min;
	 	curPage = g_epg_eventCurPage;   /*lx-090513:保存EPG切换到的第几页-*/
		EPG_IPI_SetEventPageParam();
		g_epg_eventCurPage= curPage;	/*lx-090513:保存EPG切换到的第几页-*/
		GUI_ENC_API_GetValue(ID_LBP_Event_EPG,WDT_LBP_SV_INDEX,&index,0);
		
		if(g_epg_eventCurPage*EPG_PAGE_EVENT_NUM+index > g_epg_eventNum)
		{
			if(!index)
			{
			 	GUI_ENC_API_SetValueUnPaint(ID_LBP_Event_EPG, WDT_LBP_SV_INDEX, (BU32)(index-1), 0);
			}
			else
			{
				g_epg_eventCurPage--;
				GUI_ENC_API_SetValueUnPaint(ID_LBP_Event_EPG, WDT_LBP_SV_INDEX, (BU32)(EPG_PAGE_EVENT_NUM-1), 0);
			}
		}
		EPG_IPI_ShowCurEventPage();
	}
}

void UI_APP_EPG_Enter(void)
{
	BU16 chnNum,curChn;
	BU32 preState;
	g_detail = 0;
	BU32 hwnd;
	GUI_Rect_s rect;
	GUI_ENC_API_SetVisible(ID_STA_EventName_Detail, WS_DISABLE);	
	GUI_ENC_API_SetVisible(ID_EDT_Detail_EPG, WS_DISABLE);	
	GUI_ENC_API_SetVisible(ID_STA_DetailTitle_Epg, WS_DISABLE);	
	GUI_ENC_API_Update(ID_STA_TitleBk_EPG,1);
	GUI_ENC_API_Update(ID_STA_DetailBk_EPG,1);
	GUI_ENC_API_SetVisible(ID_TAB_EPG, WS_VISIBLE);	
	GUI_ENC_API_SetVisible(ID_LBP_Event_EPG, WS_VISIBLE);
	GUI_ENC_API_SetVisible(ID_SBR_Event_EPG, WS_VISIBLE);	
	GUI_ENC_API_Update(ID_TAB_EPG,1);
	GUI_ENC_API_Update(ID_LBP_Event_EPG,1);
	GUI_ENC_API_Update(ID_SBR_Event_EPG,1);
	preState = GUI_ENC_API_GetPrevState();	
	Hisi_SetUpdateDlgID(ID_DLG_EPG);
	if(preState == ID_DLG_ConflictInfo || preState == ID_DLG_EpgReserveInfo)
	{
		GUI_ENC_API_SetFocus (ID_LBP_Event_EPG);
		return;
	}
	else if(preState == ID_DLG_ReserveList)
	{
	    GUI_ENC_API_SetFocus (ID_LBP_Prog_EPG);
		return;
	}
	g_epg_dayOffset = 0;
	EPG_IPI_ShowHelp(0);
	APP_EPG_API_StopGetEpg();
	APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	if(chnNum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_Prog_EPG, WDT_LBP_SV_NUM, 0, 0);		
		GUI_ENC_API_SetValue(ID_LBP_Event_EPG, WDT_LBP_SV_NUM, 0, 0);
 	}
	else
	{
		g_epg_dayOffset = 0;
		EPG_IPI_ShowDate();
		curChn = APP_ChnDatabase_API_GetCurChnNO();
		EPG_IPI_SetChnPageParam();
		EPG_IPI_ShowCurChnPage();
		GUI_ENC_API_SetValue(ID_LBP_Prog_EPG, WDT_LBP_SV_INDEX, (BU32)(curChn%EPG_PAGE_CHN_NUM), 0);
		GUI_ENC_API_SetFocus(ID_LBP_Prog_EPG);
		/*hwnd = GUI_API_GetDlgItem(ID_VFM_EPG);
		GUI_API_GetWindowRect(hwnd,&rect);
		APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
		*/
		GUI_ENC_API_SetValueUnPaint(ID_LBP_Event_EPG, WDT_LBP_SV_INDEX, (BU32)(g_epg_curEvent%EPG_PAGE_EVENT_NUM), 0);
		APP_EPG_API_GetEpgCountAndIndex(curChn,g_epg_dayOffset, &g_epg_eventNum, &g_epg_curEvent);
		EPG_IPI_SetEventPageParam();
		EPG_IPI_ShowCurEventPage();
 		//UI_APP_API_CheckLock_PlayChn_led (curChn, curChn, 0);
 		UI_APP_API_PlayChn(curChn, curChn, 1);
	/*	APP_ChnDatabase_API_GetSortName(g_chnEPGList_curSortIndex,g_chnEPGList_sortName);
		g_chnEPGList_sortName[16]=0;
		GUI_ENC_API_SetValue(ID_STA_SortName_EPG,WDT_STA_SV_STRING,(BU32)g_chnEPGList_sortName,0);
	*/
	}
}
void UI_APP_EPG_End(void)
{

}
void UI_APP_EPG_Prog_Key(BU32 key)
{
	BU32 index;
	BU32 keyState;
	BU16 curChn;
	BU32 epg_prestate = 0;
	BU32 reserveNum;
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		epgTimer = 0;
		APP_EPG_API_StopGetEpg();
		GUI_ENC_API_GetValue(ID_LBP_Prog_EPG,WDT_LBP_SV_INDEX,&index,0);
		curChn = (BU16)(index+g_epg_chnCurPage*EPG_PAGE_CHN_NUM);
		APP_ChnDatabase_API_SetCurChnNO(curChn);
		GUI_ENC_API_SetValue(ID_SBR_Prog_EPG, WDT_SBR_SV_VALUE, (BU32)curChn, 0);

		APP_EPG_API_GetEpgCountAndIndex(curChn,g_epg_dayOffset, &g_epg_eventNum, &g_epg_curEvent);
		//APP_Printf("current event index : %d\n",curEvent);
 		GUI_ENC_API_SetValueUnPaint(ID_LBP_Event_EPG, WDT_LBP_SV_INDEX, (BU32)(g_epg_curEvent%EPG_PAGE_EVENT_NUM), 0);
		EPG_IPI_SetEventPageParam();
		EPG_IPI_ShowCurEventPage();	 	
		GUI_ENC_API_GetValue(ID_LBP_Prog_EPG,WDT_LBP_SV_KEYSTATE,&keyState,0);
		if(!keyState)//连续键处理
		{
			//UI_APP_API_CheckLock_PlayChn_led(curChn,curChn,1);//播放节目
			UI_APP_API_PlayChn(curChn, curChn, 1);
		}
		break;
	case GUI_VK_RIGHT:
		EPG_IPI_ShowHelp(1);
		GUI_ENC_API_SetFocus(ID_LBP_Event_EPG);
		break;

/*
	case GUI_VK_LEFT:
		if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
		{
			if(g_chnEPGList_curSortIndex == 0)
				g_chnEPGList_curSortIndex = APP_ChnDatabase_API_GetSortCount()-1;
			else 
				g_chnEPGList_curSortIndex --;
		}
		else if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_RD_e)
		{
			if(g_chnEPGList_curSortIndex == 0)
				g_chnEPGList_curSortIndex = 1;
			else 
				g_chnEPGList_curSortIndex = 0;
		}
		EPG_IPI_ChangeSort();
		break;
	case GUI_VK_RIGHT:
		if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
		{
			if(g_chnEPGList_curSortIndex == APP_ChnDatabase_API_GetSortCount()-1)
				g_chnEPGList_curSortIndex = 0;
			else 
				g_chnEPGList_curSortIndex ++;
		}
		else if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_RD_e)
		{
			if(g_chnEPGList_curSortIndex == 1)
				g_chnEPGList_curSortIndex = 0;
			else 
				g_chnEPGList_curSortIndex = 1;
		}
		EPG_IPI_ChangeSort();
		break;
	case GUI_VK_F10:
		if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
			break;
		EPG_IPI_ShowHelp(1);
		GUI_ENC_API_SetFocus(ID_LBP_Event_EPG);
		break;
*/
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		if(UI_APP_API_GetEpgSaveFlag() == 1)
		{
			GUI_ENC_API_SetValue(ID_STAInfoText_EpgInfo, WDT_STA_SV_DATAID, RS_SaveWait, 0);
			GUI_ENC_API_SwitchState(ID_DLG_EpgInfo, 1);
		}
		else
		{
			epg_prestate = UI_APP_API_GetEpgPreState();
			GUI_ENC_API_SwitchState(epg_prestate, 1);
		}
		g_chnEPGList_curSortIndex = 0;
		break;
	case GUI_VK_GREEN:
		#if 0
		if(UI_APP_API_GetEpgSaveFlag() == 1)
		{
			UI_APP_API_SetEpgSaveFlag(0);
			APP_EPG_API_UpDataToFlash();
		}
		#endif
		//reserveNum = APP_EPG_API_GetReserveAmount();
		//if (reserveNum != 0)
		{
			UI_APP_API_SetReserveListPreState(ID_DLG_EPG);
			GUI_ENC_API_SwitchState(ID_DLG_ReserveList, 1);
		}
		break;
 	case GUI_VK_SELECT:
	case GUI_VK_EXIT:
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
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
				//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}
		}
		g_chnEPGList_curSortIndex = 0;
 		break;
	default:
		break;	
	}

}
void UI_APP_EPG_Prog_Change(void)
{
	BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_Prog_EPG, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_epg_chnCurPage++;
			break;
		case -1:
			if(g_epg_chnCurPage == 0)
				g_epg_chnCurPage = g_epg_chnPageNum -1;
			else
				g_epg_chnCurPage--;
			
			if(g_epg_chnCurPage == g_epg_chnPageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_Prog_EPG, WDT_LBP_SV_INDEX, (BU32)(EPG_PAGE_CHN_NUM-1), 0);
			}
			break;
		default:
			break;
	}
	EPG_IPI_ShowCurChnPage();//显示当前页

}
void UI_APP_EPG_Event_Key(BU32 key)
{
	BU32 index;
	BU32 detailLen=0;
	Epg_Info_S EpgInfo;
	APP_Date_S dateTime;
	BU32 language;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &language);
	APP_EPG_API_StopGetEpg();
	
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		if (g_detail == 1)
			break;
		GUI_ENC_API_GetValue(ID_LBP_Event_EPG,WDT_LBP_SV_INDEX,&index,0);
		index = index+g_epg_eventCurPage*EPG_PAGE_EVENT_NUM;
		GUI_ENC_API_SetValue(ID_SBR_Event_EPG, WDT_SBR_SV_VALUE, (BU32)index, 0);
		break;
	case GUI_VK_EXIT:
		if (g_detail == 1)
			break;
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
			//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			}
		}
		g_chnEPGList_curSortIndex =0;
		break;	
 	case GUI_VK_RECALL:
		if (g_detail == 0)
		{
			g_epg_dayOffset = 0;
			g_chnEPGList_curSortIndex = 0;
			EPG_IPI_ShowHelp(0);
			EPG_IPI_ShowDate();
			GUI_ENC_API_SetFocus(ID_LBP_Prog_EPG);
		}
 		break;
 	case GUI_VK_LEFT:
		if (g_detail == 1)
			break;
		epgTimer = 0;
		if(g_epg_dayOffset == 0)
		{
			EPG_IPI_ShowHelp(0);
			EPG_IPI_ShowDate();
			GUI_ENC_API_SetFocus(ID_LBP_Prog_EPG);

		}
		else
		{
			g_epg_dayOffset--;
			EPG_IPI_ShowDate();
			APP_EPG_API_GetEpgCountAndIndex(APP_ChnDatabase_API_GetCurChnNO(),g_epg_dayOffset, &g_epg_eventNum, &g_epg_curEvent);
	 		GUI_ENC_API_SetValueUnPaint(ID_LBP_Event_EPG, WDT_LBP_SV_INDEX, (BU32)(g_epg_curEvent%EPG_PAGE_EVENT_NUM), 0);
			EPG_IPI_SetEventPageParam();
			EPG_IPI_ShowCurEventPage();
		}
		break;
	case GUI_VK_RIGHT:
		if (g_detail == 1)
			break;
		epgTimer = 0;
		g_epg_dayOffset++;
		if(g_epg_dayOffset>6)g_epg_dayOffset=0;
		EPG_IPI_ShowDate();
		APP_EPG_API_GetEpgCountAndIndex(APP_ChnDatabase_API_GetCurChnNO(),g_epg_dayOffset, &g_epg_eventNum, &g_epg_curEvent);
 		GUI_ENC_API_SetValueUnPaint(ID_LBP_Event_EPG, WDT_LBP_SV_INDEX, (BU32)(g_epg_curEvent%EPG_PAGE_EVENT_NUM), 0);
		EPG_IPI_SetEventPageParam();
		EPG_IPI_ShowCurEventPage();
		break;
	case GUI_VK_SELECT:
		if (g_detail == 1)
			break;
		if(g_epg_eventNum == 0)	return;
		UI_APP_API_SetEpgSaveFlag(1);
		GUI_ENC_API_GetValue(ID_LBP_Event_EPG,WDT_LBP_SV_INDEX,&index,0);
		index = index+g_epg_eventCurPage*EPG_PAGE_EVENT_NUM;
		APP_EPG_API_GetEpgInfo(g_epg_dayOffset, index, 1, &EpgInfo);
		if(EpgInfo.playState == APP_EPG_PLAYSTATE_PLAYED_e)
		{
			g_epg_reserveType = RESERVE_PLAYED;
			//GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo, WDT_STA_SV_DATAID, RS_PlayedCannot, 0);
			GUI_ENC_API_SwitchState(ID_DLG_EpgReserveInfo, 1);
		}
		else if(EpgInfo.playState == APP_EPG_PLAYSTATE_PLAYING_e)
		{
			g_epg_reserveType = RESERVE_PLAYING;
			//GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo, WDT_STA_SV_DATAID, RS_PlayingNoReserve, 0);
			GUI_ENC_API_SwitchState(ID_DLG_EpgReserveInfo, 1);
		}
		else if(EpgInfo.reserveState)
		{
			g_epg_reserveType = RESERVE_CANCEL;
			APP_EPG_API_SetEvntReserveState(index, g_epg_dayOffset, 0);
			EPG_IPI_ShowCurEventPage();
		}
		else
		{	
			if(EPG_IPI_GetConflictEvent(&g_epg_conflictIndex))//冲突
			{
				Epg_Info_S conflictInfo;
				g_epg_reserveType = RESERVE_CONFLICT;
				APP_EPG_API_GetReservedInfo(g_epg_conflictIndex, 1, &conflictInfo);
				if(language == APP_SYS_LANGUAGE_CHINESE_e)
				{				
					sprintf(g_epg_reserveStr[0],"当前节目与已预约节目:");
					sprintf(g_epg_reserveStr[1],"频道: %s",conflictInfo.chnName);
					sprintf(g_epg_reserveStr[2],"节目: %s",conflictInfo.epgName);
				}
				else
				{
					sprintf(g_epg_reserveStr[0],"current Chn with reserved Chn:");
					sprintf(g_epg_reserveStr[1],"Chn: %s",conflictInfo.chnName);
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
			else if(EpgInfo.reserveState == 0)
			{
			    Epg_Info_S epgInfo;
				BU32 month;
				BU32 day;
				BU32 ret;
				g_epg_reserveType = RESERVE_OK;
				ret = APP_EPG_API_GetEpgInfo(g_epg_dayOffset, index, 1, &epgInfo);
				if(ret == APP_SUCCESS)
				{
				     month = (epgInfo.day % 10000)/100;
					 day = (epgInfo.day % 10000)%100;
					if(language == APP_SYS_LANGUAGE_CHINESE_e)
					{				
						sprintf(g_epg_reserveStr[0],"频道: %s",epgInfo.chnName);
						sprintf(g_epg_reserveStr[1],"节目: %s",epgInfo.epgName);
						sprintf(g_epg_reserveStr[2],"开始时间:%d-%d %02d:%02d",month, day, epgInfo.startHour,epgInfo.startMin);
						sprintf(g_epg_reserveStr[3],"结束时间:%d-%d %02d:%02d",month, day, epgInfo.endHour,epgInfo.endMin);
					}
					else
					{
						sprintf(g_epg_reserveStr[0],"Chn: %s",epgInfo.chnName);
						sprintf(g_epg_reserveStr[1],"Epg: %s",epgInfo.epgName);
						sprintf(g_epg_reserveStr[2],"StartTime:%d-%d %02d:%02d",month, day, epgInfo.startHour,epgInfo.startMin);
						sprintf(g_epg_reserveStr[3],"EndTime:%d-%d %02d:%02d",month, day, epgInfo.endHour,epgInfo.endMin);
					}
					g_epg_reserveStr[0][39]=0;
					g_epg_reserveStr[1][39]=0;
					g_epg_reserveStr[2][39]=0;
					g_epg_reserveStr[3][39]=0;
					GUI_ENC_API_SetValue(ID_STA_Line1_ConflictInfo,WDT_STA_SV_STRING, (BU32)g_epg_reserveStr[0],0);
					GUI_ENC_API_SetValue(ID_STA_Line2_ConflictInfo,WDT_STA_SV_STRING, (BU32)g_epg_reserveStr[1],0);
					GUI_ENC_API_SetValue(ID_STA_Line3_ConflictInfo,WDT_STA_SV_STRING, (BU32)g_epg_reserveStr[2],0);
					GUI_ENC_API_SetValue(ID_STA_Line4_ConflictInfo,WDT_STA_SV_STRING, (BU32)g_epg_reserveStr[3],0);
					GUI_ENC_API_SetValue(ID_STA_Line5_ConflictInfo,WDT_STA_SV_DATAID, RS_ReserveSure,0);
		
					GUI_ENC_API_SwitchState(ID_DLG_ConflictInfo, 1);
				}
			}
		}
		break;
 	case GUI_VK_GREEN:
		if (g_detail == 1)
			break;
		GUI_ENC_API_GetValue(ID_LBP_Event_EPG,WDT_LBP_SV_INDEX,&index,0);
		index = index+g_epg_eventCurPage*EPG_PAGE_EVENT_NUM;

		APP_EPG_API_GetEpgInfo(g_epg_dayOffset, index, 1, &EpgInfo);

		GUI_API_Strcpy(g_epg_eventNameStr, EpgInfo.epgName);
		g_epg_eventNameStr[APP_EPG_NAME_LEN - 1] = '\0';
		if(g_epg_eventNum == 0)
			g_epg_eventNameStr[0] = '\0';
		GUI_ENC_API_SetValue(ID_STA_EventName_Detail,WDT_STA_SV_STRING,(BU32)g_epg_eventNameStr,0);
		
//		if(EpgInfo.detailState == 1)
		{
			
	 		APP_EPG_API_GetEventDetailText(index, g_epg_detailNameStr, 255);
			//GUI_API_Memcpy(g_epg_detailNameStr, text, 255);
			detailLen = GUI_API_Strlen(g_epg_detailNameStr);
			APP_Printf("g_epg_detailNameStr = %s %d\n", g_epg_detailNameStr, detailLen);
			GUI_ENC_API_SetValue(ID_EDT_Detail_EPG,WDT_EDT_SV_PAGE_INDEX,0,0);
			GUI_ENC_API_SetValue(ID_EDT_Detail_EPG, WDT_EDT_SV_VALUE, (BU32)g_epg_detailNameStr, detailLen);

		}
		if(g_detail == 0)
		{
			g_detail = 1;
			GUI_ENC_API_SetVisible(ID_TAB_EPG, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_LBP_Event_EPG, WS_DISABLE);
			GUI_ENC_API_SetVisible(ID_SBR_Event_EPG, WS_DISABLE);
			
			GUI_ENC_API_Update(ID_STA_HelpBk_EPG, 1);
			GUI_ENC_API_SetValue(ID_STA_ReturnIco_EPG, WDT_STA_SV_STYLE, STYLE_ReturnCn_W46_H28, 0);
			GUI_ENC_API_SetValue(ID_STA_Return_EPG, WDT_STA_SV_DATAID, RS_Return, 0);
			GUI_ENC_API_Update(ID_STA_TitleBk_EPG,1);
			GUI_ENC_API_Update(ID_STA_DetailBk_EPG,1);
			GUI_ENC_API_SetVisible(ID_STA_EventName_Detail, WS_VISIBLE);	
			GUI_ENC_API_SetVisible(ID_EDT_Detail_EPG, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_STA_DetailTitle_Epg, WS_VISIBLE);	
			GUI_ENC_API_Update(ID_STA_EventName_Detail,1);
			GUI_ENC_API_Update(ID_EDT_Detail_EPG,1);
			GUI_ENC_API_Update(ID_STA_DetailTitle_Epg,1);
			GUI_ENC_API_SetFocus(ID_EDT_Detail_EPG);

		}
		else 
		{
			g_detail = 0;
			GUI_ENC_API_SetVisible(ID_STA_EventName_Detail, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_EDT_Detail_EPG, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_STA_DetailTitle_Epg, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_TitleBk_EPG,1);
			GUI_ENC_API_Update(ID_STA_DetailBk_EPG,1);
			GUI_ENC_API_SetVisible(ID_TAB_EPG, WS_VISIBLE);	
			GUI_ENC_API_SetVisible(ID_LBP_Event_EPG, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_SBR_Event_EPG, WS_VISIBLE);	
			GUI_ENC_API_Update(ID_TAB_EPG,1);
			GUI_ENC_API_Update(ID_LBP_Event_EPG,1);
			GUI_ENC_API_Update(ID_SBR_Event_EPG,1);
		}
		break;
#if 0
	case GUI_VK_YELLOW:
		if (g_detail == 1)
			break;
		if(g_epg_eventNum == 0)	return;
		UI_APP_API_SetEpgSaveFlag(1);
		GUI_ENC_API_GetValue(ID_LBP_Event_EPG,WDT_LBP_SV_INDEX,&index,0);
		index = index+g_epg_eventCurPage*EPG_PAGE_EVENT_NUM;
		APP_EPG_API_GetEpgInfo(g_epg_dayOffset, index, 1, &EpgInfo);
		if(EpgInfo.playState == APP_EPG_PLAYSTATE_PLAYED_e)
		{
			g_epg_reserveType = RESERVE_PLAYED;
			//GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo, WDT_STA_SV_DATAID, RS_PlayedCannot, 0);
			GUI_ENC_API_SwitchState(ID_DLG_EpgReserveInfo, 1);
		}
		else if(EpgInfo.playState == APP_EPG_PLAYSTATE_PLAYING_e)
		{
			g_epg_reserveType = RESERVE_PLAYING;
			//GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo, WDT_STA_SV_DATAID, RS_PlayingNoReserve, 0);
			GUI_ENC_API_SwitchState(ID_DLG_EpgReserveInfo, 1);
		}
		else if(EpgInfo.reserveState)
		{
			g_epg_reserveType = RESERVE_CANCEL;
			APP_EPG_API_SetEvntReserveState(index, g_epg_dayOffset, 0);
			EPG_IPI_ShowCurEventPage();
		}
		else
		{	
			if(EPG_IPI_GetConflictEvent(&g_epg_conflictIndex))//冲突
			{
				Epg_Info_S conflictInfo;
				g_epg_reserveType = RESERVE_CONFLICTREC;
				APP_EPG_API_GetReservedInfo(g_epg_conflictIndex, 1, &conflictInfo);
				if(language == APP_SYS_LANGUAGE_CHINESE_e)
				{				
					sprintf(g_epg_reserveStr[0],"当前节目与已预录节目:");
					sprintf(g_epg_reserveStr[1],"频道: %s",conflictInfo.chnName);
					sprintf(g_epg_reserveStr[2],"节目: %s",conflictInfo.epgName);
				}
				else
				{
					sprintf(g_epg_reserveStr[0],"current Chn with reserved Rec Chn:");
					sprintf(g_epg_reserveStr[1],"Chn: %s",conflictInfo.chnName);
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
			else if(EpgInfo.reserveState == 0)
			{
				g_epg_reserveType = RESERVE_OK;
				APP_EPG_API_SetEvntReserveState(index, g_epg_dayOffset, 2);
 				EPG_IPI_ShowCurEventPage();
			}
		}
		break;
#endif
 	default:
		break;
	}

}
void UI_APP_EPG_Event_Change(void)
{
	BS32 state;
	GUI_ENC_API_GetValue(ID_LBP_Event_EPG, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_epg_eventCurPage++;
			break;
		case -1:
			if(g_epg_eventCurPage == 0)
				g_epg_eventCurPage = g_epg_eventPageNum-1;
			else
				g_epg_eventCurPage--;
			
			if(g_epg_eventCurPage == g_epg_eventPageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_Event_EPG, WDT_LBP_SV_INDEX, (BU32)(EPG_PAGE_EVENT_NUM-1), 0);
			}
			break;
		default:
			break;
	}
//	if(g_epg_eventPageNum)
	EPG_IPI_ShowCurEventPage();//显示当前页

}
void UI_APP_EPG_Timer(void)
{
 	EPG_IPI_UpdateEventList();
	EPG_IPI_ShowDate();
}

void UI_APP_EPG_Detail_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_SELECT:
	case GUI_VK_EXIT:
	case GUI_VK_RECALL:
	case GUI_VK_INFO:
		if (g_detail == 1)
		{
			g_detail = 0;
			GUI_ENC_API_SetVisible(ID_STA_EventName_Detail, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_EDT_Detail_EPG, WS_DISABLE);	
			GUI_ENC_API_SetVisible(ID_STA_DetailTitle_Epg, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_TitleBk_EPG,1);
			GUI_ENC_API_Update(ID_STA_DetailBk_EPG,1);
			GUI_ENC_API_SetVisible(ID_TAB_EPG, WS_VISIBLE);	
			GUI_ENC_API_SetVisible(ID_LBP_Event_EPG, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_SBR_Event_EPG, WS_VISIBLE);	
			GUI_ENC_API_Update(ID_STA_LRIcon_EPG, 1);
			GUI_ENC_API_Update(ID_TAB_EPG,1);
			GUI_ENC_API_Update(ID_LBP_Event_EPG,1);
			GUI_ENC_API_Update(ID_SBR_Event_EPG,1);
			EPG_IPI_ShowHelp(1);
		}
		GUI_ENC_API_SetFocus(ID_LBP_Event_EPG);
		break;
	default:
		break;
	}
}
//Epg预约节目的冲突提示框

void UI_APP_ConflictInfo_Enter(void)
{

}
void UI_APP_ConflictInfo_End(void)
{

}
void UI_APP_ConflictInfo_Key(BU32 key)
{
	BU32 index;
	BU32 conflictIndex;
	BU32 eventIndex;
	Epg_Info_S conflictInfo;
	Epg_Info_S EpgInfo;
	switch(key)
	{
		case GUI_VK_EXIT:
			GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);
			break;
		case GUI_VK_SELECT:
			if(ID_DLG_EPG == GUI_ENC_API_GetPrevState())
			{
				GUI_ENC_API_GetValue(ID_LBP_Event_EPG,WDT_LBP_SV_INDEX,&index,0);
				index = index+g_epg_eventCurPage*EPG_PAGE_EVENT_NUM;
		 		APP_EPG_API_CancelReserve(g_epg_conflictIndex);
				if(g_epg_reserveType == RESERVE_CONFLICT)
				{
					APP_EPG_API_SetEvntReserveState(index, g_epg_dayOffset, 1);
				}
				else if(g_epg_reserveType == RESERVE_CONFLICTREC)
				{
					APP_EPG_API_SetEvntReserveState(index, g_epg_dayOffset, 2);
				}
				else if(g_epg_reserveType = RESERVE_OK)
				{
					APP_EPG_API_SetEvntReserveState(index, g_epg_dayOffset, 1);
				}	
				GUI_ENC_API_SwitchState(ID_DLG_EPG, 1);
				EPG_IPI_ShowCurEventPage();
			}
#ifdef ENABLE_NVOD
			else if(ID_DLG_Nvod == GUI_ENC_API_GetPrevState())
			{	
				conflictIndex = UI_APP_API_GetConflictIndex();
				APP_EPG_API_CancelReserve(conflictIndex);
				//APP_Nvod_SetRefEventRunStatusByReserveIndex(conflictIndex,RUNNING_NONE);
				//APP_EPG_API_CancelReserveType(conflictIndex,APP_ReserveType_Nvod_e);
				if(g_epg_reserveType == RESERVE_CONFLICT_NVOD)
				{
					eventIndex = UI_APP_API_GetNvodEventIndex();
					APP_Nvod_SetRefEventRunStatusByIndex(eventIndex,RUNNING_RESERVED);
					APP_EPG_API_SetNvodReserveState(&nvodReserveInfo,1);
				}
				GUI_ENC_API_SwitchState(ID_DLG_Nvod, 1);
			}
#endif
			break;
		default:
			break;
	} 	 
}

//Epg数据保存提示框
void UI_APP_EpgInfo_Timer(void)
{
	BU32 epg_prestate=ID_DLG_MainMenu;
	if(UI_APP_API_GetEpgSaveFlag() == 1)
	{
		UI_APP_API_SetEpgSaveFlag(0);
		APP_EPG_API_UpDataToFlash();
		epg_prestate = UI_APP_API_GetEpgPreState();
		GUI_ENC_API_SwitchState(epg_prestate, 1);
	}
}

//Epg及不能预约的提示，如:节目正在播放，节目也播放完，预约列表已满
void UI_APP_EpgReserveInfo_Enter(void)
{

	switch(g_epg_reserveType)
	{
	case RESERVE_PLAYED:
		GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo,WDT_STA_SV_DATAID, RS_PlayedCannot,0);
		break;
	case RESERVE_PLAYING:
		GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo,WDT_STA_SV_DATAID, RS_PlayingNoReserve,0);
		break;
	case RESERVE_FULL:
		GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo,WDT_STA_SV_DATAID, RS_ReserveFull,0);
		break;
	/*
	case RESERVE_PLAYSOON:
		GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo,WDT_STA_SV_DATAID, RS_ToPlayCannot,0);
		break;
	case RESERVE_CANCEL:
		GUI_ENC_API_SetValue(ID_STA_Line1_EpgReserveInfo,WDT_STA_SV_DATAID, RS_IfCancelReserve,0);
	*/
	default:
		break;
	}
	
}
void UI_APP_EpgReserveInfo_Key(BU32 key)
{

}

void UI_APP_EpgReserveInfo_Timer(void)
{
	GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 0);
}

