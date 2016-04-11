#ifndef _FYF_MESSAGE_QUEUE_
#define _FYF_MESSAGE_QUEUE_

typedef sem_t       semaphore_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_t   task_t;
typedef struct
{
    semaphore_t	  * MsgSemaphore_p;
    semaphore_t	  * ClaimSemaphore_p;
    int				Index;
} message_queue_t;


#endif
