#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

extern BU08 g_ui_app_str[13][80];

//智能卡基本信息

void UI_APP_CA_CardInfo_Enter(void)
{
	BS32 ret;
	BU32 rating;
	BU08 bLock;
	BU16 TVS[4] = {0};
	FYF_CA_WorkTime_s workTime;
	BU32 lang;
	char **pDic = DicData;
	FYF_CA_ManuInfo dvt_card;
	BU32 dwMotherCardID;
	FYF_CA_AreaInfo AreaInfo;
	BU08 str[20];
	
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	
	ret = FYF_API_ca_get_value(FYF_CA_SV_CARD_INFO,&dvt_card,0);
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[0],"%-20s %s",pDic[RS_CAManufactory],dvt_card.m_szSCCASManuName);	
		sprintf(g_ui_app_str[1],"%-20s %s",pDic[RS_CardNum],dvt_card.m_byszSerialNO);
		sprintf(g_ui_app_str[2],"%-20s %d",pDic[RS_SCCardID],dvt_card.m_dwCardID);
		sprintf(g_ui_app_str[3],"%-20s %lu",pDic[RS_SCCOSVer],dvt_card.m_dwSCCOSVer);
		sprintf(g_ui_app_str[4],"%-20s %x",pDic[RS_CAVer],dvt_card.m_dwSTBCASVer);
	}
	else
	{
		sprintf(g_ui_app_str[0],"%-20s %s",pDic[RS_CAManufactory],pDic[RS_GetFailed]);	
		sprintf(g_ui_app_str[1],"%-20s %s",pDic[RS_CardNum],pDic[RS_GetFailed]);
		sprintf(g_ui_app_str[2],"%-20s %s",pDic[RS_SCCardID],pDic[RS_GetFailed]);
		sprintf(g_ui_app_str[3],"%-20s %s",pDic[RS_SCCOSVer],pDic[RS_GetFailed]);
		sprintf(g_ui_app_str[4],"%-20s %s",pDic[RS_CAVer],pDic[RS_GetFailed]);
	}
//	sprintf(g_ui_app_str[5],"    ");
	/*工作时段*/
	ret = FYF_API_ca_get_value(FYF_CA_SV_WORK_TIME,(BU32 *)(&workTime),0);
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[5],"%-20s %02d:%02d-%02d:%02d",pDic[RS_WorkTime],workTime.byStartHour,workTime.byStartMinute,workTime.byEndHour,workTime.byEndMinute);
	}
	else
	{
		sprintf(g_ui_app_str[5],"%-20s %s",pDic[RS_WorkTime],pDic[RS_GetFailed]);
	}	
	
	/*PIN锁定状态*/
	ret = FYF_API_ca_get_value(FYF_CA_SV_PIN_LOCK,(BU32 *)(&bLock),0);
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[6],"%-20s %s",pDic[RS_PinLockState],(bLock)?pDic[RS_Yes]:pDic[RS_No]);
	}
	else
	{
		sprintf(g_ui_app_str[6],"%-20s %s",pDic[RS_PinLockState],pDic[RS_GetFailed]);
	}	

	/* 收看等级*/
	ret = FYF_API_ca_get_value(FYF_CA_SV_RATING,&rating,0);
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[7],"%-20s %d",pDic[RS_CurRating],rating);
	}
	else
	{
		sprintf(g_ui_app_str[7],"%-20s %s",pDic[RS_CurRating],pDic[RS_GetFailed]);
	}	

	
//	sprintf(g_ui_app_str[7],"    ");
	/*运营商 & 子母卡属性*/
	ret = FYF_API_ca_get_value(FYF_CA_SV_AREA_INFO,(BU32 *)&AreaInfo,0);
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[8],"%-20s %d",pDic[RS_RegionID],AreaInfo.m_dwCardArea);
		
		if(AreaInfo.m_byStartFlag)
			sprintf(g_ui_app_str[9],"%-20s %s",pDic[RS_AreaLockState],pDic[RS_AreaLockStart]);
		else
			sprintf(g_ui_app_str[9],"%-20s %s",pDic[RS_AreaLockState],pDic[RS_AreaLockUnStart]);

		UI_APP_GetTimeStrFromValC(AreaInfo.m_tSetStreamTime,str);
		sprintf(g_ui_app_str[10],"%-20s %s",pDic[RS_AreaStreamTime],str);
	
	}
	else
	{
		sprintf(g_ui_app_str[8],"%-20s %s",pDic[RS_RegionID],pDic[RS_GetFailed]);		
		sprintf(g_ui_app_str[9],"%-20s %s",pDic[RS_AreaLockState],pDic[RS_GetFailed]);
		sprintf(g_ui_app_str[10],"%-20s %s",pDic[RS_AreaStreamTime],pDic[RS_GetFailed]);
	
	}
	
	ret = FYF_API_ca_get_value(FYF_CA_SV_MOTHER_INFO,(BU32 *)&dwMotherCardID,0);
	if(ret == FYF_OK)
	{
		if(dwMotherCardID == 0)
		{
			sprintf(g_ui_app_str[11],"%-20s %s",pDic[RS_Attr],pDic[RS_MotherCard]);
			sprintf(g_ui_app_str[12],"%-20s %s",pDic[RS_MotherCardNum],"0");	
		}	
		else
		{
			sprintf(g_ui_app_str[11],"%-20s %s",pDic[RS_Attr],pDic[RS_ChildCard]);
			sprintf(g_ui_app_str[12],"%-20s %d",pDic[RS_MotherCardNum],dwMotherCardID);	
		}	
	}
	else
	{
		sprintf(g_ui_app_str[11],"%-20s %s",pDic[RS_Attr],pDic[RS_GetFailed]);
		sprintf(g_ui_app_str[12],"%-20s %s",pDic[RS_MotherCardNum],pDic[RS_GetFailed]);	
	}
	

	GUI_ENC_API_SetValue(ID_STA_Line1_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	GUI_ENC_API_SetValue(ID_STA_Line4_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
	GUI_ENC_API_SetValue(ID_STA_Line5_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[4], 0);
	GUI_ENC_API_SetValue(ID_STA_Line6_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[5], 0);
	GUI_ENC_API_SetValue(ID_STA_Line7_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[6], 0);
	GUI_ENC_API_SetValue(ID_STA_Line8_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[7], 0);
	GUI_ENC_API_SetValue(ID_STA_Line9_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[8], 0);
	GUI_ENC_API_SetValue(ID_STA_Line10_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[9], 0);
	GUI_ENC_API_SetValue(ID_STA_Line11_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[10], 0);
	GUI_ENC_API_SetValue(ID_STA_Line12_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[11], 0);
	GUI_ENC_API_SetValue(ID_STA_Line13_ICInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[12], 0);
	APP_Com_API_SetFullScreen();
}

void UI_APP_CA_CardInfo_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	}
}


