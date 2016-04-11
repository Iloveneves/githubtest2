/**
* $Id: ca_dmbsvcinfo.h,v 1.2 2007/11/20 05:15:31 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the dmb service info query control.
**/

#ifndef _CA_DMBSVCINFO_H_
#define _CA_DMBSVCINFO_H_

#include "ca_local.h"
#include <irca/pubh/s3_message_helper.h>

#define CA_SERVICE_MAX 8
#define INVALID_MAPPING_SUBCHID  0xFF
#define INVALID_PACKET_ADDRESS  0xFFFF
#define INVALID_TRANSPORT_MODE  0xFF
#define INVALID_SUBCHANNEL_SUBCHID  0xFF

void CA_DMB_AddEmmResourceToEmmService( ia_word32 wOpSeqNo);
void DefineDMBDescrambleServices(ia_word32 wGroupHandle, ia_byte *pbMsg);
void CA_DMB_AddDefaultResourceToEmmService( ia_word32 wOpSeqNo );
void CA_SVCINFO_SetDMBInfo(ia_word32 wIndex, ia_word32 wSrvType, ia_byte *pbMsg);
void UpdateDefaultResToDMBDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType,ia_bool bAction);
void AddEcmResToDMBDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType,ia_bool bAction);
ia_bool CA_DMB_ParseDescrambleInfo(ia_word32 wServiceType,ia_word32 wGroupHandle,msg_service_status_data_st* pstSvcStatusData,ia_msg_handle pvMsg);
void RemoveDMBEmmResFromService(ia_word16 wOpSeqNo);
void CA_DMB_EmmSvcNotify(ia_word32 wHandle);
void CA_DMB_ResetLocalEMMServiceInfo( CA_LOCAL_BASIC_DMB_INFO_T* pTemp );
CA_QUERY_STATUS_EN CA_SVCINFO_DMBEmmSvcInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_DMB_DescrambleInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg );
void CA_DMB_ParseEcmMonitorMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_DMB_ParseEmmMonitorMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_MONITOR_DMBEmmNotify( ia_word32 wEmmHandler);
void CA_MONITOR_DMBEcmNotify(ia_word32 wCaClientHandle);
void CA_DMB_ResetLocalECMServiceInfo(ia_word32 wIndex, ia_word32 wSrvType);
CA_LOCAL_BASIC_DMB_INFO_T* CA_SVCINFO_GetDMBEMMSvcInfoPtr( ia_word16 wSCellHandle, ia_word16 *pwIndex0, ia_word16 *pwIndex1 );
void DMB_ServicesClose(ia_word32 wIndex, ia_word32 wSrvType);
void DMB_SVCINFO_ResetLocalSvcInfo(ia_word32 wIndex);
#endif /* _CA_DMBSVCINFO_H_ */


