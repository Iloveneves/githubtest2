/**********************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: msg_q.h
 * Description: 
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2006-12-4   fengyoulin                
 ***********************************************************************/
#ifndef __MSG_Q_H
#define __MSG_Q_H

#include <pthread.h>
#include <semaphore.h>

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define TRUE    1
#define FALSE   0

/* message */

typedef struct message_hdr_s
{
    struct message_hdr_s* message_hdr_next;
} message_hdr_t;

typedef struct message_half_s
{
    message_hdr_t*       message_half_head;
    message_hdr_t*       message_half_tail;
    sem_t        message_half_sem;    /* used for sync purpose */
    pthread_mutex_t    message_half_mutex;    /* guard queue ops */
} message_half_t;

typedef struct message_queue_s
{
    message_half_t                      message_queue_free;
    message_half_t                      message_queue_queue;
    HI_VOID*                               message_queue_memory;
} message_queue_t;

typedef message_queue_t HI_MsgQueue_t;  /* for init function */
typedef message_queue_t * HI_MsgQueue_p;  /* message queue handle */

HI_MsgQueue_p HI_OSMsgQueueCreate(size_t MessageSize, unsigned int Messages);

HI_VOID HI_OSMsgQueueDel(HI_MsgQueue_p Queue);

HI_VOID* HI_OSMsgClaim(HI_MsgQueue_p Queue, unsigned int Timeout_MS);

HI_VOID HI_OSMsgSend(HI_MsgQueue_p Queue, HI_VOID *Message);

HI_VOID *HI_OSMsgReceive(HI_MsgQueue_p Queue, unsigned int Timeout_MS);

HI_VOID HI_OSMsgRelease(HI_MsgQueue_p Queue, HI_VOID *Message);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif/*__MSG_Q_H*/

