/****************************************************************************/
/*                                                                          */
/*  Copyright (c) 2001-2004 Shanghai Advanced Digital Technology Co., Ltd   */
/*                                                                          */
/* PROPRIETARY RIGHTS of Shanghai Advanced Digital Technology Co., Ltd      */
/* are involved in the subject matter of this material. All manufacturing,  */
/* reproduction, use, and sales rights pertaining to this subject matter    */
/* are governed by the license agreement. The recipient of this software    */
/* implicitly accepts the terms of the license.                             */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* FILE NAME                                                                */
/*      mgdef.h                                                             */
/*                                                                          */
/* VERSION                                                                  */
/*		v2.6                                                                */
/*                                                                          */
/* AUTHOR                                                                   */
/*      Gaston Shi                                                          */
/*                                                                          */
/****************************************************************************/


#ifndef __MG_DEF_H
#define __MG_DEF_H


typedef unsigned char		MG_U8;
typedef unsigned short		MG_U16;
typedef unsigned int		MG_U32;
typedef signed char			MG_S8;
typedef signed short		MG_S16;
typedef signed int			MG_S32;
typedef void				MG_VOID;
				
#define MG_TRUE				1
#define MG_FALSE			0

#define MGCA_SC_OUT	0
#define MGCA_SC_IN	1
#define MGCA_SC_ERROR	2
#define MGCA_SC_NOT_RESET 3

#define MG_BIT_PEC_INVALID							0x80
#define MG_BIT_CARD_PARTNER						0x20
#define MG_BIT_STB_PARTNER							0x10

typedef enum
{
       MG_ERR_SCARD_NOT_ACT=-60,   /*the son card is not activated in mother son card fuction 20071218 eileen*/
	MG_ERR_SCARD_DISABLED,         /*the leavetimelength<0 in the son card 20071218 eileen*/
	MG_ERR_ZONE_CODE,                  /*the zone code is not compatible with networkid in NIT 20080328 eileen*/
	MG_ERR_NOT_RECEIVED_SPID   =-50,
	MG_ERR_AUTHENTICATION,
	MG_ERR_NO_MONEY			=-40,
	MG_ERR_NOT_AUTHORED,
	MG_ERR_CARD_LOCKED,
	MG_ERR_APP_LOCKED,              /* 2005.9.15 yfh */
	MG_ERR_SC_PARTNER,
	MG_ERR_PIN_AUTHEN,				 /* 2005.9.14 yfh */	
	MG_ERR_CARD_NOT_READY,     /* 2005.9.14 yfh */	
	MG_ERR_EXTER_AUTHEN,
	MG_ERR_INTER_AUTHEN,
	MG_ERR_GET_CARD_INFO,
	MG_ERR_GET_CARD_DATA,
	MG_ERR_CALCU_GKAK,
	MG_ERR_NEED_GKUPD		=-20,
	MG_ERR_NOREG_CARD,
	MG_ERR_ILLEGAL_CARD,
	MG_ERR_DONOT_TELEVIEW_PPV,
	MG_ERR_QUESTION_TELEVIEW_PPV,
	MG_ERR_CARD_NOTFOUND,		/* 2004-07-26 13;15 by sgb */	
	MG_FAILING				=-1,
	MG_SUCCESS				=0,
	MG_EMM_APP_STOP			=1,
	MG_EMM_APP_RESUME,
	MG_EMM_PROG_UNAUTH,
	MG_EMM_PROG_AUTH,
	MG_EMM_EN_PARTNER,
	MG_EMM_DIS_PARTNER,
	MG_EMM_UPDATE_GNO,
	MG_EMM_CHILD_UNLOCK,
	MG_EMM_MSG,
	MG_EMM_UPDATE_GK,
	MG_EMM_EMAIL,
	MG_EMM_CHARGE,
	MG_EMM_LOADER,
	MG_EMM_FIGEIDENT,     /*finger identify 070905 eileen*/
	MG_EMM_SONCARD,       /*set son card command 071218 eileen*/
	MG_EMM_URGENT_SERVICE,  /*urgent service fuction 071220 eileen*/
	MG_EMM_MODIFY_ZONECODE  /*modify zone code in card 20080409 eileen*/
}MG_STATUS;

/* MG_API */
extern MG_VOID 		MG_Get_STB_Serial(MG_U8* MG_STBSerials);
extern MG_U8 MG_Get_Card_Ready(MG_VOID);         /*2005.9.14 yfh*/
extern MG_U8 		MG_Get_Lock_Status(MG_VOID);
extern MG_U8 MG_Get_PinErrCount(MG_VOID);   /*2005.9.15 yfh*/
extern MG_U8 		MG_Get_Partner_Status(MG_VOID);
extern MG_VOID 		MG_Get_Card_SN(MG_U8 *MG_CardSN);
extern MG_U8 		MG_Get_Card_Version(MG_VOID);
extern MG_VOID 		MG_Get_CAS_ID(MG_U8 *MG_CASID);
/*extern MG_U8 		MG_Get_CAS_Version(MG_VOID);*//*20080424 eileen*/
extern MG_VOID 		MG_Get_Card_Issue_Date(MG_U8 *MG_IssueDate);
extern MG_VOID 		MG_Get_Card_Expire_Date(MG_U8 *MG_ExpireDate);
extern MG_VOID 		MG_Get_Group_ID(MG_U8 *MG_GID);
/*extern MG_VOID 		MG_Get_Card_Provider(MG_U8 *MG_ProviderID);*/
extern MG_VOID           MG_Get_MSCard_Info(MG_U8 * MG_MSCardInfo);/*get mother or son card info 20071225 eileen*/
/*extern MG_VOID			MG_Get_Card_ZoneID(MG_U8 * MG_ZoneID );*/
extern MG_U16 		MG_Get_Card_Key_Version(MG_VOID);
extern MG_U16		MG_Get_UCounter(MG_VOID);
extern MG_U16		MG_Get_GCounter(MG_VOID);
extern MG_U16		MG_Get_ACounter(MG_VOID);
extern MG_S32 		MG_Get_Card_Balance(MG_VOID);
extern MG_U16 		MG_Get_Card_OverDraft(MG_VOID);
extern MG_U8 		MG_Get_Current_Program(MG_VOID);
extern MG_U8 		MG_Get_Program_Type(MG_U8 MG_PVName);
extern MG_U16 		MG_Get_Program_Price(MG_U8 MG_PVName);
extern MG_U8*		MG_Get_Command_Data(MG_VOID);
extern MG_VOID 		MG_Get_Odd_CW(MG_U8 *MG_OCW);
extern MG_VOID 		MG_Get_Even_CW(MG_U8 *MG_ECW);
extern MG_VOID 		MG_Set_CurTeleviewPPVTag(MG_U8 MG_State);
extern MG_U8 		MG_Get_TeleviwPPVTag(MG_U8 MG_PVName);
extern MG_VOID 		MG_Clean_LastPNo_Info(MG_VOID);
extern MG_U8*		MG_Get_Mail_Head(MG_VOID);
extern MG_U8*		MG_Get_Mail_Body(MG_VOID);
extern MG_STATUS	MG_Check_Card( MG_U8* MG_ATR);
extern MG_VOID 		MG_Info_CardRemoved( MG_VOID );
extern MG_VOID 		MG_Init_CAS(MG_VOID);
extern MG_U32 		MG_Get_ChargeValue(MG_VOID);
extern MG_VOID 		MG_Get_Card_ID(MG_U8 * MG_CardID);
extern MG_U8 		MG_Get_Program_Auth(MG_U8 MG_PVName);
extern MG_U8* 		MG_Get_LoaderInfo( MG_VOID);
extern MG_U16             MG_Get_CASoft_Version(MG_VOID);/*get mediagate soft version 20071224 eileen*/
/*urgent service 20071220 eileen*/
extern MG_VOID           MG_Get_ServiceID(MG_U8 * MG_ServiceID);

extern MG_U8               MG_Get_ZoneCode(MG_VOID);/*get zonecode in card 20080424 eileen*/
extern MG_VOID           MG_Get_EigenValue(MG_U8 *MG_Evalue);/*20080418 eileen*/

/* MG_CAS */
extern MG_STATUS MG_CAS_System_Register(MG_U8 *MG_STBSerials/*, MG_U8 *RetryTimes*/);
extern MG_STATUS MG_CAS_EMM_E_Process(MG_U8 *MG_EMM_E, MG_U8 *MG_pAKUpdOver);
extern MG_STATUS MG_CAS_EMM_U_Process(MG_U8 *MG_EMM_U, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_EMM_G_Process(MG_U8 *MG_EMM_G, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_EMM_A_Process(MG_U8 *MG_EMM_A, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_EMM_GA_Process(MG_U8 *MG_EMM_C, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_EMM_C_Process(MG_U8 *MG_EMM_C, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_ECM_Process(MG_U8 *MG_ECM, MG_U8 *MG_pCWUpd, MG_U8 *MG_pBalUpd,MG_U8 MG_ZoneCode);/*20080328 eileen*/
extern MG_STATUS MG_CAS_Update_GK(MG_VOID);

#define MG_System_Register		MG_CAS_System_Register
#define MG_ECM_Process			MG_CAS_ECM_Process
#define MG_EMM_C_Process		MG_CAS_EMM_C_Process
#define MG_EMM_GA_Process		MG_CAS_EMM_GA_Process
#define MG_EMM_E_Process		MG_CAS_EMM_E_Process
#define MG_EMM_U_Process		MG_CAS_EMM_U_Process
#define MG_EMM_G_Process		MG_CAS_EMM_G_Process
#define MG_EMM_A_Process		MG_CAS_EMM_A_Process
#define MG_Update_GK			MG_CAS_Update_GK


/* Gaston */
void MG_CA_Task(void *pvParam);
int MG_CAS_Init(void);

#endif

