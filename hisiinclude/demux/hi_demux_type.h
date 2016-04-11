/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  hi_demux_type.h
* Description:  Define structures used by api or users
*
* History:
* Version      Date         Author     DefectNum    Description
* main\1    2006-11-01      l55251        NULL      Create this file.
***********************************************************************************/
#ifndef __HI_DEMUX_TYPE_H__
#define __HI_DEMUX_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_pvr_api.h"
#include "hi_unf_demux.h"

/* add by l00111160 on 2009.9-2009.12 */
/* 定义宏HI_DMX_CHAN_BUF_DIRECTLY表示内核使用通道Buffer直接读取方案 */
#ifndef HI_DMX_CHAN_BUF_DIRECTLY
#define HI_DMX_CHAN_BUF_DIRECTLY
//#undef HI_DMX_CHAN_BUF_DIRECTLY
#endif
#ifdef HI_DMX_CHAN_BUF_DIRECTLY
#define HI_DMX_DEBUG_BUFFER_DEF_SIZE    (4*1024*1024)
#define HI_DMX_DEBUG_BUFFER_AUTO_SIZE   (8*1024*1024)
#define HI_DMX_DEBUG_BUFFER_AUTO_PID    (HI_U32)(-1)    /* -1表示所有通道 */
#define HI_DMX_DEBUG_BUFFER_AUTO    /* 自动打开调试buffer */
#undef HI_DMX_DEBUG_BUFFER_AUTO
#define HI_DMX_SUB_DEV_COUNT    3
#define DMX_RECORD_CHAN_NUM_MAX 16
#define HI_DMX_TS_POST_BIT  0x01000000
#endif

#ifndef SEM_MUTEX
#define SEM_MUTEX HI_U32
#endif

#define HI_DMX_TRACE HI_TRACE
/*sub dev define*/
#define MAX_SUB_DEV_ID                    (2)
#define DMX_REC_SUB_DEV_1              (2)
#define DMX_REC_SUB_DEV_2              (1)
#define DMX_PLAY_SUB_DEV                (0)

#define DMX_CHAN_COUNT                       (32)     /* number of all demux channels */
#define DMX_CHAN_COMM_COUNT                  (30)
#define DMX_FILTER_COUNT                     (48)     /* number of all filter channels */
#define DMX_KEY_COUNT                        (8)      /* number of all key channels */

#define DMX_FILTER_DEPTH                     (16)     /* byte depth of filter */

/*AI7D03091 mxf00129935 08/09/25 begin*/
#define DMX_ERR_INT_BITS                     (0x27f8) /*bit3~10,13*/

/*mask of demux error events*/
#define MASK_DMX_INT_STATE_BUF_OVERFLOW      (0x2000)  /* bit13 */
/*AI7D03091 mxf00129935 08/09/25 begin*/

#define MASK_DMX_INT_STATE_PES_LEN_ERR       (0x400)  /* bit10 */
#define MASK_DMX_INT_STATE_SYNC_LOST         (0x200)  /* bit9 */
#define MASK_DMX_INT_STATE_SYNC_LOCK         (0x100)  /* bit8 */
#define MASK_DMX_INT_STATE_TEI_ERROR         (0x80)   /* bit7 */
#define MASK_DMX_INT_STATE_PORT_ERROR        (0x40)   /* bit6 */
#define MASK_DMX_INT_STATE_TS_DISCON         (0x20)   /* bit5 */
#define MASK_DMX_INT_STATE_CRC_ERROR         (0x10)   /* bit4 */
#define MASK_DMX_INT_STATE_PCR_DISCON        (0x8)    /* bit3 */
#define MASK_DMX_INT_STATE_PCR_GET           (0x4)    /* bit2 */
#define MASK_DMX_INT_STATE_SCR_ADJUST        (0x2)    /* bit1 */
#define MASK_DMX_INT_STATE_SCR_RELOAD        (0x1)    /* bit0 */

#define TOTAL_ERROR_COUNT                    (12)     /* number of all errors reported to user */
#define MAX_REGISTERED_ERROR_COUNT           (8)
#define DMX_AUDIO_CHANID                        (30)       /*audio channel id*/
#define DMX_VIDEO_CHANID                        (31)       /*video channel id*/


/*************************** Structure Definition ****************************/
/*begin: for msg solution add by tzg on 2008.9.15 */
/*
    msg solution 说明:
    在hi3110QB01G版本的基础上，添加一个消息处理方式。
    每个消息的定义为消息ID号，缓冲区起始地址，和大小。
    应用或者中间层可以在开启通道过滤数据之前向底层注册足够多的空闲消息。
    底层驱动在接收到硬件数据包中断后将数据拷贝到消息中，并在中间层或者应用
    查询的时候将消息返回给上层，达到不由于硬件缓冲区溢出而丢失数据的目的。
*/
typedef struct hiDMX_UserMsg_S
{
    HI_U32 u32MsgNo;
    HI_U32 u32BufSize;
    HI_U32 u32MsgLen;
    HI_U32 u32BufStartAddr;
}DMX_UserMsg_S;

typedef struct hiDMX_UserMsgDesc_S
{
    HI_U32 u32DemuxId;
    HI_U32 u32ChanId;  /*只对查询通道消息的函数有用，表示需要获取的通道标识*/
    HI_U32 u32MsgNum;
    DMX_UserMsg_S* psMsgList;
#ifdef HI_DMX_CHAN_BUF_DIRECTLY
    HI_U32 u32ThreadId;
#endif
}DMX_UserMsgDesc_S;

typedef enum hiDMX_PacketCntCtrl_E
{
    DMX_PACKETCOUNT_RESET,                            /*reset counter*/
    DMX_PACKETCOUNT_PERMIT,                           /*start counter*/
    DMX_PACKETCOUNT_PAUSE                             /*pause counter*/
}DMX_PacketCntCtrl_E;


typedef struct hiDMX_WaitError_S
{
    HI_U32 u32ChannelId;                             /* Channel id */
    HI_U32 u32StatusId;                              /* error status */
}DMX_WaitError_S;

typedef enum hiDMX_ChanSelect_E
{
    DMX_ALL_CHAN = 0,                                /* receive datas of all channels */
    DMX_SINGAL_CHAN = 1                              /* receive datas of single channel */
}DMX_ChanSelect_E;

typedef enum hiDMX_DataRead_E
{
    DMX_ALL_DATA = 1,                                /* receive datas of all channels */
    DMX_SINGAL_DATA = 0                              /* receive datas of single channel */
}DMX_DataRead_E;

typedef struct hiDMX_GetFlag_S
{
    HI_U32 u32DmxId;
    DMX_ChanSelect_E eChanOption;
    HI_U32 u32ChannelId;
    HI_U32 *pFlag;
    HI_U32 u32TimeOut;                            /* error status */
}DMX_GetFlag_S;
/***************************************  Ts port  ******************************************/

typedef enum hiDMX_PortType_E
{
    DMX_PARALLEL_BURST,                              /*parallel burst port type*/
    DMX_PARALLEL_VALID,                              /*parallel valid port type*/
    DMX_PARALLEL_SYNCSELF,                           /*parallel nosync port type*/
    DMX_SERIAL                                       /*serial port type*/
}DMX_PortType_E;

typedef enum hiDMX_PortStatus_E
{
    DMX_PORT_OPEN,                                   /*ts port is opened*/
    DMX_PORT_CLOSE                                   /*ts port is closed*/
}DMX_PortStatus_E;

typedef enum hiDMX_ScrambleFlag_E
{
    DMX_SCRAMBLE_YES,                                /*audio/video is scrambled*/
    DMX_SCRAMBLE_NO                                  /*audio/video is not scrambled*/
}DMX_ScrambleFlag_E;

/***************************************  Clock Recovery  ******************************************/

typedef enum hiDMX_ClkRecvMode_E
{
    DMX_SOFTWARE_ADJUST = 0,                        /*clock recovery mode is adjustion by software*/
    DMX_HARDWARE_FORE = 1,                          /*clock recovery mode is foreward adjustion by hardware*/
    DMX_HARDWARE_BACK = 2                           /*clock recovery mode is backward adjustion by hardware*/
}DMX_ClkRecvMode_E;


typedef struct hiDMX_PCR_S
{
    HI_U32 u32PCRBase0;                            /*bit 0 of pcr counter*/
    HI_U32 u32PCRBase32_1;                         /*bit 1~32 of pcr counter*/
    HI_U32 u32PCRExtension9;                       /*bit 33~41 of pcr counter*/
}DMX_PCR_S;


/***************************************  DMA buffer  ******************************************/
typedef enum hiDMX_BUFFER_SIZE_E
{
    DMX_DMA_SIZE_16K = 0,
    DMX_DMA_SIZE_32K,
    DMX_DMA_SIZE_64K,
    DMX_DMA_SIZE_128K,
    DMX_DMA_SIZE_256K,
    DMX_DMA_SIZE_512K,
    DMX_DMA_SIZE_UNDEF
} DMX_BUFFER_SIZE_E;






/***************************************  Channel  ******************************************/

typedef enum hiDMX_ChanType_E
{
    DMX_CHAN_SEC = 0,                             /*channel is allocated to receive PSI/SI secitons*/
    DMX_CHAN_PES,                                 /*channel is allocated to receive PES packets*/
    DMX_CHAN_AUD,                                 /*channel is designated to receive audio data*/
    DMX_CHAN_VID,                                 /*channel is designated to receive video data*/
    DMX_CHAN_ECM_EMM,
    DMX_CHAN_UNDEF                                /*channel is neither allocated nor designated*/
}DMX_ChanType_E;

typedef enum hiDMX_BufType_E
{
    DMX_BUFFER_COMMON = 0,                             /*buffer is allocated to receive PSI/SI secitons*/
    DMX_BUFFER_REC_TS,                                 /*channel is allocated to receive PES packets*/
    DMX_BUFFER_REC_PES,                                 /*channel is allocated to receive private data*/
    DMX_BUFFER_PLAYBACK_TS,                                 /*channel is designated to receive audio data*/
    DMX_BUFFER_SCD,                                 /*channel is designated to receive video data*/
    DMX_BUFFER_AV,
    DMX_BUFFER_UNDEF                                /*channel is neither allocated nor designated*/
}DMX_BufType_E;


typedef enum hiDMX_FlushStatus_E
{
    DMX_FLUSHING,                                 /*channel is flushing*/
    DMX_FINISH_FLUSH,                             /*channel has been flushed*/
    DMX_WAIT_FLUSH                                 /*channel is waiting flushing*/
}DMX_FlushStatus_E;

typedef enum hiDMX_FLUSH_TYPE_E
{
    DMX_FLUSH_PLAY = 0x1,                          /*flush the channel's play and rec config*/
    DMX_FLUSH_RECORD = 0x2,
    DMX_FLUSH_REC_PLAY = 0x0,
    DMX_FLUSH_NULL = 0x3
}DMX_FLUSH_TYPE_E;


typedef enum hiDMX_CHAN_STATUS_E
{
    DMX_CHAN_CLOSE = 0x0,
    DMX_CHAN_PLAY_EN = 0x1,
    DMX_CHAN_REC_EN,
    DMX_CHAN_REC_PLAY_EN
}DMX_CHAN_STATUS_E;



typedef enum hiDMX_CHAN_FUNC_TYPE_E
{
    DMX_CHAN_FUNC_RESERVE = 0,
    DMX_CHAN_FUNC_PLAY = 0x1,                          /*channel has been allocated for playing*/
    DMX_CHAN_FUNC_RECORD = 0x2,                             /*channel has been attatched for Recode*/
    DMX_CHAN_FUNC_REC_PLAY = 0x3,                       /*channel has been attatched for Recode and playing usually for section access*/
    DMX_CHAN_FUNC_UNDEF = 0x4                             /*channel  has not been allocated yet*/
}DMX_CHAN_FUNC_TYTE_E;

#ifdef HI_DMX_CHAN_BUF_DIRECTLY

/***************************************  Filter  ******************************************/

typedef enum hiDMX_FilterState_E
{
    DMX_FILTER_UNUSED,                            /*filter has not been allocated yet*/
    DMX_FILTER_ALLOCATE,                          /*filter has been allocated but not been attached to channel*/
    DMX_FILTER_ACTIVE                            /*filter has been attached to channel*/
}DMX_FilterState_E;

/***************************************  KeySet  ******************************************/

typedef enum hiDMX_KeySetState_E
{
    DMX_KEYSET_UNUSED,                            /*key has not been allocated yet*/
    DMX_KEYSET_ALLOCATE,                          /*key has been allocated but not been attatched to channel*/
    DMX_KEYSET_ACTIVE                             /*key has been attatched to channel*/
}DMX_KeySetState_E;

#endif

/***************************************  Recode  ******************************************/

typedef enum hiDMX_RECORD_TYPE_E
{
    DMX_RECORD_NONE = 0,
    DMX_RECORD_DESCRAM_TS,
    DMX_RECORD_PES,
    DMX_RECORD_SCRAM_TS,
    DMX_RECORD_ALL_TS,
    DMX_RECORD_TYPE_UNDEF
} DMX_RECORD_TYPE_E;

/*Ts source select*/
typedef enum hiDMX_TS_SOURCE_TYPE_E
{
    DMX_TS_SOURCE_NONE = 0x0,
    DMX_TS_SOURCE_FROM_QAM,
    DMX_TS_SOURCE_FROM_PAD,
    DMX_TS_SOURCE_FROM_RAM
}DMX_TS_SOURCE_TYPE_E;


typedef enum hiDMX_ErrorType_E
{
    DMX_PORT_ERROR        = 1,                    /*ts port error*/
    DMX_SYNC_LOST         = 2,                    /*sync lost error*/
    DMX_TRANSPORT_ERROR   = 4,                    /*tei error*/
    DMX_PACKET_LOST       = 8,                    /*ts packet  discontinuity error*/
    DMX_BUFFER_OVERFLOW   = 16,                   /*DMA buffer overflow error*/
    DMX_CRC_ERROR         = 32,                   /*data crc error*/
    DMX_PCR_DISCONTINUITY = 64,                   /*pcr discontinuity error*/
    DMX_PES_LEN_ERROR     = 128                   /*pes packet length error*/
}DMX_ErrorType_E;

typedef struct hiDMX_DATA_S{
    HI_U8   *pAddr;
    HI_U32   u32PhyAddr;
    HI_U32   u32Len;
}DMX_DATA_S;

/* DMX_IDX_DATA_S 中各成员的顺序不能改变，必须与硬件规定的顺序保持一致 */
typedef struct hiDMX_IDX_DATA_S
{
    HI_U32 u32IdxType_Flags;
    HI_U32 u32ScType_Byte12AfterSc_OffsetInTs;
    HI_U32 u32TsCntLo32;
    HI_U32 u32TsCntHi8_Byte345AfterSc;
    HI_U32 u32ScErr_Byte678AfterSc;
    HI_U32 u32SrcClk;
    HI_U32 u32DavBufOffset;
} DMX_IDX_DATA_S;

typedef struct hiDMX_DATA_EXCHANGE_S
{
     HI_U32         u32DmxId;
     HI_U32         u32TimeOut;
     HI_BOOL        bDoCipher;
     HI_BOOL        bBlock;
     DMX_DATA_S    sData;
} DMX_DATA_EXCHANGE_S;

/*设置参考通道类型 for pvr*/
typedef enum hiDMX_REF_CHAN_TYPE_E
{
    DMX_REF_CHAN_NONE = 0x0,
    DMX_REF_CHAN_FROM_AUD,
    DMX_REF_CHAN_FROM_VID,
    DMX_REF_CHAN_FROM_ALL
}DMX_REF_CHAN_TYPE_E;

/*****************************   used by api only  *****************************************/
typedef struct hiDMX_ErrReg_S
{
    HI_U32 u32DmxId;
    HI_U32 u32ErrReg;
}DMX_ErrReg_S;

/*AI7D01879 start*/
typedef struct hiDMX_ReportError_S
{
    HI_U32 u32DmxId;
    DMX_WaitError_S *pError;
    HI_U32 u32TimeOut;
}DMX_ReportError_S;
/*AI7D01879 end*/

typedef struct hiDMX_ReadData_S
{
    HI_U32  u32DmxId;
    HI_U32  u32ChannelId;
    HI_U8 *pData;
    HI_U32  u32Len;
    HI_U32  *pCopyLen;
    HI_U32 *pFilterId;
    DMX_DataRead_E eReadType;
}DMX_ReadData_S;

typedef struct hiDMX_GetDataFlag_S
{
    HI_U32 u32DmxId;
    DMX_ChanSelect_E eChanOption;
    HI_U32 u32ChannelId;
    HI_U32 *pFlag;
    HI_U32   u32TimeOut;
#ifdef HI_DMX_CHAN_BUF_DIRECTLY
    HI_U32 u32ThreadId;
#endif
}DMX_GetDataFlag_S;

typedef struct hiDMX_SetPortType_S
{
    HI_U32 u32DmxId;
    DMX_PortType_E eType;
}DMX_SetPortType_S;

typedef struct hiDMX_GetPortType_S
{
    HI_U32 u32DmxId;
    DMX_PortType_E *pType;
}DMX_GetPortType_S;

typedef struct hiDMX_GetPortStatus_S
{
    HI_U32 u32DmxId;
    DMX_PortStatus_E *pStatus;
}DMX_GetPortStatus_S;

typedef struct hiDMX_ChanNew_S
{
    HI_U32 u32DmxId;
    DMX_ChanType_E  eType;
    DMX_CHAN_FUNC_TYTE_E eCreatType;
    HI_U32 u32BufSize;
    HI_U32          *pChannelId;
}DMX_ChanNew_S;

typedef struct hiDMX_ChanPIDSet_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
    HI_U32  u32Pid;
}DMX_ChanPIDSet_S;

typedef struct hiDMX_SetChan_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
}DMX_SetChan_S;

typedef struct hiDMX_NewChanFunc_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
    HI_U32 u32BufSize;
    DMX_CHAN_FUNC_TYTE_E eFuncType;
}DMX_NewChanFunc_S;

typedef struct hiDMX_RemoveChanFunc_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
    DMX_CHAN_FUNC_TYTE_E eFuncType;
}DMX_RemoveChanFunc_S;

typedef struct hiDMX_SetChanStatus_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
    DMX_CHAN_FUNC_TYTE_E eType;
}DMX_SetChanStatus_S;

typedef struct hiDMX_ChanPIDGet_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
    HI_U32  *pPid;
}DMX_ChanPIDGet_S;

typedef struct hiDMX_ChanStatusGet_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
    DMX_CHAN_STATUS_E  *pChanStatus;
}DMX_ChanStatusGet_S;

typedef struct hiDMX_ChannelIdGet_S
{
    HI_U32 u32DmxId;
    HI_U32  *pChannelId;
    HI_U32  u32Pid;
}DMX_ChannelIdGet_S;

typedef struct hiDMX_FreeChanGet_S
{
    HI_U32 u32DmxId;
    HI_U32  *pFreeChanNum;
}DMX_FreeChanGet_S;

typedef struct hiDMX_SetChanCRC_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChanId;
    DMX_CHAN_FUNC_TYTE_E eFuncType;
}DMX_SetChanCRC_S;

typedef struct hiDMX_ChannelFilterIDGet_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
    HI_U32  *pLowFilterId;
    HI_U32  *pHighFilterId;
}DMX_ChannelFilterIDGet_S;

typedef struct hiDMX_NewFilterID_S
{
    HI_U32 u32DmxId;
    HI_U32 *pFilterId;
}DMX_NewFilterID_S;

typedef struct hiDMX_SetFilterID_S
{
    HI_U32 u32DmxId;
    HI_U32 FilterId;
}DMX_SetFilterID_S;

typedef struct hiDMX_PostThreshSet_S
{
    HI_U32 u32DmxId;
    HI_U32  u32Thresh;
    HI_U32  u32ChannelId;
}DMX_PostThreshSet_S;

typedef struct hiDMX_PostThreshGet_S
{
    HI_U32 u32DmxId;
    HI_U32  *pThresh;
    HI_U32  u32ChannelId;
}DMX_PostThreshGet_S;

typedef struct hiDMX_ScrambledFlagGet_S
{
    HI_U32 u32DmxId;
    DMX_ScrambleFlag_E* pScrambleFlag;
}DMX_ScrambledFlagGet_S;

typedef struct hiDMX_FilterSet_S
{
    HI_U32 u32DmxId;
    HI_U32  u32FilterDepth;
    HI_U32  u32FilterId;
    HI_U8 *pMask;
    HI_U8 *pMatch;
    HI_U8 *pNegate;
    HI_BOOL bPol;
}DMX_FilterSet_S;

typedef struct hiDMX_FilterGet_S
{
    HI_U32 u32DmxId;
    HI_U32  *pFilterDepth;
    HI_U32  u32FilterId;
    HI_U8 *pMask;
    HI_U8 *pMatch;
    HI_U8 *pNegate;
    HI_BOOL *pPol;
}DMX_FilterGet_S;

typedef struct hiDMX_FilterAttach_S
{
    HI_U32 u32DmxId;
    HI_U32  u32FilterId;
    HI_U32  u32ChannelId;
}DMX_FilterAttach_S;

typedef DMX_FilterAttach_S DMX_FilterDetach_S;

typedef struct hiDMX_FilterChannelIDGet_S
{
    HI_U32 u32DmxId;
    HI_U32 u32FilterId;
    HI_U32 *pChannelId;
}DMX_FilterChannelIDGet_S;

typedef struct hiDMX_FreeFilterGet_S
{
    HI_U32 u32DmxId;
    HI_U32 *pFreeFilterNum;
}DMX_FreeFilterGet_S;

typedef struct hiDMX_NewKeyId_S
{
    HI_U32 u32DmxId;
    HI_U32 *pKeyId;
}DMX_NewKeyId_S;

typedef struct hiDMX_DelKeyId_S
{
    HI_U32 u32DmxId;
    HI_U32 KeyId;
}DMX_DelKeyId_S;


typedef struct hiDMX_KeySet_S
{
    HI_U32 u32DmxId;
    HI_U32  u32KeyId;
    HI_U8 *pKey;
}DMX_KeySet_S;

typedef DMX_KeySet_S  DMX_KeyGet_S;


typedef struct hiDMX_KeyIdGet_S
{
    HI_U32 u32DmxId;
    HI_U32 u32ChannelId;
    HI_U32 *pKeyId;
}DMX_KeyIdGet_S;

typedef struct hiDMX_KeyAttach_S
{
    HI_U32 u32DmxId;
    HI_U32 u32ChannelId;
    HI_U32 u32KeyId;
}DMX_KeyAttach_S;

typedef struct hiDMX_FreeKeyGet_S
{
    HI_U32 u32DmxId;
    HI_U32 *pFreeCount;
}DMX_FreeKeyGet_S;


typedef struct hiDMX_SetTsType_S
{
    HI_U32 u32DmxId;
    HI_U32 u32BufSize;
    HI_U32 u32BufPhyAddr;
    DMX_TS_SOURCE_TYPE_E eSource;
}DMX_SetTsType_S;

 typedef struct hiDMX_GetTsType_S
{
    HI_U32 u32DmxId;
    DMX_TS_SOURCE_TYPE_E* pSource;
}DMX_GetTsType_S;


 typedef struct hiDMX_SetRecType_S
{
    HI_U32 u32DmxId;
    DMX_RECORD_TYPE_E eRecType;
}DMX_SetRecType_S;

  typedef struct hiDMX_GetRecType_S
{
    HI_U32 u32DmxId;
    DMX_RECORD_TYPE_E *pRecType;
}DMX_GetRecType_S;

typedef enum hiDMX_RECORD_STATUS_E
{
    DMX_RECORD_INIT = 0,
    DMX_RECORD_START,
    DMX_RECORD_STOP,
    DMX_RECORD_STATUS_UNDEF
} DMX_RECORD_STATUS_E;

  typedef struct hiDMX_GetRecStatus_S
{
    HI_U32 u32DmxId;
    DMX_RECORD_STATUS_E *pRecStatus;
}DMX_GetRecStatus_S;

  typedef struct hiDMX_PauseSwitch_S
{
    HI_U32 u32DmxId;
    HI_U32 u32TimeOut;
}DMX_PauseSwitch_S;
 typedef struct hiDMX_StartRec_S
{
     HI_U32 u32DmxId;
     PVR_REC_INDEX_TYPE_E eIndexType;     /* change by quyaxin @ 20080619 */

     HI_U32 u32SCDBufSize;
     HI_U32 u32TsBufSize;
     PVR_CIPHER_S sCipherCfg;
     HI_U32 u32ScBufPhyAddr; /* output */
     HI_U32 u32TsBufPhyAddr; /* output */
     HI_U32 u32CipherBufPhyAddr; /* output */
}DMX_StartRec_S;

typedef DMX_KeyAttach_S DMX_KeyDetach_S;

/*AI7D04251 适配统一接口 m00129935 09/01/04 begin*/
typedef enum hiDmx_Channel_CRCMode_E
{
    CRC_FORBID                = 0,                         /*enable crc by force*/
    CRC_FORCE_AND_DISCARD     = 1,                         /*enable crc by force and discard packets when crc error*/
    CRC_FORCE_AND_SEND        = 2,                         /*enable crc by force and send packets when crc error*/
    CRC_BY_SYNTAX_AND_DISCARD = 3,                         /*enable crc by syntax and discard packets when crc error*/
    CRC_BY_SYNTAX_AND_SEND    = 4                         /*enable crc by syntax and send packets when crc error*/
}Dmx_Channel_CRCMode_E;

/*Port mode attach*/
typedef enum hiDMX_PORT_TS_TYPE_E
{
    PORT_QAM = 0, /*端口0*/
    PORT_PAD,     /*端口1*/
    PORT_RAM,     /*端口2*/
#ifdef HI_DMX_CHAN_BUF_DIRECTLY
    PORT_COUNT
#endif
}DMX_PORT_TS_TYPE_E;
typedef struct hiDMX_Chan_Attr_S
{
    HI_U32 u32BufSize;
    DMX_ChanType_E eChannelType;
    HI_U32 u32TsPostThreshold;
    Dmx_Channel_CRCMode_E 	eCRCMode;
	DMX_CHAN_FUNC_TYTE_E    eChannelFunc ;
}DMX_Chan_Attr_S;

typedef struct hiDMX_GetChan_Attr_S
{
    HI_U32 u32DmxId;
    HI_U32 u32ChannelId;
    DMX_Chan_Attr_S *psChanAttr;
}DMX_GetChan_Attr_S;
typedef DMX_GetChan_Attr_S DMX_SetChan_Attr_S ;

typedef struct hiDMX_Port_Attr_S
{
    DMX_TS_SOURCE_TYPE_E eTsSourceType;
    DMX_PortType_E ePortType;
    HI_U32 u32SyncLostTh;
    HI_U32 u32SyncLockTh;
    HI_U32 u32TunerInClk;
    HI_U32 u32SerialBitSelector;
    HI_U32 u32TunerErrMod;
}DMX_Port_Attr_S;

typedef struct hiDMX_GetPort_Attr_S
{
    HI_U32 u32TsPortId;
    DMX_Port_Attr_S *psPortAttr;
}DMX_GetPort_Attr_S;
typedef DMX_GetPort_Attr_S DMX_SetPort_Attr_S ;
typedef struct hiDMX_GetAllDmxDataFlag_S
{
    HI_U32 u32ChTotalNum;
    HI_U32 *pu32ChannelSelect;
    HI_U32 *pu32Flag;
    HI_U32 u32TimeOut;
#ifdef HI_DMX_CHAN_BUF_DIRECTLY
    HI_U32 u32ThreadId;
#endif
}DMX_GetAllDmxDataFlag_S;
/*AI7D04251 适配统一接口 m00129935 09/01/04 end*/

/*begin: add by mxf on 2009.04.29 */
typedef enum hiDMX_SOURCE_SEL_E
{
    DMX_NONE_SEL = 0,
    DMX_QAM_TS_SEL,
    DMX_PAD_TS_SEL,
    DMX_REPLAY_SEL
}DMX_SOURCE_SEL_E;
typedef struct hiDMX_PORT_VERSATILE_STATE_S
{
    HI_U32 u32TsPacketCnt;          /*端口进入的TS 包总个数*/
    HI_U32 u32TsErrPacketCnt;     /*端口进入的TS 错误包总个数*/
}DMX_PORT_VERSATILE_STATE_S;
typedef struct hiDMX_MULTIPORT_VERSATILE_STATE_S
{
    DMX_SOURCE_SEL_E    enPortSource;
    DMX_PORT_VERSATILE_STATE_S *pstVersatileState;
}DMX_MULTIPORT_VERSATILE_STATE_S;
/*end: add by mxf on 2009.04.29 */

#if 0
typedef struct hiDMX_Test_ChanRead_S
{
    HI_U32 u32DmxId;
    HI_U32  u32ChannelId;
    DMX_ChanInfo_S  *pChanInfo;
}DMX_Test_ChanRead_S;

typedef struct hiDMX_Test_FilterRead_S
{
    HI_U32 u32DmxId;
    HI_U32  u32FilterId;
    DMX_FilterInfo_S  *pFilterInfo;
}DMX_Test_FilterRead_S;

typedef struct hiDMX_Test_KeyRead_S
{
    HI_U32 u32DmxId;
    HI_U32  u32KeyId;
    DMX_KeySetInfo_S  *pKeyInfo;
}DMX_Test_KeyRead_S;

#endif

#ifdef HI_DMX_CHAN_BUF_DIRECTLY

typedef enum hiDMX_ATTR_FLAG_TYPE_E
{
    HI_DMX_ATTR_GLB_USE_EOP_INT,
    HI_DMX_ATTR_GLB_ACTIVED_ADDR_H4,
    HI_DMX_ATTR_GLB_DBG_EVENT,
    HI_DMX_ATTR_GLB_DBG_EVENT_PROC,
    HI_DMX_ATTR_GLB_DBG_EVENT_TRIGGER,
    HI_DMX_ATTR_GLB_DBG_EVENT_APPLY_PRINT,
    HI_DMX_ATTR_GLB_DBG_BUF_SIZE,
    HI_DMX_ATTR_GLB_CRT_DEL_DEBUG,
    HI_DMX_ATTR_GLB_EOP_DAT_DEBUG,
    HI_DMX_ATTR_GLB_DAT_BUF_DEBUG,
    HI_DMX_ATTR_GLB_MSG_BUF_DEBUG,
    HI_DMX_ATTR_GLB_KEY_DAT_DEBUG,
    HI_DMX_ATTR_GLB_USER_THREAD_DEBUG,
    HI_DMX_ATTR_CHAN_FORBID_ERR,
    HI_DMX_ATTR_CHAN_DELAY_DELIVER,
    HI_DMX_ATTR_CHAN_EOP_DAT_DEBUG,
    HI_DMX_ATTR_CHAN_DAT_BUF_DEBUG,
    HI_DMX_ATTR_CHAN_MSG_BUF_DEBUG,
    HI_DMX_ATTR_KEY_DATA_DEBUG
} HI_DMX_ATTR_FLAG_TYPE_E;

typedef enum hiDMX_DBG_EVT_ACT_E
{
    HI_DMX_DBG_EVT_SPACE,
    HI_DMX_DBG_EVT_CHAN_CREATE,
    HI_DMX_DBG_EVT_CHAN_DELETE,
    HI_DMX_DBG_EVT_CHAN_OPEN,
    HI_DMX_DBG_EVT_CHAN_CLOSE,
    HI_DMX_DBG_EVT_CHAN_SET_PID,
    HI_DMX_DBG_EVT_CHAN_DATA,
    HI_DMX_DBG_EVT_FILTER_CREATE,
    HI_DMX_DBG_EVT_FILTER_DELETE,
    HI_DMX_DBG_EVT_FILTER_SET,
    HI_DMX_DBG_EVT_FILTER_ATTACH,
    HI_DMX_DBG_EVT_FILTER_DEATTACH,
    HI_DMX_DBG_EVT_DESC_CREATE,
    HI_DMX_DBG_EVT_DESC_DELETE,
    HI_DMX_DBG_EVT_DESC_ATTACH,
    HI_DMX_DBG_EVT_DESC_DEATTACH,
    HI_DMX_DBG_EVT_DESC_DATA,
    HI_DMX_DBG_EVT_EOP_DATA
} HI_DMX_DBG_EVT_ACT_E;

typedef enum hiDMX_DBG_EVT_TRIGGER_E
{
    HI_DMX_DBG_EVT_TRIGGER_LEN_ERR,     /* 接收数据长度与EOP地址不一致错误 */
    HI_DMX_DBG_EVT_TRIGGER_SIZE    
} HI_DMX_DBG_EVT_TRIGGER_E;

typedef struct hiDMX_DBG_EVT_SPACE_S
{
    HI_U32 u32Count;
} HI_DMX_DBG_EVT_SPACE_S;

typedef struct hiDMX_DBG_EVT_CHAN_S
{
    HI_U32 u32DmxId;
    HI_U32 u32ChnId;
    DMX_ChanType_E enChannelType;
    DMX_CHAN_FUNC_TYTE_E enCreatType;
    HI_U32 u32BufSize;
    HI_U32 u32Pid;
} HI_DMX_DBG_EVT_CHAN_S;

typedef enum hiDMX_DBG_EVT_CHAN_DAT_TYPE_E
{
    HI_DMX_DBG_EVT_CHAN_DAT_GET,    /* 从通道buffer里解析到一个数据包 */
    HI_DMX_DBG_EVT_CHAN_DAT_PUT,    /* 从通道buffer里复制到Msg buffer的数据包 */
    HI_DMX_DBG_EVT_CHAN_DAT_ACQ,    /* 用户查询到的数据 */
    HI_DMX_DBG_EVT_CHAN_DAT_REL    /* 用户释放的数据 */
} HI_DMX_DBG_EVT_CHAN_DAT_TYPE_E;

typedef struct hiDMX_DBG_EVT_CHAN_DAT_S
{
    HI_U32 u32DmxId;
    HI_U32 u32ChnId;
    HI_U32 u32ChnPid;
    HI_DMX_DBG_EVT_CHAN_DAT_TYPE_E enType;
    HI_U32 u32ParserLen;            /* 解析到的数据长度 */
    HI_U32 u32ValidLen;             /* 采纳的数据长度 */
    HI_U32 u32BufFromLoc;           /* 在buffer中的起点 */
    HI_U32 u32BufToLoc;             /* 复制到buffer中的起点 */
    HI_U32 u32EOPAddr;              /* 如果是MASK_DAV_DMX_INVALID_ADDR表示不是通过EOP截断的 */
    HI_U32 u32TSTotal;              /* TS包总数 */
    HI_U32 u32TSErrTotal;           /* TS错误包总数 */
    HI_U32 u32CallFlag;
    HI_U32 u32DataLen;
    HI_U8 au8Data[64];              /* u32DataLen字节长 */
} HI_DMX_DBG_EVT_CHAN_DAT_S;

typedef struct hiDMX_DBG_EVT_EOP_DAT_S
{
    HI_U32 u32DmxId;
    HI_U32 u32ChnId;
    HI_U32 u32ChnPid;
    HI_U32 u32DataLen;
    HI_U8 au8Data[]; /*lint !e1501*/
} HI_DMX_DBG_EVT_EOP_DAT_S;

typedef struct hiDMX_DBG_EVT_FILTER_S
{
    HI_U32 u32DmxId;
    HI_U32 u32FilterId;
    HI_U32 u32ChnId;
    HI_U32 u32Depth;
    HI_U8 au8Match[16*3];   /* u32Depth * 3 */
} HI_DMX_DBG_EVT_FILTER_S;

typedef struct hiDMX_DBG_EVT_DESCRAMBLER_S
{
    HI_U32 u32DmxId;
    HI_U32 u32DescramblerId;
    HI_U32 u32ChnId;
} HI_DMX_DBG_EVT_DESCRAMBLER_S;

typedef enum hiDMX_DBG_EVT_DESC_DAT_TYPE_E
{
    HI_DMX_DBG_EVT_DESC_DAT_EVEN,
    HI_DMX_DBG_EVT_DESC_DAT_ODD
} HI_DMX_DBG_EVT_DESC_DAT_TYPE_E;

typedef struct hiDMX_DBG_EVT_DESC_DAT_S
{
    HI_U32 u32DmxId;
    HI_U32 u32DescramblerId;
    HI_DMX_DBG_EVT_DESC_DAT_TYPE_E enType;
    HI_U8 au8Data[8];
} HI_DMX_DBG_EVT_DESC_DAT_S;

typedef union hiDMX_DBG_EVT_DATA_U
{
    HI_DMX_DBG_EVT_SPACE_S stSpace;
    HI_DMX_DBG_EVT_CHAN_S stChan;
    HI_DMX_DBG_EVT_CHAN_DAT_S stChanData;
    HI_DMX_DBG_EVT_FILTER_S stFilter;
    HI_DMX_DBG_EVT_DESCRAMBLER_S stDescrambler;
    HI_DMX_DBG_EVT_DESC_DAT_S stDescData;
} HI_DMX_DBG_EVT_DATA_U;

typedef struct hiDMX_DBG_EVT_REC_S
{
    HI_U32 u32Size;
    HI_U32 u32No;
    HI_U32 u32Time;
    HI_BOOL bError;         /* 识别出有错误发生 */
    HI_DMX_DBG_EVT_ACT_E enAct;
    HI_DMX_DBG_EVT_DATA_U stData;
} HI_DMX_DBG_EVT_REC_S;

#define HI_DMX_DBG_EVT_SPACE_SIZE   (sizeof(HI_DMX_DBG_EVT_REC_S) - sizeof(HI_DMX_DBG_EVT_DATA_U) + sizeof(HI_DMX_DBG_EVT_SPACE_S))

typedef struct tagDMX_CHAN_BUF_S
{
    HI_U32 u32DmxId;
    HI_U32 u32ChanId;
    HI_U32 u32PhyStart;
    HI_U32 u32VirStart;
    HI_U32 u32Size;
} DMX_CHAN_BUF_S;
typedef enum hiDMX_ATTR_FLAG_ACT_E
{
    HI_DMX_ATTR_FLAG_GET,
    HI_DMX_ATTR_FLAG_SET
} HI_DMX_ATTR_FLAG_ACT_E;
typedef struct tagDMX_ATTR_FLAG_S
{
    HI_DMX_ATTR_FLAG_ACT_E enAct;
    HI_DMX_ATTR_FLAG_TYPE_E enType;
    HI_U32 u32DmxId;
    HI_U32 u32ObjectId;
    HI_U32 u32AttrVol;
    HI_U32 u32AttrExpand;
} DMX_ATTR_FLAG_S;
#endif










#ifdef HI_DMX_CHAN_BUF_DIRECTLY
typedef struct hiDMX_TEST_INFO_S
{
    HI_U32  u32StructSize;          /* Input */
    HI_U32  u32DemuxBase;
    HI_U32  u32DemuxDavBase;
    HI_U32  u32DemuxTsPortBase;
    HI_U32  au32ErrRegStatus[HI_DMX_SUB_DEV_COUNT];     /*error flag which is registered by application*/
    HI_U32  au32BufOverFlow[HI_DMX_SUB_DEV_COUNT];      /*overflow status of buffer*/
    HI_U32  au32ErrIntStatus[HI_DMX_SUB_DEV_COUNT];     /*error interrupt status*/
    HI_U32  au32CrcIntStatus[HI_DMX_SUB_DEV_COUNT];     /*crc error status*/
    HI_U32  au32TsDisconIntStatus[HI_DMX_SUB_DEV_COUNT];/*ts count discontuity error status*/
    HI_U32  au32TeiIntStatus[HI_DMX_SUB_DEV_COUNT];     /*Tei error status*/
    HI_U32  au32TsErrIntStatus[HI_DMX_SUB_DEV_COUNT];   /*ts port error status*/
    HI_U32  au32PesLenIntStatus[HI_DMX_SUB_DEV_COUNT];  /*pes length error status*/
    HI_U32  au32DataFlag[HI_DMX_SUB_DEV_COUNT];
    DMX_TS_SOURCE_TYPE_E aenTsType[HI_DMX_SUB_DEV_COUNT];
    HI_U32  au32UsedChan[HI_DMX_SUB_DEV_COUNT];
    HI_U32  au32PortPacketCount[PORT_COUNT];
    HI_U32  au32PortErrorPacketCount[PORT_COUNT];
    HI_U32  u32UsedCommBufNum; /* the num of used common buffer*/
    HI_U32  u32IdleMsgListNum;
    HI_U32  u32UsedMsgListNum;
    HI_U32  u32RunSeconds;
} HI_DMX_TEST_INFO_S;
typedef struct hiDMX_TEST_CHAN_INFO_S
{
    HI_U32                  u32StructSize;          /* Input */
    HI_U32                  u32DmxId;               /* Input */
    HI_U32                  u32ChanId;              /* Input */
    DMX_ChanType_E          enChanType;             /*channel type*/
    DMX_CHAN_FUNC_TYTE_E    enChanFuncType;
    DMX_CHAN_STATUS_E       enChanStatus;           /*channel status: open or close*/
    HI_U32                  u32ChanPID;             /*channel pid*/
    HI_U32                  u32IntLength;           /* 定长中断长度,字节数 */
    HI_U32                  u32KeyId;               /*key id which is attatched to a channel*/
    HI_U32                  u32LowFilterFlag;       /*  filter flag on a channel :0-31*/
    HI_U32                  u32HighFilterFlag;      /*  filter flag on a channel:32-47*/
    DMX_FlushStatus_E       eFlushStatus;           /*status of flushing channel*/
    DMX_FLUSH_TYPE_E        eFlushType;
    HI_U32                  u32BufId;
    HI_U32                  u32BufSize;
    HI_U32                  u32HalfExpand;
    HI_U32                  u32BufDataLen;          /* 数据字节数 */
    HI_U32                  u32BufFreeLen;          /* 空闲字节数 */
    HI_U32                  u32BufReadPtr;          /* 读指针 */
    HI_U32                  u32BufWritePtr;         /* 写指针 */
    HI_U32                  u32WriteTime;           /* 更新写指针的时间 */
    HI_BOOL                 bBufDirect;             /* 是否直接读取通道Buffer */
    HI_U32                  u32BufPeakValue;        /* 使用的峰值 */
    HI_U32                  u32EOPAddr;             /* 最近的EOP地址 */
    HI_U32                  u32FixedIntAddr;        /* 最近的定长中断地址 */
    HI_U32                  u32RegEOPAddr;          /* 寄存器中最近的EOP地址 */
    HI_U32                  u32RegFixedIntAddr;     /* 寄存器中最近的定长中断地址 */
    HI_U32                  u32OverflowIntCount;    /* 发现数据溢出中断的次数 */
    HI_U32                  u32OverflowPtrCount;    /* 从指针发现数据溢出的次数 */
    HI_U32                  u32EOPErrorCount;       /* 发生EOP错误次数 */
    HI_U32                  u32FixedErrorCount;     /* 发生定长地址错误次数 */
} HI_DMX_TEST_CHAN_INFO_S;
typedef struct hiDMX_TEST_FILTER_INFO_S
{
    HI_U32                  u32StructSize;          /* Input */
    HI_U32                  u32DmxId;               /* Input */
    HI_U32                  u32FilterId;            /* Input */
    DMX_FilterState_E       enState;                /*filter status*/
    HI_U32                  u32ChanID;              /*channel id about the filter*/
    HI_U32                  u32Depth;               /*filter depth*/
    HI_U8                   au8Match[DMX_FILTER_DEPTH]; /*match bytes of the filter*/
    HI_U8                   au8Mask[DMX_FILTER_DEPTH];  /*mask bytes of the filter*/
    HI_U8                   au8Negate[DMX_FILTER_DEPTH];/*negate bytes of the filter*/
} HI_DMX_TEST_FILTER_INFO_S;
typedef struct hiDMX_TEST_KEYSET_INFO_S
{
    HI_U32                  u32StructSize;          /* Input */
    HI_U32                  u32DmxId;               /* Input */
    HI_U32                  u32KeyId;               /* Input */
    DMX_KeySetState_E       enState;                /*key status*/
    HI_U32                  u32ChanFlag;
} HI_DMX_TEST_KEYSET_INFO_S;
typedef struct hiDMX_TEST_RECORD_INFO_S
{
    HI_U32                  u32StructSize;          /* Input */
    HI_U32                  u32DmxId;               /* Input */
    DMX_RECORD_TYPE_E       enRecType;
    DMX_RECORD_STATUS_E     enRecStatus;
    HI_U32                  u32IndexForPid;
    HI_U32                  u32CurNum;
    HI_U32                  au32RecPid[DMX_RECORD_CHAN_NUM_MAX];
    HI_U32                  au32RecChanId[DMX_RECORD_CHAN_NUM_MAX];
} HI_DMX_TEST_RECORD_INFO_S;
typedef struct hiDMX_TEST_ALL_INFO_S
{
    HI_U32 u32StructSize;          /* Input */
    HI_DMX_TEST_INFO_S stSummary;
    HI_DMX_TEST_CHAN_INFO_S astChan[HI_DMX_SUB_DEV_COUNT][DMX_CHAN_COUNT];
    HI_DMX_TEST_FILTER_INFO_S astFilter[HI_DMX_SUB_DEV_COUNT][DMX_FILTER_COUNT];
    HI_DMX_TEST_KEYSET_INFO_S astKey[HI_DMX_SUB_DEV_COUNT][DMX_KEY_COUNT];
    HI_DMX_TEST_RECORD_INFO_S astRecord[HI_DMX_SUB_DEV_COUNT];
} HI_DMX_TEST_ALL_INFO_S;
typedef struct hiDMX_TEST_ALL_PROC_INFO_S
{
    HI_U8 *pu8Buf;                  /* Input, Output */
    HI_U32 u32BufLen;               /* Input, Output */
    HI_DMX_TEST_ALL_INFO_S *pAllInfo;   /* Input,如果pAllInfo为HI_NULL表示取最新数据 */
} HI_DMX_TEST_ALL_PROC_INFO_S;
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEMUX_TYPE_H__ */


