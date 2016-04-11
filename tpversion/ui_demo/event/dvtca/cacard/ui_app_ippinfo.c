#include "../../../osd/include/resource.h"
#include "../../../../../tpmiddle/include/app_common.h"


#define G_IPP_NUM_IN_PAGE (3)
#define CA_PIN_LEN 			 8
typedef enum CA_BOOK_STATE
{
	CA_BOOK_NULL,
	CA_TO_BOOK,
	CA_TO_UNBOOK
}CA_BOOK_STATE;

BU32 g_ipp_pageNum = 0;
BU32 g_ipp_curPage = 0;
BU32 g_ipp_numOfLastPage = 0;
BU08 g_ipp_item[4][60];
BU08 *g_ipp_item_str[4]=
{
	g_ipp_item[0],
	g_ipp_item[1],
	g_ipp_item[2],
	g_ipp_item[3]
};

char *StrCnIPP[]=
{
"操作成功",
"预订成功,但余额不多，请及时充值",
"需要通过PIN验证",
"与IC卡通讯参数错误",
"运营商ID无效",
"钱包没有发现",
"产品已经过期",
"产品已经存在,不能操作",
"没有空间",
"产品没有发现",
"价格无效",
"未知错误",
"价格类型",
"持续时间",
"无",
"节目类型",
"已订购",
"未订购",
};

char *StrEnIPP[]=
{
"Operate succeeded",
"Book succeeded but no enough balance",
"Need PIN verify",
"Communicate error",
"Invalid Operator ID",
"No accord slot",
"Product expried",
"Product existed",
"No space",
"No accord product",
"Invalid price",
"Unknow error",
"Price Type",
"Duration Time",
"None",
"Type",
"Book",
"Unbook",
};		

static FYF_CA_Ipp_s fyf_ca_ipp[250];
extern FYF_CA_RealTimeIpp_s DVTCAS_RealTimeipp;
BU08 g_ui_app_str[12][80];
static BU08 *g_ca_str2_p[5]=
{
	g_ui_app_str[6],
	g_ui_app_str[7],
	g_ui_app_str[8],
	g_ui_app_str[9],
	g_ui_app_str[10]
};
static BU32 g_ipp_amount = 0;
static BU08 g_ipp_BookUnbookState = CA_BOOK_NULL;
static BU08 _ca_ipp_priceBSelected=0xff;

static FYF_CA_TvsInfo g_ca_tvsInfo[5];
static FYF_CA_ViewedIpp_s g_ca_viewIpp_s[250];
static BU32 g_viewed_ipp_amount = 0;
BU32 g_ViewedIPP_pageNum = 0;
BU32 g_ViewedIPP_curPage = 0;
BU32 g_ViewedIPP_numOfLastPage = 0;
#define G_ViewedIPP_NUM_IN_PAGE (4)
BU08 g_ViewedIPP_item[4][60];
BU08 *g_ViewedIPP_item_str[4]=
{
	g_ViewedIPP_item[0],
	g_ViewedIPP_item[1],
	g_ViewedIPP_item[2],
	g_ViewedIPP_item[3]
};
BU32 g_ViewedIPP_tvsNum = 0;
static char *g_ca_ps_cn_str[]=
{
	"不回传,可录像价格",
	"不回传,不录像价格",
	"可回传,可录像价格",
	"可回传,不录像价格",
	
};
static char *g_ca_ps_en_str[]=
{
	"Tpp Tap Price",
	"Tpp No Tap Price",
	"Cpp Tap Price",
	"Cpp No Tap Price",
	
};

static char *StrCnTmp[]=
{
	"天",
	"时",
	"分",
	"秒",
	"元",
	"分钟",
	"小时",
	"天",
	"月",
	"年",
};
static char *StrEnTmp[]=
{
	"Day ",
	"Hours ",
	"Min ",
	"Sec ",
	"Yuan",
	"Min",
	"Hour",
	"day",
	"mon",
	"year",
};
//已观看IPP查询
void CA_ViewedIppv_ShowProgInfo(BU32 index)
{
	BU08 StartTime[20] = {0};
	caInfo_time_s DurationTime;
	caInfo_currency_s BookPrice={"","",""};
	BU08 tmp[30];
	BU32 fen = 0;
	char **pDic;
	char **pPriceType;
	BU32 lang;
	char **pStr;
	
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
		pPriceType =  g_ca_ps_en_str;
		pStr = StrEnTmp;
		sprintf(g_ui_app_str[4],"IPP amount: %d ,selected: %d ",g_viewed_ipp_amount,index + 1);		
	}
	else
	{
		pDic = DicData;
		pPriceType =  g_ca_ps_cn_str;
		pStr = StrCnTmp;
		sprintf(g_ui_app_str[4],"IPP节目总数:%d 个,选中的是:第%d 个",g_viewed_ipp_amount,index + 1);		
	}


#if 0
		switch (g_ca_viewIpp_s[index].m_byBookedPriceType)
		{
			case 0:
				GUI_API_Strcpy(tmp, "不回传,可录像价格:");
				break;
			case 1:
				GUI_API_Strcpy(tmp, "不回传,不录像价格:");
				break;
			case 2:
				GUI_API_Strcpy(tmp, "可回传,可录像价格:");
				break;
			case 3:
				GUI_API_Strcpy(tmp, "可回传,不录像价格:");
				break;
			default:
				GUI_API_Strcpy(tmp, " ");
				break;

		}	
#endif
	GUI_API_Strcpy(tmp, pPriceType[g_ca_viewIpp_s[index].m_byBookedPriceType]);
	fen = g_ca_viewIpp_s[index].m_wBookedPrice;

	if(g_ca_viewIpp_s[index].m_byBookedInterval)
	{
		sprintf(g_ui_app_str[0],"%s: %s(%d.%02d%s/%d%s)",pDic[RS_PriceType],tmp,fen/100,fen%100,pStr[4],
			g_ca_viewIpp_s[index].m_byBookedInterval,pStr[5+g_ca_viewIpp_s[index].m_byUnit]);
	}
	else
	{
		sprintf(g_ui_app_str[0],"%s: %s(%d.%02d%s)",pDic[RS_PriceType],tmp,fen/100,fen%100,pStr[4]);
	}
	
	UI_APP_GetTimeStrFromValC(g_ca_viewIpp_s[index].m_tStartTime, StartTime);
	sprintf(g_ui_app_str[1],"%s: %s",pDic[RS_StartTime],StartTime);

	UI_APP_convertSecIntoDay(g_ca_viewIpp_s[index].m_dwDuration, &DurationTime);
	sprintf(g_ui_app_str[2],"%s: %s%s%s%s%s%s%s%s",pDic[RS_Duration],DurationTime.m_days,pStr[0],
		DurationTime.m_hrs,pStr[1],
		DurationTime.m_mins,pStr[2],
		DurationTime.m_secs,pStr[3]);

	sprintf(g_ui_app_str[3],"%s: %s",pDic[RS_OtherInfo],g_ca_viewIpp_s[index].m_szOtherInfo);
	
	GUI_ENC_API_SetValue(ID_STA_Line1_ViewedIPP, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_ViewedIPP, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_ViewedIPP, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	GUI_ENC_API_SetValue(ID_STA_Line4_ViewedIPP, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
	GUI_ENC_API_SetValue(ID_STA_Prompt_ViewedIPP, WDT_STA_SV_STRING, (BU32)g_ui_app_str[4], 0);

}
void UI_APP_ViewedIPP_ShowCurPage(void)
{
	BU32 index = 0;
	BU32 itemIndex = 0;
	BU32 itemNum = 0;
	BU32 lang;
	char **pDic = DicData;
	BU08 tvsName[21];
	BU16 tvsID;
	BU32 j;
	
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}

	if(g_ViewedIPP_curPage >= g_ViewedIPP_pageNum)
	{
		g_ViewedIPP_curPage = 0;
	}

	itemNum = 0;

	for(index = 0;index<G_ViewedIPP_NUM_IN_PAGE;index++)
	{
		itemIndex = g_ViewedIPP_curPage*G_ViewedIPP_NUM_IN_PAGE+index;
		if(itemIndex<g_viewed_ipp_amount)
		{	
			g_ca_viewIpp_s[itemIndex].m_szProdName[20] = 0;
			GUI_API_Memset(tvsName, 0, 21);
			tvsID = g_ca_viewIpp_s[itemIndex].m_wTVSID; 
			for(j=0;j<g_ViewedIPP_tvsNum;j++)
			{
				if(tvsID == g_ca_tvsInfo[j].m_wTVSID)
				{
					GUI_API_Memcpy(tvsName, g_ca_tvsInfo[j].m_szTVSName, 20);
					break;
				}
			}
			sprintf(g_ViewedIPP_item_str[index],"%-2d %-20s %s",itemIndex+1,g_ca_viewIpp_s[itemIndex].m_szProdName,tvsName);
			itemNum++;
		}
	}
	GUI_ENC_API_SetValue (ID_LBP_ViewedIPP, WDT_LBP_SV_DATA, (BU32)g_ViewedIPP_item_str, itemNum);
}

void UI_APP_CA_ViewedIPP_Enter(void)
{
	BS32 ret;
	BU32 i,j,num=5;
	BU32 index;
	BU32 itemIndex;
	
	g_viewed_ipp_amount = 250;
	ret = FYF_API_ca_get_value(FYF_CA_SV_VIEWED_PPV,(BU32 *)g_ca_viewIpp_s,&g_viewed_ipp_amount);

	g_ViewedIPP_tvsNum = 5;
	ret |= FYF_API_ca_get_value(FYF_CA_SV_OPERATOR_INFO,(BU32 *)g_ca_tvsInfo,&g_ViewedIPP_tvsNum);

	if(FYF_OK == ret && g_viewed_ipp_amount&& g_ViewedIPP_tvsNum)
	{
		g_ViewedIPP_pageNum = (g_viewed_ipp_amount-1)/G_ViewedIPP_NUM_IN_PAGE+1;
		g_ViewedIPP_numOfLastPage = g_viewed_ipp_amount%G_ViewedIPP_NUM_IN_PAGE;
		if(g_ViewedIPP_numOfLastPage == 0)
		{
			g_ViewedIPP_numOfLastPage = G_ViewedIPP_NUM_IN_PAGE;
		}

		GUI_ENC_API_SetVisible(ID_LBP_ViewedIPP, WS_VISIBLE);
		UI_APP_ViewedIPP_ShowCurPage ();
		GUI_ENC_API_GetValue (ID_LBP_ViewedIPP, WDT_LBP_SV_INDEX, &index, 0);
		itemIndex = index+g_ViewedIPP_curPage*G_ViewedIPP_NUM_IN_PAGE;
	//	GUI_ENC_API_SetValue(ID_LBP_ViewedIPP, WDT_LBP_SV_INDEX, 0, 0);
	//	GUI_ENC_API_SetValue(ID_SBR_ViewedIPP, WDT_SBR_SV_VALUE, 0, 0);
	//	GUI_ENC_API_SetValue(ID_LBX_ViewedIPP, WDT_LBX_SV_DATA, (BU32)g_ca_str_p, g_viewed_ipp_amount);
		GUI_ENC_API_SetValue (ID_SBR_ViewedIPP, WDT_SBR_SV_MAX, g_viewed_ipp_amount-1,0);
		GUI_ENC_API_SetValue (ID_SBR_ViewedIPP, WDT_SBR_SV_MIN, 0,0);
		GUI_ENC_API_SetValue (ID_SBR_ViewedIPP, WDT_SBR_SV_VALUE, itemIndex,0);

		CA_ViewedIppv_ShowProgInfo(itemIndex);

		GUI_ENC_API_SetVisible(ID_STA_Text_ViewedIPP, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ViewedIPP, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_TextBk_ViewedIPP, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_SBR_ViewedIPP, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Name_ViewedIPP, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Servicer_ViewedIPP, WS_VISIBLE);

	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_Text_ViewedIPP, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Title_ViewedIPP, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_TextBk_ViewedIPP, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_SBR_ViewedIPP, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Name_ViewedIPP, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Servicer_ViewedIPP, WS_DISABLE);
		g_ViewedIPP_curPage = 0;
		if(FYF_OK == ret && g_viewed_ipp_amount==0)
		GUI_ENC_API_SetValue(ID_STA_Text_ViewedIPP, WDT_STA_SV_DATAID, RS_NoIPP, 0);

		GUI_ENC_API_SetValue(ID_LBP_ViewedIPP, WDT_LBP_SV_INDEX, 0, 0);
		GUI_ENC_API_SetValue(ID_LBP_ViewedIPP, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetVisible(ID_LBP_ViewedIPP, WS_DISABLE);
		GUI_ENC_API_SetValue(ID_STA_Line1_ViewedIPP, WDT_STA_SV_STRING, (BU32)"", 0);
		GUI_ENC_API_SetValue(ID_STA_Line2_ViewedIPP, WDT_STA_SV_STRING, (BU32)"", 0);
		GUI_ENC_API_SetValue(ID_STA_Line3_ViewedIPP, WDT_STA_SV_STRING, (BU32)"", 0);
		GUI_ENC_API_SetValue(ID_STA_Line4_ViewedIPP, WDT_STA_SV_STRING, (BU32)"", 0);
		GUI_ENC_API_SetValue(ID_STA_Prompt_ViewedIPP, WDT_STA_SV_STRING, (BU32)"", 0);

	}
	APP_Com_API_SetFullScreen();
}
void UI_APP_CA_ViewedIPP_End(void)
{

}
void UI_APP_CA_ViewedIPP_Key(BU32 key)
{
	BU32 index;
	BU32 remainder = 0;
	BU32 itemIndex = 0;
	
	GUI_ENC_API_GetValue(ID_LBP_ViewedIPP, WDT_LBP_SV_INDEX, &index, 0);
	itemIndex = index+g_ViewedIPP_curPage*G_ViewedIPP_NUM_IN_PAGE;
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
		if(g_viewed_ipp_amount <= 1)return;
		CA_ViewedIppv_ShowProgInfo(itemIndex);
		break;
	case GUI_VK_P_DOWN:		
		if(g_viewed_ipp_amount <= G_ViewedIPP_NUM_IN_PAGE)return;
#if 0
		remainder = g_viewed_ipp_amount%IPP_NUM_IN_PAGE;
		if(!remainder) remainder = IPP_NUM_IN_PAGE;
		
		if(index < IPP_NUM_IN_PAGE)
		{
			if(index < remainder)
			{
				index += g_viewed_ipp_amount - remainder;
			}
			else
			{
				index = g_viewed_ipp_amount-1;
			}
		}
		else
		{
			index -= IPP_NUM_IN_PAGE;
		}
		GUI_ENC_API_SetValue(ID_LBX_ViewedIPP, WDT_LBX_SV_INDEX, index, 0);
#endif
		CA_ViewedIppv_ShowProgInfo(itemIndex);
		break;
	case GUI_VK_P_UP:	
		if(g_viewed_ipp_amount <= G_ViewedIPP_NUM_IN_PAGE)return;
#if 0
		remainder = g_viewed_ipp_amount%IPP_NUM_IN_PAGE;
		if(!remainder) remainder = 6;
		
		index += IPP_NUM_IN_PAGE;
		if(index >= g_viewed_ipp_amount)
		{
			if(index >= g_viewed_ipp_amount - remainder + IPP_NUM_IN_PAGE)//最后一页
			{
				index = index%IPP_NUM_IN_PAGE;
			}
			else	//倒数第二页
			{
				index = g_viewed_ipp_amount - 1;
			}

		}
		GUI_ENC_API_SetValue(ID_LBX_ViewedIPP, WDT_LBX_SV_INDEX, index, 0);
#endif
		CA_ViewedIppv_ShowProgInfo(itemIndex);
		break;
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
	GUI_ENC_API_SetValue (ID_SBR_ViewedIPP, WDT_SBR_SV_VALUE, itemIndex,0);
}
void UI_APP_CA_ViewedIPP_Change(void)
{
	BS32 state;

	GUI_ENC_API_GetValue(ID_LBP_ViewedIPP, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_ViewedIPP_curPage++;
			break;
		case -1:
			if(g_ViewedIPP_curPage == 0)
				g_ViewedIPP_curPage = g_ViewedIPP_pageNum -1;
			else
				g_ViewedIPP_curPage--;
			
			if(g_ViewedIPP_curPage == g_ViewedIPP_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_ViewedIPP, WDT_LBP_SV_INDEX, G_ViewedIPP_NUM_IN_PAGE-1, 0);
			}
			break;
		default:
			break;
	}
	UI_APP_ViewedIPP_ShowCurPage();//显示当前页

}



/*
g_ui_app_str

0 开始时间
1 持续时间
2 已订购类型及价格(未订购为无)
3 节目名称
4 所在频道名称

g_ca_str2_p
返回: 
booked=1,已订购
booked=0,未订购
*/
void CA_IppInfoToStr(FYF_CA_Ipp_s ipp, BU08 *booked,BU08 *Ipp)
{
	BU08 StartTime[20] = {0};
	caInfo_time_s DurationTime;
	caInfo_currency_s BookPrice;
	BU08 BookInterval[3] = {0};	
	BU08 tmp[30];
	BU08 CurInterval[3] = {0};
	BU08 IppFlag,TapFlag,IsBooked;
	BU32 i,len;
	char **pDic;
	char **pPriceType;
	BU32 lang;
	
	BU16 price[4]=
	{
		ipp.m_wCurTppTapPrice,
		ipp.m_wCurTppNoTapPrice,
		ipp.m_wCurCppTapPrice,
		ipp.m_wCurCppNoTapPrice,
	};

	char **pStr1;
	char **pStr2;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
		pPriceType =  g_ca_ps_en_str;
		pStr1 = StrEnTmp;
		pStr2 = StrEnIPP;
	}
	else
	{
		pDic = DicData;
		pPriceType =  g_ca_ps_cn_str;
		pStr1 = StrCnTmp;
		pStr2 = StrCnIPP;
	}
	
	IppFlag = (ipp.m_byIppStatus&0x08)>>3;
	TapFlag	= (ipp.m_byIppStatus&0x04)>>2;
	IsBooked = (ipp.m_byIppStatus&0x02)>>1;		

	*Ipp = IppFlag;
	/*--line1--*/
	UI_APP_GetTimeStrFromValC(ipp.m_tStartTime, StartTime);
	sprintf(g_ui_app_str[0],"%s: %s",pDic[RS_StartTime],StartTime);
	/*--line2--*/
	UI_APP_convertSecIntoDay(ipp.m_dwDuration, &DurationTime);
	sprintf(g_ui_app_str[1],"%s: %s%s%s%s%s%s%s%s",pStr2[13],DurationTime.m_days,pStr1[0],
		DurationTime.m_hrs,pStr1[1],DurationTime.m_mins,pStr1[2],DurationTime.m_secs,pStr1[3]);
	/*--line3--*/
	if(0 == ipp.m_wBookedPrice)
	{
		sprintf(g_ui_app_str[2],"%s: %s",pDic[RS_BookPrice],pStr2[14]);
	}
	else
	{
		UI_APP_convertFengIntoYuan(ipp.m_wBookedPrice, &BookPrice);
		GUI_API_AddDecStr(ipp.m_byBookedInterval, 2, BookInterval);
		GUI_API_Strcpy(tmp, pPriceType[ipp.m_byBookedPriceType]);
		if(IppFlag == 0)
		{
			sprintf(g_ui_app_str[2],"%s: %s(%s.%s%s%s/%s%s)",pDic[RS_BookPrice],tmp,BookPrice.m_yuan,BookPrice.m_jiao,BookPrice.m_fen,
				pStr1[4],BookInterval,pStr1[5+ipp.m_byUnit]);
		}
		else
		{
			sprintf(g_ui_app_str[2],"%s: %s(%s.%s%s%s)",pDic[RS_BookPrice],tmp,BookPrice.m_yuan,BookPrice.m_jiao,BookPrice.m_fen,pStr1[4]);
		}
	}
	/*--line4--*/
	len = GUI_API_Strlen(ipp.m_szProdName);
	if(len > 21)
	{
		ipp.m_szProdName[20] = 0;
		ipp.m_szProdName[19] = '.';
	}
	sprintf(g_ui_app_str[3],"%s: %s",pDic[RS_ProgramName],ipp.m_szProdName);
	sprintf(g_ui_app_str[4],"%s: %s",pDic[RS_ChnName],ipp.m_szServiceName);
	sprintf(g_ui_app_str[5],"%s: %s",pStr2[15],(IppFlag)?"IPPV":"IPPT");

	/*--lbx--*/
	*booked = IsBooked;

	for(i=0;i<4;i++)
	{
		if(price[i] == 0)
		{
			sprintf(g_ca_str2_p[i],"%d:%s(%s)",i,pPriceType[i],pStr2[14]);
		}
		else
		{
			UI_APP_convertFengIntoYuan(price[i], &BookPrice);
			GUI_API_AddDecStr(ipp.m_byCurInterval, 2, CurInterval);
			if(IppFlag == 0)
			{
				sprintf(g_ca_str2_p[i],"%d:%s(%s.%s%s%s/%s%s)",i,pPriceType[i],BookPrice.m_yuan,BookPrice.m_jiao,BookPrice.m_fen,pStr1[4],CurInterval,pStr1[5+ipp.m_byUnit]);
			}
			else
			{
				sprintf(g_ca_str2_p[i],"%d:%s(%s.%s%s%s)",i,pPriceType[i],BookPrice.m_yuan,BookPrice.m_jiao,BookPrice.m_fen,pStr1[4]);
			}

		}
		
	}
	

}
//显示菜单订购/退订IPP信息

void CA_BookUnbook_ShowProgInfo(BU32 progIndex)
{
	BU08 IsBooked,IppFlag;
	char **pStr;
	BU32 lang;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pStr = StrEnIPP;
	}
	else
	{
		pStr = StrCnIPP;
	}
	CA_IppInfoToStr(fyf_ca_ipp[progIndex],&IsBooked,&IppFlag);	
	
	GUI_ENC_API_SetValue(ID_STA_Line1_BookUnbook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Line2_BookUnbook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Line3_BookUnbook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	GUI_ENC_API_SetValue(ID_STA_Line4_BookUnbook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[4], 0);
	/*--line5--*/
	GUI_ENC_API_SetValue(ID_STA_Line5_BookUnbook, WDT_STA_SV_STRING, pStr[12], 0);
	/*--line6--*/
	GUI_ENC_API_SetValue(ID_LBX_Price_BookUnbook, WDT_LBX_SV_DATA, (BU32)g_ca_str2_p, 4);
	if(IppFlag)
	{
		GUI_ENC_API_SetVisible(ID_STA_zhouqi_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_TBX_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Color_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_Color_BookUnbook, 1);
		GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, RS_ChoosePriceBeforeBook, 0);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_zhouqi_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_TBX_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_SetValue(ID_TBX_BookUnbook, WDT_TBX_SV_DATA, (BU32)(fyf_ca_ipp[progIndex].m_wIpptPeriod), 0);
		GUI_ENC_API_Update(ID_STA_zhouqi_BookUnbook, 1);
		GUI_ENC_API_Update(ID_TBX_BookUnbook, 1);
		GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, RS_PsPriceZhouqiBuy, 0);
	}
	GUI_ENC_API_SetValue(ID_LBX_Price_BookUnbook, WDT_LBX_SV_INDEX, 0, 0);
	if(IsBooked)
	{
		GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, RS_PsOKUnbook, 0);
	}

}

static void UI_APP_Ipp_ShowCurPage(void)
{
	BU32 index = 0;
	BU32 itemIndex = 0;
	BU32 j = 0;
	BU32 itemNum = 0;
	BU32 lang;
	char **pDic = StrCnIPP;
	BU32 ProNameLen;
	BU08 IsBooked,IppFlag;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = StrEnIPP;
	}
	else
	{
		pDic = StrCnIPP;
	}

	if(g_ipp_curPage >= g_ipp_pageNum)
	{
		g_ipp_curPage = 0;
	}

	itemNum = 0;
	for(index = 0;index<G_IPP_NUM_IN_PAGE;index++)
	{
		itemIndex = g_ipp_curPage*G_IPP_NUM_IN_PAGE+index;
		if(itemIndex<g_ipp_amount)
		{	
			ProNameLen = GUI_API_Strlen(fyf_ca_ipp[itemIndex].m_szProdName);
			//BookType[i] = fyf_ca_ipp[i].m_byBookedPriceType;
			IppFlag = (fyf_ca_ipp[itemIndex].m_byIppStatus&0x08)>>3;
			//TapFlag[i]	= (fyf_ca_ipp[i].m_byIppStatus&0x04)>>2;
			IsBooked = (fyf_ca_ipp[itemIndex].m_byIppStatus&0x02)>>1;		
			sprintf(g_ipp_item_str[index],"%-2d %-20s %-12s %s",itemIndex+1,fyf_ca_ipp[itemIndex].m_szProdName,
				(1 == IsBooked)?pDic[16]:pDic[17],(1 == IppFlag)?"IPPV":"IPPT");
			itemNum++;
		}
	}
	GUI_ENC_API_SetValue (ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_DATA, (BU32)g_ipp_item_str, itemNum);
}

void UI_APP_CA_BookUnbookIPP_Enter(void)
{
	BU32 i,lang;
	BS32 ret;
	char **pDic;
	BU32 itemIndex;
	
	g_ipp_BookUnbookState = 0;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = StrEnIPP;
	}
	else
	{
		pDic = StrCnIPP;
	}

	_ca_ipp_priceBSelected = 0xff;
	g_ipp_amount=250;

	ret = FYF_API_ca_get_value(FYF_CA_SV_GET_BOOK_IPP,(BU32 *)fyf_ca_ipp,&g_ipp_amount);

	if(g_ipp_amount>250)
	{
		g_ipp_amount = 250;
	}
	if(FYF_OK == ret && g_ipp_amount)
	{
		g_ipp_pageNum = (g_ipp_amount-1)/G_IPP_NUM_IN_PAGE+1;
		g_ipp_numOfLastPage = g_ipp_amount%G_IPP_NUM_IN_PAGE;
		if(g_ipp_numOfLastPage == 0)
		{
			g_ipp_numOfLastPage = G_IPP_NUM_IN_PAGE;
		}
		GUI_ENC_API_SetVisible(ID_STA_Text_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_TextTitle_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_TextBk_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_SBR_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_Prog_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_LBP_Ipp_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_SetValue(ID_STA_Line5_BookUnbook, WDT_STA_SV_STRING, pDic[12], 0);
		GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
		UI_APP_Ipp_ShowCurPage ();
		GUI_ENC_API_GetValue(ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_INDEX, &i, 0);
		itemIndex = i+G_IPP_NUM_IN_PAGE*g_ipp_curPage;
		if((fyf_ca_ipp[itemIndex].m_wBookedPrice)&&(((fyf_ca_ipp[itemIndex].m_byIppStatus&0x02)>>1) == 0))
		{
			switch(fyf_ca_ipp[itemIndex].m_byBookedPriceType)
			{
				case 0:
					fyf_ca_ipp[itemIndex].m_wCurTppTapPrice = fyf_ca_ipp[itemIndex].m_wBookedPrice;
					break;
				case 1:
					fyf_ca_ipp[itemIndex].m_wCurTppNoTapPrice = fyf_ca_ipp[itemIndex].m_wBookedPrice;
					break;
				case 2:
					fyf_ca_ipp[itemIndex].m_wCurCppTapPrice = fyf_ca_ipp[itemIndex].m_wBookedPrice;
					break;
				case 3:
					fyf_ca_ipp[itemIndex].m_wCurCppNoTapPrice = fyf_ca_ipp[itemIndex].m_wBookedPrice;
					break;
				default:
					fyf_ca_ipp[itemIndex].m_wCurTppNoTapPrice = fyf_ca_ipp[itemIndex].m_wBookedPrice;
					break;
			}
			
			fyf_ca_ipp[itemIndex].m_byCurInterval = fyf_ca_ipp[itemIndex].m_byBookedInterval;
			fyf_ca_ipp[itemIndex].m_wBookedPrice = 0;
		}
		CA_BookUnbook_ShowProgInfo(itemIndex);
		GUI_ENC_API_SetValue (ID_SBR_BookUnbook, WDT_SBR_SV_MAX, g_ipp_amount-1, 0);
		GUI_ENC_API_SetValue (ID_SBR_BookUnbook, WDT_SBR_SV_MIN, 0, 0);
		GUI_ENC_API_SetValue (ID_SBR_BookUnbook, WDT_SBR_SV_VALUE,itemIndex,0);

	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_Text_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_TextTitle_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_STA_TextBk_BookUnbook, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_SBR_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Prog_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_LBP_Ipp_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_zhouqi_BookUnbook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_TBX_BookUnbook, WS_DISABLE);
		
		if(FYF_OK == ret && g_ipp_amount==0)
			GUI_ENC_API_SetValue(ID_STA_Text_BookUnbook, WDT_STA_SV_DATAID, RS_NoIPP, 0);
	
		GUI_ENC_API_SetValue(ID_STA_Line1_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
		GUI_ENC_API_SetValue(ID_STA_Line2_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
		GUI_ENC_API_SetValue(ID_STA_Line3_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
		GUI_ENC_API_SetValue(ID_STA_Line4_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
		GUI_ENC_API_SetValue(ID_STA_Line5_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
		GUI_ENC_API_SetValue(ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, DATAID_NULL, 0);
		g_ipp_curPage = 0;
		GUI_ENC_API_SetValueUnPaint (ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_INDEX, 0, 0);
	}
	APP_Com_API_SetFullScreen();

}
void UI_APP_CA_BookUnbookIPP_End(void)
{
}
void UI_APP_CA_BookUnbookIPP_Key(BU32 key)
{
	BU32 index;
	BU08 IsBooked,IppFlag;
	BU16 price[4];
	BU32 itemIndex;
	char **pDic;
	BU32 lang;
	
	GUI_ENC_API_GetValue(ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_INDEX, &index, 0);
	itemIndex = index+g_ipp_curPage*G_IPP_NUM_IN_PAGE;
	
	//GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
	IsBooked = (fyf_ca_ipp[itemIndex].m_byIppStatus&0x02)>>1;	
	IppFlag = (fyf_ca_ipp[itemIndex].m_byIppStatus&0x08)>>3;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}	
	
	switch(key)
	{
	case GUI_VK_0:	
	case GUI_VK_1:	
	case GUI_VK_2:	
	case GUI_VK_3:	
		if(g_ipp_amount == 0)return;
		if(IsBooked)
		{
			GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, RS_BookedCannotChoosePrice, 0);
		}
		else if(fyf_ca_ipp[itemIndex].m_wBookedPrice)
		{
			GUI_ENC_API_SetValue(ID_LBX_Price_BookUnbook, WDT_LBX_SV_INDEX, key, 0);
			GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, RS_NoNeedChoosePrice, 0);
		}
		else
		{
			price[0]=fyf_ca_ipp[itemIndex].m_wCurTppTapPrice;
			price[1]=fyf_ca_ipp[itemIndex].m_wCurTppNoTapPrice;
			price[2]=fyf_ca_ipp[itemIndex].m_wCurCppTapPrice;
			price[3]=fyf_ca_ipp[itemIndex].m_wCurCppNoTapPrice;
			GUI_ENC_API_SetValue(ID_LBX_Price_BookUnbook, WDT_LBX_SV_INDEX, key, 0);
			if(price[key])
			{
				sprintf(g_ui_app_str[11],"%s:  %s",pDic[RS_BookPrice],g_ca_str2_p[key]);
				GUI_ENC_API_SetValue(ID_STA_Line3_BookUnbook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[11], 0);
				_ca_ipp_priceBSelected = key;
				GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Line3_BookUnbook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
				_ca_ipp_priceBSelected = 0xff;
				GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, (BU32)RS_InvalidPriceChooseAnother, 0);
			}
		}
		break;
	case GUI_VK_UP:
	case GUI_VK_DOWN:
		if(g_ipp_amount <= 1)return;
		_ca_ipp_priceBSelected = 0xff;
		CA_BookUnbook_ShowProgInfo(itemIndex);
		break;
	case GUI_VK_P_DOWN:	
		if(g_ipp_amount <= G_IPP_NUM_IN_PAGE)return;
		_ca_ipp_priceBSelected = 0xff;
#if 0
		remainder = g_ipp_amount%IPP_NUM_IN_PAGE;
		if(!remainder) remainder = IPP_NUM_IN_PAGE;
		
		if(index < IPP_NUM_IN_PAGE)
		{
			if(index < remainder)
			{
				index += g_ipp_amount - remainder;
			}
			else
			{
				index = g_ipp_amount-1;
			}
		}
		else
		{
			index -= IPP_NUM_IN_PAGE;
		}
		GUI_ENC_API_SetValue(ID_LBX_Ipp_BookUnbook, WDT_LBX_SV_INDEX, index, 0);
#endif
		CA_BookUnbook_ShowProgInfo(itemIndex);
		break;
	case GUI_VK_P_UP:	
		if(g_ipp_amount <= G_IPP_NUM_IN_PAGE)return;
		_ca_ipp_priceBSelected = 0xff;
#if 0
		remainder = g_ipp_amount%IPP_NUM_IN_PAGE;
		if(!remainder) remainder = 6;
		
		index += IPP_NUM_IN_PAGE;
		if(index >= g_ipp_amount)
		{
			if(index >= g_ipp_amount - remainder + IPP_NUM_IN_PAGE)//最后一页
			{
				index = index%IPP_NUM_IN_PAGE;
			}
			else	//倒数第二页
			{
				index = g_ipp_amount - 1;
			}

		}
		GUI_ENC_API_SetValue(ID_LBX_Ipp_BookUnbook, WDT_LBX_SV_INDEX, index, 0);
#endif
		CA_BookUnbook_ShowProgInfo(itemIndex);
		break;
	case GUI_VK_SELECT:		
		if(g_ipp_amount == 0)return;
		if(IsBooked)//已订购
		{
			g_ipp_BookUnbookState = CA_TO_UNBOOK;
			GUI_ENC_API_SetValue(ID_STA_BookUnbookInfo, WDT_STA_SV_DATAID, RS_UnbookCurProg, 0);
			GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Ok_Cancel, 0);
			GUI_ENC_API_SwitchState(ID_DLG_BookUnbookInfo, 1);

		}
		else if(fyf_ca_ipp[itemIndex].m_wBookedPrice)
		{
			g_ipp_BookUnbookState = CA_TO_BOOK;
			GUI_ENC_API_SetValue(ID_STA_BookUnbookInfo, WDT_STA_SV_DATAID, RS_BookCurProg, 0);
			GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Ok_Cancel, 0);
			GUI_ENC_API_SwitchState(ID_DLG_BookUnbookInfo, 1);
		}
 		else
 		{
			if(_ca_ipp_priceBSelected == 0xff)
			{
				GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, RS_ChoosePriceBeforeBook, 0);
			}
			else
			{
				g_ipp_BookUnbookState = CA_TO_BOOK;
				price[0]=fyf_ca_ipp[itemIndex].m_wCurTppTapPrice;
				price[1]=fyf_ca_ipp[itemIndex].m_wCurTppNoTapPrice;
				price[2]=fyf_ca_ipp[itemIndex].m_wCurCppTapPrice;
				price[3]=fyf_ca_ipp[itemIndex].m_wCurCppNoTapPrice;
				fyf_ca_ipp[itemIndex].m_wBookedPrice = price[_ca_ipp_priceBSelected];
				fyf_ca_ipp[itemIndex].m_byBookedPriceType = _ca_ipp_priceBSelected;
				fyf_ca_ipp[itemIndex].m_byBookedInterval = fyf_ca_ipp[itemIndex].m_byCurInterval;
				if(IppFlag==0)
				{
					GUI_ENC_API_SetFocus(ID_TBX_BookUnbook);
					return ;
				}
				GUI_ENC_API_SetValue(ID_STA_BookUnbookInfo, WDT_STA_SV_DATAID, RS_BookCurProg, 0);
				GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Ok_Cancel, 0);
				GUI_ENC_API_SwitchState(ID_DLG_BookUnbookInfo, 1);
			}
		}
		
		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);		
	default:
		break;
	}
	GUI_ENC_API_SetValue (ID_SBR_BookUnbook, WDT_SBR_SV_VALUE,itemIndex,0);
}
void UI_APP_CA_BookUnbookIPP_TBXKey(BU32 key)
{
	BU32 index;
	BU32 remainder = 0;
	BU08 IsBooked;
	BU16 price[4];
	BU32 itemIndex,zhouqi = 0;
	
	GUI_ENC_API_GetValue(ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_INDEX, &index, 0);
	itemIndex = index+g_ipp_curPage*G_IPP_NUM_IN_PAGE;
	
	//GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
	IsBooked = (fyf_ca_ipp[itemIndex].m_byIppStatus&0x02)>>1;	
		
	switch(key)
	{
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		GUI_ENC_API_SetFocus(ID_LBP_Ipp_BookUnbook);
		GUI_ENC_API_SetValue(ID_TBX_BookUnbook, WDT_TBX_SV_DATA, (BU32)0, 0);
		break;
	case GUI_VK_SELECT:		
		if(g_ipp_amount == 0)return;

		g_ipp_BookUnbookState = CA_TO_BOOK;
		GUI_ENC_API_GetValue(ID_TBX_BookUnbook, WDT_TBX_SV_DATA, (&zhouqi), 0);
		if(zhouqi == 0)
		{
			GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_DATAID, RS_ZhouqiInvalid, 0);
			return ;
		}
		else
		{
			GUI_ENC_API_SetValue(ID_STA_Note_BookUnbook, WDT_STA_SV_STRING, (BU32)" ", 0);
		}
		fyf_ca_ipp[itemIndex].m_wIpptPeriod = zhouqi;
		GUI_ENC_API_SetValue(ID_STA_BookUnbookInfo, WDT_STA_SV_DATAID, RS_BookCurProg, 0);
		GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Ok_Cancel, 0);
		GUI_ENC_API_SwitchState(ID_DLG_BookUnbookInfo, 1);
		
		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SwitchState(ID_DLG_Ca, 1);		
	default:
		break;
	}
}
void UI_APP_CA_BookUnbookIPP_Change(void)
{
	BS32 state;

	GUI_ENC_API_GetValue(ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_ipp_curPage++;
			break;
		case -1:
			if(g_ipp_curPage == 0)
				g_ipp_curPage = g_ipp_pageNum -1;
			else
				g_ipp_curPage--;
			
			if(g_ipp_curPage == g_ipp_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_INDEX, G_IPP_NUM_IN_PAGE-1, 0);
			}
			break;
		default:
			break;
	}
	UI_APP_Ipp_ShowCurPage();//显示当前页

}
static BU08 ExitToFullScreen = 0;
void UI_APP_CA_BookUnbookInfo_Key(BU32 key)
{
	BU32 index;
	BU32 ret;
	BU32 itemIndex;
	char **pStr;
	BU32 lang;
	BU32 ID;
		
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pStr = StrEnIPP;
	}
	else
	{
		pStr = StrCnIPP;
	}	
	switch(key)
	{
	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_LBP_Ipp_BookUnbook, WDT_LBP_SV_INDEX, &index, 0);
		itemIndex = index+g_ipp_curPage*G_IPP_NUM_IN_PAGE;
		if(g_ipp_BookUnbookState == CA_TO_BOOK || g_ipp_BookUnbookState == CA_TO_UNBOOK)
		{
			g_ipp_BookUnbookState = CA_BOOK_NULL;
			ID = ID_STA_BookUnbookInfo;
			if(GUI_ENC_API_GetPrevState() == ID_DLG_BookUnbook)
			{
			    ret = FYF_API_ca_set_value(FYF_CA_SV_BOOK_UNBOOK,(BU32)&fyf_ca_ipp[itemIndex],0);				
			}
			else if(GUI_ENC_API_GetPrevState() == ID_DLG_RealtimeBook)
			{
				ret = FYF_API_ca_set_value(FYF_CA_SV_BOOK_UNBOOK,(BU32)&DVTCAS_RealTimeipp.ippvProgram,0);
			}
			else ret = FYF_CAERR_STB_UNKNOWN;
			if(FYF_OK == ret)
			{
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[0], 0);	
				if(GUI_ENC_API_GetPrevState() == ID_DLG_RealtimeBook)
				{
					ExitToFullScreen = 1;
				}
			}
			else if(FYF_CAERR_STB_MONEY_LACK == ret)
			{
				/* 钱不多，需要提示用户"预订成功，但余额不多，请及时充值" */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[1], 0);	
				if(GUI_ENC_API_GetPrevState() == ID_DLG_RealtimeBook)
				{
					ExitToFullScreen = 1;
				}
			}
			else if(FYF_CAERR_STB_NEED_PIN_PASS == ret)
			{
				/* 需要通过PIN验证 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[2], 0);	
			}
			else if(FYF_CAERR_STB_IC_COMMUNICATE == ret)
			{
				/* 与IC卡通讯参数错误 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[3], 0);	
			}
			else if(FYF_CAERR_STB_TVS_NOT_FOUND == ret)
			{
				/* 运营商ID无效 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[4], 0);	
			}
			else if(FYF_CAERR_STB_SLOT_NOT_FOUND == ret)
			{
				/* 钱包没有发现 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[5], 0);	
			}
			else if(FYF_CAERR_STB_VER_EXPIRED == ret)
			{
				/* 产品已经过期 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[6], 0);	
			}
			else if(FYF_CAERR_STB_OPER_INVALID == ret)
			{
				/* 产品已经存在，不能操作 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[7], 0);	
			}
			else if(FYF_CAERR_STB_NO_SPACE == ret)
			{
				/* 没有空间 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[8], 0);	
			}
			else if(FYF_CAERR_STB_PROD_NOT_FOUND == ret)
			{
				/* 产品没有发现 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[9], 0);	
			}
			else if(FYF_CAERR_STB_PRICE_INVALID == ret)
			{
				/* 价格无效 */
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[10], 0);	
			}
			else if(FYF_CAERR_STB_UNKNOWN == ret)
			{
				GUI_ENC_API_SetValue(ID, WDT_STA_SV_STRING, pStr[11], 0);	
			}

			GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Null, 0);
			GUI_ENC_API_SetValue(ID_TIM_BookUnbookInfo, WDT_TIM_SV_PLAY, 0, 0);
		}		
		else
		{
			GUI_ENC_API_SetValue(ID_STA_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Wait, 0);	
			GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Null, 0);
			GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);		
		}
		break;
	case GUI_VK_EXIT:
		GUI_ENC_API_SetValue(ID_STA_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Wait, 0);	
		GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Null, 0);
		GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);		
		break;
	default:
		break;
	}

}

void UI_APP_CA_BookUnbookInfo_Timer(void)
{
	GUI_ENC_API_SetValue(ID_TIM_BookUnbookInfo, WDT_TIM_SV_STOP, 0, 0);
	if(GUI_ENC_API_GetPrevState() == ID_DLG_BookUnbook)
	{
		GUI_ENC_API_SwitchState(ID_DLG_BookUnbook, 1);
	}
	else if(GUI_ENC_API_GetPrevState() == ID_DLG_RealtimeBook)
	{
		if(ExitToFullScreen == 1)
		{
			ExitToFullScreen = 0;
			FYF_API_ca_set_value(FYF_CA_SV_IPP_OVER,DVTCAS_RealTimeipp.ecmPid,0);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);							
		}
		else
		{
			GUI_ENC_API_SwitchState(ID_DLG_RealtimeBook, 1);
		}
	}
}

//实时购买IPP节目

void UI_APP_CA_RealTimeBook_Enter(void)
{
	BU08 IsBooked,IppFlag;
	BU08 *password = "........";

	GUI_ENC_API_SetValue(ID_TBX_RealtimeBook,WDT_TBX_SV_PASSWORD,(BU32)password,0);
	CA_IppInfoToStr(DVTCAS_RealTimeipp.ippvProgram,&IsBooked,&IppFlag);	
	GUI_ENC_API_SetValue(ID_STA_Name_RealtimeBook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[3], 0);
	GUI_ENC_API_SetValue(ID_STA_ProType_RealtimeBook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[5], 0);
	GUI_ENC_API_SetValue(ID_STA_Start_RealtimeBook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[0], 0);
	GUI_ENC_API_SetValue(ID_STA_Duration_RealtimeBook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[1], 0);
	GUI_ENC_API_SetValue(ID_STA_Booked_RealtimeBook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[2], 0);
	if(IsBooked)
	{
		GUI_ENC_API_SetValue(ID_LBX_RealtimeBook, WDT_LBX_SV_ALLNUM, 0, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_LBX_RealtimeBook, WDT_LBX_SV_DATA, (BU32)g_ca_str2_p, 4);
		GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PsPricePinBuy, 0);
	}
	if(IppFlag)
	{
		GUI_ENC_API_SetVisible(ID_STA_Zhouqi_RealtimeBook, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_TBX_Zhouqi_RealtimeBook, WS_DISABLE);	
		GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PsPricePinBuy, 0);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_Zhouqi_RealtimeBook, WS_VISIBLE);
		GUI_ENC_API_SetVisible(ID_TBX_Zhouqi_RealtimeBook, WS_VISIBLE);
		GUI_ENC_API_SetValue(ID_TBX_Zhouqi_RealtimeBook, WDT_TBX_SV_DATA, (BU32)(DVTCAS_RealTimeipp.ippvProgram.m_wIpptPeriod), 0);
		GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PsPricePinZhouqiBuy, 0);
	}	
	GUI_ENC_API_SetValue(ID_LBX_RealtimeBook, WDT_LBX_SV_INDEX, 0, 0);

}
void UI_APP_CA_RealTimeBook_End(void)
{

}
void UI_APP_CA_RealTimeBook_Key(BU32 key)
{
	BU32 index;
	BU08 IsBooked;
	char **pDic;
	BU32 lang;
	BU08 string[80];
	char **pStr2;
	BU16 price[4]=
	{
		DVTCAS_RealTimeipp.ippvProgram.m_wCurTppTapPrice,
		DVTCAS_RealTimeipp.ippvProgram.m_wCurTppNoTapPrice,
		DVTCAS_RealTimeipp.ippvProgram.m_wCurCppTapPrice,
		DVTCAS_RealTimeipp.ippvProgram.m_wCurCppNoTapPrice,
	};
	
	IsBooked = (DVTCAS_RealTimeipp.ippvProgram.m_byIppStatus&0x02)>>1;
	
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
		pStr2 = StrEnIPP;
	}
	else
	{
		pDic = DicData;
		pStr2 = StrCnIPP;
	}	
	sprintf(string,"%s: %s",pDic[RS_BookPrice],pStr2[14]);
	switch(key)
	{
	case GUI_VK_UP:
	case GUI_VK_DOWN:
		if(!IsBooked)
		{
			GUI_ENC_API_GetValue(ID_LBX_RealtimeBook, WDT_LBX_SV_INDEX, &index, 0);
			if(price[index])
			{
				sprintf(g_ui_app_str[11],"%s:  %s",pDic[RS_BookPrice],g_ca_str2_p[index]);
				GUI_ENC_API_SetValue(ID_STA_Booked_RealtimeBook, WDT_STA_SV_STRING, (BU32)g_ui_app_str[11], 0);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Booked_RealtimeBook, WDT_STA_SV_STRING, (BU32)string, 0);
			}
		}	
		break;
	case GUI_VK_SELECT:
		if(IsBooked)
		{
			GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PsBookedExit, 0);
		}
		else
		{
			GUI_ENC_API_GetValue(ID_LBX_RealtimeBook, WDT_LBX_SV_INDEX, &index, 0);
			if(price[index])
			{
				DVTCAS_RealTimeipp.ippvProgram.m_wBookedPrice = price[index];
				DVTCAS_RealTimeipp.ippvProgram.m_byBookedPriceType = index;
				DVTCAS_RealTimeipp.ippvProgram.m_byBookedInterval = DVTCAS_RealTimeipp.ippvProgram.m_byCurInterval;
				GUI_ENC_API_SetFocus(ID_TBX_RealtimeBook);
				GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PsErr3TimesLock, 0);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PsInvalidPriceChooseAnother, 0);
			}
		}		
		break;
	case GUI_VK_RECALL:
	case GUI_VK_EXIT:
		FYF_API_ca_set_value(FYF_CA_SV_IPP_OVER,DVTCAS_RealTimeipp.ecmPid,0);
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);		
	default:
		break;
	}
}
void UI_APP_CA_RealTimeBook_PinKey(BU32 key)
{
	PU08 str;
	BU08 TempStr[9]={0,0,0,0,0,0,0,0,0},IppFlag;
	BU32 StrLen = 0;
	BS32 ret;
	BU32 i;
	BU08 pin[CA_PIN_LEN+1]={0};
	IppFlag = (DVTCAS_RealTimeipp.ippvProgram.m_byIppStatus&0x08)>>3;
	
	switch(key)
	{
	case GUI_VK_0:
	case GUI_VK_1:
	case GUI_VK_2:
	case GUI_VK_3:
	case GUI_VK_4:
	case GUI_VK_5:
	case GUI_VK_6:
	case GUI_VK_7:
	case GUI_VK_8:
	case GUI_VK_9:
		GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, DATAID_NULL, 0);
		break;
	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_TBX_RealtimeBook,WDT_TBX_SV_PASSWORD,(BU32 *)&str,0);
		//if(GUI_API_Strlen(str) == 8)	
		{
			for(i=0;i<CA_PIN_LEN;i++)
			{
				pin[i] = str[i] - 0x30;
			}

			ret = FYF_API_ca_get_value(FYF_CA_SV_VERIFY_PIN,(BU32)pin,0);

			if(FYF_OK == ret)
			{//正确
				GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PsSuccess, 0);
				GUI_ENC_API_SetValue(ID_STA_BookUnbookInfo, WDT_STA_SV_DATAID, RS_BookCurProg, 0);
				GUI_ENC_API_SetValue(ID_STA_OKIco_BookUnbookInfo, WDT_STA_SV_DATAID, RS_Ok_Cancel, 0);
				if(IppFlag)
				{
					g_ipp_BookUnbookState = CA_TO_BOOK;
					GUI_ENC_API_SwitchState(ID_DLG_BookUnbookInfo, 1);
				}
				else
				{
					GUI_ENC_API_SetFocus(ID_TBX_Zhouqi_RealtimeBook);
				}
			}
			else if(ret == FYF_STB_PIN_LOCKED)
			{//被锁
				GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PinLocked, 0);
			}	
			else if(ret == FYF_STB_DATA_LEN_ERROR)
			{//错误
				GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_LenErr, 0);
 			}	
			else
			{//错误
				GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_PinErr, 0);
 			}	
		}
		break;
	case GUI_VK_LEFT:
		GUI_ENC_API_GetValue(ID_TBX_RealtimeBook,WDT_TBX_SV_PASSWORD,(BU32 *)&str,0);

		StrLen = GUI_API_Strlen(str);
		if( (GUI_API_Strlen(str) != 0))
		{
			str[StrLen-1] = '\0';
			GUI_API_Strcpy(TempStr,str);
			GUI_ENC_API_SetValue(ID_TBX_RealtimeBook,WDT_TBX_SV_PASSWORD,(BU32)TempStr,0);		
		}
		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SetFocus(ID_LBX_RealtimeBook);
		break;
	case GUI_VK_EXIT:
		FYF_API_ca_set_value(FYF_CA_SV_IPP_OVER,DVTCAS_RealTimeipp.ecmPid,0);
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);		
		break;
	default:
		break;
	}

}


void UI_APP_CA_RealTimeBook_ZhouqiKey(BU32 key)
{
	BU32 zhouqi = 0;

	switch(key)
	{
	case GUI_VK_SELECT:
		GUI_ENC_API_GetValue(ID_TBX_Zhouqi_RealtimeBook, WDT_TBX_SV_DATA, (&zhouqi), 0);
		if(zhouqi == 0)
		{
			GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_DATAID, RS_ZhouqiInvalid, 0);
			return ;
		}
		else
		{
			GUI_ENC_API_SetValue(ID_STA_Note_RealtimeBook, WDT_STA_SV_STRING, (BU32)" ", 0);
		}
		DVTCAS_RealTimeipp.ippvProgram.m_wIpptPeriod = zhouqi;
		g_ipp_BookUnbookState = CA_TO_BOOK;
		GUI_ENC_API_SwitchState(ID_DLG_BookUnbookInfo, 1);

		break;
	case GUI_VK_RECALL:
		GUI_ENC_API_SetFocus(ID_TBX_RealtimeBook);
		break;
	case GUI_VK_EXIT:
		FYF_API_ca_set_value(FYF_CA_SV_IPP_OVER,DVTCAS_RealTimeipp.ecmPid,0);
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);		
		break;
	default:
		break;
	}

}


