#include "CDCAS30.h"
#include "../../../tpversion/ui_demo/osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

struct{
	BU08 ipp_dlg_show;
	BU08 messageType;
	BU16 ecmPid;
	SCDCAIppvBuyInfo ippvProgram;
	BU32 ipp_mutex;
	BU32 retValue;
}CDCAS_ipp;
static BU08 ippretvalue = 0;
void CDSTBCA_IPI_BeforeShow(BU32 bUpdate);
void CDSTBCA_IPP_PROTECT(void)
{
	if(CDCAS_ipp.ipp_mutex)
	{
		
	}
}
void CDSTBCA_IPP_UNPROTECT(void)
{
	if(CDCAS_ipp.ipp_mutex)
	{
		
	}	
}
extern FYF_Ecm_Info_s serviceInfo_av;

BU32 CDSTBCA_IPI_VerifyEcm(BU16 ecmPid)
{
	if(ecmPid == serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid || ecmPid == serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid)
	{
		return FYF_TRUE;
	}
	else
	{
		return FYF_FALSE;
	}
}
//-------------------- IPPV应用 --------------------
void CDSTBCA_HideIPPVDlg(CDCA_U16 wEcmPid)
{
	CDSTBCA_IPP_PROTECT ();
	if(CDCAS_ipp.ipp_dlg_show == 0)
	{
		CDSTBCA_IPP_UNPROTECT ();
		return;
	}

	if(CDCAS_ipp.ecmPid != wEcmPid)
	{
		CDSTBCA_IPP_UNPROTECT ();
		return;
	}
	
	CDCAS_ipp.ipp_dlg_show = 0;	
	CDSTBCA_IPP_UNPROTECT ();

	/* 发送隐藏IPP提示框 */
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_IPP_HIDE, 0, 0);
}

void CDSTBCA_StartIppvBuyDlg(CDCA_U8 byMessageType, CDCA_U16 wEcmPid,  SCDCAIppvBuyInfo *pIppvProgram)
{
	CDSTBCA_IPP_PROTECT ();
	if(UI_APP_API_IsOsdShow ())
	{
		CDCASTB_StopIPPVBuyDlg(CDCA_FALSE,wEcmPid,NULL,NULL);
		return;
	}
	if(ippretvalue == 1)
	{
		FYF_API_thread_sleep (1000);
		ippretvalue = 0;
	}
	CDCAS_ipp.messageType = byMessageType;
	CDCAS_ipp.ecmPid = wEcmPid;
	//printf("CDCAS_ipp.m_byPriceNum=%d ,m_bySlotID=%d ,m_dwProductID=%d ,m_Price=%d ,m_wExpiredDate=%d ,m_wTvsID=%d ,wEcmPid=%d\n",pIppvProgram->m_byPriceNum,pIppvProgram->m_bySlotID,pIppvProgram->m_dwProductID,pIppvProgram->m_Price[0].m_wPrice,pIppvProgram->m_wExpiredDate,pIppvProgram->m_wTvsID,wEcmPid);
	
	FYF_API_Memcpy (&(CDCAS_ipp.ippvProgram), pIppvProgram, sizeof(SCDCAIppvBuyInfo));
	//printf("CDCAS_ipp.m_byPriceNum=%d ,m_bySlotID=%d ,m_dwProductID=%d ,m_Price=%d ,m_wExpiredDate=%d ,m_wTvsID=%d\n",CDCAS_ipp.ippvProgram.m_byPriceNum,CDCAS_ipp.ippvProgram.m_bySlotID,CDCAS_ipp.ippvProgram.m_dwProductID,CDCAS_ipp.ippvProgram.m_Price[0].m_wPrice,CDCAS_ipp.ippvProgram.m_wExpiredDate,CDCAS_ipp.ippvProgram.m_wTvsID);
	if(CDCAS_ipp.ipp_dlg_show)
	{
		CDSTBCA_IPI_BeforeShow (1);
		CDSTBCA_IPP_UNPROTECT ();
		return;
	}

	if(CDSTBCA_IPI_VerifyEcm (wEcmPid) == FYF_FALSE)
	{
		CDSTBCA_IPP_UNPROTECT ();
		return;		
	}
	
	CDCAS_ipp.ipp_dlg_show = 1;
	CDSTBCA_IPI_BeforeShow (0);
	CDSTBCA_IPP_UNPROTECT ();
	
	/* 发送显示IPP提示框 */
	GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_IPP_SHOW, 0, 0);
}

void CDSTBCA_API_InitIpp(void)
{
	FYF_API_Memset (&CDCAS_ipp, 0, sizeof(CDCAS_ipp));
}

BU08 cdcasca_ipp_price[2][100];

BU08 *pcdcasca_ipp_price[2]=
{
	cdcasca_ipp_price[0],
	cdcasca_ipp_price[1]
};
BU08 g_cdcas_str[13][80];

/*
	bUpdate: == 1	根据前端变化,更新已显示内容
			 == 0	显示内容前，初始化
*/
void CDSTBCA_IPI_BeforeShow(BU32 bUpdate)
{

	BU32 index;
	BU32 lang;
	BU32 price_num;
	BU08 tmp[30];
	BU08 *password = "......";
	BU08 ipp_type;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	price_num = (CDCAS_ipp.ippvProgram.m_byPriceNum>2)?2:(CDCAS_ipp.ippvProgram.m_byPriceNum);
	for(index = 0;index<price_num;index++)
	{
		if(CDCAS_ipp.ippvProgram.m_Price[index].m_byPriceCode == CDCA_IPPVPRICETYPE_TPPVVIEW)/* 不回传不可录像 */
		{
			if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
			{
				sprintf (cdcasca_ipp_price[index], "%d %s %d points", index+1,"TppvView",CDCAS_ipp.ippvProgram.m_Price[index].m_wPrice);
			}
			else
			{
				sprintf (cdcasca_ipp_price[index], "%d %s %d点", index+1,"不回传不可录像",CDCAS_ipp.ippvProgram.m_Price[index].m_wPrice);
			}
		}
		else if(CDCAS_ipp.ippvProgram.m_Price[index].m_byPriceCode == CDCA_IPPVPRICETYPE_TPPVVIEWTAPING)/* 不回传可录像 */
		{
			if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
			{
				sprintf (cdcasca_ipp_price[index], "%d %s %d points",index+1, "TppvViewTaping",CDCAS_ipp.ippvProgram.m_Price[index].m_wPrice);
			}
			else
			{
				sprintf (cdcasca_ipp_price[index], "%d %s   %d点", index+1,"不回传可录像",CDCAS_ipp.ippvProgram.m_Price[index].m_wPrice);
			}			
		}
	}
	if(bUpdate)
	{
		GUI_ENC_API_SetValue (ID_LBX_ipp_price_ChnInfo, WDT_LBX_SV_DATA, (BU32)pcdcasca_ipp_price, 2);
	}
	else
	{
		GUI_ENC_API_SetValueUnPaint (ID_LBX_ipp_price_ChnInfo, WDT_LBX_SV_DATA, (BU32)pcdcasca_ipp_price, 2);
		GUI_ENC_API_SetValueUnPaint (ID_LBX_ipp_price_ChnInfo, WDT_LBX_SV_INDEX, 0, 0);
	}
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		sprintf (g_cdcas_str[0], "%s%d", "TVSID:",CDCAS_ipp.ippvProgram.m_wTvsID);
		sprintf (g_cdcas_str[1], "%s%d", "SlotID:",CDCAS_ipp.ippvProgram.m_bySlotID);
		sprintf (g_cdcas_str[2], "%s%u", "ProductID:",CDCAS_ipp.ippvProgram.m_dwProductID);
		if(bUpdate == 0)
		{
			sprintf (g_cdcas_str[4], "%s", "Price:");
			sprintf (g_cdcas_str[5], "%s", "Input Pin:");
			sprintf (g_cdcas_str[6], "%s", "Prompt:");
			sprintf (g_cdcas_str[7], "%s", "Num:select price      OK:next step");
			sprintf (g_cdcas_str[8], "%s", "EXIT:exit");
		}
		if(CDCAS_ipp.messageType == CDCA_IPPV_FREEVIEWED_SEGMENT)
		{
			sprintf (g_cdcas_str[9], "%s", "IPPV Preview Stage");
			ipp_type = 1;
			UI_APP_GetDateStrFromValC_TF(CDCAS_ipp.ippvProgram.m_wExpiredDate,tmp);
			sprintf (g_cdcas_str[3], "%s%s", "Expired Date:",tmp);
		}
		else if(CDCAS_ipp.messageType == CDCA_IPPV_PAYVIEWED_SEGMENT)
		{
			sprintf (g_cdcas_str[9], "%s", "IPPV Book Stage");
			ipp_type = 2;
			UI_APP_GetDateStrFromValC_TF(CDCAS_ipp.ippvProgram.m_wExpiredDate,tmp);
			sprintf (g_cdcas_str[3], "%s%s", "Expired Date:",tmp);

		}
		else if(CDCAS_ipp.messageType == CDCA_IPPT_PAYVIEWED_SEGMENT)
		{
			sprintf (g_cdcas_str[9], "%s", "IPPT Book Stage");
			ipp_type = 3;
			sprintf (g_cdcas_str[3], "%s%dseconds(auto pay)", "Interval:",CDCAS_ipp.ippvProgram.m_wExpiredDate);
		}
	}
	else
	{
		sprintf (g_cdcas_str[0], "%s%d", "运营商ID:",CDCAS_ipp.ippvProgram.m_wTvsID);
		sprintf (g_cdcas_str[1], "%s%d", "钱包ID:",CDCAS_ipp.ippvProgram.m_bySlotID);
		sprintf (g_cdcas_str[2], "%s%u", "产品ID:",CDCAS_ipp.ippvProgram.m_dwProductID);
		if(bUpdate == 0)
		{
			sprintf (g_cdcas_str[4], "%s", "价格:");
			sprintf (g_cdcas_str[5], "%s", "输入Pin:");
			sprintf (g_cdcas_str[6], "%s", "提示:");	
			sprintf (g_cdcas_str[7], "%s", "数字键:选择价格        确认键:下一步");	
			sprintf (g_cdcas_str[8], "%s", "退出键:取消订购");
		}
		if(CDCAS_ipp.messageType == CDCA_IPPV_FREEVIEWED_SEGMENT)
		{
			sprintf (g_cdcas_str[9], "%s", "IPPV免费预览阶段");
			ipp_type = 1;
			UI_APP_GetDateStrFromValC_TF(CDCAS_ipp.ippvProgram.m_wExpiredDate,tmp);
			sprintf (g_cdcas_str[3], "%s%s", "过期日期:",tmp);		
		}
		else if(CDCAS_ipp.messageType == CDCA_IPPV_PAYVIEWED_SEGMENT)
		{
			sprintf (g_cdcas_str[9], "%s", "IPPV收费阶段");
			ipp_type = 2;
			UI_APP_GetDateStrFromValC_TF(CDCAS_ipp.ippvProgram.m_wExpiredDate,tmp);
			sprintf (g_cdcas_str[3], "%s%s", "过期日期:",tmp);	
		}
		else if(CDCAS_ipp.messageType == CDCA_IPPT_PAYVIEWED_SEGMENT)
		{
			sprintf (g_cdcas_str[9], "%s", "IPPT收费段");
			ipp_type = 3;
			sprintf (g_cdcas_str[3], "%s%d分钟(自动扣费)", "间隔:",CDCAS_ipp.ippvProgram.m_wExpiredDate);
		}
	}
	if(bUpdate)
	{
		GUI_ENC_API_SetValue (ID_STA_ipp_tvsid_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[0], 0);
		GUI_ENC_API_SetValue (ID_STA_ipp_slotid_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[1], 0);
		GUI_ENC_API_SetValue (ID_STA_ipp_productid_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[2], 0);
		GUI_ENC_API_SetValue (ID_STA_ipp_ExpiredDate_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[3], 0);	
	//	GUI_ENC_API_SetValue (ID_STA_ipp_title_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[9], 0);
		switch(ipp_type)
		{
			case 1:GUI_ENC_API_SetValue(ID_STA_ipp_title_ChnInfo, WDT_STA_SV_DATAID, RS_IPPVPreview, 0);break;
			case 2:GUI_ENC_API_SetValue(ID_STA_ipp_title_ChnInfo, WDT_STA_SV_DATAID, RS_IPPVPAYview, 0);break;
			case 3:GUI_ENC_API_SetValue(ID_STA_ipp_title_ChnInfo, WDT_STA_SV_DATAID, RS_IPPTPAYview, 0);break;
			default:
				break;
		}
	}
	else
	{
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_tvsid_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[0], 0);
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_slotid_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[1], 0);
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_productid_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[2], 0);
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_ExpiredDate_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[3], 0);
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_price_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[4], 0);
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_pin_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[5], 0);
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_prompt_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[7], 0);
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_prompt2_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[8], 0);
	//	GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_title_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[9], 0);
		GUI_ENC_API_SetValueUnPaint (ID_TBX_ipp_pin_ChnInfo,WDT_TBX_SV_PASSWORD,(BU32)password,0);
		FYF_API_Memset (g_cdcas_str[10], ' ', 80);
		g_cdcas_str[10][13] = 0;
		GUI_ENC_API_SetValueUnPaint (ID_STA_ipp_pin_prompt_ChnInfo,WDT_STA_SV_STRING,(BU32)g_cdcas_str[10],0);
		switch(ipp_type)
		{
			case 1:GUI_ENC_API_SetValueUnPaint(ID_STA_ipp_title_ChnInfo, WDT_STA_SV_DATAID, RS_IPPVPreview, 0);break;
			case 2:GUI_ENC_API_SetValueUnPaint(ID_STA_ipp_title_ChnInfo, WDT_STA_SV_DATAID, RS_IPPVPAYview, 0);break;
			case 3:GUI_ENC_API_SetValueUnPaint(ID_STA_ipp_title_ChnInfo, WDT_STA_SV_DATAID, RS_IPPTPAYview, 0);break;
			default:
				break;
		}
	}

}


void UI_APP_FullScreen_ipp_price_Key(BU32 key)
{

	BU32 lang;
	PU08 str;
	BU08 pin[7];
	BU32 index;
	BU08 *password = "......";
	FYF_API_Memset (g_cdcas_str[10], ' ', 80);
	g_cdcas_str[10][13] = 0;
	GUI_ENC_API_SetValue (ID_STA_ipp_pin_prompt_ChnInfo,WDT_STA_SV_STRING,(BU32)g_cdcas_str[10],0);
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(GUI_ENC_API_IsFocus(ID_STA_ipp_price_ChnInfo))
	{	
		if(GUI_VK_EXIT == key)
		{
			CDSTBCA_HideIPPVDlg (CDCAS_ipp.ecmPid);
			/* 退出订购 */
			CDCASTB_StopIPPVBuyDlg(CDCA_FALSE,CDCAS_ipp.ecmPid,NULL,NULL);
		}
		else if(GUI_VK_SELECT == key)
		{	
			/* 下一步 */
			GUI_ENC_API_SetFocus (ID_TBX_ipp_pin_ChnInfo);
			if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
			{
				sprintf (g_cdcas_str[7], "%s", "DIGITAL:input pin      OK:book");
				sprintf (g_cdcas_str[8], "%s", "EXIT:exit			   RECALL:previous step");	
			}
			else
			{
				sprintf (g_cdcas_str[7], "%s", "数字键:输入pin      确认键:订购");
				sprintf (g_cdcas_str[8], "%s", "退出键:取消订购     返回键:上一步");	
			}
			GUI_ENC_API_SetValue (ID_STA_ipp_prompt_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[7], 0);
			GUI_ENC_API_SetValue (ID_STA_ipp_prompt2_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[8], 0);			
		}
		else if(GUI_VK_DOWN == key || GUI_VK_UP == key)
		{
			CDSTBCA_HideIPPVDlg (CDCAS_ipp.ecmPid);
			/* 退出订购 */
			CDCASTB_StopIPPVBuyDlg(CDCA_FALSE,CDCAS_ipp.ecmPid,NULL,NULL);	
			GUI_ENC_API_PostMessage (ID_DLG_ChnInfo, WM_KEY, key, 0);
		}
		else if(GUI_VK_1 == key || GUI_VK_2 == key)
		{
			GUI_ENC_API_SetValue (ID_LBX_ipp_price_ChnInfo, WDT_LBX_SV_INDEX, key-1, 0);
		}
		else if(GUI_VK_MUTE == key )
		{
			GUI_ENC_API_PostMessage (ID_DLG_ChnInfo, WM_KEY, key, 0);
		}
	}
	else if(GUI_ENC_API_IsFocus(ID_TBX_ipp_pin_ChnInfo))
	{
		if(GUI_VK_EXIT == key)
		{
			CDSTBCA_HideIPPVDlg (CDCAS_ipp.ecmPid);
			/* 退出订购 */
			CDCASTB_StopIPPVBuyDlg(CDCA_FALSE,CDCAS_ipp.ecmPid,NULL,NULL);			
		}
		else if(GUI_VK_SELECT == key)
		{			
			GUI_ENC_API_GetValue (ID_TBX_ipp_pin_ChnInfo,WDT_TBX_SV_PASSWORD,(BU32 *)&str,0);
			if(!FYF_API_Strcmp (str, "......") || FYF_API_Strlen (str) != 6)
			{
				if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
				{
					sprintf (g_cdcas_str[10], "%s", "PIN Error");
				}
				else
				{
					sprintf (g_cdcas_str[10], "%s", "PIN无效");
				}				
				GUI_ENC_API_SetValue (ID_STA_ipp_pin_prompt_ChnInfo,WDT_STA_SV_STRING,(BU32)g_cdcas_str[10],0);
				GUI_ENC_API_SetValueUnPaint (ID_TBX_ipp_pin_ChnInfo,WDT_TBX_SV_PASSWORD,(BU32)password,0);
			}
			else
			{
				CDSTBCA_HideIPPVDlg (CDCAS_ipp.ecmPid);
				/* 订购 */
				FYF_API_Memset(pin, 0, 7);
				for(index = 0;index<6;index++)
				{
					pin[index]=str[index]-0x30;
				}
				GUI_ENC_API_GetValue (ID_LBX_ipp_price_ChnInfo, WDT_LBX_SV_INDEX,&index, 0);
				CDCAS_ipp.retValue = CDCASTB_StopIPPVBuyDlg(CDCA_TRUE,CDCAS_ipp.ecmPid,pin,&(CDCAS_ipp.ippvProgram.m_Price[index]));

				/* 发送显示IPP订购结果 */
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_IPP_NOTIFY, 0, 0);
			}
		}
		else if(GUI_VK_RECALL == key)
		{
			/* 上一步 */
			GUI_ENC_API_SetFocus (ID_STA_ipp_price_ChnInfo);
			if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
			{
				sprintf (g_cdcas_str[7], "%s", "Num:select price 	 OK:next step");
				sprintf (g_cdcas_str[8], "%s", "EXIT:exit");
			}
			else
			{
				sprintf (g_cdcas_str[7], "%s", "数字键:选择价格        确认键:下一步");	
				sprintf (g_cdcas_str[8], "%s", "退出键:取消订购");	
			}
			GUI_ENC_API_SetValue (ID_STA_ipp_prompt_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[7], 0);
			GUI_ENC_API_SetValue (ID_STA_ipp_prompt2_ChnInfo, WDT_STA_SV_STRING, (BU32)g_cdcas_str[8], 0);	
		}
		else if(GUI_VK_DOWN == key || GUI_VK_UP == key)
		{
			CDSTBCA_HideIPPVDlg (CDCAS_ipp.ecmPid);
			/* 退出订购 */
			CDCASTB_StopIPPVBuyDlg(CDCA_FALSE,CDCAS_ipp.ecmPid,NULL,NULL);	
			GUI_ENC_API_PostMessage (ID_DLG_ChnInfo, WM_KEY, key, 0);			
		}
		else if(GUI_VK_MUTE == key )
		{
			GUI_ENC_API_PostMessage (ID_DLG_ChnInfo, WM_KEY, key, 0);
		}
	}

}


BU32 CDSTBCA_API_IppDlgShow(void)
{
	CDSTBCA_IPP_PROTECT ();
	return CDCAS_ipp.ipp_dlg_show;
	CDSTBCA_IPP_UNPROTECT ();
}

BU32 CDSTBCA_API_IppBuyRetID(void)
{
	ippretvalue = 1;
	CDSTBCA_IPP_PROTECT ();
	if(CDCA_RC_OK == CDCAS_ipp.retValue)
	{
		ippretvalue = 0;
		return RS_Book_Success;
	}
	else if(CDCA_RC_CARD_INVALID == CDCAS_ipp.retValue)
	{
		return RS_CardPair_Invalid;
	}
	else if(CDCA_RC_POINTER_INVALID == CDCAS_ipp.retValue)
	{
		return RS_Pointer_Invalid;
	}
	else if(CDCA_RC_CARD_NO_ROOM == CDCAS_ipp.retValue)
	{
		return RS_Card_No_Room;
	}
	else if(CDCA_RC_PROG_STATUS_INVALID == CDCAS_ipp.retValue)
	{
		return RS_Prog_Status_Invalid;
	}
	else if(CDCA_RC_PIN_INVALID == CDCAS_ipp.retValue)
	{
		return RS_PinErr;
	}
	else if(CDCA_RC_DATA_NOT_FIND == CDCAS_ipp.retValue)
	{
		return RS_Data_Not_Find;
	}
	else
	{
		return RS_Error;
	}
	CDSTBCA_IPP_UNPROTECT ();

}

void CDSTBCA_API_ExitBuy(void)
{
	CDSTBCA_HideIPPVDlg (CDCAS_ipp.ecmPid);
	/* 退出订购 */
	CDCASTB_StopIPPVBuyDlg(CDCA_FALSE,CDCAS_ipp.ecmPid,NULL,NULL);		
}

