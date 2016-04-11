#include <irca/pubh/s3_portable_types.h>
#include <irca/pubh/s3_vd.h>
#include <irca/pubh/s3_vddf.h>
#include <irca/pubh/s3_vd_loader.h>
#include "irca_debug.h"


vd_ref_id TP_Loader_Open( vd_driver_type_e eDriverType, 
						vd_rsc_id wResourceID, void *pvData )
{
	/*IrCa_Debug_VdLod(("TP_Loader_Open [eDriverType=%d] [wResourceID=%d]\n",
		eDriverType,wResourceID));*/

	/*[irca]pyh:测试代码需要wResourceID == 0才是正确的*/
	if (wResourceID != 0)
	{
		IrCa_Debug_Err(("TP_Loader_Open fail! invalid param!\n"));
		return VD_REF_ID_ERROR;
	}

	return (vd_ref_id)0;
}


ia_result TP_Loader_Close( vd_driver_type_e eDriverType, vd_ref_id wRefId )
{
	/*IrCa_Debug_VdLod(("TP_Loader_Close [eDriverType=%d] [wRefId=%d]\n",
		eDriverType,wRefId));*/

	if (wRefId != 0 )
	{
		IrCa_Debug_Err(("TP_Loader_Close fail! [ invalid wRefId=%d]\n",wRefId));
		return IA_FAIL;
	}
	
	return IA_SUCCESS;
}


ia_result TP_Loader_IO(vd_driver_type_e eDriverType, 
						vd_ref_id wRefId, ia_word16 wOpCode,
						ia_word32 wDataWriteSize, void *pvDataWrite, 
						ia_word32 *pwDataReadSize, void *pvDataRead )
{
	ia_result ErrCode = IA_FAIL;
	
	//IrCa_Debug_VdLod(("TP_Loader_IO [eDriverType=%d] [wRefId=%d]",eDriverType,wRefId));

	if (pvDataRead == 0 || *pwDataReadSize == 0)
	{
		IrCa_Debug_Err(("TP_Loader_IO fail! [ pvDataRead == 0 || *pwDataReadSize == 0 ]\n"));
		return IA_INVALID_PARAMETER;
	} 

	switch (wOpCode)
	{
		case VD_LOADER_GET_SYSTEM_ID :
		{
			vd_loader_system_id_st* pRead;

			pRead = (vd_loader_system_id_st*)pvDataRead;
			pRead->wSystemId = 65535;
			//IrCa_Debug_VdLod(("[wOpCode=VD_LOADER_GET_SYSTEM_ID] wSystemId=%d\n",pRead->wSystemId));
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_LOADER_GET_KEY_VERSION:
		{
			vd_loader_key_version_st* pRead;

			pRead = (vd_loader_key_version_st*)pvDataRead;
			pRead->wKeyVersion = 0;
			//IrCa_Debug_VdLod(("[wOpCode=VD_LOADER_GET_KEY_VERSION] wKeyVersion=%d\n",pRead->wKeyVersion));
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_LOADER_GET_SIGNATURE_VERSION:
		{
			vd_loader_signature_version_st* pRead;

			pRead = (vd_loader_signature_version_st*)pvDataRead;
			pRead->wSignatureVersion = 0;
			//IrCa_Debug_VdLod(("[wOpCode=VD_LOADER_GET_SIGNATURE_VERSION] wSignatureVersion=%d\n",pRead->wSignatureVersion));
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_LOADER_GET_VARIANT:
		{
			vd_loader_variant_st* pRead;

			pRead = (vd_loader_variant_st*)pvDataRead;
			pRead->wVariant = 72;
			//IrCa_Debug_VdLod(("[wOpCode=VD_LOADER_GET_VARIANT] wVariant=%d\n",pRead->wVariant));
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_LOADER_GET_MANUFACTURER_ID:
		{
			vd_loader_manufacturer_id_st* pRead;

			pRead = (vd_loader_manufacturer_id_st*)pvDataRead;
			pRead->wManufacturerId = 72;
			//IrCa_Debug_VdLod(("[wOpCode=VD_LOADER_GET_MANUFACTURER_ID] wManufacturerId=%d\n",pRead->wManufacturerId));
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_LOADER_GET_HARDWARE_VERSION:
		{
			vd_loader_hardware_version_st* pRead;

			pRead = (vd_loader_hardware_version_st*)pvDataRead;
			pRead->wHardwareVersion = 4;
			//IrCa_Debug_VdLod(("[wOpCode=VD_LOADER_GET_HARDWARE_VERSION] wHardwareVersion=%d\n",pRead->wHardwareVersion));
		}
			ErrCode = IA_SUCCESS;
			break;

		case VD_LOADER_GET_LOAD_VERSION:
		{
			vd_loader_load_version_st* pRead;

			pRead = (vd_loader_load_version_st*)pvDataRead;
			pRead->wLoadVersion = 3;
			//IrCa_Debug_VdLod(("[wOpCode=VD_LOADER_GET_LOAD_VERSION] wLoadVersion=%d\n",pRead->wLoadVersion));
		}
			ErrCode = IA_SUCCESS;
			break;
			
		default :
			IrCa_Debug_Err(("TP_Loader_IO  [ invalid  wOpCode = %d]\n",wOpCode));
			ErrCode = VD_NOT_SUPPORTED;
			break;
	}
	
	return ErrCode;
}



