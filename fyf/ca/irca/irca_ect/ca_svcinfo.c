/**
* $Id: ca_svcinfo.c,v 1.23 2008/10/15 08:41:46 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* The module handles service info related queries and operations.
**/

/******************************** Header Files ********************************/
/* #include <stdio.h> */
#include "ca_svcinfo.h"
#include <irca/ca_task.h>
#include "ca_error.h"
#include "ca_scard.h"

#include <irca/pubh/s3_service.h>
#include <irca/pubh/s3_ansi_functions.h>
#include <irca/pubh/s3_osdrv.h>

#ifdef INCL_SERVICE_DMB
#include "ca_dmbsvcinfo.h"
#endif

static CA_LOCAL_GROUP_SERVICE_INFO_T *pstLocalGrpSvcInfo = 0; /* store group service info */
static ia_word16 wMaxNumSrvGroups = 0;

/* jyou: 3.4.2 multiple smart cards */
/* the holder of demux information, each element is a pointer to a structure */
static CA_LOCAL_DEMUX_INFO_T* astLocalDemuxInfo[ MAX_DEMUX_COUNTS ];
static void ResetLocalDemuxInfo( ia_word32 wIndex );
static CA_LOCAL_BASIC_SERVICE_INFO_T* FindServiceBySCellHandle(ia_word16 wSCellHandle,ia_word16 *pwIndex0, ia_word16 *pwIndex1);
#if 0
/* jyou: quoted to remove warning in 820, yet reserved for future use */
static ia_bool isStatusIdentical( ia_status_st *pstStatus0, ia_status_st *pstStatus1 );
static void StatusCopy( ia_status_st *pstStatus0, ia_status_st *pstStatus1 );
#endif

/******************************* Function Implementations ********************/
void CA_SVCINFO_Initialise(ia_word16 wMaxNumServiceGroups)
{
	ia_word16 i;

	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */

	if (0 == wMaxNumServiceGroups)
		return ;

	if (0 == pstLocalGrpSvcInfo)
	{
		pstLocalGrpSvcInfo = (CA_LOCAL_GROUP_SERVICE_INFO_T*)OS_DRV_AllocateMemory((unsigned long)
			sizeof(CA_LOCAL_GROUP_SERVICE_INFO_T)*(unsigned long)wMaxNumServiceGroups);
	}
	if (0 == pstLocalGrpSvcInfo)
	{
		CA_ERROR(("CA_SVCINFO_Initialise Grp Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ;
	}

	/* jyou: 3.4.2 multiple smart cards */
	for ( i = 0 ; i < MAX_DEMUX_COUNTS ; i++ )
	{
		astLocalDemuxInfo[i] = 0;
	}

	for (i = 0; i < wMaxNumServiceGroups; i++)
	{
		CA_SVCINFO_ResetLocalSvcInfo(i);
	}

}

void CA_SVCINFO_SetSvcMaxNumber(ia_word16 wMaxNumServiceGroups)
{
	wMaxNumSrvGroups = wMaxNumServiceGroups;
}

ia_word16 CA_SVCINFO_GetSvcMaxNumber()
{
	return wMaxNumSrvGroups;
}

ia_bool CA_SVCINFO_GetGrpSvcStatus(ia_word32 wIndex)
{
	return pstLocalGrpSvcInfo[wIndex].fFree;
}

/*
wIndex = (demux_index)
*/
ia_bool CA_SVCINFO_GetEmmSvcStatus(ia_word32 wIndex)
{
	/* jyou: 3.4.2 multiple smart cards */
	if ( isValidDemuxIndex( wIndex ) == IA_TRUE )
	{
		return astLocalDemuxInfo[ wIndex ]->fFree;
	}
	else
	{
		return IA_FALSE;
	}
}

void CA_SVCINFO_ResetLocalSvcInfo(ia_word32 wIndex)
{
	int i = 0;
	if (0 == pstLocalGrpSvcInfo)
		return ;

	/* jyou: 3.4.2 multiple smart cards */
	ResetLocalDemuxInfo( wIndex );

	ansi_memset((void*) &pstLocalGrpSvcInfo[wIndex], 0, sizeof(CA_LOCAL_GROUP_SERVICE_INFO_T));
	/* reset the group service data */
	pstLocalGrpSvcInfo[wIndex].wGroupHandle = INVALID_GROUP_SERVICE_HANDLE;
	pstLocalGrpSvcInfo[wIndex].wDemuxId = INVALID_GROUP_SERVICE_HANDLE;
	pstLocalGrpSvcInfo[wIndex].wSmartCardId = INVALID_GROUP_SERVICE_HANDLE;
	pstLocalGrpSvcInfo[wIndex].wMacrovisionId = INVALID_GROUP_SERVICE_HANDLE;

	for (i = 0; i < CA_SERVICE_MAX_SERVICES; i++)
	{
		ResetBasicServiceInfo( &( pstLocalGrpSvcInfo[wIndex].astServices[i] ) );
	}
#ifdef INCL_SERVICE_DMB
	/*DMB Initialise*/
	for (i = 0; i < CA_SERVICE_MAX_SERVICES; i++)
	{
		pstLocalGrpSvcInfo[wIndex].astDMBServices[i].bMappingTableSubChId = INVALID_MAPPING_SUBCHID;
		pstLocalGrpSvcInfo[wIndex].astDMBServices[i].bTransportMode = INVALID_TRANSPORT_MODE;
		pstLocalGrpSvcInfo[wIndex].astDMBServices[i].bEmmSubChId = INVALID_MAPPING_SUBCHID;
		pstLocalGrpSvcInfo[wIndex].astDMBServices[i].wEmmPA = INVALID_PACKET_ADDRESS;
		pstLocalGrpSvcInfo[wIndex].astDMBServices[i].wEmmPid = 0x00;
	}

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
#endif

	pstLocalGrpSvcInfo[wIndex].bStreamCount = 0;

	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.wSessionResourceId = 0xFFFFFFFF;
	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.wRightsRecId =
		MSG_SM_DRM_RIGHTS_RECORD_ID_INVALID;
	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.wSequenceRecId =
		MSG_SM_DRM_SEQUENCE_RECORD_ID_INVALID;
	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.fNonPVRable = IA_FALSE;
	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.fMustExpire = IA_FALSE;
	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.eCopyControl =
		MSG_SM_DRM_COPYCONTROL_COPYFREELY;
	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.wPlaybackWindowStartTime = 0;
	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.wPlaybackWindowDuration = 0;
	pstLocalGrpSvcInfo[wIndex].stPvrRecordServiceInfo.bNrOfPlaybacks = 0;

	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.wSessionResourceId = 0xFFFFFFFF;
	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.wRightsRecId =
		MSG_SM_DRM_RIGHTS_RECORD_ID_INVALID;
	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.wSequenceRecId =
		MSG_SM_DRM_SEQUENCE_RECORD_ID_INVALID;
	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.fNonPVRable = IA_FALSE;
	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.fMustExpire = IA_FALSE;
	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.eCopyControl =
		MSG_SM_DRM_COPYCONTROL_COPYFREELY;
	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.wPlaybackWindowStartTime = 0;
	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.wPlaybackWindowDuration = 0;
	pstLocalGrpSvcInfo[wIndex].stPvrPlaybackServiceInfo.bNrOfPlaybacks = 0;
	pstLocalGrpSvcInfo[wIndex].fFree = IA_TRUE;
	pstLocalGrpSvcInfo[wIndex].wEmmPid= 0;

}

void CA_SVCINFO_MemoryFree()
{
	if (0 != pstLocalGrpSvcInfo)
	{
		FREE_MEMORY(pstLocalGrpSvcInfo);
		pstLocalGrpSvcInfo = 0;
	}
	/* jyou: 3.4.2 multiple smart cards */
	CA_SVCINFO_ReleaseAllDemuxes( );
}

void CA_SVCINFO_SetSvcToFree(ia_word32 wIndex, ia_word32 wSrvType)
{
	if (0 == pstLocalGrpSvcInfo)
		return ;

	if (CA_SERVICE_EMM == wSrvType)
	{
		/* jyou: 3.4.2 multiple smart cards */
		if ( isValidDemuxIndex( wIndex ) == IA_TRUE )
		{
			astLocalDemuxInfo[ wIndex ]->fFree = IA_TRUE;
		}
	}
	else
		pstLocalGrpSvcInfo[wIndex].fFree = IA_TRUE;

}

/*
*     ROUTINE : CA_SVCINFO_SetSvcHeaderInfo()
* DESCRIPTION : Set the default resource IDs to various resources
*       INPUT : wHandle - The EMMHandle(DemuxID) or GroupHandle passed by CA Client.
*				pstCaSvbGrpDefHeader - Container of the default resource IDs.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : Common to all service type:
*					the wDemuxId, wMacroVisionId, and wOOBId fields of the local storage will be set
*					fFree will be set to IA_FALSE
*				if ( wSrvType == CA_SERVICE_EMM )
*					field wEmmHandle is also set in astLocalDemuxInfo[wIndex]
*				else
*					field wGroupHandle is also set in pstLocalGrpSvcInfo[wIndex]
*   CALLED BY : CaProcessMsg
*      Author : Jane You
* Last Changed: September 19, 2006
*/
void CA_SVCINFO_SetSvcHeaderInfo(ia_word32 wHandle, CA_SERVICE_HEADER *pstCaSvcGrpDefHeader)
{
	ia_word32 wTempSrvType = 0;
	CA_LOCAL_DEMUX_INFO_T*	pTemp;
	ia_word16				i;

	wTempSrvType = pstCaSvcGrpDefHeader->wServiceType;

	/* Set the Service Info header */
	switch (wTempSrvType)
	{
	case CA_SERVICE_EMM:
		/* jyou: 3.4.2 multiple smart cards */
		i = CA_SVCINFO_FindDemux( wHandle );
		if ( i == INVALID_INDEX )
		{
			CA_ERROR(("CA_SVCINFO_SetSvcHeaderInfo: Unable to add a new demux %d.\n", wHandle));
			break;
		}
		pTemp = astLocalDemuxInfo[i];
		if ( pTemp == 0 )
		{
			break;
		}
		pTemp->fFree = IA_FALSE;
		pTemp->wEMMHandle = wHandle;
		pTemp->wDemuxId = pstCaSvcGrpDefHeader->wDemuxId;
		pTemp->wOOBId = pstCaSvcGrpDefHeader->wOOBId;
		/* jyou: SCR#26235 2007-08-07 moved the lines to first allocation of astServiceInfo */
				break;
#ifdef INCL_SERVICE_DMB
				case CA_SERVICE_DMB_EMM:
		i = CA_SVCINFO_FindDemux( wHandle );
		if ( i == INVALID_INDEX )
		{
			CA_ERROR(("CA_SVCINFO_SetSvcHeaderInfo: Unable to add a new demux %d.\n", wHandle));
			break;
		}
		pTemp = astLocalDemuxInfo[i];
		if ( pTemp == 0 )
		{
			break;
		}
		pTemp->fFree = IA_FALSE;
		pTemp->wEMMHandle = wHandle;
		pTemp->wDemuxId = pstCaSvcGrpDefHeader->wDemuxId;
		pTemp->wOOBId = pstCaSvcGrpDefHeader->wOOBId;
				break;
#endif
	case CA_SERVICE_DESCRAMBLE:
	case CA_SERVICE_RECORD:
	case CA_SERVICE_PLAYBACK:
#ifdef INCL_SERVICE_DMB
	case CA_SERVICE_DMB_DESCRAMBLE:
#endif

		if (0 == pstLocalGrpSvcInfo)
			return;
		pstLocalGrpSvcInfo[wHandle].wGroupHandle = pstCaSvcGrpDefHeader->wGroupHandle;
		pstLocalGrpSvcInfo[wHandle].wSmartCardId = pstCaSvcGrpDefHeader->wSmartCardId;
		pstLocalGrpSvcInfo[wHandle].wDemuxId = pstCaSvcGrpDefHeader->wDemuxId;
		pstLocalGrpSvcInfo[wHandle].wMacrovisionId = pstCaSvcGrpDefHeader->wMacrovisionId;
		pstLocalGrpSvcInfo[wHandle].astServices[wTempSrvType].wServiceType = wTempSrvType;

		if (IA_TRUE == pstLocalGrpSvcInfo[wHandle].fFree)
		{
			pstLocalGrpSvcInfo[wHandle].fFree = IA_FALSE;
		}
#ifdef INCL_SERVICE_DMB
		if(wTempSrvType == CA_SERVICE_DMB_DESCRAMBLE)
		{
			CA_DMB_ResetLocalECMServiceInfo(pstCaSvcGrpDefHeader->wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE);
		}
#endif
		break;
	default:
		CA_ERROR(("CA_SVCINFO_SetSvcHeaderInfo: Unrecognized service type %d.\n", wTempSrvType));
		break;
	}
	return;
}

/*
*     ROUTINE : CA_SVCINFO_GetSvcHeaderInfo()
* DESCRIPTION : Get the default resource IDs for various resources
*       INPUT : wIndex	- The EMMIndex(for EMM service) or GroupHandle passed by CA Client.
*				pstCaSvcHeader - Container of the default resource IDs.
*      OUTPUT : Default resource IDs stored in the local storage for the given demux or group.
*     RETURNS : IA_FAIL -- Unsuccessful
*				IA_SUCCESS
* SIDE EFFECT : None
*   CALLED BY : AddEcmSourceToDescrambleService
*      Author : Jane You
* Last Changed: September 19, 2006
*/
ia_result CA_SVCINFO_GetSvcHeaderInfo(ia_word32 wIndex, CA_SERVICE_HEADER *pstCaSvcHeader)
{
	CA_LOCAL_DEMUX_INFO_T*	pTemp;

#ifdef INCL_SERVICE_DMB
	/*Added by Andrew Hui for DMB 070511*/
	if ((CA_SERVICE_EMM == pstCaSvcHeader->wServiceType)||(CA_SERVICE_DMB_EMM == pstCaSvcHeader->wServiceType))
#else
	if (CA_SERVICE_EMM == pstCaSvcHeader->wServiceType)
#endif
	{
		/* jyou: 3.4.2 multiple smart cards */
		if ( wIndex == INVALID_INDEX )
		{
			CA_ERROR(("CA_SVCINFO_GetSvcHeaderInfo: Unable to find demux index %d.\n", wIndex));
			return IA_FAIL;
		}
		pTemp = astLocalDemuxInfo[wIndex];
		pstCaSvcHeader->wGroupHandle = pTemp->wEMMHandle;
		pstCaSvcHeader->wDemuxId = pTemp->wDemuxId;
		pstCaSvcHeader->wMacrovisionId = pTemp->wMacrovisionId;
		pstCaSvcHeader->wOOBId = pTemp->wOOBId;
	}
	else
	{
		if (0 == pstCaSvcHeader || 0 == pstLocalGrpSvcInfo )
			return IA_FAIL;

		pstCaSvcHeader->wGroupHandle = pstLocalGrpSvcInfo[wIndex].wGroupHandle;
		pstCaSvcHeader->wSmartCardId = pstLocalGrpSvcInfo[wIndex].wSmartCardId;
		pstCaSvcHeader->wDemuxId = pstLocalGrpSvcInfo[wIndex].wDemuxId;
		pstCaSvcHeader->wMacrovisionId = pstLocalGrpSvcInfo[wIndex].wMacrovisionId;
	}

	return IA_SUCCESS;
}

/*
*     ROUTINE : CA_SVCINFO_SetSCellHandle()
* DESCRIPTION : Store the corresponding SoftCell handle of a service to the local storage.
*				A service is only considered opened when its wSCellHandle is valid.
*       INPUT : wIndex	- The EMMHandle+SmartCardIndex or GroupHanle passed by the CA client.
*				wSrvType - The type of the service. One of
*					CA_SERVICE_EMM,
*					CA_SERVICE_DESCRAMBLE,
*					CA_SERVICE_RECORD,
*					CA_SERVICE_PLAYBACK
*				wSCellHandle - The corresponding SoftCell handle of a service.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : if ( wSrvType == CA_SERVICE_EMM )
*					astLocalDemuxInfo[wIndex>>8]->astEMMServices[wIndex&0x000000FF].wSCellHandle
*						 = wSCellHandle
*				else
*					pstLocalGrpSvcInfo[wIndex].astServices[wSrvType].wSCellHandle = wSCellHandle
*   CALLED BY : ParseServiceOpenReplyMsg
*      Author : Jane You
* Last Changed: September 19, 2006
*/
void CA_SVCINFO_SetSCellHandle(ia_word32 wIndex, ia_word32 wSrvType, ia_word16 wSCellHandle)
{
	ia_byte		i, j;
	CA_LOCAL_DEMUX_INFO_T *pstDemux;
	CA_LOCAL_BASIC_SERVICE_INFO_T	*pstBasicService;
#ifdef INCL_SERVICE_DMB
	/*Added by Andrew Hui for DMB 070510*/
	if ((CA_SERVICE_EMM == wSrvType)||(CA_SERVICE_DMB_EMM == wSrvType))
#else
	if (CA_SERVICE_EMM == wSrvType)
#endif
	{
		/* jyou: 3.4.2 multiple smart cards */
		i = (ia_byte)CA_LOCAL_GetUpperBytes( wIndex, 1 );
		j = (ia_byte)CA_LOCAL_GetLowerBytes( wIndex, 1 );
		if ( isValidDemuxIndex(i) == IA_FALSE || isValidSmartCardIndex(j) == IA_FALSE )
		{
			CA_ERROR(("SetSCellHandle: Invalid indices i=%d j=%d.\n", i, j));
			return;
		}
		pstDemux = astLocalDemuxInfo[i];
		pstBasicService = &(pstDemux->astServices[j]);
		pstDemux->fFree = IA_FALSE;
		pstBasicService->wSCellHandle = wSCellHandle;
		pstBasicService->wSmartCardId = CA_SCARD_GetSmartCardId(j);
		pstBasicService->wServiceType = wSrvType;
	}
	else
	{
		if (0 == pstLocalGrpSvcInfo)
			return ;

		pstLocalGrpSvcInfo[wIndex].astServices[wSrvType].wSCellHandle = wSCellHandle;
	}

}

/*
*     ROUTINE : CA_SVCINFO_GetSCellHandle()
* DESCRIPTION : Get the SoftCell returned service handle of the given service
*       INPUT : wHandle - The EMM_index+SmartCard_index or GroupHandle.
*				wSrvType - ECT Service Type.
*      OUTPUT : None
*     RETURNS : == INVALID_SCELL_SERVICE_HANDLE -- The service is not opened in SoftCell
*				!= INVALID_SCELL_SERVICE_HANDLE -- The corresponding service handle
* SIDE EFFECT : None
*   CALLED BY :
*      Author : Jane You
* Last Changed: September 19, 2006
*/
ia_word16 CA_SVCINFO_GetSCellHandle(ia_word32 wHandle, ia_word32 wSrvType)
{
	ia_word16				i, j;
#ifdef INCL_SERVICE_DMB
	/*Added by Andrew Hui for DMB 070510*/
	if ((CA_SERVICE_EMM == wSrvType)||(CA_SERVICE_DMB_EMM == wSrvType))
#else
	if (CA_SERVICE_EMM == wSrvType)
#endif
	{
		/* jyou: 3.4.2 multiple smart cards */
		i = CA_LOCAL_GetUpperBytes( wHandle, 1 );
		j = CA_LOCAL_GetLowerBytes( wHandle, 1 );
		if ( isValidDemuxIndex(i) == IA_FALSE || isValidSmartCardIndex(j) == IA_FALSE )
		{
			CA_ERROR(("GetSCellHandle: Invalid indices i=%d j=%d.\n", i, j));
			return INVALID_SCELL_SERVICE_HANDLE;
		}
		return astLocalDemuxInfo[i]->astServices[j].wSCellHandle;
	}
	else
	{
		if ( 0 == pstLocalGrpSvcInfo )
		{
			return INVALID_SCELL_SERVICE_HANDLE;
		}
		return pstLocalGrpSvcInfo[wHandle].astServices[wSrvType].wSCellHandle;
	}
}


void CA_SVCINFO_SetSCellHandleStatus(ia_word32 wIndex, ia_word32 wSrvType, CA_SERVICE_HANDLE_STATUS_EN eSCellHandleStatus)
{
	ia_byte		i, j;
	CA_LOCAL_DEMUX_INFO_T *pstDemux;
	CA_LOCAL_BASIC_SERVICE_INFO_T	*pstBasicService;
#ifdef INCL_SERVICE_DMB
	/*Added by Andrew Hui for DMB 070510*/
	if ((CA_SERVICE_EMM == wSrvType)||(CA_SERVICE_DMB_EMM == wSrvType))
#else
	if (CA_SERVICE_EMM == wSrvType)
#endif
	{
		/* jyou: 3.4.2 multiple smart cards */
		i = (ia_byte)CA_LOCAL_GetUpperBytes( wIndex, 1 );
		j = (ia_byte)CA_LOCAL_GetLowerBytes( wIndex, 1 );
		if ( isValidDemuxIndex(i) == IA_FALSE || isValidSmartCardIndex(j) == IA_FALSE )
		{
			CA_ERROR(("SetSCellHandleStatus: Invalid indices i=%d j=%d.\n", i, j));
			return;
		}
		pstDemux = astLocalDemuxInfo[i];
		pstBasicService = &(pstDemux->astServices[j]);
		pstBasicService->eSvcHandleStatus = eSCellHandleStatus;
		
	}
	else
	{
		if (0 == pstLocalGrpSvcInfo)
			return ;

		pstLocalGrpSvcInfo[wIndex].astServices[wSrvType].eSvcHandleStatus = eSCellHandleStatus;
	}

}

CA_SERVICE_HANDLE_STATUS_EN CA_SVCINFO_GetSCellHandleStatus(ia_word32 wHandle, ia_word32 wSrvType)
{
	ia_word16				i, j;
#ifdef INCL_SERVICE_DMB
	/*Added by Andrew Hui for DMB 070510*/
	if ((CA_SERVICE_EMM == wSrvType)||(CA_SERVICE_DMB_EMM == wSrvType))
#else
	if (CA_SERVICE_EMM == wSrvType)
#endif
	{
		/* jyou: 3.4.2 multiple smart cards */
		i = CA_LOCAL_GetUpperBytes( wHandle, 1 );
		j = CA_LOCAL_GetLowerBytes( wHandle, 1 );
		if ( isValidDemuxIndex(i) == IA_FALSE || isValidSmartCardIndex(j) == IA_FALSE )
		{
			CA_ERROR(("GetSCellHandle: Invalid indices i=%d j=%d.\n", i, j));
			return CA_SERVICE_HANDLE_EMPTY;
		}
		return astLocalDemuxInfo[i]->astServices[j].eSvcHandleStatus;
	}
	else
	{
		if ( 0 == pstLocalGrpSvcInfo )
		{
			return CA_SERVICE_HANDLE_EMPTY;
		}
		return pstLocalGrpSvcInfo[wHandle].astServices[wSrvType].eSvcHandleStatus;
	}
}

/*
*     ROUTINE : CA_SVCINFO_HandleSCell2CaClient()
* DESCRIPTION : For a SoftCell service handle, get the corresponding EMMHandle or GroupHandle
*       INPUT : wServiceHandle - The SoftCell service handle.
*      OUTPUT : None
*     RETURNS : == INVALID_GROUP_SERVICE_HANDLE
*                         -- The service is not opened in SoftCell
*				Otherwise -- The corresponding demux_index or GroupHandle
* SIDE EFFECT : None
*   CALLED BY : CA_IPPV_EventNotify, BuildCaMailAnnounce, IRD_MSG_Parser
*      Author : Jane You
* Last Changed: September 19, 2006
*/
/* Fixed 27574 CA_SVCINFO_HandleSCell2CaClient not returning the CaClient Handle. */
ia_word32 CA_SVCINFO_HandleSCell2CaClient(ia_word16 wServiceHandle)
{
   ia_word32 wRet = INVALID_GROUP_SERVICE_HANDLE;
   /* ayang: 3.6.5 2008-04-09 Fixed SCR27025 */
   ia_byte        i, j;
   ia_word16    wGrpNum;

   /* Search the EMM service info storage for matching service */
   for ( i = 0; i < MAX_DEMUX_COUNTS ; i++ )
   {
       if ( astLocalDemuxInfo[i] != 0 )
       {
           for ( j = 0 ; j < SC_SLOT_MAX ; j++ )
           {
               if (astLocalDemuxInfo[i]->astServices[j].wSCellHandle == wServiceHandle)
               {
                   return astLocalDemuxInfo[i]->wEMMHandle;
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
               if (pstLocalGrpSvcInfo[i].astServices[j].wSCellHandle == wServiceHandle)
               {
                   return pstLocalGrpSvcInfo[i].wGroupHandle;
               }
           }
       }
   }

   return wRet;
}

/*
*     ROUTINE : CA_SVCINFO_HandleSCell2CaClient()
* DESCRIPTION : For a SoftCell service handle, get the corresponding EMMHandle or GroupHandle
*       INPUT : wServiceHandle - The SoftCell service handle.
*      OUTPUT : None
*     RETURNS : == INVALID_SERVICETYPE -- The service is not opened in SoftCell
*				!= INVALID_SERVICETYPE -- The corresponding service type
* SIDE EFFECT : None
*   CALLED BY :
*      Author : Jane You
* Last Changed: September 19, 2006
*/
ia_word32 CA_SVCINFO_GetSvcTypeFromSCellHandle(ia_word16 wScellHandle)
{
	/* jyou: 3.4.2 multiple smart card */
	ia_word16 wIndex0, wIndex1;
	CA_LOCAL_BASIC_SERVICE_INFO_T *pTemp;

	pTemp = FindServiceBySCellHandle( wScellHandle, &wIndex0, &wIndex1 );

	if ( pTemp == 0 )
	{
		return INVALID_SERVICETYPE;
	}
	else
	{
		return pTemp->wServiceType;
	}
}

/*
*     ROUTINE : CA_SVCINFO_GetSvcInfoPtr()
* DESCRIPTION : Get the pointer to the local Group Service storage.
*       INPUT : wIndex - The index (==group handle) of the local storage.
*				wSrvType - The service types can be defined in a service group.
*      OUTPUT : None
*     RETURNS : !=0 -- pointer to the local storage of the service group info
*				==0 -- such a service group does not exist
* SIDE EFFECT : None
*   CALLED BY : CA_SVCINFO_ParseDescrambleServiceInfo, CA_SVCINFO_ParsePVRServiceInfo,
*				AddResourceToPvrRecordServiceForDrm, DeleteAllResource
*      Author : Jane You
* Last Changed: September 19, 2006
*/
void *CA_SVCINFO_GetSvcInfoPtr(ia_word32 wIndex, ia_word32 wSrvType)
{
	/* jyou: 3.4.2 multiple smart card */
	if ( (0 == pstLocalGrpSvcInfo) || (wIndex >=wMaxNumSrvGroups) )
		return 0;

	if ( wSrvType < CA_SERVICE_MAX_SERVICES )
	{
		return (void*) &pstLocalGrpSvcInfo[wIndex];
	}

	return 0;
}

/*
*     ROUTINE : CA_SVCINFO_GetEMMSvcInfoPtr()
* DESCRIPTION : Get the pointer to the local EMM Service storage.
*       INPUT : wSCellHandle - The SoftCell service handle.
*				pwIndex - pointer to the buffer of the EMM service index
*      OUTPUT : The index of the demux that contains the EMM service in the local storage
*     RETURNS : !=0 -- pointer to the EMM service found
*				==0 -- there is no EMM service matching the given SoftCell service handle.
* SIDE EFFECT : None
*   CALLED BY : CA_SVCINFO_ParseEMMServiceInfo, DeleteAllResource
*      Author : Jane You
* Last Changed: September 19, 2006
*/
CA_LOCAL_BASIC_SERVICE_INFO_T* CA_SVCINFO_GetEMMSvcInfoPtr( ia_word16 wSCellHandle, ia_word16 *pwIndex0, ia_word16 *pwIndex1 )
{
	/* jyou: 3.4.2 multiple smart card */
	return FindServiceBySCellHandle( wSCellHandle, pwIndex0, pwIndex1 );
}

/*
*     ROUTINE : CA_SVCINFO_GetEMMServiceByOpSeqNo()
* DESCRIPTION :
*       INPUT : wOpSeqNo - emm_index<<8 | smartcard_index
*      OUTPUT :
*     RETURNS : !=0 -- the resource ID of the smart card defined for the
*											given EMM service, which is identified by wOpSeqNo
*				==0 -- no corresponding EMM service exist.
* SIDE EFFECT : None
*   CALLED BY : RemoveDefaultResourcesFromEmmService
*      Author : Jane You
* Last Changed: September 20, 2006
*/
CA_LOCAL_BASIC_SERVICE_INFO_T* CA_SVCINFO_GetEMMServiceByOpSeqNo( ia_word16 wOpSeqNo )
{
	ia_word16				i, j;

	/* jyou: 3.4.2 multiple smart cards */
	i = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	j = CA_LOCAL_GetLowerBytes( wOpSeqNo, 1 );
	if ( isValidDemuxIndex(i) == IA_FALSE || isValidSmartCardIndex(j) == IA_FALSE )
	{
		CA_ERROR(("GetSCellHandle: Invalid indices i=%d j=%d.\n", i, j));
		return 0;
	}
	return &(astLocalDemuxInfo[i]->astServices[j]);
}

/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
CA_LOCAL_BASIC_SERVICE_INFO_T* CA_SVCINFO_GetDescServiceByGroupHandle( ia_word32 wHandle )
{
	if ( (0 == pstLocalGrpSvcInfo) || (wHandle >=wMaxNumSrvGroups) )
	{
		return 0;
	}

	return &(pstLocalGrpSvcInfo[wHandle].astServices[CA_SERVICE_DESCRAMBLE]);
}
/* jyou: 3.4.3.7 SCR#25552 2007-06-06 end */

void CA_SVCINFO_EmmSvcNotify(ia_word32 wHandle)
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_EMM_SVC_INFO) == IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_EMM_SERVICE_INFO, wHandle);
	}
}

void CA_SVCINFO_GrpSvcNotify(ia_word32 wCaClientHandle, ia_word32 wSrvType)
{
	if ((CA_TASK_CheckNotifyEnable(NOTIFY_MASK_DES_SVC_INFO) == IA_TRUE) ||
		(CA_TASK_CheckNotifyEnable(NOTIFY_MASK_PVR_SVC_INFO) == IA_TRUE))
	{
		CA_CLIENT_Notify((wCaClientHandle), CA_QC_SERVICE_GROUP_INFO, (ia_uint32)wSrvType);
	}
}

void CA_SVCINFO_MRPinNotify(ia_word32 wCaClientHandle, ia_word32 wSrvType)
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_MR_TRIGGER_PIN) == IA_TRUE)
	{
		CA_CLIENT_Notify((wCaClientHandle), CA_QC_MR_TRIGGER_PIN, (ia_uint32)wSrvType);
	}
}

/*
* Description: Query control for CA_QC_EMM_SERVICE_INFO type.
*
* Parameters : wHandle      - demux_index << 16 | smartcard_index .
*              pCAClientMsg - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_SVCINFO_EmmSvcInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_word16 i, j;
	CA_EMM_SERVICE_INFO_T *pstTemp;
	CA_LOCAL_DEMUX_INFO_T *pstLocalDemuxInfo;
	CA_LOCAL_BASIC_SERVICE_INFO_T *pstLocalBasicInfo;

	CA_PARAM_NOT_USED( wHandle );
	pstTemp = ( CA_EMM_SERVICE_INFO_T* ) pCAClientMsg;
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

	/* Copy local data to the output buffer--pstTemp */
	pstTemp->wEmmHandle = pstLocalDemuxInfo->wEMMHandle;
	pstTemp->wEmmPid = pstLocalDemuxInfo->wEMMPid;

	pstTemp->wServiceHandle = pstLocalBasicInfo->wSCellHandle;
	pstTemp->stGlobalStatus = pstLocalBasicInfo->stGlobalStatus;
#ifndef TO_SOFTCELL_3_0
	pstTemp->stServiceStatus = pstLocalBasicInfo->stServiceStatus;
#endif
	pstTemp->fEmmServiceDefine = pstLocalDemuxInfo->fServiceDefined;/*QQQ Fix SCR#26068*/

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

/*
* Description: Query control for CA_QC_DESCRAMBLE_SERVICE_INFO type.
*
* Parameters : wDemux        - Demux channel number.
*              pstDesSvcInfo - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_SVCINFO_SvcInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_word16 i;
	ia_word32 wGroupHandle, wSrvType = 0;
	ia_word16 wSrvHandle;
	SERVICE_GROUP_INFO_T *pstDesSvcInfo;

	CA_PARAM_NOT_USED( wHandle );
	pstDesSvcInfo = ( SERVICE_GROUP_INFO_T* ) pCAClientMsg;
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
	/* service type is inputed by client */
	if ( SERVICE_DVB_DESCRAMBLE == pstDesSvcInfo->wServiceType)
		wSrvType = CA_SERVICE_DESCRAMBLE;
	if (SERVICE_PVR_RECORD == pstDesSvcInfo->wServiceType)
		wSrvType = CA_SERVICE_RECORD;
	if (SERVICE_PVR_PLAYBACK == pstDesSvcInfo->wServiceType)
		wSrvType = CA_SERVICE_PLAYBACK;

	wSrvHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, wSrvType);
	if (wSrvHandle == INVALID_SCELL_SERVICE_HANDLE)
	{
		CA_ERROR(("The softcell handle isn't open for Group 0x%04x.\n", wGroupHandle));
		return CA_QUERY_FAIL;
	}

	pstDesSvcInfo->wServiceHandle = wSrvHandle;

	pstDesSvcInfo->stGlobalStatus =
		pstLocalGrpSvcInfo[wGroupHandle].astServices[wSrvType].stGlobalStatus;
#ifndef TO_SOFTCELL_3_0
	pstDesSvcInfo->stServiceStatus =
		pstLocalGrpSvcInfo[wGroupHandle].astServices[wSrvType].stServiceStatus;
#endif

	pstDesSvcInfo->bStreamCount = pstLocalGrpSvcInfo[wGroupHandle].bStreamCount;
	for (i = 0; i < pstLocalGrpSvcInfo[wGroupHandle].bStreamCount; i++)
	{
		pstDesSvcInfo->astStreamInfo[i].wStreamPid =
			pstLocalGrpSvcInfo[wGroupHandle].astStreamInfo[i].wStreamPid;
		pstDesSvcInfo->astStreamInfo[i].wEcmPid =
			pstLocalGrpSvcInfo[wGroupHandle].astStreamInfo[i].wEcmPid;
		pstDesSvcInfo->astStreamInfo[i].stStatus =
			pstLocalGrpSvcInfo[wGroupHandle].astStreamInfo[i].stStatus;
	}

	pstDesSvcInfo->bRscCount = pstLocalGrpSvcInfo[wGroupHandle].astServices[wSrvType].bRscCount;
	for (i = 0; i < pstLocalGrpSvcInfo[wGroupHandle].astServices[wSrvType].bRscCount; i++)
	{
		pstDesSvcInfo->awRscType[i] =
			pstLocalGrpSvcInfo[wGroupHandle].astServices[wSrvType].awRscType[i]; /* resource type */

		pstDesSvcInfo->astRscStatus[i].eSource =
			pstLocalGrpSvcInfo[wGroupHandle].astServices[wSrvType].astRscStatus[i].eSource; /*
																							resource status */
		pstDesSvcInfo->astRscStatus[i].wStatus =
			pstLocalGrpSvcInfo[wGroupHandle].astServices[wSrvType].astRscStatus[i].wStatus;
		pstDesSvcInfo->astRscStatus[i].eSeverity =
			pstLocalGrpSvcInfo[wGroupHandle].astServices[wSrvType].astRscStatus[i].eSeverity;
	}

	if (CA_SERVICE_RECORD == wSrvType)
	{
		pstDesSvcInfo->stCaPvrServiceInfo = pstLocalGrpSvcInfo[wGroupHandle].stPvrRecordServiceInfo;
	}
	else
	{
		pstDesSvcInfo->stCaPvrServiceInfo =
		pstLocalGrpSvcInfo[wGroupHandle].stPvrPlaybackServiceInfo;

#ifdef COPY_CONTROL_SUBSTATUS_AVAILABLE
		pstDesSvcInfo->stCopyControlInfo.wTypeMask=pstLocalGrpSvcInfo[wGroupHandle].stCopyControlInfo[wSrvType].wTypeMask;
		pstDesSvcInfo->stCopyControlInfo.wLength=pstLocalGrpSvcInfo[wGroupHandle].stCopyControlInfo[wSrvType].wLength;

		if((pstDesSvcInfo->stCopyControlInfo.wTypeMask&MSG_COPYCONTROL_MACROVISION)!=0)
		{
			pstDesSvcInfo->stCopyControlInfo.bMode=pstLocalGrpSvcInfo[wGroupHandle].stCopyControlInfo[wSrvType].bMode;
		}

		if((pstDesSvcInfo->stCopyControlInfo.wTypeMask&MSG_COPYCONTROL_DIGITAL)!=0)
		{
			pstDesSvcInfo->stCopyControlInfo.eCopyControlInfoDigital=pstLocalGrpSvcInfo[wGroupHandle].stCopyControlInfo[wSrvType].eCopyControlInfoDigital;
		}

		if((pstDesSvcInfo->stCopyControlInfo.wTypeMask&MSG_COPYCONTROL_ANALOG)!=0)
		{
			pstDesSvcInfo->stCopyControlInfo.eCopyControlInfoAnalog=pstLocalGrpSvcInfo[wGroupHandle].stCopyControlInfo[wSrvType].eCopyControlInfoAnalog;
		}
#endif
	}
	return CA_QUERY_SUCCESS;
}


ia_word32 CA_SVCINFO_GetRecIdFromGroupHandle(ia_word32 wGroupHandle)
{
	if (0 == pstLocalGrpSvcInfo)
	{
		return INVALID_PVR_ID;
	}

	return pstLocalGrpSvcInfo[wGroupHandle].stPvrRecordServiceInfo.wSessionResourceId;
}

ia_word32 CA_SVCINFO_GetPlaybackIdFromGroupHandle(ia_word32 wGroupHandle)
{
	if (0 == pstLocalGrpSvcInfo)
	{
		return INVALID_PVR_ID;
	}

	return pstLocalGrpSvcInfo[wGroupHandle].stPvrPlaybackServiceInfo.wSessionResourceId;
}

void CA_SVCINFO_SetPvrMapInfo(ia_word32 wService, CA_SERVICE_HEADER *pstCaSvcHeader, ia_word32
								wSessionResourceId)
{
	if (pstCaSvcHeader == 0)
		return ;
	/* set the session resource ID */
	if (CA_SERVICE_RECORD == pstCaSvcHeader->wServiceType)
	{
		pstLocalGrpSvcInfo[wService].stPvrRecordServiceInfo.wSessionResourceId = wSessionResourceId;
		pstLocalGrpSvcInfo[wService].astServices[CA_SERVICE_RECORD].wServiceType =
			CA_SERVICE_RECORD;
	}
	else
	{
		pstLocalGrpSvcInfo[wService].stPvrPlaybackServiceInfo.wSessionResourceId =
			wSessionResourceId;
		pstLocalGrpSvcInfo[wService].astServices[CA_SERVICE_PLAYBACK].wServiceType =
			CA_SERVICE_PLAYBACK;
	}
	if (pstLocalGrpSvcInfo[wService].fFree == IA_TRUE)
		pstLocalGrpSvcInfo[wService].fFree = IA_FALSE;

}

/*
*     ROUTINE : CA_SVCINFO_ParseEMMServiceInfo()
* DESCRIPTION :
*       INPUT : ia_word32 wServiceType
*      OUTPUT : None
*     RETURNS :
* SIDE EFFECT :
*   CALLED BY :
*      Author : Shawn Ren
* Last Changed: September 12, 2006
*/
ia_bool CA_SVCINFO_ParseEMMServiceInfo(msg_service_status_data_st *pstSvcStatusData, DEMUX_INFO_ST *pstDMX,ia_msg_handle pvMsg)
{
	CA_LOCAL_BASIC_SERVICE_INFO_T *pstEmmSvc = 0;
#ifdef INCL_SERVICE_DMB
	CA_LOCAL_BASIC_DMB_INFO_T*pstDMBEmmSvc = 0;
#endif
	msg_rsc_status_st stRscStatus;
	ia_word16 i, wResCount = 0;
	ia_bool fMessageClose=IA_FALSE;
	CA_LOCAL_DEMUX_INFO_T	*pstLocalDemuxInfo;
	ia_word16				wSCellHandle;
	ia_word16				wIndex0, wIndex1;	/* demux and smart card index */

	CA_DBG_V(("Parse EMM service status.\n"));

	/* Read out the softcell service handle */
	wSCellHandle = pstSvcStatusData->wServiceHandle;
	/* Get the reference to the corresponding EMM service in the local storage */
	pstEmmSvc = (CA_LOCAL_BASIC_SERVICE_INFO_T*)CA_SVCINFO_GetEMMSvcInfoPtr(wSCellHandle, &wIndex0, &wIndex1);
	if (pstEmmSvc == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		fMessageClose=IA_TRUE;
		return fMessageClose;
	}
	/* Assign the pointer of the demux who contains the EMM service */
	pstLocalDemuxInfo = astLocalDemuxInfo[wIndex0];

	/* save the status in the local emm service info database */
	pstEmmSvc->stGlobalStatus = pstSvcStatusData->stStatus;

#ifndef TO_SOFTCELL_3_0 /*only for softcell 3.1.x or higher*/
	pstEmmSvc->stServiceStatus = pstSvcStatusData->stServiceStatus;
#endif

	/*Andrew Hui added for DMB 070514*/
	if(pstEmmSvc->wServiceType==CA_SERVICE_EMM){
		if (pstEmmSvc->stGlobalStatus.wStatus == 100)
		{
			pstLocalDemuxInfo->fServiceDefined = IA_TRUE;
		}
		/* before get the PMT, we can't judge the global status. so we just send the status tell the maufacture not to display */
		if (pstDMX[wIndex0].fGetPMT == IA_FALSE)
		{
			if (pstEmmSvc->stGlobalStatus.wStatus != 100) /*avoid displaying E101-5*/
			{
				pstEmmSvc->stGlobalStatus.eSource = SOURCE_SMARTCARD;
				pstEmmSvc->stGlobalStatus.wStatus = 34;
				pstEmmSvc->stGlobalStatus.eSeverity = STATUS_INFO;
			}
		}
	}

#ifdef INCL_SERVICE_DMB
	/*Andrew Hui added for DMB 070514*/
	if(pstEmmSvc->wServiceType==CA_SERVICE_DMB_EMM)
	{
		pstDMBEmmSvc= (CA_LOCAL_BASIC_DMB_INFO_T*)CA_SVCINFO_GetDMBEMMSvcInfoPtr(wSCellHandle, &wIndex0, &wIndex1);
		if (pstEmmSvc == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			fMessageClose=IA_TRUE;
			return fMessageClose;
		}
	}
#endif
	for (i = 0; i < pstSvcStatusData->bResourceCount; i++)
	{
		IA_ReadFromMessage( pvMsg, &stRscStatus, sizeof(stRscStatus) );

		switch( stRscStatus.wType )
		{
		case RESOURCE_EMM_SOURCE:
			{
				msg_emm_source_status_st stEmmSrcStatus;

				ansi_memset((void *)&stEmmSrcStatus, 0, sizeof (stEmmSrcStatus));
				IA_ReadFromMessage( pvMsg, &stEmmSrcStatus, sizeof(stEmmSrcStatus) );
				/* If no valid EMM PID could be found then this will hold the
				value IA_INVALID_DVB_PID. */
				CA_DBG_V(("EMM PID = 0x%04x\n", stEmmSrcStatus.wEmmPid));
				pstLocalDemuxInfo->wEMMPid = stEmmSrcStatus.wEmmPid;

				/* save the resource type and resource status */
				pstEmmSvc->awRscType[wResCount] = stRscStatus.wType;
				pstEmmSvc->astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstEmmSvc->astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstEmmSvc->astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1;
				break;
			}
#ifdef INCL_SERVICE_DMB
		case RESOURCE_DMB_EMM:
			{
				msg_dmb_emm_streamid_st stEmmSrcStatus;
				ansi_memset((void *)&stEmmSrcStatus, 0, sizeof (msg_dmb_emm_streamid_st));
				IA_ReadFromMessage( pvMsg, &stEmmSrcStatus, sizeof(msg_dmb_emm_streamid_st) );

				CA_DBG_V(("EMM service status EMM PA = 0x%04x\n", stEmmSrcStatus.wEmmPA));
				CA_DBG_V(("EMM service status EMM subChId = 0x%02x\n", stEmmSrcStatus.bEmmSubChId));
				pstDMBEmmSvc->bEmmSubChId = stEmmSrcStatus.bEmmSubChId;
				pstDMBEmmSvc->wEmmPA = stEmmSrcStatus.wEmmPA;
				/* save the resource type and resource status */
				pstEmmSvc->awRscType[wResCount] = stRscStatus.wType;
				pstEmmSvc->astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstEmmSvc->astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstEmmSvc->astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1;
				break;
			}
#endif
		case RESOURCE_SMARTCARD:
			/* save the resource type and resource status */
			pstEmmSvc->awRscType[wResCount] = stRscStatus.wType;
			pstEmmSvc->astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
			pstEmmSvc->astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
			pstEmmSvc->astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
			wResCount +=1;
			break;

		case RESOURCE_MONITORING:
			/* save the resource type and resource status */
			pstEmmSvc->awRscType[wResCount] = stRscStatus.wType;
			pstEmmSvc->astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
			pstEmmSvc->astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
			pstEmmSvc->astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
			wResCount +=1;
			break;

		case RESOURCE_DVB_OOB:
			/* save the resource type and resource status */
			pstEmmSvc->awRscType[wResCount] = stRscStatus.wType;
			pstEmmSvc->astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
			pstEmmSvc->astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
			pstEmmSvc->astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
			wResCount +=1;
			break;

		default:
			CA_ERROR(("Unknown rsc type %d.\n", stRscStatus.wType ));
			break;
		}
	}

	pstEmmSvc->bRscCount = (ia_byte)wResCount ;

	/*Andrew Hui added for DMB 070514*/
	if(pstEmmSvc->wServiceType==CA_SERVICE_EMM)
	{
		if( pstLocalDemuxInfo->wEMMPid == IA_INVALID_DVB_PID || pstLocalDemuxInfo->wEMMPid == 0 )
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			fMessageClose=IA_TRUE;
			return fMessageClose;
		}

		CA_SVCINFO_EmmSvcNotify( CA_LOCAL_MakeWord32( wIndex0, wIndex1 ) );
			return fMessageClose;
	}
#ifdef INCL_SERVICE_DMB
	/*Andrew Hui added for DMB 070514*/
	if(pstEmmSvc->wServiceType==CA_SERVICE_DMB_EMM)
	{
		CA_DMB_EmmSvcNotify( CA_LOCAL_MakeWord32( wIndex0, wIndex1 ) );
		return fMessageClose;
	}
#endif
	return fMessageClose;
}

ia_bool CA_SVCINFO_ParseDescrambleServiceInfo(ia_word32 wServiceType,ia_word32 wGroupHandle,msg_service_status_data_st* pstSvcStatusData,SERVICE_INFO_ST *pstService,ia_msg_handle pvMsg)
{
	CA_LOCAL_GROUP_SERVICE_INFO_T *pstGrpSvc = 0;
	msg_rsc_status_st stRscStatus;
	msg_copycontrol_substatus_st stCopyControlSubStatus;
	ia_word16 i,j, wResCount = 0, wTypeMask;
	ia_bool fMessageClose=IA_FALSE;
	vd_macrovision_mode_st stMacroVisionMode;
	vd_copycontrol_digital_cci_st stCopyControlDigitalCci;
	vd_copycontrol_analog_cci_st stCopyControlAnalogCci;
	ia_word16 wMRFlag = 0, wSCFlag = 0;

	ansi_memset((void *)&stRscStatus, 0, sizeof(stRscStatus));
	ansi_memset((void *)&stCopyControlSubStatus, 0, sizeof(stCopyControlSubStatus));
	ansi_memset((void *)&stMacroVisionMode, 0, sizeof(stMacroVisionMode));
	ansi_memset((void *)&stCopyControlDigitalCci, 0, sizeof(stCopyControlDigitalCci));
	ansi_memset((void *)&stCopyControlAnalogCci, 0, sizeof(stCopyControlAnalogCci));

	/* Set wSCFlag when SOURCE_SMARTCARD:E136/E137 */
	if(CA_SERVICE_DESCRAMBLE==wServiceType)
	{
		CA_DBG_V(("Descramble service status.\n"));
		pstGrpSvc = (CA_LOCAL_GROUP_SERVICE_INFO_T*)CA_SVCINFO_GetSvcInfoPtr(wGroupHandle, CA_SERVICE_DESCRAMBLE);
		if (pstGrpSvc == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			fMessageClose=IA_TRUE;
			return fMessageClose;
		}
		if (pstService[wGroupHandle].fClear == IA_TRUE)
		{
			pstSvcStatusData->stStatus.wStatus = 100;
			pstSvcStatusData->stStatus.eSource = SOURCE_DVB_DESCRAMBLE_SERVICE ;
			pstSvcStatusData->stStatus.eSeverity = STATUS_INFO_DND ;
		}
		pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].stGlobalStatus = pstSvcStatusData->stStatus;

		if ((pstSvcStatusData->stStatus.eSource == SOURCE_SMARTCARD)
			&&(pstSvcStatusData->stStatus.eSeverity == STATUS_ERROR)
			&&((pstSvcStatusData->stStatus.wStatus == 136)||(pstSvcStatusData->stStatus.wStatus == 137)))
		{
			wSCFlag = pstSvcStatusData->stStatus.wStatus;
		}

#ifndef TO_SOFTCELL_3_0 /*only for softcell 3.1.x or higher*/
		pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].stServiceStatus = pstSvcStatusData->stServiceStatus;
#endif

		for (i = 0; i < pstSvcStatusData->bResourceCount; i++)
		{
			IA_ReadFromMessage( pvMsg, &stRscStatus, sizeof(stRscStatus) );

			switch( stRscStatus.wType )
			{
			case RESOURCE_ECM_SOURCE:
				{
					msg_ecm_source_status_st stEcmSrcStatus;
					msg_stream_status_st stStreamStatus;

					IA_ReadFromMessage( pvMsg, &stEcmSrcStatus, sizeof(stEcmSrcStatus) );
					pstGrpSvc->bStreamCount = stEcmSrcStatus.bStreamCount;

					for (j = 0; j < stEcmSrcStatus.bStreamCount; j++)
					{
						IA_ReadFromMessage( pvMsg, &stStreamStatus, sizeof(stStreamStatus) );
						/* If no valid ECM PID could be found then this will hold the
						value IA_INVALID_DVB_PID. */
						CA_DBG_V(("ES PID = 0x%04x, ECM PID = 0x%04x, source = %d, Status = %d, Severity = %d\n",
							stStreamStatus.wEsPid,
							stStreamStatus.wEcmPid,
							stStreamStatus.stStatus.eSource,
							stStreamStatus.stStatus.wStatus,
							stStreamStatus.stStatus.eSeverity));

						pstGrpSvc->astStreamInfo[j].wStreamPid = stStreamStatus.wEsPid;
						pstGrpSvc->astStreamInfo[j].wEcmPid    = stStreamStatus.wEcmPid;
						pstGrpSvc->astStreamInfo[j].stStatus   = stStreamStatus.stStatus;
#ifdef TO_SOFTCELL_3_0
						/*SoftCell3.0 does not caculate the severity among the global status and modules' status,
						ECT should do this, but the later version of SoftCell has done this internal, must not do it
						again in ECT, or there will be error for multiple smartcard.
						logged to SCR#25349, on Nov 15, 2006
						*/
						if(stStreamStatus.stStatus.eSource == SOURCE_SMARTCARD ||
							stStreamStatus.stStatus.eSource == SOURCE_SERVICES ||
							stStreamStatus.stStatus.eSource == SOURCE_SOFTCELL ||
							stRscStatus.stStatus.eSource == SOURCE_DVB_DESCRAMBLE_SERVICE)
						{
							if(pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].stGlobalStatus.eSeverity >= stStreamStatus.stStatus.eSeverity)
							{
								pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].stGlobalStatus = stStreamStatus.stStatus;
							}
						}
#endif
					}
					/* save the resource type and resource status */
					pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
					pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
					pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
					pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
					wResCount +=1;
					break;
				}
			case RESOURCE_DVB_DESCRAMBLER:
				/* save the resource type and resource status */
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				break;

			case RESOURCE_SMARTCARD:
				/* save the resource type and resource status */
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				break;

			case RESOURCE_MONITORING:
				/* save the resource type and resource status */
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				break;

			case RESOURCE_MATURITY_RATING:
				/* save the resource type and resource status */
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				/* Set wMRFlag when SOURCE_MOD_MATURITY_RATING:D100/D101 */
				if ((stRscStatus.stStatus.eSource == SOURCE_MOD_MATURITY_RATING)
					&& (stRscStatus.stStatus.eSeverity == STATUS_INFO_DND))
				{
					wMRFlag = stRscStatus.stStatus.wStatus;
				}
				break;

			case RESOURCE_GENERAL_CONFIGURATION:
				/* save the resource type and resource status */
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				break;

			case RESOURCE_MACROVISION:
				/* save the resource type and resource status */
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;

#ifdef COPY_CONTROL_SUBSTATUS_AVAILABLE
					IA_ReadFromMessage( pvMsg, &stCopyControlSubStatus, sizeof(stCopyControlSubStatus) );
					pstGrpSvc->stCopyControlInfo[CA_SERVICE_DESCRAMBLE].wLength=stCopyControlSubStatus.wLength;
					pstGrpSvc->stCopyControlInfo[CA_SERVICE_DESCRAMBLE].wTypeMask=stCopyControlSubStatus.wTypeMask;
					wTypeMask=stCopyControlSubStatus.wTypeMask;

					if(wTypeMask!=0)
					{
						CA_DBG_V(("RESOURCE_COPYCONTROL TypeMask = 0x%02x\n", wTypeMask));
					}

					if((wTypeMask & MSG_COPYCONTROL_MACROVISION )!= 0 )
					{
						IA_ReadFromMessage( pvMsg, &stMacroVisionMode, sizeof(stMacroVisionMode) );
						pstGrpSvc->stCopyControlInfo[CA_SERVICE_DESCRAMBLE].bMode=stMacroVisionMode.bMode;
						CA_DBG_V(("----COPYCONTROL_MACROVISION Mode = 0x%02x\n", stMacroVisionMode.bMode));
					}

					if((wTypeMask & MSG_COPYCONTROL_DIGITAL )!= 0 )
					{
						IA_ReadFromMessage( pvMsg, &stCopyControlDigitalCci, sizeof(stCopyControlDigitalCci) );
						pstGrpSvc->stCopyControlInfo[CA_SERVICE_DESCRAMBLE].eCopyControlInfoDigital=stCopyControlDigitalCci.eCopyControlInfo;
						CA_DBG_V(("----COPYCONTROL_DIGITAL CCI = 0x%02x\n", stCopyControlDigitalCci.eCopyControlInfo));
					}

					if((wTypeMask & MSG_COPYCONTROL_ANALOG )!= 0 )
					{
						IA_ReadFromMessage( pvMsg, &stCopyControlAnalogCci, sizeof(stCopyControlAnalogCci) );
						pstGrpSvc->stCopyControlInfo[CA_SERVICE_DESCRAMBLE].eCopyControlInfoAnalog=stCopyControlAnalogCci.eCopyControlInfo;
						CA_DBG_V(("----COPYCONTROL_ANALOG CCI = 0x%02x\n", stCopyControlAnalogCci.eCopyControlInfo));
					}

#endif
				wResCount +=1 ;
				break;
			default:
				CA_ERROR(("Unknown rsc type.\n"));
				break;
			}
#ifdef TO_SOFTCELL_3_0
			/*SoftCell3.0 does not caculate the severity among the global status and modules' status,
			ECT should do this, but the later version of SoftCell has done this internal, must not do it
			again in ECT, or there will be error for multiple smartcard.
			logged to SCR#25349, on Nov 15, 2006
			*/
			if(stRscStatus.stStatus.eSource == SOURCE_SMARTCARD ||
				stRscStatus.stStatus.eSource == SOURCE_SERVICES ||
				stRscStatus.stStatus.eSource == SOURCE_SOFTCELL ||
				stRscStatus.stStatus.eSource == SOURCE_DVB_DESCRAMBLE_SERVICE)
			{
				if (pstService[wGroupHandle].fClear == IA_FALSE)
				{
					if(pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].stGlobalStatus.eSeverity >= stRscStatus.stStatus.eSeverity)
					{
						pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].stGlobalStatus = stRscStatus.stStatus;
					}
				}
			}
#endif
			CA_ERROR_UpdateErrorStatus(wGroupHandle, CA_SERVICE_DESCRAMBLE, &pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].stGlobalStatus);
		}
		pstGrpSvc->astServices[CA_SERVICE_DESCRAMBLE].bRscCount = (ia_byte)wResCount;
		CA_SVCINFO_GrpSvcNotify(wGroupHandle, SERVICE_DVB_DESCRAMBLE);

		/* Trigger MR PIN code in following cases: */
		/* case: 1.SOURCE_MOD_MATURITY_RATING:D100 && SOURCE_SMARTCARD:E136/E137*/
		/* case: 2.SOURCE_SMARTCARD:E136 */
		if (((wSCFlag == 136)&&(wMRFlag != 101)) || ((wSCFlag == 137)&&(wMRFlag == 100)))
			CA_SVCINFO_MRPinNotify(wGroupHandle, SERVICE_DVB_DESCRAMBLE);

		return fMessageClose;
	}
	else
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		fMessageClose=IA_TRUE;
		return fMessageClose;
	}
}

ia_bool CA_SVCINFO_ParsePVRServiceInfo(ia_word32 wServiceType,ia_word32 wGroupHandle,msg_service_status_data_st* pstSvcStatusData,ia_msg_handle pvMsg)
{
	CA_LOCAL_GROUP_SERVICE_INFO_T *pstGrpSvc = 0;
	msg_rsc_status_st stRscStatus;
	ia_word16 i, wResCount = 0, wTypeMask;
	ia_bool fMessageClose=IA_FALSE;
		ia_bool	fOldMustExpire = IA_FALSE;	/* mli 3.3.11 7/6 */
	ia_word16 wMRFlag = 0, wSCFlag = 0;

	/* Set wSCFlag when SOURCE_SMARTCARD:E136/E137 */
	if ((pstSvcStatusData->stStatus.eSource == SOURCE_SMARTCARD)
		&&(pstSvcStatusData->stStatus.eSeverity == STATUS_ERROR)
		&&((pstSvcStatusData->stStatus.wStatus == 136)||(pstSvcStatusData->stStatus.wStatus == 137)))
	{
		wSCFlag = pstSvcStatusData->stStatus.wStatus;
	}

	if ((wServiceType == CA_SERVICE_RECORD) || (wServiceType == CA_SERVICE_PLAYBACK))
	{
		pstGrpSvc = (CA_LOCAL_GROUP_SERVICE_INFO_T*)CA_SVCINFO_GetSvcInfoPtr(wGroupHandle, wServiceType);
		if (pstGrpSvc == 0)
		{
			CA_ERROR(("No PVR Service.\n"));
			IA_CloseMessage(pvMsg, IA_FALSE);
			fMessageClose=IA_TRUE;
			return fMessageClose;
		}
		pstGrpSvc->astServices[wServiceType].stGlobalStatus = pstSvcStatusData->stStatus;

#ifndef TO_SOFTCELL_3_0 /*only for softcell 3.1.x or higher*/
		pstGrpSvc->astServices[wServiceType].stServiceStatus = pstSvcStatusData->stServiceStatus;
#endif

		for (i = 0; i < pstSvcStatusData->bResourceCount; i++)
		{
			IA_ReadFromMessage(pvMsg, &stRscStatus, sizeof(stRscStatus));

			switch( stRscStatus.wType )
			{
			case RESOURCE_SESSION_MANAGEMENT:
				{
					msg_session_source_substatus_st stSessionSrcSubStatus;
					/* save the resource type and resource status */
					pstGrpSvc->astServices[wServiceType].awRscType[wResCount] = stRscStatus.wType;
					pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource;
					pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus;
					pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity;
					wResCount +=1;
					IA_ReadFromMessage(pvMsg, &stSessionSrcSubStatus, sizeof(stSessionSrcSubStatus));
					/* mli: 3.3.11 */
					CA_DBG(("<3316>Source=%d,Status=%d,Severity=%d", stSessionSrcSubStatus.stStatus.eSource,
					stSessionSrcSubStatus.stStatus.wStatus,
					stSessionSrcSubStatus.stStatus.eSeverity));
				}
				break;
			case RESOURCE_SESSION_MANAGEMENT_DRM:
				{
					msg_session_source_substatus_st stSessionSrcSubStatus;
					msg_sm_drm_status_st stDrmStatus;
					msg_sm_drm_record_flags_st stDrmRecFlags;
					msg_sm_drm_rights_record_st stDrmRightRec;
					msg_copycontrol_substatus_st stCopyControlSubStatus;
					vd_macrovision_mode_st stMacroVisionMode;
					vd_copycontrol_digital_cci_st stCopyControlDigitalCci;
					vd_copycontrol_analog_cci_st stCopyControlAnalogCci;

					ansi_memset((void *)&stSessionSrcSubStatus, 0, sizeof(stSessionSrcSubStatus));
					ansi_memset((void *)&stDrmStatus, 0, sizeof(stDrmStatus));
					ansi_memset((void *)&stDrmRecFlags, 0, sizeof(stDrmRecFlags));
					ansi_memset((void *)&stDrmRightRec, 0, sizeof(stDrmRightRec));
					ansi_memset((void *)&stCopyControlSubStatus, 0, sizeof(stCopyControlSubStatus));
					ansi_memset((void *)&stMacroVisionMode, 0, sizeof(stMacroVisionMode));
					ansi_memset((void *)&stCopyControlDigitalCci, 0, sizeof(stCopyControlDigitalCci));
					ansi_memset((void *)&stCopyControlAnalogCci, 0, sizeof(stCopyControlAnalogCci));

					/* save the resource type and resource status */
					pstGrpSvc->astServices[wServiceType].awRscType[wResCount] = stRscStatus.wType;
					pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource;
					pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus;
					pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity;
					IA_ReadFromMessage(pvMsg, &stSessionSrcSubStatus, sizeof(stSessionSrcSubStatus));
					/* mli: 3.3.11 */
					CA_DBG(("<ECT:ca_svcinfo:1188>Source=%d,Status=%d,Severity=%d\n", stSessionSrcSubStatus.stStatus.eSource,
					stSessionSrcSubStatus.stStatus.wStatus,
					stSessionSrcSubStatus.stStatus.eSeverity));

					IA_ReadFromMessage(pvMsg, &stDrmStatus, sizeof(stDrmStatus));
					if (wServiceType == CA_SERVICE_RECORD)
					{
						pstGrpSvc->stPvrRecordServiceInfo.stStatus = stSessionSrcSubStatus.stStatus;
#if 0 /* SCR#27131 */
						pstGrpSvc->stPvrRecordServiceInfo.wRightsRecId = stDrmStatus.wRightsRecordId;
						pstGrpSvc->stPvrRecordServiceInfo.wSequenceRecId = stDrmStatus.wSequenceRecordId;
						pstGrpSvc->stPvrRecordServiceInfo.fRecordFlagsUpdated = stDrmStatus.fRecordFlagsUpdated;
						pstGrpSvc->stPvrRecordServiceInfo.fRightsRecordUpdated = stDrmStatus.fRightsRecordUpdated;
						pstGrpSvc->stPvrRecordServiceInfo.fSequenceRecordUpdated = stDrmStatus.fSequenceRecordUpdated;
#endif
					}
					else
					{
						pstGrpSvc->stPvrPlaybackServiceInfo.stStatus = stSessionSrcSubStatus.stStatus;
#if 0 /* SCR#27131 */
						pstGrpSvc->stPvrPlaybackServiceInfo.wRightsRecId = stDrmStatus.wRightsRecordId;
						pstGrpSvc->stPvrPlaybackServiceInfo.wSequenceRecId = stDrmStatus.wSequenceRecordId;
						pstGrpSvc->stPvrPlaybackServiceInfo.fRecordFlagsUpdated = stDrmStatus.fRecordFlagsUpdated;
						pstGrpSvc->stPvrPlaybackServiceInfo.fRightsRecordUpdated = stDrmStatus.fRightsRecordUpdated;
						pstGrpSvc->stPvrPlaybackServiceInfo.fSequenceRecordUpdated = stDrmStatus.fSequenceRecordUpdated;
#endif

					}
					/* mli: 3.3.11 */
					CA_DBG(("<ECT:ca_svcinfo:1213>Type=%d,RighID=%d,SeqID=%d,RecFlagUpdate=%d,RightsRecUpdate=%d,SeqRecUpdate=%d.\n",
						wServiceType,
						stDrmStatus.wRightsRecordId,
						stDrmStatus.wSequenceRecordId,
						stDrmStatus.fRecordFlagsUpdated,
						stDrmStatus.fRightsRecordUpdated,
						stDrmStatus.fSequenceRecordUpdated));
					if (stDrmStatus.fRecordFlagsUpdated == IA_TRUE)  /* only record */
					{
						pstGrpSvc->stPvrRecordServiceInfo.fRecordFlagsUpdated = stDrmStatus.fRecordFlagsUpdated; /* SCR#27131 */
						IA_ReadFromMessage(pvMsg, &stDrmRecFlags, sizeof(stDrmRecFlags));
						pstGrpSvc->stPvrRecordServiceInfo.fMustExpire = stDrmRecFlags.fMustExpire;
						pstGrpSvc->stPvrRecordServiceInfo.fNonPVRable = stDrmRecFlags.fNonPVRable;
						/* mli: 3.3.11 */
						CA_DBG(("<ECT:ca_svcinfo:1228>MustExpire=%d,NonPVRable=%d.\n",
						stDrmRecFlags.fMustExpire,
						stDrmRecFlags.fNonPVRable));
						/* whenever	the	must expire	flag changes, it will notify the client	with the current MustExpire	flag */
						if (fOldMustExpire != stDrmRecFlags.fMustExpire)  /* mli 7/6 */
						{
							 CA_CLIENT_Notify(wGroupHandle, CA_QC_PVR_MUST_EXPIRE_NOTIFY,	(ia_uint32)stDrmRecFlags.fMustExpire);
						}
						fOldMustExpire = stDrmRecFlags.fMustExpire;
					}

					if (stDrmStatus.fRightsRecordUpdated == IA_TRUE)
					{
						IA_ReadFromMessage(pvMsg, &stDrmRightRec, sizeof(stDrmRightRec));
						if (wServiceType == CA_SERVICE_RECORD)
						{
							pstGrpSvc->stPvrRecordServiceInfo.fRightsRecordUpdated = stDrmStatus.fRightsRecordUpdated; /* SCR#27131 */
							pstGrpSvc->stPvrRecordServiceInfo.wRightsRecId = stDrmStatus.wRightsRecordId; /* SCR#27131 */
							pstGrpSvc->stPvrRecordServiceInfo.wCRID = stDrmRightRec.wCRID;
							pstGrpSvc->stPvrRecordServiceInfo.bNrOfPlaybacks = stDrmRightRec.bNrOfPlaybacks;
							pstGrpSvc->stPvrRecordServiceInfo.eCopyControl = stDrmRightRec.eCopyControl;
							pstGrpSvc->stPvrRecordServiceInfo.wPlaybackWindowDuration = stDrmRightRec.wPlaybackWindowDuration;
							pstGrpSvc->stPvrRecordServiceInfo.wPlaybackWindowStartTime = stDrmRightRec.wPlaybackWindowStartTime;
						}
						else
						{
							pstGrpSvc->stPvrPlaybackServiceInfo.fRightsRecordUpdated = stDrmStatus.fRightsRecordUpdated; /* SCR#27131 */
							pstGrpSvc->stPvrPlaybackServiceInfo.wRightsRecId = stDrmStatus.wRightsRecordId; /* SCR#27131 */
							pstGrpSvc->stPvrPlaybackServiceInfo.wCRID = stDrmRightRec.wCRID;
							pstGrpSvc->stPvrPlaybackServiceInfo.bNrOfPlaybacks = stDrmRightRec.bNrOfPlaybacks;
							pstGrpSvc->stPvrPlaybackServiceInfo.eCopyControl = stDrmRightRec.eCopyControl;
							pstGrpSvc->stPvrPlaybackServiceInfo.wPlaybackWindowDuration = stDrmRightRec.wPlaybackWindowDuration;
							pstGrpSvc->stPvrPlaybackServiceInfo.wPlaybackWindowStartTime = stDrmRightRec.wPlaybackWindowStartTime;

							/* sren: 3.4.2 Copy Control support */
#ifdef COPY_CONTROL_SUBSTATUS_AVAILABLE
							IA_ReadFromMessage( pvMsg, &stCopyControlSubStatus, sizeof(stCopyControlSubStatus) );
							pstGrpSvc->stCopyControlInfo[CA_SERVICE_PLAYBACK].wLength=stCopyControlSubStatus.wLength;
							pstGrpSvc->stCopyControlInfo[CA_SERVICE_PLAYBACK].wTypeMask=stCopyControlSubStatus.wTypeMask;
							wTypeMask = stCopyControlSubStatus.wTypeMask;

							if( (wTypeMask & MSG_COPYCONTROL_MACROVISION )!= 0 )
							{
								IA_ReadFromMessage( pvMsg, &stMacroVisionMode, sizeof(stMacroVisionMode) );
								pstGrpSvc->stCopyControlInfo[CA_SERVICE_PLAYBACK].bMode=stMacroVisionMode.bMode;
							}

							if( (wTypeMask & MSG_COPYCONTROL_DIGITAL )!= 0 )
							{
								IA_ReadFromMessage( pvMsg, &stCopyControlDigitalCci, sizeof(stCopyControlDigitalCci) );
								pstGrpSvc->stCopyControlInfo[CA_SERVICE_PLAYBACK].eCopyControlInfoDigital=stCopyControlDigitalCci.eCopyControlInfo;
							}

							if((wTypeMask & MSG_COPYCONTROL_ANALOG )!= 0 )
							{
								IA_ReadFromMessage( pvMsg, &stCopyControlAnalogCci, sizeof(stCopyControlAnalogCci) );
								pstGrpSvc->stCopyControlInfo[CA_SERVICE_PLAYBACK].eCopyControlInfoAnalog=stCopyControlAnalogCci.eCopyControlInfo;
							}
#endif
						}

						/*  SCR#27131 */
						if ( IA_TRUE == stDrmStatus.fSequenceRecordUpdated )
						{
							if (wServiceType == CA_SERVICE_RECORD)
							{
								pstGrpSvc->stPvrRecordServiceInfo.fSequenceRecordUpdated = stDrmStatus.fSequenceRecordUpdated;
								pstGrpSvc->stPvrRecordServiceInfo.wSequenceRecId = stDrmStatus.wSequenceRecordId;
							}
							else
							{
								pstGrpSvc->stPvrPlaybackServiceInfo.fSequenceRecordUpdated = stDrmStatus.fSequenceRecordUpdated;
								pstGrpSvc->stPvrPlaybackServiceInfo.wSequenceRecId = stDrmStatus.wSequenceRecordId;
							}
						}
					}

					wResCount +=1;
				}
				break;
			case RESOURCE_SMARTCARD:
			case RESOURCE_CRYPTO_OPERATION:
				/* save the resource type and resource status */
				pstGrpSvc->astServices[wServiceType].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity;
				wResCount +=1 ;
				break;

			case RESOURCE_MATURITY_RATING:
				/* save the resource type and resource status */
				pstGrpSvc->astServices[wServiceType].awRscType[wResCount] = stRscStatus.wType;
				pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].eSource = stRscStatus.stStatus.eSource ;
				pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].wStatus = stRscStatus.stStatus.wStatus ;
				pstGrpSvc->astServices[wServiceType].astRscStatus[wResCount].eSeverity = stRscStatus.stStatus.eSeverity ;
				wResCount +=1 ;
				/* Set wMRFlag when SOURCE_MOD_MATURITY_RATING:D100/D101 */
				if ((stRscStatus.stStatus.eSource == SOURCE_MOD_MATURITY_RATING)
					&& (stRscStatus.stStatus.eSeverity == STATUS_INFO_DND))
				{
					wMRFlag = stRscStatus.stStatus.wStatus;
				}
				break;

			default:
				CA_ERROR(("Unknown rsc type.\n"));
				break;
			}
		}
		pstGrpSvc->astServices[wServiceType].bRscCount = (ia_byte)wResCount;
		if (wServiceType == CA_SERVICE_RECORD)
		{
			CA_SVCINFO_GrpSvcNotify(wGroupHandle, SERVICE_PVR_RECORD);
		}
		else
		{
			CA_SVCINFO_GrpSvcNotify(wGroupHandle, SERVICE_PVR_PLAYBACK);
			if (((wSCFlag == 136)&&(wMRFlag != 101)) || ((wSCFlag == 137)&&(wMRFlag == 100)))
				CA_SVCINFO_MRPinNotify(wGroupHandle, SERVICE_PVR_PLAYBACK);
		}

		return fMessageClose;
	}
	else
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		fMessageClose=IA_TRUE;
		return fMessageClose;
	}
}

ia_word16 CA_SVCINFO_FindDemux( ia_word32 wEMMHandle )
{
	ia_byte i;

	for ( i = 0 ; i < MAX_DEMUX_COUNTS ; i++ )
	{
		/* skip the blank elements */
		if ( astLocalDemuxInfo[ i ] == 0 )
		{
			continue;
		}
		if ( astLocalDemuxInfo[ i ]->wEMMHandle == wEMMHandle )
		{
			return i;
		}
	}
	return INVALID_INDEX;
}

/*
*     ROUTINE : CA_SVCINFO_AddDemux()
* DESCRIPTION : Add the demux to the local storage if not previously exist.
*       INPUT : wEMMHandle - The emm_handle(DemuxID) passed by CA Client.
*      OUTPUT : None
*     RETURNS : --the index of the Demux in local storage, or
*				INVALID_INDEX-- Cannot add a new Demux in the local storage
* SIDE EFFECT : Memory allocated for astLocalDemuxInfo[i]
*   CALLED BY : CA_SVCINFO_SetSvcHeaderInfo, DefineEMMServicesOnDemux
*      Author : Jane You
* Last Changed: September 19, 2006
*/
ia_word16 CA_SVCINFO_AddDemux( ia_word32 wEMMHandle )
{
	ia_word16				wIndex, i, j;
	CA_LOCAL_DEMUX_INFO_T	*pTemp;

	wIndex = CA_SVCINFO_FindDemux( wEMMHandle );
	if ( wIndex == INVALID_INDEX )
	{
		/* Allocate memory for a new transponder (demux) */
		for ( i = 0 ; i < MAX_DEMUX_COUNTS ; i++ )
		{
			if ( astLocalDemuxInfo[ i ] == 0 )
			{
				pTemp = (CA_LOCAL_DEMUX_INFO_T*)OS_DRV_AllocateMemory( sizeof(CA_LOCAL_DEMUX_INFO_T) );
				if (pTemp == 0)
				{
					CA_ERROR(("CA_SVCINFO_AddDemux: Memory allocate failed.\n"));
					CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
					return INVALID_INDEX;
				}
				astLocalDemuxInfo[ i ] = pTemp;
				pTemp->wEMMHandle = wEMMHandle;
				wIndex = i;
				/* jyou: SCR#26235 2007-08-07 moved from ca_svcinfo_SetHeader */
				for ( j = 0 ; j < SC_SLOT_MAX ; j++ )
				{
					/* assign default values to the basic infos */
					ResetBasicServiceInfo( &(pTemp->astServices[j]) );
					/* assign default values to the basic infos */
#ifdef INCL_SERVICE_DMB
					CA_DMB_ResetLocalEMMServiceInfo( &(pTemp->astDMBServices[j]) );
#endif
				}
				break;
			}
		}
	}
	return wIndex;
}

void CA_SVCINFO_ReleaseDemux( ia_word32 wEMMHandle )
{
	ia_byte					i;

	for ( i = 0 ; i < MAX_DEMUX_COUNTS ; i++ )
	{
		/* skip the blank elements */
		if ( astLocalDemuxInfo[ i ] == 0 )
		{
			continue;
		}
		if ( astLocalDemuxInfo[ i ]->wEMMHandle == wEMMHandle )
		{
			OS_DRV_FreeMemory( (void *)(astLocalDemuxInfo[ i ]) );
			astLocalDemuxInfo[ i ] = 0;
			break;
		}
	}
	return;
}

void CA_SVCINFO_ReleaseAllDemuxes( void )
{
	ia_byte		i;

	for ( i = 0 ; i < MAX_DEMUX_COUNTS ; i++ )
	{
		/* skip the blank elements */
		if ( astLocalDemuxInfo[ i ] == 0 )
		{
			continue;
		}
		OS_DRV_FreeMemory( (void *)(astLocalDemuxInfo[ i ]) );
		astLocalDemuxInfo[ i ] = 0;
	}
	return;
}

static void ResetLocalDemuxInfo( ia_word32 wIndex )
{
	if ( isValidDemuxIndex( wIndex ) == IA_TRUE )
	{
		OS_DRV_FreeMemory( (void *)(astLocalDemuxInfo[ wIndex ]) );
		astLocalDemuxInfo[ wIndex ] = 0;
	}
	return;
}

ia_bool isValidDemuxIndex( ia_word32 wIndex )
{
	if ( wIndex >= MAX_DEMUX_COUNTS )
		return IA_FALSE;

	if ( astLocalDemuxInfo[ wIndex ] == 0 )
		return IA_FALSE;

	return IA_TRUE;
}

ia_bool isValidSmartCardIndex( ia_word32 wIndex )
{
	if ( wIndex > SC_SLOT_MAX )
		return IA_FALSE;
	return IA_TRUE;
}

#if 0
/* jyou: these two functions are reserved for future use */
static ia_bool isStatusIdentical( ia_status_st *pstStatus0, ia_status_st *pstStatus1 )
{
	if (
		( pstStatus0->wStatus == pstStatus1->wStatus ) &&
		( pstStatus0->eSource == pstStatus1->eSource ) &&
		( pstStatus0->eSeverity == pstStatus1->eSeverity)
		)
	{
		return IA_TRUE;
	}
	return IA_FALSE;
}

static void StatusCopy( ia_status_st *pstStatus0, ia_status_st *pstStatus1 )
{
	pstStatus0->wStatus = pstStatus1->wStatus;
	pstStatus0->eSource = pstStatus1->eSource;
	pstStatus0->eSeverity = pstStatus1->eSeverity;
	return;
}
#endif

/*
*     ROUTINE : CA_LOCAL_GetUpperBytes()
* DESCRIPTION : Get the number at the upper N bytes of a 4 byte number.
*       INPUT : wData - The 4-byte number.
*				bNumBytes - number of bytes requested from the left-most byte
*      OUTPUT : None
*     RETURNS : The value of the upper bytes
* SIDE EFFECT : None
*   CALLED BY :
*      Author : Jane You
* Last Changed: September 21, 2006
*/
ia_word16 CA_LOCAL_GetUpperBytes( ia_word32 wData, ia_byte bNumBytes )
{
	static ia_word32 awMask[4] = { 0x000000FF, 0x0000FF00, 0xFFFF0000, 0xFFFFFFFF };

	if ( bNumBytes > 0 && bNumBytes < 3 )
	{
		return (ia_word16)( ( wData & awMask[bNumBytes] ) >> (8*bNumBytes) );
	}
	return (ia_word16)awMask[3];
}

ia_word16 CA_LOCAL_GetLowerBytes( ia_word32 wData, ia_byte bNumBytes )
{
	static ia_word32 awMask[4] = { 0x00000000, 0x000000FF, 0x0000FFFF, 0xFFFFFFFF };

	if ( bNumBytes > 0 && bNumBytes < 3 )
	{
		return (ia_word16)( ( wData & awMask[bNumBytes] ) );
	}
	return (ia_word16)awMask[3];
}

ia_word16 CA_LOCAL_MakeWord16( ia_word16 wData1, ia_word16 wData2 )
{
	return (ia_word16)( ( wData1 << 8 ) | wData2 );
}

ia_word32 CA_LOCAL_MakeWord32( ia_word16 wData1, ia_word16 wData2 )
{
	return (ia_word32)( ( (ia_word32)wData1 << 16 ) | (ia_word32)wData2 );
}

/*
*     ROUTINE : ResetBasicServiceInfo()
* DESCRIPTION : Set the default values to the basic information of a service
*       INPUT : pTemp - The pointer to the service in the local storage.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : Fields in the local storage is reset.
*   CALLED BY :
*      Author : Jane You
* Last Changed: September 19, 2006
*/
void ResetBasicServiceInfo( CA_LOCAL_BASIC_SERVICE_INFO_T* pTemp )
{
	int i;

	pTemp->wSmartCardId = INVALID_SMARTCARDID;
	pTemp->wServiceType = INVALID_SERVICE_TYPE;
	pTemp->wSCellHandle = INVALID_SCELL_SERVICE_HANDLE;
	pTemp->eSvcHandleStatus = CA_SERVICE_HANDLE_EMPTY;

	pTemp->stGlobalStatus.eSource = SOURCE_UNKNOWN;
	pTemp->stGlobalStatus.wStatus = CA_IRDETO_ERROR_D00;
	pTemp->stGlobalStatus.eSeverity = STATUS_INFO_DND;
	pTemp->stServiceStatus.eSource = SOURCE_UNKNOWN;
	pTemp->stServiceStatus.wStatus = CA_IRDETO_ERROR_D00;
	pTemp->stServiceStatus.eSeverity = STATUS_INFO_DND;
	pTemp->bRscCount = 0;

	/* jyou: 3.4.2 replacement of error status in ca_error.c */
	pTemp->stErrorStatus.eSource = SOURCE_UNKNOWN;
	pTemp->stErrorStatus.wStatus = CA_IRDETO_ERROR_D00;
	pTemp->stErrorStatus.eSeverity = STATUS_INFO_DND;

	/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
	pTemp->fMonitorDefined = IA_FALSE;

	/* jyou: 3.6.1 SCR#26708 2007-12-26 */
	pTemp->bResourceDefineCount = 0;
	for ( i = 0; i < MAX_SERVICE_RESOURCES ; i++ )
	{
		pTemp->astResourceDefines[ i ].wResourceType = INVALID_RESOURCE_TYPE;
		pTemp->astResourceDefines[ i ].wResourceID = INVALID_RESOURCE_ID;
	}

	return;
}

/* Assumption: the SoftCell service handle is unique to different services regardless of type*/
/*
*	for EMM service, pwIndex0 is the demux index, and pwIndex1 is the SC index
*	for Group service, pwIndex0 is the grouphandle, and pwIndex1 is the servicetype index
*/
static CA_LOCAL_BASIC_SERVICE_INFO_T*
	FindServiceBySCellHandle( ia_word16 wSCellHandle, ia_word16 *pwIndex0, ia_word16 *pwIndex1 )
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
					return &( astLocalDemuxInfo[i]->astServices[j] );
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
					return &( pstLocalGrpSvcInfo[i].astServices[j] );
				}
			}
		}
	}

	(*pwIndex0) = INVALID_INDEX;
	/* ayang 2008-04-09 Change pwIndex0 to pwIndex1 */
	(*pwIndex1) = INVALID_INDEX;
	return 0;
}

/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
void CA_SVCINFO_SetMonitorDefinedFlag( ia_word32 wIndex, CA_SERVICE_TYPE_EN eSvcType, ia_bool fFlag )
{
	CA_LOCAL_BASIC_SERVICE_INFO_T *pstService;
	CA_DBG_V( ("Set monitor flag for index=%d, svcType=%d to %d(1--IA_TRUE).\n",
		wIndex, eSvcType, fFlag) );
	switch ( eSvcType )
	{
		case CA_SERVICE_EMM:
			pstService = CA_SVCINFO_GetEMMServiceByOpSeqNo( (ia_word16)wIndex );
			break;
		case CA_SERVICE_DESCRAMBLE:
			pstService = CA_SVCINFO_GetDescServiceByGroupHandle( (ia_word16)wIndex );
			break;
#ifdef INCL_SERVICE_DMB
		case CA_SERVICE_DMB_EMM:
			pstService = CA_SVCINFO_GetEMMServiceByOpSeqNo( (ia_word16)wIndex );
			break;
		case CA_SERVICE_DMB_DESCRAMBLE:
			pstService = CA_SVCINFO_GetDescServiceByGroupHandle( (ia_word16)wIndex );
			break;
#endif
		default:
			pstService = 0;
			break;
	}
	if ( pstService != 0 )
	{
		pstService->fMonitorDefined = fFlag;
	}
	else
	{
		CA_ERROR( ("Failed to set the monitored flag.\n") );
	}
	return;
}


ia_bool CA_SVCINFO_isMonitorDefinedFlagSet( ia_word32 wIndex, CA_SERVICE_TYPE_EN eSvcType )
{
	CA_LOCAL_BASIC_SERVICE_INFO_T *pstService;
	CA_DBG_V( ("Check monitor flag for index=%d, svcType=%d .\n",
		wIndex, eSvcType) );
	switch ( eSvcType )
	{
		case CA_SERVICE_EMM:
			pstService = CA_SVCINFO_GetEMMServiceByOpSeqNo( (ia_word16)wIndex );
			break;
		case CA_SERVICE_DESCRAMBLE:
			pstService = CA_SVCINFO_GetDescServiceByGroupHandle( (ia_word16)wIndex );
			break;
#ifdef INCL_SERVICE_DMB
		case CA_SERVICE_DMB_EMM:
			pstService = CA_SVCINFO_GetEMMServiceByOpSeqNo( (ia_word16)wIndex );
			break;
		case CA_SERVICE_DMB_DESCRAMBLE:
			pstService = CA_SVCINFO_GetDescServiceByGroupHandle( (ia_word16)wIndex );
			break;
#endif
		default:
			pstService = 0;
			break;
	}
	if ( pstService != 0 )
	{
		return pstService->fMonitorDefined;
	}
	else
	{
		return IA_FALSE;
	}
}
/* jyou: 3.4.3.7 SCR#25552 2007-06-06 end*/

/* jyou: 3.6.1 SCR#26708 2007-12-26 start */
void CA_SVCINFO_SetResourceIDs( ia_word16 wGroupHandle, CA_SERVICE_TYPE_EN eServiceType,
	ia_word32 wResourceType,  ia_word32 wResourceID )
{
	int i, index =0, wResourceCount;
	CA_LOCAL_BASIC_SERVICE_INFO_T *pstService;

	if ( wGroupHandle >= wMaxNumSrvGroups )
	{
		CA_ERROR( ("Invalid group handle %d.\n", wGroupHandle) );
		return;
	}

	pstService = (CA_LOCAL_BASIC_SERVICE_INFO_T *)(&pstLocalGrpSvcInfo [ wGroupHandle ].astServices[ eServiceType ]) ;
	wResourceCount = pstService->bResourceDefineCount ;
	for ( i = 0; i < wResourceCount ; i++ )
	{
		if ( wResourceType == pstService->astResourceDefines[i].wResourceType )
		{
			/* Ignore the current config*/
			break;
		}
		if (  INVALID_RESOURCE_TYPE == pstService->astResourceDefines[i].wResourceType )
		{
			index = i;
		}
	}

	if ( i >= wResourceCount )
	{
		/*No same information stored before, so add it*/
		pstService->astResourceDefines[ index ].wResourceType = wResourceType;
		pstService->astResourceDefines[ index ].wResourceID = wResourceID;
		if ( pstService->bResourceDefineCount < MAX_SERVICE_RESOURCES )
		{
			pstService->bResourceDefineCount++;
		}
	}

	return;
}

ia_word32 CA_SVCINFO_GetResourceID( ia_word16 wGroupHandle, CA_SERVICE_TYPE_EN eServiceType,
	ia_word32 wResourceType )
{
	int i;
	CA_LOCAL_BASIC_SERVICE_INFO_T *pstService;

	if ( wGroupHandle >= wMaxNumSrvGroups )
	{
		CA_ERROR( ("Invalid group handle %d.\n", wGroupHandle) );
		/* ayang 2008-04-09 return INVALID_RESOURCE_ID */
		return INVALID_RESOURCE_ID;
	}

	pstService = (CA_LOCAL_BASIC_SERVICE_INFO_T *)(&pstLocalGrpSvcInfo [ wGroupHandle ].astServices[ eServiceType ]) ;
	for ( i = 0; i < pstService->bResourceDefineCount ; i++ )
	{
		if  ( wResourceType == pstService->astResourceDefines[i].wResourceType  )
		{
			return pstService->astResourceDefines[i].wResourceID;
		}
	}
	return INVALID_RESOURCE_ID;
}
/* jyou: 3.6.1 SCR#26708 2007-12-26 end */


