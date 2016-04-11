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
#ifndef  __FYF_VDEC_H_
#define __FYF_VDEC_H_
typedef enum
{
    FYF_VDEC_LAST_FRAME,
    FYF_VDEC_BLANK
}FYF_VDEC_STOP_MODE_e;
typedef enum
{
	FYF_VDEC_STATE_NORMAL, //正常解码
	FYF_VDEC_STATE_STOP	   //停止解码
}FYF_VDEC_STATE_e;

/* 解码器支持的码流类型 */
typedef enum 
{
    FYF_VCODEC_TYPE_MPEG2,
    FYF_VCODEC_TYPE_MPEG4,
    FYF_VCODEC_TYPE_H264,

    FYF_VCODEC_TYPE_BUTT
}FYF_VCODEC_TYPE_E;

typedef enum
{
    FYF_VDEC_SET_SOURCE = 1,
    FYF_VDEC_START,
    FYF_VDEC_STOP,
    FYF_VDEC_PAUSE,
    FYF_VDEC_RESUME,
    FYF_VDEC_CLEAR,
    FYF_VDEC_SYNCHRONIZE,
    FYF_VDEC_GET_BUFFER_RATE,
	FYF_VDEC_BITMAP,
	FYF_VDEC_STATE,		//解码器工作状态
	FYF_VDEC_STREAM_INPUT		/* 判断有没有视频流输入 */
}FYF_VDEC_IOCTL_e;
typedef struct
{
	BU32 (*FYF_CPI_vdec_open)(void);
	BS32 (*FYF_CPI_vdec_close)(BU32 decoder);
	BS32 (*FYF_CPI_vdec_ioctl)(BU32 decoder, FYF_VDEC_IOCTL_e op, void *arg);
}FYF_VDEC_PORTING;
/******************************************************************************/
/*Description: register vdec                                                 */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_vdec_register(FYF_VDEC_PORTING *porting);
/******************************************************************************/
/*Description: vdec open                                                      */
/*Input      : no               										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_API_vdec_open(void);
/******************************************************************************/
/*Description: vdec close                                                     */
/*Input      : decoder            										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_vdec_close(BU32 decoder);
/******************************************************************************/
/*Description: vdec ioctl                                                     */
/*Input      : decoder,op          										      */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_vdec_ioctl(BU32 decoder, FYF_VDEC_IOCTL_e op, void *arg);
#endif


