#include "cas21.h"
#include "fyf_com.h"
#define DFCA_DEBUG //printf
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_CA_TF_API_Init(void)
{
	CDCA_BOOL ret;

	CD_CA_API_TS_Init ();
	FYF_CD_SmartCard_Initialise ();
	TFSTBCA_API_InitFlash();
	ret = TFCASTB_Init(8);
	if(CDCA_TRUE == ret)
	{
		DFCA_DEBUG("TFCASTB_Init success!\n");
	}
	else if(CDCA_FALSE == ret)
	{
		DFCA_DEBUG("TFCASTB_Init fail!\n");
	}
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_TF_SetCurEmmInfo(int emmpid,int caid)
{
	CDCA_BOOL ret;

	ret = TFCASTB_IsTfCa (caid);
	if(CDCA_TRUE == ret)/* 该CAS是TFCAS */
	{
		TFCASTB_SetEmmPid(emmpid);
	}
	else if(CDCA_FALSE== ret)/* 该CAS不是TFCAS */
	{
		
	}
}
FYF_Ecm_Info_s serviceInfo_av;
FYF_Ecm_Info_s serviceInfo_av_old;
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void FYF_TF_SetCurEcmInfo(const FYF_Ecm_Info_s * pServiceInfo)
{
	CDCA_BOOL ret;
	SCDCASServiceInfo serviceInfo;

	FYF_API_Memcpy (&serviceInfo_av, pServiceInfo, sizeof(FYF_Ecm_Info_s));
	if(serviceInfo_av.m_wCaSysID == serviceInfo_av_old.m_wCaSysID && \
		serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid == serviceInfo_av_old.ca_ecm.tf_ca_ecm.m_wVideoEcmPid && \
		serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid == serviceInfo_av_old.ca_ecm.tf_ca_ecm.m_wAudioEcmPid && \
		serviceInfo_av.ca_ecm.tf_ca_ecm.m_wServiceID == serviceInfo_av_old.ca_ecm.tf_ca_ecm.m_wServiceID)
	{
		return;
	}	
	FYF_API_Memcpy (&serviceInfo_av_old, pServiceInfo, sizeof(FYF_Ecm_Info_s));
	
	TFCASTB_SetEcmPid (CDCA_LIST_FIRST, NULL);
	ret = TFCASTB_IsTfCa (serviceInfo_av.m_wCaSysID);
	if(CDCA_TRUE == ret)
	{	
		/* 音视频ecm pid一样 */
		if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid == serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid)
		{
			serviceInfo.m_byServiceNum = 1;
			serviceInfo.m_wEcmPid = serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid;
			serviceInfo.m_wServiceID[0] = serviceInfo_av.ca_ecm.tf_ca_ecm.m_wServiceID;
			TFCASTB_SetEcmPid(CDCA_LIST_ADD, &serviceInfo);			
		}
		else
		{
			/* 视频 */
			if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid)
			{
				serviceInfo.m_byServiceNum = 1;
				serviceInfo.m_wEcmPid = serviceInfo_av.ca_ecm.tf_ca_ecm.m_wVideoEcmPid;
				serviceInfo.m_wServiceID[0] = serviceInfo_av.ca_ecm.tf_ca_ecm.m_wServiceID;
				TFCASTB_SetEcmPid(CDCA_LIST_ADD, &serviceInfo);				 
			}
			/* 音频 */
			if(serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid)
			{
				serviceInfo.m_byServiceNum = 1;
				serviceInfo.m_wEcmPid = serviceInfo_av.ca_ecm.tf_ca_ecm.m_wAudioEcmPid;
				serviceInfo.m_wServiceID[0] = serviceInfo_av.ca_ecm.tf_ca_ecm.m_wServiceID;
				TFCASTB_SetEcmPid(CDCA_LIST_ADD, &serviceInfo);				 
			}
		}
	}	
	TFCASTB_SetEcmPid(CDCA_LIST_OK, NULL);
}
BU08 cdca_verify_pin[CDCA_MAXLEN_PINCODE];
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
BS32 FYF_API_tfca_get_value(FYF_CA_SV_e type,BU32 *para1,BU32 *para2)
{
	CDCA_BOOL ret;
	FYF_CA_WorkTime_s * pWorkTime;
	BU32 ca_ver=0;
	BU32 slot_conut=0;
	BU08 card_sn[CDCA_MAXLEN_SN+1]={0};
	BU08 slot_id[CDCA_MAXNUM_SLOT] = {0};
	BU08 rating=0;
	BU08 isChild=0;
	BU08 DelayTime,i=0;
	BU08 mail_cnt,mail_index=0;
	BU32 acList[CDCA_MAXNUM_ACLIST] = {0};
	BU32 acCount = 0;
	CDCA_TIME LastFeedTime;
	SCDCATVSSlotInfo slot_info[CDCA_MAXNUM_SLOT];
	FYF_CDCA_TVSSlotId_s * tf_purseSP;
	FYF_CDCA_IppvInfo_s * pIppvInfo;
	FYF_CDCA_PairInfo_s pair_info_s;
	BU16 tvs_id=0;
	BU16 *tvs_tmp;
	
	switch(type)
	{
	case FYF_CA_SV_CARD_IN:
		return FYF_CA_IPI_GetCardIn();
	case FYF_CA_SV_CARD_INFO:
		/*para1->cas_ver   para2->card_sn*/
		*para1 = TFCASTB_GetVer();
		TFCASTB_GetCardSN((BU08*)para2);
		return FYF_OK;
	case FYF_CA_SV_RATING:
		ret = TFCASTB_GetRating(&rating);
		if(CDCA_RC_OK == ret)
		{
			*para1 = rating;
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_WORK_TIME:
		pWorkTime = (FYF_CA_WorkTime_s *)para1;
		ret = TFCASTB_GetWorkTime(&(pWorkTime->byStartHour), &(pWorkTime->byStartMinute), &(pWorkTime->byStartSecond), &(pWorkTime->byEndHour), &(pWorkTime->byEndMinute), &(pWorkTime->byEndSecond));
		if(CDCA_RC_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}	
		break;

	case FYF_CA_SV_CAS_ID_VERIRY: /* 检查CASID */
		ret = TFCASTB_IsTfCa (*(CDCA_U16*) (para1));
		if(CDCA_TRUE == ret)
		{
			return FYF_TRUE;
		}
		else if(CDCA_FALSE == ret)
		{
			return FYF_FALSE;
		}
		break;
	case FYF_CA_SV_MOTHER_INFO:
		tvs_tmp = (BU16*)para1;
		ret = TFCASTB_GetOperatorIds(tvs_tmp);
		if(ret == CDCA_RC_OK)
		{
			for(i=0;i<CDCA_MAXNUM_OPERATOR;i++)
			{
				if(tvs_tmp[i])
				{
					TFCASTB_GetOperatorChildStatus(tvs_tmp[i], &isChild, &DelayTime, &LastFeedTime);
					(((FYF_CA_MotherCard_s*)para2)+i)->m_isMotherCard = isChild;
					(((FYF_CA_MotherCard_s*)para2)+i)->m_FeedCycle = DelayTime;
					(((FYF_CA_MotherCard_s*)para2)+i)->m_LastFeedTime = LastFeedTime;
				}
				else
				{
					(((FYF_CA_MotherCard_s*)para2)+i)->m_isMotherCard = 0;
					(((FYF_CA_MotherCard_s*)para2)+i)->m_FeedCycle = 0;
					(((FYF_CA_MotherCard_s*)para2)+i)->m_LastFeedTime = 0;
				}
			}
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
	case FYF_CA_SV_OPERATOR_INFO:	/* 获得所有运营商信息 */
		return (BS32)TFCASTB_GetOperatorIds ((CDCA_U16 *)para1);
	case FYF_CA_SV_SEVICE_ENTITLE:
		ret = TFCASTB_GetServiceEntitles((BU16)*para1,(FYF_CDCA_Entitles_s *)para2);
		if(ret == CDCA_RC_OK)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
	case FYF_CA_SV_MONEY_INFO:
		ret = TFCASTB_GetSlotIDs((BU16)*para1, slot_id);
		tf_purseSP = (FYF_CDCA_TVSSlotId_s *)para2;
		if(ret == CDCA_RC_OK)
		{
			for(i=0,slot_conut=0;i<CDCA_MAXNUM_SLOT;i++)
			{
				if(slot_id[i])
				{
					TFCASTB_GetSlotInfo((BU16)*para1, slot_id[i], &slot_info[i]);
					
					tf_purseSP->m_SlotInfo[i].m_wSlotId = slot_id[i];
					tf_purseSP->m_SlotInfo[i].m_wCreditLimit = slot_info[i].m_wCreditLimit;
					tf_purseSP->m_SlotInfo[i].m_wBalance = slot_info[i].m_wBalance;
					slot_conut++;
				}
				else
				{
					tf_purseSP->m_SlotInfo[i].m_wSlotId = 0;
					tf_purseSP->m_SlotInfo[i].m_wCreditLimit = 0;
					tf_purseSP->m_SlotInfo[i].m_wBalance = 0;
				}
			}
			tf_purseSP->m_SlotCount = slot_conut;
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
	case FYF_CA_SV_EMAIL_COUNT:
		mail_cnt = ((FYF_CA_EmailIdx_s *)para2)->m_Count;
		mail_index = ((FYF_CA_EmailIdx_s *)para2)->m_byFromIndex;
		ret = TFCASTB_GetEmailHeads((FYF_CA_EmailHead_s *)para1,&mail_cnt,&mail_index);
		if(ret == CDCA_RC_OK)
		{
			((FYF_CA_EmailIdx_s *)para2)->m_Count = mail_cnt;
			((FYF_CA_EmailIdx_s *)para2)->m_byFromIndex = mail_index;
			return FYF_OK;
		}
		else
		{
			((FYF_CA_EmailIdx_s *)para2)->m_Count = 0;
			((FYF_CA_EmailIdx_s *)para2)->m_byFromIndex = 0;
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_MAIL_BODY:
		ret = TFCASTB_GetEmailContent(*para1, (FYF_CA_EmailContent_s *) para2);
		if(ret == CDCA_RC_OK)
		{
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
	case FYF_CA_SV_CORRESPOND_INFO:
		tvs_id = (BU16)*para1;
		memset(pair_info_s.m_PairedInfo,0,256);
		pair_info_s.m_PairedInfo_Len = 0;
		ret = TFCASTB_ReadFeedDataFromParent(tvs_id, (CDCA_U8*)pair_info_s.m_PairedInfo, (CDCA_U8*)&(pair_info_s.m_PairedInfo_Len));
		memcpy((FYF_CDCA_PairInfo_s *)para2, &pair_info_s, sizeof(FYF_CDCA_PairInfo_s));
		if(ret == CDCA_RC_OK)
			return FYF_OK;
		else
			return FYF_ERR;
		break;
	case FYF_CA_SV_ACLIST_INFO:
		ret = TFCASTB_GetACList(*(BU16*)para1, acList);
		if(ret == CDCA_RC_OK)
		{
			for(i=0;i<CDCA_MAXNUM_ACLIST;i++)
			{
				DFCA_DEBUG("acList[%d]=%d\n",i,acList[i]);
				((FYF_CDCA_TVSACList_s *)para2)->m_ACList[i] = acList[i];
				if(acList[i])
				{
					acCount++;
				}
			}
			((FYF_CDCA_TVSACList_s *)para2)->m_acCount = acCount;
			DFCA_DEBUG("acCount=%d\n",acCount);
			return FYF_OK;
		}
		else
		{
			return FYF_ERR;
		}
		break;
	case FYF_CA_SV_Card_Pair:
		ret = TFCASTB_IsPaired((BU08 *)para1, (BU08 *)para2);

        if(ret == CDCA_RC_OK)
           return FYF_OK;  
        else if(ret == CDCA_RC_CARD_INVALID)
           return FYF_CA_CARD_INVALID;        
        else if(ret == CDCA_RC_CARD_NOPAIR)
           return FYF_CA_CARD_NOPAIR;        
        else if(ret == CDCA_RC_CARD_PAIROTHER)
           return FYF_CA_CARD_PAIROTHER;        
		break;
	case FYF_CA_SV_VIEWED_PPV:
		pIppvInfo = (FYF_CDCA_IppvInfo_s *)para1;
		return (BS32)TFCASTB_GetIPPVProgram (pIppvInfo->wTVSID, pIppvInfo->pIppv, &(pIppvInfo->wNumber));
	case FYF_CA_SV_DETITLE_READED:
		return (BS32)TFCASTB_GetDetitleReaded (*(CDCA_U16 *)para1);
	case FYF_CA_SV_DETITLE_CHK_NUMS:
		return (BS32)TFCASTB_GetDetitleChkNums (*(CDCA_U16 *)para1, para2);
	case FYF_CA_SV_STBID:
		TFSTBCA_GetSTBID ((CDCA_U16 *)para1, (CDCA_U32 *)para2);
		return FYF_OK;
	case FYF_CA_SV_TVS_NAME:
		return TFCASTB_GetOperatorInfo(*(CDCA_U16*)para1, (SCDCAOperatorInfo *)para2);
 	default:
		return FYF_ERR;
	}
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  
	
	Return:       None  

-------------------------------------------------------------------------------*/
BS32 FYF_API_tfca_set_value(FYF_CA_SV_e type,BU32 para1,BU32 para2)
{
	BU16 ret;
	BU08 rating;
	FYF_CA_WorkTime_s * pWorkTime;
	FYF_CDCA_PairInfo_s pair_info_s;
	BU08 *tmp;
	BU32 len;
	BU08 tfca_old_pin[6];
	BU08 tfca_new_pin[6];
	
	switch(type)
	{
	case FYF_CA_SV_CHANGE_PIN:
		/*get new pin*/
		tmp = (BU08 *)para1;
		len = FYF_API_Strlen (tmp);
		len = (len>6)?6:len;
		FYF_API_Memset (tfca_new_pin, 0, 6);
		FYF_API_Memcpy (tfca_new_pin, tmp, len);
		/*get old pin*/
		tmp = (BU08 *)para2;
		len = FYF_API_Strlen (tmp);
		len = (len>6)?6:len;
		FYF_API_Memset (tfca_old_pin, 0, 6);
		FYF_API_Memcpy (tfca_old_pin, tmp, len);
		/*change pin*/
		ret = TFCASTB_ChangePin(tfca_old_pin, tfca_new_pin);
		if(CDCA_RC_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			printf("ret_ChangePin=0x%x\n",ret);
			return ret;
		}
		break;
	case FYF_CA_SV_RATING:
		/*get rating*/
		rating = para1;
		/*get pin*/
		tmp = (BU08 *)para2;
		len = FYF_API_Strlen (tmp);
		len = (len>6)?6:len;
		FYF_API_Memset (tfca_old_pin, 0, 6);
		FYF_API_Memcpy (tfca_old_pin, tmp, len);
		/*set rating*/
		ret = TFCASTB_SetRating(tfca_old_pin, rating);
		if(CDCA_RC_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			printf("ret_SetRating=0x%x\n",ret);
			return ret;
		}
		break;
	case FYF_CA_SV_WORK_TIME:	
		/*get work time*/
		pWorkTime = (FYF_CA_WorkTime_s *)para1;
		/*get pin*/
		tmp = (BU08 *)para2;
		len = FYF_API_Strlen (tmp);
		len = (len>6)?6:len;
		FYF_API_Memset (tfca_old_pin, 0, 6);
		FYF_API_Memcpy (tfca_old_pin, tmp, len);
		/*set work time*/
		ret = TFCASTB_SetWorkTime (tfca_old_pin, 
									pWorkTime->byStartHour,pWorkTime->byStartMinute,pWorkTime->byStartSecond,
									pWorkTime->byEndHour,pWorkTime->byEndMinute,pWorkTime->byEndSecond);
		if(CDCA_RC_OK == ret)
		{
			return FYF_OK;
		}
		else
		{
			printf("ret_SetWorkTime=0x%x\n",ret);
			return ret;
		}
		break;
	case FYF_CA_SV_EMAIL_DEL:
		TFCASTB_DelEmail(para1);
		return FYF_OK;
	case FYF_CA_SV_CORRESPOND_INFO:
		memcpy(&pair_info_s, (FYF_CDCA_PairInfo_s *)para1, sizeof(FYF_CDCA_PairInfo_s));
		ret = TFCASTB_WriteFeedDataToChild(pair_info_s.m_PairedInfo, pair_info_s.m_PairedInfo_Len);
		if(ret == CDCA_RC_OK)
			return FYF_OK;
		else
			return FYF_ERR;
	case FYF_CA_SV_DEL_DETITLE_CHK:
		return (BS32)TFCASTB_DelDetitleChkNum ((CDCA_U16)para1, para2);
	default:
		return FYF_ERR;
	}
}

