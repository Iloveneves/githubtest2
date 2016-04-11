#include "cas21.h"
#include "../../../tpversion/ui_demo/osd/include/resource.h"
#include "../../tpmiddle/include/app_common.h"
#define CDCA_DEBUG printf

BU32 TFSTBCA_IPI_BCD2Int(BU32 BCD)
{
	BU32 retVal;
	BS32 index;
	BU08 tmp[8];
	BU32 tmp2;

	retVal = 0;
	tmp2 = 0;
	for(index = 28;index>=0;index -= 4)
	{
		tmp[tmp2] = ((BCD>>index) & 0xf);
		tmp2++;
	}
	for(index = 0;index<8;index++)
	{	
		retVal *= 10;
		retVal += tmp[index];		
	}
	return retVal;
}
static ChnDataBase_ChnInfo_NoPsiSi_s chnInfo_NoPsiSi;
static SCDCALockService lockService;
void CDSTBCA_LockService_NoPsiSi(void)
{
	BU32 freq;
	BU32 symbol_rate;
	BU32 modulation;
	BU32 index;
	
	freq = TFSTBCA_IPI_BCD2Int (lockService.m_dwFrequency)/10;
	symbol_rate = TFSTBCA_IPI_BCD2Int (lockService.m_symbol_rate)/10;
	modulation = lockService.m_Modulation-1;

	APP_Signal_API_TuneToNewParam(freq/1000, symbol_rate, modulation);
	FYF_API_Memset(&chnInfo_NoPsiSi, 0, sizeof(chnInfo_NoPsiSi));
	for(index = 0;index<lockService.m_ComponentNum;index++)
	{
		if(lockService.m_CompArr[index].m_CompType == 1)
		{
			chnInfo_NoPsiSi.m_VideoPid = lockService.m_CompArr[index].m_wCompPID;
			chnInfo_NoPsiSi.m_videoEcmPID = lockService.m_CompArr[index].m_wECMPID;
		}
		else if(lockService.m_CompArr[index].m_CompType == 3)
		{
			chnInfo_NoPsiSi.m_AudioPid = lockService.m_CompArr[index].m_wCompPID;
			chnInfo_NoPsiSi.m_audioEcmPID = lockService.m_CompArr[index].m_wECMPID;
		}
	}	
	chnInfo_NoPsiSi.m_PcrPid = lockService.m_wPcrPid;
	APP_API_PlayProg_NoPsiSi(&chnInfo_NoPsiSi);
}

static BU32 cdstbca_nopsisi_flag = FYF_FALSE;
void CDSTBCA_LockService_SetNoPsiSiFlag(BU32 flag)
{
	cdstbca_nopsisi_flag = flag;
}

BU32 CDSTBCA_LockService_GetNoPsiSiFlag(void)
{
	return cdstbca_nopsisi_flag;
}
//-------------------- 强制切换频道 --------------------
void TFSTBCA_LockService( SCDCALockService* pLockService)
{
	BU32 index;
	BU32 freq;
	BU32 symbol_rate;
	APP_CHN_AV_TYPE_E av_type;
	BU32 Chn = 0,i;
	BS32 ret;
	ChnDataBase_ChnInfo_S chnInfo;
	BU16 videoPid=0,audioPid=0;
	
	CDCA_DEBUG ("TFSTBCA_LockService Enter\n");
	FYF_API_Memcpy(&lockService, pLockService, sizeof(lockService));
	CDCA_DEBUG ("pLockService->m_ComponentNum = %d\n",pLockService->m_ComponentNum);
	for(index = 0;index<pLockService->m_ComponentNum;index++)
	{
		CDCA_DEBUG ("pLockService->m_CompArr[%d].m_wCompPID = 0x%x\n",index,pLockService->m_CompArr[index].m_wCompPID);
		CDCA_DEBUG ("pLockService->m_CompArr[%d].m_wECMPID = 0x%x\n",index,pLockService->m_CompArr[index].m_wECMPID);
		CDCA_DEBUG ("pLockService->m_CompArr[%d].m_CompType = %d\n",index,pLockService->m_CompArr[index].m_CompType);
		if(pLockService->m_CompArr[index].m_CompType == 1)
		{
			videoPid = pLockService->m_CompArr[index].m_wCompPID;
		}
		else if(pLockService->m_CompArr[index].m_CompType == 3)
		{
			audioPid = pLockService->m_CompArr[index].m_wCompPID;
		}
	}
	freq = TFSTBCA_IPI_BCD2Int (pLockService->m_dwFrequency)/10;
	CDCA_DEBUG ("pLockService->m_dwFrequency = %d\n",freq);
	CDCA_DEBUG ("pLockService->m_fec_inner = %d\n",pLockService->m_fec_inner);
	CDCA_DEBUG ("pLockService->m_fec_outer = %d\n",pLockService->m_fec_outer);
	CDCA_DEBUG ("pLockService->m_Modulation = %d\n",pLockService->m_Modulation);
	symbol_rate = TFSTBCA_IPI_BCD2Int (pLockService->m_symbol_rate)/10;
	CDCA_DEBUG ("pLockService->m_symbol_rate = %d\n",symbol_rate);
	CDCA_DEBUG ("pLockService->m_wPcrPid = ox%x\n",pLockService->m_wPcrPid);

	GUI_API_SetKeyReceiveMode (KEY_RECEIVE_CLOSE);
	APP_EPG_API_SetRemindReserve(0);//2009-04-03
	APP_Com_API_Stop_Signal_Dectect ();	
	
	av_type = APP_ChnDatabase_API_GetCurChnType ();
	if(APP_CHN_AV_TYPE_TV_e != av_type)
	{
		APP_ChnDatabase_API_CreatChnList(APP_CHN_AV_TYPE_TV_e, APP_CHN_GROUP_TYPE_ALL_e);//电视列表
	}
	for(i=0;i<APP_ChnDatabase_API_GetCurTypeChnNum();i++)
	{
		if(APP_FAIL == APP_ChnDatabase_API_GetChnInfo(i, &chnInfo))
		{
			if(APP_CHN_AV_TYPE_RD_e == av_type)
			{
				APP_ChnDatabase_API_CreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);//广播列表
			}
			CDSTBCA_LockService_SetNoPsiSiFlag(FYF_TRUE);
			GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
			CDCA_DEBUG ("CDSTBCA_LockService End\n");
			return;
		}
		if(videoPid == chnInfo.m_VideoPid && audioPid == chnInfo.m_AudioPid)
		{
			break;
		}
	}	
	if(i<APP_ChnDatabase_API_GetCurTypeChnNum())
	{
		Chn = chnInfo.m_ChnIndex;
		APP_ChnDatabase_API_SetCurChnNO(Chn);
	}
	else
	{
		if(APP_CHN_AV_TYPE_RD_e == av_type)
		{
			APP_ChnDatabase_API_CreatChnList(APP_CHN_AV_TYPE_RD_e, APP_CHN_GROUP_TYPE_ALL_e);//广播列表
		}
		CDSTBCA_LockService_SetNoPsiSiFlag(FYF_TRUE);
	}
	GUI_ENC_API_SwitchState(ID_DLG_ChnInfo, 1);
	CDCA_DEBUG ("TFSTBCA_LockService End\n");
}
void TFSTBCA_UNLockService(void)
{
	CDCA_DEBUG ("TFSTBCA_UNLockService Enter\n");
	GUI_API_SetKeyReceiveMode (KEY_RECEIVE_OPEN);
	APP_EPG_API_SetRemindReserve(1);//2009-04-03
	APP_Com_API_Start_Signal_Dectect ();	
	CDCA_DEBUG ("TFSTBCA_UNLockService End\n");
}


