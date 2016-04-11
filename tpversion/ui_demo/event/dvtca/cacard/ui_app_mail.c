#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"

#define G_MAIL_NUM_IN_PAGE (6)

extern BU08 g_ui_app_str[13][80];
/*Mail*/
static BU32 g_mail_allCount = 0;
static BU08 g_mail_delAll = 0xff;//0:删除一个 1:删除全部 2:非本卡邮件

static FYF_CA_EmailHead_s g_mail_head[50];
static FYF_CA_EmailContent_s g_mail_content;

static BU32 g_mail_pageNum = 0;
static BU32 g_mail_curPage = 0;
static BU32 g_mail_numOfLastPage = 0;
static BU08 g_mail_item[6][60];
static BU08 *g_mail_item_str[6]=
{
	g_mail_item[0],
	g_mail_item[1],
	g_mail_item[2],
	g_mail_item[3],
	g_mail_item[4],
	g_mail_item[5]
};

void UI_APP_Mail_ShowCurPage(void)
{
	BU32 index = 0;
	BU32 itemIndex = 0;
	BU32 j = 0;
	BU32 itemNum = 0;
	BU08 titleTmp[12];
	BU08 CreateTime[20];
	BU08 newStr[7]={0};
	BU32 lang;
	char **pDic = DicData;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	
	if(g_mail_curPage >= g_mail_pageNum)
	{
		g_mail_curPage = 0;
	}

	itemNum = 0;
	for(index = 0;index<G_MAIL_NUM_IN_PAGE;index++)
	{
		itemIndex = g_mail_curPage*G_MAIL_NUM_IN_PAGE+index;
		if(itemIndex<g_mail_allCount)
		{
			/*mail title*/
			if(GUI_API_Strlen(g_mail_head[itemIndex].m_szEmailHead) > 11)
			{
				GUI_API_Memcpy(titleTmp, g_mail_head[itemIndex].m_szEmailHead, 8);
				GUI_API_Memset(titleTmp+8, '.', 3);
			}
			else
			{
				GUI_API_Memcpy(titleTmp, g_mail_head[itemIndex].m_szEmailHead, 11);
			}
			titleTmp[11] = 0;
			
			/*mail create time*/
			UI_APP_GetTimeStrFromValC(g_mail_head[itemIndex].m_tCreateTime, CreateTime);
			CreateTime[19]=0;

			/*read flag*/
			if(g_mail_head[itemIndex].m_bNewEmail)
			{
				GUI_API_Strcpy(newStr, pDic[RS_UnRead]);
			}
			else
			{
				GUI_API_Strcpy(newStr, pDic[RS_Readed]);
			}
			sprintf(g_mail_item_str[index]," %-3d %-11s %-22s %-6s %s",itemIndex+1,titleTmp,CreateTime,newStr,g_mail_head[itemIndex].m_szSender);
			itemNum++;
		}
	}
	GUI_ENC_API_SetValue (ID_LBP_Mail, WDT_LBP_SV_DATA, (BU32)g_mail_item_str, itemNum);
}

void UI_APP_CA_Mail_Reverser(FYF_CA_EmailHead_s *pHead,BU32 num)
{
	FYF_CA_EmailHead_s tmp;
	BU32 i,j,len;

	if(num == 0)
	{
		return;
	}
	i = 0;
	j = num-1;
	len = sizeof(FYF_CA_EmailHead_s);
	while(i<j)
	{
		FYF_API_Memcpy(&tmp, pHead+i, len);
		FYF_API_Memcpy(pHead+i, pHead+j, len);
		FYF_API_Memcpy(pHead+j, &tmp, len);
	 	i++;
		j--;
	}
	
}

void UI_APP_CA_Mail_Enter(void)
{
	BU32 newCount=0;
	BU32 realMailCount=0;
	BU32 lang,i;
	char **pDic = DicData;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	GUI_ENC_API_SetValue(ID_STA_MaxNum_Mail, WDT_STA_SV_DATAID, RS_DVTMaxMailNum, 0);
	GUI_ENC_API_SetValue(ID_STA_Level_Mail, WDT_STA_SV_DATAID, RS_Sender, 0);
	FYF_API_Memset(&g_mail_head, 0, sizeof(FYF_CA_EmailHead_s));
	FYF_API_ca_get_value(FYF_CA_SV_EMAIL_COUNT,&realMailCount,&newCount);
	FYF_API_ca_get_value(FYF_CA_SV_MAIL_HEAD,(BU32*)g_mail_head,(BU32*)&realMailCount);
	UI_APP_CA_Mail_Reverser(g_mail_head, realMailCount);
	
	if(realMailCount == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_Mail, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValueUnPaint (ID_LBP_Mail, WDT_LBP_SV_INDEX, 0, 0);					
		g_mail_curPage=0;
		g_mail_allCount=0;
		GUI_ENC_API_SetVisible(ID_SBR_Mail, WS_DISABLE);		
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_Mail, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_Mail, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Notetitle_Mail, WS_VISIBLE);
		GUI_ENC_API_SetValue(ID_STA_Note_Mail, WDT_STA_SV_DATAID, RS_NoMail, 0);
		GUI_ENC_API_SetValue(ID_STA_MailNum_Mail, WDT_STA_SV_STRING, (BU32)"", 0);
	}
	else
	{

		GUI_ENC_API_SetVisible(ID_SBR_Mail, WS_VISIBLE);		
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_Mail, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_Mail, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Notetitle_Mail, WS_DISABLE);
		
		sprintf(g_ui_app_str[0],"%s: %d  %s: %d",pDic[RS_TotalMails],realMailCount,pDic[RS_NewMails],newCount);
		GUI_ENC_API_SetValue(ID_STA_MailNum_Mail, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);

		g_mail_allCount = (realMailCount>50)?50:realMailCount;
		g_mail_pageNum = (g_mail_allCount-1)/G_MAIL_NUM_IN_PAGE+1;
		g_mail_numOfLastPage = g_mail_allCount%G_MAIL_NUM_IN_PAGE;
		if(g_mail_numOfLastPage == 0)
		{
			g_mail_numOfLastPage = G_MAIL_NUM_IN_PAGE;
		}
		GUI_ENC_API_SetValue (ID_SBR_Mail, WDT_SBR_SV_MAX, g_mail_allCount-1, 0);
		GUI_ENC_API_SetValue (ID_SBR_Mail, WDT_SBR_SV_MIN, 0, 0);
		GUI_ENC_API_SetValue (ID_SBR_Mail, WDT_SBR_SV_VALUE, 0, 0);
		UI_APP_Mail_ShowCurPage ();
	}
	APP_Com_API_SetFullScreen();
	GUI_ENC_API_SetValue(ID_TIM_Mail, WDT_TIM_SV_STOP, 0, 0);
}

void UI_APP_CA_Mail_End(void)
{

}
void UI_APP_CA_Mail_Key(BU32 key)
{
	BU32 index,itemIndex;
	BU32 remainder = 0;
	GUI_ENC_API_GetValue(ID_LBP_Mail, WDT_LBP_SV_INDEX, &index, 0);
	itemIndex = index+g_mail_curPage*G_MAIL_NUM_IN_PAGE;
	switch(key)
	{
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(UI_APP_CA_getMailPreState(), 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_SELECT:
		if(!g_mail_allCount)return;
		GUI_ENC_API_SetFocus (ID_STA_Note_Mail);
		GUI_ENC_API_SetValue(ID_STA_Note_Mail, WDT_STA_SV_DATAID, RS_Wait, 0);
		GUI_ENC_API_SetVisible(ID_STA_NoteBk_Mail, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Note_Mail, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_NoteBk_Mail ,1);
		GUI_ENC_API_Update(ID_STA_Note_Mail ,1);
		GUI_ENC_API_SetValue(ID_TIM_Mail, WDT_TIM_SV_PLAY, 0, 0);
		GUI_ENC_API_SwitchState(ID_DLG_ReadMail, 1);
		break;
	case GUI_VK_YELLOW:
		if(!g_mail_allCount)return;

		g_mail_delAll = 0;
		GUI_ENC_API_SwitchState(ID_DLG_DelCurrent, 1);
		break;
	case GUI_VK_RED:
		if(!g_mail_allCount)return;

		g_mail_delAll = 1;
		GUI_ENC_API_SwitchState(ID_DLG_DelCurrent, 1);
		break;
	default:
		break;

	}
	GUI_ENC_API_SetValue (ID_SBR_Mail,WDT_SBR_SV_VALUE, itemIndex, 0);
}
void UI_APP_CA_Mail_Change(void)
{
	BS32 state;

	GUI_ENC_API_GetValue(ID_LBP_Mail, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_mail_curPage++;
			break;
		case -1:
			if(g_mail_curPage == 0)
				g_mail_curPage = g_mail_pageNum -1;
			else
				g_mail_curPage--;
			
			if(g_mail_curPage == g_mail_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_Mail, WDT_LBP_SV_INDEX, G_MAIL_NUM_IN_PAGE-1, 0);
			}
			break;
		default:
			break;
	}
	UI_APP_Mail_ShowCurPage();//显示当前页

}

void UI_APP_CA_ReadMail_Enter(void)
{
	BU32 len;
	BU32 index = 0;
	BU32 lang;
	char **pDic = DicData;
	BU32 num;
	BU32 itemIndex = 0;
	BU08 CreateTime[20]={0};
	BS32 ret;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	GUI_ENC_API_GetValue(ID_LBP_Mail, WDT_LBP_SV_INDEX, &index, 0);
	itemIndex = g_mail_curPage*G_MAIL_NUM_IN_PAGE+index;
	/*title*/
	sprintf(g_ui_app_str[0],"%s: %s",pDic[RS_Title],g_mail_head[itemIndex].m_szEmailHead);
	GUI_ENC_API_SetValue(ID_STA_Line1_ReadMail, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	/*create time*/
	UI_APP_GetTimeStrFromValC(g_mail_head[itemIndex].m_tCreateTime, CreateTime);
	CreateTime[19]=0;
	sprintf(g_ui_app_str[1],"%s: %s",pDic[RS_SendTime],CreateTime);
	GUI_ENC_API_SetValue(ID_STA_Line2_ReadMail, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	/*sender*/
	if(g_mail_head[itemIndex].m_szSender)
	{
		sprintf(g_ui_app_str[2],"%s: %s",pDic[RS_Sender],g_mail_head[itemIndex].m_szSender);
	}
	else
	{
		sprintf(g_ui_app_str[2],"");	
	}
	GUI_ENC_API_SetValue(ID_STA_Line3_ReadMail, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	
	GUI_API_Memset(g_mail_content.m_szEmail, 0, 1025);
	ret=FYF_API_ca_get_value(FYF_CA_SV_MAIL_BODY,(BU32*)&(g_mail_head[itemIndex].m_dwActionID),(BU32*)&g_mail_content);
	if(ret==FYF_OK)
	{
		len = GUI_API_Strlen(g_mail_content.m_szEmail);
		APP_Printf("len=%d,  mail_content=%s\n",len, g_mail_content.m_szEmail);
		GUI_ENC_API_SetValue(ID_EDT_ReadMail, WDT_EDT_SV_VALUE, (BU32)g_mail_content.m_szEmail, len);
		GUI_ENC_API_SetValue(ID_EDT_ReadMail, WDT_EDT_SV_PAGE_INDEX, 0, 0);
		if(len)
		{
			GUI_ENC_API_GetValue(ID_EDT_ReadMail, WDT_EDT_SV_PAGE_NUM, &num, 0);
			sprintf(g_ui_app_str[3],"1 / %d",num);
			GUI_ENC_API_SetValue(ID_STA_Page_ReadMail, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_STA_Page_ReadMail, WDT_STA_SV_STRING, (BU32)"", 0);
		}
		FYF_API_ca_set_value(FYF_CA_SV_EMAIL_READED,(BU32)(g_mail_head[itemIndex].m_dwActionID),0);
	}
	else if(g_mail_content.m_wEmailLength!=0)
	{
		g_mail_delAll=2;
		GUI_ENC_API_SwitchState(ID_DLG_DelCurrent, 1);
	}	
}
void UI_APP_CA_ReadMail_End(void)
{
	GUI_ENC_API_SetValue(ID_EDT_ReadMail, WDT_EDT_SV_VALUE, 0, 0);
	GUI_ENC_API_SetValue(ID_EDT_ReadMail, WDT_EDT_SV_PAGE_INDEX, 0, 0);	
	GUI_ENC_API_SetValue(ID_STA_Page_ReadMail, WDT_STA_SV_STRING, " ", 0);
}
void UI_APP_CA_ReadMail_Key(BU32 key)
{
	BU32 index;
	BU32 num;

	switch(key)
	{
//	case GUI_VK_LEFT:
//	case GUI_VK_RIGHT:
	case GUI_VK_UP:
	case GUI_VK_DOWN:
	case GUI_VK_P_DOWN:
	case GUI_VK_P_UP:
		GUI_ENC_API_GetValue(ID_EDT_ReadMail, WDT_EDT_SV_PAGE_INDEX, &index, 0);
		GUI_ENC_API_GetValue(ID_EDT_ReadMail, WDT_EDT_SV_PAGE_NUM, &num, 0);
		sprintf(g_ui_app_str[3],"%d / %d",index+1,num);
		GUI_ENC_API_SetValue(ID_STA_Page_ReadMail, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
		
		break;
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(ID_DLG_Mail, 1);
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
	default:
		break;
	}

}
void UI_APP_CA_DelMail_Enter(void)
{
	if(g_mail_delAll==1)
	{
		GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent, WDT_STA_SV_DATAID, RS_IfDelAll, 0);
		GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent, WDT_STA_SV_DATAID, RS_Del_Exit, 0);		
	}
	else if(g_mail_delAll==0)
	{
		GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent, WDT_STA_SV_DATAID, RS_IfDelOne, 0);		
		GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent, WDT_STA_SV_DATAID, RS_Del_Exit, 0);		
	}
	else if(g_mail_delAll==2)
	{
		GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent, WDT_STA_SV_STRING, "非本卡邮件，不能阅读", 0);		
		GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent, WDT_STA_SV_DATAID, RS_Exit, 0);		
	}
}
void UI_APP_CA_DelMail_End(void)
{

}
void UI_APP_CA_DelMail_Key(BU32 key)
{
	BU32 index = 0;
	BS32 ret;
	BU32 temp;
	BU32 itemIndex;
	
	if(key == GUI_VK_SELECT)
	{
		if(g_mail_delAll==1)
		{
			ret=FYF_API_ca_set_value(FYF_CA_SV_EMAIL_DEL,-1,0);

			if(ret == FYF_OK)
			{			
				GUI_ENC_API_SwitchState(ID_DLG_Mail, 1);
			}
			else if(ret == FYF_CAERR_STB_NO_EMAIL)
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent, WDT_STA_SV_STRING, "非本卡邮件，不能删除", 0);
				GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent, WDT_STA_SV_DATAID, RS_Exit, 0);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent, WDT_STA_SV_DATAID, RS_DelFail, 0);
				GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent, WDT_STA_SV_DATAID, RS_Exit, 0);
			}
		}
		else if(g_mail_delAll==0)
		{
			GUI_ENC_API_GetValue(ID_LBP_Mail, WDT_LBP_SV_INDEX, &index, 0);
			itemIndex = g_mail_curPage*G_MAIL_NUM_IN_PAGE+index;
			temp = g_mail_head[itemIndex].m_dwActionID;
			ret = FYF_API_ca_set_value(FYF_CA_SV_EMAIL_DEL,temp,0);
			if(ret == FYF_OK)
			{
				if(itemIndex+1 == g_mail_allCount)
				{
					if(index == 0)
					{
						if(g_mail_pageNum>1)
						{
							GUI_ENC_API_SetValueUnPaint (ID_LBP_Mail, WDT_LBP_SV_INDEX, G_MAIL_NUM_IN_PAGE-1, 0);
							g_mail_curPage--;
						}
						else
						{
							GUI_ENC_API_SetValueUnPaint (ID_LBP_Mail, WDT_LBP_SV_INDEX, 0, 0);					
							g_mail_curPage=0;
						}
					}
					else
					{
						GUI_ENC_API_SetValueUnPaint (ID_LBP_Mail, WDT_LBP_SV_INDEX, itemIndex-1, 0);
					}
				}
				GUI_ENC_API_SwitchState(ID_DLG_Mail, 1);
			}
			else if(ret == FYF_CAERR_STB_NO_EMAIL)
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent, WDT_STA_SV_STRING, "非本卡邮件，不能删除", 0);
				GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent, WDT_STA_SV_DATAID, RS_Exit, 0);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Line1_DelCurrent, WDT_STA_SV_DATAID, RS_DelFail, 0);
				GUI_ENC_API_SetValue(ID_STA_Line2_DelCurrent, WDT_STA_SV_DATAID, RS_Exit, 0);
			}
		}
	}
	else if(key ==GUI_VK_EXIT)
	{
		GUI_ENC_API_SwitchState(ID_DLG_Mail, 1);

	}
}

