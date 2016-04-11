#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

static BU32 g_ca_service_num=0;
FYF_CDCA_TVSACList_s g_acList;
extern BU08 g_ui_app_str[13][80];
BU16* tvsid;
BU16* UI_APP_IPI_GetTvsId();

void CA_acList_ShowPage(void)
{
	BU32 i,num,index;
	BU16 tvs_id = 0;
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

	if(g_ca_service_num==0 )
	{
		for(i=0;i<10;i++)
		{
			sprintf(g_ui_app_str[i]," ");
		}
		GUI_ENC_API_SetValue(ID_STA_Text_ACList, WDT_STA_SV_DATAID, RS_NoAcList_Info, 0);
	}
	else if(g_ca_service_num==0xFFFFFFFF)
	{
		for(i=0;i<10;i++)
		{
			sprintf(g_ui_app_str[i]," ");
		}
		GUI_ENC_API_SetValue(ID_STA_Text_ACList, WDT_STA_SV_DATAID, RS_GetFailed, 0);
	}
	else
	{
/*		if(g_ca_curPage > g_ca_page-1)g_ca_curPage = 0;
		if(g_ca_curPage == g_ca_page-1)
		{
			num = g_ca_rest;
		}
		else
		{
			num = CA_ENTITLE_INFO_NUM;
		}
		*/
		GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &i, 0);
		tvs_id = tvsid[i];	
		//sprintf(g_ui_app_str[0],"%s:%d",pDic[RS_AC],g_ca_service_num);
		sprintf(g_ui_app_str[0],"     ");
		
		for(i=0;i<9;i++)
		{
			sprintf(g_ui_app_str[i+1],"%s%d:%-10u %s%d:%-10u",pDic[RS_AC],i+1,g_acList.m_ACList[i],pDic[RS_AC],10+i,g_acList.m_ACList[9+i]);
	 	}
	}	
	
	GUI_ENC_API_SetValue(ID_STA_Line1_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
	GUI_ENC_API_SetValue(ID_STA_Line4_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[4], 0);
	GUI_ENC_API_SetValue(ID_STA_Line5_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[5], 0);
	GUI_ENC_API_SetValue(ID_STA_Line6_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[6], 0);
	GUI_ENC_API_SetValue(ID_STA_Line7_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[7], 0);
	GUI_ENC_API_SetValue(ID_STA_Line8_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[8], 0);
	GUI_ENC_API_SetValue(ID_STA_Line9_ACList, WDT_STA_SV_STRING, (BU32)g_ui_app_str[9], 0);
}

void UI_APP_CA_acList_Enter(void)
{
	BU32 i;
	BU32 index,num;
	BU32 id;
	BS32 ret;

	tvsid = UI_APP_IPI_GetTvsId();
	GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &index, 0);
 	ret = FYF_API_ca_get_value(FYF_CA_SV_ACLIST_INFO,(BU32 *)(&tvsid[index]),(BU32 *)&g_acList);
	if(ret == FYF_OK)
	{
		g_ca_service_num = g_acList.m_acCount;
	}
	else
	{
		g_ca_service_num = 0xFFFFFFFF;
	}
	
	if(g_ca_service_num)
	{
		GUI_ENC_API_SetVisible(ID_STA_TextBk_ACList, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_ACList, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ACList, WS_DISABLE);
		
/*		g_ca_page = (g_ca_service_num +CA_ENTITLE_INFO_NUM-1)/CA_ENTITLE_INFO_NUM;
		g_ca_rest = (g_ca_service_num%CA_ENTITLE_INFO_NUM);
		if(!g_ca_rest)g_ca_rest = CA_ENTITLE_INFO_NUM;
		g_ca_curPage = 0;*/
 	}
	else
	{
/*		g_ca_curPage = 0;
		g_ca_rest = 0;
		g_ca_page = 0;*/
		GUI_ENC_API_SetVisible(ID_STA_TextBk_ACList, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_ACList, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ACList, WS_VISIBLE);
	}
	CA_acList_ShowPage();
}

void UI_APP_CA_acList_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_GetServicer, 1);
		break;
	default:
		break;
	}
}


