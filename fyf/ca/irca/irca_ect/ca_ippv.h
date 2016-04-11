/**
 * $Id: ca_ippv.h,v 1.8 2007/11/20 05:15:31 ayang Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the ippv query control.
**/

#ifndef _CA_IPPV_H_
#define _CA_IPPV_H_


#define _CA_IPPV_DEBIT_PATCH
#define SCPINSTATUS		0x50

/*
typedef enum
{
CA_IPPV_PREVIEW_SUBOP,
CA_IPPV_BUY_SUBOP,
CA_IPPV_DEBIT_SUBOP,
CA_IPPV_DEBIT_THRESHOLD_SUBOP,
CA_IPPV_PURSES_SUBOP,
CA_IPPV_EVENTS_LIST_SUBOP,
CA_IPPV_EVENT_CHECK_SUBOP
}CA_IPPV_QUERY_CONTROL_EN;
*/


/*VOID 	CA_IPPV_Process(ia_byte *pbMsg);*/
void 	CA_IPPV_Initialise(ia_word32 wResourceId);
void	CA_IPPV_InitEventInfo(ia_byte bScardIndex, ia_word32 wResourceId);

void CA_IPPV_EventNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_IPPV_BuyNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_IPPV_SectorInfoNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_IPPV_DebitNotify(const CA_SCELL_MSG *pstSCellMsg );
void CA_IPPV_PursesNotify(const CA_SCELL_MSG *pstSCellMsg);
void CA_IPPV_EventsListNotify(const CA_SCELL_MSG *pstSCellMsg );
void CA_IPPV_EventCheckNotify(const CA_SCELL_MSG *pstSCellMsg );
void CA_IPPV_ParseCallBackRequestNotify(const CA_SCELL_MSG *pstSCellMsg );
void CA_IPPV_PhoneNumber_Notify(const CA_SCELL_MSG *pstSCellMsg);
void CA_IPPV_MemoryFree(void);

/*
CA_QUERY_STATUS_EN CA_IPPV_PreviewQueryControl(CA_IPPV_PREVIEW_T* caIppvPreview);
CA_QUERY_STATUS_EN CA_IPPV_SetThresholdQueryControl(CA_IPPV_SET_THRESHOLD_T* caIppvDebitThreshold);
CA_QUERY_STATUS_EN CA_IPPV_GetIppvSectorInfoQueryControl(CA_IPPV_SECTOR_INFO_T* pCaIppvSectorInfo);
CA_QUERY_STATUS_EN CA_IPPV_EventsListQueryControl(CA_IPPV_EVENTS_LIST_T* caIppvEventList);
CA_QUERY_STATUS_EN CA_IPPV_EventsCheckQueryControl(CA_IPPV_EVENTS_CHECK_T* caIppvEventsCheck);
*/

CA_QUERY_STATUS_EN CA_IPPV_SendDebitQuery(ia_word32 wSCardId, ia_byte bSector);
void CA_IPPV_SetSectorBySectorIndex(ia_word32 wResourceId, ia_byte bSector);

/* jyou */
CA_QUERY_STATUS_EN CA_IPPV_PurchaseQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_IPPV_PreviewQueryControl(  ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_IPPV_SetThresholdQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_IPPV_GetIppvSectorInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_IPPV_EventsListQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_IPPV_EventsCheckQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_IPPV_CallbackRequestQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_IPPV_PhoneNumber_QueryControl( ia_word32 wHandle, void* pCAClientMsg );

#endif /* CA_IPPV_H */


