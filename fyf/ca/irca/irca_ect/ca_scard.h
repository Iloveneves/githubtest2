/**
* $Id: ca_scard.h,v 1.15 2008/04/14 08:11:52 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* Header file for smartcard info query control.
**/

#ifndef _CA_SCARD_H_
#define _CA_SCARD_H_

#include "ca_local.h"

void CA_SCARD_Initialise( ia_word32 wResourceId );
ia_byte	  CA_SCARD_ValidSmartCards( void );
ia_result CA_SCARD_SetSmartCardId(ia_word32 wResourceId);
ia_word32 CA_SCARD_GetSmartCardId(ia_byte bIndex);
ia_word16 CA_SCARD_GetGlobalSmartCardStatus(ia_word32 wResourceId);
ia_status_st CA_SCARD_GetGlobalSmartCardStatusStructure(ia_word32 wResourceId);

/* void CA_SCARD_ParseIppvPinChangeMsg( const CA_SCELL_MSG *pstSCellMsg ); */
/* void CA_SCARD_ParseIppvPinCheckMsg( const CA_SCELL_MSG *pstSCellMsg ); */
void CA_SCARD_ParseRemoteChannelReplyMsg(const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_ParsePinCodeMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_SendSmartCardHomingChQueryMsg( ia_word32 wResourceId );
void CA_SCARD_ParseSmartCardHomingChReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_SendNationalityQueryMsg(ia_word32 wResourceId );
void CA_SCARD_ParseNationalityMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_SendSmartCardDataQueryMsg( ia_word32 wResourceId );
void CA_SCARD_ParseSmartCardDataReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_SmartCardInfoNotify( ia_word32 wResourceId, ia_word16 wType );
void CA_SCARD_ParseSmartCardIdMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_MRCapabilityNotify( const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_MRConfigurationNotify( const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_MRProfileNotify(const CA_SCELL_MSG * pstSCellMsg);
CA_QUERY_STATUS_EN CA_SCARD_CodeChangeQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_SCARD_CodeCheckQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_SCARD_ScAccessQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_SCARD_SmartCardInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_SCARD_ScHashQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_SCARD_RemoteChannelQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_SCARD_MRCapabilityQueryControl(  ia_word32 wHandle, void* pCAClientMsg  );
CA_QUERY_STATUS_EN CA_SCARD_MRConfigurationQueryControl(  ia_word32 wHandle, void* pCAClientMsg  );
CA_QUERY_STATUS_EN CA_SCARD_MRProfileQueryControl(  ia_word32 wHandle, void* pCAClientMsg  );

void CA_SCARD_ParseSectorsNumberMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_SCARD_ParseNumberOfSectorsNotify( const CA_SCELL_MSG *pstSCellMsg );
CA_QUERY_STATUS_EN CA_SCARD_SectorsListQueryControl( ia_word32 wHandle, void* pCAClientMsg );
ia_byte CA_SCARD_GetSectorNumber( ia_word32 wResourceId );
ia_byte CA_SCARD_GetSectorID(ia_word32 wResourceId, int sectorIndex );

CA_QUERY_STATUS_EN CA_SCARD_RegionSendQuery( ia_byte bSectorIndex, ia_word32 wResourceId );
void CA_SCARD_ParseRegionReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
CA_QUERY_STATUS_EN CA_SCARD_RegionQueryControl( ia_word32 wHandle, void* pCAClientMsg );

void CA_SCARD_FreeMemory( void );

void CA_SCARD_SendUserDataQueryMsg(ia_word32 wResourceId);
void CA_SCARD_ParseUserDataReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
CA_QUERY_STATUS_EN CA_SCARD_UserDataQueryControl( ia_word32 wHandle, void* pCAClientMsg );

#ifdef _AUSTAR_
void CA_SCARD_STATUSReplyMsg( ia_word32	wResourceId	);
void CA_Modi_SC_Status(ia_word32 wSCardId, ia_word16 wSCardStatus);
#endif

/* jyou */
/* Query Controls handlers in version 3.4-1 SmartCard diagnose */
void CA_SCARD_ParseSmartCardUniqueAddressReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
CA_QUERY_STATUS_EN CA_SCARD_SmartCardUniqueAddressControl( ia_word32 wHandle, void* pCAClientMsg );

void CA_SCARD_ParseSmartCardVendorIDReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
CA_QUERY_STATUS_EN CA_SCARD_SmartCardVendorIDControl( ia_word32 wHandle, void* pCAClientMsg );

void CA_SCARD_ParseSmartCardSectorAddressReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
CA_QUERY_STATUS_EN CA_SCARD_SmartCardSectorAddressControl( ia_word32 wHandle, void *pCAClientMsg );

void CA_SCARD_ParseSmartCardSectorDatecodeReplyMsg(const CA_SCELL_MSG *pstSCellMsg);
CA_QUERY_STATUS_EN CA_SCARD_SmartCardSectorDatecodeControl( ia_word32 wHandle, void* pCAClientMsg );

void CA_SCARD_ParseSmartCardOperatorIDReplyMsg(const CA_SCELL_MSG *pstSCellMsg);
CA_QUERY_STATUS_EN CA_SCARD_SmartCardOperatorIDQueryControl( ia_word32 wHandle, void* pCAClientMsg );

/* Resolve the hard code of smart card ID issue */
ia_byte CA_SCARD_SaveSmartCardResourceID( ia_word32 wResourceId );

#endif /* _CA_SCARD_H_ */
