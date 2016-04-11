/**
 * $Id: ca_ppt.c,v 1.16 2008/05/09 08:55:08 ayang Exp $
*
* Copyright (c) 2005 Irdeto Access B.V., All rights reserved.
*
* The PPT module of Embedded CA Task.
**/

/******************************************************************************
* FILE          : CA_PPT.C                                                   *
*                                                                            *
* DESCRIPTION   :  														  *                                                                            *
* NOTES         :                                                            *
*                                                                            *
* STATUS/HISTORY:                                                            *
* Author          : Melissa Li                                                  *
* Status          : $Revision: 1.16 $                                         *
* History         : 														  *
******************************************************************************/

/******************************** Header Files ********************************/
/* #include <string.h> */

#include <irca/pubh/s3_ansi_functions.h>
#include <irca/pubh/s3_osdrv.h>
#include <irca/ca_task.h>
#include "ird_msg.h"
#include "ca_ppt.h"
#include "ca_scard.h"
#include "ca_local.h"
#include <irca/pubh/s3_message_helper.h>
#include <irca/pubh/s3_smartcard.h>
#include "ca_error.h"

static CA_PPT_T *pCaPPTInfos[SC_SLOT_MAX];

/* jyou: 3.4.2 SCR#25311 */
static ia_word16 FindSmartCardIndex( ia_word32 wResourceID )
{
	ia_word16 i;

	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */
	for ( i = 0 ; i < SC_SLOT_MAX ; i++ )
	{
		if ( 0 != pCaPPTInfos[i] )
		{
			if ( pCaPPTInfos[i]->wResourceId == wResourceID )
			{
				break;
			}
		}
	}
	return i;
}

static void CA_PPT_ResetPPTInfo(ia_byte bIndex, ia_word32 wSamrtcardId)
{
	CA_PPT_T *pTemp = 0,  *pHeader = 0;
	ia_byte bSectorNumber, i;

	if (bIndex >= SC_SLOT_MAX)
	{
		CA_ERROR( ("CA_PPT_ResetPPTInfo: index = %d is too bigger.\n", bIndex) );
		return;

	}

	pHeader = pCaPPTInfos[bIndex];
	while (pCaPPTInfos[bIndex] != 0)
	{
		pTemp = pCaPPTInfos[bIndex];
		pCaPPTInfos[bIndex] = pCaPPTInfos[bIndex]->next;

		if (pTemp->pstPPTRecords != 0)
		{
			OS_DRV_FreeMemory((void *)pTemp->pstPPTRecords);
			pTemp->pstPPTRecords = 0;
		}
	}
	if (0 != pHeader)
	{
		OS_DRV_FreeMemory((void *)pHeader);
		pCaPPTInfos[bIndex] = 0;
	}
	if (INVALID_SC_SMARTCARDID == wSamrtcardId)
		return ;

	bSectorNumber = CA_SCARD_GetSectorNumber(wSamrtcardId);
	if (bSectorNumber == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return ;
	}

	if (bSectorNumber > 0)
	{
		if (pCaPPTInfos[bIndex] == 0)
		{
			pCaPPTInfos[bIndex] = (CA_PPT_T*)ALLOCATE_MEMORY(CA_PPT_T,
				bSectorNumber);
			if (pCaPPTInfos[bIndex] == 0)
			{
				CA_ERROR(("CA_PPT_ResetPPTInfo: Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return ;
			}
			ansi_memset((void*)pCaPPTInfos[bIndex], 0, sizeof(CA_PPT_T)
				*bSectorNumber);
		}
	}

	pTemp = pCaPPTInfos[bIndex];

	for (i = 0; i < bSectorNumber; i++)
	{
		pTemp[i].wResourceId = wSamrtcardId;
		pTemp[i].bSector = 0xFF;
		pTemp[i].bPPTGlobalStatus = 0xFF;
		pTemp[i].bPPTSectorStatus = 0xFF;
		pTemp[i].bPPTRecordCount = 0x00;
		pTemp[i].pstPPTRecords = 0;
		if (i == bSectorNumber - 1)
		{
			pTemp[i].next = 0;
		}
		else
		{
			pTemp[i].next = &pTemp[i + 1];
		}
	}

}

/*
* Description: Initializes the PPT local database.
*
* Parameters : None.
*
* Returns    : None.
*/
void CA_PPT_Initialise(ia_word32 wResourceId)
{
	ia_byte i;
	ia_byte bUsed = SC_SLOT_MAX;


	if (wResourceId == INVALID_SMARTCARDID)
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			CA_PPT_ResetPPTInfo(i, wResourceId);
		}
	}
	else
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			if (pCaPPTInfos[i] != 0)
			{
				if (pCaPPTInfos[i]->wResourceId == wResourceId)
				{
					CA_PPT_ResetPPTInfo(i, wResourceId);
					return ;
				}
			}
			else
			{
				if (bUsed == SC_SLOT_MAX)
					bUsed = i;
			}
		}
		if (bUsed != SC_SLOT_MAX)
		{
			CA_PPT_ResetPPTInfo(bUsed, wResourceId);
		}

	}
}

void CA_PPT_Freememory(void)
{
	CA_PPT_T *pTemp = 0,  *pHeader = 0;
	ia_byte i = 0;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		pHeader = pCaPPTInfos[i];
		while (pCaPPTInfos[i] != 0)
		{
			pTemp = pCaPPTInfos[i];
			pCaPPTInfos[i] = pCaPPTInfos[i]->next;

			if (pTemp->pstPPTRecords != 0)
			{
				FREE_MEMORY(pTemp->pstPPTRecords);
				pTemp->pstPPTRecords = 0;
			}
		}
		if (0 != pHeader)
		{
			FREE_MEMORY(pHeader);
			pCaPPTInfos[i] = 0;
		}
	}

}

CA_QUERY_STATUS_EN CA_PPT_InfoQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_byte bSectorIndex, bSectorNum;
	int i;
	CA_PPT_T *pTemp = 0,  *pHeader = 0;
	ia_word32 wSCardId = 0;
	CA_PPT_T *caPPTInfo;

	CA_PARAM_NOT_USED( wHandle );
	caPPTInfo = ( CA_PPT_T* )pCAClientMsg;
	if (caPPTInfo == 0)
	{
		return CA_QUERY_FAIL;
	}
	wSCardId = caPPTInfo->wResourceId;
	/* check the input parameter smart card id and bsector */
	bSectorNum = CA_SCARD_GetSectorNumber(wSCardId);
	if (bSectorNum == INVALID_SC_SECTOR)
		/* the smartcard id is invalidate */
	{
		return CA_QUERY_FAIL;
	}

	if (caPPTInfo->bSector >= bSectorNum)
	{
		return CA_QUERY_FAIL;
	}

	bSectorIndex = caPPTInfo->bSector;

	if (caPPTInfo->bRequestType == REQUEST_TYPE_QUERY)
	{
		if (CA_SCARD_GetGlobalSmartCardStatus(wSCardId) != STATUS_CARD_IN)
		{
			return CA_QUERY_FAIL;
		}

		CA_DBG_V(("CA_PPT_INFOQueryControl send query\n"));
		return (CA_PPT_InfoSendQuery(wSCardId, caPPTInfo->bSector));
	}
	else
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			pTemp = pCaPPTInfos[i];
			if (pTemp != 0)
			{
				if (pTemp->wResourceId == wSCardId)
					break;
			}
		}
		if (i == SC_SLOT_MAX)
			/* cann't find the resource id */
		{
			return CA_QUERY_FAIL;
		}
		pTemp = 0;
		pHeader = pCaPPTInfos[i];
		while (pHeader != 0)
		{
			if (pHeader->bSector == bSectorIndex)
			{
				pTemp = pHeader;
				break;
			}
			pHeader = pHeader->next;
		}
		if (pTemp == 0)
		{
			return CA_QUERY_FAIL;
		}

		caPPTInfo->bPPTSectorStatus = pTemp->bPPTSectorStatus;
		caPPTInfo->bPPTGlobalStatus = pTemp->bPPTGlobalStatus;
		caPPTInfo->bPPTRecordCount = pTemp->bPPTRecordCount;

		if (caPPTInfo->bPPTRecordCount > 0 && caPPTInfo->pstPPTRecords == 0)
		{
			return CA_QUERY_FAIL;
		}

		for (i = 0; i < caPPTInfo->bPPTRecordCount; i++)
		{
			caPPTInfo->pstPPTRecords[i].bPPTStatus = pTemp->pstPPTRecords[i].bPPTStatus;
			ansi_memcpy((void *)caPPTInfo->pstPPTRecords[i].pbPPTLable, (void *)pTemp->pstPPTRecords[i].pbPPTLable, PPT_LABLE_LEN);
			caPPTInfo->pstPPTRecords[i].wPPTViewTimeLeft = pTemp->pstPPTRecords[i].wPPTViewTimeLeft;
		}
	}

	return CA_QUERY_SUCCESS;
}

void CA_PPT_INFOReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	int bSectorIndex;
	int bPPTRecordV5Count;
	int i;
	ia_word32 wSCardId;

	msg_ppt_info_reply_st stPPTInfo;
	msg_ppt_record_v41_st stPPTRecordV41;
	msg_ppt_record_v5plus_st astPPTRecordV5plus;
	CA_PPT_T *pTemp = 0,  *pHeader = 0;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;

	if (IA_ReadFromMessage(pvMsg, &stPPTInfo, sizeof(msg_ppt_info_reply_st)) ==
		0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}


	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (pCaPPTInfos[i] != 0)
		{
			if (stPPTInfo.wResourceId == pCaPPTInfos[i]->wResourceId)
				break;
		}
	}
	if (i == SC_SLOT_MAX)
		/* cann't find the resource id */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wSCardId = stPPTInfo.wResourceId;

	if (stPPTInfo.bSector >= CA_SCARD_GetSectorNumber(wSCardId))
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	CA_DBG_V(("ePPTGlobalStatus = %d, ePPTSectorStatus = %d\n",
		stPPTInfo.ePPTGlobalStatus, stPPTInfo.ePPTSectorStatus));

	bSectorIndex = stPPTInfo.bSector;

	pHeader = pCaPPTInfos[i];
	while (pHeader != 0)
	{
		if (pHeader->bSector == bSectorIndex)
		{
			pTemp = pHeader;
			break;
		}
		pHeader = pHeader->next;
	}

	if (pTemp == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	pTemp->bPPTSectorStatus = (ia_byte)stPPTInfo.ePPTSectorStatus;
	pTemp->bPPTGlobalStatus = (ia_byte)stPPTInfo.ePPTGlobalStatus;

	if (stPPTInfo.ePPTGlobalStatus == PPT_GS_OK_V41)
		/* && stPPTInfo.ePPTSectorStatus == PPT_SS_OK )*/
	{
		if (stPPTInfo.ePPTSectorStatus == PPT_SS_OK)
		{

			CA_SCARD_SetSectorType(wSCardId, (ia_byte)bSectorIndex, PPT_SUPPORT);

			pTemp->pstPPTRecords = (CA_PPT_RECORD_T*)ALLOCATE_MEMORY
				(CA_PPT_RECORD_T, 1);
			
			if (pTemp->pstPPTRecords == 0)
			{
				CA_ERROR(("CA_PPT_INFOReplyMsg: Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				IA_CloseMessage(pvMsg, IA_FALSE);
				return ;
			}
			IA_ReadFromMessage(pvMsg, &stPPTRecordV41, sizeof
				(msg_ppt_record_v41_st));
			pTemp->pstPPTRecords[0].bPPTStatus = stPPTRecordV41.bPPTStatus;
			pTemp->pstPPTRecords[0].wPPTViewTimeLeft =
				stPPTRecordV41.wViewingTimeLeft;
			ansi_memset((void *)pTemp->pstPPTRecords[0].pbPPTLable, 0, PPT_LABLE_LEN);
			pTemp->bPPTRecordCount = 1;
		}
		else
		{
			pTemp->pstPPTRecords = 0;
			CA_DBG_V((" PPT sector supoort V41 not support PPT %x\n",
				stPPTInfo.ePPTSectorStatus));
		}
	}
	else if (stPPTInfo.ePPTGlobalStatus == PPT_GS_OK_V5PLUS)
		/* &&ePPTSectorStatus == PPT_SS_OK )*/
	{
		if (stPPTInfo.ePPTSectorStatus == PPT_SS_OK)
		{
			CA_SCARD_SetSectorType(wSCardId, (ia_byte)bSectorIndex, PPT_SUPPORT);

			IA_ReadFromMessage(pvMsg, &bPPTRecordV5Count, sizeof(ia_byte));

			if (bPPTRecordV5Count > 0)
			{
				pTemp->pstPPTRecords = (CA_PPT_RECORD_T*)ALLOCATE_MEMORY
					(CA_PPT_RECORD_T, bPPTRecordV5Count);
				
				if (pTemp->pstPPTRecords == 0)
				{
					CA_ERROR(("CA_PPT_INFOReplyMsg: Memory allocate failed.\n"));
					CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
					IA_CloseMessage(pvMsg, IA_FALSE);
					return ;
				}
			}
			else
			{
				pTemp->pstPPTRecords = 0;
			}

			pTemp->bPPTRecordCount = (ia_byte)bPPTRecordV5Count;

			for (i = 0; i < bPPTRecordV5Count; i++)
			{
				IA_ReadFromMessage(pvMsg, &astPPTRecordV5plus, sizeof
					(msg_ppt_record_v5plus_st));
				pTemp->pstPPTRecords[i].bPPTStatus =
					astPPTRecordV5plus.bPPTStatus;
				pTemp->pstPPTRecords[i].wPPTViewTimeLeft =
					astPPTRecordV5plus.wViewingTimeLeft;
				ansi_memcpy(pTemp->pstPPTRecords[i].pbPPTLable,
					astPPTRecordV5plus.abPPTLabel, PPT_LABLE_LEN);
			}
		}
		else
		{
			pTemp->pstPPTRecords = 0;
			CA_DBG_V((" PPT sector supoort V5 not support PPT %x\n",
				stPPTInfo.ePPTSectorStatus));
		}

	}
	else
	{
		CA_DBG_V((
			" PPT sector golbal status is NOT support , Global status %x\n",
			stPPTInfo.ePPTGlobalStatus));
	}

	/* ECT3.3 enhancement: streamline of wHandle */
	CA_CLIENT_Notify( wSCardId , CA_QC_PPT_INFO ,
		(ia_word32)(((ia_word32)bSectorIndex << 8) | (ia_word32)pTemp->bPPTRecordCount));


	IA_CloseMessage(pvMsg, IA_FALSE);

	/* Fix SCR#26646: ayang 07/12/04 */
	if (++bSectorIndex == CA_SCARD_GetSectorNumber(wSCardId))
	{
		CA_Set_ScellMsgFlag( 1 );
	}
}


CA_QUERY_STATUS_EN CA_PPT_InfoSendQuery(ia_word32 wResourceId, ia_byte
										bSectorIndex)
{
	msg_ppt_info_query_st pPPTMsg;
	ia_msg_handle pvMsg;

	pvMsg = IA_OpenOutputMessage(sizeof(msg_ppt_info_query_st), sizeof
			(msg_ppt_info_query_st));

	pPPTMsg.wResourceId = wResourceId;
	pPPTMsg.bSector = bSectorIndex;
	
	IA_WriteToMessage(pvMsg, &pPPTMsg, sizeof(msg_ppt_info_query_st));

	if (SCELL_Message(MSG_PPT_INFO_QUERY, 0, IA_GetOutputMessageLength(pvMsg),
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
		CA_ERROR(("CA_RF_ConfigQueryControl: failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
		return CA_QUERY_FAIL;
	}
}

void CA_PPT_SetSectorBySectorIndex(ia_word32 wResourceId, ia_byte bSector)
{
	CA_PPT_T *pHeader = 0;
	/* jyou: 3.4.2 SCR#25311 */
	ia_word16 i;

	i = FindSmartCardIndex( wResourceId );
	if ( i == SC_SLOT_MAX )
	{
		CA_ERROR(("[CA_PPT_SetSectorBySectorIndex]: Can't locate resource ID %d.\n", wResourceId));
		return;
	}
	/* Get the pointer to the local storage of the resource ID */
	pHeader = pCaPPTInfos[i];

	while (pHeader != 0)
	{
		if (pHeader->bSector == 0xFF)
		{
			pHeader->bSector = bSector;
			break;
		}
		pHeader = pHeader->next;
	}
	return ;
}
