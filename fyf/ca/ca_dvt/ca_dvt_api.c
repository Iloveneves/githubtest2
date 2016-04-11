/*===============================================================================

 File Name:    ca_dvt_api.c

 Description:

 History:      

===============================================================================*/

/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "fyf_com.h"

#define CA_PIN_LEN 			 8
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CA_DVT_API_Init(void)
{
	int ret;
	
	CA_DVT_API_TS_Init();
	DVTSTBCA_API_InitFlash();
	DVTSTBCA_API_InitRequestFeeding();
	ret = DVTCASTB_Init();
	if(true == ret)
	{
		CA_DVT_smcard_init();
		printf("DVTCASTB_Init success!\n");
	}
	else
	{
		printf("DVTCASTB_Init fail!\n");
	}
	DVTCASTB_AddDebugMsgSign(0);
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
static int emmpid_old = 0;
void FYF_DVT_SetCurEmmInfo(int emmpid,int caid)
{
	if(true == DVTCASTB_CASIDVerify(caid))
	{		
		if(emmpid_old != emmpid)
		{
			cas_demux_stopEMMFilter();
			DVTCASTB_SetEmmPid(emmpid);
			emmpid_old = emmpid;
		}
	}
}
SDVTCAServiceInfo serviceInfo_video;
SDVTCAServiceInfo serviceInfo_audio;
FYF_Ecm_Info_s serviceInfo_av,  serviceInfo_av_old;
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_DVT_SetCurEcmInfo(const FYF_Ecm_Info_s * pServiceInfo)
{

	FYF_API_Memcpy (&serviceInfo_av, pServiceInfo, sizeof(FYF_Ecm_Info_s));
	if(serviceInfo_av.m_wCaSysID == serviceInfo_av_old.m_wCaSysID && \
		serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid == serviceInfo_av_old.ca_ecm.tf_ca_ecm.m_wVideoEcmPid && \
		serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid == serviceInfo_av_old.ca_ecm.tf_ca_ecm.m_wAudioEcmPid && \
		serviceInfo_av.ca_ecm.tf_ca_ecm.m_wServiceID == serviceInfo_av_old.ca_ecm.tf_ca_ecm.m_wServiceID)
	{
		return;
	}	

	FYF_API_Memcpy (&serviceInfo_av_old, pServiceInfo, sizeof(FYF_Ecm_Info_s));
	cas_demux_stopECMFilter();

	serviceInfo_video.m_wEcmPid = pServiceInfo->ca_ecm.dvt_ca_ecm.m_wVideoEcmPid;
	serviceInfo_video.m_wServiceID = pServiceInfo->ca_ecm.dvt_ca_ecm.m_wServiceID;

	serviceInfo_audio.m_wEcmPid = pServiceInfo->ca_ecm.dvt_ca_ecm.m_wAudioEcmPid;
	serviceInfo_audio.m_wServiceID = pServiceInfo->ca_ecm.dvt_ca_ecm.m_wServiceID;

	/* 音视频ecm pid一样 */
	if(serviceInfo_video.m_wEcmPid == serviceInfo_audio.m_wEcmPid)
	{
		DVTCASTB_SetCurEcmInfo(&serviceInfo_video);
	}
	else
	{
		if(serviceInfo_video.m_wEcmPid)
			DVTCASTB_SetCurEcmInfo(&serviceInfo_video);
		if(serviceInfo_audio.m_wEcmPid)
			DVTCASTB_SetCurEcmInfo(&serviceInfo_audio);
		
	}
		
}

BS32 FYF_API_dvtca_set_value(FYF_CA_SV_e type,BU32 para1,BU32 para2)
{
	BS32 ret;
	BS32 tmp;
	BU08 tmp1;
	FYF_CA_WorkTime_s * pWorkTime;
	BU08 *pStrTmp;
	SDVTCAPin caPin;	

	switch(type)
	{
	case FYF_CA_SV_PPV:
		if(FYF_TRUE == para1)
		{
		}
		else if(FYF_FALSE == para1)
		{
		}
		break;
	case FYF_CA_SV_CORRESPOND_INFO: /* 子母卡配对信息 */
		ret = DVTCASTB_SetCorrespondInfo(para2,(BYTE *)para1);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_EMAIL_READED: /* 邮件已读 */
		ret = DVTCASTB_EmailRead((DWORD)para1);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_EMAIL_DEL:		/* 删除邮件 */
		tmp = para1;
		ret = DVTCASTB_DelEmail((DWORD)tmp);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return ret;
		}
		break;
	case FYF_CA_SV_CHANGE_PIN:
		pStrTmp = (BU08*)para1;
		caPin.m_byLen = CA_PIN_LEN;
		FYF_API_Memcpy(caPin.m_byszPin, pStrTmp, caPin.m_byLen);
		ret = DVTCASTB_ChangePin(&caPin);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else if(DVTCAERR_STB_PIN_LOCKED == ret)
		{
			return FYF_STB_PIN_LOCKED;
		}
		else if(DVTCAERR_STB_DATA_LEN_ERROR)
		{
			return FYF_STB_DATA_LEN_ERROR;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_WORK_TIME:
		pWorkTime = (FYF_CA_WorkTime_s *)para1;
		ret = DVTCASTB_SetWorkTime(pWorkTime->byStartHour,pWorkTime->byStartMinute,pWorkTime->byEndHour,pWorkTime->byEndMinute);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_RATING:		
		tmp1 = para1;
		ret = DVTCASTB_SetRating(tmp1);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_MSG_OVER:
		DVTCASTB_ShowOSDMsgOver ((BU32)para1);
		return FYF_OK;
		break;
	case FYF_CA_SV_BOOK_UNBOOK:		
		ret = DVTCASTB_BookIpp((FYF_CA_Ipp_s*)para1);

		switch(ret)
		{
		case DVTCA_OK:					//成功
			return FYF_OK;
		case DVTCAERR_STB_MONEY_LACK:		//钱不多，需要提示用户"预订成功，但余额不多，请及时充值"
			return FYF_CAERR_STB_MONEY_LACK;			
		case DVTCAERR_STB_NEED_PIN_PASS:	//需要通过PIN验证
			return FYF_CAERR_STB_NEED_PIN_PASS;
		case DVTCAERR_STB_IC_COMMUNICATE:	//与IC卡通讯参数错误
			return FYF_CAERR_STB_IC_COMMUNICATE;
		case DVTCAERR_STB_TVS_NOT_FOUND:	//运营商ID无效
			return FYF_CAERR_STB_TVS_NOT_FOUND;
		case DVTCAERR_STB_SLOT_NOT_FOUND:	//钱包没有发现
			return FYF_CAERR_STB_SLOT_NOT_FOUND;
		case DVTCAERR_STB_VER_EXPIRED:	//产品已经过期
			return FYF_CAERR_STB_VER_EXPIRED;
		case DVTCAERR_STB_OPER_INVALID:	//产品已经存在，不能操作
			return FYF_CAERR_STB_OPER_INVALID;
		case DVTCAERR_STB_NO_SPACE:		//没有空间
			return FYF_CAERR_STB_NO_SPACE;
		case DVTCAERR_STB_PROD_NOT_FOUND:	//产品没有发现
			return FYF_CAERR_STB_PROD_NOT_FOUND;
		case DVTCAERR_STB_PRICE_INVALID:	//价格无效
			return FYF_CAERR_STB_PRICE_INVALID;
		case DVTCAERR_STB_UNKNOWN:		//未知错误
			return FYF_CAERR_STB_UNKNOWN;
		}
		break;
	case FYF_CA_SV_IPP_OVER:	
		DVTCASTB_InquireBookIppOver(para1);
		return FYF_OK;
		break;
	case FYF_CA_SV_SWITCH_CHANNEL_OVER:
		DVTCASTB_SwitchChannelOver (para1);
		return FYF_OK;
		break;		
	case FYF_CA_SV_DebugMsgSign:	
		DVTCASTB_AddDebugMsgSign(para1);
		return FYF_OK;
		break;
	default:
		break;		
	}
}
BS32 FYF_API_dvtca_get_value(FYF_CA_SV_e type,BU32 *para1,BU32 *para2)
{
	FYF_CA_EmailHead_s * mailInfo;
	SDVTCATvsInfo tvsInfo[5];
	FYF_CA_TvsInfo *pTvsInfo;
	BU08 tmp,*tmp2;
	BU08 date[6];
	FYF_CA_EmailContent_s *mailBody;
	BU32 ret;
	BU32 index;
	FYF_CA_Money_s *pMoney;
	WORD mail_cnt = 50;
	BU32 new_mail_cnt = 0;
	SDVTCAEmailInfo emailInfo[50];
	SDVTCAEmailContent emailContent;
	FYF_CA_WorkTime_s * pWorkTime;
	SDVTCAPin caPin;	
//	SDVTCAIpp ca_ipp[250];
	FYF_CA_Ipp_s *pca_ipp;
	FYF_CA_Entitle_s *entitle;
	FYF_CA_ViewedIpp_s *viewedIpp;
	BU32 mailMaxNum=0;
	
	switch(type)
	{
	case FYF_CA_SV_MAIL_HEAD:	/* 读取邮件头 */
		mail_cnt = 50;
		ret = DVTCASTB_GetEmailHead((WORD *)&mail_cnt,emailInfo);
//		mailMaxNum = (mail_cnt>(*para2))?(*para2):mail_cnt;
		mailMaxNum = mail_cnt;
		if(DVTCA_OK == ret)
		{
			*para2 = mailMaxNum;
			mailInfo = (FYF_CA_EmailHead_s *)para1;
			for(index = 0;index<mailMaxNum;index++)
			{
				FYF_API_Strcpy(mailInfo[index].m_szEmailHead,emailInfo[index].m_szTitle);
				FYF_API_Strcpy(mailInfo[index].m_szSender,emailInfo[index].m_szSenderName);
				if(DVTCAS_EMAIL_STATUS_INI == emailInfo[index].m_Status)
				{
					mailInfo[index].m_bNewEmail = 1;
				}
				else if(DVTCAS_EMAIL_STATUS_READ == emailInfo[index].m_Status)
				{
					mailInfo[index].m_bNewEmail = 0;
				}
				mailInfo[index].m_tCreateTime = (BU32)emailInfo[index].m_dwVersion;
				mailInfo[index].m_dwActionID = (BU32)emailInfo[index].m_dwVersion;

			}
			return FYF_OK;
		}
		else
		{
			*para2 = 0;
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_MAIL_BODY:		/* 读取邮件正文 */
		ret = DVTCASTB_GetEmailContent(*para1,&emailContent);
		mailBody = (FYF_CA_EmailContent_s *)para2;
		if(DVTCA_OK == ret)
		{
			FYF_API_Strcpy(mailBody->m_szEmail,emailContent.m_szEmail);
			mailBody->m_wEmailLength=(BU32)emailContent.m_wEmailLength;
			return FYF_OK;
		}
		else 
		{
			if(emailContent.m_szEmail!=NULL && emailContent.m_wEmailLength!=0)
			{
				FYF_API_Strcpy(mailBody->m_szEmail,emailContent.m_szEmail);
				mailBody->m_wEmailLength=(BU32)emailContent.m_wEmailLength;
			}
			else
				mailBody->m_wEmailLength=0;
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_MSG:			/* 读取CA消息 */
		break;
	case FYF_CA_SV_OPERATOR_INFO:	/* 获得所有运营商信息 */
		tmp = (BU08)*para2;
		ret = DVTCASTB_GetOperatorInfo((WORD)-1,&tmp,(SDVTCATvsInfo*)para1);
		if(DVTCA_OK != ret)
		{
			*para2 = 0;
			return FYF_ERR;
		}	
		else
		{
			*para2 = tmp;
			return FYF_OK;
		}
		break;
	case FYF_CA_SV_MONEY_INFO:	/* 获得电子钱包信息 */
		pMoney = (FYF_CA_Money_s *)para2;
		ret = DVTCASTB_GetMoneyInfo(*para1,&(pMoney->m_paid),&(pMoney->m_balance));
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_CORRESPOND_INFO: /* 子母卡配对信息 */
		tmp = (BU08)(*para2);
		ret = DVTCASTB_GetCorrespondInfo(&tmp,(BYTE *)para1);
		if(DVTCA_OK != ret)
		{
			*para2 = 0;
			return FYF_ERR;
		}
		else
		{
			*para2 = (BU32)tmp;
			return FYF_OK;
		}
		break;
	case FYF_CA_SV_EMAIL_COUNT:	/* 获取邮件总数 */
		ret = DVTCASTB_GetEmailCount((WORD*)&mail_cnt,(WORD *)&new_mail_cnt);
		if(DVTCA_OK == ret)
		{
			*para1 = mail_cnt;
			*para2 = new_mail_cnt;
			return FYF_OK;
		}
		else
		{
			*para1 = *para2 = 0;
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_WORK_TIME:
		pWorkTime = (FYF_CA_WorkTime_s *)para1;
		ret = DVTCASTB_GetWorkTime(&(pWorkTime->byStartHour),&(pWorkTime->byStartMinute),&(pWorkTime->byEndHour),&(pWorkTime->byEndMinute));
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_VERIFY_PIN:
		tmp2 = (BU08*)para1;
		caPin.m_byLen = CA_PIN_LEN;
		FYF_API_Memcpy(caPin.m_byszPin,tmp2,caPin.m_byLen);
#if 0
		if(!GUI_API_Strcmp(tmp2,"12345678") && caPin.m_byLen == 8)
		{
			ret = DVTCASTB_VerifyPin(&caPin);
		}
		else
		{
			return FYF_STB_PIN_INVALID;
		}
#else
		ret = DVTCASTB_VerifyPin(&caPin);
#endif
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else if(DVTCAERR_STB_DATA_LEN_ERROR == ret)
		{
			return FYF_STB_DATA_LEN_ERROR;
		}
		else if(DVTCAERR_STB_PIN_LOCKED == ret)
		{
			return FYF_STB_PIN_LOCKED;
		}
		else
		{
			return FYF_STB_PIN_INVALID;
		}
		break;
	case FYF_CA_SV_GET_BOOK_IPP:
		tmp = 250;
		ret = DVTCASTB_GetBookIpps(&tmp,(SDVTCAIpp*)para1);
		if(DVTCA_OK == ret)
		{
			*para2 = tmp;
			#if 0
			pca_ipp = (FYF_CA_Ipp_s *)para1;
			for(index = 0;index<tmp;index++)
			{
				pca_ipp[index].m_byBookedInterval = ca_ipp[index].m_byBookedInterval;
				pca_ipp[index].m_byBookedPriceType = ca_ipp[index].m_byBookedPriceType;
				pca_ipp[index].m_byCurInterval = ca_ipp[index].m_byCurInterval;
				pca_ipp[index].m_byIppStatus = ca_ipp[index].m_byIppStatus;
				pca_ipp[index].m_bySlotID = ca_ipp[index].m_bySlotID;
				pca_ipp[index].m_dwDuration = ca_ipp[index].m_dwDuration;
				GUI_API_Strcpy(pca_ipp[index].m_szProdName,ca_ipp[index].m_szProdName);
				GUI_API_Strcpy(pca_ipp[index].m_szServiceName,ca_ipp[index].m_szServiceName);
				pca_ipp[index].m_tStartTime = ca_ipp[index].m_tStartTime;
				pca_ipp[index].m_wBookedPrice = ca_ipp[index].m_wBookedPrice;
				pca_ipp[index].m_wCurCppNoTapPrice = ca_ipp[index].m_wCurCppNoTapPrice;
				pca_ipp[index].m_wCurCppTapPrice = ca_ipp[index].m_wCurCppTapPrice;
				pca_ipp[index].m_wCurTppNoTapPrice = ca_ipp[index].m_wCurTppNoTapPrice;
				pca_ipp[index].m_wCurTppTapPrice = ca_ipp[index].m_wCurTppTapPrice;
				pca_ipp[index].m_wProdID =  ca_ipp[index].m_wProdID;
				pca_ipp[index].m_wTVSID = ca_ipp[index].m_wTVSID;
			}
			#endif
			return FYF_OK;
		}
		else
		{
			*para2 = 0;
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_VIEWED_PPV:
		viewedIpp = (FYF_CA_ViewedIpp_s *)para1;
		*para2 = 250;
		
		ret = DVTCASTB_GetViewedIpps((BU08 *)para2, viewedIpp);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			*para2 = 0;
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_RATING:
		ret = DVTCASTB_GetRating(&tmp);
		if(DVTCA_OK == ret)
		{
			*para1 = tmp;
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_SEVICE_ENTITLE:
		entitle = (FYF_CA_Entitle_s *)para2;

		ret = DVTCASTB_GetServiceEntitles((BU16)*para1, &(entitle->pEntitleCount), (SDVTCAServiceEntitle *) entitle->m_Entitles);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			entitle->pEntitleCount = 0;
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_CARD_INFO: 		/*卡基本信息*/
		
		ret = DVTCASTB_GetStreamGuardManuInfo((SDVTCAManuInfo *) para1); 
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_PIN_LOCK:	/*pin是否被锁*/
		
		ret = DVTCASTB_IsPinLocked((BU08 *) para1);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_AREA_INFO: 		/*区域信息*/
		ret = DVTCASTB_GetAreaInfo((SDVTCAAreaInfo *) para1);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_MOTHER_INFO: 		/*母卡卡号*/
		ret = DVTCASTB_GetMotherInfo(para1);
		if(DVTCA_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_CARD_STATE:		/* 卡状态 */
		if(FYF_CA_IPI_GetCardIn() == 1)
		{
			*para1 = FYF_CA_CARD_STATE_NORMAL;
		}
		else
		{
			*para1 = FYF_CA_CARD_STATE_OUT;
		}
		break;
	case FYF_CA_SV_CAS_ID_VERIRY: /* 检查CASID */
		ret = DVTCASTB_CASIDVerify (*(WORD*) (para1));
		if(true == ret)
		{
			return FYF_TRUE;
		}
		else if(false == ret)
		{
			return FYF_FALSE;
		}
		break;
	case FYF_CA_SV_CARD_IN:
		return FYF_CA_IPI_GetCardIn();
		break;
	default:
		break;
	}
}


