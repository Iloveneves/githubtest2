/******************************************************************************
*             Copyright 2009 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: enreach_os.c
* Description: 该文件实现enreach浏览器、oc定义的os接口
*
* History:
* Version   Date              Author       DefectNum      Description
******************************************************************************/

#include "hi_type.h"
#include "hi_base.h"
#include "fyf_def.h"
#include <stdarg.h>

#include "enreach_pub.h"
#include "enreach_erro.h"
#include "ebrowser_api.h"
#include "enreach_oc_drv.h"

#ifndef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct _res_list_t
{
	UINT32 	id;
	struct 	_res_list_t *next;
} res_list_t;


static res_list_t *res_sem_list = NULL;

static res_list_t *res_msgq_list = NULL;

static HI_VOID evm_insert_sem_list(HI_U32 id)
{
	res_list_t *p = NULL;
	p = malloc(sizeof(res_list_t));
	if (p == NULL)
	{
		return;
	}

	p->id = id;
	p->next = res_sem_list;
	res_sem_list = p;
}

static HI_VOID evm_free_sem_list(HI_VOID)
{
	res_list_t *p = NULL;
	while (res_sem_list != NULL)
	{
		HI_SemaphoreDelete(res_sem_list->id);
		EVM_DEBUG("evm_free_sem_list, free sem:0x%x", res_sem_list->id)
		p = res_sem_list;
		res_sem_list = p->next;
		free(p);
	}
}

static HI_VOID evm_insert_msgq_list(HI_U32 id)
{
	res_list_t *p = NULL;
	p = malloc(sizeof(res_list_t));
	if (p == NULL)
	{
		return;
	}

	p->id = id;
	p->next = res_msgq_list;
	res_msgq_list = p;
}

static HI_VOID evm_free_msgq_list(HI_VOID)
{
	res_list_t *p = NULL;
	while (res_msgq_list != NULL)
	{
		HI_OsMsgQueueDel(res_msgq_list->id);
		EVM_DEBUG("evm_free_msgq_list, free msgq:0x%x", res_msgq_list->id)
		p = res_msgq_list;
		res_msgq_list = p->next;
		free(p);
	}
}


HI_VOID HI_FreeOsRes(HI_VOID)
{
	evm_free_msgq_list();
	evm_free_sem_list();
}


#define BUF_SIZE  (8 * 1024)

/***************************** enreach浏览器定义的os接口 ***********************/

/* ---- 浏览器打印输出接口 ---- */
void dbg_printf(const char *fmt, ...)
{
#if 0
    HI_CHAR aszMsgOut[BUF_SIZE];

    va_list argp;   
    va_start(argp, fmt);   
    vsprintf(aszMsgOut,fmt,argp);

    if (vsnprintf(aszMsgOut, BUF_SIZE, fmt, argp) < 0)
    {
        aszMsgOut[BUF_SIZE] = '\0';
    }

    va_end(argp);
    printf("%s", aszMsgOut);
    return;
    
#else
#if 1
	BU08  buf[BUF_SIZE];
	va_list argp;   
	
	va_start(argp, fmt);   
	buf[BUF_SIZE-1] = 0;

	if (vsnprintf(buf, BUF_SIZE, fmt, argp) < 0)
	{
		buf[BUF_SIZE] = '\0';
	}

	va_end(argp);
	printf("\tebrowser: %s", buf);
	return;	
#endif
#endif

}


/* ---- 浏览器定义的线程接口 ---- */
unsigned long EnReach_task_create(char* name, void *proc,
                                  void* args, unsigned long stackSize)
{
	HI_U32 u32Hdl = 0;
	EVM_DEBUG("Enter, name:%s, proc:0x%x, args:0x%x, stack_size:%u", name, proc, args, stackSize);
		
	u32Hdl = HI_TaskCreate(name, proc, args, stackSize, 0);

	EVM_DEBUG("create handle index: %d", u32Hdl);
	
	return u32Hdl;
}

unsigned long EnReach_task_suspend(unsigned long id)
{
//	EVM_DEBUG("Enter, id:0x%x", id);
	return HI_TaskSuspend(id);
}

unsigned long EnReach_task_resume(unsigned long id)
{
//	EVM_DEBUG("Enter, id:0x%x", id);
	return HI_TaskResume(id);
}

void EnReach_task_yield(void)
{
//	EVM_DEBUG("Enter");
    HI_TaskYield();
    return;
}

void EnReach_task_destroy(unsigned long pid)
{

	EVM_DEBUG("Enter, pid:%u", pid);
	HI_TaskDestroy(pid);
	return;
}

void EnReach__destroyself(void)
{
	EVM_DEBUG("Enter");
	HI_TaskDestroySelf();
	return;
}

unsigned long time_getCurrentMSec()
{
//	EVM_DEBUG("Enter");	
	return HI_TaskGetCurMSec();
}

void EnReach_task_sleep(unsigned long ms)
{
//	EVM_DEBUG("Enter, ms:%u", ms);
    HI_TaskSleep(ms);
    return;
}

unsigned long EnReach_task_getid(void)
{
	EVM_DEBUG("Enter");
    return HI_TaskGetId();
}

/* ---- 浏览器信号量管理接口 ---- */

unsigned long EnReach_semaphore_create(char *name)
{   
	HI_S32 s32Ret = 0;
	HI_U32 u32SemaphoreID = 0;

	EVM_DEBUG("Enter, name:%s",name);

	s32Ret = HI_SemaphoreCreate(1, &u32SemaphoreID);

	if (HI_SUCCESS != s32Ret)
	{
		u32SemaphoreID = 0;
	}
	EVM_DEBUG("create ok , semId index:%u", u32SemaphoreID);
	evm_insert_sem_list(u32SemaphoreID);

	return u32SemaphoreID;
}

void EnReach_semaphore_delete(unsigned long id)
{   

	EVM_DEBUG("Enter, id:%u", id);	
#if 0
    (HI_VOID)HI_SemaphoreDelete(id);
#endif
    return;
}

void EnReach_semaphore_lock(unsigned long id)
{
//	EVM_DEBUG("Enter, id:0x%u",id);
    (HI_VOID)HI_SemaphoreWaite(id, OC_INFINITE_WAIT);
    return;
}

void EnReach_semaphore_unlock(unsigned long id)
{
//	EVM_DEBUG("Enter, id:0x%u", id)
    (HI_VOID)HI_SemaphoreSignal(id);

    return;
}





/***************************** enreach oc定义的os接口 ***********************/

/* ---- oc库打印接口 ---- */

OC_OS_STATUS DsmccOcDrvOsDebugPrintf(const char *fmt, ...)
{
#if 0
    HI_CAHR aszMsgOut[BUF_SIZE];

    va_list argp;   
    va_start(argp, fmt);   
    vsprintf(aszMsgOut,fmt,argp);

    if (vsnprintf(aszMsgOut, BUF_SIZE, fmt, argp) < 0)
    {
        aszMsgOut[BUF_SIZE] = '\0';
    }

    va_end(argp);
    printf("%s", aszMsgOut);
#endif

#if 1
	BU08  buf[BUF_SIZE];
	va_list argp;   
	
	va_start(argp, fmt);   
	buf[BUF_SIZE-1] = 0;

	if (vsnprintf(buf, BUF_SIZE, fmt, argp) < 0)
	{
		buf[BUF_SIZE] = '\0';
	}

	va_end(argp);
	printf("\tOC: %s", buf);
	return;	
#endif
	

    return OS_OK;
}

/* ---- oc定义的信号量接口 ---- */

OC_OS_STATUS DsmccOcDrvOsCreateSemaphore(UINT32 ulInitCount, UINT32*  pSemaphoreID)
{
	HI_S32 s32Ret = 0;

	EVM_DEBUG("Enter, init:%u", ulInitCount);
	s32Ret = HI_SemaphoreCreate(ulInitCount, pSemaphoreID);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OS_ERROR);

	EVM_DEBUG("get sem ok, sem index:%u", *pSemaphoreID);

	evm_insert_sem_list(*pSemaphoreID);

	return OS_OK;
}

OC_OS_STATUS DsmccOcDrvOsDeleteSemaphore(UINT32 SemaphoreID)
{
#if 0
	HI_S32 s32Ret = 0;

	EVM_DEBUG("Enter, SemaphoreID:%u", SemaphoreID);
	s32Ret = HI_SemaphoreDelete(SemaphoreID);
	ENREACH_RETURN((HI_SUCCESS != s32Ret), OS_ERROR);
	EVM_DEBUG("delete ok, Sem index:%u", SemaphoreID);
#endif

	return OS_OK;
}

OC_OS_STATUS DsmccOcDrvOsSignalSemaphore(UINT32 ulSemaphoreID)
{
    HI_S32 s32Ret = 0;
//	EVM_DEBUG("Enter, SemaphoreID:%u", ulSemaphoreID);

    s32Ret = HI_SemaphoreSignal(ulSemaphoreID);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), OS_ERROR);


    return OS_OK;
}

OC_OS_STATUS DsmccOcDrvOsWaitSemaphore(UINT32 ulSemaphoreID, UINT32 ulTimeOutInMS)
{
    HI_S32 s32Ret = 0;
//	EVM_DEBUG("Enter, SemaphoreID:%u, timeout:%u", ulSemaphoreID, ulTimeOutInMS);

    s32Ret = HI_SemaphoreWaite(ulSemaphoreID, ulTimeOutInMS);
    ENREACH_RETURN((HI_SUCCESS != s32Ret), OS_ERROR);

    return OS_OK;
}

/* ---- oc库定义的内存管理接口 ---- */

void DsmccOcDrvOsFreeMemory(void *pvMemory)
{
	EVM_DEBUG("Enter, memory:0x%x", pvMemory);

	ENREACH_SAFE_FREE(pvMemory);
	
    	return;    
}

void* DsmccOcDrvOsAllocateMemory(UINT32 ulSizeInByte)
{
#if 0
    HI_U8 *pu8Buf = HI_NULL;

    ENREACH_ERROR("### DsmccOcDrvOsAllocateMemory size = %d ### \n", ulSizeInByte);
    
    pu8Buf = malloc(ulSizeInByte);
    ENREACH_RETURN((HI_NULL == pu8Buf), NULL);
    
    return pu8Buf;
#else
    HI_U8 *pu8Buf = HI_NULL;

    EVM_DEBUG(" Enter ,size = %d ", ulSizeInByte);
    
    pu8Buf = malloc(ulSizeInByte);
    ENREACH_RETURN((HI_NULL == pu8Buf), NULL);
	
    return pu8Buf;	
#endif
}

/* ---- oc库定义的消息队列接口 ---- */

OC_OS_STATUS OcOsCreateQueue(UINT32 MsgCount, UINT32 MsgLength, UINT32* pQueueID)
{
	OC_OS_STATUS status;
	EVM_DEBUG("Enter, MsgCount:%u, MsgLength:%u ", MsgCount, MsgLength);
	status = HI_OsMsgQueueCreate(MsgCount, MsgLength, pQueueID);
	ENREACH_RETURN((status != OS_OK), OS_ERROR);

	evm_insert_msgq_list(*pQueueID);
	
	return status;

}

OC_OS_STATUS OcOsDeleteQueue(UINT32 QueueID)
{
#if 0
	OC_OS_STATUS status;
	EVM_DEBUG("Enter, QueueId:%u", QueueID);
	status =  HI_OsMsgQueueDel(QueueID);
	ENREACH_RETURN((status != OS_OK), OS_ERROR);
	return status;
#endif

	return OS_OK;
}

#if 0 // current evm lib is no use this function 
OC_OS_STATUS  OcOsQueueReceive(UINT32 QueueID, void* pBuf, UINT32 uiBufLen)
{
    OC_OS_STATUS status;

    EVM_DEBUG("Enter, QueueId:%u", QueueID);
    status = HI_OsMsgQueueRec(QueueID, pBuf, uiBufLen, OC_INFINITE_WAIT);

	EVM_DEBUG("Leave");

    return status;
}
#endif

OC_OS_STATUS OcOsQueueReceiveTimeout(UINT32 QueueID, void* pBuf,
                                     UINT32 uiBufLen, UINT32 ulTimeInMS)
{  
	OC_OS_STATUS status;

	EVM_ERROR("QueueID:%u, uiBufLen:%u , ultimsms = 0x%x", QueueID,uiBufLen, ulTimeInMS);
	if (ulTimeInMS == OC_INFINITE_WAIT)
	{
		ulTimeInMS = 1000;

		status = OS_ERROR;
		do
		{
			status = HI_OsMsgQueueRec(QueueID, pBuf, uiBufLen, ulTimeInMS);
		} while (status != OS_OK && HI_TRD_ENREACH_IsExit() != 1);
		
	}
	else
	{
		status = HI_OsMsgQueueRec(QueueID, pBuf, uiBufLen, ulTimeInMS);
	}
	
	ENREACH_RETURN((status != OS_OK), OS_ERROR);
	EVM_DEBUG("recv msg ok");

	return status;
}

OC_OS_STATUS OcOsQueueSend(UINT32 QueueID, void* pMsg, UINT32 uiMsgLen)
{   
	OC_OS_STATUS status;

	EVM_DEBUG("Enter, QueueId:%u, uiMsgLen:%u",QueueID,uiMsgLen);
	status = HI_OsMsgQueueSend(QueueID, pMsg, uiMsgLen);
	ENREACH_RETURN((status != OS_OK), OS_ERROR);

	EVM_DEBUG("Msg send OK");

	return status;
}


/* ---- OC库定义的任务处理接口 ---- */
unsigned long EnReachOC_task_create(char *name, EnReachTaskFun TaskEntry, 
                                    void *args, unsigned long stackSize)
{
	EVM_DEBUG("Enter, name:%s, stackSize:%u", name, stackSize);
    return HI_TaskCreate(name, TaskEntry, args, stackSize, 0);
}

void EnReachOC_task_sleep(unsigned long ms)
{
//	EVM_DEBUG("Enter, ms:%u",ms);
    HI_TaskSleep(ms);
    return;
}

#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

