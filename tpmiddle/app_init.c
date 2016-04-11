#include "app_common.h"
#include "app_ipi_common.h"


typedef struct _CA_REQID_S
{
	BU32    u32ChannelID;
	BU32 	u32FilterID;
	BU16	u16tspid;
	BU16	u16reqid;
	BU16	u8runflag;
	BU16	u8callbackflag;
	BU32	u32waittime;
	BU32	u32tick;
} CA_REQID_S;

extern BS32 e2h_newVersion;
extern BS32 e2h_testE2;
/*
extern BU32 g_Year;
extern BU32 g_Month;
extern BU32 g_Date;
extern BU32 g_Hour;
extern BU32 g_Minute;
extern BU32 g_Sec;*/

BU32 g_Year = 0;
BU32 g_Month = 0;
BU32 g_Date = 0;
BU32 g_Hour = 8;
BU32 g_Minute = 0;
BU32 g_Sec = 0;


extern BS08             g_StrSQL[APP_STR_SQL_LENGTH];

extern PMT_Channel_s	pmtChannels[MAX_DEMUX_PMT_CHANNEL];

extern CA_REQID_S  CAReqID[20];


extern APP_BOOT_INFO    fyf_boot_info;

BU32					semaphoreDemux = 0;
static BU32				semaphoreLockTuner = 0;
static BU32				semaphoreEit = 0;
static BU32				semaphoreParserFuncList = 0;
	   BU32				semaphoreEPG = 0;
	   BU32             semaphoreChnDatabase = 0;
	   BU32 			semaphoreUSB = 0;
	   BU32             semaphoreMp3 = 0;
       BU32             semaphoreNvod = 0;
	   
static BU32				queueLockState = 0;
static BU32				queueCommon = 0;
static BU32				queuePlayChn = 0;
static BU32 			g_queue_ipanel = 0;

static BU08				isHasNewChn = 0;
OTA_Info_S				g_OtaInfo = {0,0,0,0,0,0,0,0,0,0};
extern BU32 semaphoreEcm;
extern BU32 semaphoreSQL;
extern BU32 g_play_times_mutex;
extern BU32 g_play_timeshift_mutex;

BU32 stopParserTable = 0;

#define FYF_FLS_LOGO_DATA_SIZE      (1024*64)
extern BU08 OldLogoVersionNO;
extern BU32 OldLogoFileTotalLength ;
BU08 *data_logo_bufTmp = NULL;
//#ifdef SYS_CONFIG_UPDATE_STARTLOGO
//BU08 data_logo_buf[1024*64] = {0};
//#endif
/*----------------------------------------------------------------------
Function Prototypes
----------------------------------------------------------------------*/
void APP_IPI_OpenE2Handle(void);
void APP_BAT_IPI_GetBatInfoFromDbs(void);
void ThreadPlayChn(void * param);
void USBDetectThread();
APP_ParserFunc APP_IPI_GetParserFunc(BU32 channelId, BU32 filterId);
void APP_Search_IPI_Search(void);
void DBA_IPI_Init(void);
void DBA_IPI_InitEpgInfoNode(void);


void APP_IPI_SetHasNewChnFlg(void)
{
	isHasNewChn = 1;
}

void APP_IPI_SetHasNewChn(BU08 version)
{
	FYF_API_printf("APP_IPI_SetHasNewChn(void)\n");
	APP_SYS_IPI_SetNitVer(version);
	isHasNewChn = 1;
//	APP_SYS_API_NotifyNitUpdated();
}

/*-------------------------------------------------------------------------------

	Description:获取OTA升级类型

	Parameters:

-------------------------------------------------------------------------------*/
BU08 APP_OTA_API_GetOTAType(OTA_Info_S *pOtaInfo)
{
	BU32 sNo = 0;

	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &sNo);
	memcpy(pOtaInfo, &g_OtaInfo, sizeof(OTA_Info_S));
	if (g_OtaInfo.hardwareVersion == APP_SYS_HARD_VERSION
		&& g_OtaInfo.softwareVersion > APP_SYS_SOFT_VERSION
		&& ((g_OtaInfo.startSerialNo <= sNo && sNo <= g_OtaInfo.endSerialNo)||((g_OtaInfo.startSerialNo == 0x0) && (0x0 == g_OtaInfo.endSerialNo))))
	{
		if (g_OtaInfo.downloadType == 1)
			return APP_SYS_OTA_UD_MANUAL;
		else if (g_OtaInfo.downloadType == 2)
			return APP_SYS_OTA_UD_FORCE;	
	}
	return APP_SYS_OTA_UD_NULL;
}

/*-------------------------------------------------------------------------------

	Description:获取节目是否更新

	Parameters:

-------------------------------------------------------------------------------*/
BU08 APP_API_GetIsHasNewChn(void)
{
	return isHasNewChn;
}

/*-------------------------------------------------------------------------------

	Description:设置节目库已经更新

	Parameters:

-------------------------------------------------------------------------------*/
void APP_API_SetHadUpdateChn(void)
{
	if(app_system_data.nitVersion != 0)
	{
		FYF_API_printf("write nit version = 0x%X\n", app_system_data.nitVersion);
		APP_SYS_API_SaveSysInfo ();
	}
}

BU32 APP_IPI_GetQueueLockState(void)
{
	return queueLockState;
}

BU32 APP_IPI_GetQueueCommon(void)
{
	return queueCommon;
}

BU32 APP_IPI_GetQueuePlayChn(void)
{
	return queuePlayChn;
}

BU32 APP_IPI_GetQueueIpanel(void)
{
	return g_queue_ipanel;
}
BU32 APP_IPI_GetSemaphoreLockTuner(void)
{
	return semaphoreLockTuner;
}

BU32 APP_IPI_GetSemaphoreEit(void)
{
	return semaphoreEit;
}

BU32 APP_IPI_GetSemaphoreParserFuncList(void)
{
	return semaphoreParserFuncList;
}


#define SECTION_CALLBACK_NUM 5
typedef struct
{
	BU32 used;
	BU32 chanelID;
	BU32 filterID;
	FYF_DEMUX_NOTIFY_FUNC fun;
} sectionCallback_s;
sectionCallback_s sectionCallback[SECTION_CALLBACK_NUM];

void CallBackGetTableFunc(BU32 channelID, BU32 filterID, BU08* pSection, BS32 len)
{
	static FYF_QUEUE_MESSAGE msgRev;
	BU32 i = 0;
	BU32 j = 0;
	struct ParserFuncNode_s *pParFuncNode = 0;
	APP_ParserFunc ParserFunc;
#ifndef NO_CA
	for (i = 0; i < 20; i++)
	{
		if (CAReqID[i].u32ChannelID == channelID && CAReqID[i].u32FilterID == filterID)
		{	
			CA_SectionCallback(channelID, filterID, pSection, len);
			return;
		}
	}
#endif
	if(stopParserTable == 1)
	{
		return;
	}

	ParserFunc = APP_IPI_GetParserFunc(channelID, filterID);
	if(ParserFunc != 0)
	{
		(*ParserFunc)(pSection, channelID, filterID);
	}

	for(j=0;j<SECTION_CALLBACK_NUM;j++)
	{
		if(1 == sectionCallback[j].used && channelID == sectionCallback[j].chanelID&& filterID == sectionCallback[j].filterID)
		{
			break;
		}
	}

	if(SECTION_CALLBACK_NUM != j)
	{
		sectionCallback[j].fun(channelID,filterID,pSection,len);
	}

}

/*-------------------------------------------------------------------------------

	Description:

	Parameters:

-------------------------------------------------------------------------------*/
BS32 APP_API_RegisterSectionRequest(BU32 channelID,BU32 filterID,FYF_DEMUX_NOTIFY_FUNC fun)
{
	BU32 i;
	
	for(i=0;i<SECTION_CALLBACK_NUM;i++)
	{
		if(0 == sectionCallback[i].used)
		{
			sectionCallback[i].used = 1;
			break;;
		}
	}
	if(SECTION_CALLBACK_NUM == i)
	{
		return FYF_ERR;
	}
	else
	{
		sectionCallback[i].chanelID = channelID;
		sectionCallback[i].filterID = filterID;
		sectionCallback[i].fun = fun;
		return FYF_OK;
	}
}
void CallBackLockTuner(BU32 id, FYF_TUNER_STATUS status)
{
	static FYF_QUEUE_MESSAGE	msgSend;
	//APP_IPI_Semaphore_Wait_Demux();
	FYF_API_printf("-----------------LOCK_TUNER id = %d, status = %d-------------------\n", id, status);

	msgSend.qlstWordOfMsg = 1;
	msgSend.q2ndWordOfMsg = status;
	FYF_API_queue_send(APP_IPI_GetQueueLockState(), &msgSend);
//	APP_IPI_Semaphore_Release_Demux();

}

void APP_IPI_InitDbaV2(void)
{
	BU32 i = 0;
	struct progInfo_s progInfo;
	
	memset(&progInfo, 0, sizeof(struct progInfo_s));
	SQL_API_Semaphore_Wait();

	if(APP_FAIL == DBA_API_ClearProg())
	{
		APP_PRINT_ERROR("DBA_API_ClearProg() error!\n");	
	}
	
	DBA_IPI_InitEpgInfoNode();
 	
 	sprintf(g_StrSQL, "SELECT service_id FROM program ORDER BY 编号 ASC;");
 	SQL_API_Select(g_StrSQL);
 	for (i = 0; i < SQL_API_GetTotalNum(); i++)
	{
		SQL_API_GotoRow(i);
		progInfo.serviceId = SQL_API_GetData(0, 0, 0, APP_DATA_TYPE_NUM);
 		if( APP_FAIL == DBA_API_AddProg(progInfo))
 		{
			APP_PRINT_ERROR("DBA_API_AddProg() error !\n");
 		}
 	}
	SQL_API_Semaphore_Release();

}

void ThreadCommon(void * param)
{
	FYF_QUEUE_MESSAGE msgRev;
	while(1)
	{
		FYF_API_queue_recv(APP_IPI_GetQueueCommon(), &msgRev, FYF_WAIT_FOREVER);
	
		switch(msgRev.qlstWordOfMsg)
		{
		case APP_Queue_Search_e:
			APP_Search_IPI_Search();
			break;
		case APP_Queue_DetectOTA:
			APP_Search_IPI_SearchNitOta();
			break;
		default:
			break;
		}
	}
}
void APP_IPI_E2pTest()
{
	BU08 buf[] = "hello world!";
	BU08 readbuf[15];
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	fyf_e2p_ioctl_param.buffer = buf;
	fyf_e2p_ioctl_param.bufferSize = sizeof(buf);

	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_WRITE_EX, 2030, (BU32)&fyf_e2p_ioctl_param);
	memset(readbuf, 0, sizeof(readbuf));
	fyf_e2p_ioctl_param.buffer = readbuf;
	fyf_e2p_ioctl_param.bufferSize = sizeof(readbuf);
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_READ_EX, 2030, (BU32)&fyf_e2p_ioctl_param);

	APP_PRINT_INFO("readbuf = %s\n", readbuf);
	while(1);
	
}
/*-------------------------------------------------------------------------------

	Description:初始化中间件

	Parameters:

-------------------------------------------------------------------------------*/
void APP_API_Init(void)
{
	BU32	i = 0,j = 0;
	FYF_QUEUE_MESSAGE msgRev;
	BU32 testqueue;
	BU32 startTime = 0;
	BU32 tunerstate = FYF_TUNER_STATUS_FAIL;
	BU32 logo_len = 0;
	BU08 *logo_data = NULL;
	BU08 logo_version = 0xff;
	BU32 new_logo_len = 0;
	BU08 new_logo_version = 0xff;
	BU32 waittime = 0;
	BU32 logo_haveflag = 0;	
	FYF_API_printf("APP_API_Init()\n");


#if 0
	testqueue = FYF_API_queue_create("qPlayChn", 20, FYF_THREAD_WAIT_FIFO);
	startTime = FYF_API_time_ms();
	FYF_API_queue_recv(testqueue, &msgRev, FYF_NO_WAIT);
	APP_Printf("\n!!!!time1 = %d\n", FYF_API_time_ms() - startTime);
	for (i = 0; i < 100; i++)
	{
		FYF_API_thread_sleep(20);
	}
	APP_Printf("\n!!!!time1 = %d\n", FYF_API_time_ms() - startTime);
#endif

	startTime = FYF_API_time_ms();

	//APP_SYS_IPI_SetSysValueToDefault();
	

	for (i = 0; i < MAX_DEMUX_PMT_CHANNEL; i++)
	{
		pmtChannels[i].pmtPID = 0;
		pmtChannels[i].pmtChannelID = 0;
		pmtChannels[i].pmtFilterID = 0;
		pmtChannels[i].isStop = 0;
	}
	g_play_times_mutex        = FYF_API_mutex_create();
	g_play_timeshift_mutex    = FYF_API_mutex_create();
	APP_PRINT_DEBUG("g_play_times_mutex = %d\n", g_play_times_mutex);
	semaphoreDemux            = FYF_API_semaphore_create("sDem", 1, FYF_THREAD_WAIT_FIFO);
	semaphoreLockTuner        = FYF_API_semaphore_create("sLTuner", 1, FYF_THREAD_WAIT_FIFO);
	semaphoreEit		      = FYF_API_semaphore_create("sEit", 1, FYF_THREAD_WAIT_FIFO);
	semaphoreParserFuncList   = FYF_API_semaphore_create("sPFuncL", 1, FYF_THREAD_WAIT_FIFO);
	semaphoreEPG 			  = FYF_API_semaphore_create("sEPG", 1, FYF_THREAD_WAIT_FIFO);
	semaphoreChnDatabase = FYF_API_semaphore_create ("ChnDat", 1, FYF_THREAD_WAIT_FIFO);
	semaphoreUSB = FYF_API_semaphore_create("USB", 1, FYF_THREAD_WAIT_FIFO);
	semaphoreMp3 = FYF_API_semaphore_create("MP3", 1, FYF_THREAD_WAIT_FIFO);
	semaphoreNvod            = FYF_API_semaphore_create("sNvod", 1, FYF_THREAD_WAIT_FIFO); 

	queueLockState            = FYF_API_queue_create("qLockS", 100, FYF_THREAD_WAIT_FIFO);
	queueCommon				  = FYF_API_queue_create("qComm", 100, FYF_THREAD_WAIT_FIFO);
	queuePlayChn			  = FYF_API_queue_create("qPlayChn", 100, FYF_THREAD_WAIT_FIFO);
	g_queue_ipanel			  = FYF_API_queue_create("qIpanel", 20, FYF_THREAD_WAIT_FIFO);
	
	semaphoreSQL = FYF_API_semaphore_create("sSQL", 1, FYF_THREAD_WAIT_FIFO);


	DBA_IPI_Init();
#ifdef ENABLE_NVOD
	FYF_API_nvod_init ();
#endif
	APP_IPI_InitDbaV2();
	APP_BAT_IPI_GetBatInfoFromDbs();


	FYF_API_demux_set_notify(CallBackGetTableFunc);
	FYF_API_tuner_set_notify(CallBackLockTuner);

	FYF_API_thread_create("tComm", ThreadCommon, FYF_NULL, FYF_THREAD_PRIORITY_SEARCH, FYF_THREAD_STACK_SEARCH);	
	FYF_API_thread_create("tPlayChn", ThreadPlayChn, FYF_NULL, FYF_THREAD_PRIORITY_PLAYCHN, FYF_THREAD_STACK_PLAYCHN);	
#ifdef ENABLE_USB
	FYF_API_thread_create("USBDetect", USBDetectThread, FYF_NULL, FYF_THREAD_PRIORITY_CLOCK, FYF_THREAD_STACK_CLOCK);
#endif




	/*锁定主频点，搜索节目更新，OTA，时间等信息*/
	APP_Search_printf("LOCK TUNER freqMHZ = %d, symKbps = %d, qam = %d\n", APP_SYS_IPI_GetMainFreq(), 6875, 2);
	
	FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
	APP_IPI_SaveTunerInfo(APP_SYS_IPI_GetMainFreq(), 6875, 2);
	FYF_API_tuner_lock_delivery(0, APP_SYS_IPI_GetMainFreq() * 1000, 6875, 2, APP_SYS_IPI_GetMainFreq());
	FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());

	tunerstate = FYF_TUNER_STATUS_FAIL;
	for(i =0 ;i < 40 ;i++)
	{
		FYF_API_thread_sleep(50);
		if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
		{
			tunerstate = FYF_TUNER_STATUS_SUCCESS;
			break;
		}

	}
#ifdef APP_CONFIG_MAIN_FREQ_BACK
	if (tunerstate == FYF_TUNER_STATUS_FAIL)
	{
		FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
		APP_IPI_SaveTunerInfo(APP_MAIN_FREQ_MHZ_BCK, 6875, 2);
		FYF_API_tuner_lock_delivery(0, APP_MAIN_FREQ_MHZ_BCK * 1000, 6875, 2, APP_MAIN_FREQ_MHZ_BCK);
		FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());

		tunerstate = FYF_TUNER_STATUS_FAIL;
		for(i =0 ;i < 40 ;i++)
		{
			FYF_API_thread_sleep(50);
			if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
			{
				tunerstate = FYF_TUNER_STATUS_SUCCESS;
				break;
			}

		}
	}
	else
	{
		APP_NIT_IPI_StartDemuxNit(APP_NIT_IPI_StartParserNitGetVerAndOTA);
		
		msgRev.qlstWordOfMsg = 0;
		FYF_API_queue_recv(APP_NIT_IPI_GetQueueParserNitEnd(), &msgRev, TIME_WAIT_FOR_NIT);
		if (msgRev.qlstWordOfMsg == 0)
		{
			APP_Printf("Parser Nit version======================================================ERROR!\n");
			APP_NIT_IPI_StopDemuxNit();
			FYF_API_semaphore_wait(APP_IPI_GetSemaphoreLockTuner(), FYF_WAIT_FOREVER);
			APP_IPI_SaveTunerInfo(APP_MAIN_FREQ_MHZ_BCK, 6875, 2);
			FYF_API_tuner_lock_delivery(0, APP_MAIN_FREQ_MHZ_BCK * 1000, 6875, 2, APP_MAIN_FREQ_MHZ_BCK);
			FYF_API_semaphore_release(APP_IPI_GetSemaphoreLockTuner());

			tunerstate = FYF_TUNER_STATUS_FAIL;
			for(i =0 ;i < 40 ;i++)
			{
				FYF_API_thread_sleep(50);
				if(FYF_API_tuner_get_status(0) == FYF_TUNER_LOCKED)
				{
					tunerstate = FYF_TUNER_STATUS_SUCCESS;
					break;
				}

			}
			
		}
		APP_Printf("msgRev.qlstWordOfMsg = %d\n", msgRev.qlstWordOfMsg);
	}
#endif

	if (tunerstate == FYF_TUNER_STATUS_FAIL)
	{
		APP_Search_printf("DO NOT LOCK TUNER======================================================ERROR!\n");
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_FAV_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_FAV_e);
		for (i = APP_CHN_GROUP_TYPE_1_e; i < APP_CHN_GROUP_TYPE_26_e; i++)
		{
			APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, i);
		}
		APP_IPI_SetTime(2010, 1, 1, 5, 8, 0, 0);
	//	FYF_API_thread_sleep(2000);
	}
	else
	{
		APP_TDT_IPI_StartDemuxTdt();
	
#ifdef SYS_CONFIG_UPDATE_STARTLOGO

#endif
		for(i = 0; i < 100; i++)
		{
			if(g_Year != 0)
				{break;}
			FYF_API_thread_sleep(10);		
		}
		
		if(g_Year != 0)
		{
			APP_EPG_IPI_InitReserve();
			//add by cw 2009-12-05
			//开机第一次搜到tdt表时，马上校正操作系统时间
		//	FYF_API_OS_Set_SysTime(g_Year, g_Month, g_Date, g_Hour, g_Minute, g_Sec);
		}
		//APP_EIT_IPI_StartDemuxPFActualEit();
		APP_NIT_API_StartDemuxOTAInfo(APP_NIT_IPI_StartParseOTAInfo);
#if APP_CONFIG_LOGO_UPDATE
		APP_ADV_API_Main();
#endif
	

		/*Test and request BAT in synchronized here if and only if the total number of prog is non-zero*/
		SQL_API_Select("SELECT service_id FROM program;");
		if (SQL_API_GetTotalNum() != 0)
		{
			APP_BAT_IPI_StartDemuxBatVersion();
			msgRev.qlstWordOfMsg = 0;
			APP_PRINT_DEBUG("%d\n", FYF_API_queue_recv(APP_BAT_IPI_GetQueueParserBatEnd(), &msgRev, 1000));
			if (msgRev.qlstWordOfMsg == 0)
			{
				APP_BAT_IPI_StopDemuxBat();
				APP_PRINT_DEBUG("cat not bat update\n");
				
			}
			else
			{
				APP_BAT_IPI_BatBootupProc();
			}
		}
		
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_FAV_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);
		APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_FAV_e);
		for (i = APP_CHN_GROUP_TYPE_1_e; i < APP_CHN_GROUP_TYPE_26_e; i++)
		{
			APP_ChnDatabase_API_ReCreatChnList(APP_CHN_AV_TYPE_TV_e, i);
		}
		
		msgRev.qlstWordOfMsg = 0;
		FYF_API_queue_recv(APP_NIT_IPI_GetQueueParserOtaEnd(), &msgRev, TIME_WAIT_FOR_NIT - 3000);
		if (msgRev.qlstWordOfMsg == 0)
		{
			APP_Printf("Parser Nit ota error ======================================================ERROR!\n");
			APP_NIT_API_StopDemuxOTAInfo();
		}
		//由于三明nit描述符只存在于主频点中，所以开机就要进行节目更新与升级的检测
#if 1
	#ifdef CD_CA
	    if ((CDCA_API_FinishNitFilter() == FYF_TRUE))
	#endif
		{
			APP_NIT_IPI_StartDemuxNit(APP_NIT_IPI_StartParserNitGetVerAndOTA);
			
			msgRev.qlstWordOfMsg = 0;
			FYF_API_queue_recv(APP_NIT_IPI_GetQueueParserNitEnd(), &msgRev, TIME_WAIT_FOR_NIT);
			if (msgRev.qlstWordOfMsg == 0)
			{
				APP_Printf("Parser Nit version======================================================ERROR!\n");
				APP_NIT_IPI_StopDemuxNit();
			}
			APP_Printf("msgRev.qlstWordOfMsg = %d\n", msgRev.qlstWordOfMsg);
	    }
#endif
//		APP_TDT_IPI_StartDemuxTot();

	}
	
#ifdef SYS_CONFIG_UPDATE_STARTLOGO 

#endif	

	StartClockThread();

	FYF_API_printf("APP_API_Init() cost time = %d\n", FYF_API_time_ms() - startTime);

}


#include <stdio.h>
 
 
int main()
{
        int i = 0;
 
        printf("Hello world! %d\n",i);
}

text    data     bss     dec     hex filename
    893     256       4    1153     481 a.out


08049598 A __bss_start
080482e4 t call_gmon_start
08049598 b completed.1
0804949c d __CTOR_END__
08049498 d __CTOR_LIST__
0804958c D __data_start
0804958c W data_start
08048438 t __do_global_ctors_aux
08048308 t __do_global_dtors_aux
08049590 D __dso_handle
080494a4 d __DTOR_END__
080494a0 d __DTOR_LIST__
080494ac D _DYNAMIC
08049598 A _edata
0804959c A _end
0804845c T _fini
08049498 A __fini_array_end
08049498 A __fini_array_start
08048478 R _fp_hw
0804833c t frame_dummy
08048494 r __FRAME_END__
08049578 D _GLOBAL_OFFSET_TABLE_
         w __gmon_start__
08048278 T _init
08049498 A __init_array_end
08049498 A __init_array_start
0804847c R _IO_stdin_used
080494a8 d __JCR_END__
080494a8 d __JCR_LIST__
         w _Jv_RegisterClasses
080483f4 T __libc_csu_fini
080483a0 T __libc_csu_init
         U __libc_start_main@@GLIBC_2.0
08048368 T main
08049594 d p.0
08049498 A __preinit_array_end
08049498 A __preinit_array_start
         U printf@@GLIBC_2.0
080482c0 T _start



