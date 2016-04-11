/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/09/23                                           	  */
/******************************************************************************/ 
#ifndef _FYF_SMC_H_
#define _FYF_SMC_H_
typedef struct
{	
	BS32 (*FYF_CPI_smc_open)(void);
	BS32 (*FYF_CPI_smc_card_exist)(void);
	BS32 (*FYF_CPI_smc_reset)(BU08 *buffer, BU16*atr_size);
	BS32 (*FYF_CPI_smc_deactive)(void);
	BS32 (*FYF_CPI_smc_iso_transfer)(BU08* command, BU16 num_to_write, BU08* response, BU16 num_to_read, BU16* actual_size);
}FYF_SMC_PORTING;
/******************************************************************************/
/*Description: register smc                                                   */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_smc_register(FYF_SMC_PORTING *porting);
/******************************************************************************/
/*Description: smc iso transfer                                               */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_smc_open(void);
/******************************************************************************/
/*Description: smc iso transfer                                               */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_smc_card_exist(void);
/******************************************************************************/
/*Description: smc iso transfer 											  */
/*Input 	 :																  */
/*Output	 :																  */
/*Return	 : 0 success, -1 failure										  */
/******************************************************************************/
BS32 FYF_API_smc_reset(BU08 *buffer, BU16*atr_size);
/******************************************************************************/
/*Description: smc iso transfer 											  */
/*Input 	 :																  */
/*Output	 :																  */
/*Return	 : 0 success, -1 failure										  */
/******************************************************************************/
BS32 FYF_API_smc_deactive(void);
/******************************************************************************/
/*Description: smc iso transfer                                               */
/*Input      :   														      */
/*Output     :                                                                */
/*Return     : 0 success, -1 failure                                          */
/******************************************************************************/
BS32 FYF_API_smc_iso_transfer(BU08* command, BU16 num_to_write, BU08* response, BU16 num_to_read, BU16* actual_size);
#endif


