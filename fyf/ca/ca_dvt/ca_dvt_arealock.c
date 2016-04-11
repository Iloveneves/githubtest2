/**********************************************************************************
 File Name:   ca_dvt_arealock.c

 Description: STB�ṩ��CA���õĽӿ� 

 History:           
**********************************************************************************/
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "fyf_com.h"
#define DEBUG_DVTCA printf

/*--------------------------------------------------------------------*/

/*++
���ܣ�֪ͨ��������Ҫ����������Ƶ�������������������(��Ƶ����������ʹ��)
������
	wWaitSeconds:			��ʱʱ�䣬��λ�롣
����ֵ��
	DVTCA_OK				�ɹ�
	����					ʧ��
--*/
HRESULT DVTSTBCA_SwitchChannel(WORD wWaitSeconds)
{
	HRESULT ret;
	int waitSeconds = wWaitSeconds;
	DEBUG_DVTCA("DVTSTBCA_SwitchChannel wWaitSeconds=%d \n",wWaitSeconds);
	FYF_IPI_ca_notify(FYF_CA_NOTIFY_SWITCH_CHANNEL,&waitSeconds,&ret);		
	return ret;
}

/*++
���ܣ�֪ͨ����������������������������ϡ�(��Ƶ����������ʹ��)
--*/
void DVTSTBCA_AreaLockOk(void)
{
	FYF_IPI_ca_notify(FYF_CA_NOTIFY_AREA_LOCK_OK,0,0);

}


