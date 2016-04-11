#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define CA_ENTITLE_INFO_NUM  8

extern BU08 g_ui_app_str[13][80];

static BU32 g_slot_tvsnum=0;
static BU08 g_servicer_tvsnum = 5;
static FYF_CA_Money_s g_slot_info[5];

void CA_Slot_ShowPage(void)
{
	BU32 i,num,index;
	BU08 str[20];
	BU08 credit[11]={0};
	BU08 balance[11]={0};
	char **pDic = DicData;
	BU32 lang;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}

	if(g_slot_tvsnum==0 )
	{
		for(i=0;i<10;i++)
		{
			sprintf(g_ui_app_str[i]," ");
		}
		GUI_ENC_API_SetValue(ID_STA_Text_ePurse, WDT_STA_SV_DATAID, RS_SlotNotExist, 0);
	}
	else
	{
			
		sprintf(g_ui_app_str[0], "%-21s %-14s %s",pDic[RS_TVSNAME],pDic[RS_Balance],pDic[RS_CreditLim]);
		for(i=0;i<g_slot_tvsnum;i++)
		{
			sprintf(credit,"%d.%02d", g_slot_info[i].m_paid/100,g_slot_info[i].m_paid%100);
			sprintf(balance,"%d.%02d", g_slot_info[i].m_balance/100,g_slot_info[i].m_balance%100);
			sprintf(g_ui_app_str[i+1],"%-21s %-14s %s",g_slot_info[i].m_szTVSName,credit,balance);
	 	}
		for(i=g_slot_tvsnum;i<CA_ENTITLE_INFO_NUM;i++)
		{
			sprintf(g_ui_app_str[i+1]," ");
		}
		sprintf(g_ui_app_str[9],"%s:%d",pDic[RS_SlotCount],g_slot_tvsnum);

	}	
	GUI_ENC_API_SetValue(ID_STA_Line0_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
	GUI_ENC_API_SetValue(ID_STA_Line4_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[4], 0);
	GUI_ENC_API_SetValue(ID_STA_Line5_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[5], 0);
	GUI_ENC_API_SetValue(ID_STA_Line6_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[6], 0);
	GUI_ENC_API_SetValue(ID_STA_Line7_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[7], 0);
	GUI_ENC_API_SetValue(ID_STA_Line8_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[8], 0);
	GUI_ENC_API_SetValue(ID_STA_Line9_ePurse, WDT_STA_SV_STRING, (BU32)g_ui_app_str[9], 0);

}

void UI_APP_CA_Slot_Enter(void)
{
	BU32 i;
	BU32 index,num;
	BU32 id;
	BS32 ret;

	FYF_CA_TvsInfo  tvsid[5];
	g_servicer_tvsnum = 5;
	ret = FYF_API_ca_get_value(FYF_CA_SV_OPERATOR_INFO,(BU32 *)tvsid,(BU32*)&g_servicer_tvsnum);
	if(g_servicer_tvsnum == 0)
	{
		g_slot_tvsnum = 0;
	}
	else
	{
		for(i=0;i<g_servicer_tvsnum;i++)
		{
		 	ret = FYF_API_ca_get_value(FYF_CA_SV_MONEY_INFO,(BU32 *)(&tvsid[i].m_wTVSID),(BU32 *)&g_slot_info[g_slot_tvsnum]);	
			if(ret == FYF_OK)
			{
				GUI_API_Memcpy(g_slot_info[g_slot_tvsnum].m_szTVSName, tvsid[i].m_szTVSName, 21*sizeof(char));
				g_slot_tvsnum ++;
			}
		}
	}
	if(g_slot_tvsnum)
	{
		GUI_ENC_API_SetVisible(ID_STA_TextBk_ePurse, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_ePurse, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ePurse, WS_DISABLE);
		
 	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_TextBk_ePurse, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_ePurse, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ePurse, WS_VISIBLE);
	}
	CA_Slot_ShowPage();
	APP_Com_API_SetFullScreen();
}


void UI_APP_CA_Slot_End(void)
{
	g_slot_tvsnum = 0;
}

void UI_APP_CA_Slot_Key(BU32 key)
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
	default:
		break;
	}
}

