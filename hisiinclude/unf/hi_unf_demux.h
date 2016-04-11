/******************************************************************************

Copyright (C), 2004-2008, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_unf_demux.h
Version       : Initial 
Author        : Hisilicon multimedia software group
Created       : 2008-06-10
Last Modified :
Description   : Hi3560 DEMUX API Layer function declaration for linux
Function List :
History       :
* Version   Date         Author     DefectNum    Description
* main\1    2008-06-10   j40671     NULL         Create this file.
******************************************************************************/

#ifndef __HI_UNF_DEMUX_H__
#define __HI_UNF_DEMUX_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


#define DMX_FILTER_MAX_DEPTH 16

typedef HI_U32 DMX_CHAN_HANDLE;
typedef HI_U32 DMX_FILTER_HANDLE;
typedef HI_U32 DMX_KEY_HANDLE;

/*************************** Structure Definition ****************************/
/****************** DEMUX module Attr ************************/
typedef struct hiUNF_DMX_GLB_ATTR_S
{
	/*
    ** TSIģ�鹤��ʱ��: 
    ** 0: ѡ��ARMPLL 2��Ƶʱ��(Ĭ��)
    ** 1: ѡ��BPLL 4��Ƶʱ�� 
    */
    HI_U32 u32TsiClk;

    /*
    ** IPģʽTS�����С��ģʽѡ��
    ** 0: ���ģʽ
    ** 1: С��ģʽ (Ĭ��)
    */
    HI_U32 u32TSInputEndian; 

    /* ��������Ĵ�С��ģʽѡ�� 
    ** 0: С��ģʽ(Ĭ��)
    ** 1: ���ģʽ 
    */
	HI_U32 u32DAVEndianMod;

    /*����ش�С*/
    /*3110E��3110Qʹ��ϵͳĬ�ϵ�ֵ����֧���޸�*/
    HI_U32 u32BufferSize;
}HI_UNF_DMX_GLB_ATTR_S;

typedef struct hiUNF_DMX_ATTR_S
{
	HI_U32 u32PortId ;

}HI_UNF_DMX_ATTR_S;


typedef struct hiUNF_DMX_CAPABILITY_S
{
    HI_U32 u32PortNum;          /* �˿�����     */
    HI_U32 u32DmxNum;           /* �豸����     *///AI7D04684
    HI_U32 u32CommonChannelNum; /* ͨ��ͨ������ */
    HI_U32 u32AudioChannelNum;  /* ��Ƶͨ������ */
    HI_U32 u32VideoChannelNum;  /* ��Ƶͨ������ */
    HI_U32 u32FilterNum;        /* ����������   */
    HI_U32 u32KeyNum;           /* ��Կ������   */
}HI_UNF_DMX_CAPABILITY_S;

/********************* Ts Port Mod  ******************/
typedef enum hiUNF_DMX_PORT_MODE_E
{
    HI_UNF_DMX_PORT_MODE_EXTERNAL,  /*�ⲿTS���� */
    HI_UNF_DMX_PORT_MODE_INTERNAL,  /*�ڲ�TS����*/
    HI_UNF_DMX_PORT_MODE_RAM,       /*���ڴ�����*/

    HI_UNF_DMX_PORT_MODE_BUTT
}HI_UNF_DMX_PORT_MODE_E;

/****************  Ts port Type **********************/
typedef enum hiUNF_DMX_PORT_TYPE_E 
{
    HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST,          /* ����BURSTģʽ             */
    HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID,          /* ����VALIDģʽ             */
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188,     /* ��ͬ��188ģʽ             */
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204,     /* ��ͬ��204ģʽ             */
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204, /* ��ͬ��188/204�Զ�ʶ��ģʽ */
    HI_UNF_DMX_PORT_TYPE_SERIAL,                  /* ����ģʽ                  */
    
    HI_UNF_DMX_PORT_TYPE_BUTT
}HI_UNF_DMX_PORT_TYPE_E;

/****************  Ts port Attr **********************/
typedef struct hiUNF_DMX_PORT_ATTR_S
{
    HI_UNF_DMX_PORT_MODE_E enPortMod;  /* �˿�ģʽ */
    HI_UNF_DMX_PORT_TYPE_E enPortType; /* �˿����ͣ�Hi3110EĬ��Ϊ HI_UNF_DMX_PARALLEL_Valid */
    HI_U32 u32SyncLostTh; /* ͬ����ʧ���ޣ�Ĭ��1 */
    HI_U32 u32SyncLockTh; /* ͬ���������ޣ�Ĭ��5 */

    /* 
    ** Tuner����ʱ���Ƿ���: 
    ** 0: ͬ��(Ĭ��)
    ** 1: ���� 
    */
    HI_U32 u32TunerInClk;

    /* �˿�����ѡ��:
    ** ����ʱ: 
    **    0: ��ʾcdata[7]���λ(Ĭ��)
    **    1: ��ʾcdata[0]���λ
    ** ����ʱ:
    **    0: ��ʾcdata[0]Ϊ������(Ĭ��)
    **    1: ��ʾcdata[7]Ϊ������ 
    */
    HI_U32 u32SerialBitSelector; 

    /* 
    ** tuner��DEMUX��cerr_n�ߵ�ƽģʽ: 
    ** 0: ��ʾcerr_n��Ϊ��ʱ��ʾ���ݴ���
    ** 1: ��ʾcerr_n��Ϊ��ʱ��ʾ���ݴ��� (Ĭ��)
    */
    HI_U32 u32TunerErrMod;
} HI_UNF_DMX_PORT_ATTR_S;

/************** Common Channel Type ******************/
typedef enum hiUNF_DMX_CHAN_TYPE_E
{
    HI_UNF_DMX_CHAN_TYPE_SEC = 0, /*channel is allocated to receive PSI/SI secitons*/
    HI_UNF_DMX_CHAN_TYPE_PES,     /*channel is allocated to receive PES packets*/
    HI_UNF_DMX_CHAN_TYPE_AUD,     /*channel is designated to receive audio data*/
    HI_UNF_DMX_CHAN_TYPE_VID,     /*channel is designated to receive video data*/
	HI_UNF_DMX_CHAN_TYPE_POST,    /*channel is designated to receive whole TS packet*/ 
	HI_UNF_DMX_CHAN_TYPE_ECM_EMM,    /*channel is designated to receive ECM/EMM data */ 
	HI_UNF_DMX_CHAN_TYPE_UNDEF,   /*channel is neither allocated nor designated*/

    HI_UNF_DMX_CHAN_TYPE_BUTT   
}HI_UNF_DMX_CHAN_TYPE_E;


/************  Common Channel CRC Mode  **************/
typedef enum hiUNF_DMX_CHAN_CRC_MODE_E
{
    HI_UNF_DMX_CHAN_CRC_MODE_FORBID = 0,                /* CRCУ���ֹ */
    HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD = 1,     /* CRCУ��ʹ�ܣ�����������Section */
    HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_SEND = 2,        /* CRCУ��ʹ�ܣ������ʹ���Section */
    HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD = 3, /* �����﷨�ж��Ƿ����CRCУ�飬����������Section */
    HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_SEND = 4,    /* �����﷨�ж��Ƿ����CRCУ�飬�����ʹ���Section */

    HI_UNF_DMX_CHAN_CRC_MODE_BUTT
}HI_UNF_DMX_CHAN_CRC_MODE_E;

/************  Common Channel output Mode  **************/
typedef enum hiUNF_DMX_CHAN_OUTPUT_MODE_E
{
	HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY = 0x1,     /* ����Ƶ���ţ������ݽ��� */
	HI_UNF_DMX_CHAN_OUTPUT_MODE_REC  = 0x2,     /* ¼�� */
	
    HI_UNF_DMX_CHAN_OUTPUT_MODE_BUTT = -1
}HI_UNF_DMX_CHAN_OUTPUT_MODE_E;

/************  Channel Attr  **************/
typedef struct hiUNF_DMX_CHAN_ATTR_S
{
    HI_U32 u32BufSize; 		                    /* ͨ��Ӳ��ʹ��buffer��С ��λKbyte*/
    HI_UNF_DMX_CHAN_TYPE_E enChannelType;       /* ͨ������ */
    HI_U32 u32TsPostThreshold;                  /* �����������ޣ�ͨ������ΪDMX_CHAN_POST ʱ��Ч */
    HI_UNF_DMX_CHAN_CRC_MODE_E enCRCMode;       /* CRCģʽ��ͨ������ΪDMX_CHAN_SEC ʱ��Ч */
	HI_UNF_DMX_CHAN_OUTPUT_MODE_E enOutputMode; /* ͨ���������ģʽ */
}HI_UNF_DMX_CHAN_ATTR_S;
/************  Data Buffer  **************/
typedef struct hiUNF_DMX_DATA_BUF_S
{
    HI_U8 * pu8Buf;
    HI_U32 u32BufLen;
    HI_U32 u32BufId;
}HI_UNF_DMX_DATA_BUF_S;

/************  data scrambled flag **************/
typedef enum hiUNF_DMX_SCRAMBLED_FLAG_E
{
    HI_UNF_DMX_SCRAMBLED_FLAG_TS,  /*data is scrambled by TS*/
    HI_UNF_DMX_SCRAMBLED_FLAG_PES, /*data is scrambled by PES*/
    HI_UNF_DMX_SCRAMBLED_FLAG_NO,  /*data is not scrambled*/

    HI_UNF_DMX_SCRAMBLED_FLAG_BUTT
}HI_UNF_DMX_SCRAMBLED_FLAG_E;

/******************  CHANNEL STATUS  ***********************/
typedef enum hiUNF_DMX_CHAN_STATUS_E
{
    HI_UNF_DMX_CHAN_CLOSE = 0x0,
    HI_UNF_DMX_CHAN_PLAY_EN = 0x1,
    HI_UNF_DMX_CHAN_REC_EN = 0X2
}HI_UNF_DMX_CHAN_STATUS_E;

typedef struct hiUNF_DMX_CHAN_STATUS_S
{
    HI_UNF_DMX_CHAN_STATUS_E enChanStatus;
}HI_UNF_DMX_CHAN_STATUS_S;

/******************  Filter Attr  ***********************/
typedef struct hiUNF_DMX_FILTER_ATTR_S
{
	HI_U32 u32FilterDepth		;
    HI_U8 au8Match[DMX_FILTER_MAX_DEPTH]			;
	HI_U8 au8Mask[DMX_FILTER_MAX_DEPTH]				;
	HI_U8 au8Negate[DMX_FILTER_MAX_DEPTH] 			;
}HI_UNF_DMX_FILTER_ATTR_S;

/*AI7D04441 m00129935 09/02/05 begin*/
/***********************recoder type***************/
typedef enum hiUNF_DMX_RECORD_TYPE_E
{
    HI_UNF_DMX_RECORD_NONE = 0,
    HI_UNF_DMX_RECORD_DESCRAM_TS,
    HI_UNF_DMX_RECORD_PES,
    HI_UNF_DMX_RECORD_SCRAM_TS,
    HI_UNF_DMX_RECORD_ALL_TS,
    HI_UNF_DMX_RECORD_TYPE_UNDEF
}HI_UNF_DMX_RECORD_TYPE_E;
/*AI7D04441 m00129935 09/02/05 end*/

/******************************* API declaration *****************************/
/* demux module*/
HI_S32 HI_UNF_DMX_GetGlbDefaultAttr(HI_UNF_DMX_GLB_ATTR_S * pstAttr);
HI_S32 HI_UNF_DMX_Init(HI_UNF_DMX_GLB_ATTR_S *pstAttr);
HI_S32 HI_UNF_DMX_DeInit(HI_VOID);
HI_S32 HI_UNF_DMX_GetCapability(HI_UNF_DMX_CAPABILITY_S *pstCap);

/* port */
HI_S32 HI_UNF_DMX_GetTSPortAttr(HI_U32 u32PortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr);
HI_S32 HI_UNF_DMX_SetTSPortAttr(HI_U32 u32PortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr);
HI_S32 HI_UNF_DMX_OpenTSPort(HI_U32 u32PortId);
HI_S32 HI_UNF_DMX_CloseTSPort(HI_U32 u32PortId);
HI_S32 HI_UNF_DMX_GetTSPortStatus(HI_U32 u32PortId, HI_BOOL *pbStatus);

/* demux*/
HI_S32 HI_UNF_DMX_GetAttr(HI_U32 u32DmxId , HI_UNF_DMX_ATTR_S *pstAttr);
HI_S32 HI_UNF_DMX_SetAttr(HI_U32 u32DmxId , HI_UNF_DMX_ATTR_S *pstAttr);
HI_S32 HI_UNF_DMX_Open(HI_U32 u32DmxId);
HI_S32 HI_UNF_DMX_Close(HI_U32 u32DmxId);
HI_S32 HI_UNF_DMX_Reset(HI_U32 u32DmxId);


/* channel */
HI_S32 HI_UNF_DMX_GetChannelDefaultAttr(HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);
HI_S32 HI_UNF_DMX_NewChannel(HI_U32 u32DemuxId, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr,
            DMX_CHAN_HANDLE *pChannelHandle);
HI_S32 HI_UNF_DMX_DeleteChannel(DMX_CHAN_HANDLE ChannelHandle);
HI_S32 HI_UNF_DMX_GetChannelAttr(DMX_CHAN_HANDLE ChannelHandle, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);
HI_S32 HI_UNF_DMX_SetChannelAttr(DMX_CHAN_HANDLE ChannelHandle, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);
HI_S32 HI_UNF_DMX_SetChannelPID(DMX_CHAN_HANDLE ChannelHandle, HI_U32 u32Pid);
HI_S32 HI_UNF_DMX_GetChannelPID(DMX_CHAN_HANDLE ChannelHandle, HI_U32 *pu32Pid);
HI_S32 HI_UNF_DMX_OpenChannel(DMX_CHAN_HANDLE ChannelHandle);
HI_S32 HI_UNF_DMX_CloseChannel(DMX_CHAN_HANDLE ChannelHandle);
HI_S32 HI_UNF_DMX_GetChannelStatus(DMX_CHAN_HANDLE ChannelHandle, HI_UNF_DMX_CHAN_STATUS_S *pstStatus);
HI_S32 HI_UNF_DMX_GetChannelHandle(HI_U32	u32DmxId , HI_U32 u32Pid, DMX_CHAN_HANDLE *pChannelHandle);
HI_S32 HI_UNF_DMX_GetFreeChannelCount (HI_U32 u32DmxId ,  HI_U32 * pu32FreeCount);
HI_S32 HI_UNF_DMX_GetScrambledFlag(DMX_CHAN_HANDLE ChannelHandle, HI_UNF_DMX_SCRAMBLED_FLAG_E *penScrambleFlag);
HI_S32 HI_UNF_DMX_GetAudioChannelHandle(HI_U32 u32DmxId, DMX_CHAN_HANDLE *pChannelHandle);
HI_S32 HI_UNF_DMX_GetVideoChannelHandle(HI_U32 u32DmxId , DMX_CHAN_HANDLE *pChannelHandle);

/* data filter */
HI_S32 HI_UNF_DMX_NewFilter(HI_U32	u32DemuxId, HI_UNF_DMX_FILTER_ATTR_S  *pstFilterAttr,
            DMX_FILTER_HANDLE *pFilterHandle);
HI_S32 HI_UNF_DMX_DeleteFilter(DMX_FILTER_HANDLE FilterHandle);
HI_S32 HI_UNF_DMX_DeleteAllFilter(DMX_CHAN_HANDLE ChannelHandle);
HI_S32 HI_UNF_DMX_SetFilterAttr(DMX_FILTER_HANDLE FilterHandle, HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr );
HI_S32 HI_UNF_DMX_GetFilterAttr(DMX_FILTER_HANDLE FilterHandle, HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr );
HI_S32 HI_UNF_DMX_AttachFilter(DMX_FILTER_HANDLE FilterHandle, DMX_CHAN_HANDLE ChannelHandle);
HI_S32 HI_UNF_DMX_DetachFilter(DMX_FILTER_HANDLE FilterHandle, DMX_CHAN_HANDLE ChannelHandle);
HI_S32 HI_UNF_DMX_GetFilterChannelHandle(DMX_FILTER_HANDLE FilterHandle, DMX_CHAN_HANDLE *pChannelHandle);
HI_S32 HI_UNF_DMX_GetFreeFilterCount(HI_U32 u32DmxId ,  HI_U32 * pu32FreeCount);

/* Descrambler */
HI_S32 HI_UNF_DMX_NewDescrambler(HI_U32 u32DmxId, DMX_KEY_HANDLE *pKeyHandle);
HI_S32 HI_UNF_DMX_DeleteDescrambler(DMX_KEY_HANDLE KeyHandle);
HI_S32 HI_UNF_DMX_SetDescramblerEvenKey(DMX_KEY_HANDLE KeyHandle, HI_U8 *pu8EvenKey);
HI_S32 HI_UNF_DMX_SetDescramblerOddKey(DMX_KEY_HANDLE KeyHandle, HI_U8 *pu8OddKey);
HI_S32 HI_UNF_DMX_AttachDescrambler(DMX_KEY_HANDLE KeyHandle, DMX_CHAN_HANDLE ChannelHandle);
HI_S32 HI_UNF_DMX_DetachDescrambler(DMX_KEY_HANDLE KeyHandle, DMX_CHAN_HANDLE ChannelHandle);
HI_S32 HI_UNF_DMX_GetDescramblerKeyHandle(DMX_CHAN_HANDLE ChannelHandle, DMX_KEY_HANDLE *pKeyHandle);
HI_S32 HI_UNF_DMX_GetFreeDescramblerKeyCount(HI_U32 u32DmxId , HI_U32 * pu32FreeCount);

HI_S32  HI_UNF_DMX_GetDataHandle(DMX_CHAN_HANDLE *pChannelHandle ,
                                 HI_U32 *pu32ChNum, HI_U32 u32TimeOut);


HI_S32  HI_UNF_DMX_AcquireBuf(DMX_CHAN_HANDLE ChannelHandle, HI_U32 u32AcquireNum,
            HI_U32 * pu32AcquiredNum, HI_UNF_DMX_DATA_BUF_S *pstBuf,
            HI_U32 u32TimeOut);

HI_S32  HI_UNF_DMX_ReleaseBuf(DMX_CHAN_HANDLE ChannelHandle, HI_U32 u32ReleaseNum,
            HI_UNF_DMX_DATA_BUF_S *pstBuf);

/*AI7D04441 m00129935 09/02/05 begin*/
/*api about recoder supporting PVR functions*/
HI_S32 HI_UNF_DMX_SetRecodeType(HI_U32 u32DmxId, HI_UNF_DMX_RECORD_TYPE_E eRecType);
HI_S32 HI_UNF_DMX_GetRecodeType(HI_U32 u32DmxId, HI_UNF_DMX_RECORD_TYPE_E *pRecType);
/*AI7D04441 m00129935 09/02/05 end*/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */



#endif /* __HI_UNF_DEMUX_H__ */

