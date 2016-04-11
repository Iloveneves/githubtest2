#include "CDCAS30.h"
#include "fyf_com.h"

//-------------------- 子母卡应用 --------------------
/*
 说明 ：
	请求机顶盒进入子卡喂养界面
*/
void  CDSTBCA_RequestFeeding(CDCA_U16 wTVSID)
{
	FYF_IPI_ca_notify(FYF_CA_NOFITY_SONCARD, (BU32 *)&wTVSID,0);
}



