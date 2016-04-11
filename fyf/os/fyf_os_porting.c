/*****************************************************************************
 *
 * Description:
 *
 *
 * Revision History:
 *    Date          Author         Modification
 * ---------------------------------------------------------------------
 * 1. 2009-08-01    caiwende
 *
 *****************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>

#include "hi_stb_api.h"
#include "hi_msg_queue.h"
#include "fyf_def.h"

#define FYF_OS_DBG printf
#define __FYF_THREAD_PORTING__
#ifdef  __FYF_THREAD_PORTING__
#include "os/fyf_thread.h"

#define MAX_THREAD_NUM     30
#define MAX_MUTEX_NUM	   30

BU08  fyf_os_thread_initial = HI_FALSE;
BU08  fyf_os_mutex_initial = HI_FALSE;
typedef struct fyf_Thread_S
{
	HI_BOOL 			u32ThreadFlag;                       /*线程标志*/
	HI_BOOL 			u32ThreadRunFlag;                 /*线程运行标志0 挂起1 运行*/
	pthread_t  			Thread;
} FYF_THREAD_S;


typedef struct fyf_mutex_S
{
	HI_BOOL  u8MutexFlag;
	pthread_mutex_t mutex;
}FYF_MUTEX_S;

FYF_THREAD_S fyf_thread[MAX_THREAD_NUM];
FYF_MUTEX_S  fyf_mutex[MAX_MUTEX_NUM];
//创建的线程没有区分优先级
/******************************************************************************/
/*Description: create thread                                                  */
/*Input      : thread name thread process function, function parameter,       */
/*             priority, stack size, arg1, arg2                               */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_CPI_thread_create_porting(const PU08 name,FYF_THREAD_PROC func,void * param,BS32 priority,BS32 stack_size)
{
	pthread_t	thread;
	BU32 ret;
	BS32 i;
	if (fyf_os_thread_initial == HI_FALSE)
	{
		for (i=0;i<MAX_THREAD_NUM;i++)
		{
			fyf_thread[i].u32ThreadFlag = HI_FALSE;
			fyf_thread[i].Thread = 0;
		}
		fyf_os_thread_initial = HI_TRUE;

	}
	for (i = 0; i < MAX_THREAD_NUM; i++)
	{
		if (fyf_thread[i].u32ThreadFlag == HI_FALSE)
			break;
	}

	if (i >= MAX_THREAD_NUM)
	{
		printf("+++ [%s] [%d] create task fail! +++\n", __FUNCTION__, __LINE__);
		return FYF_NULL;
	}
	ret = pthread_create(&fyf_thread[i].Thread, NULL, (void *)func, NULL);

	if (ret != 0)
	{
		printf("thread %s create fail!\n", name);
		printf("+++ [%s] [%d] create task fail! +++\n", __FUNCTION__, __LINE__);
		return FYF_NULL;
	}
	else
	{
		fyf_thread[i].u32ThreadRunFlag = HI_TRUE;
		fyf_thread[i].u32ThreadFlag = HI_TRUE;

		FYF_OS_DBG("thread %s create success! %d %d %d\n", name, i + 1, fyf_thread[i].Thread, pthread_self());
		return (BU32)(i + 1);
	}

}
/******************************************************************************/
/*Description: destroy thread                                                 */
/*Input      : thread handle                                                  */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_CPI_thread_destroy_porting(BU32 handle)
{
	BU32 index = handle - 1;
	pthread_cancel(fyf_thread[index].Thread);
	fyf_thread[index].u32ThreadFlag = HI_FALSE;
	fyf_thread[index].u32ThreadRunFlag = HI_FALSE;
	return FYF_OK;

}
/******************************************************************************/
/*Description: thread  sleep                                                  */
/*Input      : the time of thread to sleep                                    */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_CPI_thread_sleep_porting(BS32 ms)
{
	usleep(1000 * ms);
#if 0
	int err = 0;
	struct timespec x, rem;
	/* if < 2ms */
	if (ms <= 2)
	{
		/* Waits 2ms */
		x.tv_sec  = 0;
		x.tv_nsec = 2000001;
	}
	else
	{
		x.tv_sec  = ms / 1000;
		x.tv_nsec = (ms % 1000) * 1000 * 1000;
	}
	while (((err = nanosleep (&x, &rem))== -1) && (errno == EINTR))
	{
		x=rem;
	}
#endif
}
/******************************************************************************/
/*Description: get cur thread id                                                  */
/*Input      :                                     */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/

BU32 FYF_CPI_thread_getCurID_porting()
{
	pthread_t pid;
	pid = pthread_self();
	return (BU32)pid;
}

static FYF_THREAD_PORTING FYF_THREAD_porting=
{
	FYF_CPI_thread_create_porting,
	FYF_CPI_thread_destroy_porting,
	FYF_CPI_thread_sleep_porting,
	FYF_CPI_thread_getCurID_porting
};
#endif
#define __FYF_SEMAPHORE_PORTING__
#ifdef  __FYF_SEMAPHORE_PORTING__
#include "os/fyf_semaphore.h"
/******************************************************************************/
/*Description: create semaphore                                               */
/*Input      : initial token count, task wait mode                            */
/*Output     : no                                                             */
/*Return     : >0 handle, success, 0 failure                                  */
/******************************************************************************/
BU32 FYF_CPI_semaphore_create_porting(const PU08 name,BS32 initial_count,BU32 wait_mode)
{
	BU32 semaphoreid;
	BS32 ret;
	ret = HI_OS_SemaphoreCreate(initial_count, &semaphoreid);
	FYF_OS_DBG("semaphoreid = %d\n", semaphoreid);

	if (ret != HI_SUCCESS)
	{
		//	printf("%s semaphore create failure\n", name);
		return FYF_NULL;
	}
	else
	{
		//FYF_OS_DBG("%s semaphore create success\n", name);
		return semaphoreid;
	}
}
/******************************************************************************/
/*Description: destroy semaphore                                              */
/*Input      : semaphore handle                                               */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_CPI_semaphore_destroy_porting(BU32 handle)
{
	HI_OS_SemaphoreDelete(handle);
//	sem_t sem = (*(sem_t*)handle);
//	sem_destroy(&sem);
	return FYF_OK;

}
/******************************************************************************/
/*Description: wait semaphore                                                 */
/*Input      : semaphore handle                                               */
/*Output     : no                                                             */
/*Return     : FYF_OS_QUEUE_SEM_STATUS_AVAILABLE success,                     */
/*              FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE failure                   */
/******************************************************************************/
BS32 FYF_CPI_semaphore_wait_porting(BU32 handle,BS32 wait_time)
{
	BS32 ret;
	//sem_t sem = (*(sem_t*)handle);
	//struct timespec abstime;

	switch (wait_time)
	{
		case FYF_NO_WAIT:
			ret = HI_OS_SemaphoreWait(handle, 1);
			//while (((ret = sem_trywait(&sem)) != 0 ) && (errno == EINTR));
			//ret =  sem_trywait(&sem);
			break;
		case FYF_WAIT_FOREVER:
			ret = HI_OS_SemaphoreWait(handle, 0xffffffff);
			//ret = sem_wait(&sem);
			//while (((ret = sem_wait(&sem)) != 0) && (errno == EINTR));
			break;
		default:
			ret = HI_OS_SemaphoreWait(handle, wait_time);
#if 0
			clock_gettime(CLOCK_REALTIME, &abstime);    //获取当前时间
			abstime.tv_sec += (wait_time / 1000);        //加上等待时间的秒数
			abstime.tv_nsec += (wait_time % 1000) * 1000; //加上等待时间纳秒数
			while (((ret = sem_timedwait(&sem, &abstime)) != 0) && (errno == EINTR));
#endif
			break;
	}

	if (HI_SUCCESS == ret)
	{
		return FYF_OS_QUEUE_SEM_STATUS_AVAILABLE;
	}
	else
	{
		return FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE;
	}

}
/******************************************************************************/
/*Description: release semaphore                                              */
/*Input      : semaphore handle                                               */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_CPI_semaphore_release_porting(BU32 handle)
{
	HI_OS_SemaphoreSignal(handle);
	//sem_post((sem_t*)handle);
	return FYF_OK;
}
static FYF_SEMAPHORE_PORTING FYF_SEMAPHORE_porting=
{
	FYF_CPI_semaphore_create_porting,
	FYF_CPI_semaphore_destroy_porting,
	FYF_CPI_semaphore_wait_porting,
	FYF_CPI_semaphore_release_porting
};
#endif
#define __FYF_QUEUE_PORTING__
#ifdef  __FYF_QUEUE_PORTING__
#include "os/fyf_queue.h"
/******************************************************************************/
/*Description: message queue create                                           */
/*Input      : max queue length, wait mode                                    */
/*Output     : no                                                             */
/*Return     : >0 success, <=0 failure                                        */
/******************************************************************************/
BU32 FYF_CPI_queue_create_porting(const PU08 name,BU32 len,BU32 wait_mode)
{

	HI_MsgQueue_p msg_queue = NULL;
	FYF_OS_DBG("FYF_QUEUE_MESSAGE = %d\n", sizeof(FYF_QUEUE_MESSAGE));
	switch (wait_mode)
	{
		case FYF_THREAD_WAIT_FIFO:
			msg_queue = HI_OSMsgQueueCreate(32, len);//message_create_queue(32, len);
			break;
		case FYF_THREAD_WAIT_PRIO:
			msg_queue = HI_OSMsgQueueCreate(32, len);
			break;
		default:
			break;
	}

	if (msg_queue == NULL)
	{
		return FYF_NULL;
	}
	else
	{
		return (BU32)msg_queue;
	}

}

/******************************************************************************/
/*Description: message queue destroy                                           */
/*Input      : queue handle                                                    */
/*Output     : no                                                              */
/*Return     : 0 success, -1 failure                                           */
/*******************************************************************************/
BS32 FYF_CPI_queue_destroy_porting(BU32 handle)
{
	HI_OSMsgQueueDel((HI_MsgQueue_p)handle);
//	message_delete_queue((message_queue_t *)handle);
	return FYF_OK;
}
/******************************************************************************/
/*Description: message queue send                                             */
/*Input      : queue handle, message                                          */
/*Output     : no                                                             */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_CPI_queue_send_porting(BU32 handle,FYF_QUEUE_MESSAGE * msg)
{
	FYF_QUEUE_MESSAGE *tmpmsg = NULL;

	if ((tmpmsg = (FYF_QUEUE_MESSAGE*)HI_OSMsgClaim((HI_MsgQueue_p)handle, 0)) != NULL)
	{

		tmpmsg->qlstWordOfMsg = msg->qlstWordOfMsg;
		tmpmsg->q2ndWordOfMsg = msg->q2ndWordOfMsg;
		tmpmsg->q3rdWordOfMsg = msg->q3rdWordOfMsg;
		tmpmsg->q4thWordOfMsg = msg->q4thWordOfMsg;
		HI_OSMsgSend((HI_MsgQueue_p)handle, tmpmsg);

		return FYF_OK;
	}
	else
	{
		return FYF_ERR;
	}

}
/******************************************************************************/
/*Description: message queue receive                                          */
/*Input      : queue handle, address to record message, wait mode, wait time  */
/*Output     : no                                                             */
/*Return     : FYF_OS_QUEUE_SEM_STATUS_AVAILABLE success,                     */
/*             FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE failure                    */
/******************************************************************************/
BS32 FYF_CPI_queue_recv_porting(BU32 handle,FYF_QUEUE_MESSAGE * msg,BS32 wait_time)
{

	HI_MsgQueue_p msg_queue = (HI_MsgQueue_p)handle;
	FYF_QUEUE_MESSAGE * tmpmsg = NULL;

	switch (wait_time)
	{
		case FYF_NO_WAIT:
			tmpmsg = (FYF_QUEUE_MESSAGE *)HI_OSMsgReceiveImmediate(msg_queue);
			break;
		case FYF_WAIT_FOREVER:
			tmpmsg = (FYF_QUEUE_MESSAGE *)HI_OSMsgReceive(msg_queue, 0);
			break;
		default:
			tmpmsg = (FYF_QUEUE_MESSAGE *)HI_OSMsgReceive(msg_queue, wait_time);
			break;
	}

	if (tmpmsg == NULL)
	{
		return FYF_OS_QUEUE_SEM_STATUS_UNAVAILABLE;
	}
	else
	{
		msg->qlstWordOfMsg = tmpmsg->qlstWordOfMsg;
		msg->q2ndWordOfMsg = tmpmsg->q2ndWordOfMsg;
		msg->q3rdWordOfMsg = tmpmsg->q3rdWordOfMsg;
		msg->q4thWordOfMsg = tmpmsg->q4thWordOfMsg;
		HI_OSMsgRelease(msg_queue, tmpmsg);

		return FYF_OS_QUEUE_SEM_STATUS_AVAILABLE;
	}


}
static FYF_QUEUE_PORTING FYF_QUEUE_porting=
{
	FYF_CPI_queue_create_porting,
	FYF_CPI_queue_destroy_porting,
	FYF_CPI_queue_send_porting,
	FYF_CPI_queue_recv_porting
};
#endif

//add by cwd 2009-12-15
#define __FYF_MUTEX_PORTING__
#ifdef __FYF_MUTEX_PORTING__
#include "os/fyf_mutex.h"
BU32 FYF_API_mutex_create(void)
{
	BS32 i;
	BS32 ret = -1;
	if (fyf_os_mutex_initial == HI_FALSE)
	{
		for (i = 0; i < MAX_MUTEX_NUM; i++)
		{
			fyf_mutex[i].u8MutexFlag = HI_FALSE;
		}
		fyf_os_mutex_initial = HI_TRUE;
	}

	for (i = 0; i < MAX_MUTEX_NUM; i++)
	{
		if (fyf_mutex[i].u8MutexFlag == HI_FALSE)
			break;
	}

	if (i >= MAX_MUTEX_NUM)
	{
		printf("+++ [%s] [%d] mutex create fail! +++\n", __FUNCTION__, __LINE__);
		return FYF_NULL;
	}

	ret = pthread_mutex_init(&(fyf_mutex[i].mutex), NULL);
	if (ret == 0)
	{
		fyf_mutex[i].u8MutexFlag = HI_TRUE;
		return (i + 1);
	}
	else
	{
		printf("+++ [%s] [%d] mutex create fail! +++\n", __FUNCTION__, __LINE__);
		return FYF_NULL;
	}

}
BS32 FYF_API_mutex_delete(BU32 handle)
{
	BS32 index = handle - 1;
	BS32 ret;
	if (index < 0 || index >= MAX_MUTEX_NUM)
	{
		printf("index %d over max mutex num", index);
		return -1;
	}
	ret = pthread_mutex_destroy(&(fyf_mutex[index].mutex));
	if (ret != 0)
	{
		printf("+++ [%s] [%d] mutex destroy fail! +++\n", __FUNCTION__, __LINE__);
		return -1;
	}
	else
	{
		fyf_mutex[index].u8MutexFlag = HI_FALSE;
		return 0;
	}


}
BS32 FYF_API_mutex_lock(BU32 handle,BU32 timeout)
{
	BS32 ret;
	BS32 index = handle - 1;
	if (index < 0 || index >= MAX_MUTEX_NUM)
	{
		printf("index %d over max mutex num", index);
		return -1;
	}
	ret = pthread_mutex_lock(&(fyf_mutex[index].mutex));
	if (ret != 0)
	{
		printf("+++ [%s] [%d] mutex lock fail! +++\n", __FUNCTION__, __LINE__);
		return -1;
	}
	else
	{
		return 0;
	}
}
BS32 FYF_API_mutex_unlock(BU32 handle)
{
	BS32 ret;
	BS32 index = handle - 1;
	if (index < 0 || index >= MAX_MUTEX_NUM)
	{
		printf("index %d over max mutex num", index);
		return -1;
	}
	ret = pthread_mutex_unlock(&(fyf_mutex[index].mutex));
	if (ret != 0)
	{
		printf("+++ [%s] [%d] mutex lock fail! +++\n", __FUNCTION__, __LINE__);
		return -1;
	}
	else
	{
		return 0;
	}
}

#if 0
static FYF_MUTEX_PORTING_s FYF_MUTEX_porting=
{
	FYF_CPI_mutex_create_porting,
	FYF_CPI_mutex_delete_porting,
	FYF_CPI_mutex_lock_porting,
	FYF_CPI_mutex_unlock_porting
};
#endif
#endif

#define __FYF_OS_PORTING__
#ifdef  __FYF_OS_PORTING__

#if 0
void FunThread1()
{
	int i,j;
	int policy;
	struct sched_param param;
	pthread_getschedparam(pthread_self(),&policy,&param);
	if (policy==SCHED_OTHER)
		printf("SCHED_OTHER\n");
	if (policy==SCHED_RR)
		printf("SCHED_RR\n");
	if (policy==SCHED_FIFO)
		printf("SCHED_FIFO\n");
	for (i=1;i <50;i++)
	{
		for (j=1;j <10000000;j++)
		{
		}
		printf("thread 1\n");
	}
	printf("Thread1 exit\n");
}
void FunThread2()
{
	int i, j;
	// for(i=1;i <5000;i++)
	//sched_yield();
	//sched_yield();
	for (i=1;i <50;i++)
	{
		for (j=1;j <10000000;j++)
		{
		}
		printf("thread 2\n");
	}
	printf("Thread2 exit\n");
}
void FunThread3()
{
	int i, j;
	for (i=1;i <50;i++)
	{
		for (j=1;j <10000000;j++)
		{
		}
		printf("thread 3\n");
	}
	printf("Thread3 exit\n");
}

void fyf_thread_create_test()
{

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int inherit;
	pthread_attr_getinheritsched(&attr,&inherit);
	if (inherit==PTHREAD_EXPLICIT_SCHED)
		printf("PTHREAD_EXPLICIT_SCHED\n");
	if (inherit==PTHREAD_INHERIT_SCHED)
		printf("PTHREAD_INHERIT_SCHED\n");
	int detachstate;
	pthread_attr_getdetachstate(&attr,&detachstate);
	if (detachstate==PTHREAD_CREATE_DETACHED)
		printf("PTHREAD_CREATE_DETACHED\n");
	if (detachstate==PTHREAD_CREATE_JOINABLE)
		printf("PTHREAD_CREATE_JOINABLE\n");
	int policy;
	pthread_attr_getschedpolicy(&attr,&policy);
	if (policy==SCHED_OTHER)
		printf("SCHED_OTHER\n");
	if (policy==SCHED_FIFO)
		printf("SCHED_FIFO\n");
	if (policy==SCHED_RR)
		printf("SCHED_RR\n");
	int scope;
	pthread_attr_getscope(&attr,&scope);
	if (scope==PTHREAD_SCOPE_SYSTEM)
		printf("PTHREAD_SCOPE_SYSTEM\n");
	if (scope==PTHREAD_SCOPE_PROCESS)
		printf("PTHREAD_SCOPE_PROCESS\n");
	struct sched_param param;
	pthread_attr_getschedparam(&attr,&param);
	printf("priority:%d\n",param.sched_priority);

	int i;
	int ret;
	i=getuid();
	if (i==0)
		printf("The current user is root\n");
	else
		printf("The current user is not root\n");

	pthread_t ppid1,ppid2,ppid3;
	struct sched_param param;
	param.sched_priority= 1;
	pthread_attr_t attr,attr2, attr3;
	pthread_attr_init(&attr);
	pthread_attr_init(&attr3);
	ret = pthread_attr_init(&attr2);
	printf("ret = %d\n", ret);
	ret = pthread_attr_setschedpolicy(&attr2,SCHED_OTHER);
	ret = pthread_attr_setschedpolicy(&attr3,SCHED_OTHER);
	printf("ret = %d\n", ret);
	//ret = pthread_attr_setschedparam(&attr2,&param);
//	ret = pthread_attr_setschedparam(&attr3,&param);
	printf("ret = %d\n", ret);
	pthread_create(&ppid2,&attr,FunThread2,NULL);
	pthread_create(&ppid3,&attr,FunThread3,NULL);
	pthread_create(&ppid1,&attr,FunThread1,NULL);
	pthread_join(ppid1,NULL);
	pthread_join(ppid2,NULL);
	pthread_join(ppid3,NULL);
	pthread_attr_destroy(&attr);
	pthread_attr_destroy(&attr2);
	pthread_attr_destroy(&attr3);


}
#endif
void FYF_API_os_register(void)
{
#ifdef __FYF_THREAD_PORTING__
	FYF_API_thread_register(&FYF_THREAD_porting);
#endif

#ifdef __FYF_SEMAPHORE_PORTING__
	FYF_API_semaphore_register(&FYF_SEMAPHORE_porting);
#endif

#ifdef __FYF_QUEUE_PORTING__
	FYF_API_queue_register(&FYF_QUEUE_porting);
#endif
//	FYF_API_mutex_register(&FYF_MUTEX_porting);
//	fyf_thread_create_test();
}
#endif


void FYF_API_OS_Set_SysTime(BU32 year,BU32 month,BU32 day,BU32 hour,BU32 min,BU32 sec)
{
	time_t systm;
	struct tm systm_tm;
	struct timeval systm_tv;

//	struct timezone systm_tz;

	systm_tm.tm_year=year -1900;
	systm_tm.tm_mon=month -1;
	systm_tm.tm_mday=day;
	systm_tm.tm_hour=hour;
	systm_tm.tm_min=min;
	systm_tm.tm_sec=sec;

	systm = mktime(&systm_tm);

	systm_tv.tv_sec=systm;
	systm_tv.tv_usec=0;
	if (settimeofday (&systm_tv,NULL) < 0)
	{
		printf("Settimeofday\n");
	}
}
