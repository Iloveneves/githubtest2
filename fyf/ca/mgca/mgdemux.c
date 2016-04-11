#include "mgdef.h"
#include "../../inc/fyf_def.h"
#include "../../dev/fyf_demux.h"
#include "../../dev/fyf_descr.h"
#include "../../inc/ca/fyf_ca.h"

/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
static BU32 EcmChannelHandle ,EcmFilterHandle;
static BU32 EmmChannelHandle ,EmmFilterHandle;
BU08 g_StbSerial[10]={0,0,0,0,0,0,0,0,0,0};
static MG_STATUS g_EcmResult=100;
static BU32 oKeyValue[2];
static BU32 eKeyValue[2];
static FYF_Ecm_Info_s FYF_ecmInfo;
static BU32 FYF_emmpid,FYF_caid;
static BU32 FYF_startFilterEcm=0,FYF_startFilterEmm=0;
static BU08 FYF_EMM_E_Value,FYF_EMM_E_Mask;
extern BU08 FYF_ReceiveAKFinish;
BU32 FYF_EMM_semaphore;
BU16 FYF_MG_CA_id;
BU08 FYF_MG_CA_ID[2];
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_MG_SetCurEcmInfo(const FYF_Ecm_Info_s * pServiceInfo)
{
/* modify by szhy at 2009-01-14 */
#if 0
	if(FYF_NULL != EcmChannelHandle && pServiceInfo->m_wEcmPid)
	{
		FYF_API_demux_disable_filter(EcmChannelHandle,EcmFilterHandle);
		FYF_API_demux_set_channel_pid(EcmChannelHandle,pServiceInfo->m_wEcmPid);
		FYF_API_demux_start_channel(EcmChannelHandle);
		FYF_API_demux_enable_filter(EcmChannelHandle,EcmFilterHandle);
	}
#else
	BU08 value[1]={0x80};
	BU08 mask[1]={0xfe};
	FYF_ecmInfo.m_wCaSysID = pServiceInfo->m_wCaSysID;
	FYF_ecmInfo.m_wEcmPid = pServiceInfo->m_wEcmPid;
	FYF_ecmInfo.m_wServiceID = pServiceInfo->m_wServiceID;
	if(FYF_startFilterEcm)
	{
		FYF_API_demux_disable_filter(EcmChannelHandle,EcmFilterHandle);
		FYF_startFilterEcm=0;
	}
	if(FYF_NULL != EcmChannelHandle && FYF_ecmInfo.m_wEcmPid && 0 == FYF_startFilterEcm)
	{
		FYF_API_demux_set_channel_pid(EcmChannelHandle,FYF_ecmInfo.m_wEcmPid);
		FYF_API_demux_start_channel(EcmChannelHandle);
		FYF_API_demux_set_filter(EcmChannelHandle,EcmFilterHandle,1,value,mask,0);
		FYF_API_demux_enable_filter(EcmChannelHandle,EcmFilterHandle);
		FYF_startFilterEcm = 1;
	}
#endif
}
struct restrict{
    BU08  mask[16];
    BU08  value[8][16];
    BU08  mask_len;                   /* Mask length in unit of byte */
    BU08  value_num;                 /* Target value number. */    
    BU08 multi_mask[8][16];
    BU16  tb_flt_msk; 
};
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_MG_SetCurEmmInfo(int emmpid,int caid)
{
/* modify by szhy at 2009-01-14 */
#if 1
	if(FYF_startFilterEmm)
	{
		FYF_API_demux_disable_filter(EmmChannelHandle,EmmFilterHandle);
		FYF_startFilterEmm=0;
	}	
	if(FYF_NULL != EmmChannelHandle && emmpid && 0 == FYF_startFilterEmm)
	{		
		FYF_API_demux_set_channel_pid(EmmChannelHandle,emmpid);
		FYF_API_demux_start_channel(EmmChannelHandle);
		FYF_API_demux_enable_filter(EmmChannelHandle,EmmFilterHandle);
		FYF_startFilterEmm = 1;
	}
#else
	FYF_emmpid = emmpid;
	FYF_caid = caid;
	FYF_ecmInfo.m_wCaSysID = caid;
	FYF_EMM_E_Value = 0x82;
	FYF_EMM_E_Mask = 0xfe;
	FYF_MG_FilterEmm();
#endif
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_MG_FilterEmm(void)
{
#if 0
	struct restrict fyf_restrict;
	BU32 index;
	BU16 validCounter;
	FYF_API_semaphore_wait(FYF_EMM_semaphore,FYF_WAIT_FOREVER);
	if(FYF_startFilterEmm)
	{
		FYF_API_demux_disable_filter(EmmChannelHandle,EmmFilterHandle);
		FYF_startFilterEmm=0;		
	}
	if(FYF_NULL != EmmChannelHandle && FYF_emmpid && 0 == FYF_startFilterEmm)
	{
		FYF_API_demux_set_channel_pid(EmmChannelHandle,FYF_emmpid);
		FYF_API_demux_start_channel(EmmChannelHandle);
		fyf_restrict.value_num = 0;
		fyf_restrict.mask_len = 1;
		fyf_restrict.tb_flt_msk =0;
		/* EMM_E */
		for(index=0;index<7;index++)
		{
			fyf_restrict.multi_mask[0][index] = 0;
			fyf_restrict.value[0][index] = 0;			
		}
		fyf_restrict.multi_mask[0][0] = FYF_EMM_E_Mask;
		fyf_restrict.value[0][0] = FYF_EMM_E_Value;
		fyf_restrict.tb_flt_msk = 1;

		/* EMM_U */
		for(index=0;index<7;index++)
		{
			fyf_restrict.multi_mask[1][index] = 0;
			fyf_restrict.value[1][index] = 0;			
		}
		fyf_restrict.multi_mask[1][0] = 0xff;
		fyf_restrict.value[1][0] = 0x84;
		validCounter = FYF_MG_Get_UCounter()+1;
		fyf_restrict.value[1][1] = validCounter>>8;
		fyf_restrict.value[1][2] = validCounter;
		FYF_MG_Get_Card_ID(fyf_restrict.value[1]+3);
		fyf_restrict.multi_mask[1][1] = 0xff;
		fyf_restrict.multi_mask[1][2] = 0xff;
		fyf_restrict.multi_mask[1][3] = 0xff;
		fyf_restrict.multi_mask[1][4] = 0xff;
		fyf_restrict.multi_mask[1][5] = 0xff;
		fyf_restrict.multi_mask[1][6] = 0xff;
		fyf_restrict.tb_flt_msk = 3;	
		/* EMM_G */
		for(index=0;index<7;index++)
		{
			fyf_restrict.multi_mask[2][index] = 0;
			fyf_restrict.value[2][index] = 0;			
		}
		fyf_restrict.multi_mask[2][0] = 0xff;
		fyf_restrict.value[2][0] = 0x85;
		validCounter = FYF_MG_Get_GCounter()+1;
		fyf_restrict.value[2][1] = validCounter>>8;
		fyf_restrict.value[2][2] = validCounter;	
		fyf_restrict.multi_mask[2][1] = 0xff;
		fyf_restrict.multi_mask[2][2] = 0xff;		
		fyf_restrict.tb_flt_msk = 7;	
		/* EMM_A */
		for(index=0;index<7;index++)
		{
			fyf_restrict.multi_mask[3][index] = 0;
			fyf_restrict.value[3][index] = 0;			
		}
		fyf_restrict.multi_mask[3][0] = 0xff;
		fyf_restrict.value[3][0] = 0x87;
		validCounter = FYF_MG_Get_ACounter()+1;
		fyf_restrict.value[3][1] = validCounter>>8;
		fyf_restrict.value[3][2] = validCounter;	
		fyf_restrict.multi_mask[3][1] = 0xff;
		fyf_restrict.multi_mask[3][2] = 0xff;		
		fyf_restrict.tb_flt_msk = 15;
		FYF_API_demux_enable_filter_ex(EmmChannelHandle,EmmFilterHandle,&fyf_restrict,4,7);
		FYF_startFilterEmm=1;
	}
	FYF_API_semaphore_release(FYF_EMM_semaphore);
#endif
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_MG_SectionNotify(BU32 ch, BU32 filter, BU08 *buf, BS32 len)
{
	MG_U8 MG_CWUpd,MG_BalUpd;
	MG_U8 MG_EMMResult;
	MG_U8 MG_cardid[10];	
	MG_STATUS Result;
	MG_STATUS RegResult, GKUpdateResult;

	if(ch == EcmChannelHandle && filter == EcmFilterHandle)
	{
		if(MG_FAILING != FYF_CA_MG_GetCaRegState())
		{
			Result = FYF_MG_CAS_ECM_Process(buf,&MG_CWUpd,&MG_BalUpd,0);
			FYF_MG_ECMResultProcess(Result,MG_CWUpd,MG_BalUpd);
		}
	}
	else if(ch == EmmChannelHandle && filter == EmmFilterHandle)
	{
		if(0x82 == buf[0] || 0x83 == buf[0])
		{
	//		soc_printf("FYF_MG_CAS_EMM_E_Process\n");
			Result = FYF_MG_CAS_EMM_E_Process(buf,&FYF_ReceiveAKFinish);
			if(Result==MG_SUCCESS && FYF_ReceiveAKFinish)
			{
				if(0x82 == buf[0])
				{
					FYF_EMM_E_Value = 0x83;
				}
				else if(0x83 == buf[0])
				{
					FYF_EMM_E_Value = 0x82;
				}
				FYF_EMM_E_Mask = 0xff;
				FYF_MG_FilterEmm();
				if(MG_FAILING== FYF_CA_MG_GetCaRegState())
				{
					RegResult = MG_System_Register( g_StbSerial);
					if(MG_SUCCESS == RegResult)
					{
						FYF_CA_MG_SetCaRegState(MG_SUCCESS);
						if(FYF_CA_MG_GetCASNeedUpdateSatus() == MG_ERR_NEED_GKUPD)
						{
							GKUpdateResult = FYF_MG_CAS_Update_GK();
							if(GKUpdateResult==MG_SUCCESS)
							{
								
							}
							else if(GKUpdateResult==MG_FAILING)
							{
								
							}
						}
						FYF_MG_FilterEmm();
					}					
				}					
			}
		}
		else
		{
			if(FYF_ReceiveAKFinish)
			{
				if(0x84 == buf[0])
		 		{
			//		soc_printf("FYF_MG_CAS_EMM_U_Process\n");
					Result = FYF_MG_CAS_EMM_U_Process(buf,&MG_EMMResult);
			//		soc_printf("FYF_EMM_U_Counter=%d\n",FYF_EMM_U_Counter);
					FYF_MG_FilterEmm();
					FYF_MG_EMMResultProcess(Result,MG_EMMResult);
				}
				else if(0x85 == buf[0])
				{
			//		soc_printf("FYF_MG_CAS_EMM_G_Process\n");
					Result = FYF_MG_CAS_EMM_G_Process(buf,&MG_EMMResult);
			//		soc_printf("FYF_EMM_G_Counter=%d\n",FYF_EMM_G_Counter);
					FYF_MG_FilterEmm();
					FYF_MG_EMMResultProcess(Result,MG_EMMResult);
				}
				else if(0x87 == buf[0])
				{
			//		soc_printf("FYF_MG_CAS_EMM_A_Process\n");
					Result = FYF_MG_CAS_EMM_A_Process(buf,&MG_EMMResult);
			//		soc_printf("FYF_EMM_A_Counter=%d\n",FYF_EMM_A_Counter);
					FYF_MG_FilterEmm();
					FYF_MG_EMMResultProcess(Result,MG_EMMResult);
				}
			}
		}
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_MG_Demux_Init(void)
{
	/* 创建请求ecm的channel */
	EcmChannelHandle = FYF_API_demux_create_channel(1024,FYF_DEMUX_DATA_PSI_CHANNEL);
	if(FYF_NULL == EcmChannelHandle)
	{
		FYF_CA_Debug("FYF_MG_Demux_Init:create ecm channel fail!\n");
		return;
	}
	/* 创建请求ecm的filter */
	EcmFilterHandle  = FYF_API_demux_create_filter(EcmChannelHandle);
	if(FYF_NULL == EcmFilterHandle)
	{
		FYF_CA_Debug("FYF_MG_Demux_Init:create ecm filter fail!\n");
		return;
	}
	/* 注册请求ecm的回调函数 */
	APP_API_RegisterSectionRequest(EcmChannelHandle,EcmFilterHandle,FYF_MG_SectionNotify);
	/* 创建请求emm的channel */
	EmmChannelHandle = FYF_API_demux_create_channel(1024,FYF_DEMUX_DATA_PSI_CHANNEL);
	if(FYF_NULL == EmmChannelHandle)
	{
		FYF_CA_Debug("FYF_MG_Demux_Init:create emm channel fail!\n");
		return;
	}	
	/* 创建请求emm的filter */
	EmmFilterHandle  = FYF_API_demux_create_filter(EmmChannelHandle);
	if(FYF_NULL == EmmFilterHandle)
	{
		FYF_CA_Debug("FYF_MG_Demux_Init:create emm filter fail!\n");
		return;
	}
	/* 注册请求emm的回调函数 */	
	APP_API_RegisterSectionRequest(EmmChannelHandle,EmmFilterHandle,FYF_MG_SectionNotify);
	/* 创建保护emm请求的信号量 */
	FYF_EMM_semaphore = FYF_API_semaphore_create("emm",1,FYF_THREAD_WAIT_FIFO);
	/* 获取ca id */
	FYF_MG_Get_CAS_ID(FYF_MG_CA_ID);
	FYF_MG_CA_id = FYF_MG_CA_ID[0]<<8 | FYF_MG_CA_ID[1];
	printf("ca id = 0x%x\n",FYF_MG_CA_id);
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_MG_CA_GetSysInfo(void)
{
	FYF_API_e2p_read(g_StbSerial,10,1,FYF_API_e2p_open("mgca",10));
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_MG_ECMResultProcess(MG_STATUS EcmResult,MG_U8 MG_pCWUpd,MG_U8 OsdPromtFlag)
{
	BU08 CA_CW[8];
	BU32 audioDescrambler;
	BU32 videoDescrambler;

	if(EcmResult == MG_SUCCESS )
	{
		if( MG_pCWUpd )
		{
			if( MG_pCWUpd & 0x01)
			{
				FYF_MG_Get_Odd_CW(CA_CW);
				audioDescrambler = FYF_API_descr_open();
				FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
				videoDescrambler = FYF_API_descr_open();
				FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_ODD_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
			}
			if( MG_pCWUpd & 0x02)
			{
				FYF_MG_Get_Even_CW(CA_CW);
				audioDescrambler = FYF_API_descr_open();
				FYF_API_descr_set(audioDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
				videoDescrambler = FYF_API_descr_open();
				FYF_API_descr_set(videoDescrambler,FYF_DESCR_KEY_PARITY_EVEN_PARITY,FYF_DESCR_KEY_USAGE_VALID_FOR_ALL,CA_CW);
			}
		}		
		if( OsdPromtFlag )	//show current program is PPV
		{
		}			
	}
	if(EcmResult == MG_ERR_DONOT_TELEVIEW_PPV || EcmResult == MG_ERR_QUESTION_TELEVIEW_PPV)
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_PPV,0,0);
	}
	else if(EcmResult == MG_ERR_APP_LOCKED)//show application locked
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_APP_LOCKED,0,0);		
	}
	else if(EcmResult == MG_ERR_CARD_LOCKED)//show card locked
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_LOCKED,0,0);			
	}
	else if(EcmResult == MG_ERR_NOT_AUTHORED)//show current channel not authed
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_NOT_AUTHORED,0,0);	
	}
	else if(EcmResult == MG_ERR_CARD_NOTFOUND)//show no card
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_CARD_NOTFOUND,0,0);	
	}
	else if(EcmResult == MG_ERR_NO_MONEY)//show no money to watch current ppv channel
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_NO_MONEY,0,0);	
	}
	else if(EcmResult == MG_ERR_EXTER_AUTHEN)//show card authentication error
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_EXTER_AUTHEN,0,0);	
	}
	else if(EcmResult == MG_ERR_PIN_AUTHEN)//show the partner error according to 'PinErrCount'
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_PIN_AUTHEN,0,0);
	}
	else if(EcmResult == MG_ERR_NOREG_CARD)//show card not register
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_NOREG_CARD,0,0);
	}
	else if(EcmResult == MG_ERR_ILLEGAL_CARD)//show illegal card
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_ILLEGAL_CARD,0,0);
	}
	else if(MG_ERR_SCARD_NOT_ACT == EcmResult)//please activate the son card
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_SCARD_NOT_ACT,0,0);		
	}
	else if(MG_ERR_SCARD_DISABLED == EcmResult)//the son card is time out
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_SCARD_DISABLE,0,0);		
	}
	else if(MG_ERR_ZONE_CODE == EcmResult)//the zone code is error
	{
		FYF_IPI_ca_notify(FYF_CA_NOTIFY_ZONE_CODE,0,0);		
	}
}
/******************************************************************************/
/*Description:              												  */
/*Input 	 :              												  */
/*Output	 :   															  */
/*Return	 :                      										  */
/******************************************************************************/
void FYF_MG_EMMResultProcess(MG_STATUS EmmResult,MG_U8 MG_pEMMResult)
{
	MG_U8  *pMailHead;
	MG_U8  *pMailBody;
	MG_U32 value;
	BS08 temp[21]={0};

	if(EmmResult == MG_SUCCESS)
	{
		switch(MG_pEMMResult)
		{
		case MG_EMM_APP_STOP :	//show card app stop
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_APP_STOP,0,0); 
			break;

		case MG_EMM_APP_RESUME :	//show card app resume
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_APP_RESUME,0,0); 
			break;
			
		case MG_EMM_PROG_UNAUTH :	//show there are progs unauthed
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_PROG_UNAUTH,0,0); 
			break;
			
		case MG_EMM_PROG_AUTH :	//show there are progs authed
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_PROG_AUTH,0,0); 
			break;
			
		//write the STBserial to flash or E2PROM
		case MG_EMM_EN_PARTNER:	//show set stb-card partnered
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_EN_PARTNER,0,0); 
			FYF_MG_Get_STB_Serial( g_StbSerial) ;
			FYF_API_e2p_write(g_StbSerial,10,1,FYF_API_e2p_open("mgca",10));
			break;

		//write the STBserial to flash or E2PROM
		case MG_EMM_DIS_PARTNER:	//show set stb-card dis-partnered
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_DIS_PARTNER,0,0); 		
			FYF_MG_Get_STB_Serial( g_StbSerial) ;
			FYF_API_e2p_write(g_StbSerial,10,1,FYF_API_e2p_open("mgca",10));	
			break;

		case  MG_EMM_UPDATE_GNO :	//show group id changed
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_UPDATE_GNO,0,0); 		
			break;
			
		case MG_EMM_MSG :	//get message
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_MSG,0,0);
			break;

		case MG_EMM_UPDATE_GK :	//show card's GK need update
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_UPDATE_GK,0,0);
			break;
			
		case MG_EMM_EMAIL:
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_MAIL,0,0);
			break;
			
		case MG_EMM_CHARGE:
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_CHARGE,0,0);
			break;
			
		case MG_EMM_FIGEIDENT:
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_FIGEIDENT,0,0);
			break;	
			
		case MG_EMM_SONCARD:
			FYF_IPI_ca_notify(FYF_CA_NOFITY_SONCARD,0,0);
			break;	
			
		case MG_EMM_URGENT_SERVICE:
			FYF_IPI_ca_notify(FYF_CA_NOFITY_URGENT_SERVICE,0,0);
			break;	
			
		case MG_EMM_MODIFY_ZONECODE:
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_MODIFY_ZONECODE,0,0);
			break;
			
		case MG_EMM_LOADER:
			FYF_IPI_ca_notify(FYF_CA_NOTIFY_LOADER,0,0);
			break;
			
		default :
			break;						
		}
	}
}


