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
#ifndef  __FYF_ADEC_H_
#define __FYF_ADEC_H_
typedef enum
{
 	FYF_AV_SOURCE_DEMUX,
 	FYF_AV_SOURCE_MANUAL
}FYF_AV_SOURCE_TYPE_e;
#if 0
typedef enum
{
    FYF_AUDIO_CODEC_DEFAULT,              /* don’t assigned by iPanel Middleware*/
    FYF_AUDIO_CODEC_MPEG        = 0x3,    /* MPEG1/2, layer 1/2. */
    FYF_AUDIO_CODEC_MP3         = 0x4,    /* MPEG1/2, layer 3. */
    FYF_AUDIO_CODEC_AAC         = 0xF,    /* Advanced audio coding. Part of MPEG-4 */
    FYF_AUDIO_CODEC_AAC_PLUS    = 0x11,   /*AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE)*/
    FYF_AUDIO_CODEC_AC3         = 0x81,   /* Dolby Digital AC3 audio */
    FYF_AUDIO_CODEC_AC3_PLUS    = 0x6     /* Dolby Digital Plus (AC3+ or DDP) audio */
}FYF_ADEC_ADUIO_FORMAT_e;
#endif

typedef enum  
{
    FYF_ACODEC_TYPE_AAC,
    FYF_ACODEC_TYPE_AAC_RAW,
    FYF_ACODEC_TYPE_MP3,
    FYF_ACODEC_TYPE_AC3,

    FYF_ACODEC_TYPE_BUTT
}FYF_ACODEC_FORMAT_e;

typedef enum
{
	FYF_DISABLE,
	FYF_ENABLE
}FYF_SWITCH_e;
typedef enum
{
    FYF_AUDIO_MODE_STEREO            = 0,   // 立体声
    FYF_AUDIO_MODE_LEFT_MONO         = 1,   // 左声道
    FYF_AUDIO_MODE_RIGHT_MONO        = 2,   // 右声道
    FYF_AUDIO_MODE_MIX_MONO          = 3,   // 左右声道混合
    FYF_AUDIO_MODE_STEREO_REVERSE    = 4    // 立体声，左右声道反转
}FYF_ADEC_CHANNEL_OUT_MODE_e;
typedef enum
{
    FYF_ADEC_SET_SOURCE = 1,
    FYF_ADEC_START,
    FYF_ADEC_STOP,
    FYF_ADEC_PAUSE,
    FYF_ADEC_RESUME,
    FYF_ADEC_CLEAR,
    FYF_ADEC_SYNCHRONIZE,
    FYF_ADEC_SET_CHANNEL_MODE,
    FYF_ADEC_GET_CHANNEL_MODE,
    FYF_ADEC_SET_MUTE,
    FYF_ADEC_GET_MUTE,
    FYF_ADEC_SET_PASS_THROUGH,
    FYF_ADEC_SET_VOLUME,
    FYF_ADEC_GET_BUFFER_RATE,
    FYF_ADEC_STREAM_INPUT		/* 判断有没有音频流输入 */
}FYF_ADEC_IOCTL_e;
typedef struct
{
	BU32 (*FYF_CPI_adec_open)(void);
	BS32 (*FYF_CPI_adec_close)(BU32 decoder);
	BS32 (*FYF_CPI_adec_ioctl)(BU32 decoder, FYF_ADEC_IOCTL_e op, void *arg);
}FYF_ADEC_PORTING;
/******************************************************************************/
/*Description: register adec                                                 */
/*Input      : porting													      */
/*Output     : no                                                             */
/*Return     : no                                                             */
/******************************************************************************/
void FYF_API_adec_register(FYF_ADEC_PORTING *porting);
/******************************************************************************/
/*Description: adec open                                                      */
/*Input      : no               										      */
/*Output     : no                                                             */
/*Return     : >0 success, 0 failure                                          */
/******************************************************************************/
BU32 FYF_API_adec_open(void);
/******************************************************************************/
/*Description: adec close                                                     */
/*Input      : decoder            										      */
/*Output     : no                                                             */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_adec_close(BU32 decoder);
/******************************************************************************/
/*Description: adec ioctl                                                     */
/*Input      : decoder,op          										      */
/*Output     : arg                                                            */
/*Return     : FYF_OK: success, FYF_ERR: failure                              */
/******************************************************************************/
BS32 FYF_API_adec_ioctl(BU32 decoder, FYF_ADEC_IOCTL_e op, void *arg);
void FYF_API_dev_register(void);
#endif


