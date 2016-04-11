#include "cas21.h"
#include "fyf_com.h"

static BU32 CDSTBCA_now = 0;
//-------------------- 子母卡应用 --------------------
/*
 说明 ：
	请求机顶盒进入子卡喂养界面
*/
void  TFSTBCA_RequestFeeding(CDCA_U16 wTVSID)
{
	FYF_IPI_ca_notify(FYF_CA_NOFITY_SONCARD, (BU32 *)&wTVSID,0);
	CDSTBCA_now = FYF_API_time_ms ();
}

void CDSTBCA_RequestFeeding_Check (void)
{
	if(CDSTBCA_now)
	{
		if(FYF_API_time_ms () >= CDSTBCA_now + 5000)
		{
			FYF_IPI_ca_notify(FYF_CA_NOFITY_FEEDING_HIDE, 0,0);
			CDSTBCA_now = 0;			
		}
	}
}


