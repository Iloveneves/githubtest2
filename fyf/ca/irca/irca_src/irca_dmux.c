/*---------------------------------------------------------------------------------
   File Name	: irca_dmux.c

   Description	: Transport de-multiplexer and filter virtual driver to 
                  irdeto s3_dvb_client_3.6.8.lib
                  Create by PYH.
                  Please refer at See s3_vddf.h,
                  and Irdeto Access document 710467, "Operating System Driver API".

   Version		: REL_V1.0.0

   Copyright (c) 2008-2100 Triumph Corporation.  All rights reserved.

   History      : 1.2008/07/01 PYH - Create file and 
                    add Transport de-multiplexer and filter virtual driver.
                  2. 2008/11/11 Change to Ali Added by Ryan Lin
----------------------------------------------------------------------------------*/
#include <basic_types.h>
#include <api/libc/string.h>
#include <retcode.h>
#include <hld/hld_dev.h>
#include <hld/dmx/dmx.h>
#include <os/tds2/alitypes.h>
#include <irca/pubh/s3_portable_types.h>
#include <irca/pubh/s3_vd.h>
#include <irca/pubh/s3_vddf.h>
#include "irca_drv.h"
#include "irca_debug.h"


static OSAL_ID cas_sema = OSAL_INVALID_ID;
static struct dmx_device *cas_dmx_dev = NULL;

#define CAS_DMX_MUTEX_INIT()	if (cas_sema == OSAL_INVALID_ID) {\
			if ((cas_sema = osal_semaphore_create(1))==OSAL_INVALID_ID){\
			IrCa_Debug_Err(("create semaphore failure with %d\r", cas_sema)); \
			}}
#define CAS_DMX_MUTEX_DEL()	if (cas_sema != OSAL_INVALID_ID) {\
			osal_semaphore_delete(cas_sema);\
			cas_sema = OSAL_INVALID_ID;}
#define CAS_DMX_MUTEX_LOCK()	if(cas_sema!=OSAL_INVALID_ID) {while(osal_semaphore_capture(cas_sema, OSAL_WAIT_FOREVER_TIME) != SUCCESS) \
		IrCa_Debug_Err(("%s: capture timeout\n", __FUNCTION__));}
#define CAS_DMX_MUTEX_UNLOCK()	if(cas_sema!=OSAL_INVALID_ID) osal_semaphore_release(cas_sema);

//End Added

static Channel_t  stChannel[ NR_OF_CA_CHANNELS ];
static QUEUE_STRUCT stQueue[CA_QUEUE_SIZE];  
static UINT16  ReadIndex;
static UINT16  WriteIndex;

vd_rsc_id  *pDmxRId;
static vd_rsc_id DMX_RSC_ID = 0x0a;//Ëæ±ã¶¨ÒåµÄ


static void conti_rcv_sec_cb(struct get_section_param * param)
{
	BYTE*			pSection;
	UINT32 			SectLength;
	BYTE			CAChannelNo = NR_OF_CA_CHANNELS;
	BYTE			FilterNo = NR_OF_CA_FILTER;
    BYTE    		i;
	Channel_t *tmp_channel = (Channel_t *)param;

//	printf("******************conti_rcv_sec_cb\n");

	
	if(param->sec_hit_num >= MAX_MV_NUM)
		return;
	if(((1<<param->sec_hit_num) & param->mask_value->tb_flt_msk)==0)
	{
		IrCa_Debug_Err(("%s: not match!\n", __FUNCTION__));
		return;
	}
	if ((CHANNEL_INUSE != tmp_channel->channel_status)||(ENABLE_FILTER != tmp_channel->mv_enable[param->sec_hit_num]))
	{
		IrCa_Debug_Err(("abandan data! \n"));
		return;
	}

	for (i = 0; i < NR_OF_CA_CHANNELS; i++)
	{
		if (param->pid == stChannel[i].get_sec_param.pid)	
		{
			CAChannelNo = i;
			break;
		}
	}	
	if( i >= NR_OF_CA_CHANNELS || stChannel[CAChannelNo].ReceiveCallback == 0)
	{
	/*
		IrCa_Debug_Err(("err BufferHandle or ReceiveCallback in TP_Dmux_SectionMonitor [i=%d] [BufferHandle=0x%x] [ReceiveCallback=0x%x]\n",
			i,BufferHandle,stChannel[CAChannelNo].ReceiveCallback));
	*/	
		return ;
	}
	if (WriteIndex < CA_QUEUE_SIZE)
	{
		pSection = stQueue[WriteIndex].pSection;
	}
	else
	{
		IrCa_Debug_Err(("stQueue buffer full in TP_Dmux_SectionMonitor\n"));
		
		return;
	}

	SectLength = param->sec_tbl_len;
	if(SectLength > 1024)
	{
		IrCa_Debug_Err(("SectLength Run over SectLeng=%d\n",SectLength));
		return;
	}
	MEMCPY(pSection, param->buff, SectLength);
	FilterNo = param->sec_hit_num;
	
	stQueue[WriteIndex].SectionLength  = SectLength;
	stQueue[WriteIndex].SlotIndex = CAChannelNo;
	stQueue[WriteIndex].FilterIndex = FilterNo;
	WriteIndex = ( WriteIndex + 1 ) % CA_QUEUE_SIZE;

	stChannel[CAChannelNo].ReceiveCallback((vd_ref_id)CAChannelNo,stChannel[CAChannelNo].wSoftCellVal);

}
	
static ia_result TP_Dmux_AllocFilter(BYTE SlotIndex, vd_df_filter_id* FilterId)
{
	int FilterIndex;

	if (SlotIndex >= NR_OF_CA_CHANNELS || stChannel[SlotIndex].channel_status == CHANNEL_FREE)
	{
		IrCa_Debug_Err(("TP_Dmux_AllocFilter fail! [ invalid SlotIndex=%d]\n",SlotIndex));
		*FilterId = VD_DF_ILLEGAL_FILTER_ID;
		return IA_INVALID_PARAMETER;
	}
	for( FilterIndex=0 ;FilterIndex<NR_OF_CA_FILTER ;FilterIndex++  )
	{
		if((stChannel[SlotIndex].mv_enable[FilterIndex] == DISABLE_FILTER)
			/*&&((stChannel[SlotIndex].mask_value.tb_flt_msk & (1<<FilterIndex))== 0)*/)
		{
			break;
		}
	}

	IrCa_Debug_VdDmx(("TP_Dmux_AllocFilter Slot[%d] Filter[%d]\n",SlotIndex,FilterIndex ));

	if (FilterIndex >= NR_OF_CA_FILTER)
	{
		IrCa_Debug_Err(("TP_Dmux_AllocFilter fail! [ FilterIndex >= NR_OF_CA_FILTER]\n"));
		*FilterId = VD_DF_ILLEGAL_FILTER_ID;
		return IA_FULL;
	}
	*FilterId = FilterIndex;
	CAS_DMX_MUTEX_LOCK();
	stChannel[SlotIndex].mv_enable[FilterIndex] =READY_FILTER;
	CAS_DMX_MUTEX_UNLOCK();
	return IA_SUCCESS; 
	 
}


static ia_result TP_Dmux_FreeFilter(BYTE SlotIndex, BYTE FilterIndex)
{
	
	if (SlotIndex >= NR_OF_CA_CHANNELS || stChannel[SlotIndex].channel_status == CHANNEL_FREE)
		
	{
		IrCa_Debug_Err(("TP_Dmux_FreeFilter fail! [ invalid SlotIndex=%d]\n",SlotIndex));
		return IA_INVALID_PARAMETER;
	}

	if (FilterIndex >= NR_OF_CA_FILTER)
	{
		IrCa_Debug_Err(("TP_Dmux_FreeFilter fail! [ invalid FilterIndex=%d]\n",FilterIndex));
		return IA_INVALID_PARAMETER;
	}

	IrCa_Debug_VdDmx(("TP_Dmux_FreeFilter Slot[%d] Filter[%d] \n",SlotIndex,FilterIndex));

	
	CAS_DMX_MUTEX_LOCK();

	osal_task_dispatch_off();
	
	if((stChannel[SlotIndex].mask_value.tb_flt_msk&(~(1<<FilterIndex))== 0)&&(stChannel[SlotIndex].channel_status == CHANNEL_INUSE))
	{
		//if no filter is enable,free the slot
		dmx_io_control(cas_dmx_dev, IO_ASYNC_CLOSE, stChannel[SlotIndex].flt_id);
		stChannel[SlotIndex].channel_status = CHANNEL_READY;
	}
	
	stChannel[SlotIndex].mask_value.tb_flt_msk &= ~(1<<FilterIndex);
	stChannel[SlotIndex].mv_enable[FilterIndex] = DISABLE_FILTER;
	
	MEMSET(stChannel[SlotIndex].mask_value.value[FilterIndex],0,16);
	MEMSET(stChannel[SlotIndex].mask_value.multi_mask[FilterIndex],0,16);

	osal_task_dispatch_on();

	CAS_DMX_MUTEX_UNLOCK();
	return IA_SUCCESS;
	
}

static ia_result TP_Dmux_SetPid(BYTE SlotIndex,ia_word16 PID)
{
	if (SlotIndex >= NR_OF_CA_CHANNELS|| stChannel[SlotIndex].channel_status == CHANNEL_FREE)
			
	{
		IrCa_Debug_Err(("TP_Dmux_SetPid fail! [ invalid SlotIndex=%d]\n",SlotIndex));
		return IA_INVALID_PARAMETER;
	}
	IrCa_Debug_VdDmx(("TP_Dmux_SetPid  stChannel[%d].PidValue=0x%x\n",SlotIndex,PID));
	CAS_DMX_MUTEX_LOCK();
	
	
	stChannel[SlotIndex].PidValue = PID;

	CAS_DMX_MUTEX_UNLOCK();

	return IA_SUCCESS;
}

/*
table_id   				8 	uimsbf
table_id_exten 			16 	uimsbf
reserved	   			2	bslbf
version_number	  	 	5	uimsbf
current_next_indicator	1	bslbf
section_number	   		8	uimsbf
*/
static ia_result TP_Dmux_SetFilter(BYTE SlotIndex, BYTE FilterIndex, BYTE* pDataMask)
{
	BYTE  i;
   	BYTE acData[ 16 ],acMask[ 16 ];

	if (SlotIndex >= NR_OF_CA_CHANNELS || stChannel[SlotIndex].channel_status == CHANNEL_FREE)
			
	{
		IrCa_Debug_Err(("TP_Dmux_SetFilter fail! [ invalid SlotIndex=%d]\n",SlotIndex));
		return IA_INVALID_PARAMETER;
	}

	if (FilterIndex >= NR_OF_CA_FILTER )
	{
		IrCa_Debug_Err(("TP_Dmux_SetFilter fail! [ invalid FilterIndex=%d]\n",FilterIndex));
		return IA_INVALID_PARAMETER;
	}

	if(stChannel[SlotIndex].wFilterLength > MAX_SEC_MASK_LEN )
	{
		IrCa_Debug_Err(("TP_Dmux_SetFilter fail! [ invalid wFilterLength=%d]\n",
			stChannel[SlotIndex].mask_value.mask_len));
		return IA_FAIL;
	}

	//IrCa_Debug_VdDmx(("TP_Dmux_SetFilter  [SlotIndex=%d][FilterIndex=%d]\n ",SlotIndex ,FilterIndex));
	
	/*?????????wxf
	if((stChannel[SlotIndex].mask_value.tb_flt_msk & (1<<FilterIndex)) == 1)
	{
		return IA_FAIL;
	}*/
	MEMSET(acData,0,16);
	MEMSET(acMask,0,16);
	
	CAS_DMX_MUTEX_LOCK();
	
	stChannel[SlotIndex].mask_value.mask_len = 16;
	
	MEMCPY(acData,pDataMask,stChannel[SlotIndex].wFilterLength);
	MEMCPY(acMask,pDataMask+stChannel[SlotIndex].wFilterLength,stChannel[SlotIndex].wFilterLength);

	osal_task_dispatch_off();
	
	for(i=0; i<16; i++)
	{
		if(i >0)
		{
			stChannel[SlotIndex].mask_value.value[FilterIndex][i+2] = acData[i];
			stChannel[SlotIndex].mask_value.multi_mask[FilterIndex][i+2] = acMask[i];
		}
		else
		{
			stChannel[SlotIndex].mask_value.value[FilterIndex][i] = acData[i];
			stChannel[SlotIndex].mask_value.multi_mask[FilterIndex][i] = acMask[i];
		}
	}
	stChannel[SlotIndex].mask_value.tb_flt_msk |= 1<<FilterIndex;
	stChannel[SlotIndex].mv_enable[FilterIndex] = ENABLE_FILTER;

	if(stChannel[SlotIndex].channel_status==CHANNEL_READY)
	{
		stChannel[SlotIndex].channel_status = CHANNEL_FILTERSET;
	}
	IrCa_Debug_VdDmx(("TP_Dmux_SetFilter  [SlotIndex=%d][FilterIndex=%d][tb_flt_msk=%d]\n ",SlotIndex ,FilterIndex ,stChannel[SlotIndex].mask_value.tb_flt_msk ));
	
	osal_task_dispatch_on();

	CAS_DMX_MUTEX_UNLOCK();
	
	return IA_SUCCESS;
	
}

static ia_result TP_Dmux_ControlSlot(BYTE SlotIndex,unsigned int action)
{
	Channel_t *tmp_channel;
	ia_result  Result=IA_SUCCESS ; 
	
	if(SlotIndex >= NR_OF_CA_CHANNELS || stChannel[SlotIndex].channel_status == CHANNEL_FREE)
	{
		IrCa_Debug_Err(("TP_Dmux_ControlSlot fail! [ invalid SlotIndex=%d]\n",SlotIndex));
		return IA_INVALID_PARAMETER;
	}
	
	CAS_DMX_MUTEX_LOCK();

	if((stChannel[SlotIndex].channel_status == CHANNEL_INUSE) && (action == DISABLE_FILTER))
	{
		dmx_io_control(cas_dmx_dev, IO_ASYNC_CLOSE, stChannel[SlotIndex].flt_id);
		stChannel[SlotIndex].channel_status = CHANNEL_READY ;

		IrCa_Debug_VdDmx(("TP_Dmux_ControlSlot disable! [SlotIndex=%d]\n" , SlotIndex));
		
		Result = IA_SUCCESS;
	}
	if((stChannel[SlotIndex].channel_status == CHANNEL_FILTERSET) && (action == ENABLE_FILTER))
	{

		IrCa_Debug_VdDmx(("TP_Dmux_ControlSlot enable! [SlotIndex=%d]\n" , SlotIndex));
		
		tmp_channel = &stChannel[SlotIndex]; 
		tmp_channel->get_sec_param.pid = (UINT16)tmp_channel->PidValue;
		tmp_channel->get_sec_param.buff = (UINT8 *)(tmp_channel->sec_buf);
		tmp_channel->get_sec_param.buff_len = (UINT16)CAS_MAX_SEC_SIZE;
		tmp_channel->get_sec_param.wai_flg_dly = OSAL_WAIT_FOREVER_TIME; 
		tmp_channel->get_sec_param.mask_value = &(tmp_channel->mask_value);
		tmp_channel->get_sec_param.continue_get_sec = 1;
		tmp_channel->get_sec_param.get_sec_cb = conti_rcv_sec_cb;

		if(dmx_async_req_section(cas_dmx_dev, &(tmp_channel->get_sec_param), &(tmp_channel->flt_id)) != RET_SUCCESS)
		{
			IrCa_Debug_Err(("Err: %s-- no hardware channel\n", __FUNCTION__));

			Result = IA_FAIL;
		}
		else
		{
			tmp_channel->channel_status = CHANNEL_INUSE;
			Result = IA_SUCCESS;
		}
	}
	
	CAS_DMX_MUTEX_UNLOCK();
	return Result;
	
}

static void TP_Dmux_InitDataBuffer( void )
{
	ReadIndex  = 0;
	WriteIndex = 0;
}


BOOL TP_Dmux_Initialise(void)
{
	BYTE 	SlotIndex,FilterIndex;
	
	CAS_DMX_MUTEX_INIT();
	cas_dmx_dev = (struct dmx_device *)dev_get_by_type(NULL, HLD_DEV_TYPE_DMX);

	CAS_DMX_MUTEX_LOCK();
	
	MEMSET(stChannel, 0, sizeof(stChannel));

	for (SlotIndex = 0; SlotIndex < NR_OF_CA_CHANNELS; SlotIndex++)
	{
		for( FilterIndex =0 ; FilterIndex < NR_OF_CA_FILTER ; FilterIndex++)
		{
			stChannel[SlotIndex].mv_enable[FilterIndex] = 0;
		}
		stChannel[SlotIndex].eFilterType = VD_DF_EQUALITY;
		stChannel[SlotIndex].channel_status = CHANNEL_FREE;
	} 
	CAS_DMX_MUTEX_UNLOCK();
	
	return FALSE;
}

BOOL TP_Dmux_GetResources( vd_driver_type_e eDriverType, void **ppvList, ia_word16 *pwCount )
{
	if (pwCount == NULL)
	{
		IrCa_Debug_Err(("TP_Dmux_GetResources fail! [ pwCount == 0 || ppvList == 0 ]\n"));
		return TRUE;
	}
	
	*pwCount = 1;
	pDmxRId = (vd_rsc_id*)OS_DRV_AllocateMemory((sizeof(vd_rsc_id)*(*pwCount)));
	*pDmxRId = DMX_RSC_ID;
	
	*ppvList = (void*)pDmxRId;
	
	return FALSE;
}


vd_ref_id TP_Dmux_Open( vd_driver_type_e eDriverType, 
						vd_rsc_id wResourceID, void *pvData )
{
	vd_df_open_st* pstDmuxStatus;
	BYTE	SlotIndex=0xff;

	
	if (pvData == NULL)
	{
		IrCa_Debug_Err(("TP_Dmux_Open fail! [ pvData == NULL ]\n"));
		return VD_REF_ID_ERROR;
	}

	pstDmuxStatus = (vd_df_open_st*)(pvData);

	for( SlotIndex = 0; SlotIndex < NR_OF_CA_CHANNELS; SlotIndex++ )
	{
		if( stChannel[ SlotIndex ].channel_status == CHANNEL_FREE )
			break;
	}
	if( SlotIndex >= NR_OF_CA_CHANNELS )
	{
		IrCa_Debug_Err(("TP_Dmux_Open fail ! [ SlotIndex >= NR_OF_CA_CHANNELS]\n"));
		return VD_REF_ID_ERROR;
	}

	IrCa_Debug_VdDmx(("TP_Dmux_Open  [SlotIndex=%d]\n ",SlotIndex));
	
	CAS_DMX_MUTEX_LOCK();
	stChannel[SlotIndex].get_sec_param.get_sec_cb = conti_rcv_sec_cb;
	stChannel[SlotIndex].get_sec_param.buff = (UINT8 *)&(stChannel[SlotIndex].sec_buf);
	stChannel[SlotIndex].get_sec_param.buff_len = CAS_MAX_SEC_SIZE;

	stChannel[SlotIndex].get_sec_param.wai_flg_dly = OSAL_WAIT_FOREVER_TIME;
		
	stChannel[SlotIndex].ReceiveCallback 	= pstDmuxStatus->pfnReceiveNotify;
	stChannel[SlotIndex].wSoftCellVal 		= pstDmuxStatus->wSoftCellVal;
	stChannel[SlotIndex].wMaxSectionSize 	= pstDmuxStatus->wMaxSectionSize;
	stChannel[SlotIndex].eChannelType 		= pstDmuxStatus->eChannelType;
	stChannel[SlotIndex].channel_status 	= CHANNEL_READY;	
	CAS_DMX_MUTEX_UNLOCK();
	
	return (vd_ref_id)SlotIndex;
		
}


ia_result TP_Dmux_Close( vd_driver_type_e eDriverType, vd_ref_id wRefId )
{

	IrCa_Debug_VdDmx(("TP_Dmux_Close  [wRefId=%d] ",wRefId));

	if (wRefId >= NR_OF_CA_CHANNELS|| (stChannel[wRefId].channel_status == CHANNEL_FREE))

	{
		IrCa_Debug_Err(("TP_Dmux_Close fail! [ invalid wRefId=%d]\n",wRefId));
		return IA_FAIL;
	}

	//Initialize the global managment structure
	CAS_DMX_MUTEX_LOCK();
	MEMSET(&(stChannel[wRefId]), 0, sizeof(Channel_t));
	stChannel[wRefId].eFilterType	= VD_DF_EQUALITY;
	stChannel[wRefId].channel_status = CHANNEL_FREE;
	
	CAS_DMX_MUTEX_UNLOCK();
	
	return IA_SUCCESS;

}


ia_result TP_Dmux_IO(vd_driver_type_e eDriverType, 
						vd_ref_id wRefId, ia_word16 wOpCode,
						ia_word32 wDataWriteSize, void *pvDataWrite, 
						ia_word32 *pwDataReadSize, void *pvDataRead )
{
	ia_result ErrCode = IA_FAIL;
	
	//IrCa_Debug_VdDmx(("TP_Dmux_IO [eDriverType=%d] [wRefId=%d]\n",eDriverType,wRefId));

	switch (wOpCode) 
	{
		case VD_DF_ALLOCATE_FILTER :
		{
			vd_df_filter_id FilterId=VD_DF_ILLEGAL_FILTER_ID;
			vd_df_filter_st* pstFilter;
			vd_df_alloc_filter_st* pstWrite;

			pstFilter = (vd_df_filter_st*)pvDataRead;
			pstWrite = (vd_df_alloc_filter_st*)pvDataWrite;
			
			if (pvDataWrite == NULL || pvDataRead == NULL 
				|| wDataWriteSize <= 0 || *pwDataReadSize <= 0)
			{
				IrCa_Debug_Err(("VD_DF_ALLOCATE_FILTER fail! [ invalid param ]\n"));
				return IA_INVALID_PARAMETER;
			}

			ErrCode = TP_Dmux_AllocFilter((BYTE)wRefId,&FilterId);

			if (ErrCode == IA_SUCCESS)
			{
				CAS_DMX_MUTEX_LOCK();
				stChannel[wRefId].eFilterType = pstWrite->eFilterType;
				/*?????wxf*/
				stChannel[wRefId].wFilterLength= pstWrite->wFilterLength;
				CAS_DMX_MUTEX_UNLOCK();
				pstFilter->wFilterId = FilterId;
			}

		}
			break;

		case VD_DF_FREE_FILTER:
		{
			vd_df_filter_st* pstWrite;
			BYTE FilterIndex;

			//IrCa_Debug_VdDmx(("[wOpCode = VD_DF_FREE_FILTER] \n"));
			
			if (pvDataWrite == NULL || wDataWriteSize <= 0)
			{
				IrCa_Debug_Err(("VD_DF_FREE_FILTER fail! [pvDataWrite == NULL || wDataWriteSize <= 0]\n"));
				return IA_INVALID_PARAMETER;
			}

			pstWrite = (vd_df_filter_st*)pvDataWrite;
			FilterIndex = pstWrite->wFilterId;

			ErrCode = TP_Dmux_FreeFilter((BYTE)wRefId,FilterIndex);
			
			if (ErrCode == IA_SUCCESS)
			{
				/*  ????wxf
					CAS_DMX_MUTEX_LOCK();
					stChannel[wRefId].wFilterLength = 0;
					CAS_DMX_MUTEX_UNLOCK();
				*/
			}
		}
			break;

		case VD_DF_SET_PID:
		{
			vd_df_pid_st* pstPid;

			if (pvDataWrite == NULL || wDataWriteSize <= 0)
			{
				IrCa_Debug_Err(("VD_DF_SET_PID fail! [pvDataWrite == 0 || wDataWriteSize <= 0]\n"));
				return IA_INVALID_PARAMETER;
			}

			pstPid = (vd_df_pid_st*)pvDataWrite;
			ErrCode = TP_Dmux_SetPid((BYTE)wRefId,pstPid->wPid);
		}
			break;

		case VD_DF_SET_FILTER:
		{
			vd_df_filter_st* pstFilterId;
			BYTE* pWrite;
				
			if (pvDataWrite == NULL || wDataWriteSize <= 0)
			{
				IrCa_Debug_Err(("TP_Dmux_IO fail! [pvDataWrite == NULL || wDataWriteSize <= 0]\n"));
				return IA_INVALID_PARAMETER;
			}

			pWrite = (BYTE*)pvDataWrite;
			pstFilterId = (vd_df_filter_st*)pvDataWrite;

			
			ErrCode = TP_Dmux_SetFilter((BYTE)wRefId,(BYTE)(pstFilterId->wFilterId),pWrite+sizeof(vd_df_filter_st));
		}
			break;

		case VD_DF_READ_SECTION:
		{
			BYTE* pRead8;
			vd_df_read_st* pstRead;

			if (pvDataRead == NULL || *pwDataReadSize == 0 )
			{
				IrCa_Debug_Err(("VD_DF_READ_SECTION fail! [pvDataRead == 0]\n"));
				return IA_INVALID_PARAMETER;
			}
			if ( ReadIndex == WriteIndex )
			{
				IrCa_Debug_Err(("ReadIndex == WriteIndex [%d]\n",ReadIndex));
				*pwDataReadSize = 0;
				return IA_SUCCESS;
			}
			
			pstRead = (vd_df_read_st*)pvDataRead;
			pRead8 = (BYTE*)pvDataRead;

			if (*pwDataReadSize < stQueue[ReadIndex].SectionLength+sizeof(vd_df_read_st))
			{
				IrCa_Debug_Err(("VD_DF_READ_SECTION fail! [*pwDataReadSize too small]\n"));
				return IA_FULL;
			}

			pstRead->wFilterId = stQueue[ReadIndex].FilterIndex;
			pstRead->wSectionLength = stQueue[ReadIndex].SectionLength;

			if (stQueue[ReadIndex].SectionLength <= (*pwDataReadSize)-sizeof(vd_df_read_st))
			{
				CAS_DMX_MUTEX_LOCK();
				MEMCPY(pRead8+sizeof(vd_df_read_st),stQueue[ReadIndex].pSection,
						stQueue[ReadIndex].SectionLength);
				
				*pwDataReadSize = stQueue[ReadIndex].SectionLength+sizeof(vd_df_read_st);
				stQueue[ReadIndex].SectionLength = 0;
				stQueue[ReadIndex].pSection[0] = 0xff;
				ReadIndex = ( ReadIndex + 1 ) % CA_QUEUE_SIZE;
				CAS_DMX_MUTEX_UNLOCK();
			}
			else
			{
				*pwDataReadSize = 0;
				IrCa_Debug_Err(("VD_DF_READ_SECTION buffer not enough !! \n"));
			}
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_DF_START:
		{
			ErrCode = TP_Dmux_ControlSlot(wRefId,ENABLE_FILTER);
		}
			break;

		case VD_DF_STOP:
		{
			ErrCode = TP_Dmux_ControlSlot(wRefId,DISABLE_FILTER);
		}
			break;

		case VD_DF_RESET:
		{
			TP_Dmux_InitDataBuffer();	
		}
			ErrCode = IA_SUCCESS;
			break;

			
		default :
			IrCa_Debug_Err(("TP_Dmux_IO  [ invalid wOpCode = %d ]\n",wOpCode));
			ErrCode = VD_NOT_SUPPORTED;
			break;
	}
	
	return ErrCode;
}


