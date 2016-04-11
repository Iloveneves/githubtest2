#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define PAGE_CHN_NUM  8
#define CHN_NAME_LEN 24

static BU32 g_chnList_curSortIndex = 0;
static BU08 g_chnList_sortName[APP_CHN_NAME_LEN];
static BU08 g_chnList_chnName[PAGE_CHN_NUM][CHN_NAME_LEN];
static CHN_LIST_S g_chn_list_s[PAGE_CHN_NUM] = 
{
	{g_chnList_chnName[0],0,0},
	{g_chnList_chnName[1],0,0},
	{g_chnList_chnName[2],0,0},
	{g_chnList_chnName[3],0,0},
	{g_chnList_chnName[4],0,0},
	{g_chnList_chnName[5],0,0},
	{g_chnList_chnName[6],0,0},
	{g_chnList_chnName[7],0,0},
};

static BU32 g_chnList_curPage = 0;
static BU32 g_chnList_pageNum = 0;
static BU32 g_chnList_rest= 0;
//BU16 g_chnList_chnIndex;

/*----------------------------------------------------------------------------
描述：每次创建列表成功后调用
-----------------------------------------------------------------------------*/
void ChnList_IPI_SetPageParam(void)
{
	BU16 chnNum,curChn;
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	g_chnList_pageNum = (chnNum +  PAGE_CHN_NUM -1) / PAGE_CHN_NUM;	
	g_chnList_rest = chnNum % PAGE_CHN_NUM;
	g_chnList_curPage = curChn / PAGE_CHN_NUM;
	if(g_chnList_rest == 0)
	{
		g_chnList_rest = PAGE_CHN_NUM;
	}

	if(chnNum == 0)
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_ProgList, WDT_SBR_SV_MAX, (BU32)chnNum, 0);}
	else
		{GUI_ENC_API_SetValueUnPaint(ID_SBR_ProgList, WDT_SBR_SV_MAX, (BU32)chnNum-1, 0);}
	GUI_ENC_API_SetValueUnPaint(ID_SBR_ProgList, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_ProgList, WDT_SBR_SV_VALUE, (BU32)curChn, 0);
	
}

void ChnList_IPI_ShowCurPage(void)
{
	ChnDataBase_ChnInfo_S ChnInfo;
	BU16 ChnNo,i;
	BU16 pageNum;
	if(g_chnList_pageNum == 0)
	{
	GUI_ENC_API_SetValue(ID_LBP_Prog_ProgList, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(g_chnList_curPage >= g_chnList_pageNum)
	{
		g_chnList_curPage = 0;
	}
	
	if(g_chnList_curPage+1 == g_chnList_pageNum)
	{	
		pageNum = g_chnList_rest;
	}
	else
	{
		pageNum = PAGE_CHN_NUM;
	}
	
	for(i=0; i<pageNum; i++)
	{	
		ChnNo = i+g_chnList_curPage*PAGE_CHN_NUM;
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ChnNo,&ChnInfo))
		{
			GUI_ENC_API_SetValue(ID_LBP_Prog_ProgList, WDT_LBP_SV_DATA, (BU32)g_chn_list_s, i);
			return;
		}
		ChnInfo.m_ChnName[CHN_NAME_LEN-10]='\0';
		if(ChnInfo.m_Ca)
		{
			sprintf(g_chn_list_s[i].chnName,"￥%03d  %-14s",ChnNo + APP_ChnDatabase_API_GetChn0Flag(),ChnInfo.m_ChnName);

		}
		else
		{
			sprintf(g_chn_list_s[i].chnName,"  %03d  %-14s",ChnNo + APP_ChnDatabase_API_GetChn0Flag(),ChnInfo.m_ChnName);
		}
		g_chn_list_s[i].fav= ChnInfo.m_Fav;
		g_chn_list_s[i].lock= ChnInfo.m_Lock;
	}
	GUI_ENC_API_SetValue(ID_LBP_Prog_ProgList, WDT_LBP_SV_DATA, (BU32)g_chn_list_s, pageNum);
}


void ChnList_IPI_ChangeSort()
{
	BU32 chnNum;
	BU08 flag = 0;//是否需要进行检查上一个分类的节目是否再下一个分类的标志
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 serviceID;
	BU32 curIndex = 0;
	BU32 vol = 0;
	BS32 offset = 0;
	
	APP_PRINT_DEBUG("g_chnList_curSortIndex = %d\n", g_chnList_curSortIndex);
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
			vol = chnInfo.m_AudioVolume;
			offset = chnInfo.m_AudioOffset;
			APP_PRINT_DEBUG("serviceID = %d\n", serviceID);
		}
	}
	APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), g_chnList_curSortIndex);
	if (g_chnList_curSortIndex == 0)
	{
		sprintf(g_chnList_sortName,"所有节目");
	}
	else
	{
		sprintf(g_chnList_sortName,"喜爱节目");
	}
	//APP_ChnDatabase_API_GetSortName(g_chnList_curSortIndex,g_chnList_sortName);
	g_chnList_sortName[16]=0;
	GUI_ENC_API_SetValue(ID_STA_ProgList,WDT_STA_SV_STRING,(BU32)g_chnList_sortName,0);

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
				chnInfo.m_AudioVolume = vol;
				chnInfo.m_AudioOffset = offset;
				APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), chnInfo.m_ChnIndex, &chnInfo);
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
		if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
			APP_Com_API_ShowLED("C000");
		else
			APP_Com_API_ShowLED("R000");
		APP_Com_API_SetEcmNULL();
	}
	GUI_ENC_API_SetValueUnPaint(ID_LBP_Prog_ProgList, WDT_LBP_SV_INDEX, (BU32)(APP_ChnDatabase_API_GetCurChnNO()%PAGE_CHN_NUM), 0);
	ChnList_IPI_SetPageParam();
	ChnList_IPI_ShowCurPage();

}

void UI_APP_ChnList_Enter(void)
{
	BU16 chnNum;
	BU32 hwnd;
	GUI_Rect_s rect;
	APP_CHN_AV_TYPE_E AvType = APP_CHN_AV_TYPE_TV_e;
	APP_CHN_GROUP_TYPE_E GroupType = APP_CHN_GROUP_TYPE_ALL_e;
	AvType = APP_ChnDatabase_API_GetCurChnType();
	GroupType = APP_ChnDatabase_API_GetCurGroupType();
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	Hisi_SetUpdateDlgID(ID_DLG_ProgList);
	if(AvType == APP_CHN_AV_TYPE_TV_e)
	{
		GUI_ENC_API_SetValue(ID_STA_ProgList_ProgList, WDT_STA_SV_DATAID,RS_TVList,0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_ProgList_ProgList, WDT_STA_SV_DATAID,RS_RadioList,0);
	}
	g_chnList_curSortIndex = GroupType;
	if (g_chnList_curSortIndex == 0)
	{
		sprintf(g_chnList_sortName,"所有节目");
	}
	else
	{
		sprintf(g_chnList_sortName,"喜爱节目");
	}
	//APP_ChnDatabase_API_GetSortName(GroupType, g_chnList_sortName);
	GUI_ENC_API_SetValue(ID_STA_ProgList, WDT_STA_SV_STRING, g_chnList_sortName, 0);
	
	/*
	if(GroupType == APP_CHN_GROUP_TYPE_ALL_e)
	{
		GUI_ENC_API_SetValue(ID_CBX_ProgList, WDT_CBX_SV_INDEX, 0, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_CBX_ProgList, WDT_CBX_SV_INDEX, 1, 0);
	}
	*/
	if(chnNum == 0)
	{
		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
		APP_Com_API_StopProg(1);
		APP_Com_API_ShowLED("C000");
		APP_Com_API_SetEcmNULL();
		GUI_ENC_API_SetValue(ID_LBP_Prog_ProgList, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_ProgList, WDT_SBR_SV_MAX, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_ProgList, WDT_SBR_SV_MIN, 0, 0);
		GUI_ENC_API_SetValue(ID_SBR_ProgList, WDT_SBR_SV_VALUE, 0, 0);
	}
	else
	{
		GUI_ENC_API_SetValueUnPaint(ID_LBP_Prog_ProgList, WDT_LBP_SV_INDEX, (BU32)(APP_ChnDatabase_API_GetCurChnNO()%PAGE_CHN_NUM), 0);
		ChnList_IPI_SetPageParam();
		ChnList_IPI_ShowCurPage();
		/*hwnd = GUI_API_GetDlgItem(ID_VFM_ProgList);
		GUI_API_GetWindowRect(hwnd,&rect);
		APP_Com_API_SetSmallScreen(rect.xpos,rect.ypos,rect.width,rect.height);//缩放视频
		*/
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);//播放节目
		UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_ProgList,ID_STA_event2_ProgList,ID_PBR_Epg_ProgList, 260, 1);
	}

}

void UI_APP_ChnList_End(void)
{

}

void UI_APP_ChnList_Key(BU32 key)
{
	BU16 curChn;
	BU32 chnNum;
	BU32 index;
	BU32 keyState;
	BU32 serviceID;
	ChnDataBase_ChnInfo_S chnInfo;
	BU08 str[4];
	BU32 prestate = ID_DLG_ProgManager;
	BU08 flag = 0;//是否需要进行检查上一个分类的节目是否再下一个分类的标志

	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)return;
		
		GUI_ENC_API_GetValue(ID_LBP_Prog_ProgList,WDT_LBP_SV_INDEX,&index,0);
		curChn = (BU16)(index+g_chnList_curPage*PAGE_CHN_NUM);
		APP_ChnDatabase_API_SetCurChnNO(curChn);
		APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),  curChn, &chnInfo);
		APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e, chnInfo.m_ProgId, &chnInfo);
		APP_ChnDatabase_API_SetTypeAllCurChnNO(chnInfo.m_ChnIndex);
		GUI_ENC_API_GetValue(ID_LBP_Prog_ProgList,WDT_LBP_SV_KEYSTATE,&keyState,0);
		if(!keyState)//连续键处理
		{
			UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_ProgList,ID_STA_event2_ProgList,ID_PBR_Epg_ProgList,260,1);
			UI_APP_API_PlayChn(curChn, curChn, 1);
		}
		GUI_ENC_API_SetValue(ID_SBR_ProgList, WDT_SBR_SV_VALUE, (BU32)curChn, 0);
		break;
	case GUI_VK_LEFT:
		if(g_chnList_curSortIndex == 0)
			g_chnList_curSortIndex = 1;
		else 
			g_chnList_curSortIndex = 0;

		ChnList_IPI_ChangeSort();
		break;
	case GUI_VK_RIGHT:

		if(g_chnList_curSortIndex == 1)
			g_chnList_curSortIndex = 0;
		else 
			g_chnList_curSortIndex = 1;

		ChnList_IPI_ChangeSort();
		break;
#if 0
		if (APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			flag = 1;
		
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), curChn, &chnInfo))
			{
				flag = 0;
			}
		serviceID = chnInfo.m_ProgId;
		if(index == 1)
		{
			APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
			//APP_ChnDatabase_API_CreatChnList(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), serviceID,&chnInfo);
			APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
			GUI_ENC_API_SetValue(ID_CBX_ProgList, WDT_CBX_SV_INDEX, 0, 0);
			
		}
		else
		{
			
			if (APP_FAIL == APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_FAV_e))
			{
				return;
			}
			else
			{
				if(chnInfo.m_Fav)
				{
					APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), serviceID,&chnInfo);
					APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
				}

				GUI_ENC_API_SetValue(ID_CBX_ProgList, WDT_CBX_SV_INDEX, 1, 0);
			
			}
		}
		APP_ChnDatabase_API_JudgeChn0();
/*
		if(curChn < APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			APP_ChnDatabase_API_SetCurChnNO(curChn);
		}
		else
		{
			curChn = APP_ChnDatabase_API_GetCurTypeChnNum();
			APP_ChnDatabase_API_SetCurChnNO(curChn);
		}
*/
		GUI_ENC_API_SetValueUnPaint(ID_LBP_Prog_ProgList, WDT_LBP_SV_INDEX, (BU32)(APP_ChnDatabase_API_GetCurChnNO()%PAGE_CHN_NUM), 0);
		ChnList_IPI_SetPageParam();
		ChnList_IPI_ShowCurPage();
		GUI_ENC_API_GetValue(ID_LBP_Prog_ProgList,WDT_LBP_SV_KEYSTATE,&keyState,0);
		if(!keyState)//连续键处理
		{
			if(APP_ChnDatabase_API_GetCurTypeChnNum() != 0)
			{
				UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_ProgList,ID_STA_event2_ProgList,ID_PBR_Epg_ProgList,260,1);
		//		UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO (),1);//播放节目
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			}
		}
#endif
		break;
	case GUI_VK_SELECT:
	    UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_SortProgram,ID_STA_event2_SortProgram,ID_PBR_Epg_SortProgram,260,1);
		APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
		if (APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
		}
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
#if 0
		if(APP_SYS_API_GetCurGroupType() == APP_CHN_GROUP_TYPE_FAV_e)
		{
			if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
			{
				APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
				//APP_ChnDatabase_API_CreatChnList(APP_SYS_API_GetCurAVType(), APP_CHN_GROUP_TYPE_ALL_e);
				GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
				break;
			}
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), curChn, &chnInfo))
			{
				return;
			}
			serviceID = chnInfo.m_ProgId;
			//APP_ChnDatabase_API_CreatChnList(APP_SYS_API_GetCurAVType(), APP_CHN_GROUP_TYPE_ALL_e);
			APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), serviceID,&chnInfo);
			APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
		}
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		if(APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		}
#endif
		break;
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(UI_APP_API_GetChnListPreState(), 1);
		APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
		if (APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
		}
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		UI_APP_API_SetLockCurState(UI_APP_API_GetChnListPreState());
#if 0
		if(APP_SYS_API_GetCurGroupType() == APP_CHN_GROUP_TYPE_FAV_e)
		{
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), curChn, &chnInfo))
			{
				return;
			}
			serviceID = chnInfo.m_ProgId;
			APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
			//APP_ChnDatabase_API_CreatChnList(APP_SYS_API_GetCurAVType(), APP_CHN_GROUP_TYPE_ALL_e);			
			APP_ChnDatabase_API_JudgeChn0();
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), serviceID,&chnInfo);
			APP_ChnDatabase_API_SetCurChnNO(chnInfo.m_ChnIndex);
		}
		prestate = UI_APP_API_GetChnListPreState();
		GUI_ENC_API_SwitchState(prestate, 1);
		if(prestate == ID_DLG_ChnInfo)
		{
			if(APP_ChnDatabase_API_GetCurTypeChnNum())
			{
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
				//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}

		}
#endif
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
		if (APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
		}
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		break;
	default:
		break;
		
		
	}
}
void UI_APP_ChnList_Change(void)
{
	BS32 state;

	GUI_ENC_API_GetValue(ID_LBP_Prog_ProgList, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_chnList_curPage++;
			break;
		case -1:
			if(g_chnList_curPage == 0)
				g_chnList_curPage = g_chnList_pageNum -1;
			else
				g_chnList_curPage--;
			
			if(g_chnList_curPage == g_chnList_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_Prog_ProgList, WDT_LBP_SV_INDEX, PAGE_CHN_NUM-1, 0);
			}
			break;
		default:
			break;
	}
	ChnList_IPI_ShowCurPage();//显示当前页

}

void UI_APP_ChnList_Timer(void)
{
	//UI_APP_API_RenewEvent(ID_STA_event1_ProgList,ID_STA_event2_ProgList,ID_PBR_Epg_ProgList,260,1);
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_event1_ProgList,ID_STA_event2_ProgList,ID_PBR_Epg_ProgList,260,1);

}

