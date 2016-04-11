/*---------------------------------------------------------------------------------
   File Name	: irca_virtual_driver.c

   Description	: virtual driver driver interferce to irdeto s3_dvb_client_3.6.8.lib
                  Create by PYH.
                  Please refer at See s3_vd.h,
                  and Irdeto Access document 
                  734000_3.29.03_SoftCell3_Virtual_Driver_API_for_DVB_Clients.pdf

   Version		: REL_V1.0.0

   Copyright (c) 2008-2100 Triumph Corporation.  All rights reserved.

   History      : 1.2008/07/02 PYH - Create file and 
                    add virtual driver interferc.
----------------------------------------------------------------------------------*/

/* Includes ---------------------------------------------------------------- */
#include <irca/pubh/s3_portable_types.h>
#include <irca/pubh/s3_vd.h>
#include "irca_debug.h"

/* External global variables ---------------------------------------------- */

/* Private Constants ------------------------------------------------------ */

/* private variables ------------------------------------------------------ */

/* Global variables ------------------------------------------------------- */

/* External global function ----------------------------------------------- */

/* Private Function ------------------------------------------------------- */

/* Global Function -------------------------------------------------------- */



/* ---------------------------------------------------------------------------------
   Name: VD_Initialise()

   Description:
   Initializes the virtual drivers. SoftCell 3 will call this function 
   once during its initialization. This function will be used to initialize 
   virtual drivers and (pre-)initialize resources on start-up.

   Parameters:
   None.

   Return Value:
   IA_SUCCESS:		On success.
   other values:	Indicating the errors. See Virtual_Driver_API's section 3.4.8.
------------------------------------------------------------------------------------ */
ia_result VD_Initialise( void )
{

	TP_Storage_Initialise();
	TP_Desc_init();
		
	if (TP_SmartCard_Initialise())
	{
		IrCa_Debug_Err(("TP_SmartCard_Initialise fail\n"));
		return IA_FAIL;
	}
	if (TP_Dmux_Initialise())
	{
		IrCa_Debug_Err(("TP_Dmux_Initialise fail\n"));
		return IA_FAIL;
	}
	if (TP_Timer_Initialise())
	{
		IrCa_Debug_Err(("TP_Timer_Initialise fail\n"));
		return IA_FAIL;
	}

	return IA_SUCCESS;

}

/* ---------------------------------------------------------------------------------
   Name: VD_Terminate()

   Description:
   Terminates the virtual drivers. SoftCell 3 will call this function once 
   during its termination. This function will be used to close all virtual 
   drivers and open resources on close down.

   Parameters:
   None.

   Return Value:
   None.
------------------------------------------------------------------------------------ */
void VD_Terminate( void )
{

}

/* ---------------------------------------------------------------------------------
   Name: VD_GetDriverVersion()

   Description:

   Parameters:
   eDriverType:		Version numbers start with 1.0 
                    (VD_NOT_IMPLEMENTED Means driver not implemented). 

   Return Value:
   the driver version for that virtual driver. 
------------------------------------------------------------------------------------ */
ia_word16 VD_GetDriverVersion( vd_driver_type_e eDriverType )
{
	switch (eDriverType)
	{
		case VD_DRIVER_SMARTCARD :
	 		return (1);
			break;

		case VD_DRIVER_PERSISTENT_STORAGE :
	 		return (1);
			break;

		case VD_DRIVER_DVB_FILTER :
	 		return (1);
			break;

		case VD_DRIVER_DVB_DESCRAMBLER :
	 		return (1);
			break;

		case VD_DRIVER_TIMER :
	 		return (1);
			break;

		case VD_DRIVER_LOADER :
	 		return (1);
			break;
			
		default :
			IrCa_Debug_Err(("VD_GetDriverVersion fail! [eDriverType=%d]\n",eDriverType));
			return VD_NOT_IMPLEMENTED;
			break;
	}
}

/* ---------------------------------------------------------------------------------
   Name: VD_GetResources()

   Description:
   Gets the list of resources that are known to that virtual driver. 
   The memory pointed to by the ppvList pointer has to be allocated by 
   the virtual driver, using the OS virtual driver API call OS_DRV_AllocateMemory(), 
   and will be freed by SoftCell 3.
   Not all the virtual drivers need to support this API. 
   It will be defined, per virtual driver, if this function should be supported.

   Parameters:
   eDriverType:		Identifies the virtual driver type.
   ppvList:			Data returned with the list of resources. 
                    The meaning of this data is defined for each virtual driver 
                    type that supports this call.
   pwCount:			The number of resources in the list will be returned 
                    in this word pointer.

   Return Value:
   IA_SUCCESS:			If the virtual driver can enumerate its resources.
   VD_NOT_SUPPORTED:	If it doesn't support this call.
------------------------------------------------------------------------------------ */
ia_result VD_GetResources( vd_driver_type_e eDriverType, 
						void **ppvList, ia_word16 *pwCount )
{
	switch (eDriverType)
	{
		case VD_DRIVER_SMARTCARD :
	 		if (TP_SmartCard_GetResources(eDriverType,ppvList,pwCount))
 			{
 				IrCa_Debug_Err(("VD_GetResources fail [VD_DRIVER_SMARTCARD]\n"));
				return VD_NOT_SUPPORTED;
 			}
			
			break;

		case VD_DRIVER_PERSISTENT_STORAGE :
			IrCa_Debug_Err(("VD_GetResources fail [VD_DRIVER_PERSISTENT_STORAGE]\n"));
			return VD_NOT_SUPPORTED;
			break;

		case VD_DRIVER_DVB_FILTER :
	 		if (TP_Dmux_GetResources(eDriverType,ppvList,pwCount))
 			{
 				IrCa_Debug_Err(("VD_GetResources fail [VD_DRIVER_DVB_FILTER]\n"));
				return VD_NOT_SUPPORTED;
 			}
			break;

		case VD_DRIVER_DVB_DESCRAMBLER :
			IrCa_Debug_Err(("VD_GetResources fail [VD_DRIVER_DVB_DESCRAMBLER]\n"));
			return VD_NOT_SUPPORTED;
			break;

		case VD_DRIVER_TIMER :
			IrCa_Debug_Err(("VD_GetResources fail [VD_DRIVER_TIMER]\n"));
			return VD_NOT_SUPPORTED;
			break;

		case VD_DRIVER_LOADER :
			IrCa_Debug_Err(("VD_GetResources fail [VD_DRIVER_LOADER]\n"));
			return VD_NOT_SUPPORTED;
			break;

		default :
			IrCa_Debug_Err(("VD_GetResources fail! [eDriverType=%d]\n",eDriverType));
			return VD_NOT_SUPPORTED;
			break;
	}
	return IA_SUCCESS;
}

/* ---------------------------------------------------------------------------------
   Name: VD_Open()

   Description:
   Opens a resource on given virtual driver type. How to open a resource is 
   virtual driver dependent and is described per virtual driver.

   The returned reference id (vd_ref_id) acts as a handle to the opened instance 
   of the resource for a specific virtual driver. The range 0x00000000 up to 0xFFFEFFFF 
   (VD_REF_ID_MAX) can be used for reference ID values. The range 0xFFFF0000 to 
   0xFFFFFFFF is reserved for special reference IDs and for return values indicating 
   errors. These reserved values are currently defined:
   1, VD_REF_ID_GLOBAL: Reference ID used to send truly global commands to a 
      virtual driver (see section 3.3.7, VD_IO). VD_REF_ID_GLOBAL is a global 
      reference to the virtual driver(not to a resource) for the same lifetime 
      as the virtual driver, i.e. opened implicitly at the initialization of 
      the virtual driver (during SoftCell 3 initialization), and closed at 
      termination of the virtual driver (during SoftCell 3 termination).
   2, VD_REF_ID_ERROR: General error. The resource could not be opened.

   Parameters:
   eDriverType:		Identifies the virtual driver type.
   wResourceID:		Identifies the resource for the given virtual driver type. 
                    The wResourceID starts from 0.
   pvData:			0 if no additional data needs to be specified or pointer to the data. 
                    Format of that data depends on each individual virtual driver.

   Return Value:
   VD_REF_ID_ERROR:					If error.
   0x00000000 up to 0xFFFEFFFF:		The reference ID.
------------------------------------------------------------------------------------ */
vd_ref_id VD_Open( vd_driver_type_e eDriverType, vd_rsc_id wResourceID, void *pvData )
{
	switch (eDriverType)
	{
		case VD_DRIVER_SMARTCARD :
			
	 		return TP_SmartCard_Open(eDriverType,wResourceID,pvData);
 						
			break;

		case VD_DRIVER_PERSISTENT_STORAGE :
			
	 		return TP_Storage_Open(eDriverType,wResourceID,pvData);
			
			break;
			
		case VD_DRIVER_DVB_FILTER :
	 		
			return TP_Dmux_Open(eDriverType,wResourceID,pvData);
			
			break;

		case VD_DRIVER_DVB_DESCRAMBLER :
	 		
			return TP_Desc_Open(eDriverType,wResourceID,pvData);
			
			break;

		case VD_DRIVER_TIMER :
	 		
			return TP_Timer_Open(eDriverType,wResourceID,pvData);
			
			break;

		case VD_DRIVER_LOADER :
			
			return TP_Loader_Open(eDriverType,wResourceID,pvData);
			
			break;
			
		default :

			IrCa_Debug_Err(("VD_Open fail! [eDriverType=%d]\n",eDriverType));
			return VD_REF_ID_ERROR;
			
			break;
	}
	
}

/* ---------------------------------------------------------------------------------
   Name: VD_Close()

   Description:
   Closes the reference to a resource on given virtual driver type. 
   How to close a reference to a resource is virtual driver dependent and 
   is described per virtual driver. Any notify function for the given reference 
   will be invalid when VD_Close() is entered and can not be called during 
   or after VD_Close() for this reference.

   Parameters:
   eDriverType:		Identifies the virtual driver type.
   wRefId:			Reference ID returned from a previous VD_Open() call.

   Return Value:
   IA_SUCCESS:		On success.
   other values:	Indicating errors.
------------------------------------------------------------------------------------ */
ia_result VD_Close( vd_driver_type_e eDriverType, vd_ref_id wRefId )
{
	switch (eDriverType)
	{
		case VD_DRIVER_SMARTCARD :
			
	 		return TP_SmartCard_Close(eDriverType,wRefId);
 			
			break;

		case VD_DRIVER_PERSISTENT_STORAGE :
			
	 		return TP_Storage_Close(eDriverType,wRefId);
 			
			break;

		case VD_DRIVER_DVB_FILTER :
			
	 		return TP_Dmux_Close(eDriverType,wRefId);
 			
			break;

		case VD_DRIVER_DVB_DESCRAMBLER :
			
	 		return TP_Desc_Close(eDriverType,wRefId);
 			
			break;

		case VD_DRIVER_TIMER :
			
	 		return TP_Timer_Close(eDriverType,wRefId);
 			
			break;

		case VD_DRIVER_LOADER :

			return TP_Loader_Close(eDriverType,wRefId);
			
			break;
		
		default :

			IrCa_Debug_Err(("VD_Close fail! [eDriverType=%d]\n",eDriverType));
			return IA_FAIL;
			break;
	}
	return IA_SUCCESS;
}

/* ---------------------------------------------------------------------------------
   Name: VD_IO()

   Description:
   1, Communicates with a resource instance. Each virtual driver determines 
      the meaning of pvDataWrite and pvDataRead by use of the wOpCode value.
   2, SoftCell 3 will always be responsible for the buffers pointed to by 
      pvDataWrite and pvDataRead. The virtual driver is responsible for copying 
      the data into the buffer pointed to by pvDataRead. The virtual driver must 
      make sure that the size of the copied data is smaller or equal to the size 
      in *pvDataReadSize by SoftCell 3.
   3, SoftCell 3 will set pvDataWrite to 0 and wDataWriteSize to 0 if there is 
      no data on input. SoftCell3 will set pvDataRead to 0 and pwDataReadSize to 
      0 if there is no data expected on output.
   4, If the size of the read buffer, given on input in pwDataReadSize and pointed 
      to by pvDataRead, is too small to hold the read data then the virtual driver 
      must return no data, set the needed size in pwDataReadSize and return with 
      return value IA_FULL.
   5, In some cases the special reference ID VD_REF_ID_GLOBAL can be used to send 
      truly global command to the virtual driver. This reference ID can be used 
      without opening a resource and acts on the virtual driver, not on an open resource.
   6, If a given wOpCode value is not known to the virtual driver, then it must 
      be ignored and the virtual driver must return VD_NOT_SUPPORTED.

   Parameters:
   eDriverType:		Identifies the virtual driver type.
   wRefId:			Reference ID returned from a previous VD_Open() call.
   wOpCode:			Code defining the action to be taken.
   wDataWriteSize:	Size of the data on input.
   pvDataWrite:		Pointer to data supplied by SoftCell 3 to the virtual driver on input.
   pwDataReadSize:	On input: size of the buffer.
                    On return: size of the actual data returned.
   pvDataRead:		Pointer to the buffer in which the virtual driver can return 
                    data to SoftCell 3.

   Return Value:
   Result of the control message.
------------------------------------------------------------------------------------ */
ia_result VD_IO( vd_driver_type_e eDriverType, vd_ref_id wRefId, ia_word16 wOpCode,
				ia_word32 wDataWriteSize, void *pvDataWrite, 
				ia_word32 *pwDataReadSize, void *pvDataRead )
{
	ia_result ErrCode = IA_FAIL;
	
	switch (eDriverType)
	{
		case VD_DRIVER_SMARTCARD :
	 		ErrCode = TP_SmartCard_IO( eDriverType, wRefId, wOpCode,
								wDataWriteSize, pvDataWrite, 
								pwDataReadSize, pvDataRead );
			break;

		case VD_DRIVER_PERSISTENT_STORAGE :
			
	 		ErrCode = TP_Storage_IO( eDriverType, wRefId, wOpCode,
								wDataWriteSize, pvDataWrite, 
								pwDataReadSize, pvDataRead );
			break;

		case VD_DRIVER_DVB_FILTER :
	 		ErrCode = TP_Dmux_IO( eDriverType, wRefId, wOpCode,
								wDataWriteSize, pvDataWrite, 
								pwDataReadSize, pvDataRead );
			break;

		case VD_DRIVER_DVB_DESCRAMBLER :
			
	 		ErrCode = TP_Desc_IO( eDriverType, wRefId, wOpCode,
								wDataWriteSize, pvDataWrite, 
								pwDataReadSize, pvDataRead );
			break;

		case VD_DRIVER_TIMER :
			
	 		ErrCode = TP_Timer_IO( eDriverType, wRefId, wOpCode,
								wDataWriteSize, pvDataWrite, 
								pwDataReadSize, pvDataRead );
			break;
			
		case VD_DRIVER_LOADER :
			
	 		ErrCode = TP_Loader_IO( eDriverType, wRefId, wOpCode,
								wDataWriteSize, pvDataWrite, 
								pwDataReadSize, pvDataRead );
			break;

		default :
			IrCa_Debug_Err(("VD_IO fail! [eDriverType=%d]\n",eDriverType));
			ErrCode = VD_NOT_SUPPORTED;

			break;
	}
	return ErrCode;
}





