#include "fyf_com.h"
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "../../../tpmiddle/include/app_sys.h"

#define DEBUG_DVTCA printf

/*------------------------------------------以上接口是STB供给CAS调用---------------------------------------------------*/

/*++
功能：获取机顶盒序列号。
返回值：机顶盒序列号。
--*/
DWORD DVTSTBCA_GetSerialNo( void )
{
	BU32 dwUniqueID;
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &dwUniqueID);
	DEBUG_DVTCA("DVTSTBCA_GetSerialNo:dwUniqueID = 0x%x\n",dwUniqueID);	
	return (DWORD)(dwUniqueID);
}
/*++
功能：获取机顶盒软件版本。
返回值：机顶盒软件版本号。
--*/
DWORD DVTSTBCA_GetSoftVer(void)
{
	return APP_SYS_SOFT_VERSION;//0x00010000;
}
/*------------------------------以上接口用于条件寻址-----------------------------------*/

/*++
功能:获取加密芯片的CPUID。
参数:
	pbyCPUID:	加密芯片提供的CPUID，16个字节
返回值:
	0:		获取CPUID成功
	其他值:	获取CPUID失败
--*/
HRESULT DVTSTBCA_GetCPUID(BYTE * pbyCPUID)
{
	return 1;
}
/*------------------------------以上接口用于硬件CW加密---------------------*/

