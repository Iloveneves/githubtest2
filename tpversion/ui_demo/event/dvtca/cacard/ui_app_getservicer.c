#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

extern BU08 g_ui_app_str[11][80];
static BU08 *g_ca_str2_p[5]=
{
	g_ui_app_str[6],
	g_ui_app_str[7],
	g_ui_app_str[8],
	g_ui_app_str[9],
	g_ui_app_str[10]
};

static BU08 service_title[21];
static BU08 g_servicer_tvsnum = 5;
static FYF_CA_TvsInfo g_servicer_tvsId[5];

void UI_APP_IPI_GetTvsId(FYF_CA_TvsInfo* g_ser_tvsid)
{
	GUI_API_Memcpy(g_ser_tvsid, g_servicer_tvsId, 5*sizeof(FYF_CA_TvsInfo));
}

void UI_APP_CA_GetServicer_Enter(void)
{
	BU32 i,index;
	BU32 lang;
	BU32 menu_id;
	BS32 ret;
	char *str;
	char **pLang;
	g_servicer_tvsnum = 5;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		str = DicDataEnglish[RS_TVS_ID];
		pLang = DicDataEnglish;
	}
	else
	{
		str = DicData[RS_TVS_ID];
		pLang = DicData;
	}

	GUI_ENC_API_GetValue(ID_MNU_Ca,WDT_MNU_SV_CURDATAID,&menu_id,0);	
	if(menu_id == RS_EntitleInfo)
	{
		GUI_ENC_API_SetValue(ID_STA_OK_GetServicer, WDT_STA_SV_DATAID, RS_ReadEntitle, 0);
		GUI_ENC_API_SetValue(ID_STA_Menu_GetServicer, WDT_STA_SV_DATAID, RS_EntitleInfo, 0);
	}
	
	GUI_ENC_API_SetValue(ID_TIM_GetServicer, WDT_TIM_SV_STOP, 0, 0);
	ret = FYF_API_ca_get_value(FYF_CA_SV_OPERATOR_INFO,(BU32 *)g_servicer_tvsId,(BU32*)&g_servicer_tvsnum);

	if(ret != FYF_OK)
	{
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Line1_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetValue(ID_LBX_GetServicer, WDT_LBX_SV_ALLNUM, 0, 0);
		GUI_ENC_API_SetValue(ID_STA_Line1_GetServicer, WDT_STA_SV_DATAID, RS_GetFailed, 0);
	}
	else
	{		
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_GetServicer, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_GetServicer, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Line1_GetServicer, WS_DISABLE);
		for(i=0;i<g_servicer_tvsnum;i++)
		{
			sprintf(*(g_ca_str2_p+i),"%s: %d  %s",str,g_servicer_tvsId[i].m_wTVSID,g_servicer_tvsId[i].m_szTVSName);			
		}
		GUI_ENC_API_SetValue(ID_LBX_GetServicer, WDT_LBX_SV_DATA, (BU32)g_ca_str2_p, g_servicer_tvsnum);
		GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &i, 0);
		if(i>=g_servicer_tvsnum)i=0;
		GUI_ENC_API_SetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, i, 0);
	}
	APP_Com_API_SetFullScreen();
}

void UI_APP_CA_GetServicer_End(void)
{

}

void UI_APP_CA_GetServicer_Key(BU32 key)
{
	BU32 i;
	BU32 lang;
	char **pDic = DicData;
	BU32 menu_id;
	GUI_ENC_API_GetValue(ID_MNU_Ca,WDT_MNU_SV_CURDATAID,&menu_id,0);
	
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	
	if(GUI_VK_SELECT == key && g_servicer_tvsnum)
	{
		GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &i, 0);
		sprintf(service_title,"%s",g_servicer_tvsId[i].m_szTVSName);
		GUI_ENC_API_SetValue(ID_LBX_GetServicer, WDT_LBX_SV_DATA, 0, 0);
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Line1_GetServicer, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_NoteBk_GetServicer, 1);
		GUI_ENC_API_Update(ID_STA_Note_GetServicer, 1);
		GUI_ENC_API_SetValue(ID_STA_Line1_GetServicer, WDT_STA_SV_DATAID, RS_Wait, 0);
		GUI_ENC_API_SetValue(ID_TIM_GetServicer, WDT_TIM_SV_PLAY, 0, 0);
	}
	else if(GUI_VK_RECALL == key || GUI_VK_MENU== key)
	{
		GUI_ENC_API_SwitchState(ID_DLG_Ca,1);	
	}
	else if(GUI_VK_EXIT== key)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);	
	}
}	

void UI_APP_CA_GetServicer_Timer(void)
{
	BU32 menu_id;
	GUI_ENC_API_GetValue(ID_MNU_Ca,WDT_MNU_SV_CURDATAID,&menu_id,0);	
	if(menu_id == RS_EntitleInfo)
	{
		GUI_ENC_API_SetValue(ID_STA_Menu_EntitleInfo, WDT_STA_SV_STRING, (BU32)service_title, 0);
		GUI_ENC_API_SwitchState(ID_DLG_EntitleInfo, 1);
	}
}


