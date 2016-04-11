/**
* $Id: ca_task.c,v 1.85 2008/11/12 02:50:57 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* The main module of Embedded CA Task 3.
**/

/******************************** Header Files ********************************/
/* #include <stdarg.h> */
/* #include <stdio.h> */
/* #include <string.h> */
#include <irca/pubh/s3_ansi_functions.h>
#include <irca/pubh/s3_portable_types.h>
#include <irca/ca_task.h>
#include "ca_error.h"
#include "ca_scard.h"
#include "ird_msg.h"
#include "ca_mail.h"
#include "ca_monitor.h"
#include "ca_svcinfo.h"
#include "ca_ippv.h"
#include "ca_ppt.h"
#include "ca_lppv.h"
#include "ca_rf.h"
#include "ca_hgpc.h"
#include "ca_surflock.h"
#include "ca_product.h"
#include "ca_svcinfo.h"

#include <irca/pubh/s3_osdrv.h>
#include <irca/pubh/s3_cadrv.h>
#include <irca/pubh/s3_service.h>
#include <irca/pubh/s3_message_helper.h>
#include <irca/pubh/s3_smartcard.h>
#include <irca/pubh/s3_tm_msg.h>

#ifdef INCL_SERVICE_DMB
#include "ca_dmbsvcinfo.h"
#endif


/*********************************** Macros ***********************************/
#define MAX_GLOBAL_LAST_HANDLE_NUM   50

#ifdef _AUSTAR_
int	SynSendMessage = 0;
int	SynScMessage;
#endif

/****************************** Global Variables ******************************/

/******************************* Local Variables ******************************/

/******************************************************************************
* jyou: ECT 3.4-1	Smartcard Diagnose
******************************************************************************/

/*
NOTE: We use the OS_DRV for inter task communication.

The only data that has to be sent through the hosts message system are the
pointer to the data and the length of that data. As SoftCell requires that
memory must be shared between its tasks there is no need to send the data
itself.
The size of each message in this case is sizeof(void*) + sizeof(ia_word16).
- From doc#710467
*/

/* Message type ID for all messages coming to CATASK */
enum CA_MESSAGE_SET
{
	CADRV__SCELL_MESSAGE,
	CADRV__CA_TASK_MESSAGE
};

/* SoftCell -> CA Task message structure. */
/*typedef struct
{
ca_drv_message_type_e  eOpcode;
ia_word16              wOpSeqNo;
ia_word16              wLength;
void*                  pvMessage;
}CA_SCELL_MSG;*/

/* CA Client -> CA Task message structure. */
typedef struct
{
	MSG_ACTION_EN        eAction;
	void*                pstParams;
}CA_MSG_T;

/* any -> CA Task message. */
typedef struct
{
	enum CA_MESSAGE_SET enMsgType;
	union
	{
		CA_SCELL_MSG stSCellMsg;
		CA_MSG_T     stCaMsg;
	} u;
} CA_SCELL_MESSAGE_STRUCT;

static CA_INIT_CONFIG_ST m_stCaInitCfg ;

static CA_SOFTCELL_INFO_T stSoftCell = {0, 0, 0, "", "", ""};
static CA_COMPONENTS_INFO_T stComponents;

static os_drv_task_h caMessageQueue;
static CA_DBG_LEVEL_EN  enCaDebug = CA_DBG_NONE;
static CA_NOTIFICATION fpNotification = 0;

/* jyou: 3.4.3 DT Integration SCR#25808 2007-04-03 */
#ifdef NOTIFY_MASK_DEFAULT
static ia_word32 m_wNotifyEnable = NOTIFY_MASK_DEFAULT;
#else
static ia_word32 m_wNotifyEnable = NOTIFY_MASK_ENABLE_ALL;
#endif

static ia_word32 gwLastGrpHandle[MAX_GLOBAL_LAST_HANDLE_NUM];/*mli for sync of opening reply*/
static CA_RESOURCE_MAP_ST stResourceMap;

static DEMUX_INFO_ST /* astDMX[CA_DEMUX_MAX] */ *pstDMX = 0;
static SERVICE_INFO_ST /* astService[CA_DESC_SERVICE_MAX] */ *pstService = 0;
static TS_NIT_INFO     /* astTsNit[ CA_DEMUX_MAX ] */ *pstTsNit = 0;

static CA_DRM_PVR_RECORD_T *pstCaDrmPvrRecord = 0;
static CA_DRM_PVR_PLAYBACK_T *pstCaDrmPvrPlayback = 0;

static ia_byte        bServiceCount = 0;
static ia_bool        fTermination = IA_FALSE;
static ia_bool        fReadyCloseECT = IA_FALSE;

/* static ia_byte *pbSectorType = 0; */	/* identify sector support PPT or IPPV */

static ia_word16		oldCAPID = 0;

/* Fix SCR#26646: ayang 07/12/04 */
static ia_byte	bSCellMsgFlag = 1;
static ia_byte	bSCellMsgFlagStatic = 1;
static void ProcessSCellMsg( CA_SCELL_MSG *pstSCellMsg );
static os_drv_semaphore_h gSCellMsgSemap = 0;

/*[irca ect] pyh : pstSCellMsgStore为全局变量，可节省原本耗费的进程堆栈*/
#define STORE_MSG_NUM	(100)
static CA_SCELL_MSG	pstSCellMsgStore[STORE_MSG_NUM];

/************************* Local Function Declarations ************************/
static CA_QUERY_STATUS_EN SoftCellInfoQueryControl(   ia_word32 wHandle, void *pCAClientMsg );
static CA_QUERY_STATUS_EN ComponentsInfoQueryControl( ia_word32 wHandle, void *pCAClientMsg );
static CA_QUERY_STATUS_EN NotifyEnableQueryControl(   ia_word32 wHandle, void *pCAClientMsg );

static void CaTaskMain( void *argv );
static void CaProcessMsg( CA_SCELL_MESSAGE_STRUCT *pstCA_Msg );
static void SendClientVersionQueryMsg( void );
static void SendComponentsVersionQueryMsg( void );
static void ParseVersionReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
static void ParseErrorMsg( const CA_SCELL_MSG *pstSCellMsg );
static void ParseServiceOpenReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
static void ParseServiceCloseReplyMsg( const CA_SCELL_MSG *pstSCellMsg );

/* Map resource */
static void MapResources( ia_byte *pbMsg );
/* EMM service handlers */
static void DefineEMMServicesOnDemux( ia_byte *pbMsg, CA_MSG_T *pCaMsg );
static void StopEMMServicesOnDemux( ia_byte *pbMsg );
static void AddDefaultResourcesToEmmService( ia_word16 wOpSeqNo );
static void AddEmmSourceToEmmService( ia_word16 wOpSeqNo );

/* Descramble service handlers */
static void DefineDescrambleService(ia_word32 wGroupHandle, ia_word16 wDescLen, ia_byte *pbMsg, CA_MSG_T *pCaMsg);
static void AddEcmSourceToDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType);
void PrepareSmartCardResources( ia_word16 wResourceMgnt, ia_msg_handle pvMsg );
static void UpdateDefaultResourcesToDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType);
static void AddMRSourceToService(ia_word32 wGroupHandle, ia_word32 wSrvType);
static void AddGeneralConfigSourceToDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType);

static void ParseServiceStatusMsg( const CA_SCELL_MSG *pstSCellMsg );
static void ParseSmartcardStatusMsg( const CA_SCELL_MSG *pstSCellMsg );
static void CloseActiveProgram(ia_word32 wService);
static void ResetLocalServiceInfo(ia_word32 wService);

static void AddResourceToPvrRecordService(ia_word32 wGroupHandle);
static void AddResourceToPvrRecordServiceForDrm(ia_word32 wGroupHandle);
static void AddResourceToPvrPlayBackServiceForDrm(ia_word32 wGroupHandle);
static void AddResourceToPvrPlayBackService(ia_word32 wGroupHandle);
static void RemoveResourceFromPvrRecordService(ia_word32 wGroupHandle);
static void RemoveResourceFromPvrPlayBackService(ia_word32 wGroupHandle);

/* ayang: 2008-03-13 Fixed SCR#26933 */
static void CA_TMS_ReplyServiceMsg( const CA_SCELL_MSG *pstSCellMsg );
static void CA_TMS_ReplyTransportMsg( const CA_SCELL_MSG *pstSCellMsg );
static void CA_TMS_ReplyNetworktMsg( const CA_SCELL_MSG *pstSCellMsg );

static void CA_TASK_FreeAllocatedMemory( void );
static void DeleteAllResource( void );
static void ServicesClose(ia_word32 wIndex, ia_word32 wSrvType);

/* for OOb */
void AddOOBResourceToEmm(ia_word32 wGroupHandle);
void RemoveOOBResourceFromEmm(ia_word32 wGroupHandle);
static ia_bool fSoftCellRunning = IA_FALSE;   /* mli */

/* mli: 3.3.11 */
static os_drv_semaphore_h gECTStartUpSemap = 0;
#ifdef _AUSTAR_
static int SynInOutMessage = 0;
static int SynScStatusMessage =	0;
#endif

/* jyou: 3.4.3.6 SCR#26043 2007-06-01*/
static os_drv_semaphore_h gMessageQueueSemap = 0;

/* local reference to Reply Message parsing functions */
static CA_LOCAL_ParseReplyMsg afnParseReply[200] = {
	0,
	0, 0, /*MSG_CLIENT_VERSION_QUERY = 1,MSG_COMPONENTS_VERSION_QUERY = 2,*/
	&ParseVersionReplyMsg,
	&ParseErrorMsg,
	0, &ParseServiceOpenReplyMsg,
	0, 0, /*MSG_SERVICE_CLOSE= 7, MSG_SERVICE_DEFINE= 8,*/
	0, &ParseServiceStatusMsg,
	&ParseSmartcardStatusMsg,		/* =11 */
	0, &CA_SCARD_ParseNationalityMsg,
	&CA_SCARD_ParseSmartCardIdMsg,
	0, &CA_SCARD_ParseSmartCardDataReplyMsg,
	0, &CA_SCARD_ParseSmartCardHomingChReplyMsg,
	&IRD_MSG_ParseDecoderMsg,
	&CA_MONITOR_ParseEmmMonitorMsg,
	&CA_MONITOR_ParseEcmMonitorMsg,	/* =21 */
	0, 0, /*MSG_IPTV_REGISTRATION_ATTEMPTS    = 23, */
	/* ayang: 2008-03-13 Fixed SCR#26933 */
	&CA_TMS_ReplyServiceMsg, 0, 	/*MSG_SERVICE_INFO_REPLY            = 24, */
	&CA_TMS_ReplyTransportMsg, 0, 	/*MSG_TRANSPORT_INFO_REPLY          = 26, */
	&CA_TMS_ReplyNetworktMsg, 0, 	/*MSG_NETWORK_INFO_REPLY            = 28, */
	&CA_IPPV_EventNotify,
	0, &CA_IPPV_BuyNotify,			/* =32 */
	0, &CA_IPPV_SectorInfoNotify,
	0, &CA_IPPV_ParseCallBackRequestNotify,
	0, &CA_IPPV_EventsListNotify,
	0, &CA_IPPV_EventCheckNotify,
	0, &CA_SCARD_ParsePinCodeMsg,	/* =41 */
	&CA_SCARD_ParseNumberOfSectorsNotify,
	0, &CA_SCARD_ParseRemoteChannelReplyMsg,
	0, &CA_SCARD_ParseUserDataReplyMsg,
	0, 0, /*MSG_REGISTRATION_USE_NEW_HEADEND = 49*/
	0, 0, /*MSG_SMARTCARD_RSA_SIGN_REQUEST = 50, MSG_SMARTCARD_RSA_SIGN_REPLY = 51,*/
	0, 0, /*MSG_SMARTCARD_RSA_VERIFY_REQUEST = 52, MSG_SMARTCARD_RSA_VERIFY_REPLY = 53,*/
	0, 0, /*MSG_SMARTCARD_RSA_ENCRYPT_REQUEST = 54,	MSG_SMARTCARD_RSA_ENCRYPT_REPLY   = 55,*/
	0, 0, /*MSG_SMARTCARD_RSA_DECRYPT_REQUEST = 56,	MSG_SMARTCARD_RSA_DECRYPT_REPLY   = 57,*/
	0,
#ifdef INCL_PRODUCT_BLOCKING
	&CA_PRODUCT_CheckReplyMsg,
#else
	0,
#endif
#ifdef INCL_PRODUCT_BLOCKING
	&CA_ANY_PRODUCT_ReplyMsg, &CA_ANY_PRODUCT_ReplyMsg,	/* =60, 61 */
#else
	0, 0,
#endif
	0, &CA_PRODUCT_ListReplyMsg,
	0, 0, /*MSG_CLIENT_ID_QUERY = 64, MSG_CLIENT_ID_REPLY = 65,*/
	0, 0, 0, /*	MSG_CC_SERVICE_ADD = 66,MSG_CC_SERVICE_DELETE = 67,	MSG_CC_SERVICE_UPDATE = 68,*/
	0, &CA_PRODUCT_EXTENDED_PRODUCT_ListReplyMsg, /* =70 */
	0, &CA_PPT_INFOReplyMsg,
	0, &CA_SURFLOCK_ReplyMsg,
	0, &CA_SCARD_ParseRegionReplyMsg,
	0, &CA_PRODUCT_EXPIRED_PRODUCTReplyMsg,
	0, 0, /*MSG_IPDC_EMM_MONITOR = 79,MSG_IPDC_ECM_MONITOR= 80,*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* IPDC messages 81--90*/
	0, 0, 0,	/* IPDC CBMS ESG Purchase Data messages. */
	0, 0, /*MSG_DRM_SYSTEM_QUERY= 94,MSG_DRM_SYSTEM_REPLY= 95,*/
	0, &CA_SCARD_ParseSmartCardUniqueAddressReplyMsg,
	0, &CA_SCARD_ParseSmartCardVendorIDReplyMsg,
	0, &CA_SCARD_ParseSmartCardSectorAddressReplyMsg, /* = 101*/
	0, &CA_SCARD_ParseSmartCardSectorDatecodeReplyMsg,/* = 103*/
	0, 0, /* MSG_OOB_EMM_DOWNLOAD=104, MSG_OOB_EMM_RESPONSE=105,*/
	0, &CA_SCARD_ParseSmartCardOperatorIDReplyMsg,
#ifdef INCL_SERVICE_DMB
	0, 0, &CA_DMB_ParseEmmMonitorMsg,/*110*/
	&CA_DMB_ParseEcmMonitorMsg,/*111*/
#else
	0, 0, 0, 0,
#endif
	&ParseServiceCloseReplyMsg,/*112*/
	0, 0, 0, 0, 0,0,0,0, /* 113--120 */
	0, &CA_LPPV_AwareNotify, /* = 122 */
	&CA_LPPV_PreviewNotify,  /* = 123 */
	0, &CA_LPPV_ReadTicketNotify,  /* = 125 */
	0, &CA_LPPV_DeleteTicketNotify,/* = 127 */
	0, &CA_LPPV_ReadCreditNotify,  /* = 129 */
	0, &CA_LPPV_ReadOrderNotify,   /* = 131 */
	0, &CA_LPPV_PlaceOrderNotify,  /* = 133 */
	0, &CA_LPPV_CancelOrderNotify, /* = 135 */
	0,
	0, &CA_SCARD_MRCapabilityNotify,	/* = 138 */
	0, &CA_SCARD_MRConfigurationNotify,	/* = 140 */
	0, &CA_SCARD_MRProfileNotify,		/* = 142 */
	0, 0,
	0, 0,
	0, &CA_HGPC_SmartCardDataNotify,	/* = 148 */
	0, &CA_HGPC_SoftCellDataNotify,		/* = 150 */
	&CA_HGPC_MsgStatusNotify,			/* = 151 */
	&CA_HGPC_EventNotify,				/* = 152 */
	0, 0,
	0, &CA_IPPV_PhoneNumber_Notify		/* = 155 */
};

ia_bool CA_TestDebugError( void )
{
	if (enCaDebug >= CA_DBG_CRITICAL)
	{
		return IA_TRUE;
	}
	else
	{
		return IA_FALSE;
	}
}

ia_bool CA_TestDebug( void )
{
	if (enCaDebug >= CA_DBG_MSG)
	{
		return IA_TRUE;
	}
	else
	{
		return IA_FALSE;
	}
}

ia_bool CA_TestDebugDVError( void )
{
	if (enCaDebug >= CA_DBG_VERBOSE)
	{
		return  IA_TRUE;
	}
	else
	{
		return IA_FALSE;
	}
}

static ia_result GetMsgHeader(ia_byte *pbData, ia_word32 *pwHeader, ia_char bHeaderSize)
{
	ia_byte i = 0,j = 0;
	if (pwHeader == 0 || pbData == 0)
		return IA_FAIL;
	while (i < bHeaderSize)
	{
		pwHeader[j] = (ia_word32)(( (ia_word32)pbData[i] << 24 ) | ( (ia_word32)pbData[i+1] << 16 ) | ( (ia_word32)pbData[i+2] <<  8 ) | ( (ia_word32)pbData[i+3] ) );
		i += 4;
		j++;
	}
	return IA_SUCCESS;
}


/*
* Description: Initializes the CA Task. Starts necessary tasks and queues,
*              SoftCell will be started as a result of this call.
*
* Parameters : eDebug - Debug output level.
*
* Returns    : IA_TRUE - init OK.
*/
ia_bool CA_TASK_Init(CA_DBG_LEVEL_EN eDebug, void *pstData)
{
	ia_result scellReturn;
	ia_bool scellInit = IA_TRUE;
	//[irca ect] pyh
	int i;

	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */
	enCaDebug = eDebug;

	fTermination = IA_FALSE;

	switch(enCaDebug)
	{
	case CA_DBG_MSG:
		ansi_sprintf((char *)stSoftCell.acCaTaskVerString, "%s Dbg", CA_TASK_VERSION );
		break;

	case CA_DBG_VERBOSE:
		ansi_sprintf((char *)stSoftCell.acCaTaskVerString, "%s DbgV", CA_TASK_VERSION );
		break;

	case CA_DBG_NONE:
	case CA_DBG_CRITICAL:
	default:
		/* Don't append anything */
		ansi_sprintf((char *)stSoftCell.acCaTaskVerString, "%s", CA_TASK_VERSION );
		break;
	}
	CA_DBG(("CA TASK : %s\n", stSoftCell.acCaTaskVerString));

	CA_DBG(("+++ Initialise CA TASK, stack size %d, Queue depth %d.\n", CA_TASK_STACK, CA_TASK_QUEUE_DEPTH));

	/* ayang: move the code here to solve SCR#26200 */
	/* jyou: 3.4.3.6 SCR#26043 2007-06-01 */
	gMessageQueueSemap = OS_DRV_CreateSemaphore( 1 );
	if ( OS_DRV_SEMAPHORE_ERROR == gMessageQueueSemap )
	{
		CA_ERROR(("Message queue semaphore creation failed.\n" ));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_RESOURCE);
		return IA_FALSE;
	}
	/* jyou: SCR#26234 removed the unnecessary signalling */
	/* ayang: SCR#26200 */

	/* Fix SCR#26646: ayang 07/12/04 */
	gSCellMsgSemap = OS_DRV_CreateSemaphore( 1 );
	if ( OS_DRV_SEMAPHORE_ERROR == gSCellMsgSemap)
	{
		CA_ERROR(("SoftCell Message semaphore creation failed.\n" ));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_RESOURCE);
		return IA_FALSE;
	}
	/* SCR#26646: ayang 07/12/04 */

	/* Fix SCR#27405: Adjust some code in ECT initialize routine. */
	gECTStartUpSemap = OS_DRV_CreateSemaphore(0);
	if (OS_DRV_SEMAPHORE_ERROR == gECTStartUpSemap )
	{
		CA_ERROR(("<208> Semap create failed\n" ));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_RESOURCE);
		return IA_FALSE;
	}

	//[irca ect] pyh
	for (i=0;i<STORE_MSG_NUM;i++)
	{
		pstSCellMsgStore[i].pvMessage = 0;
	}
	
	caMessageQueue = OS_DRV_CreateTask(CaTaskMain, pstData, CA_TASK_PRIORITY,
		CA_TASK_STACK,
		CA_TASK_QUEUE_DEPTH);

	//osal_task_sleep(2000);

	//DBG_DRV_Printf("caMessageQueue = %d\n",caMessageQueue);
	if( caMessageQueue == OS_DRV_TASK_ERROR )
	{
		CA_ERROR(("ERROR INITIALISING CA TASK\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_TASK);
		scellInit = IA_FALSE;
	}

	scellReturn = SCELL_Initialise( );
	if( scellReturn != IA_SUCCESS )
	{
		CA_ERROR( ("SCELL INIT ERROR : %d\n", scellReturn) );
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_INIT);
		scellInit = IA_FALSE;
	}
	fSoftCellRunning = IA_TRUE;

	OS_DRV_WaitSemaphore(gECTStartUpSemap);

	return scellInit;
}

/*
* Description: This function is provided for the manufacturer to send a message
*              the CA Task.
*
* Parameters : eAction - The ID of the CA Client message to be sent.
*              pvParams - Related parameters.
*
* Returns    : IA_TRUE  - Successful.
*              IA_FALSE - Failed.
*/
ia_bool CA_TASK_SendMessage( MSG_ACTION_EN eAction, void *pvParams )
{
	CA_SCELL_MESSAGE_STRUCT *osMsg = 0;
	ia_word16  wMsgLen = 0;
	ia_word32 awServiceGroupDefineHeader[CA_CLIENT_SERVICE_GROUP_DEFINE_HEADER_SIZE];
	ia_word32 awServiceStopHeader[CA_CLIENT_SERVICE_STOP_HEADER_SIZE];
	ia_word32 wSrvType = 0;
	ia_byte  *pbMsg = 0, *pbLocalCopy = 0;
	ia_result enRet;

	if( pvParams == 0 )
	{
		CA_ERROR(("A null pointer.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_POINTER);
		return IA_FALSE;
	}
	pbMsg = (ia_byte *)pvParams;
	wMsgLen = (ia_word16)((ia_word16)(pbMsg[0]<<8) | (ia_word16)pbMsg[1]);
	ansi_memset((void *)awServiceGroupDefineHeader, 0, sizeof(ia_byte) * CA_CLIENT_SERVICE_GROUP_DEFINE_HEADER_SIZE);
	pbMsg += 2;
	CA_DBG(("+++ CA TASK wMsgLen = %d\n", wMsgLen));

	if (eAction == CA_CLIENT__DEFINE_SERVICE_GROUP)
	{
		if (GetMsgHeader(pbMsg, awServiceGroupDefineHeader, CA_CLIENT_SERVICE_GROUP_DEFINE_HEADER_SIZE) == IA_SUCCESS)
		{
			wSrvType = awServiceGroupDefineHeader[CA_CLIENT_SERVICE_GROUP_DEFINE_SERVICESTYPE];
			CA_DBG(("+++ CA TASK SrvType = %d\n", wSrvType));
		}
		else
		{
			CA_ERROR(("Can't get the CA_CLIENT__DEFINE_SERVICE_GROUP message header.\n"));
			return IA_FALSE;
		}
	}
	if (eAction == CA_CLIENT__STOP_SERVICE_GROUP)
	{
		if (GetMsgHeader(pbMsg, awServiceStopHeader, CA_CLIENT_SERVICE_STOP_HEADER_SIZE) == IA_SUCCESS)
		{
			wSrvType = awServiceStopHeader[CA_CLIENT_SERVICE_STOP_SERVICESTYPE];
		}
		else
		{
			CA_ERROR(("Can't get the CA_CLIENT__STOP_SERVICE_GROUP message header.\n"));
			return IA_FALSE;
		}

	}

	switch( eAction )
	{
	case CA_CLIENT__DEFINE_SERVICE_GROUP:
	case CA_CLIENT__STOP_SERVICE_GROUP :
	case CA_CLIENT__DEFINE_EMM_SERVICE:
	case CA_CLIENT__CURRENT_TS_NIT_INFO:
	case CA_CLIENT__TERMINATION:
	case CA_CLIENT__STOP_EMM_SERVICE:
	case CA_CLIENT__MAP_RESOURCES:
		/* FOR_DEBUG_ONLY */
		/*for(i=0; i<(wMsgLen+2); i++)
		CA_PRINTF("%02x ", pbMsg[i]);*/

		/* ca_task must free this memory !!! */
		/* pbLocalCopy = (ia_byte*)ALLOCATE_MEMORY( (ia_uint32)wMsgLen+2, 1 ); */
		pbLocalCopy = (ia_byte*)ALLOCATE_MEMORY( ia_byte, (ia_word32)(wMsgLen+CA_CLIENT_SERVICE_MSG_MAX_LENGTH));
		if( pbLocalCopy != 0 )
		{
			ansi_memcpy(pbLocalCopy, pvParams, wMsgLen+CA_CLIENT_SERVICE_MSG_MAX_LENGTH);
		}
		else
		{
			CA_ERROR(("Failed to allocate memory for local copy.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return IA_FALSE;
		}
		break;

	default:
		CA_ERROR(("Unknown ca_client msg.\n"));
		return IA_FALSE;
	}
	osMsg = (CA_SCELL_MESSAGE_STRUCT*)ALLOCATE_MEMORY( CA_SCELL_MESSAGE_STRUCT, 1 );
	if( osMsg != 0 )
	{
		osMsg->enMsgType           = CADRV__CA_TASK_MESSAGE;
		osMsg->u.stCaMsg.eAction  = eAction;
		osMsg->u.stCaMsg.pstParams = pbLocalCopy;

		/* jyou: 3.4.3.6 SCR#26043 2007-06-01 */
		OS_DRV_WaitSemaphore( gMessageQueueSemap );
		/* The size of each message is sizeof(void*) + sizeof(ia_word16). */
		enRet = OS_DRV_SendMessage(caMessageQueue, sizeof(CA_SCELL_MESSAGE_STRUCT), osMsg);
		OS_DRV_SignalSemaphore( gMessageQueueSemap );

		if( enRet != IA_SUCCESS )
		{
			CA_ERROR(("Can't post CA Client msg to CA Task.\n"));
			if ( pbLocalCopy !=	0 )
			{
				FREE_MEMORY(pbLocalCopy);
				pbLocalCopy = 0;
			}
			if( osMsg != 0 )
			{
				FREE_MEMORY(osMsg);
				osMsg = 0;
			}
			return IA_FALSE;
		}
	}
	else
	{
		CA_ERROR(("Failed to allocate memory for CA Client msg.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		if ( pbLocalCopy !=	0 )
		{
			//[irca ect] pyh : ect严重错误!先释放后赋值0才对!
			FREE_MEMORY(pbLocalCopy);
			pbLocalCopy	= 0;
		}
		return IA_FALSE;
	}

	return IA_TRUE;
}


/*
* Description: This function is provided for the manufacturer to register a
*              callback function for CA queries.
*
* Parameters : fnCallback - Callback function.
*
* Returns    : None.
*/
void CA_TASK_RegisterCaNotify( CA_NOTIFICATION fnCallback )
{
	fpNotification = fnCallback;
}


/*
* jyou
* Description: This function is called when we need to call the CA Client
*              notification function. GroupHandle+ServiceType makes the unique index
*				of each opened services in HST_ECT
*
* Parameters : wHanle   - the GroupHandle of the corresponding HST service
*              eAction - Query type.
*              ulParam  - the ServiceType of the corresponding HST service
*
* Returns    : None.
*/
void CA_CLIENT_Notify( ia_word32 wHandle, CA_QUERY_CONTROL_EN eAction, ia_word32 ulParam )
{
	if( fpNotification == 0 )
	{
		CA_ERROR(("NO CALLBACK FUNCTION REGISTERED!\n"));
	}
	else
	{
		fpNotification(wHandle, eAction, ulParam);
	}
}

/* local reference to Query Control handling functions */
static CA_LOCAL_QueryControl afnQueryControl[100] = {
	SoftCellInfoQueryControl,
	CA_SCARD_SmartCardInfoQueryControl,
	CA_SVCINFO_EmmSvcInfoQueryControl,
	CA_SVCINFO_SvcInfoQueryControl,
	CA_SCARD_ScAccessQueryControl,
	CA_SCARD_CodeChangeQueryControl,	/* = 5 */
	CA_SCARD_CodeCheckQueryControl,
	CA_MONITOR_MonEcmQueryControl,
	CA_MONITOR_MonEmmQueryControl,
	CA_MAIL_MailQueryControl,
	CA_MAIL_AnnounceQueryControl,		/* = 10 */
	CA_SCARD_ScHashQueryControl,
	IRD_MSG_EmmDownloadQueryControl,
	IRD_MSG_EmmChangePinQueryControl,
	IRD_MSG_DecRecQueryControl,
	IRD_MSG_EmmUserPayloadQueryControl,/* = 15 */
	NotifyEnableQueryControl,
	CA_SCARD_RemoteChannelQueryControl,
	CA_ERROR_InfoQueryControl,
	0,
	ComponentsInfoQueryControl,		/* = 20 */
	IRD_MSG_AttributedDisplay,
	CA_IPPV_PreviewQueryControl,
	CA_IPPV_PurchaseQueryControl,
	CA_IPPV_SetThresholdQueryControl,
	CA_IPPV_GetIppvSectorInfoQueryControl,/* = 25 */
	CA_IPPV_EventsListQueryControl,
	CA_IPPV_EventsCheckQueryControl,
	CA_SCARD_SectorsListQueryControl,
	CA_SCARD_RegionQueryControl,
	CA_PPT_InfoQueryControl,			/* = 30 */
	CA_SURFLOCK_QueryControl,
	CA_EXTENDED_PRODUCT_ListQueryControl,
	CA_SCARD_UserDataQueryControl,
#ifdef INCL_PRODUCT_BLOCKING
	CA_PRODUCT_BlockQueryControl,
#else
	0,
#endif
	CA_SCARD_SmartCardUniqueAddressControl,/* = 35 */
	CA_SCARD_SmartCardVendorIDControl,
	CA_SCARD_SmartCardSectorAddressControl,
	CA_SCARD_SmartCardSectorDatecodeControl,
	CA_SCARD_SmartCardOperatorIDQueryControl,
	CA_IPPV_CallbackRequestQueryControl,		/* = 40 */
	0,	/* CA_QC_PVR_MUST_EXPIRE_NOTIFY */
#ifdef INCL_PRODUCT_BLOCKING
	/* jyou: 3.4.2 merge with 3.3.11 */
	CA_PRODUCT_PVRProductBlockQueryControl,
#else
	0,
#endif
	CA_LPPV_AwareQueryControl,
	CA_LPPV_PreviewQueryControl,
	CA_LPPV_ReadTicketQueryControl,      /* = 45 */
	CA_LPPV_DeleteTicketQueryControl,
	CA_LPPV_ReadCreditQueryControl,
	CA_LPPV_ReadOrderQueryControl,
	CA_LPPV_PlaceOrderQueryControl,
	CA_LPPV_CancelOrderQueryControl,     /* = 50 */
	CA_SCARD_MRCapabilityQueryControl,
	CA_SCARD_MRConfigurationQueryControl,
	CA_SCARD_MRProfileQueryControl,
	CA_MR_PinQueryControl,
	CA_RF_ConfigQueryControl,            /* = 55 */
	CA_RF_SetQueryControl,
	CA_HGPC_SmartCardDataQueryControl,
	CA_HGPC_ScellDataQueryControl,
	CA_HGPC_MsgStatusQueryControl,
	0,									/*	= 60 */
	CA_MONITOR_MonEmmQueryControl,
	CA_MONITOR_MonEcmQueryControl,
#ifdef INCL_SERVICE_DMB
	CA_SVCINFO_DMBEmmSvcInfoQueryControl,
	CA_DMB_DescrambleInfoQueryControl,
#else
	0,
	0,
#endif
	CA_IPPV_PhoneNumber_QueryControl,	/*	= 65 */
	0,
	IRD_MSG_AttributedDisplay_2,
};

/*
* Description: This function is called by CA Client usually in response to a
*              notify, to obtain data from or control the CA Task.
*
* Parameters : wHandle	- This parameter is only valid for CA_QC_NOTIFY_ENABLE.
*						It should be the reference to an EMM service or a service group.
*						For other query control types it is currently set to 0.
*              eQuery	- Query control type.
*              pvParams - Pointer to the buffer in which returned data is supposed to be written.
*						The structure of the buffer differs for different eQuery value.
*
* Returns    : Indicating the response status.
*/
CA_QUERY_STATUS_EN CA_TASK_QueryControl( ia_word32 wHandle, CA_QUERY_CONTROL_EN eQuery, void *pvParams )
{
	CA_QUERY_STATUS_EN enStatus = CA_QUERY_FAIL;

	/* validate input parameters */
	if( pvParams == 0 )
		return enStatus;
	if (eQuery < CA_MAX_QC_QUERIES)// eQuery >= 0 && 
	{
		/* Call the specific query control handlers according to eQuery */
		enStatus = (*afnQueryControl[eQuery])( wHandle, pvParams );
	}
	else
	{
		CA_ERROR(("Unknown query control type %d\n", eQuery));
	}
	return enStatus;
}

/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
/*
* Purpose: Change the notify enable mask. In case of EMM or ECM monitoring mask, define or remove  the monitoring resource
*			according to the new mask. In case of invalid handle, this operation will be ignored.
* Return: always return success.
*
*/
static CA_QUERY_STATUS_EN NotifyEnableQueryControl(ia_word32 wHandle, void *pCAClientMsg)
{
	/*	ia_word32 wOldNotifyEnable = m_wNotifyEnable; */
	ia_word32 wEnable;
	ia_word32 *pbEn;
	ia_word16 wIndex, i, j;
	ia_bool		fEnableMonitor;

	pbEn = ( ia_word32* )pCAClientMsg;
	/* Save the new mask value */
	wEnable = pbEn[0];
	CA_DBG_V(("Old Notify Enable Mask = 0x%02x\n", m_wNotifyEnable));
	m_wNotifyEnable = pbEn[0];
	CA_DBG_V(("New Notify Enable Mask = 0x%02x\n", m_wNotifyEnable));

	/* Check the mask for EMM monitoring and define or undefine the resource accordingly */
	fEnableMonitor = ( wEnable & NOTIFY_MASK_MONITOR_EMM ) ? IA_TRUE : IA_FALSE;
	/* Fix #27573 jyou: 3.6.16 locate the demux index of the wEmmHandle */
	i = CA_SVCINFO_FindDemux( wHandle );
	for ( j = 0 ; j < SC_SLOT_MAX ; j++ )
	{
		wIndex = CA_LOCAL_MakeWord16((ia_word16)i, j);
		CA_MONITOR_MonitorControl( wIndex, CA_SERVICE_EMM, fEnableMonitor );
	}

#ifdef INCL_SERVICE_DMB
	/* Check the mask for DMB EMM monitoring and define or undefine the resource accordingly */
	fEnableMonitor = ( wEnable & NOTIFY_MASK_MONITOR_DMB_EMM ) ? IA_TRUE : IA_FALSE;
	for ( j = 0 ; j < SC_SLOT_MAX ; j++ )
	{
		wIndex = CA_LOCAL_MakeWord16((ia_word16)wHandle, j);
		CA_MONITOR_MonitorControl( wIndex, CA_SERVICE_DMB_EMM, fEnableMonitor );
	}
#endif

	/* Check the mask for ECM monitoring and define or undefine the resource accordingly */
	fEnableMonitor = ( wEnable & NOTIFY_MASK_MONITOR_ECM ) ? IA_TRUE : IA_FALSE;
	CA_MONITOR_MonitorControl(wHandle, CA_SERVICE_DESCRAMBLE, fEnableMonitor);

#ifdef INCL_SERVICE_DMB
	/* Check the mask for DMB ECM monitoring and define or undefine the resource accordingly */
	fEnableMonitor = ( wEnable & NOTIFY_MASK_MONITOR_DMB_ECM ) ? IA_TRUE : IA_FALSE;
	CA_MONITOR_MonitorControl(wHandle, CA_SERVICE_DMB_DESCRAMBLE, fEnableMonitor);
#endif

	return CA_QUERY_SUCCESS;
}


ia_bool CA_TASK_CheckNotifyEnable( ia_word32 wMask )
{
	if( (m_wNotifyEnable & wMask) != 0 )
	{
		return IA_TRUE;
	}
	else
	{
		return IA_FALSE;
	}
}

static void CA_TASK_InitLocalServiceData(ia_word16 wMaxNumGroups)
{
	ia_word16 i;

	/* Fixed SCR#27163 */
	if (0 == wMaxNumGroups)
		return;

	if (0 == pstService)  /* this is for group service define */
	{
		pstService = (SERVICE_INFO_ST *)OS_DRV_AllocateMemory(
			(unsigned long)sizeof(SERVICE_INFO_ST)*(unsigned long)wMaxNumGroups);
	}
	if (0 == pstService)
	{
		CA_ERROR(("InitialiseAllModules Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return;
	}
	if (0 == pstDMX)    /* this is for EMM service define */
	{
		pstDMX = (DEMUX_INFO_ST *)OS_DRV_AllocateMemory(
			(unsigned long)sizeof(DEMUX_INFO_ST)*(unsigned long)wMaxNumGroups);
	}
	if (0 == pstDMX)
	{
		CA_ERROR(("InitialiseAllModules Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		OS_DRV_FreeMemory(pstService);
		pstService = 0;
		return;
	}
	if (0 == pstTsNit)  /* this is for EMM service define */
	{
		pstTsNit = (TS_NIT_INFO *)OS_DRV_AllocateMemory(
			(unsigned long)sizeof(TS_NIT_INFO)*(unsigned long)wMaxNumGroups);
	}
	if (0 == pstTsNit)
	{
		CA_ERROR(("InitialiseAllModules Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		OS_DRV_FreeMemory(pstService);
		pstService = 0;
		OS_DRV_FreeMemory(pstDMX);
		pstDMX = 0;
		return;
	}

	if (0 == pstCaDrmPvrRecord)
	{
		pstCaDrmPvrRecord = (CA_DRM_PVR_RECORD_T *)OS_DRV_AllocateMemory(
			(unsigned long)sizeof(CA_DRM_PVR_RECORD_T)*(unsigned long)wMaxNumGroups);
	}

	if (0 == pstCaDrmPvrRecord)
	{
		OS_DRV_FreeMemory(pstService);
		pstService = 0;
		OS_DRV_FreeMemory(pstDMX);
		pstDMX = 0;
		OS_DRV_FreeMemory(pstTsNit);
		pstTsNit = 0;
		CA_ERROR(("InitialiseAllModules Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return;
	}

	if (0 == pstCaDrmPvrPlayback)
	{
		pstCaDrmPvrPlayback = (CA_DRM_PVR_PLAYBACK_T *)OS_DRV_AllocateMemory(
			(unsigned long)sizeof(CA_DRM_PVR_PLAYBACK_T)*(unsigned long)wMaxNumGroups);
	}
	if (0 == pstCaDrmPvrPlayback)
	{
		OS_DRV_FreeMemory(pstService);
		pstService = 0;
		OS_DRV_FreeMemory(pstDMX);
		pstDMX = 0;
		OS_DRV_FreeMemory(pstTsNit);
		pstTsNit = 0;
		OS_DRV_FreeMemory(pstCaDrmPvrRecord);
		pstCaDrmPvrRecord = 0;
		CA_ERROR(("InitialiseAllModules Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return;
	}

	/* init the desc service info */
	for( i = 0; i < wMaxNumGroups; i++ )
	{
		pstService[i].fClear = IA_TRUE;
		pstService[i].wScRscId = INVALID_SMARTCARDID;
		pstService[i].fEmmServiceReady = IA_FALSE;
		pstService[i].fDesServiceReady = IA_FALSE;
		pstService[i].fEcmSrcAdded = IA_FALSE;
		pstService[i].pbPmtInfo = 0;

		/*pstDMX[i].wScRscId = INVALID_SMARTCARDID;*/
		pstDMX[i].fEmmServiceReady = IA_FALSE;
		pstDMX[i].pbCatInfo = 0;
		pstDMX[i].pbNitInfo = 0;
		pstDMX[i].fCatSent = IA_FALSE;
		pstDMX[i].fGetPMT  = IA_FALSE;
		pstDMX[i].fHasOOB  = IA_FALSE;

		pstTsNit[i].GroupHandle = INVALID_GROUP_SERVICE_HANDLE;
		pstTsNit[i].originalNetworkID = INVALID_TS_NIT_ID;
		pstTsNit[i].transportID = INVALID_TS_NIT_ID;
		pstTsNit[i].ServiceID = INVALID_TS_NIT_ID;

		pstCaDrmPvrRecord[i].wSessionResourceId = 0xFFFFFFFF;
		pstCaDrmPvrRecord[i].wDescrambleServiceHandle = 0xFFFF;
		pstCaDrmPvrRecord[i].fUseDrm = IA_FALSE;
		pstCaDrmPvrRecord[i].fFtaCheckAnySector = IA_FALSE;
		pstCaDrmPvrRecord[i].bFtaCheckSectorNr = 0;
		pstCaDrmPvrRecord[i].fChainRecordings = IA_FALSE;
		pstCaDrmPvrRecord[i].ePvrPurpose = MSG_SM_DRM_PVR_PURPOSE_PERMANENT;
		pstCaDrmPvrRecord[i].wChainingSessionResourceId = 0xFFFFFFFF;

		pstCaDrmPvrPlayback[i].wSessionResourceId = 0xFFFFFFFF;
		pstCaDrmPvrPlayback[i].fUseDrm = IA_FALSE;
		pstCaDrmPvrPlayback[i].wRightsRecordId = 0xFFFFFFFF;
		pstCaDrmPvrPlayback[i].wSequenceRecordId = 0xFFFFFFFF;
		pstCaDrmPvrPlayback[i].fPeekRightsOnly = IA_FALSE;
		pstCaDrmPvrPlayback[i].ePvrPurpose = MSG_SM_DRM_PVR_PURPOSE_PERMANENT;
		pstCaDrmPvrPlayback[i].ePlaybackDirection = MSG_SM_PLAYBACK_DIRECTION_FORWARD;
	}


}

static void InitialiseAllModules(ia_word16 wMaxNumGroups)
{
	ansi_memset((void *)gwLastGrpHandle, 0xff, sizeof(ia_word32)*MAX_GLOBAL_LAST_HANDLE_NUM);
	/* init the service info */
	CA_SVCINFO_SetSvcMaxNumber(wMaxNumGroups);
	CA_SVCINFO_Initialise(wMaxNumGroups);
	CA_ERROR_Initialise(wMaxNumGroups);
	CA_MONITOR_Initialise(wMaxNumGroups);

	CA_TASK_InitLocalServiceData(wMaxNumGroups);

	CA_SCARD_Initialise(INVALID_SMARTCARDID);

	IRD_MSG_Initialise(wMaxNumGroups);
	CA_IPPV_Initialise(INVALID_SMARTCARDID);
	CA_PPT_Initialise(INVALID_SMARTCARDID);
	CA_LPPV_Initialise(INVALID_SMARTCARDID);
	CA_HGPC_Initialise(INVALID_SMARTCARDID);

	/* Better be called after CA_SCARD_Initialise */
	CA_SURFLOCK_Initialise();
	/*	CA_FlexiFlash_Initialise(); */
#ifdef INCL_PRODUCT_BLOCKING
	/* mli: 3.3.11 parameter changed */
	CA_PRODUCT_BLOCK_Init(INVALID_SMARTCARDID) ;
#endif
	CA_EXTENDED_PRODUCT_ListInitialise(INVALID_SMARTCARDID);
	/* mli: 3.3.11 	 for PVR	Product	7/22 */
		CA_PRODUCT_CheckInitialise(INVALID_SMARTCARDID);
}

/*
* Description: Main message loop of Embedded CA Task.
*
* Parameters : None.
*
* Returns    : None.
*/
static void CaTaskMain( void *argv )
{
	CA_SCELL_MESSAGE_STRUCT *osMsg;
	ia_word16 wMsgLen;

	CA_INIT_CONFIG_ST *pstTemp = 0;
	/* the manufacturer decides the max service number, including DVB, PVR, EMM */
	pstTemp = (CA_INIT_CONFIG_ST *)argv;
	m_stCaInitCfg.wLength = pstTemp->wLength;
	m_stCaInitCfg.wNumGroups = pstTemp->wNumGroups;
		m_stCaInitCfg.eCryptoAlgorithm = pstTemp->eCryptoAlgorithm;

		CA_DBG(("CaTaskMain wLength = %d, wNumGroups = %d\n", m_stCaInitCfg.wLength, m_stCaInitCfg.wNumGroups));

	CA_PARAM_NOT_USED(argv);   /* ??? mli */
	CA_DBG(("+++ CA TASK is running now.\n"));

	/* Initialise all modules here */
	InitialiseAllModules(m_stCaInitCfg.wNumGroups);

	/* Initialise resource map table */
	stResourceMap.stMRMap.abMRPin[0]=0xff;
	stResourceMap.stMRMap.abMRPin[1]=0xff;
	stResourceMap.stMRMap.wResourceId=0xffff;
	stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount = 0x00;
	stResourceMap.stGeneralConfig.stConfigDesc = 0;

	/* Wait for SoftCell to start. */
	while( fSoftCellRunning != IA_TRUE )
	{
		OS_DRV_DelayTask(500);
	}
	/* Let scell settle down */
	OS_DRV_DelayTask(500);

	/* mli: 3.3.11 */
	/* Henry Fix for #25309*/
	OS_DRV_SignalSemaphore(gECTStartUpSemap);

	/* Request SoftCell to tell its client version to CA Task. */
	SendClientVersionQueryMsg();
	SendComponentsVersionQueryMsg();

	/* MAIN CA MESSAGE LOOP */
	while( fTermination != IA_TRUE )
	{
		CA_DBG_V(("\n\nWaiting for message...\n"));

		osMsg = (CA_SCELL_MESSAGE_STRUCT*)OS_DRV_ReceiveMessage(&wMsgLen);

		if( (osMsg != 0) && (wMsgLen == sizeof(CA_SCELL_MESSAGE_STRUCT)) )
		{
			CaProcessMsg(osMsg);
		}
		else
		{
			CA_ERROR(("Msg len = %d\n", wMsgLen));
		}

		if( osMsg != 0 )
		{
			FREE_MEMORY(osMsg);
						osMsg = 0;
		}
	} /* End Forever */
	CA_ERROR(("Terminate the ECT\n"));
	OS_DRV_DelayTask(500);
	/* Free all located memory */
	CA_TASK_FreeAllocatedMemory();
	fpNotification = 0;
}

/*
*	Reopened by Jane in ECT 3.4.2 to remove hard code of Smart Card ID
*/
ia_word32 CA_LOCAL_GetSmartCardIdFromSectorMsg(CA_SCELL_MSG *pstMsg)
{
	ia_msg_handle						  pvMsg;
	msg_number_of_sectors_reply_data_st   stScSectorList;

	/*allocate the memeory of message and get the sectors reply message*/
	pvMsg = IA_OpenInputMessage( pstMsg->pvMessage, pstMsg->wLength );
	IA_ReadFromMessage( pvMsg, &stScSectorList, sizeof( msg_number_of_sectors_reply_data_st ));
	IA_CloseMessage(pvMsg, IA_FALSE);

	return stScSectorList.wResourceId;
}

/*
* Description: Handles all messages received from the external modules.
*
* Parameters : pstCA_Msg - Pointer to the message structure.
*
* Returns    : None.
*/
static void CaProcessMsg( CA_SCELL_MESSAGE_STRUCT *pstCA_Msg )
{
	ia_word16		wOpCode;
	CA_SCELL_MSG	*pstSCellMsg;
	/* Fix SCR#26646: ayang 07/12/04 */
	/*[irca ect] pyh : 使pstSCellMsgStore为全局变量*/
	//static CA_SCELL_MSG	pstSCellMsgStore[100];
	static ia_int8	bSCellMsgCnt = 0;
	ia_int8	bMsgCnt = 0, i = 0;

	switch( pstCA_Msg->enMsgType )
	{
	case CADRV__SCELL_MESSAGE:
		/******************************************************************/
		/* jyou: ECT v3.4.2 Structural renovation                         */
		/******************************************************************/
		pstSCellMsg = (CA_SCELL_MSG*)(&pstCA_Msg->u.stSCellMsg);
		wOpCode = ( ia_word16 )pstSCellMsg->eOpcode;

		if ( bSCellMsgFlag == 1 || bSCellMsgFlagStatic == 1)
		{
			bSCellMsgFlagStatic = 1;
			ProcessSCellMsg(pstSCellMsg);
			if (bSCellMsgCnt > 0)
			{
				OS_DRV_WaitSemaphore( gSCellMsgSemap);
				bMsgCnt = bSCellMsgCnt;
				bSCellMsgCnt = 0;
				for (i = 0; i<bMsgCnt; i++)
				{
					pstSCellMsg = &pstSCellMsgStore[i];
					ProcessSCellMsg(pstSCellMsg);
				}
				OS_DRV_SignalSemaphore( gSCellMsgSemap );
			}
		}
		else if ((wOpCode == MSG_IPPV_EVENT_NOTIFY)
			|| (wOpCode == MSG_IPPV_SECTOR_INFORMATION_REPLY)
			|| (wOpCode == MSG_IPPV_PHONE_NUMBER_REPLY)) /* Fixed SCR#27509,SCR#26646,SCR#27533 */
		{
			/*pstSCellMsgStore[bSCellMsgCnt].eOpcode = pstSCellMsg->eOpcode;
			pstSCellMsgStore[bSCellMsgCnt].wOpSeqNo = pstSCellMsg->wOpSeqNo;
			pstSCellMsgStore[bSCellMsgCnt].wLength = pstSCellMsg->wLength;
			pstSCellMsgStore[bSCellMsgCnt++].pvMessage = pstSCellMsg->pvMessage;*/
			
			/*[irca ect] pyh: 添加对bSCellMsgCnt有效性的判断,避免快速插拔卡造成的死机。
			但是对旧消息的删除，也许会导致忽略某些重要消息的处理*/
			if (pstSCellMsgStore[bSCellMsgCnt].pvMessage != 0)
			{
				FREE_MEMORY( pstSCellMsgStore[bSCellMsgCnt].pvMessage );
				pstSCellMsgStore[bSCellMsgCnt].pvMessage = 0;
			}
			/*STTBX_Print((" ###### bSCellMsgCnt = %d\n",bSCellMsgCnt));*/
			pstSCellMsgStore[bSCellMsgCnt].eOpcode = pstSCellMsg->eOpcode;
			pstSCellMsgStore[bSCellMsgCnt].wOpSeqNo = pstSCellMsg->wOpSeqNo;
			pstSCellMsgStore[bSCellMsgCnt].wLength = pstSCellMsg->wLength;
			pstSCellMsgStore[bSCellMsgCnt].pvMessage = pstSCellMsg->pvMessage;
			bSCellMsgCnt = (bSCellMsgCnt + 1)%STORE_MSG_NUM;
		}
		else
		{
			ProcessSCellMsg(pstSCellMsg);
		}
		break;
	case CADRV__CA_TASK_MESSAGE:
		{
			CA_MSG_T *pCaMsg = 0;
			ia_byte  *pbMsg = 0;
			ia_word16 wMsgLen = 0, wDescLen = 0, wRecLen = 12, wPlaybackLen = 14;
			ia_word32 wServiceType;
			ia_word32 wGrpHandle = 0;

			ia_word32 awServiceGroupDefineHeader[CA_CLIENT_SERVICE_GROUP_DEFINE_HEADER_SIZE/4];
			ia_word32 awServiceStopHeader[CA_CLIENT_SERVICE_STOP_HEADER_SIZE/4];
			ia_word32 awRecSrvIDs[CA_CLIENT_SERVICE_RECORDID_SIZE/4];
			ia_word32 awPlaybackSrvIDs[CA_CLIENT_SERVICE_PLAYBACKID_SIZE/4];

			CA_SERVICE_HEADER stCaServiceHeader;
			CA_DRM_PVR_RECORD_T stCaDrmPvrRecord;
			CA_DRM_PVR_PLAYBACK_T stCaDrmPvrPlayback;

			ansi_memset((void *)awServiceGroupDefineHeader, 0, sizeof(ia_byte) * CA_CLIENT_SERVICE_GROUP_DEFINE_HEADER_SIZE);
			ansi_memset((void *)awServiceStopHeader, 0, sizeof(ia_byte) * CA_CLIENT_SERVICE_STOP_HEADER_SIZE);
			ansi_memset((void *)awRecSrvIDs, 0, sizeof(ia_byte) * CA_CLIENT_SERVICE_RECORDID_SIZE);
			ansi_memset((void *)awPlaybackSrvIDs, 0, sizeof(ia_byte) * CA_CLIENT_SERVICE_PLAYBACKID_SIZE);

			pCaMsg = (CA_MSG_T *)(&pstCA_Msg->u.stCaMsg);
			pbMsg   = (ia_byte *)pCaMsg->pstParams;

			/*[irca ect] pyh: 添加对pCaMsg->pstParams有效性的判断*/
			if (pbMsg == 0)
			{
				break;
			}
			
			wMsgLen = (ia_word16)((ia_word16)(pbMsg[0]<<8) + (ia_word16)pbMsg[1]);

			switch(pCaMsg->eAction)
			{
			case CA_CLIENT__DEFINE_SERVICE_GROUP:
				{

					if (GetMsgHeader(pbMsg+CA_CLIENT_SERVICE_MSG_MAX_LENGTH, awServiceGroupDefineHeader, CA_CLIENT_SERVICE_GROUP_DEFINE_HEADER_SIZE) == IA_SUCCESS)
					{
						wGrpHandle = awServiceGroupDefineHeader[CA_CLIENT_SERVICE_GROUP_HANDLE];
						/* the grouphandle is too big */
						if (wGrpHandle >= CA_SVCINFO_GetSvcMaxNumber())
							break;
						stCaServiceHeader.wGroupHandle = wGrpHandle;
						/* jyou: default is the first smart card */
						stCaServiceHeader.wSmartCardId = CA_SCARD_GetSmartCardId( 0 );
						stCaServiceHeader.wDemuxId = wGrpHandle; /* default: wGroupHandle;*/
						stCaServiceHeader.wMacrovisionId = wGrpHandle;  /* default: wGroupHandle, macrovisionId is unique */
						wServiceType = awServiceGroupDefineHeader[CA_CLIENT_SERVICE_GROUP_DEFINE_SERVICESTYPE];
					}
					else
					{
						CA_ERROR(("Can't get the CA_CLIENT__DEFINE_SERVICE_GROUP message header.\n"));
						break;
					}
					switch (wServiceType & 0x0F)
					{
					case 7:  /* desc+rec+playback */
						wDescLen = (ia_word16)(wMsgLen - wRecLen - wPlaybackLen - 8);
						wRecLen = 12;
						wPlaybackLen = 14;
						break;
					case 6:  /* rec+playback */
						wDescLen = 0;
						wRecLen = 12;
						wPlaybackLen = 14;
						break;
					case 5:  /* desc + playback */
						wDescLen = (ia_word16)(wMsgLen - wPlaybackLen - 8);
						wRecLen = 0;
						wPlaybackLen = 14;
						break;
					case 4:  /* playback */
						wDescLen = 0;
						wRecLen = 0;
						wPlaybackLen = 14;
						break;
					case 3:  /* Rec+desc */
						wDescLen = (ia_word16)(wMsgLen - wRecLen - 8);
						wRecLen = 12;
						wPlaybackLen = 0;
						break;
					case 2:  /* rec */
						wDescLen = 0;
						wRecLen = 12;
						wPlaybackLen = 0;
						break;
					case 1: /* desc */
						wDescLen = (ia_word16)(wMsgLen - 8);
						wRecLen = 0;
						wPlaybackLen = 0;
						break;
#ifdef INCL_SERVICE_DMB
					case 8: /* dmbdesc */
						wDescLen = (ia_word16)(wMsgLen - 8);
						wRecLen = 0;
						wPlaybackLen = 0;
						break;
#endif
					default:
						wDescLen = 0;
						wRecLen = 0;
						wPlaybackLen = 0;
						break;
					}
					/*	CA_SCARD_SetSmartCardId(stCaServiceHeader.wSmartCardId); */     /* set the smartcard id */

					/* CA_CLIENT_DESCRAMBLE_SERVICE_TYPE */
					if ((wServiceType & CA_CLIENT_DESCRAMBLE_SERVICE_TYPE) != 0)
					{
						/* jyou: 3.4.2 multiple smart cards renovation */
						DefineDescrambleService( wGrpHandle, wDescLen, pbMsg, pCaMsg);
					}
#ifdef INCL_SERVICE_DMB
					/*Added for the CMMB*/
					if ((wServiceType & CA_CLIENT_DMB_DESCRAMBLE_SERVICE_TYPE) != 0)
					{
						DefineDMBDescrambleServices( wGrpHandle, pbMsg);
					}
#endif
					/* CLIENT_RECORD_SERVICE_TYPE */
					if ((wServiceType & CA_CLIENT_RECORD_SERVICE_TYPE) != 0)
					{
						/*jyou: save service type. The following two lines. 3_3_9 Bug#01 May 31, 06*/
						stCaServiceHeader.wServiceType = CA_SERVICE_RECORD;
						CA_SVCINFO_SetSvcHeaderInfo(wGrpHandle, &stCaServiceHeader);

						CA_DBG(("=> CA_CLIENT__START_RECORD\n"));

						/* extract header */
						ansi_memset((void *)&stCaDrmPvrRecord, 0, sizeof(stCaDrmPvrRecord));

						pstCaDrmPvrRecord[wGrpHandle].wSessionResourceId = (ia_word32)(
							((ia_word32)pbMsg[10+wDescLen] << 24) |
							((ia_word32)pbMsg[10+wDescLen+1] << 16) |
							((ia_word32)pbMsg[10+wDescLen+2] << 8) |
							((ia_word32)pbMsg[10+wDescLen+3]));

						pstCaDrmPvrRecord[wGrpHandle].fUseDrm = (ia_bool)(pbMsg[0xa+wDescLen+4]);

#ifdef ECT_SUPPORT_PVR_CLIENT_PHASE12
						if (pstCaDrmPvrRecord[wGrpHandle].fUseDrm == IA_TRUE)
						{
							CA_DBG(("=> ECT_SUPPORT_PVR_CLIENT_PHASE12 But fUseDrm is TRUE\n"));
							break;
						}

						/*jyou May 19,06: 3_3_8 bug fixed.*/
						/******************************************************************************************
						* Save the recieved ServiceType and Session_Resource_ID to pstLocalGrpSvcInfo[wGroupHandle]
						*******************************************************************************************/
						CA_SVCINFO_SetPvrMapInfo(wGrpHandle,
							&stCaServiceHeader,
							pstCaDrmPvrRecord[wGrpHandle].wSessionResourceId);

						if (INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wGrpHandle, CA_SERVICE_RECORD))
						{
							SendServiceOpenRequestMsg((ia_word16)wGrpHandle, SERVICE_PVR_RECORD);
						}
						else
						{
							AddResourceToPvrRecordService((ia_word16)wGrpHandle);
						}
#endif

#ifdef ECT_SUPPORT_PVR_CLIENT_DRM
						if (pstCaDrmPvrRecord[wGrpHandle].fUseDrm == IA_FALSE)
						{
							CA_DBG(("=> ECT_SUPPORT_PVR_CLIENT_DRM But fUseDrm is FALSE\n"));
							break;
						}
						pstCaDrmPvrRecord[wGrpHandle].wDescrambleServiceHandle = CA_SVCINFO_GetSCellHandle(wGrpHandle, CA_SERVICE_DESCRAMBLE);
						pstCaDrmPvrRecord[wGrpHandle].ePvrPurpose = (msg_sm_drm_pvr_purpose_e)pbMsg[10+wDescLen+5];
						pstCaDrmPvrRecord[wGrpHandle].fChainRecordings = (ia_bool)pbMsg[10+wDescLen+6];

						pstCaDrmPvrRecord[wGrpHandle].fFtaCheckAnySector = IA_FALSE;       /* jyou: chagende from IA_FALSE to 0 */
						pstCaDrmPvrRecord[wGrpHandle].bFtaCheckSectorNr = 0; /* the profile should be decided when compile */

						/* if (IA_TRUE == stCaDrmPvrRecord.fChainRecordings) */
												if (IA_TRUE == pstCaDrmPvrRecord[wGrpHandle].fChainRecordings)
						{
							pstCaDrmPvrRecord[wGrpHandle].wChainingSessionResourceId = (ia_word32)(
								((ia_word32)pbMsg[10+wDescLen+7] << 24) |
								((ia_word32)pbMsg[10+wDescLen+8] << 16) |
								((ia_word32)pbMsg[10+wDescLen+9] << 8) |
								((ia_word32)pbMsg[10+wDescLen+10]));
						}
						else
						{
							pstCaDrmPvrRecord[wGrpHandle].wChainingSessionResourceId = 0xFFFFFFFF;
						}

						/*jyou: ECT 3_3_9 Bug fix #03. May 31, 06*/
						CA_SVCINFO_SetPvrMapInfo(wGrpHandle,
							&stCaServiceHeader,
							pstCaDrmPvrRecord[wGrpHandle].wSessionResourceId);

						if (INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wGrpHandle, CA_SERVICE_RECORD))
						{
							SendServiceOpenRequestMsg((ia_word16)wGrpHandle, SERVICE_PVR_DRM);
						}
						else
						{
							AddResourceToPvrRecordServiceForDrm((ia_word16)wGrpHandle);
						}
#endif
					}

					/* CLIENT_PLAYBACK_SERVICE_TYPE */
					if ((wServiceType & CA_CLIENT_PLAYBACK_SERVICE_TYPE) != 0)
					{
						/*jyou: save service type. The following two lines. 3_3_9 Bug#01 May 31, 06*/
						stCaServiceHeader.wServiceType = CA_SERVICE_PLAYBACK;
						CA_SVCINFO_SetSvcHeaderInfo(wGrpHandle, &stCaServiceHeader);

						CA_DBG(("=> CA_CLIENT__START_PLAYBACK\n"));
						ansi_memset((void *)&stCaDrmPvrPlayback, 0, sizeof(stCaDrmPvrPlayback));

						pstCaDrmPvrPlayback[wGrpHandle].wSessionResourceId = (ia_word32)(
							((ia_word32)pbMsg[10+wDescLen+wRecLen] << 24) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+1] << 16) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+2] << 8) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+3]));
						pstCaDrmPvrPlayback[wGrpHandle].fUseDrm = (ia_bool)(pbMsg[10+wDescLen+4]);

#ifdef ECT_SUPPORT_PVR_CLIENT_PHASE12
						if (pstCaDrmPvrPlayback[wGrpHandle].fUseDrm == IA_TRUE)
						{
							CA_DBG(("=> ECT_SUPPORT_PVR_CLIENT_PHASE12 But fUseDrm is TRUE\n"));
							break;
						}

						CA_SVCINFO_SetPvrMapInfo(wGrpHandle, &stCaServiceHeader, pstCaDrmPvrPlayback[wGrpHandle].wSessionResourceId);

						if (INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wGrpHandle, CA_SERVICE_PLAYBACK))
						{
							SendServiceOpenRequestMsg((ia_word16)wGrpHandle, SERVICE_PVR_PLAYBACK);
						}
						else
						{
							AddResourceToPvrPlayBackService((ia_word16)wGrpHandle);
						}

#endif
#ifdef ECT_SUPPORT_PVR_CLIENT_DRM
						if (pstCaDrmPvrPlayback[wGrpHandle].fUseDrm == IA_FALSE)
						{
							CA_DBG(("=> ECT_SUPPORT_PVR_CLIENT_DRM But fUseDrm is FALSE\n"));
							break;
						}
						pstCaDrmPvrPlayback[wGrpHandle].wRightsRecordId = (ia_word32)(
							((ia_word32)pbMsg[10+wDescLen+wRecLen+5] << 24) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+6] << 16) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+7] << 8) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+8]));

						pstCaDrmPvrPlayback[wGrpHandle].wSequenceRecordId = (ia_word32)(
							((ia_word32)pbMsg[10+wDescLen+wRecLen+9] << 24) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+10] << 16) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+11] << 8) |
							((ia_word32)pbMsg[10+wDescLen+wRecLen+12]));

						pstCaDrmPvrPlayback[wGrpHandle].ePlaybackDirection = (msg_sm_drm_playback_direction_e)pbMsg[10+wDescLen+wRecLen+13];
						pstCaDrmPvrPlayback[wGrpHandle].fPeekRightsOnly = (ia_bool)pbMsg[10+wDescLen+wRecLen+14];
#ifdef INCL_EXTRA_DRM
						/* Ayang: 2008-6-17 Fixed SCR#27217 */
						pstCaDrmPvrPlayback[wGrpHandle].ePvrPurpose = (msg_sm_drm_pvr_purpose_e)pbMsg[10+wDescLen+wRecLen+15];
#endif
						CA_SVCINFO_SetPvrMapInfo(wGrpHandle, &stCaServiceHeader, pstCaDrmPvrPlayback[wGrpHandle].wSessionResourceId);
						/* jyou: only one PVR_DRM service, no need to reopen if it is already opened during record*/
						if (INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wGrpHandle, CA_SERVICE_PLAYBACK))
						{
							SendServiceOpenRequestMsg((ia_word16)(wGrpHandle|0x8000), SERVICE_PVR_DRM);
						}
						else
						{
							AddResourceToPvrPlayBackServiceForDrm((ia_word16)(wGrpHandle));
#ifndef EXCL_MATURITY_RATING
							AddMRSourceToService((ia_word16)(wGrpHandle), CA_SERVICE_PLAYBACK);
#endif
						}
#endif
					}

				}
				break;

			case CA_CLIENT__STOP_SERVICE_GROUP:  /* stop group service */
				{

					CA_DBG(("=> CA_CLIENT__STOP_SERVICE_GROUP\n"));

					if (GetMsgHeader(pbMsg+CA_CLIENT_SERVICE_MSG_MAX_LENGTH, awServiceStopHeader, CA_CLIENT_SERVICE_STOP_HEADER_SIZE))
					{
						wGrpHandle = stCaServiceHeader.wGroupHandle = awServiceStopHeader[CA_CLIENT_SERVICE_GROUP_HANDLE];
						stCaServiceHeader.wServiceType = awServiceStopHeader[CA_CLIENT_SERVICE_STOP_SERVICESTYPE];
					}
					else
					{
						CA_ERROR(("Can't get the CA_CLIENT__STOP_SERVICE_GROUP message header.\n"));
						break;
					}
					/* the grouphandle is too big */
					if (wGrpHandle >= CA_SVCINFO_GetSvcMaxNumber())
						break;
					if (IA_TRUE == CA_SVCINFO_GetGrpSvcStatus(wGrpHandle))
					{
						break;   /* the service isn't opened */
					}


					/* CA_CLIENT_DESCRAMBLE_SERVICE_TYPE */
					if ((stCaServiceHeader.wServiceType & CA_CLIENT_DESCRAMBLE_SERVICE_TYPE) != 0)
					{
						CloseActiveProgram(wGrpHandle);
						/* jyou: 3.4.2 SCR#25079 */
						/* reset the error message when stop a service */
						CA_ERROR_Reset( wGrpHandle, CA_SERVICE_DESCRAMBLE );
					}
#ifdef INCL_SERVICE_DMB
					if ((stCaServiceHeader.wServiceType & CA_CLIENT_DMB_DESCRAMBLE_SERVICE_TYPE) != 0)
					{
						/*CloseActiveProgram(wGrpHandle);*/
						/*ResetLocalServiceInfo(wGrpHandle);*/
						pstService[wGrpHandle].fDesServiceReady = IA_FALSE;
						UpdateDefaultResToDMBDescrambleService(wGrpHandle, CA_SERVICE_DMB_DESCRAMBLE,IA_FALSE);
						AddEcmResToDMBDescrambleService(wGrpHandle, CA_SERVICE_DMB_DESCRAMBLE,IA_FALSE);
						CA_MONITOR_MonitorControl(wGrpHandle, ECT_FOR_DMB_CLIENT, IA_FALSE);  /* disable ECM Monitor */

						CA_ERROR_Reset( wGrpHandle, CA_SERVICE_DMB_DESCRAMBLE );
						DMB_ServicesClose(wGrpHandle, CA_SERVICE_DMB_DESCRAMBLE);
					}
#endif
					/* CLIENT_RECORD_SERVICE_TYPE */
					if ((stCaServiceHeader.wServiceType & CA_CLIENT_RECORD_SERVICE_TYPE) != 0)
					{
						CA_DBG(("=> CA_CLIENT__STOP_RECORD\n"));
						RemoveResourceFromPvrRecordService(wGrpHandle);
						/* jyou: 3.4.2 SCR#25079 */
						/* reset the error message when stop a service */
						CA_ERROR_Reset( wGrpHandle, CA_SERVICE_RECORD );
						/* Fixed SCR#27342 */
						ServicesClose(wGrpHandle,CA_SERVICE_RECORD); /*jliu: added serviceclose*/
					}

					/* CLIENT_PLAYBACK_SERVICE_TYPE */
					if ((stCaServiceHeader.wServiceType & CA_CLIENT_PLAYBACK_SERVICE_TYPE) != 0)
					{
						CA_DBG(("=> CA_CLIENT__STOP_PLAYBACK\n"));
						RemoveResourceFromPvrPlayBackService(wGrpHandle);
						/* jyou: 3.4.2 SCR#25079 */
						/* reset the error message when stop a service */
						CA_ERROR_Reset( wGrpHandle, CA_SERVICE_PLAYBACK );
						/* Fixed SCR#27342 */
						ServicesClose(wGrpHandle,CA_SERVICE_PLAYBACK); /*jliu: added serviceclose*/
					}
				}
				break;

			case CA_CLIENT__DEFINE_EMM_SERVICE:
				DefineEMMServicesOnDemux( pbMsg, pCaMsg );
				break;

			case CA_CLIENT__STOP_EMM_SERVICE:
				StopEMMServicesOnDemux( pbMsg );
				break;

			case CA_CLIENT__CURRENT_TS_NIT_INFO:
				{
					CA_DBG(("=> CA_CLIENT__CURRENT_TS_NIT_INFO\n"));

					wGrpHandle = (ia_word32)(((ia_word32)pbMsg[2] << 24) | ((ia_word32)pbMsg[3] << 16) | ((ia_word32)pbMsg[4] <<  8) | ((ia_word32)pbMsg[5]) );
					/* the grouphandle is too big */
					if (wGrpHandle >= CA_SVCINFO_GetSvcMaxNumber())
						break;
					pstTsNit[wGrpHandle].originalNetworkID = ( ia_word16 )(( pbMsg[ 6 ] ) << 8 );
					pstTsNit[wGrpHandle].originalNetworkID += ( ia_word16 )( pbMsg[ 7 ] );

					pstTsNit[wGrpHandle].transportID = ( ia_word16 )(( pbMsg[ 8 ] ) << 8 );
					pstTsNit[wGrpHandle].transportID += ( ia_word16 )( pbMsg[ 9 ] );

					/*	pstTsNit[wGrpHandle].ServiceID = ( ia_word16 )(( pbMsg[ 10 ] ) << 8 );
					pstTsNit[wGrpHandle].ServiceID += ( ia_word16 )( pbMsg[ 11 ] ); */

					CA_DBG(("=> [wGrpHandle]: %d networkID = %2x, transport ID = %2x\n",
						wGrpHandle,
						pstTsNit[wGrpHandle].originalNetworkID,
						pstTsNit[wGrpHandle].transportID
						/* pstTsNit[wGrpHandle].ServiceID */));
				}
				break;

			case CA_CLIENT__MAP_RESOURCES:
				MapResources( pbMsg );
				break;

			case CA_CLIENT__TERMINATION:
				CA_DBG(("=> CA_CLIENT__TERMINATION\n"));
				fTermination = IA_TRUE;
				break;
			default:
				CA_ERROR(("Unknown ca_client msg.\n"));
				break;
			} /*switch*/

			if( pCaMsg->pstParams != 0 )
			{
				FREE_MEMORY(pCaMsg->pstParams);
								pCaMsg->pstParams =	0;
			}
			break;
		}
	default:
		{
			CA_ERROR(("Unknown ca msg type.\n"));
			break;
		}
	} /* switch */
	return;
}


static void SendClientVersionQueryMsg( void )
{
	CA_DBG_V(("<= MSG_CLIENT_VERSION_QUERY\n"));

	if( SCELL_Message(MSG_CLIENT_VERSION_QUERY, MSG_CLIENT_VERSION_QUERY, 0, 0) != IA_SUCCESS )
	{
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
}

static void SendComponentsVersionQueryMsg( void )
{
	CA_DBG_V(("<= MSG_COMPONENTS_VERSION_QUERY\n"));

	if( SCELL_Message(MSG_COMPONENTS_VERSION_QUERY, MSG_COMPONENTS_VERSION_QUERY, 0, 0) != IA_SUCCESS )
	{
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
}

static void ParseVersionReplyMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	msg_version_reply_data_st stVerReplyData;
	msg_version_data_st stVerData;
	ia_byte i;

	pvMsg = IA_OpenInputMessage( pstSCellMsg->pvMessage, pstSCellMsg->wLength );
	if (pvMsg == 0)
		return;
	if (IA_ReadFromMessage( pvMsg, &stVerReplyData, sizeof(stVerReplyData) ) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}
	CA_DBG_V(("wVersionCount = %d\n", stVerReplyData.wVersionCount)); /* Always >=1 now. */

	if (stVerReplyData.wVersionCount == 1) /* scell client query reply*/
	{
		IA_ReadFromMessage( pvMsg, &stVerData, sizeof(stVerData) );
		stSoftCell.bMajor = stVerData.bMajor;
		stSoftCell.bMinor = stVerData.bMinor;
		stSoftCell.uSubVersion = stVerData.wSubVersion;

		IA_ReadFromMessage( pvMsg, stSoftCell.acNameString, stVerData.wNameLength );
		IA_ReadFromMessage( pvMsg, stSoftCell.acBuildInfoString, stVerData.wBuildInfoLength );

		CA_DBG(("%s version: %d.%d.%d %s\n",
			stSoftCell.acNameString,
			stSoftCell.bMajor,
			stSoftCell.bMinor,
			stSoftCell.uSubVersion,
			stSoftCell.acBuildInfoString));

		/* Always notify CA Client. */
		CA_CLIENT_Notify( 0, CA_QC_SOFTCELL_INFO, 0 );

		IA_CloseMessage(pvMsg, IA_FALSE);
		return ;
	}
	else /* scell components query reply*/
	{
		if (stComponents.pstComponents != 0)
		{
			FREE_MEMORY(stComponents.pstComponents);
			stComponents.pstComponents = 0;
		}
		if (0 == stComponents.pstComponents)
		{
			stComponents.pstComponents = (CA_SOFTCELL_INFO_T*)ALLOCATE_MEMORY(
				CA_SOFTCELL_INFO_T, stVerReplyData.wVersionCount );
		}
		/* Fixed SCR#27163 */
		if (0 == stComponents.pstComponents && stVerReplyData.wVersionCount > 0)
		{
			CA_ERROR(("ParseVersionReplyMsg memory allocate error!\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			IA_CloseMessage(pvMsg, IA_FALSE);
			return;
		}
		stComponents.bComCount = (ia_byte)stVerReplyData.wVersionCount;
		for (i=0; i<stVerReplyData.wVersionCount; i++)
		{
			IA_ReadFromMessage( pvMsg, &stVerData, sizeof(stVerData) );
			stComponents.pstComponents[i].bMajor = stVerData.bMajor;
			stComponents.pstComponents[i].bMinor = stVerData.bMinor;
			stComponents.pstComponents[i].uSubVersion = stVerData.wSubVersion;

			IA_ReadFromMessage( pvMsg, stComponents.pstComponents[i].acNameString, stVerData.wNameLength );
			IA_ReadFromMessage( pvMsg, stComponents.pstComponents[i].acBuildInfoString, stVerData.wBuildInfoLength );

			CA_DBG(("%s version: %d.%d.%d %s\n",
				stComponents.pstComponents[i].acNameString,
				stComponents.pstComponents[i].bMajor,
				stComponents.pstComponents[i].bMinor,
				stComponents.pstComponents[i].uSubVersion,
				stComponents.pstComponents[i].acBuildInfoString));
		}

		/* Always notify CA Client. */
		CA_CLIENT_Notify( 0, CA_QC_COMPONENTS_INFO, stComponents.bComCount);
	}
	IA_CloseMessage(pvMsg, IA_FALSE);
}

/*
* Description: Query handler for CA_QC_SOFTCELL_INFO type.
*
* Parameters :
*	wHandle	 - Reserved for future use, currently is always 0
*	pvParams - Pointer to the data buffer provided by CA Client.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/
static CA_QUERY_STATUS_EN SoftCellInfoQueryControl( ia_word32 wHandle, void *pCAClientMsg )
{
	CA_SOFTCELL_INFO_T *pTemp;

	CA_PARAM_NOT_USED( wHandle );
	pTemp = (CA_SOFTCELL_INFO_T*)pCAClientMsg;

	pTemp->bMajor = stSoftCell.bMajor;
	pTemp->bMinor = stSoftCell.bMinor;
	pTemp->uSubVersion = stSoftCell.uSubVersion;

	ansi_memcpy(pTemp->acNameString, stSoftCell.acNameString, MAX_CLIENT_NAME_STR_LEN);
	ansi_memcpy(pTemp->acBuildInfoString, stSoftCell.acBuildInfoString, MAX_BUILD_INFO_STR_LEN);
	ansi_memcpy(pTemp->acCaTaskVerString, stSoftCell.acCaTaskVerString, MAX_CA_TASK_VER_STR_LEN);

	return CA_QUERY_SUCCESS;
}

/*
* Description: Query handler for CA_QC_COMPONENTS_INFO type.
*
* Parameters : pstSCell - Pointer to the data buffer provided by CA Client.
*
* Returns    : CA_QUERY_SUCCESS       - Successful.
*              CA_QUERY_FAIL          - Failed.
*/

static CA_QUERY_STATUS_EN ComponentsInfoQueryControl(ia_word32 wHandle, void *pCAClientMsg )
{
	ia_byte i;
	CA_COMPONENTS_INFO_T *pTemp;

	CA_PARAM_NOT_USED( wHandle );	/* to supress warning */

	pTemp = ( CA_COMPONENTS_INFO_T* )pCAClientMsg;
	if ( pTemp->bComCount > stComponents.bComCount )
	{
		pTemp->bComCount = stComponents.bComCount;
	}
	for ( i=0 ; i<pTemp->bComCount ; i++ )
	{
		pTemp->pstComponents[i].bMajor = stComponents.pstComponents[i].bMajor;
		pTemp->pstComponents[i].bMinor = stComponents.pstComponents[i].bMinor;
		pTemp->pstComponents[i].uSubVersion = stComponents.pstComponents[i].uSubVersion;

		ansi_memcpy(pTemp->pstComponents[i].acNameString, stComponents.pstComponents[i].acNameString, MAX_CLIENT_NAME_STR_LEN);
		ansi_memcpy(pTemp->pstComponents[i].acBuildInfoString, stComponents.pstComponents[i].acBuildInfoString, MAX_BUILD_INFO_STR_LEN);
		ansi_memcpy(pTemp->pstComponents[i].acCaTaskVerString, stComponents.pstComponents[i].acCaTaskVerString, MAX_CA_TASK_VER_STR_LEN);
	}
	return CA_QUERY_SUCCESS;
}

static void ParseErrorMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	msg_error_data_st *pstMsg;

	pstMsg = (msg_error_data_st*)(pstSCellMsg->pvMessage);

	CA_ERROR(("Opcode = 0x%02x, OpSeqNo = 0x%02x, wStatus = %d\n",
		pstMsg->eOpcode, pstMsg->wOpSeqNo, pstMsg->stStatus.wStatus));
}

/*jyou: use wGroupHandle as the OpSeqNo for service open request*/
/* ayang: fix SCR#27360,SCR#27365 */
void SendServiceOpenRequestMsg( ia_word16 wGroupHandle, ia_word16 wServiceType )
{
	msg_service_open_request_data_st *pstMsg;
	ia_word16 wCaSvcType = 0;

	CA_DBG_V(("<= MSG_SERVICE_OPEN_REQUEST( type %d) \n", wServiceType ));

	switch (wServiceType)
	{
		case SERVICE_DVB_DESCRAMBLE:
			wCaSvcType = CA_SERVICE_DESCRAMBLE;
			break;
		case SERVICE_DVB_EMM:
			wCaSvcType = CA_SERVICE_EMM;
			break;
		case SERVICE_PVR_RECORD:
			wCaSvcType = CA_SERVICE_RECORD;
			break;
		case SERVICE_PVR_PLAYBACK:
			wCaSvcType = CA_SERVICE_PLAYBACK;
			break;
		case SERVICE_PVR_DRM:
			if (wGroupHandle & 0x8000)
			{
				wCaSvcType = CA_SERVICE_PLAYBACK;
			}
			else
			{
				wCaSvcType = CA_SERVICE_RECORD;
			}
			break;
		default:
			break;
	}

	pstMsg = (msg_service_open_request_data_st*)ALLOCATE_MEMORY( msg_service_open_request_data_st, 1 );
	if( pstMsg != 0 )
	{
		pstMsg->wServiceType = wServiceType;
		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		ServiceHandleMechanism(MSG_SERVICE_OPEN_REQUEST, wGroupHandle, wCaSvcType, pstMsg );
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}
}

static void ParseServiceOpenReplyMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	msg_service_open_reply_data_st *pstMsg;
	ia_word32 wGroupHandle;
	ia_word32 wSvcType;
	/* jyou: 3.4.2 multiple smart cards */
	ia_word16 wOpSeqNo;
	ia_word16 wDemuxIndex;

	pstMsg = (msg_service_open_reply_data_st*)(pstSCellMsg->pvMessage);

	CA_DBG_V(("=> MSG_SERVICE_OPEN_REPLY( type %d ), index %d, handle = %d \n", pstMsg->wServiceType, pstSCellMsg->wOpSeqNo, pstMsg->wServiceHandle ));

	/*wScellHandle = pstMsg->wServiceHandle;*/

	if (pstMsg->wServiceType == SERVICE_DVB_EMM)
	{
		bServiceCount += 1;

		wOpSeqNo = pstSCellMsg->wOpSeqNo;
		wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo , 1 );

		CA_SVCINFO_SetSCellHandle(wOpSeqNo, CA_SERVICE_EMM, pstMsg->wServiceHandle);
		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (CA_SVCINFO_GetSCellHandleStatus(wOpSeqNo,CA_SERVICE_EMM) == CA_SERVICE_HANDLE_WAITCLOSE)
		{
			CA_SVCINFO_SetSCellHandleStatus(wOpSeqNo, CA_SERVICE_EMM, CA_SERVICE_HANDLE_OCCUPIED);
			ServicesClose(wOpSeqNo,CA_SERVICE_EMM);
			return;
		}
		CA_SVCINFO_SetSCellHandleStatus(wOpSeqNo, CA_SERVICE_EMM, CA_SERVICE_HANDLE_OCCUPIED);
		if (IA_TRUE == CA_TASK_CheckNotifyEnable(NOTIFY_MASK_MONITOR_EMM)) /* jyou: 2007-04-03 */
		{
			/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
			CA_MONITOR_MonitorControl(wOpSeqNo, CA_SERVICE_EMM, IA_TRUE);
		}
		AddDefaultResourcesToEmmService( wOpSeqNo );
		pstDMX[wDemuxIndex].fEmmServiceReady = IA_TRUE;
		AddEmmSourceToEmmService( wOpSeqNo );
		if (pstDMX[wDemuxIndex].fHasOOB == IA_TRUE)
		{
			AddOOBResourceToEmm( wOpSeqNo );
		}
	}

	if (pstMsg->wServiceType == SERVICE_DVB_DESCRAMBLE)
	{
		bServiceCount += 1;
		wGroupHandle = pstSCellMsg->wOpSeqNo;

		CA_SVCINFO_SetSCellHandle(wGroupHandle, CA_SERVICE_DESCRAMBLE, pstMsg->wServiceHandle);
		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (CA_SVCINFO_GetSCellHandleStatus(wGroupHandle,CA_SERVICE_DESCRAMBLE) == CA_SERVICE_HANDLE_WAITCLOSE)
		{
			CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, CA_SERVICE_DESCRAMBLE, CA_SERVICE_HANDLE_OCCUPIED);
			ServicesClose(wGroupHandle,CA_SERVICE_DESCRAMBLE);
			return;
		}
		CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, CA_SERVICE_DESCRAMBLE, CA_SERVICE_HANDLE_OCCUPIED);
#ifdef INCL_SERVICE_CONFIGURATION
		AddGeneralConfigSourceToDescrambleService(wGroupHandle, CA_SERVICE_DESCRAMBLE);
#endif
#ifndef EXCL_MATURITY_RATING
		AddMRSourceToService(wGroupHandle, CA_SERVICE_DESCRAMBLE);
#endif
		UpdateDefaultResourcesToDescrambleService(wGroupHandle, CA_SERVICE_DESCRAMBLE);
		if(pstService[wGroupHandle].fDesServiceReady == IA_FALSE)
			pstService[wGroupHandle].fDesServiceReady = IA_TRUE;
		AddEcmSourceToDescrambleService(wGroupHandle, CA_SERVICE_DESCRAMBLE);
		if (IA_TRUE == CA_TASK_CheckNotifyEnable(NOTIFY_MASK_MONITOR_ECM)) /* jyou: 2007-04-03 */
		{
			/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
			CA_MONITOR_MonitorControl(wGroupHandle, CA_SERVICE_DESCRAMBLE, IA_TRUE);
		}
	}
#ifdef INCL_SERVICE_DMB
	if( pstMsg->wServiceType == SERVICE_DMB_DESCRAMBLE )
	{
		bServiceCount += 1;
		wGroupHandle = pstSCellMsg->wOpSeqNo;

		CA_SVCINFO_SetSCellHandle(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE, pstMsg->wServiceHandle);
		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (CA_SVCINFO_GetSCellHandleStatus(wGroupHandle,CA_SERVICE_DMB_DESCRAMBLE) == CA_SERVICE_HANDLE_WAITCLOSE)
		{
			CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE, CA_SERVICE_HANDLE_OCCUPIED);
			ServicesClose(wGroupHandle,CA_SERVICE_DMB_DESCRAMBLE);
			return;
		}
		CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE, CA_SERVICE_HANDLE_OCCUPIED);
		UpdateDefaultResToDMBDescrambleService(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE,IA_TRUE);
		if(pstService[wGroupHandle].fDesServiceReady == IA_FALSE)
			pstService[wGroupHandle].fDesServiceReady = IA_TRUE;
		AddEcmResToDMBDescrambleService(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE,IA_TRUE);
		if (IA_TRUE == CA_TASK_CheckNotifyEnable(NOTIFY_MASK_DMB_ECM_SVC))
		{
			CA_MONITOR_MonitorControl(wGroupHandle, CA_SERVICE_DMB_DESCRAMBLE, IA_TRUE);  /* Enanle ECM Monitor */
		}
	}
	if( pstMsg->wServiceType == SERVICE_DMB_EMM )
	{
		bServiceCount += 1;
		wOpSeqNo = pstSCellMsg->wOpSeqNo;
		wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo , 1 );

		CA_SVCINFO_SetSCellHandle(wOpSeqNo, CA_SERVICE_DMB_EMM, pstMsg->wServiceHandle);
		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (CA_SVCINFO_GetSCellHandleStatus(wOpSeqNo,CA_SERVICE_DMB_EMM) == CA_SERVICE_HANDLE_WAITCLOSE)
		{
			CA_SVCINFO_SetSCellHandleStatus(wOpSeqNo, CA_SERVICE_DMB_EMM, CA_SERVICE_HANDLE_OCCUPIED);
			ServicesClose(wOpSeqNo,CA_SERVICE_DMB_EMM);
			return;
		}
		CA_SVCINFO_SetSCellHandleStatus(wOpSeqNo, CA_SERVICE_DMB_EMM, CA_SERVICE_HANDLE_OCCUPIED);
		if (IA_TRUE == CA_TASK_CheckNotifyEnable(NOTIFY_MASK_DMB_EMM_SVC))
		{
			CA_MONITOR_MonitorControl(wOpSeqNo, CA_SERVICE_DMB_EMM, IA_TRUE); /* Enanle EMM Monitor */
		}
		CA_DMB_AddDefaultResourceToEmmService(wOpSeqNo);
		pstDMX[wDemuxIndex].fEmmServiceReady = IA_TRUE;
		CA_DMB_AddEmmResourceToEmmService(wOpSeqNo);

		if (pstDMX[wDemuxIndex].fHasOOB == IA_TRUE)
		{
			/*The same as the DVB OOB added process*/
			AddOOBResourceToEmm( wOpSeqNo );
		}
	}
#endif
	if (pstMsg->wServiceType == SERVICE_PVR_RECORD)
	{
		bServiceCount += 1;
		wGroupHandle = pstSCellMsg->wOpSeqNo;
		CA_SVCINFO_SetSCellHandle(wGroupHandle, CA_SERVICE_RECORD, pstMsg->wServiceHandle);
		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (CA_SVCINFO_GetSCellHandleStatus(wGroupHandle,CA_SERVICE_RECORD) == CA_SERVICE_HANDLE_WAITCLOSE)
		{
			CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, CA_SERVICE_RECORD, CA_SERVICE_HANDLE_OCCUPIED);
			ServicesClose(wGroupHandle,CA_SERVICE_RECORD);
			return;
		}
		CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, CA_SERVICE_RECORD, CA_SERVICE_HANDLE_OCCUPIED);
		AddResourceToPvrRecordService(wGroupHandle);

	}

	if (pstMsg->wServiceType == SERVICE_PVR_PLAYBACK)
	{
		bServiceCount += 1;
		wGroupHandle = pstSCellMsg->wOpSeqNo;

		CA_SVCINFO_SetSCellHandle(wGroupHandle, CA_SERVICE_PLAYBACK, pstMsg->wServiceHandle);
		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (CA_SVCINFO_GetSCellHandleStatus(wGroupHandle,CA_SERVICE_PLAYBACK) == CA_SERVICE_HANDLE_WAITCLOSE)
		{
			CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, CA_SERVICE_PLAYBACK, CA_SERVICE_HANDLE_OCCUPIED);
			ServicesClose(wGroupHandle,CA_SERVICE_PLAYBACK);
			return;
		}
		CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, CA_SERVICE_PLAYBACK, CA_SERVICE_HANDLE_OCCUPIED);
		AddResourceToPvrPlayBackService(wGroupHandle);
	}


	if (pstMsg->wServiceType == SERVICE_PVR_DRM)
	{
		bServiceCount += 1;
		wGroupHandle = pstSCellMsg->wOpSeqNo;
		if (wGroupHandle & 0x8000)
		{
			wSvcType = CA_SERVICE_PLAYBACK;
			/* mli: get rid of the highest bit after obtained the service type */
			wGroupHandle = wGroupHandle & 0x7FFF;
		}
		else
		{
			wSvcType = CA_SERVICE_RECORD;
		}

		CA_SVCINFO_SetSCellHandle(wGroupHandle, wSvcType, pstMsg->wServiceHandle);
		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (CA_SVCINFO_GetSCellHandleStatus(wGroupHandle,wSvcType) == CA_SERVICE_HANDLE_WAITCLOSE)
		{
			CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, wSvcType, CA_SERVICE_HANDLE_OCCUPIED);
			ServicesClose(wGroupHandle,wSvcType);
			return;
		}
		CA_SVCINFO_SetSCellHandleStatus(wGroupHandle, wSvcType, CA_SERVICE_HANDLE_OCCUPIED);

		if (wSvcType == CA_SERVICE_RECORD)
		{
			AddResourceToPvrRecordServiceForDrm(wGroupHandle);
		}
		if (wSvcType == CA_SERVICE_PLAYBACK)
		{
			AddResourceToPvrPlayBackServiceForDrm(wGroupHandle);
#ifndef EXCL_MATURITY_RATING
			AddMRSourceToService((wGroupHandle), CA_SERVICE_PLAYBACK);
#endif
		}
	}
}


static void ParseServiceCloseReplyMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	CA_DBG_V(("=> MSG_SERVICE_CLOSE_REPLY\n"));

	bServiceCount -= 1;

	if( ( fReadyCloseECT == IA_TRUE ) && ( bServiceCount == 0 ) )
	{
		unsigned char terminationData[ 8 ] = { 0 };
		int           len = 6;

		CA_PARAM_NOT_USED( pstSCellMsg );

		SCELL_Close( );

		OS_DRV_DeleteSemaphore(gECTStartUpSemap);
		terminationData[ 0 ] = (ia_byte)(( len & 0xFF00 ) >> 8); /* length */
		terminationData[ 1 ] = (ia_byte)( len & 0x00FF );

		OS_DRV_DeleteSemaphore( gMessageQueueSemap );

		/* Fix SCR#26646: ayang 07/12/04 */
		OS_DRV_DeleteSemaphore( gSCellMsgSemap );

		CA_TASK_SendMessage( CA_CLIENT__TERMINATION, terminationData );
	}
}

/*
* Description: Adds default resources (smartcard) to EMM service.
*
* Parameters : wOpSeqNo - DEMUX_index<<8 | smartcard_index.
*
* Returns    : None.
*/
static void AddDefaultResourcesToEmmService(ia_word16 wOpSeqNo)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	ia_word16 wScellHandle;

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: +sc)\n"));
	wScellHandle = CA_SVCINFO_GetSCellHandle(wOpSeqNo, CA_SERVICE_EMM);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}

	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_define_data_st) + sizeof(msg_resource_st),
		sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_SMARTCARD;
	/* jyou: 3.4.2 multiple smartcard; get the smart card id by OpSeqNo */
	/* the returned service cannot be 0 since otherwise it will return invalid SCellHandle */
	stRsc.wResourceId   = ( CA_SVCINFO_GetEMMServiceByOpSeqNo(wOpSeqNo) )->wSmartCardId;
	stRsc.wResourceMgnt = MANAGE_UPDATE; /* jyou: 3.4.3.5 SCR#26041 */
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wOpSeqNo,CA_SERVICE_EMM,pvMsg);
}

static void UpdateDefaultResourcesToDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType)
{
	ia_msg_handle				pvMsg;
	msg_service_define_data_st	stSvcDefData;
	msg_resource_st				stRsc;
	/* ayang 2008-04-09 Changed type of wCopyControlRscID from ia_word16 to ia_word32 */
	ia_word16				wScellSvcHandle;
	ia_word32 				wCopyControlRscID=INVALID_RESOURCE_ID;
	CA_SERVICE_HEADER		stCaSvcGrpDefHeader;


	if(pstService[wGroupHandle].fDesServiceReady == IA_TRUE)
	{
		CA_ERROR(("pstService[wGroupHandle].fDesServiceReady == IA_TRUE.\n"));
		return;
	}

	/* wDemux = CA_SVCINFO_GetDesSvcDemuxFromServiceId(wService); */
	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_DESCRAMBLE;      /* it's desc service */
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_ERROR(("Can't get the service header.\n"));
		return;
	}

	wScellSvcHandle = CA_SVCINFO_GetSCellHandle( wGroupHandle, wSrvType );
	if ( INVALID_SCELL_SERVICE_HANDLE == wScellSvcHandle )
	{
		/* Message not sent. We must free the buffer. */
		CA_ERROR(("Can't get the valid softcell service handle.\n"));
		return;
	}

#ifdef CA_SUPPORT_COPYCONTROL	/* jyou: 3.6.1 SCR#26708 2007-12-26 */
	wCopyControlRscID = CA_SVCINFO_GetResourceID( (ia_word16)wGroupHandle,
		CA_SERVICE_DESCRAMBLE, RESOURCE_COPY_CONTROL );
#endif
	if ( INVALID_RESOURCE_ID != wCopyControlRscID )
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (DES: +des, +msc, +cc)\n"));
		stSvcDefData.bResourceCount = 2 + CA_SCARD_ValidSmartCards();
	}
	else
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (DES: +des, +msc)\n"));
		stSvcDefData.bResourceCount = 1 + CA_SCARD_ValidSmartCards();
	}
	pvMsg = IA_OpenOutputMessage( (ia_word16)(sizeof(msg_service_define_data_st) + sizeof(msg_resource_st) * stSvcDefData.bResourceCount), (ia_word16)sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wScellSvcHandle;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_DVB_DESCRAMBLER;
	stRsc.wResourceId   = stCaSvcGrpDefHeader.wDemuxId;  /* the resource id isn't used */
	stRsc.wResourceMgnt = MANAGE_UPDATE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* jyou: 3.4.2 multiple smartcard; add all resources for descramble service */
	PrepareSmartCardResources( MANAGE_UPDATE, pvMsg );

	/* jyou: 3.6.1 SCR#26708 2007-12-26 */
	if ( INVALID_RESOURCE_ID != wCopyControlRscID )
	{
		stRsc.wResourceType = RESOURCE_COPY_CONTROL;
		stRsc.wResourceId   = wCopyControlRscID;
		stRsc.wResourceMgnt = MANAGE_UPDATE;
		CA_DBG_V(("[ECT][UpdateDefaultResourcesToDescrambleService] Copy Control Resource ID=%d\n",
			wCopyControlRscID));
		IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );
	}

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_DESCRAMBLE,pvMsg);
}

static void CloseDefaultResourcesToDescrambleService( ia_word16 wGroupHandle )
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	/* ayang 2008-04-09 Changed type of wCopyControlRscID from ia_word16 to ia_word32 */
	ia_word16				wScellSvcHandle;
	ia_word32 				wCopyControlRscID=INVALID_RESOURCE_ID;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;


	/* wDemux = CA_SVCINFO_GetDesSvcDemuxFromServiceId(wService); */
	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_DESCRAMBLE;      /* it's desc service */
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
		return;

	wScellSvcHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_DESCRAMBLE);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellSvcHandle)
	{
		/* Message not sent. We must free the buffer. */
		CA_ERROR(("Can't get the valid softcell service handle.\n"));
		return;
	}

#ifdef CA_SUPPORT_COPYCONTROL	/* jyou: 3.6.1 SCR#26708 2007-12-26 */
	wCopyControlRscID = CA_SVCINFO_GetResourceID( (ia_word16)wGroupHandle,
		CA_SERVICE_DESCRAMBLE, RESOURCE_COPY_CONTROL );
#endif
	if ( INVALID_RESOURCE_ID != wCopyControlRscID )
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (DES: -des, -cc)\n"));
		stSvcDefData.bResourceCount = 2 ; /* jyou: 3.4.3.5 SCR#26041*/
	}
	else
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (DES: -des)\n"));
		stSvcDefData.bResourceCount = 1 ; /* jyou: 3.4.3.5 SCR#26041*/
	}
	pvMsg = IA_OpenOutputMessage( (ia_word16)(sizeof(msg_service_define_data_st) + sizeof(msg_resource_st) * stSvcDefData.bResourceCount), (ia_word16)sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wScellSvcHandle;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_DVB_DESCRAMBLER;
	stRsc.wResourceId   = stCaSvcGrpDefHeader.wDemuxId;  /* the resource id isn't used */
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* jyou: 2008-02-05 Should not close the smart card resource when stopping services */

	/* jyou: 3.6.1 SCR#26708 2007-12-26 */
	if ( INVALID_RESOURCE_ID != wCopyControlRscID )
	{
		stRsc.wResourceType = RESOURCE_COPY_CONTROL;
		stRsc.wResourceId   = wCopyControlRscID;
		stRsc.wResourceMgnt = MANAGE_CLOSE;
		CA_DBG_V(("[ECT][CloseDefaultResourcesToDescrambleService] Copy Control Resource ID=%d\n",
			wCopyControlRscID));
		IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );
	}

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_DESCRAMBLE,pvMsg);
}


/*
* Description: Adds EMM source resource to EMM service.
*
* Parameters : wOpSeqNo - emm_index<<8 | smartcard_index.
*
* Returns    : None.
*/
static void AddEmmSourceToEmmService(ia_word16 wOpSeqNo )
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_emm_source_st stEmmSrc;
	msg_descriptor_st stDescr;
	ia_byte *pbCaDescr = 0;
	ia_word16 i, wDemuxIndex;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wSCellSrvHandle = 0;


	if (0 == pstDMX)
		return;

	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_ERROR(("AddEMMSource: Invalid demux index.\n"));
		return;
	}

	if (0 == pstDMX[wDemuxIndex].pbCatInfo)
	{
		CA_ERROR(("The CAT info is not available.\n"));
		return;
	}

	if (IA_FALSE == pstDMX[wDemuxIndex].fEmmServiceReady)
	{
		CA_ERROR(("The EMM service is not ready.\n"));
		return;
	}

	wSCellSrvHandle = CA_SVCINFO_GetSCellHandle( wOpSeqNo, CA_SERVICE_EMM );
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellSrvHandle)
		return;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_EMM;      /* it's emm service */
	if (CA_SVCINFO_GetSvcHeaderInfo( wDemuxIndex, &stCaSvcGrpDefHeader) == IA_FAIL)
		return;

	/* jyou: 3.4.2 fixed pbCatInfo[18]-->[10] number of descriptors */
	pvMsg = IA_OpenOutputMessage( (ia_word16)(sizeof(msg_service_define_data_st)+sizeof(msg_resource_st)+
		sizeof(msg_emm_source_st)+
		pstDMX[wDemuxIndex].pbCatInfo[10]*sizeof(msg_descriptor_st)),
		(ia_word16)sizeof(msg_descriptor_st) );

	stSvcDefData.wServiceHandle = wSCellSrvHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_EMM_SOURCE;
	stRsc.wResourceId   = stCaSvcGrpDefHeader.wDemuxId;
	/* MANAGE_UPDATE will update a resource for a service. If the resource was not added yet
	then it will be added to the service. */
	if(pstDMX[wDemuxIndex].fCatSent == IA_FALSE)
	{
		CA_DBG_V(("=> Demux %x CAT not sent, add resource.\n", wDemuxIndex));
		stRsc.wResourceMgnt = MANAGE_ADD;
	}
	else
	{
		CA_DBG_V(("=> Demux %x CAT sent, update resource.\n", wDemuxIndex));
		stRsc.wResourceMgnt = MANAGE_UPDATE;
	}

	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	/* Start to parse the CAT data. */
	stEmmSrc.bDescrCount = pstDMX[wDemuxIndex].pbCatInfo[10];
	CA_DBG_V(("system_count = %d\n", stEmmSrc.bDescrCount));

	IA_WriteToMessage( pvMsg, &stEmmSrc, sizeof(msg_emm_source_st) );

	if( stEmmSrc.bDescrCount != 0 )
	{
		pbCaDescr = (ia_byte*)(&pstDMX[wDemuxIndex].pbCatInfo[11]);
		for( i=0; i<stEmmSrc.bDescrCount; i++ )
		{
			stDescr.bTag = pbCaDescr[0];
			stDescr.bDataLength = pbCaDescr[1];
			IA_WriteToMessage( pvMsg, &stDescr, sizeof(msg_descriptor_st) );
			/* Payload of the descriptor excluding tag and length. */
			IA_WriteToMessage( pvMsg, &pbCaDescr[2], stDescr.bDataLength );
			pbCaDescr += stDescr.bDataLength + 2;
		}
	}

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: +cat)\n"));

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	if (ServiceHandleMechanism(MSG_SERVICE_DEFINE,wOpSeqNo,CA_SERVICE_EMM,pvMsg) == IA_TRUE)
	{
		pstDMX[wDemuxIndex].fCatSent = IA_TRUE;
	}
	return;
}

/* jyou: 3.4.3.5 SCR#26042 */
/*
* Description: Updates EMM source resource from EMM service with an empty CAT.
*
* Parameters : wOpSeqNo - DEMUX channel number <<8 | smartcard_index.
*
* Returns    : None.
*/
static void UpdateEmmSourceToEmmService(ia_word16 wOpSeqNo)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_emm_source_st stEmmSrc;
	ia_word16 wDemuxIndex;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wSCellSrvHandle = 0;


	if (0 == pstDMX)
		return;

	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_DBG(("AddEMMSource: Invalid demux index.\n"));
		return;
	}

	wSCellSrvHandle = CA_SVCINFO_GetSCellHandle( wOpSeqNo, CA_SERVICE_EMM );
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellSrvHandle)
		return;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_EMM;      /* it's emm service */
	if (CA_SVCINFO_GetSvcHeaderInfo( wDemuxIndex, &stCaSvcGrpDefHeader) == IA_FAIL)
		return;

	pvMsg = IA_OpenOutputMessage( (ia_word16)(sizeof(msg_service_define_data_st)+sizeof(msg_resource_st)+
		sizeof(msg_emm_source_st) ),  (ia_word16)sizeof(msg_descriptor_st) );

	stSvcDefData.wServiceHandle = wSCellSrvHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(msg_service_define_data_st) );

	stRsc.wResourceType = RESOURCE_EMM_SOURCE;
	stRsc.wResourceId   = stCaSvcGrpDefHeader.wDemuxId;
	stRsc.wResourceMgnt = MANAGE_UPDATE;

	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	/* Start to parse the CAT data. */
	stEmmSrc.bDescrCount = 0;
	CA_DBG_V(("system_count = %d\n", stEmmSrc.bDescrCount));

	IA_WriteToMessage( pvMsg, &stEmmSrc, sizeof(msg_emm_source_st) );

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: -cat)\n"));

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	if (ServiceHandleMechanism(MSG_SERVICE_DEFINE,wOpSeqNo,CA_SERVICE_EMM,pvMsg) == IA_TRUE)
	{
		pstDMX[wDemuxIndex].fCatSent = IA_TRUE;
	}
	return;
}

#if 0
/*
* Description: Removes EMM source resource from EMM service.
*
* Parameters : wDemux - DEMUX channel number.
*
* Returns    : None.
*/
static void RemoveEmmSourceFromEmmService(ia_word16 wOpSeqNo)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	ia_word16 wSCellHandle = 0, wDemuxIndex;
	ia_word32 wDemuxId = 0;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;

	if (0 == pstDMX)
	{
		return;
	}
	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_DBG(("RemoveEMMSource: Invalid demux index.\n"));
		return;
	}

	if (pstDMX[ wDemuxIndex ].fCatSent == IA_FALSE)
	{
		return;
	}

	wSCellHandle = CA_SVCINFO_GetSCellHandle(wOpSeqNo, CA_SERVICE_EMM);
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_EMM;
	CA_SVCINFO_GetSvcHeaderInfo( wDemuxIndex, &stCaSvcGrpDefHeader);
	wDemuxId = stCaSvcGrpDefHeader.wDemuxId;
	if (INVALID_GROUP_SERVICE_HANDLE == wDemuxId)
	{
		return;
	}

	pvMsg = IA_OpenOutputMessage( 32, 16 );

	stSvcDefData.wServiceHandle = wSCellHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_EMM_SOURCE;
	stRsc.wResourceId   = wDemuxId;
	/* MANAGE_CLOSE will close the resource and delete it from the service. */
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: -cat)\n"));

	if( SCELL_Message( MSG_SERVICE_DEFINE, (ia_word16)wOpSeqNo, IA_GetOutputMessageLength(pvMsg),
		IA_GetOutputMessage(pvMsg) ) == IA_SUCCESS )
	{
		pstDMX[ wDemuxIndex ].fCatSent = IA_FALSE ;
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage( pvMsg, IA_FALSE );
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
	return;
}
#endif

/*
* Description: Adds ECM source resource to DESCRAMBLE service.
*
* Parameters : wDemux - DEMUX channel number.
*
* Returns    : None.
*/
static void AddEcmSourceToDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_ecm_source_st stEcmSrc;
	msg_descriptor_st stDescr;
	msg_dvb_stream_st stStream;
	/* ia_byte *pbPMT, *pbCaDescr; */
	ia_byte *pbPMT = 0, *pbDescr = 0;
	ia_word16 i, j, k, wOffset = 0;
	ia_bool fClear = IA_TRUE;
	ia_word16 wScellHandle;
	ia_word32 wSmartCardId;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;

	stCaSvcGrpDefHeader.wServiceType = wSrvType;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_ERROR(("AddEcmSourceToDescrambleService Can't find the service header.\n"));
		return;
	}
	if (0 == pstService)
	{
		CA_ERROR(("AddEcmSourceToDescrambleService pstService = 0.\n"));
		return;
	}

	if (0 == pstService[wGroupHandle].pbPmtInfo)
	{
		CA_ERROR(("AddEcmSourceToDescrambleService The PMT info is not available.\n"));
		return;
	}

	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, wSrvType);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_ERROR(("AddEcmSourceToDescrambleService The Softcell service handle is not available.\n"));
		return;
	}

	pvMsg = IA_OpenOutputMessage(128 , 32 );
		if (pvMsg == 0)
		{
				return;
		}
	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_ECM_SOURCE;
	stRsc.wResourceId   = stCaSvcGrpDefHeader.wDemuxId;
	/* MANAGE_UPDATE will update a resource for a service. If the resource was not added yet
	then it will be added to the service. */
	if( pstService[wGroupHandle].fEcmSrcAdded == IA_FALSE )
	{
		stRsc.wResourceMgnt = MANAGE_ADD;
	}
	else
	{
		stRsc.wResourceMgnt = MANAGE_UPDATE;
	}

	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* Start to parse the PMT data. */
	pbPMT = (ia_byte*)(&pstService[wGroupHandle].pbPmtInfo[16]);

	stEcmSrc.bGlobalCount = pbPMT[0];
	CA_DBG_V(("AddEcmSourceToDescrambleService glbdescrcount = %d\n", stEcmSrc.bGlobalCount));

	wOffset = 1;

	for (i=0; i < stEcmSrc.bGlobalCount; i++)
	{
		/* Skip the global CA_descriptors. */
		wOffset += pbPMT[wOffset+1]+2;
	}
	stEcmSrc.bStreamCount = pbPMT[wOffset];
	wOffset++;
	CA_DBG(("AddEcmSourceToDescrambleService streamcount = %d\n", stEcmSrc.bStreamCount));
	if( stEcmSrc.bStreamCount > MAX_SOFTCELL_STREAMS )
	{
		stEcmSrc.bStreamCount = MAX_SOFTCELL_STREAMS;
	}

	IA_WriteToMessage( pvMsg, &stEcmSrc, sizeof(stEcmSrc) );

	pbDescr = (ia_byte*)(&pbPMT[1]);
	for (i=0; i < stEcmSrc.bGlobalCount; i++)
	{
		stDescr.bTag = pbDescr[0];
		if (9 == stDescr.bTag)
		{
			fClear = IA_FALSE;
		}
		stDescr.bDataLength = pbDescr[1];
		IA_WriteToMessage( pvMsg, &stDescr, sizeof(stDescr) );
		/* Payload of the descriptor excluding tag and length. */
		IA_WriteToMessage( pvMsg, &pbDescr[2], stDescr.bDataLength );

		oldCAPID = (ia_word16)(((pbDescr[ 4 ] & 0x1F) << 8) + pbDescr[ 5 ] );
		CA_DBG_V(("gobal bDescrCount = %d, tag = 0x%2x, current PID=0x%04x\n", stEcmSrc.bGlobalCount, stDescr.bTag, oldCAPID ));

		wSmartCardId = stCaSvcGrpDefHeader.wSmartCardId;
		CA_TASK_SetLockedService(wGroupHandle, wSmartCardId); /* jyou: ??? assign grouphandle of smartcard how to deal in */
		pbDescr += stDescr.bDataLength + 2;

	}

	for( j=0; j<stEcmSrc.bStreamCount; j++ )
	{
		stStream.wPid = (ia_word16)(((pbPMT[wOffset] & 0x1F) << 8) + pbPMT[wOffset+1]);
		stStream.bDescrCount = pbPMT[wOffset+2]; /*mli*/
		IA_WriteToMessage( pvMsg, &stStream, sizeof(stStream) );
		CA_DBG_V(("<2788>streamPID = 0x%04x, strdescrcount = %d\n", stStream.wPid, stStream.bDescrCount));
		wOffset += 3; /*mli*/

		pbDescr = (ia_byte*)(&pbPMT[wOffset]);
		for( k=0; k<stStream.bDescrCount; k++ )
		{
			stDescr.bTag = pbDescr[0];
			if (9 == stDescr.bTag)
			{
				fClear = IA_FALSE;
			}
			stDescr.bDataLength = pbDescr[1];
			IA_WriteToMessage( pvMsg, &stDescr, sizeof(stDescr) );
			/* Payload of the descriptor excluding tag and length. */
			IA_WriteToMessage( pvMsg, &pbDescr[2], stDescr.bDataLength );

			oldCAPID = (ia_word16)(((pbDescr[ 4 ] & 0x1F) << 8) + pbDescr[ 5 ] );

			/* wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, wSrvType); */
			wSmartCardId = stCaSvcGrpDefHeader.wSmartCardId;
			CA_TASK_SetLockedService(wGroupHandle, wSmartCardId);
			CA_DBG_V(("in stream bDescrCount = %d, tag = 0x%2x\n", stStream.bDescrCount, stDescr.bTag ));

			pbDescr += stDescr.bDataLength + 2;
			wOffset += stDescr.bDataLength + 2;

		}
	}
	pstService[wGroupHandle].fClear = IA_FALSE;

	if( fClear == IA_TRUE )
	{
		/* This is a clear service. */
		pstService[wGroupHandle].fClear = IA_TRUE;

		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,wSrvType,pvMsg) == IA_TRUE)
		{
			pstService[wGroupHandle].fEcmSrcAdded = IA_TRUE;
		}
	}
	else if (pstService[wGroupHandle].fDesServiceReady == IA_FALSE)
	{
		CA_DBG(("The DESCRAMBLE service is not ready.\n"));

		/* Message not sent. We must free the buffer. */
		IA_CloseMessage(pvMsg, IA_TRUE);
	}
	else
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (DES: +pmt)\n"));

		/* ayang 2008-7-29 Fix SCR#27360,27365 */
		if (ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,wSrvType,pvMsg) == IA_TRUE)
		{
			pstService[wGroupHandle].fEcmSrcAdded = IA_TRUE;
		}
	}
}

/*
* Description: Updates ECM source resource to DESCRAMBLE service.  PVCS SCR 23759 mli
*
* Parameters : wService - Service number.
*
* Returns    : None.
*/
static void UpdateEcmSourceToDescrambleService(ia_word32 wGroupHandle)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_ecm_source_st stEcmSrc;
	/* ia_byte *pbPMT, *pbCaDescr; */
	/* ia_bool fClear = IA_TRUE; */
	ia_word16 wScellHandle;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_DESCRAMBLE;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}
	if (0 == pstService)
	{
		return;
	}

	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_DESCRAMBLE);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("The Softcell service handle is not available.\n"));
		return;
	}

	pvMsg = IA_OpenOutputMessage( 128, 32 );
	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(msg_service_define_data_st) );

	stRsc.wResourceType = RESOURCE_ECM_SOURCE;
	stRsc.wResourceId   = stCaSvcGrpDefHeader.wDemuxId;
	/* MANAGE_UPDATE will update a resource for a service. If the resource was not added yet
	then it will be added to the service. */
	stRsc.wResourceMgnt = MANAGE_UPDATE;

	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	/* Start to parse the PMT data. */

	stEcmSrc.bGlobalCount = 0;
	CA_DBG_V(("glbdescrcount = %d\n", stEcmSrc.bGlobalCount));
	stEcmSrc.bStreamCount = 0;
	CA_DBG_V(("streamcount = %d\n", stEcmSrc.bStreamCount));
	IA_WriteToMessage( pvMsg, &stEcmSrc, sizeof(msg_ecm_source_st) );

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	if (ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_DESCRAMBLE,pvMsg) == IA_TRUE)
	{
		pstService[wGroupHandle].fEcmSrcAdded = IA_TRUE;
	}

}

/*
* Description: Removes ECM source resource from DESCRAMBLE service.
*
* Parameters : wDemux - DEMUX channel number.
*
* Returns    : None.
*/
static void RemoveEcmSourceFromDescrambleService(ia_word32 wGroupHandle)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wScellSvcHandle;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_DESCRAMBLE;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}

	wScellSvcHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_DESCRAMBLE);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellSvcHandle)
	{
		/* Message not sent. We must free the buffer. */
		CA_ERROR(("Can't get the valid softcell service handle.\n"));
		return;
	}

	pvMsg = IA_OpenOutputMessage( 32, 16 );

	stSvcDefData.wServiceHandle = wScellSvcHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_ECM_SOURCE;
	stRsc.wResourceId   = stCaSvcGrpDefHeader.wDemuxId;
	/* MANAGE_CLOSE will close the resource and delete it from the service. */
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (DES: -pmt)\n"));

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	if (ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_DESCRAMBLE,pvMsg) == IA_TRUE)
	{
		pstService[wGroupHandle].fEcmSrcAdded = IA_FALSE;       /* mli PVCS SCR:23759 */
		pstService[wGroupHandle].fDesServiceReady = IA_FALSE;       /* mli PVCS SCR:23759 */
	}
}

static void AddMRSourceToService(ia_word32 wGroupHandle, ia_word32 wSrvType)
{
	ia_msg_handle				pvMsg;
	msg_service_define_data_st	stSvcDefData;
	msg_resource_st				stRsc;
	ia_word16					wScellHandle;
	CA_SERVICE_HEADER			stCaSvcGrpDefHeader;
	msg_mr_define_st stMr;

	if (stResourceMap.stMRMap.wResourceId > 0xf)
	{
		return;
	}

	stCaSvcGrpDefHeader.wServiceType = wSrvType;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}
	if (0 == pstService)
	{
		return;
	}

	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, wSrvType);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("The Softcell service handle is not available.\n"));
		return;
	}

	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_define_data_st)+sizeof(msg_resource_st)+sizeof(msg_mr_define_st), sizeof(msg_resource_st) );
	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(stSvcDefData) );

	stRsc.wResourceType = RESOURCE_MATURITY_RATING;
	stRsc.wResourceId   = (vd_rsc_id)stResourceMap.stMRMap.wResourceId;
	stRsc.wResourceMgnt = MANAGE_UPDATE;

	stMr.abMRPin[0] = stResourceMap.stMRMap.abMRPin[0];
	stMr.abMRPin[1] = stResourceMap.stMRMap.abMRPin[1];

	IA_WriteToMessage( pvMsg, &stRsc, sizeof(stRsc) );
	IA_WriteToMessage( pvMsg, &stMr, sizeof( stMr) );

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,wSrvType,pvMsg);

	stResourceMap.stMRMap.wResourceId = 0xffff;
	stResourceMap.stMRMap.abMRPin[0] =
	stResourceMap.stMRMap.abMRPin[1] = 0xff;

}

static void AddGeneralConfigSourceToDescrambleService(ia_word32 wGroupHandle, ia_word32 wSrvType)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_general_configuration_st stGeneralConfig;
	msg_config_descriptor_st stConfigDesc;

	ia_word16 i;
	ia_word16 wScellHandle;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;

	if (stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount == 0)
	{
		return;
	}

	stCaSvcGrpDefHeader.wServiceType = wSrvType;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_ERROR(("AddGeneralConfigSourceToDescrambleService Can't find the service header.\n"));
		return;
	}
	if (0 == pstService)
	{
		CA_ERROR(("AddGeneralConfigSourceToDescrambleService pstService = 0.\n"));
		return;
	}

	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, wSrvType);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_ERROR(("AddGeneralConfigSourceToDescrambleService The Softcell service handle is not available.\n"));
		return;
	}

	pvMsg = IA_OpenOutputMessage(128 , 32 );
	if (pvMsg == 0)
	{
			return;
	}
	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_GENERAL_CONFIGURATION;
	stRsc.wResourceId   = 0;
	stRsc.wResourceMgnt = MANAGE_UPDATE;

	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* Start to parse the Config data. */

	stGeneralConfig.bConfigCount = stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount;
	CA_DBG_V(("AddGeneralConfigSourceToDescrambleService ConfigCount = %d\n", stGeneralConfig.bConfigCount));
	IA_WriteToMessage( pvMsg, &stGeneralConfig, sizeof(msg_general_configuration_st) );

	for (i=0; i < stGeneralConfig.bConfigCount; i++)
	{
		stConfigDesc.bTag = stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.bTag;
		stConfigDesc.wDataLength = stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength;
		IA_WriteToMessage( pvMsg, &stConfigDesc, sizeof(msg_config_descriptor_st) );
		/* Payload of the descriptor excluding tag and length. */
		if (stConfigDesc.wDataLength != 0)
		{
			IA_WriteToMessage( pvMsg, &stResourceMap.stGeneralConfig.stConfigDesc[i].bData[0], stConfigDesc.wDataLength );
		}
	}

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,wSrvType,pvMsg);

	/* free stResourceMap.stGeneralConfig */
	for (i=0; i < stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount; i++ )
	{
		if (stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength != 0)
		{
			FREE_MEMORY(stResourceMap.stGeneralConfig.stConfigDesc[i].bData);
			stResourceMap.stGeneralConfig.stConfigDesc[i].bData = 0;
		}
	}
	FREE_MEMORY(stResourceMap.stGeneralConfig.stConfigDesc);
	stResourceMap.stGeneralConfig.stConfigDesc = 0;
	stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount = 0;
}

static void ParseServiceStatusMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_word32 wServiceType = 0;
	ia_word32 wService = 0;
	msg_service_status_data_st stSvcStatusData;
	ia_msg_handle pvMsg;
	ia_word16 wServiceHandle = 0;

	pvMsg = IA_OpenInputMessage(pstSCellMsg->pvMessage, pstSCellMsg->wLength);
	if (pvMsg == 0)
		return;
	if (IA_ReadFromMessage(pvMsg, &stSvcStatusData, sizeof(stSvcStatusData)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}
	CA_DBG_V(("Service Handle = %d, source = %d, Status = %d, severity = %d, RscCount = %d, Srvsource = %d, SrvStatus = %d, Srvseverity = %d \n",
		stSvcStatusData.wServiceHandle,
		stSvcStatusData.stStatus.eSource,
		stSvcStatusData.stStatus.wStatus,
		stSvcStatusData.stStatus.eSeverity,
		stSvcStatusData.bResourceCount,
		stSvcStatusData.stServiceStatus.eSource,
		stSvcStatusData.stServiceStatus.wStatus,
		stSvcStatusData.stServiceStatus.eSeverity
		));


	wServiceHandle = stSvcStatusData.wServiceHandle;

	/* Get service type and demux nr from acquired service handle. */
	wServiceType = CA_SVCINFO_GetSvcTypeFromSCellHandle(wServiceHandle); /* wServiceType is softcell service */
	wService = CA_SVCINFO_HandleSCell2CaClient(wServiceHandle); /* wService is demux index or group handle */

	if (wService >= CA_SVCINFO_GetSvcMaxNumber())
	{
		CA_ERROR(("wService handle is too big 0x%04x.\n", wService));
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}

	if (wServiceType == INVALID_SERVICETYPE)
	{
		CA_ERROR(("No svc type found for svc handle 0x%04x.\n", wServiceHandle));
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}

	switch(wServiceType)
	{
	case CA_SERVICE_EMM:
		/* wService is the index of the demux */
		if(IA_TRUE==CA_SVCINFO_ParseEMMServiceInfo( &stSvcStatusData, pstDMX, pvMsg))
			return;
		break;
#ifdef INCL_SERVICE_DMB
				case CA_SERVICE_DMB_EMM:
		/* wService is the index of the demux */
		if(IA_TRUE==CA_SVCINFO_ParseEMMServiceInfo( &stSvcStatusData, pstDMX, pvMsg))
			return;
		break;
#endif
	case CA_SERVICE_DESCRAMBLE:
		if(IA_TRUE==CA_SVCINFO_ParseDescrambleServiceInfo(CA_SERVICE_DESCRAMBLE, wService, &stSvcStatusData, pstService,pvMsg))
			return;
		break;
#ifdef INCL_SERVICE_DMB
			 case CA_SERVICE_DMB_DESCRAMBLE:
		if(IA_TRUE==CA_DMB_ParseDescrambleInfo(CA_SERVICE_DMB_DESCRAMBLE, wService, &stSvcStatusData, pvMsg))
			return;
		break;
#endif
	case CA_SERVICE_RECORD:
	case CA_SERVICE_PLAYBACK:
		if(IA_TRUE==CA_SVCINFO_ParsePVRServiceInfo( wServiceType,  wService,  &stSvcStatusData,  pvMsg))
			return;
		break;

	default:
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}

	#ifndef TO_SOFTCELL_3_0
	/*if(astService[wDemux].fClear == IA_FALSE || wServiceType == SERVICE_DVB_EMM)*/
	{
		/*	CA_ERROR_UpdateErrorStatus( wDemux, wServiceType, wServiceHandle,  &stSvcStatusData.stStatus ); */
	}
	#endif

	IA_CloseMessage(pvMsg, IA_FALSE);
}


static void ParseSmartcardStatusMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg;
	msg_smartcard_status_data_st stScStatusData;
	ia_word16 wOldSCardStatus;
	ia_word16 wSCardStatus;
	ia_word16 wMaxGrpNum = 0;
	ia_word32 wSCardId = INVALID_SC_SMARTCARDID;
	ia_status_st stSCardStatus;

#ifdef _AUSTAR_
	SynScMessage = 0;
#endif

	wMaxGrpNum = CA_SVCINFO_GetSvcMaxNumber();

	ansi_memset((void *)&stSCardStatus, 0, sizeof(ia_status_st));
	pvMsg = IA_OpenInputMessage( pstSCellMsg->pvMessage, pstSCellMsg->wLength );
	if (pvMsg == 0)
	{
		return;
	}
	if (IA_ReadFromMessage( pvMsg, &stScStatusData, sizeof(stScStatusData)) == 0)
	{
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}
	CA_DBG_V(("Smartcard RscId = %d, Status = %d\n", stScStatusData.wResourceId,
		stScStatusData.stStatus.wStatus));

	wSCardId = stScStatusData.wResourceId;

	CA_SCARD_SetSmartCardId(wSCardId);
	CA_SCARD_GetSmartCardStatus(wSCardId, &stSCardStatus);  /* get the older smartcard status*/

	wOldSCardStatus = stSCardStatus.wStatus;
	wSCardStatus = stScStatusData.stStatus.wStatus;   /* the new smartcard status */

#ifndef _AUSTAR_

	/* jyou: 3.4.2 */
	/* Only perform actions when status changes */
	if ( wSCardStatus != wOldSCardStatus)
	{
		switch( wSCardStatus )
		{
		case STATUS_CARD_IN:
			CA_DBG(("SC status is OK.\n"));
			/* Send out smart card queries */
			CA_SCARD_SendNationalityQueryMsg(wSCardId);
			CA_SCARD_SendSmartCardDataQueryMsg(wSCardId);
			CA_SCARD_SendSmartCardHomingChQueryMsg(wSCardId);
			CA_SCARD_SendUserDataQueryMsg(wSCardId);

			CA_ERROR_Initialise(wMaxGrpNum);

			break;
		/* Fix SCR#26646: ayang 07/12/04 */
		case STATUS_CARD_VERIFYING:
			bSCellMsgFlag = 0;
			bSCellMsgFlagStatic = 0;
		case STATUS_CARD_OUT:
		case STATUS_CARD_UNKNOWN:
		case STATUS_CARD_ERROR:
		default:
			CA_DBG(("SC status is bad.\n"));
			/* clean up smart card local storage for wSCardId */
			CA_SCARD_Initialise(wSCardId);
			break;
		}
		/* jyou: 3.4.2 SCR#25328 merged from mil: 3.3.11*/
				CA_PRODUCT_CheckInitialise(wSCardId);
	#ifdef INCL_PRODUCT_BLOCKING
				CA_PRODUCT_BLOCK_Init(wSCardId) ;
	#endif
		CA_EXTENDED_PRODUCT_ListInitialise(wSCardId);
		CA_MONITOR_Initialise(wMaxGrpNum);

		CA_SCARD_SetSmartCardStatus(wSCardId, &stScStatusData.stStatus);
	}

#else

	if ( (wOldSCardStatus != wSCardStatus) ||(stSCardStatus.eSource==SOURCE_UNKNOWN) )
	{
		CA_DBG_V(("wOldSCardStatus=%d\n",wOldSCardStatus ));
		CA_DBG_V(("wSCardStatus=%d\n",wSCardStatus ));
		/*CA_Modi_SC_Status(wSCardId, wSCardStatus);*/
		SynScMessage = SynScMessage	+1 ;
		/*CA_SCARD_STATUSReplyMsg(wSCardId);*/
		CA_DBG(("SynScMessage =	%d \r\n",SynScMessage ));
	}

	/* Send	the	SC's status	to the CA Client */
	switch(	wSCardStatus )
	{
	case STATUS_CARD_IN:

		CA_DBG(("SC status is OK.\n"));
		if( wSCardStatus != wOldSCardStatus )
		{

			SynSendMessage = 1;
			CA_SCARD_SendNationalityQueryMsg(wSCardId);
			CA_SCARD_SendSmartCardDataQueryMsg(wSCardId);
			CA_SCARD_SendSmartCardHomingChQueryMsg(wSCardId);
			CA_SCARD_SendSmartCardUserDataQueryMsg(wSCardId);

			CA_IPPV_Initialise(wSCardId);
			CA_PPT_Initialise(wSCardId);
			CA_LPPV_Initialise(wSCardId);
			CA_HGPC_Initialise(wSCardId);
			CA_SURFLOCK_Initialise(wSCardId);
			CA_EXTENDED_PRODUCT_ListInitialise(wSCardId);
						CA_PRODUCT_CheckInitialise(wSCardId); /*7/22*/
	#ifdef INCL_PRODUCT_BLOCKING
						CA_PRODUCT_BLOCK_Init(wSCardId) ;
	#endif
			CA_MONITOR_Initialise(wMaxGrpNum);
			CA_SCARD_SendSmartCardUserDataQueryMsg(wSCardId);
			/*	CA_FlexiFlash_OwnerIDSendQuery( ); */

			/* SendComponentsVersionQueryMsg(); */   /* mli, lx proposed just query 1 time */
			CA_ERROR_Initialise(wMaxGrpNum);
			SynInOutMessage	= 1;

		}
		break;

	case STATUS_CARD_OUT:
	case STATUS_CARD_UNKNOWN:
	case STATUS_CARD_ERROR:
	case STATUS_CARD_VERIFYING:
	default:

		CA_DBG(("SC status is bad.\n"));
		/*fCatSend = IA_FALSE ;*/
		CA_SCARD_Initialise(wSCardId);
		CA_IPPV_Initialise(wSCardId);
		CA_LPPV_Initialise(wSCardId);
		CA_HGPC_Initialise(wSCardId);
		CA_PPT_Initialise(wSCardId);
		CA_SURFLOCK_Initialise(wSCardId);
		/*			CA_FlexiFlash_Initialise(wSmartCardId);	*/
				CA_PRODUCT_CheckInitialise(wSCardId);/*7/22*/
	#ifdef INCL_PRODUCT_BLOCKING
				CA_PRODUCT_BLOCK_Init(wSCardId) ;
	#endif
		CA_EXTENDED_PRODUCT_ListInitialise(wSCardId);
		CA_MONITOR_Initialise(wMaxGrpNum);
		CA_Modi_SC_Status(wSCardId,	wSCardStatus);

		SynInOutMessage	= 0;

		break;
	}
#endif
	IA_CloseMessage(pvMsg, IA_FALSE);
	return;
}


static void CloseActiveProgram(ia_word32 wService)
{
	/* Last service may be not initiated when receiving STOP_SRV. */
	if( pstService[wService].fEcmSrcAdded == IA_TRUE )
	{
		UpdateEcmSourceToDescrambleService(wService);   /* mli PVCS SCR:23759 */
	}
	ResetLocalServiceInfo(wService);
#ifndef NO_CLOSING_SERVICE_ON_ZAPPING /* jyou: 3.6.3 2008-02-03 */
	/* Added ServiceClose by angela to remove resources of descramble service. 07/11/01 */
	ServicesClose(wService,CA_SERVICE_DESCRAMBLE);
#endif
}

static void ResetLocalServiceInfo(ia_word32 wService)
{

	/* mli PVCS SCR:23759 */
	pstService[wService].fClear = IA_TRUE;
	/* because of update ECM resource, we'll send empty PMT, we don't know the status */
	/* mli PVCS SCR:23759 */

	/* free means the group's scell handle is free */

	if( pstService[wService].pbPmtInfo != 0 )
	{
		FREE_MEMORY(pstService[wService].pbPmtInfo);
		pstService[wService].pbPmtInfo = 0;
	}
}


static void AddResourceToPvrRecordService(ia_word32 wGroupHandle)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_crypto_operation_source_st  stPvrEncryptResource;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wScellHandle;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_RECORD;      /* it's rce service */
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}
	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_RECORD);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (PVR Record: +sm +cryptop *sc)\n"));
	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_define_data_st) + 3*sizeof(msg_resource_st)
		+ sizeof(msg_crypto_operation_source_st), sizeof(msg_resource_st));

	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = 3;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_SESSION_MANAGEMENT;
	stRsc.wResourceId   = CA_SVCINFO_GetRecIdFromGroupHandle(wGroupHandle);
	stRsc.wResourceMgnt = MANAGE_ADD;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	stRsc.wResourceType = RESOURCE_CRYPTO_OPERATION;
	stRsc.wResourceId   = wGroupHandle;	  /* mli ??? */
	stRsc.wResourceMgnt = MANAGE_ADD;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );
	stPvrEncryptResource.eAlgorithm = m_stCaInitCfg.eCryptoAlgorithm; /* VD_CRYPTO_ALGORITHM_3DES */
	IA_WriteToMessage( pvMsg, &stPvrEncryptResource, sizeof(stPvrEncryptResource));

	stRsc.wResourceType = RESOURCE_SMARTCARD;
	stRsc.wResourceId   = CA_SCARD_GetSmartCardId( 0 ); /* jyou: 3.4.2 relax ID hard code */
	stRsc.wResourceMgnt = MANAGE_UPDATE; /* jyou: 3.4.3.5 SCR#26041 */
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );


	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_RECORD,pvMsg);
}


static void AddResourceToPvrRecordServiceForDrm(ia_word32 wGroupHandle)
{
	ia_msg_handle	pvMsg;
	/* jyou: 3.4.2 multiple smart cards */
	ia_byte			bResourceCount;

	CA_DRM_PVR_RECORD_T stCaDrmPvrRec;

	msg_resource_st  stRsc;
	msg_service_define_data_st stSvcDefData;
	/* DRM payload */
	msg_sm_drm_define_st stDefineHeader;
	msg_sm_drm_define_record_st stDefineRecord;
	msg_crypto_operation_source_st   stPvrEncryptResource;

	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wScellHandle;
	CA_LOCAL_GROUP_SERVICE_INFO_T *pstPvrSvc = 0;

	if (0 == pstCaDrmPvrRecord)
	{
		CA_DBG(("Invalid Pointer.\n"));
		return;
	}
	stCaDrmPvrRec = pstCaDrmPvrRecord[wGroupHandle];

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_RECORD;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}
	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_RECORD);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}
	pstPvrSvc = CA_SVCINFO_GetSvcInfoPtr(wGroupHandle, CA_SERVICE_RECORD);
	if (0 == pstPvrSvc)
	{
		CA_DBG(("Can't find the service information.\n"));
		return;
	}

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (PVR Record: +smdrm +cryptop *msc )\n"));
	/* jyou: 3.4.2 multiple smart cards */
	bResourceCount = (ia_byte)(2 + CA_SCARD_ValidSmartCards());
	pvMsg = IA_OpenOutputMessage( (ia_word16)
		(sizeof(msg_service_define_data_st)
		+ sizeof(msg_sm_drm_define_st)
		+ sizeof(msg_sm_drm_define_record_st)
		+ bResourceCount*sizeof(msg_resource_st)
		+ sizeof(msg_crypto_operation_source_st)),
		sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = bResourceCount;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_SESSION_MANAGEMENT_DRM;
	stRsc.wResourceId   = CA_SVCINFO_GetRecIdFromGroupHandle(wGroupHandle); /* incremental session key record */
	stRsc.wResourceMgnt = MANAGE_UPDATE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	stDefineHeader.eSessionSubtype = MSG_SM_DRM_SESSION_SUBTYPE_PVR_PHASE_3;
	stDefineHeader.ePvrPurpose = ( msg_sm_drm_pvr_purpose_e )stCaDrmPvrRec.ePvrPurpose;
	stDefineHeader.ePvrAction = MSG_SM_DRM_PVR_ACTION_RECORD;
	IA_WriteToMessage( pvMsg, &stDefineHeader, sizeof(msg_sm_drm_define_st) );

	stDefineRecord.wDescrambleServiceHandle = stCaDrmPvrRec.wDescrambleServiceHandle;
	CA_DBG(("<3713>DescrambleServiceHandle=%d\r\n", stDefineRecord.wDescrambleServiceHandle));
	/*jyou: added two missing fields according to section 6.13.2.1 of 710475*/
	stDefineRecord.fFtaCheckAnySector = stCaDrmPvrRec.fFtaCheckAnySector;
	stDefineRecord.bFtaCheckSectorNr = stCaDrmPvrRec.bFtaCheckSectorNr;
	if (stCaDrmPvrRec.fChainRecordings == IA_FALSE)
	{
		stDefineRecord.fChainRecordings = IA_FALSE;
	}
	else
	{
		/* get the current chaining sequence record id */
		stDefineRecord.fChainRecordings = IA_TRUE;
		stDefineRecord.wChainingSessionResourceId = stCaDrmPvrRec.wChainingSessionResourceId;
		stDefineRecord.wChainingSequenceRecordId = pstPvrSvc->stPvrRecordServiceInfo.wSequenceRecId;
		stDefineRecord.wChainingRightsRecordId = pstPvrSvc->stPvrRecordServiceInfo.wRightsRecId;
	}
	IA_WriteToMessage(pvMsg, &stDefineRecord, sizeof(msg_sm_drm_define_record_st));

	stRsc.wResourceType = RESOURCE_CRYPTO_OPERATION;
	stRsc.wResourceId   = wGroupHandle;	  /* mli unuse in virtual driver */
	stRsc.wResourceMgnt = MANAGE_UPDATE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	stPvrEncryptResource.eAlgorithm = m_stCaInitCfg.eCryptoAlgorithm; /* VD_CRYPTO_ALGORITHM_3DES */;
	IA_WriteToMessage( pvMsg, &stPvrEncryptResource, sizeof(stPvrEncryptResource));

	/* jyou: 3.4.2 multiple smart card */
	PrepareSmartCardResources( MANAGE_UPDATE, pvMsg ); /* jyou: 3.4.3.5 SCR#26041 */

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_RECORD,pvMsg);
	return;
}

static void RemoveResourceFromPvrRecordService(ia_word32 wGroupHandle)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wScellHandle;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_RECORD;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}
	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_RECORD);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}


	stSvcDefData.wServiceHandle = wScellHandle;
	/* jyou: 3.4.2 multiple smart cards */
#ifdef ECT_SUPPORT_PVR_CLIENT_PHASE12
	stSvcDefData.bResourceCount = 2 ; /* jyou: 3.4.3.5 SCR#26041 */
#endif
#ifdef ECT_SUPPORT_PVR_CLIENT_DRM
	stSvcDefData.bResourceCount = 2 ; /* jyou: 3.4.3.5 SCR#26041 */
#endif
	pvMsg = IA_OpenOutputMessage( (ia_word16)
		(sizeof(msg_service_define_data_st) +
		stSvcDefData.bResourceCount*sizeof(msg_resource_st) ),
		sizeof(msg_resource_st));
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(msg_service_define_data_st) );

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (PVR Record: -smdrm -cryptop )\n"));

#ifdef ECT_SUPPORT_PVR_CLIENT_PHASE12
	stRsc.wResourceType = RESOURCE_SESSION_MANAGEMENT;
#endif
#ifdef ECT_SUPPORT_PVR_CLIENT_DRM
	stRsc.wResourceType = RESOURCE_SESSION_MANAGEMENT_DRM;
#endif
	stRsc.wResourceId   = CA_SVCINFO_GetRecIdFromGroupHandle(wGroupHandle);
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	stRsc.wResourceType = RESOURCE_CRYPTO_OPERATION;
	stRsc.wResourceId   = wGroupHandle;
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	/* jyou: 2008-02-05 Should not close the smart card resource when stopping services */

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_RECORD,pvMsg);
	return;
}

static void AddResourceToPvrPlayBackService(ia_word32 wGroupHandle)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	msg_crypto_operation_source_st  stPvrEncryptResource;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	ia_word16 wScellHandle;

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_PLAYBACK;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}
	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_PLAYBACK);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (PVR PlayBack: +sc)\n"));

	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_define_data_st) + 3*sizeof(msg_resource_st)
		+ sizeof(msg_crypto_operation_source_st), sizeof(msg_resource_st));

	stSvcDefData.wServiceHandle = wScellHandle;
	stSvcDefData.bResourceCount = 3;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof(msg_service_define_data_st) );

	stRsc.wResourceType = RESOURCE_SESSION_MANAGEMENT;
	stRsc.wResourceId   = CA_SVCINFO_GetPlaybackIdFromGroupHandle(wGroupHandle);
	stRsc.wResourceMgnt = MANAGE_ADD;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	stRsc.wResourceType = RESOURCE_CRYPTO_OPERATION;
	stRsc.wResourceId   = wGroupHandle;  /**???? mli */
	stRsc.wResourceMgnt = MANAGE_ADD;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );
	stPvrEncryptResource.eAlgorithm = m_stCaInitCfg.eCryptoAlgorithm; /* VD_CRYPTO_ALGORITHM_3DES */;
	IA_WriteToMessage( pvMsg, &stPvrEncryptResource, sizeof(msg_crypto_operation_source_st));

	stRsc.wResourceType = RESOURCE_SMARTCARD;
	stRsc.wResourceId   = CA_SCARD_GetSmartCardId( 0 ); /* jyou: 3.4.2 relax ID hard code */
	stRsc.wResourceMgnt = MANAGE_UPDATE;	/* jyou: 3.4.3.5 SCR#26041 */
	IA_WriteToMessage( pvMsg, &stRsc, sizeof(msg_resource_st) );

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_PLAYBACK,pvMsg);
}

static void AddResourceToPvrPlayBackServiceForDrm(ia_word32 wGroupHandle)
{
	ia_msg_handle pvMsg;
	CA_DRM_PVR_PLAYBACK_T stCaDrmPvrPlayback;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	/* DRM payload */
	msg_sm_drm_define_st stDefineHeader;
	msg_sm_drm_define_playback_st stDefinePlayback;
	msg_crypto_operation_source_st  stPvrEncryptResource;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	/* ayang 2008-04-09 Changed type of wCopyControlRscID from ia_word16 to ia_word32 */
	ia_word16				wScellHandle;
	ia_word32 				wCopyControlRscID=INVALID_RESOURCE_ID;

	if (0 == pstCaDrmPvrPlayback)
	{
		CA_DBG(("Invalid Pointer.\n"));
		return;
	}
	ansi_memcpy((void*)&stCaDrmPvrPlayback, (void *)&pstCaDrmPvrPlayback[wGroupHandle], sizeof(stCaDrmPvrPlayback));

	stCaSvcGrpDefHeader.wServiceType = CA_SERVICE_PLAYBACK;
	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}
	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_PLAYBACK);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}

	stSvcDefData.wServiceHandle = wScellHandle;
#ifdef CA_SUPPORT_COPYCONTROL	/* jyou: 3.6.1 SCR#26708 2007-12-26 */
	wCopyControlRscID = CA_SVCINFO_GetResourceID( (ia_word16)wGroupHandle,
		CA_SERVICE_PLAYBACK, RESOURCE_COPY_CONTROL );
#endif
	if ( INVALID_RESOURCE_ID != wCopyControlRscID )
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (PVR PlayBack: +smdrm +cryptop +msc +cc)\n"));
		stSvcDefData.bResourceCount = 3 + CA_SCARD_ValidSmartCards();
	}
	else
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (PVR PlayBack: +smdrm +cryptop +msc)\n"));
		stSvcDefData.bResourceCount = 2 + CA_SCARD_ValidSmartCards();
	}

	pvMsg = IA_OpenOutputMessage( (ia_word16)
		(sizeof(msg_service_define_data_st) +
		stSvcDefData.bResourceCount*sizeof(msg_resource_st) +
		sizeof(msg_sm_drm_define_st) +
		sizeof(msg_sm_drm_define_playback_st) +
		sizeof(msg_crypto_operation_source_st) ),
		sizeof(msg_resource_st));
	IA_WriteToMessage(pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ));

	stRsc.wResourceType = RESOURCE_SESSION_MANAGEMENT_DRM;
	stRsc.wResourceId   = CA_SVCINFO_GetPlaybackIdFromGroupHandle(wGroupHandle);
	stRsc.wResourceMgnt = MANAGE_UPDATE;
	IA_WriteToMessage(pvMsg, &stRsc, sizeof( msg_resource_st ));

	stDefineHeader.eSessionSubtype = MSG_SM_DRM_SESSION_SUBTYPE_PVR_PHASE_3;
	stDefineHeader.ePvrPurpose = ( msg_sm_drm_pvr_purpose_e )stCaDrmPvrPlayback.ePvrPurpose;
	stDefineHeader.ePvrAction = MSG_SM_DRM_PVR_ACTION_PLAYBACK;
	IA_WriteToMessage(pvMsg, &stDefineHeader, sizeof(stDefineHeader));

	stDefinePlayback.wRightsRecordId = stCaDrmPvrPlayback.wRightsRecordId;
	stDefinePlayback.wSequenceRecordId = stCaDrmPvrPlayback.wSequenceRecordId;
	stDefinePlayback.fPeekRightsOnly = stCaDrmPvrPlayback.fPeekRightsOnly;
	stDefinePlayback.ePlaybackDirection = (msg_sm_drm_playback_direction_e)stCaDrmPvrPlayback.ePlaybackDirection;
	IA_WriteToMessage(pvMsg, &stDefinePlayback, sizeof(stDefinePlayback));

	stRsc.wResourceType = RESOURCE_CRYPTO_OPERATION;
	stRsc.wResourceId   = wGroupHandle;
	stRsc.wResourceMgnt = MANAGE_UPDATE;
	IA_WriteToMessage(pvMsg, &stRsc, sizeof( msg_resource_st ));
	stPvrEncryptResource.eAlgorithm = m_stCaInitCfg.eCryptoAlgorithm; /* VD_CRYPTO_ALGORITHM_3DES */;
	IA_WriteToMessage(pvMsg, &stPvrEncryptResource, sizeof(stPvrEncryptResource));

	/* jyou: 3.4.2 multiple smart card */
	PrepareSmartCardResources( MANAGE_UPDATE, pvMsg ); /* jyou: 3.4.3.5 SCR#26041 */

	/* jyou: 3.6.1 SCR#26708 2007-12-26 */
	if ( INVALID_RESOURCE_ID != wCopyControlRscID )
	{
		stRsc.wResourceType = RESOURCE_COPY_CONTROL;
		stRsc.wResourceId   = wCopyControlRscID;
		stRsc.wResourceMgnt = MANAGE_ADD; /*jyou*/
		CA_DBG_V(("[ECT][AddResourceToPvrPlayBackServiceForDrm] Copy Control Resource ID=%d\n",
			wCopyControlRscID));
		IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );
	}

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_PLAYBACK,pvMsg);
}


static void RemoveResourceFromPvrPlayBackService(ia_word32 wGroupHandle)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	CA_SERVICE_HEADER stCaSvcGrpDefHeader;
	/* ayang 2008-04-09 Changed type of wCopyControlRscID from ia_word16 to ia_word32 */
	ia_word16				wScellHandle;
	ia_word32 				wCopyControlRscID=INVALID_RESOURCE_ID;

	if (CA_SVCINFO_GetSvcHeaderInfo(wGroupHandle, &stCaSvcGrpDefHeader) == IA_FAIL)
	{
		CA_DBG(("Can't find the service header.\n"));
		return;
	}
	wScellHandle = CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_PLAYBACK);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}


	stSvcDefData.wServiceHandle = wScellHandle;
#ifdef ECT_SUPPORT_PVR_CLIENT_PHASE12
	stSvcDefData.bResourceCount = 2 ; /*jyou: 3.4.3.5 SCR#26041 */
#endif

/* jyou: 3.6.1 SCR#26708 2007-12-26 */
#if ((defined ECT_SUPPORT_PVR_CLIENT_DRM)&(defined CA_SUPPORT_COPYCONTROL))
	wCopyControlRscID = CA_SVCINFO_GetResourceID( (ia_word16)wGroupHandle,
		CA_SERVICE_PLAYBACK, RESOURCE_COPY_CONTROL );
#endif
	if ( INVALID_RESOURCE_ID != wCopyControlRscID )
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (PVR PlayBack: -smdrm -cryptop -cc)\n"));
		stSvcDefData.bResourceCount = 3 ;  /*jyou: 3.4.3.5 SCR#26041 */
	}
	else
	{
		CA_DBG_V(("<= MSG_SERVICE_DEFINE (PVR PlayBack: -smdrm -cryptop )\n"));
		stSvcDefData.bResourceCount = 2 ; /* jyou: 3.4.3.5 SCR#26041 */
	}
	pvMsg = IA_OpenOutputMessage( (ia_word16)
		(sizeof(msg_service_define_data_st) +
		stSvcDefData.bResourceCount*sizeof(msg_resource_st)),
		sizeof(msg_resource_st));
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

#ifdef ECT_SUPPORT_PVR_CLIENT_PHASE12
	stRsc.wResourceType = RESOURCE_SESSION_MANAGEMENT;
#endif
#ifdef ECT_SUPPORT_PVR_CLIENT_DRM
	stRsc.wResourceType = RESOURCE_SESSION_MANAGEMENT_DRM;
#endif
	/*jyou May 19,06: 3_3_8 bug fixed. PVR Phase 1/2 playback problem*/
	stRsc.wResourceId   = CA_SVCINFO_GetPlaybackIdFromGroupHandle(wGroupHandle);
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* jyou: 3.6.1 SCR#26708 2007-12-26 */
	if ( INVALID_RESOURCE_ID != wCopyControlRscID )
	{
		stRsc.wResourceType = RESOURCE_COPY_CONTROL;
		stRsc.wResourceId   = wCopyControlRscID;
		stRsc.wResourceMgnt = MANAGE_CLOSE;
		CA_DBG_V(("[ECT][RemoveResourceToPvrPlayBackServiceForDrm] Copy Control Resource ID=%d\n",
			wCopyControlRscID));
		IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );
	}

	stRsc.wResourceType = RESOURCE_CRYPTO_OPERATION;
	stRsc.wResourceId   = wGroupHandle;    /*mli: the resourceId is currently not used for this virtual driver */
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* jyou: 2008-02-05 Should not close the smart card resource when stopping services */

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wGroupHandle,CA_SERVICE_PLAYBACK,pvMsg);
}

ia_bool CA_TASK_IsClearProgram( ia_byte *pbPMTData  )
{
	ia_byte				bGlobalCount = 0;
	ia_byte				bStreamCount = 0;
	ia_byte				bDescrCount = 0;
	ia_byte				*pbPMT = 0;
	ia_word16			wOffset = 0;
	ia_word16			newCAPID = 0;
	int					i;


	pbPMT = (ia_byte*)(&pbPMTData[16]);   /* jyou: added emm_handle. start from glbdescrcount */
	bGlobalCount = pbPMT[0];

	/* there is a global CA descriptor */
	if( bGlobalCount > 0 )
	{
		/* ayang: fix suflock bug SCR#26455 */
		newCAPID = (ia_word16)((( pbPMT[5] & 0x1F ) << 8) + pbPMT[6] );

		CA_DBG_V(("In surflock:: oldPID=0x%04x, newPID=0x%04x\n", oldCAPID, newCAPID ));

		if( oldCAPID == newCAPID )
		{
			return IA_TRUE;
		}
		else
		{
			return IA_FALSE;
		}
	}

	wOffset = 1;

	/* There is no CA descriptor in the global, check whether CA descriptor in the stream descriptors */
	bStreamCount = pbPMT[ wOffset ];

	if( bStreamCount > MAX_SOFTCELL_STREAMS )
	{
		bStreamCount = MAX_SOFTCELL_STREAMS;
	}

	for( i = 0; i < bStreamCount; i++ )
	{
		/* ayang: fix suflock bug SCR#26455 - modify wOffset += 4 to wOffset += 3 */
		wOffset += 3;
		bDescrCount = pbPMT[ wOffset ];

		if( bDescrCount > 0 )
		{

			newCAPID = (ia_word16)((( pbPMT[ wOffset + 5 ] & 0x1F ) << 8) + pbPMT[ wOffset + 6 ] );

			CA_DBG_V(("In surflock:: oldPID=0x%04x, newPID=0x%04x\n", oldCAPID, newCAPID ));

			if( oldCAPID == newCAPID )
			{
				return IA_TRUE;
			}
			else
			{
				return IA_FALSE;
			}
		}
	}

	return IA_TRUE;
}

/******************************************************************************/
/*                   API CALLS PROVIDED BY THE CA_TASK DRIVER                 */
/******************************************************************************/

/*
* Description: SoftCell will use this call to send a message to the CA Task. The
*              CA Task must not process the data from this function, but queue
*              the data and process the data in context of it's own task. The CA
*              Task must free the pointer pvMessage if it is not 0.
*
* Parameters : eOpcode   - Opcode defining the message.
*              wOpSeqNo  - The sequence number identifying multiple messages with the same eOpcode.
*              wLength   - Length of the message.
*              pvMessage - Message data.
*
* Returns    : IA_SUCCESS - Success.
*              IA_FULL    - If no room is left to store the message.
*/
ia_result CA_DRV_Message( ca_drv_message_type_e eOpcode, ia_word16 wOpSeqNo, ia_word16 wLength, void *pvMessage )
{
	CA_SCELL_MESSAGE_STRUCT *pstMsg;
	ia_result enRet;

	pstMsg = (CA_SCELL_MESSAGE_STRUCT*)ALLOCATE_MEMORY( CA_SCELL_MESSAGE_STRUCT, 1 );
	if( pstMsg != 0 )
	{
		pstMsg->enMsgType              = CADRV__SCELL_MESSAGE;
		pstMsg->u.stSCellMsg.eOpcode   = eOpcode;
		pstMsg->u.stSCellMsg.wOpSeqNo  = wOpSeqNo;
		pstMsg->u.stSCellMsg.wLength   = wLength;
		pstMsg->u.stSCellMsg.pvMessage = pvMessage;

		/* The size	of each	message	is sizeof(void*) + sizeof(ia_word16). */
		/* jyou: 3.4.3.6 SCR#26043 2007-06-01 */
		OS_DRV_WaitSemaphore( gMessageQueueSemap );
		enRet =	OS_DRV_SendMessage(caMessageQueue, sizeof(CA_SCELL_MESSAGE_STRUCT),	pstMsg);
		OS_DRV_SignalSemaphore( gMessageQueueSemap );
		if(	enRet != IA_SUCCESS	)
		{
			CA_ERROR(("Can't post SC3 msg to CA	Task.\n"));
			if ( pstMsg	!= 0 )
			{
				FREE_MEMORY(pstMsg);
				pstMsg = 0;
			}

		}
	}
	else
	{
		CA_ERROR(("Failed to allocate memory for SC3 msg.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		enRet = IA_FAIL;
	}
	return enRet;
}

/*
* Description: SoftCell will use this function to report a fatal error. Typically
*              this call may occur during development and debugging, but should
*              never occur in applications in the field.
*
* Parameters : wError - The type of error that caused the fatal condition.
*
* Returns    : None.
*/
void CA_DRV_FatalError( ia_word16 wError )
{
	CA_ERROR(("A FATAL ERROR %d!!!\n", wError));
	/* 2008-5-9 AYang: Added Error dealing to notify application of errors */
	CA_ERROR_Notify(CA_ERROR_SOURCE_SOFTCELL, wError);
}


ia_bool CA_TASK_Close( void )
{
	CA_DBG_V(("-------CA_TASK_Close\n"));

	fReadyCloseECT = IA_TRUE;

	DeleteAllResource( );

	OS_DRV_DelayTask( 100 );

	return IA_TRUE;
}

static void CA_TASK_FreeLocalAllocatedMemory( void )
{
	ia_word16   wMaxNumGrps = 0, i;
	wMaxNumGrps = CA_SVCINFO_GetSvcMaxNumber();
	for (i = 0; i < wMaxNumGrps; i++)
	{
		if (0 != pstDMX)
		{
			if( pstDMX[i].pbCatInfo != 0 )
			{
				FREE_MEMORY( pstDMX[i].pbCatInfo );
				pstDMX[i].pbCatInfo = 0;
			}
			if( pstDMX[i].pbNitInfo != 0 )
			{
				FREE_MEMORY( pstDMX[i].pbNitInfo );
				pstDMX[i].pbNitInfo = 0;
			}
		}

		if (0 != pstService)
		{
			if( pstService[i].pbPmtInfo != 0 )
			{
				FREE_MEMORY(pstService[i].pbPmtInfo);
				pstService[i].pbPmtInfo = 0;
			}
		}
	}

	if (0 != pstService)
	{
		FREE_MEMORY(pstService);
		pstService = 0;
	}
	if (0 != pstDMX)
	{
		FREE_MEMORY(pstDMX);
		pstDMX = 0;
	}
	if (0 != pstTsNit)
	{
		FREE_MEMORY(pstTsNit);
		pstTsNit = 0;
	}
	if (0 != pstCaDrmPvrRecord)
	{
		FREE_MEMORY(pstCaDrmPvrRecord);
		pstCaDrmPvrRecord = 0;
	}
	if (0 != pstCaDrmPvrPlayback)
	{
		FREE_MEMORY(pstCaDrmPvrPlayback);
		pstCaDrmPvrPlayback = 0;
	}
}


static void CA_TASK_FreeAllocatedMemory( void )
{
	ia_byte j;
	ia_word32	wSCardId = INVALID_SC_SMARTCARDID;
	/*	ia_word16   wMaxNumGrps = 0, i; */

	if( stComponents.pstComponents != 0 )
	{
		FREE_MEMORY( stComponents.pstComponents );
		stComponents.pstComponents = 0;
	}

	CA_TASK_FreeLocalAllocatedMemory();

	for (j = 0; j < SC_SLOT_MAX; j++)
	{
		wSCardId = CA_SCARD_GetSmartCardId(j);
		if (wSCardId != INVALID_SC_SMARTCARDID)
		{
			CA_EXTENDED_PRODUCT_ListFreeMemory(wSCardId);
			/* mli: 3.3.11 */
						CA_PRODUCT_CHECK_ListFreeMemory(wSCardId);
			/*before free setors list, free PPT meemory first*/
		}
	}
	CA_IRD_MemoryFree();
	CA_PPT_Freememory();
	CA_IPPV_MemoryFree();
	CA_LPPV_MemoryFree();
	CA_HGPC_MemoryFree();
	CA_SCARD_FreeMemory();
	CA_ERROR_MemoryFree();
	CA_MONITOR_MemoryFree();
	CA_SVCINFO_MemoryFree();

}

#if 0
/*
*     ROUTINE : RemoveDefaultResourcesFromEmmService()
* DESCRIPTION : Disabled in 3.4.3.5 since only the smart card resource is to remove.
*					But we do not remove smart card resource due to softcell3 constraint
*       INPUT : wOpSeqNo - The emm_index+smartcard_index passed by CA Client.
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT :
*   CALLED BY :
*      Author : Jane You
* Last Changed: September 20, 2006
*/
static void RemoveDefaultResourcesFromEmmService( ia_word16 wOpSeqNo )
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	ia_word16 wSCellSrvHandle;

	wSCellSrvHandle = CA_SVCINFO_GetSCellHandle( wOpSeqNo, CA_SERVICE_EMM );
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellSrvHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: -sc)\n"));

	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_define_data_st) + sizeof(msg_resource_st),
		sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wSCellSrvHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_SMARTCARD;
	/* jyou: 3.4.2 multiple smart card */
	/* the returned service cannot be 0 since otherwise it will return invalid SCellHandle */
	stRsc.wResourceId   = ( CA_SVCINFO_GetEMMServiceByOpSeqNo(wOpSeqNo) )->wSmartCardId;
	stRsc.wResourceMgnt = MANAGE_UPDATE; /* jyou: 3.4.3.5 SCR#26041 */

	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	if( SCELL_Message( MSG_SERVICE_DEFINE, (ia_word16)wOpSeqNo, IA_GetOutputMessageLength(pvMsg),
		IA_GetOutputMessage(pvMsg) ) == IA_SUCCESS )
	{
		/* The buffer will be freed by the receiver. */
		IA_CloseMessage( pvMsg, IA_FALSE );
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
}
#endif

/*
*     ROUTINE : DeleteAllResource()
* DESCRIPTION : Close all opened services.
*       INPUT : None
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : None
*   CALLED BY : CA_TASK_Close
*      Author : Jane You
* Last Changed: September 20, 2006
*/
static void DeleteAllResource( void )
{
	ia_word16 wMaxNumGrps = 0, i, j;
	ia_word16 wDescScellHandle, wRecScellHandle, wPlayScellHandle;
	CA_LOCAL_GROUP_SERVICE_INFO_T *ptmpGrpSrvInfo = 0;

	/* jyou: 3.4.2 multiple smart cards EMM */
	for ( i = 0 ; i < MAX_DEMUX_COUNTS ; i++ )
	{
		/* Delete all unused resources */
		if ( IA_FALSE == CA_SVCINFO_GetEmmSvcStatus(i) )  /* judge the emm service is open yet not free */
		{
			/* jyou: ??? index of the monitor control */
			for ( j = 0 ; j < SC_SLOT_MAX ; j++ )
			{
				/* use the demux_index+smartcard_index as the index for a SoftCell service */
				/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
				CA_MONITOR_MonitorControl( CA_LOCAL_MakeWord16( i, j), CA_SERVICE_EMM, IA_FALSE );
				/* jyou: 3.4.3.7 SCR#25552 2007-06-06 end*/
				ServicesClose( CA_LOCAL_MakeWord16( i, j), CA_SERVICE_EMM);  /* release the scell handle*/
			}
			CA_SVCINFO_SetSvcToFree(i, CA_SERVICE_EMM);
			/* free the allocated memory of the given demux index i */
			/* CA_SVCINFO_ResetLocalSvcInfo(i); */ /* ayang: delete this line for bug fix. 07/09/29 */
		}
	}

	wMaxNumGrps = CA_SVCINFO_GetSvcMaxNumber();
	for (i = 0; i < wMaxNumGrps; i++)
	{
		if (IA_FALSE == CA_SVCINFO_GetGrpSvcStatus(i))
		{
			wDescScellHandle = CA_SVCINFO_GetSCellHandle(i, CA_SERVICE_DESCRAMBLE);
			wRecScellHandle = CA_SVCINFO_GetSCellHandle(i, CA_SERVICE_RECORD);
			wPlayScellHandle = CA_SVCINFO_GetSCellHandle(i, CA_SERVICE_PLAYBACK);

			if (INVALID_SCELL_SERVICE_HANDLE != wDescScellHandle)
			{
				/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
				CA_MONITOR_MonitorControl(i, CA_SERVICE_DESCRAMBLE, IA_FALSE);  /* disable ECM monitor */
				/* jyou: 3.4.3.7 SCR#25552 2007-06-06 end*/
				ptmpGrpSrvInfo = (CA_LOCAL_GROUP_SERVICE_INFO_T*)CA_SVCINFO_GetSvcInfoPtr(i, CA_SERVICE_DESCRAMBLE);
				if (ptmpGrpSrvInfo == 0)
					continue;
				if (ptmpGrpSrvInfo->astServices[CA_SERVICE_DESCRAMBLE].bRscCount != 0)
				{
					RemoveEcmSourceFromDescrambleService(i);
					CloseDefaultResourcesToDescrambleService(i);
				}
#if 0 /* ayang: removed 07/10/18 */
				while (ptmpGrpSrvInfo->astServices[CA_SERVICE_DESCRAMBLE].bRscCount != 0)
				{
					OS_DRV_DelayTask(10);  /* waiting for the resources moved */
				}
#endif
				ServicesClose(i, CA_SERVICE_DESCRAMBLE);  /* release the scell handle*/
				/* CA_SVCINFO_SetSvcToFree(i, CA_SERVICE_DESCRAMBLE); */

			}
			if (INVALID_SCELL_SERVICE_HANDLE != wRecScellHandle)
			{
				ServicesClose(i, CA_SERVICE_RECORD);  /* release the scell handle*/
				/*  CA_SVCINFO_SetSvcToFree(i, CA_SERVICE_RECORD);	*/
			}
			if (INVALID_SCELL_SERVICE_HANDLE != wPlayScellHandle)
			{
				ServicesClose(i, CA_SERVICE_PLAYBACK);  /* release the scell handle*/
				/*  CA_SVCINFO_SetSvcToFree(i, CA_SERVICE_PLAYBACK);	 */
			}
		}

		CA_SVCINFO_ResetLocalSvcInfo(i);
		ResetLocalServiceInfo(i);
	}

	m_wNotifyEnable = NOTIFY_MASK_DISABLE_ALL;
}

static void ServicesClose(ia_word32 wIndex, ia_word32 wSrvType)
{
	ia_msg_handle             *pvMsg;
	msg_service_close_data_st stSvcClose;
	ia_word16 wScellHandle;

	wScellHandle = CA_SVCINFO_GetSCellHandle(wIndex, wSrvType);
	if (INVALID_SCELL_SERVICE_HANDLE == wScellHandle)
	{
		return;
	}
	stSvcClose.wServiceHandle = wScellHandle;

	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_close_data_st) ,
		sizeof(msg_service_close_data_st) );

	CA_DBG_V((" cell handle = %d\n", stSvcClose.wServiceHandle ));

	IA_WriteToMessage( pvMsg, &stSvcClose, sizeof( msg_service_close_data_st ) );

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	if (ServiceHandleMechanism(MSG_SERVICE_CLOSE,wIndex,wSrvType,pvMsg) == IA_TRUE)
	{
		if (wSrvType == CA_SERVICE_DESCRAMBLE)   /* reset the status of descramble service */
		{
			pstService[wIndex].fEcmSrcAdded = IA_FALSE;
			pstService[wIndex].fDesServiceReady = IA_FALSE;
			gwLastGrpHandle[wIndex] = 0xff;
			//DBG_DRV_Printf("<4794>CloseDesc\r\n");
		}
	}
}

/* for OOB */
void AddOOBResourceToEmm(ia_word32 wOpSeqNo)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	ia_word16 wSCellSrvHandle = INVALID_SCELL_SERVICE_HANDLE, wDemuxIndex;
	ia_word32 wOobId = INVALID_GROUP_SERVICE_HANDLE;
	CA_SERVICE_HEADER stCaSvcHeader;

	CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: +oob)\n"));
	wSCellSrvHandle = CA_SVCINFO_GetSCellHandle( wOpSeqNo, CA_SERVICE_EMM);
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellSrvHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}
	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_DBG(("AddEMMSource: Invalid demux index.\n"));
		return;
	}

	stCaSvcHeader.wServiceType = CA_SERVICE_EMM;
	CA_SVCINFO_GetSvcHeaderInfo( wDemuxIndex, &stCaSvcHeader);
	wOobId = stCaSvcHeader.wOOBId;
	if (INVALID_GROUP_SERVICE_HANDLE == wOobId)
		return;

	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_define_data_st) + sizeof(msg_resource_st),
		sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wSCellSrvHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage( pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ) );

	stRsc.wResourceType = RESOURCE_DVB_OOB;
	stRsc.wResourceId   = wOobId;
	stRsc.wResourceMgnt = MANAGE_ADD;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wOpSeqNo,CA_SERVICE_EMM,pvMsg);
	return ;
}

void RemoveOOBResourceFromEmm(ia_word32 wOpSeqNo)
{
	ia_msg_handle pvMsg;
	msg_service_define_data_st stSvcDefData;
	msg_resource_st stRsc;
	ia_word16 wSCellSrvHandle, wDemuxIndex;
	ia_word32 wOobId = INVALID_GROUP_SERVICE_HANDLE;
	CA_SERVICE_HEADER stCaSvcHeader;


	CA_DBG_V(("<= MSG_SERVICE_DEFINE (EMM: -oob)\n"));
	wSCellSrvHandle = CA_SVCINFO_GetSCellHandle(wOpSeqNo, CA_SERVICE_EMM);
	if (INVALID_SCELL_SERVICE_HANDLE == wSCellSrvHandle)
	{
		CA_DBG(("Can't find the scell service handle.\n"));
		return;
	}
	wDemuxIndex = CA_LOCAL_GetUpperBytes( wOpSeqNo, 1 );
	if ( wDemuxIndex == INVALID_INDEX )
	{
		CA_DBG(("AddEMMSource: Invalid demux index.\n"));
		return;
	}

	stCaSvcHeader.wServiceType = CA_SERVICE_EMM;
	CA_SVCINFO_GetSvcHeaderInfo(wDemuxIndex, &stCaSvcHeader);
	wOobId = stCaSvcHeader.wOOBId;
	if (INVALID_GROUP_SERVICE_HANDLE == wOobId)
		return;

	pvMsg = IA_OpenOutputMessage( sizeof(msg_service_define_data_st) + sizeof(msg_resource_st),
		sizeof(msg_resource_st) );

	stSvcDefData.wServiceHandle = wSCellSrvHandle;
	stSvcDefData.bResourceCount = 1;
	IA_WriteToMessage(pvMsg, &stSvcDefData, sizeof( msg_service_define_data_st ));

	stRsc.wResourceType = RESOURCE_DVB_OOB;
	stRsc.wResourceId   = wOobId;
	stRsc.wResourceMgnt = MANAGE_CLOSE;
	IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );

	/* ayang 2008-7-29 Fix SCR#27360,27365 */
	ServiceHandleMechanism(MSG_SERVICE_DEFINE,wOpSeqNo,CA_SERVICE_EMM,pvMsg);
}


CA_INIT_CONFIG_ST* CA_TASK_GetInitCfg( void )
{
	return &m_stCaInitCfg ;
}

/* jyou: 3.4.2 multiple smart cards */
/*
*     ROUTINE : PrepareSmartCardResources()
* DESCRIPTION : Compose the resource structure for all smart cards.
*       INPUT : wResourceMgnt - {MANAGE_ADD, MANAGE_UPDTE, MANAGE_CLOSE}
*				pvMsg - pointer to the output message buffer
*      OUTPUT : pvMsg - with smart card reource definiton attached
*     RETURNS : None
* SIDE EFFECT : None
*   CALLED BY : UpdateDefaultResourcesToDescrambleService
*      Author : Jane You
* Last Changed: September 12, 2006
*/
void PrepareSmartCardResources( ia_word16 wResourceMgnt, ia_msg_handle pvMsg )
{
	ia_word32		wResourceID;
	ia_byte			i;
	msg_resource_st stRsc;
	ia_bool			fSurfLocked = IA_TRUE;

	for ( i = 0 ; i < SC_SLOT_MAX ; i++)
	{
		wResourceID = CA_SCARD_GetSmartCardId( i );
		CA_TASK_GetSurflockStatus( wResourceID, &fSurfLocked );
		/* Block invalid or surf locked smart cards */
		if ( INVALID_SMARTCARDID == wResourceID
			|| IA_TRUE == fSurfLocked )
		{
			continue;
		}
		stRsc.wResourceType = RESOURCE_SMARTCARD;
		stRsc.wResourceId   = wResourceID;
		stRsc.wResourceMgnt = wResourceMgnt;
		IA_WriteToMessage( pvMsg, &stRsc, sizeof( msg_resource_st ) );
	}

	return;
}

/*
*     ROUTINE : DefineEMMServicesForDemux()
* DESCRIPTION : Define EMM services for a transponder
*       INPUT : pbMsg -- the EMM Service Define message passed by a CA Client. Doc#731710
*					emm_handle -	The transponder(demux) id, and ECT expose this as the EMM handle
*									to the CA Client.
*					emm_resources - EMM resource mask (currently CAT and/or OOB)
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : For each smart card, send a EMM service open request or a service define to SoftCell.
*				Uses the global varible pstDMX.
*   CALLED BY : CaProcessMsg
*      Author : Jane You
* Last Changed: September 20, 2006
*/
static void DefineEMMServicesOnDemux( ia_byte *pbMsg, CA_MSG_T *pCaMsg )
{
	ia_word32			awServiceEMMHeader[CA_CLIENT_SERVICE_EMM_HEADER_SIZE/4];
	ia_word32			wEMMHandle, wResourcesMask;
	ia_word16			wMsgLen;
	ia_word16			wIndex; /* index of the given demux in astLocalDemuxInfo*/
	ia_word16			j, wOpSeqNo;
	CA_SERVICE_HEADER	stCaServiceHeader;

	CA_DBG(("=> CA_CLIENT__DEFINE_EMM_SERVICE\n"));
	/* EMM Service Define Message Length*/
	wMsgLen = (ia_word16)((ia_word16)(pbMsg[0]<<8) + (ia_word16)pbMsg[1]);
	/* Reset the header buffer */
	ansi_memset((void *)awServiceEMMHeader, 0, sizeof(ia_byte) * CA_CLIENT_SERVICE_EMM_HEADER_SIZE);
	/* Read out emm_handle and service_type from the input message */
	if ( GetMsgHeader(pbMsg+CA_CLIENT_SERVICE_MSG_MAX_LENGTH, awServiceEMMHeader,
		CA_CLIENT_SERVICE_EMM_HEADER_SIZE) == IA_SUCCESS )
	{
		wEMMHandle = awServiceEMMHeader[CA_CLIENT_EMM_SERVICE_HANDLE];
		wResourcesMask = awServiceEMMHeader[CA_CLIENT_EMM_SERVICE_SOURCE];
		CA_DBG_V(("=> emm_handle[%d] ResourceMask[0x%x]\n", wEMMHandle, wResourcesMask));
	}
	else
	{
		CA_ERROR(("[DEF-EMM] Can't get the CA_CLIENT__DEFINE_SERVICE_GROUP message header.\n"));
		return;
	}
	/* Check the availablity of the global status storage pstDMX */
	if ( 0 == pstDMX )
	{
		CA_ERROR(("[DEF-EMM] DMX container not available.\n"));
		return;
	}
	/* Add the EMM Handle to local storage */
	wIndex = CA_SVCINFO_AddDemux(wEMMHandle);
	if ( wIndex == INVALID_INDEX )
	{
		CA_ERROR(("[DEF-EMM] Failed to add a new EMM handle to local storage.\n"));
		return;
	}

	CA_DBG_V(("=> Index read is %d\n", wIndex));
	/* Set the EMM service required resource IDs on the given transponder--wEMMhandle */
	stCaServiceHeader.wServiceType = CA_SERVICE_EMM;
	stCaServiceHeader.wDemuxId = wEMMHandle;
	stCaServiceHeader.wOOBId = wEMMHandle;
	CA_SVCINFO_SetSvcHeaderInfo( wEMMHandle, &stCaServiceHeader );

	/* CLIENT_DEFINE_EMM_SOURCE_CAT */
	if ((wResourcesMask & CLIENT_DEFINE_EMM_SOURCE_CAT) != 0)
	{
		/* free previously allocated memory */
		if( pstDMX[wIndex].pbCatInfo != 0 )
		{
			FREE_MEMORY(pstDMX[wIndex].pbCatInfo);
			pstDMX[wIndex].pbCatInfo = 0;
		}
		/* allocate new memory */
		pstDMX[wIndex].pbCatInfo = (ia_byte*)ALLOCATE_MEMORY( ia_byte, (ia_uint32)(wMsgLen+CA_CLIENT_SERVICE_MSG_MAX_LENGTH));
		if (pstDMX[wIndex].pbCatInfo == 0)
		{
			CA_ERROR(("Failed to allocate memory.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
			return;
		}

		ansi_memcpy(pstDMX[wIndex].pbCatInfo, pCaMsg->pstParams, wMsgLen+CA_CLIENT_SERVICE_MSG_MAX_LENGTH);
		/* Proceed EMM Service define for each smart card */
		for ( j = 0 ; j < SC_SLOT_MAX ; j++)
		{
			/* skip invalid smart cards */
			if ( CA_SCARD_GetSmartCardId( (ia_byte) j ) == INVALID_SMARTCARDID )
			{
				continue;
			}
			wOpSeqNo = CA_LOCAL_MakeWord16(wIndex, j);
			/* Check if the EMM services are already defined for this wIndex */
			if ( INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wOpSeqNo, CA_SERVICE_EMM) )
			{
				CA_DBG_V(("=> OpSeqNo %x does not have EMM defined, so open a new one.\n", wOpSeqNo));
				SendServiceOpenRequestMsg( wOpSeqNo, SERVICE_DVB_EMM);
			}
			else
			{
				CA_DBG_V(("=> OpSeqNo %x alreay has EMM defined, so don't send open request.\n", wOpSeqNo));
				AddEmmSourceToEmmService( wOpSeqNo );
			}
		}
	} /* end CAT resource definition */

#ifdef INCL_SERVICE_DMB
	/*Addded by Andrew Hui 070510*/
	if ((wResourcesMask & CLIENT_DEFINE_DMB_EMM_SOURCE) != 0)
	{
		for ( j = 0 ; j < SC_SLOT_MAX ; j++)
		{
			/* skip invalid smart cards */
			if ( CA_SCARD_GetSmartCardId( (ia_byte) j ) == INVALID_SMARTCARDID )
			{
				continue;
			}
			wOpSeqNo = CA_LOCAL_MakeWord16(wIndex, j);
			/* Check if the EMM services are already defined for this wIndex */
			/* Set the EMM service required resource IDs on the given transponder--wEMMhandle */
			stCaServiceHeader.wServiceType = CA_SERVICE_DMB_EMM;
			stCaServiceHeader.wDemuxId = wEMMHandle;
			stCaServiceHeader.wOOBId = wEMMHandle;
			if ( INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wOpSeqNo, CA_SERVICE_DMB_EMM) )
			{
				CA_SVCINFO_SetSvcHeaderInfo( wEMMHandle, &stCaServiceHeader );

			}
			CA_SVCINFO_SetDMBInfo(wIndex,CA_SERVICE_DMB_EMM,pbMsg);
			if ( INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wOpSeqNo, CA_SERVICE_DMB_EMM) )
			{
				SendServiceOpenRequestMsg( wOpSeqNo, SERVICE_DMB_EMM);
			}
			else
			{
				CA_DMB_AddEmmResourceToEmmService(wOpSeqNo);
			}
		}
	}
	/*end added*/
#endif
	if ((wResourcesMask & CLIENT_DEFINE_EMM_SOURCE_OOB) != 0)
	{
		CA_DBG(("=> CA_CLIENT__STRAT_OOB\n"));

		if ( IA_TRUE == CA_SVCINFO_GetEmmSvcStatus( wIndex ) ) /* check the emm is open */
			return;
		else
			pstDMX[wIndex].fHasOOB  = IA_TRUE;
		/* now the emm is opened and the oob resource
		should added in the open reply function */
	}
	return;
}


/*
*     ROUTINE : StopEMMServicesOnDemux()
* DESCRIPTION : Define EMM services for a transponder
*       INPUT : pbMsg with the following structure
*					emm_handle -	The transponder(demux) id, and ECT expose this as the EMM handle
*									to the CA Client.
*					emm_resources - EMM resource mask (currently CAT and/or OOB)
*      OUTPUT : None
*     RETURNS : None
* SIDE EFFECT : For each smart card, send a EMM service open request or a service define to SoftCell.
*				Uses the global varible pstDMX.
*   CALLED BY : CaProcessMsg
*      Author : Jane You
* Last Changed: September 20, 2006
*/
static void StopEMMServicesOnDemux( ia_byte *pbMsg )
{
	ia_word32	awServiceStopEMMHeader[CA_CLIENT_SERVICE_EMM_STOP_HEADER_SIZE/4];
	ia_word32	wEMMHandle, wResourcesMask;
	ia_word16	wIndex; /* index of the given demux in astLocalDemuxInfo*/
	ia_word16	j, wOpSeqNo;

	ansi_memset((void *)awServiceStopEMMHeader, 0, sizeof(ia_byte) * CA_CLIENT_SERVICE_EMM_STOP_HEADER_SIZE);
	CA_DBG(("=> CA_CLIENT__STOP_EMM_SERVICE\n"));
	if (GetMsgHeader(pbMsg+CA_CLIENT_SERVICE_MSG_MAX_LENGTH, awServiceStopEMMHeader,
		CA_CLIENT_SERVICE_EMM_STOP_HEADER_SIZE) == IA_SUCCESS)
	{
		wEMMHandle = awServiceStopEMMHeader[CA_CLIENT_EMM_SERVICE_HANDLE];
		wResourcesMask = awServiceStopEMMHeader[CA_CLIENT_EMM_SERVICE_SOURCE];
	}
	else
	{
		CA_ERROR(("Can't get the CA_CLIENT__DEFINE_SERVICE_GROUP message header.\n"));
		return;
	}
	/* Locate the EMM Handle from the local storage */
	wIndex = CA_SVCINFO_FindDemux(wEMMHandle);
	if ( wIndex == INVALID_INDEX )
	{
		CA_ERROR(("[STOP-EMM] Failed to local an EMM handle from local storage.\n"));
		return;
	}

	if ( IA_TRUE == CA_SVCINFO_GetEmmSvcStatus(wIndex) )
	{
		return;   /* the service isn't opened */
	}

	/* Perform for every smart cards */
	for ( j = 0 ; j < SC_SLOT_MAX ; j++)
	{
		/* skip invalid smart cards */
		if ( CA_SCARD_GetSmartCardId( (ia_byte) j ) == INVALID_SMARTCARDID )
		{
			continue;
		}
		wOpSeqNo = CA_LOCAL_MakeWord16(wIndex, j);
		/* CLIENT_STOP_EMM_SOURCE_CAT */
		if ((wResourcesMask & CLIENT_DEFINE_EMM_SOURCE_CAT) != 0)
		{
			/* jyou: 3.4.3.5 SCR#26042 */
			UpdateEmmSourceToEmmService( wOpSeqNo );

			/* close the EMM service monitor for the EMM service */
			/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
			CA_MONITOR_MonitorControl(wOpSeqNo, CA_SERVICE_EMM, IA_FALSE);
			/* jyou: 3.4.3.7 SCR#25552 2007-06-06 end */
		}
		/* OOB */
		if ((wResourcesMask & CLIENT_DEFINE_EMM_SOURCE_OOB) != 0)
		{
			CA_DBG(("=> CA_CLIENT__STOP_OOB, EmmHandle : %d \n", wEMMHandle ));
			RemoveOOBResourceFromEmm(wOpSeqNo);
		}
#ifdef INCL_SERVICE_DMB
		if ((wResourcesMask & CLIENT_DEFINE_DMB_EMM_SOURCE) != 0)
		{
			CA_DBG(("=> CA_CLIENT__STOP_DMB EMM, EmmHandle : %d \n", wEMMHandle ));
			RemoveDefaultResourcesFromEmmService(wOpSeqNo);
			RemoveDMBEmmResFromService(wOpSeqNo);
			/* close the EMM service monitor for the EMM service */
			CA_MONITOR_MonitorControl(wOpSeqNo, ECT_FOR_DMB_CLIENT, IA_FALSE);
		}
		/* Reset the error status for the given EMM service */
		DMB_ServicesClose(wOpSeqNo, CA_SERVICE_DMB_EMM);
		CA_ERROR_Reset( wOpSeqNo, CA_SERVICE_DMB_EMM );
#endif
		/*CA_SVCINFO_ReleaseDemux(wEMMHandle);*/
	}
	if( pstDMX[wIndex].pbCatInfo != 0 )
	{
		FREE_MEMORY(pstDMX[wIndex].pbCatInfo);
		pstDMX[wIndex].pbCatInfo = 0;
	}
	if (pstDMX[wIndex].pbNitInfo != 0)
	{
		FREE_MEMORY(pstDMX[wIndex].pbNitInfo);
		pstDMX[wIndex].pbNitInfo = 0;
	}
	return;
}


static void DefineDescrambleService(ia_word32 wGroupHandle, ia_word16 wDescLen, ia_byte *pbMsg, CA_MSG_T *pCaMsg)
{
	ia_word32			wEMMHandle ;
	CA_SERVICE_HEADER	stCaServiceHeader;
	ia_byte		bIndex = 10;

	/* read the corresponding tranponder ID of the descramble service */
	wEMMHandle = ( ia_word16 ) ( pbMsg[bIndex++] << 24 );
	wEMMHandle += ( ia_word16 ) ( pbMsg[bIndex++] << 16 );
	wEMMHandle += ( ia_word16 ) ( pbMsg[bIndex++] << 8 );
	wEMMHandle += ( ia_word16 )( pbMsg[bIndex++] );

	stCaServiceHeader.wGroupHandle = wGroupHandle;
	/* jyou: 3.4.2 default is the first smart card */
	stCaServiceHeader.wSmartCardId = CA_SCARD_GetSmartCardId( 0 );
	/* jyou: 3.4.2 default is the EMM handle */
	stCaServiceHeader.wDemuxId = wEMMHandle;
	/* default: wGroupHandle, macrovisionId is unique */
	stCaServiceHeader.wMacrovisionId = wGroupHandle;
	stCaServiceHeader.wServiceType = CA_SERVICE_DESCRAMBLE; /* use CA_SERVICE_DESCRAMBLE as grp service index */

	/* Save the default resource IDs for descramble service */
	CA_SVCINFO_SetSvcHeaderInfo(wGroupHandle, &stCaServiceHeader);

	/* read out the service id (program number) */
	pstTsNit[wGroupHandle].ServiceID = ( ia_word16 )(( pbMsg[bIndex++] ) << 8 );
	pstTsNit[wGroupHandle].ServiceID += ( ia_word16 )( pbMsg[bIndex++] );
	CA_DBG(("=> [wGroupHandle]: %d serviceID = %2x\n", wGroupHandle, pstTsNit[wGroupHandle].ServiceID));


	/* save PMT info */
	if( pstService[wGroupHandle].pbPmtInfo!= 0 )
	{
		FREE_MEMORY( pstService[wGroupHandle].pbPmtInfo );
		pstService[wGroupHandle].pbPmtInfo = 0;
	}

	pstService[wGroupHandle].pbPmtInfo = (ia_byte*)ALLOCATE_MEMORY( ia_byte, (ia_uint32)(wDescLen+8+CA_CLIENT_SERVICE_MSG_MAX_LENGTH));
	if (pstService[wGroupHandle].pbPmtInfo != 0)
	{
		/* jyou: 3.4.2 use of wEMMHandle instead of wGroupHandle */
		pstDMX[wEMMHandle].fGetPMT  = IA_TRUE;
		ansi_memcpy(pstService[wGroupHandle].pbPmtInfo, pCaMsg->pstParams, wDescLen+8+CA_CLIENT_SERVICE_MSG_MAX_LENGTH);
	}
	else
	{
		CA_ERROR(("Failed to allocate memory.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return;
	}

	/* It's just for the new service */
	if (INVALID_SCELL_SERVICE_HANDLE == CA_SVCINFO_GetSCellHandle(wGroupHandle, CA_SERVICE_DESCRAMBLE))
	{
		SendServiceOpenRequestMsg((ia_word16)wGroupHandle, SERVICE_DVB_DESCRAMBLE);
		CA_DBG(("=> SendServiceOpenRequestMsg:[wGroupHandle]: %d serviceType = SERVICE_DVB_DESCRAMBLE\n", wGroupHandle));
	}
	else /* It's just for the PMT update */
	{
#ifdef INCL_SERVICE_CONFIGURATION
		AddGeneralConfigSourceToDescrambleService(wGroupHandle, CA_SERVICE_DESCRAMBLE);
#endif
#ifndef EXCL_MATURITY_RATING
		AddMRSourceToService(wGroupHandle, CA_SERVICE_DESCRAMBLE);
#endif
		UpdateDefaultResourcesToDescrambleService(wGroupHandle, CA_SERVICE_DESCRAMBLE);
		if(pstService[wGroupHandle].fDesServiceReady == IA_FALSE)
			pstService[wGroupHandle].fDesServiceReady = IA_TRUE;
		AddEcmSourceToDescrambleService(wGroupHandle, CA_SERVICE_DESCRAMBLE);
	}
}

static void MapResourceMR( ia_byte *pbMsg, ia_word32 wResourceID )
{
	ia_byte bIndex=0;
	ia_word16 wPayLoadLen;

#if 0
	stResourceMap.stMRMap.wResourceId = ( ia_word32 ) ( pbMsg[bIndex++] << 24 );
	stResourceMap.stMRMap.wResourceId += ( ia_word32 ) ( pbMsg[bIndex++] << 16 );
	stResourceMap.stMRMap.wResourceId += ( ia_word32 ) ( pbMsg[bIndex++] << 8 );
	stResourceMap.stMRMap.wResourceId += ( ia_word32 )( pbMsg[bIndex++] );
#else
	/* jyou: 3.6.1 */
	stResourceMap.stMRMap.wResourceId = wResourceID;
#endif

	wPayLoadLen = ((ia_word16)(pbMsg[bIndex++]<<8));
	wPayLoadLen += (ia_word16)pbMsg[bIndex++];

	if (wPayLoadLen == 2)
	{
		stResourceMap.stMRMap.abMRPin[0] = pbMsg[bIndex++];
		stResourceMap.stMRMap.abMRPin[1] = pbMsg[bIndex++];
	}
}

CA_QUERY_STATUS_EN CA_MR_PinQueryControl(ia_word32 wHandle, void* pCAClientMsg)
{
	ia_word32 wGroupHandle, wSrvType = 0;
	CA_MR_PIN_INFO_ST *pstMRPinCodeInfo;

	CA_PARAM_NOT_USED( wHandle );
	pstMRPinCodeInfo = ( CA_MR_PIN_INFO_ST* ) pCAClientMsg;
	if (0 == pstMRPinCodeInfo)
	{
		CA_ERROR(("The pointer is NULL.\n"));
		return CA_QUERY_FAIL;
	}

	wGroupHandle = pstMRPinCodeInfo->wGroupHandle;
	if (wGroupHandle >= CA_SVCINFO_GetSvcMaxNumber())
	{
		CA_ERROR(("SVC::Unknown wGroupHandle 0x%04x.\n", wGroupHandle));
		return CA_QUERY_FAIL;
	}
	/* service type is inputed by client */
	if ( SERVICE_DVB_DESCRAMBLE == pstMRPinCodeInfo->wServiceType)
		wSrvType = CA_SERVICE_DESCRAMBLE;
	if (SERVICE_PVR_PLAYBACK == pstMRPinCodeInfo->wServiceType)
		wSrvType = CA_SERVICE_PLAYBACK;

	stResourceMap.stMRMap.wResourceId = pstMRPinCodeInfo->wResourceId;
	stResourceMap.stMRMap.abMRPin[0] = pstMRPinCodeInfo->abMRPin[0];
	stResourceMap.stMRMap.abMRPin[1] = pstMRPinCodeInfo->abMRPin[1];

	AddMRSourceToService(wGroupHandle, wSrvType);

	return CA_QUERY_SUCCESS;
}

static void MapResourceGeneralConfig( ia_byte *pbMsg )
{
	ia_word16 bIndex=0, i=0;
	ia_word16 wPayLoadLen;

	wPayLoadLen = ((ia_word16)(pbMsg[bIndex++]<<8));
	wPayLoadLen += (ia_word16)pbMsg[bIndex++];

	if (wPayLoadLen == 0)
		return;

	stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount = pbMsg[bIndex++];

	if (stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount == 0)
		return;

	stResourceMap.stGeneralConfig.stConfigDesc =
		(CA_GENERAL_CONFIG_DESC_ST *)ALLOCATE_MEMORY( ia_byte,
		(sizeof(CA_GENERAL_CONFIG_DESC_ST)*stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount));

	/* Fixed SCR#27163 */
	if (stResourceMap.stGeneralConfig.stConfigDesc == 0 &&
		stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount > 0)
	{
		CA_ERROR(("Failed to allocate memory.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return;
	}

	for ( i=0; i<stResourceMap.stGeneralConfig.bConfigCnt.bConfigCount; i++ )
	{
		ansi_memset((void*)&stResourceMap.stGeneralConfig.stConfigDesc[i], 0x00,
			sizeof(CA_GENERAL_CONFIG_DESC_ST));

		stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.bTag = pbMsg[bIndex++];
		stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength
			= ((ia_word16)(pbMsg[bIndex++]<<8));
		stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength
			+= ((ia_word16)(pbMsg[bIndex++]));

		if (stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength != 0)
		{
			stResourceMap.stGeneralConfig.stConfigDesc[i].bData =
				(ia_byte*)ALLOCATE_MEMORY( ia_byte,
				stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength);
			if (stResourceMap.stGeneralConfig.stConfigDesc[i].bData == 0)
			{
				CA_ERROR(("Failed to allocate memory.\n"));
				CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
				return;
			}
			ansi_memset((void*)&stResourceMap.stGeneralConfig.stConfigDesc[i].bData[0], 0x00,
				stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength);
			ansi_memcpy((void*)&stResourceMap.stGeneralConfig.stConfigDesc[i].bData[0],
				&pbMsg[bIndex], stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength);
			bIndex += stResourceMap.stGeneralConfig.stConfigDesc[i].stConfigDescTL.wDataLength;
		}
	}
}

/* Fix SCR#26646: ayang 07/12/04 */
static void ProcessSCellMsg( CA_SCELL_MSG *pstSCellMsg )
{
	ia_word16	wOpCode = ( ia_word16 )pstSCellMsg->eOpcode;

	CA_DBG_V(("[ca_task]+++ wOpCode = 0x%04x\n", pstSCellMsg->eOpcode));

	/* validate the input OP Code */
	if ( wOpCode <= 0 || wOpCode > MSG_LPPV_OFFLINE_RECHARGE_CODE_REPLY )
	{
		CA_ERROR( ( "Unknown SoftCell Message Type %d\n", wOpCode ) );
		/*[irca ect] pyh:添加对wOpCode越界直接返回的处理。
		正常情况下不允许出现此情况，
		因为直接的返回将会导致pstSCellMsg->pvMessage空间无法释放*/
		return;
	}
	CA_DBG_V( ("[ca_task]:=> %s\n", astrSCellMsgDisplay[ wOpCode ] ) );
	/* parsing the SoftCell returned message */
	if ( afnParseReply[ wOpCode ] != 0 )
	{
		(*afnParseReply[ wOpCode ])( pstSCellMsg );
	}
#ifdef _AUSTAR_
	if((SynScMessage==5&&SynInOutMessage==1&&SynScStatusMessage==1)||
		(SynScMessage==2&&SynInOutMessage==0&&SynScStatusMessage==1))
	{
		CA_SCARD_STATUSReplyMsg(wSCardId);
		SynScStatusMessage = 0;
		SynInOutMessage	= 0;
		SynScMessage = 0;
		SynSendMessage = 0;
	}
#endif
	/* free the allocated memory */
	if( pstSCellMsg->pvMessage != 0 )
	{
		FREE_MEMORY( pstSCellMsg->pvMessage );
		pstSCellMsg->pvMessage = 0;
	}
}

/* jyou: 3.6.1 SCR#26708 2007-12-26 start */
static void MapResources( ia_byte *pbMsg )
{
	/* Parse the resource count and resource IDs */
	ia_word32 awResourceMap[CA_CLIENT_MAP_RESOURCE_HEADER_SIZE/4];
	/* ayang 2008-04-09 Change the type of bResourceCnt from ia_byte to ia_word16 */
	ia_word16 bResourceCnt, i;
	ia_word16 wResourceType;
	ia_word32 wPayLoadLen, wResourceID, wGrpHandle, wServiceType;
	ia_word32 bIndex = 0;

	CA_DBG(("=> CA_CLIENT__MAP_RESOURCES\n"));

	/* ayang: Fixed SCR#26935 */
	ansi_memset((void *)awResourceMap, 0, sizeof(ia_word32) * (CA_CLIENT_MAP_RESOURCE_HEADER_SIZE/4));

	/* ayang: Fixed SCR#26935 */
	if (GetMsgHeader(pbMsg+CA_CLIENT_SERVICE_MSG_MAX_LENGTH, awResourceMap, CA_CLIENT_MAP_RESOURCE_HEADER_SIZE-2))
	{
		wGrpHandle = awResourceMap[CA_CLIENT_SERVICE_GROUP_HANDLE];
		wServiceType = awResourceMap[CA_CLIENT_SERVICE_GROUP_MAP_TYPE];
	}
	else
	{
		CA_ERROR(("Can't get the CA_CLIENT__STOP_SERVICE_GROUP message header.\n"));
		return;
	}
#if 0
	bResourceCnt = (ia_byte)(((ia_word16)pbMsg[10]<<8) + (ia_word16)pbMsg[11]);
#else
	/* jyou: 3.6.1 try not to hard code. Ease of expansion and change */
	bIndex = CA_CLIENT_SERVICE_MSG_MAX_LENGTH + CA_CLIENT_SERVICE_GROUP_DEFINE_HEADER_SIZE;
	/* ayang: Fixed SCR#26934 */
	bResourceCnt = CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex], (ia_word16)pbMsg[bIndex+1] );
	bIndex += 2;
#endif
	/* CA_CLIENT_DESCRAMBLE_SERVICE_TYPE */
	if ( ( wServiceType & CA_CLIENT_DESCRAMBLE_SERVICE_TYPE ) != 0 )
	{
		for ( i=0; i < bResourceCnt ; i++ )
		{
#if 0
			wResourceType = (ia_word16)((ia_word16)(pbMsg[bIndex]<<8)+(ia_word16)pbMsg[bIndex+1]);
#else
			wResourceType = CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex], (ia_word16)pbMsg[bIndex+1] );
			bIndex += 2;
			/* ayang: Fixed SCR#26934 */
			wResourceID = CA_LOCAL_MakeWord32(
								CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex], (ia_word16)pbMsg[bIndex+1] ),
								CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex+2], (ia_word16)pbMsg[bIndex+3] ) );
			bIndex +=4;
#endif
			switch(wResourceType)
			{
				case RESOURCE_MATURITY_RATING:
					MapResourceMR( pbMsg+bIndex, wResourceID );
				break;
				case RESOURCE_GENERAL_CONFIGURATION:
					MapResourceGeneralConfig( pbMsg+bIndex );
				break;
				/* jyou: 3.6.1 SCR#26708 2007-12-26 start */
				case RESOURCE_COPY_CONTROL:
					/* Parse the resource count and resource IDs */
					CA_SVCINFO_SetResourceIDs( (ia_word16)wGrpHandle, CA_SERVICE_DESCRAMBLE,
						(ia_word32)wResourceType, wResourceID );
					break;
				/* jyou: 3.6.1 SCR#26708 2007-12-26 end */
				default:
				break;
			}
			/* ayang: Fixed SCR#26934 */
			wPayLoadLen = CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex], (ia_word16)pbMsg[bIndex+1] );
			bIndex += 2;

			bIndex += wPayLoadLen;
		}
	}

	/* jyou: 3.6.2 new fix CA_CLIENT_PLAYBACK_SERVICE_TYPE 2008-02-05 */
	if ( ( wServiceType & CA_CLIENT_PLAYBACK_SERVICE_TYPE ) != 0 )
	{
		for ( i=0; i < bResourceCnt ; i++ )
		{
			wResourceType = CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex], (ia_word16)pbMsg[bIndex+1] );
			bIndex += 2;
			/* ayang: Fixed SCR#26934 */
			wResourceID = CA_LOCAL_MakeWord32(
								CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex], (ia_word16)pbMsg[bIndex+1] ),
								CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex+2], (ia_word16)pbMsg[bIndex+3] ) );
			bIndex +=4;

			switch(wResourceType)
			{
				case RESOURCE_MATURITY_RATING:
					MapResourceMR( pbMsg+bIndex, wResourceID );
				break;
				case RESOURCE_GENERAL_CONFIGURATION:
					MapResourceGeneralConfig( pbMsg+bIndex );
				break;
				case RESOURCE_COPY_CONTROL:
					/* Parse the resource count and resource IDs */
					CA_SVCINFO_SetResourceIDs( (ia_word16)wGrpHandle, CA_SERVICE_PLAYBACK,
						(ia_word32)wResourceType, wResourceID );
					break;
				default:
				break;
			}

			/* ayang: Fixed SCR#26934 */
			wPayLoadLen = CA_LOCAL_MakeWord16( (ia_word16)pbMsg[bIndex], (ia_word16)pbMsg[bIndex+1] );
			bIndex += 2;

			bIndex += wPayLoadLen;
		}
	}
}

/* Added by ayang: 2008-03-13 to fix SCR#26933 */
static void CA_TMS_ReplyServiceMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle              pvMsg;
	msg_service_info_data_st   astServiceData;
	msg_service_info_st        astServices;
	ia_word16                  wIndex = 0, recordIndex[ CA_SERVICE_MAX_SERVICES ], wScellhandle;
	ia_word32                  wMaxNumGrp, wDemux;

	CA_PARAM_NOT_USED( pstSCellMsg );

	wMaxNumGrp = CA_SVCINFO_GetSvcMaxNumber();
	pvMsg = IA_OpenOutputMessage( sizeof( msg_service_info_data_st ) + sizeof( msg_service_info_st ) * CA_SERVICE_MAX_SERVICES,
																sizeof( msg_service_info_st ) );

	if( pvMsg == 0 )
	{
		CA_ERROR(("Malloc MSG_SERVICE_INFO_Reply failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return;
	}

	astServiceData.bServiceCount = 0;
	ansi_memset( &astServices, 0, sizeof( msg_service_info_st ));
	ansi_memset( &recordIndex[0], INVALID_SCELL_SERVICE_HANDLE, sizeof( ia_word16 ));

	for (wIndex = 0; wIndex < wMaxNumGrp; wIndex++)
	{
		wScellhandle = 	CA_SVCINFO_GetSCellHandle(wIndex, CA_SERVICE_DESCRAMBLE);  /* for DVB DESC service */
		if (wScellhandle != INVALID_SCELL_SERVICE_HANDLE)   /* the DESC service is opened */
		{
			recordIndex[astServiceData.bServiceCount] = wScellhandle;
			astServiceData.bServiceCount++;
		}
	}

	IA_WriteToMessage( pvMsg, &astServiceData, sizeof( msg_service_info_data_st ));

	CA_DBG_V(("=> MSG_SERVICE_INFO_Reply: servcie counter = %d\n", astServiceData.bServiceCount ));

	for( wIndex = 0; wIndex < astServiceData.bServiceCount; wIndex++ )
	{
		wDemux = CA_SVCINFO_HandleSCell2CaClient(recordIndex[wIndex]);
		if (wDemux >= CA_SVCINFO_GetSvcMaxNumber())
		{
			IA_CloseMessage( pvMsg, IA_TRUE );
			CA_ERROR(("the handle is too big.\n"));
		}

		astServices.wNetworkId = pstTsNit[wDemux].originalNetworkID;
		astServices.wTransportId = pstTsNit[wDemux].transportID;
		astServices.wServiceId = pstTsNit[wDemux].ServiceID;

		IA_WriteToMessage( pvMsg, &astServices, sizeof( msg_service_info_st ));

		CA_DBG_V(("servcie ID : %d, network ID: %d transport ID: %d\n", astServices.wServiceId, astServices.wNetworkId, astServices.wTransportId ));
	}

	if( SCELL_Message( MSG_SERVICE_INFO_REPLY, MSG_SERVICE_INFO_QUERY, IA_GetOutputMessageLength( pvMsg ), IA_GetOutputMessage( pvMsg )) == IA_SUCCESS )
	{
		IA_CloseMessage( pvMsg, IA_FALSE );
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending MSG_SERVICE_INFO_REPLY failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}
}

static void CA_TMS_ReplyTransportMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle               pvMsg;
	msg_transport_info_data_st  astTransportData;
	msg_transport_info_st       astTransports;
	ia_word16                   wDemux, wIndex, *recordIndex, wMaxGrpHandle;

	CA_PARAM_NOT_USED( pstSCellMsg );

	wMaxGrpHandle = CA_SVCINFO_GetSvcMaxNumber();
	pvMsg = IA_OpenOutputMessage( sizeof( msg_transport_info_data_st ) + sizeof( msg_transport_info_st ) * CA_SERVICE_MAX_SERVICES,
																sizeof( msg_transport_info_st ) );

	if( pvMsg == 0 )
	{
		CA_ERROR(("Malloc MSG_TRANSPORT_INFO_Relpy failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return;
	}

	recordIndex = (ia_word16*)OS_DRV_AllocateMemory(wMaxGrpHandle);
	if (recordIndex == 0)
	{
		CA_ERROR(("CA_TMS_ReplyServiceMsg: Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ;
	}

	astTransportData.bTransportCount = 0;
	ansi_memset( &astTransports, 0, sizeof( msg_transport_info_st ));

	for( wDemux = 0; wDemux < wMaxGrpHandle; wDemux++ )
	{
		if( pstTsNit[ wDemux ].originalNetworkID != INVALID_TS_NIT_ID &&
			pstTsNit[ wDemux ].transportID != INVALID_TS_NIT_ID )
		{
			recordIndex[ astTransportData.bTransportCount ] = wDemux;
			astTransportData.bTransportCount ++;
		}
	}

	IA_WriteToMessage( pvMsg, &astTransportData, sizeof( msg_transport_info_data_st ));

	CA_DBG_V(("=> MSG_TRANSPORT_INFO_Relpy: transport counter = %d\n", astTransportData.bTransportCount ));

	for( wIndex = 0; wIndex < astTransportData.bTransportCount; wIndex++ )
	{
		wDemux = recordIndex[ wIndex ];

		astTransports.wNetworkId = pstTsNit[ wDemux ].originalNetworkID;
		astTransports.wTransportId = pstTsNit[ wDemux ].transportID;

		IA_WriteToMessage( pvMsg, &astTransports, sizeof( msg_transport_info_st ));

		CA_DBG_V(( "network ID: %x transport ID: %x\n", astTransports.wNetworkId, astTransports.wTransportId ));
	}

	if( SCELL_Message( MSG_TRANSPORT_INFO_REPLY, MSG_TRANSPORT_INFO_QUERY, IA_GetOutputMessageLength( pvMsg ), IA_GetOutputMessage( pvMsg )) == IA_SUCCESS )
	{
		IA_CloseMessage( pvMsg, IA_FALSE );
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending MSG_TRANSPORT_INFO_REPLY failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}

	OS_DRV_FreeMemory( (void *)recordIndex );
	recordIndex = 0;
}

static void CA_TMS_ReplyNetworktMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle pvMsg = 0;
	msg_network_info_data_st  astNetworkData;
	msg_network_info_st       astNetworks;
	ia_word16                 wDemux, wIndex, *recordIndex, wMaxGrpHandle;

	CA_PARAM_NOT_USED( pstSCellMsg );

	astNetworkData.bNetworkCount = 0;
	ansi_memset( &astNetworks, 0, sizeof( msg_network_info_st ));
	wMaxGrpHandle = CA_SVCINFO_GetSvcMaxNumber();

	pvMsg = IA_OpenOutputMessage( sizeof( msg_network_info_data_st ) + sizeof( msg_network_info_st ) * CA_SERVICE_MAX_SERVICES,
																sizeof( msg_network_info_st ) );

	if( pvMsg == 0 )
	{
		CA_ERROR(("Malloc MSG_NETWORK_INFO_REPLY failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return;
	}

	recordIndex = (ia_word16*)OS_DRV_AllocateMemory(wMaxGrpHandle);

	/* Fixed SCR#27163 */
	if (recordIndex == 0 && wMaxGrpHandle > 0)
	{
		CA_ERROR(("CA_TMS_ReplyServiceMsg: Memory allocate failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return ;
	}

	for( wDemux = 0; wDemux < wMaxGrpHandle; wDemux++ )
	{
		if( pstTsNit[ wDemux ].originalNetworkID != INVALID_TS_NIT_ID &&
			pstTsNit[ wDemux ].transportID != INVALID_TS_NIT_ID )
		{
			recordIndex[ astNetworkData.bNetworkCount ] = wDemux;
			astNetworkData.bNetworkCount ++;
		}
	}

	CA_DBG_V(("=> MSG_NETWORK_INFO_Relpy: network counter = %d\n", astNetworkData.bNetworkCount ));

	IA_WriteToMessage( pvMsg, &astNetworkData, sizeof( msg_network_info_data_st ));

	for( wIndex = 0; wIndex < astNetworkData.bNetworkCount; wIndex++ )
	{
		wDemux = recordIndex[ wIndex ];

		astNetworks.wNetworkId = pstTsNit[ wDemux ].originalNetworkID;

		IA_WriteToMessage( pvMsg, &astNetworks, sizeof( msg_network_info_st ));

		CA_DBG_V(( "network ID: %x \n", astNetworks.wNetworkId ));
	}

	if( SCELL_Message( MSG_NETWORK_INFO_REPLY, MSG_NETWORK_INFO_QUERY, IA_GetOutputMessageLength( pvMsg ), IA_GetOutputMessage( pvMsg )) == IA_SUCCESS )
	{
		IA_CloseMessage( pvMsg, IA_FALSE );
	}
	else
	{
		/* Message not sent. We must free the buffer. */
		IA_CloseMessage( pvMsg, IA_TRUE );
		CA_ERROR(("Sending MSG_NETWORK_INFO_REPLY failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
	}

	OS_DRV_FreeMemory( (void *)recordIndex );
	recordIndex = 0;
}

/* ayang 2008-04-10 Added to avoid globle access of the varible */
void CA_Set_ScellMsgFlag(ia_byte bFlag)
{
	bSCellMsgFlag = bFlag;
}

/* ayang 2008-7-29 Fix SCR#27360,27365 */
ia_bool ServiceHandleMechanism( ca_drv_message_type_e eOpcode, ia_word32 wGroupHandle, ia_word32 wSrvType, void *pvMsg )
{
	CA_SERVICE_HANDLE_STATUS_EN eServiceHandleStatus;
	ia_word16 wScellHandle;
	ia_word32 wGrpHandle = wGroupHandle & 0x7FFF;

	eServiceHandleStatus = CA_SVCINFO_GetSCellHandleStatus(wGrpHandle, wSrvType);
	CA_DBG(("=> [GetSCellHandleStatus]: %d\n", eServiceHandleStatus));

	wScellHandle = CA_SVCINFO_GetSCellHandle(wGrpHandle, wSrvType);
	CA_DBG(("=> [GetSCellHandle]: %d\n", wScellHandle));

	switch ( eOpcode )
	{
		case MSG_SERVICE_OPEN_REQUEST:
			if ( eServiceHandleStatus == CA_SERVICE_HANDLE_EMPTY )
			{
				if( SCELL_Message(MSG_SERVICE_OPEN_REQUEST, (ia_word16)wGroupHandle, sizeof(msg_service_open_request_data_st), pvMsg) == IA_SUCCESS )
				{
					CA_SVCINFO_SetSCellHandleStatus(wGrpHandle, wSrvType, CA_SERVICE_HANDLE_PROCESSING);
					CA_DBG(("=> [SetSCellHandleStatus]: %d\n", CA_SERVICE_HANDLE_PROCESSING));
					return IA_TRUE;
				}
				else
				{
					CA_ERROR(("Sending failed.\n"));
					CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
					if(pvMsg != 0)
					{
						FREE_MEMORY(pvMsg);
						pvMsg = 0;
					}
					return IA_FALSE;
				}
			}
			else
				return IA_FALSE;
			break;

		case MSG_SERVICE_DEFINE:
			if (( eServiceHandleStatus == CA_SERVICE_HANDLE_OCCUPIED ))
			{
				if (SCELL_Message(MSG_SERVICE_DEFINE, (ia_word16)wScellHandle,
					IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg)) ==
					IA_SUCCESS)
				{
					/* The buffer will be freed by the receiver. */
					IA_CloseMessage(pvMsg, IA_FALSE);
					return IA_TRUE;
				}
				else
				{
					/* Message not sent. We must free the buffer. */
					IA_CloseMessage(pvMsg, IA_TRUE);
					CA_ERROR(("Sending failed.\n"));
					CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
					return IA_FALSE;
				}
			}
			else
				return IA_FALSE;
			break;

		case MSG_SERVICE_CLOSE:
			if ( eServiceHandleStatus == CA_SERVICE_HANDLE_OCCUPIED )
			{
				if( SCELL_Message( MSG_SERVICE_CLOSE, (ia_word16)wGroupHandle, IA_GetOutputMessageLength(pvMsg), IA_GetOutputMessage(pvMsg) ) == IA_SUCCESS )
				{
					CA_SVCINFO_SetSCellHandle( wGrpHandle, wSrvType, INVALID_SCELL_SERVICE_HANDLE );
					CA_DBG(("=> [SetSCellHandle]: %d\n", INVALID_SCELL_SERVICE_HANDLE));
					CA_SVCINFO_SetSCellHandleStatus( wGrpHandle, wSrvType, CA_SERVICE_HANDLE_EMPTY );
					CA_DBG(("=> [SetSCellHandleStatus]: %d\n", CA_SERVICE_HANDLE_EMPTY));
					/* The buffer will be freed by the receiver. */
					IA_CloseMessage( pvMsg, IA_FALSE );
					CA_DBG_V(("send MSG_SERVICE_CLOSE success\n" ));
					return IA_TRUE;
				}
				else
				{
					/* Message not sent. We must free the buffer. */
					IA_CloseMessage( pvMsg, IA_TRUE );
					CA_ERROR(("Sending failed.\n"));
					CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
					return IA_FALSE;
				}
			}
			else if ( eServiceHandleStatus == CA_SERVICE_HANDLE_PROCESSING )
			{
				CA_SVCINFO_SetSCellHandleStatus( wGrpHandle, wSrvType, CA_SERVICE_HANDLE_WAITCLOSE );
				CA_DBG(("=> [SetSCellHandleStatus]: %d\n", CA_SERVICE_HANDLE_WAITCLOSE));
				return IA_FALSE;
			}
			else
				return IA_FALSE;
			break;

		default:
			return IA_FALSE;
	}
}
