/**
 * $Id: ca_hgpc.c,v 1.6 2008/11/06 08:21:40 ayang Exp $
 *
 * Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
 *
 * The hgpc module of Embedded CA Task.
 **/

/******************************************************************************
* FILE		  : CA_hgpc.C												  	*
*																			*
* DESCRIPTION   :															*
*																			*
* NOTES		 :																*
*																			*
* STATUS/HISTORY:															*
* Author		  : Angela Yang											  	*
* Status		  : $Revision: 1.6 $									*
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
#include <irca/pubh/s3_hgpc_msg.h>
#include "ca_hgpc.h"
#include "ca_error.h"


/******************************* Local Variables ******************************/
static CA_HGPC_LOCAL_T m_astLocalHgpcInfo[SC_SLOT_MAX];

/******************************* Local Functions ******************************/
static ia_byte  CA_LOCAL_HGPC_FindSmartCardLocation(ia_word32 wResourceId);
static ia_byte CA_LOCAL_HGPC_AddSmartCardInfo(ia_word32 wResourceID);

static void ResetHgpcSmartCardData( CA_HGPC_LOCAL_T *pstInfo )
{
	if ( pstInfo->stPrimarySCData.bSecCardCount > 0 && pstInfo->stPrimarySCData.stSecCard != 0)
	{
		OS_DRV_FreeMemory( (void *)pstInfo->stPrimarySCData.stSecCard );
	}
}

static void CA_HGPC_Reset(ia_byte bIndex, ia_word32 wResourceId)
{
	CA_HGPC_LOCAL_T *pstTemp = 0;

	CA_PARAM_NOT_USED(astrSCellMsgDisplay); /* to supress warning */

	if ( bIndex > SC_SLOT_MAX-1)
	{
		CA_ERROR(("CA_HGPC_Reset bIndex= %d is too big!\r\n", bIndex));
		return;
	}
	pstTemp = &m_astLocalHgpcInfo[bIndex];

	pstTemp->wResourceId = wResourceId;
	ResetHgpcSmartCardData( pstTemp );
}

/*
* Description: Initializes the hgpc local database.
*
* Parameters : None.
*
* Returns	: None.
*/
void CA_HGPC_Initialise(ia_word32 wResourceId)
{
	ia_byte i;

	if (wResourceId == INVALID_SMARTCARDID)
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			CA_HGPC_Reset(i, wResourceId);
		}
	}
	else
	{
		i = CA_LOCAL_HGPC_AddSmartCardInfo( wResourceId );
		if (i == SC_SLOT_INVALID)
		{
			/* cann't allocate the resource id */
			return;
		}
		CA_HGPC_Reset(i, wResourceId);
	}
}

/*
* Description: Free the hgpc allocated memory.
*
* Parameters : None.
*
* Returns	: None.
*/
void CA_HGPC_MemoryFree(void)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		CA_HGPC_Reset( i, INVALID_SC_SMARTCARDID ); /*clear all the memories*/
	}

	return;
}

/********************* HGPC QC functions *********************/
void CA_HGPC_SmartCardDataNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_hgpc_card_data_reply_st stHgpcCardData;
	msg_hgpc_card_data_primary_st stHgpcCardPrimary;
	msg_hgpc_smartcard_id_st stHgpcCardId;
	msg_hgpc_card_data_secondary_st stHgpcCardSecondary;
	ia_byte i, m, bSecCardCount = 0;
	ia_byte bSerialNoLength = 0;

	ia_word32 wResourceId = 0;

	ansi_memset((void *)&stHgpcCardData, 0, sizeof(msg_hgpc_card_data_reply_st));

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &stHgpcCardData, sizeof(stHgpcCardData)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wResourceId = stHgpcCardData.wResourceId;

	m = CA_LOCAL_HGPC_FindSmartCardLocation( wResourceId );
	if (m == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	m_astLocalHgpcInfo[m].eCardStatus = stHgpcCardData.eCardStatus;

	/* Fixed SCR#27711: HGPC smart card data can't be notified in DPC function */
	/* Delete judge for stHgpcCardData.eCardStatus != HGPC_CARD_ERROR */
	m_astLocalHgpcInfo[m].eCardRole= stHgpcCardData.eCardRole;

	if(stHgpcCardData.eCardRole == HGPC_ROLE_PRIMARY)
	{
		ansi_memset((void *)&stHgpcCardPrimary, 0, sizeof(msg_hgpc_card_data_primary_st));
		if (IA_ReadFromMessage(pvMsg, &stHgpcCardPrimary, sizeof(msg_hgpc_card_data_primary_st)) == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return ;
		}
		m_astLocalHgpcInfo[m].stPrimarySCData.eMethod = stHgpcCardPrimary.eMethod;
		m_astLocalHgpcInfo[m].stPrimarySCData.wHNARefreshTimeout = stHgpcCardPrimary.wHNARefreshTimeout;
		m_astLocalHgpcInfo[m].stPrimarySCData.wHNARepeatInterval = stHgpcCardPrimary.WHNARepeatInterval;
		bSecCardCount = m_astLocalHgpcInfo[m].stPrimarySCData.bSecCardCount = stHgpcCardPrimary.bSecCardCount;
		if (bSecCardCount != 0)
		{
			m_astLocalHgpcInfo[m].stPrimarySCData.stSecCard = (CA_HGPC_SEC_CARD_T *)
				OS_DRV_AllocateMemory((unsigned long)sizeof(CA_HGPC_SEC_CARD_T)*
				(unsigned long)bSecCardCount);
			if (m_astLocalHgpcInfo[m].stPrimarySCData.stSecCard == 0)
			{
				IA_CloseMessage(pvMsg, IA_FALSE);
				CA_ERROR(("CA_HGPC_SmartCardDataNotify: Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return ;
			}
			for (i = 0;i<bSecCardCount; i++)
			{
				ansi_memset((void *)&stHgpcCardId, 0, sizeof(msg_hgpc_smartcard_id_st));
				ansi_memset((void*)&m_astLocalHgpcInfo[m].stPrimarySCData.stSecCard[i], 0x00,
					sizeof(CA_HGPC_SEC_CARD_T));
				if (IA_ReadFromMessage(pvMsg, &stHgpcCardId, sizeof(msg_hgpc_smartcard_id_st)) == 0)
				{
					IA_CloseMessage(pvMsg, IA_FALSE);
					return ;
				}
				m_astLocalHgpcInfo[m].stPrimarySCData.stSecCard[i].stSecCardId = stHgpcCardId;
				bSerialNoLength = stHgpcCardId.bSerialNoLength;
				/* Fixed SCR#27478:The null pointer in mih_hgpc_get_scell_data causes system crash */
				if (bSerialNoLength != 0)
				{
					if (IA_ReadFromMessage(pvMsg, &m_astLocalHgpcInfo[m].stPrimarySCData.stSecCard[i].abSecCardSerialNo[0], bSerialNoLength) == 0)
					{
						IA_CloseMessage(pvMsg, IA_FALSE);
						return ;

					}
				}
			}
		}
		else
		{
			m_astLocalHgpcInfo[m].stPrimarySCData.stSecCard = 0;
		}
	}

	if(stHgpcCardData.eCardRole == HGPC_ROLE_SECONDARY)
	{
		ansi_memset((void *)&stHgpcCardSecondary, 0, sizeof(msg_hgpc_card_data_secondary_st));
		if (IA_ReadFromMessage(pvMsg, &stHgpcCardSecondary, sizeof(msg_hgpc_card_data_secondary_st)) == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return ;
		}
		m_astLocalHgpcInfo[m].stSecondarySCData.eMethod = stHgpcCardSecondary.eMethod;
		m_astLocalHgpcInfo[m].stSecondarySCData.wHNRTimeLeft = stHgpcCardSecondary.wHNRTimeLeft;
		m_astLocalHgpcInfo[m].stSecondarySCData.wActivationCode = stHgpcCardSecondary.wActivationCode;
		m_astLocalHgpcInfo[m].stSecondarySCData.stPrimCardId = stHgpcCardSecondary.stPrimCardId;

		bSerialNoLength = stHgpcCardSecondary.stPrimCardId.bSerialNoLength;
		if (bSerialNoLength != 0)
		{
			/* Fixed SCR#27478:The null pointer in mih_hgpc_get_scell_data causes system crash */
			if (IA_ReadFromMessage(pvMsg, &m_astLocalHgpcInfo[m].stSecondarySCData.abPrimCardSerialNo[0], bSerialNoLength) == 0)
			{
				IA_CloseMessage(pvMsg, IA_FALSE);
				return ;
			}
		}
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_HGPC_SMARTCARD_DATA) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( (ia_word32)wResourceId , CA_QC_HGPC_SMARTCARD_DATA , (ia_word32)(stHgpcCardData.eCardStatus));
	}

	IA_CloseMessage(pvMsg, IA_FALSE);

}

void CA_HGPC_SoftCellDataNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_hgpc_scell_data_reply_st stHgpcScellData;
	msg_hgpc_scell_data_st stScellData;
	ia_byte m = 0;
	ia_byte bSerialNoLength = 0;

	ansi_memset((void *)&stHgpcScellData, 0, sizeof(msg_hgpc_scell_data_reply_st));

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &stHgpcScellData, sizeof(msg_hgpc_scell_data_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	m_astLocalHgpcInfo[m].eScellRole = stHgpcScellData.eScellRole;

	if(stHgpcScellData.eScellRole == HGPC_ROLE_PRIMARY
		|| stHgpcScellData.eScellRole == HGPC_ROLE_SECONDARY)
	{
		ansi_memset((void *)&stScellData, 0, sizeof(msg_hgpc_scell_data_st));
		if (IA_ReadFromMessage(pvMsg, &stScellData, sizeof(msg_hgpc_scell_data_st)) == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return ;
		}
		m_astLocalHgpcInfo[m].stScellData = stScellData;
		bSerialNoLength = stScellData.stPrimCardId.bSerialNoLength;

		if (bSerialNoLength != 0)
		{
			/* Fixed SCR#27478:The null pointer in mih_hgpc_get_scell_data causes system crash */
			if (IA_ReadFromMessage(pvMsg, &m_astLocalHgpcInfo[m].abPrimCardSerialNo[0], bSerialNoLength) == 0)
			{
				IA_CloseMessage(pvMsg, IA_FALSE);
				return ;
			}
		}
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_HGPC_SMARTCARD_DATA) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( 0 , CA_QC_HGPC_SOFTCELL_DATA , 0);
	}

	IA_CloseMessage(pvMsg, IA_FALSE);

}

void CA_HGPC_MsgStatusNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_hgpc_message_status_st stHgpcMsgStatus;
	ia_word32 wResourceId = 0;
	ia_byte m = 0;

	ansi_memset((void *)&stHgpcMsgStatus, 0, sizeof(msg_hgpc_message_status_st));

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &stHgpcMsgStatus, sizeof(msg_hgpc_message_status_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wResourceId = stHgpcMsgStatus.wResourceId;

	m = CA_LOCAL_HGPC_FindSmartCardLocation( wResourceId );
	if (m == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	m_astLocalHgpcInfo[m].eType = stHgpcMsgStatus.eType;
	m_astLocalHgpcInfo[m].eAction = stHgpcMsgStatus.eAction;
	m_astLocalHgpcInfo[m].eStatus = stHgpcMsgStatus.eStatus;

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_HGPC_MSG_STATUS) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( (ia_word32)wResourceId , CA_QC_HGPC_MSG_STATUS, 0);
	}

	IA_CloseMessage(pvMsg, IA_FALSE);

}

void CA_HGPC_EventNotify(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_hgpc_event_notify_st stHgpcEvent;

	ansi_memset((void *)&stHgpcEvent, 0, sizeof(msg_hgpc_event_notify_st));

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &stHgpcEvent, sizeof(msg_hgpc_event_notify_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_HGPC_EVENT) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( (ia_word32)stHgpcEvent.wResourceId , CA_QC_HGPC_EVENT, stHgpcEvent.eEventType);
	}

	IA_CloseMessage(pvMsg, IA_FALSE);
}

CA_QUERY_STATUS_EN CA_HGPC_SmartCardDataQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	ia_byte i, j, bSectorNum;
	CA_HGPC_SMARTCARD_DATA_T *pCaHgpcSCDataInfo = (CA_HGPC_SMARTCARD_DATA_T *)pCAClientMsg;
	msg_hgpc_card_data_query_st stQuery;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaHgpcSCDataInfo == 0)
	{
		return CA_QUERY_FAIL;
	}

	/* check the input parameter smart card id and bsector */
	bSectorNum = CA_SCARD_GetSectorNumber(pCaHgpcSCDataInfo->wResourceId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(pCaHgpcSCDataInfo->wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (pCaHgpcSCDataInfo->bRequestType == REQUEST_TYPE_QUERY)
	{
		/* send the aware info query message to scell */
		pvMsg = IA_OpenOutputMessage(sizeof(msg_hgpc_card_data_query_st), sizeof
			(msg_hgpc_card_data_query_st));

		ansi_memset( &stQuery, 0, sizeof(msg_hgpc_card_data_query_st));
		stQuery.wResourceId = pCaHgpcSCDataInfo->wResourceId;

		IA_WriteToMessage(pvMsg, &stQuery, sizeof(msg_hgpc_card_data_query_st));
		if (SCELL_Message(MSG_HGPC_SMARTCARD_DATA_QUERY, 0,
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
			CA_ERROR(("CA_HGPC_SmartCardData: query failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	if (pCaHgpcSCDataInfo->bRequestType == REQUEST_TYPE_REPLY)
	{
		i = CA_LOCAL_HGPC_FindSmartCardLocation( pCaHgpcSCDataInfo->wResourceId );
		if (i == SC_SLOT_INVALID)
		{
			/* cann't find the resource id */
			return CA_QUERY_FAIL;
		}

		pCaHgpcSCDataInfo->eCardStatus = m_astLocalHgpcInfo[i].eCardStatus;
		pCaHgpcSCDataInfo->eCardRole = m_astLocalHgpcInfo[i].eCardRole;
		if (pCaHgpcSCDataInfo->eCardRole == HGPC_ROLE_PRIMARY)
		{
			pCaHgpcSCDataInfo->stPrimaryData.eMethod = m_astLocalHgpcInfo[i].stPrimarySCData.eMethod;
			pCaHgpcSCDataInfo->stPrimaryData.wHNARefreshTimeout = m_astLocalHgpcInfo[i].stPrimarySCData.wHNARefreshTimeout;
			pCaHgpcSCDataInfo->stPrimaryData.wHNARepeatInterval = m_astLocalHgpcInfo[i].stPrimarySCData.wHNARepeatInterval;
			pCaHgpcSCDataInfo->stPrimaryData.bSecCardCount = m_astLocalHgpcInfo[i].stPrimarySCData.bSecCardCount;
			for (j = 0; j < pCaHgpcSCDataInfo->stPrimaryData.bSecCardCount; j++)
			{
				ansi_memcpy((void *)&pCaHgpcSCDataInfo->stPrimaryData.stSecCard[j].stSecCardId,
					(void *)&m_astLocalHgpcInfo[i].stPrimarySCData.stSecCard[j].stSecCardId,
					sizeof(msg_hgpc_smartcard_id_st));
				ansi_memcpy((void *)&pCaHgpcSCDataInfo->stPrimaryData.stSecCard[j].abSecCardSerialNo[0],
					(void *)&m_astLocalHgpcInfo[i].stPrimarySCData.stSecCard[j].abSecCardSerialNo[0],
					sizeof(ia_byte) * pCaHgpcSCDataInfo->stPrimaryData.stSecCard[j].stSecCardId.bSerialNoLength);
			}
			/*after send the data to client , delete it from ECT */
			if (m_astLocalHgpcInfo[i].stPrimarySCData.stSecCard != 0)
			{
				OS_DRV_FreeMemory((void *)m_astLocalHgpcInfo[i].stPrimarySCData.stSecCard);
				m_astLocalHgpcInfo[i].stPrimarySCData.bSecCardCount = 0;
			}
			return CA_QUERY_SUCCESS;

		}
		if (pCaHgpcSCDataInfo->eCardRole == HGPC_ROLE_SECONDARY)
		{
			pCaHgpcSCDataInfo->stSecondaryData.eMethod = m_astLocalHgpcInfo[i].stSecondarySCData.eMethod;
			pCaHgpcSCDataInfo->stSecondaryData.wHNRTimeLeft = m_astLocalHgpcInfo[i].stSecondarySCData.wHNRTimeLeft;
			pCaHgpcSCDataInfo->stSecondaryData.wActivationCode = m_astLocalHgpcInfo[i].stSecondarySCData.wActivationCode;
			ansi_memcpy((void *)&pCaHgpcSCDataInfo->stSecondaryData.stPrimCardId,
				(void *)&m_astLocalHgpcInfo[i].stSecondarySCData.stPrimCardId,
				sizeof(msg_hgpc_smartcard_id_st));
			ansi_memcpy((void *)&pCaHgpcSCDataInfo->stSecondaryData.abPrimCardSerialNo[0],
				(void *)&m_astLocalHgpcInfo[i].stSecondarySCData.abPrimCardSerialNo[0],
				sizeof(ia_byte) * pCaHgpcSCDataInfo->stSecondaryData.stPrimCardId.bSerialNoLength);
		}
		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}

CA_QUERY_STATUS_EN CA_HGPC_ScellDataQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte i = 0;
	CA_HGPC_SCELL_DATA_T *pCaHgpcSCellDataInfo = (CA_HGPC_SCELL_DATA_T *)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaHgpcSCellDataInfo == 0)
	{
		return CA_QUERY_FAIL;
	}

	if (pCaHgpcSCellDataInfo->bRequestType == REQUEST_TYPE_QUERY)
	{
		if (SCELL_Message(MSG_HGPC_SCELL_DATA_QUERY, 0,	0, 0))
		{
			return CA_QUERY_SUCCESS;
		}
		else
		{
			CA_ERROR(("CA_HGPC_SoftCellData: query failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	if (pCaHgpcSCellDataInfo->bRequestType == REQUEST_TYPE_REPLY)
	{
		pCaHgpcSCellDataInfo->eScellRole = m_astLocalHgpcInfo[i].eScellRole;
		if (pCaHgpcSCellDataInfo->eScellRole == HGPC_ROLE_PRIMARY
			|| pCaHgpcSCellDataInfo->eScellRole == HGPC_ROLE_SECONDARY)
		{
			ansi_memcpy((void *)&pCaHgpcSCellDataInfo->stScellData,
				(void *)&m_astLocalHgpcInfo[i].stScellData,
				sizeof(msg_hgpc_scell_data_st));

			ansi_memcpy((void *)&pCaHgpcSCellDataInfo->abPrimCardSerialNo[0],
				(void *)&m_astLocalHgpcInfo[i].abPrimCardSerialNo[0],
				sizeof(ia_byte) * pCaHgpcSCellDataInfo->stScellData.stPrimCardId.bSerialNoLength);

			return CA_QUERY_SUCCESS;
		}
		return CA_QUERY_SUCCESS;
	}
	return CA_QUERY_FAIL;
}

CA_QUERY_STATUS_EN CA_HGPC_MsgStatusQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte i, bSectorNum;
	CA_HGPC_MSG_STATUS_T *pCaHgpcMsgStatusInfo = (CA_HGPC_MSG_STATUS_T *)pCAClientMsg;

	CA_PARAM_NOT_USED( wHandle );
	if (pCaHgpcMsgStatusInfo == 0)
	{
		return CA_QUERY_FAIL;
	}

	/* check the input parameter smart card id and bsector */
	bSectorNum = CA_SCARD_GetSectorNumber(pCaHgpcMsgStatusInfo->wResourceId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}
	if (CA_SCARD_GetGlobalSmartCardStatus(pCaHgpcMsgStatusInfo->wResourceId) != STATUS_CARD_IN)
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	i = CA_LOCAL_HGPC_FindSmartCardLocation( pCaHgpcMsgStatusInfo->wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return CA_QUERY_FAIL;
	}

	pCaHgpcMsgStatusInfo->eType = m_astLocalHgpcInfo[i].eType;
	pCaHgpcMsgStatusInfo->eAction = m_astLocalHgpcInfo[i].eAction;
	pCaHgpcMsgStatusInfo->eStatus = m_astLocalHgpcInfo[i].eStatus;

	return CA_QUERY_SUCCESS;
}

/*********** Local Find SmartCard *****************/
static ia_byte  CA_LOCAL_HGPC_FindSmartCardLocation(ia_word32 wResourceId)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalHgpcInfo[i].wResourceId == wResourceId)
		{
			return i;
		}
	}

	return SC_SLOT_INVALID;
}

static ia_byte CA_LOCAL_HGPC_AddSmartCardInfo(ia_word32 wResourceID)
{
	ia_byte i;

	if ( (i = CA_LOCAL_HGPC_FindSmartCardLocation(wResourceID) ) != SC_SLOT_INVALID )
	{
		return i;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalHgpcInfo[i].wResourceId == INVALID_SC_SMARTCARDID)
		{
			m_astLocalHgpcInfo[i].wResourceId = wResourceID;
			return i;
		}
	}
	return SC_SLOT_INVALID;
}

