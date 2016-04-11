/*---------------------------------------------------------------------------------
   File Name	: irca_os.c

   Description	: os driver interferce to irdeto s3_dvb_client_3.6.8.lib
                  Create by PYH.
                  Please refer at See s3_osdrv.h,
                  and Irdeto Access document 710467, "Operating System Driver API".

   Version		: REL_V1.0.0

   Copyright (c) 2008-2100 Triumph Corporation.  All rights reserved.

   History      : 1.2008/07/01 PYH - Create file and 
                    add os driver interferc.
----------------------------------------------------------------------------------*/

/* Includes ---------------------------------------------------------------- */
#include <basic_types.h>
#include <api/libc/string.h>
#include <os/tds2/alitypes.h>
#include <irca/pubh/s3_osdrv.h>
#include "irca_debug.h"
#include "irca_drv.h"
#include <osal/osal_task.h>


/* External global variables ---------------------------------------------- */
//extern	ST_Partition_t  *ca_partition ;

static IRCA_TASK_STRUCT CaTask[IRCA_MAX_TASK_NUM];

/* Global variables ------------------------------------------------------- */

/* External global function ----------------------------------------------- */

/* Private Function ------------------------------------------------------- */

/* Global Function -------------------------------------------------------- */


void IRCA_OS_Initialise(void)
{
	int i;
	
	for (i=0;i<IRCA_MAX_TASK_NUM;i++)
	{
		CaTask[i].pstTaskID = OSAL_INVALID_ID;
		CaTask[i].pstMessageQueue = OSAL_INVALID_ID;
		CaTask[i].MessageLength = 0;
	}
	//IrCa_Debug_Os(("IRCA_OS_Initialise success!\n"));
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_AllocateMemory()

   Description:
   This API is called to allocate memory.
   The OS_DRV_AllocateMemory() must be used for any data that is sent to SoftCell 3. 
   SoftCell 3 will always use OS_DRV_FreeMemory() to free a block of memory.

   Parameters:
   uiSize:		Size of the memory block to be allocated.

   Return Value:
   Pointer to the successfully allocated memory, otherwise zero if no memory
   could be allocated (out of memory) or if uiSize is 0.
------------------------------------------------------------------------------------ */
void *OS_DRV_AllocateMemory( ia_uint32 uiSize )
{
	void* ptr = NULL;

	/*IrCa_Debug_Os(("OS_DRV_AllocateMemory [uiSize = %d]\n",uiSize));*/
	
	if (uiSize <= 0)
	{
		IrCa_Debug_Err(("Invalid param [uiSize = %d] in OS_DRV_AllocateMemory\n",uiSize));
		return NULL;
	}

	ptr= (void*)MALLOC( uiSize );

	if (ptr != NULL )
	{
		MEMSET( ptr, 0, uiSize );
	}
	else
	{
		IrCa_Debug_Err(("No enough mem for OS_DRV_AllocateMemory [uiSize = %d]\n",uiSize));
	}


	return ptr;
	
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_FreeMemory()

   Description:
   This API is called to release previously allocated memory. 
   A release of a 0 (NULL) pointer must be safe.

   Parameters:
   pvMemory:	Pointer to the memory previously allocated.

   Return Value:
   None.
------------------------------------------------------------------------------------ */
void OS_DRV_FreeMemory( void *pvMemory )
{
	/*IrCa_Debug_Os(("OS_DRV_FreeMemory [pvMemory = 0x%x]\n",pvMemory));*/

	if( pvMemory != NULL )
	{
		FREE(pvMemory);

		pvMemory = NULL;
	}
	else
	{
		IrCa_Debug_Err(("Invalid param [pvMemory = 0x%x] in OS_DRV_FreeMemory\n",pvMemory));
	}
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_CreateTask()

   Description:
   This API is called to create a task. 
   SoftCell 3 will use global memory to share variables between tasks.

   Parameters:
   pfnNewTask:	Pointer to an executable function that must be started 
                as/from the new thread.
   pvTaskData:	Pointer to be passed to the newly created task via the
                pfnNewTask function.
   bPriority:	Priority of the task (0 – 15). Priority 0 is the lowest 
                priority and priority 15 is the highest priority. 
                These priorities must be mapped to appropriate priorities 
                on the host operating system. SoftCell 3 may start several 
                tasks with the same priority.
   wStackSize:	Minimum stack size required for this task. This is an indication 
                of the approximate amount of stack needed by SoftCell 3.
   wQueueDepth:	The minimum number of messages this task must be able to queue 
                at the same time. If wQueueDepth > 0 then a mailbox must be 
                created as destination mailbox for this task and on return of 
                OS_DRV_CreateTask this task shall be able to receive messages. 
                If wQueueDepth is 0 then no mailbox is needed.

   Return Value:
   When successful a handle to the newly created task, otherwise
   OS_DRV_TASK_ERROR, for instance when a new task or mailbox could not be created.
------------------------------------------------------------------------------------ */
os_drv_task_h OS_DRV_CreateTask( os_drv_task_fn tNewTask, void *pvTaskData,
	ia_byte bPriority, ia_word32 wStackSize, ia_word16 wQueueDepth )
{
	int i;
	T_CMBF t_cmbf;
	OSAL_T_CTSK t_ctsk;
	ID      taskID = OSAL_INVALID_ID;
	
	for (i=0;i<IRCA_MAX_TASK_NUM;i++)
	{
		if (CaTask[i].pstTaskID == OSAL_INVALID_ID)
			break;
	}
	if (i >= IRCA_MAX_TASK_NUM)
	{
		IrCa_Debug_Err(("Need more task in OS_DRV_CreateTask\n"));
		return OS_DRV_TASK_ERROR;
	}

	/*应该先创建消息再创建进程，因为进程会利用消息*/
	if (wQueueDepth != 0)
	{
	    t_cmbf.bufsz =  (wQueueDepth*(4 + (sizeof(IRCA_MESSAGE_STRUCT) +3)&(~3))); 
	    t_cmbf.maxmsz = sizeof(IRCA_MESSAGE_STRUCT);

		if((CaTask[i].pstMessageQueue = osal_msgqueue_create(&t_cmbf))==OSAL_INVALID_ID)
		{
			IrCa_Debug_Err(("Can't creat message in OS_DRV_CreateTask\n"));
			return OS_DRV_TASK_ERROR;
		}
	}

	t_ctsk.stksz	= 	wStackSize;
	t_ctsk.quantum  =   IRCA_TASK_QUANTUM;
	t_ctsk.itskpri	= 	IRCA_TASK_PRIORITY;
	t_ctsk.para1 =(UINT32)pvTaskData;
	t_ctsk.name[0]	=  'C';
	t_ctsk.name[1]	=  'A';
	t_ctsk.name[2]	=  i+'0';
	t_ctsk.task = ( FP ) tNewTask;

	osal_task_dispatch_off();

	taskID = osal_task_create(&t_ctsk);

 	if (taskID == OSAL_INVALID_ID) 
 	{
		IrCa_Debug_Err(("Can't creat task in OS_DRV_CreateTask\n"));
		return OS_DRV_TASK_ERROR;
	}
	CaTask[i].pstTaskID = taskID ;

	/*IrCa_Debug_Os(("OS_DRV_CreateTask [tNewTask = 0x%x] [pvTaskData = 0x%x] [bPriority = %d] [wStackSize = %d] [wQueueDepth = %d][taskid=%d]\n",
		tNewTask,pvTaskData,bPriority,wStackSize,wQueueDepth ,i));*/

	osal_task_dispatch_on();
	
	return (os_drv_task_h)(i);
	
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_DelayTask()

   Description:
   This API is called to suspend a task for at least the period specified. 
   The minimum granularity of the timer should be 10ms.

   Parameters:
   wDelay:	Time in milliseconds to suspend the task. If the requested
            delay time does not correspond with a boundary of the
            granularity, the delay time needs to be extended to the
            next available boundary of the granularity.
            
   Return Value:
   None.
------------------------------------------------------------------------------------ */
void OS_DRV_DelayTask( ia_word16 wDelay )
{
	int DelayTime = 10; /*irdeto规定delay基本单位是10ms*/

//	DBG_DRV_Printf("OS_DRV_DelayTask =%d\n" , wDelay );


	if (wDelay%10 == 0)
	{
		DelayTime = wDelay;
	}
	else
	{
		DelayTime = (wDelay/10)*10 + 10;
	}

	osal_task_sleep(DelayTime);
	
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_SendMessage()

   Description:
   This API is called to send an inter-task message. 
   The status returned reflects if the function was successful. 
   OS_DRV_SendMessage must be non-blocking.

   Parameters:
   hTask:			Destination task of the message.
   wMessageLength:	Length of the data pointed to by pvMsg.
   pvMsg:			Pointer to message. The receiving task must free this pointer.
            
   Return Value:
   IA_SUCCESS:				Function successful.
   IA_FULL:					Message not sent. Destination task's mailbox is full.
   IA_INVALID_PARAMETER:	Message not sent. Destination task has no mailbox.
   IA_INVALID_OS_HANDLE:	Message not sent. Handle is not a valid task handle.
   IA_FAIL:					Any other error.
------------------------------------------------------------------------------------ */
ia_result OS_DRV_SendMessage( os_drv_task_h hTask, 
					ia_word16 wMessageLength, void *pvMsg )
{
	IRCA_MESSAGE_STRUCT  msg_p ;
	
	IrCa_Debug_Os(("OS_DRV_SendMessage [hTask = %d] [wMessageLength = %d] [pvMsg = 0x%x]\n",
		hTask,wMessageLength,pvMsg));/**/

	if (hTask >= IRCA_MAX_TASK_NUM )
	{
		IrCa_Debug_Err(("Invalid param in OS_DRV_SendMessage [hTask=0x%x]\n",hTask));
		return IA_INVALID_OS_HANDLE;
	}

	if (CaTask[hTask].pstMessageQueue == OSAL_INVALID_ID)
	{
		IrCa_Debug_Err(("Destination task has no mailbox\n"));
		return IA_INVALID_PARAMETER;
	}

	msg_p.MsgData = pvMsg;
	msg_p.MsgLen = wMessageLength;
	
	if(osal_msgqueue_send(CaTask[hTask].pstMessageQueue, &msg_p, sizeof(IRCA_MESSAGE_STRUCT),20) == E_OK)
	{
		return	IA_SUCCESS;
	}
	else
	{
		return	IA_FULL;
	}

}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_ReceiveMessage()

   Description:
   This API is called to wait (block) until a message becomes available 
   for the task and to return that message.

   Parameters:
   pwMessageLength:		Length of the message returned.

   Return Value:
   Pointer to a message, otherwise zero if the task does not have a 
   mailbox or on other errors.
------------------------------------------------------------------------------------ */
void *OS_DRV_ReceiveMessage( ia_word16 *pwMessageLength )
{
	ID TaskId;
	int i;
	void   *result;
	IRCA_MESSAGE_STRUCT Msg;
	UINT32  Len;

    TaskId = osal_task_get_current_id();

	IrCa_Debug_Os(("OS_DRV_ReceiveMessage TaskId =%d\n", TaskId));/**/

	for (i=0;i<IRCA_MAX_TASK_NUM;i++)
	{
		if (CaTask[i].pstTaskID == TaskId)
			break;
	}
	if (i >= IRCA_MAX_TASK_NUM)
	{
		IrCa_Debug_Err(("Curren task is not created by irdeto\n"));
		*pwMessageLength = 0;
		return 0;
	}

	if (CaTask[i].pstMessageQueue == OSAL_INVALID_ID || pwMessageLength == NULL)
	{
		IrCa_Debug_Err(("Curren task has no message or param err\n"));
		*pwMessageLength = 0;
		return 0;
	}

	if(osal_msgqueue_receive((VP)&Msg, &Len, CaTask[i].pstMessageQueue,OSAL_WAIT_FOREVER_TIME)==E_OK)
	{
		result = Msg.MsgData ;
		*pwMessageLength = Msg.MsgLen ;
		return (void*)result;
	}
	else
	{
		*pwMessageLength = 0;
		return 0;
	}

}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_CreateSemaphore()

   Description:
   This API is called to create a semaphore. 
   The call must return OS_DRV_SEMAPHORE_ERROR if the semaphore 
   cannot be created (e.g. out of resources).

   Parameters:
   uiInitialValue:	Initial count for the semaphore.

   Return Value:
   Semaphore handle when successful, otherwise OS_DRV_SEMAPHORE_ERROR.
------------------------------------------------------------------------------------ */
os_drv_semaphore_h OS_DRV_CreateSemaphore( ia_uint32 ulInitialValue )
{
	OSAL_ID SemaphoreId = OSAL_INVALID_ID;
	UINT32 i ;

	SemaphoreId = osal_semaphore_create(0);
	if (SemaphoreId == OSAL_INVALID_ID)
	{
		IrCa_Debug_Err(("Can't creat Semaphore\n"));
		return OS_DRV_SEMAPHORE_ERROR;
	}
	if (ulInitialValue >0 )
	{
		for (i=0 ;i<ulInitialValue ; i++)
		{
			osal_semaphore_release(SemaphoreId);
		}
	}
	return ((os_drv_semaphore_h)SemaphoreId);
	
	
/*
	if(ulInitialValue==0)
	{
		SemaphoreId = osal_semaphore_create(1);
		if (SemaphoreId != OSAL_INVALID_ID)
		{
			osal_semaphore_capture( SemaphoreId, OSAL_WAIT_FOREVER_TIME );
		}
	}
	else
	{
		SemaphoreId = osal_semaphore_create(ulInitialValue);
	}
	if (SemaphoreId != OSAL_INVALID_ID)
	{
		IrCa_Debug_Os(("OS_DRV_CreateSemaphore [ulInitialValue = %d SemaphoreId =%d] \n",ulInitialValue , SemaphoreId));
		
		return ((os_drv_semaphore_h)SemaphoreId);
	}
	else
	{
		IrCa_Debug_Err(("Can't creat Semaphore\n"));
		return OS_DRV_SEMAPHORE_ERROR;
	}
	*/
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_DeleteSemaphore()

   Description:
   This API is called to delete (free) the semaphore.

   Parameters:
   hSemaphore:	Semaphore to be deleted.

   Return Value:
   IA_SUCCESS:				If the semaphore is successfully deleted.
   IA_INVALID_OS_HANDLE:	If the given handle is not valid.
   IA_FAIL:					Any other error.
------------------------------------------------------------------------------------ */
ia_result OS_DRV_DeleteSemaphore( os_drv_semaphore_h hSemaphore )
{
	IrCa_Debug_Os(("OS_DRV_DeleteSemaphore [hSemaphore = 0x%d] \n",hSemaphore));

	if (hSemaphore == OSAL_INVALID_ID)
	{
		IrCa_Debug_Err(("Invalis param in OS_DRV_DeleteSemaphore\n"));
		return IA_INVALID_OS_HANDLE;
	}
	else
	{
		osal_semaphore_delete(hSemaphore);
		return IA_SUCCESS;
	}
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_SignalSemaphore()

   Description:
   This API is called to signal the semaphore (the counter is incremented by one).

   Parameters:
   hSemaphore:	Semaphore to be signalled.

   Return Value:
   IA_SUCCESS:				If the semaphore is successfully released.
   IA_INVALID_OS_HANDLE:	If the given handle is not valid.
   IA_FAIL:					Any other error.
------------------------------------------------------------------------------------ */
ia_result OS_DRV_SignalSemaphore( os_drv_semaphore_h hSemaphore )
{
	IrCa_Debug_Os(("OS_DRV_SignalSemaphore [hSemaphore = %d]  \n",hSemaphore));

	if ( hSemaphore == OSAL_INVALID_ID)
	{
		IrCa_Debug_Err(("Invalis param in OS_DRV_SignalSemaphore\n"));
		return IA_INVALID_OS_HANDLE;
	}
	else
	{
		osal_semaphore_release( hSemaphore);
		
		return	IA_SUCCESS ;
	}
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_WaitSemaphore()

   Description:
   This API is called to wait (block) for the semaphore to 
   get signaled (counter is decremented by one).

   Parameters:
   hSemaphore:	Semaphore to wait for.

   Return Value:
   IA_SUCCESS:				If the semaphore is successfully signalled.
   IA_INVALID_OS_HANDLE:	If the given handle is not valid.
   IA_FAIL:					Any other error.
------------------------------------------------------------------------------------ */
ia_result OS_DRV_WaitSemaphore( os_drv_semaphore_h hSemaphore )
{
	IrCa_Debug_Os(("OS_DRV_WaitSemaphore [hSemaphore = %d] \n",hSemaphore));

	if ( hSemaphore == OSAL_INVALID_ID)
	{
		IrCa_Debug_Err(("Invalis param in OS_DRV_WaitSemaphore\n"));
		return IA_INVALID_OS_HANDLE;
	}
	else
	{
		osal_semaphore_capture( hSemaphore, OSAL_WAIT_FOREVER_TIME );
		
		return	IA_SUCCESS ;	
	}
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_GetCurrentTime()

   Description:
   This API is called to retrieve an exact time reference, 
   namely the number of seconds since the Epoch(00:00:00 UTC, January 1, 1970). 
   This function maps to the commonly used ANSII C time() function. 
   The time must always be returned in UTC, not in local time!
   
   Parameters:
   None.

   Return Value:
   The value of time in seconds since 0 hours, 0 minutes, 0 seconds, January 1,
   1970, Coordinated Universal Time, without including leap seconds.
------------------------------------------------------------------------------------ */
ia_word32 OS_DRV_GetCurrentTime( void )
{
	IrCa_Debug_Os(("OS_DRV_GetCurrentTime\n"));
	//[irca]pyh:why?? 
}

/* ---------------------------------------------------------------------------------
   Name: OS_DRV_GetCurrentMs()

   Description:
   This API is called to retrieve the number of milliseconds counted since 
   the initialization of the device and is used as a high-precision time function. 
   The 32-bit unsigned value wraps around every ~74 hours.
   The minimum granularity of the time should be 10ms.
   
   Parameters:
   None.

   Return Value:
   The number of milliseconds since the initialization of the device.
------------------------------------------------------------------------------------ */
ia_word32 OS_DRV_GetCurrentMs( void )
{
	int DelayTime = 0;

	IrCa_Debug_Os(("OS_DRV_GetCurrentMs\n"));/**/
	DelayTime = osal_get_tick();

	if (DelayTime%10 != 0)
	{
		DelayTime = (DelayTime/10)*10 + 10;
	}
	
	return DelayTime;
}



