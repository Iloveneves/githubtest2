/**
 * $Id: ca_local.h,v 1.15 2008/08/01 03:11:50 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* Header file for internal function prototypes.
**/

#ifndef _CA_LOCAL_H_
#define _CA_LOCAL_H_

#include <irca/pubh/s3_cadrv.h>
#include <irca/ca_task.h>
#include <irca/pubh/s3_vd_copycontrol.h>
#include <irca/pubh/s3_maturity_rating.h>

/* To Suppress warnings */
#define CA_PARAM_NOT_USED(x) ((void)(x))

#define	NOT_FOUND			0xffff

#define CA_CLIENT_SERVICE_MSG_MAX_LENGTH			   2
#define CA_CLIENT_SERVICE_GROUP_DEFINE_HEADER_SIZE	   8
#define CA_CLIENT_SERVICE_STOP_HEADER_SIZE				8
#define CA_CLIENT_SERVICE_EMM_HEADER_SIZE			   8
#define CA_CLIENT_SERVICE_EMM_STOP_HEADER_SIZE			8
#define CA_CLIENT_CURRENT_TS_NIT_INFO_HEADER_SIZE		10
#define CA_CLIENT_MAP_RESOURCE_HEADER_SIZE          10

#define CA_CLIENT_RECORD_HEADER_MAX_LENGTH			    10
#define CA_CLIENT_PLAYBACK_HEADER_MAX_LENGTH		    10

#define CA_CLIENT_SERVICE_RECORDID_SIZE					4
#define CA_CLIENT_SERVICE_PLAYBACKID_SIZE				4
#define CA_CLIENT_SERVICE_OOBID_SIZE					4
#define CA_CLIENT_SERVICE_DEMUXID_SIZE					4

#define CA_CLIENT_SERVICE_GROUP_HANDLE					0
#define CA_CLIENT_SERVICE_GROUP_DEFINE_SERVICESTYPE		1
#define CA_CLIENT_SERVICE_STOP_SERVICESTYPE				1
#define CA_CLIENT_SERVICE_GROUP_MAP_TYPE                1

#define CA_CLIENT_SERVICE_RECORDID						0


#define CA_CLIENT_EMM_SERVICE_HANDLE					0
#define CA_CLIENT_EMM_SERVICE_SOURCE			   		1
#define CA_CLIENT_EMM_SERVICE_SMARTCARDID				2

#define	INVALID_SERVICE_TYPE        	0xFFFFFFFF
#define	INVALID_SCELL_SERVICE_HANDLE	0xFFFF
#define	INVALID_GROUP_SERVICE_HANDLE	0xFFFFFFFF
#define	INVALID_PVR_HANDLE				0xFFFF
#define INVALID_PVR_ID					0xFFFFFFFF
#define INVALID_PVR_SLOT				255
#define INVALID_SMARTCARD_INFO_BYTE		0xFF
#define INVALID_SECTOR_DATACODE			0
#define INVALID_SECTOR_OPERATOR_ID		0
#define INVALID_INDEX				0xFFFF
/* ayang 2008-04-08 changed 0xFFFF to 0xFFFF */
#define INVALID_RESOURCE_ID 		0xFFFFFFFF
#define INVALID_RESOURCE_TYPE		0 /*according to */


#define ECT_FOR_DVB_CLIENT		0X01
#define ECT_FOR_DMB_CLIENT		0X02

/* SoftCell -> CA Task message structure. */
typedef struct
{
	ca_drv_message_type_e  eOpcode;
	ia_word16              wOpSeqNo;
	ia_word16              wLength;
	void*                  pvMessage;
}CA_SCELL_MSG;

typedef struct
{
	ia_word32				wGroupHandle;
	ia_word32				wSmartCardId;
	ia_word32				wDemuxId;
	ia_word32				wMacrovisionId;
	ia_word32				wOOBId;
	ia_word32				wServiceType;
}CA_SERVICE_HEADER;

/*
typedef struct
{
ia_word32				wEmmHandle;
ia_word32				wOOBId;
ia_word32				wDemuxId;
ia_word32				wSmartCardId;
ia_word32				wSourceType;
}CA_EMM_SERVICE_HEADER;
*/

/***************************************************************************************/
typedef enum
{
	CA_SERVICE_DESCRAMBLE = 0,
	CA_SERVICE_RECORD,
	CA_SERVICE_PLAYBACK,
	CA_SERVICE_DMB_DESCRAMBLE,
	CA_SERVICE_MAX_SERVICES,  /* max grp service */
	CA_SERVICE_EMM,
	CA_SERVICE_DMB_EMM
} CA_SERVICE_TYPE_EN;

typedef enum
{
	CA_SERVICE_HANDLE_EMPTY = 0,
	CA_SERVICE_HANDLE_PROCESSING,
	CA_SERVICE_HANDLE_OCCUPIED,
	CA_SERVICE_HANDLE_WAITCLOSE
} CA_SERVICE_HANDLE_STATUS_EN;

typedef struct
{
	ia_word32 wResourceType ;
	ia_word32 wResourceID;
} CA_RESOURCE_DEFINE_INFO_T;

typedef struct
{
	ia_word32		wSmartCardId;		/* By default: = 0 */
	ia_word32		wServiceType;		/* SoftCell 3 defined */
	ia_word16		wSCellHandle;
	ia_status_st	stGlobalStatus;		/* Global prioritised status for this service. */
	ia_status_st	stServiceStatus;	/* Status of this service itself */
	ia_byte			bRscCount;
	ia_word32		awRscType[MAX_SERVICE_RESOURCES];
	ia_status_st	astRscStatus[ MAX_SERVICE_RESOURCES ];
	/* jyou: replacement of the error status for EMM service in ca_error.c */
	ia_status_st	stErrorStatus;
	/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
	/* Flags for the whether a monitoring resource has been defined for the given service */
	ia_bool			fMonitorDefined;
	ia_byte		bResourceDefineCount;
	CA_RESOURCE_DEFINE_INFO_T	astResourceDefines[ MAX_SERVICE_RESOURCES ]; /* jyou: 3.6.1 SCR#26708 2007-12-26 */
	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	CA_SERVICE_HANDLE_STATUS_EN eSvcHandleStatus;
} CA_LOCAL_BASIC_SERVICE_INFO_T;

typedef struct
{
	ia_byte			bMappingTableSubChId;
	ia_byte			bTransportMode;
	ia_byte         bEmmSubChId;
	ia_word16       wEmmPA;
	ia_word16       wEmmPid;
} CA_LOCAL_BASIC_DMB_INFO_T;
typedef struct
{
		ia_word16      wContentPA;
		ia_byte       bContentSubChId;
}CA_DMB_STREAM_INFO_T;

typedef struct
{
	/* wEmmHandle = wGroupHandle:
	You will always expect a SERVICE_DVB_DESCRAMBLE and SERVICE_DVB_EMM to use the same handle. */
	ia_word32		wGroupHandle;		/* it's a sequence number start from 0 */

	ia_word32		wSmartCardId;		/* By default: = 0 */
	ia_word32		wDemuxId;			/* By default: = wGroupHandle */
	ia_word32		wMacrovisionId;	/* By default: = wGroupHandle */
	/*	ia_word32	wOOBId;			*/	/* By default: = wGroupHandle */


	/* Multiple services can exist at the same time for a group.
	These sevices share the same resource IDs defined above. */
	CA_LOCAL_BASIC_SERVICE_INFO_T	astServices[CA_SERVICE_MAX_SERVICES];

	/* SERVICE_DVB_DESCRAMBLE specific info. */
	ia_byte			bStreamCount;
	STREAM_INFO_T	astStreamInfo[ MAX_SOFTCELL_STREAMS ];
	CA_LOCAL_BASIC_DMB_INFO_T astDMBServices[CA_SERVICE_MAX_SERVICES];
	CA_DMB_STREAM_INFO_T	astDMBStreamInfo[ MAX_SOFTCELL_STREAMS ];
	DMB_STREAM_INFO_T	astDMBStreamConInfo[ MAX_SOFTCELL_STREAMS ];

	/* Only valid when wServiceType== [ PVR_Reccord | (PVR_DRM for record) ] */
	CA_PVR_SERVICE_INFO_T	stPvrRecordServiceInfo;

	/* Only valid when wServiceType==[ PVR_Playback | (PVR_DRM for playback) ] */
	CA_PVR_SERVICE_INFO_T	stPvrPlaybackServiceInfo;

#ifdef COPY_CONTROL_SUBSTATUS_AVAILABLE
	CA_COPYCONTROL_INFO_T    stCopyControlInfo[CA_SERVICE_MAX_SERVICES];
#endif

	/* SERVICE_DVB_EMM specific info. */
	ia_word16		wEmmPid;
	ia_bool			fFree;

}CA_LOCAL_GROUP_SERVICE_INFO_T;

typedef struct
{
	/* wEmmHandle = wGroupHandle:
	You will always expect a SERVICE_DVB_DESCRAMBLE and SERVICE_DVB_EMM to use the same handle. */
	ia_word32		wEMMHandle;		/* it's a sequence number start from 0 */

	ia_word32		wDemuxId;			/* By default: = wGroupHandle */
	ia_word32		wMacrovisionId;	/* By default: = wGroupHandle */
	ia_word32		wOOBId;				/* By default: = wGroupHandle */
	ia_bool			fServiceDefined;	/* for EMM service */

	/* Multiple services can exist at the same time for a group.
	These sevices share the same resource IDs defined above. */
	CA_LOCAL_BASIC_SERVICE_INFO_T	astServices[SC_SLOT_MAX];
	/* SERVICE_DVB_EMM specific info. */
	ia_word16		wEMMPid;
	ia_bool			fFree;/*TRUE:This Demux is availble;FALSE:This Demux is ocupied*/
	/*Added by Andrew Hui For DMB*/
	CA_LOCAL_BASIC_DMB_INFO_T astDMBServices[SC_SLOT_MAX];
} CA_LOCAL_DEMUX_INFO_T;


typedef struct
{
	ia_word32		wGroupHandle;
	ia_word32		wServiceType[CA_SERVICE_MAX_SERVICES];
	ia_word16       wServiceHandle[CA_SERVICE_MAX_SERVICES];
	ia_status_st	astGlobalStatus[CA_SERVICE_MAX_SERVICES];
}CA_LOCAL_ERROR_INFO_T;

/***** CA_QC_MONITOR_EMM Query Control *****/
typedef struct
{
	ia_word32	wGroupHandle;
	ia_word16   wServiceHandle;
	/* abEmmData is deprecated - do not use abEmmData from version 3.2.6 PVCS 23471 */
	/* abEcmData is deprecated - do not use abEcmData from version 3.4.2 PVCS 25272 */
	ia_char     acEmmString[MAX_EMM_MONITOR_LEN];
	ia_char		acEcmString[MAX_ECM_MONITOR_LEN];
}CA_LOCAL_MONITOR_T;


/*
typedef struct
{
CA_IPPV_EVENTS_LIST_T stCaIppvEventsList;
void		*next;
}CA_LOCAL_IPPV_EVENTS_LIST;
*/

typedef struct
{
	ia_word32	wResourceId;
	ia_byte *pbSectorType;	/* identify sector support PPT or IPPV */
}CA_SCARD_SECTOR_TYPE_T;

typedef struct
{
	ia_word32	wResourceId;
	ia_char	acHashNo[MAX_SC_SER_NO_LEN];
}CA_SCARD_HASHID_T;


typedef struct
{
	ia_word32	wResourceId;
	ia_byte	abUserData[USER_DATA_LENGTH];
}CA_SCARD_USER_DATA_T;

typedef struct
{
	ia_word32  wResourceId;
	ia_status_st stSCartStatus;
}CA_SCARD_STATUS_T;

/* jyou */
/* Updates for ECT3.4-1 Smartcard Diagnose */
/* definition of the local store for smartcard information */
#define CA_SCARD_NATIONALITY_LENGTH			3
#define CA_SCARD_MAX_VENDOR_COUNT			4
#define CA_SCARD_MAX_SECTOR_COUNT			4
#define CA_SCARD_MAX_SERIAL_NUMBER_LENGTH	15
#define CA_SCARD_PPT_LABEL_LENGTH			11

typedef struct
{
	ia_word16		wID;
	ia_bool			fReported;
	ia_word16		wCost;
	ia_word16		wDateCode;
}CA_LOCAL_IPPV_EVENT_INFO_T;

typedef struct
{
	ia_byte			abPPTLabel[CA_SCARD_PPT_LABEL_LENGTH];
	ia_word16		wViewingTimeLeft;
	ia_byte			bPPTStatus;
}CA_LOCAL_PPT_V5_INFO_T;

typedef struct
{
	ia_word16		wID;
	ia_word16		wStartDate;
	ia_byte			bDuration;
}CA_LOCAL_PRODUCT_INFO_T;

typedef struct
{
	/* Sector general information */
	ia_byte					bSectorID;
	ia_bool					fSectorValid;
	ia_byte					abSectorAddress[SECTOR_ADDRESS_LENGTH];
	ia_word16				wDateCode;
	ia_byte					bRegion;
	ia_byte					bSubRegion;
	ia_word16				wOperatorPurse;
	ia_word16				wCustomerPurse;
	ia_word16				wDebitLimit;
	ia_word16				wDebitThreshold;
	/* Sector list information */
	ia_byte					bActive;
	ia_byte					bReserved;

	/* Products information--MSG_PRODUCT_LIST_REPLY */
	ia_byte					bProductCount;
	CA_LOCAL_PRODUCT_INFO_T	*pstProducts;

	/* Products information--MSG_EXTENDED_PRODUCT_LIST_REPLY */
	ia_byte					bExProductCount;
	CA_LOCAL_PRODUCT_INFO_T	*pstExProducts;

	/* IPPV events information--MSG_IPPV_EVENT_LIST_REPLY */
	ia_byte					bEventCount;
	CA_LOCAL_IPPV_EVENT_INFO_T	*pstIPPVEvents;

	/* Pay-per-time (PPT) information--MSG_PPT_INFO_REPLY */
	ia_byte					bPPTRecordCount;
	CA_LOCAL_PPT_V5_INFO_T	*pstPPTRecords;

	/*Operator ID information--MSG_SECTOR_OPERATORID_REPLY */
	ia_word16				wOperatorID;

	/* jyou: 3.4.2 STB reporting */
	/* IPPV Callback request */
	ia_byte		bPhoneNumberLength;
	ia_byte		bPhoneControl;
	ia_word16	wDelay;
	ia_byte		bDelayMultiplier;
	ia_byte		*pabPhoneNumber;

}CA_LOCAL_SECTOR_INFO_T;

typedef struct
{
	ia_word16			wVendorID;
	ia_bool				fActive;
} CA_LOCAL_VENDOR_ID_T;

typedef struct
{
	msg_mr_config_query_st pstMRConfigQuery;
	msg_mr_config_data_st pstMRConfigData;
}CA_LOCAL_MR_MSG_CONFIG_T;

typedef struct
{
	msg_mr_config_reply_st pstMRConfigReply;
	msg_mr_config_data_st pstMRConfigData;
}CA_LOCAL_MR_MSG_CONFIG_REPLY_T;

typedef struct
{
	/* General information */
	ia_word32				wResourceID;
	/* Homing Channel--QC_SMARTCARD_ACCESS */
	ia_byte					bHomingChannelAccessOp;
	ia_byte					bHomingChannelLength;
	ia_byte					*pbHomingChannelData;
	/* QC_SMARTCARD_UNIQUE_ADDRESS */
	ia_byte					bEMMFilterNumber;
	ia_byte					abUniqueAddress[UNIQUE_ADDRESS_LENGTH];
	/* QC_SMARTCARD_USER_DATA*/
	ia_byte					abUserData[USER_DATA_LENGTH];

	/* Hash ID--QC_Hash_ID */
	ia_word32				wClientHandle;
	ia_byte					bHashedNoLen;
	ia_byte					acHashNo[MAX_SC_SER_NO_LEN];

	/* ayang 2008-04-09 Changed ia_char to ia_byte */
	ia_byte					acNationality[CA_SCARD_NATIONALITY_LENGTH];
	ia_char					acSerialNumber[CA_SCARD_MAX_SERIAL_NUMBER_LENGTH];
	ia_byte					bNumOfVendorIDs;
	CA_LOCAL_VENDOR_ID_T	astVendorIDs[CA_SCARD_MAX_VENDOR_COUNT];

	/* Patch information */
	ia_word16				wOwnerID;
	ia_byte					bMajorVersion;
	ia_byte					bMinorVersion;
	ia_byte					bBuild;
	ia_byte					bVariant;
	ia_word16				wPatchLevel;

	/* Sector information */
	ia_byte					bSectorCount;
	CA_LOCAL_SECTOR_INFO_T	*pstSectors;
	ia_byte					*pbSectorType;	/* identify sector support PPT or IPPV */
	/*CA_LOCAL_SECTOR_INFO_T	astSectors[CA_SCARD_MAX_SECTOR_COUNT];*/

	/* additional information ECT Client does not interested */
	ia_byte					bCheckDigit;
	ia_status_st			stSmartCardStatus;
	ia_word16				wSmartCardType;
	ia_word16				wSmartCardGlobalStatus;
	/* CA_SMARTCARD_ID_T	stSmartCardID; the string a.k.a finger print */

	/* Remote channel--QC_REMOTE_CHANNEL */
	ia_byte					bRemoteChannelLength;
	ia_byte					*pbRemoteChannelPayload;

		/* SurfLocking */
	ia_bool					fSurfLocked;
	ia_word32				wGroupHandle;
	 /* Maturity rating */
	 ia_bool						 fMRcapable;
	 ia_byte	                abSubscriberPin[ 2 ];
	 CA_MR_CONFIG_ACTION_EN	 eConfigAction;
	 CA_MR_CONFIG_RESULT_EN  eConfigResult;
	 CA_MR_PROFILE_ACTION_EN eProfileAction;
	 ia_bool	                fMREnable;
	 CA_MR_USER_PROFILE_DATA_T stUserProfile;

} CA_LOCAL_SMARTCARD_INFO_T;

/* Add LPPV support */
typedef struct
{
		/* Sector general information */
		ia_byte     bSectorID;
		ia_byte     bSCPPVMode;

		/* Read ticket information */
		ia_byte     bTicketMode;
		ia_byte     bTicketCount;
		CA_LPPV_PPV_READ_TICKET_INFO_T *pstReadTickets;

		/* Credit information */
		ia_word16   wCredit;
		ia_word16   wCurrentDC;
		ia_word16   wExpiryDC;
		ia_bool     fClearOnExpiry;
		ia_word16   wLastIncreaseCredits;
		ia_word16   wLastIncreaseDC;

		/* Order information  */
		ia_byte     bReadOrderStatus;
		ia_byte     bOrderCount;
		CA_LPPV_ORDER_INFO_T *pstOrders;

}CA_LOCAL_LPPV_SECTOR_INFO_T;

typedef struct
{
		/* General information */
		vd_rsc_id   wResourceId;
		/* LPPV Mode information */
		ia_byte     bSTBPPVMode;
		ia_byte     bSCPPVMode;
		/* Sector information */
		ia_byte     bSectorCount;
		CA_LOCAL_LPPV_SECTOR_INFO_T *pstSectorInfo ;
		/* Preview information */
		ia_byte     bPreviewSectorId;
		ia_byte     bPreviewWindow;
		ia_byte     bBuyWindow;
		ia_byte     bEventTicketCount;
		CA_LPPV_PPV_EVENT_TICKET_T *pstEventTickets;
} CA_LOCAL_LPPV_SCARD_INFO_T;


typedef struct
{
	/*ia_uint32	wScRscId; jyou: unecessary*/
	ia_bool		fEmmServiceReady;
	ia_byte		*pbCatInfo;
	ia_byte		*pbNitInfo;
	ia_bool		fCatSent;
	ia_bool     fGetPMT;
	ia_bool     fHasOOB;
} DEMUX_INFO_ST;

typedef struct
{
	ia_bool		fClear;
	ia_uint32	wScRscId;
	ia_bool		fEmmServiceReady;
	ia_bool		fDesServiceReady;
	ia_bool		fEcmSrcAdded;
	ia_byte		*pbPmtInfo;
}SERVICE_INFO_ST; /* sren: moved from ca_task.h */

typedef struct
{
	ia_word32   GroupHandle;
	ia_word16   originalNetworkID;
	ia_word16   transportID;
	ia_word16   ServiceID;
} TS_NIT_INFO;

/* local display messages for query controls */
static const char* astrSCellMsgDisplay[200] = {
	0,
	/* Client identification. */
	"MSG_CLIENT_VERSION_QUERY               ", /* = 1 */
	"MSG_COMPONENTS_VERSION_QUERY           ", /* = 2 */
	"MSG_VERSION_REPLY                      ", /* = 3 */

	/* Generic errors. */
	"MSG_ERROR                              ", /* = 4 */

	/* Service control messages. */
	"MSG_SERVICE_OPEN_REQUEST               ", /* = 5 */
	"MSG_SERVICE_OPEN_REPLY                 ", /* = 6 */
	"MSG_SERVICE_CLOSE                      ", /* = 7 */
	/* "MSG_SERVICE_CLOSE_REPLY ", = 112 defined below. */
	"MSG_SERVICE_DEFINE                     ", /* = 8 */
	"MSG_SERVICE_STATUS_REQUEST             ", /* = 9 */
	"MSG_SERVICE_STATUS                     ", /* = 10 */

	/* Smartcard messages - Identification. */
	"MSG_SMARTCARD_STATUS                   ", /* = 11 */
	"MSG_NATIONALITY_QUERY                  ", /* = 12 */
	"MSG_NATIONALITY                        ", /* = 13 */
	"MSG_SMARTCARD_ID                       ", /* = 14 */
	"MSG_SMARTCARD_DATA_QUERY               ", /* = 15 */
	"MSG_SMARTCARD_DATA_REPLY               ", /* = 16 */
	"MSG_SMARTCARD_HOMING_CH_QUERY          ", /* = 17 */
	"MSG_SMARTCARD_HOMING_CH_REPLY          ", /* = 18 */

	/* Decoder messages for the user. */
	"MSG_DECODER                            ", /* = 19 */

	/* Monitoring messages. */
	"MSG_DVB_EMM_MONITOR                    ", /* = 20 */
	"MSG_DVB_ECM_MONITOR                    ", /* = 21 */
	"MSG_IPTV_EMM_MONITOR                   ", /* = 22 */
	"MSG_IPTV_REGISTRATION_ATTEMPTS         ", /* = 23 */

	/* TMS info queries and replies. */
	"MSG_SERVICE_INFO_QUERY                 ", /* = 24 */
	"MSG_SERVICE_INFO_REPLY                 ", /* = 25 */
	"MSG_TRANSPORT_INFO_QUERY               ", /* = 26 */
	"MSG_TRANSPORT_INFO_REPLY               ", /* = 27 */
	"MSG_NETWORK_INFO_QUERY                 ", /* = 28 */
	"MSG_NETWORK_INFO_REPLY                 ", /* = 29 */

	/* IPPV messages. */
	"MSG_IPPV_EVENT_NOTIFY                  ", /* = 30 */
	"MSG_IPPV_BUY_QUERY                     ", /* = 31 */
	"MSG_IPPV_BUY_REPLY                     ", /* = 32 */
	"MSG_IPPV_SECTOR_INFORMATION_QUERY      ", /* = 33 */
	"MSG_IPPV_SECTOR_INFORMATION_REPLY      ", /* = 34 */
	"MSG_IPPV_SET_DEBIT_THRESHOLD           ", /* = 35 */
	"MSG_IPPV_CALLBACK_REQUEST              ", /* = 36 */
	"MSG_IPPV_EVENT_LIST_QUERY              ", /* = 37 */
	"MSG_IPPV_EVENT_LIST_REPLY              ", /* = 38 */
	"MSG_IPPV_EVENT_CHECK_QUERY             ", /* = 39 */
	"MSG_IPPV_EVENT_CHECK_REPLY             ", /* = 40 */

	/* Smartcard messages - PIN Management. */
	"MSG_PIN_CODE_QUERY                     ", /* = 41 */
	"MSG_PIN_CODE_REPLY                     ", /* = 42 */

	/* Smartcard messages - Sector Information. */
	"MSG_NUMBER_OF_SECTORS_NOTIFY           ", /* = 43 */

	/* Smartcard messages - Remote channel to card. */
	"MSG_REMOTE_MESSAGE_QUERY               ", /* = 44 */
	"MSG_REMOTE_MESSAGE_REPLY               ", /* = 45 */

	/* Smartcard messages - Get CAM user data. */
	"MSG_SMARTCARD_USER_DATA_QUERY          ", /* = 46 */
	"MSG_SMARTCARD_USER_DATA_REPLY          ", /* = 47 */

	/* Reserved message, for future use. */
	"MSG_RFU1                               ", /* = 48 */

	/* Registration service info. */
	"MSG_REGISTRATION_USE_NEW_HEADEND       ", /* = 49 */

	/* Smartcard RSA crypto. */
	"MSG_SMARTCARD_RSA_SIGN_REQUEST         ", /* = 50 */
	"MSG_SMARTCARD_RSA_SIGN_REPLY           ", /* = 51 */
	"MSG_SMARTCARD_RSA_VERIFY_REQUEST       ", /* = 52 */
	"MSG_SMARTCARD_RSA_VERIFY_REPLY         ", /* = 53 */
	"MSG_SMARTCARD_RSA_ENCRYPT_REQUEST      ", /* = 54 */
	"MSG_SMARTCARD_RSA_ENCRYPT_REPLY        ", /* = 55 */
	"MSG_SMARTCARD_RSA_DECRYPT_REQUEST      ", /* = 56 */
	"MSG_SMARTCARD_RSA_DECRYPT_REPLY        ", /* = 57 */

	/* Product interface messages. */
	"MSG_PRODUCT_CHECK_QUERY                ", /* = 58 */
	"MSG_PRODUCT_CHECK_REPLY                ", /* = 59 */
	"MSG_ANY_PRODUCT_QUERY                  ", /* = 60 */
	"MSG_ANY_PRODUCT_REPLY                  ", /* = 61 */
	"MSG_PRODUCT_LIST_QUERY                 ", /* = 62 */
	"MSG_PRODUCT_LIST_REPLY                 ", /* = 63 */

	/* ClientId messages. DEFUNCT! DO NOT USE! */
	"MSG_CLIENT_ID_QUERY                    ", /* = 64 */
	"MSG_CLIENT_ID_REPLY                    ", /* = 65 */

	/* CypherCast Service Info messages. */
	"MSG_CC_SERVICE_ADD                     ", /* = 66 */
	"MSG_CC_SERVICE_DELETE                  ", /* = 67 */
	"MSG_CC_SERVICE_UPDATE                  ", /* = 68 */

	/* Product interface messages (extension). */
	"MSG_EXTENDED_PRODUCT_LIST_QUERY        ", /* = 69 */
	"MSG_EXTENDED_PRODUCT_LIST_REPLY        ", /* = 70 */

	/* Smartcard messages - Pay Per Time. */
	"MSG_PPT_INFO_QUERY                     ", /* = 71 */
	"MSG_PPT_INFO_REPLY                     ", /* = 72 */

	/* Smartcard messages - Surf Locking. */
	"MSG_SURF_LOCK_QUERY                    ", /* = 73 */
	"MSG_SURF_LOCK_REPLY                    ", /* = 74 */

	/* Smartcard messages - Get Region. */
	"MSG_REGION_QUERY                       ", /* = 75 */
	"MSG_REGION_REPLY                       ", /* = 76 */

	/* Product interface messages (expiration). */
	"MSG_EXPIRED_PRODUCT_QUERY              ", /* = 77 */
	"MSG_EXPIRED_PRODUCT_REPLY              ", /* = 78 */

	/* IPDC Monitoring */
	"MSG_IPDC_EMM_MONITOR                   ", /* = 79 */
	"MSG_IPDC_ECM_MONITOR                   ", /* = 80 */

	/* Registration Parameter messages. */
	"MSG_REGISTRATION_PARAMETER_QUERY       ", /* = 81 */
	"MSG_REGISTRATION_PARAMETER_REPLY       ", /* = 82 */

	/* SoftClient Download proxy. DEFUNCT! DO NOT USE! */
	"MSG_SC_DOWNLOAD_PROXY_QUERY            ", /* = 83 */
	"MSG_SC_DOWNLOAD_PROXY_REPLY            ", /* = 84 */

	/* SoftClient Security proxy. DEFUNCT! DO NOT USE!*/
	"MSG_SC_SECURITY_PROXY_QUERY            ", /* = 85 */
	"MSG_SC_SECURITY_PROXY_REPLY            ", /* = 86 */

	/* SoftClient card-ip proxy. DEFUNCT! DO NOT USE!*/
	"MSG_SC_CARDIP_PROXY_QUERY              ", /* = 87 */
	"MSG_SC_CARDIP_PROXY_REPLY              ", /* = 88 */

	"MSG_SC_MONITOR_PROXY_QUERY             ", /* = 89 */
	"MSG_SC_MONITOR_PROXY_REPLY             ", /* = 90 */

	/* IPDC CBMS ESG Purchase Data messages. */
	"MSG_EVALUATE_PURCHASE_DATA_QUERY       ", /* = 91 */
	"MSG_EVALUATE_PURCHASE_DATA_REPLY       ", /* = 92 */
	"MSG_ENTITLEMENT_UPDATE_TRIGGER         ", /* = 93 */

	"MSG_DRM_SYSTEM_QUERY                   ", /* = 94 */
	"MSG_DRM_SYSTEM_REPLY                   ", /* = 95 */

	"MSG_SMARTCARD_UNIQUE_ADDRESS_QUERY     ", /* = 96 */
	"MSG_SMARTCARD_UNIQUE_ADDRESS_REPLY     ", /* = 97 */
	"MSG_SMARTCARD_VENDOR_ID_QUERY          ", /* = 98 */
	"MSG_SMARTCARD_VENDOR_ID_REPLY          ", /* = 99 */

	"MSG_SMARTCARD_SECTOR_ADDRESS_QUERY     ", /* = 100 */
	"MSG_SMARTCARD_SECTOR_ADDRESS_REPLY     ", /* = 101 */
	"MSG_SMARTCARD_SECTOR_DATECODE_QUERY    ", /* = 102 */
	"MSG_SMARTCARD_SECTOR_DATECODE_REPLY    ", /* = 103 */

	/* OOB emm. */
	"MSG_OOB_EMM_DOWNLOAD                   ", /* = 104 */
	"MSG_OOB_EMM_RESPONSE                   ", /* = 105 */

	/* Smartcard messages - Operator Id. */
	"MSG_SMARTCARD_SECTOR_OPERATORID_QUERY  ", /* = 106 */
	"MSG_SMARTCARD_SECTOR_OPERATORID_REPLY  ", /* = 107 */

	/* Smartcard messages - Marriage status. */
	"MSG_SMARTCARD_MARRIAGE_STATUS_QUERY    ", /* = 108 */
	"MSG_SMARTCARD_MARRIAGE_STATUS_REPLY    ", /* = 109 */

	/* DMB Monitoring. */
	"MSG_DMB_EMM_MONITOR                    ", /* = 110 */
	"MSG_DMB_ECM_MONITOR                    ", /* = 111 */

	/* Service control messages (extension). */
	"MSG_SERVICE_CLOSE_REPLY                ", /* = 112 */

	0, 0, 0, 0, 0, 0, 0,
	/* Smartcard messages - Multiple smartcard state notification. */
	"MSG_SMARTCARD_MULTICARD_STATE_NOTIFY   ", /* = 120 */

	/* LPPV messages */
	"MSG_LPPV_AWARE_QUERY                   ", /* = 121 */
	"MSG_LPPV_AWARE_REPLY                   ", /* = 122 */
	"MSG_LPPV_EVENT_NOTIFY                  ", /* = 123 */
	"MSG_LPPV_READ_TICKET_QUERY             ", /* = 124 */
	"MSG_LPPV_READ_TICKET_REPLY             ", /* = 125 */
	"MSG_LPPV_DELETE_TICKET_QUERY           ", /* = 126 */
	"MSG_LPPV_DELETE_TICKET_REPLY           ", /* = 127 */
	"MSG_LPPV_READ_CREDIT_QUERY             ", /* = 128 */
	"MSG_LPPV_READ_CREDIT_REPLY             ", /* = 129 */
	"MSG_LPPV_READ_ORDER_QUERY              ", /* = 130 */
	"MSG_LPPV_READ_ORDER_REPLY              ", /* = 131 */
	"MSG_LPPV_PLACE_ORDER_QUERY             ", /* = 132 */
	"MSG_LPPV_PLACE_ORDER_REPLY             ", /* = 133 */
	"MSG_LPPV_CANCEL_ORDER_QUERY            ", /* = 134 */
	"MSG_LPPV_CANCEL_ORDER_REPLY            ", /* = 135 */

	"MSG_EMM_TLV_NOTIFY                     ", /* = 136 */

	/* Maturity Rating. */
	"MSG_MATURITY_RATING_CAPABILITY_QUERY   ", /* = 137 */
	"MSG_MATURITY_RATING_CAPABILITY_REPLY   ", /* = 138 */
	"MSG_MATURITY_RATING_CONFIG_QUERY       ", /* = 139 */
	"MSG_MATURITY_RATING_CONFIG_REPLY       ", /* = 140 */
	"MSG_MATURITY_RATING_USER_PROFILE_QUERY ", /* = 141 */
	"MSG_MATURITY_RATING_USER_PROFILE_REPLY ", /* = 142 */

	/* Usage Monitoring. */
	"MSG_UMC_CONFIG                         ", /* = 143 */
	"MSG_UMC_TRIGGER                        ", /* = 144 */

	"MSG_REGION_FILTER_CONFIG               ", /* = 145 */
	"MSG_REGION_FILTER_SET_INFO             ", /* = 146 */

	"MSG_HGPC_SMARTCARD_DATA_QUERY          ", /* = 147 */
	"MSG_HGPC_SMARTCARD_DATA_REPLY          ", /* = 148 */
	"MSG_HGPC_SCELL_DATA_QUERY              ", /* = 149 */
	"MSG_HGPC_SCELL_DATA_REPLY              ", /* = 150 */
	"MSG_HGPC_MESSAGE_STATUS                ", /* = 151 */
	"MSG_HGPC_EVENT_NOTIFY                  ", /* = 152 */

	/* CMMB Monitoring */
	"MSG_CMMB_EMM_MONITOR                   ", /* = 153 */
	"MSG_CMMB_ECM_MONITOR                   ", /* = 154 */

	"MSG_IPPV_PHONE_NUMBER_QUERY            ", /* = 155 */
	"MSG_IPPV_PHONE_NUMBER_REPLY            "  /* = 156 */

};

/********** Feature 3.4-1 Definitions End **********/

/* tools to access the local smartcard info storage */
CA_LOCAL_SMARTCARD_INFO_T* CA_LOCAL_GetLocalSCardInfo( void );
ia_byte CA_LOCAL_AddSmartCardInfo(ia_word32 wResourceID);
ia_byte	CA_LOCAL_FindSmartCardLocation(ia_word32 wResourceID);
void	CA_LOCAL_ClearAllSmartCardLocation( void );
void	CA_LOCAL_ClearSmartCardLocation(ia_word32 wResourceID);
CA_LOCAL_SMARTCARD_INFO_T* CA_LOCAL_FindSmartCard( ia_word32 wResourceID );
CA_LOCAL_SECTOR_INFO_T* CA_LOCAL_FindSmartCardSector( ia_word32 wResourceID, ia_byte bSectorID );
void CA_LOCAL_SetSurflockStatus( ia_byte wIndex, ia_bool fStatus );

/* numeric conversion tools */
ia_word16 CA_LOCAL_MakeWord16( ia_word16 wData1, ia_word16 wData2 );
ia_word32 CA_LOCAL_MakeWord32( ia_word16 wData1, ia_word16 wData2 );
ia_word16 CA_LOCAL_GetUpperBytes( ia_word32 wData, ia_byte bNumBytes );
ia_word16 CA_LOCAL_GetLowerBytes( ia_word32 wData, ia_byte bNumBytes );

ia_word32 CA_LOCAL_GetSmartCardIdFromSectorMsg(CA_SCELL_MSG *pstMsg);

/* Debugging functions */
/* void CA_DBG( char* format, ... ); */
/* void CA_ERROR( char* format, ... ); */
/* void CA_DBG_V( char* format, ... ); */

/* CA "Local" functions */
void CA_CLIENT_Notify( ia_word32 wHandle, CA_QUERY_CONTROL_EN eAction, ia_uint32 ulParam );
ia_bool CA_TASK_CheckNotifyEnable( ia_word32 wMask );
ia_bool CA_TASK_IsClearProgram( ia_byte *pbPMTData );

ia_byte CA_SCARD_GetSectorNumber(ia_word32 wResourceId);
void CA_SCARD_SetSectorType(ia_word32 wResourceId, ia_byte bIndex, ia_byte bType);
ia_byte CA_SCARD_GetSectorType(ia_word32 wResourceId, ia_byte bIndex);

void CA_SCARD_SetSmartCardStatus(ia_word32 wResourceId, ia_status_st *pStatus);
ia_result CA_SCARD_GetSmartCardStatus( ia_word32 wResourceId, ia_status_st *pScardStatus);
CA_INIT_CONFIG_ST* CA_TASK_GetInitCfg( void );
void CA_DMB_AddEmmResourceToEmmService( ia_word32 wOpSeqNo);
void SendServiceOpenRequestMsg( ia_word16 wGroupHandle, ia_word16 wServiceType );
typedef void (*CA_LOCAL_ParseReplyMsg)( const CA_SCELL_MSG *pstSCellMsg );
typedef CA_QUERY_STATUS_EN (*CA_LOCAL_QueryControl)( ia_word32 wHandle, void *pCAClientMsg );
/* Global variable access */
void CA_Set_ScellMsgFlag(ia_byte bFlag);

/* ayang 2008-7-29 Fix SCR#27360,27365 */
ia_bool ServiceHandleMechanism( ca_drv_message_type_e eOpcode, ia_word32 wGroupHandle, ia_word32 wSrvType, void *pvMsg );
#endif /* _CA_LOCAL_H_ */

