/**
 * $Id: ca_ppt.h,v 1.3 2007/11/20 05:15:31 ayang Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the ppt query control.
**/

#ifndef _CA_PPT_H_
#define _CA_PPT_H_

#include "ca_local.h"

void CA_PPT_Initialise( ia_word32 wResourceId );
void CA_PPT_Freememory(void);

CA_QUERY_STATUS_EN CA_PPT_InfoQueryControl( ia_word32 wHandle, void* pCAClientMsg );
void CA_PPT_INFOReplyMsg( const CA_SCELL_MSG *pstSCellMsg );
CA_QUERY_STATUS_EN CA_PPT_InfoSendQuery( ia_word32 wResourceId, ia_byte bSectorIndex );
void CA_PPT_SetSectorBySectorIndex(ia_word32 wResourceId, ia_byte bSector);


#endif /* CA_PPT_H */

