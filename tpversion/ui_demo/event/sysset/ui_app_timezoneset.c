#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"


extern BU08 g_ui_app_str[13][80];
//时区设置
static BU08 *g_sys_timezone_p[2]=
{
	g_ui_app_str[6],
	g_ui_app_str[7]
};
static BU08 time_zone = 0;
extern BU32 g_Date;
extern BU32 g_Hour;

void UI_APP_TimeZoneSet_Enter(void)
{
	APP_Date_S now;
	BU32 hwnd;
	GUI_Rect_s rect;
	BU32 time_zone_type;
	hwnd = GUI_API_GetDlgItem(ID_VFM_TimeZoneSet);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_TimeZoneSet,ID_STA_Event2_TimeZoneSet,ID_PBR_Epg_TimeZoneSet,260,1);
	APP_SYS_API_GetSysInfo(APP_SYS_TimeZone_e, &time_zone_type);
	GUI_ENC_API_SetValue(ID_CBX_TimeZone_TimeZoneSet,WDT_CBX_SV_INDEX,time_zone_type,0);
	APP_SYS_API_GetLocalDateAndTime(&now);
	sprintf(g_sys_timezone_p[0],"%04d.%02d.%02d",now.m_year,now.m_month,now.m_day);
	GUI_ENC_API_SetValue(ID_CBX_CurDate_TimeZoneSet,WDT_CBX_SV_STRING,(BU32)g_sys_timezone_p[0],0);
	sprintf(g_sys_timezone_p[1],"%02d:%02d",now.m_hour,now.m_min);
	GUI_ENC_API_SetValue(ID_CBX_CurTime_TimeZoneSet,WDT_CBX_SV_STRING,(BU32)g_sys_timezone_p[1],0);

}
void UI_APP_TimeZoneSet_End(void)
{

}
void UI_APP_TimeZoneSet_Key(BU32 key)
{
	BU32 index;
	APP_Date_S now;
	switch(key)
	{
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if(GUI_ENC_API_IsFocus(ID_CBX_TimeZone_TimeZoneSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_TimeZone_TimeZoneSet,WDT_CBX_SV_INDEX,&index,0);
			if((index == 1))//北京时间GMT+08.00
			{
				//time_zone = 1;
				if(g_Hour < 8)
				{
					g_Date--;
					g_Hour = 24 + g_Hour - 8;
				}
				else
				{
					g_Hour-=8;
				}
				APP_SYS_API_SetSysInfo(APP_SYS_TimeZone_e, 1);
			}
			else if((index == 0))//标准时间GMT
			{
				//time_zone = 0;
				if(g_Hour >= 16)
				{
					g_Date++;
					g_Hour = g_Hour + 8 -24;
				}
				else
				{
					g_Hour+=8;
				}
				APP_SYS_API_SetSysInfo(APP_SYS_TimeZone_e, 0);
			}		
			APP_IPI_InitDbaV2();
			UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Event1_TimeZoneSet,ID_STA_Event2_TimeZoneSet,ID_PBR_Epg_TimeZoneSet,260,1);
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWSYSTIME, 0, 0);
			//APP_SYS_API_SetSysInfo(APP_SYS_TIMEOUT_e,index);
		}
	APP_SYS_API_GetLocalDateAndTime(&now);
	sprintf(g_sys_timezone_p[0],"%04d.%02d.%02d",now.m_year,now.m_month,now.m_day);
	GUI_ENC_API_SetValue(ID_CBX_CurDate_TimeZoneSet,WDT_CBX_SV_STRING,(BU32)g_sys_timezone_p[0],0);
	sprintf(g_sys_timezone_p[1]," %02d:%02d",now.m_hour,now.m_min);
	GUI_ENC_API_SetValue(ID_CBX_CurTime_TimeZoneSet,WDT_CBX_SV_STRING,(BU32)g_sys_timezone_p[1],0);
		break;
	case GUI_VK_SELECT:
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		APP_SYS_API_SaveSysInfo();
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		APP_SYS_API_SaveSysInfo();
		break;
	default:
		break;
	}
}

