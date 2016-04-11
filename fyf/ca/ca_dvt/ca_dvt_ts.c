/*===============================================================================

 File Name:    ca_dvt_ts.c

 Description:

 History:                  
===============================================================================*/
/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "fyf_com.h"
#include "../../../tpmiddle/include/app_common.h"

#define DEBUG_DVTCA //printf
/*----------------------------------------------------------------------
	Constant Define
----------------------------------------------------------------------*/
#define	CA_REQID_NUM		(20)

#define CAS_DMX_MUTEX_INIT()	if (cas_sema == 0) {\
			if ((cas_sema = FYF_API_semaphore_create("CAS_Semaphore", 1, FYF_THREAD_WAIT_FIFO))==0){\
			DEBUG_DVTCA("create semaphore failure with %d\r", cas_sema); \
			}}
#define CAS_DMX_MUTEX_DEL()	if (cas_sema != 0) {\
			FYF_API_semaphore_destroy(cas_sema);\
			cas_sema = 0;}
#define CAS_DMX_MUTEX_LOCK()	if (cas_sema != 0) FYF_API_semaphore_wait(cas_sema, 1000);

#define CAS_DMX_MUTEX_UNLOCK()	if (cas_sema != 0) FYF_API_semaphore_release(cas_sema);

/*----------------------------------------------------------------------
	Define Enum Variable Types
----------------------------------------------------------------------*/


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
	if (channel_id == 0 && filter_id == 0)
		return 1;
	//printf("CA_Stop_Demux channelID = %d %d\n", *channelID, *filterID);
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

	APP_IPI_Semaphore_Wait_Demux();
	FYF_API_demux_create_channel(64 * 1024, FYF_DEMUX_DATA_PSI_CHANNEL, channelID, pID);
	
	if((returnValue = FYF_API_demux_set_channel_pid(*channelID, pID)) != 0)
	{
		APP_IPI_Semaphore_Release_Demux();
		return 0;
	}
	
	FYF_API_demux_create_filter(*channelID, filterID);
	//printf("CA_Start_Demux channelID = %d %d\n", *channelID, *filterID);
	
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
	

	APP_IPI_Semaphore_Release_Demux();
	return 1;
}

 void  CA_SectionCallback(BU32 ChannelID, BU32 FilterID ,BU08 *u8Buffer, BU32 u32BufferLength)
{
	BS32 i, j;
	BU16 pid;
	BU32 reqid;
//	CAS_DMX_MUTEX_LOCK();
	DEBUG_DVTCA("CA_SectionCallback channelid = %d, filterid = %d\n", ChannelID, FilterID);
	BU08 sectionflag;



	for (i = 0; i < CA_REQID_NUM; i++)
	{
		if (CAReqID[i].u32ChannelID == ChannelID && CAReqID[i].u32FilterID == FilterID)
			break;
	}
	reqid = CAReqID[i].u16reqid;
	if (FYF_NULL == u8Buffer)
	{printf("CA_SectionCallback FYF_NULL\n");
		sectionflag = FYF_FALSE;
		u32BufferLength = 0;
	}
	else
		sectionflag = FYF_TRUE;
	if (i >= CA_REQID_NUM)
	{
		DEBUG_DVTCA("CA_SectionCallback error\n");
//		CAS_DMX_MUTEX_UNLOCK();
		return;
	}
	
//EMM数据 长度小于16，暂时不丢弃
	if ( i >= 2 && i < 10 &&u32BufferLength <= 16)
	{
//		CAS_DMX_MUTEX_UNLOCK();
//		return;
	}


	//DEBUG_DVTCA("\n +++ CA_SectionCallback i=%d datafilterid =%d\n", i, FilterID);
	pid = CAReqID[i].u16tspid;

	//ECM数据包，停止过滤，释放过滤器
	if (i < 2 || i >= 10)
	{
		CA_Stop_Demux(&(CAReqID[i].u32ChannelID), &(CAReqID[i].u32FilterID));
		CAReqID[i].u8runflag = 0;
		CAReqID[i].u16reqid = 0x1fff;
		CAReqID[i].u16tspid = 0x1fff;
		
	}
DEBUG_DVTCA("DVTCASTB_StreamGuardDataGot reqid = %d,pid=%x,sectionflag=%d,u32BufferLength=%d\n",reqid,pid,sectionflag,u32BufferLength);
	
	if(APP_CA_API_GetCardIn()==1)
		DVTCASTB_StreamGuardDataGot((BYTE)reqid, (bool)FYF_FALSE, (WORD)pid, (WORD)0, NULL);
	else	
	DVTCASTB_StreamGuardDataGot((BYTE)reqid, (bool)sectionflag, (WORD)pid, (WORD)u32BufferLength, (BYTE *)u8Buffer);
//	CAS_DMX_MUTEX_UNLOCK();

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

/*=============================================================================
  
  Description:
 
  Paramenters:  @para 
 
  Return:           

==============================================================================*/
void cas_demux_stopECMFilter()
{
	BU32 i = 0;
	BU16 pid;
    BU32 reqid;
	for (i = 10; i < CA_REQID_NUM; i++)
	{
		if (CAReqID[i].u8runflag && CAReqID[i].u16tspid != 0x10)
		{
				CA_Stop_Demux(&(CAReqID[i].u32ChannelID), &(CAReqID[i].u32FilterID));
				pid = CAReqID[i].u16tspid;
				reqid = CAReqID[i].u16reqid;
	
				CAReqID[i].u8runflag = 0;
				CAReqID[i].u16reqid = 0x1fff;
				CAReqID[i].u16tspid = 0x1fff;
				CAReqID[i].u8callbackflag = 0;
				CAReqID[i].u32tick = 0;
				CAReqID[i].u32waittime = 0;
			 	DVTCASTB_StreamGuardDataGot(CAReqID[i].u16reqid,false,CAReqID[i].u16tspid, 0,NULL);
		}
	}
}

void cas_demux_stopEMMFilter()
{
	BU32 i = 0;
	BU32 channel_id = 0;
	BU16 pid;
    BU32 reqid;

	DVTCASTB_SetEmmPid(0);
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
				DVTCASTB_StreamGuardDataGot(reqid,false,pid, 0,NULL); 
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
void cas_dmx_timeout_check()
{
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
			if (times - CAReqID[i].u32tick > CAReqID[i].u32waittime)
			{
			    pid = CAReqID[i].u16tspid;
				reqid = CAReqID[i].u16reqid;
				if (i < 2 || i >= 10)
				{
					printf("times = %d %d %d %d\n", i, times, CAReqID[i].u32tick, CAReqID[i].u32waittime);
					CA_Stop_Demux(&(CAReqID[i].u32ChannelID), &(CAReqID[i].u32FilterID));
					CAReqID[i].u8runflag = 0;
					
					CAReqID[i].u16reqid = 0x1fff;
					CAReqID[i].u16tspid = 0x1fff;
				}
				DVTCASTB_StreamGuardDataGot((BYTE)reqid, (bool)false, (WORD)pid, (WORD)0, NULL );

			}
		}
	}
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

/*=============================================================================
  
  Description:
 
  Paramenters:  @para 
 
  Return:           

==============================================================================*/
BS32 cas_demux_request(BU08 req_id, BU08* value, BU08* mask, BU08 mask_len, BU16 pid, BU08 wait_seconds)
{
	BU32 s32filterid, i;
	BU08 excl[16];
	BU32 index;
	BU08 acData[16];
	BU08 acMask[16];

	if((value==NULL) || (mask==NULL) || (mask_len==0))
	{
		DEBUG_DVTCA("Err: %s- error param!\n",__FUNCTION__);
		return DVTCAERR_STB_POINTER_INVALID;
	}

	for (i = 0; i < mask_len; i++)
	{
		excl[i] = 0;
	}


	memset(acData, 0, mask_len);
	memset(acMask, 0, mask_len);

	for (i = 0; i < mask_len - 2; i++)
	{	
	
		if (i == 0)
		{
		//	acData[i] = (value[i] & mask[i]);
			acMask[i] = mask [i];
			acData[i] = value[i];
		}
		else
		{
		//	acData[i] = (value[i+2] & mask[i+2]);
		  	acMask[i] =mask [i+2];
			acData[i] = value[i+2];
		}
	
	}

	
//	CAS_DMX_MUTEX_LOCK();
	if (req_id < 2)
	{
		index = CA_GetReID(pid);
		CA_Stop_Demux(&(CAReqID[index].u32ChannelID), &(CAReqID[index].u32FilterID));
		CA_Start_Demux(mask_len - 2, pid, acData, acMask, excl, &(CAReqID[index].u32ChannelID), &(CAReqID[index].u32FilterID));
	}
	else
	{
		index = req_id;
#if 1
		if (CAReqID[req_id].u16tspid == 0x1fff)
		{
			//CA_Stop_Demux(&(CAReqID[req_id].u32ChannelID), &(CAReqID[req_id].u32FilterID));
			CA_Start_Demux(mask_len - 2, pid, acData, acMask, excl, &(CAReqID[req_id].u32ChannelID), &(CAReqID[req_id].u32FilterID));
		}
		else
		{
			DEBUG_DVTCA("CAReqID[req_id].u32ChannelID = %d CAReqID[req_id].u32FilterID = %d\n"
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
		
			CA_Start_Demux(mask_len - 2, pid, acData, acMask, excl, &(CAReqID[req_id].u32ChannelID), &(CAReqID[req_id].u32FilterID));
	
		}
#endif
	}

	DEBUG_DVTCA ("(CAReqID[req_id].u32ChannelID= %d, filterid = %d\n", CAReqID[index].u32ChannelID, CAReqID[index].u32FilterID);
	CAReqID[index].u16reqid = req_id;
	CAReqID[index].u16tspid = pid;
	CAReqID[index].u8runflag = 1;
	CAReqID[index].u32tick = FYF_API_time_ms();
	if (wait_seconds == 0)
		CAReqID[index].u32waittime = 1000*1000;
	else 
		CAReqID[index].u32waittime = wait_seconds*1000;

//	CAS_DMX_MUTEX_UNLOCK();
	return DVTCA_OK;
}
/*-------------------------------------------------------------------------------

	Description:  CAFilter module init funtion

	Paramenters:  void
	
	Return:       void

-------------------------------------------------------------------------------*/
void CA_DVT_API_TS_Init()
{
    cas_demux_init();
}

extern FYF_Ecm_Info_s serviceInfo_av;
/*-------------------------------------------------------------------------------

	Description:   Used to set descramble, sent the curent cw and the next cycle one.

	Paramenters:   wEcmPID:				    CW所属的ECMPID。
	               szOddKey:				奇CW的数据。
	               szEvenKey:				偶CW的数据。
                   byKeyLen:				CW的长度。
                   bTaingControl:			true：允许录像,false：不允许录像。
	
	Return:        0 Stand for success, otherwise, fail.    

-------------------------------------------------------------------------------*/
HRESULT DVTSTBCA_SetDescrCW(WORD wEcmPID, BYTE byKeyLen, const BYTE* szOddKey, const BYTE* szEvenKey,bool bTaingControl)
{
	BU08 CA_CW[8];
	BU32 audioDescrambler;
	BU32 videoDescrambler;
	BU32 i;

	DEBUG_DVTCA("DVTSTBCA_SetDescrCW byKeyLen = %d\n",byKeyLen);
	for (i = 0; i < 2; i++)
	{
		audioDescrambler = FYF_API_descr_GetAudioDescrambler (i);
		videoDescrambler = FYF_API_descr_GetVideoDescrambler (i);

		if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid == serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid && serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid == wEcmPID)
		{
			FYF_API_Memset (CA_CW, 0, 8);
			FYF_API_Memcpy (CA_CW, szOddKey, byKeyLen);

			FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
			FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);


			FYF_API_Memset (CA_CW, 0, 8);
			FYF_API_Memcpy (CA_CW, szEvenKey, byKeyLen);
			FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
			FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
		}
		else
		{
			if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid == wEcmPID)
			{
				FYF_API_Memset (CA_CW, 0, 8);
				FYF_API_Memcpy (CA_CW, szOddKey, byKeyLen);
				FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
				
				FYF_API_Memset (CA_CW, 0, 8);
				FYF_API_Memcpy (CA_CW, szEvenKey, byKeyLen);
				FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);

			}
			else if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid == wEcmPID)
			{
				FYF_API_Memset (CA_CW, 0, 8);
				FYF_API_Memcpy (CA_CW, szOddKey, byKeyLen);
				FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
				
				FYF_API_Memset (CA_CW, 0, 8);
				FYF_API_Memcpy (CA_CW, szEvenKey, byKeyLen);
				FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
			}
		}
	}
	return DVTCA_OK;
}
void CA_SetInvaildCW(void)
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
HRESULT DVTSTBCA_SetStreamGuardFilter(BYTE byReqID, WORD wPID, const BYTE * szFilter, const BYTE * szMask, BYTE byLen, int nWaitSeconds)
{
	int i;
	DEBUG_DVTCA("DVTSTBCA_SetStreamGuardFilter byReqID = %d,wPID= 0x%x, tableid = %x, szMask = %x,byLen = %d, nWaitSeconds = %d\n",byReqID,wPID, szFilter[0], szMask[0],byLen, nWaitSeconds);
	for (i = 0; i < byLen; i++)
		DEBUG_DVTCA ("0x%x ", szFilter[i]);
	DEBUG_DVTCA("\n");

	return cas_demux_request(byReqID, szFilter, szMask, byLen, wPID, nWaitSeconds);
}


BU08 DVTCA_SetAreaCodeInNIT(BYTE *DescData)
{
	BYTE i;
	BYTE DVTOldAreaCode[4];
	BYTE DVTAreaCode [4]={0,0,0,0};
	
	DEBUG_DVTCA("Newland DVTCA_SetAreaCodeInNIT enter  \n");
	if (DescData[0] == 0x5f)
	{
		DEBUG_DVTCA("Newland GetAreaCodeInNIT  PDescData[0] = 0x%02x\n",DescData[0]);
			
		for(i =0 ; i < 4; i++)
		{
			//DEBUG_DVTCA("Newland PDescData[%d] = 0x%02x\n",(2+i),PDescData[ 2+i ]);
			DVTAreaCode[i] = DescData[2+ i] ;
		}
		
		DEBUG_DVTCA("Newland GetAreaCodeInNIT [0x%x 0x%x 0x%x 0x%x]\n",
			DVTAreaCode[0],DVTAreaCode[1],DVTAreaCode[2],DVTAreaCode[3]);
		
		APP_SYS_API_GetSysInfo(APP_SYS_DVTCAAreaCode_e, (BU32 *)DVTOldAreaCode);

		DEBUG_DVTCA("Newland GetAreaCodeInNIT Old areaNVM[0x%x 0x%x 0x%x 0x%x]\n",
			DVTOldAreaCode[0],DVTOldAreaCode[1],DVTOldAreaCode[2],DVTOldAreaCode[3]);

		for (i =0 ; i < 4; i++)
		{
			if (DVTAreaCode[i] != DVTOldAreaCode[i] )
			{
				DEBUG_DVTCA("Newland Update DVT AreaCode to E2 !\n");
				APP_SYS_API_SetSysInfo(APP_SYS_DVTCAAreaCode_e,(BU32)DVTAreaCode);
				APP_SYS_API_SaveSysInfo();
				break;
			}
		}
		return 1;
	}

	return 0;
}

/*++
功能:获取NIT中的值，用于和区域码对比。
参数:
	pdwData:	机顶盒提供的从NIT中获取的值
返回值:
	0:		获取成功
	其它值:	获取失败
--*/
HRESULT	DVTSTBCA_GetNitValue(DWORD * pdwData)
{
	BYTE i = 0;
	DWORD DVTAreaCodedata = 0;
	BYTE AreaNVM[4];
	DEBUG_DVTCA("DVTSTBCA_GetNitValue enter\n");
	APP_SYS_API_GetSysInfo(APP_SYS_DVTCAAreaCode_e, (BU32 *)AreaNVM);
	
	DVTAreaCodedata = AreaNVM[0];
	DVTAreaCodedata = (DVTAreaCodedata<<8)|AreaNVM[1];
	DVTAreaCodedata = (DVTAreaCodedata<<8)|AreaNVM[2];
	DVTAreaCodedata = (DVTAreaCodedata<<8)|AreaNVM[3];
	*pdwData = DVTAreaCodedata;
	printf("Newland DVTSTBCA_GetNitValue old DVTAreaCodedata = 0x%x pdwData = 0x%x\n",DVTAreaCodedata,*pdwData);
	#if 0
	if( DVTAreaCodedata == 0 )
	{
		return DVTCAERR_INPUT_DATA_INVALID;
	}
	else
	{
		return DVTCA_OK;
	}
	#endif
	return DVTCA_OK;

}


