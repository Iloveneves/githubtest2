#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define EDIT_PAGE_CHN_NUM 8
#define EDIT_CHN_NAME_LEN 24


static BU08 g_chnEdit_chnName[EDIT_PAGE_CHN_NUM][EDIT_CHN_NAME_LEN];

static CHN_EDIT_LIST_S g_chnEdit_list_s[EDIT_PAGE_CHN_NUM] = 
{
   { g_chnEdit_chnName[0],0,0,0},
   { g_chnEdit_chnName[1],0,0,0},
   { g_chnEdit_chnName[2],0,0,0},
   { g_chnEdit_chnName[3],0,0,0},
   { g_chnEdit_chnName[4],0,0,0},
   { g_chnEdit_chnName[5],0,0,0},
   { g_chnEdit_chnName[6],0,0,0},
   { g_chnEdit_chnName[7],0,0,0},
};

extern GUI_lbpAttribute_s lbpa_move;
extern GUI_lbpAttribute_s lbpa_ChEdit[];
static BU32 g_chnEdit_curPage = 0;
static BU32 g_chnEdit_pageNum = 0;
static BU32 g_chnEdit_rest= 0;

static BU08 g_chnEdit_needsave = 0;

static BU08 g_chnEdit_curSortIndex = 0;

/*----------------------------------------------------------------------------
描述：每次创建列表成功后调用
-----------------------------------------------------------------------------*/
void ChnEdit_IPI_SetPageParam(void)
{
	BU16 chnNum,curChn;
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	g_chnEdit_pageNum = (chnNum +  EDIT_PAGE_CHN_NUM -1) / EDIT_PAGE_CHN_NUM;	
	g_chnEdit_rest = chnNum % EDIT_PAGE_CHN_NUM;
	g_chnEdit_curPage = curChn / EDIT_PAGE_CHN_NUM;
	if(g_chnEdit_rest == 0)
	{
		g_chnEdit_rest = EDIT_PAGE_CHN_NUM;
	}
	
	GUI_ENC_API_SetValueUnPaint(ID_SBR_ChnEdit, WDT_SBR_SV_MAX, (BU32)chnNum-1, 0);
	GUI_ENC_API_SetValueUnPaint(ID_SBR_ChnEdit, WDT_SBR_SV_MIN, 0, 0); 
	GUI_ENC_API_SetValue(ID_SBR_ChnEdit, WDT_SBR_SV_VALUE, (BU32)curChn, 0);
}

void ChnEdit_IPI_ShowCurPage(void)
{
	ChnDataBase_ChnInfo_S ChnInfo;
	BU16 ChnNo,i;
	BU16 pageNum;
	BU08 sortFlag;
	
	if(g_chnEdit_pageNum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_NUM, 0, 0);
		return;
	}
	
	if(g_chnEdit_curPage >= g_chnEdit_pageNum)
	{
		g_chnEdit_curPage = 0;
	}
	
	if(g_chnEdit_curPage+1 == g_chnEdit_pageNum)
	{	
		pageNum = g_chnEdit_rest;
	}
	else
	{
		pageNum = EDIT_PAGE_CHN_NUM;
	}
	
	for(i=0; i<pageNum; i++)
	{	
		ChnNo = i+g_chnEdit_curPage*EDIT_PAGE_CHN_NUM;
		APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ChnNo,&ChnInfo);
		ChnInfo.m_ChnName[13]=0;
		if(ChnInfo.m_Ca)
		{
			sprintf(g_chnEdit_list_s[i].chnName,"￥%03d %s",ChnNo + APP_ChnDatabase_API_GetChn0Flag(),ChnInfo.m_ChnName);

		}
		else
		{
			sprintf(g_chnEdit_list_s[i].chnName,"  %03d %s",ChnNo + APP_ChnDatabase_API_GetChn0Flag(),ChnInfo.m_ChnName);
		}
		g_chnEdit_list_s[i].fav  = ChnInfo.m_Fav;
		g_chnEdit_list_s[i].del = ChnInfo.m_Del;
		g_chnEdit_list_s[i].lock= ChnInfo.m_Lock;

		
	}
	GUI_ENC_API_SetValue(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_DATA, (BU32)g_chnEdit_list_s, pageNum);
}

void ChnEdit_IPI_SwapChn(BU16 srcChn, BU16 destChn)
{
	APP_ChnDatabase_API_SwapChn(srcChn, destChn);
	GUI_ENC_API_SetValueUnPaint(ID_LBP_ChnList_ChnEdit,WDT_LBP_SV_INDEX,(BU32)(destChn%EDIT_PAGE_CHN_NUM),0);
}

void ChnEdit_IPI_ShowHelp(void)
{
    BU32 chnEditPreState;
	chnEditPreState = UI_APP_API_GetChnEditPreState();
    if(chnEditPreState == 0)
    {
         if(APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
         {
             GUI_ENC_API_SetValue(ID_STA_Ico1_ChnEdit, WDT_STA_SV_STYLE, STYLE_RedKey_W20_H20, 0);
			 GUI_ENC_API_SetValue(ID_STA_Ico2_ChnEdit, WDT_STA_SV_STYLE, STYLE_GreenKey_W20_H20, 0);
			 GUI_ENC_API_SetValue(ID_STA_Ico3_ChnEdit, WDT_STA_SV_STYLE, STYLE_YellowKey_W20_H20, 0);
			 GUI_ENC_API_SetValue(ID_STA_Ico4_ChnEdit, WDT_STA_SV_STYLE, STYLE_NULL, 0);
			 GUI_ENC_API_SetValue(ID_STA_Ico5_ChnEdit, WDT_STA_SV_STYLE, STYLE_NULL, 0);

			 GUI_ENC_API_SetValue(ID_STA_Text1_ChnEdit, WDT_STA_SV_DATAID, RS_FavSet, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text2_ChnEdit, WDT_STA_SV_DATAID, RS_MoveSet, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text3_ChnEdit, WDT_STA_SV_DATAID, RS_DelChn, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text4_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text5_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text6_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
         }
		 else
		 {
		     GUI_ENC_API_SetValue(ID_STA_Ico1_ChnEdit, WDT_STA_SV_STYLE, STYLE_RedKey_W20_H20, 0);
			 GUI_ENC_API_SetValue(ID_STA_Ico2_ChnEdit, WDT_STA_SV_STYLE, STYLE_GreenKey_W20_H20, 0);
			 GUI_ENC_API_SetValue(ID_STA_Ico3_ChnEdit, WDT_STA_SV_STYLE, STYLE_YellowKey_W20_H20, 0);
			 GUI_ENC_API_SetValue(ID_STA_Ico4_ChnEdit, WDT_STA_SV_STYLE, STYLE_SelectLR_W46_H28, 0);
			 GUI_ENC_API_SetValue(ID_STA_Ico5_ChnEdit, WDT_STA_SV_STYLE, STYLE_NULL, 0);

			 GUI_ENC_API_SetValue(ID_STA_Text1_ChnEdit, WDT_STA_SV_DATAID, RS_FavSet, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text2_ChnEdit, WDT_STA_SV_DATAID, RS_MoveSet, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text3_ChnEdit, WDT_STA_SV_DATAID, RS_DelChn, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text4_ChnEdit, WDT_STA_SV_DATAID, RS_SortSelect, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text5_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
			 GUI_ENC_API_SetValue(ID_STA_Text6_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
		 }
    }
	else
	{
	     GUI_ENC_API_SetValue(ID_STA_Ico1_ChnEdit, WDT_STA_SV_STYLE, STYLE_NULL, 0);
		 GUI_ENC_API_SetValue(ID_STA_Ico2_ChnEdit, WDT_STA_SV_STYLE, STYLE_RedKey_W20_H20, 0);
		 GUI_ENC_API_SetValue(ID_STA_Ico3_ChnEdit, WDT_STA_SV_STYLE, STYLE_NULL, 0);
		 GUI_ENC_API_SetValue(ID_STA_Ico4_ChnEdit, WDT_STA_SV_STYLE, STYLE_NULL, 0);
		 GUI_ENC_API_SetValue(ID_STA_Ico5_ChnEdit, WDT_STA_SV_STYLE, STYLE_NULL, 0);

		 GUI_ENC_API_SetValue(ID_STA_Text1_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
		 GUI_ENC_API_SetValue(ID_STA_Text2_ChnEdit, WDT_STA_SV_DATAID, RS_Lock, 0);
		 GUI_ENC_API_SetValue(ID_STA_Text3_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
		 GUI_ENC_API_SetValue(ID_STA_Text4_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
		 GUI_ENC_API_SetValue(ID_STA_Text5_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
		 GUI_ENC_API_SetValue(ID_STA_Text6_ChnEdit, WDT_STA_SV_DATAID, RS_Null, 0);
	}
}
	

void UI_APP_ChnEdit_Enter(void)
{
    BU16 chnNum;
	BU32 curID;
    BU32 chnEditPreState;
	BU32 index;
	ChnDataBase_ChnInfo_S  ChnInfo;
	
	static BU08 sortName[15];
	g_chnEdit_needsave = 0;
	APP_Com_API_SetFullScreen();
	APP_ChnDatabase_API_JudgeChn0();
	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	chnEditPreState = UI_APP_API_GetChnEditPreState();
	if(chnNum == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_ChnEdit, WDT_SBR_SV_MAX, 0, 0);
		GUI_ENC_API_SetValueUnPaint(ID_SBR_ChnEdit, WDT_SBR_SV_MIN, 0, 0); 
		GUI_ENC_API_SetValue(ID_SBR_ChnEdit, WDT_SBR_SV_VALUE, 0, 0);
	}
	else
	{
		ChnEdit_IPI_SetPageParam();
		ChnEdit_IPI_ShowCurPage();
		GUI_ENC_API_SetValue(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_INDEX, (BU32)(APP_ChnDatabase_API_GetCurChnNO()%EDIT_PAGE_CHN_NUM), 0);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 0);
	}
	
    //节目编辑
	if(chnEditPreState == 0)
	{
	   
		lbpa_ChEdit[1].keyValue = GUI_VK_YELLOW;
		lbpa_move.keyValue = GUI_VK_GREEN;
		lbpa_ChEdit[0].keyValue = GUI_VK_RED;
		lbpa_ChEdit[2].keyValue = -1;

	}
	//节目加锁
	else
	{
	    lbpa_ChEdit[0].keyValue = -1;
		lbpa_ChEdit[1].keyValue = -1;
		lbpa_ChEdit[2].keyValue = GUI_VK_RED; 
		lbpa_move.keyValue = -1;
	}

	ChnEdit_IPI_ShowHelp();
	//P_ChnDatabase_API_GetSortName(g_chnEdit_curSortIndex,sortName);
	sprintf(sortName,"所有节目");
	GUI_ENC_API_SetValue(ID_STA_SortName_ChnEdit , WDT_STA_SV_STRING,sortName,0);
}

void UI_APP_ChnEdit_End(void)
{

}
void UI_APP_ChnEdit_Key(BU32 key)
{
    BU16 curChn;
	BU32 index;
	BU32 keyState;
	BU08 sortFlag;
	BU32 chnEditPreState;
	ChnDataBase_ChnInfo_S chnInfo;
	static BU08 sortName[15];
	chnEditPreState = UI_APP_API_GetChnEditPreState();
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_UP:
	case GUI_VK_P_DOWN:
		GUI_ENC_API_GetValue(ID_LBP_ChnList_ChnEdit,WDT_LBP_SV_INDEX,&index,0);
		curChn = (BU16)(index+g_chnEdit_curPage*EDIT_PAGE_CHN_NUM);
 
		APP_ChnDatabase_API_SetCurChnNO(curChn);
		GUI_ENC_API_GetValue(ID_LBP_ChnList_ChnEdit,WDT_LBP_SV_KEYSTATE,&keyState,0);
		if(!keyState)//连续键处理
		{
		//	UI_APP_API_CheckLock_PlayChn_led (curChn, curChn, 1);
			UI_APP_API_PlayChn(curChn, curChn, 0);
		}
		GUI_ENC_API_SetValue(ID_SBR_ChnEdit, WDT_SBR_SV_VALUE, (BU32)curChn, 0);
		break;
	case GUI_VK_RED:
		if(chnEditPreState == 0)
		{
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo))
		    {
				return;
		    }
		    if(chnInfo.m_Fav == 1)
		    {
				chnInfo.m_Fav = 0;
		    }
		    else
		    {
				chnInfo.m_Fav = 1;
	    	}
			g_chnEdit_needsave = 1;
		    APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo);
		}
		else if(chnEditPreState == 1)//节目加锁
		{
		    curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo))
		    {
				return;
		    }
		    if(chnInfo.m_Lock== 1)
		    {
				chnInfo.m_Lock = 0;
		    }
		    else
		    {
				chnInfo.m_Lock = 1;
	    	}
			g_chnEdit_needsave = 1;
		    APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo);
		}
		break;
	case GUI_VK_GREEN:
		if(chnEditPreState == 1)
			break;
 
		g_chnEdit_needsave = 1;
		break;
	case GUI_VK_YELLOW:
		chnEditPreState = UI_APP_API_GetChnEditPreState();
		if(chnEditPreState == 1)
			break; 
		
		curChn = APP_ChnDatabase_API_GetCurChnNO();
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo))
	    {
			return;
	    }
	    if(chnInfo.m_Del== 1)
	    {
			chnInfo.m_Del= 0;
	    }
	    else
	    {
			chnInfo.m_Del= 1;
    	}
		g_chnEdit_needsave = 1;
	    APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo);
		break;
#if 0
	case GUI_VK_LEFT:
		if(APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_RD_e)
		{
		     g_chnEdit_curSortIndex = (g_chnEdit_curSortIndex == 0)?1:0;
		}
		else
		{
		     if(g_chnEdit_curSortIndex == 0)
		     {
		          g_chnEdit_curSortIndex = APP_ChnDatabase_API_GetSortCount() -1;
		     }
			 else
			 {
			      g_chnEdit_curSortIndex--;
			 }
		}
		APP_ChnDatabase_API_GetSortName(g_chnEdit_curSortIndex,sortName);
	    GUI_ENC_API_SetValue(ID_STA_SortName_ChnEdit , WDT_STA_SV_STRING,sortName,0);
		if(chnEditPreState == ID_DLG_ProgManager || chnEditPreState == ID_DLG_ChnInfo)
		{
			if(g_chnEdit_curSortIndex >=2)
		    {
	           lbpa_ChEdit[0].keyValue = GUI_VK_F1;
		    }
			else
			{
			   lbpa_ChEdit[0].keyValue = -1;
			}
		}
		ChnEdit_IPI_ShowCurPage();
		break;
	case GUI_VK_RIGHT:
		if(APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_RD_e)
		{
		     g_chnEdit_curSortIndex = (g_chnEdit_curSortIndex == 0)?1:0;
		}
		else
		{
		     if(g_chnEdit_curSortIndex == (APP_ChnDatabase_API_GetSortCount()-1))
		     {
		          g_chnEdit_curSortIndex = 0;
		     }
			 else
			 {
			      g_chnEdit_curSortIndex++;
			 }
		}
		APP_ChnDatabase_API_GetSortName(g_chnEdit_curSortIndex,sortName);
	    GUI_ENC_API_SetValue(ID_STA_SortName_ChnEdit , WDT_STA_SV_STRING,sortName,0);
		if(chnEditPreState == ID_DLG_ProgManager || chnEditPreState == ID_DLG_ChnInfo)
		{
			if(g_chnEdit_curSortIndex >=2)
		    {
	           lbpa_ChEdit[0].keyValue = GUI_VK_F1;
		    }
			else
			{
			   lbpa_ChEdit[0].keyValue = -1;
			}
		}
		ChnEdit_IPI_ShowCurPage();
		break;
	case GUI_VK_F1:
		if(chnEditPreState == ID_DLG_ProgManager || chnEditPreState == ID_DLG_ChnInfo)
		{
			if(g_chnEdit_curSortIndex >= 2)
			{
				curChn = APP_ChnDatabase_API_GetCurChnNO();
				APP_ChnDatabase_API_GetSortTypeFlag(curChn, g_chnEdit_curSortIndex,&sortFlag);
				sortFlag = (sortFlag == 0)?1:0;
				APP_ChnDatabase_API_SetSortTypeFlag(curChn, g_chnEdit_curSortIndex,sortFlag);
				g_chnEdit_needsave = 1;
			}
		}
		break;
#endif
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		if(g_chnEdit_needsave)
		{
			GUI_ENC_API_SwitchState(ID_DLG_EditInfo, 1);
		}
		else
		{
	
			GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		}
		g_chnEdit_curSortIndex = 0;
		break;
	case GUI_VK_EXIT:
	case GUI_VK_SELECT:
		if(g_chnEdit_needsave)
		{
			GUI_ENC_API_SwitchState(ID_DLG_EditInfo, 1);
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		}
		g_chnEdit_curSortIndex =0;
		break;
	default:
		break;
	}
}

void UI_APP_ChnEdit_Change(void)
{
    BS32 state;
	BU16 srcChn,destChn;
	BU32 i=0,x_Chn=0;
	srcChn = APP_ChnDatabase_API_GetCurChnNO();
	GUI_ENC_API_GetValue(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	APP_PRINT_DEBUG("state = %d", state);
	switch(state)
	{
		case 1:
			g_chnEdit_curPage++;
			break;
		case -1:
			if(g_chnEdit_curPage == 0)
			{
				g_chnEdit_curPage = g_chnEdit_pageNum -1;
			}
			else
			{
				g_chnEdit_curPage--;
			}
			if(g_chnEdit_curPage == g_chnEdit_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_ChnList_ChnEdit, WDT_LBP_SV_INDEX, EDIT_PAGE_CHN_NUM-1, 0);
			}
			break;
			
		case 2://没换页下
			srcChn = APP_ChnDatabase_API_GetCurChnNO();
			destChn = srcChn+1;
			ChnEdit_IPI_SwapChn(srcChn, destChn);
			break;
		case -2://没换页向上
			srcChn = APP_ChnDatabase_API_GetCurChnNO();
			destChn = srcChn-1;
			if(APP_ChnDatabase_API_GetChn0Flag() == 0 && destChn == 0)
			{
				g_chnEdit_curPage = g_chnEdit_pageNum-1;
				destChn = APP_ChnDatabase_API_GetCurTypeChnNum() -1;
			}
			ChnEdit_IPI_SwapChn(srcChn, destChn);
			break;
		case 3://换页下
		case 4://换页向下
			srcChn = APP_ChnDatabase_API_GetCurChnNO();
			if(g_chnEdit_curPage == g_chnEdit_pageNum-1)
			{
				g_chnEdit_curPage=0;
				destChn = 0;
				if(APP_ChnDatabase_API_GetChn0Flag() == 0 && destChn == 0)
				{
					destChn = 1;
				}
				#if 0
				x_Chn = APP_ChnDatabase_API_GetCurTypeChnNum()-1;
				ChnEdit_IPI_InsertChn(srcChn,x_Chn);
				ChnEdit_IPI_SwapChn(x_Chn, destChn);
				#else
				ChnEdit_IPI_SwapChn(srcChn,destChn);
				//ChnEdit_IPI_InsertChn(srcChn,destChn);
				#endif
			}
			else
			{
				g_chnEdit_curPage ++;
				destChn = g_chnEdit_curPage*EDIT_PAGE_CHN_NUM;
				ChnEdit_IPI_SwapChn(srcChn, destChn);
				//ChnEdit_IPI_InsertChn(srcChn,destChn);
			}

			break;
		case -3://换页向上
		case -4://换页向上
			srcChn = APP_ChnDatabase_API_GetCurChnNO();
			if(g_chnEdit_curPage == 0)
			{
				g_chnEdit_curPage = g_chnEdit_pageNum -1;
				destChn = APP_ChnDatabase_API_GetCurTypeChnNum() -1;
				if(APP_ChnDatabase_API_GetChn0Flag() == 0)
				{
					x_Chn = 1;
				}
				else
				{
					x_Chn = 0;
				}	
			#if 0
				ChnEdit_IPI_InsertChn(srcChn,x_Chn);
				ChnEdit_IPI_SwapChn(x_Chn, destChn);
			#else
				//ChnEdit_IPI_InsertChn(srcChn,destChn);
				ChnEdit_IPI_SwapChn(srcChn,destChn);
			#endif
			}
			else
			{
				g_chnEdit_curPage--;
				destChn = g_chnEdit_curPage*EDIT_PAGE_CHN_NUM + EDIT_PAGE_CHN_NUM-1;
				if(APP_ChnDatabase_API_GetChn0Flag() == 0 && destChn == 0)
				{
					destChn = 1;
				}
				ChnEdit_IPI_SwapChn(srcChn, destChn);
				//ChnEdit_IPI_InsertChn(srcChn,destChn);
			}
			break;
		default:
			break;
	}
	ChnEdit_IPI_ShowCurPage();//显示当前页
}

void UI_APP_ChnEdit_Timer(void)
{
   
}
