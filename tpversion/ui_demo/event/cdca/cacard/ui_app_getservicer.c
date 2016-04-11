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

static BU08 service_title[20];
static BU32 g_servicer_tvsnum = 5;
static BU16 g_servicer_tvsId[4];
BU16* UI_APP_IPI_GetTvsId()
{
	return g_servicer_tvsId;
}

void UI_APP_CA_GetServicer_Enter(void)
{
	BU32 i,index;
	BU32 lang;
	BU32 menu_id;
	BS32 ret;
	char *str;
	char **pLang;
	g_servicer_tvsnum = 0;
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
	else if(menu_id == RS_SlotsInfo)
	{
		GUI_ENC_API_SetValue(ID_STA_OK_GetServicer, WDT_STA_SV_DATAID, RS_ReadSlotInfo, 0);
		GUI_ENC_API_SetValue(ID_STA_Menu_GetServicer, WDT_STA_SV_DATAID, RS_SlotsInfo, 0);
	}
	else if(menu_id == RS_IPPInfo)
	{
		GUI_ENC_API_SetValue(ID_STA_OK_GetServicer, WDT_STA_SV_DATAID, RS_Find, 0);
		GUI_ENC_API_SetValue(ID_STA_Menu_GetServicer, WDT_STA_SV_DATAID, RS_IPPInfo, 0);		
	}
	else if(menu_id == RS_ACList)
	{
		GUI_ENC_API_SetValue(ID_STA_OK_GetServicer, WDT_STA_SV_DATAID, RS_Find, 0);
		GUI_ENC_API_SetValue(ID_STA_Menu_GetServicer, WDT_STA_SV_DATAID, RS_ACList, 0);		
	}
	else if(menu_id == RS_Detitle_Ack_Code)
	{
		GUI_ENC_API_SetValue(ID_STA_OK_GetServicer, WDT_STA_SV_DATAID, RS_Find, 0);
		GUI_ENC_API_SetValue(ID_STA_Menu_GetServicer, WDT_STA_SV_DATAID, RS_Detitle_Ack_Code, 0);		
	}
	
	GUI_ENC_API_SetValue(ID_TIM_GetServicer, WDT_TIM_SV_STOP, 0, 0);
	ret = FYF_API_ca_get_value(FYF_CA_SV_OPERATOR_INFO,(BU32 *)g_servicer_tvsId,0);

	if(ret != FYF_CDCA_RC_OK)
	{
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Line1_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetValue(ID_LBX_GetServicer, WDT_LBX_SV_ALLNUM, 0, 0);
		GUI_ENC_API_SetValue(ID_STA_Line1_GetServicer, WDT_STA_SV_DATAID, RS_GetFailed, 0);
	}
	else
	{
		for(index = 0;index<4;index++)
		{
			if(g_servicer_tvsId[index])
			{
				g_servicer_tvsnum++;
			}
			else break;
		}
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_GetServicer, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_GetServicer, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Line1_GetServicer, WS_DISABLE);
		for(i=0;i<g_servicer_tvsnum;i++)
		{
			if(menu_id == RS_Detitle_Ack_Code)
			{
				ret = FYF_API_ca_get_value (FYF_CA_SV_DETITLE_READED, (BU32 *)&(g_servicer_tvsId[i]) , 0);
				if(FYF_TRUE == ret)
				{
					sprintf(*(g_ca_str2_p+i),"%-15d%-11d%s",i+1,g_servicer_tvsId[i],pLang[RS_Readed]);
				}
				else if(FYF_FALSE == ret)
				{
					sprintf(*(g_ca_str2_p+i),"%-15d%-11d%s",i+1,g_servicer_tvsId[i],pLang[RS_UnRead]);
				}
				sprintf(g_ui_app_str[10],"%-15s%-12s%s",pLang[RS_SeqNum],pLang[RS_TVS_ID],pLang[RS_MailStatus]);
			}
			else
			{
				sprintf(*(g_ca_str2_p+i),"%-15d%-11d",i+1,g_servicer_tvsId[i]);
				sprintf(g_ui_app_str[10],"%-15s%-12s",pLang[RS_SeqNum],pLang[RS_TVS_ID]);
			}
		}
		GUI_ENC_API_SetValue(ID_STA_Status_GetServicer, WDT_STA_SV_STRING, (BU32)g_ui_app_str[10], 0);
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
		sprintf(service_title,"%s:%d",pDic[RS_TVS_ID],g_servicer_tvsId[i]);
		GUI_ENC_API_SetValue(ID_LBX_GetServicer, WDT_LBX_SV_DATA, 0, 0);
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_GetServicer, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Line1_GetServicer, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_NoteBk_GetServicer, 1);
		GUI_ENC_API_Update(ID_STA_Note_GetServicer, 1);
		GUI_ENC_API_SetValue(ID_STA_Line1_GetServicer, WDT_STA_SV_DATAID, RS_Wait, 0);
		GUI_ENC_API_SetValue(ID_TIM_GetServicer, WDT_TIM_SV_PLAY, 0, 0);
	}
	else if(GUI_VK_EXIT == key)
	{
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo,1);	
	}
	else if(GUI_VK_RECALL== key)
	{
		GUI_ENC_API_SwitchState(ID_DLG_Ca,1);	
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
	else if(menu_id == RS_SlotsInfo)
	{
		GUI_ENC_API_SetValue(ID_STA_Menu_ePurse, WDT_STA_SV_STRING, (BU32)service_title, 0);
		GUI_ENC_API_SwitchState(ID_DLG_ePurse, 1);
	}
	else if(menu_id == RS_IPPInfo)
	{
		GUI_ENC_API_SetValue(ID_STA_Menu_IPPInfo, WDT_STA_SV_STRING, (BU32)service_title, 0);
		GUI_ENC_API_SwitchState(ID_DLG_IPPInfo, 1);
	}
	else if(menu_id == RS_ACList)
	{
		GUI_ENC_API_SetValue(ID_STA_Servicer_ACList, WDT_STA_SV_STRING, (BU32)service_title, 0);
		GUI_ENC_API_SwitchState(ID_DLG_ACList, 1);
	}
	else if(menu_id == RS_Detitle_Ack_Code)
	{
		GUI_ENC_API_SetValue(ID_STA_ReserveList_DetitleChkNums, WDT_STA_SV_STRING, (BU32)service_title, 0);
		GUI_ENC_API_SwitchState(ID_DLG_DetitleChkNums, 1);
	}
#if 0
	else if(menu_id == RS_MasterSlave)
	{
		GUI_ENC_API_SetValue(ID_STA_MasterSlave, WDT_STA_SV_STRING, (BU32)service_title, 0);
		GUI_ENC_API_SwitchState(ID_DLG_MasterSlave, 1);
	}
	else if(menu_id == RS_View_Detitle_Ack_Code)
	{
		GUI_ENC_API_SetValue(ID_STA_ReserveList_DetitleChkNums, WDT_STA_SV_STRING, (BU32)service_title, 0);
		GUI_ENC_API_SwitchState(ID_DLG_DetitleChkNums, 1);
	}	
	else if(menu_id == RS_ViewACList)
	{
		GUI_ENC_API_SetValue(ID_STA_Servicer_ACList, WDT_STA_SV_STRING, (BU32)service_title, 0);
		GUI_ENC_API_SwitchState(ID_DLG_ACList, 1);
	}	
#endif
}

