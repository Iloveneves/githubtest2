/**********************************************************************
 * Description: 
 *    
 *
 * Revision History:
 *    Date          Author         Modificahition
 * ---------------------------------------------------------------------
 * 1. 2008-10-7   
 **********************************************************************/

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/stat.h>
#include "enreach_erro.h"
//#include "msg_q.h"
#include "hi_msg_queue.h"
#include "hi_base.h"
#include "enreach_pub.h"
#include "enreach_oc_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if 0
/*-----------------------------------------------------------------------------
		add by jlin
-----------------------------------------------------------------------------*/
typedef struct tagMutex_ATTR_S
{
} MUTEX_ATTR_S;
#endif

/*----------------------------------------------------------------------------*/

typedef struct tagThread_ATTR_S
{
    HI_U32 			u32ThreadID;                           /* �߳�ID                */
    pthread_mutex_t 	thread_mutex;                 /* �߳��ź���            */
    sem_t  			sem; /* �߳��Ƿ���� */
} THREAD_ATTR_S;

typedef struct tagSem_ATTR_S
{
    HI_U32 u32SemId;                              /* �ź���ID              */
    sem_t  struSemAttr;                           /* �ź�������            */
} SEM_ATTR_S;

static HI_BOOL g_bThreadAttr_Ini  = HI_FALSE;     /* �߳����Գ�ʼ�����    */
static HI_BOOL g_bSemAttr_Ini     = HI_FALSE;     /* �ź������Գ�ʼ�����  */

#if 0
/*-----------------------------------------------------------------------------
		add by jlin
------------------------------------------------------------------------------*/
static HI_BOOL g_bMutexAttr_Ini = HI_FALSE;  /*mutex attr init flag*/

/*----------------------------------------------------------------------------*/
#endif

/* �߳����� */
static THREAD_ATTR_S g_struThread_attr[MAX_THREAD_NUM]
                               = {{INVALID_THREAD_ID, HI_FALSE, }};


/* �ź������Խṹ */
static SEM_ATTR_S    g_struSem_Attr[MAX_SEM_NUM]
       = { {INVALID_SEM_ID, } };


static HI_S32  getThreadAttrNum(HI_U32 u32ThreadId)
{
    HI_S32 i = 0;

    if (HI_FALSE == g_bThreadAttr_Ini)
    {
        for (i = 0; i < MAX_THREAD_NUM; i++)
        {
            g_struThread_attr[i].u32ThreadID  = INVALID_THREAD_ID;
        }

        g_bThreadAttr_Ini = HI_TRUE;
    }
    
    for (i = 0; i < MAX_THREAD_NUM; i++)
    {
        if (u32ThreadId == g_struThread_attr[i].u32ThreadID)
        {
            break;
        }
    }

    return i;
}

static HI_S32 getSemNum(HI_U32 u32SemId)
{
    HI_S32 i = 0;

    if (HI_FALSE == g_bSemAttr_Ini)
    {
        for (i = 0; i < MAX_SEM_NUM; i++)
        {
            g_struSem_Attr[i].u32SemId = INVALID_SEM_ID;
        }

        g_bSemAttr_Ini = HI_TRUE;
    }

    for (i = 0; i < MAX_SEM_NUM; i++)
    {
        if (u32SemId != g_struSem_Attr[i].u32SemId)
        {
            continue;
        }

        break;
    }

    return i;
}

/**********************************************************************
 * ��������
 * 
 * @param cName        : ��������
 * @param proc         : ������
 * @param args         : ����������
 * @param u32StackSize : ��ջ��С
 * @param u32Policy    : �߳����ȼ�
 *
 * @return HI_U32
**********************************************************************/
HI_U32 HI_TaskCreate(HI_CHAR* cName, HI_VOID* proc, HI_VOID* args,
                     HI_U32 u32StackSize, HI_U32 u32Policy)
{
	HI_S32    		s32Ret     = 0;
	pthread_t 		ulThreadID = 0;
	pthread_attr_t 	struAttr;

	EVM_INFO("Enter");

	if ((NULL == cName) || (NULL == proc) || (0 == u32StackSize))
	{
		/* ��������,����0��ʾ���� */
		EVM_ERROR("create task fail!\r\n");
		return 0;
	}

	HI_S32 s32Num = getThreadAttrNum(INVALID_THREAD_ID);

	/* �̴߳����Ѵ�������� */
	if (MAX_THREAD_NUM <= s32Num)
	{
		EVM_ERROR("Thread attr is full")
		return ulThreadID;
	}

	s32Ret = pthread_attr_init(&struAttr);

	if (HI_SUCCESS != s32Ret)
	{
		EVM_ERROR("create task fail!\r\n");

		/* �߳����Գ�ʼ��ʧ��,���ش��� */
		return ulThreadID;
	}

	pthread_attr_setstacksize(&struAttr, u32StackSize);

	if (0 != u32Policy)
	{
		/* �����߳����ȼ� */
		pthread_attr_setschedpolicy(&struAttr, u32Policy);
	}

	EVM_INFO("call HI_TaskCreate!");
	s32Ret = pthread_create(&ulThreadID, &struAttr, proc, args);
	if (HI_SUCCESS != s32Ret)
	{
		ulThreadID = 0;
		pthread_attr_destroy(&struAttr);
		g_struThread_attr[s32Num].u32ThreadID = INVALID_THREAD_ID;

		EVM_ERROR("create task fail!");

		return ulThreadID;
	}
	else
	{
		EVM_INFO("tid:%d, index:%d", ulThreadID, s32Num);
	}

	g_struThread_attr[s32Num].u32ThreadID = ulThreadID;

#if 0
	/* �����߳��ź��� */
	pthread_mutex_init(&g_struThread_attr[s32Num].thread_mutex, NULL);

	printf("sem_init: %d\n", s32Num);
	sem_init(&g_struThread_attr[s32Num].sem, 0, 0);
#endif

	//return ulThreadID;

	return (s32Num+1);

}

/**********************************************************************
 * �˳�Task���ͷ���Դ����ɱ��ʽ
 * 
 * @return
 **********************************************************************/
HI_VOID HI_TaskDestroySelf(HI_VOID)
{
	HI_U32 u32Id   = 0;
	pthread_t ptid = 0;

	ptid = pthread_self();


//	pthread_join(ptid, NULL);
	u32Id = getThreadAttrNum(ptid);
	if (u32Id < MAX_THREAD_NUM)
	{
		g_struThread_attr[u32Id].u32ThreadID = INVALID_THREAD_ID;
		EVM_DEBUG("Destory thread attr index:%u", u32Id);
	}
	else
	{
		EVM_ERROR("this thread is no create by the HI_TaskCreate");
	}

#if 0
	/* �ͷ��ź��� */
	pthread_mutex_destroy(&g_struThread_attr[u32Id].thread_mutex);

	EVM_INFO("sem_destroy: %d", u32Id);
	sem_destroy(&g_struThread_attr[u32Id].sem);
#endif


	/* �����߳� */
	EVM_DEBUG("Enter, kill thread tid:0x%x, thread_set index:%d", ptid, u32Id);
	//pthread_cancel(ptid);

	pthread_exit(NULL);

	return;
}

/**********************************************************************
 * �˳�Task���ͷ���Դ����ɱ��ʽ
 * 
 * @param u32Pid : ����id
 *
 * @return
 **********************************************************************/
HI_VOID HI_TaskDestroy(HI_U32 u32Pid)
{
//	HI_U32 u32Id   = 0;
//	pthread_t ptid = 0;

//	ptid = pthread_self();

//	u32Id = getThreadAttrNum(ptid);

	if (u32Pid-1 < MAX_THREAD_NUM)
	{
		pthread_cancel(g_struThread_attr[u32Pid-1].u32ThreadID);
		g_struThread_attr[u32Pid-1].u32ThreadID = INVALID_THREAD_ID;
		EVM_DEBUG("Destory thread attr index:%u", u32Pid);
	}
	else
	{
		EVM_ERROR("this thread is no create by the HI_TaskCreate");
	}

}

/**********************************************************************
 * Task����ָ��ʱ��
 * 
 * @param u32Time : ����ʱ��
 *
 * @return
 **********************************************************************/ 
HI_VOID HI_TaskSleep(HI_U32 u32Time)
{
    usleep(u32Time * 1000);
    
    //EVM_ERROR("$$$ task sleep %d! $$$\r\n", u32Time * 1000);

    return;
}

/**********************************************************************
 * task����
 * 
 * @param u32Id : task ID
 *
 * @return
 **********************************************************************/  
HI_U32 HI_TaskSuspend(HI_U32 u32Id)
{   
    usleep(50000);

    return u32Id;

#if 0
    HI_S32 s32MutexNum = MAX_THREAD_NUM;

    s32MutexNum = getThreadAttrNum(u32Id);

    if (MAX_THREAD_NUM <= s32MutexNum)
    {
        printf("HI_TaskSuspend: ���طǷ��߳�ID��ʾʧ��\n");
        return INVALID_THREAD_ID;
    }

    /* ʹ���ź��������߳� */
    printf("sem_wait: %d\n", s32MutexNum);
    sem_wait(&g_struThread_attr[s32MutexNum].sem);
    printf("sem_wait: %d done!\n", s32MutexNum);    

    return u32Id;
#endif
}

/**********************************************************************
 * task�ָ�
 * 
 * @param u32Id : task ID
 *
 * @return
 **********************************************************************/   
HI_U32 HI_TaskResume(HI_U32 u32Id)
{
#if 0
    HI_S32 s32MutexNum = MAX_THREAD_NUM;

    s32MutexNum = getThreadAttrNum(u32Id);

    if (MAX_THREAD_NUM <= s32MutexNum)
    {
        printf("HI_TaskResume: ���طǷ��߳�ID��ʾʧ��: %d\n", s32MutexNum);
        return INVALID_THREAD_ID;
    }

    printf("sem_post: %d\n", s32MutexNum);
    sem_post(&g_struThread_attr[s32MutexNum].sem);
#endif

    return u32Id;
}

/**********************************************************************
 * task�ͷ�ʱ��Ƭ
 * 
 * @return
 **********************************************************************/   
HI_VOID HI_TaskYield(HI_VOID)
{    
    /* sleep 50ms */

    usleep(50000);
    //sched_yield(); ��ׯ�汾ʹ��
    
    return;
}

/**********************************************************************
 * ��ȡ Task ID
 * 
 * @return HI_U32
 **********************************************************************/   
HI_U32 HI_TaskGetId(HI_VOID)
{

	EVM_DEBUG("Enter")
#if 0
    pthread_t ptid = 0;

    ptid = pthread_self();

    return (HI_U32)ptid;
#else
	HI_U32 index;
	index = getThreadAttrNum(pthread_self());
	printf("Get the thread index:%u", index);
	return index + 1;
#endif


}

/**********************************************************************
 * ��ȡ��ǰʱ��
 * 
 * @return HI_U32
 **********************************************************************/   
HI_U32 HI_TaskGetCurMSec(HI_VOID)
{
    HI_U32 u32Time = 0;
    struct timeval tvStart;

    /* ��ȡϵͳ����ʱ�� */

    gettimeofday(&tvStart, NULL);
    u32Time = tvStart.tv_sec * 1000  + tvStart.tv_usec / 1000;
    
    return u32Time;
}

/**********************************************************************
 * �����ź���
 * 
 * @param u32InitCount    : ��ʼ���ź���
 * @param pu32SemaphoreID : �������ź���ID
 * 
 * @return HI_U32
 **********************************************************************/ 
HI_S32 HI_SemaphoreCreate(HI_U32 u32InitCount, HI_U32 *pu32SemaphoreID)
{
    HI_S32 s32SemId = 0;

    s32SemId = getSemNum(INVALID_SEM_ID);

    if (MAX_SEM_NUM <= s32SemId)
    {
        return HI_FAILURE;
    }

    	sem_init(&g_struSem_Attr[s32SemId].struSemAttr, 0, u32InitCount);
	
    *pu32SemaphoreID = (HI_U32)s32SemId+1;
    g_struSem_Attr[s32SemId].u32SemId = (HI_U32)s32SemId+1;

    return HI_SUCCESS;    
}

/**********************************************************************
 * ɾ���ź���
 * 
 * @param u32SemaphoreID : �ź���ID
 * 
 * @return HI_U32
 **********************************************************************/ 
HI_S32 HI_SemaphoreDelete(HI_U32 u32SemaphoreID)
{
#if 0
    HI_S32 s32SemId = 0;

    s32SemId = getSemNum(u32SemaphoreID);

    if (MAX_SEM_NUM <= s32SemId)
    {
        return HI_FAILURE;
    }
    
    sem_destroy(&g_struSem_Attr[s32SemId].struSemAttr);
    g_struSem_Attr[s32SemId].u32SemId = INVALID_SEM_ID;
    return HI_SUCCESS;
#else

	if (u32SemaphoreID > MAX_SEM_NUM)
	{
		EVM_ERROR("SEM id is wrong, id:%u", u32SemaphoreID);
		return HI_FAILURE;
	}
	
    sem_destroy(&g_struSem_Attr[u32SemaphoreID-1].struSemAttr);
    g_struSem_Attr[u32SemaphoreID-1].u32SemId = INVALID_SEM_ID;

	
	return HI_SUCCESS;
	
#endif
	
}

/**********************************************************************
 * ʹ���ź���
 * 
 * @param u32SemaphoreID : �ź���ID
 * 
 * @return HI_U32
 **********************************************************************/ 
HI_S32 HI_SemaphoreSignal(HI_U32 u32SemaphoreID)
{
#if 0
    HI_S32 s32SemId = 0;

    s32SemId = getSemNum(u32SemaphoreID);

    if (MAX_SEM_NUM <= s32SemId)
    {
        return HI_FAILURE;
    }

    sem_post(&g_struSem_Attr[s32SemId].struSemAttr);

    return HI_SUCCESS;    
#else
	if (u32SemaphoreID > MAX_SEM_NUM)
	{
		return HI_FAILURE;
	}

    sem_post(&g_struSem_Attr[u32SemaphoreID-1].struSemAttr);
    return HI_SUCCESS;    
	
	
#endif
}

/**********************************************************************
 * �ȴ��ź���
 * 
 * @param u32SemaphoreID : �ź���ID
 * @param u32TimeOutInMS : ��ʱʱ��
 *
 * @return HI_U32
 **********************************************************************/ 
HI_S32 HI_SemaphoreWaite(HI_U32 u32SemaphoreID, HI_U32 u32TimeOutInMS)
{

#if 0
    HI_S32 s32SemId = 0;

    s32SemId = getSemNum(u32SemaphoreID);

    if (MAX_SEM_NUM <= s32SemId)
    {
        return HI_FAILURE;
    }
        
    if (OC_INFINITE_WAIT == u32TimeOutInMS)
    {
        sem_wait(&g_struSem_Attr[s32SemId].struSemAttr);

        return HI_SUCCESS;
    }
    else
    {
        struct timespec ts = {0};
        ts.tv_nsec = (u32TimeOutInMS % 1000) * 1000 * 1000;
        ts.tv_sec  = time(HI_NULL) + u32TimeOutInMS / 1000;  /* �յ� */
        sem_timedwait(&g_struSem_Attr[s32SemId].struSemAttr, &ts);

        return HI_SUCCESS;
    } 
#endif

	if (u32SemaphoreID > MAX_SEM_NUM)
	{
		return HI_FAILURE;
	}
	if (OC_INFINITE_WAIT == u32TimeOutInMS)
	{
		sem_wait(&g_struSem_Attr[u32SemaphoreID-1].struSemAttr);
		return HI_SUCCESS;
	}
	else
	{
		struct timespec ts = {0};
		ts.tv_nsec = (u32TimeOutInMS % 1000) * 1000 * 1000;
		ts.tv_sec  = time(HI_NULL) + u32TimeOutInMS / 1000;  /* �յ� */
		sem_timedwait(&g_struSem_Attr[u32SemaphoreID-1].struSemAttr, &ts);

		return HI_SUCCESS;
	} 
	
}

/**********************************************************************
 * ������Ϣ����
 * 
 * @param u32MsgCount  : �����Ϣ����
 * @param u32MsgLength : �����Ϣ���ȣ��ֽ�Ϊ��λ
 * @param *pQueueID    : ���ش�������Ϣ����ID
 *
 * @return HI_U32
 **********************************************************************/ 
HI_OS_STATUS HI_OsMsgQueueCreate(HI_U32 u32MsgCount,
                                 HI_U32 u32MsgLength,
                                 HI_U32* pQueueID)
{
	EVM_INFO("Enter, MsgCount:%u, MSG_len:%u", u32MsgCount, u32MsgLength);
	
	ENREACH_RETURN(((NULL == pQueueID) || (0 == u32MsgLength)), OS_ERROR);

	/* ��Ϣ�����е���Ϣ��С+4 */
	//*pQueueID = (HI_U32)HI_OSMsgQueueCreate(u32MsgLength + 4, u32MsgCount);
	*pQueueID = (HI_U32)HI_OSMsgQueueCreate(u32MsgLength, u32MsgCount);
	//*pQueueID = (HI_U32)HI_OSMsgQueueCreate(sizeof (HI_ENRC_MSG_MESSAGE_S), u32MsgCount);

	ENREACH_RETURN((0 == *pQueueID), OS_ERROR);

	EVM_DEBUG("Create ok, queueId:%u", *pQueueID);
	
	return OS_OK;
}

HI_OS_STATUS HI_OsMsgQueueDel(HI_U32 u32QueueID)
{

    	HI_OSMsgQueueDel((HI_MsgQueue_p)u32QueueID);
	
	EVM_DEBUG("delete ok, queueId:%u", u32QueueID);

    return OS_OK;
}



HI_OS_STATUS HI_OsMsgQueueRec(HI_U32 u32QueueID, 
                                  HI_VOID* pBuf,
                                  HI_U32 u32BufLen,
                                  HI_U32 u32TimeInMS)
{
#if 0
    HI_ENRC_MSG_MESSAGE_S *pstruTmpMsg = NULL;
    HI_MsgQueue_p pMsgQueue = (HI_MsgQueue_p)u32QueueID;

	EVM_DEBUG("Enter, recv queueId:%u, timeout:%u", u32QueueID, u32TimeInMS);

    pstruTmpMsg = (HI_ENRC_MSG_MESSAGE_S*)HI_OSMsgReceive(pMsgQueue, u32TimeInMS);

    ENREACH_RETURN((HI_NULL == pstruTmpMsg), OS_ERROR);
    ENREACH_RETURN((pstruTmpMsg->s32Para1 > u32BufLen), OS_ERROR);

    memcpy(pBuf, (HI_VOID*)(pstruTmpMsg->u32MessageID), pstruTmpMsg->s32Para1);
    free((HI_VOID*)(pstruTmpMsg->u32MessageID));
    HI_OSMsgRelease(pMsgQueue, (HI_VOID *)pstruTmpMsg);
#endif

	HI_U8 *buf = NULL;
	buf = HI_OSMsgReceive((HI_MsgQueue_p)u32QueueID, u32TimeInMS);
    	//ENREACH_RETURN((), OS_ERROR);
	if (HI_NULL == buf)
	{
		return OS_ERROR;
	}

	memcpy(pBuf, buf, u32BufLen);
	HI_OSMsgRelease((HI_MsgQueue_p)u32QueueID, buf);
	
	return OS_OK;
}

/**********************************************************************
 * ����Ϣ���з�����Ϣ���ڸú����з�����ڴ���receive�ӿ��ͷ�
 * 
 * @param u32QueueID   : ��Ϣ����ID
 * @param pMsg         : ��Ϣ����
 * @param u32MsgLen    : ��Ϣ����
 *
 * @return HI_U32
 **********************************************************************/ 
HI_OS_STATUS HI_OsMsgQueueSend(HI_U32 u32QueueID,
                               HI_VOID* pMsg,
                               HI_U32 u32MsgLen)
{

#if 0
    HI_U8 *pMsgBuf = NULL;
    HI_ENRC_MSG_MESSAGE_S *pstruMsg = NULL; 
    HI_MsgQueue_p pMsgQueue = (HI_MsgQueue_p)u32QueueID;

	EVM_DEBUG("Enter, send queueId:%u", u32QueueID);


    pMsgBuf = malloc(u32MsgLen);
    ENREACH_RETURN((NULL == pMsgBuf), OS_ERROR);

    /*  get a free msg */
    pstruMsg = HI_OSMsgClaim(pMsgQueue, 0);

    if (HI_NULL == pstruMsg)
    {
        free(pMsgBuf);
        return OS_ERROR;
    }

    memcpy(pMsgBuf, pMsg, u32MsgLen);
    pstruMsg->u32MessageID = (HI_U32)pMsgBuf;
    pstruMsg->s32Para1     = u32MsgLen;

    /* send msg */
    HI_OSMsgSend(pMsgQueue, (HI_VOID *)pstruMsg);
#endif

	HI_U8 *msgbuf = NULL;

	msgbuf = HI_OSMsgClaim((HI_MsgQueue_p) u32QueueID, 0);
	ENREACH_RETURN((NULL == msgbuf), OS_ERROR);
	memcpy(msgbuf, pMsg, u32MsgLen);
	HI_OSMsgSend((HI_MsgQueue_p)u32QueueID, msgbuf);

	return OS_OK;
}


#if 0
/*------------------------------------------------------------------------------
		add by jlin
------------------------------------------------------------------------------*/
HI_S32 getMutexNum(HI_S32 u32MutexId)
{
	
    HI_S32 i = 0;

    if (HI_FALSE == g_bSemAttr_Ini)
    {
        for (i = 0; i < MAX_SEM_NUM; i++)
        {
            g_struSem_Attr[i].u32SemId = INVALID_SEM_ID;
        }

        g_bSemAttr_Ini = HI_TRUE;
    }

    for (i = 0; i < MAX_SEM_NUM; i++)
    {
        if (u32SemId != g_struSem_Attr[i].u32SemId)
        {
            continue;
        }

        break;
    }

    return i;
	
}

HI_S32 HI_MutexCreate(HI_S32 u32InitValue)
{
		
}

HI_S32  HI_MutexDelete()
{
	
}

/*---------------------------------------------------------------------------*/


#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

