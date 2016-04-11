/*===============================================================================

 File Name:  ca_dvt_card.c

 Description:dvt ca smc card

 History:    
===============================================================================*/
/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/

#include "fyf_com.h"
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#define DEBUG_DVTCA //printf

static BU32 FYF_SMC_semaphore=0;
static BU32 FYF_SMC_card_in = 0;
static BU32 FYF_SMC_card_vaild = 0;
static BU32 FYF_SMC_card_in_notify = 0;
extern BU32 semaphoreEcm;
/*---------------------------------
功能：复位智能卡。
返回值：
0：成功。
其他值：失败。
实现说明：参照ISO7816 规范，实现对智能卡的复位和得到ATR。如果正常得到ATR，
返回成功（0），否则返回错误号（非0）。
-------------------------------*/
HRESULT DVTSTBCA_SCReset(void)
{
	BS32 ret;
	BU16 atr_size;int i;
	BU08 pbyATR[33];

//	FYF_API_semaphore_wait(FYF_SMC_semaphore, FYF_WAIT_FOREVER);	
	DEBUG_DVTCA("DVTSTBCA_SCReset Enter \n");
	FYF_API_Memset(pbyATR, 0, 33); 
	atr_size = 0;
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+24, 0, 0);
	ret = FYF_API_smc_reset (pbyATR, &atr_size);
	GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+25, 0, 0);
	if(0 == ret)
	{	
		DEBUG_DVTCA("DVTSTBCA_SCReset End \n");
		DEBUG_DVTCA("GetATR Count:%d\n", atr_size);

	    for (i = 0; i < atr_size; i++)
	    {
	        DEBUG_DVTCA("ATRBuf[%d]:%#x\n", i, pbyATR[i]);
	    }
		if(pbyATR[0] == 0x3B && pbyATR[1] == 0x02)
		{
//			FYF_API_semaphore_release(FYF_SMC_semaphore);
			return DVTCA_OK;
		}
		else 
		{
//			FYF_API_semaphore_release(FYF_SMC_semaphore);
			return DVTCAERR_STB_CARD_INVALID;
		}
	}
	else
	{
		DEBUG_DVTCA("DVTSTBCA_SCReset fail \n");
//		FYF_API_semaphore_release(FYF_SMC_semaphore);
		return DVTCAERR_STB_CARD_INVALID;
	}
}

/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
BU32 FYF_CA_IPI_GetCardIn(void)
{
	return FYF_SMC_card_vaild;
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_IPI_smc_thread(void *pvParam)
{
	HRESULT ret = DVTCAERR_STB_CARD_INVALID;
	while(1)
	{
		FYF_API_thread_sleep(20);
		
		if (FYF_SMC_card_in == 0 && FYF_OK == FYF_API_smc_card_exist())
		{
			FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+24, 0, 0);
			ret = DVTSTBCA_SCReset();
			GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+25, 0, 0);
			if( ret == DVTCA_OK)
			{		
				DEBUG_DVTCA("smc insert!!!!!!!!\n");
				FYF_SMC_card_vaild = 1;
				DVTCASTB_SCInsert(0) ;
				FYF_SMC_card_vaild = 2;

			}
			else
			{
				DEBUG_DVTCA("smc error!!!!!!!!\n");
				FYF_SMC_card_vaild=0;
				DVTCASTB_SCRemove();
			}	
			FYF_SMC_card_in = 1;
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);			
			FYF_API_semaphore_release (semaphoreEcm);
		}
		else if (FYF_SMC_card_in == 1 && FYF_ERR == FYF_API_smc_card_exist())
		{
	           	DEBUG_DVTCA("smc removed!\n");
			FYF_SMC_card_in=0;
			FYF_SMC_card_vaild = 0;
	            	DVTCASTB_SCRemove();
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
			CA_SetInvaildCW();
		} 	
		cas_dmx_timeout_check();
		JudgeUrgencyTime();
	}
#if 0
	BS32 ret_sem;
	static BS32 bCardInLast = FYF_ERR;
	BS32 bCardIn;

	bCardIn=FYF_API_smc_card_exist();
	bCardInLast=bCardIn;
	if(bCardIn==FYF_ERR)
	{
		DEBUG_DVTCA("smc removed!\n");
		DVTCASTB_SCRemove();
		FYF_SMC_card_in=0;
		//FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
	}
	else
	{
		FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+24, 0, 0);
		ret = DVTSTBCA_SCReset();
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+25, 0, 0);
		if( ret == DVTCA_OK)
		{
			DEBUG_DVTCA("smc insert!\n");
			DVTCASTB_SCInsert(0) ;
			FYF_SMC_card_in = 1;
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);
		}
		else
		{
			DEBUG_DVTCA("smc error!\n");
			DVTCASTB_SCRemove();
			FYF_SMC_card_in=0;
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
		}	
		FYF_API_semaphore_release (semaphoreEcm);
	}
	
	while(1)
	{
		FYF_API_thread_sleep(20);
		
	    bCardIn=FYF_API_smc_card_exist();
		if (bCardIn != bCardInLast)
	    {
	        bCardInLast = bCardIn;
	        if (FYF_OK == bCardIn)
	        {
				FYF_API_semaphore_wait (semaphoreEcm, FYF_WAIT_FOREVER);
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+24, 0, 0);
				ret = DVTSTBCA_SCReset();
				GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+25, 0, 0);
				if( ret == DVTCA_OK)
				{
					DEBUG_DVTCA("smc insert!!!!!!!!\n");
					DVTCASTB_SCInsert(0) ;
					FYF_SMC_card_in = 1;
					FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN, 0, 0);
				}
				else
				{
					DEBUG_DVTCA("smc error!!!!!!!!\n");
					DVTCASTB_SCRemove();
					FYF_SMC_card_in=0;
					FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
				}	
				FYF_API_semaphore_release (semaphoreEcm);
	        }
	        else
	        {
	            DEBUG_DVTCA("smc removed!\n");
	            DVTCASTB_SCRemove();
				FYF_SMC_card_in=0;
				FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT, 0, 0);
				CA_SetInvaildCW();
	        }
	    } 	
		cas_dmx_timeout_check();
		JudgeUrgencyTime();
		}
#endif
}
/*-------------------------------------------------------------------------------

	Description:  Modified By Ryan Lin 15th Oct 2007

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CA_DVT_smcard_init()
{	
	BU32 ret;
	FYF_API_smc_open();
	/* create smart card insert/remove notify semaphore */
	FYF_SMC_semaphore= FYF_API_semaphore_create("smc",1,FYF_THREAD_WAIT_FIFO);
	if (FYF_OK == FYF_API_smc_card_exist())
	{
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+24, 0, 0);
		ret = DVTSTBCA_SCReset();
		GUI_ENC_API_PostMessage(GUI_WND_ID_INVALID,WM_USER+25, 0, 0);
		if( ret == DVTCA_OK)
		{
			FYF_SMC_card_vaild = 1;
			DVTCASTB_SCInsert(0) ;
			FYF_SMC_card_vaild = 2;
		}
		else
		{
			DVTCASTB_SCRemove();
			FYF_SMC_card_vaild=0;
		}	
		FYF_SMC_card_in = 1;	
	}
	else
	{
		DVTCASTB_SCRemove();
		FYF_SMC_card_in = 0;
		FYF_SMC_card_vaild = 0;
	}
	/* create thread deal with smart card insert/remove */
	FYF_API_thread_create("smc",FYF_IPI_smc_thread,FYF_NULL, 15, 100 * 1024);	
	
	return ;
}
/*++
功能：实现与智能卡之间的通讯。采用7816标准。
参数：
	byReaderNo:				对应的读卡器编号。
	pbyLen:					输入为待发送命令字节的长度;输出为返回字节长度。
	byszCommand:			待发送的一串命令字节。
	byszReply:				返回的数据。
--*/
HRESULT DVTSTBCA_SCAPDU(BYTE byReaderNo,  BYTE *  pbyLen, const BYTE* byszCommand, BYTE* byszReply)
{
	BS32 ReturnResult;
	BU16 NumReaded=0;
	BU08 reponse[258];
	BU32 tryTimes,i;
		
	DEBUG_DVTCA("DVTSTBCA_SCAPDU Enter \n");	
	if(FYF_ERR==FYF_API_smc_card_exist())
	{
		return DVTCAERR_STB_CARD_INVALID;
	}
	if((pbyLen==NULL) || (byszCommand==NULL) || (byszReply==NULL))
	{
		return DVTCAERR_STB_POINTER_INVALID;
	}	
	if(*pbyLen < 5)
	{
		return DVTCAERR_STB_DATA_LEN_ERROR;
	}
//	FYF_API_semaphore_wait(FYF_SMC_semaphore, FYF_WAIT_FOREVER);	
	tryTimes = 0;
	do
	{
		ReturnResult = FYF_API_smc_iso_transfer(byszCommand,*pbyLen, reponse,258, &NumReaded);
		tryTimes++;
	}while(tryTimes < 3 && ReturnResult != 0);

	if(ReturnResult == 0)
	{
		FYF_API_Memcpy (byszReply, reponse, NumReaded);
		#if 0
		DEBUG_DVTCA("DVTSTBCA_SCAPDU NumReaded = %d\n",NumReaded);
		for(i=0;i<NumReaded;i++)
		{
			DEBUG_DVTCA("0x%x ",reponse[i]);
			if(i%4==0)
			{
				DEBUG_DVTCA("\n");
			}
		}
		DEBUG_DVTCA("\n");
		#endif
		*pbyLen = NumReaded;
		DEBUG_DVTCA("DVTSTBCA_SCAPDU End \n");
//		FYF_API_semaphore_release(FYF_SMC_semaphore);
		return DVTCA_OK;
	}
	else
	{
		DEBUG_DVTCA("DVTSTBCA_SCAPDU fail \n");
//		FYF_API_semaphore_release(FYF_SMC_semaphore);
		return DVTCAERR_STB_IC_COMMUNICATE;
	}
}




