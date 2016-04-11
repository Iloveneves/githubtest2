/**
 * $Id: ca_hgpc.h,v 1.3 2008/04/14 08:11:52 ayang Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the lppv query control.
**/

#ifndef _CA_HGPC_H_
#define _CA_HGPC_H_

void CA_HGPC_Initialise(ia_word32 wResourceId);
void CA_HGPC_MemoryFree(void);

void CA_HGPC_SmartCardDataNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_HGPC_SoftCellDataNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_HGPC_MsgStatusNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_HGPC_EventNotify(const CA_SCELL_MSG *pstSCellMsg);

CA_QUERY_STATUS_EN CA_HGPC_SmartCardDataQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_HGPC_ScellDataQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_HGPC_MsgStatusQueryControl( ia_word32 wHandle, void* pCAClientMsg );

#endif /* _CA_HGPC_H_ */


