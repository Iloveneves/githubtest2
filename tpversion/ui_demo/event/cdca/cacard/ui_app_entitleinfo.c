#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define CA_ENTITLE_INFO_NUM  8

extern BU08 g_ui_app_str[13][80];

//��ѯ��Ȩ��Ϣ
static BU32 g_EntitleInfo_tvsnum=0;
static BU32 g_EntitleInfo_curPage = 0;
static BU32 g_EntitleInfo_page = 0;
static BU32 g_EntitleInfo_rest = 0;

FYF_CDCA_Entitles_s g_cdca_entitles;

BU16* UI_APP_IPI_GetTvsId();
void CA_EntitleInfo_ShowPage(void)
{
	BU32 i,num,index;
	BU08 str[20];
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

	if(g_EntitleInfo_tvsnum==0 )
	{
		for(i=0;i<10;i++)
		{
			sprintf(g_ui_app_str[i]," ");
		}
		GUI_ENC_API_SetValue(ID_STA_Text_EntitleInfo, WDT_STA_SV_DATAID, RS_NoServicerInfo, 0);
	}
	else if(g_EntitleInfo_tvsnum==0xFFFFFFFF)
	{
		for(i=0;i<10;i++)
		{
			sprintf(g_ui_app_str[i]," ");
		}
		GUI_ENC_API_SetValue(ID_STA_Text_EntitleInfo, WDT_STA_SV_DATAID, RS_GetFailed, 0);
	}
	else
	{
		if(g_EntitleInfo_curPage > g_EntitleInfo_page-1)g_EntitleInfo_curPage = 0;
		if(g_EntitleInfo_curPage == g_EntitleInfo_page-1)
		{
			num = g_EntitleInfo_rest;
		}
		else
		{
			num = CA_ENTITLE_INFO_NUM;
		}
		
		sprintf(g_ui_app_str[0],"%-10s %-12s %s",pDic[RS_ProductID],pDic[RS_EntitleEndTime],pDic[RS_TapeFlag]);
		for(i=0;i<num;i++)
		{
			index = g_EntitleInfo_curPage*CA_ENTITLE_INFO_NUM + i;
			UI_APP_GetDateStrFromValC_TF(g_cdca_entitles.m_Entitles[index].m_tExpireDate,str);
			sprintf(g_ui_app_str[i+1],"%-10u %-12s %s",g_cdca_entitles.m_Entitles[index].m_dwProductID,str,g_cdca_entitles.m_Entitles[index].m_bCanTape?pDic[RS_Yes]:pDic[RS_No]);
	 	}
		for(i=num;i<CA_ENTITLE_INFO_NUM;i++)
		{
			sprintf(g_ui_app_str[i+1]," ");
		}
		sprintf(g_ui_app_str[9],"%s:%d    %d/%d",pDic[RS_ProdAmount],g_EntitleInfo_tvsnum,g_EntitleInfo_curPage+1,g_EntitleInfo_page);
	}	
	GUI_ENC_API_SetValue(ID_STA_Line0_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
	GUI_ENC_API_SetValue(ID_STA_Line4_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[4], 0);
	GUI_ENC_API_SetValue(ID_STA_Line5_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[5], 0);
	GUI_ENC_API_SetValue(ID_STA_Line6_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[6], 0);
	GUI_ENC_API_SetValue(ID_STA_Line7_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[7], 0);
	GUI_ENC_API_SetValue(ID_STA_Line8_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[8], 0);
	GUI_ENC_API_SetValue(ID_STA_Line9_EntitleInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[9], 0);

}

void UI_APP_CA_EntitleInfo_Enter(void)
{
	BU32 i;
	BU16* tvsid;
	BU32 index,num;
	BU32 id;
	BS32 ret;

	tvsid = UI_APP_IPI_GetTvsId();
	GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &index, 0);
 	ret = FYF_API_ca_get_value(FYF_CA_SV_SEVICE_ENTITLE,(BU32 *)(&tvsid[index]),(BU32 *)&g_cdca_entitles);
	if(ret == FYF_OK)
	{
		g_EntitleInfo_tvsnum = g_cdca_entitles.m_wProductCount;
	}
	else
	{
		g_EntitleInfo_tvsnum = 0xFFFFFFFF;
	}
	
	if(g_EntitleInfo_tvsnum)
	{
		GUI_ENC_API_SetVisible(ID_STA_TextBk_EntitleInfo, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_EntitleInfo, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_EntitleInfo, WS_DISABLE);
		
		g_EntitleInfo_page = (g_EntitleInfo_tvsnum +CA_ENTITLE_INFO_NUM-1)/CA_ENTITLE_INFO_NUM;
		g_EntitleInfo_rest = (g_EntitleInfo_tvsnum%CA_ENTITLE_INFO_NUM);
		if(!g_EntitleInfo_rest)g_EntitleInfo_rest = CA_ENTITLE_INFO_NUM;
		g_EntitleInfo_curPage = 0;
 	}
	else
	{
		g_EntitleInfo_curPage = 0;
		g_EntitleInfo_rest = 0;
		g_EntitleInfo_page = 0;
		GUI_ENC_API_SetVisible(ID_STA_TextBk_EntitleInfo, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_EntitleInfo, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_EntitleInfo, WS_VISIBLE);
	}
	CA_EntitleInfo_ShowPage();
}

void UI_APP_CA_EntitleInfo_End(void)
{

}
void UI_APP_CA_EntitleInfo_Key(BU32 key)
{
	switch(key)
	{
	//case GUI_VK_UP:
	case GUI_VK_P_UP:
	case GUI_VK_LEFT:
		if(g_EntitleInfo_page <= 1)return;
		if(g_EntitleInfo_curPage == 0)
		{
			g_EntitleInfo_curPage = g_EntitleInfo_page - 1;
		}
		else
		{
			g_EntitleInfo_curPage--;
		}
		CA_EntitleInfo_ShowPage();
		break;
	//case GUI_VK_DOWN:
	case GUI_VK_P_DOWN:
	case GUI_VK_RIGHT:
		if(g_EntitleInfo_page <= 1)return;
		g_EntitleInfo_curPage ++;
		CA_EntitleInfo_ShowPage();
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

