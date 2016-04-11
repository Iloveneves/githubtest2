#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


#define MAX_RESERVE_LEN 64
#define RESERVE_NUM_IN_PAGE 	8

static BU08 g_reserve_str[MAX_RESERVE_NUM][MAX_RESERVE_LEN];
static BU08 *g_reserve_list_p[MAX_RESERVE_NUM];
static BU32 g_reserve_allCount = 0;
static APP_ReserveType_e reserve_Type = 3;

void ReserveList_IPI_ShowPage(APP_ReserveType_e ReserveType)
{
	BU32 index;
	BU32 reservedNum,CodeDate,i;
	BU16 year;
	BU08 day_offset,month,day,weekday;
	APP_Date_S DateAndTime;
	Epg_Info_S EventInfo[MAX_RESERVE_NUM];
	switch(ReserveType)
	{
		case APP_ReserveType_All_e:
			GUI_ENC_API_SetValue(ID_STA_ReserveList_ReserveList,WDT_STA_SV_STRING, (BU32)"所有类型", 0);
			break;
		case APP_ReserveType_Epg_e:
			GUI_ENC_API_SetValue(ID_STA_ReserveList_ReserveList,WDT_STA_SV_STRING, (BU32)"EPG事件", 0);
			break;
		case APP_ReserveType_Rec_e:
			GUI_ENC_API_SetValue(ID_STA_ReserveList_ReserveList,WDT_STA_SV_STRING, (BU32)"所有预录", 0);
			break;
		case APP_ReserveType_EpgReserve_e:
			GUI_ENC_API_SetValue(ID_STA_ReserveList_ReserveList,WDT_STA_SV_STRING, (BU32)"EPG预约", 0);
			break;
		case APP_ReserveType_EpgRec_e:
			GUI_ENC_API_SetValue(ID_STA_ReserveList_ReserveList,WDT_STA_SV_STRING, (BU32)"EPG预录", 0);
			break;
		case APP_ReserveType_ManualRec_e:
			GUI_ENC_API_SetValue(ID_STA_ReserveList_ReserveList,WDT_STA_SV_STRING, (BU32)"选时预录", 0);
			break;
		case APP_ReserveType_Nvod_e:
			GUI_ENC_API_SetValue(ID_STA_ReserveList_ReserveList,WDT_STA_SV_STRING, (BU32)"点播预约", 0);
			break;		
		default:
			break;
	}
    reservedNum = APP_EPG_API_GetReserveTypeAmount(ReserveType);
	/*防止越界预约列表g_reserve_str*/
	if(reservedNum > MAX_RESERVE_NUM)
	{
		g_reserve_allCount = MAX_RESERVE_NUM;
	}
	else
	{
		g_reserve_allCount = reservedNum;
	}
	if(g_reserve_allCount == 0)
	{
		GUI_ENC_API_SetValue(ID_LBX_ReserveList, WDT_LBX_SV_DATA, (BU32)g_reserve_list_p,0);
		GUI_ENC_API_SetVisible(ID_STA_Notebk_ReserveList, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_ReserveList, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ReserveList, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_Notebk_ReserveList, 1);
		GUI_ENC_API_Update(ID_STA_Note_ReserveList, 1);
		GUI_ENC_API_Update(ID_STA_Title_ReserveList, 1);
	}
	else
	{
		if(ReserveType == APP_ReserveType_Nvod_e)
		{
			APP_EPG_API_GetReserveTypeInfo(0,g_reserve_allCount,ReserveType,EventInfo);
			for(i=0;i<g_reserve_allCount;i++)
			{
				GUI_API_Memset(g_reserve_str[i],0,MAX_RESERVE_LEN);
				EventInfo[i].chnName[13] = 0;
				EventInfo[i].epgName[19] = 0;
				sprintf(g_reserve_str[i]," %02d %-13s %-21s %04d-%02d-%02d %02d:%02d-%02d:%02d",i+1,EventInfo[i].chnName,EventInfo[i].epgName,
					EventInfo[i].day/10000,EventInfo[i].day%10000/100,EventInfo[i].day%10000%100,EventInfo[i].startHour,EventInfo[i].startMin,EventInfo[i].endHour,EventInfo[i].endMin);
				g_reserve_str[i][MAX_RESERVE_LEN-1]=0;
				g_reserve_list_p[i] = g_reserve_str[i];
			}
		}
		else
		{
			APP_SYS_API_GetLocalDateAndTime(&DateAndTime);
			APP_EPG_API_GetReserveTypeInfo(0,g_reserve_allCount,ReserveType,EventInfo);
			for(i=0;i<g_reserve_allCount;i++)
			{
				day_offset = (EventInfo[i].week < DateAndTime.m_weekday )?(EventInfo[i].week +7 -DateAndTime.m_weekday):(EventInfo[i].week - DateAndTime.m_weekday);
				if(day_offset)
				{
					APP_Com_API_NormalDateToCodeDate(DateAndTime.m_year,DateAndTime.m_month,DateAndTime.m_day,&CodeDate);
					CodeDate = CodeDate + (BU32)day_offset;
					APP_Com_API_CodeDateToNormalDate(CodeDate,&year,&month,&day,&weekday);
				}
				else
				{
					weekday = DateAndTime.m_weekday;
					year = DateAndTime.m_year;
					month = DateAndTime.m_month;
					day = DateAndTime.m_day;
				}
				GUI_API_Memset(g_reserve_str[i],0,MAX_RESERVE_LEN);
				EventInfo[i].chnName[13] = 0;
				EventInfo[i].epgName[21] = 0;
				sprintf(g_reserve_str[i]," %02d %-13s %-21s %04d-%02d-%02d %02d:%02d-%02d:%02d",i+1,EventInfo[i].chnName,EventInfo[i].epgName,
					EventInfo[i].day/10000,month,day,EventInfo[i].startHour,EventInfo[i].startMin,EventInfo[i].endHour,EventInfo[i].endMin);
				g_reserve_str[i][MAX_RESERVE_LEN-1]=0;
				g_reserve_list_p[i] = g_reserve_str[i];
			}
		}
		GUI_ENC_API_GetValue (ID_LBX_ReserveList, WDT_LBX_SV_INDEX, &index, 0);
		if(index>g_reserve_allCount-1)
		{
			GUI_ENC_API_SetValueUnPaint(ID_LBX_ReserveList,WDT_LBX_SV_INDEX,g_reserve_allCount-1,0);
		}
		else
		{
			if(GUI_ENC_API_GetPrevState () != ID_DLG_ReserveInfo)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBX_ReserveList,WDT_LBX_SV_INDEX,0,0);
			}
		}
		GUI_ENC_API_SetValue(ID_LBX_ReserveList, WDT_LBX_SV_DATA, (BU32)g_reserve_list_p,g_reserve_allCount);
		GUI_ENC_API_SetVisible(ID_STA_Notebk_ReserveList, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_ReserveList, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ReserveList, WS_DISABLE);
	}
}
//预约节目列表
void UI_APP_ReserveList_Enter(void)
{
/*
	if(ID_DLG_Nvod == UI_APP_API_GetReserveListPreState())
	{
		reserve_Type = APP_ReserveType_Nvod_e;  //Nvod预约
	}
	else
	{
		reserve_Type = APP_ReserveType_EpgReserve_e; //Epg预约
	}
	*/
	APP_EPG_API_StopGetEpg();
	APP_Com_API_SetFullScreen();
	ReserveList_IPI_ShowPage(APP_ReserveType_EpgReserve_e);
}
void UI_APP_ReserveList_End(void)
{

}
void UI_APP_ReserveList_Key(BU32 key)
{
	BU32 index,remainder;
	GUI_ENC_API_GetValue(ID_LBX_ReserveList, WDT_LBX_SV_INDEX, &index, 0);
	switch(key)
	{
	case GUI_VK_EXIT:
		if(UI_APP_API_GetbSaveFlag() == 1)
		{
			//跳转到保存节目
			GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		}
		break;	
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		if(UI_APP_API_GetbSaveFlag() == 1)
		{
			//跳转到保存节目
			GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
			GUI_ENC_API_SwitchState(UI_APP_API_GetReserveListPreState(),1);
		}
		else
		{
			GUI_ENC_API_SwitchState(UI_APP_API_GetReserveListPreState(),1);
		}
		break;
	case GUI_VK_RED:
		if(g_reserve_allCount != 0)
		{
			GUI_ENC_API_SwitchState(ID_DLG_ReserveInfo, 1);
		}	
		break;
#if 0
	case GUI_VK_LEFT:
		if(reserve_Type <= 3)
		{ 
			reserve_Type = 4;
		}
		else
		{
			reserve_Type--;
		}
		APP_Printf("----------reserve_Type = %d--------------\n",reserve_Type);
		ReserveList_IPI_ShowPage(reserve_Type);
		break;
	case GUI_VK_RIGHT:
		if(reserve_Type >= 4)
		{
			reserve_Type = 3;
		}
		else
		{
			reserve_Type++;
		}
		APP_Printf("----------reserve_Type = %d--------------\n",reserve_Type);
		ReserveList_IPI_ShowPage(reserve_Type);
		break;
#endif
	case GUI_VK_P_DOWN:
		if(g_reserve_allCount == 0)
		{
			return;
		}
		remainder = g_reserve_allCount%RESERVE_NUM_IN_PAGE;
		if(!remainder)
		{
			remainder = RESERVE_NUM_IN_PAGE;
		}
		if(index < RESERVE_NUM_IN_PAGE)
		{
			if(index < remainder)
			{
				index += g_reserve_allCount - remainder;
			}
			else
			{
				index = g_reserve_allCount-1;
			}
		}
		else
		{
			index -= RESERVE_NUM_IN_PAGE;
		}
		GUI_ENC_API_SetValue(ID_LBX_ReserveList, WDT_LBX_SV_INDEX, index, 0);
		break;
	case GUI_VK_P_UP:	
		if(g_reserve_allCount == 0)
		{
			return;
		}
		remainder = g_reserve_allCount%RESERVE_NUM_IN_PAGE;
		if(remainder == 0)
		{
			remainder = RESERVE_NUM_IN_PAGE;
		}
		index += RESERVE_NUM_IN_PAGE;
		if(index >= g_reserve_allCount)
		{
			if(index >= g_reserve_allCount - remainder + RESERVE_NUM_IN_PAGE)//最后一页
			{
				index = index%RESERVE_NUM_IN_PAGE;
			}
			else	//倒数第二页
			{
				index = g_reserve_allCount - 1;
			}
		}
		GUI_ENC_API_SetValue(ID_LBX_ReserveList, WDT_LBX_SV_INDEX, index, 0);
		break;
	}
}

//预约列表管理提示框
void UI_APP_ReserveInfo_Enter(void)
{
}
void UI_APP_ReserveInfo_End(void)
{
}
void UI_APP_ReserveInfo_Key(BU32 key)
{
	BU32 index;
	switch(key)
		{
		case GUI_VK_RECALL:
		case GUI_VK_MENU:
			GUI_ENC_API_SwitchState(ID_DLG_ReserveList, 1);
			break;
		case GUI_VK_SELECT:
			//设置保存数据标记
			UI_APP_API_SetbSaveFlag(1);
			GUI_ENC_API_GetValue(ID_LBX_ReserveList,WDT_LBX_SV_INDEX,&index,0);
			APP_EPG_API_CancelReserveType(index,reserve_Type);
			GUI_ENC_API_SwitchState(ID_DLG_ReserveList, 1);
			if(index >= APP_EPG_API_GetReserveAmount() && index !=0)
			{
				GUI_ENC_API_SetValue(ID_LBX_ReserveList,WDT_LBX_SV_INDEX,index-1,0);
			}
			break;
		case GUI_VK_EXIT:
			if(UI_APP_API_GetbSaveFlag() == 1)
			{
				//跳转到保存节目
				GUI_ENC_API_SwitchState(ID_DLG_SysInfo, 1);
				GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			}
			break;
		default:
			break;
		}
}
