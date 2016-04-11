#include "../osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"
#ifdef USE_FZVOD
#include "../../../ow_vod/ow_debug.h"
#endif
#define SANMING_VERSION
//#define BLACK_COLOR 0
#define NOTE_TIMER_PERIOD  1500

static BU08 g_epgdetail_chnNameStr[APP_EPG_NAME_LEN];
static BU08 g_epgdetail_eventNameStr[APP_EPG_NAME_LEN];
static BU08 g_epgdetail_detailNameStr[256];

static BU32 g_fullscreen_bu32areaLock = 0;
static BU32 g_fullscreen_bu32OpenDebugMsg = 0;
static BU32 g_fullscreen_bu32CloseDebugMsg = 0;
static BU32 g_fullscreen_timeout = 0;
static BU08 g_fullscreen_chnName[40];
static BU08 g_fullscreen_timeStr[6];
static BU32 g_fullscreen_save = 0;
static BU32 g_fullscreen_numKeyTime = 0;
static BU08 g_fullscreen_strTemp[40];
static BU08 g_sta_IsVisible = 0;
BU08 bfirst = 1;
extern BU08 g_caosd_show_osd_flag;
extern BU08 g_caosd_show_osd_top_flag;
#if defined (USE_FZVOD) || defined(USE_ENREACH)
BU08 bIsSelect = FYF_FALSE;
extern BU08 g_firststateInit;
BU08 bIsLeft = FYF_FALSE;
#endif
extern BU08 bTVOD; 
BU32 g_GetIpCount = 0;


void FullScreen_IPI_ShowReLogo()
{

#ifdef USE_ENREACH
	if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
	{
		if (evtc_check_channel())
		{
			GUI_ENC_API_SetVisible(ID_STA_Relogo, WS_VISIBLE);	
			GUI_ENC_API_Update(ID_STA_Relogo, 1);			
		}
		else
		{
			GUI_ENC_API_SetVisible(ID_STA_Relogo, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_Relogo, 1);
		}
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_Relogo, WS_DISABLE);	
		GUI_ENC_API_Update(ID_STA_Relogo, 1);
	}
#endif

#ifdef USE_FZVOD

	if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
	{
		ret = Tp_Ow_CheckTvodChannel();
		if (FYF_OK == ret)
		{
			GUI_ENC_API_SetVisible(ID_STA_Relogo, WS_VISIBLE);	
			GUI_ENC_API_Update(ID_STA_Relogo, 1);			
		}
		else
		{
			GUI_ENC_API_SetVisible(ID_STA_Relogo, WS_DISABLE);	
			GUI_ENC_API_Update(ID_STA_Relogo, 1);
		}
	}
#endif
	
}
void FullScreen_IPI_ClearScreen(void)
{
	BS32 ret = -1;
	GUI_ENC_API_SetVisible(ID_STABK_ChnInfo, WS_VISIBLE); 	
	GUI_ENC_API_Update(ID_STABK_ChnInfo, 1);
	GUI_ENC_API_SetVisible(ID_STA_FullTrans2_ChInfo, WS_VISIBLE); 	
	GUI_ENC_API_Update(ID_STA_FullTrans2_ChInfo, 1);
	
//	FullScreen_IPI_ShowReLogo();

}

void FullScreen_IPI_ShowConfig(BU08 InfobShow, BU08 NumbShow, BU08 VoicebShow, BU08 NoProgbShow, BU08 TrackSetbShow, BU08 IppbShow)
{
	BU32 showState = 0;
	BU32 i = 0;
#ifndef CD_CA
	IppbShow = 0;
#endif
	GUI_WinID_Demo_e Infob[10] = {ID_STA_InfoBK_ChnInfo,ID_STA_Time_ChnInfo,ID_STA_Name_ChnInfo,
									ID_STAIcon1_ChnInfo,ID_STAIcon2_ChnInfo,ID_STAIcon3_ChnInfo,
									ID_STA_Info1_ChnInfo,ID_STA_Info2_ChnInfo,ID_STA_Track_ChnInfo,
									ID_PBR_Epg_ChnInfo};
#if 1
	GUI_WinID_Demo_e TrackSetb[11] = {ID_STADLgBK_ChnInfo,ID_STABar1_ChnInfo,ID_STABar2_ChnInfo,
											ID_STABar3_ChnInfo,ID_STAText2_ChnInfo,ID_STAText4_ChnInfo,
											ID_STAText3_ChnInfo,ID_STAText1_ChnInfo,ID_ODC_ChName_ChnInfo,
											ID_CBX_Track_ChnInfo,ID_CBX_Offset_ChnInfo};
#endif 
	GUI_WinID_Demo_e Ippb[13] = {ID_STA_ipp_bk_ChnInfo,ID_LBX_ipp_price_ChnInfo,ID_STA_ipp_tvsid_ChnInfo,
									ID_STA_ipp_slotid_ChnInfo,ID_STA_ipp_productid_ChnInfo,ID_STA_ipp_ExpiredDate_ChnInfo,
									ID_STA_ipp_price_ChnInfo,ID_STA_ipp_pin_ChnInfo,ID_TBX_ipp_pin_ChnInfo,
									ID_STA_ipp_prompt_ChnInfo,ID_STA_ipp_prompt2_ChnInfo,
									ID_STA_ipp_title_ChnInfo,ID_STA_ipp_pin_prompt_ChnInfo};

	if(IppbShow)
		{showState = WS_VISIBLE;}
	else
		{showState = WS_DISABLE;}
	for(i = 0; i < 13; i++)
	{
		GUI_ENC_API_SetVisible(Ippb[i], showState);
		if(IppbShow)
			{GUI_ENC_API_Update(Ippb[i] ,1);}
	}
	
	if(InfobShow)
	{

		showState = WS_VISIBLE;
	}
	else
	{
	
		showState = WS_DISABLE;
	}
	GUI_ENC_API_SetVisible(ID_STA_NameBk_ChnInfo, showState);
	for(i = 0; i < 10; i++)
	{
		GUI_ENC_API_SetVisible(Infob[i], showState);
		if(InfobShow)
		{
			GUI_ENC_API_Update(Infob[i], 1);
		}
	}
	if (InfobShow)
	{
		GUI_ENC_API_Update(ID_ODC_ADV_ChnInfo, 3);
	}
	else
	{
		GUI_ENC_API_Update(ID_ODC_ADV_ChnInfo, 2);
	}
	if(NumbShow)
	{
		GUI_ENC_API_SetVisible(ID_CHN_ChnInfo, WS_VISIBLE);
		GUI_ENC_API_Update(ID_CHN_ChnInfo ,1);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_CHN_ChnInfo, WS_DISABLE);
	}
	
	if(VoicebShow)
	{
		GUI_ENC_API_SetVisible(ID_VOI_ChnInfo, WS_VISIBLE);
		GUI_ENC_API_Update(ID_VOI_ChnInfo, 1);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_VOI_ChnInfo, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STAVOI_ChnInfo, WS_DISABLE);
	}
	
    if(TrackSetbShow)
	{
		showState = WS_VISIBLE;
	}
	else
	{
		showState = WS_DISABLE;
	}

	for(i = 0; i < 11; i++)
	{
		GUI_ENC_API_SetVisible(TrackSetb[i], showState);
		if(TrackSetbShow)
		{
			GUI_ENC_API_Update(TrackSetb[i], 1);
		}
	}
	
 	if(NoProgbShow)
 	{
 		GUI_ENC_API_SetVisible(ID_STA_TextBk_ChnInfo,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Text_ChnInfo,WS_VISIBLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Title_ChnInfo,WS_VISIBLE);	
 		GUI_ENC_API_Update(ID_STA_TextBk_ChnInfo ,1);
 		GUI_ENC_API_Update(ID_STA_Text_ChnInfo ,1);
 		GUI_ENC_API_Update(ID_STA_Title_ChnInfo ,1);
 	}
 	else
 	{
 		GUI_ENC_API_SetVisible(ID_STA_TextBk_ChnInfo,WS_DISABLE); 
 		GUI_ENC_API_SetVisible(ID_STA_Text_ChnInfo,WS_DISABLE);	
 		GUI_ENC_API_SetVisible(ID_STA_Title_ChnInfo,WS_DISABLE);	
 	}
	GUI_ENC_API_Update(ID_STA_Mail_ChInfo ,1);
	GUI_ENC_API_Update(ID_STAMute_ChnInfo ,1);
	GUI_ENC_API_Update(ID_ODC_CaOsd_ChInfo, 8);

	FullScreen_IPI_ShowReLogo();
#ifdef CD_CA
	if(FYF_TRUE == UI_APP_API_IsProgressStripShow() && !TrackSetbShow&& !IppbShow)
	{
		GUI_ENC_API_SetVisible(ID_STA_Text1_ChInfo,WS_VISIBLE);	
		GUI_ENC_API_SetVisible(ID_STA_Text2_ChInfo,WS_VISIBLE);	
		GUI_ENC_API_SetVisible(ID_PBR_ChInfo,WS_VISIBLE);
		GUI_ENC_API_Update(ID_STA_Text1_ChInfo ,1);
		GUI_ENC_API_Update(ID_STA_Text2_ChInfo ,1);
		GUI_ENC_API_SetValueUnPaint(ID_PBR_ChInfo, WDT_PBR_SV_CHANGE, (320*UI_APP_API_GetProgress())/100, 0);
		GUI_ENC_API_Update(ID_PBR_ChInfo ,1);		
	}
#endif
}

void FullScreen_IPI_SetCurState(BU32 curState)
{
	UI_APP_API_SetChnInfoCurState(curState);
	switch(curState)
	{
	case NULL_STATE:
		if(UI_APP_API_bShowPrompt() || UI_APP_API_bShowNoProg())
		{
			FullScreen_IPI_ClearScreen();
			FullScreen_IPI_ShowConfig(0, 0, 0, 1, 0, 0);
		}
		else
		{
			FullScreen_IPI_ClearScreen();
			FullScreen_IPI_ShowConfig(0, 0, 0, 0, 0, 0);
		}
		break;
	case INFOBAR_STATE:
		if(g_fullscreen_timeout)
		{
			if(UI_APP_API_bShowPrompt())
			{
				FullScreen_IPI_ShowConfig(1, 1, 0, 1, 0, 0);
			}
			else
			{
				FullScreen_IPI_ShowConfig(1, 1, 0, 0, 0, 0);
			}
		}
		else
		{
			if(UI_APP_API_bShowPrompt())
			{
				FullScreen_IPI_ShowConfig(0, 1, 0, 1, 0, 0);
			}
			else
			{
				FullScreen_IPI_ShowConfig(0, 1, 0, 0, 0, 0);
			}
		}
		break;
	case VOICE_STATE:
		
		if(UI_APP_API_bShowPrompt())
		{
			FullScreen_IPI_ShowConfig(0, 0, 1, 1, 0, 0);
		}
		else
		{
			FullScreen_IPI_ShowConfig(0, 0, 1, 0, 0, 0);
		}
		
		break;
	case TRACKSET_STATE:
		#if 0
		if(UI_APP_API_bShowPrompt())
		{
			FullScreen_IPI_ShowConfig(1, 1, 0, 1, 1, 0);
		}
		else
		{
			FullScreen_IPI_ShowConfig(1, 1, 0, 0, 1, 0);
		}
		#endif
		FullScreen_IPI_ShowConfig(1, 1, 0, 0, 1, 0);
		break;
	case NOPROG_STATE:
	case NO_SERVICE:
		FullScreen_IPI_ShowConfig(0, 0, 0, 1, 0, 0);
		break;
	case CHNUM_STATE:
		FullScreen_IPI_ShowConfig(0, 1, 0, 0, 0, 0);
		break;
	case IPP_STATE:
		if(UI_APP_API_bShowPrompt())
		{
			FullScreen_IPI_ClearScreen();
			FullScreen_IPI_ShowConfig(1, 0, 0, 0, 0, 1);
		}
		else
		{
			FullScreen_IPI_ClearScreen();
			FullScreen_IPI_ShowConfig(1, 0, 0, 0, 0, 1);
		}
		break;
	default:
		break;
	}
}

void FullScreen_IPI_SetChnInfo(void)
{
	BU32 RS_ID;
	BU16 curChn;
	ChnDataBase_ChnInfo_S chnInfo;
	APP_Date_S now;
	int ret = -1;
	
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo))
	{
		return;
	}
	GUI_ENC_API_Update(ID_STA_NameBk_ChnInfo,1);
#if defined(USE_FZVOD) || defined(USE_ENREACH)
	
	if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
	{
	#if defined (USE_FZVOD)
		ret = Tp_Ow_CheckTvodChannel();
		if (FYF_OK == ret)
	#elif defined (USE_ENREACH)
		if (evtc_check_channel())
	#endif
		{
			sprintf(g_fullscreen_chnName,"%03d %s(回看)",curChn + APP_ChnDatabase_API_GetChn0Flag(), chnInfo.m_ChnName);
 		}
		else
		{
			sprintf(g_fullscreen_chnName,"%03d %s",curChn + APP_ChnDatabase_API_GetChn0Flag(), chnInfo.m_ChnName);
		}
	}
	else
	{
		sprintf(g_fullscreen_chnName,"%03d %s",curChn + APP_ChnDatabase_API_GetChn0Flag(), chnInfo.m_ChnName);
	}
	
	
#else
	sprintf(g_fullscreen_chnName,"%03d %s",curChn + APP_ChnDatabase_API_GetChn0Flag(), chnInfo.m_ChnName);
#endif			
	GUI_ENC_API_SetValue(ID_STA_Name_ChnInfo, WDT_STA_SV_STRING, (BU32)g_fullscreen_chnName, 0);

	if(chnInfo.m_Ca)
	{
		GUI_ENC_API_SetValue(ID_STAIcon1_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_Dollar_W24_H24, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STAIcon1_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
	}
	if(chnInfo.m_Lock)
	{
		GUI_ENC_API_SetValue(ID_STAIcon2_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_Lock_W24_H24, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STAIcon2_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
	}
	if(chnInfo.m_Fav)
	{
		GUI_ENC_API_SetValue(ID_STAIcon3_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_Fav_W24_H24, 0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STAIcon3_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
	}
	switch(chnInfo.m_AudioChn)
	{
	case APP_CHN_TRACK_LEFT_e:
		RS_ID = RS_Left;
		break;
	case APP_CHN_TRACK_RIGHT_e:
		RS_ID = RS_Right;
		break;
	case APP_CHN_TRACK_SINGLE_e:
		RS_ID = RS_Single;
		break;
	case APP_CHN_TRACK_STEREO_e:
		RS_ID = RS_Stereo;
		break;
	default:
		RS_ID = RS_Left;
		break;
	}
	GUI_ENC_API_SetValue(ID_STA_Track_ChnInfo, WDT_STA_SV_DATAID, RS_ID, 0);
	
	GUI_ENC_API_SetValue(ID_CHN_ChnInfo, WDT_CHN_SV_NUM, (BU32)(curChn+APP_ChnDatabase_API_GetChn0Flag()), 0);
	APP_SYS_API_GetLocalDateAndTime(&now);
	sprintf(g_fullscreen_timeStr, "%02d:%02d", now.m_hour, now.m_min);
	GUI_ENC_API_SetValue(ID_STA_Time_ChnInfo, WDT_STA_SV_STRING, (BU32)g_fullscreen_timeStr, 0);
	UI_APP_API_ShowCurAndNextEPGWithEndTime(ID_STA_Info1_ChnInfo, ID_STA_Info2_ChnInfo,ID_PBR_Epg_ChnInfo,380,1);
//	GUI_ENC_API_Update(ID_ODC_ADV_ChnInfo, 3);
	FullScreen_IPI_ShowReLogo();
}

void FullScreen_IPI_JudgeProgExist(void)
{
	BU32 number=0;
	BU08 bExist = 0;
	BU32 lang;
	
	GUI_ENC_API_GetValue(ID_CHN_ChnInfo, WDT_CHN_SV_NUM, (BU32 *)&number, 0);
	if(APP_ChnDatabase_API_GetChn0Flag() == 0)
	{
		if(number <= APP_ChnDatabase_API_GetCurTypeChnNum()-1)
		{	
			bExist = 1;
		}
		else
		{
			bExist = 0;
		}
	}
	else
	{
		if(number >= 1 && number <= APP_ChnDatabase_API_GetCurTypeChnNum())
		{	
			bExist = 1;
		}
		else
		{
			bExist = 0;
		}
	}
	
	if(bExist)
	{
		APP_ChnDatabase_API_SetCurChnNO(number- APP_ChnDatabase_API_GetChn0Flag());
		FullScreen_IPI_SetChnInfo();
		FullScreen_IPI_SetCurState(INFOBAR_STATE);
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);//播放节目
		APP_ChnDatabase_API_SetLastChnInfo(APP_ChnDatabase_API_GetCurChnNO());
	}
	else
	{
		APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &lang);
 		if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
 		{
			sprintf(g_fullscreen_strTemp, "%s%03d%s", DicDataEnglish[RS_Program],number, DicDataEnglish[RS_NotExist]);
 		}
 		else
 		{
 			sprintf(g_fullscreen_strTemp, "%s%03d%s", DicData[RS_Program],number, DicData[RS_NotExist]);
 		}
 		GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_STRING, (BU32)g_fullscreen_strTemp, 0);
		FullScreen_IPI_SetCurState(NOPROG_STATE);
	}
	
}

void FullScreen_IPI_ChangeProg(void)
{
	BU32 keyState;
	BS32 ret = -1;
	FullScreen_IPI_SetChnInfo();

	keyState = GUI_ENC_API_GetKeyState();
	if(UI_APP_API_GetChnInfoCurState()!= INFOBAR_STATE)
	{
		if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
		{
			FullScreen_IPI_ClearScreen();
		}
		FullScreen_IPI_SetCurState(INFOBAR_STATE);
	}
	else
	{
		if (!keyState)
		{
			GUI_ENC_API_Update(ID_ODC_ADV_ChnInfo, 4);
		}
	}
	
	if(!keyState)//连续键处理
	{
		UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);//播放节目
	}
	else
	{
		UI_APP_API_SetLockServiceId(0);
	}
	
	APP_ChnDatabase_API_SetLastChnInfo(APP_ChnDatabase_API_GetCurChnNO());
}

void FullScreen_IPI_TrackSetChangeProg(void)
{
	BU16 curChn;
	ChnDataBase_ChnInfo_S chn;
	
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	APP_ChnDatabase_API_SetLastChnInfo(curChn);
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn,&chn))
	{
		return;
	}
	GUI_ENC_API_Update(ID_ODC_ChName_ChnInfo,1);
	if(chn.m_AudioChn>3)
	{
		chn.m_AudioChn = 3;
	}
	
	GUI_ENC_API_SetValue(ID_CBX_Track_ChnInfo,WDT_CBX_SV_INDEX,(BU32)chn.m_AudioChn,0);
	if(chn.m_AudioOffset+15 > 30)
	{
		chn.m_AudioOffset = 15;
	}
	else if(chn.m_AudioOffset+15 < 0)
	{
		chn.m_AudioOffset = -15;
	}
	GUI_ENC_API_SetValue(ID_CBX_Offset_ChnInfo,WDT_CBX_SV_INDEX,(BU32)(chn.m_AudioOffset+15),0);
	UI_APP_API_PlayChn(curChn, curChn, 1);
	//UI_APP_API_CheckLock_PlayChn_led(curChn,curChn,1);
	FullScreen_IPI_SetChnInfo();
}

void FullScreen_IPI_PlayLastChn(void)
{
	APP_CHN_AV_TYPE_E avType;
	BU32 chIndex;
	APP_CHN_AV_TYPE_E to_avType;
	
	if(APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
	{
		to_avType = APP_CHN_AV_TYPE_RD_e;
	}
	else
	{
		to_avType = APP_CHN_AV_TYPE_TV_e;
	}
	
	APP_ChnDatabase_API_GetLastChnInfo(&avType, &chIndex);
	if(APP_ChnDatabase_API_GetCurChnType() != avType)
	{
		if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(avType, APP_CHN_GROUP_TYPE_ALL_e) &&
			APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
			return;
#if 0
		if(to_avType == APP_CHN_AV_TYPE_TV_e)
		{
			if(!UI_APP_API_bShowNoSignal())
			{
				APP_Com_API_ShowBmp(0);//黑屏
			}
			GUI_ENC_API_SetValueUnPaint(ID_STA_InfoBK_ChnInfo, WDT_STA_SV_STYLE, STYLE_ProgInfoBar_W534_H128, 0);
		}
		else
		{
			if(!UI_APP_API_bShowNoSignal())
			{
				APP_Com_API_ShowBmp(1);//广播LOGO
			}
		}		
#endif
		GUI_ENC_API_SetValueUnPaint(ID_STA_InfoBK_ChnInfo, WDT_STA_SV_STYLE, STYLE_ProgInfoBar_W618_H128, 0);
		APP_ChnDatabase_API_JudgeChn0();
	}
	if(chIndex>=APP_ChnDatabase_API_GetCurTypeChnNum())
		return;
	
	APP_ChnDatabase_API_SetCurChnNO(chIndex);
	FullScreen_IPI_ChangeProg();
}

void UI_APP_FullScreen_Enter(void)
{
	BU32 i,index;
	BS32 ret = -1;
	BU32 allMail=0,newMail=0;
	#if defined (CD_CA)
	FYF_CA_EmailHead_s mail_head[100];
	FYF_CA_EmailIdx_s  mail_index;
	#endif
	BU32 cur_time;
	APP_EPG_API_StopGetEpg();



	GUI_ENC_API_SetVisible(ID_STA_Relogo, WS_DISABLE);	


	GUI_ENC_API_SetValue(ID_STA_FULLSCREEN_CHECKNET,WDT_STA_SV_ALIGN,WDT_STA_ALIGN_CENTER,0);

	GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET, WS_DISABLE); 
	GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET_BAK, WS_DISABLE); 

#if defined (DVT_CA)
	if(FYF_TRUE == DVTSTBCA_GetUrgentBrctFlag())
	{
		GUI_ENC_API_SetVisible(ID_STA_UrgentBrct_ChnInfo, WS_VISIBLE);
		GUI_ENC_API_SetValue(ID_STA_UrgentBrct_ChnInfo,WDT_STA_SV_BGC, COLOR_INFOBK, 0);
	}
	else
	{
		GUI_ENC_API_SetVisible(ID_STA_UrgentBrct_ChnInfo, WS_DISABLE);
		GUI_ENC_API_SetValue(ID_STA_UrgentBrct_ChnInfo,WDT_STA_SV_BGC, COLOR_TRANS, 0);
	}
#else
		GUI_ENC_API_SetVisible(ID_STA_UrgentBrct_ChnInfo, WS_DISABLE);
		GUI_ENC_API_SetValue(ID_STA_UrgentBrct_ChnInfo,WDT_STA_SV_BGC, COLOR_TRANS, 0);
#endif

#if defined (NO_CA)
	GUI_ENC_API_SetVisible(ID_STA_Mail_ChInfo,WS_DISABLE);
#elif defined (CD_CA)
	mail_index.m_Count = 100;
	mail_index.m_byFromIndex = 0;
	FYF_API_ca_get_value(FYF_CA_SV_EMAIL_COUNT,mail_head,&mail_index);
	allMail = mail_index.m_Count;
	for(i=0;i<allMail;i++)
	{
		if(mail_head[i].m_bNewEmail)
			newMail++;
	}
	if(newMail)
	{
		GUI_ENC_API_SetValue(ID_STA_Mail_ChInfo,WDT_STA_SV_STYLE,STYLE_Email_W51_H34,0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_Mail_ChInfo,WDT_STA_SV_STYLE,STYLE_NULL,0);
	}
#elif defined (DVT_CA)
	FYF_API_ca_get_value(FYF_CA_SV_EMAIL_COUNT,&allMail,&newMail);
	if(newMail)
	{
		GUI_ENC_API_SetValue(ID_STA_Mail_ChInfo,WDT_STA_SV_STYLE,STYLE_Email_W51_H34,0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_STA_Mail_ChInfo,WDT_STA_SV_STYLE,STYLE_NULL,0);
	}
#endif
	if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
	{
		GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
		APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
	}
	else
	{
		GUI_ENC_API_Update(ID_STAMuteBk_ChnInfo,1);
		GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
		APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
		UI_APP_API_SetVolume();
	}
 	APP_SYS_API_GetSysInfo(APP_SYS_TIMEOUT_e, &g_fullscreen_timeout);
	if(g_fullscreen_timeout != 0)
	{
		GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000,0);
	}
	else
	{
		GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, NOTE_TIMER_PERIOD,0);
	}
	APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
	APP_ChnDatabase_API_JudgeChn0();
	if(APP_ChnDatabase_API_GetCurTypeChnNum() != 0)
	{
		APP_ChnDatabase_API_SetLastChnInfo(APP_ChnDatabase_API_GetCurChnNO());
		
		APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e,(BU32 *)&index);
		GUI_ENC_API_SetValue(ID_VOI_ChnInfo, WDT_VOI_SV_RIGHT, index, 0);
		
		if(UI_APP_API_GetChnInfoCurState() == TRACKSET_STATE /*&& GUI_ENC_API_GetPrevState()!=ID_DLG_PlayReserve */)
		{
			FullScreen_IPI_SetCurState(TRACKSET_STATE);
			GUI_ENC_API_SetFocus(ID_ODC_ChName_ChnInfo);
		}
#ifdef CD_CA
		else if(UI_APP_API_GetChnInfoCurState() == IPP_STATE /*&& GUI_ENC_API_GetPrevState()!=ID_DLG_PlayReserve*/)
		{
			FullScreen_IPI_SetChnInfo();
			FullScreen_IPI_SetCurState_Ipp ();
		}
#endif
		else
		{
			FullScreen_IPI_SetChnInfo();
			FullScreen_IPI_SetCurState(INFOBAR_STATE);	
		}
#ifdef CD_CA
		if(FYF_TRUE == CDSTBCA_LockService_GetNoPsiSiFlag())
		{
			CDSTBCA_LockService_NoPsiSi();
			CDSTBCA_LockService_SetNoPsiSiFlag(FYF_FALSE);
		}
		else
#endif
		{
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
		   // UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);//播放节目
		}
		
	}
	else
	{
 		GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, (BU32)RS_NoProg, 0);
 		FullScreen_IPI_SetCurState(NOPROG_STATE);
		if(ID_DLG_Nvod == UI_APP_API_GetReserveListPreState())
		{
			APP_Com_IPI_StopProg(1);
			GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);
		}
	}

#ifdef CD_CA
	if(FYF_FALSE == UI_APP_API_IsProgressStripShow())
	{
		GUI_ENC_API_SetVisible(ID_STA_Text1_ChInfo, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Text2_ChInfo, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_PBR_ChInfo, WS_DISABLE);		
	}
#else
	{
		GUI_ENC_API_SetVisible(ID_STA_Text1_ChInfo, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_STA_Text2_ChInfo, WS_DISABLE);
		GUI_ENC_API_SetVisible(ID_PBR_ChInfo, WS_DISABLE);	
	}

#endif
	APP_Com_API_SetFullScreen();
#ifdef FZTEST

#ifdef USE_FZVOD
		if (SYSTEM_GetNET_ON_OFF())
		{
			if (g_firststateInit == FYF_TRUE)
			{
				g_firststateInit = FYF_FALSE;
				ret = TP_Ipanel_CheckNetOk();
				if (-3 == ret)
				{
					FullScreen_IPI_SetCurState(NET_STATE);
					printf("\033[0;31m set NET_STATE \033[0m\n");
				}
			}

		}
#endif
#endif

}


void UI_APP_FullScreen_End(void)
{
	BU32 g_sys_volumesave = 0;
#ifndef NO_CA
	g_caosd_show_osd_flag = 1;
	g_caosd_show_osd_top_flag = 1;	
#endif
#ifdef FZTEST
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
	GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PLAY,0,0);
	GUI_ENC_API_SetValue(ID_TIM_Event_ChnInfo, WDT_TIM_SV_PLAY,0,0);
#endif
	APP_SYS_API_GetSysInfo(APP_SYS_VolumeSave_e, &g_sys_volumesave);
	if (g_sys_volumesave == 0 && g_fullscreen_save == 1)
	{
	    APP_ChnDatabase_API_UpDataToFlash(1);
		g_fullscreen_save = 0;
	}

}


void UI_APP_FullScreen_Key(BU32 key)
{
	UserData_S UserData;
	BS32 speed;
	BS32 ret = -1;
	BU08 str_tmp[3];
	BU32 k,j;
	BU32 cur_time;
	BU32 endhour, endmin;
	BU32 index;
	BU16 curChn, chnNum;
	static BU32 lastKeyState = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 broadcastNum = 0;//数据广播总数
	BroadCast_ChnInfo_S broadcastInfo;
	APP_CHN_AV_TYPE_E o_avType,to_avType;
	BU32 RS_ID;
	BU32 netEnable = 1;/*1表示关(单向),0表示开(双向)*/
	BU32 progSortCount;
	int tmpPmtFreq;
	int tmpPmtPid;
	int tmpRangStart;
	int tmpRangEnd;
	int tmpVTrackId;
	int tmpATrackId;
	Epg_Info_S CurEpgInfo, NextEpgInfo;
	ChnDataBase_ChnInfo_S 	ProgInfoListtemp ;
	BU32 detaillen;
	BU32 vol = 0;
	BS32 offset = 0;
	
	APP_EPG_API_StopGetEpg();
	
	if(bfirst == 0)
	{
		return;
	}

	chnNum = APP_ChnDatabase_API_GetCurTypeChnNum();
#ifdef DVT_CA
	if(key == GUI_VK_1 && g_fullscreen_bu32areaLock == GUI_VK_F7)
	{
		GUI_ENC_API_SwitchState(ID_DLG_SingleSlot, 1);
		return;
	}
	else
	{
		g_fullscreen_bu32areaLock = key;
	}

	if(key == GUI_VK_1 && g_fullscreen_bu32OpenDebugMsg == GUI_VK_F8)
	{
		FYF_API_ca_set_value(FYF_CA_SV_DebugMsgSign,1,0);
		return;
	}
	else
	{
		g_fullscreen_bu32OpenDebugMsg = key;
	}

	if(key == GUI_VK_1 && g_fullscreen_bu32CloseDebugMsg == GUI_VK_F9)
	{
		FYF_API_ca_set_value(FYF_CA_SV_DebugMsgSign,0,0);
		return;
	}
	else
	{
		g_fullscreen_bu32CloseDebugMsg = key;
	}
#endif	
#ifdef ENABLE_PVR
	if (APP_PVR_API_GetRecStatus(1) == 0)
#endif
	{
		switch(key)
		{
#ifdef SANMING_VERSION
		case GUI_VK_MAIL:
#else
		case GUI_VK_MAIL:
#endif
#ifndef NO_CA

		UI_APP_CA_SetMailPreState(ID_DLG_ChnInfo);
		GUI_ENC_API_SwitchState(ID_DLG_Mail,1);
#endif
	    break;
		case GUI_VK_UP:

			if(chnNum == 0)
				return;
			if(g_fullscreen_timeout != 0)
			{
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000, 0);
			}
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			
	   #if 0
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			curChn ++;
			if(curChn > chnNum-1)
				curChn=0;
	   #else
			curChn = APP_ChnDatabase_API_GetNextChnNO();
	   #endif 

			APP_ChnDatabase_API_SetCurChnNO(curChn);
#if defined(USE_FZVOD) || defined(USE_ENREACH)
		if(SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
		{
			if (evtc_check_channel())
			{
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWRELOGO, 0, 0);
			}
			else
			{
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_HIDERELOGO, 0, 0);
			}
		}
		else
		{
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_HIDERELOGO, 0, 0);
		}
#endif			
			FullScreen_IPI_ChangeProg();
			break;
			
		case GUI_VK_DOWN:
			if(chnNum == 0)
				return;
			if(g_fullscreen_timeout != 0)
			{
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000, 0);
			}
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			
		#if 0
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(curChn == 0)
				curChn=chnNum-1;
			else
				curChn --;
		#else
			curChn = APP_ChnDatabase_API_GetPrevChnNO();
		#endif

			APP_ChnDatabase_API_SetCurChnNO(curChn);
#if defined(USE_FZVOD) || defined(USE_ENREACH)
			if(SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
			{
				if (evtc_check_channel())
				{
					GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_SHOWRELOGO, 0, 0);
				}
				else
				{
					GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_HIDERELOGO, 0, 0);
				}
			}
			else
			{
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID, WM_HIDERELOGO, 0, 0);
			}
#endif				
			FullScreen_IPI_ChangeProg();
			break;
		case GUI_VK_P_DOWN:
			if(chnNum == 0)
				return;
			if(g_fullscreen_timeout != 0)
			{
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000, 0);
			}
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			curChn = curChn + 8;
			if(curChn >= chnNum)
			{
			    curChn = 0;
			}
			APP_ChnDatabase_API_SetCurChnNO(curChn);
			FullScreen_IPI_ChangeProg();
			break;
		case GUI_VK_P_UP:
			if(chnNum == 0)
				return;
			if(g_fullscreen_timeout != 0)
			{
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000, 0);
			}
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(curChn < 8)
			{
			    curChn = chnNum - 1;
			}
			else
			{
			    curChn = curChn - 8;
			}
			APP_ChnDatabase_API_SetCurChnNO(curChn);
			FullScreen_IPI_ChangeProg();
			break;
		case GUI_VK_LEFT:
#ifdef USE_FZVOD
			if(bTVOD && SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
			{
				FullScreen_IPI_SetCurState(NULL_STATE);
				ret = TP_Ipanel_CheckNetOk();//Tp_Ow_CheckTvodChannel();
				if(FYF_OK == ret)
				{
					FullScreen_IPI_ClearScreen();
					FULLTOTVOD_SetCurState(/*5*/FAST_STATE);	
					bIsSelect = FYF_FALSE;
					bIsLeft = FYF_TRUE;
					GUI_ENC_API_SwitchState(ID_DLG_FULLTOTVOD, 1);
					return;
				}
				else if (-2 == ret)
				{
					FullScreen_IPI_ClearScreen();
					
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET, WS_VISIBLE); 
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET_BAK, WS_VISIBLE); 
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET_BAK, 1);
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET, 1);
					//GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
					
					FYF_API_thread_sleep(2000);
				
					FullScreen_IPI_ClearScreen();
					FullScreen_IPI_SetCurState(NULL_STATE);
					//GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
					return;
				}
			}

            else
#endif 
            {
                if(chnNum == 0)
				   return;
					GUI_ENC_API_SetValue(ID_TIM_ChnInfo,WDT_TIM_SV_PERIOD,NOTE_TIMER_PERIOD,0);
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_Update(ID_STAMuteBk_ChnInfo, 1);
				GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}

		    if(UI_APP_API_GetChnInfoCurState() != VOICE_STATE)
			{
				if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
				{
					FullScreen_IPI_ClearScreen();
				}
				
				FullScreen_IPI_SetCurState(VOICE_STATE);
			}
			
			GUI_ENC_API_SetValueUnPaint(ID_VOI_ChnInfo, WDT_VOI_SV_KEY, key, 0);
			GUI_ENC_API_GetValue(ID_VOI_ChnInfo, WDT_VOI_SV_INDEX, &index, 0);

			if (index == 0)
			{
				GUI_ENC_API_SetVisible(ID_STAVOI_ChnInfo, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STAVOI_ChnInfo, 1);
			}
			else
			{
				GUI_ENC_API_Update(ID_VOI_ChnInfo, 1);
			}
			APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
			UI_APP_API_SetVolume();
			g_fullscreen_save = 1;
            }
			break;

		case GUI_VK_RIGHT:
#ifdef USE_FZVOD

			if(bTVOD && SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
			{
				FullScreen_IPI_SetCurState(NULL_STATE);
				GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_STRING, "请按左键进入回看节目", 0);
				GUI_ENC_API_SetVisible(ID_STA_TextBk_ChnInfo,WS_VISIBLE);	
				GUI_ENC_API_SetVisible(ID_STA_Text_ChnInfo,WS_VISIBLE); 
				GUI_ENC_API_SetVisible(ID_STA_Title_ChnInfo,WS_VISIBLE);	
				GUI_ENC_API_Update(ID_STA_TextBk_ChnInfo ,1);
				GUI_ENC_API_Update(ID_STA_Text_ChnInfo ,1);
				GUI_ENC_API_Update(ID_STA_Title_ChnInfo ,1);
				
				FYF_API_thread_sleep(1000);
				FullScreen_IPI_ClearScreen();

			}
    
            else
#endif  
            {
                 if(chnNum == 0)
				   return;
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo,WDT_TIM_SV_PERIOD,NOTE_TIMER_PERIOD,0);
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
				if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
				{
					GUI_ENC_API_Update(ID_STAMuteBk_ChnInfo, 1);
					GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
					APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
				}

			    if(UI_APP_API_GetChnInfoCurState() != VOICE_STATE)
				{
					if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
					{
						FullScreen_IPI_ClearScreen();
					}
					
					FullScreen_IPI_SetCurState(VOICE_STATE);
				}
				
				GUI_ENC_API_SetValueUnPaint(ID_VOI_ChnInfo, WDT_VOI_SV_KEY, key, 0);
				GUI_ENC_API_GetValue(ID_VOI_ChnInfo, WDT_VOI_SV_INDEX, &index, 0);

				if (index == 0)
				{
					GUI_ENC_API_SetVisible(ID_STAVOI_ChnInfo, WS_VISIBLE);
					GUI_ENC_API_Update(ID_STAVOI_ChnInfo, 1);
				}
				else
				{
					GUI_ENC_API_Update(ID_VOI_ChnInfo, 1);
				}
				APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
				UI_APP_API_SetVolume();
				g_fullscreen_save = 1;
		    }
			break;

			
			//RTSP_ParseUServer_DESCRIBE_Rspone(buftmp, &tmpPmtFreq,&tmpPmtPid,&tmpRangStart,&tmpRangEnd,&tmpVTrackId,&tmpATrackId);
			//printf(" tmpPmtFreq = %d, tmpPmtPid = %d, tmpRangStart = %d, tmpRangEnd = %d, tmpVTrackId = %d, tmpATrackId = %d\n",tmpPmtFreq,tmpPmtPid,tmpRangStart,tmpRangEnd,tmpVTrackId,tmpATrackId);
			//break;

		case GUI_VK_V_UP:
		case GUI_VK_V_DOWN:
			if(chnNum == 0)
				return;
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo,WDT_TIM_SV_PERIOD,NOTE_TIMER_PERIOD,0);
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_Update(ID_STAMuteBk_ChnInfo, 1);
				GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			}

		    if(UI_APP_API_GetChnInfoCurState() != VOICE_STATE)
			{
				if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
				{
					FullScreen_IPI_ClearScreen();
				}
				
				FullScreen_IPI_SetCurState(VOICE_STATE);
			}
			
			GUI_ENC_API_SetValueUnPaint(ID_VOI_ChnInfo, WDT_VOI_SV_KEY, key, 0);
			GUI_ENC_API_GetValue(ID_VOI_ChnInfo, WDT_VOI_SV_INDEX, &index, 0);

			if (index == 0)
			{
				GUI_ENC_API_SetVisible(ID_STAVOI_ChnInfo, WS_VISIBLE);
				GUI_ENC_API_Update(ID_STAVOI_ChnInfo, 1);
			}
			else
			{
				GUI_ENC_API_Update(ID_VOI_ChnInfo, 1);
			}
			APP_SYS_API_SetSysInfo(APP_SYS_VOLUME_e, index);
			UI_APP_API_SetVolume();
			g_fullscreen_save = 1;
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
			if(chnNum == 0)
				return;
			if(GUI_ENC_API_GetKeyState() == 0)
			{
				if(lastKeyState == 0)
				{
					GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, NOTE_TIMER_PERIOD, 0);
					GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
					if(UI_APP_API_GetChnInfoCurState()!= CHNUM_STATE)
					{
						GUI_ENC_API_SetValue(ID_CHN_ChnInfo, WDT_CHN_SV_CLR, 0, 0);
						if(UI_APP_API_GetChnInfoCurState()!= NULL_STATE)
						{
							FullScreen_IPI_ClearScreen();
						}
						FullScreen_IPI_SetCurState(CHNUM_STATE);
						g_fullscreen_numKeyTime = 0;
					}
					GUI_ENC_API_SetValue(ID_CHN_ChnInfo, WDT_CHN_SV_KEY, key, 0);
					g_fullscreen_numKeyTime++;
					if(g_fullscreen_numKeyTime == 3)
					{
						GUI_ENC_API_SetValue(ID_CHN_ChnInfo, WDT_CHN_SV_CLR, 0, 0);
						g_fullscreen_numKeyTime = 0;
					}
				}
				else
				{
					g_fullscreen_numKeyTime = 0;
				}
				
			}
			lastKeyState = GUI_ENC_API_GetKeyState();
			break;
		case GUI_VK_SELECT:
			if(chnNum == 0)
				return;
#if defined (USE_FZVOD)
			if(bTVOD && SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
#elif defined(USE_ENREACH)	
			if(evtc_get_tsvodtag() && SYSTEM_GetNET_ON_OFF())

			{
				FullScreen_IPI_SetCurState(NULL_STATE);
				//printf("bTVOD = %d\n", bTVOD);
				ret = TP_Ipanel_CheckNetOk();

				if(FYF_OK == ret)
				{
					FullScreen_IPI_ClearScreen();
					FULLTOTVOD_SetCurState(/*2*/BPZHUN_STATE);	
					bIsSelect = FYF_TRUE;
					bIsLeft = FYF_FALSE;
					APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &index);
		        	GUI_ENC_API_SetValue(ID_VOI_TVOD, WDT_VOI_SV_RIGHT, index, 0);
					GUI_ENC_API_SwitchState(ID_DLG_FULLTOTVOD, 1);
					return; 			
				}
				else if (-2 == ret)
				{
					FullScreen_IPI_ClearScreen();
					
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET, WS_VISIBLE); 
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET_BAK, WS_VISIBLE); 
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET_BAK, 1);
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET, 1);
					FYF_API_thread_sleep(2000);
				//	FullScreen_IPI_ClearScreen();
					FullScreen_IPI_SetCurState(NULL_STATE);
					//GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
					return;
				}
				
			}

#endif

			if(UI_APP_API_GetChnInfoCurState() == CHNUM_STATE)
			{
				if(g_fullscreen_timeout != 0)
				{
					GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000, 0);
				}
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
				FullScreen_IPI_JudgeProgExist();
			}
			else if(UI_APP_API_GetChnInfoCurState() != TRACKSET_STATE)
			{
				UI_APP_API_SetChnListPreState(ID_DLG_ChnInfo);
				GUI_ENC_API_SwitchState(ID_DLG_ProgList, 1);
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
				//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			}
			break;
#ifndef SANMING_VERSION
		case GUI_VK_PROGTABLE:
			if(chnNum == 0)return;
			if(UI_APP_API_GetChnInfoCurState() != TRACKSET_STATE)
			{
				UI_APP_API_SetChnListPreState(ID_DLG_ChnInfo);		
				GUI_ENC_API_SwitchState(ID_DLG_ProgList, 1);
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
				//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}
			break;
#endif
#ifdef SYS_CONFIG_ENG_REMOTE
		case GUI_VK_RED:
#else
		case GUI_VK_RED:
#endif
			if(chnNum == 0)return;
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), curChn, &chnInfo))
			{
				return;
			}
			vol = chnInfo.m_AudioVolume;
			offset = chnInfo.m_AudioOffset;
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_FAV_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
			{
				APP_ChnDatabase_API_SetCurChnType(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e);
				APP_ChnDatabase_API_GetCurChnNO();
				
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo,WDT_TIM_SV_PERIOD,NOTE_TIMER_PERIOD,0);

				GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, (BU32)Rs_NoFavProg, 0);
				if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
				{
					FullScreen_IPI_ClearScreen();
				}
				FullScreen_IPI_SetCurState(NOPROG_STATE);
			}
			else
			{
				UI_APP_API_SetChnListPreState(ID_DLG_ChnInfo);		
				GUI_ENC_API_SwitchState(ID_DLG_ProgList, 1);			
				APP_ChnDatabase_API_JudgeChn0();
				if(chnInfo.m_Fav == 1)
				{
					APP_ChnDatabase_API_GetChnInfoByService (APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),chnInfo.m_ProgId, &chnInfo);
					APP_ChnDatabase_API_SetCurChnNO (chnInfo.m_ChnIndex);
					chnInfo.m_AudioVolume = vol;
					chnInfo.m_AudioOffset = offset;
					APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), chnInfo.m_ChnIndex, &chnInfo);
				}
				else
				{
					APP_ChnDatabase_API_SetCurChnNO (0);
					APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),  0, &chnInfo);
					APP_ChnDatabase_API_GetChnInfoByService(APP_ChnDatabase_API_GetCurChnType(), APP_CHN_GROUP_TYPE_ALL_e, chnInfo.m_ProgId, &chnInfo);
					APP_ChnDatabase_API_SetTypeAllCurChnNO(chnInfo.m_ChnIndex);
				}
				UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//	UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(),APP_ChnDatabase_API_GetCurChnNO(),1);
			}
			break;

		case GUI_VK_RadioORTV:

			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			if(APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
			{
				o_avType = APP_CHN_AV_TYPE_TV_e;
				to_avType = APP_CHN_AV_TYPE_RD_e;
				RS_ID = RS_NoRadioProg;
			}
			else
			{
				o_avType = APP_CHN_AV_TYPE_RD_e;
				to_avType = APP_CHN_AV_TYPE_TV_e;
				RS_ID = RS_NoTVProg;
			}
			
			if(APP_SUCCESS == APP_ChnDatabase_API_SetCurChnType(to_avType, APP_CHN_GROUP_TYPE_ALL_e) &&
				APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
			{
			#if 0
				APP_ChnDatabase_API_SetCurChnType(o_avType, APP_CHN_GROUP_TYPE_ALL_e);
				APP_ChnDatabase_API_GetCurChnNO();
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo,WDT_TIM_SV_PERIOD,NOTE_TIMER_PERIOD,0);

				if(chnNum == 0)
				{
					RS_ID = RS_NoProg;
				}
				GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, (BU32)RS_ID, 0);
				if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
				{
					FullScreen_IPI_ClearScreen();
				}
				FullScreen_IPI_SetCurState(NOPROG_STATE);
			#else
				APP_Com_API_StopProg(1);
				if(to_avType == APP_CHN_AV_TYPE_RD_e)
				{
					if(!UI_APP_API_bShowNoSignal())
					{
						APP_Com_API_ShowBmp(1);//广播LOGO
					}
					APP_Com_API_ShowLED("R000");
				}
				else
				{
					if(!UI_APP_API_bShowNoSignal())
					{
						APP_Com_API_ShowBmp(0);//黑屏
					}
					APP_Com_API_ShowLED("C000");
				}
				APP_Com_API_SetEcmNULL();
				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_SHOWNOPROG, 0, 0);				
				FullScreen_IPI_SetCurState(NULL_STATE);
				
			#endif
			}
			else
			{
				if(g_fullscreen_timeout != 0)
				{
					GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000,0);
				}

				GUI_ENC_API_PostMessage (GUI_WND_ID_INVALID, WM_HIDENOPROG, 0, 0);
				GUI_ENC_API_SetValueUnPaint(ID_STA_InfoBK_ChnInfo, WDT_STA_SV_STYLE, STYLE_ProgInfoBar_W618_H128, 0);
				APP_ChnDatabase_API_JudgeChn0();
				FullScreen_IPI_ChangeProg();
				FullScreen_IPI_SetCurState(INFOBAR_STATE);
			}
			break;
		case GUI_VK_TRACK:
			if(chnNum == 0)
				return;
		    GUI_ENC_API_SetValue(ID_CBX_Track_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
		    GUI_ENC_API_SetValue(ID_CBX_Offset_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_SetFocus(ID_ODC_ChName_ChnInfo);
			
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), curChn,&chnInfo))
			{
				return;
			}
			GUI_ENC_API_Update(ID_ODC_ChName_ChnInfo,1);
			if(chnInfo.m_AudioChn>3)
			{
				chnInfo.m_AudioChn = 3;
			}
			
			GUI_ENC_API_SetValue(ID_CBX_Track_ChnInfo, WDT_CBX_SV_INDEX, (BU32)chnInfo.m_AudioChn, 0);
			if(chnInfo.m_AudioOffset+15 > 30)
			{
				chnInfo.m_AudioOffset = 15;
			}
			else if(chnInfo.m_AudioOffset+15 < 0)
			{
				chnInfo.m_AudioOffset = -15;
			}
			GUI_ENC_API_SetValue(ID_CBX_Offset_ChnInfo, WDT_CBX_SV_INDEX, (BU32)(chnInfo.m_AudioOffset+15), 0);
			FullScreen_IPI_ClearScreen();
			FullScreen_IPI_SetCurState(TRACKSET_STATE);	
			break;
		case GUI_VK_MUTE:
			if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
			{
				GUI_ENC_API_Update(ID_STAMuteBk_ChnInfo,1);
				GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
				APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
				UI_APP_API_SetVolume();
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
				APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
			}
			break;
		case GUI_VK_RECALL:
			if(chnNum == 0)
				return;
			if(g_fullscreen_timeout != 0)
			{
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000, 0);
			}
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			FullScreen_IPI_PlayLastChn();
			FullScreen_IPI_SetCurState(INFOBAR_STATE);
			break;
		case GUI_VK_BLUE:
			GUI_ENC_API_SwitchState(ID_DLG_ProgState, 1);
			break;
		case GUI_VK_INFO:
			if(chnNum == 0)
				return;
			if(g_fullscreen_timeout != 0)
			{
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000, 0);
			}
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			if(UI_APP_API_GetChnInfoCurState() != INFOBAR_STATE)
			{
				FullScreen_IPI_SetChnInfo();
				if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
				{
					FullScreen_IPI_ClearScreen();
				}
				FullScreen_IPI_SetCurState(INFOBAR_STATE);
			}
			else
			{
				if (APP_EPG_API_GetCurAndNextEvent(APP_ChnDatabase_API_GetCurChnNO(), &CurEpgInfo, &NextEpgInfo) == 1)
				{
					APP_PRINT_DEBUG("CurEpgInfo = %s", CurEpgInfo.epgName );
					if (GUI_API_Strlen(CurEpgInfo.epgName) != 0)
					{
						GUI_API_Strcpy(g_epgdetail_eventNameStr, CurEpgInfo.epgName);
						g_epgdetail_eventNameStr[APP_EPG_NAME_LEN - 1] = '\0';
						APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO(), &ProgInfoListtemp);
						GUI_API_Strcpy(g_epgdetail_chnNameStr, ProgInfoListtemp.m_ChnName);
						g_epgdetail_chnNameStr[APP_EPG_NAME_LEN - 1] = '\0';
						GUI_ENC_API_SetValue(ID_STA_Title_EPGDetail, WDT_STA_SV_STRING, (BU32)(g_epgdetail_chnNameStr), 0);
						GUI_ENC_API_SetValue(ID_STA_EventName_EPGDetail,WDT_STA_SV_STRING,(BU32)(g_epgdetail_eventNameStr),0);
						APP_EPG_API_GetEventDetailText(0, g_epgdetail_detailNameStr, 255);
						//GUI_API_Memcpy(g_epg_detailNameStr, text, 255);
						detaillen = GUI_API_Strlen(g_epgdetail_detailNameStr);
						APP_Printf("g_epg_detailNameStr = %s %d\n", g_epgdetail_detailNameStr, detaillen);
						GUI_ENC_API_SetValue(ID_EDT_Detail_EPGDetail,WDT_EDT_SV_PAGE_INDEX,0,0);
						GUI_ENC_API_SetValue(ID_EDT_Detail_EPGDetail, WDT_EDT_SV_VALUE, (BU32)g_epgdetail_detailNameStr, detaillen);
						FullScreen_IPI_ClearScreen();
						FullScreen_IPI_SetCurState(NULL_STATE);
						GUI_ENC_API_SwitchState(ID_DLG_EPGDetail, 1);
					}
				}
			}
			break;
		case GUI_VK_EXIT:
				if(chnNum == 0)
					return;
				if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
				{
					FullScreen_IPI_SetCurState(NULL_STATE);	
			}
			break;
#ifdef SANMING_VERSION
		case GUI_VK_RES_5:
#else
		case GUI_VK_RES_5:
#endif
			//if (chnNum == 0)
				return;
			progSortCount = APP_ChnDatabase_API_GetSortCount();
			if (progSortCount == 0)
			{
				if(UI_APP_API_GetChnInfoCurState()!= NULL_STATE)
				{
					FullScreen_IPI_ClearScreen();
				}
				GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, RS_NoProgSort, 0);
				FullScreen_IPI_SetCurState(NOPROG_STATE);
			}
			else
			{
				UI_APP_API_SetSortProgPreState(ID_DLG_ChnInfo);
				GUI_ENC_API_SwitchState(ID_DLG_SortProgram, 1);
			}
			break;
#ifdef SANMING_VERSION
		case GUI_VK_RES_6:
#else
		case GUI_VK_F4:
#endif
			if(chnNum == 0)
				return;
			UI_APP_API_SetEpgPreState(ID_DLG_ChnInfo);
			GUI_ENC_API_SwitchState(ID_DLG_EPG, 1);
			UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			//UI_APP_API_CheckLock_PlayChn_led(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 1);
			break;
		case GUI_VK_MENU:
			GUI_ENC_API_SwitchState(ID_DLG_MainMenu,1);	
			break;
#ifdef SANMING_VERSION
		case GUI_VK_F2:
#else
		case GUI_VK_F6:
#endif
			GUI_ENC_API_SetValue(ID_MNU_SysSet, WDT_MNU_SV_SELECT, RS_Games, 0);
			//GUI_ENC_API_SetValue(ID_MNU_SysSet,WDT_MNU_SV_INDEX,0,0);
			GUI_ENC_API_SwitchState(ID_DLG_SysSet,1);	
			break;
#ifdef SANMING_VERSION
		case GUI_VK_RES_0:
#else
		case GUI_VK_F1:
#endif
#ifndef NO_BROADCAST
			broadcastNum = APP_DBA_API_GetBroadCastNum();
			if(broadcastNum == 0)
			{
				if(UI_APP_API_GetChnInfoCurState()!= NULL_STATE)
				{
					FullScreen_IPI_ClearScreen();
				}
				GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, RS_NoBroadcast, 0);
				FullScreen_IPI_SetCurState(NO_SERVICE);
			//	GUI_ENC_API_SetValue(ID_STA_Line1_SysInfo,WDT_STA_SV_DATAID,RS_NoBroadcast,0);
			//	GUI_ENC_API_SwitchState(ID_DLG_SysInfo,1);
			}
			/*
			else if(broadcastNum == 1)
			{
				GUI_ENC_API_SwitchState(ID_DLG_Null,1);	
			}
			*/
			else
			{
				UI_APP_API_SetBrdcastListPreState(ID_DLG_ChnInfo);
				GUI_ENC_API_SwitchState(ID_DLG_BroadcastList,1);	
			}
#else
			GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, RS_NoService, 0);
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo,WDT_TIM_SV_PERIOD,NOTE_TIMER_PERIOD,0);
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			if(UI_APP_API_GetChnInfoCurState() != NULL_STATE)
			{
				FullScreen_IPI_ClearScreen();
			}
			FullScreen_IPI_SetCurState(NO_SERVICE);
#endif
			break;
#ifdef ENABLE_PVR
#ifdef SANMING_VERSION
		case GUI_VK_F4:
#else
		case GUI_VK_F12:
#endif
			if (chnNum == 0)
				return;
			curChn = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_PVR_API_StartRec(curChn, 1) == APP_SUCCESS)
			{

				cur_time = APP_IPI_GetTimeWithSec();
				endhour = (cur_time + 3600) / 3600;
				endmin = ((cur_time + 3600) - endhour * 3600) / 60;
				APP_API_SetPVREndTime(APP_IPI_GetDay(), endhour, endmin);
				APP_PVR_API_SetRecTime(cur_time, cur_time+3600);
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"开始录制,操作锁定",0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	

			}
			else
			{
				UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
				GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"未插入USB,无法录制",0);
				GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	
				APP_PRINT_ERROR("APP_PVR_API_StartRec error!!!!!!\n");
			}
	
			break;
#endif
#ifdef SANMING_VERSION
		case GUI_VK_RES_2:
#else

		case GUI_VK_F3:
#endif
			//APP_SYS_API_GetSysInfo(APP_SYS_SwitchItem_e,&netEnable);
			/*1表示关(单向),0表示开(双向)*/
			if(SYSTEM_GetNET_ON_OFF() == FYF_FALSE)
			{

			}
			else
			{

#if defined(USE_FZVOD) || defined(USE_ENREACH)		
				ret = TP_Ipanel_CheckNetOk();
				
				if(FYF_OK == ret)
				{
					Ipanel_SetEnterVodOrTvod_Flag(2);
					APP_Com_API_StopProg(1);
					Ipanel_SetEntrance_Flag(FYF_TRUE);
					OW_VOD_SetDhcpFlag(FYF_TRUE);
					GUI_ENC_API_SwitchState(ID_DLG_Null,1); 
				}
				else if (-2 == ret)
				{
					FullScreen_IPI_ClearScreen();
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET, WS_VISIBLE); 
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET_BAK, WS_VISIBLE); 
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET_BAK, 1);
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET, 1);
					
					FYF_API_thread_sleep(2000);
				
					FullScreen_IPI_ClearScreen();
					
				}
				
#endif			
			}
			break;
#ifdef SANMING_VERSION
		case GUI_VK_RES_1:
		case GUI_VK_RES_3:
#else
		case GUI_VK_F2:
#endif
			if(SYSTEM_GetNET_ON_OFF() == FYF_FALSE)
			{

			}
			else
			{

#if defined(USE_FZVOD) || defined(USE_ENREACH)		
				ret = TP_Ipanel_CheckNetOk();
				
				if(FYF_OK == ret)
				{
					Ipanel_SetEnterVodOrTvod_Flag(1);
					APP_Com_API_StopProg(1);
					Ipanel_SetEntrance_Flag(FYF_TRUE);
					OW_VOD_SetDhcpFlag(FYF_TRUE);
					GUI_ENC_API_SwitchState(ID_DLG_Null,1); 
				}
				else if (-2 == ret)
				{
					FullScreen_IPI_ClearScreen();
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET, WS_VISIBLE); 
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET_BAK, WS_VISIBLE); 
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET_BAK, 1);
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET, 1);
					
					FYF_API_thread_sleep(2000);
				
					FullScreen_IPI_ClearScreen();
					
				}
#endif
			}
			break;
#ifdef USE_FZVOD

#ifdef SYS_CONFIG_ENG_REMOTE
	case GUI_VK_YELLOW:
#else
	case GUI_VK_F10:
#endif			

	
			if(bTVOD && SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
			{
				FullScreen_IPI_SetCurState(NULL_STATE);
				ret = TP_Ipanel_CheckNetOk();//Tp_Ow_CheckTvodChannel();
				if (FYF_OK == ret)
				{
					FullScreen_IPI_ClearScreen();

					FULLTOTVOD_SetCurState(/*1*/BPJUMP_STATE);				
					GUI_ENC_API_SwitchState(ID_DLG_FULLTOTVOD, 1);
				}
				else if (-2 == ret)
				{
					FullScreen_IPI_ClearScreen();
					
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET, WS_VISIBLE); 
					GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET_BAK, WS_VISIBLE); 
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET_BAK, 1);
					GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET, 1);
					
					FYF_API_thread_sleep(2000);
					
					FullScreen_IPI_ClearScreen();
					
				}
			}

			break;
#endif
		case GUI_VK_F5:
			
			GUI_ENC_API_SwitchState(ID_DLG_OperationGuide, 1);
			break;
		case GUI_VK_F8:
			break;

#ifdef SANMING_VERSION
		case GUI_VK_RES_4:
			if (chnNum == 0)
			{
				return;
			}
			if (APP_ChnDatabase_API_GetCurChnType() == APP_CHN_AV_TYPE_TV_e)
			{
				GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit, WDT_STA_SV_DATAID, RS_TVEdit, 0);
			}
			else
			{
				GUI_ENC_API_SetValue(ID_STA_Title_ChnEdit, WDT_STA_SV_DATAID, RS_RadioEdit, 0);
			}
			UI_APP_API_SetChnEditPreState(0);
			GUI_ENC_API_SwitchState(ID_DLG_ChnEdit, 1);
			break;
#endif
		default:
			break;

		}
	}
#ifdef ENABLE_PVR
	else if(APP_PVR_API_GetRecStatus(1) == 1)
	{
		switch(key)
		{
#ifdef SANMING_VERSION
		case GUI_VK_F4:
#else
		case GUI_VK_F12:
#endif
 			APP_PVR_API_StopRec(1);
			//显示停止录制当前节目
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"已停止录制当前节目",0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	

			break;
#ifdef SANMING_VERSION
		case GUI_VK_F3:
#else
		case GUI_VK_F11://后退键，进入回放状态
#endif
			if (APP_PVR_API_GetRePlayStatus() == 0)
			{
			    APP_SYS_API_GetSysInfo(APP_SYS_VOLUME_e, &index);
		        GUI_ENC_API_SetValue(ID_VOI_TimeShift, WDT_VOI_SV_RIGHT, index, 0);
				GUI_ENC_API_SwitchState(ID_DLG_TimeShift, 1);
				APP_Com_API_StopProg(0);
				APP_Com_API_Stop_Signal_Dectect();
				APP_PVR_API_DetachAV();
				APP_PVR_API_StartRePlay(1);
				APP_PVR_API_SetDemux(1);
			//	APP_PVR_API_PlaySeek(0, 1, 0);
			}
			break;
		default:
			//提示正在录制当前节目操作锁定,按F12键停止录制当前节目
			UI_APP_API_SetUSBEventType(APP_EVENT_NOTIFY_OTHER);
			GUI_ENC_API_SetValue(ID_STA_Line1_EffectsInfo,WDT_STA_SV_STRING,(BU32)"正在录制节目,按F12键停止录制",0);
			GUI_ENC_API_SwitchState(ID_DLG_EffectsInfo,1);	

			break;
		}
	}
#endif
}


void UI_APP_FullScreen_Timer(void)
{
	BU32 rs_id;
	static BU32 save_times = 0;
	BU32 g_sys_volumesave = 0;
	BU32 curstate = UI_APP_API_GetChnInfoCurState();
	APP_EPG_API_StopGetEpg();
	switch(curstate)
	{
	case NO_SERVICE:
	case INFOBAR_STATE:
	case VOICE_STATE:
		rs_id = UI_APP_API_GetChnInfoSaveID();
		APP_PRINT_DEBUG("rs_id = %d\n", rs_id);
		GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, rs_id, 0);
		FullScreen_IPI_SetCurState(NULL_STATE);
		break;
	case NOPROG_STATE:
		if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
		{
			return;
		}
		else
		{
			rs_id = UI_APP_API_GetChnInfoSaveID();
			GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, rs_id, 0);
			GUI_ENC_API_SetValue(ID_CHN_ChnInfo, WDT_CHN_SV_NUM, (BU32)(APP_ChnDatabase_API_GetCurChnNO()+APP_ChnDatabase_API_GetChn0Flag()), 0);
			FullScreen_IPI_ClearScreen();
			FullScreen_IPI_SetCurState(INFOBAR_STATE);
		}
		break;
	case CHNUM_STATE:
		FullScreen_IPI_JudgeProgExist();
		break;
	case NULL_STATE:
		if (++save_times % 5 == 0)
		{
			save_times = 0;
			APP_SYS_API_GetSysInfo(APP_SYS_VolumeSave_e, &g_sys_volumesave);
			if (g_sys_volumesave == 0 && g_fullscreen_save == 1)
			{
				APP_PRINT_DEBUG("save flash volume %d\n", FYF_API_time_ms());
			    APP_ChnDatabase_API_UpDataToFlash(1);
				
				g_fullscreen_save = 0;
			}
			
		}
		break;
	case TRACKSET_STATE:
		break;
	default:
		break;
	}
//	GUI_ENC_API_Update (ID_STA_UrgentBrct_ChInfo, 1);
}

void UI_APP_FullScreen_TrackOnPaint(BU32 hdc,BU32 lparam)
{
	BU08 ProName[40];
	BU16 curChn;
	ChnDataBase_ChnInfo_S chn;
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn,&chn))
	{
		return;
	}
	
	sprintf(ProName,"%03d %s", curChn + APP_ChnDatabase_API_GetChn0Flag(), chn.m_ChnName);		
	
	if(GUI_ENC_API_IsFocus(ID_ODC_ChName_ChnInfo))
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TrackSetYellowBar_W224_H36);
		ProName[17] = 0;
		GUI_API_TextOut(hdc,96-GUI_API_GetTextLenght(ProName,GUI_API_Strlen(ProName))/2,7,ProName,GUI_API_Strlen(ProName),COLOR_BLACK,0,1);
	}
	else
	{
		GUI_API_DrawStyle(hdc, 0, 0, STYLE_TrackSetBlueBar_W224_H36);
		ProName[17] = 0;
		GUI_API_TextOut(hdc,96-GUI_API_GetTextLenght(ProName,GUI_API_Strlen(ProName))/2,7,ProName,GUI_API_Strlen(ProName),COLOR_WHITE,0,1);
	}
}

void UI_APP_FullScreen_TrackKey(BU32 key)
{
	BU32 index;
	BU32 volume;
	BU16 ProgNo;
	ChnDataBase_ChnInfo_S chn;
	BU08 volumeChange = 0;
	
	APP_EPG_API_StopGetEpg();
	//节目名称
	if(GUI_ENC_API_IsFocus(ID_ODC_ChName_ChnInfo))
	{	
		switch(key)
		{
		case GUI_VK_LEFT:
			ProgNo = APP_ChnDatabase_API_GetCurChnNO();
			if(ProgNo == 0)
				ProgNo = APP_ChnDatabase_API_GetCurTypeChnNum()-1;
			else 
				ProgNo --;
			APP_ChnDatabase_API_SetCurChnNO(ProgNo);
			FullScreen_IPI_TrackSetChangeProg();
			break;
		case GUI_VK_RIGHT:
			ProgNo = APP_ChnDatabase_API_GetCurChnNO();
			ProgNo ++;
			if (ProgNo > APP_ChnDatabase_API_GetCurTypeChnNum()-1)
				ProgNo = 0;
			APP_ChnDatabase_API_SetCurChnNO(ProgNo);
			FullScreen_IPI_TrackSetChangeProg();
			break;	
		case GUI_VK_DOWN:
			GUI_ENC_API_SetFocus(ID_CBX_Track_ChnInfo);
			GUI_ENC_API_SetValue(ID_CBX_Track_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
			GUI_ENC_API_Update(ID_ODC_ChName_ChnInfo,1);
			break;
		case GUI_VK_UP:
			GUI_ENC_API_SetFocus(ID_CBX_Offset_ChnInfo);
			GUI_ENC_API_SetValue(ID_CBX_Offset_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
			GUI_ENC_API_Update(ID_ODC_ChName_ChnInfo,1);
			break;
		default:
			break;

		}
	}
	//声道
	else if(GUI_ENC_API_IsFocus(ID_CBX_Track_ChnInfo))
	{
		switch(key)
		{
		case GUI_VK_LEFT:
		case GUI_VK_RIGHT:
			g_fullscreen_save = 1;
			ProgNo = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn))
			{
				return;
			}
			GUI_ENC_API_GetValue(ID_CBX_Track_ChnInfo, WDT_CBX_SV_INDEX, &index, 0);
			chn.m_AudioChn = (BU08)index;
			APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn);
			UI_APP_API_SetVolume();
			volumeChange = 1;
			break;
		case GUI_VK_DOWN:
			GUI_ENC_API_SetValue(ID_CBX_Offset_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_Offset_ChnInfo);
			GUI_ENC_API_SetValue(ID_CBX_Track_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			break;
		case GUI_VK_UP:
			GUI_ENC_API_SetFocus(ID_ODC_ChName_ChnInfo);
			GUI_ENC_API_SetValue(ID_CBX_Track_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_Update(ID_ODC_ChName_ChnInfo,1);
			break;
		default:
			break;
		}
	}	
	//音量补偿
	else if(GUI_ENC_API_IsFocus(ID_CBX_Offset_ChnInfo))
	{
		switch(key)
		{
		case GUI_VK_LEFT:
		case GUI_VK_RIGHT:
			g_fullscreen_save = 1;
			GUI_ENC_API_GetValue(ID_CBX_Offset_ChnInfo, WDT_CBX_SV_INDEX, &index, 0);
			ProgNo = APP_ChnDatabase_API_GetCurChnNO();
			if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn))
			{
				return;
			}
			chn.m_AudioOffset = (BS08)(index-15);
			APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn);

			UI_APP_API_SetVolume();
			volumeChange = 1;
			break;
		case GUI_VK_DOWN:
			GUI_ENC_API_SetFocus(ID_ODC_ChName_ChnInfo);
			GUI_ENC_API_SetValue(ID_CBX_Offset_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			GUI_ENC_API_Update(ID_ODC_ChName_ChnInfo,1);
			break;
		case GUI_VK_UP:
			GUI_ENC_API_SetValue(ID_CBX_Track_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_BLACK, 0);
			GUI_ENC_API_SetFocus(ID_CBX_Track_ChnInfo);
			GUI_ENC_API_SetValue(ID_CBX_Offset_ChnInfo, GUI_SV_TEXTCOLOR, COLOR_WHITE, 0);
			break;
		default:
			break;
		}
	}

	if(key == GUI_VK_EXIT || key == GUI_VK_SELECT)
	{	

		FullScreen_IPI_SetChnInfo();
		FullScreen_IPI_ClearScreen();
		if(g_fullscreen_save)
		{
			g_fullscreen_save =0;
 			GUI_ENC_API_SetValue(ID_STA_Text_ChnInfo, WDT_STA_SV_DATAID, RS_SaveWait, 0);
 			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, 2000, 0);
			FullScreen_IPI_SetCurState(NOPROG_STATE);
			APP_ChnDatabase_API_UpDataToFlash(0);
		}
		else
		{
			if(g_fullscreen_timeout != 0)
			{
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PERIOD, g_fullscreen_timeout*1000,0);
			}
			GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_REPLAY, 0, 0);
			FullScreen_IPI_SetCurState(INFOBAR_STATE);
		}
		GUI_ENC_API_SetFocus(ID_DLG_ChnInfo);
	}
	else if(key == GUI_VK_MUTE)
	{
		if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
		{
			GUI_ENC_API_Update(ID_STAMuteBk_ChnInfo,1);
			GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
			APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
		}
		else
		{
			GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_Silence_W51_H34, 0);
			APP_Audio_API_SetMuteState(APP_AUDIO_MUTE_STATE);
		}

	}	

	if(volumeChange == 1 && (GUI_ENC_API_IsFocus(ID_CBX_Track_ChnInfo) || GUI_ENC_API_IsFocus(ID_CBX_Offset_ChnInfo)))
	{
		if(APP_AUDIO_MUTE_STATE == APP_Audio_API_GetMuteState())
		{
			GUI_ENC_API_Update(ID_STAMuteBk_ChnInfo,1);
			GUI_ENC_API_SetValue(ID_STAMute_ChnInfo, WDT_STA_SV_STYLE, (BU32)STYLE_NULL, 0);
			APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
			UI_APP_API_SetVolume();
		}
	}
}


#if 0
//音量补偿
void UI_APP_FullScreen_OffSetPaint(BU32 hdc,BU32 lparam)
{
    BU08 OffSetValue[5];
    BU16 ProgNo;
	ChnDataBase_ChnInfo_S chn;
    ProgNo = APP_ChnDatabase_API_GetCurChnNO();
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn))
	{
		return;
	}

	if(lparam == GUI_VK_OFFSET_DOWN)
	{
	    g_fullscreen_save = 1;
		if(chn.m_AudioOffset <= -10)
		{
		    chn.m_AudioOffset = -10;
		}
		else
		{
		    chn.m_AudioOffset = chn.m_AudioOffset-1;
		}
		APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn);
        
		UI_APP_API_SetVolume();
		GUI_API_DrawStyle(hdc,0,0,STYLE_OffSetStyle_W268_H32);
		sprintf(OffSetValue, "%d", chn.m_AudioOffset);
		GUI_API_TextOut(hdc,130,7,OffSetValue,GUI_API_Strlen(OffSetValue),COLOR_BLACK,0,1);
		
	}
	else if(lparam == GUI_VK_OFFSET_UP)
	{
	    g_fullscreen_save = 1;
		if(chn.m_AudioOffset >= 10)
		{
		    chn.m_AudioOffset = 10;
		}
		else
		{
		    chn.m_AudioOffset = chn.m_AudioOffset+1;
		}
		APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn);
        
		UI_APP_API_SetVolume();
		GUI_API_DrawStyle(hdc,0,0,STYLE_OffSetStyle_W268_H32);
		sprintf(OffSetValue, "%d", chn.m_AudioOffset);
		GUI_API_TextOut(hdc,130,7,OffSetValue,GUI_API_Strlen(OffSetValue),COLOR_BLACK,0,1);
		
	}
}

//声道设置
void UI_APP_FullScreen_TrackSetPaint(BU32 hdc, BU32 lparam)
{
    BU08 TrackValue[15];
	PU08 *CurDicData = DicData;
	BU32 lang;
    BU16 ProgNo;
	ChnDataBase_ChnInfo_S chn;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e,&lang);
	if(lang == APP_SYS_LANGUAGE_ENGLISH_e)
	{
	    CurDicData = DicDataEnglish;
	}
	else
	{
	    CurDicData = DicData;
	}
    ProgNo = APP_ChnDatabase_API_GetCurChnNO();
	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn))
	{
		return;
	}
	switch(chn.m_AudioChn)
	{
	case 0:
		chn.m_AudioChn = 1;
		GUI_API_DrawStyle(hdc,0,0,STYLE_TrackSetBk_W260_H62);
		GUI_API_DrawStyle(hdc,10,1,STYLE_TrackIcon1_W60_H60);
		sprintf(TrackValue, "%s", CurDicData[RS_Left]);
		GUI_API_TextOut(hdc,100,20,TrackValue,GUI_API_Strlen(TrackValue),COLOR_BLACK,0,1);
		break;
	case 1:
		chn.m_AudioChn = 2;
		GUI_API_DrawStyle(hdc,0,0,STYLE_TrackSetBk_W260_H62);
		GUI_API_DrawStyle(hdc,190,1,STYLE_TrackIcon2_W60_H60);
		sprintf(TrackValue, "%s", CurDicData[RS_Right]);
		GUI_API_TextOut(hdc,100,20,TrackValue,GUI_API_Strlen(TrackValue),COLOR_BLACK,0,1);
	    break;
	case 2:
		chn.m_AudioChn = 3;
		GUI_API_DrawStyle(hdc,0,0,STYLE_TrackSetBk_W260_H62);
		GUI_API_DrawStyle(hdc,10,1,STYLE_TrackIcon1_W60_H60);
		GUI_API_DrawStyle(hdc,190,1,STYLE_TrackIcon2_W60_H60);
		sprintf(TrackValue, "%s", CurDicData[RS_Single]);
		GUI_API_TextOut(hdc,100,20,TrackValue,GUI_API_Strlen(TrackValue),COLOR_BLACK,0,1);
		break;
	case 3:
		chn.m_AudioChn = 0;
		GUI_API_DrawStyle(hdc,0,0,STYLE_TrackSetBk_W260_H62);
		GUI_API_DrawStyle(hdc,10,1,STYLE_TrackIcon1_W60_H60);
		GUI_API_DrawStyle(hdc,190,1,STYLE_TrackIcon2_W60_H60);
		sprintf(TrackValue, "%s", CurDicData[RS_Stereo]);
		GUI_API_TextOut(hdc,100,20,TrackValue,GUI_API_Strlen(TrackValue),COLOR_BLACK,0,1);
		break;
	default:
		break;
	}
	APP_ChnDatabase_API_SetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),ProgNo,&chn);
    UI_APP_API_SetVolume();
}
#endif

//当前和下个节目的定时刷新函数
void UI_APP_Fullscreen_RenewEvent_Timer(void)
{
	UI_APP_API_RenewEvent(ID_STA_Info1_ChnInfo,ID_STA_Info2_ChnInfo,ID_PBR_Epg_ChnInfo,310,1);
}

void UI_APP_NetState_Timer(void)
{
#ifdef FZTEST
	
	BU32 curstate = UI_APP_API_GetChnInfoCurState();

	switch(curstate)
	{
		
		case NET_STATE:
			if (g_GetIpCount == 0)
			{
				FullScreen_IPI_ClearScreen();
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_STOP,0,0);
				GUI_ENC_API_SetValue(ID_TIM_Event_ChnInfo, WDT_TIM_SV_STOP,0,0);
				
			}
			GUI_API_SetKeyReceiveMode(KEY_RECEIVE_CLOSE);
			
			GUI_ENC_API_SetValue(ID_STA_FULLSCREEN_CHECKNET, WDT_STA_SV_STRING, "正在获取IP，请稍候", 0);
			GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET, WS_VISIBLE); 
			GUI_ENC_API_SetVisible(ID_STA_FULLSCREEN_CHECKNET_BAK, WS_VISIBLE); 
			GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET_BAK, 1);
			GUI_ENC_API_Update(ID_STA_FULLSCREEN_CHECKNET, 1);
			printf("\033[0;31mg_GetIpCount = %d\033[0m\n", g_GetIpCount);
			if (g_GetIpCount%5 == 0)
			{	
				system("killall -9 udhcpc &");
				
				if (TP_Ipanel_CheckNetOk() != 0)
				{
					system("udhcpc -V HZCNCSTBV1 -n 3 &");
				}
				else
				{
					g_GetIpCount = 0;
					GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
					system("killall -9 udhcpc &");
					FullScreen_IPI_ClearScreen();
					FullScreen_IPI_SetCurState(INFOBAR_STATE);
					
					GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PLAY,0,0);
					
					GUI_ENC_API_SetValue(ID_TIM_Event_ChnInfo, WDT_TIM_SV_PLAY,0,0);
					return;
				}
			}
			
			g_GetIpCount++;
			if (g_GetIpCount >= 40)
			{
				printf("\033[0;31m======================g_GetIpCount = %d\033[0m\n", g_GetIpCount);
				g_GetIpCount = 0;
				GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
				system("killall -9 udhcpc &");
				FullScreen_IPI_ClearScreen();
				FullScreen_IPI_SetCurState(INFOBAR_STATE);
				
				GUI_ENC_API_SetValue(ID_TIM_ChnInfo, WDT_TIM_SV_PLAY,0,0);
				
				GUI_ENC_API_SetValue(ID_TIM_Event_ChnInfo, WDT_TIM_SV_PLAY,0,0);
			}
			
			break;
			
		default:
			break;
			
	}
#endif	
}


void UI_APP_FullScreen_ADVOnPaint(BU32 hdc,BU32 lparam)
{
	BU32 hDecoder, hDecSurface;
	FYF_Rect_s RealRect;
	BS32 ret;
	BS32 i;
	BU32 hLayerSurface;
	BU32 ucOldVersionNumber;
	ChnDataBase_ChnInfo_S chnInfo;
	if (lparam == 2)
	{
		//GUI_API_FillRect(GUI_API_GetDesktopWindow(), 530, 400, 140, 106, COLOR_TRANS);
	}
	else if (lparam == 3 || lparam == 1)
	{
#if 0
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(), APP_ChnDatabase_API_GetCurChnNO(), &chnInfo))
		{
			return;
		}
#endif
		if (APP_ChnDatabase_API_GetCurTypeChnNum() > 0)
	 		APP_ADV_API_TestDisPlay(APP_ChnDatabase_API_GetCurChnNO() + 1, 1);
	}
	else if (lparam == 4)
	{
		if (APP_ChnDatabase_API_GetCurTypeChnNum() > 0)
	 		APP_ADV_API_TestDisPlay(APP_ChnDatabase_API_GetCurChnNO() + 1, 0);
	}
}



