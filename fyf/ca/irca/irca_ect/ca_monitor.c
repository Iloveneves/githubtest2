/**
 * $Id: ca_monitor.c,v 1.15 2008/08/01 03:11:50 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* The module allows EMM and ECM monitoring.
**/

/******************************** Header Files ********************************/
/* #include <stdio.h> */
#include <irca/pubh/s3_ansi_functions.h>
#include <irca/ca_task.h>
#include "ca_local.h"
#include "ca_error.h"
#include "ca_monitor.h"
#include "ca_svcinfo.h"
#include <irca/pubh/s3_osdrv.h>
#include <irca/pubh/s3_cadrv.h>
#include <irca/pubh/s3_service.h>
#include <irca/pubh/s3_message_helper.h>
#ifdef INCL_SERVICE_DMB
#include "ca_dmbsvcinfo.h"
#endif

/******************************* Local Variables ******************************/

static CA_EMM_MONITOR_T *pstLocalEmmMonitorInfo = 0;
static CA_ECM_MONITOR_T *pstLocalEcmMonitorInfo = 0;


void CA_MONITOR_Initialise(ia_word16 wMaxNumServiceGroups)
{
	ia_word16 i, j; /*should parse all alive service*/

	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */
	if (0 == wMaxNumServiceGroups)
		return ;

	if (0 == pstLocalEmmMonitorInfo)
	{
		pstLocalEmmMonitorInfo = (CA_EMM_MONITOR_T*)OS_DRV_AllocateMemory(
			(unsigned long)sizeof(CA_EMM_MONITOR_T)*(unsigned long)
			wMaxNumServiceGroups);
	}
	if (0 == pstLocalEmmMonitorInfo)
	{
		CA_ERROR(("CA_MONITOR_Initialise Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ;
	}
	if (0 == pstLocalEcmMonitorInfo)
	{
		pstLocalEcmMonitorInfo = (CA_ECM_MONITOR_T*)OS_DRV_AllocateMemory(
			(unsigned long)sizeof(CA_ECM_MONITOR_T)*(unsigned long)
			wMaxNumServiceGroups);
	}
	if (0 == pstLocalEcmMonitorInfo)
	{
		OS_DRV_FreeMemory((void *)pstLocalEmmMonitorInfo);
		CA_ERROR(("CA_MONITOR_Initialise Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ;
	}

	for (i = 0; i < wMaxNumServiceGroups; i++)
	{
		pstLocalEmmMonitorInfo[i].wEmmHandle = INVALID_GROUP_SERVICE_HANDLE;
		pstLocalEmmMonitorInfo[i].wServiceHandle = INVALID_SCELL_SERVICE_HANDLE;
		pstLocalEcmMonitorInfo[i].wGroupHandle = INVALID_GROUP_SERVICE_HANDLE;
		pstLocalEcmMonitorInfo[i].wServiceHandle = INVALID_SCELL_SERVICE_HANDLE;
		/* EMM */
		for (j = 0; j < MAX_EMM_MONITOR_LEN; j++)
		{
			pstLocalEmmMonitorInfo[i].acEmmString[j] = 0;
		}
		/* ECM */
		for (j = 0; j < MAX_ECM_MONITOR_LEN; j++)
		{
			pstLocalEcmMonitorInfo[i].acEcmString[j] = 0;
		}
	}
}

void CA_MONITOR_MemoryFree()
{
	if (0 != pstLocalEcmMonitorInfo)
	{
		FREE_MEMORY(pstLocalEcmMonitorInfo);
		pstLocalEcmMonitorInfo = 0;
	}
	if (0 != pstLocalEmmMonitorInfo)
	{
		FREE_MEMORY(pstLocalEmmMonitorInfo);
		pstLocalEmmMonitorInfo = 0;
	}
}

/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
/*
* Description: Enable/disable EMM or ECM monitoring on the given demux channel.
*
* Parameters : wDemux      - Demux channel number.
*              eMonType - 	MONITOR_TYPE_EMM
*                            		MONITOR_TYPE_ECM
*              fEnable     - TRUE - Enable monitoring;
*                            FALSE - Disable monitoring.
*
* Returns    : None.
*/
void CA_MONITOR_MonitorControl(ia_word32 wIndex, CA_SERVICE_TYPE_EN eSvcType,
												ia_bool fEnable)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_monitoring_st stMon;
	ca_drv_message_type_e eMsgType;
	ia_word16 wSCellSrvHandle;

	switch ( eSvcType )
	{
		case CA_SERVICE_EMM:
			/* select EMM monitor */
			CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: "));
			eMsgType = MSG_DVB_EMM_MONITOR;
			break;
		case CA_SERVICE_DESCRAMBLE:
			/* select ECM monitor */
			CA_DBG_V(("<= MSG_SERVICE_DEFINE (DES: "));
			eMsgType = MSG_DVB_ECM_MONITOR;
			break;
#ifdef INCL_SERVICE_DMB
		case CA_SERVICE_DMB_EMM:
			/* select EMM monitor */
			CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: "));
			eMsgType = MSG_DMB_EMM_MONITOR;
			break;
		case CA_SERVICE_DMB_DESCRAMBLE:
			/* select ECM monitor */
			CA_DBG_V(("<= MSG_SERVICE_DEFINE (DES: "));
			eMsgType = MSG_DMB_ECM_MONITOR;
			break;
#endif
		default:
			CA_ERROR(("Unexpected service type %d.\n", eSvcType));
			return;
	}
	wSCellSrvHandle = CA_SVCINFO_GetSCellHandle( wIndex, eSvcType );
	if ( INVALID_SCELL_SERVICE_HANDLE == wSCellSrvHandle )
	{
		CA_ERROR(("Corresponding SoftCell service not opened so cannot define monitoring resource.\n"));
		return ;
	}
	stSvcDefData.wServiceHandle = wSCellSrvHandle;

	/*Check whether it is necessary to redefine the monitoring resource */
	/*Assumption: the return is the actual flag since otherwise this function will returned previously*/
	if ( fEnable == CA_SVCINFO_isMonitorDefinedFlagSet( wIndex, eSvcType) )
	{
		CA_DBG(("Not necessary to redefine the monitoring service.\n"));
		return;
	}

	pvMsg = IA_OpenOutputMessage(32, 16);
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage(pvMsg, &stSvcDefData, sizeof(msg_service_define_data_st));

	stRsc.wResourceType = RESOURCE_MONITORING;
	stRsc.wResourceId = 0;
	if (fEnable == IA_TRUE)
		/* enable monitor */
	{
		CA_DBG_V((" +mon)\n"));
		stRsc.wResourceMgnt = MANAGE_ADD;
	}
	else
	{
		CA_DBG_V((" -mon)\n"));
		stRsc.wResourceMgnt = MANAGE_CLOSE;
	}
	IA_WriteToMessage(pvMsg, &stRsc, sizeof(msg_resource_st));

	stMon.wNumberOfMonitors = 1;
	IA_WriteToMessage(pvMsg, &stMon, sizeof(msg_monitoring_st));
	IA_WriteToMessage(pvMsg, &eMsgType, sizeof(ca_drv_message_type_e));

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	if (ServiceHandleMechanism(MSG_SERVICE_DEFINE, wIndex, eSvcType, pvMsg) == IA_TRUE)
	{
		CA_SVCINFO_SetMonitorDefinedFlag( wIndex, eSvcType, fEnable );
	}

	return;
}

void CA_MONITOR_ParseEmmMonitorMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_dvb_emm_monitor_data_st stEmmMonData;
	ia_word32 wEmmHandle = 0;
	ia_word16 wSvcHandle;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &stEmmMonData, sizeof(stEmmMonData)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	wSvcHandle = stEmmMonData.wServiceHandle;
	wEmmHandle = CA_SVCINFO_HandleSCell2CaClient(wSvcHandle);
	if (wEmmHandle == INVALID_GROUP_SERVICE_HANDLE)
	{
		CA_ERROR(("wEmmHandle == INVALID_GROUP_SERVICE_HANDLE \r\n"));
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* ayang 2008-04-09 Added to check the handle to make sure the usage of memory */
	if (wEmmHandle >= CA_SVCINFO_GetSvcMaxNumber())
	{
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("The handle is too big.\n"));
		return ;
	}

	pstLocalEmmMonitorInfo[wEmmHandle].wEmmHandle = wEmmHandle;
	pstLocalEmmMonitorInfo[wEmmHandle].wServiceHandle = wSvcHandle;


	/*
	* EMM monitoring bytes to be displayed:
	*
	* EMM Count					3
	* EMMPid					2
	* Address Control			1
	* Status 1					1
	* Status 2					1
	* EMM Data[0~2]				3
	* wResourceId[0~3]			4
	*/

	/* PVCS 23471 25184 25272*/
	ansi_sprintf((char*)pstLocalEmmMonitorInfo[wEmmHandle].acEmmString,
		"(%3d) %04x  %02x %02x %02x  %02x %02x %02x %08x",
		(ia_byte)(stEmmMonData.wEmmCount&0x00FF),
		stEmmMonData.wEmmPid,
		stEmmMonData.bAddressControl,
		stEmmMonData.bScStatus1,
		stEmmMonData.bScStatus2,
		stEmmMonData.abEmmPayload[0],
		stEmmMonData.abEmmPayload[1],
		stEmmMonData.abEmmPayload[2],
		(ia_word32)stEmmMonData.wResourceId	);

	CA_MONITOR_EmmNotify(wEmmHandle);
	IA_CloseMessage(pvMsg, IA_FALSE);
	return ;
}

void CA_MONITOR_ParseEcmMonitorMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_dvb_ecm_monitor_data_st stEcmMonData;
	ia_word32 wCaClientHandle;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stEcmMonData, sizeof(stEcmMonData)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	wCaClientHandle = CA_SVCINFO_HandleSCell2CaClient
		(stEcmMonData.wServiceHandle);
	if (wCaClientHandle == INVALID_GROUP_SERVICE_HANDLE)
	{
		CA_ERROR(("wCaClientHandle == INVALID_GROUP_SERVICE_HANDLE \r\n"));
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* ayang 2008-04-09 Added to check the handle to make sure the usage of memory */
	if (wCaClientHandle >= CA_SVCINFO_GetSvcMaxNumber())
	{
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("The handle is too big.\n"));
		return ;
	}

	pstLocalEcmMonitorInfo[wCaClientHandle].wServiceHandle = stEcmMonData.wServiceHandle;
	pstLocalEcmMonitorInfo[wCaClientHandle].wGroupHandle = wCaClientHandle;

	/*
	* ECM monitoring bytes to be displayed:
	*
	* EcmCount				1
	* EcmPid				2
	* Page					1
	* Version				1
	* ScStatus1				1
	* ScStatus2				1
	* abScReply[0~5]		6
	* bAlgorithm			1
	* wResourceId[0~3]		4
	*/

	/* PVCS 23471 25184 25272*/
	ansi_sprintf((char*)pstLocalEcmMonitorInfo[wCaClientHandle].acEcmString,
		"(%3d) %04x  %02x %02x  %02x %02x  %02x %02x %02x %02x %02x %02x  %02x %08x",
		(ia_byte)(stEcmMonData.wEcmCount&0x00FF),
		stEcmMonData.wEcmPid,
		stEcmMonData.bPage,
		stEcmMonData.bVersion,
		stEcmMonData.bScStatus1,
		stEcmMonData.bScStatus2,
		stEcmMonData.abScReply[0],
		stEcmMonData.abScReply[1],
		stEcmMonData.abScReply[2],
		stEcmMonData.abScReply[3],
		stEcmMonData.abScReply[4],
		stEcmMonData.abScReply[5],
		stEcmMonData.bAlgorithm,
		(ia_word32)stEcmMonData.wResourceId
		);

	CA_MONITOR_EcmNotify(wCaClientHandle);
	IA_CloseMessage(pvMsg, IA_FALSE);
}

void CA_MONITOR_EmmNotify( /* ia_word16 wDemux */ia_word32 wEmmHandler)
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_MONITOR_EMM) == IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_MONITOR_EMM, wEmmHandler);
	}
}

void CA_MONITOR_EcmNotify(ia_word32 wCaClientHandle)
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_MONITOR_ECM) == IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_MONITOR_ECM, (ia_word32)wCaClientHandle);
	}
}


CA_QUERY_STATUS_EN CA_MONITOR_MonEcmQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_word16 i;
	ia_word32 wService;
	CA_ECM_MONITOR_T *pstMonEcm;

	CA_PARAM_NOT_USED( wHandle );
	pstMonEcm = ( CA_ECM_MONITOR_T* ) pCAClientMsg;
	if (pstMonEcm == 0)
		return CA_QUERY_FAIL;

	wService = pstMonEcm->wGroupHandle;

	if (wService >= CA_SVCINFO_GetSvcMaxNumber())
	{
		CA_ERROR(("Mon::Unknown Service nr 0x%04x.\n", wService));
		return CA_QUERY_FAIL;
	}
	pstMonEcm->wServiceHandle = pstLocalEcmMonitorInfo[wService].wServiceHandle;

	for (i = 0; i < MAX_ECM_MONITOR_LEN; i++)
	{
		pstMonEcm->acEcmString[i] = pstLocalEcmMonitorInfo[wService].acEcmString[i];
	}
	return CA_QUERY_SUCCESS;

}

CA_QUERY_STATUS_EN CA_MONITOR_MonEmmQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_word32 wService = 0;
	ia_word16 i;
	CA_EMM_MONITOR_T *pstMonEmm;

	CA_PARAM_NOT_USED( wHandle );
	pstMonEmm = ( CA_EMM_MONITOR_T* ) pCAClientMsg;
	if (pstMonEmm == 0)
		return CA_QUERY_FAIL;
	wService = pstMonEmm->wEmmHandle;
	if (wService >= CA_SVCINFO_GetSvcMaxNumber())
	{
		CA_ERROR(("Mon::Unknown Service nr 0x%04x.\n", wService));
		return CA_QUERY_FAIL;
	}
	pstMonEmm->wServiceHandle = pstLocalEmmMonitorInfo[wService].wServiceHandle;

	for (i = 0; i < MAX_EMM_MONITOR_LEN; i++)
	{
		pstMonEmm->acEmmString[i] =
			pstLocalEmmMonitorInfo[wService].acEmmString[i];
	}
	return CA_QUERY_SUCCESS;
}

