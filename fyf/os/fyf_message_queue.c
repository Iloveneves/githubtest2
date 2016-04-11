/*****************************************************************************
File name   :  messagequeue.c

Description :  实现消息队列的操作

Date               Modification                                          Name
----               ------------                                          ----
07/30/2009         Created                                               
*****************************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>

#include "fyf_message_queue.h"
#include "fyf_def.h"
/*#######################################################################*/
/*########################### DEFINITION ################################*/
/*#######################################################################*/

/* #define MESSAGE_QUEUE_DEBUG */
/* #define MESSAGE_QUEUE_FULL_DEBUG */

#define QUEUE_NB_MAX        50



typedef int BOOL;
typedef unsigned int	U32;

#define MESSAGE_MEMSIZE_QUEUE(size,nb) ((nb)*(size))

/* ---------- Allocation wrapper -------- */
#define wrapper_allocate(size)          malloc(size)//memory_allocate(NULL, size)
#define wrapper_deallocate(ptr)         free(ptr)//memory_deallocate(NULL, ptr)

/* ---------------- Lockers -------------- */
#define LOCKER_INIT         PTHREAD_MUTEX_INITIALIZER
typedef pthread_mutex_t     locker_t;

#define locker_lock(x)      pthread_mutex_lock(&x)
#define locker_unlock(x)    pthread_mutex_unlock(&x)


#define FALSE 0
#define TRUE 1

/* ----------- Semaphore Lists ---------- */
typedef struct SemaphoreList_s
{
    struct SemaphoreList_s    * Next_p;
    /* the same structure will be used for semaphore and mutex */
    void                      * Semaphore_p;
} SemaphoreList_t;




typedef struct MessageSend_s
{
    struct MessageSend_s  *	Next_p;
    void     			  *	Message_p;
} MessageSend_t;

typedef struct
{
	void 			  * Memory_p;
	BOOL 			  * Used_p;
	MessageSend_t	  *	Pending_p;
	size_t 				ElementSize;
	unsigned int 		NoElements;
} MessageElem_t;

typedef struct MessageQueueList_s
{
    struct MessageQueueList_s   * Next_p;
    message_queue_t             * MessageQueue_p;
    void                        * Memory_p;
} MessageQueueList_t;

static MessageElem_t    MessageArray[QUEUE_NB_MAX];
static BOOL   			MessageInitDone = FALSE;

static MessageQueueList_t    * MessageQueueList_p = NULL;

static locker_t message_mutex = LOCKER_INIT;


/* Traces */
#if defined(MESSAGE_QUEUE_DEBUG) || defined(MESSAGE_QUEUE_FULL_DEBUG)
#define PrintMessageQ_Debug(x)		printf x
#else
#define PrintMessageQ_Debug(x)
#endif

#ifdef MESSAGE_QUEUE_FULL_DEBUG
#define PrintMessageQ_FullDebug(x)  printf x
#else
#define PrintMessageQ_FullDebug(x)
#endif

/*
* Init_Allqueues
*/

static void Init_Allqueues(void)
{
    MessageElem_t * Elem_p;
    int				Index;

	if (! MessageInitDone)
	{
		MessageInitDone = TRUE;
		for (Index=0 , Elem_p = MessageArray ; Index<QUEUE_NB_MAX ; Index++ , Elem_p++)
        {
			Elem_p->Memory_p = NULL;
			Elem_p->Used_p = NULL;
			Elem_p->Pending_p = NULL;
    		Elem_p->ElementSize = 0;
    		Elem_p->NoElements = 0;
		}

		PrintMessageQ_Debug(("COMPAT: message initialization done !!!\n"));
    }
}


static int semaphore_wait_timeout(semaphore_t *sem, int wait_time)
{
	int ret;
	struct timespec abstime; 
	switch(wait_time)
	{
	case FYF_NO_WAIT:
		while (((ret = sem_trywait(sem)) != 0 ) && (errno == EINTR));    
		//ret =  sem_trywait(&sem);
		break;
	case FYF_WAIT_FOREVER:
		//ret = sem_wait(&sem);
		while (((ret = sem_wait(sem)) != 0) && (errno == EINTR));  
		break;
	default:            
        clock_gettime(CLOCK_REALTIME, &abstime);    //获取当前时间
        abstime.tv_sec += (wait_time / 1000);        //加上等待时间的秒数
        abstime.tv_nsec += (wait_time % 1000) * 1000; //加上等待时间纳秒数 
        while (((ret = sem_timedwait(sem, &abstime)) != 0) && (errno == EINTR)); 
		break;
	}

	return ret;
}

void message_init_queue(message_queue_t * MessageQueue,
                        void * Memory_p,
						size_t ElementSize,
                        unsigned int NoElements)
{
    MessageElem_t * Elem_p;
    int				Index;
	printf("message_init_queue 111\n");

    locker_lock(message_mutex);
    Init_Allqueues();
	printf("message_init_queue 222\n");
    /* Seeking for unused queue */
	Index = 0;
    Elem_p = MessageArray;
	while ((Index<QUEUE_NB_MAX) && (Elem_p->Memory_p != NULL))
	{
		Index++;
		Elem_p++;
	}
	printf("message_init_queue 333\n");
    MessageQueue->Index = Index;

    if (Index < QUEUE_NB_MAX)   /* if valid unused queue has been found */
	{
	    Elem_p = &MessageArray[ MessageQueue->Index ];

    	Elem_p->Used_p = wrapper_allocate(NoElements*sizeof(BOOL));    /* Allocates used/unused element array */
			printf("message_init_queue 444\n");
        if (Elem_p->Used_p != NULL)
		{
            memset((void *)Elem_p->Used_p, 0, NoElements*sizeof(BOOL));     /* Sets all elements to unused */

    		Elem_p->Memory_p = Memory_p;
			Elem_p->Pending_p = NULL;
    		Elem_p->ElementSize = ElementSize;
    		Elem_p->NoElements = NoElements;
				printf("message_init_queue 555\n");
			sem_init(MessageQueue->MsgSemaphore_p, NULL, 0);
			sem_init(MessageQueue->ClaimSemaphore_p, NULL, 0);
				printf("message_init_queue 666\n");
           // MessageQueue->MsgSemaphore_p = semaphore_create_fifo(0);
           // MessageQueue->ClaimSemaphore_p = semaphore_create_fifo(0);

			PrintMessageQ_Debug(("COMPAT: Message_queue initialized: %d\n", MessageQueue->Index));
		}
		else
		{
            printf("%s(): No memory !!!\n", __FUNCTION__);
		}
	}
	else
	{
        printf("%s(): No memory !!!\n", __FUNCTION__);
	}
    locker_unlock(message_mutex);
}

/*
 * message_delete_queue
 */
void message_delete_queue(message_queue_t * MessageQueue)
{
    MessageQueueList_t  * Current_p = NULL; /* to avoid warning */
    MessageQueueList_t  * Deleted_p = NULL;
	MessageSend_t       * Pending_p;
    MessageElem_t       * Elem_p;

	if (! MessageInitDone)
	{
       	printf("%s() not initialized !!!\n", __FUNCTION__);
		return;
	}

    if (MessageQueue->Index < QUEUE_NB_MAX)
	{
        Elem_p = &MessageArray[ MessageQueue->Index ];

        /* Delete associated semaphores */
        sem_destroy(MessageQueue->MsgSemaphore_p);
        sem_destroy(MessageQueue->ClaimSemaphore_p);

        locker_lock(message_mutex);

	    Elem_p->Memory_p = NULL;
		Pending_p = Elem_p->Pending_p;
		while (Pending_p != NULL)
		{
			Elem_p->Pending_p = Elem_p->Pending_p->Next_p;
	    	wrapper_deallocate(Pending_p);
			Pending_p = Elem_p->Pending_p;
		}
    	Elem_p->ElementSize = 0;
    	Elem_p->NoElements = 0;
    	if (Elem_p->Used_p != NULL)
    	{
    	    wrapper_deallocate(Elem_p->Used_p);
    	    Elem_p->Used_p = NULL;
        }

        locker_unlock(message_mutex);
	}
	MessageQueue->Index = QUEUE_NB_MAX;

    /* Frees the message queue in the list */
    locker_lock(message_mutex);
    if (MessageQueueList_p != NULL)
    {
        if (MessageQueueList_p->MessageQueue_p == MessageQueue)
        {
            Deleted_p = MessageQueueList_p;
        }
        else
        {
            Current_p = MessageQueueList_p;
            while (   (Current_p->Next_p != NULL)
                   && (Deleted_p == NULL))
            {
                if (Current_p->Next_p->MessageQueue_p == MessageQueue)
                {
                    /* Element to destroy has been found */
                    Deleted_p = Current_p->Next_p;
                }
                else
                {
                    /* Goes to next element */
                    Current_p = Current_p->Next_p;
                }
            }
        }

        if (Deleted_p != NULL)
        {
            if (Deleted_p == MessageQueueList_p)
            {
                MessageQueueList_p = Deleted_p->Next_p;
            }
            else
            {
                Current_p->Next_p = Deleted_p->Next_p;
            }

            wrapper_deallocate(Deleted_p->Memory_p);
            wrapper_deallocate(Deleted_p->MessageQueue_p);
            wrapper_deallocate(Deleted_p);
        }
        else
        {
            printf("%s() problem (0x%x), not found !!!\n", __FUNCTION__, (U32)MessageQueue);
        }
    }
    locker_unlock(message_mutex);

    PrintMessageQ_Debug(("COMPAT: message_delete_queue done !!!\n"));
}

/*
 * message_create_queue_timeout
 */
message_queue_t * message_create_queue(size_t ElementSize, unsigned int NoElements)
{
    MessageQueueList_t  * Current_p;
    MessageQueueList_t  * New_p;
    message_queue_t     * MesQ_p = NULL;


    if ((New_p = wrapper_allocate(sizeof(MessageQueueList_t))) != NULL)
    {
        if ((New_p->MessageQueue_p = wrapper_allocate(sizeof(message_queue_t))) != NULL)
        {
            if ((New_p->Memory_p = wrapper_allocate(MESSAGE_MEMSIZE_QUEUE(ElementSize, NoElements))) == NULL)
            {
                /* Memory allocation pb, deallocate list element */
                wrapper_deallocate(New_p->MessageQueue_p);
                wrapper_deallocate(New_p);
                New_p = NULL;
            }
        }
        else
        {
            /* Memory allocation pb, deallocate list element */
            wrapper_deallocate(New_p);
            New_p = NULL;
        }
    }
	printf("\n\nmessage_create_queue 111\n");
    if (New_p != NULL)
    {
        message_init_queue( New_p->MessageQueue_p, New_p->Memory_p,
                                    ElementSize, NoElements);
		printf("\n\nmessage_create_queue 222\n");
        if (MessageArray[ New_p->MessageQueue_p->Index ].Used_p != NULL)
        {
            /* At this stage, we know that all allocations have been correctly done */
            New_p->Next_p = NULL;

            locker_lock(message_mutex);
            Current_p = MessageQueueList_p;
            if (Current_p != NULL)
            {
                while (Current_p->Next_p != NULL)
                    Current_p = Current_p->Next_p;

                Current_p->Next_p = New_p;
            }
            else
            {
                MessageQueueList_p = New_p;
            }
            locker_unlock(message_mutex);

            MesQ_p = New_p->MessageQueue_p;

	        PrintMessageQ_Debug(("COMPAT: message_create_queue_timeout done (0x%x)!!!\n", MesQ_p));
        }
        else
        {
            message_delete_queue(New_p->MessageQueue_p);

            wrapper_deallocate(New_p->MessageQueue_p);
            wrapper_deallocate(New_p->Memory_p);
            wrapper_deallocate(New_p);

            printf("%s(): message queue init problem !!!\n", __FUNCTION__);
        }
    }
    else
    {
        printf("%s(): no memory !!!\n", __FUNCTION__);
    }

    /* NULL if Message Queue creation problem, message queue pointer otherwise */
    return(MesQ_p);
}


#if 0
/*
 * message_claim_timeout
 */
int message_claim_timeout(message_queue_t * MessageQueue, clock_t * time_end_p)
{
    MessageElem_t * Elem_p;
    int				i;
    BOOL            SemWait = FALSE;

	if (! MessageInitDone)
	{
        printf("%s() not initialized !!!\n", __FUNCTION__);
		return((int)NULL);
	}

    if (MessageQueue->Index < QUEUE_NB_MAX)
	{
		PrintMessageQ_Debug(("COMPAT: Claiming (%d)\n", MessageQueue->Index));

        Elem_p = &MessageArray[ MessageQueue->Index ];

		do
		{
		    if (SemWait)
		    {
                PrintMessageQ_FullDebug(("COMPAT: Semaphore unlocked (%d)\n", MessageQueue->Index));
		    }

            /* Scan every element to find first free one */
            locker_lock(message_mutex);
			for (i=0 ; i<Elem_p->NoElements ; i++)
			{
				if (! Elem_p->Used_p[i])
				{
		            PrintMessageQ_Debug(("COMPAT: Claiming done, elem:%d\n", i));

					Elem_p->Used_p[i] = TRUE;

                    locker_unlock(message_mutex);
					return( (int)(Elem_p->Memory_p) + (int)(i * Elem_p->ElementSize) );
				}
			}
            locker_unlock(message_mutex);

           /* No free message has been found, waiting for message release */
            SemWait = TRUE;
            PrintMessageQ_FullDebug(("COMPAT: message_claim, No free message, waiting ...\n"));
		}
		while (semaphore_wait_timeout(MessageQueue->ClaimSemaphore_p, time_end_p) == 0);

		PrintMessageQ_FullDebug(("COMPAT: Claiming timeout\n"));
	}

	return((int)NULL);
}

#endif
/*
 * message_claim
 */
int message_claim(message_queue_t * MessageQueue)
{
	MessageElem_t * Elem_p;
    int				i;
    BOOL            SemWait = FALSE;

	if (! MessageInitDone)
	{
        printf("%s() not initialized !!!\n", __FUNCTION__);
		return((int)NULL);
	}

    if (MessageQueue->Index < QUEUE_NB_MAX)
	{
		PrintMessageQ_Debug(("COMPAT: Claiming (%d)\n", MessageQueue->Index));

        Elem_p = &MessageArray[ MessageQueue->Index ];

		do
		{
		    if (SemWait)
		    {
                PrintMessageQ_FullDebug(("COMPAT: Semaphore unlocked (%d)\n", MessageQueue->Index));
		    }

            /* Scan every element to find first free one */
            locker_lock(message_mutex);
			for (i=0 ; i<Elem_p->NoElements ; i++)
			{
				if (! Elem_p->Used_p[i])
				{
		            PrintMessageQ_Debug(("COMPAT: Claiming done, elem:%d\n", i));

					Elem_p->Used_p[i] = TRUE;

                    locker_unlock(message_mutex);
					return( (int)(Elem_p->Memory_p) + (int)(i * Elem_p->ElementSize) );
				}
			}
            locker_unlock(message_mutex);

           /* No free message has been found, waiting for message release */
            SemWait = TRUE;
            PrintMessageQ_FullDebug(("COMPAT: message_claim, No free message, waiting ...\n"));
		}
		while (semaphore_wait_timeout(MessageQueue->ClaimSemaphore_p, FYF_WAIT_FOREVER) == 0);

		PrintMessageQ_FullDebug(("COMPAT: Claiming timeout\n"));
	}

	return((int)NULL);
}

/*
 * message_release
 */
void message_release (message_queue_t* MessageQueue, void* Message)
{
    MessageElem_t * Elem_p;
	U32				Index;

	if (! MessageInitDone)
	{
        printf("%s(): not initialized !!!\n", __FUNCTION__);
		return;
	}

    if (MessageQueue->Index < QUEUE_NB_MAX)
	{
		PrintMessageQ_Debug(("COMPAT: Releasing (%d)\n", MessageQueue->Index));

        Elem_p = &MessageArray[ MessageQueue->Index ];

        locker_lock(message_mutex);
        if (   ((U32)Message >= (U32)(Elem_p->Memory_p))
		    && ((U32)Message < (U32)(Elem_p->Memory_p) + (Elem_p->ElementSize*Elem_p->NoElements)) )
		{
			Index = ((U32)Message - (U32)(Elem_p->Memory_p))/((U32)(Elem_p->ElementSize));
			Elem_p->Used_p[Index] = FALSE;

			PrintMessageQ_Debug(("COMPAT: %d released\n", Index));

            sem_post(MessageQueue->ClaimSemaphore_p);
		}
		else
		{
            printf(( "%s() pb !!!\n", __FUNCTION__));
		}
        locker_unlock(message_mutex);
	}
}


/*
 * message_send
 */
int message_send(message_queue_t * MessageQueue, void * message)
{
	MessageSend_t  * Pending_p;
	MessageSend_t  * New_p;

	if (! MessageInitDone)
	{
        printf("%s(): not initialized !!!\n", __FUNCTION__);
		return 0;
	}

    if (MessageQueue->Index < QUEUE_NB_MAX)
	{
		PrintMessageQ_Debug(("COMPAT: Sending (%d)\n", MessageQueue->Index));

        New_p = wrapper_allocate(sizeof(MessageSend_t));

        if (New_p != NULL)
		{
			New_p->Next_p = NULL;
			New_p->Message_p = message;

	    	PrintMessageQ_Debug(("COMPAT: Sending (0x%x)\n", New_p->Message_p));

            locker_lock(message_mutex);
			Pending_p = MessageArray[ MessageQueue->Index ].Pending_p;
	        if (Pending_p != NULL)
			{
				while (Pending_p->Next_p != NULL)
					Pending_p = Pending_p->Next_p;

				Pending_p->Next_p = New_p;
			}
			else
			{
				MessageArray[ MessageQueue->Index ].Pending_p = New_p;
			}
            locker_unlock(message_mutex);

	    	sem_post(MessageQueue->MsgSemaphore_p);
		}
		else
		{
            printf("%s(): no memory !!!\n", __FUNCTION__);
		}
    }

	return 0;
}

#if 1
/*
 * message_receive_timeout
 */
void * message_receive_timeout (message_queue_t* MessageQueue, int waittime)
{
	MessageSend_t  * Pending_p;
    MessageElem_t  * Elem_p;
    void 		   * Message_p = NULL;
    BOOL             SemWait = FALSE;

	if (! MessageInitDone)
	{
        printf( "%s(): not initialized !!!\n", __FUNCTION__);
		return (Message_p);
	}

    if (MessageQueue->Index < QUEUE_NB_MAX)
	{
		PrintMessageQ_FullDebug(("COMPAT: Receiving (%d)\n", MessageQueue->Index));

#ifdef MESSAGE_QUEUE_DEBUG
		Elem_p = &MessageArray[ MessageQueue->Index ];

        locker_lock(message_mutex);
		Pending_p = Elem_p->Pending_p;
		if (Pending_p != NULL)
		{
			PrintMessageQ_Debug(("COMPAT: Queue %d: ", MessageQueue->Index));
			while (Pending_p != NULL)
			{
			   PrintMessageQ_Debug(("-> 0x%x ", Pending_p->Message_p));
			   Pending_p = Pending_p->Next_p;
			}
			PrintMessageQ_Debug(("-> NULL\n"));
		}
        locker_unlock(message_mutex);
#endif

	    do
		{
		    if (SemWait)
		    {
                PrintMessageQ_FullDebug(("COMPAT: Semaphore unlocked (%d)\n", MessageQueue->Index));
		    }

	        Elem_p = &MessageArray[ MessageQueue->Index ];

            locker_lock(message_mutex);
			Pending_p = Elem_p->Pending_p;
			if (Pending_p != NULL)
			{
				Elem_p->Pending_p = Pending_p->Next_p;
				Message_p = Pending_p->Message_p;
    			wrapper_deallocate(Pending_p);

				PrintMessageQ_Debug(("COMPAT: Queue %d: Received 0x%x \n", MessageQueue->Index, Message_p));

                locker_unlock(message_mutex);
                return (Message_p);
			}
            locker_unlock(message_mutex);

            SemWait = TRUE;
            PrintMessageQ_FullDebug(("COMPAT: message_receive (%d), No message available, waiting ...\n", MessageQueue->Index));
		}
		while (semaphore_wait_timeout(MessageQueue->MsgSemaphore_p, waittime) == 0);

        PrintMessageQ_FullDebug(("COMPAT: Receive timeout (%d) \n", MessageQueue->Index));
    }

	return (Message_p);
}
#endif
/*
 * message_receive
 */
void * message_receive (message_queue_t* MessageQueue)
{
	if (! MessageInitDone)
	{
        printf("%s(): not initialized !!!\n", __FUNCTION__);
		return ((void *)NULL);
	}

    return( message_receive_timeout(MessageQueue, FYF_WAIT_FOREVER) );
}

/* End of posixmessagequeue.c */


