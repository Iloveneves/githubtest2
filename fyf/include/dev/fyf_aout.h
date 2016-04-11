/******************************************************************************/
/*    Copyright (c) 2008 Embedded Internet Solutions, Inc                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by 	          */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author szhy 2008/09/26                                           	  */
/******************************************************************************/
#ifndef  __FYF_AOUT_H_
#define __FYF_AOUT_H_
typedef enum
{
    FYF_AOUT_SET_OUTPUT = 1,
    FYF_AOUT_SET_VOLUME,
	FYF_AOUT_GET_VOLUME,
    FYF_AOUT_SET_BALANCE
}FYF_AOUT_IOCTL_e;
typedef enum
{
    FYF_AOUT_DEVICE_ANALOG_STERO = 0x01, // 模拟立体声输出
    FYF_AOUT_DEVICE_ANALOG_MUTI  = 0x02, // 模拟多声道输出
    FYF_AOUT_DEVICE_SPDIF        = 0x04, // S/PDIF输出
    FYF_AOUT_DEVICE_HDMI         = 0x08, // HDMI输出
    FYF_AOUT_DEVICE_I2S          = 0x10, // I2S输出
    FYF_AOUT_DEVICE_ALL          = 0xff  // 所有端口
}FYF_AOUT_DEVICE_e;
typedef struct
{
	BU32 (*FYF_CPI_aout_open)(void);
	BS32 (*FYF_CPI_aout_close)(BU32 handle);
	BS32 (*FYF_CPI_aout_ioctl)(BU32 handle, FYF_AOUT_IOCTL_e op, void *arg);
}FYF_AOUT_PORTING;
/******************************************************************************/
/*Description: register aout                                                 */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_aout_register(FYF_AOUT_PORTING *porting);
/******************************************************************************/
/*Description: aout open                                                      */
/*Input      : no               										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_API_aout_open(void);
/******************************************************************************/
/*Description: aout close                                                     */
/*Input      : handle            										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_aout_close(BU32 handle);
/******************************************************************************/
/*Description: aout ioctl                                                     */
/*Input      : handle,op          										      */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_aout_ioctl(BU32 handle, FYF_AOUT_IOCTL_e op, void *arg);
#endif



