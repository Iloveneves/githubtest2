/**
* $Id: ird_msg.c,v 1.31 2008/11/12 02:50:57 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* The module handles decoder messages.
**/

/***************************************************************************
*
*        File : ird_msg.c
* Description : Parse and display Decoder Messages
* Reference   : Decoder Messages Document, 705410, Issue 1.01 plus
*                  updates on decoder control messages for Australia.
*
*       Notes : IRD_MSG_Initialise       Tested ok  |
*               IRD_MSG_Parser           Tested ok  |- Parse and display msg
*               IRD_MSG_UpdateClubList   Tested ok  |
*               IRD_MSG_GetMailAddress   Tested ok  |
*                                                   <
*               IRD_MSG_DecoderId        Tested ok  |- Decoder Id
*               IRD_MSG_CheckDigit       Tested ok  |
*
*      Author : Bruce Curtin
*      Status : Draft Code / Example Code
*     History : 0.0 01-09-98   Start coding
*                   11-05-99   Testing
*                   05-09-2000 Proper support of current headend capability
*
*                   28-09-2000 (Iris Weverling) Implement message buffers
*                              to store more than one message, sort buffer
*                              according to priority and receive date,
*                              allow a buffer flush.
*                              Add Priority, TimedDelete, ReceiveTime and
*                              DeleteTime to the messages.
*
*    Copyright: IRDETO 1998 (c)
*
*    Suggested Improvements: - Dynamic allocation of the Message buffers.
*                            - Delete Timed Messages on time (Copy the last message
*                              to the empty space, decrease the NoOfMsg and re-sort)
*
****************************************************************************/

/***************************** INCLUDE FILES *******************************/
/* #include <string.h> */
#include <irca/pubh/s3_ansi_functions.h>

#include "ird_msg.h"
#include <irca/ca_task.h>
#include "ca_local.h"
#include "ca_scard.h"
#include "ca_svcinfo.h"
#include "ca_error.h"

#include <irca/pubh/s3_osdrv.h>
#include <irca/pubh/s3_decoder_msg.h>
#include <irca/pubh/s3_service.h>

/***************************** END INCLUDE *********************************/

/***************************** DEFINITIONS *********************************/
/* Limits */
/*#define MAX_MAILS                  11*/  /* 10 plus one spare position */
/*#define MAX_ANNOUNCEMENTS          11*/  /* 10 plus one spare position */
#define MAX_CLUBS_PER_DECODER      12
#define MAX_CLUBS_PER_UPDATE       10
#define NO_CLUB_NUMBER_ASSIGNED    0xFFFF


/* Decoder message Destination Ids */
enum
{
	EPG = 0,
	DEC_CONTROLLER,
	CAM_MESSAGES,
	PROFDEC,
	ATTRIBUTED_DISPLAY_MSG
	/* Values 0x04 to 0x0f are spare */
};

/* Control message Types */
enum
{
	DOWNLOAD_CONTROL = 0,
	CALL_BACK,
	SPECIAL_MONITOR_MODE,
	READ_SC_USER_DATA,
	CHANGE_SC_PIN_CODE,
	DECODER_RECOVERY,
	DECODER_USER_PAYLOAD
	/* Values 0x07 to 0x0f are spare */
};

/***************************************************************************/

static void IRD_MSG_UpdateClubList(ia_word32 wGrpHandle, ia_bool fDeleteNumbers, ia_byte bNumberInList,
									 ia_byte* pbMsg);
static ia_bool IRD_MSG_DoClubCheck(ia_word32 wGrpHandle, ia_word16 wClubNumber);
/*static ia_char IRD_MSG_CheckDigit(ia_char* pszSn, ia_uint16 iLen);*/
/*static VOID IRD_MSG_Sort(ia_byte Size, MESSAGE *MessagePtr[]);*/
/*static ia_bool IRD_MSG_Comp(MESSAGE *MsgPtr1, MESSAGE *MsgPtr2);*/
static void IRD_MSG_ClearLocalInfo( ia_word16 wMaxNumGroups );

static ia_word16 CRC16(void* pvStartAddress, ia_word32 ulSize_in_bytes);

/************************* END DEFINITIONS *********************************/

/*************************** DECLARATIONS **********************************/

/* Storage for mail and Anouncement messages */
static MESSAGE *astMailBuf = 0 ;         /* Mail Buffer */
static MESSAGE *astAnnBuf = 0 ;  /* Anouncement Buffer */
static MESSAGE **apMailPtr ;    /* Array of pointers to the Mail Buffer */
static MESSAGE **apAnnPtr ;     /* Array of pointers to the Announcement Buffer */

static ia_byte bNoOfMails;           /* Number of Mails in the Mail Buffer */
static ia_byte bNoOfAnns;            /* Number of Announcements in the Announcement Buffer */
/*static ia_bool fInitMailMsg;*/         /* Flag to indicate that the Init Mail is in the Mail Buffer */
/*static ia_bool fInitAnnoucementMsg;*/  /* Flag to indicate that the Init Announcement is in the Announcement Buffer*/

/* Storage of Current clubs to which this user belongs */
typedef struct
{
	ia_word16 awClubNumbers[MAX_CLUBS_PER_DECODER];
	ia_word32 wGrpHandle;
}CA_IRD_MSG_T;

static  CA_IRD_MSG_T *pstIrdMsg;

/* The local structures for query control */
static CA_EMM_DOWNLOAD_T *pstDownLoad;
static CA_EMM_CHANGE_PIN_T *pstEmmChangePin;
static CA_EMM_DECODER_RECOVER_T *pstEmmDecoderRecover;
static CA_ATTRIB_T_2 *pstAttributedInfo;

/************************* END DECLARATIONS ********************************/

static CA_EMM_PAYLOAD_USER_DATA_LOCAL_T *pstCaEmmUserPayloadLocal = 0;

static void IRD_MSG_CreateIndex(ia_word16 wMaxNumGroups)
{
	ia_word16 i;

	/* Fixed SCR#27163 */
	if (0 == wMaxNumGroups)
		return;

	if (0 != pstCaEmmUserPayloadLocal)
	{
		for (i = 0; i < wMaxNumGroups; i++)
		{
			if (0 != pstCaEmmUserPayloadLocal[i].pbMessage)
			{
				OS_DRV_FreeMemory((void *) pstCaEmmUserPayloadLocal[i].pbMessage);
				pstCaEmmUserPayloadLocal[i].pbMessage = 0;
			}
		}
		OS_DRV_FreeMemory((void *)pstCaEmmUserPayloadLocal);
		pstCaEmmUserPayloadLocal = 0;
	}
	if (0 == pstCaEmmUserPayloadLocal)
	{
		pstCaEmmUserPayloadLocal = (CA_EMM_PAYLOAD_USER_DATA_LOCAL_T *)OS_DRV_AllocateMemory(
			(unsigned long)sizeof(CA_EMM_PAYLOAD_USER_DATA_LOCAL_T) * (unsigned long)wMaxNumGroups);
	}
	if (0 == pstCaEmmUserPayloadLocal)
	{
		CA_ERROR(("[IRD_MSG] Memory malloc failed !\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	ansi_memset((void *)pstCaEmmUserPayloadLocal, 0, sizeof(CA_EMM_PAYLOAD_USER_DATA_LOCAL_T)*wMaxNumGroups);

}

/****************************************************************************
*     ROUTINE : IRD_MSG_Initialise                                          *
* DESCRIPTION : Initialises stored message string and time                  *
*       INPUT : None                                                        *
*      OUTPUT : None                                                        *
*     RETURNS : None                                                        *
* SIDE EFFECT : None known so far!                                          *
*   CALLED BY : CA Task                                                     *
*      Author : Bruce Curtin                                                *
* Last Changed: 06-09-2000                                                  *
*       NOTES : 06-09-2000 Update for Club Support                          *
****************************************************************************/
void IRD_MSG_Initialise(ia_word16 wMaxNumGroups)
{
	ia_uint16 i;
	/*TIMESTAMP stTime;*/

	/* Fixed SCR#27163 */
	if (0 == wMaxNumGroups)
		return;

	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */
	if(astMailBuf != 0)
	{
		OS_DRV_FreeMemory( (void *)astMailBuf );
	}

	astMailBuf = (MESSAGE *)OS_DRV_AllocateMemory((unsigned long)sizeof(MESSAGE)* (unsigned long)MAX_MAILS *(unsigned long)wMaxNumGroups);
	if(astMailBuf == 0)
	{
		CA_DBG_V(("### astMailBuf create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	if(astAnnBuf != 0)
		OS_DRV_FreeMemory( (void *)astAnnBuf );
	astAnnBuf = (MESSAGE *)OS_DRV_AllocateMemory((unsigned long)sizeof(MESSAGE)* (unsigned long)MAX_ANNOUNCEMENTS *(unsigned long)wMaxNumGroups);
	if(astAnnBuf == 0)
	{
		CA_DBG_V(("### astAnnBuf create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	if(apMailPtr != 0)
		OS_DRV_FreeMemory( (void *)apMailPtr );
	apMailPtr = (MESSAGE **)OS_DRV_AllocateMemory((unsigned long)sizeof(MESSAGE *)* (unsigned long)MAX_MAILS *(unsigned long)wMaxNumGroups);
	if(apMailPtr == 0)
	{
		CA_DBG_V(("### apMailPtr create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	if(apAnnPtr != 0)
		OS_DRV_FreeMemory( (void *)apAnnPtr );
	apAnnPtr = (MESSAGE **)OS_DRV_AllocateMemory((unsigned long)sizeof(MESSAGE *)* (unsigned long)MAX_ANNOUNCEMENTS *(unsigned long)wMaxNumGroups);
	if(apAnnPtr == 0)
	{
		CA_DBG_V(("### apAnnPtr create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	for (i=0; i<MAX_MAILS * wMaxNumGroups ; i++)          /* Initialise the Mail pointers */
		apMailPtr[i] = &astMailBuf[i];

	for (i=0; i<MAX_ANNOUNCEMENTS * wMaxNumGroups ; i++)  /* Initialise the Announcement pointers */
		apAnnPtr[i] = &astAnnBuf[i];

	/*TD_GetTime (&stTime);*/

	/* Prepare the default Messages */
	apMailPtr[0]->bPriority = 0;           /* Low Priority */
	apMailPtr[0]->bClass = 0;
	apMailPtr[0]->bTimedDelete = 0;
	/*apMailPtr[0]->stReceiveTime = stTime;
	sprintf((ia_char*)&apMailPtr[0]->bMessageData[0], "No Mail Messages since %02d:%02d",
	apMailPtr[0]->stReceiveTime.bHour, apMailPtr[0]->stReceiveTime.bMinute*10);
	apMailPtr[0]->bMessageData[28] = '\0';
	bNoOfMails = 1;*/
	bNoOfMails = 0;

	apAnnPtr[0]->bPriority = 0;
	apAnnPtr[0]->bClass = 0;
	apAnnPtr[0]->bTimedDelete = 0;
	/*apAnnPtr[0]->stReceiveTime = stTime;
	sprintf((ia_char*)&apAnnPtr[0]->bMessageData[0], "No Announcement Messages since %02d:%02d",
	apAnnPtr[0]->stReceiveTime.bHour, apAnnPtr[0]->stReceiveTime.bMinute*10);
	apAnnPtr[0]->bMessageData[36] = '\0';
	bNoOfAnns = 1;*/
	bNoOfAnns = 0;

	/*fInitMailMsg = IA_TRUE;*/
	/*fInitAnnoucementMsg = IA_TRUE;*/

	if(pstIrdMsg != 0)
		OS_DRV_FreeMemory( (void *)pstIrdMsg );
	pstIrdMsg = (CA_IRD_MSG_T *)OS_DRV_AllocateMemory((unsigned long)sizeof(CA_IRD_MSG_T) * (unsigned long)wMaxNumGroups );
	if(pstIrdMsg == 0)
	{
		CA_DBG_V(("### pstIrdMsg create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	if(pstDownLoad != 0)
		OS_DRV_FreeMemory( (void *)pstDownLoad );
	pstDownLoad = (CA_EMM_DOWNLOAD_T *)OS_DRV_AllocateMemory((unsigned long)sizeof(CA_EMM_DOWNLOAD_T) * (unsigned long)wMaxNumGroups );
	if(pstDownLoad == 0)
	{
		CA_DBG_V(("### pstDownLoad create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	if(pstEmmChangePin != 0)
		OS_DRV_FreeMemory( (void *)pstEmmChangePin );
	pstEmmChangePin =
		(CA_EMM_CHANGE_PIN_T *)OS_DRV_AllocateMemory((unsigned long)sizeof(CA_EMM_CHANGE_PIN_T) * (unsigned long)wMaxNumGroups );
	if(pstEmmChangePin == 0)
	{
		CA_DBG_V(("### pstEmmChangePin create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	if(pstEmmDecoderRecover != 0)
		OS_DRV_FreeMemory( (void *)pstEmmDecoderRecover );
	pstEmmDecoderRecover = (CA_EMM_DECODER_RECOVER_T *)OS_DRV_AllocateMemory((unsigned long)sizeof(CA_EMM_DECODER_RECOVER_T) * (unsigned long)wMaxNumGroups );
	if(pstEmmDecoderRecover == 0)
	{
		CA_DBG_V(("### pstEmmDecoderRecover create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	if(pstAttributedInfo != 0)
		OS_DRV_FreeMemory( (void *)pstAttributedInfo );
	pstAttributedInfo =
		(CA_ATTRIB_T *)OS_DRV_AllocateMemory((unsigned long)sizeof(CA_ATTRIB_T) * (unsigned long)wMaxNumGroups );
	if(pstAttributedInfo == 0)
	{
		CA_DBG_V(("### pstAttributedInfo create failed\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
	}

	IRD_MSG_ClearLocalInfo( wMaxNumGroups);
	/* Create the EMM User Payload data management Index */
	IRD_MSG_CreateIndex(wMaxNumGroups);
}

static void IRD_MSG_ClearLocalInfo( ia_word16 wMaxNumGroups )
{
	ia_uint16 i,j;
	for(i=0; i<wMaxNumGroups; i++)
	{
		pstAttributedInfo[i].wEmmHandle = 0xFFFFFFFF ;
		pstAttributedInfo[i].wServiceHandle = 0xFFFF;
		pstEmmChangePin[i].wGroupHandle = 0xFFFFFFFF ;
		pstDownLoad[i].wEmmHandle = 0xFFFFFFFF ;
		pstDownLoad[i].fDownloadAllowed = IA_FALSE;
		pstDownLoad[i].fForcedDownload = IA_FALSE;
		pstDownLoad[i].fProfdecForcedDownload = IA_FALSE;
		pstEmmDecoderRecover[i].wGroupHandle = 0xFFFFFFFF ;
		pstEmmDecoderRecover[i].stForceTuneToSvc.wNetworkID = 0xFFFF;
		pstEmmDecoderRecover[i].stForceTuneToSvc.wTransportStreamID = 0xFFFF;
		pstEmmDecoderRecover[i].stForceTuneToSvc.wServiceID = 0xFFFF;
		pstEmmDecoderRecover[i].wBouquetID = 0xFFFF;
		for(j=0; j<4; j++)
		{
			pstEmmChangePin[i].abParentalPINCode[j] = 0;
			pstEmmDecoderRecover[i].abInstallerPINCode[j] = 0;
		}

		/* Initialise List of Club Numbers */
		pstIrdMsg[i].wGrpHandle = 0xFFFFFFFF ;
		for(j=0; j < MAX_CLUBS_PER_DECODER; j++)
		{
			pstIrdMsg[i].awClubNumbers[j] = (ia_word16)NO_CLUB_NUMBER_ASSIGNED;
		}
	}
}

void IRD_MSG_ParseDecoderMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	msg_decoder_data_st *pstMsg;
	ia_byte *pbMsg;
	ia_word16 wSrvHandle;

	pstMsg = (msg_decoder_data_st*)pstSCellMsg->pvMessage;
	CA_DBG_V(("Svc Hndl = %d, Msg Len = %d\n", pstMsg->wServiceHandle, pstMsg->wMessageLength));
	wSrvHandle = pstMsg->wServiceHandle;
	pbMsg = (ia_byte*)(&pstMsg[1]);
	IRD_MSG_Parser(pbMsg, wSrvHandle);
}

/****************************************************************************
*     ROUTINE : IRD_MSG_Parser                                              *
* DESCRIPTION : Parses Forced Text, Mail messages, Announcements etc and    *
*                      Displays these messages to the OSD                   *
*       INPUT : pbMsg - pointer to the message data                         *
*      OUTPUT : None                                                        *
*     RETURNS : None                                                        *
* SIDE EFFECT : None known so far!                                          *
*   CALLED BY : CA Task                                                     *
*      Author : Bruce Curtin                                                *
* Last Changed: 06-09-2000                                                  *
*       NOTES : 06-09-2000 Update for club support                          *
****************************************************************************/

void IRD_MSG_Parser(ia_byte* pbMsg, ia_word16 wServicehandle)
{
	ia_word16 wMsgLength;         /* The length of the decoder Message */
	ia_word16 wCRC;               /* 16 bit CRC */
	ia_byte bDestId;            /* Type of IRD Addressed Message */
	ia_byte bMsgType;           /* Type of EPG Message */
	ia_byte bClass;             /* Class of EPG Message */
	ia_byte bPriority;          /* The message Priority */
	ia_bool fClub;              /* Club Message indicator */
	ia_bool fFlush;             /* Determine whether buffer should be flushed */
	ia_bool fTimedDelete = IA_FALSE;       /* (Future) Support Delete of Message at set time */
	ia_bool fCompress;          /* Compressed Message indicator */
	ia_byte bEpgMsgLength;      /* EPG Message Length */
	ia_word16 wClubNumber;        /* The club number for the message */
	ia_bool fCheck;             /* Return Check for a club number match */
	/*TIMESTAMP stReceiveTime;*/ /* to store the Receive time */
	TIMESTAMP stDeleteTime;  /* to store Delete Time if TimedDelete is TRUE */
	/* Club Control Variables */
	ia_bool fDeleteNumbers;     /* If IA_TRUE del Club Nums from list, otherwise add */
	ia_byte bNumberOfClubs;     /* Number of Club Numbers to Add/Delete in list */

	ia_uint16  i,j = 0;
	ia_byte bRecoveryType;

	ia_word16 wNumGroups = CA_TASK_GetInitCfg()->wNumGroups;

	ia_word32 wSCardId = INVALID_SC_SMARTCARDID;
	ia_word32 wGrpHandle;

	ia_bool fGot = IA_FALSE;

	/* EMM user payload */
	/*ia_word16 wMaxNumGroups;
	wMaxNumGroups = CA_SVCINFO_GetSvcMaxNumber();	*/

	wGrpHandle = CA_SVCINFO_HandleSCell2CaClient(wServicehandle) ;
	/* ayang 2008-04-09 Change INVALID_SCELL_SERVICE_HANDLE to INVALID_GROUP_SERVICE_HANDLE */
	if (INVALID_GROUP_SERVICE_HANDLE == wGrpHandle)
	{
		return;
	}
	/* ayang 2008-04-09 Added to check the handle to make sure the usage of memory */
	if (wGrpHandle >= CA_SVCINFO_GetSvcMaxNumber())
	{
		CA_ERROR(("[IRD_MSG] The handle is too big.\n"));
		return ;
	}
	/* Get the destination ID for this Decoder Message 4 bit = 16 possible */
	bDestId = (ia_byte)( (pbMsg[0] >> 4) & 0x0f);

	/* Get the length of the Decoder Message - 12 bit = 4096 possible
	But SoftCell restricts the length to 1024 bytes */
	wMsgLength = (ia_word16)(
		( ((((ia_word16)pbMsg[0] & 0x000f) << 8) & 0xff00) ) |
		( ((ia_word16)pbMsg[1] & 0x00ff) )
		);
	/* Get CRC */
	wCRC = (ia_word16)( ((((ia_word16)pbMsg[(wMsgLength + 2)]) << 8) & 0xff00 ) |
		(((ia_word16)pbMsg[(wMsgLength+3)]) & 0x00ff) );
	/* Carry out CRC16 check */
	if( CRC16(&pbMsg[0], (ia_uint32)wMsgLength + 2L) != wCRC )
	{
		CA_ERROR(("[IRD_MSG] CRC Fail\n"));
		return;
	}

	ansi_memset((void *)&stDeleteTime, 0, sizeof(TIMESTAMP));

	/* Check Destination of Decoder Message */
	switch(bDestId)
	{
	case EPG:

		CA_DBG(("[IRD_MSG] Text msg\n"));
		/* Check the type of Message 4 bit = 16 possible */
		bMsgType = (ia_byte)( (pbMsg[2] >> 4) & 0x0f );
		CA_DBG_V(("[IRD_MSG] Text_Message_Type = %02x\n", bMsgType));
		switch(bMsgType)
		{
		case TEXT_MAILBOX:
		case TEXT_ANNOUNCEMENT:

			/* Determine the message Class */
			bClass = (ia_byte)((pbMsg[3] >> 5) & 0x07);      /* 1110 0000 Msg Class */

			/* Determine if the buffer should be flushed */
			/* The flush will be done before adding new message */
			fFlush = (pbMsg[3] & 0x10) ? IA_TRUE : IA_FALSE;    /* 0001 0000 Flush */

			/* Determine if Compression is used */
			fCompress = (pbMsg[3] & 0x08) ? IA_TRUE : IA_FALSE; /* 0000 1000 Compression Flag */
			CA_PARAM_NOT_USED(fCompress);

			/* Determine if a club message */
			fClub = (pbMsg[3] & 0x04) ? IA_TRUE : IA_FALSE;     /* 0000 0100 Club Msg */

			/* Get the priority */
			bPriority = (ia_byte)(pbMsg[3] & 0x03);                  /* 0000 0011 Priority */

			pbMsg = &pbMsg[4];
			fTimedDelete = IA_FALSE;

			/* Determine if the class is Timed */
			if(bClass == (ia_byte)TIMED)
			{
				CA_DBG_V(("[IRD_MSG] Timed\n"));
				fTimedDelete = IA_TRUE;

				/* Get the Delete Date */
				stDeleteTime.bYear = (ia_byte)((*pbMsg >> 1) & 0x7F);
				/* stDeleteTime.bMonth = (ia_byte)(((*pbMsg++ & 0x01) << 3) |
				((*pbMsg & 0xD0) >> 5)); */
				stDeleteTime.bMonth = (ia_byte)((*pbMsg++ & 0x01) << 3);
				stDeleteTime.bMonth = (ia_byte)(stDeleteTime.bMonth |(ia_byte) ((*pbMsg & 0xD0) >> 5));
				stDeleteTime.bDay = (ia_byte)(*pbMsg++ & 0x1F);
				stDeleteTime.bHour = (ia_byte)((*pbMsg >> 3) & 0x1F);
				stDeleteTime.bMinute = (ia_byte)(*pbMsg & 0x07);

				CA_DBG_V(("[IRD_MSG] Delete Time: Year=%d, Month=%d, Day=%d, Hour=%d, Minute=%d\n",
					stDeleteTime.bYear, stDeleteTime.bMonth, stDeleteTime.bDay,
					stDeleteTime.bHour, stDeleteTime.bMinute));

				*pbMsg += 1;  /* Skip over time info */
			}

			/* See if club membership is required */
			if(fClub == IA_TRUE)
			{
				/* Store the club Number */
				/*	wClubNumber =  (ia_word16)((((ia_word16)*pbMsg++ << 8) &0xff00) |
				((ia_word16)*pbMsg++ & 0x00ff));*/
				wClubNumber =  (ia_word16)(((ia_word16)*pbMsg++ << 8) &0xff00);
				wClubNumber= (ia_byte)(wClubNumber | ((ia_word16)*pbMsg++ & 0x00ff));
				CA_DBG(("[IRD_MSG] Club Number %04x\n", wClubNumber));

				/* Check if this Subscriber is a member */
				fCheck = IRD_MSG_DoClubCheck(wGrpHandle, wClubNumber);

				if(fCheck != IA_TRUE)
				{
					CA_DBG(("[IRD_MSG] Not Member!\n"));
					return;
				}

				/* Move the index past the club number */
			}

			/* Now deal with the message data */
			CA_DBG_V(("[IRD_MSG] Message_Length = %d\n", *pbMsg));
			bEpgMsgLength = *pbMsg++;

			/*TD_GetTime (&stReceiveTime);*/
			if(bMsgType == TEXT_MAILBOX)
			{
				CA_DBG(("[IRD_MSG] MailBox Message\n"));

				/*if (fInitMailMsg == IA_TRUE)
				{
				bNoOfMails = 0;
				fInitMailMsg = IA_FALSE;
				}*/

				if (fFlush == IA_TRUE)
					bNoOfMails = 0;      /* Flush Buffer */

				/* Store in Mailbox */
				ansi_memcpy(&apMailPtr[bNoOfMails]->bMessageData[0], pbMsg, bEpgMsgLength);
				apMailPtr[bNoOfMails]->bMessageData[bEpgMsgLength] = '\0';
				/* ayang: 2008-03-13 Fix SCR#26102 */
				apMailPtr[bNoOfMails]->bMessageLength = bEpgMsgLength + 1;
				apMailPtr[bNoOfMails]->bClass = bClass;
				apMailPtr[bNoOfMails]->bPriority = bPriority;
				apMailPtr[bNoOfMails]->bTimedDelete = (ia_byte)fTimedDelete;
				apMailPtr[bNoOfMails]->wServiceHandle = wServicehandle; /* mli */
				/*apMailPtr[bNoOfMails]->stReceiveTime = stReceiveTime;*/
				if (fTimedDelete == IA_TRUE)
					apMailPtr[bNoOfMails]->stDeleteTime = stDeleteTime;;

				CA_MAIL_Notify(wGrpHandle, bNoOfMails);

				if (bNoOfMails < MAX_MAILS*wNumGroups - 1)
					bNoOfMails ++;
				else
					bNoOfMails = 0; /* wrap */

				/* Sort the Mailbox */
				/*IRD_MSG_Sort(bNoOfMails+1, apMailPtr);*/
			}
			else if(bMsgType == TEXT_ANNOUNCEMENT)
			{
				CA_DBG(("[IRD_MSG] Announcement Message\n"));

				/*if (fInitAnnoucementMsg == IA_TRUE)
				{
				bNoOfAnns = 0;
				fInitAnnoucementMsg = IA_FALSE;
				}*/

				if (fFlush == IA_TRUE)
					bNoOfAnns = 0;      /* Flush Buffer */

				/* Put Message into Announcement Buffer */
				ansi_memcpy(&apAnnPtr[bNoOfAnns]->bMessageData[0], pbMsg, bEpgMsgLength);
				apAnnPtr[bNoOfAnns]->bMessageData[bEpgMsgLength] = '\0';
				/* ayang: 2008-03-13 Fix SCR#26102 */
				apAnnPtr[bNoOfAnns]->bMessageLength = bEpgMsgLength + 1;
				apAnnPtr[bNoOfAnns]->bPriority = bPriority;
				apAnnPtr[bNoOfAnns]->bClass = bClass;
				apAnnPtr[bNoOfAnns]->bTimedDelete = (ia_byte)fTimedDelete;
				apAnnPtr[bNoOfAnns]->wServiceHandle = wServicehandle;
				/*apAnnPtr[bNoOfAnns]->stReceiveTime = stReceiveTime;*/
				if (fTimedDelete == IA_TRUE)
					apAnnPtr[bNoOfAnns]->stDeleteTime = stDeleteTime;

				CA_MAIL_NotifyAnnounce(wGrpHandle, bNoOfAnns);

				if (bNoOfAnns < MAX_ANNOUNCEMENTS*wNumGroups - 1)
					bNoOfAnns ++;
				else
					bNoOfAnns = 0; /* wrap */

				/*IRD_MSG_Sort(bNoOfAnns+1, apAnnPtr);*/
			}
			break;

		case CLUB_NUMBERS:
			CA_DBG(("[IRD_MSG] Club Number Control\n"));
			fDeleteNumbers = (pbMsg[3] & 0x80) ? IA_TRUE : IA_FALSE; /* 1000 0000 Mask top bit */
			bNumberOfClubs = (ia_byte)(pbMsg[3] & 0x7f);                  /* 0111 1111 Mask length  */
			IRD_MSG_UpdateClubList(wGrpHandle, fDeleteNumbers, bNumberOfClubs, &pbMsg[4]);
			break;

		default:
			CA_DBG(("[IRD_MSG] Spare EPG Types\n"));
			break;
		}
		break;

	case DEC_CONTROLLER:

		CA_DBG(("[IRD_MSG] Decoder controller\n"));
		pbMsg = &pbMsg[2];
		bMsgType = (ia_byte)(((ia_byte)(*pbMsg++ >>4)) & 0x0F);
		CA_DBG_V(("[IRD_MSG] Control_Message_Type = %02x\n", bMsgType));
		switch(bMsgType)
		{
		case DOWNLOAD_CONTROL:
			for(i=0; i < wNumGroups; i++)
			{
				if(pstDownLoad[i].wEmmHandle ==  0xffffffff)
				{
					fGot = IA_TRUE ;
					j = i ;
				}

				if(pstDownLoad[i].wEmmHandle == wGrpHandle )
				{
					fGot = IA_TRUE ;
					j = i ;
					break ;
				}
			}
			if(fGot == IA_TRUE)
			{
				pstDownLoad[j].wEmmHandle = wGrpHandle ;
				pstDownLoad[j].fDownloadAllowed        = (ia_bool)( (*pbMsg & 0x80)>>7 );
				pstDownLoad[j].fForcedDownload         = (ia_bool)( (*pbMsg & 0x40)>>6 );
				pstDownLoad[j].fProfdecForcedDownload = (ia_bool)( (*pbMsg & 0x20)>>5 );

				CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_DOWNLOAD, 0);
			}
			break;

		case READ_SC_USER_DATA:
			/* jyou: 3.4.2 multiple smart cards */
			for ( i = 0 ; i < SC_SLOT_MAX ; i++ )
			{
				wSCardId = CA_SCARD_GetSmartCardId( (ia_byte)i );
				CA_SCARD_SendUserDataQueryMsg(wSCardId );
				CA_SCARD_SendSmartCardHomingChQueryMsg(wSCardId);

				/* read the region data */
				for( i = 0; i < CA_SCARD_GetSectorNumber(wSCardId); i++ )
				{
					CA_SCARD_RegionSendQuery( CA_SCARD_GetSectorID(wSCardId, i), wSCardId);
				}
			}
			break;

		case CHANGE_SC_PIN_CODE:

			for(i=0; i < wNumGroups; i++)
			{
				if(pstEmmChangePin[i].wGroupHandle ==  0xffffffff)
				{
					fGot = IA_TRUE ;
					j = i ;
				}

				if(pstEmmChangePin[i].wGroupHandle == wGrpHandle )
				{
					fGot = IA_TRUE ;
					j = i ;
					break ;
				}
			}
			if(fGot == IA_TRUE)
			{
				pstEmmChangePin[j].wGroupHandle = wGrpHandle ;

				pstEmmChangePin[j].bParentalPINIndex = *pbMsg++;
				for (i=0; i<4; i+=2)
				{
					pstEmmChangePin[j].abParentalPINCode[i] = (ia_byte)(((*pbMsg)&0xF0)>>4);
					pstEmmChangePin[j].abParentalPINCode[i+1] = (ia_byte)((*pbMsg++)&0x0F);
				}
				CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_CHANGEPIN, pstEmmChangePin[j].bParentalPINIndex);
			}
			break;

		case DECODER_RECOVERY:
			bRecoveryType = (ia_byte)((*pbMsg++)>>4);
			CA_DBG_V(("[IRD_MSG] Recovery_Type = %02x\n", bRecoveryType));
			switch(bRecoveryType)
			{
			case RESET_IRD:
				/* CA_CLIENT_Notify(CA_DEMUX_ALL, CA_QC_EMM_DECODER_RECOVER, (ia_uint32)RESET_IRD);*/
				CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_DECODER_RECOVER, (ia_uint32)RESET_IRD);
				break;

			case RESET_CHANNEL_ORDER:
				for(i=0; i < wNumGroups; i++)
				{
					if(pstEmmDecoderRecover[i].wGroupHandle ==  0xffffffff)
					{
						fGot = IA_TRUE ;
						j = i ;
					}

					if(pstEmmDecoderRecover[i].wGroupHandle == wGrpHandle )
					{
						fGot = IA_TRUE ;
						j = i ;
						break ;
					}
				}
				if(fGot == IA_TRUE)
				{
					pstEmmDecoderRecover[j].wGroupHandle = wGrpHandle ;
					pstEmmDecoderRecover[j].wBouquetID  = (ia_word16)((*pbMsg++)<<8);
					pstEmmDecoderRecover[j].wBouquetID += (ia_word16)(*pbMsg);
					CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_DECODER_RECOVER, (ia_uint32)RESET_CHANNEL_ORDER);
				}
				break;

			case RE_SCAN_SI_RESET_ORDER:
				CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_DECODER_RECOVER, (ia_uint32)RE_SCAN_SI_RESET_ORDER);
				break;

			case ERASE_SI_RESET_RETUNE:
				CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_DECODER_RECOVER, (ia_uint32)ERASE_SI_RESET_RETUNE);
				break;

			case RECONNECT_TO_SERVICE:
				CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_DECODER_RECOVER, (ia_uint32)RECONNECT_TO_SERVICE);
				break;

			case FORCE_TUNE_TO_SERVICE:
				for(i=0; i < wNumGroups; i++)
				{
					if(pstEmmDecoderRecover[i].wGroupHandle ==  0xffffffff)
					{
						fGot = IA_TRUE ;
						j = i ;
					}

					if(pstEmmDecoderRecover[i].wGroupHandle == wGrpHandle )
					{
						fGot = IA_TRUE ;
						j = i ;
						break ;
					}
				}
				if(fGot == IA_TRUE)
				{
					pstEmmDecoderRecover[j].wGroupHandle = wGrpHandle ;
					pstEmmDecoderRecover[j].stForceTuneToSvc.wGroupHandle = wGrpHandle ;
					pstEmmDecoderRecover[j].stForceTuneToSvc.wNetworkID = (ia_word16)((*pbMsg++)<<8);
					pstEmmDecoderRecover[j].stForceTuneToSvc.wNetworkID += (ia_word16)(*pbMsg++);
					pstEmmDecoderRecover[j].stForceTuneToSvc.wTransportStreamID = (ia_word16)((*pbMsg++)<<8);
					pstEmmDecoderRecover[j].stForceTuneToSvc.wTransportStreamID += (ia_word16)(*pbMsg++);
					pstEmmDecoderRecover[j].stForceTuneToSvc.wServiceID = (ia_word16)((*pbMsg++)<<8);
					pstEmmDecoderRecover[j].stForceTuneToSvc.wServiceID += (ia_word16)(*pbMsg);
					CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_DECODER_RECOVER, (ia_uint32)FORCE_TUNE_TO_SERVICE);
				}
				break;

			case FORCE_SET_PIN_CODE:

				for(i=0; i < wNumGroups; i++)
				{
					if(pstEmmDecoderRecover[i].wGroupHandle ==  0xffffffff)
					{
						fGot = IA_TRUE ;
						j = i ;
					}

					if(pstEmmDecoderRecover[i].wGroupHandle == wGrpHandle )
					{
						fGot = IA_TRUE ;
						j = i ;
						break ;
					}
				}
				if(fGot == IA_TRUE )
				{
					pstEmmDecoderRecover[j].wGroupHandle = wGrpHandle ;
					for(i=0; i<4; i+=2)
					{
						pstEmmDecoderRecover[j].abInstallerPINCode[i] = (ia_byte)(((*pbMsg) & 0xF0)>>4);
						pstEmmDecoderRecover[j].abInstallerPINCode[i+1] = (ia_byte)((*pbMsg++) & 0x0F);
					}
					CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_DECODER_RECOVER, (ia_uint32)FORCE_SET_PIN_CODE);
				}
				break;

			default:
				CA_ERROR(("Unknown Recovery_Type\n"));
				break;
			}
			break;
		case DECODER_USER_PAYLOAD:  /* mli */
			{
				CA_DBG(("[IRD_MSG] Decoder User Payload!\n"));
				if (0 == pstCaEmmUserPayloadLocal)
				{
					CA_ERROR(("Invalid pointer! \n"));
					CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_POINTER);
					break;
				}
				if (0 != pstCaEmmUserPayloadLocal[wGrpHandle].pbMessage)
				{
					OS_DRV_FreeMemory((void *)pstCaEmmUserPayloadLocal[wGrpHandle].pbMessage);
					pstCaEmmUserPayloadLocal[wGrpHandle].pbMessage = 0;
					pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength = 0;
				}
				pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength = (ia_byte)(wMsgLength - 1); /* execluding MsgType 1 byte */
				pstCaEmmUserPayloadLocal[wGrpHandle].pbMessage = OS_DRV_AllocateMemory(
					(unsigned long)sizeof(ia_byte) * (unsigned long)(pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength));
				/* Fixed SCR#27163 */
				if (pstCaEmmUserPayloadLocal[wGrpHandle].pbMessage == 0 &&
					pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength > 0)
				{
					CA_ERROR(("IRD_MSG_Parser: Memory allocate failed.\n"));
					CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
					break ;
				}
				ansi_memcpy((void *)pstCaEmmUserPayloadLocal[wGrpHandle].pbMessage,
					(void *)pbMsg, pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength);

				CA_DBG(("[IRD_MSG] GrpHandle=%d, MsgLength=%d!\n", wGrpHandle, pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength));
				CA_CLIENT_Notify(wGrpHandle, CA_QC_EMM_USER_PAYLOAD, (ia_word32)pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength);
			} /* end case */
			break;

		default:
			break;
		}
		break;

	case CAM_MESSAGES:
		CA_DBG(("[IRD_MSG] CAM msg\n"));
		break;

	case PROFDEC:
		CA_DBG(("[IRD_MSG] Prof-Dec msg\n"));
		break;

	case ATTRIBUTED_DISPLAY_MSG:

		pbMsg = &pbMsg[ 2 ];

		CA_DBG(("[IRD_MSG] Attribute-display msg\n"));
		for(i=0; i < wNumGroups; i++)
		{
			if(pstAttributedInfo[i].wEmmHandle ==  0xffffffff)
			{
				fGot = IA_TRUE ;
				j = i ;
			}

			if(pstAttributedInfo[i].wEmmHandle == wGrpHandle )
			{
				fGot = IA_TRUE ;
				j = i ;
				break ;
			}
		}
		if(fGot != IA_TRUE)
			break ;

		pstAttributedInfo[j].wServiceHandle = wServicehandle;
		pstAttributedInfo[j].wEmmHandle = wGrpHandle;
		pstAttributedInfo[j].bIndex = ( unsigned char)3; /*mli*/
		pstAttributedInfo[j].bType = *pbMsg++;
		pstAttributedInfo[j].wDuration = ( ia_word16 )(( *pbMsg++ ) << 8 );
		pstAttributedInfo[j].wDuration += ( ia_word16 )( *pbMsg++ );
		pstAttributedInfo[j].bDisplayMethod = *pbMsg++;
		pstAttributedInfo[j].bFingerprintType = *pbMsg >> 7 & 1; /* Add support for fingerprint. */

		wMsgLength = (ia_word16)(
			( ((((ia_word16)pbMsg[0] & 0x000f) << 8) & 0xff00) ) |
			( ((ia_word16)pbMsg[1] & 0x00ff) )
			);

		pbMsg = pbMsg + 2;

		if( wMsgLength >= MAX_EPG_MSG_LENGTH )
		{
			break;
		}
		pstAttributedInfo[j].wMsgLength = wMsgLength ;
		ansi_memcpy( pstAttributedInfo[j].bMessageData, pbMsg, wMsgLength );
		pstAttributedInfo[j].bMessageData[ wMsgLength ] = '\n';

		CA_CLIENT_Notify( wGrpHandle, CA_QC_ATTRIBUTED_DISPLAY_MESSAGE, (ia_byte)pstAttributedInfo[j].bIndex );
		CA_CLIENT_Notify( wGrpHandle, CA_QC_ATTRIBUTED_DISPLAY_MESSAGE_2, (ia_byte)pstAttributedInfo[j].bIndex );
		break;

	default:
		CA_DBG(("[IRD_MSG] Unknown Destination_ID = %02x\n", bDestId));
		break;
	}
}

CA_QUERY_STATUS_EN IRD_MSG_EmmDownloadQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_sint16 i;
	ia_bool fGot = IA_FALSE;
	ia_word16 wNumGroups = CA_TASK_GetInitCfg()->wNumGroups;
	CA_EMM_DOWNLOAD_T* pstDLoad;

	CA_PARAM_NOT_USED( wHandle );
	pstDLoad = ( CA_EMM_DOWNLOAD_T* ) pCAClientMsg;
	for(i=0; i < wNumGroups; i++)
	{
		if(pstDownLoad[i].wEmmHandle == pstDLoad->wEmmHandle )
		{
			fGot = IA_TRUE ;
			break ;
		}
	}

	if(fGot != IA_TRUE )
		return CA_QUERY_NOT_AVAILABLE;

	pstDLoad->fDownloadAllowed        = pstDownLoad[i].fDownloadAllowed;
	pstDLoad->fForcedDownload         = pstDownLoad[i].fForcedDownload;
	pstDLoad->fProfdecForcedDownload = pstDownLoad[i].fProfdecForcedDownload;
	return CA_QUERY_SUCCESS;
}

/* jyou: 3.4.3.9 SCR#26071 2007-06-14 */
CA_QUERY_STATUS_EN IRD_MSG_EmmChangePinQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_sint16 i,j;
	ia_bool fGot = IA_FALSE;
	ia_word16 wNumGroups = CA_TASK_GetInitCfg()->wNumGroups;
	CA_EMM_CHANGE_PIN_T* pstChangePin;

	CA_PARAM_NOT_USED( wHandle );
	pstChangePin = ( CA_EMM_CHANGE_PIN_T* ) pCAClientMsg;
	for(i=0; i < wNumGroups; i++)
	{
		if(pstEmmChangePin[i].wGroupHandle == pstChangePin->wGroupHandle )
		{
			fGot = IA_TRUE ;
			break ;
		}
	}

	if(fGot != IA_TRUE )
		return CA_QUERY_NOT_AVAILABLE;

	pstChangePin->bParentalPINIndex = pstEmmChangePin[i].bParentalPINIndex;

	for(j=0; j<4; j++)
	{
		pstChangePin->abParentalPINCode[j] = pstEmmChangePin[i].abParentalPINCode[j];
	}
	return CA_QUERY_SUCCESS;
}

CA_QUERY_STATUS_EN IRD_MSG_DecRecQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_sint16 i, j;
	ia_bool fGot = IA_FALSE;
	ia_word16 wNumGroups = CA_TASK_GetInitCfg()->wNumGroups;
	CA_EMM_DECODER_RECOVER_T *pstDecRec;

	CA_PARAM_NOT_USED( wHandle );
	pstDecRec = ( CA_EMM_DECODER_RECOVER_T* ) pCAClientMsg;
	for(i=0; i < wNumGroups; i++)
	{
		if(pstEmmDecoderRecover[i].wGroupHandle == pstDecRec->wGroupHandle )
		{
			fGot = IA_TRUE ;
			break ;
		}
	}

	if(fGot != IA_TRUE )
		return CA_QUERY_NOT_AVAILABLE ;

	switch( pstDecRec->bRecoveryType )
	{
	case RESET_IRD:
	case RE_SCAN_SI_RESET_ORDER:
	case ERASE_SI_RESET_RETUNE:
	case RECONNECT_TO_SERVICE:
		break;

	case RESET_CHANNEL_ORDER:
		pstDecRec->wBouquetID = pstEmmDecoderRecover[i].wBouquetID;
		break;

	case FORCE_TUNE_TO_SERVICE:
		pstDecRec->stForceTuneToSvc = pstEmmDecoderRecover[i].stForceTuneToSvc;
		break;

	case FORCE_SET_PIN_CODE:
		for(j=0; j<4; j++) /* Fixed SCR 27680, 27521*/
		{
			pstDecRec->abInstallerPINCode[j] = pstEmmDecoderRecover[i].abInstallerPINCode[j];
		}
		break;
	}
	return CA_QUERY_SUCCESS;
}

CA_QUERY_STATUS_EN IRD_MSG_EmmUserPayloadQueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_word32 wGrpHandle = 0xffffffff;
	CA_QUERY_STATUS_EN retStatus = CA_QUERY_SUCCESS;
	CA_EMM_USER_PAYLOAD_DATA_T *pstEmmUserPayload;

	CA_PARAM_NOT_USED( wHandle );
	pstEmmUserPayload = ( CA_EMM_USER_PAYLOAD_DATA_T* )pCAClientMsg;
	if (0 == pstEmmUserPayload)
	{
		retStatus = CA_QUERY_FAIL;
		return retStatus;
	}
	if (0 == pstEmmUserPayload->pbMessage)
	{
		retStatus = CA_QUERY_FAIL;
		return retStatus;
	}

	wGrpHandle = pstEmmUserPayload->wEmmHandle;
	if (wGrpHandle > CA_SVCINFO_GetSvcMaxNumber())
	{
		retStatus = CA_QUERY_FAIL;
		return retStatus;
	}

	if (pstEmmUserPayload->wMessageLength > pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength)
	{
		ansi_memcpy((void *)pstEmmUserPayload->pbMessage, (void *)pstCaEmmUserPayloadLocal[wGrpHandle].pbMessage,
			pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength);

	}
	else
	{
		ansi_memcpy((void *)pstEmmUserPayload->pbMessage, (void *)pstCaEmmUserPayloadLocal[wGrpHandle].pbMessage,
			pstEmmUserPayload->wMessageLength);
	}
	pstEmmUserPayload->wMessageLength = pstCaEmmUserPayloadLocal[wGrpHandle].wMessageLength;   /* org length */


	return retStatus;
}

CA_QUERY_STATUS_EN IRD_MSG_AttributedDisplay( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_sint16 i;
	ia_bool fGot = IA_FALSE;
	ia_word16 wNumGroups = CA_TASK_GetInitCfg()->wNumGroups;
	CA_ATTRIB_T *pstAttributedDisplay;

	CA_PARAM_NOT_USED( wHandle );
	pstAttributedDisplay = ( CA_ATTRIB_T* ) pCAClientMsg;
	for(i=0; i < wNumGroups; i++)
	{
		if(pstAttributedInfo[i].wEmmHandle == pstAttributedDisplay->wEmmHandle )
		{
			fGot = IA_TRUE ;
			break ;
		}
	}

	if(fGot != IA_TRUE )
		return CA_QUERY_NOT_AVAILABLE;

	if( pstAttributedInfo[i].bIndex != pstAttributedDisplay->bIndex )
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	pstAttributedDisplay->wServiceHandle = pstAttributedInfo[i].wServiceHandle;
	pstAttributedDisplay->wEmmHandle = pstAttributedInfo[i].wEmmHandle;
	pstAttributedDisplay->bType = pstAttributedInfo[i].bType ;
	pstAttributedDisplay->wDuration = pstAttributedInfo[i].wDuration ;
	pstAttributedDisplay->bDisplayMethod = pstAttributedInfo[i].bDisplayMethod;
	pstAttributedDisplay->wMsgLength = pstAttributedInfo[i].wMsgLength ;
	ansi_memcpy( (char *)pstAttributedDisplay->bMessageData, (char *)pstAttributedInfo[i].bMessageData,  pstAttributedInfo[i].wMsgLength);

	return CA_QUERY_SUCCESS;
}
CA_QUERY_STATUS_EN IRD_MSG_AttributedDisplay_2( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_sint16 i;
	ia_bool fGot = IA_FALSE;
	ia_word16 wNumGroups = CA_TASK_GetInitCfg()->wNumGroups;
	CA_ATTRIB_T_2 *pstAttributedDisplay;

	CA_PARAM_NOT_USED( wHandle );
	pstAttributedDisplay = ( CA_ATTRIB_T_2* ) pCAClientMsg;
	for(i=0; i < wNumGroups; i++)
	{
		if(pstAttributedInfo[i].wEmmHandle == pstAttributedDisplay->wEmmHandle )
		{
			fGot = IA_TRUE ;
			break ;
		}
	}

	if(fGot != IA_TRUE )
		return CA_QUERY_NOT_AVAILABLE;

	if( pstAttributedInfo[i].bIndex != pstAttributedDisplay->bIndex )
	{
		return CA_QUERY_NOT_AVAILABLE;
	}

	pstAttributedDisplay->wServiceHandle = pstAttributedInfo[i].wServiceHandle;
	pstAttributedDisplay->wEmmHandle = pstAttributedInfo[i].wEmmHandle;
	pstAttributedDisplay->bType = pstAttributedInfo[i].bType ;
	pstAttributedDisplay->wDuration = pstAttributedInfo[i].wDuration ;
	pstAttributedDisplay->bDisplayMethod = pstAttributedInfo[i].bDisplayMethod;
	pstAttributedDisplay->bFingerprintType = pstAttributedInfo[i].bFingerprintType;
	pstAttributedDisplay->wMsgLength = pstAttributedInfo[i].wMsgLength ;
	pstAttributedDisplay->bRFU = 0;
	ansi_memcpy( (char *)pstAttributedDisplay->bMessageData, (char *)pstAttributedInfo[i].bMessageData,  pstAttributedInfo[i].wMsgLength);

	return CA_QUERY_SUCCESS;
}

MESSAGE* IRD_MSG_GetMailMsg( ia_byte bMailIndex )
{
	return apMailPtr[bMailIndex];
}

MESSAGE* IRD_MSG_GetAnnounceMsg( ia_byte bAnnIndex )
{
	return apAnnPtr[bAnnIndex];
}


static ia_bool IRD_MSG_DoClubCheck(ia_word32 wGrpHandle, ia_word16 wClubNumber)
{
	ia_sint16   i,j;
	ia_bool fGot = IA_FALSE;
	ia_word16 wNumGroups = CA_TASK_GetInitCfg()->wNumGroups;

	for(i=0; i < wNumGroups; i++)
	{
		pstIrdMsg[i].wGrpHandle = wGrpHandle ;
		fGot = IA_TRUE ;
	}

	if(fGot != IA_TRUE)
		return IA_FALSE;

	/* Try to Find Number in List */
	for(j=0; j < MAX_CLUBS_PER_DECODER; j++)
	{
		if(pstIrdMsg[i].awClubNumbers[j] == wClubNumber)
		{
			/*CA_DBG(("[IRD_MSG] Match at pos %02x!\n", j));*/
			/*CA_DBG(("[IRD_MSG] Printing msg for club...%04x\n", awClubNumbers[j]));*/
			return IA_TRUE;
		}
	} /* End of stored numbers */

	/*CA_DBG(("[IRD_MSG] No match..disregarding msg.%04x!\n", wClubNumber);*/
	return IA_FALSE;
}


static void IRD_MSG_UpdateClubList(ia_word32 wGrpHandle, ia_bool fDeleteNumbers, ia_byte bNumberOfClubs,
									 ia_byte* pbMsg)
{
	ia_word16   wClubNumber;
	ia_int16  i, ix2, j, k = 0,m;
	ia_bool   fMatch ,fGot = IA_FALSE;
	ia_word16 wNumGroups = CA_TASK_GetInitCfg()->wNumGroups;

	if(bNumberOfClubs >= MAX_CLUBS_PER_UPDATE)
	{
		/*CA_DBG(("[IRD_MSG] Update List too long %04x\n", bNumberOfClubs));*/
		/* Set to maximum */
		bNumberOfClubs = MAX_CLUBS_PER_UPDATE;
	}
	else if(bNumberOfClubs == 0x00)
	{
		/*CA_DBG(("[IRD_MSG] Zero in list\n"));*/
		return;
	}

	for(m=0; m < wNumGroups; m++)
	{
		if(pstIrdMsg[m].wGrpHandle ==  0xffffffff)
		{
			k = m ;
			fGot = IA_TRUE ;
		}

		if(pstIrdMsg[m].wGrpHandle == wGrpHandle )
		{
			k = m ;
			fGot = IA_TRUE ;
			break ;
		}
	}

	if(fGot != IA_TRUE)
		return ;

	pstIrdMsg[k].wGrpHandle = wGrpHandle;

	/* Update the club List */

	if(fDeleteNumbers == IA_TRUE)
	{
		/*CA_DBG(("[IRD_MSG] Delete Club Numbers\n"));*/

		for(i=0, ix2=0; i < bNumberOfClubs; i++, ix2+=2)
		{

			/* Will have to delete numbers from list */
			/* Delete if in list */
			wClubNumber = (ia_word16)((((ia_word16)pbMsg[ix2] << 8) & 0xff00) |
				((ia_word16)pbMsg[ix2+1] & 0x00ff));
			fMatch = IA_FALSE;

			/* Try to Find Number in List */
			for(j=0; j < MAX_CLUBS_PER_DECODER; j++)
			{
				if(pstIrdMsg[k].awClubNumbers[j] == wClubNumber)
				{
					/*CA_DBG(("[IRD_MSG] Match at pos %02x!\n", j));*/
					/*CA_DBG(("[IRD_MSG] Deleting..%04x\n", awClubNumbers[j]));*/
					pstIrdMsg[k].awClubNumbers[j] = NO_CLUB_NUMBER_ASSIGNED;
					fMatch = IA_TRUE;
					break;
				}
			} /* End of stored numbers */

			if(fMatch != IA_TRUE)
				CA_DBG(("[IRD_MSG] No match for %04x!\n", wClubNumber));

		} /* End of list of numbers */

	}
	else
	{
		/* Will have to add numbers to list */
		for(i=0, ix2=0; i < bNumberOfClubs; i++, ix2+=2)
		{
			/* Get a new number from list */
			wClubNumber = (ia_word16)((((ia_word16)pbMsg[ix2] << 8) & 0xff00) |
				((ia_word16)pbMsg[ix2+1] & 0x00ff));

			fMatch = IA_FALSE;

			/* Is the club Number already in the list? */
			for(j=0; j < MAX_CLUBS_PER_DECODER; j++)
			{
				if(pstIrdMsg[k].awClubNumbers[j] == wClubNumber)
				{
					CA_DBG(("[IRD_MSG] Club Number %04x found at %02x!\n", wClubNumber, j));
					fMatch = IA_TRUE;
					break;
				}
			} /* End of stored numbers */

			/* Try to find empty position in stored club List */
			if (fMatch != IA_TRUE)
			{
				for(j=0; j < MAX_CLUBS_PER_DECODER; j++)
				{
					if(pstIrdMsg[k].awClubNumbers[j] == NO_CLUB_NUMBER_ASSIGNED)
					{
						CA_DBG(("[IRD_MSG] Free pos at %02x!\n", j));
						CA_DBG(("[IRD_MSG] Adding...%04x\n", wClubNumber));
						pstIrdMsg[k].awClubNumbers[j] = wClubNumber;
						fMatch = IA_TRUE;
						break;
					}
				} /* End of stored numbers */
			}

			if(fMatch != IA_TRUE)
				CA_DBG(("[IRD_MSG] No empty position found for %04x!\n", wClubNumber));
		}
	}
}

void CA_IRD_MemoryFree()
{
	ia_word16 i, wMaxNum = 0;

	wMaxNum = CA_SVCINFO_GetSvcMaxNumber();
	if (0 != pstCaEmmUserPayloadLocal)
	{
		for (i = 0; i < wMaxNum; i++)
		{
			if (0 != pstCaEmmUserPayloadLocal[i].pbMessage)
			{
				OS_DRV_FreeMemory((void *)pstCaEmmUserPayloadLocal[i].pbMessage);
				pstCaEmmUserPayloadLocal[i].pbMessage = 0;
				pstCaEmmUserPayloadLocal[i].wMessageLength = 0;
			}
		}
	}
	if (0 != pstCaEmmUserPayloadLocal)
	{
		OS_DRV_FreeMemory((void *)pstCaEmmUserPayloadLocal);
		pstCaEmmUserPayloadLocal = 0;
	}

	if(astMailBuf != 0)
	{
		OS_DRV_FreeMemory((void *) astMailBuf );
		astMailBuf = 0;
	}
	if(astAnnBuf != 0)
	{
		OS_DRV_FreeMemory((void *) astAnnBuf );
		astAnnBuf = 0;
	}

	if(apMailPtr != 0)
	{
		OS_DRV_FreeMemory( (void *)apMailPtr );
		apMailPtr = 0;
	}

	if(apAnnPtr != 0)
	{
		OS_DRV_FreeMemory( (void *)apAnnPtr );
		apAnnPtr = 0;
	}

	if(pstIrdMsg != 0)
	{
		OS_DRV_FreeMemory( (void *)pstIrdMsg );
		pstIrdMsg = 0;
	}

	if(pstDownLoad != 0)
	{
		OS_DRV_FreeMemory( (void *)pstDownLoad );
		pstDownLoad = 0;
	}

	if(pstEmmChangePin != 0)
	{
		OS_DRV_FreeMemory( (void *)pstEmmChangePin );
		pstEmmChangePin = 0;
	}

	if(pstEmmDecoderRecover != 0)
	{
		OS_DRV_FreeMemory( (void *)pstEmmDecoderRecover );
		pstEmmDecoderRecover = 0;
	}

	if(pstAttributedInfo != 0)
	{
		OS_DRV_FreeMemory((void *) pstAttributedInfo );
		pstAttributedInfo = 0;
	}
}


/***************************** END IRD_MSG.C ******************************/

/* rsjc */


static const ia_word16 awTable_CRC16[256] =
{
	0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
		0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
		0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
		0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
		0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
		0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
		0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
		0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
		0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
		0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
		0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
		0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
		0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
		0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
		0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
		0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
		0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
		0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
		0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
		0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
		0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
		0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
		0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
		0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
		0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
		0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
		0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
		0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
		0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
		0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
		0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
		0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040
};


/*****************************************************************************
*                                                                            *
*     ROUTINE : CRC16                                                        *
*                                                                            *
* DESCRIPTION : Calculates the "standard" PC 16 bit CRC from a start address *
*               for the size specified.  This the algorithm gives the same   *
*               result as used by PC archiving programs that use 16 bit CRCs.*
*               e.g. PKARC, LHA, LHARC, LZH etc.                             *
*                                                                            *
*       INPUT : pvStartAddress  - start address of data for crc'ing          *
*               ulSize          - block size to be crc'ed                    *
*                                                                            *
*      OUTPUT : none                                                         *
*                                                                            *
*     RETURNS : 16 bit CRC code                                              *
*                                                                            *
*      Author :  A.K.Rossouw                                                 *
*                                                                            *
* Last Changed:                                                              *
*                                                                            *
*****************************************************************************/
ia_word16 CRC16(void* pvStartAddress, ia_word32 ulSize_in_bytes)
{
	ia_word16        wCRC;
	ia_byte  *pbData;

	/* init the start value */
	wCRC = 0x0;
	/* calculate CRC */
	pbData = pvStartAddress;
	while (ulSize_in_bytes--)
	{
		wCRC = (ia_word16)(((wCRC >> 8) & 0xFF) ^ awTable_CRC16[((wCRC ^ *pbData++) & 0xFF)]);
	}
	return (wCRC);
}  /* CRC_16 */





