/*********************************************************************
Copyright (c) 2007 by Embedded Internet Solutions, Inc.
All rights reserved. You are not allowed to copy or distribute
the code without permission.
*********************************************************************/

#ifndef __IPANEL_PORTING_TASK_H______
#define __IPANEL_PORTING_TASK_H______

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


/*add for ipanel 2.0.1*/

#define IPANEL_OK	0
#define IPANEL_ERR	(-1)
#define IPANEL_NULL (void *)0


typedef  VOID (*IPANEL_TASK_PROC)(VOID *param);

typedef struct
{
    UINT32_T q1stWordOfMsg;                /* First word of a queue message.  */
    UINT32_T q2ndWordOfMsg;                /* Second word of a queue message. */
    UINT32_T q3rdWordOfMsg;                /* Third word of a queue message.  */
    UINT32_T q4thWordOfMsg;                /* Fourth word of a queue message. */
} IPANEL_QUEUE_MESSAGE;





/*add end */



/* ipanel_porting_sem_wait(), �ȴ�ģʽ��, ���semaphore��available��һֱ�ȵĲ���(����������). */
/* ipanel_porting_queue_recv(), �ȴ�ģʽ��, �������Ϣ��һֱ�ȵĲ���(����������). */
#define EIS_QUEUE_SEM_WAIT_FOREVER			0

/* 
** constants for returning status of waiting semaphore(P operation)
*/
enum
{
    EIS_OS_QUEUE_SEM_STATUS_NONE,
    EIS_OS_QUEUE_SEM_STATUS_AVAILABLE,   // queue_wait/semaphore_wait is available(value>0, not locked, etc)
    EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE, // queue_wait/semaphore_wait is unavailable(value<=0, locked, etc)
    EIS_OS_QUEUE_SEM_STATUS_UNKNOWN
};

/*
used function:
os_queue_recv,  os_sem_wait
*/
enum
{
	EIS_QUEUE_SEM_WAIT,
	EIS_QUEUE_SEM_NOWAIT	//���ʹ����NOWAITģʽ,��ʹ�÷���ֵ�ж��Ƿ������ȵ�.
};
/*
used function:
os_queue_create, os_sem_create
*/
enum
{
	EIS_TASK_WAIT_FIFO,	//�Ƽ�ʹ�õ�ģʽ
	EIS_TASK_WAIT_PRIO
};

/*******************************************************************************
����:
name: semaphore name(����ֻ��pSOS��sm_create()��Ҫ��name, ԭ����: char name[4])
initialTokenCount:����ʼ������(���ڵ���0��Ч)this is the initial token count 
		for the semaphore being created. This value will determine the maximum
		number of simultaneous accesses allowed to whatever resource is
		being protected by the semaphore.
taskWaitMode: this parameter determines how tasks will wait on a
		token from an 'empty' semaphore. There are two options for this
		parameter;
		EIS_TASK_WAIT_FIFO - the first task to start pending on the token, will
					receive the token when is made available.�Ƚ���ȴ����е�������
					����ź���
		EIS_TASK_WAIT_PRIO - the highest priority task pending on the token, 
					will receive the token when it is made available.���ȼ��ߵ�����
					�Ȼ���ź���
		�������е�RTOS��֧�������ֵȴ�ģʽ��win32��OS20��Ecos��UCOS��Linux��
		�������ã�PSOS��VxWorks�������á�
����ֵ:
Handle(0 ʧ��)
*******************************************************************************/
UINT32_T ipanel_porting_sem_create(CONST CHAR_T *name, INT32_T initialTokenCount,
                                   UINT32_T taskWaitMode);
                                 
/*******************************************************************************
����:
sem_handle sem_handle(��0,�Ҵ���,��Ч)
����ֵ:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_destroy(UINT32_T sem_handle);

/*******************************************************************************
����:
sem_handle sem_handle(��0,�Ҵ���,��Ч)  
waitMode, specifies how the calling function is to wait on the
	semaphore token in question. There are two options for this
	parameter;
	EIS_QUEUE_SEM_WAIT - the calling function will wait for a period 
		specified in the parameter 'milliSecsToWait'.
	EIS_QUEUE_SEM_NOWAIT - the calling task will not wait for a token if
		it is unavailable. Instead it will return immediately indicating
	that the token was not available.
milliSecsToWait, this parameter only has meaning if 'waitMode' is
	set to 'EIS_QUEUE_SEM_WAIT'. In this case, the calling function will wait
	for a token to be freed, for the specified number of operating
	system timer ticks. If a value of 0 is specified for this 
	parameter then the calling task will block forever, unless a 
	token becomes available.
	���ڶ���������EIS_QUEUE_SEM_WAIT������������Ϊ 0 ��ʾһֱ�ȴ���
����ֵ:
EIS_OS_QUEUE_SEM_STATUS �źŵĵ�ǰ״̬
��EIS_OS_QUEUE_SEM_STATUS_AVAILABLEʱ,��ʾ�ȴ����ź�.
EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_wait(UINT32_T sem_handle, UINT32_T waitMode,
                               INT32_T milliSecsToWait);

/*******************************************************************************
����:
sem_handle sem_handle(��0,�Ҵ���,��Ч)
��sem_handle�ͷ�һ���ź�
����ֵ:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_release(UINT32_T sem_handle); 




#ifdef __cplusplus
}
#endif

#endif // __IPANEL_PORTING_TASK_H______
