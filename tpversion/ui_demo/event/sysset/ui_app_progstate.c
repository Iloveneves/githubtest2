#include "../../osd/include/resource.h"
#include "../../../../tpmiddle/include/app_common.h"

extern BU08 g_ui_app_str[14][80];
BU08 *g_prog_state_Str[14]=
{
	g_ui_app_str[0],
	g_ui_app_str[1],
	g_ui_app_str[2],
	g_ui_app_str[3],
	g_ui_app_str[4],
	g_ui_app_str[5],
	g_ui_app_str[6],
	g_ui_app_str[7],
	g_ui_app_str[8],
	g_ui_app_str[9],
	g_ui_app_str[10],
	g_ui_app_str[11],
	g_ui_app_str[12],
	g_ui_app_str[13],
};

void UI_APP_ProgState_Timer(void);
void ProgState_IPI_ShowChnInfo(void)
{
	BU32 freq=0;
	BU32 QAMTemp;	
	BU16 ChnNo;
	BU32 serviceid=0;
	ChnDataBase_ChnInfo_S chn;
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
	
	ChnNo = APP_ChnDatabase_API_GetCurChnNO();
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ChnNo, &chn))
	{
		return;
	}

	chn.m_ChnName[17]=0;
	if(chn.m_Ca)
	{
		sprintf(g_prog_state_Str[6],"%-12s %03d %s",pDic[RS_ProgName],ChnNo + APP_ChnDatabase_API_GetChn0Flag(),chn.m_ChnName);
	}
	else
	{
		sprintf(g_prog_state_Str[6],"%-12s %03d %s",pDic[RS_ProgName],ChnNo + APP_ChnDatabase_API_GetChn0Flag(),chn.m_ChnName);
	}
	//serviceid = chn.m_ProgId & 0xffff0000;
	//serviceid = serviceid>>16;
	sprintf(g_prog_state_Str[7],"%-12s %d/%d/%d/%d",pDic[RS_SID_TSID],chn.m_TsId,SeparateSevId(chn.m_ProgId),chn.m_ProgLCN, APP_SYS_IPI_GetNitVer());
	sprintf(g_prog_state_Str[8],"%-12s %04d/%04d/%04d",pDic[RS_VAPCR],chn.m_VideoPid,chn.m_AudioPid,chn.m_PcrPid);
	switch(chn.m_QAM)
	{
	case APP_CHN_QAM_16_e:
		QAMTemp = 16;
		break;
	case APP_CHN_QAM_32_e:
		QAMTemp = 32;
		break;		
	case APP_CHN_QAM_64_e:
		QAMTemp = 64;
		break;		
	case APP_CHN_QAM_128_e:
		QAMTemp = 128;
		break;		
	case APP_CHN_QAM_256_e:
		QAMTemp = 256;
		break;	
	default:
		QAMTemp = 64;
		break;

	}
	sprintf(g_prog_state_Str[9],"%-12s %dMHz/%dQAM/%4dKbps",pDic[RS_FreqInfo],chn.m_FreqMHz,QAMTemp,chn.m_SymKbps);
	
	GUI_ENC_API_SetValue(ID_STA_Line6_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[6],0);
	GUI_ENC_API_SetValue(ID_STA_Line7_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[7],0);
	GUI_ENC_API_SetValue(ID_STA_Line8_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[8],0);
	GUI_ENC_API_SetValue(ID_STA_Line9_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[9],0);

	//APP_Signal_API_TuneToNewParam(chn.m_FreqMHz,chn.m_SymKbps,chn.m_QAM);
	UI_APP_ProgState_Timer();
}
void UI_APP_ProgState_Enter(void)
{
	BU32 year,week,type,factory_num,sn;
	BU08 year_str[5]={0};
	BU32 softVer;
	BU32 hardVer;
	//BU32 ProdDate;
	BS32 ret;
	BU08 time[20];
	BU08 stbid[40];
	BU32 time_len;
	BU32 lang;
	BU32 cas_ver;
	BU08 card_sn[17];
	char **pDic = DicData;
	APP_Com_API_SetFullScreen();
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e , &lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
		pDic = DicDataEnglish;
	}
	else
	{
		pDic = DicData;
	}
	
	APP_EPG_API_StopGetEpg();
#if defined (CD_CA)
    ret = FYF_API_ca_get_value(FYF_CA_SV_CARD_INFO,&cas_ver,(BU32 *)card_sn);
	if(ret == FYF_OK)
	{
	   	sprintf(g_prog_state_Str[0],"%-12s %s",pDic[RS_CardNum],card_sn);  
	}
	else
	{
	    sprintf(g_prog_state_Str[0],"%-12s %s",pDic[RS_CardNum]," ");
	}
	
	sprintf(g_prog_state_Str[1],"%-12s %s",pDic[RS_CAManufactory],pDic[RS_CDCA]);
#elif defined (DVT_CA)
	FYF_CA_ManuInfo dvt_card;

	ret = FYF_API_ca_get_value(FYF_CA_SV_CARD_INFO,&dvt_card,0);
	if(ret == FYF_OK)
	{
		sprintf(g_prog_state_Str[1],"%-12s %s",pDic[RS_CardNum],dvt_card.m_byszSerialNO);  
	}
	else
	{
		sprintf(g_prog_state_Str[1],"%-12s %s",pDic[RS_CardNum]," ");
	}
	sprintf(g_prog_state_Str[0],"%-12s %s",pDic[RS_CAManufactory],pDic[RS_DVTCA]);
#endif
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_NUM, &factory_num);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_STBTYPE, &type);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_YEAR, &year);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_WEEK, &week);
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &sn);
    GUI_API_AddDecStr(year, 4, year_str);
	year = GUI_API_AddStrDec(year_str+2);
	//sn = 1;
	
	//sprintf(g_prog_state_Str[2],"%-12s %02d%02d%02d%02d%07d",pDic[RS_StbSequence],factory_num,type,year,week,sn); 
	sprintf(g_prog_state_Str[2],"%-12s %010d",pDic[RS_StbSequence], sn);   

	softVer = APP_SYS_SOFT_VERSION;
	hardVer = APP_SYS_HARD_VERSION;

	GUI_API_Memset (time, 0, 20);
	time_len = FYF_API_maincode_get_time (time,20);
	time[19] = 0;
	APP_SYS_API_GetSysInfo(APP_SYS_StbID_e, stbid);
	sprintf(g_prog_state_Str[3],"%-12s %x",pDic[RS_SWVer],softVer);
	sprintf(g_prog_state_Str[4],"%-12s %x",pDic[RS_HWVer],hardVer);
	sprintf(g_prog_state_Str[5],"%-12s %s",pDic[RS_ReleaseTime],time);
	sprintf(g_prog_state_Str[13],"%-12s %s",pDic[RS_MAC],stbid);
	
	GUI_ENC_API_SetValue(ID_STA_Line0_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[0],0);
	GUI_ENC_API_SetValue(ID_STA_Line1_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[1],0);
	GUI_ENC_API_SetValue(ID_STA_Line2_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[4],0);
	GUI_ENC_API_SetValue(ID_STA_Line3_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[3],0);
	GUI_ENC_API_SetValue(ID_STA_Line4_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[2],0);
	GUI_ENC_API_SetValue(ID_STA_Line5_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[5],0);
	GUI_ENC_API_SetValue(ID_STA_MAC_ProgState, WDT_STA_SV_STRING, (BU32)g_prog_state_Str[13], 0);
	if(APP_ChnDatabase_API_GetCurTypeChnNum() != 0)
	{
		ProgState_IPI_ShowChnInfo();
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_Line6_ProgState,WDT_STA_SV_STRING,(BU32)"",0);
		GUI_ENC_API_SetValue(ID_STA_Line7_ProgState,WDT_STA_SV_STRING,(BU32)"",0);
		GUI_ENC_API_SetValue(ID_STA_Line8_ProgState,WDT_STA_SV_STRING,(BU32)"",0);
		GUI_ENC_API_SetValue(ID_STA_Line9_ProgState,WDT_STA_SV_STRING,(BU32)"",0);
		GUI_ENC_API_SetValue(ID_STA_Line10_ProgState,WDT_STA_SV_STRING,(BU32)"",0);
		GUI_ENC_API_SetValue(ID_STA_Line11_ProgState,WDT_STA_SV_STRING,(BU32)"",0);
		GUI_ENC_API_SetValue(ID_STA_Line12_ProgState,WDT_STA_SV_STRING,(BU32)"",0);
		GUI_ENC_API_SetValue(ID_PBR_SigVoltage_ProgState,WDT_PBR_SV_CHANGE,0,0);
		GUI_ENC_API_SetValue(ID_PBR_SigSNR_ProgState,WDT_PBR_SV_CHANGE,0,0);
		GUI_ENC_API_SetValue(ID_PBR_BER_ProgState,WDT_PBR_SV_CHANGE,0,0);
	}

}
void UI_APP_ProgState_End(void)
{
	
}
void UI_APP_ProgState_Key(BU32 key)
{
	BU16 curChn;
	APP_EPG_API_StopGetEpg();
	switch(key)
	{
	case GUI_VK_RECALL:
	case GUI_VK_MENU:
		//GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		GUI_ENC_API_SwitchState(GUI_ENC_API_GetPrevState(), 1);
		if(APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		}
		break;
	case GUI_VK_EXIT:
	case GUI_VK_INFO:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		if(APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
		}
		break;
	case GUI_VK_UP:
		if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)return;
		curChn = APP_ChnDatabase_API_GetCurChnNO();
		curChn ++;
		if(curChn >= APP_ChnDatabase_API_GetCurTypeChnNum())
		{
			curChn = 0;
		}
		APP_ChnDatabase_API_SetCurChnNO(curChn);
		ProgState_IPI_ShowChnInfo();
		UI_APP_API_PlayChn(curChn, curChn, 0);
		//UI_APP_API_CheckLock_PlayChn_led(curChn,curChn,1);
		break;
	case GUI_VK_DOWN:
		if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)return;
		curChn = APP_ChnDatabase_API_GetCurChnNO();
		if(curChn == 0)curChn =APP_ChnDatabase_API_GetCurTypeChnNum()-1;
   		else curChn --;
		APP_ChnDatabase_API_SetCurChnNO(curChn);
		ProgState_IPI_ShowChnInfo();
		UI_APP_API_PlayChn(curChn, curChn, 0);
		//UI_APP_API_CheckLock_PlayChn_led(curChn,curChn,1);
		break;
	default:
		break;
	}
}
void UI_APP_ProgState_Timer(void)
{
	static BU32 s_lever=0,s_snr=1,s_ber=0;
	BU32 lever;
	BU32 snr;
	BU32 ber,berTempA,berTempB,berTempC;
	
	if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)return;

	if(APP_SIGNAL_LOCK == APP_Signal_API_GetSignalState())		
	{
		GUI_ENC_API_SetValue(ID_PBR_SigVoltage_ProgState,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_SigSNR_ProgState,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_BER_ProgState,WDT_PBR_SV_COLOR,(BU32)(LOCK_COLOR),0);
		lever = (BU32)APP_Signal_API_GetSignalLever();
		
		/*adjust lever value*/
	/*	if(lever>=40)
			lever = lever + 4;
		else	
			lever = lever + 2;
			*/
		snr = (BU32)APP_Signal_API_GetSignalSNR();	
		ber = APP_Signal_API_GetSignalBER();
	}
	else
	{
		GUI_ENC_API_SetValue(ID_PBR_SigVoltage_ProgState,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_SigSNR_ProgState,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		GUI_ENC_API_SetValue(ID_PBR_BER_ProgState,WDT_PBR_SV_COLOR,(BU32)(UNLOCK_COLOR),0);
		lever = 24;
		snr = 0;
		ber = 0x09630008;
	}
	
	if(s_lever != lever)
	{
		s_lever = lever;
		sprintf(g_prog_state_Str[10],"%ddbuV",lever);
		GUI_ENC_API_SetValue(ID_STA_Line10_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[10],0);
		lever = lever*200/103;
		GUI_ENC_API_SetValue(ID_PBR_SigVoltage_ProgState,WDT_PBR_SV_CHANGE,(BU32)lever,0);

	}

	if(s_snr != snr)
	{
		s_snr = snr;
		sprintf(g_prog_state_Str[11],"%ddB",snr);
		GUI_ENC_API_SetValue(ID_STA_Line11_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[11],0);
		snr = snr*200/40;
		GUI_ENC_API_SetValue(ID_PBR_SigSNR_ProgState,WDT_PBR_SV_CHANGE,(BU32)snr,0);
	}
	
	if(s_ber != ber)
	{
		s_ber = ber;
		berTempA = ber&0xff000000;
		berTempA = berTempA>>24;
		berTempB = ber&0x00ff0000;
		berTempB = berTempB>>16;
		berTempC = ber&0x000000ff;
		sprintf(g_prog_state_Str[12],"%d.%02dE-%d",berTempA,berTempB,berTempC + 1);
		if(berTempC>20)berTempC = 20;
		GUI_ENC_API_SetValue(ID_STA_Line12_ProgState,WDT_STA_SV_STRING,(BU32)g_prog_state_Str[12],0);
		GUI_ENC_API_SetValue(ID_PBR_BER_ProgState,WDT_PBR_SV_CHANGE,(20-berTempC)*10,0);

	}
}









void UI_APP_EPGDetail_Enter(void)
{
	
}
void UI_APP_EPGDetail_End(void)
{
	
}
void UI_APP_EPGDetail_Key(BU32 key)
{
	switch(key)
	{
	case GUI_VK_MENU:
	case GUI_VK_RECALL:
	case GUI_VK_EXIT:
	case GUI_VK_INFO:
		GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
		break;
			
	}
}

