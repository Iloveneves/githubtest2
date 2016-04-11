#include "mgdef.h"
#include "../../inc/fyf_def.h"
#include "../../os/fyf_semaphore.h"
#include "../../os/fyf_queue.h"
#include "../../os/fyf_thread.h"
#include "../../dev/fyf_descr.h"
#include "../../inc/ca/fyf_ca.h"

typedef enum
{
	CA_SMC_CARD_IN_E                    = 0x00000001,
	CA_SMC_CARD_OUT_E                   = 0x00000002,
	CA_DVT_SMC_CARD_CHNG_E                  = 0x80000000
}CA_SMC_CARD_EVENT_E;
enum  
{
   T0   = 0,
   T14 = 14,
   INVALIDT15 = 15
};
static BU32 FYF_SMC_semaphore=0;
static BU32 FYF_SMC_card_in = 0;

typedef struct
{
   int  Length;
   unsigned char 	 pATR[33];
}ATR_DATA_STRUCT;
unsigned char	 fyf_ca_mg_atr[33];
unsigned char	 fyf_ca_mg_atrlen =0 ;
static unsigned char 			fyf_ca_mg_Protocol = T14;
extern BU08 g_StbSerial[10];

#define FYF_CA_Debug soc_printf
static  BU32  CardLock_MutexId           = FYF_NULL;

/* cas init state add by szhy at 2009-01-13 */
#define __CAS_INIT_STATE__
#ifdef  __CAS_INIT_STATE__
static BU08 FYF_CasInit=0;
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
BU08 FYF_IPI_CaQueryCardInitState(void)
{
	return FYF_CasInit;
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_IPI_CaSetCardInitState(void)
{
	FYF_CasInit = 1;
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_IPI_CaClearCardInitState(void)
{
	FYF_CasInit = 0;
}
#endif
/* receive AK finish flag add by szhy at 2009-01-13 */
BU08 FYF_ReceiveAKFinish=0;

static BS32 			fyf_ca_mg_CardIn = 0;
static unsigned char  T14ATR[14]={0x3B, 0x9F, 0x21, 0x0E, 0x49, 0x52, 0x44, 0x45, 0x54, 0x4F, 0x20, 0x41, 0x43, 0x53 };
static unsigned char  T0ATR[15]={0x3B, 0x9F, 0x21, 0x40, 0x64, 0x49, 0x52, 0x44, 0x45, 0x54, 0x4F, 0x20, 0x41, 0x43, 0x53 };
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
static void FYF_MG_SmartCard_EventHandle(BU32 param)
{
	FYF_QUEUE_MESSAGE msg;

	if (param ==0)
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_OUT,0,0);
		FYF_SMC_card_in=0;
	}
	else if (param ==1)
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_IN,0,0);
		FYF_SMC_card_in=1;
	}
	FYF_API_semaphore_release(FYF_SMC_semaphore);
	return;
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_IPI_smc_thread(void *pvParam)
{
	while(1)
	{
		FYF_API_semaphore_wait(FYF_SMC_semaphore,FYF_WAIT_FOREVER);
		if(FYF_SMC_card_in)
		{
			FYF_IPI_SmartCard_Inserted();
		}
		else
		{
			FYF_IPI_SmartCard_Removed();
		}
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
BS32 FYF_MG_SmartCard_Initialise()
{
	/* create smart card insert/remove notify semaphore */
	FYF_SMC_semaphore= FYF_API_semaphore_create("smc",0,FYF_THREAD_WAIT_PRIO);
	/* create smart card protect semaphore */
	CardLock_MutexId = FYF_API_semaphore_create("mgs",1,FYF_THREAD_WAIT_FIFO);
	/* create thread deal with smart card insert/remove */
	FYF_API_thread_create("smc",FYF_IPI_smc_thread,FYF_NULL, 15, 100 * 1024);	

	/* open smart card */
	FYF_API_smc_open(FYF_MG_SmartCard_EventHandle);
	FYF_CA_Debug("FYF_MG_SmartCard_Initialise Succeed!\n");
	return FYF_OK;
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
static void FYF_IPI_SmartCard_SemLock(void)
{  
   FYF_API_semaphore_wait(CardLock_MutexId,FYF_WAIT_FOREVER);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
static void FYF_IPI_SmartCard_SemUnlock( void )
{  
   FYF_API_semaphore_release(CardLock_MutexId);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
static BS32 FYF_IPI_SmartCard_CheckATR( unsigned char *response, unsigned char *bProtocol )
{
    BU08 i;
	*bProtocol = INVALIDT15 ; 
	
    if (response[ 0 ] == T14ATR[ 0 ])
    {
		for (i = 5; i < 15; i++)
		{
			if (response[ i ] != T0ATR[ i ])
			{
				*bProtocol = INVALIDT15 ;
				break;
			}
		}
		if (i==15)
		{
			*bProtocol = T0;
		}
		if (*bProtocol == INVALIDT15)
		{
			for ( i = 2; i <14; i++ )
           	{
               if( response[ i ] != T14ATR[ i ] )
               {
					*bProtocol = INVALIDT15 ;
					break;
			   }
           	}
			if (i == 14)
			{
				*bProtocol = T14; 
			}
		}
    }
	if (*bProtocol == INVALIDT15)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/******************************************************************************/
/*Description: smart card insert process    								  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_IPI_SmartCard_Inserted(void)
{
/* delete by szhy at 2009-01-13 */
#if 0
	BU16 ATRLen =0;
	BS32  ATRChk;
	BU08  bProtocol;
	MG_STATUS MG_Status;
	int i;
#endif	
	FYF_IPI_SmartCard_SemLock();

/* smart cart insert processure  modify by szhy at 2009-01-13 */	
#if 0
	memset(fyf_ca_mg_atr , 0 , 30);
	if(MG_SUCCESS != MG_STBPort_Reset_Card(fyf_ca_mg_atr))
	{
		FYF_API_smc_deactive();
		fyf_ca_mg_CardIn = 0;
		FYF_CA_Debug("FYF_IPI_SmartCart_Inserted smart card reset fail!\n");
		return;
	}
#if 0
	FYF_CA_Debug("atr = ");
	for(i =0;i<33;i++)
	{
		
		FYF_CA_Debug("0x%2x ",fyf_ca_mg_atr[i]);
	}
	FYF_CA_Debug("  \n ");	
#endif
	MG_Status = FYF_MG_Check_Card(fyf_ca_mg_atr);
	if ((MG_Status == MG_SUCCESS)||(MG_Status == MG_ERR_NEED_GKUPD))
	{
		fyf_ca_mg_CardIn = 1;
		fyf_ca_mg_atrlen = ATRLen;
		fyf_ca_mg_Protocol = T0;
		//有卡通知
		if(MG_SUCCESS==FYF_MG_Get_Card_Ready())
		{
			FYF_CA_Debug("Valid smart card.\n");
		}
		else
		{
			FYF_CA_Debug("Invalid smart card.\n");
		}
	}
	else
	{
		FYF_CA_Debug("FYF_MG_Check_Card fail!\n");
		fyf_ca_mg_CardIn = 0;
	}
#else
	FYF_CA_MG_CPI_ResetCard();
#endif
	/* start filterring ecm/emm */

	FYF_IPI_SmartCard_SemUnlock();
}
/******************************************************************************/
/*Description: smart card remove process    								  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
static void FYF_IPI_SmartCard_Removed( void )
{
	BU08 CA_CW[8];
	BU32 audioDescrambler;
	BU32 videoDescrambler;

	FYF_IPI_SmartCard_SemLock();

	/* set reveive AK finish flag to zero add by szhy at 2009-01-13 */
	FYF_ReceiveAKFinish = 0;
	
    fyf_ca_mg_CardIn = 0;
	
	/* clear cw add by szhy at 2009-01-13 */
	memset(CA_CW,0,8);
	audioDescrambler = FYF_API_descr_open();
	FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
	FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
	videoDescrambler = FYF_API_descr_open();
	FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
	FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);

	FYF_MG_Info_CardRemoved();

	/* clear card init state add by szhy at 2009-01-13 */
	FYF_IPI_CaClearCardInitState();
	
	/* clean last program info add by szhy at 2009-01-13 */
	MG_Clean_LastPNo_Info();
	
	FYF_API_smc_deactive();
	
	/* stop filterring ecm/emm */
	
	FYF_IPI_SmartCard_SemUnlock();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Reset_Card(MG_U8 *MG_RespData)
{
	unsigned short ATRLen =0;

	if(FYF_OK != FYF_API_smc_reset(MG_RespData,&ATRLen))
	{
		return MG_FAILING;
	}
	else
	{
		return MG_SUCCESS;
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
MG_STATUS MG_STBPort_Close_Card(MG_VOID)
{
	return FYF_API_smc_deactive();
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
BU32 FYF_CA_MG_CPI_ResetCard(void)
{
	MG_STATUS MG_Status;
	MG_U8 MG_Resp[32];
	MG_U8 caversion;
	int i;
	if(MG_STBPort_Reset_Card(MG_Resp) != MG_SUCCESS)
	{
		FYF_CA_Debug("--FYF_CA_MG--:reset wrong\n");
		return FYF_ERR;
	}
	MG_Status = FYF_MG_Check_Card(MG_Resp);
	if((MG_Status == MG_SUCCESS) || (MG_Status == MG_ERR_NEED_GKUPD))
	{
		if(MG_System_Register(g_StbSerial) != MG_SUCCESS)
		{
			FYF_CA_Debug("--FYF_CA_MG--:register wrong\n");
			FYF_CA_MG_SetCaRegState(MG_FAILING);
			return FYF_ERR;
		}
		else
		{
			FYF_CA_MG_SetCaRegState(MG_SUCCESS);
		}
		if(MG_Status == MG_ERR_NEED_GKUPD)
		{
			FYF_CA_MG_SetCASNeedUpdateSatus(MG_ERR_NEED_GKUPD);
			if(MG_Update_GK() != MG_SUCCESS)
			{
				FYF_CA_Debug("--FYF_CA_MG--:update wrong\n");
				return FYF_ERR;
			}
		}
		return FYF_OK;
	}
	else
	{
		FYF_CA_Debug("--FYF_CA_MG--:reset wrong\n");
		return FYF_ERR;
	}
}
void FYF_CA_MG_API_Init(void)
{
	BU08	MG_8Byet[8] ;
	BU08    DbgMsg[35];
	BU08 	errPin;

	FYF_MG_CA_GetSysInfo();
	FYF_MG_SmartCard_Initialise();
	FYF_MG_Init_CAS();
	FYF_MG_Demux_Init();
	FYF_MG_Get_Card_Expire_Date(MG_8Byet);
	
	sprintf( DbgMsg , " %x%x%x%x-%x%x-%x%x" , MG_8Byet[0]>>4, MG_8Byet[0]&0x0f , MG_8Byet[1]>>4, MG_8Byet[1]&0x0f ,
											MG_8Byet[2]>>4, MG_8Byet[2]&0x0f , MG_8Byet[3]>>4, MG_8Byet[3]&0x0f  );	
	printf("智能卡超时时间:%s\n",DbgMsg);
	errPin = FYF_MG_Get_PinErrCount();
	printf("errPin = 0x%x\n",errPin);
	/* delete by szhy at 2009-01-13 */
	//FYF_CA_MG_CPI_ResetCard(); 
}
void FYF_API_mgca_get_value(FYF_CA_SV_e type,BU32 *para1,BU32 *para2)
{
	FYF_CA_MailInfo_s * mailInfo;
	BU08 *tmp,*tmp2;
	BU08 date[6];
	BU08 * mailBody;
	
	switch(type)
	{
	case FYF_CA_SV_MAIL_HEAD:	/* 读取邮件头 */
		mailInfo = (FYF_CA_MailInfo_s *)para1;
		tmp = FYF_MG_Get_Mail_Head();
		strcpy(mailInfo->title,tmp);
//		soc_printf("mailInfo->title=%s\n",mailInfo->title);
		strcpy(mailInfo->sender,tmp+21);
//		soc_printf("mailInfo->sender=%s\n",mailInfo->sender);
		memcpy(date,tmp+42,6);
		mailInfo->date.m_year = (date[0]>>4)*1000+(date[0]&0x0f)*100+(date[1]>>4)*10+(date[1]&0x0f);
		mailInfo->date.m_month = (date[2]>>4)*10+(date[2]&0x0f);
		mailInfo->date.m_day = (date[3]>>4)*10+(date[3]&0x0f);
		mailInfo->date.m_hour = (date[4]>>4)*10+(date[4]&0x0f);
		mailInfo->date.m_min = (date[5]>>4)*10+(date[5]&0x0f);
//		soc_printf("mailInfo->date = %d-%d-%d %d:%d\n",mailInfo->date.m_year,mailInfo->date.m_month,mailInfo->date.m_day,mailInfo->date.m_hour,mailInfo->date.m_min);
		break;
	case FYF_CA_SV_MAIL_BODY:		/* 读取邮件正文 */
		tmp = FYF_MG_Get_Mail_Body();
		mailBody = (BU08 *)para1;
		strcpy(mailBody,tmp);
//		soc_printf("mailBody:%s\n",mailBody);
		break;
	case FYF_CA_SV_MSG:			/* 读取CA消息 */
		tmp = FYF_MG_Get_Command_Data();
		tmp2 = (BU08 *)para1;
		strcpy(tmp2,tmp);
		printf("command data:%s\n",tmp2);
		break;
	default:
		break;
	}
}
void FYF_API_mgca_set_value(FYF_CA_SV_e type,BU32 para1,BU32 para2)
{
	switch(type)
	{
	case FYF_CA_SV_PPV:
		if(FYF_TRUE == para1)
		{
			FYF_MG_Set_CurTeleviewPPVTag(MG_TRUE);
		}
		else if(FYF_FALSE == para1)
		{
			FYF_MG_Set_CurTeleviewPPVTag(MG_FALSE);
		}
		break;
	default:
		break;		
	}
}

