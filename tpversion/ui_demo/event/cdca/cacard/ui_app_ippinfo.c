#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define CDCA_VIEWED_IPP_NUM_OF_PAGE		(8)
extern BU08 g_ui_app_str[13][80];
//≤È—ØIPP–≈œ¢
static FYF_CDCA_IppvInfo cdca_ippv_info[FYF_CDCA_MAXNUM_IPPVP];
static FYF_CDCA_IppvInfo_s g_cdca_ippv_info;
static BU32 g_ca_viewdipp_num=0;
static BU32 g_ca_viewdipp_page = 0;
static BU32 g_ca_viewdipp_rest = 0;
static BU32 g_ca_viewdipp_curPage = 0;

BU16* UI_APP_IPI_GetTvsId();
void UI_APP_CA_ShowPage(void)
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

	if(g_ca_viewdipp_num==0 )
	{
		for(i=0;i<10;i++)
		{
			sprintf(g_ui_app_str[i]," ");
		}
	}
	else
	{
		if(g_ca_viewdipp_curPage > g_ca_viewdipp_page-1)
		{
			g_ca_viewdipp_curPage = 0;
		}
		if(g_ca_viewdipp_curPage == g_ca_viewdipp_page-1)
		{
			num = g_ca_viewdipp_rest;
		}
		else
		{
			num = CDCA_VIEWED_IPP_NUM_OF_PAGE;
		}
		
		sprintf(g_ui_app_str[0],"%-10s %-6s %-8s %-8s %-12s %-10s",pDic[RS_ProductID],pDic[RS_SlotID],pDic[RS_ProdStatus],pDic[RS_TapeFlag],pDic[RS_EntitleEndTime],pDic[RS_ProdPrice]);
		for(i=0;i<num;i++)
		{
			index = g_ca_viewdipp_curPage*CDCA_VIEWED_IPP_NUM_OF_PAGE + i;
			UI_APP_GetDateStrFromValC_TF(cdca_ippv_info[index].m_wExpiredDate,str);
			sprintf(g_ui_app_str[i+1],"%-10u %-6u %-8s %-8s %-12s %-10u",cdca_ippv_info[index].m_dwProductID,cdca_ippv_info[index].m_bySlotID,((cdca_ippv_info[index].m_byBookEdFlag == 0x01)?pDic[RS_ProdBooking]:pDic[RS_ProdViewed]),((cdca_ippv_info[index].m_bCanTape)?pDic[RS_Yes]:pDic[RS_No]),str,cdca_ippv_info[index].m_wPrice);
	 	}
		for(i=num;i<CDCA_VIEWED_IPP_NUM_OF_PAGE;i++)
		{
			sprintf(g_ui_app_str[i+1]," ");
		}

		sprintf(g_ui_app_str[9],"%s:%d    %d/%d",pDic[RS_ProdAmount],g_ca_viewdipp_num,g_ca_viewdipp_curPage+1,g_ca_viewdipp_page);
	}	
	GUI_ENC_API_SetValue(ID_STA_Line0_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Line1_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
	GUI_ENC_API_SetValue(ID_STA_Line4_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[4], 0);
	GUI_ENC_API_SetValue(ID_STA_Line5_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[5], 0);
	GUI_ENC_API_SetValue(ID_STA_Line6_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[6], 0);
	GUI_ENC_API_SetValue(ID_STA_Line7_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[7], 0);
	GUI_ENC_API_SetValue(ID_STA_Line8_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[8], 0);
	GUI_ENC_API_SetValue(ID_STA_Line9_IPPInfo, WDT_STA_SV_STRING, (BU32)g_ui_app_str[9], 0);

}
void UI_APP_CA_ViewedIPP_Clear(FYF_CDCA_IppvInfo_s *pIppvInfo)
{
	BU32 index;
	BU32 num;
	BU32 len;
	
	index = 0;
	num = 0;
	len = sizeof(FYF_CDCA_IppvInfo_s);
	for(index = 0;index<pIppvInfo->wNumber;index++)
	{
		if(0x03 == pIppvInfo->pIppv[index].m_byBookEdFlag)
		{
			if(index != num)
			{
				FYF_API_Memcpy(pIppvInfo->pIppv+num, pIppvInfo->pIppv+index, len);
			}
			num++;
		}
	}
	pIppvInfo->wNumber = num;
}

void UI_APP_CA_ViewedIPP_Enter(void)
{
	BU32 index;
	BS32 ret;
	BU16* tvsid;
	tvsid = UI_APP_IPI_GetTvsId();
	g_cdca_ippv_info.pIppv = cdca_ippv_info;
	GUI_ENC_API_GetValue(ID_LBX_GetServicer, WDT_LBX_SV_INDEX, &index, 0);
	g_cdca_ippv_info.wTVSID = tvsid[index];
	g_cdca_ippv_info.wNumber = FYF_CDCA_MAXNUM_IPPVP;
	ret = FYF_API_ca_get_value (FYF_CA_SV_VIEWED_PPV, (BU32 *)&g_cdca_ippv_info, 0);
	UI_APP_CA_ViewedIPP_Clear(&g_cdca_ippv_info);
	if(FYF_CDCA_RC_OK == ret)
	{
		if(g_cdca_ippv_info.wNumber)
		{
			g_ca_viewdipp_num = g_cdca_ippv_info.wNumber;
			g_ca_viewdipp_page = (g_ca_viewdipp_num+CDCA_VIEWED_IPP_NUM_OF_PAGE-1)/CDCA_VIEWED_IPP_NUM_OF_PAGE;
			g_ca_viewdipp_curPage = 0;
			g_ca_viewdipp_rest = g_ca_viewdipp_num%CDCA_VIEWED_IPP_NUM_OF_PAGE;
			if(!g_ca_viewdipp_rest)
			{
				g_ca_viewdipp_rest = CDCA_VIEWED_IPP_NUM_OF_PAGE; 
			}
			GUI_ENC_API_SetVisible(ID_STA_TextBk_IPPInfo, WS_DISABLE);
			GUI_ENC_API_SetVisible(ID_STA_Text_IPPInfo, WS_DISABLE);
			GUI_ENC_API_SetVisible(ID_STA_Title_IPPInfo, WS_DISABLE);
		}
		else
		{
			g_ca_viewdipp_num = 0;
			GUI_ENC_API_SetVisible(ID_STA_TextBk_IPPInfo, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_STA_Text_IPPInfo, WS_VISIBLE);
			GUI_ENC_API_SetVisible(ID_STA_Title_IPPInfo, WS_VISIBLE);
			GUI_ENC_API_SetValueUnPaint(ID_STA_Text_IPPInfo, WDT_STA_SV_DATAID, RS_DataNotFind, 0);
		}
	}
	else if(FYF_CDCA_RC_CARD_INVALID == ret)
	{
		g_ca_viewdipp_num = 0;
		GUI_ENC_API_SetVisible(ID_STA_TextBk_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetValueUnPaint(ID_STA_Text_IPPInfo, WDT_STA_SV_DATAID, RS_CardPair_Invalid, 0);
	}
	else if(FYF_CDCA_RC_DATA_NOT_FIND == ret)
	{
		g_ca_viewdipp_num = 0;
		GUI_ENC_API_SetVisible(ID_STA_TextBk_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetValueUnPaint(ID_STA_Text_IPPInfo, WDT_STA_SV_DATAID, RS_DataNotFind, 0);
	}
	else
	{
		g_ca_viewdipp_num = 0;
		GUI_ENC_API_SetVisible(ID_STA_TextBk_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Text_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_IPPInfo, WS_VISIBLE);
		GUI_ENC_API_SetValueUnPaint(ID_STA_Text_IPPInfo, WDT_STA_SV_DATAID, RS_UnknowErr, 0);
	}
	UI_APP_CA_ShowPage ();
}
void UI_APP_CA_ViewedIPP_End(void)
{

}
void UI_APP_CA_ViewedIPP_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_P_UP:
	case GUI_VK_LEFT:
		if(g_ca_viewdipp_num == 0)return;
		if(g_ca_viewdipp_page <= 1)return;
		if(g_ca_viewdipp_curPage == 0)
		{
			g_ca_viewdipp_curPage = g_ca_viewdipp_page - 1;
		}
		else
		{
			g_ca_viewdipp_curPage--;
		}
		UI_APP_CA_ShowPage();
		break;
	case GUI_VK_DOWN:
	case GUI_VK_P_DOWN:
	case GUI_VK_RIGHT:
		if(g_ca_viewdipp_num == 0)return;
		if(g_ca_viewdipp_page <= 1)return;
		g_ca_viewdipp_curPage ++;
		UI_APP_CA_ShowPage();
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

