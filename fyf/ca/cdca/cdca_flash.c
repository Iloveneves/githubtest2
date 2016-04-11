#include "CDCAS30.h"
#include "fyf_com.h"
#define CDCA_FLASH_SIZE  			(64*1024)
#ifdef BOX_ENCRYPT_E2PROM
#define EEPROM_CDCA_FLASH_FLAG_ADDR (47)
#else
#define EEPROM_CDCA_FLASH_FLAG_ADDR (47)//(121)
#endif
#if 1
#define DEBUG_CDCA //printf
#else
#define DEBUG_CDCA(...) 
#endif
#define CDCA_FLASH_CHECK_WRITE
#define CDCA_FLASHWRITE_INTERVAL 1000
BU08 cdca_flash_buf[CDCA_FLASH_SIZE];	/* 数据缓冲区 */
BU08 cdca_flash_cur_block;		/* 目前数据缓冲区与FLASH对应,0xaa代表与第一块对应,0xbb代表与第二块对应 */
BU08 cdca_write_enable = FYF_FALSE;
static BU32 cdca_flash_semaphore=0;
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_Flash_protect(void)
{
#ifdef 	CDCA_FLASH_CHECK_WRITE
	FYF_API_semaphore_wait(cdca_flash_semaphore, FYF_WAIT_FOREVER);
#endif
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_Flash_unprotect(void)
{
#ifdef 	CDCA_FLASH_CHECK_WRITE
	FYF_API_semaphore_release(cdca_flash_semaphore);
#endif
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_GetBuffer (CDCA_U32 * pdwStartAddr, CDCA_U32 * pdwSize) 
{
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;

	DEBUG_CDCA("CDSTBCA_GetBuffer Enter \n");
	CDSTBCA_Flash_protect();
	fyf_e2p_ioctl_param.buffer = &cdca_flash_cur_block;
	fyf_e2p_ioctl_param.bufferSize = 1;
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_READ_EX, EEPROM_CDCA_FLASH_FLAG_ADDR, (BU32)&fyf_e2p_ioctl_param);
	DEBUG_CDCA("CDSTBCA_GetBuffer cdca_flash_cur_block = %d\n",cdca_flash_cur_block);

	FYF_FLS_IOCTL_param.buffer = cdca_flash_buf;
	FYF_FLS_IOCTL_param.bufferSize = CDCA_FLASH_SIZE;
	if(0xaa == cdca_flash_cur_block)
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_CA, (BU32)&FYF_FLS_IOCTL_param);
	}
	else if(0xbb == cdca_flash_cur_block)
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_READ, FYF_FLASH_DEVID_CABK, (BU32)&FYF_FLS_IOCTL_param);		
	}
	else	/* CA第一次使用FLASH */
	{
		cdca_flash_cur_block = 0xaa;
	}
	*pdwStartAddr = (CDCA_U32)cdca_flash_buf;
	*pdwSize = CDCA_FLASH_SIZE;
	CDSTBCA_Flash_unprotect();
	DEBUG_CDCA("CDSTBCA_GetBuffer End \n");
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_ReadBuffer (CDCA_U32 dwStartAddr, CDCA_U8 * pbyData, CDCA_U32 * pdwLen)
{
	DEBUG_CDCA("CDSTBCA_ReadBuffer Enter \n");
	CDSTBCA_Flash_protect();
	FYF_API_Memcpy (pbyData, (void *)dwStartAddr, *pdwLen);
	CDSTBCA_Flash_unprotect();
	DEBUG_CDCA("CDSTBCA_ReadBuffer End \n");
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_IPI_WriteBuffer(void)
{
	FYF_FLS_IOCTL_PARAM_s FYF_FLS_IOCTL_param;
	FYF_E2P_IOCTL_PARAM_s fyf_e2p_ioctl_param;
	
	FYF_FLS_IOCTL_param.buffer = cdca_flash_buf;
	FYF_FLS_IOCTL_param.bufferSize = CDCA_FLASH_SIZE;	
	if(0xaa == cdca_flash_cur_block)/* 将数据保存到第二块 */
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_CABK, (BU32)&FYF_FLS_IOCTL_param);
		cdca_flash_cur_block = 0xbb;
	}
	else if(0xbb == cdca_flash_cur_block)/* 将数据保存到第一块 */
	{
		FYF_API_fls_ioctl (FYF_FLS_IOCTL_WRITE, FYF_FLASH_DEVID_CA, (BU32)&FYF_FLS_IOCTL_param);
		cdca_flash_cur_block = 0xaa;
	}
	fyf_e2p_ioctl_param.buffer = &cdca_flash_cur_block;
	fyf_e2p_ioctl_param.bufferSize = 1;
	FYF_API_e2p_ioctl (FYF_E2P_IOCTL_WRITE_EX, EEPROM_CDCA_FLASH_FLAG_ADDR, (BU32)&fyf_e2p_ioctl_param);
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_WriteBuffer (CDCA_U32 dwStartAddr, const CDCA_U8 * pbyData, CDCA_U32 dwLen)
{
	DEBUG_CDCA("CDSTBCA_WriteBuffer Enter \n");	
	CDSTBCA_Flash_protect();
	if(dwStartAddr-(CDCA_U32)cdca_flash_buf>=CDCA_FLASH_SIZE)
	{
		DEBUG_CDCA("CDSTBCA_WriteBuffer dwStartAddr-(CDCA_U32)cdca_flash_buf>=CDCA_FLASH_SIZE \n");	
		return;
	}
	FYF_API_Memcpy ((void *)dwStartAddr, pbyData, dwLen);
	printf(">>>>>>>>>>>>>>>>>CDSTBCA_WriteBuffer cur_task_id=%d\n",FYF_API_thread_getCurID());
#ifndef CDCA_FLASH_CHECK_WRITE
	CDSTBCA_IPI_WriteBuffer();
#else
	cdca_write_enable = FYF_TRUE;
#endif
	CDSTBCA_Flash_unprotect();
	DEBUG_CDCA("CDSTBCA_WriteBuffer End \n");
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_API_flash_thread(void *pvParam)
{
	while(1)
	{
		CDSTBCA_Flash_protect();
		if(FYF_TRUE == cdca_write_enable)
		{
			CDSTBCA_IPI_WriteBuffer();
			cdca_write_enable = FYF_FALSE;
		}
		CDSTBCA_Flash_unprotect();
		FYF_API_thread_sleep(CDCA_FLASHWRITE_INTERVAL);
	}
}
/*-------------------------------------------------------------------------------

	Description:  

	Paramenters:  @para 
	
	Return:       None  

-------------------------------------------------------------------------------*/
void CDSTBCA_API_InitFlash(void)
{
#ifdef CDCA_FLASH_CHECK_WRITE
	cdca_flash_semaphore= FYF_API_semaphore_create("fla",1,FYF_THREAD_WAIT_PRIO);
	
	FYF_API_thread_create("fla",CDSTBCA_API_flash_thread,FYF_NULL, 15, 10 * 1024);
#endif
}

