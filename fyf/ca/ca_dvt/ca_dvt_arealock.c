/**********************************************************************************
 File Name:   ca_dvt_arealock.c

 Description: STB提供给CA调用的接口 

 History:           
**********************************************************************************/
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "fyf_com.h"
#define DEBUG_DVTCA printf

/*--------------------------------------------------------------------*/

/*++
功能：通知机顶盒需要到特征码流频点接收区域特征码流。(单频点区域锁定使用)
参数：
	wWaitSeconds:			超时时间，单位秒。
返回值：
	DVTCA_OK				成功
	其他					失败
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
功能：通知机顶盒区域锁定特征码流接收完毕。(单频点区域锁定使用)
--*/
void DVTSTBCA_AreaLockOk(void)
{
	FYF_IPI_ca_notify(FYF_CA_NOTIFY_AREA_LOCK_OK,0,0);

}


