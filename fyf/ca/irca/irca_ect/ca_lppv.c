/**
 * $Id: ca_lppv.c,v 1.5 2008/05/09 08:55:08 ayang Exp $
 *
 * Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
 *
 * The LPPV module of Embedded CA Task.
 **/

/******************************************************************************
* FILE		  : CA_LPPV.C												  	*
*																			*
* DESCRIPTION   :															*
*																			*
* NOTES		 :																*
*																			*
* STATUS/HISTORY:															*
* Author		  : Angela Yang											  	*
* Status		  : $Revision: 1.5 $									*
* History		 :															*
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
#include <irca/pubh/s3_lppv_msg.h>
#include "ca_lppv.h"
#include "ca_error.h"

/******************************* Local Variables ******************************/
static CA_LOCAL_LPPV_SCARD_INFO_T m_astLocalLppvSCardInfo[SC_SLOT_MAX];

/******************************* Local Functions ******************************/
static CA_LOCAL_LPPV_SECTOR_INFO_T* CA_LOCAL_Lppv_FindSmartCardSector( ia_word32 wResourceId, ia_byte bSectorID );
static ia_byte  CA_LOCAL_Lppv_FindSmartCardLocation(ia_word32 wResourceId);
static ia_byte CA_LOCAL_Lppv_AddSmartCardInfo(ia_word32 wResourceID);
static ia_byte CA_LOCAL_Lppv_GetSectorType(ia_word32 wResourceId, ia_byte bIndex);

static void ResetLppvSmartCardSectorInfo( CA_LOCAL_LPPV_SECTOR_INFO_T *pstInfo )
{
	pstInfo->bSectorID = INVALID_SC_SECTOR;
	return;
}

static void ResetLppvSmartCardSectorCredit( CA_LOCAL_LPPV_SECTOR_INFO_T *pstInfo )
{
	pstInfo->wCredit = 0;
	pstInfo->wCurrentDC = 0;
	pstInfo->wExpiryDC = 0;
	pstInfo->fClearOnExpiry = IA_FALSE;
	pstInfo->wLastIncreaseCredits = 0;
	pstInfo->wLastIncreaseDC = 0;
	return;
}

static void ResetLppvSmartCardSectorTicket( CA_LOCAL_LPPV_SECTOR_INFO_T *pstInfo )
{
	if ( pstInfo->bTicketCount > 0 && pstInfo->pstReadTickets != 0 )
	{
		OS_DRV_FreeMemory( (void *)pstInfo->pstReadTickets );
	}
	pstInfo->pstReadTickets = 0;
	pstInfo->bTicketCount = 0;
	return;
}

static void ResetLppvSmartCardSectorOrder( CA_LOCAL_LPPV_SECTOR_INFO_T *pstInfo )
{
	if ( pstInfo->bOrderCount> 0 && pstInfo->pstOrders!= 0 )
	{
		OS_DRV_FreeMemory( (void *)pstInfo->pstOrders );
	}
	pstInfo->bOrderCount = 0;
	pstInfo->pstOrders = 0;
	return;
}

static void ResetLppvSmartCardSectors(CA_LOCAL_LPPV_SCARD_INFO_T *pstInfo)
{

	ia_byte i;

	if ( pstInfo->bSectorCount > 0 )
	{
		if ( pstInfo->pstSectorInfo != 0 )
		{
			for ( i = 0 ; i < pstInfo->bSectorCount ; i++ )
			{
				/* Free internal memories in Sector Info */
				ResetLppvSmartCardSectorInfo( pstInfo->pstSectorInfo + i );
				ResetLppvSmartCardSectorCredit( pstInfo->pstSectorInfo + i );
				ResetLppvSmartCardSectorTicket( pstInfo->pstSectorInfo + i );
				ResetLppvSmartCardSectorOrder( pstInfo->pstSectorInfo + i );
			}
			OS_DRV_FreeMemory( (void *)pstInfo->pstSectorInfo );
		}
	}
	pstInfo->bSectorCount = 0;
	pstInfo->pstSectorInfo = 0;
	return;
}

static void ResetLppvSmartCardPreview( CA_LOCAL_LPPV_SCARD_INFO_T *pstInfo )
{
	if ( pstInfo->bEventTicketCount > 0 && pstInfo->pstEventTickets != 0 )
	{
		OS_DRV_FreeMemory( (void *)pstInfo->pstEventTickets );
	}
	pstInfo->bEventTicketCount = 0;
	pstInfo->pstEventTickets = 0;
	return;
}

static void CA_LPPV_Reset(ia_byte bIndex, ia_word32 wResourceId)
{
	CA_LOCAL_LPPV_SCARD_INFO_T *pstTemp = 0;

	CA_PARAM_NOT_USED(astrSCellMsgDisplay); /* to supress warning */

	if ( bIndex > SC_SLOT_MAX-1)
	{
		CA_ERROR(("CA_LPPV_Reset bIndex= %d is too big!\r\n", bIndex));
		return;
	}
	pstTemp = &m_astLocalLppvSCardInfo[bIndex];

	pstTemp->wResourceId = wResourceId;
	ResetLppvSmartCardSectors( pstTemp );
	ResetLppvSmartCardPreview( pstTemp );
}

/*
* Description: Initializes the LPPV local database.
*
* Parameters : None.
*
* Returns	: None.
*/
void CA_LPPV_Initialise(ia_word32 wResourceId)
{
	ia_byte i;

	if (wResourceId == INVALID_SMARTCARDID)
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			CA_LPPV_Reset(i, wResourceId);
		}
	}
	else
	{
		i = CA_LOCAL_Lppv_AddSmartCardInfo( wResourceId );
		if (i == SC_SLOT_INVALID)
		{
			/* cann't allocate the resource id */
			return;
		}
		CA_LPPV_Reset(i, wResourceId);
	}
}

/*
* Description: Free the LPPV allocated memory.
*
* Parameters : None.
*
* Returns	: None.
*/
void CA_LPPV_MemoryFree(void)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		CA_LPPV_Reset( i, INVALID_SC_SMARTCARDID ); /*clear all the memories*/
	}

	return;
}

/********************* LPPV QC functions *********************/
/********************* CA_LPPV_AwareQueryControl *********************/
/* Description: Query from Softcell/Reply to Client :the LPPV aware status.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : wHandle	  - .
*				   pCAClientMsg - Query message;
*
** Returns	: CA_QUERY_FAIL- Query/Reply failed.
*					 CA_QUERY_SUCCESS-Query/Reply success
*********************************************************************/
CA_QUERY_STATUS_EN CA_LPPV_AwareQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_lppv_aware_query_st LppvAwareQuery;
	ia_byte i, j,bSectorNum;
	CA_LPPV_AWARE_T *pCaLppvAwareInfo = (CA_LPPV_AWARE_T*)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaLppvAwareInfo == 0)
	{
		return CA_QUERY_FAIL;
	}

	/* check the input parameter smart card id and bsector */
	bSectorNum = CA_SCARD_GetSectorNumber(pCaLppvAwareInfo->wResourceId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(pCaLppvAwareInfo->wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (pCaLppvAwareInfo->bRequestType == REQUEST_TYPE_QUERY)
	{
		/* send the aware info query message to scell */
		pvMsg = IA_OpenOutputMessage(sizeof(msg_lppv_aware_query_st), sizeof
			(msg_lppv_aware_query_st));

		LppvAwareQuery.wResourceId = pCaLppvAwareInfo->wResourceId;
		LppvAwareQuery.bSTBPPVMode = pCaLppvAwareInfo->bSTBPPVMode;

		IA_WriteToMessage(pvMsg, &LppvAwareQuery, sizeof
			(msg_lppv_aware_query_st));

		if (SCELL_Message(MSG_LPPV_AWARE_QUERY, 0,
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
			CA_ERROR(("CA_LPPV_AwareQueryControl: query failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	if (pCaLppvAwareInfo->bRequestType == REQUEST_TYPE_REPLY)
	{
		i = CA_LOCAL_Lppv_FindSmartCardLocation( pCaLppvAwareInfo->wResourceId );
		if (i == SC_SLOT_INVALID)
		{
			/* cann't find the resource id */
			return CA_QUERY_FAIL;
		}

		pCaLppvAwareInfo->bSCPPVMode = m_astLocalLppvSCardInfo[i].bSCPPVMode;
		pCaLppvAwareInfo->bSectorCount = m_astLocalLppvSCardInfo[i].bSectorCount;
		for(j=0;j<pCaLppvAwareInfo->bSectorCount;j++)
		{
			pCaLppvAwareInfo->pstLPPVMode[j].bSector = m_astLocalLppvSCardInfo[i].pstSectorInfo[j].bSectorID;
			pCaLppvAwareInfo->pstLPPVMode[j].bSCPPVMode = m_astLocalLppvSCardInfo[i].pstSectorInfo[j].bSCPPVMode;
		}

		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}

/********************* CA_LPPV_PreviewQueryControl *********************/
/* Description: Reply to Client :the LPPV preview status when a new event come.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : wHandle	  - .
*				   pCAClientMsg - Query message;
*
** Returns	: CA_QUERY_FAIL- Query/Reply failed.
*					 CA_QUERY_SUCCESS-Query/Reply success
*********************************************************************/
CA_QUERY_STATUS_EN CA_LPPV_PreviewQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte i, m;
	CA_LPPV_PREVIEW_T  *pcaLppvPreview = (CA_LPPV_PREVIEW_T*)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pcaLppvPreview == 0)
	{
		return CA_QUERY_FAIL;
	}

	m = CA_LOCAL_Lppv_FindSmartCardLocation( pcaLppvPreview->wResourceId );
	if (m == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return CA_QUERY_FAIL;
	}

	if (m_astLocalLppvSCardInfo[m].pstEventTickets == 0)
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(pcaLppvPreview->wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (CA_LOCAL_Lppv_GetSectorType(pcaLppvPreview->wResourceId, pcaLppvPreview->bSector) != 1)
	{
		return CA_QUERY_FAIL;
	}

	pcaLppvPreview->wResourceId = m_astLocalLppvSCardInfo[m].wResourceId;
	pcaLppvPreview->bSector = m_astLocalLppvSCardInfo[m].bPreviewSectorId;
	pcaLppvPreview->bPreviewWindow = m_astLocalLppvSCardInfo[m].bPreviewWindow;
	pcaLppvPreview->bBuyWindow = m_astLocalLppvSCardInfo[m].bBuyWindow;
	pcaLppvPreview->bTicketCount = m_astLocalLppvSCardInfo[m].bEventTicketCount;

	for (i = 0; i < m_astLocalLppvSCardInfo[m].bEventTicketCount; i++)
	{
		pcaLppvPreview->pstEventTickets[i] = m_astLocalLppvSCardInfo[m].pstEventTickets[i];
	}

	if (m_astLocalLppvSCardInfo[m].bEventTicketCount != 0)
	{
		OS_DRV_FreeMemory((void *)m_astLocalLppvSCardInfo[m].pstEventTickets);
		m_astLocalLppvSCardInfo[m].pstEventTickets = 0;
	}

	return CA_QUERY_SUCCESS;
}
/********************* CA_LPPV_ReadTicketQueryControl *********************/
/* Description: Query from Softcell/Reply to Client :the ticket information of one sector of one smartcard.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : wHandle	  - .
*				   pCAClientMsg - Query message;
*
** Returns	: CA_QUERY_FAIL- Query/Reply failed.
*					 CA_QUERY_SUCCESS-Query/Reply success
*********************************************************************/
CA_QUERY_STATUS_EN CA_LPPV_ReadTicketQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_lppv_read_ticket_query_st LppvReadTicketQuery;
	ia_byte bSector = 0, j,bSectorNum ,TicketMode, m;
	ia_word32 wResourceId = 0;
	CA_LPPV_READ_TICKET_T *pCaLppvTicketList = (CA_LPPV_READ_TICKET_T*)pCAClientMsg;
	CA_LOCAL_LPPV_SECTOR_INFO_T *pstSecInfo;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaLppvTicketList == 0)
	{
		return CA_QUERY_FAIL;
	}
	bSector = pCaLppvTicketList->bSector;
	wResourceId = pCaLppvTicketList->wResourceId;
	TicketMode = pCaLppvTicketList->bTicketMode;

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
	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSector) != 1)
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	m = CA_LOCAL_Lppv_FindSmartCardLocation( wResourceId );
	if (m == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return CA_QUERY_FAIL;
	}

	if (pCaLppvTicketList->bRequestType == REQUEST_TYPE_QUERY)
	{
		/* send the Read Ticket query message to scell */
		pvMsg = IA_OpenOutputMessage(sizeof(msg_lppv_read_ticket_query_st),
			sizeof(msg_lppv_read_ticket_query_st));

		LppvReadTicketQuery.wResourceId = wResourceId;
		LppvReadTicketQuery.bSector = bSector;
		LppvReadTicketQuery.bTicketMode  = TicketMode;

		IA_WriteToMessage(pvMsg, &LppvReadTicketQuery, sizeof
			(msg_lppv_read_ticket_query_st));

		if (SCELL_Message(MSG_LPPV_READ_TICKET_QUERY, 0,
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
				"CA_LPPV_ReadTicketQueryControl: query ticket failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	if (pCaLppvTicketList->bRequestType == REQUEST_TYPE_REPLY)
	{
		pstSecInfo = CA_LOCAL_Lppv_FindSmartCardSector( wResourceId, bSector );
		if (pstSecInfo == 0)
		{
			return CA_QUERY_FAIL;
		}
		if ((pstSecInfo->bTicketCount > 0)&&(pstSecInfo->pstReadTickets == 0))
		{
			return CA_QUERY_FAIL;
		}

		pCaLppvTicketList->bTicketCount = pstSecInfo->bTicketCount;
		for (j = 0; j < pstSecInfo->bTicketCount; j++)
		{
			pCaLppvTicketList->pstReadTickets[j] = pstSecInfo->pstReadTickets[j];
		}
		/*after send the data to client , delete it from ECT */
		if (pstSecInfo->pstReadTickets != 0)
		{
			OS_DRV_FreeMemory((void *)pstSecInfo->pstReadTickets);
			pstSecInfo->pstReadTickets = 0;
		}

		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}
/********************* CA_LPPV_DeleteTicketQueryControl ****************/
/* Description: Query from Softcell/Reply to Client :the result of the acrion of delete ticket.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : wHandle	  - .
*				   pCAClientMsg - Query message;
*
** Returns	: CA_QUERY_FAIL- Query/Reply failed.
*					 CA_QUERY_SUCCESS-Query/Reply success
*********************************************************************/
CA_QUERY_STATUS_EN CA_LPPV_DeleteTicketQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_lppv_delete_ticket_query_st LppvDeleteTicketQuery;
	ia_byte bSector = 0,  bSectorNum;
	ia_word16 TicketId;
	ia_word32 wResourceId = 0;
	CA_LPPV_DELETE_TICKET_T *pCaLppvTicketDelete = (CA_LPPV_DELETE_TICKET_T*)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaLppvTicketDelete == 0)
	{
		return CA_QUERY_FAIL;
	}
	bSector = pCaLppvTicketDelete->bSector;
	wResourceId = pCaLppvTicketDelete->wResourceId;
	TicketId = pCaLppvTicketDelete->wTicketId;

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
	/*CA_LOCAL_Lppv_GetSectorType(wResourceId, bSector) == PPT_SUPPORT ||*/
	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSector) != 1)
	{
		 return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	/* send the Read Ticket query message to scell */
	pvMsg = IA_OpenOutputMessage(sizeof(msg_lppv_delete_ticket_query_st),
		sizeof(msg_lppv_delete_ticket_query_st));

	LppvDeleteTicketQuery.wResourceId = wResourceId;
	LppvDeleteTicketQuery.bSector = bSector;
	LppvDeleteTicketQuery.wTicketId  = TicketId;
	LppvDeleteTicketQuery.abPIN[0] = pCaLppvTicketDelete->abPIN[0];
	LppvDeleteTicketQuery.abPIN[1] = pCaLppvTicketDelete->abPIN[1];


	IA_WriteToMessage(pvMsg, &LppvDeleteTicketQuery, sizeof
		(msg_lppv_delete_ticket_query_st));

	if (SCELL_Message(MSG_LPPV_DELETE_TICKET_QUERY, 0,
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
			"CA_LPPV_DeleteTicketQueryControl: query ticket failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}


CA_QUERY_STATUS_EN CA_LPPV_ReadCreditQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_lppv_read_credit_query_st LppvReadCreditQuery;
	ia_word32 wResourceId = 0;
	ia_byte bSector = 0, bSectorNum;
	CA_LPPV_CREDIT_INFO_T *pCaLppvCreditInfo = (CA_LPPV_CREDIT_INFO_T *)pCAClientMsg;
	CA_LOCAL_LPPV_SECTOR_INFO_T *pstSecInfo;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaLppvCreditInfo == 0)
	{
		return CA_QUERY_FAIL;
	}
	wResourceId = pCaLppvCreditInfo->wResourceId;
	bSector = pCaLppvCreditInfo->bSector;

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

	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSector) != 1)
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (pCaLppvCreditInfo->bRequestType == REQUEST_TYPE_QUERY)
	{
		/* send the sector info query message to scell */
		pvMsg = IA_OpenOutputMessage(sizeof(msg_lppv_read_credit_query_st), sizeof
			(msg_lppv_read_credit_query_st));

		LppvReadCreditQuery.wResourceId = wResourceId;
		LppvReadCreditQuery.bSector = bSector;

		IA_WriteToMessage(pvMsg, &LppvReadCreditQuery, sizeof
			(msg_lppv_read_credit_query_st));

		if (SCELL_Message(MSG_LPPV_READ_CREDIT_QUERY, 0,
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
			CA_ERROR(("CA_LPPV_ReadCreditQueryControl: Read credit query failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else if (pCaLppvCreditInfo->bRequestType == REQUEST_TYPE_REPLY)
	{
		pstSecInfo = CA_LOCAL_Lppv_FindSmartCardSector( wResourceId, bSector );
		if (pstSecInfo == 0)
		{
			return CA_QUERY_FAIL;
		}
		pCaLppvCreditInfo->wCredit = pstSecInfo->wCredit;
		pCaLppvCreditInfo->wCurrentDC = pstSecInfo->wCurrentDC;
		pCaLppvCreditInfo->wExpiryDC = pstSecInfo->wExpiryDC;
		pCaLppvCreditInfo->fClearOnExpiry = pstSecInfo->fClearOnExpiry;
		pCaLppvCreditInfo->wLastIncreaseCredits = pstSecInfo->wLastIncreaseCredits;
		pCaLppvCreditInfo->wLastIncreaseDC = pstSecInfo->wLastIncreaseDC;
		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;

}

CA_QUERY_STATUS_EN CA_LPPV_ReadOrderQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_lppv_read_order_query_st LppvOrderListQuery;
	ia_byte bSector = 0, i, bSectorNum;
	ia_word32 wResourceId = 0;
	CA_LPPV_READ_ORDER_T *pCaLppvOrderList = (CA_LPPV_READ_ORDER_T *)pCAClientMsg;
	CA_LOCAL_LPPV_SECTOR_INFO_T *pstSecInfo;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaLppvOrderList == 0)
	{
		return CA_QUERY_FAIL;
	}
	bSector = pCaLppvOrderList->bSector;
	wResourceId = pCaLppvOrderList->wResourceId;

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

	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSector) != 1)
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	i = CA_LOCAL_Lppv_FindSmartCardLocation( wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return CA_QUERY_FAIL;
	}

	if (pCaLppvOrderList->bRequestType == REQUEST_TYPE_QUERY)
	{
		/* send the Order List query message to scell */
		pvMsg = IA_OpenOutputMessage(sizeof(msg_lppv_read_order_query_st),
			sizeof(msg_lppv_read_order_query_st));

		LppvOrderListQuery.wResourceId = wResourceId;
		LppvOrderListQuery.bSector = bSector;

		/* save the input data to the ca task local database */
		IA_WriteToMessage(pvMsg, &LppvOrderListQuery, sizeof
			(msg_lppv_read_order_query_st));

		if (SCELL_Message(MSG_LPPV_READ_ORDER_QUERY, 0,
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
				"CA_LPPV_ReadOrderQueryControl: Read order list failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else if (pCaLppvOrderList->bRequestType == REQUEST_TYPE_REPLY)
	{
		if (pCaLppvOrderList->pstOrders== 0)
		{
			return CA_QUERY_FAIL;
		}

		pstSecInfo = CA_LOCAL_Lppv_FindSmartCardSector( wResourceId, bSector );
		if (pstSecInfo == 0)
		{
			return CA_QUERY_FAIL;
		}
		if ((pstSecInfo->bOrderCount > 0)&&(pstSecInfo->pstOrders == 0))
		{
			return CA_QUERY_FAIL;
		}

		pCaLppvOrderList->bOrderCount = pstSecInfo->bOrderCount;
		pCaLppvOrderList->bStatus= pstSecInfo->bReadOrderStatus;

		for (i = 0; i < pstSecInfo->bOrderCount; i++)
		{
			pCaLppvOrderList->pstOrders[i] = pstSecInfo->pstOrders[i];
		}
		if (pstSecInfo->pstOrders != 0)
		{
			OS_DRV_FreeMemory((void *)pstSecInfo->pstOrders);
			pstSecInfo->pstOrders = 0;
		}
		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}

CA_QUERY_STATUS_EN CA_LPPV_PlaceOrderQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	ia_byte bSector, bSectorNum;
	msg_lppv_place_order_query_st LppvPlaceOrder;
	ia_word32 wResourceId;
	CA_LPPV_PLACE_ORDER_T *pCaLppvPlaceOrder = (CA_LPPV_PLACE_ORDER_T *)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaLppvPlaceOrder== 0)
	{
		return CA_QUERY_FAIL;
	}
	wResourceId = pCaLppvPlaceOrder->wResourceId;
	bSector = pCaLppvPlaceOrder->bSector;
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

	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSector) != 1)
	{
		return CA_QUERY_FAIL;
	}

	/* send the place order message to scell */
	pvMsg = IA_OpenOutputMessage(sizeof(msg_lppv_place_order_query_st),
		sizeof(msg_lppv_place_order_query_st));

	LppvPlaceOrder.wResourceId = pCaLppvPlaceOrder->wResourceId;
	LppvPlaceOrder.bSector = pCaLppvPlaceOrder->bSector;
	LppvPlaceOrder.wTicketId = pCaLppvPlaceOrder->wTicketId;
	LppvPlaceOrder.fIgnoreBuyWindow = pCaLppvPlaceOrder->fIgnoreBuyWindow;
	LppvPlaceOrder.wMaxCost = pCaLppvPlaceOrder->wMaxCost;
	LppvPlaceOrder.wExpiryDataCode = pCaLppvPlaceOrder->wExpiryDataCode;
	ansi_memcpy((void*)LppvPlaceOrder.abPIN, (void*)pCaLppvPlaceOrder->abPIN,
		MAX_PINCODE_BYTE_LENGTH);

	IA_WriteToMessage(pvMsg, &LppvPlaceOrder, sizeof
		(msg_lppv_place_order_query_st));

	if (SCELL_Message(MSG_LPPV_PLACE_ORDER_QUERY, 0,
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
		CA_ERROR(("CA_LPPV_PalceOrderQueryControl: Cancel Order failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}

CA_QUERY_STATUS_EN CA_LPPV_CancelOrderQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	ia_byte bSector, bSectorNum;
	msg_lppv_cancel_order_query_st LppvCancelOrder;
	ia_word32 wResourceId;
	CA_LPPV_CANCEL_ORDER_T *pCaLppvCancelOrder = (CA_LPPV_CANCEL_ORDER_T *)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaLppvCancelOrder== 0)
	{
		return CA_QUERY_FAIL;
	}
	wResourceId = pCaLppvCancelOrder->wResourceId;
	bSector = pCaLppvCancelOrder->bSector;
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

	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSector) != 1)
	{
		return CA_QUERY_FAIL;
	}

	/* send the cancel order message to scell */
	pvMsg = IA_OpenOutputMessage(sizeof(msg_lppv_cancel_order_query_st),
		sizeof(msg_lppv_cancel_order_query_st));

	LppvCancelOrder.wResourceId = pCaLppvCancelOrder->wResourceId;
	LppvCancelOrder.bSector = pCaLppvCancelOrder->bSector;
	LppvCancelOrder.wTicketId = pCaLppvCancelOrder->wTicketId;
	ansi_memcpy((void*)LppvCancelOrder.abPIN, (void*)pCaLppvCancelOrder->abPIN,
		MAX_PINCODE_BYTE_LENGTH);

	IA_WriteToMessage(pvMsg, &LppvCancelOrder, sizeof
		(msg_lppv_cancel_order_query_st));

	if (SCELL_Message(MSG_LPPV_CANCEL_ORDER_QUERY, 0,
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
		CA_ERROR(("CA_LPPV_PalceOrderQueryControl: Cancel Order failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}


/********************* LPPV Notify functions *********************/
/********************* CA_LPPV_AwareNotify *********************/
/* Description: Notify message from softcell.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : pstSCellMsg	  - softcell message.
*
*
** Returns	: none
***************************************************************/
void CA_LPPV_AwareNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_lppv_aware_reply_st LppvAwareReply;
	msg_lppv_ppv_mode_st pstppvMode;
	ia_byte i, m, bSectorCount = 0;
	ia_word32 wResourceId = 0;

	ansi_memset((void *)&LppvAwareReply, 0, sizeof(msg_lppv_aware_reply_st));

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &LppvAwareReply, sizeof(LppvAwareReply)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wResourceId = LppvAwareReply.wResourceId;

	m = CA_LOCAL_Lppv_FindSmartCardLocation( wResourceId );
	if (m == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	bSectorCount = m_astLocalLppvSCardInfo[m].bSectorCount =
		LppvAwareReply.bSectorCount;
	m_astLocalLppvSCardInfo[m].bSCPPVMode = LppvAwareReply.bSCPPVMode;
	m_astLocalLppvSCardInfo[m].bSTBPPVMode = LppvAwareReply.bSTBPPVMode;

	if (bSectorCount > 0)
	{
		m_astLocalLppvSCardInfo[m].pstSectorInfo= (CA_LOCAL_LPPV_SECTOR_INFO_T *)
			OS_DRV_AllocateMemory((unsigned long)sizeof(CA_LOCAL_LPPV_SECTOR_INFO_T)*
			(unsigned long)bSectorCount);
		if (m_astLocalLppvSCardInfo[m].pstSectorInfo == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_LPPV_AwareNotify: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}

		for (i = 0; i < bSectorCount; i++)
		{
			ansi_memset((void *)&pstppvMode, 0, sizeof(msg_lppv_ppv_mode_st));
			ansi_memset((void*)&m_astLocalLppvSCardInfo[m].pstSectorInfo[i], 0x00,
					sizeof(CA_LOCAL_LPPV_SECTOR_INFO_T));
			IA_ReadFromMessage(pvMsg, &pstppvMode, sizeof(msg_lppv_ppv_mode_st));
			m_astLocalLppvSCardInfo[m].pstSectorInfo[i].bSectorID = pstppvMode.bSector;
			m_astLocalLppvSCardInfo[m].pstSectorInfo[i].bSCPPVMode = pstppvMode.bSCPPVMode;
		}
	}
	else
	{
		m_astLocalLppvSCardInfo[m].pstSectorInfo = 0;
	}


	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_LPPV_INFO) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( (ia_word32)wResourceId , CA_QC_LPPV_AWARE , (ia_word32)(LppvAwareReply.eResult));
	}

	IA_CloseMessage(pvMsg, IA_FALSE);

}
/********************* CA_LPPV_PreviewNotify *********************/
/* Description: Notify message from softcell.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : pstSCellMsg	  - softcell message.
*
*
** Returns	: none
***************************************************************/
void CA_LPPV_PreviewNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_lppv_event_notify_st LppvEventNotify;
	msg_lppv_event_ticket_st LppvEventTickets;
	ia_word32 i,m;
	ia_byte bSectorIndex, bSectorNum ,bTicketCount;
	ia_word32 wResourceId = INVALID_SC_SMARTCARDID;

	ansi_memset((void *)&LppvEventNotify, 0, sizeof(msg_lppv_event_notify_st));

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &LppvEventNotify, sizeof(LppvEventNotify)) ==
		0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	bSectorIndex = LppvEventNotify.bSector;
	wResourceId = LppvEventNotify.wResourceId;
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
	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSectorIndex) != 1)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	m = CA_LOCAL_Lppv_FindSmartCardLocation( wResourceId );
	if (m == SC_SLOT_INVALID)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	bTicketCount = LppvEventNotify.bTicketCount;

	m_astLocalLppvSCardInfo[m].bPreviewSectorId = LppvEventNotify.bSector;
	m_astLocalLppvSCardInfo[m].bPreviewWindow = LppvEventNotify.bPreviewWindow;
	m_astLocalLppvSCardInfo[m].bBuyWindow = LppvEventNotify.bBuyWindow;
	m_astLocalLppvSCardInfo[m].bEventTicketCount = LppvEventNotify.bTicketCount;
	if (m_astLocalLppvSCardInfo[m].pstEventTickets != 0)
	{
		OS_DRV_FreeMemory((void *)m_astLocalLppvSCardInfo[m].pstEventTickets);
		m_astLocalLppvSCardInfo[m].pstEventTickets = 0;
	}

	if (bTicketCount > 0)
	{
		m_astLocalLppvSCardInfo[m].pstEventTickets = (CA_LPPV_PPV_EVENT_TICKET_T*)
			OS_DRV_AllocateMemory((unsigned long)sizeof(CA_LPPV_PPV_EVENT_TICKET_T)
			*(unsigned long)bTicketCount);
		if (m_astLocalLppvSCardInfo[m].pstEventTickets == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_LPPV_PreviewNotify: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}

		for (i = 0; i < bTicketCount; i++)
		{
			ansi_memset((void*)&m_astLocalLppvSCardInfo[m].pstEventTickets[i], 0x00,
				sizeof(CA_LPPV_PPV_EVENT_TICKET_T));
			ansi_memset((void *)&LppvEventTickets, 0, sizeof(msg_lppv_event_ticket_st));
			IA_ReadFromMessage(pvMsg, &LppvEventTickets, sizeof(msg_lppv_event_ticket_st));
			m_astLocalLppvSCardInfo[m].pstEventTickets[i].wTicketId = LppvEventTickets.wTicketId;
			m_astLocalLppvSCardInfo[m].pstEventTickets[i].wCost = LppvEventTickets.wCost;
			m_astLocalLppvSCardInfo[m].pstEventTickets[i].wExpiryDataCode = LppvEventTickets.wExpiryDataCode;
			m_astLocalLppvSCardInfo[m].pstEventTickets[i].bPurchaseStatus = LppvEventTickets.bPurchaseStatus;
		}
	}
	else
	{
		m_astLocalLppvSCardInfo[m].pstEventTickets = 0;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_LPPV_INFO) == IA_TRUE)
	{/*the parameter of notify function*/
		CA_CLIENT_Notify(wResourceId, CA_QC_LPPV_PREVIEW, (ia_word32)(LppvEventNotify.bTicketCount));
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}
/********************* CA_LPPV_ReadTicketNotify *********************/
/* Description: Notify message from softcell.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : pstSCellMsg	  - softcell message.
*
*
** Returns	: none
***************************************************************/
void CA_LPPV_ReadTicketNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_lppv_read_ticket_reply_st LppvReadTicketReply;
	msg_lppv_read_ticket_reply_ticket_st LppvReadTicketReplyTicket;
	ia_byte bSectorIndex, bSectorNum;
	ia_byte bTickCount, i;
	ia_word32 wResourceId = INVALID_SC_SMARTCARDID;
	CA_LOCAL_LPPV_SECTOR_INFO_T *pstSecInfo;

	/* The event list hasn't been allocate */
	ansi_memset((void *)&LppvReadTicketReply, 0, sizeof(msg_lppv_read_ticket_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
	{
		return ;
	}
	if (IA_ReadFromMessage(pvMsg, &LppvReadTicketReply, sizeof(msg_lppv_read_ticket_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	wResourceId = LppvReadTicketReply.wResourceId;
	bSectorIndex = LppvReadTicketReply.bSector;

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
	/*CA_LOCAL_Lppv_GetSectorType(wResourceId, bSectorIndex) == PPT_SUPPORT ||*/
	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSectorIndex) != 1)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/*Copy the data to ECT , for the query reply of client*/
	pstSecInfo = CA_LOCAL_Lppv_FindSmartCardSector( wResourceId, bSectorIndex );
	if (pstSecInfo == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	bTickCount = LppvReadTicketReply.bTicketCount;
	pstSecInfo->bTicketCount = bTickCount;
	if (pstSecInfo->pstReadTickets != 0)
	{
		OS_DRV_FreeMemory((void *)pstSecInfo->pstReadTickets);
		pstSecInfo->pstReadTickets = 0;
	}

	if (bTickCount > 0)
	{
		pstSecInfo->pstReadTickets = (CA_LPPV_PPV_READ_TICKET_INFO_T*)
			OS_DRV_AllocateMemory((unsigned long)sizeof(CA_LPPV_PPV_READ_TICKET_INFO_T)
			*(unsigned long)bTickCount);
		if (pstSecInfo->pstReadTickets == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_LPPV_ReadTicketNotify: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}

		for (i = 0; i < bTickCount; i++)
		{
			ansi_memset((void *)&pstSecInfo->pstReadTickets[i], 0, sizeof(CA_LPPV_PPV_EVENT_TICKET_T));
			ansi_memset((void *)&LppvReadTicketReplyTicket, 0, sizeof(msg_lppv_read_ticket_reply_ticket_st));
			IA_ReadFromMessage(pvMsg, &LppvReadTicketReplyTicket, sizeof(msg_lppv_read_ticket_reply_ticket_st));
			pstSecInfo->pstReadTickets[i].wTicketId = LppvReadTicketReplyTicket.wTicketId;
			pstSecInfo->pstReadTickets[i].wCost = LppvReadTicketReplyTicket.wCost;
			pstSecInfo->pstReadTickets[i].wDateCode = LppvReadTicketReplyTicket.wDateCode;
			pstSecInfo->pstReadTickets[i].wExpiryDataCode = LppvReadTicketReplyTicket.wExpiryDataCode;
		}
	}
	else
	{
		pstSecInfo->pstReadTickets = 0;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_LPPV_INFO) == IA_TRUE)
	{
		CA_CLIENT_Notify(wResourceId, CA_QC_LPPV_READ_TICKET, (ia_word32)(LppvReadTicketReply.eResult));
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}
/********************* CA_LPPV_DeleteTicketNotify *********************/
/* Description: Notify message from softcell.
*   History : 29/03/2007
*   Author : Andrew Hui
* Parameters : pstSCellMsg	  - softcell message.
*
*
** Returns	: none
***************************************************************/
void CA_LPPV_DeleteTicketNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_lppv_delete_ticket_reply_st LppvDeleteTicketReply;
	ia_byte bSectorIndex, bSectorNum;
	ia_word32 wResourceId = INVALID_SC_SMARTCARDID;

	/* The event list hasn't been allocate */

	ansi_memset((void *)&LppvDeleteTicketReply, 0, sizeof(msg_lppv_delete_ticket_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
	{
		return ;
	}
	if (IA_ReadFromMessage(pvMsg, &LppvDeleteTicketReply, sizeof(msg_lppv_delete_ticket_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	if(LppvDeleteTicketReply.eResult != LPPV_QUERY_SUCCESS)
	{
		/* 2008-03-26 ayang:Fixed SCR#26902 */
		CA_CLIENT_Notify(wResourceId, CA_QC_LPPV_DELETE_TICKET, (ia_word32)(LppvDeleteTicketReply.eResult) );
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	wResourceId = LppvDeleteTicketReply.wResourceId;
	bSectorIndex = LppvDeleteTicketReply.bSector;
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
	/*CA_LOCAL_Lppv_GetSectorType(wResourceId, bSectorIndex) == PPT_SUPPORT ||*/
	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSectorIndex) != 1)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_LPPV_INFO) == IA_TRUE)
	{
		CA_CLIENT_Notify(wResourceId, CA_QC_LPPV_DELETE_TICKET, (ia_word32)(LppvDeleteTicketReply.eResult) );
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}

void CA_LPPV_ReadCreditNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_lppv_read_credit_reply_st LppvCreditReply;
	ia_byte bSectorIndex = 0;
	ia_word32 wResourceId = INVALID_SC_SMARTCARDID;
	CA_LOCAL_LPPV_SECTOR_INFO_T *pstSecInfo;

	ansi_memset((void *)&LppvCreditReply, 0, sizeof(msg_lppv_read_credit_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &LppvCreditReply, sizeof(LppvCreditReply)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	if (LppvCreditReply.eResult != LPPV_QUERY_SUCCESS)
	{
		/* 2008-03-26 ayang:Added to notify the status returned by softcell*/
		CA_CLIENT_Notify(wResourceId, CA_QC_LPPV_READ_CREDIT,
			((ia_word32) bSectorIndex << 8) | (ia_word32) LppvCreditReply.eResult);
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	wResourceId = LppvCreditReply.wResourceId;

	pstSecInfo = CA_LOCAL_Lppv_FindSmartCardSector( wResourceId, bSectorIndex );
	if (pstSecInfo == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	bSectorIndex = LppvCreditReply.bSector;

	if (CA_LOCAL_Lppv_GetSectorType(LppvCreditReply.wResourceId, bSectorIndex) == 1)
	{
		pstSecInfo->wCredit = LppvCreditReply.wCredit;
		pstSecInfo->wCurrentDC = LppvCreditReply.wCurrentDC;
		pstSecInfo->wExpiryDC = LppvCreditReply.wExpiryDC;
		pstSecInfo->fClearOnExpiry = LppvCreditReply.fClearOnExpiry;
		pstSecInfo->wLastIncreaseCredits = LppvCreditReply.wLastIncreaseCredits;
		pstSecInfo->wLastIncreaseDC = LppvCreditReply.wLastIncreaseDC;

		if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_LPPV_INFO) == IA_TRUE)
		{
			/* 2008-03-26 ayang:Added the status returned by softcell*/
#if 0
			CA_CLIENT_Notify(wResourceId, CA_QC_LPPV_READ_CREDIT, (ia_word32)
				bSectorIndex);
#else
			CA_CLIENT_Notify(wResourceId, CA_QC_LPPV_READ_CREDIT,
				((ia_word32) bSectorIndex << 8) | (ia_word32) LppvCreditReply.eResult);
#endif
		}
	}
	else
	{
		CA_ERROR(("CA_LPPV_SectorInfoNotify: The Sector is not LPPV support!\n"));

	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}

void CA_LPPV_ReadOrderNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_lppv_read_order_reply_st LppvOrderListReply;
	msg_lppv_read_order_reply_order_st LppvOrderListReplyOrder;
	ia_byte bSectorIndex, bSectorNum;
	ia_byte bOrderCount, i;
	ia_word32 wResourceId = INVALID_SC_SMARTCARDID;
	CA_LOCAL_LPPV_SECTOR_INFO_T *pstSecInfo;

	ansi_memset((void *)&LppvOrderListReply, 0, sizeof(msg_lppv_read_order_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
	{
		return ;
	}
	if (IA_ReadFromMessage(pvMsg, &LppvOrderListReply, sizeof(msg_lppv_read_order_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wResourceId = LppvOrderListReply.wResourceId;
	bSectorIndex = LppvOrderListReply.bSector;

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
	if (CA_LOCAL_Lppv_GetSectorType(wResourceId, bSectorIndex) != 1)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	pstSecInfo = CA_LOCAL_Lppv_FindSmartCardSector( wResourceId, bSectorIndex );
	if (pstSecInfo == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	bOrderCount = LppvOrderListReply.bOrderCount;

	/* save the smartcard LPPV order into ca task local database */
	pstSecInfo->bOrderCount = bOrderCount;
	if (pstSecInfo->pstOrders!= 0)
	{
		OS_DRV_FreeMemory((void *)pstSecInfo->pstOrders);
		pstSecInfo->pstOrders = 0;
	}

	if (bOrderCount > 0)
	{
		pstSecInfo->bReadOrderStatus = 0; /*0:OK indicate that the pstOrders contains data */
		pstSecInfo->pstOrders = (CA_LPPV_ORDER_INFO_T*)
			OS_DRV_AllocateMemory((unsigned long)sizeof(CA_LPPV_ORDER_INFO_T)
			*(unsigned long)bOrderCount);
		if (pstSecInfo->pstOrders == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_LPPV_OrderListNotify: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}
		for (i = 0; i < bOrderCount; i++)
		{
			ansi_memset((void *)&pstSecInfo->pstOrders[i], 0, sizeof(CA_LPPV_ORDER_INFO_T));
			ansi_memset((void *)&LppvOrderListReplyOrder, 0, sizeof(msg_lppv_read_order_reply_order_st));
			IA_ReadFromMessage(pvMsg, &LppvOrderListReplyOrder, sizeof(msg_lppv_read_order_reply_order_st));
			pstSecInfo->pstOrders[i].wTicketId = LppvOrderListReplyOrder.wTicketId;
			pstSecInfo->pstOrders[i].fIgnoreBuyWindow = LppvOrderListReplyOrder.fIgnoreBuyWindow;
			pstSecInfo->pstOrders[i].wMaxCost = LppvOrderListReplyOrder.wMaxCost;
			pstSecInfo->pstOrders[i].wExpiryDataCode = LppvOrderListReplyOrder.wExpiryDataCode;
		}
	}
	else
	{
		pstSecInfo->bReadOrderStatus = 1; /*1:ERROR indicate that the pstOrders is NULL */
		pstSecInfo->pstOrders = 0;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_LPPV_INFO) == IA_TRUE)
	{
		CA_CLIENT_Notify(wResourceId, CA_QC_LPPV_READ_ORDER, (ia_word32)((
			(ia_word32)bSectorIndex << 8) | (ia_word32)bOrderCount));
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}

void CA_LPPV_PlaceOrderNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_lppv_place_order_reply_st LppvPlaceOrderReply;

	ansi_memset((void *)&LppvPlaceOrderReply, 0, sizeof(msg_lppv_place_order_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &LppvPlaceOrderReply, sizeof(msg_lppv_place_order_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_LPPV_INFO) == IA_TRUE)
	{
		CA_CLIENT_Notify( LppvPlaceOrderReply.wResourceId , CA_QC_LPPV_PLACE_ORDER, (ia_word32)LppvPlaceOrderReply.eResult );
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}

void CA_LPPV_CancelOrderNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_lppv_cancel_order_reply_st LppvCancelOrderReply;

	ansi_memset((void *)&LppvCancelOrderReply, 0, sizeof(msg_lppv_cancel_order_reply_st));
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &LppvCancelOrderReply, sizeof(msg_lppv_cancel_order_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_LPPV_INFO) == IA_TRUE)
	{
		CA_CLIENT_Notify( LppvCancelOrderReply.wResourceId , CA_QC_LPPV_CANCEL_ORDER, (ia_word32)LppvCancelOrderReply.eResult );
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}

static CA_LOCAL_LPPV_SECTOR_INFO_T* CA_LOCAL_Lppv_FindSmartCardSector( ia_word32 wResourceId, ia_byte bSectorID )
{
	ia_byte i;

	 i = CA_LOCAL_Lppv_FindSmartCardLocation( wResourceId );
	if ( i == SC_SLOT_INVALID )
	{
		return 0;
	}

	 /* check whether there are sectors */
	if ( m_astLocalLppvSCardInfo[i].pstSectorInfo == 0 || m_astLocalLppvSCardInfo[i].bSectorCount <= bSectorID )
	{
		return 0;
	}

	return ( CA_LOCAL_LPPV_SECTOR_INFO_T *)(m_astLocalLppvSCardInfo[i].pstSectorInfo + bSectorID);
}

static ia_byte  CA_LOCAL_Lppv_FindSmartCardLocation(ia_word32 wResourceId)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalLppvSCardInfo[i].wResourceId == wResourceId)
		{
			return i;
		}
	}

	return SC_SLOT_INVALID;
}

static ia_byte CA_LOCAL_Lppv_AddSmartCardInfo(ia_word32 wResourceID)
{
	ia_byte i;

	if ( (i = CA_LOCAL_Lppv_FindSmartCardLocation(wResourceID) ) != SC_SLOT_INVALID )
	{
		return i;
	}

 	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalLppvSCardInfo[i].wResourceId == INVALID_SC_SMARTCARDID)
		{
			m_astLocalLppvSCardInfo[i].wResourceId = wResourceID;
			return i;
		}
	}
	return SC_SLOT_INVALID;
}

static ia_byte CA_LOCAL_Lppv_GetSectorType(ia_word32 wResourceId, ia_byte bIndex)
{
	ia_byte i;

	/* jyou: 3.4.2 */
	i = CA_LOCAL_Lppv_FindSmartCardLocation( wResourceId );
	if (i == SC_SLOT_INVALID )
		/* cann't find the resource id */
	{
		return IA_INVALID_PARAMETER;
	}
	if (bIndex > m_astLocalLppvSCardInfo[i].bSectorCount )
	{
		/* sector index out of the range */
		return IA_INVALID_PARAMETER;
	}
	if ( m_astLocalLppvSCardInfo[i].bSCPPVMode == 1 )
	{
		return m_astLocalLppvSCardInfo[i].pstSectorInfo[bIndex].bSCPPVMode;
	}
	else
	{
		return IA_INVALID_PARAMETER;
	}
}

