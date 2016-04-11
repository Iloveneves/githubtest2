/***********************************************************************************
*             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName    : hi_video_type.h
* Description : data structure definition provide to user and driver
*
* History:
* Version   Date              Author     DefectNum    Description
* main\1    2007-5-13         t52067     NULL         Create this file.
***********************************************************************************/
#ifndef __HI_VIDEO_TYPE_H__
#define __HI_VIDEO_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_vo_type.h"
#include "hi_vd_type.h"


/* Video bitstream source defination */
typedef enum hiVID_STREAM_SOURCE_E
{
    VID_STREAM_SOURCE_DEMUX=0x0,
    VID_STREAM_SOURCE_MEM,
} VID_STREAM_SOURCE_E,*PTR_VID_STREAM_SOURCE_E;

typedef enum hiVID_DECODER_CONTROL_MODE_E
{
    MCE_CONTROL_MODE = 0x0,
    SOFTWARE_CONTROL_MODE = 0x1,
}VID_DECODER_CONTROL_MODE_E;

/* Video decoder state machine defination */
typedef enum hiVID_DECODER_STATE_E
{
    VID_DECODER_INITIAL = 0x0,
    VID_DECODER_RUNNING,
    VID_DECODER_PAUSED,
    VID_DECODER_STOPPED,
    VID_DECODER_FROZEN,
} VID_DECODER_STATE_E, *PTR_VID_DECODER_STATE_E;

typedef enum hiVID_DISPLAY_MODE_E
{
    VID_DISPLAY_MODE_LIVE = 0x0,
    VID_DISPLAY_MODE_PVR
}VID_DISPLAY_MODE_E;

/* pvr play states*/
typedef enum hiVID_PVR_PALY_STATE_E
{
    VID_PVR_PLAY_STATE_INVALID = 0,                         /* invalid */
    VID_PVR_PLAY_STATE_INIT,                            /* initial */
    VID_PVR_PLAY_STATE_PLAY,                            /* play */
    VID_PVR_PLAY_STATE_PAUSE,                           /* pause */
    VID_PVR_PLAY_STATE_FF,                              /* fast forward */
    VID_PVR_PLAY_STATE_FB,                              /* fast backward */
    VID_PVR_PLAY_STATE_SF,                              /* slow forward */
    VID_PVR_PLAY_STATE_STEPF,                           /* step forward, not supported now */
    VID_PVR_PLAY_STATE_STEPB,                           /* step back */
    VID_PVR_PLAY_STATE_STOP,                            /* stop */
    VID_PVR_PLAY_STATE_BUTT
}VID_PVR_PLAY_STATE_E;

/*Decode Frame type*/
typedef enum hiVID_DECODE_FRAME_TYPE
{
    VID_DECODE_ALL = 0,
    VID_DECODE_I,
    VID_DECODE_IP,
    VID_DECODE_BUIT
}VID_DECODE_FRAME_TYPE;
typedef enum hiVID_PVR_PLAY_SPEED_E
{
    VID_PVR_PLAY_SPEED_NORMAL            = 1024,
    VID_PVR_PLAY_SPEED_2X_FAST_FORWORD   = 2 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_4X_FAST_FORWORD_C   = 4 * VID_PVR_PLAY_SPEED_NORMAL + 1,
    VID_PVR_PLAY_SPEED_4X_FAST_FORWORD   = 4 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_8X_FAST_FORWORD   = 8 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_16X_FAST_FORWORD  = 16 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_32X_FAST_FORWORD  = 32 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_2X_FAST_BACKWORD  = -2 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_4X_FAST_BACKWORD  = -4 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_8X_FAST_BACKWORD  = -8 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_16X_FAST_BACKWORD = -16 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_32X_FAST_BACKWORD = -32 * VID_PVR_PLAY_SPEED_NORMAL,
    VID_PVR_PLAY_SPEED_2X_SLOW_FORWORD   = VID_PVR_PLAY_SPEED_NORMAL / 2,
    VID_PVR_PLAY_SPEED_4X_SLOW_FORWORD   = VID_PVR_PLAY_SPEED_NORMAL / 4,
    VID_PVR_PLAY_SPEED_8X_SLOW_FORWORD   = VID_PVR_PLAY_SPEED_NORMAL / 8,
    VID_PVR_PLAY_SPEED_16X_SLOW_FORWORD  = VID_PVR_PLAY_SPEED_NORMAL / 16,
    VID_PVR_PLAY_SPEED_32X_SLOW_FORWORD  = VID_PVR_PLAY_SPEED_NORMAL / 32,
    VID_PVR_PLAY_SPEED_2X_SLOW_BACKWORD  = VID_PVR_PLAY_SPEED_NORMAL / (-2),
    VID_PVR_PLAY_SPEED_4X_SLOW_BACKWORD  = VID_PVR_PLAY_SPEED_NORMAL / (-4),
    VID_PVR_PLAY_SPEED_8X_SLOW_BACKWORD  = VID_PVR_PLAY_SPEED_NORMAL / (-8),
    VID_PVR_PLAY_SPEED_16X_SLOW_BACKWORD = VID_PVR_PLAY_SPEED_NORMAL / (-16),
    VID_PVR_PLAY_SPEED_32X_SLOW_BACKWORD = VID_PVR_PLAY_SPEED_NORMAL / (-32),
}VID_PVR_PLAY_SPEED_E;

typedef enum hiVID_DISPLAY_RATE_E
{
    VID_DISPLAY_RATE_NORMAL = 0x0,
    VID_DISPLAY_SLOWLY_2TIME,
    VID_DISPLAY_SLOWLY_4TIME,
    VID_DISPLAY_SLOWLY_8TIME,
    VID_DISPLAY_SLOWLY_16TIME,
    VID_DISPLAY_SLOWLY_32TIME
}VID_DISPLAY_RATE_E;

typedef enum hiVID_FLUSH_MODE_E
{
    VID_FLUSH_ALL = 0x0,
    VID_FLUSH_ES_BUFFER = 0x1,
    VID_FLUSH_DISPLAY_BUFFER = 0x2,
}VID_FLUSH_MODE_E, *PTR_VID_FLUSH_MODE_E;

typedef struct hiVID_USERDATA_S
{
    HI_U8*  pStream;
    HI_U32  u32Bytes;
    HI_U32* pReadBytes;
    HI_U32  u32Timeout;
}VID_USERDATA_S, *PTR_VID_USERDATA_S;

typedef enum hiVID_ADDR_TYPE_E
{
  VIRTUAL_ADDRESS =  0x0,
  FLASH_ADDRESS
}VID_ADDR_TYPE_E;

typedef struct hiVID_WRITE_PARA_S
{
    HI_VOID* pSourceAddr;
    HI_U32 u32StreamLen;
    VID_ADDR_TYPE_E enAddrType;
    HI_U32 u32TimeOut;
}VID_WRITE_CONFIG_S, *PTR_VID_WRITE_CONFIG_S;

typedef struct hiVID_ERROR_FRAME_NUM_S
{
    HI_U32 *mosaicnum;
    HI_U32 *skipnum;
}VID_ERROR_FRAME_NUM_S;
/* Default output video */
#define VID_DEFAULT_OUTPUT_VIDEO_X    0x0
#define VID_DEFAULT_OUTPUT_VIDEO_Y    0x0
#define VID_DEFAULT_OUTPUT_VIDEO_WIDTHFACTOR      0x1
#define VID_DEFAULT_OUTPUT_VIDEO_HEIGHTFACTOR    0x1

/* Default screen width and height */
#define VID_PAL_SCREEN_WIDTH      720UL             /* 720 */
#define VID_PAL_SCREEN_HEIGHT     576UL             /* 576 */
#define VID_NTSC_SCREEN_WIDTH    720UL              /* 720 */
#define VID_NTSC_SCREEN_HEIGHT   480UL              /* 480 */
#define VID_DEFAULT_SCREEN_WIDTH      VID_PAL_SCREEN_WIDTH
#define VID_DEFAULT_SCREEN_HEIGHT     VID_PAL_SCREEN_HEIGHT
#define MAX_SCREEN_WIDTH          720UL
#define MAX_SCREEN_HEIGHT         576UL


/* Default window size */
#define VID_DEFAULT_WINDOW_X   0x0
#define VID_DEFAULT_WINDOW_Y   0x0
#define VID_DEFAULT_WINDOW_WIDTH    VID_DEFAULT_SCREEN_WIDTH            /* 720 */
#define VID_DEFAULT_WINDOW_HEIGHT   VID_DEFAULT_SCREEN_HEIGHT           /* 576 */

/* Frame type */
typedef enum hiVID_FRAME_TYPE_E
{
    VID_FRAME_TYPE_D = 0x0,
    VID_FRAME_TYPE_I = 0x1,
    VID_FRAME_TYPE_P = 0x2,
    VID_FRAME_TYPE_B = 0x3,
} VID_FRAME_TYPE_E, *PTR_VID_FRAME_TYPE_E;

/*frame structure type*/
typedef enum hiVID_PIC_STRUCTURE_E
{
    PICTURE_STRUCTURE_RESERVED=0x0,
    PICTURE_STRUCTURE_TOP_FIELD = 0x1,
    PICTURE_STRUCTURE_BOTTOM_FIELD = 0x2 ,
    PICTURE_STRUCTURE_FRAME = 0x3,
} VID_PIC_STRUCTURE_TYPE_E;

typedef enum hiVID_MPEG_STANDARD_E
{
    MPEG_NULL = 0x0,/*Init the mpeg layer flag.*/
    MPEG_1 = 0x1,
    MPEG_2 = 0x2,
} VID_MPEG_STANDARD_E;

typedef struct hiVID_SEQ_INFO_S
{
    HI_U32                  u32hSize;
    HI_U32                  u32vSize;
    HI_U32                  u32hDispSize;
    HI_U32                  u32vDispSize;
    HI_U32                  FrameRate;
    HI_U32                  SeqDispInfoValid;
    HI_U32                  u32PregresiveSeq;
    HI_U32                  u32IFrameNumInPvr;
    VD_ASPECT_RATIO_E       eAspectRatio;
    VO_ENCODE_FORMAT_E      eFormat;
    VID_MPEG_STANDARD_E     eMpegStandard;
}VID_SEQ_INFO_S,* PTR_VID_SEQ_INFO_S;

typedef struct hiVID_GOP_HEADER_S
{
    HI_U32                      u32ClosedGop;
}VID_GOP_INFO_S;

/*picture information*/
typedef struct hiVID_PIC_INFO_S
{
    VID_FRAME_TYPE_E            eCodingType;
    VID_PIC_STRUCTURE_TYPE_E    eStructureType;
    HI_U32                      u32TopFieldFirst;
    HI_U32                      u32RepeatFirstField;
    HI_U32                      u32ProgressiveFrame;
    HI_U32                      u32VerticalOffset1;
    HI_U32                      u32HorizontalOffset1;
    HI_U32                      u32VerticalOffset2;
    HI_U32                      u32HorizontalOffset2;
    HI_U32                      u32VerticalOffset3;
    HI_U32                      u32HorizontalOffset3;
    HI_U32                      u32DispSeq;
    HI_BOOL                     bDisplay;
}VID_PIC_INFO_S,PTR_VID_PIC_INFO_S;

typedef struct hiVIDEO_FRAME_INFO_S
{
    HI_CHAR                         listhead[8];

    HI_U32                          u32VirAddr;
    HI_U32                          u32YAddr;
    HI_U32                          u32UAddr;
    HI_U32                          u32VAddr;
    HI_U32                          u32Pitch;

    HI_U32                          u32PTS;
    HI_U32                          u32PtsValid;
    HI_U32                          u32ParserSliceErrorNum;
    HI_U32                          u32ParserStartcodeErrorNum;
    HI_U32                          u32DecodeStartcodeErrorNum;
    HI_U32                          u32DecodeSliceErrorNum;
    HI_U32                          u32DecodeMacoblockErrorNum;
    HI_U32                          u32PicQuality;
    HI_U32                          u32FramebufFlag;

    VID_SEQ_INFO_S                  sSeqInfo;
    VID_GOP_INFO_S                  sGopInfo;
    VID_PIC_INFO_S                  sPicInfo;
    HI_U32                          u32CmdState;
}VIDEO_FRAME_INFO_S;

/****************************api struct****************************************/
typedef struct hiVID_SET_PVR_MODE_S
{
    VID_PVR_PLAY_STATE_E enState;
    VID_PVR_PLAY_SPEED_E enSpeed;
}VID_SET_PVR_MODE_S;

typedef struct hiVID_GET_PVR_PLAY_TIME_S
{
    HI_U32 *pPCRTime;
    HI_U32 *pFrameNum;
}VID_GET_PVR_PLAY_TIME_S;


typedef struct hiVID_PESBUF_STATE_S
{
    HI_U32      u32Start;
    HI_U32      u32End;
    HI_U32      u32Write;
    HI_U32      u32Read;
}VID_PESBUF_STATE_S;

/*begin: added by tzg on 2009.04.29 */
/*
    增加一个定义上层获取底层解码状态的超级数据结构体
    以后凡是可以将应用希望知道的各个状态放到这里面来，
    供上层查询。
*/
typedef struct hiVID_VERSATILE_STATE_S
{
    VID_DECODER_STATE_E   enDecoderState;
    VID_PESBUF_STATE_S stBufState;
    HI_U32                        u32DecodeFrameNum;
    HI_U32                        u32DecodeIPFrameNum;
    HI_U32                        u32DispFrameNum;
}VID_VERSATILE_STATE_S;
/*end: added by tzg on 2009.04.29 */

/*begin: added by tzg on 2009.04.29 */
/*
    增加一个直接解码I 帧数据，并将解码后的数据存储到用户指定的缓冲区中，
    而不是送到视频层显示，这样做的好处是可以保证解码I帧的时候视频保持正常
    显示不用黑屏
*/
typedef struct hiVID_DECODE_IFRAME_S
{
    HI_U8  *             pu8Addr;               /*帧数据用户态虚拟地址*/
    HI_U32               u32BufSize;          /*帧数据大小，单位字节*/

    HI_U32                      u32StartAddr;  /*存储解码后I 帧图片的物理起始地址*/
    HI_U32                      u32Width;        /*解码后I 帧图片的宽度*/
    HI_U32                      u32Height;      /*解码后I 帧图片的高度*/
    HI_U32                      u32Stride;      /*解码后I 帧图片的跨距*/
    VD_CAPTURE_COLOR_FORMAT_E   eOutColorFormat;  /*解码后I 帧图片的颜色格式，通过内部TDE转换可以获取*/
    VD_CAPTURE_TYPE_E           eCaptureType;
    HI_S32                      s32TimeOut;
}VID_DECODE_IFRAME_S;
/*end: added by tzg on 2009.04.29 */

typedef struct hiVID_PES_BUF_S
{
    HI_VOID     *pStart;
    HI_U32      u32StartPhy;
    HI_U32      u32BufSize;
}VID_PES_BUF_S;


typedef struct hiVID_ERR_COVER_S
{
    HI_U32               u32ErrCover;   /*视频解码设备的输出帧错误隐藏门限，0:出现错误即不输出；100:不管错误比例全部输出*/
    HI_U32               u32RefCover;   /*视频解码设备的参考帧错误隐藏门限，0:出现错误即不参考，丢弃整个序列；100:不管错误比例全部用作参考*/
}VID_ERR_COVER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif

