/**********************************************************************************
 File Name:   ca_dvt_sto.c

 Description: STB提供给CA调用的接口 

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
BU08 dvtca_flash_cur_block;		/* 目前数据缓冲区与FLASH对应,0xaa代表与第一块对应,0xbb代表与第二块对应 */
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
                获得机顶盒分配给CAS的存储空间的起点地址和大小Bytes
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
	else	/* CA第一次使用FLASH */
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
                       读取保存在机顶盒flash中的信息
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
                         将信息保存到机顶盒flash中
**********************************************************************************/
void DVTSTBCA_IPI_WriteBuffer(void)
{
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	
	FYF_FLS_IOCTL_param.buffer = DVT_FLASH_BUF;
	FYF_FLS_IOCTL_param.bufferSize = DVT_FLASH_SIZE;	
	if(0xaa == dvtca_flash_cur_block)/* 将数据保存到第二块 */
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_CABK, (BU32)&FYF_FLS_IOCTL_param);
		dvtca_flash_cur_block = 0xbb;
	}
	else if(0xbb == dvtca_flash_cur_block)/* 将数据保存到第一块 */
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
功能：向机顶盒的eeprom存储空间写信息。
参数：
plDataLen:				输入为要写的数据的长度；输出为写入的实际长度。
pData:					要写的数据。           
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
功能：读取保存在机顶盒eeprom中的信息。
参数：
plDataLen:				输入为要读取的最长数据值；输出为实际读取的大小。
pData:					存放输出数据。                                        
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



