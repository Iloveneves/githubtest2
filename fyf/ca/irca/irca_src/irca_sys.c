#include <basic_types.h>
#include <os/tds2/alitypes.h>
#include <api/libc/string.h>
#include <os/tds2/alitypes.h>
#include <irca/ca_task.h>
#include "irca_debug.h"



/*参考Embedded_CA_Task_API_for_DVB_Client.pdf中的
     4.6 CA_CLIENT__STOP_SERVICE_GROUP */
void IRCA_StopProg( void )
{
	BYTE Params[10];

	MEMSET( Params , 0 , 10  );

	/*msg_length ---16 uimsbf*/
	Params[1] = 8;
	/*group_handle ---32 uimsbf*/
	Params[5] = 0;
	/*emm_resources ---32 uimsbf*/
	Params[9] = CA_CLIENT_DESCRAMBLE_SERVICE_TYPE;
	
	CA_TASK_SendMessage( CA_CLIENT__STOP_SERVICE_GROUP, (void*)Params );

	IRCA_SetPMTVersion(0xff);
	//IRCA_MAIL_SetIconShow( FALSE );

	IRCA_STA_InitSCStatus_ForStopProg();
	
}

void IRCA_NewProg(BYTE* pPMTData)
{
	/*group_handle ---32 uimsbf*/
	pPMTData[ 5 ] = 0;
	/*services ---32 uimsbf*/
	pPMTData[ 9 ] = CA_CLIENT_DESCRAMBLE_SERVICE_TYPE;
	/*emm_handle ---32 uimsbf*/
	pPMTData[ 13 ] = 0;

	CA_TASK_SendMessage( CA_CLIENT__DEFINE_SERVICE_GROUP, (void*)pPMTData );

	/*换台时如果刚好显示指纹信息，应该清除指纹，并把指纹打开的图层关闭*/
	//IRCA_FIN_ClearHashID();

}


/*参考Embedded_CA_Task_API_for_DVB_Client.pdf中的
     4.4 CA_CLIENT__STOP_EMM_SERVICE*/
void IRCA_EndTs( void )
{
	BYTE Params[10];

	IRCA_STA_InitSCStatus_ForEndTs();
	
	MEMSET( Params , 0 , 10  );

	/*msg_length ---16 uimsbf*/
	Params[1] = 8;
	/*emm_handle ---32 uimsbf*/
	Params[5] = 0;
	/*emm_resources ---32 uimsbf*/
	Params[9] = CLIENT_DEFINE_EMM_SOURCE_CAT;
	
	CA_TASK_SendMessage( CA_CLIENT__STOP_EMM_SERVICE, (void*)Params);
	
	IRCA_SetCATVersion(0xff);
}

void IRCA_NewTs(BYTE* pCATData)
{
	/* emm_handle ---32 uimsbf */
	pCATData[ 5 ] = 0;
	/* emm_resources ---32 uimsbf */
	pCATData[ 9 ] = CLIENT_DEFINE_EMM_SOURCE_CAT;   

	CA_TASK_SendMessage( CA_CLIENT__DEFINE_EMM_SERVICE, (void*)pCATData );

}

void IRCA_GetNIT(BYTE* pData)
{
	CA_TASK_SendMessage( CA_CLIENT__CURRENT_TS_NIT_INFO, (void*)pData );
}


void IRCA_Notify( ia_word32 wHandle, CA_QUERY_CONTROL_EN eQuery, ia_uint32 ulParam )
{
	/*IrCa_Debug((" IRCA_Notify [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/
#if 1 
	switch (eQuery)
	{
		case CA_QC_SOFTCELL_INFO:/*softcell信息*/

			IRCA_STA_GetSoftcellnfo( wHandle, eQuery, ulParam );
			
			break;

		case CA_QC_SMARTCARD_INFO:/*智能卡信息*/

			IRCA_STA_GetSmartcardInfo( wHandle, eQuery, ulParam );
			
			break;
			
		case CA_QC_NOTIFY_CURRENT_ERROR:/*错误提示*/
			
			IRCA_BAN_GetErrorParam( wHandle, eQuery, ulParam );
			
			break;

		case CA_QC_HASH_ID:/*指纹显示*/
			
			IRCA_FIN_GetHashIDParam( wHandle, eQuery, ulParam );
			
			break;

		case CA_QC_ATTRIBUTED_DISPLAY_MESSAGE:

			IRCA_FIN_GetAttribMsgParam( wHandle, eQuery, ulParam );
			
			break;
			
		case CA_QC_SMARTCARD_NUMBER_OF_SECTORS :/*获取sector的数目、号和状态*/
		
			IRCA_STA_GetSectorParam( wHandle, eQuery, ulParam );
			
			break;
			
		case CA_QC_EXTENDED_PRODUCT_LIST: /*获取详细的sector产品信息*/
			
			IRCA_STA_GetProductParam( wHandle, eQuery, ulParam );
			
			break;
			
		case CA_QC_EMM_SERVICE_INFO: /*EMM info*/

			IRCA_STA_GetEmmServiceInfo( wHandle, eQuery, ulParam );
			
			break;
			
		case CA_QC_MONITOR_EMM: /*EMM monitoring*/

			IRCA_STA_GetMonitorEmm( wHandle, eQuery, ulParam );
			
			break;

		case CA_QC_SERVICE_GROUP_INFO: /*ECM info*/

			IRCA_STA_GetServiceGroupInfo( wHandle, eQuery, ulParam );
			
			break;


		case CA_QC_MONITOR_ECM: /*ECM monitoring*/
			
			IRCA_STA_GetMonitorEcm( wHandle, eQuery, ulParam );
			
			break;

		case CA_QC_MAIL:
		case CA_QC_ANNOUNCE:
			
			IRCA_MAIL_GetParam( wHandle, eQuery, ulParam );//邮件
			
			break;

#ifdef ENABLE_OTA_IR

		case CA_QC_EMM_DOWNLOAD:  /*软件升级*/
			
			//IRCA_DLD_GetParam( wHandle, eQuery, ulParam );
			
			break;
#endif

		case CA_QC_HGPC_SMARTCARD_DATA:

			IRCA_STA_GetHGPCSCData( wHandle, eQuery, ulParam );
			
			break;

		case CA_QC_HGPC_SOFTCELL_DATA:

			IRCA_STA_GetHGPCScellData( wHandle, eQuery, ulParam );

			break;

		case CA_QC_HGPC_MSG_STATUS:

			IRCA_STA_GetHGPCMsgData( wHandle, eQuery, ulParam );

			break;

		case CA_QC_HGPC_EVENT:

			//IRCA_STA_GetHGPCEventData( wHandle, eQuery, ulParam );

			break;
			
		case CA_QC_REGION:
			IRCA_STA_GetRegionInfo( wHandle, eQuery, ulParam );	
			
		default:
			
			break;
	}
	#endif


}
BOOL IRCA_Init( void )
{
	CA_INIT_CONFIG_ST stCaInitCfg ;
	
	IRCA_OS_Initialise();
	IRCA_STA_InitSCStatus();
	//IRCA_InitLanguage(0);
	
	stCaInitCfg.wLength = sizeof(CA_INIT_CONFIG_ST);
	stCaInitCfg.wNumGroups = 1;
	stCaInitCfg.eCryptoAlgorithm = VD_CRYPTO_ALGORITHM_3DES;
	
	CA_TASK_RegisterCaNotify(IRCA_Notify);

	//irdeto ca core init
	if (CA_TASK_Init( CA_DBG_NONE, (void*)(&stCaInitCfg)) != IA_TRUE)
	{
		IrCa_Debug_Err(("IRCA_Init CA_TASK_Init fail! \n"));
		return TRUE;
	}
	IRCA_STA_EnableQueryHGPCData();
	return FALSE;
}



