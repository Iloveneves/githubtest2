#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define CA_ENTITLE_INFO_NUM  8

extern BU08 g_ui_app_str[13][80];

static BU32 g_slot_tvsnum=0;
static BU32 g_slot_page = 0;
static BU32 g_slot_rest = 0;
static BU32 g_slot_curPage = 0;
static FYF_CDCA_TVSSlotId_s g_slot_info;

BU16* UI_APP_IPI_GetTvsId();
void CA_Slot_ShowPage(void)
{
	BU32 i,num,index;
	BU08 str[20];
	BU08 slot_id[11]={0};
	BU08 credit[11]={0};
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
	else if(g_slot_tvsnum==0xFFFFFFFF)
	{
		for(i=0;i<10;i++)
		{
			sprintf(g_ui_app_str[i]," ");
		}
		GUI_ENC_API_SetValue(ID_STA_Text_ePurse, WDT_STA_SV_DATAID, RS_GetFailed, 0);
	}
	else
	{
		if(g_slot_curPage > g_slot_page-1)g_slot_curPage = 0;
		if(g_slot_curPage == g_slot_page-1)
		{
			num = g_slot_rest;
		}
		else
		{
			num = CA_ENTITLE_INFO_NUM;
		}
		
		
		sprintf(g_ui_app_str[0], "%-10s %-10s %s",pDic[RS_SlotID],pDic[RS_CreditLim],pDic[RS_Balance]);
		for(i=0;i<num;i++)
		{
			index = g_slot_curPage*CA_ENTITLE_INFO_NUM + i;
			sprintf(slot_id,"%u", g_slot_info.m_SlotInfo[index].m_wSlotId);
			sprintf(credit,"%u", g_slot_info.m_SlotInfo[index].m_wCreditLimit);
			sprintf(g_ui_app_str[i+1],"%-10s %-10s %u",slot_id,credit,g_slot_info.m_SlotInfo[index].m_wBalance);
	 	}
		for(i=num;i<CA_ENTITLE_INFO_NUM;i++)
		{
			sprintf(g_ui_app_str[i+1]," ");
		}
		sprintf(g_ui_app_str[9],"%s:%d    %d/%d",pDic[RS_SlotCount],g_slot_tvsnum,g_slot_curPage+1,g_slot_page);

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
	BU16* tvsid;
	tvsid = UI_APP_IPI_GetTvsId();

	GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &index, 0);
 	ret = FYF_API_ca_get_value(FYF_CA_SV_MONEY_INFO,(BU32 *)(&tvsid[index]),(BU32 *)&g_slot_info);
	if(ret == FYF_OK)
	{
		g_slot_tvsnum = g_slot_info.m_SlotCount;
	}
	else
	{
		g_slot_tvsnum = 0xFFFFFFFF;
	}
	
	if(g_slot_tvsnum)
	{
		GUI_ENC_API_SetVisible(ID_STA_TextBk_ePurse, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_ePurse, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ePurse, WS_DISABLE);
		
		g_slot_page = (g_slot_tvsnum +CA_ENTITLE_INFO_NUM-1)/CA_ENTITLE_INFO_NUM;
		g_slot_rest = (g_slot_tvsnum%CA_ENTITLE_INFO_NUM);
		if(!g_slot_rest)g_slot_rest = CA_ENTITLE_INFO_NUM;
		g_slot_curPage = 0;
 	}
	else
	{
		g_slot_curPage = 0;
		g_slot_rest = 0;
		g_slot_page = 0;
		GUI_ENC_API_SetVisible(ID_STA_TextBk_ePurse, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_ePurse, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ePurse, WS_VISIBLE);
	}
	CA_Slot_ShowPage();
}


void UI_APP_CA_Slot_End(void)
{

}

void UI_APP_CA_Slot_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_P_UP:
	case GUI_VK_LEFT:
		if(g_slot_page <= 1)return;
		if(g_slot_curPage == 0)
		{
			g_slot_curPage = g_slot_page - 1;
		}
		else
		{
			g_slot_curPage--;
		}
		CA_Slot_ShowPage();
		break;
	case GUI_VK_DOWN:
	case GUI_VK_P_DOWN:
	case GUI_VK_RIGHT:
		if(g_slot_page <= 1)return;
		g_slot_curPage ++;
		CA_Slot_ShowPage();
		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_GetServicer, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;
	}
}

