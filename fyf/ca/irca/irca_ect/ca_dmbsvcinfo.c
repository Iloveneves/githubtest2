/**
 * $Id: ca_dmbsvcinfo.c,v 1.5 2008/05/09 08:55:08 ayang Exp $
 *
 * Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
 *
 * The module handles service info related queries and operations.
 **/

/******************************** Header Files ********************************/
#include <irca/pubh/s3_portable_types.h>
#include <irca/pubh/s3_service.h>
#include <irca/pubh/s3_osdrv.h>
#include <irca/ca_task.h>
#include "ca_local.h"
#include "ca_dmbsvcinfo.h"
#include "ca_svcinfo.h"
#include "ca_scard.h"

#ifdef INCL_SERVICE_DMB

static void CA_DMB_DesSvcNotify(  ia_word32 wHandle );
static ia_result CA_SVCINFO_GETDMBInfo(ia_word32 wHandle, ia_word32 wSrvType,CA_LOCAL_BASIC_DMB_INFO_T *pstBasicDMBInfo);
static CA_LOCAL_BASIC_DMB_INFO_T*
	FindDMBServiceBySCellHandle( ia_word16 wSCellHandle, ia_word16 *pwIndex0, ia_word16 *pwIndex1 );
extern void SendServiceOpenRequestMsg( ia_word16 wIndex, ia_word16 wServiceType );
extern void PrepareSmartCardResources( ia_word16 wResourceMgnt, ia_msg_handle pvMsg );
extern DEMUX_INFO_ST *pstDMX ;
extern SERVICE_INFO_ST *pstService;
extern CA_LOCAL_DEMUX_INFO_T* astLocalDemuxInfo[ MAX_DEMUX_COUNTS ];
extern CA_LOCAL_GROUP_SERVICE_INFO_T *pstLocalGrpSvcInfo ;
extern CA_EMM_MONITOR_T *pstLocalEmmMonitorInfo ;
extern CA_ECM_MONITOR_T *pstLocalEcmMonitorInfo ;
CA_EMM_TLV_NOTIFY_T CA_EMM_TLV_MESSAGE;

/************************* Local Function Declarations ************************/


/*
*     ROUTINE : CA_DMB_AddEmmResourceToEmmService()
* DESCRIPTION : define EMM mapping table resouce .
* INPUT : wOpSeqNo - DemuxID + SmartCardID
* OUTPUT : None
* RETURNS : .
* CALLED BY : DefineEMMServicesOnDemux
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void CA_DMB_AddEmmResourceToEmmService( ia_word32 wOpSeqNo)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_dmb_emm_st stDMBEmm;
	ia_byte bResourceCount = 0;
	CA_LOCAL_BASIC_DMB_INFO_T pstBasicDMBInfo;
	ia_word16 wDemuxIndex;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wSCellSrvHandle = 0;


	if (0 == pstDMX)
		return;

	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_DBG(("AddEMMSource: Invalid demux index.\n"));
		return;
	}

	if (IA_FALSE == pstDMX[wDemuxIndex].fEmmServiceReady)
	{
		CA_DBG(("The EMM service is not ready.\n"));
		return;
	}

	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_DBG(("AddEMMSource: Invalid demux index.\n"));
		return;
	}

	wSCellSrvHandle = CA_SVCINFO_GetSCellHandle( wOpSeqNo, CA_SERVICE_DMB_EMM );
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellSrvHandle)
		return;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_DMB_EMM;      /* it's emm service */
	if (CA_SVCINFO_GetSvcHeaderInfo( wDemuxIndex, &stCaSvcGrpDefHeader) == IA_FAIL)
		return;
	CA_SVCINFO_GETDMBInfo(wOpSeqNo,CA_SERVICE_DMB_EMM,&pstBasicDMBInfo);
	bResourceCount = 1;
	pvMsg = IA_OpenOutputMessage( (ia_word16)(sizeof(msg_service_define_data_st) +
					sizeof(msg_resource_st) * bResourceCount + sizeof(msg_dmb_emm_st)) ,
										(ia_word16)sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wSCellSrvHandle;
	stSvcDefData.bResourceCount = bResourceCount;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(stSvcDefData) );

	stRsc.wResourceType = RESOURCE_DMB_EMM;
	if((pstBasicDMBInfo.bTransportMode)&CLIENT_DEFINE_DMB_MPEG_TS!=0)
	{
		stRsc.wResourceId = (ia_word32)(stCaSvcGrpDefHeader.wGroupHandle << 16);
		stRsc.wResourceId += pstBasicDMBInfo.wEmmPid;
	}
	else
	{
		stRsc.wResourceId = 0;
	}

	if(pstDMX[wDemuxIndex].fCatSent == IA_FALSE)
	{
		stRsc.wResourceMgnt = MANAGE_ADD;
	}
	else
	{
		stRsc.wResourceMgnt = MANAGE_UPDATE;
	}
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(stRsc) );

	stDMBEmm.bMappingTableSubChId = pstBasicDMBInfo.bMappingTableSubChId;

	IA_WriteToMessage( pvMsg, &stDMBEmm, sizeof(stDMBEmm) );

	if( SCELL_Message( MSG_SERVICE_DEFINE, (ia_word16)wOpSeqNo, IA_GetOutputMessageLength(pvMsg),
		IA_GetOutputMessage(pvMsg) ) == IA_SUCCESS )
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage( pvMsg, IA_FALSE );
		pstDMX[wDemuxIndex].fCatSent = IA_TRUE;
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
}

/*
* ROUTINE : DefineDMBDescrambleServices()
* DESCRIPTION : Define ECM services for a transponder
* INPUT : pbMsg -- the ECM Service Define message passed by a CA Client.
*					wGroupHandle -	The service id, and ECT expose this as the ECM index
*									to the CA Client.
* OUTPUT : None
* RETURNS : None
* CALLED BY : CaProcessMsg
* Author : Andrew Hui
* Last Changed: September 20, 2006
*/
void DefineDMBDescrambleServices(ia_word32 wGroupHandle, ia_byte *pbMsg)
{
	ia_word32			wEMMHandle ;
	CA_SERVICE_HEADER	stCaServiceHeader;
	ia_byte		bIndex = 10;

	/* read the corresponding tranponder ID of the descramble service */
	wEMMHandle = ( ia_word16 ) ( pbMsg[bIndex++] << 24 );
	wEMMHandle += ( ia_word16 ) ( pbMsg[bIndex++] << 16 );
	wEMMHandle += ( ia_word16 ) ( pbMsg[bIndex++] << 8 );
	wEMMHandle += ( ia_word16 )( pbMsg[bIndex++] );

	stCaServiceHeader.wGroupHandle = wGroupHandle;
	/* jyou: 3.4.2 default is the first smart card */
	stCaServiceHeader.wSmartCardId = CA_SCARD_GetSmartCardId( 0 );
	/* jyou: 3.4.2 default is the EMM handle */
	stCaServiceHeader.wDemuxId = wEMMHandle;
	/* default: wGroupHandle, macrovisionId is unique */
	stCaServiceHeader.wMacrovisionId = wGroupHandle;
	stCaServiceHeader.wServiceType = CA_SERVICE_DMB_DESCRAMBLE; /* use CA_SERVICE_DESCRAMBLE as grp service index */

	/* Save the default resource IDs for descramble service */
	if (INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE))
	{
		CA_SVCINFO_SetSvcHeaderInfo(wGroupHandle, &stCaServiceHeader);

	}
	CA_SVCINFO_SetDMBInfo(wGroupHandle,CA_SERVICE_DMB_DESCRAMBLE,pbMsg);
	/* It's just for the new service */
	if (INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE))
	{
		SendServiceOpenRequestMsg((ia_word16)wGroupHandle, SERVICE_DMB_DESCRAMBLE);
	}
	else /* It's just for the DMB update */
	{
		UpdateDefaultResToDMBDescrambleService(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE,IA_TRUE);
		if(pstService[wGroupHandle].fDesServiceReady == IA_FALSE)
			pstService[wGroupHandle].fDesServiceReady = IA_TRUE;
		AddEcmResToDMBDescrambleService(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE,IA_TRUE);
	}
}

/*
* ROUTINE : CA_DMB_AddDefaultResourceToEmmService()
* DESCRIPTION : Define default respurce(smartcard)  for EMM services
* INPUT : wOpSeqNo -- DmuxIndex + SmartCardIndex.
* OUTPUT : None
* RETURNS : None
* CALLED BY : CaProcessMsg
* Author : Andrew Hui
* Last Changed: September 20, 2006
*/
void CA_DMB_AddDefaultResourceToEmmService( ia_word32 wOpSeqNo )
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	ia_byte bResourceCount = 0;
	ia_word16  wDemuxIndex;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wSCellSrvHandle = 0;


	if (0 == pstDMX)
		return;

	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_DBG(("AddEMMSource: Invalid demux index.\n"));
		return;
	}

	wSCellSrvHandle = CA_SVCINFO_GetSCellHandle( wOpSeqNo, CA_SERVICE_DMB_EMM );
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellSrvHandle)
		return;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_DMB_EMM;      /* it's emm service */
	if (CA_SVCINFO_GetSvcHeaderInfo( wDemuxIndex, &stCaSvcGrpDefHeader) == IA_FAIL)
		return;

	bResourceCount = 1;

	pvMsg = IA_OpenOutputMessage((ia_word16)( sizeof(msg_service_define_data_st) +
					sizeof(msg_resource_st) * bResourceCount ) ,  (ia_word16)sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wSCellSrvHandle;
	stSvcDefData.bResourceCount = bResourceCount;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(msg_service_define_data_st) );
	stRsc.wResourceType = RESOURCE_SMARTCARD;
	stRsc.wResourceId   = ( CA_SVCINFO_GetEMMServiceByOpSeqNo((ia_word16)wOpSeqNo) )->wSmartCardId;
	stRsc.wResourceMgnt = MANAGE_ADD;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	if( SCELL_Message( MSG_SERVICE_DEFINE, (ia_word16)wOpSeqNo, IA_GetOutputMessageLength(pvMsg),
																											 IA_GetOutputMessage(pvMsg) ) == IA_SUCCESS )
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage( pvMsg, IA_FALSE );
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
}
/*
* ROUTINE : CA_SVCINFO_SetDMBInfo()
* DESCRIPTION : Store the corresponding client service message to the local storage.
*
* INPUT : 	wIndex	- The EMMHandle+SmartCardIndex or GroupHanle passed by the CA client.
*			wSrvType 	- The type of the service. One of
*					CA_SERVICE_DMB_EMM,
*					CA_SERVICE_DMB_DESCRAMBLE,
*			pbMsg 		- message from CA Client.
*  OUTPUT : None
*  RETURNS : None
*  CALLED BY : ParseServiceOpenReplyMsg
*  Author : Andrew Hui
*  Last Changed: May 15, 2007
*/
void CA_SVCINFO_SetDMBInfo(ia_word32 wIndex, ia_word32 wSrvType, ia_byte *pbMsg)
{
	ia_byte		i, j;
	CA_LOCAL_DEMUX_INFO_T *pstDemux;
	CA_LOCAL_BASIC_DMB_INFO_T*pstDMBBasicService;
	/*Added by Andrew Hui for DMB 070510*/
	if ((CA_SERVICE_DMB_EMM == wSrvType))
	{
		i = (ia_byte)CA_LOCAL_GetUpperBytes( wIndex, 1 );
		j = (ia_byte)CA_LOCAL_GetLowerBytes( wIndex, 1 );
		if ( isValidDemuxIndex(i) == IA_FALSE || isValidSmartCardIndex(j) == IA_FALSE )
		{
			CA_ERROR(("SetSCellHandle: Invalid indices i=%d j=%d.\n", i, j));
			return;
		}
		pstDemux = astLocalDemuxInfo[i];
		pstDMBBasicService = &(pstDemux->astDMBServices[j]);
		pstDemux->fFree = IA_FALSE;
		pstDMBBasicService->bMappingTableSubChId = pbMsg[10];
		pstDMBBasicService->bTransportMode = pbMsg[11];
		if((pstDMBBasicService->bTransportMode)&CLIENT_DEFINE_DMB_MPEG_TS!=0)
		{
			pstDMBBasicService->wEmmPid =  (ia_word16)((ia_word16)(pbMsg[12]<<8) + (ia_word16)pbMsg[13]);
		}
	}
	else
	{
		if (0 == pstLocalGrpSvcInfo)
			return ;

		pstLocalGrpSvcInfo[wIndex].astDMBServices[wSrvType].bMappingTableSubChId = pbMsg[14];
		pstLocalGrpSvcInfo[wIndex].astDMBServices[wSrvType].bTransportMode = pbMsg[15];
		if((pstLocalGrpSvcInfo[wIndex].astDMBServices[wSrvType].bTransportMode)&CLIENT_DEFINE_DMB_MPEG_TS!=0)
		{
			pstLocalGrpSvcInfo[wIndex].wEmmPid =  (ia_word16)((ia_word16)(pbMsg[16]<<8) + (ia_word16)pbMsg[17]);
			pstLocalGrpSvcInfo[wIndex].bStreamCount = pbMsg[18];
			if(pstLocalGrpSvcInfo[wIndex].bStreamCount>MAX_SOFTCELL_STREAMS)
			{
				CA_ERROR(("Stream Count is too big.\n"));
				return;
			}
			for(i=0;i<pstLocalGrpSvcInfo[wIndex].bStreamCount;i++)
			{
				pstLocalGrpSvcInfo[wIndex].astDMBStreamInfo[i].bContentSubChId = pbMsg[ 19 + 3*i];
				pstLocalGrpSvcInfo[wIndex].astDMBStreamInfo[i].wContentPA = (ia_word16)( pbMsg[ 20 + 3*i] << 8 );
				pstLocalGrpSvcInfo[wIndex].astDMBStreamInfo[i].wContentPA += (ia_word16)(pbMsg[ 21 + 3*i]);
			}

		}
		else{
			pstLocalGrpSvcInfo[wIndex].bStreamCount = pbMsg[16];
			if(pstLocalGrpSvcInfo[wIndex].bStreamCount>MAX_SOFTCELL_STREAMS)
			{
				CA_ERROR(("Stream Count is too big.\n"));
				return;
			}
			for(i=0;i<pstLocalGrpSvcInfo[wIndex].bStreamCount;i++)
			{
				pstLocalGrpSvcInfo[wIndex].astDMBStreamInfo[i].bContentSubChId = pbMsg[ 17 + 3*i];
				pstLocalGrpSvcInfo[wIndex].astDMBStreamInfo[i].wContentPA = (ia_word16)( pbMsg[ 18 + 3*i] << 8 );
				pstLocalGrpSvcInfo[wIndex].astDMBStreamInfo[i].wContentPA += (ia_word16)(pbMsg[ 19 + 3*i]);
			}
		}
	}

}
/*
* ROUTINE : CA_SVCINFO_GETDMBInfo()
* DESCRIPTION : Get the corresponding client service message from the local storage.
*
* INPUT : 	wIndex	- The EMMHandle+SmartCardIndex or GroupHanle passed by the CA client.
*			wSrvType 	- The type of the service. One of
*					CA_SERVICE_DMB_EMM,
*					CA_SERVICE_DMB_DESCRAMBLE,
*  OUTPUT : pstBasicDMBInfo
*				DMB service Info
*  RETURNS : None
*  CALLED BY : ParseServiceOpenReplyMsg
*  Author : Andrew Hui
*  Last Changed: May 15, 2007
*/
static ia_result CA_SVCINFO_GETDMBInfo(ia_word32 wHandle, ia_word32 wSrvType,CA_LOCAL_BASIC_DMB_INFO_T *pstBasicDMBInfo)
{
	ia_word16 i, j;
	CA_LOCAL_DEMUX_INFO_T *pstDemux;
	CA_LOCAL_BASIC_DMB_INFO_T	*pstDMBBasicService;
	/*Added by Andrew Hui for DMB 070510*/
	if ((CA_SERVICE_DMB_EMM == wSrvType))
	{
		i = CA_LOCAL_GetUpperBytes( wHandle, 1 );
		j = CA_LOCAL_GetLowerBytes( wHandle, 1 );
		if ( isValidDemuxIndex(i) == IA_FALSE || isValidSmartCardIndex(j) == IA_FALSE )
		{
			CA_ERROR(("GetSCellHandle: Invalid indices i=%d j=%d.\n", i, j));
			return INVALID_SCELL_SERVICE_HANDLE;
		}

		pstDemux = astLocalDemuxInfo[i];
		pstDMBBasicService = &(pstDemux->astDMBServices[j]);
		pstBasicDMBInfo->bMappingTableSubChId = pstDMBBasicService->bMappingTableSubChId;
		pstBasicDMBInfo->bTransportMode= pstDMBBasicService->bTransportMode;
		pstBasicDMBInfo->wEmmPid= pstDMBBasicService->wEmmPid ;
	}
	else
	{
		if (0 == pstLocalGrpSvcInfo)
			return INVALID_SCELL_SERVICE_HANDLE;
		pstBasicDMBInfo->bMappingTableSubChId = pstLocalGrpSvcInfo[wHandle].astDMBServices[wSrvType].bMappingTableSubChId;
		pstBasicDMBInfo->bTransportMode= pstLocalGrpSvcInfo[wHandle].astDMBServices[wSrvType].bTransportMode;
	}
	return IA_TRUE;
}
/*
* ROUTINE : CA_SVCINFO_GetDMBEMMSvcInfoPtr()
* DESCRIPTION : Get the pointer to the local EMM Service storage.
* INPUT : wSCellHandle - The SoftCell service handle.
*			     pwIndex - pointer to the buffer of the EMM service index
* OUTPUT : The index of the demux that contains the EMM service in the local storage
* RETURNS : !=0 -- pointer to the EMM service found
*				==0 -- there is no EMM service matching the given SoftCell service handle.
* SIDE EFFECT : None
* CALLED BY : CA_SVCINFO_ParseEMMServiceInfo
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
CA_LOCAL_BASIC_DMB_INFO_T* CA_SVCINFO_GetDMBEMMSvcInfoPtr( ia_word16 wSCellHandle, ia_word16 *pwIndex0, ia_word16 *pwIndex1 )
{
	/* jyou: 3.4.2 multiple smart card */
	return FindDMBServiceBySCellHandle( wSCellHandle, pwIndex0, pwIndex1 );
}
/*
* ROUTINE : FindDMBServiceBySCellHandle()
* DESCRIPTION : Get the pointer to the local EMM Service storage.
* INPUT : wSCellHandle - The SoftCell service handle.
* OUTPUT : pwIndex0:The index of the demux that contains the EMM service in the local storage
				pwIndex1: The smart card index
* CALLED BY : CA_SVCINFO_GetDMBEMMSvcInfoPtr
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
static CA_LOCAL_BASIC_DMB_INFO_T*
	FindDMBServiceBySCellHandle( ia_word16 wSCellHandle, ia_word16 *pwIndex0, ia_word16 *pwIndex1 )
{
	ia_byte		i, j;
	ia_word16	wGrpNum;

	/* Search the EMM service info storage for matching service */
	for ( i = 0; i < MAX_DEMUX_COUNTS ; i++ )
	{
		if ( astLocalDemuxInfo[i] != 0 )
		{
			for ( j = 0 ; j < SC_SLOT_MAX ; j++ )
			{
				if (astLocalDemuxInfo[i]->astServices[j].wSCellHandle == wSCellHandle)
				{
					(*pwIndex0) = i;
					(*pwIndex1) = j;
					return &( astLocalDemuxInfo[i]->astDMBServices[j] );
				}
			}
		}
	}

	/* Search the Group service info storage for matching service */
	if ( 0 != pstLocalGrpSvcInfo )
	{
		wGrpNum = CA_SVCINFO_GetSvcMaxNumber();
		for (i = 0; i < wGrpNum; i++)
		{
			for (j = 0; j < CA_SERVICE_MAX_SERVICES; j++)
			{
				if (pstLocalGrpSvcInfo[i].astServices[j].wSCellHandle == wSCellHandle)
				{
					(*pwIndex0) = i;
					(*pwIndex1) = j;
					return &( pstLocalGrpSvcInfo[i].astDMBServices[j] );
				}
			}
		}
	}

	(*pwIndex0) = INVALID_INDEX;
	/* ayang 2008-04-09 Modified pwIndex0 to pwIndex1 */
	(*pwIndex1) = INVALID_INDEX;
	return 0;
}

/*
* ROUTINE : UpdateDefaultResToDMBDescrambleService()
* Description: Update default resource
*				DMB_DESCRAMBLER;SmartCard
* Parameters : wCaClientHandle
*				 wSrvType:
* Returns    : None
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void UpdateDefaultResToDMBDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType,ia_bool bAction)
{
	ia_msg_handle				pvMsg;
	msg_service_define_data_st	stSvcDefData;
	msg_resource_st				stRsc;
	ia_word16						wScellSvcHandle;
	CA_SERVICE_HEADER			stCaSvcGrpDefHeader;

	if(pstService[wGroupHandle].fDesServiceReady == IA_TRUE)
	{
		CA_ERROR(("pstService[wGroupHandle].fDesServiceReady == IA_TRUE.\n"));
		return;
	}

	ansi_memset( &stSvcDefData, 0x00, sizeof( stSvcDefData ));
	/* wDemux = CA_SVCINFO_GetDesSvcDemuxFromServiceId(wService); */
	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_DMB_DESCRAMBLE;      /* it's desc service */
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_ERROR(("Can't get the service header.\n"));
		return;
	}

	wScellSvcHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, wSrvType);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellSvcHandle)
	{
		/* Message not sent. We must free the buffer. */
		CA_ERROR(("Can't get the valid softcell service handle.\n"));
		return;
	}

	stSvcDefData.bResourceCount = 2;/*1 + CA_SCARD_ValidSmartCards();*/
	pvMsg = IA_OpenOutputMessage( (ia_word16)(sizeof(msg_service_define_data_st) + sizeof(msg_resource_st) * stSvcDefData.bResourceCount), (ia_word16)sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wScellSvcHandle;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(msg_service_define_data_st) );

	stRsc.wResourceType = RESOURCE_DMB_DESCRAMBLER;
	stRsc.wResourceId   = stCaSvcGrpDefHeader.wDemuxId;  /* the resource id isn't used */
	if(bAction==IA_TRUE){
		stRsc.wResourceMgnt = MANAGE_ADD;
	}
	else{
		stRsc.wResourceMgnt = MANAGE_CLOSE;
	}
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	/* jyou: 3.4.2 multiple smartcard; add all resources for descramble service */
	if(bAction==IA_TRUE){
		PrepareSmartCardResources( MANAGE_ADD, pvMsg );
	}
	else{
		PrepareSmartCardResources( MANAGE_CLOSE, pvMsg );
	}

	if( SCELL_Message( MSG_SERVICE_DEFINE, (ia_word16)wGroupHandle, IA_GetOutputMessageLength(pvMsg),
		IA_GetOutputMessage(pvMsg) ) == IA_SUCCESS )
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage( pvMsg, IA_FALSE );
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
}
/*
* ROUTINE : AddEcmResToDMBDescrambleService()
* Description: Added default resource
*			     DMB_DESCRAMBLER;SmartCard
* Parameters : wCaClientHandle
*				 wSrvType:
* Returns    : None
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void AddEcmResToDMBDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType,ia_bool bAction)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	ia_word16 i;
	ia_word16 wScellHandle;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	msg_dmb_ecm_st stDMBEcm;
	msg_dmb_content_streamid_st stContent;
	CA_LOCAL_BASIC_DMB_INFO_T pstBasicDMBInfo;

	stCaSvcGrpDefHeader.wServiceType = wSrvType;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_ERROR(("AddEcmSourceToDescrambleService Can't find the service header.\n"));
		return;
	}
	if (0 == pstService)
	{
		CA_ERROR(("AddEcmSourceToDescrambleService pstService = 0.\n"));
		return;
	}

	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE);
	CA_SVCINFO_GETDMBInfo(wGroupHandle,CA_SERVICE_DMB_DESCRAMBLE,&pstBasicDMBInfo);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_ERROR(("AddEcmSourceToDescrambleService The Softcell service handle is not available.\n"));
		return;
	}

	/*pvMsg = IA_OpenOutputMessage(128 , 32 );	*/
	pvMsg = IA_OpenOutputMessage((ia_word16)( sizeof(msg_service_define_data_st)
														+ sizeof(msg_resource_st)
														+sizeof(msg_dmb_ecm_st)
														+sizeof(msg_dmb_content_streamid_st)*(pstLocalGrpSvcInfo[wGroupHandle].bStreamCount)) ,
														(ia_word16)sizeof(msg_dmb_content_streamid_st) );
	if (pvMsg == 0)
	{
		return;
	}
	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = 1;
	/*write service define data to message*/
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(msg_service_define_data_st) );

	stRsc.wResourceType = RESOURCE_DMB_ECM;
	if((pstBasicDMBInfo.bTransportMode)&CLIENT_DEFINE_DMB_MPEG_TS!=0)
	{
		stRsc.wResourceId = (ia_word32)( stCaSvcGrpDefHeader.wDemuxId<< 16);
		stRsc.wResourceId += pstBasicDMBInfo.wEmmPid;
	}
	else
	{
		stRsc.wResourceId = 0;
	}
	/* MANAGE_UPDATE will update a resource for a service. If the resource was not added yet
	then it will be added to the service. */
	if(bAction==IA_TRUE){
		if( pstService[wGroupHandle].fEcmSrcAdded == IA_FALSE )
		{
			stRsc.wResourceMgnt = MANAGE_ADD;
		}
		else
		{
			stRsc.wResourceMgnt = MANAGE_UPDATE;
		}
	}
	else{

		stRsc.wResourceMgnt = MANAGE_CLOSE;
	}

	/*write resoure data to message*/
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	stDMBEcm.bMappingTableSubChId = pstBasicDMBInfo.bMappingTableSubChId;
	stDMBEcm.bStreamCount = pstLocalGrpSvcInfo[wGroupHandle].bStreamCount;
	/*write dmb ecm data to message*/
	IA_WriteToMessage( pvMsg, &stDMBEcm, sizeof(msg_dmb_ecm_st) );

	for( i = 0; i<stDMBEcm.bStreamCount ; i++)
	{
		stContent.bContentSubChId = pstLocalGrpSvcInfo[wGroupHandle].astDMBStreamInfo[i].bContentSubChId ;
		stContent.wContentPA = pstLocalGrpSvcInfo[wGroupHandle].astDMBStreamInfo[i].wContentPA ;
		/*write dmb content data to message*/
		IA_WriteToMessage( pvMsg, &stContent, sizeof(msg_dmb_content_streamid_st) );
	}

	/*IA_WriteToMessage( pvMsg, &stEcmSrc, sizeof(stEcmSrc) );*/

	if (SCELL_Message( MSG_SERVICE_DEFINE, (ia_word16)wGroupHandle, IA_GetOutputMessageLength(pvMsg),
		IA_GetOutputMessage(pvMsg)) == IA_SUCCESS)
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage(pvMsg, IA_FALSE);
		if(bAction){
			pstService[wGroupHandle].fEcmSrcAdded = IA_TRUE;
		}
		else{
			pstService[wGroupHandle].fEcmSrcAdded = IA_FALSE;
		}
		CA_ERROR(("SUCCESS send RESOURCE_DMB_ECM define message.\n"));
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage(pvMsg, IA_TRUE);
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}

}
/*
* ROUTINE : CA_DMB_ParseDescrambleInfo()
* Description: Parse the descramble information from softcell
* Parameters :
* Returns    :
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
ia_bool CA_DMB_ParseDescrambleInfo(ia_word32 wServiceType,ia_word32 wGroupHandle,msg_service_status_data_st* pstSvcStatusData,ia_msg_handle pvMsg)
{
	CA_LOCAL_GROUP_SERVICE_INFO_T *pstGrpSvc = 0;
	msg_rsc_status_st stRscStatus;
	ia_word16 i,j, wResCount = 0;
	ia_bool fMessageClose=IA_FALSE;
	msg_dmb_ecm_status_st stEcmSrcStatus;
	msg_dmb_stream_status_st stStreamStatus;

	ansi_memset((void *)&stRscStatus, 0, sizeof(stRscStatus));

	if(CA_SERVICE_DMB_DESCRAMBLE==wServiceType)
	{
		CA_DBG_V(("Descramble service status.\n"));
		pstGrpSvc = (CA_LOCAL_GROUP_SERVICE_INFO_T*)CA_SVCINFO_GetSvcInfoPtr(wGroupHandle, CA_SERVICE_DESCRAMBLE);
		if (pstGrpSvc == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			fMessageClose=IA_TRUE;
			return fMessageClose;
		}

		pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].stGlobalStatus = pstSvcStatusData->stStatus;

#ifndef TO_SOFTCELL_3_0 /*only for softcell 3.1.x or higher*/
		pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].stServiceStatus = pstSvcStatusData->stServiceStatus;
#endif

		for (i = 0; i < pstSvcStatusData->bResourceCount; i++)
		{
			IA_ReadFromMessage( pvMsg, &stRscStatus, sizeof(stRscStatus) );

			switch( stRscStatus.wType )
			{
			case RESOURCE_DMB_ECM:
				/*CA_DBG_V(("Descramble DMB_ECM status.\n"));*/
				{
				IA_ReadFromMessage( pvMsg, &stEcmSrcStatus, sizeof(stEcmSrcStatus) );
				pstGrpSvc->bStreamCount = stEcmSrcStatus.bStreamCount;
				/*CA_DBG_V(("bStreamCount=%d.\n",stEcmSrcStatus.bStreamCount));*/
				for( j=0; j<stEcmSrcStatus.bStreamCount; j++ )
				{
					IA_ReadFromMessage( pvMsg, &stStreamStatus, sizeof(stStreamStatus) );
					pstGrpSvc->astDMBStreamConInfo[j].bContentSubChId = stStreamStatus.bContentSubChId;
					pstGrpSvc->astDMBStreamConInfo[j].wContentPA    	= stStreamStatus.wContentPA;
					pstGrpSvc->astDMBStreamConInfo[j].bEcmSubChId 	= stStreamStatus.bEcmSubChId;
					pstGrpSvc->astDMBStreamConInfo[j].wEcmPA    		= stStreamStatus.wEcmPA;

					ansi_memcpy( &(pstGrpSvc->astDMBStreamConInfo[j].stStatus), &(stStreamStatus.stStatus) , sizeof(ia_status_st));
				}
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;

				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				break;
			}
			case RESOURCE_DMB_DESCRAMBLER:
				/* save the resource type and resource status */
				/*CA_DBG_V(("Descramble DMB_DESCRAMBLER status.\n"));*/
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;

				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;

				break;

			case RESOURCE_SMARTCARD:
				/* save the resource type and resource status */
				/*CA_DBG_V(("Descramble SMARTCARD status.\n"));*/
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				break;

			case RESOURCE_MONITORING:
				/* save the resource type and resource status */
				/*CA_DBG_V(("Descramble MONITORING status.\n"));*/
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				break;
			default:
				CA_ERROR(("Unknown rsc type.\n"));
				break;
			}
			/*SoftCell3.0 does not caculate the severity among the global status and modules' status,
			ECT should do this, but the later version of SoftCell has done this internal, must not do it
			again in ECT, or there will be error for multiple smartcard.
			logged to SCR#25349, on Nov 15, 2006
			*/
		}

		CA_ERROR_UpdateErrorStatus(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE, &pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].stGlobalStatus);
		pstGrpSvc->astServices[CA_SERVICE_DMB_DESCRAMBLE].bRscCount = (ia_byte)wResCount;
		/*CA_ERROR(("The value of resource count to ECT Locoal=%d\n",wResCount));*/
		CA_DMB_DesSvcNotify(wGroupHandle);

		IA_CloseMessage(pvMsg, IA_FALSE);
		fMessageClose=IA_TRUE;
		return fMessageClose;
	}
	else
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		fMessageClose=IA_TRUE;
		return fMessageClose;
	}
}
/*
* ROUTINE : CA_DMB_ParseDescrambleInfo()
* Description: Parse the descramble information from softcell
* Parameters :
* Returns    :
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void RemoveDMBEmmResFromService(ia_word16 wOpSeqNo)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	ia_word16 wSCellHandle = 0, wDemuxIndex;
	ia_word32 wDemuxId = 0;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;

	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_DBG(("RemoveEMMSource: Invalid demux index.\n"));
		return;
	}

	wSCellHandle = CA_SVCINFO_GetSCellHandle(wOpSeqNo, CA_SERVICE_EMM);
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_EMM;
	CA_SVCINFO_GetSvcHeaderInfo( wDemuxIndex, &stCaSvcGrpDefHeader);
	wDemuxId = stCaSvcGrpDefHeader.wDemuxId;
	if (INVALID_GROUP_SERVICE_HANDLE == wDemuxId)
	{
		return;
	}

	pvMsg = IA_OpenOutputMessage( 32, 16 );

	stSvcDefData.wServiceHandle = wSCellHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(stSvcDefData) );

	stRsc.wResourceType = RESOURCE_DMB_EMM;
	stRsc.wResourceId   = wDemuxId;
	/* MANAGE_CLOSE will close the resource and delete it from the service. */
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(stRsc) );

	if( SCELL_Message( MSG_SERVICE_DEFINE, (ia_word16)wOpSeqNo, IA_GetOutputMessageLength(pvMsg),
		IA_GetOutputMessage(pvMsg) ) == IA_SUCCESS )
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage( pvMsg, IA_FALSE );
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
	return;
}
/***************************Added for DMB Extension*****************************/
/*
*     ROUTINE : CA_DMB_DesSvcNotify()
* DESCRIPTION : Notify the CA Client about the DMB EMM service info.
* INPUT : wSCellHandle - The group handle defined by CA Client.
* OUTPUT : None
* RETURNS : None.
* CALLED BY : CA_DMB_ParseDescrambleInfo
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
static void CA_DMB_DesSvcNotify(  ia_word32 wHandle )
{
	if( CA_TASK_CheckNotifyEnable(NOTIFY_MASK_DMB_ECM_SVC) == IA_TRUE)
	{
		CA_CLIENT_Notify(wHandle , CA_QC_DMB_DESCRAMBLE_SERVICE_INFO, 0);
	}
}
/*
*     ROUTINE : CA_DMB_EmmSvcNotify()
* DESCRIPTION : Notify the CA Client about the DMB EMM service info.
* INPUT : wSCellHandle - The emm handle defined by CA Client.
* OUTPUT : None
* RETURNS : None.
* CALLED BY : CA_SVCINFO_ParseEMMServiceInfo
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void CA_DMB_EmmSvcNotify(ia_word32 wHandle)
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_DMB_EMM_SVC) == IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_DMB_EMM_SERVICE_INFO, wHandle);
	}
}

/*
*     ROUTINE : CA_DMB_DescrambleInfoQueryControl()
* DESCRIPTION : for CA Client to get the status of DMB ECM Service.
* INPUT : wSCellHandle - The group handle defined by CA Client.
			pCAClientMsg- CA Client message
* OUTPUT : None
* RETURNS : .
* CALLED BY : CA_DMB_DescrambleInfoQueryControl
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
CA_QUERY_STATUS_EN CA_DMB_DescrambleInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_word16 i;
	ia_word32 wGroupHandle;
	ia_word16 wSrvHandle;
	CA_DMB_DESCRAMBLE_INFO_T *pstDesSvcInfo;

	CA_PARAM_NOT_USED( wHandle );
	pstDesSvcInfo = ( CA_DMB_DESCRAMBLE_INFO_T* ) pCAClientMsg;
	if (0 == pstDesSvcInfo)
	{
		CA_ERROR(("The pointer is NULL.\n"));
		return CA_QUERY_FAIL;
	}
	if (0 == pstLocalGrpSvcInfo)
	{
		return CA_QUERY_FAIL;
	}

	wGroupHandle = pstDesSvcInfo->wGroupHandle;
	if (wGroupHandle >= CA_SVCINFO_GetSvcMaxNumber())
	{
		CA_ERROR(("SVC::Unknown wGroupHandle 0x%04x.\n", wGroupHandle));
		return CA_QUERY_FAIL;
	}

	wSrvHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE);
	pstDesSvcInfo->wServiceHandle = wSrvHandle;
	pstDesSvcInfo->bRscCount = pstLocalGrpSvcInfo[wGroupHandle].astServices[CA_SERVICE_DMB_DESCRAMBLE].bRscCount;
	if(pstDesSvcInfo->bRscCount == 0)
	{

		DMB_SVCINFO_ResetLocalSvcInfo(wGroupHandle);
		CA_DMB_ResetLocalECMServiceInfo(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE);
		CA_ERROR(("all the resource have been closed.\n"));
		return CA_QUERY_FAIL;
	}
	pstDesSvcInfo->stGlobalStatus =
		pstLocalGrpSvcInfo[wGroupHandle].astServices[CA_SERVICE_DMB_DESCRAMBLE].stGlobalStatus;
#ifndef TO_SOFTCELL_3_0
	pstDesSvcInfo->stServiceStatus =
		pstLocalGrpSvcInfo[wGroupHandle].astServices[CA_SERVICE_DMB_DESCRAMBLE].stServiceStatus;
#endif
	pstDesSvcInfo->bStreamCount = pstLocalGrpSvcInfo[wGroupHandle].bStreamCount;
	for (i = 0; i < pstLocalGrpSvcInfo[wGroupHandle].bStreamCount; i++)
	{
		pstDesSvcInfo->astStreamInfo[i].bContentSubChId =
			pstLocalGrpSvcInfo[wGroupHandle].astDMBStreamConInfo[i].bContentSubChId;
		pstDesSvcInfo->astStreamInfo[i].wContentPA =
			pstLocalGrpSvcInfo[wGroupHandle].astDMBStreamConInfo[i].wContentPA;
		pstDesSvcInfo->astStreamInfo[i].bEcmSubChId=
			pstLocalGrpSvcInfo[wGroupHandle].astDMBStreamConInfo[i].bEcmSubChId;
		pstDesSvcInfo->astStreamInfo[i].wEcmPA=
			pstLocalGrpSvcInfo[wGroupHandle].astDMBStreamConInfo[i].wEcmPA;
		pstDesSvcInfo->astStreamInfo[i].stStatus =
			pstLocalGrpSvcInfo[wGroupHandle].astDMBStreamConInfo[i].stStatus;
	}

	for (i = 0; i < pstLocalGrpSvcInfo[wGroupHandle].astServices[CA_SERVICE_DMB_DESCRAMBLE].bRscCount; i++)
	{
		pstDesSvcInfo->awRscType[i] =
			pstLocalGrpSvcInfo[wGroupHandle].astServices[CA_SERVICE_DMB_DESCRAMBLE].awRscType[i]; /* resource type */

		pstDesSvcInfo->astRscStatus[i].eSource =
			pstLocalGrpSvcInfo[wGroupHandle].astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[i].eSource; /*
																							resource status */
		pstDesSvcInfo->astRscStatus[i].wStatus =
			pstLocalGrpSvcInfo[wGroupHandle].astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[i].wStatus;
		pstDesSvcInfo->astRscStatus[i].eSeverity =
			pstLocalGrpSvcInfo[wGroupHandle].astServices[CA_SERVICE_DMB_DESCRAMBLE].astRscStatus[i].eSeverity;
	}
	return CA_QUERY_SUCCESS;
}
/*
* ROUTINE : CA_SVCINFO_DMBEmmSvcInfoQueryControl()
* Description: Query control for CA_QC_DMB_EMM_SERVICE_INFO type.
*
* Parameters : wHandle      - demux_index << 16 | smartcard_index .
*              pCAClientMsg - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
CA_QUERY_STATUS_EN CA_SVCINFO_DMBEmmSvcInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_word16 i, j;
	CA_DMB_EMM_SERVICE_INFO_T *pstTemp;
	CA_LOCAL_DEMUX_INFO_T *pstLocalDemuxInfo;
	CA_LOCAL_BASIC_SERVICE_INFO_T *pstLocalBasicInfo;
	CA_LOCAL_BASIC_DMB_INFO_T *pstLocalDMBBasicInfo;

	CA_PARAM_NOT_USED( wHandle );
	pstTemp = ( CA_DMB_EMM_SERVICE_INFO_T* ) pCAClientMsg;
	/* Validate the output buffer */
	if ( 0 == pstTemp )
	{
		return CA_QUERY_FAIL;
	}
	/* Validate the demux index */
		i = CA_LOCAL_GetUpperBytes( wHandle, 2 );
	if ( isValidDemuxIndex( i ) == IA_FALSE )
	{
		CA_ERROR(("SVC::Unknown demux index %d.\n", i));
		return CA_QUERY_FAIL;
	}
	pstLocalDemuxInfo = astLocalDemuxInfo[i];
	/* Validate the smart card index */
	j = CA_LOCAL_GetLowerBytes( wHandle, 2 );
	if ( isValidSmartCardIndex( j ) == IA_FALSE )
	{
		CA_ERROR(("SVC::Unknown smart card index %d.\n", j));
		return CA_QUERY_FAIL;
	}
	pstLocalBasicInfo = &( pstLocalDemuxInfo->astServices[j] );
	pstLocalDMBBasicInfo = &( pstLocalDemuxInfo->astDMBServices[j] );
	if(pstLocalBasicInfo->bRscCount ==0)
	{

		CA_SVCINFO_ReleaseDemux(pstTemp->wEmmHandle );
		CA_ERROR(("all the resource have been closed.\n"));
		return CA_QUERY_FAIL;
	}
	/* Copy local data to the output buffer--pstTemp */
	pstTemp->wEmmHandle = pstLocalDemuxInfo->wEMMHandle;
	pstTemp->wServiceHandle = pstLocalBasicInfo->wSCellHandle;
	pstTemp->stGlobalStatus = pstLocalBasicInfo->stGlobalStatus;
	pstTemp->wEmmPA = pstLocalDMBBasicInfo->wEmmPA;
	pstTemp->bEmmSubChId = pstLocalDMBBasicInfo->bEmmSubChId;
#ifndef TO_SOFTCELL_3_0
	pstTemp->stServiceStatus = pstLocalBasicInfo->stServiceStatus;
#endif

	pstTemp->bRscCount = pstLocalBasicInfo->bRscCount;
	for (i = 0; i < pstLocalBasicInfo->bRscCount; i++)
	{
		/* resource type */
		pstTemp->awRscType[i] = pstLocalBasicInfo->awRscType[i];
		/* resource status */
		pstTemp->astRscStatus[i].eSource =
			pstLocalBasicInfo->astRscStatus[i].eSource;
		pstTemp->astRscStatus[i].wStatus =
			pstLocalBasicInfo->astRscStatus[i].wStatus;
		pstTemp->astRscStatus[i].eSeverity =
			pstLocalBasicInfo->astRscStatus[i].eSeverity;
	}

	return CA_QUERY_SUCCESS;
}
/*************************Added for DMB Extension**********************************/
/*
*     ROUTINE : CA_DMB_ParseEcmMonitorMsg()
* DESCRIPTION : parse ECM monitor message from softcel .
* INPUT : softcell message
* OUTPUT : None
* RETURNS : .
* CALLED BY :
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void CA_DMB_ParseEcmMonitorMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	msg_dmb_ecm_monitor_data_st stEcmMonData;
	ia_word32 wCaClientHandle;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stEcmMonData, sizeof(stEcmMonData)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

#if 0
	stEcmMonData.wServiceHandle =0;
	stEcmMonData.wResourceId = 1;
	stEcmMonData.bScStatus1=0x80;
	stEcmMonData.bScStatus2=0x85;
	stEcmMonData.bPage=1;
	stEcmMonData.bVersion=0x02;
	stEcmMonData.bAlgorithm = 0x05;
	stEcmMonData.bOddEven = 0x00;
	stEcmMonData.fNextKey = IA_TRUE;
	stEcmMonData.abScReply[0]=0xff;
	stEcmMonData.abScReply[1]=0xff;
	stEcmMonData.abScReply[2]=0xff;
	stEcmMonData.abScReply[3]=0xff;
	stEcmMonData.abScReply[4]=0xff;
	stEcmMonData.abScReply[5]=0xff;
	stEcmMonData.wEcmCount=0x01;
	stEcmMonData.bEcmSubChId=0xff;
	stEcmMonData.wEcmPA=0xff;
#endif
	wCaClientHandle = CA_SVCINFO_HandleSCell2CaClient(stEcmMonData.wServiceHandle);
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
	* EcmCount			4
	* bEcmSubChId		1
	* wEcmPA				2
	* Page					1
	* Version				1
	* ScStatus1			1
	* ScStatus2			1
	* abScReply[0~5]		6
	* bAlgorithm			1
	* wResourceId[0~3]	4
	*/

	/* PVCS 23471 25184 25272*/
	ansi_memset(pstLocalEcmMonitorInfo[wCaClientHandle].acEcmString,0x00,MAX_ECM_MONITOR_LEN);
	ansi_sprintf((char*)pstLocalEcmMonitorInfo[wCaClientHandle].acEcmString,
		"(%4d) %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %08x",
		(ia_byte)(stEcmMonData.wEcmCount&0x00FF),
		stEcmMonData.bEcmSubChId,
		stEcmMonData.wEcmPA,
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

	CA_MONITOR_DMBEcmNotify(wCaClientHandle);

	IA_CloseMessage(pvMsg, IA_FALSE);

}
/*
*     ROUTINE : CA_DMB_ParseEmmMonitorMsg()
* DESCRIPTION : parse Emm monitor message from softcell .
* INPUT : softcell message
* OUTPUT : None
* RETURNS : .
* CALLED BY :
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void CA_DMB_ParseEmmMonitorMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	msg_dmb_emm_monitor_data_st stEmmMonData;
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

#if 0
	stEmmMonData.wServiceHandle=0x0;
	stEmmMonData.wResourceId=0x01;
	stEmmMonData.bScStatus1=0x85;
	stEmmMonData.bScStatus2=0x80;
	stEmmMonData.bAddressControl=0x01;
	stEmmMonData.abEmmPayload[ 0 ]=0x01;
	stEmmMonData.abEmmPayload[ 1 ]=0x02;
	stEmmMonData.abEmmPayload[ 2 ]=0x03;
	stEmmMonData.wEmmCount=0x01;
	stEmmMonData.bEmmSubChId=0xff;
	stEmmMonData.wEmmPA=0xff;
#endif
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
	* EMM Count				(4)
	* bEmmSubChId			1
	* wEmmPA				2
	* Address Control			1
	* Status 1					1
	* Status 2					1
	* EMM Data[0~2]			3
	* wResourceId[0~3]		4
	*/
	ansi_sprintf((char*)pstLocalEmmMonitorInfo[wEmmHandle].acEmmString,
		"(%4d) %02x %04x %02x %02x %02x %02x %02x %08x",
		(ia_byte)(stEmmMonData.wEmmCount&0x00FF),
		stEmmMonData.bEmmSubChId,
		stEmmMonData.wEmmPA,
		stEmmMonData.bAddressControl,
		stEmmMonData.bScStatus1,
		stEmmMonData.bScStatus2,
		stEmmMonData.abEmmPayload[0],
		stEmmMonData.abEmmPayload[1],
		stEmmMonData.abEmmPayload[2],
		(ia_word32)stEmmMonData.wResourceId);

	CA_MONITOR_DMBEmmNotify(wEmmHandle);

	IA_CloseMessage(pvMsg, IA_FALSE);

}
/*
* ROUTINE : CA_MONITOR_DMBEmmNotify()
* Description: Notify EMM monitor message to CA Client
* Parameters : wEmmHandler
* Returns    : None
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void CA_MONITOR_DMBEmmNotify( ia_word32 wEmmHandler)
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_MONITOR_DMB_EMM) == IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_MONITOR_DMB_EMM, wEmmHandler);
	}
}
/*
* ROUTINE : CA_MONITOR_DMBEcmNotify()
* Description: Notify ECM monitor message to CA Client
* Parameters : wCaClientHandle
* Returns    : None
* Author : Andrew Hui
* Last Changed: May 15, 2007
*/
void CA_MONITOR_DMBEcmNotify(ia_word32 wCaClientHandle)
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_MONITOR_DMB_ECM) == IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_MONITOR_DMB_ECM, (ia_word32)wCaClientHandle);
	}
}
/*
* ROUTINE : CA_DMB_ResetLocalECMServiceInfo()
* DESCRIPTION : Set the default values to the basic information of a service
* OUTPUT : None
* RETURNS : None
* CALLED BY :
* Author :
* Last Changed: May 19, 2007
*/
void CA_DMB_ResetLocalECMServiceInfo(ia_word32 wIndex, ia_word32 wSrvType)
{
	ia_byte i;

	pstLocalGrpSvcInfo[wIndex].astDMBServices[wSrvType].bMappingTableSubChId = INVALID_MAPPING_SUBCHID;
	pstLocalGrpSvcInfo[wIndex].astDMBServices[wSrvType].bTransportMode = INVALID_TRANSPORT_MODE;
	pstLocalGrpSvcInfo[wIndex].astDMBServices[wSrvType].bEmmSubChId = INVALID_MAPPING_SUBCHID;
	pstLocalGrpSvcInfo[wIndex].astDMBServices[wSrvType].wEmmPA = INVALID_PACKET_ADDRESS;
	pstLocalGrpSvcInfo[wIndex].astDMBServices[wSrvType].wEmmPid = 0x00;
	pstLocalGrpSvcInfo[wIndex].wEmmPid =  (ia_word16)(0x0);
	pstLocalGrpSvcInfo[wIndex].bStreamCount = 0;
	for(i=0;i<MAX_SOFTCELL_STREAMS;i++)
	{
		pstLocalGrpSvcInfo[wIndex].astDMBStreamInfo[i].bContentSubChId = INVALID_MAPPING_SUBCHID;
		pstLocalGrpSvcInfo[wIndex].astDMBStreamInfo[i].wContentPA = INVALID_PACKET_ADDRESS;
		pstLocalGrpSvcInfo[wIndex].astDMBStreamConInfo[i].bContentSubChId=INVALID_MAPPING_SUBCHID;
		pstLocalGrpSvcInfo[wIndex].astDMBStreamConInfo[i].bEcmSubChId=INVALID_MAPPING_SUBCHID;
		pstLocalGrpSvcInfo[wIndex].astDMBStreamConInfo[i].wContentPA=INVALID_PACKET_ADDRESS;
		pstLocalGrpSvcInfo[wIndex].astDMBStreamConInfo[i].wEcmPA=INVALID_PACKET_ADDRESS;
		pstLocalGrpSvcInfo[wIndex].astDMBStreamConInfo[i].stStatus.eSource = SOURCE_UNKNOWN;
		pstLocalGrpSvcInfo[wIndex].astDMBStreamConInfo[i].stStatus.wStatus = CA_IRDETO_ERROR_D00;
		pstLocalGrpSvcInfo[wIndex].astDMBStreamConInfo[i].stStatus.eSeverity = STATUS_INFO_DND;
	}
}
/*
*     ROUTINE : ResetBasicDMBServiceInfo()
* DESCRIPTION : Set the default values to the basic information of a service
*       INPUT : pTemp - The pointer to the DMB service in the local storage.
*      OUTPUT : None
*     RETURNS : None
*   CALLED BY :
*      Author :
* Last Changed: May 19, 2007
*/
void CA_DMB_ResetLocalEMMServiceInfo( CA_LOCAL_BASIC_DMB_INFO_T* pTemp )
{

	pTemp->bMappingTableSubChId = INVALID_MAPPING_SUBCHID;
	pTemp->bTransportMode = INVALID_TRANSPORT_MODE;
	pTemp->bEmmSubChId = INVALID_SUBCHANNEL_SUBCHID;
	pTemp->wEmmPA = INVALID_PACKET_ADDRESS;
	/*ansi_memset((void *)pTemp, 0xff, sizeof(CA_LOCAL_BASIC_DMB_INFO_T));*/
}

/********************* CA_EMM_TLV_NOTIFY *********************/
/* Description: Notify message from softcell.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : pstSCellMsg   - softcell message.
*
*
** Returns : none
***************************************************************/
void CA_EMM_TLV_NOTIFY(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_emm_tlv_notify_st EmmTlvReply;
	ia_byte  *abEMMTLV;
	ia_byte bSectorIndex = 0, bSectorNum;
	ia_word32 wResourceId = INVALID_SC_SMARTCARDID;

	/* The event list hasn't been allocate */

	ansi_memset((void *)&EmmTlvReply, 0, sizeof(msg_emm_tlv_notify_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
	{
		return ;
	}
	if (IA_ReadFromMessage(pvMsg, &EmmTlvReply, EmmTlvReply.bLength) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	if(EmmTlvReply.bLength)
	{
		CA_EMM_TLV_MESSAGE.bRequestType = REQUEST_TYPE_REPLY;
		CA_EMM_TLV_MESSAGE.wResourceId = EmmTlvReply.wResourceId;
		CA_EMM_TLV_MESSAGE.bLength = EmmTlvReply.bLength;
		abEMMTLV= (ia_byte *)ALLOCATE_MEMORY( unsigned long, EmmTlvReply.bLength );
		if (abEMMTLV == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_EMM_TLV: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}
	}
	else
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	if (IA_ReadFromMessage(pvMsg, &abEMMTLV, sizeof(msg_emm_tlv_notify_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	CA_EMM_TLV_MESSAGE.pstEMMTLV = abEMMTLV;
	bSectorNum = CA_SCARD_GetSectorNumber(wResourceId);
	if (bSectorNum == INVALID_SC_SECTOR)
	/* the smartcard id is invalidate */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* check the return value from scell */
	if (bSectorIndex >= bSectorNum)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/*
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_TLV_EMM) == IA_TRUE)
	{
		CA_CLIENT_Notify(wResourceId, CA_QC_EMM_TLV_NOTIFY, (ia_word32)(CA_EMM_TLV_MESSAGE.bLength) );
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
	*/
}

/********************* CA_EMM_TLVQueryControl *********************/
/* Description: Reply to Client :.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : wHandle   - .
*       pCAClientMsg - Query message;
*
** Returns : CA_QUERY_FAIL- Query/Reply failed.
*      CA_QUERY_SUCCESS-Query/Reply success
*********************************************************************/
CA_QUERY_STATUS_EN CA_EMM_TLVQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{

	CA_EMM_TLV_NOTIFY_T  *pcaEmmTlv = (CA_EMM_TLV_NOTIFY_T*)pCAClientMsg;
	CA_PARAM_NOT_USED( wHandle );
	if (pcaEmmTlv == 0)
	{
		return CA_QUERY_FAIL;
	}

	pcaEmmTlv->wResourceId =CA_EMM_TLV_MESSAGE.wResourceId;
	pcaEmmTlv->bLength =CA_EMM_TLV_MESSAGE.bLength;
	ansi_memcpy((void*)(pcaEmmTlv->pstEMMTLV), (void*) (CA_EMM_TLV_MESSAGE.pstEMMTLV),
	 CA_EMM_TLV_MESSAGE.bLength);

	if (CA_EMM_TLV_MESSAGE.pstEMMTLV != 0)
	{
		OS_DRV_FreeMemory((void *)(CA_EMM_TLV_MESSAGE.pstEMMTLV));
		CA_EMM_TLV_MESSAGE.pstEMMTLV = 0;
	}
	return CA_QUERY_SUCCESS;
}


void DMB_ServicesClose(ia_word32 wIndex, ia_word32 wSrvType)
{
	ia_msg_handle             *pvMsg;
	msg_service_close_data_st stSvcClose;
	ia_word16 wScellHandle;

	wScellHandle = CA_SVCINFO_GetSCellHandle(wIndex, wSrvType);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		return;
	}
	stSvcClose.wServiceHandle = wScellHandle;

	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_close_data_st) ,
		sizeof(msg_service_close_data_st) );

	CA_DBG_V((" cell handle = %d\n", stSvcClose.wServiceHandle ));

	IA_WriteToMessage( pvMsg, &stSvcClose, sizeof( msg_service_close_data_st ) );

	if( SCELL_Message( MSG_SERVICE_CLOSE, (ia_word16)wIndex, IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg) ) == IA_SUCCESS )
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage( pvMsg, IA_FALSE );
		CA_DBG_V(("send MSG_SERVICE_CLOSE success\n" ));
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}

	/*CA_SVCINFO_SetSCellHandle(wIndex, wSrvType, INVALID_SCELL_SERVICE_HANDLE);*/

}
void DMB_SVCINFO_ResetLocalSvcInfo(ia_word32 wIndex)
{
	if (0 == pstLocalGrpSvcInfo)
		return ;

	/*ansi_memset((void*) &pstLocalGrpSvcInfo[wIndex], 0, sizeof(CA_LOCAL_GROUP_SERVICE_INFO_T));*/
	/* reset the group service data */
	pstLocalGrpSvcInfo[wIndex].wGroupHandle = INVALID_GROUP_SERVICE_HANDLE;
	pstLocalGrpSvcInfo[wIndex].wDemuxId = INVALID_GROUP_SERVICE_HANDLE;
	pstLocalGrpSvcInfo[wIndex].wSmartCardId = INVALID_GROUP_SERVICE_HANDLE;
	pstLocalGrpSvcInfo[wIndex].wMacrovisionId = INVALID_GROUP_SERVICE_HANDLE;
	pstLocalGrpSvcInfo[wIndex].bStreamCount = 0;
	pstLocalGrpSvcInfo[wIndex].fFree = IA_TRUE;
	pstLocalGrpSvcInfo[wIndex].wEmmPid= 0;
	ResetBasicServiceInfo( &( pstLocalGrpSvcInfo[wIndex].astServices[CA_SERVICE_DMB_DESCRAMBLE] ) );


}

#endif

