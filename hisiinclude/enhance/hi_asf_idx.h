/******************************************************************************
Copyright (C), 2004-2010, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_asf_idx.h
Version        : Initial 
Author         : Hisilicon multimedia software group
Created       : 2009-02-23
Last Modified :
Description   : Hisilicon ASF I frame Index file
Function List :
History        :
* Version   Date              Author                  DefectNum    Description
* main\1    2009-02-23   HongGuo Qiu         NULL             Create this file.
******************************************************************************/
#ifndef __HI_ASF_IDX_H__
#define __HI_ASF_IDX_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

//功能:创建索引文件。
//参数1:原始WMV文件名
//参数2:索引文件名
HI_S32 HI_UNF_ASF_CreateIdxFile(HI_U8 *SourceFile, HI_U8 *TargetFile);

//功能:打开索引文件。注意不要直接用fopen接口。
//参数:索引文件名
FILE *HI_UNF_ASF_OpenIdxFile(HI_U8 *FileName);

//功能:关闭索引文件。注意不要直接用fclose接口。
//参数:通过HI_UNF_ASF_OpenIdxFile返回的索引文件句柄
HI_S32 HI_UNF_ASF_CloseIdxFile(FILE *pIdx);

//功能:获取索引信息，包括整个播放的持续时间，和关键帧数量
//参数1:索引文件句柄
//参数2:文件播放总时间
//参数3:关键帧数量
HI_S32 HI_UNF_ASF_GetIdxInfo(FILE *pIdx, HI_U32 *DuartionInMS, HI_U32 *KeyFrmNum);

//功能:指定时间，获取关键帧
//参数1:索引文件句柄
//参数2:指定的播放时刻。单位豪秒。
//参数3:实际对应关键帧的播放时刻。单位毫秒。
//参数4:关键帧在原始文件中的偏移
//参数5:关键帧第一段数据的大小
//参数6:关键帧是否获取完整。如果IfOK为1，表明关键帧结束；如果IfOK为0，表明还有后续数据块。
//注意:关键帧在WMV原始文件中是被分割成若干段的。这里只返回第一块数据，后续数据块通过HI_UNF_ASF_GetKeyFrmNextBlk获取。
HI_S32 HI_UNF_ASF_GetKeyFrmByTime(FILE *pIdx, HI_U32 SeekTimeInMS, HI_U32 *PlayTimeInMS, HI_U32 *Offset, HI_U32 *Size, HI_U32 *IfOK);

//功能:获取上一个关键帧，用于快退
//参数1:索引文件句柄
//参数2:实际对应关键帧的播放时刻。单位毫秒。
//参数3:关键帧在原始文件中的偏移
//参数4:关键帧第一段数据的大小
//参数5:关键帧是否获取完整。
HI_S32 HI_UNF_ASF_GetPrevKeyFrm(FILE *pIdx, HI_U32 *PlayTimeInMS, HI_U32 *Offset, HI_U32 *Size, HI_U32 *IfOK);

//功能:获取下一个关键帧，用于快进
//参数1:索引文件句柄
//参数2:实际对应关键帧的播放时刻。单位毫秒。
//参数3:关键帧在原始文件中的偏移
//参数4:关键帧第一段数据的大小
//参数5:关键帧是否获取完整。
HI_S32 HI_UNF_ASF_GetNextKeyFrm(FILE *pIdx, HI_U32 *PlayTimeInMS, HI_U32 *Offset, HI_U32 *Size, HI_U32 *IfOK);

//功能:获取关键帧的后续数据块。
//参数1:索引文件句柄
//参数2:关键帧在原始文件中的偏移
//参数3:关键帧本段数据的大小
HI_S32 HI_UNF_ASF_GetKeyFrmNextBlk(FILE *pIdx, HI_U32 *Offset, HI_U32 *Size, HI_U32 *IfOK);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_ASF_IDX_H__ */

