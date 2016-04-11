#include "cas21.h"
#include "fyf_com.h"
#define DEBUG_CDCA //printf
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
CDCA_BOOL TFSTBCA_RegisterTask( const char* szName, CDCA_U8 byPriority,  void* pTaskFun,  void* pParam, CDCA_U16 wStackSize)
{
	BU32 ret;

	DEBUG_CDCA("TFSTBCA_RegisterTask Enter \n");
	ret = FYF_API_thread_create (szName, (FYF_THREAD_PROC)pTaskFun, pParam, 15, wStackSize);
	if(0 == ret)/* 创建线程失败 */
	{
		DEBUG_CDCA("TFSTBCA_RegisterTask fail \n");
		return CDCA_FALSE;
	}
	DEBUG_CDCA("TFSTBCA_RegisterTask End \n");
	return CDCA_TRUE;
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void TFSTBCA_Sleep(CDCA_U16 wMilliSeconds)
{
	DEBUG_CDCA("TFSTBCA_Sleep Enter \n");
	FYF_API_thread_sleep (wMilliSeconds);
	DEBUG_CDCA("TFSTBCA_Sleep End \n");	
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void TFSTBCA_SemaphoreInit( CDCA_Semaphore* pSemaphore, CDCA_BOOL bInitVal)
{
	BU32 handle;

	DEBUG_CDCA("TFSTBCA_SemaphoreInit Enter \n");
	handle = FYF_API_semaphore_create (FYF_NULL, bInitVal, FYF_THREAD_WAIT_FIFO);
	if(0 == handle)
	{
		DEBUG_CDCA("TFSTBCA_SemaphoreInit fail \n");
		return;
	}
	*pSemaphore = (CDCA_Semaphore)handle;
	DEBUG_CDCA("TFSTBCA_SemaphoreInit End \n");
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void TFSTBCA_SemaphoreSignal( CDCA_Semaphore* pSemaphore )
{
	BU32 handle;

	handle = (BU32)(*pSemaphore);
	FYF_API_semaphore_release (handle);
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void TFSTBCA_SemaphoreWait( CDCA_Semaphore* pSemaphore )
{
	BU32 handle;

	handle = (BU32)(*pSemaphore);
	FYF_API_semaphore_wait (handle,FYF_WAIT_FOREVER);
}

