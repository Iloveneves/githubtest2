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
#ifndef  __FYF_VOUT_H_
#define __FYF_VOUT_H_
typedef enum
{
    FYF_VIDEO_OUTPUT_CVBS    = 1<<0,
    FYF_VIDEO_OUTPUT_SVIDEO  = 1<<1,
    FYF_VIDEO_OUTPUT_RGB     = 1<<2,
    FYF_VIDEO_OUTPUT_YPBPR   = 1<<3,
    FYF_VIDEO_OUTPUT_HDMI    = 1<<4,
    FYF_VIDEO_OUTPUT_YUV     = 1<<5
}FYF_VOUT_VIDEO_OUTPUT_e;
typedef enum
{
    FYF_VOUT_TVENC_NTSC,
    FYF_VOUT_TVENC_PAL,
    FYF_VOUT_TVENC_SECAM,
    FYF_VOUT_TVENC_AUTO
}FYF_VOUT_TVENC_MODE_e;
typedef enum
{
    FYF_VOUT_AR_FULL_SCREEN,
    FYF_VOUTS_AR_PILLARBOX,
    FYF_VOUT_AR_LETTERBOX,
    FYF_VOUT_AR_PAN_SCAN
}FYF_VOUT_AR_MODE_e;
typedef enum
{
    FYF_VOUT_SELECT_DEV = 1,
    FYF_VOUT_ENABLE_DEV,
    FYF_VOUT_SET_MODE,
    FYF_VOUT_SET_VISABLE,
    FYF_VOUT_SET_ASPECT_RATIO,
    FYF_VOUT_SET_WIN_LOCATION,
    FYF_VOUT_SET_WIN_TRANSPARENT,
    FYF_VOUT_SET_CONTRAST,
    FYF_VOUT_SET_HUE,
    FYF_VOUT_SET_BRIGHTNESS,
    FYF_VOUT_SET_SATURATION,
    FYF_VOUT_SET_SHARPNESS,
    FYF_VOUT_SET_RATIO
}FYF_VOUT_IOCTL_e;
typedef enum
{
	FYF_VOUT_SET_AR_16TO9,
	FYF_VOUT_SET_AR_4TO3	
}FYF_VOUT_SET_ASPECT_RATIO_e;
typedef struct
{
	BU32 (*FYF_CPI_vout_open)(void);
	BS32 (*FYF_CPI_vout_close)(BU32 display);
	BU32 (*FYF_CPI_vout_open_window)(BU32 display, BS32 type);
 	BS32 (*FYF_CPI_vout_close_window)(BU32 display, BU32 window);
	BS32 (*FYF_CPI_vout_ioctl)(BU32 display, FYF_VOUT_IOCTL_e op, void *arg);
}FYF_VOUT_PORTING;
/******************************************************************************/
/*Description: register vout                                                 */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_vout_register(FYF_VOUT_PORTING *porting);
/******************************************************************************/
/*Description: vout open                                                      */
/*Input      : no               										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_API_vout_open(void);
/******************************************************************************/
/*Description: vout close                                                     */
/*Input      : display            										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_vout_close(BU32 display);
/******************************************************************************/
/*Description: vout open window                                               */
/*Input      : display,type        										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_API_vout_open_window(BU32 display, BS32 type);
/******************************************************************************/
/*Description: vout close window                                              */
/*Input      : display,window      										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_vout_close_window(BU32 display, BU32 window);
/******************************************************************************/
/*Description: vout ioctl                                                     */
/*Input      : display,op          										      */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_vout_ioctl(BU32 display, FYF_VOUT_IOCTL_e op, void *arg);
#endif





