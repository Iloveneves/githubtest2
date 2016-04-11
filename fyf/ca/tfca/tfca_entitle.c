#include "cas21.h"
#include "fyf_com.h"
#define DFCA_DEBUG printf

//-------------------- 授权信息管理 --------------------
/*当智能卡授权发生变化的时候，调用接口通知机顶盒*/
void TFSTBCA_EntitleChanged(CDCA_U16 wTvsID)
{
	DFCA_DEBUG("@@CDSTBCA_EntitleChanged: wTvsID = 0x%x\n",  wTvsID);
//	cas_struct.disp_callback(CAS_MSG_ENTITLE_CHANGED, wTvsID);	
	return;	
}

void TFSTBCA_DetitleReceived(CDCA_U16 wTvsID, CDCA_U8 bstatus)
{
	DFCA_DEBUG("@@@@CDSTBCA_DetitleReceived: wTvsID = 0x%x, bstatus=0x%x\n", wTvsID, bstatus);
//	cas_struct.disp_callback(CAS_MSG_DETITLE_RECEIVED, (wTvsID<<8)|bstatus);
	FYF_IPI_ca_notify (FYF_CA_NOTIFY_DETITLE_RECEIVED, (BU32 *)&wTvsID , (BU32 *)&bstatus);
	return;

}

