/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: tunersvr.c
 * Description: 
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2008-09-01   jianghaiqi/j57220                
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "hi_stb_api.h"
//#include "software.h"
//#include "hardware.h"

#define MAX_SEM_NUM        50


typedef struct hi_Sem_S
{
    HI_BOOL u32SemFlag;                              /*线程标志*/
    sem_t  Sem;                                        
} HI_SEM_S;


HI_BOOL  hi_os_sem_initial = HI_FALSE;


static HI_SEM_S    Hi_Sem_attr[MAX_SEM_NUM];


HI_S32 HI_OS_SemaphoreCreate(HI_U32 u32InitCount, HI_U32 *pu32SemaphoreID)
{
    HI_S32 s32SemId = 0;
    HI_U32  i;	

    if(hi_os_sem_initial == HI_FALSE)	
    {
        for(i=0;i<MAX_SEM_NUM;i++)
        {
            Hi_Sem_attr[i].u32SemFlag = HI_FALSE;
        }
	 hi_os_sem_initial = HI_TRUE;
    }

    for(i=0;i<MAX_SEM_NUM;i++)
    {
    	 if(Hi_Sem_attr[i].u32SemFlag == HI_FALSE)
		 break;
    }

    if(i >= MAX_SEM_NUM)
    {
        printf("+++ [%s] [%d] create sem fail! +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    sem_init(&(Hi_Sem_attr[i].Sem), 0, u32InitCount);

    *pu32SemaphoreID = (HI_U32)i+1;
    Hi_Sem_attr[i].u32SemFlag = HI_TRUE;

    return HI_SUCCESS;    
}

HI_S32 HI_OS_SemaphoreDelete(HI_U32 u32SemaphoreID)
{
    if(hi_os_sem_initial == HI_FALSE)	
    {
        printf(" +++[%s] [%d] sem not initial +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;		
    }

    if (MAX_SEM_NUM < u32SemaphoreID || u32SemaphoreID == 0)
    {
    	 printf("+++ [%s] [%d] sem id over %d! +++\n", __FUNCTION__, __LINE__,u32SemaphoreID);
        return HI_FAILURE;
    }
    
    sem_destroy(&Hi_Sem_attr[u32SemaphoreID-1].Sem);
    Hi_Sem_attr[u32SemaphoreID-1].u32SemFlag = HI_FALSE;   
    return HI_SUCCESS;    
}

HI_S32 HI_OS_SemaphoreSignal(HI_U32 u32SemaphoreID)
{

    if(hi_os_sem_initial == HI_FALSE)	
    {
        printf(" +++[%s] [%d] sem not initial +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;		
    }

    if (MAX_SEM_NUM < u32SemaphoreID || u32SemaphoreID == 0)
    {
    	 printf("+++ [%s] [%d] sem id over %d! +++\n", __FUNCTION__, __LINE__,u32SemaphoreID);
        return HI_FAILURE;
    }

    sem_post(&Hi_Sem_attr[u32SemaphoreID-1].Sem);

    return HI_SUCCESS;    
}


HI_S32 HI_OS_SemaphoreWait(HI_U32 u32SemaphoreID, HI_U32 u32TimeOutInMS)
{
    HI_S32 s32Ret   = 0;

    if(hi_os_sem_initial == HI_FALSE)	
    {
        printf(" +++[%s] [%d] sem not initial +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;		
    }

    if (MAX_SEM_NUM < u32SemaphoreID || u32SemaphoreID == 0)
    {
    	 printf("+++ [%s] [%d] sem id over %d! +++\n", __FUNCTION__, __LINE__,u32SemaphoreID);
        return HI_FAILURE;
    }
        
    if (0xffffffff == u32TimeOutInMS)
    {
        sem_wait(&Hi_Sem_attr[u32SemaphoreID-1].Sem);

        return HI_SUCCESS;
    }
    else
    {

        struct timespec ts = {0};
        ts.tv_nsec = (u32TimeOutInMS%1000) *1000*1000;                               
        ts.tv_sec  = time(HI_NULL) + u32TimeOutInMS/1000; 
        sem_timedwait(&Hi_Sem_attr[u32SemaphoreID-1].Sem, &ts);
        return HI_SUCCESS;
    }    
}

