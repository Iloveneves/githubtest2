/*********************************************************************************** 
*             Copyright 2008 - 2050, Hisilicon Tech. Co., Ltd. 
*                           ALL RIGHTS RESERVED 
* FileName    : hi_player_ioctl.h 
* Description : define ioctl command code of video module  
* 
* History: 
* Version   Date              Author     DefectNum    Description 
* main\1    2008-3-28         z67193     NULL         Create this file. 
***********************************************************************************/
#ifndef __HI_PLAYER_TYPE_H__
#define __HI_PLAYER_TYPE_H__

#include "hi_type.h"
#include "hi_jpg_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*同步模式*/
typedef enum hiPLAYER_SYNCMODE_E
{
    PLAYER_SYNC_AUDIO_MASTER = 0, /*以音频为基准*/
    PLAYER_SYNC_VIDEO_MASTER,     /*以视频为基准*/
    PLAYER_SYNC_BYPASS,           /*不同步*/
    PLAYER_SYNC_BUTT 
}PLAYER_SYNCMODE_E;

typedef enum hiPLAYER_PICSTATE_E
{
    PLAYER_PICSTATE_IDLE = 0,   /*空闲*/
    PLAYER_PICSTATE_DECODING,   /*解码中*/
    PLAYER_PICSTATE_QUEUE,      /*待显示*/
    PLAYER_PICSTATE_READY,      /*准备显示*/
    PLAYER_PICSTATE_DISPLAYING, /*显示中*/
    PLAYER_PICSTATE_BUTT 
}PLAYER_PICSTATE_E;


/*解码BUF信息*/
typedef struct hiPLAYER_VIDEOPICTURE_S 
{
    HI_U32          Pts;       /*PTS时间戳*/
    HI_U32          Cnt;
    HI_U32          YSize;
    HI_U32          CSize;
    HI_U32          handle;    /*buffer的handle*/
    HI_U32          FrameRate; /*帧率*/
    JPG_SURFACE_S   Surface;   /*Surface信息*/
}PLAYER_VIDEOPICTURE_S;


typedef struct hiPLAYER_ALLOC_BUF
{
    HI_U32              YSize;
    HI_U32              CSize;
    JPG_SURFACE_S       Surface;
    HI_U32              FrameNum;
}PLAYER_ALLOC_BUF;

/*输入源类型*/
typedef enum hiMP3_SRCTYPE_E
{
    MP3_SRCTYPE_FILE = 0, /*文件*/
    MP3_SRCTYPE_MEM,      /*内存*/
    MP3_SRCTYPE_STREAM,   /*流式*/
    MP3_SRCTYPE_BUTT
}MP3_SRCTYPE_E;

/*各种输入源详细信息，对于流式来说，该信息无效*/
typedef union hiMP3_SRCINFO_U
{
    struct 
    {
        HI_CHAR* pAddr;     /*内存指针地址*/        
        HI_U32   Length;    /*长度*/
    }Mp3Info;               /*输入源为内存块时需要的信息*/
    HI_CHAR *pFileName;     /*输入源为文件时需要的信息:文件名*/
}MP3_SRCINFO_U;

/*输入源描述信息*/
typedef struct hiMP3_SRCDESC_S
{
    MP3_SRCTYPE_E SrcType; /*输入源类型*/
    MP3_SRCINFO_U SrcInfo; /*输入源的详细信息*/
}MP3_SRCDESC_S;

typedef struct hiMP3_BUF_S
{
    HI_U8 * pu8Data;         /* 数据指针            */
    HI_U32  u32Size;         /* 长度                */
}MP3_BUF_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HI_PLAYER_TYPE_H__ */

