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
���ܣ�����һ���߳�/����
ԭ�ͣ�UINT32_T ipanel_porting_task_create(CONST CHAR_T *name, IPANEL_TASK_PROC func, VOID *param,INT32_T priority, UINT32_T stack_size)
����˵����
  ���������
    name��һ��������ֽڳ��ַ�����ϵͳ���߳�����Ӧ��Ψһ��
    func���߳����庯����ڵ�ַ������ԭ�Ͷ������£�
    typedef VOID (*IPANEL_TASK_PROC)(VOID *param);
    param���߳����庯���Ĳ����б�ָ��(����ΪIPANEL_NULL)��
    priority�����ȼ���(ipanel���ȼ���0��31��0���,31���)��
    stack_size��ջ��С�����ֽ�Ϊ��λ
  �����������
��    �أ�
  != IPANEL_NULL���ɹ��������߳�ʵ�������
  == IPANEL_NULL��ʧ��
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
���ܣ�����һ���߳�/����
ԭ�ͣ�INT32_T ipanel_porting_task_destroy(UINT32_T task_handle)
����˵����
  ���������task_handle���߳̾��(��0�Ҵ��ڣ���Ч)��
  �����������
��    �أ�
  IPANEL_OK���ɹ���
  IPANEL_ERR��ʧ��
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
���ܣ�����ǰ�̹߳���ָ��ʱ�䣬ͬʱ�ó�CPU�������߳�ʹ�á�
ԭ�ͣ�VOID ipanel_porting_task_sleep(INT32_T ms)
����˵����
  ���������ms�������̵߳�ʱ�䳤�ȣ���λΪ���롣
  �����������
��    �أ���
********************************************************************************************************/
VOID ipanel_porting_task_sleep(INT32_T ms)
{
#ifdef HAVE_OS_DEBUG
	IPANEL_DEBUG("[ipanel_porting_task_sleep] ms = %d\n", ms);
#endif
	FYF_API_thread_sleep(ms);
}

/********************************************************************************************************
���ܣ�����һ���ź�����iPanel MiddleWareֻʹ�û����ź�������ʹ�ü����ź�����
ԭ�ͣ�UINT32_T ipanel_porting_sem_create(CONST CHAR_T *name, INT32_T initialTokenCount,UINT32_T taskWaitMode)
����˵����
  ���������
    name��һ��������ַ����ַ�����ϵͳ���ź���������Ӧ��Ψһ��
    initialTokenCount������ʼ������(ֻ��0��1��Ч)
    taskWaitMode����������������ź�����Чʱ���ȴ����ź������̻߳������Ĵ���������ѡ�
      - IPANEL_TASK_WAIT_FIFO - �������ȳ��ķ�ʽ�ڵȴ��߳��зַ���Ϣ
      - IPANEL_TASK_WAIT_PRIO - ������������ȼ����̡߳�
  �����������
��    �أ�
  != IPANEL_NULL���ɹ��������߳�ʵ�������
  == IPANEL_NULL��ʧ��
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
���ܣ�����һ���ź�����
ԭ�ͣ�INT32_T ipanel_porting_sem_destroy(UINT32_T sem_handle)
����˵����
  ���������sem_handle���ź����������ipanel_porting_sem_create��á�
  �����������
��    �أ�
  IPANEL_OK���ɹ���
  IPANEL_ERR��ʧ��
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
���ܣ��ź����ȴ���
ԭ�ͣ�INT32_T ipanel_porting_sem_wait(UINT32_T sem_handle, INT32_T milliSecsToWait)
����˵����
  ���������
    sem_handle���ź����������ipanel_porting_sem_create��á�
    milliSecsToWait���ȴ�ʱ�䣬��λΪ���롣ΪIPANEL_NO_WAIT(0)ʱ��ʾ���ȴ��������أ�ΪIPANEL_WAIT_FOREVER(-1)��ʾ���õȴ�
  �����������
��    �أ�
  IPANEL_OK���ɹ���
  IPANEL_ERR��ʧ��
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
���ܣ��ͷ��ź�����
ԭ�ͣ�INT32_T ipanel_porting_sem_release(UINT32_T sem_handle)
����˵����
  ���������sem_handle���ź����������ipanel_porting_sem_create��á�
  �����������
��    �أ�
  IPANEL_OK���ɹ���
  IPANEL_ERR��ʧ��
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
���ܣ�����һ����Ϣ���С�ÿ����Ϣ�ṹ����ΪIPANEL_QUEUE_MESSAGE������Ϊ16�ֽڡ�
ԭ�ͣ�UINT32_T ipanel_porting_queue_create(CONST CHAR_T *name, UINT32_T maxQueueLength,UINT32_T taskWaitMode)
����˵����
  ���������
    name��һ��������ַ����ַ�����ϵͳ�ж��е�����Ӧ��Ψһ��
    maxQueueLength��������ָ������������ܱ������Ϣ����������������Ϣ�ﵽ���ֵʱ������������������Ϸ���Ϣ�Ĳ�����ʧ��
    taskWaitMode�������������������������Ϣʱ���ȴ��ڴ˶����ϵ��̻߳������Ĵ���������ѡ�
      - IPANEL_TASK_WAIT_FIFO �������ȳ��ķ�ʽ�ڵȴ��߳��зַ���Ϣ
      - IPANEL_TASK_WAIT_PRIO������������ȼ����̡߳�
  �����������
��    �أ�
  != IPANEL_NULL: �ɹ�����Ϣ���о��;
  == IPANEL_NULL: ʧ��
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
���ܣ�����һ����Ϣ���С�
ԭ�ͣ�INT32_T ipanel_porting_queue_destroy(UINT32_T queue_handle)
����˵����
  ���������queue_handle����Ϣ���о������ipanel_porting_queue_create��á�
  �����������
��    �أ�
  IPANEL_OK:�ɹ�;
  IPANEL_ERR:ʧ�ܡ�
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
���ܣ�������Ϣ��ע�⣺�������غ�msgָ��Ľṹ�е����ݾ���Ч�ˣ�����ʵ����Ҫע���Լ����������Ϣ��
ԭ�ͣ�INT32_T ipanel_porting_queue_send(UINT32_T queue_handle, IPANEL_QUEUE_MESSAGE *msg)
����˵����
  ���������
    queue_handle����Ϣ���о������ipanel_porting_queue_create��á�
    msg��ָ��һ��IPANEL_QUEUE_MESSAGE�ṹ��һ��ָ�룬�ṹ�а���Ҫ�ŵ���Ϣ�����е���Ϣ
  �����������
��    �أ�
  IPANEL_OK:�ɹ�;
  IPANEL_ERR:ʧ�ܡ�
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
���ܣ���ָ�����н�����Ϣ����Ϣ������Ҫ֧��ָ��ʱ��ĳ�ʱ�ȴ�ģʽ��
ԭ�ͣ�INT32_T ipanel_porting_queue_recv(UINT32_T queue_handle, IPANEL_QUEUE_MESSAGE *msg,INT32_T waitMode)
����˵����
  ���������
    queue_handle����Ϣ���о������ipanel_porting_queue_create��á�
    msg��ָ��һ��IPANEL_QUEUE_MESSAGE�ṹ��һ��ָ�룬����Ӷ������յ�����Ϣ������Ϊ�ա�
    waitMode���ȴ�ʱ�䣬��λΪ���롣ΪIPANEL_NO_WAIT(0)ʱ��ʾ�������أ�ΪIPANEL_WAIT_FOREVER(-1)��ʾ���õȴ���
  �����������
��    �أ�
  IPANEL_OK:�ɹ�;
  IPANEL_ERR:ʧ�ܡ�
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

/*��˼ƽ̨���ӿ�û�� wcslen ���ַ�����*/
INT32_T wcslen(const char *schar)
{

    return 0;

}

/*add by zgp 20081109 end, AI7D03358*/
