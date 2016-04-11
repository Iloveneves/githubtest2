#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define SORT_PAGE_CHN_NUM  8
#define SORT_CHN_NAME_LEN 22
#define SORT_LIST  0
#define CHN_LIST_OF_SORT 1

static BU08 g_chnSort_chnName[SORT_PAGE_CHN_NUM][SORT_CHN_NAME_LEN];
static CHN_LIST_S g_chn_sort_s[SORT_PAGE_CHN_NUM] = 
{
   { g_chnSort_chnName[0],0,0},
   { g_chnSort_chnName[1],0,0},
   { g_chnSort_chnName[2],0,0},
   { g_chnSort_chnName[3],0,0},
   { g_chnSort_chnName[4],0,0},
   { g_chnSort_chnName[5],0,0},
   { g_chnSort_chnName[6],0,0},
   { g_chnSort_chnName[7],0,0},

};
static BU32 g_chnSort_curPage = 0;
static BU32 g_chnSort_pageNum = 0;
static BU32 g_chnSort_rest= 0;
static BU32 g_chnSort_curSortIndex = 0;
static BU08 g_chnSort_sortName[APP_CHN_NAME_LEN];
static BU08 g_chnSort_listFlag = SORT_LIST;

/*----------------------------------------------------------------------------
描述：每次创建列表成功后调用
-----------------------------------------------------------------------------*/
void ChnSort_IPI_SetPageParam(BU32 amount,BU32 curIndex)
{
	g_chnSort_pageNum = (amount +  SORT_PAGE_CHN_NUM -1) / SORT_PAGE_CHN_NUM;	
	g_chnSort_rest = amount % SORT_PAGE_CHN_NUM;
	g_chnSort_curPage = curIndex /SORT_PAGE_CHN_NUM;
	if(g_chnSort_rest == 0)
	{
		g_chnSort_rest = SORT_PAGE_CHN_NUM;
	}

	if(amount == 0)
	{
		GUI_ENC_API_SetValueUnPaint(ID_SBR_SortProgram, WDT_SBR_SV_MAX, 0, 0);
	}
	else
	{
		GUI_ENC_API_SetValueUnPaint(ID_SBR_SortProgram, WDT_SBR_SV_MAX, (BU32)amount-1, 0);
	}
	GUI_ENC_API_SetValueUnPaint(ID_SBR_SortProgram, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_SortProgram, WDT_SBR_SV_VALUE, curIndex, 0);
	
}
void ChnSort_IPI_ShowCurPage(void)
{
	ChnDataBase_ChnInfo_S ChnInfo;
	BU16 Index,i;
	BU16 pageNum=0;
	BU08 chnName[APP_CHN_NAME_LEN];
	if(g_chnSort_pageNum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_SortProgram, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(g_chnSort_curPage >= g_chnSort_pageNum)
	{
		g_chnSort_curPage = 0;
	}
	
	if(g_chnSort_curPage+1 == g_chnSort_pageNum)
	{	
		pageNum = g_chnSort_rest;
	}
	else
	{
		pageNum = SORT_PAGE_CHN_NUM;
	}
	
	for(i=0; i<pageNum; i++)
	{	
		Index = i+g_chnSort_curPage*SORT_PAGE_CHN_NUM;

		if(g_chnSort_listFlag == SORT_LIST)
		{
			APP_ChnDatabase_API_GetSortName(Index,chnName);
			chnName[SORT_CHN_NAME_LEN-8]='\0';
			//sprintf(g_chn_sort_s[i].chnName,"%03d  %s",Index + 1,chnName);
			sprintf(g_chn_sort_s[i].chnName,"%s", chnName);
			g_chn_sort_s[i].fav= 0;
			g_chn_sort_s[i].lock= 0;

		}
		else
		{
			APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), Index, &ChnInfo);
			//APP_ChnDatabase_API_GetSortChnInfo(g_chnSort_curSortIndex, Index, &ChnInfo);
			ChnInfo.m_ChnName[SORT_CHN_NAME_LEN-8]='\0';
			if(ChnInfo.m_Ca)
			{
				sprintf(g_chn_sort_s[i].chnName,"￥%03d  %-14s",Index + APP_ChnDatabase_API_GetChn0Flag(), ChnInfo.m_ChnName);
			}
			else
			{
				sprintf(g_chn_sort_s[i].chnName,"  %03d  %-14s",Index + APP_ChnDatabase_API_GetChn0Flag(), ChnInfo.m_ChnName);
			}
			g_chn_sort_s[i].fav= ChnInfo.m_Fav;
			g_chn_sort_s[i].lock= ChnInfo.m_Lock;


		}
	}

	GUI_ENC_API_SetValue(ID_LBP_SortProgram, WDT_LBP_SV_DATA, (BU32)g_chn_sort_s, pageNum);
}
void ChnSort_IPI_PlayChn(BU32 sortIndex,BU32 chnIndex)
{
	BU32 serviceID;
	ChnDataBase_ChnInfo_S chnInfo;
	
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), chnIndex, 1);

#if 0
	APP_ChnDatabase_API_GetSortChnInfo(sortIndex, chnIndex, &chnInfo);
	serviceID = chnInfo.m_ProgId;
	APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
	
	APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), serviceID,&chnInfo);
	APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
	APP_ChnDatabase_API_SetChn0Flag(1);
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), chnIndex, 1);
#endif

}
void ChnSort_IPI_PlayChn_ex(BU32 sortIndex,BU32 chnIndex)
{
	BU32 serviceID;
	ChnDataBase_ChnInfo_S chnInfo;

	APP_ChnDatabase_API_GetSortChnInfo(sortIndex, chnIndex, &chnInfo);
	serviceID = chnInfo.m_ProgId;
	//APP_ChnDatabase_API_CreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), serviceID,&chnInfo);
//	APP_ChnDatabase_API_GetChnInfoByService(serviceID,&chnInfo);
	APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
	APP_ChnDatabase_API_SetChn0Flag(1);
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
	//UI_APP_API_CheckPlay_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),0);//播放节目
}

void ChnSort_IPI_ChangeSort(BU32 key)
{
	BU32 chnNum;
	BU08 flag = 0;//是否需要进行检查上一个分类的节目是否再下一个分类的标志
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 serviceID;
	BU32 curIndex = 0;

	if(key == GUI_VK_LEFT)
	{
		if(g_chnSort_curSortIndex == 0)
			g_chnSort_curSortIndex = APP_ChnDatabase_API_GetSortCount()-1;
		else 
			g_chnSort_curSortIndex --;
	}
	else if(key == GUI_VK_RIGHT)
	{
		if(g_chnSort_curSortIndex == APP_ChnDatabase_API_GetSortCount()-1)
			g_chnSort_curSortIndex = 0;
		else 
			g_chnSort_curSortIndex ++;
	}
	APP_PRINT_DEBUG("g_chnSort_curSortIndex = %d\n", g_chnSort_curSortIndex);
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
	APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, g_chnSort_curSortIndex);
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
//	chnNum = APP_ChnDatabase_API_GetSortChnCount(g_chnSort_curSortIndex);
	APP_ChnDatabase_API_GetSortName(g_chnSort_curSortIndex,g_chnSort_sortName);
	g_chnSort_sortName[16]=0;
	GUI_ENC_API_SetValue(ID_STA_Sort_SortProgram,WDT_STA_SV_STRING,(BU32)g_chnSort_sortName,0);

	APP_PRINT_DEBUG("chnNum = %d\n", chnNum);
	if(chnNum)
	{
		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
		if (flag)
		{
			if (APP_FAIL == APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), serviceID,&chnInfo))
			{
				APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),  0, &chnInfo);
				APP_ChnDatabase_API_GetChnInfoByService(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e, chnInfo.m_ProgId, &chnInfo);
				APP_ChnDatabase_API_SetTypeAllCurChnNO(chnInfo.m_ChnIndex);

				APP_ChnDatabase_API_SetCurChnNO(0);
				UI_APP_API_PlayChn(0, 0, 1);
				//ChnSort_IPI_PlayChn(g_chnSort_curSortIndex,0);			
			}
			else
			{
				APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
				curIndex = chnInfo.m_ChnIndex;
				UI_APP_API_PlayChn(chnInfo.m_ChnIndex, chnInfo.m_ChnIndex, 1);
			//	ChnSort_IPI_PlayChn(g_chnSort_curSortIndex, chnInfo.m_ChnIndex);	
			}
		}
		else
		{
			APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),  0, &chnInfo);
			APP_ChnDatabase_API_GetChnInfoByService(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e, chnInfo.m_ProgId, &chnInfo);
			APP_ChnDatabase_API_SetTypeAllCurChnNO(chnInfo.m_ChnIndex);
			APP_ChnDatabase_API_SetCurChnNO(0);
			UI_APP_API_PlayChn(0, 0, 1);
			//ChnSort_IPI_PlayChn(g_chnSort_curSortIndex,0);
		}
	}
	else
	{
		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
		APP_Com_API_StopProg(1);
		APP_Com_API_ShowLED("C000");
		APP_Com_API_SetEcmNULL();
	}

	ChnSort_IPI_SetPageParam(chnNum,curIndex);
	GUI_ENC_API_SetValueUnPaint(ID_LBP_SortProgram,WDT_LBP_SV_INDEX,curIndex % SORT_PAGE_CHN_NUM,0);
	ChnSort_IPI_ShowCurPage();
	GUI_ENC_API_Update(ID_LBP_SortProgram, 1);

}

void ChnSort_IPI_ShowHelp(void)
{
	if(g_chnSort_listFlag == SORT_LIST)
	{
		GUI_ENC_API_SetValue(ID_STA_MenuExit_SortProgram,WDT_STA_SV_DATAID,RS_ExitMenu,0);
		GUI_ENC_API_SetValue(ID_STA_View_SortProgram,WDT_STA_SV_DATAID,RS_EnterMenu,0);

		GUI_ENC_API_SetValue(ID_STA_Sort_SortProgram, WDT_STA_SV_DATAID,RS_SortList,0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_MenuExit_SortProgram,WDT_STA_SV_DATAID,RS_ReturnInfo,0);
		GUI_ENC_API_SetValue(ID_STA_MenuExit_SortProgram,WDT_STA_SV_DATAID,RS_WatchTV,0);

	}
}

void UI_APP_ChnSort_Enter(void)
{
	BU32 chnNum;
	BU32 curIndex;
	BU32 preState;
	BU32 hwnd;
	GUI_Rect_s rect;
	hwnd = GUI_API_GetDlgItem(ID_VFM_SortProgram);
	GUI_API_GetWindowRect(hwnd,&rect);
	APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_SortProgram,ID_STA_event2_SortProgram,ID_PBR_Epg_SortProgram,260,1);
	preState = GUI_ENC_API_GetPrevState ();
	if(preState == ID_DLG_ChnLock || preState == ID_DLG_Unlock)
	{
		return;
	}
	g_chnSort_curSortIndex = 0;
	g_chnSort_listFlag = SORT_LIST;
	GUI_ENC_API_SetValue(ID_STA_Sort_SortProgram,WDT_STA_SV_DATAID,RS_Null,0);
	chnNum = APP_ChnDatabase_API_GetSortCount();
	//chnNum = 13;
	if(chnNum == 0)
	{
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_SortProgram, WS_DISABLE);		
		GUI_ENC_API_SetVisible(ID_STA_Note_SortProgram, WS_DISABLE);	
		GUI_ENC_API_SetVisible(ID_LBP_SortProgram, WS_DISABLE);	
		GUI_ENC_API_SetVisible(ID_SBR_SortProgram, WS_DISABLE);	

	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_SortProgram, WS_DISABLE);		
		GUI_ENC_API_SetVisible(ID_STA_Note_SortProgram, WS_DISABLE);		
		GUI_ENC_API_SetVisible(ID_LBP_SortProgram, WS_VISIBLE);	
		GUI_ENC_API_SetVisible(ID_SBR_SortProgram, WS_VISIBLE);	
		
		ChnSort_IPI_SetPageParam(chnNum,g_chnSort_curSortIndex);
		ChnSort_IPI_ShowCurPage();
		GUI_ENC_API_SetValue(ID_LBP_SortProgram, WDT_LBP_SV_INDEX, 0, 0);
	}
	ChnSort_IPI_ShowHelp();

}
void UI_APP_ChnSort_End(void)
{
}

void UI_APP_ChnSort_Key(BU32 key)
{
	BU32 curIndex;
	BU32 index;
	BU32 keyState;
	BU32 serviceID;
	BU32 chnNum;
	ChnDataBase_ChnInfo_S chnInfo;


	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		//if(APP_ChnDatabase_API_GetSortCount() == 0)return;
		if (APP_ChnDatabase_API_GetCurTypeChnNum() == 0) return;
		
		GUI_ENC_API_GetValue(ID_LBP_SortProgram,WDT_LBP_SV_INDEX,&index,0);
		curIndex = (index+g_chnSort_curPage*SORT_PAGE_CHN_NUM);
		
		if(g_chnSort_listFlag == SORT_LIST)
		{
			g_chnSort_curSortIndex = curIndex;
			GUI_ENC_API_SetValue(ID_SBR_SortProgram, WDT_SBR_SV_VALUE, (BU32)curIndex, 0);
		}
		else
		{
			GUI_ENC_API_GetValue(ID_LBP_SortProgram,WDT_LBP_SV_KEYSTATE,&keyState,0);	 
			APP_ChnDatabase_API_SetCurChnNO(curIndex);
			APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),  curIndex, &chnInfo);
			APP_ChnDatabase_API_GetChnInfoByService(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e, chnInfo.m_ProgId, &chnInfo);
			APP_ChnDatabase_API_SetTypeAllCurChnNO(chnInfo.m_ChnIndex);
			if(!keyState)//连续键处理
			{
			   UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_SortProgram,ID_STA_event2_SortProgram,ID_PBR_Epg_SortProgram,260,1);
			   //ChnSort_IPI_PlayChn(g_chnSort_curSortIndex,curIndex);
		
			   UI_APP_API_PlayChn(curIndex, curIndex, 1);
			}
			GUI_ENC_API_SetValue(ID_SBR_SortProgram, WDT_SBR_SV_VALUE, (BU32)curIndex, 0);
		}
		break;
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		//if(APP_ChnDatabase_API_GetSortCount() == 0)return;
		if(g_chnSort_listFlag == SORT_LIST)return;
		ChnSort_IPI_ChangeSort(key);
		break;
	case GUI_VK_SELECT:
		//if(APP_ChnDatabase_API_GetSortCount() == 0)return;
		
		GUI_ENC_API_GetValue(ID_LBP_SortProgram,WDT_LBP_SV_INDEX,&index,0);
		curIndex = (index+g_chnSort_curPage*SORT_PAGE_CHN_NUM);
		
		if(g_chnSort_listFlag == SORT_LIST)
		{
			g_chnSort_curSortIndex = curIndex;
			g_chnSort_listFlag = CHN_LIST_OF_SORT;
			ChnSort_IPI_ChangeSort(GUI_VK_SELECT);
			#if 0
			chnNum = APP_ChnDatabase_API_GetSortChnCount(curIndex);
			if(chnNum)
			{
				g_chnSort_listFlag = CHN_LIST_OF_SORT;
				APP_ChnDatabase_API_GetSortName(curIndex,g_chnSort_sortName);
				g_chnSort_sortName[16]=0;
				GUI_ENC_API_SetValue(ID_STA_Sort_SortProgram,WDT_STA_SV_STRING,(BU32)g_chnSort_sortName,0);
				
				ChnSort_IPI_SetPageParam(chnNum,0);
				GUI_ENC_API_SetValueUnPaint(ID_LBP_SortProgram,WDT_LBP_SV_INDEX,0,0);
				ChnSort_IPI_ShowCurPage();
				GUI_ENC_API_Update(ID_LBP_SortProgram, 1);
				ChnSort_IPI_PlayChn(g_chnSort_curSortIndex,0);
			}
			else
			{
				GUI_ENC_API_SwitchState(ID_DLG_SortInfo, 1);
			}
			#endif
		}
		else
		{
			g_chnSort_listFlag = SORT_LIST;
			//if(APP_ChnDatabase_API_GetSortChnCount(g_chnSort_curSortIndex))
			{
			    UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_SortProgram,ID_STA_event2_SortProgram,ID_PBR_Epg_SortProgram,260,1);
				APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
				if (APP_ChnDatabase_API_GetCurTypeChnNum())
				{
					GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
				}
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
				//ChnSort_IPI_PlayChn_ex(g_chnSort_curSortIndex,curIndex);
			}
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		}
		ChnSort_IPI_ShowHelp();

		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
		if (APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
		}
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		break;
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		if(g_chnSort_listFlag == SORT_LIST)
		{
			GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);
			APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		}
		else
		{
			g_chnSort_listFlag = SORT_LIST;
			GUI_ENC_API_SetValue(ID_STA_Sort_SortProgram,WDT_STA_SV_DATAID,RS_Null,0);
			chnNum = APP_ChnDatabase_API_GetSortCount();
			ChnSort_IPI_SetPageParam(chnNum,g_chnSort_curSortIndex);
			GUI_ENC_API_SetValueUnPaint(ID_LBP_SortProgram,WDT_LBP_SV_INDEX,g_chnSort_curSortIndex%SORT_PAGE_CHN_NUM,0);
			ChnSort_IPI_ShowCurPage();
			APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
			if (APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
			}
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//GUI_ENC_API_SetValue(ID_STA_Sort_SortProgram,WDT_STA_SV_DATAID,RS_AllProg,0);
		}
		ChnSort_IPI_ShowHelp();
		break;
	default:
		break;		
	}
}
void UI_APP_ChnSort_Change(void)
{
	BS32 state;

	GUI_ENC_API_GetValue(ID_LBP_SortProgram, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_chnSort_curPage++;
			ChnSort_IPI_ShowCurPage();//显示当前页
			break;
		case -1:
			if(g_chnSort_curPage == 0)
				g_chnSort_curPage = g_chnSort_pageNum -1;
			else
				g_chnSort_curPage--;
			
			if(g_chnSort_curPage == g_chnSort_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_SortProgram, WDT_LBP_SV_INDEX, SORT_PAGE_CHN_NUM-1, 0);
			}
			ChnSort_IPI_ShowCurPage();//显示当前页
			break;
		default:
			break;
	}
}
void UI_APP_ChnSort_Timer(void)
{
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_SortProgram,ID_STA_event2_SortProgram,ID_PBR_Epg_SortProgram,260,1);

}

