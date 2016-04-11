/**
* $Id: ca_product.c,v 1.32 2008/07/18 06:08:28 sren Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* The product module of Embedded CA Task.
**/

/******************************************************************************
* FILE          : ca_product.C                                                  *
*                                                                            *
* DESCRIPTION   :
*                                                                            *
* NOTES         :                                                            *
*                                                                            *
* STATUS/HISTORY:                                                            *
* Author          : Melissa Li                                                  *
* Status          : $Revision: 1.32 $                                        *
* History         :                                                           *
*                                                                            *
******************************************************************************/

/******************************** Header Files ********************************/
/* #include <string.h> */
#include <irca/pubh/s3_ansi_functions.h>
#include <irca/pubh/s3_osdrv.h>
#include <irca/pubh/s3_message_helper.h>
#include <irca/pubh/s3_smartcard.h>
#include <irca/pubh/s3_prod_if_msg.h>
#include <irca/pubh/s3_cadrv.h>
#include <irca/ca_task.h>
#include "ca_product.h"
#include "ca_scard.h"
#include "ca_error.h"

#define CA_ANY_PRODUCK_BLOCK    0x80000000

#if 0
#ifdef INCL_PRODUCT_BLOCKING
typedef enum
{
	BLOCK_ALL, BLOCK_FTA, BLOCK_NONE
} BLOCK_TYPE;
#endif /* end INCL_PRODUCT_BLOCKING */
#endif

/******************************* Local Variables ******************************/
static CA_EXTENDED_PRODUCT_LIST_T astCaExProductList[SC_SLOT_MAX];
static CA_QUERY_PRODUCT_CHECK_T astProductCheck[SC_SLOT_MAX];

#ifdef INCL_PRODUCT_BLOCKING
static CA_ANY_PRODUCT_CHECK_T astAnyProductBlock[SC_SLOT_MAX];
static CA_PRODUCT_BLOCK_T astProductBlock[SC_SLOT_MAX];
#endif

/************************* Local Function Declarations ************************/
/* jyou: 3.4.2 multiple smart cards revision */
void ResetExtendedProductList( ia_byte index );

/************************ Function Implementations ****************************/
/* jyou: the following code are merged from mli 3.3.11 */
#ifdef INCL_PRODUCT_BLOCKING
/* jyou: 3.4.2 SCR#25328 */
static ia_word16 FindSmartCardIndex( ia_word32 wResourceID )
{
	ia_word16 i;

	for ( i = 0 ; i < SC_SLOT_MAX ; i++ )
	{
		/* jyou: Since all local storage shares the same ResourceID so just use astProductCheck */
		/*  for astProductBlock and astAnyProductBlock */
		/* This style of design shall be avoid in the future, a generic local storage with a */
		/* single placeholder of the wResourceId shall be used instead. Refer to */
		/* CA_LOCAL_SMARTCARD_INFO as a sample */
		if ( astProductCheck[i].wResourceId == wResourceID )
		{
			break;
		}
	}
	return i;
}

void CA_PRODUCT_BLOCK_Init(ia_word32 wSmartCardId)
{
	ia_sint16 i;
	/* jyou: 3.4.2 SCR#25341 series. */
	/* Removed two lines of ansi_memset. This implementation does not support multiple smart cards */

	CA_PARAM_NOT_USED(astrSCellMsgDisplay); /* to supress warning */
	if (wSmartCardId == INVALID_SMARTCARDID)
	{/* Initialize called by CA_Task_init */

		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			astProductCheck[i].wResourceId = wSmartCardId;
			astProductCheck[i].bGracePeriod = 0;
			astProductCheck[i].bProductCount = 0;
			astProductCheck[i].fKeepTrack = IA_FALSE;
			astAnyProductBlock[i].wResourceId = wSmartCardId;
			/* jyou: 3.4.2 SCR#25328 */
			astProductBlock[i].wResourceId = wSmartCardId;
				}
	}
	else
	{/* Initialize called by smart card status change */
		/* jyou: 3.4.2 renovation for removing hard code of smart card resouce ID */
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			if (astProductCheck[i].wResourceId == wSmartCardId)
			{
				astProductCheck[i].bGracePeriod = 0;
				astProductCheck[i].bProductCount = 0;
				astProductCheck[i].fKeepTrack = IA_FALSE;
				astAnyProductBlock[i].wResourceId = wSmartCardId;
				/* jyou: 3.4.2 SCR#25328 */
				astProductBlock[i].wResourceId = wSmartCardId;
				return ;
			}
		}
		/* jyou: in case the card does not already exist in the local storage, add it */
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			if (astProductCheck[i].wResourceId == INVALID_SMARTCARDID)
			{
				astProductCheck[i].wResourceId = wSmartCardId;
				astProductCheck[i].bGracePeriod = 0;
				astProductCheck[i].bProductCount = 0;
				astProductCheck[i].fKeepTrack = IA_FALSE;
				astAnyProductBlock[i].wResourceId = wSmartCardId;
				/* jyou: 3.4.2 SCR#25328 */
				astProductBlock[i].wResourceId = wSmartCardId;
				return ;
			}
		}
	}
}
#endif /*INCL_PRODUCT_BLOCKING*/

/* mli: 3.3.11 product check */
#ifdef INCL_PRODUCT_BLOCKING
CA_QUERY_STATUS_EN CA_PRODUCT_CheckSendQuery(ia_bool fPVRProd, CA_QUERY_PRODUCT_CHECK_T *pQueryProductCheck)
{
	ia_msg_handle pvMsg;
	msg_product_check_query_st stProductCheck;
	msg_product_check_query_product_st stProduct;
	 /* ia_byte sectorNum; */
	ia_byte totalProductNum = 0;
	ia_word32 wResId;

	ia_word16 wLength;
	ia_sint16 i;

	if (pQueryProductCheck == 0)
	{
		return CA_QUERY_FAIL;
	}
	if ((pQueryProductCheck->pstQuerySector == 0) && (pQueryProductCheck->bProductCount > 0))
	{
		return CA_QUERY_FAIL;
	}

	if (pQueryProductCheck->bRequestType == REQUEST_TYPE_QUERY)
	{
		totalProductNum = pQueryProductCheck->bProductCount;
		wLength = (ia_word16)(sizeof(msg_product_check_query_st) + totalProductNum * sizeof
		(msg_product_check_query_product_st));

		 pvMsg = IA_OpenOutputMessage(wLength, sizeof (msg_product_check_query_product_st));
		 if (pvMsg == 0)
		 {
			return CA_QUERY_FAIL;
		 }

		ansi_memset((void *)&stProductCheck, 0, sizeof(msg_product_check_query_product_st));
		stProductCheck.wResourceId = pQueryProductCheck->wResourceId;
		stProductCheck.bGracePeriod = pQueryProductCheck->bGracePeriod;
		stProductCheck.fKeepTrack = pQueryProductCheck->fKeepTrack;
		stProductCheck.bProductCount = totalProductNum;

		IA_WriteToMessage(pvMsg, &stProductCheck, sizeof
			(msg_product_check_query_st));

		for (i = 0; i < totalProductNum; i++)
		{
			if (pQueryProductCheck->pstQuerySector == 0)
			{
				CA_ERROR(("Sending MSG_PRODUCT_CHECK_QUERY failed. sector memery error\n"));
				IA_CloseMessage(pvMsg, IA_TRUE);
				return CA_QUERY_FAIL;
			}

			ansi_memset((void *)&stProduct, 0, sizeof(msg_product_check_query_product_st));
			stProduct.bSector = pQueryProductCheck->pstQuerySector[i].bSector;
			stProduct.fAnySector = pQueryProductCheck->pstQuerySector[i].fAnySector;
			stProduct.wProduct = pQueryProductCheck->pstQuerySector[i].wProduct;

			IA_WriteToMessage(pvMsg, &stProduct, sizeof (msg_product_check_query_product_st));

		}

		if (SCELL_Message(MSG_PRODUCT_CHECK_QUERY, 0, IA_GetOutputMessageLength
			(pvMsg), IA_GetOutputMessage(pvMsg)) == IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR(("Sending MSG_PRODUCT_CHECK_QUERY failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}

	if (pQueryProductCheck->bRequestType == REQUEST_TYPE_REPLY)
	{
		/* jyou: 3.4.2 SCR#25328, a temporary solution, this piece of code needs to be reviewed */
		wResId = FindSmartCardIndex( pQueryProductCheck->wResourceId );
		if (wResId >= SC_SLOT_MAX)
		{
			return CA_QUERY_FAIL;
		}
		if (fPVRProd == IA_TRUE)
		{
			totalProductNum = pQueryProductCheck->bProductCount = astProductCheck[wResId].bProductCount;

			for (i = 0; i < totalProductNum; i++)
			{
				pQueryProductCheck->pstQuerySector[i].bSector = astProductCheck[wResId].pstQuerySector[i].bSector;
				pQueryProductCheck->pstQuerySector[i].fSectorValid = astProductCheck[wResId].pstQuerySector[i].fSectorValid;
				pQueryProductCheck->pstQuerySector[i].fProductValid = astProductCheck[wResId].pstQuerySector[i].fProductValid;
				pQueryProductCheck->pstQuerySector[i].wProduct = astProductCheck[wResId].pstQuerySector[i].wProduct;
			}
		}
		else
		{
			for (i = 0; i < pQueryProductCheck->bProductCount; i++)
			{
				pQueryProductCheck->pstQuerySector[i].bSector = astProductBlock[wResId].astProduct[i].bSector;
				pQueryProductCheck->pstQuerySector[i].fSectorValid = astProductBlock[wResId].astProduct[i].fSectorValid;
				pQueryProductCheck->pstQuerySector[i].fProductValid = astProductBlock[wResId].astProduct[i].fProductValid;
				pQueryProductCheck->pstQuerySector[i].wProduct = astProductBlock[wResId].astProduct[i].wProduct;
			}
		}
	}

	return CA_QUERY_SUCCESS;
}
#endif/*INCL_PRODUCT_BLOCKING*/

/* mli: 3.3.11 ??? quoted by jyou for passing compiling, req is yet to identify */
#ifdef INCL_PRODUCT_BLOCKING
/* PVR Product Check and FTA Product */
void CA_PRODUCT_CheckReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;

	msg_product_check_reply_st stProductReply;
	msg_product_check_reply_product_st stProdcut, *pstTempProd = 0;
	ia_word16 i, j;
	ia_word32 wResId;
	ia_bool fPVRProductCheck = IA_FALSE;
	ia_word32 wPVRProductCount = 0;
	ia_word32 wFTAProductCount = 0;
	ia_word16 m = 0, n = 0;

	/*allocate the memeory of message and get product check message*/
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stProductReply, sizeof
		(msg_product_check_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	wResId = stProductReply.wResourceId;
	pstTempProd = (msg_product_check_reply_product_st *) OS_DRV_AllocateMemory((unsigned long)
		sizeof(msg_product_check_reply_product_st)* (unsigned long)stProductReply.bProductCount);
	/* Fixed SCR#27163 */
	if (pstTempProd == 0 && stProductReply.bProductCount > 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		CA_ERROR(("CA_PRODUCT_CheckReplyMsg: Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ;
	}

	for (j = 0; j< stProductReply.bProductCount; j++)
	{
		ansi_memset((void *)&stProdcut, 0, sizeof(msg_product_check_query_product_st));
		IA_ReadFromMessage(pvMsg, &stProdcut, sizeof
			(msg_product_check_reply_product_st));
		ansi_memcpy((void *)&pstTempProd[j], (void *)&stProdcut, sizeof(msg_product_check_reply_product_st));
		/* check the product reply for PVR product check or Product blocking */

		/* sren, SCR#27165, tag 3_6_12:Application only get the PVR product check reply when there have normal and PVR check.*/ 
		if ( (stProdcut.wProduct >= 0xffe0)&&(stProdcut.wProduct <= 0xffef) )  /* PVR product */
		{
			fPVRProductCheck = IA_TRUE;
			wPVRProductCount++;
		}
		else
		{
			wFTAProductCount++;
		}
	}

	if (fPVRProductCheck == IA_TRUE)
	{
		i = CA_PRODUCT_CHECK_ListFreeMemory(wResId);
		if (i == SC_SLOT_MAX)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return;
		}
		astProductCheck[i].bProductCount = wPVRProductCount;
		if ( astProductCheck[i].bProductCount > 0 )
		{
			astProductCheck[i].pstQuerySector = OS_DRV_AllocateMemory(
				(unsigned long)sizeof(CA_QUERY_SECTOR_PRODUCT_T)*(unsigned long)wPVRProductCount);
			if ( astProductCheck[i].pstQuerySector == 0 )
			{
				IA_CloseMessage(pvMsg, IA_FALSE);
				CA_ERROR(("CA_PRODUCT_CheckReplyMsg: Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return;
			}
		}

		/*sren, SCR#27165, tag 3_6_12: changed for store the PVR product correctly*/
		for (j = 0; j < wPVRProductCount; j++)
		{
			for( ; m<stProductReply.bProductCount; m++)
			{
				if ( (pstTempProd[m].wProduct >= 0xffe0)&&(pstTempProd[m].wProduct <= 0xffef) )
				{
					astProductCheck[i].pstQuerySector[j].bSector = pstTempProd[m].bSector;
					astProductCheck[i].pstQuerySector[j].fSectorValid = pstTempProd[m].fSectorValid;
					astProductCheck[i].pstQuerySector[j].fProductValid = pstTempProd[m].fProductValid;
					astProductCheck[i].pstQuerySector[j].wProduct = pstTempProd[m].wProduct;
					m++;	/*Check the next PVR Product*/
					break;	/*This will get the PVR product*/
				}
			}
		}

		/* monitor PVR products, if the status is changed, it will send the CA_QC_PVR_PRODUCT_BLOCK notification */
		CA_CLIENT_Notify(stProductReply.wResourceId, CA_QC_PVR_PRODUCT_BLOCK, wPVRProductCount);

	}

	/*Continue check for normal products*/
	if( wFTAProductCount > 0 )
	{
		/* hqian: 3.4.2 SCR#25328  changed j++ to i++ */
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			if (astProductBlock[i].wResourceId  ==  wResId)
			{
				break;
			}
		}

		if (i == SC_SLOT_MAX)
		{
			IA_CloseMessage(pvMsg, IA_FALSE);
			return ;
		}

		if (wFTAProductCount > MAX_FTA_PRODUCTS)
		{
			wFTAProductCount = MAX_FTA_PRODUCTS;
		}

		astProductBlock[i].bNumEntries = wFTAProductCount;
		for (j = 0; j< wFTAProductCount; j++)
		{
			for( ; n<stProductReply.bProductCount; n++)
			{
				if ( (pstTempProd[n].wProduct <= 0xffe0)||(pstTempProd[n].wProduct >= 0xffef) )
				{
					astProductBlock[i].astProduct[j].bSector = pstTempProd[n].bSector;
					astProductBlock[i].astProduct[j].wProduct = pstTempProd[n].wProduct;
					astProductBlock[i].astProduct[j].fSectorValid = pstTempProd[n].fSectorValid;
					astProductBlock[i].astProduct[j].fProductValid = pstTempProd[n].fProductValid;
					n++;	/*Check the next FTA Product*/
					break;	/*This will get the FTA product*/
				}
			}
		}
		CA_CLIENT_Notify(stProductReply.wResourceId, CA_QC_PRODUCT_BLOCK, wFTAProductCount);
	}
	
	if (pstTempProd != 0)
	{
		OS_DRV_FreeMemory(pstTempProd);
		pstTempProd = 0;
	}
	
	IA_CloseMessage(pvMsg, IA_FALSE);
}

/* for any product */
CA_QUERY_STATUS_EN CA_ANY_PRODUCT_SendQuery(CA_ANY_PRODUCT_CHECK_T *proQuery)
{
	ia_msg_handle pvMsg;
	msg_any_product_query_st stAnyProduct;
	ia_byte sectorCnt;
	ia_byte abSectors;
	ia_sint16 i, j;
	ia_word16 wLength;
	ia_word32 wSCardId;

	if (proQuery != 0)
	{
		sectorCnt = proQuery->bSectorCount;
	}
	else
	{
		return CA_QUERY_FAIL;
	}
	wSCardId = proQuery->wResourceId;
	if (proQuery->bRequestType == REQUEST_TYPE_QUERY)
	{
		if (CA_SCARD_GetGlobalSmartCardStatus(wSCardId) != STATUS_CARD_IN)
		{
				return CA_QUERY_FAIL;
		}
		wLength = (ia_word16)(sizeof(msg_any_product_query_st) + sectorCnt * sizeof(ia_byte));

		pvMsg = IA_OpenOutputMessage(wLength, sizeof(ia_byte));

		if (pvMsg != 0)
		{
			stAnyProduct.wResourceId = wSCardId;
			stAnyProduct.bGracePeriod = 0;
			stAnyProduct.fKeepTrack = IA_TRUE;
			stAnyProduct.fAnySector = IA_FALSE;
			stAnyProduct.bSectorCount = sectorCnt;

			IA_WriteToMessage(pvMsg, &stAnyProduct, sizeof
				(msg_any_product_query_st));

			for (i = 0; i < sectorCnt; i++)
			{
				abSectors = proQuery->sectorIndex[i];
				IA_WriteToMessage(pvMsg, &abSectors, sizeof(ia_byte));
			}

			if (SCELL_Message(MSG_ANY_PRODUCT_QUERY, 0,
				IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg))
				== IA_SUCCESS)
			{
				/* The buffer will be freed by the receiver. */
				IA_CloseMessage(pvMsg, IA_FALSE);
			}
			else
			{
				/* Message not sent. We must free the buffer. */
				IA_CloseMessage(pvMsg, IA_TRUE);
				CA_ERROR(("Sending MSG_ANY_PRODUCT_QUERY failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
				return CA_QUERY_FAIL;
			}
		}
		else
		{
			CA_ERROR(("Malloc error.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return CA_QUERY_FAIL;
		}
	}
	if (proQuery->bRequestType == REQUEST_TYPE_REPLY)
	{
		wSCardId = proQuery->wResourceId;
		for ( j = 0; j < SC_SLOT_MAX; j++ )
		{
			if ( astAnyProductBlock[j].wResourceId == wSCardId )
			{
				break;
			}
		}

		if ( j == SC_SLOT_MAX )
		{
			return CA_QUERY_FAIL;
		}

		sectorCnt = astAnyProductBlock[j].bSectorCount;

		for (i = 0; i < sectorCnt; i++)
		{
			proQuery->stQuerySector[i].bSector = astAnyProductBlock[j].stQuerySector[i].bSector;
			proQuery->stQuerySector[i].fSectorValid = astAnyProductBlock[j].stQuerySector[i].fSectorValid;
			proQuery->stQuerySector[i].fProductValid = astAnyProductBlock[j].stQuerySector[i].fProductValid;
			proQuery->stQuerySector[i].wProduct = astAnyProductBlock[j].stQuerySector[i].wProduct;
		}
	}

	return CA_QUERY_SUCCESS;
}

CA_QUERY_STATUS_EN CA_STOP_ANY_PRODUCT_SendQuery(ia_word32 wResourceId)
{
	ia_msg_handle pvMsg;
	msg_any_product_query_st stAnyProduct;

	/*  ia_byte                             sectorCnt; */ /*=
															CA_SCARD_GetSectorNumber( );*/
	/*  sectorCnt = CA_SCARD_GetSectorNumber(wResourceId);
	CA_DBG_V(("<= MSG_ANY_STOP_PRODUCT_QUERY, sectorCnt = %d\n", sectorCnt));
	*/
	pvMsg = IA_OpenOutputMessage(sizeof(msg_any_product_query_st), sizeof
		(msg_any_product_query_st));

	if (pvMsg != 0)
	{
		stAnyProduct.wResourceId = wResourceId;
		stAnyProduct.bGracePeriod = 0;
		stAnyProduct.fKeepTrack = IA_FALSE;
		stAnyProduct.fAnySector = IA_FALSE;
		stAnyProduct.bSectorCount = 0;

		IA_WriteToMessage(pvMsg, &stAnyProduct, sizeof
			(msg_any_product_query_st));

		if (SCELL_Message(MSG_ANY_PRODUCT_QUERY, 0,
			IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg))
			== IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR(("Sending MSG_ANY_PRODUCT_QUERY failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else
	{
		CA_ERROR(("Malloc error.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return CA_QUERY_FAIL;
	}

	return CA_QUERY_SUCCESS;
}

void CA_ANY_PRODUCT_ReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;

	msg_any_product_reply_st stAnyProductReply;
	msg_any_product_reply_sector_st stAnyProdcut;
	ia_sint16 i, j;

	/*allocate the memeory of message and get any product message*/
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg
		->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stAnyProductReply, sizeof
		(msg_any_product_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (j = 0; j < SC_SLOT_MAX; j++)
	{
		if (astAnyProductBlock[j].wResourceId ==
			stAnyProductReply.wResourceId)
		{
			break;
		}
	}

	if (j == SC_SLOT_MAX)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	astAnyProductBlock[j].bSectorCount = stAnyProductReply.bSectorCount;
	for (i = 0; i < stAnyProductReply.bSectorCount; i++)
	{
		IA_ReadFromMessage(pvMsg, &stAnyProdcut, sizeof
			(msg_any_product_reply_sector_st));
		CA_DBG_V((
			"bSector = %d, fSectorValid = %d, fProductValid = %d, wProduct = %x\n", stAnyProdcut.bSector, stAnyProdcut.fSectorValid, stAnyProdcut.fProductValid, stAnyProdcut.wProduct));
		astAnyProductBlock[j].stQuerySector[i].wProduct = stAnyProdcut.wProduct;
		astAnyProductBlock[j].stQuerySector[i].bSector = stAnyProdcut.bSector;
		astAnyProductBlock[j].stQuerySector[i].fProductValid = stAnyProdcut.fProductValid;
		astAnyProductBlock[j].stQuerySector[i].fSectorValid = stAnyProdcut.fSectorValid;
	}

	IA_CloseMessage(pvMsg, IA_FALSE);
	CA_CLIENT_Notify(stAnyProductReply.wResourceId, CA_QC_PRODUCT_BLOCK,
		(ia_word32)stAnyProductReply.bSectorCount|CA_ANY_PRODUCK_BLOCK);

}
#endif /* INCL_PRODUCT_BLOCKING*/


CA_QUERY_STATUS_EN CA_STOP_PRODUCT_ListSendQuery(ia_word32 wResourceId)
{
	ia_msg_handle pvMsg;
	msg_product_list_query_st stProductList;
	ia_byte sectorCnt = 0;

	/* check the input parameter smartcardId */
	sectorCnt = CA_SCARD_GetSectorNumber(wResourceId);
	if (sectorCnt == INVALID_SC_SECTOR)
		return CA_QUERY_FAIL;
	CA_DBG_V(("<= MSG_PRODUCT_LIST_QUERY, sectorCnt = %d\n", sectorCnt));

	pvMsg = IA_OpenOutputMessage(sizeof(msg_product_list_query_st), sizeof
		(msg_product_list_query_st));

	if (pvMsg != 0)
	{
		stProductList.wResourceId = wResourceId;
		stProductList.bGracePeriod = 0;
		stProductList.fKeepTrack = IA_TRUE;
		stProductList.fAnySector = IA_FALSE;
		stProductList.bSectorCount = sectorCnt;

		IA_WriteToMessage(pvMsg, &stProductList, sizeof
			(msg_product_list_query_st));

		if (SCELL_Message(MSG_PRODUCT_LIST_QUERY, 0, IA_GetOutputMessageLength
			(pvMsg), IA_GetOutputMessage(pvMsg)) == IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR(("Sending MSG_PRODUCT_LIST_QUERY failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else
	{
		CA_ERROR(("Malloc error.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return CA_QUERY_FAIL;
	}

	return CA_QUERY_SUCCESS;
}

void CA_PRODUCT_ListReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;

	msg_product_list_reply_st stProductListReply;
	msg_product_list_sector_st stProdcutSector;
	ia_word16 awProducts;
	ia_sint16 i, j;

	/*allocate the memeory of message and get any product message*/
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stProductListReply, sizeof
		(msg_product_list_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (i = 0; i < stProductListReply.bSectorCount; i++)
	{
		IA_ReadFromMessage(pvMsg, &stProdcutSector, sizeof
			(msg_product_list_sector_st));
		CA_DBG_V(("bSector = %d, fSectorValid = %d, product counter = %d\n",
			stProdcutSector.bSector, stProdcutSector.fSectorValid,
			stProdcutSector.bProductCount));

		for (j = 0; j < stProdcutSector.bProductCount; j++)
		{
			IA_ReadFromMessage(pvMsg, &awProducts, sizeof(ia_word16));
			CA_DBG_V(("awProducts = %2x\n", awProducts));
		}
	}

	IA_CloseMessage(pvMsg, IA_FALSE);
}

/* for extended product list*/
/*
* jyou: 3.4.2
* Called By: InitialiseAllModules, ParseSmartcardStatusMsg
*
*/
void CA_EXTENDED_PRODUCT_ListInitialise(ia_word32 wResourceId)
{
	ia_byte i, j;

	/* jyou: SCR#25095 */
	if (wResourceId == INVALID_SMARTCARDID)
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			/* jyou: free allocated memory for the current smart card */
			CA_EXTENDED_PRODUCT_ListFreeMemory( astCaExProductList[i].wResourceId );
			/* set the resource ID to invalide */
			astCaExProductList[i].wResourceId = wResourceId;
			ResetExtendedProductList( i );
		}
	}
	else
	{
		/* resource ID is valid and locate the smart card */
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			if ( astCaExProductList[i].wResourceId == wResourceId )
			{
				break;
			}
		}
		if (i == SC_SLOT_MAX)
		{
			/* no matching smart card found so add a new one */
			/* jyou: 3.4.2 multiple smart cards fix */
			/* search for the first unused index */
			for ( j = 0 ; j < SC_SLOT_MAX ; j++ )
			{
				if ( astCaExProductList[j].wResourceId == INVALID_SMARTCARDID )
				{
					break;
				}
			}
			if ( j == SC_SLOT_MAX )
			{
				CA_DBG_V(("[ca_product]: Failed to add ResourceID %d\n", wResourceId));
			}
			else
			{
				astCaExProductList[j].wResourceId = wResourceId;
			}
		}
		else
		{
			/* a matching one found so need to free previously allocated memory */
			CA_EXTENDED_PRODUCT_ListFreeMemory( wResourceId );
		}
	}
	return;
}


void ResetExtendedProductList( ia_byte index )
{
	astCaExProductList[index].bSectorCount = 0;
	astCaExProductList[index].pstSectorProducts = 0;
	astCaExProductList[index].wMaxSizeSectorProducts = 0;
	astCaExProductList[index].wOrgSizeSectorProducts = 0;
	astCaExProductList[index].fExpiredProductsExist = IA_FALSE;
}

void CA_PRODUCT_CheckInitialise(ia_word32 wResourceId)
{
	ia_byte i;

	if (wResourceId ==  INVALID_SMARTCARDID)
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			astProductCheck[i].wResourceId = wResourceId;
			astProductCheck[i].bGracePeriod = 0;
			astProductCheck[i].bProductCount = 0;
			astProductCheck[i].fKeepTrack = IA_FALSE;
			if (astProductCheck[i].pstQuerySector != 0)
			{
				OS_DRV_FreeMemory(astProductCheck[i].pstQuerySector);
				astProductCheck[i].pstQuerySector = 0;
			}
		}
	}
	else
	{
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			if (astProductCheck[i].wResourceId == INVALID_SMARTCARDID ||
				astProductCheck[i].wResourceId == wResourceId)
			{
				astProductCheck[i].wResourceId = wResourceId;
				astProductCheck[i].bGracePeriod = 0;
				astProductCheck[i].bProductCount = 0;
				astProductCheck[i].fKeepTrack = IA_FALSE;
				if (astProductCheck[i].pstQuerySector != 0)
				{
					OS_DRV_FreeMemory(astProductCheck[i].pstQuerySector);
					astProductCheck[i].pstQuerySector = 0;
				}
				return ;
			}
		}

	}
}

void CA_EXTENDED_PRODUCT_ListFreeMemory(ia_word32 wResourceId)
{
	ia_byte i, j;
	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (astCaExProductList[i].wResourceId == wResourceId)
		{
			break;
		}
	}
	if (i == SC_SLOT_MAX)
	{
		return ;
	}
	if (astCaExProductList[i].pstSectorProducts != 0)
	{
		for (j = 0; j < astCaExProductList[i].bSectorCount; j++)
		{
			if (astCaExProductList[i].pstSectorProducts[j].pstExtendProducts !=	0)
			{
				CA_DBG_V(("<ca_product> free extended sector 0x6: sector %d: address %0#10x\n", j, astCaExProductList[i].pstSectorProducts[j].pstExtendProducts));
				FREE_MEMORY(astCaExProductList[i].pstSectorProducts[j].pstExtendProducts);
 				astCaExProductList[i].pstSectorProducts[j].pstExtendProducts = 0;
			}
		}

		CA_DBG_V(("<ca_product> free extended product 0x40: address %0#10x\n", astCaExProductList[i].pstSectorProducts));
		FREE_MEMORY(astCaExProductList[i].pstSectorProducts);
		astCaExProductList[i].pstSectorProducts = 0;
	}
	ResetExtendedProductList( i );
	return;
}

ia_byte CA_PRODUCT_CHECK_ListFreeMemory(ia_word32 wResourceId)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (astProductCheck[i].wResourceId == wResourceId)
		{
			break;
		}
	}
	if (i == SC_SLOT_MAX)
	{
		return i;
	}
	if (astProductCheck[i].pstQuerySector!= 0)
	{
		OS_DRV_FreeMemory(astProductCheck[i].pstQuerySector);
		astProductCheck[i].pstQuerySector = 0;
	}

	return i;
}

CA_QUERY_STATUS_EN CA_EXTENDED_PRODUCT_ListSendQuery(CA_EPL_QUERY_T *eplQuery)
{
	ia_msg_handle pvMsg;
	ia_byte sectorCnt = 0, bSectorNum;
	ia_byte bSectorID;
	ia_sint16 i;
	msg_extended_product_list_query_st stExProductList;
	ia_word16 wLength;
	ia_word32 wResourceId = 0;
	CA_LOCAL_SMARTCARD_INFO_T *pstSmartCard;

	CA_DBG_V(("<= MSG_EXTENDED_PRODUCT_LIST_QUERY\n"));

	if (eplQuery != 0)
	{
		sectorCnt = eplQuery->sectorCnt;
	}
	else
	{
		return CA_QUERY_FAIL;
	}
	wResourceId = eplQuery->wResourceId;

	/* jyou: 3.4.3 SCR#25649 2007-03-05 start */
	pstSmartCard = CA_LOCAL_FindSmartCard( eplQuery->wResourceId );
	if ( pstSmartCard == 0 )
	{
		CA_ERROR( ("Smart Card [%d] not available.\n", eplQuery->wResourceId) );
		return CA_QUERY_FAIL;
	}
	bSectorNum = pstSmartCard->bSectorCount; /* the actual sector number in card */
	if ( sectorCnt < bSectorNum && sectorCnt > 0 )
	{ /* a valid sector count is input */
		stExProductList.fAnySector = IA_FALSE;
	}
	else
	{ /* an invalid one */
		stExProductList.fAnySector = IA_TRUE;
		/* If fAnySector set to IA_TRUE, The list of sectors is then ignored and must be empty. */
		sectorCnt = 0;
	}
	/* jyou: 3.4.3 SCR#25649 2007-03-05 end */

	wLength = (ia_word16)(sizeof(msg_extended_product_list_query_st) + sectorCnt * sizeof(ia_byte));
	pvMsg = IA_OpenOutputMessage(wLength, sizeof(ia_byte));

	if (pvMsg != 0)
	{
		stExProductList.wResourceId = eplQuery->wResourceId;
		stExProductList.bSectorCount = sectorCnt;

		IA_WriteToMessage(pvMsg, &stExProductList, sizeof(msg_extended_product_list_query_st));

		for (i = 0; i < sectorCnt; i++)
		{
			bSectorID = eplQuery->sectorIndex[i];
			/* jyou: 3.4.3.6 SCR#26045 2007-06-01 */
			if ( bSectorID == INVALID_SC_SECTOR )
			{
				CA_ERROR(("Invalid sector number.\n"));
				IA_CloseMessage(pvMsg, IA_TRUE);
				return CA_QUERY_FAIL;
			}

			IA_WriteToMessage(pvMsg, &bSectorID, sizeof(ia_byte));
		}

		/* jyou: 3.4.2 multiple smart cards fix */
		if (SCELL_Message(MSG_EXTENDED_PRODUCT_LIST_QUERY, (ia_word16)wResourceId,
			IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
			IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR(("Sending MSG_EXTENDED_PRODUCT_LIST_QUERY failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else
	{
		CA_ERROR(("Malloc error.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return CA_QUERY_FAIL;
	}

	CA_DBG_V(("Return from MSG_EXTENDED_PRODUCT_LIST_QUERY\n"));
	return CA_QUERY_SUCCESS;
} /* CA_EXTENDED_PRODUCT_ListSendQuery */

void CA_PRODUCT_EXTENDED_PRODUCT_ListReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;

	msg_extended_product_list_reply_st stExProductListReply;
	msg_extended_product_list_sector_st stExProdcutSector;
	msg_extended_product_list_product_st stExProducts;
	ia_sint16 i, j, m;

	/*allocate the memeory of message and get extended product list message*/
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stExProductListReply, sizeof
		(msg_extended_product_list_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (m = 0; m < SC_SLOT_MAX; m++)
	{
		if (astCaExProductList[m].wResourceId ==
			stExProductListReply.wResourceId)
			break;
	}
	if (m == SC_SLOT_MAX)
		/* cann't find the resource id */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}


	CA_EXTENDED_PRODUCT_ListFreeMemory(stExProductListReply.wResourceId);

	astCaExProductList[m].bSectorCount = stExProductListReply.bSectorCount;
	astCaExProductList[m].wOrgSizeSectorProducts =
		stExProductListReply.bSectorCount;
	astCaExProductList[m].pstSectorProducts = (CA_SECTOR_EXTEND_SECTOR_T*)
		ALLOCATE_MEMORY(CA_SECTOR_EXTEND_SECTOR_T,
		stExProductListReply.bSectorCount);
	if (astCaExProductList[m].pstSectorProducts == 0 && stExProductListReply.bSectorCount > 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		CA_ERROR(("CA_PRODUCT_EXTENDED_PRODUCT_ListReplyMsg: Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ;
	}

	CA_DBG_V(("<ca_product> allocate extended sector 0x40 : address %0#10x\n", astCaExProductList[m].pstSectorProducts));

	for (i = 0; i < stExProductListReply.bSectorCount; i++)
	{
		IA_ReadFromMessage(pvMsg, &stExProdcutSector, sizeof
			(msg_extended_product_list_sector_st));

		astCaExProductList[m].pstSectorProducts[i].bProductCount =
			stExProdcutSector.bProductCount;
		astCaExProductList[m].pstSectorProducts[i].bSector =
			stExProdcutSector.bSector;
		astCaExProductList[m].pstSectorProducts[i].bSectorStatus =
			(ia_byte)stExProdcutSector.fSectorValid;
		astCaExProductList[m].pstSectorProducts[i].wSectorDateCode =
			stExProdcutSector.wSectorDateCode;
		astCaExProductList[m].pstSectorProducts[i].wMaxSizeExtendProducts =
			stExProdcutSector.bProductCount;
		astCaExProductList[m].pstSectorProducts[i].pstExtendProducts = 0;

		/* jyou: SCR Allocate memory error */
		if (stExProdcutSector.fSectorValid == IA_TRUE && stExProdcutSector.bProductCount > 0 )
		{
			astCaExProductList[m].pstSectorProducts[i].pstExtendProducts =
				(CA_SECTOR_EXTEND_PRODUCT_T*)ALLOCATE_MEMORY
				(CA_SECTOR_EXTEND_PRODUCT_T, stExProdcutSector.bProductCount);
			if (astCaExProductList[m].pstSectorProducts[i].pstExtendProducts == 0)
			{
				IA_CloseMessage(pvMsg, IA_FALSE);
				CA_ERROR(("CA_PRODUCT_EXTENDED_PRODUCT_ListReplyMsg: Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return ;
			}
			CA_DBG_V(("<ca_product> allocate extended sector 0x6: sector %d: address %0#10x\n", i, astCaExProductList[m].pstSectorProducts[i].pstExtendProducts));

			for (j = 0; j < stExProdcutSector.bProductCount; j++)
			{
				IA_ReadFromMessage(pvMsg, &stExProducts, sizeof
					(msg_extended_product_list_product_st));
				astCaExProductList[m].pstSectorProducts[i].pstExtendProducts[j].wProduct = stExProducts.wProductId;
				astCaExProductList[m].pstSectorProducts[i].pstExtendProducts[j].wStartDate = stExProducts.wStartDate;
				astCaExProductList[m].pstSectorProducts[i].pstExtendProducts[j].bDuration = stExProducts.bDuration;
			}
		}
	}

	/* send the extended productlist to CA client */
	CA_CLIENT_Notify(stExProductListReply.wResourceId,
		CA_QC_EXTENDED_PRODUCT_LIST, (ia_word32)
		astCaExProductList[m].bSectorCount);

	IA_CloseMessage(pvMsg, IA_FALSE);
}

/* jyou: 3.4.2 SCR#25341 added the following line */
static ia_bool SendExtendedProductListQueryMsg( CA_EXTENDED_PRODUCT_LIST_T *pstMsg )
{
	ia_bool             ret = IA_FALSE;
	ia_word32           wSCardId = 0;
	ia_sint16           i;
	CA_EPL_QUERY_T          eplQueryData;

	wSCardId = pstMsg->wResourceId;
	CA_DBG_V(("[ca_prodcut]:=> %s ResourceID %d SectorCount %d\n",
		astrSCellMsgDisplay[MSG_EXTENDED_PRODUCT_LIST_QUERY],
		wSCardId, pstMsg->bSectorCount));
	/* validate the input parameter */
	if ( CA_SCARD_GetGlobalSmartCardStatus(wSCardId) != STATUS_CARD_IN )
	{
		CA_DBG(("[ca_product] Smart card %d not in.\n", wSCardId));
		return IA_FALSE;
	}
	/* Send expired prodcut query */
	CA_EXPIRED_PRODUCTSendQuery(wSCardId);

	ansi_memset((void *)&eplQueryData, 0, sizeof(CA_EPL_QUERY_T));
	eplQueryData.wResourceId = wSCardId;
	eplQueryData.sectorCnt = pstMsg->bSectorCount;

	for (i = 0; i < eplQueryData.sectorCnt; i++)
	{
		eplQueryData.sectorIndex[i] = pstMsg->pstSectorProducts[i].bSector;
	}
	/* jyou: 3.4.3.6 SCR#26045 2007-06-01 */
	for ( i = eplQueryData.sectorCnt; i < MAX_SECTOR_NUMBER; i++ )
	{
		eplQueryData.sectorIndex[i] = INVALID_SC_SECTOR;
	}

	CA_DBG_V(("Perform extended product list query\n"));
	ret = ( CA_QUERY_SUCCESS == CA_EXTENDED_PRODUCT_ListSendQuery(&eplQueryData) ) ? IA_TRUE: IA_FALSE;

	return ret;
}

static ia_bool ReadExtendedProductListReplyMsg( CA_EXTENDED_PRODUCT_LIST_T *pstMsg )
{
	ia_byte             bSectorCount = 0, bProductCount = 0;
	ia_word32           wSCardId = 0;
	ia_sint16           i, j, k, m;

	if ( pstMsg->bSectorCount > 0 && pstMsg->pstSectorProducts == 0)
	{
		CA_DBG(("[ca_product] No sector IDs specified.\n"));
		return IA_FALSE;
	}

	wSCardId = pstMsg->wResourceId;
	/* jyou: 3.4.3 SCR#25649 2007-02-27 Put a limit to the sector count and use default value if ==0*/
	if ( pstMsg->bSectorCount == 0 )
	{
		bSectorCount = (ia_byte)pstMsg->wMaxSizeSectorProducts;
	}
	else
	{
		bSectorCount = pstMsg->bSectorCount;
	}
	/* Make sure the sector count doesn't exceed the predefined upper limit */
	bSectorCount = (ia_byte)(( bSectorCount <= MAX_SECTOR_NUMBER ) ? bSectorCount: MAX_SECTOR_NUMBER);

	for (m = 0; m < SC_SLOT_MAX; m++)
	{
		if ( astCaExProductList[m].wResourceId == wSCardId )
		{
			break;
		}
	}
	if (m == SC_SLOT_MAX)
		/* cann't find the resource id */
	{
		CA_DBG(("[ca_product] No extened product list data for smart card %d.\n", wSCardId));
		return IA_FALSE;
	}


	pstMsg->fExpiredProductsExist =
		astCaExProductList[m].fExpiredProductsExist;

	CA_DBG_V((
		"CA_EXTENDED_PRODUCT_ListQueryControl: fExpiredProductsExist = %d\n", astCaExProductList[m].fExpiredProductsExist));

	pstMsg->wOrgSizeSectorProducts =
		astCaExProductList[m].bSectorCount;
	pstMsg->bSectorCount = astCaExProductList[m].bSectorCount;

	if (bSectorCount > 0)
	{
		if (pstMsg->pstSectorProducts == 0)
		{
			return IA_FALSE;
		}
	}

	for (i = 0; i < bSectorCount; i++)
	{
		for (k = 0; k < astCaExProductList[m].bSectorCount; k++)
		{
			if (pstMsg->pstSectorProducts[i].bSector ==
				astCaExProductList[m].pstSectorProducts[k].bSector)
			{
				pstMsg->pstSectorProducts[i].bSector =
					astCaExProductList[m].pstSectorProducts[k].bSector;
				pstMsg->pstSectorProducts[i].bSectorStatus =
					astCaExProductList[m].pstSectorProducts[k].bSectorStatus;
				pstMsg->pstSectorProducts[i].wSectorDateCode =
					astCaExProductList[m].pstSectorProducts[k].wSectorDateCode;

				/*[irca ect] pyh:直接使用astCaExProductList的bProductCount*/
				
				/*if (pstMsg->pstSectorProducts[i].wMaxSizeExtendProducts >=
					astCaExProductList[m].pstSectorProducts[k].bProductCount)
				{
					bProductCount =
						astCaExProductList[m].pstSectorProducts[k].bProductCount;
				}
				else
				{
					bProductCount = (ia_byte)pstMsg->pstSectorProducts[k].wMaxSizeExtendProducts;
				}*/
				bProductCount =
						astCaExProductList[m].pstSectorProducts[k].bProductCount;

				pstMsg->pstSectorProducts[i].wOrgSizeExtendProducts =
					bProductCount;
				pstMsg->pstSectorProducts[i].bProductCount =
					bProductCount;

				/*if (bProductCount > 0)
				{
					if (pstMsg->pstSectorProducts[i].pstExtendProducts == 0)
					{
						return IA_FALSE;
					}
				}*/
				/*[irca ect] pyh:直接使用astCaExProductList的pstExtendProducts*/
				/* Attach the product list in a sector 
				for (j = 0; j < bProductCount; j++)
				{
					pstMsg->pstSectorProducts[i].pstExtendProducts[j].wProduct
					 = astCaExProductList[m].pstSectorProducts[k].pstExtendProducts[j].wProduct;
					pstMsg->pstSectorProducts[i].pstExtendProducts[j].wStartDate
					 = astCaExProductList[m].pstSectorProducts[k].pstExtendProducts[j].wStartDate;
					pstMsg->pstSectorProducts[i].pstExtendProducts[j].bDuration
					 = astCaExProductList[m].pstSectorProducts[k].pstExtendProducts[j].bDuration;
				}*/
				pstMsg->pstSectorProducts[i].pstExtendProducts = astCaExProductList[m].pstSectorProducts[k].pstExtendProducts;

				break;
			}
		}
	}

	return IA_TRUE;
} /* ReadExtendedProductListReplyMsg */

CA_QUERY_STATUS_EN CA_EXTENDED_PRODUCT_ListQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_bool             ret = IA_FALSE;
	CA_EXTENDED_PRODUCT_LIST_T *pstMsg;
	CA_QUERY_STATUS_EN      eStatus;

	/* validate the input parameter */
	if (pCAClientMsg == 0)
	{
		return CA_QUERY_FAIL;
	}

	CA_PARAM_NOT_USED( wHandle );
	pstMsg = (CA_EXTENDED_PRODUCT_LIST_T*)pCAClientMsg;
	if ( pstMsg->bRequestType == REQUEST_TYPE_QUERY )
	{
		ret = SendExtendedProductListQueryMsg( pstMsg );
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_NOT_AVAILABLE;
	}
	else
	{
		ret = ReadExtendedProductListReplyMsg( pstMsg );
		eStatus = (ret == IA_TRUE) ? CA_QUERY_SUCCESS : CA_QUERY_FAIL ;
	}

	return eStatus ;
}


/* For Expired Product*/
CA_QUERY_STATUS_EN CA_EXPIRED_PRODUCTSendQuery(ia_word32 wResourceId)
{
	ia_msg_handle pvMsg;
	msg_expired_product_query_st stExpiredProduct;

	CA_DBG_V(("<= MSG_EXPIRED_PRODUCT_QUERY\n"));

	pvMsg = IA_OpenOutputMessage(sizeof(msg_expired_product_query_st), sizeof
		(msg_expired_product_query_st));

	CA_DBG_V(("[CA_EXPIRED_PRODUCTSendQuery]Open an output message\n"));
	if (pvMsg != 0)
	{
		stExpiredProduct.wResourceId = wResourceId;
		CA_DBG_V(("[CA_EXPIRED_PRODUCTSendQuery]ResourceId is %d\n", wResourceId));

		IA_WriteToMessage(pvMsg, &stExpiredProduct, sizeof(msg_expired_product_query_st));

		/* jyou: 3.4.2 multiple smart card */
		if (SCELL_Message(MSG_EXPIRED_PRODUCT_QUERY, (ia_word16)wResourceId /*0*/,
			IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
			IA_SUCCESS)
		{
			/* The buffer will be freed by the receiver. */
			IA_CloseMessage(pvMsg, IA_FALSE);
		}
		else
		{
			/* Message not sent. We must free the buffer. */
			IA_CloseMessage(pvMsg, IA_TRUE);
			CA_ERROR(("Sending MSG_EXPIRED_PRODUCT_QUERY failed.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}

	}
	else
	{
		CA_ERROR(("Malloc error.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return CA_QUERY_FAIL;
	}

	CA_DBG_V(("Returned from MSG_EXPIRED_PRODUCT_QUERY\n"));
	return CA_QUERY_SUCCESS;

}


/*
*/
void CA_PRODUCT_EXPIRED_PRODUCTReplyMsg(const CA_SCELL_MSG *pstSCellMsg)
{
	ia_msg_handle pvMsg;
	ia_byte i;
	/* jyou: 3.4.2 moved the query of extended product list from ca_task.c to local */
	/* variabled used by the extra Extended product list query */
	ia_word32   wSCardId = INVALID_SC_SMARTCARDID;
	CA_EPL_QUERY_T  eplQuery;
	/* jyou: end */

	msg_expired_product_reply_st stExpiredProductReply;

	/*allocate the memeory of message and get expired product message*/
	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return ;
	if (IA_ReadFromMessage(pvMsg, &stExpiredProductReply, sizeof
		(msg_expired_product_reply_st)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (astCaExProductList[i].wResourceId ==
			stExpiredProductReply.wResourceId)
			break;
	}
	if (i == SC_SLOT_MAX)
		/* cann't find the resource id */
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}

	astCaExProductList[i].fExpiredProductsExist =
		stExpiredProductReply.fExpiredProductsExist;

	CA_DBG_V(("fExpiredProductsExist = %d\n",
		stExpiredProductReply.fExpiredProductsExist));

	IA_CloseMessage(pvMsg, IA_FALSE);

	/* jyou: 3.4.2 moved the query of extended product list from ca_task.c to local */
	wSCardId = stExpiredProductReply.wResourceId;
	if( CA_PRODUCT_GetListCount(wSCardId) == 0 )
	{
		eplQuery.wResourceId = wSCardId;
		eplQuery.sectorCnt = CA_SCARD_GetSectorNumber(wSCardId);
		for (i = 0; i < eplQuery.sectorCnt; i++)
		{
			eplQuery.sectorIndex[i] = CA_SCARD_GetSectorID(wSCardId, i);
		}
		CA_EXTENDED_PRODUCT_ListSendQuery(&eplQuery);
	}
	/* jyou: end */

}

ia_byte CA_PRODUCT_GetListCount(ia_word32 wResourceId)
{
	ia_byte i;

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (astCaExProductList[i].wResourceId == wResourceId)
			break;
	}
	if (i == SC_SLOT_MAX)
		/* cann't find the resource id */
	{
		return 0xff;
	}
	return astCaExProductList[i].bSectorCount;
}

#ifdef INCL_PRODUCT_BLOCKING

/*
*     ROUTINE : CA_PRODUCT_BlockQueryControl()
* DESCRIPTION : Process CA Client's Query Control reqeust CA_QC_PRODUCT_BLOCK
*       INPUT : ia_word32 wHandle - Reserved for future use.
*       void    pCAClientMsg - Pointer to the data buffer provided by CA Client.
*      OUTPUT : None
*     RETURNS : CA_QUERY_SUCCESS       - Successful.
*       CA_QUERY_FAIL          - Failed.
*       CA_QUERY_NOT_AVAILABLE - Unable to run this query.
* SIDE EFFECT : Calls SendProductBlockQueryMsg or ReadProductBlockReplyMsg depends on request type
*   CALLED BY : CA_TASK_QueryControl when INCL_PRODUCT_BLOCKING is defined
*      Author : Jane You
* Last Changed: October 18, 2006
*/
CA_QUERY_STATUS_EN CA_PRODUCT_BlockQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_sint16 i, j = 0;
	CA_QUERY_STATUS_EN status;
	CA_QUERY_PRODUCT_CHECK_T stQueryProductCheck;
	CA_ANY_PRODUCT_CHECK_T stAnyProduct;
	CA_PRODUCT_BLOCK_T  *pCaProductBlock;

	CA_PARAM_NOT_USED( wHandle );
	pCaProductBlock = (CA_PRODUCT_BLOCK_T *)pCAClientMsg;
	if (pCaProductBlock == 0)
		return CA_QUERY_FAIL;

#if 0 /* 2008-5-11 Angela: Here ResourceId indicates the smartcard resource. */
	if ( pCaProductBlock->wResourceId >= MAX_SECTOR_NUMBER )
#else
	if ( pCaProductBlock->wResourceId >= SC_SLOT_MAX)
#endif
	{
		return CA_QUERY_FAIL;
	}

	if ( pCaProductBlock->bRequestType == REQUEST_TYPE_QUERY )
	{
		ansi_memset((void *)&stAnyProduct, 0, sizeof(CA_ANY_PRODUCT_CHECK_T));
		stAnyProduct.bRequestType = REQUEST_TYPE_QUERY;
		stAnyProduct.wResourceId = pCaProductBlock->wResourceId;

		ansi_memset((void *)&stQueryProductCheck, 0, sizeof(CA_QUERY_PRODUCT_CHECK_T));
		stQueryProductCheck.bRequestType = REQUEST_TYPE_QUERY;
		stQueryProductCheck.wResourceId = pCaProductBlock->wResourceId;
		stQueryProductCheck.fKeepTrack = IA_TRUE;

		/* check and tell any product or indicated product */
		if ( pCaProductBlock->bNumEntries)   /* start check */
		{
			for (i = 0; i < pCaProductBlock->bNumEntries; i++)
			{
				if (pCaProductBlock->astProduct[i].wProduct == 0xFFFF)  /* any products on sector */
				{
					stAnyProduct.sectorIndex[stAnyProduct.bSectorCount] = pCaProductBlock->astProduct[i].bSector;
					stAnyProduct.bSectorCount++;
				}
				else  /* indicate products */
				{
					stQueryProductCheck.bProductCount++;
				}
			}

			if (stQueryProductCheck.bProductCount)  /* indicate product */
			{
				stQueryProductCheck.pstQuerySector = (CA_QUERY_SECTOR_PRODUCT_T *)OS_DRV_AllocateMemory((unsigned long)
					sizeof(CA_QUERY_SECTOR_PRODUCT_T)*(unsigned long)stQueryProductCheck.bProductCount);

				if (stQueryProductCheck.pstQuerySector == 0)
				{
					CA_ERROR(("CA_PRODUCT_BlockQueryControl: Memory allocate failed.\n"));
					CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
					return CA_QUERY_FAIL;
				}

				for (i = 0; i < pCaProductBlock->bNumEntries; i++)
				{
					if (pCaProductBlock->astProduct[i].wProduct != 0xFFFF)
					{
						stQueryProductCheck.pstQuerySector[j].fAnySector = IA_FALSE;
						stQueryProductCheck.pstQuerySector[j].bSector =  pCaProductBlock->astProduct[i].bSector;
						stQueryProductCheck.pstQuerySector[j].wProduct =  pCaProductBlock->astProduct[i].wProduct;
						j++;
					}
				}
				status = CA_PRODUCT_CheckSendQuery(IA_FALSE, &stQueryProductCheck);
				if (status == CA_QUERY_FAIL)
					return CA_QUERY_FAIL;
			}

			if (stAnyProduct.bSectorCount)
			{
				OS_DRV_DelayTask(2000);
				status = CA_ANY_PRODUCT_SendQuery(&stAnyProduct); /* just for normal products */
				if (status == CA_QUERY_FAIL)
					return CA_QUERY_FAIL;
			}

		}

		else  /* stop check */
		{
			stQueryProductCheck.fKeepTrack = IA_FALSE;
			stQueryProductCheck.bProductCount = 0;
			stQueryProductCheck.pstQuerySector = 0;
			stQueryProductCheck.bRequestType = REQUEST_TYPE_QUERY;
			status = CA_PRODUCT_CheckSendQuery(IA_FALSE, &stQueryProductCheck); /* stop check indicate product */
			if (status == CA_QUERY_FAIL)
				return CA_QUERY_FAIL;
			status = CA_STOP_ANY_PRODUCT_SendQuery(pCaProductBlock->wResourceId);
			if (status == CA_QUERY_FAIL)
				return CA_QUERY_FAIL;
		}
	}
	if (pCaProductBlock->bRequestType == REQUEST_TYPE_REPLY)
	{
		CA_ANY_PRODUCT_CHECK_T stAnyProQuery;
		ia_bool fAnyProdCheck = IA_FALSE;

		ansi_memset((void *)&stAnyProQuery, 0, sizeof(CA_ANY_PRODUCT_CHECK_T));
		ansi_memset((void *)&stQueryProductCheck, 0, sizeof(CA_QUERY_PRODUCT_CHECK_T));

		/*get the any product and indate product*/
		for (i = 0; i < pCaProductBlock->bNumEntries; i++)
		{
			if (pCaProductBlock->astProduct[i].wProduct != ANY_FTA_PRODUCT)
			{
				stQueryProductCheck.bProductCount ++;
			}
			else
			{
				if (fAnyProdCheck == IA_FALSE)
				{
					stAnyProQuery.wResourceId = pCaProductBlock->wResourceId;
					stAnyProQuery.bRequestType = REQUEST_TYPE_REPLY;
					CA_ANY_PRODUCT_SendQuery(&stAnyProQuery);
					fAnyProdCheck = IA_TRUE;
				}
			}

		}

		if (stQueryProductCheck.bProductCount)
		{
			stQueryProductCheck.wResourceId = pCaProductBlock->wResourceId;
			stQueryProductCheck.bRequestType = REQUEST_TYPE_REPLY;
			stQueryProductCheck.fKeepTrack = IA_TRUE;
			stQueryProductCheck.bGracePeriod = 0;
			stQueryProductCheck.pstQuerySector = (CA_QUERY_SECTOR_PRODUCT_T*)
				OS_DRV_AllocateMemory((unsigned long)sizeof(CA_QUERY_SECTOR_PRODUCT_T)*(unsigned long)stQueryProductCheck.bProductCount);
			if (stQueryProductCheck.pstQuerySector == 0)
			{
				CA_ERROR(("CA_PRODUCT_BlockQueryControl: Memory allocate failed.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return CA_QUERY_FAIL;
			}
			CA_PRODUCT_CheckSendQuery(IA_FALSE, &stQueryProductCheck);
		}

		for (i = 0; i < pCaProductBlock->bNumEntries; i++)
		{
			if ( pCaProductBlock->astProduct[i].wProduct == ANY_FTA_PRODUCT )
			{
				/* fill any product */
				for (j = 0; j < MAX_SECTOR_NUMBER; j++)
				{
					if (pCaProductBlock->astProduct[i].bSector == stAnyProQuery.stQuerySector[j].bSector)
					{
						pCaProductBlock->astProduct[i].wProduct = stAnyProQuery.stQuerySector[j].wProduct;
						pCaProductBlock->astProduct[i].fSectorValid = stAnyProQuery.stQuerySector[j].fSectorValid;
						pCaProductBlock->astProduct[i].fProductValid = stAnyProQuery.stQuerySector[j].fProductValid;
						break;

					}
				}
			}
			else  /* fill indicated product */
			{
				for (j = 0; j < stQueryProductCheck.bProductCount; j++)
				{
					if (pCaProductBlock->astProduct[i].wProduct == stQueryProductCheck.pstQuerySector[j].wProduct &&
						pCaProductBlock->astProduct[i].bSector == stQueryProductCheck.pstQuerySector[j].bSector)
					{
						pCaProductBlock->astProduct[i].fSectorValid = stQueryProductCheck.pstQuerySector[j].fSectorValid;
						pCaProductBlock->astProduct[i].fProductValid = stQueryProductCheck.pstQuerySector[j].fProductValid;
					}
				}

			}
		}
		if (stQueryProductCheck.pstQuerySector)
		{
			OS_DRV_FreeMemory(stQueryProductCheck.pstQuerySector);
			stQueryProductCheck.pstQuerySector = 0;
		}
	}

	return CA_QUERY_SUCCESS;

}

/*
* jyou: a wrapper of CA_PRODUCT_CheckSendQuery,
*/
CA_QUERY_STATUS_EN CA_PRODUCT_PVRProductBlockQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	CA_PARAM_NOT_USED( wHandle );
	return CA_PRODUCT_CheckSendQuery( IA_TRUE, (CA_QUERY_PRODUCT_CHECK_T *)pCAClientMsg );
}


#endif /* INCL_PRODUCT_BLOCKING */
