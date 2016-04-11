/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/10/10                                           	  */
/******************************************************************************/
#ifndef __FYF_E2P_H_
#define __FYF_E2P_H_ 
typedef enum
{
	FYF_E2P_IOCTL_SEEK,
	FYF_E2P_IOCTL_WRITE,
	FYF_E2P_IOCTL_READ,
	FYF_E2P_IOCTL_WRITE_EX,
	FYF_E2P_IOCTL_READ_EX
} FYF_E2P_IOCTL_e;
typedef struct
{
	PU08 buffer;
	BU32 bufferSize;
}FYF_E2P_IOCTL_PARAM_s;
typedef struct
{
	BS32 (*FYF_CPI_e2p_open)(const char * filename, BU32 size);
	BS32 (*FYF_CPI_e2p_write)(const void * buffer, BU32 size, BU32 count, BS32 handle);
	BU32 (*FYF_CPI_e2p_read)(void *buffer, BU32 size, BU32 count, BS32 handle); 
	BU32 (*FYF_CPI_e2p_ioctl)(FYF_E2P_IOCTL_e type, BU32 para1, BU32 para2);           
}FYF_E2P_PORTING;
/******************************************************************************/
/*Description: register eeprom                                                */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_e2p_register(FYF_E2P_PORTING *porting);
/******************************************************************************/
/*Description: open eeprom handle                                             */
/*Input      : size														      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_API_e2p_open(const char * filename, BU32 size);
/******************************************************************************/
/*Description: write data to eeprom                                           */
/*Input      : buffer,size,count,handle									      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_API_e2p_write(const void * buffer, BU32 size, BU32 count, BS32 handle);
/******************************************************************************/
/*Description: read data from eeprom                                          */
/*Input      : size,count,handle    									      */
/*Output     : buffer                                                         */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_API_e2p_read(void *buffer, BU32 size, BU32 count, BS32 handle);
/******************************************************************************/
/*Description: eeprom ioctl                                                   */
/*Input      : 					    									      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_API_e2p_ioctl(FYF_E2P_IOCTL_e type, BU32 para1, BU32 para2);
#endif



