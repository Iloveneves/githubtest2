/*********************************************************************
    Copyright (c) 2008 - 2010 Embedded Internet Solutions, Inc
    All rights reserved. You are not allowed to copy or distribute
    the code without permission.
    This is the demo implenment of the base Porting APIs needed by iPanel MiddleWare. 
    Maybe you should modify it accorrding to Platform.
    
    Note: the "int" in the file is 32bits
    
    History:
		version		date		name		desc
    
*********************************************************************/
#include "trd_ipanel_init.h"
#include "ipanel_typedef.h"
#include "ipanel_os.h"
#include "ipanel_base.h"

//#define HAVE_OS_DEBUG

#define MAX_QUEUE_NUM	        (16)

/********************************************************************************************************
功能：创建一个线程/任务。
原型：UINT32_T ipanel_porting_task_create(CONST CHAR_T *name, IPANEL_TASK_PROC func, VOID *param,INT32_T priority, UINT32_T stack_size)
参数说明：
  输入参数：
    name：一个最多四字节长字符串，系统中线程名称应该唯一；
    func：线程主体函数入口地址，函数原型定义如下；
    typedef VOID (*IPANEL_TASK_PROC)(VOID *param);
    param：线程主体函数的参数列表指针(可置为IPANEL_NULL)；
    priority：优先级别(ipanel优先级从0到31，0最低,31最高)；
    stack_size：栈大小，以字节为单位
  输出参数：无
返    回：
  != IPANEL_NULL：成功，返回线程实例句柄。
  == IPANEL_NULL：失败
********************************************************************************************************/
UINT32_T ipanel_porting_task_create(const CHAR_T *name, IPANEL_TASK_PROC func, VOID *param,
                                    INT32_T priority, UINT32_T stack_size)
{     	
	BS32 thread_id;
#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_task_create]taskname=%s, func=%p, param=0x%x, prio=%d, stack=0x%x", name, func, param,priority, stack_size);
#endif
	if ((func == NULL) || (stack_size % 0x400) || (stack_size < 0))
	{
		IPANEL_ERROR("[ipanel_porting_task_create]param error!\n");
		return 0;  
	}

	thread_id = FYF_API_thread_create((const PU08)name,  func,  param,  priority,  stack_size);
	if (FYF_NULL == thread_id)
	{
		IPANEL_ERROR("[ipanel_porting_task_create]failed, taskname=%s, func=%p,  prio=%d, stack=0x%x", name, func,  priority, stack_size);
		return 0;
	}

	return thread_id;
}


/********************************************************************************************************
功能：销毁一个线程/任务。
原型：INT32_T ipanel_porting_task_destroy(UINT32_T task_handle)
参数说明：
  输入参数：task_handle：线程句柄(非0且存在，有效)。
  输出参数：无
返    回：
  IPANEL_OK：成功，
  IPANEL_ERR：失败
********************************************************************************************************/
INT32_T ipanel_porting_task_destroy(UINT32_T task_handle)
{
	BS32 thread_id = task_handle;

#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_task_destroy] handle = 0x%x\n", task_handle);
#endif
	if (FYF_OK != FYF_API_thread_destroy(thread_id))
	{	
		IPANEL_DEBUG("[ipanel_porting_task_destroy] failed\n");
		return -1;
	}
	return IPANEL_OK;  	
}


/********************************************************************************************************
功能：将当前线程挂起指定时间，同时让出CPU供其他线程使用。
原型：VOID ipanel_porting_task_sleep(INT32_T ms)
参数说明：
  输入参数：ms：挂起线程的时间长度，单位为毫秒。
  输出参数：无
返    回：无
********************************************************************************************************/
VOID ipanel_porting_task_sleep(INT32_T ms)
{
#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_task_sleep] ms = %d\n", ms);
#endif
	FYF_API_thread_sleep(ms);
}

/********************************************************************************************************
功能：创建一个信号量，iPanel MiddleWare只使用互斥信号量，不使用计数信号量。
原型：UINT32_T ipanel_porting_sem_create(CONST CHAR_T *name, INT32_T initialTokenCount,UINT32_T taskWaitMode)
参数说明：
  输入参数：
    name：一个最多四字符长字符串，系统中信号量的名字应该唯一；
    initialTokenCount：最大初始化计数(只有0和1有效)
    taskWaitMode：这个参数决定当信号量有效时，等待此信号量的线程获得满足的次序，有两个选项：
      - IPANEL_TASK_WAIT_FIFO - 按先入先出的方式在等待线程中分发消息
      - IPANEL_TASK_WAIT_PRIO - 优先满足高优先级的线程。
  输出参数：无
返    回：
  != IPANEL_NULL：成功，返回线程实例句柄。
  == IPANEL_NULL：失败
********************************************************************************************************/
UINT32_T ipanel_porting_sem_create(const CHAR_T *name, INT32_T initialTokenCount,
                          UINT32_T taskWaitMode)
{
	BU32 semid = 0;

#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_sem_create]name=%s, initialTokenCount=%d, taskWaitMode=%d \n ",name,initialTokenCount,taskWaitMode);
#endif
	semid = FYF_API_semaphore_create((const PU08)name, initialTokenCount, taskWaitMode);
	if(FYF_NULL == semid)
	{
		IPANEL_DEBUG("[ipanel_porting_sem_create]sem_init failed, name=%p, initialTokenCount=%d \n ",name,initialTokenCount);
		return 0;
	}

	return semid;
}

                                 
/********************************************************************************************************
功能：销毁一个信号量。
原型：INT32_T ipanel_porting_sem_destroy(UINT32_T sem_handle)
参数说明：
  输入参数：sem_handle：信号量句柄，由ipanel_porting_sem_create获得。
  输出参数：无
返    回：
  IPANEL_OK：成功，
  IPANEL_ERR：失败
********************************************************************************************************/
INT32_T ipanel_porting_sem_destroy(UINT32_T sem_handle)
{
	BS32 ret = -1;

	if (0 == sem_handle)
	{
		IPANEL_ERROR("ipanel_porting_sem_destroy, sem_handle = NULL");
		return IPANEL_ERR;
	}
	
#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_sem_destroy]sem_handle=0x%x \n",sem_handle);
#endif
	ret = FYF_API_semaphore_destroy(sem_handle);
	if(FYF_OK != ret)
	{
		IPANEL_DEBUG("[ipanel_porting_sem_destroy] failed, sem_handle=0x%x \n",sem_handle);
		return IPANEL_ERR;	
	}
	
	return IPANEL_OK;

}

/********************************************************************************************************
功能：信号量等待。
原型：INT32_T ipanel_porting_sem_wait(UINT32_T sem_handle, INT32_T milliSecsToWait)
参数说明：
  输入参数：
    sem_handle：信号量句柄，由ipanel_porting_sem_create获得。
    milliSecsToWait：等待时间，单位为毫秒。为IPANEL_NO_WAIT(0)时表示不等待立即返回，为IPANEL_WAIT_FOREVER(-1)表示永久等待
  输出参数：无
返    回：
  IPANEL_OK：成功，
  IPANEL_ERR：失败
********************************************************************************************************/
//INT32_T ipanel_porting_sem_wait(UINT32_T sem_handle, INT32_T milliSecsToWait)

INT32_T ipanel_porting_sem_wait(UINT32_T sem_handle, UINT32_T waitMode,
                               INT32_T milliSecsToWait)
{
	BS32 ret;

#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_sem_wait]sem_handle=0x%x, milliSecsToWait=%d, waitMode = %d \n",sem_handle,milliSecsToWait,waitMode);
#endif

    if (0 == sem_handle || milliSecsToWait < 0)
    {
        return EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE;
    }
	
    if (waitMode != EIS_QUEUE_SEM_WAIT && waitMode != EIS_QUEUE_SEM_NOWAIT)
    {
        return EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE;
    }

    if (waitMode == EIS_QUEUE_SEM_NOWAIT)
    {
		ret = FYF_API_semaphore_wait(sem_handle, 0);
		if(FYF_OK == ret)
		{	
			return EIS_OS_QUEUE_SEM_STATUS_AVAILABLE;
		}
		
	}
    else  if (milliSecsToWait == 0)
    {
       	ret = FYF_API_semaphore_wait(sem_handle, 0xffffffff);
       	if(ret == FYF_OK)
		{	
			return EIS_OS_QUEUE_SEM_STATUS_AVAILABLE;
		}
	}
	else
	{
       	ret = FYF_API_semaphore_wait(sem_handle, milliSecsToWait);
		if(ret == FYF_OK)
		{		
			return EIS_OS_QUEUE_SEM_STATUS_AVAILABLE;
		}
	}

	return EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE;
}


/********************************************************************************************************
功能：释放信号量。
原型：INT32_T ipanel_porting_sem_release(UINT32_T sem_handle)
参数说明：
  输入参数：sem_handle：信号量句柄，由ipanel_porting_sem_create获得。
  输出参数：无
返    回：
  IPANEL_OK：成功，
  IPANEL_ERR：失败
********************************************************************************************************/
INT32_T ipanel_porting_sem_release(UINT32_T sem_handle)
{
	BS32 ret;

	if (0 == sem_handle)
	{
		IPANEL_ERROR("ipanel_porting_sem_release, sem_handle = NULL");
		return IPANEL_ERR;
	}

#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_sem_release] sem_handle=0x%x \n",sem_handle);
#endif

	ret = FYF_API_semaphore_release(sem_handle);
	if (ret == FYF_OK) 
	{
		return IPANEL_OK;
	}
	IPANEL_DEBUG("[ipanel_porting_sem_release] failed \n");
	return IPANEL_ERR;
}

/********************************************************************************************************
功能：创建一个消息队列。每个消息结构定义为IPANEL_QUEUE_MESSAGE，长度为16字节。
原型：UINT32_T ipanel_porting_queue_create(CONST CHAR_T *name, UINT32_T maxQueueLength,UINT32_T taskWaitMode)
参数说明：
  输入参数：
    name：一个最多四字符长字符串，系统中队列的名字应该唯一；
    maxQueueLength：本参数指定队列中最多能保存的消息个数。当队列中消息达到这个值时，后续的在这个队列上发消息的操作会失败
    taskWaitMode：这个参数决定当队列中有消息时，等待在此队列上的线程获得满足的次序，有两个选项：
      - IPANEL_TASK_WAIT_FIFO 按先入先出的方式在等待线程中分发消息
      - IPANEL_TASK_WAIT_PRIO优先满足高优先级的线程。
  输出参数：无
返    回：
  != IPANEL_NULL: 成功，消息队列句柄;
  == IPANEL_NULL: 失败
********************************************************************************************************/
UINT32_T ipanel_porting_queue_create(CONST CHAR_T *name, UINT32_T maxQueueLength,
                            UINT32_T taskWaitMode)
{
	unsigned int msgId = 0;

#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_queue_create]name=%s, maxQueueLength=%d \n",name,maxQueueLength);
#endif

	if (name == NULL) 
	{
		IPANEL_ERROR("[ipanel_porting_queue_create] failed, name is NULL \n");
		return 0;
	}

	msgId = FYF_API_queue_create((const PU08)name, maxQueueLength, taskWaitMode);
	if(FYF_NULL == msgId)
	{
		IPANEL_DEBUG("[ipanel_porting_queue_create] failed\n");
		return 0;
	}

	return msgId;
}

/********************************************************************************************************
功能：销毁一个消息队列。
原型：INT32_T ipanel_porting_queue_destroy(UINT32_T queue_handle)
参数说明：
  输入参数：queue_handle：消息队列句柄，由ipanel_porting_queue_create获得。
  输出参数：无
返    回：
  IPANEL_OK:成功;
  IPANEL_ERR:失败。
********************************************************************************************************/
INT32_T ipanel_porting_queue_destroy(UINT32_T queue_handle)
{

	BS32 ret = IPANEL_ERR;

#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_queue_destroy]queue_handle=0x%x \n",queue_handle);
#endif

	if (0 == queue_handle)
	{
		IPANEL_ERROR("ipanel_porting_queue_destroy, queue_handle = NULL");
		return IPANEL_ERR;
	}

	ret = FYF_API_queue_destroy(queue_handle);
	if(FYF_OK != ret)
	{
		IPANEL_DEBUG("[ipanel_porting_queue_destroy] failed, queue_handle=0x%x \n",queue_handle);
		return IPANEL_ERR;	
	}

	return IPANEL_OK;
	
}

/********************************************************************************************************
功能：发送消息。注意：函数返回后msg指向的结构中的内容就无效了，函数实现者要注意自己保存相关信息。
原型：INT32_T ipanel_porting_queue_send(UINT32_T queue_handle, IPANEL_QUEUE_MESSAGE *msg)
参数说明：
  输入参数：
    queue_handle：消息队列句柄，由ipanel_porting_queue_create获得。
    msg：指向一个IPANEL_QUEUE_MESSAGE结构的一个指针，结构中包含要放到消息队列中的消息
  输出参数：无
返    回：
  IPANEL_OK:成功;
  IPANEL_ERR:失败。
********************************************************************************************************/
INT32_T ipanel_porting_queue_send(UINT32_T queue_handle, IPANEL_QUEUE_MESSAGE *msg)
{
	BS32 ret = -1;
	
#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_queue_send]queue_handle=0x%x,msg=%p \n",queue_handle,msg);
#endif

	if (0 == queue_handle)
	{
		IPANEL_ERROR("ipanel_porting_queue_send, queue_handle = NULL");
		return IPANEL_ERR;
	}

	ret = FYF_API_queue_send(queue_handle, (FYF_QUEUE_MESSAGE *)msg);
	if(FYF_OK != ret)
	{
		IPANEL_DEBUG("[ipanel_porting_queue_send] failed, queue_handle=0x%x \n",queue_handle);
		return IPANEL_ERR;	
	}

	return IPANEL_OK;

}


/********************************************************************************************************
功能：从指定队列接收消息，消息队列需要支持指定时间的超时等待模式。
原型：INT32_T ipanel_porting_queue_recv(UINT32_T queue_handle, IPANEL_QUEUE_MESSAGE *msg,INT32_T waitMode)
参数说明：
  输入参数：
    queue_handle：消息队列句柄，由ipanel_porting_queue_create获得。
    msg：指向一个IPANEL_QUEUE_MESSAGE结构的一个指针，保存从队列中收到的消息，不能为空。
    waitMode：等待时间，单位为毫秒。为IPANEL_NO_WAIT(0)时表示立即返回，为IPANEL_WAIT_FOREVER(-1)表示永久等待。
  输出参数：无
返    回：
  IPANEL_OK:成功;
  IPANEL_ERR:失败。
********************************************************************************************************/
INT32_T ipanel_porting_queue_recv(UINT32_T queue_handle, IPANEL_QUEUE_MESSAGE *msg,
                                  INT32_T waitMode)
{
	BS32 ret = IPANEL_ERR;

#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_queue_recv]queue_handle=0x%x,msg=%p,waitMode=%d \n",queue_handle,msg,waitMode);
#endif

	if (0 == queue_handle)
	{
		IPANEL_ERROR("FYF_API_queue_recv, queue_handle = NULL");
		return IPANEL_ERR;
	}
	

	ret = FYF_API_queue_recv(queue_handle,  (FYF_QUEUE_MESSAGE *)msg, waitMode);
	if(FYF_OK != ret)
	{
		IPANEL_DEBUG("[ipanel_porting_queue_recv] failed, queue_handle=0x%x \n",queue_handle);
		return IPANEL_ERR;	
	}

	return IPANEL_OK;}

/*海思平台链接库没有 wcslen 宽字符函数*/
INT32_T wcslen(const char *schar)
{

    return 0;

}

/*add by zgp 20081109 end, AI7D03358*/
