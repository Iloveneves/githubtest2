/*****************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: ipanel_demux.c
 * Description: This file define the functions for demux device management.
 *
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1
 * 1. 2008.08.26         ZhengGuangPing            inital
 *
 *******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  /* __cplusplus */

#include "trd_ipanel_init.h"
#include "ipanel_base.h"
#include "ipanel_demux.h"
#include "ipanel_porting_event.h"

#define MAX_CHANNEL_NUM  (32)
#define MAX_FILTER_NUM   (30)
#define IPANEL_DEBUX_DEBUG //IPANEL_ERROR


NOTIFY_FUNC  func1 = 0;
BU32 utmpFilterId = 0;
//extern BS32 s_ipanel_semid;
extern BU08 g_demux_flag;
void ipanel_demux_init(void)
{
	IPANEL_DEBUX_DEBUG("ipanel_demux_init\n");
}

void ipanel_porting_callback(BU32 ch,BU32 filter,BU32 DataStartAddr,BU32 len)
{
	BU08 *buff = (BU08 *)DataStartAddr;
	BU32 filterid = ((ch & 0xff) << 16) | (filter & 0xff);
	BU32 i;
	//printf(" ipanel_porting_callback ch %d filter %d get len %d \n",ch,filter,len);
	/*for (i = 0; i < 8; i++)
	{
		printf("%x ",buff[i]);
	}
	printf("\n");*/
	if(func1 && g_demux_flag)	
	{
		func1(ch,filterid,buff,len);
	}
}

int ipanel_porting_set_demux_notify(NOTIFY_FUNC func)
{
	IPANEL_DEBUX_DEBUG("ipanel_porting_set_demux_notify\n");
	func1 = func;
	FYF_API_demux_set_ipanel_notify(0, (FYF_DEMUX_NOTIFY_FUNC)ipanel_porting_callback);
	return 0;
}	

BU32 ipanel_porting_create_demux_channel(BS32 poolsize, BU16 pid)
{
	BS32 ret = FYF_ERR;
	BU32 uChannelId = 0;
	
	IPANEL_DEBUX_DEBUG("ipanel_porting_create_demux_channel, pid = %d\n",pid);

//	FYF_API_semaphore_wait(s_ipanel_semid,FYF_WAIT_FOREVER);

	if (Ipanel_GetEntrance_Flag() == FYF_FALSE)
	{
			ret = FYF_API_demux_create_channel(poolsize, FYF_DEMUX_DATA_PSI_CHANNEL, &(uChannelId), pid);
					
			if (FYF_ERR == ret)
			{
				IPANEL_ERROR("FYF_API_demux_create_channel ERROR\n");
		//		FYF_API_semaphore_release(s_ipanel_semid);
				return 0;
			}
		//	FYF_API_semaphore_release(s_ipanel_semid);
		
			return uChannelId;

	}
	else
	{
		return 0;
	}

}


BU32 ipanel_porting_create_demux_filter(BU32 physical_channel, BU32 wide,
                                            BU08 coef[4], BU08 mask[4], BU08 excl[4])
{

	BS32 i = 0, ret = FYF_ERR, k =0;
	BU32 uFilterId = 0;
//	FYF_API_semaphore_wait(s_ipanel_semid,FYF_WAIT_FOREVER);
	if (Ipanel_GetEntrance_Flag() == FYF_FALSE)
	{
			ret = FYF_API_demux_create_filter(physical_channel, &(uFilterId));
				
			if (FYF_ERR == ret)
			{
				IPANEL_ERROR("FYF_CPI_demux_create_filter ERROR\n");
		//		FYF_API_semaphore_release(s_ipanel_semid);
		
				return 0;
			}
		
			if (wide > 4)
			{	
				wide = 4;
			}
			
#ifdef IPANEL_DEMUX_DEBUG
				printf("match:");
				for (k = 0; k<4;k++)
				{
					printf("%02x ",coef[k]);
				}
				printf("\n");
				printf("mask :");
				for (k = 0; k<4;k++)
				{
					printf("%02x ",mask[k]);
				}
				printf("\n");
#endif
				
				ret = FYF_API_demux_set_filter(physical_channel, 
												uFilterId,	
												wide,
												coef,  
												mask, 
												excl);
	//	FYF_API_semaphore_release(s_ipanel_semid);
	
		if (FYF_ERR == ret)
		{
			IPANEL_ERROR("FYF_API_demux_set_filter ERROR\n");
			return 0;
		}
		
	
		uFilterId = ((physical_channel & 0xff) << 16) | (uFilterId & 0xff);
	
		
		IPANEL_DEBUX_DEBUG("ipanel_porting_create_demux_filter,uFilterId = %x\n", uFilterId);
	
		return uFilterId;

	}
	else
	{
		return 0;
	}
}

int ipanel_porting_start_demux_channel(BU32 channel)
{	
	BS32 ret = FYF_ERR;
	IPANEL_DEBUX_DEBUG("ipanel_porting_start_demux_channel, channel = %d\n", channel);
//	FYF_API_semaphore_wait(s_ipanel_semid,FYF_WAIT_FOREVER);
	if (Ipanel_GetEntrance_Flag() == FYF_FALSE)
	{
			ret = FYF_API_demux_start_channel(channel);
		//	FYF_API_semaphore_release(s_ipanel_semid);
		
			if (FYF_ERR == ret)
			{
				IPANEL_ERROR("FYF_CPI_demux_start_channel ERROR\n");
				return FYF_ERR;
			}	
			return 0;
	}
	else
	{
		return 0;

	}
	
}


int ipanel_porting_stop_demux_channel(BU32 channel)
{
	BS32 ret = FYF_ERR;
	IPANEL_DEBUX_DEBUG("ipanel_porting_stop_demux_channel, channel = %x\n", channel);
//	FYF_API_semaphore_wait(s_ipanel_semid,FYF_WAIT_FOREVER);
	if (Ipanel_GetEntrance_Flag() == FYF_FALSE)
	{
			ret = FYF_API_demux_stop_channel(channel);
		//	FYF_API_semaphore_release(s_ipanel_semid);
		
			if (FYF_ERR == ret)
			{
				IPANEL_ERROR("FYF_CPI_demux_stop_channel ERROR\n");
		
				return FYF_ERR;
			}
			return 0;

	}
	else
	{
		return 0;
	}
	
	
}


int ipanel_porting_disable_demux_filter(BU32 filter)
{
	
	BS32 ret = FYF_ERR;
	BU32 channelid, filterid;
	
	IPANEL_DEBUX_DEBUG("ipanel_porting_disable_demux_filter, filter = %x\n", filter);
//	FYF_API_semaphore_wait(s_ipanel_semid,FYF_WAIT_FOREVER);

	if (Ipanel_GetEntrance_Flag() == 1)
		return 0;

	channelid = (filter >> 16) & 0xff;
	filterid = filter & 0xff;


	ret = FYF_API_demux_disable_filter(channelid, filterid);
//	FYF_API_semaphore_release(s_ipanel_semid);

	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("FYF_CPI_demux_disable_filter ERROR\n");
		return FYF_ERR;
	}

	return 0;
}


int ipanel_porting_enable_demux_filter(BU32 filter)
{
	BS32 ret = FYF_ERR;
	BU32 channelid, filterid;
	
	if (Ipanel_GetEntrance_Flag() == 1)
		return 0;

	IPANEL_DEBUX_DEBUG("ipanel_porting_enable_demux_filter,filter = %x\n", filter);
//	FYF_API_semaphore_wait(s_ipanel_semid,FYF_WAIT_FOREVER);

	channelid = (filter >> 16) & 0xff;
	filterid = filter & 0xff;
	
	ret = FYF_API_demux_enable_filter(channelid, filterid);
//	FYF_API_semaphore_release(s_ipanel_semid);
	
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("ipanel_porting_enable_demux_filter ERROR\n");
		return FYF_ERR;
	}

	return 0;
}


int ipanel_porting_destroy_demux_filter(BU32 filter)
{
	BS32 ret = FYF_ERR;
	BU32 channelid, filterid;
	
	IPANEL_DEBUX_DEBUG("ipanel_porting_destroy_demux_filter,filter = %x\n", filter);
//	FYF_API_semaphore_wait(s_ipanel_semid,FYF_WAIT_FOREVER);
	if (Ipanel_GetEntrance_Flag() == 1)
		return 0;


	channelid = (filter >> 16) & 0xff;
	filterid = filter & 0xff;
	
	ret = FYF_API_demux_destroy_filter(channelid, filterid);
//	FYF_API_semaphore_release(s_ipanel_semid);
	
	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("FYF_CPI_demux_disable_filter ERROR\n");
		return FYF_ERR;
	}
		
	return 0;
}


int ipanel_porting_destroy_demux_channel(BU32 channel)
{


	BS32 ret = FYF_ERR;
	BU32 uChannelId = 0;
	IPANEL_DEBUX_DEBUG("ipanel_porting_destroy_demux_channel, channel = %d\n", channel);
//	FYF_API_semaphore_wait(s_ipanel_semid,FYF_WAIT_FOREVER);
	if (Ipanel_GetEntrance_Flag() == 1)
		return 0;

	ret = FYF_API_demux_destroy_channel(channel);
//	FYF_API_semaphore_release(s_ipanel_semid);

	if (FYF_ERR == ret)
	{
		IPANEL_ERROR("FYF_CPI_demux_stop_channel ERROR\n");
		return FYF_ERR;
	}
		
	return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  /* __cplusplus */

