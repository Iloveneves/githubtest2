#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

static APP_CHN_AV_TYPE_E av_type_before_search;
static BU32 searchinfo_bkey = 0;
void UI_Search_API_GetSearchTvRadioNum(BU16 *tvnum,BU16 *radionum, BU16 *broadcastnum);

//搜索结束提示
void UI_APP_SearchInfo_Enter(void)
{
	searchinfo_bkey = 0;
	GUI_ENC_API_SetValue(ID_TIM_SearchInfo, WDT_TIM_SV_PLAY,0,0);
	av_type_before_search = APP_ChnDatabase_API_GetCurChnType ();
	if(APP_CHN_AV_TYPE_TV_e != av_type_before_search && APP_CHN_AV_TYPE_RD_e != av_type_before_search)
	{
		av_type_before_search = APP_CHN_AV_TYPE_TV_e;
	}	
}
void UI_APP_SearchInfo_End(void)
{
	BU08 str[4];
	BU16 chnNO = 0;
	BU32 chnNum = 0;
	BU32 bitmap;
	BU32 prog_id = 0;
	BU32 i;
	APP_CHN_AV_TYPE_E av_type;
	ChnDataBase_ChnInfo_S chnInfo;

	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_FAV_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_FAV_e);
	for (i = APP_CHN_GROUP_TYPE_1_e; i < APP_CHN_GROUP_TYPE_26_e; i++)
	{
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, i);
	}
	APP_Com_API_Start_Signal_Dectect();
	APP_Search_Set_OTA_Status(1);


#ifndef NO_BROADCAST
	DBA_API_CreateBroadCastList();
#endif

	if (APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(av_type_before_search, APP_CHN_GROUP_TYPE_ALL_e) &&
		APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
	{
		av_type = (av_type_before_search == APP_CHN_AV_TYPE_TV_e)?APP_CHN_AV_TYPE_RD_e:APP_CHN_AV_TYPE_TV_e;
		APP_ChnDatabase_API_SetCurChnType(av_type, APP_CHN_GROUP_TYPE_ALL_e);
	}

	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
	if (chnNum == 0)
	{
		APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
	}
	
#if 0
	if(0 == chnNum)
	{
		av_type = (av_type_before_search == APP_CHN_AV_TYPE_TV_e)?APP_CHN_AV_TYPE_RD_e:APP_CHN_AV_TYPE_TV_e;
		APP_ChnDatabase_API_SetCurChnType(av_type, APP_CHN_GROUP_TYPE_ALL_e);
		chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
		if (0 == chnNum && av_type_before_search == APP_CHN_AV_TYPE_TV_e)
		{
			APP_ChnDatabase_API_SetCurChnType(av_type_before_search, APP_CHN_GROUP_TYPE_ALL_e);
			chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
		}
	}
#endif
	APP_ChnDatabase_API_JudgeChn0();
	if(chnNum != 0)
	{
		prog_id = UI_APP_API_GetCurChnProgID();
		if(prog_id)
		{
			APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),prog_id, &chnInfo);
			chnNO = chnInfo.m_ChnIndex;
			APP_ChnDatabase_API_SetCurChnNO(chnNO);
		}
		else
		{
			chnNO = 0;
		}
		UI_APP_API_PlayChn(chnNO, chnNO, 1);
		//UI_APP_API_CheckLock_PlayChn_led (chnNO, chnNO, 0);

		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
	}
	else
	{
		GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
	}	
}
void UI_APP_SearchInfo_Key(BU32 key)
{
	if(searchinfo_bkey)
	{
		switch(key)
		{
		case GUI_VK_EXIT:
		case GUI_VK_SELECT:
			//GUI_ENC_API_SwitchState(ID_DLG_ProgManager,1);
		break;
		}
	}
}

void UI_APP_SearchInfo_Timer(void)
{
	BU32 curID;
	BU16 search_tvnum;
	BU16 search_radionum;
	BU16 search_broadcastnum;
	BU32 search_nvodnum;
	GUI_ENC_API_SetValue(ID_TIM_SearchInfo, WDT_TIM_SV_STOP,0,0);
	search_nvodnum = APP_PMT_API_GetSearchNvodNum();
	UI_Search_API_GetSearchTvRadioNum(&search_tvnum,&search_radionum,&search_broadcastnum);
	if(search_tvnum || search_radionum || search_broadcastnum ||search_nvodnum)
	{
		UI_Search_API_SetSearchTvRadioNum(0,0,0);
		APP_PMT_API_SetSearchNvodNum(0);
		GUI_ENC_API_GetValue(ID_MNU_SysSet,WDT_MNU_SV_CURDATAID,&curID,0);
		if(RS_ManualSearch != curID)
		{
			APP_Search_API_SearchSortInfo();//自动搜索不排序，手动搜索排序
		}
		APP_Search_API_StartSave();	
	}
	//GUI_ENC_API_SwitchState(ID_DLG_ProgManager,1);
	if(searchinfo_bkey == 0)
		searchinfo_bkey = 1;
	GUI_ENC_API_SwitchState(ID_DLG_SysSet,1);
//	GUI_ENC_API_SetValue(ID_STA_Line3_SearchInfo,WDT_STA_SV_DATAID,RS_SearchExit,0);

}

