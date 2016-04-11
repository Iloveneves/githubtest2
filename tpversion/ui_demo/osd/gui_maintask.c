/*===============================================================================

 File Name:gui_maintask.c

 Author:

 Create Time:
 
 Description:

 History:

===============================================================================*/
/*-------------------------------------------------------------------------------
	include files
-------------------------------------------------------------------------------*/
#include "include/resource.h"
#include "../../../tpmiddle/include/app_common.h"
#include "../event/ui_app_api.h"
BU32 g_first_mainmenu = 0;

extern BU08 bHaveDL;
extern BS32 RegionID;
extern BU32	semaphoreDemux;
BU32 semaphoreEcm = 0;
static BS32 s_dhcp_threadId = 0;
BU08 arealock_falg = 0;

void UI_APP_FirstState(void)
{
	BU32 lang,sysChnLock;
	BU16 curChn = 0;
	ChnDataBase_ChnInfo_S chnInfo;
	BU32 value = 0;
	APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
	//APP_ChnDatabase_API_SetCurChnType(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
#ifndef NO_BROADCAST
	DBA_API_CreateBroadCastList();
#endif
	APP_ChnDatabase_API_JudgeChn0();
	if(APP_ChnDatabase_API_GetCurTypeChnNum() == 0)
	{
		APP_Com_API_ShowLED("C000");
#if 0
		GUI_ENC_API_SwitchState(ID_DLG_Search, 1);
		APP_NIT_API_SetNitVerCheck(0);
#else
		GUI_ENC_API_SetValue(ID_STA_Line1_AutoInfo, WDT_STA_SV_DATAID, RS_Message, 0);
		GUI_ENC_API_SetValue(ID_STA_Line2_AutoInfo, WDT_STA_SV_DATAID, RS_EnterSearch, 0);
		GUI_ENC_API_SetValue(ID_STA_Line3_AutoInfo, WDT_STA_SV_DATAID, RS_Exit, 0);
	    GUI_ENC_API_SwitchState(ID_DLG_AutoInfo,true);
#endif
		return;
	}	

	//正常播放
	curChn = APP_ChnDatabase_API_GetCurChnNO();
	APP_Printf("curChn = %d\n", curChn);
	APP_ChnDatabase_API_SetLastChnInfo(curChn);
	if(APP_ChnDatabase_API_GetChn0Flag()==0)//有0频道
	{
		APP_ChnDatabase_API_SetCurChnNO(0);
		curChn = 0;
	}
#if defined(USE_FZVOD) || defined(USE_ENREACH)
	APP_SYS_API_GetSysInfo(APP_SYS_SwitchItem_e,&value);
	if (value == 0 && APP_NIT_API_GetOTACheck() == 0 && APP_NIT_API_GetNitVerCheck() == 0)
	{
		Ipanel_SetEnterVodOrTvod_Flag(2);
		Ipanel_SetEntrance_Flag(FYF_TRUE);
		OW_VOD_SetDhcpFlag(FYF_TRUE);
		GUI_ENC_API_SwitchState(ID_DLG_Null,1);
		//GUI_ENC_API_SwitchState(ID_DLG_VOD_FIRST_STATE, FYF_TRUE);
	}
	else
	
#endif		
	{
	g_first_mainmenu = 1;
	GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);
	UI_APP_API_PlayChn(APP_ChnDatabase_API_GetCurChnNO(), APP_ChnDatabase_API_GetCurChnNO(), 0);
	}

	if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(APP_ChnDatabase_API_GetCurChnType(), APP_ChnDatabase_API_GetCurGroupType(),curChn, &chnInfo))
	{
		return;
	}
	APP_SYS_API_GetSysInfo(APP_SYS_CHN_LOCK_e, &sysChnLock);
	if(chnInfo.m_Lock && sysChnLock)
	{
		APP_Signal_API_TuneToNewParam(chnInfo.m_FreqMHz, chnInfo.m_SymKbps, chnInfo.m_QAM);
	}

}

void IPI_SetFirstStateAndInit(void)
{
	BU32 bLock;
	BU32 lang;
	BU32 bitmap = 0;
	BU32 StartupLock;
	APP_SYS_API_GetSysInfo(APP_SYS_LANGUAGE_e, &lang);
	if(lang == APP_SYS_LANGUAGE_CHINESE_e)
	{
		GUI_API_SetDictionaryDataAddr(DicData);
		UI_APP_SetIcoToCn();
	}
	else
	{
		GUI_API_SetDictionaryDataAddr(DicDataEnglish);
		UI_APP_SetIcoToEn();
	}
	APP_Audio_API_SetMuteState(APP_AUDIO_UNMUTE_STATE);
	
	UI_APP_SYS_ShowSysTime();
	APP_Com_API_ShowBmp(bitmap);//把logo的显示入栈操作
	APP_SYS_API_SetUpdateType(APP_SYS_OTA_UD_NULL);
	APP_SYS_API_GetSysInfo(APP_SYS_STARTUP_LOCK_e, &StartupLock);
	if(StartupLock == 1)
	{
	    GUI_ENC_API_SwitchState(ID_DLG_StartUpLock, 1);
	}
	else
	{
		UI_APP_FirstState();
	}
	
}
void FYF_POWER_on(void)
{
	GUI_ENC_API_SwitchState (ID_DLG_ChnInfo, 1);
}

BU32 UI_APP_SignalNotify(BU32 para1,BU32 para2);
#define GUI_FIXEDMEM_SIZE (300*1024)
char g_guifixedmem[GUI_FIXEDMEM_SIZE];
BU32 g_mainTask_end = FYF_FALSE;

#if defined(USE_FZVOD) || defined(USE_ENREACH)
void  IpDhcpMonitor( void *pvParam )	
{
	BU32 value;
	static BU32 count = 0;

	while (1)
	{

		FYF_API_thread_sleep(8000);
		APP_SYS_API_GetSysInfo(APP_SYS_AutoIP_e, &value);
/*		if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE && !Inthird_GetFlag() && TP_Ipanel_CheckNetOk() == -3 && GUI_ENC_API_GetCurState() == ID_DLG_ChnInfo)
		{
			if (++count < 5)
			{
				system("killall -9 udhcpc");
				system("udhcpc -V HZCNCSTBV1 -n 2 ");
				system("killall -9 udhcpc");
			}
		}
*/

		if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE && !Inthird_GetFlag() && TP_Ipanel_CheckNetOk() == 0 && GUI_ENC_API_GetCurState() == ID_DLG_ChnInfo) 
		{
			test_list();
		}
	}
}
#endif

extern NetSetInfo IpInfomation;
extern BU08 g_ip_address[16];
extern BU08 g_sub_net_Mark[16];
extern BU08 g_gateway_ip[16];
extern BU08 g_first_chose_dns[16];
extern BU08 g_reserve_dns[16];
int DHCPInit(void)
{
	BU32 value;
	BU08 *tmpIP = NULL;
	int i = 0;
	

	APP_SYS_API_GetSysInfo(APP_SYS_AutoIP_e, &value);
	system("ifconfig eth0 mtu 1492");


	if (IpInfomation.netflag == 0xa0)	
	{
		memcpy(g_ip_address, IpInfomation.ipaddr,  16);
		memcpy(g_sub_net_Mark, IpInfomation.netmask,  16);
		memcpy(g_gateway_ip, IpInfomation.gateway,  16);
		memcpy(g_first_chose_dns, IpInfomation.firstdns,  16);
		memcpy(g_reserve_dns, IpInfomation.secdns, 16);
	}
	
	printf("buffer = %s\n",g_ip_address);
	printf("buffer = %s\n",g_sub_net_Mark);
	printf("buffer = %s\n",g_gateway_ip);
	printf("buffer = %s\n",g_first_chose_dns);
	printf("buffer = %s\n",g_reserve_dns);
	

	if (SYSTEM_GetNET_ON_OFF() == FYF_TRUE)
	{	
		printf("value = %d\n",value);
		
		if (value == 0)
		{	
			system("killall -9 udhcpc");
			system("udhcpc -V HZCNCSTBV1 -n 2 ");
			system("killall -9 udhcpc");
		}
		else
		{
			HI_ETH_IPAddressSet(0, g_ip_address);
			HI_ETH_SubNetmaskSet(0, g_sub_net_Mark);
			HI_ETH_GatewaySet(0, g_gateway_ip);
			HI_ETH_DNSSet(0, 1, g_first_chose_dns);

		}
		
#if defined(USE_FZVOD) || defined(USE_ENREACH)
	test_list();
#endif

	}

#if defined(USE_FZVOD) || defined(USE_ENREACH)
	if (FYF_NULL == s_dhcp_threadId)
	{
		s_dhcp_threadId = FYF_API_thread_create("IpDhcpMonitor",IpDhcpMonitor, NULL, 0, 0);  
		if(FYF_NULL == s_dhcp_threadId)
		{
			APP_PRINT_ERROR("IpDhcpMonitor error\n");
			return FYF_ERR;
		}	

	}
#endif

}

int TPOWVOD_SetStbid(void)
{
	int i, j;
	MAC_BOX_INFO_STRUCT pstMacBoxInfo;
	APP_SYS_API_GetStbID(&pstMacBoxInfo);
	char mac[20];
#if defined(USE_FZVOD) || defined(USE_ENREACH)
	/******************处理 stbid***************************/
	for (i = 0; i < 4; i++)
	{
		*(stbid + 0 + i) = pstMacBoxInfo.AreaIndicatorCode[i];
	}

	for (i = 0; i < 2; i++)
	{
		*(stbid + 4 + i) = pstMacBoxInfo.ManufactureCode[i];
	}

	for (i = 0; i < 3; i++)
	{
		*(stbid + 6 + i) = pstMacBoxInfo.StbFactoryIndicator[i];
	}

	for (i = 0; i < 2; i++)
	{
		*(stbid + 9 + i) = pstMacBoxInfo.StbType[i];
	}

	for (i = 0; i < 2; i++)
	{
		*(stbid + 11 + i) = pstMacBoxInfo.HwVersion[i];
	}

	*(stbid + 13) = pstMacBoxInfo.StbIndicatorType[0];

	for (i = 0; i < 12; i++)
	{
		*(stbid + 14 + i) = pstMacBoxInfo.ucMacAddress[i];
	}

	if (pstMacBoxInfo.ucMacAddress[10] == '0' && pstMacBoxInfo.ucMacAddress[11] == '0')
	{
		*(stbid + 14 + 10) = 0;
	}
	else
	{
		*(stbid + 14 + 12) = 0;
	}
#endif
	j = 0;
	for (i = 0; i < 12; i += 2)
	{
		mac[j++] = pstMacBoxInfo.ucMacAddress[i];
		mac[j++] = pstMacBoxInfo.ucMacAddress[i+1];
		mac[j++] =':';
	}
	mac[j - 1] = 0;
	HI_ETH_SetMac(0, mac);

}


void ThreadSignalDetect(void * param);
void MainTask(void *pvParam)
{	
	GUI_Resource_s resource;
	FYF_PVR_EventCallBack_S pvr_CallBackParam;
	GUI_API_SetTransValue(GUI_COLOR_TRANSPARENT);
	GUI_API_SetForceGroundColor(GUI_COLOR_BLACK);
	GUI_API_SetBackGroundColor(GUI_COLOR_WHITE);
	GUI_API_SetLineDistance(10);
	GUI_API_SetBmpLibAddr(acbmplib);
 	GUI_API_SetFontLibAddr(acfontlib);
	GUI_API_SetRectFreeSize(0);
	
	RegisterPSD();
	GUI_API_CreateProtect();
	//OSD_ST5105_RegisterDevice();/* 注册驱动 */
 	//GUI_API_SetPalette();

 	GUI_API_SetFixedMemony(g_guifixedmem,GUI_FIXEDMEM_SIZE);

	WDT_MNU_API_Register();
	WDT_TBX_API_Register();
	WDT_LBX_API_Register();
	WDT_SBR_API_Register();
	WDT_LVW_API_Register();
	WDT_TAB_API_Register();
	WDT_SCR_API_Register();
	WDT_VOI_API_Register();
	WDT_TIM_API_Register();
	WDT_CHN_API_Register();
	WDT_CBX_API_Register();
	WDT_STA_API_Register();
	WDT_EDT_API_Register();
	WDT_VFM_API_Register();
	WDT_CLD_API_Register();
	WDT_ODC_API_Register();
	WDT_LBP_API_Register();
	WDT_PBR_API_Register();
    WDT_MTX_API_Register();
	WDT_JPG_API_Register();
	WDT_GIF_API_Register();
	
	resource.styleDataAddr = STYLE_DATA_ADDR;
	resource.styleTableAddr = STYLE_TABLE_ADDR;
#ifdef APP_CONFIG_LANGUAGE_ENGLISH
	resource.dictionaryDataAddr = DicDataEnglish;
#else
	resource.dictionaryDataAddr = DicData;
#endif
	resource.keyTable = KEY_TABLE;
	resource.keyTableNum = KEY_TABLE_NUM;
	resource.osdTable = OSD_TABLE;
	resource.osdTableNum = OSD_TABLE_NUM;
	resource.stateTable = STATE_TABLE;
	resource.stateTableNum = STATE_TABLE_NUM;
	resource.focusTable = FOCUS_TABLE;
	resource.focusTableNum = FOCUS_TABLE_NUM;
	resource.eventTable = EVENT_TABLE;
	resource.eventTableNum = EVENT_TABLE_NUM;
	GUI_INT_API_RegisterResource(&resource);
	OSD_INIT_ADDR();
	
	APP_COM_IPI_InitAudio();

	FYF_API_descr_open ();
	


	APP_IPI_OpenE2Handle();
	APP_SYS_IPI_ReadSysInfo();
	
#if APP_CONFIG_LOGO_UPDATE
	if (APP_ADV_IPI_GetAdvData())
	{
		APP_ADV_API_DisPlayStartLogoGif();
		
	}
#endif
	semaphoreEcm = FYF_API_semaphore_create ("ecm", 1, FYF_THREAD_WAIT_FIFO);

#if defined(USE_FZVOD) || defined(USE_ENREACH)
	ipanel_update_startpagebuf();
#endif
	TPOWVOD_SetStbid();
	
#ifndef NO_CA
	FYF_API_CaInit();
#endif
	APP_API_Init();

    //GUI_ENC_API_SwitchState(OSD_INIT_PAGE,true);
	APP_EPG_API_SetReserveCallBack(UI_APP_TimeOut_Callback);
    APP_Com_API_SetSignalNotifyFun(UI_APP_SignalNotify);
#ifdef ENABLE_PVR
	pvr_CallBackParam.CallBack_RecDiskFull = UI_APP_RecDiskFull_CallBack;
	pvr_CallBackParam.CallBack_RecError = UI_APP_RecError_CallBack;
	pvr_CallBackParam.CallBack_PlayEndOfFile = UI_APP_PlayEndOfFile_CallBack;
	pvr_CallBackParam.CallBack_PlayStartOfFile = UI_APP_PlayStartOfFile_CallBack;
	pvr_CallBackParam.CallBack_PlayError = UI_APP_PlayError_CallBack;
	pvr_CallBackParam.CallBack_PlayReachRec = UI_APP_PlayReachRec_CallBack;
	//pvr_CallBackParam.CallBack_RecOverFixSize =;
	//pvr_CallBackParam.CallBack_RecReachPlay =;
	APP_PVR_API_RegisterEventCallBack(&pvr_CallBackParam);
#endif
	//GUI_ENC_API_SwitchState(ID_DLG_MainMenu, 1);

	DHCPInit();


#ifdef USE_FZVOD
		bHaveDL = FYF_TRUE; /*hqx:091020*/
		RegionID = 0x7801;
		//Ipanel_DLRegionId(); 
#endif
	IPI_SetFirstStateAndInit();
	FYF_API_thread_create("tSignal", ThreadSignalDetect, FYF_NULL, 13, 20 * 1024);
	//APP_Com_API_Printf("gui init success!\n");
	GUI_API_SetKeyReceiveMode(KEY_RECEIVE_OPEN);
	g_mainTask_end = FYF_TRUE;
	GUI_API_ProcessMsg();

}

BU32 IsMainTaskEnd(void)
{
	return g_mainTask_end;
}

void UI_APP_SetNitVerChangedUI ()
{
	
	APP_SYS_API_SetUpdateType(APP_SYS_OTA_UD_NULL);
	APP_Com_API_SetFullScreen();
	GUI_ENC_API_SetValue(ID_STA_Line1_AutoInfo, WDT_STA_SV_DATAID, RS_UpdateProg, 0);
    GUI_ENC_API_SwitchState(ID_DLG_AutoInfo,true);
}

void UI_APP_SetOTAUI ()
{

	OTA_Info_S otaInfo;
	BU08 otaRet = 0;
	// 无节目

	otaRet = APP_OTA_API_GetOTAType(&otaInfo);
	//APP_Com_API_Printf("~~~~~~~OTA TYPE %d\n", otaRet);
	if(otaRet == APP_SYS_OTA_UD_MANUAL)
	{
		APP_SYS_API_SetUpdateType(APP_SYS_OTA_UD_MANUAL);
		APP_Com_API_SetFullScreen();

		GUI_ENC_API_SetValue(ID_STA_Line1_AutoInfo, WDT_STA_SV_DATAID, RS_Message, 0);
		GUI_ENC_API_SetValue(ID_STA_Line2_AutoInfo, WDT_STA_SV_DATAID, RS_UpdateNow, 0);
		GUI_ENC_API_SetValue(ID_STA_Line3_AutoInfo, WDT_STA_SV_DATAID, RS_Ok_Cancel, 0);
		
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_FREQ_e, otaInfo.freqMHz);
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_SYMB, otaInfo.symKbps);
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_PID, otaInfo.downloadPid);
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_QAM, otaInfo.qam);
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_TYPE, APP_OTA_TYPE_AUTO);
		//APP_Com_API_Printf("~~~~~~~APP_SYS_OTA_UD_MANUAL\n");
	    GUI_ENC_API_SwitchState(ID_DLG_AutoInfo,true);
		
		
	}
	else if(otaRet == APP_SYS_OTA_UD_FORCE)
	{
		APP_SYS_API_SetUpdateType(APP_SYS_OTA_UD_FORCE);
		APP_Com_API_SetFullScreen();

		GUI_ENC_API_SetValue(ID_STA_Line1_AutoInfo, WDT_STA_SV_DATAID, RS_Message, 0);
		GUI_ENC_API_SetValue(ID_STA_Line2_AutoInfo, WDT_STA_SV_DATAID, RS_ForceUpdate, 0);
		GUI_ENC_API_SetValue(ID_STA_Line3_AutoInfo, WDT_STA_SV_DATAID, RS_Wait, 0);
		
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_FREQ_e, otaInfo.freqMHz);	
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_SYMB, otaInfo.symKbps);	
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_PID, otaInfo.downloadPid);	
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_QAM, otaInfo.qam);	
		APP_SYS_API_SetSysInfo(APP_SYS_OTA_TYPE, APP_OTA_TYPE_AUTO);	
		//APP_Com_API_Printf("~~~~~~APP_SYS_OTA_UD_FORCE\n");

		APP_SYS_API_SaveSysInfo();
		//APP_SYS_IPI_WriteStbInfo();
	    GUI_ENC_API_SwitchState(ID_DLG_AutoInfo,true);
		
	}
	else
	{
		APP_SYS_API_SetUpdateType(APP_SYS_OTA_UD_NULL);
		//APP_Com_API_Printf("~~~~~~~~~APP_SYS_OTA_UD_NULL\n");

	}
}

