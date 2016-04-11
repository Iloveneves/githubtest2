#include <os/tds2/alitypes.h>
#include <basic_types.h>
#include <irca/pubh/s3_portable_types.h>
#include <irca/pubh/s3_vd.h>
#include <irca/pubh/s3_vddd.h>
#include <irca/pubh/s3_vdps.h>
#include "irca_debug.h"
#include "irca_drv.h"

static STG_STRUCT stStg[MAX_STG_REFID];

void TP_Storage_Initialise(void)
{
	int i;
	
	for (i=0;i<MAX_STG_REFID;i++)
	{
		stStg[i].RefId = STG_REFID_INDEX+MAX_STG_REFID;
		stStg[i].pvErr_Notify = NULL;
		stStg[i].SoftCellVal = 0;
		stStg[i].DeleTag = FALSE;
	}
	//IrCa_Debug_VdStg(("TP_Storage_Initialise success!\n"));
}

#define STORAGE_API


vd_ref_id TP_Storage_Open( vd_driver_type_e eDriverType, 
						vd_rsc_id wResourceID, void *pvData )
{
	vd_ps_open_st* pstStgStatus;
	int index;
	
	/*IrCa_Debug_VdStg(("TP_Storage_Open [eDriverType=%d] [wResourceID=%d]\n",
		eDriverType,wResourceID));*/

	if (pvData == 0 || wResourceID == VD_RSC_ID_INVALID)
	{
		IrCa_Debug_Err(("TP_Storage_Open fail! [ pvData == 0 || wResourceID == VD_RSC_ID_INVALID ]\n"));
		return VD_REF_ID_ERROR;
	}

	for ( index = 0; index < MAX_STG_REFID; index++ )
	{
		if (stStg[index].RefId == (STG_REFID_INDEX+MAX_STG_REFID) 
			&& stStg[index].pvErr_Notify == NULL)
			break;
	}
	if( index >= MAX_STG_REFID )
	{
		IrCa_Debug_Err(("TP_Storage_Open fail ! [ index >= MAX_STG_REFID ]\n"));
		return VD_REF_ID_ERROR;
	}

	pstStgStatus = (vd_ps_open_st*)(pvData);
	
	stStg[index].pvErr_Notify = pstStgStatus->pfnErrorNotify;
	stStg[index].SoftCellVal = pstStgStatus->wSoftCellVal;
	stStg[index].DeleTag = FALSE;
	
	return (vd_ref_id)index;
}

ia_result TP_Storage_Close( vd_driver_type_e eDriverType, vd_ref_id wRefId )
{
	/**/IrCa_Debug_VdStg(("TP_Storage_Close [eDriverType=%d] [wRefId=%d]\n",
		eDriverType,wRefId));

	if (wRefId >= (STG_REFID_INDEX+MAX_STG_REFID) || stStg[wRefId].pvErr_Notify == NULL)
	{
		IrCa_Debug_Err(("TP_Storage_Close fail! invalid param [ wRefId =%d ]\n",wRefId));
		return IA_FAIL;
	}

	stStg[wRefId].pvErr_Notify = NULL;
	stStg[wRefId].SoftCellVal = 0;
	return IA_SUCCESS;
}

ia_result TP_Storage_IO(vd_driver_type_e eDriverType, 
						vd_ref_id wRefId, ia_word16 wOpCode,
						ia_word32 wDataWriteSize, void *pvDataWrite, 
						ia_word32 *pwDataReadSize, void *pvDataRead )
{
	ia_result ErrCode = IA_FAIL;
	
	/*IrCa_Debug_VdStg(("TP_Storage_IO [eDriverType=%d] [wRefId=%d] ",eDriverType,wRefId));*/

	if (wRefId >= (STG_REFID_INDEX+MAX_STG_REFID))
	{
		IrCa_Debug_Err(("TP_Storage_IO fail! invalid param [ wRefId =%d ]\n",wRefId));
		return IA_INVALID_PARAMETER;
	}

	if (stStg[wRefId].pvErr_Notify == NULL)
	{
		IrCa_Debug_Err(("TP_Storage_IO fail! [ pvStg_Err_Notify == NULL ]\n"));
		return IA_FAIL;
	}
	
	switch (wOpCode)
	{
		case VD_PS_WRITE :
		{
			BYTE* pWrite=0;
			vd_ps_write_st* pstWrite;
			ia_word32 wStart=0;
			ia_word32 wLength=0;

			if (stStg[wRefId].DeleTag == TRUE)
			{
				IrCa_Debug_Err(("TP_Storage_IO fail! [ StgDeleTag == TRUE ]\n"));
				stStg[wRefId].pvErr_Notify(wRefId, stStg[wRefId].SoftCellVal,VD_WRITE_INCOMPLETE);
				return IA_FAIL;
			}
			
			if (pvDataWrite == NULL || wDataWriteSize == 0)
			{
				IrCa_Debug_Err(("TP_Storage_IO fail! [ pvDataWrite == NULL || wDataWriteSize == 0]\n"));
				stStg[wRefId].pvErr_Notify(wRefId, stStg[wRefId].SoftCellVal,VD_WRITE_INCOMPLETE);
				return IA_INVALID_PARAMETER;
			}

			pWrite = (BYTE*)pvDataWrite;
			pstWrite = (vd_ps_write_st*)pvDataWrite;
			wStart = pstWrite->wStart;
			wLength = pstWrite->wLength;

			/*IrCa_Debug_VdStg(("[ wOpCode = VD_PS_WRITE ] offset=%d,wLength=%d wDataWriteSize=%d\n",
				wStart,wLength,wDataWriteSize));*/

			if ((wLength > wDataWriteSize)||(wLength > CA_PT_SIZE)||(wStart >= CA_PT_SIZE)/*||((wStart+wLength) > (CA_PT_INDEX+CA_PT_SIZE))*/)
				
			{
				IrCa_Debug_Err(("TP_Storage_IO fail! [ invalid length in VD_PS_WRITE]\n"));
				stStg[wRefId].pvErr_Notify(wRefId, stStg[wRefId].SoftCellVal,VD_WRITE_INCOMPLETE);
				return VD_OUT_OF_MEMORY;
			}


			EEP_24X_API_Set(CA_PT_INDEX+wStart,pWrite+sizeof(vd_ps_write_st),wLength);

			
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_PS_READ:
		{
			BYTE* pRead=0;
			vd_ps_read_st* pstWrite;
			ia_word32 wStart=0;
			ia_word32 wLength=0;
			
			if (stStg[wRefId].DeleTag == TRUE)
			{
				IrCa_Debug_Err(("TP_Storage_IO fail! [ StgDeleTag == TRUE ]\n"));
				stStg[wRefId].pvErr_Notify(wRefId, stStg[wRefId].SoftCellVal,VD_WRITE_INCOMPLETE);
				return IA_FAIL;
			}

			if (pvDataRead == NULL || *pwDataReadSize == 0 || pvDataWrite == NULL)
			{
				IrCa_Debug_Err(("TP_Storage_IO fail! [pvDataRead == NULL || *pwDataReadSize == 0 || pvDataWrite == NULL]\n"));
				return IA_INVALID_PARAMETER;
			}

			pstWrite = (vd_ps_read_st*)pvDataWrite; /*[irca]pyh:资料里面是说使用pvDataRead*/
			wStart = pstWrite->wStart; 
			wLength = pstWrite->wLength;

			/*IrCa_Debug_VdStg(("[ wOpCode = VD_PS_READ ] offset=%d,wLength=%d (*pwDataReadSize)=%d\n",
				wStart,wLength,(*pwDataReadSize)));*/

			if ((wLength > (*pwDataReadSize))|| (wLength > CA_PT_SIZE)||( wStart >= CA_PT_SIZE)/*||(wStart+wLength > CA_PT_INDEX+CA_PT_SIZE)*/)
				
			{
				IrCa_Debug_Err(("TP_Storage_IO fail! [ invalid length in VD_PS_READ]\n"));
				return IA_FULL;
			}

			pRead = (BYTE*)pvDataRead;
			*pwDataReadSize = wLength;
		
			EEP_24X_API_Get_Ali( CA_PT_INDEX+wStart, pRead, wLength) ;
		
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_PS_DELETE:
		{
			IrCa_Debug_VdStg(("[ wOpCode = VD_PS_DELETE ]\n"));
			//EEP_24X_API_SetByts(CA_PT_INDEX,0,CA_PT_SIZE); [irca]pyh:此函数无法退出
			stStg[wRefId].DeleTag = TRUE;
		}
			ErrCode = IA_SUCCESS;
			break;
			
		default :
			IrCa_Debug_Err(("TP_Storage_IO  [ invalid  wOpCode = %d ]\n",wOpCode));
			ErrCode = VD_NOT_SUPPORTED;
			break;
	}
	
	return ErrCode;
}





