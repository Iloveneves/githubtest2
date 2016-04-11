/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_unf_vid.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2009/1/16
Last Modified :
Description   : 
Function List :
History       :
 ******************************************************************************/
#ifndef __HI_UNF_COMMON_H__
#define __HI_UNF_COMMON_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"


#define HI_INVALID_HANDLE (-1)
typedef HI_S32 HI_UNF_VID_PLAYER_HANDLE;
typedef HI_S32 HI_UNF_AUD_PLAYER_HANDLE;

typedef enum hiUNF_MOD_ID_E
{
    HI_UNF_ID_DEMUX =0,
    HI_UNF_ID_AVPLAY = 1,
    HI_UNF_ID_SND = 2,
    HI_UNF_ID_VO = 3,
    HI_UNF_ID_PVR = 4,
    HI_UNF_ID_BUTT
} HI_UNF_MOD_ID_E;

typedef enum hiUNF_SAMPLE_RATE_E
{
    HI_UNF_SAMPLE_RATE_UNKNOWN=0,
    HI_UNF_SAMPLE_RATE_8K    = 8000,    /* 8K采样频率      */
    HI_UNF_SAMPLE_RATE_11K   = 11025,   /* 11.025K采样频率 */
    HI_UNF_SAMPLE_RATE_16K   = 16000,   /* 16K采样频率     */
    HI_UNF_SAMPLE_RATE_22K   = 22050,   /* 22.050K采样频率 */
    HI_UNF_SAMPLE_RATE_24K   = 24000,   /* 24K采样频率     */
    HI_UNF_SAMPLE_RATE_32K   = 32000,   /* 32K采样频率     */
    HI_UNF_SAMPLE_RATE_44K   = 44100,   /* 44.1K采样频率   */
    HI_UNF_SAMPLE_RATE_48K   = 48000,   /* 48K采样频率     */
    HI_UNF_SAMPLE_RATE_64K   = 64000,   /* 64K采样频率     */
    HI_UNF_SAMPLE_RATE_88K   = 88200,   /* 88.2K采样频率   */
    HI_UNF_SAMPLE_RATE_96K   = 96000,   /* 96K采样频率     */
    HI_UNF_SAMPLE_RATE_176K  = 176400,  /* 176.4K采样频率  */
    HI_UNF_SAMPLE_RATE_192K  = 192000,  /* 192K采样频率    */

    HI_UNF_SAMPLE_RATE_BUTT
}HI_UNF_SAMPLE_RATE_E;

typedef enum hiUNF_BIT_DEPTH_E
{
    HI_UNF_BIT_DEPTH_UNKNOWN =0,
    HI_UNF_BIT_DEPTH_8  = 8,    /* 8位采样位宽  */
    HI_UNF_BIT_DEPTH_16 = 16,   /* 16位采样位宽 */
    HI_UNF_BIT_DEPTH_18 = 18,   /* 18位采样位宽 */
    HI_UNF_BIT_DEPTH_20 = 20,   /* 20位采样位宽 */
    HI_UNF_BIT_DEPTH_24 = 24,   /* 24位采样位宽 */
    HI_UNF_BIT_DEPTH_32 = 32,   /* 32位采样位宽 */

    HI_UNF_BIT_DEPTH_BUTT
}HI_UNF_BIT_DEPTH_E;

typedef enum hiUNF_TRACK_MODE_E
{
    HI_UNF_TRACK_MODE_STEREO,
    HI_UNF_TRACK_MODE_DOUBLE_MONO,
    HI_UNF_TRACK_MODE_DOUBLE_LEFT,
    HI_UNF_TRACK_MODE_DOUBLE_RIGHT,
    HI_UNF_TRACK_MODE_BUTT
} HI_UNF_TRACK_MODE_E;

typedef enum hiUNF_ENC_FMT_E
{
    HI_UNF_ENC_FMT_UNKNOWN = 0,
    HI_UNF_ENC_FMT_PAL,         /* B D G H I PAL */
    HI_UNF_ENC_FMT_PAL_N,       /* (N)PAL        */
    HI_UNF_ENC_FMT_PAL_Nc,      /* (Nc)PAL       */
    HI_UNF_ENC_FMT_SECAM_SIN,
    HI_UNF_ENC_FMT_SECAM_COS,
    HI_UNF_ENC_FMT_NTSC,        /* (M)NTSC       */
    HI_UNF_ENC_FMT_NTSC_J,      /* NTSC-J        */
    HI_UNF_ENC_FMT_NTSC_PAL_M,  /* (M)PAL        */
    HI_UNF_ENC_FMT_AUTO,        /* 仅用于输入端自适应，对输出端无效 */
    HI_UNF_ENC_FMT_BUTT
}HI_UNF_ENC_FMT_E;

typedef enum hiUNF_VIDEO_SUB_STANDARD_E
{
    HI_UNF_VIDEO_SUB_STANDARD_UNKNOWN,
    HI_UNF_VIDEO_SUB_STANDARD_MPEG2_MPEG1,  /* MPEG2协议族可以兼容MPEG1 */
    HI_UNF_VIDEO_SUB_STANDARD_MPEG4_SHV,       
    HI_UNF_VIDEO_SUB_STANDARD_MPEG4_XVID,   /* MPEG4协议包含XVID分支    */
    HI_UNF_VIDEO_SUB_STANDARD_MPEG4_DIVX,

    HI_UNF_VIDEO_SUB_STANDARD_BUTT
}HI_UNF_VIDEO_SUB_STANDARD_E;

typedef enum hiUNF_ASPECT_RATIO_E
{
    HI_UNF_ASPECT_RATIO_AUTO,/*AI7D05231 j00140427 2009.04.30*/
    HI_UNF_ASPECT_RATIO_4TO3,
    HI_UNF_ASPECT_RATIO_16TO9,
    HI_UNF_ASPECT_RATIO_SQUARE,
    HI_UNF_ASPECT_RATIO_14TO9,
    HI_UNF_ASPECT_RATIO_221TO1,

    HI_UNF_ASPECT_RATIO_BUTT
}HI_UNF_ASPECT_RATIO_E;

typedef enum hiUNF_VIDEO_SAMPLE_TYPE_E
{
    HI_UNF_VIDEO_SAMPLE_TYPE_UNKNOWN,
    HI_UNF_VIDEO_SAMPLE_TYPE_PROGRESSIVE,
    HI_UNF_VIDEO_SAMPLE_TYPE_INTERLACE,
    HI_UNF_VIDEO_SAMPLE_TYPE_INFERED_PROGRESSIVE,
    HI_UNF_VIDEO_SAMPLE_TYPE_INFERED_INTERLACE,

    HI_UNF_VIDEO_SAMPLE_TYPE_BUTT
}HI_UNF_VIDEO_SAMPLE_TYPE_E;

/* 解码器支持的码流类型 */
typedef enum hiUNF_VCODEC_TYPE_E
{
    HI_UNF_VCODEC_TYPE_MPEG2,
    HI_UNF_VCODEC_TYPE_MPEG4,
    HI_UNF_VCODEC_TYPE_H264,

    HI_UNF_VCODEC_TYPE_BUTT
}HI_UNF_VCODEC_TYPE_E;

typedef struct hiUNF_VCODEC_STREAMINFO_S
{
    HI_UNF_VCODEC_TYPE_E        enVCodecType;   /* 码流类型            */
    HI_UNF_VIDEO_SUB_STANDARD_E enSubStandard;  /* 码流的协议分支      */
    HI_U32                      u32SubVersion;  /* 码流子协议版本号    */
    HI_U32                      u32Profile;     /* 码流的profile       */
    HI_U32                      u32Level;       /* 码流的level         */
    HI_UNF_ENC_FMT_E            enDisplayNorm;  /* 显示标准(P/N)       */
    HI_UNF_VIDEO_SAMPLE_TYPE_E  enSampleType;   /* 采样方式(逐行/隔行) */
    HI_UNF_ASPECT_RATIO_E       enAspectRatio;  /* 输出宽高比          */

    HI_U32 u32bps;            /* 码流的码率, Kbps                     */
    HI_U32 u32fpsInteger;     /* 码流的帧率的整数部分, fps            */
    HI_U32 u32fpsDecimal;     /* 码流的帧率的小数部分（保留3位）, fps */
    HI_U32 u32Width;          /* 解码图像宽 */
    HI_U32 u32Height;         /* 解码图像高 */
    HI_U32 u32DisplayWidth;   /* 显示图像宽 */
    HI_U32 u32DisplayHeight;  /* 显示图像高 */
    HI_U32 u32DisplayCenterX; /* 显示图像中心横坐标，以原始图像的左上角为坐标原点 */
    HI_U32 u32DisplayCenterY; /* 显示图像中心纵坐标，以原始图像的左上角为坐标原点 */
}HI_UNF_VCODEC_STREAMINFO_S, *HI_UNF_VCODEC_STREAMINFO_S_PTR;


typedef struct hiUNF_VIDEO_SEQUENCE_INFO_S
{
    HI_U32                      u32Height;
    HI_U32                      u32Width;
    HI_UNF_ASPECT_RATIO_E       enAspect;
    HI_UNF_VIDEO_SAMPLE_TYPE_E  enSampleType;
    HI_U32                      u32FrameRate;
    HI_U32                      u32BitRate;
    HI_UNF_VCODEC_TYPE_E        entype;
    HI_BOOL                     bIsLowDelay;
    HI_U32                      u32VBVBufferSize;
    HI_U32                      u32StreamID;
    HI_U32                      u32Profile;
    HI_U32                      u32Level;
    HI_UNF_ENC_FMT_E            enVideoFormat;

    /* Valid only if VIDEO_MPEGSTANDARD_E is VIDeo_MPEG_STANDARD_ISO_IEC_13818 */
    HI_U32                      u32FrameRateExtensionN; /*  2 bits */
    HI_U32                      u32FrameRateExtensionD; /*  5 bits */
}HI_UNF_VIDEO_SEQUENCE_INFO_S;

typedef enum hiUNF_VIDEO_BROADCAST_PROFILE_E
{
    HI_UNF_VIDEO_BROADCAST_DVB,
    HI_UNF_VIDEO_BROADCAST_DIRECTV,
    HI_UNF_VIDEO_BROADCAST_ATSC,
    HI_UNF_VIDEO_BROADCAST_DVD,
    HI_UNF_VIDEO_BROADCAST_ARIB,
    HI_UNF_VIDEO_BROADCAST_BUTT
}HI_UNF_VIDEO_BROADCAST_PROFILE_E;

typedef enum hiUNF_VIDEO_USER_DATA_POSITION_E
{
    HI_UNF_VIDEO_USER_DATA_POSITION_UNKNOWN,
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG2_SEQ,
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG2_GOP,
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG2_FRAME,
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG4_VSOS,
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG4_VSO,
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG4_VOL,
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG4_GOP,
    HI_UNF_VIDEO_USER_DATA_POSITION_H264_REG,
    HI_UNF_VIDEO_USER_DATA_POSITION_H264_UNREG,
    HI_UNF_VIDEO_USER_DATA_POSITION_BUTT
}HI_UNF_VIDEO_USER_DATA_POSITION_E;


typedef struct hiUNF_VIDEO_USERDATA_S
{
    HI_UNF_VIDEO_BROADCAST_PROFILE_E   enBroadcastProfile;   /*Broadcast profile of the user data.*/
    HI_UNF_VIDEO_USER_DATA_POSITION_E  enPositionInStream;   /*Position of the user data.*/
    HI_U64                             u64PTS;                /*Presentation time stamp (PTS) of the user data.*/
    HI_U32                             u32SeqCnt;          /*Previous sequence number of the user data.*/
    HI_U32                             u32SeqFrameCnt;     /*Previous image number of the user data.*/
    HI_U8                              *pu8Buffer;         /*Initial address of the memory that stores the user data.*/
    HI_U32                             u32Length;          /*Length of the user data, in the unit of byte.*/
    HI_BOOL                            bBufferOverflow;  /*Parameter indicating whether the user data exceeds MAX_USER_DATA_LEN.*/
}HI_UNF_VIDEO_USERDATA_S;

typedef enum hiUNF_VIDEO_FORMAT_E
{
    HI_UNF_FORMAT_YUV_SEMIPLANAR_422 = 0X10,
    HI_UNF_FORMAT_YUV_SEMIPLANAR_420 = 0X11,
    HI_UNF_FORMAT_YUV_SEMIPLANAR_BUTT
}HI_UNF_VIDEO_FORMAT_E;

typedef enum hiUNF_VIDEO_FRAME_TYPE_E
{
    HI_UNF_FRAME_TYPE_UNKNOWN,
    HI_UNF_FRAME_TYPE_I,
    HI_UNF_FRAME_TYPE_P,
    HI_UNF_FRAME_TYPE_B,
    HI_UNF_FRAME_TYPE_BUTT
}HI_UNF_VIDEO_FRAME_TYPE_E;

typedef enum hiUNF_VIDEO_FIELD_MODE_E
{
    HI_UNF_VIDEO_FIELD_ALL,
    HI_UNF_VIDEO_FIELD_TOP,
    HI_UNF_VIDEO_FIELD_BOTTOM,
    HI_UNF_VIDEO_FIELD_BUTT
}HI_UNF_VIDEO_FIELD_MODE_E;

typedef struct hiUNF_VIDEO_TIMECODE_S
{
    HI_U8   u8Hours;
    HI_U8   u8Minutes;
    HI_U8   u8Seconds;
    HI_U8   u8Frames;
    HI_BOOL bInterpolated;
}HI_UNF_VIDEO_TIMECODE_S;


typedef struct hiUNF_VO_FRAMEINFO_S
{
    HI_UNF_VIDEO_SEQUENCE_INFO_S     stSeqInfo;  /*Video sequence information, valid for only the current frame when the SeqCnt changes.*/
    HI_U32                           u32FrameIndex;  /*Frame index.*/
    HI_UNF_VIDEO_FORMAT_E            enVideoFormat; /*Video format.*/
    HI_UNF_ENC_FMT_E                 enDisplayNorm; /*Display standard, such as PAL or NTSC standard.*/
    HI_UNF_VIDEO_FRAME_TYPE_E        enFrameType;   /*Frame type, such as I frame, P frame, or B frame.*/
    HI_UNF_VIDEO_SAMPLE_TYPE_E       enSampleType;  /*Sampling type, such as progressive or interlaced sampling.*/
    HI_UNF_VIDEO_FIELD_MODE_E        enFieldMode;   /*Field mode, such as frame, top field, or bottom field.*/
    HI_BOOL                          bTopFieldFirst;/*Top field first or not.*/
    HI_UNF_ASPECT_RATIO_E            enAspectRatio; /*Output aspect ratio.*/
    HI_UNF_VIDEO_USERDATA_S          stUsrData[4];    /*User data array.*/
    HI_U32                           u32UsrDataNum;                     /*Amount of the valid user data, ranging from 0 to MAX_USER_DATA_NUM.*/
    HI_U32                           u32width;                 /*Width of the original image.*/
    HI_U32                           u32height;                /*Height of the original image.*/
    HI_U32                           u32DisplayWidth;          /*Width of the display image.*/
    HI_U32                           u32DisplayHeight;         /*Height of the displaye image.*/
    HI_U32                           u32DisplayCenterX;        /*Horizontal coordinate of the display image center (the upper left corner of the original image is the coordinate origin).*/
    HI_U32                           u32DisplayCenterY;        /*Vertical coordinate of the display image center (the upper left corner of the original image is the coordinate origin).*/
    HI_U32                           u32ErrorLevel;            /*Error percentage of a decoded image, value range: 0-100.*/
    HI_U32                           u32SeqCnt;                /*Sequence number. The first sequence number is 0 when the decoder is enabled, and each subsequent sequence number is incremented by 1. The number is reset to 0 when the maximum value is reached.*/
    HI_U32                           u32SeqFrameCnt;           /*Image number. The first image number of each sequence is 0, and the each subsequent number is incremented by 1.*/
    HI_U64                           u32Pts;                   /*Display PTS corresponding to a video frame.*/
    HI_UNF_VIDEO_TIMECODE_S          stTimeCode;     /*Time value of a video frame.*/

    HI_U32                           u32YAddr;        /*Data address of a Y component.*/
    HI_U32                           u32CAddr;        /*Data address of a C component.*/
    HI_U32                           u32YStride;      /*Stride of a Y component.*/
    HI_U32                           u32CStride;      /*Stride of a C component.*/
    HI_U32                           u32Repeat;        /*Time of displaying a video frame.*/
}HI_UNF_VO_FRAMEINFO_S;

typedef struct hiUNF_AO_FRAMEINFO_S{
    HI_U32                 u32FrameIndex;    /*Frame index.*/
    HI_UNF_SAMPLE_RATE_E   enSampleRate;    /*Sampling rate.*/  
    HI_UNF_BIT_DEPTH_E     enBitDepth;      /*Bit depth.*/

    HI_U32                 u32PcmLeftAddr;      /*PCM audio data address of the left channel.*/
    HI_U32                 u32PcmRightAddr;      /*PCM audio data address of the right channel.*/
    HI_U32                 u32Ac3RawLeftAddr;  
    HI_U32                 u32ac3RawRightAddr;  
    HI_U32                 u32Length;            /*Length of audio data.*/
    HI_U64                 u64Pts;            /*PTS corresponding to an audio frame.*/  
}HI_UNF_AO_FRAMEINFO_S;

typedef enum hiUNF_AVPLAY_STREAM_STATE_E{
    HI_UNF_AVPLAY_STREAM_STOP = 0,
    HI_UNF_AVPLAY_STREAM_START
}HI_UNF_AVPLAY_STREAM_STATE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_VID_ H*/
