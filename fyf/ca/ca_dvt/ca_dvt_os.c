/**********************************************************************************
 File Name:   ca_dvt_os.c

 Description: STB�ṩ��CA���õĽӿ� 

 History:             
**********************************************************************************/
/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/
#include "fyf_com.h"
#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"

#define DEBUG_DVTCA //printf
/**********************************************************************************
                                ����ǰ����
**********************************************************************************/

HRESULT DVTSTBCA_Sleep( int dwSleepMicroseconds )
{
 	FYF_API_thread_sleep (dwSleepMicroseconds); 
	return DVTCA_OK;
}

/**********************************************************************************
                                ��ʼ���ź���
**********************************************************************************/
HRESULT DVTSTBCA_SemaphoreInit( DVTCA_Semaphore *pSemaphore )
{
	BU32 handle;
	if(pSemaphore == NULL)
	{
		return DVTCAERR_STB_POINTER_INVALID;
	}
	DEBUG_DVTCA("DVTSTBCA_SemaphoreInit Enter \n");
	handle = FYF_API_semaphore_create (FYF_NULL, 0, FYF_THREAD_WAIT_FIFO);
	if(0 == handle)
	{
		DEBUG_DVTCA("DVTSTBCA_SemaphoreInit fail \n");
		return DVTCAERR_STB_NO_SPACE;
	}
	*pSemaphore = (DVTCA_Semaphore)handle;
	DEBUG_DVTCA("DVTSTBCA_SemaphoreInit End \n");	
	return	DVTCA_OK ;
}

/**********************************************************************************
                                ���ź������ź�
**********************************************************************************/

HRESULT DVTSTBCA_SemaphoreSignal( DVTCA_Semaphore *pSemaphore )
{
	BU32 handle;
	if(pSemaphore == NULL)
	{
		return DVTCAERR_STB_POINTER_INVALID;
	}
	handle = (BU32)(*pSemaphore);
	if(FYF_API_semaphore_release (handle) == FYF_OK )
		return DVTCA_OK;
	else 
		return DVTCAERR_STB_POINTER_INVALID;

}

/**********************************************************************************
                      �ȴ��ź���,�ȴ��ɹ�֮��,�ź���Ϊ���ź�
**********************************************************************************/

HRESULT DVTSTBCA_SemaphoreWait( DVTCA_Semaphore *pSemaphore )
{
	BU32 handle;
	if(pSemaphore == NULL)
	{
		return DVTCAERR_STB_POINTER_INVALID;
	}
	handle = (BU32)(*pSemaphore);
	if(FYF_API_semaphore_wait (handle,FYF_WAIT_FOREVER) == FYF_OK )
		return DVTCA_OK;
	else 
		return DVTCAERR_STB_POINTER_INVALID;

}
/**********************************************************************************
                            CASģ���������ע���߳�                
**********************************************************************************/
HRESULT DVTSTBCA_RegisterTask(const char* szName,pThreadFunc pTaskFun)
{
	BU32 ret;

	DEBUG_DVTCA("DVTSTBCA_RegisterTask Enter \n");
	ret = FYF_API_thread_create (szName, (FYF_THREAD_PROC)pTaskFun, NULL, 15, 100 * 1024);
	if(0 == ret)/* �����߳�ʧ�� */
	{
		DEBUG_DVTCA("DVTSTBCA_RegisterTask fail \n");
		return DVTCAERR_STB_NO_SPACE;
	}
	DEBUG_DVTCA("DVTSTBCA_RegisterTask End \n");
	return DVTCA_OK;
}


/**********************************************************************************
                                
**********************************************************************************/

void DVTSTBCA_AddDebugMsg(const char * pszMsg)
{
	printf("[DVTCAS Lib Debug]---");
	printf(pszMsg);
	printf("\n");
}

