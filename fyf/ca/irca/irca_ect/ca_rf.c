/**
 * $Id: ca_rf.c,v 1.5 2008/05/09 08:55:08 ayang Exp $
 *
 * Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
 *
 * The Regional Filter module of Embedded CA Task.
 **/

/******************************************************************************
* FILE					: ca_rf.c																											*
*																																							*
* DESCRIPTION   :																															*
*																																							*
* NOTES					:																															*
*																																							*
* STATUS/HISTORY:																															*
* Author		  : Angela Yang																										*
* Status		  : $Revision: 1.5 $																							*
* History			:																																*
*
******************************************************************************/

/******************************** Header Files ********************************/
#include <irca/pubh/s3_osdrv.h>
#include <irca/ca_task.h>
#include "ca_local.h"
#include "ca_svcinfo.h"
#include "ca_scard.h"
#include <irca/pubh/s3_message_helper.h>
#include <irca/pubh/s3_cadrv.h>
#include <irca/pubh/s3_smartcard.h>
#include "ca_rf.h"
#include "ca_error.h"


/******************************* Local Functions ******************************/

CA_QUERY_STATUS_EN CA_RF_ConfigQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	msg_region_filter_config_e *pRFconfig = (msg_region_filter_config_e *)pCAClientMsg;

	CA_PARAM_NOT_USED(wHandle);

	pvMsg = IA_OpenOutputMessage(sizeof(msg_region_filter_config_e), sizeof
			(msg_region_filter_config_e));

	IA_WriteToMessage(pvMsg, pRFconfig, sizeof
		(msg_region_filter_config_e));

	if (SCELL_Message(MSG_REGION_FILTER_CONFIG, 0,
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
		CA_ERROR(("CA_RF_ConfigQueryControl: failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}

CA_QUERY_STATUS_EN CA_RF_SetQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_msg_handle pvMsg;
	ia_byte i;
	CA_RF_SET_T *pRFset = (CA_RF_SET_T *)pCAClientMsg;
	msg_descriptor_st stDescr;
	ia_byte *pbDescr = 0;

	CA_PARAM_NOT_USED(wHandle);

	pvMsg = IA_OpenOutputMessage(128 , 32 );
	if (pvMsg == 0)
	{
			return CA_QUERY_FAIL;
	}

	/* AYang: 2008-03-14 Fix SCR#26921 */
	IA_WriteToMessage(pvMsg, pRFset, sizeof(msg_region_filter_set_info_st));

	pbDescr = pRFset->bMpegDescr;

	for (i=0; i < pRFset->bDescrCount; i++)
	{
		stDescr.bTag = pbDescr[0];
		stDescr.bDataLength = pbDescr[1];
		IA_WriteToMessage( pvMsg, &stDescr, sizeof(msg_descriptor_st) );
		/* Payload of the descriptor excluding tag and length. */
		IA_WriteToMessage( pvMsg, &pbDescr[2], stDescr.bDataLength );

		pbDescr += stDescr.bDataLength + 2;
	}

	if (SCELL_Message(MSG_REGION_FILTER_SET_INFO, 0,
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
		CA_ERROR(("CA_RF_SetQueryControl: failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}

