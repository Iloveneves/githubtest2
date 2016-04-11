#include "CDCAS30.h"
#include "fyf_com.h"
#define DEBUG_CDCA //printf
static BU32 FYF_CDCA_semaphore=0;
static BU32 FYF_CDCA_card_vaild = 0;
static BU32 FYF_CDCA_card_in = 0;
static BU08 ca_enable = FYF_TRUE;

extern BU32 semaphoreEcm;

BU08 CA_GetEnable(void)
{
	return ca_enable;
}

void CA_SetEnable(BU08 flag)
{
	ca_enable = flag;
}

/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
CDCA_BOOL CDSTBCA_SCReset(CDCA_U8* pbyATR,  CDCA_U8* pbyLen)
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
		DEBUG_CDCA("CDSTBCA_SCReset End \n");
		DEBUG_CDCA("GetATR Count:%d\n", *pbyLen);

	    for (i = 0; i < *pbyLen; i++)
	    {
	        DEBUG_CDCA("ATRBuf[%d]:%#x\n", i, pbyATR[i]);
	    }
		return CDCA_TRUE;
	}
	else
	{
		DEBUG_CDCA("CDSTBCA_SCReset fail \n");
		return CDCA_FALSE;
	}		
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
CDCA_BOOL CDSTBCA_SCPBRun( const CDCA_U8* pbyCommand, CDCA_U16 wCommandLen,  CDCA_U8* pbyReply,  CDCA_U16* pwReplyLen)
{	
	BS32 ReturnResult;
	BU16 NumReaded=0;
	BU08 reponse[258];
	BU32 tryTimes,i;

	DEBUG_CDCA("CDSTBCA_SCPBRun Enter \n");
	*pwReplyLen = 0;
	tryTimes = 0;
	do
	{
		if(FYF_API_smc_card_exist()==FYF_ERR)
		{
			return CDCA_FALSE;
		}
		ReturnResult = FYF_API_smc_iso_transfer(pbyCommand,wCommandLen, reponse,258, &NumReaded);
		tryTimes++;
	}while(tryTimes < 3 && ReturnResult != 0);

	if(ReturnResult == 0)
	{
		if(((reponse[NumReaded - 2] & 0xf0) != 0x60) && ((reponse[NumReaded - 2] & 0xf0) != 0x90))
		{
			printf("CDSTBCA_SCPBRun fail 0x6x 0x9x\n");
			return CDCA_FALSE;
		}
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

/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CD_smc_thread(void *pvParam)
{
	CDCA_BOOL ret = CDCA_FALSE;
	while(1)
	{
		FYF_API_thread_sleep(20);
		
		if (FYF_CDCA_card_in == 0 && FYF_OK == FYF_API_smc_card_exist())
		{
			FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
			ret = CDCASTB_SCInsert(); 
			if (ret != CDCA_TRUE)
			{
				ret = CDCASTB_SCInsert(); 
			}
			if (ret == CDCA_TRUE)
			{
				FYF_CDCA_card_vaild = 1;
			}

			DEBUG_CDCA("smc insert!!!!!!!!\n");
			FYF_CDCA_card_in = 1;
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);
			
			FYF_API_semaphore_release (semaphoreEcm);
		}
		else if (FYF_CDCA_card_in == 1 && FYF_ERR == FYF_API_smc_card_exist())
		{
			FYF_CDCA_card_in = 0;
			FYF_CDCA_card_vaild = 0;
			CDCASTB_SCRemove();
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
			CA_SetInvaildCW();
		}
		cas_dmx_timeout_check();
	}

#if 0
	static BS32 bCardInLast = FYF_ERR;
	BS32 bCardIn;
	static BU08 bCardCHANGE = 1;

	while  (!CA_GetEnable())
	{
		continue;
	}
	
	bCardIn=FYF_API_smc_card_exist();
	bCardInLast=bCardIn;
	if(bCardIn==FYF_ERR)
	{
		DEBUG_CDCA("smc removed!\n");
		CDCASTB_SCRemove ();
		FYF_CDCA_card_in=0;
		//FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
	}
	else
	{
		DEBUG_CDCA("smc insert!\n");
		FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
//      FYF_CDCA_card_in = 1;	
		ret = CDCASTB_SCInsert();
		if (ret != CDCA_TRUE)
		{
			ret = CDCASTB_SCInsert(); 
		}
	  if(ret == CDCA_TRUE)
	  	FYF_CDCA_card_in = 1;	
		//FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);
		FYF_API_semaphore_release (semaphoreEcm);
	}
	
	while(1)
	{
		FYF_API_thread_sleep(20);

		if (!CA_GetEnable())
		{
			bCardCHANGE = 0;
			continue;
		}

		if (bCardCHANGE == 0)
		{
			bCardCHANGE = 1;

			bCardIn = FYF_API_smc_card_exist();
			
			bCardInLast = bCardIn;

			if (FYF_OK == bCardIn)
			{
				FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
				if (CDCASTB_SCInsert() != 1)
				{
					CDCASTB_SCInsert(); 
				}
				FYF_CDCA_card_in = 1;
				FYF_API_semaphore_release (semaphoreEcm);
			}
			else
			{
				FYF_CDCA_card_in = 0;
				CDCASTB_SCRemove();
			}
		}
	    bCardIn=FYF_API_smc_card_exist();
		if (bCardIn != bCardInLast)
	    {
	        bCardInLast = bCardIn;
	        if (FYF_OK == bCardIn)
	        {
	            DEBUG_CDCA("smc insert!!!!!!!!\n");
	    //        CDCASTB_SCRemove();
				FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
				ret = CDCASTB_SCInsert(); 
				 
				if (ret != CDCA_TRUE)
				{
					//CDCASTB_SCRemove();
					ret = CDCASTB_SCInsert(); 
				}
				if(ret == CDCA_TRUE)
	  				FYF_CDCA_card_in = 1;
				FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);
				FYF_API_semaphore_release (semaphoreEcm);
	        }
	        else
	        {
	            DEBUG_CDCA("smc removed!\n");
	            CDCASTB_SCRemove ();
				FYF_CDCA_card_in=0;
				FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
				CA_SetInvaildCW();
	        }
	    } 	
		cas_dmx_timeout_check();

	}
#endif
}

BU32 FYF_CA_IPI_GetCardIn(void)
{
	return FYF_CDCA_card_vaild;
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
BS32 FYF_CD_SmartCard_Initialise()
{
	BU32 ret;
	FYF_API_smc_open();
	/* create smart card insert/remove notify semaphore */
	FYF_CDCA_semaphore= FYF_API_semaphore_create("smc",0,FYF_THREAD_WAIT_PRIO);
	/* open smart card */
	printf("times1 = %d\n", FYF_API_time_ms());
	if (FYF_OK == FYF_API_smc_card_exist())
	{
		ret = CDCASTB_SCInsert();
		if (ret != CDCA_TRUE)
		{
			ret = CDCASTB_SCInsert(); 
		}
		if (ret == CDCA_TRUE)
		{
			FYF_CDCA_card_vaild = 1;
		}
		FYF_CDCA_card_in = 1;
	}
	else
	{
		CDCASTB_SCRemove();
		FYF_CDCA_card_in = 0;
		FYF_CDCA_card_vaild = 0;
	}
	printf("times2 = %d\n", FYF_API_time_ms());
	
	/* create thread deal with smart card insert/remove */
	FYF_API_thread_create("smc",FYF_CD_smc_thread,FYF_NULL, 15, 10 * 1024);	

	return FYF_OK;
}


