#include "app_common.h"
#include "app_ipi_common.h"


#define ADD_BroadCast_FREQ 0

#define SEARCH_CAT_ENABLE (0)
extern BS08 g_StrSQL[APP_STR_SQL_LENGTH];
BS32 g_Frequency = 0;
BS32 g_SymbolRate = 0;
BS32 g_Modulation = 0;

BU08 isStopSearch = 1;
BU08 isEnableOta = 1;

BU32 searchProgCount = 0;//搜索到的节目数,超过一定数量则停止搜索，防止数据库大小超出
APP_Search_Param_S app_Search_Param_S;
static BU08 g_search_type = 0;
static BU32 g_search_main_freq = APP_MAIN_FREQ_MHZ;

BU08 APP_Search_API_GetSearchMainFreq()
{
	return g_search_main_freq;
}

BU08 APP_Search_API_GetSearchType()
{
	return g_search_type;
}
/*----------------------------------------------------------------------
Function Prototypes
----------------------------------------------------------------------*/
void APP_IPI_InitDbaV2(void);
//void APP_IPI_SetTagNeedReCreateChnList(void);
void APP_PMT_IPI_StopDemuxPmtPcrEcm(void);
BU08 APP_Search_IPI_GetPrintf(void);


void APP_IPI_SaveTunerInfo(BS32 Frequency, BS32 symbol_rate, BS32 Modulation)
{
	g_Frequency = Frequency;
	g_SymbolRate = symbol_rate;
	g_Modulation = Modulation;
}

BU32 APP_IPI_GetTunerFreq(void)
{
	return g_Frequency;
}

BU32 APP_IPI_GetTunerSym(void)
{
	return g_SymbolRate;
}

BU32 APP_IPI_GetTunerMod(void)
{
	return g_Modulation;
}

BU32 APP_IPI_Search_CheckParam(APP_Search_Param_SP paramP)
{
	if(paramP->m_StartFreqMHz < 51)return APP_FAIL;
	if(paramP->m_StopFreqMHz >865)return APP_FAIL;
	if(paramP->m_Callback_AddAChn == NULL)return APP_FAIL;
	if(paramP->m_Callback_EndOfSave== NULL)return APP_FAIL;
	if(paramP->m_Callback_UpdateFreqInfo== NULL)return APP_FAIL;
	if(paramP->m_Callback_UpdateProgress== NULL)return APP_FAIL;
	if(paramP->m_Callback_UpdateSignalInfo== NULL)return APP_FAIL;
	return APP_SUCCESS;
}

void APP_IPI_CallBackUpdateSignalInfo(BU08 bLock)
{
	if(app_Search_Param_S.m_Callback_UpdateSignalInfo != 0)
	{
		APP_Signal_BaseInfo_S signalBaseInfo;
		
		signalBaseInfo.m_BLock = bLock;
		FYF_API_tuner_ioctl(0, FYF_TUNER_GET_LEVEL, &signalBaseInfo.m_SignalLever);
		signalBaseInfo.m_SNR = APP_Signal_API_GetSignalSNR();
		signalBaseInfo.m_BER = (BU32)FYF_API_tuner_get_signal_ber(0);
		
		app_Search_Param_S.m_Callback_UpdateSignalInfo(&signalBaseInfo);
	}
}

/*-------------------------------------------------------------------------------

	Description:中断搜索

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Search_API_GetStopState(void)
{
	return isStopSearch;
}


BU08 APP_Search_IPI_LockFreq(BS32 Freq, BS32 SymKbps, BS32 Qam)
{
	FYF_QUEUE_MESSAGE msgRecv;
	BU32 tunerstate = FYF_TUNER_STATUS_FAIL;
	BU32 i = 0;
	APP_Search_printf("\nLOCK TUNER freqMHZ = %d, symKbps = %d, qam = %d\n", Freq, SymKbps, Qam);
	
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
	APP_IPI_SaveTunerInfo(Freq, SymKbps, Qam);
	FYF_API_tuner_lock_delivery(0, Freq * 1000, SymKbps, Qam, Freq);
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());	

	
	//APP_Printf("time1 = %d\n", FYF_API_time_ms());
	tunerstate = FYF_TUNER_STATUS_FAIL;
	for(i =0 ;i < 40 ;i++)
	{
		if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
		{
			tunerstate = FYF_TUNER_STATUS_SUCCESS;
			break;
		}
	#if 1
		if(APP_Search_API_GetStopState() == 1)
		{
			tunerstate = FYF_TUNER_STATUS_FAIL;
			break;				
		}
	#endif
		FYF_API_thread_sleep(50);
	}
	//APP_Printf("time2 = %d\n", FYF_API_time_ms());

	if (tunerstate == FYF_TUNER_STATUS_FAIL)
	{
		APP_PRINT_ERROR("DO NOT LOCK TUNER======================================================ERROR!\n");
		return 0;
	}
	return 1;
}

BU08 APP_Search_IPI_SearchPMT(void)
{
	BU32 pmtTableNum = 0;
	BU32 pmtTableDemuxNum = 0;
	BU32 pmtPID = 0;
	BU32 currentDemuxPmtTable = 0;//当前已经打开的PMT通道数量
	FYF_QUEUE_MESSAGE msgRecv;
	BU32 i = 0;
	BU32 a, b;
	struct pmtPIDInfo_s pmtPidInfo;
	
	APP_PMT_IPI_ResetPmtQueue();
	APP_PMT_IPI_StopAllDemuxPmt();

	pmtTableNum = DBA_API_GetPmtPidInfoCount();

	/*
	一次性打开多个通道滤取PMT
	当达到最大通道数量时等待一个通道滤取完毕,再打开下一个通道
	*/
	for (currentDemuxPmtTable = 0 ; currentDemuxPmtTable < pmtTableNum && currentDemuxPmtTable < MAX_START_DEMUX_PMT; currentDemuxPmtTable++)
	{
		DBA_API_GetPmtPidInfo(currentDemuxPmtTable, &pmtPidInfo);
		a = SeparateSevId(pmtPidInfo.SID)  ;
		b = pmtPidInfo.PID;
		pmtPID = (a << 16)  | b;
		APP_PMT_IPI_StartDemuxPmt(pmtPID);
		pmtTableDemuxNum++;
	}
	for (i = 0 ; i < pmtTableDemuxNum; i++)
	{
		msgRecv.qlstWordOfMsg = 0;
		FYF_API_queue_recv(APP_PMT_IPI_GetQueueParserPmtEnd(), &msgRecv, TIME_WAIT_FOR_PMT);
		if (msgRecv.qlstWordOfMsg == 0)
		{
			APP_Search_printf("Parser Pmt======================================================ERROR!\n");
			APP_PMT_IPI_StopAllDemuxPmt();
			return 0;
		}

		if(APP_Search_API_GetStopState() == 1)
		{
			APP_PMT_IPI_StopAllDemuxPmt();
			return 0;
		}

		/*
		如果还有未打开的通道，继续打开
		*/
		if(currentDemuxPmtTable < pmtTableNum)
		{

			DBA_API_GetPmtPidInfo(currentDemuxPmtTable, &pmtPidInfo);
			//pmtPID = (BU16)pmtPidInfo.PID;
			a = SeparateSevId(pmtPidInfo.SID)  ;
			b = pmtPidInfo.PID;
			pmtPID = (a << 16)  | b;
			SEARCH_DBG("demux new pmt Table pmtPID = %d\n", pmtPID);

			//if(pmtPID >= 0x0030 && pmtPID <= 0x1FFE)
			{
				APP_PMT_IPI_StartDemuxPmt(pmtPID);
				pmtTableDemuxNum++;
			}

			currentDemuxPmtTable++;
		}
	}

	return 1;
}

void APP_IPI_SearchProgInFreq(BS32 freqMHz, BS32 symKbps, BS32 qam)
{	
	FYF_QUEUE_MESSAGE msgRecv;
	
	
	/*
	锁频点
	*/
	if(0 == APP_Search_IPI_LockFreq(freqMHz, symKbps, qam))
	{
		APP_IPI_CallBackUpdateSignalInfo(0);
		return;
	}

	APP_IPI_CallBackUpdateSignalInfo(1);

	/*	搜索PAT	*/
	APP_PAT_IPI_StartDemuxPat();
	/*	搜索SDT	*/

	APP_SDT_IPI_StartDemuxSdt();
	APP_Search_printf("---111---   %d ms\n",FYF_API_time_ms());
	
#if SEARCH_CAT_ENABLE == 1
	/*	搜索CAT	*/

	APP_Cat_IPI_StartDemuxCat(APP_Cat_IPI_ParCatOnSearchProg);
	APP_Search_printf("---222----\n");
#endif	
	/*	等待PAT结束	*/
	msgRecv.qlstWordOfMsg = 0;

	
	FYF_API_queue_recv(APP_PAT_IPI_GetQueueParserPatEnd(), &msgRecv, TIME_WAIT_FOR_PAT);
	APP_Search_printf("---333---\n");
	if (msgRecv.qlstWordOfMsg == 0)
	{
		APP_Search_printf("Parser Pat======================================================ERROR %d ms!\n",FYF_API_time_ms());
		APP_PAT_IPI_StopDemuxPat();


		return;
	}
	APP_Search_printf("---444---\n");
	/*	等待SDT结束	*/
	msgRecv.qlstWordOfMsg = 0;

	FYF_API_queue_recv(APP_SDT_IPI_GetQueueParserSdtEnd(), &msgRecv, TIME_WAIT_FOR_SDT);

	if (msgRecv.qlstWordOfMsg == 0)
	{
		APP_Search_printf("Parser Sdt======================================================ERROR  %d ms!\n",FYF_API_time_ms());
		APP_SDT_IPI_StopDemuxSdt();
	}
#if SEARCH_CAT_ENABLE == 1
	/*	等待CAT结束*/
	msgRecv.qlstWordOfMsg = 0;
	FYF_API_queue_recv(APP_Cat_IPI_GetQueueParserCatEnd(), &msgRecv, TIME_WAIT_FOR_CAT);
	if (msgRecv.qlstWordOfMsg == 0)
	{
		APP_Search_printf("Parser Cat======================================================ERROR!\n");
		APP_Cat_IPI_StopDemuxCat();
	}
#endif
	APP_Search_printf("---555---\n");
	/*	搜索PMT	*/
	if(0 == APP_Search_IPI_SearchPMT())
	{
		return;
	}

}

void APP_IPI_ManualSearch(APP_Search_Param_SP paramP)
{
	APP_Tuner_FreqInfo_S freqInfo;
	BS32 startFreq;
	BS32 stopFreq;
	BU32 freqNum = 0;
	BS32 tempStartFreq;
	BU32 no = 0;
	
	startFreq = paramP->m_StartFreqMHz;
	stopFreq = paramP->m_StopFreqMHz;
	APP_Search_printf("\n\nstartFreq = %d, stopFreq = %d\n", startFreq, stopFreq);
	if (startFreq < 51)
	{
		startFreq = 51;
	}

	if (stopFreq > 865)
	{
		stopFreq  = 865;
	}
	
	if(startFreq > stopFreq)
	{
		 stopFreq = startFreq;
	}
	tempStartFreq = startFreq;

	/*搜索单独频点*/
	if(startFreq == stopFreq)
	{

		freqInfo.m_FreqMHz = startFreq;
		freqInfo.m_SymKbps = paramP->m_SymKbps;
		freqInfo.m_QAM = paramP->m_QamMode;

		if (paramP->m_Callback_UpdateFreqInfo != 0)
		{
			paramP->m_Callback_UpdateFreqInfo(&freqInfo, 1, 1);
		}
#if APP_CONFIG_FORBID_VOD_FREQ
		if ((startFreq > 674 && startFreq <= 778) || (startFreq >= 802 && startFreq <= 858))
		{
			return;
		}
#endif
		paramP->m_Callback_UpdateProgress(50);
		APP_IPI_SearchProgInFreq( startFreq,  paramP->m_SymKbps,  paramP->m_QamMode);
		return;
	}

	freqNum  = (stopFreq + 1 - startFreq) / 8 + 1;
	if(freqNum == 0)
	{
		freqNum = 1;
	}
	for(no = 1; startFreq <= stopFreq; startFreq += 8, no++)
	{
		if(APP_Search_API_GetStopState() == 1)
		{
			break;
		}
		if (startFreq >= 51 && startFreq < 474)
		{
			startFreq = startFreq - (startFreq -51) % 8;
		//	no = ((startFreq - tempStartFreq) / 8) + 1;
		}
		else if (startFreq >= 474 && startFreq < 859)
		{
			startFreq = startFreq - (startFreq -474) % 8;
		//	no = (467 - tempStartFreq) / 8 + 1 + (startFreq - 474) / 8 + 1;
		}

		freqInfo.m_FreqMHz = startFreq;
		freqInfo.m_SymKbps = paramP->m_SymKbps;
		freqInfo.m_QAM = paramP->m_QamMode;

		if (paramP->m_Callback_UpdateFreqInfo != 0)
		{
			//no = ((startFreq - tempStartFreq) / 8) ? ((startFreq - tempStartFreq) / 8) : 1;
			paramP->m_Callback_UpdateFreqInfo(&freqInfo, no, freqNum);
		}
#if APP_CONFIG_FORBID_VOD_FREQ
		if ((startFreq > 674 && startFreq <= 778) || (startFreq >= 802 && startFreq <= 858))
		{
		}
		else
#endif
		{
			APP_IPI_SearchProgInFreq( startFreq,  paramP->m_SymKbps,  paramP->m_QamMode);
		}
		


		if (paramP->m_Callback_UpdateProgress != 0)
		{
			BU08 process = (BU08)(99*((startFreq - paramP->m_StartFreqMHz) / 8 + 1)/freqNum);
			if(process >= 100)
			{	
				process = 99;
			}
			paramP->m_Callback_UpdateProgress(process);
		}
	}
}

BU08 APP_Search_IPI_SearchNit(void)
{
	FYF_QUEUE_MESSAGE msgRecv;
	
	APP_NIT_IPI_StartDemuxNit(APP_NIT_IPI_StartParserNit);
	msgRecv.qlstWordOfMsg = 0;
	if(FYF_ERR == FYF_API_queue_recv(APP_NIT_IPI_GetQueueParserNitEnd(), &msgRecv, TIME_WAIT_FOR_NIT))
	{
		APP_Search_printf("Parser Nit======================================================ERROR!\n");
		APP_NIT_IPI_StopDemuxNit();
		return 0;
	}
	return 1;
}

BU08 APP_Search_IPI_SearchNitOta(void)
{
	FYF_QUEUE_MESSAGE msgRecv;

//	APP_Printf(" APP_Search_IPI_SearchNitVer  ========== \n");

	APP_NIT_IPI_StartDemuxNitOta(APP_NIT_IPI_StartParserNitGetVerAndOTA);
	
	msgRecv.qlstWordOfMsg = 0;
	FYF_API_queue_recv(APP_NIT_IPI_GetQueueParserNitEnd(), &msgRecv, TIME_WAIT_FOR_NIT);
	if (msgRecv.qlstWordOfMsg == 0)
	{
//		app_print_error("Parser Nit======================================================ERROR!\n");
		APP_NIT_IPI_StopDemuxNit();
	}
}
BU08 APP_IPI_SearchWithNit(APP_Search_Param_SP paramP, BS32 mainFreq)
{
	APP_Tuner_FreqInfo_S freqInfo;
	BU32 nitFreqNO = 0,nitFreqNum = 0;
	struct nitFreqInfo_s nitFreqInfo;
	
		/*回调刷新频率*/
	freqInfo.m_FreqMHz = mainFreq;
	freqInfo.m_SymKbps = paramP->m_SymKbps;
	freqInfo.m_QAM = paramP->m_QamMode;
#ifndef APP_CONFIG_MAIN_FREQ_BACK
	if (paramP->m_Callback_UpdateFreqInfo != 0)
	{
		paramP->m_Callback_UpdateFreqInfo(&freqInfo,1,1);
	}
#endif

	/*
	锁定主频点
	*/
	if(0 == APP_Search_IPI_LockFreq(mainFreq,paramP->m_SymKbps,paramP->m_QamMode))
	{
		return 0;
	}
	
	/*
	搜索NIT
	*/
	if(0 == APP_Search_IPI_SearchNit())
	{
		//FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
		return 0;
	}
	else
	{
		#if ADD_BroadCast_FREQ
			DBA_API_AddNitFreqInfo(323, 6875, 2);
		#endif
	}
	/*
	根据NIT表逐个搜索频点
	*/

	DBA_API_SortNitFreqInfo();
	
	APP_IPI_SearchProgInFreq(mainFreq,paramP->m_SymKbps,paramP->m_QamMode);
	
	nitFreqNum = DBA_API_GetNitFreqInfoCount();
	while(nitFreqNO < nitFreqNum && (APP_Search_API_GetStopState() != 1))
	{
		BU16 freqMHz,symKbps;
		APP_CHN_QAM_E qam;

		if (paramP->m_Callback_UpdateProgress != 0)
		{
			BU08 process = (BU08)(99*(nitFreqNO+1)/nitFreqNum);
			if(process >= 100)
			{	
				process = 99;
			}
			paramP->m_Callback_UpdateProgress(process);
		}

		DBA_API_GetNitFreqInfo(nitFreqNO, &nitFreqInfo);
		if(nitFreqInfo.freq == 0) //lxp 获取nitFreqInfo失败
		{
			APP_PRINT_ERROR("DBA_API_GetNitFreqInfo() error!\n");
			continue;
		}
			
		nitFreqNO++;
		freqMHz = nitFreqInfo.freq;
		symKbps = nitFreqInfo.sym;
		qam = nitFreqInfo.qam;

		if(freqMHz == mainFreq)
			continue;
			
		freqInfo.m_FreqMHz = freqMHz;//APP_IPI_GetTunerFreq();
		freqInfo.m_SymKbps = symKbps;//APP_IPI_GetTunerSym();
		freqInfo.m_QAM = qam;//APP_IPI_GetTunerMod();
		if (paramP->m_Callback_UpdateFreqInfo != 0)
		{
			paramP->m_Callback_UpdateFreqInfo(&freqInfo,nitFreqNO,nitFreqNum);
		}
			
		APP_IPI_SearchProgInFreq( freqMHz,  symKbps,  qam);
		

		
	}//while(nitFreqNO < nitFreqNum)
	return 1;
}


/*-------------------------------------------------------------------------------

	Description:中断搜索

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Search_API_SetStartState(void)
{
	isStopSearch = 0;
}


/*
将数据库中的节目编号按照顺序重新编号
*/
void APP_IPI_ReSetProgIndex(void)
{
	BU32 serviceId = 0;
	BU32 i = 0;
	BU32 iCount = 0;
	
	SQL_API_Semaphore_Wait();
	SQL_API_Select("SELECT service_id FROM program ORDER BY service_id;");
	iCount = SQL_API_GetTotalNum();
	for (i = 0;i < iCount; i++)
	{
		SQL_API_GotoRow(i);
		serviceId = SQL_API_GetData(0,0,0,APP_DATA_TYPE_NUM);
		sprintf(g_StrSQL, "UPDATE program SET 编号 = %d WHERE service_id = %d;", i, serviceId);
		SQL_API_Exec(g_StrSQL);
	}
	SQL_API_Semaphore_Release();
}

void APP_Search_IPI_Search(void)
{
	BU32 startTime = 0;
	FYF_QUEUE_MESSAGE msgRev;
	APP_Tuner_FreqInfo_S freqInfo;
	struct nitFreqInfo_s nitFreqInfo;
	
	/*
	自动搜索
	*/
	startTime = FYF_API_time_ms();

	if(app_Search_Param_S.m_SrchType == APP_SEARCH_TYPE_AUTO_e)
	{
		g_search_type = APP_SEARCH_TYPE_AUTO_e;
		app_Search_Param_S.m_SymKbps = 6875;
		app_Search_Param_S.m_QamMode = 2;
	#ifdef APP_CONFIG_MAIN_FREQ_BACK
		/*回调刷新频率*/
			
		freqInfo.m_FreqMHz = APP_SYS_IPI_GetMainFreq();
		freqInfo.m_SymKbps = app_Search_Param_S.m_SymKbps;
		freqInfo.m_QAM = app_Search_Param_S.m_QamMode;
		if (app_Search_Param_S.m_Callback_UpdateFreqInfo != 0)
		{
			app_Search_Param_S.m_Callback_UpdateFreqInfo(&freqInfo,1,2);
		}
	#endif
		g_search_main_freq = APP_SYS_IPI_GetMainFreq();
		if (0 == APP_IPI_SearchWithNit(&app_Search_Param_S, APP_SYS_IPI_GetMainFreq()))
		{
	#ifdef APP_CONFIG_MAIN_FREQ_BACK
			app_Search_Param_S.m_Callback_UpdateProgress(50);
			freqInfo.m_FreqMHz = APP_MAIN_FREQ_MHZ_BCK;
			freqInfo.m_SymKbps = app_Search_Param_S.m_SymKbps;
			freqInfo.m_QAM = app_Search_Param_S.m_QamMode;
			if (app_Search_Param_S.m_Callback_UpdateFreqInfo != 0)
			{
				app_Search_Param_S.m_Callback_UpdateFreqInfo(&freqInfo,2,2);
			}
			
			if (APP_IPI_SearchWithNit(&app_Search_Param_S, APP_MAIN_FREQ_MHZ_BCK))
			{
				g_search_main_freq = APP_MAIN_FREQ_MHZ_BCK;
			}
	#endif
		}
		
	}

	/*
	全频段搜索
	*/
	if(app_Search_Param_S.m_SrchType == APP_SEARCH_TYPE_ALL_e)
	{
		app_Search_Param_S.m_StartFreqMHz = 51;
		app_Search_Param_S.m_StopFreqMHz = 859;
		app_Search_Param_S.m_SymKbps = 6875;
		app_Search_Param_S.m_QamMode = 2;
		g_search_type = APP_SEARCH_TYPE_ALL_e;
		APP_IPI_ManualSearch(&app_Search_Param_S);
	}

	/*
	手动搜索
	*/
	if(app_Search_Param_S.m_SrchType == APP_SEARCH_TYPE_MANUAL_e)
	{
		g_search_type = APP_SEARCH_TYPE_MANUAL_e;
		if(app_Search_Param_S.m_bNitOption == 1)//开启网络搜索
		{
		#ifndef APP_CONFIG_MAIN_FREQ_BACK
			APP_IPI_SearchWithNit(&app_Search_Param_S, app_Search_Param_S.m_StartFreqMHz);
		#else
			freqInfo.m_FreqMHz = app_Search_Param_S.m_StartFreqMHz;
			freqInfo.m_SymKbps = app_Search_Param_S.m_SymKbps;
			freqInfo.m_QAM = app_Search_Param_S.m_QamMode;
			if (app_Search_Param_S.m_Callback_UpdateFreqInfo != 0)
			{
				app_Search_Param_S.m_Callback_UpdateFreqInfo(&freqInfo,1,1);
			}
			APP_IPI_SearchWithNit(&app_Search_Param_S, app_Search_Param_S.m_StartFreqMHz);
		#endif
		}
		else
		{
			APP_IPI_ManualSearch(&app_Search_Param_S);//关闭网络搜索
		}
	}

//	if(app_Search_Param_S.m_SrchType != APP_SEARCH_TYPE_MANUAL_e)
//		APP_IPI_ReSetProgIndex();

	APP_IPI_InitDbaV2();


	if (app_Search_Param_S.m_Callback_UpdateProgress != 0)
	{
		app_Search_Param_S.m_Callback_UpdateProgress(100);
	}

	APP_EPG_API_SetRemindReserve(1);
	APP_Search_printf("SEARCH PROGRAM END OK! cost time = %d\n", FYF_API_time_ms() - startTime);
	APP_Search_API_HaltSearch();

}

BU32 APP_Search_Set_OTA_Status(BU32 flag) 
{
	if(flag)
		isEnableOta = 1;
	else 
		isEnableOta = 0;
}

BU32 APP_Search_Get_OTA_Status()
{
	if(isEnableOta)
		return 1;
	else
		return 0;
}


/*-------------------------------------------------------------------------------

	Description:开始搜索节目

	Parameters:

-------------------------------------------------------------------------------*/
BU32 APP_Search_API_StartSearch(APP_Search_Param_SP paramP)
{	
	static FYF_QUEUE_MESSAGE msgSend;

//	ota_nit_monitor_disable();
	APP_Com_API_Stop_Signal_Dectect();

	SQL_API_Semaphore_Wait();
	SQL_API_Select("SELECT service_id FROM program;");
	searchProgCount = SQL_API_GetTotalNum();
	SQL_API_Semaphore_Release();
	
	APP_EPG_API_SetRemindReserve(0);
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreEit(), FYF_WAIT_FOREVER);
	APP_EIT_IPI_StopDemuxEit();
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreEit());
//	APP_PMT_IPI_StopDemuxPmtPcrEcm();
//	APP_IPI_SetTagNeedReCreateChnList();
	APP_Search_API_SetStartState();
	APP_Search_Set_OTA_Status(0);
	APP_SDT_IPI_StopDemuxSdtOnTime();
	APP_PAT_IPI_StopDemuxPat();
	APP_PMT_IPI_StopDemuxPmtPcrEcm();
	APP_NIT_IPI_StopDemuxNit();

	GUI_API_Memcpy(&app_Search_Param_S, paramP, sizeof(APP_Search_Param_S));

	msgSend.qlstWordOfMsg = APP_Queue_Search_e;
	FYF_API_queue_send(APP_IPI_GetQueueCommon(), &msgSend);
	
	return APP_SUCCESS;
}

/*-------------------------------------------------------------------------------

	Description:中断搜索

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Search_API_HaltSearch(void)
{
	isStopSearch = 1;
}

/*-------------------------------------------------------------------------------

	Description:

	Parameters:

-------------------------------------------------------------------------------*/
void APP_Search_API_StartSave(void)
{
	SQL_API_SaveToFlash("flash.db");
	app_Search_Param_S.m_Callback_EndOfSave();
//	ota_nit_monitor_enable(6/APP_NIT_MONITOR_SLEEP_TIME_SEC);
}

void APP_Search_API_SearchSortInfo(void)
{
	BU32 startBatTime = 0;
	BU32 i = 0;
	BU32 tunerstate = FYF_TUNER_STATUS_FAIL;
	FYF_QUEUE_MESSAGE msgRecv;
	
	APP_EPG_API_SetRemindReserve(0);
	
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
	APP_IPI_SaveTunerInfo(g_search_main_freq, 6875, 2);
	FYF_API_tuner_lock_delivery(0, g_search_main_freq * 1000, 6875, 2, g_search_main_freq);
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());
	

	tunerstate = FYF_TUNER_STATUS_FAIL;
	for(i =0 ;i < 40 ;i++)
	{
		if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
		{
			tunerstate = FYF_TUNER_STATUS_SUCCESS;
			break;
		}

		FYF_API_thread_sleep(50);
	}
	

	

	if (tunerstate == FYF_TUNER_STATUS_FAIL)
	{
		APP_Search_printf("DO NOT LOCK TUNER======================================================ERROR!\n");
	}
	else
	{
		startBatTime = FYF_API_time_ms();
		msgRecv.qlstWordOfMsg = 0;
		APP_BAT_IPI_StartDemuxBat();
		FYF_API_queue_recv(APP_BAT_IPI_GetQueueParserBatEnd(), &msgRecv, 8000);
		if(msgRecv.qlstWordOfMsg == 0)
		{
			APP_BAT_IPI_StopDemuxBat();
		}
		APP_BAT_IPI_SaveBatInfoIntoDbs();
		APP_BAT_IPI_GetBatInfoFromDbs();
		APP_Printf("msgRecv.qlstWordOfMsg = %d, Bat cost time = %d\n", msgRecv.qlstWordOfMsg, FYF_API_time_ms() - startBatTime);
	}
//	APP_IPI_SetTagNeedReCreateChnList();
	APP_EPG_API_SetRemindReserve(1);
}

BU32 APP_Search_API_DetectOTA()
{	
	static FYF_QUEUE_MESSAGE msgSend;

	if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
	{
		msgSend.qlstWordOfMsg = APP_Queue_DetectOTA;
		FYF_API_queue_send(APP_IPI_GetQueueCommon(), &msgSend);
	}
	else
	{
		APP_PRINT_INFO(" tuner is not locked  ===== \n");	
	}
}



