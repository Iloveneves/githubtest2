#include "fyf_com.h"
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "../../../tpmiddle/include/app_sys.h"

#define DEBUG_DVTCA printf

/*------------------------------------------���Ͻӿ���STB����CAS����---------------------------------------------------*/

/*++
���ܣ���ȡ���������кš�
����ֵ�����������кš�
--*/
DWORD DVTSTBCA_GetSerialNo( void )
{
	BU32 dwUniqueID;
	APP_SYS_API_GetSysInfo(APP_SYS_FACTORY_SN, &dwUniqueID);
	DEBUG_DVTCA("DVTSTBCA_GetSerialNo:dwUniqueID = 0x%x\n",dwUniqueID);	
	return (DWORD)(dwUniqueID);
}
/*++
���ܣ���ȡ����������汾��
����ֵ������������汾�š�
--*/
DWORD DVTSTBCA_GetSoftVer(void)
{
	return APP_SYS_SOFT_VERSION;//0x00010000;
}
/*------------------------------���Ͻӿ���������Ѱַ-----------------------------------*/

/*++
����:��ȡ����оƬ��CPUID��
����:
	pbyCPUID:	����оƬ�ṩ��CPUID��16���ֽ�
����ֵ:
	0:		��ȡCPUID�ɹ�
	����ֵ:	��ȡCPUIDʧ��
--*/
HRESULT DVTSTBCA_GetCPUID(BYTE * pbyCPUID)
{
	return 1;
}
/*------------------------------���Ͻӿ�����Ӳ��CW����---------------------*/

