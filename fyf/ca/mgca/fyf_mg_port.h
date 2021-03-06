/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_STB_Serial(MG_U8* MG_STBSerials);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 FYF_MG_Get_Card_Ready(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Lock_Status(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 FYF_MG_Get_PinErrCount(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Partner_Status(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Card_SN(MG_U8 *MG_CardSN);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Card_Version(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_CAS_ID(MG_U8 *MG_CASID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Card_Issue_Date(MG_U8 *MG_IssueDate);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Card_Expire_Date(MG_U8 *MG_ExpireDate);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Group_ID(MG_U8 *MG_GID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID          FYF_MG_Get_MSCard_Info(MG_U8 * MG_MSCardInfo);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16 		FYF_MG_Get_Card_Key_Version(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16		FYF_MG_Get_UCounter(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16		FYF_MG_Get_GCounter(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16		FYF_MG_Get_ACounter(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_S32 		FYF_MG_Get_Card_Balance(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16 		FYF_MG_Get_Card_OverDraft(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Current_Program(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Program_Type(MG_U8 MG_PVName);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16 		FYF_MG_Get_Program_Price(MG_U8 MG_PVName);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8*		FYF_MG_Get_Command_Data(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Odd_CW(MG_U8 *MG_OCW);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Even_CW(MG_U8 *MG_ECW);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Set_CurTeleviewPPVTag(MG_U8 MG_State);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_TeleviwPPVTag(MG_U8 MG_PVName);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Clean_LastPNo_Info(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8*		FYF_MG_Get_Mail_Head(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8*		FYF_MG_Get_Mail_Body(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS	FYF_MG_Check_Card( MG_U8* MG_ATR);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Info_CardRemoved( MG_VOID );
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Init_CAS(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U32 		FYF_MG_Get_ChargeValue(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID 		FYF_MG_Get_Card_ID(MG_U8 * MG_CardID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8 		FYF_MG_Get_Program_Auth(MG_U8 MG_PVName);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8* 		FYF_MG_Get_LoaderInfo( MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U16            FYF_MG_Get_CASoft_Version(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID          FYF_MG_Get_ServiceID(MG_U8 * MG_ServiceID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_U8             FYF_MG_Get_ZoneCode(MG_VOID);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_VOID           FYF_MG_Get_EigenValue(MG_U8 *MG_Evalue);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_System_Register(MG_U8 *MG_STBSerials/*, MG_U8 *RetryTimes*/);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_E_Process(MG_U8 *MG_EMM_E, MG_U8 *MG_pAKUpdOver);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_U_Process(MG_U8 *MG_EMM_U, MG_U8 *MG_pEMMResult);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_G_Process(MG_U8 *MG_EMM_G, MG_U8 *MG_pEMMResult);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_A_Process(MG_U8 *MG_EMM_A, MG_U8 *MG_pEMMResult);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_GA_Process(MG_U8 *MG_EMM_C, MG_U8 *MG_pEMMResult);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_EMM_C_Process(MG_U8 *MG_EMM_C, MG_U8 *MG_pEMMResult);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_ECM_Process(MG_U8 *MG_ECM, MG_U8 *MG_pCWUpd, MG_U8 *MG_pBalUpd,MG_U8 MG_ZoneCode);
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS FYF_MG_CAS_Update_GK(MG_VOID);

