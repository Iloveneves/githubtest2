/*===============================================================================

 File Name:    ca_dvt_flt_mng.c

 Description:

 History:      Created By Ryan Lin, DuWeiqin at 15th Oct 2007
 
               Modifiy By Ryan Lin           at  29th Oct 2007
               -To fix bug due to requst the same pid but differrnt ReqIds
               

===============================================================================*/
/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/

#include "CDCAS30.h"
#include "fyf_com.h"
#include "hi_stb_api.h"

#define DEBUG_CDCA //printf


/*----------------------------------------------------------------------
	Constant Define
----------------------------------------------------------------------*/

#define CAS_INVALID_REQ_ID	0
#define CAS_MAX_SEC_SIZE	1024

#define CD_ECM_REQUEST_ID	1 //which request id is used for cas lib
//#define DVT_EMM_REQUEST_ID	2 //which request id is used for cas lib

#define	CA_REQID_NUM		(20)


#define CAS_DMX_MUTEX_INIT()	if (cas_sema == 0) {\
			if ((cas_sema = FYF_API_semaphore_create("CAS_Semaphore", 1, FYF_THREAD_WAIT_FIFO))==0){\
			DEBUG_CDCA("create semaphore failure with %d\r", cas_sema); \
			}}
#define CAS_DMX_MUTEX_DEL()	if (cas_sema != 0) {\
			FYF_API_semaphore_destroy(cas_sema);\
			cas_sema = 0;}
#define CAS_DMX_MUTEX_LOCK()	if (cas_sema != 0) FYF_API_semaphore_wait(cas_sema, 1000);

#define CAS_DMX_MUTEX_UNLOCK()	if (cas_sema != 0) FYF_API_semaphore_release(cas_sema);



/*----------------------------------------------------------------------
	Define Struct Variable Types
----------------------------------------------------------------------*/



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


 CA_REQID_S  CAReqID[CA_REQID_NUM];

/*----------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------*/
static BU32 cas_sema = 0;

static BU32 cdca_finishNitFilter = FYF_FALSE;
BU32 CDCA_API_FinishNitFilter(void)
{
	return cdca_finishNitFilter;
}
/*----------------------------------------------------------------------
	Interface Function START
----------------------------------------------------------------------*/
/*=============================================================================
  
  Description:
 
  Paramenters:  @para 
 
  Return:           

==============================================================================*/

BU32 CA_Stop_Demux(BU32 *channelID, BU32 *filterID)
{
	BU32 channel_id, filter_id;
	BS32 returnValue;
	channel_id = *channelID; 
	filter_id = *filterID;
	//printf("______StopDemux() channelID = 0x%X, filterID = 0x%X\n", *channelID, *filterID);
	if (channel_id == 0 && filter_id == 0)
		return 1;
	APP_IPI_Semaphore_Wait_Demux();

	if((returnValue =  FYF_API_demux_disable_filter(channel_id, filter_id)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}

	if((returnValue = FYF_API_demux_destroy_filter(channel_id, filter_id)) !=0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}

	if((returnValue = FYF_API_demux_stop_channel(channel_id)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}

	if((returnValue = FYF_API_demux_destroy_channel(channel_id))!= 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}
	*channelID = 0;
	*filterID = 0;
	APP_IPI_Semaphore_Release_Demux();
	return 1;
}

BU32 CA_Start_Demux(BU32 wide,BU16 pID, BU08 coef[], BU08 mask[], BU08 excl[], BU32 *channelID, BU32 *filterID)
{
	BU32 returnValue = 0;
//	printf("	StartDemux pID =%d, coef = %d\n", pID, coef[0]);
	APP_IPI_Semaphore_Wait_Demux();	
	FYF_API_demux_create_channel(64 * 1024, FYF_DEMUX_DATA_PSI_CHANNEL, channelID, pID);
	
	if((returnValue = FYF_API_demux_set_channel_pid(*channelID, pID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}
	
	FYF_API_demux_create_filter(*channelID, filterID);
	
	if((returnValue = FYF_API_demux_set_filter(*channelID, *filterID, wide, coef, mask, excl))!= 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}

	if((returnValue = FYF_API_demux_enable_filter(*channelID, *filterID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}
	
	if((returnValue = FYF_API_demux_start_channel(*channelID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}
//	printf("______StartDemux() channelID = 0x%X, filterID = 0x%X\n", *channelID, *filterID);

	APP_IPI_Semaphore_Release_Demux();
	return 1;
}
  void cas_dmx_timeout_check()
{
	static BU32 count = 0;
	//printf("cas_dmx_timeout_check\n");
	BU32 i;
	BU32 times;
	BU16 pid;
    BU32 reqid;
//	CAS_DMX_MUTEX_LOCK();
	for (i = 0; i < CA_REQID_NUM; i++)
	{
		if (CAReqID[i].u8runflag == 1 && CAReqID[i].u32waittime != 1000*1000)
		{
			times = FYF_API_time_ms();
		/*	if (times < CAReqID[i].u32tick)
			{
				count ++;
			}
		*/
			if (times - CAReqID[i].u32tick > CAReqID[i].u32waittime)
			{
			    pid = CAReqID[i].u16tspid;
				reqid = CAReqID[i].u16reqid;
				if (i < 2 || i >= 10)
				{
					printf("times = %d %d %d %d\n", i, times, CAReqID[i].u32tick, CAReqID[i].u32waittime);
					if(pid == 0x10)
					cdca_finishNitFilter = FYF_TRUE;
					CA_Stop_Demux(&(CAReqID[i].u32ChannelID), &(CAReqID[i].u32FilterID));
					CAReqID[i].u8runflag = 0;
					
					CAReqID[i].u16reqid = 0x1fff;
					CAReqID[i].u16tspid = 0x1fff;
				}
				CDCASTB_PrivateDataGot(reqid, false, pid, NULL, 0 );	

			}
		}
	}
//	printf("count = %d\n", count);
//	CAS_DMX_MUTEX_UNLOCK();
}

BS08 CA_GetReID(BU16 pid)
{
	BU08 i;
	BU08 flag = 0;
	for (i = 10; i < CA_REQID_NUM; i++)
	{
		if (CAReqID[i].u8runflag && CAReqID[i].u16tspid == pid)
		{
			flag = 1;
			break;
		}
	}
	if (flag == 1)return i;
	else
	{
		for (i = 10; i < CA_REQID_NUM; i++)
			if (CAReqID[i].u8runflag == 0)
				return i;
	}
}

 void  CA_SectionCallback(BU32 ChannelID, BU32 FilterID ,BU08 *u8Buffer, BU32 u32BufferLength)
{
	BS32 i;
	BU16 pid;
	BU32 reqid;
//	CAS_DMX_MUTEX_LOCK();
	DEBUG_CDCA("CA_SectionCallback channelid = %d, filterid = %d\n", ChannelID, FilterID);
	BU08 sectionflag;

	for (i = 0; i < CA_REQID_NUM; i++)
	{
		if (CAReqID[i].u32ChannelID == ChannelID && CAReqID[i].u32FilterID == FilterID)
			break;
	}
	reqid = CAReqID[i].u16reqid;
	if (FYF_NULL == u8Buffer)
	{
		sectionflag = FYF_FALSE;
		u32BufferLength = 0;
	}
	else
		sectionflag = FYF_TRUE;
	if (i >= CA_REQID_NUM)
	{
		DEBUG_CDCA("CA_SectionCallback error\n");
//		CAS_DMX_MUTEX_UNLOCK();
		return;
	}

//ECM数据
/*	if ( i ==1 && CAReqID[i].u8callbackflag >1)
		return;
		*/
	
//EMM数据 长度小于16，丢弃
	if ( i >= 2 && i < 10 &&u32BufferLength <= 16)
	{
//		CAS_DMX_MUTEX_UNLOCK();
		return;
	}


	DEBUG_CDCA("\n +++ CA_SectionCallback i=%d datafilterid =%d\n", i, FilterID);
	pid = CAReqID[i].u16tspid;

	//ECM数据包，停止过滤，释放过滤器
	if (i < 2 || i >= 10)
	{
		if(pid == 0x10)
		cdca_finishNitFilter = FYF_TRUE;
		CA_Stop_Demux(&(CAReqID[i].u32ChannelID), &(CAReqID[i].u32FilterID));
		CAReqID[i].u8runflag = 0;
		CAReqID[i].u16reqid = 0x1fff;
		CAReqID[i].u16tspid = 0x1fff;
		
	}

	CDCASTB_PrivateDataGot(reqid, sectionflag, pid, u8Buffer, u32BufferLength );
//	CAS_DMX_MUTEX_UNLOCK();
	//CAReqID[i].u8runflag = 2;

}

static BS32 cas_demux_init()
{
	BU32 i;
	CAS_DMX_MUTEX_INIT();
	for (i = 0; i< CA_REQID_NUM; i++)
	{
		CAReqID[i].u32ChannelID = 0;
		CAReqID[i].u32FilterID = 0;
		CAReqID[i].u16reqid = 0x1fff;
		CAReqID[i].u16tspid = 0x1fff;
		CAReqID[i].u8runflag = 0;
		CAReqID[i].u8callbackflag = 0;
		CAReqID[i].u32tick = 0;
		CAReqID[i].u32waittime = 0;
	}
	FYF_API_demux_set_CA_notify(CA_SectionCallback);
	return FYF_OK;
}

void cas_demux_stopECMFilter()
{
	BU32 i = 0;
	for (i = 10; i < CA_REQID_NUM; i++)
	{
		if (CAReqID[i].u8runflag && CAReqID[i].u16tspid != 0x10)
		{
				CA_Stop_Demux(&(CAReqID[i].u32ChannelID), &(CAReqID[i].u32FilterID));
			 //   CDCASTB_PrivateDataGot(CAReqID[i].u16reqid,false,CAReqID[i].u16tspid, NULL,0); 
				CAReqID[i].u8runflag = 0;
				CAReqID[i].u16reqid = 0x1fff;
				CAReqID[i].u16tspid = 0x1fff;
				CAReqID[i].u8callbackflag = 0;
				CAReqID[i].u32tick = 0;
				CAReqID[i].u32waittime = 0;
		}
	}
}

void cas_demux_stopEMMFilter()
{
	BU32 i = 0;
	BU32 channel_id = 0;
	BU16 pid;
    BU32 reqid;

	CDCASTB_SetEmmPid(0);
	APP_IPI_Semaphore_Wait_Demux();
	for (i = 2; i < 10; i++)
	{
		if (CAReqID[i].u8runflag && CAReqID[i].u32ChannelID != 0 && CAReqID[i].u32FilterID != 0)
		{
				channel_id = CAReqID[i].u32ChannelID;
				pid = CAReqID[i].u16tspid;
				reqid = CAReqID[i].u16reqid;
				//CA_Stop_Demux(&(CAReqID[i].u32ChannelID), &(CAReqID[i].u32FilterID));
				FYF_API_demux_disable_filter(CAReqID[i].u32ChannelID, CAReqID[i].u32FilterID);
                FYF_API_demux_destroy_filter(CAReqID[i].u32ChannelID, CAReqID[i].u32FilterID);

				CAReqID[i].u8runflag = 0;
				CAReqID[i].u16reqid = 0x1fff;
				CAReqID[i].u16tspid = 0x1fff;
				CAReqID[i].u8callbackflag = 0;
				CAReqID[i].u32tick = 0;
				CAReqID[i].u32waittime = 0;
				CDCASTB_PrivateDataGot(reqid,false,pid, NULL,0); 
		}
	}
	if (channel_id != 0)
	{
		FYF_API_demux_stop_channel(channel_id);
		FYF_API_demux_destroy_channel(channel_id);
	}
	APP_IPI_Semaphore_Release_Demux();
}



/*=============================================================================
  
  Description:
 
  Paramenters:  @para 
 
  Return:           

==============================================================================*/



BS32 cas_demux_request(BU08 req_id, BU08* value, BU08* mask, BU08 mask_len, BU16 pid, BU08 wait_seconds)
{

	BU32 s32filterid, i;
	BU08 excl[8];
	BU32 index;

	//if(CAReqID[req_id].u8runflag == 1)
	//	return CDCA_FALSE;
	if((value==NULL) || (mask==NULL) || (mask_len==0))
	{
		DEBUG_CDCA("Err: %s- error param!\n",__FUNCTION__);
		return CDCA_FALSE;
	}

	for (i = 0; i < mask_len; i++)
	{
		excl[i] = 0;
	}
//	CAS_DMX_MUTEX_LOCK();
	if (req_id < 2)
	{
		index = CA_GetReID(pid);
		CA_Stop_Demux(&(CAReqID[index].u32ChannelID), &(CAReqID[index].u32FilterID));
		CA_Start_Demux(mask_len, pid, value, mask, excl, &(CAReqID[index].u32ChannelID), &(CAReqID[index].u32FilterID));
	}
	else
	{
		index = req_id;
#if 1
		if (CAReqID[req_id].u16tspid == 0x1fff)
		{
			//CA_Stop_Demux(&(CAReqID[req_id].u32ChannelID), &(CAReqID[req_id].u32FilterID));
			CA_Start_Demux(mask_len, pid, value, mask, excl, &(CAReqID[req_id].u32ChannelID), &(CAReqID[req_id].u32FilterID));
		}
		else
		{
			DEBUG_CDCA("CAReqID[req_id].u32ChannelID = %d CAReqID[req_id].u32FilterID = %d\n"
				,CAReqID[req_id].u32ChannelID,CAReqID[req_id].u32FilterID);
			if (CAReqID[req_id].u32ChannelID != 0 && CAReqID[req_id].u32FilterID != 0)
			{
			 #if 1
				APP_IPI_Semaphore_Wait_Demux();
				if (FYF_API_demux_disable_filter(CAReqID[req_id].u32ChannelID, CAReqID[req_id].u32FilterID) != 0)
				{

				}
				if(FYF_API_demux_destroy_filter(CAReqID[req_id].u32ChannelID, CAReqID[req_id].u32FilterID) !=0)
				{
	
				}
				CAReqID[req_id].u32ChannelID = 0;
				CAReqID[req_id].u32FilterID = 0;
				APP_IPI_Semaphore_Release_Demux();
			#endif

			}
		
			CA_Start_Demux(mask_len, pid, value, mask, excl, &(CAReqID[req_id].u32ChannelID), &(CAReqID[req_id].u32FilterID));
	
		}
#endif
	}

	DEBUG_CDCA ("(CAReqID[req_id].u32ChannelID= %d, filterid = %d\n", CAReqID[index].u32ChannelID, CAReqID[index].u32FilterID);
	CAReqID[index].u16reqid = req_id;
	CAReqID[index].u16tspid = pid;
	CAReqID[index].u8runflag = 1;
	CAReqID[index].u32tick = FYF_API_time_ms();
	if (wait_seconds == 0)
		CAReqID[index].u32waittime = 1000*1000;
	else 
		CAReqID[index].u32waittime = wait_seconds*1000;

	if(pid == 0x10)
		cdca_finishNitFilter = FYF_FALSE;
//	CAS_DMX_MUTEX_UNLOCK();
	return CDCA_TRUE;
		
}


/*----------------------------------------------------------------------
	Interface Function END
----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------

	Description:  CAFilter module init funtion

	Paramenters:  void
	
	Return:       void

-------------------------------------------------------------------------------*/
void CD_CA_API_TS_Init(void)
{
    cas_demux_init();
}

/*-------------------------------------------------------------------------------

	Description:  Allocate Filters to filter private ca data, such as ECM, EMM.

	Paramenters:  byReqID:				请求收表的请求号。机顶盒应该保存该数据，
	              接收到CAS私有数据后调用DVTCASTB_StreamGuardDataGot()时应该
	              将该数据返回。
	              
	              wEcmPID:				需要过滤的流的PID。
	              
	              szFilter:				过滤器的值，为一个8个字节的数组，相当于8*8的过滤器。
	              
	              szMask:				过滤器的掩码，为一个8个字节的数组，
	              与8*8的过滤器对应，当szMask的某一bit位为0时表示要过滤的表对应的
	              bit位不需要与szFilter里对应的bit位相同，当szMask某一位为1时表示
	              要过滤的表对应的bit位一定要与szFilter里对应的bit位相同。
	              
	              byLen:				过滤器的长度，为8。
	              
	              nWaitSeconds:			收此表最长的等待时间，如果为0则无限等待，单位秒。 
	
	Return:       0 Stand for success, otherwise, fail.    

-------------------------------------------------------------------------------*/
CDCA_BOOL CDSTBCA_GetPrivateData(CDCA_U8 byReqID, const CDCA_U8* szFilter, const CDCA_U8* szMask, CDCA_U8 byLen, CDCA_U16 wPID, CDCA_U8 nWaitSeconds)

{
	int i;
	DEBUG_CDCA("CDSTBCA_GetPrivateData byReqID = %d,wPID= 0x%x, tableid = %d, byLen = %d, nWaitSeconds = %d\n",byReqID,wPID, szFilter[0], byLen, nWaitSeconds);
	for (i = 0; i < byLen; i++)
		DEBUG_CDCA ("0x%x ", szFilter[i]);
	DEBUG_CDCA("\n");

	return cas_demux_request(byReqID, szFilter, szMask, byLen, wPID, nWaitSeconds);
}



extern FYF_Ecm_Info_s serviceInfo_av;
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void	CDSTBCA_ScrSetCW(CDCA_U16 wEcmPID, const CDCA_U8* pbyOddKey, const CDCA_U8* pbyEvenKey, CDCA_U8 byKeyLen, CDCA_BOOL bTapingEnabled)
{
	BU08 CA_CW[8];
	BU32 audioDescrambler;
	BU32 videoDescrambler;
	BU32 i;

	DEBUG_CDCA("CDSTBCA_ScrSetCW byKeyLen = %d\n",byKeyLen);
	for (i = 0; i < 2; i++)
	{
		audioDescrambler = FYF_API_descr_GetAudioDescrambler (i);
		videoDescrambler = FYF_API_descr_GetVideoDescrambler (i);

		if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid == serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid && serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid == wEcmPID)
		{
			FYF_API_Memset (CA_CW, 0, 8);
			FYF_API_Memcpy (CA_CW, pbyOddKey, byKeyLen);

			FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
			FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);


			FYF_API_Memset (CA_CW, 0, 8);
			FYF_API_Memcpy (CA_CW, pbyEvenKey, byKeyLen);
			FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
			FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
		}
		else
		{
			if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid == wEcmPID)
			{
				FYF_API_Memset (CA_CW, 0, 8);
				FYF_API_Memcpy (CA_CW, pbyOddKey, byKeyLen);
				FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
				
				FYF_API_Memset (CA_CW, 0, 8);
				FYF_API_Memcpy (CA_CW, pbyEvenKey, byKeyLen);
				FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);

			}
			else if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid == wEcmPID)
			{
				FYF_API_Memset (CA_CW, 0, 8);
				FYF_API_Memcpy (CA_CW, pbyOddKey, byKeyLen);
				FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
				
				FYF_API_Memset (CA_CW, 0, 8);
				FYF_API_Memcpy (CA_CW, pbyEvenKey, byKeyLen);
				FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
			}
		}
	}
}

void CA_SetInvaildCW()
{
	BU08 CA_CW[8];
	BU32 audioDescrambler;
	BU32 videoDescrambler;
	BU32 i;

	//FYF_API_descr_open ();
	for (i = 0; i < 2; i++)
	
	{
		audioDescrambler = FYF_API_descr_GetAudioDescrambler (i);
		videoDescrambler = FYF_API_descr_GetVideoDescrambler (i);
		
		FYF_API_Memset (CA_CW, 0, 8);
		FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
		FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
		FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
		FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
	}

}


