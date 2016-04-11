
#ifndef __HI_BASE_H__
#define __HI_BASE_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define OC_INFINITE_WAIT   0xFFFFFFFF
#define INVALID_SEM_ID     0xFF
#define MAX_SEM_NUM        25

typedef enum
{
    HI_OS_OK = 0x0,
    HI_OS_ERROR,
    HI_OS_TIMEOUT,
    HI_OS_BUTT
} HI_OS_STATUS;

typedef struct hiENRC_MSG_MESSAGE_S
{
    HI_U32  u32MessageID;
    HI_S32  s32Para1; /*第一个参数*/
    HI_S32  s32Para2; /*第二个参数*/
    HI_S32  s32Para3; /*第三个参数*/
} HI_ENRC_MSG_MESSAGE_S;

#define MAX_THREAD_NUM     10
#define INVALID_THREAD_ID  0

/* 消息队列的接口 */
HI_OS_STATUS HI_OsMsgQueueCreate(HI_U32 u32MsgCount, HI_U32 u32MsgLength, HI_U32* pQueueID);
HI_OS_STATUS HI_OsMsgQueueDel(HI_U32 u32QueueID);
HI_OS_STATUS HI_OsMsgQueueTimeOut(HI_U32 u32QueueID, HI_VOID* pBuf, HI_U32 u32BufLen, HI_U32 u32TimeInMS);
HI_OS_STATUS HI_OsMsgQueueSend(HI_U32 u32QueueID, HI_VOID* pMsg, HI_U32 u32MsgLen);

/* 任务处理接口 */
HI_U32  HI_TaskCreate(HI_CHAR* cName, HI_VOID* proc, HI_VOID* args, HI_U32 u32StackSize, HI_U32 u32Policy);
HI_VOID HI_TaskDestroySelf(HI_VOID);
HI_VOID HI_TaskDestroy(HI_U32 u32Pid);
HI_VOID HI_TaskSleep(HI_U32 u32Time);
HI_U32  HI_TaskSuspend(HI_U32 u32Id);
HI_U32  HI_TaskResume(HI_U32 u32Id);
HI_VOID HI_TaskYield(HI_VOID);
HI_U32  HI_TaskGetId(HI_VOID);
HI_U32  HI_TaskGetCurMSec(HI_VOID);

/* 信号量处理接口 */
HI_S32 HI_SemaphoreCreate(HI_U32 u32InitCount, HI_U32 *pu32SemaphoreID);
HI_S32 HI_SemaphoreDelete(HI_U32 u32SemaphoreID);
HI_S32 HI_SemaphoreSignal(HI_U32 u32SemaphoreID);
HI_S32 HI_SemaphoreWaite(HI_U32 u32SemaphoreID, HI_U32 u32TimeOutInMS);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

