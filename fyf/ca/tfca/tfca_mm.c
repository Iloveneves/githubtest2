#include "cas21.h"
#include "fyf_com.h"
#define DEBUG_CDCA //printf
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void* TFSTBCA_Malloc(CDCA_U8 byBufSize)
{
	return FYF_API_malloc (byBufSize);
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void TFSTBCA_Free(void* pBuf)
{
	DEBUG_CDCA("CDSTBCA_Free Enter\n");
	FYF_API_free (pBuf);
	DEBUG_CDCA("CDSTBCA_Free End\n");
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void TFSTBCA_Memset(void* pDestBuf, CDCA_U8 c, CDCA_U16 wSize)
{
	FYF_API_Memset (pDestBuf, c, wSize);
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void TFSTBCA_Memcpy( void* pDestBuf, const void* pSrcBuf, CDCA_U16 wSize )
{
	FYF_API_Memcpy (pDestBuf, pSrcBuf, wSize);
}


