/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author caiwende 2009/08/03                                           	  */
/******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <time.h>
#include <sys/mman.h>


#include "hi_stb_api.h"
#include "hi_flashgal.h"
#include "fyf_def.h"
#include "pse_205_api.h"

#define __FYF_FLS_PORTING_
#ifdef  __FYF_FLS_PORTING_
#include "dev/fyf_fls.h"

#define MTD_FLASH_BASE (0)
#define FLASH_BLOCK_SIZE (1024 * 64)

extern void PSE_205_API_IoInit(void);
extern PSE_205_Result_e PSE_205_API_Init(void);

static pthread_mutex_t flash_mutex;
/******************************************************************************/
/*Description: open flash handle                                              */
/*Input      : size														      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
void FYF_IPI_fls_init(void)
{
	pthread_mutex_init(&flash_mutex, NULL);
}


/******************************************************************************/
/*Description: open flash handle                                              */
/*Input      : size														      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_CPI_fls_open_porting(const char * filename, BU32 size)
{
	return FYF_OK;
}
/******************************************************************************/
/*Description: write data to flash                                            */
/*Input      : buffer,size,count,handle									      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_CPI_fls_write_porting(const void * buffer, BU32 size, BU32 count, BS32 handle)
{

	return FYF_OK;
}
/******************************************************************************/
/*Description: read data from flash                                           */
/*Input      : size,count,handle    									      */
/*Output     : buffer                                                         */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_fls_read_porting(void *buffer, BU32 size, BU32 count, BS32 handle)
{
	
	return FYF_OK;
}
/******************************************************************************/
/*Description:                                           */
/*Input      :    									      */
/*Output     :                                                        */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_fls_ioctl_write(BU32 para1, BU32 para2)
{
	FYF_FLS_IOCTL_PARAM_s * para = (FYF_FLS_IOCTL_PARAM_s *)para2;
	HI_FLASH_S mtdflash;
	BS32 ret;
	mtdflash.fd = -1;
	mtdflash.mtd_id = para1 + MTD_FLASH_BASE;

	if(mtdflash.mtd_id == 0 || mtdflash.mtd_id >= FYF_FLASH_DEVID_VODINFO)
		return ;
	
	if (HI_FAILURE == hi_flash_open(&mtdflash))
	{
		printf("FYF_CPI_fls_ioctl_write hi_flash_open error\n");
		return FYF_ERR;
	}
	printf("para->bufferSize = %d, para1 = %d write\n", para->bufferSize, para1);
#ifdef FLASH_8M

	ret = hi_flash_erase(&mtdflash, 0, (para->bufferSize - 1) / (64 * 1024) + 1);

#else
	ret = hi_flash_erase(&mtdflash, 0, (para->bufferSize - 1) / (128 * 1024) + 1);
#endif
	ret = hi_flash_write(&mtdflash, (BU08 *)(para->buffer), para->bufferSize);
	printf("ret = %d %d\n", ret, para->bufferSize);

	hi_flash_close(&mtdflash);


	return FYF_OK;
}
/******************************************************************************/
/*Description:                                           */
/*Input      :    									      */
/*Output     :                                                        */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_fls_ioctl_read(BU32 para1, BU32 para2)
{
	FYF_FLS_IOCTL_PARAM_s * para = (FYF_FLS_IOCTL_PARAM_s *)para2;
	HI_FLASH_S mtdflash;
	mtdflash.fd = -1;
	mtdflash.mtd_id = para1 + MTD_FLASH_BASE;
	if(mtdflash.mtd_id == 0 || mtdflash.mtd_id >= FYF_FLASH_DEVID_VODINFO)
		return ;
	
	if (HI_FAILURE == hi_flash_open(&mtdflash))
	{
		printf("FYF_CPI_fls_ioctl_read hi_flash_open error\n");
		return FYF_ERR;
	}

	printf("para->bufferSize = %d, para1 = %d read\n", para->bufferSize, para1);
	hi_flash_read(&mtdflash, (BU08 *)(para->buffer), para->bufferSize);

	hi_flash_close(&mtdflash);

	return FYF_OK;
}
/******************************************************************************/
/*Description:                                           */
/*Input      :    									      */
/*Output     :                                                        */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_fls_ioctl_erase(BU32 para1, BU32 para2)
{
	BU32 ret;
	HI_FLASH_S mtdflash;
	mtdflash.fd = -1;
	mtdflash.mtd_id = para1 + MTD_FLASH_BASE;
	//BU08 buf[1024 * 128];

	if(mtdflash.mtd_id == 0 || mtdflash.mtd_id >= FYF_FLASH_DEVID_VODINFO)
		return ;
	
	if (HI_FAILURE == hi_flash_open(&mtdflash))
	{
		printf("FYF_CPI_fls_ioctl_read hi_flash_open error\n");
		return FYF_ERR;
	}
	//memset(buf, 0, sizeof(buf));
	//ret = hi_flash_write(&mtdflash, buf, 1024 * 128);
	ret = hi_flash_erase(&mtdflash, 0, para2);
	printf("ret = %d\n", ret);

	hi_flash_close(&mtdflash);

	return FYF_OK;
}
/******************************************************************************/
/*Description:                                           */
/*Input      :    									      */
/*Output     :                                                        */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_fls_ioctl_porting(FYF_FLS_IOCTL_e type, BU32 para1, BU32 para2)
{
	pthread_mutex_lock(&flash_mutex);
	switch(type)
	{
	case FYF_FLS_IOCTL_WRITE:
		FYF_CPI_fls_ioctl_write (para1, para2);
		break;
	case FYF_FLS_IOCTL_READ:
		FYF_CPI_fls_ioctl_read (para1, para2);
		break;	
	case FYF_FLS_IOCTL_SEEK:
		break;
	case FYF_FLS_IOCTL_ERASE:
		FYF_CPI_fls_ioctl_erase (para1, para2);
		break;
	default:
		break;
	}
	pthread_mutex_unlock(&flash_mutex);
}
FYF_FLS_PORTING FYF_fls_porting=
{
	FYF_CPI_fls_open_porting,
	FYF_CPI_fls_write_porting,
	FYF_CPI_fls_read_porting,
	FYF_CPI_fls_ioctl_porting
};
#endif


#ifndef BOX_ENCRYPT_E2PROM

#define __FYF_E2P_PORTING_
#ifdef  __FYF_E2P_PORTING_
#include "dev/fyf_e2p.h"
static pthread_mutex_t e2p_mutex;

/******************************************************************************/
/*Description: open eeprom handle                                             */
/*Input      : size														      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_CPI_e2p_open_porting(const char * filename, BU32 size)
{
	
}
/******************************************************************************/
/*Description: write data to eeprom                                           */
/*Input      : buffer,size,count,handle									      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_CPI_e2p_write_porting(const void * buffer, BU32 size, BU32 count, BS32 handle)
{
//	SNVM_API_Set((unsigned char)handle+32,(unsigned char*)buffer,size*count,100);
	return FYF_OK;
}
/******************************************************************************/
/*Description: read data from eeprom                                          */
/*Input      : size,count,handle    									      */
/*Output     : buffer                                                         */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_e2p_read_porting(void *buffer, BU32 size, BU32 count, BS32 handle)
{
//	SNVM_API_Get((unsigned char)handle+32,(unsigned char*)buffer,size*count);
	return FYF_OK;
}
static BU32 fyf_e2p_offset = 0;
/******************************************************************************/
/*Description: eeprom ioctl                                                   */
/*Input      : 					    									      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_e2p_ioctl_porting(FYF_E2P_IOCTL_e type, BU32 para1, BU32 para2)
{
	unsigned char * buff;
	FYF_E2P_IOCTL_PARAM_s *e2p_ioctl_param;
	pthread_mutex_lock(&e2p_mutex);
	BS32 ret;

	switch(type)
	{
	case FYF_E2P_IOCTL_SEEK:
		break;
	case FYF_E2P_IOCTL_WRITE:
		break;
	case FYF_E2P_IOCTL_READ:
		break;
	case FYF_E2P_IOCTL_WRITE_EX:
		e2p_ioctl_param = (FYF_E2P_IOCTL_PARAM_s *)para2;
		ret = HI_UNF_E2PROM_Write(para1, e2p_ioctl_param->buffer, e2p_ioctl_param->bufferSize);
		break;
	case FYF_E2P_IOCTL_READ_EX:
		e2p_ioctl_param = (FYF_E2P_IOCTL_PARAM_s *)para2;
		ret = HI_UNF_E2PROM_Read(para1, e2p_ioctl_param->buffer, e2p_ioctl_param->bufferSize);
		break;
	default:		
		break;
	}
	
	if (ret != HI_SUCCESS)
	{
		printf("FYF_CPI_e2p_ioctl_porting errorcode %x\n", ret);
	}
	pthread_mutex_unlock(&e2p_mutex);
}
FYF_E2P_PORTING FYF_e2p_porting=
{
	FYF_CPI_e2p_open_porting,
	FYF_CPI_e2p_write_porting,
	FYF_CPI_e2p_read_porting,
	FYF_CPI_e2p_ioctl_porting
};
#endif
#else

//#ifdef  BOX_ENCRYPT_E2PROM
#include "dev/fyf_e2p.h"
static pthread_mutex_t e2p_mutex;

/******************************************************************************/
/*Description: open eeprom handle                                             */
/*Input      : size														      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_CPI_e2p_open_porting(const char * filename, BU32 size)
{
	
}
/******************************************************************************/
/*Description: write data to eeprom                                           */
/*Input      : buffer,size,count,handle									      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_CPI_e2p_write_porting(const void * buffer, BU32 size, BU32 count, BS32 handle)
{
//	SNVM_API_Set((unsigned char)handle+32,(unsigned char*)buffer,size*count,100);
	return FYF_OK;
}
/******************************************************************************/
/*Description: read data from eeprom                                          */
/*Input      : size,count,handle    									      */
/*Output     : buffer                                                         */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_e2p_read_porting(void *buffer, BU32 size, BU32 count, BS32 handle)
{
//	SNVM_API_Get((unsigned char)handle+32,(unsigned char*)buffer,size*count);
	return FYF_OK;
}
static BU32 fyf_e2p_offset = 0;
static BU08 fyf_e2p_data[256];
/******************************************************************************/
/*Description: eeprom ioctl                                                   */
/*Input      : 					    									      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_CPI_e2p_ioctl_porting(FYF_E2P_IOCTL_e type, BU32 para1, BU32 para2)
{
	unsigned int i;
	unsigned char * buff;
	FYF_E2P_IOCTL_PARAM_s *e2p_ioctl_param;
	pthread_mutex_lock(&e2p_mutex);
	PSE_205_Result_e ret = PSE_205_ENU_RESULT_OK;
	//printf("e2p type = %d, para1 = %d ,para2 = %d\n", type, para1, para2);

	switch(type)
	{
	case FYF_E2P_IOCTL_SEEK:
		break;
	case FYF_E2P_IOCTL_WRITE:
		break;
	case FYF_E2P_IOCTL_READ:
		break;
	case FYF_E2P_IOCTL_WRITE_EX:
		e2p_ioctl_param = (FYF_E2P_IOCTL_PARAM_s *)para2;
		printf(" -----------------------   write addr %d buffer = %d\n", para1,e2p_ioctl_param->bufferSize);
		for(i=0;i<e2p_ioctl_param->bufferSize;i++)
		{
			if(fyf_e2p_data[para1+i] != e2p_ioctl_param->buffer[i])
			{
				printf("write %d value 0x%x\n",para1+i,e2p_ioctl_param->buffer[i]);
				ret = PSE_205_API_SetMainArea((BU08)para1+i,e2p_ioctl_param->buffer+i,1);
				fyf_e2p_data[para1+i] = e2p_ioctl_param->buffer[i];
			}
		}
		break;
	case FYF_E2P_IOCTL_READ_EX:
		e2p_ioctl_param = (FYF_E2P_IOCTL_PARAM_s *)para2;
	    	printf(" -----------------------   read addr %d buffer = %d\n", para1,e2p_ioctl_param->bufferSize);
//		ret = PSE_205_API_GetMainArea((BU08)para1,e2p_ioctl_param->buffer, e2p_ioctl_param->bufferSize);
//		mempy(&fyf_e2p_data[para1],e2p_ioctl_param->buffer,e2p_ioctl_param->bufferSize);
		memcpy(e2p_ioctl_param->buffer,&fyf_e2p_data[para1],e2p_ioctl_param->bufferSize);
		break;
	default:		
		break;
	}
	
	if (ret != PSE_205_ENU_RESULT_OK)
	{
		printf("FYF_CPI_e2p_ioctl_porting errorcode %x\n", ret);
	}
	pthread_mutex_unlock(&e2p_mutex);
}
FYF_E2P_PORTING FYF_e2p_porting=
{
	FYF_CPI_e2p_open_porting,
	FYF_CPI_e2p_write_porting,
	FYF_CPI_e2p_read_porting,
	FYF_CPI_e2p_ioctl_porting
};
#endif



#define __FYF_SQL_PORTING_
#ifdef  __FYF_SQL_PORTING_
#include "sql/sqlite3_port.h"
#include "sys/fyf_system.h"
void  SQLITE3_porting_printf(char * inf)
{
	printf(inf);
}
 SQLITE3_port_s SQLITE3_porting=
{
	FYF_API_malloc,
	realloc,
	FYF_API_free,
	SQLITE3_porting_printf
};
#endif


#define __FYF_PANEL_PORTING_
#ifdef  __FYF_PANEL_PORTING_
#include "dev/fyf_panel.h"

#ifdef _HAISI_DEMO_

BU08 DigDisCode[] = {0x03, 0x9F, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09};
BU08 UCharDisCode[] = {
        0xFF, 0xFF, 0x63, 0xFF, 0x61,
        0x71, 0xFF, 0x91, 0xFF, 0xFF,
        0xFF, 0xE3, 0xFF, 0xFF, 0x03,
        0x31, 0xFF, 0x11,/*R*/ 0x49, 0xFF,
        0x83, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };

    //大写字母只能够显示'C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U'，从左至右分别对应此数组中的非0xFF值
BU08 LCharDisCode[] = {
        0xFF, 0xC1, 0xE5, 0x85, 0x21,
        0xFF, 0x09, 0xD1, 0xDF, 0xFF,
        0xFF, 0x9F, 0xFF, 0xD5, 0xC5,
        0x31, 0x19, 0xFF, 0x49, 0xE1/*t*/,
        0xC7, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };

#else
BU08 DigDisCode[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
BU08 UCharDisCode[] = {
        0xFF, 0xFF, 0x39, 0xFF, 0x79,
        0x71, 0xFF, 0x76, 0xFF, 0xFF,
        0xFF, 0x38, 0xFF, 0xFF, 0x3f,
        0x73, 0xFF, 0x77,/*R*/ 0x6d, 0xFF,
        0x3e, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };

    //大写字母只能够显示'C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U'，从左至右分别对应此数组中的非0xFF值
BU08 LCharDisCode[] = {
        0xFF, 0x7c, 0x58, 0x5e, 0x79,
        0xFF, 0x6f, 0x74, 0x30, 0xFF,
        0xFF, 0x38, 0xFF, 0x54, 0x5c,
        0x73, 0x67, 0xFF, 0x6d, 0x78,
        0x1c, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };
#endif    
/******************************************************************************/
/*Description: read key		                                                  */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_CPI_panel_read_key_porting(void)
{
	//return FPC_6X0_API_ReadKeys();
}
/******************************************************************************/
/*Description: show led		                                                  */
/*Input      : no														      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
BU32 FYF_CPI_panel_show_led_porting(BU08 StartLedIndex,PU08 DisData,BU08 Length)
{
//	printf("StartLedIndex = %d %s %d\n", StartLedIndex, DisData, Length);
	BU32 value = 0;
	BU32 i; 
#if 1
	if(DisData == 0)
		HI_UNF_LED_Display(0);
	else
	{	
		for (i = 0; i < Length; i++)
		{

			if (DisData[i] >= '0' && DisData[i] <= '9')
			{
			//	printf("i = 1\n");
				value |= DigDisCode[DisData[i] - 48] << (8 * i);
			}
			else if (DisData[i] >= 'A' && DisData[i] <= 'Z')
			{

				value |= UCharDisCode[DisData[i] - 65] << (8 * i);
			}
			else 
			{

				//DisData[i] -= 97;
				value |= LCharDisCode[DisData[i] - 97] << (8 * i);
			}
			//value = (DigDisCode[DisData[3]]<< 24) | (DigDisCode[DisData[2]] << 16 )
			//	| (DigDisCode[DisData[1]] << 8 ) | (UCharDisCode[DisData[0]]);
		}
	//	printf("chnid = %s\n", DisData);
		HI_UNF_LED_Display(value);
	}
#endif
	return FYF_OK;
}
FYF_PANEL_PORTING FYF_panel_porting=
{
	FYF_CPI_panel_read_key_porting,
	FYF_CPI_panel_show_led_porting	
};
#endif



//2009-11-27 cwd add
//添加串口操作的接口

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))  
#define PRD_UART_DBG(format, arg...)  printf( "%s:%s: " format , __FILE__ , __FUNCTION__, ## arg)
#define PRD_UART_ERR(format, arg...)  printf( "%s:%s: " format , __FILE__ , __FUNCTION__, ## arg)
#define PRD_UART_INFO(format, arg...)  printf( "%s:%s: " format , __FILE__ , __FUNCTION__, ## arg)

/*串口句柄*/
static BS32 g_uartfd = -1;

/*终端属性*/
static struct termios g_tOrgTermios;
/*速度*/
static HI_S32 g_iArrSpeed[] = { B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
	    B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
static HI_S32 g_iArrName[] = {115200, 38400,  19200,  9600,  4800,  2400,  1200,  300,
	    115200, 38400,  19200,  9600, 4800, 2400, 1200,  300, };
/*提示信息*/
HI_CHAR  SerialNumHint[64] =  "请尽快进行条码扫描" ;


/******************************************************************************
 * Description：相关属性设置
 * Input:
 * Output:
 * Return:           
 ******************************************************************************/
static BS32 FYF_IPI_Uart_SetOpt(BS32 fd, BS32 speed, BS32 bits, BU08 event, BU32 stop)
{
	struct termios Opt;
	BU32 size;
	BU32 i;
	
	if(tcgetattr(fd , &Opt) != 0)
	{
		PRD_UART_ERR("tcgetarrt  Error!\n");
		return HI_FAILURE;
	}

	Opt.c_lflag &=~(ECHO | ICANON | IEXTEN | ISIG | ECHOE);
	//Opt.c_iflag &=~( ISTRIP  );
	Opt.c_iflag &=~(BRKINT | ICRNL | INPCK  |IXON | IXOFF | IXANY | ISTRIP ); /*modified*/
	
	//设置字符大小
	Opt.c_cflag |= CLOCAL | CREAD;
	Opt.c_cflag &= ~(CSIZE | PARENB);
	
	//设置波特率
	size = ARRAY_SIZE(g_iArrSpeed);
	for ( i= 0;  i < size;  i++)
	{
		if (speed == g_iArrName[i])
   		{
    		cfsetispeed(&Opt, g_iArrSpeed[i]);
    		cfsetospeed(&Opt, g_iArrSpeed[i]);
			break;		
 		}
	}
	
	//设置数据位
	switch(bits)
	{
	case 7:
		Opt.c_cflag |= CS7;
		break;
	case 8:
		Opt.c_cflag |= CS8;
		break;
	default:
		Opt.c_cflag |= CS8;
		break;
		
	}
	//设置停止位
	if (stop == 1)
		Opt.c_cflag &= ~CSTOPB;	/*一位停止位*/
	else if (stop == 2)
		Opt.c_cflag |= CSTOPB;
	
	Opt.c_oflag &=~(OPOST | OCRNL);
	
	Opt.c_cc[VMIN] = 0;
	Opt.c_cc[VTIME] = 0;
	tcflush(fd,TCIFLUSH);
	if(tcsetattr(fd , TCSAFLUSH , &Opt) != 0)
	{
		PRD_UART_ERR("tcgetarrt  Error!\n");
		return HI_FAILURE;
	}
	return HI_SUCCESS;
}
static BU32 init = 0;
BS32 FYF_API_Uart_Open()
{
	HI_S32 ret;
	if (g_uartfd >= 0) return;
	//s_iUartfd = open( "/dev/ttyS000",O_RDWR | O_NONBLOCK );	                /*以非阻塞方式打开串口并且允许CTRL-C中止程序*/
	g_uartfd = open( "/dev/ttyAMA0",O_RDWR | O_NONBLOCK );	                /*以非阻塞方式打开串口并且允许CTRL-C中止程序*/
	//g_uartfd = open( "/dev/ttyAMA0",O_RDWR | O_NONBLOCK | O_NOCTTY );		/*以非阻塞方式打开串口并且不允许CTRL-C中止程序*/

	if(g_uartfd  < 0)
	{
		PRD_UART_ERR("open err\n");
		goto err0;
	}

	printf("g_uartfd = %d\n", g_uartfd);
	
	if(tcgetattr(g_uartfd, &g_tOrgTermios) != 0)
	{
		PRD_UART_ERR("tcgetattr err! \n");
		goto err1;
	}
	
	//FYF_IPI_Uart_SetSpeed(g_uartfd, 115200);
	ret = FYF_IPI_Uart_SetOpt(g_uartfd, 115200, 8, 'N', 1);
	if(ret != HI_SUCCESS)
	{
		PRD_UART_ERR("SetTTYRawMode err\n");
		goto err2;
	}
	
	return HI_SUCCESS;

err2:
	tcsetattr(g_uartfd,TCSAFLUSH,  &g_tOrgTermios);
err1:
	close(g_uartfd);
	g_uartfd = -1;
err0:
	return  HI_FAILURE;
}

BS32 FYF_API_Uart_Read(BU08 *pbBuffer, BU32 uSize)
{
	struct pollfd atUartPoll[1];
	HI_U32 uiTotal = 0;
	HI_U32 uiOffset = 0;
	HI_S32 iNumRead;
	HI_S32 times = 0;

	if(pbBuffer == 0 )
	{
		PRD_UART_ERR( "parameters pbBuffer == NULL error\n");
		return HI_FAILURE;
	}
	
	if(0 == uSize)
	{
		return HI_SUCCESS;
	}

	uiOffset = 0;
	uiTotal = uSize;
	atUartPoll[0].fd = g_uartfd;
	atUartPoll[0].events = POLLIN;

	
	poll(atUartPoll , 1 , 10000);		/*等待10秒钟*/
	if(atUartPoll[0].revents & POLLIN)
	{
		/*有数据到来*/
		while(uiTotal >0 && ++times <= 3)
		{
			iNumRead = read(g_uartfd, pbBuffer + uiOffset, uiTotal);
			if(iNumRead < 0)
			{
				PRD_UART_ERR("Read Err !\n");
				return  HI_FAILURE;
			}
			uiOffset += iNumRead;
			uiTotal -= iNumRead;
			printf("uiOffset = %d\n", uiOffset);
			if ((uiOffset == 12 && pbBuffer[10] == 0x0d && pbBuffer[11] == 0x0a) || (uiOffset == 28 && pbBuffer[26] == 0x0d && pbBuffer[27] == 0x0a))
			{
				break;
			}
		
			FYF_API_thread_sleep(100);
		}
	}
	else
	{
		if(errno == EINTR)
		{
			/*各种中断信号和错误信号*/
			PRD_UART_ERR("EINTR occurs \n" );
		}
		/*超时或者其它情况发生*/
		PRD_UART_DBG(" the actuall num read is %d   \n" , uiOffset);
		PRD_UART_DBG(" the left num read is %d   \n" , uiTotal);
		PRD_UART_DBG(" the want num read is %d   \n" , uSize);
	 	PRD_UART_DBG(" poll uart time out  \n");
		return 0;
	}

	PRD_UART_INFO(":sucess  :the actual  num read is %d  \n" , uiOffset);
 	return uiOffset;
}

BS32 FYF_API_Uart_Close()
{
	if (close(g_uartfd) != 0)
	{
		PRD_UART_ERR("close err\n");
	}
	g_uartfd = -1;
}


void FYF_IPI_e2p_init()
{
	HI_S32 s32Ret;
	
	 /*打开E2PROM模块*/
    s32Ret = HI_UNF_E2PROM_Open();
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }
#ifdef _HAISI_DEMO_
    /*设置E2PROM型号/使用的I2C号/在I2C总线上的地址*/
    s32Ret = HI_UNF_E2PROM_SetChipType(HI_UNF_E2PROM_CHIP_AT24C02);
#else
	s32Ret = HI_UNF_E2PROM_SetChipType(HI_UNF_E2PROM_CHIP_AT24C64);
#endif
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*设置E2PROM的I2C总线地址*/
    s32Ret = HI_UNF_E2PROM_SetAddr(0xA8);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

    /*设置E2PROM所使用的I2C通道号*/
    s32Ret = HI_UNF_E2PROM_SetI2cNum(1);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        goto ERR;
    }

	pthread_mutex_init(&e2p_mutex, NULL);
	return HI_SUCCESS;
	
ERR:
	HI_UNF_E2PROM_Close();
    printf("run e2prom demo failed\n");
    return s32Ret;
	
}


/******************************************************************************/
/*Description: register dev                                                   */
/*Input      : no                                                             */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_dev_register(void)
{
	BU32 index;
	
#ifdef  __FYF_FLS_PORTING_
	FYF_IPI_fls_init ();
	FYF_API_fls_register(&FYF_fls_porting);
#endif

#ifdef BOX_ENCRYPT_E2PROM
	PSE_205_API_IoInit();
	PSE_205_API_Init();
#else
	FYF_IPI_e2p_init();
#endif
	FYF_API_e2p_register(&FYF_e2p_porting);

#ifdef BOX_ENCRYPT_E2PROM
	PSE_205_API_GetMainArea(32,&(fyf_e2p_data[32]),224);
#endif

	FYF_API_panel_register(&FYF_panel_porting);

	SQLITE3_API_register_port(&SQLITE3_porting);
	SQLITE3_API_debug(0);
}




