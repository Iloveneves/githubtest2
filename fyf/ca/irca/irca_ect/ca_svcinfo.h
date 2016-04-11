/**
* $Id: ca_svcinfo.h,v 1.11 2008/08/01 03:11:50 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the service info query control.
**/

#ifndef _CA_SVCINFO_H_
#define _CA_SVCINFO_H_

#include "ca_local.h"
#include <irca/pubh/s3_message_helper.h>

#define SERVICE_OPERATION    0
#define QUERY_OPERATION      1


void CA_SVCINFO_Initialise(ia_word16 wMaxNumServiceGroups);
void CA_SVCINFO_ResetLocalSvcInfo(ia_word32 wIndex);

void CA_SVCINFO_SetSCellHandle( ia_word32 wService, ia_word32 wSrvType, ia_word16 wSCellHandle );
ia_word16 CA_SVCINFO_GetSCellHandle(ia_word32 wGrpHandle, ia_word32 wSrvType);
void CA_SVCINFO_SetSCellHandleStatus(ia_word32 wIndex, ia_word32 wSrvType, CA_SERVICE_HANDLE_STATUS_EN eSCellHandleStatus);
CA_SERVICE_HANDLE_STATUS_EN CA_SVCINFO_GetSCellHandleStatus(ia_word32 wHandle, ia_word32 wSrvType);

ia_word32 CA_SVCINFO_GetRecIdFromGroupHandle(ia_word32 wGroupHandle);
ia_word32 CA_SVCINFO_GetPlaybackIdFromGroupHandle(ia_word32 wGroupHandle);

void CA_SVCINFO_SetSvcToFree(ia_word32 wIndex, ia_word32 wSrvType);

ia_bool CA_SVCINFO_GetGrpSvcStatus(ia_word32 wIndex);
ia_bool CA_SVCINFO_GetEmmSvcStatus(ia_word32 wIndex);

void* CA_SVCINFO_GetSvcInfoPtr(ia_word32 wIndex, ia_word32 wSrvType);

ia_word32 CA_SVCINFO_GetSvcTypeFromSCellHandle(ia_word16 wSCellHandle);
ia_word32 CA_SVCINFO_HandleSCell2CaClient(ia_word16 wServiceHandle);
void CA_SVCINFO_EmmSvcNotify(ia_word32 wHandle);
void CA_SVCINFO_DesSvcNotify(ia_word32 wCaClientHandle);
void CA_SVCINFO_SetSvcMaxNumber(ia_word16 wMaxNumServiceGroups);
ia_word16 CA_SVCINFO_GetSvcMaxNumber(void);

void CA_SVCINFO_MemoryFree(void);
void CA_SVCINFO_SetSvcHeaderInfo(ia_word32 wIndex, CA_SERVICE_HEADER *pstCaSvcGrpDefHeader);
ia_result CA_SVCINFO_GetSvcHeaderInfo(ia_word32 wService, CA_SERVICE_HEADER *pstCaSvcGrpDefHeader);


void CA_SVCINFO_GrpSvcNotify(ia_word32 wCaClientHandle, ia_word32 wSrvType);
void CA_SVCINFO_MRPinNotify(ia_word32 wCaClientHandle, ia_word32 wSrvType);

ia_word16 CA_SVCINFO_GetSvcSlotFromScellHandle(ia_word16 wScellHandle);
void CA_SVCINFO_SetPvrMapInfo(ia_word32 wService, CA_SERVICE_HEADER *pstCaSvcHeader, ia_word32 wSessionResourceId);
CA_QUERY_STATUS_EN CA_SVCINFO_EmmSvcInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_SVCINFO_SvcInfoQueryControl( ia_word32 wHandle, void* pCAClientMsg );

/* jyou: updated */
ia_bool CA_SVCINFO_ParseEMMServiceInfo(msg_service_status_data_st *pstSvcStatusData,DEMUX_INFO_ST *pstDMX,ia_msg_handle pvMsg);
/*sren*/
ia_bool CA_SVCINFO_ParseDescrambleServiceInfo(ia_word32 wServiceType,ia_word32 wService,msg_service_status_data_st *pstSvcStatusData,SERVICE_INFO_ST *pstService,ia_msg_handle pvMsg);
ia_bool CA_SVCINFO_ParsePVRServiceInfo(ia_word32 wServiceType,ia_word32 wService,msg_service_status_data_st *pstSvcStatusData,ia_msg_handle pvMsg);

/* jyou: 3.4.2 multiple smart cards */
CA_LOCAL_BASIC_SERVICE_INFO_T* CA_SVCINFO_GetEMMSvcInfoPtr( ia_word16 wSCellHandle, ia_word16 *pwIndex0, ia_word16 *pwIndex1 );
ia_word16 CA_SVCINFO_FindDemux( ia_word32 wDemuxID );
ia_word16 CA_SVCINFO_AddDemux( ia_word32 wDemuxID );
void CA_SVCINFO_ReleaseDemux( ia_word32 wDemuxID );
void CA_SVCINFO_ReleaseAllDemuxes( void );
CA_LOCAL_BASIC_SERVICE_INFO_T* CA_SVCINFO_GetEMMServiceByOpSeqNo( ia_word16 wOpSeqNo );
ia_bool isValidDemuxIndex( ia_word32 wIndex );
ia_bool isValidSmartCardIndex( ia_word32 wIndex );
void ResetBasicServiceInfo( CA_LOCAL_BASIC_SERVICE_INFO_T* pTemp );

/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
CA_LOCAL_BASIC_SERVICE_INFO_T* CA_SVCINFO_GetDescServiceByGroupHandle( ia_word32 wHandle );
void CA_SVCINFO_SetMonitorDefinedFlag( ia_word32 wIndex, CA_SERVICE_TYPE_EN eSvcType, ia_bool fFlag );
ia_bool CA_SVCINFO_isMonitorDefinedFlagSet( ia_word32 wIndex, CA_SERVICE_TYPE_EN eSvcType );

/* jyou: 3.6.1 SCR#26708 2007-12-26 */
void CA_SVCINFO_SetResourceIDs( ia_word16 wGroupHandle, CA_SERVICE_TYPE_EN eServiceType,
	ia_word32 wResourceType,  ia_word32 wResourceID );
ia_word32 CA_SVCINFO_GetResourceID( ia_word16 wGroupHandle, CA_SERVICE_TYPE_EN eServiceType,
	ia_word32 wResourceType );


#endif /* _CA_SVCINFO_H_ */


