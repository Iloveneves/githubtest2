/**
* $Id: ird_msg.h,v 1.10 2008/11/12 02:50:57 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* Header file for IRD message handler.
**/

#ifndef _IRD_MSG_H_
#define _IRD_MSG_H_

#include <irca/ca_task.h>
#include "ca_mail.h"
#include "ca_local.h"


#define MAX_MAILS                  11  /* 10 plus one spare position */
#define MAX_ANNOUNCEMENTS          11  /* 10 plus one spare position */

typedef struct
{
	ia_word16  wServiceHandle; /* mli */
	ia_byte bMessageData[MAX_EPG_MSG_LENGTH];
	ia_byte bPriority;
	ia_byte bClass;
	ia_byte bTimedDelete;
	TIMESTAMP stReceiveTime;
	TIMESTAMP stDeleteTime;
	/* ayang: 2008-03-13 Fix SCR#26102 */
	ia_byte bMessageLength;
}MESSAGE;


typedef struct
{
	ia_word16    wMessageLength;
	ia_byte      *pbMessage;
} CA_EMM_PAYLOAD_USER_DATA_LOCAL_T;

void  IRD_MSG_Initialise(ia_word16 wMaxNumGroups);
void IRD_MSG_Parser(ia_byte* pbMsg, ia_word16 wServicehandle);
void  IRD_MSG_DecoderId(ia_byte *pbMiscOp);
void IRD_MSG_ParseDecoderMsg( const CA_SCELL_MSG *pstSCellMsg );
void CA_IRD_MemoryFree(void);

ia_byte* IRD_MSG_GetMailAddress(void);
MESSAGE* IRD_MSG_GetMailMsg( ia_byte bMailIndex );
MESSAGE* IRD_MSG_GetAnnounceMsg( ia_byte bAnnIndex );

CA_QUERY_STATUS_EN IRD_MSG_EmmDownloadQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN IRD_MSG_EmmChangePinQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN IRD_MSG_DecRecQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN IRD_MSG_AttributedDisplay( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN IRD_MSG_AttributedDisplay_2( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN IRD_MSG_EmmUserPayloadQueryControl( ia_word32 wHandle, void* pCAClientMsg );

#endif /* _IRD_MSG_H_ */

