/**
 * $Id: ca_error.h,v 1.8 2008/05/09 08:55:08 ayang Exp $
 *
 * Copyright (c) 2004 Irdeto Access B.V., All rights reserved.
 *
 * Header file for the mapping of error code shown on the screen banner.
**/

#ifndef _CA_ERROR_H_
#define _CA_ERROR_H_

#define IA_FIRST_STATUS_VALUE	00

#define CA_IRDETO_ERROR_D00		00
#define CA_IRDETO_ERROR_D22		22
#define CA_IRDETO_ERROR_D23		23
#define CA_IRDETO_ERROR_D25		25
#define CA_IRDETO_ERROR_D26		26
#define CA_IRDETO_ERROR_D29		29
#define CA_IRDETO_ERROR_D100	100
#define CA_IRDETO_ERROR_D101	101
#define CA_IRDETO_ERROR_D102	102
#define CA_IRDETO_ERROR_D103	103
#define CA_IRDETO_ERROR_D104	104
#define CA_IRDETO_ERROR_D105	105

#define CA_IRDETO_ERROR_I07		7
#define CA_IRDETO_ERROR_I102	102

#define CA_IRDETO_ERROR_E04		4
#define CA_IRDETO_ERROR_E05		5
#define CA_IRDETO_ERROR_E06		6
#define CA_IRDETO_ERROR_E16		16
#define CA_IRDETO_ERROR_E17		17
#define CA_IRDETO_ERROR_E18		18
#define CA_IRDETO_ERROR_E19		19
#define CA_IRDETO_ERROR_E20		20
#define CA_IRDETO_ERROR_E21		21
#define CA_IRDETO_ERROR_E24		24
#define CA_IRDETO_ERROR_E27		27
#define CA_IRDETO_ERROR_E28		28
#define CA_IRDETO_ERROR_E30		30
#define CA_IRDETO_ERROR_E33		33
#define CA_IRDETO_ERROR_E34		34
#define CA_IRDETO_ERROR_E100	100
#define CA_IRDETO_ERROR_E101	101
#define CA_IRDETO_ERROR_E102	102
#define CA_IRDETO_ERROR_E103	103
#define CA_IRDETO_ERROR_E104	104
#define CA_IRDETO_ERROR_E105	105

#define CA_IRDETO_ERROR_F101	101
#define CA_IRDETO_ERROR_F102	102

#define CA_IRDETO_ERROR_E66	66   /* Product Blocking */

/* Status values for SOURCE_MOD_ECM_SOURCE. */
enum
{
	/* Module level statuses: */
	D100_MOD_ECM_SOURCE_OK = IA_FIRST_STATUS_VALUE,	/* Everything is OK. */
	D101_MOD_ECM_SOURCE_NO_PMT,							/* There is no PMT to parse. */
	D102_MOD_ECM_SOURCE_NO_VENDOR_ID, 				/* There is no vendor ID (card not in). */
	/* E103 was removed from use. */
	/* stream level statuses: */
	D34_MOD_ECM_SOURCE_STATUS_UNKNOWN = 34,		/* The scrambling status was not determined yet. */
	/* E104 not determined by this module. */
	D105_MOD_ECM_SOURCE_FTA = 105,						/* The stream is not scrambled. */
	E106_MOD_ECM_SOURCE_WRONG_VENDOR 				/* The stream is scrambled, but not for this vendor ID. */
};

void CA_ERROR_Initialise(ia_word16 wMaxNumServiceGroups);
void CA_ERROR_Reset(ia_word32 wIndex, ia_word32 wSvcType);
void CA_ERROR_MemoryFree(void);
void CA_ERROR_UpdateErrorStatus(ia_word32 wIndex, ia_word32 wSrvType, ia_status_st *pstStatus);
CA_QUERY_STATUS_EN CA_ERROR_InfoQueryControl( ia_word32 wHandle, void *pCAClientMsg );
void CA_ERROR_Notify( CA_ERROR_SOURCE_EN eSource, ia_word16 wError );

#endif

