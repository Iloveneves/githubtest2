/**
 * $Id: ca_mail.h,v 1.5 2007/11/20 05:15:31 ayang Exp $
*
* Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
*
* Header file for the mail query control.
**/

#ifndef _CA_MAIL_H_
#define _CA_MAIL_H_

void CA_MAIL_Initialise(void);

CA_QUERY_STATUS_EN CA_MAIL_MailQueryControl ( ia_word32 wHandle, void* pCAClientMsg );
CA_QUERY_STATUS_EN CA_MAIL_AnnounceQueryControl ( ia_word32 wHandle, void* pCAClientMsg );
void CA_MAIL_Notify(ia_word32 wGrpHandle, ia_byte mailIndex);
void CA_MAIL_NotifyAnnounce(ia_word32 wGrpHandle, ia_byte annIndex);

#endif /* _CA_MAIL_H_ */

