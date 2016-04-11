/*------------------------------------------------------*/
/*     File     : EnReach_oc_drv.h                                                         */
/*     Date     : 2005/9/5                                                                 */
/*     Purpose :                                                                                 */
/*------------------------------------------------------*/

#ifndef __ENREACH__DRV__H__
#define	__ENREACH__DRV__H__

#include "enreach_oc_type.h"

#define OC_DMUX_INVALID_CHANNEL_ID 0xFFFFFFFF
#define OC_DMUX_INVALID_FILTER_ID 0xFFFFFFFF
#define OC_DMUX_INVALID_PID 0x1FFF
#define OC_INFINITE_WAIT 0xFFFFFFFF
#define OC_IMMEDIATE_RET 0

#define EVM_FILTER_MAX_DEPTH  16

typedef void ( *EnReachTaskFun) (void*); 

typedef enum{
    OC_DEMUX_OK = 0,
    OC_DEMUX_ERROR = 1,
    OC_DEMUX_ERROR_NO_MORE_CHANNEL = 2,
    OC_DEMUX_ERROR_NO_MORE_FILTER = 3,
    OC_DEMUX_ERROR_NO_MORE_BUFFER = 4,
    OC_DEMUX_ERROR_PID = 5	
}OC_DEMUX_STATUS;

typedef enum{
    OC_DRV_DEMUX_ENABLE = 0,
    OC_DRV_DEMUX_DISABLE = 1
}OC_DEMUX_CMD;


/*-----------------------------------------------------------------------------
		OC  return type
------------------------------------------------------------------------------*/
typedef enum
{
    OS_OK 		= 0,
    OS_ERROR 	= 1,
    OS_TIMEOUT 	= 2
}OC_OS_STATUS;

/*----------------------------------------------------------------------------*/

typedef  char (*DsmccOcSectionCallback) 
(
UINT32 ulChannelId, /*channel identifier  */
UINT8 *pBuffer,             /*point to buffer store section data*/ 
UINT32 ulBufferLength       /*length of buffer data*/ 
);

extern OC_DEMUX_STATUS DsmccOcDrvDemuxAllocateSectionChannel
(
    UINT32 *pChannelId,
    INT32 ulMaxFilterNumber, 
    INT32 ulMaxFilterSize, 
    UINT32 ulBufferSize
 );

extern OC_DEMUX_STATUS DsmccOcDrvDemuxFreeSectionChannel
(
UINT32 ulChannelId        /*channel identifier to be free*/
);

extern OC_DEMUX_STATUS DsmccOcDrvDemuxRegisterChannelCallback
(
    UINT32 ulChannelId,       /*channel identifier to be register callback function*/ 
    DsmccOcSectionCallback	CallbackFunction           /*point to function parse section data*/
);

extern OC_DEMUX_STATUS DsmccOcDrvDemuxAllocateFilter ( 
    UINT32 ulChannelId,
    UINT32 *pFilterId,
    UINT32 ulFilterLength
);

extern OC_DEMUX_STATUS DsmccOcDrvDemuxSetFilter
(
    UINT32 ulChannelId,        /*channel identifier be associated with filter*/
    UINT32 ulFilterId,                 /*filter identifier to be set filter condition and mask*/
    UINT8 *pData,                                           /*point to buffer store filter condition*/ 
    UINT8 *pMask,                                           /*point to buffer store equal_mask*/
    UINT32 ulDataLength                                    /*data array length*/
);

extern OC_DEMUX_STATUS DsmccOcDrvDemuxFreeFilter
(
    UINT32 ulChannelId,                    /*channel identifier be associated with filter*/
    UINT32 ulFilterId                          /*filter identifier to be free*/
);

extern OC_DEMUX_STATUS DsmccOcDrvDemuxSetChannelPid
(
    UINT32 ulChannelId, 
    UINT16 usPid
);

extern OC_DEMUX_STATUS DsmccOcDrvDemuxControlChannel
(
    UINT32 ulChannelId,
    OC_DEMUX_CMD emChannelCmd
);

extern OC_OS_STATUS  DsmccOcDrvOsDebugPrintf(const char *fmt, ...);

extern OC_OS_STATUS  DsmccOcDrvOsCreateSemaphore(UINT32 ulInitCount, UINT32*  pSemaphoreID);
extern OC_OS_STATUS  DsmccOcDrvOsDeleteSemaphore(UINT32 SemaphoreID);
extern OC_OS_STATUS  DsmccOcDrvOsSignalSemaphore(UINT32 ulSemaphoreID);
extern OC_OS_STATUS  DsmccOcDrvOsWaitSemaphore(UINT32 ulSemaphoreID, UINT32 ulTimeOutInMS);
extern OC_OS_STATUS OcOsCreateQueue(UINT32 MsgCount, UINT32 MsgLength, UINT32* pQueueID);
extern OC_OS_STATUS OcOsDeleteQueue(UINT32 QueueID);
/*OC_OS_STATUS  OcOsQueueReceive(UINT32 QueueID,void * pBuf , UINT32 uiBufLen);*/
extern OC_OS_STATUS OcOsQueueReceiveTimeout(UINT32 QueueID,void * pBuf , UINT32 uiBufLen , UINT32 ulTimeInMS);
extern OC_OS_STATUS OcOsQueueSend(UINT32 QueueID, void *  pMsg, UINT32 uiMsgLen);
extern unsigned long EnReachOC_task_create(char* name, EnReachTaskFun TaskEntry, void *args, unsigned long stackSize );
extern void EnReachOC_task_sleep( unsigned long ms );
extern OC_OS_STATUS OC_SetTuner(OC_SERVICE_INFO OCSrvInfo);


/*下面删除任务两种方式，根据自己平台选择一种。仅需实现一种。*/
/*任务自杀方式*/
extern void Regist_Task_DestroySelf(void (*pDestroySelf)());
/*别的任务调用函数(*pDestroy)来删除任务(id)*/
extern void Regist_Task_Destroy(void (*pDestroy)(unsigned long id));
#endif  /* __ENREACH__DRV__H__ */




