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
#include "software.h"
#include "hardware.h"

 #ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define MAX_THREAD_NUM     10
#define MAX_SEM_NUM        40



typedef struct hi_Thread_S
{
    HI_BOOL 			u32ThreadFlag;                       /*线程标志*/
    HI_BOOL 			u32ThreadRunFlag;                 /*线程运行标志0 挂起1 运行*/	
    pthread_mutex_t 	thread_mutex;                 		
    pthread_t  			Thread;
    pthread_attr_t 		ThreadAttr;	
} HI_THREAD_S;

typedef struct hi_Sem_S
{
    HI_BOOL u32SemFlag;                              /*线程标志*/
    sem_t  Sem;                                        
} HI_SEM_S;


HI_BOOL  hi_os_thread_initial = HI_FALSE;
HI_BOOL  hi_os_sem_initial = HI_FALSE;


static HI_THREAD_S Hi_Thread_attr[MAX_THREAD_NUM];
static HI_SEM_S    Hi_Sem_attr[MAX_SEM_NUM];

HI_S32 HI_OS_TaskCreate(HI_CHAR* cName, HI_VOID* proc, HI_VOID* args,
                     HI_U32 u32StackSize, HI_U32 u32Policy)
{
    HI_S32    s32Ret     = 0;
    HI_U32  i;	

    if ((NULL == cName) || (NULL == proc) || (0 == u32StackSize))
    {
        printf("+++ [%s] [%d] create task fail! +++\n", __FUNCTION__, __LINE__); 
        return HI_FAILURE;
    }

    if(hi_os_thread_initial == HI_FALSE)	
    {
        for(i=0;i<MAX_THREAD_NUM;i++)
        {
            Hi_Thread_attr[i].u32ThreadFlag = HI_FALSE;
	     Hi_Thread_attr[i].Thread = 0;
        }
	 hi_os_thread_initial = HI_TRUE;
    }

    for(i=0;i<MAX_THREAD_NUM;i++)
    {
    	 if(Hi_Thread_attr[i].u32ThreadFlag == HI_FALSE)
		 break;
    }

    if(i >= MAX_THREAD_NUM)
    {
        printf("+++ [%s] [%d] create task fail! +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

            
    s32Ret = pthread_attr_init(&(Hi_Thread_attr[i].ThreadAttr));

    if (HI_SUCCESS != s32Ret)
    {
        printf(" +++[%s] [%d] create task fail! +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pthread_attr_setstacksize(&(Hi_Thread_attr[i].ThreadAttr), u32StackSize);

    if (0 != u32Policy)
    {        
        pthread_attr_setschedpolicy(&(Hi_Thread_attr[i].ThreadAttr), u32Policy);
    }
    
    s32Ret = pthread_create(&(Hi_Thread_attr[i].Thread), &(Hi_Thread_attr[i].ThreadAttr), proc, args);

    if (HI_SUCCESS != s32Ret)
    {
        pthread_attr_destroy(&Hi_Thread_attr[i].ThreadAttr);
        printf("+++ [%s] [%d] create task fail! +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }
    else
    {
        printf("+++ [%s] [%d] task id = %d! +++\r\n", __FUNCTION__, __LINE__, i);
    }

    Hi_Thread_attr[i].u32ThreadRunFlag = HI_TRUE;

    /* 创建线程信号量 */
    pthread_mutex_init(&Hi_Thread_attr[i].thread_mutex, NULL);

    return i+1;
}

HI_U32 HI_OS_TaskResume(HI_U32 u32Id)
{
    HI_S32 s32Ret      = 0;
    HI_S32 s32MutexNum = MAX_THREAD_NUM;
    HI_U32  i;	
    HI_U32 u32ThreadID;	

    if(hi_os_thread_initial == HI_FALSE)	
    {
        printf(" +++[%s] [%d] task not initial +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    if(u32Id> MAX_THREAD_NUM||u32Id == 0)	
    {
        printf(" +++[%s] [%d] task id over +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    u32ThreadID = 	u32Id - 1;


    if(Hi_Thread_attr[u32ThreadID].u32ThreadFlag != HI_TRUE)
    {
        printf(" +++[%s] [%d] task not create +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }
	

    if (HI_TRUE == Hi_Thread_attr[u32ThreadID].u32ThreadRunFlag)
    {
        /* 线程未被挂起 */
        return u32Id;
    }

    Hi_Thread_attr[u32ThreadID].u32ThreadRunFlag = HI_TRUE;
    
    return u32Id;
}

HI_U32 HI_OS_TaskSuspend(HI_U32 u32Id)
{
    HI_S32 s32Ret      = 0;
    HI_S32 s32MutexNum = MAX_THREAD_NUM;
    HI_U32  i;	
    HI_U32 u32ThreadID;	

    if(hi_os_thread_initial == HI_FALSE)	
    {
        printf(" +++[%s] [%d] task not initial +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }
	
    if(u32Id> MAX_THREAD_NUM||u32Id == 0)	
    {
        printf(" +++[%s] [%d] task id over +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }
	
    u32ThreadID = 	u32Id - 1;
	
    if(Hi_Thread_attr[u32ThreadID].u32ThreadFlag != HI_TRUE)
    {
        printf(" +++[%s] [%d] task not create +++\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }


    if (HI_FALSE == Hi_Thread_attr[u32ThreadID].u32ThreadRunFlag)
    {
        /* 线程未被运行 */
        return u32Id;
    }

    Hi_Thread_attr[u32ThreadID].u32ThreadRunFlag = HI_FALSE;
    
    return u32Id;
}


HI_VOID HI_OS_TaskYield(HI_VOID)
{    
    usleep(50000);
//    sched_yield(); 枣庄版本使用    
    return;
}

HI_VOID HI_OS_TaskDestroy(HI_U32 u32id)
{
    if(hi_os_thread_initial == HI_FALSE)	
    {
        printf(" +++[%s] [%d] task not initial +++\n", __FUNCTION__, __LINE__);
    }

	
    printf(" +++[%s] [%d] task destroy %d +++\n", __FUNCTION__, __LINE__,u32id);

    return;
}

HI_VOID HI_TaskDestroySelf(HI_VOID)
{
    HI_U32 u32Id   = 0;
    pthread_t pid = 0;
    HI_U32  i;	

    if(hi_os_thread_initial == HI_FALSE)	
    {
        printf(" +++[%s] [%d] task not initial +++\n", __FUNCTION__, __LINE__);
        return;
    }


    pid = pthread_self();
    
    /* 销毁线程 */

    for(i=0;i<MAX_THREAD_NUM;i++)
    {
    	 if(Hi_Thread_attr[i].Thread == pid)
		 break;
    }

    if(i >= MAX_THREAD_NUM)
    {
        printf("+++ [%s] [%d] not find task pid %d! +++\n", __FUNCTION__, __LINE__,(HI_U32)pid);
        return;
    }

    printf(" +++[%s] [%d] task destroy self id %d +++\n", __FUNCTION__, __LINE__,i);
	
    pthread_mutex_destroy(&Hi_Thread_attr[i].thread_mutex);
    Hi_Thread_attr[i].u32ThreadFlag = HI_FALSE;
	
    return;
}

HI_U32 HI_OS_SysGetTime(HI_VOID)
{
    HI_U32 u32Time = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    u32Time = tv.tv_sec * 1000  + tv.tv_usec / 1000;    
    return u32Time;
}

HI_VOID HI_OS_Sleep(HI_U32 u32Time)
{
    usleep(u32Time * 1000);
    return;
}

HI_U32 HI_OS_Getpid(HI_VOID)
{
    pthread_t pid = 0;
    pid = pthread_self();
    return (HI_U32)pid;
}

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


HI_S32 HI_OS_SemaphoreWaite(HI_U32 u32SemaphoreID, HI_U32 u32TimeOutInMS)
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


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

