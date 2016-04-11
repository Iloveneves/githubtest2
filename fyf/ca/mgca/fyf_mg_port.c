#include "mgdef.h"
/* MG_API */
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_STB_Serial(MG_U8* MG_STBSerials)
{
	MG_Get_STB_Serial(MG_STBSerials);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 FYF_MG_Get_Card_Ready(MG_VOID)
{
	return MG_Get_Card_Ready();
}         /*2005.9.14 yfh*/
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Lock_Status(MG_VOID)
{
	return	MG_Get_Lock_Status();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 FYF_MG_Get_PinErrCount(MG_VOID)
{
	return MG_Get_PinErrCount();
}   /*2005.9.15 yfh*/
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Partner_Status(MG_VOID)
{
	return MG_Get_Partner_Status();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Card_SN(MG_U8 *MG_CardSN)
{
	MG_Get_Card_SN(MG_CardSN);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Card_Version(MG_VOID)
{
	return MG_Get_Card_Version();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_CAS_ID(MG_U8 *MG_CASID)
{
	MG_Get_CAS_ID(MG_CASID);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Card_Issue_Date(MG_U8 *MG_IssueDate)
{
	MG_Get_Card_Issue_Date(MG_IssueDate);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Card_Expire_Date(MG_U8 *MG_ExpireDate)
{
	MG_Get_Card_Expire_Date(MG_ExpireDate);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Group_ID(MG_U8 *MG_GID)
{
	MG_Get_Group_ID(MG_GID);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID          FYF_MG_Get_MSCard_Info(MG_U8 * MG_MSCardInfo)
{
	MG_Get_MSCard_Info(MG_MSCardInfo);
}/*get mother or son card info 20071225 eileen*/
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16 		FYF_MG_Get_Card_Key_Version(MG_VOID)
{
	return	MG_Get_Card_Key_Version();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16		FYF_MG_Get_UCounter(MG_VOID)
{
	return MG_Get_UCounter();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16		FYF_MG_Get_GCounter(MG_VOID)
{
	return	MG_Get_GCounter();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16		FYF_MG_Get_ACounter(MG_VOID)
{
	return	MG_Get_ACounter();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_S32 		FYF_MG_Get_Card_Balance(MG_VOID)
{
	return	MG_Get_Card_Balance();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16 		FYF_MG_Get_Card_OverDraft(MG_VOID)
{
	return	MG_Get_Card_OverDraft();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Current_Program(MG_VOID)
{
	return	MG_Get_Current_Program();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Program_Type(MG_U8 MG_PVName)
{
	return	MG_Get_Program_Type(MG_PVName);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16 		FYF_MG_Get_Program_Price(MG_U8 MG_PVName)
{
	return MG_Get_Program_Price(MG_PVName);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8*		FYF_MG_Get_Command_Data(MG_VOID)
{
	return MG_Get_Command_Data();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Odd_CW(MG_U8 *MG_OCW)
{
	MG_Get_Odd_CW(MG_OCW);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Even_CW(MG_U8 *MG_ECW)
{
	MG_Get_Even_CW(MG_ECW);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Set_CurTeleviewPPVTag(MG_U8 MG_State)
{
	MG_Set_CurTeleviewPPVTag(MG_State);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_TeleviwPPVTag(MG_U8 MG_PVName)
{
	return MG_Get_TeleviwPPVTag(MG_PVName);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Clean_LastPNo_Info(MG_VOID)
{
	MG_Clean_LastPNo_Info();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8*		FYF_MG_Get_Mail_Head(MG_VOID)
{
	return MG_Get_Mail_Head();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8*		FYF_MG_Get_Mail_Body(MG_VOID)
{
	return MG_Get_Mail_Body();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS	FYF_MG_Check_Card( MG_U8* MG_ATR)
{
	return	MG_Check_Card( MG_ATR);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Info_CardRemoved( MG_VOID )
{
	MG_Info_CardRemoved(  );
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Init_CAS(MG_VOID)
{
	MG_Init_CAS();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U32 		FYF_MG_Get_ChargeValue(MG_VOID)
{
	return		MG_Get_ChargeValue();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Card_ID(MG_U8 * MG_CardID)
{
	MG_Get_Card_ID(MG_CardID);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Program_Auth(MG_U8 MG_PVName)
{
	return	MG_Get_Program_Auth(MG_PVName);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8* 		FYF_MG_Get_LoaderInfo( MG_VOID)
{
	return MG_Get_LoaderInfo( );
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16            FYF_MG_Get_CASoft_Version(MG_VOID)
{
	return   MG_Get_CASoft_Version();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID          FYF_MG_Get_ServiceID(MG_U8 * MG_ServiceID)
{
	MG_Get_ServiceID(MG_ServiceID);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8             FYF_MG_Get_ZoneCode(MG_VOID)
{
	return MG_Get_ZoneCode();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID           FYF_MG_Get_EigenValue(MG_U8 *MG_Evalue)
{
	MG_Get_EigenValue(MG_Evalue);
}
/* MG_CAS */
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_System_Register(MG_U8 *MG_STBSerials/*, MG_U8 *RetryTimes*/)
{
	return MG_CAS_System_Register(MG_STBSerials/*, MG_U8 *RetryTimes*/);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_E_Process(MG_U8 *MG_EMM_E, MG_U8 *MG_pAKUpdOver)
{
	return MG_CAS_EMM_E_Process(MG_EMM_E, MG_pAKUpdOver);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_U_Process(MG_U8 *MG_EMM_U, MG_U8 *MG_pEMMResult)
{
	return MG_CAS_EMM_U_Process(MG_EMM_U, MG_pEMMResult);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_G_Process(MG_U8 *MG_EMM_G, MG_U8 *MG_pEMMResult)
{
	return MG_CAS_EMM_G_Process(MG_EMM_G, MG_pEMMResult);	
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_A_Process(MG_U8 *MG_EMM_A, MG_U8 *MG_pEMMResult)
{
	return MG_CAS_EMM_A_Process(MG_EMM_A, MG_pEMMResult);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_GA_Process(MG_U8 *MG_EMM_C, MG_U8 *MG_pEMMResult)
{
	return MG_CAS_EMM_GA_Process(MG_EMM_C, MG_pEMMResult);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_C_Process(MG_U8 *MG_EMM_C, MG_U8 *MG_pEMMResult)
{
	return MG_CAS_EMM_C_Process(MG_EMM_C, MG_pEMMResult);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_ECM_Process(MG_U8 *MG_ECM, MG_U8 *MG_pCWUpd, MG_U8 *MG_pBalUpd,MG_U8 MG_ZoneCode)
{
	return MG_CAS_ECM_Process(MG_ECM, MG_pCWUpd, MG_pBalUpd,MG_ZoneCode);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_Update_GK(MG_VOID)
{
	return MG_CAS_Update_GK();
}

