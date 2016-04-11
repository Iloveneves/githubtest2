#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define FREQ_NUM_IN_PAGE  6

struct SignalDectec_s
{
	BU32 freq;
	BU32 sym;
	BU08 qam;
};

BU08 g_StrSignalDetect_item[FREQ_NUM_IN_PAGE][60];
struct SignalDectec_s g_SignalDetect_tempSigDec[100];
BU08 *g_StrSignalDetect[FREQ_NUM_IN_PAGE]  =
{
	g_StrSignalDetect_item[0],
	g_StrSignalDetect_item[1],
	g_StrSignalDetect_item[2],
	g_StrSignalDetect_item[3],
	g_StrSignalDetect_item[4],
	g_StrSignalDetect_item[5]
};
BU08 g_SignalDetect_lever[8];
BU08 g_SignalDetect_snr[6];
BU08 g_SignalDetect_ber[14];
BU32 g_SignalDetect_freqCount = 0;
BU32 g_SignalDetect_pageNum = 0;
BU32 g_SignalDetect_curPage = 0;
BU32 g_SignalDetect_numOfLastPage = 0;

void UI_APP_SignalDectec_Timer(void);

void SignalDetect_IPI_ShowSignalState(void)
{
	static BU32 s_lever=0,s_snr=0,s_ber=0;
	BU32 lever;
	BU32 snr;
	BU32 ber,berTempA,berTempB,berTempC;

	if(APP_SIGNAL_LOCK == APP_Signal_API_GetSignalState())		
	{
		GUI_ENC_API_SetValue(ID_PBR_Voltage_SignalCheck,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_SNR_SignalCheck,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_CER_SignalCheck,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		lever = (BU32)APP_Signal_API_GetSignalLever();
		
		/*adjust lever value*/
		/*
		if(lever>=40)
			lever = lever + 4;
		else	
			lever = lever + 2;
			*/
		snr = (BU32)APP_Signal_API_GetSignalSNR();	
		ber = APP_Signal_API_GetSignalBER();
	}
	else
	{
		GUI_ENC_API_SetValue(ID_PBR_Voltage_SignalCheck,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_SNR_SignalCheck,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_CER_SignalCheck,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		lever = 0;
		snr = 0;
		ber = 0x09630008;
	}
		
	if(s_lever != lever)
	{
		s_lever = lever;
		sprintf(g_SignalDetect_lever,"%ddbuV",lever);
		GUI_ENC_API_SetValue(ID_STA_StrengthNum_SignalCheck,WDT_STA_SV_STRING,(BU32)g_SignalDetect_lever,0);
		lever = lever*200/103;
		GUI_ENC_API_SetValue(ID_PBR_Voltage_SignalCheck,WDT_PBR_SV_CHANGE,(BU32)lever,0);

	}

	if(s_snr != snr)
	{
		s_snr = snr;
		sprintf(g_SignalDetect_snr,"%ddB",snr);
		GUI_ENC_API_SetValue(ID_STA_SnrNum_SignalCheck,WDT_STA_SV_STRING,(BU32)g_SignalDetect_snr,0);
		snr = snr*200/40;
		GUI_ENC_API_SetValue(ID_PBR_SNR_SignalCheck,WDT_PBR_SV_CHANGE,(BU32)snr,0);
	}
	
	if(s_ber != ber)
	{
		s_ber = ber;
		berTempA = ber&0xff000000;
		berTempA = berTempA>>24;
		berTempB = ber&0x00ff0000;
		berTempB = berTempB>>16;
		berTempC = ber&0x000000ff;
		sprintf(g_SignalDetect_ber,"%d.%dE-%d",berTempA,berTempB,berTempC + 1);
		if(berTempC>20)berTempC = 20;
		GUI_ENC_API_SetValue(ID_PBR_CER_SignalCheck,WDT_PBR_SV_CHANGE,(20-berTempC)*10,0);
		GUI_ENC_API_SetValue(ID_STA_BerNum_SignalCheck,WDT_STA_SV_STRING,(BU32)g_SignalDetect_ber,0);
	}
}
void UI_APP_SignalDectec_ShowCurPage(void)
{
	BU32 index = 0;
	BU32 itemIndex = 0;
	BU32 j = 0;
	BU32 itemNum = 0;

	if(g_SignalDetect_curPage >= g_SignalDetect_pageNum)
	{
		g_SignalDetect_curPage = 0;
	}
	itemNum = 0;
	for(index = 0;index<FREQ_NUM_IN_PAGE;index++)
	{
		itemIndex = g_SignalDetect_curPage*FREQ_NUM_IN_PAGE+index;
		if(itemIndex<g_SignalDetect_freqCount)
		{
			switch(g_SignalDetect_tempSigDec[itemIndex].qam)
			{
			case 0:
				j=16;
				break;
			case 1:
				j=32;
				break;
			case 2:
				j=64;
				break;
			case 3:
				j=128;
				break;
			case 4:
				j=256;
				break;
			default:
				j=64;
				break;
			}
			sprintf(g_StrSignalDetect[index], "%-3d %dMHz %dKbps %dQAM", itemIndex+1,g_SignalDetect_tempSigDec[itemIndex].freq, g_SignalDetect_tempSigDec[itemIndex].sym, j);
			itemNum++;
		}
	}
	GUI_ENC_API_SetValue (ID_LBP_SignalCheck, WDT_LBP_SV_DATA, (BU32)g_StrSignalDetect, itemNum);
}
void UI_APP_SignalDectec_Enter(void)
{
	BS08 StrSQL[200];
	BU32 i = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 curProgNo = 0; //当前节目号
	BU32 listIndex = 0;	//列表索引

	
	curProgNo = APP_ChnDatabase_API_GetCurChnNO();
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(),APP_ChnDatabase_API_GetCurGroupType(),curProgNo, &chnInfo))
	{
		g_SignalDetect_freqCount = 0;
		GUI_ENC_API_SetValue(ID_LBP_SignalCheck, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValue(ID_PBR_Voltage_SignalCheck,WDT_PBR_SV_CHANGE,0,0);
		GUI_ENC_API_SetValue(ID_PBR_SNR_SignalCheck,WDT_PBR_SV_CHANGE,0,0);
		GUI_ENC_API_SetValue(ID_PBR_CER_SignalCheck,WDT_PBR_SV_CHANGE,0,0);
		GUI_ENC_API_SetValue(ID_STA_StrengthNum_SignalCheck,WDT_STA_SV_STRING,(BU32)" ",0);
		GUI_ENC_API_SetValue(ID_STA_SnrNum_SignalCheck,WDT_STA_SV_STRING,(BU32)" ",0);
		GUI_ENC_API_SetValue(ID_STA_BerNum_SignalCheck,WDT_STA_SV_STRING,(BU32)" ",0);
		GUI_ENC_API_SetValue (ID_SBR_SignalCheck, WDT_SBR_SV_MAX, 0, 0);
		GUI_ENC_API_SetValue (ID_SBR_SignalCheck, WDT_SBR_SV_MIN, 0, 0);
		GUI_ENC_API_SetValue (ID_SBR_SignalCheck, WDT_SBR_SV_VALUE, 0, 0);		
		APP_Com_API_SetFullScreen();
		return;
	}
	SQL_API_Semaphore_Wait();
	sprintf (StrSQL, "SELECT DISTINCT 频率,调制方式,符号率 FROM program;");
	SQL_API_Select (StrSQL);
	g_SignalDetect_freqCount = SQL_API_GetTotalNum ();

	listIndex = 0;
	if(g_SignalDetect_freqCount == 0)
	{
		GUI_ENC_API_SetValue(ID_LBP_SignalCheck, WDT_LBP_SV_NUM, 0, 0);
		GUI_ENC_API_SetValue(ID_PBR_Voltage_SignalCheck,WDT_PBR_SV_CHANGE,0,0);
		GUI_ENC_API_SetValue(ID_PBR_SNR_SignalCheck,WDT_PBR_SV_CHANGE,0,0);
		GUI_ENC_API_SetValue(ID_PBR_CER_SignalCheck,WDT_PBR_SV_CHANGE,0,0);
		GUI_ENC_API_SetValue(ID_STA_StrengthNum_SignalCheck,WDT_STA_SV_STRING,(BU32)" ",0);
		GUI_ENC_API_SetValue(ID_STA_SnrNum_SignalCheck,WDT_STA_SV_STRING,(BU32)" ",0);
		GUI_ENC_API_SetValue(ID_STA_BerNum_SignalCheck,WDT_STA_SV_STRING,(BU32)" ",0);
	}
	else
	{	
		if(g_SignalDetect_freqCount>100)
		{
			g_SignalDetect_freqCount = 100;
		}
		g_SignalDetect_pageNum = (g_SignalDetect_freqCount-1)/FREQ_NUM_IN_PAGE+1;
		g_SignalDetect_numOfLastPage = g_SignalDetect_freqCount%FREQ_NUM_IN_PAGE;
		if(0 == g_SignalDetect_numOfLastPage)
		{
			g_SignalDetect_numOfLastPage = FREQ_NUM_IN_PAGE;
		}
		for(i = 0;i<g_SignalDetect_freqCount;i++)
		{
			SQL_API_GotoRow (i);
			g_SignalDetect_tempSigDec[i].freq = (BU32)SQL_API_GetData (0, 0, 0, APP_DATA_TYPE_NUM);
			g_SignalDetect_tempSigDec[i].qam =  (BU08)SQL_API_GetData (1, 0, 0, APP_DATA_TYPE_NUM);
			g_SignalDetect_tempSigDec[i].sym =	(BU32)SQL_API_GetData (2, 0, 0, APP_DATA_TYPE_NUM);
			if(g_SignalDetect_tempSigDec[i].freq == chnInfo.m_FreqMHz && g_SignalDetect_tempSigDec[i].qam == chnInfo.m_QAM && g_SignalDetect_tempSigDec[i].sym == chnInfo.m_SymKbps)
			{
				listIndex = i;
			}
		}
		g_SignalDetect_curPage = listIndex/FREQ_NUM_IN_PAGE;
		GUI_ENC_API_SetValue (ID_SBR_SignalCheck, WDT_SBR_SV_MAX, g_SignalDetect_freqCount-1, 0);
		GUI_ENC_API_SetValue (ID_SBR_SignalCheck, WDT_SBR_SV_MIN, 0, 0);
		GUI_ENC_API_SetValue (ID_SBR_SignalCheck, WDT_SBR_SV_VALUE, listIndex, 0);		
		GUI_ENC_API_SetValueUnPaint(ID_LBP_SignalCheck, WDT_LBP_SV_INDEX, (BU32)(listIndex%FREQ_NUM_IN_PAGE), 0);
		UI_APP_SignalDectec_ShowCurPage ();
	}
	APP_Com_API_SetFullScreen();
	SQL_API_Semaphore_Release();
	UI_APP_SignalDectec_Timer ();
}

void UI_APP_SignalDectec_End(void)
{
	BU32 i = 0;
	BU08 str[4];

	if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
	{
		return;
	}
//	UI_APP_API_CheckPlay_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),UI_APP_CurChnbPlay());
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 0);
}
void UI_APP_SignalDectec_Key(BU32 key)
{
	BU32 index;
	BU32 remainder=0;
	BU32 itemIndex;
	BU32 count, i;
	ChnDataBase_ChnInfo_S ChnInfo;
	BU08 flag = 0;

	switch(key)
	{
	case GUI_VK_EXIT:
		GUI_ENC_API_SwitchState (ID_DLG_ChnInfo, 1);
		break;
	case GUI_VK_RECALL:
	case GUI_VK_SELECT:
	case GUI_VK_MENU:
		GUI_ENC_API_SwitchState(ID_DLG_SysSet, 1);
		break;
	}
	if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)return;

	GUI_ENC_API_GetValue(ID_LBP_SignalCheck, WDT_LBP_SV_INDEX, &index, 0);
	itemIndex = g_SignalDetect_curPage*FREQ_NUM_IN_PAGE+index;
#if 0
	switch(key)
	{
	case GUI_VK_UP:		
	case GUI_VK_DOWN:
		break;
	case GUI_VK_P_UP:
		
		remainder = g_SignalDetect_freqCount%FREQ_NUM_IN_PAGE;
		if(!remainder) remainder = FREQ_NUM_IN_PAGE;
		
		if(index < FREQ_NUM_IN_PAGE)
		{
			if(index < remainder)
			{
				index += g_SignalDetect_freqCount - remainder;
			}
			else
			{
				index = g_SignalDetect_freqCount-1;
			}
		}
		else
		{
			index -= FREQ_NUM_IN_PAGE;
		}
		GUI_ENC_API_SetValue(ID_LBX_SignalCheck, WDT_LBX_SV_INDEX, index, 0);
		break;
	case GUI_VK_P_DOWN:	
		remainder = g_SignalDetect_freqCount%FREQ_NUM_IN_PAGE;
		if(!remainder) remainder = 6;
		
		index += FREQ_NUM_IN_PAGE;
		if(index >= g_SignalDetect_freqCount)
		{
			if(index >= g_SignalDetect_freqCount - remainder + FREQ_NUM_IN_PAGE)//最后一页
			{
				index = index%FREQ_NUM_IN_PAGE;
			}
			else	//倒数第二页
			{
				index = g_SignalDetect_freqCount - 1;
			}

		}
		GUI_ENC_API_SetValue(ID_LBX_SignalCheck, WDT_LBX_SV_INDEX, index, 0);
		break;

	}
#endif
	GUI_ENC_API_SetValue (ID_SBR_SignalCheck, WDT_SBR_SV_VALUE, itemIndex, 0);
	count = APP_ChnDatabase_API_GetCurTypeChnNum();
	for (i = 0; i < count; i++)
	{
		APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(),APP_ChnDatabase_API_GetCurGroupType(), i, &ChnInfo);
		if (ChnInfo.m_FreqMHz == g_SignalDetect_tempSigDec[itemIndex].freq)
		{
			APP_ChnDatabase_API_SetCurChnNO(i);
			UI_APP_API_PlayChn(i , i, 1);
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		APP_Signal_API_TuneToNewParam(g_SignalDetect_tempSigDec[itemIndex].freq, g_SignalDetect_tempSigDec[itemIndex].sym, g_SignalDetect_tempSigDec[itemIndex].qam);
}
void UI_APP_SignalDectec_Timer(void)
{
	if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)return;

	SignalDetect_IPI_ShowSignalState();
}

void UI_APP_SignalDectec_Change(void)
{
	BS32 state;

	GUI_ENC_API_GetValue(ID_LBP_SignalCheck, WDT_LBP_SV_STATE, (BU32 *)&state,0);
	switch(state)
	{
		case 1:
			g_SignalDetect_curPage++;
			break;
		case -1:
			if(g_SignalDetect_curPage == 0)
				g_SignalDetect_curPage = g_SignalDetect_pageNum -1;
			else
				g_SignalDetect_curPage--;
			
			if(g_SignalDetect_curPage == g_SignalDetect_pageNum-2)
			{
				GUI_ENC_API_SetValueUnPaint(ID_LBP_SignalCheck, WDT_LBP_SV_INDEX, FREQ_NUM_IN_PAGE-1, 0);
			}
			break;
		default:
			break;
	}
	UI_APP_SignalDectec_ShowCurPage();//显示当前页

}

