
/*======================================================================

 File Name:tp205_cpi.c

 Description:TP205 config programming interface

 History:

======================================================================*/
/*----------------------------------------------------------------------
	Include Files
----------------------------------------------------------------------*/
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
	
#include "hi_stb_api.h"
#include "hi_flashgal.h"
#include "fyf_def.h"
#include "pse_205_api.h"


#define E2P_RST 	(11) //GPIO1[3]  
#define E2P_CLK 	(12) //GPIO1[4]  
#define E2P_Dat 	(13) //GPIO1[5]  
//#define E2P_Dout (5) //GPIO1[5]  
/*----------------------------------------------------------------------
	Private functions
----------------------------------------------------------------------*/
//static void PSE_205_CPI_IoInit(void);

static void PSE_205_CPI_SetRst(PSE_205_PioLevel_e PinLevel);
static void PSE_205_CPI_SetDat(PSE_205_PioLevel_e PinLevel);
static void PSE_205_CPI_SetClk(PSE_205_PioLevel_e PinLevel);
static PSE_205_PioLevel_e PSE_205_CPI_GetDat(void);
static void PSE_205_CPI_CfgDatDir(PSE_205_PioDir_e DataPinDirection);
static void PSE_205_CPI_SemInit(void);
static void PSE_205_CPI_SemSigl(void);
static void PSE_205_CPI_SemWait(void);
static void PSE_205_CPI_msDelay(unsigned char ms);


/*----------------------------------------------------------------------

	Description:PIO init and config

	Parameters :None

----------------------------------------------------------------------*/
void PSE_205_API_IoInit(void)
{
	int s32Ret = 0;
//	HI_UNF_GPIO_Open();

	s32Ret = HI_UNF_GPIO_DirSet_Bit(E2P_RST,0);
	if (HI_SUCCESS != s32Ret)
	{
	 printf("(%d):PSE_205_API_IoInit Error,ret=0x%x\n", __LINE__, s32Ret);
	}	

	s32Ret = HI_UNF_GPIO_DirSet_Bit(E2P_CLK,0);
	if (HI_SUCCESS != s32Ret)
	{
	 printf("(%d):PSE_205_API_IoInit Error,ret=0x%x\n", __LINE__, s32Ret);
	}	

	s32Ret = HI_UNF_GPIO_DirSet_Bit(E2P_Dat,1);
	if (HI_SUCCESS != s32Ret)
	{
	 printf("(%d):PSE_205_API_IoInit Error,ret=0x%x\n", __LINE__, s32Ret);
	}	

/*
	HAL_GPIO_BIT_DIR_SET(TP205_CPI_PIO_CLK,HAL_GPIO_O_DIR);
	HAL_GPIO_BIT_DIR_SET(TP205_CPI_PIO_RST,HAL_GPIO_O_DIR);
	HAL_GPIO_BIT_DIR_SET(TP205_CPI_PIO_DAT,HAL_GPIO_O_DIR); //output
*/
}
/*----------------------------------------------------------------------

	Description:Set RST pin level

	Parameters :Pin level to set

----------------------------------------------------------------------*/
void PSE_205_CPI_SetRst(PSE_205_PioLevel_e PinLevel)
{
  int reg = 0;
  
	if(PinLevel==PSE_205_ENU_PIO_LEVEL_H)
		reg = HI_UNF_GPIO_Write_Bit(E2P_RST, 1);
	else
		reg = HI_UNF_GPIO_Write_Bit(E2P_RST, 0);
	if (HI_SUCCESS != reg)
	   {
	     printf("(%d):PSE_205_CPI_SetDat Error,ret=0x%x\n", __LINE__, reg);
	   }
	
}
/*----------------------------------------------------------------------

	Description:Set DAT pin level

	Parameters :Pin level to set

----------------------------------------------------------------------*/
void PSE_205_CPI_SetDat(PSE_205_PioLevel_e PinLevel)
{
 int reg ;
	if(PinLevel==PSE_205_ENU_PIO_LEVEL_H)
		reg = HI_UNF_GPIO_Write_Bit(E2P_Dat, 1);
	else
		reg = HI_UNF_GPIO_Write_Bit(E2P_Dat, 0);
	if (HI_SUCCESS != reg)
        {
            printf("(%d):PSE_205_CPI_SetDat Error,ret=0x%x\n", __LINE__, reg);
        }
}
/*----------------------------------------------------------------------

	Description:Set CLK pin level

	Parameters :Pin level to set

----------------------------------------------------------------------*/
void PSE_205_CPI_SetClk(PSE_205_PioLevel_e PinLevel)
{
	int reg = 0;
	if(PinLevel==PSE_205_ENU_PIO_LEVEL_H)
		reg = HI_UNF_GPIO_Write_Bit(E2P_CLK, 1);

	else
		reg = HI_UNF_GPIO_Write_Bit(E2P_CLK, 0);
	    if (HI_SUCCESS != reg)
        {
            printf("(%d):PSE_205_CPI_SetClk Error,ret=0x%x\n", __LINE__, reg);
        }
}
/*----------------------------------------------------------------------

	Description:Get DAT pin level

	Parameters :None

----------------------------------------------------------------------*/
PSE_205_PioLevel_e PSE_205_CPI_GetDat(void)
{
    HI_U32 bitval = 0;
	int reg=0;
	reg = HI_UNF_GPIO_Read_Bit(E2P_Dat, &bitval);
	if (reg != HI_SUCCESS)
	{
		printf("(%d):PSE_205_CPI_GetDat Error,ret=0x%x\n", __LINE__, reg);

	}
	if (bitval)
	{
		return (PSE_205_ENU_PIO_LEVEL_H);
	}
	else
	{
		return (PSE_205_ENU_PIO_LEVEL_L);
	}
}
/*----------------------------------------------------------------------

	Description:Config DAT pin direction

	Parameters :Pin Direction to set

----------------------------------------------------------------------*/
void PSE_205_CPI_CfgDatDir(PSE_205_PioDir_e DataPinDirection)
{
	int reg=0 ;
	if(DataPinDirection==PSE_205_ENU_PIO_DIR_IPT)/* ‰»Î*/
		reg = HI_UNF_GPIO_DirSet_Bit(E2P_Dat,HI_TRUE);
	else
		reg = HI_UNF_GPIO_DirSet_Bit(E2P_Dat,HI_FALSE);

	if (reg != HI_SUCCESS)
	{
		printf("(%d):PSE_205_CPI_GetDat Error,ret=0x%x\n", __LINE__, reg);

	}
	
}
void PSE_205_CPI_SemInit(void)
{
	return;
}
void PSE_205_CPI_SemSigl(void)
{
	return;
}
void PSE_205_CPI_SemWait(void)
{
	return;
}


/*----------------------------------------------------------------------

	Description:Delay several ms

	Parameters :None

----------------------------------------------------------------------*/
void PSE_205_CPI_msDelay(unsigned char ms)
{
	usleep(ms*1000);
}
/*----------------------------------------------------------------------

	Description:API init

	Parameters :PSE_205_Result_e

----------------------------------------------------------------------*/
PSE_205_Result_e PSE_205_API_Init(void)
{
	PSE_205_Result_e RetSuccess;
	PSE_205_IniPms_s IniPms;

	IniPms.SetRset= PSE_205_CPI_SetRst;
	IniPms.SetData= PSE_205_CPI_SetDat;
	IniPms.SetClck= PSE_205_CPI_SetClk;
	IniPms.GetData= PSE_205_CPI_GetDat;
	IniPms.DatDirt= PSE_205_CPI_CfgDatDir;
	IniPms.SemInit= PSE_205_CPI_SemInit;
	IniPms.SemSigl= PSE_205_CPI_SemSigl;
	IniPms.SemWait= PSE_205_CPI_SemWait;
	IniPms.msDelay= PSE_205_CPI_msDelay;

	RetSuccess = PSE_205_API_InitCpiPms(&IniPms);
	if (RetSuccess != PSE_205_ENU_RESULT_OK)
	{
		printf("PSE_205_API_Init failed!!!!!!!!!!!!!!!!\n");
	}
	printf("PSE_205_API_Init success \n ");
	return RetSuccess;
}


