#include "CDCAS30.h"
#include "fyf_com.h"

//-------------------- ��ĸ��Ӧ�� --------------------
/*
 ˵�� ��
	��������н����ӿ�ι������
*/
void  CDSTBCA_RequestFeeding(CDCA_U16 wTVSID)
{
	FYF_IPI_ca_notify(FYF_CA_NOFITY_SONCARD, (BU32 *)&wTVSID,0);
}



