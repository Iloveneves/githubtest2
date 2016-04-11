#include <os/tds2/alitypes.h>
#include <basic_types.h>
#include <irca/pubh/s3_portable_types.h>
#include <irca/pubh/s3_vd.h>
#include <irca/pubh/s3_vdtmr.h>
#include "irca_debug.h"
#include "irca_drv.h"

extern UINT32 SCCommunUsedTime;
extern UINT32 SCCommunStartTime;

static TIMER_STRUCT stTimer[MAX_TIMER_NUM];
static BYTE CurTimerNum = 0;


static void	TP_Timer_Monitor(void *pvParam)
{
	int i;
	UINT32  UsedTime =0;
	UINT32  CurTime;

	while (TRUE)
	{
		if (CurTimerNum == 0)
		{
			osal_task_sleep(100);
		}
		else
		{
			osal_task_sleep(10);
		} 

		/*pyh:记录卡通信的间隔时间*/
		
		CurTime = osal_get_tick();
		SCCommunUsedTime = CurTime - SCCommunStartTime ;
		
		for (i=0;i<MAX_TIMER_NUM;i++)
		{
			if (CurTimerNum == 0)
				break;
			
			if ((stTimer[i].Status == TIMER_OPEN || stTimer[i].Status == TIMER_START)
				&& stTimer[i].TimeoutCallback != 0 
				&& stTimer[i].RefId < MAX_TIMER_NUM)
			{
				CurTime = osal_get_tick();
				stTimer[i].UsedTime = CurTime - stTimer[i].StartTime;
				UsedTime = stTimer[i].UsedTime;
				
			    if (UsedTime >= stTimer[i].Timeout )
				{
					//IrCa_Debug_VdTim(("TimeoutCallback [TimerIndex=%d]\n",stTimer[i].RefId));

					stTimer[i].StartTime = osal_get_tick();
					if (stTimer[i].Repetitive == IA_FALSE)
					{
						stTimer[i].Status = TIMER_STOP;
					}
					
					stTimer[i].TimeoutCallback(stTimer[i].RefId,stTimer[i].SoftCellVal);
					
				}
			}
		}
	}
}



#define TIMER_API


BOOL TP_Timer_Initialise(void)
{
	int TimerIndex;
	OSAL_T_CTSK t_ctsk;
	ID  TimerID = OSAL_INVALID_ID;

	
	CurTimerNum = 0;
	for (TimerIndex = 0; TimerIndex < MAX_TIMER_NUM; TimerIndex++)
	{
		stTimer[TimerIndex].TimeoutCallback  = 0;
		stTimer[TimerIndex].StartTime  	= 0;
		stTimer[TimerIndex].UsedTime  	= 0;
		stTimer[TimerIndex].SoftCellVal = 0;
		stTimer[TimerIndex].RefId		= MAX_TIMER_NUM;
		stTimer[TimerIndex].Repetitive  = IA_FALSE;
		stTimer[TimerIndex].Timeout   	= 0;
		stTimer[TimerIndex].Status   	= TIMER_CLOSE;
	}

	t_ctsk.stksz = 1024;
	t_ctsk.quantum = 10;
	t_ctsk.itskpri = OSAL_PRI_LOW;
	t_ctsk.name[0] = 'T';
	t_ctsk.name[1] = 'I';
	t_ctsk.name[2] = 'M';
	t_ctsk.task = (FP)TP_Timer_Monitor;

	TimerID = osal_task_create(&t_ctsk);

	if (TimerID == OSAL_INVALID_ID)
	{
		IrCa_Debug_Err(("Create Timer Tsk failed\n"));
		return TRUE;
	}/**/
	//IrCa_Debug_VdTim(("TP_Timer_Initialise success!\n"));
	
	return FALSE;
	
}



vd_ref_id TP_Timer_Open( vd_driver_type_e eDriverType, 
						vd_rsc_id wResourceID, void *pvData )
{
	vd_timer_open_st* pData;
	BYTE TimerIndex;
	
	/*IrCa_Debug_VdTim(("TP_Timer_Open [eDriverType=%d] [wResourceID=%d]\n",
		eDriverType,wResourceID));*/

	if (wResourceID == VD_RSC_ID_INVALID)
	{
		IrCa_Debug_Err(("TP_Timer_Open fail! invalid param!\n"));
		return VD_REF_ID_ERROR;
	}

	if (pvData == NULL)
	{
		IrCa_Debug_Err(("TP_Timer_Open fail! [ pvData == NULL ]\n"));
		return VD_REF_ID_ERROR;
	}

	for (TimerIndex=0;TimerIndex<MAX_TIMER_NUM;TimerIndex++)
	{
		if (stTimer[TimerIndex].Status == TIMER_CLOSE)
			break;
	}
	if (TimerIndex >= MAX_TIMER_NUM)
	{
		IrCa_Debug_Err(("TP_Timer_Open fail! [ Timer full ]\n"));
		return VD_REF_ID_ERROR;
	}
	
	pData = (vd_timer_open_st*)(pvData);
	
	stTimer[TimerIndex].TimeoutCallback = pData->pfnTimeoutNotify;
	stTimer[TimerIndex].StartTime  		= osal_get_tick();
	stTimer[TimerIndex].UsedTime  		= 0;
	stTimer[TimerIndex].SoftCellVal 	= pData->wSoftCellVal;
	stTimer[TimerIndex].RefId			= (vd_ref_id)TimerIndex;
	stTimer[TimerIndex].Repetitive  	= pData->fRepetitive;
	stTimer[TimerIndex].Timeout   		= pData->wTimeout;
	stTimer[TimerIndex].Status   		= TIMER_OPEN;
	CurTimerNum ++;

	/*IrCa_Debug_VdTim(("TP_Timer_Open success TimerIndex[%d] wSoftCellVal[0x%x] Repetitive[%d] Timeout[%d]\n",
		TimerIndex,pData->wSoftCellVal,pData->fRepetitive,pData->wTimeout));*/
	
	return (vd_ref_id)TimerIndex;
	
}


ia_result TP_Timer_Close( vd_driver_type_e eDriverType, vd_ref_id wRefId )
{
	int i;
	
	/*IrCa_Debug_VdTim(("TP_Timer_Close [eDriverType=%d] [wRefId=%d]\n",
		eDriverType,wRefId));*/

	if (wRefId >= MAX_TIMER_NUM)// || wRefId < 0 
	{
		IrCa_Debug_Err(("TP_Timer_Close fail! [ invalid wRefId=%d]\n",wRefId));
		return IA_FAIL;
	}
	if (stTimer[wRefId].Status == TIMER_CLOSE)
	{
		IrCa_Debug_Err(("TP_Timer_Close fail! [stTimer[%d].Status == TIMER_CLOSE ]\n",wRefId));
		return IA_FAIL;
	}

	stTimer[wRefId].TimeoutCallback = 0;
	stTimer[wRefId].StartTime  		= 0;
	stTimer[wRefId].UsedTime  		= 0;
	stTimer[wRefId].SoftCellVal 	= 0;
	stTimer[wRefId].RefId			= MAX_TIMER_NUM;
	stTimer[wRefId].Repetitive  	= IA_FALSE;
	stTimer[wRefId].Timeout   		= 0;
	stTimer[wRefId].Status   		= TIMER_CLOSE;

	if (CurTimerNum > 0)
	{
		CurTimerNum--;
	}
	
	//IrCa_Debug_VdTim(("TP_Timer_Close success [TimerIndex=%d]\n",wRefId));
	
	return IA_SUCCESS;
}



ia_result TP_Timer_IO(vd_driver_type_e eDriverType, 
						vd_ref_id wRefId, ia_word16 wOpCode,
						ia_word32 wDataWriteSize, void *pvDataWrite, 
						ia_word32 *pwDataReadSize, void *pvDataRead )
{
	ia_result ErrCode = IA_FAIL;
	
	//IrCa_Debug_VdTim(("TP_Timer_IO [eDriverType=%d] [wRefId=%d] ",eDriverType,wRefId));

	if (wRefId >= MAX_TIMER_NUM)// || wRefId < 0 )
	{
		IrCa_Debug_Err(("TP_Timer_IO fail! [ invalid wRefId=%d]\n",wRefId));
		return TRUE;
	}
	if (stTimer[wRefId].Status == TIMER_CLOSE)
	{
		IrCa_Debug_Err(("TP_Timer_IO fail! [stTimer[%d].Status == TIMER_CLOSE ]\n",wRefId));
		return TRUE;
	}

	switch (wOpCode)
	{
		case VD_TIMER_STOP :
		{
			//IrCa_Debug_VdTim(("[ wOpCode = VD_TIMER_STOP ]\n"));

			stTimer[wRefId].Status  = TIMER_STOP;
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_TIMER_START:
		{
			//IrCa_Debug_VdTim(("[ wOpCode = VD_TIMER_START ]\n"));
			stTimer[wRefId].Status  = TIMER_START;
		}
			ErrCode = IA_SUCCESS;
			break;
			
		default :
			IrCa_Debug_Err(("TP_Timer_IO  [ invalid  wOpCode ]\n"));
			ErrCode = VD_NOT_SUPPORTED;
			break;
	}
	
	return ErrCode;
}





