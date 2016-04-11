/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/09/28                                           	  */
/******************************************************************************/
#define __FYF_FLS_H_
#ifdef  __FYF_FLS_H_
typedef enum
{
	FYF_FLS_IOCTL_WRITE,
	FYF_FLS_IOCTL_READ,
	FYF_FLS_IOCTL_SEEK,
	FYF_FLS_IOCTL_ERASE
}FYF_FLS_IOCTL_e;
typedef struct
{
	BS32 (*FYF_CPI_fls_open)(const char * filename, BU32 size);
	BS32 (*FYF_CPI_fls_write)(const void * buffer, BU32 size, BU32 count, BS32 handle);
	BU32 (*FYF_CPI_fls_read)(void *buffer, BU32 size, BU32 count, BS32 handle); 
	BU32 (*FYF_CPI_fls_ioctl)(FYF_FLS_IOCTL_e type, BU32 para1, BU32 para2);         
}FYF_FLS_PORTING;
typedef struct
{
	PU08 buffer;
	BU32 bufferSize;
	BU32 operaSize;
}FYF_FLS_IOCTL_PARAM_s;
/******************************************************************************/
/*Description: register fls                                                   */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_fls_register(FYF_FLS_PORTING *porting);
/******************************************************************************/
/*Description: open flash handle                                              */
/*Input      : size														      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_API_fls_open(const char * filename, BU32 size);
/******************************************************************************/
/*Description: write data to flash                                            */
/*Input      : buffer,size,count,handle									      */
/*Output     : 		                                                          */
/*Return     :                                                                */
/******************************************************************************/
BS32 FYF_API_fls_write(const void * buffer, BU32 size, BU32 count, BS32 handle);
/******************************************************************************/
/*Description: read data from flash                                           */
/*Input      : size,count,handle    									      */
/*Output     : buffer                                                         */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_API_fls_read(void *buffer, BU32 size, BU32 count, BS32 handle);
/******************************************************************************/
/*Description:                                           */
/*Input      :    									      */
/*Output     :                                                        */
/*Return     :                                                                */
/******************************************************************************/
BU32 FYF_API_fls_ioctl(FYF_FLS_IOCTL_e type, BU32 para1, BU32 para2);
#endif
