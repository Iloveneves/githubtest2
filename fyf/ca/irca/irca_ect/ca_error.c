/**
 * $Id: ca_error.c,v 1.18 2008/05/17 07:01:14 ayang Exp $
 *
 * Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
 * The module gets the global or service status from SoftCell and maps them to
 * corresponding error code in terms of document #710475.
 **/
/******************************** Header Files ********************************/
/* #include <string.h> */
#include <irca/pubh/s3_osdrv.h>
#include <irca/ca_task.h>
#include "ca_local.h"
#include "ca_error.h"
#include "ca_svcinfo.h"
#include <irca/pubh/s3_service.h>


/*********************************** Macros ***********************************/

/******************************* Local Variables ******************************/
static CA_LOCAL_ERROR_INFO_T *pstIrdetoGrpServiceError = 0;

/************************* Local Function Declarations ************************/
ia_bool equalsStatus( ia_status_st* pstTemp1, ia_status_st* pstTemp2 );

/*
*     ROUTINE : CA_ERROR_Initialise()
* DESCRIPTION : Initialize the error local storage when CA Task first start up,
*				or whenenver smart card status changes to Card-In.
*       INPUT : wMaxNumServiceGroups - The maximum number of service groups.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT :
*   CALLED BY : InitialiseAllModules, ParseSmartcardStatusMsg
*      Author : Jane You
* Last Changed: August 31, 2006
*/
void CA_ERROR_Initialise(ia_word16 wMaxNumServiceGroups)
{
	ia_word16 i, j;

	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */
	if (0 == wMaxNumServiceGroups)
	{
		return ;
	}
	if (0 == pstIrdetoGrpServiceError)
	{
		pstIrdetoGrpServiceError = (CA_LOCAL_ERROR_INFO_T*)
			OS_DRV_AllocateMemory((unsigned long)sizeof(CA_LOCAL_ERROR_INFO_T)*
			(unsigned long)wMaxNumServiceGroups);
	}
	if (0 == pstIrdetoGrpServiceError)
	{
		CA_ERROR(("CA_ERROR_Initialise Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ;
	}
	/* jyou: 3.4.2 SCR#25079 */
	/* Initiate error storage for Descramble, record, and playback services */
	for (i = 0; i < wMaxNumServiceGroups; i++)
	{
		/* jyou: 3.4.2 SCR#25079*/
		for ( j=0 ; j < CA_SERVICE_MAX_SERVICES ; j++ )
		{
			CA_ERROR_Reset(i, j);
		}
	}
}

/*
*     ROUTINE : CA_ERROR_Reset()
* DESCRIPTION : Resume the error local storage to default values.
*       INPUT : wIndex --
*					if ( wSvcType == CA_SERVICE_EMM)
*					then "wEMMIndex<<16 || wSmartCardIndex"
*					else "wGroupHandle"
*				wSvcType -- one of the following values
*					CA_SERVICE_EMM
*					CA_SERVICE_DESCRAMBLE
*					CA_SERVICE_RECORD
*					CA_SERVICE_PLAYBACK
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : if ( wSvcType == CA_SERVICE_EMM)
*					the astLocalDemuxInfo[i]->astServices[j].stErrorStatus will be reset,
*					where wIndex==( i<<16 | j )
*				else
*					the pstIrdetoGrpServiceError[bIndex] will be reset
*   CALLED BY : CA_ERROR_Initialise (only when wSvcType!=CA_SERVICE_EMM),
*				CaProcessMsg (CADRV__CA_TASK_MESSAGE->CA_CLIENT__STOP_SERVICE_GROUP),
*				StopEMMServicesOnDemux (only when wSvcType==CA_SERVICE_EMM)
*      Author : Jane You
* Last Changed: October 16, 2006
*/
void CA_ERROR_Reset( ia_word32 wIndex, ia_word32 wSvcType )
{
	CA_LOCAL_BASIC_SERVICE_INFO_T*	pstBasic;

	if (0 == pstIrdetoGrpServiceError )
		return;
	if (wIndex >= CA_SVCINFO_GetSvcMaxNumber())
	{
		CA_ERROR(("CA_ERROR_Reset index is big wIndex = %d\r\n", wIndex));
		return;
	}

	/* jyou: 3.4.2 SCR#25079 */
	switch (wSvcType)
	{
	case CA_SERVICE_EMM:
#ifdef INCL_SERVICE_DMB
	case CA_SERVICE_DMB_EMM:/*added by andrew  070514*/
#endif
		/* wGroupHandle contains the wOpSeqNo sent to CA Client during notification */
		pstBasic = CA_SVCINFO_GetEMMServiceByOpSeqNo( (ia_word16) wIndex );
		if ( pstBasic == 0 )
		{
			return;
		}
		pstBasic->stErrorStatus.eSource = SOURCE_UNKNOWN;
		pstBasic->stErrorStatus.wStatus = CA_IRDETO_ERROR_D00;
		pstBasic->stErrorStatus.eSeverity = STATUS_INFO_DND;
		break;
	case CA_SERVICE_DESCRAMBLE:
	case CA_SERVICE_RECORD:
	case CA_SERVICE_PLAYBACK:
#ifdef INCL_SERVICE_DMB
	case CA_SERVICE_DMB_DESCRAMBLE:
#endif
		pstIrdetoGrpServiceError[wIndex].wGroupHandle =
			INVALID_GROUP_SERVICE_HANDLE;
		pstIrdetoGrpServiceError[wIndex].wServiceType[wSvcType] = INVALID_SERVICETYPE;
		pstIrdetoGrpServiceError[wIndex].wServiceHandle[wSvcType] =
				INVALID_SCELL_SERVICE_HANDLE;
		pstIrdetoGrpServiceError[wIndex].astGlobalStatus[wSvcType].eSource =
				SOURCE_UNKNOWN;
		pstIrdetoGrpServiceError[wIndex].astGlobalStatus[wSvcType].wStatus =
				CA_IRDETO_ERROR_D00;
		pstIrdetoGrpServiceError[wIndex].astGlobalStatus[wSvcType].eSeverity =
				STATUS_INFO_DND;
		break;
	default:
		return;
	}
	return;
}

void CA_ERROR_MemoryFree()
{
	if (0 != pstIrdetoGrpServiceError)
	{
		FREE_MEMORY(pstIrdetoGrpServiceError);
		pstIrdetoGrpServiceError = 0;
	}
}


/*
*     ROUTINE : CA_ERROR_Reset()
* DESCRIPTION : Resume the error local storage to default values.
*       INPUT : wIndex --
*					if ( wSvcType == CA_SERVICE_EMM)
*					then "wEMMIndex<<16 || wSmartCardIndex"
*					else "wGroupHandle"
*				wSvcType -- one of the following values
*					CA_SERVICE_EMM
*					CA_SERVICE_DESCRAMBLE
*					CA_SERVICE_RECORD
*					CA_SERVICE_PLAYBACK
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : if ( wSvcType == CA_SERVICE_EMM)
*					the astLocalDemuxInfo[i]->astServices[j].stErrorStatus will be reset,
*					where wIndex==( i<<16 | j )
*				else
*					the pstIrdetoGrpServiceError[bIndex] will be reset
*   CALLED BY : CA_SVCINFO_ParseDescrambleServiceInfo,
*				CaProcessMsg (CADRV__CA_TASK_MESSAGE->CA_CLIENT__STOP_SERVICE_GROUP),
*				StopEMMServicesOnDemux (only when wSvcType==CA_SERVICE_EMM)
*      Author : Jane You
* Last Changed: October 16, 2006
*/
void CA_ERROR_UpdateErrorStatus(ia_word32 wIndex, ia_word32 wSrvType,
								ia_status_st *pstStatus)
{

#ifdef TO_SOFTCELL_3_0
	if ( pstStatus == 0 )
	{
		return ;
	}

	/* Start:This code is pnly for SoftCell 3.0.x */
	if ((irdetoDescrGlobalError[wDemux].stGlobalStatus.wStatus == pstStatus
		->wStatus) &&
		(irdetoDescrGlobalError[wDemux].stGlobalStatus.eSource == pstStatus
		->eSource) &&
		(irdetoDescrGlobalError[wDemux].stGlobalStatus.eSeverity ==
		pstStatus->eSeverity))
	{
		return ;
	}

	if (pstStatus->eSource == SOURCE_SMARTCARD || pstStatus->eSource ==
		SOURCE_DVB_DESCRAMBLE_SERVICE || pstStatus->eSource ==
		SOURCE_SERVICES || pstStatus->eSource == SOURCE_SOFTCELL)
	{
		CA_DBG_V("[Severity]Status-Source: [%d]%d-%d\n", pstStatus
			->eSeverity, pstStatus->wStatus, pstStatus->eSource);

		irdetoDescrGlobalError[wDemux].stGlobalStatus.eSource = pstStatus
			->eSource;
		irdetoDescrGlobalError[wDemux].stGlobalStatus.wStatus = pstStatus
			->wStatus;
		irdetoDescrGlobalError[wDemux].stGlobalStatus.eSeverity = pstStatus
			->eSeverity;
		CA_CLIENT_Notify(wDemux, CA_QC_NOTIFY_CURRENT_ERROR, (ia_uint32)
			SERVICE_DVB_DESCRAMBLE);

	}

	/* End:This code is only for SoftCell 3.0.x */
#else /*only for softcell 3.1.x or higher*/

	if (CA_SERVICE_EMM == wSrvType)
	{
		/* jyou: SCR#25079 */
		CA_CLIENT_Notify(wIndex, CA_QC_NOTIFY_CURRENT_ERROR, SERVICE_DVB_EMM);
	}
#ifdef INCL_SERVICE_DMB
	/*Andrew  Added 070514*/
	else if(CA_SERVICE_DMB_EMM == wSrvType)
	{
		CA_CLIENT_Notify(wIndex, CA_QC_NOTIFY_CURRENT_ERROR, SERVICE_DMB_EMM);
	}
#endif
	else
	{
		/* jyou: 3.4.2 pstStatus not used for EMM service */
		if ( pstStatus == 0 )
		{
			return ;
		}
		if ( 0 == pstIrdetoGrpServiceError )
			return ;
		if (wIndex >= CA_SVCINFO_GetSvcMaxNumber())
		{
			CA_ERROR(("CA_ERROR_UpdateErrorStatus index is big wIndex = %d\r\n", wIndex));
			return;
		}
		/* jyou: 3.4.2 SCR#25079 */
		if ( equalsStatus( &pstIrdetoGrpServiceError[wIndex].astGlobalStatus[wSrvType], pstStatus )==IA_TRUE )
		{
			return ;
		}

		CA_DBG_V(("[Severity]Status-Source: [%d]%d-%d\n", pstStatus
			->eSeverity, pstStatus->wStatus, pstStatus->eSource));

		pstIrdetoGrpServiceError[wIndex].astGlobalStatus[wSrvType].eSource
			= pstStatus->eSource;
		pstIrdetoGrpServiceError[wIndex].astGlobalStatus[wSrvType].wStatus
			= pstStatus->wStatus;
		pstIrdetoGrpServiceError[wIndex].astGlobalStatus[wSrvType].eSeverity
			= pstStatus->eSeverity;

		switch (wSrvType)
		{
		case CA_SERVICE_DESCRAMBLE:
			pstIrdetoGrpServiceError[wIndex].wServiceType[wSrvType] =
				SERVICE_DVB_DESCRAMBLE;
			break;
		case CA_SERVICE_RECORD:
			pstIrdetoGrpServiceError[wIndex].wServiceType[wSrvType] =
				/* SERVICE_PVR_DRM */SERVICE_PVR_RECORD;
				break;
		case CA_SERVICE_PLAYBACK:
			pstIrdetoGrpServiceError[wIndex].wServiceType[wSrvType] =
				/* SERVICE_PVR_DRM */SERVICE_PVR_PLAYBACK;
				break;
#ifdef INCL_SERVICE_DMB
			/*Andrew  Added 070514*/
		case CA_SERVICE_DMB_DESCRAMBLE:
				pstIrdetoGrpServiceError[wIndex].wServiceType[wSrvType] =
					SERVICE_DMB_DESCRAMBLE;
				break;
#endif
		default:
			CA_ERROR(("CA_ERROR_UpdateErrorStatus invalidate service type\r\n"));
			break;
		}

		pstIrdetoGrpServiceError[wIndex].wServiceHandle[wSrvType] =
			CA_SVCINFO_GetSCellHandle(wIndex, wSrvType);

		CA_CLIENT_Notify(wIndex, CA_QC_NOTIFY_CURRENT_ERROR,
			pstIrdetoGrpServiceError[wIndex].wServiceType[wSrvType]);

	}

#endif
}

CA_QUERY_STATUS_EN CA_ERROR_InfoQueryControl( ia_word32 wHandle, void *pCAClientMsg )
{
	ia_word32 wService = 0;
	ia_word32 wSrvType;
	CA_ERROR_INFO_T	*pTemp;
	CA_LOCAL_BASIC_SERVICE_INFO_T* pstBasic;

	CA_PARAM_NOT_USED( wHandle );

	pTemp = (CA_ERROR_INFO_T*)pCAClientMsg;

	if (pTemp == 0)
	{
		return CA_QUERY_FAIL;
	}
	wService = pTemp->wGroupHandle;
	wSrvType = pTemp->wServiceType;
	switch (wSrvType)
	{
	case SERVICE_DVB_EMM:
		/* wGroupHandle contains the wOpSeqNo sent to CA Client during notification */
		pstBasic = CA_SVCINFO_GetEMMServiceByOpSeqNo( (ia_word16) wService );
		pTemp->wServiceHandle = pstBasic->wSCellHandle;
		pTemp->stGlobalStatus = pstBasic->stErrorStatus;
		break;
#ifdef INCL_SERVICE_DMB
		/*Added by andrew 070514*/
	case SERVICE_DMB_EMM:
		/* wGroupHandle contains the wOpSeqNo sent to CA Client during notification */
		pstBasic = CA_SVCINFO_GetEMMServiceByOpSeqNo( (ia_word16) wService );
		pTemp->wServiceHandle = pstBasic->wSCellHandle;
		pTemp->stGlobalStatus = pstBasic->stGlobalStatus;
		break;
#endif
	case SERVICE_DVB_DESCRAMBLE:
		pTemp->wServiceHandle =
			pstIrdetoGrpServiceError[wService].wServiceHandle[CA_SERVICE_DESCRAMBLE];
		pTemp->stGlobalStatus =
			pstIrdetoGrpServiceError[wService].astGlobalStatus[CA_SERVICE_DESCRAMBLE];
		break;
#ifdef INCL_SERVICE_DMB
		/*Added andrew 070521*/
	case SERVICE_DMB_DESCRAMBLE:
		pTemp->wServiceHandle =
			pstIrdetoGrpServiceError[wService].wServiceHandle[CA_SERVICE_DMB_DESCRAMBLE];
		pTemp->stGlobalStatus =
			pstIrdetoGrpServiceError[wService].astGlobalStatus[CA_SERVICE_DMB_DESCRAMBLE];
		break;
#endif
	case SERVICE_PVR_RECORD:
		pTemp->wServiceHandle =
			pstIrdetoGrpServiceError[wService].wServiceHandle[CA_SERVICE_RECORD];
		pTemp->stGlobalStatus =
			pstIrdetoGrpServiceError[wService].astGlobalStatus[CA_SERVICE_RECORD];
		break;
	case SERVICE_PVR_PLAYBACK:
		pTemp->wServiceHandle =
			pstIrdetoGrpServiceError[wService].wServiceHandle[CA_SERVICE_PLAYBACK];
		pTemp->stGlobalStatus =
			pstIrdetoGrpServiceError[wService].astGlobalStatus[CA_SERVICE_PLAYBACK];
		break;
	default:
		return CA_QUERY_NOT_AVAILABLE;
	}

	CA_DBG_V(("[Current Error]Status-Source: [%d]%d-%d\n",
		pTemp->stGlobalStatus.eSeverity,
		pTemp->stGlobalStatus.wStatus,
		pTemp->stGlobalStatus.eSource));

	return CA_QUERY_SUCCESS;
}

/*
*     ROUTINE : equalsStatus()
* DESCRIPTION : Compare the values of two status structure
*       INPUT : pstTemp1, pstTemp2--pointers to the structures that will be compared
*      OUTPUT : None
*     RETURNS : IA_TRUE -- the two values are identical
*				IA_FALSE -- the two values are different
* SIDE EFFECT : None
*   CALLED BY :
*      Author : Jane You
* Last Changed: October 16, 2006
*/
ia_bool equalsStatus( ia_status_st* pstTemp1, ia_status_st* pstTemp2 )
{
	if (( pstTemp1->wStatus == pstTemp2->wStatus )
		&& ( pstTemp1->eSource == pstTemp2->eSource )
		&& ( pstTemp1->eSeverity == pstTemp2->eSeverity ))
	{
		return IA_TRUE;
	}
	return IA_FALSE;
}

/* 2008-5-9 AYang: Added Error dealing function to notify application of errors */
void CA_ERROR_Notify( CA_ERROR_SOURCE_EN eSource, ia_word16 wError )
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_FATAL_ERROR) == IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_FATAL_ERROR,
			(ia_word32)(((ia_word32) eSource << 16) | (ia_word32) wError));
	}
}
