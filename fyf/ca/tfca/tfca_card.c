#include "cas21.h"
#include "fyf_com.h"
#define DEBUG_CDCA //printf
static BU32 FYF_CDCA_semaphore=0;
static BU32 FYF_CDCA_card_in = 0;
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
CDCA_BOOL TFSTBCA_SCReset(CDCA_U8* pbyATR,  CDCA_U8* pbyLen)
{
	BS32 ret;
	BU16 atr_size;int i;
	
	DEBUG_CDCA("CDSTBCA_SCReset Enter \n");
	atr_size = *pbyLen;
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+24, 0, 0);
	ret = FYF_API_smc_reset (pbyATR, &atr_size);
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+25, 0, 0);
	if(0 == ret)
	{	
		*pbyLen = (CDCA_U8)atr_size;
		DEBUG_CDCA("TFSTBCA_SCReset End \n");
		DEBUG_CDCA("GetATR Count:%d\n", *pbyLen);

    for (i = 0; i < *pbyLen; i++)
    {
        DEBUG_CDCA("ATRBuf[%d]:%#x\n", i, pbyATR[i]);
    }
		return CDCA_TRUE;
	}
	else
	{
		DEBUG_CDCA("TFSTBCA_SCReset fail \n");
		return CDCA_FALSE;
	}		
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
CDCA_BOOL TFSTBCA_SCPBRun( const CDCA_U8* pbyCommand, CDCA_U16 wCommandLen,  CDCA_U8* pbyReply,  CDCA_U16* pwReplyLen)
{	
	BS32 ReturnResult;
	BU16 NumReaded=0;
	BU08 reponse[258];
	BU32 tryTimes,i;

	DEBUG_CDCA("CDSTBCA_SCPBRun Enter \n");
	tryTimes = 0;
	do
	{
		ReturnResult = FYF_API_smc_iso_transfer(pbyCommand,wCommandLen, reponse,258, &NumReaded);
		tryTimes++;
	}while(tryTimes < 3 && ReturnResult != 0);

	if(ReturnResult == 0)
	{
		FYF_API_Memcpy (pbyReply, reponse, NumReaded);
		#if 0
		DEBUG_CDCA("TFSTBCA_SCPBRun NumReaded = %d\n",NumReaded);
		for(i=0;i<NumReaded;i++)
		{
			DEBUG_CDCA("0x%x ",reponse[i]);
			if(i%4==0)
			{
				DEBUG_CDCA("\n");
			}
		}
		DEBUG_CDCA("\n");
		#endif
		*pwReplyLen = NumReaded;
		DEBUG_CDCA("CDSTBCA_SCPBRun End \n");
		return CDCA_TRUE;
	}
	else
	{
		DEBUG_CDCA("CDSTBCA_SCPBRun fail \n");
		return CDCA_FALSE;
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
static void FYF_CD_SmartCard_EventHandle(BU32 param)
{
	if (param ==0)
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT,0,0);
		FYF_CDCA_card_in=0;
		DEBUG_CDCA("FYF_CDCA_card_in=0\n");
	}
	else if (param ==1)
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN,0,0);
		FYF_CDCA_card_in=1;
		DEBUG_CDCA("FYF_CDCA_card_in=1\n");
	}
	FYF_API_semaphore_release(FYF_CDCA_semaphore);
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CD_smc_thread(void *pvParam)
{
	CDCA_BOOL ret;
	BS32 ret_sem;
	static BS32 bCardInLast = FYF_ERR;
	BS32 bCardIn;
	bCardIn=FYF_API_smc_card_exist();
	bCardInLast=bCardIn;
	if(bCardIn==FYF_ERR)
	{
	  DEBUG_CDCA("smc removed!\n");
	  TFCASTB_SCRemove ();
	  FYF_CDCA_card_in=0;
	  FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
	}
	else
	{
	  DEBUG_CDCA("smc insert!\n");
      FYF_CDCA_card_in = 1;	
	  ret = TFCASTB_SCInsert();
	  if (ret != CDCA_TRUE)
	  {
		TFCASTB_SCRemove();
		TFCASTB_SCInsert(); 
	   }
      FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);
	}
	while(1)
	{
	    usleep(20 * 1000);
	    bCardIn=FYF_API_smc_card_exist();
		if (bCardIn != bCardInLast)
	    {
	        bCardInLast = bCardIn;
	        if (FYF_OK == bCardIn)
	        {
	            DEBUG_CDCA("smc insert!!!!!!!!\n");
                FYF_CDCA_card_in = 1;
	            TFCASTB_SCRemove();
		
				ret = TFCASTB_SCInsert(); 
				 
				if (ret != CDCA_TRUE)
				{
					TFCASTB_SCRemove();
					ret = TFCASTB_SCInsert(); 
				}
				FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);
	        }
	        else
	        {
	            DEBUG_CDCA("smc removed!\n");
	            TFCASTB_SCRemove ();
				FYF_CDCA_card_in=0;
				FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
				CA_SetInvaildCW();
	        }
	    } 	
		cas_dmx_timeout_check();

		#if 0
		ret_sem = FYF_API_semaphore_wait(FYF_CDCA_semaphore,1000);
		if(FYF_OS_QUEUE_SEM_STATUS_AVAILABLE == ret_sem)
		{
			if(FYF_CDCA_card_in)
			{
				TFCASTB_SCRemove();
		
				ret = TFCASTB_SCInsert(); 

				if (ret != CDCA_TRUE)
				{
					TFCASTB_SCRemove();
					ret = TFCASTB_SCInsert(); 
				}
				if(CDCA_TRUE == ret)
				{
					DEBUG_CDCA ("插卡处理成功!\n");
					FYF_CDCA_card_in = 1;
					FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);
				}
				else
				{
					TFCASTB_SCRemove();
					FYF_CDCA_card_in=0;
					FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
					DEBUG_CDCA ("插卡处理失败!\n");
				}
			}
			else
			{
				TFCASTB_SCRemove ();
				FYF_CDCA_card_in=0;
				FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
			}
		}
		//cas_dmx_timeout_check ();
		#endif
//		CDSTBCA_RequestFeeding_Check ();
	}
}

BU32 FYF_CA_IPI_GetCardIn(void)
{
	return FYF_CDCA_card_in;
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
BS32 FYF_CD_SmartCard_Initialise()
{
	FYF_API_smc_open();
	/* create smart card insert/remove notify semaphore */
	FYF_CDCA_semaphore= FYF_API_semaphore_create("smc",0,FYF_THREAD_WAIT_PRIO);
	/* create thread deal with smart card insert/remove */
	FYF_API_thread_create("smc",FYF_CD_smc_thread,FYF_NULL, 15, 10 * 1024);	
	/* open smart card */
	//ret=FYF_API_smc_open(FYF_CD_SmartCard_EventHandle);
	
	return FYF_OK;
}


