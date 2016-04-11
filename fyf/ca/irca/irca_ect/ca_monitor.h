/**
 * $Id: ca_monitor.h,v 1.6 2007/11/20 05:15:31 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* Header file for CA monitor query control.
**/

#ifndef _CA_MONITOR_H_
#define _CA_MONITOR_H_

#include "ca_local.h"

void CA_MONITOR_Initialise(ia_word16 wMaxNumServiceGroups);
void CA_MONITOR_MemoryFree(void);
/* jyou: 3.4.3.7 SCR#25552 2007-06-06 */
void CA_MONITOR_MonitorControl(ia_word32 wIndex, CA_SERVICE_TYPE_EN eSvcType, ia_bool fEnable);
/* jyou: 3.4.3.7 SCR#25552 2007-06-06 end*/
void CA_MONITOR_ParseEmmMonitorMsg(const CA_SCELL_MSG *pstSCellMsg);
void CA_MONITOR_ParseEcmMonitorMsg(const CA_SCELL_MSG *pstSCellMsg);
void CA_MONITOR_EmmNotify(ia_word32 wEmmHandle);
void CA_MONITOR_EcmNotify(ia_word32 wCaClientHandle );
CA_QUERY_STATUS_EN CA_MONITOR_MonEcmQueryControl( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_MONITOR_MonEmmQueryControl( ia_word32 wHandle, void* pCAClientMsg );

#endif /* _CA_MONITOR_H_ */

