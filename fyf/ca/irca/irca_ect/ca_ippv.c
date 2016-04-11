/**
 * $Id: ca_ippv.c,v 1.22 2008/05/23 08:46:35 ayang Exp $
 *
 * Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
 *
 * The IPPV module of Embedded CA Task.
 **/

/******************************************************************************
* FILE          : CA_IPPV.C                                                  *
*                                                                            *
* DESCRIPTION   :
*                                                                            *
* NOTES         :                                                            *
*                                                                            *
* STATUS/HISTORY:                                                            *
* Author          : Melissa Li                                               *
* Status          : $Revision: 1.22 $                                    *
* History         : 														  *
*
******************************************************************************/

/******************************** Header Files ********************************/
/* #include <string.h> */
#include <irca/pubh/s3_ansi_functions.h>
#include <irca/pubh/s3_osdrv.h>
#include <irca/ca_task.h>
#include "ca_local.h"
#include "ca_svcinfo.h"
#include "ca_scard.h"
#include <irca/pubh/s3_message_helper.h>
#include <irca/pubh/s3_cadrv.h>
#include <irca/pubh/s3_event_if_msg.h>
#include <irca/pubh/s3_ippv_msg.h>
#include "ca_ippv.h"
#include "ca_error.h"

/******************************* Local Variables ******************************/

static CA_IPPV_SECTOR_INFO_T *pstCaIppvSectorInfo[SC_SLOT_MAX];
static CA_IPPV_EVENTS_LIST_T *pstCaIppvEventLists[SC_SLOT_MAX];
static CA_IPPV_EVENTS_CHECK_T astCaIppvEventsChecks[SC_SLOT_MAX];
static CA_IPPV_PURCHASE_T astCaIppvPurchaseReply[SC_SLOT_MAX];
static CA_IPPV_PREVIEW_T astCaIppvPreviewData[SC_SLOT_MAX];
static CA_IPPV_PHONE_NUMBER_T *pstCaIppvPhoneNumber[SC_SLOT_MAX];

/************************* Local Function Declarations ************************/

static void copyEventList(CA_IPPV_EVENTS_LIST_T *caIppvEventList, ia_byte
							bIndex, ia_byte bSector);

/************************ Function Implementations ****************************/

static void CA_IPPV_Reset(ia_byte bIndex, ia_word32 wResourceId)
{
	CA_IPPV_SECTOR_INFO_T *pTempIppvSectorInfo = 0;
	CA_IPPV_PHONE_NUMBER_T *pTempIppvPhoneNumber = 0;
	ia_byte bSectorNumber, i;

	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */
	if (pstCaIppvSectorInfo[bIndex] != 0)
	{
		OS_DRV_FreeMemory((void *)pstCaIppvSectorInfo[bIndex]);
		pstCaIppvSectorInfo[bIndex] = 0;
	}

	if (astCaIppvPreviewData[bIndex].wGroupHandle != 0)
	{
		OS_DRV_FreeMemory((void *)astCaIppvPreviewData[bIndex].wGroupHandle);
		astCaIppvPreviewData[bIndex].wGroupHandle = 0;
	}
	if (astCaIppvEventsChecks[bIndex].pstEventCheck != 0)
	{
		OS_DRV_FreeMemory((void *)astCaIppvEventsChecks[bIndex].pstEventCheck);
		astCaIppvEventsChecks[bIndex].pstEventCheck = 0;
	}
	if (pstCaIppvPhoneNumber[bIndex] != 0)
	{
		OS_DRV_FreeMemory((void *)pstCaIppvPhoneNumber[bIndex]);
		pstCaIppvPhoneNumber[bIndex] = 0;
	}

	/* jyou: 3.4.2 bug fixing, memory violation */
	/* Wrong sizeof(astCaIppvEventsChecks) == 2*sizeof(CA_IPPV_EVENTS_CHECK_T) */
	ansi_memset((void*)&astCaIppvEventsChecks[bIndex], 0, sizeof(CA_IPPV_EVENTS_CHECK_T));
	ansi_memset((void*)&astCaIppvPreviewData[bIndex], 0, sizeof(CA_IPPV_PREVIEW_T));
	ansi_memset((void*)&astCaIppvPurchaseReply[bIndex], 0, sizeof(CA_IPPV_PURCHASE_T));

	if (INVALID_SC_SMARTCARDID == wResourceId)
	{
		CA_IPPV_InitEventInfo(bIndex, wResourceId);
		return ;
	}

	bSectorNumber = CA_SCARD_GetSectorNumber(wResourceId);
	/* the smartcard id is invalidate */
	if (bSectorNumber == INVALID_SC_SECTOR)
		return ;

	if (bSectorNumber > 0)
	{
		if (pstCaIppvSectorInfo[bIndex] == 0)
		{
			pstCaIppvSectorInfo[bIndex] = (CA_IPPV_SECTOR_INFO_T*)
				OS_DRV_AllocateMemory((unsigned long)sizeof
				(CA_IPPV_SECTOR_INFO_T)*(unsigned long)bSectorNumber);
			if (pstCaIppvSectorInfo[bIndex] == 0)
			{
				CA_ERROR(("Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return ;
			}
		}
		pTempIppvSectorInfo = pstCaIppvSectorInfo[bIndex];

		for (i = 0; i < bSectorNumber; i++)
		{
			pTempIppvSectorInfo[i].wResourceId = wResourceId;
			pTempIppvSectorInfo[i].bSector = INVALID_SC_SECTOR;
			pTempIppvSectorInfo[i].wDebitLimit = 0;
			pTempIppvSectorInfo[i].wDebitThreshold = 0;
			pTempIppvSectorInfo[i].bMaxNoIppvEvents = 0;
			pTempIppvSectorInfo[i].wResourceId = wResourceId;
			pTempIppvSectorInfo[i].bRequestType = 0xFF;
			pTempIppvSectorInfo[i].bSector = INVALID_SC_SECTOR;
			pTempIppvSectorInfo[i].wOperatorPurse = 0;
			pTempIppvSectorInfo[i].wCustomerPurse = 0;
		}

		if (pstCaIppvPhoneNumber[bIndex] == 0)
		{
			pstCaIppvPhoneNumber[bIndex] = (CA_IPPV_PHONE_NUMBER_T *)
				OS_DRV_AllocateMemory((unsigned long)sizeof
				(CA_IPPV_PHONE_NUMBER_T)*(unsigned long)bSectorNumber);
			if (pstCaIppvPhoneNumber[bIndex] == 0)
			{
				CA_ERROR(("Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return ;
			}
		}

		pTempIppvPhoneNumber = pstCaIppvPhoneNumber[bIndex];

		for (i = 0; i < bSectorNumber; i++)
		{
			pTempIppvPhoneNumber[i].wResourceId = wResourceId;
			pTempIppvPhoneNumber[i].bSectorID = INVALID_SC_SECTOR;
			pTempIppvPhoneNumber[i].bPhoneNumberLength = 0;
			pTempIppvPhoneNumber[i].pabPhoneNumber = 0;
		}
	}
	CA_IPPV_InitEventInfo(bIndex, wResourceId);
	return;
}

/*
* Description: Initializes the IPPV local database.
*
* Parameters : None.
*
* Returns    : None.
*/
void CA_IPPV_Initialise(ia_word32 wResourceId)
{
	ia_byte i;
	CA_IPPV_SECTOR_INFO_T *pTempIppvSecInfo = 0;

	if (wResourceId == INVALID_SMARTCARDID)
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			CA_IPPV_Reset(i, wResourceId);
		}
	}
	else
	{
		ia_byte bUsed = SC_SLOT_MAX;
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			pTempIppvSecInfo = pstCaIppvSectorInfo[i];
			if (pTempIppvSecInfo != 0)
			{
				if (pTempIppvSecInfo->wResourceId == wResourceId)
				{
					CA_IPPV_Reset(i, wResourceId);
					return ;
				}
			}
			else
			{
				if (bUsed == SC_SLOT_MAX)
				{
					bUsed = i;
				}
			}
		}
		if (bUsed != SC_SLOT_MAX)
		{
			CA_IPPV_Reset(bUsed, wResourceId);
		}
	}
}

/*
* Description: Initializes the IPPV Event Info database.
*
* Parameters : None.
*
* Returns    : None.
*/
void CA_IPPV_InitEventInfo(ia_byte bScardIndex, ia_word32 wResourceId)
{
	ia_byte i;
	ia_byte bSectorNumber;
	CA_IPPV_EVENTS_LIST_T *pTemp = 0;
	CA_IPPV_EVENTS_LIST_T *pTempCaIppvEventLists = 0;
	if (bScardIndex > SC_SLOT_MAX-1)
	{
		CA_ERROR( ("CA_IPPV_InitEventInfo index = %d is too big.\n", bScardIndex));
		return;
	}

	pTempCaIppvEventLists = pstCaIppvEventLists[bScardIndex];
	while (pTempCaIppvEventLists != 0)
	{
		pTemp = pTempCaIppvEventLists;
		pTempCaIppvEventLists = pTempCaIppvEventLists->next;

		if (pTemp->pstEventRecords != 0)
		{
			OS_DRV_FreeMemory((void *)pTemp->pstEventRecords);
			pTemp->pstEventRecords = 0;
		}
	}
	if (0 != pstCaIppvEventLists[bScardIndex])
	{
		OS_DRV_FreeMemory((void *)pstCaIppvEventLists[bScardIndex]);
		pstCaIppvEventLists[bScardIndex] = 0;
	}

	if (astCaIppvEventsChecks[bScardIndex].pstEventCheck != 0)
	{
		OS_DRV_FreeMemory((void *)astCaIppvEventsChecks[bScardIndex].pstEventCheck);
		astCaIppvEventsChecks[bScardIndex].pstEventCheck = 0;
	}

	if (INVALID_SC_SMARTCARDID == wResourceId)
		return ;

	bSectorNumber = CA_SCARD_GetSectorNumber(wResourceId);
	/* the smartcard id is invalidate */
	if (bSectorNumber == INVALID_SC_SECTOR)
		return ;

	if (bSectorNumber > 0)
	{
		if (pstCaIppvEventLists[bScardIndex] == 0)
		{
			pstCaIppvEventLists[bScardIndex] = (CA_IPPV_EVENTS_LIST_T*)
				OS_DRV_AllocateMemory((unsigned long)sizeof
				(CA_IPPV_EVENTS_LIST_T)*(unsigned long)bSectorNumber);
			ansi_memset((void*)pstCaIppvEventLists[bScardIndex], 0, sizeof(CA_IPPV_EVENTS_LIST_T) *bSectorNumber);
		}
		if (pstCaIppvEventLists[bScardIndex] == 0)
		{
			CA_ERROR(("CA_IPPV_InitEventInfo: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}
	}

	pTempCaIppvEventLists = pstCaIppvEventLists[bScardIndex];
	for (i = 0; i < bSectorNumber; i++)
	{
		pTempCaIppvEventLists[i].wResourceId = wResourceId;
		pTempCaIppvEventLists[i].bDaysGrace = 0x0;
		pTempCaIppvEventLists[i].bIPPVEventCount = 0x00;
		pTempCaIppvEventLists[i].bRequestType = 0xFF;
		pTempCaIppvEventLists[i].bSector = INVALID_SC_SECTOR;
		pTempCaIppvEventLists[i].bStatus = 0x0;
		pTempCaIppvEventLists[i].pstEventRecords = 0;
		if (i == bSectorNumber - 1)
		{
			pTempCaIppvEventLists[i].next = 0;
		}
		else
		{
			pTempCaIppvEventLists[i].next = &pTempCaIppvEventLists[i + 1];
		}
	}

	astCaIppvEventsChecks[bScardIndex].wResourceId = wResourceId;
	astCaIppvEventsChecks[bScardIndex].bRequestType = 0xFF;
	astCaIppvEventsChecks[bScardIndex].bDaysGrace = 0;
	astCaIppvEventsChecks[bScardIndex].bStatus = 0xFF;
	astCaIppvEventsChecks[bScardIndex].bIPPVEventCount = 0;
	if (astCaIppvEventsChecks[bScardIndex].pstEventCheck != 0)
	{
		OS_DRV_FreeMemory((void *)astCaIppvEventsChecks[bScardIndex].pstEventCheck);
		astCaIppvEventsChecks[bScardIndex].pstEventCheck = 0;
	}

	astCaIppvPurchaseReply[bScardIndex].wResourceId = wResourceId;
	/* sren: 3.4.2 SCR#25320 */
	astCaIppvPreviewData[bScardIndex].wResourceId = wResourceId;
}

/*
* Description: Notify for CA_IPPV_PREVIEW type.
*
* Parameters : pstSCellMsg - Pointer to the data buffer from softcell.
*
* Returns    : None
*
*/
void CA_IPPV_EventNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_ippv_event_notify_st IppvEventNotify;
	ia_word32 i, j;
	ia_word16 wServiceID = 0;
	ia_word32 wGroupHandle = 0;
	/*	ia_word16 *pawServiceID = 0;*/

	ansi_memset((void *)&IppvEventNotify, 0, sizeof(msg_ippv_event_notify_st));

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &IppvEventNotify, sizeof(msg_ippv_event_notify_st)) ==
		0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (IppvEventNotify.wResourceId == astCaIppvPreviewData[i].wResourceId)
			break;
	}
	if (i == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	astCaIppvPreviewData[i].bSector = IppvEventNotify.bSector;
	astCaIppvPreviewData[i].wIPPVEventID = IppvEventNotify.wEventId;
	astCaIppvPreviewData[i].wEventCost = IppvEventNotify.wEventCost;
	astCaIppvPreviewData[i].bClockValue = IppvEventNotify.bClockValue;
	astCaIppvPreviewData[i].bPreviewValue = IppvEventNotify.bPreviewValue;
	astCaIppvPreviewData[i].wServiceCount = IppvEventNotify.wServiceCount;
	if (astCaIppvPreviewData[i].wGroupHandle != 0)
	{
		OS_DRV_FreeMemory((void *)astCaIppvPreviewData[i].wGroupHandle);
		astCaIppvPreviewData[i].wGroupHandle = 0;
	}

	if (0 == astCaIppvPreviewData[i].wGroupHandle)
	{
		astCaIppvPreviewData[i].wGroupHandle = (ia_word32*)
			OS_DRV_AllocateMemory((unsigned long)sizeof(ia_word32)*(unsigned
			long)IppvEventNotify.wServiceCount);
	}
	/* Fixed SCR#27163 */
	if (0 == astCaIppvPreviewData[i].wGroupHandle && IppvEventNotify.wServiceCount > 0)
	{
		CA_ERROR(("CA_IPPV_EventNotify: Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (j = 0; j < IppvEventNotify.wServiceCount; j++)
	{
		IA_ReadFromMessage(pvMsg, &wServiceID, sizeof(ia_word16));

		wGroupHandle = CA_SVCINFO_HandleSCell2CaClient(wServiceID);
		/* if it can't find the group handle, set it using invalide value */
		astCaIppvPreviewData[i].wGroupHandle[j] = wGroupHandle;

	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_IPPV_INFO) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify(IppvEventNotify.wResourceId, CA_QC_IPPV_PREVIEW, (ia_word32)
			IppvEventNotify.wServiceCount);
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}

/*
* Description: Notify control for CA_IPPV_Buy_REPLY type.
*
* Parameters : pstSCellMsg - Pointer to the data buffer from softcell.
*
* Returns    : None.
*
*/
void CA_IPPV_BuyNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_ippv_buy_reply_st IppvBuyReply;
	ia_byte i;

	ansi_memset((void *)&IppvBuyReply, 0, sizeof(msg_ippv_buy_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &IppvBuyReply, sizeof(msg_ippv_buy_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (IppvBuyReply.wResourceId == astCaIppvPurchaseReply[i].wResourceId)
			break;
	}
	if (i == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	astCaIppvPurchaseReply[i].wIPPVEventID = IppvBuyReply.wEventId;
	astCaIppvPurchaseReply[i].bSCpinstatus = (ia_byte)IppvBuyReply.eResult;
	astCaIppvPurchaseReply[i].bSector = IppvBuyReply.bSector;

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_IPPV_INFO) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( IppvBuyReply.wResourceId , CA_QC_IPPV_PURCHASE , 0 );
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}

/*
* Description: Notify control for CA_IPPV_DEBIT and CA_QC_IPPV_PURSES.
*
* Parameters : pstSCellMsg - Pointer to the data buffer from softcell.
*
* Returns    : None.
*
*/
void CA_IPPV_SectorInfoNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_ippv_sector_reply_st IppvSectorReply;
	ia_byte bSectorIndex = 0;
	ia_byte i;
	CA_IPPV_SECTOR_INFO_T *pstTempCaIppvSecInfo = 0;
	ia_word32 wResourceId = INVALID_SC_SMARTCARDID;

	ansi_memset((void *)&IppvSectorReply, 0, sizeof(msg_ippv_sector_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &IppvSectorReply, sizeof(msg_ippv_sector_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	if (IppvSectorReply.eResult != IPPV_SECTOR_SUCCESS)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	wResourceId = IppvSectorReply.wResourceId;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (pstCaIppvSectorInfo[i] != 0)
		{
			if (wResourceId == pstCaIppvSectorInfo[i]->wResourceId)
				break;
		}
	}
	if (i == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	pstTempCaIppvSecInfo = pstCaIppvSectorInfo[i];

	bSectorIndex = IppvSectorReply.bSector;

	if (CA_SCARD_GetSectorType(IppvSectorReply.wResourceId, bSectorIndex) !=
		PPT_SUPPORT && CA_SCARD_GetSectorType(IppvSectorReply.wResourceId,
		bSectorIndex) != IA_INVALID_PARAMETER)
	{
		pstTempCaIppvSecInfo[bSectorIndex].bSector = bSectorIndex;
		pstTempCaIppvSecInfo[bSectorIndex].wDebitLimit =
			IppvSectorReply.wDebitLimit;
		pstTempCaIppvSecInfo[bSectorIndex].wDebitThreshold =
			IppvSectorReply.wDebitThreshold;
		pstTempCaIppvSecInfo[bSectorIndex].wCustomerPurse =
			IppvSectorReply.wCustomerPurse;
		pstTempCaIppvSecInfo[bSectorIndex].wOperatorPurse =
			IppvSectorReply.wOperatorPurse;
		pstTempCaIppvSecInfo[bSectorIndex].bMaxNoIppvEvents =
			IppvSectorReply.bMaxNoIppvEvents;

		if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_IPPV_INFO) == IA_TRUE)
		{
			CA_CLIENT_Notify(wResourceId, CA_QC_IPPV_SECTOR_INFO, (ia_word32)
				bSectorIndex);
		}
	}
	else
	{
		CA_ERROR(("CA_IPPV_SectorInfoNotify: The Sector is PPT support!\n"));

	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}


/*
* Description: Notify for CA_IPPV_EVENTS_LIST type.
*
* Parameters : pstSCellMsg - Pointer to the data buffer from softcell.
*
* Returns    : None
*/
void CA_IPPV_EventsListNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	CA_IPPV_EVENTS_LIST_T *pstTemp = 0;
	CA_IPPV_EVENTS_LIST_T *pstTempCaIppvEventLists = 0;
	ia_msg_handle pvMsg;
	msg_ippv_event_list_reply_st IppvEventListReply;
	msg_ippv_event_list_reply_event_st IppvEventListReplyEvent;
	ia_byte bSectorIndex, bSectorNum;
	ia_byte bEventCount, i;
	ia_word32 wSCardId = INVALID_SC_SMARTCARDID;

	/* The event list hasn't been allocate */
	if (pstCaIppvEventLists == 0)
	{
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_POINTER);
		return ;
	}

	ansi_memset((void *)&IppvEventListReply, 0, sizeof(msg_ippv_event_list_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
	{
		return ;
	}
	if (IA_ReadFromMessage(pvMsg, &IppvEventListReply, sizeof(msg_ippv_event_list_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wSCardId = IppvEventListReply.wResourceId;
	bSectorIndex = IppvEventListReply.bSector;

	bSectorNum = CA_SCARD_GetSectorNumber(wSCardId);
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
	if (CA_SCARD_GetSectorType(wSCardId, bSectorIndex) == PPT_SUPPORT ||
		CA_SCARD_GetSectorType(wSCardId, bSectorIndex) == IA_INVALID_PARAMETER)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (pstCaIppvEventLists[i] != 0)
		{
			if (pstCaIppvEventLists[i]->wResourceId == wSCardId)
				break;
		}
	}
	if (i == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	pstTempCaIppvEventLists = pstCaIppvEventLists[i];
	bEventCount = IppvEventListReply.bEventCount;

	/* save the smartcard IPPV events into ca task local database */
	while (pstTempCaIppvEventLists != 0)
	{
		if (pstTempCaIppvEventLists->bSector == bSectorIndex)
		{
			pstTemp = pstTempCaIppvEventLists;
			break;
		}
		pstTempCaIppvEventLists = pstTempCaIppvEventLists->next;
	}
	if (pstTemp == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	pstTemp->bSector = bSectorIndex;
	pstTemp->bIPPVEventCount = bEventCount;
	if (pstTemp->pstEventRecords != 0)
	{
		OS_DRV_FreeMemory((void *)pstTemp->pstEventRecords);
		pstTemp->pstEventRecords = 0;
	}

	if (bEventCount > 0)
	{
		pstTemp->bStatus = 0; /*0:OK indicate that the pstEventRecords contains
								data */
		pstTemp->pstEventRecords = (CA_IPPV_EVENT_RECORDS_T*)
			OS_DRV_AllocateMemory((unsigned long)sizeof(CA_IPPV_EVENT_RECORDS_T)
			*(unsigned long)bEventCount);
		if (pstTemp->pstEventRecords == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_IPPV_EventsListNotify: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}
		for (i = 0; i < bEventCount; i++)
		{
			ansi_memset((void *)&IppvEventListReplyEvent, 0, sizeof(msg_ippv_event_list_reply_event_st));
			IA_ReadFromMessage(pvMsg, &IppvEventListReplyEvent, sizeof(msg_ippv_event_list_reply_event_st));
			pstTemp->pstEventRecords[i].wIPPVEventID = IppvEventListReplyEvent.wEventId;
			pstTemp->pstEventRecords[i].wDateCode = IppvEventListReplyEvent.wDateCode;
			pstTemp->pstEventRecords[i].wEventCost = IppvEventListReplyEvent.wEventCost;
			pstTemp->pstEventRecords[i].fEventReported = IppvEventListReplyEvent.fEventReported;
		}
	}
	else
	{
		pstTemp->bStatus = 1; /*0:OK indicate that the pstEventRecords contains
								data */
		pstTemp->pstEventRecords = 0;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_IPPV_INFO) == IA_TRUE)
	{
		CA_CLIENT_Notify(wSCardId, CA_QC_IPPV_EVENTS_LIST, (ia_word32)((
			(ia_word32)bSectorIndex << 8) | (ia_word32)bEventCount));
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}
/*
* Description: Notify control for CA_IPPV_EVENTS_CHECK type.
*
* Parameters : pstSCellMsg - Pointer to the data buffer from softcell.
*
* Returns    : None.
*
*/
void CA_IPPV_EventCheckNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_ippv_event_check_reply_st IppvEventCheckReply;
	msg_ippv_event_check_reply_event_st IppvEventCheckEvent;
	ia_byte i, m, bEventCount = 0;
	ia_word32 wResourceId = 0;

	ansi_memset((void *)&IppvEventCheckReply, 0, sizeof(msg_ippv_event_check_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &IppvEventCheckReply, sizeof
		(msg_ippv_event_check_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wResourceId = IppvEventCheckReply.wResourceId;

	for (m = 0; m < SC_SLOT_MAX; m++)
	{
		if (wResourceId == astCaIppvEventsChecks[m].wResourceId)
			break;
	}
	if (m == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	astCaIppvEventsChecks[m].bRequestType = REQUEST_TYPE_REPLY;

	if (astCaIppvEventsChecks[m].pstEventCheck != 0)
	{
		OS_DRV_FreeMemory((void *)astCaIppvEventsChecks[m].pstEventCheck);
		astCaIppvEventsChecks[m].pstEventCheck = 0;
	}
	bEventCount = astCaIppvEventsChecks[m].bIPPVEventCount =
		IppvEventCheckReply.bEventCount;

	if (bEventCount > 0)
	{
		astCaIppvEventsChecks[m].pstEventCheck = (CA_IPPV_EVENT_CHECK_T*)
			OS_DRV_AllocateMemory((unsigned long)sizeof(CA_IPPV_EVENT_CHECK_T)*
			(unsigned long)bEventCount);
		if (astCaIppvEventsChecks[m].pstEventCheck == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_IPPV_EventCheckNotify: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}
		astCaIppvEventsChecks[m].bStatus = 0; /* 0 is OK, on "reply", indicate
												that the pstEventRecords contains data. */
		for (i = 0; i < bEventCount; i++)
		{
			ansi_memset((void *)&IppvEventCheckEvent, 0, sizeof(msg_ippv_event_check_reply_event_st));
			IA_ReadFromMessage(pvMsg, &IppvEventCheckEvent, sizeof(msg_ippv_event_check_reply_event_st));
			astCaIppvEventsChecks[m].pstEventCheck[i].fSectorValid = IppvEventCheckEvent.fSectorValid;
			if (IppvEventCheckEvent.fSectorValid == IA_TRUE)
			{
				astCaIppvEventsChecks[m].pstEventCheck[i].bSector = IppvEventCheckEvent.bSector;
			}
			else
			{
				astCaIppvEventsChecks[m].pstEventCheck[i].bSector = INVALID_SC_SECTOR;
			}

			astCaIppvEventsChecks[m].pstEventCheck[i].fEventValid = IppvEventCheckEvent.fEventValid;
			astCaIppvEventsChecks[m].pstEventCheck[i].wEvent = IppvEventCheckEvent.wEventId;
			astCaIppvEventsChecks[m].pstEventCheck[i].wDateCode = IppvEventCheckEvent.wDateCode;
			astCaIppvEventsChecks[m].pstEventCheck[i].fReported = IppvEventCheckEvent.fReported;
			astCaIppvEventsChecks[m].pstEventCheck[i].wCost = IppvEventCheckEvent.wCost;
		}
	}
	else
	{
		astCaIppvEventsChecks[m].bStatus = 1; /* 0 is OK, on "reply", indicate
												that the pstEventRecords contains data. */
		astCaIppvEventsChecks[m].pstEventCheck = 0;
	}


	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_IPPV_INFO) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( (ia_word32)wResourceId , CA_QC_IPPV_EVENT_CHECK , 0);
	}

	IA_CloseMessage(pvMsg, IA_FALSE);

}

/*
*     ROUTINE : CA_IPPV_ParseCallBackRequestNotify()
* DESCRIPTION : Parse MSG_IPPV_CallBack_Request message sent by SoftCell
*       INPUT : pstSCellMsg - Pointer to the data buffer from SoftCell
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : Local storage of the given sector in a given smart card is changed.
*				Fields bPhoneNumberLength, bPhoneControl, wDelay, bDelayMultiplier,
*				and	*pabPhoneNumber is set,
*				where astLocalSCardInfo[i].wResourceID==IppvCallBackRequest.wResourceId,
*				and astLocalSCardInfo[i].pstSectors[j].bSectorID==IppvCallBackRequest.bSector
*   CALLED BY : CaProcessMsg
*      Author : Jane You
* Last Changed: September 23, 2006
*/
void CA_IPPV_ParseCallBackRequestNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_ippv_callback_request_st IppvCallBackRequest;
	ia_byte bSectorIndex, bTemLen = 0, i, bTemPhone;
	ia_word32 wResourceId = 0;
	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	ansi_memset((void *)&IppvCallBackRequest, 0, sizeof(msg_ippv_callback_request_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
	{
		return ;
	}

	if (IA_ReadFromMessage(pvMsg, &IppvCallBackRequest, sizeof
		(msg_ippv_callback_request_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	wResourceId = IppvCallBackRequest.wResourceId;
	bSectorIndex = IppvCallBackRequest.bSector;

	/* jyou: 3.4.2 STB Reporting */
	/* locate the corresponding sector on smartcard */
	pstSector = CA_LOCAL_FindSmartCardSector( wResourceId, bSectorIndex );
	if ( pstSector == 0 )
		/* cann't find the sector */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}

	if ( CA_SCARD_GetSectorType(wResourceId, bSectorIndex) != PPT_SUPPORT &&
		 CA_SCARD_GetSectorType(wResourceId, bSectorIndex) != IA_INVALID_PARAMETER )
	{
		pstSector->bDelayMultiplier = IppvCallBackRequest.bDelayMultiplier;
		pstSector->bPhoneControl = IppvCallBackRequest.bPhoneControl;
		pstSector->wDelay = IppvCallBackRequest.wDelay;
		pstSector->bPhoneNumberLength = bTemLen = IppvCallBackRequest.bPhoneNumberLength;
		if (bTemLen > 0)
		{
			if (pstSector->pabPhoneNumber != 0)
			{
				OS_DRV_FreeMemory((void *)pstSector->pabPhoneNumber);
				pstSector->pabPhoneNumber = 0;

			}
			pstSector->pabPhoneNumber = (ia_byte*)
				OS_DRV_AllocateMemory((unsigned long)sizeof(ia_byte)*(unsigned long)bTemLen);
			if (pstSector->pabPhoneNumber == 0)
			{
				IA_CloseMessage(pvMsg, IA_FALSE);
				CA_ERROR(("CA_IPPV_ParseCallBackRequestNotify: Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return ;
			}
			for (i = 0; i < bTemLen; i++)
			{
				IA_ReadFromMessage(pvMsg, &bTemPhone, sizeof(ia_byte));
				pstSector->pabPhoneNumber[i] = bTemPhone;
			}
		}
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
	/* jyou: 3.4.2 STB Reporting SCR#25330 */
	CA_CLIENT_Notify( wResourceId, CA_QC_IPPV_CALLBACK_REQUEST,
		CA_LOCAL_MakeWord32((ia_word16)bSectorIndex, (ia_word16)bTemLen) );
	return;
}

static ia_bool ReadIPPVCallbackRequestReplyMsg(CA_IPPV_CALLBACK_REQUEST_T *pstMsg)
{
	ia_bool ret = IA_FALSE;
	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	CA_DBG_V(("[ca_ippv]:<= MSG_IPPV_CALLBACK_REQUEST %d\n", pstMsg->bSectorID));

	/* validate input parameter */
	if ( pstMsg->bSectorID >= CA_SCARD_MAX_SECTOR_COUNT )
	{
		return ret;
	}

	/* locate the corresponding sector information */
	pstSector = CA_LOCAL_FindSmartCardSector( pstMsg->wResourceId, pstMsg->bSectorID );
	if ( pstSector == 0 )
		/* cann't find the resource id */
	{
		return ret;
	}

	pstMsg->bDelayMultiplier = pstSector->bDelayMultiplier;
	pstMsg->bPhoneControl = pstSector->bPhoneControl;
	pstMsg->bPhoneNumberLength = pstSector->bPhoneNumberLength;
	pstMsg->wDelay = pstSector->wDelay;
	ansi_memcpy(
		(void*)pstMsg->pabPhoneNumber,
		(void*)pstSector->pabPhoneNumber,
		pstMsg->bPhoneNumberLength );

	return IA_TRUE;
}

/*
*     ROUTINE : CA_IPPV_CallbackRequestQueryControl()
* DESCRIPTION : CA_QC_IPPV_CALLBACK_REQUEST query handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
* SIDE EFFECT : Return the IPPV callback request details of a sector in a smart card from
*					local database g_astLocalSCardInfo to CA Client;
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: September 23, 2006
*/
CA_QUERY_STATUS_EN CA_IPPV_CallbackRequestQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_bool ret;
	CA_IPPV_CALLBACK_REQUEST_T	*pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	CA_PARAM_NOT_USED( wHandle );

	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_IPPV_CALLBACK_REQUEST_T*)pCAClientMsg;
	ret = ReadIPPVCallbackRequestReplyMsg(pstMsg);
	eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	return eStatus ;
}

/*
* Description: Query control for CA_IPPV_PREVIEW type.
*
* Parameters : caIppvPreview - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_IPPV_PreviewQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte i, m;
	CA_IPPV_PREVIEW_T  *pcaIppvPreview	= (CA_IPPV_PREVIEW_T*)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pcaIppvPreview == 0)
	{
		return CA_QUERY_FAIL;
	}
	if (pcaIppvPreview->wGroupHandle == 0)
	{
		return CA_QUERY_FAIL;
	}

	for (m = 0; m < SC_SLOT_MAX; m++)
	{
		if (pcaIppvPreview->wResourceId ==
			astCaIppvPreviewData[m].wResourceId)
			break;
	}
	if (m == SC_SLOT_MAX)
		return CA_QUERY_FAIL;

	if (astCaIppvPreviewData[m].wGroupHandle == 0)
	{
		return CA_QUERY_FAIL;
	}

	for (i = 0; i < astCaIppvPreviewData[m].wServiceCount; i++)
	{
		pcaIppvPreview->wGroupHandle[i] =
			astCaIppvPreviewData[m].wGroupHandle[i];
	}
	pcaIppvPreview->bSector = astCaIppvPreviewData[m].bSector;
	pcaIppvPreview->wIPPVEventID = astCaIppvPreviewData[m].wIPPVEventID;
	pcaIppvPreview->wEventCost = astCaIppvPreviewData[m].wEventCost;
	pcaIppvPreview->bClockValue = astCaIppvPreviewData[m].bClockValue;
	pcaIppvPreview->bPreviewValue = astCaIppvPreviewData[m].bPreviewValue;
	pcaIppvPreview->wServiceCount = astCaIppvPreviewData[m].wServiceCount;

	return CA_QUERY_SUCCESS;
}

/*
* Description: Query control for CA_QC_PARCHASE type.
*
* Parameters : pstMsg - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
static CA_QUERY_STATUS_EN SendIPPVPurchaseQueryMsg( CA_IPPV_PURCHASE_T *pstMsg )
{
	ia_msg_handle pvMsg;
	msg_ippv_buy_query_st IppvBuyQuery;
	ia_byte bSectorIndex = 0;
	ia_word32 wSCardId = 0;

	if (pstMsg == 0)
	{
		return CA_QUERY_FAIL;
	}
	wSCardId = pstMsg->wResourceId;
	if (CA_SCARD_GetGlobalSmartCardStatus(wSCardId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (CA_SCARD_GetSectorType(wSCardId, bSectorIndex) == PPT_SUPPORT ||
		CA_SCARD_GetSectorType(wSCardId, bSectorIndex) == IA_INVALID_PARAMETER)
	{
		return CA_QUERY_FAIL;
	}
	/* save the demux and service handle */
	/*	caIppvPurchaseReply.wDemux = pstMsg->wDemux;
	caIppvPurchaseReply.wHandle = pstMsg->wHandle;
	caIppvPurchaseReply.wIPPVEventID = pstMsg->wIPPVEventId;
	caIppvPurchaseReply.bSector = bSectorIndex;
	*/

	/* send the purchase query message to scell */
	pvMsg = IA_OpenOutputMessage(sizeof(msg_ippv_buy_query_st), sizeof
		(msg_ippv_buy_query_st));
	IppvBuyQuery.wResourceId = pstMsg->wResourceId;
	IppvBuyQuery.wEventId = pstMsg->wIPPVEventID;
	IppvBuyQuery.bSector = pstMsg->bSector;

	ansi_memcpy((void*)IppvBuyQuery.abIppvPin, (void*)pstMsg->bIPPVPin,
		MAX_PINCODE_BYTE_LENGTH);

	IA_WriteToMessage(pvMsg, &IppvBuyQuery, sizeof(msg_ippv_buy_query_st));

	if (SCELL_Message(MSG_IPPV_BUY_QUERY, 0, IA_GetOutputMessageLength(pvMsg),
		IA_GetOutputMessage(pvMsg)) == IA_SUCCESS)
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return CA_QUERY_SUCCESS;
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage(pvMsg, IA_TRUE);
		CA_ERROR(("CA_IPPV_PurchaseQueryControl: purchase query failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}
/*
* Description: Query control for CA_QC_PURCHASE type.
*
* Parameters : pcaIppvBuyReply - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
/*CA_QUERY_STATUS_EN CA_IPPV_PurchaseReplyQueryControl(CA_IPPV_PURCHASE_REPLY_T
													 *pcaIppvBuyReply)*/
static CA_QUERY_STATUS_EN ReadIPPVPurchaseReplyMsg( CA_IPPV_PURCHASE_T *pstMsg )
{
	ia_byte i;
	if (pstMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (pstMsg->wResourceId ==
			astCaIppvPurchaseReply[i].wResourceId)
			break;
	}
	if (i == SC_SLOT_MAX)
		return CA_QUERY_FAIL;

	pstMsg->wGroupHandle = astCaIppvPurchaseReply[i].wGroupHandle;
	pstMsg->wIPPVEventID = astCaIppvPurchaseReply[i].wIPPVEventID;
	pstMsg->bSCpinstatus = astCaIppvPurchaseReply[i].bSCpinstatus;
	pstMsg->bSector = astCaIppvPurchaseReply[i].bSector;
	return CA_QUERY_SUCCESS;
}

/*
* Description: Query control for CA_IPPV_DEBIT_THRESHOLD type.
*
* Parameters : caIppvDebitThreshold - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_NOT_AVAILABLE - Failed. then CA Client must initialize its local database
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_IPPV_SetThresholdQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	ia_byte bSector, bSectorNum;
	msg_ippv_set_debit_threshold_query_st IppvSetThreshold;
	ia_word32 wResourceId;
	CA_IPPV_SET_THRESHOLD_T *pCaIppvDebitThreshold = (CA_IPPV_SET_THRESHOLD_T*)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaIppvDebitThreshold == 0)
	{
		return CA_QUERY_FAIL;
	}
	wResourceId = pCaIppvDebitThreshold->wResourceId;
	bSector = pCaIppvDebitThreshold->bSector;
	/* check input parameters: smartcard id and sector */
	/* the smartcard id is invalidate */
	bSectorNum = CA_SCARD_GetSectorNumber(wResourceId);
	if (bSectorNum == INVALID_SC_SECTOR)
	{
		return CA_QUERY_FAIL;
	}

	if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (bSector >= bSectorNum)
	{
		return CA_QUERY_FAIL;
	}

	if (CA_SCARD_GetSectorType(wResourceId, bSector) == PPT_SUPPORT ||
		CA_SCARD_GetSectorType(wResourceId, bSector) == IA_INVALID_PARAMETER)
	{
		return CA_QUERY_FAIL;
	}

	/* send the set threshold message to scell */
	pvMsg = IA_OpenOutputMessage(sizeof(msg_ippv_set_debit_threshold_query_st),
		sizeof(msg_ippv_set_debit_threshold_query_st));
	IppvSetThreshold.wResourceId = pCaIppvDebitThreshold->wResourceId;
	IppvSetThreshold.bSector = bSector = pCaIppvDebitThreshold->bSector;
	IppvSetThreshold.wDebitThreshold = pCaIppvDebitThreshold->wDebitThreshold;
	IA_WriteToMessage(pvMsg, &IppvSetThreshold, sizeof
		(msg_ippv_set_debit_threshold_query_st));

	if (SCELL_Message(MSG_IPPV_SET_DEBIT_THRESHOLD, 0,
		IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
		IA_SUCCESS)
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return CA_QUERY_SUCCESS;
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage(pvMsg, IA_TRUE);
		CA_ERROR(("CA_IPPV_SetThresholdQueryControl: Send threshold  failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}

/*
* Description: Query control for CA_IPPV_SECTOR_INFO type.
*
* Parameters : pCaIppvSectorInfo - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_IPPV_GetIppvSectorInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_ippv_sector_query_st IppvSectorQuery;
	ia_word32 wSCardId = 0;
	ia_byte bSector = 0, i, bSectorNum;
	CA_IPPV_SECTOR_INFO_T *pstTempCaIppvSecInfo = 0;
	CA_IPPV_SECTOR_INFO_T *pCaIppvSectorInfo = (CA_IPPV_SECTOR_INFO_T*)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaIppvSectorInfo == 0)
	{
		return CA_QUERY_FAIL;
	}
	wSCardId = pCaIppvSectorInfo->wResourceId;
	bSector = pCaIppvSectorInfo->bSector;
	/* check the input parameter smart card id and bsector */
	bSectorNum = CA_SCARD_GetSectorNumber(wSCardId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}
	if (bSector >= bSectorNum)
	{
		return CA_QUERY_FAIL;
	}

	if (CA_SCARD_GetSectorType(wSCardId, bSector) == PPT_SUPPORT ||
		CA_SCARD_GetSectorType(wSCardId, bSector) == IA_INVALID_PARAMETER)
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(wSCardId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (pCaIppvSectorInfo->bRequestType == REQUEST_TYPE_QUERY)
	{
		/* send the sector info query message to scell */
		pvMsg = IA_OpenOutputMessage(sizeof(msg_ippv_sector_query_st), sizeof
			(msg_ippv_sector_query_st));

		IppvSectorQuery.wResourceId = wSCardId;
		IppvSectorQuery.bSector = bSector;

		IA_WriteToMessage(pvMsg, &IppvSectorQuery, sizeof
			(msg_ippv_sector_query_st));

		if (SCELL_Message(MSG_IPPV_SECTOR_INFORMATION_QUERY, 0,
			IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
			IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
			return CA_QUERY_SUCCESS;
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR(("CA_IPPV_GetPursesQueryControl: query purses failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else if (pCaIppvSectorInfo->bRequestType == REQUEST_TYPE_REPLY)
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			if (pstCaIppvSectorInfo[i] != 0)
			{
				if (wSCardId == pstCaIppvSectorInfo[i]->wResourceId)
					break;
			}
		}
		if (i == SC_SLOT_MAX)
		{
			return CA_QUERY_FAIL;
		}
		pstTempCaIppvSecInfo = pstCaIppvSectorInfo[i];
		pCaIppvSectorInfo->wDebitLimit =
			pstTempCaIppvSecInfo[bSector].wDebitLimit;
		pCaIppvSectorInfo->wDebitThreshold =
			pstTempCaIppvSecInfo[bSector].wDebitThreshold;
		pCaIppvSectorInfo->bMaxNoIppvEvents =
			pstTempCaIppvSecInfo[bSector].bMaxNoIppvEvents;
		pCaIppvSectorInfo->wOperatorPurse =
			pstTempCaIppvSecInfo[bSector].wOperatorPurse;
		pCaIppvSectorInfo->wCustomerPurse =
			pstTempCaIppvSecInfo[bSector].wCustomerPurse;
		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}

/*
* Description: Query control for CA_IPPV_EVENT_LIST type.
*
* Parameters : caIppvEventList - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_IPPV_EventsListQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_ippv_event_list_query_st IppvEventListQuery;
	CA_IPPV_EVENTS_LIST_T *pstTemp = 0;
	ia_byte bSector = 0, i, bSectorNum;
	ia_word32 wResourceId = 0;
	CA_IPPV_EVENTS_LIST_T *pTempCaIppvEventList = 0;
	CA_IPPV_EVENTS_LIST_T *pCaIppvEventList = (CA_IPPV_EVENTS_LIST_T*)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaIppvEventList == 0)
	{
		return CA_QUERY_FAIL;
	}
	bSector = pCaIppvEventList->bSector;
	wResourceId = pCaIppvEventList->wResourceId;

	/* check the input parameter smart card id and bsector */
	bSectorNum = CA_SCARD_GetSectorNumber(wResourceId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}
	if (bSector >= bSectorNum)
	{
		return CA_QUERY_FAIL;
	}

	if (CA_SCARD_GetSectorType(wResourceId, bSector) == PPT_SUPPORT ||
		CA_SCARD_GetSectorType(wResourceId, bSector) == IA_INVALID_PARAMETER)
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (pstCaIppvEventLists[i] != 0)
		{
			if (wResourceId == pstCaIppvEventLists[i]->wResourceId)
				break;
		}
	}
	if (i == SC_SLOT_MAX)
	{
		return CA_QUERY_FAIL;
	}

	pTempCaIppvEventList = pstCaIppvEventLists[i];

	if (pCaIppvEventList->bRequestType == REQUEST_TYPE_QUERY)
	{
		/* send the Events List query message to scell */
		pvMsg = IA_OpenOutputMessage(sizeof(msg_ippv_event_list_query_st),
			sizeof(msg_ippv_event_list_query_st));

		IppvEventListQuery.wResourceId = wResourceId;
		IppvEventListQuery.bSector = bSector;
		IppvEventListQuery.bPeriod = pCaIppvEventList->bDaysGrace;
		/* save the input data to the ca task local database */

		while (pTempCaIppvEventList != 0)
		{
			if (pTempCaIppvEventList->bSector == bSector ||
				pTempCaIppvEventList->bSector == INVALID_SC_SECTOR)
			{
				pstTemp = pTempCaIppvEventList;
				break;
			}
			pTempCaIppvEventList = pTempCaIppvEventList->next;
		}
		if (pstTemp == 0)
		{
			IA_CloseMessage(pvMsg, IA_TRUE);
			return CA_QUERY_FAIL;
		}

		pstTemp->bSector = pCaIppvEventList->bSector;
		pstTemp->bDaysGrace = pCaIppvEventList->bDaysGrace;

		IA_WriteToMessage(pvMsg, &IppvEventListQuery, sizeof
			(msg_ippv_event_list_query_st));

		if (SCELL_Message(MSG_IPPV_EVENT_LIST_QUERY, 0,
			IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
			IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
			return CA_QUERY_SUCCESS;
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR((
				"CA_IPPV_EventsListQueryControl: query events list failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else if (pCaIppvEventList->bRequestType == REQUEST_TYPE_REPLY)
	{
		if (pCaIppvEventList->pstEventRecords == 0)
		{
			return CA_QUERY_FAIL;
		}

		while (pTempCaIppvEventList != 0)
		{
			if (pTempCaIppvEventList->bSector == bSector)
			{
				pstTemp = pTempCaIppvEventList;
				break;
			}
			pTempCaIppvEventList = pTempCaIppvEventList->next;
		}
		if (pstTemp == 0)
		{
			return CA_QUERY_FAIL;
		}

		pCaIppvEventList->bStatus = pstTemp->bStatus;
		pCaIppvEventList->bDaysGrace = pstTemp->bDaysGrace;
		pCaIppvEventList->bSector = pstTemp->bSector;
		pCaIppvEventList->bIPPVEventCount = pstTemp->bIPPVEventCount;

		copyEventList(pCaIppvEventList, i, pCaIppvEventList->bSector);
		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}

/*
* Description: Query control for CA_IPPV_EVENTS_CHECK type.
*
* Parameters : caIppvEventsCheck - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_IPPV_EventsCheckQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_ippv_event_check_query_st IppvEventCheckQuery;
	msg_ippv_event_check_query_event_st IppvEventCheckQueryEvent;
	ia_word32 wResourceId = 0;
	ia_byte i, m, bEventCount = 0, bSector = 0xff;
	CA_IPPV_EVENTS_CHECK_T	*pCaIppvEventsCheck = (CA_IPPV_EVENTS_CHECK_T*)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaIppvEventsCheck == 0)
	{
		return CA_QUERY_FAIL;
	}
	/* check smartcardId */
	wResourceId = pCaIppvEventsCheck->wResourceId;
	for (m = 0; m < SC_SLOT_MAX; m++)
	{
		if (wResourceId == astCaIppvEventsChecks[m].wResourceId)
			break;

	}
	if (m == SC_SLOT_MAX)
	{
		return CA_QUERY_FAIL;
	}

	if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (pCaIppvEventsCheck->bRequestType == REQUEST_TYPE_QUERY)
	{
		if (pCaIppvEventsCheck->bIPPVEventCount == 0)
		{
			CA_ERROR(("CA_IPPV_EventsCheckQueryControl: query event is 0.\n"));
			return CA_QUERY_FAIL;
		}

		for (i = 0; i < pCaIppvEventsCheck->bIPPVEventCount; i++)
		{
			bSector = pCaIppvEventsCheck->pstEventCheck[i].bSector;
			/* set to 0xff for any sector */
			if ((bSector >= CA_SCARD_GetSectorNumber(wResourceId)) && (bSector
				!= 0xff))
			{
				return CA_QUERY_FAIL;
			}
			if (CA_SCARD_GetSectorType(wResourceId, bSector) == PPT_SUPPORT ||
				CA_SCARD_GetSectorType(wResourceId, bSector) ==
				IA_INVALID_PARAMETER)
			{
				return CA_QUERY_FAIL;
			}
		}

		astCaIppvEventsChecks[m].bDaysGrace = pCaIppvEventsCheck->bDaysGrace;
		if (astCaIppvEventsChecks[m].pstEventCheck != 0)
		{
			OS_DRV_FreeMemory((void *)astCaIppvEventsChecks[m].pstEventCheck);
			astCaIppvEventsChecks[m].pstEventCheck = 0;
		}
		if (pCaIppvEventsCheck->bIPPVEventCount > 0)
		{
			if (pCaIppvEventsCheck->pstEventCheck == 0)
				return CA_QUERY_FAIL;
		}

		bEventCount = pCaIppvEventsCheck->bIPPVEventCount;

		/* send the Events List query message to scell */
		pvMsg = IA_OpenOutputMessage((ia_word16)(sizeof
			(msg_ippv_event_check_query_st) + sizeof
			(msg_ippv_event_check_query_event_st) *bEventCount), (ia_word16)
			sizeof(msg_ippv_event_check_query_event_st));

		IppvEventCheckQuery.wResourceId = pCaIppvEventsCheck->wResourceId;
		IppvEventCheckQuery.bGracePeriod = pCaIppvEventsCheck->bDaysGrace;
		IppvEventCheckQuery.bEventCount = bEventCount;

		/* save the input data to the ca task local database */
		astCaIppvEventsChecks[m].bRequestType = REQUEST_TYPE_QUERY;
		astCaIppvEventsChecks[m].bIPPVEventCount = bEventCount;
		astCaIppvEventsChecks[m].bDaysGrace = pCaIppvEventsCheck->bDaysGrace;

		IA_WriteToMessage(pvMsg, &IppvEventCheckQuery, sizeof
			(msg_ippv_event_check_query_st));
		if (bEventCount > 0)
		{
			for (i = 0; i < bEventCount; i++)
			{
				IppvEventCheckQueryEvent.bSector = pCaIppvEventsCheck
					->pstEventCheck[i].bSector;
				IppvEventCheckQueryEvent.wEventId = pCaIppvEventsCheck
					->pstEventCheck[i].wEvent;
				IA_WriteToMessage(pvMsg, &IppvEventCheckQueryEvent, sizeof
					(msg_ippv_event_check_query_event_st));
			}
		}

		if (SCELL_Message(MSG_IPPV_EVENT_CHECK_QUERY, 0,
			IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
			IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
			return CA_QUERY_SUCCESS;
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR((
				"CA_IPPV_EventsListQueryControl: query events list failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else if (pCaIppvEventsCheck->bRequestType == REQUEST_TYPE_REPLY)
	{
		pCaIppvEventsCheck->bDaysGrace = astCaIppvEventsChecks[m].bDaysGrace;
		pCaIppvEventsCheck->bIPPVEventCount =
			astCaIppvEventsChecks[m].bIPPVEventCount;
		pCaIppvEventsCheck->bStatus = astCaIppvEventsChecks[m].bStatus;
		if (astCaIppvEventsChecks[m].bIPPVEventCount > 0)
		{
			if (astCaIppvEventsChecks[m].pstEventCheck == 0)
			{
				return CA_QUERY_FAIL;
			}
		}
		if (pCaIppvEventsCheck->pstEventCheck == 0)
		{
			return CA_QUERY_FAIL;
		}

		for (i = 0; i < astCaIppvEventsChecks[m].bIPPVEventCount; i++)
		{
			pCaIppvEventsCheck->pstEventCheck[i].bSector =
				astCaIppvEventsChecks[m].pstEventCheck[i].bSector;
			pCaIppvEventsCheck->pstEventCheck[i].wEvent =
				astCaIppvEventsChecks[m].pstEventCheck[i].wEvent;
			pCaIppvEventsCheck->pstEventCheck[i].fSectorValid =
				astCaIppvEventsChecks[m].pstEventCheck[i].fSectorValid;
			pCaIppvEventsCheck->pstEventCheck[i].fEventValid =
				astCaIppvEventsChecks[m].pstEventCheck[i].fEventValid;
			pCaIppvEventsCheck->pstEventCheck[i].fReported =
				astCaIppvEventsChecks[m].pstEventCheck[i].fReported;
			pCaIppvEventsCheck->pstEventCheck[i].wDateCode =
				astCaIppvEventsChecks[m].pstEventCheck[i].wDateCode;
			pCaIppvEventsCheck->pstEventCheck[i].wCost =
				astCaIppvEventsChecks[m].pstEventCheck[i].wCost;
		}
		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}
/*
* Description: Send a Debit Query Mesage( CA_IPPV_DEBIT type ) to SoftCell .
*
* Parameters : bSector - the sector index .
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_IPPV_SendDebitQuery(ia_word32 wSCardId, ia_byte bSector)
{
	ia_msg_handle pvMsg;
	msg_ippv_sector_query_st IppvSectorQuery;
	ia_byte bSectorNum;

	bSectorNum = CA_SCARD_GetSectorNumber(wSCardId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}

	if (bSector >= bSectorNum)
	{
		return CA_QUERY_FAIL;
	}

	if (CA_SCARD_GetGlobalSmartCardStatus(wSCardId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (CA_SCARD_GetSectorType(wSCardId, bSector) == PPT_SUPPORT ||
		CA_SCARD_GetSectorType(wSCardId, bSector) == IA_INVALID_PARAMETER)
	{
		return CA_QUERY_FAIL;
	}

	/* send the Events List query message to scell */
	pvMsg = IA_OpenOutputMessage(sizeof(msg_ippv_sector_query_st), sizeof
		(msg_ippv_sector_query_st));
	IppvSectorQuery.bSector = bSector;
	IppvSectorQuery.wResourceId = SC_SLOT_0;
	IA_WriteToMessage(pvMsg, &IppvSectorQuery, sizeof(msg_ippv_sector_query_st));

	if (SCELL_Message(MSG_IPPV_SECTOR_INFORMATION_QUERY, 0,
		IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
		IA_SUCCESS)
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return CA_QUERY_SUCCESS;
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage(pvMsg, IA_TRUE);
		CA_ERROR(("CA_IPPV_SendDebitQuery: Send debit failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}

static void copyEventList(CA_IPPV_EVENTS_LIST_T *pCaIppvEventList, ia_byte
							bIndex, ia_byte bSector)
{
	ia_int32 i, iCount;
	CA_IPPV_EVENTS_LIST_T *pstTemp = 0;
	CA_IPPV_EVENTS_LIST_T *pTempCaIppvEventList = 0;

	if (bIndex > SC_SLOT_MAX-1)
	{
		CA_ERROR( ("copyEventList index is too big.\n") );
		return;
	}
	if (pstCaIppvEventLists[bIndex] == 0)
	{
		/* The IPPV events hasn't been allocated */
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_POINTER);
		return ;
	}
	pTempCaIppvEventList = pstCaIppvEventLists[bIndex];
	while (pTempCaIppvEventList != 0)
	{
		if (pTempCaIppvEventList->bSector == bSector)
		{
			pstTemp = pTempCaIppvEventList;
			break;
		}
		pTempCaIppvEventList = pTempCaIppvEventList->next;
	}
	if (pstTemp == 0)
	{
		return ;
	}

	iCount = pstTemp->bIPPVEventCount;

	pCaIppvEventList->bIPPVEventCount = (ia_byte)iCount;
	for (i = 0; i < iCount; i++)
	{
		pCaIppvEventList->pstEventRecords[i] = pstTemp->pstEventRecords[i];
	}
	if (pstTemp->pstEventRecords != 0)
	{
		OS_DRV_FreeMemory((void *)pstTemp->pstEventRecords);
		pstTemp->pstEventRecords = 0;
	}

}

void CA_IPPV_MemoryFree()
{
	ia_byte i,j;
	CA_IPPV_EVENTS_LIST_T *pTemp = 0;
	CA_IPPV_EVENTS_LIST_T *pTempCaIppvEventLists = 0;
	ia_word32 wSCardId;
	ia_byte bSectorNum;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (pstCaIppvSectorInfo[i] != 0)
		{
			OS_DRV_FreeMemory((void *)pstCaIppvSectorInfo[i]);
			pstCaIppvSectorInfo[i] = 0;
		}

		pTempCaIppvEventLists = pstCaIppvEventLists[i];
		while (pTempCaIppvEventLists != 0)
		{
			pTemp = pTempCaIppvEventLists;
			pTempCaIppvEventLists = pTempCaIppvEventLists->next;

			if (pTemp->pstEventRecords != 0)
			{
				OS_DRV_FreeMemory((void *)pTemp->pstEventRecords);
				pTemp->pstEventRecords = 0;
			}
		}
		if (0 != pstCaIppvEventLists[i])
		{
			OS_DRV_FreeMemory((void *)pstCaIppvEventLists[i]);
			pstCaIppvEventLists[i] = 0;
		}

		if (astCaIppvEventsChecks[i].pstEventCheck != 0)
		{
			OS_DRV_FreeMemory((void *)astCaIppvEventsChecks[i].pstEventCheck);
			astCaIppvEventsChecks[i].pstEventCheck = 0;
		}

		if (astCaIppvPreviewData[i].wGroupHandle != 0)
		{
			OS_DRV_FreeMemory((void *)astCaIppvPreviewData[i].wGroupHandle);
			astCaIppvPreviewData[i].wGroupHandle = 0;
		}

		wSCardId = CA_SCARD_GetSmartCardId(i);
		if (wSCardId != INVALID_SC_SMARTCARDID)
		{
			bSectorNum = CA_SCARD_GetSectorNumber(wSCardId);
			for (j=0; j<bSectorNum; j++)
			{
				if (pstCaIppvPhoneNumber[i][j].pabPhoneNumber != 0)
				{
					OS_DRV_FreeMemory((void *)pstCaIppvPhoneNumber[i][j].pabPhoneNumber);
					pstCaIppvPhoneNumber[i][j].pabPhoneNumber = 0;
				}
			}
		}

		if (pstCaIppvPhoneNumber[i] != 0)
		{
			OS_DRV_FreeMemory((void *)pstCaIppvPhoneNumber[i]);
			pstCaIppvPhoneNumber[i] = 0;
		}

	}
}

/* jyou: 3.4.2 SCR#25311 */
static ia_word16 FindSmartCardIndex( ia_word32 wResourceID )
{
	ia_word16 i;

	for ( i = 0 ; i < SC_SLOT_MAX ; i++ )
	{
		if ( 0 != pstCaIppvEventLists[i] )
		{
			if ( pstCaIppvEventLists[i]->wResourceId == wResourceID )
			{
				break;
			}
		}
	}
	return i;
}

void CA_IPPV_SetSectorBySectorIndex(ia_word32 wResourceId, ia_byte bSector)
{
	CA_IPPV_EVENTS_LIST_T *pTempCaIppvEventList = 0;
	/* jyou: 3.4.2 SCR#25311 */
	ia_word16 i;

	i = FindSmartCardIndex( wResourceId );
	if ( i == SC_SLOT_MAX )
	{
		CA_ERROR(("[CA_IPPV_SetSectorBySectorIndex]: Can't locate resource ID %d.\n", wResourceId));
		return;
	}
	/* Get the pointer to the local storage of the resource ID */
	pTempCaIppvEventList = pstCaIppvEventLists[i];


	while (pTempCaIppvEventList != 0)
	{
		if (pTempCaIppvEventList->bSector == INVALID_SC_SECTOR)
		{
			pTempCaIppvEventList->bSector = bSector;
			break;
		}
		pTempCaIppvEventList = pTempCaIppvEventList->next;
	}
	return;
}


CA_QUERY_STATUS_EN CA_IPPV_PurchaseQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	CA_IPPV_PURCHASE_T	*pstMsg;
	CA_QUERY_STATUS_EN	eStatus;

	CA_PARAM_NOT_USED( wHandle );
	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_IPPV_PURCHASE_T*)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		eStatus = SendIPPVPurchaseQueryMsg(pstMsg);
	}
	else
	{
		eStatus = ReadIPPVPurchaseReplyMsg(pstMsg);
	}

	return eStatus ;
}

CA_QUERY_STATUS_EN CA_IPPV_PhoneNumber_QueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_ippv_phone_number_query_data_st IppvPhoneNumberQuery;
	ia_byte bSector = 0, i, bSectorNum;
	ia_word32 wResourceId = 0;
	CA_IPPV_PHONE_NUMBER_T *pTmpCaIppvPhoneNumber = 0;
	CA_IPPV_PHONE_NUMBER_T *pCaIppvPhoneNumber = (CA_IPPV_PHONE_NUMBER_T *)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaIppvPhoneNumber == 0)
	{
		return CA_QUERY_FAIL;
	}
	bSector = pCaIppvPhoneNumber->bSectorID;
	wResourceId = pCaIppvPhoneNumber->wResourceId;

	/* check the input parameter smart card id and bsector */
	bSectorNum = CA_SCARD_GetSectorNumber(wResourceId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}
	if (bSector >= bSectorNum)
	{
		return CA_QUERY_FAIL;
	}

	if (CA_SCARD_GetSectorType(wResourceId, bSector) == PPT_SUPPORT ||
		CA_SCARD_GetSectorType(wResourceId, bSector) == IA_INVALID_PARAMETER)
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (wResourceId == pstCaIppvPhoneNumber[i]->wResourceId)
			break;
	}
	if (i == SC_SLOT_MAX)
	{
		return CA_QUERY_FAIL;
	}

	pTmpCaIppvPhoneNumber = pstCaIppvPhoneNumber[i];

	if (pCaIppvPhoneNumber->bRequestType == REQUEST_TYPE_QUERY)
	{
		/* send the Events List query message to scell */
		pvMsg = IA_OpenOutputMessage(sizeof(msg_ippv_phone_number_query_data_st),
			sizeof(msg_ippv_phone_number_query_data_st));

		IppvPhoneNumberQuery.wResourceId = wResourceId;
		IppvPhoneNumberQuery.bSector = bSector;

		IA_WriteToMessage(pvMsg, &IppvPhoneNumberQuery, sizeof
			(msg_ippv_phone_number_query_data_st));

		if (SCELL_Message(MSG_IPPV_PHONE_NUMBER_QUERY, 0,
			IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
			IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
			return CA_QUERY_SUCCESS;
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR((
				"CA_IPPV_PhoneNumberQueryControl: IPPV Phone Number Query Failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else if (pCaIppvPhoneNumber->bRequestType == REQUEST_TYPE_REPLY)
	{
		pCaIppvPhoneNumber->wResourceId = pTmpCaIppvPhoneNumber[bSector].wResourceId;
		pCaIppvPhoneNumber->bSectorID = pTmpCaIppvPhoneNumber[bSector].bSectorID;
		pCaIppvPhoneNumber->bPhoneNumberLength = pTmpCaIppvPhoneNumber[bSector].bPhoneNumberLength;

		if (pCaIppvPhoneNumber->bPhoneNumberLength == 0)
		{
			return CA_QUERY_FAIL;
		}

		ansi_memcpy((void *)&pCaIppvPhoneNumber->pabPhoneNumber[0],
			(void *)&pTmpCaIppvPhoneNumber[bSector].pabPhoneNumber[0],
			sizeof(ia_byte) * pCaIppvPhoneNumber->bPhoneNumberLength);

		if (pTmpCaIppvPhoneNumber[bSector].pabPhoneNumber != 0)
		{
			OS_DRV_FreeMemory((void *)pTmpCaIppvPhoneNumber[bSector].pabPhoneNumber);
			pTmpCaIppvPhoneNumber[bSector].pabPhoneNumber = 0;
		}

		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}

void CA_IPPV_PhoneNumber_Notify(const CA_SCELL_MSG *pstSCellMsg)
{
	CA_IPPV_PHONE_NUMBER_T *pstTempCaIppvPhoneNumber = 0;
	ia_msg_handle pvMsg;
	msg_ippv_phone_number_data_st IppvEventListReply;
	ia_byte bSectorIndex, bSectorNum;
	ia_byte i = 0;
	ia_word32 wSCardId = INVALID_SC_SMARTCARDID;

	/* The event list hasn't been allocate */
	ansi_memset((void *)&IppvEventListReply, 0, sizeof(msg_ippv_phone_number_data_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
	{
		return ;
	}
	if (IA_ReadFromMessage(pvMsg, &IppvEventListReply, sizeof(msg_ippv_phone_number_data_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wSCardId = IppvEventListReply.wResourceId;
	bSectorIndex = IppvEventListReply.bSector;

	bSectorNum = CA_SCARD_GetSectorNumber(wSCardId);
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

	if (CA_SCARD_GetSectorType(wSCardId, bSectorIndex) == PPT_SUPPORT ||
		CA_SCARD_GetSectorType(wSCardId, bSectorIndex) == IA_INVALID_PARAMETER)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (pstCaIppvPhoneNumber[i]->wResourceId == wSCardId)
			break;
	}
	if (i == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	pstTempCaIppvPhoneNumber = pstCaIppvPhoneNumber[i];

	/* save the smartcard IPPV phone number into ca task local database */
	pstTempCaIppvPhoneNumber[bSectorIndex].wResourceId = wSCardId;
	pstTempCaIppvPhoneNumber[bSectorIndex].bSectorID = bSectorIndex;
	pstTempCaIppvPhoneNumber[bSectorIndex].bPhoneNumberLength = IppvEventListReply.bPhoneNumberLength;

	if (IppvEventListReply.bPhoneNumberLength != 0)
	{
		pstTempCaIppvPhoneNumber[bSectorIndex].pabPhoneNumber = (ia_byte *)
			OS_DRV_AllocateMemory((unsigned long)sizeof(ia_byte)*
			(unsigned long)IppvEventListReply.bPhoneNumberLength);
		if (pstTempCaIppvPhoneNumber[bSectorIndex].pabPhoneNumber == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_IPPV_PhoneNumber_Notify: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}

		ansi_memset((void*)&pstTempCaIppvPhoneNumber[bSectorIndex].pabPhoneNumber[0], 0x00,
			sizeof(ia_byte) * IppvEventListReply.bPhoneNumberLength);
		if (IA_ReadFromMessage(pvMsg, &pstTempCaIppvPhoneNumber[bSectorIndex].pabPhoneNumber[0], IppvEventListReply.bPhoneNumberLength) == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return ;

		}
	}
	else
	{
		pstTempCaIppvPhoneNumber[bSectorIndex].pabPhoneNumber = 0;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_IPPV_INFO) == IA_TRUE)
	{
		CA_CLIENT_Notify(wSCardId, CA_QC_IPPV_PHONE_NUMBER, (ia_word32)((
			(ia_word32)bSectorIndex << 16) | (ia_word32)IppvEventListReply.bPhoneNumberLength));
	}
	IA_CloseMessage(pvMsg, IA_FALSE);

}
