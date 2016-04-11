
#include <basic_types.h>
#include <retcode.h>
#include <api/libc/string.h>
#include <hld/smc/smc.h>
#include <os/tds2/alitypes.h>
#include <irca/pubh/s3_vdsc.h>
#include <irca/pubh/s3_portable_types.h>
#include <osal/osal.h>
#include <osal/osal_task.h>
#include "irca_drv.h"
#include "irca_debug.h"

static BYTE  T14ATR[14]={0x3B, 0x9F, 0x21, 0x0E, 0x49, 0x52, 0x44, 0x45, 0x54, 0x4F, 0x20, 0x41, 0x43, 0x53 };
static BYTE  T0ATR[15]={0x3B, 0x9F, 0x21, 0x40, 0x64, 0x49, 0x52, 0x44, 0x45, 0x54, 0x4F, 0x20, 0x41, 0x43, 0x53 };

static struct smc_device * TP_smc_dev = NULL;
static ATR_DATA_STRUCT  AtrStruct;

static BYTE 			ScProtocol = T14;
vd_rsc_id 				*pScRId;

static vd_rsc_id 		SC_RSC_ID = 0;/*暂定为0*/

static ia_word32 		ScSoftCellVal;
static BOOL 			bCardIn = FALSE;
static void 			(*pvSC_ATR_Notify)(vd_ref_id wRefId, ia_word32 wSoftCellVal,vd_sc_status_e eReason, void *pvInfo) = NULL;

/////////////////////////
static  OSAL_ID              CardLock_MutexId           = OSAL_INVALID_ID;
static  ID		             CardEvent_TskId             = OSAL_INVALID_ID;     
static  OSAL_ID              CardEvent_FlgId             = OSAL_INVALID_ID; 
////////////////////////

UINT32 SCCommunUsedTime;
UINT32 SCCommunStartTime;
////////////////////////
vd_ref_id TP_SmartCard_GetRscId(void)
{
	return (SC_RSC_ID);
}

BOOL IR_SC_GetCardIn(void)
{
	return bCardIn;
}
static vd_ref_id TP_SmartCard_GetRefId(void)
{
	return (vd_ref_id)0;
}

static void TP_SmartCard_SemLock(void)
{  
    osal_mutex_lock(CardLock_MutexId, OSAL_WAIT_FOREVER_TIME);	
}

static void TP_SmartCard_SemUnlock( void )
{    
    osal_mutex_unlock(CardLock_MutexId);
}

static void TP_SmartCard_EventHandle( UINT32 param)
{
	if (param ==0)
	{
		
		osal_flag_clear (CardEvent_FlgId, CA_SMC_CARD_IN_E);
    	osal_flag_set (CardEvent_FlgId, CA_SMC_CARD_OUT_E);
	}
	else if (param ==1)
	{
		osal_flag_clear (CardEvent_FlgId, CA_SMC_CARD_OUT_E);
    	osal_flag_set (CardEvent_FlgId, CA_SMC_CARD_IN_E);
	}
	return;
}

static BOOL TP_SmartCard_CheckATR( BYTE *response, BYTE *bProtocol )
{
    BYTE i;
	*bProtocol = INVALIDT15 ; 
	
    if (response[ 0 ] == T14ATR[ 0 ])
    {
		for (i = 5; i < 15; i++)
		{
			if (response[ i ] != T0ATR[ i ])
			{
				*bProtocol = INVALIDT15 ;
				break;
			}
		}
		if (i==15)
		{
			*bProtocol = T0;
		}
		if (*bProtocol == INVALIDT15)
		{
			for ( i = 2; i <14; i++ )
           	{
               if( response[ i ] != T14ATR[ i ] )
               {
					*bProtocol = INVALIDT15 ;
					break;
			   }
           	}
			if (i == 14)
			{
				*bProtocol = T14; 
			}
		}
    }
	if (*bProtocol == INVALIDT15)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void TP_SmartCard_Inserted(struct smc_device *smc_dev)
{
	UINT16 ATRLen =0;
	BOOL  ATRChk;
	BYTE  bProtocol;
	vd_sc_card_in_error_st CardErr;
	INT32  ResetErr = SUCCESS;

	
	if(smc_card_exist(smc_dev) != SUCCESS)
	{
		bCardIn = FALSE;
		if (pvSC_ATR_Notify != NULL)
		{
			pvSC_ATR_Notify(TP_SmartCard_GetRefId(), 
						ScSoftCellVal,VD_SC_CARD_OUT, NULL);
		}
		return ;
	}
	TP_SmartCard_SemLock();

	MEMSET(AtrStruct.pATR , 0 , 30);

	ResetErr = smc_reset(smc_dev, AtrStruct.pATR, &ATRLen);

	if(RET_SUCCESS != ResetErr)
	{
		smc_deactive(smc_dev);
		
		//IrCa_Debug_Err(("smc_reset fail! [ResetErr=%d]\n",ResetErr));

		bCardIn = FALSE;

		CardErr.eCardError = VD_SC_HARDWARE_ERROR;
		
		if (pvSC_ATR_Notify != NULL)
		{
			pvSC_ATR_Notify(TP_SmartCard_GetRefId(),ScSoftCellVal,
						VD_SC_CARD_IN_ERROR, (void*)(&CardErr)); 
		}
	}
	else
	{
		ATRChk = TP_SmartCard_CheckATR(AtrStruct.pATR ,  &bProtocol);

		if (ATRChk == TRUE)
    	{
			bCardIn = TRUE;
			AtrStruct.Length.wAtrLength = ATRLen;

			ScProtocol = bProtocol;
			
			if (pvSC_ATR_Notify != NULL)
			{    		
				IrCa_Debug_VdSc(("Smart card insert OK! bProtocol = %d \n",bProtocol));

				printf("Smart card insert OK! bProtocol = %d \n",bProtocol);

				
				
				pvSC_ATR_Notify(TP_SmartCard_GetRefId(), ScSoftCellVal,
								VD_SC_CARD_IN, (void*)(&AtrStruct));
			}

    	}
    	else
    	{
    		IrCa_Debug_Err(("TP_SmartCard_CheckATR fail!\n"));
			
			bCardIn = FALSE;
			CardErr.eCardError = VD_SC_ATR_INVALID_FORMAT;
			if (pvSC_ATR_Notify != NULL)
			{
				pvSC_ATR_Notify(TP_SmartCard_GetRefId(),ScSoftCellVal,
						VD_SC_CARD_IN_ERROR, (void*)(&CardErr));
			}
    	}
	}

	TP_SmartCard_SemUnlock();
}


static void TP_SmartCard_Removed( void )
{
	TP_SmartCard_SemLock();

    bCardIn = FALSE;
	
	smc_deactive(TP_smc_dev);
	
	if (pvSC_ATR_Notify != NULL)
	{
		pvSC_ATR_Notify(TP_SmartCard_GetRefId(), 
			ScSoftCellVal,VD_SC_CARD_OUT, NULL);
	}
	TP_SmartCard_SemUnlock();
}

static vd_sc_error_e  TP_SmartCard_T0Transfer( INT16 Num_toWrite, UINT8* pWrite, 
                                   				INT16 Num_toRead, UINT8* pRead, INT16 *pNumRead)
{
	INT32  scerr = SUCCESS ;
	vd_sc_error_e  IrScErr = VD_SC_CARD_OUT_ERROR;  /*默认为无卡*/
	
	if(smc_card_exist(TP_smc_dev) != SUCCESS)
	{
		return IrScErr;
	}
	
	TP_SmartCard_SemLock();

	scerr = smc_iso_transfer(TP_smc_dev , pWrite,  Num_toWrite ,  pRead , Num_toRead ,pNumRead );

	if (scerr==SUCCESS)
	{
		IrScErr = VD_SC_NO_LOGICAL_CHAN_SUPPORT; //此处解析为成功

		IrCa_Debug_VdSc(("TP_SmartCard_T0Transfer Succeed!\n"));
	}
	else
	{
	 	if (scerr==ERR_DEV_ERROR)
	 		IrScErr = VD_SC_HARDWARE_ERROR ;
		else
			IrScErr = VD_SC_COMM_ERROR;
	}

	TP_SmartCard_SemUnlock();

	return IrScErr ;
}

static vd_sc_error_e  TP_SmartCard_T14Transfer( INT16 Num_toWrite, UINT8* pWrite, 
                                   				INT16 Num_toRead, UINT8* pRead, INT16 *pNumRead)
{
	INT32  scerr = SUCCESS ;
	vd_sc_error_e  IrScErr = VD_SC_CARD_OUT_ERROR;  /*默认为无卡*/
	INT16    NumWriten =0;
	INT16	 NumRead =0 ;
	INT16	 NumToRead =0 ;
	UINT8   *pData;

	UINT16   i;
	
	
	if(smc_card_exist(TP_smc_dev) != SUCCESS)
	{
		return IrScErr;
	}
	TP_SmartCard_SemLock();
/*
	printf("\nData to Write---------------------strart num =%d\n" ,Num_toWrite);

	for( i =0 ; i<Num_toWrite ; i++)
	{
		printf("0x%-4x" , pWrite[i]);
	}

	printf("\nData to Write---------------------end\n");
*/	

	*pNumRead = 0;

	scerr = smc_raw_write(TP_smc_dev, pWrite , Num_toWrite, &NumWriten);
	if (scerr==SUCCESS)
	{
		NumToRead = 8 ;
		scerr = smc_raw_read(TP_smc_dev , pRead , NumToRead , &NumRead);

		if (scerr==SUCCESS)
		{
			*pNumRead += NumRead;
			pData = pRead + 8;
			NumToRead = pRead[ 7 ] + 1;
			NumRead = 0;

			scerr = smc_raw_read(TP_smc_dev , pData , NumToRead , &NumRead);

			*pNumRead += NumRead;
		}
	}
	if (scerr==SUCCESS)
	{
/*	
		printf("\nData Read---------------------strart  num=%d\n" , *pNumRead);

		for( i =0 ; i<(*pNumRead) ; i++)
		{
			printf("0x%-4x" , pRead[i]);
		}

		printf("\nData Read---------------------end\n");
*/

		
		IrScErr = VD_SC_NO_LOGICAL_CHAN_SUPPORT; //此处解析为成功
	}
	else
	{
		IrCa_Debug_VdSc(("TP_SmartCard_T14Transfer fail! [scerr=%d]\n",scerr));
		
	 	if (scerr==ERR_DEV_ERROR)
	 		IrScErr = VD_SC_HARDWARE_ERROR ;
		else
			IrScErr = VD_SC_COMM_ERROR;
	}

	TP_SmartCard_SemUnlock();

	return IrScErr;
}

void TP_SmartCard_EventManager(UINT32 parm1, UINT32 parm2)
{
	UINT32 event_flag;
	parm1 = parm2;
	
	osal_flag_clear (CardEvent_FlgId, CA_SMC_CARD_IN_E);

	while (1)
	{
		if (E_OK == osal_flag_wait(&event_flag, CardEvent_FlgId, 0x0000000F, TWF_ORW|TWF_CLR, OSAL_WAIT_FOREVER_TIME))
		{
			if (event_flag&CA_SMC_CARD_IN_E)
			{
				//CARD IN STATEMENT

				IrCa_Debug_VdSc(("\n---Card in---\n"));

				printf("\n---Card in---\n");
				
				IRCA_STA_InitSCStatus_ForCardIn();
				
				TP_SmartCard_Inserted(TP_smc_dev);
			}
			else if (event_flag&CA_SMC_CARD_OUT_E)
			{
				//CARD OUT STATEMENT
				printf("\n---Card Out---\n");
				
				IrCa_Debug_VdSc(("\n---Card Out---\n"));

				IRCA_STA_InitSCStatus_ForCardOut();
				TP_SmartCard_Removed();
			}
		}
		else
		{
		 	osal_task_sleep(10);
		}
	}
}
BOOL TP_SmartCard_EventInit(void)
{	
	OSAL_T_CTSK t_ctsk;
	INT32 retVal;
	UINT32 i;

//FLAG INIT	
	CardEvent_FlgId = osal_flag_create(0);
    if (OSAL_INVALID_ID == CardEvent_FlgId)
	{
	  IrCa_Debug_Err(("Create CardEvent_FlgId failed!\n"));
	  return TRUE;
	}
	
//MUTEX INIT
	CardLock_MutexId = osal_mutex_create (); 
    if (OSAL_INVALID_ID == CardLock_MutexId)
	{
      IrCa_Debug_Err(("Create mutex failed!\n"));
	  return TRUE;
	}
	
//TASK INIT
	t_ctsk.stksz = 2048;
	t_ctsk.quantum = 10;
	t_ctsk.itskpri = OSAL_PRI_NORMAL;
	t_ctsk.name[0] = 'S';
	t_ctsk.name[1] = 'C';
	t_ctsk.name[2] = 'E';
	t_ctsk.task = (FP)TP_SmartCard_EventManager;
	CardEvent_TskId = osal_task_create(&t_ctsk);
	if (CardEvent_TskId == OSAL_INVALID_ID)
	{
		IrCa_Debug_Err(("Create Tsk failed\n"));
		return TRUE;
	}

	IrCa_Debug_VdSc(("TP_SmartCard_Initialise Succeed!\n"));
	return FALSE;
}


#define SMARTCARD_API


BOOL TP_SmartCard_Initialise()
{
	struct smc_dev_config smc_config;

	INT32  smcerr = SUCCESS;

	MEMSET(&smc_config, 0, sizeof(struct smc_dev_config));
	smc_config.default_etu = 372;
	smc_config.def_etu_trigger = 0;
	smc_config.force_tx_rx_trigger = 1;
	smc_config.apd_disable_trigger = 1;
	smc_config.disable_pps =1;
	smcerr = smc_dev_attach(1, &smc_config);

	if( smcerr!= SUCCESS )
	{
		IrCa_Debug_Err(( "smc_dev_attach return UNSUCCESS"));
		return (TRUE);
	}

	TP_smc_dev = (struct smc_device *)dev_get_by_id(HLD_DEV_TYPE_SMC, 0);

	if(TP_smc_dev != NULL)
	{
		smcerr = smc_open(TP_smc_dev, TP_SmartCard_EventHandle);
		if(smcerr == SUCCESS )
		{
			return (TP_SmartCard_EventInit());
		}
		else
		{
			IrCa_Debug_Err(( "smc_open return UNSUCCESS"));
		}
	}
	return (TRUE);
}


BOOL TP_SmartCard_GetResources( vd_driver_type_e eDriverType, 
						void **ppvList, ia_word16 *pwCount )
{
	IrCa_Debug_VdSc(("TP_SmartCard_GetResources [eDriverType=%d]\n",eDriverType));

	if (pwCount == NULL)
	{
		IrCa_Debug_Err(("TP_SmartCard_GetResources fail! [ pwCount == 0 || ppvList == 0 ]\n"));
		return TRUE;
	}
	
	*pwCount = 1;
	pScRId = (vd_rsc_id*)OS_DRV_AllocateMemory((sizeof(vd_rsc_id)*(*pwCount)));
	
	*pScRId = SC_RSC_ID;
	*ppvList = (void*)pScRId;
	return FALSE;
}

vd_ref_id TP_SmartCard_Open( vd_driver_type_e eDriverType, 
						vd_rsc_id wResourceID, void *pvData )
{
	vd_sc_open_st* pstScStatus;
	
	IrCa_Debug_VdSc(("TP_SmartCard_Open [eDriverType=%d] [wResourceID=%d]\n",
		eDriverType,wResourceID));

	if (wResourceID != (*pScRId))
	{
		IrCa_Debug_Err(("TP_SmartCard_Open invalid param [ wResourceID=%d ]\n",wResourceID));
		return VD_REF_ID_ERROR;
	}
	if (pvData == NULL)
	{
		IrCa_Debug_Err(("TP_SmartCard_Open fail! [ pvData == 0 ]\n"));
		return VD_REF_ID_ERROR;
	}

	pstScStatus = (vd_sc_open_st*)(pvData);
	
	pvSC_ATR_Notify = pstScStatus->pfnStatusNotify;
	ScSoftCellVal = pstScStatus->wSoftCellVal;

	return TP_SmartCard_GetRefId();
}

ia_result TP_SmartCard_Close( vd_driver_type_e eDriverType, vd_ref_id wRefId )
{
	/**/IrCa_Debug_VdSc(("TP_SmartCard_Close [eDriverType=%d] [wRefId=%d]\n",
		eDriverType,wRefId));

	if (wRefId == VD_REF_ID_ERROR)
	{
		IrCa_Debug_Err(("TP_SmartCard_Close fail! invalid param\n"));
		return IA_FAIL;
	}

	return IA_SUCCESS;
}


ia_result TP_SmartCard_IO(vd_driver_type_e eDriverType, 
						vd_ref_id wRefId, ia_word16 wOpCode,
						ia_word32 wDataWriteSize, void *pvDataWrite, 
						ia_word32 *pwDataReadSize, void *pvDataRead )
{
	ia_result ErrCode = IA_FAIL;
	UINT32   CurTime;
	
	IrCa_Debug_VdSc(("TP_SmartCard_IO [eDriverType=%d] [wRefId=%d] ",eDriverType,wRefId));

	switch (wOpCode)
	{
		case VD_SC_COMMUNICATE :
		{
			vd_sc_error_e ScErr = VD_SC_CARD_OUT_ERROR;
			BYTE* Write=NULL;
			BYTE* Read=NULL;
			INT16  Num_toRead =0;
			INT16  NumRead=0;
 
			IrCa_Debug_VdSc(("[ wOpCode = VD_SC_COMMUNICATE ]\n"));

			if (pvDataWrite == NULL || wDataWriteSize == 0
				|| pvDataRead == NULL || *pwDataReadSize == 0)
			{
				IrCa_Debug_Err(("TP_SmartCard_IO fail! [pvDataWrite == 0 || wDataWriteSize == 0]\n"));
				return VD_OUT_OF_MEMORY;
			} 

			Write = (BYTE*)pvDataWrite;
			Read = (BYTE*)pvDataRead;

			if (bCardIn == FALSE)
			{
				IrCa_Debug_Err(("TP_SmartCard_IO fail! Card Out\n"));

				((vd_sc_read_error_st*)pvDataRead)->eError = ScErr;
				*pwDataReadSize = sizeof(vd_sc_read_error_st);
				return VD_WRITE_INCOMPLETE;
			}

			/*pyh:记录卡通信的间隔时间*/
			CurTime = osal_get_tick();
			SCCommunUsedTime = CurTime - SCCommunStartTime ;
					
			if (ScProtocol == T0)
			{
				if (Write[1] != 0) /*Write action*/
				{
					Num_toRead =0 ;
				}
				else
				{
					Num_toRead =*pwDataReadSize;  /*Read action*/
				}
				ScErr = TP_SmartCard_T0Transfer(Write[0]+Write[1] , Write+sizeof(vd_sc_write_st), Num_toRead, Read, &NumRead );
			}
			else if (ScProtocol == T14)
			{
				Num_toRead =*pwDataReadSize;
			
				ScErr = TP_SmartCard_T14Transfer(Write[0]+Write[1] , Write+sizeof(vd_sc_write_st), Num_toRead, Read, &NumRead );
			}
			else
			{
				IrCa_Debug_Err(("TP_SmartCard_IO fail! [invalid ScProtocol=%d]\n",ScProtocol));
				ErrCode = VD_NOT_SUPPORTED;
			}
			if (ErrCode!=VD_NOT_SUPPORTED)
			{
				if (ScErr < VD_SC_NO_LOGICAL_CHAN_SUPPORT)
				{
					IrCa_Debug_Err(("TP_SmartCard_SendT0Data fail\n"));
					((vd_sc_read_error_st*)pvDataRead)->eError = ScErr;
					*pwDataReadSize = sizeof(vd_sc_read_error_st);
					ErrCode = VD_WRITE_INCOMPLETE;
				}
				else
				{
					*pwDataReadSize = NumRead;
					ErrCode = IA_SUCCESS;
				}
			}
		}
		break;

		case VD_SC_RESET:

			IrCa_Debug_VdSc(("[ wOpCode = VD_SC_RESET ]\n"));
			TP_SmartCard_Inserted(TP_smc_dev);
			ErrCode = IA_SUCCESS;
			break;

		case VD_SC_GET_CARD_FORM:
			{
				vd_sc_get_card_form_st* CardForm;

				if (pvDataRead == NULL || *pwDataReadSize == 0)
				{
					IrCa_Debug_Err(("TP_SmartCard_IO fail! [ VD_SC_GET_CARD_FORM err]\n"));
					return VD_OUT_OF_MEMORY;
				}

				IrCa_Debug_VdSc(("[ wOpCode = VD_SC_GET_CARD_FORM ]\n"));
				
				CardForm = (vd_sc_get_card_form_st*)pvDataRead;
				CardForm->eScForm = VD_SC_CARD_FORM_NORMAL;
				*pwDataReadSize = sizeof(vd_sc_get_card_form_st);
				ErrCode = IA_SUCCESS;
			}
			break;

		case VD_SC_SOFTCLIENT_EVENT:
			IrCa_Debug_Err(("TP_SmartCard_IO  [ VD_SC_SOFTCLIENT_EVENT ]\n"));
			ErrCode = VD_NOT_SUPPORTED;
			break;
			
		default :
			IrCa_Debug_Err(("TP_SmartCard_IO  [ invalid  wOpCode]\n"));
			ErrCode = VD_NOT_SUPPORTED;
			break;
	}
	
	return ErrCode;
}




