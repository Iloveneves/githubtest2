#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

#define SEARCH_LBX_NUM 5
#define SEARCH_LBX_CHN_LEN 20

static BU08 g_search_tvList[SEARCH_LBX_NUM][SEARCH_LBX_CHN_LEN];
static BU08 g_search_radioList[SEARCH_LBX_NUM][SEARCH_LBX_CHN_LEN];
static BU08 g_search_tv[SEARCH_LBX_CHN_LEN];
static BU08 g_search_radio[SEARCH_LBX_CHN_LEN];
static BU16 g_search_tvNum=0;
static BU16 g_search_radioNum=0;
static BU16 g_search_broadcastNum=0;
static BU08 g_search_strTemp[60];
static BU08 g_search_ProgCount[10];
static BU08 g_search_tv_tbxIndex;
static BU08 g_search_radio_tbxIndex;
static BU08 g_search_StrengthStr[7];
static BU08 g_search_SnrStr[5];
static BU08 g_search_ProgressStr[5];
void UI_Search_API_GetSearchTvRadioNum(BU16 *tvnum,BU16 *radionum, BU16 *broadcastnum)
{
	*tvnum = g_search_tvNum;
	*radionum = g_search_radioNum;
	*broadcastnum = g_search_broadcastNum;
}
void UI_Search_API_SetSearchTvRadioNum(BU16 tvnum,BU16 radionum, BU16 broadcastnum)
{
	g_search_tvNum = tvnum;
	g_search_radioNum = radionum;
	g_search_broadcastNum = broadcastnum;
}

void Search_IPI_AddChn(ChnDataBase_ChnInfo_S *chnInfoP,APP_CHN_AV_TYPE_E avType) 
{
	BU32 language;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &language);
	if(APP_CHN_AV_TYPE_TV_e == avType)
	{
		g_search_tvNum ++;	
		APP_PRINT_INFO("g_search_tvNum = %d\n", g_search_tvNum);
		if(language == APP_SYS_LANGUAGE_CHINESE_e)
			sprintf(g_search_ProgCount,"%s:%03d",DicData[RS_TV],g_search_tvNum);
		else
			sprintf(g_search_ProgCount,"%s:%03d",DicDataEnglish[RS_TV],g_search_tvNum);
	//	GUI_ENC_API_SetValue(ID_STA_TV_Search, WDT_STA_SV_STRING,(BU32)g_search_ProgCount,0);
		
		g_search_tv_tbxIndex++;
	    g_search_tv_tbxIndex=g_search_tv_tbxIndex%SEARCH_LBX_NUM;	
		chnInfoP->m_ChnName[SEARCH_LBX_CHN_LEN-7]=0;
		if(chnInfoP->m_Ca)
		{
			APP_Com_Api_Sprintf(g_search_tvList[g_search_tv_tbxIndex],"%03d $ %s",g_search_tvNum,chnInfoP->m_ChnName);
		}
		else
		{
			APP_Com_Api_Sprintf(g_search_tvList[g_search_tv_tbxIndex],"%03d   %s",g_search_tvNum,chnInfoP->m_ChnName);
		}
		APP_Com_Api_Sprintf(g_search_tv, "电视:%03d", g_search_tvNum);
		GUI_ENC_API_SetValue(ID_LBX_TV_Search, WDT_LBX_SV_ADDDATA, (BU32)(g_search_tvList[g_search_tv_tbxIndex]), 0);
		GUI_ENC_API_SetValue(ID_STA_TVNum_Search, WDT_STA_SV_STRING, (BU32)g_search_tv, 0);
	}
	else if(APP_CHN_AV_TYPE_RD_e == avType)
	{
		g_search_radioNum++;
		if(language == APP_SYS_LANGUAGE_CHINESE_e)
			sprintf(g_search_ProgCount,"%s:%03d",DicData[RS_Radio],g_search_radioNum);
		else
			sprintf(g_search_ProgCount,"%s:%03d",DicDataEnglish[RS_Radio],g_search_radioNum);
	//	GUI_ENC_API_SetValue(ID_STA_Radio_Search, WDT_STA_SV_STRING,(BU32)g_search_ProgCount,0);
		
		g_search_radio_tbxIndex++;
	    g_search_radio_tbxIndex=g_search_radio_tbxIndex%SEARCH_LBX_NUM;		
		chnInfoP->m_ChnName[SEARCH_LBX_CHN_LEN-7]=0;
		if(chnInfoP->m_Ca)
		{
			APP_Com_Api_Sprintf(g_search_radioList[g_search_radio_tbxIndex],"%03d $ %s",g_search_radioNum,chnInfoP->m_ChnName);
		}
		else
		{
			APP_Com_Api_Sprintf(g_search_radioList[g_search_radio_tbxIndex],"%03d   %s",g_search_radioNum,chnInfoP->m_ChnName);
		}
		APP_Com_Api_Sprintf(g_search_radio, "广播:%03d", g_search_radioNum);
		GUI_ENC_API_SetValue(ID_LBX_Radio_Search, WDT_LBX_SV_ADDDATA, (BU32)(g_search_radioList[g_search_radio_tbxIndex]), 0);
		GUI_ENC_API_SetValue(ID_STA_RadioNum_Search, WDT_STA_SV_STRING, (BU32)g_search_radio, 0);
	}
	else if(APP_CHN_AV_TYPE_BROADCAST_e == avType)
	{
		g_search_broadcastNum++;
	}

}

void Search_IPI_EndOfSave(void)
{

}
void Search_IPI_UpdateSignalInfo(APP_Signal_BaseInfo_SP signalBaseInfo)
{
	BU32 barLen;
	if(APP_SIGNAL_LOCK == signalBaseInfo->m_BLock)		
	{
//		GUI_ENC_API_SetValue(ID_PBR_Strength_Search,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_Snr_Search,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		barLen = (signalBaseInfo->m_SignalLever)*300/103;
		GUI_API_Memset(g_search_StrengthStr, 0, 5);
		APP_Com_Api_Sprintf(g_search_StrengthStr,"%udbuV",(signalBaseInfo->m_SignalLever));
	}
	else
	{
//		GUI_ENC_API_SetValue(ID_PBR_Strength_Search,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_Snr_Search,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		barLen = 24 * 300 / 103;
		GUI_API_Memset(g_search_StrengthStr, 0, 5);
		APP_Com_Api_Sprintf(g_search_StrengthStr,"%udbuV",24);
	}


	GUI_ENC_API_SetValue(ID_PBR_Snr_Search,WDT_PBR_SV_CHANGE,(BU32)barLen,0);//信噪比
	GUI_ENC_API_SetValue(ID_STA_StrengthNum_Search, WDT_STA_SV_STRING,(BU32)g_search_StrengthStr,0);
/*
	barLen = (signalBaseInfo->m_SignalLever)*300/103;	
	GUI_API_Memset(g_search_StrengthStr, 0, 7);
	APP_Com_Api_Sprintf(g_search_StrengthStr,"%udbuv",(signalBaseInfo->m_SignalLever));
	GUI_ENC_API_SetValue(ID_PBR_Strength_Search,WDT_PBR_SV_CHANGE,(BU32)barLen,0);//信号电平
	GUI_ENC_API_SetValue(ID_STA_StrengthNum_Search, WDT_STA_SV_STRING,(BU32)g_search_StrengthStr,0);
*/	
}
void Search_IPI_UpdateProgress(BU08 progress)
{
	BU32 lang;

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	
	GUI_API_Memset(g_search_ProgressStr, 0, 5);
	sprintf(g_search_ProgressStr,"%d%%",progress);
	GUI_ENC_API_SetValue(ID_PBR_Progress_Search,WDT_PBR_SV_CHANGE,(BU32)(progress*3),0);//进度条的长度为300，所以要乘以3
	GUI_ENC_API_SetValue(ID_STA_ProgressNum_Search,WDT_STA_SV_STRING,(BU32)g_search_ProgressStr,0);
	if(progress == 100)
	{
	//	if(g_search_tvNum == 0 && g_search_radioNum == 0 && g_search_broadcastNum == 0)
	//	{
		//	GUI_ENC_API_SetValue(ID_STA_Line1_SearchInfo,WDT_STA_SV_DATAID,DATAID_NULL,0);
		//	GUI_ENC_API_SetValue(ID_STA_Line2_SearchInfo,WDT_STA_SV_DATAID,RS_NoProgSearched,0);
		//	GUI_ENC_API_SetValue(ID_STA_Line3_SearchInfo,WDT_STA_SV_DATAID,DATAID_NULL,0);
		//}
		//else
		{
			GUI_API_Memset(g_search_strTemp, 0, 60);
			if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
			{
				sprintf(g_search_strTemp,"TV : %d  Radio : %d,Broadcast : %d",g_search_tvNum,g_search_radioNum,g_search_broadcastNum);
			}
			else
			{
				sprintf(g_search_strTemp,"电视: %d 广播: %d 信息: %d",g_search_tvNum,g_search_radioNum,g_search_broadcastNum);
			}
			GUI_ENC_API_SetValue(ID_STA_Line2_SearchInfo,WDT_STA_SV_STRING,(BU32)g_search_strTemp,0);
			GUI_ENC_API_SetValue(ID_STA_Line3_SearchInfo,WDT_STA_SV_DATAID,RS_SaveWait,0);
		}
		APP_SYS_API_SetTunerAfterSearch(999,9999,1);
		GUI_ENC_API_SwitchState(ID_DLG_SearchInfo,1);
	}
	
}
void Search_IPI_UpdateFreq(APP_Tuner_FreqInfo_SP freqInfo,BU32 NO,BU32 Num)
{
	BU32 qam = 64;
	BU32 language;
	switch(freqInfo->m_QAM)
	{
		case APP_CHN_QAM_16_e:
			qam = 16;
			break;
		case APP_CHN_QAM_32_e:
			qam = 32;
			break;			
		case APP_CHN_QAM_64_e:
			qam = 64;
			break;			
		case APP_CHN_QAM_128_e:
			qam = 128;
			break;			
		case APP_CHN_QAM_256_e:	
			qam = 256;
			break;
		default:
			qam = 64;
			break;
	}

	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &language);
	if(language == APP_SYS_LANGUAGE_CHINESE_e)
	{
		APP_Com_Api_Sprintf(g_search_strTemp, "%s [%d/%d]%dMHz/%dKbps/%dQAM",DicData[RS_FreqInfo],NO,Num,freqInfo->m_FreqMHz,freqInfo->m_SymKbps,qam);
	}
	else
	{
		APP_Com_Api_Sprintf(g_search_strTemp, "%s [%d/%d]%dMHz/%dKbps/%dQAM",DicDataEnglish[RS_FreqInfo],NO,Num,freqInfo->m_FreqMHz,freqInfo->m_SymKbps,qam);
	}
	GUI_ENC_API_SetValue(ID_STA_Freq_Search, WDT_STA_SV_STRING,(BU32)g_search_strTemp,0);
	
}
//APP_Search_Param_S manual_param;

//搜索
void UI_APP_Search_Enter(void)
{
	BU32 curID;
	APP_Search_Param_S param;
	ChnDataBase_ChnInfo_S chnInfo;
	
	//APP_Com_API_ShowLED("C001");
	
	APP_Com_API_SetFullScreen();
	if(APP_ChnDatabase_API_GetCurTypeChnNum())
	{
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
		{
			return;
		}
		UI_APP_API_SetCurChnProgID(chnInfo.m_ProgId);
	}
	else
	{
		UI_APP_API_SetCurChnProgID(0);
	}
	
	g_search_tvNum = 0;
	g_search_radioNum = 0;
	g_search_broadcastNum= 0;
	APP_Com_Api_Sprintf(g_search_radio, "广播:%03d", g_search_radioNum);
	GUI_ENC_API_SetValue(ID_STA_RadioNum_Search, WDT_STA_SV_STRING, (BU32)g_search_radio, 0);
	APP_Com_Api_Sprintf(g_search_tv, "电视:%03d", g_search_tvNum);
	GUI_ENC_API_SetValue(ID_STA_TVNum_Search, WDT_STA_SV_STRING, (BU32)g_search_tv, 0);
	
	GUI_ENC_API_SetValue(ID_LBX_TV_Search, WDT_LBX_SV_ALLNUM, 0, 0);
	GUI_ENC_API_SetValue(ID_LBX_Radio_Search, WDT_LBX_SV_ALLNUM, 0, 0);	
	GUI_ENC_API_SetValue(ID_STA_Freq_Search, WDT_STA_SV_STRING,(BU32)" ",0);
//	GUI_ENC_API_SetValue(ID_PBR_Strength_Search,WDT_PBR_SV_CHANGE,0,0);
	GUI_ENC_API_SetValue(ID_PBR_Snr_Search,WDT_PBR_SV_CHANGE,0,0);
	GUI_ENC_API_SetValue(ID_PBR_Progress_Search,WDT_PBR_SV_CHANGE,0,0);
	GUI_ENC_API_SetValue(ID_STA_StrengthNum_Search, WDT_STA_SV_STRING,(BU32)"0dbuV",0);
//	GUI_ENC_API_SetValue(ID_STA_SnrNum_Search, WDT_STA_SV_STRING,(BU32)"0dB",0);
	GUI_ENC_API_SetValue(ID_STA_ProgressNum_Search, WDT_STA_SV_STRING,(BU32)"0%",0);

	GUI_ENC_API_GetValue(ID_MNU_SysSet,WDT_MNU_SV_CURDATAID,&curID,0);

	if(RS_ManualSearch == curID)
	{
		GUI_ENC_API_GetValue(ID_TBX_StartFreq_ManualSearchSet, WDT_TBX_SV_DATA, (BU32 *)&param.m_StartFreqMHz,0);
		GUI_ENC_API_GetValue(ID_TBX_EndFreq_ManualSearchSet, WDT_TBX_SV_DATA, (BU32 *)&param.m_StopFreqMHz, 0);
		GUI_ENC_API_GetValue(ID_TBX_SymbRate_ManualSearchSet, WDT_TBX_SV_DATA, (BU32 *)&param.m_SymKbps, 0);
		GUI_ENC_API_GetValue(ID_CBX_QAM_ManualSearchSet, WDT_CBX_SV_INDEX, (BU32 *)&param.m_QamMode, 0);
		GUI_ENC_API_GetValue(ID_CBX_NetSearch_ManualSearchSet, WDT_CBX_SV_INDEX, (BU32 *)&param.m_bNitOption, 0);
		if(param.m_StopFreqMHz < param.m_StartFreqMHz)
		{	
			param.m_StopFreqMHz = param.m_StartFreqMHz;
		}
		param.m_SrchType =  APP_SEARCH_TYPE_MANUAL_e;
	}
	else if(RS_AutoSearch == curID)
	{
		param.m_SrchType =  APP_SEARCH_TYPE_AUTO_e;
	}
	else if(RS_AllSearch == curID)
	{
		param.m_SrchType =  APP_SEARCH_TYPE_ALL_e;
	}
	else
	{
		param.m_SrchType =  APP_SEARCH_TYPE_AUTO_e;

	}
	param.m_Callback_AddAChn = Search_IPI_AddChn;
	param.m_Callback_EndOfSave = Search_IPI_EndOfSave;
	param.m_Callback_UpdateFreqInfo =Search_IPI_UpdateFreq ;
	param.m_Callback_UpdateProgress = Search_IPI_UpdateProgress;
	param.m_Callback_UpdateSignalInfo = Search_IPI_UpdateSignalInfo;
	APP_Com_API_StopProg(1);
	APP_Com_API_SetEcmNULL();
	APP_Search_API_StartSearch(&param);
	
}
void UI_APP_Search_End(void)
{
	APP_API_SetHadUpdateChn();
}


void UI_APP_Search_Key(BU32 key)
{
	if(key == GUI_VK_EXIT)
	{
		APP_Search_API_HaltSearch();
	}
		
}

