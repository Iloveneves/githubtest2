#include "cas21.h"
#include "fyf_com.h"




//-------------------- ���� --------------------
CDCA_U16 TFSTBCA_Strlen( char* pString)
{
	return FYF_API_Strlen(pString);
}
void     TFSTBCA_Printf(CDCA_U8 byLevel,  const char* szMesssage)
{
	//printf(szMesssage);
	printf("[CAS Lib Debug-Level=%d]---",byLevel);
	printf(szMesssage);
	printf("\n");

}



