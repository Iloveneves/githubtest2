#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

#define PAGE_CHN_NUM  8
#define CHN_NAME_LEN 24

static BU08 g_brdcastlist_chnName[PAGE_CHN_NUM][CHN_NAME_LEN];
static PU08 g_brdcastlist_s[PAGE_CHN_NUM] = 
{
	g_brdcastlist_chnName[0],
	g_brdcastlist_chnName[1],
	g_brdcastlist_chnName[2],
	g_brdcastlist_chnName[3],
	g_brdcastlist_chnName[4],
	g_brdcastlist_chnName[5],
	g_brdcastlist_chnName[6],
	g_brdcastlist_chnName[7],
};

static BU32 g_brdcastList_curPage = 0;
static BU32 g_brdcastList_pageNum = 0;
static BU32 g_brdcastList_rest= 0;

void BrdcastList_IPI_SetPageParam(void)
{
	BU32 broadcastNum;
	broadcastNum = APP_DBA_API_GetBroadCastNum();
	g_brdcastList_pageNum = (broadcastNum +  PAGE_CHN_NUM -1) / PAGE_CHN_NUM;	
	g_brdcastList_rest = broadcastNum % PAGE_CHN_NUM;
	if(g_brdcastList_rest == 0)
	{
		g_brdcastList_rest = PAGE_CHN_NUM;
	}
	g_brdcastList_curPage = 0;
	if(broadcastNum == 0)
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_BroadcastList, WDT_SBR_SV_MAX, broadcastNum, 0);}
	else
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_BroadcastList, WDT_SBR_SV_MAX, broadcastNum-1, 0);}
	GUI_ENC_API_SetValueUnPaint(ID_SBR_BroadcastList, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_BroadcastList, WDT_SBR_SV_VALUE, 0, 0);
}

void BrdcastList_IPI_ShowCurPage(void)
{
	BroadCast_ChnInfo_S broadcastInfo;
	BU32 broadcastNo,i;
	BU32 pageNum;
	if(g_brdcastList_pageNum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_BroadcastList, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(g_brdcastList_curPage >= g_brdcastList_pageNum)
	{
		g_brdcastList_curPage = 0;
	}
	
	if(g_brdcastList_curPage+1 == g_brdcastList_pageNum)
	{	
		pageNum = g_brdcastList_rest;
	}
	else
	{
		pageNum = PAGE_CHN_NUM;
	}
	
	for(i=0; i<pageNum; i++)
	{	
		broadcastNo = i+g_brdcastList_curPage*PAGE_CHN_NUM;
		if(APP_FAIL == APP_DBA_API_GetBroadcastInfo(broadcastNo,&broadcastInfo))
		{
			GUI_ENC_API_SetValue(ID_LBP_BroadcastList, WDT_LBP_SV_DATA, (BU32)g_brdcastlist_s, i);
			return;
		}
		broadcastInfo.m_ChnName[CHN_NAME_LEN-7]='\0';
		sprintf(g_brdcastlist_s[i],"%03d  %-16s",broadcastNo+1,broadcastInfo.m_ChnName);
	}
	GUI_ENC_API_SetValue(ID_LBP_BroadcastList, WDT_LBP_SV_DATA, g_brdcastlist_s, pageNum);
}

void UI_APP_BrdcastList_Enter(void)
{
	BU32 broadcastNum;
	BroadCast_ChnInfo_S broadcastInfo;
	BU32 hwnd;
	GUI_Rect_s rect;
	g_brdcastList_curPage = 0;
	g_brdcastList_pageNum = 0;
	g_brdcastList_rest= 0;
	hwnd = GUI_API_GetDlgItem(ID_VFM_BroadcastList);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);//播放节目
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_BroadcastList,ID_STA_event2_BroadcastList,ID_PBR_Epg_BroadcastList,260,1);

	GUI_ENC_API_SetValueUnPaint(ID_LBP_BroadcastList, WDT_LBP_SV_INDEX, 0, 0);
	BrdcastList_IPI_SetPageParam();
	BrdcastList_IPI_ShowCurPage();
}

void UI_APP_BrdcastList_End(void)
{

}

void UI_APP_BrdcastList_Key(BU32 key)
{
	BU32 broadcastNo;
	BU32 index;
	BU32 prestate;
	BroadCast_ChnInfo_S broadcastInfo;

	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		GUI_ENC_API_GetValue(ID_LBP_BroadcastList,WDT_LBP_SV_INDEX,&index,0);
		broadcastNo = (index+g_brdcastList_curPage*PAGE_CHN_NUM);
		GUI_ENC_API_SetValue(ID_SBR_BroadcastList, WDT_SBR_SV_VALUE,broadcastNo, 0);
		break;
	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_LBP_BroadcastList,WDT_LBP_SV_INDEX,&index,0);
		broadcastNo = (index+g_brdcastList_curPage*PAGE_CHN_NUM);
		UI_APP_API_SetCurBrdcastNo(broadcastNo);
		GUI_ENC_API_SwitchState(ID_DLG_Null,1);
		break;
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(UI_APP_API_GetBrdcastListPreState(),1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;
	}
}
void UI_APP_BrdcastList_Change(void)
{
	BS32 state;

	GUI_ENC_API_GetValue(ID_LBP_Prog_ProgList, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_brdcastList_curPage++;
			break;
		case -1:
			if(g_brdcastList_curPage == 0)
				g_brdcastList_curPage = g_brdcastList_pageNum -1;
			else
				g_brdcastList_curPage--;
			
			if(g_brdcastList_curPage == g_brdcastList_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_BroadcastList, WDT_LBP_SV_INDEX, PAGE_CHN_NUM-1, 0);
			}
			break;
		default:
			break;
	}
	BrdcastList_IPI_ShowCurPage();//显示当前页
}

