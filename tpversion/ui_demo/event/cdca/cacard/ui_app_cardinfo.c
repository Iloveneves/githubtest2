#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

extern BU08 g_ui_app_str[13][80];
static BU16 g_cardinfo_tvsId[4];

//智能卡基本信息

void UI_APP_CA_CardInfo_Enter(void)
{
	BS32 ret;
	BU32 motherNum;
	BU32 cas_ver;
	BU32 rating;
	BU08 strTemp[40];
	BU08 card_sn[17];
	BU08 bLock;
	BU08 i =0;
	BU16 TVS[4] = {0};
	FYF_CA_WorkTime_s workTime;
	FYF_CA_MotherCard_s motherCard[4];
	BU16 wPlatformID;
	BU32 dwUniqueID;
	BU08 stbid[15];
	BU32 lang;
	char **pDic = DicData;
	FYF_CDCAOperatorInfo_s tvsName;
	
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	
	/*版本*/
	ret = FYF_API_ca_get_value(FYF_CA_SV_CARD_INFO,&cas_ver,(BU32*)card_sn);
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[0],"%-20s %s",pDic[RS_CAManufactory],pDic[RS_CDCA]);
		sprintf(g_ui_app_str[1],"%-20s 0x%08x",pDic[RS_CAVer],cas_ver);
		sprintf(g_ui_app_str[2],"%-20s %s",pDic[RS_CardNum],card_sn);
	}
	else
	{
		sprintf(g_ui_app_str[0],"%-20s %s",pDic[RS_CAManufactory],pDic[RS_GetFailed]);
		sprintf(g_ui_app_str[1],"%-20s %s",pDic[RS_CAVer],pDic[RS_GetFailed]);
		sprintf(g_ui_app_str[2],"%-20s %s",pDic[RS_CardNum],pDic[RS_GetFailed]);
	}

	/* STBID */
	FYF_API_ca_get_value (FYF_CA_SV_STBID, (BU32 *)&wPlatformID, (BU32 *)&dwUniqueID);
	sprintf (stbid, "0x%04x%08x", wPlatformID,dwUniqueID);
	sprintf(g_ui_app_str[3],"%-20s %s",pDic[RS_STBID],stbid);
	
	/*工作时段*/
	ret = FYF_API_ca_get_value(FYF_CA_SV_WORK_TIME,(BU32 *)(&workTime),0);
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[4],"%-20s %02d:%02d:%02d-%02d:%02d:%02d",pDic[RS_WorkTime],workTime.byStartHour,workTime.byStartMinute,workTime.byStartSecond,workTime.byEndHour,workTime.byEndMinute,workTime.byEndSecond);
	}
	else
	{
		sprintf(g_ui_app_str[4],"%-20s %s",pDic[RS_WorkTime],pDic[RS_GetFailed]);
	}	
	
	/*PIN锁定状态*/
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[5],"%-20s %s",pDic[RS_PinLockState],pDic[RS_No]);
	}
	else
	{
		sprintf(g_ui_app_str[5],"%-20s %s",pDic[RS_PinLockState],pDic[RS_No]);
	}	

	/* 收看等级*/
	ret = FYF_API_ca_get_value(FYF_CA_SV_RATING,&rating,0);
	if(ret == FYF_OK)
	{
		sprintf(g_ui_app_str[6],"%-20s %d",pDic[RS_CurRating],rating);
	}
	else
	{
		sprintf(g_ui_app_str[6],"%-20s %s",pDic[RS_CurRating],pDic[RS_GetFailed]);
	}	

	
	
	/*运营商 & 子母卡属性*/
	sprintf(g_ui_app_str[7],"    ");
	sprintf(g_ui_app_str[8],"%-10s %s",pDic[RS_TVS_ID],pDic[RS_TVSNAME]);
	#if 1
	ret = FYF_API_ca_get_value(FYF_CA_SV_OPERATOR_INFO,(BU32 *)g_cardinfo_tvsId,0);
	if(ret == FYF_OK)
	{
		for(i=0;i<4;i++)
		{
			if(g_cardinfo_tvsId[i])
			{
				ret = FYF_API_ca_get_value(FYF_CA_SV_TVS_NAME, (BU32 *)&g_cardinfo_tvsId[i], (BU32 *)&tvsName);
				if(ret == FYF_CDCA_RC_OK)
				{
				    APP_Printf("FYF_API_ca_get_value is FYF_CDCA_RC_OK\n");
					sprintf(g_ui_app_str[9+i],"%-10d %s",g_cardinfo_tvsId[i],tvsName.m_szTVSPriInfo);	
				}
				else
				{
				    APP_Printf("FYF_API_ca_get_value is FYF_CDCA_RC_ERROR\n");
					sprintf(g_ui_app_str[9+i],"%-10d",g_cardinfo_tvsId[i]);	
				}
			}
			else
			{
			    APP_Printf("FYF_API_ca_get_value \n");
				sprintf(g_ui_app_str[9+i],"    ");	

			}
		}
			
	}
	else
	#endif
	{	
		sprintf(g_ui_app_str[9],"    ");	
		sprintf(g_ui_app_str[10],"    ");	
		sprintf(g_ui_app_str[11],"    ");	
		sprintf(g_ui_app_str[12],"    ");	
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
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	}
}
