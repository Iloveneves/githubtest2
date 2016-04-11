/**
* $Id: ca_scard.c,v 1.39 2008/05/23 08:46:35 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* The module handles smartcard related queries and operations.
**/

/******************************** Header Files ********************************/
/* #include <string.h> */
#include "ca_scard.h"
#include <irca/pubh/s3_ansi_functions.h>
#include "ca_svcinfo.h"
#include "ca_ppt.h"
#include "ca_ippv.h"
#include "ca_lppv.h"
#include "ca_product.h"
#include "ca_hgpc.h"
#include "ca_error.h"
#include <irca/pubh/s3_osdrv.h>
#include <irca/pubh/s3_cadrv.h>
#include <irca/pubh/s3_message_helper.h>
#include <irca/pubh/s3_smartcard.h>
#include <irca/pubh/s3_maturity_rating.h>

/******************************* Local Variables ******************************/
/* jyou: 3.4.2 replaced with a central repository m_astLocalSCardInfo for ease of maintainance */

/* jyou: codes quoted in _AUSTAR_ is supposed to be put in Clue not ECT */
/* Will remove once the fixes in Glue will be done */
#ifdef _AUSTAR_
extern int SynScMessage;
extern int SynSendMessage;
#endif

/************************* Local Function Declarations ************************/

/*
* jyou: ECT 3.4-2 Multiple Smart Cards Local Storage Revision
*/
/* Smart Card Root Information */
static void ResetSmartCardInfo( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardAccess( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardHashID( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardSurfLock( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardSectors( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardUserInfo( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardRemoteChannel( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardUniqueAddress( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardVendorIDs( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );
static void ResetSmartCardMaturityRating( CA_LOCAL_SMARTCARD_INFO_T *pstInfo );

/* Smart Card Sector Data */
static void ResetSmartCardSectorRegion( CA_LOCAL_SECTOR_INFO_T *pstInfo );
static void ResetSmartCardSectorPPTRecords( CA_LOCAL_SECTOR_INFO_T *pstInfo );
static void ResetSmartCardSectorIPPVEvents( CA_LOCAL_SECTOR_INFO_T *pstInfo );
static void ResetSmartCardSectorProducts( CA_LOCAL_SECTOR_INFO_T *pstInfo );
static void ResetSmartCardSectorAddress(  CA_LOCAL_SECTOR_INFO_T *pstInfo );
static void ResetSmartCardSectorDateCode(  CA_LOCAL_SECTOR_INFO_T *pstInfo );
static void ResetSmartCardIPPVCallbackRequest( CA_LOCAL_SECTOR_INFO_T *pstInfo );
static void ResetSmartCardSectorOperatorID( CA_LOCAL_SECTOR_INFO_T *pstInfo );

/* local storage of smartcard information */
static CA_LOCAL_SMARTCARD_INFO_T m_astLocalSCardInfo[SC_SLOT_MAX];

/************************ Function Implementations ****************************/

CA_LOCAL_SMARTCARD_INFO_T* CA_LOCAL_GetLocalSCardInfo( void )
{
	return m_astLocalSCardInfo;
}

void CA_LOCAL_SetSurflockStatus( ia_byte wIndex, ia_bool fStatus )
{
	if ( wIndex >= SC_SLOT_MAX )
	{
		CA_ERROR( ("[ca_scard:SetSurflockStatus] Index out of range %d.\n", wIndex) );
	}
	else
	{
		CA_DBG_V( ("[ca_scard:SetSurflockStatus] Surflocked status changed %d.\n", fStatus) );
		m_astLocalSCardInfo[ wIndex ].fSurfLocked = fStatus;
	}
	return;
}

/*
*     ROUTINE : CA_SCARD_SetSmartCardId()
* DESCRIPTION : Add a new smart card resource ID to the local storage.
*       INPUT : ia_word32 wResourceId - The smart card resource ID to save.
*      OUTPUT : None
*     RETURNS : IA_SUCCESS	- The resource ID has been added or already exist.
*				IA_FAIL		- There is no free slots available.
* SIDE EFFECT : A unused index in m_astLocalSCardInfo will be occupied if the wResourceId was
*				in m_astLocalSCardInfo before.
*   CALLED BY :
*      Author : Jane You
* Last Changed: August 10, 2006
*/
ia_result CA_SCARD_SetSmartCardId(ia_word32 wResourceId)
{
	ia_byte i;

	i = CA_LOCAL_AddSmartCardInfo( wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* can't add any more */
		return IA_FAIL;
	}
	return IA_SUCCESS;
}

/*
*     ROUTINE : CA_SCARD_GetSmartCardId()
* DESCRIPTION : Return a smart card's resource ID.
*       INPUT : ia_byte bIndex - The corresponding index in m_astLocalSCardInfo of a smart card.
*      OUTPUT : None
*     RETURNS : The resource ID of the smart card.
*				INVALID_SMARTCARDID invalid input.
* SIDE EFFECT :
*   CALLED BY : UpdateDefaultResourcesToDescrambleService
*      Author : Jane You
* Last Changed: August 10, 2006
*/
ia_word32 CA_SCARD_GetSmartCardId(ia_byte bIndex)
{
	if ( bIndex > SC_SLOT_MAX-1 )
	{
		CA_ERROR(("CA_SCARD_ResetSmartCardInfo bIndex is too big!\r\n"));
		return INVALID_SMARTCARDID;
	}
	return m_astLocalSCardInfo[bIndex].wResourceID;
}

/*
*     ROUTINE : CA_SCARD_ResetSmartCardId()
* DESCRIPTION : Reassign the given local storage for a new smart card.
*				Free allocated memories for the old one.
*       INPUT : ia_byte bIndex - The corresponding index in m_astLocalSCardInfo.
*				ia_word32 wSmartcardId - The new smart card resource ID to be set.
*      OUTPUT : None
*     RETURNS :
* SIDE EFFECT : m_astLocalSCardInfo[bIndex] will be reset, all allocated memory will be freed
*   CALLED BY :
*      Author : Jane You
* Last Changed: August 10, 2006
*/
static void CA_SCARD_ResetSmartCardInfo(ia_byte bIndex, ia_word32 wSamrtcardId)
{
	CA_LOCAL_SMARTCARD_INFO_T *pstTemp;

	if ( bIndex > SC_SLOT_MAX-1)
	{
		CA_ERROR(("CA_SCARD_ResetSmartCardInfo bIndex= %d is too big!\r\n", bIndex));
		return;
	}

	pstTemp = &m_astLocalSCardInfo[bIndex];

	/* Smart Card Status Information */
	pstTemp->wResourceID = wSamrtcardId;
	pstTemp->wSmartCardGlobalStatus = CA_SCARD_GetGlobalSmartCardStatus( wSamrtcardId );
	pstTemp->stSmartCardStatus = CA_SCARD_GetGlobalSmartCardStatusStructure( wSamrtcardId );
	/* Smart Card Info except status */
	ResetSmartCardInfo( pstTemp );
	/* Homing Channel Information */
	ResetSmartCardAccess( pstTemp );
	/* Remote Channel Information */
	ResetSmartCardRemoteChannel( pstTemp );
	/* Sector Information */
	ResetSmartCardSectors( pstTemp );
	/* Hash ID */
	ResetSmartCardHashID( pstTemp );
	/* User Data */
	ResetSmartCardUserInfo( pstTemp);
	/* Surflocking */
	ResetSmartCardSurfLock( pstTemp );
	/* Unique Address */
	ResetSmartCardUniqueAddress( pstTemp );
	/* Vendor IDs */
	ResetSmartCardVendorIDs( pstTemp );
	/* Maturity Rating */
	ResetSmartCardMaturityRating( pstTemp );
}

/*
*     ROUTINE : CA_SCARD_Initialise()
* DESCRIPTION : Initialize the smart card local storage when CA Task first start up,
*				or whenenver smart card status changes.
*       INPUT : ia_word32 wResourceId - The new smart card resource ID.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : if ( wResourceId == INVALID_SMARTCARDID )
*					the entire m_astLocalSCardInfo will be reset, and all allocated memory freed.
*				else
*					m_astLocalSCardInfo[bIndex] will be reset, all allocated memory will be freed,
*					where m_astLocalSCardInfo[bIndex].wResourceId == wResourceId
*   CALLED BY : InitialiseAllModules, ParseSmartcardStatusMsg
*      Author : Jane You
* Last Changed: August 31, 2006
*/
void CA_SCARD_Initialise(ia_word32 wResourceId)
{
	ia_byte i;

#ifdef _AUSTAR_
	SynScMessage = SynScMessage	+ 1;
#endif

	if (wResourceId == INVALID_SMARTCARDID)
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			CA_SCARD_ResetSmartCardInfo(i, wResourceId);
		}
	}
	else
	{
		i = CA_LOCAL_AddSmartCardInfo( wResourceId );
		if (i == SC_SLOT_INVALID)
		{
			/* cann't allocate the resource id */
			return;
		}
		CA_SCARD_ResetSmartCardInfo(i, wResourceId);
	}
	return;
}

/*
*     ROUTINE : CA_SCARD_CodeChangeQueryControl()
* DESCRIPTION : CA_QC_PIN_CODE_CHANGE control handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
*				CA_QUERY_NOT_AVAILABLE - Unable to run this query.
* SIDE EFFECT : Send MSG_PIN_CODE_QUERY to SoftCell with ePinAction==IA_PIN_CHANGE
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: August 10, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_CodeChangeQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_pin_code_query_st ScardPinCodeQuery;
	CA_PIN_CODE_ACCESS_T *pstPinInfo;

	CA_PARAM_NOT_USED( wHandle );
	pstPinInfo = (CA_PIN_CODE_ACCESS_T*)pCAClientMsg;
	if (!pstPinInfo)
	{
		return CA_QUERY_FAIL;
	}

	if (CA_SCARD_GetGlobalSmartCardStatus(pstPinInfo->wResourceId) != STATUS_CARD_IN)
		/* This message is only valid if the status for that smartcard is
		SMARTCARD:D00 (Card-In). */
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	/* send the pin code change query message to scell */
	pvMsg = IA_OpenOutputMessage(sizeof(msg_pin_code_query_st), sizeof(msg_pin_code_query_st));

	ScardPinCodeQuery.wResourceId = pstPinInfo->wResourceId;
	ScardPinCodeQuery.ePinAction = IA_PIN_CHANGE;
	ScardPinCodeQuery.ePinType = (ia_pin_type_e)pstPinInfo->bCodeIndex;
	ansi_memcpy((void*)ScardPinCodeQuery.abPinCode, (void*)pstPinInfo->abOldPin,
		MAX_PINCODE_BYTE_LENGTH);
	ansi_memcpy((void*)ScardPinCodeQuery.abNewPinCode, (void*)pstPinInfo->abNewPin,
		MAX_PINCODE_BYTE_LENGTH);

	IA_WriteToMessage(pvMsg, &ScardPinCodeQuery, sizeof(ScardPinCodeQuery));

	if (SCELL_Message(MSG_PIN_CODE_QUERY, 0, IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage
		(pvMsg)) == IA_SUCCESS)
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return CA_QUERY_SUCCESS;
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage(pvMsg, IA_TRUE);
		CA_ERROR(("CA_IPPV_EventsListQueryControl: query events list failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return CA_QUERY_FAIL;
	}
}

/*
*     ROUTINE : CA_SCARD_CodeCheckQueryControl()
* DESCRIPTION : CA_QC_PIN_CODE_CHECK control handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
*				CA_QUERY_NOT_AVAILABLE - Unable to run this query.
* SIDE EFFECT : Send MSG_PIN_CODE_QUERY to SoftCell with ePinAction==IA_PIN_CHECK
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: August 10, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_CodeCheckQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_pin_code_query_st ScardPinCodeQuery;
	ia_word32 wSCardId;
	CA_PIN_CODE_ACCESS_T *pstPinInfo;

	CA_PARAM_NOT_USED( wHandle );
	pstPinInfo = (CA_PIN_CODE_ACCESS_T*)pCAClientMsg;
	if (!pstPinInfo)
	{
		return CA_QUERY_FAIL;
	}
	wSCardId = pstPinInfo->wResourceId;

	if (CA_SCARD_GetGlobalSmartCardStatus(wSCardId) != STATUS_CARD_IN)
		/* This message is only valid if the status for that smartcard is
		SMARTCARD:D00 (Card-In). */
	{
		return CA_QUERY_NOT_AVAILABLE;
	}
	/* send the pin code change query message to scell */
	pvMsg = IA_OpenOutputMessage(sizeof(msg_pin_code_query_st), sizeof(msg_pin_code_query_st));

	ScardPinCodeQuery.wResourceId = pstPinInfo->wResourceId;
	ScardPinCodeQuery.ePinAction = IA_PIN_CHECK;
	ScardPinCodeQuery.ePinType = (ia_pin_type_e)pstPinInfo->bCodeIndex;
	ansi_memcpy((void*)ScardPinCodeQuery.abPinCode, (void*)pstPinInfo->abOldPin,
		MAX_PINCODE_BYTE_LENGTH);

	IA_WriteToMessage(pvMsg, &ScardPinCodeQuery, sizeof(ScardPinCodeQuery));

	if (SCELL_Message(MSG_PIN_CODE_QUERY, 0, IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage
		(pvMsg)) == IA_SUCCESS)
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return CA_QUERY_SUCCESS;
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage(pvMsg, IA_TRUE);
		CA_ERROR(("CA_IPPV_EventsListQueryControl: query events list failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}

}

/*
*     ROUTINE : CA_SCARD_RemoteChannelQueryControl()
* DESCRIPTION : CA_QC_REMOTE_CHANNEL control handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
*				CA_QUERY_NOT_AVAILABLE - Unable to run this query.
* SIDE EFFECT : if the request type is REQUEST_TYPE_QUERY
*					Send MSG_REMOTE_MESSAGE_QUERY to SoftCell.
*				if the request type is REQUEST_TYPE_REPLY
*					Return the remote channel data from m_astLocalSCardInfo and reset them.
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: August 16, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_RemoteChannelQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_remote_channel_message_st ScardRemoteChannelMessage;
	ia_byte bRemoteMessageMaxLen = 0, i, len;
	ia_word32 wSCardId = 0;
	CA_REMOTE_CHANNEL_T *pRemoteChannel;

	CA_PARAM_NOT_USED( wHandle );
	pRemoteChannel = (CA_REMOTE_CHANNEL_T*)pCAClientMsg;
	if (!pRemoteChannel)
	{
		return CA_QUERY_FAIL;
	}
	if (!pRemoteChannel->pbPayload)
	{
		return CA_QUERY_FAIL;
	}
	wSCardId = pRemoteChannel->wResourceId;

	if (CA_SCARD_GetGlobalSmartCardStatus(wSCardId) != STATUS_CARD_IN)
		/* This message is only valid if the status for that smartcard is
		SMARTCARD:D00 (Card-In). */
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	if (pRemoteChannel->bRequestType == REQUEST_TYPE_QUERY)
	{
		bRemoteMessageMaxLen = pRemoteChannel->bPayloadLengthMax;
		/* send the sector info query message to scell */
		pvMsg = IA_OpenOutputMessage((ia_word16)(sizeof(msg_remote_channel_message_st) + sizeof
			(ia_byte)*bRemoteMessageMaxLen), (ia_word16)sizeof(ia_byte));
		ScardRemoteChannelMessage.bLength = bRemoteMessageMaxLen;
		ScardRemoteChannelMessage.wResourceId = pRemoteChannel->wResourceId;
		IA_WriteToMessage(pvMsg, &ScardRemoteChannelMessage, sizeof(ScardRemoteChannelMessage));

		if (bRemoteMessageMaxLen > 0)
		{
			for (i = 0; i < bRemoteMessageMaxLen; i++)
			{
				IA_WriteToMessage(pvMsg, &pRemoteChannel->pbPayload[i], sizeof(ia_byte));
			}
		}

		if (SCELL_Message(MSG_REMOTE_MESSAGE_QUERY, 0, IA_GetOutputMessageLength(pvMsg),
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
			CA_ERROR(("CA_IPPV_GetPursesQueryControl: query purses failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else if (pRemoteChannel->bRequestType == REQUEST_TYPE_REPLY)
	{
		i = CA_LOCAL_FindSmartCardLocation( pRemoteChannel->wResourceId );
		if (i == SC_SLOT_INVALID)
		{
			return CA_QUERY_FAIL;
		}

		/* get the minimum lenght that available */
		len = (ia_byte)((pRemoteChannel->bPayloadLengthMax < m_astLocalSCardInfo[i].bRemoteChannelLength) ?
			pRemoteChannel->bPayloadLengthMax : m_astLocalSCardInfo[i].bRemoteChannelLength);
		/* copy the payload */
		ansi_memcpy((void*)pRemoteChannel->pbPayload, (void*)m_astLocalSCardInfo[i].pbRemoteChannelPayload,
				sizeof(ia_byte) * len);
		pRemoteChannel->bPayloadLengthMax = m_astLocalSCardInfo[i].bRemoteChannelLength;

		return CA_QUERY_SUCCESS;
	}

	return CA_QUERY_FAIL;
}

/*
*     ROUTINE : CA_SCARD_ParsePinCodeMsg()
* DESCRIPTION : Parses received MSG_PIN_CODE_REPLY message from SoftCell and
*				update the local data storage.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : 1. Send CA_QC_PIN_CODE_CHANGE notify to CA Client,
*					when ScardPinCodeReply.ePinAction == IA_PIN_CHANGE
*				2. Send CA_QC_PIN_CODE_CHECK notify to CA Client,
*					when ScardPinCodeReply.ePinAction == IA_PIN_CHECK
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: August 17, 2006
*/
void CA_SCARD_ParsePinCodeMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_pin_code_reply_st ScardPinCodeReply;
	ia_word16 PinCodeParam = 0;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &ScardPinCodeReply, sizeof(ScardPinCodeReply)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_SMARTCARD_INFO) == IA_TRUE)
	{
		PinCodeParam = (ia_word16)(((ScardPinCodeReply.ePinType &0xff) << 8) + (ScardPinCodeReply.eResult &0xff));
		if (ScardPinCodeReply.ePinAction == IA_PIN_CHANGE)
		{

			CA_CLIENT_Notify(ScardPinCodeReply.wResourceId, CA_QC_PIN_CODE_CHANGE, (ia_word32)
				PinCodeParam);
		}
		if (ScardPinCodeReply.ePinAction == IA_PIN_CHECK)
		{
			CA_CLIENT_Notify(ScardPinCodeReply.wResourceId, CA_QC_PIN_CODE_CHECK, (ia_word32)
				PinCodeParam);
		}
	}
	IA_CloseMessage(pvMsg, IA_FALSE);

}

/*
*     ROUTINE : CA_SCARD_ParseRemoteChannelReplyMsg()
* DESCRIPTION : Parses received MSG_REMOTE_MESSAGE_REPLY message from SoftCell and
*				update the local data storage.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : 1. Fields bRemoteChannelLength and pbRemoteChannelPayload of m_astLocalSCardInfo[i]
*					be updated,
*					where m_astLocalSCardInfo[i].wResourceId==ScardRemoteChannelMessage.wResourceId
*				2. Send out a CA_QC_REMOTE_CHANNEL notification.
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: August 17, 2006
*/
void CA_SCARD_ParseRemoteChannelReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_remote_channel_message_st ScardRemoteChannelMessage;
	ia_byte bTmpLen = 0, i, j;
	ia_byte bPayLoad;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &ScardRemoteChannelMessage, sizeof(ScardRemoteChannelMessage)) ==
		0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	i = CA_LOCAL_FindSmartCardLocation( ScardRemoteChannelMessage.wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	m_astLocalSCardInfo[i].bRemoteChannelLength = bTmpLen = ScardRemoteChannelMessage.bLength;
	if (bTmpLen > 0)
	{
		if (m_astLocalSCardInfo[i].pbRemoteChannelPayload != 0)
		{
			FREE_MEMORY(m_astLocalSCardInfo[i].pbRemoteChannelPayload);
			m_astLocalSCardInfo[i].pbRemoteChannelPayload = 0;
		}
		m_astLocalSCardInfo[i].pbRemoteChannelPayload = ALLOCATE_MEMORY(ia_byte, bTmpLen);
		if (m_astLocalSCardInfo[i].pbRemoteChannelPayload == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_SCARD_ParseRemoteChannelReplyMsg: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}
		for (j = 0; j < bTmpLen; j++)
		{
			IA_ReadFromMessage(pvMsg, &bPayLoad, sizeof(bPayLoad));
			m_astLocalSCardInfo[i].pbRemoteChannelPayload[j] = bPayLoad;
		}
	}

	CA_CLIENT_Notify(ScardRemoteChannelMessage.wResourceId, CA_QC_REMOTE_CHANNEL,
					(ia_word32)	bTmpLen);
	IA_CloseMessage(pvMsg, IA_FALSE);
	return ;
}

/*
void CA_SCARD_ParseIppvPinCheckMsg( const CA_SCELL_MSG *pstSCellMsg )
{
ia_msg_handle pvMsg;

pvMsg = IA_OpenInputMessage( pstSCellMsg->pvMessage, pstSCellMsg->wLength );
IA_CloseMessage(pvMsg, IA_FALSE);

}
*/

/*
*     ROUTINE : CA_SCARD_ScAccessQueryControl()
* DESCRIPTION : CA_QC_SMARTCARD_ACCESS query handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
*				CA_QUERY_NOT_AVAILABLE - Unable to run this query.
* SIDE EFFECT : Return the remote channel data from m_astLocalSCardInfo and reset them.
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: August 17, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_ScAccessQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte bLen = 0, i;
	CA_SC_ACCESS_T *pstScData;

	CA_PARAM_NOT_USED( wHandle );
	pstScData = (CA_SC_ACCESS_T*)pCAClientMsg;
	/* Check that memory is valid, if not return failure */
	if (CA_MEM_Protect((void*)pstScData->pbData, (ia_uint32)pstScData->bLength) != IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_NOTIFY_MEMORY_ERROR, (ia_uint32)CA_QC_SMARTCARD_ACCESS);
		return CA_QUERY_FAIL;
	}

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( pstScData->wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return CA_QUERY_FAIL;
	}

	if (pstScData->bAccessOp == CA_SC_ACCESS_READ)
	{
		if (m_astLocalSCardInfo[i].bHomingChannelLength == 0)
		{
			CA_DBG(("The user data is not available now.\n"));
			return CA_QUERY_NOT_AVAILABLE;
		}

		if (pstScData->bLength < m_astLocalSCardInfo[i].bHomingChannelLength)
		{
			CA_ERROR(("Your buffer size is too small. (%d BYTES AT LEAST)\n", HOMING_CHANNEL_LENGTH));
			bLen = pstScData->bLength;
		}
		else
		{
			bLen = m_astLocalSCardInfo[i].bHomingChannelLength;
		}
		pstScData->bLength = m_astLocalSCardInfo[i].bHomingChannelLength; /* return the actual size. */
		ansi_memcpy(pstScData->pbData, m_astLocalSCardInfo[i].pbHomingChannelData, bLen);
		return CA_QUERY_SUCCESS;
	}

	return CA_QUERY_FAIL;
}

/*
*     ROUTINE : CA_SCARD_SendSmartCardHomingChQueryMsg()
* DESCRIPTION : Sends a MSG_SMARTCARD_HOMING_CH_QUERY message to SoftCell.
*       INPUT : wResourceId - The smart card resource ID to which this query addresses.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : None
*   CALLED BY : ParseSmartcardStatusMsg, when the status of the given smart card is STATUS_CARD_IN
*				IRD_MSG_Parser, when bDestID==DEC_CONTROLLER && bMsgType==READ_SC_USER_DATA
*      Author : Jane You
* Last Changed: August 17, 2006
*/
void CA_SCARD_SendSmartCardHomingChQueryMsg(ia_word32 wResourceId)
{
	msg_smartcard_query_data_st* pstMsg;

	CA_DBG_V(("<= MSG_SMARTCARD_HOMING_CH_QUERY\n"));

	pstMsg = (msg_smartcard_query_data_st*)ALLOCATE_MEMORY(msg_smartcard_query_data_st, 1);

	if (pstMsg != 0)
	{
		ansi_memset((void *)pstMsg, 0, sizeof(msg_smartcard_query_data_st));
		pstMsg->wResourceId = wResourceId;
		if (SCELL_Message(MSG_SMARTCARD_HOMING_CH_QUERY, 0, sizeof(msg_smartcard_query_data_st), pstMsg) != IA_SUCCESS)
		{
			if (pstMsg != 0)
			{
				FREE_MEMORY(pstMsg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}
}

/*
*     ROUTINE : CA_SCARD_ParseSmartCardHomingChReplyMsg()
* DESCRIPTION : Parses received MSG_SMARTCARD_HOMING_CH_REPLY message from SoftCell and
*				update the local data storage.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : 1. Fields bHomingChannelLength and pbHomingChannelData of m_astLocalSCardInfo[i]
*					be updated,
*					where m_astLocalSCardInfo[i].wResourceId==stScHomingReply.wResourceId
*				2. Send out a CA_QC_SMARTCARD_ACCESS notification.
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: August 17, 2006
*/
void CA_SCARD_ParseSmartCardHomingChReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_smartcard_homing_reply_data_st stScHomingReply;
	ia_word16 i, j;
	ia_bool fChanged = IA_FALSE;

#ifdef _AUSTAR_
	SynScMessage = SynScMessage + 1;
#endif

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &stScHomingReply, sizeof(stScHomingReply)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	CA_DBG_V(("RscId = %d\n", stScHomingReply.wResourceId));


	i = CA_LOCAL_FindSmartCardLocation( stScHomingReply.wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	if (m_astLocalSCardInfo[i].pbHomingChannelData != 0)
	{
		/* Compare the received data with local record. Notify the
		ca client only when they are different.*/
		for (j = 0; j < HOMING_CHANNEL_LENGTH; j++)
		{
			if (m_astLocalSCardInfo[i].pbHomingChannelData[j] != stScHomingReply.abHomingChannel[j])
			{
				fChanged = IA_TRUE;
				break;
			}
		}
		if (fChanged == IA_FALSE)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return ;
		}
		FREE_MEMORY(m_astLocalSCardInfo[i].pbHomingChannelData);
	}

	m_astLocalSCardInfo[i].bHomingChannelLength = HOMING_CHANNEL_LENGTH;
	m_astLocalSCardInfo[i].pbHomingChannelData = (ia_byte*)ALLOCATE_MEMORY(ia_byte, HOMING_CHANNEL_LENGTH);
	if (m_astLocalSCardInfo[i].pbHomingChannelData != 0)
	{
		for (j = 0; j < HOMING_CHANNEL_LENGTH; j++)
		{
			m_astLocalSCardInfo[i].pbHomingChannelData[j] = stScHomingReply.abHomingChannel[j];
		}
	}
	else
	{
		CA_ERROR(("Failed to allocate memory.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

#ifdef _AUSTAR_
	if(SynSendMessage!=1)
	{
		CA_CLIENT_Notify(stScHomingReply.wResourceId, CA_QC_SMARTCARD_ACCESS, CA_SC_ACCESS_READ);
	}
#else
	/* ECT3.3 enhancement: streamline of wHandle */
	CA_CLIENT_Notify(stScHomingReply.wResourceId, CA_QC_SMARTCARD_ACCESS, CA_SC_ACCESS_READ);
#endif
	IA_CloseMessage(pvMsg, IA_FALSE);
}

/*
*     ROUTINE : CA_SCARD_SendNationalityQueryMsg()
* DESCRIPTION : Sends a MSG_NATIONALITY_QUERY message to SoftCell.
*       INPUT : wResourceId - The smart card resource ID to which this query addresses.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : None
*   CALLED BY : ParseSmartcardStatusMsg, when the status of the given smart card is STATUS_CARD_IN
*      Author : Jane You
* Last Changed: August 17, 2006
*/
void CA_SCARD_SendNationalityQueryMsg( ia_word32 wResourceId )
{
	msg_smartcard_query_data_st *pstMsg;

	CA_DBG_V(("<= MSG_NATIONALITY_QUERY\n"));

	pstMsg = (msg_smartcard_query_data_st*)ALLOCATE_MEMORY(msg_smartcard_query_data_st, 1);
	if (pstMsg != 0)
	{
		ansi_memset((void *)pstMsg, 0, sizeof(msg_smartcard_query_data_st));
		pstMsg->wResourceId = wResourceId;

		if (SCELL_Message(MSG_NATIONALITY_QUERY, 0, sizeof(msg_smartcard_query_data_st), pstMsg) !=
			IA_SUCCESS)
		{
			if (pstMsg != 0)
			{
				FREE_MEMORY(pstMsg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}
}


/*
*     ROUTINE : CA_SCARD_ParseNationalityMsg()
* DESCRIPTION : Parses received MSG_NATIONALITY message from SoftCell and
*				update the local data storage.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : IA_TRUE  - Nationality stored on the smartcard has changed.
*				IA_FALSE - Nationality not changed.
* SIDE EFFECT : if ( Nationality has Changed )
*					1. Local storage m_astLocalSCardInfo[i].acNationality be updated,
*						where m_astLocalSCardInfo[i].wResourceId==stNatData.wResourceId
*					2. Send out a CA_QC_SMARTCARD_INFO notification to CA Client with
*						ulParam set to CA_SMARTCARD_NATIONALITY_INFO. Section 5.5 of 731710.
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: August 17, 2006
*/
void CA_SCARD_ParseNationalityMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_nationality_data_st stNatData;
	ia_byte i;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return;
	if (IA_ReadFromMessage(pvMsg, &stNatData, sizeof(stNatData)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}

	/* jyou: 3.4.2 */
	/* The MSG_NATIONALITY message will only be sent if there is a valid IA smartcard
	is inserted, otherwise an error message (MSG_ERROR) will be sent with error code. */
	/* locate the corresponding smartcard */
	i = CA_LOCAL_FindSmartCardLocation( stNatData.wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}

	if ((m_astLocalSCardInfo[i].acNationality[0] != stNatData.acNationality[0]) ||
		(m_astLocalSCardInfo[i].acNationality[1] != stNatData.acNationality[1]) ||
		(m_astLocalSCardInfo[i].acNationality[2] != stNatData.acNationality[2]))
	{
		m_astLocalSCardInfo[i].acNationality[0] = stNatData.acNationality[0];
		m_astLocalSCardInfo[i].acNationality[1] = stNatData.acNationality[1];
		m_astLocalSCardInfo[i].acNationality[2] = stNatData.acNationality[2];

		CA_DBG_V(("SC Nationality = %02x %02x %02x\n", stNatData.acNationality[0],
			stNatData.acNationality[1], stNatData.acNationality[2]));

#ifdef _AUSTAR_
		SynScMessage = SynScMessage + 1;
		if(SynSendMessage!=1)
		{
			CA_SCARD_SmartCardInfoNotify(stNatData.wResourceId);
		}

		fChanged = IA_TRUE;
#else
		/* jyou: 3.4.2 : separated nationality update notification issue */
		CA_SCARD_SmartCardInfoNotify( stNatData.wResourceId, SMARTCARD_INFO_MASK_NATIONALITY );
#endif
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
	return;
}

/*
*     ROUTINE : CA_SCARD_SendSmartCardDataQueryMsg()
* DESCRIPTION : Sends a MSG_SMARTCARD_DATA_QUERY message to SoftCell.
*       INPUT : wResourceId - The smart card resource ID to which this query addresses.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : None
*   CALLED BY : CA_SCARD_SmartCardInfoQueryControl, when bRequestType==REQUEST_TYPE_QUERY
*				ParseSmartcardStatusMsg, when the status of the given smart card is STATUS_CARD_IN
*      Author : Jane You
* Last Changed: August 17, 2006
*/
void CA_SCARD_SendSmartCardDataQueryMsg(ia_word32 wResourceId)
{
	msg_smartcard_query_data_st *pstMsg;

	CA_DBG_V(("<= MSG_SMARTCARD_DATA_QUERY\n"));

	pstMsg = (msg_smartcard_query_data_st*)ALLOCATE_MEMORY(msg_smartcard_query_data_st, 1);
	if (pstMsg != 0)
	{

		ansi_memset((void *)pstMsg, 0, sizeof(msg_smartcard_query_data_st));
		pstMsg->wResourceId = wResourceId;

		if (SCELL_Message(MSG_SMARTCARD_DATA_QUERY, 0, sizeof(msg_smartcard_query_data_st), pstMsg)
			!= IA_SUCCESS)
		{
			if (pstMsg != 0)
			{
				FREE_MEMORY(pstMsg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

}


/*
*     ROUTINE : CA_SCARD_ParseSmartCardDataReplyMsg()
* DESCRIPTION : Parses received MSG_SMARTCARD_DATA_REPLY message and updates local
*               smartcard database.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : 1. An unoccupied slot in m_astLocalSCardInfo will be assigend if the
*					smart card resource ID is not previously recorded.
*				2. Fields wSmartCardGlobalStatus, stSmartCardStatus, wSmartCardType,
*					bMajorVersion, bMinorVersion, wPatchLevel, wOwnerID, bVariant,
*					bBuild, and acSerialNumber will be updated,
*					where m_astLocalSCardInfo[i].wResourceId==stNatData.wResourceId
*				3. Send out a CA_QC_SMARTCARD_INFO notification to CA Client with
*					ulParam set to SMARTCARD_INFO_MASK_GENERAL_DATA.
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: August 17, 2006
*/
void CA_SCARD_ParseSmartCardDataReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_smartcard_data_reply_data_st stScDataReply;
	ia_byte bLen = 0;
	ia_byte i = 0;
	ia_word32 wSCardId = 0;
	ia_status_st ScardStatus;

#ifdef _AUSTAR_
	SynScMessage = SynScMessage	+ 1;
#endif

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stScDataReply, sizeof(stScDataReply)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	CA_DBG_V(("SC version = %d.%d\n", stScDataReply.bMajorVersion, stScDataReply.bMinorVersion));

	/* jyou: 3.4.2 */
	i = CA_LOCAL_AddSmartCardInfo( stScDataReply.wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wSCardId = stScDataReply.wResourceId;
	m_astLocalSCardInfo[i].wSmartCardGlobalStatus = CA_SCARD_GetGlobalSmartCardStatus(wSCardId);

	if (IA_SUCCESS != CA_SCARD_GetSmartCardStatus(wSCardId, &ScardStatus))
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	m_astLocalSCardInfo[i].stSmartCardStatus = ScardStatus;
	m_astLocalSCardInfo[i].wSmartCardType = stScDataReply.wSmartcardType;
	m_astLocalSCardInfo[i].bMajorVersion = stScDataReply.bMajorVersion;
	m_astLocalSCardInfo[i].bMinorVersion = stScDataReply.bMinorVersion;
	m_astLocalSCardInfo[i].wPatchLevel = stScDataReply.wPatchLevel;
	m_astLocalSCardInfo[i].wOwnerID = stScDataReply.wOwnerId;
	m_astLocalSCardInfo[i].bVariant = stScDataReply.bVariant;
	m_astLocalSCardInfo[i].bBuild = stScDataReply.bBuild;

	bLen = stScDataReply.bSerialNoLength;
	if ((bLen + 1) <= MAX_SC_SER_NO_LEN)
	{
		IA_ReadFromMessage(pvMsg, m_astLocalSCardInfo[i].acSerialNumber, bLen);
		/* Contain the Check Digit. */
		m_astLocalSCardInfo[i].acSerialNumber[bLen - 1] = (ia_char)stScDataReply.bCheckDigit;
		m_astLocalSCardInfo[i].acSerialNumber[bLen] = '\0';
	}
	else
	{
		CA_ERROR(("The length of sc serial no %d out of range.\n", bLen + 1));

		IA_ReadFromMessage(pvMsg, m_astLocalSCardInfo[i].acSerialNumber, MAX_SC_SER_NO_LEN);
		m_astLocalSCardInfo[i].acSerialNumber[MAX_SC_SER_NO_LEN - 1] = '\0';
	}
#ifdef _AUSTAR_
	if(SynSendMessage!=1)
	{
		CA_SCARD_SmartCardInfoNotify(stScDataReply.wResourceId);
	}
#else
	CA_SCARD_SmartCardInfoNotify( stScDataReply.wResourceId, SMARTCARD_INFO_MASK_GENERAL_DATA );
#endif
	IA_CloseMessage(pvMsg, IA_FALSE);
	return;
}

/*
*     ROUTINE : CA_SCARD_SmartCardInfoQueryControl()
* DESCRIPTION : CA_QC_SMARTCARD_INFO query handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
*				CA_QUERY_NOT_AVAILABLE - Unable to run this query.
* SIDE EFFECT : Return the general smart card data from m_astLocalSCardInfo.
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: August 17, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_SmartCardInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte i;
	ia_word32 wResourceId = INVALID_SC_SMARTCARDID;
	CA_SMARTCARD_INFO_T *pstSCard;

	CA_PARAM_NOT_USED( wHandle );
	pstSCard = ( CA_SMARTCARD_INFO_T* )pCAClientMsg;
	if (pstSCard == 0)
		return CA_QUERY_FAIL;

	wResourceId = pstSCard->wResourceId;

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( pstSCard->wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return CA_QUERY_FAIL;
	}

	if (pstSCard->bRequestType == REQUEST_TYPE_QUERY)
	{
		CA_SCARD_SendSmartCardDataQueryMsg(wResourceId);
	}
	else
	{
		pstSCard->wResourceId = m_astLocalSCardInfo[i].wResourceID;
		pstSCard->wStatus = m_astLocalSCardInfo[i].wSmartCardGlobalStatus;
		pstSCard->stSmartCardStatus = m_astLocalSCardInfo[i].stSmartCardStatus;
		pstSCard->wType = m_astLocalSCardInfo[i].wSmartCardType;
		pstSCard->bMajorVersion = m_astLocalSCardInfo[i].bMajorVersion;
		pstSCard->bMinorVersion = m_astLocalSCardInfo[i].bMinorVersion;
		pstSCard->acNationality[0] = m_astLocalSCardInfo[i].acNationality[0];
		pstSCard->acNationality[1] = m_astLocalSCardInfo[i].acNationality[1];
		pstSCard->acNationality[2] = m_astLocalSCardInfo[i].acNationality[2];
		pstSCard->wPatchLevel = m_astLocalSCardInfo[i].wPatchLevel;
		pstSCard->wOwnerId = m_astLocalSCardInfo[i].wOwnerID;
		pstSCard->bVariant = m_astLocalSCardInfo[i].bVariant;
		pstSCard->bBuild = m_astLocalSCardInfo[i].bBuild;

		ansi_memcpy(pstSCard->acSerialNo, m_astLocalSCardInfo[i].acSerialNumber, MAX_SC_SER_NO_LEN);
	}

	return CA_QUERY_SUCCESS;
}

/*
*     ROUTINE : CA_SCARD_SmartCardInfoNotify()
* DESCRIPTION : Sends CA_QC_SMARTCARD_INFO notification to CA Client if the
*				NOTIFY_MASK_SMARTCARD_INFO is enabled.
*       INPUT : wResourceId - The resource ID of the smart card this notification addresses.
*				wType - The bit-mask of the changed data type. Section 5.5 of 731710.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : None
*   CALLED BY : CA_SCARD_ParseNationalityMsg, wType=SMARTCARD_INFO_MASK_NATIONALITY;
*				CA_SCARD_ParseSmartCardDataReplyMsg, wType=SMARTCARD_INFO_MASK_GENERAL_DATA;
*				CA_SCARD_SaveSmartCardResourceID, wType=SMARTCARD_INFO_MASK_RESOURCE_ID.
*      Author : Jane You (Latest update)
* Last Changed: August 17, 2006
*/
void CA_SCARD_SmartCardInfoNotify(ia_word32 wResourceId, ia_word16 wType)
{
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_SMARTCARD_INFO) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( wResourceId , CA_QC_SMARTCARD_INFO , wType );
	}
}

/*
*     ROUTINE : CA_SCARD_ScHashQueryControl()
* DESCRIPTION : CA_QC_HASH_ID query handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
*				CA_QUERY_NOT_AVAILABLE - Unable to run this query.
* SIDE EFFECT : Return the hashed smart card serial number from m_astLocalSCardInfo.
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: August 31, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_ScHashQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte bLen = 0, i;
	CA_HASH_ID_T *pstHashId;
	ia_byte		bHashLength = 0;

	CA_PARAM_NOT_USED( wHandle );
	pstHashId = ( CA_HASH_ID_T* ) pCAClientMsg;

	if (CA_MEM_Protect((void*)pstHashId->pbHashedNo, (ia_uint32)pstHashId->bHashedNoLen) != IA_TRUE)
	{
		CA_CLIENT_Notify(0, CA_QC_NOTIFY_MEMORY_ERROR, (ia_uint32)CA_QC_HASH_ID);
		return CA_QUERY_FAIL;
	}

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( pstHashId->wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return CA_QUERY_FAIL;
	}

	/* jyou: SCR#25318 */
	bHashLength = m_astLocalSCardInfo[i].bHashedNoLen;
	if (bHashLength != 0)
	{
		if (pstHashId->bHashedNoLen < bHashLength)
		{
			CA_ERROR(("Your buffer size is too small. (see also: MAX_SC_SER_NO_LEN)\n"));
			bLen = pstHashId->bHashedNoLen;
		}
		else
		{
			bLen = bHashLength;
		}
		pstHashId->bHashedNoLen = bHashLength;
		ansi_memcpy(pstHashId->pbHashedNo, m_astLocalSCardInfo[i].acHashNo, bLen);
		/* Once we have returned the hash, null it again */
		/* jyou: SCR#25318 */
		m_astLocalSCardInfo[i].bHashedNoLen = 0;
		return CA_QUERY_SUCCESS;
	}
	else
		return CA_QUERY_FAIL;
}


/*
*     ROUTINE : CA_SCARD_ParseSmartCardDataIdMsg()
* DESCRIPTION : Parses received MSG_SMARTCARD_ID message.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : 1. This message will be ignored if the matching wResourceId doesn't
*					exist in the local database.
*				2. Fields wClientHandle, bHashedNoLen, and acHashNo will be updated,
*					where m_astLocalSCardInfo[i].wResourceID==stScIDData.wResourceId
*				3. Send out a CA_QC_HASH_ID notification to CA Client.
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: August 31, 2006
*/
void CA_SCARD_ParseSmartCardIdMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_smartcard_id_data_st stScIdData;
	ia_byte bLen = 0, i;
	ia_word16 ScellHandle;
	ia_word32 wGroupHandle;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stScIdData, sizeof(stScIdData)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	CA_DBG_V(("Rsc Id = %d, Svc Handle = %d, Id Str Len = %d\n", stScIdData.wResourceId,
		stScIdData.wServiceHandle, stScIdData.bIdStringLength));

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( stScIdData.wResourceId );
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	ScellHandle = stScIdData.wServiceHandle;
	wGroupHandle = CA_SVCINFO_HandleSCell2CaClient(ScellHandle);

	/* The length abIdString array, including the terminating zero. */
	bLen = stScIdData.bIdStringLength;
	if (bLen > MAX_SC_SER_NO_LEN)
	{
		bLen = MAX_SC_SER_NO_LEN;
	}

	IA_ReadFromMessage(pvMsg, &m_astLocalSCardInfo[i].acHashNo[0], bLen);
	m_astLocalSCardInfo[i].acHashNo[MAX_SC_SER_NO_LEN - 1] = '\0';
	/* jyou: SCR#25318 */
	m_astLocalSCardInfo[i].bHashedNoLen =
		(ia_byte)(ansi_strlen((char*) &m_astLocalSCardInfo[i].acHashNo[0]) + 1); /* Including the
																				 terminating zero. */

	CA_CLIENT_Notify(stScIdData.wResourceId, CA_QC_HASH_ID, wGroupHandle);
	IA_CloseMessage(pvMsg, IA_FALSE);
}

/*
*     ROUTINE : CA_SCARD_ParseSectorsNumberMsg()
* DESCRIPTION : Parses received MSG_NUMBER_OF_SECTORS_NOTIFY message.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : 1. A new resource will be added into the local database if it was not previously
*					added.
*				2. Existing fields pstSectors, and pbSectorType will be cleared.
*					Fields bSectorCount, pstSectors, and pbSectorType will be updated,
*					where m_astLocalSCardInfo[i].wResourceID==stScSectorList.wResourceId
*				3. Calls CA_SCARD_RegionSendQuery for each sector.
*				4. Send out a CA_QC_SMARTCARD_NUMBER_OF_SECTORS notification to CA Client.
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: August 31, 2006
*/
void CA_SCARD_ParseSectorsNumberMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	int bSectorIndex;

	msg_number_of_sectors_reply_data_st stScSectorList;
	msg_number_of_sectors_sector_st stScSectorinfo;

	ia_byte i;

	/*allocate the memeory of message and get the sectors reply message*/
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stScSectorList, sizeof(msg_number_of_sectors_reply_data_st)) ==
		0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* jyou: 3.4.2 */
	i = CA_LOCAL_AddSmartCardInfo( stScSectorList.wResourceId );
	if (i == SC_SLOT_INVALID)
		/* cann't find the resource id */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* jyou: 3.4.2 */
	/* free previously allocated memory for sectors */
	ResetSmartCardSectors( &m_astLocalSCardInfo[i] );
	/*allocte sectors status data memeory*/
	if (stScSectorList.bSectors > 0)
	{
		/* jyou: 3.4.3.8 SCR#26064 2007-06-11 */
		m_astLocalSCardInfo[i].bSectorCount = stScSectorList.bSectors;
		/* allocate memory for sector count */
		m_astLocalSCardInfo[i].pbSectorType = (ia_byte*) ALLOCATE_MEMORY( ia_byte, stScSectorList.bSectors );
		if (m_astLocalSCardInfo[i].pbSectorType == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_SCARD_ParseSectorsNumberMsg: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}
		/* allocate memory for sectors */
		m_astLocalSCardInfo[i].pstSectors = (CA_LOCAL_SECTOR_INFO_T*)ALLOCATE_MEMORY(CA_LOCAL_SECTOR_INFO_T,
			stScSectorList.bSectors );
		if (m_astLocalSCardInfo[i].pstSectors == 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			CA_ERROR(("CA_SCARD_ParseSectorsNumberMsg: Memory allocate failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return ;
		}
		ansi_memset((void*)m_astLocalSCardInfo[i].pstSectors, 0x00, sizeof(CA_LOCAL_SECTOR_INFO_T)
			*m_astLocalSCardInfo[i].bSectorCount);
	}

	/* save sector information */
	for (bSectorIndex = 0; bSectorIndex < stScSectorList.bSectors; bSectorIndex++)
	{
		IA_ReadFromMessage(pvMsg, &stScSectorinfo, sizeof(msg_number_of_sectors_sector_st));

		m_astLocalSCardInfo[i].pstSectors[bSectorIndex].bSectorID = stScSectorinfo.bSector;
		m_astLocalSCardInfo[i].pstSectors[bSectorIndex].bActive = (ia_byte)stScSectorinfo.fActive;

		CA_SCARD_RegionSendQuery((ia_byte)bSectorIndex, (ia_word32)stScSectorList.wResourceId);
	}

	/* ECT3.3 enhancement: streamline of wHandle */
	CA_CLIENT_Notify( stScSectorList.wResourceId , CA_QC_SMARTCARD_NUMBER_OF_SECTORS ,
		(ia_uint32)m_astLocalSCardInfo[i].bSectorCount );

	IA_CloseMessage(pvMsg, IA_FALSE);
}

/*
*     ROUTINE : CA_SCARD_SectorListQueryControl()
* DESCRIPTION : CA_QC_SMARTCARD_NUMBER_OF_SECTORS query handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
*				CA_QUERY_NOT_AVAILABLE - Unable to run this query.
* SIDE EFFECT : Return the sector counts of smart card from m_astLocalSCardInfo.
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: August 31, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_SectorsListQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte i, j;
	CA_SC_SECTORLIST_T *pstSectorList;

	CA_PARAM_NOT_USED( wHandle );
	pstSectorList = (CA_SC_SECTORLIST_T*)pCAClientMsg;
	if (pstSectorList == 0)
	{
		return CA_QUERY_FAIL;
	}

	if (pstSectorList->pstSectors == 0)
	{
		return CA_QUERY_FAIL;
	}

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( pstSectorList->wResourceId );
	if (i == SC_SLOT_INVALID )
		/* cann't find the resource id */
	{
		return CA_QUERY_FAIL;
	}

	pstSectorList->bSectorCount = m_astLocalSCardInfo[i].bSectorCount;

	for (j = 0; j < m_astLocalSCardInfo[i].bSectorCount; j++)
	{
		pstSectorList->pstSectors[j].bSector = m_astLocalSCardInfo[i].pstSectors[j].bSectorID;
		pstSectorList->pstSectors[j].bActive = m_astLocalSCardInfo[i].pstSectors[j].bActive;
		pstSectorList->pstSectors[j].bReserved = 0xFF;
	}

	return CA_QUERY_SUCCESS;
}

/*
*     ROUTINE : CA_SCARD_GetSectorNumber()
* DESCRIPTION : Returns the sector count of the given smart card.
*       INPUT : ia_word32 wResourceId - Resource ID of the given smart card.
*      OUTPUT : None
*     RETURNS : 0--4					- the number of sectors in the smart card.
*				INVALID_SC_SECTOR		- the resource ID doesn't exist in local database.
* SIDE EFFECT : None.
*   CALLED BY : CA_IPPV_Reset, CA_IPPV_InitEventInfo, CA_IPPV_EventsListNotify,
*				CA_IPPV_SetThresholdQueryControl, CA_IPPV_GetIppvSectorInfoQueryControl
*				CA_IPPV_EventsListQueryControl, CA_IPPV_EventsCheckQueryControl
*				CA_IPPV_SendDebitQuery, CA_PPT_ResetPPTInfo, CA_PPT_InfoQueryControl
*				CA_PPT_INFOReplyMsg, CA_PRODUCT_ListSendQuery, CA_STOP_PRODUCT_ListSendQuery
*				CA_SCARD_ParseRegionReplyMsg, CA_SCARD_RegionQueryControl, CaProcessMsg
*				IRD_MSG_Parser
*      Author : Jane You
* Last Changed: August 31, 2006
*/
ia_byte CA_SCARD_GetSectorNumber(ia_word32 wResourceId)
{
	ia_byte i;

	i = CA_LOCAL_FindSmartCardLocation( wResourceId );
	if (i == SC_SLOT_INVALID )
		/* cann't find the resource id */
	{
		return INVALID_SC_SECTOR;
	} /* mfli */
	return m_astLocalSCardInfo[i].bSectorCount;
}


/*
*     ROUTINE : CA_SCARD_GetSectorID()
* DESCRIPTION : Returns the sector ID of the given smart card and index.
*       INPUT : ia_word32 wResourceId - Resource ID of the given smart card.
*				int		  sectorIndex - Index of the sector in the smart card.
*      OUTPUT : None
*     RETURNS : < INVALID_SC_SECTOR		- the ID of the given sector in the smart card.
*				INVALID_SC_SECTOR		- the resource ID doesn't exist in local database.
* SIDE EFFECT : None.
*   CALLED BY : CA_PRODUCT_ListSendQuery, CaProcessMsg, IRD_MSG_Parser
*      Author : Jane You
* Last Changed: August 31, 2006
*/
ia_byte CA_SCARD_GetSectorID(ia_word32 wResourceId, int sectorIndex)
{
	ia_byte i;

	i = CA_LOCAL_FindSmartCardLocation( wResourceId );
	if (i == SC_SLOT_INVALID )
		/* cann't find the resource id */
	{
		return INVALID_SC_SECTOR;
	}
	/* jyou: 3.4.3.6 SCR#26044 2007-06-01 */
	if ( sectorIndex < 0 || sectorIndex >= m_astLocalSCardInfo[i].bSectorCount )
	{
		/* sector index out of the range */
		return INVALID_SC_SECTOR;
	}
	CA_DBG(("[GetSectorID]: ResourceIndex=%d, SectorIndex=%d", i, sectorIndex));
	return m_astLocalSCardInfo[i].pstSectors[sectorIndex].bSectorID;
}


/*
*     ROUTINE : CA_SCARD_RegionSendQuery()
* DESCRIPTION : Sends MSG_REGION_QUERY message to SoftCell.
*       INPUT : ia_word32 wResourceId - Resource ID of the given smart card.
*				ia_byte	  bSecIndex - Index of the sector in the smart card.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
* SIDE EFFECT : None.
*   CALLED BY : CA_SCARD_ParseSectorsNumberMsg, CA_SCARD_RegionQueryControl, IRD_MSG_Parser
*      Author : Jane You
* Last Changed: August 31, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_RegionSendQuery(ia_byte bSecIndex, ia_word32 wResourceId)
{
	/*msg_smartcard_query_data_st *pstMsg;*/
	msg_region_query_st *pRegionMsg;

	CA_DBG_V(("<= MSG_REGION_QUERY\n"));

	pRegionMsg = (msg_region_query_st*)ALLOCATE_MEMORY(msg_region_query_st, 1);
	if (pRegionMsg != 0)
	{
		ansi_memset((void *)pRegionMsg, 0, sizeof(msg_region_query_st));
		pRegionMsg->wResourceId = wResourceId;
		pRegionMsg->bSector = bSecIndex;

		if (SCELL_Message(MSG_REGION_QUERY, 0, sizeof(msg_region_query_st), pRegionMsg) !=
			IA_SUCCESS)
		{
			if (pRegionMsg != 0)
			{
				FREE_MEMORY(pRegionMsg);
			}
			CA_ERROR(("Sending failed MSG_REGION_QUERY.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return CA_QUERY_FAIL;
	}

	return CA_QUERY_SUCCESS;
}

/*
*     ROUTINE : CA_SCARD_ParseRegionReplyMsg()
* DESCRIPTION : Parses received MSG_REGION_REPLY message.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : 1. A new resource-sector will be added into the local database if it was not previously
*					added.
*				2. Existing fields m_astLocalSCardInfo[i].pstSectors[bSector] will be updated,
*					where m_astLocalSCardInfo[i].wResourceID==stLocalRegion.wResourceId
*				3. Send out a CA_QC_REGION notification to CA Client.
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: September 1, 2006
*/
void CA_SCARD_ParseRegionReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	ia_byte bSectorIndex;
	msg_region_reply_st stReply;

	/*CA_REGION_T *pTemp = 0; jyou: 3.4.2*/
	ia_word32 wResourceId;
	/* jyou: 3.4.2 */
	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply, sizeof(msg_region_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wResourceId = stReply.wResourceId;
	bSectorIndex = stReply.bSector;

	/* validate the sector index */
	if ( bSectorIndex >= CA_SCARD_MAX_SECTOR_COUNT )
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* locate the corresponding smartcard */
	pstSector = CA_LOCAL_FindSmartCardSector( wResourceId, bSectorIndex );
	if ( pstSector == 0 )
		/* cann't find the resource id */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}
	pstSector->bSectorID = bSectorIndex;
	pstSector->fSectorValid = stReply.fSectorValid;

	if (stReply.fSectorValid == IA_TRUE)
	{
		pstSector->bRegion = stReply.bRegion;
		pstSector->bSubRegion = stReply.bSubRegion;
	}
	else
	{
		ResetSmartCardSectorRegion( pstSector );
	}

	CA_CLIENT_Notify(wResourceId, CA_QC_REGION, (ia_uint32)bSectorIndex);
	IA_CloseMessage(pvMsg, IA_FALSE);
	return;
}

/*
*     ROUTINE : CA_SCARD_RegionQueryControl()
* DESCRIPTION : CA_QC_REGION query handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
* SIDE EFFECT : if (bRequestType==REQUEST_TYPE_QUERY)
*					Send MSG_REGION_QUERY message to SoftCell;
*				else
*					Return the region info of a sector in a smart card from
*					local database m_astLocalSCardInfo to CA Client;
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: September 1, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_RegionQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte bSectorIndex = 0, bSectorNum;
	ia_word32 wResourceId;
	CA_LOCAL_SECTOR_INFO_T *pstSector = 0;
	CA_REGION_T *pcaRegion;

	CA_PARAM_NOT_USED( wHandle );
	pcaRegion = (CA_REGION_T*)pCAClientMsg;
	if (pcaRegion == 0)
	{
		return CA_QUERY_FAIL;
	}
	wResourceId = pcaRegion->wResourceId;
	if ( INVALID_SMARTCARDID == wResourceId )
	{
		return CA_QUERY_FAIL;
	}
	/* check the input parameter smart card id and bsector */
	bSectorNum = CA_SCARD_GetSectorNumber(wResourceId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}
	if ( pcaRegion->bSector > bSectorNum)
	{
		return CA_QUERY_FAIL;
	}
	else
	{
		bSectorIndex = pcaRegion->bSector;
	}

	if (pcaRegion->bRequestType == REQUEST_TYPE_QUERY)
	{
		if (CA_SCARD_GetGlobalSmartCardStatus(wResourceId) != STATUS_CARD_IN)
		{
			return CA_QUERY_FAIL;
		}

		return (CA_SCARD_RegionSendQuery(bSectorIndex, wResourceId));
	}
	else
	{
		/* jyou: 3.4.2 */
		/* locate the corresponding smartcard */
		pstSector = CA_LOCAL_FindSmartCardSector( wResourceId, bSectorIndex );
		if ( pstSector == 0 )
			/* cann't find the resource id */
		{
			return CA_QUERY_FAIL;
		}

		pcaRegion->bSectorStatus = (ia_byte)pstSector->fSectorValid;
		pcaRegion->bRegion = pstSector->bRegion;
		pcaRegion->bSubRegion = pstSector->bSubRegion;
	}

	return CA_QUERY_SUCCESS;
}

/*
*     ROUTINE : CA_SCARD_SendSmartCardUserDataQueryMsg()
* DESCRIPTION : Sends MSG_USER_DATA_QUERY message to SoftCell.
*       INPUT : ia_word32 wResourceId - Resource ID of the given smart card.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
* SIDE EFFECT : None.
*   CALLED BY : ParseSmartcardStatusMsg, IRD_MSG_Parser
*      Author : Jane You
* Last Changed: August 31, 2006
*/
void CA_SCARD_SendUserDataQueryMsg(ia_word32 wResourceId)
{
	msg_smartcard_query_data_st *pstMsg;

	CA_DBG_V(("<= MSG_SMARTCARD_USER_DATA_QUERY\n"));

	pstMsg = (msg_smartcard_query_data_st*)ALLOCATE_MEMORY(msg_smartcard_query_data_st, 1);
	if (pstMsg != 0)
	{
		ansi_memset((void *)pstMsg, 0, sizeof(msg_smartcard_query_data_st));
		pstMsg->wResourceId = wResourceId;

		if (SCELL_Message(MSG_SMARTCARD_USER_DATA_QUERY, 0, sizeof(msg_smartcard_query_data_st),
			pstMsg) != IA_SUCCESS)
		{
			if (pstMsg != 0)
			{
				FREE_MEMORY(pstMsg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

}

/*
*     ROUTINE : CA_SCARD_ParseUserDataReplyMsg()
* DESCRIPTION : Parses received MSG_SMARTCARD_USER_DATA_REPLY message.
*       INPUT : pstSCellMsg - Pointer to the message buffer provided by SoftCell.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : 1. A new resource-sector will be added into the local database if it was not previously
*					added.
*				2. Existing fields m_astLocalSCardInfo[i].abUserData will be updated,
*					where m_astLocalSCardInfo[i].wResourceID==stScUserDataReply.wResourceId
*				3. Send out a CA_QC_SMARTCARD_USER_INFO notification to CA Client.
*   CALLED BY : CaProcessMsg, when pstCA_Msg->enMsgType==CADRV__SCELL_MESSAGE
*      Author : Jane You
* Last Changed: September 1, 2006
*/
void CA_SCARD_ParseUserDataReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_smartcard_user_data_reply_data_st stScUserDataReply;
	ia_byte bLen = USER_DATA_LENGTH, i;

#ifdef _AUSTAR_
	SynScMessage = SynScMessage	+ 1;
#endif

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &stScUserDataReply, sizeof(msg_smartcard_user_data_reply_data_st))
		== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( stScUserDataReply.wResourceId );
	/* jyou: 3.4.3.6 SCR#26045 2007-06-01 */
	if ( i == SC_SLOT_INVALID )
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	ansi_memcpy((void*)m_astLocalSCardInfo[i].abUserData, (void*)stScUserDataReply.abUserData,
		USER_DATA_LENGTH);

#ifdef _AUSTAR_
	if(SynSendMessage!=1)
	{
		CA_CLIENT_Notify( stScUserDataReply.wResourceId , CA_QC_SMARTCARD_USER_INFO , (ia_uint32)bLen);
	}
#else
	/* ECT3.3 enhancement: streamline of wHandle */
	CA_CLIENT_Notify( stScUserDataReply.wResourceId , CA_QC_SMARTCARD_USER_INFO , (ia_uint32)bLen);
#endif

	IA_CloseMessage(pvMsg, IA_FALSE);
}

/*
*     ROUTINE : CA_SCARD_UserDataQueryControl()
* DESCRIPTION : CA_QC_SMARTCARD_USER_INFO query handler.
*       INPUT : ia_word32 wHandle - Reserved for future use.
*				void	  pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*				CA_QUERY_FAIL          - Failed.
* SIDE EFFECT : if (bRequestType==REQUEST_TYPE_QUERY)
*					Send MSG_REGION_QUERY message to SoftCell;
*				else
*					Return the region info of a sector in a smart card from
*					local database m_astLocalSCardInfo to CA Client;
*   CALLED BY : CA_TASK_QueryControl
*      Author : Jane You
* Last Changed: September 1, 2006
*/
CA_QUERY_STATUS_EN CA_SCARD_UserDataQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte i;
	CA_SMARTCARD_USER_DATA_T *pUserData;

	CA_PARAM_NOT_USED( wHandle );
	pUserData = ( CA_SMARTCARD_USER_DATA_T* ) pCAClientMsg;

	if (pUserData == 0)
	{
		return CA_QUERY_FAIL;
	}

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( pUserData->wResourceId );
	/* jyou: 3.4.3.6 SCR#26045 2007-06-01 */
	if ( i == SC_SLOT_INVALID )
	{
		return CA_QUERY_FAIL;
	}

	ansi_memcpy(pUserData->abUserData, m_astLocalSCardInfo[i].abUserData, USER_DATA_LENGTH);

	return CA_QUERY_SUCCESS;
}


/*for ECT+softcell termination*/
void CA_SCARD_FreeMemory(void)
{
	/* jyou: 3.4.2 */
	CA_LOCAL_ClearAllSmartCardLocation();
}

/*
*     ROUTINE : CA_SCARD_SetSectorType()
* DESCRIPTION : Sets the type of a given sector in a smart card.
*       INPUT : ia_word32 wResourceId - Resource ID of the given smart card.
*				ia_byte	  bIndex	  - Index of the sector in the smart card.
*				ia_byte	  bType		  - The type to be set.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : Field pbSectorType[bIndex] in m_astLocalSCardInfo[i] is set to bType,
*				where m_astLocalSCardInfo[i].wResourceID==wResourceId.
*   CALLED BY : CA_PPT_INFOReplyMsg
*      Author : Jane You
* Last Changed: August 31, 2006
*/
void CA_SCARD_SetSectorType(ia_word32 wResourceId, ia_byte bIndex, ia_byte bType)
{
	ia_byte i;

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( wResourceId );
	if (i == SC_SLOT_INVALID )
		/* cann't find the resource id */
	{
		return;
	}
	if ( bIndex > m_astLocalSCardInfo[i].bSectorCount )
	{
		/* sector index out of the range */
		return;
	}
	if (m_astLocalSCardInfo[i].pbSectorType != 0)
	{
		m_astLocalSCardInfo[i].pbSectorType[bIndex] = bType;
	}

	return ;
}

/*
*     ROUTINE : CA_SCARD_GetSectorType()
* DESCRIPTION : Sets the type of a given sector in a smart card.
*       INPUT : ia_word32 wResourceId - Resource ID of the given smart card.
*				ia_byte	  bIndex	  - Index of the sector in the smart card.
*      OUTPUT : None
*     RETURNS : ==IA_INVALID_PARAMETER - The specified sector cannot be found.
*				!=IA_INVALID_PARAMETER - The type of the given sector.
* SIDE EFFECT : None
*   CALLED BY : Many functions
*      Author : Jane You
* Last Changed: August 31, 2006
*/
ia_byte CA_SCARD_GetSectorType(ia_word32 wResourceId, ia_byte bIndex)
{
	ia_byte i;

	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( wResourceId );
	if (i == SC_SLOT_INVALID )
		/* cann't find the resource id */
	{
		return IA_INVALID_PARAMETER;
	}
	if (bIndex > m_astLocalSCardInfo[i].bSectorCount )
	{
		/* sector index out of the range */
		return IA_INVALID_PARAMETER;
	}
	if ( m_astLocalSCardInfo[i].pbSectorType != 0 )
	{
		return m_astLocalSCardInfo[i].pbSectorType[bIndex];
	}
	else
	{
		return IA_INVALID_PARAMETER;
	}
}

/*
*     ROUTINE : CA_SCARD_SetSmartCardStatus()
* DESCRIPTION : Save the newly received smart card status to the local storage.
*       INPUT : ia_word32		wResourceId - Resource ID of the given smart card.
*				ia_status_st*	pStatus - the pointer to the received status.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : m_astLocalSCardInfo[i].stSmartCardStats is set with the value in pStatus,
*				where m_astLocalSCardInfo[i].wResourceID == wResourceId
*   CALLED BY : ParseSmartcardStatusMsg
*      Author : Jane You
* Last Changed: September 7th, 2006
*/
void CA_SCARD_SetSmartCardStatus(ia_word32 wResourceId, ia_status_st *pStatus)
{
	ia_byte i;
	if (pStatus == 0)
		return ;
	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( wResourceId );
	if (i == SC_SLOT_INVALID )
		/* cann't find the resource id */
	{
		return;
	}
	ansi_memcpy((void*) &(m_astLocalSCardInfo[i].stSmartCardStatus),
		(void*)pStatus, sizeof(ia_status_st));
	m_astLocalSCardInfo[i].wSmartCardGlobalStatus = pStatus->wStatus;
	/* jyou: 3.3.10 Smart Card In-Out problem */
	CA_SCARD_SmartCardInfoNotify( wResourceId, SMARTCARD_INFO_MASK_STATUS );
	return ;
}

/*
*     ROUTINE : CA_SCARD_GetSmartCardStatus()
* DESCRIPTION : The base function to read out the saved status of a given smart card.
*				In case the card doesn't exist, then return the default value used when the
*				card reader is first initialized.
*       INPUT : ia_word32		wResourceId - Resource ID of the given smart card.
*				ia_status_st*	pScardStatus - the address of the return buffer
*      OUTPUT : pScardStatus - the returned status.
*     RETURNS : IA_FAIL		- Cannot find the smart card with wResourceId,
*								and pScardStatus is set to default value
*				IA_SUCCESS	- Successfully read the status, and pScardStatus is set
*								to the most recently saved status of the given smart card.
* SIDE EFFECT : None
*   CALLED BY : CA_SCARD_ParseSmartCardDataReplyMsg, CA_SCARD_GetGlobalSmartCardStatus,
*				CA_SCARD_GetGlobalSmartCardStatusStructure, ParseSmartcardStatusMsg
*      Author : Jane You
* Last Changed: September 7th, 2006
*/
ia_result CA_SCARD_GetSmartCardStatus(ia_word32 wResourceId, ia_status_st *pScardStatus)
{
	ia_byte i;
	/* jyou: 3.4.2 */
	i = CA_LOCAL_FindSmartCardLocation( wResourceId );
	if (i == SC_SLOT_INVALID )
		/* cann't find the resource id */
	{
		pScardStatus->eSource = SOURCE_UNKNOWN;
		pScardStatus->wStatus = STATUS_CARD_UNKNOWN /*jyou: 3.4.2 default status STATUS_CARD_OUT*/;
		pScardStatus->eSeverity = STATUS_INFO_DND;
		return IA_FAIL;
	}
	*pScardStatus = m_astLocalSCardInfo[i].stSmartCardStatus;
	ansi_memcpy((void*)pScardStatus, (void*) &(m_astLocalSCardInfo[i].stSmartCardStatus),
		sizeof(ia_status_st));
	return IA_SUCCESS;
}

/*
*     ROUTINE : CA_SCARD_GetGlobalSmartCardStatus()
* DESCRIPTION : Returns the stored latest status of a given smart card or CARD-OUT by default.
*       INPUT : ia_word32 wResourceId - Resource ID of the given smart card.
*					When equals INVALID_SMARTCARDID means request the default value.
*      OUTPUT : None
*     RETURNS : One of the following:
*					STATUS_CARD_IN,
*					STATUS_CARD_OUT,
*					STATUS_CARD_UNKNOWN,
*					STATUS_CARD_ERROR,
*					STATUS_CARD_VERIFYING,
* SIDE EFFECT : None
*   CALLED BY : SendIPPVPurchaseQueryMsg, CA_IPPV_SetThresholdQueryControl,
*				CA_IPPV_GetIppvSectorInfoQueryControl, CA_IPPV_EventsListQueryControl,
*				CA_IPPV_EventsCheckQueryControl, CA_IPPV_SendDebitQuery, CA_PPT_InfoQueryControl,
*				CA_EXTENDED_PRODUCT_ListQueryControl, CA_SCARD_ResetSmartCardInfo
*				CA_SCARD_CodeChangeQueryControl, CA_SCARD_CodeCheckQueryControl
*				CA_SCARD_RemoteChannelQueryControl, CA_SCARD_ParseSmartCardDataReplyMsg,
*				CA_SCARD_RegionQueryControl,
*      Author : Jane You
* Last Changed: September 7th, 2006
*/
ia_word16 CA_SCARD_GetGlobalSmartCardStatus(ia_word32 wResourceId)
{
	ia_word16 wSCardStatus = 0xffff;
	ia_status_st stScardStatus;

	if (INVALID_SMARTCARDID == wResourceId)
	{
		wSCardStatus = STATUS_CARD_UNKNOWN /*jyou: 3.4.2 default status STATUS_CARD_OUT*/;
	}
	else if (CA_SCARD_GetSmartCardStatus(wResourceId, &stScardStatus) == IA_SUCCESS)
	{
		wSCardStatus = stScardStatus.wStatus;
	}
	else
	{
		wSCardStatus = STATUS_CARD_UNKNOWN /*jyou: 3.4.2 default status STATUS_CARD_OUT*/;
	}
	return wSCardStatus;
}

/*
*     ROUTINE : CA_SCARD_GetGlobalSmartCardStatusStructure()
* DESCRIPTION : Returns the stored latest status of a given smart card or CARD-OUT by default.
*       INPUT : ia_word32 wResourceId - Resource ID of the given smart card.
*					When equals INVALID_SMARTCARDID means request the default value.
*      OUTPUT : None
*     RETURNS : =={XX, SOURCE_SMARTCARD, XX} - The actual stored status.
*				=={CARD-OUT, SOURCE_UNKNOWN, INFO-DND} - The default value.
* SIDE EFFECT : None
*   CALLED BY : CA_SCARD_ResetSmartCardInfo
*      Author : Jane You
* Last Changed: September 7, 2006
*/
ia_status_st CA_SCARD_GetGlobalSmartCardStatusStructure(ia_word32 wResourceId)
{
	ia_status_st stScardStatus;

	/* assign default value */
	ansi_memset((void*) &stScardStatus, 0, sizeof(stScardStatus));

	/*jyou: 3.4.2 default status STATUS_CARD_OUT*/
	stScardStatus.wStatus = STATUS_CARD_UNKNOWN ;
	stScardStatus.eSource = SOURCE_UNKNOWN;
	stScardStatus.eSeverity = STATUS_INFO_DND;

	if ( wResourceId != INVALID_SMARTCARDID )
	{
		/* Obtain the stored status, stay as default if wResourceId doesn't match */
		CA_SCARD_GetSmartCardStatus(wResourceId, &stScardStatus);
	}
	return stScardStatus;
}


/******************************************************************************
* jyou: ECT 3.4-1	Smartcard Diagnose
*	There are four pairs of query/reply APIs added to softcell for this feature
*	For each pair of the SC APIs, ECT composes a corresponding API.
*	Each of the API includes four functions, two public and two local.
*	The two local ones are:
*		SendSmartCard<API_NAME>QueryMsg(<ECT_API_STRUCTURE>* pstMsg)
*		--send a softcell query msg to softcell
*		ReadSmartCard<API_NAME>ReplyMsg(<ECT_API_STRUCTURE>* pstMsg)
*		--return the stored info in m_astLocalSCardInfo to the corresponding CA Client
*		--these two functions are called by CA_SCARD_SmartCard<API_NAME>Control
*	The two public ones are:
*		CA_SCARD_ParseSmartCardUniqueAddressReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
*		--store returned data from softcell to a local storage: m_astLocalSCardInfo
*		--this function is called by ca_task::CaProcessMsg
*		--this function calls CA_CLIENT_Notify(wResourceID, eQuery, bSectorID)
*		CA_SCARD_SmartCardUniqueAddressControl(void *pCAClientMsg)
*		--process the QueryControl sent by a CAClient.
*		--this function is called by ca_task::CA_TASK_QueryControl
*		--this function calls the two local ones for the same API
*
* So all together, there are 16 new functions.
* For ease of future expansion, we group the functions by their purpose
* (not by their API type). And we give one general description of each function
* group in that they share a similar declaration.
******************************************************************************/

/*
* Function group 1
* Purpose:
*	Send a softcell query msg to softcell via the Softcell CA Task Driver API
* Parameters:
*	pstMsg	[in]	the data structure passed by a CA Client, refer to ECT-API for detail
* Return:
*	IA_TRUE		the query was success
*	IA_FALSE	the query was not available
*/
static ia_bool SendSmartCardUniqueAddressQueryMsg(CA_SMARTCARD_UNIQUE_ADDRESS_T* pstMsg)
{
	ia_bool ret = IA_FALSE;

	msg_unique_addr_query_st *pstS3Msg;

	CA_DBG_V(("[ca_scard]:=> %s\n", astrSCellMsgDisplay[MSG_SMARTCARD_UNIQUE_ADDRESS_QUERY]));

	pstS3Msg = ALLOCATE_MEMORY(msg_unique_addr_query_st, 1);
	if (pstS3Msg != 0)
	{
		/* prepare the message structure to softcell */
		ansi_memset((void *)pstS3Msg, 0, sizeof(msg_unique_addr_query_st));
		pstS3Msg->wResourceId = pstMsg->wResourceId;
		/* send the message structure to softcell */
		if (SCELL_Message(MSG_SMARTCARD_UNIQUE_ADDRESS_QUERY, 0, sizeof(msg_unique_addr_query_st),
			(void*)pstS3Msg) != IA_SUCCESS)
		{
			if (pstS3Msg != 0)
			{
				FREE_MEMORY(pstS3Msg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
		else
		{
			ret = IA_TRUE;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}
	return ret;
}

static ia_bool SendSmartCardVendorIDQueryMsg(CA_SMARTCARD_VENDOR_ID_T* pstMsg)
{
	ia_bool ret = IA_FALSE;
	msg_vendor_id_query_st *pstS3Msg;

	CA_DBG_V(("[ca_scard]:=> %s\n", astrSCellMsgDisplay[MSG_SMARTCARD_VENDOR_ID_QUERY]));

	pstS3Msg = ALLOCATE_MEMORY(msg_vendor_id_query_st, 1);
	if (pstS3Msg != 0)
	{
		/* prepare the message structure to softcell */
		ansi_memset((void *)pstS3Msg, 0, sizeof(msg_vendor_id_query_st));
		pstS3Msg->wResourceId = pstMsg->wResourceId;
		/* send the message structure to softcell */
		if (SCELL_Message(MSG_SMARTCARD_VENDOR_ID_QUERY, 0, sizeof(msg_vendor_id_query_st),
			(void*)pstS3Msg) != IA_SUCCESS)
		{
			if (pstS3Msg != 0)
			{
				FREE_MEMORY(pstS3Msg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
		else
		{
			ret = IA_TRUE;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}
	return ret;
}

static ia_bool SendSmartCardSectorAddressQueryMsg(CA_SMARTCARD_SECTOR_ADDRESS_T* pstMsg)
{
	ia_bool ret = IA_FALSE;

	msg_sector_addr_query_st *pstS3Msg;

	/* validate the input parameter */
	if ( pstMsg->bSectorID >= CA_SCARD_MAX_SECTOR_COUNT )
	{
		return ret;
	}

	CA_DBG_V(("[ca_scard]:=> %s %d\n",
		astrSCellMsgDisplay[MSG_SMARTCARD_SECTOR_ADDRESS_QUERY],
		pstMsg->bSectorID));

	pstS3Msg = ALLOCATE_MEMORY(msg_sector_addr_query_st, 1);
	if (pstS3Msg != 0)
	{
		/* prepare the message structure to softcell */
		ansi_memset((void *)pstS3Msg, 0, sizeof(msg_sector_addr_query_st));
		pstS3Msg->wResourceId = pstMsg->wResourceId;
		pstS3Msg->bSector = pstMsg->bSectorID;
		/* send the message structure to softcell */
		if (SCELL_Message(MSG_SMARTCARD_SECTOR_ADDRESS_QUERY, 0, sizeof(msg_sector_addr_query_st),
			(void*)pstS3Msg) != IA_SUCCESS)
		{
			if (pstS3Msg != 0)
			{
				FREE_MEMORY(pstS3Msg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
		else
		{
			ret = IA_TRUE;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}
	return ret;
}

/* Sector datecode */
static ia_bool SendSmartCardSectorDatecodeQueryMsg(CA_SMARTCARD_SECTOR_DATECODE_T *pstMsg)
{
	ia_bool ret = IA_FALSE;

	msg_datecode_query_st *pstS3Msg;

	/* validate the input parameter */
	if ( pstMsg->bSectorID >= CA_SCARD_MAX_SECTOR_COUNT )
	{
		return ret;
	}

	CA_DBG_V(("[ca_scard]:=> %s %d\n",
		astrSCellMsgDisplay[MSG_SMARTCARD_SECTOR_DATECODE_QUERY],
		pstMsg->bSectorID));

	pstS3Msg = ALLOCATE_MEMORY(msg_datecode_query_st, 1);
	if (pstS3Msg != 0)
	{
		/* prepare the message structure to softcell */
		ansi_memset((void *)pstS3Msg, 0, sizeof(msg_datecode_query_st));
		pstS3Msg->wResourceId = pstMsg->wResourceId;
		pstS3Msg->bSector = pstMsg->bSectorID;
		/* send the message structure to softcell */
		if (SCELL_Message(MSG_SMARTCARD_SECTOR_DATECODE_QUERY, 0, sizeof(msg_datecode_query_st),
			(void*)pstS3Msg) != IA_SUCCESS)
		{
			if (pstS3Msg != 0)
			{
				FREE_MEMORY(pstS3Msg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
		else
		{
			ret = IA_TRUE;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	return ret;
}


static ia_bool SendSmartCardOperatorIDQueryMsg(CA_SMARTCARD_OPERATOR_ID_T*pstMsg)
{
	ia_bool ret = IA_FALSE;

	msg_operator_id_query_st *pstS3Msg;

	/* validate the input parameter */
	if ( pstMsg->bSectorID >= CA_SCARD_MAX_SECTOR_COUNT )
	{
		return ret;
	}

	CA_DBG_V(("[ca_scard]:=> %s %d\n",
		astrSCellMsgDisplay[MSG_SMARTCARD_SECTOR_OPERATORID_QUERY],
		pstMsg->bSectorID));

	pstS3Msg = ALLOCATE_MEMORY(msg_operator_id_query_st, 1);
	if (pstS3Msg != 0)
	{
		/* prepare the message structure to softcell */
		ansi_memset((void *)pstS3Msg, 0, sizeof(msg_operator_id_query_st));
		pstS3Msg->wResourceId = pstMsg->wResourceId;
		pstS3Msg->bSector = pstMsg->bSectorID;
		/* send the message structure to softcell */
		if (SCELL_Message(MSG_SMARTCARD_SECTOR_OPERATORID_QUERY, 0, sizeof(msg_operator_id_query_st),
			(void*)pstS3Msg) != IA_SUCCESS)
		{
			if (pstS3Msg != 0)
			{
				FREE_MEMORY(pstS3Msg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
		else
		{
			ret = IA_TRUE;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	return ret;
}

static ia_bool SendSmartCardMRCapabilityQueryMsg(CA_MR_CAPABILITY_T *pstMsg)
{
	ia_bool ret = IA_FALSE;

	msg_smartcard_query_data_st *pstS3Msg;

	CA_DBG_V(("[ca_scard]:=> %s %d\n",
		astrSCellMsgDisplay[MSG_MATURITY_RATING_CAPABILITY_QUERY]));

	pstS3Msg = ALLOCATE_MEMORY(msg_smartcard_query_data_st, 1);
	if (pstS3Msg != 0)
	{
		/* prepare the message structure to softcell */
		ansi_memset((void *)pstS3Msg, 0, sizeof(msg_smartcard_query_data_st));
		pstS3Msg->wResourceId = pstMsg->wResourceId;
		/* send the message structure to softcell */
		if (SCELL_Message(MSG_MATURITY_RATING_CAPABILITY_QUERY, 0, sizeof(msg_smartcard_query_data_st),
			(void*)pstS3Msg) != IA_SUCCESS)
		{
			if (pstS3Msg != 0)
			{
				FREE_MEMORY(pstS3Msg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
		else
		{
			ret = IA_TRUE;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	return ret;
}

static ia_bool SendSmartCardMRConfigurationQueryMsg(CA_MR_CONFIGURATION_T *pstMsg)
{
	ia_bool ret = IA_FALSE;
	CA_LOCAL_MR_MSG_CONFIG_T *pstS3Msg;
	ia_byte bSize;

	/* validate the input parameter */
	CA_DBG_V(("[ca_scard]:=> %s %d\n",
		astrSCellMsgDisplay[MSG_MATURITY_RATING_CONFIG_QUERY]));

	pstS3Msg = ALLOCATE_MEMORY(CA_LOCAL_MR_MSG_CONFIG_T, 1);
	if (pstS3Msg != 0)
	{
		/* prepare the message structure to softcell */
		ansi_memset((void *)pstS3Msg, 0, sizeof(CA_LOCAL_MR_MSG_CONFIG_T));
		pstS3Msg->pstMRConfigQuery.wResourceId = pstMsg->wResourceId;
		pstS3Msg->pstMRConfigQuery.abSubscriberPin[0] = pstMsg->abSubscriberPin[0];
		pstS3Msg->pstMRConfigQuery.abSubscriberPin[1] = pstMsg->abSubscriberPin[1];
		pstS3Msg->pstMRConfigQuery.eAction = (msg_mr_config_action_e)pstMsg->eAction;
		if (pstMsg->eAction == CA_MR_CONFIG_UPDATE)
		{
			pstS3Msg->pstMRConfigData.fEnable = pstMsg->fEnable;
			bSize = sizeof(msg_mr_config_query_st) + sizeof(msg_mr_config_data_st);
		}
		else
		{
			bSize = sizeof(msg_mr_config_query_st);
		}


		/* send the message structure to softcell */
		if (SCELL_Message(MSG_MATURITY_RATING_CONFIG_QUERY, 0, bSize,
			(void*)pstS3Msg) != IA_SUCCESS)
		{
			if (pstS3Msg != 0)
			{
				FREE_MEMORY(pstS3Msg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
		else
		{
			ret = IA_TRUE;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	return ret;
}

static ia_bool SendSmartCardMRProfileQueryMsg(CA_MR_PROFILE_T *pstMsg)
{
	ia_bool ret = IA_FALSE;

	msg_mr_user_profile_query_st *pstS3Msg;

	/* validate the input parameter */
	CA_DBG_V(("[ca_scard]:=> %s\n",
		astrSCellMsgDisplay[MSG_MATURITY_RATING_USER_PROFILE_QUERY]));

	pstS3Msg = ALLOCATE_MEMORY(msg_mr_user_profile_query_st, 1);
	if (pstS3Msg != 0)
	{
		/* prepare the message structure to softcell */
		ansi_memset((void *)pstS3Msg, 0, sizeof(msg_mr_user_profile_query_st));
		pstS3Msg->wResourceId = pstMsg->wResourceId;
		pstS3Msg->abSubscriberPin[0] = pstMsg->abSubscriberPin[0];
		pstS3Msg->abSubscriberPin[1] = pstMsg->abSubscriberPin[1];
		pstS3Msg->eAction = (msg_mr_user_profile_action_e)pstMsg->eAction;
		ansi_memcpy((void*)&(pstS3Msg->stUserProfile), (void*)&(pstMsg->stUserProfile),
				sizeof(CA_MR_USER_PROFILE_DATA_T));

		/* send the message structure to softcell */
		if (SCELL_Message(MSG_MATURITY_RATING_USER_PROFILE_QUERY, 0, sizeof(msg_mr_user_profile_query_st),
			(void*)pstS3Msg) != IA_SUCCESS)
		{
			if (pstS3Msg != 0)
			{
				FREE_MEMORY(pstS3Msg);
			}
			CA_ERROR(("Sending failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		}
		else
		{
			ret = IA_TRUE;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	return ret;
}


/*
* Function group 2
* Purpose:
*	Return the stored info in m_astLocalSCardInfo to the corresponding CA Client via ECT API
* Parameters:
*	pstMsg	[in,out]	the data structure passed by a CA Client, refer to ECT-API for detail
* Return:
*	IA_TRUE		the read was success
*	IA_FALSE	the reas failed
*/
static ia_bool ReadSmartCardUniqueAddressReplyMsg(CA_SMARTCARD_UNIQUE_ADDRESS_T* pstMsg)
{
	ia_byte i;
	ia_bool ret = IA_FALSE;

	CA_DBG_V(("[ca_scard]:<= %s\n", astrSCellMsgDisplay[MSG_SMARTCARD_UNIQUE_ADDRESS_REPLY]));

	/* locate the corresponding sector information */
	i = CA_LOCAL_FindSmartCardLocation(pstMsg->wResourceId);
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return ret;
	}
	/* retrieve data */
	pstMsg->bEMMFilterNumber = m_astLocalSCardInfo[i].bEMMFilterNumber;
	ansi_memcpy(
		(void*)pstMsg->abUniqueAddress,
		(void*)m_astLocalSCardInfo[i].abUniqueAddress,
		UNIQUE_ADDRESS_LENGTH);

	return IA_TRUE;
}

static ia_bool ReadSmartCardVendorIDReplyMsg( CA_SMARTCARD_VENDOR_ID_T* pstMsg )
{
	ia_byte i, j;
	ia_bool ret = IA_FALSE;

	CA_DBG_V(("[ca_scard]:<= %s\n", astrSCellMsgDisplay[MSG_SMARTCARD_VENDOR_ID_REPLY]));

	/* locate the corresponding sector information */
	i = CA_LOCAL_FindSmartCardLocation(pstMsg->wResourceId);
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return ret;
	}
	/* retrieve data */
	pstMsg->bNumOfVendorIDs = m_astLocalSCardInfo[i].bNumOfVendorIDs;
	pstMsg->pstVendorIDs = (CA_VENDOR_ID_T*)ALLOCATE_MEMORY(CA_VENDOR_ID_T, pstMsg->bNumOfVendorIDs);

	/* Fixed SCR#27163 */
	if (pstMsg->pstVendorIDs == 0 && pstMsg->bNumOfVendorIDs > 0)
	{
		CA_ERROR(("ReadSmartCardVendorIDReplyMsg: Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ret;
	}
	for ( j=0; j<pstMsg->bNumOfVendorIDs; j++)
	{
		pstMsg->pstVendorIDs[j].fActive = m_astLocalSCardInfo[i].astVendorIDs[j].fActive;
		pstMsg->pstVendorIDs[j].wVendorID = m_astLocalSCardInfo[i].astVendorIDs[j].wVendorID;
	}
	return IA_TRUE;
}

static ia_bool ReadSmartCardSectorAddressReplyMsg( CA_SMARTCARD_SECTOR_ADDRESS_T* pstMsg )
{
	ia_bool ret = IA_FALSE;

	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	CA_DBG_V(("[ca_scard]:<= %s %d\n",
		astrSCellMsgDisplay[MSG_SMARTCARD_SECTOR_ADDRESS_REPLY],
		pstMsg->bSectorID));

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

	pstMsg->fSectorValid = pstSector->fSectorValid;
	ansi_memcpy(
		(void*)pstMsg->abSectorAddress,
		(void*)pstSector->abSectorAddress,
		SECTOR_ADDRESS_LENGTH);

	return IA_TRUE;
}

static ia_bool ReadSmartCardSectorDatecodeReplyMsg(CA_SMARTCARD_SECTOR_DATECODE_T *pstMsg)
{
	ia_bool ret = IA_FALSE;
	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	CA_DBG_V(("[ca_scard]:<= MSG_SMARTCARD_SECTOR_DATECODE_REPLY %d\n", pstMsg->bSectorID));

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

	pstMsg->fSectorValid = pstSector->fSectorValid;
	pstMsg->wDateCode = pstSector->wDateCode;

	return IA_TRUE;
}


static ia_bool ReadSmartCardOperatorIDReplyMsg(CA_SMARTCARD_OPERATOR_ID_T *pstMsg)
{
	ia_bool ret = IA_FALSE;
	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	CA_DBG_V(("[ca_scard]:<= MSG_SMARTCARD_SECTOR_OPERATORID_REPLY %d\n", pstMsg->bSectorID));

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

	pstMsg->fSectorValid= pstSector->fSectorValid;
	pstMsg->wOperatorID= pstSector->wOperatorID;

	return IA_TRUE;
}

static ia_bool ReadSmartCardMRConfigurationReplyMsg( CA_MR_CONFIGURATION_T* pstMsg )
{
	ia_byte i;
	ia_bool ret = IA_FALSE;

	CA_DBG_V(("[ca_scard]:<= %s\n", astrSCellMsgDisplay[MSG_MATURITY_RATING_CONFIG_REPLY]));

	/* locate the corresponding sector information */
	i = CA_LOCAL_FindSmartCardLocation(pstMsg->wResourceId);
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return ret;
	}
	/* retrieve data */
	pstMsg->eAction = m_astLocalSCardInfo[i].eConfigAction;
	pstMsg->eResult = m_astLocalSCardInfo[i].eConfigResult;
	if ((pstMsg->eAction == CA_MR_CONFIG_READ )
		&& (pstMsg->eResult == CA_MR_CONFIG_OK))
	{
		pstMsg->fEnable = m_astLocalSCardInfo[i].fMREnable;
	}
	return IA_TRUE;
}

static ia_bool ReadSmartCardMRProfileReplyMsg( CA_MR_PROFILE_T* pstMsg )
{
	ia_byte i;
	ia_bool ret = IA_FALSE;

	CA_DBG_V(("[ca_scard]:<= %s\n", astrSCellMsgDisplay[MSG_MATURITY_RATING_USER_PROFILE_REPLY]));

	/* locate the corresponding sector information */
	i = CA_LOCAL_FindSmartCardLocation(pstMsg->wResourceId);
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		return ret;
	}
	/* retrieve data */
	pstMsg->eAction = m_astLocalSCardInfo[i].eProfileAction;
	pstMsg->stUserProfile = m_astLocalSCardInfo[i].stUserProfile;
	return IA_TRUE;
}


/*
* Function group 3
* Purpose:
*	Parse the reply message for a previously sent query returned via
*		SoftCell CA Task Driver API.
*	Store the parsed data into a local data storage: m_astLocalSCardInfo.
*	Send out a corresponding Notification to the CA Client to trigger a
*		QueryControl with REQUEST_TYPE_REPLY.
* Parameters:
*	pstSCellMsg	[in]	the returned message from softcell
*/
void CA_SCARD_ParseSmartCardUniqueAddressReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_unique_addr_reply_st stReply;
	ia_byte i;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply, sizeof(msg_unique_addr_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/* locate the corresponding smartcard */
	i = CA_LOCAL_AddSmartCardInfo(stReply.wResourceId);
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/*save data*/
	m_astLocalSCardInfo[i].bEMMFilterNumber =stReply.bEMMFilterNum;
	ansi_memcpy(
		(void*)m_astLocalSCardInfo[i].abUniqueAddress,
		(void*)stReply.abUniqueAddress,
		UNIQUE_ADDRESS_LENGTH);
	/*send notification*/
	CA_CLIENT_Notify(stReply.wResourceId, CA_QC_SMARTCARD_UNIQUE_ADDRESS, 0);

	IA_CloseMessage(pvMsg, IA_FALSE);

	return;
}

void CA_SCARD_ParseSmartCardVendorIDReplyMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	msg_vendor_id_reply_st stReply;
	msg_vendor_id_st	stVendorID;
	ia_byte i, j;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply, sizeof(msg_unique_addr_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/* locate the corresponding smartcard */
	i = CA_LOCAL_AddSmartCardInfo(stReply.wResourceId);
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/*save data*/
	/*limitition of the vendor IDs, max is 4*/
	m_astLocalSCardInfo[i].bNumOfVendorIDs = (ia_byte)((stReply.bNumOfVendorId >= 4) ? 4 : stReply.bNumOfVendorId);
	for ( j = 0; j < m_astLocalSCardInfo[i].bNumOfVendorIDs; j++ )
	{
		if (IA_ReadFromMessage(pvMsg, &stVendorID, sizeof(msg_vendor_id_st))	== 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return ;
		}
		m_astLocalSCardInfo[i].astVendorIDs[j].wVendorID = stVendorID.wVendorId;
		m_astLocalSCardInfo[i].astVendorIDs[j].fActive = stVendorID.fActive;
	}
	/*send notification*/
	CA_CLIENT_Notify(stReply.wResourceId, CA_QC_SMARTCARD_VENDOR_ID, 0);

	IA_CloseMessage(pvMsg, IA_FALSE);

	return;
}

void CA_SCARD_ParseSmartCardSectorAddressReplyMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	msg_sector_addr_reply_st stReply;
	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply, sizeof(msg_sector_addr_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/* validate the sector index */
	if ( stReply.bSector >= CA_SCARD_MAX_SECTOR_COUNT )
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* locate the corresponding smartcard */
	pstSector = CA_LOCAL_FindSmartCardSector( stReply.wResourceId, stReply.bSector );
	if ( pstSector == 0 )
		/* cann't find the resource id */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}

	/* save the sector info into the local buffer */
	pstSector->fSectorValid = stReply.fSectorValid;
	ansi_memcpy((void*)pstSector->abSectorAddress, (void*)stReply.abGroupAddress,
		SECTOR_ADDRESS_LENGTH);
	/*send notification*/
	CA_CLIENT_Notify(stReply.wResourceId, CA_QC_SMARTCARD_SECTOR_ADDRESS, stReply.bSector);

	IA_CloseMessage(pvMsg, IA_FALSE);
	return;
}

void CA_SCARD_ParseSmartCardSectorDatecodeReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{

	ia_msg_handle pvMsg;
	msg_datecode_reply_st stReply;
	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply, sizeof(msg_datecode_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/* validate the sector index */
	if ( stReply.bSector >= CA_SCARD_MAX_SECTOR_COUNT )
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/* locate the corresponding smartcard */
	pstSector = CA_LOCAL_FindSmartCardSector( stReply.wResourceId, stReply.bSector );
	if ( pstSector == 0 )
		/* cann't find the resource id */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}
	/* save the sector info into the local buffer */
	pstSector->fSectorValid = stReply.fSectorValid;
	pstSector->wDateCode = stReply.wDateCode;
	/*send notification*/
	CA_CLIENT_Notify(stReply.wResourceId, CA_QC_SMARTCARD_SECTOR_DATECODE, stReply.bSector);

	IA_CloseMessage(pvMsg, IA_FALSE);
	return;
}

void CA_SCARD_ParseSmartCardOperatorIDReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	msg_operator_id_reply_st stReply;
	CA_LOCAL_SECTOR_INFO_T	*pstSector;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply, sizeof(msg_operator_id_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/* validate the sector index */
	if ( stReply.bSector >= CA_SCARD_MAX_SECTOR_COUNT )
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/* locate the corresponding smartcard */
	pstSector = CA_LOCAL_FindSmartCardSector( stReply.wResourceId, stReply.bSector );
	if ( pstSector == 0 )
		/* cann't find the resource id */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}
	/* save the sector info into the local buffer */
	pstSector->fSectorValid= stReply.fSectorValid;
	pstSector->wOperatorID = stReply.wOperatorId;
	/*send notification*/
	CA_CLIENT_Notify(stReply.wResourceId, CA_QC_SMARTCARD_OPERATOR_ID, stReply.bSector);

	IA_CloseMessage(pvMsg, IA_FALSE);

	return;
}

void CA_SCARD_MRCapabilityNotify( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	msg_mr_capability_reply_st stReply;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply, sizeof(msg_mr_capability_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/*send notification*/
	CA_CLIENT_Notify(stReply.wResourceId, CA_QC_MR_CAPABILITY, stReply.fMRcapable);

	IA_CloseMessage(pvMsg, IA_FALSE);

	return;
}

void CA_SCARD_MRConfigurationNotify( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	CA_LOCAL_MR_MSG_CONFIG_REPLY_T stReply;
	ia_byte i;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply.pstMRConfigReply, sizeof(msg_mr_config_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* locate the corresponding smartcard */
	i = CA_LOCAL_AddSmartCardInfo(stReply.pstMRConfigReply.wResourceId);
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/*save data*/
	m_astLocalSCardInfo[i].eConfigAction = (CA_MR_CONFIG_ACTION_EN)stReply.pstMRConfigReply.eAction;
	m_astLocalSCardInfo[i].eConfigResult = (CA_MR_CONFIG_RESULT_EN)stReply.pstMRConfigReply.eResult;
	if ((m_astLocalSCardInfo[i].eConfigAction == CA_MR_CONFIG_READ)
		&& (m_astLocalSCardInfo[i].eConfigResult == CA_MR_CONFIG_OK))
	{
		if (IA_ReadFromMessage(pvMsg, &stReply.pstMRConfigData, sizeof(msg_mr_config_data_st))	== 0)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return ;
		}

		m_astLocalSCardInfo[i].fMREnable = stReply.pstMRConfigData.fEnable;
	}

	/*send notification*/
	CA_CLIENT_Notify(stReply.pstMRConfigReply.wResourceId, CA_QC_MR_CONFIGURATION, stReply.pstMRConfigReply.eResult);

	IA_CloseMessage(pvMsg, IA_FALSE);

	return;
}

void CA_SCARD_MRProfileNotify( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	msg_mr_user_profile_reply_st stReply;
	ia_byte i;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	/* read message */
	if (IA_ReadFromMessage(pvMsg, &stReply, sizeof(msg_mr_user_profile_reply_st))	== 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	/* locate the corresponding smartcard */
	i = CA_LOCAL_AddSmartCardInfo(stReply.wResourceId);
	if (i == SC_SLOT_INVALID)
	{
		/* cann't find the resource id */
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	/*save data*/
	m_astLocalSCardInfo[i].eProfileAction = (CA_MR_PROFILE_ACTION_EN)stReply.eAction;
	ansi_memcpy((void*)&(m_astLocalSCardInfo[i].stUserProfile), (void*)&(stReply.stUserProfile),
			sizeof(CA_MR_USER_PROFILE_DATA_T));

	/*send notification*/
	CA_CLIENT_Notify(stReply.wResourceId, CA_QC_MR_PROFILE, stReply.eResult);

	IA_CloseMessage(pvMsg, IA_FALSE);

	return;
}
/*
* Function group 4
* Purpose: process CA Client's Query Control reqeust
* Parameters:
*	pCAClientMsg	[in]	the input data passed by a CA Client
* Return:
*	CA_QUERY_SUCCESS		the query was success
*	CA_QUERY_NOT_AVAILABLE	softcell does not take the query
*	CA_QUERY_FAIL			the query failed
*/
CA_QUERY_STATUS_EN CA_SCARD_SmartCardUniqueAddressControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_bool ret = IA_FALSE;
	CA_SMARTCARD_UNIQUE_ADDRESS_T	*pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	CA_PARAM_NOT_USED( wHandle );
	pstMsg = (CA_SMARTCARD_UNIQUE_ADDRESS_T*)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		ret = SendSmartCardUniqueAddressQueryMsg( pstMsg );
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;
	}
	else
	{
		ret = ReadSmartCardUniqueAddressReplyMsg( pstMsg );
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	}

	return eStatus ;
}

CA_QUERY_STATUS_EN CA_SCARD_SmartCardVendorIDControl(  ia_word32 wHandle, void* pCAClientMsg  )
{
	ia_bool ret;
	CA_SMARTCARD_VENDOR_ID_T	*pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	CA_PARAM_NOT_USED( wHandle );
	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_SMARTCARD_VENDOR_ID_T*)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		ret = SendSmartCardVendorIDQueryMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;
	}
	else
	{
		ret = ReadSmartCardVendorIDReplyMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	}

	return eStatus;
}

CA_QUERY_STATUS_EN CA_SCARD_SmartCardSectorAddressControl(  ia_word32 wHandle, void* pCAClientMsg )
{
	ia_bool ret;
	CA_SMARTCARD_SECTOR_ADDRESS_T	*pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	CA_PARAM_NOT_USED( wHandle );
	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_SMARTCARD_SECTOR_ADDRESS_T*)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		ret = SendSmartCardSectorAddressQueryMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;
	}
	else
	{
		ret = ReadSmartCardSectorAddressReplyMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	}

	return eStatus ;
}

CA_QUERY_STATUS_EN CA_SCARD_SmartCardSectorDatecodeControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_bool ret;
	CA_SMARTCARD_SECTOR_DATECODE_T	*pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	CA_PARAM_NOT_USED( wHandle );
	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_SMARTCARD_SECTOR_DATECODE_T*)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		ret = SendSmartCardSectorDatecodeQueryMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;
	}
	else
	{
		ret = ReadSmartCardSectorDatecodeReplyMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	}

	return eStatus ;
}

CA_QUERY_STATUS_EN CA_SCARD_SmartCardOperatorIDQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_bool ret;
	CA_SMARTCARD_OPERATOR_ID_T	*pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	CA_PARAM_NOT_USED( wHandle );
	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_SMARTCARD_OPERATOR_ID_T*)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		ret = SendSmartCardOperatorIDQueryMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;
	}
	else
	{
		ret = ReadSmartCardOperatorIDReplyMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	}

	return eStatus ;
}

CA_QUERY_STATUS_EN CA_SCARD_MRCapabilityQueryControl(  ia_word32 wHandle, void* pCAClientMsg  )
{
	ia_bool ret;
	CA_MR_CAPABILITY_T *pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	CA_PARAM_NOT_USED( wHandle );
	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_MR_CAPABILITY_T *)pCAClientMsg;
	ret = SendSmartCardMRCapabilityQueryMsg(pstMsg);
	eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;

	return eStatus;
}

CA_QUERY_STATUS_EN CA_SCARD_MRConfigurationQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_bool ret;
	CA_MR_CONFIGURATION_T	*pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	CA_PARAM_NOT_USED( wHandle );
	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_MR_CONFIGURATION_T *)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		ret = SendSmartCardMRConfigurationQueryMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;
	}
	else
	{
		ret = ReadSmartCardMRConfigurationReplyMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	}

	return eStatus ;
}

CA_QUERY_STATUS_EN CA_SCARD_MRProfileQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_bool ret;
	CA_MR_PROFILE_T	*pstMsg;
	CA_QUERY_STATUS_EN eStatus;

	CA_PARAM_NOT_USED( wHandle );
	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	pstMsg = (CA_MR_PROFILE_T *)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		ret = SendSmartCardMRProfileQueryMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;
	}
	else
	{
		ret = ReadSmartCardMRProfileReplyMsg(pstMsg);
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	}

	return eStatus ;
}
void CA_SCARD_ParseNumberOfSectorsNotify( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_word32	wSCardId;
	ia_byte		bSectorNumberNew, i;

	wSCardId = CA_LOCAL_GetSmartCardIdFromSectorMsg( (CA_SCELL_MSG *)pstSCellMsg );
	CA_SCARD_ParseSectorsNumberMsg( pstSCellMsg );
	/* jyou: 3.4.4 */
	/* Receives this notify after a Card-In or sector count chages */
	/* Should re-query PPT and IPPV info under any condition */

	bSectorNumberNew = CA_SCARD_GetSectorNumber(wSCardId);
	/*[irca ect] pyh: 添加对bSectorNumberNew有效性的判断*/
	if (bSectorNumberNew == INVALID_SC_SECTOR)
		return;
	
	/*Initialize the IPPV and PPT TBD*/
	CA_PPT_Initialise(wSCardId);
	CA_IPPV_Initialise(wSCardId);
	CA_LPPV_Initialise(wSCardId);
	CA_HGPC_Initialise(wSCardId);

	for( i = 0; i < bSectorNumberNew; i++ )
	{
		CA_PPT_SetSectorBySectorIndex(wSCardId, ( ia_byte )i);
		CA_IPPV_SetSectorBySectorIndex(wSCardId, ( ia_byte )i);
		CA_PPT_InfoSendQuery(wSCardId, ( ia_byte )i);
		CA_IPPV_SendDebitQuery(wSCardId, ( ia_byte )i);
	}
	CA_EXPIRED_PRODUCTSendQuery(wSCardId);  /* jyou: fixed. memory leak mli */
	return;
}

/******************************************************************************
* jyou: ECT 3.4-1	Smartcard Diagnose
* Auxiliary functions to manage the local data storage: m_astLocalSCardInfo
******************************************************************************/

/*
* Purpose: add info of the given wResourceID to the data storage
* Parameters:
*	wResourceID: the resource ID of the smartcard in SoftCell
*		and equals to the SmartCardID passed by a CA Client
* Return:
*	[0, SC_SLOT_MAX)	the index of the smartcard in Local
*	SC_SLOT_INVALID		no available position in Local
* Side Effects: send CA_QC_SMARTCARD_INFO notification to CA Client with
*						SMARTCARD_INFO_MASK_RESOURCE_ID
*/
ia_byte CA_LOCAL_AddSmartCardInfo(ia_word32 wResourceID)
{
	ia_byte i;

	if ( (i = CA_LOCAL_FindSmartCardLocation(wResourceID) ) != SC_SLOT_INVALID )
	{
		return i;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalSCardInfo[i].wResourceID == INVALID_SC_SMARTCARDID)
		{
			m_astLocalSCardInfo[i].wResourceID = wResourceID;
			return i;
		}
	}

	return SC_SLOT_INVALID;
}

/*
* Purpose: clear the entire data storage
************************************************************/
void CA_LOCAL_ClearAllSmartCardLocation( void )
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		m_astLocalSCardInfo[i].wResourceID = INVALID_SC_SMARTCARDID;
		CA_SCARD_ResetSmartCardInfo( i, INVALID_SC_SMARTCARDID );	/*clear all the memories*/
	}

	return;
}

/*
* Purpose: clear the data storage of the given wResourceID
* Parameters:
*	wResourceID: the resource ID of the smartcard in SoftCell
*		and equals to the SmartCardID passed by a CA Client
*/
void CA_LOCAL_ClearSmartCardLocation(ia_word32 wResourceID)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalSCardInfo[i].wResourceID == wResourceID)
		{
			m_astLocalSCardInfo[i].wResourceID = INVALID_SC_SMARTCARDID;
			CA_SCARD_ResetSmartCardInfo( i, INVALID_SC_SMARTCARDID );	/*clear all the memories*/
			break;
		}
	}
	return;
}

/*
* Purpose: locate the data storage of the given wResourceID
* Parameters:
*	wResourceID: the resource ID of the smartcard in SoftCell
*		and equals to the SmartCardID passed by a CA Client
* Return:
*	[0, SC_SLOT_MAX)	the index of the smartcard in Local
*	SC_SLOT_INVALID		not found
*/
ia_byte	CA_LOCAL_FindSmartCardLocation(ia_word32 wResourceID)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalSCardInfo[i].wResourceID == wResourceID)
		{
			return i;
		}
	}

	return SC_SLOT_INVALID;
}

ia_byte CA_SCARD_ValidSmartCards( void )
{
	ia_byte i, bCount=0;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalSCardInfo[i].wResourceID != INVALID_SMARTCARDID )
		{
			bCount++;
		}
	}

	return bCount;
}

CA_LOCAL_SMARTCARD_INFO_T* CA_LOCAL_FindSmartCard(ia_word32 wResourceID)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (m_astLocalSCardInfo[i].wResourceID == wResourceID)
		{
			return &m_astLocalSCardInfo[i];
		}
	}

	return 0;
}

CA_LOCAL_SECTOR_INFO_T* CA_LOCAL_FindSmartCardSector( ia_word32 wResourceID, ia_byte bSectorID )
{
	ia_byte i;

	i = CA_LOCAL_FindSmartCardLocation( wResourceID );
	if ( i == SC_SLOT_INVALID )
	{
		return 0;
	}

	/* check whether there are sectors */
	if ( m_astLocalSCardInfo[i].pstSectors == 0 || m_astLocalSCardInfo[i].bSectorCount <= bSectorID )
	{
		return 0;
	}

	return ( CA_LOCAL_SECTOR_INFO_T* )(m_astLocalSCardInfo[i].pstSectors + bSectorID);
}


ia_byte CA_SCARD_SaveSmartCardResourceID( ia_word32 wResourceId )
{
	ia_byte i;
	i = CA_LOCAL_FindSmartCardLocation( wResourceId );
	if ( i == SC_SLOT_INVALID )
	{
		i = CA_LOCAL_AddSmartCardInfo( wResourceId );
		if ( i != SC_SLOT_INVALID )
		{
			/* a new smart card resource ID added ==> notify CA Client */
			CA_SCARD_SmartCardInfoNotify( wResourceId , SMARTCARD_INFO_MASK_RESOURCE_ID );
		}
	}
	return i;
}

/*
*	ROUTINE : ResetSmartCardXXXX series ( a group of methods )
* 	DESCRIPTION : Reset a given smart card record in the local storage,
*				and free all momory it occpied
* 	INPUT : CA_LOCAL_SMARTCARD_INFO_T pstInfo - The given smart card record.
*	OUTPUT : None
*	RETURNS : None
*	SIDE EFFECT : A unused index in m_astLocalSCardInfo will be occupied if the wResourceId was
*				in m_astLocalSCardInfo before.
*	CALLED BY : CA_SCARD_ResetSmartCardInfo
*	Author : Jane You
*	Last Changed: August 14, 2006
*/
static void ResetSmartCardInfo( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	/* Smart Card Status Information */
	ansi_memset((void*) &(pstInfo->stSmartCardStatus), 0, sizeof(ia_status_st));
	pstInfo->stSmartCardStatus.wStatus = STATUS_CARD_UNKNOWN;
	pstInfo->stSmartCardStatus.eSource = SOURCE_UNKNOWN;
	pstInfo->stSmartCardStatus.eSeverity = STATUS_INFO_DND;

	/* General Information */
	pstInfo->wSmartCardType = INVALID_SC_TYPE;
	pstInfo->bMajorVersion = INVALID_SC_VERSION;
	pstInfo->bMinorVersion = INVALID_SC_VERSION;
	pstInfo->acNationality[0] = INVALID_SC_NAT_CHAR;
	pstInfo->acNationality[1] = INVALID_SC_NAT_CHAR;
	pstInfo->acNationality[2] = INVALID_SC_NAT_CHAR;
	pstInfo->acSerialNumber[0] = '\0'; /* ??? */
	pstInfo->wPatchLevel = 0;
	pstInfo->wOwnerID = 0;
	pstInfo->bVariant = 0;
	pstInfo->bBuild = 0;

	return;
}

static void ResetSmartCardAccess( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	/* Homing Channel Information */
	pstInfo->bHomingChannelAccessOp = CA_SC_ACCESS_READ;
	pstInfo->bHomingChannelLength = 0;
	if (pstInfo->pbHomingChannelData != 0)
	{
		OS_DRV_FreeMemory( (void *)pstInfo->pbHomingChannelData );
		pstInfo->pbHomingChannelData = 0;
	}
	return;
}

static void ResetSmartCardHashID( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	/* Hash ID */
	ansi_memset((void*)pstInfo->acHashNo, 0, sizeof(ia_char)*MAX_SC_SER_NO_LEN);
	return;
}

static void ResetSmartCardSectors( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	ia_byte i;

	if ( pstInfo->bSectorCount > 0 )
	{
		if ( pstInfo->pstSectors != 0 )
		{
			for ( i = 0 ; i < pstInfo->bSectorCount ; i++ )
			{
				/* Free internal memories in Sector Info */
				ResetSmartCardSectorRegion( pstInfo->pstSectors + i );
				ResetSmartCardSectorPPTRecords( pstInfo->pstSectors + i );
				ResetSmartCardSectorIPPVEvents( pstInfo->pstSectors + i );
				ResetSmartCardSectorProducts( pstInfo->pstSectors + i );
				ResetSmartCardSectorAddress( pstInfo->pstSectors + i );
				ResetSmartCardSectorDateCode( pstInfo->pstSectors + i );
				ResetSmartCardIPPVCallbackRequest( pstInfo->pstSectors + i );
				ResetSmartCardSectorOperatorID( pstInfo->pstSectors + i );
			}
			OS_DRV_FreeMemory( (void *)pstInfo->pstSectors );
		}
		if ( pstInfo->pbSectorType != 0 )
		{
			OS_DRV_FreeMemory( (void *)pstInfo->pbSectorType );
		}
	}
	pstInfo->pstSectors = 0;
	pstInfo->pbSectorType = 0;
	/* jyou: 3.4.3.6 SCR#26044 2007-06-01 */
	pstInfo->bSectorCount = 0;
	return;
}

static void ResetSmartCardUserInfo( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	/* User Data */
	ansi_memset((void*)pstInfo->abUserData, 0, sizeof(ia_byte)*USER_DATA_LENGTH);
	return;
}

static void ResetSmartCardRemoteChannel( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	/* Remote Channel Information */
	pstInfo->bRemoteChannelLength = 0;
	if (pstInfo->pbRemoteChannelPayload !=0)
	{
		OS_DRV_FreeMemory((void *)pstInfo->pbRemoteChannelPayload);
		pstInfo->pbRemoteChannelPayload = 0;
	}
	return;
}

static void ResetSmartCardSurfLock( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	pstInfo->fSurfLocked = IA_FALSE;
	pstInfo->wGroupHandle = INVALID_GROUP_SERVICE_HANDLE;
	return;
}

static void ResetSmartCardUniqueAddress( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	ansi_memset((void*)pstInfo->abUniqueAddress, INVALID_SMARTCARD_INFO_BYTE,
		UNIQUE_ADDRESS_LENGTH);
	return;
}

static void ResetSmartCardVendorIDs( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	ia_byte i;

	for ( i = 0 ; i < CA_SCARD_MAX_VENDOR_COUNT ; i++ )
	{
		pstInfo->astVendorIDs[i].fActive = IA_FALSE;
		pstInfo->astVendorIDs[i].wVendorID = INVALID_SMARTCARD_INFO_BYTE;
	}
	return;
}

static void ResetSmartCardMaturityRating( CA_LOCAL_SMARTCARD_INFO_T *pstInfo )
{
	pstInfo->fMRcapable = IA_TRUE;
	pstInfo->abSubscriberPin[0] = pstInfo->abSubscriberPin[0] = 0;
	pstInfo->eConfigAction = CA_MR_CONFIG_READ;
	pstInfo->eProfileAction = CA_MR_USER_PROFILE_READ;
	pstInfo->fMREnable = IA_TRUE;
	ansi_memset((void*)pstInfo->abUniqueAddress, 0x00, sizeof(CA_MR_USER_PROFILE_DATA_T));
	return;
}

/*
*     ROUTINE : ResetSmartCardSectorXXXX series
* DESCRIPTION : Free the extra memories allocated to a given sector of a smart card
*       INPUT : CA_LOCAL_SECTOR_INFO_T* pstInfo - The given smart card sector record.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : Memories allocated for PPT records, IPPV envets, and Products will be
*				freed.
*   CALLED BY : ResetSmartCardSectors
*      Author : Jane You
* Last Changed: August 14, 2006
*/
static void ResetSmartCardSectorRegion( CA_LOCAL_SECTOR_INFO_T *pstInfo )
{
	pstInfo->fSectorValid = IA_FALSE;
	pstInfo->bRegion = INVALID_SMARTCARD_INFO_BYTE;
	pstInfo->bSubRegion = INVALID_SMARTCARD_INFO_BYTE;
}

static void ResetSmartCardSectorPPTRecords( CA_LOCAL_SECTOR_INFO_T *pstInfo )
{
	if ( pstInfo->bPPTRecordCount > 0 && pstInfo->pstPPTRecords != 0 )
	{
		OS_DRV_FreeMemory( (void *)pstInfo->pstPPTRecords );
		pstInfo->pstPPTRecords = 0;
	}
	return;
}

static void ResetSmartCardSectorIPPVEvents( CA_LOCAL_SECTOR_INFO_T *pstInfo )
{
	/* IPPV events information--MSG_IPPV_EVENT_LIST_REPLY */
	if ( pstInfo->bEventCount > 0 && pstInfo->pstIPPVEvents != 0 )
	{
		OS_DRV_FreeMemory( (void *)pstInfo->pstIPPVEvents );
		pstInfo->pstIPPVEvents = 0;
	}
	return;
}

static void ResetSmartCardSectorProducts( CA_LOCAL_SECTOR_INFO_T *pstInfo )
{
	/* Products information--MSG_EXTENDED_PRODUCT_LIST_REPLY */
	if ( pstInfo->bProductCount > 0 && pstInfo->pstProducts != 0 )
	{
		OS_DRV_FreeMemory( (void *)pstInfo->pstProducts );
		pstInfo->pstProducts = 0;
	}
	return;
}

static void ResetSmartCardSectorAddress( CA_LOCAL_SECTOR_INFO_T *pstInfo )
{
	ansi_memset((void*)pstInfo->abSectorAddress, INVALID_SMARTCARD_INFO_BYTE,
		SECTOR_ADDRESS_LENGTH);
	return;
}

static void ResetSmartCardSectorDateCode( CA_LOCAL_SECTOR_INFO_T *pstInfo )
{
	pstInfo->wDateCode = INVALID_SECTOR_DATACODE;
	return;
}

static void ResetSmartCardIPPVCallbackRequest( CA_LOCAL_SECTOR_INFO_T *pstInfo )
{
	if ( pstInfo->pabPhoneNumber != 0 )
	{
		OS_DRV_FreeMemory( (void *)pstInfo->pabPhoneNumber );
		pstInfo->pabPhoneNumber = 0;
	}

	pstInfo->bDelayMultiplier = 0;
	pstInfo->bPhoneControl = 0;
	pstInfo->wDelay = 0;
	pstInfo->bPhoneNumberLength = 0;
	return;
}

static void ResetSmartCardSectorOperatorID( CA_LOCAL_SECTOR_INFO_T *pstInfo )
{
	pstInfo->wOperatorID= INVALID_SECTOR_OPERATOR_ID;
	return;
}

/* mli: 3.3.11 */
#ifdef _AUSTAR_
void CA_SCARD_STATUSReplyMsg(ia_word32 wResourceId)
{
	/*CA_CLIENT_Notify(	wResourceId	, CA_QC_SMARTCARD_STATUS ,	0 );*/
	if (CA_TASK_CheckNotifyEnable(NOTIFY_MASK_SMARTCARD_INFO) == IA_TRUE)
	{
		/* ECT3.3 enhancement: streamline of wHandle */
		CA_CLIENT_Notify( wResourceId ,	CA_QC_SMARTCARD_INFO , 0 );
	}
}

/* jyou: the function of this method is covered by 	CA_SCARD_SetSmartCardStatus() */
void CA_Modi_SC_Status(ia_word32 wSCardId, ia_word16 wSCardStatus)
{
	int	i;
	for	(i = 0;	i <	SC_SLOT_MAX; i++)
	{
		if (wSCardId ==	astSmartCard[i].wResourceId)
			break;
	}

	if (i == SC_SLOT_MAX)
	{
		return;
	}
	astSmartCard[i].stSmartCardStatus.wStatus =	wSCardStatus;
}
#endif
