#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

//频道编辑退出提示

void EditInfo_IPI_ReCreatChnList(void)
{
    APP_CHN_AV_TYPE_E AvType = APP_ChnDatabase_API_GetCurChnType();
    if (AvType == APP_CHN_AV_TYPE_TV_e)
	{
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_FAV_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_1_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_2_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_3_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_4_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_5_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_6_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_7_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_8_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_9_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_10_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_11_e);
		
	}
	else
	{
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_FAV_e);
	}
}
void UI_APP_EditInfo_Enter(void)
{
   // GUI_ENC_API_SetValue(ID_STA_Line1_EditInfo, WDT_STA_SV_DATAID, RS_ChnSetSave, 0);
}
void UI_APP_EditInfo_End(void)
{
#if 1
	BU32 i;
	BU32 lang;
	APP_CHN_AV_TYPE_E AvType = APP_ChnDatabase_API_GetCurChnType();
	APP_CHN_GROUP_TYPE_E GroupType = APP_CHN_GROUP_TYPE_ALL_e;
	ChnDataBase_ChnInfo_S chnInfo;
	APP_ChnDatabase_API_DelChnByTag();	
	if (AvType == APP_CHN_AV_TYPE_TV_e)
	{
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_FAV_e);
	}
	else
	{
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_FAV_e);
	}
	//同时也更新节目分类
	APP_ChnDatabase_API_UpDataToFlash(1);
	if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
	{
		AvType = (AvType == APP_CHN_AV_TYPE_TV_e)?2:1;
		APP_Com_API_StopProg(1);
		if (APP_FAIL == APP_ChnDatabase_API_SetCurChnType(AvType, GroupType))
		{
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
		}
		else
		{
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		}
	#if 0
		if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
		{
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);	
		}
		else
		{
			if (AvType == APP_CHN_AV_TYPE_RD_e)
			{
				if(!UI_APP_API_bShowNoSignal())
				{
					APP_Com_API_ShowBmp(1);//广播LOGO
				}
			}
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led (APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		}
	#endif
	}
	else
	{
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	//	UI_APP_API_CheckLock_PlayChn_led (APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
	}
#endif
}
void UI_APP_EditInfo_Key(BU32 key)
{
   #if 0
    switch(key)
    {
    case GUI_VK_SELECT:
		GUI_ENC_API_SetValue(ID_STA_Line1_EditInfo, WDT_STA_SV_DATAID, RS_SaveWait, 0);
		APP_ChnDatabase_API_UpDataToFlash(1);
		EditInfo_IPI_ReCreatChnList();
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
	case GUI_VK_EXIT:
		APP_ChnDatabase_API_LoadFromFlash();
		EditInfo_IPI_ReCreatChnList();
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	    break;
	default:
		break;
    }
#endif
	
}

void UI_APP_EditInfo_Timer(void)
{
	GUI_ENC_API_SwitchState(ID_DLG_SysSet,1);
}
