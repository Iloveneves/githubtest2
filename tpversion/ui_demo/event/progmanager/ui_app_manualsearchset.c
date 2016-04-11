#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

static BU08 g_search_StrengthStr[7];
static BU08 g_search_SnrStr[5];

static BU32 g_search_freq;
static BU32 g_end_search_freq;
static BU32 g_search_symRate;
static APP_CHN_QAM_E g_search_qam;
static BU32 g_old_search_freq = 0;
static BU32 g_old_search_symRate = 0;
static BU32 g_old_search_qam = 0;
static BU08 g_search_tunerUsed = 0;
static BU32 delayLockTime = 0;
void UI_APP_ManualSearchSet_Timer(void);

void UI_APP_ManualSearchSet_Enter(void)
{
//	BU32 endFreq = 778;
	g_search_freq = APP_SYS_IPI_GetMainFreq();//602;
	g_search_symRate = 6875;
	g_search_qam = 2;
	g_old_search_freq = 0;
	g_old_search_symRate = 0;
	g_old_search_qam = 0;
	delayLockTime = 0;

	
	APP_Com_API_Stop_Signal_Dectect();
	APP_Com_API_StopProg(1);
	GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, (BU32)g_search_freq, 0);
	GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, (BU32)g_search_freq, 0);
	GUI_ENC_API_SetValue(ID_TBX_SymbRate_ManualSearchSet, WDT_TBX_SV_DATA, (BU32)g_search_symRate, 0);
	GUI_ENC_API_SetValue(ID_CBX_QAM_ManualSearchSet, WDT_CBX_SV_INDEX, 2, 0);
	GUI_ENC_API_SetValue(ID_CBX_NetSearch_ManualSearchSet, WDT_CBX_SV_INDEX, 0, 0);
	GUI_ENC_API_Enable(ID_TBX_EndFreq_ManualSearchSet, 1);
	GUI_ENC_API_SetValue(ID_STA_NumIco_ManualSearchSet, WDT_STA_SV_STYLE, STYLE_Digital_W46_H28, 0);
	GUI_ENC_API_SetValue(ID_STA_InNum_ManualSearchSet, WDT_STA_SV_DATAID, RS_ChangeNum, 0);
//	APP_Signal_API_TuneToNewParam(g_search_freq, g_search_symRate, g_search_qam);
//	Search_IPI_SetTunerParam(g_search_freq,g_search_symRate,g_search_qam);
//	UI_APP_ManualSearchSet_Timer();
	APP_Com_API_SetFullScreen();
//	GUI_ENC_API_SetValue(ID_STA_Num_ManualSearchSet, WDT_STA_SV_DATAID, RS_EnterNum, 0);
	GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_TBX_SymbRate_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_QAM_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
	GUI_ENC_API_SetValue(ID_CBX_NetSearch_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);

}
void UI_APP_ManualSearchSet_End(void)
{
	APP_Com_API_Start_Signal_Dectect();
}

void UI_APP_ManualSearchSet_Key(BU32 key)
{
	BU32 startFreq,endFreq,symRate,qam;
	APP_Search_Param_S param;
	BU08 str[4];
	BU16 chnNO = 0;
	BU32 index;
	switch(key)
	{
	case GUI_VK_EXIT:
		APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_JudgeChn0();
		if(APP_ChnDatabase_API_GetCurTypeChnNum() != 0)
		{
			chnNO = APP_ChnDatabase_API_GetCurChnNO();
			UI_APP_API_PlayChn(chnNO, chnNO, 1);
			//UI_APP_API_CheckLock_PlayChn_led (chnNO, chnNO, 0);
		}
		APP_Com_API_Start_Signal_Dectect();
		GUI_ENC_API_SwitchState(ID_DLG_SysSet,1);
		break;
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
		if(GUI_ENC_API_IsFocus(ID_TBX_StartFreq_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, &g_search_freq, 0);
			if(g_search_freq>865)
			{
				g_search_freq = 865;
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, g_search_freq, 0);
			}
			GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, g_search_freq, 0);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_EndFreq_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, &index, 0);
			GUI_ENC_API_GetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, &g_end_search_freq, 0);
			if(index>g_end_search_freq && g_end_search_freq>100)
			{
				g_end_search_freq=index;
				GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, g_end_search_freq, 0);
			}
			else
			{
				if(g_end_search_freq>865)
				{
					g_end_search_freq = 865;
					GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, g_end_search_freq, 0);
				}
			}
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_SymbRate_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_TBX_SymbRate_ManualSearchSet, WDT_TBX_SV_DATA, &g_search_symRate, 0);
		}
		delayLockTime = 0;
		break;
	case GUI_VK_LEFT:
	case GUI_VK_RIGHT:
		if(GUI_ENC_API_IsFocus(ID_CBX_QAM_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_QAM_ManualSearchSet, WDT_CBX_SV_INDEX, (BU32*)&g_search_qam, 0);
			delayLockTime = 0;
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_NetSearch_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_NetSearch_ManualSearchSet, WDT_CBX_SV_INDEX, &index, 0);
			if(index)
			{
					GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);;//GUI_ENC_API_Enable(ID_TBX_EndFreq_ManualSearchSet, 0);
			}
			else
			{
					GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);//GUI_ENC_API_Enable(ID_TBX_EndFreq_ManualSearchSet, 1);
			}
			GUI_ENC_API_Update(ID_TBX_EndFreq_ManualSearchSet, 1);

		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_StartFreq_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, &g_search_freq, 0);
			if(g_search_freq>865)
			{
				g_search_freq=865;
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, g_search_freq, 0);
			}
			GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, g_search_freq, 0);
			delayLockTime = 0;
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_EndFreq_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, &index, 0);
			GUI_ENC_API_GetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, &g_end_search_freq, 0);
			if(index>g_end_search_freq && g_end_search_freq>100)
			{
					g_end_search_freq=index;
					GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, g_end_search_freq, 0);
			}
			else
			{
				if(g_end_search_freq>865)
				{
					g_end_search_freq=865;
					GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, g_end_search_freq, 0);
				}
			}
			delayLockTime = 0;
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_SymbRate_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_TBX_SymbRate_ManualSearchSet, WDT_TBX_SV_DATA, &g_search_symRate, 0);
			delayLockTime = 0;
		}	
		break;
	case GUI_VK_UP:
		if(GUI_ENC_API_IsFocus(ID_TBX_StartFreq_ManualSearchSet))
		{
			GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_NetSearch_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
	
			GUI_ENC_API_SetValue(ID_STA_NumIco_ManualSearchSet, WDT_STA_SV_STYLE, STYLE_SelectLR_W46_H28, 0);
			GUI_ENC_API_SetValue(ID_STA_InNum_ManualSearchSet, WDT_STA_SV_DATAID, RS_SwitchItem, 0);
			GUI_ENC_API_SetFocus(ID_CBX_NetSearch_ManualSearchSet);
			GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, &startFreq, 0);
			if (startFreq<51)
			{
				startFreq = 51;
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, startFreq, 0);
			}
			else if (startFreq > 865)
			{
				startFreq = 865;
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, startFreq, 0);
			}	
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_EndFreq_ManualSearchSet))
		{
			GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
			GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, &startFreq, 0);
			GUI_ENC_API_GetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, &endFreq, 0);
			if(endFreq < startFreq)
			{	
				GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, startFreq, 0);/*截止频率>=起始频率*/
			}
			if(endFreq>865)
			{
				endFreq = 865;
				GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, endFreq, 0);
			}
			GUI_ENC_API_SetFocus(ID_TBX_StartFreq_ManualSearchSet);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_SymbRate_ManualSearchSet))
		{
			GUI_ENC_API_GetValue(ID_CBX_NetSearch_ManualSearchSet, WDT_CBX_SV_INDEX, &index, 0);
			if(index)
			{
				GUI_ENC_API_SetValue(ID_TBX_SymbRate_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
				GUI_ENC_API_SetFocus(ID_TBX_StartFreq_ManualSearchSet);
			}
			else
			{	
				GUI_ENC_API_SetValue(ID_TBX_SymbRate_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
				GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
				GUI_ENC_API_SetFocus(ID_TBX_EndFreq_ManualSearchSet);
			}
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_QAM_ManualSearchSet))
		{
			GUI_ENC_API_SetValue(ID_CBX_QAM_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_SymbRate_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
			GUI_ENC_API_SetValue(ID_STA_NumIco_ManualSearchSet, WDT_STA_SV_STYLE, STYLE_Digital_W46_H28, 0);
			GUI_ENC_API_SetValue(ID_STA_InNum_ManualSearchSet, WDT_STA_SV_DATAID, RS_ChangeNum, 0);
			GUI_ENC_API_SetFocus(ID_TBX_SymbRate_ManualSearchSet);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_CBX_NetSearch_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_QAM_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_QAM_ManualSearchSet);
		}
		break;
	case GUI_VK_DOWN:
		if(GUI_ENC_API_IsFocus(ID_TBX_StartFreq_ManualSearchSet))
		{
	
			
			GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, &startFreq, 0);
			if(startFreq<51)
			{
				startFreq = 51;
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, startFreq, 0);
			}
			else if(startFreq>865)
			{
				startFreq = 865;
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, startFreq, 0);
			}

			GUI_ENC_API_GetValue(ID_CBX_NetSearch_ManualSearchSet, WDT_CBX_SV_INDEX, &index, 0);
			if(index)
			{
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
				GUI_ENC_API_SetValue(ID_TBX_SymbRate_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
				GUI_ENC_API_SetFocus(ID_TBX_SymbRate_ManualSearchSet);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
				GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
				GUI_ENC_API_SetFocus(ID_TBX_EndFreq_ManualSearchSet);
			}
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_EndFreq_ManualSearchSet))
		{
			GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_SymbRate_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
			GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, &startFreq, 0);
			GUI_ENC_API_GetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, &endFreq, 0);
			if(endFreq < startFreq)
			{	
				GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, startFreq, 0);/*截止频率>=起始频率*/
			}

			if(endFreq>865)
			{
				endFreq = 865;
				GUI_ENC_API_SetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, endFreq, 0);
			}

			GUI_ENC_API_SetFocus(ID_TBX_SymbRate_ManualSearchSet);
		}
		else if(GUI_ENC_API_IsFocus(ID_TBX_SymbRate_ManualSearchSet))
		{
			GUI_ENC_API_SetValue(ID_TBX_SymbRate_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_QAM_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			
			GUI_ENC_API_SetValue(ID_STA_NumIco_ManualSearchSet, WDT_STA_SV_STYLE, STYLE_SelectLR_W46_H28, 0);
			GUI_ENC_API_SetValue(ID_STA_InNum_ManualSearchSet, WDT_STA_SV_DATAID, RS_SwitchItem, 0);
			GUI_ENC_API_SetFocus(ID_CBX_QAM_ManualSearchSet);
		}
		else if(GUI_ENC_API_IsFocus(ID_CBX_QAM_ManualSearchSet))
		{
			GUI_ENC_API_SetValue(ID_CBX_QAM_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_CBX_NetSearch_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_NetSearch_ManualSearchSet);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_CBX_NetSearch_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_WHITE, 0);
			GUI_ENC_API_SetValue(ID_TBX_StartFreq_ManualSearchSet,GUI_SV_TEXTCOLOR ,COLOR_BLACK, 0);
			GUI_ENC_API_SetValue(ID_STA_NumIco_ManualSearchSet, WDT_STA_SV_STYLE, STYLE_Digital_W46_H28, 0);
			GUI_ENC_API_SetValue(ID_STA_InNum_ManualSearchSet, WDT_STA_SV_DATAID, RS_ChangeNum, 0);
			GUI_ENC_API_SetFocus(ID_TBX_StartFreq_ManualSearchSet);
		}
		break;
	case GUI_VK_SELECT:
		{
			GUI_ENC_API_SwitchState(ID_DLG_Search,1);
		}		
		break;
	default:
		break;
	}

}
void UI_APP_ManualSearchSet_Timer(void)
{
	BU32 lever,snr;
	
	++delayLockTime;
	if(delayLockTime >= 2)
	{
		if( g_old_search_freq != g_search_freq
			|| g_old_search_symRate != g_search_symRate
			|| g_old_search_qam != g_search_qam)
		{
//			APP_IPI_SetPlayFlag(1);
			APP_Signal_API_TuneToNewParam(g_search_freq, g_search_symRate, g_search_qam);
			g_old_search_freq = g_search_freq;
			g_old_search_symRate = g_search_symRate;
			g_old_search_qam = g_search_qam;
		}		
	}

	
	if(g_search_tunerUsed == 0)
	{
		g_search_tunerUsed = 1;

		if(APP_SIGNAL_LOCK == APP_Signal_API_GetSignalState())		
		{
			GUI_ENC_API_SetValue(ID_PBR_Strength_ManualSearchSet,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
			GUI_ENC_API_SetValue(ID_PBR_Snr_ManualSearchSet,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
			lever = (BU32)APP_Signal_API_GetSignalLever();
			/*if(lever>=40)
				lever = lever + 4;
			else	
				lever = lever + 2;
				*/
				
			snr = (BU32)APP_Signal_API_GetSignalSNR();				
		}
		else
		{
			GUI_ENC_API_SetValue(ID_PBR_Strength_ManualSearchSet,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
			GUI_ENC_API_SetValue(ID_PBR_Snr_ManualSearchSet,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
			lever = 24;
			snr = 0;				
		}
	

		sprintf(g_search_StrengthStr,"%ddbuV",lever);
		GUI_ENC_API_SetValue(ID_STA_StrengthStr_ManualSearchSet,WDT_STA_SV_STRING,(BU32)g_search_StrengthStr,0);
		lever = lever*300/103;
		GUI_ENC_API_SetValue(ID_PBR_Strength_ManualSearchSet,WDT_PBR_SV_CHANGE,(BU32)lever,0);
		

		sprintf(g_search_SnrStr,"%ddB",snr);
		GUI_ENC_API_SetValue(ID_STA_SnrStr_ManualSearchSet,WDT_STA_SV_STRING,(BU32)g_search_SnrStr,0);
		snr = snr*300/40;
		GUI_ENC_API_SetValue(ID_PBR_Snr_ManualSearchSet,WDT_PBR_SV_CHANGE,(BU32)snr,0);	

		g_search_tunerUsed = 0;
	}
}

