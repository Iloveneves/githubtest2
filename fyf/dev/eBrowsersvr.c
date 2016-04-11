/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: eBrowersvr.c
 * Description: 
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2008-09-01   diaoqiangwei/d60770                
 ******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hi_type.h"

#include "hi_unf_avplay.h"
#include "hi_unf_vo.h"


#include "sectionsvr_ext.h"
#include "eBrowsersvr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define EBROWSER_DBG //printf


static EBROWSER_CHANNEL_S  ebrowsersvr_channel[EBROWSER_MAX_CHANNEL];
static HI_BOOL	ebrowsersvr_init = HI_FALSE;
HI_U8 ebrowsersvr_channel_buff[EBROWSER_MAX_CHANNEL][EBROWSER_CHANNEL_BUFFLEN];
EBROWSER_CHANNEL_CALLBACK eBrowserSvrCallbackFunction = HI_NULL;

static HI_VOID eBrowserSvr_SectionCallback(HI_S32 s32DataFilterid ,HI_U8 *u8Buffer, HI_U32 u32BufferLength, HI_U8  tableid)
{
	HI_U32 i;
	HI_U32 sec_len;
	HI_U32 u32datalen;
	HI_S8 ret;
    HI_U8 *msg_buf_r = NULL;
    HI_U8 *msg_buf_e = NULL;
	SECTIONSVR_DATA_FILTER_S 	datafilter;
	if(u32BufferLength == 0)
	{
		printf(" +++ zero len +++\n");
		return;
	}
	
//	printf(" +++ eBrowserSvr_SectionCallback %d u32BufferLength %d+++\n",s32DataFilterid,u32BufferLength);
//	for(i=0;i<u32BufferLength;i++)
//	{
//		printf("0x%x ",u8Buffer[i]);
//	}

	
	SECSVR_SectionGetDataFilter(s32DataFilterid,&datafilter);
   	for(i=0;i<EBROWSER_MAX_CHANNEL;i++)
   	{
	   	if(datafilter.u32TSPID == ebrowsersvr_channel[i].channelpid)
			break;
   	}
   
   	if(i>=EBROWSER_MAX_CHANNEL)
   	{
	   	printf(" +++ error +++\n");
	   	return;
   	}		
	 if(ebrowsersvr_channel[i].CallbackFunction)
	{
   		ebrowsersvr_channel[i].CallbackFunction(i,s32DataFilterid,u8Buffer,u32BufferLength);
   		return;
   	}else if(eBrowserSvrCallbackFunction)
	{
   		eBrowserSvrCallbackFunction(i,s32DataFilterid,u8Buffer,u32BufferLength);
   		return ;
   	}

}

HI_S32 eBrowserSvr_Init()
{
	HI_U32 i;

	if(HI_FALSE== ebrowsersvr_init)
	{
		memset(ebrowsersvr_channel, 0, EBROWSER_MAX_CHANNEL*sizeof(EBROWSER_CHANNEL_S));
		for(i = 0;i<EBROWSER_MAX_CHANNEL;i++)
		{
			ebrowsersvr_channel[i].channelpid = 0x1fff;
			memset(ebrowsersvr_channel_buff[i],0,EBROWSER_CHANNEL_BUFFLEN);
		}
		ebrowsersvr_init = HI_TRUE;
	}
	return HI_SUCCESS;
}

HI_S32 eBrowserSvr_AllocateChannel(HI_U32 frontendid,HI_U32 *pChannelId,HI_U32 u32SectionType)
{
	HI_U32 i;

	EBROWSER_DBG(" alloc channel  type = %d \n",u32SectionType);
	
	if(pChannelId == HI_NULL)
		return(HI_FAILURE);	

	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(u32SectionType != 0 && u32SectionType!= 1)
	{
		printf(" alloc channel  type  error = %d \n",u32SectionType);
		return(HI_FAILURE);	
	}	

	if(frontendid > 2)
	{
		printf(" frontend  error = %d \n",frontendid);
		return(HI_FAILURE);		
	}	

	for(i = 0;i<EBROWSER_MAX_CHANNEL;i++)
	{
		if(ebrowsersvr_channel[i].channelflag == EBROWSER_CHANNEL_FREE)
			break;
	}
	
	if(i >= EBROWSER_MAX_CHANNEL)
	{
		printf("\n eBrowser no channel\n");
		return(HI_FAILURE);	
	}

	ebrowsersvr_channel[i].channelflag = EBROWSER_CHANNEL_ALLOCATE;
	ebrowsersvr_channel[i].u32SectionType = u32SectionType;
	ebrowsersvr_channel[i].frontendid = frontendid;

	*pChannelId = i;
	
	return HI_SUCCESS;
}

HI_S32 eBrowserSvr_FreeChannel(HI_U32 ulChannelId)
{
	HI_U32 i;
	EBROWSER_FILTER_S *pfilter;

	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(ulChannelId >= EBROWSER_MAX_CHANNEL) 
	{
		printf("\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_OPEN)
	{
		for(i=0;i<EBROWSER_MAX_FILTER;i++)
		{
			pfilter = &(ebrowsersvr_channel[ulChannelId].filtergrop[i]);

			if(pfilter->filterflag == EBROWSER_FILTER_RUN)
			{
				EBROWSER_DBG("+++ stop filter id %d +++ \n",pfilter->filter.s32DataFilterID);

				if (SECSVR_SectionStopDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
				{
					printf( "\n SECSVR_SectionStopDataFilter FAILURE\n");
					return(HI_FAILURE);
				}
				pfilter->filterflag = EBROWSER_FILTER_ALLOCATE;
			}
		}
		ebrowsersvr_channel[ulChannelId].channelflag = EBROWSER_CHANNEL_PID_SET;	
	}
	
	/* 通道没有打开的时候，可以直接*/
	if(ebrowsersvr_channel[ulChannelId].channelflag > EBROWSER_CHANNEL_FREE)
	{
		for(i=0;i<EBROWSER_MAX_FILTER;i++)
		{
			pfilter = &(ebrowsersvr_channel[ulChannelId].filtergrop[i]);

			if(pfilter->filterflag == EBROWSER_FILTER_RUN)
			{
				EBROWSER_DBG("+++ stop filter id %d +++ \n",pfilter->filter.s32DataFilterID);
	
				if (SECSVR_SectionStopDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
				{
					printf( "\n SECSVR_SectionStopDataFilter FAILURE\n");
					return(HI_FAILURE);
				}
				pfilter->filterflag = EBROWSER_FILTER_ALLOCATE;
			}
			
			if(pfilter->filterflag == EBROWSER_FILTER_ALLOCATE)
			{
				EBROWSER_DBG("+++ free filter id %d +++ \n",pfilter->filter.s32DataFilterID);

				SECSVR_SectionFreeDataFilter(pfilter->filter.s32DataFilterID);
				pfilter->filterflag = EBROWSER_FILTER_FREE;
			}
		}
		memset(&(ebrowsersvr_channel[ulChannelId]),0,sizeof(EBROWSER_CHANNEL_S));
		ebrowsersvr_channel[ulChannelId].channelpid = 0x1fff;
	}

	return HI_SUCCESS;	
	
}

HI_S32 eBrowserSvr_RegisterChannelCallback(HI_U32 ulChannelId,EBROWSER_CHANNEL_CALLBACK CallbackFunction)
{
	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf("\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_FREE)
	{
		printf( "\n eBrowser channel free %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	ebrowsersvr_channel[ulChannelId].CallbackFunction = CallbackFunction;

	return HI_SUCCESS;
	
}

HI_S32 eBrowserSvr_RegisterCallback(EBROWSER_CHANNEL_CALLBACK CallbackFunction)
{
	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	eBrowserSvrCallbackFunction = CallbackFunction;

	return HI_SUCCESS;
	
}


HI_S32 eBrowserSvr_AllocateFilter(HI_U32 ulChannelId,HI_U32 *pFilterId)
{
	HI_U32 i;
	HI_S32 s32filterid;

	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(HI_NULL == pFilterId)
	{
		printf( "\n point error \n");
		return(HI_FAILURE);	
	}
	
	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf("\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	
	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_FREE)
	{
		printf("\n eBrowser channel free %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	for(i = 0;i<EBROWSER_MAX_FILTER;i++)
	{
		if(ebrowsersvr_channel[ulChannelId].filtergrop[i].filterflag == EBROWSER_FILTER_FREE)
			break;
	}
	
	if(i >= EBROWSER_MAX_FILTER)
	{
		printf( "\n eBrowser channe %d no filter\n",ulChannelId);
		return(HI_FAILURE);	
	}

	

	s32filterid = SECSVR_SectionAllocDataFilter();
	if (s32filterid < 0)
	{
		printf("\n eBrowser  alloc filter error\n");
		return(HI_FAILURE);	
	}	

	EBROWSER_DBG(" +++ alloc data filter %d +++ \n",s32filterid);
	//初始化FILTER 数据结构
	memset(&(ebrowsersvr_channel[ulChannelId].filtergrop[i]),0,sizeof(EBROWSER_FILTER_S));
	ebrowsersvr_channel[ulChannelId].filtergrop[i].filter.s32DataFilterID = s32filterid;
	ebrowsersvr_channel[ulChannelId].filtergrop[i].filterflag = EBROWSER_FILTER_ALLOCATE;
	ebrowsersvr_channel[ulChannelId].filtergrop[i].filter.u8UseFlag = 1;
	//修改为返回实际的ID

	*pFilterId = s32filterid;

	return HI_SUCCESS;
}

HI_S32 eBrowserSvr_FreeFilter(HI_U32 ulChannelId,HI_U32 ulFilterId)
{
	EBROWSER_FILTER_S *pfilter;
	HI_U32 u32FilterIndex;

	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf( "\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	/* 通道未分配状态错误*/
	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_FREE)
	{
		printf("\n eBrowser channel free %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	//根据实际的ID 来查找在通道中的虚拟ID
   	for(u32FilterIndex=0;u32FilterIndex<EBROWSER_MAX_FILTER;u32FilterIndex++)
   	{
   		if(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex].filterflag > EBROWSER_FILTER_FREE)
   		{	
	   		if(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex].filter.s32DataFilterID == ulFilterId)
				break;
		}
   	}	

	//查找失败返回错误
	if(u32FilterIndex == EBROWSER_MAX_FILTER)
	{
		printf("\n eBrowser filter id cant find %d\n",ulFilterId);
		return(HI_FAILURE);	
	}

	pfilter = &(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex]);

	if(pfilter->filterflag == EBROWSER_FILTER_FREE)
	{
		return HI_SUCCESS;
	}

	if(pfilter->filterflag == EBROWSER_FILTER_RUN)
	{
		EBROWSER_DBG("+++ stop filter id %d +++ \n",pfilter->filter.s32DataFilterID);
		if (SECSVR_SectionStopDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
		{
			printf("\n SECSVR_SectionStopDataFilter FAILURE\n");
			return(HI_FAILURE);
		}
		pfilter->filterflag = EBROWSER_FILTER_ALLOCATE;
	}

	if(pfilter->filterflag == EBROWSER_FILTER_ALLOCATE)
	{
		EBROWSER_DBG("+++ free filter id %d +++ \n",pfilter->filter.s32DataFilterID);
		SECSVR_SectionFreeDataFilter(pfilter->filter.s32DataFilterID) ;
	}
	//释放需要清除FILTER 内容
	memset(pfilter,0,sizeof(EBROWSER_FILTER_S));
	pfilter->filterflag = EBROWSER_FILTER_FREE;	
	return HI_SUCCESS;	
}

HI_S32 eBrowserSvr_SetFilter(HI_U32 ulChannelId,HI_U32 ulFilterId,HI_U8  *u8Match,HI_U8  *pMask, HI_U8 *u8Negate,HI_U32 ulDataLength)
{
	HI_U32 i;
	EBROWSER_FILTER_S *pfilter;
	HI_U32 u32filterlen;
	HI_U32 u32FilterIndex;
	
	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf("\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_FREE)
	{
		printf("\n eBrowser channel free %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	//根据实际的ID 来查找在通道中的虚拟ID
   	for(u32FilterIndex=0;u32FilterIndex<EBROWSER_MAX_FILTER;u32FilterIndex++)
   	{
   		if(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex].filterflag > EBROWSER_FILTER_FREE)
   		{	
	   		if(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex].filter.s32DataFilterID == ulFilterId)
				break;
		}
   	}	

	//查找失败返回错误
	if(u32FilterIndex == EBROWSER_MAX_FILTER)
	{
		printf("\n eBrowser filter id cant find %d\n",ulFilterId);
		return(HI_FAILURE);	
	}
	
	pfilter = &(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex]);

	if(pfilter->filterflag == EBROWSER_FILTER_FREE)
	{
		printf( "\n eBrowser filter not alloc %d\n",u32FilterIndex);	
		return HI_FAILURE;
	}
	//如果滤波器已经运行，需要停止重新设置在运行
	if(pfilter->filterflag == EBROWSER_FILTER_RUN)
	{
		EBROWSER_DBG("+++ stop filter id %d +++ \n",pfilter->filter.s32DataFilterID);
	
		if (SECSVR_SectionStopDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
		{
			printf("\n SECSVR_SectionStopDataFilter FAILURE\n");
			return(HI_FAILURE);
		}	
	}

	EBROWSER_DBG(" pid = %d\n ",ebrowsersvr_channel[ulChannelId].channelpid);
	EBROWSER_DBG("\n mask ");
	for(i =0;i< ulDataLength ;i++)
	{
	     EBROWSER_DBG("0x%x ",pMask[i]);
	}
	EBROWSER_DBG("\n match ");
	for(i =0;i< ulDataLength ;i++)
	{
	     EBROWSER_DBG("0x%x ",u8Match[i]);
	}

//	u32filterlen = (ulDataLength > 2)?(ulDataLength -2):1;

	
	pfilter->filter.u16FilterDepth = ulDataLength;

//	pfilter->filter.u8Mask[0] = ~(HI_U8)(*(pMask)) ;
//   	pfilter->filter.u8Match[0] = (HI_U8)(*(u8Match)); 
		
	for(i =0;i< ulDataLength ;i++)
	{
		pfilter->filter.u8Mask[i] = ~pMask[i];
		pfilter->filter.u8Match[i] = u8Match[i];
		pfilter->filter.u8Negate[i] = u8Negate[i];
	}
	
	for(;i<SECTION_FILTER_DEPTH_MAX;i++)
	{
		pfilter->filter.u8Mask[i] = 0xff;
	}
	
	if(pfilter->filterflag == EBROWSER_FILTER_RUN)
	{
		pfilter->filter.u32TSPID = ebrowsersvr_channel[ulChannelId].channelpid;

		/* 设置数据过滤器*/
		if (SECSVR_SectionSetDataFilter(pfilter->filter.s32DataFilterID, &(pfilter->filter)) == HI_FAILURE)
		{
			printf( "\n SECTIONSVR_SetDataFilter FAILURE\n");
			return(HI_FAILURE);
		}
		EBROWSER_DBG("+++ start filter id %d +++ \n",pfilter->filter.s32DataFilterID);

		/* 启动数据过滤*/
		if (SECSVR_SectionStartDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
		{
			printf( "\n SECSVR_SectionStartDataFilter FAILURE\n");
			return(HI_FAILURE);
		}
	}
	return HI_SUCCESS;
}

HI_S32 eBrowserSvr_EnableFilter(HI_U32 ulChannelId,HI_U32 ulFilterId)
{
	HI_U32 i;
	EBROWSER_FILTER_S *pfilter;
	HI_U32 u32filterlen;
	HI_U32 u32FilterIndex;
	
	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf("\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	//通道没有设置好，不能直接启动滤波器
	if(ebrowsersvr_channel[ulChannelId].channelflag < EBROWSER_CHANNEL_PID_SET)
	{
		printf("\n eBrowser channel free %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	//根据实际的ID 来查找在通道中的虚拟ID
   	for(u32FilterIndex=0;u32FilterIndex<EBROWSER_MAX_FILTER;u32FilterIndex++)
   	{
   		if(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex].filterflag > EBROWSER_FILTER_FREE)
   		{	
	   		if(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex].filter.s32DataFilterID == ulFilterId)
				break;
		}
   	}	

	//查找失败返回错误
	if(u32FilterIndex == EBROWSER_MAX_FILTER)
	{
		printf("\n eBrowser filter id cant find %d\n",ulFilterId);
		return(HI_FAILURE);	
	}
	//对滤波器进行分析，是否满足启动要求
	pfilter = &(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex]);

	if(pfilter->filterflag == EBROWSER_FILTER_FREE)
	{
		printf("\n eBrowser filter not alloc %d\n",u32FilterIndex);	
		return HI_FAILURE;
	}
	
	if(pfilter->filterflag == EBROWSER_FILTER_RUN)
	{
		EBROWSER_DBG("+++ filter is already start %d +++ \n",pfilter->filter.s32DataFilterID);
		return HI_SUCCESS;
	}
	//设置滤波器参数
	
	pfilter->filter.frontendid			= ebrowsersvr_channel[ulChannelId].frontendid;
	pfilter->filter.u8DirTransFlag	= 1;//透传方式传送
	pfilter->filter.u8UseFlag 		= 1;
	pfilter->filter.u8RunFlag			= 0;
	pfilter->filter.u32TimeOut 		= EBROWSER_TIMEOUT;
	pfilter->filter.u32TSPID 		= ebrowsersvr_channel[ulChannelId].channelpid;


	/* 设置回调函数*/	
	pfilter->filter.funReadyFunCallback 	= HI_NULL;
	pfilter->filter.funSectionFunCallback 	= &eBrowserSvr_SectionCallback;
	pfilter->filter.funTableFunCallback 	= HI_NULL;	

	/* 设置数据过滤器*/
	if (SECSVR_SectionSetDataFilter(pfilter->filter.s32DataFilterID, &(pfilter->filter)) == HI_FAILURE)
	{
		printf( "\nSECTIONSVR_SetDataFilter FAILURE\n");
		return(HI_FAILURE);
	}

	/* 启动数据过滤*/
	if (SECSVR_SectionStartDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
	{
		printf("\n SECSVR_SectionStartDataFilter FAILURE\n");
		return(HI_FAILURE);
	}

	pfilter->filterflag = EBROWSER_FILTER_RUN;
	
	return HI_SUCCESS;
}

HI_S32 eBrowserSvr_DisableFilter(HI_U32 ulChannelId,HI_U32 ulFilterId)
{
	HI_U32 i;
	EBROWSER_FILTER_S *pfilter;
	HI_U32 u32filterlen;
	HI_U32 u32FilterIndex;
	
	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf( "\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_FREE)
	{
		printf("\n eBrowser channel free %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	//根据实际的ID 来查找在通道中的虚拟ID
   	for(u32FilterIndex=0;u32FilterIndex<EBROWSER_MAX_FILTER;u32FilterIndex++)
   	{
   		if(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex].filterflag > EBROWSER_FILTER_FREE)
   		{	
	   		if(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex].filter.s32DataFilterID == ulFilterId)
				break;
		}
   	}	

	//查找失败返回错误
	if(u32FilterIndex == EBROWSER_MAX_FILTER)
	{
		printf("\n eBrowser filter id cant find %d\n",ulFilterId);
		return(HI_FAILURE);	
	}
	
	pfilter = &(ebrowsersvr_channel[ulChannelId].filtergrop[u32FilterIndex]);

	if(pfilter->filterflag == EBROWSER_FILTER_FREE)
	{
		printf("\n eBrowser filter not alloc %d\n",u32FilterIndex);	
		return HI_FAILURE;
	}
	
	if(pfilter->filterflag == EBROWSER_FILTER_ALLOCATE)
	{
		EBROWSER_DBG("+++ filter is already stop %d +++ \n",pfilter->filter.s32DataFilterID);
		return HI_SUCCESS;
	}

	if (SECSVR_SectionStopDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
	{
		printf("\n SECSVR_SectionStopDataFilter FAILURE\n");
		return(HI_FAILURE);
	}

	pfilter->filterflag = EBROWSER_FILTER_ALLOCATE;
	
	return HI_SUCCESS;
}

HI_S32 eBrowserSvr_SetChannelPid(HI_U32 ulChannelId,HI_U16 usPid)
{
	HI_U32 i;

	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf( "\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_FREE||
		ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_OPEN)
	{
		printf( "\n eBrowser channel free %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	for(i=0;i<EBROWSER_MAX_CHANNEL;i++)
	{
		if(ulChannelId == i)
			continue;

		if(ebrowsersvr_channel[ulChannelId].channelpid == usPid)
			break;
	}

	if(i < EBROWSER_MAX_CHANNEL)
	{
		printf( "\n eBrowser channel %d same to %d\n",ulChannelId,i);
		return(HI_FAILURE);	
	}

	ebrowsersvr_channel[ulChannelId].channelflag = EBROWSER_CHANNEL_PID_SET;

	ebrowsersvr_channel[ulChannelId].channelpid = usPid;
	return HI_SUCCESS;
}

HI_S32 eBrowserSvr_GetChannelByPid(HI_U32 *ulChannelId,HI_U16 usPid)
{
	HI_U32 i;

	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	

	if(ulChannelId == 0)
		return(HI_FAILURE);	

	for(i=0;i<EBROWSER_MAX_CHANNEL;i++)
	{
		if(ebrowsersvr_channel[i].channelflag >= EBROWSER_CHANNEL_PID_SET)
		{	
			if(ebrowsersvr_channel[i].channelpid == usPid)
				break;
		}
	}

	if(i == EBROWSER_MAX_CHANNEL)
	{
		*ulChannelId = i;
		return HI_SUCCESS;
	}	
	printf(" +++ eBrowser channel cant find pid %x \n");
	return(HI_FAILURE);	
}

HI_S32 eBrowserSvr_OpenChannel(HI_U32 ulChannelId)
{
	HI_U32 i;
	EBROWSER_FILTER_S *pfilter;
	HI_S32 ret = HI_FAILURE;

	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	
	
	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf("\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}
	if(ebrowsersvr_channel[ulChannelId].channelflag < EBROWSER_CHANNEL_PID_SET)
	{
		printf( "\n eBrowser channel not set pid %d\n",ulChannelId);
		return(HI_FAILURE);	
	}
	
	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_OPEN)
		return HI_SUCCESS;

	for(i=0;i<EBROWSER_MAX_FILTER;i++)
	{
		pfilter = &(ebrowsersvr_channel[ulChannelId].filtergrop[i]);
		if(pfilter->filterflag == EBROWSER_FILTER_ALLOCATE)
		{
			pfilter->filter.frontendid			= ebrowsersvr_channel[ulChannelId].frontendid;
			pfilter->filter.u8DirTransFlag	= 1;//透传方式传送
			pfilter->filter.u8UseFlag 		= 1;
			pfilter->filter.u8RunFlag			= 0;
			pfilter->filter.u32TimeOut 		= EBROWSER_TIMEOUT;
			pfilter->filter.u32TSPID 		= ebrowsersvr_channel[ulChannelId].channelpid;
	

			/* 设置回调函数*/	
			pfilter->filter.funReadyFunCallback 	= HI_NULL;
			pfilter->filter.funSectionFunCallback 	= &eBrowserSvr_SectionCallback;
			pfilter->filter.funTableFunCallback 	= HI_NULL;	

			/* 设置数据过滤器*/
			if (SECSVR_SectionSetDataFilter(pfilter->filter.s32DataFilterID, &(pfilter->filter)) == HI_FAILURE)
			{
				printf("\nSECTIONSVR_SetDataFilter FAILURE\n");
				return(HI_FAILURE);
			}
			EBROWSER_DBG("+++ start filter id %d +++ \n",pfilter->filter.s32DataFilterID);

			/* 启动数据过滤*/
			if (SECSVR_SectionStartDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
			{
				printf( "\n SECSVR_SectionStartDataFilter FAILURE\n");
				return(HI_FAILURE);
			}
			
			pfilter->filterflag = EBROWSER_FILTER_RUN;
			ret = HI_SUCCESS;
		}
		else if(pfilter->filterflag == EBROWSER_FILTER_RUN)
		{
			EBROWSER_DBG(" open channel filter %d already run \n",i);
			ret = HI_SUCCESS;
		}	
	}
	
	ebrowsersvr_channel[ulChannelId].channelflag = EBROWSER_CHANNEL_OPEN;
	if(ret == HI_FAILURE)
	{
		printf(" open channel no filter alloc \n"); 
	}	
	return HI_SUCCESS;
	
}

HI_S32 eBrowserSvr_CloseChannel(HI_U32 ulChannelId)
{
	HI_U32 i;
	EBROWSER_FILTER_S *pfilter;
	
	if(HI_FALSE== ebrowsersvr_init)
		return(HI_FAILURE);	
	if(ulChannelId >= EBROWSER_MAX_CHANNEL)
	{
		printf("\n eBrowser channel id error %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	if(ebrowsersvr_channel[ulChannelId].channelflag == EBROWSER_CHANNEL_FREE)
	{
		printf( "\n eBrowser channel free %d\n",ulChannelId);
		return(HI_FAILURE);	
	}

	if(ebrowsersvr_channel[ulChannelId].channelflag < EBROWSER_CHANNEL_OPEN)
	{
		return(HI_SUCCESS);	
	}

	for(i=0;i<EBROWSER_MAX_FILTER;i++)
	{
		pfilter = &(ebrowsersvr_channel[ulChannelId].filtergrop[i]);

		if(pfilter->filterflag == EBROWSER_FILTER_RUN)
		{
			EBROWSER_DBG("+++ stop filter id %d +++ \n",pfilter->filter.s32DataFilterID);
			if (SECSVR_SectionStopDataFilter(pfilter->filter.s32DataFilterID) == HI_FAILURE)
			{
				printf( "\n SECSVR_SectionStopDataFilter FAILURE\n");
				return(HI_FAILURE);
			}
			pfilter->filterflag = EBROWSER_FILTER_ALLOCATE;
		}
	}
	ebrowsersvr_channel[ulChannelId].channelflag = EBROWSER_CHANNEL_PID_SET;
	return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

