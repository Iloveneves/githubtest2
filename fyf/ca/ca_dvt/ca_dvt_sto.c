/**********************************************************************************
 File Name:   ca_dvt_sto.c

 Description: STB�ṩ��CA���õĽӿ� 

 History:               
**********************************************************************************/
/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/

#include "inc/DVTCAS_STBDataPublic.h"
#include "inc/DVTCAS_STBInf.h"
#include "inc/DVTSTB_CASInf.h"
#include "fyf_com.h"

#ifdef BOX_ENCRYPT_E2PROM
#define EEPROM_DVTCA_FLASH_FLAG_ADDR (46)
#define EEPROM_DVTCA_FLASH_ADDR (48)
#else
#define EEPROM_DVTCA_FLASH_FLAG_ADDR (46)//(256)
#define EEPROM_DVTCA_FLASH_ADDR (48)//(257)
#endif
#define DVT_FLASH_SIZE 			  (64*1024)
static char DVT_FLASH_BUF[DVT_FLASH_SIZE]; 
BU08 dvtca_flash_cur_block;		/* Ŀǰ���ݻ�������FLASH��Ӧ,0xaa�������һ���Ӧ,0xbb������ڶ����Ӧ */
BU08 dvtca_write_enable = FYF_FALSE;
static BU32 dvtca_flash_semaphore=0;
#define DVTCA_FLASH_CHECK_WRITE
#define DVTCA_FLASHWRITE_INTERVAL 1000
#define DEBUG_DVTCA //printf
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void DVTSTBCA_Flash_protect(void)
{
#ifdef 	DVTCA_FLASH_CHECK_WRITE
	FYF_API_semaphore_wait(dvtca_flash_semaphore, FYF_WAIT_FOREVER);
#endif
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void DVTSTBCA_Flash_unprotect(void)
{
#ifdef 	DVTCA_FLASH_CHECK_WRITE
	FYF_API_semaphore_release(dvtca_flash_semaphore);
#endif
}
/**********************************************************************************
                ��û����з����CAS�Ĵ洢�ռ������ַ�ʹ�СBytes
**********************************************************************************/
HRESULT DVTSTBCA_GetDataBufferAddr(long* lSize,  char ** ppStartAddr)
{

	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;

	DEBUG_DVTCA("DVTSTBCA_GetDataBufferAddr Enter \n");
	DVTSTBCA_Flash_protect();
	fyf_e2p_ioctl_param.buffer = &dvtca_flash_cur_block;
	fyf_e2p_ioctl_param.bufferSize = 1;
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_READ_EX, EEPROM_DVTCA_FLASH_FLAG_ADDR, (BU32)&fyf_e2p_ioctl_param);
	DEBUG_DVTCA("DVTSTBCA_GetDataBufferAddr dvtca_flash_cur_block = %d\n",dvtca_flash_cur_block);

	FYF_FLS_IOCTL_param.buffer = DVT_FLASH_BUF;
	FYF_FLS_IOCTL_param.bufferSize = DVT_FLASH_SIZE;
	if(0xaa == dvtca_flash_cur_block)
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_CA, (BU32)&FYF_FLS_IOCTL_param);
	}
	else if(0xbb == dvtca_flash_cur_block)
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_CABK, (BU32)&FYF_FLS_IOCTL_param);		
	}
	else	/* CA��һ��ʹ��FLASH */
	{
		dvtca_flash_cur_block = 0xaa;
	}
	*ppStartAddr = DVT_FLASH_BUF;
	*lSize = DVT_FLASH_SIZE;
	DVTSTBCA_Flash_unprotect();
	DEBUG_DVTCA("DVTSTBCA_GetDataBufferAddr End \n");
	return DVTCA_OK;
}

/**********************************************************************************
                       ��ȡ�����ڻ�����flash�е���Ϣ
**********************************************************************************/
HRESULT DVTSTBCA_ReadDataBuffer(const char * pStartAddr, long * plDataLen,  unsigned char* pData)
{
	DEBUG_DVTCA("DVTSTBCA_ReadDataBuffer Enter \n");
	DVTSTBCA_Flash_protect();
	FYF_API_Memcpy (pData, (void *)pStartAddr, *plDataLen);
	DVTSTBCA_Flash_unprotect();
	DEBUG_DVTCA("DVTSTBCA_ReadDataBuffer End \n");
	return DVTCA_OK;
}
/**********************************************************************************
                         ����Ϣ���浽������flash��
**********************************************************************************/
void DVTSTBCA_IPI_WriteBuffer(void)
{
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	
	FYF_FLS_IOCTL_param.buffer = DVT_FLASH_BUF;
	FYF_FLS_IOCTL_param.bufferSize = DVT_FLASH_SIZE;	
	if(0xaa == dvtca_flash_cur_block)/* �����ݱ��浽�ڶ��� */
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_CABK, (BU32)&FYF_FLS_IOCTL_param);
		dvtca_flash_cur_block = 0xbb;
	}
	else if(0xbb == dvtca_flash_cur_block)/* �����ݱ��浽��һ�� */
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_CA, (BU32)&FYF_FLS_IOCTL_param);
		dvtca_flash_cur_block = 0xaa;
	}
	fyf_e2p_ioctl_param.buffer = &dvtca_flash_cur_block;
	fyf_e2p_ioctl_param.bufferSize = 1;
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_WRITE_EX, EEPROM_DVTCA_FLASH_FLAG_ADDR, (BU32)&fyf_e2p_ioctl_param);
}

HRESULT DVTSTBCA_WriteDataBuffer(const char * pStartAddr,  long * plDataLen,  const unsigned char* pData)
{
	DEBUG_DVTCA("DVTSTBCA_WriteDataBuffer Enter \n");	
	DVTSTBCA_Flash_protect();	
	if(pStartAddr-DVT_FLASH_BUF>=DVT_FLASH_SIZE)
	{
		DEBUG_DVTCA("DVTSTBCA_WriteDataBuffer pStartAddr-(CDCA_U32)DVT_FLASH_BUF>=DVT_FLASH_SIZE \n");	
		return DVTCAERR_STB_DATA_LEN_ERROR;
	}

	FYF_API_Memcpy ((void *)pStartAddr, pData, *plDataLen);
	printf(">>>>>>>>>>>>>>>>>DVTSTBCA_WriteDataBuffer cur_task_id=%d\n",FYF_API_thread_getCurID());
#ifndef DVTCA_FLASH_CHECK_WRITE
	DVTSTBCA_IPI_WriteBuffer();
#else
	dvtca_write_enable = FYF_TRUE;
#endif
	DVTSTBCA_Flash_unprotect();
	DEBUG_DVTCA("DVTSTBCA_WriteDataBuffer End \n");
	return DVTCA_OK;
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void DVTSTBCA_API_flash_thread(void *pvParam)
{
	while(1)
	{
		DVTSTBCA_Flash_protect();
		if(FYF_TRUE == dvtca_write_enable)
		{
			DVTSTBCA_IPI_WriteBuffer();
			dvtca_write_enable = FYF_FALSE;
		}
		DVTSTBCA_Flash_unprotect();
		FYF_API_thread_sleep(DVTCA_FLASHWRITE_INTERVAL);
	}
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void DVTSTBCA_API_InitFlash(void)
{
#ifdef DVTCA_FLASH_CHECK_WRITE
	dvtca_flash_semaphore= FYF_API_semaphore_create("fla",1,FYF_THREAD_WAIT_PRIO);
	
	FYF_API_thread_create("fla",DVTSTBCA_API_flash_thread,FYF_NULL, 15, 10 * 1024);
#endif
}
/**********************************************************************************
���ܣ�������е�eeprom�洢�ռ�д��Ϣ��
������
plDataLen:				����ΪҪд�����ݵĳ��ȣ����Ϊд���ʵ�ʳ��ȡ�
pData:					Ҫд�����ݡ�           
**********************************************************************************/
HRESULT DVTSTBCA_SaveDataToEeprom(long * plDataLen, const BYTE * pData)
{
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;

	if(* plDataLen > 80)
	{
		* plDataLen = 80;
	}

	fyf_e2p_ioctl_param.buffer = pData;
	fyf_e2p_ioctl_param.bufferSize = *plDataLen;
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_WRITE_EX, EEPROM_DVTCA_FLASH_ADDR, (BU32)&fyf_e2p_ioctl_param);

	return DVTCA_OK;	
}

/**********************************************************************************
���ܣ���ȡ�����ڻ�����eeprom�е���Ϣ��
������
plDataLen:				����ΪҪ��ȡ�������ֵ�����Ϊʵ�ʶ�ȡ�Ĵ�С��
pData:					���������ݡ�                                        
**********************************************************************************/
HRESULT DVTSTBCA_GetDataFromEeprom(long * plDataLen, BYTE * pData)
{
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	
	if(* plDataLen > 80)
	{
		* plDataLen = 80;
	}

	fyf_e2p_ioctl_param.buffer = pData;
	fyf_e2p_ioctl_param.bufferSize = *plDataLen;
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_READ_EX, EEPROM_DVTCA_FLASH_ADDR, (BU32)&fyf_e2p_ioctl_param);

	return DVTCA_OK;	
}



