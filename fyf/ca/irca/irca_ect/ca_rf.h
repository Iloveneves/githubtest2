/**
 * $Id: ca_rf.h,v 1.2 2007/11/20 05:15:31 ayang Exp $
*
* Copyright (c) 2001-2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the regional filter query control.
**/

#ifndef _CA_RF_H_
#define _CA_RF_H_

CA_QUERY_STATUS_EN CA_RF_ConfigQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_RF_SetQueryControl( ia_word32 wHandle, void* pCAClientMsg );

#endif /* CA_RF_H */


