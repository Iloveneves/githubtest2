#include <basic_types.h>
#include <os/tds2/alitypes.h>
#include <api/libc/string.h>
#include <os/tds2/alitypes.h>
#include <osal/osal.h>
#include <irca/ca_task.h>
#include <irca/irca_sys.h>
#include "irca_debug.h"

#define CURR_MAX_SECTOR_NUM			(4) /*最大sector数目*/
#define MAX_PRODUCT_PER_SECTOR		(0) /*每个sector的最大product数目*/
#define MAX_SEC_CARD_NUM			(4) /*最大副卡数目*/


static CA_SOFTCELL_INFO_T stSoftcellInfo;
static CA_SOFTCELL_INFO_T stSoftcellInfo;
static CA_SMARTCARD_INFO_T stCAInfo;
static CA_ERROR_INFO_T stErrorInfo;
static CA_REGION_T stRegionInfo;

/*EMM*/
static CA_EMM_SERVICE_INFO_T stEMMInfo;
static CA_EMM_MONITOR_T stEmmMonitor;
/*ECM*/
static SERVICE_GROUP_INFO_T stECMInfo;
static CA_ECM_MONITOR_T stEcmMonitor;
static CA_SC_SECTORLIST_T stCaSectorlist;
static CA_EXTENDED_PRODUCT_LIST_T stCaExProductList;
static CA_ECM_STRING  ECMString;

static CA_HGPC_SCELL_DATA_T stHgpcScell;
static CA_HGPC_SEC_CARD_T SecCard[MAX_SEC_CARD_NUM]; //for stHgpcSCData
static CA_HGPC_SMARTCARD_DATA_T stHgpcSCData;
static CA_HGPC_MSG_STATUS_T stHgpcMsgData;
static BOOL bCardInOrOutTag = FALSE; //快速记录卡拔出或者插入的状态
static BOOL bGetHgpcSCData = FALSE;
static BOOL bNeedUpdate = FALSE;
static int iHgpcEventHNR = 0xff; 		//记录Event的HNR状态
static int iHgpcEventAvtivated = 0xff;	//记录Event的Avtivated状态

BYTE FingerPrint[MAX_SC_SER_NO_LEN];
FP_SHOW_INFO  FPShow;


void IRCA_STA_InitSCStatus(void)
{
	int i;
	
	/*初始化stSoftcellInfo*/
	MEMSET(&stSoftcellInfo,0,sizeof(CA_SOFTCELL_INFO_T));

	/*初始化stCAInfo*/
	MEMSET(&stCAInfo,0,sizeof(CA_SMARTCARD_INFO_T));
	stCAInfo.bRequestType = REQUEST_TYPE_REPLY;
	stCAInfo.wResourceId = TP_SmartCard_GetRscId();
	stCAInfo.wStatus = STATUS_CARD_OUT;

	/*初始化stEMMInfo*/
	MEMSET(&stEMMInfo,0,sizeof(CA_EMM_SERVICE_INFO_T));
	stEMMInfo.fEmmServiceDefine = IA_FALSE;
	stEMMInfo.wEmmPid = 0x1fff;

	/*初始化stEMMMonitor*/
	MEMSET(&stEmmMonitor,0,sizeof(CA_EMM_MONITOR_T));

	/*初始化stECMInfo*/
	MEMSET(&stECMInfo,0,sizeof(SERVICE_GROUP_INFO_T));

	/*初始化stEcmMonitor*/
	MEMSET(&stEcmMonitor,0,sizeof(CA_ECM_MONITOR_T));
	MEMSET(ECMString.EcmString1,0,MAX_ECM_MONITOR_LEN);
	MEMSET(ECMString.EcmString2,0,MAX_ECM_MONITOR_LEN);

	/*初始化stCaSectorlist,并为其结构指针申请空间*/
	MEMSET(&stCaSectorlist,0,sizeof(CA_SC_SECTORLIST_T));
	stCaSectorlist.pstSectors = (CA_SC_SECTOR_T*) MALLOC(CURR_MAX_SECTOR_NUM*sizeof(CA_SC_SECTOR_T));
		
	if (stCaSectorlist.pstSectors == NULL)
	{
		IrCa_Debug_Err(("No enough mem for pstSectors!!!!\n"));
	}
		
	/*初始化stCaExProductList,并为其结构指针申请空间*/
	MEMSET(&stCaExProductList,0,sizeof(CA_EXTENDED_PRODUCT_LIST_T));
	stCaExProductList.pstSectorProducts = 
		(CA_SECTOR_EXTEND_SECTOR_T*)MALLOC(CURR_MAX_SECTOR_NUM*sizeof(CA_SECTOR_EXTEND_SECTOR_T) );
	
	if (stCaExProductList.pstSectorProducts == NULL)
	{
		IrCa_Debug_Err(("no enough mem for pstSectorProducts!!\n"));
	}
	/*
	for (i=0;i<CURR_MAX_SECTOR_NUM;i++)
	{
		stCaExProductList.pstSectorProducts[i].pstExtendProducts = 
			(CA_SECTOR_EXTEND_PRODUCT_T*)MALLOC(CURR_MAX_SECTOR_NUM*sizeof(CA_SECTOR_EXTEND_SECTOR_T) );
				
		if( stCaExProductList.pstSectorProducts[i].pstExtendProducts == NULL )
		{
			IrCa_Debug_Err(("no enough mem for pstExtendProducts!!\n"));
			break;
		}
	}	*/
	
#if 1
	/*初始化stHgpcScell*/
	MEMSET(&stHgpcScell,0,sizeof(CA_HGPC_SCELL_DATA_T));
	
	/*初始化stHgpcSCData*/
	MEMSET(&stHgpcSCData,0,sizeof(CA_HGPC_SMARTCARD_DATA_T));
	stHgpcSCData.stPrimaryData.stSecCard = SecCard;
	
	/*初始化stHgpcMsgData*/
	MEMSET(&stHgpcMsgData,0,sizeof(CA_HGPC_MSG_STATUS_T));
#endif	

	FPShow.pFPStrint = FingerPrint;
	FPShow.ShowTime =0;

}


void IRCA_STA_InitSCStatus_ForCardIn(void)
{
	bCardInOrOutTag = TRUE;
	bNeedUpdate = TRUE;
}

void IRCA_STA_InitSCStatus_ForCardOut(void)
{
	MEMSET(&stCAInfo,0,sizeof(CA_SMARTCARD_INFO_T));
	stCAInfo.wStatus = STATUS_CARD_OUT;
		
	MEMSET(stEcmMonitor.acEcmString,0,MAX_ECM_MONITOR_LEN);
	MEMSET(ECMString.EcmString1,0,MAX_ECM_MONITOR_LEN);
	MEMSET(ECMString.EcmString2,0,MAX_ECM_MONITOR_LEN);
	MEMSET(stEmmMonitor.acEmmString,0,MAX_EMM_MONITOR_LEN);

	bCardInOrOutTag = FALSE;
	bNeedUpdate = TRUE;
	iHgpcEventHNR = 0xff;
	
}

void IRCA_STA_InitSCStatus_ForStopProg(void)
{
	/*初始化stECMInfo*/
	MEMSET(&stECMInfo,0,sizeof(SERVICE_GROUP_INFO_T));

	/*初始化stEcmMonitor*/
	MEMSET(&stEcmMonitor,0,sizeof(CA_ECM_MONITOR_T));
	MEMSET(ECMString.EcmString1,0,MAX_ECM_MONITOR_LEN);
	MEMSET(ECMString.EcmString2,0,MAX_ECM_MONITOR_LEN);

}

void IRCA_STA_InitSCStatus_ForEndTs(void)
{
	/*初始化stEMMInfo*/
	MEMSET(&stEMMInfo,0,sizeof(CA_EMM_SERVICE_INFO_T));
	stEMMInfo.fEmmServiceDefine = IA_FALSE;
	stEMMInfo.wEmmPid = 0x1fff;

	/*初始化stEMMMonitor*/
	MEMSET(&stEmmMonitor,0,sizeof(CA_EMM_MONITOR_T));
}

void IRCA_STA_EnableQueryHGPCData(void) 
{
	ia_word32 wEnableMask = 0;

	/*允许申请HGPC消息*/
	wEnableMask |= NOTIFY_MASK_HGPC_SMARTCARD_DATA;
	wEnableMask |= NOTIFY_MASK_HGPC_SOFTCELL_DATA;
	wEnableMask |= NOTIFY_MASK_HGPC_MSG_STATUS;
	wEnableMask |= NOTIFY_MASK_HGPC_EVENT;
	CA_TASK_QueryControl( 0, CA_QC_NOTIFY_ENABLE, (void*)&wEnableMask );
}

void IRCA_STA_GetSoftcellnfo( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	/*IrCa_Debug((" IRCA_STA_GetSoftcellnfo [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

	if ( CA_TASK_QueryControl(wHandle, eQuery, (void *)&stSoftcellInfo) 
			!= CA_QUERY_SUCCESS)
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_SOFTCELL_INFO ] \n"));
		return;
	}

	ap_cas_call_back(CA_STATUS_E);

	IrCa_Debug((" CA_QC_SOFTCELL_INFO \n\t[ bMajor = %d ] \n\t[ bMinor = %d ] \n\t[ uSubVersion = %d ] \n\t[ acNameString = %s ] \n\t[ acBuildInfoString = %s ] \n\t[ acCaTaskVerString = %s ]\n",
			stSoftcellInfo.bMajor,stSoftcellInfo.bMinor,stSoftcellInfo.uSubVersion,
			stSoftcellInfo.acNameString,
			stSoftcellInfo.acBuildInfoString,
			stSoftcellInfo.acCaTaskVerString));/**/

}


/*
请参考Embedded_CA_Task_API_for_DVB_Client.pdf 的 5.5 CA_QC_SMARTCARD_INFO
*/
void IRCA_STA_GetSmartcardInfo( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	IrCa_Debug((" IRCA_STA_GetSmartcardInfo [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));/**/

	stCAInfo.bRequestType = REQUEST_TYPE_REPLY;
	stCAInfo.wResourceId = TP_SmartCard_GetRscId();

	if ( CA_TASK_QueryControl(0,eQuery,(void*)&stCAInfo) != CA_QUERY_SUCCESS)
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_SMARTCARD_INFO ] \n"));
		return;
	}
	ap_cas_call_back(CA_STATUS_E);

	IrCa_Debug((" ======CA_QC_SMARTCARD_INFO====== \n"));
	IrCa_Debug(("[ bRequestType = %d ] \n",stCAInfo.bRequestType));
	IrCa_Debug(("[ wResourceId = %d ] \n",stCAInfo.wResourceId));
	IrCa_Debug(("[ wStatus = %d ] \n",stCAInfo.wStatus));
	IrCa_Debug(("stSmartCardStatus [ eSource = %d ] [ wStatus = %d ] [ eSeverity = %d ]\n",
		stCAInfo.stSmartCardStatus.eSource,
		stCAInfo.stSmartCardStatus.wStatus,
		stCAInfo.stSmartCardStatus.eSeverity));
	IrCa_Debug(("[ wType = %d ] \n",stCAInfo.wType));
	IrCa_Debug(("[ bMajorVersion = %d ] [ bMinorVersion = %d ]\n",
		stCAInfo.bMajorVersion,stCAInfo.bMinorVersion));
	IrCa_Debug(("[ acSerialNo = %s ] \n",stCAInfo.acSerialNo));
	IrCa_Debug(("[ wPatchLevel = %04x ] \n",stCAInfo.wPatchLevel));
	IrCa_Debug(("[ wOwnerId = %04x ] \n",stCAInfo.wOwnerId));
	IrCa_Debug(("[ bVariant = %02d ] \n",stCAInfo.bVariant));
	IrCa_Debug(("[ bBuild = %02d ] \n",stCAInfo.bBuild));
	IrCa_Debug((" ================================ \n"));/**/
	
}

void IRCA_STA_GetRegionInfo( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	stRegionInfo.bRequestType = REQUEST_TYPE_REPLY;
	stRegionInfo.wResourceId = TP_SmartCard_GetRscId();
	stRegionInfo.bSector=1;
	
	if(CA_TASK_QueryControl( 0,eQuery,(void*)(&stRegionInfo))!= CA_QUERY_SUCCESS)
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_REGION ] \n"));
		return;
	}
	IrCa_Debug((" ======CA_QC_REGION====== \n"));

	IrCa_Debug(("sector num=%d region = 0x%x , subregion=0x%x\n" , stRegionInfo.bSector , stRegionInfo.bRegion ,stRegionInfo.bSubRegion));
	
	ap_cas_call_back(CA_STATUS_E);
}


/*请参看Embedded_CA_Task_API_for_DVB_Client.pdf 的
  5.33 CA_QC_SMARTCARD_NUMBER_OF_SECTORS*/
void IRCA_STA_GetSectorParam( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	INT	i = 0;

	/*IrCa_Debug((" IRCA_STA_GetSectorParam [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

	if (ulParam <= 0 || ulParam > CURR_MAX_SECTOR_NUM)
	{
		IrCa_Debug_Err(("no sector or sector num too large! ulParam[%d]\n",ulParam));
		return;
	}
	
	if (stCaSectorlist.pstSectors == NULL)
	{
		IrCa_Debug_Err(("No enough mem for pstSectors!!!!\n"));
		return;
	}
	
	stCaSectorlist.wResourceId = TP_SmartCard_GetRscId();
	stCaSectorlist.bSectorCount = ulParam;
	if( CA_TASK_QueryControl( 0,eQuery, (void*)(&stCaSectorlist) ) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_SMARTCARD_NUMBER_OF_SECTORS ] \n"));
		return;
	}

	/*for(i=0;i<stCaSectorlist.bSectorCount;i++)
	{
		IrCa_Debug(("stCaSectorlist.pstSectors[%d]->bSector=%d,bActive=%d\n",
			i,stCaSectorlist.pstSectors[i].bSector,
			stCaSectorlist.pstSectors[i].bActive));
		
	}*/
}




void IRCA_BAN_GetErrorParam( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	stErrorInfo.wGroupHandle = wHandle;//Irdeto文档对此参数描述错误
	stErrorInfo.wServiceType = ulParam&0xFFFF;

	if( CA_TASK_QueryControl( 0,eQuery,(void *)&stErrorInfo ) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_NOTIFY_CURRENT_ERROR ] \n"));
		return;
	}
	IrCa_Debug(( "GetErrorParam  eSource=%d,wStatus=%d,eSeverity=%d\n",
			stErrorInfo.stGlobalStatus.eSource,
			stErrorInfo.stGlobalStatus.wStatus,
			stErrorInfo.stGlobalStatus.eSeverity));

	ap_cas_call_back(CA_BANNER_E);
	
}


/*参考 Embedded_CA_Task_API_for_DVB_Client.pdf 的 5.7 CA_QC_SERVICE_GROUP_INFO*/
void IRCA_STA_GetServiceGroupInfo( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	INT i;
	
	/*IrCa_Debug((" IRCA_STA_GetServiceGroupInfo [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

	stECMInfo.wGroupHandle = wHandle;
	stECMInfo.wServiceType = SERVICE_DVB_DESCRAMBLE;
	if( CA_TASK_QueryControl( 0,eQuery,(void*)&stECMInfo ) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_SERVICE_GROUP_INFO ] \n"));
		return;
	}
	ap_cas_call_back(CA_ECMSVC_E);
	
	/*IrCa_Debug((" CA_QC_SERVICE_GROUP_INFO \n\t[ wGroupHandle = %d ] \n\t[ wServiceType = %d ] \n\t[ wServiceHandle = %d ] \n\t[ stGlobalStatus eSource = %d wStatus = %d eSeverity = %d] \n\t[ stServiceStatus eSource = %d wStatus = %d eSeverity = %d ] \n\t[ bStreamCount = %d ] \n\t[ bRscCount = %d ]\n",
		stECMInfo.wGroupHandle,stECMInfo.wServiceType,stECMInfo.wServiceHandle,
		stECMInfo.stGlobalStatus.eSource,stECMInfo.stGlobalStatus.wStatus,stECMInfo.stGlobalStatus.eSeverity,
		stECMInfo.stServiceStatus.eSource,stECMInfo.stServiceStatus.wStatus,stECMInfo.stServiceStatus.eSeverity,
		stECMInfo.bStreamCount,stECMInfo.bRscCount));
	
	for (i=0;i<stECMInfo.bStreamCount;i++)
	{
		IrCa_Debug((" \t\t\t astStreamInfo[%d] \n\t[ wStreamPid = 0x%x ] \n\t[ wEcmPid = 0x%x ] \n\t[ stStatus eSource = %d wStatus = %d eSeverity = %d] \n",
			i,stECMInfo.astStreamInfo[i].wStreamPid,stECMInfo.astStreamInfo[i].wEcmPid,
			stECMInfo.astStreamInfo[i].stStatus.eSource,
			stECMInfo.astStreamInfo[i].stStatus.wStatus,
			stECMInfo.astStreamInfo[i].stStatus.eSeverity));
	}
	for (i=0;i<stECMInfo.bRscCount;i++)
	{
		IrCa_Debug((" \t\t\t [ awRscType[%d] = %d ] \n\t[ astRscStatus eSource = %d wStatus = %d eSeverity = %d] \n",
			i,stECMInfo.awRscType[i],
			stECMInfo.astRscStatus[i].eSource,
			stECMInfo.astRscStatus[i].wStatus,
			stECMInfo.astRscStatus[i].eSeverity));
	}*/

}

void IRCA_STA_GetEmmServiceInfo( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	/*IrCa_Debug((" IRCA_STA_GetEmmServiceInfo [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

	stEMMInfo.wEmmHandle = ulParam;
	if( CA_TASK_QueryControl( 0,eQuery,(void*)&stEMMInfo ) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_EMM_SERVICE_INFO ] \n"));
		return;
	}
	ap_cas_call_back(CA_EMMSVC_E);
	
	/*IrCa_Debug((" CA_QC_EMM_SERVICE_INFO \n\t[ wEmmHandle = %d ] \n\t[ wServiceHandle = %d ] \n\t[ stGlobalStatus eSource = %d wStatus = %d eSeverity = %d] \n\t[ stServiceStatus eSource = %d wStatus = %d eSeverity = %d ] \n\t[ wEmmPid = 0x%x ] \n\t[ fEmmServiceDefine = %d ] \n\t[ bRscCount = %d ] \n\t[ awRscType[0] = %d ] \n\t[ astRscStatus[0] eSource = %d wStatus = %d eSeverity = %d ]\n",
		stEMMInfo.wEmmHandle,stEMMInfo.wServiceHandle,
		stEMMInfo.stGlobalStatus.eSource,stEMMInfo.stGlobalStatus.wStatus,stEMMInfo.stGlobalStatus.eSeverity,
		stEMMInfo.stServiceStatus.eSource,stEMMInfo.stServiceStatus.wStatus,stEMMInfo.stServiceStatus.eSeverity,
		stEMMInfo.wEmmPid,stEMMInfo.fEmmServiceDefine,stEMMInfo.bRscCount,stEMMInfo.awRscType[0],
		stEMMInfo.astRscStatus[0].eSource,stEMMInfo.astRscStatus[0].wStatus,stEMMInfo.astRscStatus[0].eSeverity));
	*/
}


void IRCA_STA_GetMonitorEmm( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	/*IrCa_Debug((" IRCA_STA_GetMonitorEmm [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

	stEmmMonitor.wEmmHandle = ulParam;
	if( CA_TASK_QueryControl( 0,eQuery,(void*)&stEmmMonitor) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_MONITOR_EMM ] \n"));
		return;
	}
	ap_cas_call_back(CA_EMMSVC_E);
				
	/*IrCa_Debug((" CA_QC_MONITOR_EMM \n\t[ wEmmHandle = %d ] \n\t[ wServiceHandle = %d ] \n\t[ acEmmString = %s]\n",
		stEmmMonitor.wEmmHandle,stEmmMonitor.wServiceHandle,
		stEmmMonitor.acEmmString));*/
}


/*参考 Embedded_CA_Task_API_for_DVB_Client.pdf 的 5.11 CA_QC_MONITOR_ECM*/
void IRCA_STA_GetMonitorEcm( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{	
	/*IrCa_Debug((" IRCA_STA_GetMonitorEcm [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

	stEcmMonitor.wGroupHandle = ulParam;
	if( CA_TASK_QueryControl( 0,eQuery,(void*)&stEcmMonitor) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_MONITOR_ECM ] \n"));
		return;
	}

	/*EcmString1未存放过数据，等于第一次进来*/
	if ( ECMString.EcmString1[0] == 0 )
	{
		MEMCPY(ECMString.EcmString1,stEcmMonitor.acEcmString,MAX_ECM_MONITOR_LEN);
	}
	/*EcmString1存放过数据，而且与当前获取到的ECM PID一致*/
	else if (ECMString.EcmString1[6] == stEcmMonitor.acEcmString[6] && ECMString.EcmString1[7] == stEcmMonitor.acEcmString[7]
		&& ECMString.EcmString1[8] == stEcmMonitor.acEcmString[8] && ECMString.EcmString1[9] == stEcmMonitor.acEcmString[9])
	{
		MEMCPY(ECMString.EcmString1,stEcmMonitor.acEcmString,MAX_ECM_MONITOR_LEN);
	}
	/*EcmString1存放过数据，而且与当前获取到的ECM PID不一致*/
	else
	{
		MEMCPY(ECMString.EcmString2,stEcmMonitor.acEcmString,MAX_ECM_MONITOR_LEN);
	}
	ap_cas_call_back(CA_ECMSVC_E);

}


void IRCA_STA_GetHGPCScellData( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	/*IrCa_Debug((" IRCA_STA_GetHGPCScellData [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

	stHgpcScell.bRequestType = REQUEST_TYPE_REPLY;
	if ( CA_TASK_QueryControl(wHandle, eQuery, (void *)&stHgpcScell) 
			!= CA_QUERY_SUCCESS)
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_HGPC_SOFTCELL_DATA ] \n"));
		return;
	}

	/*IrCa_Debug((" CA_QC_HGPC_SOFTCELL_DATA \n\t[ eScellRole = %d ] \n\t[ stPrimCardId.bSerialNoLength = %d ] \n\t[ stPrimCardId.bCheckDigit = %d ] \n\t[ stPrimCardId.abUniqueAddr[0] = %d ] \n\t[ stPrimCardId.abUniqueAddr[1] = %d ] \n\t[ stPrimCardId.abUniqueAddr[2] = %d ] \n\t[ abPrimCardSerialNo = %s ] \n",
					stHgpcScell.eScellRole,
					stHgpcScell.stScellData.stPrimCardId.bSerialNoLength,
					stHgpcScell.stScellData.stPrimCardId.bCheckDigit,
					stHgpcScell.stScellData.stPrimCardId.abUniqueAddr[0],
					stHgpcScell.stScellData.stPrimCardId.abUniqueAddr[1],
					stHgpcScell.stScellData.stPrimCardId.abUniqueAddr[2],
					stHgpcScell.abPrimCardSerialNo));*/

	ap_cas_call_back(CA_HGPC_E);

	
}


void IRCA_STA_GetHGPCSCData( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	/*IrCa_Debug((" IRCA_STA_GetHGPCSCData [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

	stHgpcSCData.bRequestType = REQUEST_TYPE_REPLY;
	if ( CA_TASK_QueryControl(wHandle, eQuery, (void *)&stHgpcSCData) 
			!= CA_QUERY_SUCCESS)
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_HGPC_SMARTCARD_DATA ] \n"));
		return;
	}

	/*IrCa_Debug((" CA_QC_HGPC_SMARTCARD_DATA \n\t[ eCardStatus = %d ] \n\t[ eCardRole = %d ] \n",
			stHgpcSCData.eCardStatus,
			stHgpcSCData.eCardRole));
	*/

	ap_cas_call_back(CA_HGPC_E);

}



void IRCA_STA_GetHGPCMsgData( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	void *pst1;
	void *pst2;

	/**/IrCa_Debug((" IRCA_STA_GetHGPCMsgData [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));

	if ( CA_TASK_QueryControl(wHandle, eQuery, (void *)&stHgpcMsgData) 
			!= CA_QUERY_SUCCESS)
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_HGPC_MSG_STATUS ] \n"));
		return;
	}

	IrCa_Debug((" CA_QC_HGPC_MSG_STATUS \n\t[ wResourceId = %d ] \n\t[ eType = %d ] \n\t[ eAction = %d ] \n\t[ eStatus = %d ] \n",
			stHgpcMsgData.wResourceId,
			stHgpcMsgData.eType,
			stHgpcMsgData.eAction,
			stHgpcMsgData.eStatus));
	/*printf(" CA_QC_HGPC_MSG_STATUS \n\t[ wResourceId = %d ] \n\t[ eType = %d ] \n\t[ eAction = %d ] \n\t[ eStatus = %d ] \n",
			stHgpcMsgData.wResourceId,
			stHgpcMsgData.eType,
			stHgpcMsgData.eAction,
			stHgpcMsgData.eStatus);*/


	IRCA_STA_EnableCAInfo(&pst1, &pst2, CA_HGPC_E);

	
#if 0
	IRCA_STA_QueryHGPCData();

	//子卡在主机上充值成功后，在空白的屏幕上弹出提示框
	if (TPUSIF_GetScreenUseState() == FALSE
		&& bRechargeOK == FALSE
		&& stHgpcMsgData.eType == HGPC_MSG_TYPE_HNR 
		&& stHgpcMsgData.eAction == HGPC_MSG_ACTION_WRITE_TO_CARD
 		&& (stHgpcMsgData.eStatus == HGPC_MSG_STATUS_OK || stHgpcMsgData.eStatus == HGPC_MSG_STATUS_CARD_UNSUPPORT))
	{
		bRechargeOK = TRUE;
		TPUSIF_SetScreenUseState(TRUE);
		TPDRAW_DisplayMessage( acInfomationStr,"",acHGPCStatusStr[5],"",FALSE );
		MILLI_DELAY(3000);
		TPDRAW_ClearMessageArea(1);
		TPUSIF_SetScreenUseState(FALSE);
	}
#endif	

}
void IRCA_STA_GetHGPCEventData( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	void *pst1;
	void *pst2;
	
	IrCa_Debug((" IRCA_STA_GetHGPCEventData [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));
	/*printf(" IRCA_STA_GetHGPCEventData [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam);*/

	if (ulParam == HGPC_EVENT_HNR_REQUIRED)
	{
		iHgpcEventHNR = HGPC_EVENT_HNR_REQUIRED;
	}
	else if (ulParam == HGPC_EVENT_ACTIVATION)
	{
		iHgpcEventAvtivated = HGPC_EVENT_ACTIVATION;
	}

	IRCA_STA_EnableCAInfo(&pst1, &pst2, CA_HGPC_E);
}


/*请参看Embedded_CA_Task_API_for_DVB_Client.pdf 的
  5.34 CA_QC_EXTENDED_PRODUCT_LIST*/
void IRCA_STA_GetProductParam( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	int i,j = 0;

	/*IrCa_Debug(("IRCA_STA_GetProductParam [ wHandle = %d ] [ eQuery = %d ] [ ulParam = %d ] \n",
		wHandle,eQuery,ulParam));*/

//	bSectorFound = FALSE;
	
	if (stCaExProductList.pstSectorProducts == NULL)
	{
		IrCa_Debug_Err(( "stCaExProductList.pstSectorProducts == NULL!\n" ));
		return;
	}
	
	stCaExProductList.bRequestType = REQUEST_TYPE_REPLY;
	if( CA_TASK_QueryControl( 0,eQuery,(void*)&stCaExProductList ) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(( "Call CA_TASK_QueryControl fail in IRCA_STA_GetProductParam!\n" ));
		return;
	}

	for (i=0;i<CURR_MAX_SECTOR_NUM;i++)
	{
		if (stCaExProductList.pstSectorProducts[i].bSectorStatus == IA_TRUE
				&& stCaExProductList.pstSectorProducts[i].bProductCount > 0
				/*&& stCaExProductList.pstSectorProducts[i].bProductCount <= MAX_PRODUCT_PER_SECTOR
				&& stCaSectorlist.bSectorCount != 0*/)
		{
			//bSectorFound = TRUE;
			ap_cas_call_back(CA_PRODUCT_E);
			break;
		}
	}
}


void IRCA_MAIL_GetParam( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )
{
	CA_MAIL_T            stMail;

	stMail.bIndex = ( BYTE) ulParam;
	if(CA_TASK_QueryControl( 0 ,eQuery, (void*)&stMail ) != CA_QUERY_SUCCESS)
	{
		IrCa_Debug_Err(( "Call CA_TASK_QueryControl fail in IRCA_MAIL_GetParam!\n" ));
		return;
	}
	if( stMail.bClass  == CA_MAIL_FORCED )
	{
		ap_cas_call_back(CA_FEMAIL_E);
	}
	
	IRCA_MAIL_SaveNew(&stMail);
	
	IrCa_Debug(("============= CA_QC_MAIL =============\n"));
	IrCa_Debug(("bMsgLength[%d]\n",stMail.bMsgLength));
	IrCa_Debug(("bMessageData[%s]\n",stMail.bMessageData));
	IrCa_Debug(("bType[%d]\n",stMail.bType));
	IrCa_Debug(("bClass[%d]\n",stMail.bClass));
	IrCa_Debug(("bPriority[%d]\n",stMail.bPriority));
	IrCa_Debug(("fTimedDelete[%d]\n",stMail.fTimedDelete));
	IrCa_Debug(("======================================\n"));/**/

}

void IRCA_FIN_GetHashIDParam( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )

{
	CA_HASH_ID_T	stHashId;

	MEMSET( FingerPrint,0,MAX_SC_SER_NO_LEN );
	stHashId.wClientHandle = ulParam;
	stHashId.wResourceId = TP_SmartCard_GetRscId();
	stHashId.pbHashedNo = FingerPrint;
	stHashId.bHashedNoLen = MAX_SC_SER_NO_LEN;

	if( CA_TASK_QueryControl( 0,eQuery,(void*)&stHashId ) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_HASH_ID ] \n"));
		return;
	}
	
	IrCa_Debug(("CA_QC_HASH_ID \t\t[ wClientHandle = %d ] \n\t\t[ wResourceId = %d ] \n\t\t[ pbHashedNo = %s ] \n\t\t[ bHashedNoLen = %d ] \n",
			stHashId.wClientHandle,stHashId.wResourceId,
			FingerPrint,stHashId.bHashedNoLen));/**/

	FPShow.ShowTime = 10;

	ap_cas_call_back(CA_HASHID_E);
	
}


/*请参考705410_1[1].26_CCP_IRD_Messages.pdf 
 6 DESTINATION ID 0X04 – ATTRIBUTED DISPLAY*/
void IRCA_FIN_GetAttribMsgParam( ia_word32 wHandle, 
							CA_QUERY_CONTROL_EN eQuery, 
							ia_uint32 ulParam )

{
	CA_MAIL_T  stMail;
	CA_ATTRIB_T	stAttrib;

	
	MEMSET( &stAttrib,0,sizeof(CA_ATTRIB_T));
	
	stAttrib.bIndex = ulParam;
	stAttrib.wEmmHandle = wHandle;
	stAttrib.wMsgLength = MAX_EPG_MSG_LENGTH;

	if( CA_TASK_QueryControl( 0,eQuery,(void*)&stAttrib ) != CA_QUERY_SUCCESS )
	{
		IrCa_Debug_Err(("CA_TASK_QueryControl fail!! [ CA_QC_ATTRIBUTED_DISPLAY_MESSAGE ] \n"));
		return;
	}
	
	IrCa_Debug(("CA_QC_ATTRIBUTED_DISPLAY_MESSAGE \n\t\t[ bIndex = %d ] \n\t\t[ wEmmHandle = %d ] \n\t\t[ wServiceHandle = %d ] \n\t\t[ wMsgLength = %d ] \n\t\t[ bType=%d ] \n\t\t[ bDisplayMethod=%d ] \n\t\t[ wDuration=%d ]\n",
			stAttrib.bIndex,stAttrib.wEmmHandle,stAttrib.wServiceHandle,
			stAttrib.wMsgLength,stAttrib.bType,
			stAttrib.bDisplayMethod,stAttrib.wDuration));/**/
#if 0
	if (stAttrib.bType != 0)
	{
		/* 0-20   The background will cover 20% of the screen.
	       21-62  The background will cover x% of the screen,
	              with 21 = 21% and 62 = 99%.
	       63     The background will cover 100% of the screen*/
		if ((((stAttrib.bDisplayMethod) & 0xfc)>>2) <= 20)
		{
			INFO_MENU_WIDTH = 350 ;
			INFO_MENU_HEIGHT = 200;
		}
		else if ((((stAttrib.bDisplayMethod) & 0xfc)>>2) < 62)
		{
			INFO_MENU_WIDTH = 550;
			INFO_MENU_HEIGHT = 400;
		}
		else
		{
			INFO_MENU_WIDTH = 650;
			INFO_MENU_HEIGHT = 500;
		}
		/*Flashing (0) or Not-Flashing (1)*/
		if (((stAttrib.bDisplayMethod) & 0x01) == 0)
		{
			bFlashing = TRUE;
		}
		else
		{
			bFlashing = FALSE;
		}
		/*Banner (0) or Normal (1)*/
		bTitleBar = (BOOLEAN)(((stAttrib.bDisplayMethod) & 0x02)>>1);
	}
#endif 	
	if (stAttrib.bType == 0 || stAttrib.bType == 1) /*Normal Text*/
	{
		if(stAttrib.bType == 0)
		{
			stMail.bClass = CA_MAIL_NORMAL;
		}
		else
		{
			stMail.bClass = CA_MAIL_FORCED;			
		}
		memcpy ( stMail.bMessageData, (char*)stAttrib.bMessageData,MAX_EPG_MSG_LENGTH);

		IRCA_MAIL_SaveNew( &stMail );
		if( stMail.bClass  == CA_MAIL_FORCED )
		{
			ap_cas_call_back(CA_FEMAIL_E);
		}
	}
	else if (stAttrib.bType == 2) /*Fingerprint*/
	{
		MEMCPY(FingerPrint, stAttrib.bMessageData, MAX_SC_SER_NO_LEN-1);
		FingerPrint[MAX_SC_SER_NO_LEN-1] = 0;
		FPShow.ShowTime = stAttrib.wDuration;
	}
}


void IRCA_STA_EnableCAInfo( void **pInfoStr1, void **pInfoStr2, CA_INFO_EVENT_E EventType)
{
	ia_word32 wEn = 0;

	switch(EventType)
	{
		case CA_STATUS_E:

				wEn |= NOTIFY_MASK_SMARTCARD_INFO;
				CA_TASK_QueryControl( 0, CA_QC_NOTIFY_ENABLE, (void*)&wEn );

				stCAInfo.bRequestType = REQUEST_TYPE_QUERY;
				stCAInfo.wResourceId = TP_SmartCard_GetRscId();
				CA_TASK_QueryControl( 0,CA_QC_SMARTCARD_INFO, (void*)&stCAInfo );

				*pInfoStr1 = (void *)&stSoftcellInfo;
				*pInfoStr2 = (void *)&stCAInfo;
				
				stRegionInfo.bRequestType = REQUEST_TYPE_QUERY;
				stRegionInfo.wResourceId = TP_SmartCard_GetRscId();
				stRegionInfo.bSector = 1;
				CA_TASK_QueryControl( 0,CA_QC_REGION,(void*)(&stRegionInfo) );
			
			break;
			
		case CA_EMMSVC_E:
			
				wEn |= NOTIFY_MASK_EMM_SVC_INFO;
				wEn |= NOTIFY_MASK_MONITOR_EMM;
				CA_TASK_QueryControl( 0, CA_QC_NOTIFY_ENABLE, (void*)&wEn );

				*pInfoStr1 = (void *)&stEMMInfo;
				*pInfoStr2 = (void *)&stEmmMonitor;
			break;
			
		case CA_ECMSVC_E:
		
				wEn |= NOTIFY_MASK_DES_SVC_INFO;
				wEn |= NOTIFY_MASK_MONITOR_ECM;
				CA_TASK_QueryControl( 0, CA_QC_NOTIFY_ENABLE, (void*)&wEn );

				*pInfoStr1 = (void *)&stECMInfo;
				*pInfoStr2 = (void *)&ECMString;
			break;	
			
		case CA_HGPC_E:

			wEn |= NOTIFY_MASK_HGPC_SMARTCARD_DATA;
			wEn |= NOTIFY_MASK_HGPC_SOFTCELL_DATA;
			wEn |= NOTIFY_MASK_HGPC_MSG_STATUS;
			wEn |= NOTIFY_MASK_HGPC_EVENT;
			CA_TASK_QueryControl( 0, CA_QC_NOTIFY_ENABLE, (void*)&wEn );

			stHgpcScell.bRequestType = REQUEST_TYPE_QUERY;
			CA_TASK_QueryControl( 0,CA_QC_HGPC_SOFTCELL_DATA, (void*)&stHgpcScell );
			
			stHgpcSCData.bRequestType = REQUEST_TYPE_QUERY;
			stHgpcSCData.wResourceId = TP_SmartCard_GetRscId();
			CA_TASK_QueryControl( 0,CA_QC_HGPC_SMARTCARD_DATA, (void*)&stHgpcSCData );

			*pInfoStr1 = (void *)&stHgpcScell;
			*pInfoStr2 = (void *)&stHgpcSCData;
			
			break;

		case CA_PRODUCT_E:
		{
			UINT16 i ;

			stCaExProductList.bRequestType = REQUEST_TYPE_QUERY;
			stCaExProductList.wResourceId = TP_SmartCard_GetRscId();
			stCaExProductList.bSectorCount = CURR_MAX_SECTOR_NUM;/*stCaSectorlist.bSectorCount;*/
			stCaExProductList.wMaxSizeSectorProducts = CURR_MAX_SECTOR_NUM;/*stCaSectorlist.bSectorCount;*/
			for (i=0;i<CURR_MAX_SECTOR_NUM/*stCaSectorlist.bSectorCount*/;i++)
			{
				stCaExProductList.pstSectorProducts[i].bSectorStatus = 0;
				stCaExProductList.pstSectorProducts[i].bProductCount = 0;
				stCaExProductList.pstSectorProducts[i].wSectorDateCode = 0;
				
				stCaExProductList.pstSectorProducts[i].bSector = i/*stCaSectorlist.pstSectors[i].bSector*/;
				stCaExProductList.pstSectorProducts[i].wMaxSizeExtendProducts = MAX_PRODUCT_PER_SECTOR;
			}	
			CA_TASK_QueryControl( 0,CA_QC_EXTENDED_PRODUCT_LIST,(void*)(&stCaExProductList) );

			*pInfoStr1 = (void*)(&stCaExProductList);
		}
		break;
			
	}

}

void IRCA_STA_DisableCAInfo(CA_INFO_EVENT_E EventType)
{
	ia_word32 wEn = 0;

	wEn = NOTIFY_MASK_DISABLE_ALL;
	CA_TASK_QueryControl( 0, CA_QC_NOTIFY_ENABLE, (void*)&wEn );
}

void IRCA_STA_Region(void **pInfo )
{
	*pInfo = (void *)(&stRegionInfo);
}

void IRCA_STA_GetBanStatus(ia_status_st **pGstatus)
{
	*pGstatus = (ia_status_st*)(&stErrorInfo.stGlobalStatus);
}

void IRCA_STA_GetFPShow( FP_SHOW_INFO **pstFPShow)
{
	*pstFPShow = &FPShow;
}



