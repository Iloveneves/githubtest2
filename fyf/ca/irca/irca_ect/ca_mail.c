/**
 * $Id: ca_mail.c,v 1.15 2008/04/14 08:11:52 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* The module handles mail related queries and operations.
**/

/******************************** Header Files ********************************/
/* #include <string.h> */
#include <irca/pubh/s3_ansi_functions.h>
#include <irca/ca_task.h>
#include "ca_local.h"
#include "ca_svcinfo.h"
#include "ird_msg.h"

static CA_QUERY_STATUS_EN BuildCaMailAnnounce(CA_MAIL_T *caMail, MESSAGE
												*pMailMsg);

/*
* Description: Query control for CA_QC_MAIL type.
*
* Parameters : pstScData - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_MAIL_MailQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	MESSAGE *pMailMsg;
	CA_MAIL_T *caMail;
	ia_word16 wNumGroups = CA_SVCINFO_GetSvcMaxNumber(); /* CA_TASK_GetInitCfg()
														->wNumGroups; */

	CA_PARAM_NOT_USED( wHandle );
	caMail = ( CA_MAIL_T* ) pCAClientMsg;

	if (caMail->bIndex >= MAX_MAILS *wNumGroups)
	{
		CA_ERROR(("Mail index is out of limit.\n"));
		return CA_QUERY_FAIL;
	}

	pMailMsg = IRD_MSG_GetMailMsg(caMail->bIndex);
	caMail->bType = CA_MAIL;
	return BuildCaMailAnnounce(caMail, pMailMsg);
}

/*
* Description: Query control for CA_QC_ANNOUNCE type.
*
* Parameters : pstScData - Pointer to the data buffer provided by the manufacturer.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
CA_QUERY_STATUS_EN CA_MAIL_AnnounceQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	MESSAGE *pMailMsg;
	CA_MAIL_T *caMail;

	ia_word16 wNumGroups = CA_SVCINFO_GetSvcMaxNumber(); /* CA_TASK_GetInitCfg()
														 ->wNumGroups; */

	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */
	CA_PARAM_NOT_USED( wHandle );
	caMail = ( CA_MAIL_T* ) pCAClientMsg;
	if (caMail->bIndex >= MAX_ANNOUNCEMENTS *wNumGroups)
	{
		CA_ERROR(("Annoucement index is out of limit.\n"));
		return CA_QUERY_FAIL;
	}

	pMailMsg = IRD_MSG_GetAnnounceMsg(caMail->bIndex);
	caMail->bType = CA_ANNOUNCE;
	return BuildCaMailAnnounce(caMail, pMailMsg);
}


CA_QUERY_STATUS_EN BuildCaMailAnnounce(CA_MAIL_T *caMail, MESSAGE *pMailMsg)
{
	ia_word16 wTempSrvHandle;
	if (pMailMsg != 0)
	{
		/* copy the whole buffer to avoid no '\0' in the buffer. */
		ansi_memcpy(caMail->bMessageData, pMailMsg->bMessageData,
			MAX_EPG_MSG_LENGTH);
		caMail->bClass = pMailMsg->bClass;
		caMail->bPriority = pMailMsg->bPriority;
		caMail->fTimedDelete = (ia_bool)pMailMsg->bTimedDelete;
		/* ayang: 2008-03-13 Fix SCR#26102 */
		caMail->bMsgLength = pMailMsg->bMessageLength;
		wTempSrvHandle = caMail->wServiceHandle = pMailMsg->wServiceHandle;
		caMail->wEmmHandle = CA_SVCINFO_HandleSCell2CaClient(wTempSrvHandle);

		if (caMail->fTimedDelete == IA_TRUE)
			caMail->stDeleteTime = pMailMsg->stDeleteTime;
		return CA_QUERY_SUCCESS;
	}
	else
	{
		return CA_QUERY_FAIL;
	}
}

void CA_MAIL_Notify(ia_word32 wGrpHandle, ia_byte mailIndex)
{
	CA_CLIENT_Notify(wGrpHandle, CA_QC_MAIL, (ia_uint32)mailIndex);
}

void CA_MAIL_NotifyAnnounce(ia_word32 wGrpHandle, ia_byte annIndex)
{
	CA_CLIENT_Notify(wGrpHandle, CA_QC_ANNOUNCE, (ia_uint32)annIndex);
}

