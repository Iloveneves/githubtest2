//-------------------- 安全控制 --------------------
#include "CDCAS30.h"
#include "fyf_com.h"
#include "../../../tpmiddle/include/app_sys.h"

#define DEBUG_CDCA printf

/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_GetSTBID( CDCA_U16* pwPlatformID,  CDCA_U32* pdwUniqueID)
{
	*pwPlatformID = CDCASTB_GetPlatformID ();
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, pdwUniqueID);
	DEBUG_CDCA ("CDSTBCA_GetSTBID:wPlatformID = 0x%x, dwUniqueID = 0x%x\n",*pwPlatformID,*pdwUniqueID);	
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
CDCA_U16 CDSTBCA_SCFunction( CDCA_U8* pData)
{
	CDCA_U16 result;

	result = 0x9100;
	switch(pData[0])
	{
	case 0x02:
		DEBUG_CDCA ("复位安全模块\n");
		pData[0]=0x91;
		pData[1]=0x00;
		break;
	case 0x04:
		DEBUG_CDCA ("读取芯片序列号\n");
		pData[0]=0x91;
		pData[1]=0x00;
		break;
	case 0x10:
		DEBUG_CDCA ("设置协商密钥DCK\n");
		pData[0]=0x91;
		pData[1]=0x00;
		break;
	case 0x16:
		DEBUG_CDCA ("送入CW\n");
		pData[0]=0x91;
		pData[1]=0x00;
		break;
	default:
		DEBUG_CDCA ("不支持输入的INS\n");
		pData[0]=0x92;
		pData[1]=0x00;
		result = 0x9200;
		break;
	}
	return result;
}


