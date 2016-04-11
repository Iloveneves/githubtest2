/**
* $Id: ca_surflock.h,v 1.5 2007/11/20 05:15:31 ayang Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the surflock query control.
**/

#ifndef _CA_SURFLOCK_H_
#define _CA_SURFLOCK_H_

#include "ca_local.h"

ia_result CA_SURFLOCK_Initialise( void );
CA_QUERY_STATUS_EN CA_SURFLOCK_SendQuery( ia_word32 wSmartCardId );

CA_QUERY_STATUS_EN CA_SURFLOCK_QueryControl( ia_word32 wHandle, void* pCAClientMsg );
void CA_SURFLOCK_ReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
ia_result CA_TASK_GetSurflockStatus(ia_word32 wSmartCardId, ia_bool *pfSurfLocked);
void CA_TASK_SetLockedService(ia_word32 wHandle, ia_word32 wSmartCardId);

/* Check whether all the smart cards are surf locked */
ia_bool CA_SURFLOCK_AllCardSurfLocked( void );


#endif /* _CA_SURFLOCK_H_ */

