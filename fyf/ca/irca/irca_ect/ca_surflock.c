/**
* $Id: ca_surflock.c,v 1.11 2008/05/09 08:55:08 ayang Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* The surflock module of Embedded CA Task.
**/

/******************************************************************************
* FILE          : ca_surflock.C                                                  *
*                                                                            *
* DESCRIPTION   :
*                                                                            *
* NOTES         :                                                            *
*                                                                            *
* STATUS/HISTORY:                                                            *
* Author          : Melissa Li                                               *
* Status          : $Revision: 1.11 $                                    *
* History         : 														 *
*                                                                            *
******************************************************************************/

/******************************** Header Files ********************************/
/* #include <string.h> */
#include <irca/pubh/s3_ansi_functions.h>
#include <irca/pubh/s3_osdrv.h>
#include <irca/pubh/s3_message_helper.h>
#include <irca/pubh/s3_smartcard.h>
#include <irca/ca_task.h>
#include "ca_surflock.h"
#include "ca_scard.h"
#include "ca_local.h"
#include "ca_error.h"

/* jyou: 3.4.2 removed astCaSurfLock, use g_astLocalSCardInfo instead */
static 	CA_LOCAL_SMARTCARD_INFO_T* m_pstInfo;

/*
* Description: Initializes the SURFLOCK local database.
*
* Parameters : None.
*
* Returns    : None.
*/
ia_result CA_SURFLOCK_Initialise( void )
{
	CA_PARAM_NOT_USED(astrSCellMsgDisplay);	/* to supress warning */
	m_pstInfo = CA_LOCAL_GetLocalSCardInfo();
	return IA_TRUE;
}


CA_QUERY_STATUS_EN CA_SURFLOCK_SendQuery(ia_word32 wResourceId)
{
	msg_smartcard_query_data_st   *pstMsg;

	CA_DBG_V(("<= MSG_SURF_LOCK_QUERY\n"));

	pstMsg = ( msg_smartcard_query_data_st* )ALLOCATE_MEMORY( msg_smartcard_query_data_st, 1);
	if( pstMsg != 0 )
	{
		ansi_memset( pstMsg,0,sizeof( msg_smartcard_query_data_st ));
		pstMsg->wResourceId = wResourceId;

		if( SCELL_Message( MSG_SURF_LOCK_QUERY, 0, sizeof( msg_smartcard_query_data_st ), pstMsg ) != IA_SUCCESS )
		{
			if( pstMsg != 0 )
			{
				FREE_MEMORY( pstMsg );
			}
			CA_ERROR(("Sending failed MSG_SURF_LOCK_QUERY.\n"));
			CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MESSAGE);
			return CA_QUERY_FAIL;
		}
	}
	else
	{
		CA_ERROR(("Allocating memory failed.\n"));
		CA_ERROR_Notify(CA_ERROR_SOURCE_ECT, CA_ERROR_MEMORY);
		return CA_QUERY_FAIL ;
	}

	return CA_QUERY_SUCCESS;
}


CA_QUERY_STATUS_EN CA_SURFLOCK_QueryControl( ia_word32 wHandle, void* pCAClientMsg )
{
	ia_word32 wResourceId = 0;
	ia_byte	i;
	CA_SURFLOCK_T *pCaSurfLock;

	CA_PARAM_NOT_USED( wHandle );
	pCaSurfLock = ( CA_SURFLOCK_T* )pCAClientMsg;
	CA_DBG(("+++ CA SurfLock Query Control Entered.\n"));
	if( pCaSurfLock == 0 )
	{
		CA_ERROR(("+++ Input parameter is null.\n"));
		return CA_QUERY_FAIL ;
	}

	wResourceId = pCaSurfLock->wResourceId;
	CA_DBG(("+++ CA SurfLock Query Control on smart card %d\n", wResourceId));
	m_pstInfo = CA_LOCAL_GetLocalSCardInfo();

	if( pCaSurfLock->bRequestType ==  REQUEST_TYPE_QUERY )
	{
		CA_DBG(("+++ SurfLock Query sent to Softcell.\n"));
		return ( CA_SURFLOCK_SendQuery(wResourceId));
	}
	else
	{
		CA_DBG(("+++ Read SurfLock status.\n"));
		for (i = 0; i < SC_SLOT_MAX; i++)
		{
			if (wResourceId == (m_pstInfo + i)->wResourceID)
			{
				break;
			}
		}
		if (i == SC_SLOT_MAX)
		{
			CA_ERROR(("+++ Can't find smart card.\n"));
			return CA_QUERY_FAIL;
		}

		pCaSurfLock->fSurfLocked = (m_pstInfo + i)->fSurfLocked ;
		CA_DBG(("+++ Surflocking status obtained is %d.\n", pCaSurfLock->fSurfLocked));
	}

	return CA_QUERY_SUCCESS ;
}

void CA_SURFLOCK_ReplyMsg( const CA_SCELL_MSG *pstSCellMsg )
{
	ia_msg_handle					pvMsg;
	msg_surf_lock_reply_st			stSurflock;
	ia_byte i;
	ia_word32	wResourceId;

	ansi_memset((void *)&stSurflock, 0 ,sizeof(stSurflock));

	/*allocate the memeory of message and get surflock message*/
	pvMsg = IA_OpenInputMessage( pstSCellMsg->pvMessage, pstSCellMsg->wLength );
	if (pvMsg == 0)
	{
		CA_ERROR(("[ca_surflock]+++ Can't open reply message.\n"));
		return;
	}
	if (IA_ReadFromMessage( pvMsg, &stSurflock, sizeof( msg_surf_lock_reply_st )) == 0)
	{
		CA_ERROR(("[ca_surflock]+++ Can't read out reply message.\n"));
		IA_CloseMessage(pvMsg, IA_FALSE);
		return;
	}
	CA_DBG_V(("[ca_surflock]+++ SurfLock status on card %d is %d.\n", stSurflock.wResourceId,
		stSurflock.fSurfLocked));

	wResourceId = stSurflock.wResourceId;
	m_pstInfo = CA_LOCAL_GetLocalSCardInfo();

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (wResourceId == (m_pstInfo + i)->wResourceID)
		{
			break;
		}
	}
	if (i == SC_SLOT_MAX)
	{
		CA_ERROR(("[ca_surflock]+++ Can't find smart card %d.\n", wResourceId));
		IA_CloseMessage( pvMsg, IA_FALSE );
		return;
	}

	CA_LOCAL_SetSurflockStatus( i, stSurflock.fSurfLocked );

	/* ECT3.3 enhancement: streamline of wHandle */
	CA_CLIENT_Notify( ( ia_word32 )wResourceId, CA_QC_SURFLOCK, 0);
	CA_DBG_V(("[ca_surflock]+++ Sent CA_QC_SURFLOCK(0x1f) notification.\n"));

	IA_CloseMessage( pvMsg, IA_FALSE );
}

ia_result CA_TASK_GetSurflockStatus(ia_word32 wResourceId, ia_bool *pfSurfLocked)
{
	ia_byte i;

	m_pstInfo = CA_LOCAL_GetLocalSCardInfo();

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (wResourceId == (m_pstInfo + i)->wResourceID)
		{
			break;
		}
	}
	if (i == SC_SLOT_MAX)
	{
		return IA_FAIL;
	}
	*pfSurfLocked = (m_pstInfo + i)->fSurfLocked;

	return IA_SUCCESS;
}

void CA_TASK_SetLockedService( ia_word32 wHandle, ia_word32 wResourceId)
{
	ia_byte i;

	m_pstInfo = CA_LOCAL_GetLocalSCardInfo();

	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		if (wResourceId == (m_pstInfo + i)->wResourceID)
		{
			break;
		}
	}
	if (i == SC_SLOT_MAX)
	{
		return;
	}
	(m_pstInfo + i)->wGroupHandle = wHandle;
}

/*
* CALLED BY: CA_TASK_SendMessage
*/
ia_bool CA_SURFLOCK_AllCardSurfLocked( void )
{
	ia_byte i;
	ia_bool fRet = IA_TRUE;

	CA_DBG(("[ca_surflock]Checking card locked status.\n"));

	m_pstInfo = CA_LOCAL_GetLocalSCardInfo();

	/*CA_LOCAL_SetSurflockStatus(0, IA_TRUE);*/
	/* find the first unsurflocked card and then stop */
	for (i = 0; i < SC_SLOT_MAX; i++)
	{
		CA_DBG(("[ca_surflock]ResourceID 0x%x, Status %d.\n", (m_pstInfo + i)->wResourceID,
			(m_pstInfo + i)->fSurfLocked));
		if ( INVALID_SMARTCARDID == (m_pstInfo + i)->wResourceID
			|| IA_FALSE == (m_pstInfo + i)->fSurfLocked )
		{
			fRet = IA_FALSE;
			break;
		}
	}
	CA_DBG(("[ca_surflock]Return value %d.\n", fRet ));
	return fRet;
}
