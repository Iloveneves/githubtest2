/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*                                          								  */
/******************************************************************************/ 

#include <pthread.h>
#include "hi_type.h"
#include "hi_unf_demux.h"
#include "fyf_def.h"
#include "fyf_demux.h"



#define SECTION_DATA_BUFFERSIZE		(10)	/* SETCION 最大缓存个数空间*/
#define FYF_DEMUX_MAX_CHANNEL       (25)
#define FYF_DEMUX_MAX_FILTER 		(20)
#define FYF_DEMUX_CHANNEL_BUFFLEN  	4*1024
#define SECTION_FILTER_DEPTH_MAX 	(16)	/* 过滤器最大过滤深度*/

static HI_U32 video_channel = 0xffffffff;
static HI_U32 audio_channel = 0xffffffff;
static HI_U32 pcr_channel = 0xffffffff;

static HI_U16 video_pid = 0x1fff;
static HI_U16 audio_pid = 0x1fff;
static HI_U16 pcr_pid = 0x1fff;
static BU32 g_cur_demuxId = 0;
#define FYF_CPI_DBG //printf

static pthread_t s_pSectionReadTaskHandle = NULL;	


typedef struct  FYF_DATA_FILTER_S
{
	HI_S32 s32DataFilterID;     	/* 过滤ID */
	HI_U16 u16FilterDepth;    		/* 过滤深度，0xff - 数据全部用用户设置，否则使用DVB算法（fixme） */
	
	HI_U8 u8Match[SECTION_FILTER_DEPTH_MAX];     	
	HI_U8 u8Mask[SECTION_FILTER_DEPTH_MAX];    	
	HI_U8 u8Negate[SECTION_FILTER_DEPTH_MAX];    
	HI_U32 u32TimeOut;     	 /* 超时，单位-秒 0- 永远等待*/

//	T_CommReadyCallback    funReadyFunCallback;    	/* 过滤准备回调函数，SECTION主任务在接收到MESSAGE_SECTIONSVR_START消息后调用  */
//	FYF_DEMUX_NOTIFY_FUNC  funSectionFunCallback; 	/* section完成回调函数 */
//	T_CommTableCallback     funTableFunCallback; 		/* table完成回调函数 */
    
} FYF_DATA_FILTER_S;


typedef struct FYF_Filter_S
{
	HI_U32 filterflag;
	FYF_DATA_FILTER_S filter;
}FYF_FILTER_S;


typedef struct FYF_Channel_S
{
	BU08 channelflag;
	BU32 channelpid;
	BU32 channelid;
	BU16 frontendid;
	BU16 filternum;
	BU08 u32SectionType;			/*sec or pes*/
	FYF_FILTER_S filtergrop[FYF_DEMUX_MAX_FILTER];
}FYF_CHANNEL_S;

static FYF_CHANNEL_S  fyf_channel[FYF_DEMUX_MAX_CHANNEL];
static BU08	fyf_demux_init = FYF_FALSE;
FYF_DEMUX_NOTIFY_FUNC FYFDemuxCallbackFunction = NULL;
FYF_DEMUX_NOTIFY_FUNC FYFDemuxIpanelCallbackFunction = NULL;
FYF_DEMUX_NOTIFY_FUNC FYFDemuxCACallbackFunction = NULL;

static pthread_mutex_t demux_mutex;

BU32 FYF_CPI_demux_get_pid(BU08 pidtype , BU32 *pid)
{
	pthread_mutex_lock(&demux_mutex);
	switch(pidtype)
	{
	case 0:
		*pid = audio_pid;
		break;
	case 1:
		*pid = video_pid;
		break;
	case 2:
		*pid = pcr_pid;
		break;	
	default:
		break;
	}
	pthread_mutex_unlock(&demux_mutex);
	return FYF_OK;
}


/******************************************************************************/
/*Description: set notify func                                                */
/*Input      : func														      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_set_notify(FYF_DEMUX_NOTIFY_FUNC func)
{
	FYF_CPI_DBG(" +++ FYF_CPI_demux_set_notify +++ \n");
	FYFDemuxCallbackFunction = func;//eBrowserSvr_RegisterCallback(func);
	return FYF_OK;
}
BS32 FYF_API_demux_set_ipanel_notify(BU32 channelid, FYF_DEMUX_NOTIFY_FUNC func)
{
	FYFDemuxIpanelCallbackFunction = func;
	return FYF_OK;
	
}

BS32 FYF_API_demux_set_CA_notify(FYF_DEMUX_NOTIFY_FUNC func)
{
	FYFDemuxCACallbackFunction = func;
}
/******************************************************************************/
/*Description: create channel                                                 */
/*Input      : poolsize,type    										      */
/*Output     : no                                                             */
/*Return     : =0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_create_channel(BS32 poolsize, FYF_DEMUX_CHANNEL_TYPE_e type, BU32 *channel_id, BU16 pid)
{
	BS32 ret;
	BU32 i = 0;
	BU08 newchanelflag = 0;
	HI_UNF_DMX_CHAN_ATTR_S struChan;
	pthread_mutex_lock(&demux_mutex);
	if(FYF_DEMUX_DATA_PSI_CHANNEL == type || FYF_DEMUX_DATA_PES_CHANNEL == type)
	{	

		for (i = 1; i < FYF_DEMUX_MAX_CHANNEL; i++)
		{
			if (fyf_channel[i].channelflag == 1 && pid == fyf_channel[i].channelpid)
			{
				*channel_id = i;
				break;
			}
		}

		if (i == FYF_DEMUX_MAX_CHANNEL)
		{
			newchanelflag = 1;
		}
		if(newchanelflag)
		{
			for (i = 1; i < FYF_DEMUX_MAX_CHANNEL; i++)
			{
				if (fyf_channel[i].channelflag == 0)
				{
					fyf_channel[i].channelpid = pid;
					fyf_channel[i].channelflag = 1;
					fyf_channel[i].u32SectionType = type;
					fyf_channel[i].frontendid = g_cur_demuxId;
					*channel_id = i;
					break;
				}
			}

			if (i == FYF_DEMUX_MAX_CHANNEL)
			{
				printf("FYF_API_demux_create_channel error\n");
				pthread_mutex_unlock(&demux_mutex);
				return FYF_ERR;
			}
			
			ret = HI_UNF_DMX_GetChannelDefaultAttr(&struChan);
			if (HI_SUCCESS != ret)
			{		
				printf("HI_UNF_DMX_GetChannelDefaultAttr fail\n");
				pthread_mutex_unlock(&demux_mutex);
				return ret;			
			}
			//升级流的pid，不进行crc校验
			if (pid == 0x1395 || pid == 0x1fea)
			{
				struChan.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
			}
			/*如果是PES类型的通道*/
			if(fyf_channel[i].u32SectionType == 1)
			{
				struChan.enChannelType = HI_UNF_DMX_CHAN_TYPE_PES;
				struChan.u32BufSize = 32;
				struChan.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
			}	

			//struChan.u32BufSize = 512;

			/*分配channel资源*/
			ret = HI_UNF_DMX_NewChannel(fyf_channel[i].frontendid, &struChan, &(fyf_channel[i].channelid));
			if (HI_SUCCESS != ret)
			{
				printf("\n +++ HI_UNF_DMX_NewChannel ERROR +++\n");
				pthread_mutex_unlock(&demux_mutex);
				return ret;	
			}
						
		}
		FYF_CPI_DBG("newchanflag =%d, channel_id = %d %d\n", newchanelflag, *channel_id, fyf_channel[*channel_id].channelid);

	}
	else if(FYF_DEMUX_VIDEO_CHANNEL == type)
	{
		ret = HI_UNF_DMX_GetVideoChannelHandle(0,channel_id);
		if(ret == HI_FAILURE)
		{	
			printf(" get video channel error %d\n");
			pthread_mutex_unlock(&demux_mutex);
			return FYF_ERR;
		}
		video_channel = *channel_id;
		printf("video_channel = %d\n", video_channel);
	}	
	else if(FYF_DEMUX_AUDIO_CHANNEL == type)
	{
		ret = HI_UNF_DMX_GetAudioChannelHandle(0,channel_id);
		if(ret == HI_FAILURE)
		{	
			printf(" get audio channel error %d\n");
			pthread_mutex_unlock(&demux_mutex);
			return FYF_ERR;
		}
		audio_channel = *channel_id;
	}	
	else if(FYF_DEMUX_PCR_CHANNEL == type)
	{	
		FYF_CPI_DBG(" error channel type %d \n",type);
		*channel_id = 32;
		pcr_channel = *channel_id;
	}
	pthread_mutex_unlock(&demux_mutex);	
	return FYF_OK;
}
/******************************************************************************/
/*Description: set channel pid                                                */
/*Input      : channel,pid       										      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_set_channel_pid(BU32 channel, BU16 pid)
{
	BS32 ret, i;		
	BU08 newchanelflag = 0;
	HI_UNF_DMX_CHAN_ATTR_S struChan;
	pthread_mutex_lock(&demux_mutex);
	if(channel == video_channel)
	{
		FYF_CPI_DBG(" set vide channel  pid %d \n",pid);
		video_pid = pid;
	}	
	else if(channel == audio_channel)
	{
		FYF_CPI_DBG(" set audio channel  pid %d \n",pid);
		audio_pid = pid;
	}	
	else if(channel == pcr_channel)
	{
		FYF_CPI_DBG(" set pcr channel  pid %d \n",pid);
		pcr_pid = pid;
	}	
	else
	{
		fyf_channel[channel].channelpid = pid;
	}
	pthread_mutex_unlock(&demux_mutex);
	return FYF_OK;
}
/******************************************************************************/
/*Description: set channel pid                                                */
/*Input      : channel,pid       										      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_destroy_channel(BU32 channel)
{
	BS32 ret, i;
	pthread_mutex_lock(&demux_mutex);
	if(channel == video_channel)
	{
		FYF_CPI_DBG(" free vide channel  \n");
		video_channel = 0xffffffff;
		video_pid = 0x1fff;
	}	
	else if(channel == audio_channel)
	{
		FYF_CPI_DBG(" free audio channel \n");
		audio_channel = 0xffffffff;
		audio_pid = 0x1fff;

	}	
	else if(channel == pcr_channel)
	{
		FYF_CPI_DBG(" free pcr channel  \n");
		pcr_channel = 0xffffffff;
		pcr_pid = 0x1fff;
	}	
	else
	{	
		if (fyf_channel[channel].channelflag == 1)
			/* 释放channel资源*/
		{
			ret = HI_UNF_DMX_DeleteChannel(fyf_channel[channel].channelid);
			if (HI_SUCCESS != ret)
			{	
				printf("\n +++ HI_UNF_DMX_DeleteChannel failure +++\n");
			}
			memset(&(fyf_channel[channel]), 0, sizeof(FYF_CHANNEL_S));
			fyf_channel[channel].channelpid = 0x1fff;
		}
		else
		{
			printf("fyf_channel[channel].channelid = %d %d!!\n", channel, fyf_channel[channel].channelid);
			pthread_mutex_unlock(&demux_mutex);
			return FYF_OK;
		}
	}
	pthread_mutex_unlock(&demux_mutex);
	return ret;
}
/******************************************************************************/
/*Description: start channel                                 	              */
/*Input      : channel       											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_start_channel(BU32 channel)
{
	BS32 ret;
	BU32 tmppid;
	HI_UNF_DMX_CHAN_STATUS_S channeltatus;
	pthread_mutex_lock(&demux_mutex);
	if(channel == video_channel)
	{
		FYF_CPI_DBG(" open vide channel  \n");
		if(video_pid == 0x1fff)
		{
			pthread_mutex_unlock(&demux_mutex);
			return HI_FAILURE;
		}
	}	
	else if(channel == audio_channel)
	{
		FYF_CPI_DBG(" open audio channel \n");
		if(audio_pid == 0x1fff)
		{
			pthread_mutex_unlock(&demux_mutex);
			return HI_FAILURE;
		}

	}	
	else if(channel == pcr_channel)
	{
		FYF_CPI_DBG(" open pcr channel pid = 0x%x\n",pcr_pid);
		if(pcr_pid == 0x1fff)
		{
			pthread_mutex_unlock(&demux_mutex);
			return HI_FAILURE;
		}
	}	
	else
	{
			FYF_CPI_DBG("fyf_channel[channel].channelid = %d\n", fyf_channel[channel].channelid);
			ret =HI_UNF_DMX_GetChannelStatus(fyf_channel[channel].channelid,&channeltatus);
			if (HI_SUCCESS != ret)
			{
				printf("HI_UNF_DMX_GetChannelStatus ERROR %x\n",ret);
			}

			if(channeltatus.enChanStatus != HI_UNF_DMX_CHAN_CLOSE)
			{
				ret = HI_UNF_DMX_GetChannelPID(fyf_channel[channel].channelid, &tmppid);
				if (ret != HI_SUCCESS)
				{
					printf("\n +++ HI_UNF_DMX_GetChannelPID ERROR  0x%x +++\n", ret );
				}
				if (tmppid != fyf_channel[channel].channelpid)
				{
					printf("tmppid = %d %d\n", tmppid, fyf_channel[channel].channelpid);
					printf("status  error  channel already run\n");
					pthread_mutex_unlock(&demux_mutex);

					return FYF_ERR;
				}
			}
			
			FYF_CPI_DBG("channel = %d, pid = %d\n", channel, fyf_channel[channel].channelpid);
			

			ret = HI_UNF_DMX_SetChannelPID(fyf_channel[channel].channelid, fyf_channel[channel].channelpid);
			FYF_CPI_DBG("pid = 0x%x\n", fyf_channel[channel].channelpid);
			if (HI_SUCCESS != ret)
			{
				printf("\n +++ HI_UNF_DMX_SetChannelPID ERROR  0x%x +++\n", ret );
				pthread_mutex_unlock(&demux_mutex);
				return ret;	
			}
			
			ret = HI_UNF_DMX_OpenChannel(fyf_channel[channel].channelid);
			if (HI_SUCCESS != ret)
			{
				printf("\n +++ HI_UNF_DMX_OpenChannel ERROR +++ 0x%x\n",ret);
				pthread_mutex_unlock(&demux_mutex);
				return FYF_ERR;
			}
	}
	pthread_mutex_unlock(&demux_mutex);
	return FYF_OK;
	
}
/******************************************************************************/
/*Description: stop channel                                 	              */
/*Input      : channel       											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_stop_channel(BU32 channel)
{
	BS32 ret;
	HI_UNF_DMX_CHAN_STATUS_S channeltatus;

	pthread_mutex_lock(&demux_mutex);
	if(channel == video_channel)
	{
		FYF_CPI_DBG(" open vide channel  \n");

	}	
	else if(channel == audio_channel)
	{
		FYF_CPI_DBG(" open audio channel \n");

	}	
	else if(channel == pcr_channel)
	{
		FYF_CPI_DBG(" open audio channel \n");
	}	
	else
	{
		if (fyf_channel[channel].channelflag == 1)
		{
		
			
			ret =HI_UNF_DMX_GetChannelStatus(fyf_channel[channel].channelid,&channeltatus);
			if (HI_SUCCESS != ret)
			{
				printf("HI_UNF_DMX_GetChannelStatus ERROR %x\n",ret);
			}


			if(channeltatus.enChanStatus == HI_UNF_DMX_CHAN_CLOSE)
			{
				//printf("status  error  channel already run\n");
			}

			ret = HI_UNF_DMX_CloseChannel(fyf_channel[channel].channelid);
			if (HI_FAILURE == ret)
			{	
				printf("\n +++ HI_UNF_DMX_CloseChannel failure +++\n");
				pthread_mutex_unlock(&demux_mutex);
				return FYF_ERR;
			}
		}
		else
		{
			printf("channel  %d  already stop \n",channel);
		}
	}
	pthread_mutex_unlock(&demux_mutex);
	return FYF_OK;
}
/******************************************************************************/
/*Description: create filter                                 	              */
/*Input      : channel       											      */
/*Output     : filterid                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_create_filter(BU32 channel, BU32 *filter)
{
	BS32 ret;
	BU32 i = 0;
	HI_UNF_DMX_FILTER_ATTR_S struFilter;
	pthread_mutex_lock(&demux_mutex);
	for (i = 1; i < FYF_DEMUX_MAX_FILTER; i++)
	{
		if (fyf_channel[channel].filtergrop[i].filterflag == 0)
		{
			fyf_channel[channel].filtergrop[i].filterflag = 1;
			break;
		}
	}

	if (i == FYF_DEMUX_MAX_FILTER)
	{
		printf("Create filter fail\n");
		pthread_mutex_unlock(&demux_mutex);
		return FYF_ERR;
	}
	struFilter.u32FilterDepth = 0;

	ret = HI_UNF_DMX_NewFilter(fyf_channel[channel].frontendid, &struFilter , &(fyf_channel[channel].filtergrop[i].filter.s32DataFilterID));

	if (ret != HI_SUCCESS)
	{
		printf("HI_UNF_DMX_NewFilter error 0x%x\n", ret);
	}

	*filter = i;
	fyf_channel[channel].filternum++;
	pthread_mutex_unlock(&demux_mutex);
	FYF_CPI_DBG("FYF_API_demux_create_filter %d %d\n", channel, *filter);
	return FYF_OK;
}
/******************************************************************************/
/*Description: set filter                                   	              */
/*Input      : channel,filter,wide,coef,mask,excl						      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_set_filter(BU32 channel, BU32 filter, BU32 wide, BU08 coef[], BU08 mask[], BU08 excl[])
{
	BS32 ret;
	BU32 i = 0;
	FYF_CPI_DBG("FYF_API_demux_set_filter channel = %d, filter = %d\n", channel, filter);

	HI_UNF_DMX_FILTER_ATTR_S struFilter;
	struFilter.u32FilterDepth = wide;
	pthread_mutex_lock(&demux_mutex);

	fyf_channel[channel].filtergrop[filter].filter.u16FilterDepth = wide;

	for (i = 0; i < wide; i++)
	{
		struFilter.au8Match[i] = coef[i];
		struFilter.au8Mask[i] = ~mask[i];
		struFilter.au8Negate[i] = excl[i];
		
		fyf_channel[channel].filtergrop[filter].filter.u8Match[i] = coef[i];
		fyf_channel[channel].filtergrop[filter].filter.u8Mask[i] = mask[i];
		fyf_channel[channel].filtergrop[filter].filter.u8Negate[i] = excl[i];
	
	}
			
	ret = HI_UNF_DMX_SetFilterAttr(fyf_channel[channel].filtergrop[filter].filter.s32DataFilterID, &struFilter); 
	if (HI_SUCCESS != ret)
	{
		printf("\n +++ HI_UNF_DMX_SetFilter ERROR +++\n");
	}
	pthread_mutex_unlock(&demux_mutex);
	return ret;
	
}
/******************************************************************************/
/*Description: destroy filter                                  	              */
/*Input      : channel,filter											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_destroy_filter(BU32 channel, BU32 filter)
{
	BS32 ret;

	FYF_CPI_DBG("FYF_API_demux_destroy_filter channel = %d, filter = %d\n", channel, filter);
	pthread_mutex_lock(&demux_mutex);
	if (fyf_channel[channel].channelflag == 1)
	{
		/* 释放filter资源*/
		ret = HI_UNF_DMX_DeleteFilter(fyf_channel[channel].filtergrop[filter].filter.s32DataFilterID);
		if (HI_SUCCESS != ret)
		{	
			printf("\n +++ HI_UNF_DMX_DeleteFilter failure +++ 0x%x\n", ret);
		}
		
		if (fyf_channel[channel].filternum >= 1)
			fyf_channel[channel].filternum --;
		memset(&(fyf_channel[channel].filtergrop[filter]), 0, sizeof(FYF_FILTER_S));
	}
	else
	{
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!1\n");
		pthread_mutex_unlock(&demux_mutex);
		return FYF_OK;
	}
	pthread_mutex_unlock(&demux_mutex);
	return ret;


}
/******************************************************************************/
/*Description: enable filter                                  	              */
/*Input      : channel,filter											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/  
BS32 FYF_API_demux_enable_filter(BU32 channel, BU32 filter)
{
	BS32 ret;
	
	FYF_CPI_DBG("FYF_API_demux_enable_filter channel = %d, filter = %d\n", channel, filter);
	pthread_mutex_lock(&demux_mutex);
	ret = HI_UNF_DMX_AttachFilter(fyf_channel[channel].filtergrop[filter].filter.s32DataFilterID, fyf_channel[channel].channelid);
	if (HI_SUCCESS != ret)
	{
		printf("\n +++ HI_UNF_DMX_AttachFilter ERROR +++ 0x%x\n", ret);
		printf("channel = %d, filter = %d, pid = %d\n ", channel, filter, fyf_channel[channel].channelpid);
		pthread_mutex_unlock(&demux_mutex);
		return FYF_ERR;
	}
	pthread_mutex_unlock(&demux_mutex);
	return FYF_OK;


}
/******************************************************************************/
/*Description: disable filter                                  	              */
/*Input      : channel,filter											      */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_demux_disable_filter(BU32 channel, BU32 filter)
{
	BS32 ret;

	FYF_CPI_DBG("FYF_API_demux_disable_filter channel = %d, filter = %d\n", channel, filter);
	pthread_mutex_lock(&demux_mutex);
	if (fyf_channel[channel].channelflag == 1)
	{
		/*释放关联关系*/
		ret =  HI_UNF_DMX_DetachFilter(fyf_channel[channel].filtergrop[filter].filter.s32DataFilterID,  fyf_channel[channel].channelid);
		if (HI_SUCCESS != ret)
		{	
			printf("\n +++ HI_UNF_DMX_DetachFilter failure +++0x%x\n", ret);
			printf("channel = %d, filter = %d, pid = %d\n ", channel, filter, fyf_channel[channel].channelpid);
			pthread_mutex_unlock(&demux_mutex);
			return FYF_ERR;
		}
	}
	else
	{
		printf("FYF_API_demux_disable_filter status error  channel %d filter %d \n",channel,filter);
		pthread_mutex_unlock(&demux_mutex);
		return FYF_OK;
	}
	pthread_mutex_unlock(&demux_mutex);
	return FYF_OK;
}


/*******************************************************************************
 * Function:      SectionCmp
 * Description:   

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID -  过滤器ID号        
 *                
 * Output:        
 *                
 * Return:		HI_SUCCESS		是此过滤器数据
 *				HI_FAILURE		不是此过滤器数据
 *            
 * Others:     
 *******************************************************************************/
static HI_S32 SectionCmp(BU32 chnindex, BU32 filterindex, HI_U8 * pu8buffer, HI_U32 u32len) 
{
	HI_S32 i;

	HI_U16 FilterDepth;
	HI_U8  sectionbuffer[SECTION_FILTER_DEPTH_MAX]={0};
	HI_U8  datafiltermatch[SECTION_FILTER_DEPTH_MAX]={0};

	if (HI_NULL == pu8buffer)
	{
		return(HI_FAILURE);
	}

	FilterDepth = fyf_channel[chnindex].filtergrop[filterindex].filter.u16FilterDepth;

	for ( i = 0; i < FilterDepth; i++)
	{
		datafiltermatch[i] = fyf_channel[chnindex].filtergrop[filterindex].filter.u8Match[i] &(fyf_channel[chnindex].filtergrop[filterindex].filter.u8Mask[i]);

		if ( i > 0)
			sectionbuffer[i] = pu8buffer[i+2] &(fyf_channel[chnindex].filtergrop[filterindex].filter.u8Mask[i]);
		else
			sectionbuffer[i] = pu8buffer[i] &(fyf_channel[chnindex].filtergrop[filterindex].filter.u8Mask[i]);

		//printf("|0x%02x  0x%02x", datafiltermatch[i], sectionbuffer[i]);
		
		if (datafiltermatch[i] != sectionbuffer[i])
			return(HI_FAILURE);
	}
	
	return(HI_SUCCESS);
}


static void SectionReadThread(BS32 s32ReadID)
{
	
	HI_S32 	index, i, j, k, m, s32ret = -1 , datafilterid;
		
	HI_U32 	channelid[FYF_DEMUX_MAX_CHANNEL];
	HI_U32	u32ChNum, ChannelID, u32secnum;	
	HI_U32 u32BufLenCnt = 0;
	HI_U32 u32SecLen = 0;
	HI_U8	*pu8buff;

	HI_UNF_DMX_DATA_BUF_S sectionbuffer[SECTION_DATA_BUFFERSIZE];
	
	while(1)
	{
		u32ChNum = FYF_DEMUX_MAX_CHANNEL;
	   	s32ret =  HI_UNF_DMX_GetDataHandle(channelid,  &u32ChNum, 5000);

		if (HI_SUCCESS != s32ret)
		{
			continue;
		}
		//printf("u32ChNum = %d----------------------\n", u32ChNum);
		if (u32ChNum > 0)
		{
			for (index = 0; index < u32ChNum; index ++)
			{
				ChannelID = channelid[index];
			//	printf("channelid = %d\n", ChannelID);

				/*查找通道对应的数据结构*/

				for (i = 1; i < FYF_DEMUX_MAX_CHANNEL; i++)
				{
					if (fyf_channel[i].channelflag && ChannelID == fyf_channel[i].channelid)
						break;
				}

				if (i >= FYF_DEMUX_MAX_CHANNEL) 
				{				
					continue;
				}
	            
						
				u32secnum =0;
				//获取单个通道的BUFF
				
				s32ret = HI_UNF_DMX_AcquireBuf(ChannelID, SECTION_DATA_BUFFERSIZE, &u32secnum, sectionbuffer, 0x7fffffff);
			//	printf("u32secnum = %d\n", u32secnum);
				if (HI_SUCCESS == s32ret)						
				{
					if (u32secnum > SECTION_DATA_BUFFERSIZE)
						u32secnum = SECTION_DATA_BUFFERSIZE;

					for(m = 0; m<u32secnum; m++)
					{
						u32BufLenCnt = 0;
						pu8buff = sectionbuffer[m].pu8Buf;	
						while (u32BufLenCnt < sectionbuffer[m].u32BufLen)
						{
							u32SecLen = ((0x0F & pu8buff[1]) << 8) + pu8buff[2] + 3;				
							
							if (fyf_channel[i].filternum < 1)
							{
	//							u32secnumrelease = u32secnum;
	//							memcpy((void *)sectionrelease, (void *)sectionbuffer, sizeof(HI_UNF_DMX_DATA_BUF_S)*u32secnum);
								break;
							}
							

//	   						printf(" ++ [%d]%d get section len = %d  tableid = %d+++\n",m,u32secnum,u32SecLen,pu8buff[0]);

	   				//		 printf(" ++ [%d]%d get section len = %d  tableid = %d+++\n",m,u32secnum,u32SecLen,pu8buff[0]);

							for (j = 1; j < FYF_DEMUX_MAX_FILTER; j++)
							{
							  
								if (fyf_channel[i].filtergrop[j].filterflag == 1)
								{
									/* 单个过滤条件处理*/
									if (fyf_channel[i].filternum == 1)
									{
									//	printf("fyf_channel[i].filternum = %d\n", fyf_channel[i].filternum);
										if (HI_NULL_PTR != FYFDemuxCallbackFunction)//fyf_channel[i].filtergrop[j].filter.funSectionFunCallback)
										{
											//fyf_channel[i].filtergrop[j].filter.funSectionFunCallback(i, j + 1, pu8buff, u32SecLen);
											FYFDemuxCallbackFunction(i, j, pu8buff, u32SecLen);
										}
										else if(FYFDemuxIpanelCallbackFunction != NULL)
										{
//											printf("FYFDemuxIpanelCallbackFunction!!!\n");
											
							
											FYFDemuxIpanelCallbackFunction(i, j, pu8buff, u32SecLen);

												
										}
							
										//处理完后退出循环jianghaiqi
										break;
									}
									else
									{
										/* 判断数据是否与过滤条件对应*/
										if (HI_SUCCESS == SectionCmp(i, j, pu8buff, u32SecLen))
										{
						
											if (NULL != FYFDemuxCallbackFunction)//fyf_channel[i].filtergrop[j].filter.funSectionFunCallback)
											{
												//fyf_channel[i].filtergrop[j].filter.funSectionFunCallback(i, j + 1, pu8buff, u32SecLen);
												FYFDemuxCallbackFunction(i, j, pu8buff, u32SecLen);
											}
											else if(FYFDemuxIpanelCallbackFunction != NULL)
											{
//												printf("FYFDemuxIpanelCallbackFunction22!!!\n");

									
												FYFDemuxIpanelCallbackFunction(i, j, pu8buff, u32SecLen);

												
											}
											//处理完后退出循环jianghaiqi
											break;
										}
										else
										{
											//printf(" %s %d status error tableid %d len %d\n",__FUNCTION__,__LINE__,sectionbuffer[m].pu8Buf[0],sectionbuffer[m].u32BufLen);
											
											/* 不合过滤器条件*/
										}
									}
								}
							}
						pu8buff += u32SecLen;
                   		u32BufLenCnt += u32SecLen;
						}

					}	
				}

				/* 释放DEMUX 空间*/
				s32ret = HI_UNF_DMX_ReleaseBuf(ChannelID, u32secnum, sectionbuffer);
			}
		
		}
			
		}
}	


/******************************************************************************/
/*Description: tuner module init											  */
/*Input 	 : no         		    										  */
/*Output	 : no															  */
/*Return	 : no                   										  */
/******************************************************************************/
/*cwd 2009-09-02 modify*/
BS32 FYF_API_demux_init(void)
{

	BS32 s32Ret;
	BU32 i;
	BU32 u32DmxId = 0;
	HI_UNF_DMX_PORT_ATTR_S  s_strAttr;
	HI_UNF_DMX_ATTR_S  s_strudemux;
	HI_UNF_DMX_GLB_ATTR_S	pstAttr;
	
	/*demux获取全局属性*/
	HI_UNF_DMX_GetGlbDefaultAttr(&pstAttr);
	/*demux初始化*/
	if (HI_SUCCESS != HI_UNF_DMX_Init(&pstAttr))
	{
		printf("\n +++HI_UNF_DMX_Open failed +++\n");
	}
	/*打开第1路demux*/
	if (HI_SUCCESS != HI_UNF_DMX_Open(u32DmxId))
	{
		printf("HI_UNF_DMX_Open failed\n");
	}

	/*demux设置属性,选择Tuner端口*/
	s_strudemux.u32PortId = 0;
	if (HI_SUCCESS!=HI_UNF_DMX_CloseTSPort(s_strudemux.u32PortId))
	{
		printf("HI_UNF_DMX_CloseTSPort failed\n");
	}
	
	if (HI_SUCCESS != HI_UNF_DMX_SetAttr(u32DmxId , &s_strudemux))
	{
		printf("\n +++ HI_UNF_DMX_SetAttr failed +++\n");
	}
	/*打开Ts端口*/
	if (HI_SUCCESS != HI_UNF_DMX_OpenTSPort(s_strudemux.u32PortId))		
	{
		printf("\n +++ HI_UNF_DMX_OpenTSPort failed +++\n");
	}

	u32DmxId = 1;

    /*打开第2路demux*/
    s32Ret = HI_UNF_DMX_Open(u32DmxId);
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_DMX_Open Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*demux设置属性,选择Tuner端口*/
    s32Ret = HI_UNF_DMX_SetAttr(u32DmxId, &s_strudemux);
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_DMX_SetAttr Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*打开Ts端口*/
    s32Ret = HI_UNF_DMX_OpenTSPort(s_strudemux.u32PortId);
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_DMX_OpenTSPort Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    u32DmxId = 2;

    /*打开第3路demux*/
    s32Ret = HI_UNF_DMX_Open(u32DmxId);
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_DMX_Open Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*demux设置属性,选择Tuner端口*/
    s32Ret = HI_UNF_DMX_SetAttr(u32DmxId, &s_strudemux);
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_DMX_SetAttr Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*打开Ts端口*/
    s32Ret = HI_UNF_DMX_OpenTSPort(s_strudemux.u32PortId);
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_DMX_OpenTSPort Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }
	
	if (FYF_FALSE == fyf_demux_init)
	{
		memset(fyf_channel, 0, FYF_DEMUX_MAX_CHANNEL*sizeof(FYF_CHANNEL_S));
		for (i = 0; i < FYF_DEMUX_MAX_CHANNEL; i++)
		{
			fyf_channel[i].channelpid = 0x1fff;
		}
		fyf_demux_init = FYF_TRUE;
	}

	s32Ret = pthread_create(&s_pSectionReadTaskHandle, NULL, 
			(HI_VOID *)SectionReadThread, NULL);   
	if(s32Ret!=0)   /*  线程创建失败 */
	{
		printf( "pthread_create() falure \n");
		s_pSectionReadTaskHandle = NULL;
		return FYF_ERR;
	}

	
	return HI_SUCCESS;

}

#if 0
BS32 FYF_API_demux_pvr_init(BU32 u32DmxId, HI_UNF_DMX_PORT_MODE_E source, BU16 pid, HI_UNF_DMX_CHAN_TYPE_E chnType)
{
	HI_UNF_DMX_ATTR_S stDmxAttr;
    HI_U32 s32Ret = HI_SUCCESS;
	BU32 *channel_handle;
	DMX_CHAN_HANDLE hChannelHandle;

    if (0 == u32DmxId)
    {
    
    }
    else
    {

		s32Ret = HI_UNF_DMX_GetChannelHandle(u32DmxId, pid, &hChannelHandle);
	    if (s32Ret == HI_SUCCESS)
	    {
	        HI_UNF_DMX_CloseChannel(hChannelHandle);
	        HI_UNF_DMX_DeleteChannel(hChannelHandle);
	        printf("demuxId, = %dhChannelHandle = %x\n", u32DmxId, hChannelHandle);
	    }
		
        HI_UNF_DMX_CHAN_ATTR_S stChnAttr;

        /*demux设置录制类型*/
        s32Ret = HI_UNF_DMX_SetRecodeType(u32DmxId, HI_UNF_DMX_RECORD_DESCRAM_TS);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_SetRecodeType Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }

        /*demux获取通道默认属性*/
        s32Ret = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_GetChannelDefaultAttr Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }

        stChnAttr.enChannelType = chnType;
        stChnAttr.enCRCMode    = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
        stChnAttr.u32BufSize   = 32;
        stChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_REC;
		  /*demux新建通道*/
		if(chnType == HI_UNF_DMX_CHAN_TYPE_AUD)
		{
			channel_handle = &fyf_audio_channelhandle[u32DmxId];
		}
		else if (chnType == HI_UNF_DMX_CHAN_TYPE_VID)
		{
			channel_handle = &fyf_video_channelhandle[u32DmxId];
		}
		s32Ret = HI_UNF_DMX_NewChannel(u32DmxId, &stChnAttr, channel_handle);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_NewChannel Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }

        /*demux设置通道PID*/
        s32Ret = HI_UNF_DMX_SetChannelPID(*channel_handle, pid);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_SetChannelPID Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }
		
	//把通道和密钥区进行绑定
		if (chnType == HI_UNF_DMX_CHAN_TYPE_AUD)
		{
			s32Ret = HI_UNF_DMX_AttachDescrambler(FYF_API_descr_GetAudioDescrambler(u32DmxId), *channel_handle);
			if (HI_SUCCESS != s32Ret)
	        {
	            printf("(%d):HI_UNF_DMX_AttachDescrambler Error,s32Ret=0x%x\n", __LINE__, s32Ret);
	           // return s32Ret;
	        }
		}
		else if (chnType == HI_UNF_DMX_CHAN_TYPE_VID)
		{
			s32Ret = HI_UNF_DMX_AttachDescrambler(FYF_API_descr_GetVideoDescrambler(u32DmxId), *channel_handle);
			if (HI_SUCCESS != s32Ret)
	        {
	            printf("(%d):HI_UNF_DMX_AttachDescrambler Error,s32Ret=0x%x\n", __LINE__, s32Ret);
	           // return s32Ret;
	        }
		}
        /*demux打开通道*/
        s32Ret = HI_UNF_DMX_OpenChannel(*channel_handle);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_OpenChannel Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }
    }

    printf("DMX:%d init OK, sourcetype:%d, pid:%d/%#x, chnTyp:%d\n", u32DmxId, source, pid, pid, chnType);
    return HI_SUCCESS;
}

/*PVR关闭通道*/
BS32 FYF_API_demux_pvr_DeInit(BU32 u32DmxId)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (0 == u32DmxId)
    {
    }
    else
    {
        s32Ret = HI_UNF_DMX_CloseChannel(fyf_video_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_CloseChannel(fyf_audio_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_DetachDescrambler(FYF_API_descr_GetAudioDescrambler(u32DmxId), fyf_audio_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_DetachDescrambler(FYF_API_descr_GetVideoDescrambler(u32DmxId), fyf_video_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_DeleteChannel(fyf_audio_channelhandle[u32DmxId]);
		s32Ret |= HI_UNF_DMX_DeleteChannel(fyf_video_channelhandle[u32DmxId]);
        if (HI_SUCCESS != s32Ret)
        {
            printf("(%d):HI_UNF_DMX_CloseChannel Error,s32Ret=0x%x\n", __LINE__, s32Ret);
            return s32Ret;
        }
    }
    return s32Ret;
}
//PVR录制、播放初始化
BS32 FYF_API_Pvr_Init(void)
{
    BS32 s32Ret = HI_SUCCESS;
    /*PVR录制初始化*/
    s32Ret = HI_UNF_PVR_RecInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_PVR_RecInit Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*PVR播放初始化*/
    s32Ret = HI_UNF_PVR_PlayInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_PVR_PlayInit Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    return s32Ret;
}
//PVR录制、播放去初始化
BS32 FYF_API_Pvr_DeInit(void)
{
    BS32 s32Ret = HI_SUCCESS;
    /*去初始化PVR录制*/
    s32Ret = HI_UNF_PVR_RecDeInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_PVR_RecDeInit  Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }

    /*去初始化PVR播放*/
    s32Ret = HI_UNF_PVR_PlayDeInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("(%d):HI_UNF_PVR_PlayDeInit  Error,s32Ret=0x%x\n", __LINE__, s32Ret);
        return s32Ret;
    }
    return s32Ret;
}
#endif

BS32 FYF_API_PVR_SetDemux(BU32 demuxId)
{
	g_cur_demuxId = demuxId;
}

BU32 FYF_API_PVR_GetDemux()
{
	return g_cur_demuxId;
}

