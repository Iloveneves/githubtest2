/*******************************************************************************
 *              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: sectionsvr.c
 * Description: 
 *DVB application software. This module is for section receiving function. 

 * History:
 * Version   Date             Author         defectNum           Description
 * main1     2008-09-01   diaoqiangwei/d60770                
 *******************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>
#include <memory.h>
#include <malloc.h>
#include <time.h>
#include <sys/time.h>

#include "hi_type.h"

#include "hi_unf_demux.h"

#include "stbpetimer.h"

#include "sectionsvr.h"
#include "sectionsvr_ext.h"

#include "hi_msg_queue.h"

#define SECTION_CHECK_TIMER           	(1000)
#define MESSAGE_SECTIONSVR_SECTION	0
#define MESSAGE_SECTIONSVR_TIMEOUT 1
#define MESSAGE_SECTIONSVR_TABLEFINISHED 2
#define EIT_TABLE_ID_SCHEDULE_ACTUAL_LOW    0x50
#define EIT_TABLE_ID_SCHEDULE_ACTUAL_HIGH  0x5F
#define EIT_TABLE_ID_SCHEDULE_OTHER_LOW    0x60
#define EIT_TABLE_ID_SCHEDULE_OTHER_HIGH  0x6F
#define TDT_TABLE_ID 0x70
#define TOT_TABLE_ID 0x73

#define SECTION_DBG  //printf


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

/* timer */
static HI_VOID* s_pTimerId = HI_NULL;
static pthread_t  s_TimerTaskId = HI_NULL;

/* SECTION数据资源*/
static SECTIONSVR_SECTIONDATA_S 	s_struChannelSection[SECTION_DATA_CHANNEL_SIZE];

/* 过滤器资源*/
static SECTIONSVR_DATA_FILTER_S 	g_struDataFilterResource[SECTION_DATA_FILTER_SIZE];                  /*  全局的数据过滤器资源，过滤器id初始化为-1 */

/* 任务*/
static pthread_t s_pSectionSvrTaskHandle = HI_NULL;	
static pthread_t s_pSectionReadTaskHandle = HI_NULL;	

/* timer */
static petimer_t  s_pSectionTimerHandle = HI_NULL;	

/* 消息*/
static HI_MsgQueue_p  s_pSvrMsgQueue = HI_NULL;	

/*******************************************************************************
 * Function:      SECSVR_SectionCreateTask
 * Description:   创建SECTIONSVR主任务，在APPMNGSVR启动时，将SECTIONSVR主任务启动
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    HI_FAILURE 失败
 *            >=0 任务ID
 * Others:     
 *******************************************************************************/
HI_S32  SECSVR_SectionCreateTask(HI_VOID) 
{
	
	HI_S32 s32Ret;

	SECTION_DBG("*****SVR_SectionCreateTask()**** \n");

	/*判断SECTON 模块是否已经启动过*/
	if( HI_NULL != s_pSectionSvrTaskHandle)
	{
		printf( "section main thread is allready in running \n");
		return HI_FAILURE;
	}


	/*初始化数据过滤器资源*/
	s32Ret = SECSVR_SectionInitDataFilter( );
	if(s32Ret!=0)   /*  初始化失败 */
	{
		printf("SectionInitDataFilter() falure \n");
		return HI_FAILURE;
	}

	
	s_pSvrMsgQueue =  HI_OSMsgQueueCreate(sizeof(MSG_MESSAGE_S), 500);
	if (HI_NULL == s_pSvrMsgQueue)
	{
		printf( " msg create failed!\n");
		return HI_FAILURE;
	}


	s32Ret = pthread_create(&s_pSectionReadTaskHandle, NULL, 
	/*&sectionthread_attribute,*/ 
			(HI_VOID *)SectionReadThread, HI_NULL);   
	if(s32Ret!=0)   /*  线程创建失败 */
	{
		HI_OSMsgQueueDel(s_pSvrMsgQueue);
		
		printf( "pthread_create() falure \n");
		s_pSectionReadTaskHandle = HI_NULL;
		return HI_FAILURE;
	}


	/*启动SECTIONSVR主线程*/
	s32Ret = pthread_create(&s_pSectionSvrTaskHandle, HI_NULL, 
			(HI_VOID *)&SectionMainThread, HI_NULL);   /*启动SECTION 主线程*/
	if(s32Ret!=0)   /*  线程创建失败 */
	{
		printf("pthread_create() falure \n");
		s_pSectionSvrTaskHandle = HI_NULL;
		return HI_FAILURE;
	}


	s32Ret = SectionStartTimer();
	
	if ( HI_FAILURE == s32Ret )
	{   
		printf( " SectionStartTimer failed!\n");

		HI_OSMsgQueueDel(s_pSvrMsgQueue);
		
		pthread_cancel(s_pSectionSvrTaskHandle);
		pthread_cancel(s_pSectionReadTaskHandle);

		s_pSectionSvrTaskHandle = HI_NULL;
		s_pSectionReadTaskHandle = HI_NULL;
		
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

/*******************************************************************************
 * Function:      SECSVR_SectionTaskReady
 * Description:   
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    
 *            
 * Others:     
 *******************************************************************************/
HI_BOOL SECSVR_SectionTaskReady(HI_VOID)
{
	if( HI_NULL != s_pSectionSvrTaskHandle )
	{
		return HI_TRUE;
	}
	else
	{
		return HI_FALSE;
	}
}

/*******************************************************************************
 * Function:      SECSVR_SectionDestroyTask
 * Description:   销毁SECTIONSVR主任务：停止子任务、销毁资源，由APPMNGSVR调用
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    HI_FAILURE 失败
 *            0 成功
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionDestroyTask(HI_VOID)
{
	/*给SECTIONSVR主线程发结束运行请求*/

	printf("SVR_SectionDestroyTask() not reallized \n"); /*  fixme , not reallize */

	HI_OSMsgQueueDel(s_pSvrMsgQueue);
	s_pSvrMsgQueue = HI_NULL;

	pthread_cancel(s_pSectionReadTaskHandle);
	pthread_cancel(s_pSectionSvrTaskHandle);

	s_pSectionReadTaskHandle = HI_NULL;
	s_pSectionSvrTaskHandle	 = HI_NULL;

	return HI_SUCCESS;
}


/*******************************************************************************
 * Function:      SectionInitDataFilter
 * Description:   数据过滤器初始化
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    HI_FAILURE 失败
 *            =0 成功
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionInitDataFilter(HI_VOID)
{
	HI_S32 i, j;


	/*初始化全局变量g_struDataFilterResource */
	for(i = 0; i < SECTION_DATA_FILTER_SIZE; i++)
	{
		
		g_struDataFilterResource[i].funReadyFunCallback = HI_NULL;
		g_struDataFilterResource[i].funSectionFunCallback = HI_NULL;
		g_struDataFilterResource[i].funTableFunCallback = HI_NULL;
		
		g_struDataFilterResource[i].s32DataFilterID = -1;
		g_struDataFilterResource[i].u32TSPID = 0xffffffff;
		g_struDataFilterResource[i].u16FilterDepth= 0xffff;

		g_struDataFilterResource[i].u8UseFlag = 0;
		g_struDataFilterResource[i].u8RunFlag = 0;
		g_struDataFilterResource[i].u8DirTransFlag = 0;
		g_struDataFilterResource[i].u8SectionType = 0;
		
		g_struDataFilterResource[i].u32TimeOut = 0;
	}


	for (i = 0; i < SECTION_DATA_CHANNEL_SIZE; i++) 
	{
		s_struChannelSection[i].u16frontendid = 0xffff;
		s_struChannelSection[i].u32TSPID = 0xffffffff;
		s_struChannelSection[i].u32channelID = 0xffffffff;
		
		s_struChannelSection[i].u8Runflag = 0;
		s_struChannelSection[i].u32filternum = 0;
		s_struChannelSection[i].u8Channeltype = 0;

		for (j = 0; j< SECTION_DATA_CHFILTER_MAX; j++)
		{
			memset(&s_struChannelSection[i].u32filtermatrix[j], 0, sizeof(SECTIONSVR_FILTER_S));
			
			s_struChannelSection[i].u32filtermatrix[j].s32datafilterID = -1;
		}
	}

	return HI_SUCCESS;
}

/*******************************************************************************
 * Function:      HI_SVR_SectionAllocDataFilterId
 * Description:   数据过滤器申请，从数据过滤器资源中分配过滤器，返回已分配过滤器的ID号。
 在过滤table 时首先要申请数据过滤器，而后再设置数据过滤器、再启动过滤
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    HI_FAILURE 失败
 *            >=0 数据过滤器ID
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionAllocDataFilter(HI_VOID)
{
	HI_S32 i;

	/*检查是否还有剩余的data filter 资源*/
	for(i = 0; i < SECTION_DATA_FILTER_SIZE; i++)
	{
		if(  0  == g_struDataFilterResource[i].u8UseFlag )
		{
			g_struDataFilterResource[i].s32DataFilterID = i;
			g_struDataFilterResource[i].u8UseFlag= 1;
			g_struDataFilterResource[i].u8RunFlag= 0;
			return(i);   /*  返回channel 的索引 */
		}
	}

	return HI_FAILURE;
}

/*******************************************************************************
 * Function:      SVR_SectionFreeDataFilter
 * Description:   释放申请的数据过滤器

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID -  过滤器ID号        
 *                
 * Output:        
 *                
 * Return:		none    
 *            
 * Others:     
 *******************************************************************************/
HI_VOID SECSVR_SectionFreeDataFilter(HI_S32 s32DataFilterID)
{
	/*检查输入参数是否合法*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		return; 
	} 

	/*检查指定资源是否已经分配*/
	if (g_struDataFilterResource[s32DataFilterID].u8UseFlag == 1 &&
		g_struDataFilterResource[s32DataFilterID].u8RunFlag ==0)
	{
		g_struDataFilterResource[s32DataFilterID].funReadyFunCallback = HI_NULL;
		g_struDataFilterResource[s32DataFilterID].funSectionFunCallback = HI_NULL;
		g_struDataFilterResource[s32DataFilterID].funTableFunCallback = HI_NULL;
		
		g_struDataFilterResource[s32DataFilterID].s32DataFilterID = -1;
		g_struDataFilterResource[s32DataFilterID].u32TSPID = 0xffffffff;
		g_struDataFilterResource[s32DataFilterID].u16FilterDepth= 0xffff;

		g_struDataFilterResource[s32DataFilterID].u8UseFlag = 0;
		g_struDataFilterResource[s32DataFilterID].u8RunFlag = 0;
		g_struDataFilterResource[s32DataFilterID].u8DirTransFlag = 0;
		g_struDataFilterResource[s32DataFilterID].u8SectionType = 0;
		
		g_struDataFilterResource[s32DataFilterID].u32TimeOut = 0;
	}
	return;
}

/*******************************************************************************
 * Function:      SVR_SectionSetDataFilter
 * Description:   设置数据过滤器，需要在申请到数据过滤资源后调用

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID -  过滤器ID号        
 *        pstruDataFilter 过滤器信息        
 * Output:        
 *                
 * Return:    HI_FAILURE 失败
 *            0 成功
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionSetDataFilter(HI_S32 s32DataFilterID, SECTIONSVR_DATA_FILTER_S  *pstruDataFilter)
{

	SECTION_DBG( "*****SVR_SectionSetDataFilter()**** \n");

	/*检查输入参数是否合法*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		printf("invalid datafilter id \n");
		return(HI_FAILURE); 
	} 

	if(HI_NULL == pstruDataFilter)
	{
		printf("invalid data filter \n");
		return( HI_FAILURE);
	}

	/*检查指定资源是否已经分配*/
	if (1 != g_struDataFilterResource[s32DataFilterID].u8UseFlag)
	{
		printf("invalid data filter \n");
		return( HI_FAILURE);	
	}


	/*设置指定的数据过滤器资源*/
	memcpy(&g_struDataFilterResource[s32DataFilterID], pstruDataFilter, sizeof(SECTIONSVR_DATA_FILTER_S));

	return (HI_SUCCESS);
}


/*******************************************************************************
 * Function:      SVR_SectionGetDataFilter
 * Description:   得到s32DataFilterID对应的数据过滤器

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID -  过滤器ID号        
 *        pstruDataFilter 过滤器指针        
 * Output:        
 *                
 * Return:    HI_FAILURE 失败
 *            0 成功
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionGetDataFilter (HI_S32 s32DataFilterID, SECTIONSVR_DATA_FILTER_S    *pstruDataFilter)
{

//	printf("*****SVR_SectionGetDataFilter()**** \n");

	/*检查输入参数是否合法*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		printf("invalid datafilter id \n");
		return(HI_FAILURE); 
	} 

	/*检查输入参数是否合法*/
	if(HI_NULL == pstruDataFilter)
	{
		printf( "invalid data filter pointer \n");
		return(HI_FAILURE);
	}

	/*检查指定资源是否已经分配*/

	/*返回指定的过滤器指针*/
	memcpy(pstruDataFilter, &g_struDataFilterResource[s32DataFilterID], sizeof(SECTIONSVR_DATA_FILTER_S) );

	return( HI_SUCCESS);

}

/*******************************************************************************
 * Function:      SVR_SectionStartDataFilter
 * Description:   应用调用，给SECTIONSRV主任务启动数据过滤，
 需要在设置数据过滤器之后调用

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID过滤器ID号        
 *               
 * Output:        
 *                
 * Return:    HI_FAILURE 失败
 *            0 成功
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionStartDataFilter(HI_S32 s32DataFilterID)
{
	MSG_MESSAGE_S *pstruMsg;  /*向主任务发送的消息*/ 

	/*检查输入参数是否合法*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		return (HI_FAILURE); 
	} 

	/* 设置运行标记*/
	g_struDataFilterResource[s32DataFilterID].u8RunFlag = 1;

	SectionStart(s32DataFilterID);

	
	return (HI_SUCCESS);
}

/*******************************************************************************
 * Function:      SVR_SectionStopDataFilter
 * Description:   应用给SECTIONSRV主任务发消息停止数据过滤器任务

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID过滤器ID号        
 *               
 * Output:        
 *                
 * Return:    HI_FAILURE 失败
 *            0 成功
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionStopDataFilter(HI_S32 s32DataFilterID)
{
	MSG_MESSAGE_S *pstruMsg;  /*向主任务发送的消息*/ 

	HI_MsgQueue_p Queue;	

	SECTION_DBG( "*****SVR_SectionStopDataFilter()**** \n");

	/*检查输入参数是否合法*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		printf( "invalid datafilter id \n");
		return(HI_FAILURE); 
	} 

	/* 设置停止标记*/
	g_struDataFilterResource[s32DataFilterID].u8RunFlag = 0;
	
	SectionStop(s32DataFilterID);

	g_struDataFilterResource[s32DataFilterID].u32TSPID = 0xffffffff;
	
	return( HI_SUCCESS);

}


/*****************************************************************************
 * Function:      SectionTimerProc
 * Description:   tuner timer proc, send timer msg to tunersvr task or sectionsvr task
 * Data Accessed:None
 * Data Updated: s_u32TunerTimerCount
 * Input:             None
 * Output:           None
 * Return:  None
 * Others:     None
 *****************************************************************************/
static HI_VOID SectionTimerProc(HI_VOID *pArg)
{
	HI_S32 s32Ret = HI_FAILURE;
	MSG_MESSAGE_S  *pstruMsg= HI_NULL;

	if( HI_NULL == pArg )
	{
		printf("SectionTimerProc arg null\n");
		return;
	}

	switch(((HI_CHAR *)pArg)[0])
	{
		case 'b':
			{
				if (HI_NULL == s_pSvrMsgQueue )
				{
					printf(" s_pTunerMsgQueue is null\n");
					return;
				}

				/* 向SECTIONSVR主任务发消息*/
				pstruMsg = (MSG_MESSAGE_S *) HI_OSMsgClaim(s_pSvrMsgQueue, 0);
				
				if (HI_NULL == pstruMsg  )
				{
					printf( "HI_OSMsgClaim falure \n");
					return;
				}
				
				pstruMsg->u32MessageID = MESSAGE_SECTIONSVR_TIMEOUT;
				pstruMsg->s32Para1 = 0;
				pstruMsg->s32Para2 = 0;
				pstruMsg->s32Para3 = 0;
				
				HI_OSMsgSend(s_pSvrMsgQueue, pstruMsg);

				break;
			}
		default:
			{
				break;
			}
	}
}




static HI_S32 SectionStartTimer(HI_VOID)
{
	HI_S32 s32Ret = HI_FAILURE;

	s32Ret = stb_petimer_start(&s_TimerTaskId, HI_NULL, &s_pTimerId);
	if ( HI_SUCCESS != s32Ret )
	{
		printf("  timer start error\n");      
		return HI_FAILURE;
	}
	
	/*create tuner timer */
	s_pSectionTimerHandle = stb_petimer_create_reltimer(SectionTimerProc,  PETF_AUTO_RESTART);
	if (HI_NULL == s_pSectionTimerHandle)
	{
		printf(" section timer create error\n");      
		return HI_FAILURE;
	}

	/* debug */
	s32Ret = stb_petimer_start_reltimer(s_pTimerId, s_pSectionTimerHandle, SECTION_CHECK_TIMER, (HI_VOID *)"b");
	if ( HI_SUCCESS != s32Ret )
	{
		printf( " section timer start error\n");      
		return HI_FAILURE;
	}
	return HI_SUCCESS;
}



static HI_VOID SectionStart(HI_S32 s32DataFilterIndex) 
{
	HI_U8    u8addflag = 0;
	HI_S32  s32ret, i, j, m;
	HI_U32  u32channelid, u32fileterid;

	HI_UNF_DMX_CHAN_ATTR_S struChan;
	HI_UNF_DMX_FILTER_ATTR_S struFilter;

	if ((g_struDataFilterResource[s32DataFilterIndex].u32TSPID == 0xffffffff) ||
		(g_struDataFilterResource[s32DataFilterIndex].s32DataFilterID == -1) )
	{
		return;
	}

	u8addflag = 0;
	
	/* 判断数据过滤器TSPID是否相同*/
	for (i =0; i< SECTION_DATA_CHANNEL_SIZE; i++)
	{
		//相同的DEMUX里不能有相同的PID
		if ((s_struChannelSection[i].u32TSPID == g_struDataFilterResource[s32DataFilterIndex].u32TSPID)
			&& (s_struChannelSection[i].u16frontendid == g_struDataFilterResource[s32DataFilterIndex].frontendid))
		{
			break;				
		}	
	}

	/* 增加*/
	if (i == SECTION_DATA_CHANNEL_SIZE)
	{
		for (i=0; i< SECTION_DATA_CHANNEL_SIZE; i++)
		{
			if ((s_struChannelSection[i].u32TSPID == 0xffffffff)&&(s_struChannelSection[i].u8Runflag == 0))
			{
				s_struChannelSection[i].u32TSPID =  g_struDataFilterResource[s32DataFilterIndex].u32TSPID;
				s_struChannelSection[i].u16frontendid = g_struDataFilterResource[s32DataFilterIndex].frontendid;

				s_struChannelSection[i].u32filternum = 1;
				
				s_struChannelSection[i].u8Runflag = 1;

				j = 0;
				
				s_struChannelSection[i].u32filtermatrix[j].u8Runflag = 1;
				s_struChannelSection[i].u32filtermatrix[j].s32datafilterID = s32DataFilterIndex;

				u8addflag = 1;
				break;
			}
		}
		if(i >= SECTION_DATA_CHANNEL_SIZE)
		{
			printf(" +++ no free channel +++ \n");
			return;	
		}	
	}
	else
	{
		s_struChannelSection[i].u32filternum ++;
		
		for (j=0; j< SECTION_DATA_CHANNEL_SIZE; j++)
		{
			if (s_struChannelSection[i].u32filtermatrix[j].u8Runflag ==0)
			{
				s_struChannelSection[i].u32filtermatrix[j].u8Runflag = 1;
				s_struChannelSection[i].u32filtermatrix[j].s32datafilterID = s32DataFilterIndex;

				break;
			}
		}
		if(j >= SECTION_DATA_CHFILTER_MAX)
		{
			printf(" +++ channel %d no free filter can attach +++ \n",i);
			return;	
		}	

	}

	/* 新增CHANNEL */	
	if (u8addflag ==1)
	{
		s32ret = HI_UNF_DMX_GetChannelDefaultAttr(&struChan);
		if (HI_SUCCESS != s32ret)
		{		
			return;			
		}
		/*如果是PES类型的通道*/
		if(g_struDataFilterResource[i].u8SectionType == 1)
		{
			struChan.enChannelType = HI_UNF_DMX_CHAN_TYPE_PES;
			struChan.u32BufSize = 32;
			struChan.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
		}	

		struChan.u32BufSize = 512;

		/*分配channel资源*/
		s32ret = HI_UNF_DMX_NewChannel(s_struChannelSection[i].u16frontendid, &struChan, &u32channelid);
		if (HI_SUCCESS != s32ret)
		{
			printf("\n +++ HI_UNF_DMX_NewChannel ERROR +++\n");
			return;	
		}

		s32ret = HI_UNF_DMX_SetChannelPID(u32channelid, s_struChannelSection[i].u32TSPID);
		if (HI_SUCCESS != s32ret)
		{
			printf("\n +++ HI_UNF_DMX_SetChannelPID ERROR  0x%x +++\n", s32ret );
			return;	
		}
		if(g_struDataFilterResource[i].u8SectionType == 1)
			s_struChannelSection[i].u8Channeltype = 1;
		s_struChannelSection[i].u32channelID = u32channelid;
	}
	
	/*分配filter资源*/	
	struFilter.u32FilterDepth = g_struDataFilterResource[s32DataFilterIndex].u16FilterDepth;
	
	memcpy(struFilter.au8Match, g_struDataFilterResource[s32DataFilterIndex].u8Match, sizeof(HI_U8)*SECTION_FILTER_DEPTH_MAX);
	memcpy(struFilter.au8Mask, g_struDataFilterResource[s32DataFilterIndex].u8Mask, sizeof(HI_U8)*SECTION_FILTER_DEPTH_MAX);
	memcpy(struFilter.au8Negate,  g_struDataFilterResource[s32DataFilterIndex].u8Negate, sizeof(HI_U8)*SECTION_FILTER_DEPTH_MAX); 

	
	s32ret = HI_UNF_DMX_NewFilter(s_struChannelSection[i].u16frontendid, &struFilter , &u32fileterid);
	if (HI_SUCCESS != s32ret)
	{
		printf("\n +++ HI_UNF_DMX_NewFilter ERROR +++\n");
		return;	
	}

		
	s32ret = HI_UNF_DMX_SetFilterAttr(u32fileterid, &struFilter); 
	if (HI_SUCCESS != s32ret)
	{
		printf("\n +++ HI_UNF_DMX_SetFilter ERROR +++\n");
		return;	
	}

	/*分配buffer资源*/
	s_struChannelSection[i].u32filtermatrix[j].u32filterID = u32fileterid;
	s_struChannelSection[i].u32filtermatrix[j].u8VersionNum = 0xff; 
	s_struChannelSection[i].u32filtermatrix[j].u32TimeOut = g_struDataFilterResource[s32DataFilterIndex].u32TimeOut;
	s_struChannelSection[i].u32filtermatrix[j].u32TimeCount = 0;

	s_struChannelSection[i].u32filtermatrix[j].u32SecNum = 0;
	s_struChannelSection[i].u32filtermatrix[j].u32GotSecNum = 0;

	memset(s_struChannelSection[i].u32filtermatrix[j].u8GotSecFlag, 0, sizeof(HI_U8)*SECTION_DATA_SECTIONLEN);
	memset(s_struChannelSection[i].u32filtermatrix[j].s16section_length, 0, sizeof(HI_S16)*SECTION_DATA_SECTIONLEN);
	memset(s_struChannelSection[i].u32filtermatrix[j].s16section_buffindex, 0, sizeof(HI_S16)*SECTION_DATA_SECTIONLEN);

	s_struChannelSection[i].u32filtermatrix[j].u16section_next = 0;

	/* debug 动态分配COPY BUFFER空间*/

	/*关联分配到的相关资源*/
	s32ret = HI_UNF_DMX_AttachFilter(u32fileterid , s_struChannelSection[i].u32channelID);
	if (HI_SUCCESS != s32ret)
	{
		printf("\n +++ HI_UNF_DMX_AttachFilter ERROR +++\n");
		return;	
	}
	
	g_struDataFilterResource[s32DataFilterIndex].u32ChannelHandle = s_struChannelSection[i].u32channelID;
	g_struDataFilterResource[s32DataFilterIndex].u32FilterHandle = u32fileterid;


	/* ready callback process */
	if (HI_NULL_PTR != g_struDataFilterResource[s32DataFilterIndex].funReadyFunCallback)
	{
		g_struDataFilterResource[s32DataFilterIndex].funReadyFunCallback(s32DataFilterIndex );
	}

	/*启动硬件的过滤过程*/
	s32ret = HI_UNF_DMX_OpenChannel(s_struChannelSection[i].u32channelID);
	if (HI_SUCCESS != s32ret)
	{
		printf("\n +++ HI_UNF_DMX_OpenChannel ERROR +++ 0x%x\n",s32ret);
		return;
	}

	return;                   
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
static HI_S32 SectionCmp(HI_S32 s32DataFilterIndex, HI_U8 * pu8buffer, HI_U32 u32len) 
{
	HI_S32 i;
	int j = 0;

	HI_U16 FilterDepth;
	HI_U8  sectionbuffer[SECTION_FILTER_DEPTH_MAX]={0};
	HI_U8  datafiltermatch[SECTION_FILTER_DEPTH_MAX]={0};

	if (HI_NULL == pu8buffer)
	{
		return(HI_FAILURE);
	}

	if (g_struDataFilterResource[s32DataFilterIndex].u8RunFlag != 1)
	{
		return(HI_FAILURE);
	}

	/*if (pu8buffer[4] == 0x3e)
	{
		for (j =0; j < 8; j++)
		{
			printf("%02x ", pu8buffer[j]);
		}
		printf("\n");
	}*/
	FilterDepth = g_struDataFilterResource[s32DataFilterIndex].u16FilterDepth;

	for ( i = 0; i <FilterDepth; i++)
	{
		datafiltermatch[i] = g_struDataFilterResource[s32DataFilterIndex].u8Match[i] &(~ g_struDataFilterResource[s32DataFilterIndex].u8Mask[i]);

		if ( i > 0)
			sectionbuffer[i] = pu8buffer[i+2] &(~ g_struDataFilterResource[s32DataFilterIndex].u8Mask[i]);
		else
			sectionbuffer[i] = pu8buffer[i] &(~ g_struDataFilterResource[s32DataFilterIndex].u8Mask[i]);

		//printf("0x%02x  0x%02x\n", datafiltermatch[i], sectionbuffer[i]);
		
		if (datafiltermatch[i] != sectionbuffer[i])
			return(HI_FAILURE);
	}
	
	return(HI_SUCCESS);
}


static HI_VOID SectionStop(HI_S32 s32DataFilterIndex) 
{
	HI_U8   u8flag = 0;
	HI_S32 s32ret, i, j;

	for (i = 0; i< SECTION_DATA_CHANNEL_SIZE; i++)
	{
		for (j = 0; j< SECTION_DATA_CHFILTER_MAX; j++)
		{
			if (s_struChannelSection[i].u32filtermatrix[j].s32datafilterID == s32DataFilterIndex)
			{
				u8flag = 1;
				break;
			}
		}

		if (u8flag == 1)
			break;
	}


	if (i == SECTION_DATA_CHANNEL_SIZE)
	{
		return;
	}




	/*停止硬件过滤过程*/
	if (s_struChannelSection[i].u32filternum == 1)
	{	
		s32ret = HI_UNF_DMX_CloseChannel(s_struChannelSection[i].u32channelID);
		if (HI_FAILURE == s32ret)
		{	
			printf("\n +++ HI_UNF_DMX_CloseChannel failure +++\n");
			return;
		}
	}
	
	/*释放关联关系*/
	s32ret =  HI_UNF_DMX_DetachFilter( s_struChannelSection[i].u32filtermatrix[j].u32filterID,  s_struChannelSection[i].u32channelID);
	if (HI_FAILURE == s32ret)
	{	
		printf("\n +++ HI_UNF_DMX_DetachFilter failure +++\n");
		return;
	}

	/* 释放filter资源*/
	s32ret = HI_UNF_DMX_DeleteFilter(s_struChannelSection[i].u32filtermatrix[j].u32filterID);
	if (HI_FAILURE == s32ret)
	{	
		printf("\n +++ HI_UNF_DMX_DeleteFilter failure +++\n");
		return;
	}

	/* 释放channel资源*/
	if (s_struChannelSection[i].u32filternum == 1)
	{	
		s32ret = HI_UNF_DMX_DeleteChannel(s_struChannelSection[i].u32channelID);
		if (HI_FAILURE == s32ret)
		{	
			printf("\n +++ HI_UNF_DMX_DeleteChannel failure +++\n");
			return;
		}	
	}

	if (s_struChannelSection[i].u32filternum >= 1)
		s_struChannelSection[i].u32filternum --;

	if (s_struChannelSection[i].u32filternum == 0)
	{
		s_struChannelSection[i].u8Channeltype = 0;
		s_struChannelSection[i].u8Runflag = 0;
		s_struChannelSection[i].u32TSPID = 0xffffffff;
	}
	
	s_struChannelSection[i].u32filtermatrix[j].s32datafilterID = -1;
	
	s_struChannelSection[i].u32filtermatrix[j].u8Runflag = 0;
	
	/* 释放buffer资源*/
	s_struChannelSection[i].u32filtermatrix[j].u32filterID = 0xffffffff;
	s_struChannelSection[i].u32filtermatrix[j].u8VersionNum = 0xff; 
	s_struChannelSection[i].u32filtermatrix[j].u32TimeOut = 0;
	s_struChannelSection[i].u32filtermatrix[j].u32TimeCount = 0;

	s_struChannelSection[i].u32filtermatrix[j].u32SecNum = 0;
	s_struChannelSection[i].u32filtermatrix[j].u32GotSecNum = 0;

	memset(s_struChannelSection[i].u32filtermatrix[j].u8GotSecFlag, 0, sizeof(HI_U8)*SECTION_DATA_SECTIONLEN);
	memset(s_struChannelSection[i].u32filtermatrix[j].s16section_length, 0, sizeof(HI_S16)*SECTION_DATA_SECTIONLEN);
	memset(s_struChannelSection[i].u32filtermatrix[j].s16section_buffindex, 0, sizeof(HI_S16)*SECTION_DATA_SECTIONLEN);

	s_struChannelSection[i].u32filtermatrix[j].u16section_next = 0;
	
	/* debug 动态释放COPY BUFFER空间*/

	return;
}

static HI_VOID SectionTimeProcess(HI_VOID)
{
	HI_S32 i,j;

	HI_S32 s32datafilterid;
	
	for (i = 0; i< SECTION_DATA_CHANNEL_SIZE; i++)
	{
		for (j = 0; j< SECTION_DATA_CHFILTER_MAX; j++)
		{
			if ((s_struChannelSection[i].u32filtermatrix[j].u8Runflag == 1)&&(s_struChannelSection[i].u32filtermatrix[j].u32TimeOut !=0))
			{
				s32datafilterid = s_struChannelSection[i].u32filtermatrix[j].s32datafilterID;
				if (g_struDataFilterResource[s32datafilterid].u8RunFlag == 1)
				{
					s_struChannelSection[i].u32filtermatrix[j].u32TimeCount ++;
					
					if (s_struChannelSection[i].u32filtermatrix[j].u32TimeOut == s_struChannelSection[i].u32filtermatrix[j].u32TimeCount)
					{
						s_struChannelSection[i].u32filtermatrix[j].u32TimeCount = 0;
						/* table接收完回调注册函数*/
						if (HI_NULL_PTR != g_struDataFilterResource[s32datafilterid].funTableFunCallback)
						{
							g_struDataFilterResource[s32datafilterid].funTableFunCallback(s32datafilterid, 0xff, HI_FALSE);
						}
					}
				}
			}
		}

	}
	
	return;	
}


static HI_VOID SectionPostProcess(HI_S32 MsgID, HI_S32 SectionID, HI_S32 FilterID, HI_S32 sectionnum)
{
	HI_U8  *pu8SectData;
	
	HI_U8   u8tableid, u8VersionNumber ;

	HI_U32 u32len;
	HI_U32 u32index;
	HI_S32 s32datafilterid;

	s32datafilterid = s_struChannelSection[SectionID].u32filtermatrix[FilterID].s32datafilterID;

	u32index = s_struChannelSection[SectionID].u32filtermatrix[FilterID].s16section_buffindex[sectionnum];

	pu8SectData = (HI_U8 *)&(s_struChannelSection[SectionID].u32filtermatrix[FilterID].pu8section_buffer[4096*u32index]);
	
	u32len = s_struChannelSection[SectionID].u32filtermatrix[FilterID].s16section_length[sectionnum];

	/* 不支持按位取反*/
	//u8VersionNumber = s_struChannelSection[SectionID].u32filtermatrix[filterid].u8VersionNum;

	u8VersionNumber = s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8VersionBack;
	
	u8tableid = pu8SectData[0]; 

 	/* section处理执行注册函数*/
	if (HI_NULL_PTR !=g_struDataFilterResource[s32datafilterid].funSectionFunCallback)
	{	
		g_struDataFilterResource[s32datafilterid].funSectionFunCallback(s32datafilterid, pu8SectData, u32len, u8tableid);		
	}
	
	/* table接收完回调注册函数*/
	if (MESSAGE_SECTIONSVR_TABLEFINISHED ==MsgID)
	{
		s_struChannelSection[SectionID].u32filtermatrix[FilterID].u32TimeOut = 0;
		
		if (HI_NULL_PTR != g_struDataFilterResource[s32datafilterid].funTableFunCallback)
		{
			g_struDataFilterResource[s32datafilterid].funTableFunCallback(s32datafilterid, u8VersionNumber, HI_TRUE);
		}
	}
	
	/* 清除SECTION列表的标志*/
	s_struChannelSection[SectionID].u32filtermatrix[FilterID].s16section_length[sectionnum] =0;
	
	return;	
}

static HI_VOID SectionProProcess( HI_U32 SectionID,  HI_U32 FilterID,  HI_U8 * pu8buffer, HI_U32 u32len)
{
	HI_S32 DatafilterID;
	HI_U8  u8tableid;
	
	HI_U32 u32lastsection=0;
	HI_U32 u32sectionnum=0;
	HI_U32 u32index, i=0;
	
	MSG_MESSAGE_S *pstruMsg;  /*向SECTIONSVR 任务发送的消息*/ 

	if (HI_NULL == pu8buffer)
		return;

	DatafilterID = s_struChannelSection[SectionID].u32filtermatrix[FilterID].s32datafilterID;

	if ( g_struDataFilterResource[DatafilterID].u8RunFlag  == 0)
		return;	

	/* 版本号处理*/
	if (s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8VersionNum == 0xff)
	{
		s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8VersionNum = pu8buffer[5]& 0x3e;
	}
	
	if (s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8VersionNum == (pu8buffer[5]& 0x3e))
	{
		s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8VersionBack = pu8buffer[5];
		
		u8tableid = pu8buffer[0]; 
		if( ((EIT_TABLE_ID_SCHEDULE_ACTUAL_LOW <= u8tableid) && ( u8tableid <= EIT_TABLE_ID_SCHEDULE_ACTUAL_HIGH))
			||((EIT_TABLE_ID_SCHEDULE_OTHER_LOW <= u8tableid) && ( u8tableid <= EIT_TABLE_ID_SCHEDULE_OTHER_HIGH)))            
		{
			u32lastsection = (pu8buffer[7]>>3) +1;
			u32sectionnum = pu8buffer[6]>>3;
		}

		else
		{
			u32lastsection = pu8buffer[7] + 1;
			u32sectionnum = pu8buffer[6];
		}

		if ( TDT_TABLE_ID == u8tableid ||TOT_TABLE_ID == u8tableid)
		{
			u32lastsection = 1;
			u32sectionnum = 0;
			
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8VersionNum = 0xff;
		}

		/* 获得SECTION总数*/
		s_struChannelSection[SectionID].u32filtermatrix[FilterID].u32SecNum = u32lastsection;

		/* 表明收到的SECTION 是第几个，进行标记*/
		if (s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8GotSecFlag[u32sectionnum] == 0)
		{
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8GotSecFlag[u32sectionnum] = 1;			
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].u32GotSecNum ++;
			
			//保存到SECTION列表位置中
			u32index = s_struChannelSection[SectionID].u32filtermatrix[FilterID].u16section_next;

			memcpy((void *)&s_struChannelSection[SectionID].u32filtermatrix[FilterID].pu8section_buffer[u32index],  (void *)pu8buffer, sizeof(HI_U8)*u32len);
				
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].s16section_length[u32index] = u32len;
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].s16section_buffindex[u32index] = u32index;

			/* 确定新的位置*/
			for (i =0; i<SECTION_DATA_BUFFERSIZE; i++)
			{
				if (s_struChannelSection[SectionID].u32filtermatrix[FilterID].s16section_length[i] == 0)
				{
					s_struChannelSection[SectionID].u32filtermatrix[FilterID].u16section_next = i;
					break;
				}
			}

			if (i == SECTION_DATA_BUFFERSIZE)
			{
				s_struChannelSection[SectionID].u32filtermatrix[FilterID].u16section_next = 0xffff;
			}

			/* 向SECTIONSVR主任务发消息*/			
			pstruMsg = (MSG_MESSAGE_S *) HI_OSMsgClaim(s_pSvrMsgQueue, 0);
			
			if (HI_NULL == pstruMsg  )
			{
				return;
			}


			if (s_struChannelSection[SectionID].u32filtermatrix[FilterID].u32GotSecNum ==  s_struChannelSection[SectionID].u32filtermatrix[FilterID].u32SecNum)
			{
				pstruMsg->u32MessageID = MESSAGE_SECTIONSVR_TABLEFINISHED;
			}
			else
			{
				pstruMsg->u32MessageID = MESSAGE_SECTIONSVR_SECTION;
			}

			pstruMsg->s32Para1 = SectionID;			/* CHANNEL */ 
			pstruMsg->s32Para2 = FilterID;				/* FILTER */
			pstruMsg->s32Para3 = u32index;				/* SECTION NO */

			HI_OSMsgSend(s_pSvrMsgQueue, (HI_VOID *)pstruMsg);

		}
	}
}


/*******************************************************************************
 * Function:      SectionMainThread
 * Description:   SECTIONSVR核心主线程，负责SECTIONSVR模块的工作协调，子线程的管理

 * Data Accessed: 
 * Data Updated:  
 * Input:   
 *               
 * Output:        
 *                
 * Return:    
 *            
 * Others:     
 *******************************************************************************/
static HI_VOID SectionMainThread(HI_VOID)
{
	MSG_MESSAGE_S  struTmpMsg;
	MSG_MESSAGE_S  struMsg;
		
	MSG_MESSAGE_S *pstruTmpMsg;  
	MSG_MESSAGE_S *pstruMsg;


	HI_MsgQueue_p Queue;	
	MSG_MESSAGE_S *message;

	printf("This is SectionMainThread.\n");

	pstruTmpMsg = &struTmpMsg;
	pstruMsg = &struMsg;
	
	while(1)
	{
		pstruTmpMsg = (MSG_MESSAGE_S *) HI_OSMsgReceive( s_pSvrMsgQueue, 0);


		if(pstruTmpMsg == HI_NULL_PTR)
		{
			printf("\n +++ section msg null +++\n");
		}
		else	
		{
			memcpy(pstruMsg, pstruTmpMsg, sizeof(MSG_MESSAGE_S) );
			HI_OSMsgRelease(s_pSvrMsgQueue, pstruTmpMsg);

			/*deal with the message*/
			switch( pstruMsg->u32MessageID)
			{

				case    MESSAGE_SECTIONSVR_SECTION:
					{
						SectionPostProcess(pstruMsg->u32MessageID, pstruMsg->s32Para1, pstruMsg->s32Para2, pstruMsg->s32Para3);
						break;
					}
				case    MESSAGE_SECTIONSVR_TIMEOUT:
					{
						SectionTimeProcess();
						break;
					}

				case  MESSAGE_SECTIONSVR_TABLEFINISHED:
					{
						SectionPostProcess(pstruMsg->u32MessageID, pstruMsg->s32Para1, pstruMsg->s32Para2, pstruMsg->s32Para3);	
						break;
					}
					
				default:
					{
						break;
					}
			}
		}

	}

	HI_OSMsgQueueDel(s_pSvrMsgQueue);
	return;
}


static HI_VOID SectionReadThread(HI_S32 s32ReadID)
{
	static HI_S32 filter_match = 0,filter_mask = 0;
	HI_S32 	index, i, j, k, m, s32ret , datafilterid;
		
	HI_U32 	channelid[CHANNEL_MAX];
	HI_U32	u32ChNum, ChannelID, u32secnum;	
	HI_U8 softfilterdataa,softfilterdatab;
    HI_U32 u32BufLenCnt = 0;
	HI_U32 u32SecLen = 0;
	HI_U8	*pu8buff;

	HI_UNF_DMX_DATA_BUF_S sectionbuffer[SECTION_DATA_BUFFERSIZE];
//	HI_UNF_DMX_DATA_BUF_S sectionrelease[SECTION_DATA_BUFFERSIZE];
	SECTIONSVR_FILTER_S * pFilter;
	SECTIONSVR_DATA_FILTER_S *pdatafilter;
	HI_UNF_DMX_DATA_BUF_S *psectionbuff;
		
	while(1)
	{
		u32ChNum = CHANNEL_MAX;
	   	s32ret =  HI_UNF_DMX_GetDataHandle(channelid,  &u32ChNum, 5000);

		if (HI_SUCCESS != s32ret)
		{
			continue;
		}
		
		if (u32ChNum > 0)
		{
			for (index = 0; index < u32ChNum; index ++)
			{
				ChannelID = channelid[index];

				/*查找通道对应的数据结构*/

				for (i =0; i < SECTION_DATA_CHANNEL_SIZE; i ++)
				{
					if (ChannelID == s_struChannelSection[i].u32channelID)
						break;
				}
				
				if (i >= SECTION_DATA_CHANNEL_SIZE) 
				{
					continue;
				}

						
				u32secnum =0;
				//获取单个通道的BUFF
				
				s32ret = HI_UNF_DMX_AcquireBuf(ChannelID, SECTION_DATA_BUFFERSIZE, &u32secnum, sectionbuffer, 0x7fffffff);

				if (HI_SUCCESS == s32ret)						
				{
					if (u32secnum>SECTION_DATA_BUFFERSIZE)
						u32secnum = SECTION_DATA_BUFFERSIZE;
					
					for(m = 0; m< u32secnum; m++)
					{
						if (s_struChannelSection[i].u32filternum < 1)
						{
							break;
						}
						
						u32BufLenCnt = 0;
						pu8buff = sectionbuffer[m].pu8Buf;
						while (u32BufLenCnt < sectionbuffer[m].u32BufLen)
						{							
							u32SecLen = ((0x0F & pu8buff[1]) << 8) + pu8buff[2] + 3;
							for (j = 0; j < SECTION_DATA_CHFILTER_MAX; j++)
							{
								filter_mask = (filter_match + j)%SECTION_DATA_CHFILTER_MAX;
								pFilter = &(s_struChannelSection[i].u32filtermatrix[filter_mask]);
							
								if (pFilter->u8Runflag ==1)
								{
									// 从通道结构里获取滤波器的资源然后来软件滤波
									datafilterid = pFilter->s32datafilterID;
									pdatafilter = &(g_struDataFilterResource[datafilterid]);
									if (pdatafilter->u8RunFlag ==1)
									{
										/* 单个过滤条件处理*/
										if (s_struChannelSection[i].u32filternum == 1)
										{
											filter_match = filter_mask;
											/* 判断是否直传*/
											if (pdatafilter->u8DirTransFlag == 1)
											{
												if (HI_NULL_PTR != pdatafilter->funSectionFunCallback)
												{
													pdatafilter->funSectionFunCallback(datafilterid, pu8buff, u32SecLen, pu8buff[0]);
												}
											}
											else
											{
												SectionProProcess(i, filter_match, pu8buff, u32SecLen);
											}			
											//处理完后退出循环jianghaiqi
											break;
										}
										else
										{
											for(k=0;k<pdatafilter->u16FilterDepth;k++)
											{
													softfilterdataa= pdatafilter->u8Match[k] &(~ pdatafilter->u8Mask[k]);

													if ( k > 0)
														softfilterdatab = pu8buff[k+2] &(~ pdatafilter->u8Mask[k]);
													else
														softfilterdatab = pu8buff[k] &(~ pdatafilter->u8Mask[k]);

													//printf("|0x%02x  0x%02x", datafiltermatch[i], sectionbuffer[i]);
													
													if (softfilterdatab != softfilterdataa)
														break;

											}	
											/* 判断数据是否与过滤条件对应*/
											if (k == pdatafilter->u16FilterDepth)
											{
												filter_match = filter_mask;
												/* 判断是否直传*/
												if (pdatafilter->u8DirTransFlag == 1)
												{
													if (HI_NULL_PTR != pdatafilter->funSectionFunCallback)
													{
														pdatafilter->funSectionFunCallback(datafilterid, pu8buff, u32SecLen, pu8buff[0]);
													}
												}
												else
												{
													SectionProProcess(i, filter_match, pu8buff, u32SecLen);
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
									else
									{
										printf(" %s %d status error \n",__FUNCTION__,__LINE__);
									}	
								}
							}
							pu8buff += u32SecLen;
                   			u32BufLenCnt += u32SecLen;
						}
					}

					// debug
//					u32secnumrelease = u32secnum;
//					memcpy((void *)sectionrelease, (void *)sectionbuffer, sizeof(HI_UNF_DMX_DATA_BUF_S)*u32secnum);

					/* 释放DEMUX 空间*/
					s32ret = HI_UNF_DMX_ReleaseBuf(ChannelID, u32secnum, sectionbuffer);
				}
		
		       }
			
		}
	}	
}


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */


