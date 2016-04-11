/******************************************************************************
Copyright (C), 2004-2010, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_ts_idx.h
Version        : Initial 
Author         : Hisilicon multimedia software group
Created       : 2009-02-23
Last Modified :
Description   : Hisilicon ASF I frame Index file
Function List :
History        :
* Version   Date              Author                  DefectNum    Description
* 
******************************************************************************/
#ifndef __HI_TS_IDX_H__
#define __HI_TS_IDX_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct hiTSIDX_SEND_INFO
{
    HI_U32 u32SendInfo0;
    HI_U32 u32SendInfo1;
    HI_U32 u32SendInfo2;
    HI_U32 u32SendInfo3;
}TSIDX_SEND_INFO;

typedef enum
{
    TSIDX_TIME,
    TSIDX_OFFSET,
    TSIDX_FRAMENO,    
}TSIDX_POS_TYPE;

typedef struct
{
    HI_U64 u64Offset;
    HI_U32 u32Time;
    HI_U32 u32FrameNo;    
}TSIDX_POS_INFO;


typedef struct
{
    HI_U32 u32HddBufRemain;
    HI_U32 u32AudPesRemain;
    HI_U32 u32VidPesRemain;    
}TSIDX_BUF_INFO;


typedef enum
{
    TSIDX_FLUSH_HDD = 1,
    TSIDX_FLUSH_AUD_PES = 2,
    TSIDX_FLUSH_VID_PES = 4,        
}TSIDX_FLUSH_MODE;


//功能:创建索引文件。
//参数1:原始WMV文件名
//参数2:索引文件名
//参数3:建立索引的视频PID
HI_S32 HI_UNF_TSIDX_CreateIdxFile(HI_U8 *SourceFile, HI_U8 *TargetFile, HI_U32 VidPid);

//功能:打开索引文件。注意不要直接用fopen接口。
//参数:索引文件名
FILE *HI_UNF_TSIDX_OpenIdxFile(HI_U8 *FileName);

//功能:关闭索引文件。注意不要直接用fclose接口。
//参数:通过HI_UNF_ASF_OpenIdxFile返回的索引文件句柄
HI_S32 HI_UNF_TSIDX_CloseIdxFile(FILE *pIdx);


//功能:获取索引信息，包括整个播放的持续时间，和帧数量
//参数1:索引文件句柄
//参数2:文件播放总时间
//参数3:帧数量
HI_S32 HI_UNF_TSIDX_GetIdxInfo(FILE *pIdx, HI_U32 *DuartionInMS, HI_U32 *FrmNum);


//功能:获取文件位置信息，实现offset、time、frameno之间的转化
//参数1:索引文件句柄
//参数2:位置信息，可以是offset、time、frameno，通过Type标识
//参数3:pos类型
//参数3:获取的位置信息，包括offset、time、frameno
HI_S32 HI_UNF_TSIDX_GetPosInfo(FILE *pIdx, HI_U64 Pos, TSIDX_POS_TYPE Type, TSIDX_POS_INFO *PosInfo);


//功能:通过帧号获取位置信息
//参数1:索引文件句柄
//参数2:是否寻找邻近I帧，特技播放4倍速需要寻找邻近I帧
//参数3:帧号
//参数4:位置
//参数5:发送数据尺寸
//参数6:辅助信息,需要传给HI_UNF_TSIDX_PrepareToSend
HI_S32 HI_UNF_TSIDX_GetOffsetByFrameNo(FILE *pIdx, HI_BOOL IsKeyFrame, HI_U32 FrameNo, HI_U64 *Offset, HI_U32 *Size, TSIDX_SEND_INFO *SendInfo);


//功能:设置video的显示速度
//参数1:显示速度
HI_S32 HI_UNF_TSIDX_VidSetSpeed(HI_S32 Speed);



//功能:send之前的预处理
//参数1:Hdd buffer地址
//参数2:send的数据长度
//参数3:辅助信息，由接口HI_UNF_TSIDX_GetOffsetByFrameNo提供
HI_S32 HI_UNF_TSIDX_PrepareToSend(HI_U8* pTsAddr, HI_U32 ReadLen, TSIDX_SEND_INFO *SendInfo);



//功能:获取buffer余量
//参数1:buff余量结构体指针
HI_S32 HI_UNF_TSIDX_GetBufInfo(TSIDX_BUF_INFO *pBufInfo);


//功能:清buffer
//参数1:清除模式
HI_S32 HI_UNF_TSIDX_FlushBuf(TSIDX_FLUSH_MODE FlushMode);


//播放器控制接口
HI_S32 HI_UNF_TSIDX_AudPause();
HI_S32 HI_UNF_TSIDX_AudResume();


HI_S32 HI_UNF_TSIDX_VidPause();
HI_S32 HI_UNF_TSIDX_VidResume();




#if 0
//功能:指定时间，获取关键帧(I帧)
//参数1:索引文件句柄
//参数2:指定的播放时刻。单位豪秒。
//参数3:实际对应关键帧的播放时刻。单位毫秒。
//参数4:关键帧在原始文件中的偏移
//参数5:关键帧第一段数据的大小
//参数6:发送TS流所需关键信息
HI_S32 HI_UNF_TSIDX_GetKeyFrmByTime(FILE *pIdx, HI_U32 SeekTimeInMS, HI_U32 *PlayTimeInMS, HI_U64 *Offset, HI_U32 *Size, TSIDX_SEND_INFO *SendInfo);

//功能:获取上一个关键帧，用于快退
//参数1:索引文件句柄
//参数2:实际对应关键帧的播放时刻。单位毫秒。
//参数3:关键帧在原始文件中的偏移
//参数4:关键帧在TS文件中的大小
//参数5:发送TS流所需关键信息
HI_S32 HI_UNF_TSIDX_GetPrevKeyFrm(FILE *pIdx, HI_U32 *PlayTimeInMS, HI_U64 *Offset, HI_U32 *Size, TSIDX_SEND_INFO *SendInfo);

//功能:获取下一个关键帧，用于快进
//参数1:索引文件句柄
//参数2:实际对应关键帧的播放时刻。单位毫秒。
//参数3:关键帧在原始文件中的偏移
//参数4:关键帧在TS文件中的大小
//参数5:发送TS流所需关键信息
HI_S32 HI_UNF_TSIDX_GetNextKeyFrm(FILE *pIdx, HI_U32 *PlayTimeInMS, HI_U64 *Offset, HI_U32 *Size, TSIDX_SEND_INFO *SendInfo);

//功能:获取关键帧的后续数据块。
//参数1:索引文件句柄
//参数2:关键帧在原始文件中的偏移
//参数3:关键帧在TS文件中的大小
HI_S32 HI_UNF_TSIDX_GetKeyFrmNextBlk(FILE *pIdx, HI_U64 *Offset, HI_U32 *Size);


/*
函数介绍：根据文件偏移地址offset,查询对应的时间。
pIdx： 输入参数，索引文件句柄。
Offset：文件中相对于起始位置的偏移量。
TimeInMs: 单位毫秒，返回参数
*/
HI_S32 HI_UNF_TSIDX_GetCurrentTime(FILE *pIdx, HI_U64 Offset, HI_U32 *PlayTimeInMS);

/*
函数介绍：通过索引文件，输入跳转的时间点，返回对应文件的偏移地址。
如果是在某时刻播放应用场景, 建议使用接口: HI_UNF_TSIDX_GetKeyFrmByTime
indexfile_fd： 输入参数，索引文件句柄
TimeInMs：输入参数，播放时间，单位是毫秒
Offset ：返回参数，对应文件中的偏移地址。用于播放定位之用（fseek）
*/
HI_S32 HI_UNF_TSIDX_GetFileOffset(FILE *pIdx, HI_U32 PlayTimeInMS, HI_U64 *Offset);


/*
函数介绍：获取影片播放的长度时间。
pIdx： 输入参数，索引文件句柄
duration：返回参数，播放时间，单位是秒
*/
//HI_S32 HI_UNF_TSIDX_GetFileDuration(FILE *pIdx, HI_U32 *duration);


/*
函数介绍：设置快进快退倍速。
pIdx： 输入参数，索引文件句柄
Speed：输入参数，播放的倍速: -32 -16 -8 -4 -2-0 2 -4 8 16 32
*/
HI_S32 HI_UNF_TSIDX_SetFileFrameSpeed(FILE *pIdx,HI_S32 Speed, HI_U32 PlayTimeInMS);


/*
函数介绍：查询快进快退倍速。
indexfile_fd： 输入参数，索引文件句柄
playtimems：输入参数，播放的倍速: -32 -16 -8 -4 -2-0 2 -4 8 16 32
*/
HI_S32 HI_UNF_TSIDX_GetFileFrameSpeed(FILE *pIdx,HI_S32 *Speed);

#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_TS_IDX_H__ */


