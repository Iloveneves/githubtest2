/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/10/14                                           	  */
/******************************************************************************/
typedef enum FYF_DESCR_KeyParity_s
{
    FYF_DESCR_KEY_PARITY_EVEN_PARITY,
    FYF_DESCR_KEY_PARITY_ODD_PARITY,
    FYF_DESCR_KEY_PARITY_GENERIC_KEY
} FYF_DESCR_KeyParity_t;
typedef enum FYF_DESCR_KeyUsage_s
{
    FYF_DESCR_KEY_USAGE_INVALID,
    FYF_DESCR_KEY_USAGE_VALID_FOR_PES,
    FYF_DESCR_KEY_USAGE_VALID_FOR_TRANSPORT,
    FYF_DESCR_KEY_USAGE_VALID_FOR_ALL
} FYF_DESCR_KeyUsage_t;  
typedef struct
{ 
	BU32 (*FYF_CPI_descr_open)(void);
	BU32 (*FYF_CPI_descr_associate_pid)(BU32 handle,BU32 pid);
	BU32 (*FYF_CPI_descr_close)(BU32 handle); 
	BU32 (*FYF_CPI_descr_disassociate_pid)(BU32 handle,BU32 pid); 
	BU32 (*FYF_CPI_descr_set)(BU32 handle, FYF_DESCR_KeyParity_t Parity, FYF_DESCR_KeyUsage_t Usage, BU08 *Data_p);        
}FYF_DESCR_PORTING; 
/******************************************************************************/
/*Description: register descramble                                            */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_descr_register(FYF_DESCR_PORTING *porting);
/******************************************************************************/
/*Description: descramble open                                                */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_API_descr_open(void);   
/******************************************************************************/
/*Description: associate pid                                                  */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_API_descr_associate_pid(BU32 handle,BU32 pid); 
/******************************************************************************/
/*Description: descramble close                                               */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_API_descr_close(BU32 handle);
/******************************************************************************/
/*Description: disassociate pid                                               */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_API_descr_disassociate_pid(BU32 handle,BU32 pid);
/******************************************************************************/
/*Description: descramble set                                                 */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_API_descr_set(BU32 handle, FYF_DESCR_KeyParity_t Parity, FYF_DESCR_KeyUsage_t Usage, BU08 *Data_p);



