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



/* ipanel_porting_sem_wait(), 等待模式下, 如果semaphore不available就一直等的参数(第三个参数). */
/* ipanel_porting_queue_recv(), 等待模式下, 如果无消息就一直等的参数(第三个参数). */
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
	EIS_QUEUE_SEM_NOWAIT	//如果使用了NOWAIT模式,请使用返回值判断是否真正等到.
};
/*
used function:
os_queue_create, os_sem_create
*/
enum
{
	EIS_TASK_WAIT_FIFO,	//推荐使用的模式
	EIS_TASK_WAIT_PRIO
};

/*******************************************************************************
参数:
name: semaphore name(好像只有pSOS的sm_create()需要的name, 原型是: char name[4])
initialTokenCount:最大初始化计数(大于等于0有效)this is the initial token count 
		for the semaphore being created. This value will determine the maximum
		number of simultaneous accesses allowed to whatever resource is
		being protected by the semaphore.
taskWaitMode: this parameter determines how tasks will wait on a
		token from an 'empty' semaphore. There are two options for this
		parameter;
		EIS_TASK_WAIT_FIFO - the first task to start pending on the token, will
					receive the token when is made available.先进入等待队列的任务先
					获得信号量
		EIS_TASK_WAIT_PRIO - the highest priority task pending on the token, 
					will receive the token when it is made available.优先级高的任务
					先获得信号量
		并非所有的RTOS都支持这两种等待模式。win32、OS20、Ecos、UCOS、Linux上
		不能设置，PSOS、VxWorks可以设置。
返回值:
Handle(0 失败)
*******************************************************************************/
UINT32_T ipanel_porting_sem_create(CONST CHAR_T *name, INT32_T initialTokenCount,
                                   UINT32_T taskWaitMode);
                                 
/*******************************************************************************
参数:
sem_handle sem_handle(非0,且存在,有效)
返回值:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_destroy(UINT32_T sem_handle);

/*******************************************************************************
参数:
sem_handle sem_handle(非0,且存在,有效)  
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
	当第二个参数是EIS_QUEUE_SEM_WAIT，第三个参数为 0 表示一直等待。
返回值:
EIS_OS_QUEUE_SEM_STATUS 信号的当前状态
当EIS_OS_QUEUE_SEM_STATUS_AVAILABLE时,表示等待到信号.
EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_wait(UINT32_T sem_handle, UINT32_T waitMode,
                               INT32_T milliSecsToWait);

/*******************************************************************************
参数:
sem_handle sem_handle(非0,且存在,有效)
将sem_handle释放一个信号
返回值:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_release(UINT32_T sem_handle); 




#ifdef __cplusplus
}
#endif

#endif // __IPANEL_PORTING_TASK_H______
