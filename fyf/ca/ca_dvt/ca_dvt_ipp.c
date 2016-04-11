/**********************************************************************************
 File Name:   ca_dvt_ipp.c

 Description: STB提供给CA调用的接口 

 History:          
**********************************************************************************/

#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "../../../tpversion/ui_demo/osd/include/resource.h"
#include "../../../tpmiddle/include/app_common.h"

FYF_CA_RealTimeIpp_s DVTCAS_RealTimeipp;

extern FYF_Ecm_Info_s serviceInfo_av;

BU32 DVTSTBCA_IPI_VerifyEcm(BU16 ecmPid)
{
	if(ecmPid == serviceInfo_av.ca_ecm.dvt_ca_ecm.m_wVideoEcmPid || ecmPid == serviceInfo_av.ca_ecm.dvt_ca_ecm.m_wAudioEcmPid)
	{
		return FYF_TRUE;
	}
	else
	{
		return FYF_FALSE;
	}
}

/*--------------------------- IPPV/IPPT -----------------------*/
/*++
功能：弹出框询问用户是否预订当前IPP节目。
参数：
	wEcmPid:				IPP对应的EcmPID
	pIpp:					当前IPP的信息。
--*/
void DVTSTBCA_InquireBookIpp(WORD wEcmPid, const SDVTCAIpp * pIpp )
{

	if(GUI_ENC_API_GetCurState() != ID_DLG_ChnInfo)
	{
		FYF_API_ca_set_value(FYF_CA_SV_IPP_OVER,wEcmPid,0);
	}	
	else
	{
		if(DVTSTBCA_IPI_VerifyEcm (wEcmPid) == FYF_TRUE)
		{
			FYF_API_Memcpy(&DVTCAS_RealTimeipp.ippvProgram, pIpp, sizeof(FYF_CA_Ipp_s));
			if((DVTCAS_RealTimeipp.ippvProgram.m_wBookedPrice)&&(((DVTCAS_RealTimeipp.ippvProgram.m_byIppStatus&0x02)>>1) == 0))
			{
				switch(DVTCAS_RealTimeipp.ippvProgram.m_byBookedPriceType)
				{
					case 0:
						DVTCAS_RealTimeipp.ippvProgram.m_wCurTppTapPrice = DVTCAS_RealTimeipp.ippvProgram.m_wBookedPrice;
						break;
					case 1:
						DVTCAS_RealTimeipp.ippvProgram.m_wCurTppNoTapPrice = DVTCAS_RealTimeipp.ippvProgram.m_wBookedPrice;
						break;
					case 2:
						DVTCAS_RealTimeipp.ippvProgram.m_wCurCppTapPrice = DVTCAS_RealTimeipp.ippvProgram.m_wBookedPrice;
						break;
					case 3:
						DVTCAS_RealTimeipp.ippvProgram.m_wCurCppNoTapPrice = DVTCAS_RealTimeipp.ippvProgram.m_wBookedPrice;
						break;
					default:
						DVTCAS_RealTimeipp.ippvProgram.m_wCurTppNoTapPrice = DVTCAS_RealTimeipp.ippvProgram.m_wBookedPrice;
						break;
				}
				
				DVTCAS_RealTimeipp.ippvProgram.m_byCurInterval = DVTCAS_RealTimeipp.ippvProgram.m_byBookedInterval;
				DVTCAS_RealTimeipp.ippvProgram.m_wBookedPrice = 0;
			}
			DVTCAS_RealTimeipp.ecmPid = wEcmPid;
			GUI_ENC_API_SwitchState(ID_DLG_RealtimeBook, 1);
		}
		else
		{
			FYF_API_ca_set_value(FYF_CA_SV_IPP_OVER,wEcmPid,0);
		}		

	}
}


