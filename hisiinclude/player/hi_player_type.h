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


/*ͬ��ģʽ*/
typedef enum hiPLAYER_SYNCMODE_E
{
    PLAYER_SYNC_AUDIO_MASTER = 0, /*����ƵΪ��׼*/
    PLAYER_SYNC_VIDEO_MASTER,     /*����ƵΪ��׼*/
    PLAYER_SYNC_BYPASS,           /*��ͬ��*/
    PLAYER_SYNC_BUTT 
}PLAYER_SYNCMODE_E;

typedef enum hiPLAYER_PICSTATE_E
{
    PLAYER_PICSTATE_IDLE = 0,   /*����*/
    PLAYER_PICSTATE_DECODING,   /*������*/
    PLAYER_PICSTATE_QUEUE,      /*����ʾ*/
    PLAYER_PICSTATE_READY,      /*׼����ʾ*/
    PLAYER_PICSTATE_DISPLAYING, /*��ʾ��*/
    PLAYER_PICSTATE_BUTT 
}PLAYER_PICSTATE_E;


/*����BUF��Ϣ*/
typedef struct hiPLAYER_VIDEOPICTURE_S 
{
    HI_U32          Pts;       /*PTSʱ���*/
    HI_U32          Cnt;
    HI_U32          YSize;
    HI_U32          CSize;
    HI_U32          handle;    /*buffer��handle*/
    HI_U32          FrameRate; /*֡��*/
    JPG_SURFACE_S   Surface;   /*Surface��Ϣ*/
}PLAYER_VIDEOPICTURE_S;


typedef struct hiPLAYER_ALLOC_BUF
{
    HI_U32              YSize;
    HI_U32              CSize;
    JPG_SURFACE_S       Surface;
    HI_U32              FrameNum;
}PLAYER_ALLOC_BUF;

/*����Դ����*/
typedef enum hiMP3_SRCTYPE_E
{
    MP3_SRCTYPE_FILE = 0, /*�ļ�*/
    MP3_SRCTYPE_MEM,      /*�ڴ�*/
    MP3_SRCTYPE_STREAM,   /*��ʽ*/
    MP3_SRCTYPE_BUTT
}MP3_SRCTYPE_E;

/*��������Դ��ϸ��Ϣ��������ʽ��˵������Ϣ��Ч*/
typedef union hiMP3_SRCINFO_U
{
    struct 
    {
        HI_CHAR* pAddr;     /*�ڴ�ָ���ַ*/        
        HI_U32   Length;    /*����*/
    }Mp3Info;               /*����ԴΪ�ڴ��ʱ��Ҫ����Ϣ*/
    HI_CHAR *pFileName;     /*����ԴΪ�ļ�ʱ��Ҫ����Ϣ:�ļ���*/
}MP3_SRCINFO_U;

/*����Դ������Ϣ*/
typedef struct hiMP3_SRCDESC_S
{
    MP3_SRCTYPE_E SrcType; /*����Դ����*/
    MP3_SRCINFO_U SrcInfo; /*����Դ����ϸ��Ϣ*/
}MP3_SRCDESC_S;

typedef struct hiMP3_BUF_S
{
    HI_U8 * pu8Data;         /* ����ָ��            */
    HI_U32  u32Size;         /* ����                */
}MP3_BUF_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HI_PLAYER_TYPE_H__ */

