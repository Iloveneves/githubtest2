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

/* SECTION������Դ*/
static SECTIONSVR_SECTIONDATA_S 	s_struChannelSection[SECTION_DATA_CHANNEL_SIZE];

/* ��������Դ*/
static SECTIONSVR_DATA_FILTER_S 	g_struDataFilterResource[SECTION_DATA_FILTER_SIZE];                  /*  ȫ�ֵ����ݹ�������Դ��������id��ʼ��Ϊ-1 */

/* ����*/
static pthread_t s_pSectionSvrTaskHandle = HI_NULL;	
static pthread_t s_pSectionReadTaskHandle = HI_NULL;	

/* timer */
static petimer_t  s_pSectionTimerHandle = HI_NULL;	

/* ��Ϣ*/
static HI_MsgQueue_p  s_pSvrMsgQueue = HI_NULL;	

/*******************************************************************************
 * Function:      SECSVR_SectionCreateTask
 * Description:   ����SECTIONSVR��������APPMNGSVR����ʱ����SECTIONSVR����������
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    HI_FAILURE ʧ��
 *            >=0 ����ID
 * Others:     
 *******************************************************************************/
HI_S32  SECSVR_SectionCreateTask(HI_VOID) 
{
	
	HI_S32 s32Ret;

	SECTION_DBG("*****SVR_SectionCreateTask()**** \n");

	/*�ж�SECTON ģ���Ƿ��Ѿ�������*/
	if( HI_NULL != s_pSectionSvrTaskHandle)
	{
		printf( "section main thread is allready in running \n");
		return HI_FAILURE;
	}


	/*��ʼ�����ݹ�������Դ*/
	s32Ret = SECSVR_SectionInitDataFilter( );
	if(s32Ret!=0)   /*  ��ʼ��ʧ�� */
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
	if(s32Ret!=0)   /*  �̴߳���ʧ�� */
	{
		HI_OSMsgQueueDel(s_pSvrMsgQueue);
		
		printf( "pthread_create() falure \n");
		s_pSectionReadTaskHandle = HI_NULL;
		return HI_FAILURE;
	}


	/*����SECTIONSVR���߳�*/
	s32Ret = pthread_create(&s_pSectionSvrTaskHandle, HI_NULL, 
			(HI_VOID *)&SectionMainThread, HI_NULL);   /*����SECTION ���߳�*/
	if(s32Ret!=0)   /*  �̴߳���ʧ�� */
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
 * Description:   ����SECTIONSVR������ֹͣ������������Դ����APPMNGSVR����
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    HI_FAILURE ʧ��
 *            0 �ɹ�
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionDestroyTask(HI_VOID)
{
	/*��SECTIONSVR���̷߳�������������*/

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
 * Description:   ���ݹ�������ʼ��
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    HI_FAILURE ʧ��
 *            =0 �ɹ�
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionInitDataFilter(HI_VOID)
{
	HI_S32 i, j;


	/*��ʼ��ȫ�ֱ���g_struDataFilterResource */
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
 * Description:   ���ݹ��������룬�����ݹ�������Դ�з���������������ѷ����������ID�š�
 �ڹ���table ʱ����Ҫ�������ݹ��������������������ݹ�����������������
 * Data Accessed: 
 * Data Updated:  
 * Input:         
 *                
 * Output:        
 *                
 * Return:    HI_FAILURE ʧ��
 *            >=0 ���ݹ�����ID
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionAllocDataFilter(HI_VOID)
{
	HI_S32 i;

	/*����Ƿ���ʣ���data filter ��Դ*/
	for(i = 0; i < SECTION_DATA_FILTER_SIZE; i++)
	{
		if(  0  == g_struDataFilterResource[i].u8UseFlag )
		{
			g_struDataFilterResource[i].s32DataFilterID = i;
			g_struDataFilterResource[i].u8UseFlag= 1;
			g_struDataFilterResource[i].u8RunFlag= 0;
			return(i);   /*  ����channel ������ */
		}
	}

	return HI_FAILURE;
}

/*******************************************************************************
 * Function:      SVR_SectionFreeDataFilter
 * Description:   �ͷ���������ݹ�����

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID -  ������ID��        
 *                
 * Output:        
 *                
 * Return:		none    
 *            
 * Others:     
 *******************************************************************************/
HI_VOID SECSVR_SectionFreeDataFilter(HI_S32 s32DataFilterID)
{
	/*�����������Ƿ�Ϸ�*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		return; 
	} 

	/*���ָ����Դ�Ƿ��Ѿ�����*/
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
 * Description:   �������ݹ���������Ҫ�����뵽���ݹ�����Դ�����

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID -  ������ID��        
 *        pstruDataFilter ��������Ϣ        
 * Output:        
 *                
 * Return:    HI_FAILURE ʧ��
 *            0 �ɹ�
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionSetDataFilter(HI_S32 s32DataFilterID, SECTIONSVR_DATA_FILTER_S  *pstruDataFilter)
{

	SECTION_DBG( "*****SVR_SectionSetDataFilter()**** \n");

	/*�����������Ƿ�Ϸ�*/
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

	/*���ָ����Դ�Ƿ��Ѿ�����*/
	if (1 != g_struDataFilterResource[s32DataFilterID].u8UseFlag)
	{
		printf("invalid data filter \n");
		return( HI_FAILURE);	
	}


	/*����ָ�������ݹ�������Դ*/
	memcpy(&g_struDataFilterResource[s32DataFilterID], pstruDataFilter, sizeof(SECTIONSVR_DATA_FILTER_S));

	return (HI_SUCCESS);
}


/*******************************************************************************
 * Function:      SVR_SectionGetDataFilter
 * Description:   �õ�s32DataFilterID��Ӧ�����ݹ�����

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID -  ������ID��        
 *        pstruDataFilter ������ָ��        
 * Output:        
 *                
 * Return:    HI_FAILURE ʧ��
 *            0 �ɹ�
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionGetDataFilter (HI_S32 s32DataFilterID, SECTIONSVR_DATA_FILTER_S    *pstruDataFilter)
{

//	printf("*****SVR_SectionGetDataFilter()**** \n");

	/*�����������Ƿ�Ϸ�*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		printf("invalid datafilter id \n");
		return(HI_FAILURE); 
	} 

	/*�����������Ƿ�Ϸ�*/
	if(HI_NULL == pstruDataFilter)
	{
		printf( "invalid data filter pointer \n");
		return(HI_FAILURE);
	}

	/*���ָ����Դ�Ƿ��Ѿ�����*/

	/*����ָ���Ĺ�����ָ��*/
	memcpy(pstruDataFilter, &g_struDataFilterResource[s32DataFilterID], sizeof(SECTIONSVR_DATA_FILTER_S) );

	return( HI_SUCCESS);

}

/*******************************************************************************
 * Function:      SVR_SectionStartDataFilter
 * Description:   Ӧ�õ��ã���SECTIONSRV�������������ݹ��ˣ�
 ��Ҫ���������ݹ�����֮�����

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID������ID��        
 *               
 * Output:        
 *                
 * Return:    HI_FAILURE ʧ��
 *            0 �ɹ�
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionStartDataFilter(HI_S32 s32DataFilterID)
{
	MSG_MESSAGE_S *pstruMsg;  /*���������͵���Ϣ*/ 

	/*�����������Ƿ�Ϸ�*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		return (HI_FAILURE); 
	} 

	/* �������б��*/
	g_struDataFilterResource[s32DataFilterID].u8RunFlag = 1;

	SectionStart(s32DataFilterID);

	
	return (HI_SUCCESS);
}

/*******************************************************************************
 * Function:      SVR_SectionStopDataFilter
 * Description:   Ӧ�ø�SECTIONSRV��������Ϣֹͣ���ݹ���������

 * Data Accessed: 
 * Data Updated:  
 * Input: s32DataFilterID������ID��        
 *               
 * Output:        
 *                
 * Return:    HI_FAILURE ʧ��
 *            0 �ɹ�
 * Others:     
 *******************************************************************************/
HI_S32 SECSVR_SectionStopDataFilter(HI_S32 s32DataFilterID)
{
	MSG_MESSAGE_S *pstruMsg;  /*���������͵���Ϣ*/ 

	HI_MsgQueue_p Queue;	

	SECTION_DBG( "*****SVR_SectionStopDataFilter()**** \n");

	/*�����������Ƿ�Ϸ�*/
	if((s32DataFilterID < 0) ||(s32DataFilterID >= SECTION_DATA_FILTER_SIZE))
	{ 
		printf( "invalid datafilter id \n");
		return(HI_FAILURE); 
	} 

	/* ����ֹͣ���*/
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

				/* ��SECTIONSVR��������Ϣ*/
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
	
	/* �ж����ݹ�����TSPID�Ƿ���ͬ*/
	for (i =0; i< SECTION_DATA_CHANNEL_SIZE; i++)
	{
		//��ͬ��DEMUX�ﲻ������ͬ��PID
		if ((s_struChannelSection[i].u32TSPID == g_struDataFilterResource[s32DataFilterIndex].u32TSPID)
			&& (s_struChannelSection[i].u16frontendid == g_struDataFilterResource[s32DataFilterIndex].frontendid))
		{
			break;				
		}	
	}

	/* ����*/
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

	/* ����CHANNEL */	
	if (u8addflag ==1)
	{
		s32ret = HI_UNF_DMX_GetChannelDefaultAttr(&struChan);
		if (HI_SUCCESS != s32ret)
		{		
			return;			
		}
		/*�����PES���͵�ͨ��*/
		if(g_struDataFilterResource[i].u8SectionType == 1)
		{
			struChan.enChannelType = HI_UNF_DMX_CHAN_TYPE_PES;
			struChan.u32BufSize = 32;
			struChan.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
		}	

		struChan.u32BufSize = 512;

		/*����channel��Դ*/
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
	
	/*����filter��Դ*/	
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

	/*����buffer��Դ*/
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

	/* debug ��̬����COPY BUFFER�ռ�*/

	/*�������䵽�������Դ*/
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

	/*����Ӳ���Ĺ��˹���*/
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
 * Input: s32DataFilterID -  ������ID��        
 *                
 * Output:        
 *                
 * Return:		HI_SUCCESS		�Ǵ˹���������
 *				HI_FAILURE		���Ǵ˹���������
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




	/*ֹͣӲ�����˹���*/
	if (s_struChannelSection[i].u32filternum == 1)
	{	
		s32ret = HI_UNF_DMX_CloseChannel(s_struChannelSection[i].u32channelID);
		if (HI_FAILURE == s32ret)
		{	
			printf("\n +++ HI_UNF_DMX_CloseChannel failure +++\n");
			return;
		}
	}
	
	/*�ͷŹ�����ϵ*/
	s32ret =  HI_UNF_DMX_DetachFilter( s_struChannelSection[i].u32filtermatrix[j].u32filterID,  s_struChannelSection[i].u32channelID);
	if (HI_FAILURE == s32ret)
	{	
		printf("\n +++ HI_UNF_DMX_DetachFilter failure +++\n");
		return;
	}

	/* �ͷ�filter��Դ*/
	s32ret = HI_UNF_DMX_DeleteFilter(s_struChannelSection[i].u32filtermatrix[j].u32filterID);
	if (HI_FAILURE == s32ret)
	{	
		printf("\n +++ HI_UNF_DMX_DeleteFilter failure +++\n");
		return;
	}

	/* �ͷ�channel��Դ*/
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
	
	/* �ͷ�buffer��Դ*/
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
	
	/* debug ��̬�ͷ�COPY BUFFER�ռ�*/

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
						/* table������ص�ע�ắ��*/
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

	/* ��֧�ְ�λȡ��*/
	//u8VersionNumber = s_struChannelSection[SectionID].u32filtermatrix[filterid].u8VersionNum;

	u8VersionNumber = s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8VersionBack;
	
	u8tableid = pu8SectData[0]; 

 	/* section����ִ��ע�ắ��*/
	if (HI_NULL_PTR !=g_struDataFilterResource[s32datafilterid].funSectionFunCallback)
	{	
		g_struDataFilterResource[s32datafilterid].funSectionFunCallback(s32datafilterid, pu8SectData, u32len, u8tableid);		
	}
	
	/* table������ص�ע�ắ��*/
	if (MESSAGE_SECTIONSVR_TABLEFINISHED ==MsgID)
	{
		s_struChannelSection[SectionID].u32filtermatrix[FilterID].u32TimeOut = 0;
		
		if (HI_NULL_PTR != g_struDataFilterResource[s32datafilterid].funTableFunCallback)
		{
			g_struDataFilterResource[s32datafilterid].funTableFunCallback(s32datafilterid, u8VersionNumber, HI_TRUE);
		}
	}
	
	/* ���SECTION�б�ı�־*/
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
	
	MSG_MESSAGE_S *pstruMsg;  /*��SECTIONSVR �����͵���Ϣ*/ 

	if (HI_NULL == pu8buffer)
		return;

	DatafilterID = s_struChannelSection[SectionID].u32filtermatrix[FilterID].s32datafilterID;

	if ( g_struDataFilterResource[DatafilterID].u8RunFlag  == 0)
		return;	

	/* �汾�Ŵ���*/
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

		/* ���SECTION����*/
		s_struChannelSection[SectionID].u32filtermatrix[FilterID].u32SecNum = u32lastsection;

		/* �����յ���SECTION �ǵڼ��������б��*/
		if (s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8GotSecFlag[u32sectionnum] == 0)
		{
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].u8GotSecFlag[u32sectionnum] = 1;			
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].u32GotSecNum ++;
			
			//���浽SECTION�б�λ����
			u32index = s_struChannelSection[SectionID].u32filtermatrix[FilterID].u16section_next;

			memcpy((void *)&s_struChannelSection[SectionID].u32filtermatrix[FilterID].pu8section_buffer[u32index],  (void *)pu8buffer, sizeof(HI_U8)*u32len);
				
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].s16section_length[u32index] = u32len;
			s_struChannelSection[SectionID].u32filtermatrix[FilterID].s16section_buffindex[u32index] = u32index;

			/* ȷ���µ�λ��*/
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

			/* ��SECTIONSVR��������Ϣ*/			
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
 * Description:   SECTIONSVR�������̣߳�����SECTIONSVRģ��Ĺ���Э�������̵߳Ĺ���

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

				/*����ͨ����Ӧ�����ݽṹ*/

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
				//��ȡ����ͨ����BUFF
				
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
									// ��ͨ���ṹ���ȡ�˲�������ԴȻ��������˲�
									datafilterid = pFilter->s32datafilterID;
									pdatafilter = &(g_struDataFilterResource[datafilterid]);
									if (pdatafilter->u8RunFlag ==1)
									{
										/* ����������������*/
										if (s_struChannelSection[i].u32filternum == 1)
										{
											filter_match = filter_mask;
											/* �ж��Ƿ�ֱ��*/
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
											//��������˳�ѭ��jianghaiqi
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
											/* �ж������Ƿ������������Ӧ*/
											if (k == pdatafilter->u16FilterDepth)
											{
												filter_match = filter_mask;
												/* �ж��Ƿ�ֱ��*/
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
												//��������˳�ѭ��jianghaiqi
												break;
											}
											else
											{
												//printf(" %s %d status error tableid %d len %d\n",__FUNCTION__,__LINE__,sectionbuffer[m].pu8Buf[0],sectionbuffer[m].u32BufLen);
												
												/* ���Ϲ���������*/
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

					/* �ͷ�DEMUX �ռ�*/
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


