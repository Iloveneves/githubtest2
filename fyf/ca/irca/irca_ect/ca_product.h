/**
* $Id: ca_product.h,v 1.9 2008/05/09 10:59:34 ayang Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the surflock query control.
**/

#ifndef _CA_PRODUCT_H_
#define _CA_PRODUCT_H_

#include "ca_local.h"
#include <irca/ca_task.h>

typedef struct
{
	ia_byte			bRequestType;
	ia_word32   wResourceId;
	ia_byte			sectorCnt;
	ia_byte			sectorIndex[ MAX_SECTOR_NUMBER ];
}CA_EPL_QUERY_T;

#ifdef INCL_PRODUCT_BLOCKING
CA_QUERY_STATUS_EN CA_PRODUCT_BlockQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_PRODUCT_PVRProductBlockQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_ANY_PRODUCT_SendQuery( CA_ANY_PRODUCT_CHECK_T *proQuery );
CA_QUERY_STATUS_EN CA_STOP_ANY_PRODUCT_SendQuery( ia_word32 wResourceId );
void CA_ANY_PRODUCT_ReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
ia_bool CA_PRODUCT_BlockCheck(ia_word32 wSmartCard) ;
void CA_PRODUCT_BLOCK_Init(ia_word32 wSmartCardId);
/* mli: 3.3.11 moved by jyou for passing compiling */
void CA_PRODUCT_CheckReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
#endif

void CA_EXTENDED_PRODUCT_ListInitialise(ia_word32 wResourceId);
void CA_EXTENDED_PRODUCT_ListFreeMemory(ia_word32 wResourceId);

CA_QUERY_STATUS_EN CA_PRODUCT_CheckSendQuery(ia_bool fPVRProd, CA_QUERY_PRODUCT_CHECK_T *pQueryProductCheck );
CA_QUERY_STATUS_EN CA_PRODUCT_ListSendQuery(ia_word32 wResourceId);
CA_QUERY_STATUS_EN CA_STOP_PRODUCT_ListSendQuery( ia_word32 wResourceId );

CA_QUERY_STATUS_EN CA_EXTENDED_PRODUCT_ListSendQuery( CA_EPL_QUERY_T *eplQuery );
CA_QUERY_STATUS_EN CA_EXTENDED_PRODUCT_ListQueryControl( ia_word32 wHandle, void *pCAClientMsg );

CA_QUERY_STATUS_EN CA_EXPIRED_PRODUCTSendQuery(ia_word32 wResourceId);

void CA_PRODUCT_EXTENDED_PRODUCT_ListReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_PRODUCT_ListReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_PRODUCT_EXPIRED_PRODUCTReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
ia_byte CA_PRODUCT_GetListCount(ia_word32 wResourceId);
ia_byte	CA_PRODUCT_CHECK_ListFreeMemory(ia_word32 wSmartCardId);
void CA_PRODUCT_CheckInitialise(ia_word32 wSmartCardId);

#endif /* _CA_SURFLOCK_H_ */

