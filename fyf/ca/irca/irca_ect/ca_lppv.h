/**
 * $Id: ca_lppv.h,v 1.3 2008/04/14 08:11:52 ayang Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the lppv query control.
**/

#ifndef _CA_LPPV_H_
#define _CA_LPPV_H_

void CA_LPPV_Initialise(ia_word32 wResourceId);
void CA_LPPV_MemoryFree(void);

void CA_LPPV_AwareNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_LPPV_PreviewNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_LPPV_ReadTicketNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_LPPV_DeleteTicketNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_LPPV_ReadCreditNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_LPPV_ReadOrderNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_LPPV_PlaceOrderNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_LPPV_CancelOrderNotify(const CA_SCELL_MSG *pstSCellMsg);

CA_QUERY_STATUS_EN CA_LPPV_AwareQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_LPPV_PreviewQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_LPPV_ReadTicketQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_LPPV_DeleteTicketQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_LPPV_ReadCreditQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_LPPV_ReadOrderQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_LPPV_PlaceOrderQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_LPPV_CancelOrderQueryControl( ia_word32 wHandle, void* pCAClientMsg );

#endif /* CA_LPPV_H */


